-- Author: EnormousApplePie


-- UTILITY FUNCTIONS
-------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------
-- Adding Dummy Buildings
function doDummyBuilding(playerID)
    for dummy_data in GameInfo.Civilization_Dummy_Buildings() do
        if GameInfo.Civilization_Dummy_Buildings == nil then print("dummy table does not exist, check the xml!") return end
        
        local civType = dummy_data.Type
        local bIsCapitalOnly = dummy_data.IsCapitalOnly
        local buildingType = dummy_data.BuildingType
        local techType = dummy_data.TechType

        for playerID, player in pairs(Players) do
            if player:GetCivilizationType() == GameInfoTypes[civType] then
                if techType ~= nil then if not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes[techType]) then break end end
                local player = Players[playerID]
                if player:IsEverAlive() then
                    for city in player:Cities() do
                        if not bIsCapitalOnly then
                            city:SetNumRealBuilding(GameInfoTypes[buildingType], 1)
                        elseif city:IsCapital() then
                            city:SetNumRealBuilding(GameInfoTypes[buildingType], 1)
                        end
                    end
                end
            end
        end
    end
end
GameEvents.PlayerCityFounded.Add(doDummyBuilding)
GameEvents.CityCaptureComplete.Add(doDummyBuilding)
GameEvents.TeamSetHasTech.Add(doDummyBuilding)
-------------------------------------------------------------------------------------------------------------------------
