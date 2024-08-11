-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Global dummy buildings. Adds a permanent dummy building to every city (or just the capital),
-- Supports various checks. Data set in the xml (XML Overrides/civ5Units.xml)
------------------------------------------------------------------------------------------------------------------------
local function add_buildings(player_or_team_id)

   for _, player in ipairs(Players) do
      if player == player_or_team_id or player:GetTeam() == player_or_team_id then
         if not player:IsAlive()
         or player:IsBarbarian()
         or player:IsMinorCiv() then
            return
         end
      end
   end

   local valid_buildings = {}

   if GameInfo.Global_Dummy_Buildings == nil then print("dummy table does not exist, check the xml!") return end
   for dummy_data in GameInfo.Global_Dummy_Buildings() do

      local civilization_type = dummy_data.CivilizationType
      local is_capital_only = dummy_data.IsCapitalOnly
      local building_type = dummy_data.BuildingType
      local technology_type = dummy_data.TechType
      local is_golden_age = dummy_data.RequiresGoldenAge
      local policy_type = dummy_data.PolicyType
      local policy_branch_finished = dummy_data.PolicyBranchFinished
      local policy_branch_chosen = dummy_data.PolicyBranchChosen

      for _, player in ipairs(Players) do

         if player == player_or_team_id or player:GetTeam() == player_or_team_id then

            -- first delete all instances of the building, then check if we can re-add it anywhere
            for city in player:Cities() do
               if city then
                  city:SetNumRealBuilding(GameInfoTypes[building_type], 0)
               end
            end
            if civilization_type ~= nil and player:GetCivilizationType() ~= GameInfoTypes[civilization_type] then -- skip
            elseif technology_type ~= nil
            and (not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes[technology_type])) then -- skip
            elseif is_golden_age and player:IsGoldenAge() then -- skip
            elseif policy_type ~= nil and not player:HasPolicy(GameInfoTypes[policy_type]) then -- skip
            elseif policy_branch_finished ~= nil
            and not player:IsPolicyBranchFinished(GameInfoTypes[policy_branch_finished]) then -- skip
            elseif policy_branch_chosen ~= nil
            -- Note: HasPolicyBranch is a Lekmod method. It is not available in the base game
            and not player:HasPolicyBranch(GameInfoTypes[policy_branch_chosen]) then -- skip
            else
               for city in player:Cities() do
                  if city then
                     if is_capital_only and not city:IsCapital() then -- skip
                     else
                        valid_buildings[player:GetID()] = valid_buildings[player:GetID()] or {}
                        -- Collect all building we can add, then add them later.
                        -- This is to avoid potential conflicts with multiple building entries of the same type.
                        table.insert(valid_buildings[player:GetID()], {building_type, is_capital_only})
                     end
                  end
               end
            end
         end

      end

   end

   -- Add the buildings
   for player_id, buildings in pairs(valid_buildings) do
      for _, building_data in ipairs(buildings) do
         local building_type = building_data[1]
         local is_capital_only = building_data[2]
         for city in Players[player_id]:Cities() do
            if city then
               if is_capital_only and not city:IsCapital() then -- skip
               else
                  city:SetNumRealBuilding(GameInfoTypes[building_type], 1)
               end
            end
         end
      end
   end

   -- Reset the valid buildings table
   for player_id, _ in pairs(valid_buildings) do
      valid_buildings[player_id] = nil
   end

end
GameEvents.PlayerCityFounded.Add(add_buildings)
GameEvents.CityCaptureComplete.Add(add_buildings)
GameEvents.TeamSetHasTech.Add(add_buildings)
GameEvents.PlayerAdoptPolicy.Add(add_buildings)
GameEvents.PlayerAdoptPolicyBranch.Add(add_buildings)
-- Note: PlayerGoldenAge is a Lekmod Event! Not available in the base game
GameEvents.PlayerSetGoldenAge.Add(add_buildings)
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