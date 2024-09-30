-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_BOLIVIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

-- Write to the save file for remembering the last expended great person
Lek_SaveData = Modding.OpenSaveData()

Lek_Properties = {}

function lekmod_bolivia_get_persistent_property(name)
   if not Lek_Properties[name] then
      Lek_Properties[name] = Lek_SaveData.GetValue(name)
   end
   return Lek_Properties[name]
end

function lekmod_bolivia_set_persistent_data(name, value)
   if lekmod_bolivia_get_persistent_property(name) == value then return end
   Lek_SaveData.SetValue(name, value)
   Lek_Properties[name] = value
end


------------------------------------------------------------------------------------------------------------------------
-- Bolivia UA. Add a dummy building that either gives +1 production or +1 food to mines globally when
-- either a writer or artist is expended. Save this data to the save file so that it can be remembered
------------------------------------------------------------------------------------------------------------------------
function lekmod_bolivia_is_person_expended(player_id, unit_id, arg3, arg4, new_player_id)

   local artist_unit_id = GameInfoTypes["UNIT_ARTIST"]
   local writer_unit_id = GameInfoTypes["UNIT_WRITER"]

   -- CityCaptureComplete passes different arguments than the other events so we need to check for that
   local player = Players[player_id]
   local new_player
   if new_player_id ~= nil then
	   new_player = Players[new_player_id]
   end

	local capital = player:GetCapitalCity()
   if (not player:IsAlive()) or player:GetCivilizationType() ~= this_civ then
      if new_player ~= nil and new_player:IsAlive() and new_player:GetCivilizationType() == this_civ then
         capital = new_player:GetCapitalCity()
         player_id = new_player_id
      else
         return
      end
   end

   -- Great Person expended event only has 2 arguments.
   if (unit_id == artist_unit_id or unit_id == writer_unit_id) then
      lekmod_bolivia_set_persistent_data("bolivia_last_expended", unit_id .. player_id)
   end

   local bolivia_last_expended = tostring(lekmod_bolivia_get_persistent_property("bolivia_last_expended") or 0)

   if bolivia_last_expended == (artist_unit_id .. player_id) then
      capital:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 1)
      capital:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 0)
      if capital:IsHuman() and unit_id == artist_unit_id and Game.GetActivePlayer() == player_id  then
         Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_RIGHT"))
      end
   end

   if bolivia_last_expended == (writer_unit_id .. player_id) then
      capital:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 1)
      capital:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 0)
      if capital:IsHuman() and unit_id == writer_unit_id and Game.GetActivePlayer() == player_id then
         Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_LEFT"))
      end
   end

   -- Make sure to delete the building in anything other than the capital to avoid stacking effects
   if new_player ~= nil then
      lekmod_bolivia_retain_building_capture(new_player_id)
   else
      lekmod_bolivia_retain_building_capture(player_id)
   end

end

function lekmod_bolivia_retain_building_capture(player_id)

   local player = Players[player_id]
   local capital = player:GetCapitalCity()

   for cities in player:Cities() do
      if cities ~= capital then
         cities:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 0)
         cities:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 0)
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
-- Bolivia UU. Colorado. Add a combat strength bonus to the Colorado unit based on excess happiness.
------------------------------------------------------------------------------------------------------------------------
-- Note: Might want to make a lua hook for when a player's happiness changes to update the combat strength better
function lekmod_bolivia_uu_combat_strength(player_id, unit_id)


   local colorado_unit_id = GameInfoTypes["UNIT_COLORADO"]
   if unit_id ~= nil and unit_id ~= colorado_unit_id then return end

	local player = Players[player_id]
	local colorado_base_strength = GameInfo.Units[colorado_unit_id].Combat
	local happiness_number = player:GetExcessHappiness()
	local bonus_combat_strength = math.min(LekmodUtilities:get_round(happiness_number/5))
   local strength_to_set = colorado_base_strength
	if (bonus_combat_strength > 0 ) then
		strength_to_set = colorado_base_strength + (bonus_combat_strength * 2)
	end
	for unit in player:Units() do
		if unit:GetUnitType() == colorado_unit_id then
			unit:SetBaseCombatStrength(strength_to_set)
		end
	end
end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	--GameEvents.PlayerDoTurn.Add(bolivia_uu_combat_strength)
   -- Note: PlayerHappinessChanged is a Lekmod specific event
	
	GameEvents.GreatPersonExpended.Add(lekmod_bolivia_is_person_expended)
   GameEvents.CityCaptureComplete.Add(lekmod_bolivia_is_person_expended)
   GameEvents.PlayerCityFounded.Add(lekmod_bolivia_is_person_expended)
end
GameEvents.PlayerHappinessChanged.Add(lekmod_bolivia_uu_combat_strength)
GameEvents.UnitCreated.Add(lekmod_bolivia_uu_combat_strength)
------------------------------------------------------------------------------------------------------------------------