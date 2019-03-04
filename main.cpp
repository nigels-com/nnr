//
// parameters: k n l
//
// k is size of required permutations
// n is the size of the alphabet
// l is the length of the sequences to start looking in
//   n(n+1)/2 >= l >= n(n-1)+1

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
//#include <algo.h>

#include "nnr.h"
#include "test.h"
#include "sequence.h"

using namespace std;

typedef string      alphabet;
typedef vector<int> histogram;

///////

ostream &operator <<(ostream &os,const string &st)
{
    for (unsigned int n=0; n<st.size(); n++)
        os << st[n];
    return os;
}

ostream &operator <<(ostream &os,const vector<string> &st)
{
    for (unsigned int n=0; n<st.size(); n++)
        cout << st[n] << endl;
    return os;
}

ostream &operator <<(ostream &os,const histogram &h)
{
    for (unsigned int n=0; n<h.size(); n++)
        os << h[n] << ' ';
    return os;
}

ostream &operator <<(ostream &os,const vector<histogram> &h)
{
    for (unsigned int n=0; n<h.size(); n++)
        cout << h[n] << endl;
    return os;
}

double
factorialD(const unsigned long n)
{
    double result = 1.0;

    for (unsigned long i=2; i<=n; i++)
        result *= i;

    return result;
}

void
factorise(unsigned long n)
{
    unsigned long d = 2;

    while (n>1)
    {
        while (n%d==0)
        {
            cout << d << ' ';
            n = n / d;
        }

        d++;
    }
}

////////////////////////////////////////////////
// permute
//
// find all the permutations of a given alphabet
//

vector<string>
permute(const alphabet &alpha)
{
    assert(alpha.size()>0);

    vector<string> perms;

    if (alpha.size()==1)
        perms.push_back(alpha);
    else
        for (unsigned int n=0; n<alpha.size(); n++)
        {
            alphabet subAlpha;

            if (n>0)
                subAlpha.insert(subAlpha.end(),alpha.begin(),alpha.begin()+n);

            if (n<alpha.size()-1)
                subAlpha.insert(subAlpha.end(),alpha.begin()+n+1,alpha.end());

            vector<string> subPermute = permute(subAlpha);

            for (unsigned int o=0; o<subPermute.size(); o++)
            {
                string st;
                st.push_back(alpha[n]);
                st.insert(st.end(),subPermute[o].begin(),subPermute[o].end());
                perms.push_back(st);
            }
        }

    return perms;
}

////////////////////////////////////////////////
// permute
//
// find all the permutations of a certain length,
// given an alphabet
//

vector<string>
permute(const alphabet &alpha,const unsigned int l)
{
    assert(alpha.size()>0);
    assert(l>0);
    assert(l<=alpha.size());

    if (l==alpha.size())
        return permute(alpha);

    vector<string> perms;

    unsigned int n;

    if (l==1)
        for (n=0; n<alpha.size(); n++)
        {
            string st;
            st.push_back(alpha[n]);
            perms.push_back(st);
        }
    else
        for (n=0; n<alpha.size(); n++)
        {
            alphabet subAlpha;

            if (n>0)
                subAlpha.insert(subAlpha.end(),alpha.begin(),alpha.begin()+n);

            if (n<alpha.size()-1)
                subAlpha.insert(subAlpha.end(),alpha.begin()+n+1,alpha.end());

            vector<string> subPermute = permute(subAlpha,l-1);

            for (unsigned int o=0; o<subPermute.size(); o++)
            {
                string st;
                st.push_back(alpha[n]);
                st.insert(st.end(),subPermute[o].begin(),subPermute[o].end());
                perms.push_back(st);
            }
        }

    return perms;
}

////////////////////////////////////////////////
// select
//
// find the i'th sequence of characters using n characters.
//
// we ignore characters with consecutive equal characters
// since they are redundant.
//

string
select(const alphabet &alpha,const unsigned long n,const unsigned long index)
{
    string st;
    unsigned long i = index;
    unsigned long base = alpha.size();

    unsigned long prev = -1;

    for (unsigned int c=0; c<n; c++)
    {
        unsigned long ch = i%base;

#if 0
        if (ch==prev)
            return string();

        prev = ch;
#endif

        st.push_back(alpha[ch]);
        i = i / base;
    }

    return st;
}

////////////////////////////////////////////////
// match
//
// check if permutation p is found in sequence "s"
// by decimation
//

int
match(const string &s,const string &p)
{
    size_t sp = 0;
    size_t pp = 0;

    for (;;)
    {
        if (s[sp]==p[pp])
            pp++;
        sp++;

        if (pp==p.size())
            return 1;

        if (sp==s.size())
            return 0;
    }
}

///////////////////////
// String normalisation
//

string
normalise(const string &st)
{
    unsigned int n = 0;
    unsigned int i = 0;

    for (i=0; i<st.size(); i++)
                n = max(n,(unsigned int) (st[i]-'a'+1));

    string trans;
    trans.insert(trans.begin(),n,'.');

    string output;
    char next = 'a';

    for (i=0; i<st.size(); i++)
    {
        char ch = st[i];

        if (trans[ch-'a']=='.')
            trans[ch-'a'] = next++;

        output.push_back(trans[ch-'a']);
    }

    return output;
}

////////////////
//
// Test sequence
//

