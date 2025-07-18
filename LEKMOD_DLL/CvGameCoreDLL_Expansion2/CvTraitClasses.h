/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_TRAIT_CLASSES_H
#define CIV5_TRAIT_CLASSES_H

struct FreeResourceXCities
{
	FreeResourceXCities():
		m_iNumCities(0),
		m_iResourceQuantity(0)
	{};

	int m_iNumCities;
	int m_iResourceQuantity;
};

struct MayaBonusChoice
{
	MayaBonusChoice():
		m_eUnitType(NO_UNIT),
		m_iBaktunJustFinished(0)
	{};

	MayaBonusChoice(const MayaBonusChoice& in)
	{
		m_eUnitType = in.m_eUnitType;
		m_iBaktunJustFinished = in.m_iBaktunJustFinished;
	};

	UnitTypes m_eUnitType;
	int m_iBaktunJustFinished;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvTraitEntry
//!  \brief		A single entry in the trait XML file
//
//!  Key Attributes:
//!  - Used to be called CvTraitInfo
//!  - Populated from XML\Civilizations\CIV5Traits.xml
//!  - Array of these contained in CvTraitXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvTraitEntry: public CvBaseInfo
{
public:
	CvTraitEntry(void);
	~CvTraitEntry(void);

	int GetLevelExperienceModifier() const;
	int GetGreatPeopleRateModifier() const;
	int GetGreatScientistRateModifier() const;
	int GetGreatGeneralRateModifier() const;
	int GetGreatGeneralExtraBonus() const;
	int GetGreatPersonGiftInfluence() const;
	int GetMaxGlobalBuildingProductionModifier() const;
	int GetMaxTeamBuildingProductionModifier() const;
	int GetMaxPlayerBuildingProductionModifier() const;
	int GetCityUnhappinessModifier() const;
	int GetPopulationUnhappinessModifier() const;
	int GetCityStateBonusModifier() const;
	int GetCityStateFriendshipModifier() const;
	int GetCityStateCombatModifier() const;
	int GetLandBarbarianConversionPercent() const;
	int GetLandBarbarianConversionExtraUnits() const;
	int GetSeaBarbarianConversionPercent() const;
	int GetCapitalBuildingModifier() const;
	int GetPlotBuyCostModifier() const;
	int GetPlotCultureCostModifier() const;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	int GetSciencePerGreatPersonBorn() const;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	int GetNumTurnsBeforeMinorAlliesRefuseBribes() const;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	int GetGoldenAgeTileBonusFaith() const;
#endif
	int GetCultureFromKills() const;
	int GetFaithFromKills() const;
	int GetCityCultureBonus() const;
	int GetCapitalThemingBonusModifier() const;
	int GetPolicyCostModifier() const;
	int GetCityConnectionTradeRouteChange() const;
	int GetWonderProductionModifier() const;
	int GetPlunderModifier() const;
	int GetImprovementMaintenanceModifier() const;
	int GetGoldenAgeDurationModifier() const;
	int GetGoldenAgeMoveChange() const;
	int GetGoldenAgeCombatModifier() const;
	int GetGoldenAgeTourismModifier() const;
	int GetGoldenAgeGreatArtistRateModifier() const;
	int GetGoldenAgeGreatMusicianRateModifier() const;
	int GetGoldenAgeGreatWriterRateModifier() const;
	int GetExtraEmbarkMoves() const;
	int GetFreeUnitClassType() const;
	int GetNaturalWonderFirstFinderGold() const;
	int GetNaturalWonderSubsequentFinderGold() const;
	
#if defined(TRAITIFY) // Int and Bool getters
	bool IsHalfMoreSpecialistUnhappiness() const;

	int GetGoldenAgeCultureModifier() const;
	int GetNumExtraLeagueVotes() const;
	int GetNumTradeRouteBonus() const;
	int GetMinorFriendshipMinimum() const;
	int GetGreatEngineerRateModifier() const;
	int GetGreatMerchantRateModifier() const;
	int GetMinorBullyModifier() const;
	int GetExtraPopulation() const;
	int GetInternationalRouteGrowthModifier() const;
	int GetLocalHappinessPerCity() const;
	int GetInternalTradeRouteYieldModifier() const;
	int GetUnhappinessModifierForPuppetedCities() const;
	int GetFaithCostModifier() const; 
	int GetIdeologyPressureUnhappinessModifier() const;
	int GetForeignRelgionPressureModifier() const;
	int GetFriendlyLandsCitizenMoveChange() const;
#endif
#if defined(LEKMOD_v34)
	bool IsReligionEnhanceReformation() const;

	int GetSelfReligiousPressureModifier() const;
	int GetLandTradeRouteYieldBonus() const;
#endif

	//EAP: Natural Wonder finder faith
	int GetNaturalWonderFirstFinderFaith() const;
	int GetNaturalWonderSubsequentFinderFaith() const;

	int GetNaturalWonderYieldModifier() const;
	int GetNaturalWonderHappinessModifier() const;
	int GetNearbyImprovementCombatBonus() const;
	int GetNearbyImprovementBonusRange() const;
	int GetCultureBuildingYieldChange() const;
	int GetCombatBonusVsHigherTech() const;
	int GetCombatBonusVsLargerCiv() const;
	int GetLandUnitMaintenanceModifier() const;
	int GetNavalUnitMaintenanceModifier() const;
	int GetRazeSpeedModifier() const;
	int GetDOFGreatPersonModifier() const;
	int GetLuxuryHappinessRetention() const;
	int GetExtraHappinessPerLuxury() const; // NQMP GJS - New Netherlands UA
	int GetExtraSpies() const;
	int GetHappinessPerReligion() const; // NQMP GJS - New Ottoman UA
	int GetUnresearchedTechBonusFromKills() const;
	int GetExtraFoundedCityTerritoryClaimRange() const;
	int GetFreeSocialPoliciesPerEra() const;
	int GetNumTradeRoutesModifier() const;
	int GetTradeRouteResourceModifier() const;
	int GetUniqueLuxuryCities() const;
	int GetUniqueLuxuryQuantity() const;
	int GetWorkerSpeedModifier() const;
	int GetAfraidMinorPerTurnInfluence() const;
	int GetLandTradeRouteRangeBonus() const;
	int GetTradeReligionModifier() const;
	int GetTradeBuildingModifier() const;
	
	// Here we insert usefull new table entries to smooth out existing code (From the Community-Patch DLL) ~EAP
	TechTypes GetFreeBuildingPrereqTech() const; 
	TechTypes GetCapitalFreeBuildingPrereqTech() const;


	TechTypes GetFreeUnitPrereqTech() const;
	ImprovementTypes GetCombatBonusImprovement() const;
	BuildingTypes GetFreeBuilding() const;
	BuildingTypes GetFreeCapitalBuilding() const;
	BuildingTypes GetFreeBuildingOnConquest() const;

	bool IsFightWellDamaged() const;
	bool IsMoveFriendlyWoodsAsRoad() const;
	bool IsFasterAlongRiver() const;
	bool IsFasterInHills() const;
	bool IsEmbarkedAllWater() const;
	bool IsEmbarkedToLandFlatCost() const;
	bool IsNoHillsImprovementMaintenance() const;
	bool IsTechBoostFromCapitalScienceBuildings() const;
	bool IsStaysAliveZeroCities() const;
	bool IsFaithFromUnimprovedForest() const;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	bool IsSeaTradeRoutesArePlunderImmune() const;
#endif
	// NQMP GJS - New France UA begin
	bool IsEarnsGreatPersonOnSlotOrGuild() const;
	bool IsHasBuiltWritersGuild() const;
	void SetHasBuiltWritersGuild(bool hasBuilt);
	bool IsHasBuiltArtistsGuild() const;
	void SetHasBuiltArtistsGuild(bool hasBuilt);
	bool IsHasBuiltMusiciansGuild() const;
	void SetHasBuiltMusiciansGuild(bool hasBuilt);
	// NQMP GJS - New France UA end

	bool IsBonusReligiousBelief() const;
	bool IsAbleToAnnexCityStates() const;
	bool IsCrossesMountainsAfterGreatGeneral() const;
	bool IsMayaCalendarBonuses() const;
	bool IsNoAnnexing() const;
	bool IsTechFromCityConquer() const;
	bool IsUniqueLuxuryRequiresNewArea() const;
	bool IsRiverTradeRoad() const;
	bool IsAngerFreeIntrusionOfCityStates() const;

	const char* getShortDescription() const;
	void setShortDescription(const char* szVal);

	// Arrays
#if defined(TRAITIFY) //Arrays
	bool IsBuildingClassRemoveRequiredTerrain(BuildingClassTypes eBuildingClass) const;
	bool IsUnitClassForceSpawnCapital(UnitClassTypes eUnitClass) const;

	int GetResourceYieldChanges(int i, int j) const;
	int GetTerrainYieldChanges(int i, int j) const;
	int GetBuildingClassHappiness(int i) const;
	int GetBuildingClassGlobalHappiness(int i) const;
	int GetResourceClassYieldChanges(int i, int j) const;
	int GetBuildingCostOverride(int i, int j) const;
	int GetPuppetYieldModifiers(int i) const;
	int GetBuildingClassYieldChanges(int i, int j) const;
	int GetFeatureYieldChanges(FeatureTypes eIndex1, YieldTypes eIndex2) const;
	int GetRouteMovementChange(int i) const;
#endif
#if defined(LEKMOD_v34)
	int GetYieldPerPopulation(int i) const;
	int GetYieldPerPopulationForeignReligion(int i) const;
#endif
#if defined(FULL_YIELD_FROM_KILLS)
	int GetYieldFromKills(int i) const;
#endif
	int GetExtraYieldThreshold(int i) const;
	int GetYieldChange(int i) const;
	int GetYieldChangeStrategicResources(int i) const;
	int GetYieldChangeLuxuryResources(int i) const; // NQMP GJS - New Netherlands UA
	int GetYieldChangeNaturalWonder(int i) const;
	int GetYieldChangePerTradePartner(int i) const;
	int GetYieldChangeIncomingTradeRoute(int i) const;
	int GetYieldModifier(int i) const;
	int GetStrategicResourceQuantityModifier(int i) const;
	int GetObsoleteTech() const;
	int GetPrereqTech() const;
	int GetResourceQuantityModifier(int i) const;
	int GetMovesChangeUnitCombat(const int unitCombatID) const;
	int GetMaintenanceModifierUnitCombat(const int unitCombatID) const;
	int GetImprovementYieldChanges(ImprovementTypes eIndex1, YieldTypes eIndex2) const;
	int GetSpecialistYieldChanges(SpecialistTypes eIndex1, YieldTypes eIndex2) const;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	int GetAnySpecificSpecialistYieldChanges(SpecialistTypes eIndex1, YieldTypes eIndex2) const;
#endif

	int GetUnimprovedFeatureYieldChanges(FeatureTypes eIndex1, YieldTypes eIndex2) const;
	FreeResourceXCities GetFreeResourceXCities(ResourceTypes eResource) const;

	bool IsFreePromotionUnitCombat(const int promotionID, const int unitCombatID) const;
	bool IsObsoleteByTech(TeamTypes eTeam);
	bool IsEnabledByTech(TeamTypes eTeam);

	bool NoTrain(UnitClassTypes eUnitClassType);

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	bool NoBuildImprovements(ImprovementTypes eImprovement);
#endif
#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	int GetBuildTimeOverride(BuildTypes eBuild, ResourceClassTypes eResourceClass = NO_RESOURCECLASS);
	
	// Add accessor methods for direct access to the vectors
	int GetBuildTimeOverrideVector(int iBuild) const { return m_aiBuildTimeOverride[iBuild]; }
	ResourceClassTypes GetBuildTimeOverrideResourceClassRequiredVector(int iBuild) const { return m_aiBuildTimeOverrideResourceClassRequired[iBuild]; }
	
	// Add accessor method for the multimap
	const std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>>& GetBuildTimeOverridesMultimap() const { return m_BuildTimeOverrides; }
#endif

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

protected:
	int m_iLevelExperienceModifier;
	int m_iGreatPeopleRateModifier;
	int m_iGreatScientistRateModifier;
	int m_iGreatGeneralRateModifier;
	int m_iGreatGeneralExtraBonus;
	int m_iGreatPersonGiftInfluence;
	int m_iMaxGlobalBuildingProductionModifier;
	int m_iMaxTeamBuildingProductionModifier;
	int m_iMaxPlayerBuildingProductionModifier;
	int m_iCityUnhappinessModifier;
	int m_iPopulationUnhappinessModifier;
	int m_iCityStateBonusModifier;
	int m_iCityStateFriendshipModifier;
	int m_iCityStateCombatModifier;
	int m_iLandBarbarianConversionPercent;
	int m_iLandBarbarianConversionExtraUnits;
	int m_iSeaBarbarianConversionPercent;
	int m_iCapitalBuildingModifier;
	int m_iPlotBuyCostModifier;
	int m_iPlotCultureCostModifier;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	int m_iSciencePerGreatPersonBorn;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	int m_iNumTurnsBeforeMinorAlliesRefuseBribes;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	int m_iGoldenAgeTileBonusFaith;
#endif

	int m_iCultureFromKills;
	int m_iFaithFromKills;
	int m_iCityCultureBonus;
	int m_iCapitalThemingBonusModifier;
	int m_iPolicyCostModifier;
	int m_iCityConnectionTradeRouteChange;
	int m_iWonderProductionModifier;
	int m_iPlunderModifier;
	int m_iImprovementMaintenanceModifier;
	int m_iGoldenAgeDurationModifier;
	int m_iGoldenAgeMoveChange;
	int m_iGoldenAgeCombatModifier;
	int m_iGoldenAgeTourismModifier;
	int m_iGoldenAgeGreatArtistRateModifier;
	int m_iGoldenAgeGreatMusicianRateModifier;
	int m_iGoldenAgeGreatWriterRateModifier;
	int m_iObsoleteTech;
	int m_iPrereqTech;
	int m_iExtraEmbarkMoves;
	int m_iFreeUnitClassType;
	int m_iNaturalWonderFirstFinderGold;
	int m_iNaturalWonderSubsequentFinderGold;

#if defined(TRAITIFY) // int and bool members
	bool m_bHalfMoreSpecialistUnhappiness;

	int m_iGoldenAgeCultureModifier;
	int m_iNumExtraLeagueVotes;
	int m_iNumTradeRouteBonus;
	int m_iMinorFriendshipMinimum;
	int m_iGreatEngineerRateModifier;
	int m_iGreatMerchantRateModifier;
	int m_iMinorBullyModifier;
	int m_iExtraPopulation;
	int m_iInternationalRouteGrowthModifier;
	int m_iLocalHappinessPerCity;
	int m_iInternalTradeRouteYieldModifier;
	int m_iUnhappinessModifierForPuppetedCities;
	int m_iFaithCostModifier;
	int m_iIdeologyPressureUnhappinessModifier;
	int m_iForeignRelgionPressureModifier;
	int m_iFriendlyLandsCitizenMoveChange;
#endif
#if defined(LEKMOD_v34)
	bool m_bReligionEnhanceReformation;

	int m_iSelfReligiousPressureModifier;
	int m_iLandTradeRouteYieldBonus;
#endif

	//EAP: Natural wonder faith for the finder
	int m_iNaturalWonderFirstFinderFaith;
	int m_iNaturalWonderSubsequentFinderFaith;

	int m_iNaturalWonderYieldModifier;
	int m_iNaturalWonderHappinessModifier;
	int m_iNearbyImprovementCombatBonus;
	int m_iNearbyImprovementBonusRange;
	int m_iCultureBuildingYieldChange;
	int m_iCombatBonusVsHigherTech;
	int m_iCombatBonusVsLargerCiv;
	int m_iLandUnitMaintenanceModifier;
	int m_iNavalUnitMaintenanceModifier;
	int m_iRazeSpeedModifier;
	int m_iDOFGreatPersonModifier;
	int m_iLuxuryHappinessRetention;
	int m_iExtraHappinessPerLuxury; // NQMP GJS - New Netherlands UA
	int m_iExtraSpies;
	int m_iHappinessPerReligion; // NQMP GJS - New Ottoman UA
	int m_iUnresearchedTechBonusFromKills;
	int m_iExtraFoundedCityTerritoryClaimRange;
	int m_iFreeSocialPoliciesPerEra;
	int m_iNumTradeRoutesModifier;
	int m_iTradeRouteResourceModifier;
	int m_iUniqueLuxuryCities;
	int m_iUniqueLuxuryQuantity;
	int m_iWorkerSpeedModifier;
	int m_iAfraidMinorPerTurnInfluence;
	int m_iLandTradeRouteRangeBonus;
	int m_iTradeReligionModifier;
	int m_iTradeBuildingModifier;

	TechTypes m_eFreeUnitPrereqTech;
	ImprovementTypes m_eCombatBonusImprovement;
	BuildingTypes m_eFreeBuilding;
	BuildingTypes m_eFreeCapitalBuilding;
	TechTypes m_eFreeBuildingPrereqTech;
	TechTypes m_eCapitalFreeBuildingPrereqTech;
	BuildingTypes m_eFreeBuildingOnConquest;

	bool m_bFightWellDamaged;
	bool m_bMoveFriendlyWoodsAsRoad;
	bool m_bFasterAlongRiver;
	bool m_bFasterInHills;
	bool m_bEmbarkedAllWater;
	bool m_bEmbarkedToLandFlatCost;
	bool m_bNoHillsImprovementMaintenance;
	bool m_bTechBoostFromCapitalScienceBuildings;
	bool m_bStaysAliveZeroCities;
	bool m_bFaithFromUnimprovedForest;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	bool m_bSeaTradeRoutesArePlunderImmune;
#endif

	// NQMP GJS - New France UA begin
	bool m_bEarnsGreatPersonOnSlotOrGuild;
	bool m_bHasBuiltWritersGuild;
	bool m_bHasBuiltArtistsGuild;
	bool m_bHasBuiltMusiciansGuild;
	// NQMP GJS - New France UA end

	bool m_bBonusReligiousBelief;
	bool m_bAbleToAnnexCityStates;
	bool m_bCrossesMountainsAfterGreatGeneral;
	bool m_bMayaCalendarBonuses;
	bool m_bNoAnnexing;
	bool m_bTechFromCityConquer;
	bool m_bUniqueLuxuryRequiresNewArea;
	bool m_bRiverTradeRoad;
	bool m_bAngerFreeIntrusionOfCityStates;

	CvString m_strShortDescription;

	// Arrays
#if defined(TRAITIFY) //Array members
	std::vector<bool> m_abBuildingClassRemoveRequiredTerrain;
	std::vector<bool> m_abUnitClassForceSpawnCapital;
	int** m_ppiBuildingCostOverride;
	int** m_ppiBuildingClassYieldChanges;
	int** m_ppiResourceClassYieldChanges;
	int** m_ppiFeatureYieldChanges;
	int** m_ppiResourceYieldChanges;
	int** m_ppiTerrainYieldChanges;

	int* m_paiRouteMovementChange;
	int* m_paiBuildingClassHappiness;
	int* m_paiBuildingClassGlobalHappiness;
	int* m_piPuppetYieldModifiers;
#endif
#if defined(LEKMOD_v34)
	int* m_paiYieldPerPopulation;
	int* m_paiYieldPerPopulationForeignReligion;
#endif
#if defined(FULL_YIELD_FROM_KILLS)
	int* m_paiYieldFromKills;
#endif
	int* m_paiExtraYieldThreshold;
	int* m_paiYieldChange;
	int* m_paiYieldChangeStrategicResources;
	int* m_paiYieldChangeLuxuryResources; // NQMP GJS - New Netherlands UA
	int* m_paiYieldChangeNaturalWonder;
	int* m_paiYieldChangePerTradePartner;
	int* m_paiYieldChangeIncomingTradeRoute;
	int* m_paiYieldModifier;
	int* m_piStrategicResourceQuantityModifier;
	int* m_piResourceQuantityModifiers;
	int* m_piMovesChangeUnitCombats;
	int* m_piMaintenanceModifierUnitCombats;
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	std::pair<int**, size_t> m_ppiImprovementYieldChanges;
	std::pair<int**, size_t> m_ppiSpecialistYieldChanges;
	std::pair<int**, size_t> m_ppiUnimprovedFeatureYieldChanges;
#else
	int** m_ppiImprovementYieldChanges;
	int** m_ppiSpecialistYieldChanges;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	int** m_ppiAnySpecificSpecialistYieldChanges;
#endif

	int** m_ppiUnimprovedFeatureYieldChanges;
#endif


	std::multimap<int, int> m_FreePromotionUnitCombats;

	std::vector<FreeResourceXCities> m_aFreeResourceXCities;
	std::vector<bool> m_abNoTrainUnitClass;

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	std::vector<bool> m_abNoBuildImprovements;
#endif

#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	std::vector<int> m_aiBuildTimeOverride;
	std::vector<ResourceClassTypes> m_aiBuildTimeOverrideResourceClassRequired;
	std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>> m_BuildTimeOverrides;
#endif

private:
	CvTraitEntry(const CvTraitEntry&);
	CvTraitEntry& operator=(const CvTraitEntry&);
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvTraitXMLEntries
//!  \brief		Game-wide information about civilization traits
//
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\GameInfo\CIV5Traits.xml
//! - Contains an array of CvTraitEntry from the above XML file
//! - One instance for the entire game
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvTraitXMLEntries
{
public:
	CvTraitXMLEntries(void);
	~CvTraitXMLEntries(void);

	// Accessor functions
	std::vector<CvTraitEntry*>& GetTraitEntries();
	int GetNumTraits();
	CvTraitEntry* GetEntry(int index);

	void DeleteArray();

private:
	std::vector<CvTraitEntry*> m_paTraitEntries;
};

#define SAFE_ESTIMATE_NUM_FREE_UNITS 5

struct FreeTraitUnit
{
#ifdef AUI_WARNING_FIXES
	UnitClassTypes m_iFreeUnit;
#else
	UnitTypes m_iFreeUnit;
#endif
	TechTypes m_ePrereqTech;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPlayerTraits
//!  \brief		Information about the traits of a player
//
//!  Key Attributes:
//!  - One instance for each player
//!  - Accessed by any class that needs to check trait info
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPlayerTraits
{
public:
	CvPlayerTraits(void);
	~CvPlayerTraits(void);
	void Init(CvTraitXMLEntries* pTraits, CvPlayer* pPlayer);
	void Uninit();
	void Reset();
	void InitPlayerTraits();

	// Accessor functions
	bool HasTrait(TraitTypes eTrait) const;
	int GetGreatPeopleRateModifier() const
	{
		return m_iGreatPeopleRateModifier;
	};
	int GetGreatScientistRateModifier() const
	{
		return m_iGreatScientistRateModifier;
	};
	int GetGreatGeneralRateModifier() const
	{
		return m_iGreatGeneralRateModifier;
	};
	int GetGreatGeneralExtraBonus() const
	{
		return m_iGreatGeneralExtraBonus;
	};
	int GetGreatPersonGiftInfluence() const
	{
		return m_iGreatPersonGiftInfluence;
	};
	int GetLevelExperienceModifier() const
	{
		return m_iLevelExperienceModifier;
	};
	int GetMaxGlobalBuildingProductionModifier() const
	{
		return m_iMaxGlobalBuildingProductionModifier;
	};
	int GetMaxTeamBuildingProductionModifier() const
	{
		return m_iMaxTeamBuildingProductionModifier;
	};
	int GetMaxPlayerBuildingProductionModifier() const
	{
		return m_iMaxPlayerBuildingProductionModifier;
	};
	int GetCityUnhappinessModifier() const
	{
		return m_iCityUnhappinessModifier;
	};
	int GetPopulationUnhappinessModifier() const
	{
		return m_iPopulationUnhappinessModifier;
	};
	int GetCityStateBonusModifier() const
	{
		return m_iCityStateBonusModifier;
	};
	int GetCityStateFriendshipModifier() const
	{
		return m_iCityStateFriendshipModifier;
	};
	int GetCityStateCombatModifier() const
	{
		return m_iCityStateCombatModifier;
	};
	int GetLandBarbarianConversionPercent() const
	{
		return m_iLandBarbarianConversionPercent;
	};
	int GetLandBarbarianConversionExtraUnits() const
	{
		return m_iLandBarbarianConversionExtraUnits;
	};
	int GetSeaBarbarianConversionPercent() const
	{
		return m_iSeaBarbarianConversionPercent;
	};
	int GetCapitalBuildingModifier() const
	{
		return m_iCapitalBuildingModifier;
	};
	int GetPlotBuyCostModifier() const
	{
		return m_iPlotBuyCostModifier;
	};
	int GetPlotCultureCostModifier() const
	{
		return m_iPlotCultureCostModifier;
	};
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	int GetSciencePerGreatPersonBorn() const
	{
		return m_iSciencePerGreatPersonBorn;
	}
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	int GetNumTurnsBeforeMinorAlliesRefuseBribes() const
	{
		return m_iNumTurnsBeforeMinorAlliesRefuseBribes;
	}
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	int GetGoldenAgeTileBonusFaith() const
	{
		return m_iGoldenAgeTileBonusFaith;
	}
#endif
	int GetCultureFromKills() const
	{
		return m_iCultureFromKills;
	};
	int GetFaithFromKills() const
	{
		return m_iFaithFromKills;
	};
	int GetCityCultureBonus() const
	{
		return m_iCityCultureBonus;
	};
	int GetCapitalThemingBonusModifier() const
	{
		return m_iCapitalThemingBonusModifier;
	}
	int GetPolicyCostModifier() const
	{
		return m_iPolicyCostModifier;
	};
	int GetCityConnectionTradeRouteChange() const
	{
		return m_iCityConnectionTradeRouteChange;
	};
	int GetWonderProductionModifier() const
	{
		return m_iWonderProductionModifier;
	};
	int GetPlunderModifier() const
	{
		return m_iPlunderModifier;
	};
	int GetImprovementMaintenanceModifier() const
	{
		return m_iImprovementMaintenanceModifier;
	};
	int GetGoldenAgeDurationModifier() const
	{
		return m_iGoldenAgeDurationModifier;
	};
	int GetGoldenAgeMoveChange() const
	{
		return m_iGoldenAgeMoveChange;
	};
	int GetGoldenAgeCombatModifier() const
	{
		return m_iGoldenAgeCombatModifier;
	};
	int GetGoldenAgeTourismModifier() const
	{
		return m_iGoldenAgeTourismModifier;
	};
	int GetGoldenAgeGreatArtistRateModifier() const
	{
		return m_iGoldenAgeGreatArtistRateModifier;
	};
	int GetGoldenAgeGreatMusicianRateModifier() const
	{
		return m_iGoldenAgeGreatMusicianRateModifier;
	};
	int GetGoldenAgeGreatWriterRateModifier() const
	{
		return m_iGoldenAgeGreatWriterRateModifier;
	};
	int GetExtraEmbarkMoves() const
	{
		return m_iExtraEmbarkMoves;
	};
	int GetFirstFreeUnit(TechTypes eTech);
	int GetNextFreeUnit();

#if defined(TRAITIFY) // int and bool getters
	bool IsHalfMoreSpecialistUnhappiness() const
	{
		return m_bHalfMoreSpecialistUnhappiness;
	};
	int GetGoldenAgeCultureModifier() const
	{
		return m_iGoldenAgeCultureModifier;
	};
	int GetNumExtraLeagueVotes() const
	{
		return m_iNumExtraLeagueVotes;
	};
	int GetNumTradeRouteBonus() const
	{
		return m_iNumTradeRouteBonus;
	};
	int GetMinorFriendshipMinimum() const
	{
		return m_iMinorFriendshipMinimum;
	};
	int GetGreatEngineerRateModifier() const
	{
		return m_iGreatEngineerRateModifier;
	};
	int GetGreatMerchantRateModifier() const
	{
		return m_iGreatMerchantRateModifier;
	};
	int GetMinorBullyModifier() const
	{
		return m_iMinorBullyModifier;
	};
	int GetExtraPopulation() const
	{
		return m_iExtraPopulation;
	};
	int GetInternationalRouteGrowthModifier() const
	{
		return m_iInternationalRouteGrowthModifier;
	};
	int GetLocalHappinessPerCity() const
	{
		return m_iLocalHappinessPerCity;
	};
	int GetInternalTradeRouteYieldModifier() const
	{
		return m_iInternalTradeRouteYieldModifier;
	};
	int GetUnhappinessModifierForPuppetedCities() const
	{
		return m_iUnhappinessModifierForPuppetedCities;
	};
	int GetFaithCostModifier() const
	{
		return m_iFaithCostModifier;
	};
	int GetIdeologyPressureUnhappinessModifier() const
	{
		return m_iIdeologyPressureUnhappinessModifier;
	};
	int GetForeignRelgionPressureModifier() const
	{
		return m_iForeignRelgionPressureModifier;
	};
	int GetFriendlyLandsCitizenMoveChange() const
	{
		return m_iFriendlyLandsCitizenMoveChange;
	};
#endif
#if defined(LEKMOD_v34)
	bool IsReligionEnhanceReformation() const
	{
		return m_bReligionEnhanceReformation;
	};
	int GetSelfReligiousPressureModifier() const
	{
		return m_iSelfReligiousPressureModifier;
	};
	int GetLandTradeRouteYieldBonus() const
	{
		return m_iLandTradeRouteYieldBonus;
	};
#endif
	int GetNaturalWonderFirstFinderGold() const
	{
		return m_iNaturalWonderFirstFinderGold;
	};
	int GetNaturalWonderSubsequentFinderGold() const
	{
		return m_iNaturalWonderSubsequentFinderGold;
	};

	//EAP Faith for the natural wonder finder

	int GetNaturalWonderFirstFinderFaith() const
	{
		return m_iNaturalWonderFirstFinderFaith;
	};
	int GetNaturalWonderSubsequentFinderFaith() const
	{
		return m_iNaturalWonderSubsequentFinderFaith;
	};


	int GetNaturalWonderYieldModifier() const
	{
		return m_iNaturalWonderYieldModifier;
	};
	int GetNaturalWonderHappinessModifier() const
	{
		return m_iNaturalWonderHappinessModifier;
	};
	int GetNearbyImprovementCombatBonus() const
	{
		return m_iNearbyImprovementCombatBonus;
	};
	int GetNearbyImprovementBonusRange() const
	{
		return m_iNearbyImprovementBonusRange;
	};
	int GetCultureBuildingYieldChange() const
	{
		return m_iCultureBuildingYieldChange;
	};
	int GetCombatBonusVsHigherTech() const
	{
		return m_iCombatBonusVsHigherTech;
	};
	int GetCombatBonusVsLargerCiv() const
	{
		return m_iCombatBonusVsLargerCiv;
	};
	int GetLandUnitMaintenanceModifier() const
	{
		return m_iLandUnitMaintenanceModifier;
	};
	int GetNavalUnitMaintenanceModifier() const
	{
		return m_iNavalUnitMaintenanceModifier;
	};
	int GetRazeSpeedModifier() const
	{
		return m_iRazeSpeedModifier;
	};
	int GetDOFGreatPersonModifier() const
	{
		return m_iDOFGreatPersonModifier;
	};
	int GetLuxuryHappinessRetention() const
	{
		return m_iLuxuryHappinessRetention;
	};
	// NQMP GJS - New Netherlands UA gives +1 Happiness per unique Luxury for the empire BEGIN
	int GetExtraHappinessPerLuxury() const
	{
		return m_iExtraHappinessPerLuxury;
	};
	// NQMP GJS - New Netherlands UA gives +1 Happiness per unique Luxury for the empire END
	int GetExtraSpies() const
	{
		return m_iExtraSpies;
	};
	// NQMP GJS - New Ottoman UA begin
	int GetHappinessPerReligion() const
	{
		return m_iHappinessPerReligion;
	};
	// NQMP GJS - New Ottoman UA end
	int GetUnresearchedTechBonusFromKills() const
	{
		return m_iUnresearchedTechBonusFromKills;
	}
	int GetExtraFoundedCityTerritoryClaimRange () const
	{
		return m_iExtraFoundedCityTerritoryClaimRange;
	}
	int GetFreeSocialPoliciesPerEra() const
	{
		return m_iFreeSocialPoliciesPerEra;
	}
	int GetNumTradeRoutesModifier() const
	{
		return m_iNumTradeRoutesModifier;
	}
	int GetTradeRouteResourceModifier() const
	{
		return m_iTradeRouteResourceModifier;
	}
	int GetUniqueLuxuryCities() const
	{
		return m_iUniqueLuxuryCities;
	}
	int GetUniqueLuxuryQuantity() const
	{
		return m_iUniqueLuxuryQuantity;
	}
	int GetWorkerSpeedModifier() const
	{
		return m_iWorkerSpeedModifier;
	}
	int GetAfraidMinorPerTurnInfluence() const
	{
		return m_iAfraidMinorPerTurnInfluence;
	}
	int GetLandTradeRouteRangeBonus() const
	{
		return m_iLandTradeRouteRangeBonus;
	}
	int GetTradeReligionModifier() const
	{
		return m_iTradeReligionModifier;
	}
	int GetTradeBuildingModifier() const
	{
		return m_iTradeBuildingModifier;
	}

	bool IsFightWellDamaged() const
	{
		return m_bFightWellDamaged;
	};
	bool IsMoveFriendlyWoodsAsRoad() const
	{
		return m_bMoveFriendlyWoodsAsRoad;
	};
	bool IsFasterAlongRiver() const
	{
		return m_bFasterAlongRiver;
	};
	bool IsFasterInHills() const
	{
		return m_bFasterInHills;
	};
	bool IsEmbarkedAllWater() const
	{
		return m_bEmbarkedAllWater;
	};
	bool IsEmbarkedToLandFlatCost() const
	{
		return m_bEmbarkedToLandFlatCost;
	};
	bool IsNoHillsImprovementMaintenance() const
	{
		return m_bNoHillsImprovementMaintenance;
	};
	bool IsTechBoostFromCapitalScienceBuildings() const
	{
		return m_bTechBoostFromCapitalScienceBuildings;
	};
	bool IsStaysAliveZeroCities() const
	{
		return m_bStaysAliveZeroCities;
	};
	// NQMP GJS - New France UA begin
	bool IsEarnsGreatPersonOnSlotOrGuild() const
	{
		return m_bEarnsGreatPersonOnSlotOrGuild;
	};
	bool IsHasBuiltWritersGuild() const
	{
		return m_bHasBuiltWritersGuild;
	};
	void SetHasBuiltWritersGuild(bool hasBuilt)
	{
		m_bHasBuiltWritersGuild = hasBuilt;
	};
	bool IsHasBuiltArtistsGuild() const
	{
		return m_bHasBuiltArtistsGuild;
	};
	void SetHasBuiltArtistsGuild(bool hasBuilt)
	{
		m_bHasBuiltArtistsGuild = hasBuilt;
	};
	bool IsHasBuiltMusiciansGuild() const
	{
		return m_bHasBuiltMusiciansGuild;
	};
	void SetHasBuiltMusiciansGuild(bool hasBuilt)
	{
		m_bHasBuiltMusiciansGuild = hasBuilt;
	};
	// NQMP GJS - New France UA end
	bool IsFaithFromUnimprovedForest() const
	{
		return m_bFaithFromUnimprovedForest;
	};
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	bool IsSeaTradeRoutesArePlunderImmune() const
	{
		return m_bSeaTradeRoutesArePlunderImmune;
	};
#endif
	bool IsBonusReligiousBelief() const
	{
		return m_bBonusReligiousBelief;
	};
	bool IsAbleToAnnexCityStates() const
	{
		return m_bAbleToAnnexCityStates;
	};
	bool IsCrossesMountainsAfterGreatGeneral() const
	{
		return m_bCrossesMountainsAfterGreatGeneral;
	};
	bool IsMayaCalendarBonuses() const
	{
		return m_bMayaCalendarBonuses;
	};
	bool IsNoAnnexing() const
	{
		return m_bNoAnnexing;
	};
	bool IsTechFromCityConquer() const
	{
		return m_bTechFromCityConquer;
	};
	bool IsUniqueLuxuryRequiresNewArea() const
	{
		return m_bUniqueLuxuryRequiresNewArea;
	}
	bool WillGetUniqueLuxury(CvArea *pArea) const;
	bool IsRiverTradeRoad() const
	{
		return m_bRiverTradeRoad;
	}
	bool IsAngerFreeIntrusionOfCityStates() const
	{
		return m_bAngerFreeIntrusionOfCityStates;
	}
	int GetExtraYieldThreshold(YieldTypes eYield) const
	{
		return m_iExtraYieldThreshold[(int)eYield];
	};
	int GetFreeCityYield(YieldTypes eYield) const
	{
		return m_iFreeCityYield[(int)eYield];
	};
	int GetYieldChangeStrategicResources(YieldTypes eYield) const
	{
		return m_iYieldChangeStrategicResources[(int)eYield];
	};
	// NQMP GJS - New Netherlands UA BEGIN
	int GetYieldChangeLuxuryResources(YieldTypes eYield) const
	{
		return m_iYieldChangeLuxuryResources[(int)eYield];
	};
	// NQMP GJS - New Netherlands UA END
	int GetYieldChangeNaturalWonder(YieldTypes eYield) const
	{
		return m_iYieldChangeNaturalWonder[(int)eYield];
	};
#if defined(FULL_YIELD_FROM_KILLS)
	int GetYieldFromKills(YieldTypes eYield) const
	{
		return m_iYieldFromKills[(int)eYield];
	};
#endif
#if defined(TRAITIFY)
	int GetPuppetYieldModifier(YieldTypes eYield) const
	{
		return m_iPuppetYieldModifiers[(int)eYield];
	};
	int GetRouteMovementChange(RouteTypes eRoute) const
	{
		return m_iRouteMovementChange[(int)eRoute];
	};
#endif
	int GetYieldChangePerTradePartner(YieldTypes eYield) const
	{
		return m_iYieldChangePerTradePartner[(int)eYield];
	};
	// NQMP GJS - Morocco UA Gateway To Africa now scales with era BEGIN
	int GetTradePartnerYieldFlatBonusPerEra() const
	{
		return 1; //m_iYieldChangePerTradePartner[(int)eYield]; // TODO: finish this to get the trait "TradePartnerYieldFlatBonusPerEra" from SQL
	};
	// NQMP GJS - Morocco UA Gateway To Africa now scales with era END
	int GetYieldChangeIncomingTradeRoute(YieldTypes eYield) const
	{
		return m_iYieldChangeIncomingTradeRoute[(int)eYield];
	};
	int GetYieldRateModifier(YieldTypes eYield) const
	{
		return m_iYieldRateModifier[(int)eYield];
	};
	int GetStrategicResourceQuantityModifier(TerrainTypes eTerrain) const
	{
		return m_iStrategicResourceQuantityModifier[(int)eTerrain];
	};
	int GetResourceQuantityModifier(ResourceTypes eResource) const
	{
		return ((uint)eResource < m_aiResourceQuantityModifier.size())?m_aiResourceQuantityModifier[(int)eResource]:0;
	};
	int GetMovesChangeUnitCombat(const int unitCombatID) const;
	int GetMaintenanceModifierUnitCombat(const int unitCombatID) const;
	int GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const;
	int GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE	
	int GetAnySpecificSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const;
#endif
	int GetUnimprovedFeatureYieldChange(FeatureTypes eFeature, YieldTypes eYield) const;
	FreeResourceXCities GetFreeResourceXCities(ResourceTypes eResource) const;

	bool HasFreePromotionUnitCombat(const int promotionID, const int unitCombatID) const;

	// Public functions to make trait-based game state changes
	void AddUniqueLuxuries(CvCity *pCity);
	bool CheckForBarbarianConversion(CvPlot* pPlot);
	int GetCapitalBuildingDiscount(BuildingTypes eBuilding);
	
	// Inserted Table Entries from CMP DLL ~EAP
	TechTypes GetFreeBuildingPrereqTech() const;
	TechTypes GetCapitalFreeBuildingPrereqTech() const;


	BuildingTypes GetFreeBuilding() const;
	BuildingTypes GetFreeCapitalBuilding() const;
	BuildingTypes GetFreeBuildingOnConquest() const;
	void SetDefeatedBarbarianCampGuardType(UnitTypes eType)
	{
		m_eCampGuardType = eType;
	};
	UnitTypes GetDefeatedBarbarianCampGuardType() const
	{
		return m_eCampGuardType;
	};
	void SetCombatBonusImprovementType(ImprovementTypes eType)
	{
		m_eCombatBonusImprovement = eType;
	};
	ImprovementTypes GetCombatBonusImprovementType() const
	{
		return m_eCombatBonusImprovement;
	};
	bool IsAbleToCrossMountains() const;

	bool NoTrain(UnitClassTypes eUnitClassType);

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	bool NoBuild(ImprovementTypes eImprovement);
#endif
#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	int GetBuildTimeOverride(BuildTypes eBuild, ResourceClassTypes eResourceClass = NO_RESOURCECLASS);
#endif
#if defined(TRAITIFY) //Array members
	bool IsBuildingClassRemoveRequiredTerrain(BuildingClassTypes eBuildingClass);
	bool IsUnitClassForceSpawnCapital(UnitClassTypes eUnitClass);

	int GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYield);
	int GetTerrainYieldChange(TerrainTypes eTerrain, YieldTypes eYield);
	int GetBuildingClassHappiness(BuildingClassTypes eBuildingClass);
	int GetBuildingClassGlobalHappiness(BuildingClassTypes eBuildingClass);
	int GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYieldType);
	int GetFeatureYieldChange(FeatureTypes eFeature, YieldTypes eYieldType) const;
	int GetBuildingCostOverride(BuildingTypes eBuilding, YieldTypes eYieldType);
	int GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYieldType);
#endif
#if defined(LEKMOD_v34)
	int GetYieldPerPopulation(YieldTypes eYield);
	int GetYieldPerPopulationForeignReligion(YieldTypes eYield);
#endif

