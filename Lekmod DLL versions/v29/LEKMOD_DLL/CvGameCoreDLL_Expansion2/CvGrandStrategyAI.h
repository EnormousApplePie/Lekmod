/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_GRAND_STRATEGY_AI_H
#define CIV5_GRAND_STRATEGY_AI_H

#include "CvDiplomacyAI.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvGrandStrategyAI
//!  \brief		Information about the Grand Strategy of a single AI player
//
//!  Author:	Jon Shafer
//
//!  Key Attributes:
//!  - Object created by CvPlayer
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvGrandStrategyAI
{
public:
	CvGrandStrategyAI(void);
	~CvGrandStrategyAI(void);
	void Init(CvAIGrandStrategyXMLEntries* pAIGrandStrategies, CvPlayer* pPlayer);
	void Uninit();
	void Reset();
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

#ifdef AUI_CONSTIFY
	CvPlayer* GetPlayer() const;
	CvAIGrandStrategyXMLEntries* GetAIGrandStrategies() const;
#else
	CvPlayer* GetPlayer();
	CvAIGrandStrategyXMLEntries* GetAIGrandStrategies();
#endif

	void DoTurn();

#ifdef AUI_CONSTIFY
	int GetConquestPriority() const;
	int GetCulturePriority() const;
	int GetUnitedNationsPriority() const;
	int GetSpaceshipPriority() const;

	int GetBaseGrandStrategyPriority(AIGrandStrategyTypes eGrandStrategy) const;
#else
	int GetConquestPriority();
	int GetCulturePriority();
	int GetUnitedNationsPriority();
	int GetSpaceshipPriority();

	int GetBaseGrandStrategyPriority(AIGrandStrategyTypes eGrandStrategy);
#endif

	AIGrandStrategyTypes GetActiveGrandStrategy() const;
	void SetActiveGrandStrategy(AIGrandStrategyTypes eGrandStrategy);
	int GetNumTurnsSinceActiveSet() const;
	void SetNumTurnsSinceActiveSet(int iValue);
	void ChangeNumTurnsSinceActiveSet(int iChange);

	int GetGrandStrategyPriority(AIGrandStrategyTypes eGrandStrategy) const;
	void SetGrandStrategyPriority(AIGrandStrategyTypes eGrandStrategy, int iValue);
	void ChangeGrandStrategyPriority(AIGrandStrategyTypes eGrandStrategy, int iChange);

#ifdef AUI_CONSTIFY
	int GetPersonalityAndGrandStrategy(FlavorTypes eFlavorType) const;
#else
	int GetPersonalityAndGrandStrategy(FlavorTypes eFlavorType);
#endif

	// **********
	// Stuff relating to guessing what other Players are up to
	// **********

	void DoGuessOtherPlayersActiveGrandStrategy();

	AIGrandStrategyTypes GetGuessOtherPlayerActiveGrandStrategy(PlayerTypes ePlayer) const;
	GuessConfidenceTypes GetGuessOtherPlayerActiveGrandStrategyConfidence(PlayerTypes ePlayer) const;
	void SetGuessOtherPlayerActiveGrandStrategy(PlayerTypes ePlayer, AIGrandStrategyTypes eGrandStrategy, GuessConfidenceTypes eGuessConfidence);

	int GetGuessOtherPlayerConquestPriority(PlayerTypes ePlayer, int iWorldMilitaryAverage);
	int GetGuessOtherPlayerCulturePriority(PlayerTypes ePlayer, int iWorldCultureAverage, int iWorldTourismAverage);
	int GetGuessOtherPlayerUnitedNationsPriority(PlayerTypes ePlayer);
	int GetGuessOtherPlayerSpaceshipPriority(PlayerTypes ePlayer, int iWorldNumTechsAverage);

private:

	void LogGrandStrategies(const FStaticVector< int, 5, true, c_eCiv5GameplayDLL >& vGrandStrategyPriorities);
	void LogGuessOtherPlayerGrandStrategy(const FStaticVector< int, 5, true, c_eCiv5GameplayDLL >& vGrandStrategyPriorities, PlayerTypes ePlayer);

	CvPlayer* m_pPlayer;
	CvAIGrandStrategyXMLEntries* m_pAIGrandStrategies;

	int m_iNumTurnsSinceActiveSet;

	AIGrandStrategyTypes m_eActiveGrandStrategy;

	int* m_paiGrandStrategyPriority;

	// **********
	// Stuff relating to guessing what other Players are up to
	// **********

	int* m_eGuessOtherPlayerActiveGrandStrategy;
	int* m_eGuessOtherPlayerActiveGrandStrategyConfidence;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvAIGrandStrategyXMLEntry
//!  \brief		A single entry in the AI Grand Strategy XML file
//
//!  Key Attributes:
//!  - Populated from XML\???? (not sure what path/name you want)
//!  - Array of these contained in CvAIGrandStrategyXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvAIGrandStrategyXMLEntry: public CvBaseInfo
{
public:
	CvAIGrandStrategyXMLEntry();
	virtual ~CvAIGrandStrategyXMLEntry();

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

	// Accessor Functions
#ifdef AUI_WARNING_FIXES
	int GetFlavorValue(uint i) const;
	int GetFlavorModValue(uint i) const;
#else
	int GetFlavorValue(int i) const;
	int GetFlavorModValue(int i) const;
#endif
	int GetSpecializationBoost(YieldTypes eYield) const;

private:
	int* m_piFlavorValue;
	int* m_piSpecializationBoost;
	int* m_piFlavorModValue;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvAIGrandStrategyXMLEntries
//!  \brief		Game-wide information about possible AI strategies
//
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\???? (not sure what path/name you want)
//! - Contains an array of CvAIGrandStrategyXMLEntry from the above XML file
//! - One instance for the entire game
//! - Accessed heavily by CvPlayerAIStrategy class (which stores the AI strategy state for 1 player)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvAIGrandStrategyXMLEntries
{
public:
	CvAIGrandStrategyXMLEntries(void);
	~CvAIGrandStrategyXMLEntries(void);

	// Accessor functions
	std::vector<CvAIGrandStrategyXMLEntry*>& GetAIGrandStrategyEntries();
#ifdef AUI_WARNING_FIXES
	uint GetNumAIGrandStrategies() const;
	CvAIGrandStrategyXMLEntry* GetEntry(uint index) const;
#else
	int GetNumAIGrandStrategies();
	CvAIGrandStrategyXMLEntry* GetEntry(int index);
#endif

	void DeleteArray();

private:
	std::vector<CvAIGrandStrategyXMLEntry*> m_paAIGrandStrategyEntries;
};

#endif //CIV5_GRAND_STRATEGY_AI_H