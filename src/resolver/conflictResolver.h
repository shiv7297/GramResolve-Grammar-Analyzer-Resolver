#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include <vector>
#include <string>
#include <sstream>
#include "../analysis/conflictDetector.h"
#include "../grammar/Grammar.h"

using namespace std;

class ConflictResolver {
public:
    // Main resolver entry
    static void resolveConflicts(
        const vector<Conflict> &conflicts,
        const Grammar &grammar
    );

private:
    // LL(1) conflict explanation (removed unused Grammar param)
    static void appendLL1Explanation(
        ostringstream &out,
        const Conflict &c
    );

    // LR conflict explanations
    static void appendLRExplanation(
        ostringstream &out,
        const Conflict &c
    );
};

#endif
