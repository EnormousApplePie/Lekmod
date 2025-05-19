-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_ARABIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Arabia UA: Add a Dummy Building if the city has a lighthouse and a sea lux and/or a stone works and a stone lux
------------------------------------------------------------------------------------------------------------------------
function lekmod_arabian_dummy_resource_yields(player_id)
	local player = Players[player_id]
	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

	local lighthouse = GameInfoTypes["BUILDING_LIGHTHOUSE"]
	local stoneWorks = GameInfoTypes["BUILDING_STONE_WORKS"]
	local dummyBuilding = GameInfoTypes["BUILDING_DUMMY_ARABIA"]

	local seaLuxury = {
		[GameInfoTypes["RESOURCE_WHALE"]] = true,
		[GameInfoTypes["RESOURCE_PEARLS"]] = true,
		[GameInfoTypes["RESOURCE_CORAL"]] = true,
		[GameInfoTypes["RESOURCE_CRAB"]] = true
	}

	local stoneLuxury = {
		[GameInfoTypes["RESOURCE_MARBLE"]] = true,
		[GameInfoTypes["RESOURCE_OBSIDIAN"]] = true
	}

	for city in player:Cities() do
		local bonusCount = 0

		-- Check Lighthouse + sea resource
		if city:IsHasBuilding(lighthouse) then
			for resID in pairs(seaLuxury) do
				if city:IsHasResourceLocal(resID, false) then
					bonusCount = bonusCount + 1
					break
				end
			end
		end

		-- Check Stone Works + stone resource
		if city:IsHasBuilding(stoneWorks) then
			for resID in pairs(stoneLuxury) do
				if city:IsHasResourceLocal(resID, false) then
					bonusCount = bonusCount + 1
					break
				end
			end
		end

		-- Apply correct dummy building count
		print(city:GetName() .. " has " .. bonusCount .. " dummy buildings.")
		city:SetNumRealBuilding(dummyBuilding, bonusCount)
	end
end

------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.BuildFinished.Add(lekmod_arabian_dummy_resource_yields)
	GameEvents.PlayerDoTurn.Add(lekmod_arabian_dummy_resource_yields)
end
