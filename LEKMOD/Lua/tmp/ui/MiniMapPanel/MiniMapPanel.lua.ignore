----------------------------------------------------------------
-- edit: Victory Screen button for EUI & vanilla UI
----------------------------------------------------------------       
----------------------------------------------------------------        
include( "InstanceManager" );

local g_LegendIM = InstanceManager:new( "LegendKey", "Item", Controls.LegendStack );
local g_Overlays = GetStrategicViewOverlays();
local g_IconModes = GetStrategicViewIconSettings();

----------------------------------------------------------------
----------------------------------------------------------------

--add lekmoddata for remembering stratview and streamer panel

if LekmodData == nil then
	LekmodData = Modding.OpenUserData("LekmodData", 1);
end

----------------------------------------------------------------        
----------------------------------------------------------------

function OnMinimapInfo( uiHandle, width, height, paddingX )
    Controls.Minimap:SetTextureHandle( uiHandle );
    Controls.Minimap:SetSizeVal( width, height );
end
Events.MinimapTextureBroadcastEvent.Add( OnMinimapInfo );
UI:RequestMinimapBroadcast();

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnMinimapClick( _, _, _, x, y )
    Events.MinimapClickedEvent( x, y );
end
Controls.Minimap:RegisterCallback( Mouse.eLClick, OnMinimapClick );

----------------------------------------------------------------        
----------------------------------------------------------------
-- save strat view defaults
local bEnabledStratView = LekmodData.GetValue("bEnabledStratView") or 0;  

function OnStrategicView()
	local bIsObserver = PreGame.GetSlotStatus( Game.GetActivePlayer() ) == SlotStatus.SS_OBSERVER;
	if (bIsObserver) then
		-- Observer gets to toggle the world view completely off.
		local eViewType = GetGameViewRenderType();
		if (eViewType == GameViewTypes.GAMEVIEW_NONE) then
			SetGameViewRenderType(GameViewTypes.GAMEVIEW_STANDARD);			
		else
			if (eViewType == GameViewTypes.GAMEVIEW_STANDARD) then
				SetGameViewRenderType(GameViewTypes.GAMEVIEW_STRATEGIC);
			else
				SetGameViewRenderType(GameViewTypes.GAMEVIEW_NONE);
			end
		end
	else
		ToggleStrategicView();
	end		
end
function EnableStratViewOnLoad()
	if (bEnabledStratView == 1) then
		OnStrategicView();
	end
end
Events.SequenceGameInitComplete.Add(EnableStratViewOnLoad);
Controls.StrategicViewButton:RegisterCallback( Mouse.eLClick, OnStrategicView );

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnResetTurnTimer()
	local bIsObserver = PreGame.GetSlotStatus( Game.GetActivePlayer() ) == SlotStatus.SS_OBSERVER;
	if (not bIsObserver) then
			Game.DoControl(GameInfoTypes.CONTROL_UNIT_ICONS);
	end		
end
Controls.ResetTurnTimerButton:RegisterCallback( Mouse.eLClick, OnResetTurnTimer );

function OnPauseTurnTimer()
	local bIsObserver = PreGame.GetSlotStatus( Game.GetActivePlayer() ) == SlotStatus.SS_OBSERVER;
	if (not bIsObserver) then
			Game.DoControl(GameInfoTypes.CONTROL_SCORES);
	end		
end
Controls.ResetTurnTimerButton:RegisterCallback( Mouse.eRClick, OnPauseTurnTimer );

if(not Game.IsOption("GAMEOPTION_END_TURN_TIMER_ENABLED")) then
	Controls.ResetTurnTimerButton:SetHide(true);
end

----------------------------------------------------------------  
-- NEW: show victory screen button when game is over      
----------------------------------------------------------------    	

