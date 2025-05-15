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
#include "CvCitySpecializationAI.h"
#include "CvEconomicAI.h"
#include "CvGrandStrategyAI.h"
#include "CvDllInterfaces.h"
#include "CvInfosSerializationHelper.h"
#include "cvStopWatch.h"

// must be included after all other headers
#include "LintFree.h"


//=====================================
// CvCityCitizens
//=====================================

/// Constructor
#ifdef AUI_CITY_FIX_COMPONENT_CONSTRUCTORS_CONTAIN_POINTERS
CvCityCitizens::CvCityCitizens(CvCity* pCity)
{
	m_pCity = pCity;
#else
CvCityCitizens::CvCityCitizens()
{
#endif
	m_aiSpecialistCounts = NULL;
	m_aiSpecialistGreatPersonProgressTimes100 = NULL;
	m_aiNumSpecialistsInBuilding = NULL;
	m_aiNumForcedSpecialistsInBuilding = NULL;
	m_piBuildingGreatPeopleRateChanges = NULL;
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	m_aiCachedGPChangeT100ForThisTurn = NULL;
#endif
}

/// Destructor
CvCityCitizens::~CvCityCitizens()
{
	Uninit();
}

/// Initialize
void CvCityCitizens::Init(CvCity* pCity)
{
	m_pCity = pCity;

	// Clear variables
	Reset();

	m_bInited = true;
}

/// Deallocate memory created in initialize
void CvCityCitizens::Uninit()
{
	if(m_bInited)
	{
		SAFE_DELETE_ARRAY(m_aiSpecialistCounts);
		SAFE_DELETE_ARRAY(m_aiSpecialistGreatPersonProgressTimes100);
		SAFE_DELETE_ARRAY(m_aiNumSpecialistsInBuilding);
		SAFE_DELETE_ARRAY(m_aiNumForcedSpecialistsInBuilding);
		SAFE_DELETE_ARRAY(m_piBuildingGreatPeopleRateChanges);
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
		SAFE_DELETE_ARRAY(m_aiCachedGPChangeT100ForThisTurn);
#endif
	}
	m_bInited = false;
}

/// Reset member variables
void CvCityCitizens::Reset()
{
	m_bAutomated = false;
	m_bNoAutoAssignSpecialists = false;
	m_iNumUnassignedCitizens = 0;
	m_iNumCitizensWorkingPlots = 0;
	m_iNumForcedWorkingPlots = 0;

	m_eCityAIFocusTypes = NO_CITY_AI_FOCUS_TYPE;

#ifdef AUI_WARNING_FIXES
	uint iI;
#else
	int iI;
#endif

	CvAssertMsg((0 < NUM_CITY_PLOTS),  "NUM_CITY_PLOTS is not greater than zero but an array is being allocated in CvCityCitizens::reset");
	for(iI = 0; iI < NUM_CITY_PLOTS; iI++)
	{
		m_pabWorkingPlot[iI] = false;
		m_pabForcedWorkingPlot[iI] = false;
	}

	m_iNumDefaultSpecialists = 0;
	m_iNumForcedDefaultSpecialists = 0;

	CvAssertMsg(m_aiSpecialistCounts==NULL, "about to leak memory, CvCityCitizens::m_aiSpecialistCounts");
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_aiSpecialistCounts);
#endif
	m_aiSpecialistCounts = FNEW(int[GC.getNumSpecialistInfos()], c_eCiv5GameplayDLL, 0);
	for(iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		m_aiSpecialistCounts[iI] = 0;
	}

	CvAssertMsg(m_aiSpecialistGreatPersonProgressTimes100==NULL, "about to leak memory, CvCityCitizens::m_aiSpecialistGreatPersonProgressTimes100");
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_aiSpecialistGreatPersonProgressTimes100);
#endif
	m_aiSpecialistGreatPersonProgressTimes100 = FNEW(int[GC.getNumSpecialistInfos()], c_eCiv5GameplayDLL, 0);
	for(iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		m_aiSpecialistGreatPersonProgressTimes100[iI] = 0;
	}

	CvAssertMsg(m_aiNumSpecialistsInBuilding==NULL, "about to leak memory, CvCityCitizens::m_aiNumSpecialistsInBuilding");
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_aiNumSpecialistsInBuilding);
#endif
	m_aiNumSpecialistsInBuilding = FNEW(int[GC.getNumBuildingInfos()], c_eCiv5GameplayDLL, 0);
	for(iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		m_aiNumSpecialistsInBuilding[iI] = 0;
	}

	CvAssertMsg(m_aiNumForcedSpecialistsInBuilding==NULL, "about to leak memory, CvCityCitizens::m_aiNumForcedSpecialistsInBuilding");
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_aiNumForcedSpecialistsInBuilding);
#endif
	m_aiNumForcedSpecialistsInBuilding = FNEW(int[GC.getNumBuildingInfos()], c_eCiv5GameplayDLL, 0);
	for(iI = 0; iI < GC.getNumBuildingInfos(); iI++)
	{
		m_aiNumForcedSpecialistsInBuilding[iI] = 0;
	}

	CvAssertMsg(m_piBuildingGreatPeopleRateChanges==NULL, "about to leak memory, CvCityCitizens::m_piBuildingGreatPeopleRateChanges");
#ifdef AUI_WARNING_FIXES
	SAFE_DELETE_ARRAY(m_piBuildingGreatPeopleRateChanges);
#endif
	m_piBuildingGreatPeopleRateChanges = FNEW(int[GC.getNumSpecialistInfos()], c_eCiv5GameplayDLL, 0);
	for(iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		m_piBuildingGreatPeopleRateChanges[iI] = 0;
	}

#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	SAFE_DELETE_ARRAY(m_aiCachedGPChangeT100ForThisTurn);
	m_aiCachedGPChangeT100ForThisTurn = FNEW(int[GC.getNumSpecialistInfos()], c_eCiv5GameplayDLL, 0);
	for (iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		m_aiCachedGPChangeT100ForThisTurn[iI] = 0;
	}
#endif

	m_bForceAvoidGrowth = false;
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	m_bIgnoreFoodProduction = true;
#endif
}

/// Serialization read
void CvCityCitizens::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;

	kStream >> m_bAutomated;
	kStream >> m_bNoAutoAssignSpecialists;
	kStream >> m_iNumUnassignedCitizens;
	kStream >> m_iNumCitizensWorkingPlots;
	kStream >> m_iNumForcedWorkingPlots;

	kStream >> m_eCityAIFocusTypes;

	kStream >> 	m_bForceAvoidGrowth;

	kStream >> m_pabWorkingPlot;
	kStream >> m_pabForcedWorkingPlot;

	kStream >> m_iNumDefaultSpecialists;
	kStream >> m_iNumForcedDefaultSpecialists;

	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_aiSpecialistCounts, GC.getNumSpecialistInfos());
	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_aiSpecialistGreatPersonProgressTimes100, GC.getNumSpecialistInfos());

	BuildingArrayHelpers::Read(kStream, m_aiNumSpecialistsInBuilding);
	BuildingArrayHelpers::Read(kStream, m_aiNumForcedSpecialistsInBuilding);

	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_piBuildingGreatPeopleRateChanges, GC.getNumSpecialistInfos());
}

/// Serialization write
void CvCityCitizens::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;

	kStream << m_bAutomated;
	kStream << m_bNoAutoAssignSpecialists;
	kStream << m_iNumUnassignedCitizens;
	kStream << m_iNumCitizensWorkingPlots;
	kStream << m_iNumForcedWorkingPlots;

	kStream << m_eCityAIFocusTypes;

	kStream << 	m_bForceAvoidGrowth;

	kStream << m_pabWorkingPlot;
	kStream << m_pabForcedWorkingPlot;

	kStream << m_iNumDefaultSpecialists;
	kStream << m_iNumForcedDefaultSpecialists;

	CvInfosSerializationHelper::WriteHashedDataArray<SpecialistTypes, int>(kStream, m_aiSpecialistCounts, GC.getNumSpecialistInfos());
	CvInfosSerializationHelper::WriteHashedDataArray<SpecialistTypes, int>(kStream, m_aiSpecialistGreatPersonProgressTimes100, GC.getNumSpecialistInfos());

	BuildingArrayHelpers::Write(kStream, m_aiNumSpecialistsInBuilding, GC.getNumBuildingInfos());
	BuildingArrayHelpers::Write(kStream, m_aiNumForcedSpecialistsInBuilding, GC.getNumBuildingInfos());

	CvInfosSerializationHelper::WriteHashedDataArray<SpecialistTypes, int>(kStream, m_piBuildingGreatPeopleRateChanges, GC.getNumSpecialistInfos());
}

/// Returns the City object this set of Citizens is associated with
#ifdef AUI_CONSTIFY
CvCity* CvCityCitizens::GetCity() const
#else
CvCity* CvCityCitizens::GetCity()
#endif
{
	return m_pCity;
}

/// Returns the Player object this City belongs to
#ifdef AUI_CONSTIFY
CvPlayer* CvCityCitizens::GetPlayer() const
#else
CvPlayer* CvCityCitizens::GetPlayer()
#endif
{
	return &GET_PLAYER(GetOwner());
}

/// Helper function to return Player owner of our City
PlayerTypes CvCityCitizens::GetOwner() const
{
	return m_pCity->getOwner();
}

/// Helper function to return Team owner of our City
TeamTypes CvCityCitizens::GetTeam() const
{
	return m_pCity->getTeam();
}

/// What happens when a City is first founded?
void CvCityCitizens::DoFoundCity()
{
	// always work the home plot (center)
	CvPlot* pHomePlot = GetCityPlotFromIndex(CITY_HOME_PLOT);
	if(pHomePlot != NULL)
	{
		bool bWorkPlot = IsCanWork(pHomePlot);
		SetWorkingPlot(pHomePlot, bWorkPlot, /*bUseUnassignedPool*/ false);
	}
}

/// Processed every turn
void CvCityCitizens::DoTurn()
{
	AI_PERF_FORMAT("City-AI-perf.csv", ("CvCityCitizens::DoTurn, Turn %03d, %s, %s", GC.getGame().getElapsedGameTurns(), m_pCity->GetPlayer()->getCivilizationShortDescription(), m_pCity->getName().c_str()) );
	DoVerifyWorkingPlots();

	CvPlayerAI& thisPlayer = GET_PLAYER(GetOwner());

	if(m_pCity->IsPuppet())
	{
		SetFocusType(CITY_AI_FOCUS_TYPE_GOLD);
		SetNoAutoAssignSpecialists(false);
		SetForcedAvoidGrowth(false);
#ifndef AUI_CITIZENS_PUPPET_STILL_WANTS_GROWTH
		int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
		if(iExcessFoodTimes100 < 0)
		{
			SetFocusType(NO_CITY_AI_FOCUS_TYPE);
			//SetNoAutoAssignSpecialists(true);
			SetForcedAvoidGrowth(false);
		}
#endif
	}
	else if(!thisPlayer.isHuman())
	{
		CitySpecializationTypes eWonderSpecializationType = thisPlayer.GetCitySpecializationAI()->GetWonderSpecialization();

		if(GC.getGame().getGameTurn() % 8 == 0)
		{
			SetFocusType(CITY_AI_FOCUS_TYPE_GOLD_GROWTH);
			SetNoAutoAssignSpecialists(true);
			SetForcedAvoidGrowth(false);
			int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
			if(iExcessFoodTimes100 < 200)
			{
				SetFocusType(NO_CITY_AI_FOCUS_TYPE);
				//SetNoAutoAssignSpecialists(true);
			}
		}
		if(m_pCity->isCapital() && !thisPlayer.isMinorCiv() && m_pCity->GetCityStrategyAI()->GetSpecialization() != eWonderSpecializationType)
		{
			SetFocusType(NO_CITY_AI_FOCUS_TYPE);
			SetNoAutoAssignSpecialists(false);
			SetForcedAvoidGrowth(false);
			int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
			if(iExcessFoodTimes100 < 400)
			{
				SetFocusType(CITY_AI_FOCUS_TYPE_FOOD);
				//SetNoAutoAssignSpecialists(true);
			}
		}
		else if(m_pCity->GetCityStrategyAI()->GetSpecialization() == eWonderSpecializationType)
		{
			SetFocusType(CITY_AI_FOCUS_TYPE_PRODUCTION);
			SetNoAutoAssignSpecialists(false);
			//SetForcedAvoidGrowth(true);
			int iExcessFoodTimes100;// = m_pCity->getYieldRateTimes100(YIELD_FOOD) - (m_pCity->foodConsumption() * 100);
			//if (iExcessFoodTimes100 < 200)
			//{
			SetForcedAvoidGrowth(false);
			//}
			iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
			if(iExcessFoodTimes100 < 200)
			{
				SetFocusType(CITY_AI_FOCUS_TYPE_PROD_GROWTH);
				//SetNoAutoAssignSpecialists(true);
				SetForcedAvoidGrowth(false);
			}
			iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
			if(iExcessFoodTimes100 < 200)
			{
				SetFocusType(NO_CITY_AI_FOCUS_TYPE);
				//SetNoAutoAssignSpecialists(true);
				SetForcedAvoidGrowth(false);
			}
		}
		else if(m_pCity->getPopulation() < 5)  // we want a balanced growth
		{
			SetFocusType(NO_CITY_AI_FOCUS_TYPE);
			//SetNoAutoAssignSpecialists(true);
			SetForcedAvoidGrowth(false);
		}
		else
		{
			// Are we running at a deficit?
			EconomicAIStrategyTypes eStrategyLosingMoney = (EconomicAIStrategyTypes) GC.getInfoTypeForString("ECONOMICAISTRATEGY_LOSING_MONEY", true);
			bool bInDeficit = false;
			if (eStrategyLosingMoney != NO_ECONOMICAISTRATEGY)
			{
				bInDeficit = thisPlayer.GetEconomicAI()->IsUsingStrategy(eStrategyLosingMoney);
			}

			if(bInDeficit)
			{
				SetFocusType(CITY_AI_FOCUS_TYPE_GOLD_GROWTH);
				SetNoAutoAssignSpecialists(false);
				SetForcedAvoidGrowth(false);
				int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
				if(iExcessFoodTimes100 < 200)
				{
					SetFocusType(NO_CITY_AI_FOCUS_TYPE);
					//SetNoAutoAssignSpecialists(true);
				}
			}
			else if(GC.getGame().getGameTurn() % 3 == 0 && thisPlayer.GetGrandStrategyAI()->GetActiveGrandStrategy() == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CULTURE"))
			{
				SetFocusType(CITY_AI_FOCUS_TYPE_CULTURE);
				SetNoAutoAssignSpecialists(true);
				SetForcedAvoidGrowth(false);
				int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
				if(iExcessFoodTimes100 < 200)
				{
					SetFocusType(NO_CITY_AI_FOCUS_TYPE);
					//SetNoAutoAssignSpecialists(true);
				}
			}
			else // we aren't a small city, building a wonder, or going broke
			{
				SetNoAutoAssignSpecialists(false);
				SetForcedAvoidGrowth(false);
				CitySpecializationTypes eSpecialization = m_pCity->GetCityStrategyAI()->GetSpecialization();
				if(eSpecialization != -1)
				{
					CvCitySpecializationXMLEntry* pCitySpecializationEntry =  GC.getCitySpecializationInfo(eSpecialization);
					if(pCitySpecializationEntry)
					{
						YieldTypes eYield = pCitySpecializationEntry->GetYieldType();
						if(eYield == YIELD_FOOD)
						{
							SetFocusType(CITY_AI_FOCUS_TYPE_FOOD);
						}
						else if(eYield == YIELD_PRODUCTION)
						{
							SetFocusType(CITY_AI_FOCUS_TYPE_PROD_GROWTH);
						}
						else if(eYield == YIELD_GOLD)
						{
							SetFocusType(CITY_AI_FOCUS_TYPE_GOLD);
						}
						else if(eYield == YIELD_SCIENCE)
						{
							SetFocusType(CITY_AI_FOCUS_TYPE_SCIENCE);
						}
						else
						{
							SetFocusType(NO_CITY_AI_FOCUS_TYPE);
						}
					}
					else
					{
						SetFocusType(NO_CITY_AI_FOCUS_TYPE);
					}
				}
				else
				{
					SetFocusType(NO_CITY_AI_FOCUS_TYPE);
				}
			}
		}
	}

	CvAssertMsg((GetNumCitizensWorkingPlots() + GetTotalSpecialistCount() + GetNumUnassignedCitizens()) <= GetCity()->getPopulation(), "Gameplay: More workers than population in the city.");
	DoReallocateCitizens();
	CvAssertMsg((GetNumCitizensWorkingPlots() + GetTotalSpecialistCount() + GetNumUnassignedCitizens()) <= GetCity()->getPopulation(), "Gameplay: More workers than population in the city.");
	DoSpecialists();

	CvAssertMsg((GetNumCitizensWorkingPlots() + GetTotalSpecialistCount() + GetNumUnassignedCitizens()) <= GetCity()->getPopulation(), "Gameplay: More workers than population in the city.");
}

/// What is the overall value of the current Plot?
int CvCityCitizens::GetPlotValue(CvPlot* pPlot, bool bUseAllowGrowthFlag)
{
	int iValue = 0;

	// Yield Values
#ifdef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
	int iFoodYieldValue = /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
#else
	int iFoodYieldValue = (/*12*/ GC.getAI_CITIZEN_VALUE_FOOD() * pPlot->getYield(YIELD_FOOD));
#endif
	int iProductionYieldValue = (/*8*/ GC.getAI_CITIZEN_VALUE_PRODUCTION() * pPlot->getYield(YIELD_PRODUCTION));
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
	int iGoldYieldValue = (pPlot->getYield(YIELD_GOLD));
	int iScienceYieldValue = (pPlot->getYield(YIELD_SCIENCE));
#else
	int iGoldYieldValue = (/*10*/ GC.getAI_CITIZEN_VALUE_GOLD() * pPlot->getYield(YIELD_GOLD));
	int iScienceYieldValue = (/*6*/ GC.getAI_CITIZEN_VALUE_SCIENCE() * pPlot->getYield(YIELD_SCIENCE));
#endif
	int iCultureYieldValue = (GC.getAI_CITIZEN_VALUE_CULTURE() * pPlot->getYield(YIELD_CULTURE));
	int iFaithYieldValue = (GC.getAI_CITIZEN_VALUE_FAITH() * pPlot->getYield(YIELD_FAITH));
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	int iGoldenAgePointsValue = (GC.getAI_CITIZEN_VALUE_GOLDEN_AGE_POINTS() * pPlot->getYield(YIELD_GOLDEN_AGE_POINTS));
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
#ifndef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
	iFoodYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FOOD);
#endif
	iProductionYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_PRODUCTION);
	iGoldYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_GOLD);
	iScienceYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_SCIENCE);
	iCultureYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_CULTURE);
	iFaithYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FAITH);
