//
//  match_ast.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_ast.h"

#include "util.h"

using namespace std;

MatchAtom::MatchAtom() {
}

MatchAtom::~MatchAtom() {
}

MatchOr::~MatchOr() {
    STLDeleteValues(&alternatives_);
}

MatchGroup::MatchGroup(MatchAtom *atom, Cardinality cardinality) : atom_(atom), cardinality_(cardinality) {
}

string MatchGroup::Repr() {
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

void MatchOr::add(MatchAtom *atom) {
    alternatives_.push_back(atom);
}

string MatchOr::Repr() {
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

MatchSequence::~MatchSequence() {
    STLDeleteValues(&sequence_);
}

void MatchSequence::add(MatchAtom *atom) {
    sequence_.push_back(atom);
}

string MatchSequence::Repr() {
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

MatchChars::MatchChars(CharClass charClass) : class_(charClass) {
}

MatchString::MatchString(const string &piece) : piece_(piece) {
}

string MatchString::Repr() {
    return "string<" + piece_ + ">";
}

string MatchChars::Repr() {
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