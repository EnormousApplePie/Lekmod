------------------------------------------------------------------------------
--	FILE:	 West_vs_East.lua
--	AUTHOR:  Bob Thomas
--	PURPOSE: Regional map script - Designed to pit two teams against each other
--	         with a strip of water dividing the map east from west.
------------------------------------------------------------------------------
--	Copyright (c) 2010 Firaxis Games, Inc. All rights reserved.
------------------------------------------------------------------------------

include("HBMapGeneratorRectangular");
include("HBFractalWorld");
include("HBFeatureGeneratorRectangular");
include("HBTerrainGenerator");
include("IslandMaker");
include("MultilayeredFractal");

------------------------------------------------------------------------------
function GetMapScriptInfo()
	local world_age, temperature, rainfall, sea_level, resources = GetCoreMapOptions()
	return {
		Name = "Lekmap: 4Corners v1.2 (Bonus Map)",
		Description = "A continent with four regions, divided by imposing natural barriers. Designed for 2v2v2v2 games.",
		SupportsMultiplayer = true,
		IconIndex = 18,
		SortIndex = 2,
	CustomOptions = {
			{
				Name = "TXT_KEY_MAP_OPTION_WORLD_AGE", -- 1
				Values = {
					"TXT_KEY_MAP_OPTION_THREE_BILLION_YEARS",
					"TXT_KEY_MAP_OPTION_FOUR_BILLION_YEARS",
					"TXT_KEY_MAP_OPTION_FIVE_BILLION_YEARS",
					"No Mountains",
					"TXT_KEY_MAP_OPTION_RANDOM",
				},
				DefaultValue = 2,
				SortPriority = -99,
			},

			{
				Name = "TXT_KEY_MAP_OPTION_TEMPERATURE",	-- 2 add temperature defaults to random
				Values = {
					"TXT_KEY_MAP_OPTION_COOL",
					"TXT_KEY_MAP_OPTION_TEMPERATE",
					"TXT_KEY_MAP_OPTION_HOT",
					"TXT_KEY_MAP_OPTION_RANDOM",
				},
				DefaultValue = 2,
				SortPriority = -98,
			},

			{
				Name = "TXT_KEY_MAP_OPTION_RAINFALL",	-- 3 add rainfall defaults to random
				Values = {
					"TXT_KEY_MAP_OPTION_ARID",
					"TXT_KEY_MAP_OPTION_NORMAL",
					"TXT_KEY_MAP_OPTION_WET",
					"TXT_KEY_MAP_OPTION_RANDOM",
				},
				DefaultValue = 2,
				SortPriority = -97,
			},

			{
				Name = "TXT_KEY_MAP_OPTION_SEA_LEVEL",	-- 4 add sea level defaults to random.
				Values = {
					"TXT_KEY_MAP_OPTION_LOW",
					"TXT_KEY_MAP_OPTION_MEDIUM",
					"TXT_KEY_MAP_OPTION_HIGH",
					"TXT_KEY_MAP_OPTION_RANDOM",
				},
				DefaultValue = 2,
				SortPriority = -96,
			},

			{
				Name = "Start Quality",	-- 5 add resources defaults to random
				Values = {
					"Legendary Start - Strat Balance",
					"Legendary - Strat Balance + Uranium",
					"TXT_KEY_MAP_OPTION_STRATEGIC_BALANCE",
					"Strategic Balance With Coal",
					"Strategic Balance With Aluminum",
					"Strategic Balance With Coal & Aluminum",
					"TXT_KEY_MAP_OPTION_RANDOM",
				},
				DefaultValue = 2,
				SortPriority = -95,
			},

			{
				Name = "Start Distance",	-- 6 add resources defaults to random
				Values = {
					"Close",
					"Normal",
					"Far - Warning: May sometimes crash during map generation",
				},
				DefaultValue = 2,
				SortPriority = -94,
			},

			{
				Name = "Natural Wonders", -- 7 number of natural wonders to spawn
				Values = {
					"0",
					"1",
					"2",
					"3",
					"4",
					"5",
					"6",
					"7",
					"8",
					"9",
					"10",
					"11",
					"12",
					"Random",
					"Default",
				},
				DefaultValue = 15,
				SortPriority = -93,
			},

			{
				Name = "Grass Moisture",	-- add setting for grassland mositure (8)
				Values = {
					"Wet",
					"Normal",
					"Dry",
				},

				DefaultValue = 2,
				SortPriority = -92,
			},

			{
				Name = "Rivers",	-- add setting for rivers (9)
				Values = {
					"Sparse",
					"Average",
					"Plentiful",
				},

				DefaultValue = 2,
				SortPriority = -91,
			},

			{
				Name = "Tundra",	-- add setting for tundra (10)
				Values = {
					"Sparse",
					"Average",
					"Plentiful",
				},

				DefaultValue = 2,
				SortPriority = -90,
			},

			{
				Name = "Land Size X",	-- add setting for land type (11) +18
				Values = {
					"20",
					"22",
					"24",
					"26",
					"28",
					"30",
					"32",
					"34",
					"36",
					"38",
					"40",
					"42",
					"44",
					"46",
					"48",
					"50",
					"52",
					"54",
					"56",
					"58",
					"60",
					"62",
					"64",
					"66",
					"68",
					"70",
					"72",
					"74",
					"76",
				},

				DefaultValue = 15,
				SortPriority = -89,
			},

			{
				Name = "Land Size Y",	-- add setting for land type (12) +18
				Values = {
					"20",
					"22",
					"24",
					"26",
					"28",
					"30",
					"32",
					"34",
					"36",
					"38",
					"40",
					"42",
					"44",
					"46",
					"48",
					"50",
					"52",
					"54",
					"56",
					"58",
					"60",
					"62",
					"64",
					"66",
					"68",
					"70",
					"72",
					"74",
					"76",

				},

				DefaultValue = 15,
				SortPriority = -88,
			},

			{
				Name = "TXT_KEY_MAP_OPTION_RESOURCES",	-- add setting for resources (13)
				Values = {
					"1 -- Nearly Nothing",
					"2",
					"3",
					"4",
					"5 -- Default",
					"6",
					"7",
					"8",
					"9",
					"10 -- Almost no normal tiles left",
				},

				DefaultValue = 5,
				SortPriority = -87,
			},

			{
				Name = "Balanced Regionals",	-- add setting for removing OP luxes from regional pool (14)
				Values = {
					"Yes",
					"No",
				},

				DefaultValue = 1,
				SortPriority = -90,
			},
			{
				Name = "Frontline Distance", -- 15 minimum distance a capital can spawn from the front
				Values = {
					"0",
					"1",
					"2 - Default",
					"3",
					"4",
					"5",
					"6",
				},
				DefaultValue = 3,
				SortPriority = -96,
			},
			{
				Name = "World Border Distance", -- 16 minimum distance a capital can spawn from the back
				Values = {
					"0 - Default",
					"1",
					"2",
					"3",
					"4",
					"5",
					"6",
				},
				DefaultValue = 1,
				SortPriority = -97,
			},
		},
	}