#endif
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
	int iCurrentScienceLoss = -GetPlayer()->calculateGoldRateTimes100() - GetPlayer()->GetTreasury()->GetGoldTimes100();
	if (IsWorkingPlot(pPlot))
		iCurrentScienceLoss += iGoldYieldValue;
	if (iCurrentScienceLoss > 0)
	{
		int iGoldToScienceT100 = MIN(iGoldYieldValue, iCurrentScienceLoss);
		iGoldYieldValue -= iGoldToScienceT100;
		iScienceYieldValue += iGoldToScienceT100;
	}
	iGoldYieldValue *= GC.getAI_CITIZEN_VALUE_GOLD();
	iScienceYieldValue *= GC.getAI_CITIZEN_VALUE_SCIENCE();
#endif

	// How much surplus food are we making?
	int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
#if defined(AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER) || defined(AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION) || defined(AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS)
	int iExcessFoodWithPlotTimes100 = iExcessFoodTimes100;
	if (IsWorkingPlot(pPlot))
		iExcessFoodTimes100 -= pPlot->getYield(YIELD_FOOD) * m_pCity->getBaseYieldRateModifier(YIELD_FOOD);
	else
		iExcessFoodWithPlotTimes100 += pPlot->getYield(YIELD_FOOD) * m_pCity->getBaseYieldRateModifier(YIELD_FOOD);
#endif

	bool bAvoidGrowth = IsAvoidGrowth();

	// City Focus
	CityAIFocusTypes eFocus = GetFocusType();
	if(eFocus == CITY_AI_FOCUS_TYPE_FOOD)
		iFoodYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_PRODUCTION)
		iProductionYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_GOLD)
		iGoldYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_SCIENCE)
		iScienceYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_CULTURE)
		iCultureYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
	{
		iFoodYieldValue *= 2;
		iGoldYieldValue *= 2;
	}
	else if(eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
	{
		iFoodYieldValue *= 2;
		iProductionYieldValue *= 2;
	}
	else if(eFocus == CITY_AI_FOCUS_TYPE_FAITH)
	{
		iFaithYieldValue *= 3;
	}
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	else if (eFocus == CITY_AI_FOCUS_TYPE_GOLDEN_AGE_POINTS)
	{
		iGoldenAgePointsValue *= 3;
	}
#endif

#ifdef AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	if (!getIgnoreFoodProduction() && m_pCity->isFoodProduction())
#else
	if (m_pCity->isFoodProduction())
#endif
	{
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
		iFoodYieldValue = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100) - m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		int iDummy = m_pCity->GetFoodProduction(iExcessFoodTimes100 / 100);
		int iCurrProdFromFood = m_pCity->foodDifference(true, &iDummy);
		iDummy = m_pCity->GetFoodProduction(iExcessFoodWithPlotTimes100 / 100);
		int iProdFromFoodWithTile = m_pCity->foodDifference(true, &iDummy);
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		iFoodYieldValue = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
		int iCurrProdFromFood = m_pCity->GetFoodProduction(iExcessFoodTimes100 / 100);
		int iProdFromFoodWithTile = m_pCity->GetFoodProduction(iExcessFoodWithPlotTimes100 / 100);
#endif
		iProductionYieldValue += (iProdFromFoodWithTile - iCurrProdFromFood) * GC.getAI_CITIZEN_VALUE_PRODUCTION() * (eFocus == CITY_AI_FOCUS_TYPE_PRODUCTION ? 3 : 1);
	}
	else
#endif
	// Food can be worth less if we don't want to grow
	if(bUseAllowGrowthFlag && iExcessFoodTimes100 >= 0 && bAvoidGrowth)
	{
		// If we at least have enough Food to feed everyone, zero out the value of additional food
#ifdef AUI_CITIZENS_AVOID_GROWTH_STILL_VALUES_EXCESS_FOOD
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100);
#endif
		iFoodYieldValue = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		iFoodYieldValue = 0;
#endif
	}
	// We want to grow here
	else
	{
#ifdef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
		iFoodYieldValue *= 8;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100);
#endif
#ifdef AUI_CITIZENS_AVOID_GROWTH_STILL_VALUES_EXCESS_FOOD
		int iExcessFoodYieldValue = 1;
#else
		int iExcessFoodYieldValue = 0;
#endif
		int iTargetFoodT100 = 0;
		if (!bUseAllowGrowthFlag || !bAvoidGrowth)
		{
			iExcessFoodYieldValue = iFoodYieldValue / 16;
#ifdef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
			if (eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_FOOD || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH
				|| eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || m_pCity->getPopulation() < 2)
#else
			if (eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_FOOD || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
#endif
			{
				iTargetFoodT100 = 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION();
			}
#ifdef AUI_CITIZENS_PUPPET_STILL_WANTS_GROWTH
			else if (m_pCity->IsPuppet())
				iTargetFoodT100 = 50 * GC.getFOOD_CONSUMPTION_PER_POPULATION();
#endif
		}

		int iNonExcessFoodPlotYieldT100 = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
		int iExcessFoodPlotYieldT100 = iNonExcessFoodPlotYieldT100;
		if (iExcessFoodWithPlotTimes100 <= iTargetFoodT100)
		{
			iExcessFoodPlotYieldT100 = 0;
		}
		else if (iExcessFoodTimes100 >= iTargetFoodT100)
		{
			iNonExcessFoodPlotYieldT100 = 0;
		}
		else
		{
			iNonExcessFoodPlotYieldT100 = iTargetFoodT100 - iExcessFoodTimes100;
			iExcessFoodPlotYieldT100 = iExcessFoodWithPlotTimes100 - iTargetFoodT100;
		}

		iFoodYieldValue *= iNonExcessFoodPlotYieldT100;
		iFoodYieldValue += (iExcessFoodPlotYieldT100 * iExcessFoodYieldValue);
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		// If we have a non-default and non-food focus, only worry about getting to 0 food
#ifdef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
		if (eFocus != NO_CITY_AI_FOCUS_TYPE && eFocus != CITY_AI_FOCUS_TYPE_FOOD && eFocus != CITY_AI_FOCUS_TYPE_PROD_GROWTH && eFocus != CITY_AI_FOCUS_TYPE_GOLD_GROWTH && m_pCity->getPopulation() >= 5)
#else
		if(eFocus != NO_CITY_AI_FOCUS_TYPE && eFocus != CITY_AI_FOCUS_TYPE_FOOD && eFocus != CITY_AI_FOCUS_TYPE_PROD_GROWTH && eFocus != CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
#endif
		{
			int iFoodT100NeededFor0 = -iExcessFoodTimes100;

			if(iFoodT100NeededFor0 > 0)
			{
				iFoodYieldValue *= 8;
			}
			else
			{
				iFoodYieldValue /= 2;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				int iExtraFoodValueT100 = (m_pCity->foodDifferenceTimes100(true, NULL, true, iExcessFoodWithPlotTimes100 - iExcessFoodTimes100) -
					(iExcessFoodWithPlotTimes100 - iExcessFoodTimes100)) * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
				if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
					iExtraFoodValueT100 *= 3;
				else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
					iExtraFoodValueT100 *= 2;
				iFoodYieldValue += iExtraFoodValueT100 / 200;
#endif
			}
		}
		// If our surplus is not at least 2, really emphasize food plots
		else if(!bAvoidGrowth)
		{
#ifdef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
			int iFoodT100NeededFor2 = 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION() - iExcessFoodTimes100;
#else
			int iFoodT100NeededFor2 = 200 - iExcessFoodTimes100;
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
			int iExtraFoodValueT100 = (m_pCity->foodDifferenceTimes100(true, NULL, true, iExcessFoodWithPlotTimes100 - iExcessFoodTimes100) -
				(iExcessFoodWithPlotTimes100 - iExcessFoodTimes100)) * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
			if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
				iExtraFoodValueT100 *= 3;
			else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
				iExtraFoodValueT100 *= 2;
#endif

			if(iFoodT100NeededFor2 > 0)
			{
				iFoodYieldValue *= 8;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				iExtraFoodValueT100 = (m_pCity->foodDifferenceTimes100(true, NULL, true, MIN(iExcessFoodWithPlotTimes100, 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION())) -
					MIN(iExcessFoodWithPlotTimes100, 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION())) * 8 * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
				if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
					iExtraFoodValueT100 *= 3;
				else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
					iExtraFoodValueT100 *= 2;
				iFoodYieldValue += iExtraFoodValueT100 / 100;
#endif
			}
			else if (eFocus != CITY_AI_FOCUS_TYPE_FOOD)
			{
				iFoodYieldValue /= 2;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				iFoodYieldValue += iExtraFoodValueT100 / 200;
			}
			else
			{
				iFoodYieldValue += iExtraFoodValueT100 / 100;
#endif
			}
		}
#ifdef AUI_CITIZENS_FIX_GET_VALUE_FOOD_YIELD_VALUE_WHEN_STARVATION_WITH_AVOID_GROWTH
		// Food focus and negative food, but with avoid growth enabled for some reason
		else
		{
			iFoodYieldValue *= 8;
		}
#endif
#endif
	}

#ifndef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
	if((eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH) && !bAvoidGrowth && m_pCity->getPopulation() < 5)
	{
		iFoodYieldValue *= 4;
	}
#endif

	iValue += iFoodYieldValue;
	iValue += iProductionYieldValue;
	iValue += iGoldYieldValue;
	iValue += iScienceYieldValue;
	iValue += iCultureYieldValue;
	iValue += iFaithYieldValue;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
	iValue /= 100;
#endif

	return iValue;
}

/// Are this City's Citizens under automation?
bool CvCityCitizens::IsAutomated() const
{
	return m_bAutomated;
}

/// Sets this City's Citizens to be under automation
void CvCityCitizens::SetAutomated(bool bValue)
{
	m_bAutomated = bValue;
}

/// Are this City's Specialists under automation?
bool CvCityCitizens::IsNoAutoAssignSpecialists() const
{
	return m_bNoAutoAssignSpecialists;
}

/// Sets this City's Specialists to be under automation
void CvCityCitizens::SetNoAutoAssignSpecialists(bool bValue)
{
	if(m_bNoAutoAssignSpecialists != bValue)
	{
		m_bNoAutoAssignSpecialists = bValue;

		// If we're giving the AI control clear all manually assigned Specialists
		if(!bValue)
		{
			DoClearForcedSpecialists();
		}

		DoReallocateCitizens();
	}
}