function OnVictoryScreenOpen()
	local victory = Game.GetVictory();
	local endgametype = -1;
	if victory == GameInfoTypes.VICTORY_TIME then
		endgametype = EndGameTypes.Time;
	elseif victory == GameInfoTypes.VICTORY_SPACE_RACE then
		endgametype = EndGameTypes.Technology;
	elseif victory == GameInfoTypes.VICTORY_DOMINATION then
		endgametype = EndGameTypes.Domination;
	elseif victory == GameInfoTypes.VICTORY_CULTURAL then
		endgametype = EndGameTypes.Culture;
	elseif victory == GameInfoTypes.VICTORY_DIPLOMATIC then
		endgametype = EndGameTypes.Diplomatic;
	elseif victory == GameInfoTypes.VICTORY_SCRAP then
		endgametype = 11;
	end
	Events.EndGameShow( endgametype, -2 );
end
Controls.VictoryScreenButton:RegisterCallback( Mouse.eLClick, OnVictoryScreenOpen );

if (Game.GetGameState() == GameplayGameStateTypes.GAMESTATE_ON) then
	Controls.VictoryScreenButton:SetHide(true);
else
	Controls.VictoryScreenButton:SetHide(false);
end
Events.EndGameShow.Add( function() Controls.VictoryScreenButton:SetHide(false) end);

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnStrategicViewStateChanged(bStrategicView)
	
	if bStrategicView then
		LekmodData.SetValue("bEnabledStratView", 1);
		Controls.ShowResources:SetDisabled( true );
		Controls.ShowResources:SetAlpha( 0.5 );
		Controls.StrategicViewButton:SetTexture( "assets/UI/Art/Icons/MainWorldButton.dds" );
		Controls.StrategicMO:SetTexture( "assets/UI/Art/Icons/MainWorldButton.dds" );
		Controls.StrategicHL:SetTexture( "assets/UI/Art/Icons/MainWorldButtonHL.dds" );
	else
		LekmodData.SetValue("bEnabledStratView", 0);
		Controls.ShowGrid:SetCheck(OptionsManager.GetGridOn());
		Controls.ShowResources:SetDisabled( false );
		Controls.ShowResources:SetAlpha( 1.0 );
		Controls.StrategicViewButton:SetTexture( "assets/UI/Art/Icons/MainStrategicButton.dds" );
		Controls.StrategicMO:SetTexture( "assets/UI/Art/Icons/MainStrategicButton.dds" );
		Controls.StrategicHL:SetTexture( "assets/UI/Art/Icons/MainStrategicButtonHL.dds" );
	end

	SetLegend();
    UpdateOptionsPanel();
end
Events.StrategicViewStateChanged.Add( OnStrategicViewStateChanged );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnMapOptions()	
	Controls.OptionsPanel:SetHide( not Controls.OptionsPanel:IsHidden() );
	Controls.SideStack:CalculateSize();
	Controls.SideStack:ReprocessAnchoring();
	
	SetLegend();
    UpdateOptionsPanel();
end
Controls.MapOptionsButton:RegisterCallback( Mouse.eLClick, OnMapOptions );


----------------------------------------------------------------        
----------------------------------------------------------------        
g_MapOptionDefaults = nil;
local g_PerPlayerMapOptions = {};
----------------------------------------------------------------    
-- Get the map option defaults from the options manager    
----------------------------------------------------------------        
function GetMapOptionDefaults()
	if (g_MapOptionDefaults == nil) then
		-- Pull in the current setting the first time through
		g_MapOptionDefaults = {};
		g_MapOptionDefaults.ShowTrade = OptionsManager.GetShowTradeOn();
		g_MapOptionDefaults.ShowGrid = OptionsManager.GetGridOn();
		g_MapOptionDefaults.ShowYield = OptionsManager.GetYieldOn();
		g_MapOptionDefaults.ShowResources = OptionsManager.GetResourceOn();
		g_MapOptionDefaults.HideTileRecommendations = OptionsManager.IsNoTileRecommendations();

		g_MapOptionDefaults.SVIconMode = 1;
		g_MapOptionDefaults.SVOverlayMode = 1;
		g_MapOptionDefaults.SVShowFeatures = true;
		g_MapOptionDefaults.SVShowFOW = true;
	end
	
	local mapOptions = {};
	for k, v in pairs(g_MapOptionDefaults) do mapOptions[k] = v; end 
	
	return mapOptions;
