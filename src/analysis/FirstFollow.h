#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../grammar/Grammar.h"

// ============================================================
// Class: FirstFollowEngine
// Purpose: Compute and store FIRST and FOLLOW sets for a Grammar
// ============================================================
class FirstFollowEngine {
private:
    std::map<std::string, std::set<std::string>> FIRST;
    std::map<std::string, std::set<std::string>> FOLLOW;
    std::map<std::string, bool> NULLABLE;

public:
    FirstFollowEngine() = default;

    // 🧠 Core computations
    void computeFIRST(const Grammar &g);
    void computeFOLLOW(const Grammar &g);

    // 📦 Accessors
    const std::set<std::string>& getFIRST(const std::string &symbol) const;
    const std::set<std::string>& getFOLLOW(const std::string &symbol) const;
    bool isNullable(const std::string &symbol) const;

    // 🖨️ Display results
    void display() const;
};

#endif
