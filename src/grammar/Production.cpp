#include "Production.h"
#include <iostream>

using namespace std;

/// Add a new right-hand side alternative
void Production::addAlternative(const vector<string> &alt) {
    rhs.push_back(alt);
}

/// Convert production to string (used in Grammar::display)
string Production::toString() const {
    string out = lhs + " -> ";
    for (size_t i = 0; i < rhs.size(); ++i) {
        for (const auto &sym : rhs[i])
            out += sym + " ";
        if (i < rhs.size() - 1)
            out += "| ";
    }
    return out;
}

/// Print production nicely to console
void Production::display() const {
    cout << toString() << endl;
}
