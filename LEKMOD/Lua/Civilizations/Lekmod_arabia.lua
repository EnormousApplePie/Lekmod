-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_ARABIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

-- Track which cities have already received the bonus for each building type
local arabian_bonus_tracker = {}

------------------------------------------------------------------------------------------------------------------------
-- Arabia UA: Add a Dummy Building if the city has a lighthouse and a sea lux and/or a stone works and a stone lux
------------------------------------------------------------------------------------------------------------------------
function lekmod_arabian_dummy_resource_yields(player_id)
	local player = Players[player_id]
	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

	-- Initialize tracker for this player if it doesn't exist
	if not arabian_bonus_tracker[player_id] then
		arabian_bonus_tracker[player_id] = {}
	end

	local buildings = {
		lighthouse = {
			building = GameInfoTypes["BUILDING_LIGHTHOUSE"],
			luxuryType = "sea"
		},
		stoneWorks = {
			building = GameInfoTypes["BUILDING_STONE_WORKS"],
			luxuryType = "stone"
		},
		circus = {
			building = GameInfoTypes["BUILDING_CIRCUS"],
			luxuryType = "circus"
		},
		factory = {
			building = GameInfoTypes["BUILDING_FACTORY"],
			luxuryType = "factory"
		}
	}

	local building_dummy = GameInfoTypes["BUILDING_DUMMY_ARABIA"]

	local luxury_resources = {
		sea = {
			[GameInfoTypes["RESOURCE_WHALE"]] = true,
			[GameInfoTypes["RESOURCE_PEARLS"]] = true,
			[GameInfoTypes["RESOURCE_CORAL"]] = true,
			[GameInfoTypes["RESOURCE_CRAB"]] = true
		},
		stone = {
			[GameInfoTypes["RESOURCE_MARBLE"]] = true,
			[GameInfoTypes["RESOURCE_OBSIDIAN"]] = true
		},
		circus = {
			[GameInfoTypes["RESOURCE_IVORY"]] = true
		},
		factory = {
			[GameInfoTypes["RESOURCE_RUBBER"]] = true
		}
	}

	for city in player:Cities() do

		-- Count how many luxury categories are present in this city
		local luxury_categories_found = 0
		local active_categories = {}
		
		for building, data in pairs(buildings) do
			local building_type = data.building
			
			if city:IsHasBuilding(building_type) then
				-- Check if city has the required luxury resource
				local has_required_luxury = false
				for res_id, _ in pairs(luxury_resources[data.luxuryType]) do
					if city:IsHasResourceLocal(res_id, false) then
						has_required_luxury = true
						break
					end
				end
				
				if has_required_luxury then
					luxury_categories_found = luxury_categories_found + 1
					active_categories[data.luxuryType] = true
				end
			end
		end
		
		-- Set the number of dummy buildings based on luxury categories found
		if luxury_categories_found > 0 then
			city:SetNumRealBuilding(building_dummy, luxury_categories_found)
		else
			city:SetNumRealBuilding(building_dummy, 0)
		end
		
	end
end

------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.BuildFinished.Add(lekmod_arabian_dummy_resource_yields)
	GameEvents.PlayerDoTurn.Add(lekmod_arabian_dummy_resource_yields)
end
