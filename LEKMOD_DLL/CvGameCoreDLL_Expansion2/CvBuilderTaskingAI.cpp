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
#include "CvBuilderTaskingAI.h"
#include "CvPlayer.h"
#include "CvTraitClasses.h"
#include "CvAStar.h"
#include "CvImprovementClasses.h"
#include "CvCityConnections.h"
#include "CvGameCoreEnumSerialization.h" //toString(const YieldTypes& v)

// include after all other headers
#include "LintFree.h"

CvWeightedVector<BuilderDirective, 100, true> CvBuilderTaskingAI::m_aDirectives;
FStaticVector<int, SAFE_ESTIMATE_NUM_EXTRA_PLOTS, true, c_eCiv5GameplayDLL, 0> CvBuilderTaskingAI::m_aiNonTerritoryPlots; // plots that we need to evaluate that are outside of our territory

/// Constructor
#ifdef AUI_WARNING_FIXES
CvBuilderTaskingAI::CvBuilderTaskingAI() : m_aiCurrentPlotYields(), m_aiProjectedPlotYields()
#else
CvBuilderTaskingAI::CvBuilderTaskingAI(void)
#endif
{
	Uninit();
}

/// Destructor
CvBuilderTaskingAI::~CvBuilderTaskingAI(void)
{
	Uninit();
}

/// Init
void CvBuilderTaskingAI::Init(CvPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_eRepairBuild = GetRepairBuild();
	m_eFalloutFeature = GetFalloutFeature();
	m_eFalloutRemove = GetFalloutRemove();

	m_aiNonTerritoryPlots.clear();
	m_bLogging = GC.getLogging() && GC.getAILogging() && GC.GetBuilderAILogging();
	m_iNumCities = -1;
	m_pTargetPlot = NULL;

#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	// special case code so the Dutch don't remove marshes
	m_bKeepMarshes = false;
	// special case code so Brazil doesn't remove jungle
	m_bKeepJungle = false;

#ifdef AUI_WARNING_FIXES
	for (uint i = 0; i < GC.getNumBuildInfos(); i++)
#else
	for(int i = 0; i < GC.getNumBuildInfos(); i++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)i;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(!pkBuild)
		{
			continue;
		}
		
		ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
		if(eImprovement == NO_IMPROVEMENT)
		{
			continue;
		}

		CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(eImprovement);
		if(pkImprovementInfo == NULL)
		{
			continue;
		}

		if(pkImprovementInfo->IsSpecificCivRequired())
		{
			CivilizationTypes eCiv = pkImprovementInfo->GetRequiredCivilization();
			if(eCiv == pPlayer->getCivilizationType())
			{
				if(pkImprovementInfo->GetFeatureMakesValid(FEATURE_MARSH))
				{
					m_bKeepMarshes = true;
				}
				else if (pkImprovementInfo->GetFeatureMakesValid(FEATURE_JUNGLE))
				{
					m_bKeepJungle = true;
				}
			}
		}
	}
#endif
}

/// Uninit
void CvBuilderTaskingAI::Uninit(void)
{
	m_eRepairBuild = NO_BUILD;
	m_pPlayer = NULL;
	m_bLogging = false;
	m_iNumCities = -1;
	m_pTargetPlot = NULL;
#ifdef AUI_WARNING_FIXES
	m_eFalloutFeature = NO_FEATURE;
	m_eFalloutRemove = NO_BUILD;
#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	m_bKeepJungle = false;
	m_bKeepMarshes = false;
#endif
#endif
}

/// Serialization read
void CvBuilderTaskingAI::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;

	kStream >> m_eRepairBuild;

	uint uiNumSlots;

	// non-territory plots
	kStream >> uiNumSlots;
	m_aiNonTerritoryPlots.resize(uiNumSlots);

	for(uint ui = 0; ui < uiNumSlots; ui++)
	{
		kStream >> m_aiNonTerritoryPlots[ui];
	}

#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	kStream >> m_bKeepMarshes;
	if (uiVersion >= 2)
	{
		kStream >> m_bKeepJungle;
	}
	else
	{
		m_bKeepJungle = false;
	}
#endif
		
	m_iNumCities = -1; //Force everyone to do an CvBuilderTaskingAI::Update() after loading
	m_pTargetPlot = NULL;		//Force everyone to recalculate current yields after loading.
}

/// Serialization write
void CvBuilderTaskingAI::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 2;
	kStream << uiVersion;

	kStream << m_eRepairBuild;

	// non-territory plots
	kStream << m_aiNonTerritoryPlots.size();
	for(uint ui = 0; ui < m_aiNonTerritoryPlots.size(); ui++)
	{
		kStream << m_aiNonTerritoryPlots[ui];
	}

#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	kStream << m_bKeepMarshes;
	kStream << m_bKeepJungle;
#endif
}

/// Update
void CvBuilderTaskingAI::Update(void)
{
	UpdateRoutePlots();
	m_iNumCities = m_pPlayer->getNumCities();

	int iLoop;
	CvCity* pCity;
	for(pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
	{
		pCity->GetCityStrategyAI()->UpdateBestYields();
	}

	if(m_bLogging)
	{
		bool bShowOutput = m_pPlayer->isHuman();

		if(m_pPlayer->IsEmpireUnhappy())
		{
			CvString str = "// Empire Unhappy! //";
			LogInfo(str, m_pPlayer, bShowOutput);
		}

		// show crisis states
		CvCity* pLoopCity;
		for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
		{
			CvString str;
			str += "// ";
			CvString strCityName;
			strCityName = pLoopCity->getName();
			str += strCityName;
			str += " \\\\";

			LogInfo(str, m_pPlayer, bShowOutput);

#ifdef AUI_WARNING_FIXES
			for (int ui = 0; ui < NUM_YIELD_TYPES; ui++)
#else
			for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
#endif
			{
				//double fYield = pLoopCity->GetCityStrategyAI()->GetYieldAverage((YieldTypes)ui);
				//double fYieldDeficient = pLoopCity->GetCityStrategyAI()->GetDeficientYieldValue((YieldTypes)ui);
				CvString strYield;
				switch(ui)
				{
				case YIELD_FOOD:
					strYield = "food       ";
					break;
				case YIELD_PRODUCTION:
					strYield = "production ";
					break;
				case YIELD_SCIENCE:
					strYield = "science    ";
					break;
				case YIELD_GOLD:
					strYield = "gold       ";
					break;
				case YIELD_CULTURE:
					strYield = "culture    ";
					break;
				case YIELD_FAITH:
					strYield = "faith      ";
					break;
				}

				CvString strNumbers;
				strNumbers.Format("%d, %d", pLoopCity->GetCityStrategyAI()->GetBestYieldAverageTimes100((YieldTypes)ui), pLoopCity->GetCityStrategyAI()->GetYieldDeltaTimes100((YieldTypes)ui));

				//int iYieldAdjusted = (int)workerround(fYield * 100);
				//int iYieldDeficientAdjacent = (int)workerround(fYieldDeficient * 100);

				//strNumbers.Format("%d / %d", iYieldAdjusted, iYieldDeficientAdjacent);
				strYield += strNumbers;

				if(ui == pLoopCity->GetCityStrategyAI()->GetFocusYield())
				{
					strYield += " *";
				}

				//if (iYieldAdjusted < iYieldDeficientAdjacent)
				//{
				//if (GetDeficientYield(pLoopCity, false) != GetDeficientYield(pLoopCity, true))
				//{
				//	strYield += "  Problem, but happiness over is overriding it";
				//}
				//else
				//{
				//	strYield += "  PROBLEM!!";
				//}
				//}
				LogInfo(strYield, m_pPlayer, bShowOutput);
			}

			str = "\\\\ end ";
			str += strCityName;
			str += " //";
			LogInfo(str, m_pPlayer, bShowOutput);
		}
	}
}

int GetPlotYield(CvPlot* pPlot, YieldTypes eYield)
{
	if(pPlot->getTerrainType() == NO_TERRAIN)
	{
		return 0;
	}

	return pPlot->calculateNatureYield(eYield, NO_TEAM);;
}

void CvBuilderTaskingAI::ConnectCitiesToCapital(CvCity* pPlayerCapital, CvCity* pTargetCity, RouteTypes eRoute)
{
	bool bMajorMinorConnection = false;
	if(pTargetCity->getOwner() != pPlayerCapital->getOwner())
	{
		bMajorMinorConnection = true;
	}

	bool bIndustrialRoute = false;
	if(GC.getGame().GetIndustrialRoute() == eRoute)
	{
		bIndustrialRoute = true;
	}

	// if we already have a connection, bail out
	if(bIndustrialRoute && pTargetCity->IsIndustrialRouteToCapital())
	{
		return;
	}
	else if(m_pPlayer->IsCapitalConnectedToCity(pTargetCity, eRoute))
	{
		return;
	}

	int iGoldForRoute = 0;
	if(!bMajorMinorConnection)
	{
		iGoldForRoute = m_pPlayer->GetTreasury()->GetCityConnectionRouteGoldTimes100(pTargetCity) / 100;
	}

	CvRouteInfo* pRouteInfo = GC.getRouteInfo(eRoute);
	if(!pRouteInfo)
	{
		return;
	}
	int iMaintenancePerTile = pRouteInfo->GetGoldMaintenance();
	if(iMaintenancePerTile < 0)  // div by zero check
	{
		return;
	}

	// build a path between the two cities
	int iPathfinderFlags = m_pPlayer->GetID();
	int iRouteValue = eRoute + 1;
	// assuming that there are fewer than 256 players
	iPathfinderFlags |= (iRouteValue << 8);
	bool bFoundPath = GC.GetBuildRouteFinder().GeneratePath(pPlayerCapital->plot()->getX(), pPlayerCapital->plot()->getY(), pTargetCity->plot()->getX(), pTargetCity->plot()->getY(), iPathfinderFlags);

	//  if no path, then bail!
	if(!bFoundPath)
	{
		return;
	}

	// walk the path
	CvPlot* pPlot = NULL;

	// go through the route to see how long it is and how many plots already have roads
	int iRoadLength = 0;
	int iPlotsNeeded = 0;
	CvAStarNode* pNode = GC.GetBuildRouteFinder().GetLastNode();
#ifdef AUI_WORKER_INCA_HILLS
	bool bIncaBonusActive = (m_pPlayer->GetPlayerTraits()->IsNoHillsImprovementMaintenance() && !m_pPlayer->isHuman());
#endif
	while(pNode)
	{
		pPlot = GC.getMap().plotCheckInvalid(pNode->m_iX, pNode->m_iY);
		pNode = pNode->m_pParent;
		if(!pPlot)
		{
			break;
		}

		CvCity* pCity = pPlot->getPlotCity();
		if(NULL != pCity && pCity->getTeam() == m_pPlayer->getTeam())
		{
			continue;
		}

#ifdef AUI_WORKER_INCA_HILLS
		if (!bIncaBonusActive || !pPlot->isHills())
		{
#endif
		if(pPlot->getRouteType() == eRoute && !pPlot->IsRoutePillaged())
		{
			// if this is already a trade route or someone else built it, we can count is as free
			if(pPlot->IsTradeRoute(m_pPlayer->GetID()) || pPlot->GetPlayerResponsibleForRoute() != m_pPlayer->GetID())
			{
				continue;
			}
			iRoadLength++;
		}
		else
		{
			iRoadLength++;
			iPlotsNeeded++;
		}
#ifdef AUI_WORKER_INCA_HILLS
		}
#endif
	}

#ifdef AUI_WORKER_INCA_HILLS
	int iFreeIncaRoadLength = 0;
	if (bIncaBonusActive)
	{
		if (iRoadLength == 0)
		{
			iRoadLength = 1;
			iFreeIncaRoadLength = 1;
		}
		if (iPlotsNeeded == 0)
		{
			iPlotsNeeded = 1;
		}
	}
#endif

	// This is very odd
	if(iRoadLength <= 0 || iPlotsNeeded <= 0)
	{
		return;
	}


#ifdef AUI_WARNING_FIXES
	int sValue = -1;
#else
	short sValue = -1;
#endif
#ifdef AUI_WORKER_FIX_CONNECT_CITIES_TO_CAPITOL_CONSIDER_MAINTENANCE_MODIFIERS
#ifdef AUI_WORKER_INCA_HILLS
	int iTotalMaintenance = (iRoadLength - iFreeIncaRoadLength) * iMaintenancePerTile;
#else
	int iTotalMaintenance = iRoadLength * iMaintenancePerTile;
#endif
	// Player modifier
	iTotalMaintenance *= (100 + m_pPlayer->GetRouteGoldMaintenanceMod());
	iTotalMaintenance /= 100;
	// Handicap
	iTotalMaintenance *= m_pPlayer->getHandicapInfo().getRouteCostPercent();
	iTotalMaintenance /= 100;
#ifdef AUI_WORKER_INCA_HILLS
	int iProfit = iGoldForRoute - iTotalMaintenance;
#else
	int iProfit = iGoldForRoute - iTotalMaintenance;
#endif
#elif defined(AUI_WORKER_INCA_HILLS)
	int iProfit = iGoldForRoute - ((iRoadLength - iFreeIncaRoadLength) * iMaintenancePerTile);
#else
	int iProfit = iGoldForRoute - (iRoadLength * iMaintenancePerTile);
#endif
	if(bIndustrialRoute)
	{
		if(iProfit >= 0)
		{
#ifdef AUI_WARNING_FIXES
			sValue = MAX_INT;
#else
			sValue = MAX_SHORT;
#endif
		}
		else if(m_pPlayer->calculateGoldRate() + iProfit >= 0)
		{
			sValue = pTargetCity->getYieldRate(YIELD_PRODUCTION, false) * GC.getINDUSTRIAL_ROUTE_PRODUCTION_MOD();
		}
		else
		{
			return;
		}
	}
	else if(bMajorMinorConnection)
	{
#ifdef AUI_WARNING_FIXES
		sValue = MIN(GC.getMINOR_CIV_ROUTE_QUEST_WEIGHT() / iPlotsNeeded, MAX_INT);
#else
		sValue = min(GC.getMINOR_CIV_ROUTE_QUEST_WEIGHT() / iPlotsNeeded, MAX_SHORT);
#endif
	}
	else // normal route
	{
		// is this route worth building?
		if(iProfit < 0)
		{
			return;
		}

#ifdef AUI_WORKER_FIX_CONNECT_CITIES_TO_CAPITOL_CONSIDER_MAINTENANCE_MODIFIERS
		int iValue = iGoldForRoute * 100 * iRoadLength;
		if (iTotalMaintenance + iRoadLength > iFreeIncaRoadLength)
			iValue /= (iTotalMaintenance + iRoadLength - iFreeIncaRoadLength);
		iValue /= iPlotsNeeded;
#else
		int iValue = (iGoldForRoute * 100) / (iRoadLength * (iMaintenancePerTile + 1));
#ifdef AUI_WORKER_INCA_HILLS
		iValue = (iValue * iRoadLength * (1 + iFreeIncaRoadLength * iMaintenancePerTile)) / iPlotsNeeded;
#else
		iValue = (iValue * iRoadLength) / iPlotsNeeded;
#endif
#endif
#ifdef AUI_WARNING_FIXES
		sValue = MIN(iValue, MAX_INT);
#else
		sValue = min(iValue, MAX_SHORT);
#endif
	}

	pPlot = NULL;
	pNode = GC.GetBuildRouteFinder().GetLastNode();

	int iGameTurn = GC.getGame().getGameTurn();

	while(pNode)
	{
		pPlot = GC.getMap().plotCheckInvalid(pNode->m_iX, pNode->m_iY);
		pNode = pNode->m_pParent;

		if(!pPlot)
		{
			break;
		}

		if(pPlot->getRouteType() == eRoute && !pPlot->IsRoutePillaged())
		{
			continue;
		}

		// if we already know about this plot, continue on
		if(pPlot->GetBuilderAIScratchPadTurn() == iGameTurn && pPlot->GetBuilderAIScratchPadPlayer() == m_pPlayer->GetID())
		{
			if(sValue > pPlot->GetBuilderAIScratchPadValue())
			{
				pPlot->SetBuilderAIScratchPadValue(sValue);
				pPlot->SetBuilderAIScratchPadRoute(eRoute);
			}
			continue;
		}

		// mark nodes and reset values
		pPlot->SetBuilderAIScratchPadTurn(iGameTurn);
		pPlot->SetBuilderAIScratchPadPlayer(m_pPlayer->GetID());
		pPlot->SetBuilderAIScratchPadValue(sValue);
		pPlot->SetBuilderAIScratchPadRoute(eRoute);

		// add nodes that are not in territory to extra list
		// minors should not build out of their borders when they are doing a major/minor connection, only when connecting their two cities
		if(!(m_pPlayer->isMinorCiv() && bMajorMinorConnection))
		{
			if(pPlot->getOwner() != m_pPlayer->GetID())
			{
				m_aiNonTerritoryPlots.push_back(GC.getMap().plotNum(pPlot->getX(), pPlot->getY()));
			}
		}
	}
}

void CvBuilderTaskingAI::ConnectCitiesForScenario(CvCity* pCity1, CvCity* pCity2, RouteTypes eRoute)
{
	// don't connect cities from different owners
	if(pCity1->getOwner() != pCity2->getOwner())
	{
		return;
	}

	// if we already have a connection, bail out
	if (m_pPlayer->IsCityConnectedToCity(pCity1, pCity2, eRoute, true))
	{
		return;
	}

	// build a path between the two cities
	int iPathfinderFlags = m_pPlayer->GetID();
	int iRouteValue = eRoute + 1;
	// assuming that there are fewer than 256 players
	iPathfinderFlags |= (iRouteValue << 8);
	bool bFoundPath = GC.GetBuildRouteFinder().GeneratePath(pCity1->plot()->getX(), pCity1->plot()->getY(), pCity2->plot()->getX(), pCity2->plot()->getY(), iPathfinderFlags);

	//  if no path, then bail!
	if(!bFoundPath)
	{
		return;
	}

	CvPlot* pPlot = NULL;
	CvAStarNode* pNode = GC.GetBuildRouteFinder().GetLastNode();
	int iGameTurn = GC.getGame().getGameTurn();

	while(pNode)
	{
		pPlot = GC.getMap().plotCheckInvalid(pNode->m_iX, pNode->m_iY);
		pNode = pNode->m_pParent;

		if(!pPlot)
		{
			break;
		}

		if(pPlot->getRouteType() == eRoute && !pPlot->IsRoutePillaged())
		{
			continue;
		}

		// if we already know about this plot, continue on
		if(pPlot->GetBuilderAIScratchPadTurn() == iGameTurn && pPlot->GetBuilderAIScratchPadPlayer() == m_pPlayer->GetID())
		{
			continue;
		}

		// mark nodes and reset values
		pPlot->SetBuilderAIScratchPadTurn(iGameTurn);
		pPlot->SetBuilderAIScratchPadPlayer(m_pPlayer->GetID());
		pPlot->SetBuilderAIScratchPadValue(1000);
		pPlot->SetBuilderAIScratchPadRoute(eRoute);

		// add nodes that are not in territory to extra list
		if(pPlot->getOwner() != m_pPlayer->GetID())
		{
			m_aiNonTerritoryPlots.push_back(GC.getMap().plotNum(pPlot->getX(), pPlot->getY()));
		}
	}
}

/// Looks at city connections and marks plots that can be added as routes by EvaluateBuilder
void CvBuilderTaskingAI::UpdateRoutePlots(void)
{
	m_aiNonTerritoryPlots.clear();

	// if there are fewer than 2 cities, we don't need to run this function
	if(m_pPlayer->GetCityConnections()->GetNumConnectableCities() < 2)
	{
		return;
	}

	RouteTypes eBestRoute = m_pPlayer->getBestRoute();
	if(eBestRoute == NO_ROUTE)
	{
		return;
	}

	// find a builder, if I don't have a builder, bail!
	CvUnit* pBuilder = NULL;
	CvUnit* pLoopUnit;
	int iLoopUnit;
	for(pLoopUnit = m_pPlayer->firstUnit(&iLoopUnit); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoopUnit))
	{
		if(pLoopUnit->AI_getUnitAIType() == UNITAI_WORKER)
		{
			pBuilder = pLoopUnit;
			break;
		}
	}

	// If there's no builder, bail!
	if(!pBuilder)
	{
		return;
	}

	// updating plots that are part of the road network
	CvCityConnections* pCityConnections = m_pPlayer->GetCityConnections();

#ifdef AUI_WARNING_FIXES
	for (uint i = 0; i < GC.getNumBuildInfos(); i++)
#else
	for(int i = 0; i < GC.getNumBuildInfos(); i++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)i;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(!pkBuild)
		{
			continue;
		}

		RouteTypes eRoute = (RouteTypes)pkBuild->getRoute();
		if(eRoute == NO_ROUTE)
		{
			continue;
		}

		if(GC.getBuildInfo(eBuild)->getTechPrereq() != NO_TECH)
		{
			bool bHasTech = GET_TEAM(m_pPlayer->getTeam()).GetTeamTechs()->HasTech((TechTypes)GC.getBuildInfo(eBuild)->getTechPrereq());
			if(!bHasTech)
			{
				continue;
			}
		}

		for(uint uiFirstCityIndex = 0; uiFirstCityIndex < pCityConnections->m_aiCityPlotIDs.size(); uiFirstCityIndex++)
		{
			for(uint uiSecondCityIndex = uiFirstCityIndex + 1; uiSecondCityIndex < pCityConnections->m_aiCityPlotIDs.size(); uiSecondCityIndex++)
			{
				// get the two cities
				CvCity* pFirstCity  = pCityConnections->GetCityFromIndex(uiFirstCityIndex);
				CvCity* pSecondCity = pCityConnections->GetCityFromIndex(uiSecondCityIndex);
				CvCity* pPlayerCapitalCity = NULL;
				CvCity* pTargetCity = NULL;

				if(!pFirstCity || !pSecondCity)
				{
					continue;
				}

				bool bConnectOnlyCapitals = (bool)GC.getCITY_CONNECTIONS_CONNECT_TO_CAPITAL();
				if (bConnectOnlyCapitals)
				{
					// only need to build roads to the capital
					if(!pFirstCity->isCapital() && !pSecondCity->isCapital())
					{
						continue;
					}

					if(pFirstCity->isCapital() && pFirstCity->getOwner() == m_pPlayer->GetID())
					{
						pPlayerCapitalCity = pFirstCity;
						pTargetCity = pSecondCity;
					}
					else
					{
						pPlayerCapitalCity = pSecondCity;
						pTargetCity = pFirstCity;
					}
					ConnectCitiesToCapital(pPlayerCapitalCity, pTargetCity, eRoute);
				}
				else
				{
					ConnectCitiesForScenario(pFirstCity, pSecondCity, eBestRoute);
				}
			}
		}
	}
}

