/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_CITY_CITIZENS_H
#define CIV5_CITY_CITIZENS_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvCityCitizens
//!  \brief		Keeps track of Citizens and Specialists in a City
//
//!  Key Attributes:
//!  - One instance for each city
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvCityCitizens
{
public:
#ifdef AUI_CITY_FIX_COMPONENT_CONSTRUCTORS_CONTAIN_POINTERS
	CvCityCitizens(CvCity* pCity);
#else
	CvCityCitizens(void);
#endif
	~CvCityCitizens(void);

	void Init(CvCity* pCity);
	void Uninit();
	void Reset();
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

#ifdef AUI_CONSTIFY
	CvCity* GetCity() const;
	CvPlayer* GetPlayer() const;
#else
	CvCity* GetCity();
	CvPlayer* GetPlayer();
#endif
	PlayerTypes GetOwner() const;
	TeamTypes GetTeam() const;

	void DoFoundCity();
	void DoTurn();

	int GetPlotValue(CvPlot* pPlot, bool bUseAllowGrowthFlag);

	// Are this City's Citizens automated? (always true for AI civs)
	bool IsAutomated() const;
	void SetAutomated(bool bValue);

	bool IsNoAutoAssignSpecialists() const;
	void SetNoAutoAssignSpecialists(bool bValue);

#ifdef AUI_CONSTIFY
	bool IsAvoidGrowth() const;
	bool IsForcedAvoidGrowth() const;
#else
	bool IsAvoidGrowth();
	bool IsForcedAvoidGrowth();
#endif
	void SetForcedAvoidGrowth(bool bAvoidGrowth);
	CityAIFocusTypes GetFocusType() const;
	void SetFocusType(CityAIFocusTypes eFocus);

	// Specialist AI
#ifndef NQM_PRUNING
	bool IsAIWantSpecialistRightNow();
#endif
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	BuildingTypes GetAIBestSpecialistBuilding(int* iSpecialistValue = NULL, bool bGetWorst = false, bool bIsWorked = false, SpecialistTypes eIgnoreSpecialist = NO_SPECIALIST) const;
#elif defined(AUI_CONSTIFY)
	BuildingTypes GetAIBestSpecialistBuilding(int& iSpecialistValue) const;
#else
	BuildingTypes GetAIBestSpecialistBuilding(int& iSpecialistValue);
#endif
#if defined(AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS) || defined(AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER) || defined(AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION) || defined(AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS) || defined(AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT)
	int GetSpecialistValue(SpecialistTypes eSpecialist, bool bForRemoval = false) const;
#elif defined(AUI_CONSTIFY)
	int GetSpecialistValue(SpecialistTypes eSpecialist) const;
#else
	int GetSpecialistValue(SpecialistTypes eSpecialist);
#endif
#ifndef AUI_CITIZENS_IS_BETTER_THAN_DEFAULT_SPECIALIST_USE_REGULAR_VALUES
#ifdef AUI_CONSTIFY
	bool IsBetterThanDefaultSpecialist(SpecialistTypes eSpecialist) const;
#else
	bool IsBetterThanDefaultSpecialist(SpecialistTypes eSpecialist);
#endif
#endif

	// Citizen Assignment
	int GetNumUnassignedCitizens() const;
	void ChangeNumUnassignedCitizens(int iChange);
	int GetNumCitizensWorkingPlots() const;
	void ChangeNumCitizensWorkingPlots(int iChange);

#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
	bool DoAddBestCitizenFromUnassigned(int* piBestScore = NULL);
#else
	bool DoAddBestCitizenFromUnassigned();
#endif
	bool DoRemoveWorstCitizen(bool bRemoveForcedStatus = false, SpecialistTypes eDontChangeSpecialist = NO_SPECIALIST, int iCurrentCityPopulation = -1);

	void DoReallocateCitizens();
#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
	void DoSelfConsistencyCheck(int iMaxIterations = -1);
#endif

	CvPlot* GetBestCityPlotWithValue(int& iValue, bool bWantBest, bool bWantWorked);

	// Worked Plots
	bool IsWorkingPlot(const CvPlot* pPlot) const;
	void SetWorkingPlot(CvPlot* pPlot, bool bNewValue, bool bUseUnassignedPool = true);
	void DoAlterWorkingPlot(int iIndex);

	// Forced Working Plots (human override)
	bool IsForcedWorkingPlot(const CvPlot* pPlot) const;
	void SetForcedWorkingPlot(CvPlot* pPlot, bool bNewValue);

	void DoValidateForcedWorkingPlots();
	void DoDemoteWorstForcedWorkingPlot();

	int GetNumForcedWorkingPlots() const;
	void ChangeNumForcedWorkingPlots(int iChange);

	bool IsCanWork(CvPlot* pPlot) const;
#ifdef AUI_CITIZENS_FIX_LOCKED_TILES_BLOCKED
	bool IsPlotBlockaded(const CvPlot* pPlot) const;
	bool GetNumForcedWorkingPlotsBlocked() const;
#else
	bool IsPlotBlockaded(CvPlot* pPlot) const;
#endif
	bool IsAnyPlotBlockaded() const;


#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	bool DoVerifyWorkingPlot(CvPlot* pPlot);
#else
	void DoVerifyWorkingPlot(CvPlot* pPlot);
#endif
	void DoVerifyWorkingPlots();

	// Helpful Stuff
	int GetCityIndexFromPlot(const CvPlot* pPlot) const;
	CvPlot* GetCityPlotFromIndex(int iIndex) const;

	// Specialists
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	int getCachedGPChangeT100ForThisTurn(SpecialistTypes eGPSpecialistType) const;
	void cacheGPChangesT100ForThisTurn();
#endif
	void DoSpecialists();

#ifdef AUI_CONSTIFY
	bool IsCanAddSpecialistToBuilding(BuildingTypes eBuilding) const;
#else
	bool IsCanAddSpecialistToBuilding(BuildingTypes eBuilding);
#endif
	void DoAddSpecialistToBuilding(BuildingTypes eBuilding, bool bForced);
	void DoRemoveSpecialistFromBuilding(BuildingTypes eBuilding, bool bForced, bool bEliminatePopulation = false);
	void DoRemoveAllSpecialistsFromBuilding(BuildingTypes eBuilding, bool bEliminatePopulation = false);
	bool DoRemoveWorstSpecialist(SpecialistTypes eDontChangeSpecialist, const BuildingTypes eDontRemoveFromBuilding = NO_BUILDING);

	int GetNumDefaultSpecialists() const;
	void ChangeNumDefaultSpecialists(int iChange);
	int GetNumForcedDefaultSpecialists() const;
	void ChangeNumForcedDefaultSpecialists(int iChange);

	int GetSpecialistCount(SpecialistTypes eIndex) const;
	int GetTotalSpecialistCount() const;

	int GetBuildingGreatPeopleRateChanges(SpecialistTypes eSpecialist) const;
	void ChangeBuildingGreatPeopleRateChanges(SpecialistTypes eSpecialist, int iChange);

	int GetSpecialistGreatPersonProgress(SpecialistTypes eIndex) const;
	int GetSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex) const;
	void ChangeSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex, int iChange);
	void DoResetSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex);

	int GetNumSpecialistsInBuilding(BuildingTypes eBuilding) const;
	int GetNumForcedSpecialistsInBuilding(BuildingTypes eBuilding) const;

	void DoClearForcedSpecialists();

