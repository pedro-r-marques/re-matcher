//
//  match_atom.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_atom_h
#define match_atom_h

#include <string>

// MatchAtom defines the interface for match operations
class MatchAtom {
public:
    enum MatchType {
        MATCH_ONE,
        MATCH_PLUS,
        MATCH_WILDCARD,
        MATCH_WILDCARD_NONGREEDY
    };
    MatchAtom();
    virtual ~MatchAtom();
    virtual int Match(const char *str) = 0;

    // Repr returns a string representation of the match.
    virtual std::string Repr() = 0;

    void set_cardinality(MatchType cardinality) {
        cardinality_ = cardinality;
    }
    MatchType cardinality() {
        return cardinality_;
    }
private:
    MatchType cardinality_;
};
#endif /* match_atom_h */
