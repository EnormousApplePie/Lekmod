-- PatronageChanges
-- Author: Fruitstrike
-- DateCreated: 4/28/2015 12:14:46 AM
--------------------------------------------------------------

function NQMP_UpdateConsulates(iPlayer)

	local player = Players[iPlayer]
	local pCapital = player:GetCapitalCity()

	-- destroy all extraneous consulates outside of capital
	for pCity in player:Cities() do
		if (pCity ~= pCapital) then
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_RENAISSANCE_CONSULATE"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_RENAISSANCE_CONSULATE"], 0)
			end
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_INDUSTRIAL_CONSULATE"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_INDUSTRIAL_CONSULATE"], 0)
			end
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_MODERN_CONSULATE"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MODERN_CONSULATE"], 0)
			end
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_ATOMIC_CONSULATE"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ATOMIC_CONSULATE"], 0)
			end
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_INFORMATION_CONSULATE"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_INFORMATION_CONSULATE"], 0)
			end
		end
	end --for pCity in player:Cities() do

	-- add consulates to capital if needed, based on current era
	local eraID = player:GetCurrentEra()
	-- always get the first consulate regardless of era
	if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_RENAISSANCE_CONSULATE"]) < 1) then
		pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_RENAISSANCE_CONSULATE"], 1)
	end

	-- Industrial era consulate
	if (eraID >= GameInfo.Eras["ERA_INDUSTRIAL"].ID) then
		if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_INDUSTRIAL_CONSULATE"]) < 1) then
			pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_INDUSTRIAL_CONSULATE"], 1)
		end
	end

	-- Modern era consulate
	if (eraID >= GameInfo.Eras["ERA_MODERN"].ID) then
		if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_MODERN_CONSULATE"]) < 1) then
			pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_MODERN_CONSULATE"], 1)
		end
	end

	-- Atomic era consulate
	if (eraID >= GameInfo.Eras["ERA_POSTMODERN"].ID) then
		if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_ATOMIC_CONSULATE"]) < 1) then
			pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_ATOMIC_CONSULATE"], 1)
		end
	end

	-- Information era consulate
	if (eraID >= GameInfo.Eras["ERA_FUTURE"].ID) then
		if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_INFORMATION_CONSULATE"]) < 1) then
			pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_INFORMATION_CONSULATE"], 1)
		end
	end

end

function NQMP_Patronage_PlayerDoTurn(iPlayer)

	local player = Players[iPlayer]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_CONSULATES"].ID)) then
			NQMP_UpdateConsulates(iPlayer)
		end
	end

end
GameEvents.PlayerDoTurn.Add(NQMP_Patronage_PlayerDoTurn);

function NQMP_Patronage_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
	if (policyID == GameInfo.Policies["POLICY_CONSULATES"].ID) then
		NQMP_UpdateConsulates(playerID)
	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_Patronage_OnPolicyAdopted)