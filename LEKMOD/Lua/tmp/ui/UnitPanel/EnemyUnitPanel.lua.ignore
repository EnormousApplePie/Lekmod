-------------------------------------------------
-- Enemy Unit Panel Screen 
-------------------------------------------------
include( "IconSupport" );
include( "InstanceManager" );

local g_MyCombatDataIM = InstanceManager:new( "UsCombatInfo", "Root", Controls.MyCombatResultsStack );
local g_TheirCombatDataIM = InstanceManager:new( "ThemCombatInfo", "Root", Controls.TheirCombatResultsStack );
local g_TheirNoteIM = InstanceManager:new( "ThemCombatNote", "Root", Controls.TheirCombatResultsStack );

local g_NumButtons = 12;
local g_lastUnitID = -1;		-- Used to determine if a different pUnit has been selected.
local g_ModifierListLength;
if OptionsManager.GetSmallUIAssets_Cached() then
	g_ModifierListLength = 6;
else
	g_ModifierListLength = 10;
end

local defaultErrorTextureSheet = "TechAtlasSmall.dds";
local nullOffset = Vector2( 0, 0 );

local g_iPortraitSize = Controls.UnitPortrait:GetSize().x;

local g_bWorldMouseOver = true;
local g_bShowPanel = false;
local g_lastHexX = nil;		-- Last hex the mouse was over, so we can refresh the combat odds when the interface mode changes.
local g_lastHexY = nil;


function SetName(name)
	
	name = Locale.ToUpper(name);

    local iNameLength = Locale.Length(name);
    if (iNameLength < 18) then
	    Controls.UnitName:SetText(name);
	    
	    Controls.UnitName:SetHide(false);
	    Controls.LongUnitName:SetHide(true);
	    Controls.ReallyLongUnitName:SetHide(true);
	    
    elseif (iNameLength < 23) then
	    Controls.LongUnitName:SetText(name);
	    
	    Controls.UnitName:SetHide(true);
	    Controls.LongUnitName:SetHide(false);
	    Controls.ReallyLongUnitName:SetHide(true);
	    
    else
	    Controls.ReallyLongUnitName:SetText(name);
	    
	    Controls.UnitName:SetHide(true);
	    Controls.LongUnitName:SetHide(true);
	    Controls.ReallyLongUnitName:SetHide(false);
    end
end
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
function UpdateCityPortrait( pCity )

	if pCity == nil then
		return;
	end

	SetName(pCity:GetName());
	local playerID = pCity:GetOwner();
	local pPlayer = Players[playerID];
    local thisCivType = PreGame.GetCivilization( playerID );
    local thisCiv = GameInfo.Civilizations[thisCivType];

	local textureOffset, textureAtlas = IconLookup( thisCiv.PortraitIndex, 32, thisCiv.AlphaIconAtlas );
	Controls.UnitIcon:SetTexture( textureAtlas );
	Controls.UnitIconShadow:SetTexture( textureAtlas );
	Controls.UnitIcon:SetTextureOffset( textureOffset );
	Controls.UnitIconShadow:SetTextureOffset( textureOffset );

    local iconColor, flagColor = pPlayer:GetPlayerColors();
	if pPlayer:IsMinorCiv() then
		flagColor, iconColor = iconColor, flagColor;
	end
	Controls.UnitBackColor:SetColor( flagColor );
	Controls.UnitIcon:SetColor( iconColor );

	IconHookup( 0, g_iPortraitSize, "ENEMY_CITY_ATLAS", Controls.UnitPortrait );				
end


--------------------------------------------------------------------------------
-- Refresh Unit portrait and name
--------------------------------------------------------------------------------
function UpdateUnitPortrait( pUnit )

	if pUnit == nil then
		return;
	end

	local name = pUnit:GetName();
	SetName(name);
		
	local flagOffset, flagAtlas = UI.GetUnitFlagIcon(pUnit);

	local textureOffset, textureSheet = IconLookup( flagOffset, 32, flagAtlas );				
	Controls.UnitIcon:SetTexture( textureSheet );
	Controls.UnitIconShadow:SetTexture( textureSheet );
	Controls.UnitIcon:SetTextureOffset( textureOffset );
	Controls.UnitIconShadow:SetTextureOffset( textureOffset );

	local pPlayer = Players[pUnit:GetOwner()];
    local iconColor, flagColor = pPlayer:GetPlayerColors();
	if pPlayer:IsMinorCiv() then
		flagColor, iconColor = iconColor, flagColor;
	end
	Controls.UnitBackColor:SetColor( flagColor );
	Controls.UnitIcon:SetColor( iconColor );

	local portraitOffset, portraitAtlas = UI.GetUnitPortraitIcon(pUnit);
	textureOffset, textureSheet = IconLookup( portraitOffset, g_iPortraitSize, portraitAtlas );				
	if textureOffset == nil then
		textureSheet = defaultErrorTextureSheet;
		textureOffset = nullOffset;
	end				
	Controls.UnitPortrait:SetTexture(textureSheet);
	Controls.UnitPortrait:SetTextureOffset(textureOffset);
end


--------------------------------------------------------------------------------
-- Refresh Unit promotions
--------------------------------------------------------------------------------
function UpdateUnitPromotions(pUnit)
	local UnitPromotionKey = "UnitPromotion";
	
	--Clear Unit Promotions
	local i = 1;
	while(Controls[UnitPromotionKey..i] ~= nil) do
		local promotionIcon = Controls[UnitPromotionKey..i];
		promotionIcon:SetHide(true);
	
		i = i + 1;
	end
	
	if pUnit then
		--For each avail promotion, display the icon
		for unitPromotion in GameInfo.UnitPromotions() do
			local unitPromotionID = unitPromotion.ID;
			if(pUnit:IsHasPromotion(unitPromotionID)) then
				
				-- Get next available promotion button
				local idx = 1;
				local promotionIcon;
				repeat
					promotionIcon = Controls[UnitPromotionKey..idx];
					idx = idx + 1;
					
				until(promotionIcon == nil or promotionIcon:IsHidden() == true);
				
				if promotionIcon ~= nil then
					IconHookup( unitPromotion.PortraitIndex, 32, unitPromotion.IconAtlas, promotionIcon );				
					promotionIcon:SetHide(false);
				end
			end
		end
	end
end


--------------------------------------------------------------------------------
-- Refresh City stats
--------------------------------------------------------------------------------
function UpdateCityStats(pCity)
	
	-- Strength
	-- Correct Attack City Strength
	local strength = math.floor(pCity:GetStrengthValue(false) / 100);
	
	strength = strength .. " [ICON_STRENGTH]";
	Controls.UnitStrengthBox:SetHide(false);
	Controls.UnitStatStrength:SetText(strength);
	
	Controls.UnitMovementBox:SetHide(true);
	Controls.UnitRangedAttackBox:SetHide(true);	
	
end


