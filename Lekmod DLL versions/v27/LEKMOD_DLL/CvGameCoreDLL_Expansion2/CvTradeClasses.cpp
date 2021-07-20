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
#include "CvCitySpecializationAI.h"

#include "CvBarbarians.h"

#include "CvNotifications.h"
#include "cvStopWatch.h"
#include "CvCityManager.h"

#include "LintFree.h"

//=====================================
// CvGameTrade
//=====================================

#define TEMPORARY_POPUPROUTE_ID (-1) //m_aTradeConnections will never use this as an index
#define MAX_PLOTS_TO_DISPLAY 256

//	--------------------------------------------------------------------------------
/// Default constructor
#ifdef AUI_WARNING_FIXES
CvGameTrade::CvGameTrade() : m_aaiTechDifference()
#else
CvGameTrade::CvGameTrade()
#endif
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Destructor
CvGameTrade::~CvGameTrade(void)
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Init
void CvGameTrade::Init (void)
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Uninit
void CvGameTrade::Uninit (void)
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Uninit
void CvGameTrade::Reset (void)
{
	m_aTradeConnections.clear();
	m_iNextID = 0;
	m_CurrentTemporaryPopupRoute.iPlotX = 0;
	m_CurrentTemporaryPopupRoute.iPlotY = 0;
	m_CurrentTemporaryPopupRoute.type = TRADE_CONNECTION_INTERNATIONAL;
#ifdef AUI_WARNING_FIXES
	ResetTechDifference();
#endif
}

//	--------------------------------------------------------------------------------
/// DoTurn
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
void CvGameTrade::DoTurn(PlayerTypes eForPlayer)
{
	ResetTechDifference(eForPlayer);
	BuildTechDifference(eForPlayer);
#else
void CvGameTrade::DoTurn (void)
{
	ResetTechDifference();
	BuildTechDifference();
#endif
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::CanCreateTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath /*= true*/) const
#else
bool CvGameTrade::CanCreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath /*= true*/)
#endif
{
	if (pOriginCity == NULL)
	{
		return false;
	}

	if (pDestCity == NULL)
	{
		return false;
	}

	// A city may not trade with itself
	if (pOriginCity == pDestCity)
	{
		return false;
	}

	TeamTypes eOriginTeam = pOriginCity->getTeam();
	TeamTypes eDestTeam = pDestCity->getTeam();

	if (eConnectionType == TRADE_CONNECTION_INTERNATIONAL)
	{
		// can't have an international trade route within the same team
		if (eOriginTeam == eDestTeam)
		{
			return false;
		}
	}
	else if (eConnectionType == TRADE_CONNECTION_PRODUCTION || eConnectionType == TRADE_CONNECTION_FOOD)
	{
		// can't have production or food connections internationally
		if (eOriginTeam != eDestTeam)
		{
			return false;
		}

		if (eConnectionType == TRADE_CONNECTION_FOOD)
		{
			bool bAllowsFoodConnection = false;
#ifdef AUI_WARNING_FIXES
			for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
			for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
			{
				BuildingTypes eBuilding = (BuildingTypes)GET_PLAYER(pOriginCity->getOwner()).getCivilizationInfo().getCivilizationBuildings(iI);
				if(eBuilding != NO_BUILDING)
				{
					CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);
#ifndef AUI_WARNING_FIXES
					if (!pBuildingEntry)
					{
						continue;
					}
#endif

					if (pBuildingEntry && pBuildingEntry->AllowsFoodTradeRoutes())
					{
						if (pOriginCity->GetCityBuildings()->GetNumBuilding((BuildingTypes)pBuildingEntry->GetID()) > 0)
						{
							bAllowsFoodConnection = true;
						}
					}
				}
			}

			if (!bAllowsFoodConnection)
			{
				return false;
			}
		}
		else if (eConnectionType == TRADE_CONNECTION_PRODUCTION)
		{
			bool bAllowsProductionConnection = false;
#ifdef AUI_WARNING_FIXES
			for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
			for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
			{
				BuildingTypes eBuilding = (BuildingTypes)GET_PLAYER(pOriginCity->getOwner()).getCivilizationInfo().getCivilizationBuildings(iI);
				if(eBuilding != NO_BUILDING)
				{
					CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);
#ifndef AUI_WARNING_FIXES
					if (!pBuildingEntry)
					{
						continue;
					}
#endif

					if (pBuildingEntry && pBuildingEntry->AllowsProductionTradeRoutes())
					{
						if (pOriginCity->GetCityBuildings()->GetNumBuilding((BuildingTypes)pBuildingEntry->GetID()) > 0)
						{
							bAllowsProductionConnection = true;
						}
					}
				}
			}

			if (!bAllowsProductionConnection)
			{
				return false;
			}
		}
	}

	// teams at war can't fight
	if (GET_TEAM(eOriginTeam).isAtWar(eDestTeam))
	{
		return false;
	}

	// teams that have not met cannot trade
	if (!GET_TEAM(eOriginTeam).isHasMet(eDestTeam))
	{
		return false;
	}

	if (pOriginCity->IsRazing() || pDestCity->IsRazing())
	{
		return false;
	}

	if (!bIgnoreExisting)
	{
		int iOriginX = pOriginCity->getX();
		int iOriginY = pOriginCity->getY();
		int iDestX = pDestCity->getX();
		int iDestY = pDestCity->getY();

		// check for duplicate routes
		for (uint i = 0; i < m_aTradeConnections.size(); i++)
		{
			if (m_aTradeConnections[i].m_iOriginX == iOriginX && m_aTradeConnections[i].m_iOriginY == iOriginY && m_aTradeConnections[i].m_iDestX == iDestX && m_aTradeConnections[i].m_iDestY == iDestY)
			{
				return false;
			}
		}
	}

	if (bCheckPath && !IsValidTradeRoutePath(pOriginCity, pDestCity, eDomain))
	{
		return false;
	}

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::CanCreateTradeRoute(PlayerTypes eOriginPlayer, PlayerTypes eDestPlayer, DomainTypes eDomainRestriction) const
#else
bool CvGameTrade::CanCreateTradeRoute(PlayerTypes eOriginPlayer, PlayerTypes eDestPlayer, DomainTypes eDomainRestriction)
#endif
{
	CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();

	int iOriginCityLoop;
	CvCity* pOriginLoopCity = NULL;
	for(pOriginLoopCity = GET_PLAYER(eOriginPlayer).firstCity(&iOriginCityLoop); pOriginLoopCity != NULL; pOriginLoopCity = GET_PLAYER(eOriginPlayer).nextCity(&iOriginCityLoop))
	{
		int iDestCityLoop;
		CvCity* pDestLoopCity = NULL;
		for(pDestLoopCity = GET_PLAYER(eDestPlayer).firstCity(&iDestCityLoop); pDestLoopCity != NULL; pDestLoopCity = GET_PLAYER(eDestPlayer).nextCity(&iDestCityLoop))
		{
			for (uint uiDomain = 0; uiDomain < NUM_DOMAIN_TYPES; uiDomain++)
			{
				DomainTypes eDomain = (DomainTypes)uiDomain;
				// check only the domain we want
				if (eDomainRestriction != NO_DOMAIN && eDomainRestriction != eDomain)
				{
					continue;
				}

				if (eDomain != DOMAIN_LAND && eDomain != DOMAIN_SEA)
				{
					continue;
				}

				for (uint uiConnectionTypes = 0; uiConnectionTypes < NUM_TRADE_CONNECTION_TYPES; uiConnectionTypes++)
				{
					// Check the route, but not the path
					if (CanCreateTradeRoute(pOriginLoopCity, pDestLoopCity, eDomain, (TradeConnectionType)uiConnectionTypes, false, false))
					{
						// Check the path
						if (pGameTrade->IsValidTradeRoutePath(pOriginLoopCity, pDestLoopCity, eDomain))
							return true;

						// else we can just break out of the loop
						break;
					}
				}
			}
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
bool CvGameTrade::CreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, int& iRouteID)
{
	iRouteID = -1;

	if (!CanCreateTradeRoute(pOriginCity, pDestCity, eDomain, eConnectionType, false))
	{
		return false;
	}

	PlayerTypes eOriginPlayer = pOriginCity->getOwner();
	PlayerTypes eDestPlayer = pDestCity->getOwner();

	int iOriginX = pOriginCity->getX();
	int iOriginY = pOriginCity->getY();
	int iDestX = pDestCity->getX();
	int iDestY = pDestCity->getY();

	bool bSuccess = false;
	CvAStarNode* pPathfinderNode = NULL;
	if (eDomain == DOMAIN_SEA)
	{
		if (pOriginCity->isCoastal(0) && pDestCity->isCoastal(0))	// Both must be on the coast (a lake is ok)  A better check would be to see if they are adjacent to the same water body.
		{
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
			bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
			bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
			pPathfinderNode = GC.GetInternationalTradeRouteWaterFinder().GetLastNode();
		}
	}
	else if (eDomain == DOMAIN_LAND)
	{
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
		pPathfinderNode = GC.GetInternationalTradeRouteLandFinder().GetLastNode();
	}

	if (!bSuccess)
	{
		return false;
	}

	CvAssertMsg(pPathfinderNode, "pPathfinderNode is null. Whaa?");
	if (pPathfinderNode == NULL)
	{
		return false;
	}

	int iNewTradeRouteIndex = GetEmptyTradeRouteIndex();
	CvAssertMsg(iNewTradeRouteIndex < (int)m_aTradeConnections.size(), "iNewTradeRouteIndex out of bounds");

	// couldn't find valid connection, bail out
	if (iNewTradeRouteIndex < 0)
	{
		TradeConnection kNewTradeConnection;
		m_aTradeConnections.push_back(kNewTradeConnection);
		iNewTradeRouteIndex = m_aTradeConnections.size() - 1;
	}

	iRouteID = m_iNextID;
	m_aTradeConnections[iNewTradeRouteIndex].m_iID = m_iNextID;
	m_aTradeConnections[iNewTradeRouteIndex].m_iOriginX = pOriginCity->getX();
	m_aTradeConnections[iNewTradeRouteIndex].m_iOriginY = pOriginCity->getY();
	m_aTradeConnections[iNewTradeRouteIndex].m_eOriginOwner = pOriginCity->getOwner();
	m_aTradeConnections[iNewTradeRouteIndex].m_iDestX = pDestCity->getX();
	m_aTradeConnections[iNewTradeRouteIndex].m_iDestY = pDestCity->getY();
	m_aTradeConnections[iNewTradeRouteIndex].m_eDestOwner = pDestCity->getOwner();
	m_aTradeConnections[iNewTradeRouteIndex].m_eDomain = eDomain;
	m_aTradeConnections[iNewTradeRouteIndex].m_eConnectionType = eConnectionType;
	m_aTradeConnections[iNewTradeRouteIndex].m_unitID = -1;

	// increment m_iNextID for the next connection
	m_iNextID += 1;

	CopyPathIntoTradeConnection(pPathfinderNode, &(m_aTradeConnections[iNewTradeRouteIndex]));

	// reveal all plots to the player who created the trade route
	TeamTypes eOriginTeam = GET_PLAYER(eOriginPlayer).getTeam();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionPlotList::const_iterator it = m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList.begin(); it != m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList.end(); ++it)
	{
		GC.getMap().plot(it->m_iX, it->m_iY)->setRevealed(eOriginTeam, true, true);
#else
	for (uint ui = 0; ui < m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList.size(); ui++)
	{
		GC.getMap().plot(m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList[ui].m_iX, m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList[ui].m_iY)->setRevealed(eOriginTeam, true, true);
#endif
	}

	m_aTradeConnections[iNewTradeRouteIndex].m_iTradeUnitLocationIndex = 0;
	m_aTradeConnections[iNewTradeRouteIndex].m_bTradeUnitMovingForward = true;

	int iRouteSpeed = GET_PLAYER(pOriginCity->getOwner()).GetTrade()->GetTradeRouteSpeed(eDomain);
	int iTurnsPerCircuit = 1;
	if (iRouteSpeed != 0)
	{
		iTurnsPerCircuit = ((m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList.size() - 1) * 2) / iRouteSpeed;
	}
	
	int iTargetTurns = 30; // how many turns do we want the cycle to consume
#ifdef NQ_TRADE_ROUTE_DURATION_SCALES_WITH_GAME_SPEED
	iTargetTurns = iTargetTurns * GC.getGame().getGameSpeedInfo().getCulturePercent() / 100;
#endif
	int iCircuitsToComplete = 1; // how many circuits do we want this trade route to run to reach the target turns
	if (iTurnsPerCircuit != 0)
	{
		iCircuitsToComplete = max(iTargetTurns / iTurnsPerCircuit, 2);
	}

	m_aTradeConnections[iNewTradeRouteIndex].m_iCircuitsCompleted = 0;
	m_aTradeConnections[iNewTradeRouteIndex].m_iCircuitsToComplete = iCircuitsToComplete;
	m_aTradeConnections[iNewTradeRouteIndex].m_iTurnRouteComplete = (iTurnsPerCircuit * iCircuitsToComplete) + GC.getGame().getGameTurn();

	GET_PLAYER(eOriginPlayer).GetTrade()->UpdateTradeConnectionValues();
	if (eDestPlayer != eOriginPlayer)
	{
		GET_PLAYER(eDestPlayer).GetTrade()->UpdateTradeConnectionValues();
	}

	pOriginCity->GetCityReligions()->UpdateNumTradeRouteConnections(pDestCity);
	pDestCity->GetCityReligions()->UpdateNumTradeRouteConnections(pOriginCity);

	CreateVis(iNewTradeRouteIndex);
	MoveUnit(iNewTradeRouteIndex);

	if(GC.getLogging())
	{
		CvString strMsg;
		CvString strDomain;
		switch (eDomain)
		{
		case DOMAIN_LAND:
			strDomain = "land";
			break;
		case DOMAIN_SEA:
			strDomain = "sea";
			break;
		}

		CvString strTRType;
		switch (eConnectionType)
		{
		case TRADE_CONNECTION_FOOD:
			strTRType = "food";
			break;
		case TRADE_CONNECTION_PRODUCTION:
			strTRType = "production";
			break;
		case TRADE_CONNECTION_INTERNATIONAL:
			strTRType = "international";
			break;
		}

		strMsg.Format("%s, New Trade Route, %s, %s, %s, %i, %s", GET_PLAYER(pOriginCity->getOwner()).getCivilizationShortDescription(), strDomain.c_str(), pOriginCity->getName().c_str(), pDestCity->getName().c_str(), m_aTradeConnections[iNewTradeRouteIndex].m_aPlotList.size(), strTRType.c_str());
		LogTradeMsg(strMsg);
	}

	

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsValidTradeRoutePath(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain) const
#else
bool CvGameTrade::IsValidTradeRoutePath (CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain)
#endif
{
	// AI_PERF_FORMAT("Trade-route-perf.csv", ("CvGameTrade::IsValidTradeRoutePath, Turn %03d, %s, %s, %d, %d, %s, %d, %d", GC.getGame().getElapsedGameTurns(), pOriginCity->GetPlayer()->getCivilizationShortDescription(), pOriginCity->getName().c_str(), pOriginCity->getX(), pOriginCity->getY(), pDestCity->getName().c_str(), pDestCity->getX(), pDestCity->getY()) );

	PlayerTypes eOriginPlayer = pOriginCity->getOwner();
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
	PlayerTypes eDestPlayer = pDestCity->getOwner();
#endif

	int iOriginX = pOriginCity->getX();
	int iOriginY = pOriginCity->getY();
	int iDestX = pDestCity->getX();
	int iDestY = pDestCity->getY();

	bool bSuccess = false;
	CvAStarNode* pPathfinderNode = NULL;
	if (eDomain == DOMAIN_SEA)
	{
		if (pOriginCity->isCoastal(0) && pDestCity->isCoastal(0))	// Both must be on the coast (a lake is ok)  A better check would be to see if they are adjacent to the same water body.
		{
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
			bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
			bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
			pPathfinderNode = GC.GetInternationalTradeRouteWaterFinder().GetLastNode();
		}
	}
	else if (eDomain == DOMAIN_LAND)
	{
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
		pPathfinderNode = GC.GetInternationalTradeRouteLandFinder().GetLastNode();
	}

	if (!bSuccess)
	{
		return false;
	}

	CvAssertMsg(pPathfinderNode, "pPathfinderNode is null. Whaa?");
	if (pPathfinderNode == NULL)
	{
		return false;
	}

	// beyond the origin player's trade range
	int iPathDistance = pPathfinderNode->m_iTotalCost;
	int iRange = GET_PLAYER(eOriginPlayer).GetTrade()->GetTradeRouteRange(eDomain, pOriginCity) * 100 + 99; // adding 99 so that any movement penalties are ignored
	if (iPathDistance > iRange)
	{
		return false;
	}

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
CvPlot* CvGameTrade::GetPlotAdjacentToWater(const CvPlot* pTargetLandPlot, const CvPlot* pFromLandPlot) const
#else
CvPlot* CvGameTrade::GetPlotAdjacentToWater (CvPlot* pTargetLandPlot, CvPlot* pFromLandPlot)
#endif
{
	CvPlot* pCoastalPlot = NULL;
	int iBestDistance = MAX_INT;

	// Find a coastal water tile adjacent to enemy city
	for(int iDirectionLoop = 0; iDirectionLoop < NUM_DIRECTION_TYPES; ++iDirectionLoop)
	{
		CvPlot* pAdjacentPlot = plotDirection(pFromLandPlot->getX(), pFromLandPlot->getY(), ((DirectionTypes)iDirectionLoop));
		if(pAdjacentPlot != NULL && pAdjacentPlot->isWater() && pAdjacentPlot->isShallowWater())
		{
			int iDistance = plotDistance(pAdjacentPlot->getX(), pAdjacentPlot->getY(), pTargetLandPlot->getX(), pTargetLandPlot->getY());
			if (iDistance < iBestDistance)
			{
				iBestDistance = iDistance;
				pCoastalPlot = pAdjacentPlot;
			}
		}
	}

	return pCoastalPlot;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsDestinationExclusive(const TradeConnection& kTradeConnection) const
{
	const TradeConnection* pConnection = NULL;
#else
bool CvGameTrade::IsDestinationExclusive(const TradeConnection& kTradeConnection)
{
	TradeConnection* pConnection = NULL;
#endif
#ifdef AUI_ITERATORIZE
	for (pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		pConnection = &(m_aTradeConnections[ui]);
#endif
		if (pConnection->m_iDestX == kTradeConnection.m_iDestX && pConnection->m_iDestY == kTradeConnection.m_iDestY)
		{
			// if we're double counting the same route, ignore
			if (pConnection->m_iOriginX == kTradeConnection.m_iOriginX && pConnection->m_iOriginY == kTradeConnection.m_iOriginY)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsConnectionInternational(const TradeConnection& kConnection) const
#else
bool CvGameTrade::IsConnectionInternational	(const TradeConnection& kConnection)
#endif
{
	TeamTypes eOriginTeam = NO_TEAM;
	TeamTypes eDestTeam = NO_TEAM;

	if (kConnection.m_eOriginOwner != NO_PLAYER)
	{
		eOriginTeam = GET_PLAYER(kConnection.m_eOriginOwner).getTeam();
	}

	if (kConnection.m_eDestOwner != NO_PLAYER)
	{
		eDestTeam = GET_PLAYER(kConnection.m_eDestOwner).getTeam();
	}

	return (eOriginTeam != eDestTeam);
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsCityConnectedToPlayer(const CvCity* pCity, PlayerTypes eOtherPlayer, bool bOnlyOwnedByCityOwner) const
#else
bool CvGameTrade::IsCityConnectedToPlayer (CvCity* pCity, PlayerTypes eOtherPlayer, bool bOnlyOwnedByCityOwner)
#endif
{
	PlayerTypes eCityOwnerPlayer = pCity->getOwner();
	int iCityX = pCity->getX();
	int iCityY = pCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}
		
#ifndef AUI_ITERATORIZE
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif
#endif

		if (pConnection->m_eOriginOwner == eCityOwnerPlayer && pConnection->m_eDestOwner == eOtherPlayer)
		{
			if (pConnection->m_iOriginX == iCityX && pConnection->m_iOriginY == iCityY)
			{
				return true;
			}
		}
		else if (!bOnlyOwnedByCityOwner && pConnection->m_eOriginOwner == eOtherPlayer && pConnection->m_eDestOwner == eCityOwnerPlayer)
		{
			if (pConnection->m_iDestX == iCityX && pConnection->m_iDestY == iCityY)
			{
				return true;
			}
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
void CvGameTrade::CopyPathIntoTradeConnection(const CvAStarNode* pNode, TradeConnection* pTradeConnection) const
#else
void CvGameTrade::CopyPathIntoTradeConnection (CvAStarNode* pNode, TradeConnection* pTradeConnection)
#endif
{
	// beyond the origin player's trade range
#ifndef AUI_TRADE_OPTIMIZE_COPY_PATH_INTO_TRADE_CONNECTION
	int iPathSteps = 0;
#endif
#ifdef AUI_CONSTIFY
	const CvAStarNode* pWalkingPath = pNode;
#else
	CvAStarNode* pWalkingPath = pNode;
#endif
	while (pWalkingPath)
	{
#ifdef AUI_TRADE_OPTIMIZE_COPY_PATH_INTO_TRADE_CONNECTION
		TradeConnectionPlot kTradeConnectionPlot;
		pTradeConnection->m_aPlotList.push_back(kTradeConnectionPlot);
#else
		iPathSteps++;
#endif
		pWalkingPath = pWalkingPath->m_pParent;
	}

#ifdef AUI_TRADE_OPTIMIZE_COPY_PATH_INTO_TRADE_CONNECTION
	int iIndex = pTradeConnection->m_aPlotList.size();
#else
	pTradeConnection->m_aPlotList.clear();
	for (int i = 0; i < iPathSteps; i++)
	{
		TradeConnectionPlot kTradeConnectionPlot;
		pTradeConnection->m_aPlotList.push_back(kTradeConnectionPlot);
	}

	int iIndex = iPathSteps - 1;
#endif
	while (pNode != NULL)
	{
#ifdef AUI_TRADE_OPTIMIZE_COPY_PATH_INTO_TRADE_CONNECTION
		iIndex--;
#endif
		pTradeConnection->m_aPlotList[iIndex].m_iX = pNode->m_iX;
		pTradeConnection->m_aPlotList[iIndex].m_iY = pNode->m_iY;
		pNode = pNode->m_pParent;
#ifndef AUI_TRADE_OPTIMIZE_COPY_PATH_INTO_TRADE_CONNECTION
		iIndex--;
#endif
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetDomainModifierTimes100(DomainTypes eDomain) const
#else
int CvGameTrade::GetDomainModifierTimes100 (DomainTypes eDomain)
#endif
{
	if (eDomain == DOMAIN_SEA)
	{
		return 100;
	}
	else
	{
		return 0;
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsPlayerConnectedToPlayer(PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer) const
#else
bool CvGameTrade::IsPlayerConnectedToPlayer (PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer)
#endif
{
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == eFirstPlayer && pConnection->m_eDestOwner == eSecondPlayer)
		{
			return true;
		}
		else if (pConnection->m_eOriginOwner == eSecondPlayer && pConnection->m_eDestOwner == eFirstPlayer)
		{
			return true;
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::CountNumPlayerConnectionsToPlayer(PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer) const
#else
int CvGameTrade::CountNumPlayerConnectionsToPlayer (PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer)
#endif
{
	int iCount = 0;

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == eFirstPlayer && pConnection->m_eDestOwner == eSecondPlayer)
		{
			iCount++;
		}
		else if (pConnection->m_eOriginOwner == eSecondPlayer && pConnection->m_eDestOwner == eFirstPlayer)
		{
			iCount++;
		}
	}

	return iCount;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsCityConnectedToCity(const CvCity* pFirstCity, const CvCity* pSecondCity) const
#else
bool CvGameTrade::IsCityConnectedToCity (CvCity* pFirstCity, CvCity* pSecondCity)
#endif
{
	int iFirstCityX = pFirstCity->getX();
	int iFirstCityY = pFirstCity->getY();
	int iSecondCityX = pSecondCity->getX();
	int iSecondCityY = pSecondCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_iOriginX == iFirstCityX && pConnection->m_iOriginY == iFirstCityY && pConnection->m_iDestX == iSecondCityX && pConnection->m_iDestY == iSecondCityY)
		{
			return true;
		}
		else if (pConnection->m_iOriginX == iSecondCityX && pConnection->m_iOriginY == iSecondCityY && pConnection->m_iDestX == iFirstCityX && pConnection->m_iDestY == iFirstCityY)
		{
			return true;
		}
	}

	return false;	
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsCityConnectedFromCityToCity(const CvCity* pOriginCity, const CvCity* pDestCity) const
#else
bool CvGameTrade::IsCityConnectedFromCityToCity (CvCity* pOriginCity, CvCity* pDestCity)
#endif
{
	int iFirstCityX = pOriginCity->getX();
	int iFirstCityY = pOriginCity->getY();
	int iSecondCityX = pDestCity->getX();
	int iSecondCityY = pDestCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_iOriginX == iFirstCityX && pConnection->m_iOriginY == iFirstCityY && pConnection->m_iDestX == iSecondCityX && pConnection->m_iDestY == iSecondCityY)
		{
			return true;
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetNumTimesOriginCity(const CvCity* pCity, bool bOnlyInternational) const
#else
int CvGameTrade::GetNumTimesOriginCity (CvCity* pCity, bool bOnlyInternational)
#endif
{
	int iCount = 0;
	int iCityX = pCity->getX();
	int iCityY = pCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_iOriginX == iCityX && pConnection->m_iOriginY == iCityY)
		{
			if (bOnlyInternational)
			{
				if (pConnection->m_eConnectionType == TRADE_CONNECTION_INTERNATIONAL)
				{
					iCount++;
				}
			}
			else
			{
				iCount++;
			}
		}
	}

	return iCount;	
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetNumTimesDestinationCity(const CvCity* pCity, bool bOnlyInternational) const
#else
int CvGameTrade::GetNumTimesDestinationCity (CvCity* pCity, bool bOnlyInternational)
#endif
{
	int iCount = 0;
	int iCityX = pCity->getX();
	int iCityY = pCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(m_aTradeConnections[ui]);
#endif

		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_iDestX == iCityX && pConnection->m_iDestY == iCityY)
		{
			if (bOnlyInternational)
			{
				if (pConnection->m_eConnectionType == TRADE_CONNECTION_INTERNATIONAL)
				{
					iCount++;
				}
			}
			else
			{
				iCount++;
			}
		}
	}

	return iCount;	
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetEmptyTradeRouteIndex() const
#else
int CvGameTrade::GetEmptyTradeRouteIndex (void)
#endif
{
	for (uint i = 0; i < m_aTradeConnections.size(); i++)
	{
		if (IsTradeRouteIndexEmpty(i))
		{
			return i;
		}
	}

	return -1;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsTradeRouteIndexEmpty(int iIndex) const
#else
bool CvGameTrade::IsTradeRouteIndexEmpty(int iIndex)
#endif
{
	if (m_aTradeConnections[iIndex].m_iOriginX == -1 && m_aTradeConnections[iIndex].m_iOriginY == -1 && m_aTradeConnections[iIndex].m_iDestX == -1 && m_aTradeConnections[iIndex].m_iDestY == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	--------------------------------------------------------------------------------
bool CvGameTrade::EmptyTradeRoute(int iIndex)
{
	CvAssertMsg(iIndex >= 0 && iIndex < (int)m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex < 0 || iIndex >= (int)m_aTradeConnections.size())
	{
		return false;
	}

	TradeConnection& kTradeConnection = m_aTradeConnections[iIndex];
	PlayerTypes eOriginPlayer = kTradeConnection.m_eOriginOwner;
	PlayerTypes eDestPlayer = kTradeConnection.m_eDestOwner;

	// Remove any visualization
	if (kTradeConnection.m_unitID != -1)
	{
		CvUnit* pkUnit = GET_PLAYER(kTradeConnection.m_eOriginOwner).getUnit(kTradeConnection.m_unitID);
		if (pkUnit)
		{
			pkUnit->kill(false);
		}
	}

	kTradeConnection.m_iID = -1;
	kTradeConnection.m_iDestX = -1;
	kTradeConnection.m_iDestY = -1;
	kTradeConnection.m_eDestOwner = NO_PLAYER;
	kTradeConnection.m_iOriginX = -1;
	kTradeConnection.m_iOriginY = -1;
	kTradeConnection.m_eOriginOwner = NO_PLAYER;
	kTradeConnection.m_eDomain = NO_DOMAIN;
	kTradeConnection.m_eConnectionType = NUM_TRADE_CONNECTION_TYPES;
#ifdef AUI_WARNING_FIXES
	kTradeConnection.m_iTradeUnitLocationIndex = MAX_UNSIGNED_INT;
#else
	kTradeConnection.m_iTradeUnitLocationIndex = -1;
#endif
	kTradeConnection.m_bTradeUnitMovingForward = false;
	kTradeConnection.m_iCircuitsCompleted = 0;
	kTradeConnection.m_iCircuitsToComplete = 0;
	kTradeConnection.m_iTurnRouteComplete = 0;
	kTradeConnection.m_aPlotList.clear();
	kTradeConnection.m_unitID = -1;

	for (uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
	{
		kTradeConnection.m_aiOriginYields[ui] = 0;
		kTradeConnection.m_aiDestYields[ui] = 0;
	}

	GET_PLAYER(eOriginPlayer).GetTrade()->UpdateTradeConnectionValues();
	GET_PLAYER(eDestPlayer).GetTrade()->UpdateTradeConnectionValues();

	gDLL->TradeVisuals_DestroyRoute(iIndex, eOriginPlayer);
	return true;
}

#ifdef AUI_ITERATORIZE
TradeConnectionList::iterator CvGameTrade::GetEmptyTradeRouteIter()
{
	for (TradeConnectionList::iterator it = m_aTradeConnections.begin(); it != m_aTradeConnections.end(); ++it)
	{
		if (IsTradeRouteIndexEmpty(it))
		{
			return it;
		}
	}

	return NULL;
}

TradeConnectionList::const_iterator CvGameTrade::GetEmptyTradeRouteIter() const
{
	for (TradeConnectionList::const_iterator it = m_aTradeConnections.begin(); it != m_aTradeConnections.end(); ++it)
	{
		if (IsTradeRouteIndexEmpty(it))
		{
			return it;
		}
	}

	return NULL;
}

bool CvGameTrade::IsTradeRouteIndexEmpty(const TradeConnectionList::iterator it) const
{
	return (it == NULL || (it->m_iOriginX == -1 && it->m_iOriginY == -1 && it->m_iDestX == -1 && it->m_iDestY == -1));
}

bool CvGameTrade::IsTradeRouteIndexEmpty(const TradeConnectionList::const_iterator it) const
{
	return (it == NULL || (it->m_iOriginX == -1 && it->m_iOriginY == -1 && it->m_iDestX == -1 && it->m_iDestY == -1));
}
#endif

//	--------------------------------------------------------------------------------
/// Called when a city changes hands
void CvGameTrade::ClearAllCityTradeRoutes (CvPlot* pPlot)
{
	CvAssert(pPlot != NULL);

	if (pPlot)
	{
		int iX = pPlot->getX();
		int iY = pPlot->getY();
		for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
		{
			if (IsTradeRouteIndexEmpty(ui))
			{
				continue;
			}

			bool bMatchesOrigin = (m_aTradeConnections[ui].m_iOriginX == iX && m_aTradeConnections[ui].m_iOriginY == iY);
			bool bMatchesDest = (m_aTradeConnections[ui].m_iDestX == iX && m_aTradeConnections[ui].m_iDestY == iY);

			if (bMatchesOrigin || bMatchesDest)
			{
				// if the destination was wiped, the origin gets a trade unit back
				if (bMatchesDest && GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).isAlive())
				{
					UnitTypes eUnitType = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).GetTrade()->GetTradeUnit(m_aTradeConnections[ui].m_eDomain);
					CvAssertMsg(eUnitType != NO_UNIT, "No trade unit found");
					if (eUnitType != NO_UNIT)
					{
						GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).initUnit(eUnitType, m_aTradeConnections[ui].m_iOriginX, m_aTradeConnections[ui].m_iOriginY, UNITAI_TRADE_UNIT);
					}
				}

				EmptyTradeRoute(ui);
			}		
		}
	}
}

//	--------------------------------------------------------------------------------
//  Reset all Civ to Civ trade routes involving ePlayer.  Trade routes involving city-states are not reset.
void CvGameTrade::ClearAllCivTradeRoutes (PlayerTypes ePlayer)
{
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
		{
			continue;
		}

		bool bMatchesOrigin = (m_aTradeConnections[ui].m_eOriginOwner == ePlayer);
		bool bMatchesDest = (m_aTradeConnections[ui].m_eDestOwner == ePlayer);
		bool bCityStateRoute = (GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).isMinorCiv() || GET_PLAYER(m_aTradeConnections[ui].m_eDestOwner).isMinorCiv());

		if (!bCityStateRoute)
		{
			if (bMatchesOrigin && bMatchesDest)
			{
				// Internal trade route - don't wipe it
				continue;
			}
			else if (bMatchesOrigin || bMatchesDest)
			{
				// if the destination was wiped, the origin gets a trade unit back
				if (GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).isAlive())
				{
					UnitTypes eUnitType = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).GetTrade()->GetTradeUnit(m_aTradeConnections[ui].m_eDomain);
					CvAssertMsg(eUnitType != NO_UNIT, "No trade unit found");
					if (eUnitType != NO_UNIT)
					{
						GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).initUnit(eUnitType, m_aTradeConnections[ui].m_iOriginX, m_aTradeConnections[ui].m_iOriginY, UNITAI_TRADE_UNIT);
					}
				}

				EmptyTradeRoute(ui);
			}
		}
	}
}

//	--------------------------------------------------------------------------------
//  Reset all Civ to City-State trade routes for all players.
void CvGameTrade::ClearAllCityStateTradeRoutes (void)
{
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
		{
			continue;
		}

		bool bMatchesDest = (GET_PLAYER(m_aTradeConnections[ui].m_eDestOwner).isMinorCiv());
		if (bMatchesDest)
		{
			// if the destination was wiped, the origin gets a trade unit back
			if (GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).isAlive())
			{
				UnitTypes eUnitType = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).GetTrade()->GetTradeUnit(m_aTradeConnections[ui].m_eDomain);
				CvAssertMsg(eUnitType != NO_UNIT, "No trade unit found");
				if (eUnitType != NO_UNIT)
				{
					GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).initUnit(eUnitType, m_aTradeConnections[ui].m_iOriginX, m_aTradeConnections[ui].m_iOriginY, UNITAI_TRADE_UNIT);
				}
			}

			EmptyTradeRoute(ui);
		}		
	}
}

//	--------------------------------------------------------------------------------
/// Called when war is declared between teams
#ifdef NQM_TEAM_TRADE_ROUTES_CANCELLED_NOT_DESTROYED_FOR_WAR_DEFENDER_ON_DOW
void CvGameTrade::CancelTradeBetweenTeams(TeamTypes eTeam1, TeamTypes eTeam2, bool bReturnUnits)
#else
void CvGameTrade::CancelTradeBetweenTeams (TeamTypes eTeam1, TeamTypes eTeam2)
#endif
{
	// there shouldn't be trade routes between the same team
	if (eTeam1 == eTeam2)
	{
		return;
	}

	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
		{
			continue;
		}

		TeamTypes eOriginTeam = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).getTeam();
		TeamTypes eDestTeam = GET_PLAYER(m_aTradeConnections[ui].m_eDestOwner).getTeam();
		if ((eOriginTeam == eTeam1 && eDestTeam == eTeam2) || (eOriginTeam == eTeam2 && eDestTeam == eTeam1)) 
		{
#ifdef NQM_TEAM_TRADE_ROUTES_CANCELLED_NOT_DESTROYED_FOR_WAR_DEFENDER_ON_DOW
			if (bReturnUnits)
			{
				UnitTypes eUnitType = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).GetTrade()->GetTradeUnit(m_aTradeConnections[ui].m_eDomain);
				CvAssertMsg(eUnitType != NO_UNIT, "No trade unit found");
				if (eUnitType != NO_UNIT)
				{
					GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).initUnit(eUnitType, m_aTradeConnections[ui].m_iOriginX, m_aTradeConnections[ui].m_iOriginY, UNITAI_TRADE_UNIT);
				}
			}
#endif
			EmptyTradeRoute(ui);
		}
	}
}

#ifdef NQM_TEAM_TRADE_ROUTES_CANCELLED_NOT_DESTROYED_FOR_WAR_DEFENDER_ON_DOW
// This invalides trade routes (i.e. cancels them and sends their units back) instead of destroying them
void CvGameTrade::InvalidateTradeBetweenTeams(TeamTypes eOriginTeam, TeamTypes eDestinationTeam)
{
	// there shouldn't be trade routes between the same team
	if (eOriginTeam == eDestinationTeam)
	{
		return;
	}

	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
		{
			continue;
		}

		TeamTypes eLoopOriginTeam = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).getTeam();
		TeamTypes eLoopDestTeam = GET_PLAYER(m_aTradeConnections[ui].m_eDestOwner).getTeam();
		if (eOriginTeam == eLoopOriginTeam && eLoopDestTeam == eDestinationTeam)
		{
			UnitTypes eUnitType = GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).GetTrade()->GetTradeUnit(m_aTradeConnections[ui].m_eDomain);
			CvAssertMsg(eUnitType != NO_UNIT, "No trade unit found");
			if (eUnitType != NO_UNIT)
			{
				GET_PLAYER(m_aTradeConnections[ui].m_eOriginOwner).initUnit(eUnitType, m_aTradeConnections[ui].m_iOriginX, m_aTradeConnections[ui].m_iOriginY, UNITAI_TRADE_UNIT);
			}
			EmptyTradeRoute(ui);
		}
	}
}
#endif

