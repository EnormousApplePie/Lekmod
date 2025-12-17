#!/bin/bash
echo "This script is a mirror of ui_check,bat for LEKMOD v34.9"
echo "[LEKMOD] Starting UI patching process..."
# Go to script directory
cd "$(dirname "$0")"

# patchfolder = current directory
PATCH="$(pwd)"

# Go one level up (same as cd ..)
cd ..

# Default folder name
EUIFOLDER="UI_bc1_xits"

# If UI_bc1 exists, use it instead
if [ -d "UI_bc1" ]; then
    EUIFOLDER="UI_bc1"
fi

############################
echo "[LEKMOD] Clearing existing patched UI files..."
rm -f "$PATCH/Lua/UI/"*
###########################
# Copy GoodyHutPopup from tmp/ui into UI, stripping .ignore extension
echo "[LEKMOD] Patching GoodyHutPopup..."
cp "$PATCH/Lua/tmp/ui/AncientRuins/GoodyHutPopup.lua.ignore" \
   "$PATCH/Lua/UI/GoodyHutPopup.lua"
###########################
# Look for the special signature inside EUI CityBannerManager.lua
SEARCH_TEXT='CityBannerProductionBox = function( city )'

echo "[LEKMOD] Patching CityBannerManager..."
if [ -f "$EUIFOLDER/CityBanners/CityBannerManager.lua" ]; then
    # If file exists, check if it contains the key text
    if grep -q "$SEARCH_TEXT" "$EUIFOLDER/CityBanners/CityBannerManager.lua"; then
        echo "  -> Detected EUI CityBannerManager variant 1"
        # Copy variant 1
        cp "$PATCH/Lua/tmp/eui/CityBanners/CityBannerManager_1.lua.ignore" \
           "$PATCH/Lua/UI/CityBannerManager.lua"

        cp "$PATCH/Lua/tmp/eui/CityBanners/CityBannerManager_1.xml.ignore" \
           "$PATCH/Lua/UI/CityBannerManager.xml"
    else
        echo "  -> Detected EUI CityBannerManager variant 2"
        # Copy variant 2
        cp "$PATCH/Lua/tmp/eui/CityBanners/CityBannerManager_2.lua.ignore" \
           "$PATCH/Lua/UI/CityBannerManager.lua"

        cp "$PATCH/Lua/tmp/eui/CityBanners/CityBannerManager_2.xml.ignore" \
           "$PATCH/Lua/UI/CityBannerManager.xml"
    fi
else
    # Not using EUI, copy vanilla UI version
    echo "  -> Using standard UI CityBannerManager"
    cp "$PATCH/Lua/tmp/ui/CityBanners/CityBannerManager.lua.ignore" \
       "$PATCH/Lua/UI/CityBannerManager.lua"

    cp "$PATCH/Lua/tmp/ui/CityBanners/CityBannerManager.xml.ignore" \
       "$PATCH/Lua/UI/CityBannerManager.xml"
fi
###########################
echo "[LEKMOD] Patching CityStateDiploPopup..."

if [ ! -f "$EUIFOLDER/CityStatePopup/CityStateDiploPopup.lua" ]; then
    echo "  -> EUI version NOT found, using UI version"
    cp "$PATCH/Lua/tmp/ui/CityStatePopup/CityStateDiploPopup.lua.ignore" \
       "$PATCH/Lua/UI/CityStateDiploPopup.lua"
else
    echo "  -> EUI version FOUND, using EUI patch"
    cp "$PATCH/Lua/tmp/eui/CityStatePopup/CityStateDiploPopup.lua.ignore" \
       "$PATCH/Lua/UI/CityStateDiploPopup.lua"
fi
###########################
echo "[LEKMOD] Patching CityView + ProductionPopup..."

SEARCH_TEXT="-- coded by bc1 from 1.0.3.276 brave new world code"

