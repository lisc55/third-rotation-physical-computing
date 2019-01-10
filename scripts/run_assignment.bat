@echo off

set assignment=%1
set assignment_name=%assignment:~3%

cmake --build ./build --config Release --target %assignment_name%
if not %ERRORLEVEL% == 0 goto :error

build\proj\%assignment%\Release\%assignment_name%.exe -o build\proj\%assignment_name%\output
if not %ERRORLEVEL% == 0 goto :error

viewer\bin\windows\opengl_viewer.exe -o build\proj\%assignment_name%\output
if not %ERRORLEVEL% == 0 goto :error

tools\ffmpeg.exe -start_number 0 -framerate 50 -i build\proj\%assignment_name%\output\_images\%%04d.png  -vcodec libx264 -crf 25 -pix_fmt yuv420p -y %assignment_name%.mp4
if not %ERRORLEVEL% == 0 goto :error

start %assignment_name%.mp4
if not %ERRORLEVEL% == 0 goto :error

goto :endofscript

:error
echo =========================
echo There was an error!
echo =========================

:endofscript
