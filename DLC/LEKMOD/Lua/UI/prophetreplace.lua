
include("PlotIterators")
include("FLuaVector.lua")
include("PlotAreaSweepIterator.lua")
-- Author: EnormousApplePie & Lek10

--=================================================================================================================
--=================================================================================================================
						-- NO LEADER SCREENS
--=================================================================================================================
--=================================================================================================================
--[[
						function OnLeavingLeaderViewMode()
							Game.ExitLeaderScreen()
							print("exit leader screen")
						end
						Events.LeavingLeaderViewMode.Add(OnLeavingLeaderViewMode)
--]]
--=================================================================================================================
--=================================================================================================================
						-- GLOBALS (from JFD)
--=================================================================================================================
--=================================================================================================================

local g_ConvertTextKey  		= Locale.ConvertTextKey
local g_MapGetPlot				= Map.GetPlot
local g_MathCeil				= math.ceil
local g_MathFloor				= math.floor
local g_MathMax					= math.max
local g_MathMin					= math.min


-- UTILITY FUNCTIONS
-------------------------------------------------------------------------------------------------------------------------

--=================================================================================================================
--Adding Globalization Dummy Building
--=================================================================================================================

-------------------------------------------------------------------------------------------------------------------------


--=================================================================================================================
--=================================================================================================================
						--PRACTICAL FUNCTIONS (Eg: Calculations, random, optimizations, utils)
--=================================================================================================================
--=================================================================================================================

-- GetRandom number
function GetRandom(lower, upper)
        return Game.Rand((upper + 1) - lower, "") + lower
 end
--Game_GetRound (from JFD)
function Game_GetRound(num, idp)
	local mult = 10^(idp or 0)
	return g_MathFloor(num * mult + 0.5) / mult
end
local g_GetRound = Game_GetRound


--IscivActive (from JFD)
local iPracticalNumCivs = (GameDefines.MAX_MAJOR_CIVS - 1)

function JFD_IsCivilisationActive(civilizationID)
	for iSlot = 0, iPracticalNumCivs, 1 do
		local slotStatus = PreGame.GetSlotStatus(iSlot)
		if (slotStatus == SlotStatus["SS_TAKEN"] or slotStatus == SlotStatus["SS_COMPUTER"]) then
			if PreGame.GetCivilization(iSlot) == civilizationID then
				return true
			end
		end
	end
	return false
end

function JFD_GetNumDomesticRoutesFromThisCity(player, city) -- for both Sea and Land
	local tradeRoutes = player:GetTradeRoutes()
	local numDomesticRoutes = 0
	for tradeRouteID, tradeRoute in ipairs(tradeRoutes) do
		local domain = tradeRoute.Domain
		local originatingCity = tradeRoute.FromCity
		local targetCity = tradeRoute.ToCity
		if targetCity and originatingCity == city then
			numDomesticRoutes = numDomesticRoutes + 1
		end
	end
	
	return numDomesticRoutes
end

