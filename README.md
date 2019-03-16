# nnr

[![Build Status](https://travis-ci.org/nigels-com/nnr.svg?branch=master)](https://travis-ci.org/nigels-com/nnr)

# Concepts

## Permutations

## Permutation Embedding Sequences (PES)

PES embed permutations. A sequence is said to be embedded
if it can be formed by removing other entries. For example,
the sequences ab and ac are embedded in abc.
Neither ba or ca are embedded in abc.

Example: cabcabc embeds all permutations of a, b and c

|       | `cabcabc`  |
| ----- | -----------|
| `abc` | `*abc***`  |
| `acb` | `*a*c*b*`  |
| `bac` | `**b*a*c`  |
| `bca` | `**bca**`  |
| `cab` | `cab****`  |
| `cba` | `c*b*a**`  |

## Normalised No Repeat (NNR) Sequences

- Contain all n elements at least once
- Contain no adjacent repetitions of elements
- Normalised.  The first a somewhere to the left of the first b,
  the first b somewhere to the left of the first c, and so on.

Examples:
- caaa is not NNR because b is omitted
- cabb is not NNR because b is repeated
- cabcabc is not NNR because the first a is to the right of the first c.
- abcabca is NNR
- abcabca embeds all permutations of a, b and c

|       | `abcabca`  |
| ----- | -----------|
| `abc` | `abc****`  |
| `acb` | `a*c*b**`  |
| `bac` | `*b*a*c*`  |
| `bca` | `*bca***`  |
| `cab` | `**cab**`  |
| `cba` | `**c*b*a`  |

## Thesis

PhD Thesis, Chapter 5

N. Stewart, An Image-Space Algorithm for Hardware-Based Rendering of
Constructive Solid Geometry, May 2008

# CLI Tools

| Command | Parameters | Description                                    |
| ------- | ---------- | ---------------------------------------------- |
| nnr     | n l        | Output the NNR sequences of *n* for length *l* |
| pes     | n          | Filter the input sequences for PES of *n*      |

# Scripts

| Command            | Description              | Output                    |
| ------------------ | ------------------------ | ------------------------- |
| [nnr2.sh](nnr2.sh) | Filter NNR *n=2* for PES | *a*                       |
| [nnr3.sh](nnr3.sh) | Filter NNR *n=3* for PES | *aba*                     |
| [nnr4.sh](nnr4.sh) | Filter NNR *n=4* for PES | *abcabac* ...             |
| [nnr5.sh](nnr5.sh) | Filter NNR *n=5* for PES | *abcdeabcdaebcadbcea* ... |

## Benchmarks

### nnr5.sh

> It takes a 2Ghz Pentium 4 around 12 hours to test all NNR sequences for n = 5, l = 19.
> We estimate that searching all NNR sequences for n = 6, l = 27 would take around 100 years on the same machine.

| Target                              | Time                     | Relative Time             |
| ----------------------------------- | ------------------------ | ------------------------- |
| Pentium 4 2Ghz                      | 12h                      | 1.0                       |
| Intel® Core™ i5-2500K CPU @ 3.30GHz |                          |                           |
| Raspberry Pi 3 Model B+             |                          |                           |
| AWS a1.medium                       |                          |                           |
| AWS c4.large                        |                          |                           |
| AWS c5.large                        |                          |                           |

### nnr5_4.sh

    $ /usr/bin/time -v ./nnr5.sh

| Target                              | Time                     | Relative Time             |
| ----------------------------------- | ------------------------ | ------------------------- |
| Intel® Core™ i5-2500K CPU @ 3.30GHz |  35.19s                  |  1.0                      |
| Raspberry Pi 3 Model B+             | 396.50s                  | 11.2                      |
| AWS a1.medium                       |                          |                           |
| AWS c4.large                        |                          |                           |
| AWS c5.large                        |                          |                           |

### nnr 4 4 12

- aarch64 a1.medium *1:36.22* m:ss (1 core @ 0.016100 hourly) cost: _0.0004303_
- x86_64 c5.large *0:52.98* m:ss (2 core @ 0.054000 hourly) cost: _0.0006254_
- x86_64 c4.large *1:02.46* m:ss (2 core @ 0.100000 hourly) cost: _0.0008675_