end
------------------------------------------------------------------------------

-------------------------------------------------------------------------------
function GetMapInitData(worldSize)
	
	local LandSizeX = 18 + (Map.GetCustomOption(11) * 2);
	local LandSizeY = 18 + (Map.GetCustomOption(12) * 2);

	local worldsizes = {};

	worldsizes = {

		[GameInfo.Worlds.WORLDSIZE_DUEL.ID] = {LandSizeX, LandSizeY}, -- 720
		[GameInfo.Worlds.WORLDSIZE_TINY.ID] = {LandSizeX, LandSizeY}, -- 1664
		[GameInfo.Worlds.WORLDSIZE_SMALL.ID] = {LandSizeX, LandSizeY}, -- 2480
		[GameInfo.Worlds.WORLDSIZE_STANDARD.ID] = {LandSizeX, LandSizeY}, -- 3900
		[GameInfo.Worlds.WORLDSIZE_LARGE.ID] = {LandSizeX, LandSizeY}, -- 6076
		[GameInfo.Worlds.WORLDSIZE_HUGE.ID] = {LandSizeX, LandSizeY} -- 9424
		}
		
	local grid_size = worldsizes[worldSize];
	--
	local world = GameInfo.Worlds[worldSize];
	if (world ~= nil) then
		return {
			Width = grid_size[1],
			Height = grid_size[2],
			WrapX = false,
		}; 
	end

end
-------------------------------------------------------------------------------
function MultilayeredFractal:GeneratePlotsByRegion()
	-- Sirian's MultilayeredFractal controlling function.
	-- You -MUST- customize this function for each script using MultilayeredFractal.
	--
	-- This implementation is specific to West vs East.
	local iW, iH = Map.GetGridSize();
	local fracFlags = {};

-- Fill all rows with land plots.
	self.wholeworldPlotTypes = table.fill(PlotTypes.PLOT_LAND, iW * iH);


-- Generate the inland sea.
	local iWestX = 0;
	local iEastX = iW / 2 - 5;
	local iWidth = iEastX - iWestX;
	local iSouthY = 2;
	local iNorthY = 7;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = iW / 2 + 4;
	local iEastX = iW;
	local iWidth = iEastX - iWestX;
	local iSouthY = 2;
	local iNorthY = 7;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = 2;
	local iEastX = 7;
	local iWidth = iEastX - iWestX;
	local iSouthY = 0;
	local iNorthY = iH / 2 - 5;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = 2;
	local iEastX = 7;
	local iWidth = iEastX - iWestX;
	local iSouthY = iH / 2 + 4;
	local iNorthY = iH;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-----------------------------------
-- Generate the inland sea.
	local iWestX = 0;
	local iEastX = iW / 2 - 5;
	local iWidth = iEastX - iWestX;
	local iSouthY = iH - 8;
	local iNorthY = iH - 2;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = iW / 2 + 4;
	local iEastX = iW;
	local iWidth = iEastX - iWestX;
	local iSouthY = iH - 8;
	local iNorthY = iH - 2;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = iW - 8;
	local iEastX = iW - 2;
	local iWidth = iEastX - iWestX;
	local iSouthY = 0;
	local iNorthY = iH / 2 - 5;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Generate the inland sea.
	local iWestX = iW - 8;
	local iEastX = iW - 2;
	local iWidth = iEastX - iWestX;
	local iSouthY = iH / 2 + 4;
	local iNorthY = iH;
	local iHeight = iNorthY - iSouthY;
	local fracFlags = {FRAC_POLAR = true};
	local grain = 1 + Map.Rand(2, "Inland Sea ocean grain - LUA");
	local seaFrac = Fractal.Create(iWidth, iHeight, grain, fracFlags, -1, -1)
	local seaThreshold = seaFrac:GetHeight(47);
	
	for region_y = 0, iHeight - 1 do
		for region_x = 0, iWidth - 1 do
			local val = seaFrac:GetHeight(region_x, region_y);
			if val >= seaThreshold then
				local x = region_x + iWestX;
				local y = region_y + iSouthY;
				local i = y * iW + x + 1; -- add one because Lua arrays start at 1
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
-- Add strips of ocean to the world borders.
	for y = 0, iH / 2 - 5 do
		for x = 0, 3 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = 0, iW / 2 - 5 do
		for y = 0, 3 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for y = 0, iH / 2 - 5 do
		for x = iW - 4, iW do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = 0, iW / 2 - 5 do
		for y = iH - 4, iH do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
------------------------
-- Add strips of ocean to the world borders.
	for y = iH / 2 + 4, iH do
		for x = 0, 3 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = iW / 2 + 4, iW do
		for y = 0, 3 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for y = iH / 2 + 4, iH do
		for x = iW - 4, iW do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = iW / 2 + 4, iW do
		for y = iH - 4, iH do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
--------------------------------------------
-- Add lakes.
	local lakesFrac = Fractal.Create(iW, iH, lake_grain, fracFlags, 6, 6);
	local iLakesThreshold = lakesFrac:GetHeight(92);
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = y * iW + x + 1; -- add one because Lua arrays start at 1
			local lakeVal = lakesFrac:GetHeight(x, y);
			if lakeVal >= iLakesThreshold then
				--self.wholeworldPlotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
			end
		end
	end



	-- Land and water are set. Now apply hills and mountains.
	local world_age = Map.GetCustomOption(1)
	if world_age == 4 then
		world_age = 1 + Map.Rand(3, "Random World Age - Lua");
	end
	local args = {world_age = world_age};
	self:ApplyTectonics(args)

	for x = iW / 2 - 3, iW / 2 + 2 do
		for y = 0, iH - 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end
	for y = iH / 2 - 3, iH / 2 + 2 do
		for x = 0, iW - 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end

-- snowve fronts
	local west_half, east_half = {}, {};
	for loop = 1, iH - 2 do
		table.insert(west_half, loop);
		table.insert(east_half, loop);

	end
	local west_shuffled = GetShuffledCopyOfTable(west_half)
	local east_shuffled = GetShuffledCopyOfTable(east_half)
	local iNumMountainsPerColumn = math.floor(iH * 0.28);
	local x_west, x_east = iW / 2 - 3, iW / 2 + 2;
	for loop = 1, iNumMountainsPerColumn do
		local y_west, y_east = west_shuffled[loop], east_shuffled[loop];
		local i_west_plot = y_west * iW + x_west + 1;
		local i_east_plot = y_east * iW + x_east + 1;
		self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_MOUNTAIN;
		self.wholeworldPlotTypes[i_east_plot] = PlotTypes.PLOT_MOUNTAIN;
	end
