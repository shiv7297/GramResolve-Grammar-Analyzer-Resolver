#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include "Production.h"

/**
 * @class Grammar
 * @brief Represents a formal grammar (used by analyzers & parsers)
 *
 * Handles reading grammar from file, storing productions,
 * classifying terminals/non-terminals, and providing helper utilities.
 */
class Grammar {
private:
    std::vector<Production> productions;     // All grammar rules
    std::set<std::string> terminals;         // Set of terminal symbols
    std::set<std::string> nonTerminals;      // Set of non-terminal symbols
    std::string startSymbol;                 // The grammar's start symbol

public:
    Grammar() = default;

    // 🔹 Grammar Management
    void addProduction(const Production &p);
    bool loadFromFile(const std::string &filename);
    void display() const;

    // 🔹 Accessors
    const std::vector<Production>& getProductions() const { return productions; }
    const std::set<std::string>& getTerminals() const { return terminals; }
    const std::set<std::string>& getNonTerminals() const { return nonTerminals; }
    const std::string& getStartSymbol() const { return startSymbol; }

    // 🔹 Classification Helpers
    bool isTerminal(const std::string &sym) const {
        return terminals.find(sym) != terminals.end();
    }

    bool isNonTerminal(const std::string &sym) const {
        return nonTerminals.find(sym) != nonTerminals.end();
    }
};

#endif
