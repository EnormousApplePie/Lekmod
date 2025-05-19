-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_TIBET"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Tibet UA. Gain a certain amount of dummy buildings with that depends on how many eras the player has advanced
------------------------------------------------------------------------------------------------------------------------
function lekmod_tibet_add_building(player)

   if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end
   local amount = (player:GetCurrentEra() - 1) or 0
   if amount < 0 then amount = 0 end
   for city in player:Cities() do
      if city then
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET2"], 0)
      end
   end
   local capital = player:GetCapitalCity()
   if capital then capital:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET2"], amount) end

end

function lekmod_tibet_ua_on_era(team_id)

   for _, player in pairs(Players) do
      if player:GetTeam() == team_id then lekmod_tibet_add_building(player) end
   end

end

function lekmod_tibet_ua_on_city_founded(player_id)

   local player = Players[player_id]
   lekmod_tibet_add_building(player)

end

function lekmod_tibet_ua_city_capture(old_owner_id, is_capital, x, y, new_owner_id)

   local new_player = Players[new_owner_id]
   local old_player = Players[old_owner_id]
   if new_player:GetCivilizationType() ~= this_civ then
      lekmod_tibet_add_building(old_player)
   else
      lekmod_tibet_add_building(new_player)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.TeamSetEra.Add(lekmod_tibet_ua_on_era)
   GameEvents.PlayerCityFounded.Add(lekmod_tibet_ua_on_city_founded)
   GameEvents.CityCaptureComplete.Add(lekmod_tibet_ua_city_capture)
end