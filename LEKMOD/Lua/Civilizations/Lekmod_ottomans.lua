-- Author: EnormousApplePie

include("Lekmod_utilities.lua")
include("FLuaVector.lua")

local this_civ = GameInfoTypes["CIVILIZATION_OTTOMAN"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Ottoman UA. Gain faith when a unit is promoted.
------------------------------------------------------------------------------------------------------------------------
function lekmod_ottoman_ua_promotion_faith(player_id, unit_id)

   local player = Players[player_id]

	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

	local unit = player:GetUnitByID(unit_id)
	local xp_needed = unit:ExperienceNeeded()
	local faith_bonus = math.ceil(xp_needed / 3)
	player:ChangeFaith(faith_bonus)

	if player:IsHuman() and Game.GetActivePlayer() == player_id then
		local hex = ToHexFromGrid(Vector2(unit:GetX(), unit:GetY()))
		Events.AddPopupTextEvent(HexToWorld(hex), Locale.ConvertTextKey("+{1_Num}[ENDCOLOR] [ICON_PEACE]", faith_bonus), true)
	end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.UnitPromoted.Add(lekmod_ottoman_ua_promotion_faith)
end