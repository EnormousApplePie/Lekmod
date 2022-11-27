------------------------------------------------------------------------------
--	FILE:          FractalWorld.lua
--	AUTHOR:        Bob Thomas
--	PYTHON TO LUA: Shaun Seckman
--	CONTRIB:       Brian Wade
--	PURPOSE:       Default method for plot generation
------------------------------------------------------------------------------
--	Copyright (c) 2009, 2010 Firaxis Games, Inc. All rights reserved.
------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------
FractalWorld = {};
-------------------------------------------------------------------------------------------
function FractalWorld.Create(fracXExp, fracYExp)
	
	local gridWidth, gridHeight = Map.GetGridSize();
	
	local data = {
		InitFractal = FractalWorld.InitFractal,
		ShiftPlotTypes = FractalWorld.ShiftPlotTypes,
		ShiftPlotTypesBy = FractalWorld.ShiftPlotTypesBy,
		DetermineXShift = FractalWorld.DetermineXShift,
		DetermineYShift = FractalWorld.DetermineYShift,
		GenerateCenterRift = FractalWorld.GenerateCenterRift,
		GeneratePlotTypes = FractalWorld.GeneratePlotTypes,
		
		iFlags = Map.GetFractalFlags(),
		
		fracXExp = fracXExp,
		fracYExp = fracYExp,
		
		iNumPlotsX = gridWidth,
		iNumPlotsY = gridHeight,
		plotTypes = table.fill(PlotTypes.PLOT_OCEAN, gridWidth * gridHeight)
	};
		
	return data;