	// Maya calendar routines
	bool IsUsingMayaCalendar() const;
	bool IsEndOfMayaLongCount();
	CvString GetMayaCalendarString();
	CvString GetMayaCalendarLongString();
	void ChooseMayaBoost();
	void ComputeMayaDate();
	int GetUnitBaktun(UnitTypes eUnit) const;
	void SetUnitBaktun(UnitTypes eUnit);
	bool IsFreeMayaGreatPersonChoice() const;

	// Serialization
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

	const std::vector<TraitTypes> GetPotentiallyActiveTraits() { return m_vPotentiallyActiveLeaderTraits; }

private:
	bool ConvertBarbarianCamp(CvPlot* pPlot);
	bool ConvertBarbarianNavalUnit(UnitHandle pUnit);

	CvTraitXMLEntries* m_pTraits;
	CvPlayer* m_pPlayer;
	std::vector<bool> m_vLeaderHasTrait;
	std::vector<TraitTypes> m_vPotentiallyActiveLeaderTraits;

	// Cached data about this player's traits
	int m_iGreatPeopleRateModifier;
	int m_iGreatScientistRateModifier;
	int m_iGreatGeneralRateModifier;
	int m_iGreatGeneralExtraBonus;
	int m_iGreatPersonGiftInfluence;
	int m_iLevelExperienceModifier;
	int m_iMaxGlobalBuildingProductionModifier;
	int m_iMaxTeamBuildingProductionModifier;
	int m_iMaxPlayerBuildingProductionModifier;
	int m_iCityUnhappinessModifier;
	int m_iPopulationUnhappinessModifier;
	int m_iCityStateBonusModifier;
	int m_iCityStateFriendshipModifier;
	int m_iCityStateCombatModifier;
	int m_iLandBarbarianConversionPercent;
	int m_iLandBarbarianConversionExtraUnits;
	int m_iSeaBarbarianConversionPercent;
	int m_iCapitalBuildingModifier;
	int m_iPlotBuyCostModifier;
	int m_iPlotCultureCostModifier;
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN
	int m_iSciencePerGreatPersonBorn;
#endif
#ifdef NQ_NUM_TURNS_BEFORE_MINOR_ALLIES_REFUSE_BRIBES_FROM_TRAIT
	int m_iNumTurnsBeforeMinorAlliesRefuseBribes;
#endif
#ifdef NQ_GOLDEN_PILGRIMAGE
	int m_iGoldenAgeTileBonusFaith;
#endif
	int m_iCultureFromKills;
	int m_iFaithFromKills;
	int m_iCityCultureBonus;
	int m_iCapitalThemingBonusModifier;
	int m_iPolicyCostModifier;
	int m_iCityConnectionTradeRouteChange;
	int m_iWonderProductionModifier;
	int m_iPlunderModifier;
	int m_iImprovementMaintenanceModifier;
	int m_iGoldenAgeDurationModifier;
	int m_iGoldenAgeMoveChange;
	int m_iGoldenAgeCombatModifier;
	int m_iGoldenAgeTourismModifier;
	int m_iGoldenAgeGreatArtistRateModifier;
	int m_iGoldenAgeGreatMusicianRateModifier;
	int m_iGoldenAgeGreatWriterRateModifier;
	int m_iExtraEmbarkMoves;
	int m_iNaturalWonderFirstFinderGold;
	int m_iNaturalWonderSubsequentFinderGold;
	int m_iNaturalWonderYieldModifier;

#if defined(TRAITIFY) // int and bool members
	bool m_bHalfMoreSpecialistUnhappiness;

