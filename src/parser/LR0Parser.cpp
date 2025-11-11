#include "LRParser.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

using namespace std;

// ===================================================
// Constructor
// ===================================================
LR0Parser::LR0Parser(const Grammar &g) : grammar(g) {}

// ===================================================
// Accessor for ACTION table
// ===================================================
const map<int, map<string, string>>& LR0Parser::getACTION() const {
    return ACTION;
}

// ===================================================
// Build Canonical Collection of LR(0) Items
// ===================================================
void LR0Parser::buildAutomaton() {
    cout << "\n🔧 Building LR(0) Automaton (with SLR reduce placement)...\n";

    FirstFollowEngine ff;
    ff.computeFIRST(grammar);
    ff.computeFOLLOW(grammar);

    string start = grammar.getStartSymbol();
    string augmented = start + "'";
    Production aug(augmented);
    aug.addAlternative({start});

    Grammar g2 = grammar;
    g2.addProduction(aug);

    LRItem startItem = {augmented, {start}, 0};
    LRState startState = {0, closure({startItem}, g2)};
    states.push_back(startState);

    queue<int> q;
    q.push(0);

    // Canonical collection
    while (!q.empty()) {
        int i = q.front(); q.pop();
        set<string> symbols = collectSymbols(states[i], g2);

        for (const string &sym : symbols) {
            set<LRItem> gotoItems = GOTOset(states[i].items, sym, g2);
            if (gotoItems.empty()) continue;

            int found = findState(gotoItems);
            if (found == -1) {
                LRState newState = {static_cast<int>(states.size()), gotoItems};
                states.push_back(newState);
                q.push(newState.id);
                found = newState.id;
            }

            // ✅ SHIFT / GOTO conflict-friendly storage
            if (g2.isTerminal(sym)) {
                string act = "s" + to_string(found);
                if (ACTION[i][sym].empty())
                    ACTION[i][sym] = act;
                else
                    ACTION[i][sym] += "|" + act;  // append for multiple actions
            } else {
                GOTO[i][sym] = found;
            }
        }
    }

    // Step 3: Add reduce/accept actions
    for (size_t i = 0; i < states.size(); ++i) {
        for (const auto &item : states[i].items) {
            if (item.dot == item.rhs.size()) {
                if (item.lhs == augmented) {
                    ACTION[i]["$"] = "acc";
                } else {
                    stringstream rule;
                    rule << item.lhs << "->";
                    for (auto &sym : item.rhs)
                        rule << sym << " ";
                    string ruleStr = rule.str();

                    for (const auto &t : ff.getFOLLOW(item.lhs)) {
                        string act = "r" + ruleStr;
                        if (ACTION[i][t].empty())
                            ACTION[i][t] = act;
                        else
                            ACTION[i][t] += "|" + act;  // ✅ combine conflicts
                    }
                }
            }
        }
    }

    cout << "✅ LR(0)/SLR Automaton built with " << states.size() << " states.\n";
}


// ===================================================
// Compute closure(I)
// ===================================================
set<LRItem> LR0Parser::closure(set<LRItem> I, const Grammar &g) {
    bool changed = true;
    while (changed) {
        changed = false;
        set<LRItem> newItems = I;

        for (const auto &item : I) {
            if (item.dot < item.rhs.size()) {
                string B = item.rhs[item.dot];
                if (g.isNonTerminal(B)) {
                    for (const auto &p : g.getProductions()) {
                        if (p.getLHS() == B) {
                            for (const auto &alt : p.getRHS()) {
                                // ✅ Handle epsilon properly
                                vector<string> rhs = alt;
                                if (rhs.size() == 1 && rhs[0] == "ε")
                                    rhs.clear();
                                
                                LRItem newItem = {B, rhs, 0};
                                if (newItems.insert(newItem).second)
                                    changed = true;
                            }
                        }
                    }
                }
            }
        }
        I = newItems;
    }
    return I;
}

// ===================================================
// Compute GOTO(I, X)
// ===================================================
set<LRItem> LR0Parser::GOTOset(const set<LRItem> &I, const string &X, const Grammar &g) {
    set<LRItem> J;
    for (const auto &item : I) {
        if (item.dot < item.rhs.size() && item.rhs[item.dot] == X) {
            LRItem moved = item;
            moved.dot++;
            J.insert(moved);
        }
    }
    return closure(J, g);
}

