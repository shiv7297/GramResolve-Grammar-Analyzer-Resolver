#include "LL1Parser.h"
#include "../report/reportWriter.h"
#include <sstream>
#include <stack>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// A helper to print+save text
#define OUT(x) do { cout << x; ReportWriter::get() << x; } while(0)

// ==========================================================
// 🎯 Build LL(1) Parsing Table
// ==========================================================
void LL1Parser::buildTable() {
    parsingTable.clear();

    OUT("\nBuilding LL(1) Parsing Table...\n");

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
                        parsingTable[A][a] += "|" + rhsStr; // conflict
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
                        parsingTable[A][b] += "|" + rhsStr; // conflict
                }
            }
        }
    }

    OUT("✅ LL(1) Table construction complete.\n");
}

// ==========================================================
// 🧩 Compute FIRST(α)
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
// 🧱 Join RHS symbols
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
// 📄 Display LL(1) Table
// ==========================================================
void LL1Parser::displayTable() const {
    std::ostringstream out;

    out << "\n=== LL(1) Parsing Table ===\n";
    for (const auto &row : parsingTable)
        for (const auto &col : row.second)
            out << "(" << row.first << ", " << col.first
                << ") => " << col.second << "\n";
    out << "===========================\n";

    OUT(out.str());
}

// ==========================================================
// 🧠 Parse input
// ==========================================================
void LL1Parser::parse(const std::vector<std::string> &tokens) const {
    std::ostringstream out;

    out << "\n===== Parsing Input =====\n";
    out << left << setw(30) << "Stack"
        << setw(30) << "Input"
        << "Action\n";
    out << string(80, '-') << "\n";

    OUT(out.str());
    out.str("");

    stack<string> st;
    st.push("$");
    st.push(grammar.getStartSymbol());

    size_t i = 0;
    string current = tokens[i];

    while (!st.empty()) {
        // ---------- Build stack string ----------
        stack<string> tmp = st;
        vector<string> stk;
        while (!tmp.empty()) {
            stk.push_back(tmp.top());
            tmp.pop();
        }
        reverse(stk.begin(), stk.end());

        string stackStr;
        for (size_t j = 0; j < stk.size(); ++j) {
            stackStr += stk[j];
            if (j + 1 < stk.size()) stackStr += " ";
        }

        // ---------- Build input string ----------
        string inputStr;
        for (size_t j = i; j < tokens.size(); j++) {
            inputStr += tokens[j];
            if (j + 1 < tokens.size()) inputStr += " ";
        }

        ostringstream line;
        line << setw(30) << stackStr
             << setw(30) << inputStr;

        string top = st.top();

        // Case 1: Match terminal
        if (top == current) {
            st.pop();
            i++;
            current = (i < tokens.size()) ? tokens[i] : "$";
            line << "Match " << top << "\n";
        }
        else if (grammar.isTerminal(top)) {
            line << "❌ Error: unexpected terminal '" << top << "'\n";
            OUT(line.str());
            return;
        }
        else {
            auto row = parsingTable.find(top);
            if (row == parsingTable.end()) {
                line << "❌ Error: no entry for non-terminal '" << top << "'\n";
                OUT(line.str());
                return;
            }

            auto col = row->second.find(current);
            if (col == row->second.end()) {
                line << "❌ Error: no rule for (" << top << ", " << current << ")\n";
                OUT(line.str());
                return;
            }

            string rhs = col->second;
            line << top << " → " << rhs << "\n";

            st.pop();

            if (rhs != "ε") {
                vector<string> symbols = tokenize(rhs);
                for (auto it = symbols.rbegin(); it != symbols.rend(); ++it)
                    st.push(*it);
            }
        }

        OUT(line.str());
    }

    OUT("\n✅ Parsing complete: Input accepted!\n");
}

// ==========================================================
// 🪓 Tokenizer
// ==========================================================
vector<string> LL1Parser::tokenize(const string &s) const {
    vector<string> out;
    string t;

    for (char c : s) {
        if (isspace(c)) {
            if (!t.empty()) {
                out.push_back(t);
                t.clear();
            }
        } else {
            t += c;
        }
    }
    if (!t.empty())
        out.push_back(t);

    return out;
}
