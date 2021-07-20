-- AestheticsChanges
-- Author: Fruitstrike
-- DateCreated: 4/24/2015 1:22:18 AM
--------------------------------------------------------------

function NQMP_Aesthetics_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Aesthetics Finisher
	if	(policyID == GameInfo.Policies["POLICY_FINE_ARTS"].ID and player:HasPolicy(GameInfo.Policies["POLICY_FLOURISHING_OF_ARTS"].ID)) or
		(policyID == GameInfo.Policies["POLICY_FLOURISHING_OF_ARTS"].ID and player:HasPolicy(GameInfo.Policies["POLICY_FINE_ARTS"].ID)) then

		-- The player has finished Aesthetics. Add a Grand Monument to the capital, which gives 1 free Social Policy.
		local pCity = player:GetCapitalCity();
		pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GRAND_MONUMENT"], 1);

	end

end
GameEvents.PlayerAdoptPolicy.Add(NQMP_Aesthetics_OnPolicyAdopted);