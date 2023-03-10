@ECHO off
pushd "%~dp0"
cd ..
set patchfolder=Lekmod_vX.X
ECHO Y | del "%cd%\%patchfolder%\UI\"
REM -------------------------------------------------
IF EXIST "%cd%\UI_bc1\NotificationPanel\NotificationPanel.lua" (
  ECHO NotificationPanel.lua: EUI version detected
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\tmp\eui\NotificationPanel.lua" "%cd%\%patchfolder%\UI\NotificationPanel.lua"
) ELSE (
  ECHO NotificationPanel.lua: no EUI, copying vanilla UI version
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\tmp\ui\NotificationPanel.lua" "%cd%\%patchfolder%\UI\NotificationPanel.lua"
)
REM -------------------------------------------------
IF EXIST "%cd%\UI_bc1\NotificationPanel\NotificationPanel.xml" (
  ECHO NotificationPanel.xml: EUI version detected
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\tmp\eui\NotificationPanel.xml" "%cd%\%patchfolder%\UI\NotificationPanel.xml"
) ELSE (
  ECHO NotificationPanel.xml: no EUI, copying vanilla UI version
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\tmp\ui\NotificationPanel.xml" "%cd%\%patchfolder%\UI\NotificationPanel.xml"
)
ECHO F | xcopy /s /y "%cd%%patchfolder%\tmp\ui\StagingRoom.lua" "%cd%%patchfolder%\UI\StagingRoom.lua"
EXIT