@ECHO off
pushd "%~dp0"
cd ..
set patchfolder=LEKMOD
ECHO Y | del "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\AncientRuins" "%cd%\%patchfolder%\Lua\UI\AncientRuins\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityStateDiploPopup" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Core" "%cd%\%patchfolder%\Lua\UI\Core\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview" "%cd%\%patchfolder%\Lua\UI\CultureOverview\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby" "%cd%\%patchfolder%\Lua\UI\Lobby\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel" "%cd%\%patchfolder%\Lua\UI\MiniMapPanel\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\DiploCorner.lua" IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\DiploCorner.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\DiploCorner.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\DiploCorner.xml"
)
IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\NotificationPanel.lua" IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\NotificationPanel.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\NotificationPanel.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel\NotificationPanel.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\OptionsMenu\OptionsMenu.lua" IF NOT EXIST "%cd%\UI_bc1\OptionsMenu\OptionsMenu.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\Lua\UI\OptionsMenu.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\Lua\tmp\eui\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\Lua\tmp\eui\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\Lua\UI\OptionsMenu.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays" "%cd%\%patchfolder%\Lua\UI\Replays\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\TechTree\TechPopup.lua" IF NOT EXIST "%cd%\UI_bc1\TechTree\TechPopup.xml" IF NOT EXIST "%cd%\UI_bc1\TechTree\TechTree.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree" "%cd%\%patchfolder%\Lua\UI\TechTree\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree" "%cd%\%patchfolder%\Lua\UI\TechTree\"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\ToolTips\TechButtonInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips" "%cd%\%patchfolder%\Lua\UI\ToolTips\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips" "%cd%\%patchfolder%\Lua\UI\ToolTips\"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel\EnemyUnitPanel.lua"
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%cd%\UI_bc1\UnitPanel\UnitPanel.lua" && (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel\UnitPanel.lua"
) || (
  ECHO UnitPanel.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel\UnitPanel.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem" "%cd%\%patchfolder%\Lua\UI\VotingSystem\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\EndGameMenu.lua" "%cd%\%patchfolder%\Lua\UI\EndGameMenu.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\InGame.lua" "%cd%\%patchfolder%\Lua\UI\InGame.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\mountain.lua" "%cd%\%patchfolder%\Lua\UI\mountain.lua"
REM --------------------------------------------------------------------------------------------------

ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ProductionPopup.lua" "%cd%\%patchfolder%\Lua\UI\ProductionPopup.lua"

REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\prophetreplace.lua" "%cd%\%patchfolder%\Lua\UI\prophetreplace.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\WorldView.lua" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
EXIT
