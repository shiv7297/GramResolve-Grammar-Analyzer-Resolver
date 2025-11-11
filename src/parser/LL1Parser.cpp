#include "LL1Parser.h"
#include <stack>
#include <iostream>
#include<vector>
#include<algorithm>
#include <iomanip>

using namespace std;

// ==========================================================
// 🎯 Build LL(1) Parsing Table
// ==========================================================
void LL1Parser::buildTable() {
    parsingTable.clear();

    cout << "\nBuilding LL(1) Parsing Table...\n";

    for (const auto &prod : grammar.getProductions()) {
        const string &A = prod.getLHS();

        for (const auto &rhs : prod.getRHS()) {
            set<string> firstSet = computeFirstOfString(rhs);

            // Rule 1: FIRST(rhs)
            for (const auto &a : firstSet) {
                if (a != "ε") {
                    string rhsStr = join(rhs, " ");
                    if (parsingTable[A][a].empty())
                        parsingTable[A][a] = rhsStr;
                    else
                        parsingTable[A][a] += "|" + rhsStr;  // ✅ append to detect conflict
                }
            }

            // Rule 2: ε in FIRST(rhs) → FOLLOW(A)
            if (firstSet.find("ε") != firstSet.end()) {
                const auto &followA = ff.getFOLLOW(A);
                for (const auto &b : followA) {
                    string rhsStr = join(rhs, " ");
                    if (parsingTable[A][b].empty())
                        parsingTable[A][b] = rhsStr;
                    else
                        parsingTable[A][b] += "|" + rhsStr;  // ✅ append conflict
                }
            }
        }
    }

    cout << "✅ LL(1) Table construction complete.\n";
}


// ==========================================================
// 🧩 Compute FIRST(α) where α is a RHS symbol sequence
// ==========================================================
set<string> LL1Parser::computeFirstOfString(const vector<string> &rhs) const {
    set<string> result;

    for (const auto &symbol : rhs) {
        const set<string> &firstSym = ff.getFIRST(symbol);

        bool hasEpsilon = false;
        for (const auto &t : firstSym) {
            if (t == "ε")
                hasEpsilon = true;
            else
                result.insert(t);
        }

        if (!hasEpsilon)
            return result;
    }

    result.insert("ε");
    return result;
}

// ==========================================================
// 🧱 Join RHS symbols into a readable string
// ==========================================================
string LL1Parser::join(const vector<string> &rhs, const string &sep) const {
    string out;
    for (size_t i = 0; i < rhs.size(); ++i) {
        out += rhs[i];
        if (i < rhs.size() - 1)
            out += sep;
    }
    return out;
}

// ==========================================================
// 🧠 Parse input using LL(1) Parsing Table
// ==========================================================
void LL1Parser::parse(const std::vector<std::string> &tokens) const {
    cout << "\n===== Parsing Input =====\n";

    std::stack<std::string> st;
    st.push("$");
    st.push(grammar.getStartSymbol());

    size_t i = 0;
    std::string current = tokens[i];

    cout << left << setw(30) << "Stack"
         << setw(30) << "Input"
         << "Action\n";
    cout << string(80, '-') << "\n";

    // --- Parsing loop ---
    while (!st.empty()) {
        // ✅ Print full stack contents (top on the right)
        std::stack<std::string> temp = st;
        std::vector<std::string> stackContent;
        while (!temp.empty()) {
            stackContent.push_back(temp.top());
            temp.pop();
        }
        std::reverse(stackContent.begin(), stackContent.end());
        std::string stackStr;
        for (size_t j = 0; j < stackContent.size(); ++j) {
            stackStr += stackContent[j];
            if (j < stackContent.size() - 1)
                stackStr += " ";
        }

        // ✅ Build remaining input string
        std::string inputStr;
        for (size_t j = i; j < tokens.size(); ++j) {
            inputStr += tokens[j];
            if (j < tokens.size() - 1)
                inputStr += " ";
        }

        // Display current parser state
        cout << setw(30) << stackStr
             << setw(30) << inputStr;

        std::string top = st.top();

        // Case 1: Match terminal
        if (top == current) {
            st.pop();
            i++;
            current = (i < tokens.size()) ? tokens[i] : "$";
            cout << "Match " << top << "\n";
        }
        // Case 2: Unexpected terminal
        else if (grammar.isTerminal(top)) {
            cout << "❌ Error: unexpected terminal '" << top << "'\n";
            return;
        }
        // Case 3: Non-terminal — consult table
        else {
            auto rowIt = parsingTable.find(top);
            if (rowIt == parsingTable.end()) {
                cout << "❌ Error: no entry for non-terminal '" << top << "'\n";
                return;
            }

            auto colIt = rowIt->second.find(current);
            if (colIt == rowIt->second.end()) {
                cout << "❌ Error: no rule for (" << top << ", " << current << ")\n";
                return;
            }

            std::string rhs = colIt->second;
            cout << top << " → " << rhs << "\n";
            st.pop();

            // Push RHS symbols in reverse order (ignore ε)
            if (rhs != "ε") {
                std::vector<std::string> symbols = tokenize(rhs);
                for (auto it = symbols.rbegin(); it != symbols.rend(); ++it)
                    st.push(*it);
            }
        }
    }

    cout << "\n✅ Parsing complete: Input accepted!\n";
}


// ==========================================================
// 🪓 Tokenize RHS string
// ==========================================================
vector<string> LL1Parser::tokenize(const string &s) const {
    vector<string> tokens;
    string token;
    for (char c : s) {
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty())
        tokens.push_back(token);
    return tokens;
}