--------------------------------------------------------------------------------
-- Refresh Unit stats
--------------------------------------------------------------------------------
function UpdateUnitStats(pUnit)
	
	-- Movement
	local move_denominator = GameDefines["MOVE_DENOMINATOR"];
	local max_moves = pUnit:MaxMoves() / move_denominator;
	local szMoveStr = max_moves .. " [ICON_MOVES]";
	
	Controls.UnitStatMovement:SetText(szMoveStr);
	Controls.UnitMovementBox:SetHide(false);
	
	-- Strength
    local strength = 0;
    if(pUnit:GetDomainType() == DomainTypes.DOMAIN_AIR) then
        strength = pUnit:GetBaseRangedCombatStrength();
    else
        strength = pUnit:GetBaseCombatStrength();
    end
	if(strength > 0) then
		strength = strength .. " [ICON_STRENGTH]";
		Controls.UnitStrengthBox:SetHide(false);
		Controls.UnitStatStrength:SetText(strength);
	else
		Controls.UnitStrengthBox:SetHide(true);
	end		
	
	-- Ranged Strength
	local iRangedStrength = 0;
	if(pUnit:GetDomainType() ~= DomainTypes.DOMAIN_AIR) then
		iRangedStrength = pUnit:GetBaseRangedCombatStrength();
	else
		iRangedStrength = 0;
	end
	if(iRangedStrength > 0) then
		local szRangedStrength = iRangedStrength .. " [ICON_RANGE_STRENGTH]";
		Controls.UnitRangedAttackBox:SetHide(false);
		Controls.UnitStatRangedAttack:SetText(szRangedStrength);
	else
		Controls.UnitRangedAttackBox:SetHide(true);
	end		
	
end


--------------------------------------------------------------------------------
-- Format our text please!
--------------------------------------------------------------------------------
function SetCombatModifierRow(controlTable, modifier, bForMe)
	controlTable.Text:SetText("");
	controlTable.Value:SetText("");

	controlTable.Text:SetText(modifier.Text or "");

	local strValue;

	if (modifier.IsPercent) then
		strValue = Locale.ToNumber(modifier.Value, "#.##") .. "%";

		if (bForMe) then
			if (modifier.Value > 0) then
				strValue = "[COLOR_POSITIVE_TEXT]+" .. strValue .. "[ENDCOLOR]";
			elseif (modifier.Value < 0) then
				strValue = "[COLOR_NEGATIVE_TEXT]" .. strValue .. "[ENDCOLOR]";
			end
		else
			if (modifier.Value > 0) then
				strValue = "[COLOR_NEGATIVE_TEXT]+" .. strValue .. "[ENDCOLOR]";
			elseif (modifier.Value < 0) then
				strValue = "[COLOR_POSITIVE_TEXT]" .. strValue .. "[ENDCOLOR]";
			end
		end
	else
		strValue = Locale.ToNumber(modifier.Value / 100, "#.##");
	end

	if (bForMe) then
		controlTable.Value:SetText(" : " .. strValue);
	else
		controlTable.Value:SetText(strValue .. " :");
	end
end

function WillShowAirInterceptWarning(bBombing, interceptor)
	return bBombing and interceptor == nil;
end

function AddAirInterceptWarning(bBombing, interceptor)
	if (not WillShowAirInterceptWarning(bBombing, interceptor)) then
		return;
	end
	g_TheirNoteIM:GetInstance().Text:SetText(Locale.ConvertTextKey("TXT_KEY_EUPANEL_AIR_INTERCEPT_WARNING1"));
end

function GetMeleeAttackFromPlot(attacker, battlePlot)
	local pPathFromPlot = attacker:GetPathAttackFromPlot(battlePlot);

	return pPathFromPlot;
end

local g_CombatIndicatorColors = { "Green", "Red", "Yellow" };

function SetCombatIndicator(color, textKey, ...)
	for _, otherColor in ipairs(g_CombatIndicatorColors) do
		Controls[otherColor .. "Indicator"]:SetHide(otherColor ~= color);
	end
	Controls[color .. "IndicatorLabel"]:SetText(Locale.ToUpper(Locale.ConvertTextKey(textKey, ...)));
end

