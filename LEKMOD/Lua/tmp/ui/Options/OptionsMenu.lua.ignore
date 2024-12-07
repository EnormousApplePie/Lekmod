-------------------------------------------------
-- Options Menu
-------------------------------------------------
-- edit: NEW: Ingame Hotkey Manager for vanilla UI
-------------------------------------------------
include( "IconSupport" );

local EUI_options = Modding.OpenUserData( "Enhanced User Interface Options", 1);

local m_FullscreenResList = {};
local m_iResolutionCount;
local m_maxX, m_maxY = OptionsManager.GetMaxResolution();

local m_bInGameQuickCombatState_Cached = false;
local m_bInGameQuickMovementState_Cached = false;

local passwordsMatchString = Locale.ConvertTextKey( "TXT_KEY_OPSCREEN_TURN_NOTIFY_SMTP_PASSWORDS_MATCH" );
local passwordsMatchTooltipString = Locale.ConvertTextKey( "TXT_KEY_OPSCREEN_TURN_NOTIFY_SMTP_PASSWORDS_MATCH_TT" );
local passwordsNotMatchString = Locale.ConvertTextKey( "TXT_KEY_OPSCREEN_TURN_NOTIFY_SMTP_PASSWORDS_NOT_MATCH" );
local passwordsNotMatchTooltipString = Locale.ConvertTextKey( "TXT_KEY_OPSCREEN_TURN_NOTIFY_SMTP_PASSWORDS_NOT_MATCH_TT" );

----------------------------------------------------------------
----------------------------------------------------------------
local m_WindowResList = {
{ x=2560, y=2048, bWide=false },
{ x=2560, y=1600, bWide=true },
{ x=1920, y=1200, bWide=true },
{ x=1920, y=1080, bWide=true },
{ x=1680, y=1050, bWide=true },
{ x=1600, y=1200, bWide=false },
{ x=1440, y=900,  bWide=true  },
{ x=1400, y=1050, bWide=true  },
{ x=1366, y=768,  bWide=true },
{ x=1280, y=1024, bWide=false },
{ x=1280, y=960,  bWide=true  },
{ x=1280, y=800,  bWide=true  },
{ x=1024, y=768,  bWide=false }, 
};

-- Store array of supported languages.
g_Languages = Locale.GetSupportedLanguages();
g_SpokenLanguages = Locale.GetSupportedSpokenLanguages();

-------------------------------------------------
-------------------------------------------------
local iMusicVolumeKnobID		= GetVolumeKnobIDFromName("USER_VOLUME_MUSIC");
local iSFXVolumeKnobID		= GetVolumeKnobIDFromName("USER_VOLUME_SFX");
local iSpeechVolumeKnobID		= GetVolumeKnobIDFromName("USER_VOLUME_SPEECH");
local iAmbienceVolumeKnobID	= GetVolumeKnobIDFromName("USER_VOLUME_AMBIENCE");

local m_cachedMusicVolumeKnob = nil;
local m_cachedSFXVolumeKnob = nil;
local m_cachedSpeechVolumeKnob = nil;
local m_cachedAmbienceVolumeKnob = nil;

function CacheAudioVolumes()
m_cachedMusicVolumeKnob = GetVolumeKnobValue(iMusicVolumeKnobID);
m_cachedSFXVolumeKnob = GetVolumeKnobValue(iSFXVolumeKnobID);
m_cachedSpeechVolumeKnob = GetVolumeKnobValue(iSpeechVolumeKnobID);
m_cachedAmbienceVolumeKnob = GetVolumeKnobValue(iAmbienceVolumeKnobID);
end

-------------------------------------------------
-------------------------------------------------
function RevertToCachedAudioVolumes()
	if( m_cachedMusicVolumeKnob ~= nil and m_cachedSFXVolumeKnob ~= nil and m_cachedSpeechVolumeKnob ~= nil 
	        and m_cachedAmbienceVolumeKnob ~= nil) then
		SetVolumeKnobValue(iMusicVolumeKnobID, m_cachedMusicVolumeKnob);
		SetVolumeKnobValue(iSFXVolumeKnobID, m_cachedSFXVolumeKnob);
		SetVolumeKnobValue(iSpeechVolumeKnobID, m_cachedSpeechVolumeKnob);
		SetVolumeKnobValue(iAmbienceVolumeKnobID, m_cachedAmbienceVolumeKnob);
	end
end

-------------------------------------------------
-------------------------------------------------
--function SetDefaultGameTabOptions()
--	OptionsManager.SetNoRewardPopups(false);
--	OptionsManager.SetNoTileRecommendations(false);
--	OptionsManager.SetCivilianYields(false);
--	OptionsManager.SetNoBasicHelp(false);
--	m_cachedSinglePlayerEndTurnTimerEnabled = false;
--	m_cachedMultiPlayerEndTurnTimerEnabled = true;
--	OptionsManager.SetTooltip1Seconds( 150 );
--	OptionsManager.SetTooltip2Seconds( 500 );
--	OptionsManager.SetTutorialLevel(0);
--end

-------------------------------------------------
-------------------------------------------------
function SetDefaultAudioVolumes()
	SetVolumeKnobValue(iMusicVolumeKnobID, 1);
	SetVolumeKnobValue(iSFXVolumeKnobID, 1);
	SetVolumeKnobValue(iSpeechVolumeKnobID, 1);
	SetVolumeKnobValue(iAmbienceVolumeKnobID, 1);
end

-------------------------------------------------
-------------------------------------------------
function OnOK()
    EUI_options.SetValue( "DB_bRightButtonRebase", Controls.RightButtonRebaseCheckbox:IsChecked() )

	OptionsManager.CommitGameOptions();
	OptionsManager.CommitGraphicsOptions();
    SaveAudioOptions();
    
    local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
    
    if (bIsInGame) then
		local bCanCommit = false;
		if (PreGame.IsMultiplayerGame()) then
			if (Matchmaking.IsHost()) then
				-- If we are the host of a game, we can change the quick states
				bCanCommit = true;
			end
		else
			bCanCommit = true;
		end
		
		if (bCanCommit) then
			local options = {};
			if (m_bInGameQuickCombatState_Cached ~= PreGame.GetQuickCombat()) then
				table.insert(options, { "GAMEOPTION_QUICK_COMBAT", m_bInGameQuickCombatState_Cached });
			end
			
			if (m_bInGameQuickMovementState_Cached ~= PreGame.GetQuickMovement()) then
				table.insert(options, { "GAMEOPTION_QUICK_MOVEMENT", m_bInGameQuickMovementState_Cached });
			end
			
			Network.SendGameOptions(options);				
		end
	end
				
    --update local caches because the hide handler will set values back to cached versions
    CacheAudioVolumes();
    
    local bResolutionChanged = OptionsManager.HasUserChangedResolution();
    local bGraphicsChanged = OptionsManager.HasUserChangedGraphicsOptions();
    
    if( bResolutionChanged ) then
		OnApplyRes();
	end
	
	if( bGraphicsChanged ) then
		Controls.GraphicsChangedPopup:SetHide(false);
    end
    
    if( (not bGraphicsChanged) and (not bResolutionChanged) ) then
		OnBack();
	end
end
Controls.AcceptButton:RegisterCallback( Mouse.eLClick, OnOK );

-------------------------------------------------
-------------------------------------------------
function OnCancel()
	OptionsManager.SyncGameOptionsCache();
	OptionsManager.SyncGraphicsOptionsCache();
	OptionsManager.SyncResolutionOptionsCache();
	RevertToCachedAudioVolumes();
	OnBack();
end
Controls.CancelButton:RegisterCallback( Mouse.eLClick, OnCancel );

-------------------------------------------------
-------------------------------------------------
function OnDefault()
	OptionsManager.ResetDefaultGameOptions();
	OptionsManager.ResetDefaultGraphicsOptions();
	SetDefaultAudioVolumes();
	UpdateOptionsDisplay();
end
Controls.DefaultButton:RegisterCallback( Mouse.eLClick, OnDefault );

-------------------------------------------------
-------------------------------------------------
local fullscreenRes;
local windowedRes = {0,0};
local msaaSetting;
local bFullscreen;

function SavePreviousResolutionSettings()
	fullscreenRes = OptionsManager.GetResolution_Cached();
	windowedRes[1], windowedRes[2] = OptionsManager.GetWindowResolution_Cached();
	msaaSetting = OptionsManager.GetAASamples_Cached();
	bFullscreen = OptionsManager.GetFullscreen_Cached();
end

-------------------------------------------------
-------------------------------------------------
function RestorePreviousResolutionSettings()
	OptionsManager.SetResolution_Cached(fullscreenRes);
	OptionsManager.SetWindowResolution_Cached(windowedRes[1], windowedRes[2]);
	OptionsManager.SetAASamples_Cached(msaaSetting);
	OptionsManager.SetFullscreen_Cached(bFullscreen);
	if OptionsManager.HasUserChangedResolution() then
		OptionsManager.CommitResolutionOptions();
	end
	UpdateGraphicsOptionsDisplay();
end

-------------------------------------------------
-- If we hear a multiplayer game invite was sent, exit
-- so we don't interfere with the transition
-------------------------------------------------
function OnMultiplayerGameInvite()
   	if( ContextPtr:IsHidden() == false ) then
		OnCancel();
	end
end

Events.MultiplayerGameLobbyInvite.Add( OnMultiplayerGameInvite );
Events.MultiplayerGameServerInvite.Add( OnMultiplayerGameInvite );

-------------------------------------------------
-------------------------------------------------
local g_fTimer;
function OnUpdate( fDTime )
	Controls.CountdownTimer:SetText( Locale.ConvertTextKey("20") );
	
	g_fTimer = g_fTimer - fDTime;
	if( g_fTimer <= 0 ) then
	    ContextPtr:ClearUpdate();
        OnCountdownNo();
    else
    	Controls.CountdownTimer:SetText( string.format( "%i", g_fTimer + 1 ) );
	end
end
	
	
-------------------------------------------------
-------------------------------------------------
g_bIsResolutionCountdown = true;
function ShowResolutionCountdown()
	g_fTimer = 20;
	ContextPtr:SetUpdate( OnUpdate );
	Controls.Countdown:SetHide(false);
	Controls.CountdownMessage:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_RESOLUTION_TIMER") );
	Controls.CountYes:SetText( Locale.ConvertTextKey("TXT_KEY_YES_BUTTON") );
	Controls.CountNo:SetText( Locale.ConvertTextKey("TXT_KEY_NO_BUTTON") );
	Controls.CountdownTimer:SetText( "20" );
	Controls.LabelStack:CalculateSize();
	Controls.LabelStack:ReprocessAnchoring();
	g_bIsResolutionCountdown = true;
end

-------------------------------------------------
-------------------------------------------------
g_chosenLanguage = 0;
function ShowLanguageCountdown()
	g_fTimer = 20;
	ContextPtr:SetUpdate( OnUpdate );
	Controls.Countdown:SetHide(false);
	Controls.CountdownMessage:SetText( Locale.LookupLanguage(g_Languages[g_chosenLanguage].Type, "TXT_KEY_OPSCREEN_LANGUAGE_TIMER") );
	
	local YesText = string.format( "%s (%s)",
		Locale.LookupLanguage( g_Languages[g_chosenLanguage].Type, "TXT_KEY_YES_BUTTON"), 
		Locale.ConvertTextKey( "TXT_KEY_YES_BUTTON" ) );
	Controls.CountYes:SetText( YesText );
	
	local NoText = string.format( "%s (%s)",
		Locale.LookupLanguage( g_Languages[g_chosenLanguage].Type, "TXT_KEY_NO_BUTTON"), 
		Locale.ConvertTextKey( "TXT_KEY_NO_BUTTON" ) );
	Controls.CountNo:SetText( NoText );
	
	Controls.CountdownTimer:SetText( "20" );
	Controls.LabelStack:CalculateSize();
	Controls.LabelStack:ReprocessAnchoring();
	g_bIsResolutionCountdown = false;
end

-------------------------------------------------
-------------------------------------------------
function OnApplyRes()
	if( OptionsManager.HasUserChangedResolution() ) then
		ShowResolutionCountdown()
		OptionsManager.CommitResolutionOptions();
		UpdateGraphicsOptionsDisplay();
	end
end
Controls.ApplyResButton:RegisterCallback( Mouse.eLClick, OnApplyRes );

-------------------------------------------------
-------------------------------------------------
function OnResetTutorial()
	OptionsManager.ResetTutorial();
end
Controls.ResetTutorialButton:RegisterCallback( Mouse.eLClick, OnResetTutorial);

-------------------------------------------------
-------------------------------------------------
function OnOptionsEvent()
    UIManager:QueuePopup( ContextPtr, PopupPriority.OptionsMenu );
end

