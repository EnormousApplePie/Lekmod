include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_VENEZ"] -- This is the Civilization name for Venice
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Venice UA: Add a Trade Route at Compass
------------------------------------------------------------------------------------------------------------------------
function lekmod_venice_route_compass(team_id, tech_id)

	for _, player in pairs(Players) do
		if player:IsAlive() and player:GetTeam() == team_id and player:GetCivilizationType() == this_civ then
			if tech_id == compassTech then
				player:ChangeNumMiscTradeRoutes(1)
			end
		end
	end
end

------------------------------------------------------------------------------------------------------------------------
-- Check if the civilization is active and add the event listener
------------------------------------------------------------------------------------------------------------------------
if LekmodUtilities:is_civilization_active(this_civ) then
	GameEvents.TeamTechResearched.Add(lekmod_venice_route_compass)
end