int CorrectWeight(int iWeight)
{
	if(iWeight < -1000)
	{
		return MAX_INT;
	}
	else
	{
		return iWeight;
	}
}

/// Use the flavor settings to determine what the worker should do
bool CvBuilderTaskingAI::EvaluateBuilder(CvUnit* pUnit, BuilderDirective* paDirectives, UINT uaDirectives, bool bOnlyKeepBest, bool bOnlyEvaluateWorkersPlot)
{
	// number of cities has changed mid-turn, so we need to re-evaluate what workers should do
	if(m_pPlayer->getNumCities() != m_iNumCities)
	{
		Update();
	}

	CvAssertMsg(uaDirectives > 0, "Need more than one directive");

	for(uint ui = 0; ui < uaDirectives; ui++)
	{
		paDirectives[ui].m_eDirective = BuilderDirective::NUM_DIRECTIVES;
	}

	m_aDirectives.clear();

	// check for no brainer bail-outs
	// if the builder is already building something
#ifdef AUI_WORKER_EVALUATE_WORKER_RETREAT_AND_BUILD
#ifdef AUI_DANGER_PLOTS_REMADE
	if (pUnit->getBuildType() != NO_BUILD && pUnit->GetCurrHitPoints() > m_pPlayer->GetPlotDanger(*pUnit->plot(), pUnit))
#elif defined(AUI_WORKER_SHOULD_BUILDER_CONSIDER_PLOT_MAXIMUM_DANGER_BASED_ON_UNIT_STRENGTH)
	if (pUnit->getBuildType() != NO_BUILD && 
		((pUnit->IsCombatUnit() && pUnit->GetBaseCombatStrengthConsideringDamage() * AUI_WORKER_SHOULD_BUILDER_CONSIDER_PLOT_MAXIMUM_DANGER_BASED_ON_UNIT_STRENGTH > m_pPlayer->GetPlotDanger(*pUnit->plot())) ||
		(!pUnit->IsCombatUnit() && !m_pPlayer->IsPlotUnderImmediateThreat(*pUnit->plot()))))
#else
	if (pUnit->getBuildType() != NO_BUILD && !m_pPlayer->IsPlotUnderImmediateThreat(*pUnit->plot())))
#endif
#else
	if(pUnit->getBuildType() != NO_BUILD)
#endif
	{
		paDirectives[0].m_eDirective = BuilderDirective::BUILD_IMPROVEMENT;
		paDirectives[0].m_eBuild = pUnit->getBuildType();
		paDirectives[0].m_sX = pUnit->getX();
		paDirectives[0].m_sY = pUnit->getY();
		//inDirective.m_sGoldCost = 0;
		paDirectives[0].m_sMoveTurnsAway = 0;
		return true;
	}

	m_aiPlots.clear();
	if(bOnlyEvaluateWorkersPlot)
	{
		// can't build on plots others own
		PlayerTypes eOwner = pUnit->plot()->getOwner();
		if(eOwner == m_pPlayer->GetID())
		{
			m_aiPlots.push_back(pUnit->plot()->GetPlotIndex());
		}
	}
	else
	{
		m_aiPlots = m_pPlayer->GetPlots();
	}

#ifdef AUI_WORKER_ADD_IMPROVING_MINOR_PLOTS_DIRECTIVES
	for (uint iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
	{
		CvBuildInfo* pkBuild = GC.getBuildInfo((BuildTypes)iBuildIndex);
		if (pkBuild)
		{
			ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
			if (eImprovement != NO_IMPROVEMENT)
			{
				CvImprovementEntry* pkImprovement = GC.getImprovementInfo(eImprovement);
				if (pkImprovement && pkImprovement->IsOnlyCityStateTerritory() && (!pkImprovement->IsSpecificCivRequired() || pkImprovement->GetRequiredCivilization() == m_pPlayer->getCivilizationType()))
				{
					for (int iJ = 0; iJ < MAX_CIV_PLAYERS; iJ++)
					{
						CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iJ);
						if (kLoopPlayer.isAlive() && kLoopPlayer.isMinorCiv() && !GET_TEAM(m_pPlayer->getTeam()).isAtWar(kLoopPlayer.getTeam()))
						{
							m_aiPlots.push_back(kLoopPlayer.GetPlots().begin(), kLoopPlayer.GetPlots().size());
						}
					}
					break;
				}
			}
		}
	}
