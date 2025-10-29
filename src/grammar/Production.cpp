// src/grammar/Production.cpp
#include "Production.h"
#include <iostream>
#include <sstream>
using namespace std;

void Production::addAlternative(const vector<string> &symbols) {
    rhs.push_back(symbols);
}

void Production::display() const {
    cout << lhs << " -> ";
    for (size_t i = 0; i < rhs.size(); ++i) {
        const auto &alt = rhs[i];
        for (size_t j = 0; j < alt.size(); ++j) {
            cout << alt[j];
            if (j + 1 < alt.size()) cout << " ";
        }
        if (i + 1 < rhs.size()) cout << " | ";
    }
    cout << endl;
}