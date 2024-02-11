function runConfig()
--[[

MOD.EAP: Here you can change the settings of the map script if you are not running a (correctly) modified database.

]]

-- In this config, for vanilla support, we assume the default resources and their values
local iron_ID = 0;
local horse_ID = 1;
local coal_ID = 2;
local oil_ID = 3;
local aluminum_ID = 4;
local uranium_ID = 5;



-- default strategic resource amounts for major deposits and minor deposits
Default_Stategic_Resource_Amounts_Major = {

    [iron_ID] = 6,
    [horse_ID] = 4,
    [coal_ID] = 7,
    [oil_ID] = 7,
    [aluminum_ID] = 8,
    [uranium_ID] = 2

};

Default_Stategic_Resource_Amounts_Minor = {

    [iron_ID] = 2,
    [horse_ID] = 2,
    [coal_ID] = 30,
    [oil_ID] = 4,
    [aluminum_ID] = 3,
    [uranium_ID] = 1
};


Default_Resource_Frequencies = {

    -- 0 recommended for strategics.
    [iron_ID] = 0,
    [horse_ID] = 0,
    [coal_ID] = 0,
    [oil_ID] = 0,
    [aluminum_ID] = 0,
    [uranium_ID] = 0

};

-- Default Values for Resource Preferences. Same structure as in the database. The Higher the preference value the less
-- likely it is to spawn on the specified terrain/feature.

Default_Resource_Preferences = {

    {ResourceType = "RESOURCE_URANIUM", FeatureType = "FEATURE_FOREST", PreferenceValue = 60},
    {ResourceType = "RESOURCE_URANIUM", FeatureType = "FEATURE_JUNGLE", PreferenceValue = 140},
    {ResourceType = "RESOURCE_URANIUM", FeatureType = "FEATURE_MARSH", PreferenceValue = 20},

    {ResourceType = "RESOURCE_OIL", TerrainType = "TERRAIN_DESERT", PreferenceValue = 28},
    {ResourceType = "RESOURCE_OIL", TerrainType = "TERRAIN_SNOW", PreferenceValue = 45},
    {ResourceType = "RESOURCE_OIL", TerrainType = "TERRAIN_TUNDRA", PreferenceValue = 50},
    {ResourceType = "RESOURCE_OIL", TerrainType = "FEATURE_MARSH", PreferenceValue = 15},
    {ResourceType = "RESOURCE_OIL", FeatureType = "FEATURE_FOREST", PreferenceValue = 140},

    {ResourceType = "RESOURCE_ALUMINUM", TerrainType = "TERRAIN_SNOW", PreferenceValue = 70},
    {ResourceType = "RESOURCE_ALUMINUM", TerrainType = "TERRAIN_TUNDRA", PreferenceValue = 80},
    {ResourceType = "RESOURCE_ALUMINUM", TerrainType = "TERRAIN_HILL", PreferenceValue = 50},

    {ResourceType = "RESOURCE_IRON", TerrainType = "TERRAIN_TUNDRA", PreferenceValue = 50},
    {ResourceType = "RESOURCE_IRON", TerrainType = "TERRAIN_SNOW", PreferenceValue = 75},
    {ResourceType = "RESOURCE_IRON", TerrainType = "TERRAIN_HILL", PreferenceValue = 88},
    {ResourceType = "RESOURCE_IRON", TerrainType = "TERRAIN_DESERT", PreferenceValue = 35},

    {ResourceType = "RESOURCE_COAL", FeatureType = "FEATURE_FOREST", PreferenceValue = 200},
    {ResourceType = "RESOURCE_COAL", FeatureType = "FEATURE_JUNGLE", PreferenceValue = 72},
    {ResourceType = "RESOURCE_COAL", TerrainType = "TERRAIN_HILL", PreferenceValue = 60},

    {ResourceType = "RESOURCE_HORSE", TerrainType = "TERRAIN_GRASS", PreferenceValue = 17},
    {ResourceType = "RESOURCE_HORSE", TerrainType = "TERRAIN_PLAINS", PreferenceValue = 17}

}
end

-- Default Resources we exclude by default (artifacts, antiquity sites, slaves, etc.)
-- Normally set in the database, but if you are not using a modified database you can set them here.

function CheckSpecialCases(resourceType)

    local excludedResourceTypes = {

        "RESOURCE_ARTIFACTS",
        "RESOURCE_HIDDEN_ARTIFACTS",
        "RESOURCE_SLAVES"

    }

    for i, excludedResourceType in ipairs(excludedResourceTypes) do
        if excludedResourceType == resourceType then   
            return true
        end
    end

    return false;
end
