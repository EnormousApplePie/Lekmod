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
	int GetCivilization() const { return m_iCivilization; }
    int GetEraOffered() const { return m_iEraOffered; }
	bool IsInstant() const { return m_bInstant; }
	// One-Shot Free Units
	bool IncludesOneShotFreeUnits(LegacyTypes eLegacy) const { return m_pbHasOneShotUnits ? m_pbHasOneShotUnits[eLegacy] : false; }
	void SetIncludesOneShotFreeUnits(LegacyTypes eLegacy);
	// Accessor Functions
	int GetHappinessPerOriginalCity() const { return m_iHappinessPerOriginalCity; }
	int GetGoldenAgeTurns() const { return m_iGoldenAgeTurns; }
	int GetGreatGeneralSiegeBonus() const { return m_iGreatGeneralSiegeBonus; }
	int GetResistanceTimeReduction() const { return m_iResistanceTimeReduction; }
	int GetYieldModCapitalProximity() const { return m_iYieldModCapitalProximity; }
	int GetPlotGoldCostModifier() const { return m_iPlotGoldCostModifier; }
	int GetPlotCultureCostModifier() const { return m_iPlotCultureCostModifier; }
	int GetHappinessFromGreatImprovements() const { return m_iHappinessFromGreatImprovements; }
	int GetHappinessFromForeignReligiousMajority() const { return m_iHappinessFromForeignReligiousMajority; }
	int GetVotesPerCapital() const { return m_iVotesPerCapital; }
	int GetInfluenceChangeMajorityReligion() const { return m_iInfluenceChangeMajorityReligion; }
	int GetInfluenceChangeTradeConnection() const { return m_iInfluenceChangeTradeConnection; }
	int GetPurchasedUnitExtraMoves() const { return m_iPurchasedUnitExtraMoves; }
	int GetHappinessPerTheme() const { return m_iHappinessPerTheme; }
	bool IsTradeUnplunderable() const { return m_bTradeUnplunderable; }
	bool IsCannotPlunder() const { return m_bCannotPlunder; }
	int GetFriendlyCityReligionCombatModifier() const { return m_iFriendlyCityReligionCombatModifier; }
	int GetOccupiedCityReligionCombatModifier() const { return m_iOccupiedCityReligionCombatModifier; }
	int GetEnemyCityReligionCombatModifier() const { return m_iEnemyCityReligionCombatModifier; }
	int GetGreatProphetCostModifier() const { return m_iGreatProphetCostModifier; }
	int GetHolyCityReligiousPressureModifier() const { return m_iHolyCityReligiousPressureModifier; }
    // Arrays
	int IsFreePromotion(int i) const { return m_pbFreePromotion ? m_pbFreePromotion[i] : false; }
	int GetPromotionNearbyGeneral(int i) const { return m_piPromotionNearbyGeneralUnitCombat ? m_piPromotionNearbyGeneralUnitCombat[i] : -1; }
	bool IsFreePromotionUnitType(const int promotionID, const int unitTypeID) const;
	int GetPlotPurchaseYieldReward(int i) const { return m_piPlotPurchaseYieldReward ? m_piPlotPurchaseYieldReward[i] : 0; }
	int GetCityYieldChange(int i) const { return m_piCityYieldChange ? m_piCityYieldChange[i] : 0; }
	int GetOriginalCityYieldChange(int i) const { return m_piOriginalCityYieldChange ? m_piOriginalCityYieldChange[i] : 0; }
	int GetConqueredCityYieldChange(int i) const { return m_piConqueredCityYieldChange ? m_piConqueredCityYieldChange[i] : 0; }
	int GetCityYieldModifier(int i) const { return m_piCityYieldModifier ? m_piCityYieldModifier[i] : 0; }
	int GetBuildingClassProductionModifier(int i) const { return m_piBuildingClassProductionModifier ? m_piBuildingClassProductionModifier[i] : 0; }
	int GetBuildingClassHappinessChange(int i) const { return m_piBuildingClassHappinessChange ? m_piBuildingClassHappinessChange[i] : 0; }
	int GetBuildingClassGlobalHappinessChange(int i) const { return m_piBuildingClassGlobalHappinessChange ? m_piBuildingClassGlobalHappinessChange[i] : 0; }
	int GetBuildingClassYieldChange(int i, int j) const { return m_paiBuildingClassYieldChange ? m_paiBuildingClassYieldChange[i][j] : 0; }
	int GetBuildingClassYieldModifier(int i, int j) const { return m_paiBuildingClassYieldModifier ? m_paiBuildingClassYieldModifier[i][j] : 0; }
	int GetBuildingClassGreatPersonPointChange(int i, int j) const { return m_paiBuildingClassGreatPersonPointChange ? m_paiBuildingClassGreatPersonPointChange[i][j] : 0; }
	int GetBuildingClassGreatPersonPointModifier(int i, int j) const { return m_paiBuildingClassGreatPersonPointModifier ? m_paiBuildingClassGreatPersonPointModifier[i][j] : 0; }
	int GetUnitResourceRequirementChange(int i, int j) const { return m_paiUnitResourceRequirementChange ? m_paiUnitResourceRequirementChange[i][j] : 0; }
	int GetUnitRangedStrengthChange(int i) const { return m_piUnitRangedStrengthChange ? m_piUnitRangedStrengthChange[i] : 0; }
	int GetUnitStrengthChange(int i) const { return m_piUnitStrengthChange ? m_piUnitStrengthChange[i] : 0; }
	int GetResourceYieldChange(int i, int j) const { return m_paiResourceYieldChange ? m_paiResourceYieldChange[i][j] : 0; }
	int GetResourceClassYieldChange(int i, int j) const { return m_paiResourceClassYieldChange ? m_paiResourceClassYieldChange[i][j] : 0; }
	int GetSpecialistYieldChange(int i, int j) const { return m_paiSpecialistYieldChange ? m_paiSpecialistYieldChange[i][j] : 0; }
	int GetSpecialistHappinessChange(int i) const { return m_piSpecialistHappinessChange ? m_piSpecialistHappinessChange[i] : 0; }
	int GetImprovementYieldChange(int i, int j) const { return m_paiImprovementYieldChange ? m_paiImprovementYieldChange[i][j] : 0; }
	int GetImprovementTourismBonus(int i) const { return m_piImprovementTourismBonus ? m_piImprovementTourismBonus[i] : 0; }
	int GetImprovementYieldChangePerXWorldWonder(int i, int j) const { return m_paiImprovementYieldChangePerXWorldWonder ? m_paiImprovementYieldChangePerXWorldWonder[i][j] : 0; }
	int GetImprovementNearbyHealChangeByDomain(int i, int j) const { return m_paiImprovementNearbyHealChangeByDomain ? m_paiImprovementNearbyHealChangeByDomain[i][j] : 0; }
	int GetImprovementNearbyCombatModifierByDomain(int i, int j) const { return m_paiImprovementNearbyCombatModifierByDomain ? m_paiImprovementNearbyCombatModifierByDomain[i][j] : 0; }
	int GetGreatWorkClassYieldChange(int i, int j) const { return m_paiGreatWorkClassYieldChange ? m_paiGreatWorkClassYieldChange[i][j] : 0; }
	int GetGreatWorkClassTourismChange(int i) const { return m_piGreatWorkClassTourismChange ? m_piGreatWorkClassTourismChange[i] : 0; }
	bool IsNoTrainUnit(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_abNoTrainUnit[eUnit] : false; }
	bool IsNoConstructBuilding(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_abNoConstructBuilding[eBuilding] : false; }
	int GetLegacyUnitClassOverride(int i) const { return m_piLegacyUnitClassOverride ? m_piLegacyUnitClassOverride[i] : -1; }
	int GetLegacyBuildingClassOverride(int i) const { return m_piLegacyBuildingClassOverride ? m_piLegacyBuildingClassOverride[i] : -1; }
	bool IsUnitIgnoreTechPrereq(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_pbUnitIgnoreTechPrereq[eUnit] : false; }
	bool IsUnitIgnorePolicyPrereq(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_pbUnitIgnorePolicyPrereq[eUnit] : false; }
	bool IsBuildingIgnoreTechPrereq(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_pbBuildingIgnoreTechPrereq[eBuilding] : false; }
	bool IsBuildingIgnorePolicyPrereq(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_pbBuildingIgnorePolicyPrereq[eBuilding] : false; }
	bool IsRevealResource(ResourceTypes eResource) const { return NO_RESOURCE != eResource ? m_abRevealResource[eResource] : false; }
	int GetUnitCostOverride(int i, int j) const { return m_paiUnitCostOverride ? m_paiUnitCostOverride[i][j] : 0; }
	int GetBuildingCostOverride(int i, int j) const { return m_paiBuildingCostOverride ? m_paiBuildingCostOverride[i][j] : 0; }
	int GetNumFreeUnitsByClass(int i) const { return m_piNumFreeUnitsByClass ? m_piNumFreeUnitsByClass[i] : 0; }
	int GetNumFreeUnitsByType(int i) const { return m_piNumFreeUnitsByType ? m_piNumFreeUnitsByType[i] : 0; }
	int GetBuildTimeOverride(int i) const { return m_piBuildTimeOverride ? m_piBuildTimeOverride[i] : 0; }
	int GetYieldBonusFromThemes(int i) const { return m_piYieldBonusFromThemes ? m_piYieldBonusFromThemes[i] : 0; }
