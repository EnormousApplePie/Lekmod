-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_NABATEA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Nabatea UA. Add a dummy technology that gives fresh water farms +1 food until civil service is researched.
------------------------------------------------------------------------------------------------------------------------
function lekmod_nabatea_ua_farm_food(player_id)

	local player = Players[player_id]
	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   local team = Teams[player:GetTeam()]
   if team:IsHasTech(GameInfoTypes["TECH_CIVIL_SERVICE"]) then
      team:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], false)
	elseif team:IsHasTech(GameInfoTypes["TECH_MATHEMATICS"]) then
		team:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], true)
	end

end
------------------------------------------------------------------------------------------------------------------------
-- Nabatea UU. Award gold for discovering cities with a Zabonah.
------------------------------------------------------------------------------------------------------------------------
function lekmod_nabatea_unit_exploration(player_id, unit, unit_x, unit_y)

   local nabatea_unit = GameInfoTypes["UNIT_MC_ZABONAH"]
	local player = Players[player_id]
	local unit_id = player:GetUnitByID(unit)
	if unit_id:GetUnitType() ~= nabatea_unit then return end

	local unit_team = unit_id:GetTeam()
	local unit_plot = Map.GetPlot(unit_x, unit_y)

	for loop_plot in PlotAreaSweepIterator(unit_plot, unit_id:VisibilityRange() + 3, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
		local city = loop_plot:GetPlotCity()
		if not city then -- skip
      elseif not(city:IsRevealed(unit_team)) and city:IsCapital() then

         loop_plot:SetRevealed(unit_team, true)
         local reward_amount = 10
         player:ChangeGold(reward_amount)

         if (player:IsHuman() and Game.GetActivePlayer() == player_id and reward_amount > 0 ) then
            local popup_text = Locale.ConvertTextKey("TXT_KEY_ZABONAH_DISCOVERED_CITY", reward_amount, city:GetName())
            Events.GameplayAlertMessage(popup_text)
         end
		end
	end

end
------------------------------------------------------------------------------------------------------------------------
-- Nabatea UB. Add a dummy building to a city with the UB that gives +1 food for each trade route originating from the city.
------------------------------------------------------------------------------------------------------------------------
function lekmod_nabatea_building_food(player_id)

	local player = Players[player_id]
	if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end
	for city in player:Cities() do
		if city:IsHasBuilding(GameInfoTypes["BUILDING_MC_KOKH"]) then
			local number_of_routes = LekmodUtilities:get_number_trade_routes_from_city(player, city, false, false)
			if city then
				city:SetNumRealBuilding(GameInfoTypes["BUILDING_NABATEA_TRAIT"], number_of_routes)
			end
		end
	end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerDoTurn.Add(lekmod_nabatea_building_food)
   GameEvents.UnitPrekill.Add(lekmod_nabatea_building_food)
   GameEvents.TeamSetHasTech.Add(lekmod_nabatea_ua_farm_food)
end
   -- A unique unit can be present in the game without the civilization being active.
   GameEvents.UnitSetXY.Add(lekmod_nabatea_unit_exploration)