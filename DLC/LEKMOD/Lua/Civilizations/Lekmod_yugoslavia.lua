-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_YUGOSLAVIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Yugoslavia UA: Award a free Tenet whenever the player adopts a new Ideology
------------------------------------------------------------------------------------------------------------------------
local function yugoslavia_ideology_tenet(player_id, policy_branch_id)

   local player = Players[player_id]
   if player:GetCivilizationType() == this_civ and player:IsAlive() then return end

   if policy_branch_id == GameInfoTypes["POLICY_BRANCH_AUTOCRACY"]
   or policy_branch_id == GameInfoTypes["POLICY_BRANCH_FREEDOM"]
   or policy_branch_id == GameInfoTypes["POLICY_BRANCH_ORDER"] then
      local free_tenets = player:GetNumFreeTenets()
      player:SetNumFreeTenets(free_tenets + 1)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerAdoptPolicyBranch.Add(yugoslavia_ideology_tenet)
end
