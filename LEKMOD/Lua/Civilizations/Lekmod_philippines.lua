
-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_PHILIPPINES"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Philippines UA. Add a building to a new city that gives +1 population to the city until the maximum is reached.
------------------------------------------------------------------------------------------------------------------------
local dummy_building = GameInfoTypes["BUILDING_PHILIPPINES_TRAIT"]
local bonus_cities_amount = 2

function lekmod_philippine_expand_population(player_id, iX, iY)
   local player = Players[player_id]
   if player:GetCivilizationType() == this_civ
      and player:IsAlive()
      and player:CountNumBuildings(dummy_building) < bonus_cities_amount
   then
      local plot = Map.GetPlot(iX, iY)
      local city = plot:GetPlotCity()
      if not city:IsCapital() then
         city:SetNumRealBuilding(dummy_building, 1)
      end
   end
end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerCityFounded.Add(lekmod_philippine_expand_population)
end