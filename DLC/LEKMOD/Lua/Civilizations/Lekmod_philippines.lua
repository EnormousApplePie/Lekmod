
-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_PHILIPPINES"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Philippines UA. Add a building to a new city that gives +1 population to the city until the maximum is reached.
------------------------------------------------------------------------------------------------------------------------
local dummy_building = GameInfoTypes["BUILDING_PHILIPPINES_TRAIT"]
local bonus_cities_amount = 2

local function philippine_expand_population(player_id, iX, iY)
   local player = Players[player_id]
   if player:GetCivilizationType() == this_civ
      and player:IsAlive()
      and player:CountNumBuildings(dummy_building) < bonus_cities_amount
   then
      local plot = Map.GetPlot(iX, iY)
      local city = plot:GetPlotCity()
      if not city:IsCapital() then
         city:SetNumRealBuilding(dummy_building, 1)
      end
   end
end
------------------------------------------------------------------------------------------------------------------------
-- Philippines UA. Give civilian units +2 movement inside owned territory.
------------------------------------------------------------------------------------------------------------------------
local function philippine_movement_bonus(player_id, unit_id)

   local movement_promotion = GameInfoTypes["PROMOTION_GOOD_FIGHT"]
	local player = Players[player_id]
	if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end

   local in_owned_territory = false
   local unit = player:GetUnitByID(unit_id)
   local plot = Map.GetPlot(unit:GetX(), unit:GetY())
   if plot == nil then return end
   if not unit:IsDead() and unit ~= nil
      and not unit:IsCombatUnit()
      and plot:GetOwner() == player_id then
      in_owned_territory = true
   end

   if not unit:IsHasPromotion(movement_promotion) and in_owned_territory then
      unit:SetHasPromotion(movement_promotion, true)
   elseif unit:IsHasPromotion(movement_promotion) and not in_owned_territory then
      unit:SetHasPromotion(movement_promotion, false)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerCityFounded.Add(philippine_expand_population)
   GameEvents.UnitSetXY.Add(philippine_movement_bonus)
end