/// Is this City avoiding growth?
#ifdef AUI_CONSTIFY
bool CvCityCitizens::IsAvoidGrowth() const
#else
bool CvCityCitizens::IsAvoidGrowth()
#endif
{
#ifndef AUI_CITIZENS_FIX_AVOID_GROWTH_FLAG_NOT_IGNORED_IF_NO_HAPPINESS
	if(GC.getGame().isOption(GAMEOPTION_NO_HAPPINESS))
	{
		return false;
	}
#endif

#ifdef AUI_CITIZENS_FIX_FORCED_AVOID_GROWTH_ONLY_WHEN_GROWING_LOWERS_HAPPINESS
	if (GetPlayer()->GetExcessHappiness() <= 0)
	{
		int iPopulation = m_pCity->getPopulation();
		int iLocalHappinessCap = iPopulation;

		// India has unique way to compute local happiness cap
		if (GetPlayer()->GetPlayerTraits()->GetCityUnhappinessModifier() != 0)
		{
			// 0.67 per population, rounded up
			iLocalHappinessCap = (iLocalHappinessCap * 20) + 15;
			iLocalHappinessCap /= 30;
		}
		// Growing would not be covered by local happiness
		if (m_pCity->GetLocalHappiness() < iLocalHappinessCap)
		{
			int iHappinessPerXPopulation = GetPlayer()->GetHappinessPerXPopulation();
			// Growing would not be covered by happiness per X population
			if (iHappinessPerXPopulation == 0 || m_pCity->IsPuppet() || (iPopulation + 1) / iHappinessPerXPopulation <= iPopulation / iHappinessPerXPopulation)
			{
				// Growing would not be covered by reduced unhappiness from population
				bool bHasSpecialistSlot = false;
#ifdef AUI_WARNING_FIXES
				for (uint iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#else
				for (int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#endif
				{
					const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
					CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
					if (pkBuildingInfo)
					{
						if (GetCity()->GetCityBuildings()->GetNumBuilding(eBuilding) > 0 && IsCanAddSpecialistToBuilding(eBuilding))
						{
							bHasSpecialistSlot = true;
							break;
						}
					}
				}
				if ((GetPlayer()->GetUnhappinessFromCityPopulation() + GetPlayer()->GetUnhappinessFromOccupiedCities()) / 100 <
					(GetPlayer()->GetUnhappinessFromCityPopulation(NULL, NULL, m_pCity, (bHasSpecialistSlot ? m_pCity : NULL))
						+ GetPlayer()->GetUnhappinessFromOccupiedCities(NULL, NULL, m_pCity, (bHasSpecialistSlot ? m_pCity : NULL))) / 100)
					return true;
			}
		}
	}
#elif !defined(AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS)
	if(GetPlayer()->GetExcessHappiness() < 0)
	{
		return true;
	}
#endif

	return IsForcedAvoidGrowth();
}

#if defined(AUI_CONSTIFY)
bool CvCityCitizens::IsForcedAvoidGrowth() const
#else
bool CvCityCitizens::IsForcedAvoidGrowth()
#endif
{
	return m_bForceAvoidGrowth;
}

void CvCityCitizens::SetForcedAvoidGrowth(bool bAvoidGrowth)
{
	if(m_bForceAvoidGrowth != bAvoidGrowth)
	{
		m_bForceAvoidGrowth = bAvoidGrowth;
		DoReallocateCitizens();
	}
}

/// What is this city focusing on?
CityAIFocusTypes CvCityCitizens::GetFocusType() const
{
	return m_eCityAIFocusTypes;
}

/// What is this city focusing on?
void CvCityCitizens::SetFocusType(CityAIFocusTypes eFocus)
{
	FAssert(eFocus >= NO_CITY_AI_FOCUS_TYPE);
	FAssert(eFocus < NUM_CITY_AI_FOCUS_TYPES);

	if(eFocus != m_eCityAIFocusTypes)
	{
		m_eCityAIFocusTypes = eFocus;
		// Reallocate with our new focus
		DoReallocateCitizens();
	}
}

#ifndef NQM_PRUNING
/// Does the AI want a Specialist?
bool CvCityCitizens::IsAIWantSpecialistRightNow()
{
	int iWeight = 100;

	// If the City is Size 1 or 2 then we probably don't want Specialists
	if(m_pCity->getPopulation() < 3)
	{
		iWeight /= 2;
	}

	int iFoodPerTurn = m_pCity->getYieldRate(YIELD_FOOD, false);
	int iFoodEatenPerTurn = m_pCity->foodConsumption();
	int iSurplusFood = iFoodPerTurn - iFoodEatenPerTurn;

	CityAIFocusTypes eFocusType = GetFocusType();

	// Don't want specialists until we've met our food needs
	if(iSurplusFood < 0)
	{
		return false;
	}
	else if(IsAvoidGrowth() && (eFocusType == NO_CITY_AI_FOCUS_TYPE || eFocusType == CITY_AI_FOCUS_TYPE_GREAT_PEOPLE))
	{
		iWeight *= 2;
	}
	else if(iSurplusFood <= 2)
	{
		iWeight /= 2;
	}
	else if(iSurplusFood > 2)
	{
		if(eFocusType == NO_CITY_AI_FOCUS_TYPE || eFocusType == CITY_AI_FOCUS_TYPE_GREAT_PEOPLE || eFocusType == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
		{
			iWeight *= 100 + (20 * (iSurplusFood - 4));
			iWeight /= 100;
		}
	}

	// If we're deficient in Production then we're less likely to want Specialists
	if(m_pCity->GetCityStrategyAI()->IsYieldDeficient(YIELD_PRODUCTION))
	{
		iWeight *= 50;
		iWeight /= 100;
	}
	// if we've got some slackers in town (since they provide Production)
	else if(GetNumDefaultSpecialists() > 0 && eFocusType != CITY_AI_FOCUS_TYPE_PRODUCTION && eFocusType != CITY_AI_FOCUS_TYPE_PROD_GROWTH)
	{
		iWeight *= 150;
		iWeight /= 100;
	}

	// Someone told this AI it should be focused on something that is usually gotten from specialists
	if(eFocusType == CITY_AI_FOCUS_TYPE_GREAT_PEOPLE)
	{
		// Loop through all Buildings
		BuildingTypes eBuilding;
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			eBuilding = (BuildingTypes) iBuildingLoop;

			// Have this Building in the City?
			if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
			{
				// Can't add more than the max
				if(IsCanAddSpecialistToBuilding(eBuilding))
				{
					iWeight *= 3;
					break;
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_CULTURE)
	{
		// Loop through all Buildings
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				// Have this Building in the City?
				if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if(IsCanAddSpecialistToBuilding(eBuilding))
					{
						const SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if(pSpecialistInfo && pSpecialistInfo->getCulturePerTurn() > 0)
						{
							iWeight *= 3;
							break;
						}
					}
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_SCIENCE)
	{
		// Loop through all Buildings
		BuildingTypes eBuilding;
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			eBuilding = (BuildingTypes) iBuildingLoop;

			// Have this Building in the City?
			if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
			{
				// Can't add more than the max
				if(IsCanAddSpecialistToBuilding(eBuilding))
				{
					SpecialistTypes eSpecialist = (SpecialistTypes) GC.getBuildingInfo(eBuilding)->GetSpecialistType();
					CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
					if(pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_SCIENCE) > 0)
					{
						iWeight *= 3;
					}

					if(GetPlayer()->getSpecialistExtraYield(YIELD_SCIENCE) > 0)
					{
						iWeight *= 3;
					}

					if(GetPlayer()->GetPlayerTraits()->GetSpecialistYieldChange(eSpecialist, YIELD_SCIENCE) > 0)
					{
						iWeight *= 3;
					}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
					if(GetPlayer()->GetPlayerTraits()->GetAnySpecificSpecialistYieldChange(eSpecialist, YIELD_SCIENCE) > 0)
					{
						iWeight *= 3;
					}
#endif
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_PRODUCTION)
	{
		// Loop through all Buildings
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				// Have this Building in the City?
				if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if(IsCanAddSpecialistToBuilding(eBuilding))
					{
						SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if(NULL != pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_PRODUCTION) > 0)
						{
							iWeight *= 150;
							iWeight /= 100;
						}

						if(GetPlayer()->getSpecialistExtraYield(YIELD_PRODUCTION) > 0)
						{
							iWeight *= 2;
						}

						if(GetPlayer()->GetPlayerTraits()->GetSpecialistYieldChange(eSpecialist, YIELD_PRODUCTION) > 0)
						{
							iWeight *= 2;
						}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
						if(GetPlayer()->GetPlayerTraits()->GetAnySpecificSpecialistYieldChange(eSpecialist, YIELD_PRODUCTION) > 0)
						{
							iWeight *= 2;
						}
#endif
					}
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_GOLD)
	{
		// Loop through all Buildings
		BuildingTypes eBuilding;
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			eBuilding = (BuildingTypes) iBuildingLoop;

			// Have this Building in the City?
			if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
			{
				// Can't add more than the max
				if(IsCanAddSpecialistToBuilding(eBuilding))
				{
					SpecialistTypes eSpecialist = (SpecialistTypes) GC.getBuildingInfo(eBuilding)->GetSpecialistType();
					CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
					if(pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_GOLD) > 0)
					{
						iWeight *= 150;
						iWeight /= 100;
						break;
					}
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_FOOD)
	{
		iWeight *= 50;
		iWeight /= 100;
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
	{
		// Loop through all Buildings
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				// Have this Building in the City?
				if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if(IsCanAddSpecialistToBuilding(eBuilding))
					{
						SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if(pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_PRODUCTION) > 0)
						{
							iWeight *= 150;
							iWeight /= 100;
							break;
						}
					}
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
	{
		// Loop through all Buildings
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				// Have this Building in the City?
				if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if(IsCanAddSpecialistToBuilding(eBuilding))
					{
						SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if(pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_GOLD) > 0)
						{
							iWeight *= 150;
							iWeight /= 100;
						}

						if(GetPlayer()->getSpecialistExtraYield(YIELD_GOLD) > 0)
						{
							iWeight *= 2;
						}

						if(GetPlayer()->GetPlayerTraits()->GetSpecialistYieldChange(eSpecialist, YIELD_GOLD) > 0)
						{
							iWeight *= 2;
						}
#ifdef LEK_TRAIT_SPECIALIST_YIELD_MAX_ONE
						if(GetPlayer()->GetPlayerTraits()->GetAnySpecificSpecialistYieldChange(eSpecialist, YIELD_GOLD) > 0)
						{
							iWeight *= 2;
						}
#endif
					}
				}
			}
		}
	}
	else if(eFocusType == CITY_AI_FOCUS_TYPE_FAITH)
	{
		// Loop through all Buildings
		for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = (BuildingTypes) iBuildingLoop;
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				// Have this Building in the City?
				if(m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if(IsCanAddSpecialistToBuilding(eBuilding))
					{
						const SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if(pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_FAITH) > 0)
						{
							iWeight *= 3;
							break;
						}
					}
				}
			}
		}
	}
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	else if(eFocusType == CITY_AI_FOCUS_TYPE_GOLDEN_AGE_POINTS)
	{
		// Loop through all Buildings
		for (int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
		{
			const BuildingTypes eBuilding = (BuildingTypes)iBuildingLoop;
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if (pkBuildingInfo)
			{
				// Have this Building in the City?
				if (m_pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					// Can't add more than the max
					if (IsCanAddSpecialistToBuilding(eBuilding))
					{
						const SpecialistTypes eSpecialist = (SpecialistTypes)pkBuildingInfo->GetSpecialistType();
						CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
						if (pSpecialistInfo && pSpecialistInfo->getYieldChange(YIELD_GOLDEN_AGE_POINTS) > 0)
						{
							iWeight *= 3;
							break;
						}
					}
				}
			}
		}
	}
#endif

	// specialists are cheaper somehow
	if (m_pCity->GetPlayer()->isHalfSpecialistUnhappiness() || m_pCity->GetPlayer()->isHalfSpecialistFood())
	{
		iWeight *= 150;
		iWeight /= 100;
	}

	// Does the AI want it enough?
	if(iWeight >= 150)
	{
		return true;
	}

	return false;
}
#endif

/// What is the Building Type the AI likes the Specialist of most right now?
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
BuildingTypes CvCityCitizens::GetAIBestSpecialistBuilding(int* iSpecialistValue, bool bGetWorst, bool bIsWorked, SpecialistTypes eIgnoreSpecialist) const
#elif defined(AUI_CONSTIFY)
BuildingTypes CvCityCitizens::GetAIBestSpecialistBuilding(int& iSpecialistValue) const
#else
BuildingTypes CvCityCitizens::GetAIBestSpecialistBuilding(int& iSpecialistValue)
#endif
{
	BuildingTypes eBestBuilding = NO_BUILDING;
	int iBestSpecialistValue = -1;
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	if (bGetWorst)
		iBestSpecialistValue = MAX_INT;
#endif
#ifndef NQM_PRUNING
	int iBestUnmodifiedSpecialistValue = -1;
#endif

	SpecialistTypes eSpecialist;
	int iValue;

	// Loop through all Buildings
#ifdef AUI_WARNING_FIXES
	for (uint iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#else
	for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#endif
	{
		const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);

		if(pkBuildingInfo)
		{
			// Have this Building in the City?
			if(GetCity()->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
			{
				// Can't add more than the max
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
				if ((!bIsWorked && IsCanAddSpecialistToBuilding(eBuilding)) || (bIsWorked &&  GetNumSpecialistsInBuilding(eBuilding) > 0))
#else
				if(IsCanAddSpecialistToBuilding(eBuilding))
#endif
				{
					eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
					if (eSpecialist == eIgnoreSpecialist)
						continue;
#endif

#ifdef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS
					iValue = GetSpecialistValue(eSpecialist, bIsWorked);
#else
					iValue = GetSpecialistValue(eSpecialist);
#endif

#ifndef NQM_PRUNING
					// Add a bit more weight to a Building if it has more slots (10% per).  This will bias the AI to fill a single building over spreading Specialists out
					int iTemp = ((GetNumSpecialistsAllowedByBuilding(*pkBuildingInfo) - 1) * iValue * 10);
					iTemp /= 100;
					iValue += iTemp;
#endif

#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
					if (((iValue > iBestSpecialistValue) && !bGetWorst) || ((iValue < iBestSpecialistValue) && bGetWorst))
#else
					if(iValue > iBestSpecialistValue)
#endif
					{
						eBestBuilding = eBuilding;
						iBestSpecialistValue = iValue;
#ifndef NQM_PRUNING
						iBestUnmodifiedSpecialistValue = iValue - iTemp;
#endif
					}
				}
			}
		}
	}

#if defined(NQM_PRUNING) && defined(AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST)
	if (iSpecialistValue)
		*iSpecialistValue = iBestSpecialistValue;
#elif defined(AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST)
	if (iSpecialistValue)
		*iSpecialistValue = iBestUnmodifiedSpecialistValue;
#elif defined(NQM_PRUNING)
	iSpecialistValue = iBestSpecialistValue;
#else
	iSpecialistValue = iBestUnmodifiedSpecialistValue;
#endif
	return eBestBuilding;
}

/// How valuable is eSpecialist?
#if defined(AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS) || defined(AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER) || defined(AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION) || defined(AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS) || defined(AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT)
int CvCityCitizens::GetSpecialistValue(SpecialistTypes eSpecialist, bool bForRemoval) const
#elif defined(AUI_CONSTIFY)
int CvCityCitizens::GetSpecialistValue(SpecialistTypes eSpecialist) const
#else
int CvCityCitizens::GetSpecialistValue(SpecialistTypes eSpecialist)
#endif
{

	CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
	if (pSpecialistInfo == NULL)
	{
		//This function should NEVER be called with an invalid specialist info type.
		CvAssert(pSpecialistInfo);
		return 0;
	}

	int iValue = 0;

	CvPlayer* pPlayer = m_pCity->GetPlayer();

	// factor in the fact that specialists may need less food
#ifdef AUI_CITIZENS_FIX_SPECIALIST_VALUE_HALF_FOOD_CONSUMPTION
	int iFoodConsumptionBonus = (pPlayer->isHalfSpecialistFood() && eSpecialist != (SpecialistTypes)GC.getDEFAULT_SPECIALIST()) ? GC.getFOOD_CONSUMPTION_PER_POPULATION() / 2 : 0;
#else
	int iFoodConsumptionBonus = (pPlayer->isHalfSpecialistFood()) ? 1 : 0;
#endif

#ifdef AUI_CITIZENS_GET_SPECIALIST_VALUE_ACCOUNT_FOR_GURUSHIP
	const CvReligion* pReligion = NULL;
	const CvBeliefEntry* pSecondaryBelief = NULL;
	ReligionTypes eMajority = m_pCity->GetCityReligions()->GetReligiousMajority();
	if (eMajority != NO_RELIGION && GetTotalSpecialistCount() == (bForRemoval ? 1 : 0))
	{
		pReligion = GC.getGame().GetGameReligions()->GetReligion(eMajority, GetPlayer()->GetID());
		BeliefTypes eSecondaryPantheon = m_pCity->GetCityReligions()->GetSecondaryReligionPantheonBelief();
		if (eSecondaryPantheon != NO_BELIEF)
			pSecondaryBelief = GC.GetGameBeliefs()->GetEntry(eSecondaryPantheon);
	}
#endif

	// Yield Values
#ifdef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
	int iFoodYieldValue = /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
#else
	int iFoodYieldValue = (GC.getAI_CITIZEN_VALUE_FOOD() * (pPlayer->specialistYield(eSpecialist, YIELD_FOOD) + iFoodConsumptionBonus));
#endif
	int iProductionYieldValue = (GC.getAI_CITIZEN_VALUE_PRODUCTION() * pPlayer->specialistYield(eSpecialist, YIELD_PRODUCTION));
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
	int iGoldYieldValue = (pPlayer->specialistYield(eSpecialist, YIELD_GOLD));
	int iScienceYieldValue = (pPlayer->specialistYield(eSpecialist, YIELD_SCIENCE));
#else
	int iGoldYieldValue = (GC.getAI_CITIZEN_VALUE_GOLD() * pPlayer->specialistYield(eSpecialist, YIELD_GOLD));
	int iScienceYieldValue = (GC.getAI_CITIZEN_VALUE_SCIENCE() * pPlayer->specialistYield(eSpecialist, YIELD_SCIENCE));
#endif
	int iCultureYieldValue = (GC.getAI_CITIZEN_VALUE_CULTURE() * m_pCity->GetCultureFromSpecialist(eSpecialist)); 
	int iFaithYieldValue = (GC.getAI_CITIZEN_VALUE_FAITH() * pPlayer->specialistYield(eSpecialist, YIELD_FAITH));
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	int iGoldenAgePointsYieldValue = (GC.getAI_CITIZEN_VALUE_GOLDEN_AGE_POINTS() * pPlayer->specialistYield(eSpecialist, YIELD_GOLDEN_AGE_POINTS));
#endif
#ifdef AUI_CITIZENS_GET_SPECIALIST_VALUE_ACCOUNT_FOR_GURUSHIP
	if (pReligion)
	{
		iProductionYieldValue += GC.getAI_CITIZEN_VALUE_PRODUCTION() * pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_PRODUCTION);
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
		iGoldYieldValue += pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_GOLD);
		iScienceYieldValue += pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_SCIENCE);
#else
		iGoldYieldValue += GC.getAI_CITIZEN_VALUE_GOLD() * pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_GOLD);
		iScienceYieldValue += GC.getAI_CITIZEN_VALUE_SCIENCE() * pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_SCIENCE);
#endif
		iCultureYieldValue += GC.getAI_CITIZEN_VALUE_CULTURE() * pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_CULTURE);
		iFaithYieldValue += GC.getAI_CITIZEN_VALUE_FAITH() * pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_FAITH);
	}
	if (pSecondaryBelief)
	{
		iProductionYieldValue += GC.getAI_CITIZEN_VALUE_PRODUCTION() * pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_PRODUCTION);
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
		iGoldYieldValue += pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_GOLD);
		iScienceYieldValue += pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_SCIENCE);
#else
		iGoldYieldValue += GC.getAI_CITIZEN_VALUE_GOLD() * pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_GOLD);
		iScienceYieldValue += GC.getAI_CITIZEN_VALUE_SCIENCE() * pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_SCIENCE);
#endif
		iCultureYieldValue += GC.getAI_CITIZEN_VALUE_CULTURE() * pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_CULTURE);
		iFaithYieldValue += GC.getAI_CITIZEN_VALUE_FAITH() * pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_FAITH);
	}
#endif
	int iGPPYieldValue = pSpecialistInfo->getGreatPeopleRateChange() * 3; // TODO: un-hardcode this
#ifdef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS
	int iHappinessYieldValue = 0;
	int iExtraUnhappinessT100 = 0;
	if (pPlayer->isHalfSpecialistUnhappiness() && eSpecialist != (SpecialistTypes)GC.getDEFAULT_SPECIALIST())
	{
		iExtraUnhappinessT100 = -GC.getUNHAPPINESS_PER_POPULATION() * 50;
		// To account for rounding up of halved unhappiness
		if ((GetTotalSpecialistCount() % 2 == 0) != bForRemoval)
			iExtraUnhappinessT100 *= 2;
		if (pPlayer->GetCapitalUnhappinessMod() != 0 && m_pCity->isCapital())
		{
			iExtraUnhappinessT100 *= (100 + pPlayer->GetCapitalUnhappinessMod());
			iExtraUnhappinessT100 /= 100;
		}
		iExtraUnhappinessT100 *= (100 + pPlayer->GetUnhappinessMod());
		iExtraUnhappinessT100 /= 100;
		iExtraUnhappinessT100 *= (100 + pPlayer->GetPlayerTraits()->GetPopulationUnhappinessModifier());
		iExtraUnhappinessT100 /= 100;
		// Handicap mod
		iExtraUnhappinessT100 *= pPlayer->getHandicapInfo().getPopulationUnhappinessMod();
		iExtraUnhappinessT100 /= 100;

		// The more happiness we have, the less it's worth
		// Numbers below are based on Primitive function of f = 2^(1-(Empire Happiness)/10) -> F = -20/ln(2) * 2^(-(Empire Happiness)/10)
		double dHappinessPre = double(pPlayer->GetExcessHappiness());
		if (bForRemoval)
			dHappinessPre += double(iExtraUnhappinessT100) / 100.0;
		double dHappinessPost = dHappinessPre - double(iExtraUnhappinessT100) / 100.0;
		double dHappinessYieldValuePre = pow(2.0, dHappinessPre / -10.0) * -20 / M_LN2;
		double dHappinessYieldValuePost = pow(2.0, dHappinessPost / -10.0) * -20 / M_LN2;
		iHappinessYieldValue = int(AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS * (dHappinessYieldValuePost - dHappinessYieldValuePre) + 0.5);

		iHappinessYieldValue *= -iExtraUnhappinessT100;
		iHappinessYieldValue /= 100;
		if (bForRemoval)
			iExtraUnhappinessT100 *= -1;
	}
#else
	int iHappinessYieldValue = (m_pCity->GetPlayer()->isHalfSpecialistUnhappiness()) ? 5 : 0; // TODO: un-hardcode this
	iHappinessYieldValue = m_pCity->GetPlayer()->IsEmpireUnhappy() ? iHappinessYieldValue * 2 : iHappinessYieldValue; // TODO: un-hardcode this
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
#ifndef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
	iFoodYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FOOD, 0, NULL, -iExtraUnhappinessT100 / 100);
