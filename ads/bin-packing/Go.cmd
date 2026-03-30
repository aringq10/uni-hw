if .%USERDOMAIN%==.SavasPC goto :savasPC

path C:\PROGRA~2\Dev-Cpp\MinGW64\bin\;%path%
mingw32-make.exe
goto :toliau

:savasPC
make

:toliau
main.exe > result0.txt

REM --- Test 1 ---
echo 10 > test1.txt
echo 6 >> test1.txt
echo 2 2 3 4 4 5 >> test1.txt
main.exe < test1.txt > result1.txt

REM --- Test 2 ---
echo 5 > test2.txt
echo 5 >> test2.txt
echo 2 2 3 3 4 >> test2.txt
main.exe < test2.txt > result2.txt

REM --- Test 3 ---
echo 7 > test3.txt
echo 7 >> test3.txt
echo 2 2 2 5 5 5 6 >> test3.txt
main.exe < test3.txt > result3.txt

REM --- Test 4 ---
echo 13 > test4.txt
echo 4 >> test4.txt
echo 2 4 5 9 >> test4.txt
main.exe < test4.txt > result4.txt

REM --- Test 5 ---
echo 9 > test5.txt
echo 5 >> test5.txt
echo 1 2 3 4 5 >> test5.txt
main.exe < test5.txt > result5.txt

REM --- Test 6 ---
echo 13 > test6.txt
echo 8 >> test6.txt
echo 1 2 3 4 5 6 7 9 >> test6.txt
main.exe < test6.txt > result6.txt

echo All tests finished.
pause
