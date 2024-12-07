-------------------------------------------------
-- edit: MP voting system for EUI & vanilla UI
-------------------------------------------------
-- Diplomacy and Advisors Buttons that float out in the screen
-------------------------------------------------
include( "IconSupport" );
include( "SupportFunctions"  );
include( "InstanceManager" );

local g_pLocalTeam = Teams[ Players[ Game.GetActivePlayer() ]:GetTeam() ];
local g_bRebuildMPList = false;
local g_SortTable = {};
local g_PlayerEntries = {};
local g_TeamEntries = {};
local g_TeamData = {};

local SEPARATOR_MARGIN = 10;
local g_MPVotingSystemLastId = 0

local function onVoteReceived( Id, playerId, bVote )
	--print('--- vote received --- Id:', Id, 'playerId:', playerId, 'bVote:', bVote)
	if Id == g_MPVotingSystemLastId then
		UpdateAndSort(Id, 0);
	end
end

local function onProposalResult( Id, expires, OwnerId, SubjectId, iType, Status)
	--print('--- proposal Result received --- Id:', Id)
	--print('--- expires ' .. expires .. ' Type: ' .. iType .. ' Owner: ' .. OwnerId .. ' SubjectId: ' .. SubjectId .. ' RESULT: ' .. Status)
	local pActivePlayer = Players[Game.GetActivePlayer()];
	local sMessage =''
	local sSummary = ''
	iType = iType + 1001
	if (pActivePlayer ~= nil) then
		if Id == g_MPVotingSystemLastId then
			OnClose();
		end
		if (Status == 1) then
			if iType == 1001 then
				-- defeat screen comes with NetworkKickedPopup
				if Matchmaking.IsHost() then
					Matchmaking.KickPlayer(SubjectId)
				end
			elseif iType == 1002 then
				--Events.EndGameShow(EndGameTypes.Diplomatic, Players[SubjectId]:GetTeam())
				Game.SetWinner(Players[SubjectId]:GetTeam(), GameInfoTypes.VICTORY_DIPLOMATIC); 
			elseif iType == 1003 then
				--Events.EndGameShow(-1, Players[Game.GetActivePlayer()]:GetTeam())  -- scrap screen
				Game.SetWinner(Players[Game.GetActivePlayer()]:GetTeam(), GameInfoTypes.VICTORY_SCRAP);
			elseif iType == 1004 then
				Game.SetWinner(Players[Game.GetActivePlayer()]:GetTeam(), GameInfoTypes.VICTORY_SCRAP);
				if ContextPtr:IsHidden() == true then
					Events.SerialEventGameMessagePopup{ 
						Type = ButtonPopupTypes.BUTTONPOPUP_MODDER_0,
						Data1 = Id,
						Data2 = Status
					};
				end
			end
		end
	end
end

GameEvents.MPVotingSystemVote.Add(onVoteReceived)
GameEvents.MPVotingSystemProposalResult.Add(onProposalResult)


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function IsActiveHuman( pPlayer )
	-- Is this player really controlled by a human player?  
	-- This check handles the fact that pPlayer:IsHuman() returns true for unoccupied observer slots. DOH!
	return (Network.IsPlayerConnected(pPlayer:GetID()) or (pPlayer:IsHuman() and not pPlayer:IsObserver()));
end     


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function MPListSort( a, b )
     local aScore = g_SortTable[ tostring( a ) ];
     local bScore = g_SortTable[ tostring( b ) ];
        
     return aScore > bScore;
end


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function sortByPlayerID( pPlayer )
	-- Always sort by score now that multiplayer turn order is shown in the Turn Queue UI.
	return false;
	-- return (not pPlayer:IsSimultaneousTurns() and Game.IsNetworkMultiPlayer());
