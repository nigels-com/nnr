// parameters: n
//
// n is the size of the alphabet

#include <vector>
#include <string>
#include <iostream>

#include <cstring>
#include <cassert>
#include <cstdint>

#include <CLI11.hpp>

using namespace std;

// Output sequence as alpha string

inline std::ostream &
operator<<(std::ostream &os, const std::vector<uint8_t> & str)
{
    string out;
    out.resize(str.size());
    size_t j = 0;
    for (const auto i : str) out[j++] = 'a' + char(i);
    os << out;
    return os;
}

static const size_t   factorialTableSize = 30;
static const uint64_t factorialTable[1+factorialTableSize] =
{
    1lu,
    1lu,
    1lu*2,
    1lu*2*3,
    1lu*2*3*4,
    1lu*2*3*4*5,
    1lu*2*3*4*5*6,
    1lu*2*3*4*5*6*7,
    1lu*2*3*4*5*6*7*8,
    1lu*2*3*4*5*6*7*8*9,
    1lu*2*3*4*5*6*7*8*9*10,
    1lu*2*3*4*5*6*7*8*9*10*11,
    1lu*2*3*4*5*6*7*8*9*10*11*12,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28*29,
    1lu*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28*29*30
};

template<typename T>
T factorial(const T n)
{
    if (n<=factorialTableSize)
        return factorialTable[n];

    T ret = factorialTable[factorialTableSize];
    for (T i = factorialTableSize+1; i <= n; ++i)
        ret *= i;
    return ret;
}

inline
std::vector<uint8_t>
permutation(const uint64_t n, const uint64_t p, const uint64_t k = 0)
{
    //
    // Check that k is not too large
    //

    assert(k<=n);
    if (k>n)
        return std::vector<uint8_t>();

    //
    // Do it
    //

    const uint64_t length = k ? k : n;

    //
    // Create 'abcde...'
    //

    std::vector<uint8_t> perm;
    for (uint64_t c=0; c<n; c++)
        perm.push_back(n-1-c);

    //

    std::vector<uint8_t> out;
    uint64_t seed = p;

    while (out.size()<length)
    {
        const uint64_t choices = perm.size();
        const uint64_t select = seed%choices;
        seed = seed/choices;

        out.push_back(perm[select]);
        perm.erase(perm.begin()+select);
    }

    return out;
}

vector<std::vector<uint8_t>>
permutations(const uint64_t n, const uint64_t k = 0)
{
    //
    // Default behaviour is when k
    // is assumed to be n
    //
    // k is the 'length' of desired
    // permutations.  k!=n implies
    // 'arrangements'.
    //

    if (k==0 || k==n)
    {
        const uint64_t size = factorial(n);
        vector<std::vector<uint8_t>> perms;
        perms.reserve(size);

        for (uint64_t p=0; p<size; p++)
            perms.push_back(permutation(n,p));

        return perms;
    }

    //
    // Do every arrangement of n
    // select k.
    //

    const uint64_t size = factorial(n)/factorial(n-k);
    vector<std::vector<uint8_t>> perms;
    perms.reserve(size);

    for (uint64_t p=0; p<size; p++)
        perms.push_back(permutation(n, p, k));

    return perms;
}

std::vector<uint8_t>
fromString(const char *str)
{
    if (!str)
        return std::vector<uint8_t>();

    const char &first = *str;
    const bool alpha = (first>='a' && first<='z');

    std::vector<uint8_t> tmp;
    const unsigned char *pos = (const unsigned char *) str;
    while (*pos!=0)
    {
        uint8_t c = (uint8_t) (alpha ? (*pos)-'a' : (*pos)-'0');
        tmp.push_back(c);
        pos++;
    }
    return tmp;
}

// p is embedded in [begin, end)

bool
embeddedSequence(const uint8_t * begin, const uint8_t * end, const std::vector<uint8_t> & p)
{
    const size_t size = end-begin;

    if (p.size() > size)
        return false;

    if (p.size() == size && !memcmp(begin, &p[0], size))
        return true;

    unsigned int n = 0;
    for (const uint8_t * i = begin; i != end; ++i)
    {
        if (*i==p[n])
        {
            n++;
            if (n==p.size())
            {
                return true;
            }
        }
    }

    return false;
}

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

    vector<std::vector<uint8_t>> criteria;
    if (k>0)
    {
        criteria = permutations(n, k);
    }
    else
    {
        criteria = permutations(n);
    }

    const size_t size = k>0 ? k : n;

    #ifndef NDEBUG
    for (const auto & i : criteria)
      cerr << i << endl;
    #endif

    while (true)
    {
        std::string line;
        getline(cin, line);

        if (!cin.good() || cin.eof()) break;

        #ifndef NDEBUG
        cerr << line << endl;
        #endif

        std::vector<uint8_t> s = fromString(line.c_str());
        for (const auto & i : criteria)
            if (!embeddedSequence(&s[0], &s[0]+s.size(), i))
            {
                goto fail;
            }


        cout << s << endl;
fail:
        continue;
    }

    return EXIT_SUCCESS;
}
