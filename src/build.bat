@echo off

IF NOT EXIST ..\bin mkdir ..\bin
pushd ..\bin

REM goto MSVC
goto CLANG

:MSVC

set Warnings=-Wall -wd4201 -wd4100 -wd4189 -wd4456 -wd4505 -wd4200 -wd4324 -wd4996 -wd4820 -wd5246 -wd5029 -wd4815 -wd5220 -wd4514 ^
             -wd4577 -wd5045 -wd4471 -wd5262 -wd4063 -wd4366 -wd4774
set CompOpts=%Warnings% -MDd -FC -Z7 -Oi -Od -GR- -EHa- -nologo
set LnkOpts=-opt:ref -subsystem:console,5.02

cl %CompOpts% -I "C:\libs\menc_lib\src" ..\src\main.c -link -OUT:lang.exe -PDB:lang.pdb %LnkOpts% %Libs%

goto end
:CLANG

clang -E -CC -I "C:\libs\menc_lib\src" ..\src\main.c > ..\src\pre_lang.c

call P:\bin\meta\meta.exe

set Warnings=-Wall -Wswitch-enum -Wno-ignored-pragma-intrinsic -Wno-ignored-attributes -Wno-microsoft-anon-tag

set CompOpts=%Warnings% -g -O0
set Libs=-l Shlwapi.lib -l Winmm.lib

clang %CompOpts% -I "C:\libs\menc_lib\src" -gcodeview-command-line -o lang.exe ..\src\main.c %Libs% -Wl,/STACK:8388608

REM del pre_lang.txt
REM del P:\src\meta.c

goto end

:end

popd