end


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function BuildStaticTeamsList()
	--print('BuildStaticTeamsList() invoked')
     for iPlayer = 0, GameDefines.MAX_MAJOR_CIVS - 1 do
    
        local pPlayer = Players[ iPlayer ];
        
        local bIsObserver = (pPlayer:IsObserver() and (PreGame.GetSlotClaim(pPlayer:GetID()) == SlotClaim.SLOTCLAIM_ASSIGNED));
        if( pPlayer:IsAlive() or bIsObserver ) then
            local iTeam = pPlayer:GetTeam();
            local pTeam = Teams[ iTeam ];
            
            if( pTeam:GetNumMembers() == 1 and not bIsObserver) then
                g_TeamData[ iTeam ] = pPlayer;
            else
                if( g_TeamData[ iTeam ] == nil ) then
                    g_TeamData[ iTeam ] = {};
                end
                table.insert( g_TeamData[ iTeam ], pPlayer );
            end
        end
    end
end

local g_bShowPings = false;

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function UpdatePlayerData( pPlayer, controlTable, Id )

	local iTeam = pPlayer:GetTeam();
	local pTeam = Teams[ iTeam ];
	local bMet = g_pLocalTeam:IsHasMet( iTeam );
	local bIsHuman = pPlayer:IsHuman();
	local bIsLocalPlayer = pPlayer:GetID() == Game.GetActivePlayer();
	local bIsNetworkMP = Game.IsNetworkMultiPlayer();
	local bIsPitboss = Game.IsOption("GAMEOPTION_PITBOSS");

	-- Hide dead players
	if(not pPlayer:IsAlive() and not pPlayer:IsObserver()) then
		controlTable.Root:SetHide(true);
	else
		controlTable.Root:SetHide(false);
	end

	-- Determine the size of player entry
	controlTable.PlayerDataStack:CalculateSize();
	controlTable.Root:SetSizeX(controlTable.PlayerDataStack:GetSizeX());
	
	-- Update seperator size
	controlTable.Seperator:SetSizeX( controlTable.PlayerDataStack:GetSizeX() - SEPARATOR_MARGIN);
	
    -- name
    if( bIsHuman ) then
		local activePlayer = Game.GetActivePlayer();
		if( Game.IsGameMultiPlayer() == false and bIsLocalPlayer ) then         
			controlTable.Name:LocalizeAndSetText( "TXT_KEY_YOU" );
		elseif( pPlayer:IsObserver() and not Network.IsPlayerConnected(pPlayer:GetID()) ) then
			-- empty observer slot
			controlTable.Name:LocalizeAndSetText( "TXT_KEY_SLOTTYPE_OPEN" );
		else
			controlTable.Name:SetText(pPlayer:GetNickName());
        end
    else
		if( bMet ) then
			controlTable.Name:SetText( pPlayer:GetName() );
        else
			controlTable.Name:LocalizeAndSetText( "TXT_KEY_POP_VOTE_RESULTS_UNMET_PLAYER" );
		end
    end
    
    -- icon
    CivIconHookup( bMet and pPlayer:GetID() or -1, 45, controlTable.Icon, controlTable.CivIconBG, controlTable.CivIconShadow, false, true );


    -- war color
    if( g_pLocalTeam:IsAtWar( pPlayer:GetTeam() ) ) then
        controlTable.Name:SetColorByName( "COLOR_NEGATIVE_TEXT" );
    else
        controlTable.Name:SetColorByName( "White" );
    end
    
    -- vote data
    local playerID = pPlayer:GetID();

    local bIsEligible = Game.GetProposalVoterEligibility(Id, playerID);
    local proposalType = Game.GetProposalType(Id);
    local proposalStatus = Game.GetProposalStatus(Id);
    if bIsEligible == true then
    	local bHasVoted = Game.GetProposalVoterHasVoted(Id, playerID);
    	if bHasVoted == true then
    		if proposalType == 3 and (playerID == Game.GetActivePlayer() or proposalStatus ~= 0) then
    			local bVote = Game.GetProposalVoterVote(Id, playerID);
    			if bVote == true then
    				controlTable.Vote:LocalizeAndSetText( "TXT_KEY_POSITIVE_VOTE_CHART_STATUS" );
    			else
    				controlTable.Vote:LocalizeAndSetText( "TXT_KEY_NEGATIVE_VOTE_CHART_STATUS" );
    			end
    		else
    			controlTable.Vote:SetText( "[COLOR_GREY]--[ENDCOLOR]" );
    		end
    	else
    		controlTable.Vote:SetText( "[COLOR_GREY]--[ENDCOLOR]" );
    	end
    	controlTable.Name:SetAlpha( 1 );
    	controlTable.IconBox:SetAlpha( 1 );
    else
    	controlTable.Vote:SetText( "" )
    	-- turn finished
		controlTable.Name:SetAlpha( 0.5 );
    	controlTable.IconBox:SetAlpha( 0.2 );
    end

	if(sortByPlayerID(pPlayer)) then
		g_SortTable[ tostring( controlTable.Root ) ] = GameDefines.MAX_PLAYERS - pPlayer:GetID();
	else
    	g_SortTable[ tostring( controlTable.Root ) ] = pPlayer:GetScore();
	end
	
	if(pPlayer:IsObserver()) then
		controlTable.Vote:SetHide(true);
	end