#endif

	// go through all the plots the player has under their control
	for(uint uiPlotIndex = 0; uiPlotIndex < m_aiPlots.size(); uiPlotIndex++)
	{
		// when we encounter the first plot that is invalid, the rest of the list will be invalid
		if(m_aiPlots[uiPlotIndex] == -1)
		{
			if(m_bLogging)
			{
				CvString strLog = "end of plot list";
				LogInfo(strLog, m_pPlayer);
			}
			break;
		}

		CvPlot* pPlot = GC.getMap().plotByIndex(m_aiPlots[uiPlotIndex]);

		if(!ShouldBuilderConsiderPlot(pUnit, pPlot))
		{
			continue;
		}

		// distance weight
		// find how many turns the plot is away
		int iMoveTurnsAway = FindTurnsAway(pUnit, pPlot);
		if(iMoveTurnsAway < 0)
		{
			if(m_bLogging)
			{
				CvString strLog;
				strLog.Format("unitx: %d unity: %d, plotx: %d ploty: %d, can't find path", pUnit->getX(), pUnit->getY(), pPlot->getX(), pPlot->getY());
				LogInfo(strLog, m_pPlayer);
			}

			continue;
		}

#ifdef AUI_WORKER_ADD_IMPROVING_MINOR_PLOTS_DIRECTIVES
		if (pPlot->getOwner() != m_pPlayer->GetID())
			AddImprovingMinorPlotsDirectives(pUnit, pPlot, iMoveTurnsAway);
		else
		{
#endif
		UpdateCurrentPlotYields(pPlot);

		//AddRepairDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddRouteDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddImprovingResourcesDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddImprovingPlotsDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddChopDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddScrubFalloutDirectives(pUnit, pPlot, iMoveTurnsAway);
		// only AIs have permission to remove roads
		if(!m_pPlayer->isHuman())
		{
			//AddRemoveUselessRoadDirectives(pUnit, pPlot, iMoveTurnsAway);
		}
#ifdef AUI_WORKER_ADD_IMPROVING_MINOR_PLOTS_DIRECTIVES
		}
#endif
	}

	// we need to evaluate the tiles outside of our territory to build roads
	for(uint ui = 0; ui < m_aiNonTerritoryPlots.size(); ui++)
	{
		//FAssertMsg(!m_pPlayer->isMinorCiv(), "MinorCivs should have no nonterritory plots");
		CvPlot* pPlot = GC.getMap().plotByIndex(m_aiNonTerritoryPlots[ui]);
		CvAssertMsg(pPlot != NULL, "Plot should not be NULL");
		if(!pPlot)
			continue;

		if(bOnlyEvaluateWorkersPlot)
		{
			if(pPlot != pUnit->plot())
			{
				continue;
			}
		}

		if(!ShouldBuilderConsiderPlot(pUnit, pPlot))
		{
			continue;
		}

		// distance weight
		// find how many turns the plot is away
		int iMoveTurnsAway = FindTurnsAway(pUnit, pPlot);
		if(iMoveTurnsAway < 0)
		{
			if(m_bLogging)
			{
				CvString strLog;
				strLog.Format("unitx: %d unity: %d, plotx: %d ploty: %d, Evaluating out of territory plot, can't find path", pUnit->getX(), pUnit->getY(), pPlot->getX(), pPlot->getY());
				LogInfo(strLog, m_pPlayer);
			}

			continue;
		}

		if(m_bLogging)
		{
			CvString strLog;
			strLog.Format("x: %d y: %d, Evaluating out of territory plot", pPlot->getX(), pPlot->getY());
			LogInfo(strLog, m_pPlayer);
		}

		//AddRepairDirectives(pUnit, pPlot, iMoveTurnsAway);
		AddRouteDirectives(pUnit, pPlot, iMoveTurnsAway);
	}

	m_aDirectives.StableSortItems();

	int iBestWeight = 0;

	int iAssignIndex = 0;
	for(int i = 0; i < m_aDirectives.size(); i++)
	{
		// If this target was far away, we only estimated the time to get there.  We need to be sure we have a real path there
		CvPlot* pTarget = GC.getMap().plot(m_aDirectives.GetElement(i).m_sX, m_aDirectives.GetElement(i).m_sY);
		CvAssertMsg(pTarget != NULL, "Not expecting the target to be NULL");
		if(!pTarget)
			continue;

#if 0	// KWG: We are now always doing a raw estimate when gathering the directives
		int iPlotDistance = plotDistance(pUnit->getX(), pUnit->getY(), pTarget->getX(), pTarget->getY());
		if(iPlotDistance >= GC.getAI_HOMELAND_ESTIMATE_TURNS_DISTANCE())
#endif
		{
			if(TurnsToReachTarget(pUnit, pTarget) == MAX_INT)
			{
				// No path, need to pick a new directive
				continue;
			}
		}

		if(iBestWeight == 0)
		{
			iBestWeight = m_aDirectives.GetWeight(i);
		}

		if(bOnlyKeepBest)
		{
			int iWeight = m_aDirectives.GetWeight(i);
			if(iWeight < iBestWeight * 3 / 4)
			{
				break;
			}
		}

		BuilderDirective directive = m_aDirectives.GetElement(i);
		paDirectives[iAssignIndex] = directive;
		iAssignIndex++;

		// if we shouldn't copy over any more directives, then break
		if(iAssignIndex >= (int)uaDirectives)
		{
			break;
		}
	}

	if(m_bLogging)
	{
		if(m_aDirectives.size() > 0)
		{
			//LogFlavors(NO_FLAVOR);
		}

		LogDirectives(pUnit);
	}

	//if (m_aDirectives.size() > 0 && iAssignIndex > 0)
	if(iAssignIndex > 0)
	{
		if(m_bLogging)
		{
			LogDirective(paDirectives[0], pUnit, -1, true /*bChosen*/);
		}
		return true;
	}

	return false;
}


/// Evaluating a plot to see if we can build resources there
void CvBuilderTaskingAI::AddImprovingResourcesDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
#ifdef AUI_WARNING_FIXES
	if (!pUnit)
		return;
#endif

	ImprovementTypes eExistingPlotImprovement = pPlot->getImprovementType();

	// Do we have a special improvement here? (great person improvement, gifted improvement from major civ)
	if(eExistingPlotImprovement != NO_IMPROVEMENT && pPlot->HasSpecialImprovement() && !pPlot->IsImprovementPillaged())
	{
		return;
	}

	// check to see if a resource is here. If not, bail out!
	ResourceTypes eResource = pPlot->getResourceType(m_pPlayer->getTeam());
	if(eResource == NO_RESOURCE)
	{
		return;
	}

	CvResourceInfo* pkResource = GC.getResourceInfo(eResource);
	if(pkResource->getResourceUsage() == RESOURCEUSAGE_BONUS)
	{
		// evaluate bonus resources as normal improvements
		return;
	}

#ifdef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a conflicting mission here
	if (pUnit->GetMissionAIPlot() != pPlot)
	{
		int iLoop;
		for (CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pUnit == pLoopUnit)
			{
				continue;
			}

			CvPlot* pMissionPlot = pLoopUnit->GetMissionAIPlot();
			if (!pMissionPlot)
			{
				continue;
			}

			MissionAITypes eGroupMissionAI = pLoopUnit->GetMissionAIType();
			if (eGroupMissionAI != MISSIONAI_BUILD)
			{
				continue;
			}

			if (pPlot->getX() == pMissionPlot->getX() && pPlot->getY() == pMissionPlot->getY())
			{
				BuildTypes eOtherBuild = pLoopUnit->getBuildType();
				CvBuildInfo* pkBuild = GC.getBuildInfo(eOtherBuild);
				if (pkBuild != NULL)
				{
					ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
					if (eImprovement != NO_IMPROVEMENT || (eOtherBuild == m_eRepairBuild && eExistingPlotImprovement != NO_IMPROVEMENT))
					{
						if (m_bLogging)
						{
							CvString strLog;
							strLog.Format("x: %d y: %d, Somebody has a conflicting mission here.", pPlot->getX(), pPlot->getY());
							LogInfo(strLog, m_pPlayer, true);
						}
						return;
					}
				}
			}
		}
	}
#endif

	// loop through the build types to find one that we can use
	BuildTypes eBuild;
	BuildTypes eOriginalBuild;
#ifdef AUI_WARNING_FIXES
	uint iBuildIndex;
#else
	int iBuildIndex;
#endif
	for(iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
	{
		eBuild = (BuildTypes)iBuildIndex;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(pkBuild == NULL)
			continue;

		eOriginalBuild = eBuild;
		ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
		if(eImprovement == NO_IMPROVEMENT)
		{
			continue;
		}

		CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(eImprovement);
		if(pkImprovementInfo == NULL || !pkImprovementInfo->IsImprovementResourceTrade(eResource))
		{
			continue;
		}
#if defined(LEKMOD_v34) // Moai and Brazilwoods can now connect resources, this can mess with other civs that cannot make this improvement when it comes to BuilderAI
		// So add a check to see if we can even make the Improvement the Build Requires. if not, then continue looping.
		CivilizationTypes eImprovementCiv = pkImprovementInfo->GetRequiredCivilization();
		if (eImprovementCiv != NO_CIVILIZATION && eImprovementCiv != m_pPlayer->getCivilizationType())
		{
			continue;
		}
		// Check if the plot can even have this improvement. If not then continue looping.
		if(!pPlot->canHaveImprovement(eImprovement))
		{
			continue;
		}
#endif
		if(eImprovement == eExistingPlotImprovement)
		{
			if(pPlot->IsImprovementPillaged())
			{
				eBuild = m_eRepairBuild;
			}
			else
			{
				// this plot already has the appropriate improvement to use the resource
				break;
			}
		}
		else
		{
			// Do we have a special improvement here? (great person improvement, gifted improvement from major civ)
			if (eExistingPlotImprovement != NO_IMPROVEMENT && pPlot->HasSpecialImprovement())
				continue;
		}

		if(!pUnit->canBuild(pPlot, eBuild))
		{
			break;
		}

		BuilderDirective::BuilderDirectiveType eDirectiveType = BuilderDirective::BUILD_IMPROVEMENT_ON_RESOURCE;
		int iWeight = GC.getBUILDER_TASKING_BASELINE_BUILD_RESOURCE_IMPROVEMENTS();
		if(eBuild == m_eRepairBuild)
		{
			eDirectiveType = BuilderDirective::REPAIR;
			iWeight = GC.getBUILDER_TASKING_BASELINE_REPAIR();
		}

		iWeight = GetBuildCostWeight(iWeight, pPlot, eBuild);

		// this is to deal with when the plot is already improved with another improvement that doesn't enable the resource
		int iInvestedImprovementTime = 0;
		if(eExistingPlotImprovement != NO_IMPROVEMENT)
		{
			BuildTypes eExistingBuild = NO_BUILD;
			BuildTypes eBuild2 = NO_BUILD;
#ifdef AUI_WARNING_FIXES
			for (uint iBuildIndex2 = 0; iBuildIndex2 < GC.getNumBuildInfos(); iBuildIndex2++)
#else
			for(int iBuildIndex2 = 0; iBuildIndex2 < GC.getNumBuildInfos(); iBuildIndex2++)
#endif
			{
				eBuild2 = (BuildTypes)iBuildIndex2;
				CvBuildInfo* pkBuild2 = GC.getBuildInfo(eBuild2);
				if(pkBuild2 && pkBuild2->getImprovement() == eExistingPlotImprovement)
				{
					eExistingBuild = eBuild2;
					break;
				}
			}

			if(eExistingBuild != NO_BUILD)
			{
				iInvestedImprovementTime = pPlot->getBuildTime(eExistingBuild, m_pPlayer->GetID());
			}
		}

		int iBuildTimeWeight = GetBuildTimeWeight(pUnit, pPlot, eBuild, DoesBuildHelpRush(pUnit, pPlot, eBuild), iInvestedImprovementTime + iMoveTurnsAway);
		iWeight += iBuildTimeWeight;
		iWeight = CorrectWeight(iWeight);

		iWeight += GetResourceWeight(eResource, eImprovement, pPlot->getNumResource());
		iWeight = CorrectWeight(iWeight);

		UpdateProjectedPlotYields(pPlot, eBuild);
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
		int iScore = ScorePlot(eBuild);
#else
		int iScore = ScorePlot();
#endif
		if(iScore > 0)
		{
			iWeight *= iScore;
			iWeight = CorrectWeight(iWeight);
		}

		{
			CvCity* pLogCity = NULL;
			int iProduction = pPlot->getFeatureProduction(eBuild, pUnit->getOwner(), &pLogCity);
			if(DoesBuildHelpRush(pUnit, pPlot, eBuild))
			{
				iWeight += iProduction; // a nominal benefit for choosing this production

				if(m_bLogging)
				{
					CvString strLog;
					strLog.Format("Helps rush, %d", iProduction);
					LogInfo(strLog, m_pPlayer);
				}
			}
		}

		if(iWeight <= 0)
		{
			continue;
		}

		BuilderDirective directive;
		directive.m_eDirective = eDirectiveType;
		directive.m_eBuild = eBuild;
		directive.m_eResource = eResource;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		//directive.m_iGoldCost = m_pPlayer->getBuildCost(pPlot, eBuild);
		directive.m_sMoveTurnsAway = iMoveTurnsAway;

		if(m_bLogging)
		{
			CvString strTemp;
			strTemp.Format("%d, Build Time Weight, %d, Weight, %d", pUnit->GetID(), iBuildTimeWeight, iWeight);
			LogInfo(strTemp, m_pPlayer);
		}

		m_aDirectives.push_back(directive, iWeight);
	}
}

/// Evaluating a plot to determine what improvement could be best there
void CvBuilderTaskingAI::AddImprovingPlotsDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
	ImprovementTypes eExistingImprovement = pPlot->getImprovementType();

	// Do we have a special improvement here? (great person improvement, gifted improvement from major civ)
	if(eExistingImprovement != NO_IMPROVEMENT && pPlot->HasSpecialImprovement() && !pPlot->IsImprovementPillaged())
	{
		return;
	}

	// if it's not within a city radius
	if(!pPlot->isWithinTeamCityRadius(pUnit->getTeam()))
	{
		return;
	}

	// check to see if a non-bonus resource is here. if so, bail out!
	ResourceTypes eResource = pPlot->getResourceType(m_pPlayer->getTeam());
	if(eResource != NO_RESOURCE)
	{
		if(GC.getResourceInfo(eResource)->getResourceUsage() != RESOURCEUSAGE_BONUS)
		{
			return;
		}
	}
#ifdef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	FeatureTypes eFeature = pPlot->getFeatureType();
#endif

	// celtic rule: if this is a forest tile next to a city, do not improve this tile with a normal improvement
	if (m_pPlayer->GetPlayerTraits()->IsFaithFromUnimprovedForest() && eExistingImprovement == NO_IMPROVEMENT)
	{
		CvCity* pNextCity = pPlot->GetAdjacentCity();
		if (pNextCity && pNextCity->getOwner() == m_pPlayer->GetID())
		{
#ifdef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
			if (eFeature == FEATURE_FOREST)
#else
			if (pPlot->getFeatureType() == FEATURE_FOREST)
#endif
			{
				return;
			}
		}
	}

#ifdef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a conflicting mission here
	if (pUnit->GetMissionAIPlot() != pPlot)
	{
		int iLoop;
		for (CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pUnit == pLoopUnit)
			{
				continue;
			}

			CvPlot* pMissionPlot = pLoopUnit->GetMissionAIPlot();
			if (!pMissionPlot)
			{
				continue;
			}

			MissionAITypes eGroupMissionAI = pLoopUnit->GetMissionAIType();
			if (eGroupMissionAI != MISSIONAI_BUILD)
			{
				continue;
			}

			if (pPlot->getX() == pMissionPlot->getX() && pPlot->getY() == pMissionPlot->getY())
			{
				BuildTypes eOtherBuild = pLoopUnit->getBuildType();
				CvBuildInfo* pkBuild = GC.getBuildInfo(eOtherBuild);
				if (pkBuild != NULL)
				{
					ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
					if (eImprovement != NO_IMPROVEMENT || (eOtherBuild == m_eRepairBuild && eExistingImprovement != NO_IMPROVEMENT))
					{
						if (m_bLogging)
						{
							CvString strLog;
							strLog.Format("x: %d y: %d, Somebody has a conflicting mission here.", pPlot->getX(), pPlot->getY());
							LogInfo(strLog, m_pPlayer, true);
						}
						return;
					}
				}
			}
		}
	}
#endif

	CvCity* pCity = GetWorkingCity(pPlot);
#ifndef AUI_WORKER_ADD_IMPROVING_PLOTS_DIRECTIVE_DEFENSIVES
	if(!pCity)
	{
		return;
	}
#endif

	// loop through the build types to find one that we can use
	BuildTypes eBuild;
	BuildTypes eOriginalBuildType;
#ifdef AUI_WARNING_FIXES
	uint iBuildIndex;
#else
	int iBuildIndex;
