/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

// game.h

#ifndef CIV5_GAME_H
#define CIV5_GAME_H

#include "CvRandom.h"
#include "FFastVector.h"
#include "FTimer.h"
#include "CvPreGame.h"
#include <CvLocalization.h>

#include "CvDealClasses.h"

class CvPlot;
class CvCity;
class CvReplayMessage;
#ifdef REPLAY_EVENTS
class CvReplayEvent;
#endif
class CvReplayInfo;
class CvSiteEvaluatorForSettler;
class CvSiteEvaluatorForStart;
class CvStartPositioner;
class CvGameReligions;
class CvGameCulture;
class CvGameLeagues;
class CvGameTrade;
class CvTacticalAnalysisMap;
#ifdef MP_PLAYERS_VOTING_SYSTEM
class CvMPVotingSystem;
#endif
class CvAdvisorCounsel;
class CvAdvisorRecommender;


class CvGameInitialItemsOverrides
{
public:
	CvGameInitialItemsOverrides();

	std::vector<bool> GrantInitialFreeTechsPerTeam;
	std::vector<bool> GrantInitialGoldPerPlayer;
	std::vector<bool> GrantInitialCulturePerPlayer;
	std::vector<bool> ClearResearchQueuePerPlayer;
	std::vector<bool> GrantInitialUnitsPerPlayer;
};

class CvGame
{
public:
	CvGame();
	~CvGame();

	void init(HandicapTypes eHandicap);
	bool init2();

	void InitPlayers();

	void reset(HandicapTypes eHandicap, bool bConstructorCall = false);
	void uninit();

	void DoGameStarted();

	void update();
	void updateScore(bool bForce = false);

	int GetMapScoreMod() const;

	void updateCitySight(bool bIncrement);
	void updateSelectionList();
	void updateTestEndTurn();

	CvUnit* getPlotUnit(CvPlot* pPlot, int iIndex);
	void getPlotUnits(CvPlot* pPlot, std::vector<CvUnit*>& plotUnits);

	void cycleCities(bool bForward = true, bool bAdd = false);
	void cycleUnits(bool bClear, bool bForward = true, bool bWorkers = false);
	bool cyclePlotUnits(CvPlot* pPlot, bool bForward = true, bool bAuto = false, int iCount = -1);

	void selectionListMove(CvPlot* pPlot, bool bShift);
	void selectionListGameNetMessage(int eMessage, int iData2 = -1, int iData3 = -1, int iData4 = -1, int iFlags = 0, bool bAlt = false, bool bShift = false);
	void selectedCitiesGameNetMessage(int eMessage, int iData2 = -1, int iData3 = -1, int iData4 = -1, bool bOption = false, bool bAlt = false, bool bShift = false, bool bCtrl = false);
	void cityPushOrder(CvCity* pCity, OrderTypes eOrder, int iData, bool bAlt = false, bool bShift = false, bool bCtrl = false);
	void CityPurchase(CvCity* pCity, UnitTypes eUnitType, BuildingTypes eBuildingType, ProjectTypes eProjectType, YieldTypes ePurchaseYield);

	void selectUnit(CvUnit* pUnit, bool bClear, bool bToggle = false, bool bSound = false);
	void mouseoverUnit(CvUnit* pUnit, bool bEnter);
	void selectGroup(CvUnit* pUnit, bool bShift, bool bCtrl, bool bAlt);
	void selectAll(CvPlot* pPlot);
	void SelectSettler(void);

	bool selectionListIgnoreBuildingDefense();

	bool canHandleAction(int iAction, CvPlot* pPlot = NULL, bool bTestVisible = false);
	void handleAction(int iAction);

	bool canDoControl(ControlTypes eControl);
	void doControl(ControlTypes eControl);

	int getProductionPerPopulation(HurryTypes eHurry);

	int getAdjustedPopulationPercent(VictoryTypes eVictory) const;
	int getAdjustedLandPercent(VictoryTypes eVictory) const;

	int countCivPlayersAlive() const;
	int countCivPlayersEverAlive() const;
	int countCivTeamsAlive() const;
	int countCivTeamsEverAlive() const;
	int countHumanPlayersAlive() const;
	int countHumanPlayersEverAlive() const;
#ifndef AUI_GAME_PLAYER_BASED_TURN_LENGTH
	int countSeqHumanTurnsUntilPlayerTurn(PlayerTypes playerID) const;
#endif

	int countMajorCivsAlive() const;
	int countMajorCivsEverAlive() const;
	int countTotalCivPower();
	int countTotalNukeUnits();
	int countKnownTechNumTeams(TechTypes eTech);

	int goldenAgeLength() const;
	int victoryDelay(VictoryTypes eVictory) const;

	int getImprovementUpgradeTimeMod(ImprovementTypes eImprovement, const CvPlot* pPlot = NULL) const;
	int getImprovementUpgradeTime(ImprovementTypes eImprovement, const CvPlot* pPlot = NULL) const;

	bool canTrainNukes() const;
	EraTypes getCurrentEra() const;

	TeamTypes getActiveTeam();
	CivilizationTypes getActiveCivilizationType();

	bool isNetworkMultiPlayer() const;
	bool isGameMultiPlayer() const;
	bool isTeamGame() const;

	void ReviveActivePlayer();
	bool CanMoveActivePlayerToObserver();
	void ActivateObserverSlot();

