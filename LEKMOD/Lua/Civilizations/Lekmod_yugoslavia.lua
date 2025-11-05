-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_YUGOSLAVIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Yugoslavia UA: Award a free Tenet whenever the player adopts a new Ideology
------------------------------------------------------------------------------------------------------------------------
function lekmod_yugoslavia_ideology_tenet(player_id, policy_branch_id)

   local player = Players[player_id]
   if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end

   if player:GetAnarchyNumTurns() > 0 then return end

   if policy_branch_id == GameInfoTypes["POLICY_BRANCH_AUTOCRACY"]
   or policy_branch_id == GameInfoTypes["POLICY_BRANCH_FREEDOM"]
   or policy_branch_id == GameInfoTypes["POLICY_BRANCH_ORDER"] then
      local free_tenets = player:GetNumFreeTenets()
      print ("Player has " .. free_tenets .. " free tenets")
      player:SetNumFreeTenets(free_tenets + 1, true)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   -- Note: PlayerPolicyBranchUnlocked is a Lekmod event. It is not available in the base game
   GameEvents.PlayerPolicyBranchUnlocked.Add(lekmod_yugoslavia_ideology_tenet)
end
