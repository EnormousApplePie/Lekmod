/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
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
#include "CvBarbarians.h"

#include "LintFree.h"

//======================================================================================================
//					CvBeliefEntry
//======================================================================================================
/// Constructor
CvBeliefEntry::CvBeliefEntry() :
	m_iMinPopulation(0),
	m_iMinFollowers(0),
	m_iMaxDistance(0),
	m_iCityGrowthModifier(0),
	m_iFaithFromKills(0),
	m_iFaithFromDyingUnits(0),
	m_iRiverHappiness(0),
	m_iHappinessPerCity(0),
	m_iHappinessPerXPeacefulForeignFollowers(0),
	m_iPlotCultureCostModifier(0),
	m_iCityRangeStrikeModifier(0),
	m_iCombatModifierEnemyCities(0),
	m_iCombatModifierFriendlyCities(0),
	m_iFriendlyHealChange(0),
	m_iCityStateFriendshipModifier(0),
	m_iLandBarbarianConversionPercent(0),
	m_iWonderProductionModifier(0),
	m_iPlayerHappiness(0),
	m_iPlayerCultureModifier(0),
	m_fHappinessPerFollowingCity(0),
	m_iGoldPerFollowingCity(0),
	m_iGoldPerXFollowers(0),
	m_iGoldWhenCityAdopts(0),
	m_iSciencePerOtherReligionFollower(0),
	m_iSpreadDistanceModifier(0),
	m_iSpreadStrengthModifier(0),
	m_iProphetStrengthModifier(0),
	m_iProphetCostModifier(0),
	m_iMissionaryStrengthModifier(0),
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	m_iFlatFaithPerCitizenBorn(0),
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	m_iMissionaryExtraSpreads(0),
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	m_iSpreadModifierOwnedCities(0),
	m_iSpreadModifierUnownedCities(0),
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	m_iExtraTradeRoutes(0),
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	m_iFaithPerCityStateThisReligion(0),
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	m_iFaithPerForeignTradeRoute(0),
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	m_iGoldenAgeTurns(0),
#endif
	m_iMissionaryCostModifier(0),
	m_iFriendlyCityStateSpreadModifier(0),
	m_iGreatPersonExpendedFaith(0),
	m_iCityStateMinimumInfluence(0),
	m_iCityStateInfluenceModifier(0),
	m_iOtherReligionPressureErosion(0),
	m_iSpyPressure(0),
	m_iInquisitorPressureRetention(0),
	m_iFaithBuildingTourism(0),
#ifdef NQ_FREE_SETTLERS_FROM_BELIEF
	m_iNumFreeSettlers(0),
#endif

	m_bPantheon(false),
	m_bFounder(false),
	m_bFollower(false),
	m_bEnhancer(false),
	m_bReformer(false),
	m_bRequiresPeace(false),
	m_bConvertsBarbarians(false),
#ifdef NQ_DEUS_VULT
	m_bDeusVult(false),
#endif
	m_bFaithPurchaseAllGreatPeople(false),
#ifdef NQ_BELIEF_TOGGLE_ALLOW_FAITH_GIFTS_TO_MINORS
	m_bAllowsFaithGiftsToMinors(false),
#endif

	m_eObsoleteEra(NO_ERA),
	m_eResourceRevealed(NO_RESOURCE),
	m_eSpreadModifierDoublingTech(NO_TECH),

	m_paiCityYieldChange(NULL),
	m_paiHolyCityYieldChange(NULL),
	m_paiYieldChangePerForeignCity(NULL),
	m_paiYieldChangePerXForeignFollowers(NULL),
	m_piResourceQuantityModifiers(NULL),
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	m_ppiImprovementYieldChanges(std::pair<int**, size_t>(NULL, 0)),
	m_ppiBuildingClassYieldChanges(std::pair<int**, size_t>(NULL, 0)),
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	m_ppiBuildingClassFlavorChanges(std::pair<int**, size_t>(NULL, 0)),
#endif
#else
	m_ppiImprovementYieldChanges(NULL),
	m_ppiBuildingClassYieldChanges(NULL),
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	m_ppiBuildingClassFlavorChanges(NULL),
#endif
#endif
	m_paiBuildingClassHappiness(NULL),
	m_paiBuildingClassTourism(NULL),
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	m_ppaiFeatureYieldChange(std::pair<int**, size_t>(NULL, 0)),
	m_ppaiResourceYieldChange(std::pair<int**, size_t>(NULL, 0)),
	m_ppaiTerrainYieldChange(std::pair<int**, size_t>(NULL, 0)),
#else
	m_ppaiFeatureYieldChange(NULL),
	m_ppaiResourceYieldChange(NULL),
	m_ppaiTerrainYieldChange(NULL),
#endif
	m_piResourceHappiness(NULL),
	m_piYieldChangeAnySpecialist(NULL),
	m_piYieldChangeTradeRoute(NULL),
	m_piYieldChangeNaturalWonder(NULL),
	m_piYieldChangeWorldWonder(NULL),
	m_piYieldModifierNaturalWonder(NULL),
	m_piMaxYieldModifierPerFollower(NULL),
	m_pbFaithPurchaseUnitEraEnabled(NULL),
	m_pbBuildingClassEnabled(NULL)
{
}

/// Destructor
CvBeliefEntry::~CvBeliefEntry()
{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldChanges.first, m_ppiImprovementYieldChanges.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldChanges.first, m_ppiBuildingClassYieldChanges.second);
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassFlavorChanges.first, m_ppiBuildingClassFlavorChanges.second);
#endif
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiFeatureYieldChange.first, m_ppaiFeatureYieldChange.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiResourceYieldChange.first, m_ppaiResourceYieldChange.second);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiTerrainYieldChange.first, m_ppaiTerrainYieldChange.second);
#else
	CvDatabaseUtility::SafeDelete2DArray(m_ppiImprovementYieldChanges);
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassYieldChanges);
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	CvDatabaseUtility::SafeDelete2DArray(m_ppiBuildingClassFlavorChanges);
#endif
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiFeatureYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiResourceYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_ppaiTerrainYieldChange);
#endif
}

/// Accessor:: Minimum population in this city for belief to be active (0 = no such requirement)
int CvBeliefEntry::GetMinPopulation() const
{
	return m_iMinPopulation;
}

/// Accessor:: Minimum followers in this city for belief to be active (0 = no such requirement)
int CvBeliefEntry::GetMinFollowers() const
{
	return m_iMinFollowers;
}

/// Accessor:: Maximum distance from a city of this religion for belief to be active (0 = no such requirement)
int CvBeliefEntry::GetMaxDistance() const
{
	return m_iMaxDistance;
}

/// Accessor:: Modifier to city growth rate
int CvBeliefEntry::GetCityGrowthModifier() const
{
	return m_iCityGrowthModifier;
}

/// Accessor:: Percentage of enemy strength received in Faith for killing him
int CvBeliefEntry::GetFaithFromKills() const
{
	return m_iFaithFromKills;
}

/// Accessor:: Faith received when a friendly unit dies
int CvBeliefEntry::GetFaithFromDyingUnits() const
{
	return m_iFaithFromDyingUnits;
}

/// Accessor:: Happiness from each city settled on a river
int CvBeliefEntry::GetRiverHappiness() const
{
	return m_iRiverHappiness;
}

/// Accessor:: Happiness per every X population in a city
int CvBeliefEntry::GetHappinessPerCity() const
{
	return m_iHappinessPerCity;
}

/// Accessor:: Happiness per every X population in a foreign city
int CvBeliefEntry::GetHappinessPerXPeacefulForeignFollowers() const
{
	return m_iHappinessPerXPeacefulForeignFollowers;
}

/// Accessor:: Boost in speed of acquiring tiles through culture
int CvBeliefEntry::GetPlotCultureCostModifier() const
{
	return m_iPlotCultureCostModifier;
}

/// Accessor:: Boost in city strike strength
int CvBeliefEntry::GetCityRangeStrikeModifier() const
{
	return m_iCityRangeStrikeModifier;
}

/// Accessor:: Boost in combat near enemy cities of this religion
int CvBeliefEntry::GetCombatModifierEnemyCities() const
{
	return m_iCombatModifierEnemyCities;
}

/// Accessor:: Boost in combat near friendly cities of this religion
int CvBeliefEntry::GetCombatModifierFriendlyCities() const
{
	return m_iCombatModifierFriendlyCities;
}

