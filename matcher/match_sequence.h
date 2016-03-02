//
//  match_sequence.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_sequence_h
#define match_sequence_h

#include <vector>
#include "match_atom.h"

class MatchSequence : public MatchAtom {
public:
    virtual int Match(const char *str);
    
    virtual std::string Repr();

    void add(MatchAtom *atom);

private:
    std::vector<MatchAtom *> sequence_;
};

#endif /* match_sequence_h */
