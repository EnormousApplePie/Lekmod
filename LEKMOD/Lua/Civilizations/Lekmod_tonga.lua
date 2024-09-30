-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_TONGA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Tonga UA. Reveal nearby islands and explore nearby coastline at the start of the game.
------------------------------------------------------------------------------------------------------------------------
function lekmod_tonga_explore(player)

   local team = player:GetTeam()
   local start_plot = player:GetStartingPlot()

   local explore_radius = 12
   for loop_plot in PlotAreaSpiralIterator(start_plot, explore_radius, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do

      if loop_plot:Area() ~= start_plot:Area() and loop_plot:IsCoastalLand() and not loop_plot:IsLake() then
         for island_plot in PlotAreaSweepIterator(loop_plot, 5, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            if island_plot:Area() == loop_plot:Area() then
               for to_reveal_plot in PlotAreaSweepIterator(island_plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
                  to_reveal_plot:SetRevealed(team, true)
               end
            end
         end
      end

   end

   for loop_plot in PlotAreaSpiralIterator(start_plot, 6, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do

      local no_lake = true
      for possible_lake_plot in PlotAreaSweepIterator(loop_plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
         if possible_lake_plot:IsLake() then
            no_lake = false
         end
      end
      if loop_plot:IsCoastalLand() and no_lake then
         for to_reveal_plot in PlotAreaSweepIterator(loop_plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            if to_reveal_plot:IsCoastalLand() or to_reveal_plot:IsWater() then
               to_reveal_plot:SetRevealed(team, true);
            end
         end
      end

   end

end

function lekmod_tonga_ua()

   for _, player in pairs(Players) do
      if player:IsEverAlive() and player:GetCivilizationType() == this_civ then
         lekmod_tonga_explore(player)
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
-- Tonga UB. Add a dummy building that awards food and culture if the city has at least 3 water tiles next to it.
------------------------------------------------------------------------------------------------------------------------
function lekmod_tonga_count_coastal_tiles(city)

   local city_plot = Map.GetPlot(city:GetX(), city:GetY())
   local count = 0
   for loop_plot in PlotAreaSweepIterator(city_plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
      if loop_plot:IsWater() then
         count = count + 1
      end
   end

return count end

function lekmod_tonga_ub_food_bonus(player_id)

   local player = Players[player_id]
  
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   for city in player:Cities() do
      if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_TONGAN_MALAE"]) and lekmod_tonga_count_coastal_tiles(city) >= 3 then
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_MALAE_FOOD"], 1)
      else
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_MALAE_FOOD"], 0)
      end
   end

end

function lekmod_tonga_ub_food_bonus_on_capture(player_id, city_id, _, _, new_player_id)

   if Players[new_player_id]:GetCivilizationType() ~= this_civ then return end
   lekmod_tonga_ub_food_bonus(new_player_id)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   Events.SequenceGameInitComplete.Add(lekmod_tonga_ua)
   --GameEvents.PlayerDoTurn.Add(tonga_ub_food_bonus)
   -- Note: BuildingSold is a Lekmod event! Not available in the base game
   GameEvents.BuildingSold.Add(lekmod_tonga_ub_food_bonus)
   GameEvents.PlayerCityFounded.Add(lekmod_tonga_ub_food_bonus)
   GameEvents.CityCaptureComplete.Add(lekmod_tonga_ub_food_bonus_on_capture)
   GameEvents.CityConstructed.Add(lekmod_tonga_ub_food_bonus)
end