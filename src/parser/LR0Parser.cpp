#include "LRParser.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include "../report/reportWriter.h"
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
    std::ostringstream out;
    out << "\n🔧 Building LR(0) Automaton (with SLR reduce placement)...\n";
    cout << out.str();
    ReportWriter::get() << out.str();
    out.str("");

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

        // FIXED: removed unused Grammar parameter
        set<string> symbols = collectSymbols(states[i]);

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

            if (g2.isTerminal(sym)) {
                string act = "s" + to_string(found);
                if (ACTION[i][sym].empty())
                    ACTION[i][sym] = act;
                else
                    ACTION[i][sym] += "|" + act;
            } else {
                GOTO[i][sym] = found;
            }
        }
    }

    // Add reduce/accept actions
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
                            ACTION[i][t] += "|" + act;
                    }
                }
            }
        }
    }

    out << "✅ LR(0)/SLR Automaton built with " << states.size() << " states.\n";
    cout << out.str();
    ReportWriter::get() << out.str();
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
// FIXED: Collect symbols — no unused parameter
// ===================================================
set<string> LR0Parser::collectSymbols(const LRState &state) const {
    set<string> symbols;

    for (const auto &item : state.items) {
        if (item.dot < item.rhs.size())
            symbols.insert(item.rhs[item.dot]);
    }

    return symbols;
}

// ===================================================
// Check if state already exists
// ===================================================
int LR0Parser::findState(const set<LRItem> &items) const {
    for (size_t i = 0; i < states.size(); ++i)
        if (states[i].items == items)
            return static_cast<int>(i);

    return -1;
}

// ===================================================
// Display canonical states
// ===================================================
void LR0Parser::displayStates() const {
    std::ostringstream out;

    out << "\n===== Canonical Collection of LR(0) Items =====\n";
    for (const auto &st : states)
        out << st.toString() << "\n";

    out << "===============================================\n";

    cout << out.str();
    ReportWriter::get() << out.str();
}

// ===================================================
// LR Parsing Simulation
// ===================================================
void LR0Parser::parse(const vector<string> &tokens) {
    std::ostringstream out;

    out << "\n===== Parsing Input (LR(0)) =====\n";
    out << left << setw(40) << "Stack (States + Symbols)"
        << setw(25) << "Input"
        << "Action\n";
    out << string(90, '-') << "\n";

    cout << out.str();
    ReportWriter::get() << out.str();
    out.str("");

    stack<int> stateStack;
    stack<string> symbolStack;

    stateStack.push(0);
    size_t i = 0;
    string a = tokens[i];

    while (true) {
        int s = stateStack.top();
        string act = ACTION[s][a];

        std::ostringstream line;
        line << setw(40) << fullStackToString(stateStack, symbolStack)
             << setw(25) << remainingInput(tokens, i);

        if (act.empty()) {
            line << "❌ Error: no action for (" << s << ", " << a << ")\n";
            cout << line.str();
            ReportWriter::get() << line.str();
            return;
        }

        // SHIFT
        if (act[0] == 's') {
            int next = stoi(act.substr(1));

            line << "Shift and go to state " << next << "\n";

            symbolStack.push(a);
            stateStack.push(next);

            i++;
            a = tokens[i];
        }

        // REDUCE
        else if (act[0] == 'r') {
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
                line << "Reduce by " << A << " -> ε\n";
            else
                line << "Reduce by " << A << " -> " << rhsPart << "\n";

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
        }

        // ACCEPT
        else if (act == "acc") {
            line << "✅ Accept!\n";
            cout << line.str();
            ReportWriter::get() << line.str();
            break;
        }

        // UNKNOWN
        else {
            line << "❌ Unknown action: " << act << "\n";
            cout << line.str();
            ReportWriter::get() << line.str();
            return;
        }

        cout << line.str();
        ReportWriter::get() << line.str();
    }
}

// ===================================================
// Helpers
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
        if (!sym.empty())
            result += " " + sym + " ";
    }

    return result;
}

string LR0Parser::remainingInput(const vector<string> &tokens, size_t i) {
    string s;

    for (size_t k = i; k < tokens.size(); ++k)
        s += tokens[k] + " ";

    return s;
}

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
