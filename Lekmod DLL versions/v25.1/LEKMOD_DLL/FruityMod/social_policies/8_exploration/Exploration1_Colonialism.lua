-- Add Governor's Mansion to newly founded cities
function NQMP_Exploration_Colonialism_OnFoundCity(iPlayer, iCityX, iCityY)
	local player = Players[iPlayer]
	if (player:HasPolicy(GameInfo.Policies["POLICY_MERCHANT_NAVY"].ID)) then
		for loopCity in player:Cities() do
			if (loopCity:GetX() == iCityX) then
				if (loopCity:GetY() == iCityY) then
					loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GOVERNORS_MANSION"], 1)
				end
			end
		end
	end
end
GameEvents.PlayerCityFounded.Add(NQMP_Exploration_Colonialism_OnFoundCity)