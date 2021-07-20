/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_TRADE_CLASSES_H
#define CIV5_TRADE_CLASSES_H

// The map layer the trade units reside on
#define TRADE_UNIT_MAP_LAYER	1

struct TradeConnectionPlot
{
	int m_iX;
	int m_iY;
};

#define PROJECTED_MAX_TRADE_LENGTH 40
#ifdef AUI_TRADE_FIX_FSTATICVECTOR_CONTENTS_ARE_POD
typedef FStaticVector<TradeConnectionPlot, PROJECTED_MAX_TRADE_LENGTH, true, c_eCiv5GameplayDLL> TradeConnectionPlotList;
#else
typedef FStaticVector<TradeConnectionPlot, PROJECTED_MAX_TRADE_LENGTH, false, c_eCiv5GameplayDLL> TradeConnectionPlotList;
#endif

struct TradeConnection
{
	int m_iID;
	int m_iOriginX;
	int m_iOriginY;
	int m_iDestX;
	int m_iDestY;
	PlayerTypes m_eOriginOwner;
	PlayerTypes m_eDestOwner;
	DomainTypes m_eDomain;
	TradeConnectionType m_eConnectionType;
#ifdef AUI_WARNING_FIXES
	uint m_iTradeUnitLocationIndex;
#else
	int m_iTradeUnitLocationIndex;
#endif
	bool m_bTradeUnitMovingForward;
	TradeConnectionPlotList m_aPlotList;
	int m_unitID;
	int m_iCircuitsCompleted;
	int m_iCircuitsToComplete;
	int m_iTurnRouteComplete;
	int m_aiOriginYields[NUM_YIELD_TYPES];
	int m_aiDestYields[NUM_YIELD_TYPES];

#ifdef AUI_EXPLICIT_DESTRUCTION
	~TradeConnection()
	{
		m_aPlotList.clear();
	}
#endif
};

#define PROJECTED_MAX_TRADE_CONNECTIONS_PER_CIV 14
#define LIKELY_NUM_OF_PLAYERS 12
#define PROJECTED_MAX_TRADE_CONNECTIONS (PROJECTED_MAX_TRADE_CONNECTIONS_PER_CIV * LIKELY_NUM_OF_PLAYERS)

typedef FStaticVector<TradeConnection, PROJECTED_MAX_TRADE_CONNECTIONS, false, c_eCiv5GameplayDLL > TradeConnectionList;

class CvGameTrade
{
public:
	CvGameTrade(void);
	~CvGameTrade(void);

	void Init (void);
	void Uninit (void);
	void Reset (void);

#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	void DoTurn(PlayerTypes eForPlayer = NO_PLAYER);
#else
	void DoTurn (void);
#endif

#ifdef AUI_CONSTIFY
	bool CanCreateTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath = true) const;
	bool CanCreateTradeRoute(PlayerTypes eOriginPlayer, PlayerTypes eDestPlayer, DomainTypes eDomainRestriction) const;
#else
	bool CanCreateTradeRoute (CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath = true);
	bool CanCreateTradeRoute(PlayerTypes eOriginPlayer, PlayerTypes eDestPlayer, DomainTypes eDomainRestriction);
#endif
	bool CreateTradeRoute (CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, int& iRouteID);

#ifdef AUI_CONSTIFY
	bool IsValidTradeRoutePath(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain) const;
	CvPlot* GetPlotAdjacentToWater(const CvPlot* pTarget, const CvPlot* pOrigin) const;

	bool IsDestinationExclusive(const TradeConnection& kTradeConnection) const;
	bool IsConnectionInternational(const TradeConnection& kTradeConnection) const;

	bool IsCityConnectedToPlayer(const CvCity* pCity, PlayerTypes eOtherPlayer, bool bOnlyOwnedByCityOwner) const;
	bool IsPlayerConnectedToPlayer(PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer) const;
	int CountNumPlayerConnectionsToPlayer(PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer) const;

	bool IsCityConnectedToCity(const CvCity* pFirstCity, const CvCity* pSecondCity) const;
	bool IsCityConnectedFromCityToCity(const CvCity* pOriginCity, const CvCity* pDestCity) const;

	int GetNumTimesOriginCity(const CvCity* pCity, bool bOnlyInternational) const;
	int GetNumTimesDestinationCity(const CvCity* pCity, bool bOnlyInternational) const;

	void CopyPathIntoTradeConnection(const CvAStarNode* pNode, TradeConnection* pTradeConnection) const;

	int GetDomainModifierTimes100(DomainTypes eDomain) const;

	int GetEmptyTradeRouteIndex() const;
	bool IsTradeRouteIndexEmpty(int iIndex) const;
