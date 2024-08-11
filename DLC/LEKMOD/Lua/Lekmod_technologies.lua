-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Add a permanent building that gives +1 Vote in the world congress if the technology has been set to give additional delegates
------------------------------------------------------------------------------------------------------------------------
function add_globalization_dummy_building(team_id, tech_id)

   local delegates_number = 0
   for player_id, player in pairs(Players) do

      if player:IsEverAlive() then
         for technology_info in GameInfo.Technologies() do

            if Teams[player:GetTeam()]:IsHasTech(technology_info.ID)
            and technology_info.AdditionalDelegates > 0 then
               delegates_number = delegates_number + technology_info.AdditionalDelegates
            end

         end

         for city in player:Cities() do
            local capital = player:GetCapitalCity()
            capital:SetNumRealBuilding(GameInfoTypes["BUILDING_TECH_EXTRA_DELEGATES"], delegates_number)
         end

      end

   end

end
------------------------------------------------------------------------------------------------------------------------
GameEvents.TeamSetHasTech.Add(add_globalization_dummy_building)
GameEvents.PlayerCityFounded.Add(add_globalization_dummy_building)
GameEvents.CityCaptureComplete.Add(add_globalization_dummy_building)
------------------------------------------------------------------------------------------------------------------------