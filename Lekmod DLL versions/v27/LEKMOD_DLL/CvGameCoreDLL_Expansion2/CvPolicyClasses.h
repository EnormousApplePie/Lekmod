/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once
 
#ifndef CIV5_POLICY_CLASSES_H
#define CIV5_POLICY_CLASSES_H

// Forward definitions
class CvPolicyAI;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPolicyEntry
//!  \brief		A single entry in the policy tree
//
//!  Key Attributes:
//!  - Used to be called CvPolicyInfo
//!  - Populated from XML\GameInfo\CIV5PolicyInfos.xml
//!  - Array of these contained in CvPolicyXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPolicyEntry: public CvBaseInfo
{
public:
	CvPolicyEntry(void);
	~CvPolicyEntry(void);

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	// Accessor Functions (Non-Arrays)
	int GetCultureCost() const;
	int GetGridX() const;
	int GetGridY() const;
	int GetLevel() const;
	int GetPolicyCostModifier() const;
	int GetCulturePerCity() const;
#ifdef FRUITY_TRADITION_ARISTOCRACY
	int GetCapitalCulturePerUniqueLuxury() const;
#endif
	int GetCulturePerWonder() const;
	int GetCultureWonderMultiplier() const;
	int GetCulturePerTechResearched() const;
	int GetCultureImprovementChange() const;
	int GetCultureFromKills() const;
	int GetCultureFromBarbarianKills() const;
	int GetGoldFromKills() const;
	int GetScienceFromKills() const; // NQMP GJS - Honor Finisher
	int GetEmbarkedExtraMoves() const;
	int GetAttackBonusTurns() const;
	int GetGoldenAgeTurns() const;
	int GetGoldenAgeMeterMod() const;
	int GetGoldenAgeDurationMod() const;
	int GetNumFreeTechs() const;
	int GetNumFreePolicies() const;
	int GetNumFreeGreatPeople() const;
#ifdef NQ_EXTRA_SPIES_FROM_POLICIES
	int GetNumExtraSpies() const;
#endif
	int GetMedianTechPercentChange() const;
	int GetStrategicResourceMod() const;
	int GetWonderProductionModifier() const;
	int GetBuildingProductionModifier() const;
	int GetGreatPeopleRateModifier() const;
	int GetGreatGeneralRateModifier() const;
	int GetGreatAdmiralRateModifier() const;
	int GetGreatWriterRateModifier() const;
	int GetGreatArtistRateModifier() const;
	int GetGreatMusicianRateModifier() const;
	int GetGreatMerchantRateModifier() const;
	int GetGreatScientistRateModifier() const;
	int GetDomesticGreatGeneralRateModifier() const;
	int GetExtraHappiness() const;
	int GetExtraHappinessPerCity() const;
	int GetUnhappinessMod() const;
	int GetCityCountUnhappinessMod() const;
	int GetOccupiedPopulationUnhappinessMod() const;
	int GetCapitalUnhappinessMod() const;
	int GetFreeExperience() const;
	int GetWorkerSpeedModifier() const;
	int GetAllFeatureProduction() const;
	int GetImprovementCostModifier() const;
	int GetImprovementUpgradeRateModifier() const;
	int GetSpecialistProductionModifier() const;
	int GetSpecialistUpgradeModifier() const;
	int GetMilitaryProductionModifier() const;
	int GetBaseFreeUnits() const;
	int GetBaseFreeMilitaryUnits() const;
	int GetFreeUnitsPopulationPercent() const;
	int GetFreeMilitaryUnitsPopulationPercent() const;
	int GetHappinessPerGarrisonedUnit() const;
	int GetCulturePerGarrisonedUnit() const;
	int GetHappinessPerTradeRoute() const;
#ifdef NQ_RAIL_CONNECTION_HAPPINESS_FROM_POLICIES
	int GetHappinessPerRailConnection() const;
#endif
	int GetHappinessPerXPopulation() const;
	int GetExtraHappinessPerLuxury() const;
	int GetUnhappinessFromUnitsMod() const;
	int GetNumExtraBuilders() const;
	int GetPlotGoldCostMod() const;
	int GetPlotCultureCostModifier() const;
	int GetPlotCultureExponentModifier() const;
	int GetNumCitiesPolicyCostDiscount() const;
	int GetNumCitiesResearchCostDiscount() const; // NQMP GJS - new Dictatorship of the Proletariat i.e. Communism
	int GetGarrisonedCityRangeStrikeModifier() const;
	int GetUnitPurchaseCostModifier() const;
	int GetBuildingPurchaseCostModifier() const;
	int GetCityConnectionTradeRouteGoldModifier() const;
	int GetTradeMissionGoldModifier() const;
	int GetFaithCostModifier() const;
	int GetCulturalPlunderMultiplier() const;
	int GetStealTechSlowerModifier() const;
	int GetStealTechFasterModifier() const;
	int GetCatchSpiesModifier() const;
	int GetGoldPerUnit() const;
	int GetGoldPerMilitaryUnit() const;
	int GetCityStrengthMod() const;
	int GetCityGrowthMod() const;
	int GetCapitalGrowthMod() const;
	int GetSettlerProductionModifier() const;
	int GetCapitalSettlerProductionModifier() const;
	int GetNewCityExtraPopulation() const;
	int GetFreeFoodBox() const;
	int GetRouteGoldMaintenanceMod() const;
	int GetBuildingGoldMaintenanceMod() const;
	int GetUnitGoldMaintenanceMod() const;
	int GetUnitSupplyMod() const;
	int GetHappyPerMilitaryUnit() const;
	int GetFreeSpecialist() const;
	int GetTechPrereq() const;
	int GetMaxConscript() const;
	int GetExpModifier() const;
	int GetExpInBorderModifier() const;
	int GetMinorQuestFriendshipMod() const;
	int GetMinorGoldFriendshipMod() const;
	int GetMinorFriendshipMinimum() const;
	int GetMinorFriendshipDecayMod() const;
	int GetOtherPlayersMinorFriendshipDecayMod() const;
	int GetCityStateUnitFrequencyModifier() const;
	int GetCommonFoeTourismModifier() const;
	int GetLessHappyTourismModifier() const;
	int GetSharedIdeologyTourismModifier() const;
	int GetLandTradeRouteGoldChange() const;
	int GetSeaTradeRouteGoldChange() const;
	int GetInternalTradeRouteGoldChange() const; // NQMP GJS - Silk Road
	int GetSharedIdeologyTradeGoldChange() const;
	int GetRiggingElectionModifier() const;
	int GetMilitaryUnitGiftExtraInfluence() const;
	int GetProtectedMinorPerTurnInfluence() const;
	int GetAfraidMinorPerTurnInfluence() const;
	int GetMinorBullyScoreModifier() const;
	int GetThemingBonusMultiplier() const;
	int GetInternalTradeRouteYieldModifier() const;
#ifdef FRUITY_TRADITION_LANDED_ELITE
	int GetInternalTradeRouteFoodYieldChange() const;
#endif
#ifdef NQ_INTERNAL_TRADE_ROUTE_PRODUCTION_YIELD_CHANGE_FROM_POLICIES
	int GetInternalTradeRouteProductionYieldChange() const;
#endif
#ifdef NQ_RAIL_CONNECTION_PRODUCTION_MODIFIER_FROM_POLICIES
	int GetRailConnectionProductionModifier() const;
#endif
#ifdef NQ_COMBAT_BONUS_VS_SMALLER_CIV_FROM_POLICIES
	int GetCombatBonusVsSmallerCiv() const;
#endif
#ifdef NQ_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER_FROM_POLICIES
	int GetProductionToGreatMusiciansModifier() const;
#endif
#ifdef NQ_TRADE_MISSION_INFLUENCE_MODIFIER_FROM_POLICIES
	int GetTradeMissionInfluenceModifier() const;
#endif
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN_FROM_POLICIES
	int GetSciencePerGreatPersonBorn() const;
#endif
#ifdef NQ_INFLUENCE_BOOST_PER_GREAT_PERSON_BORN_FROM_POLICIES
	int GetInfluenceBoostPerGreatPersonBorn() const;
#endif
#ifdef NQ_TOURISM_FROM_TRADE_MISSIONS_FROM_POLICIES
	int GetTourismFromTradeMissions() const;
#endif
#ifdef NQ_HAPPINESS_FROM_GREAT_IMPROVEMENTS_FROM_POLICIES
	int GetHappinessFromGreatImprovements() const;
#endif
#ifdef NQ_MINOR_FRIENDSHIP_GAIN_BULLY_GOLD_SUCCESS_FROM_POLICIES
	int GetMinorFriendshipGainBullyGoldSuccess() const;
#endif
#ifdef NQ_COUP_CHANCE_MODIFIER_FROM_POLICIES
	int GetCoupChanceModifier() const;
#endif

	int GetSharedReligionTourismModifier() const;
	int GetTradeRouteTourismModifier() const;
	int GetOpenBordersTourismModifier() const;
	int GetCityStateTradeChange() const;
	bool IsMinorGreatPeopleAllies() const;
	bool IsMinorScienceAllies() const;
	bool IsMinorResourceBonus() const;
	int GetMinorMilitaryNumExtraUnitsToGift() const; // NQMP GJS - Patronage Finisher
	int GetCityStateBonusModifier() const; // NQMP GJS - Patronage Finisher
	int GetExtraTerritoryClaim() const; // NQMP GJS - Colonialism
	int GetExtraTourismPerGreatWork() const; // NQMP GJS - Cultural Exchange
	int GetTourismPerWonder() const; // NQMP GJS - Flourishing of the Arts
#ifdef NQ_TOURISM_PER_CITY
	int GetTourismPerCity() const;
#endif
#ifdef NQ_IDEOLOGY_PRESSURE_UNHAPPINESS_MODIFIER_FROM_POLICIES
	int GetIdeologyPressureUnhappinessModifier() const;
#endif
	int GetProductionFromGarrison() const; // NQMP GJS - Military Caste
	int GetPolicyBranchType() const;
	int GetNumExtraBranches() const;
	int GetHappinessToCulture() const;
	int GetHappinessToScience() const;
#ifdef NQ_GOLD_TO_SCIENCE_FROM_POLICIES
	int GetGoldToScience() const;
#endif
	int GetNumCitiesFreeAestheticsSchools() const; // NQMP GJS - add support for NumCitiesFreeAestheticsSchools
	int GetNumCitiesFreeWalls() const; // NQMP GJS - New Oligarchy add support for NumCitiesFreeWalls
	int GetNumCitiesFreeCultureBuilding() const;
	int GetNumCitiesFreeFoodBuilding() const;
	bool IsHalfSpecialistUnhappiness() const;
	bool IsHalfSpecialistFood() const;
	bool IsMilitaryFoodProduction() const;
	int GetWoundedUnitDamageMod() const;
	int GetUnitUpgradeCostMod() const;
	int GetBarbarianCombatBonus() const;
	bool IsAlwaysSeeBarbCamps() const;
	bool IsRevealAllCapitals() const;
	bool IsGarrisonFreeMaintenance() const;
	bool IsGoldenAgeCultureBonusDisabled() const;
	bool IsSecondReligionPantheon() const;
	bool IsAddReformationBelief() const;
	bool IsEnablesSSPartHurry() const;
#ifdef NQ_DIABLE_RESISTANCE_TIME_VIA_POLICIES
	bool IsDisablesResistanceTime() const;
#endif
#ifdef NQ_PATRIOTIC_WAR
	bool IsDoubleTrainedMilitaryLandUnit() const;
#endif
#ifdef NQ_WAR_HERO
	bool IsWarHero() const;
#endif
#ifdef NQ_IGNORE_PUPPETS_FOR_RESEARCH_COSTS_FROM_POLICIES
	bool IsIgnorePuppetsForResearchCosts() const;
#endif
#ifdef NQ_POLICY_TOGGLE_NO_MINOR_DOW_IF_FRIENDS
	bool IsNoMinorDOWIfFriends() const;
#endif
#ifdef NQ_ALLOW_PUPPET_PURCHASING_FROM_POLICIES
	bool IsAllowPuppetPurchasing() const;
#endif

	bool IsEnablesSSPartPurchase() const;
	bool IsAbleToAnnexCityStates() const;
	std::string pyGetWeLoveTheKing()
	{
		return GetWeLoveTheKing();
	}
	const char* GetWeLoveTheKing();
	void SetWeLoveTheKingKey(const char* szVal);

	// Accessor Functions (Arrays)
	int GetPrereqOrPolicies(int i) const;
	int GetPrereqAndPolicies(int i) const;
	int GetPolicyDisables(int i) const;
	int GetYieldModifier(int i) const;
	int* GetYieldModifierArray() const;
	int GetCityYieldChange(int i) const;
	int* GetCityYieldChangeArray() const;
	int GetCoastalCityYieldChange(int i) const;
	int* GetCoastalCityYieldChangeArray() const;
	int GetCapitalYieldChange(int i) const;
	int* GetCapitalYieldChangeArray() const;
	int GetCapitalYieldPerPopChange(int i) const;
	int* GetCapitalYieldPerPopChangeArray() const;
	int GetCapitalYieldModifier(int i) const;
	int* GetCapitalYieldModifierArray() const;
	int GetGreatWorkYieldChange(int i) const;
	int* GetGreatWorkYieldChangeArray() const;
	int GetSpecialistExtraYield(int i) const;
	int* GetSpecialistExtraYieldArray() const;
	int IsFreePromotion(int i) const;
	bool IsFreePromotionUnitCombat(const int promotionID, const int unitCombatID) const;
	int GetUnitCombatProductionModifiers(int i) const;
	int GetUnitCombatFreeExperiences(int i) const;
	int GetBuildingClassCultureChange(int i) const;
	int GetBuildingClassHappiness(int i) const;
	int GetBuildingClassProductionModifier(int i) const;
	int GetBuildingClassTourismModifier(int i) const;
	int GetNumFreeUnitsByClass(int i) const;
	int GetTourismByUnitClassCreated(int i) const;
	int GetImprovementCultureChanges(int i) const;

	int GetHurryModifier(int i) const;
	bool IsSpecialistValid(int i) const;
	int GetImprovementYieldChanges(int i, int j) const;
	int GetBuildingClassYieldModifiers(int i, int j) const;
	int GetBuildingClassYieldChanges(int i, int j) const;
#ifdef AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS
	int GetBuildingClassFlavorChanges(int i, int j) const;
#endif
	int GetFlavorValue(int i) const;

	bool IsOneShot() const;
	bool IncludesOneShotFreeUnits() const;

	BuildingTypes GetFreeBuildingOnConquest() const;

private:
	int m_iTechPrereq;
	int m_iCultureCost;
	int m_iGridX;
	int m_iGridY;
	int m_iLevel;
	int m_iPolicyCostModifier;
	int m_iCulturePerCity;
#ifdef FRUITY_TRADITION_ARISTOCRACY
	int m_iCapitalCulturePerUniqueLuxury;
#endif
	int m_iCulturePerWonder;
	int m_iCultureWonderMultiplier;
	int m_iCulturePerTechResearched;
	int m_iCultureImprovementChange;
	int m_iCultureFromKills;
	int m_iCultureFromBarbarianKills;
	int m_iGoldFromKills;
	int m_iScienceFromKills; // NQMP GJS - Honor Finisher
	int m_iEmbarkedExtraMoves;
	int m_iAttackBonusTurns;
	int m_iGoldenAgeTurns;
	int m_iGoldenAgeMeterMod;
	int m_iGoldenAgeDurationMod;
	int m_iNumFreeTechs;
	int m_iNumFreePolicies;
	int m_iNumFreeGreatPeople;
#ifdef NQ_EXTRA_SPIES_FROM_POLICIES
	int m_iNumExtraSpies;
#endif
	int m_iMedianTechPercentChange;
	int m_iStrategicResourceMod;
	int m_iWonderProductionModifier;
	int m_iBuildingProductionModifier;
	int m_iGreatPeopleRateModifier;
	int m_iGreatGeneralRateModifier;
	int m_iGreatAdmiralRateModifier;
	int m_iGreatWriterRateModifier;
	int m_iGreatArtistRateModifier;
	int m_iGreatMusicianRateModifier;
	int m_iGreatMerchantRateModifier;
	int m_iGreatScientistRateModifier;
	int m_iDomesticGreatGeneralRateModifier;
	int m_iExtraHappiness;
	int m_iExtraHappinessPerCity;
	int m_iUnhappinessMod;
	int m_iCityCountUnhappinessMod;
	int m_iOccupiedPopulationUnhappinessMod;
	int m_iCapitalUnhappinessMod;
	int m_iFreeExperience;
	int m_iWorkerSpeedModifier;
	int m_iAllFeatureProduction;
	int m_iImprovementCostModifier;
	int m_iImprovementUpgradeRateModifier;
	int m_iSpecialistProductionModifier;
	int m_iSpecialistUpgradeModifier;
	int m_iMilitaryProductionModifier;
	int m_iBaseFreeUnits;
	int m_iBaseFreeMilitaryUnits;
	int m_iFreeUnitsPopulationPercent;
	int m_iFreeMilitaryUnitsPopulationPercent;
	int m_iHappinessPerGarrisonedUnit;
	int m_iCulturePerGarrisonedUnit;
	int m_iHappinessPerTradeRoute;
#ifdef NQ_RAIL_CONNECTION_HAPPINESS_FROM_POLICIES
	int m_iHappinessPerRailConnection;
#endif
	int m_iHappinessPerXPopulation;
	int m_iExtraHappinessPerLuxury;
	int m_iUnhappinessFromUnitsMod;
	int m_iNumExtraBuilders;
	int m_iPlotGoldCostMod;
	int m_iPlotCultureCostModifier;
	int m_iPlotCultureExponentModifier;
	int m_iNumCitiesPolicyCostDiscount;
	int m_iNumCitiesResearchCostDiscount; // NQMP GJS - new Dictatorship of the Proletariat i.e. Communism
	int m_iGarrisonedCityRangeStrikeModifier;
	int m_iUnitPurchaseCostModifier;
	int m_iBuildingPurchaseCostModifier;
	int m_iCityConnectionTradeRouteGoldModifier;
	int m_iTradeMissionGoldModifier;
	int m_iFaithCostModifier;
	int m_iCulturalPlunderMultiplier;
	int m_iStealTechSlowerModifier;
	int m_iStealTechFasterModifier;
	int m_iCatchSpiesModifier;
	int m_iGoldPerUnit;
	int m_iGoldPerMilitaryUnit;
	int m_iCityStrengthMod;
	int m_iCityGrowthMod;
	int m_iCapitalGrowthMod;
	int m_iSettlerProductionModifier;
	int m_iCapitalSettlerProductionModifier;
	int m_iNewCityExtraPopulation;
	int m_iFreeFoodBox;
	int m_iRouteGoldMaintenanceMod;
	int m_iBuildingGoldMaintenanceMod;
	int m_iUnitGoldMaintenanceMod;
	int m_iUnitSupplyMod;
	int m_iHappyPerMilitaryUnit;
	int m_iExpModifier;
	int m_iExpInBorderModifier;
	int m_iMinorQuestFriendshipMod;
	int m_iMinorGoldFriendshipMod;
	int m_iMinorFriendshipMinimum;
	int m_iMinorFriendshipDecayMod;
	int m_iOtherPlayersMinorFriendshipDecayMod;
	int m_iCityStateUnitFrequencyModifier;
	int m_iCommonFoeTourismModifier;
	int m_iLessHappyTourismModifier;
	int m_iSharedIdeologyTourismModifier;
	int m_iLandTradeRouteGoldChange;
	int m_iSeaTradeRouteGoldChange;
	int m_iInternalTradeRouteGoldChange; // NQMP GJS - Silk Road
	int m_iSharedIdeologyTradeGoldChange;
	int m_iRiggingElectionModifier;
	int m_iMilitaryUnitGiftExtraInfluence;
	int m_iProtectedMinorPerTurnInfluence;
	int m_iAfraidMinorPerTurnInfluence;
	int m_iMinorBullyScoreModifier;
	int m_iThemingBonusMultiplier;
	int m_iInternalTradeRouteYieldModifier;
#ifdef FRUITY_TRADITION_LANDED_ELITE
	int m_iInternalTradeRouteFoodYieldChange;
#endif
#ifdef NQ_INTERNAL_TRADE_ROUTE_PRODUCTION_YIELD_CHANGE_FROM_POLICIES
	int m_iInternalTradeRouteProductionYieldChange;
#endif
#ifdef NQ_RAIL_CONNECTION_PRODUCTION_MODIFIER_FROM_POLICIES
	int m_iRailConnectionProductionModifier;
#endif
#ifdef NQ_COMBAT_BONUS_VS_SMALLER_CIV_FROM_POLICIES
	int m_iCombatBonusVsSmallerCiv;
#endif
#ifdef NQ_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER_FROM_POLICIES
	int m_iProductionToGreatMusiciansModifier;
#endif
#ifdef NQ_TRADE_MISSION_INFLUENCE_MODIFIER_FROM_POLICIES
	int m_iTradeMissionInfluenceModifier;
#endif
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN_FROM_POLICIES
	int m_iSciencePerGreatPersonBorn;
#endif
#ifdef NQ_INFLUENCE_BOOST_PER_GREAT_PERSON_BORN_FROM_POLICIES
	int m_iInfluenceBoostPerGreatPersonBorn;
#endif
#ifdef NQ_TOURISM_FROM_TRADE_MISSIONS_FROM_POLICIES
	int m_iTourismFromTradeMissions;
#endif
#ifdef NQ_HAPPINESS_FROM_GREAT_IMPROVEMENTS_FROM_POLICIES
	int m_iHappinessFromGreatImprovements;
#endif
#ifdef NQ_MINOR_FRIENDSHIP_GAIN_BULLY_GOLD_SUCCESS_FROM_POLICIES
	int m_iMinorFriendshipGainBullyGoldSuccess;
#endif
#ifdef NQ_COUP_CHANCE_MODIFIER_FROM_POLICIES
	int m_iCoupChanceModifier;
#endif
	int m_iSharedReligionTourismModifier;
	int m_iTradeRouteTourismModifier;
	int m_iOpenBordersTourismModifier;
	int m_iCityStateTradeChange;
	bool m_bMinorGreatPeopleAllies;
	bool m_bMinorScienceAllies;
	bool m_bMinorResourceBonus;
	int m_iMinorMilitaryNumExtraUnitsToGift; // NQMP GJS - Patronage Finisher
	int m_iCityStateBonusModifier; // NQMP GJS - Patronage Finisher
	int m_iExtraTerritoryClaim; // NQMP GJS - Colonialism
	int m_iExtraTourismPerGreatWork; // NQMP GJS - Cultural Exchange
	int m_iTourismPerWonder; // NQMP GJS - Flourishing of the Arts
#ifdef NQ_TOURISM_PER_CITY
	int m_iTourismPerCity;
#endif
#ifdef NQ_IDEOLOGY_PRESSURE_UNHAPPINESS_MODIFIER_FROM_POLICIES
	int m_iIdeologyPressureUnhappinessModifier;
#endif

	int m_iProductionFromGarrison; // NQMP GJS - Military Caste
	int m_iFreeSpecialist;
	int m_iMaxConscript;
	int m_iPolicyBranchType;
	int m_iNumExtraBranches;
	int m_iWoundedUnitDamageMod;
	int m_iUnitUpgradeCostMod;
	int m_iBarbarianCombatBonus;
	int m_iHappinessToCulture;
	int m_iHappinessToScience;
#ifdef NQ_GOLD_TO_SCIENCE_FROM_POLICIES
	int m_iGoldToScience;
#endif
	int m_iNumCitiesFreeAestheticsSchools; // NQMP GJS - add support for NumCitiesFreeAestheticsSchools
	int m_iNumCitiesFreeWalls; // NQMP GJS - New Oligarchy add support for NumCitiesFreeWalls
	int m_iNumCitiesFreeCultureBuilding;
	int m_iNumCitiesFreeFoodBuilding;

	bool m_bHalfSpecialistUnhappiness;
	bool m_bHalfSpecialistFood;
	bool m_bMilitaryFoodProduction;
	bool m_bAlwaysSeeBarbCamps;
	bool m_bRevealAllCapitals;
	bool m_bGarrisonFreeMaintenance;
	bool m_bGoldenAgeCultureBonusDisabled;
	bool m_bSecondReligionPantheon;
	bool m_bAddReformationBelief;
	bool m_bEnablesSSPartHurry;
#ifdef NQ_DIABLE_RESISTANCE_TIME_VIA_POLICIES
	bool m_bDisablesResistanceTime;
#endif
#ifdef NQ_PATRIOTIC_WAR
	bool m_bDoubleTrainedMilitaryLandUnit;
#endif
#ifdef NQ_WAR_HERO
	bool m_bWarHero;
#endif
#ifdef NQ_IGNORE_PUPPETS_FOR_RESEARCH_COSTS_FROM_POLICIES
	bool m_bIgnorePuppetsForResearchCosts;
#endif
#ifdef NQ_POLICY_TOGGLE_NO_MINOR_DOW_IF_FRIENDS
	bool m_bNoMinorDOWIfFriends;
#endif
#ifdef NQ_ALLOW_PUPPET_PURCHASING_FROM_POLICIES
	bool m_bAllowPuppetPurchasing;
#endif
	bool m_bEnablesSSPartPurchase;
	bool m_bAbleToAnnexCityStates;

	bool m_bOneShot;
	bool m_bIncludesOneShotFreeUnits;

	CvString m_strWeLoveTheKingKey;
	CvString m_wstrWeLoveTheKing;

	BuildingTypes m_eFreeBuildingOnConquest;

	// Arrays
	std::multimap<int, int> m_FreePromotionUnitCombats;
	int* m_piPrereqOrPolicies;
	int* m_piPrereqAndPolicies;
	int* m_piPolicyDisables;
	int* m_piYieldModifier;
	int* m_piCityYieldChange;
	int* m_piCoastalCityYieldChange;
	int* m_piCapitalYieldChange;
	int* m_piCapitalYieldPerPopChange;
	int* m_piCapitalYieldModifier;
	int* m_piGreatWorkYieldChange;
	int* m_piSpecialistExtraYield;
	int* m_piImprovementCultureChange;
	bool* m_pabFreePromotion;
	int* m_paiUnitCombatProductionModifiers;
	int* m_paiUnitCombatFreeExperiences;
	int* m_paiHurryModifier;
	int* m_paiBuildingClassCultureChanges;
	int* m_paiBuildingClassProductionModifiers;
	int* m_paiBuildingClassTourismModifiers;
	int* m_paiBuildingClassHappiness;
	int* m_paiFreeUnitClasses;
	int* m_paiTourismOnUnitCreation;

//	bool* m_pabHurry;
	bool* m_pabSpecialistValid;
#ifdef AUI_DATABASE_UTILITY_PROPER_2D_ALLOCATION_AND_DESTRUCTION
	std::pair<int**, size_t> m_ppiImprovementYieldChanges;
#ifdef AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS
	std::pair<int**, size_t> m_ppiBuildingClassFlavorChanges;
#endif
	std::pair<int**, size_t> m_ppiBuildingClassYieldModifiers;
	std::pair<int**, size_t> m_ppiBuildingClassYieldChanges;
#else
	int** m_ppiImprovementYieldChanges;
#ifdef AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS
	int** m_ppiBuildingClassFlavorChanges;
#endif
	int** m_ppiBuildingClassYieldModifiers;
	int** m_ppiBuildingClassYieldChanges;
#endif
	int* m_piFlavorValue;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPolicyBranchEntry
//!  \brief		A branch that encompasses Policies
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPolicyBranchEntry: public CvBaseInfo
{
public:
	CvPolicyBranchEntry(void);
	~CvPolicyBranchEntry(void);

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	// Accessor Functions (Non-Arrays)
	int GetEraPrereq() const;
	int GetFreePolicy() const;
	int GetFreeFinishingPolicy() const;
	int GetFirstAdopterFreePolicies() const;
	int GetSecondAdopterFreePolicies() const;
	bool IsPurchaseByLevel() const;
	bool IsLockedWithoutReligion() const;
	bool IsMutuallyExclusive() const;
	bool IsDelayWhenNoReligion() const;
	bool IsDelayWhenNoCulture() const;
	bool IsDelayWhenNoCityStates() const;
	bool IsDelayWhenNoScience() const;

	// Accessor Functions (Arrays)
	int GetPolicyBranchDisables(int i) const;

private:
	int m_iEraPrereq;
	int m_iFreePolicy;
	int m_iFreeFinishingPolicy;
	int m_iFirstAdopterFreePolicies;
	int m_iSecondAdopterFreePolicies;
	bool m_bPurchaseByLevel;
	bool m_bLockedWithoutReligion;
	bool m_bMutuallyExclusive;
	bool m_bDelayWhenNoReligion;
	bool m_bDelayWhenNoCulture;
	bool m_bDelayWhenNoCityStates;
	bool m_bDelayWhenNoScience;

	// Arrays
	int* m_piPolicyBranchDisables;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPolicyXMLEntries
//!  \brief		Game-wide information about the policy tree
//
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\GameInfo\CIV5PolicyInfos.xml
//! - Contains an array of CvPolicyEntry from the above XML file
//! - One instance for the entire game
//! - Accessed heavily by CvPlayerPolicies class (which stores the policy state for 1 player)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPolicyXMLEntries
{
public:
	CvPolicyXMLEntries(void);
	~CvPolicyXMLEntries(void);

	// Policy functions
	std::vector<CvPolicyEntry*>& GetPolicyEntries();
#ifdef AUI_WARNING_FIXES
	uint GetNumPolicies() const;
	CvPolicyEntry* GetPolicyEntry(uint index);
#else
	int GetNumPolicies();
	CvPolicyEntry* GetPolicyEntry(int index);
#endif

	void DeletePoliciesArray();

	// Policy Branch functions
	std::vector<CvPolicyBranchEntry*>& GetPolicyBranchEntries();
#ifdef AUI_WARNING_FIXES
	uint GetNumPolicyBranches() const;
	_Ret_maybenull_ CvPolicyBranchEntry* GetPolicyBranchEntry(uint index);
#else
	int GetNumPolicyBranches();
	_Ret_maybenull_ CvPolicyBranchEntry* GetPolicyBranchEntry(int index);
#endif

	void DeletePolicyBranchesArray();

private:
	std::vector<CvPolicyEntry*> m_paPolicyEntries;
	std::vector<CvPolicyBranchEntry*> m_paPolicyBranchEntries;
};

enum PolicyModifierType
{
    POLICYMOD_EXTRA_HAPPINESS = 0,
    POLICYMOD_EXTRA_HAPPINESS_PER_CITY,
    POLICYMOD_GREAT_PERSON_RATE,
    POLICYMOD_GREAT_GENERAL_RATE,
    POLICYMOD_DOMESTIC_GREAT_GENERAL_RATE,
    POLICYMOD_POLICY_COST_MODIFIER,
    POLICYMOD_RELIGION_PRODUCTION_MODIFIER,
    POLICYMOD_WONDER_PRODUCTION_MODIFIER,
    POLICYMOD_BUILDING_PRODUCTION_MODIFIER,
    POLICYMOD_FREE_EXPERIENCE,
    POLICYMOD_EXTRA_CULTURE_FROM_IMPROVEMENTS,
    POLICYMOD_CULTURE_FROM_KILLS,
    POLICYMOD_EMBARKED_EXTRA_MOVES,
    POLICYMOD_CULTURE_FROM_BARBARIAN_KILLS,
    POLICYMOD_GOLD_FROM_KILLS,
    POLICYMOD_CULTURE_FROM_GARRISON,
    POLICYMOD_UNIT_FREQUENCY_MODIFIER,
    POLICYMOD_TRADE_MISSION_GOLD_MODIFIER,
    POLICYMOD_FAITH_COST_MODIFIER,
    POLICYMOD_CULTURAL_PLUNDER_MULTIPLIER,
    POLICYMOD_STEAL_TECH_SLOWER_MODIFIER,
    POLICYMOD_CATCH_SPIES_MODIFIER,
	POLICYMOD_GREAT_ADMIRAL_RATE,
	POLICYMOD_GREAT_WRITER_RATE,
	POLICYMOD_GREAT_ARTIST_RATE,
	POLICYMOD_GREAT_MUSICIAN_RATE,
	POLICYMOD_GREAT_MERCHANT_RATE,
	POLICYMOD_GREAT_SCIENTIST_RATE,
	POLICYMOD_TOURISM_MOD_COMMON_FOE,
	POLICYMOD_TOURISM_MOD_LESS_HAPPY,
	POLICYMOD_TOURISM_MOD_SHARED_IDEOLOGY,
	POLICYMOD_BUILDING_PURCHASE_COST_MODIFIER,
	POLICYMOD_LAND_TRADE_GOLD_CHANGE,
	POLICYMOD_SEA_TRADE_GOLD_CHANGE,
	POLICYMOD_SHARED_IDEOLOGY_TRADE_CHANGE,
	POLICYMOD_RIGGING_ELECTION_MODIFIER,
	POLICYMOD_MILITARY_UNIT_GIFT_INFLUENCE,
	POLICYMOD_PROTECTED_MINOR_INFLUENCE,
	POLICYMOD_AFRAID_INFLUENCE,
	POLICYMOD_MINOR_BULLY_SCORE_MODIFIER,
	POLICYMOD_STEAL_TECH_FASTER_MODIFIER,
	POLICYMOD_THEMING_BONUS,
	POLICYMOD_CITY_STATE_TRADE_CHANGE,
	POLICYMOD_INTERNAL_TRADE_MODIFIER,
#ifdef FRUITY_TRADITION_LANDED_ELITE
	POLICYMOD_INTERNAL_TRADE_FOOD_YIELD_CHANGE,
#endif
#ifdef NQ_INTERNAL_TRADE_ROUTE_PRODUCTION_YIELD_CHANGE_FROM_POLICIES
	POLICYMOD_INTERNAL_TRADE_PRODUCTION_YIELD_CHANGE,
#endif
#ifdef NQ_RAIL_CONNECTION_PRODUCTION_MODIFIER_FROM_POLICIES
	POLICYMOD_RAIL_CONNECTION_PRODUCTION_MODIFIER,
#endif
#ifdef NQ_COMBAT_BONUS_VS_SMALLER_CIV_FROM_POLICIES
	POLICYMOD_COMBAT_BONUS_VS_SMALLER_CIV,
#endif
#ifdef NQ_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER_FROM_POLICIES
	POLICYMOD_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER,
#endif
#ifdef NQ_TRADE_MISSION_INFLUENCE_MODIFIER_FROM_POLICIES
	POLICYMOD_TRADE_MISSION_INFLUENCE_MODIFIER,
#endif
#ifdef NQ_SCIENCE_PER_GREAT_PERSON_BORN_FROM_POLICIES
	POLICYMOD_SCIENCE_PER_GREAT_PERSON_BORN,
#endif
#ifdef NQ_INFLUENCE_BOOST_PER_GREAT_PERSON_BORN_FROM_POLICIES
	POLICYMOD_INFLUENCE_BOOST_PER_GREAT_PERSON_BORN,
#endif
#ifdef NQ_TOURISM_FROM_TRADE_MISSIONS_FROM_POLICIES
	POLICYMOD_TOURISM_FROM_TRADE_MISSIONS,
#endif
#ifdef NQ_HAPPINESS_FROM_GREAT_IMPROVEMENTS_FROM_POLICIES
	POLICYMOD_HAPPINESS_FROM_GREAT_IMPROVEMENTS,
#endif
#ifdef NQ_MINOR_FRIENDSHIP_GAIN_BULLY_GOLD_SUCCESS_FROM_POLICIES
	POLICYMOD_MINOR_FRIENDSHIP_GAIN_BULLY_GOLD_SUCCESS,
#endif
#ifdef NQ_COUP_CHANCE_MODIFIER_FROM_POLICIES
	POLICYMOD_COUP_CHANCE_MODIFIER,
#endif
    POLICYMOD_SHARED_RELIGION_TOURISM_MODIFIER,
    POLICYMOD_TRADE_ROUTE_TOURISM_MODIFIER,
	POLICYMOD_OPEN_BORDERS_TOURISM_MODIFIER,
	POLICYMOD_SCIENCE_FROM_KILLS, // NQMP GJS - Honor Finisher
	POLICYMOD_MINOR_MILITARY_NUM_EXTRA_UNITS_TO_GIFT, // NQMP GJS - Patronage Finisher
	POLICYMOD_CITY_STATE_BONUS_MODIFIER, // NQMP GJS - Patronage Finisher
	POLICYMOD_EXTRA_TERRITORY_CLAIM, // NQMP GJS - Colonialism
	POLICYMOD_PRODUCTION_FROM_GARRISON, // NQMP GJS - Military Caste
	POLICYMOD_EXTRA_TOURISM_PER_GREAT_WORK, // NQMP GJS - Cultural Exchange
	POLICYMOD_TOURISM_PER_WONDER, // NQMP GJS - Flourishing of the Arts
#ifdef NQ_TOURISM_PER_CITY
	POLICYMOD_TOURISM_PER_CITY,
#endif
#ifdef NQ_IDEOLOGY_PRESSURE_UNHAPPINESS_MODIFIER_FROM_POLICIES
	POLICYMOD_IDEOLOGY_PRESSURE_UNHAPPINESS_MODIFIER,
#endif
	POLICYMOD_INTERNAL_TRADE_GOLD_CHANGE, // NQMP GJS - Silk Road
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPlayerPolicies
//!  \brief		Information about the policies of a single player
//
//!  Key Attributes:
//!  - Plan is it will be contained in CvPlayerState object within CvPlayer class
//!  - One instance for each civ (player or AI)
//!  - Accessed by any class that needs to check policy state
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPlayerPolicies: public CvFlavorRecipient
{
public:
	CvPlayerPolicies(void);
	~CvPlayerPolicies(void);
	void Init(CvPolicyXMLEntries* pPolicies, CvPlayer* pPlayer, bool bIsCity);
	void Uninit();
	void Reset();
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream) const;

	// Flavor recipient required function
	void FlavorUpdate();

	CvPlayer* GetPlayer();

	// Accessor functions
	bool HasPolicy(PolicyTypes eIndex) const;
	void SetPolicy(PolicyTypes eIndex, bool bNewValue);
	int GetNumPoliciesOwned() const;
	int GetNumPoliciesOwnedInBranch(PolicyBranchTypes eBranch) const;
	CvPolicyXMLEntries* GetPolicies() const;

	// Functions to return benefits from policies
	int GetNumericModifier(PolicyModifierType eType);
	int GetYieldModifier(YieldTypes eYieldType);
	int GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYieldType);
	int GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYieldType);
	int GetBuildingClassProductionModifier(BuildingClassTypes eBuildingClass);
	int GetBuildingClassTourismModifier(BuildingClassTypes eBuildingClass);
	int GetImprovementCultureChange(ImprovementTypes eImprovement);
	bool HasPolicyEncouragingGarrisons() const;
	bool HasPolicyGrantingReformationBelief() const;
	CvString GetWeLoveTheKingString();
	std::vector<BuildingTypes> GetFreeBuildingsOnConquest();
	int GetTourismFromUnitCreation(UnitClassTypes eUnitClass) const;

	// Functions to give current player status with respect to policies
	int GetNextPolicyCost();
	bool CanAdoptPolicy(PolicyTypes eIndex, bool bIgnoreCost = false) const;
	int GetNumPoliciesCanBeAdopted();

	// Policy Branch Stuff
	void DoUnlockPolicyBranch(PolicyBranchTypes eBranchType);
	bool CanUnlockPolicyBranch(PolicyBranchTypes eBranchType);

	bool IsPolicyBranchUnlocked(PolicyBranchTypes eBranchType) const;
	void SetPolicyBranchUnlocked(PolicyBranchTypes eBranchType, bool bNewValue, bool bRevolution);
	int GetNumPolicyBranchesUnlocked() const;

	// Blocked branches (because of other branch choices)
	void DoSwitchToPolicyBranch(PolicyBranchTypes eBranchType);
	void SetPolicyBranchBlocked(PolicyBranchTypes eBranchType, bool bValue);
	bool IsPolicyBranchBlocked(PolicyBranchTypes eBranchType) const;
	bool IsPolicyBlocked(PolicyTypes eType) const;

	// Ideology change
	void DoSwitchIdeologies(PolicyBranchTypes eBranchType);
	void ClearPolicyBranch(PolicyBranchTypes eBranchType);

	// Finished branches
	int GetNumPolicyBranchesFinished() const;
	void SetPolicyBranchFinished(PolicyBranchTypes eBranchType, bool bValue);
	bool IsPolicyBranchFinished(PolicyBranchTypes eBranchType) const;
	bool WillFinishBranchIfAdopted(PolicyTypes eType) const;

	PolicyBranchTypes GetPolicyBranchChosen(int iID) const;
	void SetPolicyBranchChosen(int iID, PolicyBranchTypes eBranchType);
	int GetNumPolicyBranchesAllowed() const;

	int GetNumExtraBranches() const;
	void ChangeNumExtraBranches(int iChange);

	// Below is used to determine the "title" for the player
	void DoNewPolicyPickedForHistory(PolicyTypes ePolicy);
	PolicyBranchTypes GetDominantPolicyBranchForTitle() const;

	PolicyBranchTypes GetBranchPicked1() const;
	void SetBranchPicked1(PolicyBranchTypes eBranch);
	PolicyBranchTypes GetBranchPicked2() const;
	void SetBranchPicked2(PolicyBranchTypes eBranch);
	PolicyBranchTypes GetBranchPicked3() const;
	void SetBranchPicked3(PolicyBranchTypes eBranch);

	// functions to deal with one-shot effects
	bool HasOneShotPolicyFired(PolicyTypes eIndex) const;
	void SetOneShotPolicyFired(PolicyTypes eIndex, bool bFired);
	bool HaveOneShotFreeUnitsFired(PolicyTypes eIndex) const;
	void SetOneShotFreeUnitsFired(PolicyTypes eIndex, bool bFired);

	// IDEOLOGY
	PolicyBranchTypes GetLateGamePolicyTree() const;
	bool IsTimeToChooseIdeology() const;
	std::vector<PolicyTypes> GetAvailableTenets(PolicyBranchTypes eBranch, int iLevel);
	PolicyTypes GetTenet(PolicyBranchTypes eBranch, int iLevel, int iIndex);
	int GetNumTenetsOfLevel(PolicyBranchTypes eBranch, int iLevel) const;
	bool CanGetAdvancedTenet() const;

	// Functions to process AI each turn
	void DoPolicyAI();
	void DoChooseIdeology();

private:
	void AddFlavorAsStrategies(int iPropagatePercent);

	// Logging functions
	void LogFlavors(FlavorTypes eFlavor = NO_FLAVOR);

	bool* m_pabHasPolicy;
	bool* m_pabHasOneShotPolicyFired;
	bool* m_pabHaveOneShotFreeUnitsFired;
	bool* m_pabPolicyBranchUnlocked;
	bool* m_pabPolicyBranchBlocked;
	bool* m_pabPolicyBranchFinished;
	int* m_paiPolicyBranchBlockedCount;
	int* m_paiPolicyBlockedCount;
	PolicyBranchTypes* m_paePolicyBranchesChosen;
	PolicyBranchTypes* m_paePolicyBlockedBranchCheck;
	CvPolicyXMLEntries* m_pPolicies;
	CvPolicyAI* m_pPolicyAI;
	CvPlayer* m_pPlayer;

	int m_iNumExtraBranches;

	PolicyBranchTypes m_eBranchPicked1;
	PolicyBranchTypes m_eBranchPicked2;
	PolicyBranchTypes m_eBranchPicked3;
};

namespace PolicyHelpers
{
	int GetNumPlayersWithBranchUnlocked(PolicyBranchTypes eBranch);
	int GetNumFreePolicies(PolicyBranchTypes eBranch);
}

#endif //CIV5_POLICY_CLASSES_H