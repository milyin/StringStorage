rm result.txt
for /D %%i in (8, 80, 800, 8000, 80000, 800000, 8000000) do call :test randomkeys.txt %%i
goto :eof

:test
cmake -S../.. -Bbuild -DBTREE_NODE_SIZE=%2 -DBTREE_KEY_SIZE=128
cmake --build build --target examples --config Release
echo %2 >> result.txt
echo %time% >> result.txt
build\examples\Release\pack.exe db %1 %1
echo %time% >> result.txt
build\examples\Release\unpack.exe db %1 nul
echo %time% >> result.txt
exit /b