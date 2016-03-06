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
#include <vector>
#include <map>

class MatchState {
public:
    typedef std::map<char, std::vector<MatchState *> > EdgeMap;
    MatchState(const char *name);

    void AddStarTransition(MatchState *dst);
    void AddPlusTransition(MatchState *dst);
    void AddCharTransition(MatchState *dst, char c);

private:
    enum {
        TRANSITION_STAR = 0,
        TRANSITION_PLUS = 1
    };
    MatchState(const MatchState &) = delete;

    std::string name_;
    EdgeMap edges_;
};
#endif /* match_state_h */
