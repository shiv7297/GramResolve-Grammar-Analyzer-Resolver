#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <vector>
#include <string>

/**
 * @class Production
 * @brief Represents a grammar production rule (e.g., E -> E + T | T)
 *
 * Each production has a left-hand side (non-terminal) and
 * one or more right-hand side alternatives (each a sequence of symbols).
 */
class Production {
private:
    std::string lhs;  // Left-hand side non-terminal
    std::vector<std::vector<std::string>> rhs;  // List of right-hand side alternatives

public:
    /// Default + parameterized constructor
    explicit Production(const std::string &left = "") : lhs(left) {}

    /// Add an alternative RHS (e.g., {"E", "+", "T"})
    void addAlternative(const std::vector<std::string> &alt);

    /// Get left-hand side non-terminal
    const std::string &getLHS() const { return lhs; }

    /// Get all RHS alternatives
    const std::vector<std::vector<std::string>> &getRHS() const { return rhs; }

    /// Convert the production to a readable string form (for display)
    std::string toString() const;

    /// Display directly to console
    void display() const;
};

#endif
