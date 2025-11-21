#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include "Production.h"
using namespace std;

class Grammar {
private:
    vector<Production> productions;     // All grammar rules
    set<string> terminals;         // Set of terminal symbols
    set<string> nonTerminals;      // Set of non-terminal symbols
    string startSymbol;                 // The grammar's start symbol

public:
    Grammar() = default;

    // 🔹 Grammar Management
    void addProduction(const Production &p);
    bool loadFromFile(const string &filename);
    void display() const;

    // 🔹 Accessors
    const vector<Production>& getProductions() const { return productions; }
    const set<string>& getTerminals() const { return terminals; }
    const set<string>& getNonTerminals() const { return nonTerminals; }
    const string& getStartSymbol() const { return startSymbol; }

    // 🔹 Classification Helpers
    bool isTerminal(const string &sym) const {
        return terminals.find(sym) != terminals.end();
    }

    bool isNonTerminal(const string &sym) const {
        return nonTerminals.find(sym) != nonTerminals.end();
    }
};

#endif
