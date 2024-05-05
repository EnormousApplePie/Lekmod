-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local civilization_tonga = GameInfoTypes["CIVILIZATION_TONGA"]
local is_active = LekmodUtilities:is_civilization_active(civilization_tonga)

------------------------------------------------------------------------------------------------------------------------
-- Tonga UA. Reveal nearby islands and explore nearby coastline at the start of the game.
------------------------------------------------------------------------------------------------------------------------
local function tonga_explore(player)

   local pTeam = player:GetTeam();
   local startPlot = player:GetStartingPlot()

   local explore_radius = 12
   for pPlot in PlotAreaSpiralIterator(startPlot, explore_radius, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do

      if pPlot:Area() ~= startPlot:Area() and pPlot:IsCoastalLand() and not pPlot:IsLake() then
         for islandPlot in PlotAreaSweepIterator(pPlot, 5, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            if islandPlot:Area() == pPlot:Area() then
               for revealPlot in PlotAreaSweepIterator(islandPlot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
                  revealPlot:SetRevealed(pTeam, true);
               end
            end
         end
      end

   end

   for pPlot in PlotAreaSpiralIterator(startPlot, 6, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do

      local noLake = true;
      for lakeTest in PlotAreaSweepIterator(pPlot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
         if lakeTest:IsLake() then
            noLake = false;
         end
      end

      if pPlot:IsCoastalLand() and noLake then
         for revealPlot in PlotAreaSweepIterator(pPlot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            if revealPlot:IsCoastalLand() or revealPlot:IsWater() then
               revealPlot:SetRevealed(pTeam, true);
            end
         end
      end

   end

end

local function tonga_ua()

   for _, player in pairs(Players) do
      if player:IsEverAlive() and player:GetCivilizationType() == civilization_tonga then
         tonga_explore(player)
      end
   end

end
------------------------------------------------------------------------------------------------------------------------

if is_active then
   Events.SequenceGameInitComplete.Add(tonga_ua);
end