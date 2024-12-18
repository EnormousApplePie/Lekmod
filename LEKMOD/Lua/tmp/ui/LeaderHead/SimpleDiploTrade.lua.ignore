-- edit:
-- expandable MP Trade Panel size
-- for EUI & vanilla UI
-------------------------------------------------
-- Diplomacy and Advisors Buttons that float out in the screen
-------------------------------------------------
include( "TradeLogic" );
Events.OpenPlayerDealScreenEvent.Add( OnOpenPlayerDealScreen );
Controls.ModifyButton:RegisterCallback( Mouse.eLClick, OnModify );



ContextPtr:SetInputHandler( InputHandler );

Controls.CloseButton:RegisterCallback( Mouse.eLClick, OnBack );

----------------------------------------------------------------
-- 'Active' (local human) player has changed
----------------------------------------------------------------
function OnDiploActivePlayerChanged( iActivePlayer, iPrevActivePlayer )
	if (not ContextPtr:IsHidden()) then
		OnBack();
	end
end
Events.GameplaySetActivePlayer.Add(OnDiploActivePlayerChanged);

local sgPrevCursor = 0;
----------------------------------------------------------------        
----------------------------------------------------------------  
function OnShowHide( bHide )
	if(not bHide) then 
		sgPrevCursor = UIManager:SetUICursor(0);
		LuaEvents.OpenSimpleDiploTrade();
	else
		UIManager:SetUICursor(sgPrevCursor);
	end
end
ContextPtr:SetShowHideHandler( OnShowHide );


----------------------------------------------------------------        
----------------------------------------------------------------  
function OnOpenAILeaderDiploTrade()
	-- An AI leader trade was started while we're open, close ourselves.
	if (not ContextPtr:IsHidden()) then
		OnBack();
	end
end
LuaEvents.OpenAILeaderDiploTrade.Add(OnOpenAILeaderDiploTrade);

-------------------------------------------------
-- NEW: expandable MP Trade Panel size
-------------------------------------------------
local EUI_options = Modding.OpenUserData( "Enhanced User Interface Options", 1);
local timeSinceLastDBWrite = 0;
function OnEditVerticalSize(v)
    if Controls.VerticalSizeDragArea:HasMouseOver() then
        local w, h = UIManager:GetScreenSizeVal();
        local dx, dy = UIManager:GetMouseDelta();
        if Controls.Pockets:IsHidden() then
			local y1 = math.max( math.min( Controls.MainGrid:GetSizeY() - dy, h - 500 ), 300 );
        	if os.clock() - timeSinceLastDBWrite > 0.1 then  -- 0.1s cooldown for DB access
        	    EUI_options.SetValue( "DB_iTradePanelSizeY2", y1 );
        	    timeSinceLastDBWrite = os.clock();
        	end
        	LuaEvents.TradePanelResize(y1, false);
        else
        	local y1 = math.max( math.min( Controls.MainGrid:GetSizeY() - 200 - dy, h - 500 ), 200 );
        	if os.clock() - timeSinceLastDBWrite > 0.1 then  -- 0.1s cooldown for DB access
        	    EUI_options.SetValue( "DB_iTradePanelSizeY", y1 );
        	    timeSinceLastDBWrite = os.clock();
        	end
        	LuaEvents.TradePanelResize(y1, true);
        end
    end
end
function OnMouseEnterDragArea()
    UIManager:SetUICursor(15);
    return true;
end
function OnMouseExitDragArea()
    UIManager:SetUICursor(0);
    return true;
end
function ApplyVerticalSizeChange(dy, bPockets)
    local tablesY, pocketY;
    if bPockets then
    	pocketY = math.floor(dy * 229/349);
    	tablesY = math.floor(dy * 120/349);

    	Controls.MainGrid:SetSizeY(dy + 200);
    	Controls.Pockets:SetSizeY(pocketY);
    	Controls.PocketsVertSeparator:SetSizeY(pocketY + 2);
    	Controls.UsPocketWhole:SetSizeY(pocketY - 9);
    	Controls.UsPocketPanel:SetSizeY(pocketY - 11);
    	Controls.ThemPocketWhole:SetSizeY(pocketY - 9);
    	Controls.ThemPocketPanel:SetSizeY(pocketY - 11);
    else
    	Controls.MainGrid:SetSizeY(dy);
    	local shift = 262 - (Controls.ModifyButton:IsHidden() and Controls.ModifyButton:GetSizeY() or 0)
    		- (Controls.ProposeButton:IsHidden() and Controls.ProposeButton:GetSizeY() or 0)
    		- (Controls.CancelButton:IsHidden() and Controls.CancelButton:GetSizeY() or 0);
    	tablesY = dy - shift;
    end

    Controls.Tables:SetSizeY(tablesY);
    Controls.TablesVertSeparator:SetSizeY(tablesY + 2);
    Controls.UsTableWhole:SetSizeY(tablesY);
    Controls.UsTablePanel:SetSizeY(tablesY - 3);
    Controls.ThemTableWhole:SetSizeY(tablesY);
    Controls.ThemTablePanel:SetSizeY(tablesY - 3);
	Controls.ModificationBlock:SetSizeY(tablesY);

    Controls.UsPocketPanel:CalculateInternalSize();
    Controls.UsPocketPanel:ReprocessAnchoring();
    Controls.ThemPocketPanel:CalculateInternalSize();
    Controls.ThemPocketPanel:ReprocessAnchoring();
    Controls.UsTablePanel:CalculateInternalSize();
    Controls.UsTablePanel:ReprocessAnchoring();
    Controls.ThemTablePanel:CalculateInternalSize();
    Controls.ThemTablePanel:ReprocessAnchoring();
end
LuaEvents.TradePanelResize.Add(ApplyVerticalSizeChange)

Controls.VerticalSizeDragArea:RegisterSliderCallback( OnEditVerticalSize );
Controls.VerticalSizeDragArea:RegisterCallback( Mouse.eMouseEnter, OnMouseEnterDragArea );
Controls.VerticalSizeDragArea:RegisterCallback( Mouse.eMouseExit, OnMouseExitDragArea );
