/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "ICvDLLUserInterface.h"
#include "CvGameCoreUtils.h"
#include "CvImprovementClasses.h"
#include "FireWorks/FRemark.h"
#include "CvInfosSerializationHelper.h"

// must be included after all other headers
#include "LintFree.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4505 ) // unreferenced local function has been removed.. needed by REMARK below
#endif//_MSC_VER
REMARK_GROUP("CvImprovementClasses");


//======================================================================================================
//					CvImprovementResourceInfo
//======================================================================================================
CvImprovementResourceInfo::CvImprovementResourceInfo() :
	m_iDiscoverRand(0),
	m_bResourceMakesValid(false),
	m_bResourceTrade(false),
	m_piYieldChange(NULL)
{
}
//------------------------------------------------------------------------------
CvImprovementResourceInfo::~CvImprovementResourceInfo()
{
	SAFE_DELETE_ARRAY(m_piYieldChange);
}
//------------------------------------------------------------------------------
int CvImprovementResourceInfo::getDiscoverRand() const
{
	return m_iDiscoverRand;
}
//------------------------------------------------------------------------------
bool CvImprovementResourceInfo::isResourceMakesValid() const
{
	return m_bResourceMakesValid;
}
//------------------------------------------------------------------------------
bool CvImprovementResourceInfo::isResourceTrade() const
{
	return m_bResourceTrade;
}
//------------------------------------------------------------------------------
int CvImprovementResourceInfo::getYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChange ? m_piYieldChange[i] : -1;
}
//------------------------------------------------------------------------------

//======================================================================================================
//					CvImprovementEntry
//======================================================================================================
CvImprovementEntry::CvImprovementEntry(void):
	m_iGoldMaintenance(0),
	m_iCultureBombRadius(0),
	m_iCultureBombRadiusNeutral(0),
	m_iRequiresXAdjacentLand(-1),
	m_iCultureAdjacentSameType(0),
	m_iTilesPerGoody(0),
	m_iGoodyUniqueRange(0),
	m_iFeatureGrowthProbability(0),
	m_iUpgradeTime(0),
	m_iRiverSideUpgradeMod(0),
	m_iCoastalLandUpgradeMod(0),
	m_iHillsUpgradeMod(0),
	m_iFreshWaterUpgradeMod(0),
	m_iDefenseModifier(0),
	m_iDefenseModifierGlobal(0),
	m_iNearbyEnemyDamage(0),
	m_iPillageGold(0),
	m_iResourceExtractionMod(0),
	m_iLuxuryCopiesSiphonedFromMinor(0),
	m_iImprovementPillage(NO_IMPROVEMENT),
	m_iImprovementUpgrade(NO_IMPROVEMENT),
	m_bHillsMakesValid(false),
	m_bWaterAdjacencyMakesValid(false),
	m_bFreshWaterMakesValid(false),
	m_bRiverSideMakesValid(false),
	m_bNoFreshWater(false),
	m_bRequiresFlatlands(false),
	m_bRequiresFlatlandsOrFreshWater(false),
	m_bRequiresFeature(false),
	m_bRequiresImprovement(false),
	m_bRemovesResource(false),
	m_bPromptWhenComplete(false),
	m_bWater(false),
	m_bCoastal(false),
	m_bDestroyedWhenPillaged(false),
	m_bDisplacePillager(false),
	m_bBuildableOnResources(false),
	m_bBarbarianCamp(false),
	m_bGoody(false),
	m_bPermanent(false),
	m_bOutsideBorders(false),
	m_bInAdjacentFriendly(false),
	m_bIgnoreOwnership(false),
	m_bOnlyCityStateTerritory(false),
	m_bNoTwoAdjacent(false),
	m_bAdjacentLuxury(false),
	m_bAllowsWalkWater(false),
	m_bAllowsSailLand(false),
	m_bCreatedByGreatPerson(false),
	m_bSpecificCivRequired(false),
	m_eImprovementUsageType(IMPROVEMENTUSAGE_BASIC),
	m_eRequiredCivilization(NO_CIVILIZATION),
	m_iWorldSoundscapeScriptId(0),
	m_piResourceQuantityRequirements(NULL),
	m_piPrereqNatureYield(NULL),
	m_piYieldChange(NULL),
	m_piYieldPerEra(NULL),
	m_piRiverSideYieldChange(NULL),
	m_piCoastalLandYieldChange(NULL),
	m_piHillsYieldChange(NULL),
	m_piFreshWaterChange(NULL),
	m_piAdjacentCityYieldChange(NULL),
	m_piAdjacentMountainYieldChange(NULL),
	m_piFlavorValue(NULL),
	m_pbTerrainMakesValid(NULL),
	m_pbFeatureMakesValid(NULL),
	m_pbImprovementMakesValid(NULL),
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	m_ppiTechYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiTechNoFreshWaterYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiTechFreshWaterYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiRouteYieldChanges(std::pair<int**, size_t>(NULL, 0)),
#else
	m_ppiTechYieldChanges(NULL),
	m_ppiTechNoFreshWaterYieldChanges(NULL),
	m_ppiTechFreshWaterYieldChanges(NULL),
	m_ppiRouteYieldChanges(NULL),
#endif
#ifdef LEKMOD_ADJACENT_IMPROVEMENT_YIELD
	m_piImprovementAdjacentBonusCivilization(NULL),
	m_ppiImprovementAdjacentCivilizationAmount(NULL),
	m_ppiImprovementAdjacentBonusCivilizationNoAmount(NULL),
	m_ppiImprovementAdjacentBonus(NULL),
	m_ppiImprovementAdjacentAmount(NULL),

#endif

	m_paImprovementResource(NULL)
{
}

