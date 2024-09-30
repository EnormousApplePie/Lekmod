-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MONGOL"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Mongolia UA. Add a dummy building at chivalary to each city that spawns a horse resource.
------------------------------------------------------------------------------------------------------------------------
function lekmod_mongol_ua_horses_add(team_id, tech_id)

	for _, player in pairs(Players) do
		if player:GetCivilizationType() == this_civ then
			if player:GetTeam() == team_id then
				if (tech_id == GameInfoTypes["TECH_CHIVALRY"]) then
					for city in player:Cities() do
					   city:SetNumRealBuilding(GameInfoTypes["BUILDING_TOGTVORTOI"], 1)
					end
				end
			end
		end
	end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
    GameEvents.TeamTechResearched.Add(lekmod_mongol_ua_horses_add)
end