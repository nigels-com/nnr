//
// parameters: n l
//
// n is the size of the alphabet
// l is the length of the sequences

// Permutation Embedding Sequences (PES)
//
// PES embed permutations. A sequence is said to be embedded
// if it can be formed by removing other entries. For example,
// the sequences ab and ac are embedded in abc.
// Neither ba or ca are embedded in abc.
//
// Example: cabcabc embeds all permutations of a, b and c
//   *abc***   *a*c*b*
//   **b*a*c   **bca**
//   cab****   c*b*a**

// Normalised No Repeat (NNR) Sequences
//
// - Contain all n elements at least once
// - Containing no adjacent repetitions of elements
// - Normalised.  The first a somewhere to the left of the first b,
//   the first b somewhere to the left of the first c, and so on.
//
// Examples:
// - caaa is not NNR because b is omitted
// - cabb is not NNR because b is repeated
// - cabcabc is not NNR because the first a is to the right of the first c.
// - abcabca is NNR
// - abcabca embeds all permutations of a, b and c
//     abc****   a*c*b**
//     *b*a*c*   *bca***
//     **cab**   **c*b*a

// PhD Thesis, Chapter 5
// N. Stewart, An Image-Space Algorithm for Hardware-Based Rendering of
// Constructive Solid Geometry, May 2008

#include <vector>
#include <iostream>

#include <cassert>
#include <cstdint>

#include <CLI11.hpp>

using namespace std;

// Output sequence as alpha string

std::ostream &
operator<<(std::ostream &os, const std::vector<uint8_t> & str)
{
    string out;
    out.resize(str.size());
    size_t j = 0;
    for (const auto i : str) out[j++] = 'a' + char(i);
    os << out;
    return os;
}

//
// Calculate the size of NNR search-space
// (The number of NNR sequences of a particular length)
//
// Uses the recursive definition that:
//
// NNR(n,L)  =  (n-1)*NNR(n,L-1) + NNR(n-1,L-1)
//
// This reflects two cases
//   - All NNR sequences of n objects of length L-1, with (n-1) choices
//   - All NNR sequences of (n-1) objects of length L-1, with n'th appended
//
// Base cases:
//
//  NNR(n=1,L=1) = 1
//  NNR(n=1,L>1) = 0
//  NNR(n>1,L=1) = 0
//
// Implicitly, NNR(n,n) = 1 and NNR(n,L<n) = 0

// This algorithm requires O(nL) time,
// O(n) storage.

inline uint64_t NNRsize(const uint64_t n, const uint64_t length)
{
    if (length<n || n<1 || length<1)
        return 0;

    // Allocate storage for two rows of length n
    std::vector<uint64_t> a(n);
    std::vector<uint64_t> b(n);

    // Table is initialised to 1,0,0,...
    uint64_t *current = &a[0];
    current[0] = 1;
    for (size_t k=1; k<n; k++)
        current[k] = 0;

    // Next row follows current row in memory
    uint64_t *next = &b[0];

    // Calculate subsequent rows of table iteratively,
    // based on values of previous row.

    for (size_t i=1; i<length; i++)
    {
        next[0] = 0;

        // Apply recursive definition
        for (size_t j=1; j<n; j++)
            next[j] = current[j]*j + current[j-1];

        // Update row pointer
        swap(current, next);
    }

    // NNR size is n'th element of last row.
    return current[n-1];
}

//
// NNR sequences are composed of segments, according to
// the position of the first occurance of each element:
// a,b,c,...
//
// The first segment consists of a single a
// The second segment begins with b and consists of a's and b's
// The third segment begins with c and consists of a's, b's and c's
//
// Denote segment length as A,B,C...
//
// Then, A>=1, B>=1, C>=1 and so on.
// Also, A+B+C+...= L
//
// Each solution of A,B,C,.. is called a NNR Partition
//
// Task here is to determine the number of NNR partitions
//
// (L-n) is the number of slots not used by "base" permutation: abc...
//
// TODO: Justify...
//
// x/(y*z)
//
// x = (L-1)!
// y = (L-n)!
// z = (n-1)!
//

