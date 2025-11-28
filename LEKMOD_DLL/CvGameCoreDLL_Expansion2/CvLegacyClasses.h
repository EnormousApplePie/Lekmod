/*

*/
#pragma once

#ifndef LEK_CIV5_LEGACYCLASSES_H
#define LEK_CIV5_LEGACYCLASSES_H
#if defined(LEKMOD_LEGACY)
// LegacyAI
class CvLegacyAI;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvLegacyEntry
//!  \brief		A single entry in Legacies
//
//!  Key Attributes:
//!  - Populated from XML\GameInfo\CIV5LegacyInfos.xml (if we ever swap to using multiple files again.)
//!  - Array of these contained in CvLegacyXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvLegacyEntry: public CvBaseInfo
{
public:
    CvLegacyEntry(void);
    ~CvLegacyEntry(void);

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

    // These are key functions, very important.
    int GetCivilization() const;
    int GetEraOffered() const;
	bool IsInstant() const;
	// One-Shot Free Units
	bool IncludesOneShotFreeUnits(LegacyTypes eLegacy) const;
	void SetIncludesOneShotFreeUnits(LegacyTypes eLegacy);
	// Accessor Functions
	int GetHappinessPerOriginalCity() const;
	int GetGoldenAgeTurns() const;
	int GetGreatGeneralSiegeBonus() const;
	int GetResistanceTimeReduction() const;
	int GetYieldModCapitalProximity() const;
	int GetPlotGoldCostModifier() const;
	int GetPlotCultureCostModifier() const;
	int GetHappinessFromGreatImprovements() const;
	int GetHappinessFromForeignReligiousMajority() const;
	int GetVotesPerCapital() const;
	int GetInfluenceChangeMajorityReligion() const;
	int GetInfluenceChangeTradeConnection() const;
	int GetPurchasedUnitExtraMoves() const;
	int GetHappinessPerTheme() const;
	bool IsTradeUnplunderable() const;
	bool IsCannotPlunder() const;
	int GetFriendlyCityReligionCombatModifier() const;
	int GetOccupiedCityReligionCombatModifier() const;
	int GetEnemyCityReligionCombatModifier() const;
    // Arrays
	int IsFreePromotion(int i) const;
	int GetPromotionNearbyGeneral(int i) const;
	bool IsFreePromotionUnitType(const int promotionID, const int unitTypeID) const;
	int GetPlotPurchaseYieldReward(int i) const;
	int GetCityYieldChange(int i) const;
	int GetOriginalCityYieldChange(int i) const;
	int GetConqueredCityYieldChange(int i) const;
	int GetCityYieldModifier(int i) const;
	int GetBuildingClassProductionModifier(int i) const;
	int GetBuildingClassHappinessChange(int i) const;
	int GetBuildingClassGlobalHappinessChange(int i) const;
	int GetBuildingClassYieldChange(int i, int j) const;
	int GetBuildingClassYieldModifier(int i, int j) const;
	int GetBuildingClassGreatPersonPointChange(int i, int j) const;
	int GetBuildingClassGreatPersonPointModifier(int i, int j) const;
	int GetUnitResourceRequirementChange(int i, int j) const;
	int GetUnitRangedStrengthChange(int i) const;
	int GetUnitStrengthChange(int i) const;
	int GetResourceYieldChange(int i, int j) const;
	int GetResourceClassYieldChange(int i, int j) const;
	int GetSpecialistYieldChange(int i, int j) const;
	int GetSpecialistHappinessChange(int i) const;
	int GetImprovementYieldChange(int i, int j) const;
	int GetImprovementTourismBonus(int i) const;
	int GetImprovementYieldChangePerXWorldWonder(int i, int j) const;
	int GetImprovementNearbyHealChangeByDomain(int i, int j) const;
	int GetImprovementNearbyCombatModifierByDomain(int i, int j) const;
	int GetGreatWorkClassYieldChange(int i, int j) const;
	int GetGreatWorkClassTourismChange(int i) const;
	bool IsNoTrainUnit(UnitTypes eUnit) const;
	bool IsNoConstructBuilding(BuildingTypes eBuilding) const;
	int GetLegacyUnitClassOverride(int i) const;
	int GetLegacyBuildingClassOverride(int i) const;
	bool IsUnitIgnoreTechPrereq(UnitTypes eUnit) const;
	bool IsUnitIgnorePolicyPrereq(UnitTypes eUnit) const;
	bool IsBuildingIgnoreTechPrereq(BuildingTypes eBuilding) const;
	bool IsBuildingIgnorePolicyPrereq(BuildingTypes eBuilding) const;
	bool IsRevealResource(ResourceTypes eResource) const;
	int GetUnitCostOverride(int i, int j) const;
	int GetBuildingCostOverride(int i, int j) const;
	int GetNumFreeUnitsByClass(int i) const;
	int GetNumFreeUnitsByType(int i) const;
	int GetBuildTimeOverride(int i) const;
	int GetYieldBonusFromThemes(int i) const;
private:
    int m_iCivilization;
    int m_iEraOffered;
	bool m_bInstant;
	int m_iHappinessPerOriginalCity;
	int m_iGoldenAgeTurns;
	int m_iGreatGeneralSiegeBonus;
	int m_iResistanceTimeReduction;
	int m_iYieldModCapitalProximity;
	int m_iPlotGoldCostModifier;
	int m_iPlotCultureCostModifier;
	int m_iHappinessFromGreatImprovements;
	int m_iHappinessFromForeignReligiousMajority;
	int m_iVotesPerCapital;
	int m_iInfluenceChangeMajorityReligion;
	int m_iInfluenceChangeTradeConnection;
	int m_iPurchasedUnitExtraMoves;
	int m_iHappinessPerTheme;
	bool m_bTradeUnplunderable;
	bool m_bCannotPlunder;
	int m_iFriendlyCityReligionCombatModifier;
	int m_iOccupiedCityReligionCombatModifier;
	int m_iEnemyCityReligionCombatModifier;
    //Arrays
	// Weirdos
	std::multimap<int, int> m_FreePromotionUnitType;
	// 1D
	bool* m_pbHasOneShotUnits;
	bool* m_pbFreePromotion;
	int* m_piPlotPurchaseYieldReward;
	int* m_piCityYieldChange;
	int* m_piOriginalCityYieldChange;
	int* m_piConqueredCityYieldChange;
	int* m_piCityYieldModifier;
	int* m_piBuildingClassProductionModifier;
	int* m_piBuildingClassHappinessChange;
	int* m_piBuildingClassGlobalHappinessChange;
	int* m_piUnitRangedStrengthChange;
	int* m_piUnitStrengthChange;
	int* m_piSpecialistHappinessChange;
	int* m_piLegacyUnitClassOverride;
	int* m_piLegacyBuildingClassOverride;
	int* m_piPromotionNearbyGeneralUnitCombat;
	int* m_piGreatWorkClassTourismChange;
	int* m_piImprovementTourismBonus;
	int* m_piNumFreeUnitsByClass;
	int* m_piNumFreeUnitsByType;
	int* m_piBuildTimeOverride;
	int* m_piYieldBonusFromThemes;
	std::vector<bool> m_pbUnitIgnoreTechPrereq;
	std::vector<bool> m_pbUnitIgnorePolicyPrereq;
	std::vector<bool> m_pbBuildingIgnoreTechPrereq;
	std::vector<bool> m_pbBuildingIgnorePolicyPrereq;
	std::vector<bool> m_abNoTrainUnit;
	std::vector<bool> m_abNoConstructBuilding;
	std::vector<bool> m_abRevealResource;
	// 2D
	int** m_paiBuildingClassYieldChange;
	int** m_paiBuildingClassYieldModifier;
	int** m_paiBuildingClassGreatPersonPointChange;
	int** m_paiBuildingClassGreatPersonPointModifier;
	int** m_paiResourceYieldChange;
	int** m_paiResourceClassYieldChange;
	int** m_paiUnitResourceRequirementChange;
	int** m_paiSpecialistYieldChange;
	int** m_paiImprovementYieldChange;
	int** m_paiImprovementYieldChangePerXWorldWonder;
	int** m_paiImprovementNearbyHealChangeByDomain;
	int** m_paiImprovementNearbyCombatModifierByDomain;
	int** m_paiGreatWorkClassYieldChange;
	int** m_paiUnitCostOverride;
	int** m_paiBuildingCostOverride;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvLegacyXMLEntries
//!  \brief		Game-wide information about the Legacies
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\GameInfo\CIV5LegacyInfos.xml if we ever use separate files again
//! - Contains an array of CvLegacyEntry from the above XML file
//! - One instance for the entire game
//! - Accessed heavily by CvPlayerLegacies class (which stores the legacy state for 1 player)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvLegacyXMLEntries
{
public:
    CvLegacyXMLEntries(void);
    ~CvLegacyXMLEntries(void);

	std::vector<CvLegacyEntry*>& GetLegacyEntries();
    // Legacy Functions
	int GetNumLegacies();
	CvLegacyEntry* GetLegacyEntry(int iIndex);
    void DeleteLegacyArray();
private:
	std::vector<CvLegacyEntry*> m_paLegacyEntries;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPlayerLegacies
//!  \brief		Information about the legacies of a single player
//
//!  Key Attributes:
//!  - Plan is it will be contained in CvPlayerState object within CvPlayer class
//!  - One instance for each civ (player or AI)
//!  - Accessed by any class that needs to check legacy state
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPlayerLegacies
{
public:
    CvPlayerLegacies(void);
    ~CvPlayerLegacies(void);
    // Initialization, Cleanup and Save/Load
    void Init(CvLegacyXMLEntries* pLegacies, CvPlayer* pPlayer);
    void Uninit();
    void Reset();
    void Read(FDataStream& kStream);
    void Write(FDataStream& kStream) const;

    CvPlayer* GetPlayer() const;

	bool HasLegacy(LegacyTypes eIndex) const;
    void SetLegacy(LegacyTypes eIndex, bool bNewValue);
    CvLegacyXMLEntries* GetLegacies() const;
	std::vector<LegacyTypes> GetCivLegacies() const;
	std::vector<LegacyTypes> GetEraLegacies(EraTypes eEra) const;
	bool CanChooseLegacy(LegacyTypes eIndex) const;
    // Activation Time!
    void testLegacyNotification(EraTypes eEra) const;
    void updatePlayerLegacies(LegacyTypes eLegacy);
	bool HasOneShotFreeUnitsFired(LegacyTypes eLegacy) const;
	void SetOneShotFreeUnitsFired(LegacyTypes eLegacy, bool bFired);
    // Do AI for legacy choices. will likely be random until the feature settles.
    void DoLegacyAI();
    void DoChooseLegacy();
    // Accessors 
	int GetHappinessPerOriginalCity() const;
	int GetGoldenAgeTurns() const;
	int GetGreatGeneralSiegeBonus() const;
	int GetResistanceTimeReduction() const;
	int GetYieldModCapitalProximity() const;
	int GetPlotGoldCostModifier() const;
	int GetPlotCultureCostModifier() const;
	int GetHappinessFromGreatImprovements() const;
	int GetHappinessFromForeignReligiousMajority() const;
	int GetVotesPerCapital() const;
	int GetInfluenceChangeMajorityReligion() const;
	int GetInfluenceChangeTradeConnection() const;
	int GetPurchasedUnitExtraMoves() const;
	int GetHappinessPerTheme() const;
	bool IsTradeUnplunderable() const;
	bool IsCannotPlunder() const;
	int GetFriendlyCityReligionCombatModifier() const;
	int GetOccupiedCityReligionCombatModifier() const;
	int GetEnemyCityReligionCombatModifier() const;
	// Arrays
	bool HasFreePromotionUnitType(PromotionTypes ePromotion, UnitTypes eUnitType) const;
	int GetPromotionNearbyGeneralUnitCombat(UnitCombatTypes eUnitCombat) const;
	int GetPlotPurchaseYieldReward(YieldTypes eYield) const;
	int GetCityYieldChange(YieldTypes eYield) const;
	int GetOriginalCityYieldChange(YieldTypes eYield) const;
	int GetConqueredCityYieldChange(YieldTypes eYield) const;
	int GetCityYieldModifier(YieldTypes eYield) const;
	int GetUnitResourceRequirementChange(UnitTypes eUnit, ResourceTypes eResource) const;
	int GetUnitRangedStrengthChange(UnitTypes eUnit) const;
	int GetUnitStrengthChange(UnitTypes eUnit) const;
	int GetBuildingClassProductionModifier(BuildingClassTypes eBuildingClass) const;
	int GetBuildingClassHappinessChange(BuildingClassTypes eBuildingClass) const;
	int GetBuildingClassGlobalHappinessChange(BuildingClassTypes eBuildingClass) const;
	int GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;
	int GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYield) const;
	int GetBuildingClassGreatPersonPointChange(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const;
	int GetBuildingClassGreatPersonPointModifier(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const;
	int GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYield) const;
	int GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYield) const;
	int GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const;
	int GetSpecialistHappinessChange(SpecialistTypes eSpecialist) const;
	int GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const;
	int GetImprovementTourismBonus(ImprovementTypes eImprovement) const;
	int GetImprovementYieldChangePerXWorldWonder(ImprovementTypes eImprovement, YieldTypes eYield) const;
	int GetNearbyImprovementHealChangeByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const;
	int GetNearbyImprovementCombatModifierByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const;
	int GetGreatWorkClassYieldChange(GreatWorkClass eGreatWorkClass, YieldTypes eYield) const;
	int GetGreatWorkClassTourismChange(GreatWorkClass eGreatWorkClass) const;
	bool NoTrainUnit(UnitTypes eUnit) const;
	bool NoConstructBuilding(BuildingTypes eBuilding) const;
	int GetLegacyUnitClassOverride(UnitClassTypes eUnitClass) const;
	int GetLegacyBuildingClassOverride(BuildingClassTypes eBuildingClass) const;
	bool IsUnitIgnoreTechPrereq(UnitTypes eUnit) const;
	bool IsUnitIgnorePolicyPrereq(UnitTypes eUnit) const;
	bool IsBuildingIgnoreTechPrereq(BuildingTypes eBuilding) const;
	bool IsBuildingIgnorePolicyPrereq(BuildingTypes eBuilding) const;
	bool IsRevealResource(ResourceTypes eResource) const;
	int GetUnitCostOverride(UnitTypes eUnit, YieldTypes eYield) const;
	int GetBuildingCostOverride(BuildingTypes eBuilding, YieldTypes eYield) const;
	int GetBuildTimeOverride(BuildTypes eBuild) const;
	int GetYieldBonusFromThemes(YieldTypes eYield) const;
private:
    bool* m_pabHasLegacy;
	bool* m_pabOneShotFreeUnitsFired;
	CvLegacyXMLEntries* m_pLegacies;
    CvLegacyAI* m_pLegacyAI;
	CvPlayer* m_pPlayer;
    std::vector<bool> m_vCivHasLegacy; 
	std::vector<LegacyTypes> m_vPotentiallyActiveCivLegacies; 

	// Gameplay Effects
	int m_iHappinessPerOriginalCity;
	int m_iGoldenAgeTurns;
	int m_iGreatGeneralSiegeBonus;
	int m_iResistanceTimeReduction;
	int m_iYieldModCapitalProximity;
	int m_iPlotGoldCostModifier;
	int m_iPlotCultureCostModifier;
	int m_iHappinessFromGreatImprovements;
	int m_iHappinessFromForeignReligiousMajority;
	int m_iVotesPerCapital;
	int m_iInfluenceChangeMajorityReligion;
	int m_iInfluenceChangeTradeConnection;
	int m_iPurchasedUnitExtraMoves;
	int m_iHappinessPerTheme;
	bool m_bTradeUnplunderable;
	bool m_bCannotPlunder;
	int m_iFriendlyCityReligionCombatModifier;
	int m_iOccupiedCityReligionCombatModifier;
	int m_iEnemyCityReligionCombatModifier;
	//Arrays
	std::vector<bool> m_vbNoTrain;
	std::vector<bool> m_vbNoConstruct;
	std::vector<bool> m_vbUnitIgnoreTechPrereq;
	std::vector<bool> m_vbUnitIgnorePolicyPrereq;
	std::vector<bool> m_vbBuildingIgnoreTechPrereq;
	std::vector<bool> m_vbBuildingIgnorePolicyPrereq;
	std::vector<bool> m_vbRevealResource;
	std::vector <int> m_viPlotPurchaseYieldReward;
	std::vector <int> m_viCityYieldChange;
	std::vector <int> m_viOriginalCityYieldChange;
	std::vector <int> m_viConqueredCityYieldChange;
	std::vector <int> m_viCityYieldModifier;
	std::vector <int> m_viBuildingClassProductionModifiers;
	std::vector <int> m_viBuildingClassHappinessChanges;
	std::vector <int> m_viBuildingClassGlobalHappinessChanges;
	std::vector <int> m_viUnitRangedStrengthChanges;
	std::vector <int> m_viUnitStrengthChanges;
	std::vector <int> m_viLegacyUnitClassOverrides;
	std::vector <int> m_viLegacyBuildingClassOverrides;
	std::vector <int> m_viSpecialistHappinessChanges;
	std::vector <int> m_viPromotionNearbyGeneralUnitCombat;
	std::vector <int> m_viGreatWorkClassTourismChanges;
	std::vector <int> m_viImprovementTourismBonuses;
	std::vector <int> m_viBuildTimeOverrides;
	std::vector <int> m_viYieldBonusFromThemes;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiUnitCostOverrides;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiBuildingCostOverrides;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiGreatWorkClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiBuildingClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiBuildingClassYieldModifiers;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiResourceYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiResourceClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiSpecialistYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiImprovementYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiImprovementYieldChangePerXWorldWonder;
	std::vector< Firaxis::Array<int, NUM_DOMAIN_TYPES > > m_vaaiNearbyImprovementHealChangeByDomain;
	std::vector< Firaxis::Array<int, NUM_DOMAIN_TYPES > > m_vaaiNearbyImprovementCombatModifierByDomain;
#if defined(TRADE_REFACTOR) // These are reliant on the Trade Refactor.
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiTradeConnectionLandYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiTradeConnectionSeaYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiIncomingTradeConnectionLandYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiIncomingTradeConnectionSeaYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiTradeConnectionLandYieldModifier;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_ppaaiTradeConnectionSeaYieldModifier;
#endif
};
#endif //defined(LEKMOD_LEGACY)
#endif //LEK_CIV5_LEGACYCLASSES_H