function UpdateCombatOddsUnitVsCity(pMyUnit, pCity)

	g_MyCombatDataIM:ResetInstances();
	g_TheirCombatDataIM:ResetInstances();
	g_TheirNoteIM:ResetInstances();

	if (pMyUnit == nil or pCity == nil) then
		return false;
	end

	local attacker = pMyUnit;
	local defender = pCity;
	local battlePlot = defender:Plot();
	local interceptor = nil;

	if (battlePlot == nil) then
		return false;
	end

	local bRanged = false;
	local bBombing = false;
	local bSweep = false;

	-- Interface mode can indicate the intended attack type.
	local interfaceMode = UI.GetInterfaceMode();

	-- A sweep targets an interceptor, not the city.
	if (interfaceMode == InterfaceModeTypes.INTERFACEMODE_AIR_SWEEP) then
		return false;
	elseif (interfaceMode == InterfaceModeTypes.INTERFACEMODE_AIRSTRIKE) then
		bBombing = true;
	elseif (interfaceMode == InterfaceModeTypes.INTERFACEMODE_RANGE_ATTACK) then
		bRanged = true;
	elseif (attacker:GetDomainType() == DomainTypes.DOMAIN_AIR) then
		bBombing = true;
	elseif (not attacker:IsRangedSupportFire() and attacker:GetBaseRangedCombatStrength() > 0) then
		if (attacker:IsOnlyDefensive() or (attacker:IsMustSetUpToRangedAttack() and attacker:IsSetUpForRangedAttack())) then
			bRanged = true;
		end
	end

	local iNumVisibleAAUnits = 0;

	if (bBombing and attacker:EvasionProbability() < 100) then
		iNumVisibleAAUnits = attacker:GetInterceptorCount( battlePlot, nil, false, true, true);
		if (iNumVisibleAAUnits > 0) then
			-- There is no defender unit to exclude when bombing a city.
			interceptor = attacker:GetBestInterceptor( battlePlot, nil, false, true, true);
		end
	end
	
	local fromPlot = nil;
	if (not bRanged and not bBombing and not bSweep) then
		fromPlot = GetMeleeAttackFromPlot(attacker, battlePlot);
	end

	-- Arguments:
	-- plot, defender unit, defender city, interceptor, flags
	local attackerStrength = attacker:GetMaxAttackStrength( battlePlot, nil, defender, interceptor, bRanged, bBombing, bSweep, fromPlot);

	if (attackerStrength <= 0) then
		return false;
	end

	local attackerModList = attacker:GetAttackModifierList( battlePlot, nil, defender, interceptor, bRanged, bBombing, bSweep, g_ModifierListLength, fromPlot);
	-- Bool arguement is bRangedStrike
	local defenderStrength = defender:GetStrengthValue(false);
	local defenderModList = defender:GetCityCombatModifierList(false);


	local CombatDamage = Game.GetCombatDamage(attacker, nil, nil, defender, interceptor, battlePlot, bRanged, bBombing, bSweep);

	if (CombatDamage == nil or CombatDamage.Attacker == nil or CombatDamage.Defender == nil) then
		return false;
	end

	local attackerResults = CombatDamage.Attacker;
	local defenderResults = CombatDamage.Defender;
	local interceptorResults = CombatDamage.Interceptor;
	local attackerDamageInflicted = math.max( 0, defenderResults.FinalDamage - defenderResults.CurrentDamage);

	-- Includes city retaliation and interception.
	local defenderDamageInflicted = math.max( 0, attackerResults.FinalDamage - attackerResults.CurrentDamage);
	local iInterceptionDamage = 0;

	if (interceptorResults ~= nil) then
		iInterceptionDamage = math.max( 0, interceptorResults.DamageInflicted);
	end

	DoUpdateHealthBars(attackerResults.MaxHitPoints, defenderResults.MaxHitPoints, attackerResults.CurrentDamage, defenderResults.CurrentDamage, attackerDamageInflicted, defenderDamageInflicted, 0, iInterceptionDamage);

	Controls.MyStrengthValue:SetText(Locale.ToNumber(attackerStrength / 100, "#.##"));
	Controls.TheirStrengthValue:SetText(Locale.ToNumber(defenderStrength / 100, "#.##"));
	Controls.MyDamageValue:SetText("[COLOR_GREEN]" .. attackerDamageInflicted .. "[ENDCOLOR]");
	Controls.TheirDamageValue:SetText("[COLOR_RED]" .. defenderDamageInflicted .. "[ENDCOLOR]");

	for _, modifier in ipairs(attackerModList) do
		local controlTable = g_MyCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, true);
	end

	AddAirInterceptWarning(bBombing, interceptor);
	for _, modifier in ipairs(defenderModList) do
		local controlTable = g_TheirCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, false);
	end

	if (bRanged or bBombing) then
		SetCombatIndicator("Green", bBombing and "TXT_KEY_INTERFACEMODE_AIRSTRIKE" or "TXT_KEY_INTERFACEMODE_RANGE_ATTACK");
	-- A melee strike that reduces the city to 0 HP always captures it (a 1 HP city can never survive melee)
	elseif (defenderResults.FinalDamage >= defenderResults.MaxHitPoints) then
		SetCombatIndicator("Green", "TXT_KEY_EUPANEL_GUARANTEED_CAPTURE");
	elseif (attackerResults.CurrentDamage >(attackerResults.MaxHitPoints / 2) and defenderDamageInflicted > 0) then
		SetCombatIndicator("Red", "TXT_KEY_EUPANEL_RISKY_ATTACK");
	elseif (defenderDamageInflicted >= attackerDamageInflicted) then
		SetCombatIndicator("Red", "TXT_KEY_EUPANEL_RISKY_ATTACK");
	else
		SetCombatIndicator("Green", "TXT_KEY_EUPANEL_SAFE_ATTACK");
	end

	Controls.MyCombatResultsStack:CalculateSize();
	Controls.TheirCombatResultsStack:CalculateSize();

	local sizeX = Controls.DetailsGrid:GetSizeX();
	Controls.DetailsGrid:DoAutoSize();
	Controls.DetailsGrid:SetSizeX(sizeX);
	Controls.DetailsSeperator:SetSizeY(Controls.DetailsGrid:GetSizeY());
	Controls.DetailsGrid:ReprocessAnchoring();

	return true;
end

