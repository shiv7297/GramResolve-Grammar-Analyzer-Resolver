#ifndef PARSERTABLE_H
#define PARSERTABLE_H

#include <iostream>
#include <map>
#include <string>
#include <utility>

class ParserTable {
private:
    std::string type; // LL1, LR0, LR1
    std::map<std::pair<std::string, std::string>, std::string> table;

public:
    ParserTable(const std::string &t = "LL1") : type(t) {}

    void insert(const std::string &nonTerm, const std::string &term, const std::string &prod) {
        table[{nonTerm, term}] = prod;
    }

    void display() const {
        std::cout << "\n=== " << type << " Parsing Table ===\n";
        for (const auto &entry : table) {
            std::cout << "(" << entry.first.first << ", " << entry.first.second << ") -> "
                      << entry.second << "\n";
        }
        std::cout << "==============================\n";
    }

    const std::string& getType() const { return type; }
    const std::map<std::pair<std::string, std::string>, std::string>& getTable() const { return table; }
};

#endif
