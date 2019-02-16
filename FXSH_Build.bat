@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist TEST03.G1A  del TEST03.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\fxSDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\fxSDK\Tools\MakeAddinHeader363.exe" "C:\fxProjects\test03"
if not exist TEST03.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

