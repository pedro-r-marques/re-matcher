//
//  main.cpp
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include <iostream>
#include <memory>

#include "matcher.h"

bool test_matcher() {
    const char *test_matches[] = {
        "foo 1",
        "bar 2",
        "foo  3",
    };
    const char *test_no_matches[] = {
        "baz 1",
        "fo 2",
        "bar3",
        "foo a"
    };

    struct MatcherTest {
        const char *expr;
        const char **match;
        const char **no_match;
    } test = {
        "(foo|bar)\\s+([0-9]+)",
        test_matches,
        test_no_matches
    };
    bool success = true;
    
    std::unique_ptr<Matcher> m(Matcher::Parse(test.expr));
    for (int i = 0; i < sizeof(test_matches) / sizeof(char *); i++) {
        if (!m->Match(test.match[i])) {
            std::cerr << "match " << test.match[i] << " expected true, got false" << std::endl;
            success = false;
        }
    }
    for (int i = 0; i < sizeof(test_no_matches) / sizeof(char *); i++) {
        if (m->Match(test.no_match[i])) {
            std::cerr << "match " << test.no_match[i] << " expected false, got true" << std::endl;
            success = false;
        }
    }
    return success;
}

int main(int argc, const char * argv[]) {
    if (!test_matcher()) {
        return -1;
    }
    return 0;
}
