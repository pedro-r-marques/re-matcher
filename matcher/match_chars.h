//
//  match_chars.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "match_atom.h"

class MatchChars : public MatchAtom {
public:
    enum CharClass {
        CHARS_INVALID,
        CHARS_SPACES,
        CHARS_NUMERIC,
        CHARS_RANGES,
    };

    MatchChars(CharClass charClass);
    virtual int Match(const char *str);
    virtual std::string Repr();
private:
    CharClass class_;
};