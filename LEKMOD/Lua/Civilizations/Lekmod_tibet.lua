-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_TIBET"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)
------------------------------------------------------------------------------------------------------------------------
-- Tibet UA. Give cities founded by a Dalai Lama a Trait_Building after Drama, account for capturing cities that were founded by a Dalai Lama and cities founded after Drama.
------------------------------------------------------------------------------------------------------------------------
function lekmod_tibet_add_building(player)
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ or
   not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_DRAMA"]) then return end

   for city in player:Cities() do
       if city then
           city:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET"], 0) -- Remove from all
           local SettleUnit = city:GetSettlerUnit();
           if SettleUnit == GameInfoTypes["UNIT_DALAILAMA"] then
              city:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET"], 1) -- Give back to some
              print("Gave " .. city:GetName() .. " a Stupa. Reason: Settled by Dalai Lama.")
           end
       end
   end
   local capital = player:GetCapitalCity()
   if capital then capital:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET"], 1) end -- give back to capital
   print("Gave " .. capital:GetName() .. " a Stupa. Reason: Is Capital.")
end

function lekmod_tibet_ua_on_tech(team_id)
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
   GameEvents.TeamTechResearched.Add(lekmod_tibet_ua_on_tech)
   GameEvents.PlayerCityFounded.Add(lekmod_tibet_ua_on_city_founded)
   GameEvents.CityCaptureComplete.Add(lekmod_tibet_ua_city_capture)
end