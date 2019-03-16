#!/usr/bin/env bash

# Expected output:
#
# abcdbacbdabc
# abcdbacbdcab
# abcdacbadcab
# abcdabcadbca
# abcdacbadbca
# abcdabcadbac
# abcdbacbdacb
# abcdabcadcba
# abcdacbadcba

./nnr -n 4 -l 12 | ./pes 4