#endif
	iProductionYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_PRODUCTION, 0, NULL, -iExtraUnhappinessT100 / 100);
	iGoldYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_GOLD, 0, NULL, -iExtraUnhappinessT100 / 100);
	iScienceYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_SCIENCE, 0, NULL, -iExtraUnhappinessT100 / 100);
	iCultureYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_CULTURE, 0, NULL, -iExtraUnhappinessT100 / 100);
	iFaithYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FAITH, 0, NULL, -iExtraUnhappinessT100 / 100);
#else
#ifndef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
	iFoodYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FOOD);
#endif
	iProductionYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_PRODUCTION);
	iGoldYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_GOLD);
	iScienceYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_SCIENCE);
	iCultureYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_CULTURE);
	iFaithYieldValue *= m_pCity->getBaseYieldRateModifier(YIELD_FAITH);
#endif

#ifndef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_GREAT_PERSON_POINTS
	int iGPPModifier = 100 + pPlayer->getGreatPeopleRateModifier() + GetCity()->getGreatPeopleRateModifier();
	UnitClassTypes eGPUnitClass = (UnitClassTypes)pSpecialistInfo->getGreatPeopleUnitClass();
	if (eGPUnitClass != NO_UNITCLASS)
	{
		if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_WRITER", true))
		{
			iGPPModifier += pPlayer->getGreatWriterRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_ARTIST", true))
		{
			iGPPModifier += pPlayer->getGreatArtistRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_MUSICIAN", true))
		{
			iGPPModifier += pPlayer->getGreatMusicianRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_SCIENTIST", true))
		{
			iGPPModifier += pPlayer->getGreatScientistRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_MERCHANT", true))
		{
			iGPPModifier += pPlayer->getGreatMerchantRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_ENGINEER", true))
		{
			iGPPModifier += pPlayer->getGreatEngineerRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_GREAT_GENERAL", true))
		{
			iGPPModifier += pPlayer->getGreatGeneralRateModifier();
		}
		else if (eGPUnitClass == (UnitClassTypes)GC.getInfoTypeForString("UNITCLASS_GREAT_ADMIRAL", true))
		{
			iGPPModifier += pPlayer->getGreatAdmiralRateModifier();
		}
	}
	iGPPYieldValue *= iGPPModifier;
#endif
#endif
#ifdef AUI_CITIZENS_GOLD_YIELD_COUNTS_AS_SCIENCE_WHEN_IN_DEFICIT
	int iCurrentScienceLoss = -GetPlayer()->calculateGoldRateTimes100() - GetPlayer()->GetTreasury()->GetGoldTimes100();
	if (bForRemoval)
		iCurrentScienceLoss += iGoldYieldValue;
	if (iCurrentScienceLoss > 0)
	{
		int iGoldToScienceT100 = MIN(iGoldYieldValue, iCurrentScienceLoss);
		iGoldYieldValue -= iGoldToScienceT100;
		iScienceYieldValue += iGoldToScienceT100;
	}
	iGoldYieldValue *= GC.getAI_CITIZEN_VALUE_GOLD();
	iScienceYieldValue *= GC.getAI_CITIZEN_VALUE_SCIENCE();
#endif

	// How much surplus food are we making?
	int iExcessFoodTimes100 = m_pCity->getYieldRateTimes100(YIELD_FOOD, false) - (m_pCity->foodConsumption() * 100);
#if defined(AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER) || defined(AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION) || defined(AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS)
	int iSpecialistBaseFoodYield = pPlayer->specialistYield(eSpecialist, YIELD_FOOD);
#ifdef AUI_CITIZENS_GET_SPECIALIST_VALUE_ACCOUNT_FOR_GURUSHIP
	if (pReligion)
		iSpecialistBaseFoodYield += pReligion->m_Beliefs.GetYieldChangeAnySpecialist(YIELD_FOOD) * 100;
	if (pSecondaryBelief)
		iSpecialistBaseFoodYield += pSecondaryBelief->GetYieldChangeAnySpecialist(YIELD_FOOD) * 100;
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
	iSpecialistBaseFoodYield *= m_pCity->getBaseYieldRateModifier(YIELD_FOOD, 0, NULL, -iExtraUnhappinessT100 / 100);
#else
	iSpecialistBaseFoodYield *= m_pCity->getBaseYieldRateModifier(YIELD_FOOD);
#endif
#endif
	if (bForRemoval)
		iExcessFoodTimes100 -= iSpecialistBaseFoodYield + iFoodConsumptionBonus;
	int iExcessFoodWithPlotTimes100 = iSpecialistBaseFoodYield + iExcessFoodTimes100 + iFoodConsumptionBonus;
#endif
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
	// Excess Food bit is to make sure we don't starve to death trying to allocate specialists when we're unhappy
	if (iExcessFoodWithPlotTimes100 >= 0 && (pPlayer->GetExcessHappiness() - iExtraUnhappinessT100 / 100 >= 0) != (pPlayer->GetExcessHappiness() >= 0))
	{
		int iBonusScience = GetPlayer()->GetScienceFromHappinessTimes100(true);
		if (pPlayer->GetExcessHappiness() >= 0)
			iBonusScience *= -1;
		iScienceYieldValue += GC.getAI_CITIZEN_VALUE_SCIENCE() * iBonusScience / GetPlayer()->getNumCities();
	}
	// Happiness to culture already multiplied by 100
	if (pPlayer->GetExcessHappiness() + -iExtraUnhappinessT100 / 100 >= 0)
		iCultureYieldValue += GC.getAI_CITIZEN_VALUE_CULTURE() * (-iExtraUnhappinessT100 / 100) * GetPlayer()->getHappinessToCulture();
#endif

	bool bAvoidGrowth = IsAvoidGrowth();

	// City Focus
	CityAIFocusTypes eFocus = GetFocusType();
	if(eFocus == CITY_AI_FOCUS_TYPE_FOOD)
		iFoodYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_PRODUCTION)
		iProductionYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_GOLD)
		iGoldYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_SCIENCE)
		iScienceYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_CULTURE)
		iCultureYieldValue *= 3;
	else if(eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
	{
		iFoodYieldValue *= 2;
		iGoldYieldValue *= 2;
	}
	else if(eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
	{
		iFoodYieldValue *= 2;
		iProductionYieldValue *= 2;
	}
	else if(eFocus == CITY_AI_FOCUS_TYPE_FAITH)
	{
		iFaithYieldValue *= 3;
	}
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	else if (eFocus == CITY_AI_FOCUS_TYPE_GOLDEN_AGE_POINTS)
	{
		iGoldenAgePointsYieldValue *= 3;
	}
#endif
	else if(eFocus == CITY_AI_FOCUS_TYPE_GREAT_PEOPLE)
	{
		iGPPYieldValue *= 3;
	}

#ifdef AUI_CITIZENS_GET_VALUE_ALTER_FOOD_VALUE_IF_FOOD_PRODUCTION
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	if (!getIgnoreFoodProduction() && m_pCity->isFoodProduction())
#else
	if (m_pCity->isFoodProduction())
#endif
	{
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
		iFoodYieldValue = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100, -iExtraUnhappinessT100 / 100) - m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100, -iExtraUnhappinessT100 / 100);
		int iDummy = m_pCity->GetFoodProduction(iExcessFoodTimes100 / 100);
		int iCurrProdFromFood = m_pCity->foodDifference(true, &iDummy, -iExtraUnhappinessT100 / 100);
		iDummy = m_pCity->GetFoodProduction(iExcessFoodWithPlotTimes100 / 100);
		int iProdFromFoodWithTile = m_pCity->foodDifference(true, &iDummy, -iExtraUnhappinessT100 / 100);
#else
		iFoodYieldValue = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100) - m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		int iDummy = m_pCity->GetFoodProduction(iExcessFoodTimes100 / 100);
		int iCurrProdFromFood = m_pCity->foodDifference(true, &iDummy);
		iDummy = m_pCity->GetFoodProduction(iExcessFoodWithPlotTimes100 / 100);
		int iProdFromFoodWithTile = m_pCity->foodDifference(true, &iDummy);
#endif
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		iFoodYieldValue = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
		int iCurrProdFromFood = m_pCity->GetFoodProduction(iExcessFoodTimes100 / 100);
		int iProdFromFoodWithTile = m_pCity->GetFoodProduction(iExcessFoodWithPlotTimes100 / 100);
#endif
		iProductionYieldValue += (iProdFromFoodWithTile - iCurrProdFromFood) * GC.getAI_CITIZEN_VALUE_PRODUCTION() * (eFocus == CITY_AI_FOCUS_TYPE_PRODUCTION ? 3 : 1);
	}
	else
#endif
	// Food can be worth less if we don't want to grow
	if(iExcessFoodTimes100 >= 0 && bAvoidGrowth)
	{
#ifdef AUI_CITIZENS_AVOID_GROWTH_STILL_VALUES_EXCESS_FOOD
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100, -iExtraUnhappinessT100 / 100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100, -iExtraUnhappinessT100 / 100);
#else
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100);
#endif
#endif
		iFoodYieldValue = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		// If we at least have enough Food to feed everyone, zero out the value of additional food
		iFoodYieldValue = 0;
#endif
	}
	// We want to grow here
	else
	{
#ifdef AUI_CITIZENS_GET_VALUE_SPLIT_EXCESS_FOOD_MUTLIPLIER
		iFoodYieldValue *= 8;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
#ifdef AUI_CITIZENS_CONSIDER_HAPPINESS_VALUE_ON_OTHER_YIELDS
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100, -iExtraUnhappinessT100 / 100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100, -iExtraUnhappinessT100 / 100);
#else
		iExcessFoodTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodTimes100);
		iExcessFoodWithPlotTimes100 = m_pCity->foodDifferenceTimes100(true, NULL, &iExcessFoodWithPlotTimes100);
#endif
#endif
#ifdef AUI_CITIZENS_AVOID_GROWTH_STILL_VALUES_EXCESS_FOOD
		int iExcessFoodYieldValue = 1;
#else
		int iExcessFoodYieldValue = 0;
#endif
		int iTargetFoodT100 = 0;
		if (!bAvoidGrowth)
		{
			iExcessFoodYieldValue = iFoodYieldValue / 16;
#ifdef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
			if (eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_FOOD || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH
				|| eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || m_pCity->getPopulation() < 2)
#else
			if (eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_FOOD || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
#endif
			{
				iTargetFoodT100 = 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION();
			}
#ifdef AUI_CITIZENS_PUPPET_STILL_WANTS_GROWTH
			else if (m_pCity->IsPuppet())
				iTargetFoodT100 = 50 * GC.getFOOD_CONSUMPTION_PER_POPULATION();
#endif
		}

		int iNonExcessFoodPlotYieldT100 = iExcessFoodWithPlotTimes100 - iExcessFoodTimes100;
		int iExcessFoodPlotYieldT100 = iNonExcessFoodPlotYieldT100;
		if (iExcessFoodWithPlotTimes100 <= iTargetFoodT100)
		{
			iExcessFoodPlotYieldT100 = 0;
		}
		else if (iExcessFoodTimes100 >= iTargetFoodT100)
		{
			iNonExcessFoodPlotYieldT100 = 0;
		}
		else
		{
			iNonExcessFoodPlotYieldT100 = iTargetFoodT100 - iExcessFoodTimes100;
			iExcessFoodPlotYieldT100 = iExcessFoodWithPlotTimes100 - iTargetFoodT100;
		}

		iFoodYieldValue *= iNonExcessFoodPlotYieldT100;
		iFoodYieldValue += (iExcessFoodPlotYieldT100 * iExcessFoodYieldValue);
#ifndef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
		iFoodYieldValue /= 100;
#endif
#else
		// If we have a non-default and non-food focus, only worry about getting to 0 food
#ifdef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
		if (eFocus != NO_CITY_AI_FOCUS_TYPE && eFocus != CITY_AI_FOCUS_TYPE_FOOD && eFocus != CITY_AI_FOCUS_TYPE_PROD_GROWTH && eFocus != CITY_AI_FOCUS_TYPE_GOLD_GROWTH && m_pCity->getPopulation() >= 5)
#else
		if(eFocus != NO_CITY_AI_FOCUS_TYPE && eFocus != CITY_AI_FOCUS_TYPE_FOOD && eFocus != CITY_AI_FOCUS_TYPE_PROD_GROWTH && eFocus != CITY_AI_FOCUS_TYPE_GOLD_GROWTH)
#endif
		{
			int iFoodT100NeededFor0 = -iExcessFoodTimes100;

			if(iFoodT100NeededFor0 > 0)
			{
				iFoodYieldValue *= 8;
			}
			else
			{
				iFoodYieldValue /= 2;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				int iExtraFoodValueT100 = (m_pCity->foodDifferenceTimes100(true, NULL, true, iExcessFoodWithPlotTimes100 - iExcessFoodTimes100) -
					(iExcessFoodWithPlotTimes100 - iExcessFoodTimes100)) * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
				if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
					iExtraFoodValueT100 *= 3;
				else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
					iExtraFoodValueT100 *= 2;
				iFoodYieldValue += iExtraFoodValueT100 / 200;
#endif
			}
		}
		// If our surplus is not at least 2, really emphasize food plots
		else if(!bAvoidGrowth)
		{
#ifdef AUI_CITIZENS_FIX_SPECIALIST_VALUE_HALF_FOOD_CONSUMPTION
			int iFoodT100NeededFor2 = 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION() - iExcessFoodTimes100;
#else
			int iFoodT100NeededFor2 = 200 - iExcessFoodTimes100;
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
			int iExtraFoodValueT100 = (m_pCity->foodDifferenceTimes100(true, NULL, true, iExcessFoodWithPlotTimes100 - iExcessFoodTimes100) -
				(iExcessFoodWithPlotTimes100 - iExcessFoodTimes100)) * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
			if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
				iExtraFoodValueT100 *= 3;
			else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
				iExtraFoodValueT100 *= 2;
#endif

			if(iFoodT100NeededFor2 > 0)
			{
				iFoodYieldValue *= 8;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				iExtraFoodValueT100 = 0;
				iExtraFoodValueT100 += (m_pCity->foodDifferenceTimes100(true, NULL, true, MIN(iExcessFoodWithPlotTimes100, 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION())) -
					MIN(iExcessFoodWithPlotTimes100, 100 * GC.getFOOD_CONSUMPTION_PER_POPULATION())) * 8 * /*12*/ GC.getAI_CITIZEN_VALUE_FOOD();
				if (eFocus == CITY_AI_FOCUS_TYPE_FOOD)
					iExtraFoodValueT100 *= 3;
				else if (eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH)
					iExtraFoodValueT100 *= 2;
				iFoodYieldValue += iExtraFoodValueT100 / 100;
#endif
			}
			else if (eFocus != CITY_AI_FOCUS_TYPE_FOOD)
			{
				iFoodYieldValue /= 2;
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_GROWTH_MODIFIERS
				iFoodYieldValue += iExtraFoodValueT100 / 200;
			}
			else
			{
				iFoodYieldValue += iExtraFoodValueT100 / 100;
#endif
			}
		}
#ifdef AUI_CITIZENS_FIX_GET_VALUE_FOOD_YIELD_VALUE_WHEN_STARVATION_WITH_AVOID_GROWTH
		// Food focus and negative food, but with avoid growth enabled for some reason
		else
		{
			iFoodYieldValue *= 8;
		}
#endif
#endif
	}

#ifndef AUI_CITIZENS_LOW_POPULATION_CITIES_USE_2MIN_NOT_4X_FOOD
	if((eFocus == NO_CITY_AI_FOCUS_TYPE || eFocus == CITY_AI_FOCUS_TYPE_PROD_GROWTH || eFocus == CITY_AI_FOCUS_TYPE_GOLD_GROWTH) && !bAvoidGrowth && m_pCity->getPopulation() < 5)
	{
		iFoodYieldValue *= 4;
	}
#endif

	iValue += iFoodYieldValue;
	iValue += iProductionYieldValue;
	iValue += iGoldYieldValue;
	iValue += iScienceYieldValue;
	iValue += iCultureYieldValue;
	iValue += iFaithYieldValue;
#ifndef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_GREAT_PERSON_POINTS
	iValue += iGPPYieldValue;
#endif
#ifdef AUI_CITIZENS_GET_VALUE_CONSIDER_YIELD_RATE_MODIFIERS
	iValue /= 100;
#endif
#ifdef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_GREAT_PERSON_POINTS
	iValue += iGPPYieldValue;
#endif
	iValue += iHappinessYieldValue;
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	iValue += iGoldenAgePointsYieldValue;
	#endif

	return iValue;
}

