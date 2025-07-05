-- Author: EnormousApplePie & 404NotFound & Loup fixing Loup's fuckups ~~

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_SWISS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)
-----------------------------------------------------------------------------------------------------------------------
-- Switzerland UB: Ski Resort. If the city has a Ski Resort, give it a bonus for each mountain tile within 3 tiles.
local function lekmod_switzerland_ub_mountain_bonus(playerID)
   local player = Players[playerID]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   for city in player:Cities() do
      if city:IsHasBuilding(GameInfoTypes["BUILDING_SWISS_SKI_RESORT"]) then
            local plot = city:Plot()
            local mountain_count = 0
            mountain_count = city:GetNumMountainsNearCity(3, true) -- Range, RequireOwnership
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_SKI_GOLD"], mountain_count)
      else
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_SKI_GOLD"], 0)
      end
   end
end
   
-- When you build a Ski Resort, just rescan that one player
function lekmod_switzerland_ub_mountain_bonus_on_construct(playerID, cityID, buildingID)
    if buildingID == GameInfoTypes["BUILDING_SWISS_SKI_RESORT"] then
        lekmod_switzerland_ub_mountain_bonus(playerID)
    end
end

-- When a city changes hands, rescan the new owner
function lekmod_switzerland_ub_mountain_bonus_on_conquest(oldOwner, isConquest, x, y, newOwner)
    lekmod_switzerland_ub_mountain_bonus(newOwner)
end


-----------------------------------------------------------------------------------------------------------------------
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