// ===================================================
// Collect all symbols that can appear after a dot
// ===================================================
set<string> LR0Parser::collectSymbols(const LRState &state, const Grammar &g) {
    set<string> symbols;
    for (const auto &item : state.items) {
        if (item.dot < item.rhs.size()) symbols.insert(item.rhs[item.dot]);
    }
    return symbols;
}

// ===================================================
// Check if a state already exists
// ===================================================
int LR0Parser::findState(const set<LRItem> &items) const {
    for (size_t i = 0; i < states.size(); ++i) {
        if (states[i].items == items)
            return static_cast<int>(i);
    }
    return -1;
}

// ===================================================
// Display canonical collection of LR(0) items
// ===================================================
void LR0Parser::displayStates() const {
    cout << "\n===== Canonical Collection of LR(0) Items =====\n";
    for (const auto &st : states) {
        cout << st.toString() << "\n";
    }
}

// ===================================================
// Simulate LR(0) parsing
// ===================================================
void LR0Parser::parse(const vector<string> &tokens) {
    cout << "\n===== Parsing Input (LR(0)) =====\n";

    stack<int> stateStack;
    stack<string> symbolStack;

    stateStack.push(0);
    size_t i = 0;
    string a = tokens[i];

    cout << left << setw(40) << "Stack (States + Symbols)"
         << setw(25) << "Input"
         << "Action\n";
    cout << string(90, '-') << "\n";

    while (true) {
        int s = stateStack.top();
        string act = ACTION[s][a];

        cout << setw(40) << fullStackToString(stateStack, symbolStack)
             << setw(25) << remainingInput(tokens, i);

        if (act.empty()) {
            cout << "❌ Error: no action for (" << s << ", " << a << ")\n";
            return;
        }

        if (act[0] == 's') {
            int next = stoi(act.substr(1));
            cout << "Shift and go to state " << next << "\n";

            symbolStack.push(a);
            stateStack.push(next);

            i++;
            a = tokens[i];
        } else if (act[0] == 'r') {
            string rule = act.substr(1);
            size_t arrow = rule.find("->");
            string A = rule.substr(0, arrow);
            string rhsPart = rule.substr(arrow + 2);

            vector<string> rhs;
            stringstream ss(rhsPart);
            string sym;
            while (ss >> sym)
                rhs.push_back(sym);

            if (rhs.empty())
                cout << "Reduce by " << A << " -> ε\n";
            else
                cout << "Reduce by " << A << " -> " << rhsPart << "\n";

            for (size_t j = 0; j < rhs.size(); ++j) {
                if (!rhs[j].empty() && rhs[j] != "ε") {
                    if (!symbolStack.empty()) symbolStack.pop();
                    if (!stateStack.empty()) stateStack.pop();
                }
            }

            int t = stateStack.top();
            int next = GOTO[t][A];

            symbolStack.push(A);
            stateStack.push(next);
        } else if (act == "acc") {
            cout << "✅ Accept!\n";
            break;
        } else {
            cout << "❌ Unknown action: " << act << "\n";
            return;
        }
    }
}

// ===================================================
// Helper: Combine states + symbols into readable string
// ===================================================
string LR0Parser::fullStackToString(stack<int> stateStack, stack<string> symbolStack) {
    vector<pair<int, string>> temp;
    while (!stateStack.empty()) {
        string sym = symbolStack.empty() ? "" : symbolStack.top();
        temp.push_back({stateStack.top(), sym});
        stateStack.pop();
        if (!symbolStack.empty()) symbolStack.pop();
    }
    reverse(temp.begin(), temp.end());

    string result;
    for (auto &[state, sym] : temp) {
        result += to_string(state);
        if (!sym.empty()) result += " " + sym + " ";
    }
    return result;
}

// ===================================================
// Helper: Show remaining input
// ===================================================
string LR0Parser::remainingInput(const vector<string> &tokens, size_t i) {
    string s;
    for (size_t k = i; k < tokens.size(); ++k)
        s += tokens[k] + " ";
    return s;
}

// ===================================================
// Helper: Stack to string
// ===================================================
string LR0Parser::stackToString(stack<int> st) {
    vector<int> temp;
    while (!st.empty()) {
        temp.push_back(st.top());
        st.pop();
    }
    reverse(temp.begin(), temp.end());
    string s;
    for (int v : temp)
        s += to_string(v) + " ";
    return s;
}
