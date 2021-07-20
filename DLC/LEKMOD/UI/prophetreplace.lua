
-- Author: EnormousApplePie & Lek10

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

include("PlotIterators")
include("FLuaVector.lua")

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
--=================================================================================================================
--=================================================================================================================
--									UA					UB				UU			
--=================================================================================================================
--=================================================================================================================

-- Nabatea UU
local iCiv = GameInfoTypes["CIVILIZATION_MC_NABATEA"]
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
				if pCity:IsCapital() then iRewardExp = 5 end

				pPlayer:ChangeGold(iReward)
				pUnit:ChangeExperience(iRewardExp)

				if (pPlayer:IsHuman() and pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NABATEA"] and Game.GetActivePlayer() == iPlayer and iReward > 0 ) then
					Events.GameplayAlertMessage(Locale.ConvertTextKey("[COLOR_POSITIVE_TEXT]+{1_Num} [ICON_GOLD] Gold[ENDCOLOR] from discovering {2_City} with a Zabonah", iReward, pCity:GetName()))
				end				
			end
		end
	end
end
-- Nabatea UB

function JFD_TombProduction(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NABATEA"] and player:IsAlive() then
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
	if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NABATEA"]) then
		if Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_CIVIL_SERVICE"]) then
			Teams[player:GetTeam()]:SetHasTech(GameInfoTypes["TECH_CIVIL_DUMMY"], false);
			print("removed dummy")
		end
	end
	if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NABATEA"] and Teams[player:GetTeam()]:IsHasTech(GameInfoTypes["TECH_MATHEMATICS"])) then
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

local iCiv = GameInfoTypes["CIVILIZATION_MC_LITE_MOOR"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function MoorsEraUA(playerID)

	for playerID, player in pairs(Players) do
			local player = Players[playerID];
			local pCity = player:GetCapitalCity();
			if pCity ~= nil then
				if (player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_LITE_MOOR"] and player:GetCurrentEra() == GameInfoTypes["ERA_RENAISSANCE"]) then
					
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


if bIsActive then
GameEvents.PlayerDoTurn.Add(MoorsEraUA)
end
--- UAE UA


local iCiv = GameInfoTypes["CIVILIZATION_UAE"]
local bIsActive = JFD_IsCivilisationActive(iCiv)


-- From Sukritacts Nabatea's Civilization


--Sukritacts Nabatea's Civilization: Gold from Trade Routes

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


local tClasses = {}
tClasses[GameInfoTypes.UNITCLASS_CARGO_SHIP] = GameInfoTypes.UNITCLASS_CARGO_SHIP

function DetectPlunder(iPlayer, iUnit)
    local pPlayer = Players[iPlayer]
    local pUnit = pPlayer:GetUnitByID(iUnit)
    local iUnitClass = pUnit:GetUnitClassType()
    
	if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UAE"] then
		if tClasses[iUnitClass] ~= nil then
			local pTeam = Teams[pPlayer:GetTeam()]
			local pPlot = pUnit:GetPlot()
			local iNumUnits = pPlot:GetNumUnits()
			for iVal = 0,(iNumUnits - 1) do
				local pLUnit = pPlot:GetUnit(iVal)
				if pLUnit:GetCombatLimit() > 0 and pTeam:IsAtWar(pLUnit:GetTeam()) then
					-- Being plundered, run function
					QasimiPillageGold(Players[pLUnit:GetOwner()])
					break
				end
			end
		end
	end
function QasimiPillageGold(player)
	local goldPillaged = 200
	player:ChangeGold(goldPillaged)
end
    
    return false
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
GameEvents.CanSaveUnit.Add(DetectPlunder)
end

--- Oman UA

-- JFD_DomesticTRGold


local iCiv = GameInfoTypes["CIVILIZATION_MC_OMAN"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

--[[
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
	if player:IsAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_OMAN"] then
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
--]]
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
	if player:IsAlive() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_OMAN"] then
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
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_OMAN"] and player:IsAlive() then
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

--- Cuba ua

local iCiv = GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

-- UB: Dance Hall

function DanceHallDo(iPlayer)
    local pPlayer = Players[iPlayer]
    if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"] then
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
    if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"] then
        local capital = player:GetCapitalCity()
        if (capital ~= nil) then
            local iNumToSet = 0
            for i = 0, GameDefines.MAX_MAJOR_CIVS - 1, 1 do
                if (iPlayer ~= i) then
                    local pOtPlayer = Players[i]
                    if (pOtPlayer ~= nil) and pOtPlayer:IsEverAlive() and pOtPlayer:IsAlive() and (pOtPlayer:GetCapitalCity() ~= nil) and Teams[player:GetTeam()]:IsHasMet(pOtPlayer:GetTeam()) then
                        print("has met a civ, begin stealing their culture...")
                        local otherCapital = pOtPlayer:GetCapitalCity()
                        iNumToSet = (iNumToSet +  math.floor(otherCapital:GetBaseJONSCulturePerTurn() * .125) )
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
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"] then
		local policyLevel = GameInfo.Policies[policyTypeID].Level
		if policyLevel == 1 then
			DummyPolicyCuba(player)
			capital:SetNumRealBuilding(GameInfoTypes.BUILDING_CUBA_TRAIT_UB3, 1)
		end
	end
end


if bIsActive then
	GameEvents.PlayerAdoptPolicy.Add(UUnlock);
	 GameEvents.PlayerDoTurn.Add(DanceHallDo)
	 GameEvents.PlayerDoTurn.Add(CubaCultureYoink)
end


-- bolivia UA. Couldn't bother with the button UI, too much of a time sink ~EAP
local iCiv = GameInfoTypes["CIVILIZATION_LEU_BOLIVIA_BELZU"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function IsPersonExpended(iPlayer, iUnit)
	print("working: Bolivia")
	
	local pPlayer = Players[iPlayer];
	local pCity = pPlayer:GetCapitalCity();
    if (pPlayer:IsEverAlive()) then
		print("found a civ")
        if pPlayer:GetCivilizationType() == GameInfoTypes["CIVILIZATION_LEU_BOLIVIA_BELZU"] then
			print("found Bolivia")
			if Teams[pPlayer:GetTeam()]:IsHasTech(GameInfoTypes["TECH_CHEMISTRY"]) then
				local ArtUnitID = GameInfoTypes["UNIT_ARTIST"]
				if (iUnit == ArtUnitID) then
					print("found the Unit -> Artist")
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 1);
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 0);
					if pPlayer:IsHuman() and pPlayer:GetCivilizationType() == iCiv then
					Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_RIGHT"))
					end
				end
				local iWriter = GameInfoTypes["UNIT_WRITER"]
				if (iUnit == iWriter) then
					print("found the Unit -> Writer")
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_FOOD"], 1);
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BOLIVIA_TRAIT_PRODUCTION"], 0);
					if (pPlayer:IsHuman() and pPlayer:GetCivilizationType() == iCiv and Game.GetActivePlayer() == iPlayer) then
					Events.GameplayAlertMessage(Locale.ConvertTextKey("TXT_KEY_THP_BOLIVIA_BUTTON_TITLE_LEFT"))
					end
				end
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

local iCiv = GameInfoTypes["CIVILIZATION_MC_ROMANIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

function RomaniaGACulture(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_ROMANIA"] and player:IsAlive() then
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

local iCiv = GameInfoTypes["CIVILIZATION_MC_MAORI"]
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
	if (not player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_MAORI"]) then return end
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

local unitEmbarkPromotionActiveID = GameInfoTypes["PROMOTION_MOVE_ALL_TERRAIN"]
local unitEmbarkPromotionID = GameInfoTypes["PROMOTION_EMBARKATION"]

function EAP_Embark_Fix(playerID)
	local player = Players[playerID]
	local playerTeam = Teams[player:GetTeam()]
	if (not player:IsAlive()) then return end
	if player:IsBarbarian() then return end
	for unit in player:Units() do
		if unit:IsHasPromotion(unitEmbarkPromotionActiveID) then
			unit:SetHasPromotion(unitEmbarkPromotionID, false)
		end
	end
end
GameEvents.PlayerDoTurn.Add(EAP_Embark_Fix)


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

local iCiv = GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
     
function JFD_Tonga(playerMetID, playerID)
			local player = Players[playerID]
            local playerMet = Players[playerMetID]
            local majorsMet = Teams[playerMet:GetTeam()]:GetHasMetCivCount(true)
            local rewardCulture = 8
            local rewardScience = 12
            local rewardGold = 40
            local rewardFaith = 14
            if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] then
                    local random = GetRandom(1, 4)
                    if random == 1 then
                            player:ChangeFaith(rewardFaith)
							if player:IsHuman() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] and Game.GetActivePlayer() == playerID then
								Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_PEACE] Faith[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardFaith, playerMet:GetName()))
							end
                    elseif random == 2 then
                            player:ChangeJONSCulture(rewardCulture)
							if player:IsHuman() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] and Game.GetActivePlayer() == playerID then
								Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_CULTURE] Culture[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardCulture, playerMet:GetName()))
							end
                    elseif random == 3 then
                            Teams[player:GetTeam()]:GetTeamTechs():ChangeResearchProgress(player:GetCurrentResearch(), rewardScience, playerID)
							if player:IsHuman() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] and Game.GetActivePlayer() == playerID then
								Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_RESEARCH] Science[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardScience, playerMet:GetName()))
							end
                    else
                            player:ChangeGold(rewardGold)
							if player:IsHuman() and player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] and Game.GetActivePlayer() == playerID then
								 Events.GameplayAlertMessage(Locale.ConvertTextKey("You receive [COLOR_POSITIVE_TEXT]{1_Num} [ICON_GOLD] Gold[ENDCOLOR] from meeting [COLOR_POSITIVE_TEXT]{2_CivName}[ENDCOLOR]", rewardGold, playerMet:GetName()))
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
	GameEvents.TeamMeet.Add(JFD_Tonga)
	GameEvents.PlayerDoTurn.Add(MC_MaoriBattalion)
	GameEvents.PlayerDoTurn.Add(JFD_NewZealand_Defender_PlayerDoTurn)
