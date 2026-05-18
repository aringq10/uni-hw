#!/bin/bash
# Linux mirror of Go.cmd — buildina ir paleidzia testus.

set -e
make

run() { ./main.exe "$@"; }

echo "--- Test 0: numatyta konfiguracija (main.cfg) ---"
run -rnd 42 > result0.txt

cat > test1.cfg <<'EOF'
# didelis srautas, maza personalo apkrova
n 1
k 3
GrT 2
m 1
InT 1
p1 0.80
d 24
p2 0.05
h 20
SessIn 4
SessGr 7
InK 80
GrK 30
SimT 1000
EOF
echo "--- Test 1: didelis srautas ---"
run test1.cfg -rnd 100 > result1.txt

cat > test2.cfg <<'EOF'
# pigi grupine vs brangi individuali
n 3
k 5
GrT 2
m 2
InT 1
p1 0.40
d 36
p2 0.15
h 40
SessIn 6
SessGr 9
InK 100
GrK 20
SimT 720
EOF
echo "--- Test 2: pigi grupine ---"
run test2.cfg -rnd 7 > result2.txt

cat > test3.cfg <<'EOF'
# milziniska kaina + ilga simuliacija -> BigInt naudingas
n 4
k 6
GrT 3
m 4
InT 1
p1 0.60
d 48
p2 0.10
h 30
SessIn 5
SessGr 8
InK 9999999999
GrK 8888888888
SimT 10000
EOF
echo "--- Test 3: BigInt ribos ---"
run test3.cfg -rnd 2026 > result3.txt

echo "--- Test 4: reprodukcija (-rnd 42 du kartus) ---"
run -rnd 42 > result4a.txt
run -rnd 42 > result4b.txt
if diff -q result4a.txt result4b.txt > /dev/null; then
    echo "OK: reproducible"
else
    echo "FAIL: outputs differ" >&2
    exit 1
fi

echo "Visi testai baigti."