//	--------------------------------------------------------------------------------
// when war is declared, both sides plunder each others trade routes for cash!
void CvGameTrade::DoAutoWarPlundering(TeamTypes eTeam1, TeamTypes eTeam2)
{
	// walk through each team
	for (uint uiTeam = 0; uiTeam < 2; uiTeam++)
	{
		TeamTypes eTRTeam = NO_TEAM;
		TeamTypes ePlunderTeam = NO_TEAM;
		switch (uiTeam)
		{
		case 0:
			eTRTeam = eTeam1;
			ePlunderTeam = eTeam2;
			break;
		case 1:
			eTRTeam = eTeam2;
			ePlunderTeam = eTeam1;
		}

		// walk through each player on the team
		for (uint uiPlayer = 0; uiPlayer < MAX_MAJOR_CIVS; uiPlayer++)
		{
			PlayerTypes ePlayer = (PlayerTypes)uiPlayer;
			if (GET_PLAYER(ePlayer).getTeam() != eTRTeam)
			{
				continue;
			}
			
			// walk through each of the trade routes
			for (uint uiTradeRoute = 0; uiTradeRoute < m_aTradeConnections.size(); uiTradeRoute++)
			{
				if (IsTradeRouteIndexEmpty(uiTradeRoute))
				{
					continue;
				}

				// if it's not my trade route
				if (m_aTradeConnections[uiTradeRoute].m_eOriginOwner != ePlayer)
				{
					continue;
				}

				// if the destination is not the civ I'm going to war with
				if (GET_PLAYER(m_aTradeConnections[uiTradeRoute].m_eDestOwner).getTeam() != ePlunderTeam)
				{
					continue;
				}

				// get the plot where the trade route is
				int iLocationIndex = m_aTradeConnections[uiTradeRoute].m_iTradeUnitLocationIndex;
				int iPlotX = m_aTradeConnections[uiTradeRoute].m_aPlotList[iLocationIndex].m_iX;
				int iPlotY = m_aTradeConnections[uiTradeRoute].m_aPlotList[iLocationIndex].m_iY;
				CvPlot* pPlot = GC.getMap().plot(iPlotX, iPlotY);

				// see if there is unit from the other team above it
				IDInfo* pUnitNode = pPlot->headUnitNode();
				while(pUnitNode != NULL)
				{
					CvUnit* pLoopUnit = GetPlayerUnit(*pUnitNode);
					if(pLoopUnit)
					{
						if (GET_PLAYER(pLoopUnit->getOwner()).getTeam() == ePlunderTeam)
						{
							if (pLoopUnit->canPlunderTradeRoute(pPlot, true))
							{
								// cheating to get around war!
								GET_PLAYER(pLoopUnit->getOwner()).GetTrade()->PlunderTradeRoute(m_aTradeConnections[uiTradeRoute].m_iID);
								break;
							}
						}
					}

					pUnitNode = pPlot->nextUnitNode(pUnitNode);
				}
			}
		}
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetNumTradeRoutesInPlot(const CvPlot* pPlot) const
#else
int CvGameTrade::GetNumTradeRoutesInPlot (CvPlot* pPlot)
#endif
{
	int iResult = 0;
	int iX = pPlot->getX();
	int iY = pPlot->getY();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

#ifdef AUI_ITERATORIZE
		for (TradeConnectionPlotList::const_iterator it = pConnection->m_aPlotList.begin(); it != pConnection->m_aPlotList.end(); ++it)
		{
			if (it->m_iX == iX && it->m_iY == iY)
#else
		for (uint uiPlot = 0; uiPlot < m_aTradeConnections[ui].m_aPlotList.size(); uiPlot++)
		{
			if (m_aTradeConnections[ui].m_aPlotList[uiPlot].m_iX == iX && m_aTradeConnections[ui].m_aPlotList[uiPlot].m_iY == iY)
#endif
			{
				iResult++;
			}
		}
	}

	return iResult;
}

//	--------------------------------------------------------------------------------
/// GetIndexFromID
#ifdef AUI_CONSTIFY
int CvGameTrade::GetIndexFromID(int iID) const
#else
int CvGameTrade::GetIndexFromID (int iID)
#endif
{
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (m_aTradeConnections[ui].m_iID == iID)
		{
			return ui;
		}
	}

	return -1;
}

//	--------------------------------------------------------------------------------
/// GetOwnerFromID
#ifdef AUI_CONSTIFY
PlayerTypes CvGameTrade::GetOwnerFromID(int iID) const
#else
PlayerTypes CvGameTrade::GetOwnerFromID (int iID)
#endif
{
	int iIndex = GetIndexFromID(iID);
	if (iIndex < -1)
	{
		return NO_PLAYER;
	}

	return m_aTradeConnections[iIndex].m_eOriginOwner;
}

//	--------------------------------------------------------------------------------
/// GetDestFromID
#ifdef AUI_CONSTIFY
PlayerTypes CvGameTrade::GetDestFromID(int iID) const
#else
PlayerTypes CvGameTrade::GetDestFromID (int iID)
#endif
{
	int iIndex = GetIndexFromID(iID);
	if (iIndex < -1)
	{
		return NO_PLAYER;
	}

	return m_aTradeConnections[iIndex].m_eDestOwner;
}

#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
//	--------------------------------------------------------------------------------
/// GetDomainFromID
DomainTypes CvGameTrade::GetDomainFromID (int iID)
{
	int iIndex = GetIndexFromID(iID);
	if (iIndex < -1)
	{
		return NO_DOMAIN;
	}

	return m_aTradeConnections[iIndex].m_eDomain;
}
#endif

//	--------------------------------------------------------------------------------
/// GetIndexFromUnitID
#ifdef AUI_CONSTIFY
int CvGameTrade::GetIndexFromUnitID(int iUnitID, PlayerTypes eOwner) const
#else
int CvGameTrade::GetIndexFromUnitID(int iUnitID, PlayerTypes eOwner)
#endif
{
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
		if (m_aTradeConnections[ui].m_unitID == iUnitID && m_aTradeConnections[ui].m_eOriginOwner == eOwner)
			return ui;
	return -1;
}

//	--------------------------------------------------------------------------------
/// Is this unit id used? This indicates that this unit is automated
#ifdef AUI_CONSTIFY
bool CvGameTrade::IsUnitIDUsed(int iUnitID) const
#else
bool CvGameTrade::IsUnitIDUsed (int iUnitID)
#endif
{
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = m_aTradeConnections.begin(); pConnection != m_aTradeConnections.end(); ++pConnection)
	{
		if (pConnection->m_unitID == iUnitID)
#else
	for (uint ui = 0; ui < m_aTradeConnections.size(); ui++)
	{
		if (m_aTradeConnections[ui].m_unitID == iUnitID)
#endif
		{
			return true;
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
CvCity* CvGameTrade::GetOriginCity(const TradeConnection& kTradeConnection)
{
	CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_iOriginX, kTradeConnection.m_iOriginY);

	CvAssertMsg(pPlot, "CvPlayerTrade - plot is null");
	if (!pPlot)
	{
		return NULL;
	}

	return pPlot->getPlotCity();
}

//	--------------------------------------------------------------------------------
CvCity* CvGameTrade::GetDestCity(const TradeConnection& kTradeConnection)
{
	CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_iDestX, kTradeConnection.m_iDestY);

	CvAssertMsg(pPlot, "CvPlayerTrade - plot is null");
	if (!pPlot)
	{
		return NULL;
	}

	return pPlot->getPlotCity();	
}

//	--------------------------------------------------------------------------------
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
void CvGameTrade::ResetTechDifference(PlayerTypes eForPlayer)
{
	if (eForPlayer == NO_PLAYER)
	{
		for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
		{
			for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
			{
				m_aaiTechDifference[ui][ui2] = -1; // undefined
			}
		}
	}
	else
	{
		for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
		{
			m_aaiTechDifference[eForPlayer][ui2] = -1; // undefined
#else
void CvGameTrade::ResetTechDifference ()
{
	for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
	{
		for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
		{
			m_aaiTechDifference[ui][ui2] = -1; // undefined
#endif
		}
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
void CvGameTrade::BuildTechDifference(PlayerTypes eForPlayer)
#else
void CvGameTrade::BuildTechDifference ()
#endif
{
	if(GC.getGame().isOption(GAMEOPTION_NO_SCIENCE))
	{
		return;
	}

	// for each major civ
#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	if (eForPlayer == NO_PLAYER)
	{
		for (uint uiPlayer1 = 0; uiPlayer1 < MAX_MAJOR_CIVS; uiPlayer1++)
		{
			PlayerTypes ePlayer1 = (PlayerTypes)uiPlayer1;
			TeamTypes eTeam1 = GET_PLAYER(ePlayer1).getTeam();

			for (uint uiPlayer2 = 0; uiPlayer2 < MAX_MAJOR_CIVS; uiPlayer2++)
			{
				PlayerTypes ePlayer2 = (PlayerTypes)uiPlayer2;
				TeamTypes eTeam2 = GET_PLAYER(ePlayer2).getTeam();

				if (eTeam1 == eTeam2)
				{
					m_aaiTechDifference[uiPlayer1][uiPlayer2] = 0;
				}
				else if (!GET_PLAYER(ePlayer1).isAlive() || !GET_PLAYER(ePlayer2).isAlive())
				{
					m_aaiTechDifference[uiPlayer1][uiPlayer2] = 0;
				}
				else
				{
					int iTechDifference = 0;

					CvPlayerTechs* pPlayerTechs = GET_PLAYER(ePlayer1).GetPlayerTechs();
					for (uint iTechLoop = 0; iTechLoop < pPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
					{
						TechTypes eTech = (TechTypes)iTechLoop;
						bool bPlayer1Knows = GET_TEAM(eTeam1).GetTeamTechs()->HasTech(eTech);
						bool bPlayer2Knows = GET_TEAM(eTeam2).GetTeamTechs()->HasTech(eTech);
						if (bPlayer2Knows && !bPlayer1Knows)
						{
							iTechDifference++;
						}
					}

					m_aaiTechDifference[uiPlayer1][uiPlayer2] = iTechDifference;
				}
			}
		}
	}
	else
	{
		TeamTypes eTeam1 = GET_PLAYER(eForPlayer).getTeam();

		for (uint uiPlayer2 = 0; uiPlayer2 < MAX_MAJOR_CIVS; uiPlayer2++)
		{
			PlayerTypes ePlayer2 = (PlayerTypes)uiPlayer2;
			TeamTypes eTeam2 = GET_PLAYER(ePlayer2).getTeam();

			if (eTeam1 == eTeam2)
			{
				m_aaiTechDifference[eForPlayer][uiPlayer2] = 0;
			}
			else if (!GET_PLAYER(eForPlayer).isAlive() || !GET_PLAYER(ePlayer2).isAlive())
			{
				m_aaiTechDifference[eForPlayer][uiPlayer2] = 0;
			}
			else
			{
				int iTechDifference = 0;

				CvPlayerTechs* pPlayerTechs = GET_PLAYER(eForPlayer).GetPlayerTechs();
				for (uint iTechLoop = 0; iTechLoop < pPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
				{
					TechTypes eTech = (TechTypes)iTechLoop;
					bool bPlayer1Knows = GET_TEAM(eTeam1).GetTeamTechs()->HasTech(eTech);
					bool bPlayer2Knows = GET_TEAM(eTeam2).GetTeamTechs()->HasTech(eTech);
					if (bPlayer2Knows && !bPlayer1Knows)
					{
						iTechDifference++;
					}
				}

				m_aaiTechDifference[eForPlayer][uiPlayer2] = iTechDifference;
			}
		}
	}
#else
	for (uint uiPlayer1 = 0; uiPlayer1 < MAX_MAJOR_CIVS; uiPlayer1++)
	{
		PlayerTypes ePlayer1 = (PlayerTypes)uiPlayer1;
		TeamTypes eTeam1 = GET_PLAYER(ePlayer1).getTeam();

		for (uint uiPlayer2 = 0; uiPlayer2 < MAX_MAJOR_CIVS; uiPlayer2++)
		{	
			PlayerTypes ePlayer2 = (PlayerTypes)uiPlayer2;
			TeamTypes eTeam2 = GET_PLAYER(ePlayer2).getTeam();

			if (eTeam1 == eTeam2) 
			{
				m_aaiTechDifference[uiPlayer1][uiPlayer2] = 0;
			}
			else if (!GET_PLAYER(ePlayer1).isAlive() || !GET_PLAYER(ePlayer2).isAlive())
			{
				m_aaiTechDifference[uiPlayer1][uiPlayer2] = 0;
			}
			else
			{
				int iTechDifference = 0;
				
				CvPlayerTechs* pPlayerTechs = GET_PLAYER(ePlayer1).GetPlayerTechs();
#ifdef AUI_WARNING_FIXES
				for (uint iTechLoop = 0; iTechLoop < pPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
#else
				for(int iTechLoop = 0; iTechLoop < pPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
#endif
				{
					TechTypes eTech = (TechTypes)iTechLoop;
					bool bPlayer1Knows = GET_TEAM(eTeam1).GetTeamTechs()->HasTech(eTech);
					bool bPlayer2Knows = GET_TEAM(eTeam2).GetTeamTechs()->HasTech(eTech);
					if (bPlayer2Knows && !bPlayer1Knows)
					{
						iTechDifference++;
					}
				}

				m_aaiTechDifference[uiPlayer1][uiPlayer2] = iTechDifference;
			}
		}
	}
#endif
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvGameTrade::GetTechDifference(PlayerTypes ePlayer, PlayerTypes ePlayer2) const
#else
int CvGameTrade::GetTechDifference (PlayerTypes ePlayer, PlayerTypes ePlayer2)
#endif
{
	if(GC.getGame().isOption(GAMEOPTION_NO_SCIENCE))
	{
		return 0;
	}

	if (GET_PLAYER(ePlayer).isMinorCiv() || GET_PLAYER(ePlayer).isBarbarian())
	{
		return 0;
	}

	if (GET_PLAYER(ePlayer2).isMinorCiv() || GET_PLAYER(ePlayer2).isBarbarian())
	{
		return 0;
	}

	return m_aaiTechDifference[ePlayer][ePlayer2];
}

//	--------------------------------------------------------------------------------
/// move a trade unit along its path for all its movement points
bool CvGameTrade::MoveUnit (int iIndex) 
{
	CvAssertMsg(iIndex >= 0 && iIndex < (int)m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex < 0 || iIndex >= (int)m_aTradeConnections.size())
	{
		return false;
	}

	TradeConnection &kTradeConnection = m_aTradeConnections[iIndex];
	int iMoves = GET_PLAYER(kTradeConnection.m_eOriginOwner).GetTrade()->GetTradeRouteSpeed(kTradeConnection.m_eDomain);
	for (int i = 0; i < iMoves; i++)
	{
		if (kTradeConnection.m_iCircuitsCompleted >= kTradeConnection.m_iCircuitsToComplete)
		{
			break;
		}

		bool bAbleToStep = StepUnit(iIndex);
		if (!bAbleToStep)
		{
			break;
		}
	}

	gDLL->TradeVisuals_UpdateRouteDirection(iIndex, kTradeConnection.m_bTradeUnitMovingForward);

	// Send a NULL plot move to say we are complete to the vis unit.
	CvUnit *pkUnit = GetVis(iIndex);
	if (pkUnit)
	{
		pkUnit->UnitMove(NULL, false, NULL);
		pkUnit->setMoves(0);
	}

	return true;
}

//	--------------------------------------------------------------------------------
/// move a trade unit a single step along its path (called by MoveUnit)
bool CvGameTrade::StepUnit (int iIndex)
{
	CvAssertMsg(iIndex >= 0 && iIndex < (int)m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex < 0 || iIndex >= (int)m_aTradeConnections.size())
	{
		return false;
	}

	// if the unit needs to turn around
	TradeConnection &kTradeConnection = m_aTradeConnections[iIndex];
#ifdef AUI_WARNING_FIXES
	bool bAtEndGoingForward = (kTradeConnection.m_bTradeUnitMovingForward && kTradeConnection.m_iTradeUnitLocationIndex + 1 >= kTradeConnection.m_aPlotList.size());
#else
	bool bAtEndGoingForward = (kTradeConnection.m_bTradeUnitMovingForward && kTradeConnection.m_iTradeUnitLocationIndex >= ((int)kTradeConnection.m_aPlotList.size() - 1));
#endif
	bool bAtEndGoingBackward = (!kTradeConnection.m_bTradeUnitMovingForward && kTradeConnection.m_iTradeUnitLocationIndex <= 0);
	if (bAtEndGoingForward || bAtEndGoingBackward)
	{
		kTradeConnection.m_bTradeUnitMovingForward = !kTradeConnection.m_bTradeUnitMovingForward;
	}

	// if moving forward
	if (kTradeConnection.m_bTradeUnitMovingForward)
	{
		kTradeConnection.m_iTradeUnitLocationIndex += 1;
	}
	else // if moving back
	{
		kTradeConnection.m_iTradeUnitLocationIndex -= 1;
		if (kTradeConnection.m_iTradeUnitLocationIndex == 0)
		{
			kTradeConnection.m_iCircuitsCompleted += 1;
		}
	}

	// Move the visualization
	CvUnit *pkUnit = GetVis(iIndex);
	if (pkUnit)
	{
		pkUnit->setXY(kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iX, kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iY, true, false, true, true);
	}

	// auto-pillage when a trade unit moves under an enemy unit
	CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iX, kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iY);
	if (pPlot)
	{
		CvUnit* pEnemyUnit = pPlot->getVisibleEnemyDefender(kTradeConnection.m_eOriginOwner);
		if (pEnemyUnit)
		{
			if (pEnemyUnit->canPlunderTradeRoute(pPlot, false))
			{
				GET_PLAYER(pEnemyUnit->getOwner()).GetTrade()->PlunderTradeRoute(kTradeConnection.m_iID);
				// done died!
				return false;
			}
		}
	}

	return true;
}

//	----------------------------------------------------------------------------
void CvGameTrade::CreateVis(int iIndex)
{
	CvAssertMsg(iIndex >= 0 && iIndex < (int)m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex < 0 || iIndex >= (int)m_aTradeConnections.size())
	{
		return;
	}

	TradeConnection& kTradeConnection = m_aTradeConnections[iIndex];

	if (kTradeConnection.m_unitID == -1)
	{
		UnitTypes eUnitType = CvPlayerTrade::GetTradeUnit(kTradeConnection.m_eDomain);
		if (eUnitType != NO_UNIT)
		{
			CvPlayer& kPlayer = GET_PLAYER(kTradeConnection.m_eOriginOwner);
			CvUnit* pkUnit = kPlayer.initUnit(eUnitType, kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iX, kTradeConnection.m_aPlotList[kTradeConnection.m_iTradeUnitLocationIndex].m_iY, NO_UNITAI, NO_DIRECTION, false, true, TRADE_UNIT_MAP_LAYER);
			if (pkUnit)
			{
				pkUnit->SetAutomateType(AUTOMATE_TRADE);
				kTradeConnection.m_unitID = pkUnit->GetID();
			}
		}
	}
}

//	----------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
CvUnit* CvGameTrade::GetVis(uint iIndex) const
{
	CvAssertMsg(iIndex < m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex >= m_aTradeConnections.size())
#else
CvUnit* CvGameTrade::GetVis(int iIndex)
{
	CvAssertMsg(iIndex >= 0 && iIndex < (int)m_aTradeConnections.size(), "iIndex out of bounds");
	if (iIndex < 0 || iIndex >= (int)m_aTradeConnections.size())
#endif
	{
		return NULL;
	}

#ifdef AUI_WARNING_FIXES
	const TradeConnection& kTradeConnection = m_aTradeConnections[iIndex];
#else
	TradeConnection& kTradeConnection = m_aTradeConnections[iIndex];
#endif
	if (kTradeConnection.m_unitID != -1)
	{
#ifdef AUI_WARNING_FIXES
		return GET_PLAYER(kTradeConnection.m_eOriginOwner).getUnit(kTradeConnection.m_unitID);
#else
		CvPlayer& kPlayer = GET_PLAYER(kTradeConnection.m_eOriginOwner);
		return kPlayer.getUnit(kTradeConnection.m_unitID);
#endif
	}

	return NULL;
}

//	----------------------------------------------------------------------------
void CvGameTrade::DisplayTemporaryPopupTradeRoute(int iDestX, int iDestY, TradeConnectionType type, DomainTypes eDomain)
{
	int i,n;
	int plotsX[MAX_PLOTS_TO_DISPLAY], plotsY[MAX_PLOTS_TO_DISPLAY];

	int iOriginX,iOriginY;
	PlayerTypes eOriginPlayer;
	bool bSuccess = false;
	CvAStarNode* pPathfinderNode = NULL;
	TradeConnection tempTradeConnection;

	{
		auto_ptr<ICvUnit1> pSelectedUnit(GC.GetEngineUserInterface()->GetHeadSelectedUnit());
		CvUnit* pkSelectedUnit = GC.UnwrapUnitPointer(pSelectedUnit.get());
		CvAssert(pkSelectedUnit != NULL);
		if (pkSelectedUnit)
		{
			iOriginX = pkSelectedUnit->getX();
			iOriginY = pkSelectedUnit->getY();
			eOriginPlayer = pkSelectedUnit->getOwner();
		}
		else
		{
			return;
		}
	}

#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
	PlayerTypes eDestPlayer = NO_PLAYER;
	CvPlot* pDestPlot = GC.getMap().plot(iDestX, iDestY);
	if (pDestPlot)
	{
		CvCity* pDestCity = pDestPlot->getPlotCity();
		if (pDestCity)
		{
			eDestPlayer = pDestCity->getOwner();
		}
	}
#endif

	switch (eDomain)
	{
	case DOMAIN_LAND:
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
		bSuccess = GC.GetInternationalTradeRouteLandFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
		pPathfinderNode = GC.GetInternationalTradeRouteLandFinder().GetLastNode();
		break;
	case DOMAIN_SEA:
#ifdef AUI_ASTAR_TRADE_ROUTE_COST_TILE_OWNERSHIP_PREFS
		bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer + (eDestPlayer << 8), false);
#else
		bSuccess = GC.GetInternationalTradeRouteWaterFinder().GeneratePath(iOriginX, iOriginY, iDestX, iDestY, eOriginPlayer, false);
#endif
		pPathfinderNode = GC.GetInternationalTradeRouteWaterFinder().GetLastNode();
		break;
	}

	gDLL->TradeVisuals_DestroyRoute(TEMPORARY_POPUPROUTE_ID,GC.getGame().getActivePlayer());
	if (bSuccess && pPathfinderNode != NULL) {
		CopyPathIntoTradeConnection(pPathfinderNode, &tempTradeConnection);
		n = tempTradeConnection.m_aPlotList.size();
		if (n>0 && n <=MAX_PLOTS_TO_DISPLAY) {
			for (i=0;i<n;++i) {
				plotsX[i] = tempTradeConnection.m_aPlotList[i].m_iX;
				plotsY[i] = tempTradeConnection.m_aPlotList[i].m_iY;
			}
			gDLL->TradeVisuals_NewRoute(TEMPORARY_POPUPROUTE_ID,eOriginPlayer,type,n,plotsX,plotsY);
			gDLL->TradeVisuals_ActivatePopupRoute(TEMPORARY_POPUPROUTE_ID);
		}
	}
	m_CurrentTemporaryPopupRoute.iPlotX = iDestX;
	m_CurrentTemporaryPopupRoute.iPlotY = iDestY;
	m_CurrentTemporaryPopupRoute.type = type;
}

//	----------------------------------------------------------------------------
void CvGameTrade::HideTemporaryPopupTradeRoute(int iPlotX, int iPlotY, TradeConnectionType type)
{
	if (iPlotX == m_CurrentTemporaryPopupRoute.iPlotX &&
		iPlotY == m_CurrentTemporaryPopupRoute.iPlotY &&
		type == m_CurrentTemporaryPopupRoute.type) 
	{
		gDLL->TradeVisuals_DeactivatePopupRoute();
		gDLL->TradeVisuals_DestroyRoute(TEMPORARY_POPUPROUTE_ID,GC.getGame().getActivePlayer());
	}
}

//	----------------------------------------------------------------------------
CvString CvGameTrade::GetLogFileName() const
{
	CvString strLogName;
	strLogName = "TradeRouteLog.csv";
	return strLogName;
}

//	----------------------------------------------------------------------------
void CvGameTrade::LogTradeMsg(CvString& strMsg)
{
	if(GC.getLogging())
	{
		CvString strOutBuf;
		CvString strBaseString;
		FILogFile* pLog;

		pLog = LOGFILEMGR.GetLog(GetLogFileName(), FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		strBaseString.Format("%03d,", GC.getGame().getElapsedGameTurns());
		strOutBuf = strBaseString + strMsg;
		pLog->Msg(strOutBuf);
	}
}

//	----------------------------------------------------------------------------
/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvGameTrade& writeTo)
{
	int plotsX[MAX_PLOTS_TO_DISPLAY];
	int plotsY[MAX_PLOTS_TO_DISPLAY];
	int nPlots;

	uint uiVersion;
	loadFrom >> uiVersion;

	int iNum = 0;
	loadFrom >> iNum;
	for (int i = 0; i < iNum; i++)
	{
		TradeConnection kTradeConnection;
		writeTo.m_aTradeConnections.push_back(kTradeConnection);
		if (uiVersion >= 1)
		{
			loadFrom >> writeTo.m_aTradeConnections[i].m_iID;
		}
		else
		{
			writeTo.m_aTradeConnections[i].m_iID = MAX_INT;
		}

		loadFrom >> writeTo.m_aTradeConnections[i].m_iOriginX;
		loadFrom >> writeTo.m_aTradeConnections[i].m_iOriginY;
		loadFrom >> writeTo.m_aTradeConnections[i].m_iDestX;
		loadFrom >> writeTo.m_aTradeConnections[i].m_iDestY;
		loadFrom >> writeTo.m_aTradeConnections[i].m_eOriginOwner;
		loadFrom >> writeTo.m_aTradeConnections[i].m_eDestOwner;
		int iDomain;
		loadFrom >> iDomain;
		writeTo.m_aTradeConnections[i].m_eDomain = (DomainTypes)iDomain;
		int iConnectionType;
		loadFrom >> iConnectionType;
		writeTo.m_aTradeConnections[i].m_eConnectionType = (TradeConnectionType)iConnectionType;

		loadFrom >> writeTo.m_aTradeConnections[i].m_iTradeUnitLocationIndex;
		loadFrom >> writeTo.m_aTradeConnections[i].m_bTradeUnitMovingForward;

		loadFrom >> writeTo.m_aTradeConnections[i].m_unitID;

		loadFrom >> writeTo.m_aTradeConnections[i].m_iCircuitsCompleted;
		loadFrom >> writeTo.m_aTradeConnections[i].m_iCircuitsToComplete;

		if (uiVersion >= 2)
		{
			loadFrom >> writeTo.m_aTradeConnections[i].m_iTurnRouteComplete;
		}
		else
		{
			writeTo.m_aTradeConnections[i].m_iTurnRouteComplete = 0;
		}

		int iNum2 = 0;
		loadFrom >> iNum2;

		for (int i2 = 0; i2 < iNum2; i2++)
		{
			TradeConnectionPlot kTradeConnectionPlot;
			writeTo.m_aTradeConnections[i].m_aPlotList.push_back(kTradeConnectionPlot);
			loadFrom >> writeTo.m_aTradeConnections[i].m_aPlotList[i2].m_iX;
			loadFrom >> writeTo.m_aTradeConnections[i].m_aPlotList[i2].m_iY;
			
			if (i2 < MAX_PLOTS_TO_DISPLAY) {
				plotsX[i2] = writeTo.m_aTradeConnections[i].m_aPlotList[i2].m_iX;
				plotsY[i2] = writeTo.m_aTradeConnections[i].m_aPlotList[i2].m_iY;
			}
		}

		nPlots = iNum2;
		if (nPlots > 0)
		{
			if (nPlots > MAX_PLOTS_TO_DISPLAY)
				nPlots = MAX_PLOTS_TO_DISPLAY;
			gDLL->TradeVisuals_NewRoute(i,writeTo.m_aTradeConnections[i].m_eOriginOwner,writeTo.m_aTradeConnections[i].m_eConnectionType,nPlots,plotsX,plotsY);
			gDLL->TradeVisuals_UpdateRouteDirection(i, writeTo.m_aTradeConnections[i].m_bTradeUnitMovingForward);
		}

		for (uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
		{
			loadFrom >> writeTo.m_aTradeConnections[i].m_aiOriginYields[ui];
			loadFrom >> writeTo.m_aTradeConnections[i].m_aiDestYields[ui];
		}
	}

	if (uiVersion >= 3)
	{
		for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
		{
			for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
			{
				loadFrom >> writeTo.m_aaiTechDifference[ui][ui2];
			}
		}
	}
	else
	{
		for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
		{
			for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
			{
				writeTo.m_aaiTechDifference[ui][ui2] = 0;
			}
		}
	}

	loadFrom >> writeTo.m_iNextID;

	return loadFrom;
}

//	--------------------------------------------------------------------------------
FDataStream& operator<<(FDataStream& saveTo, const CvGameTrade& readFrom)
{
	uint uiVersion = 3;
	saveTo << uiVersion;

	saveTo << readFrom.m_aTradeConnections.size();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator it = readFrom.m_aTradeConnections.begin(); it != readFrom.m_aTradeConnections.end(); ++it)
	{
		saveTo << it->m_iID;

		saveTo << it->m_iOriginX;
		saveTo << it->m_iOriginY;
		saveTo << it->m_iDestX;
		saveTo << it->m_iDestY;
		saveTo << it->m_eOriginOwner;
		saveTo << it->m_eDestOwner;
		saveTo << (int)it->m_eDomain;
		saveTo << (int)it->m_eConnectionType;
		saveTo << it->m_iTradeUnitLocationIndex;
		saveTo << it->m_bTradeUnitMovingForward;
		saveTo << it->m_unitID;
		saveTo << it->m_iCircuitsCompleted;
		saveTo << it->m_iCircuitsToComplete;
		saveTo << it->m_iTurnRouteComplete;

		saveTo << it->m_aPlotList.size();
		for (TradeConnectionPlotList::const_iterator it2 = it->m_aPlotList.begin(); it2 != it->m_aPlotList.end(); ++it2)
		{
			saveTo << it2->m_iX;
			saveTo << it2->m_iY;
		}

		for (uint ui = 0; ui < NUM_YIELD_TYPES; ui++)
		{
			saveTo << it->m_aiOriginYields[ui];
			saveTo << it->m_aiDestYields[ui];
#else
	for (uint ui = 0; ui < readFrom.m_aTradeConnections.size(); ui++)
	{
		saveTo << readFrom.m_aTradeConnections[ui].m_iID;

		saveTo << readFrom.m_aTradeConnections[ui].m_iOriginX;
		saveTo << readFrom.m_aTradeConnections[ui].m_iOriginY;
		saveTo << readFrom.m_aTradeConnections[ui].m_iDestX;
		saveTo << readFrom.m_aTradeConnections[ui].m_iDestY;
		saveTo << readFrom.m_aTradeConnections[ui].m_eOriginOwner;
		saveTo << readFrom.m_aTradeConnections[ui].m_eDestOwner;
		saveTo << (int)readFrom.m_aTradeConnections[ui].m_eDomain;
		saveTo << (int)readFrom.m_aTradeConnections[ui].m_eConnectionType;
		saveTo << readFrom.m_aTradeConnections[ui].m_iTradeUnitLocationIndex;
		saveTo << readFrom.m_aTradeConnections[ui].m_bTradeUnitMovingForward;
		saveTo << readFrom.m_aTradeConnections[ui].m_unitID;
		saveTo << readFrom.m_aTradeConnections[ui].m_iCircuitsCompleted;
		saveTo << readFrom.m_aTradeConnections[ui].m_iCircuitsToComplete;
		saveTo << readFrom.m_aTradeConnections[ui].m_iTurnRouteComplete;

		saveTo << readFrom.m_aTradeConnections[ui].m_aPlotList.size();
		for (uint ui2 = 0; ui2 < readFrom.m_aTradeConnections[ui].m_aPlotList.size(); ui2++)
		{
			saveTo << readFrom.m_aTradeConnections[ui].m_aPlotList[ui2].m_iX;
			saveTo << readFrom.m_aTradeConnections[ui].m_aPlotList[ui2].m_iY;
		}

		for (uint ui2 = 0; ui2 < NUM_YIELD_TYPES; ui2++)
		{
			saveTo << readFrom.m_aTradeConnections[ui].m_aiOriginYields[ui2];
			saveTo << readFrom.m_aTradeConnections[ui].m_aiDestYields[ui2];
#endif
		}
	}

	for (uint ui = 0; ui < MAX_MAJOR_CIVS; ui++)
	{
		for (uint ui2 = 0; ui2 < MAX_MAJOR_CIVS; ui2++)
		{
			saveTo << readFrom.m_aaiTechDifference[ui][ui2];
		}
	}

	saveTo << readFrom.m_iNextID;

	return saveTo;
}

//=====================================
// CvPlayerTrade
//=====================================
//	--------------------------------------------------------------------------------
/// Default Constructor
CvPlayerTrade::CvPlayerTrade()
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Destructor
CvPlayerTrade::~CvPlayerTrade(void)
{
	Reset();
}

//	--------------------------------------------------------------------------------
/// Init
void CvPlayerTrade::Init(CvPlayer* pPlayer)
{
	Reset();
	m_pPlayer = pPlayer;
}

//	--------------------------------------------------------------------------------
/// Uninit
void CvPlayerTrade::Uninit(void)
{
	Reset();
}

//	--------------------------------------------------------------------------------
void CvPlayerTrade::Reset(void)
{
	m_pPlayer = NULL;
	m_aRecentlyExpiredConnections.clear();
	m_aTradeConnectionWasPlundered.clear();
}

//	--------------------------------------------------------------------------------
/// DoTurn
void CvPlayerTrade::DoTurn(void)
{
	m_aRecentlyExpiredConnections.clear();
	UpdateTradeConnectionValues();
	UpdateTradeConnectionWasPlundered();
	MoveUnits();
}

//	--------------------------------------------------------------------------------
/// MoveUnits
void CvPlayerTrade::MoveUnits (void)
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
		TradeConnection* pTradeConnection = &(pTrade->m_aTradeConnections[ui]);
		if (pTradeConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			pTrade->MoveUnit(ui);

			// check to see if the trade route is still active but the circuit is completed
			if (!pTrade->IsTradeRouteIndexEmpty(ui) && pTradeConnection->m_iCircuitsCompleted >= pTradeConnection->m_iCircuitsToComplete)
			{
				m_aRecentlyExpiredConnections.push_back(*pTradeConnection);

				// get data before we wipe the trade route
				int iOriginX = pTradeConnection->m_iOriginX;
				int iOriginY = pTradeConnection->m_iOriginY;
				DomainTypes eDomain = pTradeConnection->m_eDomain;

				// wipe trade route
				pTrade->EmptyTradeRoute(ui);
				
				// create new unit
				UnitTypes eUnitType = GetTradeUnit(eDomain);
#ifdef CVASSERT_ENABLE
				CvUnit* pRebornUnit = m_pPlayer->initUnit(eUnitType, iOriginX, iOriginY, UNITAI_TRADE_UNIT);

				DEBUG_VARIABLE(pRebornUnit);
				CvAssertMsg(pRebornUnit, "pRebornUnit is null. This is bad!!");
#else
				m_pPlayer->initUnit(eUnitType, iOriginX, iOriginY, UNITAI_TRADE_UNIT);
#endif
			}
		}
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionBaseValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionBaseValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	if (bAsOriginPlayer)
	{
		if (GC.getGame().GetGameTrade()->IsConnectionInternational(kTradeConnection))
		{
			if (eYield == YIELD_GOLD)
			{
				int iResult = 0;
				int iBase = GC.getINTERNATIONAL_TRADE_BASE();
				iResult = iBase;
				return iResult;
			}
			else if (eYield == YIELD_SCIENCE)
			{
				int iTechDifference = GC.getGame().GetGameTrade()->GetTechDifference(kTradeConnection.m_eOriginOwner, kTradeConnection.m_eDestOwner);
				int iAdjustedTechDifference = 0;
				if (iTechDifference > 0)
				{
					int iCeilTechDifference = (int)ceil(iTechDifference / 2.0f);
					iAdjustedTechDifference = max(iCeilTechDifference, 1);
				}

				// Cultural influence bump
				int iInfluenceBoost = GET_PLAYER(kTradeConnection.m_eOriginOwner).GetCulture()->GetInfluenceTradeRouteScienceBonus(kTradeConnection.m_eDestOwner);
				iAdjustedTechDifference += iInfluenceBoost;

				return iAdjustedTechDifference * 100;
			}
		}
	}
	else
	{
		if (eYield == YIELD_SCIENCE)
		{
			int iTechDifference = GC.getGame().GetGameTrade()->GetTechDifference(kTradeConnection.m_eDestOwner, kTradeConnection.m_eOriginOwner);
			int iAdjustedTechDifference = 0;
			if (iTechDifference > 0)
			{
				int iCeilTechDifference = (int)ceil(iTechDifference / 2.0f);
				iAdjustedTechDifference = max(iCeilTechDifference, 1);
			}

			return  iAdjustedTechDifference * 100;
		}
		else
		{
			return 100;
		}
	}

	return 0;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionGPTValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer, bool bOriginCity) const
#else
int CvPlayerTrade::GetTradeConnectionGPTValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer, bool bOriginCity)
#endif
{
	if (bAsOriginPlayer)
	{
		if (GC.getGame().GetGameTrade()->IsConnectionInternational(kTradeConnection))
		{
			if (eYield == YIELD_GOLD)
			{
				int iX, iY;
				if (bOriginCity)
				{
					iX = kTradeConnection.m_iOriginX;
					iY = kTradeConnection.m_iOriginY;
				}
				else
				{
					iX = kTradeConnection.m_iDestX;
					iY = kTradeConnection.m_iDestY;
				}

				CvPlot* pPlot = GC.getMap().plot(iX, iY);

				CvAssertMsg(pPlot, "CvPlayerTrade - plot is null");
				if (!pPlot)
				{
					return 0;
				}

				CvCity* pCity = pPlot->getPlotCity();
				CvAssertMsg(pCity, "CvPlayerTrade - pCity is null");	
				if (!pCity)
				{
					return 0;
				}

				int iDivisor = GC.getINTERNATIONAL_TRADE_CITY_GPT_DIVISOR();
				if (iDivisor == 0)
				{
					iDivisor = 1;
				}
				return pCity->getYieldRateTimes100(eYield, true) / iDivisor;
			}
		}
	}
	else
	{
		return 0;
	}

	return 0;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionResourceValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionResourceValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	if (bAsOriginPlayer)
	{
		if (GC.getGame().GetGameTrade()->IsConnectionInternational(kTradeConnection))
		{
			if (eYield == YIELD_GOLD)
			{
				CvPlot* pOriginPlot = GC.getMap().plot(kTradeConnection.m_iOriginX, kTradeConnection.m_iOriginY);
				CvPlot* pDestPlot = GC.getMap().plot(kTradeConnection.m_iDestX, kTradeConnection.m_iDestY);

				CvAssertMsg(pOriginPlot && pDestPlot, "pOriginPlot or pDestPlot are null");
				if (!pOriginPlot || !pDestPlot)
				{
					return 0;
				}

				CvCity* pOriginCity = pOriginPlot->getPlotCity();
				CvCity* pDestCity = pDestPlot->getPlotCity();

				CvAssertMsg(pOriginCity && pDestCity, "pOriginCity or pDestCity are null");
				if (!pOriginCity || !pDestCity)
				{
					return 0;
				}

				int iValue = 0;
#ifdef AUI_WARNING_FIXES
				for (uint i = 0; i < GC.getNumResourceInfos(); i++)
#else
				for(int i = 0; i < GC.getNumResourceInfos(); i++)
#endif
				{
					ResourceTypes eResource = (ResourceTypes)i;
					const CvResourceInfo* pkResourceInfo = GC.getResourceInfo(eResource);
					if (pkResourceInfo)
					{
						if (pkResourceInfo->getResourceUsage() == RESOURCEUSAGE_LUXURY || pkResourceInfo->getResourceUsage() == RESOURCEUSAGE_STRATEGIC)
						{
							if (pOriginCity->IsHasResourceLocal(eResource, false) != pDestCity->IsHasResourceLocal(eResource, false))
							{
								iValue += 50;
							}
						}
					}
				}

				int iModifer = 100 + GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerTraits()->GetTradeRouteResourceModifier();
				iValue *= iModifer;
				iValue /= 100;

				return iValue;
			}
		}
	}
	else
	{
		return 0;	
	}

	return 0;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionYourBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionYourBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	// this only applies to international trade routes, so otherwise, buzz off!
	if (!GC.getGame().GetGameTrade()->IsConnectionInternational(kTradeConnection))
	{
		return 0;
	}
	
	if (eYield != YIELD_GOLD)
	{
		return 0;
	}

	CvPlot* pOriginPlot = GC.getMap().plot(kTradeConnection.m_iOriginX, kTradeConnection.m_iOriginY);
	CvPlot* pDestPlot = GC.getMap().plot(kTradeConnection.m_iDestX, kTradeConnection.m_iDestY);

	CvAssertMsg(pOriginPlot && pDestPlot, "pOriginPlot or pDestPlot are null");
	if (!pOriginPlot || !pDestPlot)
	{
		return 0;
	}

	CvCity* pDestCity = pDestPlot->getPlotCity();
	if (!pDestCity)
	{
		return 0;
	}

	int iBonus = 0;
	if (bAsOriginPlayer)
	{
		CvCity* pOriginCity = CvGameTrade::GetOriginCity(kTradeConnection);
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
		{
			BuildingTypes eBuilding = (BuildingTypes)GET_PLAYER(pOriginCity->getOwner()).getCivilizationInfo().getCivilizationBuildings(iI);
			if(eBuilding != NO_BUILDING)
			{
				CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);
				if (!pBuildingEntry)
				{
					continue;
				}

				if (pBuildingEntry && pOriginCity->GetCityBuildings()->GetNumBuilding((BuildingTypes)pBuildingEntry->GetID()))
				{
					if (pBuildingEntry->GetTradeRouteSeaGoldBonus() > 0 && kTradeConnection.m_eDomain == DOMAIN_SEA)
					{
						iBonus += pBuildingEntry->GetTradeRouteSeaGoldBonus();
					}
					else if (pBuildingEntry->GetTradeRouteLandGoldBonus() > 0 && kTradeConnection.m_eDomain == DOMAIN_LAND)
					{
						iBonus += pBuildingEntry->GetTradeRouteLandGoldBonus();
					}
				}
			}
		}
	}

	if (bAsOriginPlayer)
	{
		iBonus *= (100 + GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerTraits()->GetTradeBuildingModifier());
		iBonus /= 100;
	}
	else
	{
		iBonus *= (100 + GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerTraits()->GetTradeBuildingModifier());
		iBonus /= 100;
	}	

	return iBonus;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionTheirBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionTheirBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	// this only applies to international trade routes, so otherwise, buzz off!
	if (!GC.getGame().GetGameTrade()->IsConnectionInternational(kTradeConnection))
	{
		return 0;
	}

	if (eYield != YIELD_GOLD)
	{
		return 0;
	}

	CvPlot* pOriginPlot = GC.getMap().plot(kTradeConnection.m_iOriginX, kTradeConnection.m_iOriginY);
	CvPlot* pDestPlot = GC.getMap().plot(kTradeConnection.m_iDestX, kTradeConnection.m_iDestY);

	CvAssertMsg(pOriginPlot && pDestPlot, "pOriginPlot or pDestPlot are null");
	if (!pOriginPlot || !pDestPlot)
	{
		return 0;
	}

	CvCity* pDestCity = pDestPlot->getPlotCity();
	if (!pDestCity)
	{
		return 0;
	}

	int iBonus = 0;

	// minor civs should not get bonuses for others trading with them
	if (!GET_PLAYER(kTradeConnection.m_eDestOwner).isMinorCiv() && !GET_PLAYER(kTradeConnection.m_eOriginOwner).isMinorCiv())
	{
		if (kTradeConnection.m_eOriginOwner == m_pPlayer->GetID())
		{
			iBonus += pDestCity->GetTradeRouteTargetBonus() * 100;
		}

		if (kTradeConnection.m_eDestOwner == m_pPlayer->GetID())
		{
			iBonus += pDestCity->GetTradeRouteRecipientBonus() * 100;
		}
	}

	if (bAsOriginPlayer)
	{
		iBonus *= (100 + GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerTraits()->GetTradeBuildingModifier());
		iBonus /= 100;
	}
	else
	{
		iBonus *= (100 + GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerTraits()->GetTradeBuildingModifier());
		iBonus /= 100;
	}	

	return iBonus;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionExclusiveValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const
#else
int CvPlayerTrade::GetTradeConnectionExclusiveValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield)
#endif
{
	// unnecessary code to make it compile for now
	if (eYield != NO_YIELD)
	{
		eYield = eYield;
	}

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	if (pTrade->IsDestinationExclusive(kTradeConnection))
	{
		return GC.getINTERNATIONAL_TRADE_EXCLUSIVE_CONNECTION();
	}
	else
	{
		return 0;
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionPolicyValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const
#else
int CvPlayerTrade::GetTradeConnectionPolicyValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield)
#endif
{
	// unnecessary code to make it compile for now
	if (eYield != NO_YIELD)
	{
		eYield = eYield;
	}

	int iValue = 0;
	if (kTradeConnection.m_eConnectionType == TRADE_CONNECTION_INTERNATIONAL)
	{
		// domain type bonuses
		if (kTradeConnection.m_eDomain == DOMAIN_LAND 
			&& eYield == YIELD_GOLD) // NQMP GJS - Merchant Confederacy
		{
			iValue += GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_LAND_TRADE_GOLD_CHANGE);
		}
		else if (kTradeConnection.m_eDomain == DOMAIN_SEA
			&& eYield == YIELD_GOLD) // NQMP GJS - Merchant Confederacy
		{
			iValue += GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_SEA_TRADE_GOLD_CHANGE);
		}

		// policy tree bonuses
		CvPlayerPolicies* pOwnerPlayerPolicies = GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerPolicies();
		CvPlayerPolicies* pDestPlayerPolicies = GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies();

		PolicyBranchTypes eFreedom = (PolicyBranchTypes)GC.getPOLICY_BRANCH_FREEDOM();
		PolicyBranchTypes eAutocracy = (PolicyBranchTypes)GC.getPOLICY_BRANCH_AUTOCRACY();
		PolicyBranchTypes eOrder = (PolicyBranchTypes)GC.getPOLICY_BRANCH_ORDER();

		bool bBothAutocracy = false;
		bool bBothOrder = false;
		bool bBothFreedom = false;
		if (eAutocracy != NO_POLICY_BRANCH_TYPE)
		{
			bBothAutocracy = pOwnerPlayerPolicies->IsPolicyBranchUnlocked(eAutocracy) && pDestPlayerPolicies->IsPolicyBranchUnlocked(eAutocracy);
		}
		if (eOrder != NO_POLICY_BRANCH_TYPE)
		{
			bBothOrder = pOwnerPlayerPolicies->IsPolicyBranchUnlocked(eOrder) && pDestPlayerPolicies->IsPolicyBranchUnlocked(eOrder);
		}
		if (eFreedom != NO_POLICY_BRANCH_TYPE)
		{
			bBothFreedom = pOwnerPlayerPolicies->IsPolicyBranchUnlocked(eFreedom) && pDestPlayerPolicies->IsPolicyBranchUnlocked(eFreedom);
		}
	
		if ((bBothAutocracy || bBothOrder || bBothFreedom)
			&& eYield == YIELD_GOLD) // NQMP GJS - Merchant Confederacy
		{
			iValue += GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_SHARED_IDEOLOGY_TRADE_CHANGE) * 100;
		}

		// city state bonus
		if (GET_PLAYER(kTradeConnection.m_eDestOwner).isMinorCiv())
		{
			iValue += GET_PLAYER(kTradeConnection.m_eOriginOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_CITY_STATE_TRADE_CHANGE);
		}
	}

	return iValue;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionOtherTraitValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionOtherTraitValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	int iValue = 0;
	if (kTradeConnection.m_eConnectionType == TRADE_CONNECTION_INTERNATIONAL)
	{
		if (bAsOriginPlayer)
		{
			iValue += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerTraits()->GetYieldChangeIncomingTradeRoute(eYield) * 100;
		}
	}

	return iValue;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionDomainValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const
#else
int CvPlayerTrade::GetTradeConnectionDomainValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield)
#endif
{
	// unnecessary code to make it compile for now
	if (eYield != NO_YIELD)
	{
		eYield = eYield;
	}

	return GC.getGame().GetGameTrade()->GetDomainModifierTimes100(kTradeConnection.m_eDomain);
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionRiverValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionRiverValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	// unnecessary code to make it compile for now
	if (eYield != NO_YIELD)
	{
		eYield = eYield;
	}

	int iModifier = 0;
	if (eYield == YIELD_GOLD && kTradeConnection.m_eDomain == DOMAIN_LAND)
	{
		if (bAsOriginPlayer)
		{
			CvCity* pOriginCity = CvGameTrade::GetOriginCity(kTradeConnection);
			CvAssert(pOriginCity != NULL);
			if (pOriginCity)
			{
				CvPlot* pOriginCityPlot = pOriginCity->plot();
				if (pOriginCityPlot->isRiver())
				{
					iModifier = 25;
				}
			}
		}
		else
		{
			CvCity* pDestCity = CvGameTrade::GetDestCity(kTradeConnection);
			CvAssert(pDestCity != NULL);
			if (pDestCity)
			{
				CvPlot* pDestCityPlot = pDestCity->plot();
				if (pDestCityPlot->isRiver())
				{
					iModifier = 25;
				}
			}
		}
	}

	return iModifier;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeConnectionValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const
#else
int CvPlayerTrade::GetTradeConnectionValueTimes100 (const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer)
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iValue = 0;

	if (bAsOriginPlayer)
	{
		if (pTrade->IsConnectionInternational(kTradeConnection))
		{
			
			switch (eYield)
			{
			// NQMP GJS - New Merchant Confederacy begin
			case YIELD_PRODUCTION:
			case YIELD_FOOD:
			//case YIELD_CULTURE: // this doesn't work yet with JONSCulture stuff, figure it out later
				iValue = GetTradeConnectionPolicyValueTimes100(kTradeConnection, eYield);
				break;
			// NQMP GJS - New Merchant Confederacy end
			case YIELD_GOLD:
				{
					int iBaseValue = GetTradeConnectionBaseValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
					int iOriginPerTurnBonus = GetTradeConnectionGPTValueTimes100(kTradeConnection, eYield, bAsOriginPlayer, true);
					int iDestPerTurnBonus = GetTradeConnectionGPTValueTimes100(kTradeConnection, eYield, bAsOriginPlayer, false);
					int iResourceBonus = GetTradeConnectionResourceValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
					int iExclusiveBonus = GetTradeConnectionExclusiveValueTimes100(kTradeConnection, eYield);
					int iPolicyBonus = GetTradeConnectionPolicyValueTimes100(kTradeConnection, eYield);
					int iYourBuildingBonus = GetTradeConnectionYourBuildingValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
					int iTheirBuildingBonus = GetTradeConnectionTheirBuildingValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
					int iTraitBonus = GetTradeConnectionOtherTraitValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);

					int iModifier = 100;
					int iDomainModifier = GetTradeConnectionDomainValueModifierTimes100(kTradeConnection, eYield);
#ifdef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
					iValue *= iDomainModifier + 100;
					iValue /= 100;
#endif
					int iOriginRiverModifier = GetTradeConnectionRiverValueModifierTimes100(kTradeConnection, eYield, bAsOriginPlayer);

					iValue = iBaseValue;
					iValue += iOriginPerTurnBonus;
					iValue += iDestPerTurnBonus;
					iValue += iExclusiveBonus;
					iValue += iYourBuildingBonus;
					iValue += iTheirBuildingBonus;
					iValue += iResourceBonus;
					iValue += iPolicyBonus;
					iValue += iTraitBonus;

#ifndef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
					iModifier += iDomainModifier;
#endif
					iModifier += iOriginRiverModifier;

					iValue *= iModifier;
					iValue /= 100;
					iValue = max(100, iValue);
				}
				break;
			case YIELD_SCIENCE:
				int iBaseValue = GetTradeConnectionBaseValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);

				iValue = iBaseValue;

				int iModifier = 100;
				
				iValue *= iModifier;
				iValue /= 100;
				break;
			}
		}
	}
	else
	{
		if (pTrade->IsConnectionInternational(kTradeConnection))
		{
			if (kTradeConnection.m_eDestOwner == m_pPlayer->GetID())
			{
				switch (eYield)
				{
				case YIELD_GOLD:
					{
						int iBaseValue = GetTradeConnectionBaseValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
						int iYourBuildingBonus = GetTradeConnectionYourBuildingValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);
						int iTheirBuildingBonus = GetTradeConnectionTheirBuildingValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);

						int iModifier = 100;
						int iDomainModifier = GetTradeConnectionDomainValueModifierTimes100(kTradeConnection, eYield);
#ifdef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
						iValue *= iDomainModifier + 100;
						iValue /= 100;
#endif
						int iDestRiverModifier = GetTradeConnectionRiverValueModifierTimes100(kTradeConnection, eYield, false);
						int iTraitBonus = GetTradeConnectionOtherTraitValueTimes100(kTradeConnection, eYield, false);

						iValue = iBaseValue;
						iValue += iYourBuildingBonus;
						iValue += iTheirBuildingBonus;
						iValue += iTraitBonus;

#ifndef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
						iModifier += iDomainModifier;
#endif
						iModifier += iDestRiverModifier;

						iValue *= iModifier;
						iValue /= 100;
					}
					break;
				case YIELD_SCIENCE:
					{
						int iBaseValue = GetTradeConnectionBaseValueTimes100(kTradeConnection, eYield, bAsOriginPlayer);

						int iModifier = 100;

						iValue = iBaseValue;

						iValue *= iModifier;
						iValue /= 100;						
					}
					break;
				}
			}
		}
		else
		{
			// NQMP GJS - Silk Road begin
			if (eYield == YIELD_GOLD)
			{
				iValue = GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_GOLD_CHANGE);
			}
			// NQMP GJS - Silk Road end
			switch (kTradeConnection.m_eConnectionType)
			{
			case TRADE_CONNECTION_FOOD:
				if (eYield == YIELD_FOOD)
				{
					iValue = 300;
					iValue += GC.getEraInfo(GET_PLAYER(kTradeConnection.m_eDestOwner).GetCurrentEra())->getTradeRouteFoodBonusTimes100();
					iValue *= GC.getEraInfo(GC.getGame().getStartEra())->getGrowthPercent();
					iValue /= 100;

					int iModifier = 100;
					int iDomainModifier = GetTradeConnectionDomainValueModifierTimes100(kTradeConnection, eYield);
#ifdef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
					iValue *= iDomainModifier + 100;
					iValue /= 100;
#else
					iModifier += iDomainModifier;
#endif
					iModifier += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_MODIFIER);
					iValue *= iModifier;
					iValue /= 100;
#ifdef FRUITY_TRADITION_LANDED_ELITE
					iValue += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_FOOD_YIELD_CHANGE);
#endif
				}

#ifdef NQ_INTERNAL_TRADE_ROUTE_PRODUCTION_YIELD_CHANGE_FROM_POLICIES
				if (eYield == YIELD_PRODUCTION)
				{
					iValue += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_PRODUCTION_YIELD_CHANGE);
				}
#endif

				break;
			case TRADE_CONNECTION_PRODUCTION:
				if (eYield == YIELD_PRODUCTION)
				{
					iValue = 300;
					iValue += GC.getEraInfo(GET_PLAYER(kTradeConnection.m_eDestOwner).GetCurrentEra())->getTradeRouteProductionBonusTimes100();
					iValue *= (GC.getEraInfo(GC.getGame().getStartEra())->getConstructPercent() + GC.getEraInfo(GC.getGame().getStartEra())->getTrainPercent()) / 2;
					iValue /= 100;

					int iModifier = 100;
					int iDomainModifier = GetTradeConnectionDomainValueModifierTimes100(kTradeConnection, eYield);
#ifdef AUI_TRADE_FIX_CONNECTION_VALUE_MULTIPLICATIVE_STACKING_DOMAIN_MODIFIERS
					iValue *= iDomainModifier + 100;
					iValue /= 100;
#else
					iModifier += iDomainModifier;
#endif
					iModifier += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_MODIFIER);
					iValue *= iModifier;
					iValue /= 100;
#ifdef NQ_INTERNAL_TRADE_ROUTE_PRODUCTION_YIELD_CHANGE_FROM_POLICIES
					iValue += GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_PRODUCTION_YIELD_CHANGE);
#endif
				}
#ifdef FRUITY_TRADITION_LANDED_ELITE
				if (eYield == YIELD_FOOD)
				{
					iValue = GET_PLAYER(kTradeConnection.m_eDestOwner).GetPlayerPolicies()->GetNumericModifier(POLICYMOD_INTERNAL_TRADE_FOOD_YIELD_CHANGE);
				}
#endif

				break;
			}
		}
	}

	return iValue;	
}

