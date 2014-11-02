@ECHO OFF
ECHO Welcome to custom extraction of dlls needed for System Monitoring x64!

if exist %CD%\System-Monitoring-x64 (
	ECHO Folder already exists!
	ECHO Cleaning up folder...
	RD /S /Q System-Monitoring-x64
	ECHO Cleaning up complete!
	MD System-Monitoring-x64
) else ( 
	MD System-Monitoring-x64
)

MD System-Monitoring-x64\config
MD System-Monitoring-x64\config\databases
MD System-Monitoring-x64\config\drivers
MD System-Monitoring-x64\config\drivers\vista
MD System-Monitoring-x64\config\drivers\win7
MD System-Monitoring-x64\config\drivers\win8
MD System-Monitoring-x64\platforms
MD System-Monitoring-x64\sqldrivers

COPY build\config\databases\config.db System-Monitoring-x64\config\databases\config.db
COPY build\config\drivers\vista\stk_driver.sys System-Monitoring-x64\config\drivers\vista\stk_driver.sys
COPY build\config\drivers\vista\stk_driver64.sys System-Monitoring-x64\config\drivers\vista\stk_driver64.sys
COPY build\config\drivers\win7\stk_driver.sys System-Monitoring-x64\config\drivers\win7\stk_driver.sys
COPY build\config\drivers\win7\stk_driver64.sys System-Monitoring-x64\config\drivers\win7\stk_driver64.sys
COPY build\config\drivers\win8\stk_driver.sys System-Monitoring-x64\config\drivers\win8\stk_driver.sys
COPY build\config\drivers\win8\stk_driver64.sys System-Monitoring-x64\config\drivers\win8\stk_driver64.sys
COPY build\config\drivers\stk_cert.cer System-Monitoring-x64\config\drivers\stk_cert.cer

COPY build\configuration.cfg System-Monitoring-x64\configuration.cfg
COPY build\Licenta.exe System-Monitoring-x64\Licenta.exe

SET QT_PATH=C:\Qt\Qt5.3.1-x64\5.3\msvc2013_64\
COPY %QT_PATH%\bin\Qt5Core.dll System-Monitoring-x64\Qt5Core.dll
COPY %QT_PATH%\bin\Qt5Gui.dll System-Monitoring-x64\Qt5Gui.dll
COPY %QT_PATH%\bin\Qt5Widgets.dll System-Monitoring-x64\Qt5Widgets.dll
COPY %QT_PATH%\bin\Qt5Sql.dll System-Monitoring-x64\Qt5Sql.dll
COPY %QT_PATH%\bin\Qt5WinExtras.dll System-Monitoring-x64\Qt5WinExtras.dll
COPY %QT_PATH%\bin\icudt52.dll System-Monitoring-x64\icudt52.dll
COPY %QT_PATH%\bin\icuin52.dll System-Monitoring-x64\icuin52.dll
COPY %QT_PATH%\bin\icuuc52.dll System-Monitoring-x64\icuuc52.dll
COPY %QT_PATH%\bin\libEGL.dll System-Monitoring-x64\libEGL.dll
COPY %QT_PATH%\bin\libGLESv2.dll System-Monitoring-x64\libGLESv2.dll

COPY %QT_PATH%\plugins\sqldrivers\qsqlite.dll System-Monitoring-x64\sqldrivers\qsqlite.dll
COPY %QT_PATH%\plugins\sqldrivers\qsqlmysql.dll System-Monitoring-x64\sqldrivers\qsqlmysql.dll
COPY %QT_PATH%\plugins\sqldrivers\qsqlodbc.dll System-Monitoring-x64\sqldrivers\qsqlodbc.dll
COPY %QT_PATH%\plugins\sqldrivers\qsqlpsql.dll System-Monitoring-x64\sqldrivers\qsqlpsql.dll

COPY %QT_PATH%\plugins\platforms\qminimal.dll System-Monitoring-x64\platforms\qminimal.dll
COPY %QT_PATH%\plugins\platforms\qoffscreen.dll System-Monitoring-x64\platforms\qoffscreen.dll
COPY %QT_PATH%\plugins\platforms\qwindows.dll System-Monitoring-x64\platforms\qwindows.dll

COPY C:\Windows\System32\advapi32.dll System-Monitoring-x64\advapi32.dll
COPY C:\Windows\System32\setupapi.dll System-Monitoring-x64\setupapi.dll
COPY C:\Windows\System32\ole32.dll System-Monitoring-x64\ole32.dll
COPY C:\Windows\System32\oleaut32.dll System-Monitoring-x64\oleaut32.dll
COPY C:\Windows\System32\version.dll System-Monitoring-x64\version.dll
COPY C:\Windows\System32\IPHLPAPI.DLL System-Monitoring-x64\iphlpapi32.dll
COPY C:\Windows\System32\ws2_32.dll System-Monitoring-x64\ws2_32.dll
COPY C:\Windows\System32\pdh.dll System-Monitoring-x64\pdh.dll
COPY C:\Windows\System32\netapi32.dll System-Monitoring-x64\netapi32.dll
COPY C:\Windows\System32\shell32.dll System-Monitoring-x64\shell32.dll
COPY C:\Windows\System32\user32.dll System-Monitoring-x64\user32.dll
COPY C:\Windows\System32\psapi.dll System-Monitoring-x64\psapi.dll
COPY C:\Windows\System32\msvcp120.dll System-Monitoring-x64\msvcp120.dll
COPY C:\Windows\System32\msvcr120.dll System-Monitoring-x64\msvcr120.dll

pause


