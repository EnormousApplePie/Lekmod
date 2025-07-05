-- Author: EnormousApplePie

include("PlotIterators.lua")

------------------------------------------------------------------------------------------------------------------------
--[[ Huey Teocalli requires a lake nearby. This has been deprecated in favor of a DLL implementation.
------------------------------------------------------------------------------------------------------------------------
local lake_wonder_id = GameInfoTypes["BUILDING_LAKE_WONDER"]

function lekmod_huey_lake_check(player_id, city_id, building_id)

   local player = Players[player_id]
   local city = player:GetCityByID(city_id)

   if building_id == lake_wonder_id then

      local is_near_lake = false
      for loop_plot in PlotAreaSweepIterator(city:Plot(), 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
         if (loop_plot:IsLake() or loop_plot:GetFeatureType() == GameInfoTypes["FEATURE_LAKE_VICTORIA"])
         and loop_plot:GetOwner() == player_id then
            is_near_lake = true
            break
         end
      end
      if is_near_lake then
         return true
      else
         return false
      end

   end

return true end
GameEvents.CityCanConstruct.Add(lekmod_huey_lake_check)
------------------------------------------------------------------------------------------------------------------------]]