#if defined(TRADE_REFACTOR)
	int GetTradeConnectionLandYieldChanges(int i, int j) const { return m_paiTradeConnectionLandYieldChanges ? m_paiTradeConnectionLandYieldChanges[i][j] : 0; }
	int GetTradeConnectionSeaYieldChanges(int i, int j) const { return m_paiTradeConnectionSeaYieldChanges ? m_paiTradeConnectionSeaYieldChanges[i][j] : 0; }
	int GetIncomingTradeConnectionLandYieldChanges(int i, int j) const { return m_paiIncomingTradeConnectionLandYieldChanges ? m_paiIncomingTradeConnectionLandYieldChanges[i][j] : 0; }
	int GetIncomingTradeConnectionSeaYieldChanges(int i, int j) const { return m_paiIncomingTradeConnectionSeaYieldChanges ? m_paiIncomingTradeConnectionSeaYieldChanges[i][j] : 0; }
	int GetTradeConnectionLandYieldModifier(int i, int j) const { return m_paiTradeConnectionLandYieldModifier ? m_paiTradeConnectionLandYieldModifier[i][j] : 0; }
	int GetTradeConnectionSeaYieldModifier(int i, int j) const { return m_paiTradeConnectionSeaYieldModifier ? m_paiTradeConnectionSeaYieldModifier[i][j] : 0; }