-------------------------------------------------
-------------------------------------------------
function ShowHideHandler( bIsHide, bIsInit )
    local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame"); --(UI.GetCurrentGameState() == GameStateTypes.CIV5_GS_MAINGAMEVIEW);
    
    if( bIsInit and bIsInGame ) then
        Events.EventOpenOptionsScreen.Add( OnOptionsEvent );
    end
    
    if( not bIsHide ) then
		RefreshTutorialLevelOptions();
        --options menu is being shown
		OptionsManager.SyncGameOptionsCache();
		OptionsManager.SyncGraphicsOptionsCache();
		OptionsManager.SyncResolutionOptionsCache();
		
		if (bIsInGame) then 
			m_bInGameQuickCombatState_Cached = PreGame.GetQuickCombat();
			m_bInGameQuickMovementState_Cached = PreGame.GetQuickMovement();
		end
		
		CacheAudioVolumes();
		SavePreviousResolutionSettings();
        UpdateOptionsDisplay();
        
        if( bIsInGame and (PreGame.IsMultiplayerGame() or PreGame.IsHotSeatGame()) )then
            Controls.TutorialPull:SetDisabled( true );
            Controls.TutorialPull:SetAlpha( 0.5 );
            Controls.ResetTutorialButton:SetDisabled(true);
            Controls.ResetTutorialButton:SetAlpha( 0.5 );
        else
            Controls.TutorialPull:SetDisabled( false );
            Controls.TutorialPull:SetAlpha( 1.0 );
            Controls.ResetTutorialButton:SetDisabled(false);
            Controls.ResetTutorialButton:SetAlpha( 1.0 );
        end
        
        if( bIsInGame and PreGame.IsMultiplayerGame() and not Matchmaking.IsHost()) then
			-- Not this host, disable
			Controls.MPQuickCombatCheckbox:SetDisabled( true );
			Controls.MPQuickCombatCheckbox:SetAlpha( 0.5 );
			Controls.MPQuickMovementCheckbox:SetDisabled(true);
			Controls.MPQuickMovementCheckbox:SetAlpha( 0.5 );
		else
			Controls.MPQuickCombatCheckbox:SetDisabled( false );
			Controls.MPQuickCombatCheckbox:SetAlpha( 1.0 );
			Controls.MPQuickMovementCheckbox:SetDisabled(false);
			Controls.MPQuickMovementCheckbox:SetAlpha( 1.0 );
		end			        
        
        if( bIsInGame )then
            Controls.BGBlock:SetHide( false );
            Controls.VideoPanelBlock:SetHide( false );
            Controls.LanguagePull:SetDisabled( true );
            Controls.LanguagePull:SetAlpha( 0.5 );
            Controls.SpokenLanguagePull:SetDisabled(true);
            Controls.SpokenLanguagePull:SetAlpha( 0.5 );
            Controls.AutoUIAssetsCheck:SetDisabled( true );
            Controls.AutoUIAssetsCheck:SetAlpha( 0.5 );
            Controls.SmallUIAssetsCheck:SetDisabled( true );
            Controls.SmallUIAssetsCheck:SetAlpha( 0.5 );
        else
            Controls.VideoPanelBlock:SetHide( true );
            Controls.BGBlock:SetHide( true );
        end
    else
        --options menu is being hidden
	    RevertToCachedAudioVolumes();
        -- NEW: Cancel hotkey editbox selection
        if next(g_CurrentEditable) ~= nil then
            OnCancelEditClick(g_CurrentEditable.HotkeyEditCancelButton:GetVoid1(), g_CurrentEditable.HotkeyEditCancelButton:GetVoid2());
        end
	end
end
ContextPtr:SetShowHideHandler( ShowHideHandler );

-------------------------------------------------
-------------------------------------------------
-- NEW: also check ConfirmResetAllHotkeysPopup
function OnBack()
	if( Controls.GraphicsChangedPopup:IsHidden() and
		Controls.Countdown:IsHidden() and
        Controls.ConfirmResetAllHotkeysPopup:IsHidden()) then	
		UIManager:DequeuePopup( ContextPtr );
	end
	
	Controls.GraphicsChangedPopup:SetHide(true);
    Controls.ConfirmResetAllHotkeysPopup:SetHide(true);
	OnCountdownNo();	
end
--Controls.CancelButton:RegisterCallback( Mouse.eLClick, OnBack );


-------------------------------------------------
-------------------------------------------------
function OnCategory( which )
    Controls.GamePanel:SetHide(  which ~= 1 );
    Controls.IFacePanel:SetHide( which ~= 2 );
    Controls.VideoPanel:SetHide( which ~= 3 );
    Controls.AudioPanel:SetHide( which ~= 4 );
    Controls.MultiplayerPanel:SetHide( which ~= 5 );
    -- NEW
    Controls.HotkeysPanel:SetHide( which ~= 6 );
    
    Controls.GameHighlight:SetHide(  which ~= 1 );
    Controls.IFaceHighlight:SetHide( which ~= 2 );
    Controls.VideoHighlight:SetHide( which ~= 3 );
    Controls.AudioHighlight:SetHide( which ~= 4 );
    Controls.MultiplayerHighlight:SetHide( which ~= 5 ); 
    -- NEW
    Controls.HotkeyManagerHighlight:SetHide( which ~= 6 );
    
    Controls.TitleLabel:SetText( m_PanelNames[ which ] );
end
Controls.GameButton:RegisterCallback(  Mouse.eLClick, OnCategory );
Controls.IFaceButton:RegisterCallback( Mouse.eLClick, OnCategory );
Controls.VideoButton:RegisterCallback( Mouse.eLClick, OnCategory );
Controls.AudioButton:RegisterCallback( Mouse.eLClick, OnCategory );
Controls.MultiplayerButton:RegisterCallback( Mouse.eLClick, OnCategory );
-- NEW
Controls.HotkeyManagerButton:RegisterCallback( Mouse.eLClick, OnCategory );

Controls.GameButton:SetVoid1(  1 );
Controls.IFaceButton:SetVoid1( 2 );
Controls.VideoButton:SetVoid1( 3 );
Controls.AudioButton:SetVoid1( 4 );
Controls.MultiplayerButton:SetVoid1( 5 );
-- NEW
Controls.HotkeyManagerButton:SetVoid1( 6 );

m_PanelNames = {};
m_PanelNames[ 1 ] = Locale.ConvertTextKey( "TXT_KEY_GAME_OPTIONS" );
m_PanelNames[ 2 ] = Locale.ConvertTextKey( "TXT_KEY_INTERFACE_OPTIONS" );
m_PanelNames[ 3 ] = Locale.ConvertTextKey( "TXT_KEY_VIDEO_OPTIONS" );
m_PanelNames[ 4 ] = Locale.ConvertTextKey( "TXT_KEY_AUDIO_OPTIONS" );
m_PanelNames[ 5 ] = Locale.ConvertTextKey( "TXT_KEY_MULTIPLAYER_OPTIONS" );
    -- NEW
m_PanelNames[ 6 ] = Locale.ConvertTextKey( "TXT_KEY_HOTKEY_OPTIONS" );

OnCategory( 1 );


-------------------------------------------------
-- Volume control
-------------------------------------------------


function OnGameChangedVolumeLevel( iVolumeKnobID, fNewVolume )

	-- NOTE: testing controls by name is more hardcoded than we'd like
	-- it would be better to put the slider controls in an array and then test their void1's.
	
	-- NOTE: we may get messages for volume controls that are not user volume controls, just ignore them
	
	fRealVolume = GetVolumeKnobValue(iVolumeKnobID);
	
	local newVolume = Locale.ToPercent(fNewVolume);
	if Controls.MusicVolumeSlider:GetVoid1() == iVolumeKnobID then
		Controls.MusicVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MUSIC_SLIDER", newVolume ) );
	elseif Controls.EffectsVolumeSlider:GetVoid1() == iVolumeKnobID then
		Controls.EffectsVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SF_SLIDER", newVolume ) );
	elseif Controls.SpeechVolumeSlider:GetVoid1() == iVolumeKnobID then
		Controls.SpeechVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SPEECH_SLIDER", newVolume ) );
	elseif Controls.AmbienceVolumeSlider:GetVoid1() == iVolumeKnobID then
		Controls.AmbienceVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_AMBIANCE_SLIDER", newVolume ) );
	end
end
--Events.AudioVolumeChanged.Add( OnGameChangedVolumeLevel );

function OnUIVolumeSliderValueChanged( fNewVolumeLevel, void1 )
    SetVolumeKnobValue(void1, fNewVolumeLevel);
    
    local newVolume = Locale.ToPercent(fNewVolumeLevel);
	if Controls.MusicVolumeSlider:GetVoid1() == void1 then
		Controls.MusicVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MUSIC_SLIDER", newVolume ) );
	elseif Controls.EffectsVolumeSlider:GetVoid1() == void1 then
		Controls.EffectsVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SF_SLIDER", newVolume ) );
	elseif Controls.SpeechVolumeSlider:GetVoid1() == void1 then
		Controls.SpeechVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SPEECH_SLIDER", newVolume ) );
	elseif Controls.AmbienceVolumeSlider:GetVoid1() == void1 then
		Controls.AmbienceVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_AMBIANCE_SLIDER", newVolume ) );
	end
end

Controls.MusicVolumeSlider:RegisterSliderCallback(		OnUIVolumeSliderValueChanged );
Controls.EffectsVolumeSlider:RegisterSliderCallback(	OnUIVolumeSliderValueChanged );
Controls.SpeechVolumeSlider:RegisterSliderCallback(		OnUIVolumeSliderValueChanged );
Controls.AmbienceVolumeSlider:RegisterSliderCallback(	OnUIVolumeSliderValueChanged );

Controls.MusicVolumeSlider:SetVoid1(	iMusicVolumeKnobID );
Controls.EffectsVolumeSlider:SetVoid1(	iSFXVolumeKnobID );
Controls.SpeechVolumeSlider:SetVoid1(	iSpeechVolumeKnobID );
Controls.AmbienceVolumeSlider:SetVoid1(	iAmbienceVolumeKnobID );

Controls.MusicVolumeSlider:SetValue(	GetVolumeKnobValue(iMusicVolumeKnobID) );
Controls.EffectsVolumeSlider:SetValue(	GetVolumeKnobValue(iSFXVolumeKnobID) );
Controls.SpeechVolumeSlider:SetValue(	GetVolumeKnobValue(iSpeechVolumeKnobID) );
Controls.AmbienceVolumeSlider:SetValue( GetVolumeKnobValue(iAmbienceVolumeKnobID) );

Controls.MusicVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MUSIC_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iMusicVolumeKnobID)) ) );
Controls.EffectsVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SF_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iSFXVolumeKnobID)) ) );
Controls.SpeechVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SPEECH_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iSpeechVolumeKnobID)) ) );
Controls.AmbienceVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_AMBIANCE_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iAmbienceVolumeKnobID)) ) );

-------------------------------------------------
-------------------------------------------------
function Tooltip1TimerSliderChanged(value)
	
	--print("value: " .. value);
	
	i = math.floor(value * 100);
	i = i * 10;
	--print("i: " .. i);
	Controls.Tooltip1TimerLength:SetText(Locale.ConvertTextKey("TXT_KEY_OPSCREEN_TOOLTIP_1_TIMER_LENGTH", math.floor(i / 100) ));
	OptionsManager.SetTooltip1Seconds_Cached(i);
end
Controls.Tooltip1TimerSlider:RegisterSliderCallback(Tooltip1TimerSliderChanged)
Controls.Tooltip1TimerSlider:SetValue(OptionsManager.GetTooltip1Seconds()/1000);


-------------------------------------------------
-------------------------------------------------
--[[
function Tooltip2TimerSliderChanged(value)
	i = math.floor(value * 100);
	i = i * 10;
	Controls.Tooltip2TimerLength:SetText(Locale.ConvertTextKey("TXT_KEY_OPSCREEN_TOOLTIP_2_TIMER_LENGTH", i / 100));
	OptionsManager.SetTooltip2Seconds_Cached(i);
end
Controls.Tooltip2TimerSlider:RegisterSliderCallback(Tooltip2TimerSliderChanged)
Controls.Tooltip2TimerSlider:SetValue(OptionsManager.GetTooltip2Seconds()/1000);
--]]


-------------------------------------------------
-- Countdown handlers
-------------------------------------------------
function OnCountdownYes()
	if g_bIsResolutionCountdown == true then
		--just hide the menu
		Controls.Countdown:SetHide(true);
		SavePreviousResolutionSettings();
	else
		--apply language, reload UI
		Locale.SetCurrentLanguage( g_Languages[g_chosenLanguage].Type );
		Events.SystemUpdateUI( SystemUpdateUIType.ReloadUI );
	end
	--turn off timer
	ContextPtr:ClearUpdate();
end
Controls.CountYes:RegisterCallback( Mouse.eLClick, OnCountdownYes );

function OnCountdownNo()
	if g_bIsResolutionCountdown == true then
		--here we revert resolution options to some old options
		Controls.Countdown:SetHide(true);
		RestorePreviousResolutionSettings();
	else
		--revert language to current setting
		Controls.Countdown:SetHide(true);
		Controls.LanguagePull:GetButton():SetText(Locale.GetCurrentLanguage().DisplayName);
	end
	--turn off timer
	ContextPtr:ClearUpdate();
end
Controls.CountNo:RegisterCallback( Mouse.eLClick, OnCountdownNo );

function OnGraphicsChangedOK()
	--close the menu
	Controls.GraphicsChangedPopup:SetHide(true);
	OnBack();
end
Controls.GraphicsChangedOK:RegisterCallback( Mouse.eLClick, OnGraphicsChangedOK );

----------------------------------------------------------------
-- Input processing
----------------------------------------------------------------
-- NEW: see updated InputHandler below
--[[function InputHandler( uiMsg, wParam, lParam )
    if uiMsg == KeyEvents.KeyDown then
        if wParam == Keys.VK_ESCAPE then
            OnBack();
            return true;
        end
    end
end
ContextPtr:SetInputHandler( InputHandler );--]]

m_LeaderText = {};
m_LeaderText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MINIMUM");
m_LeaderText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_LeaderText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_LeaderText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");

m_OverlayText = {};
m_OverlayText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_OverlayText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_OverlayText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");
    
m_ShadowText = {};
m_ShadowText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_OFF");
m_ShadowText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_ShadowText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_ShadowText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");
   
m_FOWText = {};
m_FOWText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MINIMUM");
m_FOWText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_FOWText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_FOWText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");
   
m_TerrainDetailText = {};
m_TerrainDetailText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MINIMUM");
m_TerrainDetailText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_TerrainDetailText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_TerrainDetailText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");
  
m_TerrainTessText = {};
m_TerrainTessText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_TerrainTessText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_TerrainTessText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");

m_TerrainShadowText = {};
m_TerrainShadowText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_OFF");
m_TerrainShadowText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_TerrainShadowText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_TerrainShadowText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");

m_WaterText = {};
m_WaterText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_WaterText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_MEDIUM");
m_WaterText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");

m_TextureQualityText = {};
m_TextureQualityText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_LOW");
m_TextureQualityText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SETTINGS_HIGH");

