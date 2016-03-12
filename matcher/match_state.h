//
//  match_state.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <map>

// MatchState defines a NFA node.
class MatchState {
public:
    enum {
        // When a node becomes feasible, its wildcard transitions are (recursivly) expanded
        // without consuming any characters from the input stream.
        TRANSITION_STAR = 0,
        // Plus transitions are expandded if the node has been previously considered to be
        // feasible.
        TRANSITION_PLUS = 1
    };

    typedef std::map<char, std::vector<MatchState *> > EdgeMap;
    MatchState(const char *name);

    void AddStarTransition(MatchState *dst);
    void AddPlusTransition(MatchState *dst);
    void AddCharTransition(MatchState *dst, char c);

    const std::vector<MatchState *> *GetCharTransition(char c);

    const std::string &name() const {
        return name_;
    }

    static void BFSWalk(const MatchState *start, void (*visitor)(const MatchState *));

private:
    MatchState(const MatchState &) = delete;
    MatchState &operator=(const MatchState &) = delete;

    std::string name_;
    EdgeMap edges_;
};