#endif
	int GetHolyCityYieldChange(int i) const { return m_piHolyCityYieldChange ? m_piHolyCityYieldChange[i] : 0; }
	int GetGreatWorkClassGreatPersonPoint(int i, int j) const { return m_paiGreatWorkClassGreatPersonPoint ? m_paiGreatWorkClassGreatPersonPoint[i][j] : 0; }
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
	int m_iGreatProphetCostModifier;
	int m_iHolyCityReligiousPressureModifier;
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
	int* m_piHolyCityYieldChange;
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
#if defined(TRADE_REFACTOR)
	int** m_paiTradeConnectionLandYieldChanges;
	int** m_paiTradeConnectionSeaYieldChanges;
	int** m_paiIncomingTradeConnectionLandYieldChanges;
	int** m_paiIncomingTradeConnectionSeaYieldChanges;
	int** m_paiTradeConnectionLandYieldModifier;
	int** m_paiTradeConnectionSeaYieldModifier;
#endif
	int** m_paiGreatWorkClassGreatPersonPoint;
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
	CvLegacyXMLEntries(void) {}
	~CvLegacyXMLEntries(void) { DeleteLegacyArray(); }

	std::vector<CvLegacyEntry*>& GetLegacyEntries() { return m_paLegacyEntries; }
    // Legacy Functions
	int GetNumLegacies() { return m_paLegacyEntries.size(); }
	CvLegacyEntry* GetLegacyEntry(int iIndex) { return (iIndex >= 0 && iIndex < m_paLegacyEntries.size()) ? m_paLegacyEntries[iIndex] : NULL; }
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

	CvPlayer* GetPlayer() const { return m_pPlayer; }

	bool HasLegacy(LegacyTypes eIndex) const { return m_pabHasLegacy[eIndex]; }
    void SetLegacy(LegacyTypes eIndex, bool bNewValue);
    CvLegacyXMLEntries* GetLegacies() const { return m_pLegacies; }
	std::vector<LegacyTypes> GetCivLegacies() const;
	std::vector<LegacyTypes> GetEraLegacies(EraTypes eEra) const;
	bool CanChooseLegacy(LegacyTypes eIndex) const;
    // Activation Time!
    void testLegacyNotification(EraTypes eEra) const;
    void updatePlayerLegacies(LegacyTypes eLegacy, int Change);
	bool HasOneShotFreeUnitsFired(LegacyTypes eLegacy) const { return m_pabOneShotFreeUnitsFired[eLegacy]; }
	void SetOneShotFreeUnitsFired(LegacyTypes eLegacy, bool bFired) { m_pabOneShotFreeUnitsFired[eLegacy] = bFired; }
    // Do AI for legacy choices. will likely be random until the feature settles.
    void DoLegacyAI();
    void DoChooseLegacy();
    // Accessors 
	int GetHappinessPerOriginalCity() const { return m_iHappinessPerOriginalCity; }
	int GetGoldenAgeTurns() const { return m_iGoldenAgeTurns; }
	int GetGreatGeneralSiegeBonus() const { return m_iGreatGeneralSiegeBonus; }
	int GetResistanceTimeReduction() const { return m_iResistanceTimeReduction; }
	int GetYieldModCapitalProximity() const { return m_iYieldModCapitalProximity; }
	int GetPlotGoldCostModifier() const	{ return m_iPlotGoldCostModifier; }
	int GetPlotCultureCostModifier() const { return m_iPlotCultureCostModifier; }
	int GetHappinessFromGreatImprovements() const { return m_iHappinessFromGreatImprovements; }
	int GetHappinessFromForeignReligiousMajority() const { return m_iHappinessFromForeignReligiousMajority; }
	int GetVotesPerCapital() const { return m_iVotesPerCapital; }
	int GetInfluenceChangeMajorityReligion() const { return m_iInfluenceChangeMajorityReligion; }
	int GetInfluenceChangeTradeConnection() const { return m_iInfluenceChangeTradeConnection; }
	int GetPurchasedUnitExtraMoves() const { return m_iPurchasedUnitExtraMoves; }
	int GetHappinessPerTheme() const { return m_iHappinessPerTheme; }
	bool IsTradeUnplunderable() const { return m_bTradeUnplunderable; }
	bool IsCannotPlunder() const { return m_bCannotPlunder; }
	int GetFriendlyCityReligionCombatModifier() const { return m_iFriendlyCityReligionCombatModifier; }
	int GetOccupiedCityReligionCombatModifier() const { return m_iOccupiedCityReligionCombatModifier; }
	int GetEnemyCityReligionCombatModifier() const { return m_iEnemyCityReligionCombatModifier; }
	int GetGreatProphetCostModifier() const { return m_iGreatProphetCostModifier; }
	int GetHolyCityReligiousPressureModifier() const { return m_iHolyCityReligiousPressureModifier; }
	// Arrays
	bool HasFreePromotionUnitType(PromotionTypes ePromotion, UnitTypes eUnitType) const;
	int GetGreatWorkClassGreatPersonPoint(GreatWorkClass eGreatWorkClass, SpecialistTypes eSpecialist) const;
	int GetBuildingClassGreatPersonPointChange(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const;
	int GetBuildingClassGreatPersonPointModifier(BuildingClassTypes eBuildingClass, SpecialistTypes eSpecialist) const;
	int GetUnitResourceRequirementChange(UnitTypes eUnit, ResourceTypes eResource) const;

	int GetPromotionNearbyGeneralUnitCombat(UnitCombatTypes eUnitCombat) const { return NO_UNITCOMBAT != eUnitCombat ? m_viPromotionNearbyGeneralUnitCombat[(int)eUnitCombat] : NO_PROMOTION; }
	int GetPlotPurchaseYieldReward(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viPlotPurchaseYieldReward[(int)eYield] : 0; }
	int GetCityYieldChange(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viCityYieldChange[(int)eYield] : 0; }
	int GetOriginalCityYieldChange(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viOriginalCityYieldChange[(int)eYield] : 0; }
	int GetConqueredCityYieldChange(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viConqueredCityYieldChange[(int)eYield] : 0; }
	int GetCityYieldModifier(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viCityYieldModifier[(int)eYield] : 0; }
	int GetUnitRangedStrengthChange(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_viUnitRangedStrengthChanges[(int)eUnit] : 0; }
	int GetUnitStrengthChange(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_viUnitStrengthChanges[(int)eUnit] : 0; }
	int GetBuildingClassProductionModifier(BuildingClassTypes eBuildingClass) const { return NO_BUILDINGCLASS != eBuildingClass ? m_viBuildingClassProductionModifiers[(int)eBuildingClass] : 0; }
	int GetBuildingClassHappinessChange(BuildingClassTypes eBuildingClass) const { return NO_BUILDINGCLASS != eBuildingClass ? m_viBuildingClassHappinessChanges[(int)eBuildingClass] : 0; }
	int GetBuildingClassGlobalHappinessChange(BuildingClassTypes eBuildingClass) const { return NO_BUILDINGCLASS != eBuildingClass ? m_viBuildingClassGlobalHappinessChanges[(int)eBuildingClass] : 0; }
	int GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const { return NO_BUILDINGCLASS != eBuildingClass ? m_vaaiBuildingClassYieldChanges[(int)eBuildingClass][(int)eYield] : 0; }
	int GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYield) const { return NO_BUILDINGCLASS != eBuildingClass ? m_vaaiBuildingClassYieldModifiers[(int)eBuildingClass][(int)eYield] : 0; }
	int GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYield) const { return NO_RESOURCE != eResource ? m_vaaiResourceYieldChanges[(int)eResource][(int)eYield] : 0; }
	int GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYield) const { return NO_RESOURCECLASS != eResourceClass ? m_vaaiResourceClassYieldChanges[(int)eResourceClass][(int)eYield] : 0; }
	int GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const { return NO_SPECIALIST != eSpecialist ? m_vaaiSpecialistYieldChanges[(int)eSpecialist][(int)eYield] : 0; }
	int GetSpecialistHappinessChange(SpecialistTypes eSpecialist) const { return NO_SPECIALIST != eSpecialist ? m_viSpecialistHappinessChanges[(int)eSpecialist] : 0; }
	int GetImprovementYieldChange(ImprovementTypes eImprovement, YieldTypes eYield) const { return NO_IMPROVEMENT != eImprovement ? m_vaaiImprovementYieldChanges[(int)eImprovement][(int)eYield] : 0; }
	int GetImprovementTourismBonus(ImprovementTypes eImprovement) const { return NO_IMPROVEMENT != eImprovement ? m_viImprovementTourismBonuses[(int)eImprovement] : 0; }
	int GetImprovementYieldChangePerXWorldWonder(ImprovementTypes eImprovement, YieldTypes eYield) const { return NO_IMPROVEMENT != eImprovement ? m_vaaiImprovementYieldChangePerXWorldWonder[(int)eImprovement][(int)eYield] : 0; }
	int GetNearbyImprovementHealChangeByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const { return NO_IMPROVEMENT != eImprovement ? m_vaaiNearbyImprovementHealChangeByDomain[(int)eImprovement][(int)eDomain] : 0; }
	int GetNearbyImprovementCombatModifierByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const { return NO_IMPROVEMENT != eImprovement ? m_vaaiNearbyImprovementCombatModifierByDomain[(int)eImprovement][(int)eDomain] : 0; }
	int GetGreatWorkClassYieldChange(GreatWorkClass eGreatWorkClass, YieldTypes eYield) const { return NO_GREAT_WORK_CLASS != eGreatWorkClass ? m_vaaiGreatWorkClassYieldChanges[(int)eGreatWorkClass][(int)eYield] : 0; }
	int GetGreatWorkClassTourismChange(GreatWorkClass eGreatWorkClass) const { return NO_GREAT_WORK_CLASS != eGreatWorkClass ? m_viGreatWorkClassTourismChanges[(int)eGreatWorkClass] : 0; }
	bool NoTrainUnit(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_vbNoTrain[eUnit] : false; }
	bool NoConstructBuilding(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_vbNoConstruct[eBuilding] : false; }
	int GetLegacyUnitClassOverride(UnitClassTypes eUnitClass) const { return NO_UNITCLASS != eUnitClass ? m_viLegacyUnitClassOverrides[(int)eUnitClass] : NO_UNIT; }
	int GetLegacyBuildingClassOverride(BuildingClassTypes eBuildingClass) const { return NO_BUILDINGCLASS != eBuildingClass ? m_viLegacyBuildingClassOverrides[(int)eBuildingClass] : NO_BUILDING; }
	bool IsUnitIgnoreTechPrereq(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_vbUnitIgnoreTechPrereq[eUnit] : false; }
	bool IsUnitIgnorePolicyPrereq(UnitTypes eUnit) const { return NO_UNIT != eUnit ? m_vbUnitIgnorePolicyPrereq[eUnit] : false; }
	bool IsBuildingIgnoreTechPrereq(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_vbBuildingIgnoreTechPrereq[eBuilding] : false; }
	bool IsBuildingIgnorePolicyPrereq(BuildingTypes eBuilding) const { return NO_BUILDING != eBuilding ? m_vbBuildingIgnorePolicyPrereq[eBuilding] : false; }
	bool IsRevealResource(ResourceTypes eResource) const { return NO_RESOURCE != eResource ? m_vbRevealResource[eResource] : false; }
	int GetUnitCostOverride(UnitTypes eUnit, YieldTypes eYield) const { return NO_UNIT != eUnit ? m_vaaiUnitCostOverrides[(int)eUnit][(int)eYield] : 0; }
	int GetBuildingCostOverride(BuildingTypes eBuilding, YieldTypes eYield) const { return NO_BUILDING != eBuilding ? m_vaaiBuildingCostOverrides[(int)eBuilding][(int)eYield] : 0; }
	int GetBuildTimeOverride(BuildTypes eBuild) const { return NO_BUILD != eBuild ? m_viBuildTimeOverrides[(int)eBuild] : 0; }
	int GetYieldBonusFromThemes(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viYieldBonusFromThemes[(int)eYield] : 0; }
