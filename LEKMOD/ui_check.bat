@ECHO off
pushd "%~dp0"
cd ..
set patchfolder=LEKMOD_v33.5
set euifolder=UI_bc1_xits
IF EXIST "%cd%\UI_bc1" (
  set euifolder=UI_bc1
)
DEL /q "%cd%\%patchfolder%\Lua\UI\"
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\AncientRuins\GoodyHutPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GoodyHutPopup.lua" > nul
REM --------------------------------------------------------------------------------------------------
set text="CityBannerProductionBox = function( city )"
IF EXIST "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" (
  FIND %text% "%cd%\%euifolder%\CityBanners\CityBannerManager.lua" > nul 2>&1 && (
    copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
    copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_1.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
  ) || (
    copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
    copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityBanners\CityBannerManager_2.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
  )
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CityBanners\CityBannerManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CityBannerManager.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\CityStatePopup\CityStateDiploPopup.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityStatePopup\CityStateDiploPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateDiploPopup.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\CityView\CityView.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\CityView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityView.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\CityView\CityView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityView.lua" > nul
)
IF NOT EXIST "%cd%\%euifolder%\CityView\ProductionPopup.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CityView\ProductionPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ProductionPopup.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Core\CityStateStatusHelper.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Core\CityStateStatusHelper.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CityStateStatusHelper.lua" > nul
)
IF EXIST "%cd%\%euifolder%\Core\EUI_tooltip_library.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_tooltip_library.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EUI_tooltip_library.lua" > nul
)
IF EXIST "%cd%\%euifolder%\Core\EUI_unit_include.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\Core\EUI_unit_include.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EUI_unit_include.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CultureOverview.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\CultureOverview.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CultureOverview.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\CultureOverview\EconomicGeneralInfo.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EconomicGeneralInfo.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\FrontEnd\EULA.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EULA.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\FrontEnd\FrontEnd.lua.ignore" "%cd%\%patchfolder%\Lua\UI\FrontEnd.lua" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\GameSetup\SelectCivilization.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\GameSetup\SelectCivilization.lua.ignore" "%cd%\%patchfolder%\Lua\UI\SelectCivilization.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\GameSetup\SelectCivilization.lua.ignore" "%cd%\%patchfolder%\Lua\UI\SelectCivilization.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList\GPList.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GPList.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\GPList\GPList.xml.ignore" "%cd%\%patchfolder%\Lua\UI\GPList.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Improvements\WorldView.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Improvements\WorldView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\WorldView.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\Improvements\WorldView.lua.ignore" "%cd%\%patchfolder%\Lua\UI\WorldView.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\LeaderHead\TradeLogic.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\LeaderHead\TradeLogic.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\LeaderHead\TradeLogic.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TradeLogic.lua" > nul
)
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\LeaderHead\SimpleDiploTrade.lua.ignore" "%cd%\%patchfolder%\Lua\UI\SimpleDiploTrade.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\LeaderHead\SimpleDiploTrade.xml.ignore" "%cd%\%patchfolder%\Lua\UI\SimpleDiploTrade.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\AdvancedSetup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\AdvancedSetup.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.lua.ignore" "%cd%\%patchfolder%\Lua\UI\JoiningRoom.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\JoiningRoom.xml.ignore" "%cd%\%patchfolder%\Lua\UI\JoiningRoom.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\LoadMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\LoadMenu.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\Lobby.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Lobby.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\Lobby.xml.ignore" "%cd%\%patchfolder%\Lua\UI\Lobby.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameOptions.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameOptions.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameSetupScreen.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\MPGameSetupScreen.xml.ignore" "%cd%\%patchfolder%\Lua\UI\MPGameSetupScreen.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.lua.ignore" "%cd%\%patchfolder%\Lua\UI\StagingRoom.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Lobby\StagingRoom.xml.ignore" "%cd%\%patchfolder%\Lua\UI\StagingRoom.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\MiniMapPanel.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\MiniMapPanel\MiniMapPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\MiniMapPanel.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\DiploCorner.xml" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\DiploCorner.xml.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\DiploCorner.xml.ignore" "%cd%\%patchfolder%\Lua\UI\DiploCorner.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\NotificationPanel\NotificationPanel.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\NotificationPanel\NotificationPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\NotificationPanel\NotificationPanel.xml.ignore" "%cd%\%patchfolder%\Lua\UI\NotificationPanel.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\Options\OptionsMenu.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\Options\OptionsMenu.xml.ignore" "%cd%\%patchfolder%\Lua\UI\OptionsMenu.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\Demographics.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Demographics.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\GameMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\GameMenu.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ReplayViewer.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Replays\ReplayViewer.xml.ignore" "%cd%\%patchfolder%\Lua\UI\ReplayViewer.xml" > nul
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\TechTree\TechTree.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree\TechPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\TechTree\TechTree.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechTree.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechPopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\TechPopup.xml" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\TechTree\TechTree.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechTree.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\ToolTips\TechButtonInclude.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\TechButtonInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\TechButtonInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\TechButtonInclude.lua" > nul
)
IF NOT EXIST "%cd%\%euifolder%\ToolTips\InfoTooltipInclude.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\ToolTips\InfoTooltipInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\InfoTooltipInclude.lua" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\ToolTips\InfoTooltipInclude.lua.ignore" "%cd%\%patchfolder%\Lua\UI\InfoTooltipInclude.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
IF NOT EXIST "%cd%\%euifolder%\UnitFlagManager\UnitFlagManager.lua" (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitFlagManager\UnitFlagManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.xml" > nul
) ELSE (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.lua" > nul
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitFlagManager\UnitFlagManager.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitFlagManager.xml" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitList\UnitList.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitList.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitList\UnitList.xml.ignore" "%cd%\%patchfolder%\Lua\UI\UnitList.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua" > nul
set text="-- modified by bc1 from Civ V 1.0.3.276 code"
FIND %text% "%cd%\%euifolder%\UnitPanel\UnitPanel.lua" > nul 2>&1 && (
  copy /y "%cd%\%patchfolder%\Lua\tmp\eui\UnitPanel\UnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua" > nul
) || (
  copy /y "%cd%\%patchfolder%\Lua\tmp\ui\UnitPanel\UnitPanel.lua.ignore" "%cd%\%patchfolder%\Lua\UI\UnitPanel.lua" > nul
)
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\CCVotePopup.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\CCVotePopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\CCVotePopup.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\NetworkKickedPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\NetworkKickedPopup.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.lua.ignore" "%cd%\%patchfolder%\Lua\UI\ProposalChartPopup.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\ProposalChartPopup.xml.ignore" "%cd%\%patchfolder%\Lua\UI\ProposalChartPopup.xml" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.lua.ignore" "%cd%\%patchfolder%\Lua\UI\VictoryProgress.lua" > nul
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\VotingSystem\VictoryProgress.xml.ignore" "%cd%\%patchfolder%\Lua\UI\VictoryProgress.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Bombardment.lua.ignore" "%cd%\%patchfolder%\Lua\UI\Bombardment.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\DiscussionDialog.lua.ignore" "%cd%\%patchfolder%\Lua\UI\DiscussionDialog.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\EndGameMenu.lua.ignore" "%cd%\%patchfolder%\Lua\UI\EndGameMenu.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\Highlights.xml.ignore" "%cd%\%patchfolder%\Lua\UI\Highlights.xml" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\InGame.lua.ignore" "%cd%\%patchfolder%\Lua\UI\InGame.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\mountain.lua.ignore" "%cd%\%patchfolder%\Lua\UI\mountain.lua" > nul
REM --------------------------------------------------------------------------------------------------
copy /y "%cd%\%patchfolder%\Lua\tmp\ui\prophetreplace.lua.ignore" "%cd%\%patchfolder%\Lua\UI\prophetreplace.lua" > nul
EXIT