function PlaceExtraJuiceResource(startingPlot, resourceID)
    local plots = {}
	print("running placeExtraJuice")
    for loopPlot in PlotAreaSweepIterator(startingPlot, 3, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
        table.insert(plots, loopPlot)
    end
	
    for iVal = 1, 1 do
		local isPlaced = false
		while (not isPlaced) and #plots > 0 do
			local randomNumber = GetRandom(1, #plots)
			local plot = plots[randomNumber]
			local plotType = plot:GetPlotType();
			local terrainType = plot:GetTerrainType();
			if (plot:IsWater() and plot:GetNumResource() == 0 and not plot:IsLake()) then
				print("checkedplots")
				if terrainType == TerrainTypes.TERRAIN_COAST then
					plot:SetResourceType(resourceID, 1)
					plot:SetFeatureType(-1)
					isPlaced = true
					print("placed some fish")
				end
			end
			
			table.remove(plots, randomNumber)
		end
	end
end

--====================================================================================================================
											--GLOBAL EFFECTS 

--====================================================================================================================
local FishID = GameInfoTypes["RESOURCE_FISH"]
local placeRes = false
function PlaceExtraJuice()
	print("running fish function")
	if Game.GetGameTurn() > Game.GetStartTurn() then return end
    if (placeRes) then return end
    for playerID = 0, GameDefines.MAX_MAJOR_CIVS - 1 do
        local player = Players[playerID]
        if (player:IsEverAlive()) then
             --PlaceExtraJuiceResource(player:GetStartingPlot(), FishID)
        end
	end
    placeRes = true
end

Events.SequenceGameInitComplete.Add(PlaceExtraJuice)
--===================================================================================================================================

--[[
-- Philippine ua
local iCiv = GameInfoTypes["CIVILIZATION_PHILIPPINES"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

local iBuilding = GameInfoTypes["BUILDING_PHILIPPINES_TRAIT"]
local iNumCities = 2

function PhilippineExpandsBonus(playerID, iX, iY)
    local pPlayer = Players[playerID]
    if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_PHILIPPINES"] and pPlayer:IsAlive() and pPlayer:CountNumBuildings(iBuilding) < iNumCities then
        local pPlot = Map.GetPlot(iX, iY)
        local pCity = pPlot:GetPlotCity()
        if not pCity:IsCapital() then
            pCity:SetNumRealBuilding(iBuilding, 1)
        end
    end
end




-- From JFD
function PhilippineMovementBonus(playerID, unitID, unitX, unitY)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_PHILIPPINES"] and player:IsAlive() then
		local inFriendlyTerritory = false
		local unit = player:GetUnitByID(unitID)
		if not unit:IsDead() then
			if unit ~= nil then
				if (unit:GetUnitClassType() == GameInfoTypes["UNITCLASS_WORKER"] or unit:GetUnitClassType() == GameInfoTypes["UNITCLASS_SETTLER"] or unit:GetDomainType() == GameInfoTypes["DOMAIN_SEA"]) then
					if Map.GetPlot(unit:GetX(), unit:GetY()):GetOwner() == playerID then
						inFriendlyTerritory = true
					end
				end
			end
		end
		
		if inFriendlyTerritory then
			if not unit:IsHasPromotion(GameInfoTypes["PROMOTION_GOOD_FIGHT"]) then
				unit:SetHasPromotion(GameInfoTypes["PROMOTION_GOOD_FIGHT"], true)
			end
		else
			if unit:IsHasPromotion(GameInfoTypes["PROMOTION_GOOD_FIGHT"]) then
				unit:SetHasPromotion(GameInfoTypes["PROMOTION_GOOD_FIGHT"], false)
			end
		end
	end
end
if bIsActive then
GameEvents.UnitSetXY.Add(PhilippineMovementBonus)
GameEvents.PlayerCityFounded.Add(PhilippineExpandsBonus)
end

-- Nabatea UU
local iCiv = GameInfoTypes["CIVILIZATION_NABATEA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
local iZabonah = GameInfoTypes.UNIT_MC_ZABONAH

function ZabonahDiscovery(iPlayer, iUnit, iX, iY) -- from Sukritact
	
	local pPlayer = Players[iPlayer]
	local pUnit = pPlayer:GetUnitByID(iUnit)
	if pUnit:GetUnitType() ~= iZabonah then return end

	local iTeam = pUnit:GetTeam()
	
	-- Iterate to find cities
	local pPlot = Map.GetPlot(iX, iY)
	for pAdjacentPlot in PlotAreaSweepIterator(pPlot, pUnit:VisibilityRange() + 3, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
		local pCity = pAdjacentPlot:GetPlotCity()
		if pCity then
			if not(pCity:IsRevealed(iTeam)) then
				pAdjacentPlot:SetRevealed(iTeam, true);

				local iReward = 0
				local iRewardExp = 0
				if pCity:IsCapital() then iReward = 10 end
				if pCity:IsCapital() then iRewardExp = 0 end

				pPlayer:ChangeGold(iReward)
				pUnit:ChangeExperience(iRewardExp)

				if (pPlayer:IsHuman() and pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NABATEA"] and Game.GetActivePlayer() == iPlayer and iReward > 0 ) then
					Events.GameplayAlertMessage(Locale.ConvertTextKey("[COLOR_POSITIVE_TEXT]+{1_Num} [ICON_GOLD] Gold[ENDCOLOR] from discovering {2_City} with a Zabonah", iReward, pCity:GetName()))
				end				
			end
		end
	end
end
-- Nabatea UB

function JFD_TombProduction(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NABATEA"] and player:IsAlive() then
		for city in player:Cities() do
			if city:IsHasBuilding(GameInfoTypes["BUILDING_MC_KOKH"]) then
				local numDomesticRoutes = JFD_GetNumDomesticRoutesFromThisCity(player, city)
				if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_KOKH"]) then
					city:SetNumRealBuilding(GameInfoTypes["BUILDING_NABATEA_TRAIT"], numDomesticRoutes) 
				end
			end
		end
	end
end


if bIsActive then

GameEvents.TeamSetHasTech.Add(function(iPlayer)
	print("triggered: nabatea ontechbonus")
	local player = Players[iPlayer];
	if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NABATEA"]) then
		if Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_CIVIL_SERVICE"]) then
			Teams[player:GetTeam()]:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], false);
			print("removed dummy")
		end
	end
	if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NABATEA"] and Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_MATHEMATICS"])) then
		if (not Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_CIVIL_SERVICE"])) then
			Teams[player:GetTeam()]:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], true);
			print("applied dummy")
		else
			Teams[player:GetTeam()]:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], false);
		end
	end
end);

GameEvents.PlayerDoTurn.Add(JFD_TombProduction)
GameEvents.UnitSetXY.Add(ZabonahDiscovery)
end

--- Moors ua

local iCiv = GameInfoTypes["CIVILIZATION_MOORS"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function MoorsEraUA(playerID)

	for playerID, player in pairs(Players) do
			local player = Players[playerID];
			local pCity = player:GetCapitalCity();
			if pCity ~= nil then
				if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MOORS"] and player:GetCurrentEra() == GameInfoTypes["ERA_MEDIEVAL"]) then
					
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT"], 1);
					for pCity in player:Cities() do
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT"], 1);
					end
				else
					
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT"], 0);
					for pCity in player:Cities() do
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT"], 0);
					end
				end
			end
	end
end
function MoorsEraUA2(playerID)

	for playerID, player in pairs(Players) do
			local player = Players[playerID];
			local pCity = player:GetCapitalCity();
			if pCity ~= nil then
				if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MOORS"] and player:GetCurrentEra() == GameInfoTypes["ERA_RENAISSANCE"]) then
					
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT2"], 1);
					for pCity in player:Cities() do
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT2"], 1);
					end
				else
					
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT2"], 0);
					for pCity in player:Cities() do
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOORS_TRAIT2"], 0);
					end
				end
			end
	end
end


if bIsActive then
GameEvents.PlayerDoTurn.Add(MoorsEraUA2)
GameEvents.PlayerDoTurn.Add(MoorsEraUA)
end

--- UAE UA


local iCiv = GameInfoTypes["CIVILIZATION_UAE"]
local bIsActive = JFD_IsCivilisationActive(iCiv)


-- From Sukritacts Nabatea's Civilization

function ZabonahRouteGold(iPlayer)
	local pPlayer = Players[iPlayer]
	if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UAE"] then
		for pUnit in pPlayer:Units() do
		

				--Gold if on Trade Route
				local pPlot = pUnit:GetPlot()
				local tTradeRoutes = pPlayer:GetInternationalTradeRoutePlotToolTip(pPlot);
				local bIsRoute = #tTradeRoutes > 0

				if (bIsRoute and pUnit:GetCombatLimit() > 0) then
					pPlayer:ChangeGold(3)
					pUnit:ChangeExperience(1)

					-- Fancy Notification
					if iPlayer == Game.GetActivePlayer() then
						if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UAE"] then
							local iX = pPlot:GetX()
							local iY = pPlot:GetY()
							local pHexPos = ToHexFromGrid{x=iX, y=iY}
							local pWorldPos = HexToWorld(pHexPos)

							Events.AddPopupTextEvent(pWorldPos, "[COLOR_YIELD_GOLD]+3 [ICON_GOLD] Gold[ENDCOLOR]")
						end
					end
				end
		end
		
	end
end

function UAEWonderBuilt(playerID, cityID, buildingID)
local player = Players[playerID]
if player:IsAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UAE"] then 
        local city = player:GetCityByID(cityID)
		local currentBuildingClass = GameInfo.Buildings[buildingID].BuildingClass
		local buildingClass = GameInfo.BuildingClasses[currentBuildingClass]
		if (buildingClass.MaxGlobalInstances == 1) then
		local goldBoost = 100 
			player:ChangeGold(goldBoost)
			city:ChangeWeLoveTheKingDayCounter(15)
			if (player:IsHuman() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UAE"] and Game.GetActivePlayer() == playerID) then
				local cityName = city:GetName()
				Events.GameplayAlertMessage("The rich and famous have flocked to " .. cityName .. " following the unveiling of its latest wonder! Receive " .. goldBoost .. " [ICON_GOLD] Gold and the City enters a We Love the King Day!")
			end
		end
	end
end

if bIsActive then
Events.UnitSelectionChanged.Add(ZabonahRouteHighlight)
GameEvents.PlayerDoTurn.Add(ZabonahRouteGold)
GameEvents.CityConstructed.Add(UAEWonderBuilt)
GameEvents.UnitSetXY.Add(DetectPlunder)
end

--- Oman UA

-- JFD_DomesticTRGold


local iCiv = GameInfoTypes["CIVILIZATION_OMAN"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function JFD_GetGoldFromDomesticTradeRoutes(player)
	local numDomesticTRSToConqueredCities = 0
	local goldFromDomesticTRS = 0
	local goldFromTradeBuildings = 0
	local tradeRoutes = player:GetTradeRoutes()
	for tradeRouteID, tradeRoute in ipairs(tradeRoutes) do
		local domain = tradeRoute.Domain
		local originatingCity = tradeRoute.FromCity
		local targetCity = tradeRoute.ToCity
		if tradeRoute.FromID == tradeRoute.ToID then
			if targetCity:IsPuppet() or targetCity:IsOccupied() or targetCity:IsHasBuilding(GameInfoTypes["BUILDING_COURTHOUSE"]) then
				numDomesticTRSToConqueredCities = numDomesticTRSToConqueredCities + 1
				
				goldFromDomesticTRS = 8
			end
		end
		
		for row in GameInfo.Buildings() do
			local seaGoldBonus = (row.TradeRouteSeaGoldBonus / 100)
			local recipientBonus = row.TradeRouteRecipientBonus
			if seaGoldBonus > 0 or recipientBonus > 0 then
				if originatingCity:IsHasBuilding(row.ID) then
					goldFromTradeBuildings = goldFromTradeBuildings + seaGoldBonus + recipientBonus 
				end
			end
		end
	end
	
	return (numDomesticTRSToConqueredCities * goldFromDomesticTRS) + (goldFromTradeBuildings / 2)
end


function JFD_DomesticTRGold(playerID)
	local player = Players[playerID]
	if player:IsAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_OMAN"] then
		local tradeRoutes = player:GetTradeRoutes()
		local goldFromDomesticTradeRoutes = JFD_GetGoldFromDomesticTradeRoutes(player)
		if goldFromDomesticTradeRoutes > 0 then
			player:ChangeGold(goldFromDomesticTradeRoutes -1)
		end
	end
end


if bIsActive then
	GameEvents.PlayerDoTurn.Add(JFD_DomesticTRGold)
end

--- Oman UB

local direction_types = {
	DirectionTypes.DIRECTION_NORTHEAST,
	DirectionTypes.DIRECTION_NORTHWEST,
	DirectionTypes.DIRECTION_EAST,
	DirectionTypes.DIRECTION_SOUTHEAST,
	DirectionTypes.DIRECTION_SOUTHWEST,
	DirectionTypes.DIRECTION_WEST,
	}

function JFD_MinaaAttrition(playerID)
	local player = Players[playerID]
	if player:IsAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_OMAN"] then
		for city in player:Cities() do
			if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
				local plot = Map.GetPlot(city:GetX(), city:GetY())
				for loop, direction in ipairs(direction_types) do
					local adjPlot = Map.PlotDirection(plot:GetX(), plot:GetY(), direction)
					if adjPlot:GetUnit() and adjPlot:GetUnit():GetOwner() ~= playerID then
						local unit = adjPlot:GetUnit()
						local unitOwner = Players[adjPlot:GetUnit():GetOwner()]
						if Teams[player:GetTeam()]:IsAtWar(unitOwner:GetTeam()) and unit:GetUnitCombatType() == GameInfoTypes["UNITCOMBAT_NAVALRANGED"] or unit:GetUnitCombatType() == GameInfoTypes["UNITCOMBAT_NAVALMELEE"] or unit:GetUnitCombatType() == GameInfoTypes["UNITCOMBAT_SUBMARINE"] or unit:GetUnitCombatType() == GameInfoTypes["UNITCOMBAT_CARRIER"] then
							unit:ChangeDamage(30)
						end
					end
				end
			end
		end
	end
end
if bIsActive then
GameEvents.PlayerDoTurn.Add(JFD_MinaaAttrition)
end

function JFD_MinaaNavalProduction(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_OMAN"] and player:IsAlive() then
		for city in player:Cities() do
			if city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
				local numDomesticRoutes = JFD_GetNumDomesticRoutesFromThisCity(player, city)
				if city and city:IsHasBuilding(GameInfoTypes["BUILDING_MC_OMANI_MINAA"]) then
					city:SetNumRealBuilding(GameInfoTypes["BUILDING_OMAN_TRAIT"], numDomesticRoutes) 
				end
			end
		end
	end
end

if bIsActive then
	GameEvents.PlayerDoTurn.Add(JFD_MinaaNavalProduction)	
end
--]]
--- Cuba ua
--[[
local iCiv = GameInfoTypes["CIVILIZATION_CUBA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
]]
-- UB: Dance Hall
--[[
function DanceHallDo(iPlayer)
    local pPlayer = Players[iPlayer]
    if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_CUBA"] then
        for pCity in pPlayer:Cities() do
            if pCity:IsHasBuilding(GameInfoTypes.BUILDING_DANCE_HALL) then
                if pCity:GetNumGreatWorksInBuilding(GameInfoTypes.BUILDINGCLASS_OPERA_HOUSE) > 0 then
                    pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_CUBA_TRAIT_UB, 1)
                else
                    pCity:SetNumRealBuilding(GameInfoTypes.BUILDING_CUBA_TRAIT_UB, 0)
                end
            end
        end
    end
end
-- UA
function CubaCultureYoink(iPlayer) --- Fixed my original broken code by LeeS(Master of the Galaxy)
    local player = Players[iPlayer]
    if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_CUBA"] then
        local capital = player:GetCapitalCity()
        if (capital ~= nil) then
            local iNumToSet = 0
            for i = 0, GameDefines.MAX_MAJOR_CIVS - 1, 1 do
                if (iPlayer ~= i) then
                    local pOtPlayer = Players[i]
                    if (pOtPlayer ~= nil) and pOtPlayer:IsEverAlive() and pOtPlayer:IsAlive() and (pOtPlayer:GetCapitalCity() ~= nil) and Teams[player:GetTeam()]:IsHasMet(pOtPlayer:GetTeam()) then
                        print("has met a civ, begin stealing their culture...")
                        local otherCapital = pOtPlayer:GetCapitalCity()
                        iNumToSet = (iNumToSet +  math.floor(otherCapital:GetBaseJONSCulturePerTurn() * .20) )
                    end
                end
            end
            capital:SetNumRealBuilding(GameInfoTypes.BUILDING_CUBA_TRAIT_UB2, iNumToSet)
        end
    end
end
-- UU

function UUnlock(iPlayer, policyTypeID)
	local player = Players[iPlayer]
	local capital = player:GetCapitalCity()
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_CUBA"] then
		local policyLevel = GameInfo.Policies[policyTypeID].Level
		if policyLevel == 1 then
			DummyPolicyCuba(player)
			capital:SetNumRealBuilding(GameInfoTypes.BUILDING_CUBA_TRAIT_UB3, 1)
		end
	end
end

if bIsActive then
	GameEvents.PlayerAdoptPolicy.Add(UUnlock);
	 --GameEvents.PlayerDoTurn.Add(DanceHallDo)
	 GameEvents.PlayerDoTurn.Add(CubaCultureYoink)
end


-- bolivia UA. Couldn't bother with the button UI, too much of a time sink ~EAP ---- UPDATE, didn't work in the end anyways. ~EAP
local iCiv = GameInfoTypes["CIVILIZATION_BOLIVIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
if bIsActive then
	local lastExpended
end

function IsPersonExpended(iPlayer, iUnit)
	print("working: Bolivia")
	
	local pPlayer = Players[iPlayer];
	local pCity = pPlayer:GetCapitalCity();
    if (pPlayer:IsEverAlive()) then
		print("found a civ")
        if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_BOLIVIA"] then
			print("found Bolivia")
			local ArtUnitID = GameInfoTypes["UNIT_ARTIST"]
			if (iUnit == ArtUnitID) then
				print("found the Unit -> Artist")
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 1);
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 0);
				if pPlayer:IsHuman() and pPlayer:GetCivilizationType() == iCiv and Game.GetActivePlayer() == iPlayer then
				Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_RIGHT"))
				end
				lastExpended = ArtUnitID
			end
			local iWriter = GameInfoTypes["UNIT_WRITER"]
			if (iUnit == iWriter) then
				print("found the Unit -> Writer")
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 1);
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 0);
				if (pPlayer:IsHuman() and pPlayer:GetCivilizationType() == iCiv and Game.GetActivePlayer() == iPlayer) then
				Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_LEFT"))
				end
				lastExpended = iWriter
			end
		end
	end
end

local function JFD_BoliviaBelzu_PlayerDoTurn(playerID)
	
	local player = Players[playerID]
--COLORADO -- From JDF/Leugi
	local unitColoradoID			= GameInfoTypes["UNIT_COLORADO"]
	local unitColoradoStrengthBase	= GameInfo.Units[unitColoradoID].Combat
	local numHappiness = player:GetExcessHappiness()
	local numBonusCombatStrength = g_MathMin(g_GetRound(numHappiness/5))
	local unitColoradoStrength = 40
	if (numBonusCombatStrength > 0 ) then
		unitColoradoStrength = unitColoradoStrengthBase + (numBonusCombatStrength * 2)
	end	
	for unit in player:Units() do
		if unit:GetUnitType() == unitColoradoID then
			unit:SetBaseCombatStrength(unitColoradoStrength)
		end
	end
end
	GameEvents.PlayerDoTurn.Add(JFD_BoliviaBelzu_PlayerDoTurn)
	GameEvents.UnitSetXY.Add(JFD_BoliviaBelzu_PlayerDoTurn)
if bIsActive then
	GameEvents.GreatPersonExpended.Add(IsPersonExpended)
end
-- mongolia additional UA. This is where the spice begins ~EAP

local iCiv = GameInfoTypes["CIVILIZATION_MONGOL"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if bIsActive then
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: mongolia ontechbonus")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MONGOL"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_CHIVALRY"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TOGTVORTOI"], 1);
					for pCity in player:Cities() do
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TOGTVORTOI"], 1) end
				end
			end
		end
	end
end);

