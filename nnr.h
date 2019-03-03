#ifndef NNR_H
#define NNR_H

#include <math/real.h>

#include <sequence.h>
#include <test.h>

#include <vector>

////////////////

#define USING_PURENUM

////////////////

#ifdef USING_PURENUM
#include <purenum/integer.h>
typedef Integer             PesInteger;
typedef std::vector<uint32> uint32v;
#endif

PesInteger  NNRsize      (const uint32 n,const uint32 length);
PesInteger  NNRpartitions(const uint32 n,const uint32 length);

uint32v     NNRpartition    (const uint32 n,const uint32 length,const PesInteger &index);
PesInteger  NNRpartitionSize(const uint32v &part);

PesSequence NNRsequence(const uint32v &part,const PesInteger &index);
PesSequence NNRsequence(const uint32 n,const uint32 length,const PesInteger &index);

void NNRtest(const ulong n,const ulong length,PesTest &test);
void NNRtest(const ulong n,const ulong length,PesTest &test,const PesInteger &part);

void        NNRprintSequences (const uint32 n,const uint32 length);
void        NNRprintPartitions(const uint32 n,const uint32 length);

#endif
