#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Production {
private:
    string lhs;  // Left-hand side non-terminal
    vector<std::vector<std::string>> rhs;  // List of right-hand side alternatives

public:
    /// Default + parameterized constructor
    explicit Production(const string &left = "") : lhs(left) {}

    /// Add an alternative RHS (e.g., {"E", "+", "T"})
    void addAlternative(const vector<std::string> &alt);

    /// Get left-hand side non-terminal
    const string &getLHS() const { return lhs; }

    /// Get all RHS alternatives
    const vector<std::vector<std::string>> &getRHS() const { return rhs; }

    /// Convert the production to a readable string form (for display)
    string toString() const;

    /// Display directly to console
    void display() const;
};

#endif