end
-- Romania new UA Culture from GA  (orignal code from DJS)

local iCiv = GameInfoTypes["CIVILIZATION_ROMANIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function RomaniaGACulture(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ROMANIA"] and player:IsAlive() then
		for city in player:Cities() do
			if (city:GetNumBuilding(GameInfoTypes["BUILDING_ROMANIA_TRAIT"]) > 0) then
				city:SetNumRealBuilding(GameInfoTypes["BUILDING_ROMANIA_TRAIT"], 0)
			end
			if player:IsGoldenAge() then
				city:SetNumRealBuilding(GameInfoTypes["BUILDING_ROMANIA_TRAIT"], 1)
			end
		end
	end
end
if bIsActive then
GameEvents.PlayerDoTurn.Add(RomaniaGACulture)
end

-- Maori Promotion (UA)
-- Code by EnormousApplePie

local iCiv = GameInfoTypes["CIVILIZATION_MAORI"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

local unitPromotionStaticID	= GameInfoTypes["PROMOTION_MAORI_DULL"]
local unitPromotionActiveID	= GameInfoTypes["PROMOTION_MAORI"]
local unitPromotionActiveCivilID	= GameInfoTypes["PROMOTION_MAORI_CIVILIAN"]

function EAP_Maori_Turn(playerID)
	local player = Players[playerID]
	local playerTeam = Teams[player:GetTeam()]
	local Gameturn = Game.GetGameTurn()
	if (not player:IsAlive()) then return end
	if player:IsBarbarian() then return end
	if (not player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MAORI"]) then return end
	for unit in player:Units() do
		local isPromotionValid = false
		if Gameturn >= 5 then 
			isPromotionValid = true
			--print("It is turn 9 or later")
		end
		if isPromotionValid then
			if unit:IsHasPromotion(unitPromotionActiveID) then
				unit:SetHasPromotion(unitPromotionActiveID, false)
			elseif unit:IsHasPromotion(unitPromotionActiveCivilID) then
				unit:SetHasPromotion(unitPromotionActiveCivilID, false)
			end
		end
	end
end
if bIsActive then
GameEvents.PlayerDoTurn.Add(EAP_Maori_Turn)
end



--local iairgunID = GameInfo.Units.UNIT_HELICOPTER_GUNSHIP
--local iairshipID = GameInfo.Units.UNIT_AIRSHIP

--function EAP_EMBARK_FIX(iPlayer, iUnit, iNewUnit)
    --local pPlayer = Players[iPlayer];
    --if (pPlayer:IsEverAlive()) then
		--pUnit = pPlayer:GetUnitByID(iNewUnit);
        --if (pUnit:GetUnitType() == iairshipID) or (pUnit:GetUnitType() == iairgunID) then
           -- pUnit:SetHasPromotion(unitEmbarkPromotionID, false)
			--print("embarkation removed")
		--end
   -- end
--end
--GameEvents.UnitUpgraded.Add(EAP_EMBARK_FIX)


-- New Zealand UA 
-- Original code by JDF

local iCiv = GameInfoTypes["CIVILIZATION_NEW_ZEALAND"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
     
function GiveBonus(nzPlayer, oPlayer)
	print("[GiveBonus] A meeting bonus has been granted")
        local rewardCulture = 6
        local rewardScience = 12
        local rewardGold = 40
        local rewardFaith = 10
	local random = GetRandom(1, 4)
        if random == 1 then
        	nzPlayer:ChangeFaith(rewardFaith)
		if nzPlayer:IsHuman() and nzPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NEW_ZEALAND"] and Game.GetActivePlayer() == nzPlayer:GetID() then
			Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_PEACE] Faith[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardFaith, oPlayer:GetName()))
		end

                elseif random == 2 then
                	nzPlayer:ChangeJONSCulture(rewardCulture)
			if nzPlayer:IsHuman() and nzPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NEW_ZEALAND"] and Game.GetActivePlayer() == nzPlayer:GetID() then
				Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_CULTURE] Culture[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardCulture, oPlayer:GetName()))
			end
                elseif random == 3 then
                        Teams[nzPlayer:GetTeam()]:GetTeamTechs():ChangeResearchProgress(nzPlayer:GetCurrentResearch(), rewardScience, playerID)
			if nzPlayer:IsHuman() and nzPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NEW_ZEALAND"] and Game.GetActivePlayer() == nzPlayer:GetID() then
				Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_RESEARCH] Science[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardScience, oPlayer:GetName()))
			end
                else
                        nzPlayer:ChangeGold(rewardGold)
			if nzPlayer:IsHuman() and nzPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NEW_ZEALAND"] and Game.GetActivePlayer() == nzPlayer:GetID() then
				Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_GOLD] Gold[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardGold, oPlayer:GetName()))
			end            
         end