	int getNumHumanPlayers();
	int GetNumMinorCivsEver();
#ifdef AUI_GAME_BETTER_HYBRID_MODE
	int getCurrentTurnOrderActive() const;
#else
	int getNumHumansInHumanWars(PlayerTypes ignorePlayer = NO_PLAYER);
#endif
	int getNumSequentialHumans(PlayerTypes ignorePlayer = NO_PLAYER);

#if defined(AUI_WARNING_FIXES) || defined(AUI_CONSTIFY)
	int getGameTurn() const;
#else
	int getGameTurn();
#endif
	void setGameTurn(int iNewValue);
	void incrementGameTurn();
	int getTurnYear(int iGameTurn);
	int getGameTurnYear();

	int getElapsedGameTurns() const;
	void incrementElapsedGameTurns();

	int getMaxTurns() const;
	void setMaxTurns(int iNewValue);
	void changeMaxTurns(int iChange);

	int getMaxCityElimination() const;
	void setMaxCityElimination(int iNewValue);

	int getNumAdvancedStartPoints() const;
	void setNumAdvancedStartPoints(int iNewValue);

	int getStartTurn() const;
	void setStartTurn(int iNewValue);

	int GetWinningTurn() const;
	void SetWinningTurn(int iNewValue);

	int getStartYear() const;
	void setStartYear(int iNewValue);

	int getEstimateEndTurn() const;
	void setEstimateEndTurn(int iNewValue);
	int getDefaultEstimateEndTurn() const;
	void setDefaultEstimateEndTurn(int iNewValue);

	int getTurnSlice() const;
	int getMinutesPlayed() const;
	void setTurnSlice(int iNewValue);
	void changeTurnSlice(int iChange);

	void resetTurnTimer(bool resetGameTurnStart = true);
#ifndef AUI_GAME_PLAYER_BASED_TURN_LENGTH
	int getMaxTurnLen();
#endif

	bool IsStaticTutorialActive() const;
	void SetStaticTutorialActive(bool bStaticTutorialActive);

	bool HasAdvisorMessageBeenSeen(_In_z_ const char* szAdvisorMessageName);
	void SetAdvisorMessageHasBeenSeen(_In_z_ const char* szAdvisorMessageName, bool bSeen);

	bool IsCityScreenBlocked();
	bool CanOpenCityScreen(PlayerTypes eOpener, CvCity* pCity);

	int getTargetScore() const;
	void setTargetScore(int iNewValue);

#ifdef AUI_CONSTIFY
	int getNumGameTurnActive() const;
	int countNumHumanGameTurnActive() const;
#else
	int getNumGameTurnActive();
	int countNumHumanGameTurnActive();
#endif
#ifdef AUI_GAME_BETTER_HYBRID_MODE
	bool isNoPlayerActive() const;
#endif
	void changeNumGameTurnActive(int iChange, const std::string& why);

	int getNumCities() const;
	int getNumCivCities() const;
	void changeNumCities(int iChange);

	int getTotalPopulation() const;
	void changeTotalPopulation(int iChange);

	int getNoNukesCount() const;
	bool isNoNukes() const;
	void changeNoNukesCount(int iChange);

	int getNukesExploded() const;
	void changeNukesExploded(int iChange);

	int getMaxPopulation() const;
	int getInitPopulation() const;
	int getInitLand() const;
	int getInitTech() const;
	int getInitWonders() const;
	void initScoreCalculation();

	int getAIAutoPlay();
	void setAIAutoPlay(int iNewValue, PlayerTypes eReturnPlayer);
	void changeAIAutoPlay(int iChange);

	unsigned int getInitialTime();
	void setInitialTime(unsigned int uiNewValue);
#ifdef GAME_UPDATE_TURN_TIMER_ONCE_PER_TURN
	float getPreviousTurnLen();
	void setPreviousTurnLen(float fNewValue);
#endif
#ifdef TURN_TIMER_PAUSE_BUTTON
	float getTimeElapsed();
	void setTimeElapsed(float fNewValue);
#endif
#ifdef GAME_ALLOW_ONLY_ONE_UNIT_MOVE_ON_TURN_LOADING
	bool isMPOrderedMoveOnTurnLoading() const;
	void setMPOrderedMoveOnTurnLoading(bool bNewValue);
	bool getHasReceivedFirstMission();
	void setHasReceivedFirstMission(bool bNewValue);
#endif
	bool isScoreDirty() const;
	void setScoreDirty(bool bNewValue);

	bool isCircumnavigated() const;
	void makeCircumnavigated();
	bool circumnavigationAvailable() const;

	void DoFromUIDiploEvent(FromUIDiploEventTypes eEvent, PlayerTypes eAIPlayer, int iArg1, int iArg2);

	// Diplomacy Victory
	void DoInitDiploVictory();
	void DoUpdateDiploVictory();
	int GetVotesNeededForDiploVictory() const;
	void SetVotesNeededForDiploVictory(int iValue);
	bool IsUnitedNationsActive();

