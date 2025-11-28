/*  -------------------------------------------------------------------------------------------------------
This is an experimental system to spice up the game without touching the core of civilizations.
It allows for the addition of "legacies" that can be granted to civilizations, providing unique bonuses or abilities based on historical or thematic elements.
This is mimicking the policy system in implementation, so reference that.
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvLegacyAI.h"
#include "CvFlavorManager.h"
#include "ICvDLLUserInterface.h"
#include "CvGameCoreUtils.h"
#include "CvEconomicAI.h"
#include "CvGrandStrategyAI.h"
#include "CvInfosSerializationHelper.h"

#include "LintFree.h"
#if defined(LEKMOD_LEGACY)
// Constructor
CvLegacyEntry::CvLegacyEntry(void) :
	m_iCivilization(NO_CIVILIZATION),
	m_iEraOffered(NO_ERA),
	m_bInstant(false),
	m_pbHasOneShotUnits(NULL),

	m_iHappinessPerOriginalCity(0),
	m_iGoldenAgeTurns(0),
	m_iGreatGeneralSiegeBonus(0),
	m_iResistanceTimeReduction(0),
	m_iYieldModCapitalProximity(0),
	m_iPlotGoldCostModifier(0),
	m_iPlotCultureCostModifier(0),
	m_iHappinessFromGreatImprovements(0),
	m_iHappinessFromForeignReligiousMajority(0),
	m_iVotesPerCapital(0),
	m_iInfluenceChangeMajorityReligion(0),
	m_iInfluenceChangeTradeConnection(0),
	m_iPurchasedUnitExtraMoves(0),
	m_iHappinessPerTheme(0),
	m_bTradeUnplunderable(false),
	m_bCannotPlunder(false),

	m_piYieldBonusFromThemes(NULL),
	m_piBuildTimeOverride(NULL),
	m_piNumFreeUnitsByClass(NULL),
	m_piNumFreeUnitsByType(NULL),
	m_piOriginalCityYieldChange(NULL),
	m_piConqueredCityYieldChange(NULL),
	m_paiBuildingCostOverride(NULL),
	m_paiUnitCostOverride(NULL),
	m_piLegacyBuildingClassOverride(NULL),
	m_piLegacyUnitClassOverride(NULL),
	m_piPlotPurchaseYieldReward(NULL),
	m_piCityYieldChange(NULL),
	m_piCityYieldModifier(NULL),
	m_piBuildingClassProductionModifier(NULL),
	m_piBuildingClassHappinessChange(NULL),
	m_piBuildingClassGlobalHappinessChange(NULL),
	m_paiBuildingClassYieldChange(NULL),
	m_paiBuildingClassYieldModifier(NULL),
	m_paiBuildingClassGreatPersonPointChange(NULL),
	m_paiBuildingClassGreatPersonPointModifier(NULL),
	m_paiUnitResourceRequirementChange(NULL),
	m_piUnitRangedStrengthChange(NULL),
	m_piUnitStrengthChange(NULL),
	m_paiResourceYieldChange(NULL),
	m_paiResourceClassYieldChange(NULL),
	m_paiSpecialistYieldChange(NULL),
	m_piSpecialistHappinessChange(NULL),
	m_paiImprovementYieldChange(NULL),
	m_paiImprovementYieldChangePerXWorldWonder(NULL),
	m_paiImprovementNearbyHealChangeByDomain(NULL),
	m_paiImprovementNearbyCombatModifierByDomain(NULL),
	m_piPromotionNearbyGeneralUnitCombat(NULL),
	m_paiGreatWorkClassYieldChange(NULL),
	m_piGreatWorkClassTourismChange(NULL),
	m_piImprovementTourismBonus(NULL),
	m_pbFreePromotion(NULL)
{
}
// Destructor
CvLegacyEntry::~CvLegacyEntry(void)
{
	SAFE_DELETE_ARRAY(m_pbHasOneShotUnits);
	SAFE_DELETE_ARRAY(m_pbFreePromotion);
	SAFE_DELETE_ARRAY(m_piLegacyBuildingClassOverride);
	SAFE_DELETE_ARRAY(m_piLegacyUnitClassOverride);
	SAFE_DELETE_ARRAY(m_piPlotPurchaseYieldReward);
	SAFE_DELETE_ARRAY(m_piCityYieldChange);
	SAFE_DELETE_ARRAY(m_piOriginalCityYieldChange);
	SAFE_DELETE_ARRAY(m_piConqueredCityYieldChange);
	SAFE_DELETE_ARRAY(m_piCityYieldModifier);
	SAFE_DELETE_ARRAY(m_piBuildingClassProductionModifier);
	SAFE_DELETE_ARRAY(m_piBuildingClassHappinessChange);
	SAFE_DELETE_ARRAY(m_piBuildingClassGlobalHappinessChange);
	SAFE_DELETE_ARRAY(m_piUnitRangedStrengthChange);
	SAFE_DELETE_ARRAY(m_piUnitStrengthChange);
	SAFE_DELETE_ARRAY(m_piSpecialistHappinessChange);
	SAFE_DELETE_ARRAY(m_piPromotionNearbyGeneralUnitCombat);
	SAFE_DELETE_ARRAY(m_piGreatWorkClassTourismChange);
	SAFE_DELETE_ARRAY(m_piImprovementTourismBonus);
	SAFE_DELETE_ARRAY(m_piNumFreeUnitsByClass);
	SAFE_DELETE_ARRAY(m_piNumFreeUnitsByType);
	SAFE_DELETE_ARRAY(m_piBuildTimeOverride);
	SAFE_DELETE_ARRAY(m_piYieldBonusFromThemes);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingCostOverride);
	CvDatabaseUtility::SafeDelete2DArray(m_paiUnitCostOverride);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassYieldModifier);
	CvDatabaseUtility::SafeDelete2DArray(m_paiResourceYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiResourceClassYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiSpecialistYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassGreatPersonPointModifier);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassGreatPersonPointChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiUnitResourceRequirementChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiImprovementYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiImprovementYieldChangePerXWorldWonder);
	CvDatabaseUtility::SafeDelete2DArray(m_paiImprovementNearbyHealChangeByDomain);
	CvDatabaseUtility::SafeDelete2DArray(m_paiImprovementNearbyCombatModifierByDomain);
	CvDatabaseUtility::SafeDelete2DArray(m_paiGreatWorkClassYieldChange);
}

// Cache results from the database
bool CvLegacyEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	// Base info consists of ID, Type, Text, Civilopedia, Strategy, Help, DisabledHelp, Description.
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	const char* szCivilization = kResults.GetText("CivilizationType");
	m_iCivilization = GC.getInfoTypeForString(szCivilization, true);
	const char* szEra = kResults.GetText("EraOffered");
	m_iEraOffered = GC.getInfoTypeForString(szEra, true);
	m_bInstant = kResults.GetBool("Instant");
	// Initialize a Derived Array
	m_pbHasOneShotUnits = FNEW(bool[GC.getNumLegacyInfos()], c_eCiv5GameplayDLL, 0);

	m_iHappinessPerOriginalCity = kResults.GetInt("HappinessPerOriginalCity");
	m_iGoldenAgeTurns = kResults.GetInt("GoldenAgeTurns");
	m_iGreatGeneralSiegeBonus = kResults.GetInt("GreatGeneralSiegeBonus");
	m_iResistanceTimeReduction = kResults.GetInt("ResistanceTimeReduction");
	m_iYieldModCapitalProximity = kResults.GetInt("YieldModCapitalProximity");
	m_iPlotGoldCostModifier = kResults.GetInt("PlotGoldCostModifier");
	m_iPlotCultureCostModifier = kResults.GetInt("PlotCultureCostModifier");
	m_iHappinessFromGreatImprovements = kResults.GetInt("HappinessFromGreatImprovements");
	m_iHappinessFromForeignReligiousMajority = kResults.GetInt("HappinessFromForeignReligiousMajority");
	m_iVotesPerCapital = kResults.GetInt("VotesPerCapital");
	m_iInfluenceChangeMajorityReligion = kResults.GetInt("InfluenceChangeMajorityReligion");
	m_iInfluenceChangeTradeConnection = kResults.GetInt("InfluenceChangeTradeConnection");
	m_iPurchasedUnitExtraMoves = kResults.GetInt("PurchasedUnitExtraMoves");
	m_iHappinessPerTheme = kResults.GetInt("HappinessFromThemes");

	m_bTradeUnplunderable = kResults.GetBool("TradeUnplunderable");
	m_bCannotPlunder = kResults.GetBool("CannotPlunder");

	// Arrays Start.
	const char* szLegacyType = GetType();
	kUtility.SetYields(m_piPlotPurchaseYieldReward, "Legacy_PlotPurchaseYieldReward", "LegacyType", szLegacyType);
	kUtility.SetYields(m_piYieldBonusFromThemes, "Legacy_YieldBonusFromThemes", "LegacyType", szLegacyType);

	kUtility.PopulateArrayByExistence(m_pbFreePromotion, "UnitPromotions", "Legacy_FreePromotions", "PromotionType", "LegacyType", szLegacyType);

	kUtility.PopulateArrayByValue(m_piCityYieldModifier, "Yields", "Legacy_CityYieldModifier", "YieldType", "LegacyType", szLegacyType, "Modifier");
	kUtility.PopulateArrayByValue(m_piBuildingClassProductionModifier, "BuildingClasses", "Legacy_BuildingClassProductionModifiers", "BuildingClassType", "LegacyType", szLegacyType, "Modifier");
	kUtility.PopulateArrayByValue(m_piSpecialistHappinessChange, "Specialists", "Legacy_SpecialistHappinessChange", "SpecialistType", "LegacyType", szLegacyType, "HappinessTimes100");
	kUtility.PopulateArrayByValue(m_piNumFreeUnitsByType, "Units", "Legacy_NumFreeUnitsByType", "UnitType", "LegacyType", szLegacyType, "NumUnits");
	kUtility.PopulateArrayByValue(m_piNumFreeUnitsByClass, "UnitClasses", "Legacy_NumFreeUnitsByClass", "UnitClassType", "LegacyType", szLegacyType, "NumUnits");
	kUtility.PopulateArrayByValue(m_piGreatWorkClassTourismChange, "GreatWorkClasses", "Legacy_GreatWorkClassTourismChange", "GreatWorkClassType", "LegacyType", szLegacyType, "TourismChange");
	kUtility.PopulateArrayByValue(m_piBuildTimeOverride, "Builds", "Legacy_BuildTimeOverride", "BuildType", "LegacyType", szLegacyType, "TimeOverride", -1);
	// Complex/Compound Arrays
	{
		kUtility.Initialize2DArray(m_paiImprovementNearbyCombatModifierByDomain, "Improvements", "Domains");
		std::string strKey("Legacy_ImprovementNearbyCombatModifierByDomain");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Improvements.ID as ImprovementID, Domains.ID as DomainID, CombatModifier FROM Legacy_ImprovementNearbyCombatModifierByDomain "
				"INNER JOIN Improvements ON Improvements.Type = ImprovementType "
				"INNER JOIN Domains ON Domains.Type = DomainType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int improvement = pResults->GetInt(0);
			const int domain = pResults->GetInt(1);
			const int combatModifier = pResults->GetInt(2);
			m_paiImprovementNearbyCombatModifierByDomain[improvement][domain] = combatModifier;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piCityYieldChange, "Yields");
		kUtility.InitializeArray(m_piOriginalCityYieldChange, "Yields");
		kUtility.InitializeArray(m_piConqueredCityYieldChange, "Yields");
		std::string strKey("Legacy_CityYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Yields.ID as YieldID, CityYieldChange, OriginalCityYieldChange, ConqueredCityYieldChange FROM Legacy_CityYieldChanges "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int yield = pResults->GetInt(0);
			const int cityYieldChange = pResults->GetInt(1);
			const int originalCityYieldChange = pResults->GetInt(2);
			const int conqueredCityYieldChange = pResults->GetInt(3);
			m_piCityYieldChange[yield] = cityYieldChange;
			m_piOriginalCityYieldChange[yield] = originalCityYieldChange;
			m_piConqueredCityYieldChange[yield] = conqueredCityYieldChange;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiBuildingCostOverride, "Buildings", "Yields");
		std::string strKey("Legacy_BuildingCostOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Buildings.ID as BuildingID, Yields.ID as YieldID, CostOverride FROM Legacy_BuildingCostOverride "
				"INNER JOIN Buildings ON Buildings.Type = BuildingType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int building = pResults->GetInt(0);
			const int yield = pResults->GetInt(1);
			const int costOverride = pResults->GetInt(2);
			m_paiBuildingCostOverride[building][yield] = costOverride;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiUnitCostOverride, "Units", "Yields");
		std::string strKey("Legacy_UnitCostOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, Yields.ID as YieldID, CostOverride FROM Legacy_UnitCostOverride "
				"INNER JOIN Units ON Units.Type = UnitType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unit = pResults->GetInt(0);
			const int yield = pResults->GetInt(1);
			const int costOverride = pResults->GetInt(2);
			m_paiUnitCostOverride[unit][yield] = costOverride;
		}
		pResults->Reset();
	}
	{
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			m_abRevealResource.push_back(false);
		}
		std::string strKey("Legacy_RevealResource");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Resources.ID as ResourceID FROM Legacy_RevealResource "
				"INNER JOIN Resources ON Resources.Type = ResourceType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int resource = pResults->GetInt(0);
			m_abRevealResource[resource] = true;
		}
		pResults->Reset();
	}
	{
		for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
		{
			m_pbBuildingIgnorePolicyPrereq.push_back(false);
			m_pbBuildingIgnoreTechPrereq.push_back(false);
		}
		std::string strKey("Legacy_BuildingIgnorePrereqs");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Buildings.ID as BuildingID, IgnorePolicyPrereq, IgnoreTechPrereq FROM Legacy_BuildingIgnorePrereqs "
				"INNER JOIN Buildings ON Buildings.Type = BuildingType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int building = pResults->GetInt(0);
			const bool bIgnorePolicy = pResults->GetBool(1);
			const bool bIgnoreTech = pResults->GetBool(2);
			m_pbBuildingIgnorePolicyPrereq[building] = bIgnorePolicy;
			m_pbBuildingIgnoreTechPrereq[building] = bIgnoreTech;
		}
		pResults->Reset();
	}
	{
		for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
		{
			m_pbUnitIgnorePolicyPrereq.push_back(false);
			m_pbUnitIgnoreTechPrereq.push_back(false);
		}
		std::string strKey("Legacy_UnitIgnorePrereqs");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, IgnorePolicyPrereq, IgnoreTechPrereq FROM Legacy_UnitIgnorePrereqs "
				"INNER JOIN Units ON Units.Type = UnitType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unit = pResults->GetInt(0);
			const bool bIgnorePolicy = pResults->GetBool(1);
			const bool bIgnoreTech = pResults->GetBool(2);
			m_pbUnitIgnorePolicyPrereq[unit] = bIgnorePolicy;
			m_pbUnitIgnoreTechPrereq[unit] = bIgnoreTech;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piLegacyBuildingClassOverride, "BuildingClasses", -1);
		std::string strKey("Legacy_BuildingClassOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, COALESCE(Buildings.ID, -1) as BuildingID FROM Legacy_BuildingClassOverride "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"INNER JOIN Buildings ON Buildings.Type = BuildingType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int buildingClass = pResults->GetInt(0);
			const int building = pResults->GetInt(1);
			m_piLegacyBuildingClassOverride[buildingClass] = building;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piLegacyUnitClassOverride, "UnitClasses", -1);
		std::string strKey("Legacy_UnitClassOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT UnitClasses.ID as UnitClassID, COALESCE(Units.ID, -1) as UnitID FROM Legacy_UnitClassOverride "
				"INNER JOIN UnitClasses ON UnitClasses.Type = UnitClassType "
				"INNER JOIN Units ON Units.Type = UnitType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unitClass = pResults->GetInt(0);
			const int unit = pResults->GetInt(1);
			m_piLegacyUnitClassOverride[unitClass] = unit;
		}
		pResults->Reset();
	}
	// No Construct
	{
		for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
		{
			m_abNoConstructBuilding.push_back(false);
		}

		std::string strKey("Legacy_NoConstructBuilding");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Buildings.ID as BuildingID FROM Legacy_NoConstructBuilding "
				"INNER JOIN Buildings ON Buildings.Type = BuildingType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int building = pResults->GetInt(1);
			m_abNoConstructBuilding[building] = true;
		}
		pResults->Reset();
	}
	// No Train
	{
		for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
		{
			m_abNoTrainUnit.push_back(false);
		}
		std::string strKey("Legacy_NoTrainUnit");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID FROM Legacy_NoTrainUnit "
				"INNER JOIN Units ON Units.Type = UnitType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unit = pResults->GetInt(1);
			m_abNoTrainUnit[unit] = true;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piPromotionNearbyGeneralUnitCombat, "UnitCombatInfos");
		std::string strKey("Legacy_FreePromotionNearbyGeneral");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT UnitCombatInfos.ID as UnitCombatID, UnitPromotions.ID as PromotionID FROM Legacy_FreePromotionNearbyGeneral "
				"INNER JOIN UnitPromotions ON UnitPromotions.Type = PromotionType "
				"INNER JOIN UnitCombatInfos ON UnitCombatInfos.Type = UnitCombatType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iUnitCombat = pResults->GetInt(2);
			const int iPromotion = pResults->GetInt(1);
			m_piPromotionNearbyGeneralUnitCombat[iUnitCombat] = iPromotion;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiGreatWorkClassYieldChange, "GreatWorkClasses", "Yields");
		std::string strKey("Legacy_GreatWorkClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT GreatWorkClasses.ID as GreatWorkClassID, Yields.ID as YieldID, Yield FROM Legacy_GreatWorkClassYieldChange "
				"INNER JOIN GreatWorkClasses ON GreatWorkClasses.Type = GreatWorkClassType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iGreatWorkClass = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);
			m_paiGreatWorkClassYieldChange[iGreatWorkClass][iYieldID] = iYield;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiBuildingClassGreatPersonPointChange, "BuildingClasses", "Specialists");
		kUtility.Initialize2DArray(m_paiBuildingClassGreatPersonPointModifier, "BuildingClasses", "Specialists");
		std::string strKey("Legacy_BuildingClassGreatPersonPointChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, Specialists.ID as SpecialistID, RateChange, Modifier FROM Legacy_BuildingClassGreatPersonPointChange "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"INNER JOIN Specialists ON Specialists.Type = SpecialistType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iBuildingClass = pResults->GetInt(0);
			const int iSpecialist = pResults->GetInt(1);
			const int iPointsPerTurn = pResults->GetInt(2);
			const int iPointModifier = pResults->GetInt(3);
			m_paiBuildingClassGreatPersonPointChange[iBuildingClass][iSpecialist] = iPointsPerTurn;
			m_paiBuildingClassGreatPersonPointModifier[iBuildingClass][iSpecialist] = iPointModifier;
		}
		pResults->Reset();
	}
	{
		// Building Class Yields and Yield Modifiers
		kUtility.Initialize2DArray(m_paiBuildingClassYieldChange, "BuildingClasses", "Yields");
		kUtility.Initialize2DArray(m_paiBuildingClassYieldModifier, "BuildingClasses", "Yields");
		std::string strKey("Legacy_BuildingClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, Yield, Modifier FROM Legacy_BuildingClassYieldChange "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iBuildingClass = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);
			const int iYieldModifier = pResults->GetInt(3);
			m_paiBuildingClassYieldChange[iBuildingClass][iYield] = iYieldChange;
			m_paiBuildingClassYieldModifier[iBuildingClass][iYield] = iYieldModifier;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piBuildingClassHappinessChange, "BuildingClasses");
		kUtility.InitializeArray(m_piBuildingClassGlobalHappinessChange, "BuildingClasses");
		std::string strKey("Legacy_BuildingClassHappinessChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, Happiness, GlobalHappiness FROM Legacy_BuildingClassHappinessChange "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iBuildingClass = pResults->GetInt(0);
			const int iHappiness = pResults->GetInt(1);
			const int iGlobalHappiness = pResults->GetInt(2);
			m_piBuildingClassHappinessChange[iBuildingClass] = iHappiness;
			m_piBuildingClassGlobalHappinessChange[iBuildingClass] = iGlobalHappiness;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiResourceYieldChange, "Resources", "Yields");
		std::string strKey("Legacy_ResourceYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Resources.ID as ResourceID, Yields.ID as YieldID, Yield FROM Legacy_ResourceYieldChange "
				"INNER JOIN Resources ON Resources.Type = ResourceType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iResource = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);
			m_paiResourceYieldChange[iResource][iYieldID] = iYield;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiResourceClassYieldChange, "ResourceClasses", "Yields");
		std::string strKey("Legacy_ResourceClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT ResourceClasses.ID as ResourceClass, Yields.ID as YieldID, Yield FROM Legacy_ResourceClassYieldChange "
				"INNER JOIN ResourceClasses ON ResourceClasses.Type = ResourceClassType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iResourceClass = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);
			m_paiResourceClassYieldChange[iResourceClass][iYieldID] = iYield;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiSpecialistYieldChange, "Specialists", "Yields");
		std::string strKey("Legacy_SpecialistYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield FROM Legacy_SpecialistYieldChange "
				"INNER JOIN Specialists ON Specialists.Type = SpecialistType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iSpecialist = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);
			m_paiSpecialistYieldChange[iSpecialist][iYield] = iYieldChange;
		}
		pResults->Reset();
	}
	{
		std::string sqlKey = "Legacy_FreePromotionsUnitType";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if (pResults == NULL)
		{
			const char* szSQL =
				"select UnitPromotions.ID, Units.ID "
				"from Legacy_FreePromotionsUnitType, UnitPromotions, Units "
				"where LegacyType = ? and PromotionType = UnitPromotions.Type and UnitType = Units.Type";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unitPromotionID = pResults->GetInt(0);
			const int unitCombatInfoID = pResults->GetInt(1);
			m_FreePromotionUnitType.insert(std::pair<int, int>(unitPromotionID, unitCombatInfoID));
		}
		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::multimap<int, int>(m_FreePromotionUnitType).swap(m_FreePromotionUnitType);
	}
	{
		kUtility.Initialize2DArray(m_paiUnitResourceRequirementChange, "Units", "Resources");
		std::string strKey("Legacy_UnitResourceRequirementChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, Resources.ID as ResourceID, QuantityChange FROM Legacy_UnitResourceRequirementChange "
				"INNER JOIN Units ON Units.Type = UnitType "
				"INNER JOIN Resources ON Resources.Type = ResourceType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iUnit = pResults->GetInt(0);
			const int iResource = pResults->GetInt(1);
			const int iChange = pResults->GetInt(2);
			m_paiUnitResourceRequirementChange[iUnit][iResource] = iChange;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piUnitRangedStrengthChange, "Units");
		kUtility.InitializeArray(m_piUnitStrengthChange, "Units");
		std::string strKey("Legacy_UnitStrengthChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, RangedStrengthChange, StrengthChange FROM Legacy_UnitStrengthChanges "
				"INNER JOIN Units ON Units.Type = UnitType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iUnit = pResults->GetInt(0);
			const int iRangedStrengthChange = pResults->GetInt(1);
			const int iStrengthChange = pResults->GetInt(2);
			m_piUnitRangedStrengthChange[iUnit] = iRangedStrengthChange;
			m_piUnitStrengthChange[iUnit] = iStrengthChange;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiImprovementYieldChange, "Improvements", "Yields");
		kUtility.Initialize2DArray(m_paiImprovementYieldChangePerXWorldWonder, "Improvements", "Yields");
		std::string strKey("Legacy_ImprovementYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield, YieldPerXWorldWonder FROM Legacy_ImprovementYieldChanges "
				"INNER JOIN Improvements ON Improvements.Type = ImprovementType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iImprovement = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);
			const int iYieldPerXWorldWonder = pResults->GetInt(3);
			m_paiImprovementYieldChange[iImprovement][iYield] = iYieldChange;
			m_paiImprovementYieldChangePerXWorldWonder[iImprovement][iYield] = iYieldPerXWorldWonder;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piImprovementTourismBonus, "Improvements");
		std::string strKey("Legacy_ImprovementTourismBonus");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Improvements.ID as ImprovementID, Tourism FROM Legacy_ImprovementTourismBonus "
				"INNER JOIN Improvements ON Improvements.Type = ImprovementType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iImprovement = pResults->GetInt(0);
			const int iTourism = pResults->GetInt(1);
			m_piImprovementTourismBonus[iImprovement] = iTourism;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiImprovementNearbyHealChangeByDomain, "Improvements", "Domains");
		std::string strKey("Legacy_ImprovementNearbyHealChangeByDomain");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Improvements.ID as ImprovementID, Domains.ID as DomainID, HealChange FROM Legacy_ImprovementNearbyHealChangeByDomain "
				"INNER JOIN Improvements ON Improvements.Type = ImprovementType "
				"INNER JOIN Domains ON Domains.Type = DomainType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iImprovement = pResults->GetInt(0);
			const int iDomain = pResults->GetInt(1);
			const int iHealChange = pResults->GetInt(2);
			m_paiImprovementNearbyHealChangeByDomain[iImprovement][iDomain] = iHealChange;
		}
		pResults->Reset();
	}
	SetIncludesOneShotFreeUnits((LegacyTypes)GetID());
	// Return true at the very end.
	return true;
}
// What civilization is the legacy offered to?
int CvLegacyEntry::GetCivilization() const
{
	return m_iCivilization;
}
// At what era is this Legacy offered to the player?
int CvLegacyEntry::GetEraOffered() const
{
	return m_iEraOffered;
}
// Does this legacy trigger the choice instantly when entering the prereq era?
bool CvLegacyEntry::IsInstant() const
{
	return m_bInstant;
}
// Does this Legacy Include Free Units?
bool CvLegacyEntry::IncludesOneShotFreeUnits(LegacyTypes eLegacy) const
{
	CvAssertMsg(eLegacy >= 0 && eLegacy < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	return m_pbHasOneShotUnits[eLegacy];
}
// Set if this Legacy Has Free Units
void CvLegacyEntry::SetIncludesOneShotFreeUnits(LegacyTypes eLegacy)
{
	bool bHasOneShotUnits = false;
	if (m_piNumFreeUnitsByType != NULL)
	{
		for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
		{
			if (m_piNumFreeUnitsByType[iUnit] > 0)
			{
				bHasOneShotUnits = true;
				break;
			}
		}
	}
	if (!bHasOneShotUnits && m_piNumFreeUnitsByClass != NULL)
	{
		for (int iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
		{
			if (m_piNumFreeUnitsByClass[iUnitClass] > 0)
			{
				bHasOneShotUnits = true;
				break;
			}
		}
	}

	m_pbHasOneShotUnits[eLegacy] = bHasOneShotUnits;
}
// Special Stuff, doesnt need a Player Legacy object
int CvLegacyEntry::GetNumFreeUnitsByClass(int i) const
{
	return m_piNumFreeUnitsByClass ? m_piNumFreeUnitsByClass[i] : 0;
}
int CvLegacyEntry::GetNumFreeUnitsByType(int i) const
{
	return m_piNumFreeUnitsByType ? m_piNumFreeUnitsByType[i] : 0;
}
// SIMPLE GETTERS
int CvLegacyEntry::GetHappinessPerOriginalCity() const
{
	return m_iHappinessPerOriginalCity;
}
int CvLegacyEntry::GetGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}
int CvLegacyEntry::GetGreatGeneralSiegeBonus() const
{
	return m_iGreatGeneralSiegeBonus;
}
int CvLegacyEntry::GetResistanceTimeReduction() const
{
	return m_iResistanceTimeReduction;
}
int CvLegacyEntry::GetYieldModCapitalProximity() const
{
	return m_iYieldModCapitalProximity;
}
int CvLegacyEntry::GetPlotGoldCostModifier() const
{
	return m_iPlotGoldCostModifier;
}
int CvLegacyEntry::GetPlotCultureCostModifier() const
{
	return m_iPlotCultureCostModifier;
}
int CvLegacyEntry::GetHappinessFromGreatImprovements() const
{
	return m_iHappinessFromGreatImprovements;
}
int CvLegacyEntry::GetHappinessFromForeignReligiousMajority() const
{
	return m_iHappinessFromForeignReligiousMajority;
}
int CvLegacyEntry::GetVotesPerCapital() const
{
	return m_iVotesPerCapital;
}
int CvLegacyEntry::GetInfluenceChangeMajorityReligion() const
{
	return m_iInfluenceChangeMajorityReligion;
}
int CvLegacyEntry::GetInfluenceChangeTradeConnection() const
{
	return m_iInfluenceChangeTradeConnection;
}
int CvLegacyEntry::GetPurchasedUnitExtraMoves() const
{
	return m_iPurchasedUnitExtraMoves;
}
int CvLegacyEntry::GetHappinessPerTheme() const
{
	return m_iHappinessPerTheme;
}
bool CvLegacyEntry::IsTradeUnplunderable() const
{
	return m_bTradeUnplunderable;
}
bool CvLegacyEntry::IsCannotPlunder() const
{
	return m_bCannotPlunder;
}
// ARRAYS
bool CvLegacyEntry::IsRevealResource(ResourceTypes eResource) const
{
	return NO_RESOURCE != eResource ? m_abRevealResource[eResource] : false;
}
bool CvLegacyEntry::IsNoTrainUnit(UnitTypes eUnit) const
{
	return NO_UNIT != eUnit ? m_abNoTrainUnit[eUnit] : false;
}
bool CvLegacyEntry::IsNoConstructBuilding(BuildingTypes eBuilding) const
{
	return NO_BUILDING != eBuilding ? m_abNoConstructBuilding[eBuilding] : false;
}
int CvLegacyEntry::GetLegacyBuildingClassOverride(int i) const
{
	return m_piLegacyBuildingClassOverride ? m_piLegacyBuildingClassOverride[i] : -1;
}
bool CvLegacyEntry::IsBuildingIgnorePolicyPrereq(BuildingTypes eBuilding) const
{
	return NO_BUILDING != eBuilding ? m_pbBuildingIgnorePolicyPrereq[eBuilding] : false;
}
bool CvLegacyEntry::IsBuildingIgnoreTechPrereq(BuildingTypes eBuilding) const
{
	return NO_BUILDING != eBuilding ? m_pbBuildingIgnoreTechPrereq[eBuilding] : false;
}
int CvLegacyEntry::GetLegacyUnitClassOverride(int i) const
{
	return m_piLegacyUnitClassOverride ? m_piLegacyUnitClassOverride[i] : -1;
}
bool CvLegacyEntry::IsUnitIgnorePolicyPrereq(UnitTypes eUnit) const
{
	return NO_UNIT != eUnit ? m_pbUnitIgnorePolicyPrereq[eUnit] : false;
}
bool CvLegacyEntry::IsUnitIgnoreTechPrereq(UnitTypes eUnit) const
{
	return NO_UNIT != eUnit ? m_pbUnitIgnoreTechPrereq[eUnit] : false;
}
int CvLegacyEntry::IsFreePromotion(int i) const
{
	return m_pbFreePromotion ? m_pbFreePromotion[i] : -1;
}
bool CvLegacyEntry::IsFreePromotionUnitType(const int promotionID, const int unitTypeID) const
{
	std::multimap<int, int>::const_iterator it = m_FreePromotionUnitType.find(promotionID);
	if (it != m_FreePromotionUnitType.end())
	{
		// get an iterator to the element that is one past the last element associated with key
		std::multimap<int, int>::const_iterator lastElement = m_FreePromotionUnitType.upper_bound(promotionID);

		// for each element in the sequence [itr, lastElement)
		for (; it != lastElement; ++it)
		{
			if (it->second == unitTypeID)
			{
				return true;
			}
		}
	}

	return false;
}
int CvLegacyEntry::GetPlotPurchaseYieldReward(int i) const
{
	return m_piPlotPurchaseYieldReward ? m_piPlotPurchaseYieldReward[i] : 0;
}
int CvLegacyEntry::GetPromotionNearbyGeneral(int i) const
{
	return m_piPromotionNearbyGeneralUnitCombat ? m_piPromotionNearbyGeneralUnitCombat[i] : -1;
}
int CvLegacyEntry::GetCityYieldChange(int i) const
{
	return m_piCityYieldChange ? m_piCityYieldChange[i] : 0;
}
int CvLegacyEntry::GetOriginalCityYieldChange(int i) const
{
	return m_piOriginalCityYieldChange ? m_piOriginalCityYieldChange[i] : 0;
}
int CvLegacyEntry::GetConqueredCityYieldChange(int i) const
{
	return m_piConqueredCityYieldChange ? m_piConqueredCityYieldChange[i] : 0;
}
int CvLegacyEntry::GetCityYieldModifier(int i) const
{
	return m_piCityYieldModifier ? m_piCityYieldModifier[i] : 0;
}
int CvLegacyEntry::GetBuildingClassProductionModifier(int i) const
{
	return m_piBuildingClassProductionModifier ? m_piBuildingClassProductionModifier[i] : 0;
}
int CvLegacyEntry::GetBuildingClassHappinessChange(int i) const
{
	return m_piBuildingClassHappinessChange ? m_piBuildingClassHappinessChange[i] : 0;
}
int CvLegacyEntry::GetBuildingClassGlobalHappinessChange(int i) const
{
	return m_piBuildingClassGlobalHappinessChange ? m_piBuildingClassGlobalHappinessChange[i] : 0;
}
int CvLegacyEntry::GetUnitResourceRequirementChange(int i, int j) const
{
	return m_paiUnitResourceRequirementChange ? m_paiUnitResourceRequirementChange[i][j] : 0;
}
int CvLegacyEntry::GetUnitRangedStrengthChange(int i) const
{
	return m_piUnitRangedStrengthChange ? m_piUnitRangedStrengthChange[i] : 0;
}
int CvLegacyEntry::GetUnitStrengthChange(int i) const
{
	return m_piUnitStrengthChange ? m_piUnitStrengthChange[i] : 0;
}
int CvLegacyEntry::GetBuildingClassYieldChange(int i, int j) const
{
	return m_paiBuildingClassYieldChange ? m_paiBuildingClassYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetBuildingClassYieldModifier(int i, int j) const
{
	return m_paiBuildingClassYieldModifier ? m_paiBuildingClassYieldModifier[i][j] : 0;
}
int CvLegacyEntry::GetResourceYieldChange(int i, int j) const
{
	return m_paiResourceYieldChange ? m_paiResourceYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetResourceClassYieldChange(int i, int j) const
{
	return m_paiResourceClassYieldChange ? m_paiResourceClassYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetSpecialistYieldChange(int i, int j) const
{
	return m_paiSpecialistYieldChange ? m_paiSpecialistYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetSpecialistHappinessChange(int i) const
{
	return m_piSpecialistHappinessChange ? m_piSpecialistHappinessChange[i] : 0;
}
int CvLegacyEntry::GetImprovementYieldChange(int i, int j) const
{
	return m_paiImprovementYieldChange ? m_paiImprovementYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetImprovementYieldChangePerXWorldWonder(int i, int j) const
{
	return m_paiImprovementYieldChangePerXWorldWonder ? m_paiImprovementYieldChangePerXWorldWonder[i][j] : 0;
}
int CvLegacyEntry::GetImprovementNearbyHealChangeByDomain(int i, int j) const
{
	return m_paiImprovementNearbyHealChangeByDomain ? m_paiImprovementNearbyHealChangeByDomain[i][j] : 0;
}
int CvLegacyEntry::GetImprovementNearbyCombatModifierByDomain(int i, int j) const
{
	return m_paiImprovementNearbyCombatModifierByDomain ? m_paiImprovementNearbyCombatModifierByDomain[i][j] : 0;
}
int CvLegacyEntry::GetBuildingClassGreatPersonPointModifier(int i, int j) const
{
	return m_paiBuildingClassGreatPersonPointModifier ? m_paiBuildingClassGreatPersonPointModifier[i][j] : 0;
}
int CvLegacyEntry::GetBuildingClassGreatPersonPointChange(int i, int j) const
{
	return m_paiBuildingClassGreatPersonPointChange ? m_paiBuildingClassGreatPersonPointChange[i][j] : 0;
}
int CvLegacyEntry::GetGreatWorkClassYieldChange(int i, int j) const
{
	return m_paiGreatWorkClassYieldChange ? m_paiGreatWorkClassYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetGreatWorkClassTourismChange(int i) const
{
	return m_piGreatWorkClassTourismChange ? m_piGreatWorkClassTourismChange[i] : 0;
}
int CvLegacyEntry::GetImprovementTourismBonus(int i) const
{
	return m_piImprovementTourismBonus ? m_piImprovementTourismBonus[i] : 0;
}
int CvLegacyEntry::GetBuildingCostOverride(int i, int j) const
{
	return m_paiBuildingCostOverride ? m_paiBuildingCostOverride[i][j] : -1;
}
int CvLegacyEntry::GetUnitCostOverride(int i, int j) const
{
	return m_paiUnitCostOverride ? m_paiUnitCostOverride[i][j] : -1;
}
int CvLegacyEntry::GetBuildTimeOverride(int i) const
{
	return m_piBuildTimeOverride ? m_piBuildTimeOverride[i] : -1;
}
int CvLegacyEntry::GetYieldBonusFromThemes(int i) const
{
	return m_piYieldBonusFromThemes ? m_piYieldBonusFromThemes[i] : 0;
}
//=====================================
// CvLegacyXMLEntries
//=====================================
/// Constructor
CvLegacyXMLEntries::CvLegacyXMLEntries(void)
{
}

/// Destructor
CvLegacyXMLEntries::~CvLegacyXMLEntries(void)
{
	DeleteLegacyArray();
}

// Get the vector of legacy entries
std::vector<CvLegacyEntry*>& CvLegacyXMLEntries::GetLegacyEntries()
{
	return m_paLegacyEntries;
}

// Get the number of legacies
int CvLegacyXMLEntries::GetNumLegacies()
{
	return m_paLegacyEntries.size();
}

void CvLegacyXMLEntries::DeleteLegacyArray()
{
	for (std::vector<CvLegacyEntry*>::iterator it = m_paLegacyEntries.begin(); it != m_paLegacyEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paLegacyEntries.clear();
}

CvLegacyEntry* CvLegacyXMLEntries::GetLegacyEntry(int iIndex)
{
	return m_paLegacyEntries[iIndex];
}

//=====================================
// CvPlayerLegacies
//=====================================
/// Constructor
CvPlayerLegacies::CvPlayerLegacies(void):
	m_pabHasLegacy(NULL),
	m_pabOneShotFreeUnitsFired(NULL),
	m_pLegacies(NULL),
	m_pLegacyAI(NULL),
	m_pPlayer(NULL)
{
}
// Destructor
CvPlayerLegacies::~CvPlayerLegacies(void)
{
}
// Initialization
void CvPlayerLegacies::Init(CvLegacyXMLEntries* pLegacies, CvPlayer* pPlayer)
{
	// Store Pointers.
	m_pLegacies = pLegacies;
	m_pPlayer = pPlayer;

	// Allocate memory.
	m_pabHasLegacy = FNEW(bool[m_pLegacies->GetNumLegacies()], c_eCiv5GameplayDLL, 0);
	m_pabOneShotFreeUnitsFired = FNEW(bool[m_pLegacies->GetNumLegacies()], c_eCiv5GameplayDLL, 0);
	m_pLegacyAI = FNEW(CvLegacyAI(this), c_eCiv5GameplayDLL, 0);

	Reset();

	m_vCivHasLegacy.resize(GC.getNumLegacyInfos(), false);
}
// Uninitialization
void CvPlayerLegacies::Uninit()
{
	SAFE_DELETE_ARRAY(m_pabHasLegacy);
	SAFE_DELETE_ARRAY(m_pabOneShotFreeUnitsFired);
	SAFE_DELETE(m_pLegacyAI);
	m_vbRevealResource.clear();
	m_viLegacyBuildingClassOverrides.clear();
	m_vbBuildingIgnorePolicyPrereq.clear();
	m_vbBuildingIgnoreTechPrereq.clear();
	m_viLegacyUnitClassOverrides.clear();
	m_vbUnitIgnorePolicyPrereq.clear();
	m_vbUnitIgnoreTechPrereq.clear();
	m_vbNoConstruct.clear();
	m_vbNoTrain.clear();
	m_viPlotPurchaseYieldReward.clear();
	m_viCityYieldChange.clear();
	m_viOriginalCityYieldChange.clear();
	m_viConqueredCityYieldChange.clear();
	m_viCityYieldModifier.clear();
	m_viUnitRangedStrengthChanges.clear();
	m_viUnitStrengthChanges.clear();
	m_viBuildingClassProductionModifiers.clear();
	m_viBuildingClassHappinessChanges.clear();
	m_viBuildingClassGlobalHappinessChanges.clear();
	m_viPromotionNearbyGeneralUnitCombat.clear();
	m_vaaiBuildingClassYieldChanges.clear();
	m_vaaiBuildingClassYieldModifiers.clear();
	m_vaaiResourceYieldChanges.clear();
	m_vaaiResourceClassYieldChanges.clear();
	m_vaaiSpecialistYieldChanges.clear();
	m_viSpecialistHappinessChanges.clear();
	m_vaaiImprovementYieldChanges.clear();
	m_vaaiImprovementYieldChangePerXWorldWonder.clear();
	m_vaaiNearbyImprovementHealChangeByDomain.clear();
	m_vaaiNearbyImprovementCombatModifierByDomain.clear();
	m_vaaiGreatWorkClassYieldChanges.clear();
	m_viGreatWorkClassTourismChanges.clear();
	m_viImprovementTourismBonuses.clear();
	m_vaaiBuildingCostOverrides.clear();
	m_vaaiUnitCostOverrides.clear();
	m_viBuildTimeOverrides.clear();
	m_viYieldBonusFromThemes.clear();
}
// Reset
void CvPlayerLegacies::Reset()
{
	m_vCivHasLegacy.resize(GC.getNumLegacyInfos(), false);
	m_vPotentiallyActiveCivLegacies.clear();

	for (int i = 0; i < m_pLegacies->GetNumLegacies(); i++)
	{
		m_pabHasLegacy[i] = false;
		m_pabOneShotFreeUnitsFired[i] = false;
	}
	// Gameplay Effects
	m_iHappinessPerOriginalCity = 0;
	m_iGoldenAgeTurns = 0;
	m_iGreatGeneralSiegeBonus = 0;
	m_iResistanceTimeReduction = 0;
	m_iYieldModCapitalProximity = 0;
	m_iPlotGoldCostModifier = 0;
	m_iPlotCultureCostModifier = 0;
	m_iHappinessFromGreatImprovements = 0;
	m_iHappinessFromForeignReligiousMajority = 0;
	m_iVotesPerCapital = 0;
	m_iInfluenceChangeMajorityReligion = 0;
	m_iInfluenceChangeTradeConnection = 0;
	m_iPurchasedUnitExtraMoves = 0;
	m_iHappinessPerTheme = 0;
	m_bCannotPlunder = false;
	m_bTradeUnplunderable = false;
	
	// Arrays
	// Builds
	m_viBuildTimeOverrides.clear();
	m_viBuildTimeOverrides.resize(GC.getNumBuildInfos());
	for (int iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
	{
		m_viBuildTimeOverrides[iBuild] = -1;
	}
	// Resources
	m_vbRevealResource.clear();
	m_vbRevealResource.resize(GC.getNumResourceInfos());
	for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
	{
		m_vbRevealResource[iResource] = false;
	}
	// Great Work Classes
	m_viGreatWorkClassTourismChanges.clear();
	m_viGreatWorkClassTourismChanges.resize(GC.getNumGreatWorkClassInfos());
	for (int iGreatWorkClass = 0; iGreatWorkClass < GC.getNumGreatWorkClassInfos(); iGreatWorkClass++)
	{
		m_viGreatWorkClassTourismChanges[iGreatWorkClass] = 0;
	}
	// Improvements, 1d
	m_viImprovementTourismBonuses.clear();
	m_viImprovementTourismBonuses.resize(GC.getNumImprovementInfos());
	for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
	{
		m_viImprovementTourismBonuses[iImprovement] = 0;
	}
	// Units
	m_viUnitStrengthChanges.clear();
	m_viUnitStrengthChanges.resize(GC.getNumUnitInfos());
	m_viUnitRangedStrengthChanges.clear();
	m_viUnitRangedStrengthChanges.resize(GC.getNumUnitInfos());
	m_vbNoTrain.clear();
	m_vbNoTrain.resize(GC.getNumUnitInfos());
	m_vbUnitIgnorePolicyPrereq.clear();
	m_vbUnitIgnorePolicyPrereq.resize(GC.getNumUnitInfos());
	m_vbUnitIgnoreTechPrereq.clear();
	m_vbUnitIgnoreTechPrereq.resize(GC.getNumUnitInfos());
	for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
	{
		m_viUnitStrengthChanges[iUnit] = 0;
		m_viUnitRangedStrengthChanges[iUnit] = 0;
		m_vbNoTrain[iUnit] = false;
		m_vbUnitIgnorePolicyPrereq[iUnit] = false;
		m_vbUnitIgnoreTechPrereq[iUnit] = false;
	}
	// UnitClass
	m_viLegacyUnitClassOverrides.clear();
	m_viLegacyUnitClassOverrides.resize(GC.getNumUnitClassInfos());
	for (int iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
	{
		m_viLegacyUnitClassOverrides[iUnitClass] = NO_UNIT;
	}
	// Buildings
	m_vbNoConstruct.clear();
	m_vbNoConstruct.resize(GC.getNumBuildingInfos());
	m_vbBuildingIgnorePolicyPrereq.clear();
	m_vbBuildingIgnorePolicyPrereq.resize(GC.getNumBuildingInfos());
	m_vbBuildingIgnoreTechPrereq.clear();
	m_vbBuildingIgnoreTechPrereq.resize(GC.getNumBuildingInfos());
	for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
	{
		m_vbNoConstruct[iBuilding] = false;
		m_vbBuildingIgnorePolicyPrereq[iBuilding] = false;
		m_vbBuildingIgnoreTechPrereq[iBuilding] = false;
	}
	// BuildingClass
	m_viBuildingClassProductionModifiers.clear();
	m_viBuildingClassProductionModifiers.resize(GC.getNumBuildingClassInfos());
	m_viBuildingClassHappinessChanges.clear();
	m_viBuildingClassHappinessChanges.resize(GC.getNumBuildingClassInfos());
	m_viBuildingClassGlobalHappinessChanges.clear();
	m_viBuildingClassGlobalHappinessChanges.resize(GC.getNumBuildingClassInfos());
	m_viLegacyBuildingClassOverrides.clear();
	m_viLegacyBuildingClassOverrides.resize(GC.getNumBuildingClassInfos());
	for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
	{
		m_viBuildingClassProductionModifiers[iBuildingClass] = 0;
		m_viBuildingClassHappinessChanges[iBuildingClass] = 0;
		m_viBuildingClassGlobalHappinessChanges[iBuildingClass] = 0;
		m_viLegacyBuildingClassOverrides[iBuildingClass] = NO_BUILDING;
	}
	// Specialists
	m_viSpecialistHappinessChanges.clear();
	m_viSpecialistHappinessChanges.resize(GC.getNumSpecialistInfos());
	for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
	{
		m_viSpecialistHappinessChanges[iSpecialist] = 0;
	}
	m_viPromotionNearbyGeneralUnitCombat.clear();
	m_viPromotionNearbyGeneralUnitCombat.resize(GC.getNumUnitCombatClassInfos());
	for (int iUnitCombat = 0; iUnitCombat < GC.getNumUnitCombatClassInfos(); iUnitCombat++)
	{
		m_viPromotionNearbyGeneralUnitCombat[iUnitCombat] = NO_PROMOTION;
	}
	// START YIELD RESETS
	m_viPlotPurchaseYieldReward.clear();
	m_viPlotPurchaseYieldReward.resize(NUM_YIELD_TYPES);
	m_viCityYieldChange.clear();
	m_viCityYieldChange.resize(NUM_YIELD_TYPES);
	m_viOriginalCityYieldChange.clear();
	m_viOriginalCityYieldChange.resize(NUM_YIELD_TYPES);
	m_viConqueredCityYieldChange.clear();
	m_viConqueredCityYieldChange.resize(NUM_YIELD_TYPES);
	m_viCityYieldModifier.clear();
	m_viCityYieldModifier.resize(NUM_YIELD_TYPES);
	m_vaaiBuildingClassYieldChanges.clear();
	m_vaaiBuildingClassYieldChanges.resize(GC.getNumBuildingClassInfos());
	m_vaaiBuildingClassYieldModifiers.clear();
	m_vaaiBuildingClassYieldModifiers.resize(GC.getNumBuildingClassInfos());
	m_vaaiResourceYieldChanges.clear();
	m_vaaiResourceYieldChanges.resize(GC.getNumResourceInfos());
	m_vaaiResourceClassYieldChanges.clear();
	m_vaaiResourceClassYieldChanges.resize(GC.getNumResourceClassInfos());
	m_vaaiSpecialistYieldChanges.clear();
	m_vaaiSpecialistYieldChanges.resize(GC.getNumSpecialistInfos());
	m_vaaiImprovementYieldChanges.clear();
	m_vaaiImprovementYieldChanges.resize(GC.getNumImprovementInfos());
	m_vaaiImprovementYieldChangePerXWorldWonder.clear();
	m_vaaiImprovementYieldChangePerXWorldWonder.resize(GC.getNumImprovementInfos());
	m_vaaiGreatWorkClassYieldChanges.clear();
	m_vaaiGreatWorkClassYieldChanges.resize(GC.getNumGreatWorkClassInfos());
	m_vaaiBuildingCostOverrides.clear();
	m_vaaiBuildingCostOverrides.resize(GC.getNumBuildingInfos());
	m_vaaiUnitCostOverrides.clear();
	m_vaaiUnitCostOverrides.resize(GC.getNumUnitInfos());
	m_viYieldBonusFromThemes.clear();
	m_viYieldBonusFromThemes.resize(NUM_YIELD_TYPES);

	Firaxis::Array< int, NUM_YIELD_TYPES > yield;
	for (unsigned int j = 0; j < NUM_YIELD_TYPES; ++j)
	{
		yield[j] = 0;
	}
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		m_viPlotPurchaseYieldReward[iYield] = 0;
		m_viCityYieldChange[iYield] = 0;
		m_viOriginalCityYieldChange[iYield] = 0;
		m_viConqueredCityYieldChange[iYield] = 0;
		m_viCityYieldModifier[iYield] = 0;
		m_viYieldBonusFromThemes[iYield] = 0;
		
		// BuildingClass
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
		{
			m_vaaiBuildingClassYieldChanges[iBuildingClass] = yield;
			m_vaaiBuildingClassYieldModifiers[iBuildingClass] = yield;
		}
		// Specialist
		for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			m_vaaiSpecialistYieldChanges[iSpecialist] = yield;
		}
		// Resource
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			m_vaaiResourceYieldChanges[iResource] = yield;
		}
		// ResourceClass
		for (int iResourceClass = 0; iResourceClass < GC.getNumResourceClassInfos(); iResourceClass++)
		{
			m_vaaiResourceClassYieldChanges[iResourceClass] = yield;
		}
		// Improvement
		for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			m_vaaiImprovementYieldChanges[iImprovement] = yield;
			m_vaaiImprovementYieldChangePerXWorldWonder[iImprovement] = yield;
		}
		// Great Work Class
		for (int iGreatWorkClass = 0; iGreatWorkClass < GC.getNumGreatWorkClassInfos(); iGreatWorkClass++)
		{
			m_vaaiGreatWorkClassYieldChanges[iGreatWorkClass] = yield;
		}
		// Buildings
		for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
		{
			m_vaaiBuildingCostOverrides[iBuilding] = yield;
		}
		// Units
		for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
		{
			m_vaaiUnitCostOverrides[iUnit] = yield;
		}
	} // END NUM_YIELD_TYPES LOOP
	m_vaaiNearbyImprovementHealChangeByDomain.clear();
	m_vaaiNearbyImprovementHealChangeByDomain.resize(GC.getNumImprovementInfos());
	m_vaaiNearbyImprovementCombatModifierByDomain.clear();
	m_vaaiNearbyImprovementCombatModifierByDomain.resize(GC.getNumImprovementInfos());
	Firaxis::Array< int, NUM_DOMAIN_TYPES > domain;
	for (unsigned int j = 0; j < NUM_DOMAIN_TYPES; ++j)
	{
		domain[j] = 0;
	}
	for (int iDomain = 0; iDomain < NUM_DOMAIN_TYPES; iDomain++)
	{
		// Improvement
		for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			m_vaaiNearbyImprovementHealChangeByDomain[iImprovement] = domain;
			m_vaaiNearbyImprovementCombatModifierByDomain[iImprovement] = domain;
		}
	}
	// Reset the AI
	m_pLegacyAI->Reset();
}
// Read
void CvPlayerLegacies::Read(FDataStream& kStream)
{
	// Read the data from the stream
	CvAssertMsg(m_pLegacies != NULL && GC.getNumLegacyInfos() > 0, "Number of legacies to serialize is expected to greater than 0");
	uint uiLegacyCount = 0;
	if (m_pLegacies)
	{
		uiLegacyCount = m_pLegacies->GetNumLegacies();
	}
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_pabHasLegacy, uiLegacyCount);
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_pabOneShotFreeUnitsFired, uiLegacyCount);
	kStream >> m_iHappinessPerOriginalCity;
	kStream >> m_iGoldenAgeTurns;
	kStream >> m_iGreatGeneralSiegeBonus;
	kStream >> m_iResistanceTimeReduction;
	kStream >> m_iYieldModCapitalProximity;
	kStream >> m_iPlotGoldCostModifier;
	kStream >> m_iPlotCultureCostModifier;
	kStream >> m_iHappinessFromGreatImprovements;
	kStream >> m_iHappinessFromForeignReligiousMajority;
	kStream >> m_iVotesPerCapital;
	kStream >> m_iInfluenceChangeMajorityReligion;
	kStream >> m_iInfluenceChangeTradeConnection;
	kStream >> m_iPurchasedUnitExtraMoves;
	kStream >> m_iHappinessPerTheme;
	kStream >> m_bCannotPlunder;
	kStream >> m_bTradeUnplunderable;

	kStream >> m_viYieldBonusFromThemes;
	kStream >> m_viLegacyUnitClassOverrides;
	kStream >> m_viLegacyBuildingClassOverrides;
	kStream >> m_viPlotPurchaseYieldReward;
	kStream >> m_viCityYieldChange;
	kStream >> m_viOriginalCityYieldChange;
	kStream >> m_viConqueredCityYieldChange;
	kStream >> m_viCityYieldModifier;
	kStream >> m_viUnitRangedStrengthChanges;
	kStream >> m_viUnitStrengthChanges;
	kStream >> m_viBuildingClassProductionModifiers;
	kStream >> m_viBuildingClassHappinessChanges;
	kStream >> m_viBuildingClassGlobalHappinessChanges;
	kStream >> m_viPromotionNearbyGeneralUnitCombat;
	kStream >> m_vaaiBuildingClassYieldChanges;
	kStream >> m_vaaiBuildingClassYieldModifiers;
	kStream >> m_vaaiResourceYieldChanges;
	kStream >> m_vaaiResourceClassYieldChanges;
	kStream >> m_vaaiSpecialistYieldChanges;
	kStream >> m_viSpecialistHappinessChanges;
	kStream >> m_vaaiImprovementYieldChanges;
	kStream >> m_vaaiImprovementYieldChangePerXWorldWonder;
	kStream >> m_vaaiNearbyImprovementHealChangeByDomain;
	kStream >> m_vaaiNearbyImprovementCombatModifierByDomain;
	kStream >> m_vaaiGreatWorkClassYieldChanges;
	kStream >> m_viGreatWorkClassTourismChanges;
	kStream >> m_viImprovementTourismBonuses;
	kStream >> m_vaaiBuildingCostOverrides;
	kStream >> m_vaaiUnitCostOverrides;
	kStream >> m_viBuildTimeOverrides;
	// Boolean Arrays
	int iNumEntries;
	kStream >> iNumEntries;
	m_vbNoTrain.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbNoTrain.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbNoConstruct.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbNoConstruct.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbUnitIgnorePolicyPrereq.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbUnitIgnorePolicyPrereq.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbUnitIgnoreTechPrereq.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbUnitIgnoreTechPrereq.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbBuildingIgnorePolicyPrereq.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbBuildingIgnorePolicyPrereq.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbBuildingIgnoreTechPrereq.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbBuildingIgnoreTechPrereq.push_back(bValue);
	}
	kStream >> iNumEntries;
	m_vbRevealResource.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_vbRevealResource.push_back(bValue);
	}

	m_pLegacyAI->Read(kStream);
}
// Write
void CvPlayerLegacies::Write(FDataStream& kStream) const
{
	// Write the data to the stream
	CvAssertMsg(m_pLegacies != NULL && GC.getNumLegacyInfos() > 0, "Number of legacies to serialize is expected to greater than 0");
	uint uiLegacyCount = 0;
	if (m_pLegacies)
	{
		uiLegacyCount = m_pLegacies->GetNumLegacies();
	}
	CvInfosSerializationHelper::WriteHashedDataArray<LegacyTypes>(kStream, m_pabHasLegacy, uiLegacyCount);
	CvInfosSerializationHelper::WriteHashedDataArray<LegacyTypes>(kStream, m_pabOneShotFreeUnitsFired, uiLegacyCount);
	kStream << m_iHappinessPerOriginalCity;
	kStream << m_iGoldenAgeTurns;
	kStream << m_iGreatGeneralSiegeBonus;
	kStream << m_iResistanceTimeReduction;
	kStream << m_iYieldModCapitalProximity;
	kStream << m_iPlotGoldCostModifier;
	kStream << m_iPlotCultureCostModifier;
	kStream << m_iHappinessFromGreatImprovements;
	kStream << m_iHappinessFromForeignReligiousMajority;
	kStream << m_iVotesPerCapital;
	kStream << m_iInfluenceChangeMajorityReligion;
	kStream << m_iInfluenceChangeTradeConnection;
	kStream << m_iPurchasedUnitExtraMoves;
	kStream << m_iHappinessPerTheme;
	kStream << m_bCannotPlunder;
	kStream << m_bTradeUnplunderable;

	kStream << m_viYieldBonusFromThemes;
	kStream << m_viLegacyUnitClassOverrides;
	kStream << m_viLegacyBuildingClassOverrides;
	kStream << m_viPlotPurchaseYieldReward;
	kStream << m_viCityYieldChange;
	kStream << m_viOriginalCityYieldChange;
	kStream << m_viConqueredCityYieldChange;
	kStream << m_viCityYieldModifier;
	kStream << m_viUnitRangedStrengthChanges;
	kStream << m_viUnitStrengthChanges;
	kStream << m_viBuildingClassProductionModifiers;
	kStream << m_viBuildingClassHappinessChanges;
	kStream << m_viBuildingClassGlobalHappinessChanges;
	kStream << m_viPromotionNearbyGeneralUnitCombat;
	kStream << m_vaaiBuildingClassYieldChanges;
	kStream << m_vaaiBuildingClassYieldModifiers;
	kStream << m_vaaiResourceYieldChanges;
	kStream << m_vaaiResourceClassYieldChanges;
	kStream << m_vaaiSpecialistYieldChanges;
	kStream << m_viSpecialistHappinessChanges;
	kStream << m_vaaiImprovementYieldChanges;
	kStream << m_vaaiImprovementYieldChangePerXWorldWonder;
	kStream << m_vaaiNearbyImprovementHealChangeByDomain;
	kStream << m_vaaiNearbyImprovementCombatModifierByDomain;
	kStream << m_vaaiGreatWorkClassYieldChanges;
	kStream << m_viGreatWorkClassTourismChanges;
	kStream << m_viImprovementTourismBonuses;
	kStream << m_vaaiBuildingCostOverrides;
	kStream << m_vaaiUnitCostOverrides;
	kStream << m_viBuildTimeOverrides;
	// Boolean Arrays
	kStream << m_vbNoTrain.size();
	for (uint ui = 0; ui < m_vbNoTrain.size(); ui++)
	{
		kStream << m_vbNoTrain[ui];
	}
	kStream << m_vbNoConstruct.size();
	for (uint ui = 0; ui < m_vbNoConstruct.size(); ui++)
	{
		kStream << m_vbNoConstruct[ui];
	}
	kStream << m_vbUnitIgnorePolicyPrereq.size();
	for (uint ui = 0; ui < m_vbUnitIgnorePolicyPrereq.size(); ui++)
	{
		kStream << m_vbUnitIgnorePolicyPrereq[ui];
	}
	kStream << m_vbUnitIgnoreTechPrereq.size();
	for (uint ui = 0; ui < m_vbUnitIgnoreTechPrereq.size(); ui++)
	{
		kStream << m_vbUnitIgnoreTechPrereq[ui];
	}
	kStream << m_vbBuildingIgnorePolicyPrereq.size();
	for (uint ui = 0; ui < m_vbBuildingIgnorePolicyPrereq.size(); ui++)
	{
		kStream << m_vbBuildingIgnorePolicyPrereq[ui];
	}
	kStream << m_vbBuildingIgnoreTechPrereq.size();
	for (uint ui = 0; ui < m_vbBuildingIgnoreTechPrereq.size(); ui++)
	{
		kStream << m_vbBuildingIgnoreTechPrereq[ui];
	}
	kStream << m_vbRevealResource.size();
	for (uint ui = 0; ui < m_vbRevealResource.size(); ui++)
	{
		kStream << m_vbRevealResource[ui];
	}

	m_pLegacyAI->Write(kStream);
}
// Accessors
// Get the player
CvPlayer* CvPlayerLegacies::GetPlayer() const
{
	return m_pPlayer;
}
// Check if the player has a specific legacy
bool CvPlayerLegacies::HasLegacy(LegacyTypes eIndex) const
{
	CvAssertMsg(eIndex >= 0 && eIndex < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	return m_pabHasLegacy[eIndex];
}
// Set a legacy for the player
void CvPlayerLegacies::SetLegacy(LegacyTypes eIndex, bool bNewValue)
{
	CvAssertMsg(eIndex >= 0 && eIndex < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(eIndex);
	if (pkLegacy == NULL)
		return;

	if (HasLegacy(eIndex) != bNewValue)
	{
		m_pabHasLegacy[eIndex] = bNewValue;
		updatePlayerLegacies(eIndex); // Loads the legacy effects into the player legacy object
	}
}
// Get Legacy data from the XML
CvLegacyXMLEntries* CvPlayerLegacies::GetLegacies() const
{
	return m_pLegacies;
}
// Get the Legacies for this civ that have not been chosen.
std::vector<LegacyTypes> CvPlayerLegacies::GetCivLegacies() const
{
	std::vector<LegacyTypes> result;
	for (int iLegacy = 0; iLegacy < GC.getNumLegacyInfos(); iLegacy++)
	{
		CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(iLegacy);
		if (pkLegacy && pkLegacy->GetCivilization() == m_pPlayer->getCivilizationType())
		{
			result.push_back((LegacyTypes)iLegacy);
		}
	}
	return result;
}
std::vector<LegacyTypes> CvPlayerLegacies::GetEraLegacies(EraTypes eEra) const
{
	std::vector<LegacyTypes> result;
	const std::vector<LegacyTypes>& civLegacies = GetCivLegacies();
	for (size_t i = 0; i < civLegacies.size(); i++)
	{
		CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(civLegacies[i]);
		if (pkLegacy && pkLegacy->GetEraOffered() == eEra)
		{
			result.push_back(civLegacies[i]);
		}
	}
	return result;
}
// Check if the player can choose a specific legacy
bool CvPlayerLegacies::CanChooseLegacy(LegacyTypes eLegacy) const
{
	CvAssertMsg(eLegacy >= 0, "eIndex is expected to be non-negative (invalid Index)");
	CvAssertMsg(eLegacy < GC.getNumLegacyInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	CvLegacyEntry* pkLegacyEntry = GC.getLegacyInfo(eLegacy);
	if (pkLegacyEntry == NULL)
		return false;

	// Already has Legacy?
	if (HasLegacy(eLegacy))
	{
		return false;
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(m_pPlayer->GetID());
		args->Push(eLegacy);

		// Attempt to execute the game events.
		// Will return false if there are no registered listeners.
		bool bResult = false;
		if (LuaSupport::CallTestAll(pkScriptSystem, "PlayerCanChooseLegacy", args.get(), bResult))
		{
			// Check the result.
			if (bResult == false)
			{
				return false;
			}
		}
	}

	return true;
}
// Give the player a notification to choose a legacy
void CvPlayerLegacies::testLegacyNotification(EraTypes eEra) const
{
	if (!m_pPlayer->HasLegacyThisEra(eEra) && GetCivLegacies().size() > 0) // Has not chosen a legacy this era, and has any legacies available
	{
		if (GetEraLegacies(eEra).size() > 0) // No legacies available this era for this civ
		{
			CvNotifications* pNotifications = m_pPlayer->GetNotifications();
			if (pNotifications)
			{
				CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_CHOOSE_LEGACY", GC.getEraInfo(m_pPlayer->GetCurrentEra())->GetTextKey());
				CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_CHOOSE_LEGACY_SUMMARY");
				pNotifications->Add((NotificationTypes)NOTIFICATION_CHOOSE_LEGACY, strBuffer, strSummary, -1, -1, m_pPlayer->GetID(), eEra);
			}
		}
	}
}
// Has this Legacy already given its free units?
bool CvPlayerLegacies::HasOneShotFreeUnitsFired(LegacyTypes eLegacy) const
{
	CvAssertMsg(eLegacy >= 0 && eLegacy < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	return m_pabOneShotFreeUnitsFired[eLegacy];
}
// Set that this Legacy has given its free units
void CvPlayerLegacies::SetOneShotFreeUnitsFired(LegacyTypes eLegacy, bool bFired)
{
	CvAssertMsg(eLegacy >= 0 && eLegacy < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	m_pabOneShotFreeUnitsFired[eLegacy] = bFired;
}
// Load Legacy Entry info into the PlayerLegacies object for access
void CvPlayerLegacies::updatePlayerLegacies(LegacyTypes eLegacy)
{
	CvLegacyEntry* pkLegacyEntry = GC.getLegacyInfo(eLegacy);
	if (pkLegacyEntry == NULL)
		return;
	int iChange = 0;
	bool bTemp = false;
	const CvLegacyEntry& kLegacy = (*pkLegacyEntry);
	m_iHappinessPerOriginalCity += kLegacy.GetHappinessPerOriginalCity();
	m_iGoldenAgeTurns += kLegacy.GetGoldenAgeTurns();
	m_iGreatGeneralSiegeBonus += kLegacy.GetGreatGeneralSiegeBonus();
	m_iResistanceTimeReduction += kLegacy.GetResistanceTimeReduction();
	m_iYieldModCapitalProximity += kLegacy.GetYieldModCapitalProximity();
	m_iPlotGoldCostModifier += kLegacy.GetPlotGoldCostModifier();
	m_iPlotCultureCostModifier += kLegacy.GetPlotCultureCostModifier();
	m_iHappinessFromGreatImprovements += kLegacy.GetHappinessFromGreatImprovements();
	m_iHappinessFromForeignReligiousMajority += kLegacy.GetHappinessFromForeignReligiousMajority();
	m_iVotesPerCapital += kLegacy.GetVotesPerCapital();
	m_iInfluenceChangeMajorityReligion += kLegacy.GetInfluenceChangeMajorityReligion();
	m_iInfluenceChangeTradeConnection += kLegacy.GetInfluenceChangeTradeConnection();
	m_iPurchasedUnitExtraMoves += kLegacy.GetPurchasedUnitExtraMoves();
	m_iHappinessPerTheme += kLegacy.GetHappinessPerTheme();
	bTemp = kLegacy.IsCannotPlunder();
	if (bTemp) m_bCannotPlunder = bTemp;
	bTemp = kLegacy.IsTradeUnplunderable();
	if (bTemp) m_bTradeUnplunderable = bTemp;
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		iChange = kLegacy.GetYieldBonusFromThemes((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viYieldBonusFromThemes[iYield] += iChange;
		}
		iChange = kLegacy.GetPlotPurchaseYieldReward((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viPlotPurchaseYieldReward[iYield] += iChange;
		}
		iChange = kLegacy.GetCityYieldChange((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viCityYieldChange[iYield] += iChange;
		}
		iChange = kLegacy.GetOriginalCityYieldChange((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viOriginalCityYieldChange[iYield] += iChange;
		}
		iChange = kLegacy.GetConqueredCityYieldChange((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viConqueredCityYieldChange[iYield] += iChange;
		}
		iChange = kLegacy.GetCityYieldModifier((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viCityYieldModifier[iYield] += iChange;
		}
		// BuildingClass
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
		{
			iChange = kLegacy.GetBuildingClassYieldChange((BuildingClassTypes)iBuildingClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiBuildingClassYieldChanges[iBuildingClass][iYield] += iChange;
			}
			iChange = kLegacy.GetBuildingClassYieldModifier((BuildingClassTypes)iBuildingClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiBuildingClassYieldModifiers[iBuildingClass][iYield] += iChange;
			}
		}
		// Specialist
		for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			iChange = kLegacy.GetSpecialistYieldChange((SpecialistTypes)iSpecialist, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiSpecialistYieldChanges[iSpecialist][iYield] += iChange;
			}
		}
		// Resource
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			iChange = kLegacy.GetResourceYieldChange((ResourceTypes)iResource, (YieldTypes)iYield);
			if(iChange != 0)
			{
				m_vaaiResourceYieldChanges[iResource][iYield] += iChange;
			}
		}
		// ResourceClass
		for (int iResourceClass = 0; iResourceClass < GC.getNumResourceClassInfos(); iResourceClass++)
		{
			iChange = kLegacy.GetResourceClassYieldChange((ResourceClassTypes)iResourceClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiResourceClassYieldChanges[iResourceClass][iYield] += iChange;
			}
		}
		// Improvement
		for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			iChange = kLegacy.GetImprovementYieldChange((ImprovementTypes)iImprovement, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiImprovementYieldChanges[iImprovement][iYield] += iChange;
			}
			iChange = kLegacy.GetImprovementYieldChangePerXWorldWonder((ImprovementTypes)iImprovement, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiImprovementYieldChangePerXWorldWonder[iImprovement][iYield] += iChange;
			}
		}
		// Great Work Class
		for (int iGreatWorkClass = 0; iGreatWorkClass < GC.getNumGreatWorkClassInfos(); iGreatWorkClass++)
		{
			iChange = kLegacy.GetGreatWorkClassYieldChange((GreatWorkClass)iGreatWorkClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiGreatWorkClassYieldChanges[iGreatWorkClass][iYield] += iChange;
			}
		}
		// Buildings
		for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
		{
			iChange = kLegacy.GetBuildingCostOverride((BuildingTypes)iBuilding, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiBuildingCostOverrides[iBuilding][iYield] = iChange;
			}
		}
		// Units
		for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
		{
			iChange = kLegacy.GetUnitCostOverride((UnitTypes)iUnit, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiUnitCostOverrides[iUnit][iYield] = iChange;
			}
		}
	} // END NUM_YIELD_TYPES LOOP
	for (int iDomain = 0; iDomain < NUM_DOMAIN_TYPES; iDomain++)
	{
		for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			iChange = kLegacy.GetImprovementNearbyHealChangeByDomain((ImprovementTypes)iImprovement, (DomainTypes)iDomain);
			if (iChange != 0)
			{
				m_vaaiNearbyImprovementHealChangeByDomain[iImprovement][iDomain] += iChange;
			}
			iChange = kLegacy.GetImprovementNearbyCombatModifierByDomain((ImprovementTypes)iImprovement, (DomainTypes)iDomain);
			if (iChange != 0)
			{
				m_vaaiNearbyImprovementCombatModifierByDomain[iImprovement][iDomain] += iChange;
			}
		}
	} // END NUM_DOMAIN_TYPES LOOP
	// BuildingClass - Non-yield
	for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
	{
		m_viBuildingClassProductionModifiers[iBuildingClass] += kLegacy.GetBuildingClassProductionModifier(iBuildingClass);
		m_viBuildingClassHappinessChanges[iBuildingClass] += kLegacy.GetBuildingClassHappinessChange(iBuildingClass);
		m_viBuildingClassGlobalHappinessChanges[iBuildingClass] += kLegacy.GetBuildingClassGlobalHappinessChange(iBuildingClass);
		m_viLegacyBuildingClassOverrides[iBuildingClass] = kLegacy.GetLegacyBuildingClassOverride(iBuildingClass);
	}
	// Specialist - Non-yield
	for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
	{
		iChange = kLegacy.GetSpecialistHappinessChange(iSpecialist);
		if (iChange != 0)
		{
			m_viSpecialistHappinessChanges[iSpecialist] += iChange;
		}
	}
	// Units - Non-yield
	for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
	{
		m_viUnitRangedStrengthChanges[iUnit] += kLegacy.GetUnitRangedStrengthChange(iUnit);
		m_viUnitStrengthChanges[iUnit] += kLegacy.GetUnitStrengthChange(iUnit);
		m_vbNoTrain[iUnit] = kLegacy.IsNoTrainUnit((UnitTypes)iUnit);
		m_vbUnitIgnorePolicyPrereq[iUnit] = kLegacy.IsUnitIgnorePolicyPrereq((UnitTypes)iUnit);
		m_vbUnitIgnoreTechPrereq[iUnit] = kLegacy.IsUnitIgnoreTechPrereq((UnitTypes)iUnit);
	}
	// UnitClasses
	for (int iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
	{
		m_viLegacyUnitClassOverrides[iUnitClass] = kLegacy.GetLegacyUnitClassOverride(iUnitClass);
	}
	// Buildings - Non-yield
	for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
	{
		m_vbNoConstruct[iBuilding] = kLegacy.IsNoConstructBuilding((BuildingTypes)iBuilding);
		m_vbBuildingIgnorePolicyPrereq[iBuilding] = kLegacy.IsBuildingIgnorePolicyPrereq((BuildingTypes)iBuilding);
		m_vbBuildingIgnoreTechPrereq[iBuilding] = kLegacy.IsBuildingIgnoreTechPrereq((BuildingTypes)iBuilding);
	}
	// Resources - Non-yield
	for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
	{
		m_vbRevealResource[iResource] = kLegacy.IsRevealResource((ResourceTypes)iResource);
	}
	// UnitCombatClasses
	for (int iUnitCombat = 0; iUnitCombat < GC.getNumUnitCombatClassInfos(); iUnitCombat++)
	{
		m_viPromotionNearbyGeneralUnitCombat[iUnitCombat] = kLegacy.GetPromotionNearbyGeneral(iUnitCombat);
	}
	// Great Work Classes
	for (int iGreatWorkClass = 0; iGreatWorkClass < GC.getNumGreatWorkClassInfos(); iGreatWorkClass++)
	{
		m_viGreatWorkClassTourismChanges[iGreatWorkClass] += kLegacy.GetGreatWorkClassTourismChange(iGreatWorkClass);
	}
	// Improvements - Non-yield
	for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
	{
		m_viImprovementTourismBonuses[iImprovement] += kLegacy.GetImprovementTourismBonus(iImprovement);
	}
	// Builds
	for (int iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
	{
		m_viBuildTimeOverrides[iBuild] = kLegacy.GetBuildTimeOverride(iBuild);
	}
}
// How Much happiness per original city does the player get from legacies
int CvPlayerLegacies::GetHappinessPerOriginalCity() const
{
	return m_iHappinessPerOriginalCity;
}
// How many golden age turns does the player get from legacies
int CvPlayerLegacies::GetGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}
// How much siege bonus does the player get for great generals from legacies
int CvPlayerLegacies::GetGreatGeneralSiegeBonus() const
{
	return m_iGreatGeneralSiegeBonus;
}
// How much resistance time reduction does the player get from legacies
int CvPlayerLegacies::GetResistanceTimeReduction() const
{
	return m_iResistanceTimeReduction;
}
// Change the behavoir of the CityYieldMod table based on this value
int CvPlayerLegacies::GetYieldModCapitalProximity() const
{
	return m_iYieldModCapitalProximity;
}
// How much does the player's legacies change the gold cost of plots?
int CvPlayerLegacies::GetPlotGoldCostModifier() const
{
	return m_iPlotGoldCostModifier;
}
// How much does the player's legacies change the culture cost of plots?
int CvPlayerLegacies::GetPlotCultureCostModifier() const
{
	return m_iPlotCultureCostModifier;
}
// How much happiness does the player get from great improvements from legacies
int CvPlayerLegacies::GetHappinessFromGreatImprovements() const
{
	return m_iHappinessFromGreatImprovements;
}
// How much happiness does the player get from foreign cities following their religion from legacies
int CvPlayerLegacies::GetHappinessFromForeignReligiousMajority() const
{
	return m_iHappinessFromForeignReligiousMajority;
}
// How many votes per capital does the player get from legacies
int CvPlayerLegacies::GetVotesPerCapital() const
{
	return m_iVotesPerCapital;
}
// How much influence change does the player get for majority religion in a City-State
int CvPlayerLegacies::GetInfluenceChangeMajorityReligion() const
{
	return m_iInfluenceChangeMajorityReligion;
}
// How much influence change does the player get for a trade connection with a City-State 
int CvPlayerLegacies::GetInfluenceChangeTradeConnection() const
{
	return m_iInfluenceChangeTradeConnection;
}
// How many extra moves does the player get for purchased units from legacies
int CvPlayerLegacies::GetPurchasedUnitExtraMoves() const
{
	return m_iPurchasedUnitExtraMoves;
}
// How much happiness per theme does the player get from legacies
int CvPlayerLegacies::GetHappinessPerTheme() const
{
	return m_iHappinessPerTheme;
}
// Can your Trade Routes be plundered?
bool CvPlayerLegacies::IsTradeUnplunderable() const
{
	return m_bTradeUnplunderable;
}
// Is the ability to plunder Trade Routes blocked by Legacy?
bool CvPlayerLegacies::IsCannotPlunder() const
{
	return m_bCannotPlunder;
}
// ARRAYS START
// Does the player have a free promotion for a specific unit type from legacies
bool CvPlayerLegacies::HasFreePromotionUnitType(PromotionTypes ePromotion, UnitTypes eUnitType) const
{
	CvAssertMsg((promotionID >= 0), "promotionID is less than zero");
	CvAssertMsg((promotionID < GC.getNumPromotionInfos()), "promotionID is expected to be within maximum bounds");
	if (ePromotion == NO_PROMOTION || eUnitType == NO_UNIT)
	{
		return false;
	}
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				if (pkLegacy->IsFreePromotionUnitType(ePromotion, eUnitType))
				{
					return true;
				}
			}
		}
	}

	return false;
}
// How much resource requirement change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitResourceRequirementChange(UnitTypes eUnit, ResourceTypes eResource) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	CvAssertMsg(eResource >= 0 && eResource < GC.getNumResourceInfos(), "Resource index out of bounds");
	if (eUnit == NO_UNIT || eResource == NO_RESOURCE)
	{
		return 0;
	}
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				int iChange = pkLegacy->GetUnitResourceRequirementChange(eUnit, eResource);
				if (iChange != 0)
				{
					return iChange;
				}
			}
		}
	}
	return 0;
}
// How much great person point modifier does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassGreatPersonPointModifier(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "GreatPerson index out of bounds");
	if (eBuildingClass == NO_BUILDINGCLASS || eSpecialist == NO_SPECIALIST)
	{
		return 0;
	}
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				int iChange = pkLegacy->GetBuildingClassGreatPersonPointModifier(eBuildingClass, eSpecialist);
				if (iChange != 0)
				{
					return iChange;
				}
			}
		}
	}
	return 0;
}
// How much great person point change does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassGreatPersonPointChange(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "GreatPerson index out of bounds");
	if (eBuildingClass == NO_BUILDINGCLASS || eSpecialist == NO_SPECIALIST)
	{
		return 0;
	}
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				int iChange = pkLegacy->GetBuildingClassGreatPersonPointChange(eBuildingClass, eSpecialist);
				if (iChange != 0)
				{
					return iChange;
				}
			}
		}
	}
	return 0;
}
// BELOW HERE ARE SIMPLE GETTERS FOR PLAYER LEGACY EFFECTS AS THEY HAVE SIMPLE STORAGE
// Get the promotion granted for having a nearby general for a specific unit combat type from legacies
int CvPlayerLegacies::GetPromotionNearbyGeneralUnitCombat(UnitCombatTypes eUnitCombat) const
{
	CvAssertMsg(eUnitCombat >= 0 && eUnitCombat < GC.getNumUnitCombatClassInfos(), "UnitCombat index out of bounds");
	return NO_UNITCOMBAT != eUnitCombat ? m_viPromotionNearbyGeneralUnitCombat[eUnitCombat] : NO_PROMOTION;
}
// Does this Legacy reveal this resource type for the player
bool CvPlayerLegacies::IsRevealResource(ResourceTypes eResource) const
{
	return NO_RESOURCE != eResource ? m_vbRevealResource[eResource] : false;
}
// Get the Building Type override for a specific building class from legacies
int CvPlayerLegacies::GetLegacyBuildingClassOverride(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return NO_BUILDINGCLASS != eBuildingClass ? m_viLegacyBuildingClassOverrides[(int)eBuildingClass] : NO_BUILDING;
}
// Does this Legacy allow this building type to ignore policy prerequisites
bool CvPlayerLegacies::IsBuildingIgnorePolicyPrereq(BuildingTypes eBuildingType) const
{
	return NO_BUILDING != eBuildingType ? m_vbBuildingIgnorePolicyPrereq[eBuildingType] : false;
}
// Does this Legacy allow this building type to ignore tech prerequisites
bool CvPlayerLegacies::IsBuildingIgnoreTechPrereq(BuildingTypes eBuildingType) const
{
	return NO_BUILDING != eBuildingType ? m_vbBuildingIgnoreTechPrereq[eBuildingType] : false;
}
// Get the unit type override for a specific unit class from legacies
int CvPlayerLegacies::GetLegacyUnitClassOverride(UnitClassTypes eUnitClass) const
{
	CvAssertMsg(eUnitClass >= 0 && eUnitClass < GC.getNumUnitClassInfos(), "UnitClass index out of bounds");
	return NO_UNITCLASS != eUnitClass ? m_viLegacyUnitClassOverrides[(int)eUnitClass] : NO_UNIT;
}
// Does this Legacy allow this unit type to ignore policy prerequisites
bool CvPlayerLegacies::IsUnitIgnorePolicyPrereq(UnitTypes eUnitType) const
{
	return NO_UNIT != eUnitType ? m_vbUnitIgnorePolicyPrereq[eUnitType] : false;
}
// Does this Legacy allow this unit type to ignore tech prerequisites
bool CvPlayerLegacies::IsUnitIgnoreTechPrereq(UnitTypes eUnitType) const
{
	return NO_UNIT != eUnitType ? m_vbUnitIgnoreTechPrereq[eUnitType] : false;
}
// Is this unit type not trainable by the player from legacies
bool CvPlayerLegacies::NoTrainUnit(UnitTypes eUnitType) const
{
	return NO_UNIT != eUnitType ? m_vbNoTrain[eUnitType] : false;
}
// Is this building type not constructable by the player from legacies
bool CvPlayerLegacies::NoConstructBuilding(BuildingTypes eBuildingType) const
{
	return NO_BUILDING != eBuildingType ? m_vbNoConstruct[eBuildingType] : false;
}
// How much yield reward does the player get for purchasing a plot from legacies
int CvPlayerLegacies::GetPlotPurchaseYieldReward(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viPlotPurchaseYieldReward[eYield] : 0;
}
// How much yield change does the player get for a city from legacies
int CvPlayerLegacies::GetCityYieldChange(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viCityYieldChange[eYield] : 0;
}
// How much yield change does the player get for an original city from legacies
int CvPlayerLegacies::GetOriginalCityYieldChange(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viOriginalCityYieldChange[eYield] : 0;
}
// How much yield change does the player get for a conquered city from legacies
int CvPlayerLegacies::GetConqueredCityYieldChange(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viConqueredCityYieldChange[eYield] : 0;
}
// How much yield modifier does the player get for a city from legacies
int CvPlayerLegacies::GetCityYieldModifier(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viCityYieldModifier[eYield] : 0;
}
// How much production modifier does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassProductionModifier(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassProductionModifiers[(int)eBuildingClass] : 0;
}
int CvPlayerLegacies::GetBuildingClassHappinessChange(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassHappinessChanges[(int)eBuildingClass] : 0;
}
int CvPlayerLegacies::GetBuildingClassGlobalHappinessChange(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassGlobalHappinessChanges[(int)eBuildingClass] : 0;
}
// How much ranged strength change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitRangedStrengthChange(UnitTypes eUnit) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	return eUnit != NO_UNIT ? m_viUnitRangedStrengthChanges[(int)eUnit] : 0;
}
// How much strength change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitStrengthChange(UnitTypes eUnit) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	return eUnit != NO_UNIT ? m_viUnitStrengthChanges[(int)eUnit] : 0;
}
// How much yield change does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_vaaiBuildingClassYieldChanges[(int)eBuildingClass][(int)eYield] : 0;
}
// How much yield modifier does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYield) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_vaaiBuildingClassYieldModifiers[(int)eBuildingClass][(int)eYield] : 0;
}
// How much of eYield does eResource get from Legacies?
int CvPlayerLegacies::GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYield) const
{
	CvAssertMsg(eResource >= 0 && eResource < GC.getNumResourceInfos(), "Resource index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eResource != NO_RESOURCE ? m_vaaiResourceYieldChanges[(int)eResource][(int)eYield] : 0;
}
// How much of eYield does eResourceClass get from Legacies?
int CvPlayerLegacies::GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYield) const
{
	CvAssertMsg(eResourceClass >= 0 && eResourceClass < GC.getNumResourceClaasInfos(), "ResourceClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eResourceClass != NO_RESOURCECLASS ? m_vaaiResourceClassYieldChanges[(int)eResourceClass][(int)eYield] : 0;
}
// How much yield change does the player get for a specialist from legacies
int CvPlayerLegacies::GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "Specialist index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eSpecialist != NO_SPECIALIST ? m_vaaiSpecialistYieldChanges[(int)eSpecialist][(int)eYield] : 0;
}
// How much happiness change does the player get for a specialist from legacies
int CvPlayerLegacies::GetSpecialistHappinessChange(SpecialistTypes eSpecialist) const
{
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "Specialist index out of bounds");
	return eSpecialist != NO_SPECIALIST ? m_viSpecialistHappinessChanges[(int)eSpecialist] : 0;
}
// How much yield change does the player get for an improvement from legacies
int CvPlayerLegacies::GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const
{
	CvAssertMsg(eImprovement >= 0 && eImprovement < GC.getNumImprovementInfos(), "Improvement index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eImprovement != NO_IMPROVEMENT ? m_vaaiImprovementYieldChanges[(int)eImprovement][(int)eYield] : 0;
}
// How much yield change per X world wonders does the player get for an improvement from legacies
int CvPlayerLegacies::GetImprovementYieldChangePerXWorldWonder(ImprovementTypes eImprovement, YieldTypes eYield) const
{
	CvAssertMsg(eImprovement >= 0 && eImprovement < GC.getNumImprovementInfos(), "Improvement index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eImprovement != NO_IMPROVEMENT ? m_vaaiImprovementYieldChangePerXWorldWonder[(int)eImprovement][(int)eYield] : 0;
}
// How much nearby heal change by domain does the player get for an improvement from legacies
int CvPlayerLegacies::GetNearbyImprovementHealChangeByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const
{
	CvAssertMsg(eImprovement >= 0 && eImprovement < GC.getNumImprovementInfos(), "Improvement index out of bounds");
	CvAssertMsg(eDomain >= 0 && eDomain < NUM_DOMAIN_TYPES, "Domain index out of bounds");
	return eImprovement != NO_IMPROVEMENT ? m_vaaiNearbyImprovementHealChangeByDomain[(int)eImprovement][(int)eDomain] : 0;
}
// How much nearby combat modifier by domain does the player get for an improvement from legacies
int CvPlayerLegacies::GetNearbyImprovementCombatModifierByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const
{
	CvAssertMsg(eImprovement >= 0 && eImprovement < GC.getNumImprovementInfos(), "Improvement index out of bounds");
	CvAssertMsg(eDomain >= 0 && eDomain < NUM_DOMAIN_TYPES, "Domain index out of bounds");
	return eImprovement != NO_IMPROVEMENT ? m_vaaiNearbyImprovementCombatModifierByDomain[(int)eImprovement][(int)eDomain] : 0;
}
// How much yield change does the player get for a great work class from legacies
int CvPlayerLegacies::GetGreatWorkClassYieldChange(GreatWorkClass eGreatWorkClass, YieldTypes eYield) const
{
	CvAssertMsg(eGreatWorkClass >= 0 && eGreatWorkClass < GC.getNumGreatWorkClassInfos(), "GreatWorkClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_GREAT_WORK_CLASS != eGreatWorkClass ? m_vaaiGreatWorkClassYieldChanges[(int)eGreatWorkClass][(int)eYield] : 0;
}
// How much tourism change does the player get for a great work class from legacies
int CvPlayerLegacies::GetGreatWorkClassTourismChange(GreatWorkClass eGreatWorkClass) const
{
	CvAssertMsg(eGreatWorkClass >= 0 && eGreatWorkClass < GC.getNumGreatWorkClassInfos(), "GreatWorkClass index out of bounds");
	return NO_GREAT_WORK_CLASS != eGreatWorkClass ? m_viGreatWorkClassTourismChanges[(int)eGreatWorkClass] : 0;
}
// How much tourism bonus does the player get for an improvement from legacies
int CvPlayerLegacies::GetImprovementTourismBonus(ImprovementTypes eImprovement) const
{
	CvAssertMsg(eImprovement >= 0 && eImprovement < GC.getNumImprovementInfos(), "Improvement index out of bounds");
	return NO_IMPROVEMENT != eImprovement ? m_viImprovementTourismBonuses[(int)eImprovement] : 0;
}
// Does this Legacy set the cost of a building type for the player
int CvPlayerLegacies::GetBuildingCostOverride(BuildingTypes eBuilding, YieldTypes eYield) const
{
	CvAssertMsg(eBuilding >= 0 && eBuilding < GC.getNumBuildingInfos(), "Building index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_BUILDING != eBuilding ? m_vaaiBuildingCostOverrides[(int)eBuilding][(int)eYield] : 0;
}
// Does this Legacy set the cost of a unit type for the player
int CvPlayerLegacies::GetUnitCostOverride(UnitTypes eUnit, YieldTypes eYield) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_UNIT != eUnit ? m_vaaiUnitCostOverrides[(int)eUnit][(int)eYield] : 0;
}
// Does this Legacy set the build time of a build type for the player
int CvPlayerLegacies::GetBuildTimeOverride(BuildTypes eBuild) const
{
	CvAssertMsg(eBuild >= 0 && eBuild < GC.getNumBuildInfos(), "Build index out of bounds");
	return NO_BUILD != eBuild ? m_viBuildTimeOverrides[(int)eBuild] : -1;
}
// Does this legacy give a yield bonus for having active themes?
int CvPlayerLegacies::GetYieldBonusFromThemes(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return NO_YIELD != eYield ? m_viYieldBonusFromThemes[(int)eYield] : 0;
}
void CvPlayerLegacies::DoLegacyAI()
{
	//
}
// Choose a legacy for the AI Player
void CvPlayerLegacies::DoChooseLegacy()
{
	m_pLegacyAI->ChooseLegacy(m_pPlayer);
}
#endif // LEKMOD_LEGACY