-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MOORS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Moors UA. Give every city a dummy building that gives a production% bonus to buildings depending on the era.
------------------------------------------------------------------------------------------------------------------------
local trait_building = GameInfoTypes["BUILDING_MOORS_TRAIT2"]
local function moors_on_era_ua(team, era)
   print("moors_on_era_ua", team, era)
	for _, player in pairs(Players) do

      if player:IsAlive()
      and player:GetCivilizationType() == this_civ
      and player:GetTeam() == team
      then
         -- for the desired production% amount we can simply add the building twice
         local amount_buildings = 0
         if era == GameInfoTypes["ERA_MEDIEVAL"] then
            amount_buildings = 2
         elseif era == GameInfoTypes["ERA_RENAISSANCE"] then
            amount_buildings = 1
         end
         print("moors_on_era_ua", amount_buildings, era)
         for city in player:Cities() do
            city:SetNumRealBuilding(trait_building, amount_buildings)
         end
      end

   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.TeamSetEra.Add(moors_on_era_ua)
end