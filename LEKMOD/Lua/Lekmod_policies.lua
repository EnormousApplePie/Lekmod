------------------------------------------------------------------------------------------------------------------------
-- Resettlements. Add a few buildings to newly founded cities if the player has the policy
------------------------------------------------------------------------------------------------------------------------
function lekmod_resettlements_policy_new_buildings(player_id, x, y)

    local player = Players[player_id]
    if not player:HasPolicy(GameInfo.Policies["POLICY_RESETTLEMENT"].ID) then return end

    for loopCity in player:Cities() do
        if (loopCity:GetX() == x and loopCity:GetY() == y) then
            --Note: SetNumRealBuildingClass is a lekmod method. It is not available in the base game
            loopCity:SetNumRealBuildingClass(GameInfoTypes["BUILDINGCLASS_WORKSHOP"], 1)
            loopCity:SetNumRealBuildingClass(GameInfoTypes["BUILDINGCLASS_GRANARY"], 1)
            loopCity:SetNumRealBuildingClass(GameInfoTypes["BUILDINGCLASS_AQUEDUCT"], 1)
            loopCity:SetNumRealBuildingClass(GameInfoTypes["BUILDINGCLASS_MONUMENT"], 1)
            loopCity:SetNumRealBuildingClass(GameInfoTypes["BUILDINGCLASS_LIBRARY"], 1)
        end
    end

end
GameEvents.PlayerCityFounded.Add(lekmod_resettlements_policy_new_buildings)
------------------------------------------------------------------------------------------------------------------------
-- Policy_FreePromotionUnitCombats. Give a free promotion to units for specific combat classes as put in the xml table
------------------------------------------------------------------------------------------------------------------------
function lekmod_policy_free_promotion_unit_combats(player_id, unit_id)

    local player = Players[player_id]
    local unit = player:GetUnitByID(unit_id)
    if not player:IsAlive() or not unit then return end

    for row in GameInfo.Policy_FreePromotionUnitCombats() do

        local policy_id = GameInfoTypes[row.PolicyType]
        local promotion_id = GameInfoTypes[row.PromotionType]
        local combat_class_id = GameInfoTypes[row.UnitCombatType]

        if player:HasPolicy(policy_id)
        and unit:GetUnitCombatType() == combat_class_id
        and (not unit:IsHasPromotion(promotion_id)) then

            unit:SetHasPromotion(promotion_id, true)

        end

    end

end

function lekmod_policy_free_promotion_unit_combats_on_adopt(player_id)

    local player = Players[player_id]

    -- Apply to all the right currently existing units once upon adopting the policy
    for unit in player:Units() do
        if unit then
            lekmod_policy_free_promotion_unit_combats(player_id, unit:GetID())
        end
    end

end
GameEvents.PlayerAdoptPolicy.Add(lekmod_policy_free_promotion_unit_combats_on_adopt)
GameEvents.UnitCreated.Add(lekmod_policy_free_promotion_unit_combats)
------------------------------------------------------------------------------------------------------------------------