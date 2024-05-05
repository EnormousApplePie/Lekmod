-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MOORS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- UAE UA. Award 100 gold and start a We Love the King Day (15 turns) in a city that builds a World Wonder.
------------------------------------------------------------------------------------------------------------------------
local function uae_wonder_ua(player_id, city_id, building_id)

   local player = Players[player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   local city = player:GetCityByID(city_id)
   local built_building_class = GameInfo.Buildings[building_id].BuildingClass
   local building_class_info = GameInfo.BuildingClasses[built_building_class]
   if not building_class_info.MaxGlobalInstances == 1 then return end

   local gold_boost = 100
   local kings_day_length = 15
   player:ChangeGold(gold_boost)
   city:ChangeWeLoveTheKingDayCounter(kings_day_length)

end
------------------------------------------------------------------------------------------------------------------------
-- UAE UA. Award +3 Gold per turn and 1 experience to units stationed on a trade route.
------------------------------------------------------------------------------------------------------------------------
local function uae_trade_route_gold(player_id)
	local player = Players[player_id]
	if player:GetCivilizationType() ~= this_civ then return end

   for loop_unit in player:Units() do

      local unit_plot = loop_unit:GetPlot()
      local trade_route_plot_info = player:GetInternationalTradeRoutePlotToolTip(unit_plot)
      local is_trade_route_on_plot = #trade_route_plot_info > 0

      if not is_trade_route_on_plot or loop_unit:IsCombatUnit() <= 0 then return end

      player:ChangeGold(3)
      loop_unit:ChangeExperience(1)

      -- show popup text on the unit's plot
      if player_id == Game.GetActivePlayer() then
         local hex_position = ToHexFromGrid{unit_plot:GetX(), unit_plot:GetY()}
         local world_position = HexToWorld(hex_position)
         Events.AddPopupTextEvent(world_position, "[COLOR_YIELD_GOLD]+3 [ICON_GOLD] Gold[ENDCOLOR]")
      end

   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.CityConstructed.Add(uae_wonder_ua)
   GameEvents.PlayerDoTurn.Add(uae_trade_route_gold)
end