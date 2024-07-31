-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_GOLDEN_HORDE"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Golden Horde UA.
------------------------------------------------------------------------------------------------------------------------
local function HordeUA(playerID)

	local player = Players[playerID]

	if player:GetCivilizationType() ~= this_civ and not player:IsAlive() then return end
   for city in player:Cities() do

      if city:IsPuppet() then
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_HORDE_TRAIT"], 1)
         local pop = city:GetPopulation()
         local unhappy = pop + 3;
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_HORDE_TRAIT_HAPPY"], unhappy/3)
      else
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_HORDE_TRAIT"], 0)
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_HORDE_TRAIT_HAPPY"], 0)
      end

   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerDoTurn.Add(HordeUA);
end