//	--------------------------------------------------------------------------------
void CvPlayerTrade::UpdateTradeConnectionValues (void)
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);

		if (pTrade->IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			for (uint uiYields = 0; uiYields < NUM_YIELD_TYPES; uiYields++)
			{
				pConnection->m_aiOriginYields[uiYields] = GetTradeConnectionValueTimes100(*pConnection, (YieldTypes)uiYields, true);
			}
		}

		if (pConnection->m_eDestOwner == m_pPlayer->GetID())
		{
			for (uint uiYields = 0; uiYields < NUM_YIELD_TYPES; uiYields++)
			{
				pConnection->m_aiDestYields[uiYields] = GetTradeConnectionValueTimes100(*pConnection, (YieldTypes)uiYields, false);
			}
		}
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeValuesAtCityTimes100(const CvCity *const pCity, YieldTypes eYield) const
#else
int CvPlayerTrade::GetTradeValuesAtCityTimes100 (const CvCity *const pCity, YieldTypes eYield)
#endif
{
	int iResult = 0;
	int iCityX = pCity->getX();
	int iCityY = pCity->getY();

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#endif

		if (pTrade->IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			if (pConnection->m_iOriginX == iCityX && pConnection->m_iOriginY == iCityY)
			{
				iResult += pConnection->m_aiOriginYields[eYield];
			}
		}
		
		if (pConnection->m_eDestOwner == m_pPlayer->GetID())
		{
			if (pConnection->m_iDestX == iCityX && pConnection->m_iDestY == iCityY)
			{
				iResult += pConnection->m_aiDestYields[eYield];
			}
		}
	}

	return iResult;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetAllTradeValueTimes100(YieldTypes eYield) const
