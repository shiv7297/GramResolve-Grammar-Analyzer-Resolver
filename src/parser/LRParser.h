#ifndef LR_PARSER_H
#define LR_PARSER_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <iostream>
#include "../grammar/Grammar.h"
#include "../analysis/FirstFollow.h"

// ===============================================================
// Struct: LRItem
// Represents an LR(0) item [A → α • β]
// ===============================================================
struct LRItem {
    std::string lhs;
    std::vector<std::string> rhs;
    size_t dot; // position of dot

    bool operator<(const LRItem &other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return dot < other.dot;
    }

    bool operator==(const LRItem &other) const {
        return lhs == other.lhs && rhs == other.rhs && dot == other.dot;
    }

    std::string toString() const {
        std::string s = lhs + " → ";
        if (rhs.empty()) {
            s += "• (ε)";
        } else {
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (i == dot) s += "• ";
                s += rhs[i] + " ";
            }
            if (dot == rhs.size()) s += "•";
        }
        return s;
    }
};

// ===============================================================
// Struct: LRState
// Represents a state (set of LRItems)
// ===============================================================
struct LRState {
    int id;
    std::set<LRItem> items;

    bool operator==(const LRState &other) const {
        return items == other.items;
    }

    std::string toString() const {
        std::string s = "State " + std::to_string(id) + ":\n";
        for (auto &item : items) s += "  " + item.toString() + "\n";
        return s;
    }
};

// ===============================================================
// Class: LR0Parser
// Implements LR(0)/SLR(1) Automaton and Parsing Table
// ===============================================================
class LR0Parser {
private:
    const Grammar &grammar;
    std::vector<LRState> states;

    std::map<int, std::map<std::string, std::string>> ACTION; // ACTION[state][symbol]
    std::map<int, std::map<std::string, int>> GOTO;           // GOTO[state][NonTerminal]

public:
    explicit LR0Parser(const Grammar &g);

    // Build automaton and populate ACTION/GOTO
    void buildAutomaton();

    // Display canonical collection
    void displayStates() const;

    // Run parser simulation
    void parse(const std::vector<std::string> &tokens);

    // Accessor for ACTION table
    const std::map<int, std::map<std::string, std::string>>& getACTION() const;

private:
    // LR(0) core helper functions
    std::set<LRItem> closure(std::set<LRItem> I, const Grammar &g);
    std::set<LRItem> GOTOset(const std::set<LRItem> &I, const std::string &X, const Grammar &g);
    std::set<std::string> collectSymbols(const LRState &state, const Grammar &g);
    int findState(const std::set<LRItem> &items) const;

    // Stack + display helpers
    std::string fullStackToString(std::stack<int> stateStack, std::stack<std::string> symbolStack);
    std::string remainingInput(const std::vector<std::string> &tokens, size_t i);
    std::string stackToString(std::stack<int> st);
};

#endif
