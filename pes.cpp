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

// Input sequence as alpha string

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

// Factorial

template<typename T>
T factorial(const T n)
{
    T ret = 1;
    for (T i = 2; i <= n; ++i)
        ret *= i;
    return ret;
}

inline
bool
permutation(uint8_t * buffer, const uint64_t n, const uint64_t p, const uint64_t k = 0)
{
    //
    // Check that k is not too large
    //

    assert(k<=n);
    if (k>n)
        return false;

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

    for (uint8_t *i = buffer; i<buffer+length; ++i)
    {
        const uint64_t choices = perm.size();
        const uint64_t select = seed%choices;
        seed = seed/choices;

        *i = perm[select];
        perm.erase(perm.begin()+select);
    }
    return true;
}

bool
permutations(std::vector<uint8_t> & buffer, const uint64_t n, const uint64_t k = 0)
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
        buffer.resize(size*n);

        uint8_t *i = &buffer[0];
        for (uint64_t p=0; p<size; p++, i += n)
            permutation(i, n, p);

        return true;
    }

    //
    // Do every arrangement of n
    // select k.
    //

    const uint64_t size = factorial(n)/factorial(n-k);
    buffer.resize(size*k);

    uint8_t *i = &buffer[0];
    for (uint64_t p=0; p<size; p++, i += k)
        permutation(i, n, p, k);

    return true;
}

inline bool
shuffle(std::vector<uint8_t> & buffer, const uint64_t n, const uint64_t k = 0)
{
    const uint64_t length = k>0 ? k : n;

    uint8_t *begin = &buffer[0];
    uint8_t *end   = begin + buffer.size();

    uint64_t m = buffer.size()/length;
    vector<uint64_t> tmp(length);

    for (uint8_t *i = begin; i<end; i += length, --m)
    {
        uint8_t *j = i + (rand()%m)*length;
        memcpy(&tmp[0], i,       length);
        memcpy(i,       j,       length);
        memcpy(j,       &tmp[0], length);
    }

    return true;
}

// [nBegin, nEnd] is embedded in [hBegin, hEnd)

bool
embeddedSequence(
    const uint8_t * hBegin, const uint8_t * hEnd,
    const uint8_t * nBegin, const uint8_t * nEnd)
{
    const size_t hSize = hEnd-hBegin;
    const size_t nSize = nEnd-nBegin;

    if (nSize > hSize)
        return false;

    if (nSize == hSize && !memcmp(hBegin, nBegin, nSize))
        return true;

    const uint8_t * j = nBegin;
    for (const uint8_t * i = hBegin; i != hEnd; ++i)
    {
        if (*i==*j)
        {
            j++;
            if (j==nEnd)
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

    std::vector<uint8_t> buffer;
    if (k>0)
    {
        permutations(buffer, n, k);
        shuffle(buffer, n, k);
    }
    else
    {
        permutations(buffer, n);
        shuffle(buffer, n);
    }

    const size_t size = k>0 ? k : n;

    while (true)
    {
        std::string line;
        getline(cin, line);

        if (!cin.good() || cin.eof()) break;

        #ifndef NDEBUG
        cerr << line << endl;
        #endif

        std::vector<uint8_t> s = fromString(line.c_str());

        const uint8_t * begin = &buffer[0];
        const uint8_t * end   = begin + buffer.size();
        for (const uint8_t *i = begin; i<end; i+=size)
            if (!embeddedSequence(&s[0], &s[0]+s.size(), i, i + size))
            {
                goto fail;
            }


        cout << s << endl;
fail:
        continue;
    }

    return EXIT_SUCCESS;
}