end

----------------------------------------------------------------    
-- Take the current options and apply them to the game
----------------------------------------------------------------        
function ApplyMapOptions(mapOptions)

	UI.SetGridVisibleMode( mapOptions.ShowGrid );
	UI.SetYieldVisibleMode( mapOptions.ShowYield );
	UI.SetResourceVisibleMode( mapOptions.ShowResources );
	Events.Event_ToggleTradeRouteDisplay(mapOptions.ShowTrade);
	
	LuaEvents.OnRecommendationCheckChanged(mapOptions.HideTileRecommendations);
	
	-- Because outside contexted will also want to access the settings, we have to push them back to the OptionsManager	
	if (PreGame.IsHotSeatGame()) then
		local bChanged = false;
		
		if (OptionsManager.GetGridOn() ~= mapOptions.ShowGrid) then
			OptionsManager.SetGridOn_Cached( mapOptions.ShowGrid );
			bChanged = true;
		end
		if (OptionsManager.GetShowTradeOn() ~= mapOptions.ShowTrade) then 
			OptionsManager.SetShowTradeOn_Cached( mapOptions.ShowTrade );
			bChanged = true;
		end
		if (OptionsManager.GetYieldOn() ~= mapOptions.ShowYield) then
			OptionsManager.SetYieldOn_Cached( mapOptions.ShowYield );
			bChanged = true;
		end
		if (OptionsManager.GetResourceOn() ~= mapOptions.ShowResources) then
			OptionsManager.SetResourceOn_Cached( mapOptions.ShowResources );
			bChanged = true;
		end
		if (OptionsManager.IsNoTileRecommendations() ~= mapOptions.HideTileRecommendations) then
			OptionsManager.SetNoTileRecommendations_Cached(mapOptions.HideTileRecommendations );
			bChanged = true;
		end
		-- We will tell the manager to not write them out
		if (bChanged) then 
			OptionsManager.CommitGameOptions(true);
		end
	end
	
	StrategicViewShowFeatures( mapOptions.SVShowFeatures );
	StrategicViewShowFogOfWar( mapOptions.SVShowFOW );
	SetStrategicViewIconSetting( mapOptions.SVIconMode ); 
	SetStrategicViewOverlay( mapOptions.SVOverlayMode ); 
end

----------------------------------------------------------------    
-- Store the current options to the specified player's slot
----------------------------------------------------------------        
function StoreMapOptions(iPlayer, mapOptions)
	g_PerPlayerMapOptions[iPlayer] = mapOptions;
end

----------------------------------------------------------------        
----------------------------------------------------------------        
function GetMapOptions(iPlayer)

	local mapOptions;
	
	-- Get the options from the local player cache
	if (g_PerPlayerMapOptions[iPlayer] == nil) then
		-- Initialize with the defaults
		mapOptions = GetMapOptionDefaults();			
		StoreMapOptions(iPlayer, mapOptions);
	else
		mapOptions = g_PerPlayerMapOptions[iPlayer];
	end
	
	return mapOptions;
end		

----------------------------------------------------------------        
----------------------------------------------------------------        
function UpdateOptionsPanel()
	
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	
	Controls.ShowTrade:SetCheck( mapOptions.ShowTrade );
	Controls.ShowGrid:SetCheck( mapOptions.ShowGrid );
	Controls.ShowYield:SetCheck( mapOptions.ShowYield );
	Controls.ShowResources:SetCheck( mapOptions.ShowResources );
	Controls.HideRecommendation:SetCheck( mapOptions.HideTileRecommendations );
	Controls.StrategicStack:SetHide( not InStrategicView() );
	
	Controls.ShowFeatures:SetCheck( mapOptions.SVShowFeatures );
	Controls.ShowFogOfWar:SetCheck( mapOptions.SVShowFOW );	

   	Controls.OverlayDropDown:GetButton():SetText(Locale.ConvertTextKey( g_Overlays[mapOptions.SVOverlayMode] ));
	SetLegend(mapOptions.SVOverlayMode);
	
	Controls.IconDropDown:GetButton():SetText( Locale.ConvertTextKey( g_IconModes[mapOptions.SVIconMode] ));
	
	Controls.StrategicStack:CalculateSize();
	Controls.MainStack:CalculateSize();
	Controls.OptionsPanel:DoAutoSize();
	
	Controls.SideStack:CalculateSize();
	Controls.SideStack:ReprocessAnchoring();
