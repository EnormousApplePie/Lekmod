------------------------------------------------------------------------------
--	FILE:	 Lekmapv2.2.lua (Modified Pangaea_Plus.lua)
--	AUTHOR:  Original Bob Thomas, Changes HellBlazer, lek10, EnormousApplePie, Cirra, Meota, t0mtezuma, Jacobian
--	PURPOSE: Global map script - Simulates a Pan-Earth Supercontinent, with
--           numerous tectonic island chains.
------------------------------------------------------------------------------
--	Copyright (c) 2011 Firaxis Games, Inc. All rights reserved.
------------------------------------------------------------------------------


-- :2863 using Hax function if coastal
-- :9291 call to expand coastal plots

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
		Name = "Lekmap v6.2",
		Description = "A map script made for Lekmod based of HB's Mapscript v8.1. Pangaea - Fractal with Beta options by Jacobian",
		IsAdvancedMap = false,
		IconIndex = 0,
		SortIndex = 2,
		SupportsMultiplayer = true,
	CustomOptions = {
			-- 1
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

			-- 2
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

			-- 3
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

			-- 4
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

			-- 5
			{
				Name = "Start Quality",	-- 5 start quality
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

			-- 6
			{
				Name = "Start Distance",	-- 6 start distance
				Values = {
					"Close",
					"Normal",
					"Far - Default",
				},
				DefaultValue = 3,
				SortPriority = -94,
			},

			-- 7
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
					"Between 3-5",
					"Between 2-6",
				},
				DefaultValue = 15,
				SortPriority = -93,
			},

			-- 8
			{
				Name = "Grass Moisture",	-- add setting for grassland moisture (8)
				Values = {
					"Wet",
					"Normal",
					"Dry",
				},

				DefaultValue = 2,
				SortPriority = -92,
			},

			-- 9
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

			-- 10
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

			-- 11
			{
				Name = "Land Size X",	-- add setting for land type (11)
				Values = {
					"Default -10 tiles",
					"Default -8 tiles",
					"Default -6 tiles",
					"Default -4 tiles",
					"Default -2 tiles",
					"Default (58 on Small)",
					"Default +2 tiles",
					"Default +4 tiles",
					"Default +6 tiles",
					"Default +8 tiles",
					"Default +10 tiles",
				},

				DefaultValue = 6,
				SortPriority = -89,
			},

			-- 12
			{
				Name = "Land Size Y",	-- add setting for land type (12)
				Values = {
					"Default -10 tiles",
					"Default -8 tiles",
					"Default -6 tiles",
					"Default -4 tiles",
					"Default -2 tiles",
					"Default (52 on Small)",
					"Default +2 tiles",
					"Default +4 tiles",
					"Default +6 tiles",
					"Default +8 tiles",
					"Default +10 tiles",

				},

				DefaultValue = 6,
				SortPriority = -88,
			},

			-- 13
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

			-- 14
			{
				Name = "Balanced Regionals",	-- add setting for removing OP luxes from regional pool (14)
				Values = {
					"Yes",
					"No",
				},

				DefaultValue = 1,
				SortPriority = -90,
			},

			-- 15
			-- {
			-- 	Name = "Old Islands",	-- add setting for islands (15)
			-- 	Values = {
			-- 		"[COLOR_POSITIVE_TEXT]No Islands[ENDCOLOR]",
			-- 		"1",
			-- 		"2",
			-- 		"3",
			-- 		"4",
			-- 		"5",
			-- 		"6",
			-- 		"7",
			-- 		"8 - Old Default",
			-- 		"9",
			-- 		"10",
			-- 		"11",
			-- 		"12",
			-- 		"13",
			-- 		"14",
			-- 		"15",
			-- 		"16",
			-- 		"17",
			-- 		"18",
			-- 		"19",
			-- 		"20",
			-- 		"21",
			-- 		"22",
			-- 		"23",
			-- 		"24",
			-- 		"Between 6-10",
			-- 		"Between 8-12",
			-- 		"Between 10-14",
			-- 	},

			-- 	DefaultValue = 1,
			-- 	SortPriority = -86,
			-- },

			-- 16
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

			-- 17
			{
				Name = "Coastal Luxes",	-- Can coast spawns have non-coastal luxes (16)
				Values = {
					"Guaranteed",
					"Random",
				},

				DefaultValue = 1,
				SortPriority = -84,
			},

			-- 18
			{
				Name = "Inland Sea Spawns",	-- Can coastal civ spawn on inland seas (17)
				Values = {
					"Allowed",
					"Not allowed",
				},

				DefaultValue = 2,
				SortPriority = -83,
			},
			
			-- 19
			-- {
			-- 	Name = "Fjord Distance",	-- Distance between fjords (19)
			-- 	Values = {
			-- 		"[COLOR_POSITIVE_TEXT]No fjords[ENDCOLOR]",
			-- 		"20 tiles",
			-- 		"15 tiles",
			-- 		"12 tiles",
			-- 		"10 tiles -- Old Default",
			-- 		"8 tiles",
			-- 		"6 tiles",
			-- 	},

			-- 	DefaultValue = 1,
			-- 	SortPriority = -82,
			-- },
			
			--20
			-- {
			-- 	Name = "Fjord Length",	-- Length of fjords (20)
			-- 	Values = {
			-- 		"2 tiles -- Default",
			-- 		"3 tiles",
			-- 		"4 tiles",
			-- 		"5 tiles",
			-- 		"6 tiles",
			-- 	},

			-- 	DefaultValue = 1,
			-- 	SortPriority = -81,
			-- },

			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Bay/Lake Rework[ENDCOLOR]", -- (22)
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			{
				Name = "No Flat Desert Luxes", -- (18)
				Values = {
					"Off - Old Default",
					"On - Default",
				},
				DefaultValue = 2,
				SortPriority = -99,
			},
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Coastal CS Deadzone[ENDCOLOR]", -- (24)
			-- 	Values = {
			-- 		"4 - Old Default",
			-- 		"5",
			-- 		"6",
			-- 		"[COLOR_POSITIVE_TEXT]7[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 4,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Flexible Lux Distance[ENDCOLOR]", -- (25)
			-- 	Values = {
			-- 		"4 - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]3-5[ENDCOLOR]",
			-- 		"3-4",
			-- 		"3-4 Lower Weighted",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Meteors[ENDCOLOR]", -- (26)   -- disable
			-- 	Values = {
			-- 		"0 - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]1[ENDCOLOR]",
			-- 		"3",
			-- 		"5",
			-- 		"7",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Reworked Spawns[ENDCOLOR]", -- (26)
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"On - Weak Center Bias",
			-- 		"[COLOR_POSITIVE_TEXT]On - Medium Center Bias[ENDCOLOR]",
			-- 		"On - Strong Center Bias",
			-- 	},
			-- 	DefaultValue = 3,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Sea Side Cliffs[ENDCOLOR]", -- (27) -- adjust
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Bad CS Boost[ENDCOLOR]", -- (29) -- disable
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Independent Hill Reduction[ENDCOLOR]", -- (28)
			-- 	Values = {
			-- 		"0% - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]5%[ENDCOLOR]",
			-- 		"10%",
			-- 		"20%",
			-- 		"30%",
			-- 		"40%",
			-- 		"50%",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Remove Ice[ENDCOLOR]", -- (29)
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]Only Shallow Ice[ENDCOLOR]",
			-- 		"All Ice",
			-- 		"[COLOR_NEGATIVE_TEXT]Literally Nuke It From Orbit[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 1 Handicap[ENDCOLOR]", -- (30)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 2 Handicap[ENDCOLOR]", -- (31)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 3 Handicap[ENDCOLOR]", -- (32)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 4 Handicap[ENDCOLOR]", -- (33)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 5 Handicap[ENDCOLOR]", -- (34)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_NEGATIVE_TEXT]Player 6 Handicap[ENDCOLOR]", -- (35)
			-- 	Values = {
			-- 		"Off",
			-- 		"[COLOR_NEGATIVE_TEXT]Scout[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Worker[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Engineer[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]Great Prophet[ENDCOLOR]",
			-- 		"[COLOR_NEGATIVE_TEXT]6 Regional Copies[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 1,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Non-Coastal-CS Deadband[ENDCOLOR]", -- (36)
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"2 Hex",
			-- 		"3 Hex",
			-- 		"[COLOR_POSITIVE_TEXT]4 Hex[ENDCOLOR]",
			-- 		"5 Hex",
			-- 		"6 Hex",
			-- 		"7 Hex",
			-- 		"8 Hex",
			-- 	},
			-- 	DefaultValue = 4,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- 	Name = "[COLOR_POSITIVE_TEXT]Lake Fish[ENDCOLOR]", -- (37)
			-- 	Values = {
			-- 		"Off - Old Default",
			-- 		"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- 	},
			-- 	DefaultValue = 2,
			-- 	SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR_POSITIVE_TEXT]Larger Random Lux Pool[ENDCOLOR]", -- (38)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR_NEGATIVE_TEXT]Extra Secondary Copies[ENDCOLOR]", -- (39)
			-- Values = {
			-- 	"Off",
			-- 	"[COLOR_NEGATIVE_TEXT]1[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]2[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]3[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]4[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]5[ENDCOLOR]",
			-- },
			-- DefaultValue = 1,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:0:185:255:255]Isolation Fix[ENDCOLOR]", -- (40)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"Warning Only",
			-- 	"[COLOR:0:185:135:255]5.0 Cutoff (Low Remake Chance)[ENDCOLOR]",
			-- 	"[COLOR:0:185:255:255]7.0 Cutoff (Medium Remake Chance)[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]9.0 Cutoff (High Remake Chance)[ENDCOLOR]",
			-- },
			-- DefaultValue = 4,
			-- SortPriority = -99,
			-- },
			{
				Name = "Error Notifications", -- (19) -- change
				Values = {
					"[COLOR_NEGATIVE_TEXT]Off - This Will Make Jacobian Very Sad[ENDCOLOR]",
					"On",
					"[COLOR_NEGATIVE_TEXT]Error Testing - This Will Intentionally Cause an Error[ENDCOLOR]",
				},
				DefaultValue = 2,
				SortPriority = -99,
			},
			-- {
			-- Name = "[COLOR_NEGATIVE_TEXT]More Coal[ENDCOLOR]", -- (42)
			-- Values = {
			-- 	"Off",
			-- 	"[COLOR_NEGATIVE_TEXT]On - 8[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]On - 9[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]On - 10[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]On - 11[ENDCOLOR]",
			-- 	"[COLOR_NEGATIVE_TEXT]On - 12[ENDCOLOR]",
			-- },
			-- DefaultValue = 1,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:0:185:255:255]Min Distance (Req. Isolation Fix)[ENDCOLOR]", -- (43)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR:0:185:135:255]7[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]8[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]9[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]10[ENDCOLOR]",
			-- 	"[COLOR:0:185:255:255]11[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]12[ENDCOLOR]",
			-- },
			-- DefaultValue = 6,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:0:185:255:255]Proximal CS (Req. Isolation Fix)[ENDCOLOR]", -- (44)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR:0:185:255:255]On[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR_POSITIVE_TEXT]Buff Bad Luxes[ENDCOLOR]", -- (47) -- disable
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR_POSITIVE_TEXT]+1 Gold Incense/Truffles[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:225:155:255:230]Tectonic Rework[ENDCOLOR]", -- (45)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR:225:155:255:230]On (DO NOT TURN ON ISLANDS)[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR_POSITIVE_TEXT]Smart Regional Placement[ENDCOLOR]", -- (46)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:0:185:255:255]Reroll Count[ENDCOLOR]", -- (47)
			-- Values = {
			-- 	"[COLOR:0:185:135:255]15[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]50[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]100[ENDCOLOR]",
			-- 	"[COLOR:0:185:255:255]300[ENDCOLOR]",
			-- 	"[COLOR:0:185:135:255]99999[ENDCOLOR]",
			-- },
			-- DefaultValue = 4,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR_POSITIVE_TEXT]Better CS Placement[ENDCOLOR]", -- (48)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR_POSITIVE_TEXT]On[ENDCOLOR]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			-- {
			-- Name = "[COLOR:0:185:255:255]Spawn Validation (Req. Isolation Fix)[ENDCOLOR]", -- (49)
			-- Values = {
			-- 	"Off - Old Default",
			-- 	"[COLOR:0:185:255:255]On - Turn off If < 6 Players[COLOR:0:185:255:255]",
			-- },
			-- DefaultValue = 2,
			-- SortPriority = -99,
			-- },
			{
				Name = "Mountain Reduction", -- (20)
				Values = {
					"0% - Old Default",
					"5%",
					"10% - Default",
					"15%",
					"20%",
					"30%",
				},
				DefaultValue = 3,
				SortPriority = -99,
			},
			{
				Name = "Independent Hill Reduction", -- (21)
				Values = {
					"0% - Old Default",
					"5%",
					"10%",
					"20%",
					"30%",
					"40%",
					"50%",
				},
				DefaultValue = 1,
				SortPriority = -99,
			},
			{
			Name = "Isolation Fix", -- (22)
			Values = {
				"5.0 Cutoff (Low Remake Chance)",
				"7.0 Cutoff (Medium Remake Chance) - Default",
				"9.0 Cutoff (High Remake Chance)",
			},
			DefaultValue = 2,
			SortPriority = -99,
			},
			{
				Name = "Non-Coastal-CS Deadband", -- (23)
				Values = {
					"Off - Old Default",
					"2 Hex",
					"3 Hex - Default",
					"4 Hex",
				},
				DefaultValue = 3,
				SortPriority = -99,
			},
			{
				Name = "Sea Side Cliffs", -- (24) -- adjust
				Values = {
					"Off - Old Default",
					"On - Default",
				},
				DefaultValue = 2,
				SortPriority = -99,
			},
		},
	};
end
------------------------------------------------------------------------------
--map helper functions
function i_to_xy(i, iW, iH)
	local x = (i - 1) % iW
	return x, (i - x - 1) / iW
end
function xy_to_i(x, y, iW, iH)
	return y * iW + x + 1
end
function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

function GetAdjacent(x, y)
	local radius = 1;
	-- local iW, iH = Map.GetGridSize();

	local impact_value = 99;
	local odd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
	local even = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
	local nextX, nextY, plot_adjustments;
	-- Place Ripples
	local ripple_value = radius - 1 + 1;
	-- Moving clockwise around the ring, the first direction to travel will be Northeast.
	-- This matches the direction-based data in the odd and even tables. Each
	-- subsequent change in direction will correctly match with these tables, too.
	--
	-- Locate the plot within this ripple ring that is due West of the Impact Plot.
	local currentX = x - 1;
	local currentY = y;
	-- Now loop through the six directions, moving ripple_radius number of times
	-- per direction. At each plot in the ring, add the ripple_value for that ring 
	-- to the plot's entry in the distance data table.
	local adj_indices = {};
	for direction_index = 1, 6 do
		-- Must account for hex factor.
		if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
			plot_adjustments = odd[direction_index];
		else -- Current Y is even. Use plot adjustments from even table.
			plot_adjustments = even[direction_index];
		end
		-- Identify the next plot in the ring.
		nextX = currentX + plot_adjustments[1];
		nextY = currentY + plot_adjustments[2];
		-- Make sure the plot exists
		if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
			-- Do not add ripple data to this plot.
			adj_indices[direction_index] = -1;
		elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
			adj_indices[direction_index] = -1;
			-- Do not add ripple data to this plot.
		else -- Plot is in bounds, process it.
			-- Handle any world wrap.
			local realX = nextX;
			local realY = nextY;
			if wrapX then
				realX = realX % iW;
			end
			if wrapY then
				realY = realY % iH;
			end
			-- Record ripple data for this plot.
			adj_indices[direction_index] = realY * iW + realX + 1;
		end
		currentX, currentY = nextX, nextY;
	end
	return adj_indices;
end

------------------------------------------------------------------------------
function GetMapInitData(worldSize)
	
	local LandSizeXDuel = 22 + (Map.GetCustomOption(11) * 2);
	local LandSizeYDuel = 18 + (Map.GetCustomOption(12) * 2);

	local LandSizeXTiny = 36 + (Map.GetCustomOption(11) * 2);
	local LandSizeYTiny = 30 + (Map.GetCustomOption(12) * 2);

	local LandSizeXSmall = 46 + (Map.GetCustomOption(11) * 2);
	local LandSizeYSmall = 40 + (Map.GetCustomOption(12) * 2);

	local LandSizeXStandard = 54 + (Map.GetCustomOption(11) * 2);
	local LandSizeYStandard = 48 + (Map.GetCustomOption(12) * 2);

	local LandSizeXLarge = 62 + (Map.GetCustomOption(11) * 2);
	local LandSizeYLarge = 54 + (Map.GetCustomOption(12) * 2);

	local LandSizeXHuge = 70 + (Map.GetCustomOption(11) * 2);
	local LandSizeYHuge = 62 + (Map.GetCustomOption(12) * 2);

	local worldsizes = {};

	worldsizes = {

		[GameInfo.Worlds.WORLDSIZE_DUEL.ID] = {LandSizeXDuel, LandSizeYDuel}, -- 1020
		[GameInfo.Worlds.WORLDSIZE_TINY.ID] = {LandSizeXTiny, LandSizeYTiny}, -- 2016
		[GameInfo.Worlds.WORLDSIZE_SMALL.ID] = {LandSizeXSmall, LandSizeYSmall}, -- 3016
		[GameInfo.Worlds.WORLDSIZE_STANDARD.ID] = {LandSizeXStandard, LandSizeYStandard}, -- 3960
		[GameInfo.Worlds.WORLDSIZE_LARGE.ID] = {LandSizeXLarge, LandSizeYLarge}, -- 5032
		[GameInfo.Worlds.WORLDSIZE_HUGE.ID] = {LandSizeXHuge, LandSizeYHuge} -- 6068
		}
		
	local grid_size = worldsizes[worldSize];
	--
	local world = GameInfo.Worlds[worldSize];
	if (world ~= nil) then
		return {
			Width = grid_size[1],
			Height = grid_size[2],
			WrapX = true,
		}; 
	end

end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- START OF FRACTAL PANGAEA CREATION CODE
------------------------------------------------------------------------------
PangaeaFractalWorld = {};

------------------------------------------------------------------------------
function PangaeaFractalWorld.Create(fracXExp, fracYExp)
	local gridWidth, gridHeight = Map.GetGridSize();
	
	local data = {
		InitFractal = FractalWorld.InitFractal,
		ShiftPlotTypes = FractalWorld.ShiftPlotTypes,
		ShiftPlotTypesBy = FractalWorld.ShiftPlotTypesBy,
		DetermineXShift = FractalWorld.DetermineXShift,
		DetermineYShift = FractalWorld.DetermineYShift,
		GenerateCenterRift = FractalWorld.GenerateCenterRift,
		GeneratePlotTypes = PangaeaFractalWorld.GeneratePlotTypes,	-- Custom method
		
		iFlags = Map.GetFractalFlags(),
		
		fracXExp = fracXExp,
		fracYExp = fracYExp,
		
		iNumPlotsX = gridWidth,
		iNumPlotsY = gridHeight,
		plotTypes = table.fill(PlotTypes.PLOT_OCEAN, gridWidth * gridHeight)
	};
		
	return data;
end


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
	
	print("X Shift: " .. shift_x)
	print("Y Shift: " .. shift_y)

	if beta_tectonic_mounts == 2 then
		ShiftPlotTypesByGeneric(shift_x, shift_y, BETA_TECTONIC_LANDS)
	end

	self:ShiftPlotTypesBy(shift_x, shift_y);
end
-------------------------------------------------------------------------------------------	

-- bug fix
function FractalWorld:ShiftPlotTypesBy(xshift, yshift)
	if(xshift > 0 or yshift > 0) then
		-- local iWH = self.iNumPlotsX * self.iNumPlotsY
		local buf = {};
		-- for i = 1, iWH + 1 do
		-- 	buf[i] = self.plotTypes[i];
		-- end

		for iDestY = 0, self.iNumPlotsY do
			for iDestX = 0, self.iNumPlotsX do
				local i = self.iNumPlotsX * iDestY + iDestX + 1;
				buf[i] = self.plotTypes[i];
			end
		end
		
		for iDestY = 0, self.iNumPlotsY do
			for iDestX = 0, self.iNumPlotsX do
				local iDestI = self.iNumPlotsX * iDestY + iDestX + 1;
				local iSourceX = (iDestX + xshift) % self.iNumPlotsX;
				local iSourceY = (iDestY + yshift) % self.iNumPlotsY;
				local iSourceI = self.iNumPlotsX * iSourceY + iSourceX + 1;
				self.plotTypes[iDestI] = buf[iSourceI]
			end
		end
	end
end

function ShiftPlotTypesByGeneric(xshift, yshift, graph)
	if(xshift > 0 or yshift > 0) then

		local buf = {};
		for iDestY = 0, iH do
			for iDestX = 0, iW do
				local i = iW * iDestY + iDestX + 1;
				buf[i] = graph[i];
			end
		end
		
		for iDestY = 0, iH do
			for iDestX = 0, iW do
				local iDestI = iW * iDestY + iDestX + 1;
				local iSourceX = (iDestX + xshift) % iW;
				local iSourceY = (iDestY + yshift) % iH;
				
				local iSourceI = iW * iSourceY + iSourceX + 1
				graph[iDestI] = buf[iSourceI]
			end
		end
	end
end

function choke_check(i, map)
	local adj_is = adj_is_cache[i]
	local toggle = -1
	local toggles = 0
	for loop2, adj_i in ipairs(adj_is) do
		if map[adj_i] ~= nil then
			local toggle_value = 1
			if map[adj_i] == -1 then
				toggle_value = 0
			end
			if toggle ~= toggle_value then
				toggles = toggles + 1
				toggle = toggle_value
			end
		end
	end
	return (toggles >= 4)
end

function impassable_check(i, map)
	local adj_is = adj_is_cache[i]
	local count = 0
	for loop2, adj_i in ipairs(adj_is) do
		if map[adj_i] ~= nil then
			local toggle_value = 1
			if map[adj_i] == -1 then
				count = count+1
			end
		end
	end
	return (count >= 4)
end

function get_blobs(map)
	local blob_to_region = {}
	local blob_count = {}
	local blob_graph = table.fill(0, iW * iH)
	local blob_number = 1

	--function PlotDFS(x, y, plot_list, comp_list, comp_val)

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			if map[i] == nil then
				print("MAP GOT A NIL?? "..i)
			else
				if map[i] > 0 then
					if blob_graph[i] == 0 then
						blob_to_region[blob_number] = map[i]
						local blob_fill = {}
						PlotDFS(i, blob_fill, map, map[i])
						blob_count[blob_number] = tablelength(blob_fill)
						for l = 1, table.maxn(blob_fill) do
							if blob_fill[l] ~= nil then
								blob_graph[l] = blob_number
							end
						end
						blob_number = blob_number+1
					end
				end
			end
		end
	end

	local blobs = {}
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			if blob_graph[i] > 0 then
				if blobs[blob_graph[i]] == nil then
					blobs[blob_graph[i]] = {}
				end
				blobs[blob_graph[i]][i] = true
			end
		end
	end

	return blob_graph, blobs
end



------ v 44 overides for bug fixes --------

-- fixed bad name
function AdjacentToSaltWater(x, y)
	-- Checks a plot (x, y) to see if it is any type of land adjacent to at least one body of salt water.
	local plot = Map.GetPlot(x, y);
	local plotType = plot:GetPlotType()
	if plotType ~= PlotTypes.PLOT_OCEAN then -- This plot is land, process it.
		-- Check all adjacent plots to see if any of those are salt water.
		local directions = { DirectionTypes.DIRECTION_NORTHEAST,
		                     DirectionTypes.DIRECTION_EAST,
		                     DirectionTypes.DIRECTION_SOUTHEAST,
		                     DirectionTypes.DIRECTION_SOUTHWEST,
		                     DirectionTypes.DIRECTION_WEST,
		                     DirectionTypes.DIRECTION_NORTHWEST };
		-- 
		for loop, current_direction in ipairs(directions) do
			local testPlot = Map.PlotDirection(x, y, current_direction);
			if testPlot ~= nil then
				local plot_type = testPlot:GetPlotType()
				if plot_type == PlotTypes.PLOT_OCEAN then -- Adjacent plot is water! Check if ocean or lake.
					if testPlot:IsLake() == false then -- Adjacent plot is salt water!
						return true
					end
				end
			end
		end
	end
	-- Current plot is itself water, or else no salt water found among adjacent plots.
	return false
end

-- fixed skipping lake

function GenerateNextToCoastalLandDataTables()
	-- Set up data table for IsCoastal
	local plotDataIsCoastal = GenerateCoastalLandDataTable()

	-- Set up data table for IsNextToCoast
	local iW, iH = Map.GetGridSize();
	local plotDataIsNextToCoast = {};
	table.fill(plotDataIsNextToCoast, false, iW * iH);
	-- When generating a plot data table incrementally, process Y first so that plots go row by row.
	-- Keeping plot data table indices consistent with the main plot database could save you enormous grief.
	-- In this case, accessing an existing table by plot index, it doesn't matter.
	for x = 0, iW - 1 do
		for y = 0, iH - 1 do
			local i = iW * y + x + 1;
			local plot = Map.GetPlot(x, y);
			-- v44 fix here
			if plotDataIsCoastal[i] == false and (not plot:IsWater() or (plot:IsWater() and plot:IsFreshWater())) then -- plot is not itself on the coast or in the water.
				-- So we will check all adjacent plots to see if any of those are on the coast.
				local NEPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_NORTHEAST);
				local EPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_EAST);
				local SEPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_SOUTHEAST);
				local SWPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_SOUTHWEST);
				local WPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_WEST);
				local NWPlot = Map.PlotDirection(x, y, DirectionTypes.DIRECTION_NORTHWEST);
				-- 
				-- Check plot to northeast of current plot. This operation accounts for map edge and world wrap.
				if NEPlot ~= nil then
					local adjX = NEPlot:GetX();
					local adjY = NEPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						-- The current loop plot is not itself on the coast but is next to a plot that is on the coast.
						plotDataIsNextToCoast[i] = true;
					end
				end
				-- Check plot to east of current plot.
				if EPlot ~= nil then
					local adjX = EPlot:GetX();
					local adjY = EPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						plotDataIsNextToCoast[i] = true;
					end
				end
				-- Check plot to southeast of current plot.
				if SEPlot ~= nil then
					local adjX = SEPlot:GetX();
					local adjY = SEPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						plotDataIsNextToCoast[i] = true;
					end
				end
				-- Check plot to southwest of current plot.
				if SWPlot ~= nil then
					local adjX = SWPlot:GetX();
					local adjY = SWPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						plotDataIsNextToCoast[i] = true;
					end
				end
				-- Check plot to west of current plot.
				if WPlot ~= nil then
					local adjX = WPlot:GetX();
					local adjY = WPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						plotDataIsNextToCoast[i] = true;
					end
				end
				-- Check plot to northwest of current plot.
				if NWPlot ~= nil then
					local adjX = NWPlot:GetX();
					local adjY = NWPlot:GetY();
					local adjI = iW * adjY + adjX + 1;
					if plotDataIsCoastal[adjI] == true then
						plotDataIsNextToCoast[i] = true;
					end
				end
			end
		end
	end
	
	-- returns table, table
	return plotDataIsCoastal, plotDataIsNextToCoast
end


-- designed to work the the tectonic rework
------------------------------------------------------------------------------
function PangaeaFractalWorld:GeneratePlotTypes(args)

	-- local iW, iH = Map.GetGridSize();
	-- region_assignments = table.fill(-1, iW * iH)

	if(args == nil) then args = {}; end

	-- local beta_tectonic_mounts = Map.GetCustomOption(45)
	local beta_tectonic_mounts = 2
	local allcomplete = false;

	while allcomplete == false do

		local sea_level_low = 64;
		local sea_level_normal = 67;
		local sea_level_high = 70;
		local world_age_old = 3;
		local world_age_normal = 4;
		local world_age_new = 5;
		--
		local extra_mountains = 6;
		local grain_amount = 0;
		local adjust_plates = 1.3;
		local shift_plot_types = true;
		local tectonic_islands = true;
		local hills_ridge_flags = self.iFlags;
		local peaks_ridge_flags = self.iFlags;
		local has_center_rift = true;
		local adjadj = 1.2;
		local xshift = 0;
		local yshift = 0;
		local yshiftamt = 0;
		local xshiftamt = 0;
		local xstart, xend = 0,0;
		local ystart, yend = 0,0;

		local sea_level = Map.GetCustomOption(4)
		if sea_level == 4 then
			sea_level = 1 + Map.Rand(3, "Random Sea Level - Lua");
		end
		local world_age = Map.GetCustomOption(1)
		if world_age == 5 then
			world_age = 1 + Map.Rand(3, "Random World Age - Lua");
		end

		-- Set Sea Level according to user selection.
		local water_percent = sea_level_normal;
		-- local fjorddistmodif = Map.GetCustomOption(19);		-- Small effect added based on fjord settings
		local fjorddistmodif = 1
		-- local fjordlengthmodif = Map.GetCustomOption(20);
		local fjordlengthmodif = 1

		local beta_mountain_mod = Map.GetCustomOption(20);
		local beta_hill_mod = Map.GetCustomOption(21);
		-- local beta_mountain_mod = 4
		-- local beta_hill_mod = 2

		local hill_prob = 100
		if beta_hill_mod == 2 then
			hill_prob = 95
		elseif beta_hill_mod == 3 then
			hill_prob = 90
		elseif beta_hill_mod == 4 then
			hill_prob = 80
		elseif beta_hill_mod == 5 then
			hill_prob = 70
		elseif beta_hill_mod == 6 then
			hill_prob = 60
		elseif beta_hill_mod == 7 then
			hill_prob = 50
		end

		local fjordmodif = (fjorddistmodif - 1) * (fjordlengthmodif + 1);
		if sea_level == 1 then -- Low Sea Level
			water_percent = sea_level_low
		elseif sea_level == 3 then -- High Sea Level
			water_percent = sea_level_high
		else -- Normal Sea Level
		
		end
		water_percent = water_percent - math.floor(fjordmodif / 10);

		-- increase water to account for peninsulas
		if beta_tectonic_mounts == 2 then
			water_percent = water_percent+3
		end
		
		-- Set values for hills and mountains according to World Age chosen by user.
		local adjustment = world_age_normal;
		if world_age == 4 then -- No Moutains
			adjustment = world_age_old;
			adjust_plates = adjust_plates * 0.5;
		elseif world_age == 3 then -- 5 Billion Years
			adjustment = world_age_old;
			adjust_plates = adjust_plates * 0.5;
		elseif world_age == 1 then -- 3 Billion Years
			adjustment = world_age_new;
			adjust_plates = adjust_plates * 1;
		else -- 4 Billion Years
		end
		-- Apply adjustment to hills and peaks settings.
		local hillsBottom1 = 26 - (adjustment * adjadj);
		local hillsTop1 = 26 + (adjustment * adjadj);
		local hillsBottom2 = 72 - (adjustment * adjadj);
		local hillsTop2 = 72 + (adjustment * adjadj);
		local hillsClumps = 1 + (adjustment * adjadj);
		local hillsNearMountains = 91 - (adjustment * 2) - extra_mountains;
		local mountains = 95 - adjustment - extra_mountains;
	
		if world_age == 4 then
			mountains = 300 - adjustment - extra_mountains;
		end

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
			[WorldSizeTypes.WORLDSIZE_SMALL]    = 3,
			[WorldSizeTypes.WORLDSIZE_STANDARD] = 3,
			[WorldSizeTypes.WORLDSIZE_LARGE]    = 3,
			[WorldSizeTypes.WORLDSIZE_HUGE]		= 3
		};
		local grain = sizevalues[sizekey] or 3;
		-- Tectonics Plate Counts
		local platevalues = {
			[WorldSizeTypes.WORLDSIZE_DUEL]		= 100,
			[WorldSizeTypes.WORLDSIZE_TINY]     = 100,
			[WorldSizeTypes.WORLDSIZE_SMALL]    = 100,
			[WorldSizeTypes.WORLDSIZE_STANDARD] = 100,
			[WorldSizeTypes.WORLDSIZE_LARGE]    = 100,
			[WorldSizeTypes.WORLDSIZE_HUGE]     = 100
		};
		local numPlates = platevalues[sizekey] or 5;
		-- Add in any plate count modifications passed in from the map script. - Bob
		numPlates = numPlates * adjust_plates;

		-- Generate continental fractal layer and examine the largest landmass. Reject
		-- the result until the largest landmass occupies 90% or more of the total land.
		local bMapOK = false;
		while bMapOK == false do
			local done = false;
			local iAttempts = 0;
			local iWaterThreshold, biggest_area, iNumTotalLandTiles, iNumBiggestAreaTiles, iBiggestID;
			while done == false do
				local grain_dice = Map.Rand(7, "Continental Grain roll - LUA Pangaea");
				if grain_dice < 4 then
					grain_dice = 1;
				else
					grain_dice = 2;
				end
				local rift_dice = Map.Rand(3, "Rift Grain roll - LUA Pangaea");
				if rift_dice < 1 then
					rift_dice = -1;
				end

				rift_dice = -1;
				grain_dice = 7;

				self.continentsFrac = nil;
				self:InitFractal{continent_grain = grain_dice, rift_grain = rift_dice};
				iWaterThreshold = self.continentsFrac:GetHeight(water_percent);
		
				iNumTotalLandTiles = 0;
				for x = 0, self.iNumPlotsX - 1 do
					for y = 0, self.iNumPlotsY - 1 do
						local i = y * self.iNumPlotsX + x + 1;
						local val = self.continentsFrac:GetHeight(x, y);
						if(val <= iWaterThreshold) then
							self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
						else
							self.plotTypes[i] = PlotTypes.PLOT_LAND;
							iNumTotalLandTiles = iNumTotalLandTiles + 1;
						end
					end
				end

				SetPlotTypes(self.plotTypes);
				Map.RecalculateAreas();
		
				biggest_area = Map.FindBiggestArea(false);
				iNumBiggestAreaTiles = biggest_area:GetNumTiles();
				-- Now test the biggest landmass to see if it is large enough.
				if iNumBiggestAreaTiles >= iNumTotalLandTiles * 1 then
					done = true;
					iBiggestID = biggest_area:GetID();
				end
				iAttempts = iAttempts + 1;

				--[[--Printout for debug use only
				print("-"); print("--- Pangaea landmass generation, Attempt#", iAttempts, "---");
				print("- This attempt successful: ", done);
				print("- Total Land Plots in world:", iNumTotalLandTiles);
				print("- Land Plots belonging to biggest landmass:", iNumBiggestAreaTiles);
				print("- Percentage of land belonging to Pangaea: ", 100 * iNumBiggestAreaTiles / iNumTotalLandTiles);
				print("- Continent Grain for this attempt: ", grain_dice);
				print("- Rift Grain for this attempt: ", rift_dice);
				print("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
				print(".");--]]
		
			end

			-- Generate fractals to govern hills and mountains
			self.hillsFrac = Fractal.Create(self.iNumPlotsX, self.iNumPlotsY, grain, self.iFlags, self.fracXExp, self.fracYExp);
			self.mountainsFrac = Fractal.Create(self.iNumPlotsX, self.iNumPlotsY, grain, self.iFlags, self.fracXExp, self.fracYExp);
			self.hillsFrac:BuildRidges(numPlates, hills_ridge_flags, 1, 2);
			self.mountainsFrac:BuildRidges((numPlates * 2) / 3, peaks_ridge_flags, 6, 1);
			-- if beta_mountain_mod ~= 1 then
			-- 	self.mountainsFrac:BuildRidges((numPlates * 2) / 3, peaks_ridge_flags, 4, 4);
			-- else
			-- 	self.mountainsFrac:BuildRidges((numPlates * 2) / 3, peaks_ridge_flags, 6, 1);
			-- end
			-- 
			-- Get height values
			local iHillsBottom1 = self.hillsFrac:GetHeight(hillsBottom1);
			local iHillsTop1 = self.hillsFrac:GetHeight(hillsTop1);
			local iHillsBottom2 = self.hillsFrac:GetHeight(hillsBottom2);
			local iHillsTop2 = self.hillsFrac:GetHeight(hillsTop2);
			local iHillsClumps = self.mountainsFrac:GetHeight(hillsClumps);
			local iHillsNearMountains = 0;
			local iMountainThreshold = 0;

			-- 0, 2, 5, 8, 10, 20, 30, 40, 50
			if beta_mountain_mod == 1 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(hillsNearMountains);
				iMountainThreshold = self.mountainsFrac:GetHeight(mountains);
			elseif beta_mountain_mod == 2 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((100+hillsNearMountains*19)/20));
				iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((100+mountains*19)/20));
			elseif beta_mountain_mod == 3 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((100+hillsNearMountains*9)/10));
				iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((100+mountains*9)/10));
			elseif beta_mountain_mod == 4 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((300+hillsNearMountains*17)/20));
				iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((300+mountains*17)/20));
			elseif beta_mountain_mod == 5 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((200+hillsNearMountains*8)/10));
				iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((200+mountains*8)/10));
			elseif beta_mountain_mod == 6 then
				iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((300+hillsNearMountains*7)/10));
				iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((300+mountains*7)/10));
			-- elseif beta_mountain_mod == 6 then
			-- 	iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((400+hillsNearMountains*6)/10));
			-- 	iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((400+mountains*6)/10));
			-- elseif beta_mountain_mod == 7 then
			-- 	iHillsNearMountains = self.mountainsFrac:GetHeight(math.floor((500+hillsNearMountains*5)/10));
			-- 	iMountainThreshold = self.mountainsFrac:GetHeight(math.floor((500+mountains*5)/10));
			end
			local iPassThreshold = self.hillsFrac:GetHeight(hillsNearMountains);
			-- Get height values for tectonic islands
			
			local iMountain100 = self.mountainsFrac:GetHeight(100);
			local iMountain99 = self.mountainsFrac:GetHeight(99);
			local iMountain97 = self.mountainsFrac:GetHeight(97);
			local iMountain95 = self.mountainsFrac:GetHeight(95);

			local iShallow = self.mountainsFrac:GetHeight(93);

			local iWaterThresholdIslands1 = self.continentsFrac:GetHeight(water_percent-4);
			local iWaterThresholdIslands2 = self.continentsFrac:GetHeight(water_percent-20);
			local iWaterShallow = self.continentsFrac:GetHeight(water_percent-28);

			if beta_tectonic_mounts == 2 then
				iMountain100 = self.mountainsFrac:GetHeight(94);
				iMountain99 = self.mountainsFrac:GetHeight(85);
				-- iMountain97 = self.mountainsFrac:GetHeight(82);
				iMountain95 = self.mountainsFrac:GetHeight(68);
				iShallow = self.mountainsFrac:GetHeight(54);
			end

			-- dump_map(self.mountainsFrac)

			-- dump_map(self.mountainsFrac, iHillsNearMountains, iMountainThreshold)

			-- Because we haven't yet shifted the plot types, we will not be able to take advantage 
			-- of having water and flatland plots already set. We still have to generate all data
			-- for hills and mountains, too, then shift everything, then set plots one more time.


			--create a global that tracks tectonic land
			if beta_tectonic_mounts == 2 then
				BETA_TECTONIC_LANDS = table.fill(-1, iW * iH)
			end

			for x = 0, self.iNumPlotsX - 1 do
				for y = 0, self.iNumPlotsY - 1 do
		
					local i = y * self.iNumPlotsX + x + 1;
					local val = self.continentsFrac:GetHeight(x, y);
					local mountainVal = self.mountainsFrac:GetHeight(x, y);
					local hillVal = self.hillsFrac:GetHeight(x, y);
	
					if(val <= iWaterThreshold) then
						self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
				
						if tectonic_islands then -- Build islands in oceans along tectonic ridge lines - Brian
							if beta_tectonic_mounts == 1 then
								if (mountainVal == iMountain100) then -- Isolated peak in the ocean
									self.plotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
								elseif (mountainVal == iMountain99) then
									self.plotTypes[i] = PlotTypes.PLOT_HILLS;
								elseif (mountainVal == iMountain97) or (mountainVal == iMountain95) then
									self.plotTypes[i] = PlotTypes.PLOT_LAND;
								end
							else
								if (mountainVal >= iMountain100) and (val >= iWaterThresholdIslands2) and (val >= iWaterThresholdIslands1) == false then -- Isolated peak in the ocean
									self.plotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
									BETA_TECTONIC_LANDS[i] = 1
								elseif (mountainVal >= iMountain99) and (val >= iWaterThresholdIslands2) and (val >= iWaterThresholdIslands1) == false then
									self.plotTypes[i] = PlotTypes.PLOT_HILLS;
									BETA_TECTONIC_LANDS[i] = 1
								elseif (mountainVal >= iMountain95) and (val >= iWaterThresholdIslands2) and (val >= iWaterThresholdIslands1) == false then
									self.plotTypes[i] = PlotTypes.PLOT_LAND;
									BETA_TECTONIC_LANDS[i] = 1
								elseif (mountainVal >= iShallow) and (val >= iWaterShallow) and (val >= iWaterThreshold) == false then
									local iIsShallows = Map.Rand(100, "Shallows Chance")
									if iIsShallows > 70 then
										BETA_TECTONIC_LANDS[i] = 3
									end
								end
							end
						end
					
					else
						if beta_tectonic_mounts == 2 then
							BETA_TECTONIC_LANDS[i] = 0
						end
						if (mountainVal >= iMountainThreshold) then
							if (hillVal >= iPassThreshold) then -- Mountain Pass though the ridgeline - Brian
								self.plotTypes[i] = PlotTypes.PLOT_HILLS;
							else -- Mountain
								-- set some randomness to mountains next to each other
								local iIsMount = Map.Rand(100, "Mountain Spawn Chance");
								--print("-"); print("Mountain Spawn Chance: ", iIsMount);
								local iIsMountAdj = 48 - adjustment;
								if iIsMount >= iIsMountAdj then
									self.plotTypes[i] = PlotTypes.PLOT_MOUNTAIN;
								else
									-- set some randomness to hills or flat land next to the mountain
									local iIsHill = Map.Rand(100, "Hill Spawn Chance");
									--print("-"); print("Mountain Spawn Chance: ", iIsMount);
									local iIsHillAdj = 30 - adjustment;
									if iIsHill >= iIsHillAdj then
										self.plotTypes[i] = PlotTypes.PLOT_HILLS;
									else
										self.plotTypes[i] = PlotTypes.PLOT_LAND;
									end
								end
							end
						elseif (mountainVal >= iHillsNearMountains) then
							self.plotTypes[i] = PlotTypes.PLOT_HILLS; -- Foot hills - Bob
						else
							local iHillChance = Map.Rand(100, "Beta Hill Roll")
							if (((hillVal >= iHillsBottom1 and hillVal <= iHillsTop1) or (hillVal >= iHillsBottom2 and hillVal <= iHillsTop2))) and (iHillChance < hill_prob) then
								self.plotTypes[i] = PlotTypes.PLOT_HILLS;
							else
								self.plotTypes[i] = PlotTypes.PLOT_LAND;
							end
						end
					end
				end
			end


			self:ShiftPlotTypes();

			-- dump_map(self.plotTypes)
			-- dump_map(BETA_TECTONIC_LANDS)

			if beta_tectonic_mounts == 2 then
			
				local blob_graph, blobs = get_blobs(BETA_TECTONIC_LANDS)

				local blob_islands = {}
				-- local blob_connectors = {}

				

				for loop, blob in ipairs(blobs) do

					blob_islands[loop] = true
					local blob_connectors = {}
					local blob_depths = {}
					local blob_size = tablelength(blob)
					for i, dumby in pairs(blob) do

						--- new pass to restore hills to small islands
						if self.plotTypes[i] == PlotTypes.PLOT_LAND then
							local hill_restore = Map.Rand(blob_size*3+9, "Hill Restore")
							if hill_restore < 3 then
								self.plotTypes[i] = PlotTypes.PLOT_HILLS
							end
						end

						adj_is = adj_is_cache[i]
						for loop2, adj_i in ipairs(adj_is) do
							if BETA_TECTONIC_LANDS[adj_i] == 0 then
								blob_islands[loop] = false
								blob_connectors[i] = true
								-- BETA_TECTONIC_LANDS[i] = 2
								-- if choke_check(i, BETA_TECTONIC_LANDS) then
								-- 	BETA_TECTONIC_LANDS[i] = 3
								-- end
							end
						end
					end

					if blob_islands[loop] == false then
						local blob_depths = {}
						PlotBFS(blob_connectors, blob_depths, BETA_TECTONIC_LANDS, 1, 1)
						local total_plots = tablelength(blob_depths)
						local max_depth = 0
						local depth_plots = {}
						for i, depth in pairs(blob_depths) do
							if depth_plots[depth] == nil then
								depth_plots[depth] = {}
								if depth > max_depth then
									max_depth = depth
								end
							end
							depth_plots[depth][i] = true
						end

						local sever_plots = {}
						local severed = false
						local remaining_plots = total_plots
						for depth = 1, max_depth do
							remaining_plots = remaining_plots - tablelength(depth_plots[depth])
							if remaining_plots >= 5 and (severed == false) then
								local sever_depth = true
								for i, dumby in pairs(depth_plots[depth]) do
									if choke_check(i, BETA_TECTONIC_LANDS) == false then
										sever_depth = false
									end
								end

								if sever_depth then
									severed = true
									sever_plots = depth_plots[depth]
								end
							end
						end

						for i, dumby in pairs(sever_plots) do
							BETA_TECTONIC_LANDS[i] = 2
						end
					end
				end
			end

			-- dump_map(BETA_TECTONIC_LANDS)

			-- for i, vals in pairs(BETA_TECTONIC_LANDS) do
			-- 	self.plotTypes[i] = PlotTypes.PLOT_OCEAN
			-- 	local adj_is = adj_is_cache[i]
			-- 	if adj_is_cache[i] == nil then
			-- 		print("NIL i??? " .. i)
			-- 	else
			-- 		for loop, adj_i in ipairs(adj_is) do
			-- 			if self.plotTypes[adj_i] == PlotTypes.PLOT_MOUNTAIN then
			-- 				self.plotTypes[adj_i] = PlotTypes.PLOT_HILLS
			-- 			end
			-- 		end
			-- 	end
			-- end
			-- dump_map(blob_graph)

			-- SetPlotTypes(self.plotTypes);
			-- Map.RecalculateAreas();


			-- dump_map()
	
			--#####################
		    --beta_tectonic_mounts == 2 
			if false then
				bMapOK = true
			else

				
				--check landmass
				-- local iW, iH = Map.GetGridSize();
				local bfland = false;
				local startcol = 0;
				local cont = 0;
				local bprev = false;
				local biggest = 0;
				local mainstart = 0;
				local mainend = 0;
				local cencol = 0;
				local colshift = 0;
				local landincol = 0;
				local chkstart = 0;
				local chkend = 0;
				local chokepoint = 16;
				if beta_tectonic_mounts == 2 then
					chokepoint = 14;
				end
				local bXChkFail = false;
				local bYChkFail = false;
				local bLastLand = false;
				local contlandincol = 0;
				local xcen = 0;
				local ycen = 0;

				--check y choke points
				print("-----------------------------------");
				print("Checking Y Chokes");
				print("-----------------------------------");

				local valid_xs_from = 1
				local valid_xs_to = iW

				if beta_tectonic_mounts == 2 then
					valid_xs_from = math.ceil(iW * 1 / 4)
					valid_xs_to = math.ceil(iW * 3 / 4)
				end

				for x = valid_xs_from, valid_xs_to do
					bfland = false;
					landincol = 0;
			
					for y = 2, iH-2  do
						local i = iW * y + x + 1;
						--print("Plot Location = ", i);
						local tec_check = true
						if beta_tectonic_mounts == 2 then
							if BETA_TECTONIC_LANDS[i] > 0 then
								tec_check = false
							end
						end

						if self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN and tec_check then
							landincol = landincol + 1;
							bfland = true;
						end
					end
			
					if bfland == false then
						--print("No Land Found in Col: ", x);
						bprev = false;
						if cont > biggest then
							biggest = cont;
							mainstart = startcol;
							mainend = x-1;
						end
						cont = 0;
						startcol = 0;
					else
						--print("Land Found In Col: ", x, "Qty: ", landincol);
						if startcol == 0 then
							startcol = x;
						end
						bprev = true;
						cont = cont + 1;	
					end
				end
			
				xstart = mainstart;
				xend = mainend;

				chkstart = mainstart + 8;
				chkend = mainend -  8;

				local landincol_prev1 = chokepoint;
				local landincol_prev2 = chokepoint;

				for x = chkstart, chkend do
					landincol = 0;
					contlandincol = 0;
					for y = 2, iH-2  do
						local i = iW * y + x + 1;
						--print("Plot Location = ", i);
						local tec_check = true
						if beta_tectonic_mounts == 2 then
							if BETA_TECTONIC_LANDS[i] > 0 then
								tec_check = false
							end
						end

						if self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN and tec_check then
						
							if bLastLand == true then
								landincol = landincol + 1;
								bLastLand = true;
							else
								landincol = 1;
								bLastLand = true;
							end
						else
							if contlandincol < landincol then
								contlandincol = landincol;
							end
							bLastLand = false;
							landincol = 0;
						end
					end

					--print("Checking Col:", x, "Continuous Land In Col: ", contlandincol);

					if landincol_prev1 + landincol_prev2 + contlandincol < 3 * chokepoint then
						--print("Choke Point in Col: ", x);
						bXChkFail = true;
					end
					landincol_prev2 = contlandincol;
					landincol_prev1 = landincol_prev2;
				end



				--check x choke points
				print("-----------------------------------");
				print("Checking X Chokes");
				print("-----------------------------------");
				startcol = 0;
				cont = 0;
				biggest = 0;

				local valid_ys_from = 1
				local valid_ys_to = iH-2

				if beta_tectonic_mounts == 2 then
					valid_ys_from = math.ceil((iH-2) * 1 / 3)
					valid_ys_to = math.ceil((iH-2) * 2 / 3)
				end
				
				for y = valid_ys_from, valid_ys_to do
					bfland = false;
					landincol = 0;
			
					for x = 1, iW  do
						local i = iW * y + x + 1;
						--print("Plot Location = ", i);
						local tec_check = true
						if beta_tectonic_mounts == 2 then
							if BETA_TECTONIC_LANDS[i] > 0 then
								tec_check = false
							end
						end

						if self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN and tec_check then
							landincol = landincol + 1;
							bfland = true;
						end
					end
			
					if bfland == false then
						--print("No Land Found in Row: ", y);
						bprev = false;
						if cont > biggest then
							biggest = cont;
							mainstart = startcol;
							mainend = y-1;
						end
						cont = 0;
						startcol = 0;
					else
						--print("Land Found In Row: ", y, "Qty: ", landincol);
						if startcol == 0 then
							startcol = y;
						end
						bprev = true;
						cont = cont + 1;	
					end
				end
		
				ystart = mainstart;
				yend = mainend;

				chkstart = mainstart + 5;
				chkend = mainend -  5;
				--print("-----");
				--print("Mainland Start Row: ", chkstart);
				--print("Mainland End Row: ", chkend);
				--print("-----");
				for y = chkstart, chkend do
					landincol = 0;
					contlandincol = 0;
					for x = 1, iW  do
						local i = iW * y + x + 1;
						--print("Plot Location = ", i);
						local tec_check = true
						if beta_tectonic_mounts == 2 then
							if BETA_TECTONIC_LANDS[i] > 0 then
								tec_check = false
							end
						end

						if self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN and tec_check then
							if bLastLand == true then
								landincol = landincol + 1;
								bLastLand = true;
							else
								landincol = 1;
								bLastLand = true;
							end
						else
							if contlandincol < landincol then
								contlandincol = landincol;
							end
							bLastLand = false;
							landincol = 0;
						end
					end

					--print("Checking Col:", y, "Continuous Land In Col: ", contlandincol);

					if contlandincol < chokepoint then
						--print("Choke Point in Row: ", y);
						bYChkFail = true;
					end
				end



				if bXChkFail == true then
					print("X Check: False");
				else
					print("X Check: True");
				end

				if bYChkFail == true then
					print("Y Check: False");
				else
					print("Y Check: True");
				end

				if (bXChkFail == true or bYChkFail == true) then
					print("##############################################");
					print("Map No Good");
					print("##############################################");
					bMapOK = false;
				else
					print("##############################################");
					print("Map Passes");
					print("##############################################");
					bMapOK = true;
				
					cencol = xstart + ((xend - xstart) / 2);
					colshift = (iW/2)-cencol;
					print("Pangaea X Starts At Col: ", xstart, " And Edns At Col: ", xend);
					print("Center X of Lanmass is at Col: ", cencol, "Shift Need: ", colshift);
					xshiftamt = math.ceil(colshift);
					print("Actual Integer Shift Applied: ", xshiftamt);
					if xshiftamt > 0 then
						xshift = 1;
					elseif xshiftamt < 0 then
						xshift = 2;
					else
						xshift = 0;
					end

					print("##############################################");
					cencol = ystart + ((yend - ystart) / 2);
					colshift = (iH/2)-cencol;
					print("Pangaea Y Starts At Col: ", ystart, " And Edns At Col: ", yend);
					print("Center Y of Lanmass is at Col: ", cencol, "Shift Need: ", colshift);
					yshiftamt = math.ceil(colshift);
					print("Actual Integer Shift Applied: ", yshiftamt);
					print("##############################################");
					if yshiftamt > 0 then
						yshift = 1;
					elseif yshiftamt < 0 then
						yshift = 2;
					else
						yshift = 0;
					end
				end

			
			end

		end

		-- dump_map(self.plotTypes)
		--####################################################
		--clear area around pangaea
		if beta_tectonic_mounts == 1 then
			-- local iW, iH = Map.GetGridSize();
			for x = 0, xstart - 1 do --clear west side of map
				for y = 0, iH  do
					destPlotIndex = iW * y + x + 1;
					self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
				end
			end


			for x = xend + 1, iW  do --clear east side of map
				for y = 0, iH  do
					destPlotIndex = iW * y + x + 1;
					self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
				end
			end

			for y = 0, ystart - 1 do --clear south side of map
				for x = 0, iW  do
					destPlotIndex = iW * y + x + 1;
					self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
				end
			end
		
			for y = yend + 1, iH  do --clear north side of map
				for x = 0, iW  do
					destPlotIndex = iW * y + x + 1;
					self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
				end
			end

			--map generated now shift to center
			-- x shift first
			if xshift == 1 then --shift east
				print("-----------------------------------");
				print("Shifting East........");
				print("-----------------------------------");

				for x = iW, 0, -1 do
					for y = iH, 0, -1 do
						local destPlotIndex = iW * y + x + 1;
						local sourcePlotIndex = destPlotIndex - math.abs(xshiftamt);
						--print("Moving Plot: ", sourcePlotIndex, "To Location: ",destPlotIndex );
						self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
					end	
				end
			elseif xshift == 2 then --shift west
				print("-----------------------------------");
				print("Shifting West........");
				print("-----------------------------------");

				for x = 0, iW do
					for y = 0, iH do
						local destPlotIndex = iW * y + x + 1;
						local sourcePlotIndex = destPlotIndex + math.abs(xshiftamt);
						--print("Moving Plot: ", sourcePlotIndex, "To Location: ",destPlotIndex );
						self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
					end	
				end

			else
				--no shift
			end

			-- now shift y
			if yshift == 1 then --shift north
				print("-----------------------------------");
				print("Shifting North........");
				print("-----------------------------------");

				for y = iH, 0, -1 do
					for x = iW, 0, -1 do
						local destPlotIndex = iW * y + x + 1;
						local sourcePlotIndex = destPlotIndex - iW * (math.abs(yshiftamt));
						--print("Moving Plot: ", sourcePlotIndex, "To Location: ",destPlotIndex );
						self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
					end	
				end
			
				local i = math.abs(yshiftamt);
				for y = 0, i do
					for x = 0, iW do
						destPlotIndex = iW * y + x + 1;
						self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
					end
				end

			elseif yshift == 2 then --shift south
				print("-----------------------------------");
				print("Shifting South........");
				print("-----------------------------------");

				for y = 0, iH do
					for x = 0, iW do
						local destPlotIndex = iW * y + x + 1;
						local sourcePlotIndex = destPlotIndex + iW * (math.abs(yshiftamt));
						--print("Moving Plot: ", sourcePlotIndex, "To Location: ",destPlotIndex );
						self.plotTypes[destPlotIndex] = self.plotTypes[sourcePlotIndex]
					end	
				end
			
				local i = math.abs(yshiftamt);
				for y = iH-i, iH do
					for x = 0, iW do
						destPlotIndex = iW * y + x + 1;
						self.plotTypes[destPlotIndex] = PlotTypes.PLOT_OCEAN;
					end
				end

			else
				--no shift
			end
		else
			-- local iW, iH = Map.GetGridSize();
			for y = 0, iH-1  do
				local i_left = iW * y + 1 + 1;
				local i_right = iW * y + (iW-1)+1;
				self.plotTypes[i_left] = PlotTypes.PLOT_OCEAN;
				self.plotTypes[i_right] = PlotTypes.PLOT_OCEAN;
			end
			for x = 0, iW-1  do
				local i_top = iW * (iH-1) + x + 1;
				local i_bottom = iW * (1) + x + 1;
				self.plotTypes[i_top] = PlotTypes.PLOT_OCEAN;
				self.plotTypes[i_bottom] = PlotTypes.PLOT_OCEAN;
			end
		end

		-- dump_map(self.plotTypes)

		

		--Fjordgenerator by t0m:
		-- fjord_distance_setting = Map.GetCustomOption(19);
		fjord_distance_setting = 1;
		if fjord_distance_setting ~= 1 then
			if fjord_distance_setting == 2 then
				fjord_d = 20;
			elseif fjord_distance_setting == 3 then
				fjord_d = 15;
			elseif fjord_distance_setting == 4 then
				fjord_d = 12;
			elseif fjord_distance_setting == 5 then
				fjord_d = 10;
			elseif fjord_distance_setting == 6 then
				fjord_d = 8;
			else
				fjord_d = 6;
			end
		
			-- fjord_length_setting = Map.GetCustomOption(20);
			fjord_length_setting = 1
			if fjord_length_setting == 1 then
				fjord_l = 2;
			elseif fjord_length_setting == 2 then
				fjord_l = 3;
			elseif fjord_length_setting == 3 then
				fjord_l = 4;
			elseif fjord_length_setting == 4 then
				fjord_l = 5;
			else
				fjord_l = 6;
			end

			
			y = 9;
			k = 0;
			while (k == 0) -- Starts from bottom left going up. Fjordmaking towards right
			do
				x = 6;
				i = 0;
				while (i == 0)
				do
					local PlotIndex = iW * y + x + 1;
					if self.plotTypes[PlotIndex] ~= PlotTypes.PLOT_OCEAN then
						self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
						j = 1;
						while (j < fjord_l - 1 + Map.Rand(3, ""))
						do
							local rdm = Map.Rand(4, "")
							if (y % 2 == 0) then --even, either y increases or decreases, or x increases
								if rdm == 0 then
									y = y + 1;
								elseif rdm == 1 then
									y = y - 1;
								else
									x = x + 1;
								end
							else --odd, x increases by 1 and y increases or decreases by 1
								x = x + 1;
								if rdm == 0 then
									y = y + 1;
								elseif rdm == 1 then
									y = y - 1;
								end
							end
							if x > iW - 18 then
								i = 1;
							end
							local PlotIndex = iW * y + x + 1;
							self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
							j = j + 1;
						end
						i = 1;
					else
						x = x + 1;
						if x > iW - 18 then
							i = 1;
						end
					end
				end
				y = y + fjord_d - 2 + Map.Rand(5, "");
				if y > iH - 9 then
					k = 1;
				end
				i = 0;
			end
			y = 9;
			k = 0;
			while (k == 0)	-- Starts from bottom right going up. Fjordmaking towards left
			do
				x = iW - 6;
				i = 0;
				while (i == 0)
				do
					local PlotIndex = iW * y + x + 1;
					if self.plotTypes[PlotIndex] ~= PlotTypes.PLOT_OCEAN then
						self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
						j = 1;
						while (j < fjord_l - 1 + Map.Rand(3, ""))
						do
							local rdm = Map.Rand(4, "")
							if (y % 2 == 0) then
								x = x - 1;
								if rdm == 0 then
									y = y + 1;
								elseif rdm == 1 then
									y = y - 1;
								end
							else
								if rdm == 0 then
									y = y + 1;
								elseif rdm == 1 then
									y = y - 1;
								else
									x = x - 1;
								end
							end
							if x < 18 then
								i = 1;
							end
							local PlotIndex = iW * y + x + 1;
							self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
							j = j + 1;
						end
						i = 1;
					else
						x = x - 1;
						if x < 18 then
							i = 1;
						end
					end
				end
				y = y + fjord_d - 2 + Map.Rand(5, "");
				if y > iH - 9 then
					k = 1;
				end
				i = 0;
			end
			x = 10;
			k = 0;
			while (k == 0) -- Starts from top left going right. Fjordmaking downwards.
			do
				y = iH - 6;
				i = 0;
				while (i == 0)
				do
					local PlotIndex = iW * y + x + 1;
					if self.plotTypes[PlotIndex] ~= PlotTypes.PLOT_OCEAN then
						self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
						j = 1;
						while (j < fjord_l - 1 + Map.Rand(3, ""))
						do
							local rdm = Map.Rand(10, "")
							if (y % 2 == 0) then
								if rdm < 4 then
									y = y - 1;
								elseif rdm > 5 then
									y = y - 1;
									x = x - 1;
								elseif rdm == 4 then
									x = x - 1;
								else
									x = x + 1;
								end
							else
								if rdm < 4 then
									y = y - 1;
								elseif rdm > 5 then
									y = y - 1;
									x = x + 1;
								elseif rdm == 4 then
									x = x - 1;
								else
									x = x + 1;
								end
							end
							if y < 3 then
								i = 1;
							end
							local PlotIndex = iW * y + x + 1;
							self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
							j = j + 1;
						end
						i = 1;
					else
						y = y - 1;
						if y < 10 then
							i = 1;
						end
					end
				end
				x = x + fjord_d - 2 + Map.Rand(5, "");
				if x > iW - 10 then
					k = 1;
				end
				i = 0;
			end
			x = 10;
			k = 0;
			while (k == 0) -- Starts from bottom left going right. Fjordmaking upwards.
			do
				y = 6;
				i = 0;
				while (i == 0)
				do
					local PlotIndex = iW * y + x + 1;
					if self.plotTypes[PlotIndex] ~= PlotTypes.PLOT_OCEAN then
						self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
						j = 1;
						while (j < fjord_l - 1 + Map.Rand(3, ""))
						do
							local rdm = Map.Rand(10, "")
							if (y % 2 == 0) then
								if rdm < 4 then
									y = y + 1;
								elseif rdm > 5 then
									y = y + 1;
									x = x - 1;
								elseif rdm == 4 then
									x = x - 1;
								else
									x = x + 1;
								end
							else --odd, x increases by 1 and y increases or decreases by 1
								if rdm < 4 then
									y = y + 1;
								elseif rdm > 5 then
									y = y + 1;
									x = x + 1;
								elseif rdm == 4 then
									x = x - 1;
								else
									x = x + 1;
								end
							end
							if y > iH - 9 then
								i = 1;
							end
							local PlotIndex = iW * y + x + 1;
							self.plotTypes[PlotIndex] = PlotTypes.PLOT_OCEAN;
							j = j + 1;
						end
						i = 1;
					else
						y = y + 1;
						if y > iH - 9 then
							i = 1;
						end
					end
				end
				x = x + fjord_d - 2 + Map.Rand(5, "");
				if x > iW - 10 then
					k = 1;
				end
				i = 0;
			end
		end --fjord-process ends
		
		--#####################
		--add bays to the outter edge of the biggest landmass
		--[[
		local baysdone = false;
		local iW, iH = Map.GetGridSize();

		while baysdone == false do
			local x = Map.Rand(iW, "");
			local y = 6 + Map.Rand((iH-12), "");
			local plot = Map.GetPlot(x, y);

			if plot:IsCoastalLand() then
				--add a bay here



				print("----"); print("Bay Added"); print("----");
				baysdone = true;
			end
		end
		--]]
		--#####################


		-- Create islands. Try to make more useful islands than the default code.
		-- pick a random tile and check if it is ocean, if it is check tiles around it
		-- to see how big an island we can make, then make an island from size 1 up to the biggest we can make

		-- Hex Adjustment tables. These tables direct plot by plot scans in a radius 
		-- around a center hex, starting to Northeast, moving clockwise.
		local islandQty = {
			[WorldSizeTypes.WORLDSIZE_DUEL]		= 5,
			[WorldSizeTypes.WORLDSIZE_TINY]     = 16,
			[WorldSizeTypes.WORLDSIZE_SMALL]    = 24,
			[WorldSizeTypes.WORLDSIZE_STANDARD] = 32,
			[WorldSizeTypes.WORLDSIZE_LARGE]    = 52,
			[WorldSizeTypes.WORLDSIZE_HUGE]		= 77
		}

		local firstRingYIsEven = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

		local secondRingYIsEven = {
		{1, 2}, {1, 1}, {2, 0}, {1, -1}, {1, -2}, {0, -2},
		{-1, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-1, 2}, {0, 2}
		};

		local thirdRingYIsEven = {
		{1, 3}, {2, 2}, {2, 1}, {3, 0}, {2, -1}, {2, -2},
		{1, -3}, {0, -3}, {-1, -3}, {-2, -3}, {-2, -2}, {-3, -1},
		{-3, 0}, {-3, 1}, {-2, 2}, {-2, 3}, {-1, 3}, {0, 3}
		};

		local firstRingYIsOdd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};

		local secondRingYIsOdd = {		
		{1, 2}, {2, 1}, {2, 0}, {2, -1}, {1, -2}, {0, -2},
		{-1, -2}, {-1, -1}, {-2, 0}, {-1, 1}, {-1, 2}, {0, 2}
		};

		local thirdRingYIsOdd = {		
		{2, 3}, {2, 2}, {3, 1}, {3, 0}, {3, -1}, {2, -2},
		{2, -3}, {1, -3}, {0, -3}, {-1, -3}, {-2, -2}, {-2, -1},
		{-3, 0}, {-2, 1}, {-2, 2}, {-1, 3}, {0, 3}, {1, 3}
		};

		-- Direction types table, another method of handling hex adjustments, in combination with Map.PlotDirection()
		local direction_types = {
			DirectionTypes.DIRECTION_NORTHEAST,
			DirectionTypes.DIRECTION_EAST,
			DirectionTypes.DIRECTION_SOUTHEAST,
			DirectionTypes.DIRECTION_SOUTHWEST,
			DirectionTypes.DIRECTION_WEST,
			DirectionTypes.DIRECTION_NORTHWEST
			};


		plotTypesTwo = self.plotTypes;

		-- local iW, iH = Map.GetGridSize();
		local islMax = islandQty[sizekey] or 24;
		local mapSize = iW * iH;
		local islCount = 0;
		local islLandInRing = 0;
		local goodX = 0;
		local goodY = 0;

		local wrapX = Map:IsWrapX();
		local wrapY = false; --Map:IsWrapY();
		local nextX, nextY, plot_adjustments;
		local odd = firstRingYIsOdd;
		local even = firstRingYIsEven;
		local failedattemps = 0;
		local bIslandsFailure = false;

		local minIslandSize = 1;
		local maxIslandSize = 5;

		local escapeRedo = 500;
		local redoMap = false;

		print("######### Creating Islands #########");

		-- islandSetting = Map.GetCustomOption(15);
		islandSetting = 1
		
		if islandSetting < 26 then	
			islCount = Map.GetCustomOption(15) - 1;
		elseif islandSetting == 26 then
			islCount = Map.Rand(5, "") + 6
		elseif islandSetting == 27 then
			islCount = Map.Rand(5, "") + 8
		else
			islCount = Map.Rand(5, "") + 10
		end
		

		

		-- MOD.EAP: Island Nerf
		maxIslandSize = 3;
		


		while islCount > 0 and escapeRedo > 0 do

			local islLandInRing = 0;
			local startingPlot = 0;
			local landX = 0;
			local landY = 0;
			local landPlot = 0;

			--pick random location
			local x = Map.Rand(iW, "");
			local y = 3 + Map.Rand((iH-6), "");	
			local plotIndex = y * iW + x + 1;

			local radius = Map.Rand(4, "");
			--print("----------------------------------------------------------------------------------------");
			--print("Count: ", islCount);
			--print ("Radius: ", radius);
			--print("X=", x);
			--print("Y=", y);		
		
			--print("--------");
			--print("Random Plot Is: ", plotIndex);

			--check if random location is ocean
			if self.plotTypes[plotIndex] == PlotTypes.PLOT_OCEAN then
				
				startingPlot = plotIndex;

				--print("Location is Ocean");
				local radiuschk = 5;
	
				for ripple_radius = 1, radiuschk do
					local ripple_value = radiuschk - ripple_radius + 1;
					local currentX = x - ripple_radius;
					local currentY = y;
					for direction_index = 1, 6 do
						for plot_to_handle = 1, ripple_radius do
				 			if currentY / 2 > math.floor(currentY / 2) then
								plot_adjustments = odd[direction_index];
							else
								plot_adjustments = even[direction_index];
							end
							nextX = currentX + plot_adjustments[1];
							nextY = currentY + plot_adjustments[2];
							if wrapX == false and (nextX < 0 or nextX >= iW) then
								-- X is out of bounds.
							elseif wrapY == false and (nextY < 0 or nextY >= iH) then
								-- Y is out of bounds.
							else
								local realX = nextX;
								local realY = nextY;
								if wrapX then
									realX = realX % iW;
								end
								if wrapY then
									realY = realY % iH;
								end
								-- We've arrived at the correct x and y for the current plot.
								--local plot = Map.GetPlot(realX, realY);
								local plotIndex = realY * iW + realX + 1;
	
								--print("--------");
								--print("Plot Is: ", plotIndex);
	
								-- Check this plot for land.

								if self.plotTypes[plotIndex] == PlotTypes.PLOT_LAND then
									islLandInRing = ripple_radius;
									
									landPlot = plotIndex;

									landX = realX;
									landY = realY;

									--print("PlotID: " .. tostring(plotIndex));
									--print("RealX: " .. tostring(realX));
									--print("RealY: " .. tostring(realY));
									break;
								end

								currentX, currentY = nextX, nextY;
							end
						end

						if islLandInRing ~= 0 then
							break;
						end
					end
	
					if islLandInRing ~= 0 then
						break;
					end

				end


				if islLandInRing ~= 0 then

					--print("We hit land, check if it is the Mainland");

					local biggest_area = Map.FindBiggestArea(false);
					local biggest_ID = biggest_area:GetID();
					local plotCheck = Map.GetPlot(landX, landY);
					local plotArea = plotCheck:Area();
					local iAreaID = plotArea:GetID();
					local pullBack = 3;
					if _lek_islands_nerfed then
						pullBack = 3;
					end

					-- pull back the radius by 2 to 3 tiles and as long as island will be a radius of 2 then plunk it in da water init bruv!

--					if iAreaID == biggest_ID then
						if plotTypesTwo[landPlot] == PlotTypes.PLOT_LAND then

							-- create us an island
							islLandInRing = islLandInRing - pullBack;

							--self.plotTypes[startingPlot] = PlotTypes.PLOT_LAND

							if islLandInRing > minIslandSize and islLandInRing < maxIslandSize then

								local islThresh = 0;
								local landvarDefault = 10;

								local locationRnd = Map.Rand(100, "");

								-- MOD.EAP: Nerf island hill count, less towards outer, more towards inner
								local hill_thresh = 70;
								local inner_hill_thresh = 50;

								if (locationRnd > inner_hill_thresh) then
									self.plotTypes[startingPlot] = PlotTypes.PLOT_LAND;
								else
									self.plotTypes[startingPlot] = PlotTypes.PLOT_HILLS;
								end

								for ripple_radius = 1, islLandInRing do
									local ripple_value = islLandInRing - ripple_radius + 1;
									local currentX = x - ripple_radius;
									local currentY = y;
									for direction_index = 1, 6 do
										for plot_to_handle = 1, ripple_radius do
							 				if currentY / 2 > math.floor(currentY / 2) then
												plot_adjustments = odd[direction_index];
											else
												plot_adjustments = even[direction_index];
											end
											nextX = currentX + plot_adjustments[1];
											nextY = currentY + plot_adjustments[2];
											if wrapX == false and (nextX < 0 or nextX >= iW) then
												-- X is out of bounds.
											elseif wrapY == false and (nextY < 0 or nextY >= iH) then
												-- Y is out of bounds.
											else
												local realX = nextX;
												local realY = nextY;
												if wrapX then
													realX = realX % iW;
												end
												if wrapY then
													realY = realY % iH;
												end
												-- We've arrived at the correct x and y for the current plot.
												--local plot = Map.GetPlot(realX, realY);
												local plotIndex = realY * iW + realX + 1;
											
												-- MOD.EAP: Nerf island size	
												--local thisislandvar = Map.Rand(60, "") + landvarDefault; -- 10
										
												local thisislandvar = Map.Rand(30, "") + landvarDefault; -- x + 10 = 40 ~ 33~ smaller islands
											

												-- closer we get to outer edge increase chance of ocean.
												if _lek_islands_nerfed then
													if ripple_radius == 1  then --100%
														islThresh = Map.Rand(50, "") + thisislandvar;
													elseif ripple_radius == 2 then -- 57% to 74%
														islThresh = Map.Rand(45, "") + (thisislandvar / 1.25);
													elseif ripple_radius == 3 then --40% to 57%
														islThresh = Map.Rand(37, "") + (thisislandvar / 1.5);
													else --30% to 50%
														islThresh = Map.Rand(30, "") + (thisislandvar / 2);
													end
												else
													if ripple_radius == 1  then --100%
														islThresh = Map.Rand(50, "") + thisislandvar;
													elseif ripple_radius == 2 then -- 57% to 74%
														islThresh = Map.Rand(45, "") + (thisislandvar / 1.25);
													elseif ripple_radius == 3 then --40% to 57%
														islThresh = Map.Rand(37, "") + (thisislandvar / 1.5);
													else --30% to 50%
														islThresh = Map.Rand(30, "") + (thisislandvar / 2);
													end
												end

												local islRand = Map.Rand(100, "");
												local islHill = Map.Rand(100, "");

												--print("Rand: ", islRand, "Thresh: ", islThresh);

												if islRand > islThresh then
													self.plotTypes[plotIndex] = PlotTypes.PLOT_OCEAN
													landvarDefault = landvarDefault + 5;
												else
													if islHill <= hill_thresh then
														self.plotTypes[plotIndex] = PlotTypes.PLOT_LAND
													else
														self.plotTypes[plotIndex] = PlotTypes.PLOT_HILLS
													end
												end

												currentX, currentY = nextX, nextY;
											end
										end
									end
								end
								islCount = islCount -1;
							end
--						end
					end
				end
			end
			
			escapeRedo = escapeRedo - 1;

		end

		-- make sure islands were created
		if escapeRedo == 0 then
			--oh boy something went wrong, regen a new map
			redoMap = true
		end

		print("######### Finished Islands #########");
		
		-- local iW, iH = Map.GetGridSize();
		local centerX = iW / 2;
		local centerY = iH / 2;
		local fracFlags = {FRAC_POLAR = true};
		-- local beta_bay_diffusion =  Map.GetCustomOption(22);
		local baysFrac = nil;
		local iBaysThreshold = nil;
		
		-- local lakesFrac = nil;
		-- local iLakesThreshold = nil;
		-- local iLandThreshold = nil;

		if beta_bay_diffusion == 1 then
			baysFrac = Fractal.Create(iW, iH, 3, fracFlags, -1, -1);
			iBaysThreshold = baysFrac:GetHeight(96);
		else
			-- lakesFrac = Fractal.Create(iW, iH, 5, fracFlags, 7, 7);
			baysFrac = Fractal.Create(iW, iH, 5, fracFlags, 7, 7);
			-- iLandThreshold = self.continentsFrac:GetHeight(math.min(100, water_percent));
			iBaysThreshold = baysFrac:GetHeight(96);
			-- iLakesThreshold = lakesFrac:GetHeight(96);
		end
		

		-- local baysFrac = Fractal.Create(iW, iH, 3, fracFlags, -1, -1);

		-- dump_map(baysFrac);
		 --lakes lavel size
		local axis_list = {0.87, 0.81, 0.75};
		local axis_multiplier = axis_list[sea_level];
		local cohesion_list = {0.36, 0.33, 0.30};
		local cohesion_multiplier = cohesion_list[sea_level];
		majorAxis = centerX * cohesion_multiplier;
		minorAxis = centerY * cohesion_multiplier;
		majorAxisSquared = majorAxis * majorAxis;
		minorAxisSquared = minorAxis * minorAxis;
		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				local deltaX = x - centerX;
				local deltaY = y - centerY;
				local deltaXSquared = deltaX * deltaX;
				local deltaYSquared = deltaY * deltaY;
				local d = deltaXSquared/majorAxisSquared + deltaYSquared/minorAxisSquared;
				if d > 1 then
					local i = y * iW + x + 1;
					local baysVal = baysFrac:GetHeight(x, y);
					-- if beta_bay_diffusion == 1 then
						if baysVal >= iBaysThreshold then
							self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
							-- local plot = Map.GetPlot(x,y);
							-- plot:SetImprovementType(GameInfo.Improvements.IMPROVEMENT_PONTOON_BRIDGE.ID);
						end
					-- else
					-- 	-- local landsVal = self.continentsFrac:GetHeight(x,y);
					-- 	local lakesVal = lakesFrac:GetHeight(x,y);
					-- 	-- if baysVal-landsVal >= iBaysThreshold-iLandThreshold then
					-- 	-- 	self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
					-- 	-- end
					-- 	if lakesVal >= iLakesThreshold then
					-- 		self.plotTypes[i] = PlotTypes.PLOT_OCEAN;
					-- 	end
					-- end
				end
			end
		end
		
		if beta_tectonic_mounts == 2 then
			allcomplete = true
		else
		--check to make sure map has not failed
			local iNumLandTilesInUse = 0;
			-- local iW, iH = Map.GetGridSize();
			local iPercent = (iW * iH) * 0.30;

			for y = 0, iH - 1 do
				for x = 0, iW - 1 do
					local i = iW * y + x + 1;
					if self.plotTypes[i] ~= PlotTypes.PLOT_OCEAN then
						iNumLandTilesInUse = iNumLandTilesInUse + 1;
					end
				end
			end

			print("######### Map Failure Check #########");
			print("30% Of Map Area: ", iPercent);
			print("Map Land Tiles: ", iNumLandTilesInUse);

			if iNumLandTilesInUse >= iPercent then
				allcomplete = true;
				print("######### Map Pass #########");
			else
				print("######### Map Failure #########");
			end
		end
	end

	return self.plotTypes;
end

function PlaceImpactAndRipples2(x, y, betaDistanceData, regionNumber)
	-- local iW, iH = Map.GetGridSize();
	local wrapX = Map:IsWrapX();
	local wrapY = Map:IsWrapY();
	local impact_value = 99;

	-- local ripple_decider = Map.GetCustomOption(6);
	local ripple_values = {97, 95, 92, 88, 83, 77, 70, 62, 51, 41, 30, 18};	
	if ripple_decider == 1 then
		local ripple_values = {97, 95, 92, 89, 69, 57, 24, 15};
	end
	if ripple_decider == 2 then	
		local ripple_values = {97, 95, 92, 88, 83, 77, 70, 62, 51, 41, 30, 18};
	end
	if ripple_decider == 3 then	
		local ripple_values = {99, 98, 97, 89, 88, 83, 77, 70, 62, 51, 41, 30, 18, 12};
	end
	
	--local ripple_values = {99, 99, 99, 99, 99, 99};

	local odd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
	local even = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
	local nextX, nextY, plot_adjustments;
	local plot = Map.GetPlot(x, y);

	local impactPlotIndex = y * iW + x + 1;

	-- self.playerCoastalCollisionData[impactPlotIndex] = true
	-- Place Ripples
	for ripple_radius, ripple_value in ipairs(ripple_values) do
		-- Moving clockwise around the ring, the first direction to travel will be Northeast.
		-- This matches the direction-based data in the odd and even tables. Each
		-- subsequent change in direction will correctly match with these tables, too.
		--
		-- Locate the plot within this ripple ring that is due West of the Impact Plot.
		local currentX = x - ripple_radius;
		local currentY = y;
		-- Now loop through the six directions, moving ripple_radius number of times
		-- per direction. At each plot in the ring, add the ripple_value for that ring 
		-- to the plot's entry in the distance data table.
		for direction_index = 1, 6 do
			for plot_to_handle = 1, ripple_radius do
				-- Must account for hex factor.
			 	if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
					plot_adjustments = odd[direction_index];
				else -- Current Y is even. Use plot adjustments from even table.
					plot_adjustments = even[direction_index];
				end
				-- Identify the next plot in the ring.
				nextX = currentX + plot_adjustments[1];
				nextY = currentY + plot_adjustments[2];
				-- Make sure the plot exists
				if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
					-- Do not add ripple data to this plot.
				elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
					-- Do not add ripple data to this plot.
				else -- Plot is in bounds, process it.
					-- Handle any world wrap.
					local realX = nextX;
					local realY = nextY;
					if wrapX then
						realX = realX % iW;
					end
					if wrapY then
						realY = realY % iH;
					end
					-- Record ripple data for this plot.
					local ringPlotIndex = realY * iW + realX + 1;

					for region, distanceData in pairs(betaDistanceData) do
						if region ~= regionNumber then
							if distanceData[ringPlotIndex] > 0 then -- This plot is already in range of at least one other civ!
								-- First choose the greater of the two, existing value or current ripple.
								local stronger_value = math.max(distanceData[ringPlotIndex], ripple_value);
								-- Now increase it by 1.2x to reflect that multiple civs are in range of this plot.
								local overlap_value = math.min(97, math.floor(stronger_value * 1.4));
								distanceData[ringPlotIndex] = overlap_value;
							else
								distanceData[ringPlotIndex] = ripple_value;
							end
						end
					end
				end
				currentX, currentY = nextX, nextY;
			end
		end
	end
end

-- a way of applying ripples to an arbitrary graph

function PlaceImpactAndRipplesGeneric(x, y, rippleData, radius, impact_value)
	-- local iW, iH = Map.GetGridSize();
	local wrapX = Map:IsWrapX();
	local wrapY = Map:IsWrapY();
	-- local impact_value = 99;
	local ripple_values = table.fill(impact_value, radius)


	local odd = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {0, 1}};
	local even = {{0, 1}, {1, 0}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
	local nextX, nextY, plot_adjustments;
	local plot = Map.GetPlot(x, y);

	local impactPlotIndex = y * iW + x + 1;

	rippleData[impactPlotIndex] = impact_value

	-- Place Ripples
	for ripple_radius, ripple_value in ipairs(ripple_values) do
		-- Moving clockwise around the ring, the first direction to travel will be Northeast.
		-- This matches the direction-based data in the odd and even tables. Each
		-- subsequent change in direction will correctly match with these tables, too.
		--
		-- Locate the plot within this ripple ring that is due West of the Impact Plot.
		local currentX = x - ripple_radius;
		local currentY = y;
		-- Now loop through the six directions, moving ripple_radius number of times
		-- per direction. At each plot in the ring, add the ripple_value for that ring 
		-- to the plot's entry in the distance data table.
		for direction_index = 1, 6 do
			for plot_to_handle = 1, ripple_radius do
				-- Must account for hex factor.
			 	if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
					plot_adjustments = odd[direction_index];
				else -- Current Y is even. Use plot adjustments from even table.
					plot_adjustments = even[direction_index];
				end
				-- Identify the next plot in the ring.
				nextX = currentX + plot_adjustments[1];
				nextY = currentY + plot_adjustments[2];
				-- Make sure the plot exists
				if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
					-- Do not add ripple data to this plot.
				elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
					-- Do not add ripple data to this plot.
				else -- Plot is in bounds, process it.
					-- Handle any world wrap.
					local realX = nextX;
					local realY = nextY;
					if wrapX then
						realX = realX % iW;
					end
					if wrapY then
						realY = realY % iH;
					end
					-- Record ripple data for this plot.
					local ringPlotIndex = realY * iW + realX + 1;

					-- for region, distanceData in pairs(betaDistanceData) do
					-- 	if region ~= regionNumber then
					if rippleData[ringPlotIndex] > 0 then -- This plot is already in range of at least one other civ!
						-- First choose the greater of the two, existing value or current ripple.
						local stronger_value = math.max(rippleData[ringPlotIndex], ripple_value);
						-- Now increase it by 1.2x to reflect that multiple civs are in range of this plot.
						local overlap_value = math.min(97, math.floor(stronger_value * 1.4));
						rippleData[ringPlotIndex] = overlap_value;
					else
						rippleData[ringPlotIndex] = ripple_value;
					end
					-- 	end
					-- end
				end
				currentX, currentY = nextX, nextY;
			end
		end
	end
end

-- bug fixes

function AssignStartingPlots:PlaceImpactAndRipples(x, y)
	-- This function operates upon the "impact and ripple" data overlays. This
	-- is the core version, which operates on start points. Resources and city 
	-- states have their own data layers, using this same design principle.
	-- Execution of this function handles a single start point (x, y).
	--[[ The purpose of the overlay is to strongly discourage placement of new
	     start points near already-placed start points. Each start placed makes
	     an "impact" on the map, and this impact "ripples" outward in rings, each
	     ring weaker in bias than the previous ring. ... Civ4 attempted to adjust
	     the minimum distance between civs according to a formula that factored
	     map size and number of civs in the game, but the formula was chock full 
	     of faulty assumptions, resulting in an accurate calibration rate of less
	     than ten percent. The failure of this approach is the primary reason 
	     that an all-new positioner was written for Civ5. ... Rather than repeat
	     the mistakes of the old system, in part or in whole, I have opted to go 
	     with a flat 9-tile impact crater for all map sizes and number of civs.
	     The new system will place civs at least 9 tiles away from other civs
	     whenever and wherever a reasonable candidate plot exists at this range. 
	     If a start must be found within that range, it will attempt to balance
	     quality of the location against proximity to another civ, with the bias
	     becoming very heavy inside 7 plots, and all but prohibitive inside 5.
	     The only starts that should see any Civs crowding together are those 
	     with impossible conditions such as cramming more than a dozen civs on 
	     to Tiny or Duel sized maps. ... The Impact and Ripple is aimed mostly
	     at assisting with Rectangular Method regional division on islands maps,
	     as the primary method of spacing civs is the Center Bias factor. The 
	     Impact and Ripple is a second layer of protection, for those rare cases
	     when regional shapes are severely distorted, with little to no land in
	     the region center, and the start having to be placed near the edge, and
	     for cases of extremely thin regional dimension.   ]]--
	-- To establish a bias of 9, we Impact the overlay and Ripple outward 8 times.
	-- Value of 0 in a plot means no influence from existing Impacts in that plot.
	-- Value of 99 means an Impact occurred in that plot and it IS a start point.
	-- Values > 0 and < 99 are "ripples", meaning that plot is near a start point.
	-- local iW, iH = Map.GetGridSize();
	local wrapX = Map:IsWrapX();
	local wrapY = Map:IsWrapY();
	local impact_value = 99;

	-- local ripple_decider = Map.GetCustomOption(6);
	local ripple_values = {97, 95, 92, 88, 83, 77, 70, 62, 51, 41, 30, 18};	
	if ripple_decider == 1 then
		local ripple_values = {97, 95, 92, 89, 69, 57, 24, 15};
	end
	if ripple_decider == 2 then	
		local ripple_values = {97, 95, 92, 88, 83, 77, 70, 62, 51, 41, 30, 18};
	end
	if ripple_decider == 3 then	
		local ripple_values = {99, 98, 97, 89, 88, 83, 77, 70, 62, 51, 41, 30, 18, 12};
	end
	
	--local ripple_values = {99, 99, 99, 99, 99, 99};

	local odd = self.firstRingYIsOdd;
	local even = self.firstRingYIsEven;
	local nextX, nextY, plot_adjustments;
	local plot = Map.GetPlot(x, y);

	-- Start points need to impact the resource layers, so let's handle that first.
	self:PlaceResourceImpact(x, y, 1, 0) -- Strategic layer, at impact site only.
	self:PlaceResourceImpact(x, y, 2, 3) -- Luxury layer, set all plots within this civ start as off limits.
	self:PlaceResourceImpact(x, y, 3, 3) -- Bonus layer
	self:PlaceResourceImpact(x, y, 4, 3) -- Fish layer
	if plot:IsCoastalLand(50) then
		if self._lek_collide_coastals then
			self:PlaceResourceImpactCoastalMod(x, y, 5, 3, 4) -- MOD: SAPHT 10 range city state coastal
		else
			self:PlaceResourceImpact(x, y, 5, 5) -- Add CS layer with radius of 6 tiles
		end
	else
		self:PlaceResourceImpact(x, y, 5, 5) -- Add CS layer with radius of 6 tiles
	end

	self:PlaceResourceImpact(x, y, 6, 4) -- Natural Wonders layer, set a minimum distance of 5 plots (4 ripples) away.
	-- Now the main data layer, for start points themselves, and the City State data layer.
	-- Place Impact!
	local impactPlotIndex = y * iW + x + 1;
	self.distanceData[impactPlotIndex] = impact_value;
	self.playerCollisionData[impactPlotIndex] = true;
	self.cityStateData[impactPlotIndex] = 1;

	-- self.playerCoastalCollisionData[impactPlotIndex] = true
	-- Place Ripples
	for ripple_radius, ripple_value in ipairs(ripple_values) do
		-- Moving clockwise around the ring, the first direction to travel will be Northeast.
		-- This matches the direction-based data in the odd and even tables. Each
		-- subsequent change in direction will correctly match with these tables, too.
		--
		-- Locate the plot within this ripple ring that is due West of the Impact Plot.
		local currentX = x - ripple_radius;
		local currentY = y;
		-- Now loop through the six directions, moving ripple_radius number of times
		-- per direction. At each plot in the ring, add the ripple_value for that ring 
		-- to the plot's entry in the distance data table.
		for direction_index = 1, 6 do
			for plot_to_handle = 1, ripple_radius do
				-- Must account for hex factor.
			 	if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
					plot_adjustments = odd[direction_index];
				else -- Current Y is even. Use plot adjustments from even table.
					plot_adjustments = even[direction_index];
				end
				-- Identify the next plot in the ring.
				nextX = currentX + plot_adjustments[1];
				nextY = currentY + plot_adjustments[2];
				-- Make sure the plot exists
				if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
					-- Do not add ripple data to this plot.
				elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
					-- Do not add ripple data to this plot.
				else -- Plot is in bounds, process it.
					-- Handle any world wrap.
					local realX = nextX;
					local realY = nextY;
					if wrapX then
						realX = realX % iW;
					end
					if wrapY then
						realY = realY % iH;
					end
					-- Record ripple data for this plot.
					local ringPlotIndex = realY * iW + realX + 1;
					if self.distanceData[ringPlotIndex] > 0 then -- This plot is already in range of at least one other civ!
						-- First choose the greater of the two, existing value or current ripple.
						local stronger_value = math.max(self.distanceData[ringPlotIndex], ripple_value);
						-- Now increase it by 1.2x to reflect that multiple civs are in range of this plot.
						local overlap_value = math.min(97, math.floor(stronger_value * 1.4));
						self.distanceData[ringPlotIndex] = overlap_value;
					else
						self.distanceData[ringPlotIndex] = ripple_value;
					end
					-- Now impact the City State layer if appropriate.
					if ripple_radius <= 6 then
						self.cityStateData[ringPlotIndex] = 1;
					end
				end
				currentX, currentY = nextX, nextY;
			end
		end
	end
end

-- completely reworked spawn calculation to use a formulaic system

function AssignStartingPlots:ChooseLocations(args)

	-- local iW, iH = Map.GetGridSize();

	-- local beta_start_rework = Map.GetCustomOption(26);
	local beta_start_rework = 3
	-- local beta_tectonic_mounts = Map.GetCustomOption(45);
	local beta_tectonic_mounts = 2

	local master_center_x = 0
	local master_center_y = 0

	local biggest_area = Map.FindBiggestArea(false);
	local iBiggestAreaID = biggest_area:GetID();
	

	-- get median of the master volume

	if beta_tectonic_mounts == 2 then

		local real_tiles = 0

		local region_xs = {}
		local region_ys = {}
		

		for region_y = 0, iH - 1 do -- When handling global plot indices, process Y first.
			for region_x = 0, iW - 1 do
				local x = (region_x) % iW; -- Actual coords, adjusted for world wrap, if any.
				local y = (region_y) % iH; --
				local i = y * iW + x + 1;
				local plot = Map.GetPlot(x, y);
				local plotType = plot:GetPlotType();
				-- region_assignments[i] = assignIndex
				if (plotType == PlotTypes.PLOT_LAND or plotType == PlotTypes.PLOT_HILLS) then -- (plot:GetArea() == iBiggestAreaID)
					real_tiles = real_tiles+1
					-- region_assignments[i] = assignIndex+10
					table.insert(region_xs, region_x)
					table.insert(region_ys, region_y)
				end
			end
		end
		
		table.sort(region_xs)
		table.sort(region_ys)


		local Xc = region_xs[math.ceil(real_tiles/2)]
		local Yc = region_ys[math.ceil(real_tiles/2)]

		-- local mass_center_x = Xsum/math.max(1, real_tiles)+iWestX
		-- local mass_center_y = Ysum/math.max(1, real_tiles)+iSouthY

		-- print("MASTER X Center " .. Xc)

		local mass_center_x = Xc
		local mass_center_y = Yc
		
		-- local bullseyeX = iWestX + (iWidth / 2);
		local bullseyeX = mass_center_x
		-- local bullseyeY = iSouthY + (iHeight / 2);
		local bullseyeY = mass_center_y
		if bullseyeY / 2 ~= math.floor(bullseyeY / 2) then -- Y coord is odd, add .5 to X coord for hex-shift.
			bullseyeX = bullseyeX + 0.5;
		end

		master_center_x = bullseyeX
		master_center_y = bullseyeY

		-- print(master_center_x)
		-- print(master_center_y)
		
	end


	if beta_start_rework == 1 then

		print("Map Generation - Choosing Start Locations for Civilizations");
		local args = args or {};
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
		table.sort(averageFertilityListSorted);
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

		-- main loop
		-- lets check how many coastal civs are in the game and force that many regions to be coastal
		
		print("<<<<<<<<<<<<<<<<<< START OF REGION MANIPLUATION >>>>>>>>>>>>>>>>>>>>>");
		
		local iNumCoastNeeded = 0;
		local iNumRiverCivs, iNumPriorityCivs = 0, 0;
		local priority_lists = {};
		local res_reg = table.fill(false, self.iNumCivs);
		local reg_still_active = {};
		
		for loop = 1, self.iNumCivs do
			table.insert(reg_still_active, loop);
		end
		
		for loop = 1, self.iNumCivs do
			local playerNum = self.player_ID_list[loop]; -- MP games can have gaps between player numbers, so we cannot assume a sequential set of IDs.
			local player = Players[playerNum];
			local civType = GameInfo.Civilizations[player:GetCivilizationType()].Type;
			print("Player", playerNum, "of Civ Type", civType);
			local bNeedsCoastalStart = CivNeedsCoastalStart(civType);
			-- Roll for coastal start for weak bias civs
			if self.MixedBias and Map.Rand(100, "") >= 60 and CivNeedsPlaceFirstCoastalStart(civType) then
				bNeedsCoastalStart = false;
			end
			if bNeedsCoastalStart == true then
				print("- - - - - - - needs Coastal Start!"); print("-");
				iNumCoastNeeded = iNumCoastNeeded + 1;
			else
				local bNeedsRiverStart = CivNeedsRiverStart(civType)
				if bNeedsRiverStart == true then
					print("- - - - - - - needs River Start!"); print("-");
					iNumRiverCivs = iNumRiverCivs + 1;
				else
					local iNumRegionPriority = GetNumStartRegionPriorityForCiv(civType)
					if iNumRegionPriority > 0 then
						print("- - - - - - - needs Region Priority!"); print("-");
						local table_of_this_civs_priority_needs = GetStartRegionPriorityListForCiv_GetIDs(civType)
						iNumPriorityCivs = iNumPriorityCivs + 1;
						priority_lists[playerNum] = table_of_this_civs_priority_needs;
					end
				end
			end
		end
		
		for regcount = 1, iNumRegions do
			print("Region #", regcount, " Is type: ", self.regionTypes[regcount]);
		end
		
		print("-"); print("-"); print("--- REGION PRIORITY READOUT ---"); print("-");
		local iNumSinglePriority, iNumMultiPriority, iNumNeedFallbackPriority, iNumReserved = 0, 0, 0, 0;
		local single_priority, multi_priority, fallback_priority = {}, {}, {};
		local single_sorted, multi_sorted = {}, {};
		-- Separate priority civs in to two categories: single priority, multiple priority.
		for playerNum, priority_needs in pairs(priority_lists) do
			local len = table.maxn(priority_needs)
			if len == 1 then
				print("Player#", playerNum, "has a single Region Priority of type", priority_needs[1]);
				
				local found_reg = false;
				
				--loop thru all the regions and see if we can find a match
				for regcount = 1, iNumRegions do
					if self.regionTypes[regcount] == priority_needs[1] and found_reg == false then	
						-- this region matches this civ
						
						if res_reg[regcount] == false then
							print("Region match found for player #", playerNum, " Region #:", regcount);
							print("--");
							res_reg[regcount] = true;
							iNumReserved = iNumReserved + 1;
							found_reg = true;
							table.remove(reg_still_active, regcount);
						end
					end
				end
				
				-- if found_reg is still false at this point there are no regions left for this civs type, find the next best
				if found_reg == false then
					local iPriorityType = priority_needs[1];
					local choose_this_region = self:FindFallbackForUnmatchedRegionPriority(iPriorityType, reg_still_active)
					print("Fallback region found for player #", playerNum, " Region #:", choose_this_region);
					res_reg[choose_this_region] = true;
					iNumReserved = iNumReserved + 1;
					table.remove(reg_still_active, choose_this_region);
				end
			else
				print("Player#", playerNum, "has multiple Region Priority, this many types:", len);
				--local priority_data = {playerNum, len};
				--table.insert(multi_priority, priority_data)
				--iNumMultiPriority = iNumMultiPriority + 1;
			end
		end
		-- add extra coastals if balanced coast setting was chosen
		if self.BalancedCoastal then
			iRoll = Map.Rand(100, "Roll for extra coast");
			local iNumCoastStart = iNumCoastNeeded;
			if iNumRegions == 6 then
				if iNumCoastStart == 0 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 20 and 1 or 0) + (iRoll >= 45 and 1 or 0) + (iRoll >= 95 and 1 or 0);
				end
				if iNumCoastStart == 1 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 15 and 1 or 0) + (iRoll >= 90 and 1 or 0)
				end
				if iNumCoastStart == 2 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 90 and 1 or 0)
				end
			end
			
			if iNumRegions == 8 then
				if iNumCoastStart == 0 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 15 and 1 or 0) + (iRoll >= 35 and 1 or 0) + (iRoll >= 65 and 1 or 0) + (iRoll >= 85 and 1 or 0)
				end
				if iNumCoastStart == 1 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 10 and 1 or 0) + (iRoll >= 55 and 1 or 0) + (iRoll >= 85 and 1 or 0)
				end
				if iNumCoastStart == 2 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 35 and 1 or 0) + (iRoll >= 80 and 1 or 0)  + (iRoll >= 95 and 1 or 0)
				end
				if iNumCoastStart == 3 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 60 and 1 or 0)  + (iRoll >= 90 and 1 or 0)
				end
				if iNumCoastStart == 4 then
					iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 75 and 1 or 0)  + (iRoll >= 95 and 1 or 0)
				end
			end
			
			-- clear out reservations randomly
			local i = 1;
			while iNumRegions - iNumReserved > iNumCoastNeeded and i <= 100 do
			iRoll = Map.Rand(iNumRegions, "Roll region number to clear");
				if res_reg[iRoll] then
					res_reg[iRoll] = false;
					iNumReserved = iNumReserved - 1;
				end
				i = i + 1;
			end
		end
		-- now we have reserved the bias region all civ left must be coastal, so give them the remanining regions
		
		for assignIndex = 1, iNumRegions do
			local currentRegionNumber = regionAssignList[assignIndex];
			local bSuccessFlag = false;
			local bForcedPlacementFlag = false;
			
			print("Region #" .. currentRegionNumber);
			print("Num coastal still needed " .. tostring(iNumCoastNeeded));
			--print(tostring(self.startLocationConditions[currentRegionNumber][1]));

			if res_reg[currentRegionNumber] == false and iNumCoastNeeded > 0 then
				-- not already reserved, can be coastal
				bSuccessFlag, bForcedPlacementFlag = self:FindCoastalStart(currentRegionNumber)
				iNumCoastNeeded = iNumCoastNeeded - 1;
			else
				print("Don't Allow Spawning on Coast: " .. tostring(self.NoCoastInland));

				bSuccessFlag, bForcedPlacementFlag = self:FindStart(currentRegionNumber, self.NoCoastInland)
			end
			
			--Printout for debug only.
			print("- - -");
			print("Start Plot for Region #", currentRegionNumber, " was successful: ", bSuccessFlag);
			print("Start Plot for Region #", currentRegionNumber, " was forced: ", bForcedPlacementFlag);		
		end
		--

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
	else
		-- keep same
			-- local beta_cliffs = Map.GetCustomOption(27);
			beta_cliffs = 2

			print("Map Generation - Choosing Start Locations for Civilizations");
			local args = args or {};
			-- local iW, iH = Map.GetGridSize();
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
			table.sort(averageFertilityListSorted);
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

			-- main loop
			-- lets check how many coastal civs are in the game and force that many regions to be coastal
			
			print("<<<<<<<<<<<<<<<<<< START OF REGION MANIPLUATION >>>>>>>>>>>>>>>>>>>>>");
			
			local iNumCoastNeeded = 0;
			local iNumRiverCivs, iNumPriorityCivs = 0, 0;
			local priority_lists = {};
			local res_reg = table.fill(false, self.iNumCivs);
			local reg_still_active = {};
			
			for loop = 1, self.iNumCivs do
				table.insert(reg_still_active, loop);
			end
			
			for loop = 1, self.iNumCivs do
				local playerNum = self.player_ID_list[loop]; -- MP games can have gaps between player numbers, so we cannot assume a sequential set of IDs.
				local player = Players[playerNum];
				local civType = GameInfo.Civilizations[player:GetCivilizationType()].Type;
				print("Player", playerNum, "of Civ Type", civType);
				local bNeedsCoastalStart = CivNeedsCoastalStart(civType);
				-- Roll for coastal start for weak bias civs
				if self.MixedBias and Map.Rand(100, "") >= 60 and CivNeedsPlaceFirstCoastalStart(civType) then
					bNeedsCoastalStart = false;
				end
				if bNeedsCoastalStart == true then
					print("- - - - - - - needs Coastal Start!"); print("-");
					iNumCoastNeeded = iNumCoastNeeded + 1;
				else
					local bNeedsRiverStart = CivNeedsRiverStart(civType)
					if bNeedsRiverStart == true then
						print("- - - - - - - needs River Start!"); print("-");
						iNumRiverCivs = iNumRiverCivs + 1;
					else
						local iNumRegionPriority = GetNumStartRegionPriorityForCiv(civType)
						if iNumRegionPriority > 0 then
							print("- - - - - - - needs Region Priority!"); print("-");
							local table_of_this_civs_priority_needs = GetStartRegionPriorityListForCiv_GetIDs(civType)
							iNumPriorityCivs = iNumPriorityCivs + 1;
							priority_lists[playerNum] = table_of_this_civs_priority_needs;
						end
					end
				end
			end
			
			for regcount = 1, iNumRegions do
				print("Region #", regcount, " Is type: ", self.regionTypes[regcount]);
			end
			
			print("-"); print("-"); print("--- REGION PRIORITY READOUT ---"); print("-");
			local iNumSinglePriority, iNumMultiPriority, iNumNeedFallbackPriority, iNumReserved = 0, 0, 0, 0;
			local single_priority, multi_priority, fallback_priority = {}, {}, {};
			local single_sorted, multi_sorted = {}, {};
			-- Separate priority civs in to two categories: single priority, multiple priority.
			for playerNum, priority_needs in pairs(priority_lists) do
				local len = table.maxn(priority_needs)
				if len == 1 then
					print("Player#", playerNum, "has a single Region Priority of type", priority_needs[1]);
					
					local found_reg = false;
					
					--loop thru all the regions and see if we can find a match
					for regcount = 1, iNumRegions do
						if self.regionTypes[regcount] == priority_needs[1] and found_reg == false then	
							-- this region matches this civ
							
							if res_reg[regcount] == false then
								print("Region match found for player #", playerNum, " Region #:", regcount);
								print("--");
								res_reg[regcount] = true;
								iNumReserved = iNumReserved + 1;
								found_reg = true;
								table.remove(reg_still_active, regcount);
							end
						end
					end
					
					-- if found_reg is still false at this point there are no regions left for this civs type, find the next best
					if found_reg == false then
						local iPriorityType = priority_needs[1];
						local choose_this_region = self:FindFallbackForUnmatchedRegionPriority(iPriorityType, reg_still_active)
						print("Fallback region found for player #", playerNum, " Region #:", choose_this_region);
						res_reg[choose_this_region] = true;
						iNumReserved = iNumReserved + 1;
						table.remove(reg_still_active, choose_this_region);
					end
				else
					print("Player#", playerNum, "has multiple Region Priority, this many types:", len);
					--local priority_data = {playerNum, len};
					--table.insert(multi_priority, priority_data)
					--iNumMultiPriority = iNumMultiPriority + 1;
				end
			end
			-- add extra coastals if balanced coast setting was chosen
			if self.BalancedCoastal then
				iRoll = Map.Rand(100, "Roll for extra coast");
				local iNumCoastStart = iNumCoastNeeded;
				if iNumRegions == 6 then
					if iNumCoastStart == 0 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 20 and 1 or 0) + (iRoll >= 45 and 1 or 0) + (iRoll >= 95 and 1 or 0);
					end
					if iNumCoastStart == 1 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 15 and 1 or 0) + (iRoll >= 90 and 1 or 0)
					end
					if iNumCoastStart == 2 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 90 and 1 or 0)
					end
				end
				
				if iNumRegions == 8 then
					if iNumCoastStart == 0 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 15 and 1 or 0) + (iRoll >= 35 and 1 or 0) + (iRoll >= 65 and 1 or 0) + (iRoll >= 85 and 1 or 0)
					end
					if iNumCoastStart == 1 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 10 and 1 or 0) + (iRoll >= 55 and 1 or 0) + (iRoll >= 85 and 1 or 0)
					end
					if iNumCoastStart == 2 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 35 and 1 or 0) + (iRoll >= 80 and 1 or 0)  + (iRoll >= 95 and 1 or 0)
					end
					if iNumCoastStart == 3 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 60 and 1 or 0)  + (iRoll >= 90 and 1 or 0)
					end
					if iNumCoastStart == 4 then
						iNumCoastNeeded = iNumCoastNeeded + (iRoll >= 75 and 1 or 0)  + (iRoll >= 95 and 1 or 0)
					end
				end
				
				-- clear out reservations randomly
				local i = 1;
				while iNumRegions - iNumReserved > iNumCoastNeeded and i <= 100 do
				iRoll = Map.Rand(iNumRegions, "Roll region number to clear");
					if res_reg[iRoll] then
						res_reg[iRoll] = false;
						iNumReserved = iNumReserved - 1;
					end
					i = i + 1;
				end
			end
			-- now we have reserved the bias region all civ left must be coastal, so give them the remanining regions
		-- keep same end	

		local impassable_terrain = table.fill(0.0, iW * iH)

		for x = 0, iW-1 do
			for y = 0, iH-1 do
				local i = xy_to_i(x, y, iW, iH)
				local plot_type = Map.GetPlot(x, y):GetPlotType()
				if plot_type == PlotTypes.PLOT_OCEAN or plot_type == PlotTypes.PLOT_MOUNTAIN then
					impassable_terrain[i] = -1.0
				end
			end
		end

		local coastal_assignments = {}
		local betaDistanceData = {}
		local startPlotIndices = {}
		-- local iW, iH = Map.GetGridSize();
		local wrapX = Map:IsWrapX();
		local wrapY = Map:IsWrapY();

		-- initialize bootstrap regions
		for assignIndex = 1, iNumRegions do
			local currentRegionNumber = regionAssignList[assignIndex];
			betaDistanceData[currentRegionNumber] = table.fill(0, iW * iH);
			if res_reg[currentRegionNumber] == false and iNumCoastNeeded > 0 then
				-- not already reserved, can be coastal
				coastal_assignments[currentRegionNumber] = true
				iNumCoastNeeded = iNumCoastNeeded - 1;
			else
				coastal_assignments[currentRegionNumber] = false
			end		
		end

		local bootstrap_iters = 9

		local region_centers = {}

		

		for b_iter = 1, bootstrap_iters do
			-- execute assignment pass
			for assignIndex = 1, iNumRegions do
				local region_number = regionAssignList[assignIndex];
				local isCoastal = coastal_assignments[region_number]

				local region_data_table = self.regionData[region_number];
				local iWestX = region_data_table[1];
				local iSouthY = region_data_table[2];
				local iWidth = region_data_table[3];
				local iHeight = region_data_table[4];
				local iAreaID = region_data_table[5];
				local iMembershipEastX = iWestX + iWidth - 1;
				local iMembershipNorthY = iSouthY + iHeight - 1;
				local terrainCounts = self.regionTerrainCounts[region_number];--
				local region_type = self.regionTypes[region_number];

				local plotScores = {}




				-- NEW ---> base the "rec center" on the actual land center-of-mass, not the stupid rectangle
				-- local real_tiles = 0
				local Xsum = 0
				local Ysum = 0
				local real_tiles = 0

				local region_xs = {}
				local region_ys = {}
				

				for region_y = 0, iHeight - 1 do -- When handling global plot indices, process Y first.
					for region_x = 0, iWidth - 1 do
						local x = (region_x + iWestX) % iW; -- Actual coords, adjusted for world wrap, if any.
						local y = (region_y + iSouthY) % iH; --
						local i = y * iW + x + 1;
						local plot = Map.GetPlot(x, y);
						local plotType = plot:GetPlotType();
						-- region_assignments[i] = assignIndex
						if plotType == PlotTypes.PLOT_LAND or plotType == PlotTypes.PLOT_HILLS then
							Xsum = Xsum+region_x
							Ysum = Ysum+region_y
							real_tiles = real_tiles+1
							-- region_assignments[i] = assignIndex+10
							table.insert(region_xs, region_x)
							table.insert(region_ys, region_y)
						end
					end
				end

				table.sort(region_xs)
				table.sort(region_ys)


				local Xc = region_xs[math.ceil(real_tiles/2)]
				local Yc = region_ys[math.ceil(real_tiles/2)]

				-- local mass_center_x = Xsum/math.max(1, real_tiles)+iWestX
				-- local mass_center_y = Ysum/math.max(1, real_tiles)+iSouthY

				local mass_center_x = Xc+iWestX
				local mass_center_y = Yc+iSouthY
				
				-- local bullseyeX = iWestX + (iWidth / 2);
				local bullseyeX = mass_center_x
				if bullseyeX < iWestX then -- wrapped around: un-wrap it for test purposes.
					bullseyeX = bullseyeX + iW;
				end
				-- local bullseyeY = iSouthY + (iHeight / 2);
				local bullseyeY = mass_center_y
				if bullseyeY < iSouthY then -- wrapped around: un-wrap it for test purposes.
					bullseyeY = bullseyeY + iH;
				end
				if bullseyeY / 2 ~= math.floor(bullseyeY / 2) then -- Y coord is odd, add .5 to X coord for hex-shift.
					bullseyeX = bullseyeX + 0.5;
				end

				region_centers[assignIndex] = {math.floor(bullseyeX % iW), math.floor(bullseyeY % iH)}

				for region_y = 0, iHeight - 1 do -- When handling global plot indices, process Y first.
					for region_x = 0, iWidth - 1 do
						local x = (region_x + iWestX) % iW; -- Actual coords, adjusted for world wrap, if any.
						local y = (region_y + iSouthY) % iH; --
						local i = y * iW + x + 1;
						local plot = Map.GetPlot(x, y);
						local plotType = plot:GetPlotType();

						local plotMult = 1.0;

						--calculate fDistance for center regularization
						
						local adjusted_x = x;
						local adjusted_y = y;
						if y / 2 ~= math.floor(y / 2) then -- Y coord is odd, add .5 to X coord for hex-shift.
							adjusted_x = x + 0.5;
						end
						
						if x < iWestX then -- wrapped around: un-wrap it for test purposes.
							adjusted_x = adjusted_x + iW;
						end
						if y < iSouthY then -- wrapped around: un-wrap it for test purposes.
							adjusted_y = y + iH;
						end
						local fDistance = math.sqrt( (adjusted_x - bullseyeX)^2 + (adjusted_y - bullseyeY)^2 );
						local fMasterDistance = math.sqrt( (adjusted_x - master_center_x)^2 + (adjusted_y - master_center_y)^2 );
						local fMedDistance = math.sqrt( (mass_center_x - master_center_x)^2 + (mass_center_y - master_center_y)^2 );
						fMasterDistance = fMasterDistance/math.max(1e-10, fMedDistance)

						-- if fDistance < 1.0 then
						-- 	region_assignments[i] = assignIndex+20
						-- end
						
						--apply center reg
						local reg_factor = 1
						if beta_start_rework == 2 then
							reg_factor = (0.75-(fDistance/math.max(1, math.min(iWidth, iHeight))))*4;
							reg_factor = 1.0/(1+(math.exp(-reg_factor))^2);
						elseif beta_start_rework == 3 then
							reg_factor = (0.5-(fDistance/math.max(1, math.min(iWidth, iHeight))))*4;
							reg_factor = 1.0/(1+(math.exp(-reg_factor))^2);
						else
							reg_factor = (0.25-(fDistance/math.max(1, math.min(iWidth, iHeight))))*6;
							reg_factor = 1.0/(1+(math.exp(-reg_factor))^2);
						end
						
						
						plotMult = plotMult*reg_factor;

						if (plotType ~= PlotTypes.PLOT_HILLS) and (plotType ~= PlotTypes.PLOT_LAND) then
							plotMult = -1.0;
						end
						if self.plotDataIsNextToCoast[i] == true and (isCoastal ~= true) then
							plotMult = plotMult*0.003;
						elseif self.plotDataIsThreeFromCoast[i] == true and (isCoastal ~= true) then
							plotMult = plotMult*0.01;
						end
						if (self.plotDataIsCoastal[i] == true) and (isCoastal ~= true) then
							plotMult = -1.0
						end
						if (plot:IsCoastalLand(50) ~= true) and (isCoastal == true) then
							plotMult = plotMult*0.00001
						end
						if choke_check(i, impassable_terrain) or impassable_check(i, impassable_terrain) then
							plotMult = plotMult*0.00001
						end
						if plotMult > 0.0 then
							if plot:IsRiverSide() then
								plotMult = plotMult*1.3
							elseif plot:IsFreshWater() then
								plotMult = plotMult*1.2
							end
						end
						
						if betaDistanceData[region_number][i] > 0 then
							plotMult = plotMult*(1.0-(betaDistanceData[region_number][i]/100.0))
						end

						if plot:GetArea() ~= iBiggestAreaID then
							plotMult = 0.0
						end

						if beta_inland_prox_penalty == 2 and (isCoastal ~= true) then
							local coast_prox = GetCoastDistance(x, y)
							if coast_prox < 3 then
								plotMult = 0.0
							else
								local coast_penalty = (1.0/(1+(math.exp(-(coast_prox-3)*1.5))));
								plotMult = plotMult*coast_penalty
							end
						end
						
						if plotMult <= 0.0 then
							plotScores[i] = 0.0
						else
							local landScore = 0.0
							local blockingScore = 0.0
							-- reworked EvaluateCanidatePlot
							local isEvenY = true;
							if y / 2 > math.floor(y / 2) then
								isEvenY = false;
							end
							local adjacentMountainCount = 0;
							local foodTotal, prodTotal, goodTotal, junkTotal, riverTotal, coastScore = 0, 0, 0, 0, 0, 0;
							local search_table = {};
							local innerRingScore = 0;
							local result = self:MeasureSinglePlot(x, y, region_type, 0)
							if result[2] then
								innerRingScore = innerRingScore + 4;
								if result[5] then
									innerRingScore = innerRingScore + 4;
								end
							end
							if result[3] then
								goodTotal = goodTotal + 1;
								if result[5] then
									goodTotal = goodTotal + 1;
								end
							end
							if plot:IsRiverSide() or plot:IsFreshWater() then
								riverTotal = riverTotal + 4;
							end
							-- Evaluate First Ring
							if isEvenY then
								search_table = self.firstRingYIsEven;
							else
								search_table = self.firstRingYIsOdd;
							end

							for loop, plot_adjustments in ipairs(search_table) do
								local searchX, searchY;
								if wrapX then
									searchX = (x + plot_adjustments[1]) % iW;
								else
									searchX = x + plot_adjustments[1];
								end
								if wrapY then
									searchY = (y + plot_adjustments[2]) % iH;
								else
									searchY = y + plot_adjustments[2];
								end
								--
								if searchX < 0 or searchX >= iW or searchY < 0 or searchY >= iH then
									-- This plot does not exist. It's off the map edge.
									junkTotal = junkTotal + 1;
								else
									local searchPlot = Map.GetPlot(searchX, searchY);
									local result = self:MeasureSinglePlot(searchX, searchY, region_type, 1)

									if searchPlot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN or searchPlot:IsLake() then
										blockingScore = blockingScore + 4.0
									end

									if result[4] then
										junkTotal = junkTotal + 1;
										if searchPlot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN and adjacentMountainCount == 0 then
											--junkTotal = junkTotal - 1;
											adjacentMountainCount = adjacentMountainCount + 1;
										elseif result[5] then
											junkTotal = junkTotal + 1;
										end

										if searchPlot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN and beta_cliffs and isCoastal == true then
											junkTotal = junkTotal + 20;
										end

									else
										if result[1] then
											foodTotal = foodTotal + 1;
											if result[5] then
												foodTotal = foodTotal + 1;
											end
										end
										if result[2] then
											prodTotal = prodTotal + 1;
											if result[5] then
												prodTotal = prodTotal + 1;
											end
										end
										if result[3] then
											goodTotal = goodTotal + 1;
											if result[5] then
												goodTotal = goodTotal + 1;
											end
										end
										if searchPlot:IsRiverSide() or plot:IsFreshWater() then
											riverTotal = riverTotal + 2;
										end
									end
								end
							end

							if foodTotal < self.minFoodInner then
								goodSoFar = false;
							elseif prodTotal < self.minProdInner then
								goodSoFar = false;
							elseif goodTotal < self.minGoodInner then
								goodSoFar = false;
							end

							local weightedFoodInner = {0, 8, 14, 19, 22, 24, 25};
							local foodResultInner = weightedFoodInner[foodTotal + 1];
							local weightedProdInner = {0, 10, 16, 20, 20, 12, 0};
							local prodResultInner = weightedProdInner[prodTotal + 1];
							local goodResultInner = goodTotal * 2;
							innerRingScore = innerRingScore + foodResultInner + prodResultInner + goodResultInner + riverTotal - (junkTotal * 3);
							
							-- Evaluate Second Ring
							if isEvenY then
								search_table = self.secondRingYIsEven;
							else
								search_table = self.secondRingYIsOdd;
							end

							for loop, plot_adjustments in ipairs(search_table) do
								local searchX, searchY;
								if wrapX then
									searchX = (x + plot_adjustments[1]) % iW;
								else
									searchX = x + plot_adjustments[1];
								end
								if wrapY then
									searchY = (y + plot_adjustments[2]) % iH;
								else
									searchY = y + plot_adjustments[2];
								end
								if searchX < 0 or searchX >= iW or searchY < 0 or searchY >= iH then
									-- This plot does not exist. It's off the map edge.
									junkTotal = junkTotal + 1;
								else
									local result = self:MeasureSinglePlot(searchX, searchY, region_type, 2)
									local searchPlot = Map.GetPlot(searchX, searchY);

									if searchPlot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN or searchPlot:IsLake() then
										blockingScore = blockingScore + 2.0
									end

									if result[4] then
										junkTotal = junkTotal + 1;
										if result[5] then
											junkTotal = junkTotal + 1;
										end
									else
										if result[1] then
											foodTotal = foodTotal + 1;
											if result[5] then
												foodTotal = foodTotal + 1;
											end
										end
										if result[2] then
											prodTotal = prodTotal + 1;
											if result[5] then
												prodTotal = prodTotal + 1;
											end
										end
										if result[3] then
											goodTotal = goodTotal + 1;
											if result[5] then
												goodTotal = goodTotal + 1;
											end
										end
										if searchPlot:IsRiverSide() or plot:IsFreshWater() then
											riverTotal = riverTotal + 2;
										end
									end
								end
							end

							-- Check the results from the second ring against the established targets.
							if foodTotal < self.minFoodMiddle then
								goodSoFar = false;
							-- extra minProd beta-change
							elseif prodTotal < self.minProdMiddle then
								goodSoFar = false;
							elseif goodTotal < self.minGoodMiddle then
								goodSoFar = false;
							end

							-- Update up the "score" for this candidate. Middle ring results weigh significantly.
							local weightedFoodMiddle = {0, 2, 5, 10, 20, 25, 28, 30, 32, 34, 35}; -- 35 for any further values.
							local foodResultMiddle = 35;
							if foodTotal < 10 then
								foodResultMiddle = weightedFoodMiddle[foodTotal + 1];
							end
							local weightedProdMiddle = {0, 10, 20, 25, 30, 35}; -- 35 for any further values.
							local effectiveProdTotal = prodTotal;
							if foodTotal * 2 < prodTotal then
								effectiveProdTotal = math.ceil(foodTotal / 2);
							end
							local prodResultMiddle = 35;
							if effectiveProdTotal < 5 then
								prodResultMiddle = weightedProdMiddle[effectiveProdTotal + 1];
							end
							local goodResultMiddle = goodTotal * 2;
							local middleRingScore = foodResultMiddle + prodResultMiddle + goodResultMiddle + riverTotal - (junkTotal * 3);
							
							-- Evaluate Third Ring
							if isEvenY then
								search_table = self.thirdRingYIsEven;
							else
								search_table = self.thirdRingYIsOdd;
							end

							for loop, plot_adjustments in ipairs(search_table) do
								local searchX, searchY;
								if wrapX then
									searchX = (x + plot_adjustments[1]) % iW;
								else
									searchX = x + plot_adjustments[1];
								end
								if wrapY then
									searchY = (y + plot_adjustments[2]) % iH;
								else
									searchY = y + plot_adjustments[2];
								end
								if searchX < 0 or searchX >= iW or searchY < 0 or searchY >= iH then
									-- This plot does not exist. It's off the map edge.
									junkTotal = junkTotal + 1;
								else
									local result = self:MeasureSinglePlot(searchX, searchY, region_type, 3)
									local searchPlot = Map.GetPlot(searchX, searchY);
									if searchPlot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN or searchPlot:IsLake() then
										blockingScore = blockingScore + 1.0
									end

									if result[4] then
										junkTotal = junkTotal + 1;
										if result[5] then
											junkTotal = junkTotal + 1;
										end
									else
										if result[1] then
											foodTotal = foodTotal + 1;
											if result[5] then
												foodTotal = foodTotal + 1;
											end
										end
										if result[2] then
											prodTotal = prodTotal + 1;
											if result[5] then
												prodTotal = prodTotal + 1;
											end
										end
										if result[3] then
											goodTotal = goodTotal + 1;
											if result[5] then
												goodTotal = goodTotal + 1;
											end
										end
										if searchPlot:IsRiverSide() or plot:IsFreshWater() then
											riverTotal = riverTotal + 2;
										end
									end
								end
							end

							-- Check the results from the third ring against the established targets.
							if foodTotal < self.minFoodOuter then
								goodSoFar = false;
							elseif prodTotal < self.minProdOuter then
								goodSoFar = false;
							elseif goodTotal < self.minGoodOuter then
								goodSoFar = false;
							end
							if junkTotal > self.maxJunk then
								goodSoFar = false;
							end

							-- Tally the final "score" for this candidate.
							local outerRingScore = foodTotal + prodTotal + goodTotal + riverTotal - (junkTotal * 2);
							local finalScore = innerRingScore + middleRingScore + outerRingScore + coastScore;

							if goodSoFar == true then
								landScore = math.max(0, finalScore)
							else
								landScore = math.max(finalScore)*0.7
							end

							local block_factor = (0.75-((blockingScore*1.5)/(math.max(1, math.min(iWidth, iHeight))*8.0)))*4;
							if beta_tectonic_mounts == 2 then
								local center_mult = 2.0*(1.0/(1+(math.exp(-(fMasterDistance-1.0)*1.5))^2));
								block_factor = 10.0*block_factor*center_mult-(fMasterDistance-1.0)*1.5
							end
							local blocking_mult = 1.0/(1+(math.exp(-block_factor))^2);
							-- record final value
							plotScores[i] = plotMult*landScore*blocking_mult
						end
					end
				end

				-- Get Best Scored Index
				local iBest = 0;
				local scoreBest = -1;
				for region_y = 0, iHeight - 1 do -- When handling global plot indices, process Y first.
					for region_x = 0, iWidth - 1 do
						local x = (region_x + iWestX) % iW; -- Actual coords, adjusted for world wrap, if any.
						local y = (region_y + iSouthY) % iH; --
						local i = y * iW + x + 1;

						if plotScores[i] > scoreBest then
							iBest = i
							scoreBest = plotScores[i]
						end
					end
				end
				startPlotIndices[region_number] = iBest;
			end
			-- print("Master Centers " .. master_center_x .. " " .. master_center_y)
			-- region_assignments[xy_to_i(math.ceil(master_center_x), math.ceil(master_center_y), iW, iH)] = 40

			-- dump_map(region_assignments)
			-- execute the bootstrap update
			-- reset
			for assignIndex = 1, iNumRegions do
				local currentRegionNumber = regionAssignList[assignIndex];
				for y = 0, iH - 1 do
					for x = 0, iW - 1 do
						local i = xy_to_i(x, y, iW, iH)
						betaDistanceData[currentRegionNumber][i] = math.ceil(betaDistanceData[currentRegionNumber][i]/7)
					end
				end
				-- betaDistanceData[currentRegionNumber] = table.fill(0, iW * iH);
			end
			-- refill
			for assignIndex = 1, iNumRegions do
				local currentRegionNumber = regionAssignList[assignIndex];
				local i = startPlotIndices[currentRegionNumber];
				local x = (i - 1) % iW;
				local y = (i - x - 1) / iW;
				PlaceImpactAndRipples2(x, y, betaDistanceData, currentRegionNumber);
				print("Center X:" .. region_centers[assignIndex][1])
				PlaceImpactAndRipples2(region_centers[assignIndex][1], region_centers[assignIndex][2], betaDistanceData, currentRegionNumber);
			end
		end

		-- actually perform the placements
		for assignIndex = 1, iNumRegions do
			local currentRegionNumber = regionAssignList[assignIndex];
			local i = startPlotIndices[currentRegionNumber]
			local x = (i - 1) % iW;
			local y = (i - x - 1) / iW;
			self.startingPlots[currentRegionNumber] = {x, y, 1}
			self:PlaceImpactAndRipples(x, y)
		end
	end
end

-- removed region dependencies from land quality measure

function AssignStartingPlots:MeasureSinglePlot(x, y, region_type, distance_from_city)

	-- local beta_start_rework = Map.GetCustomOption(26);

	local data = table.fill(false, 5);
	-- Note that "Food" is not strictly about tile yield.
	-- Different regions get their food in different ways.
	-- Tundra, Jungle, Forest, Desert, Plains regions will 
	-- get Bonus resource support to cover food shortages.
	--
	-- Data table entries hold results; all begin as false:
	-- [1] "Food"
	-- [2] "Prod"
	-- [3] "Good"
	-- [4] "Junk"
	-- [5] "Count Double" (For hills on city plot as production and snow as junk)
	-- local iW, iH = Map.GetGridSize();
	local plot = Map.GetPlot(x, y);
	local plotType = plot:GetPlotType()
	local terrainType = plot:GetTerrainType()
	local featureType = plot:GetFeatureType()
	
	if plotType == PlotTypes.PLOT_MOUNTAIN then -- Mountains are Junk
		data[4] = true;
		return data
	elseif plotType == PlotTypes.PLOT_OCEAN then
		if featureType == FeatureTypes.FEATURE_ICE then -- Icebergs are Junk.
			data[4] = true;
		elseif plot:IsLake() then -- Lakes are Food, not good.
			data[1] = true;
		elseif terrainType == TerrainTypes.TERRAIN_COAST then 
			data[1] = true;
			if self.method == 3 or self.method == 4 then -- Shallow water is Good for Archipelago-type maps.
				data[3] = true;
			end
		end
		-- Other water plots are ignored.
		return data
	end

	if featureType == FeatureTypes.FEATURE_JUNGLE and distance_from_city ~= 0 then -- Jungles are Food except in Grass regions and only Good in Jungle regions.
		if plotType == PlotTypes.PLOT_HILLS then -- Jungle hill count as Prod but not Good.
			data[2] = true;
		end

		data[1] = true; -- Hard BETA change

		-- if region_type ~= 7 then -- Region type is not Grass.
		-- 	data[1] = true;
		-- 	if region_type == 2 then -- Region type is jungle
		-- 		data[3] = true;
		-- 	end
		-- end


		return data
	elseif featureType == FeatureTypes.FEATURE_FOREST and distance_from_city ~= 0 then -- Forests are Prod, Good.
		data[2] = true;
		if plotType == PlotTypes.PLOT_HILLS then
			data[3] = true;
		else
			-- tile under forest is a flat land tile so must be food
			if terrainType ~= TerrainTypes.TERRAIN_TUNDRA then -- must be flat plains or grassland
				data[1] = true;
			end
		end

		return data
	elseif featureType == FeatureTypes.FEATURE_OASIS then -- Oases are Food, Good.
		data[1] = true;
		data[3] = true;
		return data
	elseif featureType == FeatureTypes.FEATURE_FLOOD_PLAINS then -- Flood Plains are Food, Good.
		data[1] = true;
		data[3] = true;
		return data
	elseif featureType == FeatureTypes.FEATURE_MARSH and distance_from_city ~= 0 then -- Marsh are ignored.
		
		-- HARD BETA CHANGE
		

		-- -- marsh is good for wetlands region types
		-- if region_type == 9 then
		-- 	data[3] = true;
		-- else
		-- 	data[4] = true;
		-- end

		return data
	end

	if plotType == PlotTypes.PLOT_HILLS then -- Hills with no features are Prod, Good.
		data[2] = true;
		data[3] = true;
		return data
	end
	
	-- If we have reached this point in the process, the plot is flatlands.
	if terrainType == TerrainTypes.TERRAIN_SNOW then -- Snow are Junk.
		data[4] = true;
		data[5] = true;
		return data
		
	elseif terrainType == TerrainTypes.TERRAIN_DESERT then -- Non-Oasis, non-FloodPlain flat deserts are Junk, except in Desert regions.
		-- if region_type == 4 and beta_start_rework == 1 then
		-- 	data[3] = true;
		-- else
		data[4] = true;  ----- HARD BETA CHANGE
		-- end
		return data

	elseif terrainType == TerrainTypes.TERRAIN_TUNDRA then -- Naked Tundra are Junk, except in Tundra Regions where they are Food

		--HARD BETA CHANGE
		-- if region_type == 1 then
		-- 	data[3] = true;
		-- else
		-- 	data[4] = true;
		-- end
		return data

	elseif terrainType == TerrainTypes.TERRAIN_PLAINS then -- Plains are Good for all region types, but Food in only non-Grassland.
		data[3] = true; -- HARD BETA CHANGE
		-- if region_type == 1 or region_type == 4 or region_type == 5 or region_type == 6 or region_type == 8 then
		-- 	data[1] = true;
		-- end
		return data

	elseif terrainType == TerrainTypes.TERRAIN_GRASS then -- Grass is Food, Good for all region types.
		data[1] = true;
		data[3] = true;
		return data
	end

	-- If we have arrived here, the plot has non-standard terrain.
	print("Encountered non-standard terrain.");
	return data
end

------------------------------------------------------------------------------

-- actually performs the pinch
function PinchTectonics()
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			i = xy_to_i(x, y, iW, iH)
			if BETA_TECTONIC_LANDS[i] == 2 then
				plot = Map.GetPlot(x, y)
				plot:SetPlotType(PlotTypes.PLOT_OCEAN)
				local adj_is = adj_is_cache[i]
				for loop, adj_i in ipairs(adj_is) do
					local adj_x, adj_y = i_to_xy(adj_i, iW, iH)
					local adj_plot = Map.GetPlot(adj_x, adj_y)
					if adj_plot ~= nil then
						if adj_plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
							adj_plot:SetPlotType(PlotTypes.PLOT_HILLS)
						end
					end
				end
			end 
		end
	end
	Map.RecalculateAreas()
end
------------------------------------------------------------------------------
function GeneratePlotTypes()
	-- Plot generation customized to ensure enough land belongs to the Pangaea.
	print("Generating Plot Types (Lua Pangaea) ...");
	
	local fractal_world = PangaeaFractalWorld.Create();

	local plotTypes = fractal_world:GeneratePlotTypes();
	-- dump_map(fractal_world);
	
	SetPlotTypes(plotTypes);
	if beta_tectonic_mounts == 2 then
		PinchTectonics();
	end

	GenerateCoasts();
end
------------------------------------------------------------------------------
function GenerateTerrain()

	local DesertPercent = 22;

	-- Get Temperature setting input by user.
	local temp = Map.GetCustomOption(2)
	if temp == 4 then
		temp = 1 + Map.Rand(3, "Random Temperature - Lua");
	end

	local grassMoist = Map.GetCustomOption(8);

	local args = {
			temperature = temp,
			iDesertPercent = DesertPercent,
			iGrassMoist = grassMoist,
			};

	local terraingen = TerrainGenerator.Create(args);
	

	-- added local beta-27
	local terrainTypes = terraingen:GenerateTerrain();
	
	SetTerrainTypes(terrainTypes);

	-- MOD.EAP: New
	Map.RecalculateAreas();
	FixCoastLine();
	
	FixIslands();

end

------------------------------------------------------------------------------
function FixIslands()
	--function to change some of the flat land tundra on islands to plains tiles
	-- local iW, iH = Map.GetGridSize();
	local biggest_area = Map.FindBiggestArea(false);
	local iAreaID = biggest_area:GetID();

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = iW * y + x + 1;
			local plot = Map.GetPlotByIndex(i-1);
			plotAreaID = plot:GetArea();
			if plotAreaID ~= iAreaID then
				local terrainType = plot:GetTerrainType();
				local plotType = plot:GetPlotType();

				if terrainType == TerrainTypes.TERRAIN_TUNDRA then
					if plotType ~= PlotTypes.PLOT_HILLS then
						--give a chance to turn this flat tundra to plains
						local tundratoplains = Map.Rand(100, "Plains Spawn Chance");
						if tundratoplains >= 30 then
							plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true);
						end
					end
				end
			end
		end
	end
end

function LakeCheck(x, y)
	-- local iW, iH = Map.GetGridSize();
	local adj_is1 = GetAdjacent(x,y);
	for loop, adj_i1 in ipairs(adj_is1) do
		local adjx1, adjy1 = i_to_xy(adj_i1, iW, iH);
		local adj_plot1 = Map.GetPlot(adjx1, adjy1);
		if adj_plot1:IsLake() == true then
			return true
		end
	end
	return false
end



function GetCoastDistance(x, y)
	-- local iW, iH = Map.GetGridSize();
	if has_cached_coast == false then
		CacheCoast()
	end
	local i = xy_to_i(x, y, iW, iH);
	if coast_cache[i] ~= nil then
		return coast_cache[i]
	else
		return -1
	end
end


function CacheCoast()
	local coast_adj = {};
	-- local iW, iH = Map.GetGridSize();
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = iW * y + x + 1;
			local plot = Map.GetPlot(x, y);
			if plot:IsCoastalLand(50) then
				coast_cache[i] = 0;
				local adj_is1 = adj_is_cache[i]
				for loop, adj_i1 in ipairs(adj_is1) do
					if(adj_i1 ~= -1) then
						-- coast_adj[adj_i1] = 1
						local adjx1, adjy1 = i_to_xy(adj_i1, iW, iH);
						local adj_plot1 = Map.GetPlot(adjx1, adjy1);
						if adj_plot1 ~= nil then
							if (adj_plot1:IsCoastalLand(50) ~= true) and (adj_plot1:GetPlotType() ~= PlotTypes.PLOT_OCEAN) then
								coast_adj[adj_i1] = 1;
							end
						else
							coast_adj[adj_i1] = 1;
						end
					end
				end
			end
		end
	end
	local loop_depth = 1
	while tablelength(coast_adj) > 0 and loop_depth < 100 do
		local coast_adj_temp = {}
		for i, dumby in pairs(coast_adj) do
			coast_cache[i] = loop_depth
			-- local x, y = i_to_xy(i, iW, iH)
			local adj_is1 = adj_is_cache[i]
			for loop, adj_i1 in ipairs(adj_is1) do
				if (coast_cache[adj_i1] == nil) and (coast_adj[adj_i1] == nil) then
					coast_adj_temp[adj_i1] = 1;
				end
			end
		end
		coast_adj = {}
		for i, dumby in pairs(coast_adj_temp) do
			coast_adj[i] = 1
		end
		loop_depth = loop_depth+1
	end
	has_cached_coast = true
end

function AssignStartingPlots:CanPlaceCityStateAt(x, y, area_ID, force_it, ignore_collisions)
	-- local beta_cliffs = Map.GetCustomOption(28);
	-- local beta_coast_zone = Map.GetCustomOption(38);
	--beta_tectonic_mounts

	-- local iW, iH = Map.GetGridSize();
	local plot = Map.GetPlot(x, y);
	local area = plot:GetArea()
	local biggest_area = Map.FindBiggestArea(false);
	local iAreaID = biggest_area:GetID();

	if beta_coast_zone ~= 1 then
		if plot:IsCoastalLand(50) == false then
			local coast_dist = GetCoastDistance(x, y)
			if coast_dist ~= -1 and coast_dist <= beta_coast_zone then
				return false
			end
		end
	end

	if beta_tectonic_mounts == 2 then
		local i = xy_to_i(x, y, iW, iH)
		local adj_is = adj_is_cache[i]
		local adj_dead = 0
		local adj2_dead = 0
		for loop, adj_i in ipairs(adj_is) do
			local adj_x, adj_y = i_to_xy(adj_i, iW, iH)
			local adj_plot = Map.GetPlot(adj_x, adj_y)
			if adj_plot == nil then
				adj_dead = adj_dead+1
				adj2_dead = adj2_dead+4
			else
				if adj_plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN or adj_plot:GetPlotType() == PlotTypes.PLOT_OCEAN then
					adj_dead = adj_dead+1
					adj2_dead = adj2_dead+1
				end

				

			end
		end
		if adj_dead >= 3 then
			return false
		end
	end

	if self.method == 1 then
		if area_ID ~= iAreaID then
			return false
		end
	end

	if area ~= area_ID and area_ID ~= -1 then
		return false
	end
	local plotType = plot:GetPlotType()
	if plotType == PlotTypes.PLOT_OCEAN then
		return false
	end
	if beta_cliffs == 1 then
		if plotType == PlotTypes.PLOT_MOUNTAIN then
			return false
		end
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
function FixCoastLine()

	-- MOD.EAP: Add a few more hills to the coast
	-- local iW, iH = Map.GetGridSize();
	local biggest_area = Map.FindBiggestArea(false);
	local iAreaID = biggest_area:GetID();

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = iW * y + x + 1;
			local plot = Map.GetPlotByIndex(i-1);
			plotAreaID = plot:GetArea();
			-- only the mainland please
			if plotAreaID == iAreaID then

				local plotType = plot:GetPlotType();
				if plot:IsCoastalLand(50) and (plot:IsFreshWater() == false) then 
					if plotType ~= PlotTypes.PLOT_HILLS then
						if not plot:IsRiverSide() then
							local flatToHill = Map.Rand(100, "Plains Spawn Chance");
							if flatToHill >= 80 then
								print("Adding Hills to Coast");
								plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true);
							end
						end
					end
					
				end
			end
		end
	end
end

------------------------------------------------------------------------------
function AddFeatures()

	-- Get Rainfall setting input by user.
	local rain = Map.GetCustomOption(3)
	if rain == 4 then
		rain = 1 + Map.Rand(3, "Random Rainfall - Lua");
	end
	
	local args = {rainfall = rain}
	local featuregen = FeatureGenerator.Create(args);

	-- False parameter removes mountains from coastlines.
	-- local beta_cliffs = Map.GetCustomOption(27);
	beta_cliffs = 2
	if beta_cliffs == 1 then
		featuregen:AddFeatures(false);
	else
		featuregen:AddFeatures(true);
	end
end
------------------------------------------------------------------------------

-- full map print functionality

function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. ''..k..': ' .. dump(v) .. ', '
      end
      return s .. '}'
   else
      return tostring(o)
   end
end

function dump_map(graph)
	local map_output = {};
	-- local iW, iH = Map.GetGridSize();
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = iW * y + x+1;
			-- local plot = Map.GetPlot(x, y);
			-- local boost = 0

			-- if fractal:GetHeight(x, y) >= thresh1 then
			-- 	boost = boost+200
			-- end

			-- if fractal:GetHeight(x, y) >= thresh2 then
			-- 	boost = boost+200
			-- end

			-- map_output[i] = {x, y, fractal:GetHeight(x, y)+boost}
			
			-- 
			-- if coast_adj[i] ~= nill then
			-- 	map_output[i] = {x, y, 2}
			-- else
			-- 	map_output[i] = {x, y, 4}
			-- end

			map_output[i] = {x, y, graph[i]}
			-- if plot:GetPlotType() ~= PlotTypes.PLOT_OCEAN then
			-- 	map_output[i] = {x, y, 1}
			-- else
			-- 	map_output[i] = {x, y, 0}
			-- end
			
			-- if coast_cache[i] ~= nill then
			-- 	map_output[i] = {x, y, coast_cache[i]}
			-- else
			-- 	map_output[i] = {x, y, 32}
			-- end
			
			-- if plot:IsCoastalLand(50) == true then
			-- 	map_output[i] = {x, y, 1}
			-- else
			-- 	map_output[i] = {x, y, 0}
			-- end

			-- if (plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN) then
			-- 	map_output[i] = {x, y, 2}
			-- elseif plot:GetPlotType() == PlotTypes.PLOT_OCEAN then
			-- 	map_output[i] = {x, y, 0};
			-- else
			-- 	map_output[i] = {x, y, 1};
			-- end
			-- local thresh = fractal:GetHeight(94)
			-- local boost = 0
			-- if fractal:GetHeight(x, y) >= thresh then
			-- 	boost = 200
			-- end
			-- map_output[i] = {x, y, fractal:GetHeight(x, y)+boost}

		end
	end
	print("MAP READOUT:");
	print(dump(map_output));
	print("MAP READOUT OVER");
end

-- added functionality to remove flat-desert luxes

function AssignStartingPlots:FixResourceGraphics()

	local rain = Map.GetCustomOption(2)	-- BETA MOVED THIS OUTSIDE FOR PERFORMANCE
	local remove_desert_lux = Map.GetCustomOption(18);
	
	--[[ MOD.Barathor: 
	
		 Hijacked this function and it now fixes many resource types.  Formerly, this function was FixSugarJungles.
		 This function does not adjust terrain types (except when under jungle), so that terrain bands stay intact.
	
		 This allows much greater flexibility when assigning resources to the map. ]]

		 							-- if beta_lake_fish == 2 then
							-- 	if res_plot:IsLake() then
							-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_FOOD, -1)
							-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
							-- 	end
							-- end
		 
	-- local iW, iH = Map.GetGridSize()
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			
			local plot = Map.GetPlot(x, y)
			local res_ID = plot:GetResourceType(-1)
			local featureType = plot:GetFeatureType()
			local terrainType = plot:GetTerrainType()
			local plotType = plot:GetPlotType()
			-- local resourceType = plot:GetResourceType();
			
			-- Mined/Quarried Resources

			-- if beta_lake_fish == 2 then
			-- 	if plot:IsLake() and res_ID == GameInfo.Resources.RESOURCE_FISH.ID then
			-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_FOOD, -1)
			-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
			-- 	end
			-- end

			if res_ID == self.marble_ID or 
			   res_ID == self.gold_ID or 
			   res_ID == self.silver_ID or 
			   res_ID == self.copper_ID or 
			   res_ID == self.gems_ID or 
			   res_ID == self.salt_ID or 
			   res_ID == self.lapis_ID or 
			   res_ID == self.jade_ID or 
			   res_ID == self.obsidian_ID or
			   res_ID == self.amber_ID then 
			   
				-- If a forest or jungle is present, keep it.  Remove anything else.
				if (featureType ~= FeatureTypes.FEATURE_FOREST) and (featureType ~= FeatureTypes.FEATURE_JUNGLE) then
					plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1)
				end
				-- Always want hills.
				-- plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true)		-- MOD.Barathor: Keep flat mined resources as well.
				
			-- Tree Resources
			elseif res_ID == self.cocoa_ID or 
			       res_ID == self.citrus_ID or 
				   res_ID == self.spices_ID or 
				   res_ID == self.sugar_ID or 
				   res_ID == self.truffles_ID or 
				   res_ID == self.silk_ID or 
				   res_ID == self.dye_ID or 
				   res_ID == self.fur_ID or
				   red_ID == self.coconut_ID or
				   red_ID == self.rubber_ID or
				   res_ID == self.hardwood_ID or
				   res_ID == self.deer_ID then
				
				if (featureType ~= FeatureTypes.FEATURE_FOREST) then
					plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1)
				elseif (featureType ~= FeatureTypes.FEATURE_JUNGLE) then
					plot:SetFeatureType(FeatureTypes.FEATURE_JUNGLE, -1)
				end
				
				if res_ID == self.fur_ID then
					-- Always want it flat.  The foxes fall into the hills.
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, true)
				end
				
				-- MOD.Barathor: Gets the latitude of the tile to determine jungle eligibility.  Note: I like to use symmetrical latitudes, with an equator the width of two rows, which my map script utilizes.
				local lat = 0
				if (y >= (iH/2)) then
					lat = math.abs((iH/2) - y)/(iH/2)
				else
					lat = math.abs((iH/2) - (y + 1))/(iH/2)
				end
				local AvgJungleRange = 0
				--[[ MOD.Barathor: 
					 Get Rainfall setting to determine appropriate jungle range.
					 Assumed Formula: (jungle percent) / ((jungle percent) * (jungle factor)) = X; Jungle will form from 0.0 to X, decreasing as it gets further.
					 Example: Normal Rainfall: 65 / (65 * 5) = 0.2; Divide by 2 to get the mid range, which we'll use to blend it in.  ]]
				
				if rain == 1 then		
					-- Arid
					AvgJungleRange = 0.08
				elseif rain == 3 then	
					-- Wet
					AvgJungleRange = 0.25
				else					
					-- Normal or Random (Note: I'm currently not sure how to retrieve random, so we'll just use normal for now.)
					AvgJungleRange = 0.12
				end
				
				-- Always want it covered for most tree resources.
				if (featureType == FeatureTypes.FEATURE_MARSH) then
					if res_ID == self.sugar_ID or res_ID == self.spices_ID or res_ID == self.dye_ID or res_ID == self.rubber_ID or res_ID == self.coconut_ID then
						-- Keep it marsh for these resources.
					else
						-- Add some jungle or forest.
						if lat <= AvgJungleRange then
							if res_ID ~= self.deer_ID and res_ID ~= self.fur_ID then
								plot:SetFeatureType(FeatureTypes.FEATURE_JUNGLE, -1)
								plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
							else
								plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1)
							end
						else
							plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1)
						end	
					end
				else
					-- Add some jungle or forest.
					if lat <= AvgJungleRange then
						if res_ID ~= self.deer_ID and res_ID ~= self.fur_ID then
							plot:SetFeatureType(FeatureTypes.FEATURE_JUNGLE, -1)
							plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
						else
							plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1)
						end
					else
						plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1)
					end		
				end
				
			-- Open Land Resources
			elseif res_ID == self.incense_ID or 
			       res_ID == self.ivory_ID or 
				   res_ID == self.wine_ID or 
				   res_ID == self.olives_ID or 
				   res_ID == self.coffee_ID or
				   res_ID == self.tobacco_ID or 
				   res_ID == self.tea_ID or 
				   res_ID == self.perfume_ID or 
				   res_ID == self.cotton_ID then 
				
				--if res_ID == self.ivory_ID then
					-- Always want it flat.  Other types are fine on hills.
					plot:SetPlotType(PlotTypes.PLOT_LAND, false, true)
				--end				
				
				-- Don't remove flood plains if present for the few that are placed on it, only remove other features, like marsh or any trees.				
				if (featureType ~= FeatureTypes.FEATURE_FLOOD_PLAINS) then	
					plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1)
				end
				
				-- if res_ID == self.incense_ID then
				-- 	-- Because incense is very restricted, it was expanded to look for grass tiles as a final fallback.
				-- 	-- This will help with certain distributions that incense previously didn't work well in, such as assignments to city-states which could be hit or miss.
				-- 	-- Besides jungle placements, this is the only luxury which will change the terrain it's found on.  Plus, plains are mixed in with grass anyway.
				-- 	if terrainType == TerrainTypes.TERRAIN_GRASS then
				-- 		plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
				-- 	end
				-- 	if terrainType == TerrainTypes.TERRAIN_DESERT and featureType ~= FeatureTypes.FEATURE_FLOOD_PLAINS then
				-- 		print("Naughty Incense Current Check")
				-- 		plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
				-- 	end
				-- end
			end

			
			featureType = plot:GetFeatureType()
			terrainType = plot:GetTerrainType()
			plotType = plot:GetPlotType()
			if remove_desert_lux == 2 then
				if 	res_ID == self.marble_ID or 
					res_ID == self.gold_ID or 
					res_ID == self.silver_ID or 
					res_ID == self.copper_ID or 
					res_ID == self.gems_ID or 
					res_ID == self.salt_ID or 
					res_ID == self.lapis_ID or 
					res_ID == self.jade_ID or 
					res_ID == self.obsidian_ID or
					res_ID == self.amber_ID or

					res_ID == self.cocoa_ID or 
					res_ID == self.citrus_ID or 
					res_ID == self.spices_ID or 
					res_ID == self.sugar_ID or 
					res_ID == self.truffles_ID or 
					res_ID == self.silk_ID or 
					res_ID == self.dye_ID or 
					res_ID == self.fur_ID or
					red_ID == self.coconut_ID or
					red_ID == self.rubber_ID or
					res_ID == self.hardwood_ID or
					res_ID == self.deer_ID or

					res_ID == self.incense_ID or 
					res_ID == self.ivory_ID or 
					res_ID == self.wine_ID or 
					res_ID == self.olives_ID or 
					res_ID == self.coffee_ID or
					res_ID == self.tobacco_ID or 
					res_ID == self.tea_ID or 
					res_ID == self.perfume_ID or 
					res_ID == self.cotton_ID then

						if terrainType == TerrainTypes.TERRAIN_DESERT and featureType ~= FeatureTypes.FEATURE_FLOOD_PLAINS and plotType ~= PlotTypes.PLOT_HILLS then
							plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
							-- plot:SetOwner(63, -1)
							-- plot:SetPlotType(PlotTypes.PLOT_MOUNTAIN)
							-- plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT)
							-- print("HIT BARB")
							-- plot:SetImprovementType(GameInfo.Improvements.IMPROVEMENT_BARBARIAN_CAMP.ID)
							-- Barbarians:DoCampActivationNotice(plot)
							-- local barbPlayer = Players[63]
							-- local barbCity = barbPlayer:InitCity(x, y);
							-- local GDR = barbPlayer:InitUnit(23, x,y);
							-- GDR:DomainCargo();
							-- local gun_unit = barbPlayer:InitUnit(43, x, y);
							-- gun_unit:SetMoves(0);
							-- barbPlayer:InitUnit(43, x, y)
							-- barbPlayer:InitCity(x, y)
							-- for id, player in pairs(Players) do
							-- 	-- Events.GameMessageChat(63, id, "YOUR DESERT LUX IS OURS!!!")
							-- 	player:AddNotification(NotificationTypes.NOTIFICATION_GENERIC, "YOUR LAND IS OURS", "BARB ANNOUNCEMENT")
							-- end
							
						end
				end
			end
		end
	end
	--[[
	-- Sugar could not be made to look good in both jungle and open/marsh at the same time.
	-- Jon and I decided the best workaround would be to turn any Sugar/Jungle in to Marsh.
	local iW, iH = Map.GetGridSize()
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local plot = Map.GetPlot(x, y)
			if plot:GetResourceType(-1) == self.sugar_ID then
				local featureType = plot:GetFeatureType();
				if featureType == FeatureTypes.FEATURE_JUNGLE then
					local plotType = plot:GetPlotType()
					if plotType ~= PlotTypes.PLOT_LAND then
						plot:SetPlotType(PlotTypes.PLOT_LAND, false, true)
					end
					plot:SetFeatureType(FeatureTypes.FEATURE_MARSH, -1)
					plot:SetTerrainType(TerrainTypes.TERRAIN_GRASS, false, true)
					--
					--print("-"); print("Fixed a Sugar/Jungle at plot", x, y);
				end
			end
		end
	end
	--]]
end

-- added smart lux option to place luxes in "real" player territories

function AssignStartingPlots:GenerateLuxuryPlotListsInRegion(region_number, smart_lux, smart_plots)
	-- local iW, iH = Map.GetGridSize();
	-- This function groups a region's plots in to lists, for Luxury resource assignment.
	local region_data_table = self.regionData[region_number];
	local iWestX = region_data_table[1];
	local iSouthY = region_data_table[2];
	local iWidth = region_data_table[3];
	local iHeight = region_data_table[4];
	local iAreaID = region_data_table[5];
	local region_area_object;
	if iAreaID ~= -1 then
		region_area_object = Map.GetArea(iAreaID);
	end

	local region_coast, region_marsh, region_flood_plains, region_tundra_flat_including_forests = {}, {}, {}, {};
	local region_hills_open, region_hills_covered, region_hills_jungle, region_hills_forest = {}, {}, {}, {};
	local region_desert_flat_no_feature, region_plains_flat_no_feature, region_jungle_flat = {}, {}, {};
	local region_forest_flat, region_forest_flat_but_not_tundra = {}, {};
	local region_dry_grass_flat_no_feature, region_fresh_water_grass_flat_no_feature = {}, {};
	local region_dry_plains_flat_no_feature, region_fresh_water_plains_flat_no_feature = {}, {};								-- MOD.Barathor: New
	local region_desert_or_tundra_flat_no_feature, region_grass_flat_no_feature = {}, {};										-- MOD.Barathor: New
	local region_tundra_flat_no_feature, region_tundra_flat_forest = {}, {};													-- MOD.Barathor: New
	local region_hills_open_no_tundra, region_hills_open_no_desert, region_hills_open_no_tundra_no_desert = {}, {}, {};			-- MOD.Barathor: New
	local region_hills_open_no_grass, region_hills_open_no_grass_no_tundra, region_hills_covered_no_tundra = {}, {}, {};		-- MOD.Barathor: New
	local region_hills_covered_no_grass, region_hills_covered_no_grass_no_tundra, region_flat_covered = {}, {}, {};				-- MOD.Barathor: New
	local region_flat_covered_no_grass, region_flat_covered_no_tundra, region_flat_covered_no_grass_no_tundra = {}, {}, {};		-- MOD.Barathor: New
	local region_flat_open, region_flat_open_no_grass_no_plains, region_flat_open_no_tundra_no_desert = {}, {}, {};				-- MOD.Barathor: New
	local region_flat_open_no_desert, region_flat_desert_including_flood, region_hills_open_no_grass_no_plains = {}, {}, {};	-- MOD.Barathor: New

	-- Iterate through the region's plots, building the fifteen lists defined above.
	if smart_lux then
		print("USING SMART LUX")
		for plotLoop, plot in ipairs(smart_plots) do
			local x = plot:GetX()
			local y = plot:GetY()
			local plotIndex = y * iW + x + 1;
			-- local plot = Map.GetPlot(x, y);
			local area_of_plot = plot:GetArea();
			-- get plot info
			local plotType = plot:GetPlotType()
			local terrainType = plot:GetTerrainType()
			local featureType = plot:GetFeatureType()
			--
			if plotType == PlotTypes.PLOT_OCEAN then
				if terrainType == TerrainTypes.TERRAIN_COAST then
					if plot:IsLake() == false then
						if featureType ~= self.feature_atoll and featureType ~= FeatureTypes.FEATURE_ICE then
							if iAreaID == -1 then
								if plot:IsAdjacentToLand() then
									table.insert(region_coast, plotIndex);
								end
							else
								if plot:IsAdjacentToArea(region_area_object) then
									table.insert(region_coast, plotIndex);
								end
							end
						end
					end
				end
			elseif plotType == PlotTypes.PLOT_HILLS and terrainType ~= TerrainTypes.TERRAIN_SNOW then
				if featureType == FeatureTypes.NO_FEATURE then
					table.insert(region_hills_open, plotIndex);
					if terrainType == TerrainTypes.TERRAIN_TUNDRA then								-- MOD.Barathor: New Condition
						table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_DESERT then
						table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass_no_tundra, plotIndex);				-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
						table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_grass_no_tundra, plotIndex);				-- MOD.Barathor: New
						table.insert(region_hills_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then
						table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
					end
				elseif featureType == FeatureTypes.FEATURE_JUNGLE then		
					table.insert(region_hills_jungle, plotIndex);
					table.insert(region_hills_covered, plotIndex);
					if terrainType == TerrainTypes.TERRAIN_PLAINS then								-- MOD.Barathor: New Condition
						table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
						table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then
						table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
					end
				elseif featureType == FeatureTypes.FEATURE_FOREST then		
					table.insert(region_hills_forest, plotIndex);
					table.insert(region_hills_covered, plotIndex);
					if terrainType == TerrainTypes.TERRAIN_TUNDRA then
						table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_PLAINS then							-- MOD.Barathor: New Condition
						table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
						table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_hills_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then							-- MOD.Barathor: New Condition
						table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
					end
				end
			elseif plotType == PlotTypes.PLOT_LAND then
				if featureType == FeatureTypes.NO_FEATURE then
					if terrainType == TerrainTypes.TERRAIN_TUNDRA then
						table.insert(region_tundra_flat_no_feature, plotIndex);						-- MOD.Barathor: New
						table.insert(region_desert_or_tundra_flat_no_feature, plotIndex);			-- MOD.Barathor: New
						table.insert(region_tundra_flat_including_forests, plotIndex);
						table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
						table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_DESERT then
						table.insert(region_desert_flat_no_feature, plotIndex);
						table.insert(region_desert_or_tundra_flat_no_feature, plotIndex);			-- MOD.Barathor: New
						table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
						table.insert(region_flat_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
						table.insert(region_flat_desert_including_flood, plotIndex);				-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
						table.insert(region_plains_flat_no_feature, plotIndex);
						table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
						table.insert(region_flat_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
						if plot:IsFreshWater() then													-- MOD.Barathor: New Condition -- fresh water check
							table.insert(region_fresh_water_plains_flat_no_feature, plotIndex);		-- MOD.Barathor: New
						else
							table.insert(region_dry_plains_flat_no_feature, plotIndex);				-- MOD.Barathor: New
						end
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then
						table.insert(region_grass_flat_no_feature, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
						table.insert(region_flat_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
						if plot:IsFreshWater() then
							table.insert(region_fresh_water_grass_flat_no_feature, plotIndex);
						else
							table.insert(region_dry_grass_flat_no_feature, plotIndex);
						end
					end
				elseif featureType == FeatureTypes.FEATURE_MARSH then		
					table.insert(region_marsh, plotIndex);
				elseif featureType == FeatureTypes.FEATURE_FLOOD_PLAINS then		
					table.insert(region_flood_plains, plotIndex);
					table.insert(region_flat_desert_including_flood, plotIndex);					-- MOD.Barathor: New
				elseif featureType == FeatureTypes.FEATURE_JUNGLE then		
					table.insert(region_jungle_flat, plotIndex);
					if terrainType == TerrainTypes.TERRAIN_PLAINS then								-- MOD.Barathor: New Condition
						table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then
						table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
					end
				elseif featureType == FeatureTypes.FEATURE_FOREST then		
					table.insert(region_forest_flat, plotIndex);
					if terrainType == TerrainTypes.TERRAIN_TUNDRA then
						table.insert(region_tundra_flat_including_forests, plotIndex);

						table.insert(region_tundra_flat_forest, plotIndex);							-- MOD.Barathor: New
						table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
						table.insert(region_forest_flat_but_not_tundra, plotIndex);
						table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
						table.insert(region_flat_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
						table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
					elseif terrainType == TerrainTypes.TERRAIN_GRASS then
						table.insert(region_forest_flat_but_not_tundra, plotIndex);
						table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
					end
				end
			end
		end
	else
		for region_loop_y = 0, iHeight - 1 do
			for region_loop_x = 0, iWidth - 1 do
				local x = (region_loop_x + iWestX) % iW;
				local y = (region_loop_y + iSouthY) % iH;
				local plotIndex = y * iW + x + 1;
				local plot = Map.GetPlot(x, y);
				local area_of_plot = plot:GetArea();
				-- get plot info
				local plotType = plot:GetPlotType()
				local terrainType = plot:GetTerrainType()
				local featureType = plot:GetFeatureType()
				--
				if plotType == PlotTypes.PLOT_OCEAN then
					if terrainType == TerrainTypes.TERRAIN_COAST then
						if plot:IsLake() == false then
							if featureType ~= self.feature_atoll and featureType ~= FeatureTypes.FEATURE_ICE then
								if iAreaID == -1 then
									if plot:IsAdjacentToLand() then
										table.insert(region_coast, plotIndex);
									end
								else
									if plot:IsAdjacentToArea(region_area_object) then
										table.insert(region_coast, plotIndex);
									end
								end
							end
						end
					end
				elseif plotType == PlotTypes.PLOT_HILLS and terrainType ~= TerrainTypes.TERRAIN_SNOW then
					if featureType == FeatureTypes.NO_FEATURE then
						table.insert(region_hills_open, plotIndex);
						if terrainType == TerrainTypes.TERRAIN_TUNDRA then								-- MOD.Barathor: New Condition
							table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_DESERT then
							table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass_no_tundra, plotIndex);				-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
							table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_grass_no_tundra, plotIndex);				-- MOD.Barathor: New
							table.insert(region_hills_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then
							table.insert(region_hills_open_no_tundra, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
						end
					elseif featureType == FeatureTypes.FEATURE_JUNGLE then		
						table.insert(region_hills_jungle, plotIndex);
						table.insert(region_hills_covered, plotIndex);
						if terrainType == TerrainTypes.TERRAIN_PLAINS then								-- MOD.Barathor: New Condition
							table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
							table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then
							table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
						end
					elseif featureType == FeatureTypes.FEATURE_FOREST then		
						table.insert(region_hills_forest, plotIndex);
						table.insert(region_hills_covered, plotIndex);
						if terrainType == TerrainTypes.TERRAIN_TUNDRA then
							table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_PLAINS then							-- MOD.Barathor: New Condition
							table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
							table.insert(region_hills_covered_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_hills_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then							-- MOD.Barathor: New Condition
							table.insert(region_hills_covered_no_tundra, plotIndex);					-- MOD.Barathor: New
						end
					end
				elseif plotType == PlotTypes.PLOT_LAND then
					if featureType == FeatureTypes.NO_FEATURE then
						if terrainType == TerrainTypes.TERRAIN_TUNDRA then
							table.insert(region_tundra_flat_no_feature, plotIndex);						-- MOD.Barathor: New
							table.insert(region_desert_or_tundra_flat_no_feature, plotIndex);			-- MOD.Barathor: New
							table.insert(region_tundra_flat_including_forests, plotIndex);
							table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
							table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_DESERT then
							table.insert(region_desert_flat_no_feature, plotIndex);
							table.insert(region_desert_or_tundra_flat_no_feature, plotIndex);			-- MOD.Barathor: New
							table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
							table.insert(region_flat_open_no_grass_no_plains, plotIndex);				-- MOD.Barathor: New
							table.insert(region_flat_desert_including_flood, plotIndex);				-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
							table.insert(region_plains_flat_no_feature, plotIndex);
							table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
							table.insert(region_flat_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
							if plot:IsFreshWater() then													-- MOD.Barathor: New Condition -- fresh water check
								table.insert(region_fresh_water_plains_flat_no_feature, plotIndex);		-- MOD.Barathor: New
							else
								table.insert(region_dry_plains_flat_no_feature, plotIndex);				-- MOD.Barathor: New
							end
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then
							table.insert(region_grass_flat_no_feature, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_open_no_desert, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_open, plotIndex);									-- MOD.Barathor: New
							table.insert(region_flat_open_no_tundra_no_desert, plotIndex);				-- MOD.Barathor: New
							if plot:IsFreshWater() then
								table.insert(region_fresh_water_grass_flat_no_feature, plotIndex);
							else
								table.insert(region_dry_grass_flat_no_feature, plotIndex);
							end
						end
					elseif featureType == FeatureTypes.FEATURE_MARSH then		
						table.insert(region_marsh, plotIndex);
					elseif featureType == FeatureTypes.FEATURE_FLOOD_PLAINS then		
						table.insert(region_flood_plains, plotIndex);
						table.insert(region_flat_desert_including_flood, plotIndex);					-- MOD.Barathor: New
					elseif featureType == FeatureTypes.FEATURE_JUNGLE then		
						table.insert(region_jungle_flat, plotIndex);
						if terrainType == TerrainTypes.TERRAIN_PLAINS then								-- MOD.Barathor: New Condition
							table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then
							table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
						end
					elseif featureType == FeatureTypes.FEATURE_FOREST then		
						table.insert(region_forest_flat, plotIndex);
						if terrainType == TerrainTypes.TERRAIN_TUNDRA then
							table.insert(region_tundra_flat_including_forests, plotIndex);

							table.insert(region_tundra_flat_forest, plotIndex);							-- MOD.Barathor: New
							table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_PLAINS then
							table.insert(region_forest_flat_but_not_tundra, plotIndex);
							table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
							table.insert(region_flat_covered_no_grass_no_tundra, plotIndex);			-- MOD.Barathor: New
							table.insert(region_flat_covered_no_grass, plotIndex);						-- MOD.Barathor: New
						elseif terrainType == TerrainTypes.TERRAIN_GRASS then
							table.insert(region_forest_flat_but_not_tundra, plotIndex);
							table.insert(region_flat_covered_no_tundra, plotIndex);						-- MOD.Barathor: New
						end
					end
				end
			end
		end
	end
	
	-- Luxury Plot Lists
	local results_table = {
	region_coast, -- (Coast next to land)		-- 1
	region_marsh,								-- 2
	region_flood_plains,						-- 3
	region_hills_open,							-- 4
	region_hills_covered,						-- 5
	region_hills_jungle,						-- 6
	region_hills_forest,						-- 7
	region_jungle_flat,							-- 8
	region_forest_flat,							-- 9
	region_desert_flat_no_feature,				-- 10
	region_plains_flat_no_feature,				-- 11			
	region_dry_grass_flat_no_feature,			-- 12
	region_fresh_water_grass_flat_no_feature,	-- 13
	region_tundra_flat_including_forests,		-- 14
	region_forest_flat_but_not_tundra,			-- 15
	region_grass_flat_no_feature,				-- 16	-- MOD.Barathor: New
	region_tundra_flat_no_feature,				-- 17	-- MOD.Barathor: New
	region_dry_plains_flat_no_feature,			-- 18	-- MOD.Barathor: New
	region_fresh_water_plains_flat_no_feature,	-- 19	-- MOD.Barathor: New
	region_desert_or_tundra_flat_no_feature,	-- 20	-- MOD.Barathor: New
	region_tundra_flat_forest,					-- 21	-- MOD.Barathor: New
	region_hills_open_no_tundra,				-- 22	-- MOD.Barathor: New
	region_hills_open_no_desert,				-- 23	-- MOD.Barathor: New
	region_hills_open_no_tundra_no_desert,		-- 24	-- MOD.Barathor: New
	region_hills_open_no_grass,					-- 25	-- MOD.Barathor: New
	region_hills_open_no_grass_no_tundra,		-- 26   -- MOD.Barathor: New
	region_hills_open_no_grass_no_plains,		-- 27   -- MOD.Barathor: New
	region_hills_covered_no_tundra,				-- 28	-- MOD.Barathor: New
	region_hills_covered_no_grass,				-- 29	-- MOD.Barathor: New
	region_hills_covered_no_grass_no_tundra,	-- 30	-- MOD.Barathor: New
	region_flat_covered,						-- 31	-- MOD.Barathor: New
	region_flat_covered_no_grass,				-- 32	-- MOD.Barathor: New
	region_flat_covered_no_tundra,				-- 33	-- MOD.Barathor: New
	region_flat_covered_no_grass_no_tundra,		-- 34	-- MOD.Barathor: New
	region_flat_open,							-- 35	-- MOD.Barathor: New
	region_flat_open_no_grass_no_plains,		-- 36	-- MOD.Barathor: New
	region_flat_open_no_tundra_no_desert,		-- 37	-- MOD.Barathor: New
	region_flat_open_no_desert,					-- 38	-- MOD.Barathor: New
	region_flat_desert_including_flood,			-- 39	-- MOD.Barathor: New
	};
	return results_table
end

-- added smart lux functionality to place luxes in players "real" territory
-- added option to increase secondary copies for SP

function AssignStartingPlots:PlaceLuxuries()

	-- local beta_double_secondary = Map.GetCustomOption(39);
	local beta_double_secondary = 1
	-- local beta_smart_lux = Map.GetCustomOption(46);
	local beta_smart_lux = 2

	-- This function is dependent upon AssignLuxuryRoles() and PlaceCityStates() having been executed first.
	-- local iW, iH = Map.GetGridSize();
	-- Place Luxuries at civ start locations.
	local used_randoms_as_secondaries =	table.fill(false, 99);

	for loop, reg_data in ipairs(self.regions_sorted_by_type) do
		local region_number = reg_data[1];
		local this_region_luxury = reg_data[2];
		local x = self.startingPlots[region_number][1];
		local y = self.startingPlots[region_number][2];
		print("-"); print("Attempting to place Luxury#", this_region_luxury, "at start plot", x, y, "in Region#", region_number);
		-- Determine number to place at the start location
		local iNumToPlace = 2;	-- MOD.Barathor: Updated -- original = 1 -- Most times, 2 of the initial type are placed at the start anyway, because of the old fertility checks below.  This will make it consistent.
		if self.start_locations == 1 or self.start_locations == 2 then -- Legendary Start
			iNumToPlace = 3;	-- MOD.Barathor: Updated -- original = 2
		end
		-- MOD.Barathor: Disabled -- These aren't very useful and almost always trigger.  Plus, there's better ways to increase habitability than to assign more luxuries (which is just more gold, in most cases).
		--[[
		if self.regionData[region_number][8] < 2.5 then -- Low fertility per region rectangle plot, add a lux.
			print("-"); print("Region#", region_number, "has low rectangle fertility, giving it an extra Luxury at start plot.");
			iNumToPlace = iNumToPlace + 1;
			self.luxury_low_fert_compensation[this_region_luxury] = self.luxury_low_fert_compensation[this_region_luxury] + 1;
			self.region_low_fert_compensation[region_number] = self.region_low_fert_compensation[region_number] + 1;
		end
		if self.regionData[region_number][6] / self.regionTerrainCounts[region_number][2] < 4 then -- Low fertility per land plot.
			print("-"); print("Region#", region_number, "has low per-plot fertility, giving it an extra Luxury at start plot.");
			iNumToPlace = iNumToPlace + 1;
			self.luxury_low_fert_compensation[this_region_luxury] = self.luxury_low_fert_compensation[this_region_luxury] + 1;
			self.region_low_fert_compensation[region_number] = self.region_low_fert_compensation[region_number] + 1;
		end
		]]--
		-- Obtain plot lists appropriate to this luxury type.
		local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, shuf_list;					-- MOD.Barathor: New -- added a quinary and senary list
		primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(this_region_luxury);	-- MOD.Barathor: New -- added a quinary and senary list

		-- First pass, checking only first two rings with a 50% ratio.
		luxury_plot_lists = self:GenerateLuxuryPlotListsAtCitySite(x, y, 2, false)
		shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
		local iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumToPlace, 0.5, -1, 0, 0, shuf_list);
		if iNumLeftToPlace > 0 and secondary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 0.5, -1, 0, 0, shuf_list);
		end
		if iNumLeftToPlace > 0 and tertiary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 0.5, -1, 0, 0, shuf_list);
		end
		if iNumLeftToPlace > 0 and quaternary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 0.5, -1, 0, 0, shuf_list);
		end
		if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 0.5, -1, 0, 0, shuf_list);
		end
		if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 0.5, -1, 0, 0, shuf_list);
		end

		if iNumLeftToPlace > 0 then
			-- Second pass, checking three rings with a 100% ratio.
			luxury_plot_lists = self:GenerateLuxuryPlotListsAtCitySite(x, y, 3, false)
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			if iNumLeftToPlace > 0 and secondary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			end
			if iNumLeftToPlace > 0 and tertiary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			end
			if iNumLeftToPlace > 0 and quaternary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			end
			if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			end
			if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(this_region_luxury, 1, iNumLeftToPlace, 1, -1, 0, 0, shuf_list);
			end
		end

		if iNumLeftToPlace > 0 then
			-- If we haven't been able to place all of this lux type at the start, it CAN be placed
			-- in the region somewhere. Subtract remainder from this region's compensation, so that the
			-- regional process, later, will attempt to place this remainder somewhere in the region.
			self.luxury_low_fert_compensation[this_region_luxury] = self.luxury_low_fert_compensation[this_region_luxury] - iNumLeftToPlace;
			self.region_low_fert_compensation[region_number] = self.region_low_fert_compensation[region_number] - iNumLeftToPlace;
		end

		if iNumLeftToPlace > 0 and self.iNumTypesRandom > 0 then
			-- We'll attempt to place one source of a Luxury type assigned to random distribution.
			local randoms_to_place = 1;
			for loop, random_res in ipairs(self.resourceIDs_assigned_to_random) do

				primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(random_res);	-- MOD.Barathor: New -- added a quinary and senary list
				if randoms_to_place > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if randoms_to_place > 0 and secondary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if randoms_to_place > 0 and tertiary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if randoms_to_place > 0 and quaternary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if randoms_to_place > 0 and quinary > 0 then	-- MOD.Barathor: New -- added a quinary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if randoms_to_place > 0 and senary > 0 then	-- MOD.Barathor: New -- added a senary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
					randoms_to_place = self:PlaceSpecificNumberOfResources(random_res, 1, 1, 1, -1, 0, 0, shuf_list);
				end
			end
		end
	end
	
	-- Place Luxuries at City States.
	-- Candidates include luxuries exclusive to CS, the lux assigned to this CS's region (if in a region), and the randoms.
	for city_state = 1, self.iNumCityStates do
		-- First check to see if this city state number received a valid start plot.
		if self.city_state_validity_table[city_state] == false then
			-- This one did not! It does not exist on the map nor have valid data, so we will ignore it.
		else
			
			-- OK, it's a valid city state. Process it.
			local region_number = self.city_state_region_assignments[city_state];
			local x = self.cityStatePlots[city_state][1];
			local y = self.cityStatePlots[city_state][2];
			local allowed_luxuries = self:GetListOfAllowableLuxuriesAtCitySite(x, y, 2)
			local lux_possible_for_cs = {}; -- Recorded with ID as key, weighting as data entry
			-- Identify Allowable Luxuries assigned to City States.
			-- If any CS-Only types are eligible, then all combined will have a weighting of 80%
			local cs_only_types = {};
			for loop, res_ID in ipairs(self.resourceIDs_assigned_to_cs) do
				if allowed_luxuries[res_ID] == true then
					table.insert(cs_only_types, res_ID);
				end
			end
			local iNumCSAllowed = table.maxn(cs_only_types);
			if iNumCSAllowed > 0 then
				for loop, res_ID in ipairs(cs_only_types) do
					lux_possible_for_cs[res_ID] = 80 / iNumCSAllowed;
				end
			end
			-- Identify Allowable Random Luxuries and the Regional Luxury if any.
			-- If any random types are eligible (plus the regional type if in a region) these combined carry a 20% weighting.
			if self.iNumTypesRandom > 0 or region_number > 0 then
				local random_types_allowed = {};
				for loop, res_ID in ipairs(self.resourceIDs_assigned_to_random) do
					if allowed_luxuries[res_ID] == true then
						table.insert(random_types_allowed, res_ID);
					end
				end
				local iNumRandAllowed = table.maxn(random_types_allowed);
				local iNumAllowed = iNumRandAllowed;
				--[[
				if region_number > 0 then
					iNumAllowed = iNumAllowed + 1; -- Adding the region type in to the mix with the random types.
					local res_ID = self.region_luxury_assignment[region_number];
					if allowed_luxuries[res_ID] == true then
						lux_possible_for_cs[res_ID] = 25 / iNumAllowed;
					end
				end
				--]]

				if iNumRandAllowed > 0 then
					for loop, res_ID in ipairs(random_types_allowed) do
						lux_possible_for_cs[res_ID] = 20 / iNumAllowed;
					end
				end
			end

			-- If there are no allowable luxury types at this city site, then this city state gets none.
			local iNumAvailableTypes = table.maxn(lux_possible_for_cs);
			if iNumAvailableTypes == 0 then
				print("City State #", city_state, "has poor land, ineligible to receive a Luxury resource.");
			else
				-- Calculate probability thresholds for each allowable luxury type.
				local res_threshold = {};
				local totalWeight, accumulatedWeight = 0, 0;
				for res_ID, this_weight in pairs(lux_possible_for_cs) do
					totalWeight = totalWeight + this_weight;
				end

				-- Choose luxury type.
				local use_this_ID;
				local diceroll = Map.Rand(10000, "Choose resource type - Assign Luxury To City State - Lua");

				for res_ID, this_weight in pairs(lux_possible_for_cs) do
					local threshold = (this_weight + accumulatedWeight) * 10000 / totalWeight;
					if diceroll < threshold then
						use_this_ID = res_ID;
						print("CS Given Lux ID: " .. tostring(use_this_ID));
						break
					end
					accumulatedWeight = accumulatedWeight + this_weight;
				end

				print("-"); print("-"); print("-Assigned Luxury Type", use_this_ID, "to City State#", city_state);
				-- Place luxury.
				local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, shuf_list;			-- MOD.Barathor: New -- added a quinary and senary list
 				primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(use_this_ID);	-- MOD.Barathor: New -- added a quinary and senary list
				luxury_plot_lists = self:GenerateLuxuryPlotListsAtCitySite(x, y, 2, false)
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
				local iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				if iNumLeftToPlace > 0 and secondary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and tertiary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quaternary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, 1, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace == 0 then
					print("-"); print("Placed Luxury ID#", use_this_ID, "at City State#", city_state, "in Region#", region_number, "located at Plot", x, y);
				end
			end
		end
	end

	local smart_region_plots = {}
	local region_number_to_loop = {}

	if beta_smart_lux == 2 then
		local player_plots = {}
		local player_is_coastal = {}

		for loop, reg_data in ipairs(self.regions_sorted_by_type) do
			local region_number = reg_data[1];
			region_number_to_loop[region_number] = loop;
			local this_region_luxury = reg_data[2];
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			local player_plot = Map.GetPlot(x,y)
			table.insert(player_plots, player_plot)
			player_is_coastal[loop] = player_plot:IsCoastalLand(50)
		end

		local cs_plots = {}
		for cs_number = 1, self.iNumCityStates do
			if self.city_state_validity_table[cs_number] == true then
				local data_table = self.cityStatePlots[cs_number];
				local x = data_table[1];
				local y = data_table[2];
				table.insert(cs_plots, Map.GetPlot(x, y))
			end
		end

		local land_graph, total_flow_graph = CalculateFlows(player_plots, 40, true, cs_plots, 4, 0.5)

		-- dump_map(total_flow_graph)
		-- dump_map(land_graph)

		local smart_region_plots_temp = {}
		for loop, reg_data in ipairs(self.regions_sorted_by_type) do
			smart_region_plots[loop] = {}
			smart_region_plots_temp[loop] = {}
		end

		for i, region_num in pairs(land_graph) do
			if region_num ~= 0 then
				if player_is_coastal[region_num] then
					-- local x, y = i_to_xy(i, iW, iH)
					local adj_is = adj_is_cache[i]
					for loop, adj_i in ipairs(adj_is) do
						local adj_x, adj_y = i_to_xy(adj_i, iW, iH)
						local adj_plot = Map.GetPlot(adj_x, adj_y)
						if adj_plot ~= nil then
							if (adj_plot:IsLake() == false) and (adj_plot:GetPlotType() == PlotTypes.PLOT_OCEAN) then
								smart_region_plots_temp[region_num][adj_i] = 1
							end
						end	
					end
				else
					smart_region_plots_temp[region_num][i] = 1
				end
			end
			
		end

		for loop, region in pairs(smart_region_plots_temp) do
			for i, dumby in pairs(region) do
				local x, y = i_to_xy(i, iW, iH)
				table.insert(smart_region_plots[loop], Map.GetPlot(x, y))
			end
		end
		-- dump_map(land_graph)
	end
		
	-- Place Regional Luxuries
	for region_number, res_ID in ipairs(self.region_luxury_assignment) do
		print("-"); print("- - -"); print("Attempting to place regional luxury #", res_ID, "in Region#", region_number);
		local iNumAlreadyPlaced = self.amounts_of_resources_placed[res_ID + 1];
		local assignment_split = self.luxury_assignment_count[res_ID];
		local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, shuf_list, iNumLeftToPlace;		-- MOD.Barathor: New -- added a quinary and senary list
		primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(res_ID);
		if beta_smart_lux == 1 then				-- MOD.Barathor: New -- added a quinary and senary list
			luxury_plot_lists = self:GenerateLuxuryPlotListsInRegion(region_number, false, {})
		else
			if tablelength(smart_region_plots[region_number_to_loop[region_number]]) > 6 then
				luxury_plot_lists = self:GenerateLuxuryPlotListsInRegion(region_number, true, smart_region_plots[region_number_to_loop[region_number]])
			--fallback incase not enough plots
			else
				luxury_plot_lists = self:GenerateLuxuryPlotListsInRegion(region_number, false, {})
			end
		end

		-- Calibrate number of luxuries per region to world size and number of civs
		-- present. The amount of lux per region should be at its highest when the 
		-- number of civs in the game is closest to "default" for that map size.
		local target_list = self:GetRegionLuxuryTargetNumbers()
		local targetNum = target_list[self.iNumCivs] 		-- MOD.Barathor: Updated -- Keep it simple and consistent.  Plus, fertility compensation above is disabled anyway.
		-- local targetNum = math.floor((target_list[self.iNumCivs] + (0.5 * self.luxury_low_fert_compensation[res_ID])) / assignment_split);	-- MOD.Barathor: Disabled
		targetNum = targetNum - self.region_low_fert_compensation[region_number];
		-- Adjust target number according to Resource Setting.
		if self.resource_setting == 1 or self.resource_setting == 2 then --sparse
			targetNum = targetNum - 2;
		elseif self.resource_setting == 3 or self.resource_setting == 4 or self.resource_setting == 5 or self.resource_setting == 6 then --mediocre
			targetNum = targetNum - 1;
		elseif self.resource_setting == 7 then --plenty
			targetNum = targetNum + 1;
		elseif self.resource_setting == 8 or self.resource_setting == 9 or self.resource_setting == 10 then --abundant
			targetNum = targetNum + 2;
		end
		local iNumThisLuxToPlace = math.max(1, targetNum); -- Always place at least one.

		print("-"); print("Target number for Luxury#", res_ID, "with assignment split of", assignment_split, "is", targetNum);
		
		-- Place luxuries.
		shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
		iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumThisLuxToPlace, 0.25, 2, 1, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.3, min radius = 0, max radius = 3
		if iNumLeftToPlace > 0 and secondary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.25, 2, 1, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.3, min radius = 0, max radius = 3
		end
		if iNumLeftToPlace > 0 and tertiary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.25, 2, 1, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.4, min radius = 0, max radius = 2
		end
		if iNumLeftToPlace > 0 and quaternary > 0 then
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.25, 2, 0, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.5, min radius = 0, max radius = 2 
		end
		if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.25, 2, 1, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.5, min radius = 0, max radius = 2 
		end
		if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.25, 2, 1, 3, shuf_list);	-- MOD.Barathor: Updated -- Existing ratio = 0.5, min radius = 0, max radius = 2 
		end
		print("-"); print("-"); print("Number of LuxuryID", res_ID, "left to place in Region#", region_number, "is", iNumLeftToPlace);
		
		-- MOD.Barathor: New -- New Fallback loop since some luxuries frequently miss the target total during regionl placement (like silk, dyes, spices, etc.)
					-- This is because some luxuries have only a few indices and don't utilize all four, plus they have strict requirements, like foliage only and on flat land only
					-- When a luxury only uses two or three selective indices, it gets harder to place them all since the first default ratios above are only 0.3 (3 placements for every 10 eligible plots, rounded up)
		if iNumLeftToPlace > 0 then	
			-- Second pass, checking all with a 100% ratio to make sure the target total is reached for this region!
			shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			if iNumLeftToPlace > 0 and secondary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			end
			if iNumLeftToPlace > 0 and tertiary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			end
			if iNumLeftToPlace > 0 and quaternary > 0 then
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			end
			if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			end
			if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 1, 2, 1, 3, shuf_list);
			end
			print("-"); print("Number of LuxuryID", res_ID, "not placed in Region#", region_number, "is", iNumLeftToPlace);
		end	
	end

	-- Place Random Luxuries
	if self.iNumTypesRandom > 0 then
		print("* *"); print("* iNumTypesRandom = ", self.iNumTypesRandom); print("* *");
		-- This table governs targets for total number of luxuries placed in the world, not
		-- including the "extra types" of Luxuries placed at start locations. These targets
		-- are approximate. An additional random factor is added in based on number of civs.
		-- Any difference between regional and city state luxuries placed, and the target, is
		-- made up for with the number of randomly placed luxuries that get distributed.
		local world_size_data = self:GetWorldLuxuryTargetNumbers()
		local targetLuxForThisWorldSize = world_size_data[1];
		local loopTarget = world_size_data[2];
		local extraLux = Map.Rand(self.iNumCivs, "Luxury Resource Variance - Place Resources LUA");
		local iNumRandomLuxTarget = targetLuxForThisWorldSize + extraLux - self.totalLuxPlacedSoFar;
		print("* *"); print("* targetLuxForThisWorldSize = ", targetLuxForThisWorldSize); print("* *");	-- MOD.Barathor: Test
		print("* *"); print("* random to add to target = ", extraLux); print("* *");					-- MOD.Barathor: Test
		print("* *"); print("* totalLuxPlacedSoFar = ", self.totalLuxPlacedSoFar); print("* *");		-- MOD.Barathor: Test
		print("* *"); print("* iNumRandomLuxTarget = ", iNumRandomLuxTarget); print("* *");				-- MOD.Barathor: Test
		local iNumRandomLuxPlaced, iNumThisLuxToPlace = 0, 0;
		-- This table weights the amount of random luxuries to place, with first-selected getting heavier weighting.
		local random_lux_ratios_table = {
		{1},
		{1, 1},
		{1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1} };

		for loop, res_ID in ipairs(self.resourceIDs_assigned_to_random) do
			local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, current_list, iNumLeftToPlace;	-- MOD.Barathor: New -- added a quinary and senary list
			primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(res_ID);					-- MOD.Barathor: New -- added a quinary and senary list
			--if self.iNumTypesRandom > 8 then
				-- local iW, iH = Map.GetGridSize();
				local LandXY = iW * iH;
				local NumRandToAdd = 4;

				if LandXY < 2500 then
					NumRandToAdd = 4;
				elseif LandXY < 6000 then
					NumRandToAdd = 5;
				elseif LandXY < 10000 then
					NumRandToAdd = 6;
				end

				iNumThisLuxToPlace = math.max(NumRandToAdd, math.ceil(iNumRandomLuxTarget / 10));
			--else
			--	local lux_minimum = math.max(3, loopTarget - loop);
			--	local lux_share_of_remaining = math.ceil(iNumRandomLuxTarget * random_lux_ratios_table[self.iNumTypesRandom][loop]);
			--	iNumThisLuxToPlace = math.max(lux_minimum, lux_share_of_remaining);
			--end

			local lux_distance = 3;

			-- Place this luxury type.
			current_list = self.global_luxury_plot_lists[primary];
			iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumThisLuxToPlace, 0.25, 2, lux_distance, 0, current_list);
			if iNumLeftToPlace > 0 and secondary > 0 then
				current_list = self.global_luxury_plot_lists[secondary];
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.3, 2, lux_distance, 0, current_list);
			end
			if iNumLeftToPlace > 0 and tertiary > 0 then
				current_list = self.global_luxury_plot_lists[tertiary];
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.35, 2, lux_distance, 0, current_list);
			end
			if iNumLeftToPlace > 0 and quaternary > 0 then
				current_list = self.global_luxury_plot_lists[quaternary];
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.4, 2, lux_distance, 0, current_list);
			end
			if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
				current_list = self.global_luxury_plot_lists[quinary];
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.5, 2, lux_distance, 0, current_list);
			end
			if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
				current_list = self.global_luxury_plot_lists[senary];
				iNumLeftToPlace = self:PlaceSpecificNumberOfResources(res_ID, 1, iNumLeftToPlace, 0.75, 2, lux_distance, 0, current_list);
			end
			iNumRandomLuxPlaced = iNumRandomLuxPlaced + iNumThisLuxToPlace - iNumLeftToPlace;
			print("-"); 
			print("Random Luxury ID#:", res_ID);	-- MOD.Barathor: Test
			print("-"); print("Random Luxury Target Number:", iNumThisLuxToPlace);
			print("Random Luxury Target Placed:", iNumThisLuxToPlace - iNumLeftToPlace); print("-");
		end
		print("-"); print("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
		print("+ Random Luxuries Target Number:", iNumRandomLuxTarget);
		print("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
		print("+ Random Luxuries Number Placed:", iNumRandomLuxPlaced);
		print("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"); print("-");
	end

	-- For Resource settings other than Sparse, add a second luxury type at start locations.
	-- This second type will be selected from Random types if possible, CS types if necessary, and other regions' types as a final fallback.
	-- Marble is included in the types possible to be placed.
	local placed2ndLux = true;

	if self.resource_setting ~= 1 then

		print("|||||||||||||||||||||||||||||||||||| Secondary Lux Check ||||||||||||||||||||||||||||||||||||");

		local coastal_rotation = 1;
		for region_number = 1, self.iNumCivs do
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];

			local cplot = Map.GetPlot(x, y)

			local use_this_ID;
			local candidate_types, iNumTypesAllowed = {}, 0;
			local allowed_luxuries = self:GetListOfAllowableLuxuriesAtCitySite(x, y, 2)
			print("-"); print("--- Eligible Types List for Second Luxury in Region#", region_number, "---");
			-- See if any Random types are eligible.
			for loop, res_ID in ipairs(self.resourceIDs_assigned_to_random) do
				if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
					print("- Found eligible luxury type:", res_ID);
					iNumTypesAllowed = iNumTypesAllowed + 1;
					table.insert(candidate_types, res_ID);
				end
			end
			-- Check to see if any Special Case luxuries are eligible. Disallow if Strategic Balance resource setting.
			if (self.start_locations ~= 1) and (self.start_locations ~= 2) and (self.start_locations ~= 3) then
				for loop, res_ID in ipairs(self.resourceIDs_assigned_to_special_case) do
					if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
						print("- Found eligible luxury type:", res_ID);
						iNumTypesAllowed = iNumTypesAllowed + 1;
						table.insert(candidate_types, res_ID);
					end
				end
			end

			-- MOD sapht, force coastals to have 1 fishy lux
			if self._lek_coastal_refish and Map.GetPlot(x, y):IsCoastalLand() then
				iNumTypesAllowed = 0;
				candidate_types = {};
				table.insert(candidate_types, self.whale_ID);
				table.insert(candidate_types, self.pearls_ID);
				table.insert(candidate_types, self.crab_ID);
				table.insert(candidate_types, self.coral_ID);

				local force_id = coastal_rotation % 4
				coastal_rotation = coastal_rotation + 1
				use_this_ID = candidate_types[force_id];

				print("sapht: forcing a coastal lux rot/id = ", force_id, use_this_ID)
				local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");

				iNumTypesAllowed = iNumTypesAllowed + 1;
				-- local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
				-- use_this_ID = candidate_types[diceroll];
			elseif iNumTypesAllowed > 0 then
				local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
				print("sapht: rolling lux dice")
				use_this_ID = candidate_types[diceroll];
			else
				-- See if any City State types are eligible.
				for loop, res_ID in ipairs(self.resourceIDs_assigned_to_cs) do
					if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
						print("- Found eligible luxury type:", res_ID);
						iNumTypesAllowed = iNumTypesAllowed + 1;
						table.insert(candidate_types, res_ID);
					end
				end
				if iNumTypesAllowed > 0 then
					local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
					use_this_ID = candidate_types[diceroll];
				else
					-- See if anybody else's regional type is eligible.
					local region_lux_ID = self.region_luxury_assignment[region_number];
					for loop, res_ID in ipairs(self.resourceIDs_assigned_to_regions) do
						if res_ID ~= region_lux_ID then
							if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
								print("- Found eligible luxury type:", res_ID);
								iNumTypesAllowed = iNumTypesAllowed + 1;
								table.insert(candidate_types, res_ID);
							end
						end
					end
					if iNumTypesAllowed > 0 then
						local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
						use_this_ID = candidate_types[diceroll];
					else
						print("-"); print("Failed to place second Luxury type in 2nd ring at start in Region#", region_number, "-- no eligible types!"); print("-");
						placed2ndLux = false;
					end
				end
			end
			print("--- End of Eligible Types list for Second Luxury in Region#", region_number, "---");
			print("Random Res 2 Rings: " .. tostring(use_this_ID));

			if use_this_ID ~= nil then -- Place this luxury type at this start.
				local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, shuf_list;			-- MOD.Barathor: New -- added a quinary and senary list
				primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(use_this_ID);	-- MOD.Barathor: New -- added a quinary and senary list
				luxury_plot_lists = self:GenerateLuxuryPlotListsAtCitySite(x, y, 2, false)
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
				local iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				if iNumLeftToPlace > 0 and secondary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and tertiary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quaternary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace == 0 then
					print("-"); print("Placed Second Luxury type of ID#", use_this_ID, "for start located at Plot", x, y, " in Region#", region_number);
					used_randoms_as_secondaries[use_this_ID] = true;
					print("Random Res State: " .. tostring(used_randoms_as_secondaries[use_this_ID]));
				end
			end
		end
	end

	-- if we failed to place a 2nd lux within the first 2 rings extend the possible locations to the 3rd ring
	if placed2ndLux == false then
		
		local placed2ndLux = true;

		for region_number = 1, self.iNumCivs do
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			local use_this_ID;
			local candidate_types, iNumTypesAllowed = {}, 0;
			local allowed_luxuries = self:GetListOfAllowableLuxuriesAtCitySite(x, y, 3)
			print("-"); print("--- Eligible Types List for Second Luxury in Region#", region_number, "---");
			-- See if any Random types are eligible.
			for loop, res_ID in ipairs(self.resourceIDs_assigned_to_random) do
				if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
					print("- Found eligible luxury type:", res_ID);
					iNumTypesAllowed = iNumTypesAllowed + 1;
					table.insert(candidate_types, res_ID);
				end
			end
			-- Check to see if any Special Case luxuries are eligible. Disallow if Strategic Balance resource setting.
			if (self.start_locations ~= 1) and (self.start_locations ~= 2) and (self.start_locations ~= 3) then
				for loop, res_ID in ipairs(self.resourceIDs_assigned_to_special_case) do
					if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
						print("- Found eligible luxury type:", res_ID);
						iNumTypesAllowed = iNumTypesAllowed + 1;
						table.insert(candidate_types, res_ID);
					end
				end
			end

			-- MOD sapht, force coastals to have 1 fishy lux
			if self._lek_coastal_refish and Map.GetPlot(x, y):IsCoastalLand() then
				iNumTypesAllowed = 0;
				candidate_types = {};
				table.insert(candidate_types, self.whale_ID);
				table.insert(candidate_types, self.pearls_ID);
				table.insert(candidate_types, self.crab_ID);
				table.insert(candidate_types, self.coral_ID);
				iNumTypesAllowed = iNumTypesAllowed + 4;
				print("sapht: forcing a coastal lux (2)")
				-- local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
				-- use_this_ID = candidate_types[diceroll];
			end
		
			if iNumTypesAllowed > 0 then
				local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
				use_this_ID = candidate_types[diceroll];
			else
				-- See if any City State types are eligible.
				for loop, res_ID in ipairs(self.resourceIDs_assigned_to_cs) do
					if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
						print("- Found eligible luxury type:", res_ID);
						iNumTypesAllowed = iNumTypesAllowed + 1;
						table.insert(candidate_types, res_ID);
					end
				end
				if iNumTypesAllowed > 0 then
					local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
					use_this_ID = candidate_types[diceroll];
				else
					-- See if anybody else's regional type is eligible.
					local region_lux_ID = self.region_luxury_assignment[region_number];
					for loop, res_ID in ipairs(self.resourceIDs_assigned_to_regions) do
						if res_ID ~= region_lux_ID then
							if allowed_luxuries[res_ID] == true and used_randoms_as_secondaries[res_ID] == false then
								print("- Found eligible luxury type:", res_ID);
								iNumTypesAllowed = iNumTypesAllowed + 1;
								table.insert(candidate_types, res_ID);
							end
						end
					end
					if iNumTypesAllowed > 0 then
						local diceroll = 1 + Map.Rand(iNumTypesAllowed, "Choosing second luxury type at a start location - LUA");
						use_this_ID = candidate_types[diceroll];
					else
						print("-"); print("Failed to place second Luxury type in 3rd ring at start in Region#", region_number, "-- no eligible types!"); print("-");
						placed2ndLux = false;
					end
				end
			end
			print("--- End of Eligible Types list for Second Luxury in Region#", region_number, "---");

			print("Random Res 3 Rings: " .. tostring(use_this_ID));

			if use_this_ID ~= nil then -- Place this luxury type at this start.
				local primary, secondary, tertiary, quaternary, quinary, senary, luxury_plot_lists, shuf_list;			-- MOD.Barathor: New -- added a quinary and senary list
				primary, secondary, tertiary, quaternary, quinary, senary = self:GetIndicesForLuxuryType(use_this_ID);	-- MOD.Barathor: New -- added a quinary and senary list
				luxury_plot_lists = self:GenerateLuxuryPlotListsAtCitySite(x, y, 3, false)
				shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[primary])
				local iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				if iNumLeftToPlace > 0 and secondary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[secondary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and tertiary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[tertiary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quaternary > 0 then
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quaternary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and quinary > 0 then		-- MOD.Barathor: New -- added a quinary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[quinary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace > 0 and senary > 0 then		-- MOD.Barathor: New -- added a senary list
					shuf_list = GetShuffledCopyOfTable(luxury_plot_lists[senary])
					iNumLeftToPlace = self:PlaceSpecificNumberOfResources(use_this_ID, beta_double_secondary, 1, 1, -1, 0, 0, shuf_list);
				end
				if iNumLeftToPlace == 0 then
					print("-"); print("Placed Second Luxury type of ID#", use_this_ID, "for start located at Plot", x, y, " in Region#", region_number);
					used_randoms_as_secondaries[use_this_ID] = true;
					print("Random Res State: " .. tostring(used_randoms_as_secondaries[random_res]));
				end
			end
		end
	end

--	self:PlaceMarble()
	self.realtotalLuxPlacedSoFar = self.totalLuxPlacedSoFar		-- MOD.Barathor: New -- save the real total of luxuries before it gets corrupted with non-luxury additions which use the luxury placement method
end


-- a bunch of search map search utility functions 

function GraphBFS(current_list, node_list, graph, depth)
	local next_list = {}
	-- print("SAMPLE DUMP")
	for node, dumby in pairs(current_list) do
		if node_list[node] == nil then
			node_list[node] = depth
			for new_node, dumby in pairs(graph[node]) do
				next_list[new_node] = true
			end
		end
	end
	-- print(dump(next_list))
	if tablelength(next_list) > 0 then
		GraphBFS(next_list, node_list, graph, depth+1)
	end
end

function GraphSuspendedBFS(current_list, node_list, graph, depth, suspend_val)
	local next_list = {}
	-- print("SAMPLE DUMP")
	for node, dumby in pairs(current_list) do
		if node ~= suspend_val then
			if node_list[node] == nil then
				node_list[node] = depth
				for new_node, dumby in pairs(graph[node]) do
					if new_node ~= suspend_val then
						next_list[new_node] = true
					end
				end
			end
		end
	end
	-- print(dump(next_list))
	if tablelength(next_list) > 0 then
		GraphSuspendedBFS(next_list, node_list, graph, depth+1, suspend_val)
	end
end


function PlotBFS(i_list, plot_list, comp_list, comp_val, depth)
	local next_is = {}
	for i, dumby in pairs(i_list) do
		if plot_list[i] == nil then
			plot_list[i] = depth
			local adj_is = adj_is_cache[i]
			for loop2, adj_i in ipairs(adj_is) do
				if comp_list[adj_i] ~= nil then
					if comp_list[adj_i] == comp_val then
						next_is[adj_i] = true
					end
				end
			end
		end
	end
	if tablelength(next_is) > 0 then
		 PlotBFS(next_is, plot_list, comp_list, comp_val, depth+1)
	end
end

function PlotDFS(i, plot_list, comp_list, comp_val)
	-- local iW, iH = Map.GetGridSize();
	if plot_list[i] ~= nil then
		return
	end
	plot_list[i] = 1
	local adj_is = adj_is_cache[i]
	for loop2, adj_i in ipairs(adj_is) do
		if comp_list[adj_i] ~= nil then
			if comp_list[adj_i] == comp_val then
				PlotDFS(adj_i, plot_list, comp_list, comp_val)
			end
		end
	end
end

-- function used to determine "real" land of players at various point in the script
function CalculateFlows(test_plots, pulse_depth, use_CS, CS_plots, CS_mult, impass_mult)

	-- local time = os.clock()
	-- local iW, iH = Map.GetGridSize();	
	local cs_resistance = table.fill(0.0, iW * iH);
	if use_CS then
		for cs_number = 1, tablelength(CS_plots) do
			local x = CS_plots[cs_number]:GetX()
			local y = CS_plots[cs_number]:GetY()
			PlaceImpactAndRipplesGeneric(x, y, cs_resistance, 1, 4*CS_mult)
			PlaceImpactAndRipplesGeneric(x, y, cs_resistance, 2, 2*CS_mult)
			PlaceImpactAndRipplesGeneric(x, y, cs_resistance, 3, 1*CS_mult)
		end
	end


	-- local time = os.clock()
	-- local elapsed_time = 0


	local is_coastal_cache = table.fill(false, iW * iH)
	local plot_type_cache = table.fill(false, iW * iH)
	
	local is_riverW_cache = table.fill(false, iW * iH)
	local is_riverNW_cache = table.fill(false, iW * iH)
	local is_riverNE_cache = table.fill(false, iW * iH)

	local mountain_base_cache = table.fill(0, iW * iH)
	local mountain_base_cache_coast = table.fill(0, iW * iH)
	-- local adj_is_cache = {}

	

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			local plot = Map.GetPlot(x,y);
			plot_type_cache[i] = plot:GetPlotType();

			-- treat lakes like mountains
			if plot:IsLake() == true then
				plot_type_cache[i] = PlotTypes.PLOT_MOUNTAIN
			end
			is_coastal_cache[i] = plot:IsCoastalLand(50);
			is_riverW_cache[i] = plot:IsWOfRiver()
			is_riverNW_cache[i] = plot:IsNWOfRiver()
			is_riverNE_cache[i] = plot:IsNEOfRiver()

			mountain_base_cache[i] = mountain_base_cache[i]+cs_resistance[i]
			mountain_base_cache_coast[i] = mountain_base_cache[i]
			-- local adj_is = GetAdjacent(x,y);
			-- adj_is_cache[i] = adj_is;
			local adj_is = adj_is_cache[i]
			for loop2, adj_i in ipairs(adj_is) do
				local adj_x, adj_y = i_to_xy(adj_i, iW, iH);
				-- local adj_plot = Map.GetPlot(adj_x, adj_y);
				if plot_type_cache[adj_i] ~= nil then
					if plot_type_cache[adj_i] == PlotTypes.PLOT_OCEAN then
						if is_coastal_cache[i] == false then  -- inland sea
							mountain_base_cache[i] = mountain_base_cache[i]+20.0*impass_mult
							mountain_base_cache_coast[i] = mountain_base_cache_coast[i]+2.0
						else -- coastal plot
							mountain_base_cache[i] = mountain_base_cache[i]+2.0
						end
					end
					if plot_type_cache[adj_i] == PlotTypes.PLOT_MOUNTAIN then
						mountain_base_cache[i] = mountain_base_cache[i]+20.0*impass_mult
						mountain_base_cache_coast[i] = mountain_base_cache_coast[i]+20.0*impass_mult
					end
				end
			end
		end
	end
	
	-- local player_flow_graphs = {}
	local land_graph = table.fill(-1, iW * iH)
	local flow_graph = table.fill(-1.0, iW * iH)
	local l_to_coastal = {}

	for l = 1, tablelength(test_plots) do
		
		local test_player_plot = test_plots[l]
		l_to_coastal[l] = test_player_plot:IsCoastalLand(50);
		local test_play_i = xy_to_i(test_player_plot:GetX(), test_player_plot:GetY(), iW, iH)
		flow_graph[test_play_i] = 1.0
		land_graph[test_play_i] = l
	end

	for loop = 1, pulse_depth do
		local temp_flow_graph = table.fill(-1.0, iW * iH)
		local temp_land_graph = table.fill(-1.0, iW * iH)
		-- local temp_a_flows = table.fill(-1.0, iW * iH)
		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				local i = xy_to_i(x, y, iW, iH);
				if flow_graph[i] > 0.0 then
					temp_flow_graph[i] = flow_graph[i]
					temp_land_graph[i] = land_graph[i]
				elseif plot_type_cache[i] == PlotTypes.PLOT_OCEAN or plot_type_cache[i] == PlotTypes.PLOT_MOUNTAIN then
					temp_flow_graph[i] = 0.0
					temp_land_graph[i] = 0
				else
					local a_hit = false
					local a_tot = 0.0
					local a_num = 0

					local a_flow_tot = 0
					local a_flows = {}
					local a_lands = {}
					local a_mountain_base = mountain_base_cache[i]
					local a_mountain_base_coast = mountain_base_cache_coast[i]

					local adj_is = adj_is_cache[i];
					-- northeast clockwise
					for loop2, adj_i in ipairs(adj_is) do
						-- local adj_plot = Map.GetPlot(adj_x, adj_y);
						if is_riverW_cache[adj_i] ~= nil then

							if flow_graph[adj_i] > 0.0 then
								local a_flow = 1.0
								if l_to_coastal[land_graph[adj_i]] then
									a_flow = a_flow+a_mountain_base_coast
								else
									a_flow = a_flow+a_mountain_base
								end
								
								local hasRiver = false
								if loop2 == 2 then
									hasRiver = is_riverW_cache[i]
								elseif loop2 == 3 then
									hasRiver = is_riverNW_cache[i]
								elseif loop2 == 4 then
									hasRiver = is_riverNE_cache[i]
								elseif loop2 == 5 then
									hasRiver = is_riverW_cache[adj_i]
								elseif loop2 == 6 then
									hasRiver = is_riverNW_cache[adj_i]
								elseif loop2 == 1 then
									hasRiver = is_riverNE_cache[adj_i]
								end

								if hasRiver then
									a_flow = a_flow+2.0;
								end

								if plot_type_cache[i] == PlotTypes.PLOT_HILLS then
									a_flow = a_flow+1.0;
								end

							
								
								a_hit = true
								a_tot = a_tot + flow_graph[adj_i]
								a_num = a_num+1
								a_flow_tot = a_flow_tot + 1
								a_flows[a_flow_tot] = a_flow
								a_lands[a_flow_tot] = land_graph[adj_i]
								
							end
						end
					end

					local tot_flow = 0;
					local land_tots = {}
					for loop2 = 1, a_flow_tot do
						tot_flow = tot_flow+(1.0/a_flows[loop2])
						if land_tots[a_lands[loop2]] == nil then
							land_tots[a_lands[loop2]] = 0
						end
						land_tots[a_lands[loop2]] = land_tots[a_lands[loop2]]+(1.0/a_flows[loop2])
					end

					if a_hit then

						local best_land = -1
						local best_flow = -1.0
						for loop2 = 1, a_flow_tot do
							if land_tots[a_lands[loop2]] > best_flow then
								best_flow = land_tots[a_lands[loop2]]
								best_land = a_lands[loop2]
							end
						end

						temp_land_graph[i] = best_land

						if flow_graph[i] <= -1.0*(tot_flow) then
							temp_flow_graph[i] = flow_graph[i]+tot_flow
						else
							temp_flow_graph[i] = 0.99985*a_tot/a_num*0.9998^loop
							-- temp_flow_graph[i] =
						end
						if l_to_coastal[best_land] then
							if is_coastal_cache[i] then
								temp_flow_graph[i] = temp_flow_graph[i]*1.00005*1.0001^loop
							else
								temp_flow_graph[i] = temp_flow_graph[i]*0.99995*0.9999^loop
							end
						end
						temp_flow_graph[i] = temp_flow_graph[i]*1.0
					end
				end
			end
		end
		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				local i = xy_to_i(x, y, iW, iH);
				flow_graph[i] = temp_flow_graph[i]
				land_graph[i] = temp_land_graph[i]
			end
		end
	end

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			if land_graph[i] < 0 then
				land_graph[i] = 0
			end
		end
	end

	local total_flow_graph = table.fill(-1.0, iW * iH)
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			if land_graph[i] ~= 0 then

				local adj_is = adj_is_cache[i];
				local is_boundary = false
				local boundary_flows = {}
				local total_flows = 0
				local total_blocking = 0

				if plot_type_cache[i] == PlotTypes.PLOT_HILLS then
					total_blocking = total_blocking+1
				end

				for loop2, adj_i in ipairs(adj_is) do
					-- local adj_x, adj_y = i_to_xy(adj_i, iW, iH);
					if land_graph[adj_i] ~= nil then
						if land_graph[adj_i] ~= 0 then
							if land_graph[adj_i] ~= land_graph[i] then
								is_boundary = true

								local boundary_flow = 1.0
								boundary_flow = boundary_flow+cs_resistance[adj_i]
								
								local hasRiver = false
								if loop2 == 2 then
									hasRiver = is_riverW_cache[i]
								elseif loop2 == 3 then
									hasRiver = is_riverNW_cache[i]
								elseif loop2 == 4 then
									hasRiver = is_riverNE_cache[i]
								elseif loop2 == 5 then
									hasRiver = is_riverW_cache[adj_i]
								elseif loop2 == 6 then
									hasRiver = is_riverNW_cache[adj_i]
								elseif loop2 == 1 then
									hasRiver = is_riverNE_cache[adj_i]
								end

								if hasRiver then
									boundary_flow = boundary_flow+2
								end
								-- if plot_type_cache[adj_i] == PlotTypes.PLOT_HILLS then
								-- 	boundary_flow = boundary_flow+1
								-- end
								total_flows = total_flows+1
								boundary_flows[total_flows] = boundary_flow

							end
						else
							total_blocking = total_blocking+2
						end
					end
				end

				-- if math.min(25.0, land_graph_best[i]^2/math.max(1e-12, land_graph_second_best[i]*land_graph_third_best[i])) > 15.0 then
				-- 	total_flow_graph[i] = 2.0
				-- elseif land_graph_best[i] > 0.0 then
				-- 	total_flow_graph[i] = 1.0
				-- else
				-- 	total_flow_graph[i] = 0.0
				-- end
				if is_boundary then
					local total_flow = 0.0
					for loop2 = 1, total_flows do
						total_flow = total_flow + 1.0/(total_blocking+boundary_flows[loop2])
					end
					total_flow_graph[i] = total_flow
				else
					total_flow_graph[i] = 0.0
					-- total_flow_graph[i] = land_graph_best[i]/math.max(1e-12, land_graph_second_best[i]*land_graph_third_best[i])
				end
			end
		end
	end

	-- print(os.clock()-time)
	-- print("FLOW TIME")
	
	return land_graph, total_flow_graph
end

-- all of the rejection sampling stuff is inserted here

function AssignStartingPlots:PlaceResourcesAndCityStates()
	-- suff before isolation
		-- local iW, iH = Map.GetGridSize();	
		
		-- This function controls nearly all resource placement. Only resources
		-- placed during Normalization operations are handled elsewhere.
		--
		-- Luxury resources are placed in relationship to Regions, adapting to the
		-- details of the given map instance, including number of civs and city 
		-- states present. At Jon's direction, Luxuries have been implemented to
		-- be diplomatic widgets for trading, in addition to sources of Happiness.
		--
		-- Strategic and Bonus resources are terrain-adjusted. They will customize
		-- to each map instance. Each terrain type has been measured and has certain 
		-- resource types assigned to it. You can customize resource placement to 
		-- any degree desired by controlling generation of plot groups to feed in
		-- to the process. The default plot groups are terrain-based, but any
		-- criteria you desire could be used to determine plot group membership.
		-- 
		-- If any default methods fail to meet a specific need, don't hesitate to 
		-- replace them with custom methods. I have labored to make this new 
		-- system as accessible and powerful as any ever before offered.



		print("Map Generation - Assigning Luxury Resource Distribution");
		self:AssignLuxuryRoles()

		self:PlaceCityStates()



		-- Generate global plot lists for resource distribution.
		self:GenerateGlobalResourcePlotLists()

		--takes a lot of time
		self:PlaceLuxuries()

		-- Place Strategic and Bonus resources.
		self:PlaceStrategicAndBonusResources()

		self:NormalizeCityStateLocations()	

		-- Fix Sugar graphics
		self:FixResourceGraphics()


		
		-- Necessary to implement placement of Natural Wonders, and possibly other plot-type changes.
		-- This operation must be saved for last, as it invalidates all regional data by resetting Area IDs.
		Map.RecalculateAreas();

		-- local map_output = {};
		-- local iW, iH = Map.GetGridSize();
		-- for y = 0, iH - 1 do
		-- 	for x = 0, iW - 1 do
		-- 		local i = iW * y + x;
		-- 		local plot = Map.GetPlot(x, y);
		-- 		if (plot:GetTerrainType() == TerrainTypes.TERRAIN_DESERT) and (plot:GetFeatureType() ~= FeatureTypes.FEATURE_FLOOD_PLAINS) and (plot:GetResourceType(-1) == self.incense_ID) then
		-- 			map_output[i] = {x, y, 2}
		-- 		elseif plot:GetPlotType() == PlotTypes.PLOT_OCEAN then
		-- 			map_output[i] = {x, y, 0};
		-- 		else
		-- 			map_output[i] = {x, y, 1};
		-- 		end

		-- 	end
		-- end
		-- print("MAP READOUT:");
		-- print(dump(map_output));
		-- print("MAP READOUT OVER");

		-- local beta_boost_cs = Map.GetCustomOption(29);

		for cs_number = 1, self.iNumCityStates do
			if self.city_state_validity_table[cs_number] == true then
				local data_table = self.cityStatePlots[cs_number];
				local x = data_table[1];
				local y = data_table[2];
				local city_state_ID = cs_number + GameDefines.MAX_MAJOR_CIVS - 1
				local cityState = Players[city_state_ID];
				local plot = Map.GetPlot(x, y);
				if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
					plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true);
				end
				-- if beta_boost_cs == 2 then
				-- 	BoostCS(cityState, x, y);
				-- end
			else
				local data_table = self.cityStatePlots[cs_number];
				if data_table ~= nil then
					local x = data_table[1];
					local y = data_table[2];
					-- print("WEIRD CS")
					-- print(x .. " " .. y)
					local city_state_ID = cs_number + GameDefines.MAX_MAJOR_CIVS - 1
					local cityState = Players[city_state_ID];
					local plot = Map.GetPlot(x, y);
					if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
						plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true);
					end
					-- if beta_boost_cs == 2 then
					-- 	BoostCS(cityState, x, y);
					-- end
				end
			end
		end

	local accept_map = false
	local beta_isolation_rejection = Map.GetCustomOption(22) + 2
	-- local beta_isolation_rejection = 4
	
	-- self.iNumCivs, self.iNumCityStates, self.player_ID_list, self.bTeamGame, self.teams_with_major_civs, self.number_civs_per_team = GetPlayerAndTeamInfo()
	if beta_isolation_rejection == 1 or beta_isolation_rejection == 2 then
		accept_map = true
	end
	-- isolation stuff
	if beta_isolation_rejection ~= 1 then

		local player_plots = {}
		local player_coastal_bias = {}
		for l = 1, self.iNumCivs do
			table.insert(player_plots, Players[self.player_ID_list[l]]:GetStartingPlot())
			table.insert(player_coastal_bias, CivNeedsCoastalStart(GameInfo.Civilizations[Players[self.player_ID_list[l]]:GetCivilizationType()].Type))
		end

		local cs_plots = {}
		for cs_number = 1, self.iNumCityStates do
			if self.city_state_validity_table[cs_number] == true then
				local data_table = self.cityStatePlots[cs_number];
				local x = data_table[1];
				local y = data_table[2];
				table.insert(cs_plots, Map.GetPlot(x, y))
			end
		end

		local land_graph, total_flow_graph = CalculateFlows(player_plots, 24, true, cs_plots, 2, 2.0)
		local land_graph_territory, total_flow_graph_territory = CalculateFlows(player_plots, 36, true, cs_plots, 8, 0.5)
		-- local back_flow_plots = {}
		-- for y = 0, iH - 1 do
		-- 	for x = 0, iW - 1 do
		-- 		local i = xy_to_i(x, y, iW, iH);
		-- 		if total_flow_graph[i] > 0 then
		-- 			table.insert(back_flow_plots, Map.GetPlot(x, y))
		-- 		end
		-- 	end
		-- end

		-- local back_trace, total_back_flow_graph = CalculateFlows(back_flow_plots, 32, true, cs_plots, 2)

		-- dump_map(total_flow_graph)
		-- dump_map(back_trace)
		-- dump_map(total_back_flow_graph)

		local flow_from_to = {}
		local flow_first = {}
		local flow_second = {}
		local flow_third = {}
		local land_sums = {}
		for l = 1, self.iNumCivs do
			land_sums[l] = 0
			flow_from_to[l] = {}
			flow_first[l] = 0.0
			flow_second[l] = 0.0
			flow_third[l] = 0.0
			for p = 1, self.iNumCivs do
				flow_from_to[l][p] = 0.0
			end
		end

		
		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				local i = xy_to_i(x, y, iW, iH);
				if total_flow_graph[i] > 0.0 then
					local adj_is = adj_is_cache[i];
					-- land_sums[land_graph[i]] = land_sums[land_graph[i]]+1
					for loop2, adj_i in ipairs(adj_is) do
						-- local adj_x, adj_y = i_to_xy(adj_i, iW, iH);
						if land_graph[adj_i] ~= nil then
							if land_graph[adj_i] ~= 0 then
								if land_graph[adj_i] ~= land_graph[i] then
									flow_from_to[land_graph[i]][land_graph[adj_i]] = flow_from_to[land_graph[i]][land_graph[adj_i]]+total_flow_graph[i]
									-- print(flow_from_to[land_graph[i]][land_graph[adj_i]])
								end
							end
						end
					end
				end
				if total_flow_graph_territory[i] > -1.0 then
					land_sums[land_graph_territory[i]] = land_sums[land_graph_territory[i]]+1
				end
			end
		end

		

		local worst_ratio = 1.0
		
		if beta_isolation_rejection > 2 then
			for l = 1, self.iNumCivs do
				for p = 1, self.iNumCivs do
					local land_ratio = land_sums[l]/math.max(1, land_sums[p])
					if land_ratio < worst_ratio then
						worst_ratio = land_ratio
					end
				end
			end
		end

		-- local beta_prox_cs = Map.GetCustomOption(44)
		local beta_prox_cs = 2
		local prox_cs = false
		if beta_prox_cs == 1 then
			prox_cs = true
		else
			-- use a different pulse that doesn't have friction on CS
			local land_graph2, total_flow_graph2 = CalculateFlows(player_plots, 50, false, {}, 0, 0.5)
			local player_hits = {}
			local missed_player = false
			for cs_number = 1, self.iNumCityStates do
				if self.city_state_validity_table[cs_number] == true then
					local data_table = self.cityStatePlots[cs_number];
					local cs_x = data_table[1];
					local cs_y = data_table[2];
					player_hits[land_graph2[xy_to_i(cs_x, cs_y, iW, iH)]] = true
				end
			end
			-- print("PLAYER HITS")
			-- print(dump(player_hits))
			for l = 1, self.iNumCivs do
				if player_hits[l] == nil then
					missed_player = true
				end
			end
			if missed_player == false then
				prox_cs = true
			end
			-- dump_map(land_graph2)
		end


		-- new distance factor
		-- local beta_min_distance = Map.GetCustomOption(43)
		local beta_min_distance = 6
		local min_dist = 99
		if beta_min_distance ~= 1 then
			min_dist = 5+beta_min_distance
		end
		if self.iNumCivs ~= 6 then
			min_dist = 99
		end
		local min_acceptable = true
		
		for l = 1, self.iNumCivs do
			for p = 1, self.iNumCivs do

				local player_plot1 = Players[self.player_ID_list[l]]:GetStartingPlot()
				local player_plot2 = Players[self.player_ID_list[p]]:GetStartingPlot()

				local deltaX = math.abs(player_plot1:GetX()-player_plot2:GetX())
				-- wrapping
				if deltaX > math.floor(iW / 2) and Map:IsWrapX() then 
					deltaX = math.floor(iW / 2) - (deltaX % math.floor(iW / 2))
				end

				local deltaY = math.abs(player_plot1:GetY()-player_plot2:GetY())

				local pDistance = math.sqrt( (deltaX)^2 + (deltaY)^2 );

				local iso_factor = (0.75-(pDistance/26.0))*2.8; -- was 3.3
				local iso_factor = 1.0/(1+(math.exp(-iso_factor))^2);

				flow_from_to[l][p] = flow_from_to[l][p]*iso_factor*3.8

				if l ~= p then
					if pDistance < min_dist then
						min_acceptable = false
					end
				end

			end
		end

		local region_adjs = {}
		local topology_thresh = 0.0
		if beta_isolation_rejection == 3 then
			topology_thresh = 5.0
		elseif beta_isolation_rejection == 3 then
			topology_thresh = 5.75
		elseif beta_isolation_rejection == 5 then
			topology_thresh = 6.5
		end

		for l = 1, self.iNumCivs do
			region_adjs[l] = {}
			for p = 1, self.iNumCivs do
				if flow_from_to[l][p] > topology_thresh or l == p then
					region_adjs[l][p] = true
				end

				if flow_from_to[l][p] > flow_first[l] then
					flow_third[l] = flow_second[l]
					flow_second[l] = flow_first[l]
					flow_first[l] = flow_from_to[l][p]
				elseif flow_from_to[l][p] > flow_second[l] then
					flow_third[l] = flow_second[l]
					flow_second[l] = flow_from_to[l][p]
				elseif flow_from_to[l][p] > flow_third[l] then
					flow_third[l] = flow_from_to[l][p]
				end
			end
		end


		local topology_check = true

		if beta_isolation_rejection > 2 then
			for l = 1, self.iNumCivs do
				for p = 1, self.iNumCivs do
					if l ~= p then
						local reg_d = {}
						GraphSuspendedBFS(region_adjs[l], reg_d, region_adjs, 1, p)
						if tablelength(reg_d) < self.iNumCivs-1 then
							topology_check = false
						end
					end
				end
			end
		end
		--- old method

		-- local region_depth_adjs = {}
		-- for l = 1, self.iNumCivs do
		-- 	local reg_d = {}
		-- 	GraphBFS(region_adjs[l], reg_d, region_adjs, 1)
		-- 	region_depth_adjs[l] = reg_d
		-- end
		

		
		-- for l = 1, self.iNumCivs do
		-- 	local far_count = 0
		-- 	if tablelength(region_depth_adjs[l]) < 6 then
		-- 		topology_check = false
		-- 	else
		-- 		for p = 1, self.iNumCivs do
		-- 			if region_depth_adjs[l][p] > 2 then
		-- 				far_count = far_count+1
		-- 			end
		-- 			if region_depth_adjs[l][p] > 3 then
		-- 				topology_check = false
		-- 			end
		-- 		end
		-- 		if far_count > 1 then
		-- 			topology_check = false
		-- 		end
		-- 	end
		-- end
		-- dump_map(land_graph)
		-- print(dump(region_adjs))
		-- print(dump(region_depth_adjs))
		-- print("TOPOLOGY CHECK: " .. tostring(topology_check))

		local most_isolated = 1000.0
		local worst_player = 0

		for l = 1, self.iNumCivs do
			local isolation_score = flow_first[l]*0.1+flow_second[l]*0.85+flow_third[l]*0.85
			if isolation_score < most_isolated then
				most_isolated = isolation_score
				worst_player = l
			end
		end

		local iso_thresh = 0.0
		if beta_isolation_rejection == 3 then
			iso_thresh = 5.0
		elseif beta_isolation_rejection == 4 then
			iso_thresh = 7.0
		elseif beta_isolation_rejection == 5 then
			iso_thresh = 9.0
		end

		local iso_check  = most_isolated > iso_thresh


		-- spawn validity check


		local lux_ids = {}
		lux_ids[GameInfo.Resources.RESOURCE_WHALE.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_PEARLS.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_IVORY.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_FUR.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_SILK.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_DYE.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_SPICES.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_SUGAR.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_COTTON.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_WINE.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_INCENSE.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_GOLD.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_SILVER.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_GEMS.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_MARBLE.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_COPPER.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_SALT.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_CITRUS.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_TRUFFLES.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_CRAB.ID] = 1
		lux_ids[GameInfo.Resources.RESOURCE_COCOA.ID] = 1


		if beta_using_lekmod then
			lux_ids[GameInfo.Resources.RESOURCE_COFFEE.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_TEA.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_TOBACCO.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_AMBER.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_JADE.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_OLIVE.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_PERFUME.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_CORAL.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_LAPIS.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_OBSIDIAN.ID] = 1
			lux_ids[GameInfo.Resources.RESOURCE_COCONUT.ID] = 1
		end

		local spawn_validity = true
		if beta_spawn_validation == 2 and self.iNumCivs == 6 then
			for player_index, pPlot in ipairs(player_plots) do
				local needs_coast = player_coastal_bias[player_index]
				if pPlot:IsCoastalLand(50) ~= needs_coast then
					spawn_validity = false
				end
				p_i = xy_to_i(pPlot:GetX(), pPlot:GetY(), iW, iH)
				all_rings = {}
				ring1 = {}
				ring2 = {}
				ring3 = {}
				all_rings[p_i] = 1
				ring1[p_i] = 1

				local adj_is1 = adj_is_cache[p_i]
				for loop1, adj_i1 in ipairs(adj_is1) do
					if all_rings[adj_i1] == nil then
						all_rings[adj_i1] = 1
						ring1[adj_i1] = 1
					end
				end

				for adj_i1, dumby in pairs(ring1) do
					local adj_is2 = adj_is_cache[adj_i1]
					for loop2, adj_i2 in ipairs(adj_is2) do
						if all_rings[adj_i2] == nil then
							all_rings[adj_i2] = 1
							ring2[adj_i2] = 1
						end
					end
				end
				for adj_i2, dumby in pairs(ring2) do
					local adj_is3 = adj_is_cache[adj_i2]
					for loop3, adj_i3 in ipairs(adj_is3) do
						if all_rings[adj_i3] == nil then
							all_rings[adj_i3] = 1
							ring3[adj_i3] = 1
						end
					end
				end

				local block_total = 0.0
				local lux_count = 0
				local horse_count = 0
				local iron_count = 0
				local coal_count = 0
				local oil_count = 0

				for i, dumby in pairs(all_rings) do
					local x, y = i_to_xy(i, iW, iH)
					local plot = Map.GetPlot(x, y)
					if plot ~= nil then
						local plotType = plot:GetPlotType()
						local resourceType = plot:GetResourceType()
						if lux_ids[resourceType] ~= nil then
							lux_count = lux_count+1
						elseif resourceType == GameInfo.Resources.RESOURCE_HORSE.ID then
							horse_count = horse_count+1
						elseif resourceType == GameInfo.Resources.RESOURCE_IRON.ID then
							iron_count = iron_count+1
						elseif resourceType == GameInfo.Resources.RESOURCE_COAL.ID then
							coal_count = coal_count+1
						elseif resourceType == GameInfo.Resources.RESOURCE_OIL.ID then
							oil_count = oil_count+1
						end
						--get blocks
						if plot:GetPlotType() == PlotTypes.PLOT_MOUNTAIN then
							if ring1[i] ~= nil then
								block_total = block_total+5.0
							elseif ring2[i] ~= nil then
								block_total = block_total+3.0
							elseif ring3[i] ~= nil then
								block_total = block_total+2.0
							end
						end

						if plot:IsLake() then
							if ring1[i] ~= nil then
								block_total = block_total+4.0
							elseif ring2[i] ~= nil then
								block_total = block_total+2.0
							elseif ring3[i] ~= nil then
								block_total = block_total+1.0
							end
						end

					end
				end

				if block_total > 17.0 then 
					spawn_validity = false
				end
				if lux_count ~= 4 then
					spawn_validity = false
				end
				if horse_count < 1 then
					spawn_validity = false
				end
				if iron_count < 1 then
					spawn_validity = false
				end
				if coal_count < 1 then
					spawn_validity = false
				end
				if oil_count < 2 then
					spawn_validity = false
				end
			end
		end

		local biggest_area = Map.FindBiggestArea(false);
		local iBiggestAreaID = biggest_area:GetID();


		local final_choke_pass = true
		for y = math.ceil(iH * 2 / 5), math.ceil((iH - 1) * 3 / 5) do
			local entry_count = 0
			for x = 0, iW - 1 do
				local plot = Map.GetPlot(x, y);
				local i = xy_to_i(x, y, iW, iH)
				if plot:GetArea() == iBiggestAreaID and BETA_TECTONIC_LANDS[i] < 1 then
					entry_count = entry_count+1
				end
			end
			if entry_count < 10 then
				final_choke_pass = false
			end
		end

		
		for x =  math.ceil(iW * 2 / 5), math.ceil((iW - 1) * 3 / 5) do
			local entry_count = 0
			for y = 0, iH - 1 do
				local plot = Map.GetPlot(x, y);
				local i = xy_to_i(x, y, iW, iH)
				if plot:GetArea() == iBiggestAreaID and BETA_TECTONIC_LANDS[i] < 1 then
					entry_count = entry_count+1
				end
			end
			if entry_count < 10 then
				final_choke_pass = false
			end
		end

		-- print("MAINLAND TILES " .. main_tile_count)

	
		-- local fail_data = table.fill(1, 6)
		-- if iso_check == false then
		-- 	print("ISOLATION FAILED")
		-- 	fail_data[1] = 0
		-- end
		-- if min_acceptable == false then
		-- 	print("MIN DISTANCE FAILED")
		-- 	fail_data[2] = 0
		-- end
		-- if prox_cs == false then
		-- 	print("PROXIMAL CS FAILED")
		-- 	fail_data[3] = 0
		-- end
		-- if topology_check == false then
		-- 	print("TOPOLOGY FAILED")
		-- 	fail_data[4] = 0
		-- end
		-- if (worst_ratio > 0.45) == false then
		-- 	print("LAND RATIO FAILED")
		-- 	fail_data[5] = 0
		-- end
		-- if spawn_validity == false then
		-- 	print("SPAWN FAILED")
		-- 	fail_data[6] = 0
		-- end



		-- print("FAIL READOUT:")
		-- print(dump(fail_data))
		-- print("FAIL READOUT OVER")


		if iso_check and min_acceptable and prox_cs and topology_check and worst_ratio > 0.45 and spawn_validity and final_choke_pass then
			accept_map = true
			if beta_isolation_rejection == 2 then
				for id, player in pairs(Players) do
					player:AddNotification(NotificationTypes.NOTIFICATION_GENERIC, "Player " .. worst_player .. " Has the Worst Isolation Score of: " .. most_isolated, "Player Connectivity")
				end
			end
		end
	end

	-- Activate for debug only
	self:PrintFinalResourceTotalsToLog()

	

	return accept_map
	--
end

-- added flexy-lux option

function AssignStartingPlots:PlaceSpecificNumberOfResources(resource_ID, quantity, amount,
	                         ratio, impact_table_number, min_radius, max_radius, plot_list)
	-- This function needs to receive seven numbers and one table.
	--
	-- Resource_ID is the type of resource to place.
	-- Quantity is the in-game quantity of the resource, or 0 if unquantified resource type.
	-- Amount is the number of plots intended to receive an assignment of this resource.
	--
	-- Ratio should be > 0 and <= 1 and is what determines when secondary and tertiary lists 
	-- come in to play. The actual ratio is (AmountOfResource / PlotsInList). For instance, 
	-- if we are assigning Sugar resources to Marsh, then if we are to assign eight Sugar 
	-- resources, but there are only four Marsh plots in the list, a ratio of 1 would assign
	-- a Sugar to every single marsh plot, and then have to return an unplaced value of 4; 
	-- but a ratio of 0.5 would assign only two Sugars to the four marsh plots, and return a 
	-- value of 6. Any ratio less than or equal to 0.25 would assign one Sugar and return
	-- seven, as the ratio results will be rounded up not down, to the nearest integer.
	--
	-- Impact tables: -1 = ignore, 1 = strategic, 2 = luxury, 3 = bonus, 4 = fish
	-- Radius is amount of impact to place on this table when placing a resource.
	--
	-- nil tables are not acceptable but empty tables are fine
	--
	-- The plot lists will be processed sequentially, so randomize them in advance.
	-- 
	
	--print("-"); print("PlaceSpecificResource called. ResID:", resource_ID, "Quantity:", quantity, "Amount:", amount, "Ratio:", ratio);
	

	if plot_list == nil then
		--print("Plot list was nil! -PlaceSpecificNumberOfResources");
		return
	end
	local bCheckImpact = false;
	local impact_table = {};
	if impact_table_number == 1 then
		bCheckImpact = true;
		impact_table = self.strategicData;
	elseif impact_table_number == 2 then
		bCheckImpact = true;
		impact_table = self.luxuryData;
	elseif impact_table_number == 3 then
		bCheckImpact = true;
		impact_table = self.bonusData;
	elseif impact_table_number == 4 then
		bCheckImpact = true;
		impact_table = self.fishData;
	elseif impact_table_number == 8 then
		bCheckImpact = true;
		impact_table = self.seaOilData;
	end
	-- local iW, iH = Map.GetGridSize();
	local iNumLeftToPlace = amount;
	local iNumPlots = table.maxn(plot_list);
	local iNumResources = math.min(amount, math.ceil(ratio * iNumPlots));
	-- Main loop
	for place_resource = 1, iNumResources do
		for loop, plotIndex in ipairs(plot_list) do
			if bCheckImpact == false or impact_table[plotIndex] == 0 then
				local x = (plotIndex - 1) % iW;
				local y = (plotIndex - x - 1) / iW;
				local res_plot = Map.GetPlot(x, y)
				local i = xy_to_i(x, y, iW, iH)
				if res_plot:GetResourceType(-1) == -1 then -- Placing this resource in this plot.
					if beta_tectonic_mounts == 1 then
						res_plot:SetResourceType(resource_ID, quantity);
						self.amounts_of_resources_placed[resource_ID + 1] = self.amounts_of_resources_placed[resource_ID + 1] + quantity;
						--print("-"); print("Placed Resource#", resource_ID, "at Plot", x, y);
						self.totalLuxPlacedSoFar = self.totalLuxPlacedSoFar + 1;
						iNumLeftToPlace = iNumLeftToPlace - 1;
						if bCheckImpact == true then
							local res_addition = 0;
							if max_radius > min_radius then
								res_addition = Map.Rand(1 + (max_radius - min_radius), "Resource Radius - Place Resource LUA");
							end
							local rad = min_radius + res_addition;
							if beta_flexy_lux == 2 and impact_table_number == 2 and rad == 3 then
								rad = 2+Map.Rand(3, "Flexy Lux Radius");
							elseif beta_flexy_lux == 3 and impact_table_number == 2 and rad == 3 then
								rad = 2+Map.Rand(2, "Flexy Lux Radius");
							elseif beta_flexy_lux == 4 and impact_table_number == 2 and rad == 3 then
								rad = 2+math.min(Map.Rand(2, "Flexy Lux Radius"), Map.Rand(2, "Flexy Lux Radius"));
							end
							self:PlaceResourceImpact(x, y, impact_table_number, rad);
							
						end
						break
					else
						if (res_plot:GetPlotType() == PlotTypes.PLOT_OCEAN and BETA_TECTONIC_LANDS[i] > 0) == false then
							res_plot:SetResourceType(resource_ID, quantity);
							self.amounts_of_resources_placed[resource_ID + 1] = self.amounts_of_resources_placed[resource_ID + 1] + quantity;
							--print("-"); print("Placed Resource#", resource_ID, "at Plot", x, y);
							self.totalLuxPlacedSoFar = self.totalLuxPlacedSoFar + 1;
							iNumLeftToPlace = iNumLeftToPlace - 1;
							if bCheckImpact == true then
								local res_addition = 0;
								if max_radius > min_radius then
									res_addition = Map.Rand(1 + (max_radius - min_radius), "Resource Radius - Place Resource LUA");
								end
								local rad = min_radius + res_addition;
								if beta_flexy_lux == 2 and impact_table_number == 2 and rad == 3 then
									rad = 2+Map.Rand(3, "Flexy Lux Radius");
								elseif beta_flexy_lux == 3 and impact_table_number == 2 and rad == 3 then
									rad = 2+Map.Rand(2, "Flexy Lux Radius");
								elseif beta_flexy_lux == 4 and impact_table_number == 2 and rad == 3 then
									rad = 2+math.min(Map.Rand(2, "Flexy Lux Radius"), Map.Rand(2, "Flexy Lux Radius"));
								end
								self:PlaceResourceImpact(x, y, impact_table_number, rad);
								
							end
							break
						end
					end
				end
			end
		end
	end
	return iNumLeftToPlace
end


-- extended region edge range to add more space for cs

function AssignStartingPlots:ObtainNextSectionInRegion(incoming_west_x, incoming_south_y,
	                         incoming_width, incoming_height, iAreaID, force_it, ignore_collisions)
	--print("ObtainNextSectionInRegion called, for AreaID", iAreaID, "with SW plot at ", incoming_west_x, incoming_south_y, " Width/Height at", incoming_width, incoming_height);
	--[[ This function carves off the outermost plots in a region, checks them for City
	     State Placement eligibility, and returns 7 variables: two plot lists, the 
	     coordinates of the inner portion of the area that was not processed on this 
	     round, and a boolean indicating whether the middle of the region was reached. ]]--
	--[[ If this round does not produce a suitable placement site, another round can be 
	     executed on the remaining unprocessed plots, recursively, until the middle of
	     the region has been reached. If the entire region has no eligible plots, then
	     it is likely that something extreme is going on with the map. Then choose a plot 
	     from the outermost portion of the region at random and hope for the best. ]]--
	--

	local region_edge_range = 0.2
	if beta_cs_placement == 2 then
		region_edge_range = 0.35
	end

	local iW, iH = Map.GetGridSize();
	local reached_middle = false;
	if incoming_width <= 0 or incoming_height <= 0 then -- Nothing to process
		return {}, {}, -1, -1, -1, -1, true;
	end
	if incoming_width < 4 or incoming_height < 4 then
		reached_middle = true;
	end
	local bTaller = false;
	local rows_to_check = math.ceil(region_edge_range * incoming_width);
	if incoming_height > incoming_width then
		bTaller = true;
		rows_to_check = math.ceil(region_edge_range * incoming_height);
	end
	-- Main loop
	local coastal_plots, inland_plots = {}, {};
	for section_y = incoming_south_y, incoming_south_y + incoming_height - 1 do
		for section_x = incoming_west_x, incoming_west_x + incoming_width - 1 do
			if reached_middle then -- Process all plots.
				local x = section_x % iW;
				local y = section_y % iH;
				if self:CanPlaceCityStateAt(x, y, iAreaID, force_it, ignore_collisions) == true then
					local i = y * iW + x + 1;
					if self.plotDataIsCoastal[i] == true then
						table.insert(coastal_plots, i);
					else
						table.insert(inland_plots, i);
					end
				end
			else -- Process only plots near enough to the region edge.
				if bTaller == false then -- Processing leftmost and rightmost columns.
					if section_x < incoming_west_x + rows_to_check or section_x >= incoming_west_x + incoming_width - rows_to_check then
						local x = section_x % iW;
						local y = section_y % iH;
						if self:CanPlaceCityStateAt(x, y, iAreaID, force_it, ignore_collisions) == true then
							local i = y * iW + x + 1;
							if self.plotDataIsCoastal[i] == true then
								table.insert(coastal_plots, i);
							else
								table.insert(inland_plots, i);
							end
						end
					end
				else -- Processing top and bottom rows.
					if section_y < incoming_south_y + rows_to_check or section_y >= incoming_south_y + incoming_height - rows_to_check then
						local x = section_x % iW;
						local y = section_y % iH;
						if self:CanPlaceCityStateAt(x, y, iAreaID, force_it, ignore_collisions) == true then
							local i = y * iW + x + 1;
							if self.plotDataIsCoastal[i] == true then
								table.insert(coastal_plots, i);
							else
								table.insert(inland_plots, i);
							end
						end
					end
				end
			end
		end
	end
	local new_west_x, new_south_y, new_width, new_height;
	if bTaller then
		new_west_x = incoming_west_x + rows_to_check;
		new_south_y = incoming_south_y;
		new_width = incoming_width - (2 * rows_to_check);
		new_height = incoming_height;
	else
		new_west_x = incoming_west_x;
		new_south_y = incoming_south_y + rows_to_check;
		new_width = incoming_width;
		new_height = incoming_height - (2 * rows_to_check);
	end		

	return coastal_plots, inland_plots, new_west_x, new_south_y, new_width, new_height, reached_middle;
end

-- added option to override rejected luxes

function AssignStartingPlots:GetRandomLuxuriesTargetNumber()

	-- local beta_lux_variety = Map.GetCustomOption(38);
	local beta_lux_variety = 2

	--[[ MOD.Barathor:
		 This data was separated out to allow easy replacement in map scripts.
		 With more luxuries available, this ensures that the total luxuries used each game
		 still match the default game, except for Huge, which really needed a few more anyway! 
	local worldsizes = {							
		[GameInfo.Worlds.WORLDSIZE_DUEL.ID] = 4,
		[GameInfo.Worlds.WORLDSIZE_TINY.ID] = 7,
		[GameInfo.Worlds.WORLDSIZE_SMALL.ID] = 12,
		[GameInfo.Worlds.WORLDSIZE_STANDARD.ID] = 14,
		[GameInfo.Worlds.WORLDSIZE_LARGE.ID] = 16,
		[GameInfo.Worlds.WORLDSIZE_HUGE.ID] = 18,
		}
	local maxRandoms = worldsizes[Map.GetWorldSize()]
	]]

	--HB base number of luxes avaliable on the chosen map X & Y size
	-- max is 30, min 4
	-- local iW, iH = Map.GetGridSize();

	local LandXY = iW * iH
	local maxRandoms = 30
	local baseLuxCount = 4

	if LandXY < 6700 and beta_lux_variety == 1 then
		maxRandoms = (LandXY-720)/((2560-720)/8)+baseLuxCount
	end

	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LUX COUNT @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	print("Random Lux Count: " .. maxRandoms);
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

	return maxRandoms
end

-- tuned fish rates to account for extra islands, surface area, and lake fish

function AssignStartingPlots:PlaceStrategicAndBonusResources()

	-- local beta_lake_fish = Map.GetCustomOption(37)
	-- KEY: {Resource ID, Quantity (0 = unquantified), weighting, minimum radius, maximum radius}
	-- KEY: (frequency (1 per n plots in the list), impact list number, plot list, resource data)
	--
	-- The radius creates a zone around the plot that other resources of that
	-- type will avoid if possible. See ProcessResourceList for impact numbers.
	--
	-- Order of placement matters, so changing the order may affect a later dependency.
	
	-- Adjust amounts, if applicable, based on Resource Setting.
	local uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = self:GetMajorStrategicResourceQuantityValues()
	
	-- Adjust appearance rate per Resource Setting chosen by user.
	local bonus_multiplier = 0.65;

	if self.resource_setting == 1 then -- Near to nothing
		bonus_multiplier = 1;
	elseif self.resource_setting == 2 then -- 
		bonus_multiplier = 0.90;
	elseif self.resource_setting == 3 then -- 
		bonus_multiplier = 0.80;
	elseif self.resource_setting == 4 then -- 
		bonus_multiplier = 0.75;
	elseif self.resource_setting == 6 then -- 
		bonus_multiplier = 0.55;
	elseif self.resource_setting == 7 then -- 
		bonus_multiplier = 0.45;
	elseif self.resource_setting == 8 then -- 
		bonus_multiplier = 0.35;
	elseif self.resource_setting == 9 then -- 
		bonus_multiplier = 0.25;
	elseif self.resource_setting == 10 then -- filled the map full
		bonus_multiplier = 0.15;
	end

	-- Place Strategic resources.
	print("Map Generation - Placing Strategics");
	local resources_to_place = {
	{self.oil_ID, oil_amt, 65, 1, 4},
	{self.uranium_ID, uran_amt, 35, 1, 4} };
	self:ProcessResourceList(7, 1, self.marsh_list, resources_to_place)

	local resources_to_place = {
	{self.oil_ID, oil_amt, 55, 1, 5},
	{self.aluminum_ID, alum_amt, 15, 1, 2},
	{self.iron_ID, iron_amt, 35, 1, 2} };
	self:ProcessResourceList(16, 1, self.tundra_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.oil_ID, oil_amt, 65, 1, 5},
	{self.aluminum_ID, alum_amt, 15, 1, 2},
	{self.iron_ID, iron_amt, 20, 1, 2} };
	self:ProcessResourceList(15, 1, self.snow_flat_list, resources_to_place)

	local resources_to_place = {
	{self.oil_ID, oil_amt, 70, 1, 2},
	{self.iron_ID, iron_amt, 30, 1, 2} };
	self:ProcessResourceList(11, 1, self.desert_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.iron_ID, iron_amt, 26, 1, 3},
	{self.coal_ID, coal_amt, 35, 1, 3},
	{self.aluminum_ID, alum_amt, 39, 1, 3} };
	self:ProcessResourceList(22, 1, self.hills_list, resources_to_place)

	local resources_to_place = {
	{self.coal_ID, coal_amt, 30, 1, 2},
	{self.uranium_ID, uran_amt, 70, 1, 2} };
	self:ProcessResourceList(33, 1, self.jungle_flat_list, resources_to_place)
	local resources_to_place = {
	{self.coal_ID, coal_amt, 25, 1, 2},
	{self.oil_ID, oil_amt, 25, 1, 5},
	{self.uranium_ID, uran_amt, 50, 10, 0} };
	self:ProcessResourceList(39, 1, self.forest_flat_list, resources_to_place)

	local resources_to_place = {
	{self.horse_ID, horse_amt, 100, 1, 5} };
	self:ProcessResourceList(10, 1, self.dry_grass_flat_no_feature, resources_to_place)
	local resources_to_place = {
	{self.horse_ID, horse_amt, 100, 1, 5} };
	self:ProcessResourceList(10, 1, self.plains_flat_no_feature, resources_to_place)

	self:AddModernMinorStrategicsToCityStates() -- Added spring 2011
	
	self:PlaceSmallQuantitiesOfStrategics(23 * bonus_multiplier, self.land_list);
	
	self:PlaceOilInTheSea();

	
	-- Check for low or missing Strategic resources
	if self.amounts_of_resources_placed[self.iron_ID + 1] < 8 then
		--print("Map has very low iron, adding another.");
		local resources_to_place = { {self.iron_ID, iron_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.hills_list, resources_to_place) -- 99999 means one per that many tiles: a single instance.
	end
	if self.amounts_of_resources_placed[self.iron_ID + 1] < 4 * self.iNumCivs then
		--print("Map has very low iron, adding another.");
		local resources_to_place = { {self.iron_ID, iron_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.land_list, resources_to_place)
	end
	if self.amounts_of_resources_placed[self.horse_ID + 1] < 4 * self.iNumCivs then
		print("Map has very low horse, adding another.");
		local resources_to_place = { {self.horse_ID, horse_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.plains_flat_no_feature, resources_to_place)
		
		--print("Map has very low horse, adding another.");
		local resources_to_place = { {self.horse_ID, horse_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.dry_grass_flat_no_feature, resources_to_place)
	end
	if self.amounts_of_resources_placed[self.coal_ID + 1] < 8 then
		--print("Map has very low coal, adding another.");
		local resources_to_place = { {self.coal_ID, coal_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.hills_list, resources_to_place)
	end
	if self.amounts_of_resources_placed[self.coal_ID + 1] < 4 * self.iNumCivs then
		--print("Map has very low coal, adding another.");
		local resources_to_place = { {self.coal_ID, coal_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.land_list, resources_to_place)
	end
	if self.amounts_of_resources_placed[self.oil_ID + 1] < 4 * self.iNumCivs then
		print("Map has very low oil, adding another.");
		local resources_to_place = { {self.oil_ID, oil_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.land_list, resources_to_place)
	end
	if self.amounts_of_resources_placed[self.aluminum_ID + 1] < 4 * self.iNumCivs then
		--print("Map has very low aluminum, adding another.");
		local resources_to_place = { {self.aluminum_ID, alum_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.hills_list, resources_to_place)
	end
	
	while self.amounts_of_resources_placed[self.uranium_ID + 1] < 5 * self.iNumCivs do
		print("Map has very low uranium, adding another.");
		local resources_to_place = { {self.uranium_ID, uran_amt, 100, 0, 0} };
		self:ProcessResourceList(99999, 1, self.land_list, resources_to_place)
	end
	
	
	-- Place Bonus Resources
	print("Map Generation - Placing Bonuses");
	
	if self._lek_coastal_refish then


		-- local iW, iH = Map.GetGridSize()
		local plotDataImmediateCoast = {};
		local plotDataNextToImmediateCoast ={};
		local plotDataIsThreeFromMainland = {};

		plotDataImmediateCoast, plotDataNextToImmediateCoast = GenerateMainlandExpandedCoastData();
		plotDataIsThreeFromMainland = GenerateThreeFromMainlandCoast(plotDataImmediateCoast, plotDataNextToImmediateCoast);

		table.fill(self.mainland_coast_list, false, iW * iH);
		table.fill(self.mainland_coast_list_inner, false, iW * iH);
		table.fill(self.mainland_coast_list_second, false, iW * iH);
		table.fill(self.mainland_coast_list_outer, false, iW * iH);

		local temp_list_pangaea = {};
		local temp_list_pangaea_inner = {};
		local temp_list_pangaea_second = {};
		local temp_list_pangaea_outer = {};

		-- create a single combined mainland coast list *and* separate
		for x = 0, iW - 1 do
			for y = 0, iH - 1 do
				local i = iW * y + x + 1;
				local test_plot = Map.GetPlot(x, y)
				if plotDataImmediateCoast[i] == true then
					self.mainland_coast_list[i] = true;
					self.mainland_coast_list_inner[i] = true;
					-- local test_plot = Map.GetPlot(x, y);
					table.insert(temp_list_pangaea, i);
					table.insert(temp_list_pangaea_inner, i);
				elseif plotDataNextToImmediateCoast[i] == true then
					self.mainland_coast_list[i] = true;
					self.mainland_coast_list_second[i] = true;
					-- local test_plot = Map.GetPlot(x, y);
					table.insert(temp_list_pangaea, i);
					table.insert(temp_list_pangaea_second, i);
				elseif plotDataIsThreeFromMainland[i] == true then
					self.mainland_coast_list[i] = true;
					self.mainland_coast_list_outer[i] = true;
					-- local test_plot = Map.GetPlot(x, y);
					table.insert(temp_list_pangaea, i);
					table.insert(temp_list_pangaea_outer, i);
				end
			end
		end

		local fish_coast_inner = GetShuffledCopyOfTable(temp_list_pangaea_inner)
		local fish_coast_second = GetShuffledCopyOfTable(temp_list_pangaea_second)
		local fish_coast_outer = GetShuffledCopyOfTable(temp_list_pangaea_outer)

		if beta_tectonic_mounts == 1 then
			self:PlaceFishMainland(3 * bonus_multiplier, fish_coast_inner);
			self:PlaceFishMainland(8 * bonus_multiplier, fish_coast_second);
			self:PlaceFishMainland(15 * bonus_multiplier, fish_coast_outer);
		else
			self:PlaceFishMainland(3 * bonus_multiplier + (0.15*1.75), fish_coast_inner);
			self:PlaceFishMainland(8 * bonus_multiplier + (0.4*1.75), fish_coast_second);
			self:PlaceFishMainland(15 * bonus_multiplier + (0.75*1.75), fish_coast_outer);
		end

	elseif self.method == 1 then

		-- local iW, iH = Map.GetGridSize()
		local plotDataImmediateCoast = {};
		local plotDataNextToImmediateCoast ={};
		local plotDataIsThreeFromMainland = {};

		plotDataImmediateCoast, plotDataNextToImmediateCoast = GenerateMainlandExpandedCoastData();
		plotDataIsThreeFromMainland = GenerateThreeFromMainlandCoast(plotDataImmediateCoast, plotDataNextToImmediateCoast);

		table.fill(self.mainland_coast_list, false, iW * iH);

		-- create a single combined maniland coast list
		for x = 0, iW - 1 do
			for y = 0, iH - 1 do
				local i = iW * y + x + 1;
				if plotDataImmediateCoast[i] == true or plotDataNextToImmediateCoast[i] == true or plotDataIsThreeFromMainland[i] == true then
					self.mainland_coast_list[i] = true;
				end
			end
		end

		local temp_list_panagaea = {};

		for x = 0, iW - 1 do
			for y = 0, iH - 1 do
				local i = iW * y + x + 1;
				local test_plot = Map.GetPlot(x, y);
				if self.mainland_coast_list[i] == true then	
					table.insert(temp_list_panagaea, i);
				end
			end
		end

		local fish_list_b = {}

		for x = 0, iW - 1 do
			for y = 0, iH - 1 do
				local i = iW * y + x + 1;
				local test_plot = Map.GetPlot(x, y);
				if self.mainland_coast_list[i] == true or (beta_lake_fish == 2 and test_plot:IsLake() == true) then	
					table.insert(fish_list_b, i);
					if((beta_lake_fish == 2 and test_plot:IsLake() == true)) then
						table.insert(fish_list_b, i);
						table.insert(fish_list_b, i);
						table.insert(fish_list_b, i);
						table.insert(fish_list_b, i);
					end
				end
			end
		end

		self.coast_list_panagaea = GetShuffledCopyOfTable(temp_list_panagaea)
		
		-- self:PlaceFishMainland(10 * bonus_multiplier + fish_boost, self.coast_list_panagaea);
		-- print("FISHINESS HERE")
		if beta_tectonic_mounts == 1 then
			self:PlaceFishMainland(10 * bonus_multiplier, GetShuffledCopyOfTable(fish_list_b));
		else
			self:PlaceFishMainland(10 * bonus_multiplier + (0.5*1.75), GetShuffledCopyOfTable(fish_list_b));
		end
	end

	if self._lek_coastal_refish then
		-- place fish around island
		if beta_tectonic_mounts == 1 then
			self:PlaceFish(16 * bonus_multiplier, self.coast_list);
		else
			self:PlaceFish(16 * bonus_multiplier + (0.8*1.75), self.coast_list);
		end
	else
		if beta_tectonic_mounts == 1 then
			self:PlaceFish(8 * bonus_multiplier, self.coast_list);
		else
			self:PlaceFish(8 * bonus_multiplier + (0.4*1.75), self.coast_list);
		end
	end


	self:PlaceSexyBonusAtCivStarts()
	self:AddExtraBonusesToHillsRegions()
	
	local resources_to_place = {
	{self.deer_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(6 * bonus_multiplier, 3, self.extra_deer_list, resources_to_place)

	local resources_to_place = {
	{self.wheat_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(6 * bonus_multiplier, 3, self.desert_wheat_list, resources_to_place)

	local resources_to_place = {
	{self.deer_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(8 * bonus_multiplier, 3, self.tundra_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.banana_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(10 * bonus_multiplier, 3, self.banana_list, resources_to_place)

	local resources_to_place = {
	{self.wheat_ID, 1, 100, 1, 3} };
	self:ProcessResourceList(30 * bonus_multiplier, 3, self.plains_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.bison_ID, 1, 100, 2, 3} };
	self:ProcessResourceList(15 * bonus_multiplier, 3, self.plains_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.cow_ID, 1, 100, 2, 3} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.plains_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.cow_ID, 1, 100, 2, 3} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.grass_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.stone_ID, 1, 100, 1, 1} };
	self:ProcessResourceList(20 * bonus_multiplier, 3, self.dry_grass_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.bison_ID, 1, 100, 1, 1} };
	self:ProcessResourceList(20 * bonus_multiplier, 3, self.dry_grass_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.sheep_ID, 1, 100, 1, 1} };
	self:ProcessResourceList(20 * bonus_multiplier, 3, self.hills_open_list, resources_to_place)

	local resources_to_place = {
	{self.stone_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(10 * bonus_multiplier, 3, self.tundra_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.stone_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(16 * bonus_multiplier, 3, self.desert_flat_no_feature, resources_to_place)

	local resources_to_place = {
	{self.deer_ID, 1, 100, 3, 4} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.forest_flat_that_are_not_tundra, resources_to_place)
	
	local resources_to_place = {
	{self.hardwood_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.hills_covered_list, resources_to_place)

	local resources_to_place = {
	{self.hardwood_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.flat_covered, resources_to_place)

	local resources_to_place = {
	{self.hardwood_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(22 * bonus_multiplier, 3, self.tundra_flat_forest, resources_to_place)
	
	local resources_to_place = {
	{self.maize_ID, 1, 100, 1, 2} };
	self:ProcessResourceList(35 * bonus_multiplier, 3, self.plains_flat_no_feature, resources_to_place)
	
	
end

-- added functionality to make sure fish don't collide with shallows

function AssignStartingPlots:PlaceFishMainland(frequency, plot_list)

	-- local beta_lake_fish = Map.GetCustomOption(37)


	-- This function places fish at members of plot_list. (Sounds fishy to me!)
	if plot_list == nil then
		--print("No fish were placed! -PlaceFish");
		return
	end
	-- local iW, iH = Map.GetGridSize();
	-- local iNumTotalPlots = table.maxn(plot_list);
	local iNumTotalPlots = tablelength(plot_list)
	local iNumFishToPlace = math.ceil(iNumTotalPlots / frequency);
	
	-- Main loop
	local current_index = 1;
	for place_resource = 1, iNumFishToPlace do
		local placed_this_res = false;
		if current_index <= iNumTotalPlots then
			for index_to_check = current_index, iNumTotalPlots do
				if placed_this_res == true then
					break
				else
					current_index = current_index + 1;
				end
				local plotIndex = plot_list[index_to_check];

				if self.fishData[plotIndex] == 0 then
					local x = (plotIndex - 1) % iW;
					local y = (plotIndex - x - 1) / iW;
					local res_plot = Map.GetPlot(x, y)
					--TODO: Check feature (Atoll)
					local featureType = res_plot:GetFeatureType()

					if featureType ~= self.feature_atoll and featureType ~= FeatureTypes.FEATURE_ICE then

						if res_plot:GetResourceType(-1) == -1 then
							-- Placing fish here. First decide impact radius of this fish.
							local fish_radius = Map.Rand(0, "Fish Radius - Place Fish LUA") + 1;
							--if fish_radius > 4 then
							--	fish_radius = 3;
							--end
							if beta_tectonic_mounts == 1 then
								res_plot:SetResourceType(self.fish_ID, 1);
								-- if beta_lake_fish == 2 then
								-- 	if res_plot:IsLake() then
								-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_FOOD, -1)
								-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
								-- 	end
								-- end
								self:PlaceResourceImpact(x, y, 4, fish_radius);
								placed_this_res = true;
								self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
							else
								if BETA_TECTONIC_LANDS[plotIndex] < 1 then 
									res_plot:SetResourceType(self.fish_ID, 1);
									-- if beta_lake_fish == 2 then
									-- 	if res_plot:IsLake() then
									-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_FOOD, -1)
									-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
									-- 	end
									-- end
									self:PlaceResourceImpact(x, y, 4, fish_radius);
									placed_this_res = true;
									self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
								end
							end
						end
					end
				end
			end
		end
	end
end

-- added functionality to make sure fish don't collide with shallows

function AssignStartingPlots:PlaceFish(frequency, plot_list)
	-- This function places fish at members of plot_list. (Sounds fishy to me!)
	if plot_list == nil then
		--print("No fish were placed! -PlaceFish");
		return
	end
	local iW, iH = Map.GetGridSize();
	local iNumTotalPlots = table.maxn(plot_list);
	local iNumFishToPlace = math.ceil(iNumTotalPlots / frequency);
	local bMainlandCoast = false;

	-- Main loop
	local current_index = 1;
	for place_resource = 1, iNumFishToPlace do
		local placed_this_res = false;
		if current_index <= iNumTotalPlots then
			for index_to_check = current_index, iNumTotalPlots do
				if placed_this_res == true then
					break
				else
					current_index = current_index + 1;
				end
				local plotIndex = plot_list[index_to_check];
				bMainlandCoast = false;

				if self.method == 1 then
					bMainlandCoast = self.mainland_coast_list[plotIndex];
				end

				if self.fishData[plotIndex] == 0 and bMainlandCoast == false then
					local x = (plotIndex - 1) % iW;
					local y = (plotIndex - x - 1) / iW;
					local res_plot = Map.GetPlot(x, y)
					if res_plot:GetResourceType(-1) == -1 then
						-- Placing fish here. First decide impact radius of this fish.
						local fish_radius = Map.Rand(4, "Fish Radius - Place Fish LUA") + 1;
						--if fish_radius > 4 then
						--	fish_radius = 3;
						--end
						if beta_tectonic_mounts == 1 then
							res_plot:SetResourceType(self.fish_ID, 1);
							self:PlaceResourceImpact(x, y, 4, fish_radius);
							placed_this_res = true;
							self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
						else
							if BETA_TECTONIC_LANDS[plotIndex] < 1 then
								res_plot:SetResourceType(self.fish_ID, 1);
								self:PlaceResourceImpact(x, y, 4, fish_radius);
								placed_this_res = true;
								self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
							end
						end
					end
				end
			end
		end
	end
end

-- edited for lake fish and shallows

function AssignStartingPlots:AttemptToPlaceBonusResourceAtPlot(x, y, bAllowOasis, Fish_Count)
	-- Returns two booleans. First is true if something was placed. Second true if Oasis placed.
	--print("-"); print("Attempting to place a Bonus at: ", x, y);
	local plot = Map.GetPlot(x, y);
	local maxNumGranary = 4
	local maxFishPlace = Fish_Count
	if plot == nil then
		--print("Placement failed, plot was nil.");
		return false
	end
	if plot:GetResourceType(-1) ~= -1 then
		--print("Plot already had a resource.");
		return false
	end
	local terrainType = plot:GetTerrainType()
	if terrainType == TerrainTypes.TERRAIN_SNOW then
		--print("Plot was arctic land buried beneath endless snow.");
		return false
	end
	local featureType = plot:GetFeatureType()
	if featureType == FeatureTypes.FEATURE_OASIS then
		--print("Plot already had an Oasis.");
		return false
	end
	local plotType = plot:GetPlotType()
	--
	-- Made by EAP
	-- Note: a lot of this code doesn't do anything, yet, lot of it is for if you increase the iNumFoodBonusNeeded above 3 at the end of the iNumFoodBonusNeeded calculations
	-- Here we place possible fish
	if plotType == PlotTypes.PLOT_OCEAN then
		if maxNumGranary > 0 and maxFishPlace > 0 then
			if terrainType == TerrainTypes.TERRAIN_COAST and featureType == FeatureTypes.NO_FEATURE then
				if plot:IsLake() == false or beta_lake_fish == 2 then -- Place Fish
					if beta_tectonic_mounts == 1 then
						plot:SetResourceType(self.fish_ID, 1);
						print("Placed Fish.");
						self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
						maxFishPlace = maxFishPlace - 1;
						return true, false, true
					else
						local i = xy_to_i(x, y, iW, iH)
						if BETA_TECTONIC_LANDS[i] < 1 then
							plot:SetResourceType(self.fish_ID, 1);
							print("Placed Fish.");
							self.amounts_of_resources_placed[self.fish_ID + 1] = self.amounts_of_resources_placed[self.fish_ID + 1] + 1;
							maxFishPlace = maxFishPlace - 1;
							return true, false, true
						end
					end
				end
			end
		end
	end
	if featureType == FeatureTypes.FEATURE_JUNGLE then -- Place Banana
		if maxNumGranary > 0 then
		plot:SetResourceType(self.banana_ID, 1);
		print("Placed Banana.");
		self.amounts_of_resources_placed[self.banana_ID + 1] = self.amounts_of_resources_placed[self.banana_ID + 1] + 1;
		maxNumGranary = maxNumGranary - 1;
		return true, false, false
		else
		return false
		end
	elseif featureType == FeatureTypes.FEATURE_FOREST then -- Place Deer
		if maxNumGranary > 0 then
		plot:SetResourceType(self.deer_ID, 1);
		print("Placed Deer.");
		self.amounts_of_resources_placed[self.deer_ID + 1] = self.amounts_of_resources_placed[self.deer_ID + 1] + 1; 
		maxNumGranary = maxNumGranary - 1;
		return true, false, false
		else
		return false
		end
	elseif featureType == FeatureTypes.FEATURE_FOREST then -- Place Hardwood
		plot:SetResourceType(self.hardwood_ID, 1);
		print("Placed Hardwood.");
		self.amounts_of_resources_placed[self.hardwood_ID + 1] = self.amounts_of_resources_placed[self.hardwood_ID + 1] + 1;
		return true, false, false
	elseif plotType == PlotTypes.PLOT_HILLS and featureType == FeatureTypes.NO_FEATURE and terrainType ~= TerrainTypes.TERRAIN_DESERT then
		-- add a sheep or deer, for deer add forest first
		if maxNumGranary > 0 then
			plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
			plot:SetResourceType(self.deer_ID, 1);
			print("Placed Deer xx.");
			self.amounts_of_resources_placed[self.deer_ID + 1] = self.amounts_of_resources_placed[self.deer_ID + 1] + 1;
			maxNumGranary = maxNumGranary - 1;
			return true, false, false
		else
			return false
		end
	-- Sheep or Deer on Hills, if not desert	
	elseif plotType == PlotTypes.PLOT_HILLS and featureType == FeatureTypes.NO_FEATURE and terrainType ~= TerrainTypes.TERRAIN_DESERT then
		plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
		plot:SetResourceType(self.hardwood_ID, 1);
		print("Placed Hardwood.");
		self.amounts_of_resources_placed[self.hardwood_ID + 1] = self.amounts_of_resources_placed[self.hardwood_ID + 1] + 1;
		return true, false, false
	elseif plotType == PlotTypes.PLOT_HILLS and featureType == FeatureTypes.NO_FEATURE and terrainType ~= TerrainTypes.TERRAIN_DESERT then
		plot:SetResourceType(self.sheep_ID, 1);
		print("Placed Sheep xx.");
		self.amounts_of_resources_placed[self.sheep_ID + 1] = self.amounts_of_resources_placed[self.sheep_ID + 1] + 1;
		return true, false, false
		
	-- Flat grassland Bison, Deer or Cow
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_GRASS then
		local placethis = Map.Rand(100, "");
		if placethis < 50 then
			if maxNumGranary > 0 then	
				plot:SetResourceType(self.bison_ID, 1);
				print("Placed Bison.");
				self.amounts_of_resources_placed[self.bison_ID + 1] = self.amounts_of_resources_placed[self.bison_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		elseif placethis > 50 and placethis < 90 then
			if maxNumGranary > 0 then
				plot:SetResourceType(self.sheep_ID, 1);
				plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true); -- make it a hill
				print("Placed Sheep.");
				self.amounts_of_resources_placed[self.sheep_ID + 1] = self.amounts_of_resources_placed[self.sheep_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		else
			if maxNumGranary > 0 then	
				plot:SetResourceType(self.stone_ID, 1);
				print("Placed Stone.");
				self.amounts_of_resources_placed[self.stone_ID + 1] = self.amounts_of_resources_placed[self.stone_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		end
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_GRASS then
		local placethis = Map.Rand(100, "");
		if placethis < 67 then
			if maxNumGranary > 0 then
				plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
				plot:SetResourceType(self.deer_ID, 1);
				print("Placed Deer xx.");
				self.amounts_of_resources_placed[self.deer_ID + 1] = self.amounts_of_resources_placed[self.deer_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		else
			if maxNumGranary > 0 then
				plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
				plot:SetResourceType(self.deer_ID, 1);
				plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true); -- make it a hill
				print("Placed Deer xx.");
				self.amounts_of_resources_placed[self.deer_ID + 1] = self.amounts_of_resources_placed[self.deer_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		end
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_GRASS then
			plot:SetResourceType(self.cow_ID, 1);
			print("Placed Cow.");
			self.amounts_of_resources_placed[self.cow_ID + 1] = self.amounts_of_resources_placed[self.cow_ID + 1] + 1;
		return true, false, false
	
	-- Wheat, Bison, Cow or Hardwood on Flat plains
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_PLAINS then
		local placethis = Map.Rand(100, "");
		if placethis < 75 then
			if maxNumGranary > 0 then
				plot:SetResourceType(self.wheat_ID, 1);
				print("Placed Wheat.");
				self.amounts_of_resources_placed[self.wheat_ID + 1] = self.amounts_of_resources_placed[self.wheat_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		else
			if maxNumGranary > 0 then
				plot:SetResourceType(self.bison_ID, 1);
				print("Placed Bison.");
				self.amounts_of_resources_placed[self.bison_ID + 1] = self.amounts_of_resources_placed[self.bison_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		end
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_PLAINS then
		if maxNumGranary > 0 then
			plot:SetResourceType(self.bison_ID, 1);
			print("Placed Bison.");
			self.amounts_of_resources_placed[self.bison_ID + 1] = self.amounts_of_resources_placed[self.bison_ID + 1] + 1;
			maxNumGranary = maxNumGranary - 1;
			return true, false, false
		else
			return false
		end
	-- Place Wheat on Floodplains
	elseif terrainType == TerrainTypes.TERRAIN_DESERT and plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.FEATURE_FLOOD_PLAINS then
		-- Place Wheat
		local placethis = Map.Rand(100, "");
		if placethis < 25 then
			plot:SetResourceType(self.wheat_ID, 1);
			print("Placed Wheat.");
			self.amounts_of_resources_placed[self.wheat_ID + 1] = self.amounts_of_resources_placed[self.wheat_ID + 1] + 1;
			return true, false, false
		elseif placethis > 25 and placethis < 75 then
			plot:SetResourceType(self.sheep_ID, 1);
			plot:SetPlotType(PlotTypes.PLOT_HILLS, false, true); -- make it a hill
			plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1);
			print("Placed Sheep.");
			self.amounts_of_resources_placed[self.sheep_ID + 1] = self.amounts_of_resources_placed[self.sheep_ID + 1] + 1;
			maxNumGranary = maxNumGranary - 1;
			return true, false, false
		else
			if maxNumGranary > 0 then	-- we do actually want a limit on stone placed
				plot:SetResourceType(self.stone_ID, 1);
				print("Placed Stone.");
				self.amounts_of_resources_placed[self.stone_ID + 1] = self.amounts_of_resources_placed[self.stone_ID + 1] + 1;
				maxNumGranary = maxNumGranary - 1;
				return true, false, false
			else
				return false
			end
		end
		
		
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_PLAINS then
		plot:SetResourceType(self.cow_ID, 1);
		print("Placed Cow.");
		self.amounts_of_resources_placed[self.cow_ID + 1] = self.amounts_of_resources_placed[self.cow_ID + 1] + 1;
		return true, false, false
	elseif plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE and terrainType == TerrainTypes.TERRAIN_PLAINS then
		plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
		plot:SetResourceType(self.hardwood_ID, 1);
		print("Placed Hardwood.");
		self.amounts_of_resources_placed[self.hardwood_ID + 1] = self.amounts_of_resources_placed[self.hardwood_ID + 1] + 1;
		return true, false, false
		
	-- Tundra support, does not include granary limit since tundra bad (for now)
	elseif terrainType == TerrainTypes.TERRAIN_TUNDRA and plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE then -- Place Deer
					--add forest to the location to make it even better
					plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
					plot:SetResourceType(self.deer_ID, 1);
					print("Placed Deer.");
					self.amounts_of_resources_placed[self.deer_ID + 1] = self.amounts_of_resources_placed[self.deer_ID + 1] + 1;
					return true, false, false
	elseif terrainType == TerrainTypes.TERRAIN_TUNDRA and plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE then -- Place Hardwood
					--add forest to the location to make it even better
					plot:SetFeatureType(FeatureTypes.FEATURE_FOREST, -1);
					plot:SetResourceType(self.hardwood_ID, 1);
					print("Placed Hardwood.");
					self.amounts_of_resources_placed[self.hardwood_ID + 1] = self.amounts_of_resources_placed[self.hardwood_ID + 1] + 1;
					return true, false, false
	-- Place Wheat on Desert
	elseif terrainType == TerrainTypes.TERRAIN_DESERT and plotType == PlotTypes.PLOT_LAND and featureType == FeatureTypes.NO_FEATURE then 
		if plot:IsFreshWater() then
			-- Place Wheat
			plot:SetResourceType(self.wheat_ID, 1);
			print("Placed Wheat.");
			self.amounts_of_resources_placed[self.wheat_ID + 1] = self.amounts_of_resources_placed[self.wheat_ID + 1] + 1;
			return true, false, false
		elseif bAllowOasis then -- Place Oasis
					plot:SetFeatureType(FeatureTypes.FEATURE_OASIS, -1);
					print("Placed Oasis.");
					return true, true, false
		else
					print("Not allowed to place any more Oasis help at this site.");
		end
	end
	
	-- Nothing placed.
	return false, false, false
end

-- bug fix so lakes don't propegate apply this

function AssignStartingPlots:PlaceResourceImpactCoastalMod(x, y, impact_table_number, radius, radiusCoastal)
	-- This function operates upon one of the "impact and ripple" data overlays for resources.
	-- These data layers are a primary way of preventing assignments from clustering too much.
	-- Impact #s - 1 strategic - 2 luxury - 3 bonus - 4 fish - 5 city states - 6 natural wonders - 7 marble - 8 sheep
	-- local iW, iH = Map.GetGridSize();
	local wrapX = Map:IsWrapX();
	local wrapY = Map:IsWrapY();
	local impact_value = 99;
	local odd = self.firstRingYIsOdd;
	local even = self.firstRingYIsEven;
	local nextX, nextY, plot_adjustments;
	-- Place Impact!
	local impactPlotIndex = y * iW + x + 1;

	self.cityStateData[impactPlotIndex] = impact_value;

	if radius == 0 then
		return
	end
	-- Place Ripples

	-- local beta_coastal_deadzone = Map.GetCustomOption(24);
	local radiusCoastal = radiusCoastal
	if beta_coastal_deadzone == 1 then
		radiusCoastal = 4
	elseif beta_coastal_deadzone == 2 then
		radiusCoastal = 5
	elseif beta_coastal_deadzone == 3 then
		radiusCoastal = 6
	else
		radiusCoastal = 7
	end
	-- print("Radius Deadzone: " .. radiusCoastal)

	if radius > 0 and radius < iH / 2 then
		for ripple_radius = 1, radius do
			local ripple_value = radius - ripple_radius + 1;
			-- Moving clockwise around the ring, the first direction to travel will be Northeast.
			-- This matches the direction-based data in the odd and even tables. Each
			-- subsequent change in direction will correctly match with these tables, too.
			--
			-- Locate the plot within this ripple ring that is due West of the Impact Plot.
			local currentX = x - ripple_radius;
			local currentY = y;
			-- Now loop through the six directions, moving ripple_radius number of times
			-- per direction. At each plot in the ring, add the ripple_value for that ring 
			-- to the plot's entry in the distance data table.
			for direction_index = 1, 6 do
				for plot_to_handle = 1, ripple_radius do
					-- Must account for hex factor.
				 	if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
						plot_adjustments = odd[direction_index];
					else -- Current Y is even. Use plot adjustments from even table.
						plot_adjustments = even[direction_index];
					end
					-- Identify the next plot in the ring.
					nextX = currentX + plot_adjustments[1];
					nextY = currentY + plot_adjustments[2];
					-- Make sure the plot exists
					if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
						-- Do not add ripple data to this plot.
					elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
						-- Do not add ripple data to this plot.
					else -- Plot is in bounds, process it.
						-- Handle any world wrap.
						local realX = nextX;
						local realY = nextY;
						if wrapX then
							realX = realX % iW;
						end
						if wrapY then
							realY = realY % iH;
						end
						-- Record ripple data for this plot.
						local ringPlotIndex = realY * iW + realX + 1;

						self.cityStateData[ringPlotIndex] = 1;
					end
					currentX, currentY = nextX, nextY;
				end
			end
		end
	else
		print("Unsupported Radius length of ", radius, " passed to PlaceResourceImpact()");
	end

	if radiusCoastal > 0 and radiusCoastal < iH / 2 then
		for ripple_radius = 1, radiusCoastal do
			local ripple_value = radiusCoastal - ripple_radius + 1;
			-- Moving clockwise around the ring, the first direction to travel will be Northeast.
			-- This matches the direction-based data in the odd and even tables. Each
			-- subsequent change in direction will correctly match with these tables, too.
			--
			-- Locate the plot within this ripple ring that is due West of the Impact Plot.
			local currentX = x - ripple_radius;
			local currentY = y;
			-- Now loop through the six directions, moving ripple_radius number of times
			-- per direction. At each plot in the ring, add the ripple_value for that ring 
			-- to the plot's entry in the distance data table.
			for direction_index = 1, 6 do
				for plot_to_handle = 1, ripple_radius do
					-- Must account for hex factor.
				 	if currentY / 2 > math.floor(currentY / 2) then -- Current Y is odd. Use odd table.
						plot_adjustments = odd[direction_index];
					else -- Current Y is even. Use plot adjustments from even table.
						plot_adjustments = even[direction_index];
					end
					-- Identify the next plot in the ring.
					nextX = currentX + plot_adjustments[1];
					nextY = currentY + plot_adjustments[2];
					
						-- Make sure the plot exists
					if wrapX == false and (nextX < 0 or nextX >= iW) then -- X is out of bounds.
						-- Do not add ripple data to this plot.
					elseif wrapY == false and (nextY < 0 or nextY >= iH) then -- Y is out of bounds.
						-- Do not add ripple data to this plot.
					else -- Plot is in bounds, process it.
						-- Handle any world wrap.
						local realX = nextX;
						local realY = nextY;
						if wrapX then
							realX = realX % iW;
						end
						if wrapY then
							realY = realY % iH;
						end
						-- Record ripple data for this plot.
						local plot = Map.GetPlot(realX, realY);
						if plot:IsCoastalLand(50) then
							local ringPlotIndex = realY * iW + realX + 1;

							self.cityStateData[ringPlotIndex] = 1;
							-- 
							-- This is the only call that is proper to this modded function
							self:ExpandCoastalRing(realX, realY, 3)
						end
					end
					currentX, currentY = nextX, nextY;
				end
			end
		end
	else
		print("Unsupported RadiusCoastal length of ", radiusCoastal, " passed to PlaceResourceImpact()");
	end
end

-- function ThrowMeteors(starting_plots, num_meteors)
-- 	-- local iW, iH = Map.GetGridSize();
-- 	local temp_list_all = {};
-- 	local adj_list = {}
-- 	for y = 0, iH - 1 do
-- 		for x = 0, iW - 1 do
-- 			local i = y * iW + x + 1;
-- 			local plot = Map.GetPlot(x, y);
-- 			if starting_plots.cityStateData[i] < 1 and plot:GetPlotType() ~= PlotTypes.PLOT_OCEAN and adj_list[i] == nil then 
-- 				-- starting_plots.distanceData[i] < 50 and 
-- 				table.insert(temp_list_all, i);
-- 				local adj_is = adj_is_cache[i]
-- 				for loop, adj_i in ipairs(adj_is) do
-- 					adj_list[adj_i] = 1;
-- 					local adj_is2 = adj_is_cache[adj_i]
-- 					if adj_is2 ~= nil then
-- 						for loop2, adj_i2 in ipairs(adj_is2) do
-- 							adj_list[adj_i2] = 1;
-- 							local adj_is3 = adj_is_cache[adj_i2]
-- 							if adj_is3 ~= nil then
-- 								for loop3, adj_i3 in ipairs(adj_is3) do
-- 									adj_list[adj_i3] = 1;
-- 								end
-- 							end
-- 						end
-- 					end
-- 				end
-- 			end
-- 		end
-- 	end
-- 	local list_all = GetShuffledCopyOfTable(temp_list_all);
-- 	-- print("GOT HERE SMITIN")
-- 	-- print(tablelength(list_all))
-- 	for l = 1, num_meteors do
-- 		i = list_all[l];
-- 		if i ~= nil then
-- 			local x, y = i_to_xy(i, iW, iH);
-- 			local plot = Map.GetPlot(x, y);
-- 			plot:SetPlotType(PlotTypes.PLOT_HILLS)
-- 			plot:SetTerrainType(TerrainTypes.TERRAIN_TUNDRA)
-- 			plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT)
-- 			plot:SetResourceType(starting_plots.gems_ID, 1)

-- 			starting_plots:PlaceResourceImpact(x, y, 1, 0)					-- Strategic layer
-- 			starting_plots:PlaceResourceImpact(x, y, 2, 0)					-- Luxury layer
-- 			starting_plots:PlaceResourceImpact(x, y, 3, 0)					-- Bonus layer
-- 			starting_plots:PlaceResourceImpact(x, y, 5, 2)					-- City State layer
-- 			starting_plots:PlaceResourceImpact(x, y, 7, 0)					-- Marble layer

-- 			local temp_adj_is = adj_is_cache[i]
-- 			local adj_list_m = GetShuffledCopyOfTable(temp_adj_is)
-- 			local loop_count = 1
-- 			for loop, adj_i in ipairs(adj_list_m) do
-- 				local adj_x, adj_y = i_to_xy(adj_i, iW, iH);
-- 				local adj_plot = Map.GetPlot(adj_x, adj_y);
-- 				if adj_plot ~= nil then
-- 					if adj_plot:GetPlotType() ~= PlotTypes.PLOT_OCEAN then
-- 						adj_plot:SetPlotType(PlotTypes.PLOT_LAND)
-- 						adj_plot:SetTerrainType(TerrainTypes.TERRAIN_TUNDRA)
-- 						if loop_count == 1 then
-- 							adj_plot:SetResourceType(starting_plots.uranium_ID, 2)
-- 							adj_plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT)

-- 							starting_plots:PlaceResourceImpact(x, y, 1, 0)					-- Strategic layer
-- 							starting_plots:PlaceResourceImpact(x, y, 2, 0)					-- Luxury layer
-- 							starting_plots:PlaceResourceImpact(x, y, 3, 0)					-- Bonus layer
-- 							starting_plots:PlaceResourceImpact(x, y, 7, 0)					-- Marble layer

-- 						elseif loop_count == 2 then
-- 							adj_plot:SetResourceType(starting_plots.oil_ID, 6)
-- 							adj_plot:SetFeatureType(-1)
							
-- 							starting_plots:PlaceResourceImpact(x, y, 1, 0)					-- Strategic layer
-- 							starting_plots:PlaceResourceImpact(x, y, 2, 0)					-- Luxury layer
-- 							starting_plots:PlaceResourceImpact(x, y, 3, 0)					-- Bonus layer
-- 							starting_plots:PlaceResourceImpact(x, y, 7, 0)					-- Marble layer

-- 						else
-- 							-- adj_plot:SetResourceType(-1)
-- 							adj_plot:SetFeatureType(-1)
-- 						end
-- 						loop_count = loop_count+1
-- 					end
-- 				end
-- 			end
-- 			-- print("GET SMITED")
-- 		end
-- 	end
-- end

-- function BoostCS(cityState, x, y)
-- 	-- local iW, iH = Map.GetGridSize();
	
-- 	local resources = 0;
-- 	local food_yields = 0;
-- 	local prod_yields = 0;

-- 	local adj_is1 = GetAdjacent(x, y, iW, iH);
-- 	adj_is1 = GetShuffledCopyOfTable(adj_is1);
-- 	for loop, adj_i1 in ipairs(adj_is1) do
-- 		local adjx1, adjy1 = i_to_xy(adj_i1, iW, iH);
-- 		local adj_plot1 = Map.GetPlot(adjx1, adjy1);
-- 		if adj_plot1:GetResourceType() ~= -1 then
-- 			resources = resources+1
-- 		end

		
-- 		food_yields = food_yields+adj_plot1:CalculateBestNatureYield(YieldTypes.YIELD_FOOD, cityState:GetTeam())
-- 		prod_yields = prod_yields+adj_plot1:CalculateBestNatureYield(YieldTypes.YIELD_PRODUCTION, cityState:GetTeam())
-- 	end

-- 	local boost_needed = 0;
-- 	if resources < 3 then
-- 		boost_needed = boost_needed+1;
-- 	end
-- 	if (food_yields < 6) or (prod_yields < 5) or (food_yields+prod_yields < 14) then
-- 		boost_needed = boost_needed+1;
-- 	end

-- 	local cs_trait = cityState:GetMinorCivTrait();
-- 	for loop, adj_i1 in ipairs(adj_is1) do
-- 		local adjx1, adjy1 = i_to_xy(adj_i1, iW, iH);
-- 		local adj_plot1 = Map.GetPlot(adjx1, adjy1);
-- 		local adj_ptype = adj_plot1:GetPlotType();
-- 		local adj_ftype = adj_plot1:GetFeatureType();
-- 		-- print("Resource Type: " .. tostring(adj_plot1:GetResourceType()));
-- 		if boost_needed > 0 and (adj_plot1:GetResourceType() == -1) then
-- 			if cs_trait == MinorCivTraitTypes.MINOR_CIV_TRAIT_CULTURED then
-- 				local imp_canidates = {};
-- 				if adj_ftype == FeatureTypes.FEATURE_FOREST or adj_ftype == FeatureTypes.FEATURE_JUNGLE then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_BRAZILWOOD_CAMP.ID);
-- 				end
				
-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TIBET.ID);
-- 				elseif adj_ptype == PlotTypes.PLOT_MOUNTAIN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TIBET.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_OCEAN or adj_ptype == PlotTypes.PLOT_LAND then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_AYER.ID);
-- 				end
				
-- 				if adj_ptype ~= PlotTypes.PLOT_OCEAN and  adj_ptype ~= PlotTypes.PLOT_MOUNTAIN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_CHATEAU.ID);
-- 				end
					
-- 				adj_plot1:SetImprovementType(GetShuffledCopyOfTable(imp_canidates)[1]);
-- 				boost_needed = boost_needed-1;

-- 			elseif cs_trait == MinorCivTraitTypes.MINOR_CIV_TRAIT_MILITARISTIC then
-- 				local imp_canidates = {};
				
-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE.ID);
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_GOTH.ID);
-- 				elseif adj_ptype == PlotTypes.PLOT_LAND then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_GOTH.ID);
-- 				end

				

-- 				if adj_ptype == PlotTypes.PLOT_OCEAN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_LANDMARK.ID);
-- 				end
				
-- 				if adj_ptype ~= PlotTypes.PLOT_OCEAN and  adj_ptype ~= PlotTypes.PLOT_MOUNTAIN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_KASBAH.ID);
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_MOTTE.ID);
-- 				end
					
-- 				adj_plot1:SetImprovementType(GetShuffledCopyOfTable(imp_canidates)[1]);
-- 				boost_needed = boost_needed-1;

-- 			elseif cs_trait == MinorCivTraitTypes.MINOR_CIV_TRAIT_MARITIME then
-- 				local imp_canidates = {};
				
-- 				if adj_ftype == FeatureTypes.FEATURE_MARSH and adj_ptype == PlotTypes.PLOT_LAND then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_POLDER.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_LAND then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TEPE.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_OCEAN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_WATER_POLDER.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_MC_MAORI_PA.ID);
-- 					-- table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TERRACE_FARM.ID);
-- 				end

-- 				adj_plot1:SetImprovementType(GetShuffledCopyOfTable(imp_canidates)[1]);
-- 				boost_needed = boost_needed-1;

-- 			elseif cs_trait == MinorCivTraitTypes.MINOR_CIV_TRAIT_MERCANTILE then
-- 				local imp_canidates = {};
				
-- 				if adj_ftype == FeatureTypes.FEATURE_FOREST or adj_ftype == FeatureTypes.FEATURE_JUNGLE then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_BRAZILWOOD_CAMP.ID);
-- 				end

-- 				if adj_ptype ~= PlotTypes.PLOT_OCEAN and  adj_ptype ~= PlotTypes.PLOT_MOUNTAIN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_CHATEAU.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_LAND or adj_ptype == PlotTypes.PLOT_OCEAN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_LANDMARK.ID);
-- 				end

					
-- 				adj_plot1:SetImprovementType(GetShuffledCopyOfTable(imp_canidates)[1]);
-- 				boost_needed = boost_needed-1;
			
-- 			elseif cs_trait == MinorCivTraitTypes.MINOR_CIV_TRAIT_RELIGIOUS then
-- 				local imp_canidates = {};

-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TIBET.ID);
-- 				elseif adj_ptype == PlotTypes.PLOT_MOUNTAIN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TIBET.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_HILLS then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_MC_MAORI_PA.ID);
-- 					-- table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TERRACE_FARM.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_LAND then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_TEPE.ID);
-- 				end

-- 				if adj_ptype == PlotTypes.PLOT_OCEAN then
-- 					table.insert(imp_canidates, GameInfo.Improvements.IMPROVEMENT_AKSUM.ID);
-- 				end
					
-- 				adj_plot1:SetImprovementType(GetShuffledCopyOfTable(imp_canidates)[1]);
-- 				boost_needed = boost_needed-1;

-- 			end
-- 		end
-- 	end
-- end

-- changed to add regional probe 

function AssignStartingPlots:BalanceAndAssign(args)
	-- This function determines what level of Bonus Resource support a location
	-- may need, identifies compatibility with civ-specific biases, and places starts.


	-- added player-region probe

	self.player_to_region = {};


	-- Normalize each start plot location.
	-- local iW, iH = Map.GetGridSize();
	local iNumStarts = table.maxn(self.startingPlots);
	for region_number = 1, iNumStarts do
		self:NormalizeStartLocation(region_number)
	end

	-- Check Game Option for disabling civ-specific biases.
	-- If they are to be disabled, then all civs are simply assigned to start plots at random.
	local bDisableStartBias = Game.GetCustomOption("GAMEOPTION_DISABLE_START_BIAS");
	if bDisableStartBias == 1 then
		--print("-"); print("ALERT: Civ Start Biases have been selected to be Disabled!"); print("-");
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
			local i = y * iW + x + 1;
			player:SetStartingPlot(start_plot)
			self.player_to_region[player_ID] = region_number
		end
		-- If this is a team game (any team has more than one Civ in it) then make 
		-- sure team members start near each other if possible. (This may scramble 
		-- Civ biases in some cases, but there is no cure).
		if self.bTeamGame == true then
			self:NormalizeTeamLocations()
		end
		-- Done with un-biased Civ placement.
		return
	end

	-- If the process reaches here, civ-specific start-location biases are enabled. Handle them now.
	-- Create a randomized list of all regions. As a region gets assigned, we'll remove it from the list.
	local all_regions = {};
	for loop = 1, self.iNumCivs do
		table.insert(all_regions, loop);
	end
	local regions_still_available = GetShuffledCopyOfTable(all_regions)

	local civs_needing_coastal_start = {};
	local civs_priority_coastal_start = {};
	local civs_needing_river_start = {};
	local civs_needing_region_priority = {};
	local civs_needing_region_avoid = {};
	local regions_with_coastal_start = {};
	local regions_with_lake_start = {};
	local regions_with_river_start = {};
	local regions_with_near_river_start = {};
	local civ_status = table.fill(false, GameDefines.MAX_MAJOR_CIVS); -- Have to account for possible gaps in player ID numbers, for MP.
	local region_status = table.fill(false, self.iNumCivs);
	local priority_lists = {};
	local avoid_lists = {};
	local iNumCoastalCivs, iNumRiverCivs, iNumPriorityCivs, iNumAvoidCivs = 0, 0, 0, 0;
	local iNumCoastalCivsRemaining, iNumRiverCivsRemaining, iNumPriorityCivsRemaining, iNumAvoidCivsRemaining = 0, 0, 0, 0;
	
	--print("-"); print("-"); print("--- DEBUG READOUT OF PLAYER START ASSIGNMENTS ---"); print("-");
	
	-- Generate lists of player needs. Each additional need type is subordinate to those
	-- that come before. In other words, each Civ can have only one need type.
	for loop = 1, self.iNumCivs do
		local playerNum = self.player_ID_list[loop]; -- MP games can have gaps between player numbers, so we cannot assume a sequential set of IDs.
		local player = Players[playerNum];
		local civType = GameInfo.Civilizations[player:GetCivilizationType()].Type;
		print("Player", playerNum, "of Civ Type", civType);
		local bNeedsCoastalStart = CivNeedsCoastalStart(civType)
		if args.MixedBias and Map.Rand(100, "") >= 0 and CivNeedsPlaceFirstCoastalStart(civType) then 
			bNeedsCoastalStart = false;
		end
		if bNeedsCoastalStart == true then
			print("- - - - - - - needs Coastal Start!"); print("-");
			iNumCoastalCivs = iNumCoastalCivs + 1;
			iNumCoastalCivsRemaining = iNumCoastalCivsRemaining + 1;
			table.insert(civs_needing_coastal_start, playerNum);
			local bPlaceFirst = CivNeedsPlaceFirstCoastalStart(civType);
			if bPlaceFirst then
				print("- - - - - - - needs to Place First!"); print("-");
				table.insert(civs_priority_coastal_start, playerNum);
			end
		else
			local bNeedsRiverStart = CivNeedsRiverStart(civType)
			if bNeedsRiverStart == true then
				--print("- - - - - - - needs River Start!"); print("-");
				iNumRiverCivs = iNumRiverCivs + 1;
				iNumRiverCivsRemaining = iNumRiverCivsRemaining + 1;
				table.insert(civs_needing_river_start, playerNum);
			else
				local iNumRegionPriority = GetNumStartRegionPriorityForCiv(civType)
				if iNumRegionPriority > 0 then
					--print("- - - - - - - needs Region Priority!"); print("-");
					local table_of_this_civs_priority_needs = GetStartRegionPriorityListForCiv_GetIDs(civType)
					iNumPriorityCivs = iNumPriorityCivs + 1;
					iNumPriorityCivsRemaining = iNumPriorityCivsRemaining + 1;
					table.insert(civs_needing_region_priority, playerNum);
					priority_lists[playerNum] = table_of_this_civs_priority_needs;
				else
					local iNumRegionAvoid = GetNumStartRegionAvoidForCiv(civType)
					if iNumRegionAvoid > 0 then
						--print("- - - - - - - needs Region Avoid!"); print("-");
						local table_of_this_civs_avoid_needs = GetStartRegionAvoidListForCiv_GetIDs(civType)
						iNumAvoidCivs = iNumAvoidCivs + 1;
						iNumAvoidCivsRemaining = iNumAvoidCivsRemaining + 1;
						table.insert(civs_needing_region_avoid, playerNum);
						avoid_lists[playerNum] = table_of_this_civs_avoid_needs;
					end
				end
			end
		end
	end
	
	print("Civs with Coastal Bias:", iNumCoastalCivs);
	print("Civs with River Bias:", iNumRiverCivs);
	print("Civs with Region Priority:", iNumPriorityCivs);
	print("Civs with Region Avoid:", iNumAvoidCivs); print("-");
	
	-- Handle Coastal Start Bias
	if iNumCoastalCivs > 0 then
		-- Generate lists of regions eligible to support a coastal start.
		local iNumRegionsWithCoastalStart, iNumRegionsWithLakeStart, iNumUnassignableCoastStarts = 0, 0, 0;
		for region_number, bAlreadyAssigned in ipairs(region_status) do
			if bAlreadyAssigned == false then
				if self.startLocationConditions[region_number][1] == true then
					print("Region#", region_number, "has a Coastal Start.");
					iNumRegionsWithCoastalStart = iNumRegionsWithCoastalStart + 1;
					table.insert(regions_with_coastal_start, region_number);
				end
			end
		end
		if iNumRegionsWithCoastalStart < iNumCoastalCivs then
			for region_number, bAlreadyAssigned in ipairs(region_status) do
				if bAlreadyAssigned == false then
					if self.startLocationConditions[region_number][2] == true and
					   self.startLocationConditions[region_number][1] == false then
						print("Region#", region_number, "has a Lake Start.");
						iNumRegionsWithLakeStart = iNumRegionsWithLakeStart + 1;
						table.insert(regions_with_lake_start, region_number);
					end
				end
			end
		end
		if iNumRegionsWithCoastalStart + iNumRegionsWithLakeStart < iNumCoastalCivs then
			iNumUnassignableCoastStarts = iNumCoastalCivs - (iNumRegionsWithCoastalStart + iNumRegionsWithLakeStart);
		end
		-- Now assign those with coastal bias to start locations, where possible.
		print("iNumCoastalCivs: " .. iNumCoastalCivs);
		print("iNumUnassignableCoastStarts: " .. iNumUnassignableCoastStarts);
		if iNumCoastalCivs - iNumUnassignableCoastStarts > 0 then
			-- create non-priority coastal start list
			local non_priority_coastal_start = {};
			for loop1, iPlayerNum1 in ipairs(civs_needing_coastal_start) do
				local bAdd = true;
				for loop2, iPlayerNum2 in ipairs(civs_priority_coastal_start) do
					if (iPlayerNum1 == iPlayerNum2) then
						bAdd = false;
					end
				end
				if bAdd then
					table.insert(non_priority_coastal_start, iPlayerNum1);
				end
			end
			
			local shuffled_priority_coastal_start = GetShuffledCopyOfTable(civs_priority_coastal_start);
			local shuffled_non_priority_coastal_start = GetShuffledCopyOfTable(non_priority_coastal_start);
			local shuffled_coastal_civs = {};
			
			-- insert priority coastal starts first
			for loop, iPlayerNum in ipairs(shuffled_priority_coastal_start) do
				table.insert(shuffled_coastal_civs, iPlayerNum);
			end
			
			-- insert non-priority coastal starts second
			for loop, iPlayerNum in ipairs(shuffled_non_priority_coastal_start) do
				table.insert(shuffled_coastal_civs, iPlayerNum);
			end			
			
			for loop, iPlayerNum in ipairs(shuffled_coastal_civs) do
				print("shuffled_coastal_civs[" .. loop .. "]: " .. iPlayerNum);
			end
			
			local shuffled_coastal_regions, shuffled_lake_regions;
			local current_lake_index = 1;
			if iNumRegionsWithCoastalStart > 0 then
				shuffled_coastal_regions = GetShuffledCopyOfTable(regions_with_coastal_start);
			end
			if iNumRegionsWithLakeStart > 0 then
				shuffled_lake_regions = GetShuffledCopyOfTable(regions_with_lake_start);
			end
			for loop, playerNum in ipairs(shuffled_coastal_civs) do
				if loop > iNumCoastalCivs - iNumUnassignableCoastStarts then
					--print("Ran out of Coastal and Lake start locations to assign to Coastal Bias.");
					break
				end
				-- Assign next randomly chosen civ in line to next randomly chosen eligible region.
				if loop <= iNumRegionsWithCoastalStart then
					-- Assign this civ to a region with coastal start.
					local choose_this_region = shuffled_coastal_regions[loop];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[playerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[playerNum] = choose_this_region

					--print("Player Number", playerNum, "assigned a COASTAL START BIAS location in Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[playerNum + 1] = true;
					iNumCoastalCivsRemaining = iNumCoastalCivsRemaining - 1;
					local a, b, c = IdentifyTableIndex(civs_needing_coastal_start, playerNum)
					if a then
						table.remove(civs_needing_coastal_start, c[1]);
					end
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				else
					-- Out of coastal starts, assign this civ to region with lake start.
					local choose_this_region = shuffled_lake_regions[current_lake_index];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[playerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[playerNum] = choose_this_region
					--print("Player Number", playerNum, "with Coastal Bias assigned a fallback Lake location in Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[playerNum + 1] = true;
					iNumCoastalCivsRemaining = iNumCoastalCivsRemaining - 1;
					local a, b, c = IdentifyTableIndex(civs_needing_coastal_start, playerNum)
					if a then
						table.remove(civs_needing_coastal_start, c[1]);
					end
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
					current_lake_index = current_lake_index + 1;
				end
			end
		--else
			--print("Either no civs required a Coastal Start, or no Coastal Starts were available.");
		end
	end
	
	-- Handle River bias
	if iNumRiverCivs > 0 or iNumCoastalCivsRemaining > 0 then
		-- Generate lists of regions eligible to support a river start.
		local iNumRegionsWithRiverStart, iNumRegionsNearRiverStart, iNumUnassignableRiverStarts = 0, 0, 0;
		for region_number, bAlreadyAssigned in ipairs(region_status) do
			if bAlreadyAssigned == false then
				if self.startLocationConditions[region_number][3] == true then
					iNumRegionsWithRiverStart = iNumRegionsWithRiverStart + 1;
					table.insert(regions_with_river_start, region_number);
				end
			end
		end
		for region_number, bAlreadyAssigned in ipairs(region_status) do
			if bAlreadyAssigned == false then
				if self.startLocationConditions[region_number][4] == true and
				   self.startLocationConditions[region_number][3] == false then
					iNumRegionsNearRiverStart = iNumRegionsNearRiverStart + 1;
					table.insert(regions_with_near_river_start, region_number);
				end
			end
		end
		if iNumRegionsWithRiverStart + iNumRegionsNearRiverStart < iNumRiverCivs then
			iNumUnassignableRiverStarts = iNumRiverCivs - (iNumRegionsWithRiverStart + iNumRegionsNearRiverStart);
		end
		-- Now assign those with river bias to start locations, where possible.
		-- Also handle fallback placement for coastal bias that failed to find a match.
		if iNumRiverCivs - iNumUnassignableRiverStarts > 0 then
			local shuffled_river_civs = GetShuffledCopyOfTable(civs_needing_river_start);
			local shuffled_river_regions, shuffled_near_river_regions;
			if iNumRegionsWithRiverStart > 0 then
				shuffled_river_regions = GetShuffledCopyOfTable(regions_with_river_start);
			end
			if iNumRegionsNearRiverStart > 0 then
				shuffled_near_river_regions = GetShuffledCopyOfTable(regions_with_near_river_start);
			end
			for loop, playerNum in ipairs(shuffled_river_civs) do
				if loop > iNumRiverCivs - iNumUnassignableRiverStarts then
					--print("Ran out of River and Near-River start locations to assign to River Bias.");
					break
				end
				-- Assign next randomly chosen civ in line to next randomly chosen eligible region.
				if loop <= iNumRegionsWithRiverStart then
					-- Assign this civ to a region with river start.
					local choose_this_region = shuffled_river_regions[loop];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[playerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[playerNum] = choose_this_region
					--print("Player Number", playerNum, "assigned a RIVER START BIAS location in Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[playerNum + 1] = true;
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				else
					-- Assign this civ to a region where a river is near the start.
					local choose_this_region = shuffled_near_river_regions[loop - iNumRegionsWithRiverStart];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[playerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[playerNum] = choose_this_region
					--print("Player Number", playerNum, "with River Bias assigned a fallback 'near river' location in Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[playerNum + 1] = true;
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				end
			end
		end
		-- Now handle any fallbacks for unassigned coastal bias.
		if iNumCoastalCivsRemaining > 0 and iNumRiverCivs < iNumRegionsWithRiverStart + iNumRegionsNearRiverStart then
			local iNumFallbacksWithRiverStart, iNumFallbacksNearRiverStart = 0, 0;
			local fallbacks_with_river_start, fallbacks_with_near_river_start = {}, {};
			for region_number, bAlreadyAssigned in ipairs(region_status) do
				if bAlreadyAssigned == false then
					if self.startLocationConditions[region_number][3] == true then
						iNumFallbacksWithRiverStart = iNumFallbacksWithRiverStart + 1;
						table.insert(fallbacks_with_river_start, region_number);
					end
				end
			end
			for region_number, bAlreadyAssigned in ipairs(region_status) do
				if bAlreadyAssigned == false then
					if self.startLocationConditions[region_number][4] == true and
					   self.startLocationConditions[region_number][3] == false then
						iNumFallbacksNearRiverStart = iNumFallbacksNearRiverStart + 1;
						table.insert(fallbacks_with_near_river_start, region_number);
					end
				end
			end
			if iNumFallbacksWithRiverStart + iNumFallbacksNearRiverStart > 0 then
			
				local shuffled_coastal_fallback_civs = GetShuffledCopyOfTable(civs_needing_coastal_start);
				local shuffled_river_fallbacks, shuffled_near_river_fallbacks;
				if iNumFallbacksWithRiverStart > 0 then
					shuffled_river_fallbacks = GetShuffledCopyOfTable(fallbacks_with_river_start);
				end
				if iNumFallbacksNearRiverStart > 0 then
					shuffled_near_river_fallbacks = GetShuffledCopyOfTable(fallbacks_with_near_river_start);
				end
				for loop, playerNum in ipairs(shuffled_coastal_fallback_civs) do
					if loop > iNumFallbacksWithRiverStart + iNumFallbacksNearRiverStart then
						--print("Ran out of River and Near-River start locations to assign as fallbacks for Coastal Bias.");
						break
					end
					-- Assign next randomly chosen civ in line to next randomly chosen eligible region.
					if loop <= iNumFallbacksWithRiverStart then
						-- Assign this civ to a region with river start.
						local choose_this_region = shuffled_river_fallbacks[loop];
						local x = self.startingPlots[choose_this_region][1];
						local y = self.startingPlots[choose_this_region][2];
						local plot = Map.GetPlot(x, y);
						local player = Players[playerNum];
						player:SetStartingPlot(plot);
						self.player_to_region[playerNum] = choose_this_region
						--print("Player Number", playerNum, "with Coastal Bias assigned a fallback river location in Region#", choose_this_region, "at Plot", x, y);
						region_status[choose_this_region] = true;
						civ_status[playerNum + 1] = true;
						local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
						if a then
							table.remove(regions_still_available, c[1]);
						end
					else
						-- Assign this civ to a region where a river is near the start.
						local choose_this_region = shuffled_near_river_fallbacks[loop - iNumRegionsWithRiverStart];
						local x = self.startingPlots[choose_this_region][1];
						local y = self.startingPlots[choose_this_region][2];
						local plot = Map.GetPlot(x, y);
						local player = Players[playerNum];
						player:SetStartingPlot(plot);
						self.player_to_region[playerNum] = choose_this_region
						--print("Player Number", playerNum, "with Coastal Bias assigned a fallback 'near river' location in Region#", choose_this_region, "at Plot", x, y);
						region_status[choose_this_region] = true;
						civ_status[playerNum + 1] = true;
						local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
						if a then
							table.remove(regions_still_available, c[1]);
						end
					end
				end
			end
		end
	end
	
	-- Handle Region Priority
	if iNumPriorityCivs > 0 then
		print("-"); print("-"); print("--- REGION PRIORITY READOUT ---"); print("-");
		local iNumSinglePriority, iNumMultiPriority, iNumNeedFallbackPriority = 0, 0, 0;
		local single_priority, multi_priority, fallback_priority = {}, {}, {};
		local single_sorted, multi_sorted = {}, {};
		-- Separate priority civs in to two categories: single priority, multiple priority.
		for playerNum, priority_needs in pairs(priority_lists) do
			local len = table.maxn(priority_needs)
			if len == 1 then
				print("Player#", playerNum, "has a single Region Priority of type", priority_needs[1]);
				local priority_data = {playerNum, priority_needs[1]};
				table.insert(single_priority, priority_data)
				iNumSinglePriority = iNumSinglePriority + 1;
			else
				print("Player#", playerNum, "has multiple Region Priority, this many types:", len);
				local priority_data = {playerNum, len};
				table.insert(multi_priority, priority_data)
				iNumMultiPriority = iNumMultiPriority + 1;
			end
		end
		-- Single priority civs go first, and will engage fallback methods if no match found.
		if iNumSinglePriority > 0 then
			-- Sort the list so that proper order of execution occurs. (Going to use a blunt method for easy coding.)
			for region_type = 1, 9 do							-- Must expand if new region types are added.
				for loop, data in ipairs(single_priority) do
					if data[2] == region_type then
						--print("Adding Player#", data[1], "to sorted list of single Region Priority.");
						table.insert(single_sorted, data);
					end
				end
			end
			-- Match civs who have a single Region Priority to the region type they need, if possible.
			for loop, data in ipairs(single_sorted) do
				local iPlayerNum = data[1];
				local iPriorityType = data[2];
				print("* Attempting to assign Player#", iPlayerNum, "to a region of Type#", iPriorityType);
				local bFoundCandidate, candidate_regions = false, {};
				for test_loop, region_number in ipairs(regions_still_available) do
					if self.regionTypes[region_number] == iPriorityType then
						table.insert(candidate_regions, region_number);
						bFoundCandidate = true;
						--print("- - Found candidate: Region#", region_number);
					end
				end
				if bFoundCandidate then
					local diceroll = 1 + Map.Rand(table.maxn(candidate_regions), "Choosing from among Candidate Regions for start bias - LUA");
					local choose_this_region = candidate_regions[diceroll];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[iPlayerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[iPlayerNum] = choose_this_region
					print("Player Number", iPlayerNum, "with single Region Priority assigned to Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[iPlayerNum + 1] = true;
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				else
					table.insert(fallback_priority, data)
					iNumNeedFallbackPriority = iNumNeedFallbackPriority + 1;
					--print("Player Number", iPlayerNum, "with single Region Priority was UNABLE to be matched to its type. Added to fallback list.");
				end
			end
		end
		-- Multiple priority civs go next, with fewest regions of priority going first.
		if iNumMultiPriority > 0 then
			for iNumPriorities = 2, 8 do						-- Must expand if new region types are added.
				for loop, data in ipairs(multi_priority) do
					if data[2] == iNumPriorities then
						--print("Adding Player#", data[1], "to sorted list of multi Region Priority.");
						table.insert(multi_sorted, data);
					end
				end
			end
			-- Match civs who have mulitple Region Priority to one of the region types they need, if possible.
			for loop, data in ipairs(multi_sorted) do
				local iPlayerNum = data[1];
				local iNumPriorityTypes = data[2];
				--print("* Attempting to assign Player#", iPlayerNum, "to one of its Priority Region Types.");
				local bFoundCandidate, candidate_regions = false, {};
				for test_loop, region_number in ipairs(regions_still_available) do
					for inner_loop = 1, iNumPriorityTypes do
						local region_type_to_test = priority_lists[iPlayerNum][inner_loop];
						if self.regionTypes[region_number] == region_type_to_test then
							table.insert(candidate_regions, region_number);
							bFoundCandidate = true;
							--print("- - Found candidate: Region#", region_number);
						end
					end
				end
				if bFoundCandidate then
					local diceroll = 1 + Map.Rand(table.maxn(candidate_regions), "Choosing from among Candidate Regions for start bias - LUA");
					local choose_this_region = candidate_regions[diceroll];
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[iPlayerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[iPlayerNum] = choose_this_region
					--print("Player Number", iPlayerNum, "with multiple Region Priority assigned to Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[iPlayerNum + 1] = true;
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				--else
					--print("Player Number", iPlayerNum, "with multiple Region Priority was unable to be matched.");
				end
			end
		end
		-- Fallbacks are done (if needed) after multiple-region priority is handled. The list is pre-sorted.
		if iNumNeedFallbackPriority > 0 then
			for loop, data in ipairs(fallback_priority) do
				local iPlayerNum = data[1];
				local iPriorityType = data[2];
				print("* Attempting to assign Player#", iPlayerNum, "to a fallback region as similar as possible to Region Type#", iPriorityType);
				local choose_this_region = self:FindFallbackForUnmatchedRegionPriority(iPriorityType, regions_still_available)
				if choose_this_region == -1 then
					--print("FAILED to find fallback region bias for player#", iPlayerNum);
				else
					local x = self.startingPlots[choose_this_region][1];
					local y = self.startingPlots[choose_this_region][2];
					local plot = Map.GetPlot(x, y);
					local player = Players[iPlayerNum];
					player:SetStartingPlot(plot);
					self.player_to_region[iPlayerNum] = choose_this_region
					--print("Player Number", iPlayerNum, "with single Region Priority assigned to FALLBACK Region#", choose_this_region, "at Plot", x, y);
					region_status[choose_this_region] = true;
					civ_status[iPlayerNum + 1] = true;
					local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
					if a then
						table.remove(regions_still_available, c[1]);
					end
				end
			end
		end
	end
	
	-- Handle Region Avoid
	if iNumAvoidCivs > 0 then
		--print("-"); print("-"); print("--- REGION AVOID READOUT ---"); print("-");
		local avoid_sorted, avoid_unsorted, avoid_counts = {}, {}, {};
		-- Sort list of civs with Avoid needs, then process in reverse order, so most needs goes first.
		for playerNum, avoid_needs in pairs(avoid_lists) do
			local len = table.maxn(avoid_needs)
			--print("- Player#", playerNum, "has this number of Region Avoid needs:", len);
			local avoid_data = {playerNum, len};
			table.insert(avoid_unsorted, avoid_data)
			table.insert(avoid_counts, len)
		end
		table.sort(avoid_counts)
		for loop, avoid_count in ipairs(avoid_counts) do
			for test_loop, avoid_data in ipairs(avoid_unsorted) do
				if avoid_count == avoid_data[2] then
					table.insert(avoid_sorted, avoid_data[1])
					table.remove(avoid_unsorted, test_loop)
				end
			end
		end
		-- Process the Region Avoid needs.
		for loop = iNumAvoidCivs, 1, -1 do
			local iPlayerNum = avoid_sorted[loop];
			local candidate_regions = {};
			for test_loop, region_number in ipairs(regions_still_available) do
				local bFoundCandidate = true;
				for inner_loop, region_type_to_avoid in ipairs(avoid_lists[iPlayerNum]) do
					if self.regionTypes[region_number] == region_type_to_avoid then
						bFoundCandidate = false;
					end
				end
				if bFoundCandidate == true then
					table.insert(candidate_regions, region_number);
					--print("- - Found candidate: Region#", region_number)
				end
			end
			if table.maxn(candidate_regions) > 0 then
				local diceroll = 1 + Map.Rand(table.maxn(candidate_regions), "Choosing from among Candidate Regions for start bias - LUA");
				local choose_this_region = candidate_regions[diceroll];
				local x = self.startingPlots[choose_this_region][1];
				local y = self.startingPlots[choose_this_region][2];
				local plot = Map.GetPlot(x, y);
				local player = Players[iPlayerNum];
				player:SetStartingPlot(plot);
				self.player_to_region[iPlayerNum] = choose_this_region
				--print("Player Number", iPlayerNum, "with Region Avoid assigned to allowed region type in Region#", choose_this_region, "at Plot", x, y);
				region_status[choose_this_region] = true;
				civ_status[iPlayerNum + 1] = true;
				local a, b, c = IdentifyTableIndex(regions_still_available, choose_this_region)
				if a then
					table.remove(regions_still_available, c[1]);
				end
			--else
				--print("Player Number", iPlayerNum, "with Region Avoid was unable to avoid the undesired region types.");
			end
		end
	end
				
	-- Assign remaining civs to start plots.
	local playerList, regionList = {}, {};
	for loop = 1, self.iNumCivs do
		local player_ID = self.player_ID_list[loop];
		if civ_status[player_ID + 1] == false then -- Using C++ player ID, which starts at zero. Add 1 for Lua indexing.
			table.insert(playerList, player_ID);
		end
		if region_status[loop] == false then
			table.insert(regionList, loop);
		end
	end
	local iNumRemainingPlayers = table.maxn(playerList);
	local iNumRemainingRegions = table.maxn(regionList);
	if iNumRemainingPlayers > 0 or iNumRemainingRegions > 0 then
		--print("-"); print("Table of players with no start bias:");
		--PrintContentsOfTable(playerList);
		--print("-"); print("Table of regions still available after bias handling:");
		--PrintContentsOfTable(regionList);
		if iNumRemainingPlayers ~= iNumRemainingRegions then
			print("-"); print("ERROR: Number of civs remaining after handling biases does not match number of regions remaining!"); print("-");
		end
		local playerListShuffled = GetShuffledCopyOfTable(playerList)
		for index, player_ID in ipairs(playerListShuffled) do
			local region_number = regionList[index];
			local x = self.startingPlots[region_number][1];
			local y = self.startingPlots[region_number][2];
			--print("Now placing Player#", player_ID, "in Region#", region_number, "at start plot:", x, y);
			local start_plot = Map.GetPlot(x, y)
			local player = Players[player_ID]
			player:SetStartingPlot(start_plot)
			self.player_to_region[player_ID] = region_number
		end
	end

	-- If this is a team game (any team has more than one Civ in it) then make 
	-- sure team members start near each other if possible. (This may scramble 
	-- Civ biases in some cases, but there is no cure).
	if self.bTeamGame == true then
		self:NormalizeTeamLocations()
	end
	--	
end


--BETA adjusting penality for non-areaID stuff

function AssignStartingPlots:MeasureStartPlacementFertilityOfLandmass(iAreaID, iWestX, iEastX, iSouthY, iNorthY, wrapsX, wrapsY)
	-- This function is designed to provide initial data for regional division recursion.
	-- Loop through plots in this landmass and measure Fertility Rating.
	-- Results will include a data table of all plots within the rectangle that includes the entirety of this landmass.
	--
	-- This function will account for any wrapping around the world this landmass may do.
	-- local iW, iH = Map.GetGridSize()
	local xEnd, yEnd; --[[ These coordinates will be used in case of wrapping landmass, 
	                       extending the landmass "off the map", in to imaginary space 
	                       to process it. Modulo math will correct the coordinates for 
	                       accessing the plot data array. ]]--
	if wrapsX then
		xEnd = iEastX + iW;
	else
		xEnd = iEastX;
	end
	if wrapsY then
		yEnd = iNorthY + iH;
	else
		yEnd = iNorthY;
	end
	--
	local areaFertilityTable = {};
	local areaFertilityCount = 0;
	local plotCount = 0;
	for yLoop = iSouthY, yEnd do -- When generating a plot data table incrementally, process Y first so that plots go row by row.
		for xLoop = iWestX, xEnd do
			plotCount = plotCount + 1;
			local x = xLoop % iW;
			local y = yLoop % iH;
			local plot = Map.GetPlot(x, y);
			local thisPlotsArea = plot:GetArea()
			if thisPlotsArea ~= iAreaID then -- This plot is not a member of the landmass, set value to 0
				table.insert(areaFertilityTable, 0);
			else -- This plot is a member, process it.
				local plotFertility = self:MeasureStartPlacementFertilityOfPlot(x, y, true); -- Check for coastal land is enabled.
				table.insert(areaFertilityTable, plotFertility);
				areaFertilityCount = areaFertilityCount + plotFertility;
			end
		end
	end
	
	-- Note: The table accounts for world wrap, so make sure to translate its index correctly.
	-- Plots in the table run from the southwest corner along the bottom row, then upward row by row, per normal plot data indexing.
	return areaFertilityTable, areaFertilityCount, plotCount
end

-- incorporates option for increasing coal nodes

function AssignStartingPlots:GetMajorStrategicResourceQuantityValues()
	-- This function determines quantity per tile for each strategic resource's major deposit size.
	-- Note: scripts that cannot place Oil in the sea need to increase amounts on land to compensate.
	local uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = 2, 4, 7, 6, 7, 8;
	-- Check the resource setting.
	if self.resource_setting == 1 or self.resource_setting == 2 then -- Sparse
		uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = 2, 2, 5, 4, 5, 6;
	elseif self.resource_setting == 3 then -- mediocre
		uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = 2, 3, 6, 5, 6, 7;
	elseif self.resource_setting == 7 then -- plenty
		uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = 2, 5, 8, 7, 8, 9;
	elseif self.resource_setting == 8 or self.resource_setting == 9 or self.resource_setting == 10 then -- Abundant
		uran_amt, horse_amt, oil_amt, iron_amt, coal_amt, alum_amt = 2, 6, 9, 8, 9, 10;
	end
	return uran_amt, horse_amt, oil_amt, iron_amt, coal_amt-1+beta_more_coal, alum_amt
end

-- function that severs tectonic pensinsulas that are too thin and adds all the shallows

function ZonePinches()
	local blocking_map = {}
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH)
			local plot = Map.GetPlot(x, y)
			local ptype = plot:GetPlotType()
			if ptype == PlotTypes.PLOT_OCEAN then
				blocking_map[i] = -1
			elseif ptype == PlotTypes.PLOT_MOUNTAIN then
				blocking_map[i] = 1
			else
				blocking_map[i] = 0
			end
		end
	end

	-- gets blob mountain ranges
	-- local converts = {}
	local blob_graph, blobs = get_blobs(blocking_map)

	for loop, blob in ipairs(blobs) do
		if tablelength(blob) > 1 then
			local coastal_check = false
			local inland_check = false
			local convert_canidates = {}
			for i, dumby in pairs(blob) do
				local x, y = i_to_xy(i, iW, iH)
				local plot = Map.GetPlot(x, y)
				if plot:IsCoastalLand(50) then
					coastal_check = true
					convert_canidates[i] = true
				else
					inland_check = true
				end
			end
			if inland_check and coastal_check then
				for i, dumby in pairs(convert_canidates) do
					local x, y = i_to_xy(i, iW, iH)
					local plot = Map.GetPlot(x, y)
					plot:SetPlotType(PlotTypes.PLOT_OCEAN)
					BETA_TECTONIC_LANDS[i] = 4
				end
			end
		end
	end

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH)
			local plot = Map.GetPlot(x, y)
			if BETA_TECTONIC_LANDS[i] > 0 and plot:GetPlotType() == PlotTypes.PLOT_OCEAN and beta_using_lekmod and plot:GetTerrainType() == TerrainTypes.TERRAIN_COAST then
				local adj_is = adj_is_cache[i]
				local land_adjacent = false
				for loop, adj_i in ipairs(adj_is) do
					local adj_x, adj_y = i_to_xy(adj_i, iW, iH)
					local adj_plot = Map.GetPlot(adj_x, adj_y)
					if adj_plot ~= nil then
						if adj_plot:GetPlotType() ~= PlotTypes.PLOT_OCEAN then
							land_adjacent = true
						end
					end
				end
				if land_adjacent then
					plot:SetFeatureType(FeatureTypes.FEATURE_SHALLOWS)
				end
			end
		end
	end
end

-- places shallow resource impacts

function ZoneSets(asp)
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH)
			local plot = Map.GetPlot(x, y)
			if BETA_TECTONIC_LANDS[i] > 0 and plot:GetPlotType() == PlotTypes.PLOT_OCEAN then
				asp:PlaceResourceImpact(x, y, 1, 0)
				asp:PlaceResourceImpact(x, y, 2, 0)
				asp:PlaceResourceImpact(x, y, 3, 0)
				asp:PlaceResourceImpact(x, y, 4, 0)
				asp:PlaceResourceImpact(x, y, 5, 0)
				asp:PlaceResourceImpact(x, y, 6, 0)
				asp:PlaceResourceImpact(x, y, 7, 0)
				asp:PlaceResourceImpact(x, y, 8, 0)
			end
		end
	end
end

------------------------------------------------------------------------------
function StartPlotSystem()



	local RegionalMethod = 1;

	-- Get Resources setting input by user.
	local AllowInlandSea = Map.GetCustomOption(17)
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

	if beta_tectonic_mounts == 2 then
		ZonePinches()
		Map.RecalculateAreas()
	end

	print("Creating start plot database.");
	local start_plot_database = AssignStartingPlots.Create()

	     start_plot_database._lek_prioritize_center = true
	     -- _lek_stronger_bias 
	     start_plot_database.centerBias = 20
	     start_plot_database.middleBias = 50
	     -- 
	     start_plot_database._lek_collide_coastals = true
		-- Interacts with CoastLux, makes that option undefined -- however true/false just marks guarantee/random
		-- CoastLux = false
		start_plot_database._lek_coastal_refish = false
	
	if beta_tectonic_mounts == 2 then
		ZoneSets(start_plot_database)
	end

	print("Dividing the map in to Regions.");
	-- Regional Division Method 1: Biggest Landmass
	local args = {
		method = RegionalMethod,
		start_locations = starts,
		resources = res,
		AllowInlandSea = AllowInlandSea,
		CoastLux = CoastLux,
		NoCoastInland = OnlyCoastal,
		BalancedCoastal = BalancedCoastal,
		MixedBias = MixedBias;
		};

	start_plot_database:GenerateRegions(args)




	print("Choosing start locations for civilizations.");
	
	-- takes some time
	start_plot_database:ChooseLocations()
	
	print("Normalizing start locations and assigning them to Players.");
	start_plot_database:BalanceAndAssign(args)

	print("Placing Natural Wonders.");
	local wonders = Map.GetCustomOption(7)
	if wonders == 14 then
		wonders = Map.Rand(13, "Number of Wonders To Spawn - Lua");
	elseif wonders == 15 then
		wonders = Map.Rand(3, "") + 3
	elseif wonders == 16 then
		wonders = Map.Rand(5, "") + 2
	else
		wonders = wonders - 1;
	end

	print("########## Wonders ##########");
	print("Natural Wonders To Place: ", wonders);

	local wonderargs = {
		wonderamt = wonders,
	};
	start_plot_database:PlaceNaturalWonders(wonderargs);

	-- local beta_meteors = Map.GetCustomOption(26);
	-- if beta_meteors == 2 then
	-- 	ThrowMeteors(start_plot_database, 1);
	-- elseif beta_meteors == 3 then
	-- 	ThrowMeteors(start_plot_database, 3);
	-- elseif beta_meteors == 4 then
	-- 	ThrowMeteors(start_plot_database, 5);
	-- elseif beta_meteors == 5 then
	-- 	ThrowMeteors(start_plot_database, 7);
	-- end

	print("Placing Resources and City States.");
	local accept_map = start_plot_database:PlaceResourcesAndCityStates();

	return accept_map, start_plot_database.region_luxury_assignment, start_plot_database.player_to_region
	

end


-- v44 cache cs starts and then set them at the end

function AssignStartingPlots:PlaceCityStateInRegion(city_state_number, region_number)
	--print("Place City State in Region called for City State", city_state_number, "Region", region_number);
	local iW, iH = Map.GetGridSize();
	local placed_city_state = false;
	local reached_middle = false;
	local region_data_table = self.regionData[region_number];
	local iWestX = region_data_table[1];
	local iSouthY = region_data_table[2];
	local iWidth = region_data_table[3];
	local iHeight = region_data_table[4];
	local iAreaID = region_data_table[5];
	
	local eligible_coastal, eligible_inland = {}, {};
	
	-- Main loop, first pass, unforced
	local x, y;
	local curWX = iWestX;
	local curSY = iSouthY;
	local curWid = iWidth;
	local curHei = iHeight;
	while placed_city_state == false and reached_middle == false do
		-- Send the remaining unprocessed portion of the region to be processed.
		local nextWX, nextSY, nextWid, nextHei;
		eligible_coastal, eligible_inland, nextWX, nextSY, nextWid, nextHei, 
		  reached_middle = self:ObtainNextSectionInRegion(curWX, curSY, curWid, curHei, iAreaID, false, false) -- Don't force it. Yet.
		curWX, curSY, curWid, curHei = nextWX, nextSY, nextWid, nextHei;
		-- Attempt to place city state using the two plot lists received from the last call.
		x, y, placed_city_state = self:PlaceCityState(eligible_coastal, eligible_inland, false, false) -- Don't need to re-check collisions.
	end
	
	-- Disabling all fallback methods of city state placement. Jon has decided that, rather than
	-- force city states in to locations where they cannot even settle, we will discard them instead.
	--
	-- I am leaving the fallback methods in the code, but disabled, in case they are of any use to modders. - BT

	--[[
	if placed_city_state == false then -- Failed with proximity checks in play. Drop the prox check and force it.
		-- Main loop, second pass, forced
		reached_middle = false;
		local curWX = iWestX;
		local curSY = iSouthY;
		local curWid = iWidth;
		local curHei = iHeight;
		while placed_city_state == false and reached_middle == false do
			-- Send the remaining unprocessed portion of the region to be processed.
			local nextWX, nextSY, nextWid, nextHei;
			eligible_coastal, eligible_inland, nextWX, nextSY, nextWid, nextHei, 
			  reached_middle = self:ObtainNextSectionInRegion(curWX, curSY, curWid, curHei, iAreaID, true, false) -- Force it, but not on top of an already placed player.
			curWX, curSY, curWid, curHei = nextWX, nextSY, nextWid, nextHei;
			-- Attempt to place city state using the two plot lists received from the last call.
			x, y, placed_city_state = self:PlaceCityState(eligible_coastal, eligible_inland, false, false) -- Don't need to re-check collisions.
		end
	end

	
	if placed_city_state == false then -- Failed even trying to force it. Now allow the CS to be placed on top of another.
		-- Main loop, third pass, forced with collision checks completely disabled.
		reached_middle = false;
		local curWX = iWestX;
		local curSY = iSouthY;
		local curWid = iWidth;
		local curHei = iHeight;
		while placed_city_state == false and reached_middle == false do
			-- Send the remaining unprocessed portion of the region to be processed.
			local nextWX, nextSY, nextWid, nextHei;
			eligible_coastal, eligible_inland, nextWX, nextSY, nextWid, nextHei, 
			  reached_middle = self:ObtainNextSectionInRegion(curWX, curSY, curWid, curHei, iAreaID, true, true) -- Force it any way you can.
			curWX, curSY, curWid, curHei = nextWX, nextSY, nextWid, nextHei;
			-- Attempt to place city state using the two plot lists received from the last call.
			x, y, placed_city_state = self:PlaceCityState(eligible_coastal, eligible_inland, false, false) -- Don't need to re-check collisions.
		end
	end

	if placed_city_state == false then -- Getting desperate to place this city state.
		local fallback_plots, fallback_scores, best_fallback_plots, best_fallback_score = {}, {}, {}, 99999999;
		for region_loop_y = 0, iHeight - 1 do
			for region_loop_x = 0, iWidth - 1 do
				local x = (region_loop_x + iWestX) % iW;
				local y = (region_loop_y + iSouthY) % iH;
				local plotIndex = y * iW + x + 1;
				local plot = Map.GetPlot(x, y);
				local plotType = plot:GetPlotType()
				local terrainType = plot:GetTerrainType()
				local featureType = plot:GetFeatureType()
				--
				local iPlotScore = 1 + self.cityStateData[plotIndex];
				if self.playerCollisionData[plotIndex] == true then
					iPlotScore = iPlotScore * 1000;
				end
				if plotType == PlotTypes.PLOT_OCEAN then
					iPlotScore = iPlotScore * 10;
				elseif plotType == PlotTypes.PLOT_MOUNTAIN then
					iPlotScore = iPlotScore * 2;
				elseif terrainType == TerrainTypes.TERRAIN_SNOW then
					iPlotScore = iPlotScore * 3;
				end
				table.insert(fallback_plots, plotIndex);
				table.insert(fallback_scores, iPlotScore);
			end
		end
		for loop, iPlotScore in ipairs(fallback_scores) do
			if iPlotScore < best_fallback_score then
				best_fallback_score = iPlotScore;
			end
		end
		for loop, iPlotScore in ipairs(fallback_scores) do
			if iPlotScore == best_fallback_score then
				table.insert(best_fallback_plots, fallback_plots[loop]);
			end
		end
		local iNumFallbackCandidates = table.maxn(best_fallback_plots);
		local selectedPlotIndex;
		if iNumFallbackCandidates > 0 then
			local diceroll = 1 + Map.Rand(iNumFallbackCandidates, "City State Placement fallback plot - Lua");
			selectedPlotIndex = best_fallback_plots[diceroll];
			x = (selectedPlotIndex - 1) % iW;
			y = (selectedPlotIndex - x - 1) / iW;
			placed_city_state = true;
			local plot = Map.GetPlot(x, y);
			local plotType = plot:GetPlotType()
			if plotType == PlotTypes.PLOT_OCEAN or plotType == PlotTypes.PLOT_MOUNTAIN then
				plot:SetPlotType(PlotTypes.PLOT_LAND, false, false)
			end
			plot:SetTerrainType(TerrainTypes.TERRAIN_PLAINS, false, true)
			plot:SetFeatureType(FeatureTypes.NO_FEATURE, -1)
			print("-"); print("Forced placement on emergency fallback plot for City State #", city_state_number); print("-");
		else
			print("ERROR: Can't find any water, mountains, or land in this region. ... Yup, it's bad.");
		end
	end
	]]--

	if placed_city_state == true then
		-- Record and enact the placement.
		self.cityStatePlots[city_state_number] = {x, y, region_number};
		self.city_state_validity_table[city_state_number] = true; -- This is the line that marks a city state as valid to be processed by the rest of the system.
		local city_state_ID = city_state_number + GameDefines.MAX_MAJOR_CIVS - 1;
		local cityState = Players[city_state_ID];
		local cs_start_plot = Map.GetPlot(x, y)
		-- cityState:SetStartingPlot(cs_start_plot)
		cs_starts[city_state_ID] = cs_start_plot
		self:GenerateLuxuryPlotListsAtCitySite(x, y, 1, true) -- Removes Feature Ice from coasts adjacent to the city state's new location
		self:PlaceResourceImpact(x, y, 5, 4) -- City State layer
		self:PlaceResourceImpact(x, y, 2, 3) -- Luxury layer
		self:PlaceResourceImpact(x, y, 1, 0) -- Strategic layer, at start point only.
		self:PlaceResourceImpact(x, y, 3, 3) -- Bonus layer
		self:PlaceResourceImpact(x, y, 4, 3) -- Fish layer
		self:PlaceResourceImpact(x, y, 7, 3) -- Marble layer

		local impactPlotIndex = y * iW + x + 1;
		self.playerCollisionData[impactPlotIndex] = true;
		--print("-"); print("City State", city_state_number, "has been started at Plot", x, y, "in Region#", region_number);
	else
		--print("-"); print("WARNING: Crowding issues for City State #", city_state_number, " - Could not find valid site in Region#", region_number);
		self.iNumCityStatesDiscarded = self.iNumCityStatesDiscarded + 1;
	end
end

--caches CS locations instead of places them so that they can be placed after rerolls since they can't be unset
------------------------------------------------------------------------------
function AssignStartingPlots:PlaceCityStates()
	print("Map Generation - Choosing sites for City States");
	-- This function is dependent on AssignLuxuryRoles() having been executed first.
	-- This is because some city state placements are made in compensation for drawing
	-- the short straw in regard to multiple regions being assigned the same luxury type.

	self:AssignCityStatesToRegionsOrToUninhabited()
	
	--print("-"); print("--- City State Placement Results ---");

	local iW, iH = Map.GetGridSize();
	local iUninhabitedCandidatePlots = table.maxn(self.uninhabited_areas_coastal_plots) + table.maxn(self.uninhabited_areas_inland_plots);
	--print("-"); print("."); print(". NUMBER OF UNINHABITED CS CANDIDATE PLOTS: ", iUninhabitedCandidatePlots); print(".");
	for cs_number, region_number in ipairs(self.city_state_region_assignments) do
		if cs_number <= self.iNumCityStates then -- Make sure it's an active city state before processing.
			if region_number == -1 and iUninhabitedCandidatePlots > 0 then -- Assigned to areas outside of Regions.
				--print("Place City States, place in uninhabited called for City State", cs_number);
				iUninhabitedCandidatePlots = iUninhabitedCandidatePlots - 1;
				local cs_x, cs_y, success;
				cs_x, cs_y, success = self:PlaceCityState(self.uninhabited_areas_coastal_plots, self.uninhabited_areas_inland_plots, true, true)
				--
				-- Disabling fallback methods that remove proximity and collision checks. Jon has decided
				-- that city states that do not fit on the map will simply not be placed, but instead discarded.
				--[[
				if not success then -- Try again, this time with proximity checks disabled.
					cs_x, cs_y, success = self:PlaceCityState(self.uninhabited_areas_coastal_plots, self.uninhabited_areas_inland_plots, false, true)
					if not success then -- Try a third time, this time with all collision checks disabled.
						cs_x, cs_y, success = self:PlaceCityState(self.uninhabited_areas_coastal_plots, self.uninhabited_areas_inland_plots, false, false)
					end
				end
				]]--
				--
				if success == true then
					self.cityStatePlots[cs_number] = {cs_x, cs_y, -1};
					self.city_state_validity_table[cs_number] = true; -- This is the line that marks a city state as valid to be processed by the rest of the system.
					local city_state_ID = cs_number + GameDefines.MAX_MAJOR_CIVS - 1;
					local cityState = Players[city_state_ID];
					local cs_start_plot = Map.GetPlot(cs_x, cs_y)
					-- cityState:SetStartingPlot(cs_start_plot)
					cs_starts[city_state_ID] = cs_start_plot
					self:GenerateLuxuryPlotListsAtCitySite(cs_x, cs_y, 1, true) -- Removes Feature Ice from coasts adjacent to the city state's new location
					self:PlaceResourceImpact(cs_x, cs_y, 5, 3) -- City State layer
					self:PlaceResourceImpact(cs_x, cs_y, 2, 3) -- Luxury layer
					self:PlaceResourceImpact(cs_x, cs_y, 1, 0) -- Strategic layer, at start point only.
					self:PlaceResourceImpact(cs_x, cs_y, 3, 3) -- Bonus layer
					self:PlaceResourceImpact(cs_x, cs_y, 4, 3) -- Fish layer
					self:PlaceResourceImpact(cs_x, cs_y, 7, 3) -- Marble layer

					local impactPlotIndex = cs_y * iW + cs_x + 1;
					self.playerCollisionData[impactPlotIndex] = true;
					--print("-"); print("City State", cs_number, "has been started at Plot", cs_x, cs_y, "in Uninhabited Lands");
				else
					--print("-"); print("WARNING: Crowding issues for City State #", city_state_number, " - Could not find valid site in Uninhabited Lands.", region_number);
					self.iNumCityStatesDiscarded = self.iNumCityStatesDiscarded + 1;
				end
			elseif region_number == -1 and iUninhabitedCandidatePlots <= 0 then -- Assigned to areas outside of Regions, but nowhere there to put them!
				local iRandRegion = 1 + Map.Rand(self.iNumCivs, "Emergency Redirect of CS placement, choosing Region - LUA");
				--print("Place City States, place in uninhabited called for City State", cs_number, "but it has no legal site, so is being put in Region#", iRandRegion);
				self:PlaceCityStateInRegion(cs_number, iRandRegion)
			else -- Assigned to a Region.
				--print("Place City States, place in Region#", region_number, "for City State", cs_number);
				self:PlaceCityStateInRegion(cs_number, region_number)
			end
		end
	end
	
	-- Last chance method to place city states that didn't fit where they were supposed to go.
	if self.iNumCityStatesDiscarded > 0 then
		-- Assemble a global plot list of eligible City State sites that remain.
		local cs_last_chance_plot_list = {};
		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				if self:CanPlaceCityStateAt(x, y, -1, false, false) == true then
					local i = y * iW + x + 1;
					table.insert(cs_last_chance_plot_list, i);
				end
			end
		end
		local iNumLastChanceCandidates = table.maxn(cs_last_chance_plot_list);
		-- If any eligible sites were found anywhere on the map, place as many of the remaining CS as possible.
		if iNumLastChanceCandidates > 0 then
			print("-"); print("-"); print("ALERT: Some City States failed to be placed due to overcrowding. Attempting 'last chance' placement method.");
			print("Total number of remaining eligible candidate plots:", iNumLastChanceCandidates);
			local last_chance_shuffled = GetShuffledCopyOfTable(cs_last_chance_plot_list)
			local cs_list = {};
			for cs_num = 1, self.iNumCityStates do
				if self.city_state_validity_table[cs_num] == false then
					table.insert(cs_list, cs_num);
					--print("City State #", cs_num, "not yet placed, adding it to 'last chance' list.");
				end
			end
			for loop, cs_number in ipairs(cs_list) do
				local cs_x, cs_y, success;
				cs_x, cs_y, success = self:PlaceCityState(last_chance_shuffled, {}, true, true)
				if success == true then
					self.cityStatePlots[cs_number] = {cs_x, cs_y, -1};
					self.city_state_validity_table[cs_number] = true; -- This is the line that marks a city state as valid to be processed by the rest of the system.
					local city_state_ID = cs_number + GameDefines.MAX_MAJOR_CIVS - 1;
					local cityState = Players[city_state_ID];
					local cs_start_plot = Map.GetPlot(cs_x, cs_y)
					-- cityState:SetStartingPlot(cs_start_plot)
					cs_starts[city_state_ID] = cs_start_plot
					self:GenerateLuxuryPlotListsAtCitySite(cs_x, cs_y, 1, true) -- Removes Feature Ice from coasts adjacent to the city state's new location
					self:PlaceResourceImpact(cs_x, cs_y, 5, 3) -- City State layer
					self:PlaceResourceImpact(cs_x, cs_y, 2, 3) -- Luxury layer
					self:PlaceResourceImpact(cs_x, cs_y, 1, 0) -- Strategic layer, at start point only.
					self:PlaceResourceImpact(cs_x, cs_y, 3, 3) -- Bonus layer
					self:PlaceResourceImpact(cs_x, cs_y, 4, 3) -- Fish layer
					self:PlaceResourceImpact(cs_x, cs_y, 7, 3) -- Marble layer

					local impactPlotIndex = cs_y * iW + cs_x + 1;
					self.playerCollisionData[impactPlotIndex] = true;
					self.iNumCityStatesDiscarded = self.iNumCityStatesDiscarded - 1;
					--print("-"); print("City State", cs_number, "has been RESCUED from the trash bin of history and started at Fallback Plot", cs_x, cs_y);
				else
					--print("-"); print("We have run out of possible 'last chance' sites for unplaced city states!");
					break
				end
			end
			if self.iNumCityStatesDiscarded > 0 then
				print("-"); print("ALERT: No eligible city state sites remain. DISCARDING", self.iNumCityStatesDiscarded, "city states. BYE BYE!"); print("-");
			end
		else
			print("-"); print("-"); print("ALERT: No eligible city state sites remain. DISCARDING", self.iNumCityStatesDiscarded, "city states. BYE BYE!"); print("-");
		end
	end
end

-- nested function to allow in-game error reporting

function GenMap()

	local accept_map = false
	local attempts = 0
	local region_luxury_assignment = {}
	local iNumCivs, iNumCityStates, player_ID_list, bTeamGame, teams_with_major_civs, number_civs_per_team = GetPlayerAndTeamInfo()

	-- local iW, iH = Map.GetGridSize();
	local attempt_cap = 15
	-- local beta_tectonic_mounts = Map.GetCustomOption(45)
	local beta_tectonic_mounts = 2
	-- local beta_reroll_count = Map.GetCustomOption(47)
	beta_reroll_count = 4

	if beta_reroll_count == 2 then
		attempt_cap = 50
	elseif beta_reroll_count == 3 then
		attempt_cap = 100
	elseif beta_reroll_count == 4 then
		attempt_cap = 300
	elseif beta_reroll_count == 5 then
		attempt_cap = 99999
	end

	local time = os.clock()
	local elapsed_time = 0

	while accept_map == false and attempts < attempt_cap do
		attempts = attempts + 1
		print("Generating Map");
		-- This is the core map generation function.
		-- Every step in this process carries dependencies upon earlier steps.
		-- There isn't any way to change the order of operations without breaking dependencies,
		-- although it would be possible to repair and reorganize certain dependencies with enough work.
		
		-- Plot types are the core layer of the map, determining land or sea, determining flatland, hills or mountains.


		--reset globals across other function
		-- AssignStartingPlots = {};
		-- FeatureGenerator = {};
		-- TerrainGenerator = {};
		nextRiverID = 0;
		_rivers = {};

		--wipe map

		coast_cache = {};
		has_cached_coast = false;

		cs_starts = {}


		for y = 0, iH - 1 do
			for x = 0, iW - 1 do
				local plot = Map.GetPlot(x, y);
				plot:SetFeatureType(FeatureTypes.NO_FEATURE);
				plot:SetResourceType(-1, -1);
				plot:SetImprovementType(-1);
				plot:SetNWOfRiver(false);
				plot:SetNEOfRiver(false);
				plot:SetWOfRiver(false);
			end
		end


		
		-- FractalWorld = {};


		GeneratePlotTypes();


		-- print("GOT HERE 1")
		
		-- Terrain covers climate: grassland, plains, desert, tundra, snow.
		GenerateTerrain();


		
		-- print("GOT HERE 2")
		
		
		-- Each body of water, area of mountains, or area of hills+flatlands is independently grouped and tagged.
		Map.RecalculateAreas();


		
		-- print("GOT HERE 3")

		
		-- River generation is affected by plot types, originating from highlands and preferring to traverse lowlands.
		AddRivers();

		

		-- print("GOT HERE 4")
		
		-- Lakes would interfere with rivers, causing them to stop and not reach the ocean, if placed any sooner.
		AddLakes();

		

		-- print("GOT HERE 5")
		
		-- Features depend on plot types, terrain types, rivers and lakes to help determine their placement.
		-- dump_map();
		AddFeatures();
		

		-- print("GOT HERE 6")

		-- Feature Ice is impassable and therefore requires another area recalculation.
		Map.RecalculateAreas();


		-- print("GOT HERE 7")

		-- Assign Starting Plots, Place Natural Wonders, and Distribute Resources.
		-- This system was designed and programmed for Civ5 by Bob Thomas.
		-- Starting plots are wholly dependent on all the previous elements being in place.
		-- Natural Wonders are dependent on civ starts being in place, to keep them far enough away.
		-- Resources are dependent on start locations, Natural Wonders, as well as plots, terrain, rivers, lakes and features.
		--
		-- This system relies on Area-based data and cannot tolerate an AreaID recalculation during its operations.
		-- Due to plot changes from Natural Wonders and possibly other source, another recalculation is done as the final action of the system.
		accept_map, region_luxury_assignment, player_to_region = StartPlotSystem();

		-- accept_map = false



		-- if accept_map == false and attempts < attempt_cap then
		-- 	for cs_number = 1, iNumCityStates do
		-- 		local city_state_ID = cs_number + GameDefines.MAX_MAJOR_CIVS - 1
		-- 		local cityState = Players[city_state_ID];
		-- 		-- print("CS NAME:")
		-- 		-- print(cityState:GetName())
		-- 		if cityState ~= nil then
		-- 			cityState:SetStartingPlot(Map.GetPlot(cs_number-1,0));
		-- 		end
		-- 	end
		-- end
	end

	local elapsed_time = os.clock()-time

	-- janky way to deal with unfit cs - make a mountain at plot 00

	-- for cs_number = 1, iNumCityStates do
	-- 	Map.GetPlot(cs_number-1,0):SetPlotType(PlotTypes.PLOT_MOUNTAIN);
	-- end
	for city_state_ID, cs_start_plot in pairs(cs_starts) do
		local cityState = Players[city_state_ID];
		cityState:SetStartingPlot(cs_start_plot)
	end

	--for good measure
	Map.RecalculateAreas();


	-- if iNumCivs <= 6 then
	-- 	for loop = 1, iNumCivs do
	-- 		local playerNum = player_ID_list[loop]; -- MP games can have gaps between player numbers, so we cannot assume a sequential set of IDs.
	-- 		local player = Players[playerNum];
	-- 		local plot = player:GetStartingPlot();
	-- 		local beta_handicap_player = Map.GetCustomOption(29+loop);
	-- 		if beta_handicap_player == 2 then
	-- 			player:InitUnit(82, plot:GetX(), plot:GetY());
	-- 		elseif beta_handicap_player == 3 then
	-- 			player:InitUnit(1, plot:GetX(), plot:GetY());
	-- 		elseif beta_handicap_player == 4 then
	-- 			player:InitUnit(6, plot:GetX(), plot:GetY());
	-- 		elseif beta_handicap_player == 5 then
	-- 			player:InitUnit(128, plot:GetX(), plot:GetY());
	-- 		elseif beta_handicap_player == 6 then
	-- 			local resource_ID = region_luxury_assignment[player_to_region[playerNum]];
	-- 			plot:SetResourceType(resource_ID, 6)
	-- 		end
	-- 	end
	-- end
	
	
	-- local beta_lake_fish = Map.GetCustomOption(37)
	-- local beta_buff_luxes = Map.GetCustomOption(45)
	-- local beta_meteors = Map.GetCustomOption(26);
	
	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			
			local plot = Map.GetPlot(x, y)
			local res_ID = plot:GetResourceType(-1)
			local featureType = plot:GetFeatureType()
			local terrainType = plot:GetTerrainType()
			local plotType = plot:GetPlotType()
			-- local resourceType = plot:GetResourceType();
			
			-- Mined/Quarried Resources
			if beta_lake_fish == 2 then
				if plot:IsLake() and res_ID == GameInfo.Resources.RESOURCE_FISH.ID then
					Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_FOOD, -1)
					Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
				end
			end

			-- if beta_buff_luxes == 2 then
			-- 	if res_ID == GameInfo.Resources.RESOURCE_TRUFFLES.ID or res_ID == GameInfo.Resources.RESOURCE_INCENSE.ID then
			-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, 1)
			-- 	end
			-- end

			-- if beta_meteors == 2 then
			-- 	if res_ID == GameInfo.Resources.RESOURCE_GEMS.ID and terrainType == TerrainTypes.TERRAIN_TUNDRA and plotType == PlotTypes.PLOT_HILLS then
			-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_GOLD, -1)
			-- 		Game.SetPlotExtraYield(x, y, YieldTypes.YIELD_SCIENCE, 2)
			-- 	end
			-- end	
		end
	end


	if accept_map then
		for id, player in pairs(Players) do
			player:AddNotification(NotificationTypes.NOTIFICATION_TRADE_ROUTE, "The map was made " .. tostring(attempts) .. " times across " .. string.format("%.3f", elapsed_time) .. " seconds. (" .. string.format("%.4f", elapsed_time/attempts) .. " Seconds/Map)", "Remakes")
		end
	else
		for id, player in pairs(Players) do
			player:AddNotification(NotificationTypes.NOTIFICATION_TRADE_ROUTE_BROKEN, "The map was made the Maximum (" .. attempt_cap .. ") number of times - using the last map made.", "Remakes Failed")
		end
	end

	-- Goodies depend on not colliding with resources or Natural Wonders, or being placed too near to start plots.
	AddGoodies();

	-- Continental artwork selection must wait until Areas are finalized, so it gets handled last.
	DetermineContinents();

	local beta_errors = Map.GetCustomOption(19)
	if beta_errors == 3 then
		local bad_var = {}
		if bad_var > 5 then
			print("THIS SHOULD NEVER WORK")
		end
	end
	-- dump_map();
end

function GenerateMap()

	beta_using_lekmod = false
	if GameInfo.Resources.RESOURCE_CORAL ~= nil then
		beta_using_lekmod = true
	end

	-- make these globals for performance
	-- beta_flexy_lux = Map.GetCustomOption(25); -- trying this outside for performance
	beta_flexy_lux = 2
	-- beta_no_ice = Map.GetCustomOption(29);
	beta_no_ice = 2
	-- beta_bay_diffusion =  Map.GetCustomOption(22);
	beta_bay_diffusion =  2
	ripple_decider = Map.GetCustomOption(6);
	beta_cliffs = Map.GetCustomOption(24);
	-- beta_cliffs = 2
	beta_coast_zone = Map.GetCustomOption(23);
	-- beta_coast_zone = 4
	-- beta_coastal_deadzone = Map.GetCustomOption(24);
	beta_coastal_deadzone = 4
	-- beta_more_coal = Map.GetCustomOption(42);
	beta_more_coal = 1
	-- beta_cs_placement = Map.GetCustomOption(48);
	beta_cs_placement = 2

	-- beta_tectonic_mounts = Map.GetCustomOption(45);
	beta_tectonic_mounts = 2
	-- beta_spawn_validation = Map.GetCustomOption(49);
	beta_spawn_validation = 2

	-- beta_lake_fish = Map.GetCustomOption(37)
	beta_lake_fish = 1


	beta_inland_prox_penalty = 2

	iW, iH = Map.GetGridSize();

	wrapX = Map:IsWrapX();
	wrapY = Map:IsWrapY();

	adj_is_cache = {}

	for y = 0, iH - 1 do
		for x = 0, iW - 1 do
			local i = xy_to_i(x, y, iW, iH);
			local adj_is = GetAdjacent(x, y);
			adj_is_cache[i] = adj_is;
		end
	end

	local beta_errors = Map.GetCustomOption(19)
	-- test a gen loop
	 local status, err = xpcall(GenMap, debug.traceback)
	 if status == false then
		print(err)
		if beta_errors ~= 1 then
			for id, player in pairs(Players) do
				player:AddNotification(NotificationTypes.NOTIFICATION_SPY_WAS_KILLED, err, "Error Report - Please Screenshot and Send to Jacobian.")
			end
		end
	else
		if beta_errors ~= 1 then
			for id, player in pairs(Players) do
				player:AddNotification(NotificationTypes.NOTIFICATION_SPY_RIG_ELECTION_SUCCESS, "The map completed generation. You are good to play. Thank you for playing Lekmap v6.0!", "The Map Script Completed Successfully")
			end
		end
	end
end

function FeatureGenerator:AddIceAtPlot(plot, iX, iY, lat)
	-- local beta_no_ice = Map.GetCustomOption(31);
	if(plot:CanHaveFeature(self.featureIce)) then
		if Map.IsWrapX() and (iY == 0 or iY == self.iGridH - 1) then
			if beta_no_ice == 1 then
				plot:SetFeatureType(self.featureIce, -1)
			elseif beta_no_ice == 2 and plot:GetTerrainType() ~= TerrainTypes.TERRAIN_COAST then
				plot:SetFeatureType(self.featureIce, -1)
			elseif beta_no_ice == 4 then
				plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT, -1)
			end
		else
			local rand = Map.Rand(200, "Add Ice Lua")/100.0;

			if(rand < 8 * (lat - 0.875)) then
				if beta_no_ice == 1 then
					plot:SetFeatureType(self.featureIce, -1)
				elseif beta_no_ice == 2 and plot:GetTerrainType() ~= TerrainTypes.TERRAIN_COAST then
					plot:SetFeatureType(self.featureIce, -1)
				elseif beta_no_ice == 4 then
					plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT, -1)
				end
			elseif(rand < 4 * (lat - 0.75)) then
				if beta_no_ice == 1 then
					plot:SetFeatureType(self.featureIce, -1)
				elseif beta_no_ice == 2 and plot:GetTerrainType() ~= TerrainTypes.TERRAIN_COAST then
					plot:SetFeatureType(self.featureIce, -1)
				elseif beta_no_ice == 4 then
					plot:SetFeatureType(FeatureTypes.FEATURE_FALLOUT, -1)
				end
			end
		end
	end
end

function AddRivers()
	local riverSourceRangeDefault = 1.5;
	local seaWaterRangeDefault = 0.75;
	local plotsPerRiverEdge =  9;
	local riverRnd = 210;
	
	if beta_tectonic_mounts == 2 then
		riverSourceRangeDefault = 1.48;
		seaWaterRangeDefault = 0.75;
		plotsPerRiverEdge =  9;
		riverRnd = 212;
	end

	local rivers_level = Map.GetCustomOption(9)

	if rivers_level == 1 then
		riverSourceRangeDefault = 3;
		seaWaterRangeDefault = 2;
		plotsPerRiverEdge =  12;
		riverRnd = 180;
	elseif rivers_level	== 3 then
		riverSourceRangeDefault = 1;
		seaWaterRangeDefault = 0.5;
		plotsPerRiverEdge =  8;
		riverRnd = 230;
	end

	print("Map Generation - Adding Rivers");
	
	local passConditions = {
		function(plot)
			return (plot:IsHills() or plot:IsMountain());
		end,
		
		function(plot)
			return (not plot:IsCoastalLand()) and (Map.Rand(8, "MapGenerator AddRivers") == 0);
		end,
		
		function(plot)
			local area = plot:Area();
			return (plot:IsHills() or plot:IsMountain()) and (area:GetNumRiverEdges() <	((area:GetNumTiles() / plotsPerRiverEdge) + 1));
		end,
		
		function(plot)
			local area = plot:Area();
			return (area:GetNumRiverEdges() < (area:GetNumTiles() / plotsPerRiverEdge) + 1);
		end
	}
	
	for iPass, passCondition in ipairs(passConditions) do
					
		if (iPass <= 2) then
			riverSourceRange = riverSourceRangeDefault;
			seaWaterRange = seaWaterRangeDefault;
		else
			riverSourceRange = (riverSourceRangeDefault / 2);
			seaWaterRange = (seaWaterRangeDefault / 2);
		end
			
		local iW, iH = Map.GetGridSize();

		for i = 0, (iW * iH) - 1, 1 do
			plot = Map.GetPlotByIndex(i);
			if(not plot:IsWater()) then
				if(passCondition(plot)) then -- and plot:IsNaturalWonder() == false and AdjacentToNaturalWonder(plot) == false) then
					if (not Map.FindWater(plot, riverSourceRange, true)) then
						if (not Map.FindWater(plot, seaWaterRange, false)) then
							local inlandCorner = plot:GetInlandCorner();
							if(inlandCorner) then -- and plot:IsNaturalWonder() == false and AdjacentToNaturalWonder(plot) == false) then
								local riverChance = Map.Rand(riverRnd, "MapGenerator AddRivers");
								if riverChance > 178 then
									DoRiver(inlandCorner);
								end
							end
						end
					end
				end			
			end
		end
	end		
end
