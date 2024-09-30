-- Author: EnormousApplePie

------------------------------------------------------------------------------------------------------------------------
-- Blocked units
------------------------------------------------------------------------------------------------------------------------

function lekmod_block_unit_construction(player_id, _, unit_type)

   local player = Players[player_id]
   if unit_type ~= GameInfoTypes["UNIT_WORKBOAT"] then return true end

   if not player:IsHuman() then return true
   else return false end

end
GameEvents.CityCanTrain.Add(lekmod_block_unit_construction)
------------------------------------------------------------------------------------------------------------------------
-- Unit Embark bug fix
------------------------------------------------------------------------------------------------------------------------
local hover_promotion = GameInfoTypes["PROMOTION_MOVE_ALL_TERRAIN"]
local embark_promotion = GameInfoTypes["PROMOTION_EMBARKATION"]

function lekmod_embark_fix(player_id)

	local player = Players[player_id]
	if not player:IsAlive() then return end
	for unit in player:Units() do

		if unit:IsHasPromotion(hover_promotion) then
			unit:SetHasPromotion(embark_promotion, false)
		end

	end

end
-- Note: UnitCreated is a Lekmod Event! Not available in the base game
GameEvents.UnitCreated.Add(lekmod_embark_fix)
GameEvents.PlayerDoTurn.Add(lekmod_embark_fix)