#else
	bool IsValidTradeRoutePath (CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain);
	CvPlot* GetPlotAdjacentToWater (CvPlot* pTarget, CvPlot* pOrigin);

	bool IsDestinationExclusive(const TradeConnection& kTradeConnection);
	bool IsConnectionInternational (const TradeConnection& kTradeConnection);

	bool IsCityConnectedToPlayer (CvCity* pCity, PlayerTypes eOtherPlayer, bool bOnlyOwnedByCityOwner);
	bool IsPlayerConnectedToPlayer (PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer);
	int CountNumPlayerConnectionsToPlayer (PlayerTypes eFirstPlayer, PlayerTypes eSecondPlayer);

	bool IsCityConnectedToCity (CvCity* pFirstCity, CvCity* pSecondCity);
	bool IsCityConnectedFromCityToCity (CvCity* pOriginCity, CvCity* pDestCity);

	int GetNumTimesOriginCity (CvCity* pCity, bool bOnlyInternational);
	int GetNumTimesDestinationCity (CvCity* pCity, bool bOnlyInternational);

	void CopyPathIntoTradeConnection (CvAStarNode* pNode, TradeConnection* pTradeConnection);

	int GetDomainModifierTimes100 (DomainTypes eDomain);

	int GetEmptyTradeRouteIndex (void);
	bool IsTradeRouteIndexEmpty (int iIndex);
#endif
	bool EmptyTradeRoute (int iIndex);
#ifdef AUI_ITERATORIZE
	TradeConnectionList::iterator GetEmptyTradeRouteIter();
	TradeConnectionList::const_iterator GetEmptyTradeRouteIter() const;
	bool IsTradeRouteIndexEmpty(const TradeConnectionList::iterator it) const;
	bool IsTradeRouteIndexEmpty(const TradeConnectionList::const_iterator it) const;
#endif

	void ClearAllCityTradeRoutes (CvPlot* pPlot); // called when a city is captured or traded
	void ClearAllCivTradeRoutes (PlayerTypes ePlayer); // called from world congress code
	void ClearAllCityStateTradeRoutes (void); // called from world congress code
#ifdef NQM_TEAM_TRADE_ROUTES_CANCELLED_NOT_DESTROYED_FOR_WAR_DEFENDER_ON_DOW
	void CancelTradeBetweenTeams(TeamTypes eTeam1, TeamTypes eTeam2, bool bReturnUnits = false);
	void InvalidateTradeBetweenTeams(TeamTypes eOriginTeam, TeamTypes eDestinationTeam);
#else
	void CancelTradeBetweenTeams (TeamTypes eTeam1, TeamTypes eTeam2);
#endif

	void DoAutoWarPlundering(TeamTypes eTeam1, TeamTypes eTeam2); // when war is declared, both sides plunder each others trade routes for cash!

#ifdef AUI_CONSTIFY
	int GetNumTradeRoutesInPlot(const CvPlot* pPlot) const;

	int GetIndexFromID(int iID) const;
	PlayerTypes GetOwnerFromID(int iID) const;
	PlayerTypes GetDestFromID(int iID) const;

	int GetIndexFromUnitID(int iUnitID, PlayerTypes eOwner) const;
	bool IsUnitIDUsed(int iUnitID) const;
#else
	int GetNumTradeRoutesInPlot (CvPlot* pPlot);

	int GetIndexFromID (int iID);
	PlayerTypes GetOwnerFromID (int iID);
	PlayerTypes GetDestFromID (int iID);
#ifdef NQ_UNIT_IMMUNE_TO_PLUNDER_FROM_TRAIT
	DomainTypes GetDomainFromID (int iID);