function UpdateCombatOddsUnitVsUnit(pMyUnit, pTheirUnit, pBattlePlot)

	g_MyCombatDataIM:ResetInstances();
	g_TheirCombatDataIM:ResetInstances();
	g_TheirNoteIM:ResetInstances();
	local bSweepPreview = (pTheirUnit == nil and pBattlePlot ~= nil);

	if (pMyUnit ~= nil and (pTheirUnit ~= nil or bSweepPreview)) then
		local attacker = pMyUnit;
		local defender = pTheirUnit;
		local battlePlot = pBattlePlot or pTheirUnit:GetPlot();
		local interceptor = nil;

		local bRanged = false;
		local bBombing = false;
		local bSweep = false;
		-- Interface mode can give us a hint as to what kind of combat is going on, but it is not guaranteed to be correct.
		-- need to figure out how to determine the combat type from a right click.
		local interfaceMode = UI.GetInterfaceMode();
		if (interfaceMode == InterfaceModeTypes.INTERFACEMODE_AIR_SWEEP) then
			bSweep = true;
		-- Air Unit Bombing Interface Mode
		elseif (interfaceMode == InterfaceModeTypes.INTERFACEMODE_AIRSTRIKE) then
			bBombing = true;	
		-- Ranged Unit Attacking Interface Mode
		elseif (interfaceMode == InterfaceModeTypes.INTERFACEMODE_RANGE_ATTACK) then
			bRanged = true;
		-- Air Unit Attacking
		elseif (attacker:GetDomainType() == DomainTypes.DOMAIN_AIR) then
			bBombing = true;
		-- Special Case: A Unit that is both ranged and melee, but needs to set up for a ranged attack
		elseif (attacker:GetBaseRangedCombatStrength() > 0) then -- Positive Ranged Combat Strength
			-- OnlyDefensive means it cannot melee attack.
			-- MustSetUpToRangedAttack means it cannot ranged attack without setting up first.
			if (attacker:IsOnlyDefensive() or (attacker:IsMustSetUpToRangedAttack() and attacker:IsSetUpForRangedAttack())) then
				bRanged = true;
			end
		end

		-- Do we have an interceptor? plot, defender, bLandInterceptorsOnly, bVisibleOnly, bIgnoreActivity
		local iNumVisibleAAUnits = pMyUnit:GetInterceptorCount(battlePlot, nil, false, true, true);
		local bInterceptPossible = (iNumVisibleAAUnits > 0 and pMyUnit:EvasionProbability() < 100);
		-- If we have visible interceptors, and the attacker is not immune to interception, then get the best interceptor for this combat.
		if ((bInterceptPossible) and (bBombing or bSweep)) then
			if (bBombing) then
				interceptor = attacker:GetBestInterceptor(battlePlot, defender, false, true, true);
				if (interceptor == defender) then
					interceptor = nil;
				end
			elseif (bSweep) then
				interceptor = attacker:GetBestInterceptor(battlePlot, nil, false, true, true);
			end
			if (bSweep) then -- interceptors are the defenders in an airsweep.
				defender = interceptor;
				interceptor = nil;
				if defender == nil then
					return; -- no visible interceptors, so we can't simulate a sweep.
				end
			end
		end

		if (defender == nil) then
			return;
		end

		local fromPlot = nil;
		if (not bRanged and not bBombing and not bSweep) then
			fromPlot = GetMeleeAttackFromPlot(attacker, battlePlot);
		end

		local rangedSupportFireStrength = 0;
		local rangedSupportFireModList = nil;
		if (attacker:IsRangedSupportFire()) then
			rangedSupportFireStrength = attacker:GetMaxAttackStrength(battlePlot, defender, nil, interceptor, true, false, false);
			rangedSupportFireModList = attacker:GetAttackModifierList(battlePlot, defender, nil, interceptor, true, false, false, g_ModifierListLength);
		end
		local attackerStrength = attacker:GetMaxAttackStrength(battlePlot, defender, nil, interceptor, bRanged, bBombing, bSweep, fromPlot);
		local attackerModList = attacker:GetAttackModifierList(battlePlot, defender, nil, interceptor, bRanged, bBombing, bSweep, g_ModifierListLength, fromPlot);
		local defenderStrength = defender:GetMaxDefenseStrength(battlePlot, attacker, nil, interceptor, bRanged, bBombing, bSweep);

		local defenderModListLength = WillShowAirInterceptWarning(bBombing, interceptor) and (g_ModifierListLength - 1) or g_ModifierListLength;
		local defenderModList = defender:GetDefenseModifierList(battlePlot, attacker, nil, interceptor, bRanged, bBombing, bSweep, defenderModListLength);
		local interceptorStrength = 0
		local interceptorModList = nil;
		if (interceptor ~= nil) then
			interceptorStrength = interceptor:GetMaxDefenseStrength(battlePlot, attacker, nil, interceptor, bRanged, bBombing, bSweep);
			interceptorModList = interceptor:GetDefenseModifierList(battlePlot, attacker, nil, interceptor, bRanged, bBombing, bSweep, g_ModifierListLength);
		end

		local rangedSupportFireResults = nil;
		if(rangedSupportFireStrength > 0) then
			rangedSupportFireResults = Game.GetCombatDamage(attacker, nil, defender, nil, nil, battlePlot, true, false, false);
		end	

		if (attackerStrength > 0) then
			local CombatDamage = Game.GetCombatDamage(attacker, nil, defender, nil, interceptor, battlePlot, bRanged, bBombing, bSweep);
			
			local attackerResults = CombatDamage.Attacker;
			local defenderResults = CombatDamage.Defender;
			local interceptorResults = CombatDamage.Interceptor;
			local iRangedSupportFireDamage = 0;

			if (rangedSupportFireResults ~= nil and rangedSupportFireResults.Defender ~= nil) then
				iRangedSupportFireDamage = math.max(0, rangedSupportFireResults.Defender.FinalDamage - rangedSupportFireResults.Defender.CurrentDamage);
			end
			local iInterceptionDamage = 0;

			if (interceptorResults ~= nil) then
				iInterceptionDamage = interceptorResults.DamageInflicted;
			end

			-- Clamped because this sums two independent combat sims (main attack + ranged support fire); the
			-- DLL only caps each sim's own FinalDamage, not their Lua-side sum against remaining HP.
			local attackerDamageInflicted = math.max(0, defenderResults.FinalDamage - defenderResults.CurrentDamage) + iRangedSupportFireDamage;
			attackerDamageInflicted = math.min(attackerDamageInflicted, defenderResults.MaxHitPoints - defenderResults.CurrentDamage);
			local defenderDamageInflicted = math.max(0, attackerResults.FinalDamage - attackerResults.CurrentDamage);

			DoUpdateHealthBars(attacker:GetMaxHitPoints(), defender:GetMaxHitPoints(), attackerResults.CurrentDamage, defenderResults.CurrentDamage, attackerDamageInflicted, defenderDamageInflicted, iRangedSupportFireDamage, iInterceptionDamage);
			
			Controls.MyStrengthValue:SetText( Locale.ToNumber(attackerStrength / 100, "#.##"));
			Controls.TheirStrengthValue:SetText( Locale.ToNumber(defenderStrength / 100, "#.##") );
			Controls.MyDamageValue:SetText("[COLOR_GREEN]" .. attackerDamageInflicted .. "[ENDCOLOR]");
			Controls.TheirDamageValue:SetText("[COLOR_RED]" .. defenderDamageInflicted .. "[ENDCOLOR]");

			-- Now do text stuff
			local controlTable;
			local strValue;

			for _, modifier in ipairs(attackerModList) do
				local controlTable = g_MyCombatDataIM:GetInstance();
				SetCombatModifierRow(controlTable, modifier, true);
			end
			AddAirInterceptWarning(bBombing, interceptor);
			for _, modifier in ipairs(defenderModList) do
				local controlTable = g_TheirCombatDataIM:GetInstance();
				SetCombatModifierRow(controlTable, modifier, false);
			end
			
			if (bRanged or bBombing or bSweep) then
				local textKey;
				if bSweep then
					textKey = "TXT_KEY_INTERFACEMODE_AIR_SWEEP";
				elseif bBombing then
					textKey = "TXT_KEY_INTERFACEMODE_AIRSTRIKE";
				else -- bRanged
					textKey = "TXT_KEY_INTERFACEMODE_RANGE_ATTACK";
				end
				SetCombatIndicator("Green", textKey);
			else
				local bAttackerDies = (attackerResults.CurrentDamage + defenderDamageInflicted) >= attacker:GetMaxHitPoints();
				local bDefenderDies = (defenderResults.CurrentDamage + attackerDamageInflicted) >= defender:GetMaxHitPoints();

				if (bAttackerDies and bDefenderDies) then
					SetCombatIndicator("Yellow", "TXT_KEY_EUPANEL_STALEMATE");
				elseif (bAttackerDies) then
					SetCombatIndicator("Red", "TXT_KEY_EUPANEL_TOTAL_DEFEAT");
				elseif (bDefenderDies) then
					SetCombatIndicator("Green", "TXT_KEY_EUPANEL_TOTAL_VICTORY");
				elseif (attackerDamageInflicted - defenderDamageInflicted > 30) then
					SetCombatIndicator("Green", "TXT_KEY_EUPANEL_MAJOR_VICTORY");
				elseif (attackerDamageInflicted > defenderDamageInflicted) then
					SetCombatIndicator("Green", "TXT_KEY_EUPANEL_MINOR_VICTORY");
				elseif (defenderDamageInflicted - attackerDamageInflicted > 30) then
					SetCombatIndicator("Red", "TXT_KEY_EUPANEL_MAJOR_DEFEAT");
				elseif (attackerDamageInflicted < defenderDamageInflicted) then
					SetCombatIndicator("Red", "TXT_KEY_EUPANEL_MINOR_DEFEAT");
				else
					SetCombatIndicator("Yellow", "TXT_KEY_EUPANEL_STALEMATE");
				end
			end
			-------------------------
			-- PRIZE SHIPS PREVIEW --
			-------------------------
			if (not bRanged and not bSweep) then
				local iChance;
				iChance = pMyUnit:GetCaptureChance(defender);
				if (iChance > 0) then
					controlTable = g_TheirCombatDataIM:GetInstance();
					controlTable.Text:LocalizeAndSetText("TXT_KEY_EUPANEL_CAPTURE_CHANCE", iChance);
					controlTable.Value:SetText("");
					controlTable.Separator:SetHide(true);
				end
			end
		end
	end
	
    Controls.MyCombatResultsStack:CalculateSize();
    Controls.TheirCombatResultsStack:CalculateSize();
    
    local sizeX = Controls.DetailsGrid:GetSizeX();
    Controls.DetailsGrid:DoAutoSize();
    Controls.DetailsGrid:SetSizeX( sizeX );
    Controls.DetailsSeperator:SetSizeY( Controls.DetailsGrid:GetSizeY() );
    Controls.DetailsGrid:ReprocessAnchoring();
