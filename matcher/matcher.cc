//
//  matcher.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "matcher.h"

//#include <cassert>
#include <iostream>
#include <memory>

#include "match_ast.h"
#include "match_parser.h"
#include "match_state.h"
#include "util.h"

using namespace std;

Matcher::Matcher() {
}

Matcher::~Matcher() {
    STLDeleteValues(&states_);
}


Matcher *Matcher::Parse(const char *str) {
    Matcher *m = new Matcher();
    MatchParser parser;
    std::unique_ptr<MatchAtom> rules(parser.Parse(str));
    cout << rules->Repr() << endl;
    m->BuildStateMachine(rules.get());
    return m;
}

bool Matcher::Match(const char *str) {
    return false;
}

void Matcher::BuildStateMachine(const MatchAtom *ast) {
    MatchState *start = new MatchState("<start>");
    MatchState *end = new MatchState("<end>");
    states_.push_back(start);
    states_.push_back(end);
    ast->BuildStateMachine(&states_, start, end);
}