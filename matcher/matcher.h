//
//  matcher.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#pragma once

#include <vector>

class MatchAtom;
class MatchState;

class Matcher {
public:
    Matcher(const Matcher &) = delete;
    Matcher &operator=(const Matcher &) = delete;

    ~Matcher();
    static Matcher *Parse(const char *expr);
    bool Match(const char *str);
    
private:
    Matcher();

    void BuildStateMachine(const MatchAtom *ast);

    std::vector<MatchState *> states_;
};