#else
int CvPlayerTrade::GetAllTradeValueTimes100 (YieldTypes eYield)
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iTotal = 0;
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#endif

		if (pTrade->IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			iTotal += pConnection->m_aiOriginYields[eYield];
		}

		if (pConnection->m_eDestOwner == m_pPlayer->GetID())
		{
			iTotal += pConnection->m_aiDestYields[eYield];
		}
	}

	return iTotal;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetAllTradeValueFromPlayerTimes100(YieldTypes eYield, PlayerTypes ePlayer) const
#else
int CvPlayerTrade::GetAllTradeValueFromPlayerTimes100 (YieldTypes eYield, PlayerTypes ePlayer)
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iTotal = 0;
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#endif

		if (pTrade->IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

		if (pConnection->m_eOriginOwner == m_pPlayer->GetID() && pConnection->m_eDestOwner == ePlayer)
		{
			iTotal += pConnection->m_aiOriginYields[eYield];
		}

		if (pConnection->m_eDestOwner == m_pPlayer->GetID() && pConnection->m_eOriginOwner == ePlayer)
		{
			iTotal += pConnection->m_aiDestYields[eYield];
		}
	}

	return iTotal;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::IsConnectedToPlayer(PlayerTypes eOtherPlayer) const
#else
bool CvPlayerTrade::IsConnectedToPlayer(PlayerTypes eOtherPlayer)
#endif
{
	return GC.getGame().GetGameTrade()->IsPlayerConnectedToPlayer(m_pPlayer->GetID(), eOtherPlayer);
}


