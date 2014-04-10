if "%~1" == "x64_Release" goto x64_Release
if "%~1" == "x64_Debug" goto x64_Debug
if "%~1" == "x86_Release" goto x86_Release
if "%~1" == "x86_Debug" goto x86_Debug
goto EOF

:x86_Release
copy "..\dep\MariaDB\win32\lib\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win32\ssleay32.dll" "%~2" /Y
goto EOF

:x86_Debug
copy "..\dep\MariaDB\win32\lib\debug\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win32\ssleay32.dll" "%~2" /Y
goto EOF

:x64_Release
copy "..\dep\MariaDB\win64\lib\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win64\ssleay32.dll" "%~2" /Y
goto EOF

:x64_Debug
copy "..\dep\MariaDB\win64\lib\debug\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win64\ssleay32.dll" "%~2" /Y
goto EOF

:EOF