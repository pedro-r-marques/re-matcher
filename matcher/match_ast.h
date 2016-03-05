//
//  match_ast.h
//  matcher
//
//  Created by Pedro Roque Marques on 3/4/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#ifndef match_ast_h
#define match_ast_h

#include <memory>
#include <string>
#include <vector>

class MatchAtom {
public:
    virtual ~MatchAtom();
    // Repr returns a string representation of the match.
    virtual std::string Repr() = 0;

protected:
    MatchAtom();

private:
    MatchAtom(const MatchAtom &) = delete;
};

class MatchGroup : public MatchAtom {
public:
    enum Cardinality {
        MATCH_ONE,
        MATCH_OPTIONAL,             // ?
        MATCH_PLUS,                 // +
        MATCH_WILDCARD,             // *
        MATCH_WILDCARD_NONGREEDY    // (?*)
    };
    MatchGroup(MatchAtom *atom, Cardinality cardinality);
    virtual std::string Repr();

private:
    std::unique_ptr<MatchAtom> atom_;
    Cardinality cardinality_;
};

class MatchSequence : public MatchAtom {
public:
    ~MatchSequence();

    virtual std::string Repr();
    
    void add(MatchAtom *atom);
    
private:
    std::vector<MatchAtom *> sequence_;
};

class MatchOr : public MatchAtom {
public:
    ~MatchOr();
    virtual std::string Repr();
    
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
    virtual std::string Repr();

private:
    CharClass class_;
};

class MatchString : public MatchAtom {
public:
    MatchString(const std::string &piece);
    virtual std::string Repr();

private:
    std::string piece_;
};
#endif /* match_ast_h */
