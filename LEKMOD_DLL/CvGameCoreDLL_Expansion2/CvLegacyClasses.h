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

    // Accessor Functions
    int GetCivilization() const;
    int GetEraOffered() const;

	int GetHappinessPerOriginalCity() const;
	int GetGoldenAgeTurns() const;
	int GetGreatGeneralSiegeBonus() const;
	int GetResistanceTimeReduction() const;
	int GetYieldModCapitalProximity() const;
	int GetPlotGoldCostModifier() const;
	int GetPlotCultureCostModifier() const;
    // Arrays
	int IsFreePromotion(int i) const;
	int GetPromotionNearbyGeneral(int i) const;
	bool IsFreePromotionUnitType(const int promotionID, const int unitTypeID) const;
	int GetCityYieldChange(int i) const;
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
	int GetImprovementYieldChangePerXWorldWonder(int i, int j) const;
	int GetImprovementNearbyHealChangeByDomain(int i, int j) const;
	int GetGreatWorkClassYieldChange(int i, int j) const;
private:
    int m_iCivilization;
    int m_iEraOffered;
	int m_iHappinessPerOriginalCity;
	int m_iGoldenAgeTurns;
	int m_iGreatGeneralSiegeBonus;
	int m_iResistanceTimeReduction;
	int m_iYieldModCapitalProximity;
	int m_iPlotGoldCostModifier;
	int m_iPlotCultureCostModifier;
    //Arrays
	bool* m_pbFreePromotion;
	int* m_piPromotionNearbyGeneral;
	std::multimap<int, int> m_FreePromotionUnitType;
	int* m_piCityYieldChange;
	int* m_piCityYieldModifier;
	int* m_piBuildingClassProductionModifier;
	int* m_piBuildingClassHappinessChange;
	int* m_piBuildingClassGlobalHappinessChange;
	int** m_paiBuildingClassYieldChange;
	int** m_paiBuildingClassYieldModifier;
	int** m_paiBuildingClassGreatPersonPointChange;
	int** m_paiBuildingClassGreatPersonPointModifier;
	int** m_paiResourceYieldChange;
	int** m_paiResourceClassYieldChange;
	int** m_paiUnitResourceRequirementChange;
	int* m_piUnitRangedStrengthChange;
	int* m_piUnitStrengthChange;
	int** m_paiSpecialistYieldChange;
	int* m_piSpecialistHappinessChange;
	int** m_paiImprovementYieldChange;
	int** m_paiImprovementYieldChangePerXWorldWonder;
	int** m_paiImprovementNearbyHealChangeByDomain;
	int** m_paiGreatWorkClassYieldChange;
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
	std::vector<LegacyTypes> GetPotentialCivLegacies() const;
	bool CanChooseLegacy(LegacyTypes eIndex) const;
    // Activation Time!
    void testLegacyNotification() const;
    void updatePlayerLegacies(LegacyTypes eLegacy);
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
	// Arrays
	bool HasFreePromotionUnitType(PromotionTypes ePromotion, UnitTypes eUnitType) const;
	int GetPromotionNearbyGeneral(PromotionTypes ePromotion) const;
	int GetCityYieldChange(YieldTypes eYield) const;
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
	int GetImprovementYieldChangePerXWorldWonder(ImprovementTypes eImprovement, YieldTypes eYield) const;
	int GetNearbyImprovementHealChangeByDomain(ImprovementTypes eImprovement, DomainTypes eDomain) const;
	int GetGreatWorkClassYieldChange(GreatWorkClass eGreatWorkClass, YieldTypes eYield) const;
private:
    bool* m_pabHasLegacy;
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
	//Arrays
	std::vector <int> m_viCityYieldChange;
	std::vector <int> m_viCityYieldModifier;
	std::vector <int> m_viBuildingClassProductionModifiers;
	std::vector <int> m_viBuildingClassHappinessChanges;
	std::vector <int> m_viBuildingClassGlobalHappinessChanges;
	std::vector <int> m_viUnitRangedStrengthChanges;
	std::vector <int> m_viUnitStrengthChanges;
	std::vector <int> m_viPromotionNearbyGeneral;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiGreatWorkClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiBuildingClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiBuildingClassYieldModifiers;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiResourceYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiResourceClassYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiSpecialistYieldChanges;
	std::vector <int> m_viSpecialistHappinessChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiImprovementYieldChanges;
	std::vector< Firaxis::Array<int, NUM_YIELD_TYPES > > m_vaaiImprovementYieldChangePerXWorldWonder;
	std::vector< Firaxis::Array<int, NUM_DOMAIN_TYPES > > m_vaaiNearbyImprovementHealChangeByDomain;
};
#endif
#endif //LEK_CIV5_LEGACYCLASSES_H
