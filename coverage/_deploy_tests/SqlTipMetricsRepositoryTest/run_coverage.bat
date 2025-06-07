@echo off
set COV="C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe"
set SRC="C:\Projects\project"
set BIN="C:\Projects\project\out\build\debug\SqlTipMetricsRepositoryTest.exe"
set OUT="C:\Projects\project\coverage"

%COV% --sources %SRC% --export_type html:%OUT% -- %BIN%
start "" %OUT%\index.html
