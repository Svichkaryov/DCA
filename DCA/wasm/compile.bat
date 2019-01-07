@echo off

CALL emsdk_env.bat 

@echo on

setlocal enabledelayedexpansion
SET OBJ=
FOR %%o in (*.o) do (set "OBJ=!OBJ! %%o")
echo %OBJ%
call emcc %OBJ% -o ca.bc

call del *.o

rem emcc -s WASM=1 %SOURCE% -O3 -o userMode.html --shell-file shell_minimal.html -s NO_EXIT_RUNTIME=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" --std=c++11
emcc -s WASM=1 ca.bc -O3 -o userMode.html --shell-file shell_minimal.html -s NO_EXIT_RUNTIME=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" --std=c++11

