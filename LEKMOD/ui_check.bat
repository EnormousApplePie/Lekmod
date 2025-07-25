@ECHO off
pushd "%~dp0"
set patchfolder=%cd%
cd ..
set euifolder=UI_bc1_xits
IF EXIST "UI_bc1" (
  set euifolder=UI_bc1
)
DEL /q "%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\AncientRuins\GoodyHutPopup.lua.ignore" "%patchfolder%\Lua\UI\GoodyHutPopup.lua" > nul
REM --------------------------------------------------------------------------------------------------
set text="CityBannerProductionBox = function( city )"
IF EXIST "%euifolder%\CityBanners\CityBannerManager.lua" (
  FIND %text% "%euifolder%\CityBanners\CityBannerManager.lua" > nul 2>&1 && (
    copy /y "%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.lua.ignore" "%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
    copy /y "%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.xml.ignore" "%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
  ) || (
    copy /y "%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.lua.ignore" "%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
    copy /y "%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.xml.ignore" "%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
  )
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.lua.ignore" "%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.xml.ignore" "%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\CityStatePopup\CityStateDiploPopup.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%patchfolder%\Lua\UI\CityStateDiploPopup.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%patchfolder%\Lua\UI\CityStateDiploPopup.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
set text="-- coded by bc1 from 1.0.3.276 brave new world code"
FIND %text% "%euifolder%\CityView\CityView.lua" > nul 2>&1 && (
  copy /y "%patchfolder%\Lua\tmp\eui\CityView\CityView.lua.ignore" "%patchfolder%\Lua\UI\CityView.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\CityView\CityView.xml.ignore" "%patchfolder%\Lua\UI\CityView.xml" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\CityView\CityView_small.xml.ignore" "%patchfolder%\Lua\UI\CityView_small.xml" > nul
) || (
  copy /y "%patchfolder%\Lua\tmp\ui\CityView\CityView.lua.ignore" "%patchfolder%\Lua\UI\CityView.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\CityView\CityView.xml.ignore" "%patchfolder%\Lua\UI\CityView.xml" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\CityView\CityView_small.xml.ignore" "%patchfolder%\Lua\UI\CityView_small.xml" > nul
)
IF NOT EXIST "%euifolder%\CityView\ProductionPopup.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.lua.ignore" "%patchfolder%\Lua\UI\ProductionPopup.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.xml.ignore" "%patchfolder%\Lua\UI\ProductionPopup.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\Core\CityStateStatusHelper.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\Core\CityStateStatusHelper.lua.ignore" "%patchfolder%\Lua\UI\CityStateStatusHelper.lua" > nul
)
IF EXIST "%euifolder%\Core\EUI_tooltip_library.lua" (
  copy /y "%patchfolder%\Lua\tmp\eui\Core\EUI_tooltip_library.lua.ignore" "%patchfolder%\Lua\UI\EUI_tooltip_library.lua" > nul
)
IF EXIST "%euifolder%\Core\EUI_unit_include.lua" (
  copy /y "%patchfolder%\Lua\tmp\eui\Core\EUI_unit_include.lua.ignore" "%patchfolder%\Lua\UI\EUI_unit_include.lua" > nul
)
copy /y "%patchfolder%\Lua\tmp\ui\Core\IconSupport.lua.ignore" "%patchfolder%\Lua\UI\IconSupport.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.lua.ignore" "%patchfolder%\Lua\UI\CultureOverview.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.xml.ignore" "%patchfolder%\Lua\UI\CultureOverview.xml" > nul
IF NOT EXIST "%euifolder%\EconomicGeneralInfo.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\CultureOverview\EconomicGeneralInfo.lua.ignore" "%patchfolder%\Lua\UI\EconomicGeneralInfo.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\CultureOverview\EconomicGeneralInfo.xml.ignore" "%patchfolder%\Lua\UI\EconomicGeneralInfo.xml" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\EconomicGeneralInfo.lua.ignore" "%patchfolder%\Lua\UI\EconomicGeneralInfo.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\EconomicGeneralInfo.xml.ignore" "%patchfolder%\Lua\UI\EconomicGeneralInfo.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\FrontEnd\EULA.lua.ignore" "%patchfolder%\Lua\UI\EULA.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\FrontEnd\FrontEnd.lua.ignore" "%patchfolder%\Lua\UI\FrontEnd.lua" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\GameSetup\SelectCivilization.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\GameSetup\SelectCivilization.lua.ignore" "%patchfolder%\Lua\UI\SelectCivilization.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\GameSetup\SelectCivilization.lua.ignore" "%patchfolder%\Lua\UI\SelectCivilization.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\GPList\GPList.lua.ignore" "%patchfolder%\Lua\UI\GPList.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\GPList\GPList.xml.ignore" "%patchfolder%\Lua\UI\GPList.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\Improvements\WorldView.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\Improvements\WorldView.lua.ignore" "%patchfolder%\Lua\UI\WorldView.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\Improvements\WorldView.lua.ignore" "%patchfolder%\Lua\UI\WorldView.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\LeaderHead\TradeLogic.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\LeaderHead\TradeLogic.lua.ignore" "%patchfolder%\Lua\UI\TradeLogic.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\LeaderHead\TradeLogic.lua.ignore" "%patchfolder%\Lua\UI\TradeLogic.lua" > nul
)
copy /y "%patchfolder%\Lua\tmp\ui\LeaderHead\SimpleDiploTrade.lua.ignore" "%patchfolder%\Lua\UI\SimpleDiploTrade.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\LeaderHead\SimpleDiploTrade.xml.ignore" "%patchfolder%\Lua\UI\SimpleDiploTrade.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\AdvancedSetup.lua.ignore" "%patchfolder%\Lua\UI\AdvancedSetup.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.lua.ignore" "%patchfolder%\Lua\UI\JoiningRoom.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.xml.ignore" "%patchfolder%\Lua\UI\JoiningRoom.xml" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\LoadMenu.lua.ignore" "%patchfolder%\Lua\UI\LoadMenu.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\Lobby.lua.ignore" "%patchfolder%\Lua\UI\Lobby.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\Lobby.xml.ignore" "%patchfolder%\Lua\UI\Lobby.xml" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\MPGameOptions.lua.ignore" "%patchfolder%\Lua\UI\MPGameOptions.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.lua.ignore" "%patchfolder%\Lua\UI\MPGameSetupScreen.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.xml.ignore" "%patchfolder%\Lua\UI\MPGameSetupScreen.xml" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.lua.ignore" "%patchfolder%\Lua\UI\StagingRoom.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.xml.ignore" "%patchfolder%\Lua\UI\StagingRoom.xml" > nul
 copy /y "%patchfolder%\Lua\tmp\ui\PlotMouseoverInclude.lua.ignore" "%patchfolder%\Lua\UI\PlotMouseoverInclude.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.lua.ignore" "%patchfolder%\Lua\UI\MiniMapPanel.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.xml.ignore" "%patchfolder%\Lua\UI\MiniMapPanel.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\MPTurnPanel\MPTurnPanel.lua.ignore" "%patchfolder%\Lua\UI\MPTurnPanel.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\MPTurnPanel\MPTurnPanel.xml.ignore" "%patchfolder%\Lua\UI\MPTurnPanel.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\NotificationPanel\DiploCorner.xml" (
  copy /y "%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua.ignore" "%patchfolder%\Lua\UI\DiploCorner.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml.ignore" "%patchfolder%\Lua\UI\DiploCorner.xml" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua.ignore" "%patchfolder%\Lua\UI\DiploCorner.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml.ignore" "%patchfolder%\Lua\UI\DiploCorner.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\NotificationPanel\NotificationPanel.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua.ignore" "%patchfolder%\Lua\UI\NotificationPanel.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml.ignore" "%patchfolder%\Lua\UI\NotificationPanel.xml" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua.ignore" "%patchfolder%\Lua\UI\NotificationPanel.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml.ignore" "%patchfolder%\Lua\UI\NotificationPanel.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\Options\OptionsMenu.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.lua.ignore" "%patchfolder%\Lua\UI\OptionsMenu.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.xml.ignore" "%patchfolder%\Lua\UI\OptionsMenu.xml" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.lua.ignore" "%patchfolder%\Lua\UI\OptionsMenu.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.xml.ignore" "%patchfolder%\Lua\UI\OptionsMenu.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\Replays\Demographics.lua.ignore" "%patchfolder%\Lua\UI\Demographics.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Replays\GameMenu.lua.ignore" "%patchfolder%\Lua\UI\GameMenu.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.lua.ignore" "%patchfolder%\Lua\UI\ReplayViewer.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.xml.ignore" "%patchfolder%\Lua\UI\ReplayViewer.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\TechTree\TechTree.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\TechTree\TechPopup.lua.ignore" "%patchfolder%\Lua\UI\TechPopup.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\TechTree\TechTree.lua.ignore" "%patchfolder%\Lua\UI\TechTree.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.lua.ignore" "%patchfolder%\Lua\UI\TechPopup.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.xml.ignore" "%patchfolder%\Lua\UI\TechPopup.xml" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\TechTree\TechTree.lua.ignore" "%patchfolder%\Lua\UI\TechTree.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\ToolTips\TechButtonInclude.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\ToolTips\TechButtonInclude.lua.ignore" "%patchfolder%\Lua\UI\TechButtonInclude.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\ToolTips\TechButtonInclude.lua.ignore" "%patchfolder%\Lua\UI\TechButtonInclude.lua" > nul
)
IF NOT EXIST "%euifolder%\ToolTips\InfoTooltipInclude.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\ToolTips\InfoTooltipInclude.lua.ignore" "%patchfolder%\Lua\UI\InfoTooltipInclude.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\ToolTips\InfoTooltipInclude.lua.ignore" "%patchfolder%\Lua\UI\InfoTooltipInclude.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\UnitFlagManager\UnitFlagManager.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.lua.ignore" "%patchfolder%\Lua\UI\UnitFlagManager.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.xml.ignore" "%patchfolder%\Lua\UI\UnitFlagManager.xml" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.lua.ignore" "%patchfolder%\Lua\UI\UnitFlagManager.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.xml.ignore" "%patchfolder%\Lua\UI\UnitFlagManager.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\UnitList\UnitList.lua.ignore" "%patchfolder%\Lua\UI\UnitList.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\UnitList\UnitList.xml.ignore" "%patchfolder%\Lua\UI\UnitList.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua.ignore" "%patchfolder%\Lua\UI\EnemyUnitPanel.lua" > nul
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%euifolder%\UnitPanel\UnitPanel.lua" > nul 2>&1 && (
  copy /y "%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua.ignore" "%patchfolder%\Lua\UI\UnitPanel.lua" > nul
) || (
  copy /y "%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua.ignore" "%patchfolder%\Lua\UI\UnitPanel.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.lua.ignore" "%patchfolder%\Lua\UI\CCVotePopup.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.xml.ignore" "%patchfolder%\Lua\UI\CCVotePopup.xml" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\NetworkKickedPopup.lua.ignore" "%patchfolder%\Lua\UI\NetworkKickedPopup.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.lua.ignore" "%patchfolder%\Lua\UI\ProposalChartPopup.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.xml.ignore" "%patchfolder%\Lua\UI\ProposalChartPopup.xml" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.lua.ignore" "%patchfolder%\Lua\UI\VictoryProgress.lua" > nul
copy /y "%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.xml.ignore" "%patchfolder%\Lua\UI\VictoryProgress.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\Bombardment.lua.ignore" "%patchfolder%\Lua\UI\Bombardment.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\DiscussionDialog.lua.ignore" "%patchfolder%\Lua\UI\DiscussionDialog.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\EndGameMenu.lua.ignore" "%patchfolder%\Lua\UI\EndGameMenu.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\Highlights.xml.ignore" "%patchfolder%\Lua\UI\Highlights.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\InGame.lua.ignore" "%patchfolder%\Lua\UI\InGame.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\mountain.lua.ignore" "%patchfolder%\Lua\UI\mountain.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%patchfolder%\Lua\tmp\ui\prophetreplace.lua.ignore" "%patchfolder%\Lua\UI\prophetreplace.lua" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\Improvements\YieldIconManager.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\YieldIconManager.lua.ignore" "%patchfolder%\Lua\UI\YieldIconManager.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\Improvements\YieldIconManager.lua.ignore" "%patchfolder%\Lua\UI\YieldIconManager.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%euifolder%\TopPanel\TopPanel.lua" (
  copy /y "%patchfolder%\Lua\tmp\ui\TopPanel.lua.ignore" "%patchfolder%\Lua\UI\TopPanel.lua" > nul
) ELSE (
  copy /y "%patchfolder%\Lua\tmp\eui\TopPanel.lua.ignore" "%patchfolder%\Lua\UI\TopPanel.lua" > nul
  copy /y "%patchfolder%\Lua\tmp\eui\TopPanel.xml.ignore" "%patchfolder%\Lua\UI\TopPanel.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
EXIT