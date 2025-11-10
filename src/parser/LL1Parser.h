#ifndef LL1PARSER_H
#define LL1PARSER_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include "../grammar/Grammar.h"
#include "../analysis/FirstFollow.h"

// 🔷 LL(1) Parser generator and simulator
class LL1Parser {
private:
    const Grammar &grammar;             // Reference to the grammar
    const FirstFollowEngine &ff;        // Reference to FIRST/FOLLOW sets
    std::map<std::string, std::map<std::string, std::string>> parsingTable; // Table[NonTerm][Term] = RHS

public:
    // Default constructor (if needed for temporary instances)
    LL1Parser() = delete; // disallow empty construction

    // Constructor binds grammar and first/follow engine
    LL1Parser(const Grammar &g, const FirstFollowEngine &f)
        : grammar(g), ff(f) {}

    // 🧱 Build LL(1) parsing table (no args needed; uses internal grammar + ff)
    void buildTable();

    // 🧩 Compute FIRST(α) where α is a RHS symbol sequence
    std::set<std::string> computeFirstOfString(const std::vector<std::string> &rhs) const;

    // 🪓 Join RHS symbols into a readable string
    std::string join(const std::vector<std::string> &rhs,
                     const std::string &sep) const;

    // ⚙️ Parse a token sequence using this table
    void parse(const std::vector<std::string> &tokens) const;

    // 🧰 Split input string into tokens
    std::vector<std::string> tokenize(const std::string &input) const;

    // 🖨️ Display parsing table
    void displayTable() const {
        std::cout << "\n=== LL(1) Parsing Table ===\n";
        for (const auto &row : parsingTable)
            for (const auto &col : row.second)
                std::cout << "(" << row.first << ", " << col.first
                          << ") => " << col.second << "\n";
        std::cout << "===========================\n";
    }
};

#endif
