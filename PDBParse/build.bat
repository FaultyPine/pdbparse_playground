@echo off
setlocal

for %%a in (%*) do set "%%a=1"
set cwd=%~dp0

pushd src

::set LINKER_ARGS=-Xlinker /SUBSYSTEM:CONSOLE -luser32 -ldiaguids -L"C:\Program Files\Microsoft Visual Studio\2022\Community\DIA SDK\lib\amd64"
set LINKER_ARGS=-Xlinker /SUBSYSTEM:CONSOLE -luser32 -lRawPDB -L"%cwd%external/raw_pdb/lib/x64/Release"
REM set sources=pdbparse_main.cpp DIA2Dump/DIA2Dump.cpp
set sources=pdbparse_main.cpp mapped_file.cpp typetable.cpp
REM set includes=-I"C:\Program Files\Microsoft Visual Studio\2022\Community\DIA SDK\include"
set includes=-I"%cwd%external/raw_pdb/src"
clang %sources% -o ../main.exe -g -D_CONSOLE -mconsole %LINKER_ARGS% -Wno-trigraphs  %includes%

popd

if "%run%"=="1" (
    main.exe
)

endlocal