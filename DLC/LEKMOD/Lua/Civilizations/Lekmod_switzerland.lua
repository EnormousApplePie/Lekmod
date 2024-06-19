-- Author: EnormousApplePie & 404NotFound

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_SWISS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Switzerland UB: Swiss Ski Resort. Loop trough all cities with the UB. Count the mountains within 3 tiles of the city,
-- then add the appropriate amount of mountain buildings to the city to give the bonus.
------------------------------------------------------------------------------------------------------------------------
local function ub_mountain_bonus(player_id)

   local player = Players[player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   for city in player:Cities() do

      if city:IsHasBuilding(GameInfoTypes["BUILDING_SWISS_SKI_RESORT"]) then
            local plot = city:Plot()
            local mountain_count = 0
            for loop_plot in PlotAreaSweepIterator(plot, 3, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
               if loop_plot:IsMountain() and loop_plot:GetOwner() == player_id then
                  mountain_count = mountain_count + 1
               end
            end
            print(mountain_count)
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_1"], mountain_count)
      else
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_1"], 0)
      end

   end

end

local function ub_mountain_bonus_on_construct(player_id)
   ub_mountain_bonus(player_id)
end
local function ub_mountain_bonus_on_conquest(_, _, _, _, new_player_id)
   ub_mountain_bonus(new_player_id)
end
------------------------------------------------------------------------------------------------------------------------
-- Switzerland UU: Reislaufer. Check if the unit that just moved is a reislaufer and if it is near a mountain. If it is,
-- give it the active promotion. If it is not, remove the active promotion.
------------------------------------------------------------------------------------------------------------------------
local mountaineer = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER"];
local mountaineer_active = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER_ACTIVE"];

local function uu_mountain_bonus(player_id, unit_id)

	local player = Players[player_id]
	local unit = player:GetUnitByID(unit_id)
	if not unit:IsHasPromotion(mountaineer) then return end
   -- Note: IsNearTerrainType (method) and FEATURE_ARARAT_MOUNTAIN are Lekmod specific! Not available in the base game
   if unit:IsNearFeatureType(GameInfoTypes["FEATURE_ARARAT_MOUNTAIN"], 1, false) then
      unit:SetHasPromotion(mountaineer_active, true);
   else
      unit:SetHasPromotion(mountaineer_active, false);
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   -- Note: CityAcquirePlot is a Lekmod Event! Not available in the base game
   GameEvents.CityAcquirePlot.Add(ub_mountain_bonus)
   GameEvents.CityCaptureComplete.Add(ub_mountain_bonus_on_conquest)
   GameEvents.CityConstructed.Add(ub_mountain_bonus_on_construct)
end
-- Unique Units can be active even if the civilization is not active
GameEvents.UnitSetXY.Add(uu_mountain_bonus)
-- Note: UnitCreated is a Lekmod Event! Not available in the base game
GameEvents.UnitCreated.Add(uu_mountain_bonus)