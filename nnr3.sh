#!/usr/bin/env bash

# Expected output:
#
# abcabca
# abcbabc
# abcabac
# abcacba
# abcbacb
# abacbab
# abacaba

./nnr -n 3 -l 7 | ./pes 3
