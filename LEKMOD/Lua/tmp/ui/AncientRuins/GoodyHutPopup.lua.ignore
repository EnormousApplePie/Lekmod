-------------------------------------------------
-- Goody Hut Popup
-------------------------------------------------

local m_PopupInfo = nil;

-------------------------------------------------
-- On Display
-------------------------------------------------
function OnPopup( popupInfo )

	if( popupInfo.Type ~= ButtonPopupTypes.BUTTONPOPUP_GOODY_HUT_REWARD ) then
		return;
	end

	m_PopupInfo = popupInfo;

    local iGoodyType = popupInfo.Data1;
    local pGoodyInfo = GameInfo.GoodyHuts[iGoodyType]
	
	local strText = "";
	
	if (pGoodyInfo.Type == "GOODY_GOLD") then
		local iGold = popupInfo.Data2;
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description, iGold));
	elseif (pGoodyInfo.Type == "GOODY_CULTURE" or pGoodyInfo.Type == "GOODY_PANTHEON_FAITH" or pGoodyInfo.Type == "GOODY_PROPHET_FAITH") then
		local iData = popupInfo.Data2;
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description, iData));
	-- MOD.EAP: Support for new Ancient ruins
	elseif (pGoodyInfo.FoodMin > 0 and pGoodyInfo.FoodMax > 0) then
		local iFood = popupInfo.Data2;
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description, iFood));
	elseif (pGoodyInfo.FaithMin > 0 and pGoodyInfo.FaithMax > 0) then
		local iFaith = popupInfo.Data2;
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description, iFaith));
	elseif (pGoodyInfo.CityStateInfluence > 0) then
		local sCityStateName = popupInfo.Data2;
		local iInfluence = popupInfo.Data3;
		print(sCityStateName, iInfluence);
		print(Locale.ConvertTextKey(pGoodyInfo.Description), sCityStateName, iInfluence);
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description), sCityStateName, iInfluence);
	elseif (pGoodyInfo.FreePromotion ~= nil) then
		local sPromotion = Locale.ConvertTextKey(GameInfo.UnitPromotions[pGoodyInfo.FreePromotion].Description);
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description, sPromotion));
	else
		Controls.DescriptionLabel:SetText(Locale.ConvertTextKey(pGoodyInfo.Description));
	end
		
	UIManager:QueuePopup( ContextPtr, PopupPriority.GoodyHut );
end
Events.SerialEventGameMessagePopup.Add( OnPopup );


----------------------------------------------------------------        
-- Input processing
----------------------------------------------------------------        
function OnCloseButtonClicked ()
    UIManager:DequeuePopup( ContextPtr );
end
Controls.CloseButton:RegisterCallback( Mouse.eLClick, OnCloseButtonClicked );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function InputHandler( uiMsg, wParam, lParam )
    if uiMsg == KeyEvents.KeyDown then
        if wParam == Keys.VK_ESCAPE or wParam == Keys.VK_RETURN then
            OnCloseButtonClicked();
            return true;
        end
    end
end
ContextPtr:SetInputHandler( InputHandler );


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
function ShowHideHandler( bIsHide, bInitState )

    if( not bInitState ) then
        if( not bIsHide ) then
        	UI.incTurnTimerSemaphore();
        	Events.SerialEventGameMessagePopupShown(m_PopupInfo);
        else
            UI.decTurnTimerSemaphore();
            Events.SerialEventGameMessagePopupProcessed.CallImmediate(ButtonPopupTypes.BUTTONPOPUP_GOODY_HUT_REWARD, 0);
        end
    end
end
ContextPtr:SetShowHideHandler( ShowHideHandler );

----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
Events.GameplaySetActivePlayer.Add(OnCloseButtonClicked);
