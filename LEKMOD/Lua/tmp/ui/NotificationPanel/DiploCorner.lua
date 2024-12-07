-------------------------------------------------
-- Diplomacy and Advisors Buttons that float out in the screen
-------------------------------------------------
-- edit:
--     Ingame Hotkey Manager – extended controls
--     Restore messages on game load
--     Emote Picker Menu
-- for vanilla UI
-------------------------------------------------
g_needsUpdate = true;
g_bWaitForKeyUp = false;
-- NEW: simple map from legacy KB to VK keycodes
g_KeyMap = { 27, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 189, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 187, 8, 9, 219, 220, 13, 162, 186, 222, 192, 160, 220, 188, 190, 191, 161, 106,
    164, 32, 20, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 144, 145, 103, 104, 105, 109, 100, 101, 102, 107, 97, 98,
    99, 96, 110, 122, 123, -1, -1, -1, -1, -1, 163, 174, 175, -1, 111, -1, 165, 19, 36, 38, 33, 37, 39, 35, 40, 34, 45, 46 };
-- NEW update GameInfoActions for this context explicitly
-- check g_needsUpdate before any call to GameInfoActions[].HotKey property
LuaEvents.UpdateHotkey.Add(function() g_needsUpdate = true; end);
-- NEW: search for chat control index
local ToggleChat = -1;
for actionID, action in next, GameInfoActions do
    if action.Type == 'CONTROL_TOGGLE_CHAT' then
        ToggleChat = actionID;
    end
end
-------------------------------------------------
include( "IconSupport" );
include( "SupportFunctions"  );
include( "InstanceManager" );
local g_ChatInstances = {};

local g_iChatTeam   = -1;
local g_iChatPlayer = -1;

local g_iLocalPlayer = Game.GetActivePlayer();
local g_pLocalPlayer = Players[ g_iLocalPlayer ];
local g_iLocalTeam = g_pLocalPlayer:GetTeam();
local g_pLocalTeam = Teams[ g_iLocalTeam ];      

local m_bChatOpen = not Controls.ChatPanel:IsHidden();


-------------------------------------------------
-------------------------------------------------
-- This method refreshes the entries that are in the additional information dropdown.
-- Modders can use the Lua event "AdditionalInformationDropdownGatherEntries" to 
-- add entries to the list.
function RefreshAdditionalInformationEntries()

	local function Popup(popupType, data1, data2)
		Events.SerialEventGameMessagePopup{ 
			Type = popupType,
			Data1 = data1,
			Data2 = data2
		};
	end

	local additionalEntries = {
		{ text = Locale.Lookup("TXT_KEY_ADVISOR_COUNSEL"),					call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_ADVISOR_COUNSEL); end};
		{ text = Locale.Lookup("TXT_KEY_ADVISOR_SCREEN_TECH_TREE_DISPLAY"), call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_TECH_TREE, nil, -1); end };
		{ text = Locale.Lookup("TXT_KEY_DIPLOMACY_OVERVIEW"),				call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_DIPLOMATIC_OVERVIEW); end };
		{ text = Locale.Lookup("TXT_KEY_MILITARY_OVERVIEW"),				call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_MILITARY_OVERVIEW); end };
		{ text = Locale.Lookup("TXT_KEY_ECONOMIC_OVERVIEW"),				call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_ECONOMIC_OVERVIEW); end };
		{ text = Locale.Lookup("TXT_KEY_VP_TT"),							call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_VICTORY_INFO); end };
		{ text = Locale.Lookup("TXT_KEY_DEMOGRAPHICS"),						call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_DEMOGRAPHICS); end };
		{ text = Locale.Lookup("TXT_KEY_POP_NOTIFICATION_LOG"),				call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_NOTIFICATION_LOG,Game.GetActivePlayer()); end };
		{ text = Locale.Lookup("TXT_KEY_TRADE_ROUTE_OVERVIEW"),				call=function() Popup(ButtonPopupTypes.BUTTONPOPUP_TRADE_ROUTE_OVERVIEW); end };
	};

	-- Obtain any modder/dlc entries.
	LuaEvents.AdditionalInformationDropdownGatherEntries(additionalEntries);
	
	-- Now that we have all entries, call methods to sort them
	LuaEvents.AdditionalInformationDropdownSortEntries(additionalEntries);

	 Controls.MultiPull:ClearEntries();

	Controls.MultiPull:RegisterSelectionCallback(function(id)
		local entry = additionalEntries[id];
		if(entry and entry.call ~= nil) then
			entry.call();
		end
	end);
		 
	for i,v in ipairs(additionalEntries) do
		local controlTable = {};
		Controls.MultiPull:BuildEntry( "InstanceOne", controlTable );

		controlTable.Button:SetText( v.text );
		controlTable.Button:LocalizeAndSetToolTip( v.tip );
		controlTable.Button:SetVoid1(i);
		
	end

	-- STYLE HACK
	-- The grid has a nice little footer that will overlap entries if it is not resized to be larger than everything else.
	Controls.MultiPull:CalculateInternals();
	local dropDown = Controls.MultiPull;
	local width, height = dropDown:GetGrid():GetSizeVal();
	dropDown:GetGrid():SetSizeVal(width, height+100);

