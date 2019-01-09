@echo off

set assignment=%1
set assignment_name=%assignment:~3%

cmake --build ./build --config Release --target %assignment_name%
if not %ERRORLEVEL% == 0 goto :error

build\proj\%assignment%\Release\%assignment_name%.exe -o build\proj\%assignment_name%\output
if not %ERRORLEVEL% == 0 goto :error

call run_viewer.bat build\proj\%assignment_name%\output
if not %ERRORLEVEL% == 0 goto :error

call render_video.bat build\proj\%assignment_name%\output\_images %assignment_name%
if not %ERRORLEVEL% == 0 goto :error

start %assignment_name%.mp4
if not %ERRORLEVEL% == 0 goto :error

goto :endofscript

:error
echo =========================
echo There was an error!
echo =========================

:endofscript
