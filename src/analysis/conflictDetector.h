#ifndef CONFLICTDETECTOR_H
#define CONFLICTDETECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include "../parser/parserTable.h"  // relative path

class ConflictDetector {
private:
    std::vector<std::string> ll1Conflicts;
    std::vector<std::string> lrConflicts;

public:
    void detectLL1Conflicts(const ParserTable &table);
    void detectLRConflicts(const ParserTable &table);
    void report() const;

    const std::vector<std::string>& getLL1Conflicts() const { return ll1Conflicts; }
    const std::vector<std::string>& getLRConflicts() const { return lrConflicts; }
};

#endif