end
LuaEvents.RequestRefreshAdditionalInformationDropdownEntries.Add(RefreshAdditionalInformationEntries);

function SortAdditionalInformationDropdownEntries(entries)
	table.sort(entries, function(a,b)
		return (Locale.Compare(a.text, b.text) == -1);
	end);
end
LuaEvents.AdditionalInformationDropdownSortEntries.Add(SortAdditionalInformationDropdownEntries);

-------------------------------------------------
-------------------------------------------------
function OnCultureOverview()
    local popupInfo = {
        Type = ButtonPopupTypes.BUTTONPOPUP_CULTURE_OVERVIEW,
    }
    Events.SerialEventGameMessagePopup(popupInfo);

end
Controls.CultureOverviewButton:RegisterCallback( Mouse.eLClick, OnCultureOverview );

function OnEspionageButton()
	Events.SerialEventGameMessagePopup{ 
		Type = ButtonPopupTypes.BUTTONPOPUP_ESPIONAGE_OVERVIEW,
	};
end
Controls.EspionageButton:RegisterCallback(Mouse.eLClick, OnEspionageButton);

-------------------------------------------------
-- On ChatToggle
-------------------------------------------------
function OnChatToggle()

    m_bChatOpen = not m_bChatOpen;

    if( m_bChatOpen ) then
        Controls.ChatPanel:SetHide( false );
        Controls.ChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOn.dds" );
        Controls.HLChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOffHL.dds" );
        Controls.MOChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOff.dds" );
    else
        Controls.ChatPanel:SetHide( true );
        Controls.ChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOff.dds" );
        Controls.HLChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOnHL.dds" );
        Controls.MOChatToggle:SetTexture( "assets/UI/Art/Icons/MainChatOn.dds" );
    end

    LuaEvents.ChatShow( m_bChatOpen );
end
Controls.ChatToggle:RegisterCallback( Mouse.eLClick, OnChatToggle );


