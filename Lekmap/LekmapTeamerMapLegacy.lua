------------------------------------------------------------------------------
--	FILE:	 West_vs_East.lua
--	AUTHOR:  Bob Thomas
--	PURPOSE: Regional map script - Designed to pit two teams against each other
--	         with a strip of water dividing the map east from west.
------------------------------------------------------------------------------
--	Copyright (c) 2010 Firaxis Games, Inc. All rights reserved.
------------------------------------------------------------------------------

include("HBMapGeneratorMirrored");
include("HBFractalWorld");
include("HBFeatureGeneratorMirrored");
include("HBTerrainGenerator");
include("IslandMaker");
include("MultilayeredFractal");


------------------------------------------------------------------------------
function GetMapScriptInfo()
	local world_age, temperature, rainfall, sea_level, resources = GetCoreMapOptions()
	return {
		Name = "Lekmap: Teamer Map (Bonus Map)",
		Description = "HellBlazers Teamer Map combined with Skirmish. Uses Leszek Deska's mirroring algorithm. Frankensteined together by Meota.",
		IsAdvancedMap = false,
		IconIndex = 17,
		SortIndex = 2,
		SupportsMultiplayer = true,
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
					"20 - Duel",
					"22",
					"24",
					"26",
					"28",
					"30 - 4 players",
					"32",
					"34 - Duel with back coast",
					"36 - 6 players",
					"38",
					"40 - 4 players with back coast",
					"42",
					"44 - 8 players; 6 players with back coast",
					"46",
					"48",
					"50 - 8 players with back coast",
					"52",
					"54",
				},

				DefaultValue = 9,
				SortPriority = -89,
			},

			{
				Name = "Land Size Y",	-- add setting for land type (12) +12
				Values = {
					"14 - Duel",
					"16",
					"18",
					"20 - 4 players; Duel with north/south coast",
					"22",
					"24",
					"26 - 6 players; 4 players with north/south coast",
					"28",
					"30 - 8 players",
					"32 - 6 players with north/south coast",
					"34",
					"36 - 8 players with north/south coast",
					"38",
					"40",
				},

				DefaultValue = 7,
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
				Name = "TXT_KEY_MAP_OPTION_TEAM_SETTING", -- (15)
				Values = {
					"TXT_KEY_MAP_OPTION_START_TOGETHER",
--					"TXT_KEY_MAP_OPTION_START_SEPARATED",
					"TXT_KEY_MAP_OPTION_START_ANYWHERE",
				},
				DefaultValue = 1,
				SortPriority = 1,
			},
			{
				Name = "Frontline Distance", --  minimum distance a capital can spawn from the front (16)
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
				Name = "Back Distance", -- minimum distance a capital can spawn from the back (17)
				Values = {
					"0 - Default",
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
				},
				DefaultValue = 1,
				SortPriority = -97,
			},
			{
				Name = "Center Split", -- (18)
				Values = {
					"Ocean Strip",
					"Landbridges",
					"Marsh",
					"Snow",
					"Normal Land",
					"Skirmish",
				},
				DefaultValue = 1,
				SortPriority = -99,
			},
			{
				Name = "Back Coasts", -- decide whether or not the map should have oceans on the x world borders (19)
				Values = {
					"Yes",
					"No",
				},
				DefaultValue = 2,
				SortPriority = -102,
			},
			{
				Name = "North and South Coasts", -- decide whether or not the map should have oceans on the y world borders (20)
				Values = {
					"Yes",
					"No",
				},
				DefaultValue = 2,
				SortPriority = -103,
			},
			{
				Name = "World Wrap", -- decide whether or not the map should wrap over the x world borders (21)
				Values = {
					"Yes",
					"No",
				},
				DefaultValue = 2,
				SortPriority = -104,
			},
		},
	};
end
------------------------------------------------------------------------------

