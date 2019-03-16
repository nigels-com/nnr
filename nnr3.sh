#!/usr/bin/env bash

# ----------------------------------

# abc

printf "n=3 k=1\n"
./nnr -n 3 -l 3 | ./pes -n 3 -k 1

# ----------------------------------

# abcab
# abcba

printf "n=3 k=2\n"
./nnr -n 3 -l 5 | ./pes -n 3 -k 2

# ----------------------------------

# abcabca
# abcbabc
# abcabac
# abcacba
# abcbacb
# abacbab
# abacaba

printf "n=3\n"
./nnr -n 3 -l 7 | ./pes -n 3