/// Accessor:: Additional healing in friendly territory
int CvBeliefEntry::GetFriendlyHealChange() const
{
	return m_iFriendlyHealChange;
}

/// Accessor:: Boost in city state influence effectiveness
int CvBeliefEntry::GetCityStateFriendshipModifier() const
{
	return m_iCityStateFriendshipModifier;
}

/// Accessor:: Chance of converting a barbarian camp guard
int CvBeliefEntry::GetLandBarbarianConversionPercent() const
{
	return m_iLandBarbarianConversionPercent;
}

/// Accessor:: boost in production speed for wonders prior to obsolete era
int CvBeliefEntry::GetWonderProductionModifier() const
{
	return m_iWonderProductionModifier;
}

/// Accessor:: boost in production speed for wonders prior to obsolete era
int CvBeliefEntry::GetPlayerHappiness() const
{
	return m_iPlayerHappiness;
}

/// Accessor:: boost in production speed for wonders prior to obsolete era
int CvBeliefEntry::GetPlayerCultureModifier() const
{
	return m_iPlayerCultureModifier;
}

/// Accessor:: amount of extra happiness from each city following this religion
float CvBeliefEntry::GetHappinessPerFollowingCity() const
{
	return m_fHappinessPerFollowingCity;
}

/// Accessor:: amount of extra gold from each city following this religion
int CvBeliefEntry::GetGoldPerFollowingCity() const
{
	return m_iGoldPerFollowingCity;
}

/// Accessor:: amount of extra gold from each city following this religion
int CvBeliefEntry::GetGoldPerXFollowers() const
{
	return m_iGoldPerXFollowers;
}

/// Accessor:: amount of extra gold from each city following this religion
int CvBeliefEntry::GetGoldWhenCityAdopts() const
{
	return m_iGoldWhenCityAdopts;
}

/// Accessor:: amount of science for each follower of another religion in city spread to
int CvBeliefEntry::GetSciencePerOtherReligionFollower() const
{
	return m_iSciencePerOtherReligionFollower;
}

/// Accessor:: extra distance in city-to-city religion spread
int CvBeliefEntry::GetSpreadDistanceModifier() const
{
	return m_iSpreadDistanceModifier;
}

/// Accessor:: extra strength in city-to-city religion spread
int CvBeliefEntry::GetSpreadStrengthModifier() const
{
	return m_iSpreadStrengthModifier;
}

/// Accessor:: prophet conversion strength modifier
int CvBeliefEntry::GetProphetStrengthModifier() const
{
	return m_iProphetStrengthModifier;
}

/// Accessor:: prophet cost discount
int CvBeliefEntry::GetProphetCostModifier() const
{
	return m_iProphetCostModifier;
}

/// Accessor:: missionary conversion strength modifier
int CvBeliefEntry::GetMissionaryStrengthModifier() const
{
	return m_iMissionaryStrengthModifier;
}

#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
/// Accessor:: flat faith gain in the empire when a citizen is born
int CvBeliefEntry::GetFlatFaithPerCitizenBorn() const
{
	return m_iFlatFaithPerCitizenBorn;
}
#endif

#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
/// Accessor:: missionary extra spreads
int CvBeliefEntry::GetMissionaryExtraSpreads() const
{
	return m_iMissionaryExtraSpreads;
}
#endif

#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
/// Accessor:: spread modifier to your cities
int CvBeliefEntry::GetSpreadModifierOwnedCities() const
{
	return m_iSpreadModifierOwnedCities;
}

/// Accessor:: spread modifier to other players' cities
int CvBeliefEntry::GetSpreadModifierUnownedCities() const
{
	return m_iSpreadModifierUnownedCities;
}
#endif

#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
/// Accessor:: extra trade routes
int CvBeliefEntry::GetExtraTradeRoutes() const
{
	return m_iExtraTradeRoutes;
}
#endif

#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
/// Accessor:: faith per turn from city states that are this religion
int CvBeliefEntry::GetFaithPerCityStateThisReligion() const
{
	return m_iFaithPerCityStateThisReligion;
}
#endif

#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
/// Accessor:: extra trade routes
int CvBeliefEntry::GetFaithPerForeignTradeRoute() const
{
	return m_iFaithPerForeignTradeRoute;
}
#endif

#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
/// Accessor:: golden age turns
int CvBeliefEntry::GetGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}
#endif

/// Accessor:: missionary cost discount
int CvBeliefEntry::GetMissionaryCostModifier() const
{
	return m_iMissionaryCostModifier;
}

/// Accessor: speed increase of spread to friendly city states
int CvBeliefEntry::GetFriendlyCityStateSpreadModifier() const
{
	return m_iFriendlyCityStateSpreadModifier;
}

/// Accessor: faith earned for each GP expended
int CvBeliefEntry::GetGreatPersonExpendedFaith() const
{
	return m_iGreatPersonExpendedFaith;
}

/// Accessor: minimum influence with city states of a shared religion
int CvBeliefEntry::GetCityStateMinimumInfluence() const
{
	return m_iCityStateMinimumInfluence;
}

/// Accessor: modifier to influence boosts with city states
int CvBeliefEntry::GetCityStateInfluenceModifier() const
{
	return m_iCityStateInfluenceModifier;
}

/// Accessor: percentage of religious pressure gain that becomes a drop in pressure of other religions
int CvBeliefEntry::GetOtherReligionPressureErosion() const
{
	return m_iOtherReligionPressureErosion;
}

/// Accessor: base religious pressure (before speed multiplier) from having a spy in a city
int CvBeliefEntry::GetSpyPressure() const
{
	return m_iSpyPressure;
}

/// Accessor: percentage of religious pressure retained if one of your cities is hit with an Inquisitor
int CvBeliefEntry::GetInquisitorPressureRetention() const
{
	return m_iInquisitorPressureRetention;
}

/// Accessor: how much tourism can I get from Buildings bought with Faith?
int CvBeliefEntry::GetFaithBuildingTourism() const
{
	return m_iFaithBuildingTourism;
}

#ifdef NQ_FREE_SETTLERS_FROM_BELIEF
/// Accessor: how many free settlers do I get from selecting this belief?
int CvBeliefEntry::GetNumFreeSettlers() const
{
	return m_iNumFreeSettlers;
}
#endif

/// Accessor: is this a belief a pantheon can adopt
bool CvBeliefEntry::IsPantheonBelief() const
{
	return m_bPantheon;
}

/// Accessor: is this a belief a religion founder can adopt
bool CvBeliefEntry::IsFounderBelief() const
{
	return m_bFounder;
}

/// Accessor: is this a belief a religion follower can adopt
bool CvBeliefEntry::IsFollowerBelief() const
{
	return m_bFollower;
}

/// Accessor: is this a belief that enhances the spread of the religion
bool CvBeliefEntry::IsEnhancerBelief() const
{
	return m_bEnhancer;
}

/// Accessor: is this a belief that is added with the Reformation social policy
bool CvBeliefEntry::IsReformationBelief() const
{
	return m_bReformer;
}

/// Accessor: is this a belief that requires you to be at peace to benefit?
bool CvBeliefEntry::RequiresPeace() const
{
	return m_bRequiresPeace;
}

/// Accessor: is this a belief that allows your missionaries to convert adjacent barbarians?
bool CvBeliefEntry::ConvertsBarbarians() const
{
	return m_bConvertsBarbarians;
}

#ifdef NQ_DEUS_VULT
/// Accessor: is this belief deus vult?
bool CvBeliefEntry::DeusVult() const
{
	return m_bDeusVult;
}
#endif

/// Accessor: is this a belief that allows you to purchase any type of Great Person with Faith?
bool CvBeliefEntry::FaithPurchaseAllGreatPeople() const
{
	return m_bFaithPurchaseAllGreatPeople;
}

#ifdef NQ_BELIEF_TOGGLE_ALLOW_FAITH_GIFTS_TO_MINORS
/// Accessor: is this a belief that allows you to gift Faith to city states?
bool CvBeliefEntry::AllowsFaithGiftsToMinors() const
{
	return m_bAllowsFaithGiftsToMinors;
}
#endif

/// Accessor: era when wonder production modifier goes obsolete
EraTypes CvBeliefEntry::GetObsoleteEra() const
{
	return m_eObsoleteEra;
}

/// Accessor:: resource revealed near this city
ResourceTypes CvBeliefEntry::GetResourceRevealed() const
{
	return m_eResourceRevealed;
}

