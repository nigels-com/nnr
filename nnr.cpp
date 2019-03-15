//
// parameters: n l
//
// n is the size of the alphabet
// l is the length of the sequences

#include <iostream>
#include <cassert>

#include "nnr.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc!=3)
    {
        cerr << argv[0] << " n l" << endl;
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    int l = atoi(argv[2]);

    #ifndef NDEBUG
    cerr << "n = " << n << endl;
    cerr << "l = " << l << endl;
    cerr << endl;
    #endif

    NNRprintSequences(n, l);

    return EXIT_SUCCESS;
}
