// ===============================================================
// File: conflictResolver.cpp
// Description: Provides explanations and possible fixes for conflicts
// ===============================================================

#include "conflictResolver.h"
#include <iostream>
#include <iomanip>
using namespace std;

void ConflictResolver::resolveConflicts(
    const vector<Conflict> &conflicts,
    const Grammar &grammar
) {
    if (conflicts.empty()) {
        cout << "✅ No conflicts to resolve.\n";
        return;
    }

    cout << "\n🧩 Attempting to resolve or explain conflicts...\n";
    cout << string(80, '=') << "\n";

    for (const auto &c : conflicts) {
        cout << "🔹 Conflict Type: " << c.type << "\n";
        cout << "   Location: " << c.location << "\n";

        if (c.type.find("LL(1)") != string::npos)
            explainLL1Conflict(c, grammar);
        else
            explainLRConflict(c);

        cout << string(80, '-') << "\n";
    }
}

void ConflictResolver::explainLL1Conflict(const Conflict &c, const Grammar &grammar) {
    cout << "   🔸 Likely Cause: Overlapping FIRST/FOLLOW sets or ambiguous productions.\n";
    cout << "   🔧 Suggestion: Try left-factoring or eliminating ε-productions.\n";
    cout << "   Productions involved:\n";
    for (const string &d : c.details)
        cout << "      → " << d << "\n";
}

void ConflictResolver::explainLRConflict(const Conflict &c) {
    cout << "   🔸 Likely Cause: Ambiguous grammar or missing operator precedence.\n";

    if (c.type.find("Shift/Reduce") != string::npos)
        cout << "   🔧 Suggestion: Add precedence/associativity rules or refactor grammar.\n";
    else if (c.type.find("Reduce/Reduce") != string::npos)
        cout << "   🔧 Suggestion: Remove duplicate reductions or refactor similar productions.\n";
    else
        cout << "   🔧 Suggestion: Check your state transitions and item closures.\n";

    cout << "   Actions involved:\n";
    for (const string &d : c.details)
        cout << "      → " << d << "\n";
}