-------------------------------------------------
-- NEW: bSkipSound, show message timestamp on hover
-------------------------------------------------
local bFlipper = false;
function OnChat( fromPlayer, toPlayer, text, eTargetType, bSkipSound, iTurn, iTimestamp )

    local controlTable = {};
    ContextPtr:BuildInstanceForControl( "ChatEntry", controlTable, Controls.ChatStack );
  
    table.insert( g_ChatInstances, controlTable );
    if( #g_ChatInstances > 100 ) then
        Controls.ChatStack:ReleaseChild( g_ChatInstances[ 1 ].Box );
        table.remove( g_ChatInstances, 1 );
    end
    
    TruncateString( controlTable.String, 200, Players[fromPlayer]:GetNickName() );
    local fromName = controlTable.String:GetText();
    
    if( eTargetType == ChatTargetTypes.CHATTARGET_TEAM ) then
        controlTable.String:SetColorByName( "Green_Chat" );
        controlTable.String:SetText( fromName .. ": " .. text ); 
        
    elseif( eTargetType == ChatTargetTypes.CHATTARGET_PLAYER ) then
    
        local toName;
        if( toPlayer == g_iLocalPlayer ) then
            toName = Locale.ConvertTextKey( "TXT_KEY_YOU" );
        else
            TruncateString( controlTable.String, 200, Players[toPlayer]:GetNickName() );
            toName = Locale.ConvertTextKey( "TXT_KEY_DIPLO_TO_PLAYER", controlTable.String:GetText() );
        end
        controlTable.String:SetText( fromName .. " (" .. toName .. "): " .. text ); 
        controlTable.String:SetColorByName( "Magenta_Chat" );
        
    elseif( fromPlayer == g_iLocalPlayer ) then
        controlTable.String:SetColorByName( "Gray_Chat" );
        
        controlTable.String:SetText( fromName .. ": " .. text ); 
    else
        controlTable.String:SetText( fromName .. ": " .. text ); 
    end
      
    controlTable.Box:SetSizeY( controlTable.String:GetSizeY() + 8 );
    controlTable.Box:ReprocessAnchoring();

    if( bFlipper ) then
        controlTable.Box:SetColorChannel( 3, 0.4 );
    end
    bFlipper = not bFlipper;
    
    if ( bSkipSound ~= true ) then
        Events.AudioPlay2DSound( "AS2D_IF_MP_CHAT_DING" );
    end 

    -- set tooltip with message timestamp info, either historical or generated ATM
    if ( iTurn == nil or iTimestamp == nil ) then
        iTurn = Game.GetElapsedGameTurns();
        iTimestamp = Game.GetTurnTimeElapsed();
    end
    local strToolTip = string.format("T%i, %is", iTurn, iTimestamp / 1000);
    controlTable.Box:SetToolTipString(strToolTip);

    Controls.ChatStack:CalculateSize();
    Controls.ChatScroll:CalculateInternalSize();
    Controls.ChatScroll:SetScrollValue( 1 );
end
Events.GameMessageChat.Add( OnChat );


-------------------------------------------------
-- NEW: store chat messages
-------------------------------------------------
function SendChat( text )
    if( string.len( text ) > 0 ) then
        local iTarget = ChatTargetTypes.NO_CHATTARGET;
        local iToPlayerOrTeam = -1;
        if (g_iChatTeam ~= -1) then
            iTarget = ChatTargetTypes.CHATTARGET_TEAM;
            iToPlayerOrTeam = g_iChatTeam;
        elseif (g_iChatPlayer ~= -1) then
            iTarget = ChatTargetTypes.CHATTARGET_PLAYER;
            iToPlayerOrTeam = g_iChatPlayer;
        else
            iTarget = ChatTargetTypes.CHATTARGET_ALL;
        end
        Network.SendEnhanceReligion(Game.GetActivePlayer(), -1, text:sub(1,127), iTarget, iToPlayerOrTeam, -1, -1);
        Network.SendChat( text, g_iChatTeam, g_iChatPlayer );
    end
    Controls.ChatEntry:ClearString();
end
Controls.ChatEntry:RegisterCallback( SendChat );

function onDummyFocus(text, control, focus)
    ContextPtr:SetUpdate(KillDummy)
end
local m_dummy = InstanceManager:new( "DummyInstance", "Root", Controls.DummyStack );
g_ins = m_dummy:GetInstance();
g_ins.Dummy:RegisterCallback(onDummyFocus);
function KillDummy()
    ContextPtr:ClearUpdate()
    Controls.DummyStack:DestroyAllChildren();
    OnChatToggle();
    ContextPtr:SetUpdate(BuildDummy)
end
function BuildDummy()
    ContextPtr:ClearUpdate()
    g_ins = m_dummy:GetInstance();
    g_ins.Dummy:RegisterCallback(onDummyFocus);
end
function ChatFocus()
    ContextPtr:ClearUpdate()
    Controls.ChatEntry:TakeFocus();
end

-------------------------------------------------
-------------------------------------------------
function ShowHideInviteButton()
	local bShow = PreGame.IsInternetGame();
	Controls.MPInvite:SetHide( not bShow );
end

-------------------------------------------------
-- On MPInvite
-------------------------------------------------
function OnMPInvite()
    Steam.ActivateInviteOverlay();	
end
Controls.MPInvite:RegisterCallback( Mouse.eLClick, OnMPInvite );

----------------------------------------------------------------
----------------------------------------------------------------
function OnPlayerDisconnect( playerID )
    if( ContextPtr:IsHidden() == false ) then
    	ShowHideInviteButton();
	end
end
Events.MultiplayerGamePlayerDisconnected.Add( OnPlayerDisconnect );

-------------------------------------------------
-------------------------------------------------
function ShowHideHandler( bIsHide )
    Controls.CornerAnchor:SetHide( false );
    
    if(not bIsHide) then
		ShowHideInviteButton();
		LuaEvents.RequestRefreshAdditionalInformationDropdownEntries();
    end
end
ContextPtr:SetShowHideHandler( ShowHideHandler );

-------------------------------------------------
-------------------------------------------------
-- NEW: replace input handles with configurable variables
function InputHandler( uiMsg, wParam, lParam )
    -- NEW: update GameInfoActions now?
    if g_needsUpdate == true then
        Game.UpdateActions();
        g_needsUpdate = false;
    end
    if ( uiMsg == KeyEvents.KeyDown and wParam == g_KeyMap[GameInfoActions[ToggleChat].HotKeyVal] ) then
        if( m_bChatOpen and not g_bWaitForKeyUp ) then
            g_bWaitForKeyUp = true;
            ContextPtr:SetUpdate(ChatFocus);
        -- show chat if hidden
        elseif not g_bWaitForKeyUp then
            g_bWaitForKeyUp = true;
            OnChatToggle();
        end
        return true;
    end
    if ( uiMsg == KeyEvents.KeyUp and wParam == g_KeyMap[GameInfoActions[ToggleChat].HotKeyVal] ) then
        g_bWaitForKeyUp = false;
    end
end
ContextPtr:SetInputHandler( InputHandler );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function OnSocialPoliciesClicked()
    Events.SerialEventGameMessagePopup( { Type = ButtonPopupTypes.BUTTONPOPUP_CHOOSEPOLICY } );
end
Controls.SocialPoliciesButton:RegisterCallback( Mouse.eLClick, OnSocialPoliciesClicked );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function OnDiploClicked()
    Controls.DiploList:SetHide( not Controls.DiploList:IsHidden() );
    -- Events.SerialEventGameMessagePopup( { Type = ButtonPopupTypes.BUTTONPOPUP_DIPLOMACY } );
end
Controls.DiploButton:RegisterCallback( Mouse.eLClick, OnDiploClicked );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function OnOpenPlayerDealScreen( iOtherPlayer )
    local iUs = Game.GetActivePlayer();
    local iUsTeam = Players[ iUs ]:GetTeam();
    local pUsTeam = Teams[ iUsTeam ];

    -- any time we're legitimately opening the pvp deal screen, make sure we hide the diplolist.
    local iOtherTeam = Players[iOtherPlayer]:GetTeam();
    local iProposalTo = UI.HasMadeProposal( iUs );
   
	  -- this logic should match OnOpenPlayerDealScreen in TradeLogic.lua, DiploCorner.lua, and DiploList.lua  
    if( (pUsTeam:IsAtWar( iOtherTeam ) and (g_bAlwaysWar or g_bNoChangeWar) ) or																					 -- Always at War
	    (iProposalTo ~= -1 and iProposalTo ~= iOtherPlayer and not UI.ProposedDealExists(iOtherPlayer, iUs)) ) then -- Only allow one proposal from us at a time.
	    -- do nothing
	    return;
    else
        Controls.CornerAnchor:SetHide( true );
    end

end
Events.OpenPlayerDealScreenEvent.Add( OnOpenPlayerDealScreen );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function OnChatTarget( iTeam, iPlayer )
    g_iChatTeam = iTeam;
    g_iChatPlayer = iPlayer;

    if( iTeam ~= -1 ) then
		TruncateString( Controls.LengthTest, Controls.ChatPull:GetSizeX(), Locale.ConvertTextKey("TXT_KEY_DIPLO_TO_TEAM"));
        Controls.ChatPull:GetButton():SetText( Controls.LengthTest:GetText() );
    else
        if( iPlayer ~= -1 ) then
			TruncateString( Controls.LengthTest, Controls.ChatPull:GetSizeX(), Locale.ConvertTextKey("TXT_KEY_DIPLO_TO_PLAYER", Players[ iPlayer ]:GetNickName()));
            Controls.ChatPull:GetButton():SetText( Controls.LengthTest:GetText() );
        else
            Controls.ChatPull:GetButton():LocalizeAndSetText( "TXT_KEY_DIPLO_TO_ALL" );
        end
    end
end
Controls.ChatPull:RegisterSelectionCallback( OnChatTarget );


-------------------------------------------------------
-------------------------------------------------------
function PopulateChatPull()

    Controls.ChatPull:ClearEntries();

    -------------------------------------------------------
    -- Add All Entry
    local controlTable = {};
    Controls.ChatPull:BuildEntry( "InstanceOne", controlTable );
    controlTable.Button:SetVoids( -1, -1 );
    local textControl = controlTable.Button:GetTextControl();
    textControl:LocalizeAndSetText( "TXT_KEY_DIPLO_TO_ALL" );


    -------------------------------------------------------
    -- See if Team has more than 1 other human member
    local iTeamCount = 0;
    for iPlayer = 0, GameDefines.MAX_PLAYERS do
        local pPlayer = Players[iPlayer];

        if( iPlayer ~= g_iLocalPlayer and pPlayer ~= nil and pPlayer:IsHuman() and pPlayer:GetTeam() == g_iLocalTeam ) then
            iTeamCount = iTeamCount + 1;
        end
    end

    if( iTeamCount > 0 ) then
        local controlTable = {};
        Controls.ChatPull:BuildEntry( "InstanceOne", controlTable );
        controlTable.Button:SetVoids( g_iLocalTeam, -1 );
        local textControl = controlTable.Button:GetTextControl();
        textControl:LocalizeAndSetText( "TXT_KEY_DIPLO_TO_TEAM" );
    end


    -------------------------------------------------------
    -- Humans
    for iPlayer = 0, GameDefines.MAX_PLAYERS do
        local pPlayer = Players[iPlayer];

        if( iPlayer ~= g_iLocalPlayer and pPlayer ~= nil and pPlayer:IsHuman() ) then

            controlTable = {};
            Controls.ChatPull:BuildEntry( "InstanceOne", controlTable );
            controlTable.Button:SetVoids( -1, iPlayer );
            textControl = controlTable.Button:GetTextControl();
			TruncateString( textControl, Controls.ChatPull:GetSizeX()-20, Locale.ConvertTextKey("TXT_KEY_DIPLO_TO_PLAYER", pPlayer:GetNickName()));
        end
    end
    
    Controls.ChatPull:GetButton():LocalizeAndSetText( "TXT_KEY_DIPLO_TO_ALL" );
    Controls.ChatPull:CalculateInternals();
end
Events.MultiplayerGamePlayerUpdated.Add(PopulateChatPull);


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function DoUpdateLeagueCountdown()
	local bHide = true;
	local sTooltip = Locale.ConvertTextKey("TXT_KEY_EO_DIPLOMACY");
				
	if (Game.GetNumActiveLeagues() > 0) then
		local pLeague = Game.GetActiveLeague();
		if (pLeague ~= nil) then
			local iCountdown = pLeague:GetTurnsUntilSession();
			if (iCountdown ~= 0 and not pLeague:IsInSession()) then
				bHide = false;
				if (PreGame.IsVictory(GameInfo.Victories["VICTORY_DIPLOMATIC"].ID) and Game.IsUnitedNationsActive() and Game.GetGameState() == GameplayGameStateTypes.GAMESTATE_ON) then
					local iCountdownToVictorySession = pLeague:GetTurnsUntilVictorySession();
					if (iCountdownToVictorySession <= iCountdown) then
						Controls.UNTurnsLabel:SetText("[COLOR_POSITIVE_TEXT]" .. iCountdownToVictorySession .. "[ENDCOLOR]");
					else
						Controls.UNTurnsLabel:SetText(iCountdown);
					end
					sTooltip = Locale.ConvertTextKey("TXT_KEY_EO_DIPLOMACY_AND_VICTORY_SESSION", iCountdown, iCountdownToVictorySession);
				else
					Controls.UNTurnsLabel:SetText(iCountdown);
					sTooltip = Locale.ConvertTextKey("TXT_KEY_EO_DIPLOMACY_AND_LEAGUE_SESSION", iCountdown);
				end
			end
		end
	end
	
	Controls.UNTurnsLabel:SetHide(bHide);
	Controls.DiploButton:SetToolTipString(sTooltip);
end
Events.SerialEventGameDataDirty.Add(DoUpdateLeagueCountdown);

-- Also call it once so it starts correct - surprisingly enough, GameData isn't dirtied as we're loading a game
DoUpdateLeagueCountdown();

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function DoUpdateEspionageButton()
	local iLocalPlayer = Game.GetActivePlayer();
	local pLocalPlayer = Players[iLocalPlayer];
	local iNumUnassignedSpies = pLocalPlayer:GetNumUnassignedSpies();
	
	local strToolTip = Locale.ConvertTextKey("TXT_KEY_EO_TITLE");
	
	if (iNumUnassignedSpies > 0) then
		strToolTip = strToolTip .. "[NEWLINE][NEWLINE]";
		strToolTip = strToolTip .. Locale.ConvertTextKey("TXT_KEY_EO_UNASSIGNED_SPIES_TT", iNumUnassignedSpies);
		Controls.UnassignedSpiesLabel:SetHide(false);
		Controls.UnassignedSpiesLabel:SetText(iNumUnassignedSpies);
	else
		Controls.UnassignedSpiesLabel:SetHide(true);
	end
	
	Controls.EspionageButton:SetToolTipString(strToolTip);
end
Events.SerialEventEspionageScreenDirty.Add(DoUpdateEspionageButton);

--------------------------------------------------------------------
function HandleNotificationAdded(notificationId, notificationType, toolTip, summary, gameValue, extraGameData)
	
	-- In the event we receive a new spy, make sure the large button is displayed.
	if(ContextPtr:IsHidden() == false) then
		if(notificationType == NotificationTypes.NOTIFICATION_SPY_CREATED_ACTIVE_PLAYER) then
			CheckEspionageStarted();
		end
	end
end
Events.NotificationAdded.Add(HandleNotificationAdded);

DoUpdateEspionageButton();

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
if( Game.IsGameMultiPlayer() ) then
    PopulateChatPull();

	if ( not Game.IsHotSeat() ) then
		Controls.ChatToggle:SetHide( false );
		OnChatToggle();
	end
end


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function OnEndGameButton()

    local which = math.random( 1, 6 );
    
    if( which == 1 ) then Events.EndGameShow( EndGameTypes.Technology, Game.GetActivePlayer() ); 
    elseif( which == 2 ) then Events.EndGameShow( EndGameTypes.Domination, Game.GetActivePlayer() );
    elseif( which == 3 ) then Events.EndGameShow( EndGameTypes.Culture, Game.GetActivePlayer() );
    elseif( which == 4 ) then Events.EndGameShow( EndGameTypes.Diplomatic, Game.GetActivePlayer() );
    elseif( which == 5 ) then Events.EndGameShow( EndGameTypes.Time, Game.GetActivePlayer() );
    elseif( which == 6 ) then Events.EndGameShow( EndGameTypes.Time, Game.GetActivePlayer() + 1 ); 
    end
end
Controls.EndGameButton:RegisterCallback( Mouse.eLClick, OnEndGameButton );

local g_PerPlayerState = {};
----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
function OnDiploCornerActivePlayerChanged( iActivePlayer, iPrevActivePlayer )
	-- Restore the state per player
	local bIsHidden = Controls.DiploList:IsHidden() == true;
	-- Save the state per player
	if (iPrevActivePlayer ~= -1) then
		g_PerPlayerState[ iPrevActivePlayer + 1 ] = bIsHidden;
	end
	
	if (iActivePlayer ~= -1) then
		if (g_PerPlayerState[ iActivePlayer + 1 ] == nil or g_PerPlayerState[ iActivePlayer + 1 ] == -1) then
			Controls.DiploList:SetHide( true );
		else
			local bWantHidden = g_PerPlayerState[ iActivePlayer + 1 ];
			if ( bWantHidden ~= Controls.DiploList:IsHidden()) then
				Controls.DiploList:SetHide( bWantHidden );
			end
		end
	end

	g_iLocalPlayer = Game.GetActivePlayer();
	g_pLocalPlayer = Players[ g_iLocalPlayer ];
	g_iLocalTeam = g_pLocalPlayer:GetTeam();
	g_pLocalTeam = Teams[ g_iLocalTeam ];
	PopulateChatPull();
end
Events.GameplaySetActivePlayer.Add(OnDiploCornerActivePlayerChanged);


function CheckEspionageStarted()
	function TestEspionageStarted()
		local player = Players[Game.GetActivePlayer()];
		return player:GetNumSpies() > 0;
	end

	local bEspionageStarted = TestEspionageStarted();
	Controls.CornerAnchor:SetHide(bEspionageStarted);
	Controls.CornerAnchor_Espionage:SetHide(not bEspionageStarted);
	Controls.EspionageButton:SetHide(not bEspionageStarted);
	if(bEspionageStarted) then
		DoUpdateEspionageButton();
	end
end

function OnActivePlayerTurnStart()
	CheckEspionageStarted();
	
end
Events.ActivePlayerTurnStart.Add(OnActivePlayerTurnStart);


OnActivePlayerTurnStart();

--Hide CultureOverview, if disabled.
if(Game.IsOption("GAMEOPTION_NO_CULTURE_OVERVIEW_UI")) then
	Controls.CultureOverviewButton:SetHide(true);
end

-- NEW: disable chatpull in tournament mode
if PreGame.GetGameOption("GAMEOPTION_DISABLE_CHAT") > 0 then
    Controls.ChatPull:SetDisabled(true);
end

-- NEW: add saved chat messages on load
function LoadChatMessages()
    local messages = Game.GetReplayMessages();
    local iLocalPlayer = Game.GetActivePlayer();
    local iLocalTeam = Players[iLocalPlayer]:GetTeam();
    for i,message in ipairs(messages) do
        if (message.Type == 7) then -- chat message
            print(message.Player, message.Data1, message.Data2, message.Text);
            local eTarget = message.Data1;
            local toPlayer = message.Data2;
            if ( not (eTarget == ChatTargetTypes.CHATTARGET_PLAYER and message.Player ~= iLocalPlayer and toPlayer ~= iLocalPlayer or eTarget == ChatTargetTypes.CHATTARGET_TEAM and Players[message.Player]:GetTeam() ~= iLocalTeam)) then
                OnChat( message.Player, message.Data2, message.Text, message.Data1, true, message.Turn, message.Timestamp )
            end
        end
    end
end

-- NEW: populate Emote Picker
Controls.EmotePickerButton:RegisterCallback( Mouse.eLClick, function() Controls.EmotePicker:SetHide(not Controls.EmotePicker:IsHidden()) end );
for i in GameInfo.IconFontMapping('1 ORDER BY LENGTH(IconFontTexture), rowid') do
    local ins = {};
    ContextPtr:BuildInstanceForControl('EmoteInstance', ins, Controls.EmoteStack);
    local emoteText = string.format('[%s]', i.IconName);
    ins.EmoteButton:SetText(emoteText);
    ins.EmoteButton:RegisterCallback( Mouse.eLClick, function() Controls.ChatEntry:TakeFocus(); Controls.ChatEntry:SetText(Controls.ChatEntry:GetText() .. emoteText); return true; end );
end
Controls.EmotesScrollPanel:ReprocessAnchoring()
Controls.EmotesScrollPanel:CalculateInternalSize()

LoadChatMessages();
