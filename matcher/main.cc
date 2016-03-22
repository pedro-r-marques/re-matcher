//
//  main.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "matcher.h"

#include <array>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

bool test_matcher() {
    struct MatcherTest {
        const char *expr;
        vector<string> match;
        vector<string> no_match;
    } const tests[] = {
        {
            "(foo|bar)\\s+([0-9]+)",
            {"foo 1", "bar 2", "foo  3"},
            {"baz 1", "fo 2", "bar3", "foo a"}
        },
        {
            "a(bb)+a",
            {"abba", "abbbba"},
            {"aa", "aba", "abbba"},
        },
        {
            "a?a?a?aaa",
            {"aaa", "aaaa"},
            {"aa"}
        },
        {
            "ab*a",
            {"aba", "abba", "aa"},
            {"aca"}
        },
        {
            "a(b|c)+d",
            {"abd", "abcd", "abccd"},
            {"ad", "axd"},
        },
        {
            "(a|b)",
            {"a", "b"},
            {"c"},
        },
        {
            "ab(cd)+c",
            {"abcdc"},
            {"abc"},
        },
        {
            "(a+a+)+b",
            {"aab", "aaab"},
            {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac", "aaac"},
        },
        {
            "(a|aa)+b",
            {"aaaaaaaaaaaaaaaaaaaaaab", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab"},
            {"aaaaad", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac"},
        }
    };
    bool success = true;
    
    for (int t = 0; t < sizeof(tests) / sizeof(struct MatcherTest); ++t) {
        const MatcherTest *test = &tests[t];
        std::unique_ptr<Matcher> m(Matcher::Parse(test->expr));
        
        for (int i = 0; i < test->match.size(); i++) {
            if (!m->Match(test->match[i].c_str())) {
                std::cerr << "match " << test->match[i] << " expected true, got false" << std::endl;
                success = false;
            }
        }
        for (int i = 0; i < test->no_match.size(); i++) {
            if (m->Match(test->no_match[i].c_str())) {
                std::cerr << "match " << test->no_match[i] << " expected false, got true" << std::endl;
                success = false;
            }
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
