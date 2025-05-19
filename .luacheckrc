-- Luacheck configuration for Civ 5 modding
-- Allow global functions (common in Civ 5 Lua)
allow_defined_top = true

-- Ignore 'include' directive used by Civ 5
ignore = {"111", "113"} -- 111 = setting undefined global, 113 = accessing undefined global

-- Standard globals in Civ 5 Lua environment
globals = {
    "Game",
    "Map",
    "Players",
    "Teams",
    "GameInfoTypes",
    "GameEvents",
    "include",
    "GameDefines",
    "PlotAreaSweepIterator",
    -- Add any other Civ 5 globals here as needed
    "SECTOR_NORTH",
    "DIRECTION_CLOCKWISE",
    "DIRECTION_OUTWARDS",
    "CENTRE_EXCLUDE"
} 