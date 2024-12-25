-- Author: 404NotFound & EnormousApplePie

include("Lekmod_utilities.lua")
include("PlotIterators.lua")

local this_civ = GameInfoTypes["CIVILIZATION_GEORGIA"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Georgia UA. Disable regular faith buildings and enable special ones based on beliefs in the city.
------------------------------------------------------------------------------------------------------------------------
function lekmod_georgia_ua_check_faith_building(player, city)

   city:SetNumRealBuilding(GameInfoTypes["BUILDING_CATHEDRAL_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_MOSQUE_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_PAGODA_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_GURDWARA_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_SYNAGOGUE_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_VIHARA_GEORGIA_DUMMY"], 0);
   city:SetNumRealBuilding(GameInfoTypes["BUILDING_MANDIR_GEORGIA_DUMMY"], 0);

	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

	local religion = city:GetReligiousMajority();
	if religion == 0 or nil then return end

   for _, belief in ipairs(Game.GetBeliefsInReligion(religion)) do
      if belief then
         if GameInfoTypes["BELIEF_CATHEDRALS"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_CATHEDRAL_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_MOSQUES"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_MOSQUE_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_PAGODAS"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_PAGODA_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_GURDWARAS"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_GURDWARA_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_SYNAGOGUES"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_SYNAGOGUE_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_VIHARAS"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_VIHARA_GEORGIA_DUMMY"], 1);
         elseif GameInfoTypes["BELIEF_MANDIRS"] == belief then
            city:SetNumRealBuilding(GameInfoTypes["BUILDING_MANDIR_GEORGIA_DUMMY"], 1);
         end
      end
   end

end

function lekmod_georgia_ua(player_id)
	local player = Players[player_id]
	for city in player:Cities() do
      if city ~= nil then
         lekmod_georgia_ua_check_faith_building(player, city)
      end
	end
end

function lekmod_georgia_ua_on_convert(player_id, _, x, y)
	lekmod_georgia_ua_check_faith_building(Players[player_id], Map.GetPlot(x, y):GetPlotCity());
end

function lekmod_georgia_disable_normal_faith_buildings(playerID, cityID, buildingType)

	local player = Players[playerID];
	if player:GetCivilizationType() == this_civ then
		if buildingType == GameInfoTypes["BUILDING_CATHEDRAL"] or buildingType == GameInfoTypes["BUILDING_MOSQUE"] or buildingType == GameInfoTypes["BUILDING_PAGODA"] or buildingType == GameInfoTypes["BUILDING_GURDWARA"] or buildingType == GameInfoTypes["BUILDING_SYNAGOGUE"] or buildingType == GameInfoTypes["BUILDING_VIHARA"] or buildingType == GameInfoTypes["BUILDING_MANDIR"] then
			return false
		end
	end
	return true


end
------------------------------------------------------------------------------------------------------------------------
-- Georgia UU. Give Khevsur the Golden Age promotion if the player is in a Golden Age.
------------------------------------------------------------------------------------------------------------------------
function lekmod_georgia_uu_golden_age(player_id)

	local player = Players[player_id]
	if player:GetCivilizationType() ~= this_civ or not player:IsAlive() then return end

    for loop_unit in player:Units() do
        if loop_unit:GetUnitType() == GameInfoTypes["UNIT_GEORGIA_KHEVSUR"] and not loop_unit:IsDead() then
            if player:IsGoldenAge() and not loop_unit:IsHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"]) then
                loop_unit:SetHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"], true);
            elseif not player:IsGoldenAge() and loop_unit:IsHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"])then
                loop_unit:SetHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"], false);
            end
        end
    end

end
------------------------------------------------------------------------------------------------------------------------
if is_active then
   GameEvents.ReligionFounded.Add(lekmod_georgia_ua)
   GameEvents.ReligionEnhanced.Add(lekmod_georgia_ua)
   GameEvents.CityAdoptReligion.Add(lekmod_georgia_ua_on_convert)
   GameEvents.CityConvertsReligion.Add(lekmod_georgia_ua_on_convert)
   GameEvents.CityCanConstruct.Add(lekmod_georgia_disable_normal_faith_buildings)
end
GameEvents.PlayerDoTurn.Add(lekmod_georgia_uu_golden_age)
GameEvents.GreatPersonExpended.Add(lekmod_georgia_uu_golden_age)