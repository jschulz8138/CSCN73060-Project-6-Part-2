@echo off
SET /A "index = 1"
SET /A "count = 25"

:while
if %index% leq %count% (
     START /MIN "" Client.exe 127.0.0.1
     SET /A index = %index% + 1
     @echo %index%
     goto :while
)