if grep -F "$SEARCH_TEXT" "$EUIFOLDER/CityView/CityView.lua" >/dev/null 2>&1; then
    echo "  -> Detected EUI CityView; applying EUI patches"
    cp "$PATCH/Lua/tmp/eui/CityView/CityView.lua.ignore"       "$PATCH/Lua/UI/CityView.lua"
    cp "$PATCH/Lua/tmp/eui/CityView/CityView.xml.ignore"       "$PATCH/Lua/UI/CityView.xml"
    cp "$PATCH/Lua/tmp/eui/CityView/CityView_small.xml.ignore" "$PATCH/Lua/UI/CityView_small.xml"
else
    echo "  -> Using standard UI CityView patches"
    cp "$PATCH/Lua/tmp/ui/CityView/CityView.lua.ignore"       "$PATCH/Lua/UI/CityView.lua"
    cp "$PATCH/Lua/tmp/ui/CityView/CityView.xml.ignore"       "$PATCH/Lua/UI/CityView.xml"
    cp "$PATCH/Lua/tmp/ui/CityView/CityView_small.xml.ignore" "$PATCH/Lua/UI/CityView_small.xml"
fi

# ProductionPopup: UI-only unless explicitly overridden by EUI
if [ ! -f "$EUIFOLDER/CityView/ProductionPopup.lua" ]; then
    echo "  -> ProductionPopup not found in EUI, applying UI variant"
    cp "$PATCH/Lua/tmp/ui/CityView/ProductionPopup.lua.ignore" "$PATCH/Lua/UI/ProductionPopup.lua"
    cp "$PATCH/Lua/tmp/ui/CityView/ProductionPopup.xml.ignore" "$PATCH/Lua/UI/ProductionPopup.xml"
else
    echo "  -> ProductionPopup exists in EUI, NO override applied"
fi
###########################
echo "[LEKMOD] Patching Core UI & EUI helpers..."

# CityStateStatusHelper (UI-only unless EUI overrides it)
if [ ! -f "$EUIFOLDER/Core/CityStateStatusHelper.lua" ]; then
    echo "  -> Using standard UI CityStateStatusHelper"
    cp "$PATCH/Lua/tmp/ui/Core/CityStateStatusHelper.lua.ignore" \
       "$PATCH/Lua/UI/CityStateStatusHelper.lua"
else
    echo "  -> EUI provides CityStateStatusHelper, skipping UI copy"
fi

# If EUI provides custom tooltip library, apply the EUI patch
if [ -f "$EUIFOLDER/Core/EUI_tooltip_library.lua" ]; then
    echo "  -> Patching EUI tooltip library"
    cp "$PATCH/Lua/tmp/eui/Core/EUI_tooltip_library.lua.ignore" \
       "$PATCH/Lua/UI/EUI_tooltip_library.lua"
else
    echo "  -> EUI tooltip library not found, skipping"
fi

# If EUI provides unit include lib, apply the EUI patch
if [ -f "$EUIFOLDER/Core/EUI_unit_include.lua" ]; then
    echo "  -> Patching EUI unit include"
    cp "$PATCH/Lua/tmp/eui/Core/EUI_unit_include.lua.ignore" \
       "$PATCH/Lua/UI/EUI_unit_include.lua"
else
    echo "  -> EUI unit include not found, skipping"
fi

# Always overwrite IconSupport with the UI version unless EUI has its own override
echo "  -> Copying UI IconSupport"
cp "$PATCH/Lua/tmp/ui/Core/IconSupport.lua.ignore" \
   "$PATCH/Lua/UI/IconSupport.lua"
###########################
echo "[LEKMOD] Applying Culture Overview UI patches..."

# Always override the main Culture Overview UI (same for UI & EUI)
echo "  -> Copying CultureOverview (UI)"
cp "$PATCH/Lua/tmp/ui/CultureOverview/CultureOverview.lua.ignore" \
   "$PATCH/Lua/UI/CultureOverview.lua"

cp "$PATCH/Lua/tmp/ui/CultureOverview/CultureOverview.xml.ignore" \
   "$PATCH/Lua/UI/CultureOverview.xml"


