//
//  match_chars.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//
#include "match_chars.h"

#include <ctype.h>

using namespace std;

MatchChars::MatchChars(CharClass charClass) : class_(charClass) {
}

int MatchChars::Match(const char *str) {
    int count = 0;
    for (char ch = str[count]; ch; ch = str[++count]) {
        if (count > 0 && cardinality() == MATCH_ONE) {
            break;
        }
        bool match = false;
        switch (class_) {
            case CHARS_SPACES:
                match = isspace(ch) != 0;
                break;

            case CHARS_NUMERIC:
                match = isnumber(ch) != 0;
            default:
                break;
        }
        if (!match) {
            break;
        }
    }
    if (count == 0) {
        switch(cardinality()) {
        case MatchAtom::MATCH_ONE:
        case MatchAtom::MATCH_PLUS:
            return -1;
        default:
            break;
        }
    }
    return count;
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
    if (cardinality() == MATCH_PLUS) {
        repr += "+";
    }
    return repr;
}