	// DEPRECATED
	int GetUnitedNationsCountdown() const;
	void SetUnitedNationsCountdown(int iValue);
	void ChangeUnitedNationsCountdown(int iChange);
	int GetNumVictoryVotesTallied() const;
	void SetNumVictoryVotesTallied(int iValue, bool bPreliminaryVote);
	void ChangeNumVictoryVotesTallied(int iChange, bool bPreliminaryVote);
	int GetNumVictoryVotesExpected() const;
	void SetNumVictoryVotesExpected(int iValue);
	TeamTypes GetVoteCast(TeamTypes eVotingTeam) const;
	void SetVoteCast(TeamTypes eVotingTeam, TeamTypes eVote, bool bPreliminaryVote = false);
	TeamTypes GetPreviousVoteCast(TeamTypes eVotingTeam) const;
	void SetPreviousVoteCast(TeamTypes eVotingTeam, TeamTypes eVotingTarget);
	int GetNumVotesForTeam(TeamTypes eTeam) const;
	void SetNumVotesForTeam(TeamTypes eTeam, int iValue);
	void ChangeNumVotesForTeam(TeamTypes eTeam, int iChange);
	// End DEPRECATED

	Localization::String GetDiploResponse(const char* szLeader, const char* szResponse, const Localization::String& strOptionalKey1, const Localization::String& strOptionalKey2);

	bool isDebugMode() const;
	void setDebugMode(bool bDebugMode);
	void toggleDebugMode();
	void updateDebugModeCache();

	void setFOW(bool bMode);
	bool getFOW();

	int getPitbossTurnTime() const;
	void setPitbossTurnTime(int iHours);

	bool isHotSeat() const;
	bool isPbem() const;
	bool isPitboss() const;
#ifdef AUI_GAME_BETTER_HYBRID_MODE
	bool isAnySimultaneousTurns() const;
	bool isAllActivePlayersTurnAllComplete() const;
#else
	bool isSimultaneousTeamTurns() const;
#endif

	bool isFinalInitialized() const;
	void setFinalInitialized(bool bNewValue);

	bool IsWaitingForBlockingInput() const
	{
		return (m_eWaitDiploPlayer != NO_PLAYER);
	};
	void SetWaitingForBlockingInput(PlayerTypes ePlayer)
	{
		m_eWaitDiploPlayer = ePlayer;
	};

	bool getPbemTurnSent() const;
	void setPbemTurnSent(bool bNewValue);

	bool getHotPbemBetweenTurns() const;
	void setHotPbemBetweenTurns(bool bNewValue);

	bool isPlayerOptionsSent() const;
	void sendPlayerOptions(bool bForce = false);

	PlayerTypes getActivePlayer() const;
	void setActivePlayer(PlayerTypes eNewValue, bool bForceHotSeat = false, bool bAutoplaySwitch = false);

	CvHandicapInfo& getHandicapInfo() const;
	HandicapTypes getHandicapType() const;
	void setHandicapType(HandicapTypes eHandicap);

	PlayerTypes getPausePlayer();
	bool isPaused();
	void setPausePlayer(PlayerTypes eNewValue);

	UnitTypes getBestLandUnit();
	int getBestLandUnitCombat();
	void setBestLandUnit(UnitTypes eNewValue);

	int GetFaithCost(CvUnitEntry *pkUnit) const;

	TeamTypes getWinner() const;
	VictoryTypes getVictory() const;
	void setWinner(TeamTypes eNewWinner, VictoryTypes eNewVictory);

	bool isVictoryAvailable(VictoryTypes eVictory) const;
	int GetNextAvailableVictoryCompetitionRank(VictoryTypes eVictory) const;
	void DoPlaceTeamInVictoryCompetition(VictoryTypes eNewVictory, TeamTypes eTeam);
	TeamTypes getTeamVictoryRank(VictoryTypes eNewVictory, int iRank) const;
	void setTeamVictoryRank(VictoryTypes eNewVictory, int iRank, TeamTypes eTeam);

	int GetWorldMilitaryStrengthAverage(PlayerTypes ePlayer, bool bIncludeMe, bool bIncludeOnlyKnown);

	int GetTotalReligionTechCost() const;
	void DoUpdateTotalReligionTechCost();
	int GetCachedWorldReligionTechProgress() const;
	void DoUpdateCachedWorldReligionTechProgress();

	TechTypes GetReligionTech() const;
	void SetReligionTech(TechTypes eTech);
	int GetResearchLeftToReligionTech(TeamTypes eTeam);
	int GetResearchLeftToTech(TeamTypes eTeam, TechTypes eTech);

	GameStateTypes getGameState();
	void setGameState(GameStateTypes eNewValue);

	CvEraInfo& getStartEraInfo() const;
	EraTypes getStartEra() const;

	CalendarTypes getCalendar() const;

	CvGameSpeedInfo& getGameSpeedInfo() const;
	GameSpeedTypes getGameSpeedType() const;

	int getEndTurnMessagesReceived(int iIndex);
	void incrementEndTurnMessagesReceived(int iIndex);

	PlayerTypes getRankPlayer(int iRank);
	void setRankPlayer(int iRank, PlayerTypes ePlayer);

	int getPlayerRank(PlayerTypes ePlayer);
	void setPlayerRank(PlayerTypes ePlayer, int iRank);

	int getPlayerScore(PlayerTypes ePlayer);
	void setPlayerScore(PlayerTypes ePlayer, int iScore);

	TeamTypes getRankTeam(int iRank);
	void setRankTeam(int iRank, TeamTypes eTeam);

