-- Author: EnormousApplePie & 404NotFound

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_SWISS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Switzerland UB: Swiss Ski Resort. Loop trough all cities with the UB. Count the mountains within 3 tiles of the city,
-- then add the appropriate amount of mountain buildings to the city to give the bonus.
------------------------------------------------------------------------------------------------------------------------
function lekmod_switzerland_ub_mountain_bonus(playerID)
   local player = Players[playerID]
   local skiResort = GameInfoTypes["BUILDING_SWISS_SKI_RESORT"]
   local yieldGold = GameInfoTypes["YIELD_GOLD"]

   local bonusPerMountain = 2
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then 
      return 
   end

   for city in player:Cities() do
      local cid = city:GetID()
      local prevBonus = previousMountainBonus[cid] or 0

      if city:IsHasBuilding(skiResort) then
         -- GetNumMountainsNearCity is a Lekmod method! Not available in the base game also passes (iRange, bRequireOwnership)
         local mountainCount = city:GetNumMountainsNearCity(3, true)
         local newBonus = mountainCount * bonusPerMountain
         local delta    = newBonus - prevBonus

         if delta ~= 0 then
            city:ChangeBaseYieldRateFromBuildings(yieldGold, delta)
            previousMountainBonus[cid] = newBonus
         end
      else
         -- building gone — remove any old bonus
         if prevBonus ~= 0 then
            city:ChangeBaseYieldRateFromBuildings(yieldGold, -prevBonus)
            previousMountainBonus[cid] = nil
         end
      end
   end
end

function lekmod_switzerland_ub_mountain_bonus_on_construct(player_id)
   lekmod_switzerland_ub_mountain_bonus(player_id)
end
function lekmod_switzerland_ub_mountain_bonus_on_conquest(_, _, _, _, new_player_id)
   lekmod_switzerland_ub_mountain_bonus(new_player_id)
end
------------------------------------------------------------------------------------------------------------------------
-- Switzerland UU: Reislaufer. Check if the unit that just moved is a reislaufer and if it is near a mountain. If it is,
-- give it the active promotion. If it is not, remove the active promotion.
------------------------------------------------------------------------------------------------------------------------
local mountaineer = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER"];
local mountaineer_active = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER_ACTIVE"];

function lekmod_switzerland_uu_mountain_bonus(player_id, unit_id)

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
   GameEvents.CityAcquirePlot.Add(lekmod_switzerland_ub_mountain_bonus)
   GameEvents.CityCaptureComplete.Add(lekmod_switzerland_ub_mountain_bonus_on_conquest)
   GameEvents.CityConstructed.Add(lekmod_switzerland_ub_mountain_bonus_on_construct)
end
-- Unique Units can be active even if the civilization is not active
GameEvents.UnitSetXY.Add(lekmod_switzerland_uu_mountain_bonus)
-- Note: UnitCreated is a Lekmod Event! Not available in the base game
GameEvents.UnitCreated.Add(lekmod_switzerland_uu_mountain_bonus)