-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MOORS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Moors UA. Give every city a dummy building that gives a production% bonus to buildings depending on the era.
------------------------------------------------------------------------------------------------------------------------
local trait_building = GameInfoTypes["BUILDING_MOORS_TRAIT2"]
function lekmod_moors_on_era_ua(team_id, era_id)

	for _, player in pairs(Players) do

      if player:IsAlive()
      and player:GetCivilizationType() == this_civ
      and player:GetTeam() == team_id
      then
         -- for the desired production% amount we can simply add the building twice
         local amount_buildings = 0
         if era_id == GameInfoTypes["ERA_MEDIEVAL"] then
            amount_buildings = 2
         elseif era_id == GameInfoTypes["ERA_RENAISSANCE"] then
            amount_buildings = 1
         end
         for city in player:Cities() do
            city:SetNumRealBuilding(trait_building, amount_buildings)
         end
      end

   end

end

function lekmod_moors_on_turn_ua(player_id)

   local player = Players[player_id]
   if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end
   for city in player:Cities() do
      local amount_buildings = 0
      if player:GetCurrentEra() == GameInfoTypes["ERA_MEDIEVAL"] then
         amount_buildings = 2
      elseif player:GetCurrentEra() == GameInfoTypes["ERA_RENAISSANCE"] then
         amount_buildings = 1
      end
      city:SetNumRealBuilding(trait_building, amount_buildings)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.TeamSetEra.Add(lekmod_moors_on_era_ua)
   GameEvents.PlayerDoTurn.Add(lekmod_moors_on_turn_ua)
end