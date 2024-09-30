-- Nabatea Addin
-- Author: JFD, Sukritact
--=======================================================================================================================
Events.SequenceGameInitComplete.Add(function()
include("IconSupport")
include("PlotIterators")
print("loaded")

--=======================================================================================================================
-- Defines
--=======================================================================================================================
local iCiv			= GameInfoTypes.CIVILIZATION_NABATEA
local iBuilding		= GameInfoTypes.BUILDING_TRAIT_MC_NABATEA
local iDesert 		= GameInfoTypes.TERRAIN_DESERT

local iMod = ((GameInfo.GameSpeeds[Game.GetGameSpeedType()].BuildPercent)/100)
local tCost = {
		50,
		100,
		150,
	}

local tFlavor = {
		'BUY',
		'BUY',
		'BUY',
		'BUY',
		'BUY',
	}	
--==========================================================================================================================
-- GAMEPLAY FUNCTIONS
--==========================================================================================================================
function GetImprovementCost(pCity)
	local iNum = pCity:GetNumBuilding(iBuilding)
	local iCost = tCost[iNum+1] -- arrays in lua start at 1
	if not(iCost) then
		-- when you have 3 buildings, the next building should cost 200, then increment by 100 onwards
		-- therefore 3 should return 200; 4 should return 300; 5 should return 400, etc.
		-- the formula for that is (n-1) * 100
		iCost = (iNum - 1) * 100 
	end

	local iMod2 = 1
	local iPlayer = pCity:GetOwner()
	if Players[iPlayer]:HasPolicy(GameInfoTypes.POLICY_DECISIONS_NABATEANGARDENS) and IsDesertCity(pCity) then
		iMod2 = 2/3
	end

	iCost = math.ceil(iMod * iCost * iMod2)
	return iCost
end
------------------------------------
-- GetPleasureGarden
------------------------------------
function GetPleasureGarden(pCity)
	local iNum = pCity:GetNumBuilding(iBuilding)
	local iReward = tCost[iNum+1] -- arrays in lua start at 1
	if not(iReward) then
		iReward = (iNum - 1) * 100 
	end

	iReward = math.ceil(iMod * iReward/6)
	return iReward
end
------------------------------------
-- IsDesertCity
------------------------------------
function IsDesertCity(pCity)
	pPlot = pCity:Plot()

	for pAdjacentPlot in PlotAreaSweepIterator(pPlot, 1, SECTOR_NORTH, DIRECTION_CLOCKWISE, DIRECTION_OUTWARDS, CENTRE_INCLUDE) do
		if ((pAdjacentPlot:GetTerrainType() == iDesert) and not(pAdjacentPlot:IsMountain())) then
			return true
		end
	end

	return false
end
------------------------------------
-- AI Support
------------------------------------
function AI_Support(iPlayer)
	
	local pPlayer = Players[iPlayer]
	if not(pPlayer:GetCivilizationType() == iCiv) then return end
	if pPlayer:IsHuman() then return end

	for pCity in pPlayer:Cities() do
		local iNum = pCity:GetNumBuilding(iBuilding)
		local iCost = GetImprovementCost(pCity)

		if pPlayer:GetGold() < iCost then break end
		pPlayer:ChangeGold(-iCost)
		pCity:SetNumRealBuilding(iBuilding, iNum + 1)	
	end

end

GameEvents.PlayerDoTurn.Add(AI_Support)
--==========================================================================================================================
-- UI FUNCTIONS
--==========================================================================================================================
-- UI
----------------------------------------------------------------------------------------------------------------------------
-- UpdateNabateaBoxAndResize
function UpdateNabateaBoxAndResize()

	-- Hide Where Applicable
	local pCity = UI.GetHeadSelectedCity()
	if not pCity then
		Controls.NabateaBox:SetHide(true)
		return
	end
	local iPlayer = pCity:GetOwner()
	if iPlayer ~= Game.GetActivePlayer() then
		Controls.NabateaBox:SetHide(true)
		return
	end		
	local pPlayer = Players[iPlayer]
	if not(pPlayer:GetCivilizationType() == iCiv) then
		Controls.NabateaBox:SetHide(true)
		return
	end
	
	local tCultureBox = ContextPtr:LookUpControl("/InGame/CityView/RightStack")
	Controls.NabateaBox:ChangeParent(tCultureBox)
	Controls.NabateaBox:SetHide(false)


	local iPanelX = tCultureBox:GetSizeX()
	Controls.NabateaBox:SetSizeX(iPanelX)
	Controls.NabateaLine:SetSizeX(iPanelX)
	tCultureBox:SortChildren(function(tA, tB)
		if (tA:GetID() == "NabateaBox") then return true
		elseif tB:GetID() == "NabateaBox" then return false
		end
	end)

	IconHookup(4, 64, "MC_NABATEA_ATLAS", Controls.NabateaIcon)

	UpdateNabateaBox()
end
-- UpdateNabateaBox
function UpdateNabateaBox()

	-- Hide Where Applicable
	local pCity = UI.GetHeadSelectedCity()
	if not pCity then
		Controls.NabateaBox:SetHide(true)
		return
	end
	local iPlayer = pCity:GetOwner()
	if iPlayer ~= Game.GetActivePlayer() then
		Controls.NabateaBox:SetHide(true)
		return
	end		
	local pPlayer = Players[iPlayer]
	if not(pPlayer:GetCivilizationType() == iCiv) then
		Controls.NabateaBox:SetHide(true)
		return
	end

	local iNum = pCity:GetNumBuilding(iBuilding)
	local iCost = GetImprovementCost(pCity)
	if Players[iPlayer]:HasPolicy(GameInfoTypes.POLICY_DECISIONS_NABATEANGARDENS) then
		Controls.BuyButton:LocalizeAndSetToolTip("TXT_KEY_TRAIT_MC_NABATEA_TOOLTIP2", iCost, iNum * 2, GetPleasureGarden(pCity))
	else
		Controls.BuyButton:LocalizeAndSetToolTip("BUY", iCost, iNum * 2)		
	end
	Controls.BuyButton:SetDisabled(false)

	local sCost = iCost .. " [ICON_GOLD]"
	if pPlayer:GetGold() < iCost then
		sCost = '[COLOR_NEGATIVE_TEXT]' .. sCost
		Controls.BuyButton:SetDisabled(true)
	end
	Controls.FlavorText:LocalizeAndSetText(tFlavor[iNum % 5 + 1])
	Controls.CostText:LocalizeAndSetText(sCost)

	Controls.NabateaBox:SetHide(false)
end

if Players[Game.GetActivePlayer()]:GetCivilizationType() == iCiv then
	Events.SerialEventEnterCityScreen.Add(UpdateNabateaBoxAndResize)
	Events.SerialEventCityScreenDirty.Add(UpdateNabateaBox)
	Events.SerialEventCityInfoDirty.Add(UpdateNabateaBox)
	Events.SerialEventGameDataDirty.Add(UpdateNabateaBox)
end
------------------------------------
-- On Button Click
------------------------------------
function OnClick()
	local iPlayer = Game.GetActivePlayer()
	local pPlayer = Players[iPlayer]
	local pCity = UI.GetHeadSelectedCity()

	local iNum = pCity:GetNumBuilding(iBuilding)
	local iCost = GetImprovementCost(pCity)

	if pPlayer:GetGold() < iCost then return end
	pPlayer:ChangeGold(-iCost)

	if pPlayer:HasPolicy(GameInfoTypes.POLICY_DECISIONS_NABATEANGARDENS) then
		local iReward = GetPleasureGarden(pCity)
		pPlayer:ChangeJONSCulture(iReward)
	end

	pCity:SetNumRealBuilding(iBuilding, iNum + 1)
	Events.AudioPlay2DSound("AS2D_SOUND_TRAIT_MC_NABATEA")
end

Controls.BuyButton:RegisterCallback(Mouse.eLClick, OnClick)

end)
--==========================================================================================================================
--==========================================================================================================================