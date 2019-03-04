#include "test.h"
#include "AltGroup.h"

#include <algorithm>
#include <iostream>
using namespace std;

ulong PesTest::_testCountTotal = 0;

PesTest::PesTest(const PesTestType testType,const bool histogram)
: _testType(testType),
  _testCount(0),
  _enableCollection(true),
  _enableHistogram(histogram)
{
}

vector<PesSequence> &PesTest::criteria() { return _criteria; }
vector<PesSequence> &PesTest::accepted() { return _accepted; }

ulong
PesTest::fitness(const PesSequence &str)
{
    _testCount++;
    _testCountTotal++;

    ulong match = 0;

    for (ulong p=0; p<_criteria.size(); p++)
        if (str.insideByDecimation(_criteria[p]))
            match++;

    if (match==_criteria.size())
        addAccepted(str);

    return match;
}

bool
PesTest::test(const PesSequence &str)
{
    _testCount++;
    _testCountTotal++;

//    if (!_criteria.size())
//        return false;

    switch (_testType)
    {
        case STRING_TEST_ALL_BY_DECIMATION:
        {
            ulong match = 0;

            for (ulong p=0; p<_criteria.size(); p++)
                if (!str.insideByDecimation(_criteria[p]))
                {
                    if (!_enableHistogram)
                        break;
                }
                else
                    match++;

            if (_enableHistogram)
            {
                if (!_histogram.size())
                {
                    _histogram.clear();
                    _histogram.resize(_criteria.size()+1,0);
                }

                _histogram[match]++;
            }

            if (match==_criteria.size())
            {
                addAccepted(str);
                return true;
            }
        }
        break;

        case STRING_TEST_NONE_BY_DECIMATION:
        {
            for (ulong p=0; p<_criteria.size(); p++)
                if (str.insideByDecimation(_criteria[p]))
                    return 0;

            addAccepted(str);
            return true;
        }
        break;

        case STRING_TEST_DISABLE:
        default:
        {
                        addAccepted(str);
                        return true;
        }
        break;
    }

    return false;
}

void
PesTest::histogram(ostream &os)
{
    if (_histogram.size())
        for (ulong f=0; f<_histogram.size(); f++)
            cout << f << ' ' << _histogram[f] << endl;
}

void
PesTest::addAccepted(const PesSequence &str)
{
    if (_enableCollection)
        if (find(_accepted.begin(),_accepted.end(),str)!=_accepted.end())
            return;

    // Check if it is shorter than any found so far

    if (_accepted.size())
    {
        if (str.size()<_accepted[0].size())
            _accepted = vector<PesSequence>();
    }

    // Check if it is no longer than those known already

    if (_accepted.size())
    {
        if (str.size()>_accepted[0].size())
            return;
    }

    _accepted.push_back(str);

    displayString(str);
}

void
PesTest::reset()
{
    _testCount = 0;
    _histogram.clear();
}

ulong &PesTest::testCount()      { return _testCount; }
ulong &PesTest::testCountTotal() { return _testCountTotal; }

PesTestType &PesTest::testType()         { return _testType; }
bool        &PesTest::enableHistogram()  { return _enableHistogram; }
bool        &PesTest::enableCollection() { return _enableCollection; }

