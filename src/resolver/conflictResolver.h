#ifndef CONFLICTRESOLVER_H
#define CONFLICTRESOLVER_H

#include <iostream>
#include <string>
#include "Grammar.h"
#include "Production.h"

class ConflictResolver {
public:
    void suggestLeftFactoring(const Production &p) const;
    void removeLeftRecursion(Grammar &g) const;
    void applyTransformations(Grammar &g) const;
};

#endif
