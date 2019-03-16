#!/usr/bin/env bash

# ----------------------------------

# ab

printf "n=2 k=1\n"
./nnr -n 2 -l 2 | ./pes -n 2 -k 1

# ----------------------------------

# aba

printf "n=2\n"
./nnr -n 2 -l 3 | ./pes -n 2
