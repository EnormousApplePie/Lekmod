-- Author: EnormousApplePie

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MUGHALS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

local DUMMY_BUILDING = "BUILDING_DUMMY_MUGHALS"

------------------------------------------------------------------------------------------------------------------------
-- Mughal UA: Foreign religions give benefits to both the Mughal city and the religion's holy city
------------------------------------------------------------------------------------------------------------------------

-- Main function to check for foreign religions and apply the dummy building
function lekmod_ua_mughals_foreign_religion_check(player_id)
   local player = Players[player_id]

   -- Skip if not the Mughal civilization
   if not player or not player:IsAlive() or player:GetCivilizationType() ~= this_civ then
      return
   end

   local dummy_building_id = GameInfoTypes[DUMMY_BUILDING]
   if not dummy_building_id then
      print("Error: Could not find " .. DUMMY_BUILDING)
      return
   end

   -- If the player has a religion, record it
   local mughal_religion = -1
   if player:HasCreatedReligion() then
      mughal_religion = player:GetReligionCreatedByPlayer()
   end

   -- Clear all existing dummy buildings first to reset
   for city in player:Cities() do
      if city:IsHasBuilding(dummy_building_id) then
         city:SetNumRealBuilding(dummy_building_id, 0)
      end
   end

  -- Process current cities
   for city in player:Cities() do
      local city_religion = city:GetReligiousMajority()

      -- Check if city has a proper religion (not just pantheon) and it's foreign
      if city_religion ~= ReligionTypes.RELIGION_PANTHEON and city_religion ~= -1 and city_religion ~= mughal_religion then

         -- Add the dummy building to this city
         city:SetNumRealBuilding(dummy_building_id, 1)

         -- Check all other players' cities for holy cities with this religion
         for other_player_id = 0, GameDefines.MAX_MAJOR_CIVS - 1 do
            local other_player = Players[other_player_id]
            
            -- Skip if it's the Mughal player or if player doesn't exist/isn't alive
            if other_player and other_player:IsAlive() and other_player_id ~= player_id then
               
               for other_city in other_player:Cities() do
                  -- Check if this city is a holy city and if its religion matches the foreign religion in Mughal city
                  if other_city:IsHolyCityForReligion(city_religion) then
                     -- Add the dummy building to this holy city if it doesn't already have it
                     if not other_city:IsHasBuilding(dummy_building_id) then
                        other_city:SetNumRealBuilding(dummy_building_id, 1)
                     end
                  end
               end
            end
         end
      end
   end
end

-- Callback for when a city changes religion
function lekmod_ua_mughals_religion_changed(player_id, city_id, religion_id, majority)
   lekmod_ua_mughals_foreign_religion_check(player_id)
end

-- Callback for turn start to ensure everything is up to date
function lekmod_ua_mughals_turn_start(player_id)
   lekmod_ua_mughals_foreign_religion_check(player_id)
end

-- Check when a city is acquired (captured or traded)
function lekmod_ua_mughals_city_acquired(old_owner_id, new_owner_id, city_id)
   lekmod_ua_mughals_foreign_religion_check(new_owner_id)
end

-- Register events if Mughal civilization is active
if is_active then
   GameEvents.PlayerDoTurn.Add(lekmod_ua_mughals_turn_start)
   GameEvents.CityReligionChanged.Add(lekmod_ua_mughals_religion_changed)
   GameEvents.CityCaptureComplete.Add(lekmod_ua_mughals_city_acquired)
   GameEvents.PlayerCityFounded.Add(function(player_id) lekmod_ua_mughals_foreign_religion_check(player_id) end)
end

