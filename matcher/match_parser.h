//
//  match_parse.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/5/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#pragma once

class MatchAtom;

class MatchParser {
public:
    MatchAtom *Parse(const char *str);
};

