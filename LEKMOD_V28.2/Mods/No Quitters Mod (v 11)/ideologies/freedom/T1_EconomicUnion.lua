-- T1_EconomicUnion
-- Author: Fruitstrike
-- DateCreated: 2/14/2016 8:45:43 PM
--------------------------------------------------------------

function addT1EconomicUnionBuilding(player, iX, iY)
	local plot = Map.GetPlot(iX, iY);
	local cCity = plot:GetPlotCity();
	cCity:SetNumRealBuilding(GameInfoTypes["BUILDING_T1_ECONOMIC_UNION"], 1)
end

function NQMP_Freedom_T1_Economic_Union_OnCityCaptureComplete(iOldOwner, bIsCapital, iX, iY, iNewOwner, iPop, bConquest)
	local player = Players[iNewOwner]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_ECONOMIC_UNION"].ID)) then
			addT1EconomicUnionBuilding(player, iX, iY)
		end
	end
end
GameEvents.CityCaptureComplete.Add(NQMP_Freedom_T1_Economic_Union_OnCityCaptureComplete)

function NQMP_Freedom_T1_Economic_Union_OnFoundCity(iPlayer, iX, iY)
	local player = Players[iPlayer]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_ECONOMIC_UNION"].ID)) then
			addT1EconomicUnionBuilding(player, iX, iY)
		end
	end
end
GameEvents.PlayerCityFounded.Add(NQMP_Freedom_T1_Economic_Union_OnFoundCity)

function NQMP_Freedom_T1_Economic_Union_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
	if (policyID == GameInfo.Policies["POLICY_ECONOMIC_UNION"].ID) then
		for loopCity in player:Cities() do
			loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_T1_ECONOMIC_UNION"], 1)
		end
	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_Freedom_T1_Economic_Union_OnPolicyAdopted)