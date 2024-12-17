Set-Location -Path ..
Set-Variable -Name 'patchfolder' -Value 'LEKMOD'
Set-Variable -Name 'euifolder' -Value 'UI_bc1_xits'
if (!(Test-Path -Path $euifolder)) {
    Set-Variable -Name 'euifolder' -Value 'UI_bc1'
}
Remove-Item -Path $patchfolder\Lua\UI\*

################################################################################################
Copy-Item -Path ("$patchfolder\lua\tmp\ui\AncientRuins\GoodyHutPopup.lua.ignore") -Destination "$patchfolder\Lua\UI\GoodyHutPopup.lua"
################################################################################################
if (Test-Path -Path $euifolder\CityBanners\CityBannerManager.lua)
{
    if (Select-String -Path $euifolder\CityBanners\CityBannerManager.lua -Pattern 'CityBannerProductionBox = function( city )')
    {
        Copy-Item -Path "$patchfolder\lua\tmp\eui\CityBanners\CityBannerManager_1.lua.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.lua"
        Copy-Item -Path "$patchfolder\lua\tmp\eui\CityBanners\CityBannerManager_1.xml.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.xml"
    }
    else
    {
        Copy-Item -Path "$patchfolder\lua\tmp\eui\CityBanners\CityBannerManager_2.lua.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.lua"
        Copy-Item -Path "$patchfolder\lua\tmp\eui\CityBanners\CityBannerManager_2.xml.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.xml"
    }
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\CityBanners\CityBannerManager.lua.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\CityBanners\CityBannerManager.xml.ignore" -Destination "$patchfolder\Lua\UI\CityBannerManager.xml"
}
################################################################################################
if (Test-Path -Path $euifolder\CityStatePopup\CityStateDiploPopup.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\CityStatePopup\CityStateDiploPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\CityStateDiploPopup.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\CityStatePopup\CityStateDiploPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\CityStateDiploPopup.lua"
}
################################################################################################
if (Test-Path -Path $euifolder\CityView\CityView.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\CityView\CityView.lua.ignore" -Destination "$patchfolder\Lua\UI\CityView.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\CityView\CityView.lua.ignore" -Destination "$patchfolder\Lua\UI\CityView.lua"
}
if (!(Test-Path -Path $euifolder\CityView\ProductionPopup.lua))
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\CityView\ProductionPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\ProductionPopup.lua"
}
################################################################################################
if (!(Test-Path -Path $euifolder\Core\CityStateStatusHelper.lua))
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\Core\CityStateStatusHelper.lua.ignore" -Destination "$patchfolder\Lua\UI\CityStateStatusHelper.lua"
}
if (Test-Path -Path $euifolder\Core\EUI_tooltip_library.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\Core\EUI_tooltip_library.lua.ignore" -Destination "$patchfolder\Lua\UI\EUI_tooltip_library.lua"
}
if (Test-Path -Path $euifolder\Core\EUI_unit_include.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\Core\EUI_unit_include.lua.ignore" -Destination "$patchfolder\Lua\UI\EUI_unit_include.lua"
}
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\CultureOverview\CultureOverview.lua.ignore" -Destination "$patchfolder\Lua\UI\CultureOverview.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\CultureOverview\CultureOverview.xml.ignore" -Destination "$patchfolder\Lua\UI\CultureOverview.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\CultureOverview\EconomicGeneralInfo.lua.ignore" -Destination "$patchfolder\Lua\UI\EconomicGeneralInfo.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\FrontEnd\EULA.lua.ignore" -Destination "$patchfolder\Lua\UI\EULA.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\FrontEnd\FrontEnd.lua.ignore" -Destination "$patchfolder\Lua\UI\FrontEnd.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\GPList\GPList.lua.ignore" -Destination "$patchfolder\Lua\UI\GPList.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\GPList\GPList.xml.ignore" -Destination "$patchfolder\Lua\UI\GPList.xml"
################################################################################################
if (Test-Path -Path $euifolder\Improvements\WorldView.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\Improvements\WorldView.lua.ignore" -Destination "$patchfolder\Lua\UI\WorldView.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\Improvements\WorldView.lua.ignore" -Destination "$patchfolder\Lua\UI\WorldView.lua"
}
################################################################################################
if (Test-Path -Path $euifolder\LeaderHead\TradeLogic.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\LeaderHead\TradeLogic.lua.ignore" -Destination "$patchfolder\Lua\UI\TradeLogic.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\LeaderHead\TradeLogic.lua.ignore" -Destination "$patchfolder\Lua\UI\TradeLogic.lua"
}
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\AdvancedSetup.lua.ignore" -Destination "$patchfolder\Lua\UI\AdvancedSetup.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\JoiningRoom.lua.ignore" -Destination "$patchfolder\Lua\UI\JoiningRoom.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\JoiningRoom.xml.ignore" -Destination "$patchfolder\Lua\UI\JoiningRoom.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\LoadMenu.lua.ignore" -Destination "$patchfolder\Lua\UI\LoadMenu.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\Lobby.lua.ignore" -Destination "$patchfolder\Lua\UI\Lobby.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\Lobby.xml.ignore" -Destination "$patchfolder\Lua\UI\Lobby.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\MPGameOptions.lua.ignore" -Destination "$patchfolder\Lua\UI\MPGameOptions.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\MPGameSetupScreen.lua.ignore" -Destination "$patchfolder\Lua\UI\MPGameSetupScreen.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\MPGameSetupScreen.xml.ignore" -Destination "$patchfolder\Lua\UI\MPGameSetupScreen.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\StagingRoom.lua.ignore" -Destination "$patchfolder\Lua\UI\StagingRoom.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Lobby\StagingRoom.xml.ignore" -Destination "$patchfolder\Lua\UI\StagingRoom.xml"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\MiniMapPanel\MiniMapPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\MiniMapPanel.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\MiniMapPanel\MiniMapPanel.xml.ignore" -Destination "$patchfolder\Lua\UI\MiniMapPanel.xml"
################################################################################################
if (Test-Path -Path $euifolder\NotificationPanel\DiploCorner.xml)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\NotificationPanel\DiploCorner.lua.ignore" -Destination "$patchfolder\Lua\UI\DiploCorner.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\eui\NotificationPanel\DiploCorner.xml.ignore" -Destination "$patchfolder\Lua\UI\DiploCorner.xml"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\NotificationPanel\DiploCorner.lua.ignore" -Destination "$patchfolder\Lua\UI\DiploCorner.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\NotificationPanel\DiploCorner.xml.ignore" -Destination "$patchfolder\Lua\UI\DiploCorner.xml"
}
if (Test-Path -Path $euifolder\NotificationPanel\NotificationPanel.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\NotificationPanel\NotificationPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\NotificationPanel.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\eui\NotificationPanel\NotificationPanel.xml.ignore" -Destination "$patchfolder\Lua\UI\NotificationPanel.xml"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\NotificationPanel\NotificationPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\NotificationPanel.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\NotificationPanel\NotificationPanel.xml.ignore" -Destination "$patchfolder\Lua\UI\NotificationPanel.xml"
}
################################################################################################
if (Test-Path -Path $euifolder\Options\OptionsMenu.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\Options\OptionsMenu.lua.ignore" -Destination "$patchfolder\Lua\UI\OptionsMenu.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\eui\Options\OptionsMenu.xml.ignore" -Destination "$patchfolder\Lua\UI\OptionsMenu.xml"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\Options\OptionsMenu.lua.ignore" -Destination "$patchfolder\Lua\UI\OptionsMenu.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\Options\OptionsMenu.xml.ignore" -Destination "$patchfolder\Lua\UI\OptionsMenu.xml"
}
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\Replays\Demographics.lua.ignore" -Destination "$patchfolder\Lua\UI\Demographics.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Replays\GameMenu.lua.ignore" -Destination "$patchfolder\Lua\UI\GameMenu.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Replays\ReplayViewer.lua.ignore" -Destination "$patchfolder\Lua\UI\ReplayViewer.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\Replays\ReplayViewer.xml.ignore" -Destination "$patchfolder\Lua\UI\ReplayViewer.xml"
################################################################################################
if (Test-Path -Path $euifolder\TechTree\TechTree.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\TechTree\TechPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\TechPopup.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\eui\TechTree\TechTree.lua.ignore" -Destination "$patchfolder\Lua\UI\TechTree.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\TechTree\TechPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\TechPopup.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\TechTree\TechTree.lua.ignore" -Destination "$patchfolder\Lua\UI\TechTree.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\TechTree\TechTree.xml.ignore" -Destination "$patchfolder\Lua\UI\TechTree.xml"
}
################################################################################################
if (Test-Path -Path $euifolder\ToolTips\TechButtonInclude.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\ToolTips\TechButtonInclude.lua.ignore" -Destination "$patchfolder\Lua\UI\TechButtonInclude.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\ToolTips\TechButtonInclude.lua.ignore" -Destination "$patchfolder\Lua\UI\TechButtonInclude.lua"
}
if (Test-Path -Path $euifolder\ToolTips\InfoTooltipInclude.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\ToolTips\InfoTooltipInclude.lua.ignore" -Destination "$patchfolder\Lua\UI\InfoTooltipInclude.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\ToolTips\InfoTooltipInclude.lua.ignore" -Destination "$patchfolder\Lua\UI\InfoTooltipInclude.lua"
}
################################################################################################
if (Test-Path -Path $euifolder\UnitFlagManager\UnitFlagManager.lua)
{
    Copy-Item -Path "$patchfolder\lua\tmp\eui\UnitFlagManager\UnitFlagManager.lua.ignore" -Destination "$patchfolder\Lua\UI\UnitFlagManager.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\eui\UnitFlagManager\UnitFlagManager.xml.ignore" -Destination "$patchfolder\Lua\UI\UnitFlagManager.xml"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitFlagManager\UnitFlagManager.lua.ignore" -Destination "$patchfolder\Lua\UI\UnitFlagManager.lua"
    Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitFlagManager\UnitFlagManager.xml.ignore" -Destination "$patchfolder\Lua\UI\UnitFlagManager.xml"
}
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitList\UnitList.lua.ignore" -Destination "$patchfolder\Lua\UI\UnitList.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitList\UnitList.xml.ignore" -Destination "$patchfolder\Lua\UI\UnitList.xml"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitPanel\EnemyUnitPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\EnemyUnitPanel.lua"
#if (Test-Path -Path $euifolder\UnitPanel\UnitPanel.lua)
if (Select-String -Path $euifolder\UnitPanel\UnitPanel.lua -Pattern '-- modified by bc1 from Civ V 1.0.3.276 code')
{
    'File'
    Copy-Item -Path "$patchfolder\lua\tmp\eui\UnitPanel\UnitPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\UnitPanel.lua"
}
else
{
    Copy-Item -Path "$patchfolder\lua\tmp\ui\UnitPanel\UnitPanel.lua.ignore" -Destination "$patchfolder\Lua\UI\UnitPanel.lua"
}
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\CCVotePopup.lua.ignore" -Destination "$patchfolder\Lua\UI\CCVotePopup.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\CCVotePopup.xml.ignore" -Destination "$patchfolder\Lua\UI\CCVotePopup.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\NetworkKickedPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\NetworkKickedPopup.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\ProposalChartPopup.lua.ignore" -Destination "$patchfolder\Lua\UI\ProposalChartPopup.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\ProposalChartPopup.xml.ignore" -Destination "$patchfolder\Lua\UI\ProposalChartPopup.xml"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\VictoryProgress.lua.ignore" -Destination "$patchfolder\Lua\UI\VictoryProgress.lua"
Copy-Item -Path "$patchfolder\lua\tmp\ui\VotingSystem\VictoryProgress.xml.ignore" -Destination "$patchfolder\Lua\UI\VictoryProgress.xml"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\Bombardment.lua.ignore" -Destination "$patchfolder\Lua\UI\Bombardment.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\DiscussionDialog.lua.ignore" -Destination "$patchfolder\Lua\UI\DiscussionDialog.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\EndGameMenu.lua.ignore" -Destination "$patchfolder\Lua\UI\EndGameMenu.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\Highlights.xml.ignore" -Destination "$patchfolder\Lua\UI\Highlights.xml"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\InGame.lua.ignore" -Destination "$patchfolder\Lua\UI\InGame.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\mountain.lua.ignore" -Destination "$patchfolder\Lua\UI\mountain.lua"
################################################################################################
Copy-Item -Path "$patchfolder\lua\tmp\ui\prophetreplace.lua.ignore" -Destination "$patchfolder\Lua\UI\prophetreplace.lua"
################################################################################################

#Invoke-PS2EXE ui_check.ps1 ui_check.exe


Set-Location -Path $patchfolder