@ECHO off
pushd "%~dp0"
cd ..
set patchfolder=LEKMOD
ECHO Y | DEL "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\AncientRuins" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
set text="CityBannerProductionBox = function( city )"
IF EXIST "%cd%\UI_bc1\CityBanners\CityBannerManager.lua" (
  FIND %text% "%cd%\UI_bc1\CityBanners\CityBannerManager.lua" && (
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.lua" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.xml" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
  ) || (
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.lua" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.xml" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
  )
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.lua" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.xml" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityStateDiploPopup" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\CityView\ProductionPopup.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.lua" "%cd%\%patchfolder%\Lua\UI\ProductionPopup.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Core" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\Improvements\WorldView.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Improvements\WorldView.lua" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\DiploCorner.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO NotificationPanel.lua does not exists on EUI, copying to TM
IF NOT EXIST "%cd%\UI_bc1\NotificationPanel\NotificationPanel.lua" (
  ECHO NotificationPanel1.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
) ELSE (
  ECHO NotificationPanel2.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\OptionsMenu\OptionsMenu.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\TechTree\TechTree.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree" "%cd%\%patchfolder%\Lua\UI\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree" "%cd%\%patchfolder%\Lua\UI\"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\UI_bc1\ToolTips\TechButtonInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips" "%cd%\%patchfolder%\Lua\UI\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips" "%cd%\%patchfolder%\Lua\UI\"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%cd%\UI_bc1\UnitPanel\UnitPanel.lua" && (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
) || (
  ECHO UnitPanel.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem" "%cd%\%patchfolder%\Lua\UI\"
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