# Determine whether to use EUI or default UI version of EconomicGeneralInfo
if [ ! -f "$EUIFOLDER/EconomicGeneralInfo.lua" ]; then
    echo "  -> Using UI version of EconomicGeneralInfo"
    cp "$PATCH/Lua/tmp/ui/CultureOverview/EconomicGeneralInfo.lua.ignore" \
       "$PATCH/Lua/UI/EconomicGeneralInfo.lua"
    cp "$PATCH/Lua/tmp/ui/CultureOverview/EconomicGeneralInfo.xml.ignore" \
       "$PATCH/Lua/UI/EconomicGeneralInfo.xml"
else
    echo "  -> Using EUI version of EconomicGeneralInfo"
    cp "$PATCH/Lua/tmp/eui/EconomicGeneralInfo.lua.ignore" \
       "$PATCH/Lua/UI/EconomicGeneralInfo.lua"
    cp "$PATCH/Lua/tmp/eui/EconomicGeneralInfo.xml.ignore" \
       "$PATCH/Lua/UI/EconomicGeneralInfo.xml"
fi
###########################
echo "[LEKMOD] Patching FrontEnd UI components..."
# --- FrontEnd ---
cp "$PATCH/Lua/tmp/ui/FrontEnd/EULA.lua.ignore"      "$PATCH/Lua/UI/EULA.lua"
cp "$PATCH/Lua/tmp/ui/FrontEnd/FrontEnd.lua.ignore"  "$PATCH/Lua/UI/FrontEnd.lua"
###########################
echo "[LEKMOD] Patching GameSetup UI components..."
# --- SelectCivilization ---
if [ ! -f "$EUIFOLDER/GameSetup/SelectCivilization.lua" ]; then
    cp "$PATCH/Lua/tmp/ui/GameSetup/SelectCivilization.lua.ignore" \
       "$PATCH/Lua/UI/SelectCivilization.lua"
else
    cp "$PATCH/Lua/tmp/eui/GameSetup/SelectCivilization.lua.ignore" \
       "$PATCH/Lua/UI/SelectCivilization.lua"
fi
########################### <<<<<
echo "[LEKMOD] Patching GPList UI components..."
# --- GPList ---
cp "$PATCH/Lua/tmp/ui/GPList/GPList.lua.ignore" "$PATCH/Lua/UI/GPList.lua"
cp "$PATCH/Lua/tmp/ui/GPList/GPList.xml.ignore" "$PATCH/Lua/UI/GPList.xml"
###########################
echo "[LEKMOD] Patching WorldView UI components..."
# --- WorldView ---
if [ ! -f "$EUIFOLDER/Improvements/WorldView.lua" ]; then
    cp "$PATCH/Lua/tmp/ui/Improvements/WorldView.lua.ignore" \
       "$PATCH/Lua/UI/WorldView.lua"
else
    cp "$PATCH/Lua/tmp/eui/Improvements/WorldView.lua.ignore" \
       "$PATCH/Lua/UI/WorldView.lua"
fi
###########################
echo "[LEKMOD] Patching LeaderHead / Trade UI components..."
# --- LeaderHead / Trade ---
if [ ! -f "$EUIFOLDER/LeaderHead/TradeLogic.lua" ]; then
    echo "  -> Using UI version of TradeLogic"
    cp "$PATCH/Lua/tmp/ui/LeaderHead/TradeLogic.lua.ignore" \
       "$PATCH/Lua/UI/TradeLogic.lua"
else
    echo "  -> Using EUI version of TradeLogic"
    cp "$PATCH/Lua/tmp/eui/LeaderHead/TradeLogic.lua.ignore" \
       "$PATCH/Lua/UI/TradeLogic.lua"
fi

echo "  -> Patching SimpleDiploTrade UI components..."
cp "$PATCH/Lua/tmp/ui/LeaderHead/SimpleDiploTrade.lua.ignore" \
   "$PATCH/Lua/UI/SimpleDiploTrade.lua"