/// Destructor
CvImprovementEntry::~CvImprovementEntry(void)
{
	SAFE_DELETE_ARRAY(m_piResourceQuantityRequirements);
	SAFE_DELETE_ARRAY(m_piPrereqNatureYield);
	SAFE_DELETE_ARRAY(m_piYieldChange);
	SAFE_DELETE_ARRAY(m_piYieldPerEra);
	SAFE_DELETE_ARRAY(m_piRiverSideYieldChange);
	SAFE_DELETE_ARRAY(m_piCoastalLandYieldChange);
	SAFE_DELETE_ARRAY(m_piHillsYieldChange);
	SAFE_DELETE_ARRAY(m_piFreshWaterChange);
	SAFE_DELETE_ARRAY(m_piAdjacentCityYieldChange);
	SAFE_DELETE_ARRAY(m_piAdjacentMountainYieldChange);
	SAFE_DELETE_ARRAY(m_piFlavorValue);
	SAFE_DELETE_ARRAY(m_pbTerrainMakesValid);
	SAFE_DELETE_ARRAY(m_pbFeatureMakesValid);
	SAFE_DELETE_ARRAY(m_pbImprovementMakesValid);

	if(m_paImprovementResource != NULL)
	{
		SAFE_DELETE_ARRAY(m_paImprovementResource); // XXX make sure this isn't leaking memory...
	}

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	CvDatabaseUtility::SafeDelete2DArray(m_ppiTechYieldChanges.first, m_ppiTechYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiTechNoFreshWaterYieldChanges.first, m_ppiTechNoFreshWaterYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiTechFreshWaterYieldChanges.first, m_ppiTechFreshWaterYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiRouteYieldChanges.first, m_ppiRouteYieldChanges.second);
#else
	if(m_ppiTechYieldChanges != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiTechYieldChanges);
	}

	if(m_ppiTechNoFreshWaterYieldChanges != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiTechNoFreshWaterYieldChanges);
	}

	if(m_ppiTechFreshWaterYieldChanges != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiTechFreshWaterYieldChanges);
	}

	if(m_ppiRouteYieldChanges != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiRouteYieldChanges);
	}
#endif
#ifdef LEKMOD_ADJACENT_IMPROVEMENT_YIELD
	if (m_ppiImprovementAdjacentBonus != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementAdjacentBonus);
	}

	if (m_ppiImprovementAdjacentCivilizationAmount != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementAdjacentCivilizationAmount);
	}

	if (m_ppiImprovementAdjacentBonusCivilizationNoAmount != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementAdjacentBonusCivilizationNoAmount);
	}

	if (m_piImprovementAdjacentBonusCivilization != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_piImprovementAdjacentBonusCivilization);
	}

	if (m_ppiImprovementAdjacentAmount != NULL)
	{
		CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementAdjacentAmount);
	}

#endif
}