/// Accessor:: technology that doubles the effect of the SpreadStrengthModifier
TechTypes CvBeliefEntry::GetSpreadModifierDoublingTech() const
{
	return m_eSpreadModifierDoublingTech;
}

/// Accessor:: Get brief text description
const char* CvBeliefEntry::getShortDescription() const
{
	return m_strShortDescription;
}

/// Accessor:: Set brief text description
void CvBeliefEntry::setShortDescription(const char* szVal)
{
	m_strShortDescription = szVal;
}

/// Accessor:: Additional yield
int CvBeliefEntry::GetCityYieldChange(int i) const
{
	return m_paiCityYieldChange ? m_paiCityYieldChange[i] : -1;
}

/// Accessor:: Additional player-level yield for controlling holy city
int CvBeliefEntry::GetHolyCityYieldChange(int i) const
{
	return m_paiHolyCityYieldChange ? m_paiHolyCityYieldChange[i] : -1;
}

/// Accessor:: Additional player-level yield for each foreign city converted
int CvBeliefEntry::GetYieldChangePerForeignCity(int i) const
{
	return m_paiYieldChangePerForeignCity ? m_paiYieldChangePerForeignCity[i] : -1;
}

/// Accessor:: Additional player-level yield for followers in foreign cities
int CvBeliefEntry::GetYieldChangePerXForeignFollowers(int i) const
{
	return m_paiYieldChangePerXForeignFollowers ? m_paiYieldChangePerXForeignFollowers[i] : -1;
}

/// Accessor:: Additional quantity of a specific resource
int CvBeliefEntry::GetResourceQuantityModifier(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piResourceQuantityModifiers ? m_piResourceQuantityModifiers[i] : -1;
}

/// Accessor:: Extra yield from an improvement
int CvBeliefEntry::GetImprovementYieldChange(ImprovementTypes eIndex1, YieldTypes eIndex2) const
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

/// Yield change for a specific BuildingClass by yield type
int CvBeliefEntry::GetBuildingClassYieldChange(int i, int j) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiBuildingClassYieldChanges.first ? m_ppiBuildingClassYieldChanges.first[i][j] : 0;
#else
	return m_ppiBuildingClassYieldChanges[i][j];
#endif
}

#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
/// Yield change for a specific BuildingClass by yield type
int CvBeliefEntry::GetBuildingClassFlavorChange(int i, int j) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < GC.getNumFlavorTypes(), "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppiBuildingClassFlavorChanges.first ? m_ppiBuildingClassFlavorChanges.first[i][j] : 0;
#else
	return m_ppiBuildingClassFlavorChanges[i][j];
#endif
}
#endif


/// Amount of extra Happiness per turn a BuildingClass provides
int CvBeliefEntry::GetBuildingClassHappiness(int i) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiBuildingClassHappiness ? m_paiBuildingClassHappiness[i] : -1;
}

/// Amount of extra Tourism per turn a BuildingClass provides
int CvBeliefEntry::GetBuildingClassTourism(int i) const
{
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_paiBuildingClassTourism ? m_paiBuildingClassTourism[i] : -1;
}

/// Change to Feature yield by type
int CvBeliefEntry::GetFeatureYieldChange(int i, int j) const // GJS - make a new one of these? combination
{
	CvAssertMsg(i < GC.getNumFeatureInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppaiFeatureYieldChange.first ? m_ppaiFeatureYieldChange.first[i][j] : -1;
#else
	return m_ppaiFeatureYieldChange ? m_ppaiFeatureYieldChange[i][j] : -1;
#endif
}

/// Change to Resource yield by type
int CvBeliefEntry::GetResourceYieldChange(int i, int j) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppaiResourceYieldChange.first ? m_ppaiResourceYieldChange.first[i][j] : -1;
#else
	return m_ppaiResourceYieldChange ? m_ppaiResourceYieldChange[i][j] : -1;
#endif
}

