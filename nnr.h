#pragma once

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

#include "sequence.h"
#include "test.h"

#include <cstdint>
#include <vector>

typedef std::vector<uint32_t> uint32v;

uint64_t NNRsize      (const uint64_t n, const uint64_t length);
uint64_t NNRpartitions(const uint64_t n, const uint64_t length);

uint32v  NNRpartition    (const uint64_t n, const uint64_t length, const uint64_t index);
uint64_t NNRpartitionSize(const uint32v &part);

PesSequence NNRsequence(const uint32v &part,const uint64_t index);
PesSequence NNRsequence(const uint32_t n,const uint32_t length,const uint64_t index);

void NNRtest(const uint64_t n, const uint64_t length, PesTest &test);
void NNRtest(const uint64_t n, const uint64_t length, PesTest &test,const uint64_t part);

void NNRprintSequences (const uint64_t n, const uint64_t length);
void NNRprintPartitions(const uint64_t n, const uint64_t length);

std::string NNRprintPartition (const uint32v & p);