cp "$PATCH/Lua/tmp/ui/LeaderHead/SimpleDiploTrade.xml.ignore" \
   "$PATCH/Lua/UI/SimpleDiploTrade.xml"
###########################
# --- Lobby / MP ---
echo "[LEKMOD] Patching Lobby UI components..."
cp "$PATCH/Lua/tmp/ui/Lobby/AdvancedSetup.lua.ignore"      "$PATCH/Lua/UI/AdvancedSetup.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/JoiningRoom.lua.ignore"       "$PATCH/Lua/UI/JoiningRoom.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/JoiningRoom.xml.ignore"       "$PATCH/Lua/UI/JoiningRoom.xml"
cp "$PATCH/Lua/tmp/ui/Lobby/LoadMenu.lua.ignore"          "$PATCH/Lua/UI/LoadMenu.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/Lobby.lua.ignore"             "$PATCH/Lua/UI/Lobby.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/Lobby.xml.ignore"             "$PATCH/Lua/UI/Lobby.xml"
cp "$PATCH/Lua/tmp/ui/Lobby/MPGameOptions.lua.ignore"     "$PATCH/Lua/UI/MPGameOptions.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/MPGameSetupScreen.lua.ignore" "$PATCH/Lua/UI/MPGameSetupScreen.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/MPGameSetupScreen.xml.ignore" "$PATCH/Lua/UI/MPGameSetupScreen.xml"
cp "$PATCH/Lua/tmp/ui/Lobby/StagingRoom.lua.ignore"       "$PATCH/Lua/UI/StagingRoom.lua"
cp "$PATCH/Lua/tmp/ui/Lobby/StagingRoom.xml.ignore"       "$PATCH/Lua/UI/StagingRoom.xml"

echo "[LEKMOD] Patching PlotMouseoverInclude..."
cp "$PATCH/Lua/tmp/ui/PlotMouseoverInclude.lua.ignore"    "$PATCH/Lua/UI/PlotMouseoverInclude.lua"
###########################
# --- MiniMapPanel ---
echo "[LEKMOD] Patching MiniMapPanel components..."
cp "$PATCH/Lua/tmp/ui/MiniMapPanel/MiniMapPanel.lua.ignore" \
   "$PATCH/Lua/UI/MiniMapPanel.lua"

cp "$PATCH/Lua/tmp/ui/MiniMapPanel/MiniMapPanel.xml.ignore" \
   "$PATCH/Lua/UI/MiniMapPanel.xml"

# --- MPTurnPanel ---
echo "[LEKMOD] Patching MPTurnPanel components..."
cp "$PATCH/Lua/tmp/ui/MPTurnPanel/MPTurnPanel.lua.ignore" \
   "$PATCH/Lua/UI/MPTurnPanel.lua"

cp "$PATCH/Lua/tmp/ui/MPTurnPanel/MPTurnPanel.xml.ignore" \
   "$PATCH/Lua/UI/MPTurnPanel.xml"
###########################
echo "[LEKMOD] Patching DiploCorner UI components..."
# --- DiploCorner ---
if [ ! -f "$EUIFOLDER/NotificationPanel/DiploCorner.xml" ]; then
    echo "  -> Using UI version of DiploCorner"
    cp "$PATCH/Lua/tmp/ui/NotificationPanel/DiploCorner.lua.ignore" "$PATCH/Lua/UI/DiploCorner.lua"
    cp "$PATCH/Lua/tmp/ui/NotificationPanel/DiploCorner.xml.ignore" "$PATCH/Lua/UI/DiploCorner.xml"
else
    echo "  -> Using EUI version of DiploCorner"
    cp "$PATCH/Lua/tmp/eui/NotificationPanel/DiploCorner.lua.ignore" "$PATCH/Lua/UI/DiploCorner.lua"
    cp "$PATCH/Lua/tmp/eui/NotificationPanel/DiploCorner.xml.ignore" "$PATCH/Lua/UI/DiploCorner.xml"