/// Read from XML file
bool CvImprovementEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if(!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	//Basic properties
	const char* szArtDefineTag = kResults.GetText("ArtDefineTag");
	SetArtDefineTag(szArtDefineTag);

	m_iGoldMaintenance = kResults.GetInt("GoldMaintenance");
	m_iCultureBombRadius = kResults.GetInt("CultureBombRadius");
	m_iCultureBombRadiusNeutral = kResults.GetInt("CultureBombRadiusNeutral");
	m_iRequiresXAdjacentLand = kResults.GetInt("RequiresXAdjacentLand");
	m_iCultureAdjacentSameType = kResults.GetInt("CultureAdjacentSameType");
	m_bHillsMakesValid = kResults.GetBool("HillsMakesValid");
	m_bWaterAdjacencyMakesValid = kResults.GetBool("WaterAdjacencyMakesValid");
	m_bFreshWaterMakesValid = kResults.GetBool("FreshWaterMakesValid");
	m_bRiverSideMakesValid = kResults.GetBool("RiverSideMakesValid");
	m_bNoFreshWater = kResults.GetBool("NoFreshWater");
	m_bRequiresFlatlands = kResults.GetBool("RequiresFlatlands");
	m_bRequiresFlatlandsOrFreshWater = kResults.GetBool("RequiresFlatlandsOrFreshWater");
	m_bRequiresFeature = kResults.GetBool("RequiresFeature");
	m_bRequiresImprovement = kResults.GetBool("RequiresImprovement");
	m_bRemovesResource = kResults.GetBool("RemovesResource");
	m_bPromptWhenComplete = kResults.GetBool("PromptWhenComplete");
	m_bWater = kResults.GetBool("Water");
	m_bCoastal = kResults.GetBool("Coastal");
	m_bDestroyedWhenPillaged = kResults.GetBool("DestroyedWhenPillaged");
	m_bDisplacePillager = kResults.GetBool("DisplacePillager");
	m_bBuildableOnResources = kResults.GetBool("BuildableOnResources");
	m_eImprovementUsageType = m_bBuildableOnResources ? IMPROVEMENTUSAGE_LANDMARK : IMPROVEMENTUSAGE_BASIC;
	m_bBarbarianCamp = kResults.GetBool("BarbarianCamp");
	m_bGoody = kResults.GetBool("Goody");
	m_bPermanent = kResults.GetBool("Permanent");
	m_iTilesPerGoody = kResults.GetInt("TilesPerGoody");
	m_iGoodyUniqueRange = kResults.GetInt("GoodyRange");
	m_iFeatureGrowthProbability = kResults.GetInt("FeatureGrowth");
	m_iUpgradeTime = kResults.GetInt("UpgradeTime");
	m_iRiverSideUpgradeMod = kResults.GetInt("RiverSideUpgradeMod");
	m_iCoastalLandUpgradeMod = kResults.GetInt("CoastalLandUpgradeMod");
	m_iHillsUpgradeMod = kResults.GetInt("HillsUpgradeMod");
	m_iFreshWaterUpgradeMod = kResults.GetInt("FreshWaterUpgradeMod");
	m_iDefenseModifier = kResults.GetInt("DefenseModifier");
	m_iDefenseModifierGlobal = kResults.GetInt("DefenseModifierGlobal");
	m_iNearbyEnemyDamage = kResults.GetInt("NearbyEnemyDamage");
	m_iPillageGold = kResults.GetInt("PillageGold");
	m_bOutsideBorders = kResults.GetBool("OutsideBorders");
	m_bInAdjacentFriendly = kResults.GetBool("InAdjacentFriendly");
	m_bIgnoreOwnership = kResults.GetBool("IgnoreOwnership");
	m_bOnlyCityStateTerritory = kResults.GetBool("OnlyCityStateTerritory");
	m_bNoTwoAdjacent = kResults.GetBool("NoTwoAdjacent");
	m_bAdjacentLuxury = kResults.GetBool("AdjacentLuxury");
	m_bAllowsWalkWater = kResults.GetBool("AllowsWalkWater");
	m_bAllowsSailLand = kResults.GetBool("AllowsSailLand"); // from Izy
	m_bCreatedByGreatPerson = kResults.GetBool("CreatedByGreatPerson");
	m_bSpecificCivRequired = kResults.GetBool("SpecificCivRequired");
	m_iResourceExtractionMod = kResults.GetInt("ResourceExtractionMod");
	m_iLuxuryCopiesSiphonedFromMinor = kResults.GetInt("LuxuryCopiesSiphonedFromMinor");

	const char* szCivilizationType = kResults.GetText("CivilizationType");
	m_eRequiredCivilization = (CivilizationTypes)GC.getInfoTypeForString(szCivilizationType, true);

#ifdef LEKMOD_CUSTOM_IMPROVEMENT_ICONS
	const char* szIconString = kResults.GetText("IconString");
	SetIconString(szIconString);
#endif

	//References
	const char* szWorldsoundscapeAudioScript = kResults.GetText("WorldSoundscapeAudioScript");
	if(szWorldsoundscapeAudioScript != NULL)
	{
		m_iWorldSoundscapeScriptId = gDLL->GetAudioTagIndex(szWorldsoundscapeAudioScript, AUDIOTAG_SOUNDSCAPE);
	}
	else
	{
		m_iWorldSoundscapeScriptId = -1;
		Remark(1, "Warning: Missing soundscape definition in XML for feature: '%s'", GetType());
	}

	const char* szImprovementPillage = kResults.GetText("ImprovementPillage");
	m_iImprovementPillage = GC.getInfoTypeForString(szImprovementPillage, true);

	const char* szImprovementUpgrade = kResults.GetText("ImprovementUpgrade");
	m_iImprovementUpgrade = GC.getInfoTypeForString(szImprovementUpgrade, true);

	//Arrays
	const char* szImprovementType = GetType();
	const size_t lenImprovementType = strlen(szImprovementType);

	kUtility.PopulateArrayByExistence(m_pbTerrainMakesValid,
	                                  "Terrains",
	                                  "Improvement_ValidTerrains",
	                                  "TerrainType",
	                                  "ImprovementType",
	                                  szImprovementType);

	kUtility.PopulateArrayByExistence(m_pbFeatureMakesValid,
	                                  "Features",
	                                  "Improvement_ValidFeatures",
	                                  "FeatureType",
	                                  "ImprovementType",
	                                  szImprovementType);

	kUtility.PopulateArrayByExistence(m_pbImprovementMakesValid,
									  "Improvements",
									  "Improvement_ValidImprovements",
									  "PrereqImprovement",
									  "ImprovementType",
							          szImprovementType);

	kUtility.SetYields(m_piYieldChange, "Improvement_Yields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piYieldPerEra, "Improvement_YieldPerEra", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piAdjacentCityYieldChange, "Improvement_AdjacentCityYields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piAdjacentMountainYieldChange, "Improvement_AdjacentMountainYieldChanges", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piCoastalLandYieldChange, "Improvement_CoastalLandYields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piFreshWaterChange, "Improvement_FreshWaterYields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piHillsYieldChange, "Improvement_HillsYields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piRiverSideYieldChange, "Improvement_RiverSideYields", "ImprovementType", szImprovementType);
	kUtility.SetYields(m_piPrereqNatureYield, "Improvement_PrereqNatureYields", "ImprovementType", szImprovementType);

	kUtility.SetFlavors(m_piFlavorValue, "Improvement_Flavors", "ImprovementType", szImprovementType);



	{
		//Initialize Improvement Resource Types to number of Resources
		const int iNumResources = kUtility.MaxRows("Resources");
		m_paImprovementResource = FNEW(CvImprovementResourceInfo[iNumResources], c_eCiv5GameplayDLL, 0);

		kUtility.InitializeArray(m_piResourceQuantityRequirements, iNumResources);


		std::string strResourceTypesKey = "Improvements - ResourceTypes";
		Database::Results* pResourceTypes = kUtility.GetResults(strResourceTypesKey);
		if(pResourceTypes == NULL)
		{
			pResourceTypes = kUtility.PrepareResults(strResourceTypesKey, "select Resources.ID, ResourceType, ResourceMakesValid, ResourceTrade, DiscoveryRand from Improvement_ResourceTypes inner join Resources on ResourceType = Resources.Type where ImprovementType = ?");
		}

		std::string strYieldResultsKey = "Improvements - YieldResults";
		Database::Results* pYieldResults = kUtility.GetResults(strYieldResultsKey);
		if(pYieldResults == NULL)
		{
			pYieldResults = kUtility.PrepareResults(strYieldResultsKey, "select Yields.ID, Yield from Improvement_ResourceType_Yields inner join Yields on YieldType = Yields.Type where ImprovementType = ? and ResourceType = ?");
		}

		pResourceTypes->Bind(1, szImprovementType, lenImprovementType, false);

		while(pResourceTypes->Step())
		{
			const int idx = pResourceTypes->GetInt("ID");
			CvImprovementResourceInfo& pResourceInfo = m_paImprovementResource[idx];

			const char* szResourceType = pResourceTypes->GetText("ResourceType");
			pResourceInfo.m_bResourceMakesValid = pResourceTypes->GetBool("ResourceMakesValid");
			pResourceInfo.m_bResourceTrade = pResourceTypes->GetBool("ResourceTrade");
			pResourceInfo.m_iDiscoverRand = pResourceTypes->GetInt("DiscoveryRand");
			m_piResourceQuantityRequirements[idx] = pResourceTypes->GetInt("QuantityRequirement");

			//Populate Yields for structure
			kUtility.InitializeArray(pResourceInfo.m_piYieldChange, "Yields");

			pYieldResults->Bind(1, szImprovementType, lenImprovementType, false);
			pYieldResults->Bind(2, szResourceType, -1, false);
			while(pYieldResults->Step())
			{
				const int yieldIdx = pYieldResults->GetInt("ID");
				const int yield = pYieldResults->GetInt("Yield");
				pResourceInfo.m_piYieldChange[yieldIdx] = yield;
			}

			pYieldResults->Reset();
		}

		pResourceTypes->Reset();
	}


	const int iNumYields = kUtility.MaxRows("Yields");
	const int iNumTechs = GC.getNumTechInfos();
	CvAssertMsg(iNumTechs > 0, "Num Tech Infos <= 0");


	//TechYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiTechYieldChanges.first, iNumTechs, iNumYields);
		m_ppiTechYieldChanges.second = iNumTechs;
#else
		kUtility.Initialize2DArray(m_ppiTechYieldChanges, iNumTechs, iNumYields);
#endif

		std::string strKey = "Improvements - TechYieldChanges";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, Technologies.ID as TechID, Yield from Improvement_TechYieldChanges inner join Yields on YieldType = Yields.Type inner join Technologies on TechType = Technologies.Type where ImprovementType = ?");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while(pResults->Step())
		{
			const int yield_idx = pResults->GetInt(0);
			CvAssert(yield_idx > -1);

			const int tech_idx = pResults->GetInt(1);
			CvAssert(tech_idx > -1);

			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiTechYieldChanges.first[tech_idx][yield_idx] = yield;
#else
			m_ppiTechYieldChanges[tech_idx][yield_idx] = yield;
#endif
		}
	}

#ifdef LEKMOD_ADJACENT_IMPROVEMENT_YIELD

	{
		
		const int iImprovementTypes = kUtility.MaxRows("Improvements");
		const int iNumYields = kUtility.MaxRows("Yields");
		const int iNumMaxAmount = 7;

		kUtility.Initialize2DArray(m_piImprovementAdjacentBonusCivilization, 200, iImprovementTypes);
		kUtility.Initialize2DArray(m_ppiImprovementAdjacentCivilizationAmount, iNumYields, iNumMaxAmount);
		kUtility.Initialize2DArray(m_ppiImprovementAdjacentBonusCivilizationNoAmount, iImprovementTypes, iNumYields);

		std::string strKey = "Improvement_AdjacencyYieldCivilization";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select OtherImprovementType, Civilizations.ID, Amount, Yields.ID, Yield from Improvement_AdjacencyYieldCivilization inner join Yields on YieldType = Yields.Type inner join Civilizations on CivilizationType = Civilizations.Type where ImprovementType = ?");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while (pResults->Step())
		{
			const int sImprovementType = GC.getInfoTypeForString(pResults->GetText(0), true);
			CvAssert(sImprovementType > -1);
			const int iCivilizationType = pResults->GetInt(1);
			CvAssert(iCivilizationType > -1);
			const int iAmount = pResults->GetInt(2);
			CvAssert(iAmount > -1);
			const int iYieldType = pResults->GetInt(3);
			CvAssert(iYieldType > -1);
			const int iYield = pResults->GetInt(4);

			m_piImprovementAdjacentBonusCivilization[iCivilizationType][sImprovementType] = sImprovementType;
			if (iAmount == 0)
			{
				m_ppiImprovementAdjacentBonusCivilizationNoAmount[sImprovementType][iYieldType] = iYield;
			}
			else if (iAmount > 0 && iAmount < iNumMaxAmount)
			{
				m_ppiImprovementAdjacentCivilizationAmount[iYieldType][iAmount] = iYield;
			}
		}

		pResults->Reset();

	}
	
	//ImprovementAdjacentBonus no Civilization but with amount
	{
		
		const int iImprovementTypes = kUtility.MaxRows("Improvements");
		const int iNumYields = kUtility.MaxRows("Yields");
		const int iNumMaxAmount = 7;
		kUtility.Initialize2DArray(m_ppiImprovementAdjacentBonus, iImprovementTypes, iNumYields);
		kUtility.Initialize2DArray(m_ppiImprovementAdjacentAmount, iImprovementTypes, iNumMaxAmount);

		std::string strKey = "Improvement_AdjacencyYield";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select OtherImprovementType, Amount, Yields.ID, Yield from Improvement_AdjacencyYield inner join Yields on YieldType = Yields.Type where ImprovementType = ?");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while (pResults->Step())
		{
			const int sImprovementType = GC.getInfoTypeForString(pResults->GetText(0), true);
			CvAssert(sImprovementType > -1);
			const int iAmount = pResults->GetInt(1);
			CvAssert(iAmount > -1);
			const int iYieldType = pResults->GetInt(2);
			CvAssert(iYieldType > -1);
			const int iYield = pResults->GetInt(3);

			if (iAmount == 0)
			{
				m_ppiImprovementAdjacentBonus[sImprovementType][iYieldType] = iYield;
			}
			else if (iAmount > 0 && iAmount < iNumMaxAmount)
			{
				m_ppiImprovementAdjacentAmount[iYieldType][iAmount] = iYield;
			}
		}

		pResults->Reset();

	}
	

#endif

	//TechNoFreshWaterYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiTechNoFreshWaterYieldChanges.first, iNumTechs, iNumYields);
		m_ppiTechNoFreshWaterYieldChanges.second = iNumTechs;
#else
		kUtility.Initialize2DArray(m_ppiTechNoFreshWaterYieldChanges, iNumTechs, iNumYields);
#endif

		std::string strKey = "Improvements - TechNoFreshWaterYieldChanges";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, Technologies.ID as TechID, Yield from Improvement_TechNoFreshWaterYieldChanges inner join Yields on YieldType = Yields.Type inner join Technologies on TechType = Technologies.Type where ImprovementType = ?");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while(pResults->Step())
		{
			const int yield_idx = pResults->GetInt(0);
			CvAssert(yield_idx > -1);

			const int tech_idx = pResults->GetInt(1);
			CvAssert(tech_idx > -1);

			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiTechNoFreshWaterYieldChanges.first[tech_idx][yield_idx] = yield;
#else
			m_ppiTechNoFreshWaterYieldChanges[tech_idx][yield_idx] = yield;
#endif
		}

		pResults->Reset();

	}

	//TechFreshWaterYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiTechFreshWaterYieldChanges.first, iNumTechs, iNumYields);
		m_ppiTechFreshWaterYieldChanges.second = iNumTechs;