end
Events.GameOptionsChanged.Add(UpdateOptionsPanel);
	
----------------------------------------------------------------        
----------------------------------------------------------------        
function PopulateOverlayPulldown( pullDown )

	for i, text in pairs( g_Overlays ) do
	    controlTable = {};
        pullDown:BuildEntry( "InstanceOne", controlTable );

        controlTable.Button:SetVoid1( i );
        controlTable.Button:SetText( Locale.ConvertTextKey( text ) );
   	end
   	
   	Controls.OverlayDropDown:GetButton():SetText(Locale.ConvertTextKey( g_Overlays[1] ));
	
	pullDown:CalculateInternals();
    pullDown:RegisterSelectionCallback( OnOverlaySelected );
end


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnOverlaySelected( index )
	SetStrategicViewOverlay(index); 
	
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.SVOverlayMode = index;
	
	Controls.OverlayDropDown:GetButton():SetText( Locale.ConvertTextKey( g_Overlays[index] ));
	SetLegend(index);
end

----------------------------------------------------------------        
----------------------------------------------------------------        
function SetLegend(index)
	g_LegendIM:ResetInstances();
	
	local info = GetOverlayLegend();
	if(index ~= nil) then
		Controls.OverlayTitle:SetText(Locale.ConvertTextKey( g_Overlays[index] ));
	end
	
	if(info ~= nil and InStrategicView()) then
		for i, v in pairs(info) do
			local controlTable = g_LegendIM:GetInstance();
			
			local keyColor = { x = v.Color.R, y = v.Color.G, z = v.Color.B, w = 1 };
			controlTable.KeyColor:SetColor(keyColor);
			controlTable.KeyName:LocalizeAndSetText(v.Name);
		end

		Controls.LegendStack:CalculateSize();
		Controls.LegendStack:ReprocessAnchoring();
		
		Controls.LegendFrame:SetHide(false);
		Controls.LegendFrame:DoAutoSize();
		
    	Controls.SideStack:CalculateSize();
    	Controls.SideStack:ReprocessAnchoring();
	else
		Controls.LegendFrame:SetHide(true);
    	Controls.SideStack:CalculateSize();
    	Controls.SideStack:ReprocessAnchoring();
	end
	
end

----------------------------------------------------------------        
----------------------------------------------------------------        
function PopulateIconPulldown( pullDown )
	
	for i, text in pairs(g_IconModes) do
	    controlTable = {};
        pullDown:BuildEntry( "InstanceOne", controlTable );

        controlTable.Button:SetVoid1( i );
        controlTable.Button:SetText( Locale.ConvertTextKey( text ) );
   	end
   	
   	Controls.IconDropDown:GetButton():SetText(Locale.ConvertTextKey( g_IconModes[1] ));
	
	pullDown:CalculateInternals();
    pullDown:RegisterSelectionCallback( OnIconModeSelected );
end


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnIconModeSelected( index )
	SetStrategicViewIconSetting(index); 
	
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.SVIconMode = index;
	
	Controls.IconDropDown:GetButton():SetText( Locale.ConvertTextKey( g_IconModes[index] ));
