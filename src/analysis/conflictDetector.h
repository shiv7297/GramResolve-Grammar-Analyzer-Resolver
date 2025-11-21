// ===============================================================
// File: ConflictDetector.h
// Description: Detects conflicts in LL(1) and LR(0)/SLR parsing tables
// ===============================================================

#ifndef CONFLICT_DETECTOR_H
#define CONFLICT_DETECTOR_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../grammar/Grammar.h"
#include "../analysis/FirstFollow.h"

using namespace std;

struct Conflict {
    string type;               // conflict type
    string location;           // (NonTerminal, Terminal) or (State, Symbol)
    vector<string> details;    // conflicting rules/actions
};

class ConflictDetector {
public:

    // Detect LL(1) conflicts (multiple productions in same cell)
    // ⚠️ Clean version: removed unused Grammar & FirstFollowEngine parameters
    static vector<Conflict> detectLL1Conflicts(
        const map<string, map<string, string>> &table
    );

    // Detect LR(0)/SLR conflicts from ACTION table
    static vector<Conflict> detectLRConflicts(
        const map<int, map<string, string>> &ACTION
    );

    // Nicely print detected conflicts
    static void displayConflicts(const vector<Conflict> &conflicts);
};

#endif
