//
//  match_string.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_string_h
#define match_string_h

#include <string>
#include "match_atom.h"

class MatchString : public MatchAtom {
public:
    MatchString(const std::string &piece);
    virtual int Match(const char *str);
    virtual std::string Repr();
private:
    std::string piece_;
};
#endif /* match_string_h */