/// Change to yield by terrain
int CvBeliefEntry::GetTerrainYieldChange(int i, int j) const // GJS - make a new one of these? combination
{
	CvAssertMsg(i < GC.getNumTerrainInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(j < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(j > -1, "Index out of bounds");
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	return m_ppaiTerrainYieldChange.first ? m_ppaiTerrainYieldChange.first[i][j] : -1;
#else
	return m_ppaiTerrainYieldChange ? m_ppaiTerrainYieldChange[i][j] : -1;
#endif
}

/// Happiness from a resource
int CvBeliefEntry::GetResourceHappiness(int i) const
{
	CvAssertMsg(i < GC.getNumResourceInfos(), "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piResourceHappiness ? m_piResourceHappiness[i] : -1;
}

/// Yield boost from having a specialist of any type in city
int CvBeliefEntry::GetYieldChangeAnySpecialist(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChangeAnySpecialist ? m_piYieldChangeAnySpecialist[i] : -1;
}

/// Yield boost from a trade route
int CvBeliefEntry::GetYieldChangeTradeRoute(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChangeTradeRoute ? m_piYieldChangeTradeRoute[i] : -1;
}

/// Yield boost from a natural wonder
int CvBeliefEntry::GetYieldChangeNaturalWonder(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChangeNaturalWonder ? m_piYieldChangeNaturalWonder[i] : -1;
}

/// Yield boost from a world wonder
int CvBeliefEntry::GetYieldChangeWorldWonder(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldChangeWorldWonder ? m_piYieldChangeWorldWonder[i] : -1;
}

/// Yield percentage boost from a natural wonder
int CvBeliefEntry::GetYieldModifierNaturalWonder(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piYieldModifierNaturalWonder ? m_piYieldModifierNaturalWonder[i] : -1;
}

/// Do we get a yield modifier 
int CvBeliefEntry::GetMaxYieldModifierPerFollower(int i) const
{
	CvAssertMsg(i < NUM_YIELD_TYPES, "Index out of bounds");
	CvAssertMsg(i > -1, "Index out of bounds");
	return m_piMaxYieldModifierPerFollower ? m_piMaxYieldModifierPerFollower[i] : -1;
}

/// Can we buy units of this era with faith?
bool CvBeliefEntry::IsFaithUnitPurchaseEra(int i) const
{
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(i < GC.getNumEraInfos(), "Index out of bounds");
	return m_pbFaithPurchaseUnitEraEnabled ? m_pbFaithPurchaseUnitEraEnabled[i] : false;
}

/// Can we buy units of this era with faith?
bool CvBeliefEntry::IsBuildingClassEnabled(int i) const
{
	CvAssertMsg(i > -1, "Index out of bounds");
	CvAssertMsg(i < GC.getNumBuildingClassInfos(), "Index out of bounds");
	return m_pbBuildingClassEnabled ? m_pbBuildingClassEnabled[i] : false;
}

/// Load XML data
bool CvBeliefEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if(!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	//Basic Properties
	setShortDescription(kResults.GetText("ShortDescription"));

	m_iMinPopulation                  = kResults.GetInt("MinPopulation");
	m_iMinFollowers                   = kResults.GetInt("MinFollowers");
	m_iMaxDistance					  = kResults.GetInt("MaxDistance");
	m_iCityGrowthModifier		      = kResults.GetInt("CityGrowthModifier");
	m_iFaithFromKills				  = kResults.GetInt("FaithFromKills");
	m_iFaithFromDyingUnits			  = kResults.GetInt("FaithFromDyingUnits");
	m_iRiverHappiness				  = kResults.GetInt("RiverHappiness");
	m_iHappinessPerCity				  = kResults.GetInt("HappinessPerCity");
	m_iHappinessPerXPeacefulForeignFollowers  = kResults.GetInt("HappinessPerXPeacefulForeignFollowers");
	m_iPlotCultureCostModifier	      = kResults.GetInt("PlotCultureCostModifier");
	m_iCityRangeStrikeModifier	      = kResults.GetInt("CityRangeStrikeModifier");
	m_iCombatModifierEnemyCities      = kResults.GetInt("CombatModifierEnemyCities");
	m_iCombatModifierFriendlyCities   = kResults.GetInt("CombatModifierFriendlyCities");
	m_iFriendlyHealChange	          = kResults.GetInt("FriendlyHealChange");
	m_iCityStateFriendshipModifier    = kResults.GetInt("CityStateFriendshipModifier");
	m_iLandBarbarianConversionPercent = kResults.GetInt("LandBarbarianConversionPercent");
	m_iWonderProductionModifier       = kResults.GetInt("WonderProductionModifier");
	m_iPlayerHappiness			      = kResults.GetInt("PlayerHappiness");
	m_iPlayerCultureModifier          = kResults.GetInt("PlayerCultureModifier");
	m_fHappinessPerFollowingCity      = kResults.GetFloat("HappinessPerFollowingCity");
	m_iGoldPerFollowingCity           = kResults.GetInt("GoldPerFollowingCity");
	m_iGoldPerXFollowers              = kResults.GetInt("GoldPerXFollowers");
	m_iGoldWhenCityAdopts             = kResults.GetInt("GoldPerFirstCityConversion");
	m_iSciencePerOtherReligionFollower= kResults.GetInt("SciencePerOtherReligionFollower");
	m_iSpreadDistanceModifier         = kResults.GetInt("SpreadDistanceModifier");
	m_iSpreadStrengthModifier		  = kResults.GetInt("SpreadStrengthModifier");
	m_iProphetStrengthModifier        = kResults.GetInt("ProphetStrengthModifier");
	m_iProphetCostModifier            = kResults.GetInt("ProphetCostModifier");
	m_iMissionaryStrengthModifier     = kResults.GetInt("MissionaryStrengthModifier");
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	m_iFlatFaithPerCitizenBorn        = kResults.GetInt("FlatFaithPerCitizenBorn");
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	m_iMissionaryExtraSpreads		  = kResults.GetInt("MissionaryExtraSpreads");
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	m_iSpreadModifierOwnedCities	  = kResults.GetInt("SpreadModifierOwnedCities");
	m_iSpreadModifierUnownedCities	  = kResults.GetInt("SpreadModifierUnownedCities");
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	m_iExtraTradeRoutes				  = kResults.GetInt("ExtraTradeRoutes");
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	m_iFaithPerCityStateThisReligion  = kResults.GetInt("FaithPerCityStateThisReligion");
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	m_iFaithPerForeignTradeRoute	  = kResults.GetInt("FaithPerForeignTradeRoute");
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	m_iGoldenAgeTurns				  = kResults.GetInt("GoldenAgeTurns");
#endif
	m_iMissionaryCostModifier         = kResults.GetInt("MissionaryCostModifier");
	m_iFriendlyCityStateSpreadModifier= kResults.GetInt("FriendlyCityStateSpreadModifier");
	m_iGreatPersonExpendedFaith       = kResults.GetInt("GreatPersonExpendedFaith");
	m_iCityStateMinimumInfluence      = kResults.GetInt("CityStateMinimumInfluence");
	m_iCityStateInfluenceModifier     = kResults.GetInt("CityStateInfluenceModifier");
	m_iOtherReligionPressureErosion   = kResults.GetInt("OtherReligionPressureErosion");
	m_iSpyPressure					  = kResults.GetInt("SpyPressure");
	m_iInquisitorPressureRetention    = kResults.GetInt("InquisitorPressureRetention");
	m_iFaithBuildingTourism           = kResults.GetInt("FaithBuildingTourism");
#ifdef NQ_FREE_SETTLERS_FROM_BELIEF
	m_iNumFreeSettlers				  = kResults.GetInt("NumFreeSettlers");
#endif

	m_bPantheon						  = kResults.GetBool("Pantheon");
	m_bFounder						  = kResults.GetBool("Founder");
	m_bFollower						  = kResults.GetBool("Follower");
	m_bEnhancer						  = kResults.GetBool("Enhancer");
	m_bReformer						  = kResults.GetBool("Reformation");
	m_bRequiresPeace				  = kResults.GetBool("RequiresPeace");
	m_bConvertsBarbarians			  = kResults.GetBool("ConvertsBarbarians");
#ifdef NQ_DEUS_VULT
	m_bDeusVult						  = kResults.GetBool("DeusVult");
#endif
	m_bFaithPurchaseAllGreatPeople	  = kResults.GetBool("FaithPurchaseAllGreatPeople");
#ifdef NQ_BELIEF_TOGGLE_ALLOW_FAITH_GIFTS_TO_MINORS
	m_bAllowsFaithGiftsToMinors       = kResults.GetBool("AllowsFaithGiftsToMinors");
#endif

	//References
	const char* szTextVal;
	szTextVal						  = kResults.GetText("ObsoleteEra");
	m_eObsoleteEra					  = (EraTypes)GC.getInfoTypeForString(szTextVal, true);
	szTextVal						  = kResults.GetText("ResourceRevealed");
	m_eResourceRevealed				  = (ResourceTypes)GC.getInfoTypeForString(szTextVal, true);
	szTextVal						  = kResults.GetText("SpreadModifierDoublingTech");
	m_eSpreadModifierDoublingTech     = (TechTypes)GC.getInfoTypeForString(szTextVal, true);

	//Arrays
	const char* szBeliefType = GetType();
	kUtility.SetYields(m_paiCityYieldChange, "Belief_CityYieldChanges", "BeliefType", szBeliefType);
	kUtility.SetYields(m_paiHolyCityYieldChange, "Belief_HolyCityYieldChanges", "BeliefType", szBeliefType);
	kUtility.SetYields(m_piYieldChangeAnySpecialist, "Belief_YieldChangeAnySpecialist", "BeliefType", szBeliefType);
	kUtility.SetYields(m_piYieldChangeTradeRoute, "Belief_YieldChangeTradeRoute", "BeliefType", szBeliefType);
	kUtility.SetYields(m_piYieldChangeNaturalWonder, "Belief_YieldChangeNaturalWonder", "BeliefType", szBeliefType);
	kUtility.SetYields(m_piYieldChangeWorldWonder, "Belief_YieldChangeWorldWonder", "BeliefType", szBeliefType);
	kUtility.SetYields(m_piYieldModifierNaturalWonder, "Belief_YieldModifierNaturalWonder", "BeliefType", szBeliefType);
	kUtility.PopulateArrayByValue(m_piMaxYieldModifierPerFollower, "Yields", "Belief_MaxYieldModifierPerFollower", "YieldType", "BeliefType", szBeliefType, "Max");
	kUtility.PopulateArrayByValue(m_piResourceHappiness, "Resources", "Belief_ResourceHappiness", "ResourceType", "BeliefType", szBeliefType, "HappinessChange");
	kUtility.PopulateArrayByValue(m_piResourceQuantityModifiers, "Resources", "Belief_ResourceQuantityModifiers", "ResourceType", "BeliefType", szBeliefType, "ResourceQuantityModifier");
	kUtility.PopulateArrayByValue(m_paiBuildingClassHappiness, "BuildingClasses", "Belief_BuildingClassHappiness", "BuildingClassType", "BeliefType", szBeliefType, "Happiness");
	kUtility.PopulateArrayByValue(m_paiBuildingClassTourism, "BuildingClasses", "Belief_BuildingClassTourism", "BuildingClassType", "BeliefType", szBeliefType, "Tourism");
	kUtility.PopulateArrayByValue(m_paiYieldChangePerForeignCity, "Yields", "Belief_YieldChangePerForeignCity", "YieldType", "BeliefType", szBeliefType, "Yield");
	kUtility.PopulateArrayByValue(m_paiYieldChangePerXForeignFollowers, "Yields", "Belief_YieldChangePerXForeignFollowers", "YieldType", "BeliefType", szBeliefType, "ForeignFollowers");
	kUtility.PopulateArrayByExistence(m_pbFaithPurchaseUnitEraEnabled, "Eras", "Belief_EraFaithUnitPurchase", "EraType", "BeliefType", szBeliefType);
	kUtility.PopulateArrayByExistence(m_pbBuildingClassEnabled, "BuildingClasses", "Belief_BuildingClassFaithPurchase", "BuildingClassType", "BeliefType", szBeliefType);

	//ImprovementYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiImprovementYieldChanges.first, "Improvements", "Yields");
		m_ppiImprovementYieldChanges.second = kUtility.MaxRows("Improvements");
#else
		kUtility.Initialize2DArray(m_ppiImprovementYieldChanges, "Improvements", "Yields");
#endif

		std::string strKey("Belief_ImprovementYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Improvements.ID as ImprovementID, Yields.ID as YieldID, Yield from Belief_ImprovementYieldChanges inner join Improvements on Improvements.Type = ImprovementType inner join Yields on Yields.Type = YieldType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

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

	//BuildingClassYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldChanges.first, "BuildingClasses", "Yields");
		m_ppiBuildingClassYieldChanges.second = kUtility.MaxRows("BuildingClasses");
#else
		kUtility.Initialize2DArray(m_ppiBuildingClassYieldChanges, "BuildingClasses", "Yields");
#endif

		std::string strKey("Belief_BuildingClassYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, YieldChange from Belief_BuildingClassYieldChanges inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Yields on Yields.Type = YieldType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

		while(pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiBuildingClassYieldChanges.first[BuildingClassID][iYieldID] = iYieldChange;
#else
			m_ppiBuildingClassYieldChanges[BuildingClassID][iYieldID] = iYieldChange;
#endif
		}
	}

#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	//BuildingClassFlavorChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppiBuildingClassFlavorChanges.first, "BuildingClasses", "Flavors");
		m_ppiBuildingClassFlavorChanges.second = kUtility.MaxRows("BuildingClasses");
#else
		kUtility.Initialize2DArray(m_ppiBuildingClassFlavorChanges, "BuildingClasses", "Flavors");
#endif

		std::string strKey("Belief_BuildingClassFlavorChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select BuildingClasses.ID as BuildingClassID, Flavors.ID as FlavorID, FlavorChange from Belief_BuildingClassFlavorChanges inner join BuildingClasses on BuildingClasses.Type = BuildingClassType inner join Flavors on Flavors.Type = FlavorType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

		while (pResults->Step())
		{
			const int BuildingClassID = pResults->GetInt(0);
			const int iFlavorID = pResults->GetInt(1);
			const int iFlavorChange = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppiBuildingClassFlavorChanges.first[BuildingClassID][iFlavorID] = iFlavorChange;
#else
			m_ppiBuildingClassFlavorChanges[BuildingClassID][iFlavorID] = iFlavorChange;
#endif
		}
	}
#endif

	//FeatureYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppaiFeatureYieldChange.first, "Features", "Yields");
		m_ppaiFeatureYieldChange.second = kUtility.MaxRows("Features");
#else
		kUtility.Initialize2DArray(m_ppaiFeatureYieldChange, "Features", "Yields");
#endif

		std::string strKey("Belief_FeatureYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Features.ID as FeatureID, Yields.ID as YieldID, Yield from Belief_FeatureYieldChanges inner join Features on Features.Type = FeatureType inner join Yields on Yields.Type = YieldType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

		while(pResults->Step())
		{
			const int FeatureID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppaiFeatureYieldChange.first[FeatureID][YieldID] = yield;
#else
			m_ppaiFeatureYieldChange[FeatureID][YieldID] = yield;
#endif
		}
	}

	//ResourceYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppaiResourceYieldChange.first, "Resources", "Yields");
		m_ppaiResourceYieldChange.second = kUtility.MaxRows("Resources");
#else
		kUtility.Initialize2DArray(m_ppaiResourceYieldChange, "Resources", "Yields");
#endif

		std::string strKey("Belief_ResourceYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Resources.ID as ResourceID, Yields.ID as YieldID, Yield from Belief_ResourceYieldChanges inner join Resources on Resources.Type = ResourceType inner join Yields on Yields.Type = YieldType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

		while(pResults->Step())
		{
			const int ResourceID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppaiResourceYieldChange.first[ResourceID][YieldID] = yield;
#else
			m_ppaiResourceYieldChange[ResourceID][YieldID] = yield;
#endif
		}
	}

	//TerrainYieldChanges
	{
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
		kUtility.Initialize2DArray(m_ppaiTerrainYieldChange.first, "Terrains", "Yields");
		m_ppaiTerrainYieldChange.second = kUtility.MaxRows("Terrains");
#else
		kUtility.Initialize2DArray(m_ppaiTerrainYieldChange, "Terrains", "Yields");
#endif

		std::string strKey("Belief_TerrainYieldChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if(pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey, "select Terrains.ID as TerrainID, Yields.ID as YieldID, Yield from Belief_TerrainYieldChanges inner join Terrains on Terrains.Type = TerrainType inner join Yields on Yields.Type = YieldType where BeliefType = ?");
		}

		pResults->Bind(1, szBeliefType);

		while(pResults->Step())
		{
			const int TerrainID = pResults->GetInt(0);
			const int YieldID = pResults->GetInt(1);
			const int yield = pResults->GetInt(2);

#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
			m_ppaiTerrainYieldChange.first[TerrainID][YieldID] = yield;
#else
			m_ppaiTerrainYieldChange[TerrainID][YieldID] = yield;
#endif
		}
	}

	return true;
}

//=====================================
// CvBeliefXMLEntries
//=====================================
/// Constructor
CvBeliefXMLEntries::CvBeliefXMLEntries(void)
{

}

/// Destructor
CvBeliefXMLEntries::~CvBeliefXMLEntries(void)
{
	DeleteArray();
}

/// Returns vector of belief entries
std::vector<CvBeliefEntry*>& CvBeliefXMLEntries::GetBeliefEntries()
{
	return m_paBeliefEntries;
}

/// Number of defined beliefs
int CvBeliefXMLEntries::GetNumBeliefs()
{
	return m_paBeliefEntries.size();
}

/// Clear belief entries
void CvBeliefXMLEntries::DeleteArray()
{
	for(std::vector<CvBeliefEntry*>::iterator it = m_paBeliefEntries.begin(); it != m_paBeliefEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paBeliefEntries.clear();
}

/// Get a specific entry
CvBeliefEntry* CvBeliefXMLEntries::GetEntry(int index)
{
	return m_paBeliefEntries[index];
}

//=====================================
// CvReligionBeliefs
//=====================================
/// Constructor
CvReligionBeliefs::CvReligionBeliefs():
	m_paiBuildingClassEnabled(NULL)
{
	Reset();
}

/// Destructor
CvReligionBeliefs::~CvReligionBeliefs(void)
{
	Uninit();
}

/// Copy Constructor with typical parameters
CvReligionBeliefs::CvReligionBeliefs(const CvReligionBeliefs& source)
{
	m_iFaithFromDyingUnits = source.m_iFaithFromDyingUnits;
	m_iRiverHappiness = source.m_iRiverHappiness;
	m_iPlotCultureCostModifier = source.m_iPlotCultureCostModifier;
	m_iCityRangeStrikeModifier = source.m_iCityRangeStrikeModifier;
	m_iCombatModifierEnemyCities = source.m_iCombatModifierEnemyCities;
	m_iCombatModifierFriendlyCities = source.m_iCombatModifierFriendlyCities;
	m_iFriendlyHealChange = source.m_iFriendlyHealChange;
	m_iCityStateFriendshipModifier = source.m_iCityStateFriendshipModifier;
	m_iLandBarbarianConversionPercent = source.m_iLandBarbarianConversionPercent;
	m_iSpreadDistanceModifier = source.m_iSpreadDistanceModifier;
	m_iSpreadStrengthModifier = source.m_iSpreadStrengthModifier;
	m_iProphetStrengthModifier = source.m_iProphetStrengthModifier;
	m_iProphetCostModifier = source.m_iProphetCostModifier;
	m_iMissionaryStrengthModifier = source.m_iMissionaryStrengthModifier;
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	m_iFlatFaithPerCitizenBorn = source.m_iFlatFaithPerCitizenBorn;
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	m_iMissionaryExtraSpreads = source.m_iMissionaryExtraSpreads;
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	m_iSpreadModifierOwnedCities = source.m_iSpreadModifierOwnedCities;
	m_iSpreadModifierUnownedCities = source.m_iSpreadModifierUnownedCities;
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	m_iExtraTradeRoutes	= source.m_iExtraTradeRoutes;
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	m_iFaithPerCityStateThisReligion = source.m_iFaithPerCityStateThisReligion;
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	m_iFaithPerForeignTradeRoute = source.m_iFaithPerForeignTradeRoute;
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	m_iGoldenAgeTurns = source.m_iGoldenAgeTurns;
#endif
	m_iMissionaryCostModifier = source.m_iMissionaryCostModifier;
	m_iFriendlyCityStateSpreadModifier = source.m_iFriendlyCityStateSpreadModifier;
	m_iGreatPersonExpendedFaith = source.m_iGreatPersonExpendedFaith;
	m_iCityStateMinimumInfluence = source.m_iCityStateMinimumInfluence;
	m_iCityStateInfluenceModifier = source.m_iCityStateInfluenceModifier;
	m_iOtherReligionPressureErosion = source.m_iOtherReligionPressureErosion;
	m_iSpyPressure = source.m_iSpyPressure;
	m_iInquisitorPressureRetention = source.m_iInquisitorPressureRetention;
	m_iFaithBuildingTourism = source.m_iFaithBuildingTourism;

	m_eObsoleteEra = source.m_eObsoleteEra;
	m_eResourceRevealed = source.m_eResourceRevealed;
	m_eSpreadModifierDoublingTech = source.m_eSpreadModifierDoublingTech;

	m_ReligionBeliefs = source.m_ReligionBeliefs;

	m_paiBuildingClassEnabled = FNEW(int[GC.getNumBuildingClassInfos()], c_eCiv5GameplayDLL, 0);
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
#else
	for(int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		CvBuildingClassInfo* pkBuildingClassInfo = GC.getBuildingClassInfo((BuildingClassTypes)iI);
		if(!pkBuildingClassInfo)
		{
			continue;
		}
#endif

		m_paiBuildingClassEnabled[iI] = source.m_paiBuildingClassEnabled[iI];
	}
}

/// Deallocate memory created in initialize
void CvReligionBeliefs::Uninit()
{
	SAFE_DELETE_ARRAY(m_paiBuildingClassEnabled);
}

/// Reset data members
void CvReligionBeliefs::Reset()
{
	m_iFaithFromDyingUnits = 0;
	m_iRiverHappiness = 0;
	m_iPlotCultureCostModifier = 0;
	m_iCityRangeStrikeModifier = 0;
	m_iCombatModifierEnemyCities = 0;
	m_iCombatModifierFriendlyCities = 0;
	m_iFriendlyHealChange = 0;
	m_iCityStateFriendshipModifier = 0;
	m_iLandBarbarianConversionPercent = 0;
	m_iSpreadDistanceModifier = 0;
	m_iSpreadStrengthModifier = 0;
	m_iProphetStrengthModifier = 0;
	m_iProphetCostModifier = 0;
	m_iMissionaryStrengthModifier = 0;
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	m_iFlatFaithPerCitizenBorn = 0;
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	m_iMissionaryExtraSpreads = 0;
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	m_iSpreadModifierOwnedCities = 0;
	m_iSpreadModifierUnownedCities = 0;
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	m_iExtraTradeRoutes	= 0;
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	m_iFaithPerCityStateThisReligion = 0;
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	m_iFaithPerForeignTradeRoute = 0;
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	m_iGoldenAgeTurns = 0;
#endif
	m_iMissionaryCostModifier = 0;
	m_iFriendlyCityStateSpreadModifier = 0;
	m_iGreatPersonExpendedFaith = 0;
	m_iCityStateMinimumInfluence = 0;
	m_iCityStateInfluenceModifier = 0;
	m_iOtherReligionPressureErosion = 0;
	m_iSpyPressure = 0;
	m_iInquisitorPressureRetention = 0;
	m_iFaithBuildingTourism = 0;

	m_eObsoleteEra = NO_ERA;
	m_eResourceRevealed = NO_RESOURCE;
	m_eSpreadModifierDoublingTech = NO_TECH;

	m_ReligionBeliefs.clear();

#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_paiBuildingClassEnabled);
#endif
	m_paiBuildingClassEnabled = FNEW(int[GC.getNumBuildingClassInfos()], c_eCiv5GameplayDLL, 0);
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
#else
	for(int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		CvBuildingClassInfo* pkBuildingClassInfo = GC.getBuildingClassInfo((BuildingClassTypes)iI);
		if(!pkBuildingClassInfo)
		{
			continue;
		}
#endif

		m_paiBuildingClassEnabled[iI] = 0;
	}
}

/// Store off data on bonuses from beliefs
void CvReligionBeliefs::AddBelief(BeliefTypes eBelief)
{
	CvAssert(eBelief != NO_BELIEF);
	if(eBelief == NO_BELIEF)
		return;

	CvBeliefEntry* belief = GC.GetGameBeliefs()->GetEntry(eBelief);
	CvAssert(belief != NULL);
	if(belief == NULL)
		return;

	m_iFaithFromDyingUnits += belief->GetFaithFromDyingUnits();
	m_iRiverHappiness += belief->GetRiverHappiness();
	m_iPlotCultureCostModifier += belief->GetPlotCultureCostModifier();
	m_iCityRangeStrikeModifier += belief->GetCityRangeStrikeModifier();
	m_iCombatModifierEnemyCities += belief->GetCombatModifierEnemyCities();
	m_iCombatModifierFriendlyCities += belief->GetCombatModifierFriendlyCities();
	m_iFriendlyHealChange += belief->GetFriendlyHealChange();
	m_iCityStateFriendshipModifier += belief->GetCityStateFriendshipModifier();
	m_iLandBarbarianConversionPercent += belief->GetLandBarbarianConversionPercent();
	m_iSpreadDistanceModifier += belief->GetSpreadDistanceModifier();
	m_iSpreadStrengthModifier += belief->GetSpreadStrengthModifier();
	m_iProphetStrengthModifier += belief->GetProphetStrengthModifier();
	m_iProphetCostModifier += belief->GetProphetCostModifier();
	m_iMissionaryStrengthModifier += belief->GetMissionaryStrengthModifier();
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	m_iFlatFaithPerCitizenBorn += belief->GetFlatFaithPerCitizenBorn();
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	m_iMissionaryExtraSpreads += belief->GetMissionaryExtraSpreads();
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	m_iSpreadModifierOwnedCities += belief->GetSpreadModifierOwnedCities();
	m_iSpreadModifierUnownedCities += belief->GetSpreadModifierUnownedCities();
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	m_iExtraTradeRoutes	+= belief->GetExtraTradeRoutes();
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	m_iFaithPerCityStateThisReligion += belief->GetFaithPerCityStateThisReligion();
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	m_iFaithPerForeignTradeRoute += belief->GetFaithPerForeignTradeRoute();
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	m_iGoldenAgeTurns += belief->GetGoldenAgeTurns();
#endif
	m_iMissionaryCostModifier += belief->GetMissionaryCostModifier();
	m_iFriendlyCityStateSpreadModifier += belief->GetFriendlyCityStateSpreadModifier();
	m_iGreatPersonExpendedFaith += belief->GetGreatPersonExpendedFaith();
	m_iCityStateMinimumInfluence += belief->GetCityStateMinimumInfluence();
	m_iCityStateInfluenceModifier += belief->GetCityStateInfluenceModifier();
	m_iOtherReligionPressureErosion += belief->GetOtherReligionPressureErosion();
	m_iSpyPressure += belief->GetSpyPressure();
	m_iInquisitorPressureRetention += belief->GetInquisitorPressureRetention();
	m_iFaithBuildingTourism += belief->GetFaithBuildingTourism();

	m_eObsoleteEra = belief->GetObsoleteEra();
	m_eResourceRevealed = belief->GetResourceRevealed();

#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
	{
		if (belief->IsBuildingClassEnabled(iI))
		{
			m_paiBuildingClassEnabled[iI]++;
		}
	}

	if(belief->GetSpreadModifierDoublingTech() != NO_TECH)
	{
		m_eSpreadModifierDoublingTech = belief->GetSpreadModifierDoublingTech();
	}

	m_ReligionBeliefs.push_back((int)eBelief);
}

/// Does this religion possess a specific belief?
bool CvReligionBeliefs::HasBelief(BeliefTypes eBelief) const
{
	return (find(m_ReligionBeliefs.begin(), m_ReligionBeliefs.end(), (int)eBelief) != m_ReligionBeliefs.end());
}

/// Does this religion possess a specific belief?
BeliefTypes CvReligionBeliefs::GetBelief(int iIndex) const
{
	return (BeliefTypes)m_ReligionBeliefs[iIndex];
}

/// Does this religion possess a specific belief?
int CvReligionBeliefs::GetNumBeliefs() const
{
	return m_ReligionBeliefs.size();
}

/// Faith from kills
int CvReligionBeliefs::GetFaithFromKills(int iDistance) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;
	int iRequiredDistance;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			iRequiredDistance = pBeliefs->GetEntry(i)->GetMaxDistance();
			if(iRequiredDistance == 0 || iDistance <= iRequiredDistance)
			{
				rtnValue += pBeliefs->GetEntry(i)->GetFaithFromKills();
			}
		}
	}

	return rtnValue;
}

/// Happiness per city
int CvReligionBeliefs::GetHappinessPerCity(int iPopulation) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(iPopulation >= pBeliefs->GetEntry(i)->GetMinPopulation())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetHappinessPerCity();
			}
		}
	}

	return rtnValue;
}