int
testSequence
(
    const string &alphabet,
    const vector<string> &permutations,
    const int length
)
{
    unsigned long cases = power(alphabet.size(),length);

    vector<histogram>     hists;
    vector<unsigned long> histFreq;
    vector<string>        solution;
    vector<unsigned long> permFreq;

    permFreq.insert(permFreq.begin(),permutations.size()+1,0);

    int found = 0;

    for (unsigned long c=0; c<cases; c++)
    {
        string st = select(alphabet,length,c);

        // Ignore cases which repeat characters

        if (st.size()==0)
            continue;

        // See if all permutations can be found

#if 0
        int ok = 1;

        for (int p=0; p<permutations.size(); p++)
            if (!match(st,permutations[p]))
            {
                ok = 0;
                break;
            }

        // If all permutations were found
        // print the sequence and keep track
        // of weightings.

        if (ok)
#else
        unsigned int pc = 0;
        for (unsigned int p=0; p<permutations.size(); p++)
            if (match(st,permutations[p]))
                pc++;

        permFreq[pc]++;

//      if (pc==2)
        if (pc==permutations.size())
#endif
        {
            string sol = normalise(st);

            if (find(solution.begin(),solution.end(),sol)==solution.end())
                solution.push_back(sol);

            cout << '.'; cout.flush();
            found++;

            // Build the 'histogram'

            histogram hist;
            hist.insert(hist.begin(),alphabet.size(),0);
            for (int w=0; w<length; w++)
                hist[st[w]-'a']++;

            // Sort the histogram

            sort(hist.begin(),hist.end());

            // Check if we know about it already.

            int dupHist = 0;
            unsigned int x = 0;
            for (; x<hists.size(); x++)
                if (hists[x]==hist)
                {
                    dupHist = 1;
                    break;
                }

            if (dupHist)
                histFreq[x]++;
            else
            {
                hists.push_back(hist);
                histFreq.push_back(1);
            }
        }

    }

    if (found)
    {
        cout << endl;

        sort(solution.begin(),solution.end());

        for (unsigned int s=0; s<solution.size(); s++)
            cout << solution[s] << endl;

        cout << endl;
        cout << found << " optimal strings found for n = ";
        cout << alphabet.size() << endl << endl;

        for (unsigned int h=0; h<hists.size(); h++)
        {
            for (unsigned int w=0; w<hists[h].size(); w++)
                cout << (hists[h])[w] << ' ';
            cout << " (" << histFreq[h] << ')' << endl;
        }

        cout << endl;

    }

    cout << "Permutations" << endl;

    for (unsigned int f=0; f<permFreq.size(); f++)
    {
        cout << f << '\t' << permFreq[f] << '\t';
        if (f>0 && permFreq[f]>0)
        {
            cout << "n! ";
            factorise(permFreq[f]/factorial(alphabet.size()));
        }

        cout << endl;
    }

    cout << endl;

    return found;
}

//////////////////
// histograms
//
// Produce all the possible histograms
// that sum to 'l'
//
// Eg 5
//          5
//          4 1
//          3 2
//          3 1 1
//          2 2 1
//          2 1 1 1
//          1 1 1 1 1

vector<histogram>
histograms(const int l,const int max = 0)
{
    assert(l>0);

    vector<histogram> h;

    if (l<=max)
    {
        histogram hb;
        hb.push_back(l);
        h.push_back(hb);
    }

    if (l==1)
        return h;

    for (int j=min(max ? max : l,l-1); j>0; j--)
    {
        vector<histogram> subH = histograms(l-j,j);

        for (unsigned int k=0; k<subH.size(); k++)
        {
            subH[k].push_back(j);
            if (find(h.begin(),h.end(),subH[k])==h.end())
                h.push_back(subH[k]);
        }
    }

    return h;
}

/////////////////////
// histogramLength
//
// Filter a set of histograms which
// satisfy a length criteria.
//

vector<histogram>
histogramLength(const vector<histogram> &in,const unsigned int n)
{
    vector<histogram> out;

    for (unsigned int l=0; l<in.size(); l++)
        if (in[l].size()==n)
            out.push_back(in[l]);

    return out;
}

//
//              n!
//        ---------------
//        r1! r2! ... ri!

double arrangements(const histogram &h)
{
    double divisor = 1.0;
    unsigned long n = 0;

    for (unsigned int e=0; e<h.size(); e++)
    {
        divisor *= factorialD(h[e]);
        n += h[e];
    }

    return factorialD(n)/divisor;
}

///////////

int main(int argc,char *argv[])
{
    if (argc!=4)
    {
        cout << argv[0] << " k n l " << endl;
        return 0;
    }

    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int l = atoi(argv[3]);

    cout << "k = " << k << endl;
    cout << "n = " << n << endl;
    cout << "l = " << l << endl;

    cout << endl;

#if 0
    //
    // Create alphabet of size n
    //

    alphabet alpha;

    for (int m=0; m<n; m++)
        alpha.push_back('a'+m);

    cout << "'" << alpha << "'" << endl;

    //
    // Find all permutations of interest
    //

    vector<string> perms = permute(alpha,k);

    cout << perms.size() << " permutations... " << endl;

    cout << perms << endl;

    //
    // Test all sequences against each permutation
    //

    for (int o=l;; o++ )
    {
        cout << "Trying strings of length " << o << endl;
        if (testSequence(alpha,perms,o))
            break;
    }
#else

    PesTest test(STRING_TEST_ALL_BY_DECIMATION);
    test.criteria() = PesSequence::permutations(n, k);
    cout << "Permutations: " << test.criteria().size() << endl;

    NNRtest(n, l, test);
#endif

    cout << endl;

    return 1;
}
