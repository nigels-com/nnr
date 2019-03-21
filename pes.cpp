// parameters: n
//
// n is the size of the alphabet

#include <iostream>
#include <string>

#include <cassert>
#include <cstdint>

#include <CLI11.hpp>

#include "sequence.h"

using namespace std;

int main(int argc, char *argv[])
{
    int n = 1;
    int k = -1;

    CLI::App app{"pes - Permutation Embedding Sequences"};
    app.add_option("-n,--n", n, "Alphabet size")->required();
    app.add_option("-k,--k", k, "k");
    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }

    #ifndef NDEBUG
    cerr << "n = " << n << endl;
    cerr << endl;
    #endif

    std::vector<PesSequence> criteria;
    if (k>0)
    {
        criteria = PesSequence::permutations(n, k);
    }
    else
    {
        criteria = PesSequence::permutations(n);
    }

    while (true)
    {
        std::string line;
        getline(cin, line);

        if (!cin.good() || cin.eof()) break;

        #ifndef NDEBUG
        cerr << line << endl;
        #endif

        PesSequence s(line.c_str());
        for (const auto & i : criteria)
            if (!s.insideByDecimation(i))
            {
                goto fail;
            }


        cout << s << endl;
fail:
        continue;
    }

    return EXIT_SUCCESS;
}