#else
		kUtility.Initialize2DArray(m_ppiTechFreshWaterYieldChanges, iNumTechs, iNumYields);
#endif

		std::string strKey = "Improvements - TechFreshWaterYieldChanges";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, Technologies.ID as TechID, Yield from Improvement_TechFreshWaterYieldChanges inner join Yields on YieldType = Yields.Type inner join Technologies on TechType = Technologies.Type where ImprovementType = ?");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while(pResults->Step())
		{
			const int yield_idx = pResults->GetInt(0);
			CvAssert(yield_idx > -1);

			const int tech_idx = pResults->GetInt(1);
			CvAssert(tech_idx > -1);

			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiTechFreshWaterYieldChanges.first[tech_idx][yield_idx] = yield;
#else
			m_ppiTechFreshWaterYieldChanges[tech_idx][yield_idx] = yield;
#endif
		}

		pResults->Reset();

	}

	//RouteYieldChanges
	{
		const int iNumRoutes = kUtility.MaxRows("Routes");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiRouteYieldChanges.first, iNumRoutes, iNumYields);
		m_ppiRouteYieldChanges.second = iNumRoutes;
#else
		kUtility.Initialize2DArray(m_ppiRouteYieldChanges, iNumRoutes, iNumYields);
#endif

		std::string strKey = "Improvements - RouteYieldChanges";
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Yields.ID as YieldID, Routes.ID as RouteID, Yield from Improvement_RouteYieldChanges inner join Yields on YieldType = Yields.Type inner join Routes on RouteType = Routes.Type where ImprovementType = ?;");
		}

		pResults->Bind(1, szImprovementType, lenImprovementType, false);

		while(pResults->Step())
		{
			const int yield_idx = pResults->GetInt(0);
			CvAssert(yield_idx > -1);

			const int route_idx = pResults->GetInt(1);
			CvAssert(route_idx > -1);

			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiRouteYieldChanges.first[route_idx][yield_idx] = yield;
#else
			m_ppiRouteYieldChanges[route_idx][yield_idx] = yield;
#endif
		}

		pResults->Reset();

	}

	return true;
}