//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::CanCreateTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath /* = true */) const
#else
bool CvPlayerTrade::CanCreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath /* = true */)
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();

	// if you can't see the plot, you're not allowed to connect it
	if (!pDestCity->plot()->isRevealed(m_pPlayer->getTeam(), false))
	{
		return false;
	}

	if (!m_pPlayer->IsAllowedToTradeWith(pDestCity->getOwner()))
	{
		return false;
	}

	if (pTrade->CanCreateTradeRoute(pOriginCity, pDestCity, eDomain, eConnectionType, bIgnoreExisting, bCheckPath))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::CanCreateTradeRoute(PlayerTypes eOtherPlayer, DomainTypes eDomain) const
#else
bool CvPlayerTrade::CanCreateTradeRoute(PlayerTypes eOtherPlayer, DomainTypes eDomain)
#endif
{
	CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();

	int iCityLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iCityLoop))
	{
		int iCityLoop2;
		CvCity* pLoopCity2;
		for (pLoopCity2 = GET_PLAYER(eOtherPlayer).firstCity(&iCityLoop2); pLoopCity2 != NULL; pLoopCity2 = GET_PLAYER(eOtherPlayer).nextCity(&iCityLoop2))
		{
			for (uint uiConnectionTypes = 0; uiConnectionTypes < NUM_TRADE_CONNECTION_TYPES; uiConnectionTypes++)
			{
				// Check the route, but not the path
				if (CanCreateTradeRoute(pLoopCity, pLoopCity2, eDomain, (TradeConnectionType)uiConnectionTypes, false, false))
				{
					// Check the path
					if (pGameTrade->IsValidTradeRoutePath(pLoopCity, pLoopCity2, eDomain))
						return true;

					// else we can just break out of the loop
					break;
				}
			}
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
//	Can the player create any trade routes in the domain, even with itself.
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::CanCreateTradeRoute(DomainTypes eDomain) const
#else
bool CvPlayerTrade::CanCreateTradeRoute(DomainTypes eDomain)
#endif
{
	CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();

	int iCityLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iCityLoop))
	{
		// Get a sorted list of nearby cities 
		const CvCityManager::CityList& kNearbyCities = CvCityManager::GetNearbyCities(pLoopCity);
		for (CvCityManager::CityList::const_iterator itr = kNearbyCities.begin(); itr != kNearbyCities.end(); ++itr)
		{
			CvCity* pLoopCity2 = (*itr);
			for (uint uiConnectionTypes = 0; uiConnectionTypes < NUM_TRADE_CONNECTION_TYPES; uiConnectionTypes++)
			{
				// Check the route, but not the path
				if (CanCreateTradeRoute(pLoopCity, pLoopCity2, eDomain, (TradeConnectionType)uiConnectionTypes, false, false))
				{
					// Check the path
					if (pGameTrade->IsValidTradeRoutePath(pLoopCity, pLoopCity2, eDomain))
						return true;

					// else we can just break out of the loop
					break;
				}
			}
		}
	}

	return false;

}

//	--------------------------------------------------------------------------------
bool CvPlayerTrade::CreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType)
{
	int plotsX[MAX_PLOTS_TO_DISPLAY], plotsY[MAX_PLOTS_TO_DISPLAY];

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iRouteID = -1;
	bool bResult = pTrade->CreateTradeRoute(pOriginCity, pDestCity, eDomain, eConnectionType, iRouteID);
	if (!bResult)
	{
		return false;
	}

	int iRouteIndex = pTrade->GetIndexFromID(iRouteID);
	if (iRouteIndex != -1)
	{
		int nPlots = pTrade->m_aTradeConnections[iRouteIndex].m_aPlotList.size();
		if (nPlots > 0) {
			if (nPlots > MAX_PLOTS_TO_DISPLAY)
				nPlots = MAX_PLOTS_TO_DISPLAY;
			for (uint ui = 0; ui < (uint)nPlots; ui++) 
			{
				plotsX[ui] = pTrade->m_aTradeConnections[iRouteIndex].m_aPlotList[ui].m_iX;
				plotsY[ui] = pTrade->m_aTradeConnections[iRouteIndex].m_aPlotList[ui].m_iY;
			}
			gDLL->TradeVisuals_NewRoute(iRouteIndex, m_pPlayer->GetID(),pTrade->m_aTradeConnections[iRouteIndex].m_eConnectionType, nPlots, plotsX, plotsY);
			gDLL->TradeVisuals_UpdateRouteDirection(iRouteIndex, pTrade->m_aTradeConnections[iRouteIndex].m_bTradeUnitMovingForward);
		}
	}

	if (m_pPlayer->isHuman() && !GC.getGame().isGameMultiPlayer())
	{
		bool bConnectedToArabs = false;
		bool bConnectedToPersia = false;
		bool bConnectedToMongols = false;
		bool bConnectedToChina = false;
		bool bConnectedToIndia = false;

#ifdef AUI_ITERATORIZE
		for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
		{
			if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
		CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();
		for (uint ui = 0; ui < pGameTrade->m_aTradeConnections.size(); ui++)
		{
			if (pGameTrade->IsTradeRouteIndexEmpty(ui))
#endif
			{
				continue;
			}

#ifdef AUI_ITERATORIZE
			TradeConnection kConnection = *pConnection;
#else
			TradeConnection kConnection = pGameTrade->m_aTradeConnections[ui];
#endif
			if (kConnection.m_eOriginOwner != m_pPlayer->GetID())
			{
				continue;
			}

			if (kConnection.m_eDomain != DOMAIN_LAND)
			{
				continue;
			}

			if (strcmp(GET_PLAYER(kConnection.m_eDestOwner).getCivilizationTypeKey(), "CIVILIZATION_ARABIA") == 0)
			{
				bConnectedToArabs = true;
			}
			else if (strcmp(GET_PLAYER(kConnection.m_eDestOwner).getCivilizationTypeKey(), "CIVILIZATION_PERSIA") == 0)
			{
				bConnectedToPersia = true;
			}
			else if (strcmp(GET_PLAYER(kConnection.m_eDestOwner).getCivilizationTypeKey(), "CIVILIZATION_MONGOL") == 0)
			{
				bConnectedToMongols = true;
			}
			else if (strcmp(GET_PLAYER(kConnection.m_eDestOwner).getCivilizationTypeKey(), "CIVILIZATION_CHINA") == 0)
			{
				bConnectedToChina = true;
			}
			else if (strcmp(GET_PLAYER(kConnection.m_eDestOwner).getCivilizationTypeKey(), "CIVILIZATION_INDIA") == 0)
			{
				bConnectedToIndia = true;
			}
		}

		if (bConnectedToArabs && bConnectedToPersia && bConnectedToMongols && bConnectedToChina && bConnectedToIndia)
		{
			gDLL->UnlockAchievement(ACHIEVEMENT_XP2_29);
		}
	}

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
TradeConnection* CvPlayerTrade::GetTradeConnection(const CvCity* pOriginCity, const CvCity* pDestCity)
#else
TradeConnection* CvPlayerTrade::GetTradeConnection(CvCity* pOriginCity, CvCity* pDestCity)
#endif
{
	int iOriginX = pOriginCity->getX();
	int iOriginY = pOriginCity->getY();
	int iDestX = pDestCity->getX();
	int iDestY = pDestCity->getY();

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#endif
		if (pConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			if (pConnection->m_iOriginX == iOriginX && pConnection->m_iOriginY == iOriginY && pConnection->m_iDestX == iDestX && pConnection->m_iDestY == iDestY)
			{
				return pConnection;
			}
		}
	}

	return NULL;
}

//Returns the number of city state trade routes connected to a city
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetNumberOfCityStateTradeRoutes() const
#else
int CvPlayerTrade::GetNumberOfCityStateTradeRoutes()
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iNumConnections = 0;
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[ui]);
#endif
#endif

		if (pConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			if(GET_PLAYER(pConnection->m_eDestOwner).isMinorCiv())
			{
				iNumConnections++;
			}
		}
	}

	return iNumConnections;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::IsPreviousTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType) const
