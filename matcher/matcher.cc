//
//  matcher.cc
//  matcher
//
//  Created by Pedro Roque Marques on 3/1/16.
//  Copyright © 2016 Code samples. All rights reserved.
//

#include "matcher.h"

#include <cassert>
#include <iostream>
#include <string>
#include <ctype.h>

#include "match_or.h"
#include "match_atom.h"
#include "match_string.h"
#include "match_sequence.h"
#include "match_chars.h"

using namespace std;

Matcher::Matcher() {
}

Matcher::~Matcher() {
    for (auto iter = rules_.begin(); iter != rules_.end(); ++iter) {
        delete *iter;
    }
}

// parseStack implements a stack which allows for multiple objects to be
// stored per frame and takes (memory) ownership of the objects.
class parseStack {
public:
    parseStack() {
    }
    ~parseStack() {
        for (auto iter = vec_.begin(); iter != vec_.end(); ++iter) {
            delete (*iter);
        }
    }
    // Push a new stack frame.
    void Push() {
        frames_.push_back(vec_.size());
    }
    
    // Pop the stack frame; flushes the variables on this stack frame.
    void Pop() {
        size_t index = frames_.back();
        frames_.pop_back();
        for (auto iter = vec_.begin() + index; iter != vec_.end(); ++iter) {
            delete (*iter);
        }
        vec_.erase(vec_.begin() + index, vec_.end());
    }
    
    void AddVariable(MatchAtom *atom) {
        vec_.push_back(atom);
    }

    MatchAtom *back() {
        if (vec_.size() == 0) {
            return nullptr;
        }
        return vec_.back();
    }

    // RemoveFrame clears this stack frame, transfering the ownership of the variables up
    // the stack.
    void RemoveFrame() {
        frames_.pop_back();
    }

    // Remove the variable at the head of this stack frame and tranfer the memory ownership out.
    MatchAtom *ReleaseHeadVariable() {
        size_t index = frames_.back();
        auto iter = vec_.begin() + index;
        if (iter == vec_.end()) {
            return nullptr;
        }
        MatchAtom *atom = (*iter);
        vec_.erase(iter);
        return atom;
    }

    size_t VariableCount() {
        return vec_.size() - frames_.back();
    }

    vector<MatchAtom *>::iterator begin() {
        return vec_.begin();
    }

    vector<MatchAtom *>::iterator end() {
        return vec_.end();
    }

    void clear() {
        vec_.clear();
        frames_.clear();
    }
private:
    vector<MatchAtom *> vec_;
    vector<size_t> frames_;
};

// grammar
//
// expr : expr_item expr
// expr_item: group | expr_alt_atom
// group: '(' expr_alt_atom ')'
// expr_alt_atom: expr_atom ('|' expr_atom) +
// expr_atom: expr_match expr_cardinality
// expr_match: expr_class | expr_interval | expr_string

class parseAtom {
public:
    typedef void acceptFunc(parseStack *, const string &piece);

    parseAtom(const char *name) : name_(name) {
    }

    virtual int Parse(parseStack *stack, const char *str) = 0;
    const char *name() {
        return name_.c_str();
    }
    size_t name_length() {
        return name_.length();
    }
    void set_accept_function(acceptFunc *func) {
        accept_ = func;
    }
protected:
    void Accept(parseStack *stack, const string &piece) {
        if (accept_ != nullptr) {
            (*accept_)(stack, piece);
        }
    }
private:
    string name_;
    acceptFunc *accept_;
};

template <class MatchOp>
static void reduceMatchOp(parseStack *stack) {
    // Add the contents of the stack to a MatchAtom Object
    MatchOp *var = new MatchOp();
    while (true) {
        MatchAtom *atom = stack->ReleaseHeadVariable();
        if (atom == NULL) {
            break;
        }
        var->add(atom);
    }
    stack->Pop();
    stack->AddVariable(var);
}

class parseAtomOptional : public parseAtom {
public:
    parseAtomOptional(const char *name, parseAtom *item) : parseAtom(name), item_(item) {
    }
    