end


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnShowFeaturesChecked( bIsChecked )
	StrategicViewShowFeatures( bIsChecked );
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.SVShowFeatures = bIsChecked;
end
Controls.ShowFeatures:RegisterCheckHandler( OnShowFeaturesChecked );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnShowFOWChecked( bIsChecked )
	StrategicViewShowFogOfWar( bIsChecked );
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.SVShowFOW = bIsChecked;
end
Controls.ShowFogOfWar:RegisterCheckHandler( OnShowFOWChecked );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnShowGridChecked( bIsChecked )
	
	UI.SetGridVisibleMode(bIsChecked);
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowGrid = bIsChecked;
	
	OptionsManager.SetGridOn_Cached( bIsChecked );
	OptionsManager.CommitGameOptions( PreGame.IsHotSeatGame() );
		
end
Controls.ShowGrid:RegisterCheckHandler( OnShowGridChecked );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnGridOn()
    Controls.ShowGrid:SetCheck( true );
    
    local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowGrid = true;
	
end
Events.SerialEventHexGridOn.Add( OnGridOn )


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnGridOff()
    Controls.ShowGrid:SetCheck( false );
    
    local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowGrid = false;
	
end
Events.SerialEventHexGridOff.Add( OnGridOff )

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnShowTradeToggled( bIsChecked )
	Controls.ShowTrade:SetCheck(bIsChecked);
	
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowTrade = bIsChecked;
end
Events.Event_ToggleTradeRouteDisplay.Add( OnShowTradeToggled )

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnShowTradeChecked( bIsChecked )
	Events.Event_ToggleTradeRouteDisplay(bIsChecked);
	
	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowTrade = bIsChecked;
    
	OptionsManager.SetShowTradeOn_Cached( bIsChecked );
	OptionsManager.CommitGameOptions( PreGame.IsHotSeatGame() );
end
Controls.ShowTrade:RegisterCheckHandler( OnShowTradeChecked );

----------------------------------------------------------------        
----------------------------------------------------------------        
function OnYieldChecked( bIsChecked )
    UI.SetYieldVisibleMode(bIsChecked);
    local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowYield = bIsChecked;
    
	OptionsManager.SetYieldOn_Cached( bIsChecked );
	OptionsManager.CommitGameOptions( PreGame.IsHotSeatGame() );
end
Controls.ShowYield:RegisterCheckHandler( OnYieldChecked );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnResourcesChecked( bIsChecked )
    UI.SetResourceVisibleMode(bIsChecked);
    local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.ShowResources = bIsChecked;
    
	OptionsManager.SetResourceOn_Cached( bIsChecked );
	OptionsManager.CommitGameOptions(PreGame.IsHotSeatGame());
    
end
Controls.ShowResources:RegisterCheckHandler( OnResourcesChecked );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnYieldDisplay( type )

	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	
    if( type == YieldDisplayTypes.USER_ALL_ON ) then
        Controls.ShowYield:SetCheck( true );
        mapOptions.ShowYield = true;
        
    elseif( type == YieldDisplayTypes.USER_ALL_OFF ) then
        Controls.ShowYield:SetCheck( false );
         mapOptions.ShowYield = false;
        
    elseif( type == YieldDisplayTypes.USER_ALL_RESOURCE_ON ) then
        Controls.ShowResources:SetCheck( true );
        mapOptions.ShowResources = true;
        
    elseif( type == YieldDisplayTypes.USER_ALL_RESOURCE_OFF ) then
        Controls.ShowResources:SetCheck( false );
        mapOptions.ShowResources = false;
    end
    
end
Events.RequestYieldDisplay.Add( OnYieldDisplay );


----------------------------------------------------------------        
----------------------------------------------------------------        
function OnRecommendationChecked( bIsChecked )

	local mapOptions = GetMapOptions(Game.GetActivePlayer());
	mapOptions.HideTileRecommendations = bIsChecked;
	
	OptionsManager.SetNoTileRecommendations_Cached( bIsChecked );
	OptionsManager.CommitGameOptions(PreGame.IsHotSeatGame());
	LuaEvents.OnRecommendationCheckChanged( bIsChecked );
end
Controls.HideRecommendation:RegisterCheckHandler( OnRecommendationChecked );

