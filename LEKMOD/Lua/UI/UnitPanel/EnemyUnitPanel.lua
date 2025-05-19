-- Modifications to show ideology combat bonus in the enemy unit panel

include("IconSupport");
include("InstanceManager");
include("SupportFunctions");
include("InfoTooltipInclude");
include("CombatOverview");
include("CommonBehaviors");

local g_iPortraitSize = 256;

local g_MyCombatDataIM = InstanceManager:new("CombatResultsInstance", "Text", Controls.MyCombatResultsStack);
local g_TheirCombatDataIM = InstanceManager:new("CombatResultsInstance", "Text", Controls.TheirCombatResultsStack);
local maxUnitHitPoints = GameDefines.MAX_HIT_POINTS;

local g_iPortraitSize = 256;
local g_screenWidth, g_screenHeight = UIManager:GetScreenSizeVal();

-- We need this to access pUnit:GetCombatDifferentIdeology()
-- This will add our ideology combat bonus if appropriate
function LuaEvents.UnitPanelShowCombatModifiers(ownerID, unitID, targetOwnerID, targetUnitID, targetPlotID)
    local pPlayer = Players[ownerID];
    if (pPlayer == nil) then
        return;
    end
    
    local pUnit = pPlayer:GetUnitByID(unitID);
    if (pUnit == nil) then
        return;
    end
    
    local targetPlayer = Players[targetOwnerID];
    if (targetPlayer == nil) then
        return;
    end
    
    local targetUnit = targetPlayer:GetUnitByID(targetUnitID);
    if (targetUnit == nil) then
        return;
    end
    
    local pPlot = Map.GetPlot(targetPlotID);
    if (pPlot == nil) then
        return;
    end
    
    -- Check for ideology combat bonus
    local iIdeologyBonus = pUnit:GetCombatDifferentIdeology();
    
    if (iIdeologyBonus > 0) then
        local myPlayer = Players[ownerID];
        local targetPlayer = Players[targetOwnerID];
        
        -- Get ideologies
        local myIdeology = myPlayer:GetLateGamePolicyTree();
        local theirIdeology = targetPlayer:GetLateGamePolicyTree();
        
        -- Only show if ideologies are different and both players have one
        if (myIdeology ~= -1 and theirIdeology ~= -1 and myIdeology ~= theirIdeology) then
            local controlTable = g_MyCombatDataIM:GetInstance();
            controlTable.Text:LocalizeAndSetText("TXT_KEY_EUPANEL_IDEOLOGY_COMBAT_BONUS");
            controlTable.Value:SetText("+" .. tostring(iIdeologyBonus) .. "%");
        end
    end
end

LuaEvents.UnitPanelShowCombatModifiers.Add(UnitPanelShowCombatModifiers); 