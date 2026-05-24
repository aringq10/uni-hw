@echo off
REM Paskolu tinklas - 3 lab darbo testu paleidimas.
REM Pirmas blokas: aplinkos atsaka (fakulteto vs asmeninis PC).

if .%USERDOMAIN%==.SavasPC goto :savasPC

REM Fakulteto kompiuteryje - Dev-Cpp MinGW kelias prie path
path C:\PROGRA~2\Dev-Cpp\MinGW64\bin\;%path%
mingw32-make.exe
goto :toliau

:savasPC
mingw32-make

:toliau

REM --- Test 1: tiesiogine paslauga (Jonas -> Maryte) ---
echo Jonas Maryte > data1.txt
programa.exe data1.txt Jonas Maryte > result1.txt 2>&1

REM --- Test 2: grandine per Maryte (Jonas -> Maryte -> Petras) ---
echo Jonas Maryte > data2.txt
echo Maryte Petras >> data2.txt
programa.exe data2.txt Jonas Petras > result2.txt 2>&1

REM --- Test 3: nera kelio (Jonas -> Maryte, Petras -> Ona) ---
echo Jonas Maryte > data3.txt
echo Petras Ona >> data3.txt
programa.exe data3.txt Jonas Ona > result3.txt 2>&1

REM --- Test 4: abipuses paslaugos su ciklu ---
REM Jonas <-> Maryte, ir Maryte -> Petras.
REM Klausimas: ar Petras gali tiketis, kad Jonas paskolins?
REM Petras niekam nepadare paslaugos, todel - NE.
REM Patikrina, kad DFS ciklo nesuga'os.
echo Jonas Maryte > data4.txt
echo Maryte Jonas >> data4.txt
echo Maryte Petras >> data4.txt
programa.exe data4.txt Petras Jonas > result4.txt 2>&1

echo Visi 4 testai baigti.
echo Rezultatai irasyti i result1.txt, result2.txt, result3.txt, result4.txt.
