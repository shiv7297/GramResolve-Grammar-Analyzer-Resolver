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

struct Conflict {
    std::string type;      // e.g., "LL(1) MULTIPLE ENTRIES" or "Shift/Reduce"
    std::string location;  // e.g., "(E, id)" or "(State 3, +)"
    std::vector<std::string> details; // Conflicting entries
};

class ConflictDetector {
public:
    // Detect LL(1) conflicts (multiple productions in same cell)
    static std::vector<Conflict> detectLL1Conflicts(
        const Grammar &grammar,
        const FirstFollowEngine &ff,
        const std::map<std::string, std::map<std::string, std::string>> &table
    );

    // Detect LR(0)/SLR conflicts from ACTION table
    static std::vector<Conflict> detectLRConflicts(
        const std::map<int, std::map<std::string, std::string>> &ACTION
    );

    // Nicely print detected conflicts
    static void displayConflicts(const std::vector<Conflict> &conflicts);
};

#endif
