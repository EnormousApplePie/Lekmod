-- T1_EconomicUnion
-- Author: Fruitstrike
-- DateCreated: 2/14/2016 8:45:43 PM
--------------------------------------------------------------

function addT1EconomicUnionBuilding(player, iX, iY)
	local plot = Map.GetPlot(iX, iY);
	local cCity = plot:GetPlotCity();
	cCity:SetNumRealBuilding(GameInfoTypes["DUMMY_BUILDING_T1_FREE_TRADE"], 1)
end

function NQMP_Freedom_T1_Free_Trade_OnCityCaptureComplete(iOldOwner, bIsCapital, iX, iY, iNewOwner, iPop, bConquest)
	local player = Players[iNewOwner]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_FREE_TRADE"].ID)) then
			addT1EconomicUnionBuilding(player, iX, iY)
		end
	end
end
GameEvents.CityCaptureComplete.Add(NQMP_Freedom_T1_Free_Trade_OnCityCaptureComplete)

function NQMP_Freedom_T1_Free_Trade_OnFoundCity(iPlayer, iX, iY)
	local player = Players[iPlayer]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_FREE_TRADE"].ID)) then
			addT1EconomicUnionBuilding(player, iX, iY)
		end
	end
end
GameEvents.PlayerCityFounded.Add(NQMP_Freedom_T1_Free_Trade_OnFoundCity)

function NQMP_Freedom_T1_Free_Trade_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
	if (policyID == GameInfo.Policies["POLICY_FREE_TRADE"].ID) then
		for loopCity in player:Cities() do
			loopCity:SetNumRealBuilding(GameInfoTypes["DUMMY_BUILDING_T1_FREE_TRADE"], 1)
		end
	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_Freedom_T1_Free_Trade_OnPolicyAdopted)