----------------------------------------------------------------        
----------------------------------------------------------------   
function UpdateStrategicViewToggleTT()
	local controlInfo = GameInfo.Controls.CONTROL_TOGGLE_STRATEGIC_VIEW;
	if( controlInfo ~= nil and type(controlInfo) == "table" ) then
		local hotKey = controlInfo.HotKey;
		if( hotKey ~= nil and type(hotKey) == "string" ) then
			local keyDesc = Locale.GetHotKeyDescription(hotKey, controlInfo.CtrlDown, controlInfo.AltDown, controlInfo.ShiftDown);
			if( keyDesc ~= nil and type(keyDesc) == "string" ) then
				local sToolTip = Locale.ConvertTextKey("TXT_KEY_POP_STRATEGIC_VIEW_TT");
				Controls.StrategicViewButton:SetToolTipString(sToolTip .. " (" .. keyDesc .. ")");
			end
		end
	end
end

----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
function OnActivePlayerChanged(iActivePlayer, iPrevActivePlayer)

	local mapOptions = GetMapOptions(iActivePlayer);
	ApplyMapOptions(mapOptions);
	UpdateOptionsPanel();
	
	if (not Controls.OptionsPanel:IsHidden()) then
		OnMapOptions();
	end
end
Events.GameplaySetActivePlayer.Add(OnActivePlayerChanged);

PopulateOverlayPulldown( Controls.OverlayDropDown );
PopulateIconPulldown( Controls.IconDropDown );
UpdateOptionsPanel();
UpdateStrategicViewToggleTT();

-------------------------
--- STREAMER PANEL UI ---
-------------------------

function UpdateStreamerView()
	local pActivePlayer = Players[ Game.GetActivePlayer() ];

	-- Policies
	local strPoliciesTextFull = "";
	local strPoliciesTextAbbreviated = "";
	for pPolicyBranch in GameInfo.PolicyBranchTypes() do
		local iPolicyBranch = pPolicyBranch.ID;
					
		local iCount = 0;
		local bBranchOpened = pActivePlayer:IsPolicyBranchUnlocked(iPolicyBranch);

		for pPolicy in GameInfo.Policies() do
			local iPolicy = pPolicy.ID;
						
			if (pPolicy.PolicyBranchType == pPolicyBranch.Type) then
				if (pActivePlayer:HasPolicy(iPolicy)) then
					iCount = iCount + 1;
				end
			end
		end

		if (iCount > 0 or bBranchOpened) then
			if (strPoliciesTextFull ~= "") then
				strPoliciesTextFull = strPoliciesTextFull .. ", ";
				strPoliciesTextAbbreviated = strPoliciesTextAbbreviated .. ", ";
			end

			local strBranchName = Locale.ConvertTextKey(pPolicyBranch.Description);
			--local strBranchNameAbbrev = string.upper(string.sub(strBranchName, 1, 2));
			local strBranchNameAbbrev = string.sub(strBranchName, 1, 3);

			strPoliciesTextFull = strPoliciesTextFull .. strBranchName .. " " .. iCount;
			strPoliciesTextAbbreviated = strPoliciesTextAbbreviated .. strBranchNameAbbrev .. " " .. iCount;
		end
	end

	-- if it's empty, make it look right and grey it out
	if (strPoliciesTextFull == "") then
		strPoliciesText = "[COLOR_GREY]" .. Locale.ConvertTextKey("TXT_KEY_VP_POLICIES") .. ": " .. Locale.ConvertTextKey("TXT_KEY_RO_BELIEFS_NONE") .. "[ENDCOLOR]";
	-- otherwise, if it's too long, use the abbrivated version
	elseif (string.len(strPoliciesTextFull) > 75) then
		strPoliciesText = strPoliciesTextAbbreviated;
	-- otherwise, use the full version
	else
		strPoliciesText = strPoliciesTextFull;
	end

	-- religion strings
	local strReligion1Text = "";
	local strReligion2Text = "";
	if (pActivePlayer:HasCreatedReligion()) then
		local beliefCount = 0;
		local eReligion = pActivePlayer:GetReligionCreatedByPlayer();
		for i,v in ipairs(Game.GetBeliefsInReligion(eReligion)) do
			local belief = GameInfo.Beliefs[v];
			if (belief ~= nil) then
				if (beliefCount < 3) then
					-- first religion line
					if (strReligion1Text ~= "") then
						strReligion1Text = strReligion1Text .. ", ";
					end
					strReligion1Text = strReligion1Text .. Locale.ConvertTextKey(belief.ShortDescription);
				else
					-- second religion line
					if (strReligion2Text == "") then
						strReligion1Text = strReligion1Text .. ", ";
					else
						strReligion2Text = strReligion2Text .. ", ";
					end
					strReligion2Text = strReligion2Text .. Locale.ConvertTextKey(belief.ShortDescription);
				end
				beliefCount = beliefCount + 1;
			end
		end
	elseif (pActivePlayer:HasCreatedPantheon()) then
		local belief = GameInfo.Beliefs[pActivePlayer:GetBeliefInPantheon()];
		if (belief ~= nil) then
			strReligion1Text = Locale.ConvertTextKey("TXT_KEY_RELIGION_PANTHEON") .. ": " .. Locale.ConvertTextKey(belief.ShortDescription);
			strReligion2Text = "[COLOR_GREY]" .. Locale.ConvertTextKey("TXT_KEY_RO_WR_RELIGION") .. ": " .. Locale.ConvertTextKey("TXT_KEY_RO_BELIEFS_NONE") .. "[ENDCOLOR]";
		end
	else
		strReligion1Text = "[COLOR_GREY]" .. Locale.ConvertTextKey("TXT_KEY_RELIGION_PANTHEON") .. ": " .. Locale.ConvertTextKey("TXT_KEY_RO_BELIEFS_NONE") .. "[ENDCOLOR]";
		strReligion2Text = "";
	end

	Controls.StreamerPoliciesText:SetText(strPoliciesText);
	Controls.StreamerBeliefs1Text:SetText(strReligion1Text);
	Controls.StreamerBeliefs2Text:SetText(strReligion2Text);