#else
bool CvPlayerTrade::IsPreviousTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType)
#endif
{
	int iOriginCityX = pOriginCity->getX();
	int iOriginCityY = pOriginCity->getY();
	int iDestCityX = pDestCity->getX();
	int iDestCityY = pDestCity->getY();

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator it = m_aRecentlyExpiredConnections.begin(); it != m_aRecentlyExpiredConnections.end(); ++it)
	{
		if (it->m_iOriginX == iOriginCityX &&
			it->m_iOriginY == iOriginCityY &&
			it->m_iDestX == iDestCityX &&
			it->m_iDestY == iDestCityY &&
			it->m_eDomain == eDomain &&
			it->m_eConnectionType == eConnectionType)
#else
	for (uint ui = 0; ui < m_aRecentlyExpiredConnections.size(); ui++)
	{
		if (m_aRecentlyExpiredConnections[ui].m_iOriginX == iOriginCityX &&
			m_aRecentlyExpiredConnections[ui].m_iOriginY == iOriginCityY &&
			m_aRecentlyExpiredConnections[ui].m_iDestX == iDestCityX &&
			m_aRecentlyExpiredConnections[ui].m_iDestY == iDestCityY &&
			m_aRecentlyExpiredConnections[ui].m_eDomain == eDomain &&
			m_aRecentlyExpiredConnections[ui].m_eConnectionType == eConnectionType) 
#endif
		{
			return true;
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetNumPotentialConnections(const CvCity* pFromCity, DomainTypes eDomain) const
#else
int CvPlayerTrade::GetNumPotentialConnections (CvCity* pFromCity, DomainTypes eDomain)
#endif
{
	CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();

	int iNumValid = 0;
	for (uint ui = 0; ui < MAX_CIV_PLAYERS; ui++)
	{
		PlayerTypes ePlayer = (PlayerTypes)ui;

		if (!GET_PLAYER(ePlayer).isAlive())
		{
			continue;
		}

		if (GET_PLAYER(ePlayer).isBarbarian())
		{
			continue;
		}

		int iLoop;
		CvCity* pLoopCity;
		for(pLoopCity = GET_PLAYER(ePlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iLoop))
		{
			bool bCheckPath = true;
			for (uint uiConnectionTypes = 0; uiConnectionTypes < NUM_TRADE_CONNECTION_TYPES; uiConnectionTypes++)
			{
				// Check the trade route, ignoring the path for now
				if (CanCreateTradeRoute(pFromCity, pLoopCity, eDomain, (TradeConnectionType)uiConnectionTypes, false, false))
				{
					// Now test the path
					if (bCheckPath)
					{
						bool bTradeAvailable = pGameTrade->IsValidTradeRoutePath(pFromCity, pLoopCity, eDomain);
						if (!bTradeAvailable)
							break;		// If there is no path for this domain, just skip the rest of the connection tests.

						bCheckPath = false;		// No need to check the path for this domain again
					}

					iNumValid++;
				}
			}
		}
	}

	return iNumValid;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<int> CvPlayerTrade::GetTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar) const
#else
std::vector<int> CvPlayerTrade::GetTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar)
#endif
{
	std::vector<int> aiTradeConnectionIDs;

	if (pPlot == NULL)
	{
		return aiTradeConnectionIDs;
	}

	int iX = pPlot->getX();
	int iY = pPlot->getY();

	TeamTypes eMyTeam = m_pPlayer->getTeam();

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint uiConnection = 0; uiConnection < pTrade->m_aTradeConnections.size(); uiConnection++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#endif
		if (pTrade->IsTradeRouteIndexEmpty(uiConnection))
#endif
		{
			continue;
		}

		TeamTypes eOtherTeam = GET_PLAYER(pConnection->m_eOriginOwner).getTeam();

		bool bIgnore = false;
		if (bExcludingMe && eOtherTeam == eMyTeam)
		{
			bIgnore = true;
		}

		if (bOnlyWar && !GET_TEAM(eMyTeam).isAtWar(eOtherTeam))
		{
			bIgnore = true;
		}

		if (bIgnore)
		{
			continue;
		}

		if (pConnection->m_aPlotList[pConnection->m_iTradeUnitLocationIndex].m_iX == iX && pConnection->m_aPlotList[pConnection->m_iTradeUnitLocationIndex].m_iY == iY)
		{
			aiTradeConnectionIDs.push_back(pConnection->m_iID);
			if (bFailAtFirstFound)
			{
				break;
			}
		}
	}

	return aiTradeConnectionIDs;	
}


//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<int> CvPlayerTrade::GetTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar) const
#else
std::vector<int> CvPlayerTrade::GetTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar)
#endif
{
	std::vector<int> aiTradeConnectionIDs;

	if (pPlot == NULL)
	{
		return aiTradeConnectionIDs;
	}

	int iX = pPlot->getX();
	int iY = pPlot->getY();

	TeamTypes eMyTeam = m_pPlayer->getTeam();

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint uiConnection = 0; uiConnection < pTrade->m_aTradeConnections.size(); uiConnection++)
	{
#ifdef AUI_CONSTIFY
		const TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#else
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#endif
		if (pTrade->IsTradeRouteIndexEmpty(uiConnection))
#endif
		{
			continue;
		}

		TeamTypes eOtherTeam = GET_PLAYER(pConnection->m_eOriginOwner).getTeam();

		bool bIgnore = false;
		if (bExcludingMe && eOtherTeam == eMyTeam)
		{
			bIgnore = true;
		}

		if (bOnlyWar && !GET_TEAM(eMyTeam).isAtWar(eOtherTeam))
		{
			bIgnore = true;
		}

		if (bIgnore)
		{
			continue;
		}

#ifdef AUI_ITERATORIZE
		for (TradeConnectionPlotList::const_iterator it = pConnection->m_aPlotList.begin(); it != pConnection->m_aPlotList.end(); ++it)
		{
			if (it->m_iX == iX && it->m_iY == iY)
#else
		for (uint ui = 0; ui < pConnection->m_aPlotList.size(); ui++)
		{
			if (pConnection->m_aPlotList[ui].m_iX == iX && pConnection->m_aPlotList[ui].m_iY == iY)
#endif
			{
				aiTradeConnectionIDs.push_back(pConnection->m_iID);
				if (bFailAtFirstFound)
				{
					break;
				}
			}
		}

		if (bFailAtFirstFound && aiTradeConnectionIDs.size() > 0)
		{
			break;
		}
	}

	return aiTradeConnectionIDs;	
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<int> CvPlayerTrade::GetOpposingTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const
#else
std::vector<int> CvPlayerTrade::GetOpposingTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound)
#endif
{
	return GetTradeUnitsAtPlot(pPlot, bFailAtFirstFound, true, false);
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::ContainsOpposingPlayerTradeUnit(const CvPlot* pPlot) const
#else
bool CvPlayerTrade::ContainsOpposingPlayerTradeUnit(const CvPlot* pPlot)
#endif
{
	std::vector<int> aiTradeConnectionIDs;
	aiTradeConnectionIDs = GetOpposingTradeUnitsAtPlot(pPlot, true);
	if (aiTradeConnectionIDs.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<int> CvPlayerTrade::GetEnemyTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const
#else
std::vector<int> CvPlayerTrade::GetEnemyTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound)
#endif
{
	return GetTradeUnitsAtPlot(pPlot, bFailAtFirstFound, true, true);
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::ContainsEnemyTradeUnit(const CvPlot* pPlot) const
#else
bool CvPlayerTrade::ContainsEnemyTradeUnit(const CvPlot* pPlot)
#endif
{
	std::vector<int> aiTradeConnectionIDs;
	aiTradeConnectionIDs = GetEnemyTradeUnitsAtPlot(pPlot, true);
	if (aiTradeConnectionIDs.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<int> CvPlayerTrade::GetEnemyTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const
#else
std::vector<int> CvPlayerTrade::GetEnemyTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound)
#endif
{
	return GetTradePlotsAtPlot(pPlot, bFailAtFirstFound, true, true);
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::ContainsEnemyTradePlot(const CvPlot* pPlot) const
#else
bool CvPlayerTrade::ContainsEnemyTradePlot(const CvPlot* pPlot)
#endif
{
	std::vector<int> aiTradeConnectionIDs;
	aiTradeConnectionIDs = GetEnemyTradePlotsAtPlot(pPlot, true);
	if (aiTradeConnectionIDs.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//	--------------------------------------------------------------------------------
bool CvPlayerTrade::PlunderTradeRoute(int iTradeConnectionID)
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iTradeConnectionIndex = pTrade->GetIndexFromID(iTradeConnectionID);

	CvAssertMsg(iTradeConnectionIndex >= 0, "iTradeConnectionIndex < 0");
	if (iTradeConnectionIndex < 0)
	{
		return false;
	}

	TradeConnection* pTradeConnection = &(pTrade->m_aTradeConnections[iTradeConnectionIndex]);
	DomainTypes eDomain = pTradeConnection->m_eDomain;
	PlayerTypes eOwningPlayer = pTradeConnection->m_eOriginOwner;
	PlayerTypes eDestPlayer = pTradeConnection->m_eDestOwner;
	TeamTypes eOwningTeam = GET_PLAYER(eOwningPlayer).getTeam();
	TeamTypes eDestTeam = GET_PLAYER(eDestPlayer).getTeam();
	CvPlot* pPlunderPlot = GC.getMap().plot(pTradeConnection->m_aPlotList[pTradeConnection->m_iTradeUnitLocationIndex].m_iX, pTradeConnection->m_aPlotList[pTradeConnection->m_iTradeUnitLocationIndex].m_iY);
	int iDomainModifier = GetTradeConnectionDomainValueModifierTimes100(*pTradeConnection, YIELD_GOLD);

	// add trade connnection to broken list
	GET_PLAYER(eOwningPlayer).GetTrade()->AddTradeConnectionWasPlundered(*pTradeConnection);

	CvCity* pOriginCity = NULL;
	CvCity* pDestCity = NULL;
	CvPlot* pPlot = GC.getMap().plot(pTradeConnection->m_iOriginX, pTradeConnection->m_iOriginY);
	if (pPlot)
	{
		pOriginCity = pPlot->getPlotCity();
	}
	pPlot = GC.getMap().plot(pTradeConnection->m_iDestX, pTradeConnection->m_iDestY);
	if (pPlot)
	{
		pDestCity = pPlot->getPlotCity();
	}
	CvAssertMsg(pOriginCity, "pOriginCity doesn't exist");
	CvAssertMsg(pDestCity, "pDestCity doesn't exist");

	bool bEmptyResult = pTrade->EmptyTradeRoute(iTradeConnectionIndex);

	// if the trade route was not broken
	if (!bEmptyResult)
	{
		return false;
	}

	int iPlunderGoldValue = 100;
	iPlunderGoldValue *= 100 + iDomainModifier;
	iPlunderGoldValue /= 100;
	m_pPlayer->GetTreasury()->ChangeGold(iPlunderGoldValue);

	// do the floating popup
	if (GC.getGame().getActivePlayer() == m_pPlayer->GetID())
	{
		char text[256] = {0};
		sprintf_s(text, "[COLOR_YELLOW]+%d[ENDCOLOR][ICON_GOLD]", iPlunderGoldValue);
		float fDelay = 0.0f;
		DLLUI->AddPopupText(pPlunderPlot->getX(), pPlunderPlot->getY(), text, fDelay);
		CvString strBuffer;
		if (eDomain == DOMAIN_LAND)
		{
			strBuffer = GetLocalizedText("TXT_KEY_MISC_PLUNDERED_GOLD_FROM_IMP", iPlunderGoldValue, "TXT_KEY_UNIT_CARAVAN");
		}
		else
		{
			strBuffer = GetLocalizedText("TXT_KEY_MISC_PLUNDERED_GOLD_FROM_IMP", iPlunderGoldValue, "TXT_KEY_UNIT_CARGO_SHIP");
		}

		
		DLLUI->AddMessage(0, m_pPlayer->GetID(), true, GC.getEVENT_MESSAGE_TIME(), strBuffer);
	}

	// barbarians get a bonus unit out of the deal!
	if (m_pPlayer->isBarbarian() && pPlot)
	{
		int iLoop = 0;
		for(CvUnit* pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
		{
			if (pLoopUnit->plot() == pPlunderPlot)
			{
				CvBarbarians::DoSpawnBarbarianUnit(pPlunderPlot, true, true);
				break;
			}
		}
	}

	// do the notification stuff
	if (pOriginCity && pDestCity)
	{
		// send notification to owner player
		CvNotifications* pNotifications = GET_PLAYER(eOwningPlayer).GetNotifications();
		if(pNotifications)
		{
			Localization::String strSummary;
			Localization::String strMessage;

			strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADER_SUMMARY");
			if (m_pPlayer->isBarbarian())
			{
				strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADER_BARBARIAN");
				strMessage << pOriginCity->getNameKey();
				strMessage << pDestCity->getNameKey();
			}
			else if (GET_TEAM(eOwningTeam).isHasMet(m_pPlayer->getTeam()))
			{
				strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADER_KNOWN");
				if(GC.getGame().isGameMultiPlayer() && m_pPlayer->isHuman())
				{
					strMessage << m_pPlayer->getNickName();
				}
				else
				{
					strMessage << m_pPlayer->getNameKey();
				}
				strMessage << pOriginCity->getNameKey();
				strMessage << pDestCity->getNameKey();
			}
			else
			{
				strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADER_UNKNOWN");
				strMessage << pOriginCity->getNameKey();
				strMessage << pDestCity->getNameKey();
			}

			pNotifications->Add(NOTIFICATION_TRADE_ROUTE_BROKEN, strMessage.toUTF8(), strSummary.toUTF8(), pPlunderPlot->getX(), pPlunderPlot->getY(), -1);
		}

		if (!GET_PLAYER(eDestPlayer).isMinorCiv() && eOwningPlayer != eDestPlayer && eDestPlayer != m_pPlayer->GetID())
		{
			// send notification to destination player
			pNotifications = GET_PLAYER(eDestPlayer).GetNotifications();
			if (pNotifications)
			{
				Localization::String strSummary;
				Localization::String strMessage;

				strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADEE_SUMMARY");
				if (m_pPlayer->isBarbarian())
				{
					strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADEE_BARBARIANS");
					if(GC.getGame().isGameMultiPlayer() && GET_PLAYER(eOwningPlayer).isHuman())
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNickName();
					}
					else
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNameKey();
					}
					strMessage << pOriginCity->getNameKey();
					strMessage << pDestCity->getNameKey();
				}
				else if (GET_TEAM(eDestTeam).isHasMet(m_pPlayer->getTeam()))
				{
					strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADEE_KNOWN");
					if(GC.getGame().isGameMultiPlayer() && m_pPlayer->isHuman())
					{
						strMessage << m_pPlayer->getNickName();
					}
					else
					{
						strMessage << m_pPlayer->getNameKey();
					}

					if(GC.getGame().isGameMultiPlayer() && GET_PLAYER(eOwningPlayer).isHuman())
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNickName();
					}
					else
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNameKey();
					}

					strMessage << pOriginCity->getNameKey();
					strMessage << pDestCity->getNameKey();
				}
				else
				{
					strMessage = Localization::Lookup("TXT_KEY_NOTIFICATION_TRADE_UNIT_PLUNDERED_TRADEE_UNKNOWN");

					if(GC.getGame().isGameMultiPlayer() && GET_PLAYER(eOwningPlayer).isHuman())
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNickName();
					}
					else
					{
						strMessage << GET_PLAYER(eOwningPlayer).getNameKey();
					}

					strMessage << pOriginCity->getNameKey();
					strMessage << pDestCity->getNameKey();
				}

				pNotifications->Add(NOTIFICATION_TRADE_ROUTE_BROKEN, strMessage.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
			}
		}
	}

	if (eDomain == DOMAIN_LAND && m_pPlayer->isHuman() && !GC.getGame().isGameMultiPlayer())
	{
		gDLL->UnlockAchievement(ACHIEVEMENT_XP2_28);
	}

	return true;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeRouteRange(DomainTypes eDomain, const CvCity* pOriginCity) const
#else
int CvPlayerTrade::GetTradeRouteRange (DomainTypes eDomain, CvCity* pOriginCity)
#endif
{
	int iRange = 0;

	int iBaseRange = 0;
	switch (eDomain)
	{
	case DOMAIN_SEA:
		iBaseRange = 20;
		break;
	case DOMAIN_LAND:
		iBaseRange = 10;
		break;
	default:
		CvAssertMsg(false, "Undefined domain for trade route range");
		return -1;
		break;
	}

	int iTraitRange = 0;
	switch (eDomain)
	{
	case DOMAIN_SEA:
		// not implemented
		iTraitRange = 0;
		break;
	case DOMAIN_LAND:
		iTraitRange = m_pPlayer->GetPlayerTraits()->GetLandTradeRouteRangeBonus();
		break;
	}

	CvPlayerTechs* pMyPlayerTechs = m_pPlayer->GetPlayerTechs();
	CvTeamTechs* pMyTeamTechs = GET_TEAM(GET_PLAYER(m_pPlayer->GetID()).getTeam()).GetTeamTechs();
	CvTechEntry* pTechInfo = NULL; 

	int iExtendedRange = 0;
#ifdef AUI_WARNING_FIXES
	for (uint iTechLoop = 0; iTechLoop < pMyPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
#else
	for(int iTechLoop = 0; iTechLoop < pMyPlayerTechs->GetTechs()->GetNumTechs(); iTechLoop++)
#endif
	{
		TechTypes eTech = (TechTypes)iTechLoop;
		if (!pMyTeamTechs->HasTech(eTech))
		{
			continue;
		}

		pTechInfo = pMyPlayerTechs->GetTechs()->GetEntry(eTech);
		CvAssertMsg(pTechInfo, "null tech entry");
		if (pTechInfo)
		{
			iExtendedRange += pTechInfo->GetTradeRouteDomainExtraRange(eDomain);
		}
	}
	
	int iRangeModifier = 0;
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
	{
		BuildingTypes eBuilding = (BuildingTypes)GET_PLAYER(pOriginCity->getOwner()).getCivilizationInfo().getCivilizationBuildings(iI);
		if(eBuilding != NO_BUILDING)
		{
			CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);
			if (!pBuildingEntry)
			{
				continue;
			}

			if (pBuildingEntry && pOriginCity->GetCityBuildings()->GetNumBuilding((BuildingTypes)pBuildingEntry->GetID()))
			{
				if (pBuildingEntry->GetTradeRouteSeaDistanceModifier() > 0 && eDomain == DOMAIN_SEA)
				{
					iRangeModifier += pBuildingEntry->GetTradeRouteSeaDistanceModifier();
				}
				else if (pBuildingEntry->GetTradeRouteLandDistanceModifier() > 0 && eDomain == DOMAIN_LAND)
				{
					iRangeModifier += pBuildingEntry->GetTradeRouteLandDistanceModifier();
				}
			}
		}
	}

	iRange = iBaseRange;
	iRange += iTraitRange;
	iRange += iExtendedRange;
	iRange = (iRange * (100 + iRangeModifier)) / 100;
	return iRange;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetTradeRouteSpeed(DomainTypes eDomain) const
#else
int CvPlayerTrade::GetTradeRouteSpeed (DomainTypes eDomain)
#endif
{
	switch (eDomain)
	{
	case DOMAIN_SEA:
		return 4;
		break;
	case DOMAIN_LAND:
		return 2;
		break;
	}

	CvAssertMsg(false, "Undefined domain for trade route speed");
	return -1;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
uint CvPlayerTrade::GetNumTradeRoutesPossible() const
#else
uint CvPlayerTrade::GetNumTradeRoutesPossible (void)
#endif
{
	int iNumRoutes = 0;

	CvAssert(m_pPlayer->getCivilizationType() != NO_CIVILIZATION);
	if (m_pPlayer->getCivilizationType() == NO_CIVILIZATION)
		return 0;

	CvPlayerTechs* pMyPlayerTechs = m_pPlayer->GetPlayerTechs();
	CvTeamTechs* pMyTeamTechs = GET_TEAM(GET_PLAYER(m_pPlayer->GetID()).getTeam()).GetTeamTechs();
	CvTechEntry* pTechInfo = NULL; 

	CvTechXMLEntries* pMyPlayerTechEntries = pMyPlayerTechs->GetTechs();
	CvAssert(pMyPlayerTechEntries);
	if (pMyPlayerTechEntries == NULL)
		return 0;

#ifdef AUI_WARNING_FIXES
	for (uint iTechLoop = 0; iTechLoop < pMyPlayerTechEntries->GetNumTechs(); iTechLoop++)
#else
	for(int iTechLoop = 0; iTechLoop < pMyPlayerTechEntries->GetNumTechs(); iTechLoop++)
#endif
	{
		TechTypes eTech = (TechTypes)iTechLoop;
		if (!pMyTeamTechs->HasTech(eTech))
		{
			continue;
		}

		pTechInfo = pMyPlayerTechEntries->GetEntry(eTech);
		CvAssertMsg(pTechInfo, "null tech entry");
		if (pTechInfo)
		{
			iNumRoutes += pTechInfo->GetNumInternationalTradeRoutesChange();
		}
	}

#ifdef NQ_EXTRA_TRADE_ROUTES_FROM_BELIEF
	ReligionTypes eFoundedReligion = GC.getGame().GetGameReligions()->GetFounderBenefitsReligion(m_pPlayer->GetID());
	if (eFoundedReligion && eFoundedReligion != NO_RELIGION)
	{
		const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eFoundedReligion, NO_PLAYER);
		iNumRoutes += pReligion->m_Beliefs.GetExtraTradeRoutes();
	}
#endif

	CvCivilizationInfo& kCivInfo = m_pPlayer->getCivilizationInfo();
	int iLoop = 0;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
		{
			BuildingTypes eBuilding = (BuildingTypes)kCivInfo.getCivilizationBuildings(iI);
			if(eBuilding != NO_BUILDING)
			{
				CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);
				if (!pBuildingEntry)
				{
					continue;
				}

				if (pBuildingEntry)
				{
					if (pLoopCity->GetCityBuildings()->GetNumBuilding((BuildingTypes)pBuildingEntry->GetID()))
					{
						int iNumRouteBonus = pBuildingEntry->GetNumTradeRouteBonus();
						if (iNumRouteBonus != 0)
						{
							iNumRoutes += iNumRouteBonus;
						}
					}
				}
			}
		}
	}

	int iModifier = 100 + m_pPlayer->GetPlayerTraits()->GetNumTradeRoutesModifier();
	iNumRoutes *= iModifier;
	iNumRoutes /= 100;

	return iNumRoutes;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetNumTradeRoutesUsed(bool bContinueTraining) const
#else
int CvPlayerTrade::GetNumTradeRoutesUsed (bool bContinueTraining)
#endif
{
	int iReturnValue = 0;
	int iLoop;

	// look for units on the map
	CvUnit* pLoopUnit;
	for(pLoopUnit = m_pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = m_pPlayer->nextUnit(&iLoop))
	{
		if (pLoopUnit->isTrade())
		{
			iReturnValue++;
		}
	}

	// look inside cities
	if (!bContinueTraining)
	{
		CvCity* pLoopCity;
		for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
		{
			iReturnValue += pLoopCity->getNumTrainUnitAI(UNITAI_TRADE_UNIT);
		}
	}

	return iReturnValue;
}


//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetNumTradeRoutesRemaining(bool bContinueTraining) const
#else
int CvPlayerTrade::GetNumTradeRoutesRemaining (bool bContinueTraining)
#endif
{
	return (GetNumTradeRoutesPossible() - GetNumTradeRoutesUsed(bContinueTraining));
}

#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
//	--------------------------------------------------------------------------------
int CvPlayerTrade::GetNumForeignTradeRoutes(PlayerTypes ePlayer)
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	int iResult = 0;
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
		if (pTrade->IsTradeRouteIndexEmpty(ui))
		{
			continue;
		}
		TradeConnection* pTradeConnection = &(pTrade->m_aTradeConnections[ui]);
		if (pTradeConnection->m_eOriginOwner == ePlayer && pTradeConnection->m_eOriginOwner != pTradeConnection->m_eDestOwner)
		{
			iResult++;
		}
	}
	return iResult;
}
#endif

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
int CvPlayerTrade::GetNumDifferentTradingPartners() const
#else
int CvPlayerTrade::GetNumDifferentTradingPartners (void)
#endif
{
	CvGameTrade* pTrade = GC.getGame().GetGameTrade();

#ifdef AUI_TRADE_FIX_GET_NUM_DIFFERENT_TRADING_PARTNERS_USES_ARRAY
	bool abConnections[MAX_CIV_PLAYERS] = {};
#else
	std::vector<bool> abConnections;
	abConnections.resize(MAX_CIV_PLAYERS, false);
#endif

	int iResult = 0;

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator pTradeConnection = pTrade->m_aTradeConnections.begin(); pTradeConnection != pTrade->m_aTradeConnections.end(); ++pTradeConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pTradeConnection))
#else
	for (uint ui = 0; ui < pTrade->m_aTradeConnections.size(); ui++)
	{
		if (pTrade->IsTradeRouteIndexEmpty(ui))
#endif
		{
			continue;
		}

#ifndef AUI_ITERATORIZE
#ifdef AUI_CONSTIFY
		const TradeConnection* pTradeConnection = &(pTrade->m_aTradeConnections[ui]);
#else
		TradeConnection* pTradeConnection = &(pTrade->m_aTradeConnections[ui]);
#endif
#endif
		if (pTradeConnection->m_eOriginOwner == pTradeConnection->m_eDestOwner)
		{
			continue;
		}

		// this involves us
		if (pTradeConnection->m_eOriginOwner == m_pPlayer->GetID())
		{
			if (!abConnections[pTradeConnection->m_eDestOwner])
			{
				abConnections[pTradeConnection->m_eDestOwner] = true;
				iResult++;
			}
		}
		else if (pTradeConnection->m_eDestOwner == m_pPlayer->GetID())
		{
			if (!abConnections[pTradeConnection->m_eOriginOwner])
			{
				abConnections[pTradeConnection->m_eOriginOwner] = true;
				iResult++;
			}
		}
	}

	return iResult;
}