#endif
#ifdef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
	int iLoopScore;
	int iBestUniqueImprovementWeight = 0;
	for (uint iI = 0; iI < m_pPlayer->GetNumUniqueImprovements(); iI++)
	{
		ImprovementTypes eUniqueImprovement = m_pPlayer->GetUniqueImprovement(iI);
		if (eUniqueImprovement != NO_IMPROVEMENT)
		{
			CvImprovementEntry* pUniqueImprovement = GC.getImprovementInfo(eUniqueImprovement);
			if (pUniqueImprovement && pUniqueImprovement->IsRequiresFeature() && pUniqueImprovement->GetFeatureMakesValid(eFeature))
			{
				eBuild = m_pPlayer->GetUniqueImprovementBuild(iI);
#ifdef AUI_WORKER_ADD_IMPROVING_PLOTS_DIRECTIVE_DEFENSIVES
				iLoopScore = 0;
				if (pCity)
				{
					UpdateProjectedPlotYields(pPlot, eBuild);
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
					iLoopScore = ScorePlot(eBuild);
#else
					iLoopScore = ScorePlot();
#endif
				}
				if (!pCity || iLoopScore > 0)
				{
					int iBaseDefenseBonus = (pPlot->isHills() || pPlot->isMountain() ? GC.getHILLS_EXTRA_DEFENSE() : GC.getFeatureInfo(eFeature)->getDefenseModifier());
					iLoopScore += (pUniqueImprovement->GetDefenseModifier() + iBaseDefenseBonus) * pPlot->getStrategicValue() / (100 * GC.getCHOKEPOINT_STRATEGIC_VALUE());
				}
#else
				UpdateProjectedPlotYields(pPlot, eBuild);
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
				iLoopScore = ScorePlot(eBuild);
#else
				iLoopScore = ScorePlot();
#endif
#endif
				int iWeight = GC.getBUILDER_TASKING_BASELINE_BUILD_IMPROVEMENTS();
				iWeight = GetBuildCostWeight(iWeight, pPlot, eBuild);
				iWeight += GetBuildTimeWeight(pUnit, pPlot, eBuild, DoesBuildHelpRush(pUnit, pPlot, eBuild), iMoveTurnsAway);
				iWeight *= iLoopScore;
				iWeight = CorrectWeight(iWeight);

				if (iWeight > iBestUniqueImprovementWeight)
					iBestUniqueImprovementWeight = iWeight;
			}
		}
	}
