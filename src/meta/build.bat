@echo off

set Warnings=-Wall -wd4201 -wd4100 -wd4189 -wd4456 -wd4505 -wd4200 -wd4324 -wd4996 -wd4820 -wd5246 -wd5029 -wd4815 -wd5220 -wd4514 ^
             -wd4577 -wd5045 -wd4471 -wd5262 -wd4063 -wd4366 -wd4774
set CompOpts=%Warnings% -MDd -FC -Z7 -Oi -Od -GR- -EHa- -nologo
set LnkOpts=-opt:ref -subsystem:console,5.02
set Libs=Shlwapi.lib

IF NOT EXIST P:\bin\meta mkdir P:\bin\meta
pushd P:\bin\meta

cl %CompOpts% -I "C:\libs\menc_lib\src" P:\src\meta\main.c -link -OUT:meta.exe -PDB:meta.pdb %LnkOpts% %Libs%

popd
