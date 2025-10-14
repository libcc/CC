@echo off
set PLATFORM=windows
set MAKE_TOOL=make

if "%1"=="debug" (
    set DEBUG=debug=1
) else (
    set DEBUG=
)

set current_path=%~dp0
cd /d "%current_path%/.."

%MAKE_TOOL% .dll platform=%PLATFORM% %DEBUG%