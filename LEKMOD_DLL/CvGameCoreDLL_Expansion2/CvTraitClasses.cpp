/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "ICvDLLUserInterface.h"
#include "CvGameCoreUtils.h"
#include "CvInfosSerializationHelper.h"
#include "CvDiplomacyAI.h"
#include "CvGrandStrategyAI.h"

#include "LintFree.h"

//======================================================================================================
//					CvTraitEntry
//======================================================================================================
/// Constructor
CvTraitEntry::CvTraitEntry() :
	m_iLevelExperienceModifier(0),
	m_iGreatPeopleRateModifier(0),
	m_iGreatScientistRateModifier(0),
	m_iGreatGeneralRateModifier(0),
	m_iGreatGeneralExtraBonus(0),
	m_iGreatPersonGiftInfluence(0),
	m_iMaxGlobalBuildingProductionModifier(0),
	m_iMaxTeamBuildingProductionModifier(0),
	m_iMaxPlayerBuildingProductionModifier(0),
	m_iCityUnhappinessModifier(0),
	m_iPopulationUnhappinessModifier(0),
	m_iCityStateBonusModifier(0),
	m_iCityStateFriendshipModifier(0),
	m_iCityStateCombatModifier(0),
	m_iLandBarbarianConversionPercent(0),
	m_iLandBarbarianConversionExtraUnits(0),
	m_iSeaBarbarianConversionPercent(0),
	m_iCapitalBuildingModifier(0),
	m_iPlotBuyCostModifier(0),
	m_iPlotCultureCostModifier(0),
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	m_iSciencePerGreatPersonBorn(0),
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	m_iNumTurnsBeforeMinorAlliesRefuseBribes(0),
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	m_iGoldenAgeTileBonusFaith(0),
#endif
	m_iCultureFromKills(0),
	m_iFaithFromKills(0),
	m_iCityCultureBonus(0),
	m_iCapitalThemingBonusModifier(0),
	m_iPolicyCostModifier(0),
	m_iCityConnectionTradeRouteChange(0),
	m_iWonderProductionModifier(0),
	m_iPlunderModifier(0),
	m_iImprovementMaintenanceModifier(0),
	m_iGoldenAgeDurationModifier(0),
	m_iGoldenAgeMoveChange(0),
	m_iGoldenAgeCombatModifier(0),
	m_iGoldenAgeTourismModifier(0),
	m_iGoldenAgeGreatArtistRateModifier(0),
	m_iGoldenAgeGreatMusicianRateModifier(0),
	m_iGoldenAgeGreatWriterRateModifier(0),
	m_iObsoleteTech(NO_TECH),
	m_iPrereqTech(NO_TECH),
	m_iExtraEmbarkMoves(0),
	m_iFreeUnitClassType(NO_UNITCLASS),
	m_iNaturalWonderFirstFinderGold(0),
	m_iNaturalWonderSubsequentFinderGold(0),

#if defined(TRAITIFY) // Constructor, int and bools
	m_bHalfMoreSpecialistUnhappiness(false),

	m_iGoldenAgeCultureModifier(0),
	m_iNumExtraLeagueVotes(0),
	m_iNumTradeRouteBonus(0),
	m_iMinorFriendshipMinimum(0),
	m_iGreatEngineerRateModifier(0),
	m_iGreatMerchantRateModifier(0),
	m_iMinorBullyModifier(0),
	m_iExtraPopulation(0),
	m_iInternationalRouteGrowthModifier(0),
	m_iLocalHappinessPerCity(0),
	m_iInternalTradeRouteYieldModifier(0),
	m_iUnhappinessModifierForPuppetedCities(0),
	m_iFaithCostModifier(0),
	m_iIdeologyPressureUnhappinessModifier(0),
	m_iForeignRelgionPressureModifier(0),
	m_iFriendlyLandsCitizenMoveChange(0),
#endif
#if defined(LEKMOD_v34)
	m_bReligionEnhanceReformation(false),

	m_iSelfReligiousPressureModifier(0),
	m_iLandTradeRouteYieldBonus(0),
#endif

	//EAP: Natural wonder faith for the finder
	m_iNaturalWonderFirstFinderFaith(0),
	m_iNaturalWonderSubsequentFinderFaith(0),

	m_iNaturalWonderYieldModifier(0),
	m_iNaturalWonderHappinessModifier(0),
	m_iNearbyImprovementCombatBonus(0),
	m_iNearbyImprovementBonusRange(0),
	m_iCultureBuildingYieldChange(0),
	m_iCombatBonusVsHigherTech(0),
	m_iCombatBonusVsLargerCiv(0),
	m_iLandUnitMaintenanceModifier(0),
	m_iNavalUnitMaintenanceModifier(0),
	m_iRazeSpeedModifier(0),
	m_iDOFGreatPersonModifier(0),
	m_iLuxuryHappinessRetention(0),
	m_iExtraHappinessPerLuxury(0), // NQMP GJS - New Netherlands UA
	m_iExtraSpies(0),
	m_iHappinessPerReligion(0), // NQMP GJS - New Ottoman UA
	m_iUnresearchedTechBonusFromKills(0),
	m_iExtraFoundedCityTerritoryClaimRange(0),
	m_iFreeSocialPoliciesPerEra(0),
	m_iNumTradeRoutesModifier(0),
	m_iTradeRouteResourceModifier(0),
	m_iUniqueLuxuryCities(0),
	m_iUniqueLuxuryQuantity(0),

	m_eFreeUnitPrereqTech(NO_TECH),
	m_eFreeBuilding(NO_BUILDING),

	m_eFreeCapitalBuilding(NO_BUILDING),
	m_eFreeBuildingPrereqTech(NO_TECH),
	m_eCapitalFreeBuildingPrereqTech(NO_TECH),

	m_eFreeBuildingOnConquest(NO_BUILDING),
	m_bFightWellDamaged(false),
	m_bMoveFriendlyWoodsAsRoad(false),
	m_bFasterAlongRiver(false),
	m_bFasterInHills(false),
	m_bEmbarkedAllWater(false),
	m_bEmbarkedToLandFlatCost(false),
	m_bNoHillsImprovementMaintenance(false),
	m_bTechBoostFromCapitalScienceBuildings(false),
	m_bStaysAliveZeroCities(false),
	m_bFaithFromUnimprovedForest(false),
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	m_bSeaTradeRoutesArePlunderImmune(false),
#endif

	// NQMP GJS - New France UA begin
	m_bEarnsGreatPersonOnSlotOrGuild(false),
	m_bHasBuiltWritersGuild(false),
	m_bHasBuiltArtistsGuild(false),
	m_bHasBuiltMusiciansGuild(false),
	// NQMP GJS - New France UA end

	m_bBonusReligiousBelief(false),
	m_bAbleToAnnexCityStates(false),
	m_bCrossesMountainsAfterGreatGeneral(false),
	m_bMayaCalendarBonuses(false),
	m_bNoAnnexing(false),
	m_bTechFromCityConquer(false),
	m_bUniqueLuxuryRequiresNewArea(false),

#if defined(TRAITIFY) // Constructor, Arrays
	m_ppiBuildingCostOverride(NULL),
	m_ppiBuildingClassYieldChanges(NULL),
	m_piPuppetYieldModifiers(NULL),
	m_paiRouteMovementChange(NULL),
	m_ppiResourceClassYieldChanges(NULL),
	m_ppiFeatureYieldChanges(NULL),
	m_ppiTerrainYieldChanges(NULL),
	m_ppiResourceYieldChanges(NULL),

	m_paiBuildingClassGlobalHappiness(NULL),
	m_paiBuildingClassHappiness(NULL),
#endif
#if defined(LEKMOD_v34)
	m_paiYieldPerPopulation(NULL),
	m_paiYieldPerPopulationForeignReligion(NULL),
#endif
#if defined(FULL_YIELD_FROM_KILLS)
	m_paiYieldFromKills(NULL),
#endif
	m_paiExtraYieldThreshold(NULL),
	m_paiYieldChange(NULL),
	m_paiYieldChangeStrategicResources(NULL),
	m_paiYieldChangeLuxuryResources(NULL), // NQMP GJS - New Netherlands UA
	m_paiYieldChangeNaturalWonder(NULL),
	m_paiYieldChangePerTradePartner(NULL),
	m_paiYieldChangeIncomingTradeRoute(NULL),
	m_paiYieldModifier(NULL),
	m_piStrategicResourceQuantityModifier(NULL),
	m_piResourceQuantityModifiers(NULL),
#ifdef AUI_WARNING_FIXES
	m_piMovesChangeUnitCombats(NULL),
	m_piMaintenanceModifierUnitCombats(NULL),
	m_iWorkerSpeedModifier(0),
	m_iAfraidMinorPerTurnInfluence(0),
	m_iLandTradeRouteRangeBonus(0),
	m_iTradeReligionModifier(0),
	m_iTradeBuildingModifier(0),
	m_eCombatBonusImprovement(NO_IMPROVEMENT),
	m_bRiverTradeRoad(false),
	m_bAngerFreeIntrusionOfCityStates(false),
#endif
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	m_ppiImprovementYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiSpecialistYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiUnimprovedFeatureYieldChanges(std::pair<int**, size_t>(NULL, 0))
#else
	m_ppiImprovementYieldChanges(NULL),
	m_ppiSpecialistYieldChanges(NULL),
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	m_ppiAnySpecificSpecialistYieldChanges(NULL),
#endif
	m_ppiUnimprovedFeatureYieldChanges(NULL)
#endif
{
}

/// Destructor
CvTraitEntry::~CvTraitEntry()
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldChanges.first, m_ppiImprovementYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiSpecialistYieldChanges.first, m_ppiSpecialistYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiUnimprovedFeatureYieldChanges.first, m_ppiUnimprovedFeatureYieldChanges.second);
#else
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiSpecialistYieldChanges);
#if defined(TRAITIFY) // Destructor, Arrays
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingCostOverride);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiResourceClassYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiFeatureYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiResourceYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiTerrainYieldChanges);
	SAFE_DELETE_ARRAY(m_piPuppetYieldModifiers);
	SAFE_DELETE_ARRAY(m_paiRouteMovementChange);
	SAFE_DELETE_ARRAY(m_paiBuildingClassGlobalHappiness);
	SAFE_DELETE_ARRAY(m_paiBuildingClassHappiness);
#endif
#if defined(FULL_YIELD_FROM_KILLS)
	SAFE_DELETE_ARRAY(m_paiYieldFromKills);
#endif
#if defined(LEKMOD_v34)
	SAFE_DELETE_ARRAY(m_paiYieldPerPopulation);
	SAFE_DELETE_ARRAY(m_paiYieldPerPopulationForeignReligion);
#endif
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	CvDatabaseUtility::SafeDelete2DArray(m_ppiAnySpecificSpecialistYieldChanges);
#endif
	CvDatabaseUtility::SafeDelete2DArray(m_ppiUnimprovedFeatureYieldChanges);
#endif
#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	// Clean up build time override arrays
	m_aiBuildTimeOverride.clear();
	m_aiBuildTimeOverrideResourceClassRequired.clear();
	m_BuildTimeOverrides.clear();
#endif
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_paiExtraYieldThreshold);
	SAFE_DELETE_ARRAY(m_paiYieldChange);
	SAFE_DELETE_ARRAY(m_paiYieldChangeStrategicResources);
	SAFE_DELETE_ARRAY(m_paiYieldChangeLuxuryResources); // NQMP GJS - New Netherlands UA
	SAFE_DELETE_ARRAY(m_paiYieldChangeNaturalWonder);
	SAFE_DELETE_ARRAY(m_paiYieldChangePerTradePartner);
	SAFE_DELETE_ARRAY(m_paiYieldChangeIncomingTradeRoute);
	SAFE_DELETE_ARRAY(m_paiYieldModifier);
	SAFE_DELETE_ARRAY(m_piStrategicResourceQuantityModifier);
	SAFE_DELETE_ARRAY(m_piResourceQuantityModifiers);
	SAFE_DELETE_ARRAY(m_piMovesChangeUnitCombats);
	SAFE_DELETE_ARRAY(m_piMaintenanceModifierUnitCombats);
#endif
}

/// Accessor:: Modifier to experience needed for new level
int CvTraitEntry::GetLevelExperienceModifier() const
{
	return m_iLevelExperienceModifier;
}

/// Accessor:: Great person generation rate change
int CvTraitEntry::GetGreatPeopleRateModifier() const
{
	return m_iGreatPeopleRateModifier;
}

/// Accessor:: Great scientist generation rate change
int CvTraitEntry::GetGreatScientistRateModifier() const
{
	return m_iGreatScientistRateModifier;
}

/// Accessor:: Great general generation rate change
int CvTraitEntry::GetGreatGeneralRateModifier() const
{
	return m_iGreatGeneralRateModifier;
}

/// Accessor:: Great general additional combat bonus
int CvTraitEntry::GetGreatGeneralExtraBonus() const
{
	return m_iGreatGeneralExtraBonus;
}

/// Accessor:: Influence gained for gifting GP to city state
int CvTraitEntry::GetGreatPersonGiftInfluence() const
{
	return m_iGreatPersonGiftInfluence;
}

/// Accessor:: Overall production boost
int CvTraitEntry::GetMaxGlobalBuildingProductionModifier() const
{
	return m_iMaxGlobalBuildingProductionModifier;
}

/// Accessor:: Team production boost
int CvTraitEntry::GetMaxTeamBuildingProductionModifier() const
{
	return m_iMaxTeamBuildingProductionModifier;
}

/// Accessor:: Player production boost
int CvTraitEntry::GetMaxPlayerBuildingProductionModifier() const
{
	return m_iMaxPlayerBuildingProductionModifier;
}

/// Accessor:: City unhappiness change
int CvTraitEntry::GetCityUnhappinessModifier() const
{
	return m_iCityUnhappinessModifier;
}

/// Accessor:: Unhappiness change based on overall population
int CvTraitEntry::GetPopulationUnhappinessModifier() const
{
	return m_iPopulationUnhappinessModifier;
}

/// Accessor:: percent boost in value of city state bonuses
int CvTraitEntry::GetCityStateBonusModifier() const
{
	return m_iCityStateBonusModifier;
}

/// Accessor:: percent boost in value of city state bonuses
int CvTraitEntry::GetCityStateFriendshipModifier() const
{
	return m_iCityStateFriendshipModifier;
}

/// Accessor:: percent boost in value of city state bonuses
int CvTraitEntry::GetCityStateCombatModifier() const
{
	return m_iCityStateCombatModifier;
}

/// Accessor:: percent chance a barbarian camp joins this civ
int CvTraitEntry::GetLandBarbarianConversionPercent() const
{
	return m_iLandBarbarianConversionPercent;
}

/// Accessor:: extra copies of the camp guard created if it joins this civ
int CvTraitEntry::GetLandBarbarianConversionExtraUnits() const
{
	return m_iLandBarbarianConversionExtraUnits;
}

/// Accessor:: percent chance a barbarian naval unit joins this civ
int CvTraitEntry::GetSeaBarbarianConversionPercent() const
{
	return m_iSeaBarbarianConversionPercent;
}

/// Accessor:: discount when constructing a building that is already present in capital
int CvTraitEntry::GetCapitalBuildingModifier() const
{
	return m_iCapitalBuildingModifier;
}

/// Accessor:: cheaper purchase of tiles for culture border expansion
int CvTraitEntry::GetPlotBuyCostModifier() const
{
	return m_iPlotBuyCostModifier;
}

/// Accessor:: increased rate of culture border expansion
int CvTraitEntry::GetPlotCultureCostModifier() const
{
	return m_iPlotCultureCostModifier;
}

#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
/// Accessor:: science from great person spawning
int CvTraitEntry::GetSciencePerGreatPersonBorn() const
{
	return m_iSciencePerGreatPersonBorn;
}
#endif

#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
/// Accessor:: num turns before minor allies refuse bribes
int CvTraitEntry::GetNumTurnsBeforeMinorAlliesRefuseBribes() const
{
	return m_iNumTurnsBeforeMinorAlliesRefuseBribes;
}
#endif

#ifdef NQ_GOLDEN_PILGRIMAGE
/// Accessor:: during golden ages, tiles that get bonus gold also get this much bonus faith
int CvTraitEntry::GetGoldenAgeTileBonusFaith() const
{
	return m_iGoldenAgeTileBonusFaith;
}
#endif


/// Accessor:: culture for kills
int CvTraitEntry::GetCultureFromKills() const
{
	return m_iCultureFromKills;
}

/// Accessor:: faith for kills
int CvTraitEntry::GetFaithFromKills() const
{
	return m_iFaithFromKills;
}

/// Accessor:: extra culture from buildings that provide culture
int CvTraitEntry::GetCityCultureBonus() const
{
	return m_iCityCultureBonus;
}

/// Accessor:: boost to theming bonuses in capital
int CvTraitEntry::GetCapitalThemingBonusModifier() const
{
	return m_iCapitalThemingBonusModifier;
}

/// Accessor:: discount on buying policies
int CvTraitEntry::GetPolicyCostModifier() const
{
	return m_iPolicyCostModifier;
}

/// Accessor:: extra money from trade routes
int CvTraitEntry::GetCityConnectionTradeRouteChange() const
{
	return m_iCityConnectionTradeRouteChange;
}

/// Accessor:: boost in wonder building speed
int CvTraitEntry::GetWonderProductionModifier() const
{
	return m_iWonderProductionModifier;
}

/// Accessor:: boost in gold received from cities and barbarian camps
int CvTraitEntry::GetPlunderModifier() const
{
	return m_iPlunderModifier;
}

/// Accessor:: percent change to cost of improvement maintenance
int CvTraitEntry::GetImprovementMaintenanceModifier() const
{
	return m_iImprovementMaintenanceModifier;
}

/// Accessor:: extra length of each golden age
int CvTraitEntry::GetGoldenAgeDurationModifier() const
{
	return m_iGoldenAgeDurationModifier;
}

/// Accessor:: movement bonus during golden ages
int CvTraitEntry::GetGoldenAgeMoveChange() const
{
	return m_iGoldenAgeMoveChange;
}

/// Accessor:: combat bonus during golden ages
int CvTraitEntry::GetGoldenAgeCombatModifier() const
{
	return m_iGoldenAgeCombatModifier;
}

/// Accessor:: tourism bonus during golden ages
int CvTraitEntry::GetGoldenAgeTourismModifier() const
{
	return m_iGoldenAgeTourismModifier;
}

/// Accessor:: artist bonus during golden ages
int CvTraitEntry::GetGoldenAgeGreatArtistRateModifier() const
{
	return m_iGoldenAgeGreatArtistRateModifier;
}

/// Accessor:: musician bonus during golden ages
int CvTraitEntry::GetGoldenAgeGreatMusicianRateModifier() const
{
	return m_iGoldenAgeGreatMusicianRateModifier;
}

/// Accessor:: writer bonus during golden ages
int CvTraitEntry::GetGoldenAgeGreatWriterRateModifier() const
{
	return m_iGoldenAgeGreatWriterRateModifier;
}

/// Accessor:: combat bonus during golden ages
int CvTraitEntry::GetExtraEmbarkMoves() const
{
	return m_iExtraEmbarkMoves;
}

/// Accessor: what unit does this trait provide?
int CvTraitEntry::GetFreeUnitClassType() const
{
	return m_iFreeUnitClassType;
}

/// Accessor:: bonus gold for being first to find a natural wonder
int CvTraitEntry::GetNaturalWonderFirstFinderGold() const
{
	return m_iNaturalWonderFirstFinderGold;
}

/// Accessor:: bonus gold for being first to find a natural wonder
int CvTraitEntry::GetNaturalWonderSubsequentFinderGold() const
{
	return m_iNaturalWonderSubsequentFinderGold;
}

#if defined(TRAITIFY) // Int and Bool Accessors
/// Accessor:: whether this trait has a half more specialist unhappiness
bool CvTraitEntry::IsHalfMoreSpecialistUnhappiness() const
{
	return m_bHalfMoreSpecialistUnhappiness;
}
/// Accessor:: does this trait give a culture modifier during golden ages?
int CvTraitEntry::GetGoldenAgeCultureModifier() const
{
	return m_iGoldenAgeCultureModifier;
}
/// Accessor:: does this trait give extra league votes?
int CvTraitEntry::GetNumExtraLeagueVotes() const
{
	return m_iNumExtraLeagueVotes;
}
/// Accessor:: does this trait give more trade routes?
int CvTraitEntry::GetNumTradeRouteBonus() const
{
	return m_iNumTradeRouteBonus;
}
/// Accessor:: does this trait increase the minimum friendship with city states?
int CvTraitEntry::GetMinorFriendshipMinimum() const
{
	return m_iMinorFriendshipMinimum;
}
/// Accessor:: does this trait give a bonus to great engineer points?
int CvTraitEntry::GetGreatEngineerRateModifier() const
{
	return m_iGreatEngineerRateModifier;
}
/// Accessor:: does this trait give a bonus to great merchant points?
int CvTraitEntry::GetGreatMerchantRateModifier() const
{
	return m_iGreatMerchantRateModifier;
}
/// Accessor:: does this trait give a bonus to tributing city states?
int CvTraitEntry::GetMinorBullyModifier() const
{
	return m_iMinorBullyModifier;
}
/// Accessor:: does this trait give extra population?
int CvTraitEntry::GetExtraPopulation() const
{
	return m_iExtraPopulation;
}
/// Accessor:: does this trait give a bonus to international trade routes?
int CvTraitEntry::GetInternationalRouteGrowthModifier() const
{
	return m_iInternationalRouteGrowthModifier;
}
/// Accessor:: does this trait give a bonus to local happiness per city?
int CvTraitEntry::GetLocalHappinessPerCity() const
{
	return m_iLocalHappinessPerCity;
}
/// Accessor:: does this trait give a bonus to internal trade route yield?
int CvTraitEntry::GetInternalTradeRouteYieldModifier() const
{
	return m_iInternalTradeRouteYieldModifier;
}
/// Accessor:: does this trait give a bonus to unhappiness from puppeted cities?
int CvTraitEntry::GetUnhappinessModifierForPuppetedCities() const
{
	return m_iUnhappinessModifierForPuppetedCities;
}
/// Accessor:: does this trait give a discount/increase to faith cost?
int CvTraitEntry::GetFaithCostModifier() const
{
	return m_iFaithCostModifier;
}
/// Accessor:: does this trait give a bonus to ideology pressure unhappiness?
int CvTraitEntry::GetIdeologyPressureUnhappinessModifier() const
{
	return m_iIdeologyPressureUnhappinessModifier;
}
/// Accessor:: does this trait give a bonus to foreign religion pressure?
int CvTraitEntry::GetForeignRelgionPressureModifier() const
{
	return m_iForeignRelgionPressureModifier;
}
/// Accessor:: does this trait give additional movement to civilian units in friendly lands?
int CvTraitEntry::GetFriendlyLandsCitizenMoveChange() const
{
	return m_iFriendlyLandsCitizenMoveChange;
}
#endif
#if defined(LEKMOD_v34)
/// Accessor:: does this trait give a bonus to religious reformation?
bool CvTraitEntry::IsReligionEnhanceReformation() const
{
	return m_bReligionEnhanceReformation;
}
/// Accessor:: does this trait give a bonus to self religious pressure?
int CvTraitEntry::GetSelfReligiousPressureModifier() const
{
	return m_iSelfReligiousPressureModifier;
}
/// Accessor:: does this trait give a land trade route yield bonus?
int CvTraitEntry::GetLandTradeRouteYieldBonus() const
{
	return m_iLandTradeRouteYieldBonus;
}
#endif
///////////////////
//EAP: Faith for finding a Natural Wonder
// Accessor:: bonus faith for being first to find a natural wonder
int CvTraitEntry::GetNaturalWonderFirstFinderFaith() const
{
	return m_iNaturalWonderFirstFinderFaith;
}

/// Accessor:: bonus faith for being first to find a natural wonder
int CvTraitEntry::GetNaturalWonderSubsequentFinderFaith() const
{
	return m_iNaturalWonderSubsequentFinderFaith;
}
/////////////////


/// Accessor:: modifier to bonuses for having natural wonders worked or in territory
int CvTraitEntry::GetNaturalWonderYieldModifier() const
{
	return m_iNaturalWonderYieldModifier;
}

/// Accessor: modifier to happiness received from finding natural wonders
int CvTraitEntry::GetNaturalWonderHappinessModifier() const
{
	return m_iNaturalWonderHappinessModifier;
}

/// Accessor: size of combat bonus for nearby improvements
int CvTraitEntry::GetNearbyImprovementCombatBonus() const
{
	return m_iNearbyImprovementCombatBonus;
}