/// Support for custom improvement icons
#ifdef LEKMOD_CUSTOM_IMPROVEMENT_ICONS
const char* CvImprovementEntry::GetIconString() const
{
	return m_strIconString;
}
//------------------------------------------------------------------------------
void CvImprovementEntry::SetIconString(const char* szVal)
{
	m_strIconString = szVal;
}
#endif

/// The gold maintenance cost
int CvImprovementEntry::GetGoldMaintenance() const
{
	return m_iGoldMaintenance;
}

/// Convert nearby tiles?
int CvImprovementEntry::GetCultureBombRadius() const
{
	return m_iCultureBombRadius;
}

int CvImprovementEntry::GetCultureBombRadiusNeutral() const
{
	return m_iCultureBombRadiusNeutral;
}

/// How many adjacent tiles must be land?
int CvImprovementEntry::GetRequiresXAdjacentLand() const
{
	return m_iRequiresXAdjacentLand;
}

/// Bonus culture if another Improvement of same type is adjacent
int CvImprovementEntry::GetCultureAdjacentSameType() const
{
	return m_iCultureAdjacentSameType;
}

/// The number of tiles in an area needed for a goody hut to be placed by the map generator
int CvImprovementEntry::GetTilesPerGoody() const
{
	return m_iTilesPerGoody;
}

/// How far goody huts need to be away from each other
int CvImprovementEntry::GetGoodyUniqueRange() const
{
	return m_iGoodyUniqueRange;
}

/// How likely this improvement is to expand into an adjacent tile
int CvImprovementEntry::GetFeatureGrowthProbability() const
{
	return m_iFeatureGrowthProbability;
}

/// Amount of time needed before an improvement upgrades to its next state
int CvImprovementEntry::GetUpgradeTime() const
{
	return m_iUpgradeTime;
}

/// Modifier for how much improvements upgrade if they are adjacent to a river
int CvImprovementEntry::GetRiverSideUpgradeMod() const
{
	return m_iRiverSideUpgradeMod;
}

/// Modifier for how much improvements upgrade if they are on the coast
int CvImprovementEntry::GetCoastalLandUpgradeMod() const
{
	return m_iCoastalLandUpgradeMod;
}

/// Modifier for how much improvements upgrade if they are on a hill
int CvImprovementEntry::GetHillsUpgradeMod() const
{
	return m_iHillsUpgradeMod;
}

/// Modifier for how much improvements upgrade if they are exposed to fresh water
int CvImprovementEntry::GetFreshWaterUpgradeMod() const
{
	return m_iFreshWaterUpgradeMod;
}

/// Modifier for the defensive improvement this improvement provides
int CvImprovementEntry::GetDefenseModifier() const
{
	return m_iDefenseModifier;
}

/// Modifier for the defensive improvement this improvement provides Global ~EAP
int CvImprovementEntry::GetDefenseModifierGlobal() const
{
	return m_iDefenseModifierGlobal;
}

/// Damage done to nearby enemy units
int CvImprovementEntry::GetNearbyEnemyDamage() const
{
	return m_iNearbyEnemyDamage;
}

/// How much gold this improvement provides the pillager
int CvImprovementEntry::GetPillageGold() const
{
	return m_iPillageGold;
}

/// Modifier on the amount of resources generated from the tile with this improvement. (100 doubles output)
int CvImprovementEntry::GetResourceExtractionMod() const
{
	return m_iResourceExtractionMod;
}

/// Do we get any copies of the luxury types that the plot's owner has?
int CvImprovementEntry::GetLuxuryCopiesSiphonedFromMinor() const
{
	return m_iLuxuryCopiesSiphonedFromMinor;
}

/// Returns the type of improvement that results from this improvement being pillaged
int CvImprovementEntry::GetImprovementPillage() const
{
	return m_iImprovementPillage;
}

/// Set the type of improvement that results from this improvement being pillaged
void CvImprovementEntry::SetImprovementPillage(int i)
{
	m_iImprovementPillage = i;
}

/// Returns the type of improvement that results from this improvement growing
int CvImprovementEntry::GetImprovementUpgrade() const
{
	return m_iImprovementUpgrade;
}

/// Set the type of improvement that results from this improvement growing
void CvImprovementEntry::SetImprovementUpgrade(int i)
{
	m_iImprovementUpgrade = i;
}

/// Requires hills to be constructed
bool CvImprovementEntry::IsHillsMakesValid() const
{
	return m_bHillsMakesValid;
}
// Requires any body of water to build
bool CvImprovementEntry::IsWaterAdjacencyMakesValid() const
{
	return m_bWaterAdjacencyMakesValid;
}

/// Requires fresh water to build
bool CvImprovementEntry::IsFreshWaterMakesValid() const
{
	return m_bFreshWaterMakesValid;
}

/// Requires being adjacent to a river to build
bool CvImprovementEntry::IsRiverSideMakesValid() const
{
	return m_bRiverSideMakesValid;
}

/// Can't be built next on a tile with fresh water
bool CvImprovementEntry::IsNoFreshWater() const
{
	return m_bNoFreshWater;
}

/// Requires that it must be built on something other than a hill
bool CvImprovementEntry::IsRequiresFlatlands() const
{
	return m_bRequiresFlatlands;
}

