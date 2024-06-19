-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_TONGA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Tonga UA. Reveal nearby islands and explore nearby coastline at the start of the game.
------------------------------------------------------------------------------------------------------------------------
local function tonga_explore(player)

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

local function tonga_ua()

   for _, player in pairs(Players) do
      if player:IsEverAlive() and player:GetCivilizationType() == this_civ then
         tonga_explore(player)
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
-- Tonga UB. Add a dummy building that awards food and culture if the city has at least 3 water tiles next to it.
------------------------------------------------------------------------------------------------------------------------
local function count_coastal_tiles(city)

   local city_plot = Map.GetPlot(city:GetX(), city:GetY())
   local count = 0
   for loop_plot in PlotAreaSweepIterator(city_plot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
      if loop_plot:IsWater() then
         count = count + 1
      end
   end

return count end

local function tonga_ub_food_bonus(player_id, ...)

   local player
   -- CityCaptureComplete has a different argument structure
   if arg["eNewPlayer"] ~= nil then
      if Players[arg["eNewPlayer"]]:GetCivilizationType() ~= this_civ then return end
      player = Players[arg["eNewPlayer"]]
   else
      player = Players[player_id]
   end
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   for city in player:Cities() do
      if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_TONGAN_MALAE"]) and count_coastal_tiles(city) >= 3 then
         city:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_MALAE_FOOD"], 1)
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   Events.SequenceGameInitComplete.Add(tonga_ua)
   GameEvents.PlayerDoTurn.Add(tonga_ub_food_bonus)
   GameEvents.PlayerCityFounded.Add(tonga_ub_food_bonus)
   GameEvents.CityCaptureComplete.Add(tonga_ub_food_bonus)
   GameEvents.CityConstructed.Add(tonga_ub_food_bonus)
end