/// Accessor: range of bonus from nearby improvements
int CvTraitEntry::GetNearbyImprovementBonusRange() const
{
	return m_iNearbyImprovementBonusRange;
}

/// Accessor: extra yield for culture buildings
int CvTraitEntry::GetCultureBuildingYieldChange() const
{
	return m_iCultureBuildingYieldChange;
}

/// Accessor: combat bonus in own territory vs. higher tech units
int CvTraitEntry::GetCombatBonusVsHigherTech() const
{
	return m_iCombatBonusVsHigherTech;
}

/// Accessor: combat bonus vs. civ with more cities
int CvTraitEntry::GetCombatBonusVsLargerCiv() const
{
	return m_iCombatBonusVsLargerCiv;
}

/// Accessor: change to speed razing cities
int CvTraitEntry::GetRazeSpeedModifier() const
{
	return m_iRazeSpeedModifier;
}

/// Accessor: great person boost from friendship
int CvTraitEntry::GetDOFGreatPersonModifier() const
{
	return m_iDOFGreatPersonModifier;
}

/// Accessor: change to usual cost for maintaining land units
int CvTraitEntry::GetLandUnitMaintenanceModifier() const
{
	return m_iLandUnitMaintenanceModifier;
}

/// Accessor: change to usual cost for maintaining naval units
int CvTraitEntry::GetNavalUnitMaintenanceModifier() const
{
	return m_iNavalUnitMaintenanceModifier;
}

/// Accessor: percentage of happiness retained from luxuries traded away
int CvTraitEntry::GetLuxuryHappinessRetention() const
{
	return m_iLuxuryHappinessRetention;
}

// NQMP GJS - New Netherlands UA BEGIN
/// Accessor: extra happiness for the empire per unique luxury
int CvTraitEntry::GetExtraHappinessPerLuxury() const
{
	return m_iExtraHappinessPerLuxury;
}

// NQMP GJS - New Netherlands UA END

/// Accessor: number of extra spies
int CvTraitEntry::GetExtraSpies() const
{
	return m_iExtraSpies;
}

// NQMP GJS - New Ottoman UA begin
/// Accessor: number of extra spies
int CvTraitEntry::GetHappinessPerReligion() const
{
	return m_iHappinessPerReligion;
}
// NQMP GJS - New Ottoman UA end

/// Accessor: modifier for beaker bonus towards unresearched tech upon killing unit from that tech
int CvTraitEntry::GetUnresearchedTechBonusFromKills() const
{
	return m_iUnresearchedTechBonusFromKills;
}

int CvTraitEntry::GetExtraFoundedCityTerritoryClaimRange() const
{
	return m_iExtraFoundedCityTerritoryClaimRange;
}

/// Accessor: extra social policy from advancing to the next age
int CvTraitEntry::GetFreeSocialPoliciesPerEra() const
{
	return m_iFreeSocialPoliciesPerEra;
}

int CvTraitEntry::GetNumTradeRoutesModifier() const
{
	return m_iNumTradeRoutesModifier;
}

int CvTraitEntry::GetTradeRouteResourceModifier() const
{
	return m_iTradeRouteResourceModifier;
}

int CvTraitEntry::GetUniqueLuxuryCities() const
{
	return m_iUniqueLuxuryCities;
}

int CvTraitEntry::GetUniqueLuxuryQuantity() const
{
	return m_iUniqueLuxuryQuantity;
}

int CvTraitEntry::GetWorkerSpeedModifier() const
{
	return m_iWorkerSpeedModifier;
}

int CvTraitEntry::GetAfraidMinorPerTurnInfluence() const
{
	return m_iAfraidMinorPerTurnInfluence;
}

int CvTraitEntry::GetLandTradeRouteRangeBonus() const
{
	return m_iLandTradeRouteRangeBonus;
}

int CvTraitEntry::GetTradeReligionModifier() const
{
	return m_iTradeReligionModifier;
}

int CvTraitEntry::GetTradeBuildingModifier() const
{
	return m_iTradeBuildingModifier;
}

// CMP DLL Table inserts ~EAP

TechTypes CvTraitEntry::GetFreeBuildingPrereqTech() const
{
	return m_eFreeBuildingPrereqTech;
}
TechTypes CvTraitEntry::GetCapitalFreeBuildingPrereqTech() const
{
	return m_eCapitalFreeBuildingPrereqTech;
}

/// Accessor: tech that triggers this free unit
TechTypes CvTraitEntry::GetFreeUnitPrereqTech() const
{
	return m_eFreeUnitPrereqTech;
}

/// Accessor: tech that triggers this free unit
ImprovementTypes CvTraitEntry::GetCombatBonusImprovement() const
{
	return m_eCombatBonusImprovement;
}

/// Accessor: free building in each city
BuildingTypes CvTraitEntry::GetFreeBuilding() const
{
	return m_eFreeBuilding;
}

/// Does the capital get a free building? -- From CMP DLL
BuildingTypes CvTraitEntry::GetFreeCapitalBuilding() const
{
	return m_eFreeCapitalBuilding;
}

/// Accessor: free building in each city conquered
BuildingTypes CvTraitEntry::GetFreeBuildingOnConquest() const
{
	return m_eFreeBuildingOnConquest;
}

/// Accessor:: does this civ get combat bonuses when damaged?
bool CvTraitEntry::IsFightWellDamaged() const
{
	return m_bFightWellDamaged;
}

/// Accessor:: does this civ move units through forest as if it is road?
bool CvTraitEntry::IsMoveFriendlyWoodsAsRoad() const
{
	return m_bMoveFriendlyWoodsAsRoad;
}

/// Accessor:: does this civ move along rivers like a Scout?
bool CvTraitEntry::IsFasterAlongRiver() const
{
	return m_bFasterAlongRiver;
}

/// Accessor:: does this civ move in Hills like a Scout?
bool CvTraitEntry::IsFasterInHills() const
{
	return m_bFasterInHills;
}

/// Accessor:: are our embarked units allowed to enter ocean?
bool CvTraitEntry::IsEmbarkedAllWater() const
{
	return m_bEmbarkedAllWater;
}

/// Accessor:: are our embarked units able to move to land for 1 MP?
bool CvTraitEntry::IsEmbarkedToLandFlatCost() const
{
	return m_bEmbarkedToLandFlatCost;
}

/// Accessor:: free improvement maintenance in hills?
bool CvTraitEntry::IsNoHillsImprovementMaintenance() const
{
	return m_bNoHillsImprovementMaintenance;
}

/// Accessor:: tech boost when build science building in capital?
bool CvTraitEntry::IsTechBoostFromCapitalScienceBuildings() const
{
	return m_bTechBoostFromCapitalScienceBuildings;
}

/// Accessor:: does this civ still exist with zero cities?
bool CvTraitEntry::IsStaysAliveZeroCities() const
{
	return m_bStaysAliveZeroCities;
}

/// Accessor: does this civ get Faith from settling cities near Forest?
bool CvTraitEntry::IsFaithFromUnimprovedForest() const
{
	return m_bFaithFromUnimprovedForest;
}

#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
/// Accessor: does this civ have naval trade units that are immune to plundering?
bool CvTraitEntry::IsSeaTradeRoutesArePlunderImmune() const
{
	return m_bSeaTradeRoutesArePlunderImmune;
}
#endif

// NQMP GJS - New France UA begin
// Accessor: does this civ get free cultural Great People from Guilds and Wonders with Great Works Slots?
bool CvTraitEntry::IsEarnsGreatPersonOnSlotOrGuild() const
{
	return m_bEarnsGreatPersonOnSlotOrGuild;
}
// Accessor: has this civ already built a writer's guild?
bool CvTraitEntry::IsHasBuiltWritersGuild() const
{
	return m_bHasBuiltWritersGuild;
};
// Mutator: set whether this civ has already built a writer's guild
void CvTraitEntry::SetHasBuiltWritersGuild(bool hasBuilt)
{
	m_bHasBuiltWritersGuild = hasBuilt;
};
// Accessor: has this civ already built an artist's guild?
bool CvTraitEntry::IsHasBuiltArtistsGuild() const
{
	return m_bHasBuiltArtistsGuild;
};
// Mutator: set whether this civ has already built a writer's guild
void CvTraitEntry::SetHasBuiltArtistsGuild(bool hasBuilt)
{
	m_bHasBuiltArtistsGuild = hasBuilt;
};
// Accessor: has this civ already built a musician's guild?
bool CvTraitEntry::IsHasBuiltMusiciansGuild() const
{
	return m_bHasBuiltMusiciansGuild;
};
// Mutator: set whether this civ has already built a writer's guild
void CvTraitEntry::SetHasBuiltMusiciansGuild(bool hasBuilt)
{
	m_bHasBuiltMusiciansGuild = hasBuilt;
};
// NQMP GJS - New France UA end

/// Accessor: does this civ get a bonus religious belief?
bool CvTraitEntry::IsBonusReligiousBelief() const
{
	return m_bBonusReligiousBelief;
}

/// Accessor: does this civ have the ability to use gold to annex city states?
bool CvTraitEntry::IsAbleToAnnexCityStates() const
{
	return m_bAbleToAnnexCityStates;
}

/// Accessor: do combat units have the ability to cross mountains after a great general is born?
bool CvTraitEntry::IsCrossesMountainsAfterGreatGeneral() const
{
	return m_bCrossesMountainsAfterGreatGeneral;
}

/// Accessor: is this civ receiving bonuses based on the Maya calendar?
bool CvTraitEntry::IsMayaCalendarBonuses() const
{
	return m_bMayaCalendarBonuses;
}

/// Accessor: is this civ prevented from annexing cities?
bool CvTraitEntry::IsNoAnnexing() const
{
	return m_bNoAnnexing;
}

/// Accessor: does this civ get a tech from conquering a city?
bool CvTraitEntry::IsTechFromCityConquer() const
{
	return m_bTechFromCityConquer;
}

/// Accessor: do unique luxuries only get added into cities founded on new continents?
bool CvTraitEntry::IsUniqueLuxuryRequiresNewArea() const
{
	return m_bUniqueLuxuryRequiresNewArea;
}

/// Accessor: Do rivers count as trade roads
bool CvTraitEntry::IsRiverTradeRoad() const
{
	return m_bRiverTradeRoad;
}

/// Accessor: Do our units not anger City-States when they intrude in their lands?
bool CvTraitEntry::IsAngerFreeIntrusionOfCityStates() const
{
	return m_bAngerFreeIntrusionOfCityStates;
}

/// Accessor:: Get brief text description
const char* CvTraitEntry::getShortDescription() const
{
	return m_strShortDescription;
}

/// Accessor:: Set brief text description
void CvTraitEntry::setShortDescription(const char* szVal)
{
	m_strShortDescription = szVal;
}

/// Accessor:: 1 extra yield comes all tiles with a base yield of this
int CvTraitEntry::GetExtraYieldThreshold(int i) const
{
	return m_paiExtraYieldThreshold ? m_paiExtraYieldThreshold[i] : -1;
}

/// Accessor:: Additional yield
int CvTraitEntry::GetYieldChange(int i) const
{
	return m_paiYieldChange ? m_paiYieldChange[i] : -1;
}

/// Accessor:: Extra yield from strategic resources
int CvTraitEntry::GetYieldChangeStrategicResources(int i) const
{
	return m_paiYieldChangeStrategicResources ? m_paiYieldChangeStrategicResources[i] : -1;
}

// NQMP GJS - New Netherlands UA BEGIN 
/// Accessor:: Extra yield from luxury resources
int CvTraitEntry::GetYieldChangeLuxuryResources(int i) const
{
	return m_paiYieldChangeLuxuryResources ? m_paiYieldChangeLuxuryResources[i] : -1;
}
// NQMP GJS - New Netherlands UA END

/// Accessor:: Extra yield from natural wonders
int CvTraitEntry::GetYieldChangeNaturalWonder(int i) const
{
	return m_paiYieldChangeNaturalWonder ? m_paiYieldChangeNaturalWonder[i] : -1;
}
#if defined(FULL_YIELD_FROM_KILLS) // Accessors, Arrays
/// Accessor:: Get Yield from Killing Units
int CvTraitEntry::GetYieldFromKills(int i) const
{
	return m_paiYieldFromKills ? m_paiYieldFromKills[i] : -1;
}
#endif
/// Accessor:: Extra yield from trade partners
int CvTraitEntry::GetYieldChangePerTradePartner(int i) const
{
	return m_paiYieldChangePerTradePartner ? m_paiYieldChangePerTradePartner[i] : -1;
}

/// Accessor:: Extra yield from incoming trade routes
int CvTraitEntry::GetYieldChangeIncomingTradeRoute(int i) const
{
	return m_paiYieldChangeIncomingTradeRoute ? m_paiYieldChangeIncomingTradeRoute[i] : -1;
}

/// Accessor:: Modifier to yield
int CvTraitEntry::GetYieldModifier(int i) const
{
	return m_paiYieldModifier ? m_paiYieldModifier[i] : -1;
}

/// Accessor:: Additional quantity of strategic resources
int CvTraitEntry::GetStrategicResourceQuantityModifier(int i) const
{
	return m_piStrategicResourceQuantityModifier ? m_piStrategicResourceQuantityModifier[i] : -1;
}

/// Accessor:: Additional quantity of a specific resource
int CvTraitEntry::GetResourceQuantityModifier(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piResourceQuantityModifiers ? m_piResourceQuantityModifiers[i] : -1;
}

