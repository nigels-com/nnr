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

./nnr 4 12 | ./pes 4