---------------------------------------------------------------

	local south_half, north_half = {}, {};
	for loop = 1, iW - 2 do
		table.insert(south_half, loop);
		table.insert(north_half, loop);
	end
	local south_shuffled = GetShuffledCopyOfTable(south_half)
	local north_shuffled = GetShuffledCopyOfTable(north_half)
	local iNumMountainsPerColumn = math.floor(iW * 0.28);
	local y_south, y_north = iH / 2 - 3, iH / 2 + 2;
	for loop = 1, iNumMountainsPerColumn do
		local x_south, x_north = south_shuffled[loop], north_shuffled[loop];
		local i_south_plot = y_south * iW + x_south + 1;
		local i_north_plot = y_north * iW + x_north + 1;
		self.wholeworldPlotTypes[i_south_plot] = PlotTypes.PLOT_MOUNTAIN;
		self.wholeworldPlotTypes[i_north_plot] = PlotTypes.PLOT_MOUNTAIN;
	end

-----------------------------------------------
-- wawawave fronts

	local west_half, east_half = {}, {};
	for loop = 1, iH / 2 - 5 do
		table.insert(west_half, loop);
		table.insert(east_half, loop);
	end
	local west_shuffled = GetShuffledCopyOfTable(west_half)
	local east_shuffled = GetShuffledCopyOfTable(east_half)
	local iNumMountainsPerColumn = math.floor(iH * 0.19);
	local x_west, x_east = iW / 2 - 2, iW / 2 + 1;
	for loop = 1, iNumMountainsPerColumn do
		local y_west, y_east = west_shuffled[loop], east_shuffled[loop];
		local i_west_plot = y_west * iW + x_west + 1;
		local i_east_plot = y_east * iW + x_east + 1;
		self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_OCEAN;
		self.wholeworldPlotTypes[i_east_plot] = PlotTypes.PLOT_OCEAN;
	end
---------------------------------------------------------------

	local south_half, north_half = {}, {};
	for loop = 1, iW / 2 - 5 do
		table.insert(south_half, loop);
		table.insert(north_half, loop);
	end
	local south_shuffled = GetShuffledCopyOfTable(south_half)
	local north_shuffled = GetShuffledCopyOfTable(north_half)
	local iNumMountainsPerColumn = math.floor(iW * 0.19);
	local y_south, y_north = iH / 2 - 2, iH / 2 + 1;
	for loop = 1, iNumMountainsPerColumn do
		local x_south, x_north = south_shuffled[loop], north_shuffled[loop];
		local i_south_plot = y_south * iW + x_south + 1;
		local i_north_plot = y_north * iW + x_north + 1;
		self.wholeworldPlotTypes[i_south_plot] = PlotTypes.PLOT_OCEAN;
		self.wholeworldPlotTypes[i_north_plot] = PlotTypes.PLOT_OCEAN;
	end
--------------------------------------------

	local west_half, east_half = {}, {};
	for loop = iH / 2 + 4, iH - 2 do
		table.insert(west_half, loop);
		table.insert(east_half, loop);
	end
	local west_shuffled = GetShuffledCopyOfTable(west_half)
	local east_shuffled = GetShuffledCopyOfTable(east_half)
	local iNumMountainsPerColumn = math.floor(iH * 0.19);
	local x_west, x_east = iW / 2 - 2, iW / 2 + 1;
	for loop = 1, iNumMountainsPerColumn do
		local y_west, y_east = west_shuffled[loop], east_shuffled[loop];
		local i_west_plot = y_west * iW + x_west + 1;
		local i_east_plot = y_east * iW + x_east + 1;
		self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_OCEAN;
		self.wholeworldPlotTypes[i_east_plot] = PlotTypes.PLOT_OCEAN;
	end
---------------------------------------------------------------

	local south_half, north_half = {}, {};
	for loop = iH / 2 + 4, iH - 2 do
		table.insert(south_half, loop);
		table.insert(north_half, loop);
	end
	local south_shuffled = GetShuffledCopyOfTable(south_half)
	local north_shuffled = GetShuffledCopyOfTable(north_half)
	local iNumMountainsPerColumn = math.floor(iW * 0.19);
	local y_south, y_north = iH / 2 - 2, iH / 2 + 1;
	for loop = 1, iNumMountainsPerColumn do
		local x_south, x_north = south_shuffled[loop], north_shuffled[loop];
		local i_south_plot = y_south * iW + x_south + 1;
		local i_north_plot = y_north * iW + x_north + 1;
		self.wholeworldPlotTypes[i_south_plot] = PlotTypes.PLOT_OCEAN;
		self.wholeworldPlotTypes[i_north_plot] = PlotTypes.PLOT_OCEAN;
	end

-- Add strip of ocean to middle of map --- Always start with this for civ placements
	for y = 0, iH / 2 - 5 do
		for x = math.floor(iW / 2) - 1, math.floor(iW / 2) do
			local plotIndex = y * iW + x + 1;
			--if y >= math.floor(iH / 2) - 2 and y <= math.floor(iH / 2) + 1 then
				--if x == math.floor(iW / 2) or x == math.floor(iW / 2) - 1 then
					--self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
				--end
			--else
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			--end
		end
	end
-- Add strip of ocean to middle of map --- Always start with this for civ placements
	for x = 0, iW / 2 - 5 do
		for y = math.floor(iH / 2) - 1, math.floor(iH / 2) do
			local plotIndex = y * iW + x + 1;
			--if y >= math.floor(iH / 2) - 2 and y <= math.floor(iH / 2) + 1 then
				--if x == math.floor(iW / 2) or x == math.floor(iW / 2) - 1 then
					--self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
				--end
			--else
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			--end
		end
	end
-- Add strip of ocean to middle of map --- Always start with this for civ placements
	for y = iH / 2 + 4, iH do
		for x = math.floor(iW / 2) - 1, math.floor(iW / 2) do
			local plotIndex = y * iW + x + 1;
			--if y >= math.floor(iH / 2) - 2 and y <= math.floor(iH / 2) + 1 then
				--if x == math.floor(iW / 2) or x == math.floor(iW / 2) - 1 then
					--self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
				--end
			--else
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			--end
		end
	end
-- Add strip of ocean to middle of map --- Always start with this for civ placements
	for x = iW / 2 + 4, iW do
		for y = math.floor(iH / 2) - 1, math.floor(iH / 2) do
			local plotIndex = y * iW + x + 1;
			--if y >= math.floor(iH / 2) - 2 and y <= math.floor(iH / 2) + 1 then
				--if x == math.floor(iW / 2) or x == math.floor(iW / 2) - 1 then
					--self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
				--end
			--else
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			--end
		end
	end

