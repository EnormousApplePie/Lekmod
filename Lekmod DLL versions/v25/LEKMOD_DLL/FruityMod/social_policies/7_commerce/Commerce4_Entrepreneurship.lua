-- 7_CommerceChanges
-- Author: Fruitstrike
-- DateCreated: 5/4/2015 10:37:07 PM
--------------------------------------------------------------

function NQMP_UpdateGrandBazaar(iPlayer)
	local player = Players[iPlayer]
	local pCapital = player:GetCapitalCity()

	-- destroy extraneous Grand Bazaar
	for pCity in player:Cities() do
		if (pCity ~= pCapital) then
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_GRAND_BAZAAR"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GRAND_BAZAAR"], 0)
			end
		end
	end

	-- add Grand Bazaar to capital if needed
	if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_GRAND_BAZAAR"]) < 1) then
		pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_GRAND_BAZAAR"], 1)
	end

end

function NQMP_Commerce_PlayerDoTurn(iPlayer)

	local player = Players[iPlayer]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_ENTREPRENEURSHIP"].ID)) then
			NQMP_UpdateGrandBazaar(iPlayer)
		end
	end

end
GameEvents.PlayerDoTurn.Add(NQMP_Commerce_PlayerDoTurn);

function NQMP_Commerce_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
	if (policyID == GameInfo.Policies["POLICY_ENTREPRENEURSHIP"].ID) then
		NQMP_UpdateGrandBazaar(playerID)
	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_Commerce_OnPolicyAdopted)