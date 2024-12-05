
-- KICKED POPUP
PopupLayouts[ButtonPopupTypes.BUTTONPOPUP_KICKED] = function(popupInfo)
	local eReason		= popupInfo.Data1;
	
	if (eReason == NetKicked.BY_HOST) then
		popupText = Locale.ConvertTextKey("TXT_KEY_MP_KICKED");
	elseif (eReason == NetKicked.NO_HOST) then
		popupText = Locale.ConvertTextKey("TXT_KEY_MP_HOST_LOST");
	else
		popupText = Locale.ConvertTextKey("TXT_KEY_MP_NETWORK_CONNECTION_LOST");
	end	
		
	SetPopupText(popupText);
		
	-- Initialize 'OK' button.
	local OnOK = function()
		Events.ExitToMainMenu();		
		local interfaceModeSelection = InterfaceModeTypes.INTERFACEMODE_SELECTION
		UI.SetInterfaceMode(interfaceModeSelection);
	end
	
	local buttonText = Locale.ConvertTextKey("TXT_KEY_OK_BUTTON");
	AddButton(buttonText, OnOK);
			
	Controls.CloseButton:SetHide( true );
end

----------------------------------------------------------------        
-- Key Down Processing
----------------------------------------------------------------        
PopupInputHandlers[ButtonPopupTypes.BUTTONPOPUP_KICKED] = function( uiMsg, wParam, lParam )
    if uiMsg == KeyEvents.KeyDown then
        if( wParam == Keys.VK_ESCAPE or wParam == Keys.VK_RETURN ) then
			Events.ExitToMainMenu();		
            return true;
        end
    end
end

----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
Events.GameplaySetActivePlayer.Add(HideWindow);
