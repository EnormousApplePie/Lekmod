-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_TIBET"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Tibet UA. Gain a certain amount of dummy buildings with that depends on how many eras the player has advanced
------------------------------------------------------------------------------------------------------------------------
local function add_building(player)

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

local function tibet_ua_on_era(team_id)

   for _, player in pairs(Players) do
      if player:GetTeam() == team_id then add_building(player) end
   end

end

local function tibet_ua_on_turn(player_id)

   local player = Players[player_id]
   add_building(player)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.TeamSetEra.Add(tibet_ua_on_era)
   GameEvents.PlayerDoTurn.Add(tibet_ua_on_turn)
end