end
     
function NZMeetBonus(iTeamMet, iTeam)
	local pTeam = Teams[iTeam]
	local pTeamMet = Teams[iTeamMet]
	local newzPlayerID = 21
	for oPlayerID = 0, GameDefines.MAX_MAJOR_CIVS-1, 1 do
		local oPlayer = Players[oPlayerID]
		if oPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_NEW_ZEALAND"] then
			newzPlayerID = oPlayer:GetID()
		end
	end

	local oTeamID = 21
	local newzPlayer = Players[newzPlayerID]
	if newzPlayer:GetTeam() == iTeam then
		oTeamID = iTeamMet
	elseif newzPlayer:GetTeam() == iTeamMet then
		oTeamID = iTeam
	else
		return
	end

	local oTeam = Teams[oTeamID]
	if oTeam:IsMinorCiv() then
		GiveBonus(newzPlayer, oTeam)
	else	
		for oPlayerID = 0, GameDefines.MAX_MAJOR_CIVS-1, 1 do
			local oPlayer = Players[oPlayerID]
			if oPlayer:GetTeam() == oTeamID then
				GiveBonus(newzPlayer, oPlayer)
			end
		end
	end
end

-- New Zealand UU infantry Influence
-- Code by MC and JFD

include("FLuaVector.lua")
-- JFD_IsInCityStateBorders
function JFD_IsInCityStateBorders(playerID, unit)
	local plot = unit:GetPlot()
	if plot:GetOwner() > -1 then
		local player = Players[plot:GetOwner()]
		return (player:IsMinorCiv() and player:GetMinorCivFriendshipLevelWithMajor(playerID) >= 1)
	end
	return false
end

-- MC_MaoriBattalion
local unitPromotionMaoriID = GameInfoTypes["PROMOTION_ATTACK_AWAY_CAPITAL"]
function MC_MaoriBattalion(playerID)
	local player = Players[playerID]
	if (player:IsAlive() and (not player:IsBarbarian())) then	
		for unit in player:Units() do
			if unit:IsHasPromotion(unitPromotionMaoriID) then
				if JFD_IsInCityStateBorders(playerID, unit) then
					local minorPlayerID = unit:GetPlot():GetOwner()
					local minorPlayer = Players[minorPlayerID]
					minorPlayer:ChangeMinorCivFriendshipWithMajor(playerID, 1)
					if player:IsHuman() and Game.GetActivePlayer() == playerID then
						local hex = ToHexFromGrid(Vector2(unit:GetX(), unit:GetY()))
						Events.AddPopupTextEvent(HexToWorld(hex), Locale.ConvertTextKey("[ICON_WHITE]+{1_Num}[ENDCOLOR] [ICON_INFLUENCE]", 1), true)
					end
				end
			end
		end
	end
end


-- New Zealand UU Ironclad promo near Cities
-- Code by JDF

local unitPromotionDefenderActiveID	= GameInfoTypes["PROMOTION_JFD_DEFENDER_ACTIVE"]
local unitPromotionDefenderID		= GameInfoTypes["PROMOTION_JFD_DEFENDER"]
function JFD_NewZealand_Defender_PlayerDoTurn(playerID)
	local player = Players[playerID]
	local playerTeam = Teams[player:GetTeam()]
	if (not player:IsAlive()) then return end
	if player:IsBarbarian() then return end
	for unit in player:Units() do
		local isPromotionValid = false
		if (unit and (unit:IsHasPromotion(unitPromotionDefenderActiveID) or unit:IsHasPromotion(unitPromotionDefenderID))) then
			local plot = unit:GetPlot()
			if (plot and plot:IsPlayerCityRadius(playerID)) then 
				isPromotionValid = true
			else
				for otherPlayerID = 0, GameDefines.MAX_MAJOR_CIVS-1, 1 do
					local otherPlayer = Players[otherPlayerID]
					if (otherPlayer:IsAlive() and otherPlayerID ~= playerID) then	
						local otherTeamID = otherPlayer:GetTeam()
						if otherPlayer:IsDoF(player:GetTeam()) then
							if plot:IsPlayerCityRadius(otherPlayerID) then 
								isPromotionValid = true
								break
							end
						end
					end
				end
			end
		end
		if isPromotionValid then
			if (not unit:IsHasPromotion(unitPromotionDefenderActiveID)) then
				unit:SetHasPromotion(unitPromotionDefenderActiveID, true)
				unit:SetHasPromotion(unitPromotionDefenderID, false)
			end
		else
			if unit:IsHasPromotion(unitPromotionDefenderActiveID) then
				unit:SetHasPromotion(unitPromotionDefenderActiveID, false)
				unit:SetHasPromotion(unitPromotionDefenderID, true)
			end
		end
	end
end

if bIsActive then
	GameEvents.TeamMeet.Add(NZMeetBonus)
	GameEvents.PlayerDoTurn.Add(MC_MaoriBattalion)
	GameEvents.PlayerDoTurn.Add(JFD_NewZealand_Defender_PlayerDoTurn)
end
	-- Tongo UB food
	-- Code by JFD
	-- Hoped to edit it somewhat and make it slightly more original, but I couldn't quite change it without bringing down the balance ~EAP
	local iCiv = GameInfoTypes["CIVILIZATION_TONGA"]
	local bIsActive = JFD_IsCivilisationActive(iCiv)

	local direction_types = {
            DirectionTypes["DIRECTION_NORTHEAST"],
            DirectionTypes["DIRECTION_NORTHWEST"],
            DirectionTypes["DIRECTION_EAST"],
            DirectionTypes["DIRECTION_SOUTHEAST"],
            DirectionTypes["DIRECTION_SOUTHWEST"],
            DirectionTypes["DIRECTION_WEST"]
            }
     
    function JFD_GetNumAdjacentSeaTiles(city)
            local numAdjacentSeaTiles = 0
            if Map.GetPlot(city:GetX(), city:GetY()) then
                    for loop, direction in ipairs(direction_types) do
                            local adjPlot = Map.PlotDirection(city:GetX(), city:GetY(), direction)
                            if adjPlot:GetTerrainType() == GameInfoTypes["TERRAIN_COAST"] then     
                                    numAdjacentSeaTiles = numAdjacentSeaTiles + 1
                            end
                    end
            end
           
            return numAdjacentSeaTiles     
    end
     
    function JFD_MalaeFood(playerID, unitID, unitX, unitY)
            local player = Players[playerID]
            if player:IsAlive() then
                    for city in player:Cities() do
                            if city:IsHasBuilding(GameInfoTypes["BUILDING_MC_TONGAN_MALAE"]) and JFD_GetNumAdjacentSeaTiles(city) >= 3 then
                                    city:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_MALAE_FOOD"], 1)
                            end
                    end
            end
    end

if bIsActive then
   GameEvents.PlayerDoTurn.Add(JFD_MalaeFood)
end

-- Aksum UA 
-- Ah, Optimization! Thanks JFD!
local iCiv = GameInfoTypes["CIVILIZATION_AKSUM"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if JFD_IsCivilisationActive(iCiv) then
	print("Aksum is in this game")
end

-- Aksum UA function (new)
function AllCitiesFollowReligionOnFound(ePlayer, iCity, eReligion)
    print("working: aksum 1")
    local player = Players[ePlayer]
    if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_AKSUM"] then
        print("working: aksum 2")
        for iCity in player:Cities() do
            if (not iCity:IsHolyCityForReligion(eReligion)) then
                print("aksum: adopting religion in city")
                iCity:AdoptReligionFully(eReligion)
            end        
        end
    end
end

if bIsActive then
	GameEvents.ReligionFounded.Add(AllCitiesFollowReligionOnFound)
end

-- Kilwa UA 
-- Ah, Optimization! Thanks JFD!
local iCiv = GameInfoTypes["CIVILIZATION_KILWA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)


-- Kilwa UA function 
function GetTradeRoutesNumber(player, city)
	print("working: kilwa 1")
	local tradeRoutes = player:GetTradeRoutes()
	local numRoutes = 0
	for i, v in ipairs(tradeRoutes) do
		local originatingCity = v.FromCity
		if (originatingCity == city) and (v.FromID ~= v.ToID) then -- Thnx TophatPalading for fixing my stuff <3 
			numRoutes = numRoutes + 1
		end
	end
	
	return numRoutes
end

local buildingCoralPortID = GameInfoTypes["BUILDING_CORALSHOP"]
local buildingTraitKilwaID = GameInfoTypes["BUILDING_KILWA_TRAIT"]
function KilwaTrait(playerID)
	print("working: kilwa 2")
	local player = Players[playerID]
    if player:IsEverAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_KILWA"] then 
		for city in player:Cities() do
			city:SetNumRealBuilding(buildingTraitKilwaID, math.min(GetTradeRoutesNumber(player, city), 420)) -- I wonder if this will work (note: it does)
		end
	end
end

if bIsActive then
	GameEvents.PlayerDoTurn.Add(KilwaTrait)
end
-- GAULS UA 
local iCiv = GameInfoTypes["CIVILIZATION_GAUL"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
if bIsActive then
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: gaul ontechbonus")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_GAUL"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_MINING"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MURUS_GALLICUS"], 1);
				end
			end
		end
	end
end);
end