-- Add strips of ocean to the world borders.
	for y = 0, iH do
		for x = 0, 1 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = 0, iW do
		for y = 0, 1 do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for y = 0, iH do
		for x = iW - 2, iW do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
	for x = 0, iW do
		for y = iH - 2, iH do
			local plotIndex = y * iW + x + 1;
			self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
		end
	end
-- Add a Landbridge
	for x = math.floor(iW / 6) - 2, math.floor(iW / 6) + 2 do
		for y = iH / 2 - 2, iH / 2 + 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end
-- Add a Landbridge
	for x = iW - math.floor(iW / 6) - 3, iW - math.floor(iW / 6) + 1 do
		for y = iH / 2 - 2, iH / 2 + 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end
-- Add a Landbridge
	for y = math.floor(iH / 6) - 2, math.floor(iH / 6) + 2 do
		for x = iW / 2 - 2, iW / 2 + 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end
-- Add a Landbridge
	for y = iH - math.floor(iH / 6) - 3, iH - math.floor(iH / 6) + 1 do
		for x = iW / 2 - 2, iW / 2 + 1 do
			local i = y * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
		end
	end

	-- Plot Type generation completed. Return global plot array.
	return self.wholeworldPlotTypes
end
------------------------------------------------------------------------------
function GeneratePlotTypes()
	print("Setting Plot Types (Lua Four Corners) ...");

	local layered_world = MultilayeredFractal.Create();
	local plot_list = layered_world:GeneratePlotsByRegion();
	
	SetPlotTypes(plot_list);

	local plot_list = layered_world:GeneratePlotsByRegion();
	local iW, iH = Map.GetGridSize();
	local firstRingYIsEven = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
	local firstRingYIsOdd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
	for x = iW / 2 - 4, iW / 2 + 3 do
		for y = 1, iH - 2 do
			local plot = Map.GetPlot(x, y)
			if plot:IsFlatlands() then -- Check for adjacent Mountain plot; if found, change this plot to Hills.
				local isEvenY, search_table = true, {};
				if y / 2 > math.floor(y / 2) then
				isEvenY = false;
				end
				if isEvenY then
					search_table = firstRingYIsEven;
				else
					search_table = firstRingYIsOdd;
				end

				for loop, plot_adjustments in ipairs(search_table) do
					local searchX, searchY;
					searchX = x + plot_adjustments[1];
					searchY = y + plot_adjustments[2];
					local searchPlot = Map.GetPlot(searchX, searchY)
					local plotType = searchPlot:GetPlotType()
					if plotType == PlotTypes.PLOT_MOUNTAIN then
						plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false)
						break
					end
				end
			end
		end
	end

	for y = iH / 2 - 4, iH / 2 + 3 do
		for x = 1, iW - 2 do
			local plot = Map.GetPlot(x, y)
			if plot:IsFlatlands() then -- Check for adjacent Mountain plot; if found, change this plot to Hills.
				local isEvenY, search_table = true, {};
				if y / 2 > math.floor(y / 2) then
				isEvenY = false;
				end
				if isEvenY then
					search_table = firstRingYIsEven;
				else
					search_table = firstRingYIsOdd;
				end

				for loop, plot_adjustments in ipairs(search_table) do
					local searchX, searchY;
					searchX = x + plot_adjustments[1];
					searchY = y + plot_adjustments[2];
					local searchPlot = Map.GetPlot(searchX, searchY)
					local plotType = searchPlot:GetPlotType()
					if plotType == PlotTypes.PLOT_MOUNTAIN then
						plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false)
						break
					end
				end
			end
		end
	end

	local args = {bExpandCoasts = false};
	GenerateCoasts(args);
end
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
function TerrainGenerator:GetLatitudeAtPlot(iX, iY)
	local lat = math.abs((self.iHeight / 2) - iY) / (self.iHeight / 2);
	lat = lat + (128 - self.variation:GetHeight(iX, iY))/(255.0 * 5.0);
	lat = 0.8 * (math.clamp(lat, 0, 1));
	return lat;
end
----------------------------------------------------------------------------------
function GenerateTerrain()
	print("Generating Terrain (Lua Four Corners) ...");
	
	-- Get Temperature setting input by user.
	local temp = Map.GetCustomOption(2)
	if temp == 4 then
		temp = 1 + Map.Rand(3, "Random Temperature - Lua");
	end

	local args = {temperature = temp};
	local terraingen = TerrainGenerator.Create(args);

	terrainTypes = terraingen:GenerateTerrain();
	
	SetTerrainTypes(terrainTypes);
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function GetRiverValueAtPlot(plot)
	local numPlots = PlotTypes.NUM_PLOT_TYPES;
	local sum = (numPlots - plot:GetPlotType()) * 20;
	local numDirections = DirectionTypes.NUM_DIRECTION_TYPES;
	for direction = 0, numDirections - 1, 1 do
		local adjacentPlot = Map.PlotDirection(plot:GetX(), plot:GetY(), direction);
		if (adjacentPlot ~= nil) then
			sum = sum + (numPlots - adjacentPlot:GetPlotType());
		else
			sum = 0 -- Custom, prevents rivers avoiding running off the map edge.
		end
	end
	sum = sum + Map.Rand(10, "River Rand");
	return sum;
