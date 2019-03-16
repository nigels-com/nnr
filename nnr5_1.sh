#!/usr/bin/env bash

# abcde

printf "n=5 k=1\n"
./nnr -n 5 -l 5 | ./pes -n 5 -k 1
