-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_OMAN"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Oman UB. If a city has this UB, enemy naval units adjacent to the city take 30 damage per turn.
------------------------------------------------------------------------------------------------------------------------
function lekmod_oman_ub_damage(player_id)

	local player = Players[player_id]
	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end
   for city in player:Cities() do

      if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
         local plot = Map.GetPlot(city:GetX(), city:GetY())
         for loop_plot in PlotAreaSweepIterator(plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            -- get every unit in the plot
            for i = 0, loop_plot:GetNumUnits() - 1 do
               local unit = loop_plot:GetUnit(i)
               if unit and loop_plot:GetUnit(i):GetOwner() ~= player_id then
                  local unit_owner = Players[loop_plot:GetUnit(i):GetOwner()]
                  if Teams[player:GetTeam()]:IsAtWar(unit_owner:GetTeam())
                  and (unit:GetDomainType() == GameInfoTypes["DOMAIN_SEA"] or unit:IsEmbarked()) then
                     unit:ChangeDamage(30)
                  end
               end
            end
         end
      end

    end

end
------------------------------------------------------------------------------------------------------------------------
-- Oman UB. Add a dummy building to a city with the UB that gives +2 Production for each trade route originating from the city.
------------------------------------------------------------------------------------------------------------------------
function lekmod_oman_building_production(player_id)

	local player = Players[player_id]
	if player:GetCivilizationType() == this_civ and player:IsAlive() then
		for city in player:Cities() do
			if city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
				local number_of_routes = LekmodUtilities:get_number_trade_routes_from_city(player, city, false, false)
				if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
					city:SetNumRealBuilding(GameInfoTypes["BUILDING_OMAN_TRAIT"], number_of_routes)
				end
			end
		end
	end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
    GameEvents.PlayerDoTurn.Add(lekmod_oman_ub_damage)
    GameEvents.PlayerDoTurn.Add(lekmod_oman_building_production)
    GameEvents.UnitPrekill.Add(lekmod_oman_building_production)
end

