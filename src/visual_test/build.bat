@echo off

set Warnings=-Wall -wd4201 -wd4100 -wd4189 -wd4456 -wd4505 -wd4200 -wd4324 -wd4996 -wd4820 -wd5246 -wd5029 -wd4815 -wd5220 -wd4514 ^
             -wd4577 -wd5045 -wd4471 -wd5262 -wd4063 -wd4366
set CompOpts=%Warnings% -MDd -FC -Z7 -Oi -Od -GR- -EHa- -nologo -std:c++20 -Zc:strictStrings-
set LnkOpts=-opt:ref -subsystem:windows,5.02
set Libs=User32.lib Gdi32.lib Winmm.lib

IF NOT EXIST P:\bin\visual_test mkdir P:\bin\visual_test
pushd P:\bin\visual_test

del *pdb

cl %CompOpts% -I "C:\libs\jenh_libs\src" P:\src\visual_test\main.c -link -OUT:vis_lang.exe -PDB:vis_lang.pdb %LnkOpts% %Libs%

popd
