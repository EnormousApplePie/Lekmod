-- Author: EnormousApplePie
include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_MEXICO"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Mexico UA, search for 10 tiles around the starting settler and reveal city-states
------------------------------------------------------------------------------------------------------------------------
local function ua_do_discover(player_id)

	local player = Players[player_id]
	local player_team = player:GetTeam()

	-- Iterate to find cities from the starting plot
	local start_plot = player:GetStartingPlot()
	for loop_plot in PlotAreaSweepIterator(start_plot, 10, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
		local city = loop_plot:GetPlotCity()
		if city then
			--get the owner of the city tile
			local city_owner = Players[city:GetOwner()]
			if city_owner:IsMinorCiv() and (not(city:IsRevealed(player_team))) then
				loop_plot:SetRevealed(player_team, true);
			end
		end
	end
end

local function mexico_ua(player_id)

    local player = Players[player_id]
    if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end

	-- get the amount of passed turns in the game
	local turn = Game.GetElapsedGameTurns()
	if turn > 2 then return end

	ua_do_discover(player_id)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.PlayerDoTurn.Add(mexico_ua);
end