	int getTeamRank(TeamTypes eTeam);
	void setTeamRank(TeamTypes eTeam, int iRank);

	int getTeamScore(TeamTypes eTeam) const;
	void setTeamScore(TeamTypes eTeam, int iScore);

	bool isOption(GameOptionTypes eIndex) const;
	bool isOption(const char* pszOption) const;
	void setOption(GameOptionTypes eIndex, bool bEnabled);
	void setOption(const char* pszOption, bool bEnabled);

	bool isMPOption(MultiplayerOptionTypes eIndex) const;
	void setMPOption(MultiplayerOptionTypes eIndex, bool bEnabled);

	int getUnitCreatedCount(UnitTypes eIndex);
	void incrementUnitCreatedCount(UnitTypes eIndex);

	int getUnitClassCreatedCount(UnitClassTypes eIndex);
	bool isUnitClassMaxedOut(UnitClassTypes eIndex, int iExtra = 0);
	void incrementUnitClassCreatedCount(UnitClassTypes eIndex);

	int getBuildingClassCreatedCount(BuildingClassTypes eIndex);
	bool isBuildingClassMaxedOut(BuildingClassTypes eIndex, int iExtra = 0);
	void incrementBuildingClassCreatedCount(BuildingClassTypes eIndex);

	int getProjectCreatedCount(ProjectTypes eIndex);
	bool isProjectMaxedOut(ProjectTypes eIndex, int iExtra = 0);
	void incrementProjectCreatedCount(ProjectTypes eIndex, int iExtra = 1);

	bool isVictoryValid(VictoryTypes eIndex) const;
	void setVictoryValid(VictoryTypes eIndex, bool bValid);
	bool areNoVictoriesValid() const;

	bool isSpecialUnitValid(SpecialUnitTypes eIndex);
	void makeSpecialUnitValid(SpecialUnitTypes eIndex);

	bool isNukesValid() const;
	void makeNukesValid(bool bValid = true);

	bool isInAdvancedStart() const;

	const CvString& getName();
	void setName(const char* szName);

	bool isDestroyedCityName(CvString& szName) const;
	void addDestroyedCityName(const CvString& szName);

	bool isGreatPersonBorn(CvString& szName) const;
	void addGreatPersonBornName(const CvString& szName);

	CvRandom& getMapRand();
	int getMapRandNum(int iNum, const char* pszLog);

	CvRandom& getJonRand();
	int getJonRandNum(int iNum, const char* pszLog);
#ifdef AUI_BINOM_RNG
	int getJonRandNumBinom(int iNum, const char* pszLog);
#endif
	int getJonRandNumVA(int iNum, const char* pszLog, ...);
	int getAsyncRandNum(int iNum, const char* pszLog);

	int calculateSyncChecksum();
	int calculateOptionsChecksum();

	void addReplayMessage(ReplayMessageTypes eType, PlayerTypes ePlayer, const CvString& pszText, int iPlotX = -1, int iPlotY = -1);
#ifdef REPLAY_MESSAGE_EXTENDED
	void addReplayMessage(ReplayMessageTypes eType, PlayerTypes ePlayer, const CvString& pszText, int iData1, int iData2, int iPlotX, int iPlotY);
#endif
	void clearReplayMessageMap();
	uint getNumReplayMessages() const;
	const CvReplayMessage* getReplayMessage(uint i) const;
#ifdef REPLAY_EVENTS
	void clearReplayEventMap();
	void addReplayEvent(int eType, std::vector<int> vNumArgs, CvString strArg = "");
	void addReplayEvent(int eType, PlayerTypes ePlayer, std::vector<int> vNumArgs, CvString strArg = "");
	uint getNumReplayEvents() const;
	const CvReplayEvent* getReplayEvent(uint i) const;
	void exportReplayEvents();
#endif

	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream) const;
	void ReadSupportingClassData(FDataStream& kStream);
	void WriteSupportingClassData(FDataStream& kStream);

	void writeReplay(FDataStream& kStream);

	// Ported in from old CvGameAI
	int GetCombatValue(UnitTypes eUnit);

	void saveReplay();

	void addPlayer(PlayerTypes eNewPlayer, LeaderHeadTypes eLeader, CivilizationTypes eCiv);

	void testVictory();
	bool testVictory(VictoryTypes eVictory, TeamTypes eTeam, bool* pbEndScore = NULL) const;

	int getPlotExtraYield(int iX, int iY, YieldTypes eYield) const;
	void setPlotExtraYield(int iX, int iY, YieldTypes eYield, int iCost);
	void removePlotExtraYield(int iX, int iY);

	int getPlotExtraCost(int iX, int iY) const;
	void changePlotExtraCost(int iX, int iY, int iCost);
	void removePlotExtraCost(int iX, int iY);

	bool isCivEverActive(CivilizationTypes eCivilization) const;
	bool isLeaderEverActive(LeaderHeadTypes eLeader) const;
	bool isUnitEverActive(UnitTypes eUnit) const;
	bool isBuildingEverActive(BuildingTypes eBuilding) const;

	RouteTypes GetIndustrialRoute() const;
	void DoUpdateIndustrialRoute();

	ImprovementTypes GetBarbarianCampImprovementType();
	UnitTypes getRandomBarbarianUnitType(CvArea* pArea, UnitAITypes eUnitAI);
	int GetBarbarianReleaseTurn() const
	{
		return m_iEarliestBarbarianReleaseTurn;
	};
	void SetBarbarianReleaseTurn(int iValue);

	UnitTypes GetRandomSpawnUnitType(PlayerTypes ePlayer, bool bIncludeUUs, bool bIncludeRanged);
	UnitTypes GetCompetitiveSpawnUnitType(PlayerTypes ePlayer, bool bIncludeUUs, bool bIncludeRanged);
	UnitTypes GetRandomUniqueUnitType(bool bIncludeCivsInGame, bool bIncludeStartEra, bool bIncludeOldEras, bool bIncludeRanged);

	CvSiteEvaluatorForSettler* GetSettlerSiteEvaluator();
	CvSiteEvaluatorForStart* GetStartSiteEvaluator();
	CvStartPositioner* GetStartPositioner();
	CvGameDeals* GetGameDeals();
	CvGameReligions* GetGameReligions();
	CvGameCulture* GetGameCulture();
	CvGameLeagues* GetGameLeagues();
	CvGameTrade* GetGameTrade();
	CvTacticalAnalysisMap* GetTacticalAnalysisMap();
