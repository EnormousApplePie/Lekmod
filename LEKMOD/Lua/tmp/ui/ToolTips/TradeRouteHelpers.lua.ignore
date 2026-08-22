-------------------------------------------------
-- TradeRouteHelpers.lua (Vibe coded. not sorry, this was giving me a headache)
-------------------------------------------------

local TRADE_CONNECTION_INTERNATIONAL = GameInfoTypes.TRADE_CONNECTION_INTERNATIONAL or 0
local YIELD_SCIENCE = GameInfoTypes.YIELD_SCIENCE

local influenceLookup = {
		[InfluenceLevelTypes.INFLUENCE_LEVEL_UNKNOWN] = "TXT_KEY_CO_UNKNOWN",
		[InfluenceLevelTypes.INFLUENCE_LEVEL_EXOTIC] = "TXT_KEY_CO_EXOTIC",
		[InfluenceLevelTypes.INFLUENCE_LEVEL_FAMILIAR] = "TXT_KEY_CO_FAMILIAR",
		[InfluenceLevelTypes.INFLUENCE_LEVEL_POPULAR] = "TXT_KEY_CO_POPULAR",
		[InfluenceLevelTypes.INFLUENCE_LEVEL_INFLUENTIAL] = "TXT_KEY_CO_INFLUENTIAL",
		[InfluenceLevelTypes.INFLUENCE_LEVEL_DOMINANT] = "TXT_KEY_CO_DOMINANT",
	};

local function IsMajorCiv(p)
	return (not p:IsMinorCiv()) and (not p:IsBarbarian())
end

local function GetDisplayPlayerName(pPlayer)
	if Game:IsNetworkMultiPlayer() then
		local nick = pPlayer:GetNickName()
		if nick and nick ~= "" then return nick end
	end
	return pPlayer:GetName()
end

local function IsBuildingValid(civInfo ,buildingInfo)
	if buildingInfo and civInfo then
		local classInfo = GameInfo.BuildingClasses[buildingInfo.BuildingClass]
		if classInfo then
			for override in GameInfo.Civilization_BuildingClassOverrides{ CivilizationType = civInfo.Type, BuildingClassType = buildingInfo.BuildingClass} do
				return override.BuildingType == buildingInfo.Type
			end
			return classInfo.DefaultBuilding == buildingInfo.Type
		end
	end
	return false
end


