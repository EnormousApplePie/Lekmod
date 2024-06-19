-- Author: EnormousApplePie

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

------------------------------------------------------------------------------------------------------------------------
-- Improvement Adjacency bonuses
------------------------------------------------------------------------------------------------------------------------
local function improvement_adjacent_bonus(player_id)

    print("Improvement Adjacency bonuses")
    local player = Players[player_id]
    if not player:IsAlive() then return end

    if GameInfo.Improvement_Adjacency_Yields == nil then print("table does not exist, check the xml!") return end
    for adjacency_data in GameInfo.Improvement_Adjacency_Yields() do
        local improvement = GameInfoTypes[adjacency_data.ImprovementType]
        local other_improvement = GameInfoTypes[adjacency_data.OtherImprovementType]
        local this_civ = nil
        if adjacency_data.CivilizationRequired ~= nil then 
            this_civ = GameInfoTypes[adjacency_data.CivilizationRequired]
        end
        local amount = adjacency_data.Amount
        local yield_type = adjacency_data.YieldType
        local yield_amount = adjacency_data.Yield
        local is_per_amount = adjacency_data.IsPerAmount
        local player = Players[player_id]
        print(is_per_amount)
        if this_civ ~= nil and player:GetCivilizationType() ~= this_civ then -- Skip
        else
            print("Improvement Adjacency bonuses 2")
            for city in player:Cities() do
                if city then
                    city:SetImprovementAdjacencyBonus(improvement, other_improvement, amount, yield_type, yield_amount, is_per_amount)
                end
            end
        end
      
    end

end
local function improvement_adjacent_bonus_on_capture(old_player_id, _, x, y, new_player_id)

    improvement_adjacent_bonus(new_player_id)

end
------------------------------------------------------------------------------------------------------------------------
GameEvents.BuildFinished.Add(improvement_adjacent_bonus)
GameEvents.CityCaptureComplete.Add(improvement_adjacent_bonus_on_capture)