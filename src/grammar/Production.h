#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <vector>
#include <string>

class Production {
private:
    std::string lhs;
    std::vector<std::vector<std::string>> rhs; // multiple alternatives

public:
    Production() = default;
    Production(const std::string &left) : lhs(left) {}

    void addAlternative(const std::vector<std::string> &symbols);
    void display() const;

    const std::string& getLHS() const { return lhs; }
    const std::vector<std::vector<std::string>>& getRHS() const { return rhs; }
};

#endif
