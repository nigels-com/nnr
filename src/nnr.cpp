#include "nnr.h"

#include <cmath>
#include <cassert>
#include <sstream>
#include <iostream>
using namespace std;

template<typename T>
T fac(const T n)
{
    T ret = 1;
    for(T i = 2; i <= n; ++i)
        ret *= i;
    return ret;
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

uint64_t NNRsize(const uint64_t n, const uint64_t length)
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

uint64_t
NNRpartitions(const uint64_t n, const uint64_t length)
{
    const uint64_t x = length>1   ? fac(length-1) : 1;
    const uint64_t y = n>1        ? fac(n-1)      : 1;
    const uint64_t z = length-n>1 ? fac(length-n) : 1;
    return x/(y*z);
}

// Determine the i'th partition
//
// Output the size of each segment

uint32v NNRpartition(const uint64_t n, const uint64_t length, const uint64_t i)
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

    uint32v s;

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

uint64_t
NNRpartitionSize(const std::vector<uint32_t> &p)
{
    cerr << "Segments: " << NNRprintPartition(p) << endl;

    uint64_t size = 1;
    for (size_t i=2; i<p.size(); i++)
        size *= pow(uint64_t(i),uint64_t(p[i]-1));

    return size;
}

//
// Determine the i'th sequence of a particular NNR partition
//

PesSequence
NNRsequence(const std::vector<uint32_t> &p, const uint64_t i)
{
    assert(p.size()>0);

	// idx is used to choose elements in each segment
    uint64_t idx = i;

	// Output sequence
    PesSequence seq(p.size());

    // For each segment...
    for (uint32_t j=0; j<p.size(); j++)
    {
        // Each segment begins with j'th element
        seq.push_back(j);

		// First segment can only have length of one
		if (j>0)
		{
			// For each subsequent entry in the segment
			for (uint32_t k=1; k<p[j]; k++)
			{
				// Make a choice based on idx and segment
				uint32_t c = idx%j;

				// No repeats, based on previous entry
				seq.push_back((seq.back()+1+c)%(j+1));

				// Adjust idx
				idx /= j;
			}
		}
    }

    return seq;
}

//
// Determine the i'th NNR sequence, based on partitions
//

PesSequence
NNRsequence(const uint32_t n, const uint32_t length, const uint64_t index)
{
    uint64_t idx = index;

    // Try each partition, keeping first segment
    // size one...

    uint64_t partitions = NNRpartitions(n-1, length-1);

    for (uint64_t i = 0; i<partitions ; i++ )
    {
        // Determine segments in i'th partition
        uint32v   part = NNRpartition(n, length, i);
        // Determine size of i'th partition
        uint64_t size = NNRpartitionSize(part);

        // If idx is in range, generate sequence,
        // otherwise keep looking...

        if (idx<size)
            return NNRsequence(part, idx);
        else
            idx -= size;
    }

    // Shouldn't get here
    assert(0);

    return PesSequence(n);
}

/////////////////////////////////////////
// NNR Testing

void NNRtest(const uint64_t n, const uint64_t length, PesTest &test)
{
    assert(length>=n);

    if (length<n)
        return;

    uint64_t partitions = NNRpartitions(n-1,length-1);

    // All partitions
    for (uint64_t i=0; i<partitions; ++i)
        NNRtest(n, length, test, i);
}

void NNRtest(const uint64_t n, const uint64_t length, PesTest &test, const uint64_t part)
{
    cout << "Partition: " << part << endl;

    uint32v  seg  = NNRpartition(n, length, part);
    uint64_t size = NNRpartitionSize(seg);

    cout << "Partition: " << part << " Size: " << size << " Seqments: " << NNRprintPartition(seg) << endl;
    NNRprintPartition(seg);

    for (uint64_t j=0; j<size; ++j)
    {
        test.test(NNRsequence(seg, j));
    }

    cout << "Done" << endl;
}

/////////////////////////////////////////
// DEBUG ROUTINES
/////////////////////////////////////////

void
NNRprintSequences(const uint64_t n, const uint64_t length)
{
    const uint64_t size = NNRsize(n,length);
    for (uint64_t i = 0; i<size; i++)
    {
        cout << NNRsequence(n, length, i) << endl;
    }
}

std::string
NNRprintPartition(const uint32v & p)
{
    std::ostringstream os;

    os << "{ ";
    for (size_t k=0; k<p.size(); k++)
    {
        if (k>0)
            os << ',';
        os << p[k];
    }
    os << " }";
    return os.str();
}

void
NNRprintPartitions(const uint64_t n, const uint64_t length)
{
    uint64_t i    = 0;
    uint64_t size = NNRpartitions(n-1,length-1);

    for (;i<size; i++)
    {
        uint32v p = NNRpartition(n, length, i);
        uint64_t pSize = NNRpartitionSize(p);

        cout << pSize << " { ";

        for (size_t k=0; k<p.size(); k++)
        {
            if (k>0)
                cout << ',';
            cout << p[k];
        }

        cout << " } " << endl;

        for (uint64_t j=0; j<pSize; j++)
        {
            PesSequence seq = NNRsequence(p,j);
            cout << seq << endl;
        }
    }
}