fi
###########################
# --- NotificationPanel ---
echo "[LEKMOD] Patching NotificationPanel UI components..."
if [ ! -f "$EUIFOLDER/NotificationPanel/NotificationPanel.lua" ]; then
    echo "  -> Using UI version of NotificationPanel"
    cp "$PATCH/Lua/tmp/ui/NotificationPanel/NotificationPanel.lua.ignore" "$PATCH/Lua/UI/NotificationPanel.lua"
    cp "$PATCH/Lua/tmp/ui/NotificationPanel/NotificationPanel.xml.ignore" "$PATCH/Lua/UI/NotificationPanel.xml"
else
    echo "  -> Using EUI version of NotificationPanel"
    cp "$PATCH/Lua/tmp/eui/NotificationPanel/NotificationPanel.lua.ignore" "$PATCH/Lua/UI/NotificationPanel.lua"
    cp "$PATCH/Lua/tmp/eui/NotificationPanel/NotificationPanel.xml.ignore" "$PATCH/Lua/UI/NotificationPanel.xml"
fi
###########################
# --- Options Menu ---
echo "[LEKMOD] Patching Options Menu UI components..."
if [ ! -f "$EUIFOLDER/Options/OptionsMenu.lua" ]; then
    echo "  -> Using UI version of OptionsMenu"
    cp "$PATCH/Lua/tmp/ui/Options/OptionsMenu.lua.ignore" "$PATCH/Lua/UI/OptionsMenu.lua"
    cp "$PATCH/Lua/tmp/ui/Options/OptionsMenu.xml.ignore" "$PATCH/Lua/UI/OptionsMenu.xml"
else
    echo "  -> Using EUI version of OptionsMenu"
    cp "$PATCH/Lua/tmp/eui/Options/OptionsMenu.lua.ignore" "$PATCH/Lua/UI/OptionsMenu.lua"
    cp "$PATCH/Lua/tmp/eui/Options/OptionsMenu.xml.ignore" "$PATCH/Lua/UI/OptionsMenu.xml"
fi
###########################
# --- Replay UI ---
echo "[LEKMOD] Patching Replay UI components..."
cp "$PATCH/Lua/tmp/ui/Replays/Demographics.lua.ignore" "$PATCH/Lua/UI/Demographics.lua"
cp "$PATCH/Lua/tmp/ui/Replays/GameMenu.lua.ignore"     "$PATCH/Lua/UI/GameMenu.lua"
cp "$PATCH/Lua/tmp/ui/Replays/ReplayViewer.lua.ignore" "$PATCH/Lua/UI/ReplayViewer.lua"
cp "$PATCH/Lua/tmp/ui/Replays/ReplayViewer.xml.ignore" "$PATCH/Lua/UI/ReplayViewer.xml"
###########################
# --- TechTree ---
echo "[LEKMOD] Patching TechTree UI components..."
if [ ! -f "$EUIFOLDER/TechTree/TechTree.lua" ]; then
    echo "  -> Using UI version of TechTree"
    cp "$PATCH/Lua/tmp/ui/TechTree/TechPopup.lua.ignore" "$PATCH/Lua/UI/TechPopup.lua"
    cp "$PATCH/Lua/tmp/ui/TechTree/TechTree.lua.ignore"  "$PATCH/Lua/UI/TechTree.lua"
else
    echo "  -> Using EUI version of TechTree"
    cp "$PATCH/Lua/tmp/eui/TechTree/TechPopup.lua.ignore" "$PATCH/Lua/UI/TechPopup.lua"
    cp "$PATCH/Lua/tmp/eui/TechTree/TechPopup.xml.ignore" "$PATCH/Lua/UI/TechPopup.xml"
    cp "$PATCH/Lua/tmp/eui/TechTree/TechTree.lua.ignore"  "$PATCH/Lua/UI/TechTree.lua"
