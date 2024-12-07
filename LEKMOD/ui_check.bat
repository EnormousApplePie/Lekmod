@ECHO off
pushd "%~dp0"
cd ..
set patchfolder=LEKMOD
set euifolder=UI_bc1_xits
IF EXIST "%cd%\UI_bc1" (
  ECHO 1
  set euifolder=UI_bc1
)
ECHO Y | DEL "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\AncientRuins" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
set text="CityBannerProductionBox = function( city )"
IF EXIST "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" (
  FIND %text% "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" && (
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
IF NOT EXIST "%cd%\%euifolder%\CityStatePopup\CityStateDiploPopup.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityStatePopup\CityStateDiploPopup.lua" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityStatePopup\CityStateDiploPopup.lua" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\CityView\ProductionPopup.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.lua" "%cd%\%patchfolder%\Lua\UI\ProductionPopup.lua"
)
IF NOT EXIST "%cd%\%euifolder%\CityView\CityView.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\CityView.lua" "%cd%\%patchfolder%\Lua\UI\CityView.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Core\CityStateStatusHelper.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Core\CityStateStatusHelper.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateStatusHelper.lua"
)
IF EXIST "%cd%\%euifolder%\Core\EUI_tooltip_library.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_tooltip_library.lua" "%cd%\%patchfolder%\Lua\UI\EUI_tooltip_library.lua"
)
IF EXIST "%cd%\%euifolder%\Core\EUI_unit_include.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_unit_include.lua" "%cd%\%patchfolder%\Lua\UI\EUI_unit_include.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\FrontEnd" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Improvements\WorldView.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Improvements\WorldView.lua" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Improvements\WorldView.lua" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\LeaderHead\TradeLogic.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\LeaderHead\TradeLogic.lua" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\LeaderHead\TradeLogic.lua" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\DiploCorner.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\NotificationPanel.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Options\OptionsMenu.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\TechTree\TechTree.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree" "%cd%\%patchfolder%\Lua\UI\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree" "%cd%\%patchfolder%\Lua\UI\"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\ToolTips\TechButtonInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\TechButtonInclude.lua" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\TechButtonInclude.lua" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua"
)
IF NOT EXIST "%cd%\%euifolder%\ToolTips\InfoTooltipInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\InfoTooltipInclude.lua" "%cd%\%patchfolder%\Lua\UI\InfoTooltipInclude.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\EUI_tooltip_library.lua" "%cd%\%patchfolder%\Lua\UI\EUI_tooltip_library.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\UnitFlagManager\UnitFlagManager.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitFlagManager" "%cd%\%patchfolder%\Lua\UI\"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitFlagManager" "%cd%\%patchfolder%\Lua\UI\"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%cd%\%euifolder%\UnitPanel\UnitPanel.lua" && (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
) || (
  ECHO UnitPanel.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem" "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Bombardment.lua" "%cd%\%patchfolder%\Lua\UI\Bombardment.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\DiscussionDialog.lua" "%cd%\%patchfolder%\Lua\UI\DiscussionDialog.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\EndGameMenu.lua" "%cd%\%patchfolder%\Lua\UI\EndGameMenu.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Highlights.xml" "%cd%\%patchfolder%\Lua\UI\Highlights.xml"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\InGame.lua" "%cd%\%patchfolder%\Lua\UI\InGame.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\mountain.lua" "%cd%\%patchfolder%\Lua\UI\mountain.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\prophetreplace.lua" "%cd%\%patchfolder%\Lua\UI\prophetreplace.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\WorldView.lua" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
EXIT