	int m_iGoldenAgeCultureModifier;
	int m_iNumExtraLeagueVotes;
	int m_iNumTradeRouteBonus;
	int m_iMinorFriendshipMinimum;
	int m_iGreatEngineerRateModifier;
	int m_iGreatMerchantRateModifier;
	int m_iMinorBullyModifier;
	int m_iExtraPopulation;
	int m_iInternationalRouteGrowthModifier;
	int m_iLocalHappinessPerCity;
	int m_iInternalTradeRouteYieldModifier;
	int m_iUnhappinessModifierForPuppetedCities;
	int m_iFaithCostModifier;
	int m_iIdeologyPressureUnhappinessModifier;
	int m_iForeignRelgionPressureModifier;
	int m_iFriendlyLandsCitizenMoveChange;
#endif
#if defined(LEKMOD_v34)
	bool m_bReligionEnhanceReformation;

	int m_iSelfReligiousPressureModifier;
	int m_iLandTradeRouteYieldBonus;
#endif

	//EAP: Natural wonder faith for the finder
	int m_iNaturalWonderFirstFinderFaith;
	int m_iNaturalWonderSubsequentFinderFaith;
	


	int m_iNaturalWonderHappinessModifier;
	int m_iNearbyImprovementCombatBonus;
	int m_iNearbyImprovementBonusRange;
	int m_iCultureBuildingYieldChange;
	int m_iCombatBonusVsHigherTech;
	int m_iCombatBonusVsLargerCiv;
	int m_iLandUnitMaintenanceModifier;
	int m_iNavalUnitMaintenanceModifier;
	int m_iRazeSpeedModifier;
	int m_iDOFGreatPersonModifier;
	int m_iLuxuryHappinessRetention;
	int m_iExtraHappinessPerLuxury; // NQMP GJS - New Netherlands UA
	int m_iExtraSpies;
	int m_iHappinessPerReligion; //NQMP GJS - New Ottoman UA
	int m_iUnresearchedTechBonusFromKills;
	int m_iExtraFoundedCityTerritoryClaimRange;
	int m_iFreeSocialPoliciesPerEra;
	int m_iNumTradeRoutesModifier;
	int m_iTradeRouteResourceModifier;
	int m_iUniqueLuxuryCities;
	int m_iUniqueLuxuryQuantity;
	int m_iUniqueLuxuryCitiesPlaced;  
	int m_iWorkerSpeedModifier;
	int m_iAfraidMinorPerTurnInfluence; 
	int m_iLandTradeRouteRangeBonus;
	int m_iTradeReligionModifier;
	int m_iTradeBuildingModifier;
	// Saved