/// Determine if eSpecialist is preferable to a default specialist, based on our focus
#ifndef AUI_CITIZENS_IS_BETTER_THAN_DEFAULT_SPECIALIST_USE_REGULAR_VALUES
#ifdef AUI_CONSTIFY
bool CvCityCitizens::IsBetterThanDefaultSpecialist(SpecialistTypes eSpecialist) const
#else
bool CvCityCitizens::IsBetterThanDefaultSpecialist(SpecialistTypes eSpecialist)
#endif
{
	CvSpecialistInfo* pSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
	CvAssertMsg(pSpecialistInfo, "Invalid specialist type when assigning citizens. Please send Anton your save file and version.");
	if(!pSpecialistInfo) return false; // Assumes that default specialist will work out

	SpecialistTypes eDefaultSpecialist = (SpecialistTypes) GC.getDEFAULT_SPECIALIST();
	CvSpecialistInfo* pDefaultSpecialistInfo = GC.getSpecialistInfo(eDefaultSpecialist);
	CvAssertMsg(pDefaultSpecialistInfo, "Invalid default specialist type when assigning citizens. Please send Anton your save file and version.");
	if(!pDefaultSpecialistInfo) return false;

	//antonjs: consider: deficient yield

	CityAIFocusTypes eFocus = GetFocusType();
	YieldTypes eYield = NO_YIELD;
	switch (eFocus)
	{
	case CITY_AI_FOCUS_TYPE_FOOD:
		eYield = YIELD_FOOD;
		break;
	case CITY_AI_FOCUS_TYPE_PRODUCTION:
		eYield = YIELD_PRODUCTION;
		break;
	case CITY_AI_FOCUS_TYPE_GOLD:
		eYield = YIELD_GOLD;
		break;
	case CITY_AI_FOCUS_TYPE_GREAT_PEOPLE:
		eYield = NO_YIELD;
		break;
	case CITY_AI_FOCUS_TYPE_SCIENCE:
		eYield = YIELD_SCIENCE;
		break;
	case CITY_AI_FOCUS_TYPE_CULTURE:
		eYield = YIELD_CULTURE;
		break;
	case CITY_AI_FOCUS_TYPE_PROD_GROWTH:
	case CITY_AI_FOCUS_TYPE_GOLD_GROWTH:
		eYield = YIELD_FOOD;
		break;
	case CITY_AI_FOCUS_TYPE_FAITH:
		eYield = YIELD_FAITH;
		break;
#if defined(LEKMOD_v34) //Support for the New Golden Age Points yield
	case CITY_AI_FOCUS_TYPE_GOLDEN_AGE_POINTS:
		eYield = YIELD_GOLDEN_AGE_POINTS;
		break;
#endif
	default:
		eYield = NO_YIELD;
		break;
	}

	if (eYield == NO_YIELD)
		return true;

	int iSpecialistYield = pSpecialistInfo->getYieldChange(eYield);
	int iDefaultSpecialistYield = pDefaultSpecialistInfo->getYieldChange(eYield);

	if (m_pCity->GetPlayer()->isHalfSpecialistUnhappiness() || m_pCity->GetPlayer()->isHalfSpecialistFood())
	{
		iSpecialistYield *= 2;
	}

	return (iSpecialistYield >= iDefaultSpecialistYield); // Unless default Specialist has strictly more, this Specialist is better
}
#endif

/// How many Citizens need to be given a job?
int CvCityCitizens::GetNumUnassignedCitizens() const
{
	return m_iNumUnassignedCitizens;
}

/// Changes how many Citizens need to be given a job
void CvCityCitizens::ChangeNumUnassignedCitizens(int iChange)
{
	m_iNumUnassignedCitizens += iChange;
	CvAssert(m_iNumUnassignedCitizens >= 0);
}

/// How many Citizens are working Plots?
int CvCityCitizens::GetNumCitizensWorkingPlots() const
{
	return m_iNumCitizensWorkingPlots;
}

/// Changes how many Citizens are working Plots
void CvCityCitizens::ChangeNumCitizensWorkingPlots(int iChange)
{
	m_iNumCitizensWorkingPlots += iChange;
}

/// Pick the best Plot to work from one of our unassigned pool
#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
bool CvCityCitizens::DoAddBestCitizenFromUnassigned(int* piBestScore)
#else
bool CvCityCitizens::DoAddBestCitizenFromUnassigned()
#endif
{
	// We only assign the unassigned here, folks
	if (GetNumUnassignedCitizens() == 0)
	{
		return false;
	}

	// First Specialist Pass
	int iSpecialistValue = 0;
	BuildingTypes eBestSpecialistBuilding = NO_BUILDING;
	if (!IsNoAutoAssignSpecialists())
	{
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
		eBestSpecialistBuilding = GetAIBestSpecialistBuilding(&iSpecialistValue);
#else
		eBestSpecialistBuilding = GetAIBestSpecialistBuilding(iSpecialistValue);
#endif
	}

#if defined(AUI_CITIZENS_IS_PLOT_BETTER_THAN_DEFAULT_SPECIALIST) || defined(AUI_CITIZENS_SELF_CONSISTENCY_CHECK)
	int iDefaultSpecialistValue = GetSpecialistValue((SpecialistTypes)GC.getDEFAULT_SPECIALIST());
#endif

#ifdef AUI_CITIZENS_IS_BETTER_THAN_DEFAULT_SPECIALIST_USE_REGULAR_VALUES
	bool bBetterThanSlacker = eBestSpecialistBuilding != NO_BUILDING && iSpecialistValue >= iDefaultSpecialistValue;
#else
	bool bBetterThanSlacker = false;
	if (eBestSpecialistBuilding != NO_BUILDING)
	{
		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBestSpecialistBuilding);
		if(pkBuildingInfo)
		{
			SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
			// Must not be worse than a default Specialist for our focus!
			if (IsBetterThanDefaultSpecialist(eSpecialist))
			{
				bBetterThanSlacker = true;
			}
		}
	}
#endif

	int iBestPlotValue = 0;
	CvPlot* pBestPlot = GetBestCityPlotWithValue(iBestPlotValue, /*bBest*/ true, /*bWorked*/ false);

	bool bSpecialistBetterThanPlot = (eBestSpecialistBuilding != NO_BUILDING && iSpecialistValue >= iBestPlotValue);

	// Is there a Specialist we can assign?
	if (bSpecialistBetterThanPlot && bBetterThanSlacker)
	{
		DoAddSpecialistToBuilding(eBestSpecialistBuilding, /*bForced*/ false);
#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
		if (piBestScore)
			*piBestScore = iSpecialistValue;
#endif
		return true;
	}
	// Found a Valid Plot to place a guy?
#ifdef AUI_CITIZENS_IS_PLOT_BETTER_THAN_DEFAULT_SPECIALIST
	else if (pBestPlot != NULL && iBestPlotValue >= iDefaultSpecialistValue)
#else
	else if (!bSpecialistBetterThanPlot && pBestPlot != NULL)
#endif
	{
		// Now assign the guy to the best possible Plot
		SetWorkingPlot(pBestPlot, true);
#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
		if (piBestScore)
			*piBestScore = iBestPlotValue;
#endif
		return true;
	}
	// No Valid Plots left - and no good specialists
	else
	{
#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
		if (piBestScore)
			*piBestScore = iDefaultSpecialistValue;
#else
		CvPlayer* pOwner = &GET_PLAYER(GetOwner());
		CvAssertMsg(pOwner, "Could not find owner of city when assigning citizens. Please send Anton your save file and version.");

		// Assign a cool Specialist! Only do this for AI players, or humans who do not have manual specialist control set
		if (pOwner)
		{
			if (!GET_PLAYER(GetOwner()).isHuman() || !IsNoAutoAssignSpecialists())
			{
				BuildingTypes eBestBuilding = GetAIBestSpecialistBuilding(iSpecialistValue);
				if(eBestBuilding != NO_BUILDING)
				{
					CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBestBuilding);
					if(pkBuildingInfo)
					{
						SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
						// Must not be worse than a default Specialist for our focus!
						if (IsBetterThanDefaultSpecialist(eSpecialist))
						{
							DoAddSpecialistToBuilding(eBestBuilding, false);
							return true;
						}
					}
				}
			}
		}
#endif

		// Default Specialist if we can't do anything else
		ChangeNumDefaultSpecialists(1);
	}

	return false;
}

/// Pick the worst Plot to stop working
bool CvCityCitizens::DoRemoveWorstCitizen(bool bRemoveForcedStatus, SpecialistTypes eDontChangeSpecialist, int iCurrentCityPopulation)
{
	if (iCurrentCityPopulation == -1)
	{
		iCurrentCityPopulation = GetCity()->getPopulation();
	}

	// Are all of our guys already not working Plots?
	if(GetNumUnassignedCitizens() == GetCity()->getPopulation())
	{
		return false;
	}

#ifndef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	// Find default Specialist to pull off, if there is one
	if(GetNumDefaultSpecialists() > 0)
	{
		// Do we either have unforced default specialists we can remove?
		if(GetNumDefaultSpecialists() > GetNumForcedDefaultSpecialists())
		{
			ChangeNumDefaultSpecialists(-1);
			return true;
		}
		if(GetNumDefaultSpecialists() > iCurrentCityPopulation)
		{
			ChangeNumForcedDefaultSpecialists(-1);
			ChangeNumDefaultSpecialists(-1);
			return true;
		}
	}
#endif

	// No Default Specialists, remove a working Pop, if there is one
	int iWorstPlotValue = 0;
	CvPlot* pWorstPlot = GetBestCityPlotWithValue(iWorstPlotValue, /*bBest*/ false, /*bWorked*/ true);

#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	int iWorstSpecialistValue = 0;
	BuildingTypes eWorstSpecialistBuilding = NO_BUILDING;
	if (!IsNoAutoAssignSpecialists())
	{
		eWorstSpecialistBuilding = GetAIBestSpecialistBuilding(&iWorstSpecialistValue, true, true, eDontChangeSpecialist);
	}

	// Find default Specialist to pull off, if there is one
	if (GetNumDefaultSpecialists() > 0)
	{
		int iDefaultSpecialistScore = GetSpecialistValue((SpecialistTypes)GC.getDEFAULT_SPECIALIST());
		// Do we either have unforced default specialists we can remove?
		if (GetNumDefaultSpecialists() > GetNumForcedDefaultSpecialists() && (eWorstSpecialistBuilding == NO_BUILDING || iDefaultSpecialistScore <= iWorstSpecialistValue) 
			&& (pWorstPlot == NULL || iDefaultSpecialistScore <= iWorstPlotValue))
		{
			ChangeNumDefaultSpecialists(-1);
			return true;
		}
		if (GetNumDefaultSpecialists() > iCurrentCityPopulation && (eWorstSpecialistBuilding == NO_BUILDING || iDefaultSpecialistScore <= iWorstSpecialistValue)
			&& (pWorstPlot == NULL || iDefaultSpecialistScore <= iWorstPlotValue))
		{
			ChangeNumForcedDefaultSpecialists(-1);
			ChangeNumDefaultSpecialists(-1);
			return true;
		}
	}

	if (pWorstPlot != NULL && (eWorstSpecialistBuilding == NO_BUILDING || iWorstPlotValue < iWorstSpecialistValue))
#else
	if(pWorstPlot != NULL)
#endif
	{
		SetWorkingPlot(pWorstPlot, false);

		// If we were force-working this Plot, turn it off
		if(bRemoveForcedStatus)
		{
			if(IsForcedWorkingPlot(pWorstPlot))
			{
				SetForcedWorkingPlot(pWorstPlot, false);
			}
		}

		return true;
	}
	// Have to resort to pulling away a good Specialist
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	else if (eWorstSpecialistBuilding != NO_BUILDING)
	{
		DoRemoveSpecialistFromBuilding(eWorstSpecialistBuilding, false);
		return true;
	}
#else
	else
	{
		if(DoRemoveWorstSpecialist(eDontChangeSpecialist))
		{
			return true;
		}
	}
#endif

	return false;
}

