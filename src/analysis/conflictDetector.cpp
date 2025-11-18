#include "conflictDetector.h"
#include "../report/reportWriter.h"
#include <sstream>
#include <set>
#include <regex>
using namespace std;

// helper to split strings by '|'
static vector<string> splitActions(const string &s) {
    vector<string> parts;
    stringstream ss(s);
    string part;
    while (getline(ss, part, '|')) {
        if (!part.empty())
            parts.push_back(part);
    }
    return parts;
}

// ---------------------------------------------------------------
// LL(1) Conflict Detection
// ---------------------------------------------------------------
vector<Conflict> ConflictDetector::detectLL1Conflicts(
    const Grammar &grammar,
    const FirstFollowEngine &ff,
    const map<string, map<string, string>> &table
) {
    vector<Conflict> conflicts;

    for (const auto &ntPair : table) {
        const string &nonTerminal = ntPair.first;
        for (const auto &tPair : ntPair.second) {
            const string &terminal = tPair.first;
            const string &entry = tPair.second;

            auto parts = splitActions(entry);
            if (parts.size() > 1) {
                Conflict c;
                c.type = "LL(1) MULTIPLE ENTRIES";
                c.location = "(" + nonTerminal + ", " + terminal + ")";
                c.details = parts;
                conflicts.push_back(c);
            }
        }
    }
    return conflicts;
}

// ---------------------------------------------------------------
// LR(0)/SLR Conflict Detection
// ---------------------------------------------------------------
vector<Conflict> ConflictDetector::detectLRConflicts(
    const map<int, map<string, string>> &ACTION
) {
    vector<Conflict> conflicts;
    regex shift("^s\\d+$"), reduce("^r\\d+$");

    for (const auto &statePair : ACTION) {
        int state = statePair.first;
        for (const auto &symPair : statePair.second) {
            const string &symbol = symPair.first;
            auto parts = splitActions(symPair.second);

            bool hasShift = false, hasReduce = false;
            int shiftCount = 0, reduceCount = 0;

            for (const string &p : parts) {
                if (regex_match(p, shift)) { hasShift = true; shiftCount++; }
                else if (regex_match(p, reduce)) { hasReduce = true; reduceCount++; }
            }

            if (parts.size() > 1) {
                if (hasShift && hasReduce) {
                    conflicts.push_back({"Shift/Reduce Conflict",
                        "(State " + to_string(state) + ", " + symbol + ")", parts});
                } else if (shiftCount > 1) {
                    conflicts.push_back({"Shift/Shift Conflict",
                        "(State " + to_string(state) + ", " + symbol + ")", parts});
                } else if (reduceCount > 1) {
                    conflicts.push_back({"Reduce/Reduce Conflict",
                        "(State " + to_string(state) + ", " + symbol + ")", parts});
                } else {
                    conflicts.push_back({"Multiple Actions",
                        "(State " + to_string(state) + ", " + symbol + ")", parts});
                }
            }
        }
    }

    return conflicts;
}


// ---------------------------------------------------------------
// Display detected conflicts
// ---------------------------------------------------------------
void ConflictDetector::displayConflicts(const vector<Conflict> &conflicts) {
    std::ostringstream out;

    if (conflicts.empty()) {
        out << "✅ No conflicts detected!\n";

        // Print + Save
        cout << out.str();
        ReportWriter::get() << out.str();
        return;
    }

    out << "\n⚠️  Conflicts detected:\n";
    out << left << setw(30) << "Type"
        << setw(25) << "Location"
        << "Details\n";
    out << string(80, '-') << "\n";

    for (const auto &c : conflicts) {
        out << left << setw(30) << c.type
            << setw(25) << c.location;

        if (!c.details.empty())
            out << c.details[0];

        out << "\n";

        for (size_t i = 1; i < c.details.size(); ++i)
            out << setw(55) << " " << c.details[i] << "\n";
    }

    out << string(80, '-') << "\n";
    out << "Total Conflicts: " << conflicts.size() << "\n";

    // Print to terminal
    cout << out.str();

    // Save to report file
    ReportWriter::get() << out.str();
}