	bool m_bFightWellDamaged;
	bool m_bMoveFriendlyWoodsAsRoad;
	bool m_bFasterAlongRiver;
	bool m_bFasterInHills;
	bool m_bEmbarkedAllWater;
	bool m_bEmbarkedToLandFlatCost;
	bool m_bNoHillsImprovementMaintenance;
	bool m_bTechBoostFromCapitalScienceBuildings;
	bool m_bStaysAliveZeroCities;
	bool m_bFaithFromUnimprovedForest;
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	bool m_bSeaTradeRoutesArePlunderImmune;
#endif

	// NQMP GJS - New France UA begin
	bool m_bEarnsGreatPersonOnSlotOrGuild;
	bool m_bHasBuiltWritersGuild;
	bool m_bHasBuiltArtistsGuild;
	bool m_bHasBuiltMusiciansGuild;
	// NQMP GJS - New France UA end

	bool m_bBonusReligiousBelief;
	bool m_bAbleToAnnexCityStates;
	bool m_bCrossesMountainsAfterGreatGeneral;
	bool m_bMayaCalendarBonuses;
	bool m_bNoAnnexing;
	bool m_bTechFromCityConquer;
	bool m_bUniqueLuxuryRequiresNewArea;
	bool m_bRiverTradeRoad;
	bool m_bAngerFreeIntrusionOfCityStates;

