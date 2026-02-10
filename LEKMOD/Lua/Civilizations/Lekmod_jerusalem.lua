-- Author: AI Assistant (for Lekmod)

include("Lekmod_utilities.lua")
include("PlotIterators")

local this_civ = GameInfoTypes["CIVILIZATION_JERUSALEM"]
local this_building = GameInfoTypes["BUILDING_OUTREMER"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Jerusalem UB Outremer. When built:
-- 1. Spread capital's religion (or pantheon) to this city
-- 2. Grant +2 movement to all units in and adjacent to the city for this turn
------------------------------------------------------------------------------------------------------------------------
function lekmod_jerusalem_outremer_built(player_id, city_id, building_id)

   -- Only trigger for Outremer
   if building_id ~= this_building then return end

   local player = Players[player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   local city = player:GetCityByID(city_id)
   if not city then return end

   -- 1. Spread capital's religion to this city
   local capital = player:GetCapitalCity()
   if capital then
      local capital_religion = capital:GetReligiousMajority()
      if capital_religion and capital_religion > 0 then
         city:AdoptReligionFully(capital_religion)
         
         if player:IsHuman() and Game.GetActivePlayer() == player_id then
            local religion_name = Game.GetReligionName(capital_religion)
            local city_name = city:GetName()
            Events.GameplayAlertMessage(city_name .. " has adopted " .. Locale.ConvertTextKey(religion_name) .. "!")
         end
      end
   end

   -- 2. Grant +2 movement to units in and adjacent to the city
   local city_plot = city:Plot()
   local units_boosted = 0
   
   -- Check city plot and all adjacent plots (radius 1)
   for loop_plot in PlotAreaSpiralIterator(city_plot, 1, nil, nil, nil, true) do
      -- Iterate through all units on this plot
      for i = 0, loop_plot:GetNumUnits() - 1 do
         local unit = loop_plot:GetUnit(i)
         if unit and unit:GetOwner() == player_id then
            unit:ChangeMoves(200) -- 200 = 2 movement points (stored in hundredths)
            units_boosted = units_boosted + 1
         end
      end
   end

   -- Show popup text
   if player:IsHuman() and Game.GetActivePlayer() == player_id and units_boosted > 0 then
      local city_name = city:GetName()
      Events.GameplayAlertMessage(units_boosted .. " unit(s) near " .. city_name .. " received +2 [ICON_MOVES] Movement!")
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.CityConstructed.Add(lekmod_jerusalem_outremer_built)
end
