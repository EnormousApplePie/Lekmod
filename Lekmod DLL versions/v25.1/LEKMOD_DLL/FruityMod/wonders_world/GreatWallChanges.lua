function NQMP_GreatWall_PlayerDoTurn(iPlayer)

	local player = Players[iPlayer]
	if (player:IsEverAlive()) then

		local hasGreatWall = false
		for pCity in player:Cities() do
			if (pCity:GetNumRealBuilding(GameInfoTypes["BUILDING_GREAT_WALL"]) > 0) then
				hasGreatWall = true
			end
		end

		if (hasGreatWall) then
			for pCity in player:Cities() do
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_WATCH_TOWER"], 1)
			end
			GameEvents.PlayerDoTurn.Remove(NQMP_GreatWall_PlayerDoTurn);
		end

	end
end
GameEvents.PlayerDoTurn.Add(NQMP_GreatWall_PlayerDoTurn);