#ifdef MP_PLAYERS_VOTING_SYSTEM
	CvMPVotingSystem* GetMPVotingSystem();
#endif

	int GetAction(int iKeyStroke, bool bAlt, bool bShift, bool bCtrl);
	int IsAction(int iKeyStroke, bool bAlt, bool bShift, bool bCtrl);

	void endTurnTimerSemaphoreIncrement();
	void endTurnTimerSemaphoreDecrement();
	void endTurnTimerReset();
	bool AreUnitsSameType(UnitTypes eFirstUnitType, UnitTypes eSecondUnitType);

	void DoMinorPledgeProtection(PlayerTypes eMajor, PlayerTypes eMinor, bool bProtect, bool bPledgeNowBroken = false);
	void DoMinorGiftGold(PlayerTypes eMinor, int iNumGold);
	void DoMinorGiftTileImprovement(PlayerTypes eMajor, PlayerTypes eMinor, int iPlotX, int iPlotY);
	void DoMinorBullyGold(PlayerTypes eBully, PlayerTypes eMinor);
	void DoMinorBullyUnit(PlayerTypes eBully, PlayerTypes eMinor);
	void DoMinorBuyout(PlayerTypes eMajor, PlayerTypes eMinor);

	void DoResearchAgreementNotification(TeamTypes eTeam1, TeamTypes eTeam2);

	int GetResearchAgreementCost(PlayerTypes ePlayer1, PlayerTypes ePlayer2);

	// Victory Stuff
	void DoTestConquestVictory();

	PlayerTypes GetBestWondersPlayer();
	void SetBestWondersPlayer(PlayerTypes ePlayer, int iWonderCount);
	PlayerTypes GetBestPoliciesPlayer();
	void SetBestPoliciesPlayer(PlayerTypes ePlayer, int iPolicyCount);
	PlayerTypes GetBestGreatPeoplePlayer();
	void SetBestGreatPeoplePlayer(PlayerTypes ePlayer, int iGreatPeopleCount);

	bool IsEndGameTechResearched() const;
	void SetEndGameTechResearched(bool bValue);
	// End Victory Stuff

	// Returns true if the tuner has ever been connected at any point during this game.
	bool TunerEverConnected() const;

	// slewis - Tutorial stuff
	bool IsEverAttackedTutorial() const;
	void SetEverAttackedTutorial(bool bValue);
	bool IsEverRightClickMoved() const;
	void SetEverRightClickMoved(bool bValue);
	bool IsCombatWarned() const;
	void SetCombatWarned(bool bValue);
	// end Tutorial stuff

	CvAdvisorCounsel* GetAdvisorCounsel();
	CvAdvisorRecommender* GetAdvisorRecommender();

#if defined(AUI_WARNING_FIXES) || defined(AUI_CONSTIFY)
	int GetTurnsBetweenMinorCivElections() const;
	int GetTurnsUntilMinorCivElection() const;
#else
	int GetTurnsBetweenMinorCivElections();
	int GetTurnsUntilMinorCivElection();
#endif

	void LogGameState(bool bLogHeaders = false);
	void unitIsMoving();

	void BuildProdModHelpText(CvString* toolTipSink, const char* strTextKey, int iMod, const char* strExtraKey = "") const;
	void BuildCannotPerformActionHelpText(CvString* toolTipSink, const char* strTextKey, const char* strExtraKey1 = "", const char* strExtraKey2 = "", int iValue = -666) const;

	bool allUnitAIProcessed() const;

	void updateTurnTimer();
	bool hasTurnTimerExpired(PlayerTypes playerID);
	void TurnTimerSync(float fCurTurnTime, float fTurnStartTime);
	void GetTurnTimerData(float& fCurTurnTime, float& fTurnStartTime);

	int GetDealDuration();
	int GetPeaceDuration();

	CombatPredictionTypes GetCombatPrediction(const CvUnit* pAttackingUnit, const CvUnit* pDefendingUnit);

	void NetMessageStaticsReset();
	void SetLastTurnAICivsProcessed();

	void TriggerArchaeologySiteCreation(bool bCheckInitialized);
	bool IsArchaeologyTriggered() const;
	int GetNumArchaeologySites() const;
	int GetNumHiddenArchaeologySites() const;
	
