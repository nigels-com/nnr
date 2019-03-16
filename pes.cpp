// parameters: n
//
// n is the size of the alphabet

#include <iostream>
#include <string>

#include <cassert>
#include <cstdint>

#include "test.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc!=2)
    {
        cerr << argv[0] << " n" << endl;
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    #ifndef NDEBUG
    cerr << "n = " << n << endl;
    cerr << endl;
    #endif

    PesTest test(STRING_TEST_ALL_BY_DECIMATION);
    test.criteria() = PesSequence::permutations(n);

    while (true)
    {
        std::string line;
        getline(cin, line);

        if (!cin.good() || cin.eof()) break;

        #ifndef NDEBUG
        cerr << line << endl;
        #endif

        PesSequence s(line.c_str());
        if (test.test(s))
        {
            cout << s << endl;
        }
    }

    return EXIT_SUCCESS;
}
