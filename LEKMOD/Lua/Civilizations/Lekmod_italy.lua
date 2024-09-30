-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_ITALY"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Italy UA: Enter a golden age or extend one whenever a player finishes a policy tree
------------------------------------------------------------------------------------------------------------------------
function lekmod_italy_ua_on_policy_adopt(player_id, policy_id)

   print("Italy UA: Policy Adopted")
	local player = Players[player_id]
   local policy = GameInfo.Policies[policy_id]
   local policy_branch = GameInfo.PolicyBranchTypes[policy.PolicyBranchType]

	if not player:IsEverAlive() or player:GetCivilizationType() ~= this_civ
   or policy_branch == nil or not player:HasPolicy(policy_id)
   or not player:IsPolicyBranchFinished(policy_branch.ID)
   or Game.GetActivePlayer() ~= player_id then return end

   -- amount made for quick speed, but scales with other speeds.
   local golden_age_turns = 5 * (GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100);
   local golden_age_points = 312.5 * (GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100)

   if player:IsGoldenAge() then
      player:ChangeGoldenAgeTurns(golden_age_turns)
      local popup_text = Locale.ConvertTextKey("TXT_KEY_TRAIT_ITALY_POLICY_BRANCH_FINISHED_EXTENDED", golden_age_turns)
      Events.GameplayAlertMessage(popup_text)
   else
      player:ChangeGoldenAgeProgressMeter(golden_age_points)
      local popup_text = Locale.ConvertTextKey("TXT_KEY_TRAIT_ITALY_POLICY_BRANCH_FINISHED", golden_age_points)
      Events.GameplayAlertMessage(popup_text)
   end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.PlayerAdoptPolicy.Add(lekmod_italy_ua_on_policy_adopt)
end