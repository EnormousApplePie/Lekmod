-- Author: EnormousApplePie
include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_BOERS"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Boers UA. Give adjacency bonuses to farms depending on how many are adjacentPlot
------------------------------------------------------------------------------------------------------------------------
