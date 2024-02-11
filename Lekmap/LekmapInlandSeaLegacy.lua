------------------------------------------------------------------------------
--	FILE:	 Lekmapv2.2.lua (Modified Pangaea_Plus.lua)
--	AUTHOR:  Original Bob Thomas, Changes HellBlazer, lek10, EnormousApplePie, Cirra, Meota
--	PURPOSE: Global map script - Simulates a Pan-Earth Supercontinent, with
--           numerous tectonic island chains.
------------------------------------------------------------------------------
--	Copyright (c) 2011 Firaxis Games, Inc. All rights reserved.
------------------------------------------------------------------------------

include("HBMapGenerator");
include("HBFractalWorld");
include("HBFeatureGenerator");
include("HBTerrainGenerator");
include("IslandMaker");
include("MultilayeredFractal");

------------------------------------------------------------------------------
function GetMapScriptInfo()
	local world_age, temperature, rainfall, sea_level, resources = GetCoreMapOptions()
	return {
		Name = "Lekmap: Inland Sea (Bonus Map)",
		Description = "A map script made for Lekmod based of HB's Mapscript v8.1. Inland Sea",
		IsAdvancedMap = false,
		IconIndex = 12,
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
				Name = "Land Size X",	-- add setting for land type (11) +22
				Values = {
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
					"78",
					"80",
					"82",
					"84",
					"86",
					"88",
					"90",
					"92",
					"94",
					"96",
					"98",
					"100",
					"102",
					"104",
				},

				DefaultValue = 9,
				SortPriority = -89,
			},

			{
				Name = "Land Size Y",	-- add setting for land type (12) +14
				Values = {
					"16",
					"18",
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
				},

				DefaultValue = 5,
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
				Name = "Coastal Spawns",	-- Can inland civ spawn on the coast (15)
				Values = {
					"Coastal Civs Only",
					"Random",
					"Random+ (~2 coastals)",
				},

				DefaultValue = 1,
				SortPriority = -85,
			},

			{
				Name = "Coastal Luxes",	-- Can coast spawns have non-coastal luxes (16)
				Values = {
					"Guaranteed",
					"Random",
				},

				DefaultValue = 1,
				SortPriority = -84,
			},
		},
	};
end
------------------------------------------------------------------------------
function GetMapInitData(worldSize)
	
	local LandSizeX = 22 + (Map.GetCustomOption(11) * 2);
	local LandSizeY = 14 + (Map.GetCustomOption(12) * 2);

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
------------------------------------------------------------------------------

-------------------------------------------------------------------------------
function MultilayeredFractal:GeneratePlotsByRegion()
	-- Sirian's MultilayeredFractal controlling function.
	-- You -MUST- customize this function for each script using MultilayeredFractal.
	--
	-- This implementation is specific to Inland Sea.
	local iW, iH = Map.GetGridSize();

	-- Fill all rows with land plots.
	self.wholeworldPlotTypes = table.fill(PlotTypes.PLOT_LAND, iW * iH);

	-- Generate the inland sea.
	local iWestX = math.floor(iW * 0.18) - 1;
	local iEastX = math.ceil(iW * 0.82) - 1;
	local iWidth = iEastX - iWestX;
	local iSouthY = math.floor(iH * 0.28) - 1;
	local iNorthY = math.ceil(iH * 0.72) - 1;
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

	-- Second, oval layer to ensure one main body of water.
	local centerX = (iW / 2) - 1;
	local centerY = (iH / 2) - 1;
	local xAxis = centerX / 2;
	local yAxis = centerY * 0.35;
	local xAxisSquared = xAxis * xAxis;
	local yAxisSquared = yAxis * yAxis;
	for x = 0, iW - 1 do
		for y = 0, iH - 1 do
			local i = y * iW + x + 1;
			local deltaX = x - centerX;
			local deltaY = y - centerY;
			local deltaXSquared = deltaX * deltaX;
			local deltaYSquared = deltaY * deltaY;
			local oval_value = deltaXSquared / xAxisSquared + deltaYSquared / yAxisSquared;
			if oval_value <= 1 then
				self.wholeworldPlotTypes[i] = PlotTypes.PLOT_OCEAN;
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

	-- Plot Type generation completed. Return global plot array.
	return self.wholeworldPlotTypes
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function GeneratePlotTypes()
	print("Setting Plot Types (Lua Inland Sea) ...");

	local layered_world = MultilayeredFractal.Create();
	local plotsIS = layered_world:GeneratePlotsByRegion();
	
	SetPlotTypes(plotsIS);

	GenerateCoasts();
end
----------------------------------------------------------------------------------
function TerrainGenerator:GetLatitudeAtPlot(iX, iY)
	local lat = math.abs((self.iHeight / 2) - iY) / (self.iHeight / 2);
	lat = lat + (128 - self.variation:GetHeight(iX, iY))/(255.0 * 5.0);
	lat = math.clamp(lat, 0, 1);

	-- For Inland Sea only, adjust latitude to cut out Tundra and most Jungle.
	local adjusted_lat = 0.07 + 0.52 * lat;
	
	return adjusted_lat;
