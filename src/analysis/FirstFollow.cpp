#include "FirstFollow.h"
#include <iostream>
#include <stack>
#include <algorithm>

// Utility: add one set into another; returns true if FIRST changed
static bool addSet(std::set<std::string>& dest, const std::set<std::string>& src) {
    size_t oldSize = dest.size();
    dest.insert(src.begin(), src.end());
    return dest.size() != oldSize;
}

// Utility: check if vector of symbols can all derive epsilon
static bool allNullable(const std::vector<std::string>& symbols,
                        const std::map<std::string, bool>& nullable) {
    for (const auto& s : symbols) {
        if (nullable.find(s) == nullable.end() || !nullable.at(s))
            return false;
    }
    return true;
}

// ------------------------------------------------------------
// Compute FIRST sets
// ------------------------------------------------------------
void FirstFollowEngine::computeFIRST(const Grammar &g) {
    FIRST.clear();
    NULLABLE.clear();

    // Initialize FIRST sets: terminals map to themselves
    for (const auto& t : g.getTerminals()) {
        FIRST[t].insert(t);
        NULLABLE[t] = false;
    }

    // Nonterminals start empty
    for (const auto& nt : g.getNonTerminals()) {
        FIRST[nt];          // ensure existence
        NULLABLE[nt] = false;
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto& prod : g.getProductions()) {
            const std::string& A = prod.getLHS();

            for (const auto& rhs : prod.getRHS()) {
                bool allEps = true;

                for (const auto& sym : rhs) {
                    // Add FIRST(sym) \ {ε} to FIRST(A)
                    for (const auto& f : FIRST[sym]) {
                        if (f != "ε")
                            changed |= FIRST[A].insert(f).second;
                    }

                    if (NULLABLE[sym]) continue;
                    allEps = false;
                    break;
                }

                if (allEps) {
                    changed |= FIRST[A].insert("ε").second;
                    NULLABLE[A] = true;
                }
            }
        }
    }
}

// ------------------------------------------------------------
// Compute FOLLOW sets
// ------------------------------------------------------------
void FirstFollowEngine::computeFOLLOW(const Grammar &g) {
    FOLLOW.clear();

    // Initialize empty FOLLOW sets
    for (const auto& nt : g.getNonTerminals())
        FOLLOW[nt];

    // Add $ to FOLLOW(start)
    FOLLOW[g.getStartSymbol()].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto& prod : g.getProductions()) {
            const std::string& A = prod.getLHS();

            for (const auto& rhs : prod.getRHS()) {
                for (size_t i = 0; i < rhs.size(); ++i) {
                    const std::string& B = rhs[i];
                    if (g.getNonTerminals().count(B) == 0) continue; // skip terminals

                    // Everything after B
                    std::set<std::string> trailer;

                    if (i + 1 < rhs.size()) {
                        for (size_t j = i + 1; j < rhs.size(); ++j) {
                            const std::string& beta = rhs[j];

                            // Add FIRST(beta) \ {ε}
                            for (const auto& f : FIRST[beta]) {
                                if (f != "ε")
                                    changed |= FOLLOW[B].insert(f).second;
                            }

                            // Stop if beta not nullable
                            if (!NULLABLE[beta])
                                break;

                            // If reached end and all nullable → add FOLLOW(A)
                            if (j == rhs.size() - 1)
                                for (const auto& f : FOLLOW[A])
                                    changed |= FOLLOW[B].insert(f).second;
                        }
                    } else {
                        // B is at end → add FOLLOW(A)
                        for (const auto& f : FOLLOW[A])
                            changed |= FOLLOW[B].insert(f).second;
                    }
                }
            }
        }
    }
}

// ------------------------------------------------------------
// Accessors
// ------------------------------------------------------------
const std::set<std::string>& FirstFollowEngine::getFIRST(const std::string &symbol) const {
    static std::set<std::string> empty;
    auto it = FIRST.find(symbol);
    return (it != FIRST.end()) ? it->second : empty;
}

const std::set<std::string>& FirstFollowEngine::getFOLLOW(const std::string &symbol) const {
    static std::set<std::string> empty;
    auto it = FOLLOW.find(symbol);
    return (it != FOLLOW.end()) ? it->second : empty;
}

bool FirstFollowEngine::isNullable(const std::string &symbol) const {
    auto it = NULLABLE.find(symbol);
    return (it != NULLABLE.end()) ? it->second : false;
}

// ------------------------------------------------------------
// Display
// ------------------------------------------------------------
void FirstFollowEngine::display() const {
    std::cout << "\n===== FIRST & FOLLOW Sets =====\n";

    std::cout << "\nFIRST sets:\n";
    for (const auto& p : FIRST) {
        std::cout << "FIRST(" << p.first << ") = { ";
        bool first = true;
        for (const auto& s : p.second) {
            if (!first) std::cout << ", ";
            std::cout << s;
            first = false;
        }
        std::cout << " }\n";
    }

    std::cout << "\nFOLLOW sets:\n";
    for (const auto& p : FOLLOW) {
        std::cout << "FOLLOW(" << p.first << ") = { ";
        bool first = true;
        for (const auto& s : p.second) {
            if (!first) std::cout << ", ";
            std::cout << s;
            first = false;
        }
        std::cout << " }\n";
    }

    std::cout << "===============================\n";
}
