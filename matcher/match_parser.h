//
//  match_parse.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/5/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_parse_h
#define match_parse_h

class MatchAtom;

class MatchParser {
public:
    MatchAtom *Parse(const char *str);
};

#endif /* match_parse_h */