end



local bEnabledStreamerPanel = LekmodData.GetValue("bEnabledStreamerPanel") or 0;

function OnStreamerViewShow()
	--set the value in the database
	LekmodData.SetValue("bEnabledStreamerPanel", 1);

	
	print(bEnabledStreamerPanel);
	print("OnStreamerViewShow");
    UpdateStreamerView();
	Controls.StreamerPanel:SetHide(false);
	Controls.StreamerViewButtonOpen:SetHide(true);
	Controls.StreamerViewButtonClose:SetHide(false);
end
Controls.StreamerViewButtonOpen:RegisterCallback( Mouse.eLClick, OnStreamerViewShow );

function OnStreamerViewHide()
	LekmodData.SetValue("bEnabledStreamerPanel", 0);
	print(bEnabledStreamerPanel);
	Controls.StreamerPanel:SetHide(true);
	Controls.StreamerViewButtonOpen:SetHide(false);
	Controls.StreamerViewButtonClose:SetHide(true);
end
Controls.StreamerViewButtonClose:RegisterCallback( Mouse.eLClick, OnStreamerViewHide );

function onGameLoadStreamerPanel()
	print("onGameLoadStreamerPanel");
	if (bEnabledStreamerPanel == 1) then
		OnStreamerViewShow();
	else
		OnStreamerViewHide();
	end
end
Events.SequenceGameInitComplete.Add(onGameLoadStreamerPanel);

GameEvents.PlayerAdoptPolicyBranch.Add(UpdateStreamerView);
GameEvents.PlayerAdoptPolicy.Add(UpdateStreamerView);
GameEvents.PantheonFounded.Add(UpdateStreamerView);
GameEvents.ReligionFounded.Add(UpdateStreamerView);
GameEvents.ReligionEnhanced.Add(UpdateStreamerView);
GameEvents.ReformationAdded.Add(UpdateStreamerView);
UpdateStreamerView();