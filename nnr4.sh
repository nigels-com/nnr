#!/usr/bin/env bash

# ----------------------------------

# abcd

printf "n=4 k=1\n"
./nnr -n 4 -l 4 | ./pes -n 4 -k 1

# ----------------------------------

# abcdabc
# abcdcab
# abcdbca
# abcdbac
# abcdacb
# abcdcba

printf "n=4 k=2\n"
./nnr -n 4 -l 7 | ./pes -n 4 -k 2

# ----------------------------------

# abcdabcdab
# abcdcabcda
# abcdbcabcd
# abcdbacdab
# abcdacbcda
# abcdcbabcd
# abcdabcbda
# abcdcabacd
# abcdabdcab
# abcdbcadbc
# abcdbacbda
# abcdacbacd
# abcdbadcab
# abcdcbadbc
# abcdabcabd
# abcdcabdac
# abcdbcacdb
# abcdbacabd
# abcdacbdac
# abcdcbacdb
# abcdabcadb
# abcdcabdca
# abcdbcacbd
# abcdbacadb
# abcdacbdca
# abcdcbacbd
# abcdabcdba
# abcdcabcad
# abcdbcabdc
# abcdbacdba
# abcdacbcad
# abcdcbabdc
# abcdabcbad
# abcdcabadc
# abcdabdcba
# abcdbcadcb
# abcdbacbad
# abcdacbadc
# abcdbadcba
# abcdcbadcb

printf "n=4 k=3\n"
./nnr -n 4 -l 10 | ./pes -n 4 -k 3

# ----------------------------------

# abcdbacbdabc
# abcdbacbdcab
# abcdacbadcab
# abcdabcadbca
# abcdacbadbca
# abcdabcadbac
# abcdbacbdacb
# abcdabcadcba
# abcdacbadcba

printf "n=4\n"
./nnr -n 4 -l 12 | ./pes -n 4