#if defined (CAN_PARADROP_HALF_TIMER) || defined (CAN_SET_INTERCEPT_HALF_TIMER)
	FTimer  m_endTurnTimer;
	int     m_endTurnTimerSemaphore;
	FTimer  m_curTurnTimer;
	FTimer  m_timeSinceGameTurnStart;		//time since game turn started for human players
	float	m_fCurrentTurnTimerPauseDelta;
#endif
#ifdef TURN_TIMER_PAUSE_BUTTON
	bool m_bIsPaused;
#endif
	bool AnyoneHasBuildingClass(BuildingClassTypes iBuildingClassType) const;

public:

	//Function to determine city size from city population
	unsigned int GetVariableCitySizeFromPopulation(unsigned int nPopulation);

	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------

private:
	//------------------------------------------------------------
	// Convert from city population to discrete size
	//------------------------------------------------------------
	const static unsigned int ms_aiSizes[10];

protected:

	int m_iEndTurnMessagesSent;
	int m_iElapsedGameTurns;
	int m_iStartTurn;
	int m_iWinningTurn;
	int m_iStartYear;
	int m_iEstimateEndTurn;
	int m_iDefaultEstimateEndTurn;
	int m_iTurnSlice;
	int m_iCutoffSlice;
	int m_iNumCities;
	int m_iTotalPopulation;
	int m_iNoNukesCount;
	int m_iNukesExploded;
	int m_iMaxPopulation;
	int m_iUnused1;  //unused
	int m_iUnused2;  //unused
	int m_iUnused3;  //unused
	int m_iInitPopulation;
	int m_iInitLand;
	int m_iInitTech;
	int m_iInitWonders;
	int m_iAIAutoPlay;
	int m_iTotalReligionTechCost;
	int m_iCachedWorldReligionTechProgress;
	int m_iUnitedNationsCountdown;
	int m_iNumVictoryVotesTallied;
	int m_iNumVictoryVotesExpected;
	int m_iVotesNeededForDiploVictory;
	int m_iMapScoreMod;

	unsigned int m_uiInitialTime;
#ifdef GAME_UPDATE_TURN_TIMER_ONCE_PER_TURN
	float m_fPreviousTurnLen;
#endif
#ifdef TURN_TIMER_PAUSE_BUTTON
	float m_fTimeElapsed;
#endif
#ifdef GAME_ALLOW_ONLY_ONE_UNIT_MOVE_ON_TURN_LOADING
	bool m_bMPOrderedMoveOnTurnLoading;
	bool m_bReceivedFirstMission;
#endif
	bool m_bScoreDirty;
	bool m_bCircumnavigated;
	bool m_bDebugMode;
	bool m_bDebugModeCache;
	bool m_bFinalInitialized;
	bool m_bPbemTurnSent;
	bool m_bHotPbemBetweenTurns;
	bool m_bPlayerOptionsSent;
	bool m_bNukesValid;
	bool m_bEndGameTechResearched;
	bool m_bTunerEverConnected;
	bool m_bDynamicTurnsSimultMode;		//if playing dynamic turn mode, are we currently running simultaneous turns?
	PlayerTypes m_eWaitDiploPlayer;

	bool m_bFOW;

	// slewis - tutorial values
	bool m_bStaticTutorialActive;
	bool m_bTutorialEverAttacked;
	bool m_bEverRightClickMoved;
	bool m_bCombatWarned;
	std::tr1::unordered_set<std::string> m_AdvisorMessagesViewed;
	// slewis - tutorial values

	HandicapTypes m_eHandicap;
	PlayerTypes m_ePausePlayer;
	PlayerTypes m_eAIAutoPlayReturnPlayer;
	UnitTypes m_eBestLandUnit;
	TeamTypes m_eWinner;
	VictoryTypes m_eVictory;
	GameStateTypes m_eGameState;
	PlayerTypes m_eEventPlayer;
	PlayerTypes m_eBestWondersPlayer;
	PlayerTypes m_eBestPoliciesPlayer;
	PlayerTypes m_eBestGreatPeoplePlayer;
	TechTypes m_eReligionTech;
	RouteTypes m_eIndustrialRoute;
	bool m_bArchaeologyTriggered;

	CvString m_strScriptData;

	int* m_aiEndTurnMessagesReceived;
	int* m_aiRankPlayer;        // Ordered by rank...
	int* m_aiPlayerRank;        // Ordered by player ID...
	int* m_aiPlayerScore;       // Ordered by player ID...
	int* m_aiRankTeam;						// Ordered by rank...
	int* m_aiTeamRank;						// Ordered by team ID...
	int* m_aiTeamScore;						// Ordered by team ID...

	int* m_paiUnitCreatedCount;
	int* m_paiUnitClassCreatedCount;
	int* m_paiBuildingClassCreatedCount;
	int* m_paiProjectCreatedCount;
	PlayerVoteTypes* m_paiVoteOutcome;
	int* m_aiSecretaryGeneralTimer;
	int* m_aiVoteTimer;
	int* m_aiDiploVote;
	int* m_aiVotesCast;
	int* m_aiPreviousVotesCast;
	int* m_aiNumVotesForTeam;

	int* m_aiTeamCompetitionWinnersScratchPad;

	bool* m_pabSpecialUnitValid;

	int** m_apaiPlayerVote;
	int** m_ppaaiTeamVictoryRank;

	Database::Results* m_pDiploResponseQuery;

	std::vector<CvString> m_aszDestroyedCities;
	std::vector<CvString> m_aszGreatPeopleBorn;

	FFreeListTrashArray<VoteSelectionData> m_voteSelections;
	FFreeListTrashArray<VoteTriggeredData> m_votesTriggered;

	CvRandom m_mapRand;
	CvRandom m_jonRand;

	typedef std::vector<CvReplayMessage> ReplayMessageList;
	ReplayMessageList m_listReplayMessages;