	UnitTypes m_eCampGuardType;
	unsigned int m_uiFreeUnitIndex;
	TechTypes m_eFreeUnitPrereqTech;
	ImprovementTypes m_eCombatBonusImprovement;
	BuildingTypes m_eFreeBuilding;
	BuildingTypes m_eFreeCapitalBuilding;
	

	BuildingTypes m_eFreeBuildingOnConquest;
#if defined(FULL_YIELD_FROM_KILLS)
	int m_iYieldFromKills[NUM_YIELD_TYPES];
#endif
	int m_iExtraYieldThreshold[NUM_YIELD_TYPES];
	int m_iFreeCityYield[NUM_YIELD_TYPES];
	int m_iYieldChangeStrategicResources[NUM_YIELD_TYPES];
	int m_iYieldChangeLuxuryResources[NUM_YIELD_TYPES]; // NQMP GJS - New Netherlands UA
	int m_iYieldChangeNaturalWonder[NUM_YIELD_TYPES];
	int m_iYieldChangePerTradePartner[NUM_YIELD_TYPES];
	int m_iYieldChangeIncomingTradeRoute[NUM_YIELD_TYPES];
	int m_iYieldRateModifier[NUM_YIELD_TYPES];
	int m_iStrategicResourceQuantityModifier[NUM_TERRAIN_TYPES];
	std::vector<int> m_aiResourceQuantityModifier;
	std::vector<bool> m_abNoTrain;

#ifdef LEKMOD_TRAIT_NO_BUILD_IMPROVEMENTS
	std::vector<bool> m_abNoBuild;
#endif
#ifdef LEKMOD_BUILD_TIME_OVERRIDE
	std::vector<int> m_aiBuildTimeOverride;
	std::vector<ResourceClassTypes> m_aiBuildTimeOverrideResourceClassRequired;
	std::multimap<BuildTypes, std::pair<int, ResourceClassTypes>> m_BuildTimeOverrides;
#endif

