-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

-------------------------------------------------------------------------------------------------------------------------
-- Macedonia UU: Hetairoi scaling with Era
-------------------------------------------------------------------------------------------------------------------------
function lekmod_macedonia_hetairoi_era_scaling(player_id)
    local player = Players[player_id]

    local current_era = player:GetCurrentEra()
    local era_types = {
        [GameInfoTypes["ERA_ANCIENT"]] = 12,
        [GameInfoTypes["ERA_CLASSICAL"]] = 16,
        [GameInfoTypes["ERA_MEDIEVAL"]] = 23,
        [GameInfoTypes["ERA_RENAISSANCE"]] = 30,
        [GameInfoTypes["ERA_INDUSTRIAL"]] = 40,
        [GameInfoTypes["ERA_MODERN"]] = 65,
        [GameInfoTypes["ERA_POSTMODERN"]] = 80,
        [GameInfoTypes["ERA_FUTURE"]] = 100
    }

    for unit in player:Units() do
        if unit:GetUnitType() == GameInfoTypes["UNIT_HETAIROI"] then
            -- Update all bonuses when era changes
            unit:SetBaseCombatStrength(era_types[current_era])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_MARCH"], current_era >= GameInfoTypes["ERA_INDUSTRIAL"])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_ARMOR_VS_LAND"], current_era >= GameInfoTypes["ERA_MODERN"])
            unit:SetHasPromotion(GameInfoTypes["PROMOTION_EXTRA_MOVES_I"], current_era >= GameInfoTypes["ERA_POSTMODERN"])
            -- Refresh movement points to apply movement bonuses immediately
            unit:SetMoves(unit:MaxMoves())
        end
    end
end

function lekmod_macedonia_hetairoi_created(player_id, unit_id)
    local player = Players[player_id]
    
    local unit = player:GetUnitByID(unit_id)
    if not unit or unit:GetUnitType() ~= GameInfoTypes["UNIT_HETAIROI"] then return end

    local current_era = player:GetCurrentEra()
    
    -- Set initial combat strength based on era
    local era_types = {
        [GameInfoTypes["ERA_ANCIENT"]] = {strength = 12, xp = 0},
        [GameInfoTypes["ERA_CLASSICAL"]] = {strength = 16, xp = 0},
        [GameInfoTypes["ERA_MEDIEVAL"]] = {strength = 23, xp = 15},
        [GameInfoTypes["ERA_RENAISSANCE"]] = {strength = 30, xp = 30},
        [GameInfoTypes["ERA_INDUSTRIAL"]] = {strength = 40, xp = 30},
        [GameInfoTypes["ERA_MODERN"]] = {strength = 65, xp = 30},
        [GameInfoTypes["ERA_POSTMODERN"]] = {strength = 80, xp = 30},
        [GameInfoTypes["ERA_FUTURE"]] = {strength = 100, xp = 60}
    }

    -- Set initial strength and award XP
    unit:SetBaseCombatStrength(era_types[current_era].strength)
    unit:ChangeExperience(era_types[current_era].xp)
    -- Check if unit is promotion ready
    if era_types[current_era].xp >= 15 then
        unit:SetPromotionReady(true)
    end
   

    -- Set era-based promotions
    if current_era >= GameInfoTypes["ERA_INDUSTRIAL"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_MARCH"], true)
    end
    if current_era >= GameInfoTypes["ERA_MODERN"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_ARMOR_VS_LAND"], true)
    end
    if current_era >= GameInfoTypes["ERA_POSTMODERN"] then
        unit:SetHasPromotion(GameInfoTypes["PROMOTION_EXTRA_MOVES_I"], true)
        -- Refresh movement points to apply movement bonuses immediately
        unit:SetMoves(unit:MaxMoves())
    end
end

-------------------------------------------------------------------------------------------------------------------------
GameEvents.PlayerDoTurn.Add(lekmod_macedonia_hetairoi_era_scaling)
-- UnitCreated is lekmod specific event
GameEvents.UnitCreated.Add(lekmod_macedonia_hetairoi_created)
GameEvents.TeamSetEra.Add(lekmod_macedonia_hetairoi_era_scaling)

