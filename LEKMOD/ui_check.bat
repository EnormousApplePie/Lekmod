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
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\AncientRuins\GoodyHutPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GoodyHutPopup.lua"
REM --------------------------------------------------------------------------------------------------
set text="CityBannerProductionBox = function( city )"
IF EXIST "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" (
  FIND %text% "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" && (
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
  ) || (
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
    ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
  )
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\CityStatePopup\CityStateDiploPopup.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\CityView\CityView.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\CityView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityView.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\CityView\CityView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityView.lua"
)
IF NOT EXIST "%cd%\%euifolder%\CityView\ProductionPopup.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ProductionPopup.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Core\CityStateStatusHelper.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Core\CityStateStatusHelper.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateStatusHelper.lua"
)
IF EXIST "%cd%\%euifolder%\Core\EUI_tooltip_library.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_tooltip_library.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EUI_tooltip_library.lua"
)
IF EXIST "%cd%\%euifolder%\Core\EUI_unit_include.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_unit_include.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EUI_unit_include.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CultureOverview.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CultureOverview.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\EconomicGeneralInfo.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EconomicGeneralInfo.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\FrontEnd\EULA.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EULA.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\FrontEnd\FrontEnd.lua.ignore" "%cd%\%patchfolder%\Lua\UI\FrontEnd.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList\GPList.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GPList.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList\GPList.xml.ignore" "%cd%\%patchfolder%\Lua\UI\GPList.xml"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Improvements\WorldView.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Improvements\WorldView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\Improvements\WorldView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\WorldView.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\LeaderHead\TradeLogic.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\LeaderHead\TradeLogic.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\LeaderHead\TradeLogic.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\AdvancedSetup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\AdvancedSetup.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.lua.ignore" "%cd%\%patchfolder%\Lua\UI\JoiningRoom.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.xml.ignore" "%cd%\%patchfolder%\Lua\UI\JoiningRoom.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\LoadMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\LoadMenu.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\Lobby.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Lobby.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\Lobby.xml.ignore" "%cd%\%patchfolder%\Lua\UI\Lobby.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameOptions.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameOptions.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameSetupScreen.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.xml.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameSetupScreen.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.lua.ignore" "%cd%\%patchfolder%\Lua\UI\StagingRoom.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.xml.ignore" "%cd%\%patchfolder%\Lua\UI\StagingRoom.xml"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MiniMapPanel.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\MiniMapPanel.xml"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\DiploCorner.xml" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\NotificationPanel.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml"
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
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\Demographics.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Demographics.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\GameMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GameMenu.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ReplayViewer.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.xml.ignore" "%cd%\%patchfolder%\Lua\UI\ReplayViewer.xml"
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\TechTree\TechTree.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree\TechPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree\TechTree.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechTree.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.xml"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechTree.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechTree.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\ToolTips\TechButtonInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\TechButtonInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\TechButtonInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua"
)
IF NOT EXIST "%cd%\%euifolder%\ToolTips\InfoTooltipInclude.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\InfoTooltipInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\InfoTooltipInclude.lua"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\EUI_tooltip_library.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EUI_tooltip_library.lua"
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\UnitFlagManager\UnitFlagManager.lua" (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.xml"
) ELSE (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.lua"
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.xml"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitList\UnitList.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitList.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitList\UnitList.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitList.xml"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%cd%\%euifolder%\UnitPanel\UnitPanel.lua" && (
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
) || (
  ECHO UnitPanel.lua does not exists on EUI, copying to TM
  ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua"
)
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CCVotePopup.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CCVotePopup.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\NetworkKickedPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NetworkKickedPopup.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ProposalChartPopup.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\ProposalChartPopup.xml"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.lua.ignore" "%cd%\%patchfolder%\Lua\UI\VictoryProgress.lua"
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.xml.ignore" "%cd%\%patchfolder%\Lua\UI\VictoryProgress.xml"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Bombardment.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Bombardment.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\DiscussionDialog.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiscussionDialog.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\EndGameMenu.lua" "%cd%\%patchfolder%\Lua\UI\EndGameMenu.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\Highlights.xml" "%cd%\%patchfolder%\Lua\UI\Highlights.xml"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\InGame.lua.ignore" "%cd%\%patchfolder%\Lua\UI\InGame.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\mountain.lua.ignore" "%cd%\%patchfolder%\Lua\UI\mountain.lua"
REM --------------------------------------------------------------------------------------------------
ECHO F | xcopy /s /y "%cd%\%patchfolder%\Lua\tmp\ui\prophetreplace.lua.ignore" "%cd%\%patchfolder%\Lua\UI\prophetreplace.lua"
EXIT