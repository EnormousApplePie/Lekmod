include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_AKKAD"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

-------------------------------------------------------------------------------------------------------------------------
-- Akkad UA: Add a bonus promotion to units near a great general
-------------------------------------------------------------------------------------------------------------------------
function lekmod_akkad_general_city_bonus(player_id)

    local player = Players[player_id]
    if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

    local range = 2
    local needs_same_domain = false
    local needs_same_player = true

    for unit in player:Units() do
        if not unit:IsCombatUnit() and unit:GetDomainType() ~= GameInfoTypes["DOMAIN_AIR"] then -- skip
        -- Note: IsNearUnitWithPromotion is a Lekmod method. It is not available in the base game
        elseif unit:IsNearUnitWithPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"], range, needs_same_domain, needs_same_player) then
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_LITE_AKKAD_CITY_BONUS"], true)
        else
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_LITE_AKKAD_CITY_BONUS"], false)
        end
    end

end
-------------------------------------------------------------------------------------------------------------------------
if is_active then
    GameEvents.UnitSetXY.Add(lekmod_akkad_general_city_bonus)
    -- Note: UnitCreated is a Lekmod Event! Not available in the base game
    GameEvents.UnitCreated.Add(lekmod_akkad_general_city_bonus)
end