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

./nnr 3 7 | ./pes 3