#endif
	
	int GetIndexFromUnitID(int iUnitID, PlayerTypes eOwner);
	bool IsUnitIDUsed (int iUnitID);
#endif

	static CvCity* GetOriginCity(const TradeConnection& kTradeConnection);
	static CvCity* GetDestCity(const TradeConnection& kTradeConnection);

#ifdef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	void ResetTechDifference(PlayerTypes eForPlayer = NO_PLAYER);
	void BuildTechDifference(PlayerTypes eForPlayer = NO_PLAYER);
#else
	void ResetTechDifference ();
	void BuildTechDifference ();
#endif
#ifdef AUI_CONSTIFY
	int GetTechDifference(PlayerTypes ePlayer, PlayerTypes ePlayer2) const;
#else
	int GetTechDifference (PlayerTypes ePlayer, PlayerTypes ePlayer2);
#endif

	void CreateVis (int iIndex); // Create the trade unit vis unit
#ifdef AUI_WARNING_FIXES
	CvUnit* GetVis(uint iIndex) const;
#else
	CvUnit* GetVis(int iIndex);
#endif
	// trade unit movement
	bool MoveUnit (int iIndex); // move a trade unit along its path for all its movement points
	bool StepUnit (int iIndex); // move a trade unit a single step along its path (called by MoveUnit)
#ifdef AUI_ITERATORIZE
	bool MoveUnit(TradeConnectionList::iterator it); // move a trade unit along its path for all its movement points
	bool StepUnit(TradeConnectionList::iterator it); // move a trade unit a single step along its path (called by MoveUnit)
#endif

	void DisplayTemporaryPopupTradeRoute(int iPlotX, int iPlotY, TradeConnectionType type, DomainTypes eDomain);
	void HideTemporaryPopupTradeRoute(int iPlotX, int iPlotY, TradeConnectionType type);

	CvString GetLogFileName() const;
	void LogTradeMsg(CvString& strMsg);

	TradeConnectionList m_aTradeConnections;
	int m_iNextID; // used to assign IDs to trade routes to avoid confusion when some are disrupted in multiplayer

	int m_aaiTechDifference[MAX_MAJOR_CIVS][MAX_MAJOR_CIVS];

	struct {
		int iPlotX, iPlotY;
		TradeConnectionType type;
	} m_CurrentTemporaryPopupRoute;
};

FDataStream& operator>>(FDataStream&, CvGameTrade&);
FDataStream& operator<<(FDataStream&, const CvGameTrade&);

struct TradeConnectionWasPlundered
{
	TradeConnection m_kTradeConnection;
	int m_iTurnPlundered;
};

#ifdef AUI_TRADE_FIX_POSSIBLE_DEALLOCATION_CRASH
typedef FStaticVector<TradeConnectionWasPlundered, PROJECTED_MAX_TRADE_CONNECTIONS_PER_CIV, false, c_eCiv5GameplayDLL> TradeConnectionWasPlunderedList;
#else
typedef FStaticVector<TradeConnectionWasPlundered, 10, false, c_eCiv5GameplayDLL > TradeConnectionWasPlunderedList;
#endif

class CvPlayerTrade
{
public:
	CvPlayerTrade(void);
	~CvPlayerTrade(void);
	void Init(CvPlayer* pPlayer);
	void Uninit(void);
	void Reset(void);

	// Functions invoked each player turn
	void DoTurn(void);
	void MoveUnits(void);

#ifdef AUI_CONSTIFY
	int GetTradeConnectionBaseValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionGPTValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer, bool bOriginCity) const;
	int GetTradeConnectionResourceValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionYourBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionTheirBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionExclusiveValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const;
	int GetTradeConnectionPolicyValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const;
	int GetTradeConnectionOtherTraitValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionDomainValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield) const;
	int GetTradeConnectionRiverValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
	int GetTradeConnectionValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer) const;
