@echo off
set QTDIR=C:\Qt6.8.0\Qt\6.8.0\msvc2022_64
set EXE=C:\Projects\project\out\build\debug\UserManagerTest.exe

"%QTDIR%\bin\windeployqt.exe" %EXE%
pause
