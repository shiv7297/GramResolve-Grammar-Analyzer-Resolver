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
    // Default constructor (disallowed)
    LL1Parser() = delete;

    // Constructor binds grammar and FIRST/FOLLOW engine
    LL1Parser(const Grammar &g, const FirstFollowEngine &f)
        : grammar(g), ff(f) {}

    // Accessor for table
    const std::map<std::string, std::map<std::string, std::string>>& getTable() const {
        return parsingTable;
    }

    // 🧱 Build LL(1) parsing table
    void buildTable();

    // 🧩 Compute FIRST(α)
    std::set<std::string> computeFirstOfString(const std::vector<std::string> &rhs) const;

    // 🪓 Join RHS symbols
    std::string join(const std::vector<std::string> &rhs, const std::string &sep) const;

    // ⚙️ Parse token sequence
    void parse(const std::vector<std::string> &tokens) const;

    // 🧰 Tokenize string
    std::vector<std::string> tokenize(const std::string &input) const;

    // 🖨️ Display LL(1) Parsing Table (Declaration Only!)
    void displayTable() const;   // <---- FIXED
};

#endif
