
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
   return math.floor(number * mult + 0.5) / mult

end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:get_number_trade_routes_from_city(player, city, is_international, is_internal, domain)

	local trade_routes = player:GetTradeRoutes()
	local routes_amount = 0
	for _, trade_route in ipairs(trade_routes) do
      local domain_type = trade_route.Domain
		local originating_city = trade_route.FromCity
		local target_city = trade_route.ToCity
      if domain ~= nil and domain ~= domain_type then -- skip
      elseif target_city and originating_city == city then
         if is_international then
            if trade_route.ToCity:GetOwner() ~= player:GetID() then
               routes_amount = routes_amount + 1
            end
         elseif is_internal then
            if trade_route.ToCity:GetOwner() == player:GetID() then
               routes_amount = routes_amount + 1
            end
         else
            routes_amount = routes_amount + 1
         end
      end

	end

return routes_amount end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:table_to_string(table)

   for k, v in pairs(table) do
      print(k, v)
   end
   local string = ""
   for key, value in pairs(table) do

      if type(value) == "table" then
         string = string .. key .. "=" .. LekmodUtilities:table_to_string(value) .. ","
      else
         string = string .. key .. "=" .. value .. ","
      end
   end

return string end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:string_to_table(string)
   
     local table = {}
     -- Split the string into key-value pairs, account for a sub-table
       for key, value in string.gmatch(string, "(%w+)=(%w+),") do
         if tonumber(value) then
             table[key] = tonumber(value)
         else
             table[key] = value
         end
       end

return table end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:remove_from_table(table, value)

   for i = #table, 1, -1 do
      if table[i] == value then
         table[i] = table[#table]
         table[#table] = nil
      end
   end

end
------------------------------------------------------------------------------------------------------------------------
function LekmodUtilities:table_contains(table, value)

   for _, v in ipairs(table) do
      if v == value then return true end
   end
end