fi
###########################
# --- Tech Tooltip Helpers ---
echo "[LEKMOD] Patching Tech Tooltip Helper UI components..."
if [ ! -f "$EUIFOLDER/ToolTips/TechButtonInclude.lua" ]; then
    echo "  -> Using UI version of TechButtonInclude"
    cp "$PATCH/Lua/tmp/ui/ToolTips/TechButtonInclude.lua.ignore" "$PATCH/Lua/UI/TechButtonInclude.lua"
else
    echo "  -> Using EUI version of TechButtonInclude"
    cp "$PATCH/Lua/tmp/eui/ToolTips/TechButtonInclude.lua.ignore" "$PATCH/Lua/UI/TechButtonInclude.lua"
fi

echo "[LEKMOD] Patching Info Tooltip Helper UI components..."
if [ ! -f "$EUIFOLDER/ToolTips/InfoTooltipInclude.lua" ]; then
    echo "  -> Using UI version of InfoTooltipInclude"
    cp "$PATCH/Lua/tmp/ui/ToolTips/InfoTooltipInclude.lua.ignore" "$PATCH/Lua/UI/InfoTooltipInclude.lua"
else
    echo "  -> Using EUI version of InfoTooltipInclude"
    cp "$PATCH/Lua/tmp/eui/ToolTips/InfoTooltipInclude.lua.ignore" "$PATCH/Lua/UI/InfoTooltipInclude.lua"
fi
###########################
# --- UnitFlagManager ---
echo "[LEKMOD] Patching UnitFlagManager UI components..."
if [ ! -f "$EUIFOLDER/UnitFlagManager/UnitFlagManager.lua" ]; then
    echo "  -> Using UI version of UnitFlagManager"
    cp "$PATCH/Lua/tmp/ui/UnitFlagManager/UnitFlagManager.lua.ignore" "$PATCH/Lua/UI/UnitFlagManager.lua"
    cp "$PATCH/Lua/tmp/ui/UnitFlagManager/UnitFlagManager.xml.ignore" "$PATCH/Lua/UI/UnitFlagManager.xml"
else
    echo "  -> Using EUI version of UnitFlagManager"
    cp "$PATCH/Lua/tmp/eui/UnitFlagManager/UnitFlagManager.lua.ignore" "$PATCH/Lua/UI/UnitFlagManager.lua"
    cp "$PATCH/Lua/tmp/eui/UnitFlagManager/UnitFlagManager.xml.ignore" "$PATCH/Lua/UI/UnitFlagManager.xml"
fi
###########################
# --- Unit List ---
echo "[LEKMOD] Patching Unit List UI components..."
cp "$PATCH/Lua/tmp/ui/UnitList/UnitList.lua.ignore" "$PATCH/Lua/UI/UnitList.lua"
cp "$PATCH/Lua/tmp/ui/UnitList/UnitList.xml.ignore" "$PATCH/Lua/UI/UnitList.xml"

# --- EnemyUnitPanel ---
echo "[LEKMOD] Patching EnemyUnitPanel UI components..."
cp "$PATCH/Lua/tmp/ui/UnitPanel/EnemyUnitPanel.lua.ignore" "$PATCH/Lua/UI/EnemyUnitPanel.lua"

# --- UnitPanel (EUI check) ---
echo "[LEKMOD] Patching UnitPanel UI components..."
if grep -q "-- modified by bc1 from Civ V 1.0.3.276 code" "$EUIFOLDER/UnitPanel/UnitPanel.lua" 2>/dev/null; then
    echo "  -> Using EUI version of UnitPanel"
    cp "$PATCH/Lua/tmp/eui/UnitPanel/UnitPanel.lua.ignore" "$PATCH/Lua/UI/UnitPanel.lua"
else
    echo "  -> Using UI version of UnitPanel"
    cp "$PATCH/Lua/tmp/ui/UnitPanel/UnitPanel.lua.ignore" "$PATCH/Lua/UI/UnitPanel.lua"
