#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include <vector>
#include <string>
#include <sstream>
#include "../analysis/conflictDetector.h"
#include "../grammar/Grammar.h"

class ConflictResolver {
public:
    // Main resolver entry
    static void resolveConflicts(
        const std::vector<Conflict> &conflicts,
        const Grammar &grammar
    );

private:
    // 🔹 Add missing declarations
    static void appendLL1Explanation(
        std::ostringstream &out,
        const Conflict &c,
        const Grammar &grammar
    );

    static void appendLRExplanation(
        std::ostringstream &out,
        const Conflict &c
    );
};

#endif