static const size_t   factorialTableSize = 30;
static const uint64_t factorialTable[1+factorialTableSize] =
{
    uint64_t(1),
    uint64_t(1),
    uint64_t(1)*2,
    uint64_t(1)*2*3,
    uint64_t(1)*2*3*4,
    uint64_t(1)*2*3*4*5,
    uint64_t(1)*2*3*4*5*6,
    uint64_t(1)*2*3*4*5*6*7,
    uint64_t(1)*2*3*4*5*6*7*8,
    uint64_t(1)*2*3*4*5*6*7*8*9,
    uint64_t(1)*2*3*4*5*6*7*8*9*10,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28*29,
    uint64_t(1)*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16*17*18*19*20*21*22*23*24*25*26*27*28*29*30
};

template<typename T>
T fac(const T n)
{
    if (n<=factorialTableSize)
        return factorialTable[n];

    T ret = factorialTable[factorialTableSize];
    for (T i = factorialTableSize+1; i <= n; ++i)
        ret *= i;
    return ret;
}

template<typename T>
T pow(const T base, const T exponent)
{
  if (exponent == 0) return 1;
  if (exponent == 1) return base;

  T tmp = pow(base, exponent/2);

  return exponent%2 == 0 ? tmp * tmp : base * tmp * tmp;
}

inline uint64_t
NNRpartitions(const uint64_t n, const uint64_t length)
{
    const uint64_t x = length>1   ? fac<uint64_t>(length-1) : 1;
    const uint64_t y = n>1        ? fac<uint64_t>(n-1)      : 1;
    const uint64_t z = length-n>1 ? fac<uint64_t>(length-n) : 1;

    #ifndef NDEBUG
    cerr << "x = " << x << endl;
    cerr << "y = " << y << endl;
    cerr << "z = " << z << endl;
    cerr << "p = " << y*z << endl;
    cerr << endl;
    #endif

    return x/(y*z);
}

// Determine the i'th partition
//
// Output the size of each segment

inline std::vector<uint32_t>
NNRpartition(const uint64_t n, const uint64_t length, const uint64_t i)
{
    assert(n>0);        // n must be positive uint64_t
    assert(length>0);   // length must be positive uint64_t
    assert(length>=n);  // must be longer than n to fit all elements

    // Check that requested patition is in range

    #ifndef NDEBUG
    uint64_t parts = NNRpartitions(n, length);
    assert(i<=parts);
    #endif

    //

    uint64_t idx = i;

    uint64_t _n = n;
    uint64_t _length = length;

    // Output vector

    std::vector<uint32_t> s;

    while (_n>1)
    {
        // Choice is constrained by the number of free slots,
        // those not occupied by first a,b,c,...

        uint64_t choices = _length-_n+1;

        // Track the bounds of current solutions
        // in order to locate i'th possible partition

        uint64_t begin = 0;
        uint64_t end = 0;

        // Try each choice until i'th possibility located

        for (uint64_t c=0; c<choices; c++)
        {
            // Find the number of possible partitions
            // of n-1 elements of length-1-c

            uint64_t parts = NNRpartitions(_n-1, _length-1-c);

            begin = end;
            end   += parts;

            // If the i'th partition is in the current
            // arrangement

            if (idx<end)
            {
                // Append segment length to output
                s.push_back(c+1);

                // Now narrow search to sub-problem of
                // n=n-1, length = length - c - 1

                --_n;
                _length -= c+1;

                // Seek i'th possibility in the current
                // context.

                idx -= begin;

                break;
            }
        }
    }

    // Append remaining space to output as last segment

    s.push_back(_length);

    return s;
}

//
// Size of an NNR Partition
//
// Calculate the number of possible sequences given
// a particular partition.
//
// size = 0^(A-1) * 1^(B-1) * 2^(C-1) * 3^(D-1) * 4^(E-1) * ...
//
// Note that there are no choices for first two segments,
// but things quickly muliply out in subsequent segments.
//

