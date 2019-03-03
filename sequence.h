#pragma once

#include <vector>
#include <string>
#include <iosfwd>

uint64_t factorial(const uint64_t n);
uint64_t power(const uint64_t a,const uint64_t b);

//

class PesSequence : public std::vector<uint64_t>
{
public:
    PesSequence(const uint64_t n = 2,const uint64_t l = 0);
    PesSequence(const char *str);
    PesSequence(std::istream &is);

    uint64_t base() const;

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

    PesSequence  operator+ (const uint64_t &x);
    PesSequence &operator+=(const uint64_t &x);

    //
    // Is another string contained by 'decimation'?
    //

    bool    insideByDecimation(const PesSequence &str) const;
    bool    insideByDecimation(const uint64_t c) const;

    //
    // Swap two entries in the string
    //

    PesSequence transposition(const uint32_t a,const uint32_t b) const;

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

    PesSequence skipNth(const uint32_t pos) const;

    //

    static PesSequence         random(const uint32_t n,const uint32_t l);
    static PesSequence         randomNoDup(const uint32_t n,const uint32_t l);

	//

    static PesSequence              permutation (const uint64_t n,const uint64_t p,const uint64_t k=0);
    static std::vector<PesSequence> permutations(const uint64_t n,const uint64_t k=0);

	//

	static PesSequence adleman74 (const uint64_t n);
	static PesSequence galbiati76(const uint64_t n);
	static PesSequence savage82  (const uint64_t n,const uint64_t k);

    // Comparison

    // Conversion to std::string

    operator std::string() const;

    static bool asAlpha;

private:

    typedef std::vector<uint64_t>::iterator iterator;

    uint64_t _n;
};

extern void displayString(const PesSequence &str);
