#ifndef PES_TEST_H
#define PES_TEST_H

#include <vector>
#include <iosfwd>

#include <sequence.h>

typedef enum
{
    STRING_TEST_ALL_BY_DECIMATION,
    STRING_TEST_NONE_BY_DECIMATION,
    STRING_TEST_DISABLE,
    STRING_TEST_TYPES
} PesTestType;

class PesTest
{
public:

  PesTest(const PesTestType testType = STRING_TEST_ALL_BY_DECIMATION,const bool histogram = false);
  std::vector<PesSequence> &criteria();
  std::vector<PesSequence> &accepted();

  bool  test(const PesSequence &str);
  ulong fitness(const PesSequence &str);

         ulong &testCount();
  static ulong &testCountTotal();

  void reset();

  PesTestType &testType();
  bool        &enableHistogram();
  bool        &enableCollection();

  void histogram(std::ostream &os);

private:

  PesTestType              _testType;
  std::vector<PesSequence> _criteria;
  std::vector<PesSequence> _accepted;

         ulong _testCount;
  static ulong _testCountTotal;

  bool _enableCollection;

  void addAccepted(const PesSequence &str);

  // Histogram Functionality

  bool               _enableHistogram;
  std::vector<ulong> _histogram;
};

#endif