fi
###########################
# --- Voting UI ---
echo "[LEKMOD] Patching Voting System UI components..."
cp "$PATCH/Lua/tmp/ui/VotingSystem/CCVotePopup.lua.ignore"      "$PATCH/Lua/UI/CCVotePopup.lua"
cp "$PATCH/Lua/tmp/ui/VotingSystem/CCVotePopup.xml.ignore"      "$PATCH/Lua/UI/CCVotePopup.xml"
cp "$PATCH/Lua/tmp/ui/VotingSystem/NetworkKickedPopup.lua.ignore" "$PATCH/Lua/UI/NetworkKickedPopup.lua"
cp "$PATCH/Lua/tmp/ui/VotingSystem/ProposalChartPopup.lua.ignore" "$PATCH/Lua/UI/ProposalChartPopup.lua"
cp "$PATCH/Lua/tmp/ui/VotingSystem/ProposalChartPopup.xml.ignore" "$PATCH/Lua/UI/ProposalChartPopup.xml"
cp "$PATCH/Lua/tmp/ui/VotingSystem/VictoryProgress.lua.ignore"    "$PATCH/Lua/UI/VictoryProgress.lua"
cp "$PATCH/Lua/tmp/ui/VotingSystem/VictoryProgress.xml.ignore"    "$PATCH/Lua/UI/VictoryProgress.xml"
###########################
# --- Bombardment ---
echo "[LEKMOD] Patching Bombardment UI components..."
cp "$PATCH/Lua/tmp/ui/Bombardment.lua.ignore" "$PATCH/Lua/UI/Bombardment.lua"
###########################
# --- Misc UI ---
echo "[LEKMOD] Patching Misc UI components..."
cp "$PATCH/Lua/tmp/ui/DiscussionDialog.lua.ignore" "$PATCH/Lua/UI/DiscussionDialog.lua"
cp "$PATCH/Lua/tmp/ui/EndGameMenu.lua.ignore"      "$PATCH/Lua/UI/EndGameMenu.lua"
cp "$PATCH/Lua/tmp/ui/Highlights.xml.ignore"       "$PATCH/Lua/UI/Highlights.xml"
cp "$PATCH/Lua/tmp/ui/InGame.lua.ignore"           "$PATCH/Lua/UI/InGame.lua"
cp "$PATCH/Lua/tmp/ui/mountain.lua.ignore"         "$PATCH/Lua/UI/mountain.lua"
cp "$PATCH/Lua/tmp/ui/prophetreplace.lua.ignore"   "$PATCH/Lua/UI/prophetreplace.lua"
###########################
# --- YieldIconManager ---
echo "[LEKMOD] Patching YieldIconManager UI components..."
if [ ! -f "$EUIFOLDER/Improvements/YieldIconManager.lua" ]; then
    echo "  -> Using UI version of YieldIconManager"
    cp "$PATCH/Lua/tmp/ui/YieldIconManager.lua.ignore" "$PATCH/Lua/UI/YieldIconManager.lua"
else
    echo "  -> Using EUI version of YieldIconManager"
    cp "$PATCH/Lua/tmp/eui/Improvements/YieldIconManager.lua.ignore" "$PATCH/Lua/UI/YieldIconManager.lua"
fi
###########################
# --- TopPanel ---
echo "[LEKMOD] Patching TopPanel UI components..."
if [ ! -f "$EUIFOLDER/TopPanel/TopPanel.lua" ]; then
    echo "  -> Using UI version of TopPanel"
    cp "$PATCH/Lua/tmp/ui/TopPanel.lua.ignore" "$PATCH/Lua/UI/TopPanel.lua"
else
    echo "  -> Using EUI version of TopPanel"
    cp "$PATCH/Lua/tmp/eui/TopPanel.lua.ignore" "$PATCH/Lua/UI/TopPanel.lua"
    cp "$PATCH/Lua/tmp/eui/TopPanel.xml.ignore" "$PATCH/Lua/UI/TopPanel.xml"
fi

printf "\n\n\n[LEKMOD] UI patching process completed.\n\nSUCCESS\n\n"
exit 0