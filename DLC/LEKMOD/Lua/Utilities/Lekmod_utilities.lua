
-- Author: EnormousApplePie


LekmodUtilities = {}

------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:is_civilization_active(civilization_id)

   for slot = 0, GameDefines.MAX_MAJOR_CIVS - 1, 1 do
      local slot_status = PreGame.GetSlotStatus(slot)
      if (slot_status == SlotStatus["SS_TAKEN"] or slot_status == SlotStatus["SS_COMPUTER"]) then
         if PreGame.GetCivilization(slot) == civilization_id then return true end
      end
   end

return false end

------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:get_random_between(lower, upper)

   return Game.Rand((upper + 1) - lower, "") + lower

end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:get_round(number, idp)

   local mult = 10^(idp or 0)
   return g_MathFloor(number * mult + 0.5) / mult

end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:get_number_trade_routes_from_city(player, city, domain)

	local trade_routes = player:GetTradeRoutes()
	local routes_amount = 0
	for _, trade_route in ipairs(trade_routes) do

      local domain_type = trade_route.Domain
		local originating_city = trade_route.FromCity
		local target_city = trade_route.ToCity
      if domain ~= nil and domain ~= domain_type then -- skip
      elseif target_city and originating_city == city then
         routes_amount = routes_amount + 1
      end

	end

return routes_amount end
------------------------------------------------------------------------------------------------------------------------