end	
-------------------------------------------------------------------------------------------
function FractalWorld:InitFractal(args)
	if(args == nil) then args = {}; end
	
	print("=============== USING NQ PANGAEA FRACTAL ===============");

	--local continent_grain = args.continent_grain or 2;
	local continent_grain = 1;
	--local rift_grain = args.rift_grain or -1; -- Default no rifts. Set grain to between 1 and 3 to add rifts. - Bob
	local rift_grain = 1;
	--local invert_heights = args.invert_heights or false;
	local invert_heights = false;
	--local polar = args.polar or true;
	local polar = true;
	local ridge_flags = args.ridge_flags or self.iFlags;
	
	local fracFlags = {};
	
	if(invert_heights) then
		fracFlags.FRAC_INVERT_HEIGHTS = true;
	end
	
	if(polar) then
		fracFlags.FRAC_POLAR = true;
	end
	
	if(rift_grain > 0 and rift_grain < 4) then
		self.riftsFrac = Fractal.Create((self.iNumPlotsX), self.iNumPlotsY, rift_grain, {}, self.fracXExp, self.fracYExp);
		self.continentsFrac = Fractal.CreateRifts((self.iNumPlotsX * 1.1), self.iNumPlotsY, continent_grain, fracFlags, self.riftsFrac, self.fracXExp, self.fracYExp);
	else
		self.continentsFrac = Fractal.Create(self.iNumPlotsX, self.iNumPlotsY, continent_grain, fracFlags, self.fracXExp, self.fracYExp);	
	end

	-- Use Brian's tectonics method to weave ridgelines in to the continental fractal.
	-- Without fractal variation, the tectonics come out too regular.
	--
	--[[ "The principle of the RidgeBuilder code is a modified Voronoi diagram. I 
	added some minor randomness and the slope might be a little tricky. It was 
	intended as a 'whole world' modifier to the fractal class. You can modify 
	the number of plates, but that is about it." ]]-- Brian Wade - May 23, 2009
	--
	local WorldSizeTypes = {};
	for row in GameInfo.Worlds() do
		WorldSizeTypes[row.Type] = row.ID;
	end
	local sizekey = Map.GetWorldSize();
	local sizevalues = {
		[WorldSizeTypes.WORLDSIZE_DUEL]     = 4,
		[WorldSizeTypes.WORLDSIZE_TINY]     = 8,
		[WorldSizeTypes.WORLDSIZE_SMALL]    = 8,
		[WorldSizeTypes.WORLDSIZE_STANDARD] = 20,
		[WorldSizeTypes.WORLDSIZE_LARGE]    = 24,
		[WorldSizeTypes.WORLDSIZE_HUGE]		= 32
	}
	--
	local numPlates = sizevalues[sizekey] or 4
	-- Blend a bit of ridge into the fractal.
	-- This will do things like roughen the coastlines and build inland seas. - Brian
	self.continentsFrac:BuildRidges(numPlates, ridge_flags, 1, 4);
end
-------------------------------------------------------------------------------------------
function FractalWorld:ShiftPlotTypes()
	local stripRadius = self.stripRadius;
	local shift_x = 0; 
	local shift_y = 0;

	shift_x = self:DetermineXShift();	
	shift_y = self:DetermineYShift();	
	
	self:ShiftPlotTypesBy(shift_x, shift_y);
end
-------------------------------------------------------------------------------------------	
function FractalWorld:ShiftPlotTypesBy(xshift, yshift)
	if(xshift > 0 or yshift > 0) then
		local iWH = self.iNumPlotsX * self.iNumPlotsY
		local buf = {};
		for i = 1, iWH + 1 do
			buf[i] = self.plotTypes[i];
		end
		
		for iDestY = 0, self.iNumPlotsY do
			for iDestX = 0, self.iNumPlotsX do
				local iDestI = self.iNumPlotsX * iDestY + iDestX;
				local iSourceX = (iDestX + xshift) % self.iNumPlotsX;
				local iSourceY = (iDestY + yshift) % self.iNumPlotsY;
				
				local iSourceI = self.iNumPlotsX * iSourceY + iSourceX
				self.plotTypes[iDestI] = buf[iSourceI]
			end
		end
	end
end
-------------------------------------------------------------------------------------------
function FractalWorld:DetermineXShift()
	--[[ This function will align the most water-heavy vertical portion of the map with the 
	vertical map edge. This is a form of centering the landmasses, but it emphasizes the
	edge not the middle. If there are columns completely empty of land, these will tend to
	be chosen as the new map edge, but it is possible for a narrow column between two large 
	continents to be passed over in favor of the thinnest section of a continent, because
	the operation looks at a group of columns not just a single column, then picks the 
	center of the most water heavy group of columns to be the new vertical map edge. ]]--

	-- First loop through the map columns and record land plots in each column.
	local land_totals = {};
	for x = 0, self.iNumPlotsX - 1 do
		local current_column = 0;
		for y = 0, self.iNumPlotsY - 1 do
			local i = y * self.iNumPlotsX + x + 1;
			if (self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN) then
				current_column = current_column + 1;
			end
		end
		table.insert(land_totals, current_column);
	end
	
	-- Now evaluate column groups, each record applying to the center column of the group.
	local column_groups = {};
	-- Determine the group size in relation to map width.
	local group_radius = math.floor(self.iNumPlotsX / 10);
	-- Measure the groups.
	for column_index = 1, self.iNumPlotsX do
		local current_group_total = 0;
		for current_column = column_index - group_radius, column_index + group_radius do
			local current_index = current_column % self.iNumPlotsX;
			if current_index == 0 then -- Modulo of the last column will be zero; this repairs the issue.
				current_index = self.iNumPlotsX;
			end
			current_group_total = current_group_total + land_totals[current_index];
		end
		table.insert(column_groups, current_group_total);
	end
	
	-- Identify the group with the least amount of land in it.
	local best_value = self.iNumPlotsY * (2 * group_radius + 1); -- Set initial value to max possible.
	local best_group = 1; -- Set initial best group as current map edge.
	for column_index, group_land_plots in ipairs(column_groups) do
		if group_land_plots < best_value then
			best_value = group_land_plots;
			best_group = column_index;
		end
	end
	
	-- Determine X Shift
	local x_shift = best_group - 1;
	return x_shift;
end
-------------------------------------------------------------------------------------------
function FractalWorld:DetermineYShift()
	-- Counterpart to DetermineXShift()

	-- First loop through the map rows and record land plots in each row.
	local land_totals = {};
	for y = 0, self.iNumPlotsY - 1 do
		local current_row = 0;
		for x = 0, self.iNumPlotsX - 1 do
			local i = y * self.iNumPlotsX + x + 1;
			if (self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN) then
				current_row = current_row + 1;
			end
		end
		table.insert(land_totals, current_row);
	end
	
	-- Now evaluate row groups, each record applying to the center row of the group.
	local row_groups = {};
	-- Determine the group size in relation to map height.
	local group_radius = math.floor(self.iNumPlotsY / 15);
	-- Measure the groups.
	for row_index = 1, self.iNumPlotsY do
		local current_group_total = 0;
		for current_row = row_index - group_radius, row_index + group_radius do
			local current_index = current_row % self.iNumPlotsY;
			if current_index == 0 then -- Modulo of the last row will be zero; this repairs the issue.
				current_index = self.iNumPlotsY;
			end
			current_group_total = current_group_total + land_totals[current_index];
		end
		table.insert(row_groups, current_group_total);
	end
	
	-- Identify the group with the least amount of land in it.
	local best_value = self.iNumPlotsX * (2 * group_radius + 1); -- Set initial value to max possible.
	local best_group = 1; -- Set initial best group as current map edge.
	for row_index, group_land_plots in ipairs(row_groups) do
		if group_land_plots < best_value then
			best_value = group_land_plots;
			best_group = row_index;
		end
	end
	
	-- Determine Y Shift
	local y_shift = best_group - 1;
	return y_shift;
end
-------------------------------------------------------------------------------------------
function FractalWorld:GenerateCenterRift()
	-- Causes a rift to break apart and separate any landmasses overlaying the map center.
	-- Rift runs south to north ala the Atlantic Ocean.
	-- Any land plots in the first or last map columns will be lost, overwritten.
	-- This rift function is hex-dependent. It would have to be adapted to work with squares tiles.
	-- Center rift not recommended for non-oceanic worlds or with continent grains higher than 2.
	-- 
	-- First determine the rift "lean". 0 = Starts west, leans east. 1 = Starts east, leans west.
	local riftLean = Map.Rand(2, "FractalWorld Center Rift Lean - Lua");
	
	-- Set up tables recording the rift line and the edge plots to each side of the rift line.
	local riftLine = {};
	local westOfRift = {};
	local eastOfRift = {};
	-- Determine minimum and maximum length of line segments for each possible direction.
	local primaryMaxLength = math.max(1, math.floor(self.iNumPlotsY / 8));
	local secondaryMaxLength = math.max(1, math.floor(self.iNumPlotsY / 11));
	local tertiaryMaxLength = math.max(1, math.floor(self.iNumPlotsY / 14));
	
	-- Set rift line starting plot and direction.
	local startDistanceFromCenterColumn = math.floor(self.iNumPlotsY / 8);
	if riftLean == 0 then
		startDistanceFromCenterColumn = -(startDistanceFromCenterColumn);
	end
	local startX = math.floor(self.iNumPlotsX / 2) + startDistanceFromCenterColumn;
	local startY = 0;
	local startingDirection = DirectionTypes.DIRECTION_NORTHWEST;
	if riftLean == 0 then
		startingDirection = DirectionTypes.DIRECTION_NORTHEAST;
	end
	-- Set rift X boundary.
	local riftXBoundary = math.floor(self.iNumPlotsX / 2) - startDistanceFromCenterColumn;
	
	-- Rift line is defined by a series of line segments traveling in one of three directions.
	-- East-leaning lines move NE primarily, NW secondarily, and E tertiary.
	-- West-leaning lines move NW primarily, NE secondarily, and W tertiary.
	-- Any E or W segments cause a wider gap on that row, requiring independent storage of data regarding west or east of rift.
	--
	-- Key variables need to be defined here so they persist outside of the various loops that follow.
	-- This requires that the starting plot be processed outside of those loops.
	local currentDirection = startingDirection;
	local currentX = startX;
	local currentY = startY;
	table.insert(riftLine, {currentX, currentY});
	-- Record west and east of the rift for this row.
	local rowIndex = currentY + 1;
	westOfRift[rowIndex] = currentX - 1;
	eastOfRift[rowIndex] = currentX + 1;
	-- Set this rift plot as type Ocean.
	local plotIndex = currentX + 1; -- Lua arrays starting at 1 sure makes for a lot of extra work and chances for bugs.
	self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN; -- Tiles crossed by the rift all turn in to water.
	
	-- Generate the rift line.
	if riftLean == 0 then -- Leans east
		while currentY < self.iNumPlotsY - 1 do
			-- Generate a line segment
			local nextDirection = 0;

			if currentDirection == DirectionTypes.DIRECTION_EAST then
				local segmentLength = Map.Rand(tertiaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX >= riftXBoundary then -- Gone as far east as allowed, must turn back west.
					nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
				else
					local dice = Map.Rand(3, "FractalWorld Center Rift Direction - Lua");
					if dice == 1 then
						nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 do
					currentX = currentX + 1; -- Moving east, no change to Y.
					rowIndex = currentY;
					-- westOfRift[rowIndex] does not change.
					eastOfRift[rowIndex] = currentX + 1;
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end

			elseif currentDirection == DirectionTypes.DIRECTION_NORTHWEST then
				local segmentLength = Map.Rand(secondaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX >= riftXBoundary then -- Gone as far east as allowed, must turn back west.
					nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
				else
					local dice = Map.Rand(4, "FractalWorld Center Rift Direction - Lua");
					if dice == 2 then
						nextDirection = DirectionTypes.DIRECTION_EAST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 and currentY < self.iNumPlotsY - 1 do
					-- Identifying hex plots other than to east or west is tricky.
					-- The X coord could be one of two possibilities.
					-- Call on Map.PlotDirection to safely handle this task.
					local nextPlot = Map.PlotDirection(currentX, currentY, currentDirection);
					currentX = nextPlot:GetX();
					currentY = currentY + 1;
					rowIndex = currentY;
					westOfRift[rowIndex] = currentX - 1;
					eastOfRift[rowIndex] = currentX + 1;
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end
				
			else -- NORTHEAST
				local segmentLength = Map.Rand(primaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX >= riftXBoundary then -- Gone as far east as allowed, must turn back west.
					nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
				else
					local dice = Map.Rand(2, "FractalWorld Center Rift Direction - Lua");
					if dice == 1 and currentY > self.iNumPlotsY * 0.28 then
						nextDirection = DirectionTypes.DIRECTION_EAST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 and currentY < self.iNumPlotsY - 1 do
					-- Identifying hex plots other than to east or west is tricky.
					-- The X coord could be one of two possibilities.
					-- Call on Map.PlotDirection to safely handle this task.
					local nextPlot = Map.PlotDirection(currentX, currentY, currentDirection);
					currentX = nextPlot:GetX();
					currentY = currentY + 1;
					rowIndex = currentY;
					westOfRift[rowIndex] = currentX - 1;
					eastOfRift[rowIndex] = currentX + 1;
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end
			end
			
			-- Line segment is done, set next direction.
			currentDirection = nextDirection;
		end

	else -- Leans west
		while currentY < self.iNumPlotsY - 1 do
			-- Generate a line segment
			local nextDirection = 0;

			if currentDirection == DirectionTypes.DIRECTION_WEST then
				local segmentLength = Map.Rand(tertiaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX <= riftXBoundary then -- Gone as far west as allowed, must turn back east.
					nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
				else
					local dice = Map.Rand(3, "FractalWorld Center Rift Direction - Lua");
					if dice == 1 then
						nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 do
					currentX = currentX - 1; -- Moving west, no change to Y.
					rowIndex = currentY;
					westOfRift[rowIndex] = currentX - 1;
					-- eastOfRift[rowIndex] does not change.
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end

			elseif currentDirection == DirectionTypes.DIRECTION_NORTHEAST then
				local segmentLength = Map.Rand(secondaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX <= riftXBoundary then -- Gone as far west as allowed, must turn back east.
					nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
				else
					local dice = Map.Rand(4, "FractalWorld Center Rift Direction - Lua");
					if dice == 2 then
						nextDirection = DirectionTypes.DIRECTION_WEST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHWEST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 and currentY < self.iNumPlotsY - 1 do
					-- Identifying hex plots other than to east or west is tricky.
					-- The X coord could be one of two possibilities.
					-- Call on Map.PlotDirection to safely handle this task.
					local nextPlot = Map.PlotDirection(currentX, currentY, currentDirection);
					currentX = nextPlot:GetX();
					currentY = currentY + 1;
					rowIndex = currentY;
					westOfRift[rowIndex] = currentX - 1;
					eastOfRift[rowIndex] = currentX + 1;
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end
				
			else -- NORTHWEST
				local segmentLength = Map.Rand(primaryMaxLength + 1, "FractalWorld Center Rift Segment Length - Lua");
				-- Choose next direction
				if currentX <= riftXBoundary then -- Gone as far west as allowed, must turn back east.
					nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
				else
					local dice = Map.Rand(2, "FractalWorld Center Rift Direction - Lua");
					if dice == 1 and currentY > self.iNumPlotsY * 0.28 then
						nextDirection = DirectionTypes.DIRECTION_WEST;
					else
						nextDirection = DirectionTypes.DIRECTION_NORTHEAST;
					end
				end
				-- Process the line segment
				local plotsToDo = segmentLength;
				while plotsToDo > 0 and currentY < self.iNumPlotsY - 1 do
					-- Identifying hex plots other than to east or west is tricky.
					-- The X coord could be one of two possibilities.
					-- Call on Map.PlotDirection to safely handle this task.
					local nextPlot = Map.PlotDirection(currentX, currentY, currentDirection);
					currentX = nextPlot:GetX();
					currentY = currentY + 1;
					rowIndex = currentY;
					westOfRift[rowIndex] = currentX - 1;
					eastOfRift[rowIndex] = currentX + 1;
					plotIndex = currentY * self.iNumPlotsX + currentX + 1;
					self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
					plotsToDo = plotsToDo - 1;
				end
			end
			
			-- Line segment is done, set next direction.
			currentDirection = nextDirection;
		end
	end
	-- Process the final plot in the rift.
	westOfRift[self.iNumPlotsY] = currentX - 1;
	eastOfRift[self.iNumPlotsY] = currentX + 1;
	plotIndex = (self.iNumPlotsY - 1) * self.iNumPlotsX + currentX + 1;
	self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;

	-- Now force the rift to widen, causing land on either side of the rift to drift apart.
	local horizontalDrift = 3;
	local verticalDrift = 2;
	--
	if riftLean == 0 then
		-- Process Western side from top down.
		for y = self.iNumPlotsY - 1 - verticalDrift, 0, -1 do
			local thisRowX = westOfRift[y+1];
			for x = horizontalDrift, thisRowX do
				local sourcePlotIndex = y * self.iNumPlotsX + x + 1;
				local destPlotIndex = (y + verticalDrift) * self.iNumPlotsX + (x - horizontalDrift) + 1;
				self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
			end
		end
		-- Process Eastern side from bottom up.
		for y = verticalDrift, self.iNumPlotsY - 1 do
			local thisRowX = eastOfRift[y+1];
			for x = thisRowX, self.iNumPlotsX - horizontalDrift - 1 do
				local sourcePlotIndex = y * self.iNumPlotsX + x + 1;
				local destPlotIndex = (y - verticalDrift) * self.iNumPlotsX + (x + horizontalDrift) + 1;
				self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
			end
		end
		-- Clean up remainder of tiles (by turning them all to Ocean).
		-- Clean up bottom left.
		for y = 0, verticalDrift - 1 do
			local thisRowX = westOfRift[y+1];
			for x = 0, thisRowX do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		-- Clean up top right.
		for y = self.iNumPlotsY - verticalDrift, self.iNumPlotsY - 1 do
			local thisRowX = eastOfRift[y+1];
			for x = thisRowX, self.iNumPlotsX - 1 do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		-- Clean up the rift.
		for y = verticalDrift, self.iNumPlotsY - 1 - verticalDrift do
			local westX = westOfRift[y-verticalDrift+1] - horizontalDrift + 1;
			local eastX = eastOfRift[y+verticalDrift+1] + horizontalDrift - 1;
			for x = westX, eastX do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end

	else -- riftLean = 1
		-- Process Western side from bottom up.
		for y = verticalDrift, self.iNumPlotsY - 1 do
			local thisRowX = westOfRift[y+1];
			for x = horizontalDrift, thisRowX do
				local sourcePlotIndex = y * self.iNumPlotsX + x + 1;
				local destPlotIndex = (y - verticalDrift) * self.iNumPlotsX + (x - horizontalDrift) + 1;
				self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
			end
		end
		-- Process Eastern side from top down.
		for y = self.iNumPlotsY - 1 - verticalDrift, 0, -1 do
			local thisRowX = eastOfRift[y+1];
			for x = thisRowX, self.iNumPlotsX - horizontalDrift - 1 do
				local sourcePlotIndex = y * self.iNumPlotsX + x + 1;
				local destPlotIndex = (y + verticalDrift) * self.iNumPlotsX + (x + horizontalDrift) + 1;
				self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
			end
		end
		-- Clean up remainder of tiles (by turning them all to Ocean).
		-- Clean up top left.
		for y = self.iNumPlotsY - verticalDrift, self.iNumPlotsY - 1 do
			local thisRowX = westOfRift[y+1];
			for x = 0, thisRowX do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		-- Clean up bottom right.
		for y = 0, verticalDrift - 1 do
			local thisRowX = eastOfRift[y+1];
			for x = thisRowX, self.iNumPlotsX - 1 do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
		-- Clean up the rift.
		for y = verticalDrift, self.iNumPlotsY - 1 - verticalDrift do
			local westX = westOfRift[y+verticalDrift+1] - horizontalDrift + 1;
			local eastX = eastOfRift[y-verticalDrift+1] + horizontalDrift - 1;
			for x = westX, eastX do
				local plotIndex = y * self.iNumPlotsX + x + 1;
				self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN;
			end
		end
	end
end
-------------------------------------------------------------------------------------------
function FractalWorld:GeneratePlotTypes(args)
	--[[ Civ4's truncated "Climate" setting has been abandoned. Civ5 has returned to 
	Civ3-style map options for World Age, Temperature, and Rainfall. Control over the 
	terrain has been removed from the XML.  - Bob Thomas, March 2010  ]]--
	--
	-- Sea Level and World Age map options affect only plot generation.
	-- Temperature map options affect only terrain generation.
	-- Rainfall map options affect only feature generation.
	--
	local args = args or {};
	local sea_level = args.sea_level or 2; -- Default is Medium sea level.
	local world_age = args.world_age or 2; -- Default is 4 Billion Years old.
	-- Note: World Age and Sea Level settings, if applicable, must be passed in by the map script.
	--
	local sea_level_low = args.sea_level_low or 65;
	local sea_level_normal = args.sea_level_normal or 72;
	local sea_level_high = args.sea_level_high or 78;
	local world_age_old = args.world_age_old or 2;
	local world_age_normal = args.world_age_normal or 3;
	local world_age_new = args.world_age_new or 5;
	--
	local extra_mountains = args.extra_mountains or 0;
	local grain_amount = args.grain_amount or 3;
	local adjust_plates = args.adjust_plates or 1.0;
	local shift_plot_types = args.shift_plot_types or true;
	local tectonic_islands = args.tectonic_islands or false;
	local hills_ridge_flags = args.hills_ridge_flags or self.iFlags;
	local peaks_ridge_flags = args.peaks_ridge_flags or self.iFlags;
	local has_center_rift = args.has_center_rift or false;
	
	-- Set Sea Level according to user selection. - Bob
	local water_percent = sea_level_normal;
	if sea_level == 1 then -- Low Sea Level
		water_percent = sea_level_low
	elseif sea_level == 3 then -- High Sea Level
		water_percent = sea_level_high
	else -- Normal Sea Level
	end

	-- Set values for hills and mountains according to World Age chosen by user. - Bob
	local adjustment = world_age_normal;
	if world_age == 3 then -- 5 Billion Years
		adjustment = world_age_old;
		adjust_plates = adjust_plates * 0.75;
	elseif world_age == 1 then -- 3 Billion Years
		adjustment = world_age_new;
		adjust_plates = adjust_plates * 1.5;
	else -- 4 Billion Years
	end
	-- Apply adjustment to hills and peaks settings.
	local hillsBottom1 = 28 - adjustment;
	local hillsTop1 = 28 + adjustment;
	local hillsBottom2 = 72 - adjustment;
	local hillsTop2 = 72 + adjustment;
	local hillsClumps = 1 + adjustment;
	local hillsNearMountains = 90 - (adjustment * 2) - extra_mountains;
	local mountains = 100 - adjustment - extra_mountains;

	-- Hills and Mountains handled differently according to map size - Bob
	local WorldSizeTypes = {};
	for row in GameInfo.Worlds() do
		WorldSizeTypes[row.Type] = row.ID;
	end
	local sizekey = Map.GetWorldSize();
	-- Fractal Grains
	local sizevalues = {
		[WorldSizeTypes.WORLDSIZE_DUEL]     = 3,
		[WorldSizeTypes.WORLDSIZE_TINY]     = 3,
		[WorldSizeTypes.WORLDSIZE_SMALL]    = 4,
		[WorldSizeTypes.WORLDSIZE_STANDARD] = 4,
		[WorldSizeTypes.WORLDSIZE_LARGE]    = 5,
		[WorldSizeTypes.WORLDSIZE_HUGE]		= 5
	};
	local grain = sizevalues[sizekey] or 3;
	-- Tectonics Plate Counts
	local platevalues = {
		[WorldSizeTypes.WORLDSIZE_DUEL]		= 6,
		[WorldSizeTypes.WORLDSIZE_TINY]     = 9,
		[WorldSizeTypes.WORLDSIZE_SMALL]    = 12,
		[WorldSizeTypes.WORLDSIZE_STANDARD] = 18,
		[WorldSizeTypes.WORLDSIZE_LARGE]    = 24,
		[WorldSizeTypes.WORLDSIZE_HUGE]     = 30
	};
	local numPlates = platevalues[sizekey] or 5;
	-- Add in any plate count modifications passed in from the map script. - Bob
	numPlates = numPlates * adjust_plates;

	-- Generate fractals to govern hills and mountains - Bob
	self.hillsFrac = Fractal.Create(self.iNumPlotsX, self.iNumPlotsY, grain, self.iFlags, self.fracXExp, self.fracYExp);
	self.mountainsFrac = Fractal.Create(self.iNumPlotsX, self.iNumPlotsY, grain, self.iFlags, self.fracXExp, self.fracYExp);

	-- Use Brian's tectonics method to weave ridgelines in to the fractals.
	-- Without fractal variation, the tectonics come out too regular.
	--
	--[[ "The principle of the RidgeBuilder code is a modified Voronoi diagram. I 
	added some minor randomness and the slope might be a little tricky. It was 
	intended as a 'whole world' modifier to the fractal class. You can modify 
	the number of plates, but that is about it." ]]-- Brian Wade - May 23, 2009
	--
	-- Have the hills be clumpy with a bit of ridges in them - Brian
	self.hillsFrac:BuildRidges(numPlates, hills_ridge_flags, 1, 2);
	-- Have the mountain ranges tend to be be distinct - Brian
	self.mountainsFrac:BuildRidges((numPlates * 2) / 3, peaks_ridge_flags, 6, 1);

	-- Get height values for plot types
	local iWaterThreshold = self.continentsFrac:GetHeight(water_percent);
	local iHillsBottom1 = self.hillsFrac:GetHeight(hillsBottom1);
	local iHillsTop1 = self.hillsFrac:GetHeight(hillsTop1);
	local iHillsBottom2 = self.hillsFrac:GetHeight(hillsBottom2);
	local iHillsTop2 = self.hillsFrac:GetHeight(hillsTop2);
	local iHillsClumps = self.mountainsFrac:GetHeight(hillsClumps);
	local iHillsNearMountains = self.mountainsFrac:GetHeight(hillsNearMountains);
	local iMountainThreshold = self.mountainsFrac:GetHeight(mountains);
	local iPassThreshold = self.hillsFrac:GetHeight(hillsNearMountains);

	-- Get height values for tectonic islands
	local iMountain100 = self.mountainsFrac:GetHeight(100);
	local iMountain99 = self.mountainsFrac:GetHeight(99);
	local iMountain97 = self.mountainsFrac:GetHeight(97);
	local iMountain95 = self.mountainsFrac:GetHeight(95);
	
	--[[ Activate printout for debugging only.
	print("-"); print("--- Plot Generation Readout ---");
	print("- Sea Level Setting:", sea_level);
	print("- World Age Setting:", world_age);
	print("- Water Percentage:", water_percent);
	print("- Mountain Threshold:", mountains);
	print("- Foot Hills Threshold:", hillsNearMountains);
	print("- Clumps of Hills %:", hillsClumps);
	print("- Loose Hills %:", 4 * adjustment);
	print("- Tectonic Plate Count:", numPlates);
	print("- Tectonic Islands?", tectonic_islands);
	print("- Center Rift?", has_center_rift);
	print("- - - - - - - - - - - - - - - - -");
	]]--

	-- Main loop
	for x = 0, self.iNumPlotsX - 1 do
		for y = 0, self.iNumPlotsY - 1 do
		
			local i = y * self.iNumPlotsX + x + 1;
			local val = self.continentsFrac:GetHeight(x, y);
			local mountainVal = self.mountainsFrac:GetHeight(x, y);
			local hillVal = self.hillsFrac:GetHeight(x, y);
	
			if(val <= iWaterThreshold) then
				self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
				
				if tectonic_islands then -- Build islands in oceans along tectonic ridge lines - Brian
					if (mountainVal == iMountain100) then -- Isolated peak in the ocean
						self.plotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
					elseif (mountainVal == iMountain99) then
						self.plotTypes[i] = PlotTypes.PLOT_HILLS;
					elseif (mountainVal == iMountain97) or (mountainVal == iMountain95) then
						self.plotTypes[i] = PlotTypes.PLOT_LAND;
					end
				end
					
			else
				if (mountainVal >= iMountainThreshold) then
					if (hillVal >= iPassThreshold) then -- Mountain Pass though the ridgeline - Brian
						self.plotTypes[i] = PlotTypes.PLOT_HILLS;
					else -- Mountain
						self.plotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
					end
				elseif (mountainVal >= iHillsNearMountains) then
					self.plotTypes[i] = PlotTypes.PLOT_HILLS; -- Foot hills - Bob
				else
					if ((hillVal >= iHillsBottom1 and hillVal <= iHillsTop1) or (hillVal >= iHillsBottom2 and hillVal <= iHillsTop2)) then
						self.plotTypes[i] = PlotTypes.PLOT_HILLS;
					else
						self.plotTypes[i] = PlotTypes.PLOT_LAND;
					end
				end
			end
		end
	end

	if(shift_plot_types) then
		self:ShiftPlotTypes();
		-- Center Rift warrants plot shifting to guarantee centered landmasses.
		if(has_center_rift) then
			self:GenerateCenterRift()
		end
	end

	return self.plotTypes;
end
-------------------------------------------------------------------------------------------