end

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function TeamHasObservers(pTeam, teamData)

    if( pTeam:GetNumMembers() == 1 ) then
        -- single member team
        local pPlayer = teamData;
		if( pPlayer:IsObserver() and PreGame.GetSlotClaim(pPlayer:GetID()) == SlotClaim.SLOTCLAIM_ASSIGNED ) then
			return true;
		end
	else
		for _, pPlayer in pairs( teamData ) do	        
			if( pPlayer:IsObserver() and PreGame.GetSlotClaim(pPlayer:GetID()) == SlotClaim.SLOTCLAIM_ASSIGNED ) then
				return true;
			end
		end
	end
	
	return false;
end

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function BuildControls()
	--print('BuildControls() invoked')

    for iTeam, teamData in pairs( g_TeamData ) do
        local pTeam = Teams[ iTeam ];

		local bObserverTeam = TeamHasObservers(pTeam, teamData);
        if( pTeam:GetNumMembers() == 1 and not bObserverTeam) then

            -- single member team
            local pPlayer = teamData;

            -- Make sure they're alive
            if( pPlayer:IsAlive() ) then
                local controlTable = {};
                ContextPtr:BuildInstanceForControl( "PlayerEntry", controlTable, Controls.MPListStack );
                g_PlayerEntries[ pPlayer:GetID() ] = controlTable;

                --UpdatePlayerData( pPlayer, controlTable, Id );
            end
        else
            local teamEntry = {};
            ContextPtr:BuildInstanceForControl( "TeamEntry", teamEntry, Controls.MPListStack );
			
            for _, pPlayer in pairs( teamData ) do
                
                if( pPlayer:IsEverAlive() or (pPlayer:IsObserver() and (PreGame.GetSlotClaim(pPlayer:GetID()) == SlotClaim.SLOTCLAIM_ASSIGNED)) ) then
                    local controlTable = {};
                    ContextPtr:BuildInstanceForControl( "PlayerEntry", controlTable, teamEntry.TeamStack );
                    g_PlayerEntries[ pPlayer:GetID() ] = controlTable;
                    controlTable.Seperator:SetHide( true );
																		
                    --UpdatePlayerData( pPlayer, controlTable, Id );
                end
            end
            
            if(bObserverTeam) then 
				-- OBSERVER_TEAM
				teamEntry.TeamName:LocalizeAndSetText( "TXT_KEY_MULTIPLAYER_OBSERVER_TEAM_NAME" );
			else			
				teamEntry.TeamName:LocalizeAndSetText( "TXT_KEY_MULTIPLAYER_DEFAULT_TEAM_NAME", iTeam + 1 );
			end

            teamEntry.TeamStack:CalculateSize();
            teamEntry.Root:SetSizeX( teamEntry.TeamStack:GetSizeX() );
            teamEntry.Root:SetSizeY( teamEntry.TeamStack:GetSizeY() + teamEntry.TeamName:GetSizeY() );

            g_TeamEntries[ iTeam ] = teamEntry;
            g_SortTable[ tostring( teamEntry.Root ) ] = pTeam:GetScore();
        end
    end
    
    Controls.MPListStack:CalculateSize();
    Controls.MPListScroll:SetSizeX(Controls.MPListStack:GetSizeX());
    Controls.MPListScroll:CalculateInternalSize();
    if( Controls.MPListScroll:GetRatio() < 1 ) then
        Controls.MPListScroll:SetOffsetX( 15 );
    end
    Controls.MPListScroll:ReprocessAnchoring();   

