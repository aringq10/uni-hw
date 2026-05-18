@echo off
REM Apsilankymas pas psichologa - testu paleidimas.
REM Pirmas blokas: aplinkos atsaka (fakulteto vs asmeninis PC).

if .%USERDOMAIN%==.SavasPC goto :savasPC

REM Fakulteto kompiuteryje - Dev-Cpp MinGW kelias prie path
path C:\PROGRA~2\Dev-Cpp\MinGW64\bin\;%path%
mingw32-make.exe
goto :toliau

:savasPC
make

:toliau

REM --- Test 0: numatyta konfiguracija per argv[0] -> main.cfg ---
main.exe -rnd 42 > result0.txt 2>&1

REM --- Test 1: didelis srautas, maza personalo apkrova ---
echo # Test 1: didelis srautas             > test1.cfg
echo n      1                              >> test1.cfg
echo k      3                              >> test1.cfg
echo GrT    2                              >> test1.cfg
echo m      1                              >> test1.cfg
echo InT    1                              >> test1.cfg
echo p1     0.80                           >> test1.cfg
echo d      24                             >> test1.cfg
echo p2     0.05                           >> test1.cfg
echo h      20                             >> test1.cfg
echo SessIn 4                              >> test1.cfg
echo SessGr 7                              >> test1.cfg
echo InK    80                             >> test1.cfg
echo GrK    30                             >> test1.cfg
echo SimT   1000                           >> test1.cfg
main.exe test1.cfg -rnd 100 > result1.txt 2>&1

REM --- Test 2: pigus grupines + brangus individualus ---
echo # Test 2: pigi grupine                > test2.cfg
echo n      3                              >> test2.cfg
echo k      5                              >> test2.cfg
echo GrT    2                              >> test2.cfg
echo m      2                              >> test2.cfg
echo InT    1                              >> test2.cfg
echo p1     0.40                           >> test2.cfg
echo d      36                             >> test2.cfg
echo p2     0.15                           >> test2.cfg
echo h      40                             >> test2.cfg
echo SessIn 6                              >> test2.cfg
echo SessGr 9                              >> test2.cfg
echo InK    100                            >> test2.cfg
echo GrK    20                             >> test2.cfg
echo SimT   720                            >> test2.cfg
main.exe test2.cfg -rnd 7 > result2.txt 2>&1

REM --- Test 3: ilga simuliacija - BigInt ADT iesiraso i zaidima ---
echo # Test 3: 10 000 valandu = BigInt naudingas > test3.cfg
echo n      4                              >> test3.cfg
echo k      6                              >> test3.cfg
echo GrT    3                              >> test3.cfg
echo m      4                              >> test3.cfg
echo InT    1                              >> test3.cfg
echo p1     0.60                           >> test3.cfg
echo d      48                             >> test3.cfg
echo p2     0.10                           >> test3.cfg
echo h      30                             >> test3.cfg
echo SessIn 5                              >> test3.cfg
echo SessGr 8                              >> test3.cfg
echo InK    9999999999                     >> test3.cfg
echo GrK    8888888888                     >> test3.cfg
echo SimT   10000                          >> test3.cfg
main.exe test3.cfg -rnd 2026 > result3.txt 2>&1

REM --- Test 4: reproducibility - tas pats -rnd duoda ta pati rezultata ---
main.exe -rnd 42 > result4a.txt 2>&1
main.exe -rnd 42 > result4b.txt 2>&1
fc result4a.txt result4b.txt > result4.txt 2>&1

echo Visi testai baigti.
pause
