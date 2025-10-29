// src/grammar/Grammar.cpp
#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Helper: trim from both ends
static inline string trim(const string &s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) ++start;
    if (start == s.size()) return "";

    size_t end = s.size() - 1;
    while (end > start && isspace((unsigned char)s[end])) --end;
    return s.substr(start, end - start + 1);
}

// Helper: split by whitespace (one or more) preserving tokens like "+" or "id" etc.
// Assumes tokens are separated by spaces in grammar input (common for BNF samples).
static vector<string> splitTokens(const string &s) {
    vector<string> out;
    istringstream iss(s);
    string tok;
    while (iss >> tok) out.push_back(tok);
    return out;
}

void Grammar::addProduction(const Production &p) {
    productions.push_back(p);
}

void Grammar::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: cannot open grammar file '" << filename << "'\n";
        exit(1);
    }

    productions.clear();
    terminals.clear();
    nonTerminals.clear();
    startSymbol.clear();

    string line;
    bool firstProduction = true;

    // We'll store temporary parsed productions (lhs + vector of alternatives as raw strings)
    struct TempProd {
        string lhs;
        vector<string> rawAlts; // each alt as raw string (e.g. "E + T")
    };
    vector<TempProd> temp;

    while (getline(file, line)) {
        // remove comments (anything after '#')
        size_t commentPos = line.find('#');
        if (commentPos != string::npos) line = line.substr(0, commentPos);

        line = trim(line);
        if (line.empty()) continue;

        // Expect format: LHS -> RHS1 | RHS2 ...
        size_t arrowPos = line.find("->");
        if (arrowPos == string::npos) {
            // Try unicode arrow or alternative '→'
            size_t unicodePos = line.find("→");
            if (unicodePos != string::npos) arrowPos = unicodePos;
        }

        if (arrowPos == string::npos) {
            cerr << "Warning: skipping invalid production line (no '->'): " << line << "\n";
            continue;
        }

        string lhs = trim(line.substr(0, arrowPos));
        string rhsPart = trim(line.substr(arrowPos + 2)); // after "->"
        if (lhs.empty() || rhsPart.empty()) {
            cerr << "Warning: skipping malformed production: " << line << "\n";
            continue;
        }

        if (firstProduction) {
            startSymbol = lhs;
            firstProduction = false;
        }

        nonTerminals.insert(lhs);

        // split by '|' for alternatives, but be robust to spaces
        vector<string> alts;
        size_t start = 0;
        while (start < rhsPart.size()) {
            size_t bar = rhsPart.find('|', start);
            if (bar == string::npos) bar = rhsPart.size();
            string alt = trim(rhsPart.substr(start, bar - start));
            if (!alt.empty()) alts.push_back(alt);
            start = bar + 1;
        }

        TempProd tp;
        tp.lhs = lhs;
        tp.rawAlts = alts;
        temp.push_back(tp);
    }
    file.close();

    // Now that we have all LHS nonTerminals, convert to Production objects and classify terminals
    for (const auto &tp : temp) {
        Production p(tp.lhs);
        for (const auto &rawAlt : tp.rawAlts) {
            vector<string> tokens = splitTokens(rawAlt);
            // If empty alternative (epsilon) represented as epsilon or eps or "ε", keep as single token "EPS"
            if (tokens.empty()) {
                tokens.push_back("ε"); // epsilon token symbol
            }
            p.addAlternative(tokens);
        }
        productions.push_back(p);
    }

    // Classify terminals: any symbol appearing in RHS that is not an LHS nonTerminal => terminal.
    for (const auto &prod : productions) {
        for (const auto &alt : prod.getRHS()) {
            for (const auto &sym : alt) {
                if (nonTerminals.find(sym) == nonTerminals.end()) {
                    // it's not an LHS non-terminal -> terminal (but ignore epsilon symbol)
                    if (sym != "ε") terminals.insert(sym);
                }
            }
        }
    }
}

void Grammar::display() const {
    cout << "===== Grammar =====\n";
    cout << "Start Symbol: " << startSymbol << "\n\nProductions:\n";
    for (const auto &p : productions) {
        p.display();
    }

    cout << "\nNon-terminals (" << nonTerminals.size() << "): ";
    bool first = true;
    for (const auto &nt : nonTerminals) {
        if (!first) cout << ", ";
        cout << nt;
        first = false;
    }
    cout << "\nTerminals (" << terminals.size() << "): ";
    first = true;
    for (const auto &t : terminals) {
        if (!first) cout << ", ";
        cout << t;
        first = false;
    }
    cout << "\n===================\n";
}
