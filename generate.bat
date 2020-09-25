@echo off

set REL_PATH=.\generator
set ABS_PATH=

rem // Save current directory and change to target directory
pushd %REL_PATH%

rem // Save value of CD variable (current directory)
set ABS_PATH=%CD%

rem // Restore original directory
popd

forfiles /p .\Frost\src\ /s /m *cx /c "cmd /c call %ABS_PATH%\generate.exe @PATH"