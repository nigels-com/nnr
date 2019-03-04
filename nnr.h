#pragma once

#include "sequence.h"
#include "test.h"

#include <cstdint>
#include <vector>

////////////////

//#define USING_PURENUM

////////////////

#ifdef USING_PURENUM
#include <purenum/integer.h>
typedef Integer               PesInteger;
typedef std::vector<uint32_t> uint32v;
#else
typedef uint64_t              PesInteger;
typedef std::vector<uint32_t> uint32v;
#endif

uint64_t NNRsize      (const uint64_t n,const uint64_t length);
uint64_t NNRpartitions(const uint64_t n,const uint64_t length);

uint32v     NNRpartition    (const uint32_t n,const uint32_t length,const PesInteger &index);
PesInteger  NNRpartitionSize(const uint32v &part);

PesSequence NNRsequence(const uint32v &part,const PesInteger &index);
PesSequence NNRsequence(const uint32_t n,const uint32_t length,const PesInteger &index);

void NNRtest(const uint64_t n, const uint64_t length, PesTest &test);
void NNRtest(const uint64_t n, const uint64_t length, PesTest &test,const PesInteger &part);

void        NNRprintSequences (const uint32_t n,const uint32_t length);
void        NNRprintPartitions(const uint32_t n,const uint32_t length);
