#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include "Grammar.h"

class FirstFollowEngine {
private:
    std::map<std::string, std::set<std::string>> FIRST;
    std::map<std::string, std::set<std::string>> FOLLOW;
    std::map<std::string, bool> NULLABLE;

public:
    void computeFIRST(const Grammar &g);
    void computeFOLLOW(const Grammar &g);

    const std::set<std::string>& getFIRST(const std::string &symbol) const;
    const std::set<std::string>& getFOLLOW(const std::string &symbol) const;

    void display() const;
};

#endif
