-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Globalization. -- Add a permanent building that gives +1 Vote in the world congress.
------------------------------------------------------------------------------------------------------------------------
function add_globalization_dummy_building(playerID)
	for playerID, player in pairs(Players) do
		if player:IsEverAlive() then
			if Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_GLOBALIZATION"]) then
				for city in player:Cities() do
					local pCapital = player:GetCapitalCity()
					pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_GLOBALIZATION"], 1)
				end
			end
		end
	end

end
GameEvents.TeamSetHasTech.Add(add_globalization_dummy_building)
GameEvents.PlayerCityFounded.Add(add_globalization_dummy_building)
GameEvents.CityCaptureComplete.Add(add_globalization_dummy_building)
------------------------------------------------------------------------------------------------------------------------