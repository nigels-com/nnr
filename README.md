# nnr

Normalised No-Repeat (NNR) sequences

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

## AWS Benchmarks

### nnr 4 4 12

- aarch64 a1.medium *1:36.22* m:ss (1 core @ 0.016100 hourly) cost: _0.0004303_
- x86_64 c5.large *0:52.98* m:ss (2 core @ 0.054000 hourly) cost: _0.0006254_
- x86_64 c4.large *1:02.46* m:ss (2 core @ 0.100000 hourly) cost: _0.0008675_

