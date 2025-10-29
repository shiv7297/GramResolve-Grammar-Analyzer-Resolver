#include "grammar/Grammar.h"

int main() {
    Grammar g;
    g.loadFromFile("data/sample_grammar.txt");
    g.display();
    return 0;
}