/// Find a Plot the City is either working or not, and the best/worst value for it - this function does "double duty" depending on what the user wants to find
CvPlot* CvCityCitizens::GetBestCityPlotWithValue(int& iValue, bool bWantBest, bool bWantWorked)
{
	bool bPlotForceWorked;

	int iBestPlotValue = -1;
	int iBestPlotID = -1;

	CvPlot* pLoopPlot;

	// Look at all workable Plots
	for(int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
	{
		if(iPlotLoop != CITY_HOME_PLOT)
		{
			pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

			if(pLoopPlot != NULL)
			{
				// Is this a Plot this City controls?
				if(pLoopPlot->getWorkingCity() != NULL && pLoopPlot->getWorkingCity()->GetID() == GetCity()->GetID())
				{
					// Working the Plot and wanting to work it, or Not working it and wanting to find one to work?
					if((IsWorkingPlot(pLoopPlot) && bWantWorked) ||
					        (!IsWorkingPlot(pLoopPlot) && !bWantWorked))
					{
						// Working the Plot or CAN work the Plot?
						if(bWantWorked || IsCanWork(pLoopPlot))
						{
#ifdef AUI_CITIZENS_REALLOCATE_CITIZENS_USES_SELF_CONSISTENCY
							iValue = GetPlotValue(pLoopPlot, true);
#else
							iValue = GetPlotValue(pLoopPlot, bWantBest);
#endif

							bPlotForceWorked = IsForcedWorkingPlot(pLoopPlot);

							if(bPlotForceWorked)
							{
								// Looking for best, unworked Plot: Forced plots are FIRST to be picked
								if(bWantBest && !bWantWorked)
								{
									iValue += 10000;
								}
								// Looking for worst, worked Plot: Forced plots are LAST to be picked, so make it's value incredibly high
								if(!bWantBest && bWantWorked)
								{
									iValue += 10000;
								}
							}

							if(iBestPlotValue == -1 ||							// First Plot?
							        (bWantBest && iValue > iBestPlotValue) ||		// Best Plot so far?
							        (!bWantBest && iValue < iBestPlotValue))			// Worst Plot so far?
							{
								iBestPlotValue = iValue;
								iBestPlotID = iPlotLoop;
							}
						}
					}
				}
			}
		}
	}

	// Passed in by reference
	iValue = iBestPlotValue;

	if(iBestPlotID == -1)
	{
		return NULL;
	}

	return GetCityPlotFromIndex(iBestPlotID);
}

/// Optimize our Citizen Placement
void CvCityCitizens::DoReallocateCitizens()
{
	// Make sure we don't have more forced working plots than we have citizens working.  If so, clean it up before reallocating
	DoValidateForcedWorkingPlots();

	// Remove all of the allocated guys
#ifdef AUI_CITIZENS_FIX_DO_REALLOCATE_CITIZENS_NO_COSTLY_PLOT_REMOVAL
	CvPlot* pLoopPlot;
	for (int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
	{
		if (iPlotLoop != CITY_HOME_PLOT)
		{
			pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

			if (pLoopPlot != NULL)
			{
				if (pLoopPlot->getWorkingCity() != NULL && pLoopPlot->getWorkingCity()->GetID() == GetCity()->GetID())
				{
					SetWorkingPlot(pLoopPlot, false, true);
				}
			}
		}
	}
#else
	int iNumCitizensToRemove = GetNumCitizensWorkingPlots();
	for(int iWorkerLoop = 0; iWorkerLoop < iNumCitizensToRemove; iWorkerLoop++)
	{
		DoRemoveWorstCitizen();
	}
#endif

	int iSpecialistLoop;

#ifdef AUI_CITIZENS_FIX_DO_REALLOCATE_CITIZENS_OBEY_MANUAL_SPECIALIST_CONTROL
	if (!IsNoAutoAssignSpecialists())
	{
#endif
	// Remove Non-Forced Specialists in Buildings
	int iNumSpecialistsToRemove;
	BuildingTypes eBuilding;
#ifdef AUI_WARNING_FIXES
	for (uint iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#else
	for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#endif
	{
		eBuilding = (BuildingTypes) iBuildingLoop;

		// Have this Building in the City?
		if(GetCity()->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
		{
			iNumSpecialistsToRemove = GetNumSpecialistsInBuilding(eBuilding) - GetNumForcedSpecialistsInBuilding(eBuilding);	// Don't include Forced guys

			// Loop through guys to remove (if there are any)
			for(iSpecialistLoop = 0; iSpecialistLoop < iNumSpecialistsToRemove; iSpecialistLoop++)
			{
				DoRemoveSpecialistFromBuilding(eBuilding, /*bForced*/ false);
			}
		}
	}
#ifdef AUI_CITIZENS_FIX_DO_REALLOCATE_CITIZENS_OBEY_MANUAL_SPECIALIST_CONTROL
	}
#endif

	// Remove Default Specialists
	int iNumDefaultsToRemove = GetNumDefaultSpecialists() - GetNumForcedDefaultSpecialists();
	for(iSpecialistLoop = 0; iSpecialistLoop < iNumDefaultsToRemove; iSpecialistLoop++)
	{
		ChangeNumDefaultSpecialists(-1);
	}

	// Now put all of the unallocated guys back
	int iNumToAllocate = GetNumUnassignedCitizens();
	for(int iUnallocatedLoop = 0; iUnallocatedLoop < iNumToAllocate; iUnallocatedLoop++)
	{
		DoAddBestCitizenFromUnassigned();
	}

#ifdef AUI_CITIZENS_REALLOCATE_CITIZENS_USES_SELF_CONSISTENCY
	DoSelfConsistencyCheck();
#elif defined(AUI_CITIZENS_FIX_DO_REALLOCATE_CITIZENS_NO_COSTLY_PLOT_REMOVAL)
	ICvUserInterface2* pkIFace = GC.GetEngineUserInterface();
	pkIFace->setDirty(GameData_DIRTY_BIT, true);
	pkIFace->setDirty(CityInfo_DIRTY_BIT, true);
	//pkIFace->setDirty(InfoPane_DIRTY_BIT, true );
	pkIFace->setDirty(CityScreen_DIRTY_BIT, true);
	pkIFace->setDirty(ColoredPlots_DIRTY_BIT, true);
#endif
}

#ifdef AUI_CITIZENS_SELF_CONSISTENCY_CHECK
void CvCityCitizens::DoSelfConsistencyCheck(int iMaxIterations)
{
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	setIgnoreFoodProduction(false);
#endif
	int iLoopScore = 0;
	int iLastLoopScore = 0;
	if (iMaxIterations < 1)
	{
		iMaxIterations = m_pCity->getPopulation();
		iMaxIterations += 1 - iMaxIterations % 2;
	}

	for (int iI = 0; iI < iMaxIterations; iI++)
	{
		iLastLoopScore = iLoopScore;

		DoRemoveWorstCitizen();

		DoAddBestCitizenFromUnassigned(&iLoopScore);

		if (abs(iLoopScore - iLastLoopScore) <= AUI_CITIZENS_SELF_CONSISTENCY_CHECK)
			break;
	}
#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
	setIgnoreFoodProduction(true);
#endif
	ICvUserInterface2* pkIFace = GC.GetEngineUserInterface();
	pkIFace->setDirty(GameData_DIRTY_BIT, true);
	pkIFace->setDirty(CityInfo_DIRTY_BIT, true);
	//pkIFace->setDirty(InfoPane_DIRTY_BIT, true );
	pkIFace->setDirty(CityScreen_DIRTY_BIT, true);
	pkIFace->setDirty(ColoredPlots_DIRTY_BIT, true);
}
#endif



///////////////////////////////////////////////////
// Worked Plots
///////////////////////////////////////////////////



/// Is our City working a CvPlot?
bool CvCityCitizens::IsWorkingPlot(const CvPlot* pPlot) const
{
	int iIndex;

	iIndex = GetCityIndexFromPlot(pPlot);

	if(iIndex != -1)
	{
		return m_pabWorkingPlot[iIndex];
	}

	return false;
}

/// Tell a City to start or stop working a Plot.  Citizens will go to/from the Unassigned Pool if the 3rd argument is true
void CvCityCitizens::SetWorkingPlot(CvPlot* pPlot, bool bNewValue, bool bUseUnassignedPool)
{
	int iI;

	int iIndex = GetCityIndexFromPlot(pPlot);

	CvAssertMsg(iIndex >= 0, "iIndex expected to be >= 0");
	CvAssertMsg(iIndex < NUM_CITY_PLOTS, "iIndex expected to be < NUM_CITY_PLOTS");

	if(IsWorkingPlot(pPlot) != bNewValue && iIndex >= 0 && iIndex < NUM_CITY_PLOTS)
	{
		m_pabWorkingPlot[iIndex] = bNewValue;

		// Don't look at the center Plot of a City, because we always work it for free
		if(iIndex != CITY_HOME_PLOT)
		{
			// Alter the count of Plots being worked by Citizens
			if(bNewValue)
			{
				ChangeNumCitizensWorkingPlots(1);

				if(bUseUnassignedPool)
				{
					ChangeNumUnassignedCitizens(-1);
				}
			}
			else
			{
				ChangeNumCitizensWorkingPlots(-1);

				if(bUseUnassignedPool)
				{
					ChangeNumUnassignedCitizens(1);
				}
			}
		}

		if(pPlot != NULL)
		{
			// investigate later
			//CvAssertMsg(pPlot->getWorkingCity() == GetCity(), "WorkingCity is expected to be this");

			// Now working pPlot
			if(IsWorkingPlot(pPlot))
			{
				//if (iIndex != CITY_HOME_PLOT)
				//{
				//	GetCity()->changeWorkingPopulation(1);
				//}

				for(iI = 0; iI < NUM_YIELD_TYPES; iI++)
				{
					GetCity()->ChangeBaseYieldRateFromTerrain(((YieldTypes)iI), pPlot->getYield((YieldTypes)iI));
				}
			}
			// No longer working pPlot
			else
			{
				//if (iIndex != CITY_HOME_PLOT)
				//{
				//	GetCity()->changeWorkingPopulation(-1);
				//}

				for(iI = 0; iI < NUM_YIELD_TYPES; iI++)
				{
					GetCity()->ChangeBaseYieldRateFromTerrain(((YieldTypes)iI), -pPlot->getYield((YieldTypes)iI));
				}
			}
		}

		if(GetCity()->isCitySelected())
		{
			GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
			//GC.GetEngineUserInterface()->setDirty(InfoPane_DIRTY_BIT, true );
			GC.GetEngineUserInterface()->setDirty(CityScreen_DIRTY_BIT, true);
			GC.GetEngineUserInterface()->setDirty(ColoredPlots_DIRTY_BIT, true);
		}

		GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
	}
}

/// Tell City to work a Plot, pulling a Citizen from the worst location we can
void CvCityCitizens::DoAlterWorkingPlot(int iIndex)
{
	CvAssertMsg(iIndex >= 0, "iIndex expected to be >= 0");
	CvAssertMsg(iIndex < NUM_CITY_PLOTS, "iIndex expected to be < NUM_CITY_PLOTS");

	// Clicking ON the city "resets" it to default setup
	if(iIndex == CITY_HOME_PLOT)
	{
		CvPlot* pLoopPlot;

		// If we've forced any plots to be worked, reset them to the normal state
		for(int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
		{
			if(iPlotLoop != CITY_HOME_PLOT)
			{
				pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

				if(pLoopPlot != NULL)
				{
					if(IsForcedWorkingPlot(pLoopPlot))
					{
						SetForcedWorkingPlot(pLoopPlot, false);
					}
				}
			}
		}

		// Reset Forced Default Specialists
		ChangeNumForcedDefaultSpecialists(-GetNumForcedDefaultSpecialists());

		DoReallocateCitizens();
	}
	else
	{
		CvPlot* pPlot = GetCityPlotFromIndex(iIndex);

		if(pPlot != NULL)
		{
			if(IsCanWork(pPlot))
			{
//				GetCity()->setCitizensAutomated(false);

				// If we're already working the Plot, then take the guy off and turn him into a Default Specialist
				if(IsWorkingPlot(pPlot))
				{
					SetWorkingPlot(pPlot, false);
					SetForcedWorkingPlot(pPlot, false);
					ChangeNumDefaultSpecialists(1);
					ChangeNumForcedDefaultSpecialists(1);
				}
				// Player picked a new Plot to work
				else
				{
					// Pull from the Default Specialist pool, if possible
					if(GetNumDefaultSpecialists() > 0)
					{
						ChangeNumDefaultSpecialists(-1);
						// Player is forcibly telling city to work a plot, so reduce count of forced default specialists
						if(GetNumForcedDefaultSpecialists() > 0)
							ChangeNumForcedDefaultSpecialists(-1);

						SetWorkingPlot(pPlot, true);
						SetForcedWorkingPlot(pPlot, true);
					}
					// No Default Specialists, so grab a better allocated guy
					else
					{
						// Working Plot
						if(DoRemoveWorstCitizen(true))
						{
							SetWorkingPlot(pPlot, true);
							SetForcedWorkingPlot(pPlot, true);
							//ChangeNumUnassignedCitizens(-1);
						}
						// Good Specialist
						else
						{
							CvAssert(false);
						}
					}
					//if ((GetCity()->extraSpecialists() > 0) || GetCity()->AI_removeWorstCitizen())
					//{
					//	SetWorkingPlot(pPlot, true);
					//}
				}
			}
			// JON: Need to update this block to work with new system
#ifdef AUI_CITIZENS_FIX_LOCKED_TILES_BLOCKED
			else if ((pPlot->getOwner() == GetOwner()) && pPlot->getWorkingCityOverride() != GetCity())
#else
			else if(pPlot->getOwner() == GetOwner())
#endif
			{
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
				CvCity* pOldWorkingCityOverride = pPlot->getWorkingCityOverride();
				if (pOldWorkingCityOverride != NULL)
				{
					if (pOldWorkingCityOverride->IsPuppet())
					{
						return;
#else
				// Can't take away forced plots from puppet Cities
				if(pPlot->getWorkingCityOverride() != NULL)
				{
					if(pPlot->getWorkingCityOverride()->IsPuppet())
					{
						return;
#endif
					}
				}

				pPlot->setWorkingCityOverride(GetCity());
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
				if (pOldWorkingCityOverride)
					pOldWorkingCityOverride->GetCityCitizens()->DoSelfConsistencyCheck();
#endif
			}
		}
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
		DoSelfConsistencyCheck();
#endif
	}
}



/// Has our City been told it MUST a particular CvPlot?
bool CvCityCitizens::IsForcedWorkingPlot(const CvPlot* pPlot) const
{
	int iIndex;

	iIndex = GetCityIndexFromPlot(pPlot);

	if(iIndex != -1)
	{
		return m_pabForcedWorkingPlot[iIndex];
	}

	return false;
}

/// Tell our City it MUST work a particular CvPlot
void CvCityCitizens::SetForcedWorkingPlot(CvPlot* pPlot, bool bNewValue)
{
	int iIndex = GetCityIndexFromPlot(pPlot);

	CvAssertMsg(iIndex >= 0, "iIndex expected to be >= 0");
	CvAssertMsg(iIndex < NUM_CITY_PLOTS, "iIndex expected to be < NUM_CITY_PLOTS");

	if(IsForcedWorkingPlot(pPlot) != bNewValue && iIndex >= 0 && iIndex < NUM_CITY_PLOTS)
	{
		m_pabForcedWorkingPlot[iIndex] = bNewValue;

		// Change the count of how many are forced
		if(bNewValue)
		{
			ChangeNumForcedWorkingPlots(1);

			// More forced plots than we have citizens working?  If so, then pick someone to lose their forced status
			if(GetNumForcedWorkingPlots() > GetNumCitizensWorkingPlots())
			{
				DoValidateForcedWorkingPlots();
			}
		}
		else
		{
			ChangeNumForcedWorkingPlots(-1);
		}
	}
}

/// Make sure we don't have more forced working plots than we have citizens to work
void CvCityCitizens::DoValidateForcedWorkingPlots()
{
	int iNumForcedWorkingPlotsToDemote = GetNumForcedWorkingPlots() - GetNumCitizensWorkingPlots();

	if(iNumForcedWorkingPlotsToDemote > 0)
	{
		for(int iLoop = 0; iLoop < iNumForcedWorkingPlotsToDemote; iLoop++)
		{
			DoDemoteWorstForcedWorkingPlot();
		}
	}
}

/// Remove the Forced status from the worst ForcedWorking plot
void CvCityCitizens::DoDemoteWorstForcedWorkingPlot()
{
	int iValue;

	int iBestPlotValue = -1;
	int iBestPlotID = -1;

	CvPlot* pLoopPlot;

	// Look at all workable Plots
	for(int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
	{
		if(iPlotLoop != CITY_HOME_PLOT)
		{
			pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

			if(pLoopPlot != NULL)
			{
				if(IsForcedWorkingPlot(pLoopPlot))
				{
					iValue = GetPlotValue(pLoopPlot, false);

					// First, or worst yet?
					if(iBestPlotValue == -1 || iValue < iBestPlotValue)
					{
						iBestPlotValue = iValue;
						iBestPlotID = iPlotLoop;
					}
				}
			}
		}
	}

	if(iBestPlotID > -1)
	{
		pLoopPlot = GetCityPlotFromIndex(iBestPlotID);
		SetForcedWorkingPlot(pLoopPlot, false);
	}
}

/// How many plots have we forced to be worked?
int CvCityCitizens::GetNumForcedWorkingPlots() const
{
#ifdef AUI_CITIZENS_FIX_LOCKED_TILES_BLOCKED
	return m_iNumForcedWorkingPlots + GetNumForcedWorkingPlotsBlocked();
#else
	return m_iNumForcedWorkingPlots;
#endif
}

/// Changes how many plots we have forced to be worked
void CvCityCitizens::ChangeNumForcedWorkingPlots(int iChange)
{
	if(iChange != 0)
	{
		m_iNumForcedWorkingPlots += iChange;
	}
}

/// Can our City work a particular CvPlot?
bool CvCityCitizens::IsCanWork(CvPlot* pPlot) const
{
	if(pPlot->getWorkingCity() != m_pCity)
	{
		return false;
	}

	CvAssertMsg(GetCityIndexFromPlot(pPlot) != -1, "GetCityIndexFromPlot(pPlot) is expected to be assigned (not -1)");

	if(pPlot->plotCheck(PUF_canSiege, GetOwner()) != NULL)
	{
		return false;
	}

	if(pPlot->isWater())
	{
		if(!(GET_TEAM(GetTeam()).isWaterWork()))
		{
			return false;
		}

	}

	if(!pPlot->hasYield())
	{
		return false;
	}

	if(IsPlotBlockaded(pPlot))
	{
		return false;
	}

	return true;
}

// Is there a naval blockade on this water tile?
#ifdef AUI_CITIZENS_FIX_LOCKED_TILES_BLOCKED
bool CvCityCitizens::IsPlotBlockaded(const CvPlot* pPlot) const
#else
bool CvCityCitizens::IsPlotBlockaded(CvPlot* pPlot) const
#endif
{
	// See if there are any enemy boats near us that are blockading this plot
	int iBlockadeDistance = /*2*/ GC.getNAVAL_PLOT_BLOCKADE_RANGE();
	int iDX, iDY;
	CvPlot* pNearbyPlot;
#ifdef AUI_CITY_CITIZENS_COUNTERBLOCKADE
	int iClosestEnemyDistance = MAX_INT;
	int iClosestAllyDistance = MAX_INT - 1;
	int iLoopDistance = 0;
#endif

	PlayerTypes ePlayer = m_pCity->getOwner();

	// Might be a better way to do this that'd be slightly less CPU-intensive
#ifdef AUI_HEXSPACE_DX_LOOPS
	int iMaxDX;
	for (iDY = -iBlockadeDistance; iDY <= iBlockadeDistance; iDY++)
	{
		iMaxDX = iBlockadeDistance - MAX(0, iDY);
		for (iDX = -iBlockadeDistance - MIN(0, iDY); iDX <= iMaxDX; iDX++) // MIN() and MAX() stuff is to reduce loops (hexspace!)
#else
	for(iDX = -(iBlockadeDistance); iDX <= iBlockadeDistance; iDX++)
	{
		for(iDY = -(iBlockadeDistance); iDY <= iBlockadeDistance; iDY++)
#endif
		{
			pNearbyPlot = plotXY(pPlot->getX(), pPlot->getY(), iDX, iDY);

			if(pNearbyPlot != NULL)
			{
				// Must be water in the same Area
				if(pNearbyPlot->isWater() && pNearbyPlot->getArea() == pPlot->getArea())
				{
#ifndef AUI_HEXSPACE_DX_LOOPS
#ifdef AUI_FIX_HEX_DISTANCE_INSTEAD_OF_PLOT_DISTANCE
					if (hexDistance(iDX, iDY) <= iBlockadeDistance)
#else
					if(plotDistance(pNearbyPlot->getX(), pNearbyPlot->getY(), pPlot->getX(), pPlot->getY()) <= iBlockadeDistance)
#endif
#endif
					{
						// Enemy boat within range to blockade our plot?
						if(pNearbyPlot->IsActualEnemyUnit(ePlayer))
						{
#ifdef AUI_CITY_CITIZENS_COUNTERBLOCKADE
#ifdef AUI_HEXSPACE_DX_LOOPS
							iLoopDistance = hexDistance(iDX, iDY);
#else
							iLoopDistance = plotDistance(pPlot->getX(), pPlot->getY(), pNearbyPlot->getX(), pNearbyPlot->getY());
#endif
							if (iLoopDistance < iClosestEnemyDistance)
								iClosestEnemyDistance = iLoopDistance;
						}

						if (pNearbyPlot->HasAlliedUnit(ePlayer))
						{
#ifdef AUI_HEXSPACE_DX_LOOPS
							iLoopDistance = hexDistance(iDX, iDY);
#else
							iLoopDistance = plotDistance(pPlot->getX(), pPlot->getY(), pNearbyPlot->getX(), pNearbyPlot->getY());
#endif
							if (iLoopDistance < iClosestAllyDistance)
								iClosestAllyDistance = iLoopDistance;
#else
							return true;
#endif
						}
					}
				}
			}
		}
	}
#ifdef AUI_CITY_CITIZENS_COUNTERBLOCKADE
	return iClosestEnemyDistance < iClosestAllyDistance;
#else
	return false;
#endif
}

// Is there a naval blockade on any of this city's water tiles?
bool CvCityCitizens::IsAnyPlotBlockaded() const
{
	CvPlot* pLoopPlot;

	// Look at all workable Plots
	for(int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
	{
		if(iPlotLoop != CITY_HOME_PLOT)
		{
			pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

			if(pLoopPlot != NULL)
			{
				if(IsPlotBlockaded(pLoopPlot))
				{
					return true;
				}
			}
		}
	}

	return false;
}

#ifdef AUI_CITIZENS_FIX_LOCKED_TILES_BLOCKED
bool CvCityCitizens::GetNumForcedWorkingPlotsBlocked() const
{
	int iCount = 0;

	const CvPlot* pLoopPlot;

	// Look at all workable Plots
	for (int iPlotLoop = 0; iPlotLoop < NUM_CITY_PLOTS; iPlotLoop++)
	{
		if (iPlotLoop != CITY_HOME_PLOT)
		{
			pLoopPlot = GetCityPlotFromIndex(iPlotLoop);

			if (pLoopPlot != NULL)
			{
				if (IsForcedWorkingPlot(pLoopPlot))
				{
					if ((pLoopPlot->plotCheck(PUF_canSiege, GetOwner()) != NULL) || (IsPlotBlockaded(pLoopPlot)))
					{
						iCount++;
					}
				}
			}
		}
	}

	return iCount;
}
#endif

/// If we're working this plot make sure we're allowed, and if we're not then correct the situation
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
bool CvCityCitizens::DoVerifyWorkingPlot(CvPlot* pPlot)
{
	bool bRet = false;
#else
void CvCityCitizens::DoVerifyWorkingPlot(CvPlot* pPlot)
{
#endif
	if(pPlot != NULL)
	{
		if(IsWorkingPlot(pPlot))
		{
			if(!IsCanWork(pPlot))
			{
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
				bRet = true;
#endif
				SetWorkingPlot(pPlot, false);
				DoAddBestCitizenFromUnassigned();
			}
		}
	}
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	return bRet;
#endif
}

/// Check all Plots by this City to see if we can actually be working them (if we are)
void CvCityCitizens::DoVerifyWorkingPlots()
{
	int iI;
	CvPlot* pPlot;
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	bool bDoSelfConsistency = false;
#endif

	for(iI = 0; iI < NUM_CITY_PLOTS; iI++)
	{
		pPlot = GetCityPlotFromIndex(iI);

#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
		bDoSelfConsistency = DoVerifyWorkingPlot(pPlot) || bDoSelfConsistency;
#else
		DoVerifyWorkingPlot(pPlot);
#endif
	}

#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	if (bDoSelfConsistency)
		DoSelfConsistencyCheck();
#endif
}




///////////////////////////////////////////////////
// Helpful Stuff
///////////////////////////////////////////////////




/// Returns the Plot Index from a CvPlot
int CvCityCitizens::GetCityIndexFromPlot(const CvPlot* pPlot) const
{
	return plotCityXY(m_pCity, pPlot);
}


/// Returns the CvPlot from a Plot Index
CvPlot* CvCityCitizens::GetCityPlotFromIndex(int iIndex) const
{
	return plotCity(m_pCity->getX(), m_pCity->getY(), iIndex);
}




///////////////////////////////////////////////////
// Specialists
///////////////////////////////////////////////////


#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
int CvCityCitizens::getCachedGPChangeT100ForThisTurn(SpecialistTypes eGPSpecialistType) const
{
	return m_aiCachedGPChangeT100ForThisTurn[eGPSpecialistType];
}

void CvCityCitizens::cacheGPChangesT100ForThisTurn()
{
	int iGPPChange;
	int iCount;
	int iMod;
#ifdef AUI_WARNING_FIXES
	for (uint iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#else
	for (int iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#endif
	{
		const SpecialistTypes eSpecialist = static_cast<SpecialistTypes>(iSpecialistLoop);
		m_aiCachedGPChangeT100ForThisTurn[eSpecialist] = 0;
		CvSpecialistInfo* pkSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
		if (pkSpecialistInfo)
		{
			// Does this Specialist spawn a GP?
			if (pkSpecialistInfo->getGreatPeopleUnitClass() != NO_UNITCLASS)
			{
				iCount = GetSpecialistCount(eSpecialist);

				// GPP from Specialists
				iGPPChange = pkSpecialistInfo->getGreatPeopleRateChange() * iCount * 100;

				// GPP from Buildings
				iGPPChange += GetBuildingGreatPeopleRateChanges(eSpecialist) * 100;

				if (iGPPChange > 0)
				{
					iMod = 0;

					// City mod
					iMod += GetCity()->getGreatPeopleRateModifier();

					// Player mod
					iMod += GetPlayer()->getGreatPeopleRateModifier();

					// Player and Golden Age mods to this specific class
					if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_SCIENTIST"))
					{
						iMod += GetPlayer()->getGreatScientistRateModifier();
					}
					else if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_WRITER"))
					{
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatWriterRateModifier();
						}
						iMod += GetPlayer()->getGreatWriterRateModifier();
					}
					else if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_ARTIST"))
					{
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatArtistRateModifier();
						}
						iMod += GetPlayer()->getGreatArtistRateModifier();
					}
					else if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_MUSICIAN"))
					{
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatMusicianRateModifier();
						}
						iMod += GetPlayer()->getGreatMusicianRateModifier();
					}
					else if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_MERCHANT"))
					{
						iMod += GetPlayer()->getGreatMerchantRateModifier();
					}
					else if ((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_ENGINEER"))
					{
						iMod += GetPlayer()->getGreatEngineerRateModifier();
					}

					// Apply mod
					iGPPChange *= (100 + iMod);
					iGPPChange /= 100;

					m_aiCachedGPChangeT100ForThisTurn[eSpecialist] = iGPPChange;
				}
			}
		}
	}
}
#endif

/// Called at the end of every turn: Looks at the specialists in this City and levels them up
void CvCityCitizens::DoSpecialists()
{
#ifndef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	int iGPPChange;
	int iCount;
	int iMod;
#endif
#ifdef AUI_WARNING_FIXES
	for (uint iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#else
	for(int iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#endif
	{
		const SpecialistTypes eSpecialist = static_cast<SpecialistTypes>(iSpecialistLoop);
		CvSpecialistInfo* pkSpecialistInfo = GC.getSpecialistInfo(eSpecialist);
		if(pkSpecialistInfo)
		{
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
			UnitClassTypes eGPUnitClassType = (UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass();
			if (eGPUnitClassType != NO_UNITCLASS)
			{
				ChangeSpecialistGreatPersonProgressTimes100(eSpecialist, getCachedGPChangeT100ForThisTurn(eSpecialist));

				int iGPThreshold = GetSpecialistUpgradeThreshold(eGPUnitClassType);
#else
			int iGPThreshold = GetSpecialistUpgradeThreshold((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass());

			// Does this Specialist spawn a GP?
			if(pkSpecialistInfo->getGreatPeopleUnitClass() != NO_UNITCLASS)
			{
				iCount = GetSpecialistCount(eSpecialist);

				// GPP from Specialists
				iGPPChange = pkSpecialistInfo->getGreatPeopleRateChange() * iCount * 100;

				// GPP from Buildings
				iGPPChange += GetBuildingGreatPeopleRateChanges(eSpecialist) * 100;

				if(iGPPChange > 0)
				{
					iMod = 0;

					// City mod
					iMod += GetCity()->getGreatPeopleRateModifier();

					// Player mod
					iMod += GetPlayer()->getGreatPeopleRateModifier();

					// Player and Golden Age mods to this specific class
					if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_SCIENTIST"))
					{
						iMod += GetPlayer()->getGreatScientistRateModifier();
					}
					else if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_WRITER"))
					{ 
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatWriterRateModifier();
						}
						iMod += GetPlayer()->getGreatWriterRateModifier();
					}
					else if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_ARTIST"))
					{
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatArtistRateModifier();
						}
						iMod += GetPlayer()->getGreatArtistRateModifier();
					}
					else if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_MUSICIAN"))
					{
						if (GetPlayer()->isGoldenAge())
						{
							iMod += GetPlayer()->GetPlayerTraits()->GetGoldenAgeGreatMusicianRateModifier();
						}
#ifdef NQ_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER_FROM_POLICIES
						int iProductionToGreatMusiciansModifier = GetPlayer()->GetPlayerPolicies()->GetNumericModifier(POLICYMOD_PRODUCTION_TO_GREAT_MUSICIANS_MODIFIER);
						if (iProductionToGreatMusiciansModifier > 0)
						{
							iMod += GetPlayer()->calculateTotalYield(YIELD_PRODUCTION) * iProductionToGreatMusiciansModifier / 100;
						}
#endif
						iMod += GetPlayer()->getGreatMusicianRateModifier();
					}
					else if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_MERCHANT"))
					{
						iMod += GetPlayer()->getGreatMerchantRateModifier();
					}
					else if((UnitClassTypes)pkSpecialistInfo->getGreatPeopleUnitClass() == GC.getInfoTypeForString("UNITCLASS_ENGINEER"))
					{
						iMod += GetPlayer()->getGreatEngineerRateModifier();
					}

					// Apply mod
					iGPPChange *= (100 + iMod);
					iGPPChange /= 100;

					ChangeSpecialistGreatPersonProgressTimes100(eSpecialist, iGPPChange);
				}
#endif

				// Enough to spawn a GP?
				if(GetSpecialistGreatPersonProgress(eSpecialist) >= iGPThreshold)
				{
					// No Minors
					if(!GET_PLAYER(GetCity()->getOwner()).isMinorCiv())
					{
						// Reset progress on this Specialist
						DoResetSpecialistGreatPersonProgressTimes100(eSpecialist);

						// Now... actually create the GP!
						const UnitClassTypes eUnitClass = (UnitClassTypes) pkSpecialistInfo->getGreatPeopleUnitClass();
						const CivilizationTypes eCivilization = GetCity()->getCivilizationType();
						CvCivilizationInfo* pCivilizationInfo = GC.getCivilizationInfo(eCivilization);
						if(pCivilizationInfo != NULL)
						{
							UnitTypes eUnit = (UnitTypes) pCivilizationInfo->getCivilizationUnits(eUnitClass);

							DoSpawnGreatPerson(eUnit, true, false);
						}
					}
				}
			}
		}
	}
}

/// How many Specialists are assigned to this Building Type?
#ifdef AUI_CONSTIFY
int CvCityCitizens::GetNumSpecialistsAllowedByBuilding(const CvBuildingEntry& kBuilding) const
#else
int CvCityCitizens::GetNumSpecialistsAllowedByBuilding(const CvBuildingEntry& kBuilding)
#endif
{
	return kBuilding.GetSpecialistCount();
}

/// Are we in the position to add another Specialist to eBuilding?
#ifdef AUI_CONSTIFY
bool CvCityCitizens::IsCanAddSpecialistToBuilding(BuildingTypes eBuilding) const
#else
bool CvCityCitizens::IsCanAddSpecialistToBuilding(BuildingTypes eBuilding)
#endif
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());
#ifdef AUI_WARNING_FIXES
	CvBuildingEntry* pBuildingInfo = GC.getBuildingInfo(eBuilding);
	if (!pBuildingInfo)
		return false;
#endif

	int iNumSpecialistsAssigned = GetNumSpecialistsInBuilding(eBuilding);

	if(iNumSpecialistsAssigned < GetCity()->getPopulation() &&	// Limit based on Pop of City
#ifdef AUI_WARNING_FIXES
		iNumSpecialistsAssigned < pBuildingInfo->GetSpecialistCount() &&				// Limit for this particular Building
#else
	        iNumSpecialistsAssigned < GC.getBuildingInfo(eBuilding)->GetSpecialistCount() &&				// Limit for this particular Building
#endif
	        iNumSpecialistsAssigned < GC.getMAX_SPECIALISTS_FROM_BUILDING())	// Overall Limit
	{
		return true;
	}

	return false;
}

/// Adds and initializes a Specialist for this building
void CvCityCitizens::DoAddSpecialistToBuilding(BuildingTypes eBuilding, bool bForced)
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());

	CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
	if(pkBuildingInfo == NULL)
	{
		return;
	}

	SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();

	// Can't add more than the max
	if(IsCanAddSpecialistToBuilding(eBuilding))
	{
		// If we're force-assigning a specialist, then we can reduce the count on forced default specialists
		if(bForced)
		{
			if(GetNumForcedDefaultSpecialists() > 0)
				ChangeNumForcedDefaultSpecialists(-1);
		}

		// If we don't already have an Unassigned Citizen to turn into a Specialist, find one from somewhere
		if(GetNumUnassignedCitizens() == 0)
		{
			DoRemoveWorstCitizen(true, /*Don't remove this type*/ eSpecialist);
			if(GetNumUnassignedCitizens() == 0)
			{
				// Still nobody, all the citizens may be assigned to the eSpecialist we are looking for, try again
				if(!DoRemoveWorstSpecialist(NO_SPECIALIST, eBuilding))
				{
					return; // For some reason we can't do this, we must exit, else we will be going over the population count
				}
			}
		}

		// Increase count for the whole city
		m_aiSpecialistCounts[eSpecialist]++;
		m_aiNumSpecialistsInBuilding[eBuilding]++;

		if(bForced)
		{
			m_aiNumForcedSpecialistsInBuilding[eBuilding]++;
		}

		GetCity()->processSpecialist(eSpecialist, 1);
		GetCity()->UpdateReligion(GetCity()->GetCityReligions()->GetReligiousMajority());

		ChangeNumUnassignedCitizens(-1);

		ICvUserInterface2* pkIFace = GC.GetEngineUserInterface();
		pkIFace->setDirty(GameData_DIRTY_BIT, true);
		pkIFace->setDirty(CityInfo_DIRTY_BIT, true);
		//pkIFace->setDirty(InfoPane_DIRTY_BIT, true );
		pkIFace->setDirty(CityScreen_DIRTY_BIT, true);
		pkIFace->setDirty(ColoredPlots_DIRTY_BIT, true);

		CvCity* pkCity = GetCity();
		auto_ptr<ICvCity1> pCity = GC.WrapCityPointer(pkCity);

		pkIFace->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_SPECIALISTS);
	}
}

/// Removes and uninitializes a Specialist for this building
void CvCityCitizens::DoRemoveSpecialistFromBuilding(BuildingTypes eBuilding, bool bForced, bool bEliminatePopulation)
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());

	CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
	if(pkBuildingInfo == NULL)
	{
		return;
	}

	SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();

	int iNumSpecialistsAssigned = GetNumSpecialistsInBuilding(eBuilding);

	// Need at least 1 assigned to remove
	if(iNumSpecialistsAssigned > 0)
	{
		// Decrease count for the whole city
		m_aiSpecialistCounts[eSpecialist]--;
		m_aiNumSpecialistsInBuilding[eBuilding]--;

		if(bForced)
		{
			m_aiNumForcedSpecialistsInBuilding[eBuilding]--;
		}

		GetCity()->processSpecialist(eSpecialist, -1);
		GetCity()->UpdateReligion(GetCity()->GetCityReligions()->GetReligiousMajority());

		// Do we kill this population or reassign him?
		if(bEliminatePopulation)
		{
			GetCity()->changePopulation(-1, /*bReassignPop*/ false);
		}
		else
		{
			ChangeNumUnassignedCitizens(1);
		}

#ifdef AUI_WARNING_FIXES
		ICvUserInterface2* pkIFace = GC.GetEngineUserInterface();
		pkIFace->setDirty(GameData_DIRTY_BIT, true);
		pkIFace->setDirty(CityInfo_DIRTY_BIT, true);
		//pkIFace->setDirty(InfoPane_DIRTY_BIT, true );
		pkIFace->setDirty(CityScreen_DIRTY_BIT, true);
		pkIFace->setDirty(ColoredPlots_DIRTY_BIT, true);

		auto_ptr<ICvCity1> pCity = GC.WrapCityPointer(GetCity());

		pkIFace->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_SPECIALISTS);
#else
		GC.GetEngineUserInterface()->setDirty(GameData_DIRTY_BIT, true);
		GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
		//GC.GetEngineUserInterface()->setDirty(InfoPane_DIRTY_BIT, true );
		GC.GetEngineUserInterface()->setDirty(CityScreen_DIRTY_BIT, true);
		GC.GetEngineUserInterface()->setDirty(ColoredPlots_DIRTY_BIT, true);

		auto_ptr<ICvCity1> pCity = GC.WrapCityPointer(GetCity());

		GC.GetEngineUserInterface()->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_SPECIALISTS);
#endif
	}
}

//	----------------------------------------------------------------------------
/// Clear EVERYONE from this Building
/// Any one in the building will be put in the unassigned citizens list.
/// It is up to the caller to reassign population.
void CvCityCitizens::DoRemoveAllSpecialistsFromBuilding(BuildingTypes eBuilding, bool bEliminatePopulation)
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());

	CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
	if(pkBuildingInfo == NULL)
	{
		return;
	}

	SpecialistTypes eSpecialist = (SpecialistTypes) pkBuildingInfo->GetSpecialistType();
	int iNumSpecialists = GetNumSpecialistsInBuilding(eBuilding);

	m_aiNumForcedSpecialistsInBuilding[eBuilding] = 0;

	// Pick the worst to remove
	for(int iAssignedLoop = 0; iAssignedLoop < iNumSpecialists; iAssignedLoop++)
	{
		// Decrease count for the whole city
		m_aiSpecialistCounts[eSpecialist]--;
		m_aiNumSpecialistsInBuilding[eBuilding]--;
		GetCity()->processSpecialist(eSpecialist, -1);

		// Do we kill this population or reassign him?
		if(bEliminatePopulation)
		{
			GetCity()->changePopulation(-1, /*bReassignPop*/ false);
		}
		else
		{
			ChangeNumUnassignedCitizens(1);
		}

#ifdef AUI_WARNING_FIXES
		ICvUserInterface2* pkIFace = GC.GetEngineUserInterface();
		pkIFace->setDirty(CityInfo_DIRTY_BIT, true);
		//pkIFace->setDirty(InfoPane_DIRTY_BIT, true );
		pkIFace->setDirty(CityScreen_DIRTY_BIT, true);
		pkIFace->setDirty(ColoredPlots_DIRTY_BIT, true);

		auto_ptr<ICvCity1> pCity = GC.WrapCityPointer(GetCity());
		pkIFace->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_SPECIALISTS);
#else
		GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
		//GC.GetEngineUserInterface()->setDirty(InfoPane_DIRTY_BIT, true );
		GC.GetEngineUserInterface()->setDirty(CityScreen_DIRTY_BIT, true);
		GC.GetEngineUserInterface()->setDirty(ColoredPlots_DIRTY_BIT, true);

		auto_ptr<ICvCity1> pCity = GC.WrapCityPointer(GetCity());
		GC.GetEngineUserInterface()->SetSpecificCityInfoDirty(pCity.get(), CITY_UPDATE_TYPE_SPECIALISTS);
#endif
	}
}


/// Find the worst Specialist and remove him from duty
bool CvCityCitizens::DoRemoveWorstSpecialist(SpecialistTypes eDontChangeSpecialist, const BuildingTypes eDontRemoveFromBuilding /* = NO_BUILDING */)
{
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	BuildingTypes eWorstBuilding = NO_BUILDING;
	int iWorstSpecialistScore = MAX_INT;
	int iLoopSpecialistScore = 0;
#endif
#ifdef AUI_WARNING_FIXES
	for (uint iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#else
	for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#endif
	{
		const BuildingTypes eBuilding = static_cast<BuildingTypes>(iBuildingLoop);

		if(eBuilding == eDontRemoveFromBuilding)
		{
			continue;
		}

		CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
		if(pkBuildingInfo == NULL)
		{
			continue;
		}

		// We might not be allowed to change this Building's Specialists
		if(eDontChangeSpecialist == pkBuildingInfo->GetSpecialistType())
		{
			continue;
		}

		if(GetNumSpecialistsInBuilding(eBuilding) > 0)
		{
#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
#ifdef AUI_CITIZENS_UNHARDCODE_SPECIALIST_VALUE_HAPPINESS
			iLoopSpecialistScore = GetSpecialistValue((SpecialistTypes)pkBuildingInfo->GetSpecialistType(), true);
#else
			iLoopSpecialistScore = GetSpecialistValue((SpecialistTypes)pkBuildingInfo->GetSpecialistType());
#endif
			if (iLoopSpecialistScore < iWorstSpecialistScore)
			{
				iWorstSpecialistScore = iLoopSpecialistScore;
				eWorstBuilding = eBuilding;
			}
#else
			DoRemoveSpecialistFromBuilding(eBuilding, true);

			return true;
#endif
		}
	}

#ifdef AUI_CITIZENS_FIX_REMOVE_WORST_SPECIALIST_ACTUALLY_REMOVES_WORST
	if (eWorstBuilding != NO_BUILDING)
	{
		DoRemoveSpecialistFromBuilding(eWorstBuilding, false);

		return true;
	}
#endif

	return false;
}

/// How many Default Specialists are assigned in this City?
int CvCityCitizens::GetNumDefaultSpecialists() const
{
	return m_iNumDefaultSpecialists;
}

/// Changes how many Default Specialists are assigned in this City
void CvCityCitizens::ChangeNumDefaultSpecialists(int iChange)
{
	m_iNumDefaultSpecialists += iChange;

	SpecialistTypes eSpecialist = (SpecialistTypes) GC.getDEFAULT_SPECIALIST();
	m_aiSpecialistCounts[eSpecialist] += iChange;

	GetCity()->processSpecialist(eSpecialist, iChange);

	ChangeNumUnassignedCitizens(-iChange);
}

/// How many Default Specialists have been forced assigned in this City?
int CvCityCitizens::GetNumForcedDefaultSpecialists() const
{
	return m_iNumForcedDefaultSpecialists;
}

/// How many Default Specialists have been forced assigned in this City?
void CvCityCitizens::ChangeNumForcedDefaultSpecialists(int iChange)
{
	m_iNumForcedDefaultSpecialists += iChange;
}

/// How many Specialists do we have assigned of this type in our City?
int CvCityCitizens::GetSpecialistCount(SpecialistTypes eIndex) const
{
	CvAssert(eIndex > -1);
	CvAssert(eIndex < GC.getNumSpecialistInfos());

	return m_aiSpecialistCounts[eIndex];
}

/// Count up all the Specialists we have here
int CvCityCitizens::GetTotalSpecialistCount() const
{
	int iNumSpecialists = 0;
	SpecialistTypes eSpecialist;

#ifdef AUI_WARNING_FIXES
	for (uint iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#else
	for(int iSpecialistLoop = 0; iSpecialistLoop < GC.getNumSpecialistInfos(); iSpecialistLoop++)
#endif
	{
		eSpecialist = (SpecialistTypes) iSpecialistLoop;

		if (eSpecialist != (SpecialistTypes) GC.getDEFAULT_SPECIALIST())
		{
			iNumSpecialists += GetSpecialistCount(eSpecialist);
		}
	}

	return iNumSpecialists;
}

/// GPP changes from Buildings
int CvCityCitizens::GetBuildingGreatPeopleRateChanges(SpecialistTypes eSpecialist) const
{
	CvAssert(eSpecialist > -1);
	CvAssert(eSpecialist < GC.getNumSpecialistInfos());

	return m_piBuildingGreatPeopleRateChanges[eSpecialist];
}

/// Change GPP from Buildings
void CvCityCitizens::ChangeBuildingGreatPeopleRateChanges(SpecialistTypes eSpecialist, int iChange)
{
	CvAssert(eSpecialist > -1);
	CvAssert(eSpecialist < GC.getNumSpecialistInfos());

	m_piBuildingGreatPeopleRateChanges[eSpecialist] += iChange;
}

/// How much progress does this City have towards a Great Person from eIndex?
int CvCityCitizens::GetSpecialistGreatPersonProgress(SpecialistTypes eIndex) const
{
	CvAssert(eIndex > -1);
	CvAssert(eIndex < GC.getNumSpecialistInfos());

	return GetSpecialistGreatPersonProgressTimes100(eIndex) / 100;
}

/// How much progress does this City have towards a Great Person from eIndex? (in hundreds)
int CvCityCitizens::GetSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex) const
{
	CvAssert(eIndex > -1);
	CvAssert(eIndex < GC.getNumSpecialistInfos());

	return m_aiSpecialistGreatPersonProgressTimes100[eIndex];
}

/// How much progress does this City have towards a Great Person from eIndex?
void CvCityCitizens::ChangeSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex, int iChange)
{
	CvAssert(eIndex > -1);
	CvAssert(eIndex < GC.getNumSpecialistInfos());

	m_aiSpecialistGreatPersonProgressTimes100[eIndex] += iChange;
}

/// Reset Specialist progress
void CvCityCitizens::DoResetSpecialistGreatPersonProgressTimes100(SpecialistTypes eIndex)
{
	CvAssert(eIndex > -1);
	CvAssert(eIndex < GC.getNumSpecialistInfos());

	m_aiSpecialistGreatPersonProgressTimes100[eIndex] = 0;
}

/// How many Specialists are assigned to eBuilding?
int CvCityCitizens::GetNumSpecialistsInBuilding(BuildingTypes eBuilding) const
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());

	return m_aiNumSpecialistsInBuilding[eBuilding];
}

/// How many Forced Specialists are assigned to eBuilding?
int CvCityCitizens::GetNumForcedSpecialistsInBuilding(BuildingTypes eBuilding) const
{
	CvAssert(eBuilding > -1);
	CvAssert(eBuilding < GC.getNumBuildingInfos());

	return m_aiNumForcedSpecialistsInBuilding[eBuilding];
}

/// Remove forced status from all Specialists
void CvCityCitizens::DoClearForcedSpecialists()
{
	// Loop through all Buildings
	BuildingTypes eBuilding;
#ifdef AUI_WARNING_FIXES
	for (uint iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#else
	for(int iBuildingLoop = 0; iBuildingLoop < GC.getNumBuildingInfos(); iBuildingLoop++)
#endif
	{
		eBuilding = (BuildingTypes) iBuildingLoop;

		// Have this Building in the City?
		if(GetCity()->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
		{
			m_aiNumForcedSpecialistsInBuilding[eBuilding] = 0;
		}
	}
}

/// What upgrade progress does a Specialist need to level up?
#ifdef AUI_CONSTIFY
int CvCityCitizens::GetSpecialistUpgradeThreshold(UnitClassTypes eUnitClass) const
#else
int CvCityCitizens::GetSpecialistUpgradeThreshold(UnitClassTypes eUnitClass)
#endif
{
	int iThreshold = /*100*/ GC.getGREAT_PERSON_THRESHOLD_BASE();
	int iNumCreated;

	if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_WRITER", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatWritersCreated();
	}
	else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_ARTIST", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatArtistsCreated();
	}
	else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_MUSICIAN", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatMusiciansCreated();
	}
	// GJS: begin separation of great people types
	else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_SCIENTIST", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatScientistsCreated();
	}
	else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_ENGINEER", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatEngineersCreated();
	}
	else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_MERCHANT", true))
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatMerchantsCreated();
	}
	// GJS: end separation of great people types
	else
	{
		iNumCreated = GET_PLAYER(GetCity()->getOwner()).getGreatPeopleCreated();
	}

	// Increase threshold based on how many GP have already been spawned
	iThreshold += (/*50*/ GC.getGREAT_PERSON_THRESHOLD_INCREASE() * iNumCreated);

	// Game Speed mod
	iThreshold *= GC.getGame().getGameSpeedInfo().getGreatPeoplePercent();
	iThreshold /= 100;

	// Start era mod
	iThreshold *= GC.getGame().getStartEraInfo().getGreatPeoplePercent();
	iThreshold /= 100;

	return iThreshold;
}

