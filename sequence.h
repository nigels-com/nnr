#ifndef PES_SEQUENCE_H
#define PES_SEQUENCE_H

#include <vector>
#include <string>
#include <iosfwd>

typedef unsigned long ulong;
typedef unsigned int  uint;

//

ulong factorial(const ulong n);
ulong power(const ulong a,const ulong b);

//

class PesSequence : public std::vector<ulong>
{
public:
    PesSequence(const ulong n = 2,const ulong l = 0);
    PesSequence(const char *str);
    PesSequence(std::istream &is);

    ulong base() const;

    // IO

    std::istream &read(std::istream &is);
    std::ostream &write(std::ostream &os) const;

    static std::ostream &write(std::ostream &os,const std::vector<PesSequence> &str);
    static std::istream &read (std::istream &is,      std::vector<PesSequence> &str);

    friend std::ostream &operator<<(std::ostream &os,      const PesSequence  &str);
    friend std::ostream &operator<<(std::ostream &os,std::vector<PesSequence> &str);

    // Lexicographic neighbours

    bool operator++();
    bool operator--();

    // Comparison

    bool operator==(const PesSequence &str) const;
    bool operator!=(const PesSequence &str) const;
    bool operator==(const std::vector<PesSequence> &str) const;
    bool operator!=(const std::vector<PesSequence> &str) const;

    // Catenate

    PesSequence  operator+ (const PesSequence &b);
    PesSequence &operator+=(const PesSequence &b);

    // Offset each element

    PesSequence  operator+ (const ulong &x);
    PesSequence &operator+=(const ulong &x);

    //
    // Is another string contained by 'decimation'?
    //

    bool    insideByDecimation(const PesSequence &str) const;
    bool    insideByDecimation(const ulong c) const;

    //
    // Swap two entries in the string
    //

    PesSequence transposition(const uint a,const uint b) const;

    //
    // Remap entries according to provided
    // string.  'a' becomes first entry,
    // 'b' the second, and so on...
    //

    PesSequence substitute(const PesSequence s) const;

    //
    // Substitute such that 'a' is first unqiue, 'b' is second, etc..
    //

    PesSequence normalise() const;

    //
    // Mirror the string such that 'abcde' -> 'edcba'
    //

    PesSequence mirror() const;

    //
    // Rotate left and right 'abc....x' -> 'xabc...'
    //

    PesSequence rotateLeft() const;
    PesSequence rotateRight() const;

    //
    // Extract all but n'th character in string
    //

    PesSequence skipNth(const uint pos) const;

    //

    static PesSequence         random(const uint n,const uint l);
    static PesSequence         randomNoDup(const uint n,const uint l);

	//

    static PesSequence              permutation (const ulong n,const ulong p,const ulong k=0);
    static std::vector<PesSequence> permutations(const ulong n,const ulong k=0);

	//

	static PesSequence adleman74 (const ulong n);
	static PesSequence galbiati76(const ulong n);
	static PesSequence savage82  (const ulong n,const ulong k);

    // Comparison

    // Conversion to std::string

    operator std::string() const;

    static bool asAlpha;

private:

    typedef std::vector<ulong>::iterator iterator;

    ulong _n;
};

extern void displayString(const PesSequence &str);

#endif
