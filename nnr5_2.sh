#!/usr/bin/env bash

# abcdeabcd
# abcdedabc
# abcdecdab
# abcdebacd
# abcdeadbc
# abcdedcab
# abcdebcda
# abcdecabd
# abcdebdac
# abcdecbda
# abcdeacbd
# abcdedbac
# abcdeacdb
# abcdedbca
# abcdebcad
# abcdecadb
# abcdebdca
# abcdecbad
# abcdeabdc
# abcdedacb
# abcdecdba
# abcdebadc
# abcdeadcb
# abcdedcba

printf "n=5 k=2\n"
./nnr -n 5 -l 9 | ./pes -n 5 -k 2