#else
	int GetTradeConnectionBaseValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionGPTValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer, bool bOriginCity);
	int GetTradeConnectionResourceValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionYourBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionTheirBuildingValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionExclusiveValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield);
	int GetTradeConnectionPolicyValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield);
	int GetTradeConnectionOtherTraitValueTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionDomainValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield);
	int GetTradeConnectionRiverValueModifierTimes100(const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
	int GetTradeConnectionValueTimes100 (const TradeConnection& kTradeConnection, YieldTypes eYield, bool bAsOriginPlayer);
#endif
	void UpdateTradeConnectionValues (void); // updates the all the values for the trade routes that go to and from this player

#ifdef AUI_CONSTIFY
	int GetTradeValuesAtCityTimes100(const CvCity* const pCity, YieldTypes eYield) const;

	int GetAllTradeValueTimes100(YieldTypes eYield) const;
	int GetAllTradeValueFromPlayerTimes100(YieldTypes eYield, PlayerTypes ePlayer) const;

	bool IsConnectedToPlayer(PlayerTypes eOtherPlayer) const;

	bool CanCreateTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath = true) const;
	bool CanCreateTradeRoute(PlayerTypes eOtherPlayer, DomainTypes eDomain) const;
	bool CanCreateTradeRoute(DomainTypes eDomain) const;
#else
	int GetTradeValuesAtCityTimes100 (const CvCity* const pCity, YieldTypes eYield);

	int GetAllTradeValueTimes100 (YieldTypes eYield);
	int GetAllTradeValueFromPlayerTimes100 (YieldTypes eYield, PlayerTypes ePlayer);

	bool IsConnectedToPlayer(PlayerTypes eOtherPlayer);

	bool CanCreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType, bool bIgnoreExisting, bool bCheckPath = true);
	bool CanCreateTradeRoute(PlayerTypes eOtherPlayer, DomainTypes eDomain);
	bool CanCreateTradeRoute(DomainTypes eDomain);
#endif

	bool CreateTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType);

#ifdef AUI_CONSTIFY
	TradeConnection* GetTradeConnection(const CvCity* pOriginCity, const CvCity* pDestCity);
	int GetNumberOfCityStateTradeRoutes() const;

	bool IsPreviousTradeRoute(const CvCity* pOriginCity, const CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType) const;

	int GetNumPotentialConnections(const CvCity* pFromCity, DomainTypes eDomain) const;

	std::vector<int> GetTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar) const;
	std::vector<int> GetTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar) const;

	std::vector<int> GetOpposingTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const; // returns the ID of trade connections with units at that plot
	bool ContainsOpposingPlayerTradeUnit(const CvPlot* pPlot) const;

	std::vector<int> GetEnemyTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const; // returns the ID of trade connections with units at that plot
	bool ContainsEnemyTradeUnit(const CvPlot* pPlot) const;
	std::vector<int> GetEnemyTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound) const; // returns the ID of trade connections that go through that plot
	bool ContainsEnemyTradePlot(const CvPlot* pPlot) const;
#else
	TradeConnection* GetTradeConnection(CvCity* pOriginCity, CvCity* pDestCity);
	int GetNumberOfCityStateTradeRoutes();

	bool IsPreviousTradeRoute(CvCity* pOriginCity, CvCity* pDestCity, DomainTypes eDomain, TradeConnectionType eConnectionType);

	int GetNumPotentialConnections (CvCity* pFromCity, DomainTypes eDomain);

	std::vector<int> GetTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar);
	std::vector<int> GetTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound, bool bExcludingMe, bool bOnlyWar);

	std::vector<int> GetOpposingTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound); // returns the ID of trade connections with units at that plot
	bool ContainsOpposingPlayerTradeUnit(const CvPlot* pPlot);

	std::vector<int> GetEnemyTradeUnitsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound); // returns the ID of trade connections with units at that plot
	bool ContainsEnemyTradeUnit(const CvPlot* pPlot);
	std::vector<int> GetEnemyTradePlotsAtPlot(const CvPlot* pPlot, bool bFailAtFirstFound); // returns the ID of trade connections that go through that plot
	bool ContainsEnemyTradePlot(const CvPlot* pPlot);
#endif

	bool PlunderTradeRoute(int iTradeConnectionID);

