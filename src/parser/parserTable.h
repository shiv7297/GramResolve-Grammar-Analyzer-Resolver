#ifndef PARSERTABLE_H
#define PARSERTABLE_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include "../grammar/Grammar.h"
#include "../analysis/FirstFollow.h"

/**
 * ParserTable class
 * -----------------
 * Builds and stores the LL(1) Parsing Table.
 * Can later be extended for LR(0)/LR(1) parsing.
 */
class ParserTable {
private:
    std::string type;  // e.g. "LL1", "LR0", "LR1"

    // Table[NonTerminal][Terminal] = set of Productions (to detect conflicts)
    std::map<std::string, std::map<std::string, std::set<std::string>>> table;

public:
    // Constructor
    explicit ParserTable(const std::string &t = "LL1") : type(t) {}

    /** Build LL(1) table using Grammar and FIRST/FOLLOW sets */
    void buildLL1Table(const Grammar &g, const FirstFollowEngine &ff);

    /** Insert an entry manually (useful for testing or custom parsing) */
    void insert(const std::string &nonTerm, const std::string &term, const std::string &prod) {
        table[nonTerm][term].insert(prod);
    }

    /** Check if a conflict exists in a particular cell */
    bool hasConflict(const std::string &nonTerm, const std::string &term) const {
        auto it = table.find(nonTerm);
        if (it == table.end()) return false;
        auto jt = it->second.find(term);
        return jt != it->second.end() && jt->second.size() > 1;
    }

    /** Print the parsing table neatly */
    void display() const {
        std::cout << "\n=== " << type << " Parsing Table ===\n";
        for (const auto &row : table) {
            const std::string &nonTerm = row.first;
            for (const auto &col : row.second) {
                const std::string &term = col.first;
                std::cout << "(" << nonTerm << ", " << term << ") -> ";
                for (const auto &prod : col.second)
                    std::cout << prod << " ";
                std::cout << "\n";
            }
        }
        std::cout << "==============================\n";
    }

    /** Accessors */
    const std::string &getType() const { return type; }
    const auto &getTable() const { return table; }

    /** Clear all entries */
    void clear() { table.clear(); }
};

#endif