-- Ottoman new UA addition
-- Code by Uighur_Caesar

include("FLuaVector.lua")

local ottoID = GameInfoTypes.CIVILIZATION_OTTOMAN

function OttoPromotion(ownerId, unitId, ePromotion)
local player = Players[ownerId]
	if player:IsAlive() and player:GetCivilizationType() == ottoID then
	local unit = player:GetUnitByID(unitId)
	local xp = unit:GetExperience()
	local needed = unit:ExperienceNeeded()
	local faith = math.ceil(needed / 3)
	player:ChangeFaith(faith)
	if player:IsHuman() and Game.GetActivePlayer() == ownerId then
		local hex = ToHexFromGrid(Vector2(unit:GetX(), unit:GetY()))
		Events.AddPopupTextEvent(HexToWorld(hex), Locale.ConvertTextKey("+{1_Num}[ENDCOLOR] [ICON_PEACE]", faith), true)
			end
		end
	end
GameEvents.UnitPromoted.Add(OttoPromotion)



-- Tibet new Trait UA

local iCiv = GameInfoTypes["CIVILIZATION_TIBET"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if bIsActive then
print("loaded tibet ua")
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: tibet UA 25%")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_TIBET"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_DRAMA"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET"], 1);
				end
			end
		end
	end
end)
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: tibet UA 50%")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_TIBET"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_ACOUSTICS"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET2"], 1);
				end
			end
		end
	end
end)
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: tibet UA 75%")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_TIBET"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_RADIO"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET3"], 1);
				end
			end
		end
	end
end)
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: tibet UA 100%")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_TIBET"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_GLOBALIZATION"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_TIBET4"], 1);
				end
			end
		end
	end
end)
end
]]

local iCiv = GameInfoTypes["CIVILIZATION_VENEZ"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
--[[
-- Venez trait tech research
print("loaded venez ua")
if bIsActive then
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: venez UA")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_VENEZ"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_COMPASS"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_VENEZ_TRAIT"], 1);
				end
			end
		end
	end
end)
end

--Horde

local civHorde = GameInfoTypes["CIVILIZATION_GOLDEN_HORDE"];
local hordeIsActive = JFD_IsCivilisationActive(civHorde);

local hordePolicy = GameInfoTypes["POLICY_DUMMY_HORDE"];
local puppetBuilding = GameInfoTypes["BUILDING_HORDE_TRAIT"];
local puppetBuildingHappy = GameInfoTypes["BUILDING_HORDE_TRAIT_HAPPY"];

function HordeUA(playerID)
	local player = Players[playerID];
	
	if player:GetCivilizationType() == civHorde and player:IsAlive() then
		for pCity in player:Cities() do
			if pCity:IsPuppet() then
				pCity:SetNumRealBuilding(puppetBuilding, 1);
			
				local pop = pCity:GetPopulation();
				local unhappy = pop + 3;
			
				pCity:SetNumRealBuilding(puppetBuildingHappy, unhappy/3);
			else
				pCity:SetNumRealBuilding(puppetBuilding, 0);
				pCity:SetNumRealBuilding(puppetBuildingHappy, 0);
			end
		end
	end
end

if hordeIsActive then
GameEvents.PlayerDoTurn.Add(HordeUA);
end
--]]
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------- DUMMY POLICIES ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Cuba dummy policy

--local iCiv = GameInfoTypes["CIVILIZATION_CUBA"]
--local bIsActive = JFD_IsCivilisationActive(iCiv)
--[[
print("dummy policy loaded - Cuba")
function DummyPolicyCuba(player)
	print("working - Cuba")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_CUBA"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"]) then
				if player:GetNumFreePolicies() == 0 then
					player:SetNumFreePolicies(1)
					player:SetNumFreePolicies(0)
					player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
				elseif player:GetNumFreePolicies() == 1 then
					player:SetNumFreePolicies(2)
					player:SetNumFreePolicies(1)
					player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
				elseif player:GetNumFreePolicies() == 2 then
					player:SetNumFreePolicies(3)
					player:SetNumFreePolicies(2)
					player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
				elseif player:GetNumFreePolicies() == 3 then
					player:SetNumFreePolicies(4)
					player:SetNumFreePolicies(3)
					player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
				elseif player:GetNumFreePolicies() >= 4 then
					player:SetNumFreePolicies(5)
					player:SetNumFreePolicies(4)
					player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_CUBA"], true)
				end
			end
		end
	end 
end
-- AKKAD UA --