    virtual int Parse(parseStack *stack, const char *str) {
        // cout << "enter optional " << name() << endl;
        stack->Push();
        int consumed = 0;
        while (str[0]) {
            int itemChars = item_->Parse(stack, str);
            if (itemChars <= 0) {
                break;
            }
            // cout << "accept " << name() << endl;
            consumed += itemChars;
            str += itemChars;
        }
        // cout << "exit optional " << name() << " bytes: " << consumed << endl;
        if (stack->VariableCount() > 1) {
            reduceMatchOp<MatchSequence>(stack);
        } else {
            stack->RemoveFrame();
        }

        return consumed;
    }
private:
    parseAtom *item_;
};

class parseAlternative : public parseAtom {
public:
    parseAlternative(const char *name) : parseAtom(name) {
    }

    virtual int Parse(parseStack *stack, const char *str) {
        // cout << "enter alternative " << name() << endl;
        for (auto iter = alternatives_.begin(); iter != alternatives_.end(); ++iter) {
            int result = (*iter)->Parse(stack, str);
            if (result > 0) {
                // cout << "accept alternative " << name() << endl;
                return result;
            }
        }
        // cout << "exit alternative " << name() << endl;
        return 0;
    }
    
    void add(parseAtom *atom) {
        alternatives_.push_back(atom);
    }
private:
    vector<parseAtom *> alternatives_;
};

class parseSequence : public parseAtom {
public:
    enum SequenceType {
        OP_INVALID,
        OP_AND,
        OP_OR,
    };
    parseSequence(const char *name, SequenceType type) : parseAtom(name), type_(type) {
    }
    void add(parseAtom *atom) {
        sequence_.push_back(atom);
    }

    virtual int Parse(parseStack *stack, const char *str) {
        // cout << "enter sequence " << name() << endl;
        stack->Push();
        int consumed = 0;
        const char *strp = str;
        for (auto iter = sequence_.begin(); iter != sequence_.end(); ++iter) {
            int item = (*iter)->Parse(stack, strp);
            if (item < 0) {
                // cout << "exit sequence " << name() << endl;
                stack->Pop();
                return -1;
            }
            consumed += item;
            strp += item;
        }
        // cout << "accept sequence " << name() << " piece: " << string(str, consumed) << endl;
        if (stack->VariableCount() > 1) {
            createMatchOperation(stack);
        } else {
            stack->RemoveFrame();
        }
        return consumed;
    }

private:

    void createMatchOperation(parseStack *stack) {
        switch (type_) {
            case OP_AND:
                reduceMatchOp<MatchSequence>(stack);
                break;
            case OP_OR:
                reduceMatchOp<MatchOr>(stack);
                break;
            case OP_INVALID:
                assert(false);
                break;
        }
    }

    vector<parseAtom *> sequence_;
    SequenceType type_;
};

class parseToken : public parseAtom {
public:
    parseToken(const char *token) : parseAtom(token) {
    }
    virtual int Parse(parseStack *stack, const char *str) {
        if (strncmp(str, name(), name_length()) == 0) {
            // cout << "token " << name() << endl;
            int lenght = (int) name_length();
            Accept(stack, string(str, lenght));
            return lenght;
        }
        return -1;
    }
};

class parseInterval : public parseAtom {
public:
    parseInterval(const char *name) : parseAtom(name) {
    }
    virtual int Parse(parseStack *stack, const char *str) {
        if (str[0] != '[') {
            return -1;
        }
        const char *endp = index(str, ']');
        if (endp == NULL) {
            return -1;
        }
        int consume = (int)(endp - str) + 1;
        string piece(str, consume);
        if (piece == "[0-9]") {
            stack->AddVariable(new MatchChars(MatchChars::CHARS_NUMERIC));
        }
        return consume;
    }
};

