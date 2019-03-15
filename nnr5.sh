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

./nnr 5 19 | ./pes 5