include("PlotIterators")
--_________________________________________________________________________________________________________________________________________________________________________________________________________
local iCiv = GameInfoTypes["CIVILIZATION_AKKAD"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
--_________________________________________________________________________________________________________________________________________________________________________________________________________
local civilizationID = GameInfoTypes["CIVILIZATION_AKKAD"]
local promotionID = GameInfoTypes["PROMOTION_LITE_AKKAD_CITY_BONUS"]
local greatGeneralPoints = 2
--_________________________________________________________________________________________________________________________________________________________________________________________________________
--GREAT GENERAL BONUS AGAINST CITIES
--_________________________________________________________________________________________________________________________________________________________________________________________________________
function liteGreatGeneralBonusReset(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == civilizationID then
		local greatGenerals = {}
		for unit in player:Units() do
			if unit:IsHasPromotion(promotionID) then
				unit:SetHasPromotion(promotionID, false)
			end
			if unit:IsHasPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"]) then
				table.insert(greatGenerals, unit)
			end
		end
		for key,greatGeneral in pairs(greatGenerals) do 
			local plot = greatGeneral:GetPlot()
			for loopPlot in PlotAreaSweepIterator(plot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
				for i = 0, loopPlot:GetNumUnits() - 1, 1 do  
					local otherUnit = loopPlot:GetUnit(i)
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsCombatUnit() and not(otherUnit:GetDomainType() == DomainTypes.DOMAIN_SEA) then
						otherUnit:SetHasPromotion(promotionID, true)
					end
				end
			end
		end
	end
end

function liteGreatGeneralBonusAgainstCities(playerID, unitID, unitX, unitY)
	local player = Players[playerID]
	if player:GetCivilizationType() == civilizationID and (player:GetUnitClassCount(GameInfoTypes["UNITCLASS_GREAT_GENERAL"]) > 0) then
		local unit = player:GetUnitByID(unitID)
		local plot = unit:GetPlot()
		if unit:IsHasPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"]) then
			for loopPlot in PlotAreaSweepIterator(plot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
				for i = 0, loopPlot:GetNumUnits() - 1, 1 do  
					local otherUnit = loopPlot:GetUnit(i)
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsCombatUnit() and not(otherUnit:GetDomainType() == DomainTypes.DOMAIN_SEA) then
						otherUnit:SetHasPromotion(promotionID, true)
					end
				end
			end
		elseif unit:IsCombatUnit() and not(unit:GetDomainType() == DomainTypes.DOMAIN_SEA) then
			unit:SetHasPromotion(promotionID, false)
			for loopPlot in PlotAreaSweepIterator(plot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
				for i = 0, loopPlot:GetNumUnits() - 1, 1 do  
					local otherUnit = loopPlot:GetUnit(i)
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsHasPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"]) then
						unit:SetHasPromotion(promotionID, true)
					end
				end
			end
		end
	end
end




if bIsActive then
GameEvents.PlayerDoTurn.Add(liteGreatGeneralBonusReset)
GameEvents.UnitSetXY.Add(liteGreatGeneralBonusAgainstCities)
end
--]]
--_________________________________________________________________________________________________________________________________________________________________________________________________________








----////--------------------------------------------------

----////--------------------------------------------------

--local OldUnitType = "UNIT_WORKER"
--local oldUnitID = GameInfo.Units.UNIT_WORKER.ID
--local newUnitID = GameInfo.Units.UNIT_ARGENTINA_WORKER.ID
--local iCivType = GameInfo.Civilizations["CIVILIZATION_AYYUBID"].ID

--function AyyuOverride(iPlayer, iUnit)
  --  local pPlayer = Players[iPlayer];
  --  if (pPlayer:IsEverAlive()) then
    --    if (pPlayer:GetCivilizationType() == iCivType) then
     --  	    if pPlayer:GetUnitByID(iUnit) ~= nil then
	--	pUnit = pPlayer:GetUnitByID(iUnit);
           --     if (pUnit:GetUnitType() == oldUnitID) then
            --        local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
              --      newUnit:Convert(pUnit);
			--		newUnit:SetMoves(pUnit:MaxMoves())
               -- end
           -- end
       -- end
    --end
--end

--Events.SerialEventUnitCreated.Add(AyyuOverride)



--========================================================================================
--========================================================================================
---------------------
-- On Policy Adopted stuff (uncludes UA's , easier for me to copy pasta stuff <3)
---------------
--========================================================================================
--========================================================================================

--[[
--================
-- RESETTLEMENTS
--================

local iKilwa = GameInfo.Civilizations["CIVILIZATION_KILWA"].ID
local iIroq = GameInfo.Civilizations["CIVILIZATION_IROQUOIS"].ID

local iBulg = GameInfo.Civilizations["CIVILIZATION_BULGARIA"].ID
local iColom = GameInfo.Civilizations["CIVILIZATION_COLOMBIA"].ID
local iTonga = GameInfo.Civilizations["CIVILIZATION_TONGA"].ID

local iMaurya = GameInfo.Civilizations["CIVILIZATION_MAURYA"].ID
local iEtho = GameInfo.Civilizations["CIVILIZATION_ETHIOPIA"].ID

local iAkkad = GameInfo.Civilizations["CIVILIZATION_AKKAD"].ID
local iAssy = GameInfo.Civilizations["CIVILIZATION_ASSYRIA"].ID

-- Add ResettlementsBuilding to newly founded cities
function ResettlementsNewCities(iPlayer, iCityX, iCityY)
    local player = Players[iPlayer]
    if (player:HasPolicy(GameInfo.Policies["POLICY_RESETTLEMENT"].ID)) then
        for loopCity in player:Cities() do
            if (loopCity:GetX() == iCityX) then
                if (loopCity:GetY() == iCityY) then
					if player:GetCivilizationType() == iKilwa then 
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_CORALSHOP"], 1)
					elseif player:GetCivilizationType() == iIroq then
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_LONGHOUSE"], 1)
					else
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_WORKSHOP"], 1)
					end
					if player:GetCivilizationType() == iBulg then 
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_KHAMBAR"], 1)
					elseif player:GetCivilizationType() == iColom then
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_COLOMBIAN_GRANARY"], 1)
					elseif player:GetCivilizationType() == iTonga then
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_TONGAN_MALAE"], 1)
					else
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GRANARY"], 1)
					end
					if player:GetCivilizationType() == iMaurya then 
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ASHOKA"], 1)
					elseif player:GetCivilizationType() == iEtho then
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_STELE"], 1)
					else
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MONUMENT"], 1)
					end
					if player:GetCivilizationType() == iAkkad then 
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_AKKAD_LIBRARY"], 1)
					elseif player:GetCivilizationType() == iAssy then
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ROYAL_LIBRARY"], 1)
					else
						loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_LIBRARY"], 1)
					end
					loopCity:SetNumRealBuilding(GameInfoTypes["BUILDING_AQUEDUCT"], 1)
                end
            end
        end
    end
end
GameEvents.PlayerCityFounded.Add(ResettlementsNewCities)

--================
-- Italy ua
--================
local iCiv = GameInfoTypes["CIVILIZATION_ITALY"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]
    local policy = GameInfo.Policies[policyID]
    local policyBranch = GameInfo.PolicyBranchTypes[policy.PolicyBranchType]

	if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"]) and (player:IsEverAlive()) then
		if policyBranch ~= nil then
			if player:HasPolicy(policyID) then
				if player:IsPolicyBranchFinished(policyBranch.ID) == true then 

					--amount made for quick speed, but scales with other speeds.
					local iGoldenageturns = 5 * (GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100);
					local iGoldenagepoints = 312.5 * (GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100)
					if Game.GetActivePlayer() == playerID then
						if player:IsGoldenAge() then
							player:ChangeGoldenAgeTurns(iGoldenageturns)
							if iGoldenageturns == 1 then
								Events.GameplayAlertMessage("Your people become inspired by your political prowess! " .. iGoldenageturns .. " turn has been added to your [ICON_GOLDEN_AGE] Golden Age!")
							else
								Events.GameplayAlertMessage("Your people become inspired by your political prowess! " .. iGoldenageturns .. " turns have been added to your [ICON_GOLDEN_AGE] Golden Age!")
							end
						else
							player:ChangeGoldenAgeProgressMeter(iGoldenagepoints)
							Events.GameplayAlertMessage("Your people become inspired by your political prowess! " .. iGoldenagepoints .. " [ICON_GOLDEN_AGE] Golden Age points have been added to your [ICON_GOLDEN_AGE] Golden Age progress!")
						end
					end
				end
			end
		end	
	end
end
if bIsActive then
	GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end
]]
-- HonorChanges
-- Author: Cirra
-- DateCreated: 7/27/2019 1:22:18 AM
--------------------------------------------------------------
--[[
function Honor_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Honor Finisher
	if	(policyID == GameInfo.Policies["POLICY_DISCIPLINE"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_CASTE"].ID)) or
		(policyID == GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_DISCIPLINE"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_CASTE"].ID)) or
		(policyID == GameInfo.Policies["POLICY_MILITARY_CASTE"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_DISCIPLINE"].ID)) then

		-- The player has finished Honor. Add old ToA to the capital, which gives +10% food everywhere.
		local pCity = player:GetCapitalCity();
		pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_HONOR_FINISHER"], 1);

	end

end
GameEvents.PlayerAdoptPolicy.Add(Honor_OnPolicyAdopted)

--================
-- Huey Teocalli
--================
local lakeWonder = GameInfoTypes["BUILDING_LAKE_WONDER"]

function LakeWonderRequireLake(playerID, cityID, buildingType)
    local player = Players[playerID]
    local pCity = player:GetCityByID(cityID)
    
    if buildingType == lakeWonder then
        local isNearLake = false
            
        for pAdjacentPlot in PlotAreaSweepIterator(pCity:Plot(), 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
            if pAdjacentPlot:IsLake() or pAdjacentPlot:GetFeatureType() == GameInfoTypes["FEATURE_LAKE_VICTORIA"] and pAdjacentPlot:GetOwner() == playerID then
                isNearLake = true
            end
            if isNearLake then
                break
            end
        end
        
        if isNearLake then
            return true
        else
            return false
        end
    end
    
    return true
end

GameEvents.CityCanConstruct.Add(LakeWonderRequireLake);
--]]
--================
-- Workboat for AI
--================
--[[
local workBoat = GameInfoTypes["UNIT_WORKBOAT"]

function WorkboatforAI(playerID, cityID, unitType)
    local player = Players[playerID]
    local pCity = player:GetCityByID(cityID)
    
    if unitType == workBoat then
        if (not player:IsHuman()) then
			return true
		else return false
		end
    end
    return true
   
end

GameEvents.CityCanTrain.Add(WorkboatforAI);

--=================================================================================================================
-- Georgia
--=================================================================================================================

local civGeorgia = GameInfoTypes["CIVILIZATION_GEORGIA"];
local georgiaIsActive = JFD_IsCivilisationActive(civGeorgia);

local policyTrait = GameInfoTypes["POLICY_DUMMY_GEORGIA"];

function GeorgiaDummyPolicy(playerID)
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == civGeorgia then
			if not player:HasPolicy(policyTrait) then
				player:SetNumFreePolicies(1);
				player:SetNumFreePolicies(0);
				player:SetHasPolicy(policyTrait, true);
			end
		end
	end
end

-- used some code from Ummayyad by Uighur_Caesar
function CheckForFaithBuildings(player, pCity)
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_CATHEDRAL_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOSQUE_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_PAGODA_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GURDWARA_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_SYNAGOGUE_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_VIHARA_GEORGIA_DUMMY"], 0);
	pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MANDIR_GEORGIA_DUMMY"], 0);

	if player:IsAlive() and player:GetCivilizationType() == civGeorgia then
	
	local religion = pCity:GetReligiousMajority();
	if religion > 0 then
		for i,v in ipairs(Game.GetBeliefsInReligion(religion)) do
			if GameInfoTypes["BELIEF_CATHEDRALS"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_CATHEDRAL_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_MOSQUES"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MOSQUE_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_PAGODAS"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_PAGODA_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_GURDWARAS"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GURDWARA_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_SYNAGOGUES"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_SYNAGOGUE_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_VIHARAS"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_VIHARA_GEORGIA_DUMMY"], 1);
			elseif GameInfoTypes["BELIEF_MANDIRS"] == v then
				pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MANDIR_GEORGIA_DUMMY"], 1);
			end
		end
	end
	
	end
end

function GeorgiaUA(playerID)
	local player = Players[playerID];
	for pCity in player:Cities() do
		CheckForFaithBuildings(player, pCity);
	end
end

function GeorgiaUAOnConvert(playerID, religion, x, y)
	CheckForFaithBuildings(Players[playerID], Map.GetPlot(x, y):GetPlotCity());
end

function GeorgiaGA(playerID)
	local player = Players[playerID];
	if player:GetCivilizationType() == civGeorgia and player:IsAlive() then
		-- UU
		for pUnit in player:Units() do
			if pUnit:GetUnitType() == GameInfoTypes["UNIT_GEORGIA_KHEVSUR"] and not pUnit:IsDead() then
				if player:IsGoldenAge() and not pUnit:IsHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"]) then	
					pUnit:SetHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"], true);
				elseif not player:IsGoldenAge() and pUnit:IsHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"])then
					pUnit:SetHasPromotion(GameInfoTypes["PROMOTION_GEORGIA_KHEVSUR_GA"], false);
				end
			end
		end
	end 
end

function GeorgiaGAMove(playerID, unitID, x, y)
	GeorgiaGA(playerID);
end

function GeorgiaDisallowFaithBuildings(playerID, cityID, buildingType)
	local player = Players[playerID];
	if player:GetCivilizationType() == civGeorgia then
		if buildingType == GameInfoTypes["BUILDING_CATHEDRAL"] or buildingType == GameInfoTypes["BUILDING_MOSQUE"] or buildingType == GameInfoTypes["BUILDING_PAGODA"] or buildingType == GameInfoTypes["BUILDING_GURDWARA"] or buildingType == GameInfoTypes["BUILDING_SYNAGOGUE"] or buildingType == GameInfoTypes["BUILDING_VIHARA"] or buildingType == GameInfoTypes["BUILDING_MANDIR"] then
			return false;
		end
	end
	return true;
end

if georgiaIsActive then
GameEvents.PlayerDoTurn.Add(GeorgiaGA);
GameEvents.PlayerDoTurn.Add(GeorgiaUA);
GameEvents.CityConvertsReligion.Add(GeorgiaUAOnConvert);
Events.SequenceGameInitComplete.Add(GeorgiaDummyPolicy);
GameEvents.UnitSetXY.Add(GeorgiaGAMove);
GameEvents.CityCanConstruct.Add(GeorgiaDisallowFaithBuildings);

end



--=================================================================================================================
-- Palmyra
--=================================================================================================================

local civPalmyra = GameInfoTypes["CIVILIZATION_PALMYRA"];
local palmyraIsActive = JFD_IsCivilisationActive(civPalmyra);

local freshWaterFeature = GameInfoTypes["FEATURE_WATER_DUMMY"];
local freshWaterFeatureHill = GameInfoTypes["FEATURE_WATER_DUMMY_HILL"];
local freshWaterFeatureRes = GameInfoTypes["FEATURE_WATER_DUMMY_RES"];
local freshWaterFeatureSheep = GameInfoTypes["FEATURE_WATER_DUMMY_SHEEP"];

-- Adds fresh water feature to palmyrene cities
-- used some code to learn things from Yemen by Urdnot, Relic, Darth
function AddFreshWater(player, plot)
	local teamID = player:GetTeam();

	if player:GetCivilizationType() == civPalmyra then
		if plot:IsHills() and plot:GetResourceType() == GameInfoTypes["RESOURCE_SHEEP"] and not (plot:GetTerrainType() == GameInfoTypes["TERRAIN_PLAINS"]) then
			plot:SetFeatureType(freshWaterFeatureSheep);
			return;
		elseif plot:IsHills() and not (plot:GetTerrainType() == GameInfoTypes["TERRAIN_PLAINS"]) then
			plot:SetFeatureType(freshWaterFeatureRes);
			return;
		elseif plot:IsHills() then
			plot:SetFeatureType(freshWaterFeatureHill);
			return;
		else
			plot:SetFeatureType(freshWaterFeature);
			return;
		end
	elseif not player:GetCivilizationType() == civPalmyra then
		if plot:GetFeatureType() == freshWaterFeature or pCity:Plot():GetFeatureType() == freshWaterFeatureHill or pCity:Plot():GetFeatureType() == freshWaterFeatureRes then
			plot:SetFeatureType(iNone);
			return;
		end
	end
end

-- Fresh water during do turn
function FreshWaterTurn(playerID)
	local player = Players[playerID];
	if player:IsAlive() then
		for pCity in player:Cities() do
			AddFreshWater(player, pCity:Plot());
		end
	end
end

if palmyraIsActive then
GameEvents.PlayerDoTurn.Add(FreshWaterTurn);
GameEvents.PlayerCityFounded.Add(FreshWaterTurn);
end

--=================================================================================================================
-- Switzerland
--=================================================================================================================

local civSwiss = GameInfoTypes["CIVILIZATION_SWISS"];
local swissIsActive = JFD_IsCivilisationActive(civSwiss);

local skiResort = GameInfoTypes["BUILDING_SWISS_SKI_RESORT"];

local swissTrait = GameInfoTypes["BUILDING_SWISS_TRAIT"];

local mountaineer = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER"];
local mountaineerActive = GameInfoTypes["PROMOTION_SWISS_MOUNTAINEER_ACTIVE"];

local reislaufer = GameInfoTypes["UNIT_SWISS_REISLAUFER"];

local skiResortM = {GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_1"], GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_2"], GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_3"], GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_4"],GameInfoTypes["BUILDING_SWISS_SKI_RESORT_MOUNTAIN_5"]
}
function SkiResortMountain(playerID)
    local player = Players[playerID];
    
    if player:GetCivilizationType() == civSwiss and player:IsAlive() then
        for pCity in player:Cities() do 
            if pCity:IsHasBuilding(skiResort) then  --and (not pCity:IsHasBuilding(skiResortM))
                local pPlot = pCity:Plot()
                local MountainCount = 0
                for pAdjacentPlot in PlotAreaSweepIterator(pPlot, 3, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
                    
                    if pAdjacentPlot:IsMountain() and pAdjacentPlot:GetOwner() == playerID and MountainCount <= 5 then
                        -- count the mountains, then add a dummy building for the amount counted
                        print(MountainCount)
						MountainCount = MountainCount + 1
                        pCity:SetNumRealBuilding(skiResortM[MountainCount], 1)
                        
                    end
                end
                
            else
               	for iLoop, skiResort in pairs(skiResortM) do
					if pCity:IsHasBuilding(skiResort) then
						pCity:SetNumRealBuilding(skiResortM, 0)
					end
				end
            end
        end
    end
end

function Mountaineer(playerID, unitID, x, y)
	local player = Players[playerID];
	local unit = player:GetUnitByID(unitID);
	
	if unit:IsHasPromotion(mountaineer) then
		local pPlot = Map.GetPlot(unit:GetX(),unit:GetY());
			
		local isNearMountain = false;
			
		for pAdjacentPlot in PlotAreaSweepIterator(pPlot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
			if pAdjacentPlot:IsMountain() then
				isNearMountain = true;
			end
			if isNearMountain then
				break;
			end
		end
		
		if isNearMountain then
			unit:SetHasPromotion(mountaineerActive, true);
		else
			unit:SetHasPromotion(mountaineerActive, false);
		end
	end	
	
	if player:GetCivilizationType() == civSwiss and player:IsAlive() then
		for pUnit in player:Units() do
			if pUnit:GetUnitType() == reislaufer and not pUnit:IsDead() then
				if pUnit:GetLevel() == 0 and pUnit:GetExperience() == 0 then
					pUnit:ChangeLevel(2);
				end
			end
		end
	end
end

function SwissUA(playerID)
	local player = Players[playerID];
	
	if player:GetCivilizationType() == civSwiss and player:IsAlive() then
		for pCity in player:Cities() do
			if pCity:GetSpecialistCount(5) > 0 then
				pCity:SetNumRealBuilding(swissTrait, 1);
			else
				pCity:SetNumRealBuilding(swissTrait, 0);
			end
		end
	end
	
	if player:GetCivilizationType() == civSwiss and player:IsAlive() then
		for pUnit in player:Units() do
			if pUnit:GetUnitType() == reislaufer and not pUnit:IsDead() then
				if pUnit:GetLevel() == 1 and pUnit:GetExperience() == 0 then
					pUnit:ChangeExperience(30);
				end
			end
		end
	end
end

if swissIsActive then
GameEvents.PlayerDoTurn.Add(SkiResortMountain);
--GameEvents.PlayerDoTurn.Add(SwissUA);
GameEvents.UnitSetXY.Add(Mountaineer);
end
]]
--=================================================================================================================
-- Yugoslavia
--=================================================================================================================
--[[
local civYugoslavia = GameInfoTypes["CIVILIZATION_YUGOSLAVIA"];
local yugoslaviaIsActive = JFD_IsCivilisationActive(civYugoslavia);

-- Ideology bonus policies for UA
local yugoAuto = GameInfoTypes["POLICY_DUMMY_YUGO_AUTO"];
local yugoFreedom = GameInfoTypes["POLICY_DUMMY_YUGO_FREEDOM"];
local yugoOrder = GameInfoTypes["POLICY_DUMMY_YUGO_ORDER"];

-- Placeholder policies that do nothing
local policyA = GameInfoTypes["POLICY_DUMMY_YUGO_A"];
local policyB = GameInfoTypes["POLICY_DUMMY_YUGO_B"];

local freedom = GameInfoTypes["POLICY_BRANCH_FREEDOM"];
local order = GameInfoTypes["POLICY_BRANCH_ORDER"];
local auto = GameInfoTypes["POLICY_BRANCH_AUTOCRACY"];

--Updates UA on policy adopt
function YugoslaviaUAAdopt(playerID, policyID)
	YugoslaviaUA(playerID);
end
	
function YugoslaviaUA(playerID)
	local player = Players[playerID];
	if player:GetCivilizationType() == civYugoslavia and player:IsAlive() then	
		if player:GetLateGamePolicyTree() == freedom then
			if player:HasPolicy(yugoFreedom) or player:HasPolicy(policyA) then
				player:SetHasPolicy(yugoFreedom, false);
				player:SetHasPolicy(policyA, false);
				player:SetHasPolicy(policyB, false);
			
				player:SetHasPolicy(yugoOrder, true);
				player:SetHasPolicy(yugoAuto, true);
				
				player:ChangeNumFreeTenets(1, true);
			end
		elseif player:GetLateGamePolicyTree() == order then
			if player:HasPolicy(yugoOrder) or player:HasPolicy(policyA) then
				player:SetHasPolicy(yugoOrder, false);
				player:SetHasPolicy(policyA, false);
				player:SetHasPolicy(policyB, false);
			
				player:SetHasPolicy(yugoFreedom, true);
				player:SetHasPolicy(yugoAuto, true);
			
				player:ChangeNumFreeTenets(1, true);
			end
		elseif player:GetLateGamePolicyTree() == auto then
			if player:HasPolicy(yugoAuto) or player:HasPolicy(policyA) then
				player:SetHasPolicy(yugoAuto, false);
				player:SetHasPolicy(policyA, false);
				player:SetHasPolicy(policyB, false);
			
				player:SetHasPolicy(yugoFreedom, true);
				player:SetHasPolicy(yugoOrder, true);
			
				player:ChangeNumFreeTenets(1, true);
			end
		end
	end 
end

function YugoslaviaInit(playerID)
    for playerID, player in pairs(Players) do
        local player = Players[playerID];
        if player:GetCivilizationType() == civYugoslavia and not player:HasPolicy(policyA) and not player:HasPolicy(yugoAuto) and not player:HasPolicy(yugoFreedom) and not player:HasPolicy(yugoOrder)  then
            player:SetNumFreePolicies(1);
            player:SetNumFreePolicies(0);
            player:SetHasPolicy(policyA, true);
            
            player:SetNumFreePolicies(1);
            player:SetNumFreePolicies(0);
            player:SetHasPolicy(policyB, true);
        end
    end
end

if yugoslaviaIsActive then
GameEvents.PlayerDoTurn.Add(YugoslaviaUA);
GameEvents.PlayerAdoptPolicy.Add(YugoslaviaUAAdopt);
Events.SequenceGameInitComplete.Add(YugoslaviaInit);
end
-- =======================================================================================================================
-- Colombia UA : Add a dummy building to give science from lumbermills on tech, add golden age points on city capture
-- =======================================================================================================================
local iCiv = GameInfoTypes["CIVILIZATION_COLOMBIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if (bIsActive and iCiv) then
	function ColombiaCityCaptureGoldenAgePoints(iOldOwner, bIsCapital, iX, iY, iNewOwner, iPop, bConquest)
		local pPlayer = Players[iNewOwner]
		if (pPlayer:GetCivilizationType() == iCiv) then
			local iGoldenagepointsColombia = 125 * GameInfo.GameSpeeds[Game.GetGameSpeedType()].GoldenAgePercent / 100
			pPlayer:ChangeGoldenAgeProgressMeter(iGoldenagepointsColombia)
		end
	end
    function ColombiaTrait(iPlayer)
        local player = Players[iPlayer];
		local pCity = player:GetCapitalCity();
        if (player:IsEverAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_COLOMBIA"]) then
            if Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_CHEMISTRY"]) then
				if pCity:IsCapital() then
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_COLOMBIA_TRAIT"], 1);
				end
            end
        end
    end
	GameEvents.CityCaptureComplete.Add(ColombiaCityCaptureGoldenAgePoints)
    GameEvents.TeamSetHasTech.Add(ColombiaTrait)
    GameEvents.CityCaptureComplete.Add(ColombiaTrait)
    GameEvents.PlayerCityFounded.Add(ColombiaTrait)
end

-- =======================================================================================================================
-- Phoenicia UA : Add +1 Population after Optics on new cities
-- =======================================================================================================================

local iCiv = GameInfoTypes["CIVILIZATION_PHOENICIAN"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

local iBuilding = GameInfoTypes["BUILDING_PHOENICIAN_TRAIT"]

function PhoeniciaUACityBonus(playerID, iX, iY)
    local pPlayer = Players[playerID]
    if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_PHOENICIAN"] and pPlayer:IsAlive() then
		if Teams[pPlayer:GetTeam()]:IsHasTech(GameInfoTypes["TECH_OPTICS"]) then
			local pPlot = Map.GetPlot(iX, iY)
			local pCity = pPlot:GetPlotCity()
			pCity:SetNumRealBuilding(iBuilding, 1)
		end
    end
end
if bIsActive then
	GameEvents.PlayerCityFounded.Add(PhoeniciaUACityBonus)
end

-- =======================================================================================================================
-- Polynesia UA Bug fix : Remove ocean impassable promotion from upgraded units (galley -> galleas)
-- =======================================================================================================================
local iCiv = GameInfoTypes["CIVILIZATION_POLYNESIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function PolynesiaUABugFix(playerID)
	local pPlayer = Players[playerID]
	if pPlayer:GetCivilizationType() == iCiv then
		print("Polynesia UA Bug Fix")
		for pUnit in pPlayer:Units() do
			if pUnit:IsHasPromotion(GameInfoTypes["PROMOTION_OCEAN_IMPASSABLE"]) then
				pUnit:SetHasPromotion(GameInfoTypes["PROMOTION_OCEAN_IMPASSABLE"], false)
			end
		end
	end
end
if bIsActive then
	GameEvents.PlayerDoTurn.Add(PolynesiaUABugFix)
end
-- =======================================================================================================================
-- Mexico UA, search for 10 tiles around settler and reveal city-states
-- =======================================================================================================================
local civMexico = GameInfoTypes["CIVILIZATION_MEXICO"]
local bIsActive = JFD_IsCivilisationActive(civMexico)
function MexicoDiscovery(playerID)
	
	local player = Players[playerID]
	local iTeam = player:GetTeam()
	
	-- Iterate to find cities from the starting plot
	local startPlot = player:GetStartingPlot()
	for pAdjacentPlot in PlotAreaSweepIterator(startPlot, 10, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_EXCLUDE) do
		local pCity = pAdjacentPlot:GetPlotCity()
		if pCity then
			--get the owner of the city tile
			local cityOwner = Players[pCity:GetOwner()]
			if cityOwner:IsMinorCiv() and (not(pCity:IsRevealed(iTeam))) then
				pAdjacentPlot:SetRevealed(iTeam, true);
			end
		end
	end
end

function MexicoUA(playerID)
	-- get the amount of passed turns in the game
	local turn = Game.GetElapsedGameTurns()
	if turn > 2 then
		return
	end
	print("Mexico UA fired")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == civMexico then
			print("Mexico UA fired 2")
			MexicoDiscovery(playerID);
		end
	end
end

if bIsActive then
	GameEvents.PlayerDoTurn.Add(MexicoUA);
end
--=======================================================================================================================
-- Papal States UA
--=======================================================================================================================
local iCiv = GameInfoTypes["CIVILIZATION_VATICAN"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
local iVaticanReligion;
function VaticanUAonFound(ePlayer, eReligion)
	--get the player
	local player = Players[ePlayer];
	--are we iciv and did we just found the religion?
	if (player:GetCivilizationType() == iCiv) then
		--loop through our cities
		for iCity in player:Cities() do
			if (iCity:IsHolyCityForReligion(eReligion)) then
				--vatican founded this relgion, store it
				iVaticanReligion = eReligion;
			end        
		end
	end
end

function fetchVaticanRelgion(ePlayer, eReligion)
	local player = Players[ePlayer];
	if player:GetCivilizationType() == iCiv then
		for iCity in player:Cities() do
			if (iCity:IsHolyCityForReligion(eReligion)) then
				--vatican founded this relgion, store it
				iVaticanReligion = eReligion;
			end        
		end
	end
	return iVaticanReligion;
end

function VaticanUACourthouse(ePlayer, eReligion)
	local player = Players[ePlayer];
	if (player:GetCivilizationType() == iCiv) then
		print(eReligion);
		fetchVaticanRelgion(ePlayer, eReligion);
		print(iVaticanReligion);
		for pCity in player:Cities() do
			if (pCity:IsOccupied() or pCity:IsPuppet()) then
				if pCity:GetReligiousMajority() == iVaticanReligion then
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_COURTHOUSE"], 1);
				end
			end
		end
	end
end

function VaticanUACourthouseCapture(eOldOwner, bCapital, iX, iY, eNewOwner, iPop, bConquest)
	local player = Players[eNewOwner];
	if (player:GetCivilizationType() == iCiv) then
		local pCity = Map.GetPlot(iX, iY):GetPlotCity();
		if pCity:GetReligiousMajority() == iVaticanReligion then
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_COURTHOUSE"], 1);
		end
	end
end
	
	
if bIsActive then
	GameEvents.CityConvertsReligion.Add(VaticanUACourthouse);
	GameEvents.CityCaptureComplete.Add(VaticanUACourthouseCapture);
	GameEvents.ReligionFounded.Add(VaticanUAonFound);
end
]]