end

function UpdateCombatOddsCityVsUnit(myCity, theirUnit)

	g_MyCombatDataIM:ResetInstances();
	g_TheirCombatDataIM:ResetInstances();
	g_TheirNoteIM:ResetInstances();

	if (myCity == nil or theirUnit == nil) then
		return false;
	end

	local attacker = myCity;
	local defender = theirUnit;
	local battlePlot = defender:GetPlot();

	if (battlePlot == nil) then
		return false;
	end

	local bRanged = true;
	local bBombing = false;
	local bSweep = false;

	local attackerStrength = attacker:GetStrengthValue(true);
	local attackerModList = attacker:GetCityCombatModifierList(true);
	local defenderStrength = defender:GetMaxDefenseStrength( battlePlot, nil, attacker, nil, bRanged, bBombing, bSweep);
	local defenderModList = defender:GetDefenseModifierList( battlePlot, nil, attacker, nil, bRanged, bBombing, bSweep, g_ModifierListLength);

	if (attackerStrength <= 0 or defenderStrength <= 0) then
		return false;
	end

	local CombatDamage = Game.GetCombatDamage( nil, attacker, defender, nil, nil, battlePlot, bRanged, bBombing, bSweep);

	if ( CombatDamage == nil or CombatDamage.Attacker == nil or CombatDamage.Defender == nil ) then
		return false;
	end

	local attackerResults = CombatDamage.Attacker;
	local defenderResults = CombatDamage.Defender;

	local attackerDamageInflicted = math.max( 0, defenderResults.FinalDamage - defenderResults.CurrentDamage);
	local defenderDamageInflicted = math.max( 0, attackerResults.FinalDamage - attackerResults.CurrentDamage);

	Controls.MyDamageValue:SetText("[COLOR_GREEN]" .. attackerDamageInflicted .. "[ENDCOLOR]");
	Controls.MyStrengthValue:SetText(Locale.ToNumber(attackerStrength / 100, "#.##"));
	Controls.TheirDamageValue:SetText( "[COLOR_RED]" .. defenderDamageInflicted .. "[ENDCOLOR]");
	Controls.TheirStrengthValue:SetText(Locale.ToNumber(defenderStrength / 100, "#.##"));


	DoUpdateHealthBars( attackerResults.MaxHitPoints, defenderResults.MaxHitPoints, attackerResults.CurrentDamage, defenderResults.CurrentDamage, attackerDamageInflicted, defenderDamageInflicted, 0, 0);


	for _, modifier in ipairs(attackerModList) do
		local controlTable = g_MyCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, true);
	end

	for _, modifier in ipairs(defenderModList) do
		local controlTable = g_TheirCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, false);
	end


	SetCombatIndicator("Green", "TXT_KEY_INTERFACEMODE_RANGE_ATTACK");

	Controls.MyCombatResultsStack:CalculateSize();
	Controls.TheirCombatResultsStack:CalculateSize();

	local sizeX = Controls.DetailsGrid:GetSizeX();
	Controls.DetailsGrid:DoAutoSize();
	Controls.DetailsGrid:SetSizeX(sizeX);
	Controls.DetailsSeperator:SetSizeY(Controls.DetailsGrid:GetSizeY());
	Controls.DetailsGrid:ReprocessAnchoring();

	return true;
end

function UpdateCombatOddsCityVsCity(myCity, theirCity)

	g_MyCombatDataIM:ResetInstances();
	g_TheirCombatDataIM:ResetInstances();
	g_TheirNoteIM:ResetInstances();

	if (myCity == nil or theirCity == nil) then
		return false;
	end

	local attacker = myCity;
	local defender = theirCity;
	local battlePlot = defender:Plot();

	if (battlePlot == nil) then
		return false;
	end

	local bRanged = true;
	local bBombing = false;
	local bSweep = false;

	-- Attacking city uses range-strike strength.
	local attackerStrength = attacker:GetStrengthValue(true);
	local attackerModList = attacker:GetCityCombatModifierList(true);

	-- Defending city uses full defensive strength.
	local defenderStrength = defender:GetStrengthValue(false);
	local defenderModList = defender:GetCityCombatModifierList(false);

	if (attackerStrength <= 0 or defenderStrength <= 0) then
		return false;
	end

	local CombatDamage = Game.GetCombatDamage(
		nil,        -- attacker unit
		attacker,   -- attacker city
		nil,        -- defender unit
		defender,   -- defender city
		nil,        -- interceptor
		battlePlot,
		bRanged,
		bBombing,
		bSweep
	);

	if ( CombatDamage == nil or CombatDamage.Attacker == nil or CombatDamage.Defender == nil) then
		return false;
	end

	local attackerResults = CombatDamage.Attacker;
	local defenderResults = CombatDamage.Defender;

	local attackerDamageInflicted = math.max( 0, defenderResults.FinalDamage - defenderResults.CurrentDamage);
	local defenderDamageInflicted = math.max( 0, attackerResults.FinalDamage - attackerResults.CurrentDamage);


	Controls.MyDamageValue:SetText( "[COLOR_GREEN]" .. attackerDamageInflicted .. "[ENDCOLOR]");
	Controls.MyStrengthValue:SetText(Locale.ToNumber(attackerStrength / 100, "#.##"));
	Controls.TheirDamageValue:SetText( "[COLOR_RED]" .. defenderDamageInflicted .. "[ENDCOLOR]");
	Controls.TheirStrengthValue:SetText(Locale.ToNumber(defenderStrength / 100, "#.##"));

	DoUpdateHealthBars( attackerResults.MaxHitPoints, defenderResults.MaxHitPoints, attackerResults.CurrentDamage, defenderResults.CurrentDamage, attackerDamageInflicted, defenderDamageInflicted, 0, 0);


	for _, modifier in ipairs(attackerModList) do
		local controlTable = g_MyCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, true);
	end

	for _, modifier in ipairs(defenderModList) do
		local controlTable = g_TheirCombatDataIM:GetInstance();
		SetCombatModifierRow(controlTable, modifier, false);
	end

	SetCombatIndicator("Green", "TXT_KEY_INTERFACEMODE_RANGE_ATTACK");


	Controls.MyCombatResultsStack:CalculateSize();
	Controls.TheirCombatResultsStack:CalculateSize();

	local sizeX = Controls.DetailsGrid:GetSizeX();
	Controls.DetailsGrid:DoAutoSize();
	Controls.DetailsGrid:SetSizeX(sizeX);
	Controls.DetailsSeperator:SetSizeY(Controls.DetailsGrid:GetSizeY());
	Controls.DetailsGrid:ReprocessAnchoring();

	return true;
