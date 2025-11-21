#ifndef LL1PARSER_H
#define LL1PARSER_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "../grammar/Grammar.h"
#include "../analysis/FirstFollow.h"

using namespace std;

// 🔷 LL(1) Parser generator and simulator
class LL1Parser {
private:
    const Grammar &grammar;             // Reference to the grammar
    const FirstFollowEngine &ff;        // Reference to FIRST/FOLLOW sets
    map<string, map<string, string>> parsingTable; // Table[NonTerm][Term] = RHS

public:
    // Default constructor (disallowed)
    LL1Parser() = delete;

    // Constructor binds grammar and FIRST/FOLLOW engine
    LL1Parser(const Grammar &g, const FirstFollowEngine &f)
        : grammar(g), ff(f) {}

    // Accessor for table
    const map<string, map<string, string>>& getTable() const {
        return parsingTable;
    }

    // 🧱 Build LL(1) parsing table
    void buildTable();

    // 🧩 Compute FIRST(α)
    set<string> computeFirstOfString(const vector<string> &rhs) const;

    // 🪓 Join RHS symbols
    string join(const vector<string> &rhs, const string &sep) const;

    // ⚙️ Parse token sequence
    void parse(const vector<string> &tokens) const;

    // 🧰 Tokenize string
    vector<string> tokenize(const string &input) const;

    // 🖨️ Display LL(1) Parsing Table (Declaration Only!)
    void displayTable() const;   // <---- FIXED
};

#endif