m_BindMouseText = {};
m_BindMouseText[ 0 ] = Locale.ConvertTextKey( "TXT_KEY_NEVER" );
m_BindMouseText[ 1 ] = Locale.ConvertTextKey( "TXT_KEY_FULLSCREEN_ONLY" );
m_BindMouseText[ 2 ] = Locale.ConvertTextKey( "TXT_KEY_ALWAYS" );

m_MSAAText = {};
m_MSAAText[ 0 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MSAA_OFF");
m_MSAAText[ 1 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MSAA_2X");
m_MSAAText[ 2 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MSAA_4X");
m_MSAAText[ 3 ] = Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MSAA_8X");

m_MSAAMap = {};
m_MSAAMap[ 0 ] = 1;
m_MSAAMap[ 1 ] = 2;
m_MSAAMap[ 2 ] = 4;
m_MSAAMap[ 3 ] = 8;

m_MSAAInvMap = {};
m_MSAAInvMap[ 0 ] = 0;
m_MSAAInvMap[ 1 ] = 0;
m_MSAAInvMap[ 2 ] = 1;
m_MSAAInvMap[ 4 ] = 2;
m_MSAAInvMap[ 8 ] = 3;

----------------------------------------------------------------
-- Display updating
----------------------------------------------------------------

function UpdateGameOptionsDisplay()

	local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
	
    Controls.RightButtonRebaseCheckbox:SetCheck( EUI_options.GetValue( "DB_bRightButtonRebase" ) ~= 0 )
    --Controls.NoCitizenWarningCheckbox:SetCheck( OptionsManager.IsNoCitizenWarning() );
    Controls.AutoWorkersDontReplaceCB:SetCheck( OptionsManager.IsAutoWorkersDontReplace_Cached() );
    Controls.AutoWorkersDontRemoveFeaturesCB:SetCheck( OptionsManager.IsAutoWorkersDontRemoveFeatures_Cached() );
    Controls.NoRewardPopupsCheckbox:SetCheck( OptionsManager.IsNoRewardPopups_Cached() );
    Controls.NoBasicHelpCheckbox:SetCheck( OptionsManager.IsNoBasicHelp_Cached() );
    Controls.NoTileRecommendationsCheckbox:SetCheck( OptionsManager.IsNoTileRecommendations_Cached() );
    Controls.CivilianYieldsCheckbox:SetCheck( OptionsManager.IsCivilianYields_Cached() );
    Controls.SinglePlayerAutoEndTurnCheckBox:SetCheck(OptionsManager.GetSinglePlayerAutoEndTurnEnabled_Cached());
		Controls.MultiplayerAutoEndTurnCheckbox:SetCheck(OptionsManager.GetMultiplayerAutoEndTurnEnabled_Cached());
    Controls.QuickSelectionAdvCheckbox:SetCheck( OptionsManager.GetQuickSelectionAdvanceEnabled_Cached() );
	
	if (bIsInGame) then
		if (PreGame.IsMultiplayerGame() or PreGame.IsHotSeatGame()) then
			Controls.SPQuickCombatCheckBox:SetCheck(OptionsManager.GetSinglePlayerQuickCombatEnabled_Cached());
			Controls.SPQuickMovementCheckBox:SetCheck(OptionsManager.GetSinglePlayerQuickMovementEnabled_Cached());
			Controls.MPQuickCombatCheckbox:SetCheck(m_bInGameQuickCombatState_Cached);
			Controls.MPQuickMovementCheckbox:SetCheck(m_bInGameQuickMovementState_Cached);
		else
			Controls.SPQuickCombatCheckBox:SetCheck(m_bInGameQuickCombatState_Cached);
			Controls.SPQuickMovementCheckBox:SetCheck(m_bInGameQuickMovementState_Cached);
			Controls.MPQuickCombatCheckbox:SetCheck(OptionsManager.GetMultiplayerQuickCombatEnabled_Cached());
			Controls.MPQuickMovementCheckbox:SetCheck(OptionsManager.GetMultiplayerQuickMovementEnabled_Cached());
		end
	else
		Controls.SPQuickCombatCheckBox:SetCheck(OptionsManager.GetSinglePlayerQuickCombatEnabled_Cached());
		Controls.SPQuickMovementCheckBox:SetCheck(OptionsManager.GetSinglePlayerQuickMovementEnabled_Cached());
		Controls.MPQuickCombatCheckbox:SetCheck(OptionsManager.GetMultiplayerQuickCombatEnabled_Cached());
		Controls.MPQuickMovementCheckbox:SetCheck(OptionsManager.GetMultiplayerQuickMovementEnabled_Cached());
	end

	Controls.Tooltip1TimerLength:SetText(Locale.ConvertTextKey("TXT_KEY_OPSCREEN_TOOLTIP_1_TIMER_LENGTH", math.floor(OptionsManager.GetTooltip1Seconds_Cached() / 100) ));
 	--Controls.Tooltip2TimerLength:SetText(Locale.ConvertTextKey("TXT_KEY_OPSCREEN_TOOLTIP_2_TIMER_LENGTH", OptionsManager.GetTooltip2Seconds_Cached() / 100));

    Controls.ZoomCheck:SetCheck( OptionsManager.GetStraightZoom_Cached() );
    Controls.PolicyInfo:SetCheck( OptionsManager.GetPolicyInfo_Cached() );
    Controls.AutoUnitCycleCheck:SetCheck( OptionsManager.GetAutoUnitCycle_Cached() );
    Controls.ScoreListCheck:SetCheck( OptionsManager.GetScoreList_Cached() );
    Controls.MPScoreListCheck:SetCheck( OptionsManager.GetMPScoreList_Cached() );
    
    Controls.AutosaveTurnsEdit:SetText( OptionsManager.GetTurnsBetweenAutosave_Cached() );
    Controls.AutosaveMaxEdit:SetText( OptionsManager.GetNumAutosavesKept_Cached() );
    Controls.BindMousePull:GetButton():SetText( m_BindMouseText[ OptionsManager.GetBindMouseMode_Cached() ] );
	    
    local iTutorialLevel = OptionsManager.GetTutorialLevel_Cached();
	if (iTutorialLevel < 0) then
		iTutorialLevel = #m_TutorialLevelText;		
	end
	Controls.TutorialPull:GetButton():SetText( m_TutorialLevelText[ iTutorialLevel ] );
    
    Controls.LanguagePull:GetButton():SetText(Locale.GetCurrentLanguage().DisplayName);
    Controls.SpokenLanguagePull:GetButton():SetText(Locale.GetCurrentSpokenLanguage().DisplayName); --TODO: make this work like its friends -KS
	
	Controls.MusicVolumeSlider:SetValue(	GetVolumeKnobValue(iMusicVolumeKnobID) );
	Controls.EffectsVolumeSlider:SetValue(	GetVolumeKnobValue(iSFXVolumeKnobID) );
	Controls.SpeechVolumeSlider:SetValue(	GetVolumeKnobValue(iSpeechVolumeKnobID) );
	Controls.AmbienceVolumeSlider:SetValue( GetVolumeKnobValue(iAmbienceVolumeKnobID) );

	Controls.MusicVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_MUSIC_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iMusicVolumeKnobID)) ) );
	Controls.EffectsVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SF_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iSFXVolumeKnobID)) ) );
	Controls.SpeechVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_SPEECH_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iSpeechVolumeKnobID)) ) );
	Controls.AmbienceVolumeSliderValue:SetText( Locale.ConvertTextKey("TXT_KEY_OPSCREEN_AMBIANCE_SLIDER", Locale.ToPercent(GetVolumeKnobValue(iAmbienceVolumeKnobID)) ) );

	Controls.DragSpeedSlider:SetValue( OptionsManager.GetDragSpeed_Cached() / 2 );
	Controls.DragSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_DRAG_SPEED", OptionsManager.GetDragSpeed_Cached() ) );
	Controls.PinchSpeedSlider:SetValue( OptionsManager.GetPinchSpeed_Cached() / 2 );
	Controls.PinchSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_PINCH_SPEED", OptionsManager.GetPinchSpeed_Cached() ) );
	
	if( OptionsManager.GetAutoUIAssets_Cached() ) then
        Controls.AutoUIAssetsCheck:SetCheck( true );
        Controls.SmallUIAssetsCheck:SetCheck( false );
        Controls.SmallUIAssetsCheck:SetDisabled( true );
        Controls.SmallUIAssetsCheck:SetAlpha( 0.5 );
    else
        Controls.AutoUIAssetsCheck:SetCheck( false );
        Controls.SmallUIAssetsCheck:SetDisabled( false );
        Controls.SmallUIAssetsCheck:SetAlpha( 1.0 );
        Controls.SmallUIAssetsCheck:SetCheck( OptionsManager.GetSmallUIAssets_Cached() );
    end
	
    Controls.EnableMapInertiaCheck:SetCheck( OptionsManager.GetEnableMapInertia_Cached() );
    Controls.SkipIntroVideoCheck:SetCheck( OptionsManager.GetSkipIntroVideo_Cached() );
	
	Controls.Tooltip1TimerSlider:SetValue(OptionsManager.GetTooltip1Seconds_Cached()/1000);
	--Controls.Tooltip2TimerSlider:SetValue(OptionsManager.GetTooltip2Seconds_Cached()/1000);
end
Events.GameOptionsChanged.Add(UpdateGameOptionsDisplay);


----------------------------------------------------------------
----------------------------------------------------------------
function UpdateGraphicsOptionsDisplay()
	--resolution options
	BuildFSResPulldown();
	BuildWResPulldown();
	
    local bIsFullscreen = OptionsManager.GetFullscreen_Cached(); 
    Controls.FullscreenCheck:SetCheck( bIsFullscreen );
    Controls.FSResolutionPull:SetHide( not bIsFullscreen);
    Controls.WResolutionPull:SetHide( bIsFullscreen );
    
    local kResInfo = m_FullscreenResList[ OptionsManager.GetResolution_Cached() ];
    if( kResInfo ~= nil ) then
        Controls.FSResolutionPull:GetButton():SetText( kResInfo.Width .. "x" .. kResInfo.Height .. "   " .. kResInfo.Refresh .. " Hz" );
	end
    	
    local x, y = OptionsManager.GetWindowResolution_Cached();
    Controls.WResolutionPull:GetButton():SetText( x .. "x" .. y );
    Controls.MSAAPull:GetButton():SetText( m_MSAAText[ m_MSAAInvMap[OptionsManager.GetAASamples_Cached()] ] );
    
    --graphics options
    Controls.VSyncCheck:SetCheck( OptionsManager.GetVSync_Cached() );
    Controls.HDStratCheck:SetCheck( OptionsManager.GetHDStrategicView_Cached() );
    Controls.GPUDecodeCheck:SetCheck( OptionsManager.GetGPUTextureDecode_Cached() );
    if ( not OptionsManager.IsGPUTextureDecodeSupported() ) then
		Controls.GPUDecodeCheck:SetDisabled( true );
		Controls.GPUDecodeCheck:SetAlpha( 0.5 );
	end
	
	Controls.MinimizeGrayTilesCheck:SetCheck( OptionsManager.GetMinimizeGrayTiles_Cached() );
	Controls.FadeShadowsCheck:SetCheck( OptionsManager.GetFadeShadows_Cached() );
	
    Controls.LeaderPull:GetButton():SetText( m_LeaderText[ OptionsManager.GetLeaderQuality_Cached() ] );
	Controls.OverlayPull:GetButton():SetText( m_OverlayText[ OptionsManager.GetOverlayLevel_Cached() ] );
	Controls.ShadowPull:GetButton():SetText( m_ShadowText[ OptionsManager.GetShadowLevel_Cached() ] );
	Controls.FOWPull:GetButton():SetText( m_FOWText[ OptionsManager.GetFOWLevel_Cached() ] );
	Controls.TerrainDetailPull:GetButton():SetText( m_TerrainDetailText[ OptionsManager.GetTerrainDetailLevel_Cached() ] );
	Controls.TerrainTessPull:GetButton():SetText( m_TerrainTessText[ OptionsManager.GetTerrainTessLevel_Cached() ] );
	Controls.TerrainShadowPull:GetButton():SetText( m_TerrainShadowText[ OptionsManager.GetTerrainShadowQuality_Cached() ] );
	Controls.WaterPull:GetButton():SetText( m_WaterText[ OptionsManager.GetWaterQuality_Cached() ] );
	Controls.TextureQualityPull:GetButton():SetText( m_TextureQualityText[ OptionsManager.GetTextureQuality_Cached() ] );
end
Events.GraphicsOptionsChanged.Add(UpdateGraphicsOptionsDisplay);


----------------------------------------------------------------
----------------------------------------------------------------
function UpdateMultiplayerOptionsDisplay()
	Controls.TurnNotifySteamInviteCheckbox:SetCheck(OptionsManager.GetTurnNotifySteamInvite_Cached());
	Controls.TurnNotifyEmailCheckbox:SetCheck(OptionsManager.GetTurnNotifyEmail_Cached());
	Controls.TurnNotifyEmailAddressEdit:SetText(OptionsManager.GetTurnNotifyEmailAddress_Cached());
	Controls.TurnNotifySmtpEmailEdit:SetText(OptionsManager.GetTurnNotifySmtpEmailAddress_Cached());
	Controls.TurnNotifySmtpHostEdit:SetText(OptionsManager.GetTurnNotifySmtpHost_Cached());
	Controls.TurnNotifySmtpPortEdit:SetText(OptionsManager.GetTurnNotifySmtpPort_Cached());
	Controls.TurnNotifySmtpUserEdit:SetText(OptionsManager.GetTurnNotifySmtpUsername_Cached());
	Controls.TurnNotifySmtpPassEdit:SetText(OptionsManager.GetTurnNotifySmtpPassword_Cached());
	Controls.TurnNotifySmtpPassRetypeEdit:SetText(OptionsManager.GetTurnNotifySmtpPassword_Cached());
	Controls.TurnNotifySmtpTLS:SetCheck(OptionsManager.GetTurnNotifySmtpTLS_Cached());
	Controls.LANNickNameEdit:SetText(OptionsManager.GetLANNickName_Cached());
	ValidateSmtpPassword(); -- Update passwords match label
