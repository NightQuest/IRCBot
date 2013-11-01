if "%~1" == "x64" goto x64
if "%~1" == "x86" goto x86
goto EOF

:x86
copy "..\dep\MariaDB\win32\lib\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win32\libeay32.dll" "%~2" /Y
copy "..\dep\OpenSSL\win32\ssleay32.dll" "%~2" /Y
goto EOF

:x64
copy "..\dep\MariaDB\win64\lib\libmariadb.dll" "%~2" /Y
copy "..\dep\OpenSSL\win64\libeay32.dll" "%~2" /Y
copy "..\dep\OpenSSL\win64\ssleay32.dll" "%~2" /Y
goto EOF

:EOF