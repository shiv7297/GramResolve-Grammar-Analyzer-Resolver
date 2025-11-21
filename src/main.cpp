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
#include "report/reportWriter.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int main() {

    // Start fresh report
    ReportWriter::get().clear();

    // Step 1: Load grammar
    Grammar grammar;
    if (!grammar.loadFromFile("data/sample_grammar.txt")) {
        cerr << "❌ Error: cannot open grammar file.\n";
        ReportWriter::get() << "❌ Error: cannot open grammar file.\n";
        return 1;
    }

    // Header section
    {
        std::ostringstream out;
        out << "\n================= GRAMMAR LOADED =================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    grammar.display();

    // Step 2: Compute FIRST & FOLLOW sets
    {
        std::ostringstream out;
        out << "\n================= FIRST & FOLLOW =================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    FirstFollowEngine ff;
    ff.computeFIRST(grammar);
    ff.computeFOLLOW(grammar);
    ff.display();

    // Step 3: Build LL(1) Parsing Table
    {
        std::ostringstream out;
        out << "\n================= LL(1) PARSER ===================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    LL1Parser ll1Parser(grammar, ff);
    ll1Parser.buildTable();
    ll1Parser.displayTable();

    // Step 4: Parse an example input automatically
    vector<string> inputTokens = {"id", "+", "id", "*", "id", "$"};

    {
        std::ostringstream out;
        out << "\nUsing automatic test input: id + id * id $\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    ll1Parser.parse(inputTokens);

    // Step 5: Run LR(0) Parser
    {
        std::ostringstream out;
        out << "\n\n================= LR(0) PARSER ===================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    LR0Parser lr0(grammar);
    lr0.buildAutomaton();
    lr0.displayStates();

    {
        std::ostringstream out;
        out << "\nParsing same input using LR(0)...\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    lr0.parse(inputTokens);

    // ---------------------------------------------------------------
    // Step 6: Conflict Detection (LL(1) + LR(0))
    // ---------------------------------------------------------------
    {
        std::ostringstream out;
        out << "\n\n================= CONFLICT DETECTOR =================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

  // ---------- LL(1) Conflict Detection ----------
auto ll1Conflicts = ConflictDetector::detectLL1Conflicts(
    ll1Parser.getTable()
);


    {
        std::ostringstream out;
        out << "\n----- 🔍 LL(1) Conflict Report -----\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    if (ll1Conflicts.empty()) {
        cout << "✅ No LL(1) conflicts found!\n";
        ReportWriter::get() << "✅ No LL(1) conflicts found!\n";
    } else {
        ConflictDetector::displayConflicts(ll1Conflicts);
    }

    // ---------- LR(0)/SLR Conflict Detection ----------
    auto lrConflicts = ConflictDetector::detectLRConflicts(
        lr0.getACTION()
    );

    {
        std::ostringstream out;
        out << "\n----- 🔍 LR(0)/SLR Conflict Report -----\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    if (lrConflicts.empty()) {
        cout << "✅ No LR(0)/SLR conflicts found!\n";
        ReportWriter::get() << "✅ No LR(0)/SLR conflicts found!\n";
    } else {
        ConflictDetector::displayConflicts(lrConflicts);
    }

    // Step 7: Conflict Resolution
    {
        std::ostringstream out;
        out << "\n\n================= CONFLICT RESOLVER =================\n";
        cout << out.str();
        ReportWriter::get() << out.str();
    }

    ConflictResolver::resolveConflicts(ll1Conflicts, grammar);
    ConflictResolver::resolveConflicts(lrConflicts, grammar);

    // Final summary
    {
        std::ostringstream out;
        out << "\n==================================================\n"
            << "✅ Parsing and Conflict Analysis completed!\n"
            << "==================================================\n";

        cout << out.str();
        ReportWriter::get() << out.str();
    }

    return 0;
}
