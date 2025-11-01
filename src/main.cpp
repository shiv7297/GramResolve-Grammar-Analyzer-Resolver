#include "grammar/Grammar.h"
#include "analysis/FirstFollow.h"

int main() {
    Grammar g;
    g.loadFromFile("data/sample_grammar.txt");
    g.display();

    FirstFollowEngine ff;
    ff.computeFIRST(g);
    ff.computeFOLLOW(g);
    ff.display();

    return 0;
}
