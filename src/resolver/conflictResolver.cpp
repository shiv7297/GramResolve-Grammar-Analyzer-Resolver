// ===============================================================
// File: conflictResolver.cpp
// Description: Provides explanations and possible fixes for conflicts
// ===============================================================

#include "conflictResolver.h"
#include "../report/reportWriter.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// ===============================================================
// Main resolver function
// ===============================================================
void ConflictResolver::resolveConflicts(
    const vector<Conflict> &conflicts,
    const Grammar &/* grammar */
) {
    ostringstream out;

    if (conflicts.empty()) {
        out << "✅ No conflicts to resolve.\n";
        cout << out.str();
        ReportWriter::get() << out.str();
        return;
    }

    out << "\n🧩 Attempting to resolve or explain conflicts...\n";
    out << string(80, '=') << "\n";

    cout << out.str();
    ReportWriter::get() << out.str();
    out.str("");

    for (const auto &c : conflicts) {
        ostringstream block;

        block << "🔹 Conflict Type: " << c.type << "\n";
        block << "   Location: " << c.location << "\n";

        // LL(1) or LR conflict?
        if (c.type.find("LL(1)") != string::npos)
            appendLL1Explanation(block, c);
        else
            appendLRExplanation(block, c);

        block << string(80, '-') << "\n";

        cout << block.str();
        ReportWriter::get() << block.str();
    }
}

// ===============================================================
// Produce explanation for LL(1) conflicts
// ===============================================================
void ConflictResolver::appendLL1Explanation(
    ostringstream &out,
    const Conflict &c
) {
    out << "   🔸 Likely Cause: Overlapping FIRST/FOLLOW sets or ambiguous productions.\n";
    out << "   🔧 Suggested Fixes:\n";
    out << "      • Try left-factoring to eliminate common prefixes.\n";
    out << "      • Remove or restructure ε-productions that cause overlap.\n";
    out << "      • Ensure FIRST(A) ∩ FOLLOW(A) = ∅ when A → ε exists.\n";

    out << "   Productions involved:\n";
    for (const string &d : c.details)
        out << "      → " << d << "\n";
}

// ===============================================================
// Produce explanation for LR conflicts
// ===============================================================
void ConflictResolver::appendLRExplanation(
    ostringstream &out,
    const Conflict &c
) {
    out << "   🔸 Likely Cause: Grammar ambiguity or insufficient lookahead.\n";

    if (c.type.find("Shift/Reduce") != string::npos) {
        out << "   🔧 Suggested Fixes:\n";
        out << "      • Add operator precedence or associativity rules.\n";
        out << "      • Refactor grammar to avoid constructs like dangling-else.\n";
        out << "      • Remove ambiguity by rewriting productions.\n";
    }
    else if (c.type.find("Reduce/Reduce") != string::npos) {
        out << "   🔧 Suggested Fixes:\n";
        out << "      • Ensure only one valid reduction can occur for each lookahead.\n";
        out << "      • Split or reorganize overlapping productions.\n";
    }
    else {
        out << "   🔧 Suggested Fixes:\n";
        out << "      • Inspect closure and GOTO computations.\n";
        out << "      • Check if FOLLOW sets cause conflicting reduce placements.\n";
    }

    out << "   Actions involved:\n";
    for (const string &d : c.details)
        out << "      → " << d << "\n";
}
