#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

// ============================================
// 🔧 Helper Functions
// ============================================

// Trim leading and trailing whitespace
static inline string trim(const string &s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) ++start;
    if (start == s.size()) return "";

    size_t end = s.size() - 1;
    while (end > start && isspace((unsigned char)s[end])) --end;
    return s.substr(start, end - start + 1);
}

// Split a string into tokens by whitespace
static vector<string> splitTokens(const string &s) {
    vector<string> out;
    istringstream iss(s);
    string tok;
    while (iss >> tok)
        out.push_back(tok);
    return out;
}

// ============================================
// 🧩 Grammar Member Functions
// ============================================

// Add a production to the grammar
void Grammar::addProduction(const Production &p) {
    productions.push_back(p);
}

// Load grammar rules from a file
bool Grammar::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "❌ Error: cannot open grammar file '" << filename << "'\n";
        return false;
    }

    // Reset existing data
    productions.clear();
    terminals.clear();
    nonTerminals.clear();
    startSymbol.clear();

    string line;
    bool firstProduction = true;

    // Temporary container for raw production data
    struct TempProd {
        string lhs;
        vector<string> rawAlts;
    };
    vector<TempProd> temp;

    // ===============================
    // 📖 Step 1: Read grammar file
    // ===============================
    while (getline(file, line)) {
        // Remove comments (# ...)
        size_t commentPos = line.find('#');
        if (commentPos != string::npos)
            line = line.substr(0, commentPos);

        line = trim(line);
        if (line.empty()) continue;

        // Find production arrow ("->" or "→")
        size_t arrowPos = line.find("->");
        if (arrowPos == string::npos) {
            size_t unicodePos = line.find("→");
            if (unicodePos != string::npos) arrowPos = unicodePos;
        }

        if (arrowPos == string::npos) {
            cerr << "⚠️ Warning: skipping invalid line (no '->'): " << line << "\n";
            continue;
        }

        string lhs = trim(line.substr(0, arrowPos));
        string rhsPart = trim(line.substr(arrowPos + 2));

        if (lhs.empty() || rhsPart.empty()) {
            cerr << "⚠️ Warning: malformed production: " << line << "\n";
            continue;
        }

        // The first production defines the start symbol
        if (firstProduction) {
            startSymbol = lhs;
            firstProduction = false;
        }

        nonTerminals.insert(lhs);

        // Split alternatives by '|'
        vector<string> alts;
        size_t start = 0;
        while (start < rhsPart.size()) {
            size_t bar = rhsPart.find('|', start);
            if (bar == string::npos) bar = rhsPart.size();
            string alt = trim(rhsPart.substr(start, bar - start));
            if (!alt.empty()) alts.push_back(alt);
            start = bar + 1;
        }

        temp.push_back({lhs, alts});
    }

    file.close();

    // ===============================
    // 🧱 Step 2: Build Production Objects
    // ===============================
    for (const auto &tp : temp) {
        Production p(tp.lhs);
        for (const auto &rawAlt : tp.rawAlts) {
            vector<string> tokens = splitTokens(rawAlt);
            if (tokens.empty())
                tokens.push_back("ε"); // Empty alternative = epsilon
            p.addAlternative(tokens);
        }
        productions.push_back(p);
    }

    // ===============================
    // 🧠 Step 3: Identify Terminals
    // ===============================
    for (const auto &prod : productions) {
        for (const auto &alt : prod.getRHS()) {
            for (const auto &sym : alt) {
                if (nonTerminals.find(sym) == nonTerminals.end() && sym != "ε") {
                    terminals.insert(sym);
                }
            }
        }
    }

    return true;
}

// ============================================
// 🖨️ Display Grammar
// ============================================

void Grammar::display() const {
    cout << "===== Grammar =====\n";
    cout << "Start Symbol: " << startSymbol << "\n\nProductions:\n";

    for (const auto &p : productions)
        cout << p.toString() << "\n";

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
