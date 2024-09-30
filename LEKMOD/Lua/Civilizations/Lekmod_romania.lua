-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_ROMANIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Romania UA. Award golden age points whenever the player captures or liberates a city
------------------------------------------------------------------------------------------------------------------------
function lekmod_romania_ua_on_capture(player_id, _, _, _, new_player_id)

   local player = Players[new_player_id]
   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end
   local golden_age_points = 125 * GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100
   player:ChangeGoldenAgeProgressMeter(golden_age_points)

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.CityCaptureComplete.Add(lekmod_romania_ua_on_capture)
end