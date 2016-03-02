//
//  match_sequence.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_sequence.h"

using namespace std;

int MatchSequence::Match(const char *str) {
    int consumed = 0;
    const char *strp = str;
    for (auto iter = sequence_.begin(); iter != sequence_.end(); ++iter) {
        int length = (*iter)->Match(strp);
        if (length < 0) {
            return -1;
        }
        consumed += length;
        strp += length;
    }
    return consumed;
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