//
//  match_string.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_string.h"

using namespace std;

MatchString::MatchString(const string &piece) : piece_(piece) {
}

int MatchString::Match(const char *str) {
    if (piece_.find(str, 0, piece_.length()) == 0) {
        return (int) piece_.length();
    }
    return -1;
}

string MatchString::Repr() {
    return "string<" + piece_ + ">";
}