/// Create a GP!
#ifdef ENHANCED_GRAPHS
void CvCityCitizens::DoSpawnGreatPerson(UnitTypes eUnit, bool bIncrementCount, bool bCountAsProphet, bool bMayaBoost)
#else
void CvCityCitizens::DoSpawnGreatPerson(UnitTypes eUnit, bool bIncrementCount, bool bCountAsProphet)
#endif
{
	CvAssert(eUnit != NO_UNIT);

	if (eUnit == NO_UNIT)
		return;	// Better than crashing.

	// If it's the active player then show the popup
	if(GetCity()->getOwner() == GC.getGame().getActivePlayer())
	{
		// Don't show in MP
		if(!GC.getGame().isNetworkMultiPlayer())	// KWG: Candidate for !GC.getGame().IsOption(GAMEOPTION_SIMULTANEOUS_TURNS)
		{
			CvPopupInfo kPopupInfo(BUTTONPOPUP_GREAT_PERSON_REWARD, eUnit, GetCity()->GetID());
			GC.GetEngineUserInterface()->AddPopup(kPopupInfo);
		}
	}

	CvPlayer& kPlayer = GET_PLAYER(GetCity()->getOwner());
	CvUnit* newUnit = kPlayer.initUnit(eUnit, GetCity()->getX(), GetCity()->getY());
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
	if (bSpawnWithNoExpendedTrigger)
		newUnit->SetIgnoreExpended(true);
#endif

	// Bump up the count
	if(bIncrementCount && !bCountAsProphet)
	{
		if(newUnit->IsGreatGeneral())
		{
#ifdef ENHANCED_GRAPHS
			kPlayer.ChangeNumGeneralsTotal(1);
#else
			kPlayer.incrementGreatGeneralsCreated();
#endif
		}

		else if(newUnit->IsGreatAdmiral())
		{
#ifdef ENHANCED_GRAPHS
			kPlayer.ChangeNumAdmiralsTotal(1);
#else
			kPlayer.incrementGreatAdmiralsCreated();
#endif
			CvPlot *pSpawnPlot = kPlayer.GetGreatAdmiralSpawnPlot(newUnit);
			if (newUnit->plot() != pSpawnPlot)
			{
				newUnit->setXY(pSpawnPlot->getX(), pSpawnPlot->getY());
			}
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_WRITER"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostWriters(true);
			}
			kPlayer.ChangeNumWritersTotal(1);
#endif
			kPlayer.incrementGreatWritersCreated();
		}							
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_ARTIST"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostArtists(true);
			}
			kPlayer.ChangeNumArtistsTotal(1);
