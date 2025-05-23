-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

-------------------------------------------------------------------------------------------------------------------------
-- Macedonia UU: Hetairoi scaling with Era
-------------------------------------------------------------------------------------------------------------------------
function lekmod_macedonia_hetairoi_era_scaling(player_id)
    local player = Players[player_id]
    if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

    local current_era = player:GetCurrentEra()
    local era_types = {
        [GameInfoTypes["ERA_ANCIENT"]] = 10,
        [GameInfoTypes["ERA_CLASSICAL"]] = 15,
        [GameInfoTypes["ERA_MEDIEVAL"]] = 22,
        [GameInfoTypes["ERA_RENAISSANCE"]] = 30,
        [GameInfoTypes["ERA_INDUSTRIAL"]] = 38,
        [GameInfoTypes["ERA_MODERN"]] = 50,
        [GameInfoTypes["ERA_ATOMIC"]] = 60,
        [GameInfoTypes["ERA_INFORMATION"]] = 80
    }

    for unit in player:Units() do
        if unit:GetUnitType() == GameInfoTypes["UNIT_HETAIROI"] then
            -- Update all bonuses when era changes
            unit:SetBaseCombatStrength(era_types[current_era])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_MARCH"], current_era >= GameInfoTypes["ERA_INDUSTRIAL"])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_ARMOR_VS_LAND"], current_era >= GameInfoTypes["ERA_MODERN"])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_MOVEMENT_1"], current_era >= GameInfoTypes["ERA_ATOMIC"])
        end
    end
end

function lekmod_macedonia_hetairoi_created(player_id, unit_id)
    local player = Players[player_id]
    if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end
    
    local unit = player:GetUnitByID(unit_id)
    if not unit or unit:GetUnitType() ~= GameInfoTypes["UNIT_HETAIROI"] then return end

    local current_era = player:GetCurrentEra()
    
    -- Set initial combat strength based on era
    local era_types = {
        [GameInfoTypes["ERA_ANCIENT"]] = {strength = 10, xp = 15},
        [GameInfoTypes["ERA_CLASSICAL"]] = {strength = 15, xp = 15},
        [GameInfoTypes["ERA_MEDIEVAL"]] = {strength = 22, xp = 15},
        [GameInfoTypes["ERA_RENAISSANCE"]] = {strength = 30, xp = 30},
        [GameInfoTypes["ERA_INDUSTRIAL"]] = {strength = 38, xp = 30},
        [GameInfoTypes["ERA_MODERN"]] = {strength = 50, xp = 30},
        [GameInfoTypes["ERA_ATOMIC"]] = {strength = 60, xp = 30},
        [GameInfoTypes["ERA_INFORMATION"]] = {strength = 80, xp = 60}
    }

    -- Set initial strength and award XP
    unit:SetBaseCombatStrength(era_types[current_era].strength)
    unit:ChangeExperience(era_types[current_era].xp)

    -- Set era-based promotions
    if current_era >= GameInfoTypes["ERA_INDUSTRIAL"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_MARCH"], true)
    end
    if current_era >= GameInfoTypes["ERA_MODERN"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_ARMOR_VS_LAND"], true)
    end
    if current_era >= GameInfoTypes["ERA_ATOMIC"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_MOVEMENT_1"], true)
    end
end

-------------------------------------------------------------------------------------------------------------------------
if is_active then
    GameEvents.PlayerDoTurn.Add(lekmod_macedonia_hetairoi_era_scaling)
    -- UnitCreated is lekmod specific event
    GameEvents.UnitCreated.Add(lekmod_macedonia_hetairoi_created)
    GameEvents.TeamTechResearched.Add(lekmod_macedonia_hetairoi_era_scaling)
end