class parseString : public parseAtom {
public:
    parseString(const char *name) : parseAtom(name) {

    }
    virtual int Parse(parseStack *stack, const char *str) {
        int advance = 0;
        for (char c = str[0]; c; c = str[++advance]) {
            if (!isalnum(c)) {
                break;
            }
        }
        if (advance > 0) {
            stack->AddVariable(new MatchString(string(str, advance)));
        }
        return advance;
    }
};

static void MatchSetCardinalityPlus(parseStack *stack, const string &piece) {
    MatchAtom *atom = stack->back();
    if (atom == nullptr) {
        cerr << "Unable to set cardinality: empty stack" << endl;
        return;
    }
    atom->set_cardinality(MatchAtom::MATCH_PLUS);
}

static void MatchAddClassAtom(parseStack *stack, const string &piece) {
    if (piece == "\\s") {
        stack->AddVariable(new MatchChars(MatchChars::CHARS_SPACES));
    }
}

Matcher *Matcher::Parse(const char *str) {
    parseInterval parseExprInterval("interval");
    parseString parseExprString("string");

    parseToken parseSpaceClass("\\s");
    parseSpaceClass.set_accept_function(MatchAddClassAtom);
    parseAlternative parseExprClass("class");
    parseExprClass.add(&parseSpaceClass);

    parseToken parseCardinalityPlus("+");
    parseCardinalityPlus.set_accept_function(MatchSetCardinalityPlus);
    parseAlternative parseCardinality("cardinality");
    parseCardinality.add(&parseCardinalityPlus);
    parseAtomOptional parseExprOptionalCardinality("opt_cardinality", &parseCardinality);

    // match: class | interval | string
    parseAlternative parseExprMatch("match");
    parseExprMatch.add(&parseExprClass);
    parseExprMatch.add(&parseExprInterval);
    parseExprMatch.add(&parseExprString);

    parseSequence parseExprAtom("atom", parseSequence::OP_AND);
    parseExprAtom.add(&parseExprMatch);
    parseExprAtom.add(&parseExprOptionalCardinality);

    parseToken leftParentisis("(");
    parseToken rightParentisis(")");
    parseToken tokenBar("|");

    parseSequence parseExprAltAtomOptionalSequence("alt_atom_optional_sequence", parseSequence::OP_OR);
    parseExprAltAtomOptionalSequence.add(&tokenBar);
    parseExprAltAtomOptionalSequence.add(&parseExprAtom);
    parseAtomOptional parseExprAltAtomOptional("alt_atom_optional", &parseExprAltAtomOptionalSequence);
    parseSequence parseExprAltAtom("alt_atom", parseSequence::OP_OR);
    parseExprAltAtom.add(&parseExprAtom);
    parseExprAltAtom.add(&parseExprAltAtomOptional);

    parseSequence parseGroup("group", parseSequence::OP_AND);
    parseGroup.add(&leftParentisis);
    parseGroup.add(&parseExprAltAtom);
    parseGroup.add(&rightParentisis);


    parseAlternative parseExprItem("expr_item");
    parseExprItem.add(&parseGroup);
    parseExprItem.add(&parseExprAltAtom);
    
    parseAtomOptional parseExpr("expr", &parseExprItem);
    parseAtom *grammar = &parseExpr;
    Matcher *m = new Matcher();
    parseStack stack;
    grammar->Parse(&stack, str);
    m->AddRules(stack.begin(), stack.end());
    stack.clear();
    cout << "rules " << m->rules_.size() << endl;
    for (auto iter = m->rules_.begin(); iter != m->rules_.end(); ++iter) {
        string str = (*iter)->Repr();
        cout << str << endl;
    }
    return m;
}

bool Matcher::Match(const char *str) {
    for (auto iter = rules_.begin(); iter != rules_.end(); ++iter) {
        int result = (*iter)->Match(str);
        if (result > 0) {
            return true;
        }
    }
    return false;
}

void Matcher::AddRules(std::vector<MatchAtom *>::iterator begin, std::vector<MatchAtom *>::iterator end) {
    rules_.insert(rules_.begin(), begin, end);
}
