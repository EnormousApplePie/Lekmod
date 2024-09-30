-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_CUBA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Cuba UA. Add a dumy building for every 5 culture present in other players' capitals
------------------------------------------------------------------------------------------------------------------------
function lekmod_cuba_ua_culture(player_id)

   local player = Players[player_id]
   if player:GetCivilizationType() == this_civ then
      local capital = player:GetCapitalCity()
      if not capital then return end
      local number_to_add = 0
      for i = 0, GameDefines.MAX_MAJOR_CIVS - 1, 1 do
         if (player_id ~= i) then
            local other_player = Players[i]
            if other_player ~= nil
            and other_player:GetCapitalCity() ~= nil
            and Teams[player:GetTeam()]:IsHasMet(other_player:GetTeam())
            then
               print(number_to_add)
               local other_capital = other_player:GetCapitalCity()
               number_to_add = (number_to_add + math.floor(other_capital:GetBaseJONSCulturePerTurn() * .20) )
               print(number_to_add)
            end
         end

      end
      capital:SetNumRealBuilding(GameInfoTypes["BUILDING_CUBA_TRAIT_UB2"], number_to_add)
   end

end
------------------------------------------------------------------------------------------------------------------------
-- Cuba UB Dance Hall. Add a dummy building if there is a great work of music in the city.
------------------------------------------------------------------------------------------------------------------------
function lekmod_cuba_building(player_id)
   local player = Players[player_id]
   if player:GetCivilizationType() == this_civ then
      for city in player:Cities() do
         if city:IsHasBuilding(GameInfoTypes["BUILDING_DANCE_HALL"]) then
            if city:GetNumGreatWorksInBuilding(GameInfoTypes.BUILDINGCLASS_OPERA_HOUSE) > 0 then
               city:SetNumRealBuilding(GameInfoTypes["BUILDING_CUBA_TRAIT_UB"], 1)
            else
               city:SetNumRealBuilding(GameInfoTypes["BUILDING_CUBA_TRAIT_UB"], 0)
            end
         end
      end
   end
end
------------------------------------------------------------------------------------------------------------------------
-- Cuba UU. Unlock and give 2 of the unit if the player has adopted an ideology tenet for the first time.
------------------------------------------------------------------------------------------------------------------------
function lekmod_cuba_unit_unlock(player_id, policy_id)
	local player = Players[player_id]
	local capital = player:GetCapitalCity()
	if player:GetCivilizationType() == this_civ then
		local policy_level = GameInfo.Policies[policy_id].Level
		if policy_level == 1 then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"]) then
            local player_amount_free_policies = player:GetNumFreePolicies()
            player:SetNumFreePolicies(player_amount_free_policies + 1)
            player:SetNumFreePolicies(player_amount_free_policies)
            player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
         end
			capital:SetNumRealBuilding(GameInfoTypes["BUILDING_CUBA_TRAIT_UB3"], 1)
		end
	end
end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerDoTurn.Add(lekmod_cuba_building)
   GameEvents.PlayerDoTurn.Add(lekmod_cuba_ua_culture)
   GameEvents.PlayerAdoptPolicy.Add(lekmod_cuba_unit_unlock)
end