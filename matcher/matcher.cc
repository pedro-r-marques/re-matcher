//
//  matcher.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "matcher.h"

#include <iostream>
#include <memory>
#include <set>

#include "match_ast.h"
#include "match_parser.h"
#include "match_state.h"
#include "util.h"

using namespace std;

Matcher::Matcher() {
}

Matcher::~Matcher() {
    STLDeleteValues(&states_);
}


Matcher *Matcher::Parse(const char *str) {
    Matcher *m = new Matcher();
    MatchParser parser;
    std::unique_ptr<MatchAtom> rules(parser.Parse(str));
    cout << rules->Repr() << endl;
    m->BuildStateMachine(rules.get());
    return m;
}

static void expandStarTransition(const vector<MatchState *> *in, vector<MatchState *> *out) {
    vector <MatchState *> expand(in->begin(), in->end());
    set <MatchState *> seen;

    while (!expand.empty()) {
        vector<MatchState *> next;
        for (auto iter = expand.begin(); iter != expand.end(); ++iter) {
            const vector<MatchState *> *starPtr = (*iter)->GetCharTransition(MatchState::TRANSITION_STAR);
            if (starPtr == nullptr) {
                continue;
            }
            seen.insert(*iter);
            out->insert(out->end(), starPtr->begin(), starPtr->end());
            for (auto desc = starPtr->begin(); desc != starPtr->end(); ++desc) {
                if (seen.find(*desc) == seen.end()) {
                    next.push_back(*desc);
                }
            }
        }
        expand = next;
    }
}

vector<MatchState *> dedup(vector <MatchState *> input) {
    vector<MatchState *> output;
    set<MatchState *> unique;
    for (auto iter = input.begin(); iter != input.end(); ++iter) {
        if (unique.find((*iter)) != unique.end()) {
            continue;
        }
        output.push_back((*iter));
        unique.insert((*iter));
    }
    return output;
}

bool Matcher::Match(const char *str) {
    vector<MatchState *> feasible;
    set<MatchState *> plusSet;
    feasible.push_back(states_.front());

    const char *current = str;
    for (char ch = *current++; ch; ch = *current++) {
        vector<MatchState *> epsilon;
        vector<MatchState *> next;

        expandStarTransition(&feasible, &epsilon);
        
        for (auto iter = feasible.begin(); iter != feasible.end(); ++iter) {
            const vector<MatchState *> *starPtr = (*iter)->GetCharTransition(MatchState::TRANSITION_PLUS);
            if (starPtr == nullptr) {
                continue;
            }
            if (plusSet.find((*iter)) != plusSet.end()) {
                epsilon.insert(epsilon.end(), starPtr->begin(), starPtr->end());
                expandStarTransition(starPtr, &epsilon);
            }
        }

        epsilon = dedup(epsilon);
        feasible.insert(feasible.end(), epsilon.begin(), epsilon.end());

        for (auto iter = feasible.begin(); iter != feasible.end(); ++iter) {
            const vector<MatchState *> *charPtr = (*iter)->GetCharTransition(ch);
            if (charPtr != nullptr) {
                next.insert(next.begin(), charPtr->begin(), charPtr->end());
                plusSet.insert(*iter);
                
            }
        }

        next = dedup(next);
        feasible.swap(next);
    }

    vector<MatchState *> epsilon;
    for (auto iter = feasible.begin(); iter != feasible.end(); ++iter) {
        const vector<MatchState *> *starPtr = (*iter)->GetCharTransition(MatchState::TRANSITION_PLUS);
        if (starPtr == nullptr) {
            continue;
        }
        if (plusSet.find((*iter)) != plusSet.end()) {
            epsilon.insert(epsilon.end(), starPtr->begin(), starPtr->end());
        }
    }
    feasible.insert(feasible.end(), epsilon.begin(), epsilon.end());

    for (auto iter = feasible.begin(); iter != feasible.end(); ++iter) {
        if ((*iter)->name() == "<end>") {
            return true;
        }
    }
    return false;
}

static void displaySM(const MatchState *state) {
    cout << "state: " << state->name() << " " << state << endl;
}

void Matcher::BuildStateMachine(const MatchAtom *ast) {
    MatchState *start = new MatchState("<start>");
    MatchState *end = new MatchState("<end>");
    states_.push_back(start);
    states_.push_back(end);
    ast->BuildStateMachine(&states_, start, end);

    MatchState::BFSWalk(states_[0], displaySM);
}