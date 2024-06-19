-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_VATICAN"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Vatican UA. Add a courthouse to cities that are occupied or puppeted and have the player's religion
------------------------------------------------------------------------------------------------------------------------
local function ua_courthouse(player_id, _ , x, y)

	local player = Players[player_id]
   local player_religion = player:GetReligionCreatedByPlayer();
   local city = Map.GetPlot(x, y):GetPlotCity()

	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   if not (city:IsOccupied() or city:IsPuppet()) 
   or city:GetReligiousMajority() ~= player_religion then return end

   city:SetNumRealBuilding(GameInfoTypes["BUILDING_COURTHOUSE"], 1);

end

local function ua_courthouse_on_capture(_, _, x, y, new_player_id)

   ua_courthouse(new_player_id, _, x, y)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.CityConvertsReligion.Add(ua_courthouse)
	GameEvents.CityCaptureComplete.Add(ua_courthouse_on_capture)
end