end

function OnInterfaceModeChanged()
	if (g_lastHexX ~= nil and g_lastHexY ~= nil) then
		OnMouseOverHex(g_lastHexX, g_lastHexY);
	end
end
Events.InterfaceModeChanged.Add(OnInterfaceModeChanged);

--------------------------------------------------------------------------------
-- Update Health Bar combat preview
--------------------------------------------------------------------------------
function DoUpdateHealthBars(iMaxMyHP, iTheirMaxHP, myCurrentDamage, theirCurrentDamage, iMyDamageInflicted, iTheirDamageInflicted, iMyExtraDamageInflicted, iTheirExtraDamageInflicted)
	iMyExtraDamageInflicted = math.max(0, iMyExtraDamageInflicted or 0);
	iTheirExtraDamageInflicted = math.max(0, iTheirExtraDamageInflicted or 0);


	local myRemainingHP = math.max(0, iMaxMyHP - myCurrentDamage);
	local myDamageTaken = math.max(0, math.min(iTheirDamageInflicted, myRemainingHP));

	local myExtraDamageTaken = math.min(iTheirExtraDamageInflicted, myDamageTaken);
	local myNormalDamageTaken = myDamageTaken - myExtraDamageTaken;

	myCurrentDamage = myCurrentDamage + myDamageTaken;

	local myHealthPercent = (iMaxMyHP - myCurrentDamage) / iMaxMyHP;
	local myHealthTimes100 = math.floor(100 * myHealthPercent + 0.5);

	local myHealthBarSize =
	{
		x = 8,
		y = math.floor(115 * myHealthPercent)
	};

	if myHealthTimes100 <= 30 then
		Controls.MyRedBar:SetSize(myHealthBarSize);
		Controls.MyGreenBar:SetHide(true);
		Controls.MyYellowBar:SetHide(true);
		Controls.MyRedBar:SetHide(false);
	elseif myHealthTimes100 <= 50 then
		Controls.MyYellowBar:SetSize(myHealthBarSize);
		Controls.MyGreenBar:SetHide(true);
		Controls.MyYellowBar:SetHide(false);
		Controls.MyRedBar:SetHide(true);
	else
		Controls.MyGreenBar:SetSize(myHealthBarSize);
		Controls.MyGreenBar:SetHide(false);
		Controls.MyYellowBar:SetHide(true);
		Controls.MyRedBar:SetHide(true);
	end

	local myDamageOffset;

	if myHealthBarSize.y > 0 then
		myDamageOffset = myHealthBarSize.y + 4;
	else
		myDamageOffset = 2;
	end

	-- Main combat damage: flashing red.
	local myNormalDamageBarSize =
	{
		x = 8,
		y = math.floor(115 * myNormalDamageTaken / iMaxMyHP)
	};

	if myNormalDamageTaken > 0 then
		Controls.MyDeltaBar:SetHide(false);
		Controls.MyDeltaBar:SetOffsetVal(0, myDamageOffset);
		Controls.MyDeltaBar:SetSize(myNormalDamageBarSize);
		Controls.MyDeltaBarFlash:SetSize(myNormalDamageBarSize);
	else
		Controls.MyDeltaBar:SetHide(true);
	end

	-- Preliminary damage: flashing orange.
	if myExtraDamageTaken > 0 then
		local myExtraDamageBarSize =
		{
			x = 8,
			y = math.floor(115 * myExtraDamageTaken / iMaxMyHP)
		};

		Controls.MyExtraDeltaBar:SetHide(false);
		Controls.MyExtraDeltaBar:SetOffsetVal(0, myDamageOffset + myNormalDamageBarSize.y);
		Controls.MyExtraDeltaBar:SetSize(myExtraDamageBarSize);
		Controls.MyExtraDeltaBarFlash:SetSize(myExtraDamageBarSize);
	else
		Controls.MyExtraDeltaBar:SetHide(true);
	end


	local theirRemainingHP = math.max(0, iTheirMaxHP - theirCurrentDamage);

	local theirDamageTaken = math.max(0, math.min(iMyDamageInflicted, theirRemainingHP));

	-- Extra damage is already included in theirDamageTaken.
	local theirExtraDamageTaken = math.min(iMyExtraDamageInflicted, theirDamageTaken);
	local theirNormalDamageTaken = theirDamageTaken - theirExtraDamageTaken;

	theirCurrentDamage = theirCurrentDamage + theirDamageTaken;

	-- Remaining health.
	local theirHealthPercent = (iTheirMaxHP - theirCurrentDamage) / iTheirMaxHP;

	local theirHealthTimes100 = math.floor(100 * theirHealthPercent + 0.5);

	local theirHealthBarSize =
	{
		x = 8,
		y = math.floor(115 * theirHealthPercent)
	};

	if theirHealthTimes100 <= 30 then
		Controls.TheirRedBar:SetSize(theirHealthBarSize);
		Controls.TheirGreenBar:SetHide(true);
		Controls.TheirYellowBar:SetHide(true);
		Controls.TheirRedBar:SetHide(false);
	elseif theirHealthTimes100 <= 50 then
		Controls.TheirYellowBar:SetSize(theirHealthBarSize);
		Controls.TheirGreenBar:SetHide(true);
		Controls.TheirYellowBar:SetHide(false);
		Controls.TheirRedBar:SetHide(true);
	else
		Controls.TheirGreenBar:SetSize(theirHealthBarSize);
		Controls.TheirGreenBar:SetHide(false);
		Controls.TheirYellowBar:SetHide(true);
		Controls.TheirRedBar:SetHide(true);
	end

	local theirDamageOffset;

	if theirHealthBarSize.y > 0 then
		theirDamageOffset = theirHealthBarSize.y + 4;
	else
		theirDamageOffset = 2;
	end

	-- Main combat damage: flashing red.
	local theirNormalDamageBarSize =
	{
		x = 8,
		y = math.floor(115 * theirNormalDamageTaken / iTheirMaxHP)
	};

	if theirNormalDamageTaken > 0 then
		Controls.TheirDeltaBar:SetHide(false);
		Controls.TheirDeltaBar:SetOffsetVal(0,theirDamageOffset);
		Controls.TheirDeltaBar:SetSize(theirNormalDamageBarSize);
		Controls.TheirDeltaBarFlash:SetSize(theirNormalDamageBarSize);
	else
		Controls.TheirDeltaBar:SetHide(true);
	end

	-- Preliminary damage: flashing orange.
	if theirExtraDamageTaken > 0 then
		local theirExtraDamageBarSize =
		{
			x = 8,
			y = math.floor( 115 * theirExtraDamageTaken / iTheirMaxHP )
		};

		Controls.TheirExtraDeltaBar:SetHide(false);
		Controls.TheirExtraDeltaBar:SetOffsetVal(0, theirDamageOffset + theirNormalDamageBarSize.y);
		Controls.TheirExtraDeltaBar:SetSize(theirExtraDamageBarSize);
		Controls.TheirExtraDeltaBarFlash:SetSize(theirExtraDamageBarSize);
	else
		Controls.TheirExtraDeltaBar:SetHide(true);
	end