/// Accessor:: Extra yield from an improvement
int CvTraitEntry::GetImprovementYieldChanges(ImprovementTypes eIndex1, YieldTypes eIndex2) const
{
	CvAssertMsg(eIndex1 < GC.getNumImprovementInfos(), "Index out of bounds");
	CvAssertMsg(eIndex1 > -1, "Index out of bounds");
	CvAssertMsg(eIndex2 < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(eIndex2 > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiImprovementYieldChanges.first ? m_ppiImprovementYieldChanges.first[eIndex1][eIndex2] : 0;
#else
	return m_ppiImprovementYieldChanges ? m_ppiImprovementYieldChanges[eIndex1][eIndex2] : 0;
#endif
}

/// Accessor:: Extra yield from an improvement
int CvTraitEntry::GetSpecialistYieldChanges(SpecialistTypes eIndex1, YieldTypes eIndex2) const
{
	CvAssertMsg(eIndex1 < GC.getNumSpecialistInfos(), "Index out of bounds");
	CvAssertMsg(eIndex1 > -1, "Index out of bounds");
	CvAssertMsg(eIndex2 < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(eIndex2 > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiSpecialistYieldChanges.first ? m_ppiSpecialistYieldChanges.first[eIndex1][eIndex2] : 0;
#else
	return m_ppiSpecialistYieldChanges ? m_ppiSpecialistYieldChanges[eIndex1][eIndex2] : 0;
#endif
}

#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
int CvTraitEntry::GetAnySpecificSpecialistYieldChanges(SpecialistTypes eIndex1, YieldTypes eIndex2) const
{
	CvAssertMsg(eIndex1 < GC.getNumSpecialistInfos(), "Index out of bounds");
	CvAssertMsg(eIndex1 > -1, "Index out of bounds");
	CvAssertMsg(eIndex2 < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(eIndex2 > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiAnySpecificSpecialistYieldChanges.first ? m_ppiAnySpecificSpecialistYieldChanges.first[eIndex1][eIndex2] : 0;
#else
	return m_ppiAnySpecificSpecialistYieldChanges ? m_ppiAnySpecificSpecialistYieldChanges[eIndex1][eIndex2] : 0;
#endif
}
#endif
/// Accessor:: Extra yield from an unimproved feature
int CvTraitEntry::GetUnimprovedFeatureYieldChanges(FeatureTypes eIndex1, YieldTypes eIndex2) const
{
	CvAssertMsg(eIndex1 < GC.getNumFeatureInfos(), "Index out of bounds");
	CvAssertMsg(eIndex1 > -1, "Index out of bounds");
	CvAssertMsg(eIndex2 < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(eIndex2 > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiUnimprovedFeatureYieldChanges.first ? m_ppiUnimprovedFeatureYieldChanges.first[eIndex1][eIndex2] : 0;
#else
	return m_ppiUnimprovedFeatureYieldChanges ? m_ppiUnimprovedFeatureYieldChanges[eIndex1][eIndex2] : 0;
#endif
}

/// Accessor:: Additional moves for a class of combat unit
int CvTraitEntry::GetMovesChangeUnitCombat(const int unitCombatID) const
{
	CvAssertMsg((unitCombatID >= 0), "unitCombatID is less than zero");
	CvAssertMsg((unitCombatID < GC.getNumUnitCombatClassInfos()), "unitCombatID exceeds number of combat classes");

	return m_piMovesChangeUnitCombats[unitCombatID];
}

/// Accessor:: Maintenance Modifier for a class of combat unit
int CvTraitEntry::GetMaintenanceModifierUnitCombat(const int unitCombatID) const
{
	CvAssertMsg((unitCombatID >= 0), "unitCombatID is less than zero");
	CvAssertMsg((unitCombatID < GC.getNumUnitCombatClassInfos()), "unitCombatID exceeds number of combat classes");

	return m_piMaintenanceModifierUnitCombats[unitCombatID];
}

/// Does this trait provide free resources in the first X cities?
FreeResourceXCities CvTraitEntry::GetFreeResourceXCities(ResourceTypes eResource) const
{
	return m_aFreeResourceXCities[(int)eResource];
}

/// Tech that makes this trait obsolete
int CvTraitEntry::GetObsoleteTech() const
{
	return m_iObsoleteTech;
}

/// Tech that enables Maya calendar bonuses
int CvTraitEntry::GetPrereqTech() const
{
	return m_iPrereqTech;
}

/// Accessor:: Does the civ get free promotions?
bool CvTraitEntry::IsFreePromotionUnitCombat(const int promotionID, const int unitCombatID) const
{
	std::multimap<int, int>::const_iterator it = m_FreePromotionUnitCombats.find(promotionID);
	if(it != m_FreePromotionUnitCombats.end())
	{
		// get an iterator to the element that is one past the last element associated with key
		std::multimap<int, int>::const_iterator lastElement = m_FreePromotionUnitCombats.upper_bound(promotionID);

		// for each element in the sequence [itr, lastElement)
		for(; it != lastElement; ++it)
		{
			if(it->second == unitCombatID)
			{
				return true;
			}
		}
	}

	return false;
}

/// Has this trait become obsolete?
bool CvTraitEntry::IsObsoleteByTech(TeamTypes eTeam)
{
	if(m_iObsoleteTech != NO_TECH)
	{
		if(GET_TEAM(eTeam).GetTeamTechs()->HasTech((TechTypes)m_iObsoleteTech))
		{
			return true;
		}
	}
	return false;
}

/// Is this trait enabled by tech?
bool CvTraitEntry::IsEnabledByTech(TeamTypes eTeam)
{
	if(m_iPrereqTech != NO_TECH)
	{
		if(GET_TEAM(eTeam).GetTeamTechs()->HasTech((TechTypes)m_iPrereqTech))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CvTraitEntry::NoTrain(UnitClassTypes eUnitClass)
{
	if (eUnitClass != NO_UNITCLASS)
	{
		return m_abNoTrainUnitClass[eUnitClass];
	}
	else
	{
		return false;
	}
}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
bool CvTraitEntry::NoBuildImprovements(ImprovementTypes eImprovement)
{
	if (eImprovement != NO_IMPROVEMENT)
	{
		return m_abNoBuildImprovements[eImprovement];
	}
	else
	{
		return false;
	}
}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
int CvTraitEntry::GetBuildTimeOverride(BuildTypes eBuild, ResourceClassTypes eResourceClass)
{
	if (eBuild == NO_BUILD)
	{
		return -1;
	}

	// First try to find a direct match for the resource class
	int iBestTime = -1;
	bool bFoundGenericMatch = false;

	typedef std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>>::const_iterator it_type;
	std::pair<it_type, it_type> range = m_BuildTimeOverrides.equal_range(eBuild);

	// First pass: look for exact resource class match
	for (it_type it = range.first; it != range.second; ++it)
	{
		ResourceClassTypes eRequiredClass = it->second.second;
		int iBuildTime = it->second.first;

		// Exact match for resource class
		if (eRequiredClass == eResourceClass)
		{
			return iBuildTime; // Found exact match, return immediately
		}

		// Keep track of NO_RESOURCECLASS entries for fallback
		if (eRequiredClass == NO_RESOURCECLASS)
		{
			iBestTime = iBuildTime;
			bFoundGenericMatch = true;
		}
	}

	// If we found a generic match, return that
	if (bFoundGenericMatch)
	{
		return iBestTime;
	}

	return -1;  // No suitable override found
}
#endif

#if defined(TRAITIFY) // Array accessors
// Remove Terrain Requirement
bool CvTraitEntry::IsBuildingClassRemoveRequiredTerrain(BuildingClassTypes eBuildingClass) const
{
	if (eBuildingClass != NO_BUILDINGCLASS)
	{
		return m_abBuildingClassRemoveRequiredTerrain[eBuildingClass];
	}
	else
	{
		return false;
	}
}
// Make Defined UnitClasses spawn in the Capital when given by traits
bool CvTraitEntry::IsUnitClassForceSpawnCapital(UnitClassTypes eUnitClass) const
{
	if (eUnitClass != NO_UNITCLASS)
	{
		return m_abUnitClassForceSpawnCapital[eUnitClass];
	}
	else
	{
		return false;
	}
}
// Change Yield based on ResourceClassType (Netherlands, Russia and Jerusalem)
int CvTraitEntry::GetResourceClassYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumResourceClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiResourceClassYieldChanges ? m_ppiResourceClassYieldChanges[i][j] : -1;
}
// Change the Local happiness of a building
int CvTraitEntry::GetBuildingClassHappiness(int i) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiBuildingClassHappiness ? m_paiBuildingClassHappiness[i] : -1;
}
// Change the Global happiness of a building
int CvTraitEntry::GetBuildingClassGlobalHappiness(int i) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiBuildingClassGlobalHappiness ? m_paiBuildingClassGlobalHappiness[i] : -1;
}
// Override prod/gold/faith cost of buildings
int CvTraitEntry::GetBuildingCostOverride(int i, int j) const
{
	CvAssertMsg(i < GC.getNumBuildingInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiBuildingCostOverride[i][j];
}
// Change the yield of a building when a trait is active
int CvTraitEntry::GetBuildingClassYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiBuildingClassYieldChanges[i][j];
}
// Puppet City Yield Modifiers
int CvTraitEntry::GetPuppetYieldModifiers(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piPuppetYieldModifiers ? m_piPuppetYieldModifiers[i] : -1;
}
// Route Movement Change
int CvTraitEntry::GetRouteMovementChange(int i) const
{
	CvAssertMsg(i < GC.getNumRouteInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiRouteMovementChange ? m_paiRouteMovementChange[i] : 0;
}
// Feature Yield Changes
int CvTraitEntry::GetFeatureYieldChanges(FeatureTypes eIndex1, YieldTypes eIndex2) const
{
	CvAssertMsg(eIndex1 < GC.getNumFeatureInfos(), "Index out of bounds");
	CvAssertMsg(eIndex1 > -1, "Index out of bounds");
	CvAssertMsg(eIndex2 < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(eIndex2 > -1, "Index out of bounds");
	return m_ppiFeatureYieldChanges ? m_ppiFeatureYieldChanges[eIndex1][eIndex2] : 0;
}
// Terrain Yield Changes
int CvTraitEntry::GetTerrainYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumTerrainInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiTerrainYieldChanges ? m_ppiTerrainYieldChanges[i][j] : 0;
}
// Resource Yield Changes
int CvTraitEntry::GetResourceYieldChanges(int i, int j) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
	return m_ppiResourceYieldChanges ? m_ppiResourceYieldChanges[i][j] : 0;
}
// Yield Per Population
int CvTraitEntry::GetYieldPerPopulation(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiYieldPerPopulation ? m_paiYieldPerPopulation[i] : -1;
}
// Yield per Population, with Foreign Relgion switch
int CvTraitEntry::GetYieldPerPopulationForeignReligion(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiYieldPerPopulationForeignReligion ? m_paiYieldPerPopulationForeignReligion[i] : -1;
}
#endif
/// Load XML data
bool CvTraitEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if(!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	//Basic Properties
	setShortDescription(kResults.GetText("ShortDescription"));

	m_iLevelExperienceModifier				= kResults.GetInt("LevelExperienceModifier");
	m_iGreatPeopleRateModifier				= kResults.GetInt("GreatPeopleRateModifier");
	m_iGreatScientistRateModifier			= kResults.GetInt("GreatScientistRateModifier");
	m_iGreatGeneralRateModifier				= kResults.GetInt("GreatGeneralRateModifier");
	m_iGreatGeneralExtraBonus				= kResults.GetInt("GreatGeneralExtraBonus");
	m_iGreatPersonGiftInfluence				= kResults.GetInt("GreatPersonGiftInfluence");
	m_iMaxGlobalBuildingProductionModifier	= kResults.GetInt("MaxGlobalBuildingProductionModifier");
	m_iMaxTeamBuildingProductionModifier	= kResults.GetInt("MaxTeamBuildingProductionModifier");
	m_iMaxPlayerBuildingProductionModifier	= kResults.GetInt("MaxPlayerBuildingProductionModifier");
	m_iCityUnhappinessModifier           	= kResults.GetInt("CityUnhappinessModifier");
	m_iPopulationUnhappinessModifier    	= kResults.GetInt("PopulationUnhappinessModifier");
	m_iCityStateBonusModifier               = kResults.GetInt("CityStateBonusModifier");
	m_iCityStateFriendshipModifier          = kResults.GetInt("CityStateFriendshipModifier");
	m_iCityStateCombatModifier				= kResults.GetInt("CityStateCombatModifier");
	m_iLandBarbarianConversionPercent       = kResults.GetInt("LandBarbarianConversionPercent");
	m_iLandBarbarianConversionExtraUnits    = kResults.GetInt("LandBarbarianConversionExtraUnits");
	m_iSeaBarbarianConversionPercent        = kResults.GetInt("SeaBarbarianConversionPercent");
	m_iCapitalBuildingModifier				= kResults.GetInt("CapitalBuildingModifier");
	m_iPlotBuyCostModifier					= kResults.GetInt("PlotBuyCostModifier");
	m_iPlotCultureCostModifier              = kResults.GetInt("PlotCultureCostModifier");
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	m_iSciencePerGreatPersonBorn			= kResults.GetInt("SciencePerGreatPersonBorn");
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	m_iNumTurnsBeforeMinorAlliesRefuseBribes = kResults.GetInt("NumTurnsBeforeMinorAlliesRefuseBribes");
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	m_iGoldenAgeTileBonusFaith              = kResults.GetInt("GoldenAgeTileBonusFaith");
#endif
	m_iCultureFromKills						= kResults.GetInt("CultureFromKills");
	m_iFaithFromKills						= kResults.GetInt("FaithFromKills");
	m_iCityCultureBonus						= kResults.GetInt("CityCultureBonus");
	m_iCapitalThemingBonusModifier          = kResults.GetInt("CapitalThemingBonusModifier");
	m_iPolicyCostModifier					= kResults.GetInt("PolicyCostModifier");
	m_iCityConnectionTradeRouteChange		= kResults.GetInt("CityConnectionTradeRouteChange");
	m_iWonderProductionModifier				= kResults.GetInt("WonderProductionModifier");
	m_iPlunderModifier						= kResults.GetInt("PlunderModifier");
	m_iImprovementMaintenanceModifier       = kResults.GetInt("ImprovementMaintenanceModifier");
	m_iGoldenAgeDurationModifier			= kResults.GetInt("GoldenAgeDurationModifier");
	m_iGoldenAgeMoveChange				    = kResults.GetInt("GoldenAgeMoveChange");
	m_iGoldenAgeCombatModifier				= kResults.GetInt("GoldenAgeCombatModifier");
	m_iGoldenAgeTourismModifier				= kResults.GetInt("GoldenAgeTourismModifier");
	m_iGoldenAgeGreatArtistRateModifier		= kResults.GetInt("GoldenAgeGreatArtistRateModifier");
	m_iGoldenAgeGreatMusicianRateModifier	= kResults.GetInt("GoldenAgeGreatMusicianRateModifier");
	m_iGoldenAgeGreatWriterRateModifier		= kResults.GetInt("GoldenAgeGreatWriterRateModifier");
	m_iExtraEmbarkMoves						= kResults.GetInt("ExtraEmbarkMoves");
	m_iNaturalWonderFirstFinderGold         = kResults.GetInt("NaturalWonderFirstFinderGold");
	m_iNaturalWonderSubsequentFinderGold    = kResults.GetInt("NaturalWonderSubsequentFinderGold");

#if defined(TRAITIFY) // CvTraitEntry::CacheResults, int and bool
	m_bHalfMoreSpecialistUnhappiness		= kResults.GetBool("HalfMoreSpecialistUnhappiness");

	m_iGoldenAgeCultureModifier				= kResults.GetInt("GoldenAgeCultureModifier");
	m_iNumExtraLeagueVotes					= kResults.GetInt("NumExtraLeagueVotes");
	m_iNumTradeRouteBonus					= kResults.GetInt("NumTradeRouteBonus");
	m_iMinorFriendshipMinimum				= kResults.GetInt("MinorFriendshipMinimum");
	m_iGreatEngineerRateModifier			= kResults.GetInt("GreatEngineerRateModifier");
	m_iGreatMerchantRateModifier			= kResults.GetInt("GreatMerchantRateModifier");
	m_iMinorBullyModifier					= kResults.GetInt("MinorBullyModifier");
	m_iInternationalRouteGrowthModifier		= kResults.GetInt("InternationalRouteGrowthModifier");
	m_iLocalHappinessPerCity				= kResults.GetInt("LocalHappinessPerCity");
	m_iInternalTradeRouteYieldModifier		= kResults.GetInt("InternalTradeRouteYieldModifier");
	m_iUnhappinessModifierForPuppetedCities = kResults.GetInt("UnhappinessModifierForPuppetedCities");
	m_iExtraPopulation						= kResults.GetInt("ExtraPopulation");
	m_iFaithCostModifier					= kResults.GetInt("FaithCostModifier");
	m_iIdeologyPressureUnhappinessModifier  = kResults.GetInt("IdeologyPressureUnhappinessModifier");
	m_iForeignRelgionPressureModifier		= kResults.GetInt("ForeignRelgionPressureModifier");
	m_iFriendlyLandsCitizenMoveChange		= kResults.GetInt("FriendlyLandsCitizenMoveChange");
#endif
#if defined(LEKMOD_v34)
	m_bReligionEnhanceReformation			= kResults.GetBool("ReligionEnhanceReformation");
	m_iSelfReligiousPressureModifier		= kResults.GetInt("SelfReligiousPressureModifier");
	m_iLandTradeRouteYieldBonus				= kResults.GetInt("LandTradeRouteYieldBonus");
#endif
	//EAP: Faith for the Natural wonder findor
	m_iNaturalWonderFirstFinderFaith         = kResults.GetInt("NaturalWonderFirstFinderFaith");
	m_iNaturalWonderSubsequentFinderFaith    = kResults.GetInt("NaturalWonderSubsequentFinderFaith");

	m_iNaturalWonderYieldModifier           = kResults.GetInt("NaturalWonderYieldModifier");
	m_iNaturalWonderHappinessModifier       = kResults.GetInt("NaturalWonderHappinessModifier");
	m_iNearbyImprovementCombatBonus			= kResults.GetInt("NearbyImprovementCombatBonus");
	m_iNearbyImprovementBonusRange			= kResults.GetInt("NearbyImprovementBonusRange");
	m_iCultureBuildingYieldChange			= kResults.GetInt("CultureBuildingYieldChange");
	m_iCombatBonusVsHigherTech				= kResults.GetInt("CombatBonusVsHigherTech");
	m_iCombatBonusVsLargerCiv				= kResults.GetInt("CombatBonusVsLargerCiv");
	m_iLandUnitMaintenanceModifier          = kResults.GetInt("LandUnitMaintenanceModifier");
	m_iNavalUnitMaintenanceModifier         = kResults.GetInt("NavalUnitMaintenanceModifier");
	m_iRazeSpeedModifier					= kResults.GetInt("RazeSpeedModifier");
	m_iDOFGreatPersonModifier				= kResults.GetInt("DOFGreatPersonModifier");
	m_iLuxuryHappinessRetention				= kResults.GetInt("LuxuryHappinessRetention");
	m_iExtraHappinessPerLuxury				= kResults.GetInt("ExtraHappinessPerLuxury"); // NQMP GJS - New Netherlands UA
	m_iExtraSpies							= kResults.GetInt("ExtraSpies");
	m_iHappinessPerReligion					= kResults.GetInt("HappinessPerReligion"); // NQMP GJS - New Ottoman UA
	m_iUnresearchedTechBonusFromKills		= kResults.GetInt("UnresearchedTechBonusFromKills");
	m_iExtraFoundedCityTerritoryClaimRange  = kResults.GetInt("ExtraFoundedCityTerritoryClaimRange");
	m_iFreeSocialPoliciesPerEra				= kResults.GetInt("FreeSocialPoliciesPerEra");
	m_iNumTradeRoutesModifier				= kResults.GetInt("NumTradeRoutesModifier");
	m_iTradeRouteResourceModifier			= kResults.GetInt("TradeRouteResourceModifier");
	m_iUniqueLuxuryCities					= kResults.GetInt("UniqueLuxuryCities");
	m_iUniqueLuxuryQuantity					= kResults.GetInt("UniqueLuxuryQuantity");
	m_iWorkerSpeedModifier					= kResults.GetInt("WorkerSpeedModifier");
	m_iAfraidMinorPerTurnInfluence			= kResults.GetInt("AfraidMinorPerTurnInfluence");
	m_iLandTradeRouteRangeBonus				= kResults.GetInt("LandTradeRouteRangeBonus");
	m_iTradeReligionModifier				= kResults.GetInt("TradeReligionModifier");
	m_iTradeBuildingModifier				= kResults.GetInt("TradeBuildingModifier");

	const char* szTextVal = NULL;
	szTextVal = kResults.GetText("FreeUnit");
	if(szTextVal)
	{
		m_iFreeUnitClassType = GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("FreeUnitPrereqTech");
	if(szTextVal)
	{
		m_eFreeUnitPrereqTech = (TechTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	//// CMP DLL THING

	szTextVal = kResults.GetText("FreeBuildingPrereqTech");
	if(szTextVal)
	{
		m_eFreeBuildingPrereqTech = (TechTypes)GC.getInfoTypeForString(szTextVal, true);
	}
	szTextVal = kResults.GetText("CapitalFreeBuildingPrereqTech");
	if(szTextVal)
	{
		m_eCapitalFreeBuildingPrereqTech = (TechTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	///


	szTextVal = kResults.GetText("CombatBonusImprovement");
	if(szTextVal)
	{
		m_eCombatBonusImprovement = (ImprovementTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("ObsoleteTech");
	if(szTextVal)
	{
		m_iObsoleteTech = GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("PrereqTech");
	if(szTextVal)
	{
		m_iPrereqTech = GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("FreeBuilding");
	if(szTextVal)
	{
		m_eFreeBuilding = (BuildingTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("FreeCapitalBuilding");
	if(szTextVal)
	{
		m_eFreeCapitalBuilding = (BuildingTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	szTextVal = kResults.GetText("FreeBuildingOnConquest");
	if(szTextVal)
	{
		m_eFreeBuildingOnConquest = (BuildingTypes)GC.getInfoTypeForString(szTextVal, true);
	}

	m_bFightWellDamaged = kResults.GetBool("FightWellDamaged");
	m_bMoveFriendlyWoodsAsRoad = kResults.GetBool("MoveFriendlyWoodsAsRoad");
	m_bFasterAlongRiver = kResults.GetBool("FasterAlongRiver");
	m_bFasterInHills = kResults.GetBool("FasterInHills");
	m_bEmbarkedAllWater = kResults.GetBool("EmbarkedAllWater");
	m_bEmbarkedToLandFlatCost = kResults.GetBool("EmbarkedToLandFlatCost");
	m_bNoHillsImprovementMaintenance = kResults.GetBool("NoHillsImprovementMaintenance");
	m_bTechBoostFromCapitalScienceBuildings = kResults.GetBool("TechBoostFromCapitalScienceBuildings");
	m_bStaysAliveZeroCities = kResults.GetBool("StaysAliveZeroCities");
	m_bFaithFromUnimprovedForest = kResults.GetBool("FaithFromUnimprovedForest");
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	m_bSeaTradeRoutesArePlunderImmune = kResults.GetBool("SeaTradeRoutesArePlunderImmune");
#endif
	m_bEarnsGreatPersonOnSlotOrGuild = kResults.GetBool("EarnsGreatPersonOnSlotOrGuild"); // NQMP GJS - New France UA
	m_bBonusReligiousBelief = kResults.GetBool("BonusReligiousBelief");
	m_bAbleToAnnexCityStates = kResults.GetBool("AbleToAnnexCityStates");
	m_bCrossesMountainsAfterGreatGeneral = kResults.GetBool("CrossesMountainsAfterGreatGeneral");
	m_bMayaCalendarBonuses = kResults.GetBool("MayaCalendarBonuses");
	m_bNoAnnexing = kResults.GetBool("NoAnnexing");
	m_bTechFromCityConquer = kResults.GetBool("TechFromCityConquer");
	m_bUniqueLuxuryRequiresNewArea = kResults.GetBool("UniqueLuxuryRequiresNewArea");
	m_bRiverTradeRoad = kResults.GetBool("RiverTradeRoad");
	m_bAngerFreeIntrusionOfCityStates = kResults.GetBool("AngerFreeIntrusionOfCityStates");

	//Arrays
	const char* szTraitType = GetType();
	kUtility.SetYields(m_paiExtraYieldThreshold, "Trait_ExtraYieldThresholds", "TraitType", szTraitType);

	kUtility.SetYields(m_paiYieldChange, "Trait_YieldChanges", "TraitType", szTraitType);
#if !defined(LEKMOD_v34)
	kUtility.SetYields(m_paiYieldChangeStrategicResources, "Trait_YieldChangesStrategicResources", "TraitType", szTraitType);
	kUtility.SetYields(m_paiYieldChangeLuxuryResources, "Trait_YieldChangesLuxuryResources", "TraitType", szTraitType); // NQMP GJS - New Netherlands UA
#endif
#if defined(FULL_YIELD_FROM_KILLS)
	kUtility.SetYields(m_paiYieldFromKills, "Trait_YieldFromKills", "TraitType", szTraitType);
#endif
	kUtility.SetYields(m_paiYieldChangeNaturalWonder, "Trait_YieldChangesNaturalWonder", "TraitType", szTraitType);
	kUtility.SetYields(m_paiYieldChangePerTradePartner, "Trait_YieldChangesPerTradePartner", "TraitType", szTraitType);
	kUtility.SetYields(m_paiYieldChangeIncomingTradeRoute, "Trait_YieldChangesIncomingTradeRoute", "TraitType", szTraitType);
	kUtility.SetYields(m_paiYieldModifier, "Trait_YieldModifiers", "TraitType", szTraitType);

	const int iNumTerrains = GC.getNumTerrainInfos();
#if defined(BENCHED)
	{
		kUtility.InitializeArray(m_paiYieldPerPopulation, "Trait_YieldPerPopulation", 0);
		kUtility.InitializeArray(m_paiYieldPerPopulationForeignReligion, "Trait_YieldPerPopulationForeignReligion", 0);

		std::string strKey("Trait_YieldPerPopulation");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Yields.ID AS YieldID, Trait_YieldPerPopulation.YieldTimes100, Trait_YieldPerPopulation.ForeignReligion"
				"FROM Trait_YieldPerPopulation"
				"INNER JOIN Yields ON Yields.Type = Trait_YieldPerPopulation.YieldType"
				"WHERE Trait_YieldPerPopulation.TraitType = ?");
		}
		pResults->Bind(1, szTraitType);
		while (pResults->Step())
		{
			const int YieldID = pResults->GetInt(0);
			const int yield = pResults->GetInt(1);
			const bool ForeignReligion = pResults->GetBool(2);

			if (ForeignReligion)
			{
				m_paiYieldPerPopulationForeignReligion[YieldID] = yield;
			}
			else
			{
				m_paiYieldPerPopulation[YieldID] = yield;
			}
		}
		pResults->Reset();
	}
#endif
#if defined(TRAITIFY) // CvTraitEntry::CacheResults, ARRAY
	kUtility.SetYields(m_piPuppetYieldModifiers, "Trait_PuppetYieldModifiers", "TraitType", szTraitType);
	// Route Movement Change.
	{
		kUtility.InitializeArray(m_paiRouteMovementChange, "Routes", 0);

		std::string strKey("Trait_RouteMovementChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Routes.ID, Trait_RouteMovementChanges.MovementChange "
				"FROM Trait_RouteMovementChanges "
				"INNER JOIN Routes ON Trait_RouteMovementChanges.RouteType = Routes.Type "
				"WHERE Trait_RouteMovementChanges.TraitType = ?");
		}
		pResults->Bind(1, szTraitType);
		while (pResults->Step())
		{
			const int RouteID = pResults->GetInt(0);
			const int MovementChange = pResults->GetInt(1);
			m_paiRouteMovementChange[RouteID] = MovementChange;
		}
		pResults->Reset();
	}
	// Trait_BuildingClassRequiredTerrainRemoval
	{
		int BuildingClassLoop;
		for (BuildingClassLoop = 0; BuildingClassLoop < GC.getNumBuildingClassInfos(); BuildingClassLoop++)
		{
			m_abBuildingClassRemoveRequiredTerrain.push_back(false);
		}

		std::string strKey("Trait_BuildingClassRequiredTerrainRemoval");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Traits.ID, BuildingClasses.ID FROM Trait_BuildingClassRequiredTerrainRemoval "
				"INNER JOIN Traits on Trait_BuildingClassRequiredTerrainRemoval.TraitType = Traits.Type "
				"INNER JOIN BuildingClasses on Trait_BuildingClassRequiredTerrainRemoval.BuildingClassType = BuildingClasses.Type "
				"WHERE TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(1);

			m_abBuildingClassRemoveRequiredTerrain[BuildingClassID] = true; // Mark as allowed
		}
	}
	// Trait_UnitClassForceCapitalSpawn
	{
		int iUnitClassLoop;
		for (iUnitClassLoop = 0; iUnitClassLoop < GC.getNumUnitClassInfos(); iUnitClassLoop++)
		{
			m_abUnitClassForceSpawnCapital.push_back(false);
		}

		std::string strKey("Trait_UnitClassForceCapitalSpawn");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Traits.ID as TraitID, UnitClasses.ID as UnitClassID FROM Trait_UnitClassForceCapitalSpawn "
				"INNER JOIN Traits ON Trait_UnitClassForceCapitalSpawn.TraitType = TraitType "
				"INNER JOIN UnitClasses on Trait_UnitClassForceCapitalSpawn.UnitClassType = UnitClasses.Type "
				"WHERE TraitType = ?");
		}

		while (pResults->Step())
		{
			const int UnitClassID = pResults->GetInt(1);

			m_abUnitClassForceSpawnCapital[UnitClassID] = true; // Mark as allowed
		}
	}
	{
		kUtility.Initialize2DArray(m_ppiResourceClassYieldChanges, "ResourceClasses", "Yields");

		std::string strKey("Trait_ResourceClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT ResourceClasses.ID AS ResourceClassID, Yields.ID AS YieldID, Trait_ResourceClassYieldChange.Yield \
			 FROM Trait_ResourceClassYieldChange \
			 INNER JOIN ResourceClasses ON ResourceClasses.Type = Trait_ResourceClassYieldChange.ResourceClassType \
			 INNER JOIN Yields ON Yields.Type = Trait_ResourceClassYieldChange.YieldType \
			 WHERE Trait_ResourceClassYieldChange.TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int ResourceClassID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);
			m_ppiResourceClassYieldChanges[ResourceClassID][YieldID] = yield;
		}

		pResults->Reset();
	}
	// BuildingClassHappiness + Global
	{
		{
			const int iNumBuildingClasses = GC.getNumBuildingClassInfos();

			kUtility.InitializeArray(m_paiBuildingClassHappiness, iNumBuildingClasses, 0);
			kUtility.InitializeArray(m_paiBuildingClassGlobalHappiness, iNumBuildingClasses, 0);

			std::string strKey("Trait_BuildingClassHappinessChanges");
			Database::Results* pResults = kUtility.GetResults(strKey);
			if (pResults == NULL)
			{
				pResults = kUtility.PrepareResults(strKey,
					"SELECT BuildingClasses.ID AS BuildingClassID, Trait_BuildingClassHappinessChanges.Happiness, Trait_BuildingClassHappinessChanges.GlobalHappiness "
					"FROM Trait_BuildingClassHappinessChanges "
					"INNER JOIN BuildingClasses ON BuildingClasses.Type = Trait_BuildingClassHappinessChanges.BuildingClassType "
					"WHERE Trait_BuildingClassHappinessChanges.TraitType = ?");
			}

			pResults->Bind(1, szTraitType);

			while (pResults->Step())
			{
				const int iBuildingClassID = pResults->GetInt(0);
				const int iHappiness = pResults->GetInt(1);
				const bool bGlobal = pResults->GetBool(2);

				if (bGlobal)
				{
					m_paiBuildingClassGlobalHappiness[iBuildingClassID] = iHappiness;
				}
				else
				{
					m_paiBuildingClassHappiness[iBuildingClassID] = iHappiness;
				}
			}
		}
	}
	//BuildingYieldChanges
	{
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldChanges, "BuildingClasses", "Yields");

		std::string strKey("Trait_BuildingClassYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, Yield from Trait_BuildingClassYieldChanges \
inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);

			m_ppiBuildingClassYieldChanges[BuildingClassID][iYieldID] = iYieldChange;
		}
	}
	//Building Cost Override
	{
		kUtility.Initialize2DArray(m_ppiBuildingCostOverride, "Buildings", "Yields");

		std::string strKey("Trait_BuildingCostOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Buildings.ID AS BuildingID, Yields.ID AS YieldID, Trait_BuildingCostOverride.Cost "
				"FROM Trait_BuildingCostOverride "
				"INNER JOIN Buildings ON Buildings.Type = Trait_BuildingCostOverride.BuildingType "
				"INNER JOIN Yields ON Yields.Type = Trait_BuildingCostOverride.YieldType "
				"WHERE Trait_BuildingCostOverride.TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int iBuildingID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iCost = pResults->GetInt(2);

			m_ppiBuildingCostOverride[iBuildingID][iYieldID] = iCost;
		}
	}
	//Trait_ResourceYieldChanges
	{
		kUtility.Initialize2DArray(m_ppiResourceYieldChanges, "Resources", "Yields");

		std::string strKey("Trait_ResourceYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, Yield from Trait_ResourceYieldChanges\
 inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int ResourceID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppiResourceYieldChanges[ResourceID][YieldID] = yield;
		}
	}
	//Trait_TerrainYieldChanges
	{
		kUtility.Initialize2DArray(m_ppiTerrainYieldChanges, "Terrains", "Yields");

		std::string strKey("Trait_TerrainYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Terrains.ID as TerrainID, Yields.ID as YieldID, Yield from Trait_TerrainYieldChanges\
 inner join Terrains on Terrains.Type = TerrainType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int TerrainID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

			m_ppiTerrainYieldChanges[TerrainID][YieldID] = yield;
		}
	}
#endif
	//Trait_Terrains
	{
		kUtility.InitializeArray(m_piStrategicResourceQuantityModifier, iNumTerrains, 0);

		std::string sqlKey = "Trait_Terrains";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if(pResults == NULL)
		{
			const char* szSQL = "select TraitType, Terrains.ID as TerrainID, StrategicResourceQuantityModifier from Trait_Terrains join Terrains on Terrains.Type = TerrainType where TraitType = ?";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int iTerrainID = pResults->GetInt("TerrainID");
			CvAssert(iTerrainID > -1 && iTerrainID < iNumTerrains);

			const int iStrategicResourceQuantityModifier = pResults->GetInt("StrategicResourceQuantityModifier");
			m_piStrategicResourceQuantityModifier[iTerrainID] = iStrategicResourceQuantityModifier;
		}
	}

	//Populate m_FreePromotionUnitCombats
	{
		std::string sqlKey = "FreePromotionUnitCombats";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if(pResults == NULL)
		{
			const char* szSQL = "select UnitPromotions.ID, UnitCombatInfos.ID from Trait_FreePromotionUnitCombats, UnitPromotions, UnitCombatInfos where TraitType = ? and PromotionType = UnitPromotions.Type and UnitCombatType = UnitCombatInfos.Type";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int unitPromotionID = pResults->GetInt(0);
			const int unitCombatInfoID = pResults->GetInt(1);

			m_FreePromotionUnitCombats.insert(std::pair<int, int>(unitPromotionID, unitCombatInfoID));
		}

		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::multimap<int,int>(m_FreePromotionUnitCombats).swap(m_FreePromotionUnitCombats);

		kUtility.PopulateArrayByValue(m_piResourceQuantityModifiers, "Resources", "Trait_ResourceQuantityModifiers", "ResourceType", "TraitType", szTraitType, "ResourceQuantityModifier");
	}

	//Populate m_MovesChangeUnitCombats
	{
		const int iNumUnitCombatClasses = kUtility.MaxRows("UnitCombatInfos");
		kUtility.InitializeArray(m_piMovesChangeUnitCombats, iNumUnitCombatClasses, 0);

		std::string sqlKey = "Trait_MovesChangeUnitCombats";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if(pResults == NULL)
		{
			const char* szSQL = "select UnitCombatInfos.ID, MovesChange from Trait_MovesChangeUnitCombats inner join UnitCombatInfos on UnitCombatInfos.Type = UnitCombatType where TraitType = ?;";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int iUnitCombatID = pResults->GetInt(0);
			CvAssert(iUnitCombatID > -1 && iUnitCombatID < iNumUnitCombatClasses);

			const int iMovesChange = pResults->GetInt(1);
			m_piMovesChangeUnitCombats[iUnitCombatID] = iMovesChange;
		}

		pResults->Reset();
	}

	//Populate m_MaintenanceModifierUnitCombats
	{
		const int iNumUnitCombatClasses = kUtility.MaxRows("UnitCombatInfos");
		kUtility.InitializeArray(m_piMaintenanceModifierUnitCombats, iNumUnitCombatClasses, 0);

		std::string sqlKey = "Trait_MaintenanceModifierUnitCombats";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if(pResults == NULL)
		{
			const char* szSQL = "select UnitCombatInfos.ID, MaintenanceModifier from Trait_MaintenanceModifierUnitCombats inner join UnitCombatInfos on UnitCombatInfos.Type = UnitCombatType where TraitType = ?;";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int iUnitCombatID = pResults->GetInt(0);
			CvAssert(iUnitCombatID > -1 && iUnitCombatID < iNumUnitCombatClasses);

			const int iMaintenanceModifier = pResults->GetInt(1);
			m_piMaintenanceModifierUnitCombats[iUnitCombatID] = iMaintenanceModifier;
		}

		pResults->Reset();
	}

	//ImprovementYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiImprovementYieldChanges.first, "Improvements", "Yields");
		m_ppiImprovementYieldChanges.second = kUtility.MaxRows("Improvements");
#else
		kUtility.Initialize2DArray(m_ppiImprovementYieldChanges, "Improvements", "Yields");
#endif

		std::string strKey("Trait_ImprovementYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield from Trait_ImprovementYieldChanges inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int ImprovementID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiImprovementYieldChanges.first[ImprovementID][YieldID] = yield;
#else
			m_ppiImprovementYieldChanges[ImprovementID][YieldID] = yield;
#endif
		}
	}

	//SpecialistYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiSpecialistYieldChanges.first, "Specialists", "Yields");
		m_ppiSpecialistYieldChanges.second = kUtility.MaxRows("Specialists");
#else
		kUtility.Initialize2DArray(m_ppiSpecialistYieldChanges, "Specialists", "Yields");
#endif


		std::string strKey("Building_SpecialistYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield from Trait_SpecialistYieldChanges inner join Specialists on Specialists.Type = SpecialistType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int SpecialistID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiSpecialistYieldChanges.first[SpecialistID][YieldID] = yield;
#else
			m_ppiSpecialistYieldChanges[SpecialistID][YieldID] = yield;
#endif

		}
	}

#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	//AnySpecificSpecialistYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiAnySpecificSpecialistYieldChanges.first, "Specialists", "Yields");
		m_ppiAnySpecificSpecialistYieldChanges.second = kUtility.MaxRows("Specialists");
#else
		kUtility.Initialize2DArray(m_ppiAnySpecificSpecialistYieldChanges, "Specialists", "Yields");
#endif


		std::string strKey("Building_SpecialistYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield from Trait_AnySpecificSpecialistYieldChanges inner join Specialists on Specialists.Type = SpecialistType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int SpecialistID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiAnySpecificSpecialistYieldChanges.first[SpecialistID][YieldID] = yield;
#else
			m_ppiAnySpecificSpecialistYieldChanges[SpecialistID][YieldID] = yield;
#endif

		}
	}
#endif
#if !defined(TRAITIFY) // Altering the UnimprovedFeatureYieldChanges to also include improved features, optionally
	//UnimprovedFeatureYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiUnimprovedFeatureYieldChanges.first, "Features", "Yields");
		m_ppiUnimprovedFeatureYieldChanges.second = kUtility.MaxRows("Features");
#else
		kUtility.Initialize2DArray(m_ppiUnimprovedFeatureYieldChanges, "Features", "Yields");
#endif

		std::string strKey("Trait_UnimprovedFeatureYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Features.ID as FeatureID, Yields.ID as YieldID, Yield from Trait_UnimprovedFeatureYieldChanges inner join Features on Features.Type = FeatureType inner join Yields on Yields.Type = YieldType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			const int FeatureID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiUnimprovedFeatureYieldChanges.first[FeatureID][YieldID] = yield;
#else
			m_ppiUnimprovedFeatureYieldChanges[FeatureID][YieldID] = yield;
#endif
		}
	}
#else
	{

		kUtility.Initialize2DArray(m_ppiUnimprovedFeatureYieldChanges, "Features", "Yields");
		kUtility.Initialize2DArray(m_ppiFeatureYieldChanges, "Features", "Yields");

		std::string strKey("Trait_FeatureYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, 
				"SELECT AllowImprovement, Features.ID as FeatureID, Yields.ID as YieldID, Yield "
				"FROM Trait_FeatureYieldChanges "
				"INNER JOIN Features ON Features.Type = FeatureType "
				"INNER JOIN Yields ON Yields.Type = YieldType " 
				"WHERE TraitType = ? ");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const bool bAllowImprovement = pResults->GetBool(0);
			const int iFeatureType = pResults->GetInt(1);
			const int iYieldType = pResults->GetInt(2);
			const int iYield = pResults->GetInt(3);

			if (bAllowImprovement)
			{
				m_ppiFeatureYieldChanges[iFeatureType][iYieldType] = iYield;
			}
			else
			{
				m_ppiUnimprovedFeatureYieldChanges[iFeatureType][iYieldType] = iYield;
			}
		}

	}

#endif

	// NoTrain
	{
#ifdef AUI_WARNING_FIXES
		for (uint iUnitClassLoop = 0; iUnitClassLoop < GC.getNumUnitClassInfos(); iUnitClassLoop++)
#else
		int iUnitClassLoop;
		for (iUnitClassLoop = 0; iUnitClassLoop < GC.getNumUnitClassInfos(); iUnitClassLoop++)
#endif
		{
			m_abNoTrainUnitClass.push_back(false);
		}

		std::string strKey("Trait_NoTrain");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "SELECT Traits.ID, UnitClasses.ID FROM Trait_NoTrain inner join Traits on Trait_NoTrain.TraitType = Traits.Type inner join UnitClasses on Trait_NoTrain.UnitClassType = UnitClasses.Type where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int iUnitClass = pResults->GetInt(1);
			m_abNoTrainUnitClass[iUnitClass] = true;
		}
	}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	// NoBuild (Improvements)
	{
		for (int iImprovementLoop = 0; iImprovementLoop < GC.getNumImprovementInfos(); iImprovementLoop++)
		{
			m_abNoBuildImprovements.push_back(false);
		}

		std::string strKey("Trait_NoBuildImprovement");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "SELECT Traits.ID, Improvements.ID FROM Trait_NoBuildImprovement inner join Traits on Trait_NoBuildImprovement.TraitType = Traits.Type inner join Improvements on Trait_NoBuildImprovement.ImprovementType = Improvements.Type where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while (pResults->Step())
		{
			const int iImprovement = pResults->GetInt(1);
			m_abNoBuildImprovements[iImprovement] = true;
		}

	}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	// Build Improvement Build Override from the builds table
	{
		// Initialize the backward compatibility vectors
		const int iNumBuilds = kUtility.MaxRows("Builds");
		if (iNumBuilds > 0)
		{
			m_aiBuildTimeOverride.resize(iNumBuilds, -1);
			m_aiBuildTimeOverrideResourceClassRequired.resize(iNumBuilds, NO_RESOURCECLASS);
		}

		// Clear the multimap
		m_BuildTimeOverrides.clear();

		std::string strKey("Trait_BuildImprovementBuildTimeOverride");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, 
				"SELECT Builds.ID as BuildID, Trait_BuildImprovementBuildTimeOverride.Time, COALESCE(ResourceClasses.ID, -1) as ResourceClassID "
				"FROM Trait_BuildImprovementBuildTimeOverride "
				"LEFT JOIN ResourceClasses ON Trait_BuildImprovementBuildTimeOverride.ResourceClassRequired = ResourceClasses.Type "
				"INNER JOIN Traits ON Trait_BuildImprovementBuildTimeOverride.TraitType = Traits.Type "
				"INNER JOIN Builds ON Trait_BuildImprovementBuildTimeOverride.BuildType = Builds.Type "
				"WHERE TraitType = ?");
		}

		if (pResults != NULL)
		{
			pResults->Bind(1, szTraitType);

			while (pResults->Step())
			{
				const int iBuildID = pResults->GetInt(0);
				const int iBuildTime = pResults->GetInt(1);
				const int iResourceClassID = pResults->GetInt(2);
				ResourceClassTypes eResourceClass = NO_RESOURCECLASS;

				if (iResourceClassID != -1)
				{
					eResourceClass = (ResourceClassTypes)iResourceClassID;
				}
				
				// Add to the multimap - this allows multiple entries per build type
				m_BuildTimeOverrides.insert(std::make_pair((BuildTypes)iBuildID, std::make_pair(iBuildTime, eResourceClass)));

				// Also update the vectors for backward compatibility (keep the last entry)
				if (iBuildID >= 0 && iBuildID < iNumBuilds)
				{
					m_aiBuildTimeOverride[iBuildID] = iBuildTime;
					m_aiBuildTimeOverrideResourceClassRequired[iBuildID] = eResourceClass;
				}
			}

			pResults->Reset();
		}

		//Trim extra memory off container since this is mostly read-only.
		std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>>(m_BuildTimeOverrides).swap(m_BuildTimeOverrides);
	}
#endif

	// FreeResourceXCities
	{
		// Init vector
#ifdef AUI_WARNING_FIXES
		for (uint iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#else
		int iResourceLoop;
		for(iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#endif
		{
			FreeResourceXCities temp;
			m_aFreeResourceXCities.push_back(temp);
		}

		std::string strKey("Trait_FreeResourceFirstXCities");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, ResourceQuantity, NumCities from Trait_FreeResourceFirstXCities inner join Resources on Resources.Type = ResourceType where TraitType = ?");
		}

		pResults->Bind(1, szTraitType);

		while(pResults->Step())
		{
			FreeResourceXCities temp;

			const int iResource = pResults->GetInt(0);
			temp.m_iResourceQuantity = pResults->GetInt(1);
			temp.m_iNumCities = pResults->GetInt(2);

			m_aFreeResourceXCities[iResource] = temp;
		}
	}

	return true;
}

//=====================================
// CvTraitXMLEntries
//=====================================
/// Constructor
CvTraitXMLEntries::CvTraitXMLEntries(void)
{

}

/// Destructor
CvTraitXMLEntries::~CvTraitXMLEntries(void)
{
	DeleteArray();
}

/// Returns vector of trait entries
std::vector<CvTraitEntry*>& CvTraitXMLEntries::GetTraitEntries()
{
	return m_paTraitEntries;
}

/// Number of defined traits
int CvTraitXMLEntries::GetNumTraits()
{
	return m_paTraitEntries.size();
}

/// Clear trait entries
void CvTraitXMLEntries::DeleteArray()
{
	for(std::vector<CvTraitEntry*>::iterator it = m_paTraitEntries.begin(); it != m_paTraitEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paTraitEntries.clear();
}

/// Get a specific entry
CvTraitEntry* CvTraitXMLEntries::GetEntry(int index)
{
	return m_paTraitEntries[index];
}

//=====================================
// CvPlayerTraits
//=====================================
/// Constructor
CvPlayerTraits::CvPlayerTraits()
{
}

/// Destructor
CvPlayerTraits::~CvPlayerTraits(void)
{
	Uninit();
}

/// Initialize
void CvPlayerTraits::Init(CvTraitXMLEntries* pTraits, CvPlayer* pPlayer)
{
	// Store off the pointers to objects we'll need later
	m_pTraits = pTraits;
	m_pPlayer = pPlayer;

	Reset();

	// Initialize items that can't be reset everytime we earn a new tech (which resets all the other
	// trait data that can rebuilt from the trait entries)
	m_iBaktunPreviousTurn = 0;
	m_aMayaBonusChoices.clear();
	m_iBaktun = 0;
	m_iKatun = 0;
	m_iTun = 0;
	m_iWinal = 0;
	m_iKin = 0;

	m_aUniqueLuxuryAreas.clear();
	m_iUniqueLuxuryCitiesPlaced = 0;

	m_vLeaderHasTrait = std::vector<bool>( GC.getNumTraitInfos(), false );
}

/// Store off data on bonuses from traits
void CvPlayerTraits::InitPlayerTraits()
{

	// precompute the traits our leader has
	m_vPotentiallyActiveLeaderTraits.clear();
	for(int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		m_vLeaderHasTrait[iI] = false;
		if (m_pPlayer && m_pPlayer->isMajorCiv() && m_pPlayer->isAlive() && m_pPlayer->getLeaderInfo().hasTrait( (TraitTypes)iI ))
		{
			m_vLeaderHasTrait[iI] = true;
			m_vPotentiallyActiveLeaderTraits.push_back( (TraitTypes)iI );
		}
	}
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumTraitInfos(); iI++)
#else
	for(int iI = 0; iI < GC.getNumTraitInfos(); iI++)
#endif

	{
		if(HasTrait((TraitTypes)iI))
		{
			CvTraitEntry* trait = GC.getTraitInfo((TraitTypes)iI);
			m_iGreatPeopleRateModifier += trait->GetGreatPeopleRateModifier();
			m_iGreatScientistRateModifier += trait->GetGreatScientistRateModifier();
			m_iGreatGeneralRateModifier += trait->GetGreatGeneralRateModifier();
			m_iGreatGeneralExtraBonus += trait->GetGreatGeneralExtraBonus();
			m_iGreatPersonGiftInfluence += trait->GetGreatPersonGiftInfluence();
			m_iLevelExperienceModifier += trait->GetLevelExperienceModifier();
			m_iMaxGlobalBuildingProductionModifier += trait->GetMaxGlobalBuildingProductionModifier();
			m_iMaxTeamBuildingProductionModifier += trait->GetMaxTeamBuildingProductionModifier();
			m_iMaxPlayerBuildingProductionModifier += trait->GetMaxPlayerBuildingProductionModifier();
			m_iCityUnhappinessModifier += trait->GetCityUnhappinessModifier();
			m_iPopulationUnhappinessModifier += trait->GetPopulationUnhappinessModifier();
			m_iCityStateBonusModifier += trait->GetCityStateBonusModifier();
			m_iCityStateFriendshipModifier += trait->GetCityStateFriendshipModifier();
			m_iCityStateCombatModifier += trait->GetCityStateCombatModifier();
			m_iLandBarbarianConversionPercent += trait->GetLandBarbarianConversionPercent();
			m_iLandBarbarianConversionExtraUnits += trait->GetLandBarbarianConversionExtraUnits();
			m_iSeaBarbarianConversionPercent += trait->GetSeaBarbarianConversionPercent();
			m_iCapitalBuildingModifier += trait->GetCapitalBuildingModifier();
			m_iPlotBuyCostModifier += trait->GetPlotBuyCostModifier();
			m_iPlotCultureCostModifier += trait->GetPlotCultureCostModifier();
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
			m_iSciencePerGreatPersonBorn += trait->GetSciencePerGreatPersonBorn();
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
			m_iNumTurnsBeforeMinorAlliesRefuseBribes += trait->GetNumTurnsBeforeMinorAlliesRefuseBribes();
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
			m_iGoldenAgeTileBonusFaith += trait->GetGoldenAgeTileBonusFaith();
#endif
			m_iCultureFromKills += trait->GetCultureFromKills();
			m_iFaithFromKills += trait->GetFaithFromKills();
			m_iCityCultureBonus += trait->GetCityCultureBonus();
			m_iCapitalThemingBonusModifier += trait->GetCapitalThemingBonusModifier();
			m_iPolicyCostModifier += trait->GetPolicyCostModifier();
			m_iCityConnectionTradeRouteChange += trait->GetCityConnectionTradeRouteChange();
			m_iWonderProductionModifier += trait->GetWonderProductionModifier();
			m_iPlunderModifier += trait->GetPlunderModifier();
			m_iImprovementMaintenanceModifier += trait->GetImprovementMaintenanceModifier();
			m_iGoldenAgeDurationModifier += trait->GetGoldenAgeDurationModifier();
			m_iGoldenAgeMoveChange += trait->GetGoldenAgeMoveChange();
			m_iGoldenAgeCombatModifier += trait->GetGoldenAgeCombatModifier();
			m_iGoldenAgeTourismModifier += trait->GetGoldenAgeTourismModifier();
			m_iGoldenAgeGreatArtistRateModifier += trait->GetGoldenAgeGreatArtistRateModifier();
			m_iGoldenAgeGreatMusicianRateModifier += trait->GetGoldenAgeGreatMusicianRateModifier();
			m_iGoldenAgeGreatWriterRateModifier += trait->GetGoldenAgeGreatWriterRateModifier();
			m_iExtraEmbarkMoves += trait->GetExtraEmbarkMoves();
			m_iNaturalWonderFirstFinderGold += trait->GetNaturalWonderFirstFinderGold();
			m_iNaturalWonderSubsequentFinderGold += trait->GetNaturalWonderSubsequentFinderGold();
#if defined(TRAITIFY) // CvPlayerTraits::InitPlayerTraits
			m_bHalfMoreSpecialistUnhappiness = trait->IsHalfMoreSpecialistUnhappiness();

			m_iGoldenAgeCultureModifier += trait->GetGoldenAgeCultureModifier();
			m_iNumExtraLeagueVotes += trait->GetNumExtraLeagueVotes();
			m_iNumTradeRouteBonus += trait->GetNumTradeRouteBonus();
			m_iMinorFriendshipMinimum += trait->GetMinorFriendshipMinimum();
			m_iGreatEngineerRateModifier += trait->GetGreatEngineerRateModifier();
			m_iGreatMerchantRateModifier += trait->GetGreatMerchantRateModifier();
			m_iMinorBullyModifier += trait->GetMinorBullyModifier();
			m_iInternationalRouteGrowthModifier += trait->GetInternationalRouteGrowthModifier();
			m_iLocalHappinessPerCity += trait->GetLocalHappinessPerCity();
			m_iInternalTradeRouteYieldModifier += trait->GetInternalTradeRouteYieldModifier();
			m_iUnhappinessModifierForPuppetedCities += trait->GetUnhappinessModifierForPuppetedCities();
			m_iExtraPopulation += trait->GetExtraPopulation();
			m_iFaithCostModifier += trait->GetFaithCostModifier();
			m_iIdeologyPressureUnhappinessModifier += trait->GetIdeologyPressureUnhappinessModifier();
			m_iForeignRelgionPressureModifier += trait->GetForeignRelgionPressureModifier();
			m_iFriendlyLandsCitizenMoveChange += trait->GetFriendlyLandsCitizenMoveChange();
#endif
#if defined(LEKMOD_v34)
			m_bReligionEnhanceReformation = trait->IsReligionEnhanceReformation();
			m_iSelfReligiousPressureModifier += trait->GetSelfReligiousPressureModifier();
			m_iLandTradeRouteYieldBonus += trait->GetLandTradeRouteYieldBonus();
#endif
			//EAP: Natural wonder faith for the finder
			m_iNaturalWonderFirstFinderFaith += trait->GetNaturalWonderFirstFinderFaith();
			m_iNaturalWonderSubsequentFinderFaith += trait->GetNaturalWonderSubsequentFinderFaith();

			m_iNaturalWonderYieldModifier += trait->GetNaturalWonderYieldModifier();
			m_iNaturalWonderHappinessModifier += trait->GetNaturalWonderHappinessModifier();
			m_iNearbyImprovementCombatBonus += trait->GetNearbyImprovementCombatBonus();
			m_iNearbyImprovementBonusRange += trait->GetNearbyImprovementBonusRange();
			m_iCultureBuildingYieldChange += trait->GetCultureBuildingYieldChange();
			m_iCombatBonusVsHigherTech += trait->GetCombatBonusVsHigherTech();
			m_iCombatBonusVsLargerCiv += trait->GetCombatBonusVsLargerCiv();
			m_iLandUnitMaintenanceModifier += trait->GetLandUnitMaintenanceModifier();
			m_iNavalUnitMaintenanceModifier += trait->GetNavalUnitMaintenanceModifier();
			m_iRazeSpeedModifier += trait->GetRazeSpeedModifier();
			m_iDOFGreatPersonModifier += trait->GetDOFGreatPersonModifier();
			m_iLuxuryHappinessRetention += trait->GetLuxuryHappinessRetention();
			m_iExtraHappinessPerLuxury += trait->GetExtraHappinessPerLuxury(); // NQMP GJS - New Netherlands UA
			m_iExtraSpies += trait->GetExtraSpies();
			m_iHappinessPerReligion += trait->GetHappinessPerReligion(); // NQMP GJS - New Ottoman UA
			m_iUnresearchedTechBonusFromKills += trait->GetUnresearchedTechBonusFromKills();
			m_iExtraFoundedCityTerritoryClaimRange += trait->GetExtraFoundedCityTerritoryClaimRange();
			m_iFreeSocialPoliciesPerEra += trait->GetFreeSocialPoliciesPerEra();
			m_iNumTradeRoutesModifier += trait->GetNumTradeRoutesModifier();
			m_iTradeRouteResourceModifier += trait->GetTradeRouteResourceModifier();
			m_iUniqueLuxuryCities += trait->GetUniqueLuxuryCities();
			m_iUniqueLuxuryQuantity	+= trait->GetUniqueLuxuryQuantity();
			m_iWorkerSpeedModifier += trait->GetWorkerSpeedModifier();
			m_iAfraidMinorPerTurnInfluence += trait->GetAfraidMinorPerTurnInfluence();
			m_iLandTradeRouteRangeBonus += trait->GetLandTradeRouteRangeBonus();
			m_iTradeReligionModifier += trait->GetTradeReligionModifier();
			m_iTradeBuildingModifier += trait->GetTradeBuildingModifier();

			if(trait->IsFightWellDamaged())
			{
				m_bFightWellDamaged = true;
				// JON: Changing the way this works. Above line can/should probably be removed at some point
				int iWoundedUnitDamageMod = /*-33*/ GC.getTRAIT_WOUNDED_DAMAGE_MOD();
				m_pPlayer->ChangeWoundedUnitDamageMod(iWoundedUnitDamageMod);
			}
			if(trait->IsMoveFriendlyWoodsAsRoad())
			{
				m_bMoveFriendlyWoodsAsRoad = true;
			}
			if(trait->IsFasterAlongRiver())
			{
				m_bFasterAlongRiver = true;
			}
			if(trait->IsFasterInHills())
			{
				m_bFasterInHills = true;
			}
			if(trait->IsEmbarkedAllWater())
			{
				m_bEmbarkedAllWater = true;
			}
			if(trait->IsEmbarkedToLandFlatCost())
			{
				m_bEmbarkedToLandFlatCost = true;
			}
			if(trait->IsNoHillsImprovementMaintenance())
			{
				m_bNoHillsImprovementMaintenance = true;
			}
			if(trait->IsTechBoostFromCapitalScienceBuildings())
			{
				m_bTechBoostFromCapitalScienceBuildings = true;
			}
			if(trait->IsStaysAliveZeroCities())
			{
				m_bStaysAliveZeroCities = true;
			}
			if(trait->IsFaithFromUnimprovedForest())
			{
				m_bFaithFromUnimprovedForest = true;
			}
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
			if(trait->IsSeaTradeRoutesArePlunderImmune())
			{
				m_bSeaTradeRoutesArePlunderImmune = true;
			}
#endif
			// NQMP GJS - New France UA begin
			if(trait->IsEarnsGreatPersonOnSlotOrGuild())
			{
				m_bEarnsGreatPersonOnSlotOrGuild = true;
			}
			// NQMP GJS - New France UA end
			if(trait->IsBonusReligiousBelief())
			{
				m_bBonusReligiousBelief = true;
			}
			if(trait->IsAbleToAnnexCityStates())
			{
				m_bAbleToAnnexCityStates = true;
			}
			if(trait->IsCrossesMountainsAfterGreatGeneral())
			{
				m_bCrossesMountainsAfterGreatGeneral = true;
			}
			if(trait->IsMayaCalendarBonuses())
			{
				m_bMayaCalendarBonuses = true;
			}
			if (trait->IsNoAnnexing())
			{
				m_bNoAnnexing = true;
			}
			if (trait->IsTechFromCityConquer())
			{
				m_bTechFromCityConquer = true;
			}
			if (trait->IsUniqueLuxuryRequiresNewArea())
			{
				m_bUniqueLuxuryRequiresNewArea = true;
			}
			if (trait->IsRiverTradeRoad())
			{
				m_bRiverTradeRoad = true;
			}
			if (trait->IsAngerFreeIntrusionOfCityStates())
			{
				m_bAngerFreeIntrusionOfCityStates = true;
			}

			for(int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
			{
				if(trait->GetExtraYieldThreshold(iYield) > m_iExtraYieldThreshold[iYield])
				{
					m_iExtraYieldThreshold[iYield] = trait->GetExtraYieldThreshold(iYield);
				}
				m_iFreeCityYield[iYield] = trait->GetYieldChange(iYield);
				m_iYieldChangeStrategicResources[iYield] = trait->GetYieldChangeStrategicResources(iYield);
				m_iYieldChangeLuxuryResources[iYield] = trait->GetYieldChangeLuxuryResources(iYield); // NQMP GJS - New Netherlands UA
				m_iYieldChangeNaturalWonder[iYield] = trait->GetYieldChangeNaturalWonder(iYield);
				m_iYieldChangePerTradePartner[iYield] = trait->GetYieldChangePerTradePartner(iYield);
				m_iYieldChangeIncomingTradeRoute[iYield] = trait->GetYieldChangeIncomingTradeRoute(iYield);
				m_iYieldRateModifier[iYield] = trait->GetYieldModifier(iYield);
#if defined(FULL_YIELD_FROM_KILLS) // CvPlayerTraits::InitPlayerTraits, Arrays
				m_iYieldFromKills[iYield] = trait->GetYieldFromKills(iYield);
#endif
#if defined(TRAITIFY)
				m_iPuppetYieldModifiers[iYield] = trait->GetPuppetYieldModifiers(iYield);
#endif

#ifdef AUI_WARNING_FIXES
				for (uint iFeatureLoop = 0; iFeatureLoop < GC.getNumFeatureInfos(); iFeatureLoop++)
#else
				for (int iFeatureLoop = 0; iFeatureLoop < GC.getNumFeatureInfos(); iFeatureLoop++)
#endif
				{
					int iChange = trait->GetUnimprovedFeatureYieldChanges((FeatureTypes)iFeatureLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiUnimprovedFeatureYieldChange[iFeatureLoop];
						yields[iYield] = (m_ppaaiUnimprovedFeatureYieldChange[iFeatureLoop][iYield] + iChange);
						m_ppaaiUnimprovedFeatureYieldChange[iFeatureLoop] = yields;
					}
#if defined(TRAITIFY) // Load CvTraitEntry arrays into CvPlayerTraitArrays
					// This one allows Improvements on Features in CvPlot.cpp
					iChange = trait->GetFeatureYieldChanges((FeatureTypes)iFeatureLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiFeatureYieldChange[iFeatureLoop];
						yields[iYield] = (m_ppaaiFeatureYieldChange[iFeatureLoop][iYield] + iChange);
						m_ppaaiFeatureYieldChange[iFeatureLoop] = yields;
					}
#endif
				}
#if defined(TRAITIFY) // Load CvTraitEntry arrays into CvPlayerTraitArrays
				// Terrain Yield Changes
				for (int iTerrainLoop = 0; iTerrainLoop < GC.getNumTerrainInfos(); iTerrainLoop++)
				{
					int iChange = trait->GetTerrainYieldChanges((TerrainTypes)iTerrainLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiTerrainYieldChange[iTerrainLoop];
						yields[iYield] = (m_ppaaiTerrainYieldChange[iTerrainLoop][iYield] + iChange);
						m_ppaaiTerrainYieldChange[iTerrainLoop] = yields;
					}
				}
				// Resource Yield Changes
				for (int iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
				{
					int iChange = trait->GetResourceYieldChanges((ResourceTypes)iResourceLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiResourceYieldChange[iResourceLoop];
						yields[iYield] = (m_ppaaiResourceYieldChange[iResourceLoop][iYield] + iChange);
						m_ppaaiResourceYieldChange[iResourceLoop] = yields;
					}
				}
				// ResourceClass Yield Changes
				for (int iResourceClassLoop = 0; iResourceClassLoop < GC.getNumResourceClassInfos(); iResourceClassLoop++)
				{
					int iChange = trait->GetResourceClassYieldChanges((ResourceClassTypes)iResourceClassLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiResourceClassYieldChange[iResourceClassLoop];
						yields[iYield] = (m_ppaaiResourceClassYieldChange[iResourceClassLoop][iYield] + iChange);
						m_ppaaiResourceClassYieldChange[iResourceClassLoop] = yields;
					}
				}
				for (int iBuildingClassLoop = 0; iBuildingClassLoop < GC.getNumBuildingClassInfos(); iBuildingClassLoop++)
				{
					int iChange = trait->GetBuildingClassYieldChanges((BuildingClassTypes)iBuildingClassLoop, (YieldTypes)iYield);
					if (iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiBuildingClassYieldChange[iBuildingClassLoop];
						yields[iYield] = (m_ppaaiBuildingClassYieldChange[iBuildingClassLoop][iYield] + iChange);
						m_ppaaiBuildingClassYieldChange[iBuildingClassLoop] = yields;
					}
				}
				for (int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
				{
					int iChange = trait->GetBuildingCostOverride((BuildingTypes)iBuildingLoop, (YieldTypes)iYield);
					if (iChange != 0) // 0 is not allowed, but negatives are.
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiBuildingCostOverride[iBuildingLoop];
						yields[iYield] = (m_ppaaiBuildingCostOverride[iBuildingLoop][iYield] + iChange);
						m_ppaaiBuildingCostOverride[iBuildingLoop] = yields;
					}
				}
#endif

#ifdef AUI_WARNING_FIXES
				for (uint iImprovementLoop = 0; iImprovementLoop < GC.getNumImprovementInfos(); iImprovementLoop++)
#else
				for(int iImprovementLoop = 0; iImprovementLoop < GC.getNumImprovementInfos(); iImprovementLoop++)
#endif
				{
					int iChange = trait->GetImprovementYieldChanges((ImprovementTypes)iImprovementLoop, (YieldTypes)iYield);
					if(iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiImprovementYieldChange[iImprovementLoop];
						yields[iYield] = (m_ppaaiImprovementYieldChange[iImprovementLoop][iYield] + iChange);
						m_ppaaiImprovementYieldChange[iImprovementLoop] = yields;
					}
				}

#ifdef AUI_WARNING_FIXES
				for (uint iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#else
				for(int iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#endif
				{
					int iChange = trait->GetSpecialistYieldChanges((SpecialistTypes)iSpecialistLoop, (YieldTypes)iYield);
					if(iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiSpecialistYieldChange[iSpecialistLoop];
						yields[iYield] = (m_ppaaiSpecialistYieldChange[iSpecialistLoop][iYield] + iChange);
						m_ppaaiSpecialistYieldChange[iSpecialistLoop] = yields;
					}
				}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
				for(int iSpecialistLoop = 0; iSpecialistLoop < 2; iSpecialistLoop++)

				{
					int iChange = trait->GetAnySpecificSpecialistYieldChanges((SpecialistTypes)iSpecialistLoop, (YieldTypes)iYield);
					if(iChange > 0)
					{
						Firaxis::Array<int, NUM_YIELD_TYPES> yields = m_ppaaiAnySpecificSpecialistYieldChange[iSpecialistLoop];
						yields[iYield] = (m_ppaaiAnySpecificSpecialistYieldChange[iSpecialistLoop][iYield] + iChange);
						m_ppaaiAnySpecificSpecialistYieldChange[iSpecialistLoop] = yields;
					}
				}
#endif
			} // END NUM_YIELD_TYPES loop
			CvAssert(GC.getNumTerrainInfos() <= NUM_TERRAIN_TYPES);
#if defined(TRAITIFY) // Building Class Loop for non Yield Arrays
			for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
			{
				m_aiBuildingClassHappiness[iBuildingClass] = trait->GetBuildingClassHappiness((BuildingClassTypes)iBuildingClass);
				m_aiBuildingClassGlobalHappiness[iBuildingClass] = trait->GetBuildingClassGlobalHappiness((BuildingClassTypes)iBuildingClass);
				m_abRemoveRequiredTerrain[iBuildingClass] = trait->IsBuildingClassRemoveRequiredTerrain((BuildingClassTypes)iBuildingClass);
			}
			for (int iRouteType = 0; iRouteType < NUM_ROUTE_TYPES; iRouteType++)
			{
				m_iRouteMovementChange[iRouteType] = trait->GetRouteMovementChange((RouteTypes)iRouteType);
			}
#endif
#ifdef AUI_WARNING_FIXES
			for (uint iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
#else
			for(int iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
#endif
			{
				m_iStrategicResourceQuantityModifier[iTerrain] = trait->GetStrategicResourceQuantityModifier(iTerrain);
			}

#ifdef AUI_WARNING_FIXES
			for (uint iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
#else
			for(int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
#endif
			{
				m_aiResourceQuantityModifier[iResource] = trait->GetResourceQuantityModifier(iResource);
			}

#ifdef AUI_WARNING_FIXES
			for (uint iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
#else
			for (int iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
#endif
			{
				m_abNoTrain[iUnitClass] = trait->NoTrain((UnitClassTypes)iUnitClass);
#if defined(TRAITIFY) // UnitClassForceSpawnCapital insert
				m_abForceSpawnCapital[iUnitClass] = trait->IsUnitClassForceSpawnCapital((UnitClassTypes)iUnitClass);
#endif
			}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
			for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
			{
				m_abNoBuild[iImprovement] = trait->NoBuildImprovements((ImprovementTypes)iImprovement);
			}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	// Copy the backward compatibility vectors
	for (int iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
	{
		m_aiBuildTimeOverride[iBuild] = trait->GetBuildTimeOverrideVector(iBuild);
		m_aiBuildTimeOverrideResourceClassRequired[iBuild] = trait->GetBuildTimeOverrideResourceClassRequiredVector(iBuild);
	}

	// Copy all build time overrides in the multimap
	typedef std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>>::const_iterator it_type;
	const std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>>& buildTimeOverrides = trait->GetBuildTimeOverridesMultimap();
	for (it_type it = buildTimeOverrides.begin(); it != buildTimeOverrides.end(); ++it)
	{
		m_BuildTimeOverrides.insert(*it);
	}
#endif

			FreeTraitUnit traitUnit;
#ifdef AUI_WARNING_FIXES
			traitUnit.m_iFreeUnit = (UnitClassTypes)trait->GetFreeUnitClassType();
#else
			traitUnit.m_iFreeUnit = (UnitTypes)trait->GetFreeUnitClassType();
#endif
			if(traitUnit.m_iFreeUnit != NO_UNITCLASS)
			{
				traitUnit.m_ePrereqTech = trait->GetFreeUnitPrereqTech();
				m_aFreeTraitUnits.push_back(traitUnit);
			}

			m_eCombatBonusImprovement = trait->GetCombatBonusImprovement();

#ifdef AUI_WARNING_FIXES
			for (uint jJ = 0; jJ < GC.getNumUnitCombatClassInfos(); jJ++)
#else
			int iNumUnitCombatClassInfos = GC.getNumUnitCombatClassInfos();
			for(int jJ= 0; jJ < iNumUnitCombatClassInfos; jJ++)
#endif
			{
				m_paiMovesChangeUnitCombat[jJ] += trait->GetMovesChangeUnitCombat(jJ);
				m_paiMaintenanceModifierUnitCombat[jJ] += trait->GetMaintenanceModifierUnitCombat(jJ);
			}

#ifdef AUI_WARNING_FIXES
			for (uint iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#else
			for(int iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#endif
			{
				FreeResourceXCities temp = trait->GetFreeResourceXCities((ResourceTypes)iResourceLoop);
				if(temp.m_iResourceQuantity > 0)
				{
					m_aFreeResourceXCities[iResourceLoop] = temp;
				}
			}
		}
	}
}

/// Deallocate memory created in initialize
void CvPlayerTraits::Uninit()
{
	m_aiResourceQuantityModifier.clear();
	m_abNoTrain.clear();

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	m_abNoBuild.clear();
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	m_aiBuildTimeOverride.clear();
#endif
	m_paiMovesChangeUnitCombat.clear();
	m_paiMaintenanceModifierUnitCombat.clear();
	m_ppaaiImprovementYieldChange.clear();
	m_ppaaiSpecialistYieldChange.clear();

#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	m_ppaaiAnySpecificSpecialistYieldChange.clear();
#endif

	m_ppaaiUnimprovedFeatureYieldChange.clear();
#if defined(TRAITIFY) // CvPlayerTraits::Uninit
	m_abForceSpawnCapital.clear();
	m_abRemoveRequiredTerrain.clear();
	m_aiBuildingClassHappiness.clear();
	m_aiBuildingClassGlobalHappiness.clear();
	m_ppaaiBuildingClassYieldChange.clear();
	m_ppaaiFeatureYieldChange.clear();
	m_ppaaiTerrainYieldChange.clear();
	m_ppaaiResourceYieldChange.clear();
	m_ppaaiResourceClassYieldChange.clear();
	m_ppaaiBuildingCostOverride.clear();
#endif
	m_aFreeResourceXCities.clear();
}

/// Reset data members
void CvPlayerTraits::Reset()
{
	Uninit();


	m_vLeaderHasTrait = std::vector<bool>( GC.getNumTraitInfos(), false );
	m_vPotentiallyActiveLeaderTraits.clear();

	m_iGreatPeopleRateModifier = 0;
	m_iGreatScientistRateModifier = 0;
	m_iGreatGeneralRateModifier = 0;
	m_iGreatGeneralExtraBonus = 0;
	m_iGreatPersonGiftInfluence = 0;
	m_iLevelExperienceModifier= 0;
	m_iMaxGlobalBuildingProductionModifier = 0;
	m_iMaxTeamBuildingProductionModifier = 0;
	m_iMaxPlayerBuildingProductionModifier = 0;
	m_iCityUnhappinessModifier = 0;
	m_iPopulationUnhappinessModifier = 0;
	m_iCityStateBonusModifier = 0;
	m_iCityStateFriendshipModifier = 0;
	m_iCityStateCombatModifier = 0;
	m_iLandBarbarianConversionPercent = 0;
	m_iLandBarbarianConversionExtraUnits = 0;
	m_iSeaBarbarianConversionPercent = 0;
	m_iCapitalBuildingModifier = 0;
	m_iPlotBuyCostModifier = 0;
	m_iPlotCultureCostModifier = 0;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	m_iSciencePerGreatPersonBorn = 0;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	m_iNumTurnsBeforeMinorAlliesRefuseBribes = 0;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	m_iGoldenAgeTileBonusFaith = 0;
#endif
	m_iCultureFromKills = 0;
	m_iFaithFromKills = 0;
	m_iCityCultureBonus = 0;
	m_iCapitalThemingBonusModifier = 0;
	m_iPolicyCostModifier = 0;
	m_iCityConnectionTradeRouteChange = 0;
	m_iWonderProductionModifier = 0;
	m_iPlunderModifier = 0;
	m_iImprovementMaintenanceModifier = 0;
	m_iGoldenAgeDurationModifier = 0;
	m_iGoldenAgeMoveChange = 0;
	m_iGoldenAgeCombatModifier = 0;
	m_iGoldenAgeTourismModifier = 0;
	m_iGoldenAgeGreatArtistRateModifier = 0;
	m_iGoldenAgeGreatMusicianRateModifier = 0;
	m_iGoldenAgeGreatWriterRateModifier = 0;
	m_iExtraEmbarkMoves = 0;
	m_iNaturalWonderFirstFinderGold = 0;
	m_iNaturalWonderSubsequentFinderGold = 0;
#if defined(TRAITIFY) // CvPlayerTraits::Reset
	m_bHalfMoreSpecialistUnhappiness = false;

	m_iGoldenAgeCultureModifier = 0;
	m_iNumExtraLeagueVotes = 0;
	m_iNumTradeRouteBonus = 0;
	m_iMinorFriendshipMinimum = 0;
	m_iGreatEngineerRateModifier = 0;
	m_iGreatMerchantRateModifier = 0;
	m_iMinorBullyModifier = 0;
	m_iInternationalRouteGrowthModifier = 0;
	m_iLocalHappinessPerCity = 0;
	m_iInternalTradeRouteYieldModifier = 0;
	m_iUnhappinessModifierForPuppetedCities = 0;
	m_iExtraPopulation = 0;
	m_iFaithCostModifier = 0; 
	m_iIdeologyPressureUnhappinessModifier = 0;
	m_iForeignRelgionPressureModifier = 0;
	m_iFriendlyLandsCitizenMoveChange = 0;
#endif
#if defined(LEKMOD_v34)
	m_bReligionEnhanceReformation = false;
	m_iSelfReligiousPressureModifier = 0;
	m_iLandTradeRouteYieldBonus = 0;
#endif
	//EAP: Natural wonder faith for the finder
	m_iNaturalWonderFirstFinderFaith = 0;
	m_iNaturalWonderSubsequentFinderFaith = 0;

	m_iNaturalWonderYieldModifier = 0;
	m_iNaturalWonderHappinessModifier = 0;
	m_iNearbyImprovementCombatBonus = 0;
	m_iNearbyImprovementBonusRange = 0;
	m_iCultureBuildingYieldChange = 0;
	m_iCombatBonusVsHigherTech = 0;
	m_iCombatBonusVsLargerCiv = 0;
	m_iLandUnitMaintenanceModifier = 0;
	m_iNavalUnitMaintenanceModifier = 0;
	m_iRazeSpeedModifier = 0;
	m_iDOFGreatPersonModifier = 0;
	m_iLuxuryHappinessRetention = 0;
	m_iExtraHappinessPerLuxury = 0; // NQMP GJS - New Netherlands UA
	m_iExtraSpies = 0;
	m_iHappinessPerReligion = 0; // NQMP GJS - New Ottoman UA
	m_iUnresearchedTechBonusFromKills = 0;
	m_iExtraFoundedCityTerritoryClaimRange = 0;
	m_iFreeSocialPoliciesPerEra = 0;
	m_iNumTradeRoutesModifier = 0;
	m_iTradeRouteResourceModifier = 0;
	m_iUniqueLuxuryCities = 0;
	m_iUniqueLuxuryQuantity = 0;
	m_iWorkerSpeedModifier = 0;
	m_iAfraidMinorPerTurnInfluence = 0;
	m_iLandTradeRouteRangeBonus = 0;
	m_iTradeReligionModifier = 0;
	m_iTradeBuildingModifier = 0;

	m_bFightWellDamaged = false;
	m_bMoveFriendlyWoodsAsRoad = false;
	m_bFasterAlongRiver = false;
	m_bFasterInHills = false;
	m_bEmbarkedAllWater = false;
	m_bEmbarkedToLandFlatCost = false;
	m_bNoHillsImprovementMaintenance = false;
	m_bTechBoostFromCapitalScienceBuildings = false;
	m_bStaysAliveZeroCities = false;
	m_bFaithFromUnimprovedForest = false;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	m_bSeaTradeRoutesArePlunderImmune = false;
#endif
	
	// NQMP GJS - New France UA begin
	m_bEarnsGreatPersonOnSlotOrGuild = false;
	m_bHasBuiltWritersGuild = false;
	m_bHasBuiltArtistsGuild = false;
	m_bHasBuiltMusiciansGuild = false;
	// NQMP GJS - New France UA end
	
	m_bBonusReligiousBelief = false;
	m_bAbleToAnnexCityStates = false;
	m_bCrossesMountainsAfterGreatGeneral = false;
	m_bMayaCalendarBonuses = false;
	m_bNoAnnexing = false;
	m_bTechFromCityConquer = false;
	m_bUniqueLuxuryRequiresNewArea = false;
	m_bRiverTradeRoad = false;
	m_bAngerFreeIntrusionOfCityStates = false;

	m_eCampGuardType = NO_UNIT;
	m_eCombatBonusImprovement = NO_IMPROVEMENT;

	m_ppaaiImprovementYieldChange.clear();
	m_ppaaiImprovementYieldChange.resize(GC.getNumImprovementInfos());
	m_ppaaiSpecialistYieldChange.clear();
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	m_ppaaiAnySpecificSpecialistYieldChange.clear();
#endif
	m_ppaaiSpecialistYieldChange.resize(GC.getNumSpecialistInfos());
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	m_ppaaiAnySpecificSpecialistYieldChange.resize(GC.getNumSpecialistInfos());
#endif

	m_ppaaiUnimprovedFeatureYieldChange.clear();
	m_ppaaiUnimprovedFeatureYieldChange.resize(GC.getNumFeatureInfos());
#if defined(TRAITIFY) // CvPlayerTraits::Reset, for CvPlayerTrait arrays
	m_abForceSpawnCapital.clear();
	m_abForceSpawnCapital.resize(GC.getNumUnitClassInfos());
	m_abRemoveRequiredTerrain.clear();
	m_abRemoveRequiredTerrain.resize(GC.getNumBuildingClassInfos());
	m_aiBuildingClassHappiness.clear();
	m_aiBuildingClassHappiness.resize(GC.getNumBuildingClassInfos());
	m_aiBuildingClassGlobalHappiness.clear();
	m_aiBuildingClassGlobalHappiness.resize(GC.getNumBuildingClassInfos());
	m_ppaaiBuildingClassYieldChange.clear();
	m_ppaaiBuildingClassYieldChange.resize(GC.getNumBuildingClassInfos());
	m_ppaaiFeatureYieldChange.clear();
	m_ppaaiFeatureYieldChange.resize(GC.getNumFeatureInfos());
	m_ppaaiTerrainYieldChange.clear();
	m_ppaaiTerrainYieldChange.resize(GC.getNumTerrainInfos());
	m_ppaaiResourceYieldChange.clear();
	m_ppaaiResourceYieldChange.resize(GC.getNumResourceInfos());
	m_ppaaiResourceClassYieldChange.clear();
	m_ppaaiResourceClassYieldChange.resize(GC.getNumResourceClassInfos());
	m_ppaaiBuildingCostOverride.clear();
	m_ppaaiBuildingCostOverride.resize(GC.getNumBuildingInfos());
#endif
	
	Firaxis::Array< int, NUM_YIELD_TYPES > yield;
	for(unsigned int j = 0; j < NUM_YIELD_TYPES; ++j)
	{
		yield[j] = 0;
	}

	for(int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		m_iExtraYieldThreshold[iYield] = 0;
		m_iFreeCityYield[iYield] = 0;
		m_iYieldChangeStrategicResources[iYield] = 0;
		m_iYieldChangeLuxuryResources[iYield] = 0; // NQMP GJS - New Netherlands UA
		m_iYieldChangeNaturalWonder[iYield] = 0;
		m_iYieldChangePerTradePartner[iYield] = 0;
		m_iYieldChangeIncomingTradeRoute[iYield] = 0;
		m_iYieldRateModifier[iYield] = 0;
#if defined(FULL_YIELD_FROM_KILLS) // CvPlayerTraits::Reset, Arrays
		m_iYieldFromKills[iYield] = 0;
#endif
#if defined(TRAITIFY)
		m_iPuppetYieldModifiers[iYield] = 0;
#endif

#ifdef AUI_WARNING_FIXES
		for (uint iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			m_ppaaiImprovementYieldChange[iImprovement] = yield;
		}
		for (uint iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			m_ppaaiSpecialistYieldChange[iSpecialist] = yield;
		}
		for (uint iFeature = 0; iFeature < GC.getNumFeatureInfos(); iFeature++)
#else
		for(int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
		{
			m_ppaaiImprovementYieldChange[iImprovement] = yield;
		}
		for(int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			m_ppaaiSpecialistYieldChange[iSpecialist] = yield;
		}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
		for(int iSpecialist = 0; iSpecialist < 2; iSpecialist++)
		{
			m_ppaaiAnySpecificSpecialistYieldChange[iSpecialist] = yield;
		}
#endif
		for (int iFeature = 0; iFeature < GC.getNumFeatureInfos(); iFeature++)
#endif
		{
			m_ppaaiUnimprovedFeatureYieldChange[iFeature] = yield;
#if !defined(TRAITIFY) // CvPlayerTraits::Reset
		}
#else 
			m_ppaaiFeatureYieldChange[iFeature] = yield;
		}
#endif
#if defined(TRAITIFY) // CvPlayerTraits::Reset, in NUM_YIELD_TYPE loop
		for (int iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
		{
			m_ppaaiTerrainYieldChange[iTerrain] = yield;
		}
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			m_ppaaiResourceYieldChange[iResource] = yield;
		}
		for (int iResourceClass = 0; iResourceClass < GC.getNumResourceClassInfos(); iResourceClass++)
		{
			m_ppaaiResourceClassYieldChange[iResourceClass] = yield;
		}
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
		{
			m_ppaaiBuildingClassYieldChange[iBuildingClass] = yield;
		}
		for (int iBuilding = 0; iBuilding < GC.getNumBuildingInfos(); iBuilding++)
		{
			m_ppaaiBuildingCostOverride[iBuilding] = yield;
		}
#endif
		
	}

#ifdef AUI_WARNING_FIXES
	for (uint iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
#else
	for(int iTerrain = 0; iTerrain < GC.getNumTerrainInfos(); iTerrain++)
#endif
	{
		m_iStrategicResourceQuantityModifier[iTerrain] = 0;
	}
#if defined(TRAITIFY) // BuildingClassLoop for non Yield Arrays
	for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
	{
		m_aiBuildingClassHappiness[iBuildingClass] = 0;
		m_aiBuildingClassGlobalHappiness[iBuildingClass] = 0;
		m_abRemoveRequiredTerrain[iBuildingClass] = false;
	}
	for (int iRouteType = 0; iRouteType < NUM_ROUTE_TYPES; iRouteType++)
	{
		m_iRouteMovementChange[iRouteType] = 0;
	}
#endif
	m_aiResourceQuantityModifier.clear();
	m_aiResourceQuantityModifier.resize(GC.getNumResourceInfos());

#ifdef AUI_WARNING_FIXES
	for (uint iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
#else
	for(int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
#endif
	{
		m_aiResourceQuantityModifier[iResource] = 0;
	}

	m_abNoTrain.clear();
	m_abNoTrain.resize(GC.getNumUnitClassInfos());



#ifdef AUI_WARNING_FIXES
	for (uint iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
#else
	for (int iUnitClass = 0; iUnitClass < GC.getNumUnitClassInfos(); iUnitClass++)
#endif
	{
		m_abNoTrain[iUnitClass] = false;
#if defined(TRAITIFY) // UnitClassForceSpawnCapital insert
		m_abForceSpawnCapital[iUnitClass] = false;
#endif
	}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	m_abNoBuild.clear();
	m_abNoBuild.resize(GC.getNumImprovementInfos());

	for (int iImprovement = 0; iImprovement < GC.getNumImprovementInfos(); iImprovement++)
	{
		m_abNoBuild[iImprovement] = false;
	}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	m_aiBuildTimeOverride.clear();
	m_aiBuildTimeOverrideResourceClassRequired.clear();
	m_BuildTimeOverrides.clear();

	// Initialize vectors for backward compatibility
	m_aiBuildTimeOverride.resize(GC.getNumBuildInfos(), -1);
	m_aiBuildTimeOverrideResourceClassRequired.resize(GC.getNumBuildInfos(), NO_RESOURCECLASS);
#endif

	m_aFreeTraitUnits.clear();

#ifdef AUI_WARNING_FIXES
	uint iNumUnitCombatClassInfos = GC.getNumUnitCombatClassInfos();
	CvAssertMsg((0 < iNumUnitCombatClassInfos), "GC.getNumUnitCombatClassInfos() is not greater than zero but an array is being allocated in CvPlayerTraits::Reset");
	m_paiMovesChangeUnitCombat.clear();
	m_paiMaintenanceModifierUnitCombat.clear();
	m_paiMovesChangeUnitCombat.resize(iNumUnitCombatClassInfos);
	m_paiMaintenanceModifierUnitCombat.resize(iNumUnitCombatClassInfos);
	for (uint iI = 0; iI < iNumUnitCombatClassInfos; iI++)
#else
	int iNumUnitCombatClassInfos = GC.getNumUnitCombatClassInfos();
	CvAssertMsg((0 < iNumUnitCombatClassInfos),  "GC.getNumUnitCombatClassInfos() is not greater than zero but an array is being allocated in CvPlayerTraits::Reset");
	m_paiMovesChangeUnitCombat.clear();
	m_paiMaintenanceModifierUnitCombat.clear();
	m_paiMovesChangeUnitCombat.resize(iNumUnitCombatClassInfos);
	m_paiMaintenanceModifierUnitCombat.resize(iNumUnitCombatClassInfos);
	for(int iI = 0; iI < iNumUnitCombatClassInfos; iI++)
#endif
	{
		m_paiMovesChangeUnitCombat[iI] = 0;
		m_paiMaintenanceModifierUnitCombat[iI] = 0;
	}

#ifdef AUI_WARNING_FIXES
	for (uint iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#else
	int iResourceLoop;
	for(iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#endif
	{
		FreeResourceXCities temp;
		m_aFreeResourceXCities.push_back(temp);
	}
}

/// Does this player possess a specific trait?
bool CvPlayerTraits::HasTrait(TraitTypes eTrait) const
{
	CvAssert(m_pPlayer);

	if(m_pPlayer != NULL)
	{
		CvAssertMsg((m_pPlayer->getLeaderType() >= 0), "getLeaderType() is less than zero");
		CvAssertMsg((eTrait >= 0), "eTrait is less than zero");
		return m_pPlayer->getLeaderInfo().hasTrait(eTrait) && !m_pTraits->GetEntry(eTrait)->IsObsoleteByTech(m_pPlayer->getTeam()) && m_pTraits->GetEntry(eTrait)->IsEnabledByTech(m_pPlayer->getTeam());
	}
	else
	{
		return false;
	}
}

/// Will settling a city in this new area unlock a unique luxury?
bool CvPlayerTraits::WillGetUniqueLuxury(CvArea *pArea) const
{
	// Still have more of these cities to award?
	if (m_iUniqueLuxuryCities > m_iUniqueLuxuryCitiesPlaced)
	{
		// NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area BEGIN
		if (m_pPlayer->GetNumCitiesFounded() == 0)
		{
			return false;
		}
		// NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area END

		int iArea = pArea->GetID();

		// If we have to be in a new area, check to see if this area is okay
		if (m_bUniqueLuxuryRequiresNewArea)
		{
			/* NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area
			// Can't be the capital itself
			if (m_pPlayer->GetNumCitiesFounded() == 0)
			{
				return false;
			}
			*/

			CvPlot *pOriginalCapitalPlot = GC.getMap().plot(m_pPlayer->GetOriginalCapitalX(), m_pPlayer->GetOriginalCapitalY());
			if (pOriginalCapitalPlot)
			{
				if (pOriginalCapitalPlot->getArea() == iArea)
				{
					return false;
				}
			}

			// Already in the list?
			if (std::find (m_aUniqueLuxuryAreas.begin(), m_aUniqueLuxuryAreas.end(), iArea) != m_aUniqueLuxuryAreas.end())
			{
				return false;
			}
		}

		int iNumUniqueResourcesGiven = m_aUniqueLuxuryAreas.size();

		// Loop through all resources and see if we can find one more
		int iNumUniquesFound = 0;
#ifdef AUI_WARNING_FIXES
		for (uint iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#else
		for(int iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#endif
		{
			ResourceTypes eResource = (ResourceTypes) iResourceLoop;
			CvResourceInfo* pkResource = GC.getResourceInfo(eResource);
			if (pkResource != NULL && pkResource->GetRequiredCivilization() == m_pPlayer->getCivilizationType())
			{
				iNumUniquesFound++;
				if (iNumUniquesFound > iNumUniqueResourcesGiven)
				{
					return true;
				}
			}
		}
	}

	return false;
}

/// Bonus movement for this combat class
int CvPlayerTraits::GetMovesChangeUnitCombat(const int unitCombatID) const
{
	CvAssertMsg(unitCombatID < GC.getNumUnitCombatClassInfos(),  "Invalid unitCombatID parameter in call to CvPlayerTraits::GetMovesChangeUnitCombat()");

	if(unitCombatID == NO_UNITCLASS)
	{
		return 0;
	}

	return m_paiMovesChangeUnitCombat[unitCombatID];
}

/// Maintenance modifier for this combat class
int CvPlayerTraits::GetMaintenanceModifierUnitCombat(const int unitCombatID) const
{
	CvAssertMsg(unitCombatID < GC.getNumUnitCombatClassInfos(),  "Invalid unitCombatID parameter in call to CvPlayerTraits::GetMaintenanceModifierUnitCombat()");

	if(unitCombatID == NO_UNITCLASS)
	{
		return 0;
	}

	return m_paiMaintenanceModifierUnitCombat[unitCombatID];
}

/// Extra yield from this improvement
int CvPlayerTraits::GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const
{
	CvAssertMsg(eImprovement < GC.getNumImprovementInfos(),  "Invalid eImprovement parameter in call to CvPlayerTraits::GetImprovementYieldChange()");
	CvAssertMsg(eYield < NUM_YIELD_TYPES,  "Invalid eYield parameter in call to CvPlayerTraits::GetImprovementYieldChange()");

	if(eImprovement == NO_IMPROVEMENT)
	{
		return 0;
	}

	return m_ppaaiImprovementYieldChange[(int)eImprovement][(int)eYield];
}

/// Extra yield from this specialist
int CvPlayerTraits::GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	CvAssertMsg(eSpecialist < GC.getNumSpecialistInfos(),  "Invalid eSpecialist parameter in call to CvPlayerTraits::GetSpecialistYieldChange()");
	CvAssertMsg(eYield < NUM_YIELD_TYPES,  "Invalid eYield parameter in call to CvPlayerTraits::GetSpecialistYieldChange()");

	if(eSpecialist == NO_SPECIALIST)
	{
		return 0;
	}

	return m_ppaaiSpecialistYieldChange[(int)eSpecialist][(int)eYield];
}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
/// Extra yield from specific specialist once
int CvPlayerTraits::GetAnySpecificSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	CvAssertMsg(eSpecialist < GC.getNumSpecialistInfos(),  "Invalid eSpecialist parameter in call to CvPlayerTraits::GetAnySpecificSpecialistYieldChange()");
	CvAssertMsg(eYield < NUM_YIELD_TYPES,  "Invalid eYield parameter in call to CvPlayerTraits::GetAnySpecificSpecialistYieldChange()");

	if(eSpecialist == NO_SPECIALIST)
	{
		return 0;
	}

	return m_ppaaiAnySpecificSpecialistYieldChange[(int)eSpecialist][(int)eYield];
}
#endif
/// Extra yield from a feature without improvement
int CvPlayerTraits::GetUnimprovedFeatureYieldChange(FeatureTypes eFeature, YieldTypes eYield) const
{
	CvAssertMsg(eFeature < GC.getNumFeatureInfos(),  "Invalid eFeature parameter in call to CvPlayerTraits::GetUnimprovedFeatureYieldChange()");
	CvAssertMsg(eYield < NUM_YIELD_TYPES,  "Invalid eYield parameter in call to CvPlayerTraits::GetUnimprovedFeatureYieldChange()");

	if(eFeature == NO_FEATURE)
	{
		return 0;
	}

	return m_ppaaiUnimprovedFeatureYieldChange[(int)eFeature][(int)eYield];
}

/// Do all new units get a specific promotion?
bool CvPlayerTraits::HasFreePromotionUnitCombat(const int promotionID, const int unitCombatID) const
{
	CvAssertMsg((promotionID >= 0), "promotionID is less than zero");
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumTraitInfos(); iI++)
#else
	for(int iI = 0; iI < GC.getNumTraitInfos(); iI++)
#endif
	{
		const TraitTypes eTrait = static_cast<TraitTypes>(iI);
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(eTrait);
		if(pkTraitInfo)
		{
			if(HasTrait(eTrait))
			{
				if(pkTraitInfo->IsFreePromotionUnitCombat(promotionID, unitCombatID))
				{
					return true;
				}
			}
		}
	}

	return false;
}

/// Does each city get a free building?
BuildingTypes CvPlayerTraits::GetFreeBuilding() const
{
	for(size_t iI = 0; iI < m_vPotentiallyActiveLeaderTraits.size(); iI++)
	{
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(m_vPotentiallyActiveLeaderTraits[iI]);
		if(pkTraitInfo && HasTrait(m_vPotentiallyActiveLeaderTraits[iI]))
		{
			if(pkTraitInfo->GetFreeBuilding()!=NO_BUILDING)
			{
				return pkTraitInfo->GetFreeBuilding();
			}
		}
	}


	return NO_BUILDING;
}
/// Does each conquered city get a free building?
BuildingTypes CvPlayerTraits::GetFreeCapitalBuilding() const
{
	for(size_t iI = 0; iI < m_vPotentiallyActiveLeaderTraits.size(); iI++)
	{
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(m_vPotentiallyActiveLeaderTraits[iI]);
		if(pkTraitInfo && HasTrait(m_vPotentiallyActiveLeaderTraits[iI]))
		{
			if(pkTraitInfo->GetFreeCapitalBuilding()!=NO_BUILDING)
			{
				return pkTraitInfo->GetFreeCapitalBuilding();
			}
		}
	}

	return NO_BUILDING;
}


/// Does each conquered city get a free building?
BuildingTypes CvPlayerTraits::GetFreeBuildingOnConquest() const
{
	for(size_t iI = 0; iI < m_vPotentiallyActiveLeaderTraits.size(); iI++)
	{
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(m_vPotentiallyActiveLeaderTraits[iI]);
		if(pkTraitInfo && HasTrait(m_vPotentiallyActiveLeaderTraits[iI]))
		{
			if(pkTraitInfo->GetFreeBuildingOnConquest()!=NO_BUILDING)
			{
				return pkTraitInfo->GetFreeBuildingOnConquest();
			}
		}
	}

	return NO_BUILDING;
}
#if defined(TRAITIFY) // CvPlayerTraits:: ARRAYS
/// Does this trait remove a building's requirement for terrain
bool CvPlayerTraits::IsBuildingClassRemoveRequiredTerrain(BuildingClassTypes eBuildingClass)
{
	if (eBuildingClass != NO_BUILDINGCLASS)
	{
		return m_abRemoveRequiredTerrain[eBuildingClass];
	}
	else
	{
		return false;
	}
}
// Force Spawn UnitClass is Capital
bool CvPlayerTraits::IsUnitClassForceSpawnCapital(UnitClassTypes eUnitClass)
{
	if (eUnitClass != NO_UNITCLASS)
	{
		return m_abForceSpawnCapital[eUnitClass];
	}
	else
	{
		return false;
	}
}
// Building Cost Override (Gold Faith and Production)
int CvPlayerTraits::GetBuildingCostOverride(BuildingTypes eBuilding, YieldTypes eYieldType)
{
	CvAssertMsg(eBuilding < GC.getNumBuildingInfos(), "Invalid eBuilding parameter in call to CvPlayerTraits::GetBuildingCostOverride()");
	CvAssertMsg(eYieldType < NUM_YIELD_TYPES, "Invalid eYieldType parameter in call to CvPlayerTraits::GetBuildingCostOverride()");

	if (eBuilding == NO_BUILDING)
	{
		return 0;
	}

	return m_ppaaiBuildingCostOverride[(int)eBuilding][(int)eYieldType];
}
// Feature Yield Changes
int CvPlayerTraits::GetFeatureYieldChange(FeatureTypes eFeature, YieldTypes eYield) const
{
	CvAssertMsg(eFeature < GC.getNumFeatureInfos(), "Invalid eFeature parameter in call to CvPlayerTraits::GetFeatureYieldChange()");
	CvAssertMsg(eYield < NUM_YIELD_TYPES, "Invalid eYield parameter in call to CvPlayerTraits::GetFeatureYieldChange()");

	if (eFeature == NO_FEATURE)
	{
		return 0;
	}

	return m_ppaaiFeatureYieldChange[(int)eFeature][(int)eYield];
}
// Get the Yield Change from Trait for a Specific Terrain type
int CvPlayerTraits::GetTerrainYieldChange(TerrainTypes eTerrain, YieldTypes eYieldType)
{
	CvAssertMsg(eTerrain < GC.getNumTerrainInfos(), "Invalid eTerrain parameter in call to CvPlayerTraits::GetTerrainYieldChange()");
	CvAssertMsg(eYieldType < NUM_YIELD_TYPES, "Invalid eYield parameter in call to CvPlayerTraits::GetTerrainYieldChange()");

	if (eTerrain == NO_TERRAIN)
	{
		return 0;
	}

	return m_ppaaiTerrainYieldChange[(int)eTerrain][(int)eYieldType];
}
// Get the Yield change from Trait for a Specific Resource type
int CvPlayerTraits::GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYieldType)
{
	CvAssertMsg(eResource < GC.getNumResourceInfos(), "Invalid eResource parameter in call to CvPlayerTraits::GetResourceYieldChange()");
	CvAssertMsg(eYieldType < NUM_YIELD_TYPES, "Invalid eYieldType parameter in call to CvPlayerTraits::GetResourceYieldChange()");

	if (eResource == NO_RESOURCE)
	{
		return 0;
	}

	return m_ppaaiResourceYieldChange[(int)eResource][(int)eYieldType];
}
// Change the Yield of a ResourceClass
int CvPlayerTraits::GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYieldType)
{
	CvAssertMsg(eResourceClass < GC.getNumResourceClassInfos(), "Invalid eResourceClass parameter in call to CvPlayerTraits::GetResourceClassYieldChange()");
	CvAssertMsg(eYieldType < NUM_YIELD_TYPES, "Invalid eYieldType parameter in call to CvPlayerTraits::GetResourceClassYieldChange()");

	if (eResourceClass == NO_RESOURCECLASS)
	{
		return 0;
	}

	return m_ppaaiResourceClassYieldChange[(int)eResourceClass][(int)eYieldType];
}
// Building Class Happiness
int CvPlayerTraits::GetBuildingClassHappiness(BuildingClassTypes eBuildingClass)
{
	CvAssertMsg(eBuildingClass < GC.getNumBuildingClassInfos(), "Invalid eBuildingClass parameter in call to CvPlayerTraits::GetBuildingClassHappiness()");
	if (eBuildingClass == NO_BUILDINGCLASS)
	{
		return 0;
	}

	return m_aiBuildingClassHappiness[(int)eBuildingClass];
}
//Building Class Global Happiness
int CvPlayerTraits::GetBuildingClassGlobalHappiness(BuildingClassTypes eBuildingClass)
{
	CvAssertMsg(eBuildingClass < GC.getNumBuildingClassInfos(), "Invalid eBuildingClass parameter in call to CvPlayerTraits::GetBuildingClassGlobalHappiness()");
	if (eBuildingClass == NO_BUILDINGCLASS)
	{
		return 0;
	}

	return m_aiBuildingClassGlobalHappiness[(int)eBuildingClass];
}
///Get Yield Change from Trait for a specific building class
int CvPlayerTraits::GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYieldType)
{
	CvAssertMsg(eBuildingClass < GC.getNumBuildingClassInfos(), "Invalid eBuildingClass parameter in call to CvPlayerTraits::GetBuildingClassYieldChange()");
	CvAssertMsg(eYieldType < NUM_YIELD_TYPES, "Invalid eYieldType parameter in call to CvPlayerTraits::GetBuildingClassYieldChange()");
	if (eBuildingClass == NO_BUILDINGCLASS)
	{
		return 0;
	}

	return m_ppaaiBuildingClassYieldChange[(int)eBuildingClass][(int)eYieldType];
}
//Yield Per Pop
int CvPlayerTraits::GetYieldPerPopulation(YieldTypes eYieldType)
{
	int rtnValue = 0;
	for (int i = 0; i < GC.getNumTraitInfos(); i++)
	{
		if (HasTrait((TraitTypes)i))
		{
			rtnValue += GC.getTraitInfo((TraitTypes)i)->GetYieldPerPopulation(eYieldType);
		}
	}
	return rtnValue;
}
// Yield Per Population, Foreign Religon
int CvPlayerTraits::GetYieldPerPopulationForeignReligion(YieldTypes eYieldType)
{
	int rtnValue = 0;
	for (int i = 0; i < GC.getNumTraitInfos(); i++)
	{
		if (HasTrait((TraitTypes)i))
		{
			rtnValue += GC.getTraitInfo((TraitTypes)i)->GetYieldPerPopulationForeignReligion(eYieldType);
		}
	}
	return rtnValue;
}
#endif
/// Should unique luxuries appear beneath this tile?
void CvPlayerTraits::AddUniqueLuxuries(CvCity *pCity)
{
	// Still have more of these cities to award?
	if (m_iUniqueLuxuryCities > m_iUniqueLuxuryCitiesPlaced)
	{
		// NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area BEGIN
		if (m_pPlayer->GetNumCitiesFounded() == 1)
		{
			return;
		}
		// NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area END

		// NQMP GJS - Spice Islanders must be coastal BEGIN
		if (!pCity->isCoastal())
		{
			return;
		}
		// NQMP GJS - Spice Islanders must be coastal END

		int iArea = pCity->getArea();

		// If we have to be in a new area, check to see if this area is okay
		if (m_bUniqueLuxuryRequiresNewArea)
		{
			/* NQMP GJS - Spice Islanders changed it so this ability will NEVER trigger in the capital, even if it's not requiring a new area
			// Can't be the capital itself of the area where the capital was founded
			if (m_pPlayer->GetNumCitiesFounded() == 1)
			{
				return;
			}
			*/

			CvPlot *pOriginalCapitalPlot = GC.getMap().plot(m_pPlayer->GetOriginalCapitalX(), m_pPlayer->GetOriginalCapitalY());
			if (pOriginalCapitalPlot)
			{
				if (pOriginalCapitalPlot->getArea() == iArea)
				{
					return;
				}
			}

			// Already in the list?
			if (std::find (m_aUniqueLuxuryAreas.begin(), m_aUniqueLuxuryAreas.end(), iArea) != m_aUniqueLuxuryAreas.end())
			{
				return;
			}
		}

		m_aUniqueLuxuryAreas.push_back(iArea);  		// Store area
		int iNumUniqueResourcesGiven = m_aUniqueLuxuryAreas.size();
		m_iUniqueLuxuryCitiesPlaced++;   // One less to give out

		// Loop through all resources and see if we can find this many unique ones
		ResourceTypes eResourceToGive = NO_RESOURCE;
		int iNumUniquesFound = 0;
#ifdef AUI_WARNING_FIXES
		for (uint iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#else
		for(int iResourceLoop = 0; iResourceLoop < GC.getNumResourceInfos(); iResourceLoop++)
#endif
		{
			ResourceTypes eResource = (ResourceTypes) iResourceLoop;
			CvResourceInfo* pkResource = GC.getResourceInfo(eResource);
			if (pkResource != NULL && pkResource->GetRequiredCivilization() == m_pPlayer->getCivilizationType())
			{
				iNumUniquesFound++;
				if (iNumUniquesFound == iNumUniqueResourcesGiven)
				{
					eResourceToGive = eResource;
					break;
				}
			}
		}

		if (eResourceToGive != NO_RESOURCE)
		{
			pCity->plot()->setResourceType(NO_RESOURCE, 0, true);
			pCity->plot()->setResourceType(eResourceToGive, m_iUniqueLuxuryQuantity, true);
		}
	}
}

/// Does a unit entering this tile cause a barbarian to convert to the player?
bool CvPlayerTraits::CheckForBarbarianConversion(CvPlot* pPlot)
{
	// Loop through all adjacent plots
	CvPlot* pAdjacentPlot;
	int iI;
	bool bRtnValue = false;

	if(pPlot->isWater() && GetSeaBarbarianConversionPercent() > 0)
	{
		for(iI = 0; iI < NUM_DIRECTION_TYPES; ++iI)
		{
			pAdjacentPlot = plotDirection(pPlot->getX(), pPlot->getY(), ((DirectionTypes)iI));

			if(pAdjacentPlot != NULL)
			{
				if(pAdjacentPlot->isWater())
				{
					UnitHandle pNavalUnit = pAdjacentPlot->getBestDefender(BARBARIAN_PLAYER);
					if(pNavalUnit)
					{
						if(ConvertBarbarianNavalUnit(pNavalUnit))
						{
							bRtnValue = true;
						}
					}
				}
			}
		}
	}

	else if(GetLandBarbarianConversionPercent() > 0 && pPlot->getImprovementType() == GC.getBARBARIAN_CAMP_IMPROVEMENT() &&
	        m_eCampGuardType != NO_UNIT)
	{
		bRtnValue = ConvertBarbarianCamp(pPlot);
	}

	return bRtnValue;
}

/// Discounted cost if building already present in capital
int CvPlayerTraits::GetCapitalBuildingDiscount(BuildingTypes eBuilding)
{
	if(m_iCapitalBuildingModifier > 0)
	{
		// Find this player's capital
		CvCity* pCapital = m_pPlayer->getCapitalCity();
		if(pCapital)
		{
			if(pCapital->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
			{
				return m_iCapitalBuildingModifier;
			}
		}
	}
	return 0;
}

/// Table Entries from CMP DLL ~EAP

TechTypes CvPlayerTraits::GetFreeBuildingPrereqTech() const
{
	for(size_t iI = 0; iI < m_vPotentiallyActiveLeaderTraits.size(); iI++)
	{
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(m_vPotentiallyActiveLeaderTraits[iI]);
		if(pkTraitInfo && HasTrait(m_vPotentiallyActiveLeaderTraits[iI]) && pkTraitInfo->GetFreeBuildingPrereqTech())
			return pkTraitInfo->GetFreeBuildingPrereqTech();
	}

	return NO_TECH;
}

TechTypes CvPlayerTraits::GetCapitalFreeBuildingPrereqTech() const
{
	for(size_t iI = 0; iI < m_vPotentiallyActiveLeaderTraits.size(); iI++)
	{
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(m_vPotentiallyActiveLeaderTraits[iI]);
		if(pkTraitInfo && HasTrait(m_vPotentiallyActiveLeaderTraits[iI]) && pkTraitInfo->GetCapitalFreeBuildingPrereqTech())
			return pkTraitInfo->GetCapitalFreeBuildingPrereqTech();
	}

	return NO_TECH;
}


/// First free unit received through traits
int CvPlayerTraits::GetFirstFreeUnit(TechTypes eTech)
{
	m_uiFreeUnitIndex = 0;
	m_eFreeUnitPrereqTech = eTech;

	while(m_uiFreeUnitIndex < m_aFreeTraitUnits.size())
	{
		if(eTech == m_aFreeTraitUnits[m_uiFreeUnitIndex].m_ePrereqTech)
		{
			return m_aFreeTraitUnits[m_uiFreeUnitIndex].m_iFreeUnit;
		}

		m_uiFreeUnitIndex++;
	}

	return NO_UNITCLASS;
}

/// Next free unit received through traits
int CvPlayerTraits::GetNextFreeUnit()
{
	m_uiFreeUnitIndex++;

	while(m_uiFreeUnitIndex < m_aFreeTraitUnits.size())
	{
		if(m_eFreeUnitPrereqTech == m_aFreeTraitUnits[m_uiFreeUnitIndex].m_ePrereqTech)
		{
			return m_aFreeTraitUnits[m_uiFreeUnitIndex].m_iFreeUnit;
		}

		m_uiFreeUnitIndex++;
	}

	return NO_UNITCLASS;
}

/// Does this trait provide free resources in the first X cities?
FreeResourceXCities CvPlayerTraits::GetFreeResourceXCities(ResourceTypes eResource) const
{
	return m_aFreeResourceXCities[(int)eResource];
}

/// Is this civ currently able to cross mountains with combat units?
bool CvPlayerTraits::IsAbleToCrossMountains() const
{
	return (m_bCrossesMountainsAfterGreatGeneral /*&& m_pPlayer->getGreatGeneralsCreated() > 0*/); // NQMP GJS - new Carthage UA no longer needs Great General
}

bool CvPlayerTraits::NoTrain(UnitClassTypes eUnitClassType)
{
	if (eUnitClassType != NO_UNITCLASS)
	{
		return m_abNoTrain[eUnitClassType];
	}
	else
	{
		return false;
	}
}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS

bool CvPlayerTraits::NoBuild(ImprovementTypes eImprovement)
{
	if (eImprovement != NO_IMPROVEMENT)
	{
		return m_abNoBuild[eImprovement];
	}
	else
	{
		return false;
	}
}

#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
int CvPlayerTraits::GetBuildTimeOverride(BuildTypes eBuild, ResourceClassTypes eResourceClass)
{
	CvAssertMsg(eBuild < GC.getNumBuildInfos(), "Invalid eBuild parameter in call to CvPlayerTraits::GetBuildTimeOverride()");

	for (int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		TraitTypes eTrait = static_cast<TraitTypes>(iI);
		if (!HasTrait(eTrait))
			continue;

		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(eTrait);
		if (!pkTraitInfo)
			continue;

		int iOverride = pkTraitInfo->GetBuildTimeOverride(eBuild, eResourceClass);
		if (iOverride > -1) // 0 is valid, below that is not I guess? wouldn't make sense
		{
			return iOverride;
		}
	}

	return -1; // No trait had an override
}
#endif

// MAYA TRAIT SPECIAL METHODS

const float CALENDAR_START = -3112.3973f;  // Actual date is August 11, 3114 BCE.  This float is reversed engineered to come out to Dec 21, 2012 as 13.0.0.0.0
const float DAYS_IN_BAKTUN = 144000.0f;
const float DAYS_IN_KATUN = 7200.0f;
const float DAYS_IN_TUN = 360.0f;
const float DAYS_IN_WINAL = 20.0f;
const float DAYS_IN_YEAR = 365.242199f;

/// Is the Maya calendar active for this player?
bool CvPlayerTraits::IsUsingMayaCalendar() const
{
	int iNumTraits = GC.getNumTraitInfos();
	for(int iI = 0; iI < iNumTraits; iI++)
	{
		const TraitTypes eTrait = static_cast<TraitTypes>(iI);
		CvTraitEntry* pkTraitInfo = GC.getTraitInfo(eTrait);
		if(pkTraitInfo)
		{
			if(pkTraitInfo->IsMayaCalendarBonuses())
			{
				if(HasTrait(eTrait))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}

/// Is this the first turn at the end of a baktun (cycle) of the Maya Long Count calendar?
bool CvPlayerTraits::IsEndOfMayaLongCount()
{
	bool bRtnValue = false;

	if (!IsUsingMayaCalendar())
	{
		return bRtnValue;
	}

	ComputeMayaDate();

	if(m_iBaktunPreviousTurn + 1 == m_iBaktun)
	{
		bRtnValue = true;
	}

	m_iBaktunPreviousTurn = m_iBaktun;

	return bRtnValue;
}

/// Return a string with the Maya calendar date
CvString CvPlayerTraits::GetMayaCalendarString()
{
	CvString szRtnValue;
	ComputeMayaDate();
	szRtnValue.Format("%d.%d.%d.%d.%d", m_iBaktun, m_iKatun, m_iTun, m_iWinal, m_iKin);
	return szRtnValue;
}

/// Return a string with the Maya calendar date
CvString CvPlayerTraits::GetMayaCalendarLongString()
{
	CvString szRtnValue;
	ComputeMayaDate();
	szRtnValue = GetLocalizedText("TXT_KEY_MAYA_CALENDAR_LONG_STRING", m_iBaktun, m_iKatun, m_iTun, m_iWinal, m_iKin);
	return szRtnValue;
}

/// AI routine to pick a Maya bonus for an AI civ
void CvPlayerTraits::ChooseMayaBoost()
{
	UnitTypes eDesiredGreatPerson = NO_UNIT;
	UnitTypes ePossibleGreatPerson;

	// Go for a prophet?
	ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
	if(GetUnitBaktun(ePossibleGreatPerson) == 0)
	{
		CvGameReligions* pReligions = GC.getGame().GetGameReligions();
		ReligionTypes eReligion = pReligions->GetReligionCreatedByPlayer(m_pPlayer->GetID());

		// Have a religion that isn't enhanced yet?
		if(eReligion != NO_RELIGION)
		{
			const CvReligion* pMyReligion = pReligions->GetReligion(eReligion, m_pPlayer->GetID());
			if(!pMyReligion->m_bEnhanced)
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}

		// Don't have a religion and they can still be founded?
		else
		{
			if(pReligions->GetNumReligionsStillToFound() > 0)
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}

	// Highly wonder competitive and still early in game?
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_ENGINEER");
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			if(m_pPlayer->GetDiplomacyAI()->GetWonderCompetitiveness() >= 8 && GC.getGame().getGameTurn() <= (GC.getGame().getEstimateEndTurn() / 2))
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}

	// Does our grand strategy match one that is available?
	AIGrandStrategyTypes eVictoryStrategy = m_pPlayer->GetGrandStrategyAI()->GetActiveGrandStrategy();
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_SCIENTIST");
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			if(eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_SPACESHIP"))
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_ARTIST");
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			if(eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CULTURE"))
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_MERCHANT");
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			if(eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_UNITED_NATIONS"))
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_GREAT_GENERAL");
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			if(eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CONQUEST"))
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
		}
	}

	// No obvious strategic choice, just go for first one available in a reasonable order
	if(eDesiredGreatPerson == NO_UNIT)
	{
		ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
		if(GetUnitBaktun(ePossibleGreatPerson) == 0)
		{
			eDesiredGreatPerson = ePossibleGreatPerson;
		}
		else
		{
			ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_ENGINEER");
			if(GetUnitBaktun(ePossibleGreatPerson) == 0)
			{
				eDesiredGreatPerson = ePossibleGreatPerson;
			}
			else
			{
				ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_WRITER");
				if(GetUnitBaktun(ePossibleGreatPerson) == 0)
				{
					eDesiredGreatPerson = ePossibleGreatPerson;
				}
				else
				{
					ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_MERCHANT");
					if(GetUnitBaktun(ePossibleGreatPerson) == 0)
					{
						eDesiredGreatPerson = ePossibleGreatPerson;
					}
					else
					{
						ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_SCIENTIST");
						if(GetUnitBaktun(ePossibleGreatPerson) == 0)
						{
							eDesiredGreatPerson = ePossibleGreatPerson;
						}
						else
						{
							ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_ARTIST");
							if(GetUnitBaktun(ePossibleGreatPerson) == 0)
							{
								eDesiredGreatPerson = ePossibleGreatPerson;
							}
							else
							{
								ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_MUSICIAN");
								if(GetUnitBaktun(ePossibleGreatPerson) == 0)
								{
									eDesiredGreatPerson = ePossibleGreatPerson;
								}
								else
								{
									ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_GREAT_GENERAL");
									if(GetUnitBaktun(ePossibleGreatPerson) == 0)
									{
										eDesiredGreatPerson = ePossibleGreatPerson;
									}
									else
									{
										ePossibleGreatPerson = (UnitTypes)GC.getInfoTypeForString("UNIT_GREAT_ADMIRAL");
										if(GetUnitBaktun(ePossibleGreatPerson) == 0)
										{
											eDesiredGreatPerson = ePossibleGreatPerson;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// Actually get the great person
	if(eDesiredGreatPerson != NO_UNIT)
	{
		CvCity* pCity = m_pPlayer->GetGreatPersonSpawnCity(eDesiredGreatPerson);
		if(pCity)
		{
			pCity->GetCityCitizens()->DoSpawnGreatPerson(eDesiredGreatPerson, true, false);
			SetUnitBaktun(eDesiredGreatPerson);
		}
		m_pPlayer->ChangeNumMayaBoosts(-1);
	}
}

/// Converts current game year to Maya date information for use by other functions
void CvPlayerTraits::ComputeMayaDate()
{
	float fCalendarStart = CALENDAR_START;
	int iYear = GC.getGame().getGameTurnYear();
	float fYear = (float)iYear;

	if(fYear >= fCalendarStart)
	{
		// Days since calendar start
		float fDays = (fYear - fCalendarStart) * DAYS_IN_YEAR;
		m_iBaktun = (int)(fDays / DAYS_IN_BAKTUN);
		fDays = fDays - ((float)m_iBaktun * DAYS_IN_BAKTUN);
		m_iKatun = (int)(fDays / DAYS_IN_KATUN);
		fDays = fDays - ((float)m_iKatun * DAYS_IN_KATUN);
		m_iTun = (int)(fDays / DAYS_IN_TUN);
		fDays = fDays - ((float)m_iTun * DAYS_IN_TUN);
		m_iWinal = (int)(fDays / DAYS_IN_WINAL);
		fDays = fDays - ((float)m_iWinal * DAYS_IN_WINAL);
		m_iKin = (int)fDays;
	}
}

/// At the end of which calendar cycle was this unit chosen as a special bonus (0 if none)?
int CvPlayerTraits::GetUnitBaktun(UnitTypes eUnit) const
{
	std::vector<MayaBonusChoice>::const_iterator it;

	// Loop through all units available to tactical AI this turn
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_aMayaBonusChoices.begin(); it != m_aMayaBonusChoices.end(); ++it)
#else
	for(it = m_aMayaBonusChoices.begin(); it != m_aMayaBonusChoices.end(); it++)
#endif
	{
		if(it->m_eUnitType == eUnit)
		{
			return it->m_iBaktunJustFinished;
		}
	}

	return 0;
}

/// Set calendar cycle when this unit was chosen as a special bonus
void CvPlayerTraits::SetUnitBaktun(UnitTypes eUnit)
{
	MayaBonusChoice choice;
	choice.m_eUnitType = eUnit;
	choice.m_iBaktunJustFinished = m_iBaktun;
	m_aMayaBonusChoices.push_back(choice);
}

/// Have Maya unlocked free choice of Great People?
bool CvPlayerTraits::IsFreeMayaGreatPersonChoice() const
{
	// True if have already selected each GP type once
	int iNumGreatPeopleTypes = 0;
	SpecialUnitTypes eSpecialUnitGreatPerson = (SpecialUnitTypes) GC.getInfoTypeForString("SPECIALUNIT_PEOPLE");

	// Loop through adding the available units
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
#else
	for(int iI = 0; iI < GC.getNumUnitClassInfos(); iI++)
#endif
	{
		const UnitClassTypes eUnitClass = static_cast<UnitClassTypes>(iI);
		CvUnitClassInfo* pkUnitClassInfo = GC.getUnitClassInfo(eUnitClass);
		if(pkUnitClassInfo)
		{
			const UnitTypes eUnit = (UnitTypes)m_pPlayer->getCivilizationInfo().getCivilizationUnits(eUnitClass);
			if (eUnit != NO_UNIT)
			{
				CvUnitEntry* pUnitEntry = GC.getUnitInfo(eUnit);
				if (pUnitEntry)
				{
					if (pUnitEntry->GetSpecialUnitType() == eSpecialUnitGreatPerson)
					{
						iNumGreatPeopleTypes++;
					}
				}
			}	
		}
	}

	return ((int)m_aMayaBonusChoices.size() >= iNumGreatPeopleTypes);
}

// SERIALIZATION METHODS

/// Serialization read
void CvPlayerTraits::Read(FDataStream& kStream)
{
	int iNumEntries;

	// Version number to maintain backwards compatibility
	uint uiVersion; // 19
	kStream >> uiVersion;

	// precompute the traits our leader has
	m_vPotentiallyActiveLeaderTraits.clear();
	for(int iI = 0; iI < GC.getNumTraitInfos(); iI++)
	{
		m_vLeaderHasTrait[iI] = false;
		if (m_pPlayer && m_pPlayer->isMajorCiv() && m_pPlayer->isAlive() && m_pPlayer->getLeaderInfo().hasTrait( (TraitTypes)iI ))
		{
			m_vLeaderHasTrait[iI] = true;
			m_vPotentiallyActiveLeaderTraits.push_back( (TraitTypes)iI );
		}
	}

	kStream >> m_iGreatPeopleRateModifier;
	kStream >> m_iGreatScientistRateModifier;
	kStream >> m_iGreatGeneralRateModifier;
	kStream >> m_iGreatGeneralExtraBonus;

	kStream >> m_iGreatPersonGiftInfluence;

	kStream >> m_iLevelExperienceModifier;
	kStream >> m_iMaxGlobalBuildingProductionModifier;
	kStream >> m_iMaxTeamBuildingProductionModifier;
	kStream >> m_iMaxPlayerBuildingProductionModifier;
	kStream >> m_iCityUnhappinessModifier;
	kStream >> m_iPopulationUnhappinessModifier;
	kStream >> m_iCityStateBonusModifier;
	kStream >> m_iCityStateFriendshipModifier;
	kStream >> m_iCityStateCombatModifier;
	kStream >> m_iLandBarbarianConversionPercent;
	kStream >> m_iLandBarbarianConversionExtraUnits;
	kStream >> m_iSeaBarbarianConversionPercent;
	kStream >> m_iCapitalBuildingModifier;
	kStream >> m_iPlotBuyCostModifier;
	kStream >> m_iPlotCultureCostModifier;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	kStream >> m_iSciencePerGreatPersonBorn;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	kStream >> m_iNumTurnsBeforeMinorAlliesRefuseBribes;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	kStream >> m_iGoldenAgeTileBonusFaith;
#endif
	kStream >> m_iCultureFromKills;
	if (uiVersion >= 19)
	{
		kStream >> m_iFaithFromKills;
	}
	else
	{
		m_iFaithFromKills = 0;
	}
	kStream >> m_iCityCultureBonus;

	if (uiVersion >= 17)
	{
		kStream >> m_iCapitalThemingBonusModifier;
	}
	else
	{
		m_iCapitalThemingBonusModifier = 0;
	}

	kStream >> m_iPolicyCostModifier;
	kStream >> m_iCityConnectionTradeRouteChange;
	kStream >> m_iWonderProductionModifier;
	kStream >> m_iPlunderModifier;

	kStream >> m_iImprovementMaintenanceModifier;

	kStream >> m_iGoldenAgeDurationModifier;
	kStream >> m_iGoldenAgeMoveChange;
	kStream >> m_iGoldenAgeCombatModifier;

	if (uiVersion >= 2)
	{
		kStream >> m_iGoldenAgeTourismModifier;
		kStream >> m_iGoldenAgeGreatArtistRateModifier;
		kStream >> m_iGoldenAgeGreatMusicianRateModifier;
		kStream >> m_iGoldenAgeGreatWriterRateModifier;
	}
	else
	{
		m_iGoldenAgeTourismModifier = 0;
		m_iGoldenAgeGreatArtistRateModifier = 0;
		m_iGoldenAgeGreatMusicianRateModifier = 0;
		m_iGoldenAgeGreatWriterRateModifier = 0;
	}

	kStream >> m_iExtraEmbarkMoves;

	kStream >> m_iNaturalWonderFirstFinderGold;

	kStream >> m_iNaturalWonderSubsequentFinderGold;
#if defined(TRAITIFY) // CvPlayerTraits::Read
	kStream >> m_bHalfMoreSpecialistUnhappiness;

	kStream >> m_iGoldenAgeCultureModifier;
	kStream >> m_iNumExtraLeagueVotes;
	kStream >> m_iNumTradeRouteBonus;
	kStream >> m_iMinorFriendshipMinimum;
	kStream >> m_iGreatEngineerRateModifier;
	kStream >> m_iGreatMerchantRateModifier;
	kStream >> m_iMinorBullyModifier;
	kStream >> m_iInternationalRouteGrowthModifier;
	kStream >> m_iLocalHappinessPerCity;
	kStream >> m_iInternalTradeRouteYieldModifier;
	kStream >> m_iUnhappinessModifierForPuppetedCities;
	kStream >> m_iExtraPopulation;
	kStream >> m_iFaithCostModifier;
	kStream >> m_iIdeologyPressureUnhappinessModifier;
	kStream >> m_iForeignRelgionPressureModifier;
	kStream >> m_iFriendlyLandsCitizenMoveChange;
#endif
#if defined(LEKMOD_v34)
	kStream >> m_bReligionEnhanceReformation;
	kStream >> m_iSelfReligiousPressureModifier;
	kStream >> m_iLandTradeRouteYieldBonus;
#endif

	//EAP: Natural wonder faith for the finder:

	kStream >> m_iNaturalWonderFirstFinderFaith;

	kStream >> m_iNaturalWonderSubsequentFinderFaith;

	//

	kStream >> m_iNaturalWonderYieldModifier;
	kStream >> m_iNaturalWonderHappinessModifier;

	kStream >> m_iNearbyImprovementCombatBonus;
	kStream >> m_iNearbyImprovementBonusRange;

	kStream >> m_iCultureBuildingYieldChange;

	kStream >> m_iCombatBonusVsHigherTech;

	kStream >> m_iCombatBonusVsLargerCiv;

	kStream >> m_iLandUnitMaintenanceModifier;

	kStream >> m_iNavalUnitMaintenanceModifier;

	kStream >> m_iRazeSpeedModifier;

	kStream >> m_iDOFGreatPersonModifier;

	kStream >> m_iLuxuryHappinessRetention;

	kStream >> m_iExtraHappinessPerLuxury; // NQMP GJS - New Netherlands UA

	kStream >> m_iExtraSpies;

	kStream >> m_iHappinessPerReligion; // NQMP GJS - New Ottoman UA

	kStream >> m_iUnresearchedTechBonusFromKills;

	if (uiVersion >= 4)
	{
		kStream >> m_iExtraFoundedCityTerritoryClaimRange;
	}
	else
	{
		m_iExtraFoundedCityTerritoryClaimRange = 0;
	}

	if (uiVersion >= 5)
	{
		kStream >> m_iFreeSocialPoliciesPerEra;
	}
	else
	{
		m_iFreeSocialPoliciesPerEra = 0;
	}

	if (uiVersion >= 6)
	{
		kStream >> m_iNumTradeRoutesModifier;
	}
	else
	{
		m_iNumTradeRoutesModifier = 0;
	}

	if (uiVersion >= 8)
	{
		kStream >> m_iTradeRouteResourceModifier;
	}
	else
	{
		m_iTradeRouteResourceModifier = 0;
	}

	if (uiVersion >= 9)
	{
		kStream >> m_iUniqueLuxuryCities;
		kStream >> m_iUniqueLuxuryQuantity;
	}
	else
	{
		m_iUniqueLuxuryCities = 0;
		m_iUniqueLuxuryQuantity = 0;
	}

	if (uiVersion >= 11)
	{
		kStream >> m_iUniqueLuxuryCitiesPlaced;
	}
	else
	{
		m_iUniqueLuxuryCitiesPlaced = 0;
	}

	if (uiVersion >= 13)
	{
		kStream >> m_iWorkerSpeedModifier;
	}
	else
	{
		m_iWorkerSpeedModifier = 0;
	}

	if (uiVersion >= 14)
	{
		kStream >> m_iAfraidMinorPerTurnInfluence;
	}
	else
	{
		m_iAfraidMinorPerTurnInfluence = 0;
	}
	
	if (uiVersion >= 15)
	{
		kStream >> m_iLandTradeRouteRangeBonus;
		kStream >> m_iTradeReligionModifier;
	}
	else
	{
		m_iLandTradeRouteRangeBonus = 0;
		m_iTradeReligionModifier = 0;
	}

	if (uiVersion >= 16)
	{
		kStream >> m_iTradeBuildingModifier;
	}
	else
	{
		m_iTradeBuildingModifier = 0;
	}

	kStream >> m_bFightWellDamaged;
	kStream >> m_bMoveFriendlyWoodsAsRoad;
	kStream >> m_bFasterAlongRiver;

	kStream >> m_bFasterInHills;

	kStream >> m_bEmbarkedAllWater;

	kStream >> m_bEmbarkedToLandFlatCost;

	kStream >> m_bNoHillsImprovementMaintenance;

	kStream >> m_bTechBoostFromCapitalScienceBuildings;
	kStream >> m_bStaysAliveZeroCities;

	kStream >> m_bFaithFromUnimprovedForest;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	kStream >> m_bSeaTradeRoutesArePlunderImmune;
#endif

	// NQMP GJS - New France UA begin
	kStream >> m_bEarnsGreatPersonOnSlotOrGuild;
	kStream >> m_bHasBuiltWritersGuild;
	kStream >> m_bHasBuiltArtistsGuild;
	kStream >> m_bHasBuiltMusiciansGuild;
	// NQMP GJS - New France UA end


	kStream >> m_bBonusReligiousBelief;

	kStream >> m_bAbleToAnnexCityStates;

	kStream >> m_bCrossesMountainsAfterGreatGeneral;

	kStream >> m_bMayaCalendarBonuses;
	kStream >> m_iBaktunPreviousTurn;

	kStream >> iNumEntries;
	m_aMayaBonusChoices.clear();
	MayaBonusChoice choice;
	for(int iI = 0; iI < iNumEntries; iI++)
	{
		kStream >> choice.m_eUnitType;
		kStream >> choice.m_iBaktunJustFinished;
		m_aMayaBonusChoices.push_back(choice);
	}

	kStream >> m_bNoAnnexing;
	if (uiVersion >= 3)
	{
		kStream >> m_bTechFromCityConquer;
	}
	else
	{
		m_bTechFromCityConquer = false;
	}

	if (uiVersion >= 9)
	{
		kStream >> m_bUniqueLuxuryRequiresNewArea;
	}
	else
	{
		m_bUniqueLuxuryRequiresNewArea = false;
	}

	if (uiVersion >= 12)
	{
		kStream >> m_bRiverTradeRoad;
	}
	else
	{
		m_bRiverTradeRoad = false;
	}

	if (uiVersion >= 18)
	{
		kStream >> m_bAngerFreeIntrusionOfCityStates;
	}
	else
	{
		m_bAngerFreeIntrusionOfCityStates = false;
	}

	kStream >> m_eCampGuardType;

	kStream >> m_eCombatBonusImprovement;

	ArrayWrapper<int> kExtraYieldThreshold(NUM_YIELD_TYPES, m_iExtraYieldThreshold);
	kStream >> kExtraYieldThreshold;

	ArrayWrapper<int> kFreeCityYield(NUM_YIELD_TYPES, m_iFreeCityYield);
	kStream >> kFreeCityYield;

	ArrayWrapper<int> kYieldChangeResourcesWrapper(NUM_YIELD_TYPES, m_iYieldChangeStrategicResources);
	kStream >> kYieldChangeResourcesWrapper;

	ArrayWrapper<int> kYieldChangeLuxuryResourcesWrapper(NUM_YIELD_TYPES, m_iYieldChangeLuxuryResources); // NQMP GJS - New Netherlands UA
	kStream >> kYieldChangeLuxuryResourcesWrapper; // NQMP GJS - New Netherlands UA

	ArrayWrapper<int> kYieldRateModifierWrapper(NUM_YIELD_TYPES, m_iYieldRateModifier);
	kStream >> kYieldRateModifierWrapper;

	ArrayWrapper<int> kYieldChangeNaturalWonderWrapper(NUM_YIELD_TYPES, m_iYieldChangeNaturalWonder);
	kStream >> kYieldChangeNaturalWonderWrapper;

#if defined(FULL_YIELD_FROM_KILLS) // CvPlayerTraits::Read (for CvPlayerTraits Arrays)
	ArrayWrapper<int> kYieldFromKillsWrapper(NUM_YIELD_TYPES, m_iYieldFromKills);
	kStream >> kYieldFromKillsWrapper;
#endif
#if defined(TRAITIFY) // CvPlayerTraits::Read (for CvPlayerTraits Arrays)
	ArrayWrapper<int> kPuppetYieldModifiersWrapper(NUM_YIELD_TYPES, m_iPuppetYieldModifiers);
	kStream >> kPuppetYieldModifiersWrapper;
	ArrayWrapper<int> kRouteMovementChangeWrapper(NUM_ROUTE_TYPES, m_iRouteMovementChange);
	kStream >> kRouteMovementChangeWrapper;
#endif

	if (uiVersion >= 7)
	{
		ArrayWrapper<int> kYieldChangePerTradePartnerWrapper(NUM_YIELD_TYPES, m_iYieldChangePerTradePartner);
		kStream >> kYieldChangePerTradePartnerWrapper;

		ArrayWrapper<int> kYieldChangeIncomingTradeRouteWrapper(NUM_YIELD_TYPES, m_iYieldChangeIncomingTradeRoute);
		kStream >> kYieldChangeIncomingTradeRouteWrapper;
	}
	else
	{
		for(int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
		{
			m_iYieldChangePerTradePartner[iYield] = 0;
			m_iYieldChangeIncomingTradeRoute[iYield] = 0;
		}
	}

	CvAssert(GC.getNumTerrainInfos() == NUM_TERRAIN_TYPES);	// If this is not true, m_iStrategicResourceQuantityModifier must be resized dynamically
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, &m_iStrategicResourceQuantityModifier[0], GC.getNumTerrainInfos());

	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_aiResourceQuantityModifier);
#if defined(TRAITIFY)
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_aiBuildingClassGlobalHappiness);
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_aiBuildingClassHappiness);

	kStream >> iNumEntries;
	m_abRemoveRequiredTerrain.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_abRemoveRequiredTerrain.push_back(bValue);
	}

	kStream >> iNumEntries;
	m_abForceSpawnCapital.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_abForceSpawnCapital.push_back(bValue);
	}
#endif
	kStream >> iNumEntries;
	m_abNoTrain.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_abNoTrain.push_back(bValue);
	}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	kStream >> iNumEntries;
	m_abNoBuild.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		bool bValue;
		kStream >> bValue;
		m_abNoBuild.push_back(bValue);
	}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	kStream >> iNumEntries;
	m_aiBuildTimeOverride.clear();
	for (int i = 0; i < iNumEntries; i++)
	{
		int iBuildTime;
		kStream >> iBuildTime;
		m_aiBuildTimeOverride.push_back(iBuildTime);
	}
#endif

	kStream >> iNumEntries;
	m_aFreeTraitUnits.clear();
	for(int iI = 0; iI < iNumEntries; iI++)
	{
		FreeTraitUnit trait;
		kStream >> trait.m_iFreeUnit;
		kStream >> trait.m_ePrereqTech;
		m_aFreeTraitUnits.push_back(trait);
	}

	kStream >> iNumEntries;
	for(int iI = 0; iI < iNumEntries; iI++)
	{
		kStream >> m_paiMovesChangeUnitCombat[iI];
	}
	if (uiVersion >= 10)
	{
		for(int iI = 0; iI < iNumEntries; iI++)
		{
			kStream >> m_paiMaintenanceModifierUnitCombat[iI];
		}
	}
	else
	{
		int iNumUnitCombatClassInfos = GC.getNumUnitCombatClassInfos();
		m_paiMaintenanceModifierUnitCombat.resize(iNumUnitCombatClassInfos);
		for(int iI = 0; iI < iNumUnitCombatClassInfos; iI++)
		{
			m_paiMaintenanceModifierUnitCombat[iI] = 0;
		}
	}

	kStream >> m_ppaaiImprovementYieldChange;
	kStream >> m_ppaaiSpecialistYieldChange;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	kStream >> m_ppaaiAnySpecificSpecialistYieldChange;
#endif
	kStream >> m_ppaaiUnimprovedFeatureYieldChange;
#if defined(TRAITIFY) // CvPlayerTraits::Read (for CvPlayerTraits Arrays)
	kStream >> m_ppaaiBuildingClassYieldChange;
	kStream >> m_ppaaiFeatureYieldChange;
	kStream >> m_ppaaiTerrainYieldChange;
	kStream >> m_ppaaiResourceClassYieldChange;
	kStream >> m_ppaaiResourceYieldChange;
	kStream >> m_ppaaiBuildingCostOverride;
#endif

	if (uiVersion >= 11)
	{
		kStream >> iNumEntries;
		m_aUniqueLuxuryAreas.clear();
		for (int iI = 0; iI < iNumEntries; iI++)
		{
			int iAreaID;
			kStream >> iAreaID;
			m_aUniqueLuxuryAreas.push_back(iAreaID);
		}
	}
	else if (uiVersion >= 9)
	{
		kStream >> m_aUniqueLuxuryAreas;
	}
	else
	{
		m_aUniqueLuxuryAreas.clear();
	}
}

/// Serialization write
void CvPlayerTraits::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 19;
	kStream << uiVersion;

	kStream << m_iGreatPeopleRateModifier;
	kStream << m_iGreatScientistRateModifier;
	kStream << m_iGreatGeneralRateModifier;
	kStream << m_iGreatGeneralExtraBonus;
	kStream << m_iGreatPersonGiftInfluence;
	kStream << m_iLevelExperienceModifier;
	kStream << m_iMaxGlobalBuildingProductionModifier;
	kStream << m_iMaxTeamBuildingProductionModifier;
	kStream << m_iMaxPlayerBuildingProductionModifier;
	kStream << m_iCityUnhappinessModifier;
	kStream << m_iPopulationUnhappinessModifier;
	kStream << m_iCityStateBonusModifier;
	kStream << m_iCityStateFriendshipModifier;
	kStream << m_iCityStateCombatModifier;
	kStream << m_iLandBarbarianConversionPercent;
	kStream << m_iLandBarbarianConversionExtraUnits;
	kStream << m_iSeaBarbarianConversionPercent;
	kStream << m_iCapitalBuildingModifier;
	kStream << m_iPlotBuyCostModifier;
	kStream << m_iPlotCultureCostModifier;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	kStream << m_iSciencePerGreatPersonBorn;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	kStream << m_iNumTurnsBeforeMinorAlliesRefuseBribes;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	kStream << m_iGoldenAgeTileBonusFaith;
#endif
	kStream << m_iCultureFromKills;
	kStream << m_iFaithFromKills;
	kStream << m_iCityCultureBonus;
	kStream << m_iCapitalThemingBonusModifier;
	kStream << m_iPolicyCostModifier;
	kStream << m_iCityConnectionTradeRouteChange;
	kStream << m_iWonderProductionModifier;
	kStream << m_iPlunderModifier;
	kStream << m_iImprovementMaintenanceModifier;
	kStream << m_iGoldenAgeDurationModifier;
	kStream << m_iGoldenAgeMoveChange;
	kStream << m_iGoldenAgeCombatModifier;
	kStream << m_iGoldenAgeTourismModifier;
	kStream << m_iGoldenAgeGreatArtistRateModifier;
	kStream << m_iGoldenAgeGreatMusicianRateModifier;
	kStream << m_iGoldenAgeGreatWriterRateModifier;
	kStream << m_iExtraEmbarkMoves;
	kStream << m_iNaturalWonderFirstFinderGold;
	kStream << m_iNaturalWonderSubsequentFinderGold;
#if defined(TRAITIFY) // CvPlayerTraits::Write
	kStream << m_bHalfMoreSpecialistUnhappiness;

	kStream << m_iGoldenAgeCultureModifier;
	kStream << m_iNumExtraLeagueVotes;
	kStream << m_iNumTradeRouteBonus;
	kStream << m_iMinorFriendshipMinimum;
	kStream << m_iGreatEngineerRateModifier;
	kStream << m_iGreatMerchantRateModifier;
	kStream << m_iMinorBullyModifier;
	kStream << m_iInternationalRouteGrowthModifier;
	kStream << m_iLocalHappinessPerCity;
	kStream << m_iInternalTradeRouteYieldModifier;
	kStream << m_iUnhappinessModifierForPuppetedCities;
	kStream << m_iExtraPopulation;
	kStream << m_iFaithCostModifier;
	kStream << m_iIdeologyPressureUnhappinessModifier;
	kStream << m_iForeignRelgionPressureModifier;
	kStream << m_iFriendlyLandsCitizenMoveChange;
#endif
#if defined(LEKMOD_v34)
	kStream << m_bReligionEnhanceReformation;
	kStream << m_iSelfReligiousPressureModifier;
	kStream << m_iLandTradeRouteYieldBonus;
#endif
	//EAP: Natural wonder faith for the finder
	
	kStream << m_iNaturalWonderFirstFinderFaith;
	kStream << m_iNaturalWonderSubsequentFinderFaith;
	

	//
	kStream << m_iNaturalWonderYieldModifier;
	kStream << m_iNaturalWonderHappinessModifier;
	kStream << m_iNearbyImprovementCombatBonus;
	kStream << m_iNearbyImprovementBonusRange;
	kStream << m_iCultureBuildingYieldChange;
	kStream << m_iCombatBonusVsHigherTech;
	kStream << m_iCombatBonusVsLargerCiv;
	kStream << m_iLandUnitMaintenanceModifier;
	kStream << m_iNavalUnitMaintenanceModifier;
	kStream << m_iRazeSpeedModifier;
	kStream << m_iDOFGreatPersonModifier;
	kStream << m_iLuxuryHappinessRetention;
	kStream << m_iExtraHappinessPerLuxury; // NQMP GJS - New Netherlands UA
	kStream << m_iExtraSpies;
	kStream << m_iHappinessPerReligion; // NQMP GJS - New Ottoman UA
	kStream << m_iUnresearchedTechBonusFromKills;
	kStream << m_iExtraFoundedCityTerritoryClaimRange;
	kStream << m_iFreeSocialPoliciesPerEra;
	kStream << m_iNumTradeRoutesModifier;
	kStream << m_iTradeRouteResourceModifier;
	kStream << m_iUniqueLuxuryCities;
	kStream << m_iUniqueLuxuryQuantity;
	kStream << m_iUniqueLuxuryCitiesPlaced;
	kStream << m_iWorkerSpeedModifier;
	kStream << m_iAfraidMinorPerTurnInfluence;
	kStream << m_iLandTradeRouteRangeBonus;
	kStream << m_iTradeReligionModifier;
	kStream << m_iTradeBuildingModifier;

	kStream << m_bFightWellDamaged;
	kStream << m_bMoveFriendlyWoodsAsRoad;
	kStream << m_bFasterAlongRiver;
	kStream << m_bFasterInHills;
	kStream << m_bEmbarkedAllWater;
	kStream << m_bEmbarkedToLandFlatCost;
	kStream << m_bNoHillsImprovementMaintenance;
	kStream << m_bTechBoostFromCapitalScienceBuildings;
	kStream << m_bStaysAliveZeroCities;
	kStream << m_bFaithFromUnimprovedForest;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	kStream << m_bSeaTradeRoutesArePlunderImmune;
#endif

	// NQMP GJS - New France UA begin
	kStream << m_bEarnsGreatPersonOnSlotOrGuild;
	kStream << m_bHasBuiltWritersGuild;
	kStream << m_bHasBuiltArtistsGuild;
	kStream << m_bHasBuiltMusiciansGuild;
	// NQMP GJS - New France UA end

	kStream << m_bBonusReligiousBelief;
	kStream << m_bAbleToAnnexCityStates;
	kStream << m_bCrossesMountainsAfterGreatGeneral;
	kStream << m_bMayaCalendarBonuses;

	kStream << m_iBaktunPreviousTurn;

	std::vector<MayaBonusChoice>::const_iterator it;
	kStream << m_aMayaBonusChoices.size();
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_aMayaBonusChoices.begin(); it != m_aMayaBonusChoices.end(); ++it)
#else
	for(it = m_aMayaBonusChoices.begin(); it != m_aMayaBonusChoices.end(); it++)
#endif
	{
		kStream << it->m_eUnitType;
		kStream << it->m_iBaktunJustFinished;
	}

	kStream << m_bNoAnnexing;
	kStream << m_bTechFromCityConquer;
	kStream << m_bUniqueLuxuryRequiresNewArea;
	kStream << m_bRiverTradeRoad;
	kStream << m_bAngerFreeIntrusionOfCityStates;

	kStream << m_eCampGuardType;
	kStream << m_eCombatBonusImprovement;

	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iExtraYieldThreshold);
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iFreeCityYield);
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldChangeStrategicResources);
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldChangeLuxuryResources); // NQMP GJS - New Netherlands UA
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldRateModifier);
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldChangeNaturalWonder);
#if defined(FULL_YIELD_FROM_KILLS) // CvPlayerTraits::Write (for CvPlayerTraits Arrays)
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldFromKills);
#endif
#if defined(TRAITIFY)
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iPuppetYieldModifiers);
	kStream << ArrayWrapper<int>(NUM_ROUTE_TYPES, m_iRouteMovementChange);
#endif
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldChangePerTradePartner);
	kStream << ArrayWrapper<int>(NUM_YIELD_TYPES, m_iYieldChangeIncomingTradeRoute);


	
	CvInfosSerializationHelper::WriteHashedDataArray<TerrainTypes>(kStream, &m_iStrategicResourceQuantityModifier[0], GC.getNumTerrainInfos());
	CvInfosSerializationHelper::WriteHashedDataArray<ResourceTypes>(kStream, m_aiResourceQuantityModifier);
#if defined(TRAITIFY)
	CvInfosSerializationHelper::WriteHashedDataArray<BuildingClassTypes, int>(kStream, m_aiBuildingClassGlobalHappiness);
	CvInfosSerializationHelper::WriteHashedDataArray<BuildingClassTypes, int>(kStream, m_aiBuildingClassHappiness);

	kStream << m_abRemoveRequiredTerrain.size();
	for (uint ui = 0; ui < m_abRemoveRequiredTerrain.size(); ui++)
	{
		kStream << m_abRemoveRequiredTerrain[ui];
	}

	kStream << m_abForceSpawnCapital.size();
	for (uint ui = 0; ui < m_abForceSpawnCapital.size(); ui++)
	{
		kStream << m_abForceSpawnCapital[ui];
	}
#endif

	kStream << m_abNoTrain.size();
	for (uint ui = 0; ui < m_abNoTrain.size(); ui++)
	{
		kStream << m_abNoTrain[ui];
	}

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	kStream << m_abNoBuild.size();
	for (uint ui = 0; ui < m_abNoBuild.size(); ui++)
	{
		kStream << m_abNoBuild[ui];
	}
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	kStream << m_aiBuildTimeOverride.size();
	for (uint ui = 0; ui < m_aiBuildTimeOverride.size(); ui++)
	{
		kStream << m_aiBuildTimeOverride[ui];
	}
#endif
	kStream << m_aFreeTraitUnits.size();
	for(uint ui = 0; ui < m_aFreeTraitUnits.size(); ui++)
	{
		kStream << m_aFreeTraitUnits[ui].m_iFreeUnit;
		kStream << m_aFreeTraitUnits[ui].m_ePrereqTech;
	}

	int iNumUnitCombatClassInfos = GC.getNumUnitCombatClassInfos();
	kStream << 	iNumUnitCombatClassInfos;
	for(int iI = 0; iI < iNumUnitCombatClassInfos; iI++)
	{
		kStream << m_paiMovesChangeUnitCombat[iI];
	}
	for(int iI = 0; iI < iNumUnitCombatClassInfos; iI++)
	{
		kStream << m_paiMaintenanceModifierUnitCombat[iI];
	}

	kStream << m_ppaaiImprovementYieldChange;
	kStream << m_ppaaiSpecialistYieldChange;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	kStream << m_ppaaiAnySpecificSpecialistYieldChange;
#endif
	kStream << m_ppaaiUnimprovedFeatureYieldChange;
#if defined(TRAITIFY) // CvPlayerTraits::Write (for CvPlayerTraits Arrays)
	kStream << m_ppaaiBuildingClassYieldChange;
	kStream << m_ppaaiFeatureYieldChange;
	kStream << m_ppaaiTerrainYieldChange;
	kStream << m_ppaaiResourceClassYieldChange;
	kStream << m_ppaaiResourceYieldChange;
	kStream << m_ppaaiBuildingCostOverride;
#endif

	kStream << (int)m_aUniqueLuxuryAreas.size();
	for (unsigned int iI = 0; iI < m_aUniqueLuxuryAreas.size(); iI++)
	{
		kStream << m_aUniqueLuxuryAreas[iI];
	}
}

// PRIVATE METHODS

/// Is there an adjacent barbarian camp that could be converted?
bool CvPlayerTraits::ConvertBarbarianCamp(CvPlot* pPlot)
{
	UnitHandle pGiftUnit;

	// Has this camp already decided not to convert?
	if(pPlot->IsBarbarianCampNotConverting())
	{
		return false;
	}

	// Roll die to see if it converts
	if(GC.getGame().getJonRandNum(100, "Barbarian Camp Conversion") < m_iLandBarbarianConversionPercent)
	{
		pPlot->setImprovementType(NO_IMPROVEMENT);

		int iNumGold = /*25*/ GC.getGOLD_FROM_BARBARIAN_CONVERSION();
		m_pPlayer->GetTreasury()->ChangeGold(iNumGold);

		// Set who last cleared the camp here
		pPlot->SetPlayerThatClearedBarbCampHere(m_pPlayer->GetID());

		// Convert the barbarian into our unit
		FAssertMsg(m_eCampGuardType < GC.getNumUnitInfos(), "Illegal camp guard unit type");
		pGiftUnit = m_pPlayer->initUnit(m_eCampGuardType, pPlot->getX(), pPlot->getY(), NO_UNITAI, NO_DIRECTION, true /*bNoMove*/);
		if (!pGiftUnit->jumpToNearestValidPlot())
			pGiftUnit->kill(false);
		else
			pGiftUnit->finishMoves();

		// Convert any extra units
		for(int iI = 0; iI < m_iLandBarbarianConversionExtraUnits; iI++)
		{
			pGiftUnit = m_pPlayer->initUnit(m_eCampGuardType, pPlot->getX(), pPlot->getY(), NO_UNITAI, NO_DIRECTION, true /*bNoMove*/);
			if (!pGiftUnit->jumpToNearestValidPlot())
				pGiftUnit->kill(false);
			else
				pGiftUnit->finishMoves();
		}

		if(GC.getLogging() && GC.getAILogging())
		{
			CvString logMsg;
			logMsg.Format("Converted barbarian camp, X: %d, Y: %d", pPlot->getX(), pPlot->getY());
			m_pPlayer->GetHomelandAI()->LogHomelandMessage(logMsg);
		}

		CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_BARB_CAMP_CONVERTS");
		CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_BARB_CAMP_CONVERTS");
		m_pPlayer->GetNotifications()->Add(NOTIFICATION_GENERIC, strBuffer, strSummary, pPlot->getX(), pPlot->getY(), -1);
		//Increase Stat
		if(m_pPlayer->isHuman() &&!GC.getGame().isGameMultiPlayer())
		{
			gDLL->IncrementSteamStatAndUnlock(ESTEAMSTAT_BARBSCONVERTED, 10, ACHIEVEMENT_SPECIAL_BARBARIANWARLORD);
		}
	}

	// Decided not to
	else
	{
		pPlot->SetBarbarianCampNotConverting(true);
		return false;
	}

	return true;
}

/// Is there an adjacent barbarian naval unit that could be converted?
bool CvPlayerTraits::ConvertBarbarianNavalUnit(UnitHandle pUnit)
{
	UnitHandle pGiftUnit;

	// Has this unit already decided not to convert?
	if(pUnit->IsNotConverting())
	{
		return false;
	}

	// Roll die to see if it converts
	if(GC.getGame().getJonRandNum(100, "Barbarian Naval Unit Conversion") < m_iSeaBarbarianConversionPercent)
	{
		int iNumGold = /*25*/ GC.getGOLD_FROM_BARBARIAN_CONVERSION();
		m_pPlayer->GetTreasury()->ChangeGold(iNumGold);

		// Convert the barbarian into our unit
#ifdef AUI_UNIT_FIX_GIFTED_UNITS_ARE_GIFTED_NOT_CLONED
		pGiftUnit = m_pPlayer->initUnit(pUnit->getUnitType(), pUnit->getX(), pUnit->getY(), pUnit->AI_getUnitAIType(), NO_DIRECTION, true /*bNoMove*/, false, DEFAULT_UNIT_MAP_LAYER, 0, true);
#else
		pGiftUnit = m_pPlayer->initUnit(pUnit->getUnitType(), pUnit->getX(), pUnit->getY(), pUnit->AI_getUnitAIType(), NO_DIRECTION, true /*bNoMove*/, false);
#endif
		CvAssertMsg(pGiftUnit, "GiftUnit is not assigned a valid value");
		pGiftUnit->convert(pUnit.pointer(), false);
		pGiftUnit->setupGraphical();
		pGiftUnit->finishMoves(); // No move first turn

		// Validate that the achievement is reached by a live human and active player at the same time
		if(m_pPlayer->isHuman() && !GC.getGame().isGameMultiPlayer() && m_pPlayer->getLeaderInfo().GetType() && _stricmp(m_pPlayer->getLeaderInfo().GetType(), "LEADER_SULEIMAN") == 0)
		{
			gDLL->IncrementSteamStatAndUnlock(ESTEAMSTAT_BARBSNAVALCONVERTED, 10, ACHIEVEMENT_SPECIAL_BARBARYPIRATE);
		}

		if(GC.getLogging() && GC.getAILogging())
		{
			CvString logMsg;
			logMsg.Format("Converted barbarian naval unit, X: %d, Y: %d", pUnit->getX(), pUnit->getY());
			m_pPlayer->GetHomelandAI()->LogHomelandMessage(logMsg);
		}

		CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_BARB_NAVAL_UNIT_CONVERTS");
		CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_BARB_NAVAL_UNIT_CONVERTS");
		m_pPlayer->GetNotifications()->Add(NOTIFICATION_GENERIC, strBuffer, strSummary, pUnit->getX(), pUnit->getY(), -1);
		return true;
	}

	// Decided not to
	else
	{
		pUnit->SetNotConverting(true);
		return false;
	}
}