	FStaticVector<FreeTraitUnit, SAFE_ESTIMATE_NUM_FREE_UNITS, true, c_eCiv5GameplayDLL, 0> m_aFreeTraitUnits;
	std::vector<int> m_aUniqueLuxuryAreas;

	// Maya calendar bonus data
	int m_iBaktunPreviousTurn;  // Saved
	int m_iBaktun;	 // Not saved
	int m_iKatun;    // Not saved
	int m_iTun;      // Not saved
	int m_iWinal;    // Not saved
	int m_iKin;      // Not saved
	std::vector<MayaBonusChoice> m_aMayaBonusChoices;  // Saved

	std::vector<int> m_paiMovesChangeUnitCombat;
	std::vector<int> m_paiMaintenanceModifierUnitCombat;

	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiImprovementYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiSpecialistYieldChange;
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiAnySpecificSpecialistYieldChange;
#endif

	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiUnimprovedFeatureYieldChange;
#if defined(TRAITIFY)
	int m_iRouteMovementChange[NUM_ROUTE_TYPES];
	int m_iPuppetYieldModifiers[NUM_YIELD_TYPES];
	std::vector<bool> m_abRemoveRequiredTerrain;
	std::vector<bool> m_abForceSpawnCapital;
	std::vector<int> m_aiBuildingClassHappiness;
	std::vector<int> m_aiBuildingClassGlobalHappiness;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiBuildingClassYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiFeatureYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiTerrainYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiResourceYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiResourceClassYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiBuildingCostOverride;
#endif

	std::vector<FreeResourceXCities> m_aFreeResourceXCities;
};

#endif //CIV5_TRAIT_CLASSES_H