-------------------------------------------------------------------------------
function GetMapInitData(worldSize)
	-- This function can reset map grid sizes or world wrap settings.
	--
	-- East vs West is an extremely compact multiplayer map type.

	local LandSizeX = 18 + (Map.GetCustomOption(11) * 2);
	local LandSizeY = 12 + (Map.GetCustomOption(12) * 2);
	local AnyTruers = Map.GetCustomOption(21);
	local ShouldWrap = false;

	if AnyTruers == 1 then
		ShouldWrap = true;
	elseif AnyTruers == 2 then
		ShouldWrap = false;
	end
	
	local worldsizes = {
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
	if(world ~= nil) then
	return {
		Width = grid_size[1],
		Height = grid_size[2],
		WrapX = ShouldWrap,
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
	local SplitOps = Map.GetCustomOption(18);
	local BackOcean = Map.GetCustomOption(19);
	local NorthSouthOcean = Map.GetCustomOption(20);

	-- Fill all rows with land plots.
	self.wholeworldPlotTypes = table.fill(PlotTypes.PLOT_LAND, iW * iH);

	if SplitOps == 1 then
	
		-- Add strip of ocean to middle of map --- Always start with this for civ placements
		for y = 0, iH - 1 do
			for x = math.floor(iW / 2) - 2, math.floor(iW / 2) + 1 do
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
	elseif SplitOps == 2 then
		
		-- Add strip of ocean to middle of map --- Always start with this for civ placements
		for y = 4, iH - 5 do
			for x = math.floor(iW / 2) - 2, math.floor(iW / 2) + 1 do
				local plotIndex = y * iW + x + 1;
				if y >= math.floor(iH / 2) - 2 and y <= math.floor(iH / 2) + 1 then
					if x == math.floor(iW / 2) or x == math.floor(iW / 2) - 1 then
						--self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					end
				else
					self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
				end
			end
		end
	end

	if BackOcean == 1 then
	-- Add strips of ocean to the x world borders.
		local west_half = {};
		for loop = 1, iH - 2 do
			table.insert(west_half, loop);
		end
		local west_shuffled = GetShuffledCopyOfTable(west_half)
		local iNumMountainsPerColumn = math.max(math.floor(iH * 0.33), math.floor((iH / 3) - 1));
		local x_west = 3;
		for loop = 1, iNumMountainsPerColumn do
			local y_west = west_shuffled[loop];
			local i_west_plot = y_west * iW + x_west + 1;
			self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_OCEAN;
		end

		for y = 0, iH do
			for x = 0, 2 do
				local plotIndex = y * iW + x + 1;
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		for y = 0, iH do
			for x = iW - 3, iW do
				local plotIndex = y * iW + x + 1;
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
	if NorthSouthOcean == 1 then
	-- Add strips of ocean to the y world borders.
		local south_half = {};
		for loop = 1, iW - 2 do
			table.insert(south_half, loop);
		end
		local south_shuffled = GetShuffledCopyOfTable(south_half)
		local iNumMountainsPerColumn = math.max(math.floor(iW * 0.33), math.floor((iW / 3) - 1));
		local y_south = 3;
		for loop = 1, iNumMountainsPerColumn do
			local x_south = south_shuffled[loop];
			local i_south_plot = y_south * iW + x_south + 1;
			self.wholeworldPlotTypes[i_south_plot] = PlotTypes.PLOT_OCEAN;
		end
		local north_half = {};
		for loop = 1, iW - 2 do
			table.insert(north_half, loop);
		end
		local north_shuffled = GetShuffledCopyOfTable(north_half)
		local iNumMountainsPerColumn = math.max(math.floor(iW * 0.33), math.floor((iW / 3) - 1));
		local y_north = iH - 4;
		for loop = 1, iNumMountainsPerColumn do
			local x_north = north_shuffled[loop];
			local i_north_plot = y_north * iW + x_north + 1;
			self.wholeworldPlotTypes[i_north_plot] = PlotTypes.PLOT_OCEAN;
		end
		for x = 0, iW do
			for y = 0, 2 do
				local plotIndex = y * iW + x + 1;
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		for x = 0, iW do
			for y = iH - 3, iH do
				local plotIndex = y * iW + x + 1;
				self.wholeworldPlotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
	end

	-- Add lakes.
	local lakesFrac = Fractal.Create(iW, iH, lake_grain, fracFlags, 6, 6);
	local iLakesThreshold = lakesFrac:GetHeight(92);
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = y * iW + x + 1; -- add one because Lua arrays start at 1
			local lakeVal = lakesFrac:GetHeight(x, y);
			if lakeVal >= iLakesThreshold then
				--self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
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
	
	if SplitOps == 6 then
		for x = iW / 2 - 2, iW / 2 + 1 do
			for y = 1, iH - 2 do
				local i = y * iW + x + 1;
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
			end
		end
		for x = iW / 2 - 2, iW / 2 + 1 do
			local i = x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
			local i = (iH - 1) * iW + x + 1;
			self.wholeworldPlotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
		end
		local west_half, east_half = {}, {};
		for loop = 1, iH - 2 do
			table.insert(west_half, loop);
			table.insert(east_half, loop);
		end
		local west_shuffled = GetShuffledCopyOfTable(west_half)
		local east_shuffled = GetShuffledCopyOfTable(east_half)
		local iNumMountainsPerColumn = math.max(math.floor(iH * 0.225), math.floor((iH / 4) - 1));
		local x_west, x_east = iW / 2 - 1, iW / 2;
		for loop = 1, iNumMountainsPerColumn do
			local y_west, y_east = west_shuffled[loop], iH - 1- west_shuffled[loop];
			local i_west_plot = y_west * iW + x_west + 1;
			local i_east_plot = y_east * iW + x_east + 1;
			self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_MOUNTAIN;
			self.wholeworldPlotTypes[i_east_plot] = PlotTypes.PLOT_MOUNTAIN;
		end
	end
	if SplitOps == 4 then
		for x = iW / 2 - 4, iW / 2 + 3 do
			for y = 0, iH - 1 do
				local i = y * iW + x + 1;
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_LAND;
			end
		end
		local west_half, east_half = {}, {};
		for loop = 1, iH - 2 do
			table.insert(west_half, loop);
			table.insert(east_half, loop);
		end
		local west_shuffled = GetShuffledCopyOfTable(west_half)
		local east_shuffled = GetShuffledCopyOfTable(east_half)
		local iNumMountainsPerColumn = math.max(math.floor(iH * 0.45), math.floor((iH / 2) - 1));
		local x_west, x_east = iW / 2 - 4, iW / 2 + 3;
		for loop = 1, iNumMountainsPerColumn do
			local y_west, y_east = west_shuffled[loop], iH - 1- west_shuffled[loop];
			local i_west_plot = y_west * iW + x_west + 1;
			local i_east_plot = y_east * iW + x_east + 1;
			self.wholeworldPlotTypes[i_west_plot] = PlotTypes.PLOT_MOUNTAIN;
			self.wholeworldPlotTypes[i_east_plot] = PlotTypes.PLOT_MOUNTAIN;
		end
	end
	-- Plot Type generation completed. Return global plot array.
	return self.wholeworldPlotTypes
end
------------------------------------------------------------------------------
function GeneratePlotTypes()
	print("Setting Plot Types (Lua West vs East) ...");

	local layered_world = MultilayeredFractal.Create();
	local plot_list = layered_world:GeneratePlotsByRegion();
	local SplitOps = Map.GetCustomOption(18);

	SetPlotTypes(plot_list);

	if SplitOps == 4 then
		local plot_list = layered_world:GeneratePlotsByRegion();
		local iW, iH = Map.GetGridSize();
		local firstRingYIsEven = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
		local firstRingYIsOdd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
		for x = iW / 2 - 5, iW / 2 + 4 do
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
	end
	if SplitOps == 6 then
		local iW, iH = Map.GetGridSize();
		local firstRingYIsEven = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
		local firstRingYIsOdd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
		for x = iW / 2 - 2, iW / 2 + 1 do
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
	print("Generating Terrain (Lua West vs East) ...");
	
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
	print("Adding Features (Lua West vs East) ...");

	-- Get Rainfall setting input by user.
	local rain = Map.GetCustomOption(3)
	if rain == 4 then
		rain = 1 + Map.Rand(3, "Random Rainfall - Lua");
	end
	
	local args = {rainfall = rain}
	local featuregen = FeatureGenerator.Create(args);

	featuregen:AddFeatures(false);
end
------------------------------------------------------------------------------
------------------------------------------------------------------------------
function GetRiverValueAtPlot(plot)
	-- Custom method to force rivers to flow away from the map center.
	local iW, iH = Map.GetGridSize()
	local x = plot:GetX()
	local y = plot:GetY()
	local random_factor = Map.Rand(3, "River direction random factor - Skirmish LUA");
	local direction_influence_value = (math.abs(iW - (x - (iW / 2))) + ((math.abs(y - (iH / 2))) / 3)) * random_factor;

	local numPlots = PlotTypes.NUM_PLOT_TYPES;
	local sum = ((numPlots - plot:GetPlotType()) * 20) + direction_influence_value;

	local numDirections = DirectionTypes.NUM_DIRECTION_TYPES;
	for direction = 0, numDirections - 1 do
		local adjacentPlot = Map.PlotDirection(plot:GetX(), plot:GetY(), direction);
		if (adjacentPlot ~= nil) then
			sum = sum + (numPlots - adjacentPlot:GetPlotType());
		else
			sum = sum + (numPlots * 10);
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

					-- Custom addition for Highlands, to fix river problems in top row of the map. Any other all-land map may need similar special casing.
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

					-- Custom addition for Highlands, to fix river problems in left column of the map. Any other all-land map may need similar special casing.
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
function AddRivers()

	-- Customization for Skirmish, to keep river starts away from buffer zone in middle columns of map, and set river "original flow direction".
	local iW, iH = Map.GetGridSize()
	print("Skirmish - Adding Rivers");
	local passConditions = {
		function(plot)
			return plot:IsHills() or plot:IsMountain();
		end,
		
		function(plot)
			return (not plot:IsCoastalLand()) and (Map.Rand(8, "HBTMapGenerator AddRivers") == 0);
		end,
		
		function(plot)
			local area = plot:Area();
			local plotsPerRiverEdge = GameDefines["PLOTS_PER_RIVER_EDGE"];
			return (plot:IsHills() or plot:IsMountain()) and (area:GetNumRiverEdges() <	((area:GetNumTiles() / plotsPerRiverEdge) + 1));
		end,
		
		function(plot)
			local area = plot:Area();
			local plotsPerRiverEdge = GameDefines["PLOTS_PER_RIVER_EDGE"];
			return (area:GetNumRiverEdges() < (area:GetNumTiles() / plotsPerRiverEdge) + 1);
		end
	}
	for iPass, passCondition in ipairs(passConditions) do
		local riverSourceRange;
		local seaWaterRange;
		if (iPass <= 2) then
			riverSourceRange = GameDefines["RIVER_SOURCE_MIN_RIVER_RANGE"];
			seaWaterRange = GameDefines["RIVER_SOURCE_MIN_SEAWATER_RANGE"];
		else
			riverSourceRange = (GameDefines["RIVER_SOURCE_MIN_RIVER_RANGE"] / 2);
			seaWaterRange = (GameDefines["RIVER_SOURCE_MIN_SEAWATER_RANGE"] / 2);
		end
		for i, plot in Plots() do
			local current_x = plot:GetX()
			local current_y = plot:GetY()
			if current_x < 1 or current_x >= iW - 2 or current_y < 2 or current_y >= iH - 1 then
				-- Plot too close to edge, ignore it.
			elseif current_x >= (iW / 2) - 3 and current_x <= (iW / 2) + 2 then
				-- Plot in buffer zone, ignore it.
			elseif (not plot:IsWater()) then
				if(passCondition(plot)) then
					if (not Map.FindWater(plot, riverSourceRange, true)) then
						if (not Map.FindWater(plot, seaWaterRange, false)) then
							local inlandCorner = plot:GetInlandCorner();
							if(inlandCorner) then
								local start_x = inlandCorner:GetX()
								local start_y = inlandCorner:GetY()
								local orig_direction;
								if start_y < iH / 2 then -- South half of map
									if start_x < iW / 2 then -- West half of map
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_NORTHWEST;
									else -- East half
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_NORTHEAST;
									end
								else -- North half of map
									if start_x < iW / 2 then -- West half of map
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST;
									else -- NE corner
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST;
									end
								end
								DoRiver(inlandCorner, nil, orig_direction, nil);
							end
						end
					end
				end			
			end
		end
	end		
end
------------------------------------------------------------------------------------------------------------------------------------------------------------
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

	local setback = Map.GetCustomOption(16)-1;

	local setforward = Map.GetCustomOption(17)-1;

	local setrange = setforward + setback;

	print("Moveback: ", setback);

	self.resource_setting = res; -- Each map script has to pass in parameter for Resource setting chosen by user.
	self.method = 3; -- Flag the map as using a Rectangular division method.

	-- Determine number of civilizations and city states present in this game.
	self.iNumCivs, self.iNumCityStates, self.player_ID_list, self.bTeamGame, self.teams_with_major_civs, self.number_civs_per_team = GetPlayerAndTeamInfo()
	self.iNumCityStatesUnassigned = self.iNumCityStates;
	print("-"); print("Civs:", self.iNumCivs); print("City States:", self.iNumCityStates);

	-- Determine number of teams (of Major Civs only, not City States) present in this game.
	iNumTeams = table.maxn(self.teams_with_major_civs);				-- GLOBAL
	print("-"); print("Teams:", iNumTeams);

	-- Fetch team setting.
	local team_setting = Map.GetCustomOption(15)

	-- If two teams are present, use team-oriented handling of start points, one team west, one east.
	if iNumTeams == 2 and team_setting == 1 then
		print("-"); print("Number of Teams present is two! Using custom team start placement for West vs East."); print("-");
		
		-- ToDo: Correctly identify team IDs and how many Civs are on each team.
		-- Also need to shuffle the teams so its random who starts on which half.
		local shuffled_team_list = GetShuffledCopyOfTable(self.teams_with_major_civs)
		teamWestID = self.teams_with_major_civs[1];							-- GLOBAL
		teamEastID = self.teams_with_major_civs[2]; 						-- GLOBAL
		iNumCivsInWest = self.number_civs_per_team[teamWestID];		-- GLOBAL
		iNumCivsInEast = self.number_civs_per_team[teamEastID];		-- GLOBAL

		-- Process the team in the west.
		self.inhabited_WestX = 0 + setforward;
		self.inhabited_SouthY = 0;
		self.inhabited_Width = (math.floor(iW / 2)) - setrange;
		self.inhabited_Height = iH;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumCivsInWest, fert_table, rect_table)

		-- Process the team in the east.
		self.inhabited_WestX = (math.floor(iW / 2)) + setback;
		self.inhabited_SouthY = 0;
		self.inhabited_Width = math.floor(iW / 2) - setrange;
		self.inhabited_Height = iH;
		-- Obtain "Start Placement Fertility" inside the rectangle.
		-- Data returned is: fertility table, sum of all fertility, plot count.
		local fert_table, fertCount, plotCount = self:MeasureStartPlacementFertilityInRectangle(self.inhabited_WestX, 
		                                         self.inhabited_SouthY, self.inhabited_Width, self.inhabited_Height)
		-- Assemble the Rectangle data table:
		local rect_table = {self.inhabited_WestX, self.inhabited_SouthY, self.inhabited_Width, 
		                    self.inhabited_Height, -1, fertCount, plotCount}; -- AreaID -1 means ignore area IDs.
		-- Divide the rectangle.
		self:DivideIntoRegions(iNumCivsInEast, fert_table, rect_table)
		-- The regions have been defined.

	-- If number of teams is any number other than two, use standard One Landmass division.
	else	
		print("-"); print("Dividing the map at random."); print("-");
		self.method = 2;	
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
function AssignStartingPlots:BalanceAndAssign()
	-- This function determines what level of Bonus Resource support a location
	-- may need, identifies compatibility with civ-specific biases, and places starts.

	-- Normalize each start plot location.
	local iNumStarts = table.maxn(self.startingPlots);
	for region_number = 1, iNumStarts do
		print("Normalize Region: ", region_number);
		self:NormalizeStartLocation(region_number)
	end

	-- Assign Civs to start plots.
	local team_setting = Map.GetCustomOption(15)
	if iNumTeams == 2 and team_setting == 1 then
		-- Two teams, place one in the west half, other in east -- even if team membership totals are uneven.
		print("-"); print("This is a team game with two teams! Place one team in West, other in East."); print("-");
		local playerList, westList, eastList = {}, {}, {};
		for loop = 1, self.iNumCivs do
			local player_ID = self.player_ID_list[loop];
			table.insert(playerList, player_ID);
			local player = Players[player_ID];
			local team_ID = player:GetTeam()
			if team_ID == teamWestID then
				print("Player #", player_ID, "belongs to Team #", team_ID, "and will be placed in the North.");
				table.insert(westList, player_ID);
			elseif team_ID == teamEastID then
				print("Player #", player_ID, "belongs to Team #", team_ID, "and will be placed in the South.");
				table.insert(eastList, player_ID);
			else
				print("* ERROR * - Player #", player_ID, "belongs to Team #", team_ID, "which is neither West nor East!");
			end
		end
		
		-- Debug
		if table.maxn(westList) ~= iNumCivsInWest then
			print("-"); print("*** ERROR! *** . . . Mismatch between number of Civs on West team and number of civs assigned to west locations.");
		end
		if table.maxn(eastList) ~= iNumCivsInEast then
			print("-"); print("*** ERROR! *** . . . Mismatch between number of Civs on East team and number of civs assigned to east locations.");
		end
		
		local westListShuffled = GetShuffledCopyOfTable(westList)
		local eastListShuffled = GetShuffledCopyOfTable(eastList)
		for region_number, player_ID in ipairs(westListShuffled) do
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			local start_plot = Map.GetPlot(x, y)
			local player = Players[player_ID]
			player:SetStartingPlot(start_plot)
		end
		for loop, player_ID in ipairs(eastListShuffled) do
			local x = self.startingPlots[loop + iNumCivsInWest][1];
			local y = self.startingPlots[loop + iNumCivsInWest][2];
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
	-- Overriding default city state placement to prevent city states from being placed too close to map edges.
	
	--disable city states
	if 1<2 then
		return false
	end

	local iW, iH = Map.GetGridSize();
	local plot = Map.GetPlot(x, y)
	local area = plot:GetArea()
	
	-- Adding this check for West vs East.
	if x < 1 or x >= iW - 1 or y < 1 or y >= iH - 1 then
		return false
	end
	--
	
	if area ~= area_ID and area_ID ~= -1 then
		return false
	end
	local plotType = plot:GetPlotType()
	if plotType == PlotTypes.PLOT_OCEAN or plotType == PlotTypes.PLOT_MOUNTAIN then
		return false
	end
	local terrainType = plot:GetTerrainType()
	if terrainType == TerrainTypes.TERRAIN_SNOW then
		return false
	end
	local plotIndex = y * iW + x + 1;
	if self.cityStateData[plotIndex] > 0 and force_it == false then
		return false
	end
	local plotIndex = y * iW + x + 1;
	if self.playerCollisionData[plotIndex] == true and ignore_collisions == false then
		--print("-"); print("City State candidate plot rejected: collided with already-placed civ or City State at", x, y);
		return false
	end
	return true
end
------------------------------------------------------------------------------------------------------------------------------------------------------------
function SetDivide()

	local SplitOps = Map.GetCustomOption(18);
	local iW, iH = Map.GetGridSize();

	if SplitOps == 2 then
		-- check landbridges have no lakes or moutains

		--check bottom land bridge
		for y = 0, 2 do
			for x = math.floor(iW / 2) - 4, math.floor(iW / 2) + 3 do
				local plot = Map.GetPlot(x, y)
				
				--check for mountain or lake
				if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
					plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false);
				elseif plot:IsLake() then
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, false);
				end
			end
		end

		--check the top landbridge
		for y = (iH-3), (iH-1) do
			for x = math.floor(iW / 2) - 4, math.floor(iW / 2) + 3 do
				local plot = Map.GetPlot(x, y)
				
				--check for mountain or lake
				if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
					plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false);
				elseif plot:IsLake() then
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, false);
				end
			end
		end

		--check the middle landbridge
		for y = math.floor(iH / 2) - 1, math.floor(iH / 2) + 1 do
			for x = math.floor(iW / 2) - 4, math.floor(iW / 2) + 3 do
				local plot = Map.GetPlot(x, y)
				
				--check for mountain or lake
				if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
					plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false);
				elseif plot:IsLake() then
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, false);
				end
			end
		end
	elseif SplitOps == 3 then
		--Marsh
		
		-- Add strip of marsh to middle of map
		for y = 0, iH - 1 do
			for x = math.floor(iW / 2) - 2, math.floor(iW / 2) + 1 do
				local plot = Map.GetPlot(x, y)
				plot:SetPlotType(PlotTypes.PLOT_LAND, false, false);
				plot:SetTerrainType(TerrainTypes.TERRAIN_GRASS, false, false);
				plot:SetFeatureType(FeatureTypes.FEATURE_MARSH, -1);
			end
		end
	elseif SplitOps == 4 then
		--Snow

		-- Add strip of tundra to middle of map
		for y = 0, iH - 1 do
			for x = math.floor(iW / 2) - 3, math.floor(iW / 2) + 2 do
				local plot = Map.GetPlot(x, y)
				plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1);
				plot:SetTerrainType(TerrainTypes.TERRAIN_TUNDRA, false, false);
			end
		end
		-- Add strip of snow to middle of map
		for y = 0, iH - 1 do
			for x = math.floor(iW / 2) - 2, math.floor(iW / 2) + 1 do
				local plot = Map.GetPlot(x, y)
				if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
					plot:SetPlotType(PlotTypes.PLOT_HILLS, false, false);
				elseif plot:IsLake() then
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, false);
				end
				plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1);
				plot:SetTerrainType(TerrainTypes.TERRAIN_SNOW, false, false);
			end
		end
	end
