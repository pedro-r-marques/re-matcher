//
//  match_or.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_or_h
#define match_or_h

#include <vector>

#include "match_atom.h"

class MatchOr : public MatchAtom {
public:
    virtual int Match(const char *str);
    virtual std::string Repr();

    void add(MatchAtom *atom);

private:
    std::vector<MatchAtom *> alternatives_;
};
#endif /* match_or_h */
