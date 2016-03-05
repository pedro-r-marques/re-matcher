//
//  match_state.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_state_h
#define match_state_h

#include <string>
#include <map>

class MatchState {
public:
    typedef std::map<char, MatchState *> EdgeMap;
    MatchState(const char *name);

private:
    MatchState(const MatchState &) = delete;

    std::string name_;
    EdgeMap edges_;
};
#endif /* match_state_h */