end
------------------------------------------------------------------------------
function DoRiver(startPlot, thisFlowDirection, originalFlowDirection, riverID)
	-- Customizing to handle problems in top row of the map. Only this aspect has been altered.

	local iW, iH = Map.GetGridSize()
	thisFlowDirection = thisFlowDirection or FlowDirectionTypes.NO_FLOWDIRECTION;
	originalFlowDirection = originalFlowDirection or FlowDirectionTypes.NO_FLOWDIRECTION;

	-- pStartPlot = the plot at whose SE corner the river is starting
	if (riverID == nil) then
		riverID = nextRiverID;
		nextRiverID = nextRiverID + 1;
	end

	local otherRiverID = _rivers[startPlot]
	if (otherRiverID ~= nil and otherRiverID ~= riverID and originalFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION) then
		return; -- Another river already exists here; can't branch off of an existing river!
	end

	local riverPlot;
	
	local bestFlowDirection = FlowDirectionTypes.NO_FLOWDIRECTION;
	if (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTH) then
	
		riverPlot = startPlot;
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_EAST);
		if ( adjacentPlot == nil or riverPlot:IsWOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater() ) then
			return;
		end

		_rivers[riverPlot] = riverID;
		riverPlot:SetWOfRiver(true, thisFlowDirection);
		riverPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_NORTHEAST);
		
	elseif (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTHEAST) then
	
		riverPlot = startPlot;
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_SOUTHEAST);
		if ( adjacentPlot == nil or riverPlot:IsNWOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater() ) then
			return;
		end

		_rivers[riverPlot] = riverID;
		riverPlot:SetNWOfRiver(true, thisFlowDirection);
		-- riverPlot does not change
	
	elseif (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST) then
	
		riverPlot = Map.PlotDirection(startPlot:GetX(), startPlot:GetY(), DirectionTypes.DIRECTION_EAST);
		if (riverPlot == nil) then
			return;
		end
		
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_SOUTHWEST);
		if (adjacentPlot == nil or riverPlot:IsNEOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater()) then
			return;
		end

		_rivers[riverPlot] = riverID;
		riverPlot:SetNEOfRiver(true, thisFlowDirection);
		-- riverPlot does not change
	
	elseif (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_SOUTH) then
	
		riverPlot = Map.PlotDirection(startPlot:GetX(), startPlot:GetY(), DirectionTypes.DIRECTION_SOUTHWEST);
		if (riverPlot == nil) then
			return;
		end
		
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_EAST);
		if (adjacentPlot == nil or riverPlot:IsWOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater()) then
			return;
		end
		
		_rivers[riverPlot] = riverID;
		riverPlot:SetWOfRiver(true, thisFlowDirection);
		-- riverPlot does not change
	
	elseif (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST) then

		riverPlot = startPlot;
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_SOUTHEAST);
		if (adjacentPlot == nil or riverPlot:IsNWOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater()) then
			return;
		end
		
		_rivers[riverPlot] = riverID;
		riverPlot:SetNWOfRiver(true, thisFlowDirection);
		-- riverPlot does not change

	elseif (thisFlowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTHWEST) then
		
		riverPlot = startPlot;
		local adjacentPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_SOUTHWEST);
		
		if ( adjacentPlot == nil or riverPlot:IsNEOfRiver() or riverPlot:IsWater() or adjacentPlot:IsWater()) then
			return;
		end

		_rivers[riverPlot] = riverID;
		riverPlot:SetNEOfRiver(true, thisFlowDirection);
		riverPlot = Map.PlotDirection(riverPlot:GetX(), riverPlot:GetY(), DirectionTypes.DIRECTION_WEST);

	else
		-- River is starting here, set the direction in the next step
		riverPlot = startPlot;		
	end

	if (riverPlot == nil or riverPlot:IsWater()) then
		-- The river has flowed off the edge of the map or into the ocean. All is well.
		return; 
	end

	-- Storing X,Y positions as locals to prevent redundant function calls.
	local riverPlotX = riverPlot:GetX();
	local riverPlotY = riverPlot:GetY();
	
	-- Table of methods used to determine the adjacent plot.
	local adjacentPlotFunctions = {
		[FlowDirectionTypes.FLOWDIRECTION_NORTH] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_NORTHWEST); 
		end,
		
		[FlowDirectionTypes.FLOWDIRECTION_NORTHEAST] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_NORTHEAST);
		end,
		
		[FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_EAST);
		end,
		
		[FlowDirectionTypes.FLOWDIRECTION_SOUTH] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_SOUTHWEST);
		end,
		
		[FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_WEST);
		end,
		
		[FlowDirectionTypes.FLOWDIRECTION_NORTHWEST] = function() 
			return Map.PlotDirection(riverPlotX, riverPlotY, DirectionTypes.DIRECTION_NORTHWEST);
		end	
	}
	
	if(bestFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION) then

		-- Attempt to calculate the best flow direction.
		local bestValue = math.huge;
		for flowDirection, getAdjacentPlot in pairs(adjacentPlotFunctions) do
			
			if (GetOppositeFlowDirection(flowDirection) ~= originalFlowDirection) then
				
				if (thisFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION or
					flowDirection == TurnRightFlowDirections[thisFlowDirection] or 
					flowDirection == TurnLeftFlowDirections[thisFlowDirection]) then
				
					local adjacentPlot = getAdjacentPlot();
					
					if (adjacentPlot ~= nil) then
					
						local value = GetRiverValueAtPlot(adjacentPlot);
						if (flowDirection == originalFlowDirection) then
							value = (value * 3) / 4;
						end
						
						if (value < bestValue) then
							bestValue = value;
							bestFlowDirection = flowDirection;
						end

					-- Custom addition to fix river problems in top row of the map. Any other all-land map may need similar special casing.
					elseif adjacentPlot == nil and riverPlotY == iH - 1 then -- Top row of map, needs special handling
						if flowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTH or
						   flowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTHWEST or
						   flowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTHEAST then
							
							local value = Map.Rand(5, "River Rand");
							if (flowDirection == originalFlowDirection) then
								value = (value * 3) / 4;
							end
							if (value < bestValue) then
								bestValue = value;
								bestFlowDirection = flowDirection;
							end
						end

					-- Custom addition to fix river problems in left column of the map. Any other all-land map may need similar special casing.
					elseif adjacentPlot == nil and riverPlotX == 0 then -- Left column of map, needs special handling
						if flowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTH or
						   flowDirection == FlowDirectionTypes.FLOWDIRECTION_SOUTH or
						   flowDirection == FlowDirectionTypes.FLOWDIRECTION_NORTHWEST or
						   flowDirection == FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST then
							
							local value = Map.Rand(5, "River Rand");
							if (flowDirection == originalFlowDirection) then
								value = (value * 3) / 4;
							end
							if (value < bestValue) then
								bestValue = value;
								bestFlowDirection = flowDirection;
							end
						end
					end
				end
			end
		end
		
		-- Try a second pass allowing the river to "flow backwards".
		if(bestFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION) then
		
			local bestValue = math.huge;
			for flowDirection, getAdjacentPlot in pairs(adjacentPlotFunctions) do
			
				if (thisFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION or
					flowDirection == TurnRightFlowDirections[thisFlowDirection] or 
					flowDirection == TurnLeftFlowDirections[thisFlowDirection]) then
				
					local adjacentPlot = getAdjacentPlot();
					
					if (adjacentPlot ~= nil) then
						
						local value = GetRiverValueAtPlot(adjacentPlot);
						if (value < bestValue) then
							bestValue = value;
							bestFlowDirection = flowDirection;
						end
					end	
				end
			end
		end
	end
	
	--Recursively generate river.
	if (bestFlowDirection ~= FlowDirectionTypes.NO_FLOWDIRECTION) then
		if  (originalFlowDirection == FlowDirectionTypes.NO_FLOWDIRECTION) then
			originalFlowDirection = bestFlowDirection;
		end
		
		DoRiver(riverPlot, bestFlowDirection, originalFlowDirection, riverID);
	end
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function FeatureGenerator:GetLatitudeAtPlot(iX, iY)
	local lat = math.abs((self.iGridH/2) - iY)/(self.iGridH/2);
	local adjusted_lat = 0.8 * lat;
	return adjusted_lat