end

	-- Tongo UB food
	-- Code by JFD
	-- Hoped to edit it somewhat and make it slightly more original, but I couldn't quite change it without bringing down the balance ~EAP
	local iCiv = GameInfoTypes["CIVILIZATION_MC_TONGA"]
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

-- Aksum UA function 
function GetTradeRoutesNumberAksum(player, city)
	print("working: aksum 1")
	local tradeRoutes = player:GetTradeRoutes()
	local numRoutesAksum = 0
	for i, v in ipairs(tradeRoutes) do
		local originatingCity = v.FromCity
		if (originatingCity == city) then
			numRoutesAksum = numRoutesAksum + 1
		end
	end
	
	return numRoutesAksum
end


local buildingTraitAksumID = GameInfoTypes["BUILDING_AKSUM_TRAIT"]
function AksumTrait(playerID)
	print("working: aksum 2")
	local player = Players[playerID]
    if player:IsEverAlive() and player:GetCivilizationType() == civilissationID then 
		for city in player:Cities() do
			city:SetNumRealBuilding(buildingTraitAksumID, math.min(GetTradeRoutesNumberAksum(player, city), 1)) -- it does!
		end
	end
end

if bIsActive then
	GameEvents.PlayerDoTurn.Add(AksumTrait)
end

-- Kilwa UA 
-- Ah, Optimization! Thanks JFD!
local iCiv = GameInfoTypes["CIVILIZATION_MC_KILWA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if JFD_IsCivilisationActive(iCiv) then
	print("Kilwa is in this game")
end

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
    if player:IsEverAlive() and player:GetCivilizationType() == civilisationID then 
		for city in player:Cities() do
			city:SetNumRealBuilding(buildingTraitKilwaID, math.min(GetTradeRoutesNumber(player, city), 420)) -- I wonder if this will work (note: it does)
		end
	end
end

if bIsActive then
	GameEvents.PlayerDoTurn.Add(KilwaTrait)
end

-- Ukraine UA 

local iCiv = GameInfoTypes["CIVILIZATION_UKRAINE"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
if bIsActive then
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: ukraine ontechbonus")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UKRAINE"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_THE_WHEEL"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_UKRAINE_TRAIT"], 1);
				end
			end
		end
	end