#endif
			kPlayer.incrementGreatArtistsCreated();
		}							
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_MUSICIAN"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostMusicians(true);
			}
			kPlayer.ChangeNumMusiciansTotal(1);
#endif
			kPlayer.incrementGreatMusiciansCreated();
		}		
		// GJS: Start separation of great people
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_SCIENTIST"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostScientist(true);
			}
			kPlayer.ChangeNumScientistsTotal(1);
#endif
			kPlayer.incrementGreatScientistsCreated();
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_ENGINEER"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostEngineers(true);
			}
			kPlayer.ChangeNumEngineersTotal(1);
#endif
			kPlayer.incrementGreatEngineersCreated();
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_MERCHANT"))
		{
#ifdef ENHANCED_GRAPHS
			if (bMayaBoost)
			{
				kPlayer.SetMayaBoostMerchants(true);
			}
			kPlayer.ChangeNumMerchantsTotal(1);
#endif
			kPlayer.incrementGreatMerchantsCreated();
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_PROPHET"))
		{
#ifdef ENHANCED_GRAPHS
			kPlayer.ChangeNumProphetsTotal(1);
#endif
			kPlayer.incrementGreatProphetsCreated();
		}
		// GJS: End separation of great people
		else
		{
			kPlayer.incrementGreatPeopleCreated();
		}
	}
#ifdef ENHANCED_GRAPHS
	if (!bIncrementCount && !bCountAsProphet)
	{
		if (newUnit->IsGreatGeneral())
		{
			kPlayer.ChangeNumGeneralsTotal(1);
		}
		else if (newUnit->IsGreatAdmiral())
		{
			kPlayer.ChangeNumAdmiralsTotal(1);
			CvPlot* pSpawnPlot = kPlayer.GetGreatAdmiralSpawnPlot(newUnit);
			if (newUnit->plot() != pSpawnPlot)
			{
				newUnit->setXY(pSpawnPlot->getX(), pSpawnPlot->getY());
			}
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_WRITER"))
		{
			kPlayer.ChangeNumWritersTotal(1);
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_ARTIST"))
		{
			kPlayer.ChangeNumArtistsTotal(1);
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_MUSICIAN"))
		{
			kPlayer.ChangeNumMusiciansTotal(1);
		}
		else if (newUnit->getUnitInfo().GetUnitClassType() == GC.getInfoTypeForString("UNITCLASS_PROPHET"))
		{
			kPlayer.ChangeNumProphetsTotal(1);
		}
	}
#endif
	// NQMP GJS: In order to make "free" Great Prophets truly free, had to remove the 2nd half of the check below, left it commented out though in case I screw something up.
	if(bCountAsProphet)// || newUnit->getUnitInfo().IsFoundReligion())
	{
		kPlayer.GetReligions()->ChangeNumProphetsSpawned(1);
#ifdef ENHANCED_GRAPHS
		kPlayer.ChangeNumProphetsTotal(1);
#endif
	}

	// Setup prophet properly
	if(newUnit->getUnitInfo().IsFoundReligion())
	{
		ReligionTypes eReligion = kPlayer.GetReligions()->GetReligionCreatedByPlayer();
		int iReligionSpreads = newUnit->getUnitInfo().GetReligionSpreads();
		int iReligiousStrength = newUnit->getUnitInfo().GetReligiousStrength();
		if(iReligionSpreads > 0 && eReligion > RELIGION_PANTHEON)
		{
			newUnit->GetReligionData()->SetSpreadsLeft(iReligionSpreads);
			newUnit->GetReligionData()->SetReligiousStrength(iReligiousStrength);
			newUnit->GetReligionData()->SetReligion(eReligion);
		}
	}

	if (newUnit->getUnitInfo().GetOneShotTourism() > 0)
	{
		newUnit->SetTourismBlastStrength(kPlayer.GetCulture()->GetTourismBlastStrength(newUnit->getUnitInfo().GetOneShotTourism()));
	}
	// GJS: Great Scientists now bulb for science at point of birth, not current science
#ifdef DECREASE_BULB_AMOUNT_OVER_TIME
	if (GC.getGame().isOption("GAMEOPTION_NO_SCIENTIST_SAVING"))
	{
		newUnit->SetScientistBirthTurn(GC.getGame().getGameTurn());
	}
	else if (newUnit->getUnitInfo().GetBaseBeakersTurnsToCount() > 0)
	{
		newUnit->SetResearchBulbAmount(kPlayer.GetScienceYieldFromPreviousTurns(GC.getGame().getGameTurn(), newUnit->getUnitInfo().GetBaseBeakersTurnsToCount()));
	}
#else 
	if (newUnit->getUnitInfo().GetBaseBeakersTurnsToCount() > 0)
	{
		newUnit->SetResearchBulbAmount(kPlayer.GetScienceYieldFromPreviousTurns(GC.getGame().getGameTurn(), newUnit->getUnitInfo().GetBaseBeakersTurnsToCount()));
	}
#endif
#ifdef NQ_WAR_HERO
	if (newUnit->IsGreatGeneral() && kPlayer.IsWarHero())
	{
		kPlayer.addFreeUnit((UnitTypes)GC.getInfoTypeForString("UNIT_ARTIST"));
	}
#endif

	// Notification
	if(GET_PLAYER(GetOwner()).GetNotifications())
	{
		Localization::String strText = Localization::Lookup("TXT_KEY_NOTIFICATION_GREAT_PERSON_ACTIVE_PLAYER");
		Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_SUMMARY_GREAT_PERSON");
		GET_PLAYER(GetOwner()).GetNotifications()->Add(NOTIFICATION_GREAT_PERSON_ACTIVE_PLAYER, strText.toUTF8(), strSummary.toUTF8(), GetCity()->getX(), GetCity()->getY(), eUnit);
	}
}

#ifdef AUI_CITIZENS_FOOD_PRODUCTION_TRIAL_RUN_THEN_SELF_CONSISTENCY
bool CvCityCitizens::getIgnoreFoodProduction() const
{
	return m_bIgnoreFoodProduction;
}

void CvCityCitizens::setIgnoreFoodProduction(bool bNewValue)
{
	m_bIgnoreFoodProduction = bNewValue;
}
#endif