/// Happiness per X followers in foreign cities of powers you are not at war with
int CvReligionBeliefs::GetHappinessPerXPeacefulForeignFollowers() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i) && pBeliefs->GetEntry(i)->GetHappinessPerXPeacefulForeignFollowers() > 0)
		{
			return pBeliefs->GetEntry(i)->GetHappinessPerXPeacefulForeignFollowers();
		}
	}

	return 0;
}

/// Wonder production boost
int CvReligionBeliefs:: GetWonderProductionModifier(EraTypes eWonderEra) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if((int)eWonderEra < (int)pBeliefs->GetEntry(i)->GetObsoleteEra())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetWonderProductionModifier();
			}
		}
	}

	return rtnValue;
}

/// Player happiness boost
int CvReligionBeliefs:: GetPlayerHappiness(bool bAtPeace) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(bAtPeace || !pBeliefs->GetEntry(i)->RequiresPeace())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetPlayerHappiness();
			}
		}
	}

	return rtnValue;
}

/// Player culture modifier
int CvReligionBeliefs:: GetPlayerCultureModifier(bool bAtPeace) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(bAtPeace || !pBeliefs->GetEntry(i)->RequiresPeace())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetPlayerCultureModifier();
			}
		}
	}

	return rtnValue;
}

/// Happiness per following city
float CvReligionBeliefs:: GetHappinessPerFollowingCity() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	float rtnValue = 0.0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetHappinessPerFollowingCity();
		}
	}

	return rtnValue;
}

