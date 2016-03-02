//
//  match_or.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_or.h"

using namespace std;

int MatchOr::Match(const char *str) {
    for (auto iter = alternatives_.begin(); iter != alternatives_.end(); ++iter) {
        int result = (*iter)->Match(str);
        if (result > 0) {
            return result;
        }
    }
    return -1;
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