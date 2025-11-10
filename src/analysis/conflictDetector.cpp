#include "conflictDetector.h"
#include <set>
#include <map>
#include <sstream>

// -------------------------------------------------------------
// Detect LL(1) Conflicts
// -------------------------------------------------------------
void ConflictDetector::detectLL1Conflicts(const ParserTable &table) {
    ll1Conflicts.clear();

    // LL(1) conflict = multiple productions for same [NonTerminal, Terminal]
    for (const auto &row : table.getTable()) {
        const std::string &nonTerminal = row.first;
        const auto &entries = row.second;

        for (const auto &entry : entries) {
            const std::string &terminal = entry.first;
            const auto &actions = entry.second;

            // If more than one action is mapped to the same cell => conflict
            if (actions.size() > 1) {
                std::ostringstream oss;
                oss << "LL(1) conflict at [" << nonTerminal
                    << ", " << terminal << "]: { ";
                for (const auto &a : actions)
                    oss << a << " ";
                oss << "}";
                ll1Conflicts.push_back(oss.str());
            }
        }
    }
}

// -------------------------------------------------------------
// Detect LR Conflicts
// -------------------------------------------------------------
void ConflictDetector::detectLRConflicts(const ParserTable &table) {
    lrConflicts.clear();

    // Example detection: shift/reduce or reduce/reduce
    for (const auto &row : table.getTable()) {
        const std::string &state = row.first;
        const auto &actions = row.second;

        for (const auto &entry : actions) {
            const std::string &symbol = entry.first;
            const auto &ops = entry.second;

            bool hasShift = false, hasReduce = false;
            for (const auto &op : ops) {
                if (op.find("shift") != std::string::npos)
                    hasShift = true;
                if (op.find("reduce") != std::string::npos)
                    hasReduce = true;
            }

            if (hasShift && hasReduce) {
                std::ostringstream oss;
                oss << "Shift/Reduce conflict at state " << state
                    << ", symbol '" << symbol << "'";
                lrConflicts.push_back(oss.str());
            } else if (ops.size() > 1 && !hasShift) {
                std::ostringstream oss;
                oss << "Reduce/Reduce conflict at state " << state
                    << ", symbol '" << symbol << "'";
                lrConflicts.push_back(oss.str());
            }
        }
    }
}

// -------------------------------------------------------------
// Report All Conflicts
// -------------------------------------------------------------
void ConflictDetector::report() const {
    std::cout << "===== Conflict Report =====\n";

    if (ll1Conflicts.empty() && lrConflicts.empty()) {
        std::cout << "No grammar conflicts detected.\n";
    }

    if (!ll1Conflicts.empty()) {
        std::cout << "\n-- LL(1) Conflicts --\n";
        for (const auto &c : ll1Conflicts)
            std::cout << c << "\n";
    }

    if (!lrConflicts.empty()) {
        std::cout << "\n-- LR Conflicts --\n";
        for (const auto &c : lrConflicts)
            std::cout << c << "\n";
    }

    std::cout << "===========================\n";
}