/// Requires that it must be built on something other than a hill or next to fresh water
bool CvImprovementEntry::IsRequiresFlatlandsOrFreshWater() const
{
	return m_bRequiresFlatlandsOrFreshWater;
}

/// Is this only built on top of a feature?
bool CvImprovementEntry::IsRequiresFeature() const
{
	return m_bRequiresFeature;
}

/// Is this only built on top of an improvement?
bool CvImprovementEntry::IsRequiresImprovement() const
{
	return m_bRequiresImprovement;
}

/// Does this remove the resource underneath when built?
bool CvImprovementEntry::IsRemovesResource() const
{
	return m_bRemovesResource;
}

/// Do we prompt for a user choice after this is constructed?
bool CvImprovementEntry::IsPromptWhenComplete() const
{
	return m_bPromptWhenComplete;
}

/// Is this only placed out in the water?
bool CvImprovementEntry::IsWater() const
{
	return m_bWater;
}

/// Is this only placed on the coast?
bool CvImprovementEntry::IsCoastal() const
{
	return m_bCoastal;
}


/// Is this a destroyed rather than pillaged?
bool CvImprovementEntry::IsDestroyedWhenPillaged() const
{
	return m_bDestroyedWhenPillaged;
}

/// Is the unit that pillaged this perhaps in an illegal spot?
bool CvImprovementEntry::IsDisplacePillager() const
{
	return m_bDisplacePillager;
}

/// Can this be built on top of Resources?
bool CvImprovementEntry::IsBuildableOnResources() const
{
	return m_bBuildableOnResources;
}

/// Is this a barbarian camp?
bool CvImprovementEntry::IsBarbarianCamp() const
{
	return m_bBarbarianCamp;
}

/// Is this a goody hut?
bool CvImprovementEntry::IsGoody() const
{
	return m_bGoody;
}

/// Is this permanent?
bool CvImprovementEntry::IsPermanent() const
{
	return m_bPermanent;
}

/// Does this improvement need to be built outside of a civ's borders?
bool CvImprovementEntry::IsOutsideBorders() const
{
	return m_bOutsideBorders;
}

/// Can this improvement be built in anyone's lands?
bool CvImprovementEntry::IsIgnoreOwnership() const
{
	return m_bIgnoreOwnership;
}

/// Can this improvement only be built in City-State lands?
bool CvImprovementEntry::IsOnlyCityStateTerritory() const
{
	return m_bOnlyCityStateTerritory;
}

/// Can this improvement not be built adjacent to another one of the same type?
bool CvImprovementEntry::IsNoTwoAdjacent() const
{
	return m_bNoTwoAdjacent;
}

/// Does this improvement need to be built next to a luxury resource?
bool CvImprovementEntry::IsAdjacentLuxury() const
{
	return m_bAdjacentLuxury;
}

/// Does this improvement allows land units to cross water?
bool CvImprovementEntry::IsAllowsWalkWater() const
{
	return m_bAllowsWalkWater;
}
/// Does this improvement allow naval units to cross land?
bool CvImprovementEntry::IsAllowsSailLand() const // from Izy
{
    return m_bAllowsSailLand;
}
/// Does this improvement need to be built inside or adjacent to a civ's borders?
bool CvImprovementEntry::IsInAdjacentFriendly() const
{
	return m_bInAdjacentFriendly;
}

bool CvImprovementEntry::IsCreatedByGreatPerson() const
{
	return m_bCreatedByGreatPerson;
}

bool CvImprovementEntry::IsSpecificCivRequired() const
{
	return m_bSpecificCivRequired;
}

CivilizationTypes CvImprovementEntry::GetRequiredCivilization() const
{
	return m_eRequiredCivilization;
}

/// DEPRECATED
const char* CvImprovementEntry::GetArtDefineTag() const
{
	return m_strArtDefineTag;
}

/// DEPRECATED
void CvImprovementEntry::SetArtDefineTag(const char* szVal)
{
	m_strArtDefineTag = szVal;
}

ImprovementUsageTypes CvImprovementEntry::GetImprovementUsage() const
{
	return m_eImprovementUsageType;
}

void CvImprovementEntry::SetImprovementUsage(const ImprovementUsageTypes usageType)
{
	m_eImprovementUsageType = usageType;
}

/// DEPRECATED
int CvImprovementEntry::GetWorldSoundscapeScriptId() const
{
	return m_iWorldSoundscapeScriptId;
}

/// What resource is required to build this improvement?
int CvImprovementEntry::GetResourceQuantityRequirement(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piResourceQuantityRequirements ? m_piResourceQuantityRequirements[i] : -1;
}

/// How much of a resource yield is required before this improvement can be built
int CvImprovementEntry::GetPrereqNatureYield(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piPrereqNatureYield ? m_piPrereqNatureYield[i] : -1;
}

int* CvImprovementEntry::GetPrereqNatureYieldArray()
{
	return m_piPrereqNatureYield;
}

/// How much this improvement improves a certain yield
int CvImprovementEntry::GetYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChange ? m_piYieldChange[i] : 0;
}

int* CvImprovementEntry::GetYieldChangeArray()
{
	return m_piYieldChange;
}

/// How much this improvement improves a certain yield for each era of age
int CvImprovementEntry::GetYieldChangePerEra(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldPerEra ? m_piYieldPerEra[i] : 0;
}

/// How much being next to a river improves the yield of this improvement
int CvImprovementEntry::GetRiverSideYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piRiverSideYieldChange ? m_piRiverSideYieldChange[i] : 0;
}

int* CvImprovementEntry::GetRiverSideYieldChangeArray()
{
	return m_piRiverSideYieldChange;
}

/// How much being on a coastal tile improves the yield of this improvement
int CvImprovementEntry::GetCoastalLandYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piCoastalLandYieldChange ? m_piCoastalLandYieldChange[i] : 0;
}

int* CvImprovementEntry::GetCoastalLandYieldChangeArray()
{
	return m_piCoastalLandYieldChange;
}

/// How much being on a hill tile improves the yield of this improvement
int CvImprovementEntry::GetHillsYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piHillsYieldChange ? m_piHillsYieldChange[i] : 0;
}

int* CvImprovementEntry::GetHillsYieldChangeArray()
{
	return m_piHillsYieldChange;
}

