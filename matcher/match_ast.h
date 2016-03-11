//
//  match_ast.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

class MatchState;

// MatchAtom is the abstract base class for the objects in the abstract syntax tree.
class MatchAtom {
public:
    MatchAtom();
    MatchAtom(const MatchAtom &) = delete;
    MatchAtom &operator=(const MatchAtom &) = delete;

    virtual ~MatchAtom();

    // Repr returns a string representation of the match.
    virtual std::string Repr() const = 0;

    // Create the state machine representation corresponding to this object.
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const = 0;

};

class MatchGroup : public MatchAtom {
public:
    enum Cardinality {
        MATCH_ONE,
        MATCH_OPTIONAL,             // ?
        MATCH_PLUS,                 // +
        MATCH_WILDCARD,             // *
    };
    MatchGroup(MatchAtom *atom, Cardinality cardinality);
    virtual std::string Repr() const;
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const;

private:
    std::unique_ptr<MatchAtom> atom_;
    Cardinality cardinality_;
};

class MatchSequence : public MatchAtom {
public:
    virtual ~MatchSequence();

    virtual std::string Repr() const;
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const;

    void add(MatchAtom *atom);
    
private:
    std::vector<MatchAtom *> sequence_;
};

class MatchOr : public MatchAtom {
public:
    virtual ~MatchOr();
    virtual std::string Repr() const;
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const;

    void add(MatchAtom *atom);
    
private:
    std::vector<MatchAtom *> alternatives_;
};

class MatchChars : public MatchAtom {
public:
    enum CharClass {
        CHARS_INVALID,
        CHARS_SPACES,
        CHARS_NUMERIC,
        CHARS_RANGES,
    };
    MatchChars(CharClass charClass);
    virtual std::string Repr() const;
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const;

private:
    CharClass class_;
};

class MatchString : public MatchAtom {
public:
    MatchString(const std::string &piece);
    virtual std::string Repr() const;
    virtual void BuildStateMachine(std::vector<MatchState *> *states, MatchState *start, MatchState *end) const;

private:
    std::string piece_;
};