#ifdef REPLAY_EVENTS
	typedef std::vector< CvReplayEvent > ReplayEventList;
	ReplayEventList m_listReplayEvents;
#endif

	int m_iNumSessions;

	std::vector<PlotExtraYield> m_aPlotExtraYields;
	std::vector<PlotExtraCost> m_aPlotExtraCosts;

	int m_iEarliestBarbarianReleaseTurn;

	CvSiteEvaluatorForSettler* m_pSettlerSiteEvaluator;
	CvSiteEvaluatorForStart*   m_pStartSiteEvaluator;
	CvStartPositioner*         m_pStartPositioner;
	CvGameReligions*           m_pGameReligions;
	CvGameCulture*             m_pGameCulture;
	CvGameLeagues*             m_pGameLeagues;
	CvGameTrade*               m_pGameTrade;
	CvTacticalAnalysisMap*     m_pTacticalMap;
#ifdef MP_PLAYERS_VOTING_SYSTEM
	CvMPVotingSystem* m_pMPVotingSystem;
#endif

	CvAdvisorCounsel*          m_pAdvisorCounsel;
	CvAdvisorRecommender*      m_pAdvisorRecommender;

	CvGameDeals                m_kGameDeals;

	//necessary because we only want to hide the mouseover of the most recently moused over unit -KS
	int                        m_iLastMouseoverUnitID;

	// CACHE: cache frequently used values

#if !defined (CAN_PARADROP_HALF_TIMER) || !defined (CAN_SET_INTERCEPT_HALF_TIMER)
	FTimer  m_endTurnTimer;
	int     m_endTurnTimerSemaphore;
#endif
	int     m_lastTurnAICivsProcessed;
#if !defined (CAN_PARADROP_HALF_TIMER) || !defined (CAN_SET_INTERCEPT_HALF_TIMER)
	FTimer  m_curTurnTimer;
#endif
#if !defined (CAN_PARADROP_HALF_TIMER) || !defined (CAN_SET_INTERCEPT_HALF_TIMER)
	FTimer  m_timeSinceGameTurnStart;		//time since game turn started for human players
#endif
#if !defined (CAN_PARADROP_HALF_TIMER) || !defined (CAN_SET_INTERCEPT_HALF_TIMER)
	float	m_fCurrentTurnTimerPauseDelta;	//
#endif
	bool    m_sentAutoMoves;
	bool	m_bForceEndingTurn;

	void initDiplomacy();
	void initFreeState(CvGameInitialItemsOverrides& kOverrides);
	void initFreeUnits(CvGameInitialItemsOverrides& kOverrides);

	bool InitMap(CvGameInitialItemsOverrides& kGameInitialItemsOverrides);

	void assignStartingPlots();

	void setInitialItems(CvGameInitialItemsOverrides& kOverrides);
	void CheckGenerateArchaeology();
	void regenerateMap();

	void testExtendedGame();

	bool IsForceEndingTurn() const;
	void SetForceEndingTurn(bool bValue);

	void DoCacheMapScoreMod();

	void doTurn();

#ifdef AUI_GAME_BETTER_HYBRID_MODE
	void constructTurnOrders();
	int m_iCurrentTurnOrderActive;
	int m_iLastTurnOrderID;
#ifdef AUI_GAME_PLAYER_BASED_TURN_LENGTH
	void calculateMaxTurnLengths();
#ifndef NQM_GAME_EXTEND_TURN_TIMER_ON_LAST_MINUTE_WAR_DECLARATION_IF_SIMULTANEOUS
	FFastVector<int, true, c_eCiv5GameplayDLL> m_aiMaxTurnLengths;
#endif
#endif
#endif

	void updateWar();
	void updateMoves();
	void updateTimers();
	void UpdatePlayers();
	void testAlive();

	void showEndGameSequence();

	CvPlot* normalizeFindLakePlot(PlayerTypes ePlayer);

	void doUpdateCacheOnTurn();

	void CheckPlayerTurnDeactivate();

	void PopulateDigSite(CvPlot& kPlot, EraTypes eEra, GreatWorkArtifactClass eArtifact);
	void SpawnArchaeologySitesHistorically();


};