local function BuildSingleSide(pPlayer, pOriginCity, pTargetCity, eConnectionType, eDomain, bAsOrigin)
	local lines = {}
	local connectionInfo = GameInfo.TradeConnections[eConnectionType]
	local domainInfo = GameInfo.Domains[eDomain]
	if not connectionInfo or not domainInfo then return "" end

	local pOriginOwner = Players[pOriginCity:GetOwner()]
	local pDestOwner = Players[pTargetCity:GetOwner()]
	local originCivInfo = GameInfo.Civilizations[pOriginOwner:GetCivilizationType()]
	local destCivInfo = GameInfo.Civilizations[pDestOwner:GetCivilizationType()]
	local bIsTargetingMinor = pDestOwner:IsMinorCiv()

	local thisConnectionDomainYield = { TradeConnectionType = connectionInfo.Type, DomainType = domainInfo.Type }

	local potentialYourBuildings = {}
	local potentialTheirBuildings = {}
	local resourceSources = {}
	local potentialPolicies = {}
	local potentialBeliefs = {}
	local potentialPolicyModifiers = {}

	local originBeliefs = {}
	local destBeliefs = {}
	local originReligion = pOriginCity:GetReligiousMajority()
	local destReligion = pTargetCity:GetReligiousMajority()

	if originReligion and originReligion > ReligionTypes.RELIGION_PANTHEON then
		for _, beliefID in pairs(Game.GetBeliefsInReligion(originReligion) or {}) do
			originBeliefs[beliefID] = true
		end
	end

	if destReligion and destReligion > ReligionTypes.RELIGION_PANTHEON then
		for _, beliefID in pairs(Game.GetBeliefsInReligion(destReligion) or {}) do
			destBeliefs[beliefID] = true
		end
	end

	local baseText = ""
	local originCityText, destCityText = "", ""
	local yourBuildingText, theirBuildingText = "", ""
	local resourceText, exclusiveText = "", ""
	local policyText, traitText, religionText = "", "", ""
	local policyModifierText, traitModifierText = "", ""
	local riverModifierText, domainModifierText = "", ""
	local sumText, totalText = "", ""
	local anyBase, hasModifiers = false, false

	for yield in GameInfo.Yields() do
		local yieldID = yield.ID
		local yieldIcon = yield.IconString or ""
		thisConnectionDomainYield.YieldType = yield.Type

		local baseValue = (pPlayer:GetTradeConnectionBaseYield(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local originCityValue = (pPlayer:GetTradeConnectionOriginGPTValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local destCityValue = (pPlayer:GetTradeConnectionDestinationGPTValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local yourBuildingValue = (pPlayer:GetTradeConnectionYourBuildingValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local theirBuildingValue = (pPlayer:GetTradeConnectionTheirBuildingValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local resourceValue = (pPlayer:GetTradeConnectionResourceValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local exclusiveValue = (pPlayer:GetTradeConnectionExclusiveValue(pOriginCity, pTargetCity, eConnectionType, yieldID, eDomain) or 0) / 100
		local policyValue = (pPlayer:GetTradeConnectionPolicyValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local traitValue = (pPlayer:GetTradeConnectionTraitValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100
		local religionValue = (pPlayer:GetTradeConnectionReligionValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0) / 100

		if baseValue ~= 0 then
			baseText = string.format("%s %+g%s", baseText, baseValue, yieldIcon)
			anyBase = true
		end
		if originCityValue ~= 0 then
			originCityText = string.format("%s %+g%s", originCityText, originCityValue, yieldIcon)
			anyBase = true
		end
		if destCityValue ~= 0 then
			destCityText = string.format("%s %+g%s", destCityText, destCityValue, yieldIcon)
			anyBase = true
		end
		if yourBuildingValue ~= 0 then
			yourBuildingText = string.format("%s %+g%s", yourBuildingText, yourBuildingValue, yieldIcon)
			anyBase = true
		end
		if theirBuildingValue ~= 0 then
			theirBuildingText = string.format("%s %+g%s", theirBuildingText, theirBuildingValue, yieldIcon)
			anyBase = true
		end
		if resourceValue ~= 0 then
			resourceText = string.format("%s %+g%s", resourceText, resourceValue, yieldIcon)
			anyBase = true
		end
		if exclusiveValue ~= 0 then
			exclusiveText = string.format("%s %+g%s", exclusiveText, exclusiveValue, yieldIcon)
			anyBase = true
		end
		if policyValue ~= 0 then
			policyText = string.format("%s %+g%s", policyText, policyValue, yieldIcon)
			anyBase = true
		end
		if traitValue ~= 0 then
			traitText = string.format("%s %+g%s", traitText, traitValue, yieldIcon)
			anyBase = true
		end
		if religionValue ~= 0 then
			religionText = string.format("%s %+g%s", religionText, religionValue, yieldIcon)
			anyBase = true
		end

		local sumValue = (pPlayer:GetTradeConnectionTotalValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain, false) or 0) / 100

		if sumValue ~= 0 then
			sumText = string.format("%s %+g%s", sumText, sumValue, yieldIcon)
		end

		if bAsOrigin then
			for row in GameInfo.Building_TradeConnectionOriginYieldChanges(thisConnectionDomainYield) do
				local buildingInfo = GameInfo.Buildings[row.BuildingType]

				if IsBuildingValid(originCivInfo, buildingInfo) and not pOriginCity:IsHasBuilding(buildingInfo.ID) then
					local value = (row.YieldTimes100 or 0) / 100

					if value ~= 0 then
						local entry = potentialYourBuildings[buildingInfo.ID]

						if not entry then
							entry = { BuildingInfo = buildingInfo, Text = "" }
							potentialYourBuildings[buildingInfo.ID] = entry
						end

						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		end

		if bAsOrigin and not bIsTargetingMinor then
			for row in GameInfo.Building_IncomingTradeConnectionYieldChanges(thisConnectionDomainYield) do
				local buildingInfo = GameInfo.Buildings[row.BuildingType]

				if IsBuildingValid(destCivInfo, buildingInfo) and not pTargetCity:IsHasBuilding(buildingInfo.ID) then
					local value = (row.YieldTimes100 or 0) / 100

					if value ~= 0 then
						local entry = potentialTheirBuildings[buildingInfo.ID]

						if not entry then
							entry = { BuildingInfo = buildingInfo, Text = "" }
							potentialTheirBuildings[buildingInfo.ID] = entry
						end

						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		elseif not bAsOrigin and not bIsTargetingMinor then
			for row in GameInfo.Building_TradeConnectionDestinationYieldChanges(thisConnectionDomainYield) do
				local buildingInfo = GameInfo.Buildings[row.BuildingType]

				if IsBuildingValid(destCivInfo, buildingInfo) and not pTargetCity:IsHasBuilding(buildingInfo.ID) then
					local value = (row.YieldTimes100 or 0) / 100

					if value ~= 0 then
						local entry = potentialTheirBuildings[buildingInfo.ID]

						if not entry then
							entry = { BuildingInfo = buildingInfo, Text = "" }
							potentialTheirBuildings[buildingInfo.ID] = entry
						end

						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		end

		if bAsOrigin then
			for row in GameInfo.Resource_TradeConnectionYieldBonus(thisConnectionDomainYield) do
				local resourceInfo = GameInfo.Resources[row.ResourceType]

				if resourceInfo then
					local originHas = pOriginCity:IsHasResourceLocal(resourceInfo.ID, false)
					local destinationHas = pTargetCity:IsHasResourceLocal(resourceInfo.ID, false)

					if originHas ~= destinationHas then
						local value = (row.YieldTimes100 or 0) / 100

						if value ~= 0 then
							local entry = resourceSources[resourceInfo.ID]

							if not entry then
								entry = { ResourceInfo = resourceInfo, Text = "" }
								resourceSources[resourceInfo.ID] = entry
							end

							entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
						end
					end
				end
			end
		end

		if bAsOrigin then
			for row in GameInfo.Policy_TradeRouteYieldChanges(thisConnectionDomainYield) do
				local policyInfo = GameInfo.Policies[row.PolicyType]
				local cityStateOnly = (tonumber(row.CityStateOnly) or 0) ~= 0
				local validTarget = not cityStateOnly or bIsTargetingMinor

				if policyInfo and validTarget and not pPlayer:HasPolicy(policyInfo.ID) then
					local value = (row.YieldTimes100 or 0) / 100

					if value ~= 0 then
						local entry = potentialPolicies[policyInfo.ID]

						if not entry then
							entry = { PolicyInfo = policyInfo, Text = "" }
							potentialPolicies[policyInfo.ID] = entry
						end

						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		end

		if bAsOrigin then
			for row in GameInfo.Belief_TradeConnectionOriginYieldChanges(thisConnectionDomainYield) do
				local beliefInfo = GameInfo.Beliefs[row.BeliefType]
			
				if beliefInfo and not originBeliefs[beliefInfo.ID] then
					local value = (row.YieldTimes100 or 0) / 100
				
					if value ~= 0 then
						local entry = potentialBeliefs[beliefInfo.ID]
					
						if not entry then
							entry = { BeliefInfo = beliefInfo, Text = "" }
							potentialBeliefs[beliefInfo.ID] = entry
						end
					
						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		else
			for row in GameInfo.Belief_IncomingTradeConnectionYieldChanges(thisConnectionDomainYield) do
				local beliefInfo = GameInfo.Beliefs[row.BeliefType]

				if beliefInfo and not destBeliefs[beliefInfo.ID] then
					local value = (row.YieldTimes100 or 0) / 100

					if value ~= 0 then
						local entry = potentialBeliefs[beliefInfo.ID]

						if not entry then
							entry = { BeliefInfo = beliefInfo, Text = "" }
							potentialBeliefs[beliefInfo.ID] = entry
						end

						entry.Text = string.format("%s %+g%s", entry.Text, value, yieldIcon)
					end
				end
			end
		end

		if sumValue ~= 0 then
			local policyModifier = pPlayer:GetTradeConnectionPolicyValueModifier(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0
			local traitModifier = pPlayer:GetTradeConnectionTraitValueModifier(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0
			local riverModifier = pPlayer:GetTradeConnectionRiverValueModifier(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain) or 0
			local domainModifier = pPlayer:GetTradeConnectionDomainValueModifier(pOriginCity, pTargetCity, eConnectionType, yieldID, eDomain) or 0

			if policyModifier ~= 0 then
				local bonus = sumValue * policyModifier / 100
				policyModifierText = string.format("%s %+d%%%s (%+g%s)", policyModifierText, policyModifier, yieldIcon, bonus, yieldIcon)
				hasModifiers = true
			end
			if traitModifier ~= 0 then
				local bonus = sumValue * traitModifier / 100
				traitModifierText = string.format("%s %+d%%%s (%+g%s)", traitModifierText, traitModifier, yieldIcon, bonus, yieldIcon)
				hasModifiers = true
			end
			if riverModifier ~= 0 then
				local bonus = sumValue * riverModifier / 100
				riverModifierText = string.format("%s %+d%%%s (%+g%s)", riverModifierText, riverModifier, yieldIcon, bonus, yieldIcon)
				hasModifiers = true
			end
			if domainModifier ~= 0 then
				local bonus = sumValue * domainModifier / 100
				domainModifierText = string.format("%s %+d%%%s (%+g%s)", domainModifierText, domainModifier, yieldIcon, bonus, yieldIcon)
				hasModifiers = true
			end

			for row in GameInfo.Policy_TradeRouteYieldModifiers(thisConnectionDomainYield) do
				local policyInfo = GameInfo.Policies[row.PolicyType]

				if policyInfo and not pPlayer:HasPolicy(policyInfo.ID) then
					local modifier = tonumber(row.YieldModifier) or 0

					if modifier ~= 0 then
						local bonus = sumValue * modifier / 100
						local entry = potentialPolicyModifiers[policyInfo.ID]

						if not entry then
							entry = { PolicyInfo = policyInfo, Text = "" }
							potentialPolicyModifiers[policyInfo.ID] = entry
						end

						entry.Text = string.format("%s %+d%%%s (%+g%s)", entry.Text, modifier, yieldIcon, bonus, yieldIcon)
					end
				end
			end
		end

		local totalValue = (pPlayer:GetTradeConnectionTotalValue(pOriginCity, pTargetCity, eConnectionType, bAsOrigin, yieldID, eDomain, true) or 0) / 100

		if totalValue ~= 0 then
			totalText = string.format("%s %+g%s", totalText, totalValue, yieldIcon)
		end
	end -- end yield loop

	local hasDetails =
		next(potentialYourBuildings) or next(potentialTheirBuildings) or
		next(resourceSources) or next(potentialPolicies) or next(potentialBeliefs)

	-- Some special science bonuses
	local technologyText = ""
	if eConnectionType == TRADE_CONNECTION_INTERNATIONAL
		and IsMajorCiv(pOriginOwner)
		and IsMajorCiv(pDestOwner) then

		local sciIcon = GameInfo.Yields[YIELD_SCIENCE].IconString or ""

		if bAsOrigin then
			local destPlayerID = pDestOwner:GetID()
			local diffForOrigin = pOriginOwner:GetNumTechDifference(destPlayerID) or 0

			if diffForOrigin > 0 then
				local sciGain = diffForOrigin * 0.5
				technologyText = string.format( "[ICON_BULLET]+%.1f%s (%s knows %d techs you don't)", sciGain, sciIcon, GetDisplayPlayerName(pDestOwner), diffForOrigin)
			end

			local influenceScience = pOriginOwner:GetInfluenceTradeRouteScienceBonus(destPlayerID) or 0
			if influenceScience > 0 then
				local influenceLevel = pOriginOwner:GetInfluenceLevel(destPlayerID)
				local influenceText = Locale.ConvertTextKey(influenceLookup[influenceLevel] or "TXT_KEY_CO_UNKNOWN")
				local text = string.format( "[ICON_BULLET]+%g%s (You are Culturally %s on %s)", influenceScience, sciIcon, influenceText, GetDisplayPlayerName(pDestOwner))

				technologyText = technologyText ~= "" and technologyText .. "[NEWLINE]" .. text or text
			end
		else
			local diffForDest = pDestOwner:GetNumTechDifference(pOriginOwner:GetID()) or 0

			if diffForDest > 0 then
				local sciGain = diffForDest * 0.5
				technologyText = string.format( "Technologies: +%.1f%s (You know %d techs they don't)", sciGain, sciIcon, diffForDest)
			end
		end
	end

	if anyBase or hasDetails then
		lines[#lines + 1] = "[BASE]"

		if baseText ~= "" then lines[#lines + 1] = "Base:" .. baseText end
		if technologyText ~= "" then lines[#lines + 1] = technologyText end
		if originCityText ~= "" or destCityText ~= "" then
			lines[#lines + 1] = "Cities:"

			if originCityText ~= "" then
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_POSITIVE_TEXT]" .. pOriginCity:GetName() .. "[ENDCOLOR]:" .. originCityText
			end

			if destCityText ~= "" then
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_POSITIVE_TEXT]" .. pTargetCity:GetName() .. "[ENDCOLOR]:" .. destCityText
			end
		end

		if yourBuildingText ~= "" or next(potentialYourBuildings) then
			local cityName = bAsOrigin and pOriginCity:GetName() or pTargetCity:GetName()
			lines[#lines + 1] = cityName .. "'s Buildings:" .. yourBuildingText

			for _, entry in pairs(potentialYourBuildings) do
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_YELLOW]" .. Locale.Lookup(entry.BuildingInfo.Description) .. "[ENDCOLOR]:" .. entry.Text
			end
		end

		if theirBuildingText ~= "" or next(potentialTheirBuildings) then
			lines[#lines + 1] = pTargetCity:GetName() .. "'s Buildings:" .. theirBuildingText

			for _, entry in pairs(potentialTheirBuildings) do
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_YELLOW]" .. Locale.Lookup(entry.BuildingInfo.Description) .. "[ENDCOLOR]:" .. entry.Text
			end
		end

		if resourceText ~= "" or next(resourceSources) then
			lines[#lines + 1] = "Resources:" .. resourceText

			for _, entry in pairs(resourceSources) do
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_POSITIVE_TEXT]" .. (entry.ResourceInfo.IconString or "") .. Locale.Lookup(entry.ResourceInfo.Description) .. "[ENDCOLOR]:" .. entry.Text
			end
		end

		if exclusiveText ~= "" then lines[#lines + 1] = "Exclusive:" .. exclusiveText end

		if policyText ~= "" or next(potentialPolicies) then
			lines[#lines + 1] = "Policies:" .. policyText

			for _, entry in pairs(potentialPolicies) do
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_YELLOW]" .. Locale.Lookup(entry.PolicyInfo.Description) .. "[ENDCOLOR]:" .. entry.Text
			end
		end

		if traitText ~= "" then lines[#lines + 1] = "Traits:" .. traitText end

		if religionText ~= "" or next(potentialBeliefs) then
			lines[#lines + 1] = "Religion:" .. religionText

			for _, entry in pairs(potentialBeliefs) do
				local description = entry.BeliefInfo.ShortDescription or entry.BeliefInfo.Description
				lines[#lines + 1] = "  [ICON_BULLET][COLOR_YELLOW]" .. Locale.Lookup(description) .. "[ENDCOLOR]:" .. entry.Text
			end
		end
	end

	if hasModifiers or next(potentialPolicyModifiers) then
		if sumText ~= baseText then
			lines[#lines + 1] = "Sum:" .. (sumText ~= "" and sumText or " 0")
		end
		lines[#lines + 1] = "[MODIFIERS]"

		if policyModifierText ~= "" then lines[#lines + 1] = " [ICON_BULLET][COLOR_POSITIVE_TEXT]Policies[ENDCOLOR]:" .. policyModifierText end
		if traitModifierText ~= "" then lines[#lines + 1] = " [ICON_BULLET][COLOR_POSITIVE_TEXT]Traits[ENDCOLOR]:" .. traitModifierText end
		if riverModifierText ~= "" then lines[#lines + 1] = " [ICON_BULLET][COLOR_POSITIVE_TEXT]River[ENDCOLOR]:" .. riverModifierText end
		if domainModifierText ~= "" then lines[#lines + 1] = " [ICON_BULLET][COLOR_POSITIVE_TEXT]Domain[ENDCOLOR]:" .. domainModifierText end

		for _, entry in pairs(potentialPolicyModifiers) do
			lines[#lines + 1] = "  [ICON_BULLET][COLOR_YELLOW]" .. Locale.Lookup(entry.PolicyInfo.Description) .. "[ENDCOLOR]:" .. entry.Text
		end
	end

	if totalText ~= "" or sumText ~= "" then
		lines[#lines + 1] = "[TOTAL]"
		lines[#lines + 1] = "Total:" .. (totalText ~= "" and totalText or sumText)
	end

	return table.concat(lines, "[NEWLINE]")
end

function BuildTradeRouteToolTipString(pPlayer, pOriginCity, pTargetCity, eConnectionType, eDomain)
	if not pOriginCity or not pTargetCity then return "" end

	local pOriginOwner = Players[pOriginCity:GetOwner()]
	local pDestOwner   = Players[pTargetCity:GetOwner()]

	local header1 = string.format("=== %s (%s) ===", pOriginCity:GetName(), Locale.Lookup(GameInfo.Civilizations[pOriginOwner:GetCivilizationType()].Description) )
	local header2 = string.format("=== %s (%s) ===", pTargetCity:GetName(), Locale.Lookup(GameInfo.Civilizations[pDestOwner:GetCivilizationType()].Description) )

	local originBlock, originSum = BuildSingleSide(pOriginOwner, pOriginCity, pTargetCity, eConnectionType, eDomain, true)
	local destBlock, destSum = BuildSingleSide(pDestOwner, pOriginCity, pTargetCity, eConnectionType, eDomain, false)

	local out = header1 .. "[NEWLINE]" .. originBlock .. "[NEWLINE][NEWLINE]" .. header2 .. "[NEWLINE]" .. destBlock
	return out
end