#endif
	for(iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
	{
		eBuild = (BuildTypes)iBuildIndex;
		eOriginalBuildType = eBuild;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(pkBuild == NULL)
		{
			continue;
		}

		ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
		if(eImprovement == NO_IMPROVEMENT)
		{
			continue;
		}

		CvImprovementEntry* pImprovement = GC.getImprovementInfo(eImprovement);

#ifdef AUI_WORKER_ADD_IMPROVING_PLOTS_DIRECTIVE_DEFENSIVES
		// If the improvement has no defensive effect, skip it
		if (pImprovement->GetDefenseModifier() == 0 && !pCity)
		{
			continue;
		}
#else
		// if this improvement has a defense modifier, ignore it for now
		//if(pImprovement->GetDefenseModifier() > 0)
		//{
		//	continue;
		//}
#endif

#ifndef AUI_WORKER_FIX_IMPROVING_PLOTS_DIRECTIVE_DONT_REQUIRE_BONUS_RESOURCE_UNLOCKER
		// for bonus resources, check to see if this is the improvement that connects it
		if(eResource != NO_RESOURCE)
		{
			if(!pImprovement->IsImprovementResourceTrade(eResource))
			{
				if(m_bLogging){
					CvString strTemp;
					strTemp.Format("Weight,!pImprovement->IsImprovementResourceTrade(eResource),%s,%i,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), eResource, pPlot->getX(), pPlot->getY());
					LogInfo(strTemp, m_pPlayer);
				}
				continue;
			}
		}
#endif

		if(eImprovement == pPlot->getImprovementType())
		{
			if(pPlot->IsImprovementPillaged())
			{
				eBuild = m_eRepairBuild;
			}
			else
			{
				if(m_bLogging){
					CvString strTemp;
					strTemp.Format("Weight,eImprovement == pPlot->getImprovementType(),%s,%i,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), eImprovement, pPlot->getX(), pPlot->getY());
					LogInfo(strTemp, m_pPlayer);
				}
				continue;
			}
		}
		else
		{
			// Do we have a special improvement here? (great person improvement, gifted improvement from major civ)
			if (eExistingImprovement != NO_IMPROVEMENT)
			{
				if (pPlot->HasSpecialImprovement() || GET_PLAYER(pUnit->getOwner()).isOption(PLAYEROPTION_SAFE_AUTOMATION))
				{
					if(m_bLogging){
						CvString strTemp;
						strTemp.Format("Weight,Improvement Type Blocked by Special Improvement,%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
						LogInfo(strTemp, m_pPlayer);
					}
					continue;
				}
			}
		}

		// Only check to make sure our unit can build this after possibly switching this to a repair build in the block of code above
		if(!pUnit->canBuild(pPlot, eBuild))
		{
			if(m_bLogging){
				CvString strTemp;
				strTemp.Format("Weight,!pUnit->canBuild(),%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
				LogInfo(strTemp, m_pPlayer);
			}
			continue;
		}

		bool bWillRemoveForestOrJungle = false;
#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
		FeatureTypes eFeature = pPlot->getFeatureType();
#endif
		if(eFeature == FEATURE_FOREST || eFeature == FEATURE_JUNGLE)
		{
			if(pkBuild->isFeatureRemove(eFeature))
			{
				bWillRemoveForestOrJungle = true;
			}
		}

#ifndef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
		// special case for Dutch
		if (m_bKeepMarshes && eFeature == FEATURE_MARSH)
		{
			if (pkBuild->isFeatureRemove(FEATURE_MARSH))
			{
				if(m_bLogging){
					CvString strTemp;
					strTemp.Format("Weight,Marsh Remove,%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
					LogInfo(strTemp, m_pPlayer);
				}
				continue;
			}
		}

		// special case for Brazil
		if (m_bKeepJungle && eFeature == FEATURE_JUNGLE)
		{
			if (pkBuild->isFeatureRemove(FEATURE_JUNGLE))
			{
				if(m_bLogging){
					CvString strTemp;
					strTemp.Format("Weight,Jungle Remove,%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
					LogInfo(strTemp, m_pPlayer);
				}
				if (pPlot->getResourceType(m_pPlayer->getTeam()) == NO_RESOURCE)
				{
					continue;
				}
			}
		}
#endif

		if(GET_PLAYER(pUnit->getOwner()).isOption(PLAYEROPTION_LEAVE_FORESTS))
		{
			if(eFeature != NO_FEATURE)
			{
				if(pkBuild->isFeatureRemove(eFeature))
				{
					if(m_bLogging){
						CvString strTemp;
						strTemp.Format("Weight,Keep Forests,%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
						LogInfo(strTemp, m_pPlayer);
					}
					continue;
				}
			}
		}

#ifdef AUI_WORKER_ADD_IMPROVING_PLOTS_DIRECTIVE_DEFENSIVES
		int iScore = 0;
		if (pCity)
		{
			UpdateProjectedPlotYields(pPlot, eBuild);
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
			iScore = ScorePlot(eBuild);
#else
			iScore = ScorePlot();
#endif
		}
		if (!pCity || iScore > 0)
		{
			int iBaseDefenseBonus = (pPlot->isHills() || pPlot->isMountain() ? GC.getHILLS_EXTRA_DEFENSE() :
				(eFeature != NO_FEATURE && !pkBuild->isFeatureRemove(eFeature) ? GC.getFeatureInfo(eFeature)->getDefenseModifier() :
					GC.getTerrainInfo(pPlot->getTerrainType())->getDefenseModifier() + GC.getFLAT_LAND_EXTRA_DEFENSE()));
			iScore += (pImprovement->GetDefenseModifier() + iBaseDefenseBonus) * pPlot->getStrategicValue() / (100 * GC.getCHOKEPOINT_STRATEGIC_VALUE());
		}
#else
		UpdateProjectedPlotYields(pPlot, eBuild);
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
		int iScore = ScorePlot(eBuild);
#else
		int iScore = ScorePlot();
#endif
#endif

		// if we're going backward, bail out!
		if(iScore <= 0)
		{
			if(m_bLogging){
				CvString strTemp;
				strTemp.Format("Weight,Negative Score,%s,%i,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), iScore, pPlot->getX(), pPlot->getY());
				LogInfo(strTemp, m_pPlayer);
			}
			continue;
		}

		BuilderDirective::BuilderDirectiveType eDirectiveType = BuilderDirective::BUILD_IMPROVEMENT;
		int iWeight = GC.getBUILDER_TASKING_BASELINE_BUILD_IMPROVEMENTS();
		if(eBuild == m_eRepairBuild)
		{
			eDirectiveType = BuilderDirective::REPAIR;
			iWeight = GC.getBUILDER_TASKING_BASELINE_REPAIR();
		}
		else if(pImprovement->GetYieldChange(YIELD_CULTURE) > 0)
		{
			iWeight = GC.getBUILDER_TASKING_BASELINE_ADDS_CULTURE() * GC.getImprovementInfo(eImprovement)->GetYieldChange(YIELD_CULTURE);
			int iAdjacentCulture = pImprovement->GetCultureAdjacentSameType();

			if(iAdjacentCulture > 0)
			{
				iScore *= (1 + pPlot->ComputeCultureFromAdjacentImprovement(*pImprovement, eImprovement));
			}
		}

		iWeight = GetBuildCostWeight(iWeight, pPlot, eBuild);
		int iBuildTimeWeight = GetBuildTimeWeight(pUnit, pPlot, eBuild, DoesBuildHelpRush(pUnit, pPlot, eBuild), iMoveTurnsAway);
		iWeight += iBuildTimeWeight;
		iWeight *= iScore;

		if(m_pPlayer->GetPlayerTraits()->IsMoveFriendlyWoodsAsRoad() && bWillRemoveForestOrJungle)
		{
			iWeight = iWeight / 4;
		}

		iWeight = CorrectWeight(iWeight);

#ifdef AUI_WORKER_UNHARDCODE_NO_REMOVE_FEATURE_THAT_IS_REQUIRED_FOR_UNIQUE_IMPROVEMENT
		if (iWeight < iBestUniqueImprovementWeight && eFeature != NO_FEATURE && pkBuild->isFeatureRemove(eFeature))
			continue;
#endif

		BuilderDirective directive;

		directive.m_eDirective = eDirectiveType;
		directive.m_eBuild = eBuild;
		directive.m_eResource = NO_RESOURCE;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		directive.m_sMoveTurnsAway = iMoveTurnsAway;

		if(m_bLogging)
		{
			CvString strTemp;
			strTemp.Format("Weight,Directive Score Added,%s,,,,%i, %i, %i, %d", GC.getBuildInfo(eBuild)->GetType(), directive.m_sX, directive.m_sY, directive.m_sMoveTurnsAway, iWeight);
			LogInfo(strTemp, m_pPlayer);
		}

		m_aDirectives.push_back(directive, iWeight);
	}
}

#ifdef AUI_WORKER_ADD_IMPROVING_MINOR_PLOTS_DIRECTIVES
void CvBuilderTaskingAI::AddImprovingMinorPlotsDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
	ImprovementTypes eExistingImprovement = pPlot->getImprovementType();

	// Do we have a special improvement here? (great person improvement, gifted improvement from major civ)
	if (eExistingImprovement != NO_IMPROVEMENT && pPlot->HasSpecialImprovement() && !pPlot->IsImprovementPillaged())
	{
		return;
	}

	// check to see if a non-bonus resource is here. if so, bail out!
	ResourceTypes eResource = pPlot->getResourceType(m_pPlayer->getTeam());
	if (eResource != NO_RESOURCE)
	{
		if (GC.getResourceInfo(eResource)->getResourceUsage() != RESOURCEUSAGE_BONUS)
		{
			return;
		}
	}

#ifdef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a conflicting mission here
	if (pUnit->GetMissionAIPlot() != pPlot)
	{
		int iLoop;
		for (CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pUnit == pLoopUnit)
			{
				continue;
			}

			CvPlot* pMissionPlot = pLoopUnit->GetMissionAIPlot();
			if (!pMissionPlot)
			{
				continue;
			}

			MissionAITypes eGroupMissionAI = pLoopUnit->GetMissionAIType();
			if (eGroupMissionAI != MISSIONAI_BUILD)
			{
				continue;
			}

			if (pPlot->getX() == pMissionPlot->getX() && pPlot->getY() == pMissionPlot->getY())
			{
				BuildTypes eOtherBuild = pLoopUnit->getBuildType();
				CvBuildInfo* pkBuild = GC.getBuildInfo(eOtherBuild);
				if (pkBuild != NULL)
				{
					ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
					if (eImprovement != NO_IMPROVEMENT || (eOtherBuild == m_eRepairBuild && eExistingImprovement != NO_IMPROVEMENT))
					{
						if (m_bLogging)
						{
							CvString strLog;
							strLog.Format("x: %d y: %d, Somebody has a conflicting mission here.", pPlot->getX(), pPlot->getY());
							LogInfo(strLog, m_pPlayer, true);
						}
						return;
					}
				}
			}
		}
	}
#endif

	// loop through the build types to find one that we can use
	BuildTypes eBuild;
	BuildTypes eOriginalBuildType;
#ifdef AUI_WARNING_FIXES
	uint iBuildIndex;
#else
	int iBuildIndex;
#endif
	for (iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
	{
		eBuild = (BuildTypes)iBuildIndex;
		eOriginalBuildType = eBuild;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if (pkBuild == NULL)
		{
			continue;
		}

		ImprovementTypes eImprovement = (ImprovementTypes)pkBuild->getImprovement();
		if (eImprovement == NO_IMPROVEMENT)
		{
			continue;
		}

		CvImprovementEntry* pImprovement = GC.getImprovementInfo(eImprovement);

		if (eImprovement == pPlot->getImprovementType())
		{
			if (pPlot->IsImprovementPillaged())
			{
				eBuild = m_eRepairBuild;
			}
			else
			{
				if (m_bLogging) {
					CvString strTemp;
					strTemp.Format("Weight,eImprovement == pPlot->getImprovementType(),%s,%i,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), eImprovement, pPlot->getX(), pPlot->getY());
					LogInfo(strTemp, m_pPlayer);
				}
				continue;
			}
		}

		// Only check to make sure our unit can build this after possibly switching this to a repair build in the block of code above
		if (!pUnit->canBuild(pPlot, eBuild))
		{
			if (m_bLogging) {
				CvString strTemp;
				strTemp.Format("Weight,!pUnit->canBuild(),%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
				LogInfo(strTemp, m_pPlayer);
			}
			continue;
		}

		FeatureTypes eFeature = pPlot->getFeatureType();
		if (GET_PLAYER(pUnit->getOwner()).isOption(PLAYEROPTION_LEAVE_FORESTS))
		{
			if (eFeature != NO_FEATURE)
			{
				if (pkBuild->isFeatureRemove(eFeature))
				{
					if (m_bLogging) {
						CvString strTemp;
						strTemp.Format("Weight,Keep Forests,%s,,,,%i, %i", GC.getBuildInfo(eBuild)->GetType(), pPlot->getX(), pPlot->getY());
						LogInfo(strTemp, m_pPlayer);
					}
					continue;
				}
			}
		}

		int iExtraWeight = 0;
		int iScore = 0;
		if (pImprovement->GetLuxuryCopiesSiphonedFromMinor() > 0)
		{
			if (m_pPlayer->getSiphonLuxuryCount(pPlot->getOwner()) <= 0)
			{
#ifdef AUI_WARNING_FIXES
				for (uint iI = 0; iI < GC.getNumResourceInfos(); iI++)
#else
				for (int iI = 0; iI < GC.getNumResourceInfos(); iI++)
#endif
				{
					int iResouceCount = GET_PLAYER(pPlot->getOwner()).getResourceInOwnedPlots((ResourceTypes)iI);
					if (iResouceCount > 0)
					{
						CvResourceInfo* pkResource = GC.getResourceInfo(eResource);
						if (pkResource && pkResource->getResourceUsage() == RESOURCEUSAGE_LUXURY)
						{
							iExtraWeight += GetResourceWeight((ResourceTypes)iI, eImprovement, iResouceCount);
							iScore += iMoveTurnsAway;
						}
					}
				}
#ifdef AUI_WORKER_ADD_IMPROVING_PLOTS_DIRECTIVE_DEFENSIVES
				int iBaseDefenseBonus = (pPlot->isHills() || pPlot->isMountain() ? GC.getHILLS_EXTRA_DEFENSE() :
					(eFeature != NO_FEATURE && !pkBuild->isFeatureRemove(eFeature) ? GC.getFeatureInfo(eFeature)->getDefenseModifier() :
						GC.getTerrainInfo(pPlot->getTerrainType())->getDefenseModifier() + GC.getFLAT_LAND_EXTRA_DEFENSE()));
				iScore += (pImprovement->GetDefenseModifier() + iBaseDefenseBonus) * pPlot->getStrategicValue() / (100 * GC.getCHOKEPOINT_STRATEGIC_VALUE());
#endif
			}
		}

		// Add other improvements that are advantageous when built on other players' territory here

		// if we're going backward, bail out!
		if (iScore <= 0)
		{
			continue;
		}

		BuilderDirective::BuilderDirectiveType eDirectiveType = BuilderDirective::BUILD_IMPROVEMENT;
		int iWeight = GC.getBUILDER_TASKING_BASELINE_BUILD_IMPROVEMENTS();
		if (eBuild == m_eRepairBuild)
		{
			eDirectiveType = BuilderDirective::REPAIR;
			iWeight = GC.getBUILDER_TASKING_BASELINE_REPAIR();
		}

		iWeight = GetBuildCostWeight(iWeight, pPlot, eBuild);
		int iBuildTimeWeight = GetBuildTimeWeight(pUnit, pPlot, eBuild, DoesBuildHelpRush(pUnit, pPlot, eBuild), iMoveTurnsAway);
		iWeight += iBuildTimeWeight;
		iWeight = CorrectWeight(iWeight);

		if (iExtraWeight > 0)
		{
			iWeight += iExtraWeight;
			iWeight = CorrectWeight(iWeight);
		}

		if (iScore > 0)
		{
			iWeight *= iScore;
			iWeight = CorrectWeight(iWeight);
		}

		BuilderDirective directive;

		directive.m_eDirective = eDirectiveType;
		directive.m_eBuild = eBuild;
		directive.m_eResource = NO_RESOURCE;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		directive.m_sMoveTurnsAway = iMoveTurnsAway;

		if (m_bLogging)
		{
			CvString strTemp;
			strTemp.Format("Weight,Directive Score Added,%s,,,,%i, %i, %i, %d", GC.getBuildInfo(eBuild)->GetType(), directive.m_sX, directive.m_sY, directive.m_sMoveTurnsAway, iWeight);
			LogInfo(strTemp, m_pPlayer);
		}

		m_aDirectives.push_back(directive, iWeight);
	}
}
#endif

/// Adds a directive if the unit can construct a road in the plot
void CvBuilderTaskingAI::AddRouteDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
	RouteTypes eBestRouteType = m_pPlayer->getBestRoute();

	// if the player can't build a route, bail out!
	if(eBestRouteType == NO_ROUTE)
	{
		return;
	}

	if(pPlot->getRouteType() == eBestRouteType && !pPlot->IsRoutePillaged())
	{
		return;
	}

#ifdef AUI_WARNING_FIXES
	if (!pUnit)
		return;
#endif

	// the plot was not flagged this turn, so ignore
	bool bShouldRoadThisTile = (pPlot->GetBuilderAIScratchPadTurn() == GC.getGame().getGameTurn()) && (pPlot->GetBuilderAIScratchPadPlayer() == pUnit->getOwner());
#ifdef AUI_WORKER_INCA_HILLS
	bool bIncaBonusActive = (m_pPlayer->GetPlayerTraits()->IsNoHillsImprovementMaintenance() && !m_pPlayer->isHuman() && pPlot->getTerrainType() == TERRAIN_HILL);
	if (!bShouldRoadThisTile && !bIncaBonusActive)
	{
		return;
	}
#else
	if(!bShouldRoadThisTile)
	{
		return;
	}
#endif

#ifdef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a conflicting mission here
	if (pUnit->GetMissionAIPlot() != pPlot)
	{
		int iLoop;
		for (CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pUnit == pLoopUnit)
			{
				continue;
			}

			CvPlot* pMissionPlot = pLoopUnit->GetMissionAIPlot();
			if (!pMissionPlot)
			{
				continue;
			}

			MissionAITypes eGroupMissionAI = pLoopUnit->GetMissionAIType();
			if (eGroupMissionAI != MISSIONAI_BUILD)
			{
				continue;
			}

			if (pPlot->getX() == pMissionPlot->getX() && pPlot->getY() == pMissionPlot->getY())
			{
				BuildTypes eOtherBuild = pLoopUnit->getBuildType();
				CvBuildInfo* pkBuild = GC.getBuildInfo(eOtherBuild);
				if (pkBuild != NULL)
				{
					RouteTypes eOtherRoute = (RouteTypes)pkBuild->getRoute();
					if (eOtherRoute != NO_ROUTE || (eOtherBuild == m_eRepairBuild && pPlot->IsRoutePillaged()))
					{
						if (m_bLogging)
						{
							CvString strLog;
							strLog.Format("x: %d y: %d, Somebody has a conflicting mission here.", pPlot->getX(), pPlot->getY());
							LogInfo(strLog, m_pPlayer, true);
						}
						return;
					}
				}
			}
		}
	}
#endif

	// find the route build
	BuildTypes eRouteBuild = NO_BUILD;
	if(pPlot->IsRoutePillaged())
	{
		eRouteBuild = m_eRepairBuild;
	}
	else
	{
		RouteTypes eRoute = pPlot->GetBuilderAIScratchPadRoute();
#ifdef AUI_WARNING_FIXES
		for (uint i = 0; i < GC.getNumBuildInfos(); i++)
#else
		for(int i = 0; i < GC.getNumBuildInfos(); i++)
#endif
		{
			BuildTypes eBuild = (BuildTypes)i;
			CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
			if(pkBuild && pkBuild->getRoute() == eRoute)
			{
				eRouteBuild = eBuild;
				break;
			}
		}
	}

	if(eRouteBuild == NO_BUILD)
	{
		return;
	}

	CvUnitEntry& kUnitInfo = pUnit->getUnitInfo();
	if(!kUnitInfo.GetBuilds(eRouteBuild))
	{
		return;
	}

	if(GET_PLAYER(pUnit->getOwner()).isOption(PLAYEROPTION_LEAVE_FORESTS))
	{
		FeatureTypes eFeature = pPlot->getFeatureType();
		if(eFeature != NO_FEATURE)
		{
			CvBuildInfo* pkBuild = GC.getBuildInfo(eRouteBuild);
			if(pkBuild && pkBuild->isFeatureRemove(eFeature))
			{
				return;
			}
		}
	}

	int iWeight = GC.getBUILDER_TASKING_BASELINE_BUILD_ROUTES();
	BuilderDirective::BuilderDirectiveType eDirectiveType = BuilderDirective::BUILD_ROUTE;
	if(eRouteBuild == m_eRepairBuild)
	{
		iWeight = GC.getBUILDER_TASKING_BASELINE_REPAIR();
		eDirectiveType = BuilderDirective::REPAIR;
	}

	// int iTurnsAway = FindTurnsAway(pUnit, pPlot);
	iWeight = iWeight / (iMoveTurnsAway/*iTurnsAway*/ + 1);
	iWeight = GetBuildCostWeight(iWeight, pPlot, eRouteBuild);
	iWeight += GetBuildTimeWeight(pUnit, pPlot, eRouteBuild, false, iMoveTurnsAway);
#ifdef AUI_WORKER_INCA_HILLS
	if (!(bIncaBonusActive && pPlot->GetBuilderAIScratchPadValue() <= 0))
	{
		iWeight *= pPlot->GetBuilderAIScratchPadValue();
	}
#else
	iWeight *= pPlot->GetBuilderAIScratchPadValue();
#endif
	iWeight = CorrectWeight(iWeight);

	BuilderDirective directive;
	directive.m_eDirective = eDirectiveType;
	directive.m_eBuild = eRouteBuild;
	directive.m_eResource = NO_RESOURCE;
	directive.m_sX = pPlot->getX();
	directive.m_sY = pPlot->getY();
	directive.m_sMoveTurnsAway = iMoveTurnsAway;

	if(m_bLogging)
	{
		CvString strTemp;
		strTemp.Format("AddRouteDirectives, adding, x: %d y: %d, Weight, %d", pPlot->getX(), pPlot->getY(), iWeight);
		LogInfo(strTemp, m_pPlayer);
	}

	m_aDirectives.push_back(directive, iWeight);
}

/// Determines if the builder should "chop" the feature in the tile
void CvBuilderTaskingAI::AddChopDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
#ifdef AUI_WARNING_FIXES
	if (!pUnit || !pPlot)
		return;
#endif
	// if it's not within a city radius
	if(!pPlot->isWithinTeamCityRadius(pUnit->getTeam()))
	{
		return;
	}

	if(pPlot->getImprovementType() != NO_IMPROVEMENT)
	{
		return;
	}

	if(GET_PLAYER(pUnit->getOwner()).isOption(PLAYEROPTION_LEAVE_FORESTS))
	{
		return;
	}

	// check to see if a resource is here. If so, bail out!
	ResourceTypes eResource = pPlot->getResourceType(m_pPlayer->getTeam());
	if(eResource != NO_RESOURCE)
	{
		return;
	}

	CvCity* pCity = GetWorkingCity(pPlot);
	if(!pCity)
	{
		return;
	}

	if(pCity->GetCityCitizens()->IsWorkingPlot(pPlot))
	{
		return;
	}

	FeatureTypes eFeature = pPlot->getFeatureType();
	if(eFeature == NO_FEATURE)
	{
		// no feature in this tile, so bail
		return;
	}

	// celtic rule: if this is a forest tile next to a city, do not chop the trees
	if (m_pPlayer->GetPlayerTraits()->IsFaithFromUnimprovedForest())
	{
		CvCity* pAdjacentCity = pPlot->GetAdjacentCity();
		if (pAdjacentCity && pAdjacentCity->getOwner() == m_pPlayer->GetID())
		{
			if (eFeature == FEATURE_FOREST)
			{
				return;
			}
		}
	}

#ifdef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a conflicting mission here
	if (pUnit->GetMissionAIPlot() != pPlot)
	{
		int iLoop;
		for (CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pUnit == pLoopUnit)
			{
				continue;
			}

			CvPlot* pMissionPlot = pLoopUnit->GetMissionAIPlot();
			if (!pMissionPlot)
			{
				continue;
			}

			MissionAITypes eGroupMissionAI = pLoopUnit->GetMissionAIType();
			if (eGroupMissionAI != MISSIONAI_BUILD)
			{
				continue;
			}

			if (pPlot->getX() == pMissionPlot->getX() && pPlot->getY() == pMissionPlot->getY())
			{
				BuildTypes eOtherBuild = pLoopUnit->getBuildType();
				CvBuildInfo* pkBuild = GC.getBuildInfo(eOtherBuild);
				if (pkBuild != NULL && pkBuild->getImprovement() == NO_IMPROVEMENT && pkBuild->isFeatureRemove(eFeature))
				{
					if (m_bLogging)
					{
						CvString strLog;
						strLog.Format("x: %d y: %d, Somebody has a conflicting mission here.", pPlot->getX(), pPlot->getY());
						LogInfo(strLog, m_pPlayer, true);
					}
					return;
				}
			}
		}
	}
#endif

	BuildTypes eChopBuild = NO_BUILD;
#ifdef AUI_WARNING_FIXES
	for (uint iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#else
	for(int iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)iBuildIndex;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(NULL != pkBuild && pkBuild->getImprovement() == NO_IMPROVEMENT && pkBuild->isFeatureRemove(eFeature) && pkBuild->getFeatureProduction(eFeature) > 0 && pUnit->canBuild(pPlot, eBuild))
		{
			eChopBuild = eBuild;
			break;
		}
	}

	if(eChopBuild == NO_BUILD)
	{
		// we couldn't find a build that removed the feature without a production benefit
		return;
	}

	pCity = NULL;
	int iProduction = pPlot->getFeatureProduction(eChopBuild, pUnit->getOwner(), &pCity);

	if(!DoesBuildHelpRush(pUnit, pPlot, eChopBuild))
	{
		return;
	}

	int iWeight = GC.getBUILDER_TASKING_BASELINE_REPAIR();
	//int iTurnsAway = FindTurnsAway(pUnit, pPlot);
	iWeight = iWeight / (iMoveTurnsAway/*iTurnsAway*/ + 1);
	iWeight = GetBuildCostWeight(iWeight, pPlot, eChopBuild);
	int iBuildTimeWeight = GetBuildTimeWeight(pUnit, pPlot, eChopBuild, false, iMoveTurnsAway);
	iWeight += iBuildTimeWeight;
	iWeight *= iProduction; // times the amount that the plot produces from the chopping

	int iYieldDifferenceWeight = 0;
	CvFlavorManager* pFlavorManager = m_pPlayer->GetFlavorManager();
	for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		// calculate natural yields
		int iPreviousYield = pPlot->calculateNatureYield((YieldTypes)ui, m_pPlayer->getTeam());
		int iNewYield = pPlot->calculateNatureYield((YieldTypes)ui, m_pPlayer->getTeam(), true /*bIgnoreFeature*/);
		int iDeltaYield = iNewYield - iPreviousYield;

		if(iDeltaYield == 0)
		{
			continue;
		}

		for(int iFlavorLoop = 0; iFlavorLoop < GC.getNumFlavorTypes(); iFlavorLoop++)
		{
			switch(ui)
			{
			case YIELD_FOOD:
				if(GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_GROWTH")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_FOOD();
				}
				break;
			case YIELD_PRODUCTION:
				if(GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_PRODUCTION")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_PRODUCTION();
				}
				break;
			case YIELD_GOLD:
				if(GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_GOLD")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_GOLD();
				}
				break;
			case YIELD_SCIENCE:
				if(GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_SCIENCE")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_SCIENCE();
				}
				break;
			case YIELD_CULTURE:
				if(GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_CULTURE")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_CULTURE();
				}
				break;
			case YIELD_FAITH:
#ifdef AUI_WORKER_EVALUATE_FAITH
				if (GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_RELIGION")
				{
					iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_CULTURE();
				}
#else
				//if (GC.getFlavorTypes((FlavorTypes)iFlavorLoop) == "FLAVOR_SCIENCE")
				//{
				//	iYieldDifferenceWeight += iDeltaYield * pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_SCIENCE();
				//}
#endif
				break;
			}
		}
	}

	// if we are going backwards, bail
	if(iYieldDifferenceWeight < 0)
	{
		return;
	}

	iWeight += iYieldDifferenceWeight;

	if (m_pPlayer->GetPlayerTraits()->IsMoveFriendlyWoodsAsRoad() && (eFeature == FEATURE_FOREST || eFeature == FEATURE_JUNGLE))
	{
		iWeight = iWeight / 4;
	}
	if (m_pPlayer->GetPlayerTraits()->IsFaithFromUnimprovedForest() && eFeature == FEATURE_FOREST)
	{
		iWeight = iWeight / 4;
	}
#ifndef AUI_WORKER_NO_CHOP_BIAS
	// this doesn't actually help them, but adds some historical flavor
	if (m_pPlayer->GetPlayerTraits()->IsEmbarkedAllWater() && (eFeature == FEATURE_FOREST || eFeature == FEATURE_JUNGLE))
	{
		iWeight = iWeight * 2;
	}
#endif

	iWeight = CorrectWeight(iWeight);

	if(iWeight > 0)
	{
		BuilderDirective directive;
		directive.m_eDirective = BuilderDirective::CHOP;
		directive.m_eBuild = eChopBuild;
		directive.m_eResource = NO_RESOURCE;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		//directive.m_iGoldCost = m_pPlayer->getBuildCost(pPlot, eChopBuild);
		directive.m_sMoveTurnsAway = iMoveTurnsAway;

		if(m_bLogging)
		{
			CvString strTemp;
			strTemp.Format("BuildTimeWeight, %d, Weight, %d", iBuildTimeWeight, iWeight);
			LogInfo(strTemp, m_pPlayer);
		}

		m_aDirectives.push_back(directive, iWeight);
	}
	else
	{
		if(m_bLogging)
		{
			LogInfo("Add chop directives, Weight is zero!", m_pPlayer);
		}
	}
}

void CvBuilderTaskingAI::AddRemoveUselessRoadDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
	// if it's not within a city radius
	if(!pPlot->isWithinTeamCityRadius(pUnit->getTeam()))
	{
		return;
	}

	// don't try to remove the route under the city
	if(pPlot->isCity())
	{
		return;
	}

	// nothing here to remove
	if(pPlot->getRouteType() == NO_ROUTE)
	{
		return;
	}

	// flagged this turn means this is a valid route plot
	if(pPlot->GetBuilderAIScratchPadTurn() == GC.getGame().getGameTurn() && pPlot->GetBuilderAIScratchPadPlayer() == pUnit->getOwner())
	{
		return;
	}

	BuildTypes eRemoveRouteBuild = NO_BUILD;
#ifdef AUI_WARNING_FIXES
	for (uint iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#else
	for(int iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)iBuildIndex;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
		if(NULL != pkBuild && pkBuild->IsRemoveRoute() && pUnit->canBuild(pPlot, eBuild))
		{
			eRemoveRouteBuild = eBuild;
			break;
		}
	}


	if(eRemoveRouteBuild == NO_BUILD)
	{
		return;
	}

	// evaluate if removing the road helps anything
	ImprovementTypes eImprovement = pPlot->getImprovementType();
	BuildTypes eBuild = NO_BUILD;
	if(eImprovement != NO_IMPROVEMENT)
	{
		eBuild = GetBuildTypeFromImprovement(eImprovement);
	}

	//int iWeight = GetWeightFromPlotYields(m_pPlayer, pPlot, eBuild, NULL, NO_YIELD, 100, NO_ROUTE) - GetWeightFromPlotYields(m_pPlayer, pPlot, eBuild, NULL, NO_YIELD, 10);
	int iWeight = 0;
	iWeight = CorrectWeight(iWeight);

	if(iWeight > 0)
	{
		BuilderDirective directive;
		directive.m_eDirective = BuilderDirective::REMOVE_ROAD;
		directive.m_eBuild = eRemoveRouteBuild;
		directive.m_eResource = NO_RESOURCE;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		directive.m_sMoveTurnsAway = iMoveTurnsAway;

		m_aDirectives.push_back(directive, iWeight);
	}

}

// Everything means less than zero, hey
void CvBuilderTaskingAI::AddScrubFalloutDirectives(CvUnit* pUnit, CvPlot* pPlot, int iMoveTurnsAway)
{
	if(m_eFalloutFeature == NO_FEATURE || m_eFalloutRemove == NO_BUILD)
	{
		return;
	}

	CvCity* pCity = GetWorkingCity(pPlot);
	if(!pCity)
	{
		return;
	}

	if(pPlot->getFeatureType() == m_eFalloutFeature && pUnit->canBuild(pPlot, m_eFalloutRemove))
	{
		int iWeight = GC.getBUILDER_TASKING_BASELINE_SCRUB_FALLOUT();
		//int iTurnsAway = FindTurnsAway(pUnit, pPlot);
		iWeight = iWeight / (iMoveTurnsAway/*iTurnsAway*/ + 1);
#ifdef AUI_WORKER_FIX_FALLOUT
		// For scrubbing fallout, build times and build costs should be ignored because... well, it's fallout
		// Max values returned from BuildCostWeight and BuildTimeWeight
		iWeight *= 100;
		iWeight += 10000 / (iMoveTurnsAway/*iTurnsAway*/ + 1);
#else
		iWeight = GetBuildCostWeight(iWeight, pPlot, m_eFalloutRemove);
		int iBuildTimeWeight = GetBuildTimeWeight(pUnit, pPlot, m_eFalloutRemove, false, iMoveTurnsAway);
		iWeight += iBuildTimeWeight;
#endif

		BuilderDirective directive;
		directive.m_eDirective = BuilderDirective::CHOP;
		directive.m_eBuild = m_eFalloutRemove;
		directive.m_eResource = NO_RESOURCE;
		directive.m_sX = pPlot->getX();
		directive.m_sY = pPlot->getY();
		directive.m_sMoveTurnsAway = iMoveTurnsAway;
		m_aDirectives.push_back(directive, iWeight);
	}
}


/// Evaluates all the circumstances to determine if the builder can and should evaluate the given plot
bool CvBuilderTaskingAI::ShouldBuilderConsiderPlot(CvUnit* pUnit, CvPlot* pPlot)
{
	// if plot is impassable, bail!
#ifdef AUI_WORKER_FIX_SHOULD_CONSIDER_PLOT_FLYING_WORKER_DISREGARDS_PEAKS
	if (!pUnit->canMoveAllTerrain() && ((pPlot->isImpassable() && !pUnit->canMoveImpassable()) ||
		(pPlot->isMountain() && !pUnit->IsHoveringUnit() && !m_pPlayer->GetPlayerTraits()->IsAbleToCrossMountains())))
#else
	if(pPlot->isImpassable() || pPlot->isMountain())
#endif
	{
		if(m_bLogging)
		{
			CvString strLog;
			strLog.Format("x: %d y: %d,,Impassable tile. Toss out", pPlot->getX(), pPlot->getY());
			LogInfo(strLog, m_pPlayer);
		}
		return false;
	}

	// can't build on plots others own (unless inside a minor)
	PlayerTypes eOwner = pPlot->getOwner();
	if(eOwner != NO_PLAYER && eOwner != m_pPlayer->GetID() && !GET_PLAYER(eOwner).isMinorCiv())
	{
		return false;
	}

	// workers should not be able to work in plots that do not match their default domain
	switch(pUnit->getDomainType())
	{
	case DOMAIN_LAND:
		if(pPlot->isWater())
		{
			return false;
		}
		break;
	case DOMAIN_SEA:
		if(!pPlot->isWater())
		{
			return false;
		}
		break;
	default:
		break;
	}

	// need more planning for amphibious units
	// we should include here the ability for work boats to cross to other areas with cities
	if(pPlot->area() != pUnit->area())
	{
#ifdef AUI_WORKER_FIX_SHOULD_CONSIDER_PLOT_WORK_BOATS_CONSIDER_ALL_SEA_PLOTS
		if (pUnit->getDomainType() != DOMAIN_SEA && !pUnit->CanEverEmbark())
		{
			if (m_bLogging)
			{
				CvString strLog;
				strLog.Format("unitx: %d unity: %d, plotx: %d ploty: %d, plot area: %d, unit area: %d,,Plot areas don't match and can't embark", pUnit->getX(), pUnit->getY(), pPlot->getX(), pPlot->getY(), pPlot->area(), pUnit->area());
				if (pPlot->isWater() == pUnit->plot()->isWater())
				{
					strLog += ",This is weird";
				}
				else
				{
					strLog += ",This is normal";
				}
				LogInfo(strLog, m_pPlayer);
			}
			return false;
		}
#else
		bool bCanCrossToNewArea = false;

		if(pUnit->getDomainType() == DOMAIN_SEA)
		{
			if(pPlot->isAdjacentToArea(pUnit->area()))
			{
				bCanCrossToNewArea = true;
			}
		}
		else
		{
			if(pUnit->CanEverEmbark())
			{
				bCanCrossToNewArea = true;
			}
		}

		if(!bCanCrossToNewArea)
		{
			if(m_bLogging)
			{
				CvString strLog;
				strLog.Format("unitx: %d unity: %d, plotx: %d ploty: %d, plot area: %d, unit area: %d,,Plot areas don't match and can't embark", pUnit->getX(), pUnit->getY(), pPlot->getX(), pPlot->getY(), pPlot->area(), pUnit->area());
				if(pPlot->isWater() == pUnit->plot()->isWater())
				{
					strLog += ",This is weird";
				}
				else
				{
					strLog += ",This is normal";
				}
				LogInfo(strLog, m_pPlayer);
			}
			return false;
		}
#endif
	}

#ifndef AUI_WORKER_FIX_SHOULD_BUILDER_CONSIDER_PLOT_EXISTING_BUILD_MISSIONS_SHIFT
	// check to see if someone already has a mission here
	if(pUnit->GetMissionAIPlot() != pPlot)
	{
		if(m_pPlayer->AI_plotTargetMissionAIs(pPlot, MISSIONAI_BUILD) > 0)
		{
			if(m_bLogging)
			{
				CvString strLog;
				strLog.Format("x: %d y: %d,,Somebody has a mission here, ", pPlot->getX(), pPlot->getY());
				LogInfo(strLog, m_pPlayer, true);
			}

			return false;
		}
	}
#endif

#if defined(AUI_WORKER_SHOULD_BUILDER_CONSIDER_PLOT_MAXIMUM_DANGER_BASED_ON_UNIT_STRENGTH)
if ((!pUnit->IsCombatUnit() && m_pPlayer->GetPlotDanger(*pPlot) > 0) ||
	m_pPlayer->GetPlotDanger(*pPlot) > pUnit->GetBaseCombatStrengthConsideringDamage() * AUI_WORKER_SHOULD_BUILDER_CONSIDER_PLOT_MAXIMUM_DANGER_BASED_ON_UNIT_STRENGTH)
#else
if (m_pPlayer->GetPlotDanger(*pPlot) > 0)
#endif
	if(m_pPlayer->GetPlotDanger(*pPlot) > 0)
	{
		if(m_bLogging)
		{
			CvString strLog;
			strLog.Format("plotX: %d plotY: %d, danger: %d,, bailing due to danger", pPlot->getX(), pPlot->getY(), m_pPlayer->GetPlotDanger(*pPlot));
			LogInfo(strLog, m_pPlayer, true);
		}

		return false;
	}

	if(!pUnit->atPlot(*pPlot) && pPlot->getNumFriendlyUnitsOfType(pUnit) >= GC.getPLOT_UNIT_LIMIT())
	{
		if(m_bLogging)
		{
			CvString strLog;
			strLog.Format("plotX: %d plotY: %d,, this tile is full with another unit. bailing!", pPlot->getX(), pPlot->getY());
			LogInfo(strLog, m_pPlayer, true);
		}

		return false;
	}

	return true;
}

/// Determines if the builder can get to the plot. Returns -1 if no path can be found, otherwise it returns the # of turns to get there
int CvBuilderTaskingAI::FindTurnsAway(CvUnit* pUnit, CvPlot* pPlot)
{
	// If this plot is far away, we'll just use its distance as an estimate of the time to get there (to avoid hitting the path finder)
	// We'll be sure to check later to make sure we have a real path before we execute this
	if(pUnit->getDomainType() == DOMAIN_LAND && pUnit->plot()->area() != pPlot->area() && !pUnit->CanEverEmbark())
	{
		return -1;
	}

#ifdef AUI_WORKER_FIND_TURNS_AWAY_USES_PATHFINDER
	int iPlotDistance = TurnsToReachTarget(pUnit, pPlot, true /*bReusePaths*/, AUI_WORKER_FIND_TURNS_AWAY_USES_PATHFINDER /*bIgnoreUnits*/);
	if (iPlotDistance < MAX_INT)
		return iPlotDistance;
	else
		return -1;
#else
	int iPlotDistance = plotDistance(pUnit->getX(), pUnit->getY(), pPlot->getX(), pPlot->getY());
#if 1
	// Always return the raw distance
	return iPlotDistance;
#else
	if(iPlotDistance >= GC.getAI_HOMELAND_ESTIMATE_TURNS_DISTANCE())
	{
		return iPlotDistance;
	}
	else
	{
		int iResult = TurnsToReachTarget(pUnit, pPlot);
		if(iResult == MAX_INT)
		{
			return -1;
		}

		return iResult;
	}
#endif
#endif
}

/// Get the weight determined by the cost of building the item
int CvBuilderTaskingAI::GetBuildCostWeight(int iWeight, CvPlot* pPlot, BuildTypes eBuild)
{
	int iBuildCost = m_pPlayer->getBuildCost(pPlot, eBuild);
	if(iBuildCost > 0)
	{
		iWeight = (iWeight * 100) / iBuildCost;
	}
	else
	{
		//if (m_bLogging)
		//{
		//	LogInfo("Build cost is zero", m_pPlayer);
		//}
		iWeight = (iWeight * 100);
	}
	return iWeight;
}

/// Get the weight determined by the building time of the item
int CvBuilderTaskingAI::GetBuildTimeWeight(CvUnit* pUnit, CvPlot* pPlot, BuildTypes eBuild, bool bIgnoreFeatureTime, int iAdditionalTime)
{
	// if we need to repair this plot, replace the build with a repair build
	if((GC.getBuildInfo(eBuild)->getImprovement() != NO_IMPROVEMENT && pPlot->IsImprovementPillaged()) || (GC.getBuildInfo(eBuild)->getRoute() != NO_ROUTE && pPlot->IsRoutePillaged()))
	{
		// find a repair directive to replace
		// find the repair build
		eBuild = m_eRepairBuild;
	}

	int iBuildTimeNormal = pPlot->getBuildTime(eBuild, m_pPlayer->GetID());
	int iBuildTurnsLeft = pPlot->getBuildTurnsLeft(eBuild, m_pPlayer->GetID(), pUnit->workRate(true), pUnit->workRate(true));
	int iBuildTime = min(iBuildTimeNormal, iBuildTurnsLeft);
	if(iBuildTime <= 0)
	{
		iBuildTime = 1;
	}

	if(bIgnoreFeatureTime)
	{
		if(pPlot->getFeatureType() != NO_FEATURE)
		{
			iBuildTime -= GC.getBuildInfo(eBuild)->getFeatureTime(pPlot->getFeatureType());
		}
	}

	iBuildTime += iAdditionalTime;

	return 10000 / iBuildTime;
}

/// Return the weight of this resource
int CvBuilderTaskingAI::GetResourceWeight(ResourceTypes eResource, ImprovementTypes eImprovement, int iQuantity)
{
	int iWeight = 0;
	CvResourceInfo* pkResource = GC.getResourceInfo(eResource);
	if(pkResource == NULL)
	{
		return 0;
	}

	for(int i = 0; i < GC.getNumFlavorTypes(); i++)
	{
		int iResourceFlavor = pkResource->getFlavorValue((FlavorTypes)i);
		int iPersonalityFlavorValue = m_pPlayer->GetFlavorManager()->GetPersonalityIndividualFlavor((FlavorTypes)i);
		int iResult = iResourceFlavor * iPersonalityFlavorValue;

		if(iResult > 0)
		{
			iWeight += iResult;
		}

		int iImprovementFlavor = 1;
		if(eImprovement != NO_IMPROVEMENT)
		{
			iImprovementFlavor = GC.getImprovementInfo(eImprovement)->GetFlavorValue(i);
		}

		int iUsableByCityWeight = iPersonalityFlavorValue * iImprovementFlavor;
		if(iUsableByCityWeight > 0)
		{
			iWeight += iUsableByCityWeight;
		}
	}

	// if the empire is unhappy (or close to it) and this is a luxury resource the player doesn't have, provide a super bonus to getting it
	if(pkResource->getResourceUsage() == RESOURCEUSAGE_LUXURY)
	{
		int iModifier = GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_LUXURY_RESOURCE() * pkResource->getHappiness();
#ifdef AUI_WORKER_GET_RESOURCE_WEIGHT_CONSIDER_EXTRAS_FOR_HAPPINESS_FROM_RESOURCE
		iModifier += GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_LUXURY_RESOURCE() * m_pPlayer->GetExtraHappinessPerLuxury();
#endif

		//if (m_pPlayer->IsEmpireUnhappy() || m_pPlayer->GetExcessHappiness() <= 2)
		//{
		//}
		if(m_pPlayer->getNumResourceAvailable(eResource) == 0)
		{
			// full bonus
#ifdef AUI_WORKER_GET_RESOURCE_WEIGHT_CONSIDER_EXTRAS_FOR_HAPPINESS_FROM_RESOURCE
			if (m_pPlayer->GetPlayerTraits()->GetLuxuryHappinessRetention() > 0)
				iModifier = iModifier * (m_pPlayer->GetPlayerTraits()->GetLuxuryHappinessRetention() + pkResource->getHappiness()) / pkResource->getHappiness();
			iModifier += GC.getBUILDER_TASKING_PLOT_EVAL_MULTIPLIER_LUXURY_RESOURCE() * GC.getHAPPINESS_PER_EXTRA_LUXURY();
#endif
#ifdef AUI_WORKER_GET_RESOURCE_WEIGHT_INCREASE_UNOWNED_LUXURY_WEIGHT
			if (m_pPlayer->GetExcessHappiness() < -GC.getVERY_UNHAPPY_THRESHOLD())
			{
				iModifier = int(iModifier * pow(AUI_WORKER_GET_RESOURCE_WEIGHT_INCREASE_UNOWNED_LUXURY_WEIGHT, 1.0 - (double)m_pPlayer->GetExcessHappiness() / -(double)GC.getVERY_UNHAPPY_THRESHOLD()) + 0.5);
			}
#endif
		}
		else
		{
			iModifier = (iModifier * 3) / 4; // 3/4ths the awesome bonus, so that we pick up extra resources 
		}

		iWeight *= iModifier;
	}
	else if(pkResource->getResourceUsage() == RESOURCEUSAGE_STRATEGIC && pkResource->getTechCityTrade())
	{
		bool bHasTech = GET_TEAM(m_pPlayer->getTeam()).GetTeamTechs()->HasTech((TechTypes) pkResource->getTechCityTrade());
		if(bHasTech)
		{
			// measure quantity
			int iMultiplyingAmount = iQuantity * 200;

			// if we don't have any currently available
			if(m_pPlayer->getNumResourceAvailable(eResource) == 0)
			{
				// if we have some of the strategic resource, but all is used
				if(m_pPlayer->getNumResourceUsed(eResource) > 0)
				{
					iMultiplyingAmount *= 4;
				}
				else
				{
					// if we don't have any of it
#ifdef AUI_WORKER_TWEAKED_DONT_HAVE_MULTIPLIER
					iMultiplyingAmount *= AUI_WORKER_TWEAKED_DONT_HAVE_MULTIPLIER;
#else
					iMultiplyingAmount *= 4;
#endif
				}
			}

			iWeight *= iMultiplyingAmount;
		}
	}

	return iWeight;
}

#ifndef NQM_PRUNING
/// Determine if an improvement will increase any of the outputs of the plot
bool CvBuilderTaskingAI::IsImprovementBeneficial(CvPlot* pPlot, const CvBuildInfo& kBuild, YieldTypes eYield, bool bIsBreakEvenOK)
{
	const ImprovementTypes eImprovement = (ImprovementTypes)kBuild.getImprovement();

	const FeatureTypes eFeature = pPlot->getFeatureType();

	bool bFeatureNeedsRemove = false;

	if(eFeature != NO_FEATURE)
	{
		if(kBuild.isFeatureRemove(eFeature))
		{
			bFeatureNeedsRemove = true;
		}
	}

	CvImprovementEntry* pkImprovementInfo = NULL;
	if(eImprovement != NO_IMPROVEMENT)
	{
		pkImprovementInfo = GC.getImprovementInfo(eImprovement);
	}
	CvAssert(pkImprovementInfo);

	//This can technically happen if a build as passed in that doesn't contain an improvement..
	//Returning false isn't the ideal error approach, but there's nothing better to do atm.
	if(pkImprovementInfo == NULL)
	{
		return false;
	}

	CvRouteInfo* pkPlotRouteInfo = NULL;
	if(pPlot->getRouteType() != NO_ROUTE)
	{
		pkPlotRouteInfo = GC.getRouteInfo(pPlot->getRouteType());
	}


	int aiNaturalYieldTypes[NUM_YIELD_TYPES];
	int aiImprovedYieldTypes[NUM_YIELD_TYPES];

	// hacky solution for the great artist's landmark.
	// if an improvement generates culture, then it is beneficial
	if(pkImprovementInfo->GetYieldChange(YIELD_CULTURE) > 0)
	{
		return true;
	}

	for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		// calculate natural yields
		aiNaturalYieldTypes[ui] = 0;
		aiNaturalYieldTypes[ui] = pPlot->calculateNatureYield((YieldTypes)ui, m_pPlayer->getTeam());

		// calculate improvement yields
		aiImprovedYieldTypes[ui] = 0;

		bool bIgnoreFeature = false;
		if(bFeatureNeedsRemove)
		{
			bIgnoreFeature = true;
		}

		aiImprovedYieldTypes[ui] = pPlot->calculateNatureYield((YieldTypes)ui, m_pPlayer->getTeam(), bIgnoreFeature);
		if(pkPlotRouteInfo)
		{
			aiImprovedYieldTypes[ui] += pkPlotRouteInfo->getYieldChange(ui);
		}
		aiImprovedYieldTypes[ui] += pPlot->calculateImprovementYieldChange(eImprovement, (YieldTypes)ui, m_pPlayer->GetID(), false /*bOptimal*/);
	}

	if(eYield == NO_YIELD)
	{
		// if any of the yields are increased from their natural state, then the improvement is considered a success
		for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
		{
			if(bIsBreakEvenOK)
			{
				if(aiImprovedYieldTypes[ui] >= aiNaturalYieldTypes[ui])
				{
					return true;
				}
			}
			else
			{
				if(aiImprovedYieldTypes[ui] > aiNaturalYieldTypes[ui])
				{
					return true;
				}
			}
		}
	}
	else
	{
		if(bIsBreakEvenOK)
		{
			if(aiImprovedYieldTypes[eYield] >= aiNaturalYieldTypes[eYield])
			{
				return true;
			}
		}
		else
		{
			if(aiImprovedYieldTypes[eYield] > aiNaturalYieldTypes[eYield])
			{
				return true;
			}
		}
	}

	return false;
}
#endif

/// Get this city that can interact with this plot
CvCity* CvBuilderTaskingAI::GetWorkingCity(CvPlot* pPlot)
{
#ifdef AUI_WARNING_FIXES
	CvCity* pCity = pPlot->getWorkingCity();
	if (!pCity)
#else
	CvCity* pCity = NULL;
	if(pPlot->getWorkingCity())
	{
		pCity = pPlot->getWorkingCity();
	}
	else
#endif
	{
		CvCity* pLoopCity;
		int iLoop;
		for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
		{
			if(pLoopCity->GetCityCitizens()->IsCanWork(pPlot))
			{
#ifdef AUI_WARNING_FIXES
				return pLoopCity;
#else
				pCity = pLoopCity;
				break;
#endif
			}
		}
	}

	return pCity;
}

/// Does this city want to rush a unit?
bool CvBuilderTaskingAI::DoesBuildHelpRush(CvUnit* pUnit, CvPlot* pPlot, BuildTypes eBuild)
{
	CvCity* pCity = NULL;
	int iProduction = pPlot->getFeatureProduction(eBuild, pUnit->getOwner(), &pCity);
	if(iProduction <= 0)
	{
		return false;
	}

	if(!pCity)
	{
		// this chop does not benefit any city
		return false;
	}

	if(pCity->getOrderQueueLength() <= 0)
	{
		// nothing in the build queue
		return false;
	}

	if(!(pCity->getOrderFromQueue(0)->bRush))
	{
		// this order should not be rushed
		return false;
	}

	return true;
}

#ifdef AUI_WORKER_SCORE_PLOT_CHOP
int CvBuilderTaskingAI::ScorePlot(BuildTypes eBuild) const
#elif defined(AUI_CONSTIFY)
int CvBuilderTaskingAI::ScorePlot() const
#else
int CvBuilderTaskingAI::ScorePlot()
#endif
{
	if(!m_pTargetPlot)
	{
		return -1;
	}

	CvCity* pCity = m_pTargetPlot->getWorkingCity();
#ifdef AUI_WORKER_SCORE_PLOT_NO_SCORE_FROM_RAZE
	if (!pCity || pCity->IsRazing())
#else
	if(!pCity)
#endif
	{
		return -1;
	}

	CvCityStrategyAI* pCityStrategy = pCity->GetCityStrategyAI();
	if(!pCityStrategy)
	{
		return -1;
	}

#ifdef AUI_WORKER_SCORE_PLOT_CHOP
	CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);
	if (!pkBuild)
	{
		return -1;
	}
	FeatureTypes ePlotFeature = m_pTargetPlot->getFeatureType();
#endif

#ifdef AUI_WORKER_SCORE_PLOT_MULTIPLY_SCORE_IF_WOULD_WORK
	bool bWouldBeWorked = m_pTargetPlot->isBeingWorked();
#endif
	int iScore = 0;
	bool bAnyNegativeMultiplier = false;
	YieldTypes eFocusYield = pCityStrategy->GetFocusYield();
	for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		int iMultiplier = pCityStrategy->GetYieldDeltaTimes100((YieldTypes)ui);
		int iAbsMultiplier = abs(iMultiplier);
		int iYieldDelta = m_aiProjectedPlotYields[ui] - m_aiCurrentPlotYields[ui];
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
		double dFlatBonus = 0;
		if (ePlotFeature != NO_FEATURE && pkBuild->isFeatureRemove(ePlotFeature) && ui == YIELD_PRODUCTION && iYieldDelta >= 0)
		{
			dFlatBonus = AUI_WORKER_SCORE_PLOT_CHOP;
		}
#endif

		// the multiplier being lower than zero means that we need more of this resource
		if(iMultiplier < 0)
		{
			bAnyNegativeMultiplier = true;
			if(iYieldDelta > 0)  // this would be an improvement to the yield
			{
#ifdef AUI_WORKER_SCORE_PLOT_CHOP
				iScore += int((m_aiProjectedPlotYields[ui] + dFlatBonus) * iAbsMultiplier + 0.5);
			}
			else if (iYieldDelta < 0)  // the yield would go down
			{
				iScore += int((iYieldDelta + dFlatBonus) * iAbsMultiplier - 0.5);
			}
		}
		else
		{
			if (iYieldDelta >= 0)
			{
				iScore += int(m_aiProjectedPlotYields[ui] + dFlatBonus + 0.5); // provide a nominal score to plots that improve anything
			}
			else if (iYieldDelta < 0)
			{
				iScore += int((iYieldDelta + dFlatBonus) * iAbsMultiplier - 0.5);
#else
				iScore += m_aiProjectedPlotYields[ui] * iAbsMultiplier;
			}
			else if(iYieldDelta < 0)  // the yield would go down
			{
				iScore += iYieldDelta * iAbsMultiplier;
			}
		}
		else
		{
			if(iYieldDelta >= 0)
			{
				iScore += m_aiProjectedPlotYields[ui]; // provide a nominal score to plots that improve anything
			}
			else if(iYieldDelta < 0)
			{
				iScore += iYieldDelta * iAbsMultiplier;
#endif
			}
		}
	}

	if(!bAnyNegativeMultiplier && eFocusYield != NO_YIELD)
	{
		int iYieldDelta = m_aiProjectedPlotYields[eFocusYield] - m_aiCurrentPlotYields[eFocusYield];
		if(iYieldDelta > 0)
		{
#ifdef AUI_WORKER_SCORE_PLOT_EFFECT_FROM_CITY_FOCUS
			iScore += m_aiProjectedPlotYields[eFocusYield] * AUI_WORKER_SCORE_PLOT_EFFECT_FROM_CITY_FOCUS;
#else
			iScore += m_aiProjectedPlotYields[eFocusYield] * 100;
#endif
		}
	}

#ifndef AUI_WORKER_SCORE_PLOT_NO_CAPITOL_FAVORING
	if (pCity->isCapital()) // this is our capital and needs emphasis
	{
		iScore *= 8;
	}
	else if (pCity->IsOriginalCapital()) // this was a particularly good city and needs a little emphasis
	{
		iScore *= 2;
	}
#endif
#ifdef AUI_WORKER_SCORE_PLOT_MULTIPLY_SCORE_IF_WOULD_WORK
	if (bWouldBeWorked)
	{
		iScore *= AUI_WORKER_SCORE_PLOT_MULTIPLY_SCORE_IF_WOULD_WORK;
	}
#endif
#ifdef AUI_WORKER_SCORE_PLOT_REDUCED_PUPPET_SCORE
	if (pCity->IsPuppet())
	{
		iScore /= AUI_WORKER_SCORE_PLOT_REDUCED_PUPPET_SCORE;
	}
#endif

	return iScore;
}

#ifdef AUI_CONSTIFY
BuildTypes CvBuilderTaskingAI::GetBuildTypeFromImprovement(ImprovementTypes eImprovement) const
#else
BuildTypes CvBuilderTaskingAI::GetBuildTypeFromImprovement(ImprovementTypes eImprovement)
#endif
{
#ifdef AUI_WARNING_FIXES
	for (uint iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#else
	for(int iBuildIndex = 0; iBuildIndex < GC.getNumBuildInfos(); iBuildIndex++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)iBuildIndex;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);

		if(NULL != pkBuild && eImprovement == (ImprovementTypes)pkBuild->getImprovement())
		{
			return eBuild;
		}
	}

	return NO_BUILD;
}

