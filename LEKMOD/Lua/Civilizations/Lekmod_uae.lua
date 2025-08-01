-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_UAE"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- UAE UA. Award 100 gold and start a We Love the King Day (15 turns) in a city that builds a World Wonder.
------------------------------------------------------------------------------------------------------------------------
function lekmod_uae_wonder_ua(player_id, city_id, building_id)

   local player = Players[player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   local city = player:GetCityByID(city_id)
   local built_building_class = GameInfo.Buildings[building_id].BuildingClass
   local building_class_info = GameInfo.BuildingClasses[built_building_class]
   if building_class_info.MaxGlobalInstances ~= 1 then return end

   local gold_boost = 100
   local kings_day_length = 15
   player:ChangeGold(gold_boost)
   city:ChangeWeLoveTheKingDayCounter(kings_day_length)

   if (player:IsHuman() and Game.GetActivePlayer() == player_id) then
      local city_name = city:GetName()
      local popup_text = Locale.ConvertTextKey("TXT_KEY_TRAIT_UAE_WONDER_BUILT", city_name, gold_boost, kings_day_length)
      Events.GameplayAlertMessage(popup_text)
   end

end
------------------------------------------------------------------------------------------------------------------------
-- UAE UA. Award +3 Gold per turn and 1 experience to units stationed on a trade route.
------------------------------------------------------------------------------------------------------------------------
function lekmod_uae_trade_route_gold(player_id)

	local player = Players[player_id]
	if player:GetCivilizationType() ~= this_civ then return end

   for loop_unit in player:Units() do

      local unit_plot = loop_unit:GetPlot()
      local trade_route_plot_info = player:GetInternationalTradeRoutePlotToolTip(unit_plot)

      if #trade_route_plot_info > 0 and loop_unit:IsCombatUnit() then
         player:ChangeGold(3)
         loop_unit:ChangeExperience(1)

         -- show popup text on the unit plot
         if player_id == Game.GetActivePlayer() then
            print("popup text")
            local hex_position = ToHexFromGrid{x = unit_plot:GetX(), y = unit_plot:GetY()}
            local world_position = HexToWorld(hex_position)
            Events.AddPopupTextEvent(world_position, "[COLOR_YIELD_GOLD]+3 [ICON_GOLD][ENDCOLOR]")
         end
      end

   end

end
------------------------------------------------------------------------------------------------------------------------
-- UAE UU. Award +15 XP and restore 2 movement when Qasimi Raider pillages.
------------------------------------------------------------------------------------------------------------------------
function lekmod_uae_qasimi_raider_pillage(player_id, unit_id, plot_x, plot_y)
   local player = Players[player_id]
   if player:GetCivilizationType() ~= this_civ then return end

   local unit = player:GetUnitByID(unit_id)
   if not unit or unit:GetUnitType() ~= GameInfoTypes.UNIT_QASIMI_RAIDER then return end

   -- Award XP and movement
   unit:ChangeExperience(15)
   unit:ChangeMoves(200) -- Movement points are in hundredths, so 200 = 2 movement points

   -- Show popup text if this is the active player
   if player_id == Game.GetActivePlayer() then
      local hex_position = ToHexFromGrid{x = plot_x, y = plot_y}
      local world_position = HexToWorld(hex_position)
      Events.AddPopupTextEvent(world_position, "[COLOR_POSITIVE_TEXT]+15 XP[ENDCOLOR]")
      Events.AddPopupTextEvent(world_position, "[COLOR_POSITIVE_TEXT]+2 [ICON_MOVES][ENDCOLOR]")
   end
end

------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.CityConstructed.Add(lekmod_uae_wonder_ua)
   GameEvents.PlayerDoTurn.Add(lekmod_uae_trade_route_gold)
end
   GameEvents.UnitPillaged.Add(lekmod_uae_qasimi_raider_pillage)
   GameEvents.UnitPlundered.Add(lekmod_uae_qasimi_raider_pillage)