/// Gold per following city
int CvReligionBeliefs:: GetGoldPerFollowingCity() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetGoldPerFollowingCity();
		}
	}

	return rtnValue;
}

/// Gold per following city
int CvReligionBeliefs:: GetGoldPerXFollowers() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetGoldPerXFollowers();
		}
	}

	return rtnValue;
}

/// Gold per following city
int CvReligionBeliefs:: GetGoldWhenCityAdopts() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetGoldWhenCityAdopts();
		}
	}

	return rtnValue;
}

/// Science per other religion follower
int CvReligionBeliefs:: GetSciencePerOtherReligionFollower() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetSciencePerOtherReligionFollower();
		}
	}

	return rtnValue;
}

/// City growth modifier
int CvReligionBeliefs::GetCityGrowthModifier(bool bAtPeace) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(bAtPeace || !pBeliefs->GetEntry(i)->RequiresPeace())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetCityGrowthModifier();
			}
		}
	}

	return rtnValue;
}

/// Extra yield
int CvReligionBeliefs::GetCityYieldChange(int iPopulation, YieldTypes eYield) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(iPopulation >= pBeliefs->GetEntry(i)->GetMinPopulation())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetCityYieldChange(eYield);
			}
		}
	}

	return rtnValue;
}

/// Extra holy city yield
int CvReligionBeliefs::GetHolyCityYieldChange (YieldTypes eYield) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
				rtnValue += pBeliefs->GetEntry(i)->GetHolyCityYieldChange(eYield);
		}
	}

	return rtnValue;
}

