//
// parameters: n l
//
// k is the size of the alphabet
// n is the size of the alphabet
// l is the length of the sequences

#include <iostream>
#include <string>
#include <cassert>

#include "nnr.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc!=4)
    {
        cerr << argv[0] << " k n l" << endl;
        return EXIT_FAILURE;
    }

    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int l = atoi(argv[3]);

    #ifndef NDEBUG
    cerr << "k = " << k << endl;
    cerr << "n = " << n << endl;
    cerr << "l = " << l << endl;
    cerr << endl;
    #endif

    PesTest test(STRING_TEST_ALL_BY_DECIMATION);
    test.criteria() = PesSequence::permutations(n, k);

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