/// How much having access to fresh water improves the yield of this improvement
int CvImprovementEntry::GetFreshWaterYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piFreshWaterChange ? m_piFreshWaterChange[i] : 0;
}

int* CvImprovementEntry::GetFreshWaterYieldChangeArray() // For Moose - CvWidgetData XXX
{
	return m_piFreshWaterChange;
}

/// How much being adjacent to a city improves the yield of this improvement
int CvImprovementEntry::GetAdjacentCityYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piAdjacentCityYieldChange ? m_piAdjacentCityYieldChange[i] : 0;
}

int* CvImprovementEntry::GetAdjacentCityYieldChangeArray()
{
	return m_piAdjacentCityYieldChange;
}

/// How much being adjacent to a mountain improves the yield of this improvement
int CvImprovementEntry::GetAdjacentMountainYieldChange(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piAdjacentMountainYieldChange ? m_piAdjacentMountainYieldChange[i] : 0;
}

int* CvImprovementEntry::GetAdjacentMountainYieldChangeArray()
{
	return m_piAdjacentMountainYieldChange;
}

/// If this improvement requires a terrain type to be valid
bool CvImprovementEntry::GetTerrainMakesValid(int i) const
{
	CvAssertMsg(i < GC.getNumTerrainInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_pbTerrainMakesValid ? m_pbTerrainMakesValid[i] : false;
}

/// If this improvement requires a feature to be valid
bool CvImprovementEntry::GetFeatureMakesValid(int i) const
{
	CvAssertMsg(i < GC.getNumFeatureInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_pbFeatureMakesValid ? m_pbFeatureMakesValid[i] : false;
}

/// If this improvement requires a different improvement to be valid
bool CvImprovementEntry::GetImprovementMakesValid(int i) const
{
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_pbImprovementMakesValid ? m_pbImprovementMakesValid[i] : false;
}

/// How much a tech improves the yield of this improvement
int CvImprovementEntry::GetTechYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumTechInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechYieldChanges.first ? m_ppiTechYieldChanges.first[i][j] : 0;
#else
	return m_ppiTechYieldChanges[i][j];
#endif
}

int* CvImprovementEntry::GetTechYieldChangesArray(int i)
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechYieldChanges.first ? m_ppiTechYieldChanges.first[i] : NULL;
#else
	return m_ppiTechYieldChanges[i];
#endif
}

#ifdef LEKMOD_ADJACENT_IMPROVEMENT_YIELD
/// How much a type of improvement adjacent to this improvement improves the yield of this improvement
int CvImprovementEntry::GetImprovementAdjacentBonus(int i, int j) const
{
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiImprovementAdjacentBonus[i][j];
}

int CvImprovementEntry::GetImprovementAdjacentBonusCivilization(int i, int j) const
{	
	CvAssertMsg(i < 200, "Index out of bounds");
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piImprovementAdjacentBonusCivilization[i][j];
}

// what improvement type is required to be adjacent to this improvement to get the yield bonus
int CvImprovementEntry::GetImprovementAdjacentCivilizationAmount(int i, int j) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < 6, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiImprovementAdjacentCivilizationAmount[i][j];
}

int CvImprovementEntry::GetImprovementAdjacentBonusCivilizationNoAmount(int i, int j) const
{
	CvAssertMsg(i < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiImprovementAdjacentBonusCivilizationNoAmount[i][j];
}

// what improvement type is required to be adjacent to this improvement to get the yield bonus
int CvImprovementEntry::GetImprovementAdjacentAmount(int i, int j) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < 6, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiImprovementAdjacentAmount[i][j];
}
// New method to trigger the visual changes when nessesary
bool CvImprovementEntry::HasAnyAdjacencyYieldBonus() const
{
	for (int i = 0; i < GC.getNumImprovementInfos(); ++i)
	{
		for (int j = 0; j < NUM_YIELD_TYPES; ++j) // Yield loop
		{
			if (GetImprovementAdjacentBonus(i, j) > 0)
				return true;
			if (GetImprovementAdjacentBonusCivilization(i, j) > 0)
				return true;
			if (GetImprovementAdjacentBonusCivilizationNoAmount(i, j) > 0)
				return true;
		}
	}

	for (int i = 0; i < NUM_YIELD_TYPES; ++i)
	{
		for (int j = 0; j < 6; ++j) // Civilization loop
		{
			if (GetImprovementAdjacentAmount(i, j) > 0)
				return true;
			if (GetImprovementAdjacentCivilizationAmount(i, j) > 0)
				return true;
		}
	}

	return false;
}
#endif
/// How much a tech improves the yield of this improvement if it DOES NOT have fresh water
int CvImprovementEntry::GetTechNoFreshWaterYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumTechInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechNoFreshWaterYieldChanges.first ? m_ppiTechNoFreshWaterYieldChanges.first[i][j] : 0;
#else
	return m_ppiTechNoFreshWaterYieldChanges[i][j];
#endif
}

int* CvImprovementEntry::GetTechNoFreshWaterYieldChangesArray(int i)
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechNoFreshWaterYieldChanges.first ? m_ppiTechNoFreshWaterYieldChanges.first[i] : NULL;
#else
	return m_ppiTechNoFreshWaterYieldChanges[i];
#endif
}

/// How much a tech improves the yield of this improvement if it has fresh water
int CvImprovementEntry::GetTechFreshWaterYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumTechInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechFreshWaterYieldChanges.first ? m_ppiTechFreshWaterYieldChanges.first[i][j] : 0;
#else
	return m_ppiTechFreshWaterYieldChanges[i][j];
#endif
}

int* CvImprovementEntry::GetTechFreshWaterYieldChangesArray(int i)
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiTechFreshWaterYieldChanges.first ? m_ppiTechFreshWaterYieldChanges.first[i] : NULL;
#else
	return m_ppiTechFreshWaterYieldChanges[i];
#endif
}

/// How much a type of route improves the yield of this improvement
int CvImprovementEntry::GetRouteYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumRouteInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiRouteYieldChanges.first ? m_ppiRouteYieldChanges.first[i][j] : 0;
#else
	return m_ppiRouteYieldChanges[i][j];
#endif
}

int* CvImprovementEntry::GetRouteYieldChangesArray(int i)				// For Moose - CvWidgetData XXX
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiRouteYieldChanges.first ? m_ppiRouteYieldChanges.first[i] : NULL;
#else
	return m_ppiRouteYieldChanges[i];
#endif
}

