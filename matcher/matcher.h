//
//  matcher.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef matcher_h
#define matcher_h

#include <vector>

class MatchAtom;
class MatchState;

class Matcher {
public:
    ~Matcher();
    static Matcher *Parse(const char *expr);
    bool Match(const char *str);
    
private:
    Matcher();
    Matcher(const Matcher &) = delete;
    
    void BuildStateMachine(const MatchAtom *ast);

    std::vector<MatchState *> states_;
};
#endif /* matcher_h */