end


----------------------------------------------------------------
----------------------------------------------------------------
function UpdateOptionsDisplay()
    UpdateGameOptionsDisplay();
    UpdateGraphicsOptionsDisplay();
    UpdateMultiplayerOptionsDisplay();
end


----------------------------------------------------------------
-- Game Options Handlers
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
function OnNoCitizenWarningCheckbox( bIsChecked )
    OptionsManager.SetNoCitizenWarning_Cached( bIsChecked );
end
--Controls.NoCitizenWarningCheckbox:RegisterCheckHandler( OnNoCitizenWarningCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnAutoWorkersDontReplaceCheckbox( bIsChecked )
    OptionsManager.SetAutoWorkersDontReplace_Cached( bIsChecked );
end
Controls.AutoWorkersDontReplaceCB:RegisterCheckHandler( OnAutoWorkersDontReplaceCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnAutoWorkersDontRemoveFeaturesCheckbox( bIsChecked )
	OptionsManager.SetAutoWorkersDontRemoveFeatures_Cached( bIsChecked );
end
Controls.AutoWorkersDontRemoveFeaturesCB:RegisterCheckHandler( OnAutoWorkersDontRemoveFeaturesCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnNoRewardPopupsCheckbox( bIsChecked )
    OptionsManager.SetNoRewardPopups_Cached( bIsChecked );
end
Controls.NoRewardPopupsCheckbox:RegisterCheckHandler( OnNoRewardPopupsCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnNoTileRecommendationsCheckbox( bIsChecked )
    OptionsManager.SetNoTileRecommendations_Cached( bIsChecked );
end
Controls.NoTileRecommendationsCheckbox:RegisterCheckHandler( OnNoTileRecommendationsCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnCivilianYieldsCheckbox( bIsChecked )
    OptionsManager.SetCivilianYields_Cached( bIsChecked );
end
Controls.CivilianYieldsCheckbox:RegisterCheckHandler( OnCivilianYieldsCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnNoBasicHelpCheckbox( bIsChecked )
    OptionsManager.SetNoBasicHelp_Cached( bIsChecked );
end
Controls.NoBasicHelpCheckbox:RegisterCheckHandler( OnNoBasicHelpCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnQuickSelectionAdvCheckbox( bIsChecked )
    OptionsManager.SetQuickSelectionAdvanceEnabled_Cached( bIsChecked );
end
Controls.QuickSelectionAdvCheckbox:RegisterCheckHandler( OnQuickSelectionAdvCheckbox );

----------------------------------------------------------------
----------------------------------------------------------------
function OnAutosaveTurnsChanged( string )
    OptionsManager.SetTurnsBetweenAutosave_Cached( string );
end
Controls.AutosaveTurnsEdit:RegisterCallback( OnAutosaveTurnsChanged );


----------------------------------------------------------------
----------------------------------------------------------------
function OnAutosaveMaxChanged( string )
    OptionsManager.SetNumAutosavesKept_Cached( string );
end
Controls.AutosaveMaxEdit:RegisterCallback( OnAutosaveMaxChanged );


----------------------------------------------------------------
----------------------------------------------------------------
function OnZoomCheck( bIsChecked )
    OptionsManager.SetStraightZoom_Cached( bIsChecked );
end
Controls.ZoomCheck:RegisterCheckHandler( OnZoomCheck );


----------------------------------------------------------------
----------------------------------------------------------------
function OnGridCheck( bIsChecked )
    OptionsManager.SetGridOn_Cached( bIsChecked );
end
--Controls.GridCheck:RegisterCheckHandler( OnGridCheck );

----------------------------------------------------------------
----------------------------------------------------------------
function OnSinglePlayerAutoEndTurnCheck( bIsChecked )
	OptionsManager.SetSinglePlayerAutoEndTurnEnabled_Cached(bIsChecked);
end
Controls.SinglePlayerAutoEndTurnCheckBox:RegisterCheckHandler(OnSinglePlayerAutoEndTurnCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnMultiplayerAutoEndTurnCheck( bIsChecked )
	OptionsManager.SetMultiplayerAutoEndTurnEnabled_Cached(bIsChecked);
end
Controls.MultiplayerAutoEndTurnCheckbox:RegisterCheckHandler(OnMultiplayerAutoEndTurnCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnSinglePlayerQuickCombatCheck( bIsChecked )
	local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
	if (bIsInGame) then
		if (not PreGame.IsMultiplayerGame() and not PreGame.IsHotSeatGame()) then
			-- In a single player game, store for updating the current game option
			m_bInGameQuickCombatState_Cached = bIsChecked;
		end
	end
	OptionsManager.SetSinglePlayerQuickCombatEnabled_Cached(bIsChecked);
end
Controls.SPQuickCombatCheckBox:RegisterCheckHandler(OnSinglePlayerQuickCombatCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnSinglePlayerQuickMovementCheck( bIsChecked )
	local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
	if (bIsInGame) then
		if (not PreGame.IsMultiplayerGame() and not PreGame.IsHotSeatGame()) then
			-- In a single player game, store for updating the current game option
			m_bInGameQuickMovementState_Cached = bIsChecked;
		end
	end
	OptionsManager.SetSinglePlayerQuickMovementEnabled_Cached(bIsChecked);
end
Controls.SPQuickMovementCheckBox:RegisterCheckHandler(OnSinglePlayerQuickMovementCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnMultiplayerQuickCombatCheck( bIsChecked )
	local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
	if (bIsInGame) then
		if (PreGame.IsMultiplayerGame() or PreGame.IsHotSeatGame()) then
			-- In a multiplayer game, store for updating the current game option
			m_bInGameQuickCombatState_Cached = bIsChecked;
		end
	end
	OptionsManager.SetMultiplayerQuickCombatEnabled_Cached(bIsChecked);
end
Controls.MPQuickCombatCheckbox:RegisterCheckHandler(OnMultiplayerQuickCombatCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnMultiplayerQuickMovementCheck( bIsChecked )
	local bIsInGame = (ContextPtr:GetID() == "OptionsMenu_InGame");
	if (bIsInGame) then
		if (PreGame.IsMultiplayerGame() or PreGame.IsHotSeatGame()) then
			-- In a multiplayer game, store for updating the current game option
			m_bInGameQuickMovementState_Cached = bIsChecked;
		end
	end
	OptionsManager.SetMultiplayerQuickMovementEnabled_Cached(bIsChecked);
end
Controls.MPQuickMovementCheckbox:RegisterCheckHandler(OnMultiplayerQuickMovementCheck);

----------------------------------------------------------------
----------------------------------------------------------------
function OnAutoUI( bIsChecked )
    OptionsManager.SetAutoUIAssets_Cached( bIsChecked );
    --mini dispaly update here
    if( bIsChecked ) then
		Controls.SmallUIAssetsCheck:SetCheck( false );
        Controls.SmallUIAssetsCheck:SetDisabled( true );
        Controls.SmallUIAssetsCheck:SetAlpha( 0.5 );
    elseif( UI.GetCurrentGameState() == GameStateTypes.CIV5_GS_MAIN_MENU ) then
        Controls.SmallUIAssetsCheck:SetDisabled( false );
        Controls.SmallUIAssetsCheck:SetAlpha( 1.0 );
        Controls.SmallUIAssetsCheck:SetCheck( OptionsManager.GetSmallUIAssets_Cached() );
    end
end
Controls.AutoUIAssetsCheck:RegisterCheckHandler( OnAutoUI );


----------------------------------------------------------------
----------------------------------------------------------------
function OnSmallUI( bIsChecked )
    OptionsManager.SetSmallUIAssets_Cached( bIsChecked );
end
Controls.SmallUIAssetsCheck:RegisterCheckHandler( OnSmallUI );


----------------------------------------------------------------
----------------------------------------------------------------
function OnBindMousePull( level )
	OptionsManager.SetBindMouseMode_Cached( level );
    Controls.BindMousePull:GetButton():SetText( m_BindMouseText[ OptionsManager.GetBindMouseMode_Cached() ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnLanguagePull(level)
	level = level + 1; --offset because the pulldown is 0-based.
	Controls.LanguagePull:GetButton():SetText( g_Languages[level].DisplayName );
	g_chosenLanguage = level;
	ShowLanguageCountdown();
end

function OnSpokenLanguagePull(level) --TODO: hook this up too! -KS
	level = level + 1;
	Locale.SetCurrentSpokenLanguage( g_SpokenLanguages[level].Type );
	Controls.SpokenLanguagePull:GetButton():SetText( g_SpokenLanguages[level].DisplayName );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnPolicyInfo( bIsChecked )
    OptionsManager.SetPolicyInfo_Cached( bIsChecked );
end
Controls.PolicyInfo:RegisterCheckHandler( OnPolicyInfo );


----------------------------------------------------------------
----------------------------------------------------------------
function OnAutoUnitCycleCheck( bIsChecked )
    OptionsManager.SetAutoUnitCycle_Cached( bIsChecked );
end
Controls.AutoUnitCycleCheck:RegisterCheckHandler( OnAutoUnitCycleCheck );


----------------------------------------------------------------
----------------------------------------------------------------
function OnScoreListCheck( bIsChecked )
    OptionsManager.SetScoreList_Cached( bIsChecked );
end
Controls.ScoreListCheck:RegisterCheckHandler( OnScoreListCheck );

----------------------------------------------------------------
----------------------------------------------------------------
function OnMPScoreListCheck( bIsChecked )
    OptionsManager.SetMPScoreList_Cached( bIsChecked );
end
Controls.MPScoreListCheck:RegisterCheckHandler( OnMPScoreListCheck );

----------------------------------------------------------------
----------------------------------------------------------------
function OnEnableMapInertia( bIsChecked )
    OptionsManager.SetEnableMapInertia_Cached( bIsChecked );
end
Controls.EnableMapInertiaCheck:RegisterCheckHandler( OnEnableMapInertia );

----------------------------------------------------------------
----------------------------------------------------------------
function OnSkipIntroVideo( bIsChecked )
    OptionsManager.SetSkipIntroVideo_Cached( bIsChecked );
end
Controls.SkipIntroVideoCheck:RegisterCheckHandler( OnSkipIntroVideo );

----------------------------------------------------------------
----------------------------------------------------------------
function OnTutorialPull( level )
    local iTutorialLevel = level;
    
    local bExpansion2Active = ContentManager.IsActive("6DA07636-4123-4018-B643-6575B4EC336B", ContentType.GAMEPLAY);
    local bExpansion1Active = ContentManager.IsActive("0E3751A1-F840-4e1b-9706-519BF484E59D", ContentType.GAMEPLAY);
	if(bExpansion1Active and not bExpansion2Active) then
		if (iTutorialLevel == 4) then
			iTutorialLevel = -1;
		end
	elseif(bExpansion2Active) then
		if(iTutorialLevel == 5) then
			iTutorialLevel = -1;
		end
	else
		if (iTutorialLevel == 3) then
			iTutorialLevel = -1;
		end
    end
    OptionsManager.SetTutorialLevel_Cached(iTutorialLevel);
	Controls.TutorialPull:GetButton():SetText( m_TutorialLevelText[ level ] );
end


----------------------------------------------------------------
-- Graphics Options Handlers
----------------------------------------------------------------

---------------------------------------------------------------
----------------------------------------------------------------
function OnHDStrategicView( bIsChecked )
    OptionsManager.SetHDStrategicView_Cached( bIsChecked );
end
Controls.HDStratCheck:RegisterCheckHandler( OnHDStrategicView );

---------------------------------------------------------------
----------------------------------------------------------------
function OnGPUTextureDecode( bIsChecked )
    OptionsManager.SetGPUTextureDecode_Cached( bIsChecked );
end
Controls.GPUDecodeCheck:RegisterCheckHandler( OnGPUTextureDecode );

----------------------------------------------------------------
----------------------------------------------------------------
function OnMinimizeGrayTiles( bIsChecked )
	OptionsManager.SetMinimizeGrayTiles_Cached( bIsChecked );
end
Controls.MinimizeGrayTilesCheck:RegisterCheckHandler( OnMinimizeGrayTiles );

----------------------------------------------------------------
----------------------------------------------------------------
function OnFadeShadows( bIsChecked )
	OptionsManager.SetFadeShadows_Cached( bIsChecked );
end
Controls.FadeShadowsCheck:RegisterCheckHandler( OnFadeShadows );

----------------------------------------------------------------
----------------------------------------------------------------
function OnFullscreen( bIsChecked )
	OptionsManager.SetFullscreen_Cached( bIsChecked );
    Controls.FSResolutionPull:SetHide( not bIsChecked );
    Controls.WResolutionPull:SetHide( bIsChecked );
    
end
Controls.FullscreenCheck:RegisterCheckHandler( OnFullscreen );


----------------------------------------------------------------
----------------------------------------------------------------
function OnVSync( bIsChecked )
    OptionsManager.SetVSync_Cached( bIsChecked );
end
Controls.VSyncCheck:RegisterCheckHandler( OnVSync );

----------------------------------------------------------------
----------------------------------------------------------------
function OnLeaderPull( level )
    OptionsManager.SetLeaderQuality_Cached( level );
	Controls.LeaderPull:GetButton():SetText( m_LeaderText[ level ] );
end

----------------------------------------------------------------
----------------------------------------------------------------
function OnOverlayPull( level )
    OptionsManager.SetOverlayLevel_Cached( level );
	Controls.OverlayPull:GetButton():SetText( m_OverlayText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnShadowPull( level )
    OptionsManager.SetShadowLevel_Cached( level );
	Controls.ShadowPull:GetButton():SetText( m_ShadowText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnFOWPull( level )
    OptionsManager.SetFOWLevel_Cached( level );
	Controls.FOWPull:GetButton():SetText( m_FOWText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnTerrainDetailPull( level )
    OptionsManager.SetTerrainDetailLevel_Cached( level );
	Controls.TerrainDetailPull:GetButton():SetText( m_TerrainDetailText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnTerrainTessPull( level )
    OptionsManager.SetTerrainTessLevel_Cached( level );
	Controls.TerrainTessPull:GetButton():SetText( m_TerrainTessText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnTerrainShadowPull( level )
    OptionsManager.SetTerrainShadowQuality_Cached( level );
	Controls.TerrainShadowPull:GetButton():SetText( m_TerrainShadowText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnWaterPull( level )
    OptionsManager.SetWaterQuality_Cached( level );
	Controls.WaterPull:GetButton():SetText( m_WaterText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnTextureQualityPull( level )
    OptionsManager.SetTextureQuality_Cached( level );
	Controls.TextureQualityPull:GetButton():SetText( m_TextureQualityText[ level ] );
end


----------------------------------------------------------------
----------------------------------------------------------------
function OnWResolutionPull( index )
    local kResInfo = m_WindowResList[ index ];

    if( kResInfo ~= nil ) then
        OptionsManager.SetWindowResolution_Cached( kResInfo.x, kResInfo.y );
    	Controls.WResolutionPull:GetButton():SetText( kResInfo.x .. "x" .. kResInfo.y );
	end

end
Controls.WResolutionPull:RegisterSelectionCallback( OnWResolutionPull );


----------------------------------------------------------------
----------------------------------------------------------------
function OnFSResolutionPull( index )
    local kResInfo = m_FullscreenResList[ index ];
    
    OptionsManager.SetResolution_Cached( index );
    Controls.FSResolutionPull:GetButton():SetText( kResInfo.Width .. "x" .. kResInfo.Height .. "   " .. kResInfo.Refresh .. " Hz" );
    
    OptionsManager.SetWindowResolution_Cached( kResInfo.Width, kResInfo.Height );
    Controls.WResolutionPull:GetButton():SetText( kResInfo.Width .. "x" .. kResInfo.Height );

end
Controls.FSResolutionPull:RegisterSelectionCallback( OnFSResolutionPull );

----------------------------------------------------------------
----------------------------------------------------------------
function OnMSAAPull( level )
	OptionsManager.SetAASamples_Cached( m_MSAAMap[level] );
	Controls.MSAAPull:GetButton():SetText( m_MSAAText[ level ] );
end
Controls.MSAAPull:RegisterSelectionCallback( OnMSAAPull );


----------------------------------------------------------------
-- Multiplayer Options Handlers
----------------------------------------------------------------
----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySteamInviteCheck( bIsChecked )
	OptionsManager.SetTurnNotifySteamInvite_Cached(bIsChecked);
end
Controls.TurnNotifySteamInviteCheckbox:RegisterCheckHandler(OnTurnNotifySteamInviteCheck);


----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifyEmailCheck( bIsChecked )
	OptionsManager.SetTurnNotifyEmail_Cached(bIsChecked);
end
Controls.TurnNotifyEmailCheckbox:RegisterCheckHandler(OnTurnNotifyEmailCheck);


----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifyEmailAddressChanged( string )
	OptionsManager.SetTurnNotifyEmailAddress_Cached( string );
end
Controls.TurnNotifyEmailAddressEdit:RegisterCallback( OnTurnNotifyEmailAddressChanged );

----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySmtpEmailChanged( string )
	OptionsManager.SetTurnNotifySmtpEmailAddress_Cached( string );
end
Controls.TurnNotifySmtpEmailEdit:RegisterCallback( OnTurnNotifySmtpEmailChanged );

----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySmtpHostChanged( string )
	OptionsManager.SetTurnNotifySmtpHost_Cached( string );
end
Controls.TurnNotifySmtpHostEdit:RegisterCallback( OnTurnNotifySmtpHostChanged );


----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySmtpPortChanged( string )
	OptionsManager.SetTurnNotifySmtpPort_Cached( string );
end
Controls.TurnNotifySmtpPortEdit:RegisterCallback( OnTurnNotifySmtpPortChanged );


----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySmtpUsernameChanged( string )
	OptionsManager.SetTurnNotifySmtpUsername_Cached( string );
end
Controls.TurnNotifySmtpUserEdit:RegisterCallback( OnTurnNotifySmtpUsernameChanged );


----------------------------------------------------------------
----------------------------------------------------------------
function OnLANNickNameChanged( string )
	OptionsManager.SetLANNickName_Cached( string );
end
Controls.LANNickNameEdit:RegisterCallback( OnLANNickNameChanged );

----------------------------------------------------------------
----------------------------------------------------------------
function ValidateSmtpPassword()
	if( Controls.TurnNotifySmtpPassEdit:GetText() and Controls.TurnNotifySmtpPassRetypeEdit:GetText() and
		Controls.TurnNotifySmtpPassEdit:GetText() == Controls.TurnNotifySmtpPassRetypeEdit:GetText() ) then
		-- password editboxes match.
		OptionsManager.SetTurnNotifySmtpPassword_Cached( Controls.TurnNotifySmtpPassEdit:GetText() );
		Controls.StmpPasswordMatchLabel:SetText(passwordsMatchString);
		Controls.StmpPasswordMatchLabel:SetToolTipString(passwordsMatchTooltipString);
		Controls.StmpPasswordMatchLabel:SetColorByName( "Green_Chat" );
	else
		-- password editboxes do not match.
		Controls.StmpPasswordMatchLabel:SetText(passwordsNotMatchString);
		Controls.StmpPasswordMatchLabel:SetToolTipString(passwordsNotMatchTooltipString);
		Controls.StmpPasswordMatchLabel:SetColorByName( "Magenta_Chat" );		
	end
end
Controls.TurnNotifySmtpPassEdit:RegisterCallback( ValidateSmtpPassword );
Controls.TurnNotifySmtpPassRetypeEdit:RegisterCallback( ValidateSmtpPassword );

----------------------------------------------------------------
----------------------------------------------------------------
function OnTurnNotifySmtpTLSCheck( bIsChecked )
	OptionsManager.SetTurnNotifySmtpTLS_Cached(bIsChecked);
end
Controls.TurnNotifySmtpTLS:RegisterCheckHandler(OnTurnNotifySmtpTLSCheck);


----------------------------------------------------------------
-- Pulldowns
----------------------------------------------------------------

----------------------------------------------------------------
----------------------------------------------------------------
function BuildSinglePulldown( pulldown, textTable )
    local controlTable;
    for i = 0, #textTable do
        controlTable = {};
        pulldown:BuildEntry( "InstanceOne", controlTable );
        controlTable.Button:LocalizeAndSetText( textTable[i] );
        controlTable.Button:SetVoid1( i );
    end
    pulldown:CalculateInternals();
end


----------------------------------------------------------------
----------------------------------------------------------------
function BuildLeaderQualityPulldown()
    
    local controlTable;
    
    local size = #m_LeaderText;
    if UI.IsDX9() then
        size = size-1;        -- disallow high leaders on DX9, across the board
    end
    
    for i = 0, size do
    
        controlTable = {};
        Controls.LeaderPull:BuildEntry( "InstanceOne", controlTable );
        controlTable.Button:LocalizeAndSetText( m_LeaderText[i] );
        controlTable.Button:SetVoid1( i );
        
        -- disallow medium leaders on certain low-end machines
        if (i == 2 and not UI.AreMediumLeadersAllowed()) then
            controlTable.Button:SetDisabled(true);
            controlTable.Button:SetAlpha(.3);
        end
    
    end
    
    Controls.LeaderPull:CalculateInternals();
    
end

----------------------------------------------------------------
----------------------------------------------------------------
function BuildMSAAPulldown()
    local controlTable;
    for i = 0, #m_MSAAText do
        controlTable = {};
		Controls.MSAAPull:BuildEntry( "InstanceOne", controlTable );
		controlTable.Button:LocalizeAndSetText( m_MSAAText[i] );
		controlTable.Button:SetVoid1( i );
		if not OptionsManager.IsAALevelSupported(m_MSAAMap[i]) then
			controlTable.Button:SetDisabled(true);
			controlTable.Button:SetAlpha(.3);
		end
    end
    Controls.MSAAPull:CalculateInternals();
end

----------------------------------------------------------------
----------------------------------------------------------------
function BuildPulldowns()

    BuildFSResPulldown();
    BuildWResPulldown();
    BuildMSAAPulldown();
    
    --BuildSinglePulldown( Controls.TutorialPull, m_TutorialLevelText );
    Controls.TutorialPull:RegisterSelectionCallback( OnTutorialPull );
    
    BuildSinglePulldown( Controls.OverlayPull, m_OverlayText );
    Controls.OverlayPull:RegisterSelectionCallback( OnOverlayPull );
    
    BuildSinglePulldown( Controls.ShadowPull, m_ShadowText );
    Controls.ShadowPull:RegisterSelectionCallback( OnShadowPull );

    BuildSinglePulldown( Controls.FOWPull, m_FOWText );
    Controls.FOWPull:RegisterSelectionCallback( OnFOWPull );
  
    BuildSinglePulldown( Controls.TerrainDetailPull, m_TerrainDetailText );
    Controls.TerrainDetailPull:RegisterSelectionCallback( OnTerrainDetailPull );
  
    BuildSinglePulldown( Controls.TerrainTessPull, m_TerrainTessText );
    Controls.TerrainTessPull:RegisterSelectionCallback( OnTerrainTessPull );

    BuildSinglePulldown( Controls.TerrainShadowPull, m_TerrainShadowText );
    Controls.TerrainShadowPull:RegisterSelectionCallback( OnTerrainShadowPull );

    BuildSinglePulldown( Controls.WaterPull, m_WaterText );
    Controls.WaterPull:RegisterSelectionCallback( OnWaterPull );

    BuildSinglePulldown( Controls.TextureQualityPull, m_TextureQualityText );
    Controls.TextureQualityPull:RegisterSelectionCallback( OnTextureQualityPull );


    ----------------------------------------------------------------
    -- leader detail pull (special case)
    local controlTable;
    local count = #m_LeaderText
    
    BuildLeaderQualityPulldown();
    Controls.LeaderPull:RegisterSelectionCallback( OnLeaderPull );
    ----------------------------------------------------------------


    BuildSinglePulldown( Controls.BindMousePull, m_BindMouseText );
    Controls.BindMousePull:RegisterSelectionCallback( OnBindMousePull );
    
    local languageTable = {};
    for i,v in ipairs(g_Languages) do
		languageTable[i - 1] = v.DisplayName;
    end
    BuildSinglePulldown( Controls.LanguagePull, languageTable);
    Controls.LanguagePull:RegisterSelectionCallback( OnLanguagePull );
    
    
    local spokenLanguageTable = {};
    for i,v in ipairs(g_SpokenLanguages) do
		spokenLanguageTable[i - 1] = v.DisplayName;
    end
    
    BuildSinglePulldown( Controls.SpokenLanguagePull, spokenLanguageTable);
    Controls.SpokenLanguagePull:RegisterSelectionCallback( OnSpokenLanguagePull );
end


----------------------------------------------------------------
----------------------------------------------------------------
function BuildWResPulldown()
    Controls.WResolutionPull:ClearEntries();
    m_maxX, m_maxY = OptionsManager.GetMaxResolution();

    local bFound = false;
    for i, kResInfo in ipairs( m_WindowResList ) do
        if( kResInfo.x <= m_maxX and
            kResInfo.y <= m_maxY ) then
            
            local controlTable = {};
            Controls.WResolutionPull:BuildEntry( "InstanceOne", controlTable );
            
            controlTable.Button:SetText( kResInfo.x .. "x" .. kResInfo.y );
            controlTable.Button:SetVoid1( i );
        end
    end
    
    Controls.WResolutionPull:CalculateInternals();
end


----------------------------------------------------------------
----------------------------------------------------------------
function BuildFSResPulldown()

    Controls.FSResolutionPull:ClearEntries();
    local count = #m_FullscreenResList;
    
    for i = count, 0, -1 do
        local kResInfo = m_FullscreenResList[i];
    
            --[[
            print( "Testing Mode: " .. kResInfo.Width .. " "
                                    .. kResInfo.Height .. " "
                                    .. kResInfo.Adapter .. " "
                                    .. kResInfo.Display .. " "
                                    .. kResInfo.Scale .. " "
                                    .. kResInfo.Refresh );--]]
                                   
        if( kResInfo.Height >= 768 and
            kResInfo.Adapter == 0 ) then
            
            local controlTable = {};
            Controls.FSResolutionPull:BuildEntry( "InstanceOne", controlTable );
            
            controlTable.Button:SetText( kResInfo.Width .. "x" .. kResInfo.Height .. "   " .. kResInfo.Refresh .. " Hz" );
            controlTable.Button:SetVoid1( i );
        end
    end
    
    Controls.FSResolutionPull:CalculateInternals();
end


----------------------------------------------------------------
-- build the internal list of fullscreen modes
----------------------------------------------------------------
m_iResolutionCount = UIManager:GetResCount();
for i = 0, m_iResolutionCount - 1 do
    local width, height, refresh, scale, display, adapter = UIManager:GetResInfo( i );
    m_FullscreenResList[i] = { Width = width,
                            Height = height,
                            Refresh = refresh,
                            Display = display,
                            Adapter = adapter };
end

function RefreshTutorialLevelOptions()
	TutorialLevels = {};
	table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_LOW"));
	table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_MEDIUM"));

	local bExpansion1Active = ContentManager.IsActive("0E3751A1-F840-4e1b-9706-519BF484E59D", ContentType.GAMEPLAY);
	local bExpansion2Active = ContentManager.IsActive("6DA07636-4123-4018-B643-6575B4EC336B", ContentType.GAMEPLAY);
	
	if(bExpansion1Active or bExpansion2Active) then
		table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_NEW_TO_XP"));
	end
	
	if(bExpansion2Active) then
		table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_NEW_TO_XP2"));
	end

	table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_HIGH"));
	table.insert(TutorialLevels, Locale.Lookup("TXT_KEY_OPSCREEN_TUTORIAL_OFF"));

	m_TutorialLevelText = {};
	for i,v in ipairs(TutorialLevels) do
		m_TutorialLevelText[i - 1] = v;
	end
	
	Controls.TutorialPull:ClearEntries();
	BuildSinglePulldown(Controls.TutorialPull, m_TutorialLevelText);
end

----------------------------------------------------------------
-- add the machine's desktop resolution just in case they're
-- running something weird
----------------------------------------------------------------
local bFound = false;
for _, kResInfo in ipairs( m_WindowResList ) do
    if( kResInfo.x == m_maxX and
        kResInfo.y == m_maxY ) then
        bFound = true;
    end
end
if( bFound == false ) then
    m_WindowResList[0] = { x=m_maxX, y=m_maxY };
end


RefreshTutorialLevelOptions();
BuildPulldowns();
UpdateOptionsDisplay();

----------------------------------------------------------------
----------------------------------------------------------------
function OnDragSpeedSlider( fSpeed )

    --fSpeed = (fSpeed * 2) + 0.1;
	iSpeed = math.floor((fSpeed + 0.02) * 20) / 10;
	
    OptionsManager.SetDragSpeed_Cached( iSpeed );
	OptionsManager.CommitGameOptions();
	Controls.DragSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_DRAG_SPEED", iSpeed ) );
end
Controls.DragSpeedSlider:RegisterSliderCallback( OnDragSpeedSlider )
Controls.DragSpeedSlider:SetValue( OptionsManager.GetDragSpeed() / 2 );
Controls.DragSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_DRAG_SPEED", OptionsManager.GetDragSpeed() ) );


----------------------------------------------------------------
----------------------------------------------------------------
function OnPinchSpeedSlider( fSpeed )

    --fSpeed = (fSpeed * 2) + 0.1;
	iSpeed = math.floor((fSpeed + 0.02) * 20) / 10;

    OptionsManager.SetPinchSpeed_Cached( iSpeed );
	OptionsManager.CommitGameOptions();
	Controls.PinchSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_PINCH_SPEED", iSpeed ) );
end
Controls.PinchSpeedSlider:RegisterSliderCallback( OnPinchSpeedSlider )
Controls.PinchSpeedSlider:SetValue( OptionsManager.GetPinchSpeed() / 2 );
Controls.PinchSpeedValue:SetText( Locale.ConvertTextKey( "TXT_KEY_PINCH_SPEED", OptionsManager.GetPinchSpeed() ) );


----------------------------------------------------------------
-- NEW: HOTKEY MANAGER START
----------------------------------------------------------------
HotkeyManagerData = Modding.OpenUserData( "IngameHotkeyManager", 1);


g_StackChildren = {};
g_StackStacks = {};
g_CurrentEditable = {};
g_ListenInput = false;
g_InputSeqStr = {'', '', '', ''};
g_bShowInvalidActions = HotkeyManagerData.GetValue('bShowInvalidActions') or 0;

local Categories = {[0] = 'InterfaceModes', 'Commands', 'Builds', 'UnitPromotions', 'Specialists', 'Controls', 'Automates', 'Missions', };

-- Universal map from legacy KB to VK keycodes
g_KeyMap = {
    KB_MOUSE_LBUTTON = {1, "VK_LBUTTON", "LMB"}, --//not used
    KB_MOUSE_RBUTTON = {2, "VK_RBUTTON", "RMB"}, --//not used
    KB_CANCEL = {},
    KB_MOUSE_MIDDLEBUTTON = {4, "VK_MBUTTON", "MMB"}, --//not used
    KB_MOUSE_XBUTTON1 = {}, --//not used
    KB_MOUSE_XBUTTON2 = {}, --//not used
    KB_UNUSED_07 = {}, --//not used
    KB_BACKSPACE = {8, "VK_BACK", "Backspace"},
    KB_TAB = {9, "VK_TAB", "TAB"},
    KB_UNUSED_0A = {},
    KB_UNUSED_0B = {},
    KB_CLEAR = {12, "VK_CLEAR", "CLEAR"},
    KB_RETURN = {13, "VK_RETURN", "Enter"},
    KB_UNUSED_0E = {},
    KB_UNUSED_0F = {},
    KB_SHIFT = {16, "VK_SHIFT", "..."},
    KB_CTRL = {17, "VK_CONTROL", "..."},
    KB_ALT = {18, "VK_MENU", "..."},
    KB_PAUSE = {19, "VK_PAUSE", "Pause Break"},
    KB_CAPSLOCK = {20, "VK_CAPITAL", "Caps Lock"},
    KB_KANA = {}, --//language key
    KB_UNUSED_16 = {},
    KB_JUNJA = {}, --//Language key
    KB_FINAL = {}, --//Language key
    KB_HANJA = {},  --//Language key
    KB_UNUSED_1A = {},
    KB_ESCAPE = {27, "VK_ESCAPE", "Esc"},
    KB_CONVERT = {}, --//language key
    KB_NONCONVERT = {}, --//language key
    KB_ACCEPT = {}, --//language key
    KB_MODECHANGE = {}, --//language key
    KB_SPACE = {32, "VK_SPACE", "Spacebar"},
    KB_PGUP = {33, "VK_PRIOR", "Page Up"},
    KB_PGDN = {34, "VK_NEXT", "Page Down"},
    KB_END = {35, "VK_END", "End"},
    KB_HOME = {36, "VK_HOME", "Home"},
    KB_LEFT = {37, "VK_LEFT", "Left Arrow"},
    KB_UP = {38, "VK_UP", "Up Arrow"},
    KB_RIGHT = {39, "VK_RIGHT", "Right Arrow"},
    KB_DOWN = {40, "VK_DOWN", "Down Arrow"},
    KB_SELECT = {41, "VK_SELECT", "SELECT"},
    KB_PRINT = {42, "VK_PRINT", "PRINT"},
    KB_EXECUTE = {43, "VK_EXECUTE ", "EXECUTE"},
    KB_PRINTSCRN = {44, "VK_SNAPSHOT", "Print Screen"},
    KB_INSERT = {45, "VK_INSERT", "Insert"},
    KB_DELETE = {46, "VK_DELETE", "Delete"},
    KB_HELP = {47, "VK_HELP", "HELP"},
    KB_0 = {48, "0", "0"},
    KB_1 = {49, "1", "1"},
    KB_2 = {50, "2", "2"},
    KB_3 = {51, "3", "3"},
    KB_4 = {52, "4", "4"},
    KB_5 = {53, "5", "5"},
    KB_6 = {54, "6", "6"},
    KB_7 = {55, "7", "7"},
    KB_8 = {56, "8", "8"},
    KB_9 = {57, "9", "9"},
    KB_UNUSED_3A = {},
    KB_UNUSED_3B = {},
    KB_UNUSED_3C = {},
    KB_UNUSED_3D = {},
    KB_UNUSED_3E = {},
    KB_UNUSED_3F = {},
    KB_UNUSED_40 = {},
    KB_A = {65, "A", "A"},
    KB_B = {66, "B", "B"},
    KB_C = {67, "C", "C"},
    KB_D = {68, "D", "D"},
    KB_E = {69, "E", "E"},
    KB_F = {70, "F", "F"},
    KB_G = {71, "G", "G"},
    KB_H = {72, "H", "H"},
    KB_I = {73, "I", "I"},
    KB_J = {74, "J", "J"},
    KB_K = {75, "K", "K"},
    KB_L = {76, "L", "L"},
    KB_M = {77, "M", "M"},
    KB_N = {78, "N", "N"},
    KB_O = {79, "O", "O"},
    KB_P = {80, "P", "P"},
    KB_Q = {81, "Q", "Q"},
    KB_R = {82, "R", "R"},
    KB_S = {83, "S", "S"},
    KB_T = {84, "T", "T"},
    KB_U = {85, "U", "U"},
    KB_V = {86, "V", "V"},
    KB_W = {87, "W", "W"},
    KB_X = {88, "X", "X"},
    KB_Y = {89, "Y", "Y"},
    KB_Z = {90, "Z", "Z"},
    KB_LWIN = {91, "VK_LWIN", "[COLOR_GREY]L WIN"},
    KB_RWIN = {92, "VK_RWIN", "[COLOR_GREY]R WIN"},
    KB_APPS = {93, "VK_APPS", "[COLOR_GREY]APPS"},
    KB_UNUSED_5E = {},
    KB_SLEEP = {},
    KB_NUMPAD0 = {96, "VK_NUMPAD0", "NUMPAD 0"},
    KB_NUMPAD1 = {97, "VK_NUMPAD1", "NUMPAD 1"},
    KB_NUMPAD2 = {98, "VK_NUMPAD2", "NUMPAD 2"},
    KB_NUMPAD3 = {99, "VK_NUMPAD3", "NUMPAD 3"},
    KB_NUMPAD4 = {100, "VK_NUMPAD4", "NUMPAD 4"},
    KB_NUMPAD5 = {101, "VK_NUMPAD5", "NUMPAD 5"},
    KB_NUMPAD6 = {102, "VK_NUMPAD6", "NUMPAD 6"},
    KB_NUMPAD7 = {103, "VK_NUMPAD7", "NUMPAD 7"},
    KB_NUMPAD8 = {104, "VK_NUMPAD8", "NUMPAD 8"},
    KB_NUMPAD9 = {105, "VK_NUMPAD9", "NUMPAD 9"},
    KB_NUMPADSTAR = {106, "VK_MULTIPLY", "NUMPAD *"},
    KB_NUMPADPLUS = {107, "VK_ADD", "NUMPAD +"},
    KB_SEPERATOR = {108, "VK_SEPARATOR", "SEPARATOR"},
    KB_NUMPADMINUS = {109, "VK_SUBTRACT", "NUMPAD -"},
    KB_NUMPADPERIOD = {110, "VK_DECIMAL", "[COLOR_GREY]NUMPAD ."},
    KB_NUMPADSLASH = {111, "VK_DIVIDE", "[COLOR_GREY]NUMPAD /"},
    KB_F1 = {112, "VK_F1", "F1"},
    KB_F2 = {113, "VK_F2", "F2"},
    KB_F3 = {114, "VK_F3", "F3"},
    KB_F4 = {115, "VK_F4", "F4"},
    KB_F5 = {116, "VK_F5", "F5"},
    KB_F6 = {117, "VK_F6", "F6"},
    KB_F7 = {118, "VK_F7", "F7"},
    KB_F8 = {119, "VK_F8", "F8"},
    KB_F9 = {120, "VK_F9", "F9"},
    KB_F10 = {121, "VK_F10", "F10"},
    KB_F11 = {122, "VK_F11", "F11"},
    KB_F12 = {123, "VK_F12", "F12"},
    KB_F13 = {124, "VK_F13", "[COLOR_GREY]F13"},
    KB_F14 = {125, "VK_F14", "[COLOR_GREY]F14"},
    KB_F15 = {126, "VK_F15", "[COLOR_GREY]F15"},
    KB_F16 = {127, "VK_F16", "[COLOR_GREY]F16"},
    KB_F17 = {128, "VK_F17", "[COLOR_GREY]F17"},
    KB_F18 = {129, "VK_F18", "[COLOR_GREY]F18"},
    KB_F19 = {130, "VK_F19", "[COLOR_GREY]F19"},
    KB_F20 = {131, "VK_F20", "[COLOR_GREY]F20"},
    KB_F21 = {132, "VK_F21", "[COLOR_GREY]F21"},
    KB_F22 = {133, "VK_F22", "[COLOR_GREY]F22"},
    KB_F23 = {134, "VK_F23", "[COLOR_GREY]F23"},
    KB_F24 = {135, "VK_F24", "[COLOR_GREY]F24"},
    KB_UNUSED_88 = {},
    KB_UNUSED_89 = {},
    KB_UNUSED_8A = {},
    KB_UNUSED_8B = {},
    KB_UNUSED_8C = {},
    KB_UNUSED_8D = {},
    KB_UNUSED_8E = {},
    KB_UNUSED_8F = {},
    KB_NUMLOCK = {144, "VK_NUMLOCK", "Num Lock"},
    KB_SCROLL = {145, "VK_SCROLL", "Scroll Lock"},
    KB_UNUSED_92_OEM = {},
    KB_UNUSED_93_OEM = {},
    KB_UNUSED_94_OEM = {},
    KB_UNUSED_95_OEM = {},
    KB_UNUSED_96_OEM = {},
    KB_UNUSED_97 = {},
    KB_UNUSED_98 = {},
    KB_UNUSED_99 = {},
    KB_UNUSED_9A = {},
    KB_UNUSED_9B = {},
    KB_UNUSED_9C = {},
    KB_UNUSED_9D = {},
    KB_UNUSED_9E = {},
    KB_UNUSED_9F = {},
    KB_LSHIFT = {160, "VK_LSHIFT", "[COLOR_GREY]L SHIFT"},
    KB_RSHIFT = {161, "VK_RSHIFT", "[COLOR_GREY]R SHIFT"},
    KB_LCTRL = {162, "VK_LCONTROL", "[COLOR_GREY]L CONTROL"},
    KB_RCTRL = {163, "VK_RCONTROL", "[COLOR_GREY]R CONTROL"},
    KB_LMENU = {164, "VK_LMENU", "[COLOR_GREY]L ALT"},
    KB_RMENU = {165, "VK_RMENU", "[COLOR_GREY]R ALT"},
    KB_BROWSER_BACK = {166, "VK_BROWSER_BACK", "[COLOR_GREY]BROWSER BACK"},
    KB_BROWSER_FORWARD = {167, "VK_BROWSER_FORWARD", "[COLOR_GREY]BROWSER FORWARD"},
    KB_BROWSER_REFRESH = {168, "VK_BROWSER_REFRESH", "[COLOR_GREY]BROWSER REFRESH"},
    KB_BROWSER_STOP = {169, "VK_BROWSER_STOP", "[COLOR_GREY]BROWSER STOP"},
    KB_BROWSER_SEARCH = {170, "VK_BROWSER_SEARCH", "[COLOR_GREY]BROWSER SEARCH"},
    KB_BROWSER_FAVORITES = {171, "VK_BROWSER_FAVORITES", "[COLOR_GREY]BROWSER FAVORITES"},
    KB_BROWSER_HOME = {172, "VK_BROWSER_HOME", "[COLOR_GREY]BROWSER HOME"},
    KB_VOLUME_MUTE = {173, "VK_VOLUME_MUTE", "[COLOR_GREY]VOLUME MUTE"},
    KB_VOLUMEDOWN = {174, "VK_VOLUME_DOWN", "VOLUME DOWN"},
    KB_VOLUMEUP = {175, "VK_VOLUME_UP", "VOLUME UP"},
    KB_MEDIA_NEXT_TRACK = {176, "VK_MEDIA_NEXT_TRACK", "[COLOR_GREY]NEXT TRACK"},
    KB_MEDIA_PREV_TRACK = {177, "VK_MEDIA_PREV_TRACK", "[COLOR_GREY]PREVIOUS TRACK"},
    KB_MEDIA_STOP = {178, "VK_MEDIA_STOP", "[COLOR_GREY]STOP MEDIA"},
    KB_MEDIA_PLAY = {179, "VK_MEDIA_PLAY_PAUSE", "[COLOR_GREY]PLAY/PAUSE MEDIA"},
    KB_LAUNCH_MAIL = {180, "VK_LAUNCH_MAIL", "[COLOR_GREY]START MAIL"},
    KB_LAUNCH_MEDIA_SELECT = {181, "VK_LAUNCH_MEDIA_SELECT", "[COLOR_GREY]SELECT MEDIA"},
    KB_LAUNCH_APP1 = {182, "VK_LAUNCH_APP1", "[COLOR_GREY]START APP1"},
    KB_LAUNCH_APP2 = {183, "VK_LAUNCH_APP2", "[COLOR_GREY]START APP2"},
    KB_UNUSED_B8 = {},
    KB_UNUSED_B9 = {},
    KB_SEMICOLON = {186, "VK_OEM_1", ";"}, --//semicolon for US keyboards
    KB_EQUALS = {187, "VK_OEM_PLUS", "="}, --//plus key in US
    KB_COMMA = {188, "VK_OEM_COMMA", ","}, --//comma key in US
    KB_MINUS = {189, "VK_OEM_MINUS", "-"}, --//minus key in US
    KB_PERIOD = {190, "VK_OEM_PERIOD", "."}, --//period key in US
    KB_SLASH = {191, "VK_OEM_2", "/"}, --//question mark key in US
    KB_GRAVE = {192, "VK_OEM_3", "~"}, --//tidle key in US

    KB_LBRACKET = {219, "VK_OEM_4", "{"}, --//left bracket
    KB_BACKSLASH = {220, "VK_OEM_5", "\\"}, --//backslash
    KB_RBRACKET = {221, "VK_OEM_6", "}"}, --//right bracket
    KB_APOSTROPHE = {222, "VK_OEM_7", "\'"}, --//quotation key
    KB_OEM_8 = {223, "VK_OEM_8", "VK_OEM_8"}, --//misc keys

    M_ = {}, --//Left mouse button
    M_ = {}, --//Right
    M_ = {}, --//Middle
    M_ = {}, --//4
    M_ = {}, --//5
    KB_LASTINPUT = {},
};

-------------------------------------------------
-------------------------------------------------
-------------------------------------------------
function KeySequenceToString(key, bCtrl, bAlt, bShift)
    local keySeqStr = '';
    if bCtrl then
        keySeqStr = keySeqStr .. 'Ctrl + ';
    end
    if bAlt then
        keySeqStr = keySeqStr .. 'Alt + ';
    end
    if bShift then
        keySeqStr = keySeqStr .. 'Shift + ';
    end
    if key == nil or key == '' then
        key = '--'
    end
    keySeqStr = keySeqStr .. key;
    return keySeqStr;
end

-------------------------------------------------
-------------------------------------------------
-- returns a table { HotKey, HotKeyPriority, CtrlDown, AltDown, ShiftDown, HotKeyAlt, HotKeyPriorityAlt, CtrlDownAlt, AltDownAlt, ShiftDownAlt }
function GetHotkey( Cat, Id, bBackup )
    local res = {}
    local name = string.format("'action_%d_%d'", Cat, Id);
    for query in HotkeyManagerData.Query(string.format('SELECT * FROM backup WHERE Name = %s', name)) do
        res.HotKey = query.HotKey;
        res.HotKeyPriority = query.HotKeyPriority;
        res.CtrlDown = query.CtrlDown;
        res.AltDown = query.AltDown;
        res.ShiftDown = query.ShiftDown;
        res.HotKeyAlt = query.HotKeyAlt;
        res.HotKeyPriorityAlt = query.HotKeyPriorityAlt;
        res.CtrlDownAlt = query.CtrlDownAlt;
        res.AltDownAlt = query.AltDownAlt;
        res.ShiftDownAlt = query.ShiftDownAlt;
    end
    if bBackup ~= true then
        for query in HotkeyManagerData.Query(string.format('SELECT * FROM SimpleValues WHERE Name = %s', name)) do
            res.HotKey = query.HotKey;
            res.HotKeyPriority = query.HotKeyPriority;
            res.CtrlDown = query.CtrlDown;
            res.AltDown = query.AltDown;
            res.ShiftDown = query.ShiftDown;
            res.HotKeyAlt = query.HotKeyAlt;
            res.HotKeyPriorityAlt = query.HotKeyPriorityAlt;
            res.CtrlDownAlt = query.CtrlDownAlt;
            res.AltDownAlt = query.AltDownAlt;
            res.ShiftDownAlt = query.ShiftDownAlt;
        end
    end
    return res;
end

-------------------------------------------------
-------------------------------------------------
function SendUpdateHotkey( Cat, Id, HotKey, CtrlDown, AltDown, ShiftDown, HotKeyPriority )
    -- here we 'pack' arguments into single 32-bit integer (kind of binary serialization)
    if (Cat < 0 or Cat > 15 or
        Id < 0 or Id > 65535 or
        HotKeyPriority < 0 or HotKeyPriority > 15) then

        print('WARNING SendUpdateHotkey: arguments out of bounds');
    else
        local product = (2 ^ 28) + (Cat * 2 ^ 24) + (Id * 2 ^ 8) + 0 + ((CtrlDown and 1 or 0) * 2 ^ 6) + ((AltDown and 1 or 0) * 2 ^ 5) + ((ShiftDown and 1 or 0) * 2 ^ 4) + HotKeyPriority;
        -- reason? so we can use Pregame object method which is defined both ingame and in the main menu (unlike 'Game' methods)
        PreGame.SetLeaderKey(product, HotKey);
        LuaEvents.UpdateHotkey();
    end
end

-------------------------------------------------
-------------------------------------------------
-------------------------------------------------
function OnHotkeyEditMode( Cat, Id )
    if next(g_CurrentEditable) ~= nil then
        local oldCat = g_CurrentEditable.HotkeyEditButton:GetVoid1();
        local oldId = g_CurrentEditable.HotkeyEditButton:GetVoid2();
        local HK = GetHotkey(oldCat, oldId);

        g_CurrentEditable.HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[HK.HotKey] and g_KeyMap[HK.HotKey][3] or '--', HK.CtrlDown, HK.AltDown, HK.ShiftDown));
        g_CurrentEditable.HotkeyEditButtonsBox:SetHide(true);
        g_CurrentEditable.HotkeyOuterBox:SetColorVal(255/255, 255/255, 200/255, 255/255);  -- COLOR_BEIGE
    end

    g_InputSeqStr = {'', '', '', ''};

    g_StackChildren[Cat][Id].HotkeyEditFrontLabel:LocalizeAndSetText('TXT_KEY_OPSCREEN_EDIT_HOTKEY_MODE');
    g_StackChildren[Cat][Id].HotkeyEditButtonsBox:SetHide(false);
    g_StackChildren[Cat][Id].HotkeyOuterBox:SetColorVal(231/255, 213/255, 0/255, 255/255);  -- COLOR_YIELD_GOLD

    g_CurrentEditable = g_StackChildren[Cat][Id];
    g_ListenInput = true;
end

-------------------------------------------------
-------------------------------------------------
function OnConfirmEditClick( Cat, Id )
    g_ListenInput = false;
    if next(g_CurrentEditable) ~= nil then

        local bCtrl = g_InputSeqStr[1] ~= '';
        local bAlt = g_InputSeqStr[2] ~= '';
        local bShift = g_InputSeqStr[3] ~= '';
        local key = g_InputSeqStr[4] or '';

        g_CurrentEditable.HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[key] and g_KeyMap[key][3], bCtrl, bAlt, bShift));
        g_CurrentEditable.HotkeyEditButtonsBox:SetHide(true);
        g_CurrentEditable.HotkeyOuterBox:SetColorVal(255/255, 255/255, 200/255, 255/255);  -- COLOR_BEIGE

        SendUpdateHotkey(Cat, Id, key, bCtrl, bAlt, bShift, 0);
        local DBstr = 'action_' .. Cat .. '_' .. Id;
        local q = string.format("REPLACE INTO SimpleValues (Name, ActionSubType, ActionIndex, HotKey, CtrlDown, AltDown, ShiftDown) VALUES ('%s', %d, %d, '%s', %d, %d, %d)", DBstr, Cat, Id, g_InputSeqStr[4], bCtrl and 1 or 0, bAlt and 1 or 0, bShift and 1 or 0)
        for row in HotkeyManagerData.Query(q) do
        end;
    end

end

-------------------------------------------------
-------------------------------------------------
function OnCancelEditClick( Cat, Id )
    g_ListenInput = false;
    if next(g_CurrentEditable) ~= nil then
        local HK = GetHotkey(Cat, Id);

        g_CurrentEditable.HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[HK.HotKey] and g_KeyMap[HK.HotKey][3] or '--', HK.CtrlDown, HK.AltDown, HK.ShiftDown));
        g_CurrentEditable.HotkeyEditButtonsBox:SetHide(true);
        g_CurrentEditable.HotkeyOuterBox:SetColorVal(255/255, 255/255, 200/255, 255/255);  -- COLOR_BEIGE
    end
end

-------------------------------------------------
-------------------------------------------------
function OnResetHotkeyClick( Cat, Id )
    g_ListenInput = false;
    if next(g_CurrentEditable) ~= nil then
        -- delete userdata entry and fallback to original hotkey
        for row in HotkeyManagerData.Query(string.format("DELETE FROM SimpleValues WHERE Name = 'action_%d_%d'", Cat, Id)) do
        end
        local HK = GetHotkey(Cat, Id, true);
        SendUpdateHotkey(Cat, Id, HK.HotKey, HK.CtrlDown, HK.AltDown, HK.ShiftDown, 0);

        g_CurrentEditable.HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[HK.HotKey] and g_KeyMap[HK.HotKey][3] or '--', HK.CtrlDown, HK.AltDown, HK.ShiftDown));
        g_CurrentEditable.HotkeyEditButtonsBox:SetHide(true);
        g_CurrentEditable.HotkeyOuterBox:SetColorVal(255/255, 255/255, 200/255, 255/255);  -- COLOR_BEIGE
    end
end

-------------------------------------------------
-------------------------------------------------
function OnResetAllHotkeysClick()
    g_ListenInput = false;
    if next(g_CurrentEditable) ~= nil then
        OnCancelEditClick(g_CurrentEditable.HotkeyEditCancelButton:GetVoid1(), g_CurrentEditable.HotkeyEditCancelButton:GetVoid2());
    end
    -- delete userdata entry and fallback to original hotkey
    local actions = {};
    for row in HotkeyManagerData.Query("SELECT Name, ActionSubType, ActionIndex FROM SimpleValues WHERE Name != 'bShowInvalidActions'") do
        actions[row.Name] = {row.ActionSubType, row.ActionIndex};
    end
    for name, action in next, actions do
        for row in HotkeyManagerData.Query(string.format("DELETE FROM SimpleValues WHERE Name = '%s'", name)) do
        end
        local HK = GetHotkey(action[1], action[2], true);
        g_StackChildren[action[1]][action[2]].HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[HK.HotKey] and g_KeyMap[HK.HotKey][3] or '--', HK.CtrlDown, HK.AltDown, HK.ShiftDown));
        SendUpdateHotkey(action[1], action[2], HK.HotKey, HK.CtrlDown, HK.AltDown, HK.ShiftDown, 0);
    end
    Controls.ConfirmResetAllHotkeysPopup:SetHide(true);
end

-------------------------------------------------
-------------------------------------------------
function ShowConfirmResetAllHotkeysPopup()
    g_ListenInput = false;
    if next(g_CurrentEditable) ~= nil then
        OnCancelEditClick(g_CurrentEditable.HotkeyEditCancelButton:GetVoid1(), g_CurrentEditable.HotkeyEditCancelButton:GetVoid2());
    end

    Controls.ConfirmResetAllHotkeysPopup:SetHide(false);
end

function CloseConfirmResetAllHotkeysPopup()
    Controls.ConfirmResetAllHotkeysPopup:SetHide(true);
end

Controls.ResetAllHotkeysButton:RegisterCallback(Mouse.eLClick, ShowConfirmResetAllHotkeysPopup);
Controls.ConfirmResetAllHotkeysYes:RegisterCallback(Mouse.eLClick, OnResetAllHotkeysClick);
Controls.ConfirmResetAllHotkeysNo:RegisterCallback(Mouse.eLClick, CloseConfirmResetAllHotkeysPopup);

-------------------------------------------------
-------------------------------------------------
function OnShowInvalidHotkeysCheckbox( bIsChecked )
    g_bShowInvalidActions = bIsChecked and 1 or 0;
    HotkeyManagerData.SetValue('bShowInvalidActions', g_bShowInvalidActions);
    for cat, it in next, g_StackChildren do
        for _, entry in next, it do
            --local cat = entry.HotkeyEditButton:GetVoid1();
            local id = entry.HotkeyEditButton:GetVoid2();
            if (cat ~= 2 and cat ~= 3 and cat ~= 5 and GameInfo[Categories[cat]][id].Visible == false) or (cat == 3 and (GameInfo[Categories[cat]][id].CannotBeChosen or (GameInfo[Categories[cat]][id].IconAtlas == 'ABILITY_ATLAS'))) then
                --print(GameInfo[Categories[cat]][id].Description, GameInfo[Categories[cat]][id].Visible)
                entry.Root:SetHide( not bIsChecked )
            end
        end
        
        g_StackStacks[cat].CategoryStack2:CalculateSize();
        g_StackStacks[cat].CategoryStack2:ReprocessAnchoring();
        g_StackStacks[cat].Root:SetSizeY(g_StackStacks[cat].CategoryStack2:GetSizeY());
    end
    Controls.HotkeysStack:CalculateSize();
    Controls.HotkeysStack:ReprocessAnchoring();
    Controls.ScrollPanel:CalculateInternalSize();
end
Controls.ShowInvalidHotkeysCheckbox:RegisterCheckHandler( OnShowInvalidHotkeysCheckbox );

-------------------------------------------------
-------------------------------------------------
function OnCategoryButtonClick( Cat )
    g_StackStacks[Cat].Root:SetHide(not g_StackStacks[Cat].Root:IsHidden());
    Controls.HotkeysStack:CalculateSize();
    Controls.HotkeysStack:ReprocessAnchoring();
    Controls.ScrollPanel:CalculateInternalSize();
end

-------------------------------------------------
-------------------------------------------------
-------------------------------------------------
function BackupActions()
    print('hotkeys backup START')
    for row in HotkeyManagerData.Query("DROP TABLE IF EXISTS backup") do
    end
    for row in HotkeyManagerData.Query("CREATE TABLE backup (Name TEXT)") do
    end
    local newColumns = {
        ActionSubType = { 'integer', 0 },
        ActionIndex = { 'integer', 0 },
        HotKey = { 'text', '' },
        HotKeyPriority = { 'integer', 0 },
        CtrlDown = { 'boolean', 0 },
        AltDown = { 'boolean', 0 },
        ShiftDown = { 'boolean', 0 },
        HotKeyAlt = { 'text', '' },
        HotKeyPriorityAlt = { 'integer', 0 },
        CtrlDownAlt = { 'boolean', 0 },
        AltDownAlt = { 'boolean', 0 },
        ShiftDownAlt = { 'boolean', 0 },
    }
    for col, info in next, newColumns do
        local defaultStr = '';
        if info[2] ~= '' then
            defaultStr = ' DEFAULT ' .. info[2];
        end
        for row in HotkeyManagerData.Query(string.format('ALTER TABLE backup ADD %s %s%s', col, info[1], defaultStr)) do
        end
    end

    local Categories = {[0] = 'InterfaceModes', 'Commands', 'Builds', 'UnitPromotions', 'Specialists', 'Controls', 'Automates', 'Missions', };
    for Cat,category in next,Categories do
        for row in DB.Query(string.format("SELECT * FROM %s", category)) do
            local DBstr = 'action_' .. Cat .. '_' .. row.ID;
            local q = string.format("REPLACE INTO backup (Name, ActionSubType, ActionIndex, HotKey, CtrlDown, AltDown, ShiftDown) VALUES ('%s', %d, %d, '%s', %d, %d, %d)", DBstr, Cat, row.ID, row.HotKey or '', row.CtrlDown and 1 or 0, row.AltDown and 1 or 0, row.ShiftDown and 1 or 0)
            for action in HotkeyManagerData.Query(q) do
            end
        end
    end
    print('hotkeys backup END')
end

-------------------------------------------------
-------------------------------------------------
-- on script load, make sure all columns are presented
-- in user data DB. if not, recreate the missing ones
function VerifyUserDataIntegrity()
    local columns = {};
    local newColumns = {
        ActionSubType = { 'integer', 0 },
        ActionIndex = { 'integer', 0 },
        HotKey = { 'text', '' },
        HotKeyPriority = { 'integer', 0 },
        CtrlDown = { 'boolean', 0 },
        AltDown = { 'boolean', 0 },
        ShiftDown = { 'boolean', 0 },
        HotKeyAlt = { 'text', '' },
        HotKeyPriorityAlt = { 'integer', 0 },
        CtrlDownAlt = { 'boolean', 0 },
        AltDownAlt = { 'boolean', 0 },
        ShiftDownAlt = { 'boolean', 0 },
    }
    for row in HotkeyManagerData.Query([[PRAGMA table_info(SimpleValues)]]) do
        columns[row.name] = true;
    end
    for col, info in next, newColumns do
        if columns[col] == nil then
            local defaultStr = '';
            if info[2] ~= '' then
                defaultStr = ' DEFAULT ' .. info[2];
            end
            for row in HotkeyManagerData.Query(string.format('ALTER TABLE SimpleValues ADD %s %s%s', col, info[1], defaultStr)) do
            end
        end
    end
end

-------------------------------------------------
-------------------------------------------------
-- on script load, send all user's previously saved hotkeys 
function InitHotkeys()
    for row in HotkeyManagerData.Query("SELECT * FROM SimpleValues WHERE Name != 'bShowInvalidActions'") do
        SendUpdateHotkey(row.ActionSubType, row.ActionIndex, row.HotKey, row.CtrlDown, row.AltDown, row.ShiftDown, row.HotKeyPriority);
    end
end

-------------------------------------------------
-------------------------------------------------
function BuildHotkeyManagerControls()
    for cat, it in ipairs(Categories) do
        -- 'Automates' category is removed, change it here if needed. 
        if it ~= 'Specialists' and it ~= 'Automates' then
            local instance = {};
            ContextPtr:BuildInstanceForControl("CategorySeparator", instance, Controls.HotkeysStack);
            instance.ControlCategoryText:SetText(it);
            instance.CategoryButton:SetVoid1(cat);
            instance.ControlCategoryText:LocalizeAndSetText("TXT_KEY_ACTIONTYPE_" .. Locale.ToUpper(Categories[cat]))
            instance.CategoryButton:RegisterCallback(Mouse.eLClick, OnCategoryButtonClick);
            if (g_bShowInvalidActions == 0) and it == 'Automates' then
                instance.Root:SetHide(true);
            end
            local catStackInstance = {};
            ContextPtr:BuildInstanceForControl("CategoryStackInstance", catStackInstance, Controls.HotkeysStack);
            g_StackStacks[cat] = catStackInstance;
            g_StackChildren[cat] = {};
    
            local i = 0;
            local scanRange = 0;
            local data = GameInfo[it];
    
            while (scanRange < 15) do
                if data[i] == nil then
                    scanRange = scanRange + 1;
                else
                    scanRange = 0;
                    local HK = GetHotkey(cat, i);

                    instance = {};
                    ContextPtr:BuildInstanceForControl("KeyEntry", instance, catStackInstance.CategoryStack2);

                    instance.HotkeyDescriptionLabel:LocalizeAndSetText(data[i].Description or data[i].Type or '??');
                    instance.HotkeyDescriptionLabel:LocalizeAndSetToolTip(data[i].Help or it == 'Builds' and data[i].Recommendation or '');
                    instance.HotkeyDescriptionIcon:LocalizeAndSetToolTip(data[i].Help or it == 'Builds' and data[i].Recommendation or '');
                    instance.HotkeyDescriptionIcon32:LocalizeAndSetToolTip(data[i].Help or it == 'Builds' and data[i].Recommendation or '');
                    -- illustrate actions
                    if it == 'Controls' then
                        -- skip
                        instance.HotkeyDescriptionIcon:SetHide(true);
                    elseif it == 'UnitPromotions' or it == 'Specialists' then
                        -- PortraitIndex/IconAtlas
                        if data[i].PortraitIndex ~= -1 then
                            if data[i].IconAtlas == 'ABILITY_ATLAS' then  -- no size 45 icons here :(
                                IconHookup( data[i].PortraitIndex, 32, data[i].IconAtlas, instance.HotkeyDescriptionIcon32 );
                                instance.HotkeyDescriptionIcon:SetHide(true);
                                instance.HotkeyDescriptionIcon32:SetHide(false);
                            else
                                IconHookup( data[i].PortraitIndex, 45, data[i].IconAtlas, instance.HotkeyDescriptionIcon );
                            end
                        else
                            instance.HotkeyDescriptionIcon:SetHide(true);
                        end
                    else
                        -- IconIndex/IconAtlas
                        if data[i].Type == 'MISSION_FOUND' then  -- special care for city found mission
                            instance.HotkeyDescriptionIcon:SetTextureAndResize("Assets/UI/Art/Icons/BuildCity36.dds");
                            instance.HotkeyDescriptionIcon:SetOffsetVal(instance.HotkeyDescriptionIcon:GetOffsetX() + 5, instance.HotkeyDescriptionIcon:GetOffsetY());
                        elseif data[i].IconIndex ~= -1 then
                            IconHookup( data[i].IconIndex, 45, data[i].IconAtlas, instance.HotkeyDescriptionIcon );
                        else
                            instance.HotkeyDescriptionIcon:SetHide(true);
                        end
                    end

                    instance.HotkeyEditButton:SetVoid1(cat);
                    instance.HotkeyEditButton:SetVoid2(i);
                    instance.HotkeyEditButton:RegisterCallback(Mouse.eLClick, OnHotkeyEditMode);

                    instance.HotkeyEditFrontLabel:SetText(KeySequenceToString(g_KeyMap[HK.HotKey] and g_KeyMap[HK.HotKey][3] or '--', HK.CtrlDown, HK.AltDown, HK.ShiftDown));

                    instance.HotkeyEditCancelButton:SetVoid1(cat);
                    instance.HotkeyEditCancelButton:SetVoid2(i);
                    instance.HotkeyEditCancelButton:RegisterCallback(Mouse.eLClick, OnCancelEditClick);

                    instance.HotkeyEditConfirmButton:SetVoid1(cat);
                    instance.HotkeyEditConfirmButton:SetVoid2(i);
                    instance.HotkeyEditConfirmButton:RegisterCallback(Mouse.eLClick, OnConfirmEditClick);

                    instance.HotkeyEditResetButton:SetVoid1(cat);
                    instance.HotkeyEditResetButton:SetVoid2(i);
                    instance.HotkeyEditResetButton:RegisterCallback(Mouse.eLClick, OnResetHotkeyClick);

                    g_StackChildren[cat][i] = instance;

                    -- exclude invisible (internal) actions
                    -- exclude embedded promotions
                    -- it seems all promotions from 'ABILITY_ATLAS' are embedded as well.
                    if (g_bShowInvalidActions == 0) and ((it ~= 'Builds' and it ~= 'Controls' and it ~= 'UnitPromotions' and data[i].Visible == false) or (it == 'UnitPromotions' and (data[i].CannotBeChosen or (data[i].IconAtlas == 'ABILITY_ATLAS')))) then
                        instance.Root:SetHide(true);
                    end
                end
                i = i + 1;
            end;
            catStackInstance.CategoryStack2:CalculateSize();
            catStackInstance.CategoryStack2:ReprocessAnchoring();
            catStackInstance.Root:SetSizeY(catStackInstance.CategoryStack2:GetSizeY());
        end
    end
    ContextPtr:BuildInstanceForControl("ScrollFinisher", instance, Controls.HotkeysStack);

    Controls.HotkeysStack:CalculateSize();
    Controls.HotkeysStack:ReprocessAnchoring();
    Controls.ScrollPanel:CalculateInternalSize();

    Controls.ShowInvalidHotkeysCheckbox:SetCheck( g_bShowInvalidActions == 1 );
end

----------------------------------------------------------------
-- Key Down Processing
----------------------------------------------------------------
function InputHandler( uiMsg, wParam, lParam )
    --print(uiMsg, wParam)
    if( g_ListenInput == true and g_CurrentEditable ~= nil) then
        local keySeqStr = ''
        if uiMsg == KeyEvents.KeyDown or uiMsg == 260 then
            if UIManager:GetControl() then
                g_InputSeqStr[1] = 'Ctrl + '
            else
                g_InputSeqStr[1] = ''
            end
            if UIManager:GetAlt() then
                g_InputSeqStr[2] = 'Alt + ';
            else
                g_InputSeqStr[2] = ''
            end
            if UIManager:GetShift() then
                g_InputSeqStr[3] = 'Shift + ';
            else
                g_InputSeqStr[3] = ''
            end
            if wParam ~= 16 and wParam ~= 17 and wParam ~= 18 then
                   for k,v in pairs(g_KeyMap) do
                          if v[1] == wParam then
                                 g_InputSeqStr[4] = k;
                          end
                   end
            else
                g_InputSeqStr[4] = (g_InputSeqStr[4] ~= '' and g_InputSeqStr[4]) or '...';
            end

            g_CurrentEditable.HotkeyEditFrontLabel:SetText(g_InputSeqStr[1] .. g_InputSeqStr[2] .. g_InputSeqStr[3] .. (g_InputSeqStr[4] ~= '...' and g_KeyMap[g_InputSeqStr[4]] and g_KeyMap[g_InputSeqStr[4]][3] or '...'));
        end
    elseif uiMsg == KeyEvents.KeyDown then
        if wParam == Keys.VK_ESCAPE then
            OnBack();
            return true;
        end
    end
    return true;
end
ContextPtr:SetInputHandler( InputHandler );

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

-- use UI:HasShownLegal() to backup xml hotkeys info EXACTLY once on initial application loading
if not UI:HasShownLegal() then
    BackupActions()
end
VerifyUserDataIntegrity();
InitHotkeys();
BuildHotkeyManagerControls();

-------------------------------------------------------------------------------
-- HOTKEY MANAGER END
-------------------------------------------------------------------------------
