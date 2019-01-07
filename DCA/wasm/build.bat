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

FOR %%s in (%source%) do emcc -c %%s --std=c++11

