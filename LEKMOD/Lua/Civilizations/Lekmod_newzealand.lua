-- Author: EnormousApplePie

include("Lekmod_utilities.lua")
include("FLuaVector.lua")

local this_civ = GameInfoTypes["CIVILIZATION_NEW_ZEALAND"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- New Zealand UA. Give a bonus to the player when they meet a new civ.
------------------------------------------------------------------------------------------------------------------------
function lekmod_new_zealand_ua_award_bonus(player, other_player)

   if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   local rewards = {
      [1] = {reward = 10, type = "Faith", string = "[ICON_PEACE] Faith", method = "ChangeFaith"},
      [2] = {reward = 6, type = "Culture", string = "[ICON_CULTURE] Culture", method = "ChangeJONSCulture"},
      [3] = {reward = 12, type = "Science", string = "[ICON_RESEARCH] Science", method = "ChangeResearchProgress"},
      [4] = {reward = 40, type = "Gold", string = "[ICON_GOLD] Gold", method = "ChangeGold"}
   }

	local random = LekmodUtilities:get_random_between(1, 4)

   -- Science is handled on team level
   if rewards[random].method == "ChangeResearchProgress" then
      -- if the player is not currently researching anything, add the science to the overflow
      if player:GetCurrentResearch() == -1 then
         Teams[player:GetTeam()]:GetTeamTechs():ChangeResearchProgress(player:GetOverflowResearch(), rewards[random].reward, player:GetID())
      else
         Teams[player:GetTeam()]:GetTeamTechs():ChangeResearchProgress(player:GetCurrentResearch(), rewards[random].reward, player:GetID())
      end
   else
      player[rewards[random].method](player, rewards[random].reward)
   end

   if player:IsHuman() and Game.GetActivePlayer() == player:GetID() then
      local text = Locale.ConvertTextKey("TXT_KEY_NZ_MEET_BONUS", rewards[random].reward, rewards[random].string, other_player:GetName())
      Events.GameplayAlertMessage(text)
   end

end

function lekmod_new_zealand_ua_on_meet(team_met_id, player_team_id)

	local player_team = Teams[player_team_id]
	local team_met = Teams[team_met_id]
	
	local player_id = player_team:GetLeaderID()
	local met_player_id = team_met:GetLeaderID()

	lekmod_new_zealand_ua_award_bonus(Players[player_id], Players[met_player_id])
	lekmod_new_zealand_ua_award_bonus(Players[met_player_id], Players[player_id])

end
------------------------------------------------------------------------------------------------------------------------
-- New Zealand Maori Battalion. Yields 1 influence with a city state per turn if within their borders.
------------------------------------------------------------------------------------------------------------------------
function lekmod_is_in_city_state_borders(player_id, unit)

	local plot = unit:GetPlot()
	if plot:GetOwner() > -1 then
		local player = Players[plot:GetOwner()]
		return (player:IsMinorCiv() and player:GetMinorCivFriendshipLevelWithMajor(player_id) >= 1)
	end

return false end

function lekmod_new_zealand_uu_batallion(player_id)

   local maori_promotion_id = GameInfoTypes["PROMOTION_ATTACK_AWAY_CAPITAL"]
   local player = Players[player_id]

	if not player:IsAlive() or player:IsBarbarian() then return end

   for unit in player:Units() do

      if not unit:IsHasPromotion(maori_promotion_id) then -- skip
      elseif lekmod_is_in_city_state_borders(player_id, unit) then
         local city_state_id = unit:GetPlot():GetOwner()
         local city_state = Players[city_state_id]
         city_state:ChangeMinorCivFriendshipWithMajor(player_id, 1)
         if player:IsHuman() and Game.GetActivePlayer() == player_id then
            local hex = ToHexFromGrid(Vector2(unit:GetX(), unit:GetY()))
            Events.AddPopupTextEvent(HexToWorld(hex), Locale.ConvertTextKey("[ICON_WHITE]+{1_Num}[ENDCOLOR] [ICON_INFLUENCE]", 1), true)
         end
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
-- New Zealand UU ironclad. Set a promotion that ignores ZoC on the unit if it is a near a friendly city.
------------------------------------------------------------------------------------------------------------------------
function lekmod_new_zealand_uu_defender(player_id)

   local defender_promotion_zoc_id = GameInfoTypes["PROMOTION_JFD_DEFENDER_ACTIVE"]
   local defender_promotion_default_id	= GameInfoTypes["PROMOTION_JFD_DEFENDER"]

	local player = Players[player_id]
	if not player:IsAlive() or player:IsBarbarian() then return end

	for unit in player:Units() do

		local is_promotion_valid = false
		if (unit and (unit:IsHasPromotion(defender_promotion_zoc_id)
      or unit:IsHasPromotion(defender_promotion_default_id))) then
			local plot = unit:GetPlot()
			if plot and plot:IsPlayerCityRadius(player_id) then
            print("New Zealand UU defender is in city radius")
				is_promotion_valid = true
			else
				for loop_player_id = 0, GameDefines.MAX_MAJOR_CIVS-1, 1 do
					local loop_player = Players[loop_player_id]
					if not loop_player:IsAlive() or loop_player == player_id then -- skip
               elseif loop_player:IsDoF(player:GetTeam()) then
                  if plot:IsPlayerCityRadius(loop_player) then
                     is_promotion_valid = true
                     break
                  end
					end
				end
			end
		end
		if is_promotion_valid and (not unit:IsHasPromotion(defender_promotion_zoc_id)) then
			unit:SetHasPromotion(defender_promotion_zoc_id, true)
			unit:SetHasPromotion(defender_promotion_default_id, false)
		elseif not is_promotion_valid and unit:IsHasPromotion(defender_promotion_zoc_id) then
			unit:SetHasPromotion(defender_promotion_zoc_id, false)
			unit:SetHasPromotion(defender_promotion_default_id, true)
		end

	end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.TeamMeet.Add(lekmod_new_zealand_ua_on_meet)
end
-- Unique units should work even if the civ is not active
GameEvents.PlayerDoTurn.Add(lekmod_new_zealand_uu_batallion)
GameEvents.PlayerDoTurn.Add(lekmod_new_zealand_uu_defender)