end
------------------------------------------------------------------------------
function GenerateTerrain()
	print("Generating Terrain (Lua Inland Sea) ...");
	
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
	-- Custom method to force rivers to flow toward the map center.
	local iW, iH = Map.GetGridSize()
	local x = plot:GetX()
	local y = plot:GetY()
	local random_factor = Map.Rand(3, "River direction random factor - Inland Sea LUA");
	local direction_influence_value = (math.abs(x - (iW / 2)) + math.abs(y - (iH / 2))) * random_factor;

	local numPlots = PlotTypes.NUM_PLOT_TYPES;
	local sum = ((numPlots - plot:GetPlotType()) * 20) + direction_influence_value;

	local numDirections = DirectionTypes.NUM_DIRECTION_TYPES;
	for direction = 0, numDirections - 1, 1 do
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
function AddRivers()
	-- Customization for Inland Sea, to keep river starts away from map edges and set river "original flow direction".
	local iW, iH = Map.GetGridSize()
	print("Inland Sea - Adding Rivers");
	local passConditions = {
		function(plot)
			return plot:IsHills() or plot:IsMountain();
		end,
		
		function(plot)
			return (not plot:IsCoastalLand()) and (Map.Rand(8, "MapGenerator AddRivers") == 0);
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
			elseif(not plot:IsWater()) then
				if(passCondition(plot)) then
					if (not Map.FindWater(plot, riverSourceRange, true)) then
						if (not Map.FindWater(plot, seaWaterRange, false)) then
							local inlandCorner = plot:GetInlandCorner();
							if(inlandCorner) then
								local start_x = inlandCorner:GetX()
								local start_y = inlandCorner:GetY()
								local orig_direction;
								if start_y < iH / 2 then -- South half of map
									if start_x < iW / 3 then -- SW Corner
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_NORTHEAST;
									elseif start_x > iW * 0.66 then -- SE Corner
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_NORTHWEST;
									else -- South, middle
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_NORTH;
									end
								else -- North half of map
									if start_x < iW / 3 then -- NW corner
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_SOUTHEAST;
									elseif start_x > iW * 0.66 then -- NE corner
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_SOUTHWEST;
									else -- North, middle
										orig_direction = FlowDirectionTypes.FLOWDIRECTION_SOUTH;
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
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function FeatureGenerator:GetLatitudeAtPlot(iX, iY)
	local lat = math.abs((self.iGridH/2) - iY)/(self.iGridH/2);

	-- For Inland Sea only, adjust latitude to cut out Tundra and most Jungle.
	local adjusted_lat = 0.07 + 0.52 * lat;
	
	return adjusted_lat
end
------------------------------------------------------------------------------
function FeatureGenerator:AddIceAtPlot(plot, iX, iY, lat)
	return
end
------------------------------------------------------------------------------
function AddFeatures()
	print("Adding Features (Lua Inland Sea) ...");

	-- Get Rainfall setting input by user.
	local rain = Map.GetCustomOption(3)
	if rain == 4 then
		rain = 1 + Map.Rand(3, "Random Rainfall - Lua");
	end
	
	local args = {rainfall = rain}
	local featuregen = FeatureGenerator.Create(args);

	-- False parameter removes mountains from coastlines.
	featuregen:AddFeatures(false);
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
function AssignStartingPlots:CanPlaceCityStateAt(x, y, area_ID, force_it, ignore_collisions)
	-- Overriding default city state placement to prevent city states from being placed too close to map edges.
	local iW, iH = Map.GetGridSize();
	local plot = Map.GetPlot(x, y)
	local area = plot:GetArea()
	
	-- Adding this check for Inland Sea
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
		print("-"); print("City State candidate plot rejected: collided with already-placed civ or City State at", x, y);
		return false
	end
	return true
end

------------------------------------------------------------------------------
function StartPlotSystem()

	local RegionalMethod = 1;

	-- Get Resources setting input by user.
	local res = Map.GetCustomOption(13)
	local starts = Map.GetCustomOption(5)
	--if starts == 7 then
		--starts = 1 + Map.Rand(8, "Random Resources Option - Lua");
	--end

	-- Handle coastal spawns and start bias
	MixedBias = false;
	if Map.GetCustomOption(15) == 1 then
		OnlyCoastal = true;
		BalancedCoastal = false;
	end	
	if Map.GetCustomOption(15) == 2 then
		BalancedCoastal = false;
		OnlyCoastal = false;
	end
	
	if Map.GetCustomOption(15) == 3 then
		OnlyCoastal = true;
		BalancedCoastal = true;
	end
	
	if Map.GetCustomOption(16) == 1 then
	CoastLux = true
	end

	if Map.GetCustomOption(16) == 2 then
	CoastLux = false
	end

	print("Creating start plot database.");
	local start_plot_database = AssignStartingPlots.Create()
	
	print("Dividing the map in to Regions.");
	-- Regional Division Method 1: Biggest Landmass
	local args = {
		method = RegionalMethod,
		start_locations = starts,
		resources = res,
		CoastLux = CoastLux,
		NoCoastInland = OnlyCoastal,
		BalancedCoastal = BalancedCoastal,
		MixedBias = MixedBias;
		};
	start_plot_database:GenerateRegions(args)

	print("Choosing start locations for civilizations.");
	start_plot_database:ChooseLocations()
	
	print("Normalizing start locations and assigning them to Players.");
	start_plot_database:BalanceAndAssign(args)

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
	start_plot_database:PlaceNaturalWonders(wonderargs);
	print("Placing Resources and City States.");
	start_plot_database:PlaceResourcesAndCityStates()
end
------------------------------------------------------------------------------