#if defined(TRADE_REFACTOR)
	int GetTradeConnectionLandYieldChanges(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiTradeConnectionLandYieldChange[(int)eConnection][(int)eYield] : 0; }
	int GetTradeConnectionSeaYieldChanges(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiTradeConnectionSeaYieldChange[(int)eConnection][(int)eYield] : 0; }
	int GetIncomingTradeConnectionLandYieldChanges(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiIncomingTradeConnectionLandYieldChange[(int)eConnection][(int)eYield] : 0; }
	int GetIncomingTradeConnectionSeaYieldChanges(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiIncomingTradeConnectionSeaYieldChange[(int)eConnection][(int)eYield] : 0; }
	int GetTradeConnectionLandYieldModifier(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiTradeConnectionLandYieldModifier[(int)eConnection][(int)eYield] : 0; }
	int GetTradeConnectionSeaYieldModifier(TradeConnectionType eConnection, YieldTypes eYield) const { return NO_TRADE_CONNECTION != eConnection ? m_vaaiTradeConnectionSeaYieldModifier[(int)eConnection][(int)eYield] : 0; }
#endif
	int GetHolyCityYieldChange(YieldTypes eYield) const { return NO_YIELD != eYield ? m_viHolyCityYieldChange[(int)eYield] : 0; }
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
	int m_iGreatProphetCostModifier;
	int m_iHolyCityReligiousPressureModifier;
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
	std::vector <int> m_viHolyCityYieldChange;
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
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiTradeConnectionLandYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiTradeConnectionSeaYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiIncomingTradeConnectionLandYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiIncomingTradeConnectionSeaYieldChange;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiTradeConnectionLandYieldModifier;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiTradeConnectionSeaYieldModifier;
#endif
};
#endif //defined(LEKMOD_LEGACY)
#endif //LEK_CIV5_LEGACYCLASSES_H