//	--------------------------------------------------------------------------------
void CvPlayerTrade::UpdateTradeConnectionWasPlundered()
{
	int iTurnsUntilForget = 30;
#ifdef AUI_ITERATORIZE
	for (TradeConnectionWasPlunderedList::iterator it = m_aTradeConnectionWasPlundered.begin(); it != m_aTradeConnectionWasPlundered.end(); ++it)
	{
		if ((it->m_iTurnPlundered + iTurnsUntilForget) <= GC.getGame().getGameTurn())
		{
			// erasing entry to make sure vector doesn't get bigger than it should
			m_aTradeConnectionWasPlundered.erase(it);
			--it;
#else
	for (uint ui = 0; ui < m_aTradeConnectionWasPlundered.size(); ui++)
	{
		if ((m_aTradeConnectionWasPlundered[ui].m_iTurnPlundered + iTurnsUntilForget) <= GC.getGame().getGameTurn())
		{
			// setting turn plundered to -1 to flag it as not counting
			m_aTradeConnectionWasPlundered[ui].m_iTurnPlundered = -1;
#endif
		}
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvPlayerTrade::AddTradeConnectionWasPlundered(const TradeConnection& kTradeConnection)
#else
void CvPlayerTrade::AddTradeConnectionWasPlundered(const TradeConnection kTradeConnection)
#endif
{
	bool bAdded = false;
#ifdef AUI_ITERATORIZE
	for (TradeConnectionWasPlunderedList::iterator it = m_aTradeConnectionWasPlundered.begin(); it != m_aTradeConnectionWasPlundered.end(); ++it)
	{
		if (it->m_kTradeConnection.m_iOriginX == kTradeConnection.m_iOriginX &&
			it->m_kTradeConnection.m_iOriginY == kTradeConnection.m_iOriginY &&
			it->m_kTradeConnection.m_iDestX == kTradeConnection.m_iDestX &&
			it->m_kTradeConnection.m_iDestY == kTradeConnection.m_iDestY)
		{
			it->m_iTurnPlundered = GC.getGame().getGameTurn();
#else
	for (uint ui = 0; ui < m_aTradeConnectionWasPlundered.size(); ui++)
	{
		if (m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginX == kTradeConnection.m_iOriginX &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginY == kTradeConnection.m_iOriginY &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestX   == kTradeConnection.m_iDestX &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestY   == kTradeConnection.m_iDestY)
		{
			m_aTradeConnectionWasPlundered[ui].m_iTurnPlundered = GC.getGame().getGameTurn();
#endif
			bAdded = true;
			break;
		}
	}

	if (!bAdded)
	{
		TradeConnectionWasPlundered kWasPlundered;
#ifdef AUI_ITERATORIZE
		m_aTradeConnectionWasPlundered.push_back(kWasPlundered);
		const TradeConnectionWasPlunderedList::iterator it = m_aTradeConnectionWasPlundered.end() - 1;
		it->m_kTradeConnection = kTradeConnection;
		it->m_iTurnPlundered = GC.getGame().getGameTurn();
#else
		uint uiIndex = m_aTradeConnectionWasPlundered.size();
		m_aTradeConnectionWasPlundered.push_back(kWasPlundered);
		m_aTradeConnectionWasPlundered[uiIndex].m_kTradeConnection = kTradeConnection;
		m_aTradeConnectionWasPlundered[uiIndex].m_iTurnPlundered = GC.getGame().getGameTurn();
#endif
	}
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
bool CvPlayerTrade::CheckTradeConnectionWasPlundered(const TradeConnection& kTradeConnection) const
#else
bool CvPlayerTrade::CheckTradeConnectionWasPlundered(const TradeConnection& kTradeConnection)
#endif
{
#ifdef AUI_ITERATORIZE
	for (TradeConnectionWasPlunderedList::const_iterator it = m_aTradeConnectionWasPlundered.begin(); it != m_aTradeConnectionWasPlundered.end(); ++it)
	{
		if (it->m_kTradeConnection.m_iOriginX == kTradeConnection.m_iOriginX &&
			it->m_kTradeConnection.m_iOriginY == kTradeConnection.m_iOriginY &&
			it->m_kTradeConnection.m_iDestX == kTradeConnection.m_iDestX &&
			it->m_kTradeConnection.m_iDestY == kTradeConnection.m_iDestY &&
			it->m_iTurnPlundered != -1)
#else
	for (uint ui = 0; ui < m_aTradeConnectionWasPlundered.size(); ui++)
	{
		if (m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginX == kTradeConnection.m_iOriginX &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginY == kTradeConnection.m_iOriginY &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestX   == kTradeConnection.m_iDestX &&
			m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestY   == kTradeConnection.m_iDestY &&
			m_aTradeConnectionWasPlundered[ui].m_iTurnPlundered != -1)
#endif
		{
			return true;
		}
	}

	return false;
}

//	--------------------------------------------------------------------------------
UnitTypes CvPlayerTrade::GetTradeUnit (DomainTypes eDomain)
{
	UnitTypes eUnitType = NO_UNIT;
	if (eDomain == DOMAIN_LAND)
	{
		eUnitType = (UnitTypes)GC.getInfoTypeForString("UNIT_CARAVAN");
	}
	else if (eDomain == DOMAIN_SEA)
	{
		eUnitType = (UnitTypes)GC.getInfoTypeForString("UNIT_CARGO_SHIP");
	}

	return eUnitType;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<CvString> CvPlayerTrade::GetPlotToolTips(CvPlot* pPlot) const
#else
std::vector<CvString> CvPlayerTrade::GetPlotToolTips (CvPlot* pPlot)
#endif
{
	std::vector<CvString> aToolTips;

	if (!pPlot)
	{
		return aToolTips;
	}

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	PlayerTypes ePlayer = m_pPlayer->GetID();
	int iX = pPlot->getX();
	int iY = pPlot->getY();
	CvCity* pOriginCity = NULL;
	CvCity* pDestCity = NULL;

	Localization::String strResult;

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint uiConnection = 0; uiConnection < pTrade->m_aTradeConnections.size(); uiConnection++)
	{
		if (pTrade->IsTradeRouteIndexEmpty(uiConnection))
#endif
		{
			continue;
		}

#ifndef AUI_ITERATORIZE
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#endif

		bool bDisplayInfo = false;
		if (pConnection->m_eOriginOwner == ePlayer || pConnection->m_eDestOwner == ePlayer)
		{
			bDisplayInfo = true;
		}
		else
		{
			int iTradeUnitIndex = pConnection->m_iTradeUnitLocationIndex;
			int iTradeUnitX = pConnection->m_aPlotList[iTradeUnitIndex].m_iX;
			int iTradeUnitY = pConnection->m_aPlotList[iTradeUnitIndex].m_iY;
			if (iTradeUnitX == iX && iTradeUnitY == iY)
			{
				bDisplayInfo = true;
			}
		}

		// if this trade route involves us
		if (bDisplayInfo)
		{
			for (uint uiPlotIndex = 0; uiPlotIndex < pConnection->m_aPlotList.size(); uiPlotIndex++)
			{
				if (pConnection->m_aPlotList[uiPlotIndex].m_iX == iX && pConnection->m_aPlotList[uiPlotIndex].m_iY == iY)
				{
					pOriginCity = GC.getMap().plot(pConnection->m_iOriginX, pConnection->m_iOriginY)->getPlotCity();
					pDestCity = GC.getMap().plot(pConnection->m_iDestX, pConnection->m_iDestY)->getPlotCity();

					// Trade Routes:
					// (if major to major)
					// Kyoto (Japan) ~~ London (England)
					// (if major to minor)
					// Kyoto (Japan) ~~ Monaco (City-State)

					if (uiPlotIndex == pConnection->m_iTradeUnitLocationIndex)
					{
						if (pConnection->m_eDomain == DOMAIN_LAND)
						{
							Localization::String strTradeUnit = Localization::Lookup("TXT_KEY_TRADE_ROUTE_TT_PLOT_CARAVAN");
							strTradeUnit << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationAdjectiveKey();
							aToolTips.push_back(strTradeUnit.toUTF8());
						}
						else if (pConnection->m_eDomain == DOMAIN_SEA)
						{
							Localization::String strTradeUnit = Localization::Lookup("TXT_KEY_TRADE_ROUTE_TT_PLOT_CARGO_SHIP");
							strTradeUnit << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationAdjectiveKey();
							aToolTips.push_back(strTradeUnit.toUTF8());
						}
					}

					Localization::String strTradeLine;

					if (GET_PLAYER(pConnection->m_eDestOwner).isMinorCiv()) // if a minor civ, have a special help text
					{
						strTradeLine = Localization::Lookup("TXT_KEY_TRADE_ROUTE_TT_PLOT_MAJOR_MINOR");
						strTradeLine << ((pOriginCity)?pOriginCity->getNameKey():"NULL Origin City");
						strTradeLine << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationShortDescriptionKey();
						strTradeLine << ((pDestCity)?pDestCity->getNameKey():"NULL Dest City");
					}
					else
					{
						strTradeLine = Localization::Lookup("TXT_KEY_TRADE_ROUTE_TT_PLOT_MAJOR_MAJOR");
						strTradeLine << ((pOriginCity)?pOriginCity->getNameKey():"NULL Origin City");
						strTradeLine << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationShortDescriptionKey();
						strTradeLine << ((pDestCity)?pDestCity->getNameKey():"NULL Dest City");

						strTradeLine << GET_PLAYER(pConnection->m_eDestOwner).getCivilizationShortDescriptionKey();
					}

					aToolTips.push_back(strTradeLine.toUTF8());
				}
			}
		}
	}

	return aToolTips;
}

//	--------------------------------------------------------------------------------
#ifdef AUI_CONSTIFY
std::vector<CvString> CvPlayerTrade::GetPlotMouseoverToolTips(CvPlot* pPlot) const
#else
std::vector<CvString> CvPlayerTrade::GetPlotMouseoverToolTips (CvPlot* pPlot)
#endif
{
	std::vector<CvString> aToolTips;

	if (!pPlot)
	{
		return aToolTips;
	}

	CvGameTrade* pTrade = GC.getGame().GetGameTrade();
	PlayerTypes ePlayer = m_pPlayer->GetID();
	int iX = pPlot->getX();
	int iY = pPlot->getY();
	CvCity* pOriginCity = NULL;
	CvCity* pDestCity = NULL;

	Localization::String strResult;

#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator pConnection = pTrade->m_aTradeConnections.begin(); pConnection != pTrade->m_aTradeConnections.end(); ++pConnection)
	{
		if (pTrade->IsTradeRouteIndexEmpty(pConnection))
#else
	for (uint uiConnection = 0; uiConnection < pTrade->m_aTradeConnections.size(); uiConnection++)
	{
		if (pTrade->IsTradeRouteIndexEmpty(uiConnection))
#endif
		{
			continue;
		}

#ifndef AUI_ITERATORIZE
		TradeConnection* pConnection = &(pTrade->m_aTradeConnections[uiConnection]);
#endif

		bool bDisplayInfo = false;
		if (pConnection->m_eOriginOwner == ePlayer || pConnection->m_eDestOwner == ePlayer)
		{
			bDisplayInfo = true;
		}
		else
		{
			int iTradeUnitIndex = pConnection->m_iTradeUnitLocationIndex;
			int iTradeUnitX = pConnection->m_aPlotList[iTradeUnitIndex].m_iX;
			int iTradeUnitY = pConnection->m_aPlotList[iTradeUnitIndex].m_iY;
			if (iTradeUnitX == iX && iTradeUnitY == iY)
			{
				bDisplayInfo = true;
			}
		}

		// if this trade route involves us
		if (bDisplayInfo)
		{
			for (uint uiPlotIndex = 0; uiPlotIndex < pConnection->m_aPlotList.size(); uiPlotIndex++)
			{
				if (pConnection->m_aPlotList[uiPlotIndex].m_iX == iX && pConnection->m_aPlotList[uiPlotIndex].m_iY == iY)
				{
					pOriginCity = GC.getMap().plot(pConnection->m_iOriginX, pConnection->m_iOriginY)->getPlotCity();
					pDestCity = GC.getMap().plot(pConnection->m_iDestX, pConnection->m_iDestY)->getPlotCity();
					if (uiPlotIndex == pConnection->m_iTradeUnitLocationIndex)
					{
#ifdef AUI_WARNING_FIXES
						CvUnitEntry* pTradeUnitInfo = GC.getUnitInfo(GetTradeUnit(pConnection->m_eDomain));
#endif
						Localization::String strLine;
						if(strcmp(GET_PLAYER(pConnection->m_eOriginOwner).getNickName(), "") != 0)
						{
							strLine = Localization::Lookup("TXT_KEY_MULTIPLAYER_UNIT_TT");
							strLine << GET_PLAYER(pConnection->m_eOriginOwner).getNickName();
							strLine << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationAdjectiveKey();
#ifdef AUI_WARNING_FIXES
							if (pTradeUnitInfo)
								strLine << pTradeUnitInfo->GetDescription();
#else
							strLine << GC.getUnitInfo(GetTradeUnit(pConnection->m_eDomain))->GetDescription();
#endif
						}
						else
						{
							strLine = Localization::Lookup("TXT_KEY_PLOTROLL_UNIT_DESCRIPTION_CIV");
							strLine << GET_PLAYER(pConnection->m_eOriginOwner).getCivilizationAdjectiveKey();
#ifdef AUI_WARNING_FIXES
							if (pTradeUnitInfo)
								strLine << pTradeUnitInfo->GetDescription();
#else
							strLine << GC.getUnitInfo(GetTradeUnit(pConnection->m_eDomain))->GetDescription();
#endif
						}

						
						Localization::String strColorString;
						TeamTypes eTeam = GET_PLAYER(pConnection->m_eOriginOwner).getTeam();
						if (eTeam == GC.getGame().getActiveTeam())
						{
							strColorString = "[COLOR_WHITE]{1_InnerStr}[ENDCOLOR]";
							strColorString << strLine;
						}
						else if (GET_TEAM(GC.getGame().getActiveTeam()).isAtWar(eTeam))
						{
							strColorString = "[COLOR_NEGATIVE_TEXT]{1_InnerStr}[ENDCOLOR]";
							strColorString << strLine;
						}
						else
						{
							strColorString = "[COLOR_POSITIVE_TEXT]{1_InnerStr}[ENDCOLOR]";
							strColorString << strLine;
						}

						aToolTips.push_back(strColorString.toUTF8());
					}
				}
			}
		}
	}

	return aToolTips;
}

/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvPlayerTrade& writeTo)
{
	uint uiVersion;
	loadFrom >> uiVersion;

	if (uiVersion >= 1) 
	{
		int iNum = 0;
		loadFrom >> iNum;
		for (int i = 0; i < iNum; i++)
		{
			TradeConnection kTradeConnection;
			writeTo.m_aRecentlyExpiredConnections.push_back(kTradeConnection);
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iID;

			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iOriginX;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iOriginY;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iDestX;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iDestY;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_eOriginOwner;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_eDestOwner;
			int iDomain;
			loadFrom >> iDomain;
			writeTo.m_aRecentlyExpiredConnections[i].m_eDomain = (DomainTypes)iDomain;
			int iConnectionType;
			loadFrom >> iConnectionType;
			writeTo.m_aRecentlyExpiredConnections[i].m_eConnectionType = (TradeConnectionType)iConnectionType;

			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iTradeUnitLocationIndex;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_bTradeUnitMovingForward;

			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_unitID;

			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iCircuitsCompleted;
			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iCircuitsToComplete;

			loadFrom >> writeTo.m_aRecentlyExpiredConnections[i].m_iTurnRouteComplete;
		}
	}
	else
	{
		int iEmptyInt;
		loadFrom >> iEmptyInt;
	}

	if (uiVersion >= 2)
	{
		int iNum = 0;
		loadFrom >> iNum;
		for (int i = 0; i < iNum; i++)
		{
			TradeConnectionWasPlundered kTradeConnectionWasPlundered;
			writeTo.m_aTradeConnectionWasPlundered.push_back(kTradeConnectionWasPlundered);
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_iTurnPlundered;

			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iOriginX;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iOriginY;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iDestX;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iDestY;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_eOriginOwner;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_eDestOwner;
			int iDomain;
			loadFrom >> iDomain;
			writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_eDomain = (DomainTypes)iDomain;
			int iConnectionType;
			loadFrom >> iConnectionType;
			writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_eConnectionType = (TradeConnectionType)iConnectionType;

			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iTradeUnitLocationIndex;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_bTradeUnitMovingForward;

			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_unitID;

			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iCircuitsCompleted;
			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iCircuitsToComplete;

			loadFrom >> writeTo.m_aTradeConnectionWasPlundered[i].m_kTradeConnection.m_iTurnRouteComplete;
		}
	}

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvPlayerTrade& readFrom)
{
	uint uiVersion = 2;
	saveTo << uiVersion;

	saveTo << readFrom.m_aRecentlyExpiredConnections.size();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::const_iterator it = readFrom.m_aRecentlyExpiredConnections.begin(); it != readFrom.m_aRecentlyExpiredConnections.end(); ++it)
	{
		saveTo << it->m_iID;

		saveTo << it->m_iOriginX;
		saveTo << it->m_iOriginY;
		saveTo << it->m_iDestX;
		saveTo << it->m_iDestY;
		saveTo << it->m_eOriginOwner;
		saveTo << it->m_eDestOwner;
		saveTo << (int)it->m_eDomain;
		saveTo << (int)it->m_eConnectionType;
		saveTo << it->m_iTradeUnitLocationIndex;
		saveTo << it->m_bTradeUnitMovingForward;
		saveTo << it->m_unitID;
		saveTo << it->m_iCircuitsCompleted;
		saveTo << it->m_iCircuitsToComplete;
		saveTo << it->m_iTurnRouteComplete;
#else
	for (uint ui = 0; ui < readFrom.m_aRecentlyExpiredConnections.size(); ui++)
	{
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iID;

		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iOriginX;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iOriginY;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iDestX;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iDestY;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_eOriginOwner;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_eDestOwner;
		saveTo << (int)readFrom.m_aRecentlyExpiredConnections[ui].m_eDomain;
		saveTo << (int)readFrom.m_aRecentlyExpiredConnections[ui].m_eConnectionType;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iTradeUnitLocationIndex;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_bTradeUnitMovingForward;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_unitID;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iCircuitsCompleted;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iCircuitsToComplete;
		saveTo << readFrom.m_aRecentlyExpiredConnections[ui].m_iTurnRouteComplete;
#endif
	}

	saveTo << readFrom.m_aTradeConnectionWasPlundered.size();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionWasPlunderedList::const_iterator it = readFrom.m_aTradeConnectionWasPlundered.begin(); it != readFrom.m_aTradeConnectionWasPlundered.end(); ++it)
	{
		saveTo << it->m_iTurnPlundered;

		saveTo << it->m_kTradeConnection.m_iOriginX;
		saveTo << it->m_kTradeConnection.m_iOriginY;
		saveTo << it->m_kTradeConnection.m_iDestX;
		saveTo << it->m_kTradeConnection.m_iDestY;
		saveTo << it->m_kTradeConnection.m_eOriginOwner;
		saveTo << it->m_kTradeConnection.m_eDestOwner;
		saveTo << (int)it->m_kTradeConnection.m_eDomain;
		saveTo << (int)it->m_kTradeConnection.m_eConnectionType;
		saveTo << it->m_kTradeConnection.m_iTradeUnitLocationIndex;
		saveTo << it->m_kTradeConnection.m_bTradeUnitMovingForward;
		saveTo << it->m_kTradeConnection.m_unitID;
		saveTo << it->m_kTradeConnection.m_iCircuitsCompleted;
		saveTo << it->m_kTradeConnection.m_iCircuitsToComplete;
		saveTo << it->m_kTradeConnection.m_iTurnRouteComplete;
#else
	for (uint ui = 0; ui < readFrom.m_aTradeConnectionWasPlundered.size(); ui++)
	{
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_iTurnPlundered;

		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginX;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iOriginY;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestX;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iDestY;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_eOriginOwner;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_eDestOwner;
		saveTo << (int)readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_eDomain;
		saveTo << (int)readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_eConnectionType;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iTradeUnitLocationIndex;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_bTradeUnitMovingForward;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_unitID;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iCircuitsCompleted;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iCircuitsToComplete;
		saveTo << readFrom.m_aTradeConnectionWasPlundered[ui].m_kTradeConnection.m_iTurnRouteComplete;
#endif
	}

	return saveTo;
}

//=====================================
// CvTradeAI
//=====================================
/// Constructor
CvTradeAI::CvTradeAI(void):
#ifdef AUI_WARNING_FIXES
	m_iRemovableValue(0),
#endif
m_pPlayer(NULL)
{
}

/// Destructor
CvTradeAI::~CvTradeAI(void)
{
	Uninit();
}

/// Initialize class data
void CvTradeAI::Init(CvPlayer* pPlayer)
{
	Reset();
	m_pPlayer = pPlayer;
	m_iRemovableValue = 0;
}

/// Cleanup
void CvTradeAI::Uninit()
{
	Reset();
}

/// Reset
void CvTradeAI::Reset()
{
	m_pPlayer = NULL;
}

/// DoTurn
void CvTradeAI::DoTurn()
{

}

/// Get all available TR
void CvTradeAI::GetAvailableTR(TradeConnectionList& aTradeConnectionList)
{
	aTradeConnectionList.clear();

	// build trade route list
	CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	CvGameTrade* pGameTrade = GC.getGame().GetGameTrade();
	int iOriginCityLoop;
	CvCity* pOriginCity = NULL;
	for (pOriginCity = m_pPlayer->firstCity(&iOriginCityLoop); pOriginCity != NULL; pOriginCity = m_pPlayer->nextCity(&iOriginCityLoop))
	{
		PlayerTypes eOtherPlayer = NO_PLAYER;
		for (uint ui = 0; ui < MAX_CIV_PLAYERS; ui++)
		{
			eOtherPlayer = (PlayerTypes)ui;

			if (!GET_PLAYER(eOtherPlayer).isAlive())
			{
				continue;
			}

			if (GET_PLAYER(eOtherPlayer).isBarbarian())
			{
				continue;
			}

			int iDestCityLoop;
			CvCity* pDestCity = NULL;
			for (pDestCity = GET_PLAYER(eOtherPlayer).firstCity(&iDestCityLoop); pDestCity != NULL; pDestCity = GET_PLAYER(eOtherPlayer).nextCity(&iDestCityLoop))
			{
				// if this is the same city
				if (pOriginCity == pDestCity)
				{
					continue;
				}

				DomainTypes eDomain = NO_DOMAIN;
				for (eDomain = (DomainTypes)0; eDomain < NUM_DOMAIN_TYPES; eDomain = (DomainTypes)(eDomain + 1))
				{
					// if this isn't a valid trade domain, ignore
					if (eDomain != DOMAIN_LAND && eDomain != DOMAIN_SEA)
					{
						continue;
					}

					bool bCheckPath = true;
					TradeConnectionType eConnection = NUM_TRADE_CONNECTION_TYPES;
					for (uint uiConnectionTypes = 0; uiConnectionTypes < NUM_TRADE_CONNECTION_TYPES; uiConnectionTypes++)
					{
						eConnection = (TradeConnectionType)uiConnectionTypes;

						// Check the trade route, ignoring the path for now
						bool bTradeAvailable = pPlayerTrade->CanCreateTradeRoute(pOriginCity, pDestCity, eDomain, eConnection, true, false);
						if (!bTradeAvailable)
						{
							continue;
						}

						// Now test the path
						if (bCheckPath)
						{
							bTradeAvailable = pGameTrade->IsValidTradeRoutePath(pOriginCity, pDestCity, eDomain);
							if (!bTradeAvailable)
								break;		// If there is no path for this domain, just skip the rest of the connection tests.

							bCheckPath = false;		// No need to check the path for this domain again
						}

						TradeConnection kConnection;
						kConnection.m_iOriginX = pOriginCity->getX();
						kConnection.m_iOriginY = pOriginCity->getY();
						kConnection.m_iDestX = pDestCity->getX();
						kConnection.m_iDestY = pDestCity->getY();
						kConnection.m_eConnectionType = eConnection;
						kConnection.m_eDomain = eDomain;
						kConnection.m_eOriginOwner = pOriginCity->getOwner();
						kConnection.m_eDestOwner = pDestCity->getOwner();

						CvAStarNode* pNode = NULL;
						if (eDomain ==  DOMAIN_LAND)
						{
							pNode = GC.GetInternationalTradeRouteLandFinder().GetLastNode();
						}
						else if (eDomain == DOMAIN_SEA)
						{
							pNode = GC.GetInternationalTradeRouteWaterFinder().GetLastNode();
						}

						CvAssertMsg(pNode != NULL, "no path found for unit");
						if (pNode == NULL)
						{
							continue;
						}

						GC.getGame().GetGameTrade()->CopyPathIntoTradeConnection(pNode, &kConnection);
						aTradeConnectionList.push_back(kConnection);
					}
				}
			}
		}
	}
}

/// Score TR
#ifdef AUI_CONSTIFY
int CvTradeAI::ScoreInternationalTR(const TradeConnection& kTradeConnection) const
#else
int CvTradeAI::ScoreInternationalTR (const TradeConnection& kTradeConnection)
#endif
{
	// don't evaluate other trade types
	if (kTradeConnection.m_eConnectionType != TRADE_CONNECTION_INTERNATIONAL)
	{
		return 0;
	}

	// if this was recently plundered, 0 the score
	if (m_pPlayer->GetTrade()->CheckTradeConnectionWasPlundered(kTradeConnection))
	{
		return 0;
	}

	CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	CvPlayerTrade* pOtherPlayerTrade = GET_PLAYER(kTradeConnection.m_eDestOwner).GetTrade();
	int iDangerSum = 1; // can't be zero because we divide by zero!
	for (uint uiPlotList = 0; uiPlotList < kTradeConnection.m_aPlotList.size(); uiPlotList++)
	{
		CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_aPlotList[uiPlotList].m_iX, kTradeConnection.m_aPlotList[uiPlotList].m_iY);
		CvAssertMsg(pPlot, "pPlot is null when trying to evaluate the list");
		if (pPlot == NULL)
		{
			break;
		}
		int iDangerValue = m_pPlayer->GetPlotDanger(*pPlot);
		if (iDangerValue == 0)
		{
			if (!pPlot->isVisible(m_pPlayer->getTeam()))
			{
				iDangerValue += 1;
			}

			if (pPlot->getTeam() != NO_TEAM && GET_TEAM(m_pPlayer->getTeam()).isAtWar(pPlot->getTeam()))
			{
				iDangerValue += 1000;
			}
		}
		iDangerSum += iDangerValue;
	}

	// gold
	int iGoldAmount = pPlayerTrade->GetTradeConnectionValueTimes100(kTradeConnection, YIELD_GOLD, true);
	int iOtherGoldAmount = pOtherPlayerTrade->GetTradeConnectionValueTimes100(kTradeConnection, YIELD_GOLD, false);
	int iGoldDelta = iGoldAmount - iOtherGoldAmount;

	// getting out of a logjam at the beginning of the game on an archepeligo map
	// if the player has made a trade unit but all the trade routes yields will be negative to the player
	// still try to trade with some of the minor civs
	if (iGoldDelta <= 0 && GET_PLAYER(kTradeConnection.m_eDestOwner).isMinorCiv())
	{
		iGoldDelta = 1;
	}

	// tech
	int iTechDifferenceP1fromP2 = GC.getGame().GetGameTrade()->GetTechDifference(kTradeConnection.m_eOriginOwner, kTradeConnection.m_eDestOwner);
	int iTechDifferenceP2fromP1 = GC.getGame().GetGameTrade()->GetTechDifference(kTradeConnection.m_eDestOwner,   kTradeConnection.m_eOriginOwner);
	int iTechDelta = iTechDifferenceP1fromP2 - iTechDifferenceP2fromP1;

	// religion
	ReligionTypes eOwnerFoundedReligion = GC.getGame().GetGameReligions()->GetReligionCreatedByPlayer(m_pPlayer->GetID());
	int iReligionDelta = 0;
	if (eOwnerFoundedReligion != NO_RELIGION)
	{
		CvCity* pToCity = CvGameTrade::GetDestCity(kTradeConnection);
		CvCity* pFromCity = CvGameTrade::GetOriginCity(kTradeConnection);
		CvAssert(pToCity != NULL && pFromCity != NULL);
		if (pToCity != NULL && pFromCity != NULL)
		{
			ReligionTypes eToReligion = NO_RELIGION;
			int iToPressure = 0;
			ReligionTypes eFromReligion = NO_RELIGION;
			int iFromPressure = 0;
			bool bAnyFromCityPressure = pFromCity->GetCityReligions()->WouldExertTradeRoutePressureToward(pToCity, eToReligion, iToPressure);
			bool bAnyToCityPressure = pToCity->GetCityReligions()->WouldExertTradeRoutePressureToward(pFromCity, eFromReligion, iFromPressure);

			// Internally pressure is now 10 times greater than what is shown to user
			iToPressure /= GC.getRELIGION_MISSIONARY_PRESSURE_MULTIPLIER();
			iFromPressure /= GC.getRELIGION_MISSIONARY_PRESSURE_MULTIPLIER();

			// if anyone is exerting pressure
			if (bAnyFromCityPressure || bAnyToCityPressure)
			{
				// "to" and "from" religions need to be different for us to care
				if (eToReligion != eFromReligion)
				{
					if (eToReligion == eOwnerFoundedReligion)
					{
						iReligionDelta += iToPressure;
					}

					if (eFromReligion != eOwnerFoundedReligion)
					{
						iReligionDelta -= iFromPressure;
					}
				}
			}
		}
	}

	int iScore = 0;
	iScore += iGoldDelta;
	iScore += iTechDelta * 3; // 3 science = 1 gold
	iScore += iReligionDelta * 2; // 2 religion = 1 gold
	iScore = (iScore * 10) / iDangerSum;
	
	// if we have any tourism and the destination owner is not a minor civ
	if (m_pPlayer->GetCulture()->GetTourism() > 0 && !GET_PLAYER(kTradeConnection.m_eDestOwner).isMinorCiv())
	{
		// if we're not connected to a player, double the value to that player
		if (!GC.getGame().GetGameTrade()->IsPlayerConnectedToPlayer(m_pPlayer->GetID(), kTradeConnection.m_eDestOwner))
		{
			iScore *= 2;
		}
	}

	return iScore;
}

/// Score Food TR
#ifdef AUI_CONSTIFY
int CvTradeAI::ScoreFoodTR(const TradeConnection& kTradeConnection, const CvCity* pSmallestCity) const
#else
int CvTradeAI::ScoreFoodTR (const TradeConnection& kTradeConnection, CvCity* pSmallestCity)
#endif
{
	if (kTradeConnection.m_eConnectionType != TRADE_CONNECTION_FOOD)
	{
		return 0;
	}

	// if this was recently plundered, 0 the score
	if (m_pPlayer->GetTrade()->CheckTradeConnectionWasPlundered(kTradeConnection))
	{
		return 0;
	}

	// if we're not going to the smallest city, ignore
	if (kTradeConnection.m_iDestX != pSmallestCity->getX() && kTradeConnection.m_iDestY != pSmallestCity->getY())
	{
		return 0;
	}

	int iMaxDistance = 60;
	int iDistance = kTradeConnection.m_aPlotList.size();
	int iDistanceScore = iMaxDistance - iDistance;
	if (kTradeConnection.m_eDomain == DOMAIN_SEA)
	{
		iDistanceScore = iDistanceScore * 2;
	}

	//CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	int iDangerSum = 1; // can't be zero because we divide by zero!
	for (uint uiPlotList = 0; uiPlotList < kTradeConnection.m_aPlotList.size(); uiPlotList++)
	{
		CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_aPlotList[uiPlotList].m_iX, kTradeConnection.m_aPlotList[uiPlotList].m_iY);
		CvAssertMsg(pPlot, "pPlot is null when trying to evaluate the list");
		if (pPlot == NULL)
		{
			break;
		}
		int iDangerValue = m_pPlayer->GetPlotDanger(*pPlot);
		if (iDangerValue == 0)
		{
			if (!pPlot->isVisible(m_pPlayer->getTeam()))
			{
				iDangerValue += 1;
			}

			if (pPlot->getTeam() != NO_TEAM && GET_TEAM(m_pPlayer->getTeam()).isAtWar(pPlot->getTeam()))
			{
				iDangerValue += 1000;
			}
		}
		iDangerSum += iDangerValue;
	}

	return iDistanceScore - iDangerSum;
}

/// Score Production TR
#ifdef AUI_CONSTIFY
int CvTradeAI::ScoreProductionTR(const TradeConnection& kTradeConnection, std::vector<const CvCity*>& aTargetCityList) const
#else
int CvTradeAI::ScoreProductionTR (const TradeConnection& kTradeConnection, std::vector<CvCity*> aTargetCityList)
#endif
{
	// only consider production trades
	if (kTradeConnection.m_eConnectionType != TRADE_CONNECTION_PRODUCTION)
	{
		return 0;
	}

	// if this was recently plundered, 0 the score
	if (m_pPlayer->GetTrade()->CheckTradeConnectionWasPlundered(kTradeConnection))
	{
		return 0;
	}

	// if we're not going to a target production city, ignore
	bool bValidTarget = false;
#ifdef AUI_ITERATORIZE
	for (std::vector<const CvCity*>::iterator it = aTargetCityList.begin(); it != aTargetCityList.end(); ++it)
	{
		if (kTradeConnection.m_iDestX == (*it)->getX() && kTradeConnection.m_iDestY == (*it)->getY())
#else
	for (uint ui = 0; ui < aTargetCityList.size(); ui++)
	{
		if (kTradeConnection.m_iDestX == aTargetCityList[ui]->getX() && kTradeConnection.m_iDestY == aTargetCityList[ui]->getY())
#endif
		{
			bValidTarget = true;
			break;
		}
	}

	if (!bValidTarget)
	{
		return 0;
	}

	int iMaxDistance = 60;
	int iDistance = kTradeConnection.m_aPlotList.size();
	int iDistanceScore = iMaxDistance - iDistance;
	if (kTradeConnection.m_eDomain == DOMAIN_SEA)
	{
		iDistanceScore = iDistanceScore * 2;
	}

	//CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	int iDangerSum = 1; // can't be zero because we divide by zero!
	for (uint uiPlotList = 0; uiPlotList < kTradeConnection.m_aPlotList.size(); uiPlotList++)
	{
		CvPlot* pPlot = GC.getMap().plot(kTradeConnection.m_aPlotList[uiPlotList].m_iX, kTradeConnection.m_aPlotList[uiPlotList].m_iY);
		CvAssertMsg(pPlot, "pPlot is null when trying to evaluate the list");
		if (pPlot == NULL)
		{
			break;
		}
		int iDangerValue = m_pPlayer->GetPlotDanger(*pPlot);
		if (iDangerValue == 0)
		{
			if (!pPlot->isVisible(m_pPlayer->getTeam()))
			{
				iDangerValue += 1;
			}

			if (pPlot->getTeam() != NO_TEAM && GET_TEAM(m_pPlayer->getTeam()).isAtWar(pPlot->getTeam()))
			{
				iDangerValue += 1000;
			}
		}
		iDangerSum += iDangerValue;
	}

	return iDistanceScore - iDangerSum;
}

// sort player numbers
struct TRSortElement
{
	TradeConnection m_kTradeConnection;
	int m_iScore;
};

struct SortTR
{
	bool operator()(TRSortElement const& a, TRSortElement const& b) const
	{
		return a.m_iScore < b.m_iScore;
	}
};

/// Prioritize TRs
void CvTradeAI::PrioritizeTradeRoutes(TradeConnectionList& aTradeConnectionList)
{	
	//CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	GetAvailableTR(aTradeConnectionList);

	// if the list is empty, bail
	if (aTradeConnectionList.size() == 0)
	{
		return;
	}

	// score TR
	std::vector<TRSortElement> aProductionSortedTR;
	std::vector<TRSortElement> aFoodSortedTR;
	std::vector<TRSortElement> aGoldSortedTR;

	// FOOD FOOD FOOD FOOD
	if (m_pPlayer->GetHappiness() >= 0)
	{
		// - Find smallest city
		int iSmallestCitySize = MAX_INT;
		CvCity* pSmallestCity = NULL;
		int iCityLoop;
		CvCity* pCity = NULL;
		for (pCity = m_pPlayer->firstCity(&iCityLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iCityLoop))
		{
			int iCitySize = pCity->getPopulation();
			if (iCitySize < iSmallestCitySize)
			{
				pSmallestCity = pCity;
				iSmallestCitySize = iCitySize;
			}
		}

		// - Send tr there
		// if there is a smallest city, score according to that
		if (pSmallestCity)
		{
			aFoodSortedTR.clear();
#ifdef AUI_ITERATORIZE
			for (TradeConnectionList::iterator it = aTradeConnectionList.begin(); it != aTradeConnectionList.end(); ++it)
			{
				TRSortElement kElement;
				kElement.m_kTradeConnection = *it;
				kElement.m_iScore = ScoreFoodTR(*it, pSmallestCity);
#else
			for (uint ui = 0; ui < aTradeConnectionList.size(); ui++)
			{
				TRSortElement kElement;
				kElement.m_kTradeConnection = aTradeConnectionList[ui];
				kElement.m_iScore = ScoreFoodTR(aTradeConnectionList[ui], pSmallestCity);
#endif
				if (kElement.m_iScore > 0)
				{
					aFoodSortedTR.push_back(kElement);
				}
			}

			std::stable_sort(aFoodSortedTR.begin(), aFoodSortedTR.end(), SortTR());
		}
	}

	// PRODUCTION PRODUCTION PRODUCTION PRODUCTION
	// - Search for wonder city
	// - Search for spaceship city
#ifdef AUI_CONSTIFY
	std::vector<const CvCity*> apProductionTargetCities;
#else
	std::vector<CvCity*> apProductionTargetCities;
#endif
	CvCity* pWonderCity = m_pPlayer->GetCitySpecializationAI()->GetWonderBuildCity();
	if (pWonderCity)
	{
		apProductionTargetCities.push_back(pWonderCity);
	}
	CvCity* pCity = NULL;
	int iCityLoop;
	for (pCity = m_pPlayer->firstCity(&iCityLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iCityLoop))
	{
		UnitTypes eUnit = pCity->getProductionUnit();
		if (eUnit != -1)
		{
			CvUnitEntry *pkUnit = GC.GetGameUnits()->GetEntry(eUnit);
			if (pkUnit)
			{
				if (pkUnit->GetSpaceshipProject() != NO_PROJECT)
				{
					apProductionTargetCities.push_back(pCity);
				}
			}
		}
	}
	if (apProductionTargetCities.size() > 0)
	{
		aProductionSortedTR.clear();
#ifdef AUI_ITERATORIZE
		for (TradeConnectionList::iterator it = aTradeConnectionList.begin(); it != aTradeConnectionList.end(); ++it)
		{
			TRSortElement kElement;
			kElement.m_kTradeConnection = *it;
			kElement.m_iScore = ScoreProductionTR(*it, apProductionTargetCities);
#else
		for (uint ui = 0; ui < aTradeConnectionList.size(); ui++)
		{
			TRSortElement kElement;
			kElement.m_kTradeConnection = aTradeConnectionList[ui];
			kElement.m_iScore = ScoreProductionTR(aTradeConnectionList[ui], apProductionTargetCities);
#endif
			if (kElement.m_iScore > 0)
			{
				aProductionSortedTR.push_back(kElement);
			}
		}
		std::stable_sort(aProductionSortedTR.begin(), aProductionSortedTR.end(), SortTR());
	}

	// GOLD GOLD GOLD GOLD
	aGoldSortedTR.clear();
#ifdef AUI_ITERATORIZE
	for (TradeConnectionList::iterator it = aTradeConnectionList.begin(); it != aTradeConnectionList.end(); ++it)
	{
		TRSortElement kElement;
		kElement.m_kTradeConnection = *it;
		kElement.m_iScore = ScoreInternationalTR(*it);
#else
	for (uint ui = 0; ui < aTradeConnectionList.size(); ui++)
	{
		TRSortElement kElement;
		kElement.m_kTradeConnection = aTradeConnectionList[ui];
		kElement.m_iScore = ScoreInternationalTR(aTradeConnectionList[ui]);
#endif
		if (kElement.m_iScore > 0)
		{
			aGoldSortedTR.push_back(kElement);
		}
	}
	std::stable_sort(aGoldSortedTR.begin(), aGoldSortedTR.end(), SortTR());
	uint uiNumInternationalTradesToAdd = 2;

	// clear list
	aTradeConnectionList.clear();

	// add all the gold trs
#ifdef AUI_ITERATORIZE
	std::vector<TRSortElement>::iterator it;
	uint ui = 0;
	for (it = aGoldSortedTR.begin(); ui < uiNumInternationalTradesToAdd && it < aGoldSortedTR.end(); ++it)
	{
		aTradeConnectionList.push_back(it->m_kTradeConnection);
		ui++;
#else
	for (uint ui = 0; ui < uiNumInternationalTradesToAdd && ui < aGoldSortedTR.size(); ui++)
	{
		aTradeConnectionList.push_back(aGoldSortedTR[ui].m_kTradeConnection);
#endif
	}

	// add first food
	if (aFoodSortedTR.size() > 0) 
	{
#ifdef AUI_ITERATORIZE
		aTradeConnectionList.push_back(aFoodSortedTR.begin()->m_kTradeConnection);
#else
		aTradeConnectionList.push_back(aFoodSortedTR[0].m_kTradeConnection);
#endif
	}
	
	// add first production tr
	if (aProductionSortedTR.size() > 0)
	{
#ifdef AUI_ITERATORIZE
		aTradeConnectionList.push_back(aProductionSortedTR.begin()->m_kTradeConnection);
#else
		aTradeConnectionList.push_back(aProductionSortedTR[0].m_kTradeConnection);
#endif
	}

	// add all the gold trs
#ifdef AUI_ITERATORIZE
	for (;it < aGoldSortedTR.end(); ++it)
	{
		aTradeConnectionList.push_back(it->m_kTradeConnection);
#else
	for (uint ui = uiNumInternationalTradesToAdd; ui < aGoldSortedTR.size(); ui++)
	{
		aTradeConnectionList.push_back(aGoldSortedTR[ui].m_kTradeConnection);
#endif
	}
}

/// ChooseTradeUnitTargetPlot
#ifdef AUI_CONSTIFY
bool CvTradeAI::ChooseTradeUnitTargetPlot(CvUnit* pUnit, int& iOriginPlotIndex, int& iDestPlotIndex, TradeConnectionType& eTradeConnectionType, bool& bDisband, const TradeConnectionList& aTradeConnections) const
#else
bool CvTradeAI::ChooseTradeUnitTargetPlot(CvUnit* pUnit, int& iOriginPlotIndex, int& iDestPlotIndex, TradeConnectionType& eTradeConnectionType, bool& bDisband, const TradeConnectionList& aTradeConnections)
#endif
{
	// clearing the returned data
	iOriginPlotIndex = MAX_INT;
	iDestPlotIndex = MAX_INT;
	eTradeConnectionType = NUM_TRADE_CONNECTION_TYPES;
	bDisband = false;

	CvCity* pOriginCity = pUnit->plot()->getPlotCity();
	CvAssertMsg(pOriginCity, "pOriginCity is null! Whaa?");
	if (!pOriginCity)
	{
		return false;
	}

	CvPlayerTrade* pkTradeAI = m_pPlayer->GetTrade();
	//CvPlayerTrade* pPlayerTrade = m_pPlayer->GetTrade();
	uint uiNumDuplicateTRs = 0;
	uint uiNumTradeRoutesPossible =  pkTradeAI->GetNumTradeRoutesPossible();
#ifdef AUI_ITERATORIZE
	const TradeConnectionList::const_iterator pNoMoreTradeRoutesPossible = aTradeConnections.begin() + uiNumTradeRoutesPossible;
	for (TradeConnectionList::const_iterator it = aTradeConnections.begin(); it != aTradeConnections.end() && it < pNoMoreTradeRoutesPossible; ++it)
	{
		for (TradeConnectionList::const_iterator it2 = it + 1; it2 != aTradeConnections.end() && it2 < pNoMoreTradeRoutesPossible; ++it2)
		{
			if (it->m_iOriginX == it2->m_iOriginX &&
				it->m_iOriginY == it2->m_iOriginY &&
				it->m_iDestX == it2->m_iDestX &&
				it->m_iDestY == it2->m_iDestY &&
				it->m_eConnectionType == it2->m_eConnectionType)
#else
	uint uiNumTradeConnections = aTradeConnections.size();

	for (uint ui = 0; ui < uiNumTradeRoutesPossible && ui < uiNumTradeConnections; ui++)
	{
		for (uint ui2 = ui + 1;  ui2 < uiNumTradeRoutesPossible && ui2 < uiNumTradeConnections; ui2++)
		{
			if (aTradeConnections[ui].m_iOriginX == aTradeConnections[ui2].m_iOriginX && 
				aTradeConnections[ui].m_iOriginY == aTradeConnections[ui2].m_iOriginY && 
				aTradeConnections[ui].m_iDestX   == aTradeConnections[ui2].m_iDestX && 
				aTradeConnections[ui].m_iDestY   == aTradeConnections[ui2].m_iDestY && 
				aTradeConnections[ui].m_eConnectionType == aTradeConnections[ui2].m_eConnectionType)
#endif
			{
				uiNumDuplicateTRs++;
			}
		}
	}

#ifdef AUI_ITERATORIZE
	const TradeConnectionList::const_iterator pDisbandThreshold = aTradeConnections.begin() + uiNumTradeRoutesPossible + uiNumDuplicateTRs;
	for (TradeConnectionList::const_iterator it = aTradeConnections.begin(); it != aTradeConnections.end() && it < pNoMoreTradeRoutesPossible; ++it)
	{
		if (it >= pDisbandThreshold)
		{
			if ((uiNumTradeRoutesPossible - pkTradeAI->GetNumTradeRoutesUsed(true)) == 0) // GetNumTradeRoutesRemaining(true)
			{
				bDisband = true;
				return true;
			}
		}

		if (it->m_eDomain == pUnit->getDomainType())
		{
			CvPlot* pPlot = GC.getMap().plot(it->m_iOriginX, it->m_iOriginY);
			if (pUnit->canMakeTradeRouteAt(pPlot, it->m_iDestX, it->m_iDestY, it->m_eConnectionType))
			{
				CvPlot* pDestPlot = GC.getMap().plot(it->m_iDestX, it->m_iDestY);
				iOriginPlotIndex = pPlot->GetPlotIndex();
				iDestPlotIndex = pDestPlot->GetPlotIndex();
				eTradeConnectionType = it->m_eConnectionType;
#else
	for (uint ui = 0; ui < aTradeConnections.size(); ui++)
	{
		if (ui >= (uiNumTradeRoutesPossible + uiNumDuplicateTRs))
		{
			if ((uiNumTradeRoutesPossible - pkTradeAI->GetNumTradeRoutesUsed(true)) == 0) // GetNumTradeRoutesRemaining(true)
			{
				bDisband = true;
				return true;
			}
		}

		if (aTradeConnections[ui].m_eDomain == pUnit->getDomainType())
		{
			CvPlot* pPlot = GC.getMap().plot(aTradeConnections[ui].m_iOriginX, aTradeConnections[ui].m_iOriginY);
			if (pUnit->canMakeTradeRouteAt(pPlot, aTradeConnections[ui].m_iDestX, aTradeConnections[ui].m_iDestY, aTradeConnections[ui].m_eConnectionType))
			{
				CvPlot* pDestPlot = GC.getMap().plot(aTradeConnections[ui].m_iDestX, aTradeConnections[ui].m_iDestY);
				iOriginPlotIndex = pPlot->GetPlotIndex();
				iDestPlotIndex = pDestPlot->GetPlotIndex();
				eTradeConnectionType = aTradeConnections[ui].m_eConnectionType;
#endif
				return true;
			}
		}
	}

	return false;
}


/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvTradeAI& writeTo)
{
	uint uiVersion;
	loadFrom >> uiVersion;

	loadFrom >> writeTo.m_iRemovableValue;

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvTradeAI& readFrom)
{
	uint uiVersion = 0;
	saveTo << uiVersion;

	saveTo << readFrom.m_iRemovableValue;

	return saveTo;
}