end);
end
-- Turks Unit Promotion 

include("PlotIterators")
local civID = GameInfoTypes["CIVILIZATION_UC_TURKEY"]
local promoID = GameInfoTypes["PROMOTION_UNIT_TURKISH_GWI"]
function TurksGeneralreset(playerID)
	local player = Players[playerID]
	if player:GetCivilizationType() == civID then
		local greatGenerals = {}
		for unit in player:Units() do
			if unit:IsHasPromotion(promoID) then
				unit:SetHasPromotion(promoID, false)
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
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsCombatUnit() then
						otherUnit:SetHasPromotion(promoID, true)
					end
				end
			end
		end
	end
end
GameEvents.PlayerDoTurn.Add(TurksGeneralreset)

function TurksGeneralbonus(playerID, unitID, unitX, unitY)
	local player = Players[playerID]
	if player:GetCivilizationType() == civID and (player:GetUnitClassCount(GameInfoTypes["UNITCLASS_GREAT_GENERAL"]) > 0) then
		local unit = player:GetUnitByID(unitID)
		local plot = unit:GetPlot()
		if unit:IsHasPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"]) then
			for loopPlot in PlotAreaSweepIterator(plot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
				for i = 0, loopPlot:GetNumUnits() - 1, 1 do  
					local otherUnit = loopPlot:GetUnit(i)
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsCombatUnit() and unit:IsHasPromotion(GameInfoTypes.PROMOTION_FREE_UPGRADE_TURKISH) then
						otherUnit:SetHasPromotion(promoID, true)
					end
				end
			end
		elseif unit:IsCombatUnit() and unit:IsHasPromotion(GameInfoTypes.PROMOTION_FREE_UPGRADE_TURKISH) then
			unit:SetHasPromotion(promoID, false)
			for loopPlot in PlotAreaSweepIterator(plot, 2, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
				for i = 0, loopPlot:GetNumUnits() - 1, 1 do  
					local otherUnit = loopPlot:GetUnit(i)
					if otherUnit and otherUnit:GetOwner() == playerID and otherUnit:IsHasPromotion(GameInfoTypes["PROMOTION_GREAT_GENERAL"]) then
						unit:SetHasPromotion(promoID, true)
					end
				end
			end
		end
	end
end
GameEvents.UnitSetXY.Add(TurksGeneralbonus)

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


local iCiv = GameInfoTypes["CIVILIZATION_VENEZ"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

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
-- Horde free building 

local iCiv = GameInfoTypes["CIVILIZATION_HORDE"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

if bIsActive then
print("loaded horde ua")
GameEvents.TeamSetHasTech.Add(function(iTeam, iTech, bAdopted)
	print("working: horde UA")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_HORDE"] then
			if player:GetTeam() == iTeam then
				if (iTech == GameInfoTypes["TECH_PHILOSOPHY"]) then
					local pCity = player:GetCapitalCity();
					pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_JARLIQ"], 1);
				end
			end
		end
	end
end)
end

----------------------------------------------------------
-------------------- DUMMY POLICIES ----------------------
----------------------------------------------------------
-- 'Nam dummy policy

--local iCiv = GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"]
--local bIsActive = JFD_IsCivilisationActive(iCiv)

print("dummy policy loaded - Cuba")
function DummyPolicyCuba(player)
	print("working - Cuba")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UC_CUBA_BATISTA"] then
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

-- 'Nam dummy policy

local iCiv = GameInfoTypes["CIVILIZATION_VIET"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

print("dummy policy loaded - Vietnam")
function DummyPolicy(player)
	print("working - Vietnam")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_VIET"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_VIETNAM"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_VIETNAM"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Mexico dummy policy

local iCiv = GameInfoTypes["CIVILIZATION_LEXICO"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

print("dummy policy loaded - Mexico")
function DummyPolicy(player)
	print("working - Mexico")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_LEXICO"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_LEXICO"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_LEXICO"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Maori dummy policy

local iCiv = GameInfoTypes["CIVILIZATION_MC_MAORI"]
local bIsActive = JFD_IsCivilisationActive(iCiv)

print("dummy policy loaded - Maori")
function DummyPolicy(player)
	print("working - Maori")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_MAORI"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_MAORI"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_MAORI"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end

-- New zealand dummy policy
local iCiv = GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - NZ")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_NEW_ZEALAND"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_NZ"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_NZ"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Kilwa Dummy policy
local iCiv = GameInfoTypes["CIVILIZATION_MC_KILWA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Kilwa")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_KILWA"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_KILWA"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_KILWA"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Turkey dummy Policy
local iCiv = GameInfoTypes["CIVILIZATION_UC_TURKEY"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Turkey")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_UC_TURKEY"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_TURKEY"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_TURKEY"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Ireland dummy Policy
local iCiv = GameInfoTypes["CIVILIZATION_IRELAND"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Ireland")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_IRELAND"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_IRELAND"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_IRELAND"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Scotland dummy Policy
local iCiv = GameInfoTypes["CIVILIZATION_MC_SCOTLAND"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Wales")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_SCOTLAND"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_SCOTLAND"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_SCOTLAND"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Romania dummy Policy
local iCiv = GameInfoTypes["CIVILIZATION_MC_ROMANIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Romania")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_MC_ROMANIA"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_ROMANIA"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_ROMANIA"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Goth dummy Policy
local iCiv = GameInfoTypes["CIVILIZATION_GOTH"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Goth")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_GOTH"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_GOTH"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_GOTH"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- German dummy policy
local iCiv = GameInfoTypes["CIVILIZATION_GERMANY"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Germany")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_GERMANY"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_GERMANY"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_GERMANY"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Korea dummy policy

local iCiv = GameInfoTypes["CIVILIZATION_KOREA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Korea")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_KOREA"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_KOREA"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_KOREA"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Akkad dummy policy
local iCiv = GameInfoTypes["CIVILIZATION_LITE_AKKAD"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Akkad")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_LITE_AKKAD"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_AKKAD"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_AKKAD"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end
-- Prussia dummy policy
local iCiv = GameInfoTypes["CIVILIZATION_PRUSSIA"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function DummyPolicy(player)
	print("working - Prussia")
	for playerID, player in pairs(Players) do
		local player = Players[playerID];
		if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_PRUSSIA"] then
			if not player:HasPolicy(GameInfoTypes["POLICY_DUMMY_PRUSSIA"]) then
				
				player:SetNumFreePolicies(1)
				player:SetNumFreePolicies(0)
				player:SetHasPolicy(GameInfoTypes["POLICY_DUMMY_PRUSSIA"], true)	
			end
		end
	end 
end
if bIsActive then
Events.SequenceGameInitComplete.Add(DummyPolicy)
end

-- Akkad_Laputtu
-- Author: Tomatekh
-- DateCreated: 6/22/2015 6:11:46 PM
--------------------------------------------------------------

local Laputtu = GameInfoTypes.UNIT_LITE_AKKAD_LAPUTTU;
local Pyramids = GameInfoTypes.BUILDINGCLASS_PYRAMID;

function AkkadOverseer(iPlayer)
	local pPlayer = Players[iPlayer];
	local pTeam = pPlayer:GetTeam();
	if (pPlayer:IsAlive()) then
		for pUnit in pPlayer:Units() do
			if (pUnit:GetUnitType() == Laputtu) then
				if not pUnit:IsEmbarked() then 
					local uPlot = pUnit:GetPlot();
					local WorkBonus = 100;
					if pPlayer:GetBuildingClassCount(Pyramids) >= 1 then
						WorkBonus = 125;
					end
					for pBuildInfo in GameInfo.Builds() do
						local BuildTurns = uPlot:GetBuildTurnsLeft(pBuildInfo.ID, 0, 0);
						local BuildProgress = uPlot:GetBuildProgress(pBuildInfo.ID)
						if (BuildTurns >= 1) and (BuildProgress > WorkBonus) then
							uPlot:ChangeBuildProgress(pBuildInfo.ID, WorkBonus, pTeam)
						end
					end
				end
			end
		end
	end
end
GameEvents.PlayerDoTurn.Add(AkkadOverseer)


-- AKKAD UA --

include("PlotIterators")
--_________________________________________________________________________________________________________________________________________________________________________________________________________
local iCiv = GameInfoTypes["CIVILIZATION_LITE_AKKAD"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
--_________________________________________________________________________________________________________________________________________________________________________________________________________
local civilizationID = GameInfoTypes["CIVILIZATION_LITE_AKKAD"]
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


--_________________________________________________________________________________________________________________________________________________________________________________________________________
--GREAT GENERAL POINTS FROM IMPROVEMENTS
--_________________________________________________________________________________________________________________________________________________________________________________________________________
function liteGreatGeneralPointsfromImproving(playerID, plotX, plotY, improvementID) 
	local player = Players[playerID]
	if improvementID then
		if player:GetCivilizationType() == civilizationID then
			local plot = Map.GetPlot(plotX, plotY)
			local city = plot:GetWorkingCity()
			if city and city:IsOccupied() then
				player:ChangeCombatExperience(greatGeneralPoints)
			end
		end
	end
end

if bIsActive then
GameEvents.PlayerDoTurn.Add(liteGreatGeneralBonusReset)
GameEvents.UnitSetXY.Add(liteGreatGeneralBonusAgainstCities)
GameEvents.BuildFinished.Add(liteGreatGeneralPointsfromImproving)
end
--_________________________________________________________________________________________________________________________________________________________________________________________________________






-- ProphetReplacer
-- Author: LastSword
-- DateCreated: 8/24/2013 2:56:18 PM
--------------------------------------------------------------
local sUnitType = "UNIT_PROPHET"
local iProphetID = GameInfo.Units.UNIT_PROPHET.ID
local iProphetOverride = GameInfo.Units.UNIT_DALAILAMA.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_TIBET"].ID

function TibetOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
      	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
               if (pUnit:GetUnitType() == iProphetID) then
                   local newUnit = pPlayer:InitUnit(iProphetOverride, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(TibetOverride)


-- krivis
-- Author: lek10
-- DateCreated: 4/25/2018 8:23:15 PM
--------------------------------------------------------------

local sUnitType = "UNIT_PROPHET"
local iProphetID = GameInfo.Units.UNIT_PROPHET.ID
local iProphetOverride = GameInfo.Units.UNIT_KRIVIS.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_LITHUANIA"].ID

function KriviOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == iProphetID) then
                    local newUnit = pPlayer:InitUnit(iProphetOverride, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(KriviOverride)


-- mpiambina
-- Author: lek10
-- DateCreated: 11/21/2018 5:29:36 PM
--------------------------------------------------------------
local sUnitType = "UNIT_INQUISITOR"
local iProphetID = GameInfo.Units.UNIT_INQUISITOR.ID
local iProphetOverride = GameInfo.Units.UNIT_MPIAMBINA.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_MALAGASY"].ID

function MadaOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == iProphetID) then
                    local newUnit = pPlayer:InitUnit(iProphetOverride, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(MadaOverride)



--[[--////--------------------------------------------------
				--- NABATEA WORKER ---
----////--------------------------------------------------

local OldUnitType = "UNIT_WORKER"
local oldUnitID = GameInfo.Units.UNIT_WORKER.ID
local newUnitID = GameInfo.Units.UNIT_NABATEA_WORKER.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_MC_NABATEA"].ID

function NabaTeaWorkerOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(NabaTeaWorkerOverride)


-- Now reverse!! I typed this backwards so you will get a better explantion below in similar functions but right now I couldnt be bothered copy-pasting so I decided instead, to write this novel. ~EAP

local OldUnitType = "UNIT_NABATEA_WORKER"
local oldUnitID = GameInfo.Units.UNIT_NABATEA_WORKER.ID
local newUnitID = GameInfo.Units.UNIT_WORKER.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_MC_NABATEA"].ID

function NabaTeaWorkerROverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (not pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(NabaTeaWorkerROverride)

--]]
----////--------------------------------------------------
				--- ARGENTINA WORKER ---
----////--------------------------------------------------

local OldUnitType = "UNIT_WORKER"
local oldUnitID = GameInfo.Units.UNIT_WORKER.ID
local newUnitID = GameInfo.Units.UNIT_ARGENTINA_WORKER.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_ARGENTINA"].ID

function ArgentinaWorkerOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ArgentinaWorkerOverride)


-- Now reverse!! I typed this backwards so you will get a better explantion below in similar functions but right now I couldnt be bothered copy-pasting so I decided instead, to write this novel. ~EAP

local OldUnitType = "UNIT_ARGENTINA_WORKER"
local oldUnitID = GameInfo.Units.UNIT_ARGENTINA_WORKER.ID
local newUnitID = GameInfo.Units.UNIT_WORKER.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_ARGENTINA"].ID

function ArgentinaWorkerROverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (not pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ArgentinaWorkerROverride)


----////--------------------------------------------------
				--- CHILE WORKBOAT ---
----////--------------------------------------------------

local OldUnitType = "UNIT_WORKBOAT"
local oldUnitID = GameInfo.Units.UNIT_WORKBOAT.ID
local newUnitID = GameInfo.Units.UNIT_CHILE_WORKBOAT.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_CHILE"].ID

function ChileWorkBoatOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ChileWorkBoatOverride)

-- Now reverse it so other civ cant get it by various means

local OldUnitType = "UNIT_CHILE_WORKBOAT"
local oldUnitID = GameInfo.Units.UNIT_CHILE_WORKBOAT.ID
local newUnitID = GameInfo.Units.UNIT_WORKBOAT.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_CHILE"].ID

function ChileWorkBoatROverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (not pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ChileWorkBoatROverride)



----////--------------------------------------------------
				--- CHILE ADMIRAL ---
----////--------------------------------------------------

local OldUnitType = "UNIT_GREAT_ADMIRAL"
local oldUnitID = GameInfo.Units.UNIT_GREAT_ADMIRAL.ID
local newUnitID = GameInfo.Units.UNIT_CHILE_GREAT_ADMIRAL.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_CHILE"].ID

function ChileAdmiralOverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ChileAdmiralOverride)

--- Now reverse it so other civs wont get it by various means
local OldUnitType = "UNIT_CHILE_GREAT_ADMIRAL"
local oldUnitID = GameInfo.Units.UNIT_CHILE_GREAT_ADMIRAL.ID
local newUnitID = GameInfo.Units.UNIT_GREAT_ADMIRAL.ID
local iCivType = GameInfo.Civilizations["CIVILIZATION_CHILE"].ID

function ChileAdmiralROverride(iPlayer, iUnit)
    local pPlayer = Players[iPlayer];
    if (pPlayer:IsEverAlive()) then
        if (not pPlayer:GetCivilizationType() == iCivType) then
       	    if pPlayer:GetUnitByID(iUnit) ~= nil then
		pUnit = pPlayer:GetUnitByID(iUnit);
                if (pUnit:GetUnitType() == oldUnitID) then
                    local newUnit = pPlayer:InitUnit(newUnitID, pUnit:GetX(), pUnit:GetY())
                    newUnit:Convert(pUnit);
                end
            end
        end
    end
end

Events.SerialEventUnitCreated.Add(ChileAdmiralROverride)

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



---------------------
-- On Policy Adopted stuff (uncludes UA's , easier for me to copy pasta stuff <3)
---------------

-- Notes, do realize that this is coded keeping policy requirements/paths in mind, so if you change any of the policy paths in the future, if anyone besides me is doing policies stuff, DONT FORGET TO CHECK THIS! thnx! ~EAP

-- Italy ua

local iCiv = GameInfoTypes["CIVILIZATION_ITALY"]
local bIsActive = JFD_IsCivilisationActive(iCiv)
function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Piety finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_THEOCRACY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
			(policyID == GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
			(policyID == GameInfo.Policies["POLICY_FREE_RELIGION"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
			(policyID == GameInfo.Policies["POLICY_REFORMATION"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_PIETY"], 1);
		end
	end 
end

if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end
function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Tradition Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_ARISTOCRACY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_LANDED_ELITE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MONARCHY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_OLIGARCHY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_LANDED_ELITE"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_ARISTOCRACY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MONARCHY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_OLIGARCHY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_MONARCHY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_LANDED_ELITE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_ARISTOCRACY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_OLIGARCHY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_OLIGARCHY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_LANDED_ELITE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MONARCHY"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_ARISTOCRACY"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_TRADITION"], 1);
		end
	end 
end

if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end
function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Liberty Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_COLLECTIVE_RULE"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_REPRESENTATION"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MERITOCRACY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_REPRESENTATION"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_COLLECTIVE_RULE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MERITOCRACY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_MERITOCRACY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_COLLECTIVE_RULE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_REPRESENTATION"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_LIBERTY"], 1);
		end
	end 
end
if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end

function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Honor Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_MILITARY_CASTE"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_DISCIPLINE"].ID)) or
			(policyID == GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_CASTE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_DISCIPLINE"].ID)) or
			(policyID == GameInfo.Policies["POLICY_DISCIPLINE"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_CASTE"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_MILITARY_TRADITION"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_HONOR"], 1);
		end
	end 
end
if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end

function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Patronage Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_CONSULATES"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_SCHOLASTICISM"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_PHILANTHROPY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_SCHOLASTICISM"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_CONSULATES"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_PHILANTHROPY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_PHILANTHROPY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_CONSULATES"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_SCHOLASTICISM"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_PATRONAGE"], 1);
		end
	end 
end
if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end

function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Aesthetics Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_FINE_ARTS"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_FLOURISHING_OF_ARTS"].ID)) or
			(policyID == GameInfo.Policies["POLICY_FLOURISHING_OF_ARTS"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_FINE_ARTS"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_AESTHETICS"], 1);
		end
	end 
end

if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end
function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Exploration Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_TREASURE_FLEETS"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_MERCHANT_NAVY"].ID)) or
			(policyID == GameInfo.Policies["POLICY_MERCHANT_NAVY"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_TREASURE_FLEETS"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_EXPLORATION"], 1);
		end
	end 
end
if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end

function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Rationalism Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_SECULARISM"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_HUMANISM"].ID)) or
			(policyID == GameInfo.Policies["POLICY_HUMANISM"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_SECULARISM"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_RATIONALISM"], 1);
		end
	end 
end
if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end

function Italy_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Commerce Finished
	if player:GetCivilizationType() == GameInfoTypes["CIVILIZATION_ITALY"] then

		if	(policyID == GameInfo.Policies["POLICY_ENTREPRENEURSHIP"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_PROTECTIONISM"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_TRADE_UNIONS"].ID)) or
			(policyID == GameInfo.Policies["POLICY_PROTECTIONISM"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_ENTREPRENEURSHIP"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_TRADE_UNIONS"].ID)) or
			(policyID == GameInfo.Policies["POLICY_TRADE_UNIONS"].ID 
			and player:HasPolicy(GameInfo.Policies["POLICY_ENTREPRENEURSHIP"].ID)
			and player:HasPolicy(GameInfo.Policies["POLICY_PROTECTIONISM"].ID)) then

			-- Finished Policy Tree, now add the building
			local pCity = player:GetCapitalCity();
			pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_ITALY_TRAIT_COMMERCE"], 1);
		end
	end 
end

if bIsActive then
GameEvents.PlayerAdoptPolicy.Add(Italy_OnPolicyAdopted);
end



-- PietyChanges
-- Author: Cirra
-- DateCreated: 10/17/2019 1:22:18 AM
--------------------------------------------------------------

function Piety_OnPolicyAdopted(playerID, policyID)

	local player = Players[playerID]

	-- Piety finisher

	if	(policyID == GameInfo.Policies["POLICY_THEOCRACY"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
		(policyID == GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
		(policyID == GameInfo.Policies["POLICY_FREE_RELIGION"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_REFORMATION"].ID)) or
		(policyID == GameInfo.Policies["POLICY_REFORMATION"].ID 
		and player:HasPolicy(GameInfo.Policies["POLICY_MANDATE_OF_HEAVEN"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_FREE_RELIGION"].ID)
		and player:HasPolicy(GameInfo.Policies["POLICY_THEOCRACY"].ID)) then

		-- The player has finished Piety. Add a Grand Monument to the capital, gives allows buying great people.
		local pCity = player:GetCapitalCity();
		pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_PIETY_FINISHER"], 1);

		--[[
		local i = 0
		local iIndo = GameInfo.Civilizations["CIVILIZATION_INDONESIA"].ID
		local iKhmer = GameInfo.Civilizations["CIVILIZATION_KHMER"].ID
		local iRoma = GameInfo.Civilizations["CIVILIZATION_MC_ROMANIA"].ID
		for pCity in player:Cities() do
			if i >= 4 then break end
			if (player:GetCivilizationType() == iIndo) then pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_CANDI"], 1) 
			elseif (player:GetCivilizationType() == iKhmer) then pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_BARAY"], 1)
			elseif (player:GetCivilizationType() == iRoma) then pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_MC_ROMANIAN_PAINTED_MONASTERY"], 1)
			else pCity:SetNumRealBuilding(GameInfoTypes["BUILDING_GARDEN"], 1) end
			i = i + 1
		end
		--]]
	end
end
GameEvents.PlayerAdoptPolicy.Add(Piety_OnPolicyAdopted);

-- HonorChanges
-- Author: Cirra
-- DateCreated: 7/27/2019 1:22:18 AM
--------------------------------------------------------------

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
GameEvents.PlayerAdoptPolicy.Add(Honor_OnPolicyAdopted);