#ifdef AUI_CONSTIFY
BuildTypes CvBuilderTaskingAI::GetRepairBuild() const
#else
BuildTypes CvBuilderTaskingAI::GetRepairBuild(void)
#endif
{
#ifdef AUI_WARNING_FIXES
	for (uint i = 0; i < GC.getNumBuildInfos(); i++)
#else
	for(int i = 0; i < GC.getNumBuildInfos(); i++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)i;
		CvBuildInfo* pkBuild = GC.getBuildInfo(eBuild);

		if(NULL != pkBuild && pkBuild->isRepair())
		{
			return eBuild;
		}
	}

	return NO_BUILD;
}

#ifdef AUI_CONSTIFY
FeatureTypes CvBuilderTaskingAI::GetFalloutFeature() const
#else
FeatureTypes CvBuilderTaskingAI::GetFalloutFeature(void)
#endif
{
	return static_cast<FeatureTypes>(GC.getNUKE_FEATURE());
}

#ifdef AUI_CONSTIFY
BuildTypes CvBuilderTaskingAI::GetFalloutRemove() const
#else
BuildTypes CvBuilderTaskingAI::GetFalloutRemove(void)
#endif
{
	FeatureTypes eFalloutFeature = m_eFalloutFeature;
	if(eFalloutFeature == NO_FEATURE)
	{
		eFalloutFeature = GetFalloutFeature();
	}

	if(eFalloutFeature == NO_FEATURE)
	{
		return NO_BUILD;
	}

#ifdef AUI_WARNING_FIXES
	for (uint iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
#else
	for(int iBuild = 0; iBuild < GC.getNumBuildInfos(); iBuild++)
#endif
	{
		BuildTypes eBuild = (BuildTypes)iBuild;
		CvBuildInfo* pBuildInfo = GC.getBuildInfo(eBuild);
		if(NULL != pBuildInfo && pBuildInfo->isFeatureRemove(eFalloutFeature))
		{
			return eBuild;
		}
	}

	return NO_BUILD;
}

/// Central logging repository!
#ifdef AUI_WARNING_FIXES
void CvBuilderTaskingAI::LogInfo(const CvString& strNewLogStr, CvPlayer* pPlayer, bool /*bWriteToOutput*/)
#else
void CvBuilderTaskingAI::LogInfo(CvString strNewLogStr, CvPlayer* pPlayer, bool /*bWriteToOutput*/)
#endif
{
	if(!(GC.getLogging() && GC.getAILogging() && GC.GetBuilderAILogging()))
	{
		return;
	}

	// Open the log file
	CvString strFileName = "BuilderTaskingLog.csv";
	FILogFile* pLog;
	pLog = LOGFILEMGR.GetLog(strFileName, FILogFile::kDontTimeStamp);

	CvString strLog, strTemp;

	CvString strPlayerName;
	strPlayerName = pPlayer->getCivilizationShortDescription();
	strLog += strPlayerName;
	strLog += ",";

	strTemp.Format("%d,", GC.getGame().getGameTurn()); // turn
	strLog += strTemp;
	strLog += strNewLogStr;
	pLog->Msg(strLog);
}

#ifdef AUI_WARNING_FIXES
void CvBuilderTaskingAI::LogYieldInfo(const CvString& strNewLogStr, CvPlayer* pPlayer)
#else
void CvBuilderTaskingAI::LogYieldInfo(CvString strNewLogStr, CvPlayer* pPlayer)
#endif
{
	if(!(GC.getLogging() && GC.getAILogging() && GC.GetBuilderAILogging()))
	{
		return;
	}

	// Open the log file
	CvString strFileName = "BuilderTaskingYieldLog.csv";
	FILogFile* pLog;
	pLog = LOGFILEMGR.GetLog(strFileName, FILogFile::kDontTimeStamp);

	CvString strLog, strTemp;

	CvString strPlayerName;
	strPlayerName = pPlayer->getCivilizationShortDescription();
	strLog += strPlayerName;
	strLog += ",";

	strTemp.Format("%d,", GC.getGame().getGameTurn()); // turn
	strLog += strTemp;
	strLog += strNewLogStr;
	pLog->Msg(strLog);
}

/// Log flavor information out
void CvBuilderTaskingAI::LogFlavors(FlavorTypes eFlavor)
{
	if(!m_bLogging)
	{
		return;
	}

	// Open the log file
	CvString strLog;
	CvString strTemp;

	// Dump out the setting for each flavor
	if(eFlavor == NO_FLAVOR)
	{
		for(int iI = 0; iI < GC.getNumFlavorTypes(); iI++)
		{
			strLog.clear();
			strTemp.Format("Flavor, %s, %d,", GC.getFlavorTypes((FlavorTypes)iI).GetCString(), m_pPlayer->GetFlavorManager()->GetPersonalityIndividualFlavor((FlavorTypes)iI));
			strLog += strTemp;
			LogInfo(strLog, m_pPlayer);
		}
	}
	else
	{
		strLog.clear();
		strTemp.Format("Flavor, %s, %d,", GC.getFlavorTypes(eFlavor).GetCString(), m_pPlayer->GetFlavorManager()->GetPersonalityIndividualFlavor(eFlavor));
		strLog += strTemp;
		LogInfo(strLog, m_pPlayer);
	}
}

/// Logs all the directives for the unit
void CvBuilderTaskingAI::LogDirectives(CvUnit* pUnit)
{
	if(!m_bLogging)
	{
		return;
	}

	if(m_aDirectives.size() > 0)
	{
		for(int i = 0; i < m_aDirectives.size(); i++)
		{
			LogDirective(m_aDirectives.GetElement(i), pUnit, m_aDirectives.GetWeight(i));
		}
	}
	else
	{
		CvString strLog;
		CvString strTemp;

		strTemp.Format("%d,", pUnit->GetID()); // unit id
		strLog += strTemp;

		strLog += "No directives!";
		LogInfo(strLog, m_pPlayer);
	}
}

void CvBuilderTaskingAI::LogDirective(BuilderDirective directive, CvUnit* pUnit, int iWeight, bool bChosen)
{
	if(!m_bLogging)
	{
		return;
	}

	CvString strLog;
	CvString strTemp;

	strTemp.Format("%d,", pUnit->GetID()); // unit id
	strLog += strTemp;

	strLog += "Evaluating,";

	switch(directive.m_eDirective)
	{
	case BuilderDirective::BUILD_IMPROVEMENT_ON_RESOURCE:
		strLog += "BUILD_IMPROVEMENT_ON_RESOURCE,";
		break;
	case BuilderDirective::BUILD_IMPROVEMENT:
		strLog += "BUILD_IMPROVEMENT,";
		break;
	case BuilderDirective::REPAIR:
		strLog += "REPAIR,";
		break;
	case BuilderDirective::BUILD_ROUTE:
		strLog += "BUILD_ROUTE,";
		break;
	case BuilderDirective::CHOP:
		strLog += "CHOP";
	}

	strLog += GC.getBuildInfo(directive.m_eBuild)->GetType();
	strLog += ",";

	if(directive.m_eResource != NO_RESOURCE)
	{
		CvResourceInfo* pkResourceInfo = GC.getResourceInfo(directive.m_eResource);
		if(pkResourceInfo)
		{
			strLog += pkResourceInfo->GetType();
			strLog += ",";
			CvPlot* pPlot = GC.getMap().plot(directive.m_sX, directive.m_sY);
#ifdef AUI_WARNING_FIXES
			strTemp.Format("%d,", pPlot->getNumResource());
			strLog += strTemp;
#else
			strLog += pPlot->getNumResource();
#endif
			strLog += ",";
		}
	}
	else
	{
		strLog += ",";
	}

	if(directive.m_eDirective == BuilderDirective::REPAIR)
	{
		CvPlot* pPlot = GC.getMap().plot(directive.m_sX, directive.m_sY);
		if(pPlot->IsImprovementPillaged())
		{
			if(pPlot->getImprovementType() != NO_IMPROVEMENT)
			{
				CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(pPlot->getImprovementType());
				if(pkImprovementInfo)
				{
					strLog += pkImprovementInfo->GetType();
				}
			}
		}
		else
		{
			if(pPlot->getRouteType() != NO_ROUTE)
			{
				CvRouteInfo* pkRouteInfo = GC.getRouteInfo(pPlot->getRouteType());
				if(pkRouteInfo)
				{
					strLog += pkRouteInfo->GetType();
				}
			}
		}
	}
	else
	{
		CvPlot* pPlot = GC.getMap().plot(directive.m_sX, directive.m_sY);
		if(pPlot->getImprovementType() != NO_IMPROVEMENT)
		{
			CvImprovementEntry* pkImprovementInfo = GC.getImprovementInfo(pPlot->getImprovementType());
			if(pkImprovementInfo)
			{
				strLog += pkImprovementInfo->GetType();
			}
		}
	}
	strLog += ",";

	strTemp.Format("%d,%d,", directive.m_sX, directive.m_sY);
	strLog += strTemp;

	strTemp.Format("%d,", directive.m_sMoveTurnsAway);
	strLog += strTemp;

	strTemp.Format("%d,", iWeight);
	strLog += strTemp;

	if(bChosen)
	{
		strLog += (", Chosen!");
	}

	LogInfo(strLog, m_pPlayer, GET_PLAYER(pUnit->getOwner()).isHuman());
}

// looks at the current plot to see what it's worth
void CvBuilderTaskingAI::UpdateCurrentPlotYields(CvPlot* pPlot)
{
	m_pTargetPlot = pPlot;
	for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		m_aiCurrentPlotYields[ui] = pPlot->getYield((YieldTypes)ui);

		if(m_bLogging){
			CvString strLog;
			YieldTypes yield = (YieldTypes) ui;
			strLog.Format("Plot Yield Update, %s, %i, %i, %i", FSerialization::toString(yield).c_str(), m_aiCurrentPlotYields[ui], pPlot->getX(), pPlot->getY());
			LogYieldInfo(strLog, m_pPlayer);
		}
	}
}

