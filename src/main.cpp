// ===============================================================
// File: main.cpp
// Description: Driver for Grammar Analyzer and LL(1) Parser
// ===============================================================

#include "grammar/Grammar.h"
#include "analysis/FirstFollow.h"
#include "parser/LL1Parser.h"
#include <iostream>

int main() {
    // Step 1: Load grammar
    Grammar grammar;
    if (!grammar.loadFromFile("data/sample_grammar.txt")) {
        std::cerr << "Error: cannot open grammar file.\n";
        return 1;
    }

    // Step 2: Compute FIRST & FOLLOW sets
    FirstFollowEngine ff;
    ff.computeFIRST(grammar);
    ff.computeFOLLOW(grammar);

    // Step 3: Display grammar and FIRST/FOLLOW
    grammar.display();
    ff.display();

    // Step 4: Build LL(1) parsing table
    LL1Parser parser(grammar, ff);
    parser.buildTable();

    // ✅ Step 4.5: Display LL(1) Table
    parser.displayTable(); // <-- Make sure LL1Parser has a displayTable() method

    // ✅ Step 5: Auto input test (no waiting)
    std::vector<std::string> inputTokens = {"id", "+", "id", "*", "id", "$"};
    std::cout << "\nUsing automatic test input: id + id * id $\n";

    parser.parse(inputTokens);

    return 0;
}