/// Extra yield for foreign cities following religion
int CvReligionBeliefs::GetYieldChangePerForeignCity(YieldTypes eYield) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangePerForeignCity(eYield);
		}
	}

	return rtnValue;
}

/// Extra yield for foreign followers
int CvReligionBeliefs::GetYieldChangePerXForeignFollowers(YieldTypes eYield) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangePerXForeignFollowers(eYield);
		}
	}

	return rtnValue;
}

/// Extra yield from this improvement
int CvReligionBeliefs::GetResourceQuantityModifier(ResourceTypes eResource) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetResourceQuantityModifier(eResource);
		}
	}

	return rtnValue;
}

/// Extra yield from this improvement
int CvReligionBeliefs::GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetImprovementYieldChange(eImprovement, eYield);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a specific building class
int CvReligionBeliefs::GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYieldType, int iFollowers) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(iFollowers >= pBeliefs->GetEntry(i)->GetMinFollowers())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetBuildingClassYieldChange(eBuildingClass, eYieldType);
			}
		}
	}

	return rtnValue;
}

#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
int CvReligionBeliefs::GetBuildingClassFlavorChange(BuildingClassTypes eBuildingClass, FlavorTypes eFlavorType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for (int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if (HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetBuildingClassFlavorChange(eBuildingClass, eFlavorType);
		}
	}

	return rtnValue;
}
#endif

/// Get Happiness from beliefs for a specific building class
int CvReligionBeliefs::GetBuildingClassHappiness(BuildingClassTypes eBuildingClass, int iFollowers) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if(iFollowers >= pBeliefs->GetEntry(i)->GetMinFollowers())
			{
				rtnValue += pBeliefs->GetEntry(i)->GetBuildingClassHappiness(eBuildingClass);
			}
		}
	}

	return rtnValue;
}

/// Get Tourism from beliefs for a specific building class
int CvReligionBeliefs::GetBuildingClassTourism(BuildingClassTypes eBuildingClass) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetBuildingClassTourism(eBuildingClass);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a specific feature
int CvReligionBeliefs::GetFeatureYieldChange(FeatureTypes eFeature, YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetFeatureYieldChange(eFeature, eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a specific resource
int CvReligionBeliefs::GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetResourceYieldChange(eResource, eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a specific terrain
int CvReligionBeliefs::GetTerrainYieldChange(TerrainTypes eTerrain, YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetTerrainYieldChange(eTerrain, eYieldType);
		}
	}

	return rtnValue;
}

// Get happiness boost from a resource
int CvReligionBeliefs::GetResourceHappiness(ResourceTypes eResource) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetResourceHappiness(eResource);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a specialist being present in city
int CvReligionBeliefs::GetYieldChangeAnySpecialist(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangeAnySpecialist(eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a trade route
int CvReligionBeliefs::GetYieldChangeTradeRoute(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangeTradeRoute(eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a natural wonder
int CvReligionBeliefs::GetYieldChangeNaturalWonder(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangeNaturalWonder(eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield change from beliefs for a world wonder
int CvReligionBeliefs::GetYieldChangeWorldWonder(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldChangeWorldWonder(eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield modifier from beliefs for a natural wonder
int CvReligionBeliefs::GetYieldModifierNaturalWonder(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetYieldModifierNaturalWonder(eYieldType);
		}
	}

	return rtnValue;
}

/// Get yield modifier from beliefs for a natural wonder
int CvReligionBeliefs::GetMaxYieldModifierPerFollower(YieldTypes eYieldType) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetMaxYieldModifierPerFollower(eYieldType);
		}
	}

	return rtnValue;
}

#ifdef NQ_FREE_SETTLERS_FROM_BELIEF
/// Get number of free settlers from beliefs
int CvReligionBeliefs::GetNumFreeSettlers() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();
	int rtnValue = 0;

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			rtnValue += pBeliefs->GetEntry(i)->GetNumFreeSettlers();
		}
	}

	return rtnValue;
}
#endif

/// Does this belief allow a building to be constructed?
bool CvReligionBeliefs::IsBuildingClassEnabled(BuildingClassTypes eType) const
{
	return m_paiBuildingClassEnabled[(int)eType];
}

/// Is there a belief that allows faith buying of units
bool CvReligionBeliefs::IsFaithBuyingEnabled(EraTypes eEra) const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if (pBeliefs->GetEntry(i)->IsFaithUnitPurchaseEra((int)eEra))
			{
				return true;
			}
		}
	}

	return false;
}

/// Is there a belief that allows us to convert adjacent barbarians?
bool CvReligionBeliefs::IsConvertsBarbarians() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if (pBeliefs->GetEntry(i)->ConvertsBarbarians())
			{
				return true;
			}
		}
	}

	return false;
}

#ifdef NQ_DEUS_VULT
/// Is there a belief that matches Deus Vult?
bool CvReligionBeliefs::IsDeusVult() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if (pBeliefs->GetEntry(i)->DeusVult())
			{
				return true;
			}
		}
	}

	return false;
}
#endif

/// Is there a belief that allows faith buying of all great people
bool CvReligionBeliefs::IsFaithPurchaseAllGreatPeople() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if (pBeliefs->GetEntry(i)->FaithPurchaseAllGreatPeople())
			{
				return true;
			}
		}
	}

	return false;
}

