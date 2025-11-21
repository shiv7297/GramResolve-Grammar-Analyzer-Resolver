#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../grammar/Grammar.h"
using namespace std;

// ============================================================
// Class: FirstFollowEngine
// Purpose: Compute and store FIRST and FOLLOW sets for a Grammar
// ============================================================
class FirstFollowEngine {
private:
    map<string, set<string>> FIRST;
    map<string, set<string>> FOLLOW;
    map<string, bool> NULLABLE;

public:
    FirstFollowEngine() = default;

    // 🧠 Core computations
    void computeFIRST(const Grammar &g);
    void computeFOLLOW(const Grammar &g);

    // 📦 Accessors
    const set<string>& getFIRST(const string &symbol) const;
    const set<string>& getFOLLOW(const string &symbol) const;
    bool isNullable(const string &symbol) const;

    // 🖨️ Display results
    void display() const;
};

#endif
