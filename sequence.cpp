#include "sequence.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

//
// Constructors
//

bool PesSequence::asAlpha = true;

PesSequence::PesSequence(const ulong n,const ulong l)
: _n(n)
{
    reserve(l+n);

    for (uint i=0; i<l; i++)
      push_back(0);

    // insert(begin(),l,0);
}

PesSequence::PesSequence(const char *str)
: _n(0)
{
    if (!str)
        return;

    const char &firstChar = *str;
    bool alpha = (firstChar>='a' && firstChar<='z');

    const unsigned char *pos = (const unsigned char *) str;

    while (*pos!=0)
    {
        ulong c = (ulong) (alpha ? (*pos)-'a' : (*pos)-'0');
        if (c+1>_n)
            _n = c+1;
        push_back(c);
        pos++;
    }
}

PesSequence::PesSequence(istream &is)
: _n(0)
{
    read(is);
}

ulong PesSequence::base() const { return _n; }

//
// IO
//

istream &
PesSequence::read(istream &is)
{
    ulong n,l;

    is.read((char *) &n,sizeof(ulong));
    is.read((char *) &l,sizeof(ulong));

    _n = n;
    erase(begin(),end());

    ulong *str = new ulong[l];
    is.read((char *) str,sizeof(ulong)*l);
    insert(begin(),str,str+l);
    delete [] str;

    return is;
}

ostream &
PesSequence::write(ostream &os) const
{
    os.write((char *) &_n,sizeof(ulong));
    ulong l = size();
    os.write((char *) &l,sizeof(ulong));
    os.write((char *) &((*this)[(uint) 0]),sizeof(ulong)*l);
    return os;
}

istream &
PesSequence::read(istream &is,std::vector<PesSequence> &str)
{
    str.erase(str.begin(),str.end());

    for (;;)
    {
        PesSequence s(is);
        if (!is.eof())
            str.push_back(s);
        else
            break;
    }

    #ifdef _DEBUG
    cout << str.size() << " strings read." << endl;
    #endif

    return is;
}

ostream &
PesSequence::write(ostream &os,const vector<PesSequence> &str)
{
    for (ulong s=0; s<str.size(); s++)
        str[s].write(os);

    #ifdef _DEBUG
    cout << str.size() << " strings written." << endl;
    #endif

    return os;
}

//
// Convert OString to displayable string
//
// REVISIT - resolve display for large n
//

PesSequence::operator std::string() const
{
    string str("");

    for (uint i=0; i<size(); i++)
        if (asAlpha)
            str += 'a'+(char)((*this)[i]);
        else
            str += '0'+(char)((*this)[i]);

    return str;
}

ostream &
operator<<(ostream &os,const PesSequence &str)
{
    return os << (string) str;
}

//
// Find next/previous string, lexigraphically.
//
// Return true if size of string has changed.
//

bool
PesSequence::operator++()
{
    for (iterator i=end() - 1;;i--)
    {
        if ( ++(*i) < _n )
            return false;

        *i = 0;

        if (i==begin())
            break;
    }

    insert(begin(),0);

    return true;
}

bool
PesSequence::operator--()
{
    for (iterator i=end() - 1;;i--)
    {
        if ( --(*i) < _n )
            return false;

        *i = _n - 1;

        if (i==begin())
            break;
    }

    erase(begin());

    return true;
}

// Comparison

bool
PesSequence::operator==(const PesSequence &str) const
{
    if (_n!=str._n) return false;
    if (size()!=str.size()) return false;

    return !memcmp(&(*this)[0],&str[0],size()*sizeof(ulong));
}

bool
PesSequence::operator!=(const PesSequence &str) const
{
    return !((*this)==str);
}

bool
PesSequence::operator==(const vector<PesSequence> &str) const
{
    for (ulong s=0; s<str.size(); s++)
        if ((*this)==str[s])
            return true;

    return false;
}

bool
PesSequence::operator!=(const vector<PesSequence> &str) const
{
    return !((*this)==str);
}

//
// Catenation
//

PesSequence
PesSequence::operator+(const PesSequence &b)
{
    PesSequence out(*this);
    out.insert(out.end(),b.begin(),b.end());
    return out;
}

PesSequence &
PesSequence::operator+=(const PesSequence &b)
{
    insert(end(),b.begin(),b.end());
    return *this;
}

// Offset each element

