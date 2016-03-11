//
//  match_ast.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_ast.h"

#include "match_state.h"
#include "util.h"

using namespace std;

MatchAtom::MatchAtom() {
}

MatchAtom::~MatchAtom() {
}

MatchOr::~MatchOr() {
    STLDeleteValues(&alternatives_);
}

void MatchOr::add(MatchAtom *atom) {
    alternatives_.push_back(atom);
}

string MatchOr::Repr() const {
    string repr("OR <");
    for (auto iter = alternatives_.begin(); iter != alternatives_.end(); ++iter) {
        if (iter != alternatives_.begin()) {
            repr += ", ";
        }
        repr += (*iter)->Repr();
    }
    repr += ">";
    return repr;
}

void MatchOr::BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const {
    for (auto iter = alternatives_.begin(); iter != alternatives_.end(); ++iter) {
        (*iter)->BuildStateMachine(states, start, end);
    }
}

MatchSequence::~MatchSequence() {
    STLDeleteValues(&sequence_);
}

void MatchSequence::add(MatchAtom *atom) {
    sequence_.push_back(atom);
}

string MatchSequence::Repr() const {
    string repr("Sequence <");
    for (auto iter = sequence_.begin(); iter != sequence_.end(); ++iter) {
        if (iter != sequence_.begin()) {
            repr += ", ";
        }
        repr += (*iter)->Repr();
    }
    repr += ">";
    return repr;
}

void MatchSequence::BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const {
    MatchState *current = start;
    int count = 0;
    for (auto iter = sequence_.begin(); iter != sequence_.end(); ++iter) {
        MatchState *next;
        if (iter + 1 == sequence_.end()) {
            next = end;
        } else {
            string name("S");
            name += to_string(++count);
            next = new MatchState(name.c_str());
            states->push_back(next);
        }
        (*iter)->BuildStateMachine(states, current, next);
        current = next;
    }
}

MatchGroup::MatchGroup(MatchAtom *atom, Cardinality cardinality) : atom_(atom), cardinality_(cardinality) {
}

string MatchGroup::Repr() const {
    string repr("(");
    repr += atom_->Repr();
    repr += ")";
    switch (cardinality_) {
        case MATCH_PLUS:
            repr += "+";
            break;
        case MATCH_OPTIONAL:
            repr += "?";
            break;
        case MATCH_WILDCARD:
            repr += "*";
            break;
        default:
            break;
    }
    return repr;
}

void MatchGroup::BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const {
    switch (cardinality_) {
        case MATCH_PLUS:
            atom_->BuildStateMachine(states, start, start);
            start->AddPlusTransition(end);
            break;
        case MATCH_OPTIONAL:
            atom_->BuildStateMachine(states, start, end);
            start->AddStarTransition(end);
            break;
        case MATCH_WILDCARD:
            atom_->BuildStateMachine(states, start, start);
            start->AddStarTransition(end);
        default:
            atom_->BuildStateMachine(states, start, end);
            break;
    }
}

MatchChars::MatchChars(CharClass charClass) : class_(charClass) {
}

string MatchChars::Repr() const {
    string repr("chars ");
    switch (class_) {
        case CHARS_SPACES:
            repr += "<space>";
            break;
        case CHARS_NUMERIC:
            repr += "<digit>";
            break;
        default:
            repr += "<?>";
            break;
    }
    return repr;
}

void MatchChars::BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const {
    switch (class_) {
        case CHARS_SPACES:
            start->AddCharTransition(end, ' ');
            start->AddCharTransition(end, '\t');
            start->AddCharTransition(end, '\n');
            break;
        case CHARS_NUMERIC:
            for (int i = 0; i < 10; i++) {
                start->AddCharTransition(end, '0' + i);
            }
            break;
        default:
            break;
    }
}

MatchString::MatchString(const string &piece) : piece_(piece) {
}

string MatchString::Repr() const {
    return "string<" + piece_ + ">";
}

void MatchString::BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const {
    MatchState *current = start;
    for (int i = 0; i < piece_.length() - 1; i++) {
        string name;
        name.push_back(piece_[i]);
        MatchState *next = new MatchState(name.c_str());
        states->push_back(next);
        current->AddCharTransition(next, piece_[i]);
        current = next;
    }
    current->AddCharTransition(end, piece_[piece_.length() - 1]);
}