// looks at the current plot assuming the build to see what it's worth
void CvBuilderTaskingAI::UpdateProjectedPlotYields(CvPlot* pPlot, BuildTypes eBuild)
{
	if(pPlot != m_pTargetPlot)
	{
		UpdateCurrentPlotYields(pPlot);
	}

	for(uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		m_aiProjectedPlotYields[ui] = pPlot->getYieldWithBuild(eBuild, (YieldTypes)ui, false, m_pPlayer->GetID());
		m_aiProjectedPlotYields[ui] = max(m_aiProjectedPlotYields[ui], 0);
#ifdef AUI_WORKER_FIX_CELTIC_IMPROVE_UNIMPROVED_FORESTS
		if (ui == YIELD_FAITH && m_pPlayer->GetPlayerTraits()->IsFaithFromUnimprovedForest() && pPlot->getImprovementType() == NO_IMPROVEMENT)
		{
			CvBuildInfo* pBuildInfo = GC.getBuildInfo(eBuild);
			if (pBuildInfo && (pBuildInfo->getImprovement() != NO_IMPROVEMENT || pBuildInfo->isFeatureRemove(FEATURE_FOREST)))
			{
				CvCity* pNextCity = pPlot->GetAdjacentCity();
				if (pNextCity && pNextCity->getOwner() == m_pPlayer->GetID())
				{
					int iNeighboringForestCount = 1;
					CvPlot* pAdjacentPlot = NULL;
					for (int iI = 0; iI < NUM_DIRECTION_TYPES; iI++)
					{
						pAdjacentPlot = pNextCity->plot()->getNeighboringPlot((DirectionTypes)iI);
						if (pAdjacentPlot && pAdjacentPlot != pPlot && pAdjacentPlot->getFeatureType() == FEATURE_FOREST && pPlot->getImprovementType() == NO_IMPROVEMENT)
						{
							ResourceTypes eAdjacentResource = pPlot->getResourceType(m_pPlayer->getTeam());
							if (eAdjacentResource == NO_RESOURCE || GC.getResourceInfo(eAdjacentResource)->getResourceUsage() == RESOURCEUSAGE_BONUS)
							{
								iNeighboringForestCount++;
							}
						}
					}
					if (iNeighboringForestCount == 3 || iNeighboringForestCount == 1)
					{
						m_aiProjectedPlotYields[YIELD_FAITH] -= 1;
					}
				}
			}
		}
#endif

		if(m_bLogging){
			CvString strLog;
			YieldTypes yield = (YieldTypes) ui;
			strLog.Format("Plot Projected Yield Update, %s, %i, %i, %i", FSerialization::toString(yield).c_str(), m_aiProjectedPlotYields[ui], pPlot->getX(), pPlot->getY());
			LogYieldInfo(strLog, m_pPlayer);
		}
	}
}