inline uint64_t
NNRpartitionSize(const std::vector<uint32_t> &p)
{
    uint64_t size = 1;
    for (size_t i=2; i<p.size(); i++)
        size *= pow<uint64_t>(i, p[i]-1);

    return size;
}

//
// Determine the i'th sequence of a particular NNR partition
//

inline void
NNRsequence(const std::vector<uint32_t> & p, const uint64_t l, const uint64_t i, std::vector<uint8_t> & sequence)
{
    assert(p.size()>0);

    // idx is used to choose elements in each segment
    uint64_t idx = i;

    sequence.resize(l);
    size_t o = 0;

    // For each segment...
    for (uint32_t j=0; j<p.size(); j++)
    {
        // Each segment begins with j'th element
        sequence[o++] = j;

        // First segment can only have length of one
        if (j>0)
        {
            // For each subsequent entry in the segment
            for (uint32_t k=1; k<p[j]; k++)
            {
                // Make a choice based on idx and segment
                uint32_t c = idx%j;

                // No repeats, based on previous entry
                sequence[o] = (sequence[o-1]+1+c)%(j+1);
                o++;

                // Adjust idx
                idx /= j;
            }
        }
    }

    assert(o==l);
}

int main(int argc, char *argv[])
{
    int n = 1;
    int l = 1;
    int p = -1;
    uint64_t start = 0;
    uint64_t end = 0;

    CLI::App app{"nnr - Normalised No Repeats"};
    app.add_option("-n,--n", n, "Alphabet size")->required();
    app.add_option("-l,--l", l, "Sequence length")->required();
    app.add_option("-p,--partition", p, "Limit output to specific partition");
    app.add_option("-s,--start", start, "Start at a specific sequence");
    app.add_option("-e,--end", end, "End at a specific sequence");
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
    cerr << "l = " << l << endl;
    if (start>0 || end>0)
    {
        cerr << "start=" << start << endl;    
        cerr << "end=" << end << endl;
    }
    cerr << endl;
    #endif

    // Output a specific partition

    if (p>=0)
    {
        const uint64_t partitions = NNRpartitions(n-1, l-1);
        if (p>partitions)
        {
            return EXIT_FAILURE;
        }
        const std::vector<uint32_t> part = NNRpartition(n, l, p);
        std::vector<uint8_t> seq;
        const uint64_t size = NNRpartitionSize(part);
        for (uint64_t i = 0; i<size; ++i)
        {
            NNRsequence(part, l, i, seq);
            cout << seq << endl;
        }
        return EXIT_SUCCESS;
    }

    const uint64_t size = NNRsize(n, l);
    const uint64_t partitions = NNRpartitions(n-1, l-1);

    #ifndef NDEBUG
    cerr << "size = " << size << " partitions = " << partitions << endl;
    #endif

    uint64_t i = 0; // NNR sequence index
    std::vector<uint8_t> seq;

    // Output from start to end

    if (start>=0 && end>0)
    {
        for (uint64_t j = 0; j<partitions; ++j)
        {
            // Determine segments in j'th partition
            const std::vector<uint32_t> part = NNRpartition(n, l, j);
            // Determine size of j'th partition
            const uint64_t size = NNRpartitionSize(part);

            #ifndef NDEBUG
            cerr << "partition = " << j << " size = " << size << endl;
            #endif

            if (i+size <= start)
            {
                i += size;
                continue;
            }
            for (uint64_t k = 0; k<size && i<end; ++k, ++i)
            {
                if (i>=start)
                {
                    NNRsequence(part, l, k, seq);
                    cout << seq << endl;
                }
            }
        }

        return EXIT_SUCCESS;
    }

    // Output everything

    for (uint64_t j = 0; j<partitions; ++j)
    {
        // Determine segments in j'th partition
        const std::vector<uint32_t> part = NNRpartition(n, l, j);
        // Determine size of j'th partition
        const uint64_t size = NNRpartitionSize(part);
        for (uint64_t k = 0; k<size; k++, ++i)
        {
            NNRsequence(part, l, k, seq);
            cout << seq << endl;
        }
    }

    assert(i==size);

    return EXIT_SUCCESS;
}