end

------------------------------------------------------------------------------
function getMirroredPlot(plot)
	local iW, iH = Map.GetGridSize();
	local x = iW - plot:GetX() - 1;
	local y = iH - plot:GetY() - 1;
	print("mirror x/y=",x,y);
	local mirrorPlot = Map.GetPlot(x, y);
	return mirrorPlot;
end
------------------------------------------------------------------------------
function isValidPlayer(pPlayer)
	return  pPlayer ~= nil and pPlayer:GetStartingPlot() ~= nil and pPlayer:IsAlive();
end
------------------------------------------------------------------------------
function StartPlotSystem()
	-- Get Resources setting input by user.
	local MirroredMapPlease;
	MirroredMapPlease = 1;
	local res = Map.GetCustomOption(13)
	if res == 9 then
		res = 1 + Map.Rand(3, "Random Resources Option - Lua");
	end

	print("Creating start plot database.");
	local start_plot_database = AssignStartingPlots.Create()

	print("Dividing the map in to Regions.");
	print("Resource Setting: ", res);
	local args = {
		resources = res,
		};
	start_plot_database:GenerateRegions()

	-- need a way to add the middle split options here
	print("Setting divide section of map");
	SetDivide()

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
	
	if Map.GetCustomOption(18) == 4 then
		local iW, iH = Map.GetGridSize()
		for x = math.floor(iW / 2) - 2, (iW / 2) + 1 do
			for y = 0, iH - 1 do
				local plot = Map.GetPlot(x, y)
				plot:SetWOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
				plot:SetNWOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
				plot:SetNEOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
			end
		end
	end
	if MirroredMapPlease == 1 then
	------------------------------------------------------------------------------
	----------------------- INCLUDE getMirroredPlot()-----------------------------
	----------------- Copyright 2010  (c)  Leszek Deska --------------------------
	------------------------------------------------------------------------------
	-- mirrorize plot types, terrain, resource, natural wonders, ruins (ruins doesn't work)
	local iW, iH = Map.GetGridSize()
	print("iW/iH=",iW, iH)
	for x = 0, iW * 0.5 do
		for y = 0, iH - 1 do
			if( iW-x-y%2 ~= x ) then
				print("x/y=",x,y);
				local plot = Map.GetPlot(x, y);
				local mirrorPlot = getMirroredPlot(plot);
				local plotType = plot:GetPlotType();
				local terrainType = plot:GetTerrainType();
				local featureType = plot:GetFeatureType();
				local improvementType = plot:GetImprovementType();
				local resourceType = plot:GetResourceType(-1)
				mirrorPlot:SetPlotType(plotType,false,false)
				mirrorPlot:SetTerrainType(terrainType,false,false)
				mirrorPlot:SetFeatureType(featureType)
				mirrorPlot:SetResourceType(resourceType,plot:GetNumResource())
				mirrorPlot:SetImprovementType(improvementType)
			end
		end
	end
	-- rivers
	-- mirrorize rivers
	--rivers
	for x = math.floor(iW / 2)+1, iW - 1 do
		for y = 0, iH - 1 do
			local plot = Map.GetPlot(x, y)
			plot:SetWOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
			plot:SetNWOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
			plot:SetNEOfRiver(false,FlowDirectionTypes.NO_FLOWDIRECTION)
		end
	end
	for y = 0, iH - 1 do
		for x = 0, iW * 0.5 do
		    if ( x < (iH/2 + 1) ) then
            	local plot = Map.GetPlot(x, y);
				if ( plot:IsWOfRiver() ) then
					local mirrorPlot = getMirroredPlot(plot);
					mirrorPlot = Map.PlotDirection(mirrorPlot:GetX(), mirrorPlot:GetY(), DirectionTypes.DIRECTION_WEST);
					local dir = FlowDirectionTypes.FLOWDIRECTION_NORTH;
					if( plot:GetRiverEFlowDirection() == FlowDirectionTypes.FLOWDIRECTION_NORTH ) then
						dir = FlowDirectionTypes.FLOWDIRECTION_SOUTH;
					end
					mirrorPlot:SetWOfRiver(true, dir);
				end
				if ( plot:IsNWOfRiver() ) then
					local mirrorPlot = getMirroredPlot(plot);
					mirrorPlot = Map.PlotDirection(mirrorPlot:GetX(), mirrorPlot:GetY(), DirectionTypes.DIRECTION_NORTHWEST);
					local dir = FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST;
					if( plot:GetRiverSEFlowDirection() == FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST) then
						dir = FlowDirectionTypes.FLOWDIRECTION_NORTHEAST;
					end;
					mirrorPlot:SetNWOfRiver(true, dir);
				end
				if ( plot:IsNEOfRiver() ) then
					local mirrorPlot = getMirroredPlot(plot);
					mirrorPlot = Map.PlotDirection(mirrorPlot:GetX(), mirrorPlot:GetY(), DirectionTypes.DIRECTION_NORTHEAST);
					local dir = FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST;
					if( plot:GetRiverSWFlowDirection() == FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST) then
						dir = FlowDirectionTypes.FLOWDIRECTION_NORTHWEST;
					end;
					mirrorPlot:SetNEOfRiver(true, dir);
				end
			end
		end
	end
	-- mirrorize starting positions
	local playerStartPlot;
	local searchMode = 0;
	for i = 0, GameDefines.MAX_MAJOR_CIVS + GameDefines.MAX_MINOR_CIVS - 1 do
		local player = Players[i];
		if (isValidPlayer(player)) then
			if player:IsEverAlive() then
				if( searchMode == 0 ) then
					searchMode = 1;
					player = Players[i];
					playerStartPlot = player:GetStartingPlot();
				else
					searchMode = 0;
					player = Players[i];
					player:SetStartingPlot(getMirroredPlot(playerStartPlot));
				end
			end
		end
	end
		Map:RecalculateAreas();
		Game.SetOption(1, true);
	
	end
end
------------------------------------------------------------------------------

