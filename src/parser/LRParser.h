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

using namespace std;

// ===============================================================
// Struct: LRItem
// Represents an LR(0) item [A → α • β]
// ===============================================================
struct LRItem {
    string lhs;
    vector<string> rhs;
    size_t dot; // position of dot

    bool operator<(const LRItem &other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return dot < other.dot;
    }

    bool operator==(const LRItem &other) const {
        return lhs == other.lhs && rhs == other.rhs && dot == other.dot;
    }

    string toString() const {
        string s = lhs + " → ";
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
    set<LRItem> items;

    bool operator==(const LRState &other) const {
        return items == other.items;
    }

    string toString() const {
        string s = "State " + to_string(id) + ":\n";
        for (auto &item : items)
            s += "  " + item.toString() + "\n";
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
    vector<LRState> states;

    map<int, map<string, string>> ACTION;  // ACTION[state][symbol]
    map<int, map<string, int>> GOTO;       // GOTO[state][NonTerminal]

public:
    explicit LR0Parser(const Grammar &g);

    // Build automaton and populate ACTION/GOTO
    void buildAutomaton();

    // Display canonical collection
    void displayStates() const;

    // Run parser simulation
    void parse(const vector<string> &tokens);

    // Accessor for ACTION table
    const map<int, map<string, string>>& getACTION() const;

private:
    // LR(0) core helper functions
    set<LRItem> closure(set<LRItem> I, const Grammar &g);
    set<LRItem> GOTOset(const set<LRItem> &I, const string &X, const Grammar &g);

    // FIXED: removed unused Grammar parameter
    set<string> collectSymbols(const LRState &state) const;

    int findState(const set<LRItem> &items) const;

    // Stack + display helpers
    string fullStackToString(stack<int> stateStack, stack<string> symbolStack);
    string remainingInput(const vector<string> &tokens, size_t i);
    string stackToString(stack<int> st);
};

#endif