end


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function UpdateAndSort(Id, iResult)

	if Id ~= nil then
		g_MPVotingSystemLastId = Id;
	else
		print('Id was nil! fallback to last cached Id:', g_MPVotingSystemLastId)
		if g_MPVotingSystemLastId ~= nil then
			Id = g_MPVotingSystemLastId;
		else
			print('g_MPVotingSystemLastId is nil!')
		end
	end
	local proposalType = Game.GetProposalType(Id);
	--print('proposalType:', proposalType)
	local owner = Players[Game.GetProposalOwner(Id)] and Players[Game.GetProposalOwner(Id)]:GetName() or nil;
	local subject = Game.GetProposalSubject(Id);
	--print(Id, iResult, 'SUBJECT', subject, 'OWNER', owner)
	if subject ~= -1 then
		subject = Players[subject]:GetName()
	end
	if proposalType == 0 then  -- irr
		Controls.ProposalName:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_SUMMARY_IRR")
	elseif proposalType == 1 then  -- cc
		Controls.ProposalName:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_SUMMARY_CC", subject)
	elseif proposalType == 2 then  -- scrap
		Controls.ProposalName:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_SUMMARY_SCRAP")
	elseif proposalType == 3 then  -- remap
		Controls.ProposalName:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_SUMMARY_REMAP")
	end
	if owner then
		Controls.ProposalStartedBy:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_STARTED_BY", owner);
	else
		Controls.ProposalStartedBy:LocalizeAndSetText(" ");
	end
	Controls.Expiration:SetText(Game.GetElapsedGameTurns() + Game.GetProposalExpirationCounter(Id) + 1)
	local MaxVotes = Game.GetMaxVotes(Id)
	local YesVotes = Game.GetYesVotes(Id)
	local NoVotes = Game.GetNoVotes(Id)
	local ReceivedVotes = YesVotes + NoVotes
	local MissingVotes = MaxVotes - YesVotes - NoVotes
	Controls.MaxVoters:SetText(MaxVotes)
	if proposalType ~= 3 then
		Controls.ReceivedVotes:SetText(ReceivedVotes)
		Controls.MissingVotes:SetText(MissingVotes)
	else
		Controls.ReceivedVotes:SetText('--')
		Controls.MissingVotes:SetText('--')
	end

    for iPlayer, controlTable in pairs( g_PlayerEntries ) do
		UpdatePlayerData( Players[ iPlayer ], controlTable, Id );
    end
    
	local bIsNetworkMP = Game.IsNetworkMultiPlayer();
	
    for iTeam, teamEntry in pairs( g_TeamEntries ) do
		local pTeam = Teams[ iTeam ];
    	local score = pTeam:GetScore();
	
    	teamEntry.TeamStack:CalculateSize();
		teamEntry.TeamBox:SetSizeX( teamEntry.TeamStack:GetSizeX() );
    	teamEntry.Root:SetSizeX( teamEntry.TeamStack:GetSizeX() );
    	teamEntry.TeamSeparator:SetSizeX( teamEntry.TeamStack:GetSizeX() - SEPARATOR_MARGIN);
    	teamEntry.TeamHeaderSeparator:SetSizeX( teamEntry.TeamStack:GetSizeX() - SEPARATOR_MARGIN);
                  
    	g_SortTable[ tostring( teamEntry.Root ) ] = score;
        
    	-- sort all the players on the team
    	teamEntry.TeamStack:SortChildren( MPListSort );
	end
    
    -- sort all of the teams
	Controls.MPListStack:SortChildren( MPListSort );
  	--print('updating buttons for Id:', Id)
  	--print('result:', iResult)
	if iResult == 0 then  -- no result yet
		Controls.ProposalResultLabel:SetHide(true);
  		Controls.VoteYesButton:SetHide(false)
  		Controls.VoteNoButton:SetHide(false)
		if (Id ~= nil) and (Controls.VoteYesButton ~= nil) and (Controls.VoteNoButton ~= nil) then
  			local bEligible = Game.GetProposalVoterEligibility(Id, Game.GetActivePlayer())
  			local bHasVoted = Game.GetProposalVoterHasVoted(Id, Game.GetActivePlayer())
  			if bEligible == true and bHasVoted == false then
  				Controls.VoteYesButton:SetDisabled(false)
  				Controls.VoteNoButton:SetDisabled(false)
				Controls.VoteYesLabel:SetAlpha( 1 );
				Controls.VoteNoLabel:SetAlpha( 1 );
  			else
  				Controls.VoteYesButton:SetDisabled(true)
  				Controls.VoteNoButton:SetDisabled(true)
				Controls.VoteYesLabel:SetAlpha( 0.5 );
				Controls.VoteNoLabel:SetAlpha( 0.5 );
  			end
    	else
  		    print('Id was nil! don\'t press voteyes/voteno!')
    	end
	else
		--print('ProposalResultLabel')
		Controls.ProposalResultLabel:SetHide(false);
		--print('votey/n')
  		Controls.VoteYesButton:SetHide(true)
  		Controls.VoteNoButton:SetHide(true)
		if iResult == 1 then  -- result is passed
			--print('iResult1')
			Controls.ProposalResultLabel:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_PROPOSAL_PASSED");
		elseif iResult == 2 then  -- result is failed
			--print('iResult2')
			Controls.ProposalResultLabel:LocalizeAndSetText("TXT_KEY_MP_PROPOSAL_SCREEN_PROPOSAL_FAILED");
		end
    end
	--print('ReprocessAnchoring')
    Controls.MPListStack:ReprocessAnchoring();
