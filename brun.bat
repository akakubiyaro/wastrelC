@echo off
cls
call build.bat
if %ERRORLEVEL% NEQ 0 ( echo An error occured when trying to build. & exit /b 1 )
echo.
echo -------
echo.
call run.bat
