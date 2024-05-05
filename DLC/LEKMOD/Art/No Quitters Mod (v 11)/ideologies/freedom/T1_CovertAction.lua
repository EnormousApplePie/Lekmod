-- T1_CovertAction
-- Author: Fruitstrike
-- DateCreated: 2/14/2016 9:59:48 PM
--------------------------------------------------------------

function NQMP_UpdateT1CovertAction(iPlayer)

	local player = Players[iPlayer]
	local pCapital = player:GetCapitalCity()

	-- destroy extraneous Covert Ops HQ
	for pCity in player:Cities() do
		if (pCity ~= pCapital) then
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_T1_COVERT_ACTION"]) > 0) then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_T1_COVERT_ACTION"], 0)
			end
		end
	end 

	-- add Covert Ops HQ to capital if needed
	if (pCapital:GetNumRealBuilding(GameInfoTypes["BUILDING_T1_COVERT_ACTION"]) < 1) then
		pCapital:SetNumRealBuilding(GameInfoTypes["BUILDING_T1_COVERT_ACTION"], 1)
	end

end

function NQMP_T1_Covert_Action_PlayerDoTurn(iPlayer)

	local player = Players[iPlayer]
	if (player:IsEverAlive()) then
		if (player:HasPolicy(GameInfo.Policies["POLICY_COVERT_ACTION"].ID)) then
			NQMP_UpdateT1CovertAction(iPlayer)
		end
	end

end
GameEvents.PlayerDoTurn.Add(NQMP_T1_Covert_Action_PlayerDoTurn);

function NQMP_T1_Covert_Action_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
	if (policyID == GameInfo.Policies["POLICY_COVERT_ACTION"].ID) then
		NQMP_UpdateT1CovertAction(playerID)
	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_T1_Covert_Action_OnPolicyAdopted)