#ifdef REPLAY_EVENTS
// add new event types to the end of the enum, before NUM_REPLAYEVENTS
// each event listed here should have its own corresponding xml entry in ReplayEvents table
enum ReplayEventTypes
{
	// Network actions
	REPLAYEVENT_AdvancedStartAction,
	REPLAYEVENT_AutoMission,
	REPLAYEVENT_BarbarianRansom,
	REPLAYEVENT_ChangeWar,
	REPLAYEVENT_IgnoreWarning,
	REPLAYEVENT_CityBuyPlot,
	REPLAYEVENT_CityDoTask,
	REPLAYEVENT_CityPopOrder,
	REPLAYEVENT_CityPurchase,
	REPLAYEVENT_CityPushOrder,
	REPLAYEVENT_CitySwapOrder,
	REPLAYEVENT_ChooseElection,
	REPLAYEVENT_DestroyUnit,
	REPLAYEVENT_DiplomacyFromUI,
	REPLAYEVENT_DiploVote,
	REPLAYEVENT_DoCommand,
	REPLAYEVENT_ExtendedGame,
	REPLAYEVENT_FoundPantheon,
	REPLAYEVENT_FoundReligion,
	REPLAYEVENT_EnhanceReligion,
	REPLAYEVENT_MoveSpy,
	REPLAYEVENT_StageCoup,
	REPLAYEVENT_FaithPurchase,
	REPLAYEVENT_LeagueVoteEnact,
	REPLAYEVENT_LeagueVoteRepeal,
	REPLAYEVENT_LeagueVoteAbstain,
	REPLAYEVENT_LeagueProposeEnact,
	REPLAYEVENT_LeagueProposeRepeal,
	REPLAYEVENT_LeagueEditName,
	REPLAYEVENT_SetSwappableGreatWork,
	REPLAYEVENT_SwapGreatWorks,
	REPLAYEVENT_MoveGreatWorks,
	REPLAYEVENT_ChangeIdeology,
	REPLAYEVENT_GiftUnit,
	REPLAYEVENT_LaunchSpaceship,
	REPLAYEVENT_LiberatePlayer,
	REPLAYEVENT_MinorCivBullyGold,
	REPLAYEVENT_MinorCivBullyUnit,
	REPLAYEVENT_MinorCivGiftGold,
	REPLAYEVENT_MinorCivGiftTileImprovement,
	REPLAYEVENT_MinorCivBuyout,
	REPLAYEVENT_MinorNoUnitSpawning,
	REPLAYEVENT_PlayerDealFinalized,
	REPLAYEVENT_PlayerOption,
	REPLAYEVENT_PledgeMinorProtection,
	REPLAYEVENT_PushMission,
	REPLAYEVENT_GreatPersonChoice,
	REPLAYEVENT_MayaBonusChoice,
	REPLAYEVENT_FaithGreatPersonChoice,
	REPLAYEVENT_GoodyChoice,
	REPLAYEVENT_ArchaeologyChoice,
	REPLAYEVENT_IdeologyChoice,
	REPLAYEVENT_RenameCity,
	REPLAYEVENT_RenameUnit,
	REPLAYEVENT_Research,
	REPLAYEVENT_ReturnCivilian,
	REPLAYEVENT_SellBuilding,
	REPLAYEVENT_SetCityAIFocus,
	REPLAYEVENT_SetCityAvoidGrowth,
	REPLAYEVENT_SwapUnits,
	REPLAYEVENT_UpdateCityCitizens,
	REPLAYEVENT_UpdatePolicies,
	/// 
	REPLAYEVENT_CityPurchaseUnit,
	REPLAYEVENT_CityPurchaseBuilding,
	REPLAYEVENT_FreeTech,
	REPLAYEVENT_StealTech,
	REPLAYEVENT_ProposalIrr,
	REPLAYEVENT_ProposalCc,
	REPLAYEVENT_ProposalScrap,
	REPLAYEVENT_ProposalYes,
	REPLAYEVENT_ProposalNo,
	REPLAYEVENT_ResetTimer,
	REPLAYEVENT_PauseTimer,
	REPLAYEVENT_OpenDemoScreen,
	REPLAYEVENT_ExtractSpy,
	REPLAYEVENT_UpdatePolicyBranch,
	REPLAYEVENT_UnpauseTimer,

	// Gameplay events
	REPLAYEVENT_CityUnitComplete,
	REPLAYEVENT_CityBuildingComplete,
	REPLAYEVENT_CityGrowth,
	REPLAYEVENT_CityStarvation,
	REPLAYEVENT_CityBorderGrowth,
	REPLAYEVENT_UnitDisbanded,
	REPLAYEVENT_UnitKilledInCombat,
	REPLAYEVENT_UnitPromotion,
	REPLAYEVENT_UnitUpgrade,
	REPLAYEVENT_ImprovementFinished,
	REPLAYEVENT_GoodyHut,
	REPLAYEVENT_NaturalWonderDiscovered,
	REPLAYEVENT_CircumnavigatedGlobe,
	REPLAYEVENT_MeetTeam,
	REPLAYEVENT_TechAcquired,
	REPLAYEVENT_AdvanceEra,
	REPLAYEVENT_SpyCoupResult,
	REPLAYEVENT_SpyOperationResult,
	REPLAYEVENT_MinorQuestComplete,
	REPLAYEVENT_MinorAllyChanged,
	REPLAYEVENT_MinorFriendChanged,
	REPLAYEVENT_CongressHostChange,
	REPLAYEVENT_CongressEnactedProposalsChange,
	REPLAYEVENT_MPProposalResult,
	REPLAYEVENT_PlotNewCityName,  // Special event to track city name changes
	REPLAYEVENT_EnterCityScreen,

	NUM_REPLAYEVENTS
};
#endif

#endif