#ifdef NQ_BELIEF_TOGGLE_ALLOW_FAITH_GIFTS_TO_MINORS
/// Is there a belief that allows faith gifts to city states?
bool CvReligionBeliefs::IsAllowsFaithGiftsToMinors() const
{
	CvBeliefXMLEntries* pBeliefs = GC.GetGameBeliefs();

	for(int i = 0; i < pBeliefs->GetNumBeliefs(); i++)
	{
		if(HasBelief((BeliefTypes)i))
		{
			if (pBeliefs->GetEntry(i)->AllowsFaithGiftsToMinors())
			{
				return true;
			}
		}
	}

	return false;
}
#endif

/// Serialization read
void CvReligionBeliefs::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;

	kStream >> m_iFaithFromDyingUnits;
	kStream >> m_iRiverHappiness;
	kStream >> m_iPlotCultureCostModifier;
	kStream >> m_iCityRangeStrikeModifier;
	kStream >> m_iCombatModifierEnemyCities;
	kStream >> m_iCombatModifierFriendlyCities;
	kStream >> m_iFriendlyHealChange;
	kStream >> m_iCityStateFriendshipModifier;
	kStream >> m_iLandBarbarianConversionPercent;
	kStream >> m_iSpreadStrengthModifier;
	kStream >> m_iSpreadDistanceModifier;
	kStream >> m_iProphetStrengthModifier;
	kStream >> m_iProphetCostModifier;
	kStream >> m_iMissionaryStrengthModifier;
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	kStream >> m_iFlatFaithPerCitizenBorn;
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	kStream >> m_iMissionaryExtraSpreads;
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	kStream >> m_iSpreadModifierOwnedCities;
	kStream >> m_iSpreadModifierUnownedCities;
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	kStream >> m_iExtraTradeRoutes;
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	kStream >> m_iFaithPerCityStateThisReligion;
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	kStream >> m_iFaithPerForeignTradeRoute;
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	kStream >> m_iGoldenAgeTurns;
#endif
	kStream >> m_iMissionaryCostModifier;
	kStream >> m_iFriendlyCityStateSpreadModifier;
	kStream >> m_iGreatPersonExpendedFaith;
	kStream >> m_iCityStateMinimumInfluence;
	kStream >> m_iCityStateInfluenceModifier;
	kStream >> m_iOtherReligionPressureErosion;
	kStream >> m_iSpyPressure;
	kStream >> m_iInquisitorPressureRetention;
	if (uiVersion >= 2)
	{
		kStream >> m_iFaithBuildingTourism;
	}
	else
	{
		m_iFaithBuildingTourism = 0;
	}

	kStream >> m_eObsoleteEra;
	kStream >> m_eResourceRevealed;
	kStream >> m_eSpreadModifierDoublingTech;

	m_ReligionBeliefs.clear();
	uint uiBeliefCount;
	kStream >> uiBeliefCount;
	while(uiBeliefCount--)
	{
		int iBeliefIndex = CvInfosSerializationHelper::ReadHashed(kStream);
		m_ReligionBeliefs.push_back(iBeliefIndex);
	}

	BuildingClassArrayHelpers::Read(kStream, m_paiBuildingClassEnabled);
}

/// Serialization write
void CvReligionBeliefs::Write(FDataStream& kStream) const
{
	// Current version number
	uint uiVersion = 2;
	kStream << uiVersion;

	kStream << m_iFaithFromDyingUnits;
	kStream << m_iRiverHappiness;
	kStream << m_iPlotCultureCostModifier;
	kStream << m_iCityRangeStrikeModifier;
	kStream << m_iCombatModifierEnemyCities;
	kStream << m_iCombatModifierFriendlyCities;
	kStream << m_iFriendlyHealChange;
	kStream << m_iCityStateFriendshipModifier;
	kStream << m_iLandBarbarianConversionPercent;
	kStream << m_iSpreadStrengthModifier;
	kStream << m_iSpreadDistanceModifier;
	kStream << m_iProphetStrengthModifier;
	kStream << m_iProphetCostModifier;
	kStream << m_iMissionaryStrengthModifier;
#ifdef NQ_FLAT_FAITH_PER_CITIZEN_BORN_FROM_BELIEFS
	kStream << m_iFlatFaithPerCitizenBorn;
#endif
#ifdef NQ_BELIEF_EXTRA_MISSIONARY_SPREADS
	kStream << m_iMissionaryExtraSpreads;
#endif
#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
	kStream << m_iSpreadModifierOwnedCities;
	kStream << m_iSpreadModifierUnownedCities;
#endif
#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	kStream << m_iExtraTradeRoutes;
#endif
#ifdef NQ_FAITH_PER_CITY_STATE_THIS_RELIGION
	kStream << m_iFaithPerCityStateThisReligion;
#endif
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	kStream << m_iFaithPerForeignTradeRoute;
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	kStream << m_iGoldenAgeTurns;
#endif
	kStream << m_iMissionaryCostModifier;
	kStream << m_iFriendlyCityStateSpreadModifier;
	kStream << m_iGreatPersonExpendedFaith;
	kStream << m_iCityStateMinimumInfluence;
	kStream << m_iCityStateInfluenceModifier;
	kStream << m_iOtherReligionPressureErosion;
	kStream << m_iSpyPressure;
	kStream << m_iInquisitorPressureRetention;
	kStream << m_iFaithBuildingTourism;

	kStream << m_eObsoleteEra;
	kStream << m_eResourceRevealed;
	kStream << m_eSpreadModifierDoublingTech;

	// m_ReligionBeliefs contains the BeliefTypes, which are indices into the religion info table (GC.getBeliefInfo).  Write out the info hashes
	kStream << m_ReligionBeliefs.size();
	for (uint i = 0; i < m_ReligionBeliefs.size(); ++i)
	{
		CvInfosSerializationHelper::WriteHashed(kStream, (BeliefTypes)m_ReligionBeliefs[i]);
	}

	BuildingClassArrayHelpers::Write(kStream, m_paiBuildingClassEnabled, GC.getNumBuildingClassInfos());
}

/// BELIEF HELPER CLASSES

/// Is there an adjacent barbarian naval unit that could be converted?
bool CvBeliefHelpers::ConvertBarbarianUnit(CvPlayer *pPlayer, UnitHandle pUnit)
{
	UnitHandle pNewUnit;
	CvPlot *pPlot = pUnit->plot();

	// Convert the barbarian into our unit
#ifdef AUI_UNIT_FIX_GIFTED_UNITS_ARE_GIFTED_NOT_CLONED
	pNewUnit = pPlayer->initUnit(pUnit->getUnitType(), pUnit->getX(), pUnit->getY(), pUnit->AI_getUnitAIType(), NO_DIRECTION, true /*bNoMove*/, false, DEFAULT_UNIT_MAP_LAYER, 0, true);
#else
	pNewUnit = pPlayer->initUnit(pUnit->getUnitType(), pUnit->getX(), pUnit->getY(), pUnit->AI_getUnitAIType(), NO_DIRECTION, true /*bNoMove*/, false);
#endif
	CvAssertMsg(pNewUnit, "pNewUnit is not assigned a valid value");
	pNewUnit->convert(pUnit.pointer(), false);
	pNewUnit->setupGraphical();
	pNewUnit->finishMoves(); // No move first turn

	if(GC.getLogging() && GC.getAILogging())
	{
		CvString logMsg;
		logMsg.Format("Converted barbarian (with belief), X: %d, Y: %d", pUnit->getX(), pUnit->getY());
		pPlayer->GetHomelandAI()->LogHomelandMessage(logMsg);
	}

	CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_HEATHEN_CONVERTS");
	CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_HEATHEN_CONVERTS");
	pPlayer->GetNotifications()->Add(NOTIFICATION_GENERIC, strBuffer, strSummary, pUnit->getX(), pUnit->getY(), -1);

	if (pPlot->getImprovementType() == GC.getBARBARIAN_CAMP_IMPROVEMENT())
	{
		pPlot->setImprovementType(NO_IMPROVEMENT);

		CvBarbarians::DoBarbCampCleared(pPlot, pPlayer->GetID());
		pPlot->SetPlayerThatClearedBarbCampHere(pPlayer->GetID());

		// Don't give gold for Camps cleared by heathen conversion
	}

	return true;
}