end

function OnPopup( popupInfo )
	--print('OnPopup start')
	if( popupInfo.Type == ButtonPopupTypes.BUTTONPOPUP_MODDER_0 ) then
		m_PopupInfo = popupInfo;
		UpdateAndSort(m_PopupInfo.Data1, m_PopupInfo.Data2)
        if ( ContextPtr:IsHidden() == true ) then
			--print('OnPopup received Id:', m_PopupInfo.Data1)
			--print('OnPopup result:', m_PopupInfo.Data2)
        	UIManager:QueuePopup( ContextPtr, PopupPriority.InGameUtmost );
        end
	end
end
Events.SerialEventGameMessagePopup.Add( OnPopup );

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

function OnClose()
	--print("Dequeuing proposal chart");
    Events.SerialEventGameMessagePopupProcessed.CallImmediate(ButtonPopupTypes.BUTTONPOPUP_MODDER_0, 0);
	UIManager:DequeuePopup( ContextPtr );
end
Controls.CloseButton:RegisterCallback( Mouse.eLClick, OnClose );
Events.ActivePlayerTurnEnd.Add(OnClose)
Events.GameplaySetActivePlayer.Add(OnClose);



----------------------------------------------------------------
-- Key Down Processing
----------------------------------------------------------------
function InputHandler( uiMsg, wParam, lParam )
    if( uiMsg == KeyEvents.KeyDown )
    then
        if( wParam == Keys.VK_RETURN or wParam == Keys.VK_ESCAPE ) then
			OnClose();
            return true;
        end
    end
end
ContextPtr:SetInputHandler( InputHandler );

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

BuildStaticTeamsList();
BuildControls();

Controls.VoteYesButton:RegisterCallback( Mouse.eLClick, function() Network.SendGiftUnit(g_MPVotingSystemLastId, -5); end );  -- Yes
Controls.VoteNoButton:RegisterCallback( Mouse.eLClick, function() Network.SendGiftUnit(g_MPVotingSystemLastId, -6); end );  -- No