#ifdef AUI_CONSTIFY
	int GetTradeRouteRange(DomainTypes eDomain, const CvCity* pOriginCity) const;
	int GetTradeRouteSpeed(DomainTypes eDomain) const;

	uint GetNumTradeRoutesPossible() const;
	int GetNumTradeRoutesUsed(bool bContinueTraining) const;
	int GetNumTradeRoutesRemaining(bool bContinueTraining) const;

	int GetNumDifferentTradingPartners() const;
#else
	int GetTradeRouteRange (DomainTypes eDomain, CvCity* pOriginCity);
	int GetTradeRouteSpeed (DomainTypes eDomain);

	uint GetNumTradeRoutesPossible (void);
	int GetNumTradeRoutesUsed (bool bContinueTraining);
	int GetNumTradeRoutesRemaining (bool bContinueTraining);

	int GetNumDifferentTradingPartners (void);
#ifdef NQ_FAITH_PER_FOREIGN_TRADE_ROUTE
	int GetNumForeignTradeRoutes(PlayerTypes ePlayer);
#endif
#endif

	void UpdateTradeConnectionWasPlundered();
#ifdef AUI_WARNING_FIXES
	void AddTradeConnectionWasPlundered(const TradeConnection& kTradeConnection);
#else
	void AddTradeConnectionWasPlundered(const TradeConnection kTradeConnection);
#endif
#ifdef AUI_CONSTIFY
	bool CheckTradeConnectionWasPlundered(const TradeConnection& kTradeConnection) const;
#else
	bool CheckTradeConnectionWasPlundered(const TradeConnection& kTradeConnection);
#endif

	static UnitTypes GetTradeUnit (DomainTypes eDomain);

#ifdef AUI_CONSTIFY
	std::vector<CvString> GetPlotToolTips(CvPlot* pPlot) const;
	std::vector<CvString> GetPlotMouseoverToolTips(CvPlot* pPlot) const;
#else
	std::vector<CvString> GetPlotToolTips (CvPlot* pPlot);
	std::vector<CvString> GetPlotMouseoverToolTips (CvPlot* pPlot);
#endif

	TradeConnectionList m_aRecentlyExpiredConnections;
	TradeConnectionWasPlunderedList m_aTradeConnectionWasPlundered;

	CvPlayer* m_pPlayer;
};

FDataStream& operator>>(FDataStream&, CvPlayerTrade&);
FDataStream& operator<<(FDataStream&, const CvPlayerTrade&);

class CvTradeAI
{
public:
	CvTradeAI(void);
	~CvTradeAI(void);
	void Init(CvPlayer* pPlayer);
	void Uninit(void);
	void Reset(void);

	void DoTurn(void);

	void GetAvailableTR(TradeConnectionList& aTradeConnectionList);
	void PrioritizeTradeRoutes(TradeConnectionList& aTradeConnectionList);
#ifdef AUI_CONSTIFY
	int	ScoreInternationalTR(const TradeConnection& kTradeConnection) const;
	int ScoreFoodTR(const TradeConnection& kTradeConnection, const CvCity* pSmallestCity) const;
	int ScoreProductionTR(const TradeConnection& kTradeConnection, std::vector<const CvCity*>& aTargetCityList) const;

	bool ChooseTradeUnitTargetPlot(CvUnit* pUnit, int& iOriginPlotIndex, int& iDestPlotIndex, TradeConnectionType& eTradeConnectionType, bool& bDisband, const TradeConnectionList& aTradeConnections) const;
#else
	int	ScoreInternationalTR (const TradeConnection& kTradeConnection);
	int ScoreFoodTR(const TradeConnection& kTradeConnection, CvCity* pSmallestCity);
	int ScoreProductionTR (const TradeConnection& kTradeConnection, std::vector<CvCity*> aTargetCityList);

	bool ChooseTradeUnitTargetPlot(CvUnit* pUnit, int& iOriginPlotIndex, int& iDestPlotIndex, TradeConnectionType& eTradeConnectionType, bool& bDisband, const TradeConnectionList& aTradeConnections);
#endif

	int m_iRemovableValue;

	CvPlayer* m_pPlayer;
};

FDataStream& operator>>(FDataStream&, CvTradeAI&);
FDataStream& operator<<(FDataStream&, const CvTradeAI&);

#endif //CIV5_TRADE_CLASSES_H