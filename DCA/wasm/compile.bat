@echo off

CALL emsdk_env.bat 

SET SOURCE= src\CAEmulator.cpp
SET SOURCE=%SOURCE% src\CAWrap.cpp
SET SOURCE=%SOURCE% ..\Attack\CA\CubeAttack.cpp
SET SOURCE=%SOURCE% ..\Attack\CA\CubeFormer.cpp
SET SOURCE=%SOURCE% ..\Ciphers\Cipher_32_64.cpp
SET SOURCE=%SOURCE% ..\Ciphers\Simeck.cpp
SET SOURCE=%SOURCE% ..\Ciphers\Simon.cpp
SET SOURCE=%SOURCE% ..\Ciphers\Speck.cpp

@echo on

emcc -s WASM=1 %SOURCE% -O3 -o userMode.html --shell-file shell_minimal.html -s NO_EXIT_RUNTIME=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" --std=c++11

