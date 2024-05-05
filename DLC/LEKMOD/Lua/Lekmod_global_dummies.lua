-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Global dummy buildings. Adds a permanent dummy building to every city (or just the capital),
-- either at the start of the game or on a specific tech. Data set in the xml (XML Overrides/civ5Units.xml)
------------------------------------------------------------------------------------------------------------------------
local function add_buildings(player_or_team_id)

   if GameInfo.Civilization_Dummy_Buildings == nil then print("dummy table does not exist, check the xml!") return end
   for dummy_data in GameInfo.Civilization_Dummy_Buildings() do

      local civilization_type = dummy_data.Type
      local is_capital_only = dummy_data.IsCapitalOnly
      local building_type = dummy_data.BuildingType
      local technology_type = dummy_data.TechType

      for _, player in ipairs(Players) do
         if player == player_or_team_id or player:GetTeam() == player_or_team_id then
            if not player:IsAlive() or player:GetCivilizationType() ~= GameInfoTypes[civilization_type] then -- skip
            elseif technology_type ~= nil
            and (not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes[technology_type])) then -- skip
            else
               for city in player:Cities() do
                  if is_capital_only and not city:IsCapital() then -- skip
                  else
                     city:SetNumRealBuilding(GameInfoTypes[building_type], 1)
                  end
               end
            end
         end
      end

   end

end
GameEvents.PlayerCityFounded.Add(add_buildings)
GameEvents.CityCaptureComplete.Add(add_buildings)
GameEvents.TeamSetHasTech.Add(add_buildings)
------------------------------------------------------------------------------------------------------------------------
-- Global dummy policies. Set at the start of the game. Data set in the xml (XML Overrides/civ5Units.xml)
------------------------------------------------------------------------------------------------------------------------
local function add_policies(player_id)

   if GameInfo.Civilization_Dummy_Policies == nil then print("dummy table does not exist, check the xml!") return end
   for dummy_data in GameInfo.Civilization_Dummy_Policies() do

      local civilization_type = dummy_data.Type
      local policy_type = dummy_data.PolicyType

      for _, player in ipairs(Players) do
         if player:IsEverAlive() and player:GetCivilizationType() == GameInfoTypes[civilization_type] then
            if not player:HasPolicy(GameInfoTypes[policy_type]) then
               player:SetNumFreePolicies(1)
               player:SetNumFreePolicies(0)
               player:SetHasPolicy(GameInfoTypes[policy_type], true)
            end
         end
      end

   end

end
Events.SequenceGameInitComplete.Add(add_policies)
------------------------------------------------------------------------------------------------------------------------