PesSequence
PesSequence::operator+ (const ulong &x)
{
    PesSequence out(*this);
    for (uint i=0; i<out.size(); i++)
        out[i] = (out[i]+x)%_n;
    return out;
}

PesSequence &
PesSequence::operator+=(const ulong &x)
{
    _n += x;            // REVISIT

    for (uint i=0; i<size(); i++)
        (*this)[i] = ((*this)[i]+x)%_n;

    return *this;
}

//
//

bool
PesSequence::insideByDecimation(const PesSequence &str) const
{
    unsigned int tPos=0;
    unsigned int sPos=0;

    if ( str.size() > size() )
        return false;

    if ( str==(*this) )
        return true;

    for (;;)
    {
        if ((*this)[tPos]==str[sPos])
            sPos++;
        tPos++;

        if (sPos==str.size())
            return true;

        if (tPos==size())
            return false;
    }
}

bool
PesSequence::insideByDecimation(const ulong x) const
{
    for (uint i=0; i<size(); i++)
        if ((*this)[i]==x)
            return true;

    return false;
}

PesSequence
PesSequence::normalise() const
{
    PesSequence trans(_n);
//  trans.insert(trans.begin(),_n,(ulong) '.');

    {
        for (uint i=0; i<_n; i++)
            trans.push_back('.');
    }

    PesSequence out(_n);
    ulong next = 0;

    for (uint i=0; i<size(); i++)
    {
        uint ch = (*this)[i];

        if (trans[ch]=='.')
            trans[ch] = next++;

        out.push_back(trans[ch]);
    }

    return out;
}

// Mirror the string

PesSequence
PesSequence::mirror() const
{
    PesSequence out(_n);

    for (uint i=0; i<size(); i++)
        out.push_back((*this)[size()-1-i]);

    return out;
}

PesSequence
PesSequence::rotateLeft() const
{
    PesSequence out(_n);
    if (!size())
        return out;

    for (uint i=1; i<size(); i++)
        out.push_back((*this)[i]);

    out.push_back(front());

    return out;
}

PesSequence
PesSequence::rotateRight() const
{
    PesSequence out(_n);
    if (!size())
        return out;

    out.push_back(back());

    for (uint i=0; i<size()-1; i++)
        out.push_back((*this)[i]);

    return out;
}

PesSequence
PesSequence::skipNth(const uint pos) const
{
    PesSequence out(_n);

    for (uint i=0; i<size(); i++)
        if (i!=pos)
            out.push_back((*this)[i]);

    return out;
}

PesSequence
PesSequence::random(const uint n,const uint l)
{
    PesSequence out(n);

    for (uint p=0; p<l; p++)
        out.push_back(rand()%n);

    return out;
}

PesSequence
PesSequence::randomNoDup(const uint n,const uint l)
{
    PesSequence out(n,l);

    ulong last = rand()%n;
    out[0] = last;

    for (uint p=1; p<l; p++)
    {
        out[p] = (last + 1 + rand()%(n-1))%n;
        last = out[p];
    }

    return out;
}

PesSequence 
PesSequence::adleman74(const ulong n)
{
    assert(n>=3);

    // Storage for generated sequence.

    PesSequence tmp(n);
    tmp.reserve(n*n-2*n+4);

    // The sequence begins with the n'th symbol
	tmp.push_back(n-1);

	// Repeat the string abcd..abcd..
	// With the n'th symbol inserted after
	// the i'th occurance of the symbol n-i
	// where 1 <= i <= n-2

	const int size = n*n-3*n+4;

	int k=n-2;
    for (int i=0; i<size; i++)
    {
		const int j = i%(n-1);
		tmp.push_back(j);

		// Insert the n'th symbol

		if (j==k && k>0)
		{
			tmp.push_back(n-1);
			k--;
		}
    }

    // The sequence ends with the n'th symbol
	tmp.push_back(n-1);

    return tmp;
}

PesSequence 
PesSequence::galbiati76(const ulong n)
{
    assert(n>=4);

    // Storage for generated sequence.

    PesSequence tmp(n);
    tmp.reserve(n*n-2*n+4);

    // Galbiati uses four arbitrary elements: 1,2,3,4
    // We use zero as base: 0,1,2,3

    tmp.push_back(1);
    tmp.push_back(0);
    tmp.push_back(2);

    // Append each segment

    const int V[3] = { 1,2,3 };
    int v = 2;

    for (int i=0; i<n-1; i++)
    {
        // Append U

        for (int j=4; j<n; j++)
            tmp.push_back(j);

        // Append next element in V
        tmp.push_back(V[v++%3]);
        tmp.push_back(0);
        tmp.push_back(V[v++%3]);
    }

    return tmp;
}

