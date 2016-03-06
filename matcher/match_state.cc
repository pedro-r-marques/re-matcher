//
//  match_state.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_state.h"

using namespace std;

MatchState::MatchState(const char *name) : name_(name) {
}

void MatchState::AddStarTransition(MatchState *dst) {
    AddCharTransition(dst, TRANSITION_STAR);
}
void MatchState::AddPlusTransition(MatchState *dst) {
    AddCharTransition(dst, TRANSITION_PLUS);
}

void MatchState::AddCharTransition(MatchState *dst, char c) {
    vector<MatchState *> *vec;
    auto loc = edges_.find(c);
    if (loc == edges_.end()) {
        auto result = edges_.insert(make_pair(c, vector<MatchState *>()));
        vec = &((*result.first).second);
    } else {
        vec = &((*loc).second);
    }
    vec->push_back(dst);
}

const std::vector<MatchState *> *MatchState::GetCharTransition(char c) {
    auto loc = edges_.find(c);
    if (loc == edges_.end()) {
        return nullptr;
    }
    return &loc->second;
}