end
------------------------------------------------------------------------------
function FeatureGenerator:AddIceAtPlot(plot, iX, iY, lat)
	return
end
------------------------------------------------------------------------------
function AddFeatures()
	print("Adding Features (Lua Four Corners) ...");

	-- Get Rainfall setting input by user.
	local rain = Map.GetCustomOption(3)
	if rain == 4 then
		rain = 1 + Map.Rand(3, "Random Rainfall - Lua");
	end
	
	local args = {rainfall = rain}
	local featuregen = FeatureGenerator.Create(args);

	featuregen:AddFeatures();
end
------------------------------------------------------------------------------
function AssignStartingPlots:GenerateRegions(args)
	print("Map Generation - Dividing the map in to Regions");
	-- This is a customized version for West vs East.
	-- This version is tailored for handling two-teams play.
	local args = args or {};
	local iW, iH = Map.GetGridSize();
	local res = Map.GetCustomOption(13)
	if res == 9 then
		res = 1 + Map.Rand(3, "Random Resources Option - Lua");
	end

	local setback = Map.GetCustomOption(15)-1;

	local setforward = Map.GetCustomOption(16)-1;

	local setrange = setforward + setback;

	print("Moveback: ", setback);

	self.resource_setting = res; -- Each map script has to pass in parameter for Resource setting chosen by user.
	self.method = 3; -- Flag the map as using a Rectangular division method.
	self.start_locations = args.start_locations or 2;

	-- Determine number of civilizations and city states present in this game.
	self.iNumCivs, self.iNumCityStates, self.player_ID_list, self.bTeamGame, self.teams_with_major_civs, self.number_civs_per_team = GetPlayerAndTeamInfo()
	self.iNumCityStatesUnassigned = self.iNumCityStates;
	print("-"); print("Civs:", self.iNumCivs); print("City States:", self.iNumCityStates);

	-- Determine number of teams (of Major Civs only, not City States) present in this game.
	iNumTeams = table.maxn(self.teams_with_major_civs);				-- GLOBAL
	print("-"); print("Teams:", iNumTeams);

	-- If four teams are present, use team-oriented handling of start points, one team in each corner.
	if iNumTeams == 4 then
		print("-"); print("Number of Teams present is four! Using custom team start placement for DefianceCorners."); print("-");
		
		-- ToDo: Correctly identify team IDs and how many Civs are on each team.
		-- Also need to shuffle the teams so its random who starts on which half.
		iNumDivisions = 2

		-- Process the bottom left corner.
		self.inhabited_WestX = 0 + setforward;
		self.inhabited_SouthY = 0 + setforward;
		self.inhabited_Width = (math.floor(iW / 2)) - setrange;
		self.inhabited_Height = (math.floor(iH / 2)) - setrange;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumDivisions, fert_table, rect_table)

		-- Process the bottom right corner.
		self.inhabited_WestX = (math.floor(iW / 2)) + setback;
		self.inhabited_SouthY = 0 + setforward;
		self.inhabited_Width = (math.floor(iW / 2)) - setrange;
		self.inhabited_Height = (math.floor(iH / 2)) - setrange;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumDivisions, fert_table, rect_table)
		
		-- Process the top left corner.
		self.inhabited_WestX = 0 + setforward;
		self.inhabited_SouthY = (math.floor(iH / 2)) + setback;
		self.inhabited_Width = (math.floor(iW / 2)) - setrange;
		self.inhabited_Height = (math.floor(iH / 2)) - setrange;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumDivisions, fert_table, rect_table)

		-- Process the top right corner.
		self.inhabited_WestX = (math.floor(iW / 2)) + setback;
		self.inhabited_SouthY = (math.floor(iH / 2)) + setback;
		self.inhabited_Width = (math.floor(iW / 2)) - setrange;
		self.inhabited_Height = (math.floor(iH / 2)) - setrange;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumDivisions, fert_table, rect_table)
		-- The regions have been defined.

	-- If number of teams is any number other than two, use standard division.
	else	
		print("-"); print("Dividing the map at random."); print("-");
		self.method = 1;	
		local best_areas = {};
		local globalFertilityOfLands = {};

		-- Obtain info on all landmasses for comparision purposes.
		local iGlobalFertilityOfLands = 0;
		local iNumLandPlots = 0;
		local iNumLandAreas = 0;
		local land_area_IDs = {};
		local land_area_plots = {};
		local land_area_fert = {};
		-- Cycle through all plots in the world, checking their Start Placement Fertility and AreaID.
		for x = 0, iW - 1 do
			for y = 0, iH - 1 do
				local i = y * iW + x + 1;
				local plot = Map.GetPlot(x, y);
				if not plot:IsWater() then -- Land plot, process it.
					iNumLandPlots = iNumLandPlots + 1;
					local iArea = plot:GetArea();
					local plotFertility = self:MeasureStartPlacementFertilityOfPlot(x, y, true); -- Check for coastal land is enabled.
					iGlobalFertilityOfLands = iGlobalFertilityOfLands + plotFertility;
					--
					if TestMembership(land_area_IDs, iArea) == false then -- This plot is the first detected in its AreaID.
						iNumLandAreas = iNumLandAreas + 1;
						table.insert(land_area_IDs, iArea);
						land_area_plots[iArea] = 1;
						land_area_fert[iArea] = plotFertility;
					else -- This AreaID already known.
						land_area_plots[iArea] = land_area_plots[iArea] + 1;
						land_area_fert[iArea] = land_area_fert[iArea] + plotFertility;
					end
				end
			end
		end
		
		-- Sort areas, achieving a list of AreaIDs with best areas first.
		--
		-- Fertility data in land_area_fert is stored with areaID index keys.
		-- Need to generate a version of this table with indices of 1 to n, where n is number of land areas.
		local interim_table = {};
		for loop_index, data_entry in pairs(land_area_fert) do
			table.insert(interim_table, data_entry);
		end
		-- Sort the fertility values stored in the interim table. Sort order in Lua is lowest to highest.
		table.sort(interim_table);
		-- If less players than landmasses, we will ignore the extra landmasses.
		local iNumRelevantLandAreas = math.min(iNumLandAreas, self.iNumCivs);
		-- Now re-match the AreaID numbers with their corresponding fertility values
		-- by comparing the original fertility table with the sorted interim table.
		-- During this comparison, best_areas will be constructed from sorted AreaIDs, richest stored first.
		local best_areas = {};
		-- Currently, the best yields are at the end of the interim table. We need to step backward from there.
		local end_of_interim_table = table.maxn(interim_table);
		-- We may not need all entries in the table. Process only iNumRelevantLandAreas worth of table entries.
		for areaTestLoop = end_of_interim_table, (end_of_interim_table - iNumRelevantLandAreas + 1), -1 do
			for loop_index, AreaID in ipairs(land_area_IDs) do
				if interim_table[areaTestLoop] == land_area_fert[land_area_IDs[loop_index]] then
					table.insert(best_areas, AreaID);
					table.remove(land_area_IDs, landLoop);
					break
				end
			end
		end

		-- Assign continents to receive start plots. Record number of civs assigned to each landmass.
		local inhabitedAreaIDs = {};
		local numberOfCivsPerArea = table.fill(0, iNumRelevantLandAreas); -- Indexed in synch with best_areas. Use same index to match values from each table.
		for civToAssign = 1, self.iNumCivs do
			local bestRemainingArea;
			local bestRemainingFertility = 0;
			local bestAreaTableIndex;
			-- Loop through areas, find the one with the best remaining fertility (civs added 
			-- to a landmass reduces its fertility rating for subsequent civs).
			for area_loop, AreaID in ipairs(best_areas) do
				local thisLandmassCurrentFertility = land_area_fert[AreaID] / (1 + numberOfCivsPerArea[area_loop]);
				if thisLandmassCurrentFertility > bestRemainingFertility then
					bestRemainingArea = AreaID;
					bestRemainingFertility = thisLandmassCurrentFertility;
					bestAreaTableIndex = area_loop;
				end
			end
			-- Record results for this pass. (A landmass has been assigned to receive one more start point than it previously had).
			numberOfCivsPerArea[bestAreaTableIndex] = numberOfCivsPerArea[bestAreaTableIndex] + 1;
			if TestMembership(inhabitedAreaIDs, bestRemainingArea) == false then
				table.insert(inhabitedAreaIDs, bestRemainingArea);
			end
		end
				
		-- Loop through the list of inhabited landmasses, dividing each landmass in to regions.
		-- Note that it is OK to divide a continent with one civ on it: this will assign the whole
		-- of the landmass to a single region, and is the easiest method of recording such a region.
		local iNumInhabitedLandmasses = table.maxn(inhabitedAreaIDs);
		for loop, currentLandmassID in ipairs(inhabitedAreaIDs) do
			-- Obtain the boundaries of and data for this landmass.
			local landmass_data = ObtainLandmassBoundaries(currentLandmassID);
			local iWestX = landmass_data[1];
			local iSouthY = landmass_data[2];
			local iEastX = landmass_data[3];
			local iNorthY = landmass_data[4];
			local iWidth = landmass_data[5];
			local iHeight = landmass_data[6];
			local wrapsX = landmass_data[7];
			local wrapsY = landmass_data[8];
			-- Obtain "Start Placement Fertility" of the current landmass. (Necessary to do this
			-- again because the fert_table can't be built prior to finding boundaries, and we had
			-- to ID the proper landmasses via fertility to be able to figure out their boundaries.
			local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityOfLandmass(currentLandmassID, 
		  	                                         iWestX, iEastX, iSouthY, iNorthY, wrapsX, wrapsY);
			-- Assemble the rectangle data for this landmass.
			local rect_table = {iWestX, iSouthY, iWidth, iHeight, currentLandmassID, fertCount, plotCount};
			-- Divide this landmass in to number of regions equal to civs assigned here.
			iNumCivsOnThisLandmass = numberOfCivsPerArea[loop];
			if iNumCivsOnThisLandmass > 0 and iNumCivsOnThisLandmass <= 22 then -- valid number of civs.
				self:DivideIntoRegions(iNumCivsOnThisLandmass, fert_table, rect_table)
			else
				print("Invalid number of civs assigned to a landmass: ", iNumCivsOnThisLandmass);
			end
		end
	end
end
------------------------------------------------------------------------------
function AssignStartingPlots:ChooseLocations(args)
	print("Map Generation - Choosing Start Locations for Civilizations");
	local args = args or {};
	local iW, iH = Map.GetGridSize();
	local mustBeCoast = args.mustBeCoast or false; -- if true, will force all starts on salt water coast if possible
	
	-- Defaults for evaluating potential start plots are assigned in .Create but args
	-- passed in here can override. If args value for a field is nil (no arg) then
	-- these assignments will keep the default values in place.
	self.centerBias = args.centerBias or self.centerBias; -- % of radius from region center to examine first
	self.middleBias = args.middleBias or self.middleBias; -- % of radius from region center to check second
	print(string.format("DEV/SAPHT Center bias %d, %d", self.centerBias, self.middleBias))
	self.minFoodInner = args.minFoodInner or self.minFoodInner;
	self.minProdInner = args.minProdInner or self.minProdInner;
	self.minGoodInner = args.minGoodInner or self.minGoodInner;
	self.minFoodMiddle = args.minFoodMiddle or self.minFoodMiddle;
	self.minProdMiddle = args.minProdMiddle or self.minProdMiddle;
	self.minGoodMiddle = args.minGoodMiddle or self.minGoodMiddle;
	self.minFoodOuter = args.minFoodOuter or self.minFoodOuter;
	self.minProdOuter = args.minProdOuter or self.minProdOuter;
	self.minGoodOuter = args.minGoodOuter or self.minGoodOuter;
	self.maxJunk = args.maxJunk or self.maxJunk;

	-- Measure terrain/plot/feature in regions.
	self:MeasureTerrainInRegions()
	
	-- Determine region type.
	self:DetermineRegionTypes()

	-- Set up list of regions (to be processed in this order).
	--
	-- First, make a list of all average fertility values...
	local regionAssignList = {};
	local averageFertilityListUnsorted = {};
	local averageFertilityListSorted = {}; -- Have to make this a separate table, not merely a pointer to the first table.
	for i, region_data in ipairs(self.regionData) do
		local thisRegionAvgFert = region_data[8];
		table.insert(averageFertilityListUnsorted, {i, thisRegionAvgFert});
		table.insert(averageFertilityListSorted, thisRegionAvgFert);
	end
	-- Now sort the copy low to high.
	-- table.sort(averageFertilityListSorted);
	-- Finally, match each sorted fertilty value to the matching unsorted region number and record in sequence.
	local iNumRegions = table.maxn(averageFertilityListSorted);
	for region_order = 1, iNumRegions do
		for loop, data_pair in ipairs(averageFertilityListUnsorted) do
			local unsorted_fert = data_pair[2];
			if averageFertilityListSorted[region_order] == unsorted_fert then
				local unsorted_reg_num = data_pair[1];
				table.insert(regionAssignList, unsorted_reg_num);
				-- HAVE TO remove the entry from the table in rare case of ties on fert 
				-- value. Or it will just match this value for a second time, then crash 
				-- when the region it was tied with ends up with nil data.
				table.remove(averageFertilityListUnsorted, loop);
				break
			end
		end
	end

-----------------------------------------------------------------------------------------

	for assignIndex = 1, iNumRegions do
		local currentRegionNumber = regionAssignList[assignIndex];
		local bSuccessFlag = false;
		local bForcedPlacementFlag = false;
		
		print("Region #" .. currentRegionNumber);
		local player_ID = self.player_ID_list[assignIndex];
		local player = Players[player_ID];
		local civType = GameInfo.Civilizations[player:GetCivilizationType()].Type;
		local bNeedsCoastalStart = CivNeedsCoastalStart(civType);
		if bNeedsCoastalStart then
			bSuccessFlag, bForcedPlacementFlag = self:FindCoastalStart(currentRegionNumber)
		else
			bSuccessFlag, bForcedPlacementFlag = self:FindStart(currentRegionNumber, self.NoCoastInland)
		end
	end
	--[[ Printout of start plots. Debug use only.
	print("-");
	print("--- Table of results, New Start Finder ---");
	for loop, startData in ipairs(self.startingPlots) do
		print("-");
		print("Region#", loop, " has start plot at: ", startData[1], startData[2], "with Fertility Rating of ", startData[3]);
	end
	print("-");
	print("--- Table of results, New Start Finder ---");
	print("-");
	]]--
	
	--[[ Printout of Impact and Ripple data.
	print("--- Impact and Ripple ---");
	PrintContentsOfTable(self.distanceData)
	print("-");  ]]--
end
------------------------------------------------------------------------------
function AssignStartingPlots:BalanceAndAssign()
	-- This function determines what level of Bonus Resource support a location
	-- may need, identifies compatibility with civ-specific biases, and places starts.

	-- Normalize each start plot location.
	local iNumStarts = table.maxn(self.startingPlots);
	for region_number = 1, iNumStarts do
		self:NormalizeStartLocation(region_number)
	end

	-- Assign Civs to start plots.
	if iNumTeams == 4 then
		-- Four teams, place one in each corner.
		print("-"); print("This is a team game with four teams! Place one team in each corner."); print("-");
		local playerList = {};
		for loop = 1, self.iNumCivs do
			local player_ID = self.player_ID_list[loop];
			table.insert(playerList, player_ID);
			local player = Players[player_ID];
		end
		
		for region_number, player_ID in ipairs(playerList) do
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			local start_plot = Map.GetPlot(x, y)
			local player = Players[player_ID]
			player:SetStartingPlot(start_plot)
		end
	else
		print("-"); print("This game does not have specific start zone assignments."); print("-");
		local playerList = {};
		for loop = 1, self.iNumCivs do
			local player_ID = self.player_ID_list[loop];
			table.insert(playerList, player_ID);
		end
		local playerListShuffled = GetShuffledCopyOfTable(playerList)
		for region_number, player_ID in ipairs(playerListShuffled) do
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			local start_plot = Map.GetPlot(x, y)
			local player = Players[player_ID]
			player:SetStartingPlot(start_plot)
		end
		-- If this is a team game (any team has more than one Civ in it) then make 
		-- sure team members start near each other if possible. (This may scramble 
		-- Civ biases in some cases, but there is no cure).
		if self.bTeamGame == true and team_setting ~= 2 then
			print("However, this IS a team game, so we will try to group team members together."); print("-");
			self:NormalizeTeamLocations()
		end
	end
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function AssignStartingPlots:CanPlaceCityStateAt(x, y, area_ID, force_it, ignore_collisions)
	local iW, iH = Map.GetGridSize();
	local plot = Map.GetPlot(x, y)
	-- Adding this check for Four Corners
	if x < 1 or x >= iW - 1 or y < 1 or y >= iH - 1 then
		return false
	end

	local area = plot:GetArea()
	local biggest_area = Map.FindBiggestArea(False);
	local iAreaID = biggest_area:GetID();

	-- if area_ID ~= iAreaID then
	--	return false
	-- end

	if area ~= area_ID and area_ID ~= -1 then
		return false
	end

	-- Moving CS away from map center
	if x > math.floor(iW / 4) and x <= iW - math.floor(iW / 4) and y > math.floor(iH / 4) and y <= iW - math.floor(iW / 4) then
		return false
	end
	--

	local plotType = plot:GetPlotType()
	if plotType == PlotTypes.PLOT_OCEAN or plotType == PlotTypes.PLOT_MOUNTAIN then
		return false
	end
	local terrainType = plot:GetTerrainType()
	if terrainType == TerrainTypes.TERRAIN_SNOW then
		return false
	end
	local featureType = plot:GetFeatureType()
	if featureType == FeatureTypes.FEATURE_OASIS then
		return false
	end
	local plotIndex = y * iW + x + 1;
	if self.cityStateData[plotIndex] > 0 and force_it == false then
		return false
	end
	if self.playerCollisionData[plotIndex] == true and ignore_collisions == false then
		--print("-"); print("City State candidate plot rejected: collided with already-placed civ or City State at", x, y);
		return false
	end
	if self.plotDataIsNextToCoast[plotIndex] == true then
		return false
	end
	return true
end
------------------------------------------------------------------------------
function StartPlotSystem()
	-- Get Resources setting input by user.
	local res = Map.GetCustomOption(13)
	if res == 9 then
		res = 1 + Map.Rand(3, "Random Resources Option - Lua");
	end

	print("Creating start plot database.");
	local start_plot_database = AssignStartingPlots.Create()

	print("Dividing the map in to Regions.");
	-- Regional Division Method 3: Rectangular
	local args = {
		method = 3,
		resources = res,
		};
	start_plot_database:GenerateRegions()

	print("Choosing start locations for civilizations.");
	start_plot_database:ChooseLocations()
	
	print("Normalizing start locations and assigning them to Players.");
	start_plot_database:BalanceAndAssign()

	--print("Placing Natural Wonders.");
	--start_plot_database:PlaceNaturalWonders()

	print("Placing Natural Wonders.");
	local wonders = Map.GetCustomOption(7)
	if wonders == 14 then
		wonders = Map.Rand(13, "Number of Wonders To Spawn - Lua");
	else
		wonders = wonders - 1;
	end

	print("########## Wonders ##########");
	print("Natural Wonders To Place: ", wonders);

	local wonderargs = {
		wonderamt = wonders,
	};

	start_plot_database:PlaceNaturalWonders(wonderargs)

	print("Placing Resources and City States.");
	start_plot_database:PlaceResourcesAndCityStates()
end
------------------------------------------------------------------------------

