#include "Production.h"
#include "../report/reportWriter.h"
#include <iostream>
#include <sstream>

using namespace std;

// ===============================================================
// Add a new RHS alternative
// ===============================================================
void Production::addAlternative(const vector<string> &alt) {
    rhs.push_back(alt);
}

// ===============================================================
// Convert production to readable string
// ===============================================================
string Production::toString() const {
    std::ostringstream out;

    out << lhs << " -> ";

    for (size_t i = 0; i < rhs.size(); ++i) {
        for (const auto &sym : rhs[i]) {
            out << sym;
            if (&sym != &rhs[i].back())
                out << " ";
        }
        if (i < rhs.size() - 1)
            out << " | ";
    }

    return out.str();
}

// ===============================================================
// Display production (terminal + report)
// ===============================================================
void Production::display() const {
    string line = toString() + "\n";
    cout << line;
    ReportWriter::get() << line;
}