end


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
function OnWorldMouseOver( bWorldHasMouseOver )
    g_bWorldMouseOver = bWorldHasMouseOver;
    
	if( g_bShowPanel and g_bWorldMouseOver ) then
		ContextPtr:SetHide( false );
	else
		ContextPtr:SetHide( true );
    end
end
Events.WorldMouseOver.Add( OnWorldMouseOver );

function UnitRangedAttackOnlyInDomain( pUnit, pPlot )
	if (pUnit:IsRanged() and pUnit:IsRangeAttackOnlyInDomain()) then
		-- Walk-water (pontoon) is land-like for domain-restricted ranged attacks.
		local bPlotWater = pPlot:IsWater() and not (pPlot.IsAllowsWalkWater and pPlot:IsAllowsWalkWater());
		if (pUnit:GetDomainType() == DomainTypes.DOMAIN_LAND and bPlotWater) then
			return true;
		elseif (pUnit:GetDomainType() == DomainTypes.DOMAIN_SEA and not bPlotWater) then
			return true;
		end
	end
	return false;
end

-- Cross-domain melee prediction rules (must match what can actually attack).
function CanShowCrossDomainMeleePrediction( pHeadUnit, pUnit, pBattlePlot )
	-- Hover units (DOMAIN_HOVER / hovering promo) can always melee land and naval.
	-- getDomainType() morphs with terrain, so same-domain checks alone miss many cases.
	if ((pHeadUnit.IsHoveringUnit and pHeadUnit:IsHoveringUnit()) or (pUnit.IsHoveringUnit and pUnit:IsHoveringUnit())) then
		return true;
	end
	if (pHeadUnit:GetDomainType() == pUnit:GetDomainType()) then
		return true;
	end
	if (pHeadUnit:IsRanged()) then
		return true;
	end
	if (pHeadUnit:GetDomainType() == DomainTypes.DOMAIN_SEA and pUnit:IsEmbarked()) then
		return true;
	end

	-- Walk-water only enables cross-domain when the *defender* is on the pontoon
	-- (land can attack onto it; naval can fight land stacked there). Naval on a
	-- pontoon still cannot attack a true land tile.
	if (pBattlePlot ~= nil and pBattlePlot.IsAllowsWalkWater and pBattlePlot:IsAllowsWalkWater()) then
		return true;
	end

	return false;
end

