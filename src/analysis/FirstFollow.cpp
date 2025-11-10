#include "FirstFollow.h"
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// ======================================================
// 🧩 Compute FIRST sets for all grammar symbols
// ======================================================
void FirstFollowEngine::computeFIRST(const Grammar &grammar) {
    FIRST.clear();
    NULLABLE.clear();

    // Initialize FIRST sets for terminals and nonterminals
    for (const auto &t : grammar.getTerminals()) {
        FIRST[t] = {t}; // FIRST(a) = {a} for terminals
        NULLABLE[t] = false;
    }
    for (const auto &nt : grammar.getNonTerminals()) {
        FIRST[nt] = {};
        NULLABLE[nt] = false;
    }
    FIRST["ε"] = {"ε"};
    NULLABLE["ε"] = true;

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto &prod : grammar.getProductions()) {
            const string &A = prod.getLHS();

            for (const auto &rhs : prod.getRHS()) {
                bool allNullable = true;

                for (const string &symbol : rhs) {
                    const auto &firstSym = FIRST[symbol];

                    // Add FIRST(symbol) \ {ε} to FIRST(A)
                    for (const auto &f : firstSym) {
                        if (f != "ε" && FIRST[A].insert(f).second)
                            changed = true;
                    }

                    // If ε not in FIRST(symbol), stop
                    if (firstSym.find("ε") == firstSym.end()) {
                        allNullable = false;
                        break;
                    }
                }

                // If all symbols in RHS can produce ε → ε ∈ FIRST(A)
                if (allNullable) {
                    if (FIRST[A].insert("ε").second)
                        changed = true;
                    NULLABLE[A] = true;
                }
            }
        }
    }
}

// ======================================================
// 🧭 Compute FOLLOW sets for all nonterminals
// ======================================================
void FirstFollowEngine::computeFOLLOW(const Grammar &grammar) {
    FOLLOW.clear();

    // Initialize empty sets
    for (const auto &nt : grammar.getNonTerminals())
        FOLLOW[nt] = {};

    // Start symbol gets $
    FOLLOW[grammar.getStartSymbol()].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto &prod : grammar.getProductions()) {
            const string &A = prod.getLHS();

            for (const auto &rhs : prod.getRHS()) {
                for (size_t i = 0; i < rhs.size(); ++i) {
                    const string &B = rhs[i];
                    if (!grammar.isNonTerminal(B))
                        continue;

                    // Everything in FIRST(β) except ε → FOLLOW(B)
                    bool epsilonInAll = true;
                    for (size_t j = i + 1; j < rhs.size(); ++j) {
                        const string &beta = rhs[j];
                        for (const auto &f : FIRST[beta]) {
                            if (f != "ε" && FOLLOW[B].insert(f).second)
                                changed = true;
                        }
                        if (FIRST[beta].find("ε") == FIRST[beta].end()) {
                            epsilonInAll = false;
                            break;
                        }
                    }

                    // If β ⇒ ε or B is last in RHS → FOLLOW(A) ⊆ FOLLOW(B)
                    if (i == rhs.size() - 1 || epsilonInAll) {
                        for (const auto &f : FOLLOW[A]) {
                            if (FOLLOW[B].insert(f).second)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

// ======================================================
// 🧾 Accessor Methods
// ======================================================
const std::set<std::string>& FirstFollowEngine::getFIRST(const std::string &symbol) const {
    static const std::set<std::string> empty;
    auto it = FIRST.find(symbol);
    return (it != FIRST.end()) ? it->second : empty;
}

const std::set<std::string>& FirstFollowEngine::getFOLLOW(const std::string &symbol) const {
    static const std::set<std::string> empty;
    auto it = FOLLOW.find(symbol);
    return (it != FOLLOW.end()) ? it->second : empty;
}

bool FirstFollowEngine::isNullable(const std::string &symbol) const {
    auto it = NULLABLE.find(symbol);
    return (it != NULLABLE.end()) ? it->second : false;
}

// ======================================================
// 🖨️ Display FIRST and FOLLOW sets
// ======================================================
void FirstFollowEngine::display() const {
    cout << "\n===== FIRST & FOLLOW Sets =====\n";

    cout << "\nFIRST sets:\n";
    for (const auto &p : FIRST) {
        cout << "FIRST(" << p.first << ") = { ";
        bool first = true;
        for (const auto &sym : p.second) {
            if (!first) cout << ", ";
            cout << sym;
            first = false;
        }
        cout << " }\n";
    }

    cout << "\nFOLLOW sets:\n";
    for (const auto &p : FOLLOW) {
        cout << "FOLLOW(" << p.first << ") = { ";
        bool first = true;
        for (const auto &sym : p.second) {
            if (!first) cout << ", ";
            cout << sym;
            first = false;
        }
        cout << " }\n";
    }

    cout << "===============================\n";
}