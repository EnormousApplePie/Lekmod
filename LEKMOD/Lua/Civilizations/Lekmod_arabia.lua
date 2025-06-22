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
		}
	}

	local building_yield_change = {
		
		[GameInfoTypes["YIELD_CULTURE"]] = 1,
		[GameInfoTypes["YIELD_GOLD"]] = 1
		
	}

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
		}
	}

	for city in player:Cities() do
		local city_id = city:GetID()
		
		-- Initialize city tracker if it doesn't exist
		if not arabian_bonus_tracker[player_id][city_id] then
			arabian_bonus_tracker[player_id][city_id] = {}
		end
		
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
					-- Check if we've already added the bonus for this building in this city
					if not arabian_bonus_tracker[player_id][city_id][building_type] then
						local building_class = GameInfo.Buildings[building_type].BuildingClass
						for yield_type, amount in pairs(building_yield_change) do
							local current_yield = city:getBuildingYieldChange(GameInfoTypes[building_class], yield_type)
							-- Add the bonus on top of existing yields
							city:setBuildingYieldChange(GameInfoTypes[building_class], yield_type, current_yield + amount)
						end
						-- Mark this building as having received the bonus
						arabian_bonus_tracker[player_id][city_id][building_type] = true
					end
				else
					-- If the city no longer has the required luxury, remove the bonus and reset tracker
					if arabian_bonus_tracker[player_id][city_id][building_type] then
						local building_class = GameInfo.Buildings[building_type].BuildingClass
						for yield_type, amount in pairs(building_yield_change) do
							local current_yield = city:getBuildingYieldChange(GameInfoTypes[building_class], yield_type)
							-- Remove the bonus
							city:setBuildingYieldChange(GameInfoTypes[building_class], yield_type, math.max(0, current_yield - amount))
						end
						-- Reset the tracker
						arabian_bonus_tracker[player_id][city_id][building_type] = false
					end
				end
			else
				-- If the building is no longer present, reset the tracker
				if arabian_bonus_tracker[player_id][city_id][building_type] then
					arabian_bonus_tracker[player_id][city_id][building_type] = false
				end
			end
		end
	end
end

------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.BuildFinished.Add(lekmod_arabian_dummy_resource_yields)
	GameEvents.PlayerDoTurn.Add(lekmod_arabian_dummy_resource_yields)
end