/// How much a yield improves when a resource is present with the improvement
int CvImprovementEntry::GetImprovementResourceYield(int i, int j) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_paImprovementResource[i].m_piYieldChange ? m_paImprovementResource[i].getYieldChange(j) : 0;
}

/// What resources does this improvement require to be built
bool CvImprovementEntry::IsImprovementResourceMakesValid(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paImprovementResource[i].m_bResourceMakesValid;
}

/// Does this improvement enable a tradeable resource
bool CvImprovementEntry::IsImprovementResourceTrade(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paImprovementResource[i].m_bResourceTrade;
}

/// the chance of the specified Resource appearing randomly when the Improvement is present with no current Resource
int CvImprovementEntry::GetImprovementResourceDiscoverRand(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paImprovementResource[i].m_iDiscoverRand;
}

/// Gets the flavor value of the improvement
int CvImprovementEntry::GetFlavorValue(int i) const
{
	CvAssertMsg(i < GC.getNumFlavorTypes(), "Index out of bounds");
	CvAssertMsg(i > -1, "Indes out of bounds");
	return m_piFlavorValue[i];
}


//=====================================
// CvPromotionEntryXMLEntries
//=====================================
/// Constructor
CvImprovementXMLEntries::CvImprovementXMLEntries(void)
{

}

/// Destructor
CvImprovementXMLEntries::~CvImprovementXMLEntries(void)
{
	DeleteArray();
}

/// Returns vector of improvement entries
std::vector<CvImprovementEntry*>& CvImprovementXMLEntries::GetImprovementEntries()
{
	return m_paImprovementEntries;
}

/// Number of defined projects
#ifdef AUI_WARNING_FIXES
uint CvImprovementXMLEntries::GetNumImprovements() const
#else
int CvImprovementXMLEntries::GetNumImprovements()
#endif
{
	return m_paImprovementEntries.size();
}

/// Get a specific entry
#ifdef AUI_WARNING_FIXES
_Ret_maybenull_ CvImprovementEntry* CvImprovementXMLEntries::GetEntry(uint index)
#else
CvImprovementEntry* CvImprovementXMLEntries::GetEntry(int index)
#endif
{
	return m_paImprovementEntries[index];
}

/// Tell which improvement unlocks a resource
CvImprovementEntry* CvImprovementXMLEntries::GetImprovementForResource(int eResource)
{
	for(unsigned int iImprovement = 0; iImprovement < m_paImprovementEntries.size(); ++iImprovement)
	{
		CvImprovementEntry* pImprovement = GetEntry((ImprovementTypes)iImprovement);
		if(pImprovement && pImprovement->IsImprovementResourceMakesValid(eResource))
		{
			return pImprovement;
		}
	}

	return NULL;
}

/// Clear improvement entries
void CvImprovementXMLEntries::DeleteArray()
{
	for(std::vector<CvImprovementEntry*>::iterator it = m_paImprovementEntries.begin(); it != m_paImprovementEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paImprovementEntries.clear();
}

/// Helper function to read in an integer array of data sized according to number of building types
void ImprovementArrayHelpers::Read(FDataStream& kStream, int* paiImprovementArray)
{
	int iNumEntries;

	kStream >> iNumEntries;

	int iArraySize = GC.getNumImprovementInfos();
	for(int iI = 0; iI < iNumEntries; iI++)
	{
		uint uiHash;
		kStream >> uiHash;
		if (uiHash != 0 && uiHash != (uint)NO_IMPROVEMENT)
		{
			int iType = GC.getInfoTypeForHash(uiHash);
			if(iType != -1 && iType < iArraySize)
			{
				kStream >> paiImprovementArray[iType];
			}
			else
			{
				CvString szError;
				szError.Format("LOAD ERROR: Improvement Type not found");
				GC.LogMessage(szError.GetCString());
				CvAssertMsg(false, szError);

				int iDummy;
				kStream >> iDummy;
			}
		}
	}
}

/// Helper function to write out an integer array of data sized according to number of improvement types
void ImprovementArrayHelpers::Write(FDataStream& kStream, int* paiImprovementArray, int iArraySize)
{
	kStream << iArraySize;

	for(int iI = 0; iI < iArraySize; iI++)
	{
		const ImprovementTypes eImprovement = static_cast<ImprovementTypes>(iI);
		CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(eImprovement);
		if(pkImprovementInfo)
		{
			CvInfosSerializationHelper::WriteHashed(kStream, pkImprovementInfo);
			kStream << paiImprovementArray[iI];
		}
		else
		{
			kStream << (int)NO_IMPROVEMENT;
		}
	}
}

/// Helper function to read in an integer array of data sized according to number of building types
void ImprovementArrayHelpers::ReadYieldArray(FDataStream& kStream, int** ppaaiImprovementYieldArray, int iNumYields)
{
	int iNumEntries;

	kStream >> iNumEntries;

	for(int iI = 0; iI < iNumEntries; iI++)
	{
		int iHash;
		kStream >> iHash;
		if(iHash != (int)0)
		{
			int iType = GC.getInfoTypeForHash(iHash);
			if(iType != -1)
			{
				for(int jJ = 0; jJ < iNumYields; jJ++)
				{
					kStream >> ppaaiImprovementYieldArray[iType][jJ];
				}
			}
			else
			{
				CvString szError;
				szError.Format("LOAD ERROR: Improvement Type not found: %08x", iHash);
				GC.LogMessage(szError.GetCString());
				CvAssertMsg(false, szError);

				for(int jJ = 0; jJ < iNumYields; jJ++)
				{
					int iDummy;
					kStream >> iDummy;
				}
			}
		}
	}
}

/// Helper function to write out an integer array of data sized according to number of improvement types
void ImprovementArrayHelpers::WriteYieldArray(FDataStream& kStream, int** ppaaiImprovementYieldArray, int iArraySize)
{
	kStream << iArraySize;

	for(int iI = 0; iI < iArraySize; iI++)
	{
		const ImprovementTypes eImprovement = static_cast<ImprovementTypes>(iI);
		CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(eImprovement);
		if(pkImprovementInfo)
		{
			CvInfosSerializationHelper::WriteHashed(kStream, pkImprovementInfo);
			for(int jJ = 0; jJ < NUM_YIELD_TYPES; jJ++)
			{
				kStream << ppaaiImprovementYieldArray[iI][jJ];
			}
		}
		else
		{
			kStream << (int)0;
		}
	}
}