PesSequence 
PesSequence::savage82(const ulong n,const ulong k)
{
    assert(n>=3 && k>=3);

    // Storage for generated sequence.

	PesSequence tmp(n);
    tmp.reserve(k*n-2*k+4);

	// The n elements are partitioned into two
	// sets: A = ab..x_{k-1}, B = x_{k}..x_{n}

    // The sequence begins with B
	int m;
	for (m=k-1; m<n; m++)
		tmp.push_back(m);	

	// Repeat the string AA..
	// With the B symbol inserted after
	// the i'th occurance of the symbol n-i
	// where 1 <= i <= k-2

	const int size = k*k-3*k+4;

	int p=k-2;
    for (int i=0; i<size; i++)
    {
		const int j = i%(k-1);
		tmp.push_back(j);

		// Insert B

		if (j==p && p>0)
		{
			for (m=k-1; m<n; m++)
				tmp.push_back(m);	
			p--;
		}
    }

    // The sequence ends with B
	for (m=k-1; m<n; m++)
		tmp.push_back(m);	

    return tmp;
}

PesSequence
PesSequence::permutation(const ulong n,const ulong p,const ulong k)
{
    //
    // Check that k is not too large
    //

    assert(k<=n);

    if (k>n)
        return PesSequence();

    //
    // Do it
    //

    ulong length = k ? k : n;

    //
    // Create 'abcde...'
    //

    PesSequence perm(n);
    for (ulong c=0; c<n; c++)
        perm.push_back(n-1-c);

    //
    //
    //

    PesSequence out(n);
    ulong seed = p;

    while (out.size()<length)
    {
        ulong choices = perm.size();
        ulong select = seed%choices;
        seed = seed/choices;

        out.insert(out.begin(),perm[(uint) select]);
        perm.erase(perm.begin()+select);
    }

    return out;
}

vector<PesSequence>
PesSequence::permutations(const ulong n,const ulong k)
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
        ulong nf = factorial(n);
        vector<PesSequence> perms;
        perms.reserve(nf);

        for (ulong p=0; p<nf; p++)
            perms.push_back(permutation(n,p));

        return perms;
    }
    else

    //
    // Do every arrangement of n
    // select k.
    //

    {
        ulong arr = factorial(n)/factorial(n-k);
        vector<PesSequence> perms;
        perms.reserve(arr);

        for (ulong p=0; p<arr; p++)
            perms.push_back(permutation(n,p,k));

        return perms;
    }
}

PesSequence
PesSequence::transposition(const uint a,const uint b) const
{
    PesSequence out = *this;

    assert(out.size());
    assert(a<out.size());
    assert(b<out.size());

    ulong tmp = out[a];
    out[a] = out[b];
    out[b] = tmp;

    return out;
}

//
//

PesSequence
PesSequence::substitute(const PesSequence s) const
{
    PesSequence out(_n);

    //
    // Check that remapping is consistent with
    // size of alphabet
    //

    assert(s._n<=_n);

    if (s._n>_n)
        return out;

    //
    // Check that remapping caters for every
    // character in alphabet.
    //

    assert(_n==s.size());

    if (_n!=s.size())
        return out;

    // Do it

    for (ulong c=0; c<size(); c++)
        out.push_back(s[(*this)[c]]);

    return out;
}

//
//

ostream &operator<<(ostream &os,vector<PesSequence> &str)
{
    for (uint s=0; s<str.size(); s++)
        os << str[s] << endl;

    return os;
}

void displayString(const PesSequence &str)
{
    cout << str;

    // Output length

    cout << " (" << str.size() << ")";

    // Output in normalised form

    PesSequence n = str.normalise();
    if (n!=str)
        cout << " -> " << n;

    cout << endl;
}

//
//
//


ulong
factorial(const ulong n)
{
    ulong fact = 1;

    for (ulong f=n; f>1; f--)
        fact *= f;

    return fact;
}

ulong
power(const ulong a,const ulong b)
{
    if (b==0)
        return 1;

    ulong pow = 1;

    for (ulong p=0; p<b; p++)
        pow *= a;

    return pow;
}

