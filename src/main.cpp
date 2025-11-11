// ===============================================================
// File: main.cpp
// Description: Driver for Grammar Analyzer, LL(1) & LR(0) Parsers
// ===============================================================

#include "grammar/Grammar.h"
#include "analysis/FirstFollow.h"
#include "analysis/conflictDetector.h"
#include "parser/LL1Parser.h"
#include "parser/LRParser.h"
#include "resolver/conflictResolver.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // Step 1: Load grammar
    Grammar grammar;
    if (!grammar.loadFromFile("data/sample_grammar.txt")) {
        cerr << "❌ Error: cannot open grammar file.\n";
        return 1;
    }

    cout << "\n================= GRAMMAR LOADED =================\n";
    grammar.display();

    // Step 2: Compute FIRST & FOLLOW sets
    FirstFollowEngine ff;
    ff.computeFIRST(grammar);
    ff.computeFOLLOW(grammar);

    cout << "\n================= FIRST & FOLLOW =================\n";
    ff.display();

    // Step 3: Build LL(1) Parsing Table
    cout << "\n================= LL(1) PARSER ===================\n";
    LL1Parser ll1Parser(grammar, ff);
    ll1Parser.buildTable();
    ll1Parser.displayTable();

    // Step 4: Parse an example input automatically
    vector<string> inputTokens = {"id", "+", "id", "*", "id", "$"};
    cout << "\nUsing automatic test input: id + id * id $\n";
    ll1Parser.parse(inputTokens);

    // Step 5: Run LR(0) Parser
    cout << "\n\n================= LR(0) PARSER ===================\n";
    LR0Parser lr0(grammar);
    lr0.buildAutomaton();
    lr0.displayStates();

    cout << "\nParsing same input using LR(0)...\n";
    lr0.parse(inputTokens);

    // ---------------------------------------------------------------
    // Step 6: Conflict Detection (LL(1) + LR(0))
    // ---------------------------------------------------------------
    cout << "\n\n================= CONFLICT DETECTOR =================\n";

    // ---------- LL(1) Conflict Detection ----------
    auto ll1Conflicts = ConflictDetector::detectLL1Conflicts(
        grammar, ff, ll1Parser.getTable()
    );

    cout << "\n----- 🔍 LL(1) Conflict Report -----\n";
    if (ll1Conflicts.empty())
        cout << "✅ No LL(1) conflicts found!\n";
    else
        ConflictDetector::displayConflicts(ll1Conflicts);

    // ---------- LR(0)/SLR Conflict Detection ----------
    auto lrConflicts = ConflictDetector::detectLRConflicts(
        lr0.getACTION()
    );

    cout << "\n----- 🔍 LR(0)/SLR Conflict Report -----\n";
    if (lrConflicts.empty())
        cout << "✅ No LR(0)/SLR conflicts found!\n";
    else
        ConflictDetector::displayConflicts(lrConflicts);

    // Step 7: Conflict Resolution / Explanation
    cout << "\n\n================= CONFLICT RESOLVER =================\n";
    ConflictResolver::resolveConflicts(ll1Conflicts, grammar);
    ConflictResolver::resolveConflicts(lrConflicts, grammar);


    // ---------- Summary ----------
    cout << "\n==================================================\n";
    cout << "✅ Parsing and Conflict Analysis completed!\n";
    cout << "==================================================\n";

    return 0;
}
