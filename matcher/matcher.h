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

#include "match_atom.h"

class Matcher {
public:
    ~Matcher();
    static Matcher *Parse(const char *expr);
    bool Match(const char *str);
    
private:
    Matcher();
    Matcher(const Matcher &) = delete;
    void AddRules(std::vector<MatchAtom *>::iterator, std::vector<MatchAtom *>::iterator);
    std::vector<MatchAtom *> rules_;
};
#endif /* matcher_h */