--------------------------------------------------------------------------------
-- Hex has been moused over
--------------------------------------------------------------------------------
function OnMouseOverHex( hexX, hexY )

	g_lastHexX = hexX;
	g_lastHexY = hexY;

	g_bShowPanel = false;

	Controls.MyCombatResultsStack:SetHide(true);
	Controls.TheirCombatResultsStack:SetHide(true);

	local pPlot = Map.GetPlot( hexX, hexY );

	if (pPlot ~= nil) then
		local pHeadUnit = UI.GetHeadSelectedUnit();
		local pHeadPlayer = Players[Game.GetActivePlayer()];
		local pHeadCity = UI.GetHeadSelectedCity();
		local FightWhileEmbarked = pHeadPlayer:IsEmbarkedMissionAllowed(GameInfoTypes.MISSION_RANGE_ATTACK);
		local bCtrl = UIManager:GetControl(); -- Hold Control to preview the fight as if the enemy were attacking us (defensive combat). Works without a war, same as Alt.
		local bEmbarkedFullStrength = pHeadPlayer:IsEmbarkedUnitsFullStrength(); -- Trait: embarked units fight at full strength, so they can melee naval/embarked units too.

		if (pHeadUnit ~= nil) then

			local interfaceMode = UI.GetInterfaceMode();

			if (interfaceMode == InterfaceModeTypes.INTERFACEMODE_AIR_SWEEP) then

				-- A sweep can be contested by an interceptor even when no unit is sitting on the swept tile itself.
				local iTeam = Game.GetActiveTeam();

				if (pPlot:IsRevealed(iTeam, false)) then
					-- Ignore the interceptor's exact readiness state -- see the comment on the other GetBestInterceptor calls.
					local interceptor = pHeadUnit:GetBestInterceptor(pPlot, nil, false, true, true);

					if (interceptor ~= nil) then
						UpdateUnitPortrait(interceptor);
						UpdateUnitPromotions(interceptor);
						UpdateUnitStats(interceptor);

						UpdateCombatOddsUnitVsUnit(pHeadUnit, nil, pPlot);

						Controls.MyCombatResultsStack:SetHide(false);
						Controls.TheirCombatResultsStack:SetHide(false);

						g_bShowPanel = true;
					end
				end

			elseif (pHeadUnit:IsCombatUnit() and (pHeadUnit:IsRanged() and pHeadUnit:IsEmbarked() and not FightWhileEmbarked) == false) and (UnitRangedAttackOnlyInDomain(pHeadUnit, pPlot) == false) then

				local iTeam = Game.GetActiveTeam()
				local pTeam = Teams[iTeam]
				
				-- Don't show info for stuff we can't see
				if (pPlot:IsRevealed(iTeam, false)) then
					
					-- City
					if (pPlot:IsCity()) then
						
						local pCity = pPlot:GetPlotCity();
						
						if (pTeam:IsAtWar(pCity:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and pCity:GetOwner() ~= iTeam)) then
							UpdateCityStats(pCity);
							if (bCtrl) then
								UpdateCombatOddsCityVsUnit(pCity, pHeadUnit); -- Defensive preview: simulate their city bombarding us.
							else
								UpdateCombatOddsUnitVsCity(pHeadUnit, pCity);
							end
							UpdateUnitPromotions(nil);
							UpdateCityPortrait(pCity);

							Controls.MyCombatResultsStack:SetHide(false);
							Controls.TheirCombatResultsStack:SetHide(false);
							g_bShowPanel = true;
						end
						
					-- No City Here
					else
						
						-- Can see this plot right now
						if (pPlot:IsVisible(iTeam, false) and not pHeadUnit:IsCityAttackOnly()) then
							
							local iNumUnits = pPlot:GetNumUnits();
							local pUnit;
							
							-- Loop through all Units
							for i = 0, iNumUnits do
								pUnit = pPlot:GetUnit(i);
								if (pUnit ~= nil and not pUnit:IsInvisible(iTeam, false)) then
									
									-- No air units
									--if (pUnit:GetDomainType() ~= DomainTypes.DOMAIN_AIR) then
										
										-- Other guy must be same domain, OR we must be ranged, OR we must be naval and he is embarked,
										-- OR we are embarked ourselves and our trait lets embarked units fight at full strength (melee vs. naval/embarked targets),
										-- OR walk-water (pontoon) enables cross-domain melee.
										if (CanShowCrossDomainMeleePrediction(pHeadUnit, pUnit, pPlot)
											or (bEmbarkedFullStrength and pHeadUnit:IsEmbarked() and (pUnit:GetDomainType() == DomainTypes.DOMAIN_SEA or pUnit:IsEmbarked()))) then
										
											 if (pUnit:GetBaseCombatStrength() > 0 or pHeadUnit:IsRanged()) then
												UpdateUnitPortrait(pUnit);
												UpdateUnitPromotions(pUnit);
												UpdateUnitStats(pUnit);
												
												if (pTeam:IsAtWar(pUnit:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and pUnit:GetOwner() ~= iTeam)) then
													if (bCtrl) then
														UpdateCombatOddsUnitVsUnit(pUnit, pHeadUnit); -- Defensive preview: simulate them attacking us.
													else
														UpdateCombatOddsUnitVsUnit(pHeadUnit, pUnit);
													end
													Controls.MyCombatResultsStack:SetHide(false);
													Controls.TheirCombatResultsStack:SetHide(false);
													
													g_bShowPanel = true;
													break;
												end
											end
										end
									--end
								end
							end
						end
					end
				end
			elseif(pHeadUnit:IsRanged() == true and (pHeadUnit:IsEmbarked() == false or FightWhileEmbarked)) then
				
				local iTeam = Game.GetActiveTeam()
				local pTeam = Teams[iTeam]
				
				-- Don't show info for stuff we can't see
				if (pPlot:IsRevealed(iTeam, false)) then
					
					-- City
					if (pPlot:IsCity()) then
						
						local pCity = pPlot:GetPlotCity();
						
						if (pTeam:IsAtWar(pCity:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and pCity:GetOwner() ~= iTeam)) then
							UpdateCityStats(pCity);
							if (bCtrl) then
								UpdateCombatOddsCityVsUnit(pCity, pHeadUnit); -- Defensive preview: simulate their city bombarding us.
							else
								UpdateCombatOddsUnitVsCity(pHeadUnit, pCity);
							end
							UpdateUnitPromotions(nil);
							UpdateCityPortrait(pCity);

							Controls.MyCombatResultsStack:SetHide(false);
							Controls.TheirCombatResultsStack:SetHide(false);
							g_bShowPanel = true;
						end
						
					-- No City Here
					else
						
						-- Can see this plot right now
						if (pPlot:IsVisible(iTeam, false)) then
							
							local iNumUnits = pPlot:GetNumUnits();
							local pUnit;
							
							-- Loop through all Units
							for i = 0, iNumUnits do
								pUnit = pPlot:GetUnit(i);
								if (pUnit ~= nil and not pUnit:IsInvisible(iTeam, false)) then
									 if (pUnit:GetBaseCombatStrength() > 0 or pHeadUnit:IsRanged()) then
										UpdateUnitPortrait(pUnit);
										UpdateUnitPromotions(pUnit);
										UpdateUnitStats(pUnit);
												
										if (pTeam:IsAtWar(pUnit:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and pUnit:GetOwner() ~= iTeam)) then
											if (bCtrl) then
												UpdateCombatOddsUnitVsUnit(pUnit, pHeadUnit); -- Defensive preview: simulate them attacking us.
											else
												UpdateCombatOddsUnitVsUnit(pHeadUnit, pUnit);
											end
											Controls.MyCombatResultsStack:SetHide(false);
											Controls.TheirCombatResultsStack:SetHide(false);
											
											g_bShowPanel = true;
											break;
										end
									end
								end
							end
						end
					end
				end
				
			end
		
		elseif(pHeadCity ~= nil and pHeadCity:CanRangeStrikeNow()) then	-- no unit selected, what about a city?

			local myTeamID = Game.GetActiveTeam();
			local myTeam = Teams[myTeamID];

			-- Don't show info for stuff we can't see
			if (pPlot:IsVisible(myTeamID, false)) then

				if (pPlot:IsCity()) then

					-- Enemy city info
					local theirCity = pPlot:GetPlotCity();

					if (theirCity ~= nil and (myTeam:IsAtWar(theirCity:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and theirCity:GetOwner() ~= myTeamID))) then

						UpdateCityStats(theirCity);
						if (bCtrl) then
							UpdateCombatOddsCityVsCity(theirCity, pHeadCity); -- Defensive preview: simulate their city bombarding ours.
						else
							UpdateCombatOddsCityVsCity(pHeadCity, theirCity);
						end
						UpdateUnitPromotions(nil);
						UpdateCityPortrait(theirCity);

						Controls.MyCombatResultsStack:SetHide(false);
						Controls.TheirCombatResultsStack:SetHide(false);

						g_bShowPanel = true;
					end

				else

					local numUnitsOnPlot = pPlot:GetNumUnits();

					-- Loop through all Units
					for i = 0, numUnitsOnPlot - 1 do
						local theirUnit = pPlot:GetUnit(i);


						if (theirUnit ~= nil and not theirUnit:IsInvisible(myTeamID, false)) then

							-- No air units
							if (theirUnit:GetDomainType() ~= DomainTypes.DOMAIN_AIR) then

								if (myTeam:IsAtWar(theirUnit:GetTeam()) or ((UIManager:GetAlt() or bCtrl) and theirUnit:GetOwner() ~= myTeamID)) then

									-- Enemy Unit info
									UpdateUnitPortrait(theirUnit);
									UpdateUnitPromotions(theirUnit);
									UpdateUnitStats(theirUnit);

									if (bCtrl) then
										UpdateCombatOddsUnitVsCity(theirUnit, pHeadCity); -- Defensive preview: simulate their unit attacking our city.
									else
										UpdateCombatOddsCityVsUnit(pHeadCity, theirUnit);
									end

									Controls.MyCombatResultsStack:SetHide(false);
									Controls.TheirCombatResultsStack:SetHide(false);

									g_bShowPanel = true;

									break;
								end
							end
						end
					end
				end
			end
		end
	end
	
	if( g_bShowPanel and g_bWorldMouseOver ) then
		ContextPtr:SetHide( false );
	else
		ContextPtr:SetHide( true );
	end
end
Events.SerialEventMouseOverHex.Add( OnMouseOverHex );


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
function ShowHideHandler( bIsHide, bIsInit )
    if( not bIsInit ) then
        LuaEvents.EnemyPanelHide( bIsHide );
    end
end
ContextPtr:SetShowHideHandler( ShowHideHandler );