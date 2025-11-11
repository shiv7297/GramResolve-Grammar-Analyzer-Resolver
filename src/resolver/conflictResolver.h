// ===============================================================
// File: conflictResolver.h
// Description: Suggests solutions or explanations for parsing conflicts
// ===============================================================

#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include <vector>
#include <string>
#include "../analysis/conflictDetector.h"
#include "../grammar/Grammar.h"

class ConflictResolver {
public:
    // Analyze and suggest resolutions for conflicts
    static void resolveConflicts(
        const std::vector<Conflict> &conflicts,
        const Grammar &grammar
    );

private:
    // Helper methods for specific conflict types
    static void explainLL1Conflict(const Conflict &c, const Grammar &grammar);
    static void explainLRConflict(const Conflict &c);
};

#endif
