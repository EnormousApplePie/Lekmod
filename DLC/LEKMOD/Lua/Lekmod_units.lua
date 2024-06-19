-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Blocked units
------------------------------------------------------------------------------------------------------------------------

local function block_unit_construction(player_id, _, unit_type)

   local player = Players[player_id]
   if unit_type ~= GameInfoTypes["UNIT_WORKBOAT"] then return true end

   if not player:IsHuman() then return true
   else return false end

end
------------------------------------------------------------------------------------------------------------------------
GameEvents.CityCanTrain.Add(block_unit_construction)
------------------------------------------------------------------------------------------------------------------------
-- Unit Embark bug fix
------------------------------------------------------------------------------------------------------------------------
local hover_promotion = GameInfoTypes["PROMOTION_MOVE_ALL_TERRAIN"]
local embark_promotion = GameInfoTypes["PROMOTION_EMBARKATION"]

local function embark_fix(player_id)

	local player = Players[player_id]
	if not player:IsAlive() then return end
	for unit in player:Units() do
		if unit:IsHasPromotion(hover_promotion) then
			unit:SetHasPromotion(embark_promotion, false)
		end
	end

end
------------------------------------------------------------------------------------------------------------------------
GameEvents.PlayerDoTurn.Add(embark_fix)