#ifdef AUI_CONSTIFY
	int GetNumSpecialistsAllowedByBuilding(const CvBuildingEntry& kBuilding) const;

	int GetSpecialistUpgradeThreshold(UnitClassTypes eUnitClass) const;
#else
	int GetNumSpecialistsAllowedByBuilding(const CvBuildingEntry& kBuilding);

	int GetSpecialistUpgradeThreshold(UnitClassTypes eUnitClass);
#endif
#ifdef ENHANCED_GRAPHS
	void DoSpawnGreatPerson(UnitTypes eUnit, bool bIncrementCount, bool bCountAsProphet, bool bMayaBoost = false);
#else
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
	void DoSpawnGreatPerson(UnitTypes eUnit, bool bIncrementCount, bool bCountAsProphet, bool bSpawnWithNoExpendedTrigger = false);
#else
	void DoSpawnGreatPerson(UnitTypes eUnit, bool bIncrementCount, bool bCountAsProphet);
#endif
#endif

#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	bool getIgnoreFoodProduction() const;
	void setIgnoreFoodProduction(bool bNewValue);
#endif

private:

	CvCity* m_pCity;

	bool m_bAutomated;
	bool m_bNoAutoAssignSpecialists;

	int m_iNumUnassignedCitizens;
	int m_iNumCitizensWorkingPlots;
	int m_iNumForcedWorkingPlots;

	CityAIFocusTypes m_eCityAIFocusTypes;
	bool m_bForceAvoidGrowth;
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	bool m_bIgnoreFoodProduction;
#endif

	bool m_pabWorkingPlot[NUM_CITY_PLOTS];
	bool m_pabForcedWorkingPlot[NUM_CITY_PLOTS];

	int m_iNumDefaultSpecialists;
	int m_iNumForcedDefaultSpecialists;
	int* m_aiSpecialistCounts;
	int* m_aiSpecialistGreatPersonProgressTimes100;
	int* m_aiNumSpecialistsInBuilding;
	int* m_aiNumForcedSpecialistsInBuilding;
	int* m_piBuildingGreatPeopleRateChanges;
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	int* m_aiCachedGPChangeT100ForThisTurn;
#endif

	bool m_bInited;

};

#endif // CIV5_CITY_CITIZENS_H