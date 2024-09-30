-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_PHOENICIAN"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Phoenicia UA. Add a building to a new city that gives +1 Population, +1 Happiness and +40 Gold.
------------------------------------------------------------------------------------------------------------------------
function lekmod_phoenicia_ua_new_city_bonus(player_id, x, y)

   local player = Players[player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ or
   not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_OPTICS"]) then return end

   local plot = Map.GetPlot(x, y)
   local city = plot:GetPlotCity()
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_PHOENICIAN_TRAIT"], 1)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.PlayerCityFounded.Add(lekmod_phoenicia_ua_new_city_bonus)
end