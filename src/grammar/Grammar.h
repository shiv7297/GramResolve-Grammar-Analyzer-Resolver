#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "Production.h"

class Grammar {
private:
    std::string startSymbol;
    std::set<std::string> terminals;
    std::set<std::string> nonTerminals;
    std::vector<Production> productions;

public:
    Grammar() = default;

    void loadFromFile(const std::string &filename);
    void addProduction(const Production &p);
    void display() const;

    // Getters
    const std::string& getStartSymbol() const { return startSymbol; }
    const std::set<std::string>& getTerminals() const { return terminals; }
    const std::set<std::string>& getNonTerminals() const { return nonTerminals; }
    const std::vector<Production>& getProductions() const { return productions; }

    // Setters
    void setStartSymbol(const std::string &s) { startSymbol = s; }
};

#endif
