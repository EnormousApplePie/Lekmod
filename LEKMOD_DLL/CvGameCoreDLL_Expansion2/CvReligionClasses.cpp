/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
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
#include "CvMinorCivAI.h"
#include "CvDiplomacyAI.h"
#include "CvEconomicAI.h"
#include "CvGrandStrategyAI.h"
#include "CvMilitaryAI.h"
#include "cvStopWatch.h"

#include "LintFree.h"
 
//======================================================================================================
//					CvReligionEntry
//======================================================================================================
/// Constructor
CvReligionEntry::CvReligionEntry()
{
}

/// Destructor
CvReligionEntry::~CvReligionEntry()
{
}

/// Load XML data
bool CvReligionEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	if(!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;

	//Basic Properties
	m_strIconString = kResults.GetText("IconString");
	 
	return true;
}

//------------------------------------------------------------------------------
CvString CvReligionEntry::GetIconString() const
{
	return m_strIconString;
}

//=====================================
// CvReligionXMLEntries
//=====================================
/// Constructor
CvReligionXMLEntries::CvReligionXMLEntries(void)
{

}

/// Destructor
CvReligionXMLEntries::~CvReligionXMLEntries(void)
{
	DeleteArray();
}

/// Returns vector of trait entries
std::vector<CvReligionEntry*>& CvReligionXMLEntries::GetReligionEntries()
{
	return m_paReligionEntries;
}

/// Number of defined traits
int CvReligionXMLEntries::GetNumReligions()
{
	return m_paReligionEntries.size();
}

/// Clear trait entries
void CvReligionXMLEntries::DeleteArray()
{
	for(std::vector<CvReligionEntry*>::iterator it = m_paReligionEntries.begin(); it != m_paReligionEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paReligionEntries.clear();
}

/// Get a specific entry
CvReligionEntry* CvReligionXMLEntries::GetEntry(int index)
{
	return m_paReligionEntries[index];
}

//=====================================
// CvReligion
//=====================================
/// Default Constructor
CvReligion::CvReligion()
	: m_eReligion(NO_RELIGION)
	, m_eFounder(NO_PLAYER)
	, m_iHolyCityX(-1)
	, m_iHolyCityY(-1)
	, m_iTurnFounded(-1)
	, m_bPantheon(false)
	, m_bEnhanced(false)
{
	ZeroMemory(m_szCustomName, sizeof(m_szCustomName));
}

/// Constructor
CvReligion::CvReligion(ReligionTypes eReligion, PlayerTypes eFounder, CvCity* pHolyCity, bool bPantheon)
	: m_eReligion(eReligion)
	, m_eFounder(eFounder)
	, m_bPantheon(bPantheon)
	, m_bEnhanced(false)
{
	if(pHolyCity)
	{
		m_iHolyCityX = pHolyCity->getX();
		m_iHolyCityY = pHolyCity->getY();
	}
	m_iTurnFounded = GC.getGame().getGameTurn();
	ZeroMemory(m_szCustomName, sizeof(m_szCustomName));
}

/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvReligion& writeTo)
{
	uint uiVersion;
	loadFrom >> uiVersion;

	loadFrom >> writeTo.m_eReligion;
	loadFrom >> writeTo.m_eFounder;
	loadFrom >> writeTo.m_iHolyCityX;
	loadFrom >> writeTo.m_iHolyCityY;
	loadFrom >> writeTo.m_iTurnFounded;

	if(uiVersion >= 2)
	{
		loadFrom >> writeTo.m_bPantheon;
	}
	else
	{
		writeTo.m_bPantheon = false;
	}

	if(uiVersion >= 4)
	{
		loadFrom >> writeTo.m_bEnhanced;
	}
	else
	{
		writeTo.m_bEnhanced = false;
	}

	ZeroMemory(writeTo.m_szCustomName, sizeof(writeTo.m_szCustomName));
	if(uiVersion >= 3)
	{
		loadFrom >> writeTo.m_szCustomName;
	}

	writeTo.m_Beliefs.Read(loadFrom);

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvReligion& readFrom)
{
	uint uiVersion = 4;
	saveTo << uiVersion;

	saveTo << readFrom.m_eReligion;
	saveTo << readFrom.m_eFounder;
	saveTo << readFrom.m_iHolyCityX;
	saveTo << readFrom.m_iHolyCityY;
	saveTo << readFrom.m_iTurnFounded;
	saveTo << readFrom.m_bPantheon;
	saveTo << readFrom.m_bEnhanced;
	saveTo << readFrom.m_szCustomName;

	readFrom.m_Beliefs.Write(saveTo);

	return saveTo;
}

CvString CvReligion::GetName() const
{
#ifdef AUI_WARNING_FIXES
	CvString szReligionName = m_szCustomName;
	if (szReligionName.IsEmpty())
	{
		CvReligionEntry* pEntry = GC.getReligionInfo(m_eReligion);
		CvAssertMsg(pEntry, "pEntry for religion not expected to be NULL. Please send Anton or Ed your save file and version.");
		if (pEntry)
			szReligionName = pEntry->GetDescriptionKey();
		else
			szReligionName = "No Religion";
	}
	
	return szReligionName;
#else
	CvReligionEntry* pEntry = GC.getReligionInfo(m_eReligion);
	CvAssertMsg(pEntry, "pEntry for religion not expected to be NULL. Please send Anton or Ed your save file and version.");
	if (pEntry)
	{
		CvString szReligionName = (m_szCustomName == NULL || strlen(m_szCustomName) == 0) ? pEntry->GetDescriptionKey() : m_szCustomName;
		return szReligionName;
	}

	const char* szReligionNameBackup = "No Religion";
	return szReligionNameBackup;
#endif
}

//=====================================
// CvReligionInCity
//=====================================
/// Default Constructor
CvReligionInCity::CvReligionInCity()
{
	m_eReligion = NO_RELIGION;
	m_bFoundedHere = false;
	m_iFollowers = 0;
	m_iPressure = 0;
	m_iNumTradeRoutesApplyingPressure = 0;
	m_iTemp = 0;
}

/// Constructor
CvReligionInCity::CvReligionInCity(ReligionTypes eReligion, bool bFoundedHere, int iFollowers, int iPressure):
	m_eReligion(eReligion),
	m_bFoundedHere(bFoundedHere),
	m_iFollowers(iFollowers),
	m_iPressure(iPressure),
	m_iNumTradeRoutesApplyingPressure(0),
	m_iTemp(0)
{
}

/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvReligionInCity& writeTo)
{
	uint uiVersion;
	loadFrom >> uiVersion;

	loadFrom >> writeTo.m_eReligion;
	loadFrom >> writeTo.m_bFoundedHere;
	loadFrom >> writeTo.m_iFollowers;
	loadFrom >> writeTo.m_iPressure;

	if (uiVersion == 2)
	{
		loadFrom >> writeTo.m_iNumTradeRoutesApplyingPressure;
	}
	else
	{
		writeTo.m_iNumTradeRoutesApplyingPressure = 0;
	}

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvReligionInCity& readFrom)
{
	uint uiVersion = 2;
	saveTo << uiVersion;

	saveTo << readFrom.m_eReligion;
	saveTo << readFrom.m_bFoundedHere;
	saveTo << readFrom.m_iFollowers;
	saveTo << readFrom.m_iPressure;
	saveTo << readFrom.m_iNumTradeRoutesApplyingPressure;

	return saveTo;
}

//=====================================
// CvGameReligions
//=====================================
/// Constructor
CvGameReligions::CvGameReligions(void):
	m_iMinimumFaithForNextPantheon(0)
{
}

/// Destructor
CvGameReligions::~CvGameReligions(void)
{

}

/// Initialize class data
void CvGameReligions::Init()
{
	m_iMinimumFaithForNextPantheon = GC.getRELIGION_MIN_FAITH_FIRST_PANTHEON();
	m_iMinimumFaithForNextPantheon *= GC.getGame().getGameSpeedInfo().getTrainPercent();
	m_iMinimumFaithForNextPantheon /= 100;
}

/// Handle turn-by-turn religious updates
void CvGameReligions::DoTurn()
{
	SpreadReligion();
}

/// Spread religious pressure into adjacent cities
void CvGameReligions::SpreadReligion()
{
	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				SpreadReligionToOneCity(pLoopCity);
			}
		}
	}
}

/// Spread religious pressure to one city
void CvGameReligions::SpreadReligionToOneCity(CvCity* pCity)
{
#ifdef AUI_WARNING_FIXES
	if (!pCity)
		return;
#endif
	// Used to calculate how many trade routes are applying pressure to this city. This resets the value so we get a true count every turn.
	pCity->GetCityReligions()->ResetNumTradeRoutePressure();

	// Is this a city where a religion was founded?
	if(pCity->GetCityReligions()->IsHolyCityAnyReligion())
	{
		pCity->GetCityReligions()->AddHolyCityPressure();
	}

	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			int iSpyPressure = kPlayer.GetReligions()->GetSpyPressure();
			if (iSpyPressure > 0)
			{
				if (kPlayer.GetEspionage()->GetSpyIndexInCity(pCity) != -1)
				{
					ReligionTypes eReligionFounded = kPlayer.GetReligions()->GetReligionCreatedByPlayer();
					pCity->GetCityReligions()->AddSpyPressure(eReligionFounded, iSpyPressure);
				}
			}

			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				// Ignore the same city
				if(pCity == pLoopCity)
				{
					continue;
				}

				// Does this city have a majority religion?
				ReligionTypes eMajorityReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();
				if(eMajorityReligion == NO_RELIGION)
				{
					continue;
				}

				int iNumTradeRoutes = 0;
				int iPressure = GetAdjacentCityReligiousPressure (eMajorityReligion, pLoopCity, pCity, iNumTradeRoutes, false);
				if (iPressure > 0)
				{
					pCity->GetCityReligions()->AddReligiousPressure(FOLLOWER_CHANGE_ADJACENT_PRESSURE, eMajorityReligion, iPressure);
					if (iNumTradeRoutes != 0)
					{
						pCity->GetCityReligions()->IncrementNumTradeRouteConnections(eMajorityReligion, iNumTradeRoutes);
					}
				}
			}
		}
	}
}

/// Religious activities at the start of a player's turn
void CvGameReligions::DoPlayerTurn(CvPlayer& kPlayer)
{
	AI_PERF_FORMAT("AI-perf.csv", ("CvGameReligions::DoPlayerTurn, Turn %03d, %s", GC.getGame().getElapsedGameTurns(), kPlayer.getCivilizationShortDescription()) );
	bool bCouldAtStartAffordFaithPurchase = kPlayer.GetReligions()->CanAffordFaithPurchase();
	const PlayerTypes ePlayer = kPlayer.GetID();

#ifndef AUI_YIELDS_APPLIED_AFTER_TURN_NOT_BEFORE
	int iFaithPerTurn = kPlayer.GetTotalFaithPerTurn();
	if(iFaithPerTurn > 0)
	{
		kPlayer.ChangeFaith(iFaithPerTurn);
	}
#endif

	// If just now can afford missionary, add a notification
	bool bCanNowAffordFaithPurchase = kPlayer.GetReligions()->CanAffordFaithPurchase();
	if (kPlayer.GetFaithPurchaseType() == NO_AUTOMATIC_FAITH_PURCHASE && !bCouldAtStartAffordFaithPurchase && bCanNowAffordFaithPurchase)
	{
		CvNotifications* pNotifications = kPlayer.GetNotifications();
		if(pNotifications)
		{
			CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_ENOUGH_FAITH_FOR_MISSIONARY");
			CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ENOUGH_FAITH_FOR_MISSIONARY");
			pNotifications->Add(NOTIFICATION_CAN_BUILD_MISSIONARY, strBuffer, strSummary, -1, -1, -1);
		}
	}

	// Check for pantheon or great prophet spawning (now restricted so must occur before Industrial era)
	if(kPlayer.GetFaith() > 0 && !kPlayer.isMinorCiv() && kPlayer.GetCurrentEra() < GC.getInfoTypeForString("ERA_INDUSTRIAL"))
	{
		if(CanCreatePantheon(kPlayer.GetID(), true) == FOUNDING_OK)
		{
			// Create the pantheon
			if(kPlayer.isHuman())
			{
				//If the player is human then a net message will be received which will pick the pantheon.
				CvNotifications* pNotifications = kPlayer.GetNotifications();
				if(pNotifications)
				{
					CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_ENOUGH_FAITH_FOR_PANTHEON");

					CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ENOUGH_FAITH_FOR_PANTHEON");
					pNotifications->Add(NOTIFICATION_FOUND_PANTHEON, strBuffer, strSummary, -1, -1, -1);
				}
			}
			else
			{
				const BeliefTypes eBelief = kPlayer.GetReligionAI()->ChoosePantheonBelief();

				FoundPantheon(ePlayer, eBelief);
			}
		}

		switch (kPlayer.GetFaithPurchaseType())
		{
		case NO_AUTOMATIC_FAITH_PURCHASE:
		case FAITH_PURCHASE_SAVE_PROPHET:
				CheckSpawnGreatProphet(kPlayer);
				break;
		}
	}

	// Pick a Reformation belief?
	ReligionTypes eReligionCreated = GetReligionCreatedByPlayer(ePlayer);
#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
	if (eReligionCreated > RELIGION_PANTHEON && CanAddReformationBelief(ePlayer) == FOUNDING_OK)
#else
#if !defined(LEKMOD_v34) // Trigger the Reformation belief.
	if (eReligionCreated > RELIGION_PANTHEON && !HasAddedReformationBelief(ePlayer) && kPlayer.GetPlayerPolicies()->HasPolicyGrantingReformationBelief())
#else
	if (eReligionCreated > RELIGION_PANTHEON && // AND
		!HasAddedReformationBelief(ePlayer) && // AND
		(kPlayer.GetPlayerPolicies()->HasPolicyGrantingReformationBelief() || kPlayer.CanChooseReformationBelief())) //HAS EITHER OF THESE
#endif
#endif
	{
		if (!kPlayer.isHuman())
		{
#ifdef AI_CANNOT_FOUND_OR_ENHANCE_OR_SPREAD_RELIGION
			if (!GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_RELIGION_FOUNDING"))
			{
#endif
			BeliefTypes eReformationBelief = kPlayer.GetReligionAI()->ChooseReformationBelief();
			AddReformationBelief(ePlayer, eReligionCreated, eReformationBelief);
#ifdef AI_CANNOT_FOUND_OR_ENHANCE_OR_SPREAD_RELIGION
			}
#endif
		}
		else
		{
			CvNotifications* pNotifications;
			pNotifications = kPlayer.GetNotifications();
			if(pNotifications)
			{
				CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_ADD_REFORMATION_BELIEF");
				CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ADD_REFORMATION_BELIEF");
				pNotifications->Add(NOTIFICATION_ADD_REFORMATION_BELIEF, strBuffer, strSummary, -1, -1, -1);
			}
		}
	}

	// Automatic faith purchases?
	bool bSelectionStillValid = true;
	CvString szItemName = "";
	ReligionTypes eReligion = kPlayer.GetReligionAI()->GetReligionToSpread();

	switch (kPlayer.GetFaithPurchaseType())
	{
	case FAITH_PURCHASE_SAVE_PROPHET:
		if (eReligion <= RELIGION_PANTHEON && GetNumReligionsStillToFound() <= 0)
		{
			szItemName = GetLocalizedText("TXT_KEY_RO_AUTO_FAITH_PROPHET");
			bSelectionStillValid = false;
		}
		else if (kPlayer.GetCurrentEra() >= GC.getInfoTypeForString("ERA_INDUSTRIAL"))
		{
			szItemName = GetLocalizedText("TXT_KEY_RO_AUTO_FAITH_PROPHET");
			bSelectionStillValid = false;
		}
		break;

	case FAITH_PURCHASE_UNIT:
		{
			UnitTypes eUnit = (UnitTypes)kPlayer.GetFaithPurchaseIndex();
			CvUnitEntry *pkUnit = GC.getUnitInfo(eUnit);
			if (pkUnit)
			{
				szItemName = pkUnit->GetDescriptionKey();
			}

			if (!kPlayer.IsCanPurchaseAnyCity(false, false /* Don't worry about faith balance */, eUnit, NO_BUILDING, YIELD_FAITH))
			{
				bSelectionStillValid = false;
			}
			else
			{
				if (kPlayer.IsCanPurchaseAnyCity(true, true /* Check faith balance */, eUnit, NO_BUILDING, YIELD_FAITH))
				{
					CvCity *pCity = CvReligionAIHelpers::GetBestCityFaithUnitPurchase(kPlayer, eUnit, eReligion);
					if (pCity)
					{
						pCity->Purchase(eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH);

						CvNotifications* pNotifications = kPlayer.GetNotifications();
						if(pNotifications)
						{
							CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_AUTOMATIC_FAITH_PURCHASE", szItemName, pCity->getNameKey());
							CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_AUTOMATIC_FAITH_PURCHASE");
							pNotifications->Add(NOTIFICATION_CAN_BUILD_MISSIONARY, strBuffer, strSummary, pCity->getX(), pCity->getY(), -1);
						}
					}
					else
					{
						bSelectionStillValid = false;
					}
				}
			}
		}
		break;
	case FAITH_PURCHASE_BUILDING:
		{
			BuildingTypes eBuilding = (BuildingTypes)kPlayer.GetFaithPurchaseIndex();
			CvBuildingEntry *pkBuilding = GC.getBuildingInfo(eBuilding);
			if (pkBuilding)
			{
				szItemName = pkBuilding->GetDescriptionKey();
			}

			if (!kPlayer.IsCanPurchaseAnyCity(false, false, NO_UNIT, eBuilding, YIELD_FAITH))
			{
				bSelectionStillValid = false;
			}
			else
			{
				if (kPlayer.IsCanPurchaseAnyCity(true, true /* Check faith balance */, NO_UNIT, eBuilding, YIELD_FAITH))
				{
					CvCity *pCity = CvReligionAIHelpers::GetBestCityFaithBuildingPurchase(kPlayer, eBuilding, eReligion);
					if (pCity)
					{
						pCity->Purchase(NO_UNIT, eBuilding, NO_PROJECT, YIELD_FAITH);

						CvNotifications* pNotifications = kPlayer.GetNotifications();
						if(pNotifications)
						{
							CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_AUTOMATIC_FAITH_PURCHASE", szItemName, pCity->getNameKey());
							CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_AUTOMATIC_FAITH_PURCHASE");
							pNotifications->Add(NOTIFICATION_CAN_BUILD_MISSIONARY, strBuffer, strSummary, -1, -1, -1);
						}
					}
					else
					{
						bSelectionStillValid = false;
					}
				}
			}
		}
		break;
	}

	if (!bSelectionStillValid)
	{
		CvNotifications* pNotifications = kPlayer.GetNotifications();
		if(pNotifications)
		{
			CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_NEED_NEW_AUTOMATIC_FAITH_SELECTION", szItemName);
			CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_NEED_NEW_AUTOMATIC_FAITH_SELECTION");
			pNotifications->Add(NOTIFICATION_AUTOMATIC_FAITH_PURCHASE_STOPPED, strBuffer, strSummary, -1, -1, -1);
		}

		gDLL->SendFaithPurchase(kPlayer.GetID(), NO_AUTOMATIC_FAITH_PURCHASE, 0);
	}
}

/// Time to create a pantheon?
CvGameReligions::FOUNDING_RESULT CvGameReligions::CanCreatePantheon(PlayerTypes ePlayer, bool bCheckFaithTotal)
{
	if(ePlayer == NO_PLAYER)
		return FOUNDING_INVALID_PLAYER;

	CvPlayer& kPlayer = GET_PLAYER(ePlayer);
	const int iFaith = kPlayer.GetFaith();

	if(kPlayer.isMinorCiv())
	{
		return FOUNDING_INVALID_PLAYER;
	}

	if(HasCreatedPantheon(ePlayer) || HasCreatedReligion(ePlayer))
	{
		return FOUNDING_PLAYER_ALREADY_CREATED_PANTHEON;
	}

	if(bCheckFaithTotal && iFaith < GetMinimumFaithNextPantheon())
	{
		return FOUNDING_NOT_ENOUGH_FAITH;
	}

	// Has a religion been enhanced yet (and total number of religions/pantheons is equal to number of religions allowed)?
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_bEnhanced)
		{
			if (GetNumPantheonsCreated() >= GC.getMap().getWorldInfo().getMaxActiveReligions())
			{
				return FOUNDING_RELIGION_ENHANCED;
			}
		}
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem) 
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);

		// Attempt to execute the game events.
		// Will return false if there are no registered listeners.
		bool bResult = false;
		if (LuaSupport::CallTestAll(pkScriptSystem, "PlayerCanFoundPantheon", args.get(), bResult))
		{
			if (bResult == false) 
			{
				return FOUNDING_INVALID_PLAYER;
			}
		}
	}

#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
	if (!IsAnyAvailablePantheonBeliefs())
#else
	if (GetAvailablePantheonBeliefs().size() == 0)
#endif
		return FOUNDING_NO_BELIEFS_AVAILABLE;

	return FOUNDING_OK;
}

/// Get the appropriate religion for this player to found next
ReligionTypes CvGameReligions::GetReligionToFound(PlayerTypes ePlayer)
{
	ReligionTypes eCivReligion;
	eCivReligion = GET_PLAYER(ePlayer).getCivilizationInfo().GetReligion();

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem) 
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);
		args->Push(eCivReligion);
		args->Push(HasBeenFounded(eCivReligion));

		int iValue = 0;
		if (LuaSupport::CallAccumulator(pkScriptSystem, "GetReligionToFound", args.get(), iValue)) 
		{
#ifdef AUI_WARNING_FIXES
			if (uint(iValue) < GC.getNumReligionInfos() && iValue != RELIGION_PANTHEON)
#else
			if (iValue >= 0 && iValue < GC.getNumReligionInfos() && iValue != RELIGION_PANTHEON)
#endif
			{
				eCivReligion = (ReligionTypes)iValue;
			}
		}
	}

	if(!HasBeenFounded(eCivReligion))
	{
		CvReligionEntry* pEntry = GC.getReligionInfo(eCivReligion);
		if(pEntry)
		{
			return eCivReligion;
		}
	}

	// Need to "borrow" from another civ.  Loop through all religions looking for one that is eligible
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumReligionInfos(); iI++)
#else
	for(int iI = 0; iI < GC.getNumReligionInfos(); iI++)
#endif
	{
		ReligionTypes eReligion = (ReligionTypes)iI;
		CvReligionEntry* pEntry = GC.getReligionInfo(eReligion);
		if(!pEntry)
		{
			continue;
		}

		if(pEntry->GetID() == RELIGION_PANTHEON)
		{
			continue;
		}

		if(HasBeenFounded((ReligionTypes)pEntry->GetID()))
		{
			continue;
		}

		if(IsPreferredByCivInGame(eReligion))
		{
			continue;
		}

		return (eReligion);
	}

	// Will have to use a religion that someone else prefers
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumReligionInfos(); iI++)
#else
	for(int iI = 0; iI < GC.getNumReligionInfos(); iI++)
#endif
	{
		ReligionTypes eReligion = (ReligionTypes)iI;
		CvReligionEntry* pEntry = GC.getReligionInfo(eReligion);
		if(!pEntry)
		{
			continue;
		}

		if(pEntry->GetID() == RELIGION_PANTHEON)
		{
			continue;
		}

		if(HasBeenFounded((ReligionTypes)pEntry->GetID()))
		{
			continue;
		}

		return (eReligion);
	}

	return NO_RELIGION;
}

/// Tell the game a new pantheon has been created
void CvGameReligions::FoundPantheon(PlayerTypes ePlayer, BeliefTypes eBelief)
{
	CvGame& kGame = GC.getGame();
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);

	CvReligion newReligion(RELIGION_PANTHEON, ePlayer, NULL, true);
	newReligion.m_Beliefs.AddBelief(eBelief);

	// Found it
	m_CurrentReligions.push_back(newReligion);

	// Update game systems
	kPlayer.UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	kPlayer.doSelfConsistencyCheckAllCities();
#endif
	kPlayer.ChangeFaith(-GetMinimumFaithNextPantheon());

	int iIncrement = GC.getRELIGION_GAME_FAITH_DELTA_NEXT_PANTHEON();
	iIncrement *= GC.getGame().getGameSpeedInfo().getTrainPercent();
	iIncrement /= 100;
	SetMinimumFaithNextPantheon(GetMinimumFaithNextPantheon() + iIncrement);

#ifdef AUI_RELIGION_FIX_FOUND_PANTHEON_NULL_POINTER_DEREFERENCE
	CvCity* pCapitol = GET_PLAYER(ePlayer).getCapitalCity();
	if (pCapitol)
	{
#endif
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem) 
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);
#ifdef AUI_RELIGION_FIX_FOUND_PANTHEON_NULL_POINTER_DEREFERENCE
		args->Push(pCapitol->GetID());
#else
		args->Push(GET_PLAYER(ePlayer).getCapitalCity()->GetID());
#endif
		args->Push(RELIGION_PANTHEON);
		args->Push(eBelief);

		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "PantheonFounded", args.get(), bResult);
	}
#ifdef AUI_RELIGION_FIX_FOUND_PANTHEON_NULL_POINTER_DEREFERENCE
	}
#endif

	// Spread the pantheon into each of their cities
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
	{
		// Add enough pressure to make this the likely majority religion
		pLoopCity->GetCityReligions()->AddReligiousPressure(FOLLOWER_CHANGE_PANTHEON_FOUNDED, newReligion.m_eReligion, GC.getRELIGION_ATHEISM_PRESSURE_PER_POP() * pLoopCity->getPopulation() * 2);
	}

	// Send out messaging
	CvReligionEntry* pEntry = GC.getReligionInfo(newReligion.m_eReligion);
	CvBeliefEntry* pBelief = GC.getBeliefInfo(eBelief);
	if(pEntry && pBelief)
	{
		//Add replay message.
		Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_PANTHEON_FOUNDED_S");
		Localization::String replayText = Localization::Lookup("TXT_KEY_NOTIFICATION_PANTHEON_FOUNDED");
		replayText << kPlayer.getCivilizationShortDescriptionKey() << pBelief->getShortDescription() << pBelief->GetDescriptionKey();

		kGame.addReplayMessage(REPLAY_MESSAGE_PANTHEON_FOUNDED, newReligion.m_eFounder, replayText.toUTF8());

		for(int iNotifyLoop = 0; iNotifyLoop < MAX_MAJOR_CIVS; ++iNotifyLoop){
			PlayerTypes eNotifyPlayer = (PlayerTypes) iNotifyLoop;
			CvPlayerAI& kCurNotifyPlayer = GET_PLAYER(eNotifyPlayer);
			CvNotifications* pNotifications = kCurNotifyPlayer.GetNotifications();
			if(pNotifications){
				// Message slightly different for founder player
				if(newReligion.m_eFounder == eNotifyPlayer)
				{
					Localization::String localizedText = GetLocalizedText("TXT_KEY_NOTIFICATION_PANTHEON_FOUNDED_ACTIVE_PLAYER");
					localizedText << pBelief->getShortDescription() << pBelief->GetDescriptionKey();

					pNotifications->Add(NOTIFICATION_PANTHEON_FOUNDED_ACTIVE_PLAYER, localizedText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
				else
				{
					//If the notify player has not met this civ yet, display a more ambiguous notification.
					CvTeam& kTeam = GET_TEAM(kCurNotifyPlayer.getTeam());
					if(kTeam.isHasMet(kPlayer.getTeam()))
					{
						pNotifications->Add(NOTIFICATION_PANTHEON_FOUNDED, replayText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
					}
					else
					{
						Localization::String unknownFoundedText = Localization::Lookup("TXT_KEY_NOTIFICATION_PANTHEON_FOUNDED_UNKNOWN");
						unknownFoundedText << pBelief->getShortDescription() << pBelief->GetDescriptionKey();

						pNotifications->Add(NOTIFICATION_PANTHEON_FOUNDED, unknownFoundedText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
					}
				}
			}
		}

		// Logging
		if(GC.getLogging())
		{
			CvString strLogMsg;
			strLogMsg = kPlayer.getCivilizationShortDescription();
			strLogMsg += ", PANTHEON FOUNDED, ";
			strLogMsg += pEntry->GetDescription();
			LogReligionMessage(strLogMsg);
		}

		//Achievements!
		if(ePlayer == GC.getGame().getActivePlayer())
		{
			gDLL->UnlockAchievement(ACHIEVEMENT_XP1_10);
		}
	}

	GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
}

/// Create a new religion
void CvGameReligions::FoundReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, BeliefTypes eBelief3, BeliefTypes eBelief4, CvCity* pkHolyCity)
{
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);

	CvReligion kReligion(eReligion, ePlayer, pkHolyCity, false);

	// Copy over belief from your pantheon
	BeliefTypes eBelief = GC.getGame().GetGameReligions()->GetBeliefInPantheon(kPlayer.GetID());
	if(eBelief != NO_BELIEF)
		kReligion.m_Beliefs.AddBelief(eBelief);

	kReligion.m_Beliefs.AddBelief(eBelief1);
	kReligion.m_Beliefs.AddBelief(eBelief2);

	if(eBelief3 != NO_BELIEF)
	{
		kReligion.m_Beliefs.AddBelief(eBelief3);
	}

	if(eBelief4 != NO_BELIEF)
	{
		kReligion.m_Beliefs.AddBelief(eBelief4);
	}

	if(szCustomName != NULL && strlen(szCustomName) <= sizeof(kReligion.m_szCustomName))
	{
		strcpy_s(kReligion.m_szCustomName, szCustomName);
	}

	// Found it
	m_CurrentReligions.push_back(kReligion);

	// Inform the holy city
	pkHolyCity->GetCityReligions()->DoReligionFounded(kReligion.m_eReligion);

	// Update game systems
	kPlayer.UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	kPlayer.doSelfConsistencyCheckAllCities();
#endif
	kPlayer.GetReligions()->SetFoundingReligion(false);

#ifdef NQ_ALLOW_RELIGION_ONE_SHOTS
	kPlayer.DoReligionOneShots(eReligion);
#endif

	// In case we have another prophet sitting around, make sure he's set to this religion
	int iLoopUnit;
	CvUnit* pLoopUnit;
	for(pLoopUnit = kPlayer.firstUnit(&iLoopUnit); pLoopUnit != NULL; pLoopUnit = kPlayer.nextUnit(&iLoopUnit))
	{
		if(pLoopUnit->getUnitInfo().IsFoundReligion())
		{
			pLoopUnit->GetReligionData()->SetReligion(eReligion);
			pLoopUnit->GetReligionData()->SetSpreadsLeft(pLoopUnit->getUnitInfo().GetReligionSpreads());
			pLoopUnit->GetReligionData()->SetReligiousStrength(pLoopUnit->getUnitInfo().GetReligiousStrength());
		}
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);
		args->Push(pkHolyCity->GetID());
		args->Push(eReligion);
		args->Push(eBelief);
		args->Push(eBelief1);
		args->Push(eBelief2);
		args->Push(eBelief3);
		args->Push(eBelief4);

		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "ReligionFounded", args.get(), bResult);
	}

	// Send out messaging
	CvReligionEntry* pEntry = GC.getReligionInfo(kReligion.m_eReligion);
	if(pEntry)
	{
		//Add replay message
		CvString szReligionName = kReligion.GetName();
		Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_FOUNDED_S");
		Localization::String replayText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_FOUNDED");
		replayText << kPlayer.getCivilizationShortDescriptionKey() << szReligionName << pkHolyCity->getNameKey();

		GC.getGame().addReplayMessage(REPLAY_MESSAGE_RELIGION_FOUNDED, kReligion.m_eFounder, replayText.toUTF8(), kReligion.m_iHolyCityX, kReligion.m_iHolyCityY);

		//Notify the masses
		for(int iNotifyLoop = 0; iNotifyLoop < MAX_MAJOR_CIVS; ++iNotifyLoop){
			PlayerTypes eNotifyPlayer = (PlayerTypes) iNotifyLoop;
			CvPlayerAI& kNotifyPlayer = GET_PLAYER(eNotifyPlayer);
			CvNotifications* pNotifications = kNotifyPlayer.GetNotifications();
			if(pNotifications){
				// Message slightly different for founder player
				if(kReligion.m_eFounder == eNotifyPlayer)
				{
					Localization::String localizedText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_FOUNDED_ACTIVE_PLAYER");
					localizedText << szReligionName << pkHolyCity->getNameKey();

					pNotifications->Add(NOTIFICATION_RELIGION_FOUNDED_ACTIVE_PLAYER, localizedText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
				else
				{
					CvTeam& kNotifyTeam = GET_TEAM(kNotifyPlayer.getTeam());

					if(kNotifyTeam.isHasMet(kPlayer.getTeam()))
					{
						pNotifications->Add(NOTIFICATION_RELIGION_FOUNDED, replayText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
					}
					else
					{
						Localization::String unknownCivText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_FOUNDED_UNKNOWN");
						unknownCivText << szReligionName;

						pNotifications->Add(NOTIFICATION_RELIGION_FOUNDED, unknownCivText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
					}
				}
			}
		}

		// Logging
		if(GC.getLogging())
		{
			CvString strLogMsg;
			strLogMsg = kPlayer.getCivilizationShortDescription();
			strLogMsg += ", RELIGION FOUNDED, ";
			strLogMsg += pkHolyCity->getName();
			strLogMsg += ", ";
			strLogMsg += pEntry->GetDescription();
			LogReligionMessage(strLogMsg);
		}

		//Achievements!
		if(ePlayer == GC.getGame().getActivePlayer())
		{
			gDLL->UnlockAchievement(ACHIEVEMENT_XP1_11);
		}
	}
	GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
}

/// Can the supplied religion be created?
CvGameReligions::FOUNDING_RESULT CvGameReligions::CanFoundReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, BeliefTypes eBelief3, BeliefTypes eBelief4, CvCity* pkHolyCity)
{
	if(GetNumReligionsStillToFound() <= 0)
		return FOUNDING_NO_RELIGIONS_AVAILABLE;

	if(ePlayer == NO_PLAYER)
		return FOUNDING_INVALID_PLAYER;

	if(HasCreatedReligion(ePlayer))
		return FOUNDING_PLAYER_ALREADY_CREATED_RELIGION;

	CvPlayer& kPlayer = GET_PLAYER(ePlayer);

#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
	if (!IsAnyAvailableFounderBeliefs() || !IsAnyAvailableFollowerBeliefs())
	{
		return FOUNDING_NO_BELIEFS_AVAILABLE;
	}
	if (eReligion == NO_RELIGION || pkHolyCity == NULL || eBelief1 == NO_BELIEF || eBelief2 == NO_BELIEF)
	{
		if (kPlayer.GetReligions()->IsFoundingReligion())
		{
			return FOUNDING_PLAYER_ALREADY_CREATED_RELIGION;
		}
		return FOUNDING_OK;
	}
#endif

	CvReligion kReligion(eReligion, ePlayer, pkHolyCity, false);

	// Copy over belief from your pantheon
	BeliefTypes eBelief = GC.getGame().GetGameReligions()->GetBeliefInPantheon(kPlayer.GetID());
	if(eBelief != NO_BELIEF)
		kReligion.m_Beliefs.AddBelief(eBelief);

	kReligion.m_Beliefs.AddBelief(eBelief1);
	kReligion.m_Beliefs.AddBelief(eBelief2);

	if(eBelief3 != NO_BELIEF)
	{
		kReligion.m_Beliefs.AddBelief(eBelief3);
	}

	if(eBelief4 != NO_BELIEF)
	{
		kReligion.m_Beliefs.AddBelief(eBelief4);
	}

	if(szCustomName != NULL && strlen(szCustomName) <= sizeof(kReligion.m_szCustomName))
	{
		strcpy_s(kReligion.m_szCustomName, szCustomName);
	}

	// Now see if there are any conflicts.
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	for(ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if ((*it).m_eFounder != ePlayer)	// Only check other player's religions
		{
			if(kReligion.m_eReligion == (*it).m_eReligion)
				return FOUNDING_RELIGION_IN_USE;

			for(int iSrcBelief = (*it).m_Beliefs.GetNumBeliefs(); iSrcBelief--;)
			{
				BeliefTypes eSrcBelief = (*it).m_Beliefs.GetBelief(iSrcBelief);
				if(eSrcBelief != NO_BELIEF)
				{
					for(int iDestBelief = kReligion.m_Beliefs.GetNumBeliefs(); iDestBelief--;)
					{
						BeliefTypes eDestBelief = kReligion.m_Beliefs.GetBelief(iDestBelief);
						if(eDestBelief != NO_BELIEF && eDestBelief == eSrcBelief)
							return FOUNDING_BELIEF_IN_USE;
					}
				}
			}
		}
	}

	return FOUNDING_OK;
}

/// Add new beliefs to an existing religion
void CvGameReligions::EnhanceReligion(PlayerTypes ePlayer, ReligionTypes eReligion, BeliefTypes eBelief1, BeliefTypes eBelief2)
{
	bool bFoundIt = false;
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);
	ReligionList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			bFoundIt = true;
			break;
		}
	}
	if(!bFoundIt)
	{
		CvAssertMsg(false, "Internal error in religion code.");
		return;
	}

#ifdef AUI_WARNING_FIXES
	it->m_bEnhanced = true;
#endif
	it->m_Beliefs.AddBelief(eBelief1);
	it->m_Beliefs.AddBelief(eBelief2);
#ifndef AUI_WARNING_FIXES
	it->m_bEnhanced = true;
#endif
#if defined(LEKMOD_v34)
	// Get the player's Traits
	CvPlayerTraits* pPlayerTraits = kPlayer.GetPlayerTraits();
	// Now see if thier trait lets them pick a reformation belief
	if (pPlayerTraits->IsReligionEnhanceReformation() == true)
	{
		// If they can, and they haven't already, give them the option to pick one
		if (!HasAddedReformationBelief(ePlayer))
		{
			CvNotifications* pNotifications;
			pNotifications = kPlayer.GetNotifications();
			if (pNotifications)
			{
				CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_ADD_REFORMATION_BELIEF");
				CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ADD_REFORMATION_BELIEF");
				pNotifications->Add(NOTIFICATION_ADD_REFORMATION_BELIEF, strBuffer, strSummary, -1, -1, -1);
			}
		}
	}
#endif
	// Update game systems
	UpdateAllCitiesThisReligion(eReligion);
	kPlayer.UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	kPlayer.doSelfConsistencyCheckAllCities();
#endif
#ifdef AUI_RELIGION_FIX_SIMULTANEOUS_ENHANCE_OR_FOUND_CAUSING_MULTIPLE
	kPlayer.GetReligions()->SetFoundingReligion(false);
#endif

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem) 
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);
		args->Push(eReligion);
		args->Push(eBelief1);
		args->Push(eBelief2);

		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "ReligionEnhanced", args.get(), bResult);
	}

#ifdef NQ_ALLOW_RELIGION_ONE_SHOTS
	kPlayer.DoReligionOneShots(eReligion);
#endif

	//Notify the masses
	for(int iNotifyLoop = 0; iNotifyLoop < MAX_MAJOR_CIVS; ++iNotifyLoop){
		PlayerTypes eNotifyPlayer = (PlayerTypes) iNotifyLoop;
		CvPlayerAI& kNotifyPlayer = GET_PLAYER(eNotifyPlayer);
		CvNotifications* pNotifications = kNotifyPlayer.GetNotifications();
		if(pNotifications){
			Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_ENHANCED_S");
			Localization::String notificationText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_ENHANCED");
			notificationText << kPlayer.getCivilizationShortDescriptionKey() << it->GetName();

			// Message slightly different for enhancing player
			if(ePlayer == eNotifyPlayer)
			{
				Localization::String localizedText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_ENHANCED_ACTIVE_PLAYER");
				localizedText << it->GetName();

				pNotifications->Add(NOTIFICATION_RELIGION_ENHANCED_ACTIVE_PLAYER, localizedText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
			}
			else
			{
				CvTeam& kNotifyTeam = GET_TEAM(kNotifyPlayer.getTeam());
				if(kNotifyTeam.isHasMet(kPlayer.getTeam()))
				{
					pNotifications->Add(NOTIFICATION_RELIGION_ENHANCED, notificationText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
				else
				{
					Localization::String unknownText = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_ENHANCED_UNKNOWN");
					unknownText << it->GetName();

					pNotifications->Add(NOTIFICATION_RELIGION_ENHANCED, unknownText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
			}
		}

		// Logging
		if(GC.getLogging())
		{
			CvString strLogMsg;
			strLogMsg = kPlayer.getCivilizationShortDescription();
			strLogMsg += ", RELIGION ENHANCED";
			LogReligionMessage(strLogMsg);
		}
	}
	GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
}

/// Can the new beliefs be added to the religion?
CvGameReligions::FOUNDING_RESULT CvGameReligions::CanEnhanceReligion(PlayerTypes ePlayer, ReligionTypes eReligion, BeliefTypes eBelief1, BeliefTypes eBelief2)
{
#ifdef AUI_WARNING_FIXES
	if (eBelief1 != NO_BELIEF && IsInSomeReligion(eBelief1))
		return FOUNDING_BELIEF_IN_USE;
	if (eBelief2 != NO_BELIEF && IsInSomeReligion(eBelief2))
		return FOUNDING_BELIEF_IN_USE;
#else
	bool bFoundIt = false;
#endif
#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
	if (eReligion == NO_RELIGION)
		return FOUNDING_INVALID_PLAYER;
	if (!IsAnyAvailableEnhancerBeliefs() || !IsAnyAvailableFollowerBeliefs())
	{
		return FOUNDING_NO_BELIEFS_AVAILABLE;
	}
	if (eBelief1 == NO_BELIEF || eBelief2 == NO_BELIEF)
	{
		if (GET_PLAYER(ePlayer).GetReligions()->IsFoundingReligion())
		{
			return FOUNDING_PLAYER_ALREADY_CREATED_RELIGION;
		}
	}
#endif
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion && it->m_eFounder == ePlayer)
		{
#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
			if (it->m_bEnhanced)
				return FOUNDING_RELIGION_ENHANCED;
#endif
#ifdef AUI_WARNING_FIXES
			return FOUNDING_OK;
#else
			bFoundIt = true;
			break;
#endif
		}
	}

#ifndef AUI_WARNING_FIXES
	if(bFoundIt)
	{
		if(eBelief1 != NO_BELIEF && IsInSomeReligion(eBelief1))
			return FOUNDING_BELIEF_IN_USE;
		if(eBelief2 != NO_BELIEF && IsInSomeReligion(eBelief2))
			return FOUNDING_BELIEF_IN_USE;

		return FOUNDING_OK;
	}
#endif

	return FOUNDING_RELIGION_IN_USE;
}

/// Add an extra belief to a religion (through a policy)
void CvGameReligions::AddReformationBelief(PlayerTypes ePlayer, ReligionTypes eReligion, BeliefTypes eBelief1)
{
	bool bFoundIt = false;
	CvPlayer& kPlayer = GET_PLAYER(ePlayer);
	ReligionList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			bFoundIt = true;
			break;
		}
	}
	if(!bFoundIt)
	{
		CvAssertMsg(false, "Internal error in religion code.");
		return;
	}

	it->m_Beliefs.AddBelief(eBelief1);

	// Update game systems
	UpdateAllCitiesThisReligion(eReligion);
	kPlayer.UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
	kPlayer.doSelfConsistencyCheckAllCities();
#endif

#ifdef NQ_ADD_REFORMATION_LUA_HOOK
	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem) 
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);
		args->Push(eReligion);
		args->Push(eBelief1);
		bool bResult;
		LuaSupport::CallHook(pkScriptSystem, "ReformationAdded", args.get(), bResult);
	}
#endif

#ifdef NQ_ALLOW_RELIGION_ONE_SHOTS
	kPlayer.DoReligionOneShots(eReligion);
#endif
#ifdef LEKMOD_REFORMATION_NOTIFICATION_MID_TURN
	kPlayer.DoReformationNotification();
#endif

	//Notify the masses
	for(int iNotifyLoop = 0; iNotifyLoop < MAX_MAJOR_CIVS; ++iNotifyLoop){
		PlayerTypes eNotifyPlayer = (PlayerTypes) iNotifyLoop;
		CvPlayerAI& kNotifyPlayer = GET_PLAYER(eNotifyPlayer);
		CvNotifications* pNotifications = kNotifyPlayer.GetNotifications();
		if(pNotifications){
			Localization::String strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_REFORMATION_BELIEF_ADDED_S");
			Localization::String notificationText = Localization::Lookup("TXT_KEY_NOTIFICATION_REFORMATION_BELIEF_ADDED");
			notificationText << kPlayer.getCivilizationShortDescriptionKey() << it->GetName();

			// Message slightly different for reformation player
			if(ePlayer == eNotifyPlayer)
			{
				Localization::String localizedText = Localization::Lookup("TXT_KEY_NOTIFICATION_REFORMATION_BELIEF_ADDED_ACTIVE_PLAYER");
				localizedText << it->GetName();

				pNotifications->Add(NOTIFICATION_REFORMATION_BELIEF_ADDED_ACTIVE_PLAYER, localizedText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
			}
			else
			{
				CvTeam& kNotifyTeam = GET_TEAM(kNotifyPlayer.getTeam());
				if(kNotifyTeam.isHasMet(kPlayer.getTeam()))
				{
					pNotifications->Add(NOTIFICATION_REFORMATION_BELIEF_ADDED, notificationText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
				else
				{
					Localization::String unknownText = Localization::Lookup("TXT_KEY_NOTIFICATION_REFORMATION_BELIEF_ADDED_UNKNOWN");
					unknownText << it->GetName();

					pNotifications->Add(NOTIFICATION_REFORMATION_BELIEF_ADDED, unknownText.toUTF8(), strSummary.toUTF8(), -1, -1, -1);
				}
			}
		}

		// Logging
		if(GC.getLogging())
		{
			CvString strLogMsg;
			strLogMsg = kPlayer.getCivilizationShortDescription();
			strLogMsg += ", REFORMATION BELIEF ADDED";
			LogReligionMessage(strLogMsg);
		}
	}

	GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
}

#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
CvGameReligions::FOUNDING_RESULT CvGameReligions::CanAddReformationBelief(PlayerTypes ePlayer, BeliefTypes eBelief) const
{
	if (ePlayer == NO_PLAYER)
	{
		return FOUNDING_INVALID_PLAYER;
	}
	if (!GET_PLAYER(ePlayer).GetPlayerPolicies()->HasPolicyGrantingReformationBelief())
	{
		return FOUNDING_INVALID_PLAYER;
	}
	if (!IsAnyAvailableReformationBeliefs())
	{
		return FOUNDING_NO_BELIEFS_AVAILABLE;
	}
	if (HasAddedReformationBelief(ePlayer))
	{
		return FOUNDING_PLAYER_ALREADY_CREATED_RELIGION;
	}
	if (!GET_PLAYER(ePlayer).GetReligions()->HasCreatedReligion())
	{
		return FOUNDING_INVALID_PLAYER;
	}
	if (eBelief != NO_BELIEF && IsInSomeReligion(eBelief))
	{
		return FOUNDING_BELIEF_IN_USE;
	}

	return FOUNDING_OK;
}
#endif

/// Move the Holy City for a religion (useful for scenario scripting)
void CvGameReligions::SetHolyCity(ReligionTypes eReligion, CvCity* pkHolyCity)
{
	ReligionList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		// If talking about a pantheon, make sure to match the player
		if(it->m_eReligion == eReligion)
		{
			it->m_iHolyCityX = pkHolyCity->getX();
			it->m_iHolyCityY = pkHolyCity->getY();
			break;
		}
	}
}

/// Switch founder for a religion (useful for scenario scripting)
void CvGameReligions::SetFounder(ReligionTypes eReligion, PlayerTypes eFounder)
{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		// If talking about a pantheon, make sure to match the player
		if(it->m_eReligion == eReligion)
		{
			it->m_eFounder = eFounder;
			break;
		}
	}
}

/// After a religion is enhanced, the newly chosen beliefs need to be turned on in all cities
void CvGameReligions::UpdateAllCitiesThisReligion(ReligionTypes eReligion)
{
	int iLoop;

	for(int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		PlayerTypes ePlayer = (PlayerTypes)iPlayer;
		CvPlayer& kPlayer = GET_PLAYER(ePlayer);
		if(kPlayer.isAlive())
		{
			for(CvCity* pCity = kPlayer.firstCity(&iLoop); pCity != NULL; pCity = kPlayer.nextCity(&iLoop))
			{
				if(pCity->GetCityReligions()->GetReligiousMajority() == eReligion)
				{
					pCity->UpdateReligion(eReligion);
				}
			}
		}
	}
}

/// Return a pointer to a religion that has been founded
const CvReligion* CvGameReligions::GetReligion(ReligionTypes eReligion, PlayerTypes ePlayer) const
{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		// If talking about a pantheon, make sure to match the player
#ifdef AUI_WARNING_FIXES
		if (it->m_eReligion == eReligion)
		{
			if (it->m_eReligion != RELIGION_PANTHEON || it->m_eFounder == ePlayer)
#else
		if(it->m_eReligion == eReligion && it->m_eReligion == RELIGION_PANTHEON)
		{
			if(it->m_eFounder == ePlayer)
#endif
			{
				return it;
			}
		}
#ifndef AUI_WARNING_FIXES
		else if(it->m_eReligion == eReligion)
		{
			return it;
		}
#endif
	}

	return NULL;
}

/// Has some religion already claimed this belief?
bool CvGameReligions::IsInSomeReligion(BeliefTypes eBelief) const
{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_Beliefs.HasBelief(eBelief))
		{
			return true;
		}
	}

	return false;
}

/// Get the belief in this player's pantheon
BeliefTypes CvGameReligions::GetBeliefInPantheon(PlayerTypes ePlayer) const
{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eFounder == ePlayer && it->m_bPantheon)
		{
			return (BeliefTypes)it->m_Beliefs.GetBelief(0);
		}
	}

	return NO_BELIEF;
}

/// Has this player created a pantheon?
bool CvGameReligions::HasCreatedPantheon(PlayerTypes ePlayer) const
{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eFounder == ePlayer && it->m_bPantheon)
		{
			return true;
		}
	}

	return false;
}

/// How many players have created a pantheon?
int CvGameReligions::GetNumPantheonsCreated() const
{
	int iRtnValue = 0;

	for(int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
		for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
		ReligionList::const_iterator it;
		for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
		{
			if (it->m_eFounder == iI)
			{
				iRtnValue++;
				break;
			}
		}
	}

	return iRtnValue;
}

/// List of beliefs that can be adopted by pantheons
std::vector<BeliefTypes> CvGameReligions::GetAvailablePantheonBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && pEntry->IsPantheonBelief())
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && pEntry->IsPantheonBelief())
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

// Is the supplied belief available to a pantheon?
#ifdef AUI_CONSTIFY
bool CvGameReligions::IsPantheonBeliefAvailable(BeliefTypes eBelief) const
#else
bool CvGameReligions::IsPantheonBeliefAvailable(BeliefTypes eBelief)
#endif
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
#ifdef AUI_WARNING_FIXES
	CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
	if (pEntry && pEntry->IsPantheonBelief())
	{
		if (!IsInSomeReligion(eBelief))
#else
	if(!IsInSomeReligion(eBelief))
	{
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if(pEntry && pEntry->IsPantheonBelief())
#endif
		{
			return true;
		}
	}

	return false;
}

/// Number of followers of this religion
int CvGameReligions::GetNumFollowers(ReligionTypes eReligion) const
{
	int iRtnValue = 0;

	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				iRtnValue += pLoopCity->GetCityReligions()->GetNumFollowers(eReligion);
			}
		}
	}
	return iRtnValue;
}

/// Number of cities following this religion
int CvGameReligions::GetNumCitiesFollowing(ReligionTypes eReligion) const
{
	int iRtnValue = 0;

	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				if(pLoopCity->GetCityReligions()->GetReligiousMajority() == eReligion)
				{
					iRtnValue++;
				}
			}
		}
	}
	return iRtnValue;
}

/// Has this player created a religion?
bool CvGameReligions::HasCreatedReligion(PlayerTypes ePlayer) const
{
#ifdef AUI_WARNING_FIXES
	return GetReligionCreatedByPlayer(ePlayer) > RELIGION_PANTHEON;
#else
    if (GetReligionCreatedByPlayer(ePlayer) > RELIGION_PANTHEON)
	{
		return true;
	}

	return false;
#endif
}

/// Has this player reformed their religion?
bool CvGameReligions::HasAddedReformationBelief(PlayerTypes ePlayer) const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();

	ReligionTypes eReligion = GetReligionCreatedByPlayer(ePlayer);
    if (eReligion > RELIGION_PANTHEON)
	{
		const CvReligion* pMyReligion = GetReligion(eReligion, ePlayer);
		if (pMyReligion)
		{
			for(int iI = 0; iI < pMyReligion->m_Beliefs.GetNumBeliefs(); iI++)
			{
				const BeliefTypes eBelief = pMyReligion->m_Beliefs.GetBelief(iI);
				CvBeliefEntry* pEntry = pkBeliefs->GetEntry((int)eBelief);
				if (pEntry && pEntry->IsReformationBelief())
				{
					return true;
				}
			}
		}
	}

	return false;
}

/// Does this player qualify for founder benefits?
bool CvGameReligions::IsEligibleForFounderBenefits(ReligionTypes eReligion, PlayerTypes ePlayer) const
{
	const CvReligion* religion = GetReligion(eReligion, NO_PLAYER);
	if(religion)
	{
		CvPlot* pHolyCityPlot = NULL;
		pHolyCityPlot = GC.getMap().plot(religion->m_iHolyCityX, religion->m_iHolyCityY);

		if(religion->m_eFounder == ePlayer)
		{
			if(!religion->m_bPantheon)
			{
				if(pHolyCityPlot && pHolyCityPlot->getOwner() == ePlayer)
				{
					return true;
				}
			}
		}
	}

	return false;
}

/// Is this city state friendly with the player that founded this religion?
#ifdef AUI_CONSTIFY
bool CvGameReligions::IsCityStateFriendOfReligionFounder(ReligionTypes eReligion, PlayerTypes ePlayer) const
#else
bool CvGameReligions::IsCityStateFriendOfReligionFounder(ReligionTypes eReligion, PlayerTypes ePlayer)
#endif
{
	const CvReligion* religion = GetReligion(eReligion, NO_PLAYER);
	if(religion)
	{
		CvPlayer& kMinor = GET_PLAYER(ePlayer);
		CvPlayer& kFounder = GET_PLAYER(religion->m_eFounder);
		if(!kFounder.isMinorCiv() && kMinor.GetMinorCivAI()->IsFriends(religion->m_eFounder))
		{
			return true;
		}
	}

	return false;
}

/// Get the religion this player created
ReligionTypes CvGameReligions::GetReligionCreatedByPlayer(PlayerTypes ePlayer) const
{
	ReligionTypes eRtnValue = NO_RELIGION;

#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eFounder == ePlayer)
		{
			if(!it->m_bPantheon)
			{
				eRtnValue = it->m_eReligion;
#ifdef AUI_WARNING_FIXES
				break;
#endif
			}
		}
	}

	return eRtnValue;
}

/// Get the religion for which this player is eligible for founder benefits
ReligionTypes CvGameReligions::GetFounderBenefitsReligion(PlayerTypes ePlayer) const
{
	ReligionTypes eReligion;

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(ePlayer);

		int iValue = 0;
		if (LuaSupport::CallAccumulator(pkScriptSystem, "GetFounderBenefitsReligion", args.get(), iValue))
		{
			eReligion = (ReligionTypes)iValue;
			return eReligion;
		}
	}

	eReligion = GetReligionCreatedByPlayer(ePlayer);

	if(IsEligibleForFounderBenefits(eReligion, ePlayer))
	{
		return eReligion;
	}

	return NO_RELIGION;
}

/// Number of religions founded so far (does not include pantheons)
int CvGameReligions::GetNumReligionsFounded() const
{
	int iRtnValue = 0;

#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(!it->m_bPantheon)
		{
			iRtnValue++;
		}
	}

	return iRtnValue;
}

/// Number of religions enhanced so far
int CvGameReligions::GetNumReligionsEnhanced() const
{
	int iRtnValue = 0;

#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (ReligionList::const_iterator it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	ReligionList::const_iterator it;
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if (it->m_bEnhanced)
		{
			iRtnValue++;
		}
	}

	return iRtnValue;
}

/// Number of religions that still can be founded on this size map
int CvGameReligions::GetNumReligionsStillToFound() const
{
	return (GC.getMap().getWorldInfo().getMaxActiveReligions() - GetNumReligionsFounded());
}

/// List of beliefs that can be adopted by religion founders
std::vector<BeliefTypes> CvGameReligions::GetAvailableFounderBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && pEntry->IsFounderBelief())
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && pEntry->IsFounderBelief())
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

/// List of beliefs that can be adopted by religion followers
std::vector<BeliefTypes> CvGameReligions::GetAvailableFollowerBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && pEntry->IsFollowerBelief())
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && pEntry->IsFollowerBelief())
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

/// List of beliefs that enhance religions
std::vector<BeliefTypes> CvGameReligions::GetAvailableEnhancerBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && pEntry->IsEnhancerBelief())
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && pEntry->IsEnhancerBelief())
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

/// List of all beliefs still available
std::vector<BeliefTypes> CvGameReligions::GetAvailableBonusBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && (pEntry->IsEnhancerBelief() || pEntry->IsFollowerBelief() || pEntry->IsFounderBelief() || pEntry->IsPantheonBelief()))
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && (pEntry->IsEnhancerBelief() || pEntry->IsFollowerBelief() || pEntry->IsFounderBelief() || pEntry->IsPantheonBelief()))
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

/// List of beliefs that are added with Reformation social policy
std::vector<BeliefTypes> CvGameReligions::GetAvailableReformationBeliefs()
{
	std::vector<BeliefTypes> availableBeliefs;

	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const int iNumBeleifs = pkBeliefs->GetNumBeliefs();

	availableBeliefs.reserve(iNumBeleifs);
	for(int iI = 0; iI < iNumBeleifs; iI++)
	{
		const BeliefTypes eBelief(static_cast<BeliefTypes>(iI));
#ifdef AUI_WARNING_FIXES
		CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
		if (pEntry && pEntry->IsReformationBelief())
		{
			if (!IsInSomeReligion(eBelief))
#else
		if(!IsInSomeReligion(eBelief))
		{
			CvBeliefEntry* pEntry = pkBeliefs->GetEntry(eBelief);
			if(pEntry && pEntry->IsReformationBelief())
#endif
			{
				availableBeliefs.push_back(eBelief);
			}
		}
	}

	return availableBeliefs;
}

#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
bool CvGameReligions::IsAnyAvailablePantheonBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && pLoopEntry->IsPantheonBelief())
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
bool CvGameReligions::IsAnyAvailableFounderBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && pLoopEntry->IsFounderBelief())
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
bool CvGameReligions::IsAnyAvailableFollowerBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && pLoopEntry->IsFollowerBelief())
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
bool CvGameReligions::IsAnyAvailableEnhancerBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && pLoopEntry->IsEnhancerBelief())
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
bool CvGameReligions::IsAnyAvailableBonusBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && (pLoopEntry->IsEnhancerBelief() || pLoopEntry->IsFollowerBelief() || pLoopEntry->IsFounderBelief() || pLoopEntry->IsPantheonBelief()))
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
bool CvGameReligions::IsAnyAvailableReformationBeliefs() const
{
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	const CvBeliefEntry* pLoopEntry = NULL;
	BeliefTypes eBelief = NO_BELIEF;
	for (int iI = 0; iI < pkBeliefs->GetNumBeliefs(); iI++)
	{
		eBelief = static_cast<BeliefTypes>(iI);
		pLoopEntry = pkBeliefs->GetEntry(eBelief);
		if (pLoopEntry && pLoopEntry->IsReformationBelief())
		{
			if (!IsInSomeReligion(eBelief))
			{
				return true;
			}
		}
	}
	return false;
}
#endif

/// How much pressure is exerted between these cities?
#ifdef AUI_CONSTIFY
int CvGameReligions::GetAdjacentCityReligiousPressure(ReligionTypes eReligion, const CvCity* pFromCity, const CvCity* pToCity, int& iNumTradeRoutesInfluencing, bool bPretendTradeConnection) const
#else
int CvGameReligions::GetAdjacentCityReligiousPressure (ReligionTypes eReligion, CvCity *pFromCity, CvCity *pToCity, int& iNumTradeRoutesInfluencing, bool bPretendTradeConnection)
#endif
{
	int iPressure = 0;
	iNumTradeRoutesInfluencing = 0;

	if (eReligion <= RELIGION_PANTHEON)
	{
		return iPressure;
	}

	const CvReligion* pReligion = GetReligion(eReligion, pFromCity->getOwner());
	if (!pReligion)
	{
		return iPressure;
	}

	// Are the cities within the minimum distance?
	int iDistance = GC.getRELIGION_ADJACENT_CITY_DISTANCE();

	// Boost to distance due to belief?
	int iDistanceMod = pReligion->m_Beliefs.GetSpreadDistanceModifier();
	if(iDistanceMod > 0)
	{
		iDistance *= (100 + iDistanceMod);
		iDistance /= 100;
	}

	bool bWithinDistance = (plotDistance(pFromCity->getX(), pFromCity->getY(), pToCity->getX(), pToCity->getY()) <= iDistance);
	bool bConnectedWithTrade = GC.getGame().GetGameTrade()->IsCityConnectedToCity(pFromCity, pToCity) || bPretendTradeConnection;

	if(bWithinDistance || bConnectedWithTrade)
	{
		bool bIncrementTRInfluencing = false;
		iPressure = GC.getGame().getGameSpeedInfo().getReligiousPressureAdjacentCity();
		if (bConnectedWithTrade && !bWithinDistance)
		{
			if (!bIncrementTRInfluencing)
			{
				iNumTradeRoutesInfluencing++;
				bIncrementTRInfluencing = true;
			}
		}

		if (bConnectedWithTrade)
		{
			if (GC.getGame().GetGameTrade()->IsCityConnectedFromCityToCity(pFromCity, pToCity))
			{
				int iTradeReligionModifer = GET_PLAYER(pFromCity->getOwner()).GetPlayerTraits()->GetTradeReligionModifier();
				if (iTradeReligionModifer != 0)
				{
					iPressure *= 100 + iTradeReligionModifer;
					iPressure /= 100;

					if (!bIncrementTRInfluencing)
					{
						iNumTradeRoutesInfluencing++;
						bIncrementTRInfluencing = true;
					}
				}
			}
		}

		// If we are spreading to a friendly city state, increase the effectiveness if we have the right belief
		if(IsCityStateFriendOfReligionFounder(eReligion, pToCity->getOwner()))
		{
			int iFriendshipMod = pReligion->m_Beliefs.GetFriendlyCityStateSpreadModifier();
			if(iFriendshipMod > 0)
			{
				iPressure *= (100 + iFriendshipMod);
				iPressure /= 100;
			}
		}

		// Have a belief that always strengthens spread?
		int iStrengthMod = pReligion->m_Beliefs.GetSpreadStrengthModifier();
		if(iStrengthMod > 0)
		{
			TechTypes eDoublingTech = pReligion->m_Beliefs.GetSpreadModifierDoublingTech();
			if(eDoublingTech != NO_TECH)
			{
				CvPlayer& kPlayer = GET_PLAYER(pReligion->m_eFounder);
				if(GET_TEAM(kPlayer.getTeam()).GetTeamTechs()->HasTech(eDoublingTech))
				{
					iStrengthMod *= 2;
				}
			}
			iPressure *= (100 + iStrengthMod);
			iPressure /= 100;
		}

#ifdef NQ_SPREAD_MODIFIER_OWNED_CITIES
		// modifier if this religion spreads to cities owned by founder or not owned by founder
		int iTotalSelfReligiousPressure = 0;
		int iTotalForeignReligiousPressure = 0;
		CvPlayer& kPlayer = GET_PLAYER(pReligion->m_eFounder);
		if (pToCity->getOwner() == pReligion->m_eFounder)
		{	
			int iOwnedCityModifier = pReligion->m_Beliefs.GetSpreadModifierOwnedCities();
			iTotalSelfReligiousPressure += iOwnedCityModifier;
			int iOwnedCityModifierPolicy = kPlayer.GetPlayerPolicies()->GetNumericModifier(POLICYMOD_SPREAD_MODIFIER_OWNED_CITIES);
			iTotalSelfReligiousPressure += iOwnedCityModifierPolicy;
#if defined(LEKMOD_v34) // New Effect, Self Religious Pressure.
			int iOwnedCityModifierTrait = kPlayer.GetPlayerTraits()->GetSelfReligiousPressureModifier();
			iTotalSelfReligiousPressure += iOwnedCityModifierTrait;
#endif
		}
		else
		{
			int iUnownedCityModifier = pReligion->m_Beliefs.GetSpreadModifierUnownedCities();
			iTotalForeignReligiousPressure += iUnownedCityModifier;
			int iUnownedCityModifierPolicy = kPlayer.GetPlayerPolicies()->GetNumericModifier(POLICYMOD_SPREAD_MODIFIER_OTHER_CITIES);
			iTotalForeignReligiousPressure += iUnownedCityModifierPolicy;
#if defined(TRAITIFY) // Foreign Religious Pressure
			int iUnownedCityModifierTrait = kPlayer.GetPlayerTraits()->GetForeignRelgionPressureModifier();
			iTotalForeignReligiousPressure += iUnownedCityModifierTrait;
#endif
		}
		if (iTotalSelfReligiousPressure != 0)
		{
			iPressure *= (100 + iTotalSelfReligiousPressure);
			iPressure /= 100;
		}
		if (iTotalForeignReligiousPressure != 0)
		{
			iPressure *= (100 + iTotalForeignReligiousPressure);
			iPressure /= 100;
		}
#endif

		// Strengthened spread from World Congress? (World Religion)
		int iLeaguesMod = GC.getGame().GetGameLeagues()->GetReligionSpreadStrengthModifier(pFromCity->getOwner(), eReligion);
		if (iLeaguesMod > 0)
		{
			iPressure *= (100 + iLeaguesMod);
			iPressure /= 100;
		}

		// Building that boosts pressure from originating city?
		int iModifier = pFromCity->GetCityReligions()->GetReligiousPressureModifier();
		if (iModifier > 0)
		{
			iPressure *= (100 + iModifier);
			iPressure /= 100;
		}
	}

	return iPressure;
}

/// How much does this prophet cost (recursive)
int CvGameReligions::GetFaithGreatProphetNumber(int iNum) const
{
	int iRtnValue = 0;

	if(iNum >= 1)
	{
		if(iNum == 1)
		{
			iRtnValue = GC.getRELIGION_MIN_FAITH_FIRST_PROPHET();
		}
		else
		{
			iRtnValue = (GC.getRELIGION_FAITH_DELTA_NEXT_PROPHET() * (iNum - 1)) + GetFaithGreatProphetNumber(iNum - 1);
		}
	}

	return iRtnValue;
}

/// How much does this great person cost (recursive)
int CvGameReligions::GetFaithGreatPersonNumber(int iNum) const
{
	int iRtnValue = 0;

	if(iNum >= 1)
	{
		if(iNum == 1)
		{
			iRtnValue = GC.getRELIGION_MIN_FAITH_FIRST_GREAT_PERSON();
		}
		else
		{
			iRtnValue = (GC.getRELIGION_FAITH_DELTA_NEXT_GREAT_PERSON() * (iNum - 1)) + GetFaithGreatPersonNumber(iNum - 1);
		}
	}

	return iRtnValue;
}

/// Does the religion in nearby city give this battle winner a yield? If so return multipler of losing unit strength
#ifdef AUI_CONSTIFY
int CvGameReligions::GetBeliefYieldForKill(YieldTypes eYield, int iX, int iY, PlayerTypes eWinningPlayer) const
#else
int CvGameReligions::GetBeliefYieldForKill(YieldTypes eYield, int iX, int iY, PlayerTypes eWinningPlayer)
#endif
{
	int iRtnValue = 0;
	int iMultiplier;
	int iLoop;
	CvCity* pLoopCity;

	// Only Faith supported for now
	if(eYield != YIELD_FAITH)
	{
		return iRtnValue;
	}

	for(pLoopCity = GET_PLAYER(eWinningPlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(eWinningPlayer).nextCity(&iLoop))
	{
		// Find religion in this city
		ReligionTypes eReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();

		if(eReligion != NO_RELIGION)
		{
			// Find distance to this city
			int iDistance = plotDistance(iX, iY, pLoopCity->getX(), pLoopCity->getY());

			// Do we have a yield from this?
			iMultiplier = GetReligion(eReligion, eWinningPlayer)->m_Beliefs.GetFaithFromKills(iDistance);
			if(iMultiplier > 0)
			{
				// Just looking for one city providing this
				iRtnValue = iMultiplier;
				break;
			}
			else
			{
				BeliefTypes eSecondaryPantheon = pLoopCity->GetCityReligions()->GetSecondaryReligionPantheonBelief();
				if (eSecondaryPantheon != NO_BELIEF)
				{
					iMultiplier = GC.GetGameBeliefs()->GetEntry(eSecondaryPantheon)->GetFaithFromKills();
					if(iMultiplier > 0 && iDistance <= GC.GetGameBeliefs()->GetEntry(eSecondaryPantheon)->GetMaxDistance())
					{
						// Just looking for one city providing this
						iRtnValue = iMultiplier;
						break;
					}	
				}
			}
		}
	}

	return iRtnValue;
}

/// Build log filename
CvString CvGameReligions::GetLogFileName() const
{
	CvString strLogName;
	strLogName = "ReligionLog.csv";
	return strLogName;
}

// PRIVATE METHODS

/// Has this religion already been founded?
#ifdef AUI_CONSTIFY
bool CvGameReligions::HasBeenFounded(ReligionTypes eReligion) const
#else
bool CvGameReligions::HasBeenFounded(ReligionTypes eReligion)
#endif
{
	ReligionList::const_iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); ++it)
#else
	for(it = m_CurrentReligions.begin(); it != m_CurrentReligions.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			return true;
		}
	}

	return false;
}

/// Does any civ in the game like this religion?
#ifdef AUI_CONSTIFY
bool CvGameReligions::IsPreferredByCivInGame(ReligionTypes eReligion) const
#else
bool CvGameReligions::IsPreferredByCivInGame(ReligionTypes eReligion)
#endif
{
	PlayerTypes eLoopPlayer;

	for(int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
		eLoopPlayer = (PlayerTypes) iI;
		CvPlayer& loopPlayer = GET_PLAYER(eLoopPlayer);

		if(!loopPlayer.isAlive())
		{
			continue;
		}

		ReligionTypes eCivReligion = loopPlayer.getCivilizationInfo().GetReligion();
		if(eCivReligion == eReligion)
		{
			return true;
		}
	}

	return false;
}

/// Time to spawn a Great Prophet?
bool CvGameReligions::CheckSpawnGreatProphet(CvPlayer& kPlayer)
{
#ifdef LEK_UNIQUE_FAITH_UNIT_FIX
	UnitClassTypes eUnitClass = (UnitClassTypes) GC.getInfoTypeForString("UNITCLASS_PROPHET", true);
	UnitTypes eUnit = (UnitTypes) kPlayer.getCivilizationInfo().getCivilizationUnits(eUnitClass);
#else
	UnitTypes eUnit = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
#endif

#ifdef AI_CANNOT_FOUND_OR_ENHANCE_OR_SPREAD_RELIGION
	if (GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_RELIGION_FOUNDING"))
	{
		if (!kPlayer.isHuman())
		{
			return false;
		}
	}
#endif

#ifdef LEK_UNIQUE_FAITH_UNIT_FIX
	
	if (eUnitClass == NO_UNITCLASS)
    {
        return false;
    }
#endif
	if (eUnit == NO_UNIT)
	{
		return false;
	}

	const CvReligion* pReligion = NULL;
	const int iFaith = kPlayer.GetFaith();
	int iCost = kPlayer.GetReligions()->GetCostNextProphet(true /*bIncludeBeliefDiscounts*/, true /*bAdjustForSpeedDifficulty*/);

	ReligionTypes ePlayerReligion = GetReligionCreatedByPlayer(kPlayer.GetID());
	if(ePlayerReligion > RELIGION_PANTHEON)
	{
		pReligion = GetReligion(ePlayerReligion, kPlayer.GetID());
	}

	// If player hasn't founded a religion yet, drop out of this if all religions have been founded
	else if(GetNumReligionsStillToFound() <= 0)
	{
		return false;
	}

	if(iFaith < iCost)
	{
		return false;
	}

	int iChance = GC.getRELIGION_BASE_CHANCE_PROPHET_SPAWN();
	iChance += (iFaith - iCost);

	int iRand = GC.getGame().getJonRandNum(100, "Religion: spawn Great Prophet roll.");
	if(iRand >= iChance)
	{
		return false;
	}

	CvCity* pSpawnCity = NULL;
	if(pReligion)
	{
		pSpawnCity = GC.getMap().plot(pReligion->m_iHolyCityX, pReligion->m_iHolyCityY)->getPlotCity();
	}

	if(pSpawnCity != NULL && pSpawnCity->getOwner() == kPlayer.GetID())
	{
#ifdef LEK_UNIQUE_FAITH_UNIT_FIX
		pSpawnCity->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, true);
#else
		pSpawnCity->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, true);
#endif
#ifdef NQ_SPAWN_PROPHETS_REMOVE_ONLY_REQUIRED_FAITH
		kPlayer.ChangeFaith(-iCost);
#else
		kPlayer.SetFaith(0);
#endif
	}
	else
	{
		pSpawnCity = kPlayer.getCapitalCity();
		if(pSpawnCity != NULL)
		{
#ifdef LEK_UNIQUE_FAITH_UNIT_FIX
		pSpawnCity->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, true);
#else
		pSpawnCity->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, true);
#endif
#ifdef NQ_SPAWN_PROPHETS_REMOVE_ONLY_REQUIRED_FAITH
			kPlayer.ChangeFaith(-iCost);
#else
			kPlayer.SetFaith(0);
#endif
		}
	}

	// Logging
	if(GC.getLogging() && pSpawnCity)
	{
		CvString strLogMsg;
		strLogMsg = kPlayer.getCivilizationShortDescription();
		strLogMsg += ", PROPHET SPAWNED, ";
		strLogMsg += pSpawnCity->getName();
		strLogMsg += ", Faith: 0";
		LogReligionMessage(strLogMsg);
	}

	return true;
}

/// Log a message with status information
void CvGameReligions::LogReligionMessage(CvString& strMsg)
{
	if(GC.getLogging() && GC.getAILogging())
	{
		CvString strOutBuf;
		CvString strBaseString;
		FILogFile* pLog;

		pLog = LOGFILEMGR.GetLog(GetLogFileName(), FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		strBaseString.Format("%03d, %d, ", GC.getGame().getElapsedGameTurns(), GC.getGame().getGameTurnYear());
		strOutBuf = strBaseString + strMsg;
		pLog->Msg(strOutBuf);
	}
}

// Notify the supplied player (if they are the local player) of an error when founding/modifying a religion/pantheon
void CvGameReligions::NotifyPlayer(PlayerTypes ePlayer, CvGameReligions::FOUNDING_RESULT eResult)
{
	CvString strMessage;
	CvString strSummary;

	NotificationTypes eNotificationType = NOTIFICATION_RELIGION_ERROR;

	switch(eResult)
	{
	case FOUNDING_OK:
		break;
	case FOUNDING_BELIEF_IN_USE:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_PANTHEON_BELIEF_IN_USE");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_PANTHEON_BELIEF_IN_USE");
		break;
	case FOUNDING_RELIGION_IN_USE:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_RELIGION_IN_USE");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_RELIGION_IN_USE");
		break;
	case FOUNDING_NOT_ENOUGH_FAITH:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_NOT_ENOUGH_FAITH_FOR_PANTHEON");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_NOT_ENOUGH_FAITH_FOR_PANTHEON");
		break;
	case FOUNDING_NO_RELIGIONS_AVAILABLE:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_NO_RELIGIONS_AVAILABLE");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_NO_RELIGIONS_AVAILABLE");
		break;
	case FOUNDING_INVALID_PLAYER:
		break;
	case FOUNDING_PLAYER_ALREADY_CREATED_RELIGION:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_ALREADY_CREATED_RELIGION");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ALREADY_CREATED_RELIGION");
		break;
	case FOUNDING_PLAYER_ALREADY_CREATED_PANTHEON:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_ALREADY_CREATED_PANTHEON");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ALREADY_CREATED_PANTHEON");
		break;
	case FOUNDING_NAME_IN_USE:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_RELIGION_NAME_IN_USE");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_RELIGION_NAME_IN_USE");
		break;
	case FOUNDING_RELIGION_ENHANCED:
		strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_ENHANCED_RELIGION_IN_USE");
		strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ENHANCED_RELIGION_IN_USE");
		break;
	}

	if(strMessage.GetLength() != 0)
		CvNotifications::AddToPlayer(ePlayer, eNotificationType, strMessage.c_str(), strSummary.c_str());
}

// SERIALIZATION

/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvGameReligions& writeTo)
{
	uint uiVersion;
	int iMinFaith;

	loadFrom >> uiVersion;

	if(uiVersion >= 3)
	{
		loadFrom >> iMinFaith;
		writeTo.SetMinimumFaithNextPantheon(iMinFaith);
	}
	else
	{
		writeTo.SetMinimumFaithNextPantheon(0);
	}

	if(uiVersion < 4)
	{
		loadFrom >> iMinFaith;
		//	writeTo.SetMinimumFaithNextGreatProphet(iMinFaith);  -- eliminated in Version 4
	}

	if(uiVersion >= 2)
	{
		int iEntriesToRead;
		CvReligion tempItem;

		writeTo.m_CurrentReligions.clear();
		loadFrom >> iEntriesToRead;
		for(int iI = 0; iI < iEntriesToRead; iI++)
		{
			loadFrom >> tempItem;
			writeTo.m_CurrentReligions.push_back(tempItem);
		}
	}

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvGameReligions& readFrom)
{
	uint uiVersion = 4;
	saveTo << uiVersion;

	saveTo << readFrom.GetMinimumFaithNextPantheon();

	ReligionList::const_iterator it;
	saveTo << readFrom.m_CurrentReligions.size();
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = readFrom.m_CurrentReligions.begin(); it != readFrom.m_CurrentReligions.end(); ++it)
#else
	for(it = readFrom.m_CurrentReligions.begin(); it != readFrom.m_CurrentReligions.end(); it++)
#endif
	{
		saveTo << *it;
	}

	return saveTo;
}

//=====================================
// CvPlayerReligions
//=====================================
/// Constructor
CvPlayerReligions::CvPlayerReligions(void):
	m_pPlayer(NULL),
	m_iNumProphetsSpawned(0),
	m_bFoundingReligion(false)
{
}

/// Destructor
CvPlayerReligions::~CvPlayerReligions(void)
{
	Uninit();
}

/// Initialize class data
void CvPlayerReligions::Init(CvPlayer* pPlayer)
{
	m_pPlayer = pPlayer;

	Reset();
}

/// Cleanup
void CvPlayerReligions::Uninit()
{

}

/// Reset
void CvPlayerReligions::Reset()
{
	m_bFoundingReligion = false;
	m_iNumProphetsSpawned = 0;
}

/// Serialization read
void CvPlayerReligions::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;
	kStream >> m_iNumProphetsSpawned;
	kStream >> m_bFoundingReligion;
}

/// Serialization write
void CvPlayerReligions::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;
	kStream << m_iNumProphetsSpawned;
	kStream << m_bFoundingReligion;
}

/// How many prophets have we spawned
int CvPlayerReligions::GetNumProphetsSpawned() const
{
	return m_iNumProphetsSpawned;
}

/// Change count of prophets spawned
void CvPlayerReligions::ChangeNumProphetsSpawned(int iValue)
{
	m_iNumProphetsSpawned += iValue;
}

/// How much will the next prophet cost this player?
int CvPlayerReligions::GetCostNextProphet(bool bIncludeBeliefDiscounts, bool bAdjustForSpeedDifficulty) const
{
	int iCost = GC.getGame().GetGameReligions()->GetFaithGreatProphetNumber(m_iNumProphetsSpawned + 1);

	// Boost to faith due to belief?
	ReligionTypes ePlayerReligion = GetReligionCreatedByPlayer();
	if (bIncludeBeliefDiscounts && ePlayerReligion > RELIGION_PANTHEON)
	{
		const CvReligion* pReligion = NULL;
		pReligion = GC.getGame().GetGameReligions()->GetReligion(ePlayerReligion, m_pPlayer->GetID());
		if(pReligion)
		{
			int iProphetCostMod = pReligion->m_Beliefs.GetProphetCostModifier();

			if(iProphetCostMod != 0)
			{
				iCost *= (100 + iProphetCostMod);
				iCost /= 100;
			}
		}
	}

	if (bAdjustForSpeedDifficulty)
	{
		// Adjust for game speed
		iCost *= GC.getGame().getGameSpeedInfo().getTrainPercent();
		iCost /= 100;

		// Adjust for difficulty
		if(!m_pPlayer->isHuman() && !m_pPlayer->IsAITeammateOfHuman() && !m_pPlayer->isBarbarian())
		{
			iCost *= GC.getGame().getHandicapInfo().getAITrainPercent();
			iCost /= 100;
		}
	}

#ifdef NQ_SPAWN_PROPHETS_REMOVE_ONLY_REQUIRED_FAITH
	// Make the number not be funky
	int iDivisor = /*10*/ GC.getGOLD_PURCHASE_VISIBLE_DIVISOR();
	iCost /= iDivisor;
	iCost *= iDivisor;
#endif

	return iCost;
}

/// Has this player created a pantheon?
bool CvPlayerReligions::HasCreatedPantheon() const
{
	return GC.getGame().GetGameReligions()->HasCreatedPantheon(m_pPlayer->GetID());
}

/// Has this player created a religion?
bool CvPlayerReligions::HasCreatedReligion() const
{
	return GC.getGame().GetGameReligions()->HasCreatedReligion(m_pPlayer->GetID());
}

/// Has this player reformed his religion
bool CvPlayerReligions::HasAddedReformationBelief() const
{
	return GC.getGame().GetGameReligions()->HasAddedReformationBelief(m_pPlayer->GetID());
}

/// Get the religion this player created
ReligionTypes CvPlayerReligions::GetReligionCreatedByPlayer() const
{
	return GC.getGame().GetGameReligions()->GetReligionCreatedByPlayer(m_pPlayer->GetID());
}

/// Does this player have enough faith to buy a religious unit or building?
bool CvPlayerReligions::CanAffordFaithPurchase() const
{
	int iFaith = m_pPlayer->GetFaith();
	CvCity* pCapital = m_pPlayer->getCapitalCity();
	if(pCapital)
	{
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumUnitInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumUnitInfos(); iI++)
#endif
		{
			const UnitTypes eUnit = static_cast<UnitTypes>(iI);
			CvUnitEntry* pkUnitInfo = GC.getUnitInfo(eUnit);
			if(pkUnitInfo)
			{
				if (m_pPlayer->IsCanPurchaseAnyCity(false, false, eUnit, NO_BUILDING, YIELD_FAITH))
				{
					int iCost = pCapital->GetFaithPurchaseCost(eUnit, true);
					if(iCost != 0 && iFaith > iCost)
					{
						return true;
					}
				}
			}
		}
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingInfos(); iI++)
#endif
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iI);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				if (m_pPlayer->IsCanPurchaseAnyCity(false, false, NO_UNIT, eBuilding, YIELD_FAITH))
				{
					int iCost = pCapital->GetFaithPurchaseCost(eBuilding);
					if(iCost != 0 && iFaith > iCost)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

/// Does this player have a city following a religion?
bool CvPlayerReligions::HasReligiousCity() const
{
	int iLoop = 0;
	CvCity* pCity = NULL;

	for(pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pCity)
		{
			if(pCity->GetCityReligions()->GetReligiousMajority() > RELIGION_PANTHEON)
			{
				return true;
			}
		}
	}

	return false;
}

/// Is this player happily following this other player's religion?
bool CvPlayerReligions::HasOthersReligionInMostCities(PlayerTypes eOtherPlayer) const
{
	// Not happy about it if have their own religion
	if(HasCreatedReligion())
	{
		return false;
	}

	ReligionTypes eOtherReligion = GC.getGame().GetGameReligions()->GetReligionCreatedByPlayer(eOtherPlayer);

	if(eOtherReligion <= RELIGION_PANTHEON)
	{
		return false;
	}

	int iNumFollowingCities = 0;
	int iLoop = 0;
	CvCity* pCity = NULL;
	for(pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pCity)
		{
			if(pCity->GetCityReligions()->GetReligiousMajority() == eOtherReligion)
			{
				iNumFollowingCities++;
			}
		}
	}

	// Over half?
	return (iNumFollowingCities * 2 > m_pPlayer->getNumCities());
}

/// Do a majority of this player's cities follow a specific religion?
bool CvPlayerReligions::HasReligionInMostCities(ReligionTypes eReligion) const
{
	if (eReligion <= RELIGION_PANTHEON)
	{
		return false;
	}

	int iNumFollowingCities = 0;
	int iLoop = 0;
	CvCity* pCity = NULL;
	for(pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pCity)
		{
			if(pCity->GetCityReligions()->GetReligiousMajority() == eReligion)
			{
				iNumFollowingCities++;
			}
		}
	}

	// Over half?
	return (iNumFollowingCities * 2 > m_pPlayer->getNumCities());
}

/// What religion is followed in a majority of our cities?
ReligionTypes CvPlayerReligions::GetReligionInMostCities() const
{
	for (int iI = RELIGION_PANTHEON + 1; iI < GC.GetGameReligions()->GetNumReligions(); iI++)
	{
		ReligionTypes eReligion = (ReligionTypes)iI;
		if (HasReligionInMostCities(eReligion))
		{
			return eReligion;
		}
	}
	return NO_RELIGION;
}

/// Does this player get a default influence boost with city states following this religion?
int CvPlayerReligions::GetCityStateMinimumInfluence(ReligionTypes eReligion) const
{
	int iMinInfluence = 0;

	ReligionTypes eFounderBenefitReligion = GC.getGame().GetGameReligions()->GetFounderBenefitsReligion(m_pPlayer->GetID());
	if (eReligion == eFounderBenefitReligion)
	{
		CvGameReligions* pReligions = GC.getGame().GetGameReligions();
		if(eFounderBenefitReligion != NO_RELIGION)
		{
			const CvReligion* pReligion = pReligions->GetReligion(eFounderBenefitReligion, NO_PLAYER);
			if(pReligion)
			{
				iMinInfluence += pReligion->m_Beliefs.GetCityStateMinimumInfluence();
			}
		}
	}

	return iMinInfluence;
}

/// Does this player get a modifier to city state influence boosts?
int CvPlayerReligions::GetCityStateInfluenceModifier() const
{
	int iRtnValue = 0;
	ReligionTypes eReligion = GetReligionCreatedByPlayer();
	if (eReligion != NO_RELIGION)
	{
		const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligion, NO_PLAYER);
		if(pReligion)
		{
			iRtnValue += pReligion->m_Beliefs.GetCityStateInfluenceModifier();
		}
	}
	return iRtnValue;
}

/// Does this player get religious pressure from spies?
int CvPlayerReligions::GetSpyPressure() const
{
	int iRtnValue = 0;
	ReligionTypes eReligion = GetReligionCreatedByPlayer();
	if (eReligion != NO_RELIGION)
	{
		const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligion, NO_PLAYER);
		if(pReligion)
		{
			iRtnValue += pReligion->m_Beliefs.GetSpyPressure();
		}
	}
	return iRtnValue;
}

/// How many foreign cities are following a religion we founded?
int CvPlayerReligions::GetNumForeignCitiesFollowing() const
{
	CvCity *pLoopCity;
	int iCityLoop;
	int iRtnValue = 0;

	ReligionTypes eFounderBenefitReligion = GC.getGame().GetGameReligions()->GetFounderBenefitsReligion(m_pPlayer->GetID());
	if (eFounderBenefitReligion > RELIGION_PANTHEON)
	{
		for(int iPlayerLoop = 0; iPlayerLoop < MAX_CIV_PLAYERS; iPlayerLoop++)
		{
			CvPlayer &kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayerLoop);
			if(kLoopPlayer.isAlive() && iPlayerLoop != m_pPlayer->GetID())
			{
				for(pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).nextCity(&iCityLoop))
				{
					if (pLoopCity->GetCityReligions()->GetReligiousMajority() == eFounderBenefitReligion)
					{
						iRtnValue++;
					}
				}
			}
		}
	}

	return iRtnValue;
}

/// How many foreign citizens are following a religion we founded?
int CvPlayerReligions::GetNumForeignFollowers(bool bAtPeace) const
{
	CvCity *pLoopCity;
	int iCityLoop;
	int iRtnValue = 0;

	ReligionTypes eFounderBenefitReligion = GC.getGame().GetGameReligions()->GetFounderBenefitsReligion(m_pPlayer->GetID());
	if (eFounderBenefitReligion > RELIGION_PANTHEON)
	{
		for(int iPlayerLoop = 0; iPlayerLoop < MAX_CIV_PLAYERS; iPlayerLoop++)
		{
			CvPlayer &kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayerLoop);
			if(kLoopPlayer.isAlive() && iPlayerLoop != m_pPlayer->GetID())
			{
				if (!bAtPeace || !atWar(m_pPlayer->getTeam(), kLoopPlayer.getTeam()))
				{
					for(pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).nextCity(&iCityLoop))
					{
						iRtnValue += pLoopCity->GetCityReligions()->GetNumFollowers(eFounderBenefitReligion);
					}
				}
			}
		}
	}

	return iRtnValue;
}

//=====================================
// CvCityReligions
//=====================================
/// Constructor
#ifdef AUI_CITY_FIX_COMPONENT_CONSTRUCTORS_CONTAIN_POINTERS
CvCityReligions::CvCityReligions(CvCity* pCity) :
	m_pCity(pCity),
#else
CvCityReligions::CvCityReligions(void):
#endif
	m_bHasPaidAdoptionBonus(false),
	m_iReligiousPressureModifier(0)
{
	m_ReligionStatus.clear();
}

/// Destructor
CvCityReligions::~CvCityReligions(void)
{

}

/// Initialize class data
void CvCityReligions::Init(CvCity* pCity)
{
	m_pCity = pCity;
	m_bHasPaidAdoptionBonus = false;
	m_iReligiousPressureModifier = 0;
	m_ReligionStatus.clear();
}

/// Cleanup
void CvCityReligions::Uninit()
{

}

/// Copy data from old city into new (for conquest)
void CvCityReligions::Copy(CvCityReligions* pOldCity)
{
	m_ReligionStatus.clear();

	SetPaidAdoptionBonus(pOldCity->HasPaidAdoptionBonus());
	SetReligiousPressureModifier(pOldCity->GetReligiousPressureModifier());

	ReligionInCityList::iterator religionIt;
	for(religionIt = pOldCity->m_ReligionStatus.begin(); religionIt != pOldCity->m_ReligionStatus.end(); ++religionIt)
	{
		m_ReligionStatus.push_back(*religionIt);
	}
}

/// How many citizens here are following this religion?
#ifdef AUI_CONSTIFY
int CvCityReligions::GetNumFollowers(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator religionIt;
#else
int CvCityReligions::GetNumFollowers(ReligionTypes eReligion)
{
	ReligionInCityList::iterator religionIt;
#endif

	// Find the religion in the list
	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if(religionIt->m_eReligion == eReligion)
		{
			return religionIt->m_iFollowers;
		}
	}

	return 0;
}

/// Number of followers of this religion
#ifdef AUI_CONSTIFY
int CvCityReligions::GetNumSimulatedFollowers(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator religionIt;
#else
int CvCityReligions::GetNumSimulatedFollowers(ReligionTypes eReligion)
{
	ReligionInCityList::iterator religionIt;
#endif

	// Find the religion in the list
	for(religionIt = m_SimulatedStatus.begin(); religionIt != m_SimulatedStatus.end(); ++religionIt)
	{
		if(religionIt->m_eReligion == eReligion)
		{
			return religionIt->m_iFollowers;
		}
	}

	return 0;
}

/// How many religions have at least 1 follower?
#ifdef AUI_CONSTIFY
int CvCityReligions::GetNumReligionsWithFollowers() const
#else
int CvCityReligions::GetNumReligionsWithFollowers()
#endif
{
	int iRtnValue = 0;
#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator religionIt;
#else
	ReligionInCityList::iterator religionIt;
#endif

	// Find the religion in the list
	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if(religionIt->m_iFollowers > 0 && religionIt->m_eReligion > RELIGION_PANTHEON)
		{
			iRtnValue++;
		}
	}

	return iRtnValue;
}

///Any religion in this city?
#ifdef AUI_CONSTIFY
bool CvCityReligions::IsReligionInCity() const
{
	ReligionInCityList::const_iterator religionIt;
#else
bool CvCityReligions::IsReligionInCity()
{
	ReligionInCityList::iterator religionIt;
#endif

	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if(religionIt->m_eReligion != NO_RELIGION)
		{
			return true;
		}
	}

	return false;
}

/// Is this the holy city for a specific religion?
#ifdef AUI_CONSTIFY
bool CvCityReligions::IsHolyCityForReligion(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator religionIt;
#else
bool CvCityReligions::IsHolyCityForReligion(ReligionTypes eReligion)
{
	ReligionInCityList::iterator religionIt;
#endif

	// Find the religion in the list
	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if(religionIt->m_eReligion == eReligion)
		{
			return religionIt->m_bFoundedHere;
		}
	}

	return false;
}

/// Is this the holy city for any religion?
#ifdef AUI_CONSTIFY
bool CvCityReligions::IsHolyCityAnyReligion() const
{
	ReligionInCityList::const_iterator religionIt;
#else
bool CvCityReligions::IsHolyCityAnyReligion()
{
	ReligionInCityList::iterator religionIt;
#endif
	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if(religionIt->m_bFoundedHere)
		{
			return true;
		}
	}

	return false;
}

/// Is there a "heretical" religion here that can be stomped out?
#ifdef AUI_CONSTIFY
bool CvCityReligions::IsReligionHereOtherThan(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator it;
#else
bool CvCityReligions::IsReligionHereOtherThan(ReligionTypes eReligion)
{
	ReligionInCityList::iterator it;
#endif
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion != NO_RELIGION && it->m_eReligion != eReligion)
		{
			return true;
		}
	}
	return false;
}

/// Is there an inquisitor from another religion here?
#ifdef AUI_CONSTIFY
bool CvCityReligions::IsDefendedAgainstSpread(ReligionTypes eReligion) const
#else
bool CvCityReligions::IsDefendedAgainstSpread(ReligionTypes eReligion)
#endif
{
	CvUnit* pLoopUnit;

	CvPlot* pCityPlot = m_pCity->plot();
	if(pCityPlot)
	{
#ifdef AUI_WARNING_FIXES
		for (uint iUnitLoop = 0; iUnitLoop < pCityPlot->getNumUnits(); iUnitLoop++)
#else
		for(int iUnitLoop = 0; iUnitLoop < pCityPlot->getNumUnits(); iUnitLoop++)
#endif
		{
			pLoopUnit = pCityPlot->getUnitByIndex(iUnitLoop);
			CvUnitEntry* pkEntry = GC.getUnitInfo(pLoopUnit->getUnitType());
			if(pkEntry && pkEntry->IsProhibitsSpread())
			{
				if(pLoopUnit->getOwner() == m_pCity->getOwner() && pLoopUnit->GetReligionData()->GetReligion() != eReligion)
				{
					return true;
				}
			}
		}
	}

	CvPlot* pAdjacentPlot;
	for(int iDirectionLoop = 0; iDirectionLoop < NUM_DIRECTION_TYPES; iDirectionLoop++)
	{
		pAdjacentPlot = plotDirection(m_pCity->getX(), m_pCity->getY(), ((DirectionTypes)iDirectionLoop));

		if(pAdjacentPlot != NULL)
		{
#ifdef AUI_WARNING_FIXES
			for (uint iUnitLoop = 0; iUnitLoop < pAdjacentPlot->getNumUnits(); iUnitLoop++)
#else
			for(int iUnitLoop = 0; iUnitLoop < pAdjacentPlot->getNumUnits(); iUnitLoop++)
#endif
			{
				pLoopUnit = pAdjacentPlot->getUnitByIndex(iUnitLoop);
#ifdef AUI_CONSTIFY
				if (pLoopUnit->getUnitInfo().IsProhibitsSpread())
#else
				CvUnitEntry* pkEntry = GC.getUnitInfo(pLoopUnit->getUnitType());
				if(pkEntry && pkEntry->IsProhibitsSpread())
#endif
				{
					if(pLoopUnit->getOwner() == m_pCity->getOwner() && pLoopUnit->GetReligionData()->GetReligion() != eReligion)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

/// Is there a religion that at least half of the population follows?
#ifdef AUI_CONSTIFY
ReligionTypes CvCityReligions::GetReligiousMajority() const
#else
ReligionTypes CvCityReligions::GetReligiousMajority()
#endif
{
	int iTotalFollowers = 0;
	int iMostFollowerPressure = 0;
	int iMostFollowers = -1;
	ReligionTypes eMostFollowers = NO_RELIGION;
#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator religionIt;
#else
	ReligionInCityList::iterator religionIt;
#endif

	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		iTotalFollowers += religionIt->m_iFollowers;

		if(religionIt->m_iFollowers > iMostFollowers || religionIt->m_iFollowers == iMostFollowers && religionIt->m_iPressure > iMostFollowerPressure)
		{
			iMostFollowers = religionIt->m_iFollowers;
			iMostFollowerPressure = religionIt->m_iPressure;
			eMostFollowers = religionIt->m_eReligion;
		}
	}

	if ((iMostFollowers * 2) >= iTotalFollowers)
	{
		return eMostFollowers;
	}
	else
	{
		return NO_RELIGION;
	}
}

/// Just asked to simulate a conversion - who would be the majority religion?
#ifdef AUI_CONSTIFY
ReligionTypes CvCityReligions::GetSimulatedReligiousMajority() const
#else
ReligionTypes CvCityReligions::GetSimulatedReligiousMajority()
#endif
{
	int iTotalFollowers = 0;
	int iMostFollowerPressure = 0;
	int iMostFollowers = -1;
	ReligionTypes eMostFollowers = NO_RELIGION;
#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator religionIt;
#else
	ReligionInCityList::iterator religionIt;
#endif

	for(religionIt = m_SimulatedStatus.begin(); religionIt != m_SimulatedStatus.end(); ++religionIt)
	{
		iTotalFollowers += religionIt->m_iFollowers;

		if(religionIt->m_iFollowers > iMostFollowers || religionIt->m_iFollowers == iMostFollowers && religionIt->m_iPressure > iMostFollowerPressure)
		{
			iMostFollowers = religionIt->m_iFollowers;
			iMostFollowerPressure = religionIt->m_iPressure;
			eMostFollowers = religionIt->m_eReligion;
		}
	}

	if ((iMostFollowers * 2) >= iTotalFollowers)
	{
		return eMostFollowers;
	}
	else
	{
		return NO_RELIGION;
	}
}

/// What is the second most popular religion in this city with a majority religion?
#ifdef AUI_CONSTIFY
ReligionTypes CvCityReligions::GetSecondaryReligion() const
#else
ReligionTypes CvCityReligions::GetSecondaryReligion()
#endif
{
	int iMostFollowers = -1;
	int iMostPressure = -1;
	ReligionTypes eMajority = GetReligiousMajority();
	ReligionTypes eMostFollowers = NO_RELIGION;
#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator religionIt;
#else
	ReligionInCityList::iterator religionIt;
#endif

	if (eMajority != NO_RELIGION)
	{	
		for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
		{
			if (religionIt->m_eReligion != eMajority && religionIt->m_eReligion > RELIGION_PANTHEON && religionIt->m_iFollowers > 0)
			{
				if (religionIt->m_iFollowers > iMostFollowers)
				{
					iMostFollowers = religionIt->m_iFollowers;
					eMostFollowers = religionIt->m_eReligion;
					iMostPressure = religionIt->m_iPressure;
				}
				else if (religionIt->m_iFollowers == iMostFollowers && religionIt->m_iPressure > iMostPressure)
				{
					iMostFollowers = religionIt->m_iFollowers;
					eMostFollowers = religionIt->m_eReligion;
					iMostPressure = religionIt->m_iPressure;
				}
			}
		}
	}

	return eMostFollowers;
}

/// Is there a pantheon belief in the secondary religion here?
#ifdef AUI_CONSTIFY
BeliefTypes CvCityReligions::GetSecondaryReligionPantheonBelief() const
#else
BeliefTypes CvCityReligions::GetSecondaryReligionPantheonBelief()
#endif
{
	BeliefTypes eRtnValue = NO_BELIEF;

	// Check for the policy that allows a secondary religion to be active
	if (GET_PLAYER(m_pCity->getOwner()).IsSecondReligionPantheon())
	{
		ReligionTypes eSecondary = GetSecondaryReligion();
		if (eSecondary != NO_RELIGION)
		{
			const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eSecondary, m_pCity->getOwner());
			if(pReligion)
			{
				for(int iI = 0; iI < pReligion->m_Beliefs.GetNumBeliefs(); iI++)
				{
					const BeliefTypes eBelief = pReligion->m_Beliefs.GetBelief(iI);
					CvBeliefEntry* pEntry = GC.GetGameBeliefs()->GetEntry((int)eBelief);
					if(pEntry && pEntry->IsPantheonBelief())
					{
						eRtnValue = eBelief;
						break;
					}
				}
			}
		}
	}

	return eRtnValue;
}

/// How many followers are there of religions OTHER than this one?
#ifdef AUI_CONSTIFY
int CvCityReligions::GetFollowersOtherReligions(ReligionTypes eReligion) const
#else
int CvCityReligions::GetFollowersOtherReligions(ReligionTypes eReligion)
#endif
{
	int iOtherFollowers = 0;
#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator religionIt;
#else
	ReligionInCityList::iterator religionIt;
#endif

	for(religionIt = m_ReligionStatus.begin(); religionIt != m_ReligionStatus.end(); ++religionIt)
	{
		if (religionIt->m_eReligion > RELIGION_PANTHEON && religionIt->m_eReligion != eReligion)
		{
			iOtherFollowers += religionIt->m_iFollowers;
		}
	}

	return iOtherFollowers;
}

/// Total pressure exerted by all religions
#ifdef AUI_CONSTIFY
int CvCityReligions::GetTotalPressure() const
#else
int CvCityReligions::GetTotalPressure()
#endif
{
	int iTotalPressure = 0;

#ifdef AUI_CONSTIFY
	ReligionInCityList::const_iterator it;
#else
	ReligionInCityList::iterator it;
#endif
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		iTotalPressure += it->m_iPressure;
	}

	return iTotalPressure;
}

/// Pressure exerted by one religion
#ifdef AUI_CONSTIFY
int CvCityReligions::GetPressure(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator it;
#else
int CvCityReligions::GetPressure(ReligionTypes eReligion)
{
	ReligionInCityList::iterator it;
#endif
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			return it->m_iPressure;
		}
	}

	return 0;
}

/// Pressure exerted by one religion per turn
#ifdef AUI_CONSTIFY
int CvCityReligions::GetPressurePerTurn(ReligionTypes eReligion, int& iNumTradeRoutesInvolved) const
#else
int CvCityReligions::GetPressurePerTurn(ReligionTypes eReligion, int& iNumTradeRoutesInvolved)
#endif
{
	int iPressure = 0;
	iNumTradeRoutesInvolved = 0;

	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				// Ignore the same city
				if (m_pCity == pLoopCity)
				{
					continue;
				}

				// Does this city have the matching majority religion?
				ReligionTypes eMajorityReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();
				if(eMajorityReligion == eReligion)
				{
					int iNumTradeRoutes;
					iPressure += GC.getGame().GetGameReligions()->GetAdjacentCityReligiousPressure (eMajorityReligion, pLoopCity, m_pCity, iNumTradeRoutes, false);
					iNumTradeRoutesInvolved += iNumTradeRoutes;
				}
			}
		}
	}

	// Holy city for this religion?
	if (IsHolyCityForReligion(eReligion))
	{
		int iHolyCityPressure = GC.getGame().getGameSpeedInfo().getReligiousPressureAdjacentCity();
		iHolyCityPressure *=  GC.getRELIGION_PER_TURN_FOUNDING_CITY_PRESSURE();
		iPressure += iHolyCityPressure;
	}

	return iPressure;
}

/// How many trade routes are applying pressure to this city
#ifdef AUI_CONSTIFY
int CvCityReligions::GetNumTradeRouteConnections(ReligionTypes eReligion) const
{
	ReligionInCityList::const_iterator it;
#else
int CvCityReligions::GetNumTradeRouteConnections (ReligionTypes eReligion)
{
	ReligionInCityList::iterator it;
#endif
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			return it->m_iNumTradeRoutesApplyingPressure;
		}
	}

	return 0;
}

/// Would this city exert religious pressure toward the target city if connected with a trade route
#ifdef AUI_CONSTIFY
bool CvCityReligions::WouldExertTradeRoutePressureToward(const CvCity* pTargetCity, ReligionTypes& eReligion, int& iAmount) const
#else
bool CvCityReligions::WouldExertTradeRoutePressureToward (CvCity* pTargetCity, ReligionTypes& eReligion, int& iAmount)
#endif
{
	eReligion = NO_RELIGION;
	iAmount = 0;

	ReligionTypes eReligiousMajority = GetReligiousMajority();

	// if there isn't a religious connection, whatvz
	if (eReligiousMajority == NO_RELIGION)
	{
		return false;
	}

	const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligiousMajority, m_pCity->getOwner());
	if (!pReligion)
	{
		return false;
	}

	// Are the cities within the minimum distance?
	int iDistance = GC.getRELIGION_ADJACENT_CITY_DISTANCE();

	// Boost to distance due to belief?
	int iDistanceMod = pReligion->m_Beliefs.GetSpreadDistanceModifier();
	if(iDistanceMod > 0)
	{
		iDistance *= (100 + iDistanceMod);
		iDistance /= 100;
	}

	bool bWithinDistance = (plotDistance(m_pCity->getX(), m_pCity->getY(), pTargetCity->getX(), pTargetCity->getY()) <= iDistance);

	// if not within distance, then we're using a trade route
	if (!bWithinDistance) 
	{
		eReligion = eReligiousMajority;
		int iNumTradeRoutes = 0;
		iAmount = GC.getGame().GetGameReligions()->GetAdjacentCityReligiousPressure(eReligiousMajority, m_pCity, pTargetCity, iNumTradeRoutes, true);
		return true;
	}

	return false;
}


/// Handle a change in the city population
void CvCityReligions::DoPopulationChange(int iChange)
{
	ReligionTypes eMajorityReligion = GetReligiousMajority();

	// Only add pressure if the population went up; if starving, leave pressure alone (but recompute followers)
	if(iChange > 0)
	{
		AddReligiousPressure(FOLLOWER_CHANGE_POP_CHANGE, eMajorityReligion, iChange * GC.getRELIGION_ATHEISM_PRESSURE_PER_POP());
	}
	else if (iChange < 0)
	{
		RecomputeFollowers(FOLLOWER_CHANGE_POP_CHANGE, eMajorityReligion);
	}
}

/// Note that a religion was founded here
void CvCityReligions::DoReligionFounded(ReligionTypes eReligion)
{
	int iInitialPressure;
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	iInitialPressure = GC.getRELIGION_INITIAL_FOUNDING_CITY_PRESSURE() * m_pCity->getPopulation();
	CvReligionInCity newReligion(eReligion, true, 0, iInitialPressure);
	m_ReligionStatus.push_back(newReligion);

	RecomputeFollowers(FOLLOWER_CHANGE_RELIGION_FOUNDED, eOldMajorityReligion);
}

/// Prophet spread is very powerful: eliminates all existing religions and adds to his
void CvCityReligions::AddProphetSpread(ReligionTypes eReligion, int iPressure, PlayerTypes eResponsiblePlayer)
{
	int iAtheismPressure = 0;
	int iReligionPressure = 0;
	int iPressureRetained = 0;
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();
	ReligionTypes eHolyCityReligion = NO_RELIGION;
	ReligionTypes ePressureRetainedReligion = NO_RELIGION;
	bool bProphetsReligionFoundedHere = false;

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if (it->m_eReligion == NO_RELIGION)
		{
			iAtheismPressure = it->m_iPressure;
		}
		else if (eReligion == it->m_eReligion)
		{
			iReligionPressure = it->m_iPressure;
			if (it->m_bFoundedHere)
			{
				bProphetsReligionFoundedHere = true;
			}
		}
		else if (it->m_bFoundedHere)
		{
			eHolyCityReligion = it->m_eReligion;
		}

		if (it->m_eReligion > RELIGION_PANTHEON &&  it->m_eReligion != eReligion)
		{
			const CvReligion *pReligion = GC.getGame().GetGameReligions()->GetReligion(it->m_eReligion, NO_PLAYER);
			int iPressureRetention = pReligion->m_Beliefs.GetInquisitorPressureRetention();  // Normally 0
			if (iPressureRetention > 0)
			{
				ePressureRetainedReligion = it->m_eReligion;
				iPressureRetained = it->m_iPressure * iPressureRetention / 100;
			}
		}
	}

	// Clear list
	m_ReligionStatus.clear();

	// Add atheists and this back in
	CvReligionInCity atheism(NO_RELIGION, false/*bFoundedHere*/, 0, iAtheismPressure);
	m_ReligionStatus.push_back(atheism);
	CvReligionInCity prophetReligion(eReligion, bProphetsReligionFoundedHere, 0, iReligionPressure + iPressure);
	m_ReligionStatus.push_back(prophetReligion);

	// Reestablish Holy City religion
	if (eHolyCityReligion != NO_RELIGION && !bProphetsReligionFoundedHere)
	{
		if (eHolyCityReligion == ePressureRetainedReligion)
		{
			CvReligionInCity holyCityReligion(eHolyCityReligion, true/*bFoundedHere*/, 0, iPressureRetained);
			m_ReligionStatus.push_back(holyCityReligion);			
		}
		else
		{
			CvReligionInCity holyCityReligion(eHolyCityReligion, true/*bFoundedHere*/, 0, 0);
			m_ReligionStatus.push_back(holyCityReligion);
		}
	}

	// Reestablish pressure-retained religion (if wasn't Holy City religion)
	if (ePressureRetainedReligion != NO_RELIGION && eHolyCityReligion != ePressureRetainedReligion)
	{
		CvReligionInCity pressureRetainedReligion(ePressureRetainedReligion, false/*bFoundedHere*/, 0, iPressureRetained);
		m_ReligionStatus.push_back(pressureRetainedReligion);
	}

	RecomputeFollowers(FOLLOWER_CHANGE_PROPHET, eOldMajorityReligion, eResponsiblePlayer);
}

/// Add pressure to recruit followers to a religion
void CvCityReligions::AddReligiousPressure(CvReligiousFollowChangeReason eReason, ReligionTypes eReligion, int iPressure, PlayerTypes eResponsiblePlayer)
{
	bool bFoundIt = false;

	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			it->m_iPressure += iPressure;
			bFoundIt = true;
		}

		//  If this is pressure from a real religion, reduce presence of pantheon by the same amount
		else if(eReligion > RELIGION_PANTHEON && it->m_eReligion == RELIGION_PANTHEON)
		{
			it->m_iPressure = max(0, (it->m_iPressure - iPressure));
		}

		else if (it->m_eReligion > RELIGION_PANTHEON && eReason == FOLLOWER_CHANGE_MISSIONARY)
		{
			const CvReligion *pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligion, NO_PLAYER);
			int iPressureErosion = pReligion->m_Beliefs.GetOtherReligionPressureErosion();  // Normally 0
			if (iPressureErosion > 0)
			{
				int iErosionAmount = iPressureErosion * iPressure / 100;
				it->m_iPressure = max(0, (it->m_iPressure - iErosionAmount));
			}
		}
	}

	// Didn't find it, add new entry
	if(!bFoundIt)
	{
		CvReligionInCity newReligion(eReligion, false, 0, iPressure);
		m_ReligionStatus.push_back(newReligion);
	}

	RecomputeFollowers(eReason, eOldMajorityReligion, eResponsiblePlayer);
}

/// Simulate prophet spread
void CvCityReligions::SimulateProphetSpread(ReligionTypes eReligion, int iPressure)
{
	int iAtheismPressure = 0;
	int iReligionPressure = 0;
	int iPressureRetained = 0;
	ReligionTypes ePressureRetainedReligion = NO_RELIGION;

	CopyToSimulatedStatus();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); ++it)
#else
	for(it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == NO_RELIGION)
		{
			iAtheismPressure = it->m_iPressure;
		}
		else if(eReligion == it->m_eReligion)
		{
			iReligionPressure = it->m_iPressure;
		}

		if (it->m_eReligion > RELIGION_PANTHEON && eReligion != it->m_eReligion)
		{
			const CvReligion *pReligion = GC.getGame().GetGameReligions()->GetReligion(it->m_eReligion, NO_PLAYER);
			int iPressureRetention = pReligion->m_Beliefs.GetInquisitorPressureRetention();  // Normally 0
			if (iPressureRetention > 0)
			{
				ePressureRetainedReligion = it->m_eReligion;
				iPressureRetained = it->m_iPressure * iPressureRetention / 100;
			}
		}
	}

	// Clear list
	m_SimulatedStatus.clear();

	// Add atheists and this back in
	CvReligionInCity atheism(NO_RELIGION, false, 0, iAtheismPressure);
	m_SimulatedStatus.push_back(atheism);
	CvReligionInCity prophetReligion(eReligion, false, 0, iReligionPressure + iPressure);
	m_SimulatedStatus.push_back(prophetReligion);

	if (ePressureRetainedReligion != NO_RELIGION)
	{
		CvReligionInCity pressureRetainedReligion(ePressureRetainedReligion, false, 0, iPressureRetained);
		m_SimulatedStatus.push_back(pressureRetainedReligion);

	}

	SimulateFollowers();
}

/// Simulate religious pressure addition
void CvCityReligions::SimulateReligiousPressure(ReligionTypes eReligion, int iPressure)
{
	bool bFoundIt = false;

	CopyToSimulatedStatus();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); ++it)
#else
	for(it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			it->m_iPressure += iPressure;
			bFoundIt = true;
		}

		//  If this is pressure from a real religion, reduce presence of pantheon by the same amount
		else if(eReligion > RELIGION_PANTHEON && it->m_eReligion == RELIGION_PANTHEON)
		{
			it->m_iPressure = max(0, (it->m_iPressure - iPressure));
		}

		else if (it->m_eReligion > RELIGION_PANTHEON)
		{
			const CvReligion *pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligion, NO_PLAYER);
			int iPressureErosion = pReligion->m_Beliefs.GetOtherReligionPressureErosion();  // Normally 0
			if (iPressureErosion > 0)
			{
				int iErosionAmount = iPressureErosion * iPressure / 100;
				it->m_iPressure = max(0, (it->m_iPressure - iErosionAmount));
			}
		}
	}

	// Didn't find it, add new entry
	if(!bFoundIt)
	{
		CvReligionInCity newReligion(eReligion, false, 0, iPressure);
		m_SimulatedStatus.push_back(newReligion);
	}

	SimulateFollowers();
}

/// Convert some percentage of followers from one religion to another
void CvCityReligions::ConvertPercentFollowers(ReligionTypes eToReligion, ReligionTypes eFromReligion, int iPercent)
{
	int iPressureConverting = 0;

	// Find old religion
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eFromReligion)
		{
			iPressureConverting = it->m_iPressure * iPercent / 100;
			it->m_iPressure -= iPressureConverting;
			if (it->m_iPressure < 0)
			{
				it->m_iPressure = 0;
			}
		}
	}
	AddReligiousPressure(FOLLOWER_CHANGE_SCRIPTED_CONVERSION, eToReligion, iPressureConverting, NO_PLAYER);
}

/// Add pressure to recruit followers to a religion
void CvCityReligions::AddHolyCityPressure()
{
	bool bRecompute = false;
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_bFoundedHere)
		{
			int iPressure = GC.getGame().getGameSpeedInfo().getReligiousPressureAdjacentCity();
			iPressure *=  GC.getRELIGION_PER_TURN_FOUNDING_CITY_PRESSURE();
			it->m_iPressure += iPressure;

			// Found it, so we're done
			bRecompute = true;
		}
	}

	// Didn't find it, add new entry
	if(bRecompute)
	{
		RecomputeFollowers(FOLLOWER_CHANGE_HOLY_CITY, eOldMajorityReligion);
	}
}

/// Add pressure to recruit followers to a religion
void CvCityReligions::AddSpyPressure(ReligionTypes eReligion, int iBasePressure)
{
	bool bRecompute = false;
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			int iPressure = GC.getGame().getGameSpeedInfo().getReligiousPressureAdjacentCity();
			iPressure *= iBasePressure;
			it->m_iPressure += iPressure;

			// Found it, so we're done
			bRecompute = true;
		}
	}

	// Didn't find it, add new entry
	if(bRecompute)
	{
		RecomputeFollowers(FOLLOWER_CHANGE_SPY_PRESSURE, eOldMajorityReligion);
	}
}

/// Set this city to have all citizens following a religion (mainly for scripting)
void CvCityReligions::AdoptReligionFully(ReligionTypes eReligion)
{
	m_ReligionStatus.clear();

	CvReligionInCity religion;

	// Add 1 pop of Atheism (needed in case other religions wiped out by an Inquisitor/Prophet
	religion.m_bFoundedHere = false;
	religion.m_eReligion = NO_RELIGION;
	religion.m_iFollowers = 1;
	religion.m_iPressure = religion.m_iFollowers * GC.getRELIGION_ATHEISM_PRESSURE_PER_POP();
	m_ReligionStatus.push_back(religion);

	// Now add full pop of this religion
	religion.m_bFoundedHere = false;
	religion.m_eReligion = eReligion;
	religion.m_iFollowers = m_pCity->getPopulation();
	religion.m_iPressure = religion.m_iFollowers * GC.getRELIGION_ATHEISM_PRESSURE_PER_POP();
	m_ReligionStatus.push_back(religion);

	m_pCity->UpdateReligion(eReligion);
}

/// Remove presence of old owner's pantheon (used when a city is conquered)
void CvCityReligions::RemoveFormerPantheon()
{
#ifndef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	bool bFoundIt = false;
#endif
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end() && !bFoundIt; it++)
#endif
	{
		if(it->m_eReligion == RELIGION_PANTHEON)
		{
			m_ReligionStatus.erase(it);

			// Found it, so we're done
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
			break;
#else
			bFoundIt = true;
#endif
		}
	}

	RecomputeFollowers(FOLLOWER_CHANGE_CONQUEST, eOldMajorityReligion);
}

/// Remove other religions in a city (used by Inquisitor)
void CvCityReligions::RemoveOtherReligions(ReligionTypes eReligion, PlayerTypes eResponsiblePlayer)
{
	ReligionTypes eOldMajorityReligion = GetReligiousMajority();

	// Copy list
	ReligionInCityList tempList;
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		tempList.push_back(*it);
	}

	// Erase old list
	m_ReligionStatus.clear();

	// Recopy just what we want to keep
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = tempList.begin(); it != tempList.end(); ++it)
#else
	for(it = tempList.begin(); it != tempList.end(); it++)
#endif
	{
		int iPressureRetained = 0;

		ReligionTypes eLoopReligion = it->m_eReligion;
		if (eLoopReligion > RELIGION_PANTHEON && eLoopReligion != eReligion)
		{
			const CvReligion *pReligion = GC.getGame().GetGameReligions()->GetReligion(eLoopReligion, NO_PLAYER);
			iPressureRetained = pReligion->m_Beliefs.GetInquisitorPressureRetention();  // Normally 0
		}

		if (eLoopReligion == NO_RELIGION || eLoopReligion == eReligion || iPressureRetained > 0)
		{
			if (iPressureRetained > 0)
			{
				it->m_iPressure = it->m_iPressure * iPressureRetained / 100;
			}

			m_ReligionStatus.push_back(*it);
		}
	}

	RecomputeFollowers(FOLLOWER_CHANGE_REMOVE_HERESY, eOldMajorityReligion, eResponsiblePlayer);
}

/// Called from the trade system when a trade connection is made between two cities
void CvCityReligions::UpdateNumTradeRouteConnections(CvCity* pOtherCity)
{
	ReligionTypes eReligiousMajority = GetReligiousMajority();

	// if there isn't a religious connection, whatvz
	if (eReligiousMajority == NO_RELIGION)
	{
		return;
	}

	const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(eReligiousMajority, m_pCity->getOwner());
	if (!pReligion)
	{
		return;
	}

	// Are the cities within the minimum distance?
	int iDistance = GC.getRELIGION_ADJACENT_CITY_DISTANCE();

	// Boost to distance due to belief?
	int iDistanceMod = pReligion->m_Beliefs.GetSpreadDistanceModifier();
	if(iDistanceMod > 0)
	{
		iDistance *= (100 + iDistanceMod);
		iDistance /= 100;
	}

	bool bWithinDistance = (plotDistance(m_pCity->getX(), m_pCity->getY(), pOtherCity->getX(), pOtherCity->getY()) <= iDistance);

	// if not within distance, then we're using a trade route
	if (!bWithinDistance) 
	{
		pOtherCity->GetCityReligions()->IncrementNumTradeRouteConnections(eReligiousMajority, 1);
	}
}

/// Increment the number of trade connections a city has
void CvCityReligions::IncrementNumTradeRouteConnections(ReligionTypes eReligion, int iNum)
{
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		if(it->m_eReligion == eReligion)
		{
			it->m_iNumTradeRoutesApplyingPressure += iNum;
			break;
		}
	}
}

/// How many followers would we have adding this religious pressure here?
int CvCityReligions::GetNumFollowersAfterSpread(ReligionTypes eReligion, int iConversionStrength)
{
	SimulateReligiousPressure(eReligion, iConversionStrength);
	return GetNumSimulatedFollowers(eReligion);
}

/// How many followers would we have having a prophet add religious pressure here?
int CvCityReligions::GetNumFollowersAfterProphetSpread(ReligionTypes eReligion, int iConversionStrength)
{
	SimulateProphetSpread(eReligion, iConversionStrength);
	return GetNumSimulatedFollowers(eReligion);
}

/// What would the majority religion be adding this religious pressure here?
ReligionTypes CvCityReligions::GetMajorityReligionAfterSpread(ReligionTypes eReligion, int iConversionStrength)
{
	SimulateReligiousPressure(eReligion, iConversionStrength);
	return GetSimulatedReligiousMajority();
}

/// What would the majority religion be adding this religious pressure here?
ReligionTypes CvCityReligions::GetMajorityReligionAfterProphetSpread(ReligionTypes eReligion, int iConversionStrength)
{
	SimulateProphetSpread(eReligion, iConversionStrength);
	return GetSimulatedReligiousMajority();
}

/// Resets the number of trade routes pressuring a city
void CvCityReligions::ResetNumTradeRoutePressure()
{
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		it->m_iNumTradeRoutesApplyingPressure = 0;
	}
}

// PRIVATE METHODS

/// Calculate the number of followers for each religion
void CvCityReligions::RecomputeFollowers(CvReligiousFollowChangeReason eReason, ReligionTypes eOldMajorityReligion, PlayerTypes eResponsibleParty)
{
	int iOldFollowers = GetNumFollowers(eOldMajorityReligion);
	int iUnassignedFollowers = m_pCity->getPopulation();
	int iPressurePerFollower;

	// Safety check to avoid divide by zero
	if (iUnassignedFollowers < 1)
	{
		CvAssertMsg (false, "Invalid city population when recomputing followers");
		return;
	}

	// Find total pressure
	int iTotalPressure = 0;
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		iTotalPressure += it->m_iPressure;
	}

	// safety check - if pressure was wiped out somehow, just rebuild pressure of 1 atheist
	if (iTotalPressure <= 0)
	{
		m_ReligionStatus.clear();

		CvReligionInCity religion;
		religion.m_bFoundedHere = false;
		religion.m_eReligion = NO_RELIGION;
		religion.m_iFollowers = 1;
		religion.m_iPressure = GC.getRELIGION_ATHEISM_PRESSURE_PER_POP();
		m_ReligionStatus.push_back(religion);

		iTotalPressure = GC.getRELIGION_ATHEISM_PRESSURE_PER_POP();
	}

	iPressurePerFollower = iTotalPressure / iUnassignedFollowers;

	// Loop through each religion
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		it->m_iFollowers = it->m_iPressure / iPressurePerFollower;
		iUnassignedFollowers -= it->m_iFollowers;
		it->m_iTemp = it->m_iPressure - (it->m_iFollowers * iPressurePerFollower);  // Remainder
	}

	// Assign out the remainder
	for (int iI = 0; iI < iUnassignedFollowers; iI++)
	{
		ReligionInCityList::iterator itLargestRemainder = NULL;
		int iLargestRemainder = 0;

#ifdef AUI_WARNING_FIXES
		for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end() && it; ++it)
#else
		for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
		{
			if (it->m_iTemp > iLargestRemainder)
			{
				iLargestRemainder = it->m_iTemp;
				itLargestRemainder = it;
			}
		}

		if (itLargestRemainder && iLargestRemainder > 0)
		{
			itLargestRemainder->m_iFollowers++;
			itLargestRemainder->m_iTemp = 0;
		}
	}

	ReligionTypes eMajority = GetReligiousMajority();
	int iFollowers = GetNumFollowers(eMajority);

	if(eMajority != eOldMajorityReligion || iFollowers != iOldFollowers)
	{
		CityConvertsReligion(eMajority, eOldMajorityReligion, eResponsibleParty);
		GC.GetEngineUserInterface()->setDirty(CityInfo_DIRTY_BIT, true);
		LogFollowersChange(eReason);
	}
}

/// Calculate the number of followers for each religion from simulated data
void CvCityReligions::SimulateFollowers()
{
	int iUnassignedFollowers = m_pCity->getPopulation();
	int iPressurePerFollower;

	// Find total pressure
	int iTotalPressure = 0;
	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); ++it)
#else
	for(it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); it++)
#endif
	{
		iTotalPressure += it->m_iPressure;
	}

	// safety check
	if (iTotalPressure == 0 || iUnassignedFollowers == 0)
	{
		CvAssertMsg(false, "Internal religion data error. Send save to Ed");
		return;
	}

	iPressurePerFollower = iTotalPressure / iUnassignedFollowers;

	// Loop through each religion
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); ++it)
#else
	for(it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); it++)
#endif
	{
		it->m_iFollowers = it->m_iPressure / iPressurePerFollower;
		iUnassignedFollowers -= it->m_iFollowers;
		it->m_iTemp = it->m_iPressure - (it->m_iFollowers * iPressurePerFollower);  // Remainder
	}

	// Assign out the remainder
	for (int iI = 0; iI < iUnassignedFollowers; iI++)
	{
		ReligionInCityList::iterator itLargestRemainder = NULL;
		int iLargestRemainder = 0;

#ifdef AUI_WARNING_FIXES
		for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end() && it; ++it)
#else
		for (it = m_SimulatedStatus.begin(); it != m_SimulatedStatus.end(); it++)
#endif
		{
			if (it->m_iTemp > iLargestRemainder)
			{
				iLargestRemainder = it->m_iTemp;
				itLargestRemainder = it;
			}
		}

		if (itLargestRemainder && iLargestRemainder > 0)
		{
			itLargestRemainder->m_iFollowers++;
			itLargestRemainder->m_iTemp = 0;
		}
	}
}

/// Copy data to prepare to simulate a conversion
void CvCityReligions::CopyToSimulatedStatus()
{
	m_SimulatedStatus.clear();

	ReligionInCityList::iterator it;
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); ++it)
#else
	for(it = m_ReligionStatus.begin(); it != m_ReligionStatus.end(); it++)
#endif
	{
		m_SimulatedStatus.push_back(*it);
	}
}

/// Implement changes from a city changing religion
void CvCityReligions::CityConvertsReligion(ReligionTypes eMajority, ReligionTypes eOldMajority, PlayerTypes eResponsibleParty)
{
	CvGameReligions* pReligions = GC.getGame().GetGameReligions();

	m_pCity->UpdateReligion(eMajority);

	if(eOldMajority > RELIGION_PANTHEON)
	{
		const CvReligion* pOldReligion = pReligions->GetReligion(eOldMajority, NO_PLAYER);
		GET_PLAYER(pOldReligion->m_eFounder).UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
		GET_PLAYER(pOldReligion->m_eFounder).doSelfConsistencyCheckAllCities();
#endif
	}

	if(eMajority > RELIGION_PANTHEON)
	{
		const CvReligion* pNewReligion = pReligions->GetReligion(eMajority, NO_PLAYER);
		GET_PLAYER(pNewReligion->m_eFounder).UpdateReligion();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
		GET_PLAYER(pNewReligion->m_eFounder).doSelfConsistencyCheckAllCities();
#endif

		// Pay adoption bonuses (if any)
		if(!m_bHasPaidAdoptionBonus)
		{
			int iGoldBonus = pNewReligion->m_Beliefs.GetGoldWhenCityAdopts();
			iGoldBonus *= GC.getGame().getGameSpeedInfo().getTrainPercent();;
			iGoldBonus /= 100;

			if(iGoldBonus > 0)
			{
				GET_PLAYER(pNewReligion->m_eFounder).GetTreasury()->ChangeGold(iGoldBonus);
				SetPaidAdoptionBonus(true);

				if(pNewReligion->m_eFounder == GC.getGame().getActivePlayer())
				{
					char text[256] = {0};
					sprintf_s(text, "[COLOR_YELLOW]+%d[ENDCOLOR][ICON_GOLD]", iGoldBonus);
					GC.GetEngineUserInterface()->AddPopupText(m_pCity->getX(), m_pCity->getY(), text, 0.5f);
				}
			}
		}

		// Notification if the player's city was converted to a religion they didn't found
		PlayerTypes eOwnerPlayer = m_pCity->getOwner();
		CvPlayerAI& kOwnerPlayer = GET_PLAYER(eOwnerPlayer);
		const ReligionTypes eOwnerPlayerReligion = kOwnerPlayer.GetReligions()->GetReligionCreatedByPlayer();

		if(eOwnerPlayer != eResponsibleParty && eMajority != eOldMajority && pNewReligion->m_eFounder != eOwnerPlayer
			&& eOwnerPlayerReligion > RELIGION_PANTHEON)
		{
			if(kOwnerPlayer.GetNotifications())
			{
				Localization::String strMessage;
				Localization::String strSummary;
				strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_RELIGION_SPREAD_ACTIVE_PLAYER", m_pCity->getName());
				strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_SPREAD_ACTIVE_PLAYER_S");
				kOwnerPlayer.GetNotifications()->Add(NOTIFICATION_RELIGION_SPREAD, strMessage.toUTF8(), strSummary.toUTF8(), m_pCity->getX(), m_pCity->getY(), -1);
			}

			//Achievements!
			if(eOwnerPlayer == GC.getGame().getActivePlayer()){
				const CvReligion* pkReligion = GC.getGame().GetGameReligions()->GetReligion(eOwnerPlayerReligion, eOwnerPlayer);
				if(pkReligion != NULL)
				{
					if(m_pCity->getX() == pkReligion->m_iHolyCityX && m_pCity->getY() == pkReligion->m_iHolyCityY)
					{
						gDLL->UnlockAchievement(ACHIEVEMENT_XP1_20);
					}
				}
			}
		}

		else if(eOwnerPlayer != eResponsibleParty && eMajority != eOldMajority && eOldMajority == NO_RELIGION)
		{
			if(kOwnerPlayer.GetNotifications())
			{
				Localization::String strMessage;
				Localization::String strSummary;
				strMessage = GetLocalizedText("TXT_KEY_NOTIFICATION_RELIGION_SPREAD_INITIAL_CONVERSION", m_pCity->getName());
				strSummary = Localization::Lookup("TXT_KEY_NOTIFICATION_RELIGION_SPREAD_INITIAL_CONVERSION_S");
				kOwnerPlayer.GetNotifications()->Add(NOTIFICATION_RELIGION_SPREAD_NATURAL, strMessage.toUTF8(), strSummary.toUTF8(), m_pCity->getX(), m_pCity->getY(), -1);
			}
		}

		//More Achievements
		if(m_pCity->getOwner() != GC.getGame().getActivePlayer() && pNewReligion->m_eFounder == GC.getGame().getActivePlayer())
		{
			if(m_pCity->GetCityReligions()->IsHolyCityAnyReligion() && !m_pCity->GetCityReligions()->IsHolyCityForReligion(pNewReligion->m_eReligion))
			{
				gDLL->UnlockAchievement(ACHIEVEMENT_XP1_18);
			}
		}

#ifndef AUI_ACHIEVEMENT_FIX_RELIGION_WE_ARE_FAMILY_WORKING
		if(m_pCity->isCapital() && pNewReligion->m_eFounder == GC.getGame().getActivePlayer())
		{
			//Determine if this is a standard size or larger map.
			bool bIsStandardOrLarger = false;
			Database::Connection* pDB = GC.GetGameDatabase();
			Database::Results kStandardSize;
			if(pDB->SelectAt(kStandardSize, "Worlds", "Type", "WORLDSIZE_STANDARD"))
			{
				if(kStandardSize.Step())
				{
					int idColumn = kStandardSize.ColumnPosition("ID");
					if(idColumn >= 0)
					{
						WorldSizeTypes eWorldSize = GC.getMap().getWorldSize();
						int standardWorldSize = kStandardSize.GetInt(idColumn);
						if(eWorldSize >= standardWorldSize)
						{
							bIsStandardOrLarger = true;
						}
					}
				}
			}

			if(bIsStandardOrLarger)
			{
				//Determine if this religion has spread to all capitals
				bool bSpreadToAllCapitals = true;
				for(int i = 0; i < MAX_MAJOR_CIVS; ++i)
				{
					CvPlayerAI& kPlayer = GET_PLAYER(static_cast<PlayerTypes>(i));
					if(kPlayer.isAlive())
					{
						CvCity* pCapital = kPlayer.getCapitalCity();
						if(pCapital != NULL)
						{
							CvCityReligions* pCityReligions = pCapital->GetCityReligions();
							if(pCityReligions != NULL)
							{
								if(pCityReligions->GetReligiousMajority() != pNewReligion->m_eFounder)
								{
									bSpreadToAllCapitals = false;
									break;
								}
							}
						}
					}

					if(bSpreadToAllCapitals)
					{
						gDLL->UnlockAchievement(ACHIEVEMENT_XP1_19);
					}
				}
			}
		}
#endif

		// Diplo implications (there must have been religion switch and a responsible party)
		if(eMajority != eOldMajority && eResponsibleParty != NO_PLAYER)
		{
			// Is the city owner not the founder of this religion?
			if(pNewReligion->m_eFounder != m_pCity->getOwner())
			{
				CvPlayer& kCityOwnerPlayer = GET_PLAYER(m_pCity->getOwner());

				// Did he found another religion?
				ReligionTypes eCityOwnerReligion = kCityOwnerPlayer.GetReligions()->GetReligionCreatedByPlayer();
				if(eCityOwnerReligion >= RELIGION_PANTHEON)
				{
					int iPoints = 0;

					// His religion wasn't present here, minor hit
					if(eOldMajority != eCityOwnerReligion)
					{
						iPoints = GC.getRELIGION_DIPLO_HIT_INITIAL_CONVERT_FRIENDLY_CITY(); /*1*/
					}

					// This was his holy city; huge hit!
					else if(m_pCity->GetCityReligions()->IsHolyCityForReligion(eCityOwnerReligion))
					{
						iPoints = GC.getRELIGION_DIPLO_HIT_CONVERT_HOLY_CITY(); /*25*/
					}

					// He had established his religion here, major hit
					else
					{
						iPoints = GC.getRELIGION_DIPLO_HIT_RELIGIOUS_FLIP_FRIENDLY_CITY(); /*3*/
					}

					kCityOwnerPlayer.GetDiplomacyAI()->ChangeNegativeReligiousConversionPoints(eResponsibleParty, iPoints);
				}
			}
		}

		ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
		if(pkScriptSystem)
		{
			CvLuaArgsHandle args;
			args->Push(m_pCity->getOwner());
			args->Push(eMajority);
			args->Push(m_pCity->getX());
			args->Push(m_pCity->getY());

			// Attempt to execute the game events.
			// Will return false if there are no registered listeners.
			bool bResult = false;
			LuaSupport::CallHook(pkScriptSystem, "CityConvertsReligion", args.get(), bResult);
		}
	}
}

/// Log a message with status information
void CvCityReligions::LogFollowersChange(CvReligiousFollowChangeReason eReason)
{
	if(GC.getLogging() && GC.getAILogging())
	{
		CvString strOutBuf;
		CvString strReasonString;
		CvString temp;
		FILogFile* pLog;
		CvCityReligions* pCityRel = m_pCity->GetCityReligions();

		pLog = LOGFILEMGR.GetLog(GC.getGame().GetGameReligions()->GetLogFileName(), FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		strOutBuf.Format("%03d, %d, ", GC.getGame().getElapsedGameTurns(), GC.getGame().getGameTurnYear());
		strOutBuf += m_pCity->getName();
		strOutBuf += ", ";

		// Add a reason string
		switch(eReason)
		{
		case FOLLOWER_CHANGE_ADJACENT_PRESSURE:
			strReasonString = "Adjacent city pressure";
			break;
		case FOLLOWER_CHANGE_HOLY_CITY:
			strReasonString = "Holy city pressure";
			break;
		case FOLLOWER_CHANGE_POP_CHANGE:
			strReasonString = "Population change";
			break;
		case FOLLOWER_CHANGE_RELIGION_FOUNDED:
			strReasonString = "Religion founded";
			break;
		case FOLLOWER_CHANGE_PANTHEON_FOUNDED:
			strReasonString = "Pantheon founded";
			break;
		case FOLLOWER_CHANGE_CONQUEST:
			strReasonString = "City captured";
			break;
		case FOLLOWER_CHANGE_MISSIONARY:
			strReasonString = "Missionary expended";
			break;
		case FOLLOWER_CHANGE_PROPHET:
			strReasonString = "Prophet spreading";
			break;
		case FOLLOWER_CHANGE_REMOVE_HERESY:
			strReasonString = "Remove heresy";
			break;
		case FOLLOWER_CHANGE_SCRIPTED_CONVERSION:
			strReasonString = "Scripted conversion";
			break;
		case FOLLOWER_CHANGE_SPY_PRESSURE:
			strReasonString = "Spy pressure";
			break;
		}
		strOutBuf += strReasonString + ", ";
		temp.Format("Pop: %d", m_pCity->getPopulation());
		strOutBuf += temp;
		if(pCityRel->IsReligionInCity())
		{
			ReligionTypes eMajority = pCityRel->GetReligiousMajority();
			if(eMajority != NO_RELIGION)
			{
				CvReligionEntry* pEntry = GC.getReligionInfo(eMajority);
				if(pEntry)
				{
					strOutBuf += ", Majority: ";
					strOutBuf += pEntry->GetDescription();
					temp.Format("(%d)", pCityRel->GetNumFollowers(eMajority));
					strOutBuf += temp;
				}
			}
			
			ReligionTypes eSecondary = pCityRel->GetSecondaryReligion();
			if (eSecondary != NO_RELIGION)
			{
				CvReligionEntry* pEntry = GC.getReligionInfo(eSecondary);
				if(pEntry)
				{
					strOutBuf += ", Secondary: ";
					strOutBuf += pEntry->GetDescription();
					temp.Format("(%d)", pCityRel->GetNumFollowers(eSecondary));
					strOutBuf += temp;
				}
			}

			temp.Format("Atheists: %d", pCityRel->GetNumFollowers(NO_RELIGION));
			strOutBuf += ", " + temp;
		}
		else
		{
			strOutBuf += ", No religion in city";
		}

		pLog->Msg(strOutBuf);
	}
}
/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvCityReligions& writeTo)
{
	uint uiVersion;

	loadFrom >> uiVersion;

	if(uiVersion >= 2)
	{
		bool bTemp;
		loadFrom >> bTemp;
		writeTo.SetPaidAdoptionBonus(bTemp);
	}
	else
	{
		writeTo.SetPaidAdoptionBonus(false);
	}

	if (uiVersion >= 3)
	{
		int iTemp;
		loadFrom >> iTemp;
		writeTo.SetReligiousPressureModifier(iTemp);
	}
	else
	{
		writeTo.SetReligiousPressureModifier(0);
	}

	int iEntriesToRead;
	CvReligionInCity tempItem;

	writeTo.m_ReligionStatus.clear();
	loadFrom >> iEntriesToRead;
	for(int iI = 0; iI < iEntriesToRead; iI++)
	{
		loadFrom >> tempItem;
		writeTo.m_ReligionStatus.push_back(tempItem);
	}

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvCityReligions& readFrom)
{
	uint uiVersion = 3;

	saveTo << uiVersion;

	saveTo << readFrom.HasPaidAdoptionBonus();

	saveTo << readFrom.GetReligiousPressureModifier();

	ReligionInCityList::const_iterator it;
	saveTo << readFrom.m_ReligionStatus.size();
#ifdef AUI_ITERATOR_POSTFIX_INCREMENT_OPTIMIZATIONS
	for (it = readFrom.m_ReligionStatus.begin(); it != readFrom.m_ReligionStatus.end(); ++it)
#else
	for(it = readFrom.m_ReligionStatus.begin(); it != readFrom.m_ReligionStatus.end(); it++)
#endif
	{
		saveTo << *it;
	}

	return saveTo;
}

//=====================================
// CvGameReligions
//=====================================
/// Constructor
CvUnitReligion::CvUnitReligion(void):
	m_eReligion(NO_RELIGION),
	m_iStrength(0),
	m_iSpreadsLeft(0)
{
}

/// Initialize class data
void CvUnitReligion::Init()
{
	m_eReligion = NO_RELIGION;
	m_iStrength = 0;
	m_iSpreadsLeft = 0;
}

/// Serialization read
FDataStream& operator>>(FDataStream& loadFrom, CvUnitReligion& writeTo)
{
	uint uiVersion;

	loadFrom >> uiVersion;

	int temp;
	loadFrom >> temp;
	writeTo.SetReligion((ReligionTypes)temp);
	loadFrom >> temp;
	writeTo.SetReligiousStrength(temp);

	if(uiVersion >= 2)
	{
		loadFrom >> temp;
		writeTo.SetSpreadsLeft(temp);
	}
	else
	{
		writeTo.SetSpreadsLeft(0);
	}

	return loadFrom;
}

/// Serialization write
FDataStream& operator<<(FDataStream& saveTo, const CvUnitReligion& readFrom)
{
	uint uiVersion = 2;

	saveTo << uiVersion;

	saveTo << readFrom.GetReligion();
	saveTo << readFrom.GetReligiousStrength();
	saveTo << readFrom.GetSpreadsLeft();

	return saveTo;
}

//=====================================
// CvReligionAI
//=====================================
/// Constructor
CvReligionAI::CvReligionAI(void):
#ifdef AUI_WARNING_FIXES
	m_pBeliefs(NULL),
#endif
	m_pPlayer(NULL)
{
}

/// Destructor
CvReligionAI::~CvReligionAI(void)
{
	Uninit();
}

/// Initialize class data
void CvReligionAI::Init(CvBeliefXMLEntries* pBeliefs, CvPlayer* pPlayer)
{
	m_pBeliefs = pBeliefs;
	m_pPlayer = pPlayer;

	Reset();
}

/// Cleanup
void CvReligionAI::Uninit()
{

}

/// Reset
void CvReligionAI::Reset()
{

}

/// Serialization read
void CvReligionAI::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;
}

/// Serialization write
void CvReligionAI::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;
}

/// Called every turn to see what to spend Faith on
void CvReligionAI::DoTurn()
{
	// Only AI players use this function for now
	if(m_pPlayer->isHuman())
	{
		return;
	}

	AI_PERF_FORMAT("AI-perf.csv", ("Religion AI, Turn %03d, %s", GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );

	DoFaithPurchases();
}

/// Select the belief most helpful to this pantheon
BeliefTypes CvReligionAI::ChoosePantheonBelief()
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailablePantheonBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailablePantheonBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			const int iScore = ScoreBelief(pEntry);
			if(iScore > 0)
			{
				beliefChoices.push_back(eBelief, iScore);
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // Throw out two-thirds of the choices -- this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Select the belief most helpful to this pantheon
BeliefTypes CvReligionAI::ChooseFounderBelief()
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailableFounderBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailableFounderBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			const int iScore = ScoreBelief(pEntry);
			if(iScore > 0)
			{
				beliefChoices.push_back(eBelief, iScore);
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Select the belief most helpful to this pantheon
BeliefTypes CvReligionAI::ChooseFollowerBelief()
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailableFollowerBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailableFollowerBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			const int iScore = ScoreBelief(pEntry);
			if(iScore > 0)
			{
				beliefChoices.push_back(eBelief, iScore);
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Select the belief most helpful to enhance this religion
BeliefTypes CvReligionAI::ChooseEnhancerBelief()
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailableEnhancerBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailableEnhancerBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			const int iScore = ScoreBelief(pEntry);
			if(iScore > 0)
			{
				beliefChoices.push_back(eBelief, iScore);
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Select the belief most helpful to enhance this religion
BeliefTypes CvReligionAI::ChooseBonusBelief(int iExcludeBelief1, int iExcludeBelief2, int iExcludeBelief3)
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailableBonusBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailableBonusBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			if (pEntry->GetID() != iExcludeBelief1 && pEntry->GetID() != iExcludeBelief2 && pEntry->GetID() != iExcludeBelief3)
			{
				const int iScore = ScoreBelief(pEntry);
				if(iScore > 0)
				{
					beliefChoices.push_back(eBelief, iScore);
				}
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Select the belief most helpful to gain from Reformation social policy
BeliefTypes CvReligionAI::ChooseReformationBelief()
{
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();
	CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true> beliefChoices;

#ifdef AUI_WARNING_FIXES
	const std::vector<BeliefTypes>& availableBeliefs = pGameReligions->GetAvailableReformationBeliefs();

	for (std::vector<BeliefTypes>::const_iterator it = availableBeliefs.begin(); it != availableBeliefs.end(); ++it)
#else
	std::vector<BeliefTypes> availableBeliefs = pGameReligions->GetAvailableReformationBeliefs();

	for(std::vector<BeliefTypes>::iterator it = availableBeliefs.begin();
	        it!= availableBeliefs.end(); ++it)
#endif
	{
		const BeliefTypes eBelief = (*it);
		CvBeliefEntry* pEntry = m_pBeliefs->GetEntry(eBelief);
		if(pEntry)
		{
			const int iScore = ScoreBelief(pEntry);
			if(iScore > 0)
			{
				beliefChoices.push_back(eBelief, iScore);
			}
		}
	}

	// Choose from weighted vector
	beliefChoices.SortItems();
	int iNumChoices = MIN(beliefChoices.size(),3);   // this was way too loose as choices way down were being selected now only top 3
	RandomNumberDelegate fcn = MakeDelegate(&GC.getGame(), &CvGame::getJonRandNum);
	BeliefTypes rtnValue = beliefChoices.ChooseFromTopChoices(iNumChoices, &fcn, "Choosing belief from Top Choices");
	LogBeliefChoices(beliefChoices, rtnValue);

	return rtnValue;
}

/// Find the city where a missionary should next spread his religion
#ifdef AUI_CONSTIFY
CvCity* CvReligionAI::ChooseMissionaryTargetCity(UnitHandle pUnit) const
#else
CvCity* CvReligionAI::ChooseMissionaryTargetCity(UnitHandle pUnit)
#endif
{
	ReligionTypes eMyReligion = GetReligionToSpread();
	int iBestScore = 0;
	CvCity* pBestCity = NULL;

	if(eMyReligion <= RELIGION_PANTHEON)
	{
		return NULL;
	}

#pragma warning ( push )
#pragma warning ( disable : 6011 ) // Dereferencing NULL pointer
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseMissionaryTargetCity: %s %d, Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );
#pragma warning ( pop )

	// Loop through all the players
	for(int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		CvPlayer& kPlayer = GET_PLAYER((PlayerTypes)iI);
		if(kPlayer.isAlive())
		{
			// Loop through each of their cities
			int iLoop;
			CvCity* pLoopCity;
			for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				if(pLoopCity && !pLoopCity->GetCityReligions()->IsDefendedAgainstSpread(eMyReligion))
				{
					int iScore = ScoreCityForMissionary(pLoopCity, pUnit);
					if(iScore > iBestScore)
					{
						iBestScore = iScore;
						pBestCity = pLoopCity;
					}
				}
			}
		}
	}

	return pBestCity;
}

/// Choose a plot next to the target city for the missionary to maneuver to
#ifdef AUI_CONSTIFY
CvPlot* CvReligionAI::ChooseMissionaryTargetPlot(UnitHandle pUnit, int* piTurns) const
#else
CvPlot* CvReligionAI::ChooseMissionaryTargetPlot(UnitHandle pUnit, int* piTurns)
#endif
{
	CvCity* pCity = ChooseMissionaryTargetCity(pUnit);
	int iBestNumTurns = MAX_INT;
	int iTurns;
	int iBestDistance = MAX_INT;
	int iDistance;
	CvPlot* pBestTarget = NULL;

	if(pCity == NULL)
	{
		return NULL;
	}

	// Our city with no civilian units?  If so go right in there
	if(pCity->getOwner() == m_pPlayer->GetID())
	{
		CvUnit* pFirstUnit = pCity->plot()->getUnitByIndex(0);
		if(!pFirstUnit || pFirstUnit->IsCombatUnit())
		{
			iTurns = TurnsToReachTarget(pUnit, pCity->plot(), true /* bReusePaths */);
			if(iTurns < MAX_INT)
			{
				*piTurns = iTurns;
				return pCity->plot();
			}
		}
	}

#pragma warning ( push )
#pragma warning ( disable : 6011 ) // Dereferencing NULL pointer
#ifdef AUI_PERF_LOGGING_FORMATTING_TWEAKS
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseMissionaryTargetPlot: %s %d (Targeting %s), Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), pCity->getName().c_str(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()));
#else
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseMissionaryTargetPlot: %s %d; %s, Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), pCity->getName().c_str(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );
#endif
#pragma warning ( pop )

	// Find adjacent plot with no units (that aren't our own)
	CvPlot* pLoopPlot;
	for(int iI = 0; iI < NUM_DIRECTION_TYPES; iI++)
	{
		pLoopPlot = plotDirection(pCity->getX(), pCity->getY(), ((DirectionTypes)iI));

		if(pLoopPlot != NULL)
		{
			CvUnit* pFirstUnit = pLoopPlot->getUnitByIndex(0);
			if(pFirstUnit && pFirstUnit->getOwner() != m_pPlayer->GetID())
			{
				continue;
			}

			if(pUnit->CanSpreadReligion(pLoopPlot))
			{
				iTurns = TurnsToReachTarget(pUnit, pLoopPlot, true /* bReusePaths */);
				if(iTurns < MAX_INT)
				{
					iDistance = plotDistance(pUnit->getX(), pUnit->getY(), pLoopPlot->getX(), pLoopPlot->getY());

					// Consider it to be twice as far if a water plot (those are dangerous!)
					if(pLoopPlot->isWater())
					{
						iDistance *= 2;
					}

					if(iTurns < iBestNumTurns || (iTurns == iBestNumTurns && iDistance < iBestDistance))
					{
						iBestNumTurns = iTurns;
						iBestDistance = iDistance;
						pBestTarget = pLoopPlot;
					}
				}
			}
		}
	}

	if(piTurns)
		*piTurns = iBestNumTurns;
	return pBestTarget;
}

/// Find the city where an inquisitor should next remove heresy
#ifdef AUI_CONSTIFY
CvCity* CvReligionAI::ChooseInquisitorTargetCity(UnitHandle pUnit) const
#else
CvCity* CvReligionAI::ChooseInquisitorTargetCity(UnitHandle pUnit)
#endif
{
	ReligionTypes eMyReligion = GetReligionToSpread();
	int iBestScore = 0;
	CvCity* pBestCity = NULL;

	if(eMyReligion <= RELIGION_PANTHEON)
	{
		return NULL;
	}

#pragma warning ( push )
#pragma warning ( disable : 6011 ) // Dereferencing NULL pointer
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseInquisitorTargetCity: %s %d, Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );
#pragma warning ( pop )

	// Loop through each of my cities
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pLoopCity)
		{
			int iScore = ScoreCityForInquisitor(pLoopCity, pUnit);
			if(iScore > iBestScore)
			{
				iBestScore = iScore;
				pBestCity = pLoopCity;
			}
		}
	}

	return pBestCity;
}

/// Choose a plot next to the target city for the inquisitor to maneuver to
#ifdef AUI_CONSTIFY
CvPlot* CvReligionAI::ChooseInquisitorTargetPlot(UnitHandle pUnit, int* piTurns) const
#else
CvPlot* CvReligionAI::ChooseInquisitorTargetPlot(UnitHandle pUnit, int* piTurns)
#endif
{
	CvCity* pCity = ChooseInquisitorTargetCity(pUnit);
	int iBestNumTurns = MAX_INT;
	int iTurns;
	int iBestDistance = MAX_INT;
	int iDistance;
	CvPlot* pBestTarget = NULL;

	if(pCity == NULL)
	{
		return NULL;
	}

	// Our city with no civilian units?  If so go right in there
	if(pCity->getOwner() == m_pPlayer->GetID())
	{
		CvUnit* pFirstUnit = pCity->plot()->getUnitByIndex(0);
		if(!pFirstUnit || pFirstUnit->IsCombatUnit())
		{
			iTurns = TurnsToReachTarget(pUnit, pCity->plot(), true /* bReusePaths */);
			if(iTurns < MAX_INT)
			{
				*piTurns = iTurns;
				return pCity->plot();
			}
		}
	}

#pragma warning ( push )
#pragma warning ( disable : 6011 ) // Dereferencing NULL pointer
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseInquisitorTargetPlot: %s %d; %s, Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), pCity->getName().c_str(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );
#pragma warning ( pop )

	// Find adjacent plot with no units (that aren't our own)
	CvPlot* pLoopPlot;
	for(int iI = 0; iI < NUM_DIRECTION_TYPES; iI++)
	{
		pLoopPlot = plotDirection(pCity->getX(), pCity->getY(), ((DirectionTypes)iI));

		if(pLoopPlot != NULL)
		{
			CvUnit* pFirstUnit = pLoopPlot->getUnitByIndex(0);
			if(pFirstUnit && pFirstUnit->getOwner() != m_pPlayer->GetID())
			{
				continue;
			}

			if(pUnit->CanRemoveHeresy(pLoopPlot))
			{
				iTurns = TurnsToReachTarget(pUnit, pLoopPlot, true /* bReusePaths */);
				if(iTurns < MAX_INT)
				{
					iDistance = plotDistance(pUnit->getX(), pUnit->getY(), pLoopPlot->getX(), pLoopPlot->getY());

					// Consider it to be twice as far if a water plot (those are dangerous!)
					if(pLoopPlot->isWater())
					{
						iDistance *= 2;
					}

					if(iTurns < iBestNumTurns || (iTurns == iBestNumTurns && iDistance < iBestDistance))
					{
						iBestNumTurns = iTurns;
						iBestDistance = iDistance;
						pBestTarget = pLoopPlot;
					}
				}
			}
		}
	}

	if(piTurns)
		*piTurns = iBestNumTurns;
	return pBestTarget;
}

/// If we were going to use a prophet to convert a city, which one would it be?
CvCity *CvReligionAI::ChooseProphetConversionCity(bool bOnlyBetterThanEnhancingReligion) const
{
	CvCity *pBestCity = NULL;
	CvCity *pHolyCity = NULL;
	int iBestScore = 50;  // Not zero because we don't want prophets to ALWAYS pick something up

	// Make sure we're spreading a religion and find holy city
	ReligionTypes eReligion = GetReligionToSpread();
	if (eReligion <= RELIGION_PANTHEON)
	{
		return pBestCity;
	}

	const CvReligion* pkReligion = GC.getGame().GetGameReligions()->GetReligion(eReligion, m_pPlayer->GetID());
	if (pkReligion)
	{
		CvPlot* pkPlot = GC.getMap().plot(pkReligion->m_iHolyCityX, pkReligion->m_iHolyCityY);
		if(pkPlot)
		{
			pHolyCity = pkPlot->getPlotCity();
		}
	}
	if (!pHolyCity)
	{
		return pBestCity;
	}

	// Look at our cities first, checking them for followers of other religions
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
		ReligionTypes eMajorityReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();
		int iHeretics = pLoopCity->GetCityReligions()->GetFollowersOtherReligions(eReligion);
		int iDistanceToHolyCity = plotDistance(pLoopCity->getX(), pLoopCity->getY(), pHolyCity->getX(), pHolyCity->getY());

		// If this is the holy city and it has been converted, want to go there no matter what
		if (pLoopCity == pHolyCity && eMajorityReligion != eReligion)
		{
			return pHolyCity;
		}

		// Otherwise score this city
		int iScore = (iHeretics * 100) / (iDistanceToHolyCity + 1);
		if (eMajorityReligion != eReligion && eMajorityReligion > RELIGION_PANTHEON)
		{
			iScore *= 3;
		}

		// Don't give it any score if our religion hasn't been enhanced yet
		else if (bOnlyBetterThanEnhancingReligion)
		{
			iScore = 0;
		}

		if (iScore > iBestScore)
		{
			iBestScore = iScore;
			pBestCity = pLoopCity;
		}
	}

	// Now try other players, assuming don't need to enhance religion
	if (!bOnlyBetterThanEnhancingReligion)
	{
		for(int iPlayerLoop = 0; iPlayerLoop < MAX_CIV_PLAYERS; iPlayerLoop++)
		{
			CvPlayer &kLoopPlayer = GET_PLAYER((PlayerTypes)iPlayerLoop);
			if(kLoopPlayer.isAlive() && iPlayerLoop != m_pPlayer->GetID())
			{
#ifdef NQM_AI_GIMP_NO_RELIGION_SPREAD
				if ((GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_RELIGION_SPREAD")) && kLoopPlayer.isHuman())
					continue;
#endif
#ifdef NQ_AI_GIMP_NO_MINOR_SPREAD // NQMP_Bing - AI cannot spread religion to City-States
				if ((GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_MINOR_RELIGION_SPREAD")) && kLoopPlayer.isMinorCiv())
					continue;
#endif
#ifdef AUI_RELIGION_FIX_CHOOSE_PROPHET_CONVERSION_CITY_HONOR_NONCONVERT_PROMISE
				if (m_pPlayer->GetDiplomacyAI()->IsPlayerAgreeNotToConvert((PlayerTypes)iPlayerLoop))
					continue;
#endif
				int iCityLoop;
				for(pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).firstCity(&iCityLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER((PlayerTypes)iPlayerLoop).nextCity(&iCityLoop))
				{
					if (!pLoopCity->GetCityReligions()->IsDefendedAgainstSpread(eReligion))
					{
						ReligionTypes eMajorityReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();
						int iHeretics = pLoopCity->GetCityReligions()->GetFollowersOtherReligions(eReligion);
						int iDistanceToHolyCity = plotDistance(pLoopCity->getX(), pLoopCity->getY(), pHolyCity->getX(), pHolyCity->getY());

						// Score this city
						int iScore = (iHeretics * 50) / (iDistanceToHolyCity + 1);

						//    - high score if this city has another religion as its majority
						if (eMajorityReligion != eReligion)
						{
							iScore *= 3;
						}

						//    - Holy city will anger folks, let's not do that one right away
						ReligionTypes eCityOwnersReligion = GET_PLAYER((PlayerTypes)iPlayerLoop).GetReligions()->GetReligionCreatedByPlayer();
						if (eCityOwnersReligion > RELIGION_PANTHEON && pLoopCity->GetCityReligions()->IsHolyCityForReligion(eCityOwnersReligion))
						{
							iScore /= 2;
						}

						//    - City not owned by religion founder, won't anger folks as much
						const CvReligion* pkMajorityReligion = GC.getGame().GetGameReligions()->GetReligion(eMajorityReligion, NO_PLAYER);
						if (pkMajorityReligion && pkMajorityReligion->m_eFounder != pLoopCity->getOwner())
						{
							iScore *= 2;
						}

						if (iScore > iBestScore)
						{
							iBestScore = iScore;
							pBestCity = pLoopCity;
						}
					}
				}
			}
		}
	}

	return pBestCity;
}

/// Choose a plot next to the target city for the prophet to maneuver to
#ifdef AUI_CONSTIFY
CvPlot* CvReligionAI::ChooseProphetTargetPlot(UnitHandle pUnit, int* piTurns) const
#else
CvPlot* CvReligionAI::ChooseProphetTargetPlot(UnitHandle pUnit, int* piTurns)
#endif
{
	CvCity* pCity = ChooseProphetConversionCity(false/*bOnlyBetterThanEnhancingReligion*/);
	int iBestNumTurns = MAX_INT;
	int iTurns;
	int iBestDistance = MAX_INT;
	int iDistance;
	CvPlot* pBestTarget = NULL;

	if(pCity == NULL)
	{
		return NULL;
	}

	// Our city with no civilian units?  If so go right in there
	if(pCity->getOwner() == m_pPlayer->GetID())
	{
		CvUnit* pFirstUnit = pCity->plot()->getUnitByIndex(0);
		if(!pFirstUnit || pFirstUnit->IsCombatUnit())
		{
			iTurns = TurnsToReachTarget(pUnit, pCity->plot(), true /* bReusePaths */);
			if(iTurns < MAX_INT)
			{
				*piTurns = iTurns;
				return pCity->plot();
			}
		}
	}

#pragma warning ( push )
#pragma warning ( disable : 6011 ) // Dereferencing NULL pointer
	AI_PERF_FORMAT("AI-perf-tact.csv", ("ChooseProphetTargetPlot: %s %d; %s, Turn %03d, %s", pUnit->getName().c_str(), pUnit->GetID(), pCity->getName().c_str(), GC.getGame().getElapsedGameTurns(), m_pPlayer->getCivilizationShortDescription()) );
#pragma warning ( pop )

	// Find adjacent plot with no units (that aren't our own)
	CvPlot* pLoopPlot;
	for(int iI = 0; iI < NUM_DIRECTION_TYPES; iI++)
	{
		pLoopPlot = plotDirection(pCity->getX(), pCity->getY(), ((DirectionTypes)iI));

		if(pLoopPlot != NULL)
		{
			CvUnit* pFirstUnit = pLoopPlot->getUnitByIndex(0);
			if(pFirstUnit && pFirstUnit->getOwner() != m_pPlayer->GetID())
			{
				continue;
			}

			if(pUnit->CanSpreadReligion(pLoopPlot))
			{
				iTurns = TurnsToReachTarget(pUnit, pLoopPlot, true /* bReusePaths */);
				if(iTurns < MAX_INT)
				{
					iDistance = plotDistance(pUnit->getX(), pUnit->getY(), pLoopPlot->getX(), pLoopPlot->getY());

					// Consider it to be twice as far if a water plot (those are dangerous!)
					if(pLoopPlot->isWater())
					{
						iDistance *= 2;
					}

					if(iTurns < iBestNumTurns || (iTurns == iBestNumTurns && iDistance < iBestDistance))
					{
						iBestNumTurns = iTurns;
						iBestDistance = iDistance;
						pBestTarget = pLoopPlot;
					}
				}
			}
		}
	}

	if(piTurns)
		*piTurns = iBestNumTurns;
	return pBestTarget;
}

/// What religion should this AI civ be spreading?
ReligionTypes CvReligionAI::GetReligionToSpread() const
{
	ReligionTypes eRtnValue = NO_RELIGION;

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if(pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(m_pPlayer->GetID());

		int iValue = 0;
		if (LuaSupport::CallAccumulator(pkScriptSystem, "GetReligionToSpread", args.get(), iValue))
		{
			eRtnValue = (ReligionTypes)iValue;
			return eRtnValue;
		}
	}

	CvGameReligions *pReligions = GC.getGame().GetGameReligions();
	eRtnValue = pReligions->GetReligionCreatedByPlayer(m_pPlayer->GetID());
	if(eRtnValue > RELIGION_PANTHEON)
	{
		return eRtnValue;
	}

	return NO_RELIGION;
}

// PRIVATE METHODS

/// Spend faith if already have an enhanced religion
void CvReligionAI::DoFaithPurchases()
{
	ReligionTypes eReligion = GetReligionToSpread();

	CvGameReligions* pReligions = GC.getGame().GetGameReligions();
	const CvReligion* pMyReligion = pReligions->GetReligion(eReligion, m_pPlayer->GetID());
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
	std::vector<BuildingClassTypes> eFaithBuilding = FaithBuildingAvailable(eReligion);
#else
	BuildingClassTypes eFaithBuilding = FaithBuildingAvailable(eReligion);
#endif
	bool bTooManyMissionaries = m_pPlayer->GetNumUnitsWithUnitAI(UNITAI_MISSIONARY) > GC.getRELIGION_MAX_MISSIONARIES();

	CvString strLogMsg;
	if(GC.getLogging())
	{
		strLogMsg = m_pPlayer->getCivilizationShortDescription();
	}

	// Save for another prophet if haven't enhanced religion yet
	if(pMyReligion == NULL || !pMyReligion->m_bEnhanced)
	{
		// Unless all religions gone and we didn't start one
		if(pMyReligion == NULL && pReligions->GetNumReligionsStillToFound() <= 0)
		{
			// Fill our cities with any Faith buildings possible
			if(!BuyAnyAvailableFaithBuilding())
			{
				if(m_pPlayer->GetCurrentEra() >= GC.getInfoTypeForString("ERA_INDUSTRIAL"))
				{
					UnitTypes eGPType = GetDesiredFaithGreatPerson();
					if (eGPType != NO_UNIT)
					{
						BuyGreatPerson(eGPType);

						if(GC.getLogging())
						{
							strLogMsg += ", Saving for Great Person, ";
							CvUnitEntry *pkEntry = GC.getUnitInfo(eGPType);
							if (pkEntry)
							{
								strLogMsg += pkEntry->GetDescription();
							}
						}				
					}
				}
			}
		}
	}
	else
	{
		// Do we need a prophet pronto to reestablish our religion?
		UnitTypes eProphetType = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
		if (eProphetType != NO_UNIT && ChooseProphetConversionCity(true/*bOnlyBetterThanEnhancingReligion*/) && m_pPlayer->GetReligions()->GetNumProphetsSpawned() <= 5)
		{
			BuyGreatPerson(eProphetType);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Prophet, ";
				strLogMsg += GC.getUnitInfo(eProphetType)->GetDescription();
			}				
		}

		// Besides prophets, first priority is to convert all our non-puppet cities
		else if(!bTooManyMissionaries && !AreAllOurCitiesConverted(eReligion, false /*bIncludePuppets*/))
		{
			BuyMissionary(eReligion);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Missionary, Need to Convert Non-Puppet Cities";
			}
		}

		// Next priority is to establish our faith building in every non-puppet city
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
		else if (eFaithBuilding.size() > 0 && !AreAllOurCitiesHaveFaithBuilding(eReligion, false /*bIncludePuppets*/))
		{
			for (std::vector<BuildingClassTypes>::iterator it = eFaithBuilding.begin(); it != eFaithBuilding.end(); ++it)
			{
				BuyFaithBuilding(eReligion, (*it));
			}
#else
		else if(eFaithBuilding != NO_BUILDINGCLASS && !AreAllOurCitiesHaveFaithBuilding(eReligion, false /*bIncludePuppets*/))
		{
			BuyFaithBuilding(eReligion, eFaithBuilding);
#endif

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Faith Building, For Our Non-Puppet Cities";
			}
		}

		// Try to build other buildings with Faith if we took that belief
		else if (CanBuyNonFaithBuilding())
		{
			if (!BuyAnyAvailableNonFaithBuilding())
			{
				if(GC.getLogging())
				{
					strLogMsg += ", Saving for Science Building, Have Reformation Belief";
				}
			}
		}

		// If in Industrial, see if we want to save for buying a great person
		else if (m_pPlayer->GetCurrentEra() >= GC.getInfoTypeForString("ERA_INDUSTRIAL") && GetDesiredFaithGreatPerson() != NO_UNIT)
		{
			UnitTypes eGPType = GetDesiredFaithGreatPerson();
			BuyGreatPerson(eGPType);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Great Person, ";
#ifdef AUI_WARNING_FIXES
				CvUnitEntry* pGPInfo = GC.getUnitInfo(eGPType);
				if (pGPInfo)
					strLogMsg += pGPInfo->GetDescription();
#else
				strLogMsg += GC.getUnitInfo(eGPType)->GetDescription();
#endif
			}				
		}

		// Might as well convert puppet-cities to build our religious strength
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
		else if (!bTooManyMissionaries && eFaithBuilding.size() > 0 && !AreAllOurCitiesHaveFaithBuilding(eReligion, true /*bIncludePuppets*/))
#else
		else if(!bTooManyMissionaries && eFaithBuilding != NO_BUILDINGCLASS && !AreAllOurCitiesHaveFaithBuilding(eReligion, true /*bIncludePuppets*/))
#endif
		{
			BuyMissionary(eReligion);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Missionary, Need to Convert Puppet Cities";
			}
		}

		// Have civs nearby to target who didn't start a religion?
		else if(!bTooManyMissionaries && HaveNearbyConversionTarget(eReligion, false /*bCanIncludeReligionStarter*/))
		{
			BuyMissionary(eReligion);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Missionary, Need to Convert Cities of Non-Religion Starters";
			}
		}

		// Have cities Inquisitors can defend?
		else if(!HaveEnoughInquisitors(eReligion))
		{
			BuyInquisitor(eReligion);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Inquisitors, Need to Defend Our Cities";
			}
		}

		// Have civs nearby to target who did start a religion?
		else if(!bTooManyMissionaries && HaveNearbyConversionTarget(eReligion, true /*bCanIncludeReligionStarter*/))
		{
			BuyMissionary(eReligion);

			if(GC.getLogging())
			{
				strLogMsg += ", Saving for Missionary, Need to Convert Cities of Religion Starters";
			}
		}

		// Any faith buildings from other religions we can buy?
		else
		{
			BuyAnyAvailableFaithBuilding();
		}

		if(GC.getLogging())
		{
			CvString strFaith;
			strFaith.Format(", Faith: %d", m_pPlayer->GetFaith());
			strLogMsg += strFaith;
			GC.getGame().GetGameReligions()->LogReligionMessage(strLogMsg);
		}
	}
}

/// Pick the right city to purchase a missionary in
void CvReligionAI::BuyMissionary(ReligionTypes eReligion)
{
	CvPlayer &kPlayer = GET_PLAYER(m_pPlayer->GetID());
	UnitTypes eMissionary = (UnitTypes)GC.getInfoTypeForString("UNIT_MISSIONARY");
	CvCity *pCapital = kPlayer.getCapitalCity();
	if (pCapital)
	{
		int iCost = pCapital->GetFaithPurchaseCost(eMissionary, true /*bIncludeBeliefDiscounts*/);
		if (iCost <= kPlayer.GetFaith())
		{
			CvCity *pBestCity = CvReligionAIHelpers::GetBestCityFaithUnitPurchase(kPlayer, eMissionary, eReligion);
			if (pBestCity)
			{
				pBestCity->Purchase(eMissionary, (BuildingTypes)-1, (ProjectTypes)-1, YIELD_FAITH);
			}
		}
	}
}

/// Pick the right city to purchase an inquisitor in
void CvReligionAI::BuyInquisitor(ReligionTypes eReligion)
{
	CvPlayer &kPlayer = GET_PLAYER(m_pPlayer->GetID());
	UnitTypes eInquisitor = (UnitTypes)GC.getInfoTypeForString("UNIT_INQUISITOR");
	CvCity *pCapital = kPlayer.getCapitalCity();
	if (pCapital)
	{
		int iCost = pCapital->GetFaithPurchaseCost(eInquisitor, true /*bIncludeBeliefDiscounts*/);
		if (iCost <= kPlayer.GetFaith())
		{
			CvCity *pBestCity = CvReligionAIHelpers::GetBestCityFaithUnitPurchase(kPlayer, eInquisitor, eReligion);
			if (pBestCity)
			{
				pBestCity->Purchase(eInquisitor, (BuildingTypes)-1, (ProjectTypes)-1, YIELD_FAITH);
			}
		}
	}
}

/// Pick the right city to purchase a great person in
void CvReligionAI::BuyGreatPerson(UnitTypes eUnit)
{
	CvPlayer &kPlayer = GET_PLAYER(m_pPlayer->GetID());
	CvCity *pCapital = kPlayer.getCapitalCity();
	if (pCapital)
	{
		int iCost = pCapital->GetFaithPurchaseCost(eUnit, true /*bIncludeBeliefDiscounts*/);
		if (iCost <= kPlayer.GetFaith())
		{
			CvCity *pBestCity = CvReligionAIHelpers::GetBestCityFaithUnitPurchase(kPlayer, eUnit, NO_RELIGION);
			if (pBestCity)
			{
				pBestCity->Purchase(eUnit, (BuildingTypes)-1, (ProjectTypes)-1, YIELD_FAITH);
			}
		}
	}
}

/// Pick the right city to purchase a faith building in
void CvReligionAI::BuyFaithBuilding(ReligionTypes eReligion, BuildingClassTypes eBuildingClass)
{
	CvPlayer &kPlayer = GET_PLAYER(m_pPlayer->GetID());
	BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(eBuildingClass);
	CvCity *pCapital = kPlayer.getCapitalCity();
	if (pCapital)
	{
		int iCost = pCapital->GetFaithPurchaseCost(eBuilding);
		if (iCost <= kPlayer.GetFaith())
		{
			CvCity *pBestCity = CvReligionAIHelpers::GetBestCityFaithBuildingPurchase(kPlayer, eBuilding, eReligion);
			if (pBestCity)
			{
				pBestCity->Purchase((UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH);
			}
		}
	}
}

/// Any building that we can build with Faith (not Faith-generating ones)
bool CvReligionAI::BuyAnyAvailableNonFaithBuilding()
{
	PlayerTypes ePlayer = m_pPlayer->GetID();

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = GET_PLAYER(ePlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iLoop))
	{
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
		{
			BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(iI);
			if(eBuilding != NO_BUILDING)
			{
				CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);

				// Check to make sure this isn't a Faith-generating building
				if(pBuildingEntry && pBuildingEntry->GetYieldChange(YIELD_FAITH) == 0)
				{
					if(pLoopCity->IsCanPurchase(true, true, (UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH))
					{
						pLoopCity->Purchase((UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH);
						return true;
					}
				}
			}
		}
	}
	return false;
}

/// We didn't start a religion but we can still buy Faith buildings of other religions
bool CvReligionAI::BuyAnyAvailableFaithBuilding()
{
	PlayerTypes ePlayer = m_pPlayer->GetID();

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = GET_PLAYER(ePlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iLoop))
	{
		ReligionTypes eReligion = pLoopCity->GetCityReligions()->GetReligiousMajority();
		if(eReligion > RELIGION_PANTHEON)
		{
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
			std::vector<BuildingClassTypes> eBuildingClass = FaithBuildingAvailable(eReligion);
			if (eBuildingClass.size() > 0)
			{
				for (std::vector<BuildingClassTypes>::iterator it = eBuildingClass.begin(); it != eBuildingClass.end(); ++it)
				{
					BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings((*it));
#else
			BuildingClassTypes eBuildingClass = FaithBuildingAvailable(eReligion);
			if(eBuildingClass != NO_BUILDINGCLASS)
			{
				BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(eBuildingClass);
#endif
				if(eBuilding != NO_BUILDING)
				{
					if(pLoopCity->IsCanPurchase(true, true, (UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH))
					{
						pLoopCity->Purchase((UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH);
						return true;
					}
				}
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
				}
#endif
			}
		}
	}
	return false;
}

/// AI's perceived worth of a belief
#if defined(AUI_CONSTIFY) || defined(AUI_RELIGION_SCORE_BELIEF_AT_PLOT_REMADE) || defined(AUI_RELIGION_SCORE_BELIEF_AT_CITY_REMADE) || defined(AUI_RELIGION_USE_DOUBLES)
int CvReligionAI::ScoreBelief(const CvBeliefEntry* pEntry) const
#else
int CvReligionAI::ScoreBelief(CvBeliefEntry* pEntry)
#endif
{
	int iRtnValue = 5;  // Base value since everything has SOME value

	// Loop through each plot on map
#ifdef AUI_WARNING_FIXES
	CvPlot* pPlot;
	for (uint iPlotLoop = 0; iPlotLoop < GC.getMap().numPlots(); iPlotLoop++)
#else
	int iPlotLoop;
	CvPlot* pPlot;
	for(iPlotLoop = 0; iPlotLoop < GC.getMap().numPlots(); iPlotLoop++)
#endif
	{
		pPlot = GC.getMap().plotByIndexUnchecked(iPlotLoop);

		// Skip if not revealed or in enemy territory
		PlayerTypes ePlotOwner = pPlot->getOwner();
		if(pPlot->isRevealed(m_pPlayer->getTeam()) && (ePlotOwner == NO_PLAYER || ePlotOwner == m_pPlayer->GetID()))
		{
			// Also skip if closest city of ours is not within 3
			CvCity* pClosestCity = m_pPlayer->GetClosestFriendlyCity(*pPlot, 3);
			if(pClosestCity)
			{
				// Score it
				int iScoreAtPlot = ScoreBeliefAtPlot(pEntry, pPlot);

				// Apply multiplier based on whether or not being worked, within culture borders, or not
				if(pPlot->isBeingWorked())
				{
					iScoreAtPlot *= GC.getRELIGION_BELIEF_SCORE_WORKED_PLOT_MULTIPLIER();
				}
				else if(ePlotOwner != NO_PLAYER)
				{
					iScoreAtPlot *= GC.getRELIGION_BELIEF_SCORE_OWNED_PLOT_MULTIPLIER();
				}
				else
				{
					iScoreAtPlot *= GC.getRELIGION_BELIEF_SCORE_UNOWNED_PLOT_MULTIPLIER();
				}

				iRtnValue += iScoreAtPlot;
			}
		}
	}

	// Add in value at city level
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
		int iScoreAtCity = ScoreBeliefAtCity(pEntry, pLoopCity);
		iScoreAtCity *= GC.getRELIGION_BELIEF_SCORE_CITY_MULTIPLIER();

		iRtnValue += iScoreAtCity;
	}

	// Add in player-level value
	iRtnValue += ScoreBeliefForPlayer(pEntry);

	// Divide by 2 if a Pantheon belief (to deemphasize these to Byzantine bonus belief)
	if (pEntry->IsPantheonBelief())
	{
		iRtnValue /= 2;
	}

	return iRtnValue;
}

/// AI's evaluation of this belief's usefulness at this one plot
#if defined(AUI_CONSTIFY)
int CvReligionAI::ScoreBeliefAtPlot(const CvBeliefEntry* pEntry, const CvPlot* pPlot) const
#else
int CvReligionAI::ScoreBeliefAtPlot(CvBeliefEntry* pEntry, CvPlot* pPlot)
#endif
{
	int iRtnValue = 0;

	for(int iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		// Terrain
		TerrainTypes eTerrain = pPlot->getTerrainType();
		if(eTerrain != NO_TERRAIN)
		{
			iRtnValue += pEntry->GetTerrainYieldChange(eTerrain, iI);
		}

		// Feature
		FeatureTypes eFeature = pPlot->getFeatureType();
		if(eFeature != NO_FEATURE)
		{
			iRtnValue += pEntry->GetFeatureYieldChange(eFeature, iI);

			if(pPlot->IsNaturalWonder())
			{
				iRtnValue += pEntry->GetYieldChangeNaturalWonder(iI);
				iRtnValue += (pEntry->GetYieldModifierNaturalWonder(iI) / 25);
			}
		}

		// Resource
		ResourceTypes eResource = pPlot->getResourceType();
		if(eResource != NO_RESOURCE)
		{
			iRtnValue += pEntry->GetResourceYieldChange(eResource, iI);

			// Improvement
#ifdef AUI_WARNING_FIXES
			for (uint jJ = 0; jJ < GC.getNumImprovementInfos(); jJ++)
#else
			int iNumImprovementInfos = GC.getNumImprovementInfos();
			for(int jJ = 0; jJ < iNumImprovementInfos; jJ++)
#endif
			{
				if(pPlot->canHaveImprovement((ImprovementTypes)jJ, m_pPlayer->getTeam()))
				{
					iRtnValue += (pEntry->GetImprovementYieldChange((ImprovementTypes)jJ, (YieldTypes)iI) * 2);
				}
			}
		}
	}

	return iRtnValue;
}

/// AI's evaluation of this belief's usefulness at this one plot
#if defined(AUI_CONSTIFY)
int CvReligionAI::ScoreBeliefAtCity(const CvBeliefEntry* pEntry, const CvCity* pCity) const
#else
int CvReligionAI::ScoreBeliefAtCity(CvBeliefEntry* pEntry, CvCity* pCity)
#endif
{
	int iRtnValue = 0;
	int iTempValue;
	int iMinPop;
	int iMinFollowers;
	int iHappinessMultiplier = 2;

	CvFlavorManager* pFlavorManager = m_pPlayer->GetFlavorManager();
	int iFlavorOffense = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_OFFENSE"));
	int iFlavorDefense = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_DEFENSE"));
	int iFlavorCityDefense = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_CITY_DEFENSE"));
	int iFlavorHappiness = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_HAPPINESS"));

	int iHappinessNeedFactor = iFlavorOffense * 2 + iFlavorHappiness - iFlavorDefense;
	if (iHappinessNeedFactor > 15)
	{
		iHappinessMultiplier = 3;
	}
	else if (iHappinessNeedFactor < 6)
	{
		iHappinessMultiplier = 1;
	}

	iMinPop = pEntry->GetMinPopulation();
	iMinFollowers = pEntry->GetMinFollowers();

	// Simple ones
	iRtnValue += pEntry->GetCityGrowthModifier() / 3;
	if(pEntry->RequiresPeace())
	{
		iRtnValue /= 2;
	}
	iRtnValue += (-pEntry->GetPlotCultureCostModifier() * 2) / 10;
	iRtnValue += (pEntry->GetCityRangeStrikeModifier() / 10) * MAX(iFlavorDefense,iFlavorCityDefense);
	iRtnValue += pEntry->GetFriendlyHealChange() / 10;

	// Wonder production multiplier
	if(pEntry->GetObsoleteEra() > 0)
	{	
		if (pEntry->GetObsoleteEra() > GC.getGame().getCurrentEra())
		{
			iRtnValue += (pEntry->GetWonderProductionModifier() * pEntry->GetObsoleteEra()) / 10;
		}
	}
	else
	{
		iRtnValue += pEntry->GetWonderProductionModifier() / 3;
	}

	// River happiness
	iTempValue = pEntry->GetRiverHappiness() * iHappinessMultiplier;
	if(iMinPop > 0)
	{
		if(pCity->getPopulation() >= iMinPop)
		{
			iTempValue *= 2;
		}
	}
	iRtnValue += iTempValue;

	// Happiness per city
	iTempValue = pEntry->GetHappinessPerCity() * iHappinessMultiplier;
	if(iMinPop > 0)
	{
		if(pCity->getPopulation() >= iMinPop)
		{
			iTempValue *= 2;
		}
	}
	iRtnValue += iTempValue;

	// Building class happiness
#ifdef AUI_WARNING_FIXES
	for (uint jJ = 0; jJ < GC.getNumBuildingClassInfos(); jJ++)
#else
	for(int jJ = 0; jJ < GC.getNumBuildingClassInfos(); jJ++)
#endif
	{
		iTempValue = pEntry->GetBuildingClassHappiness(jJ) * iHappinessMultiplier;
		if(iMinFollowers > 0)
		{
			if(pCity->getPopulation() >= iMinFollowers)
			{
				iTempValue *= 2;
			}
		}
		iRtnValue += iTempValue;
	}

	for(int iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		// City yield change
		iTempValue = pEntry->GetCityYieldChange(iI);
		if(iMinPop > 0)
		{
			if(pCity->getPopulation() >= iMinPop)
			{
				iTempValue *= 2;
			}
		}
		iRtnValue += iTempValue;

		// Trade route yield change
		iTempValue = pEntry->GetYieldChangeTradeRoute(iI);
		if(iMinPop > 0)
		{
			if(pCity->getPopulation() >= iMinPop)
			{
				iTempValue *= 2;
			}
		}
		if(pCity->IsRouteToCapitalConnected())
		{
			iTempValue *= 2;
		}
		iRtnValue += iTempValue;

		// Specialist yield change
		iTempValue = pEntry->GetYieldChangeAnySpecialist(iI);
		if(pCity->getPopulation() >= 8)  // Like it more with large cities
		{
			iTempValue *= 3;
		}
		iRtnValue += iTempValue;

		// Building class yield change
#ifdef AUI_WARNING_FIXES
		for (uint jJ = 0; jJ < GC.getNumBuildingClassInfos(); jJ++)
#else
		for(int jJ = 0; jJ < GC.getNumBuildingClassInfos(); jJ++)
#endif
		{
			CvBuildingClassInfo* pkBuildingClassInfo = GC.getBuildingClassInfo((BuildingClassTypes)jJ);
			if(!pkBuildingClassInfo)
			{
				continue;
			}

			iTempValue = pEntry->GetBuildingClassYieldChange(jJ, iI);
			if(iMinFollowers > 0)
			{
				if(pCity->getPopulation() < iMinFollowers)
				{
					iTempValue /= 2;
				}
			}

			BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(jJ);
			if (eBuilding != NO_BUILDING)
			{
				if(pCity->GetCityBuildings()->GetNumBuilding(eBuilding) > 0)
				{
					iTempValue *= 2;
				}
			}

			if(pkBuildingClassInfo->getMaxPlayerInstances() == 1)
			{
				iTempValue /= 2;
			}

			iRtnValue += iTempValue;
		}

		// World wonder change
		iRtnValue += pEntry->GetYieldChangeWorldWonder(iI) * 3 / 2;

		// Production per follower
		if (pEntry->GetMaxYieldModifierPerFollower(iI) > 0)
		{
			iTempValue = min(pEntry->GetMaxYieldModifierPerFollower(iI), pCity->getPopulation());
			iTempValue /= 2;
			iRtnValue += iTempValue;
		}
	}

	return iRtnValue;
}

/// AI's evaluation of this belief's usefulness to this player
#if defined(AUI_CONSTIFY)
int CvReligionAI::ScoreBeliefForPlayer(const CvBeliefEntry* pEntry) const
#else
int CvReligionAI::ScoreBeliefForPlayer(CvBeliefEntry* pEntry)
#endif
{
	int iRtnValue = 0;
	CvFlavorManager* pFlavorManager = m_pPlayer->GetFlavorManager();
	CvGameReligions* pGameReligions = GC.getGame().GetGameReligions();

	//--------------------
	// GET BACKGROUND DATA
	//--------------------
	int iFlavorOffense = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_OFFENSE"));
	int iFlavorDefense = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_DEFENSE"));
	int iFlavorHappiness = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_HAPPINESS"));
	int iFlavorCulture = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_CULTURE"));
	int iFlavorGold = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_GOLD"));
	int iFlavorGP = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_GREAT_PEOPLE"));
	int iFlavorScience = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_SCIENCE"));
	int iFlavorDiplomacy = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_DIPLOMACY"));
	int iFlavorExpansion = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)GC.getInfoTypeForString("FLAVOR_EXPANSION"));

	int iNumEnhancedReligions = pGameReligions->GetNumReligionsEnhanced();
	int iReligionsEnhancedPercent = (100 * iNumEnhancedReligions) / GC.getMap().getWorldInfo().getMaxActiveReligions();

	//------------------------------
	// PLAYER-LEVEL PANTHEON BELIEFS
	//------------------------------
	if(pEntry->GetFaithFromKills() > 0)
	{
		int iTemp = pEntry->GetFaithFromKills() * pEntry->GetMaxDistance() * iFlavorOffense / 100;
		if (m_pPlayer->GetDiplomacyAI()->IsGoingForWorldConquest())
		{
			iTemp *= 2;
		}
		iRtnValue += iTemp;
	}

	//-------
	// UNUSED
	//-------
	iRtnValue += pEntry->GetPlayerHappiness() * iFlavorHappiness;
	iRtnValue += pEntry->GetPlayerCultureModifier() * iFlavorCulture;

	//-----------------
	// FOLLOWER BELIEFS
	//-----------------
	// Unlocks a building
#ifdef AUI_WARNING_FIXES
	for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
	{
		if (pEntry->IsBuildingClassEnabled(iI))
		{
			BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(iI);
			CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);

			if(pBuildingEntry && pFlavorManager)
			{
				for(int iFlavorLoop = 0; iFlavorLoop < GC.getNumFlavorTypes(); iFlavorLoop++)
				{
					int iFlavorValue = pFlavorManager->GetPersonalityIndividualFlavor((FlavorTypes)iFlavorLoop) * pBuildingEntry->GetFlavorValue(iFlavorLoop);

					// If can also be built with hammers, much less valuable
					if (pBuildingEntry->GetProductionCost() > 0)
					{
						iFlavorValue /= 10;
					}
					iRtnValue += iFlavorValue;
				}
			}
		}
	}

	// Unlocks units?	
#ifdef AUI_WARNING_FIXES
	for (uint i = 0; i < GC.getNumEraInfos(); i++)
#else
	for(int i = 0; i < GC.getNumEraInfos(); i++)
#endif
	{
		// Add in for each era enabled
		if (pEntry->IsFaithUnitPurchaseEra(i))
		{
			int iTemp = (iFlavorOffense) + (iFlavorDefense / 2);
			if (m_pPlayer->GetDiplomacyAI()->IsGoingForWorldConquest())
			{
				iTemp *= 2;
			}
			iRtnValue += iTemp;
		}
	}

	//----------------
	// FOUNDER BELIEFS
	//----------------
	iRtnValue += (int)(pEntry->GetHappinessPerFollowingCity() * (float)(iFlavorHappiness + 2 * iFlavorOffense - iFlavorDefense));
	if (pEntry->GetHappinessPerXPeacefulForeignFollowers() > 0 && iFlavorDefense > 0)
	{
		iRtnValue += (iFlavorHappiness * 10 * iFlavorDefense) / (pEntry->GetHappinessPerXPeacefulForeignFollowers() * iFlavorDefense);
	}
	iRtnValue += iFlavorScience * pEntry->GetSciencePerOtherReligionFollower() / 5;
	iRtnValue += pEntry->GetGoldPerFollowingCity() * iFlavorGold;
	if(pEntry->GetGoldPerXFollowers() > 0)
	{
		iRtnValue += (iFlavorGold * 4 / pEntry->GetGoldPerXFollowers());
	}
	iRtnValue += iFlavorGold * pEntry->GetGoldWhenCityAdopts() / 50;

	// Minimum influence with city states
	iRtnValue += iFlavorDiplomacy * pEntry->GetCityStateMinimumInfluence() / 7;

	// Yields for foreign followers
	for(int iI = 0; iI < NUM_YIELD_TYPES; iI++)
	{
		if (iI == YIELD_CULTURE)
		{
			iRtnValue += pEntry->GetYieldChangePerForeignCity(iI) * 2 * iFlavorCulture;
			if (pEntry->GetYieldChangePerXForeignFollowers(iI) > 0)
			{
				iRtnValue += 10 * iFlavorCulture / pEntry->GetYieldChangePerXForeignFollowers(iI);
			}
		}
		else
		{
			iRtnValue += pEntry->GetYieldChangePerForeignCity(iI) * 10;
			if (pEntry->GetYieldChangePerXForeignFollowers(iI) > 0)
			{
				iRtnValue += 50 / pEntry->GetYieldChangePerXForeignFollowers(iI);
			}
		}
	}

#ifdef NQ_FREE_SETTLERS_FROM_BELIEF
	// extra value for expansionist civs
	iRtnValue += pEntry->GetNumFreeSettlers() * iFlavorExpansion;
#endif

	//-----------------
	// ENHANCER BELIEFS
	//-----------------
	iRtnValue += iFlavorGP * pEntry->GetGreatPersonExpendedFaith() / 10;
	iRtnValue += iFlavorDiplomacy * pEntry->GetFriendlyCityStateSpreadModifier() / 20;
	iRtnValue += iFlavorDefense * pEntry->GetCombatModifierFriendlyCities() / 4;
	iRtnValue += iFlavorOffense * pEntry->GetCombatModifierEnemyCities() / 4;
#ifdef NQ_DEUS_VULT
	if (pEntry->DeusVult())
	{
		iRtnValue += (iFlavorOffense + iFlavorDefense) * 3;
	}
#endif
#ifdef NQ_GOLDEN_AGE_TURNS_FROM_BELIEF
	iRtnValue += pEntry->GetGoldenAgeTurns() * (iFlavorGold + iFlavorCulture) / 5;
#endif

	// Chosen EARLY?
	if (iReligionsEnhancedPercent < 33)
	{
		// Good early
		iRtnValue += (2 * iFlavorExpansion) + pEntry->GetSpreadDistanceModifier();
		iRtnValue += (2 * iFlavorScience) + pEntry->GetSpreadStrengthModifier();
		iRtnValue += pEntry->GetMissionaryStrengthModifier() * 2;
		iRtnValue += (-1 * pEntry->GetMissionaryCostModifier()) * 2;

		// Poor early
		iRtnValue += pEntry->GetProphetStrengthModifier() / 4;
		iRtnValue += (-1 * pEntry->GetProphetCostModifier()) / 4;
	}

	// Chosen in the MIDDLE?
	else if (iReligionsEnhancedPercent < 66)
	{
		iRtnValue += iFlavorExpansion + pEntry->GetSpreadDistanceModifier() / 2;
		iRtnValue += iFlavorScience + pEntry->GetSpreadStrengthModifier() / 2;
		iRtnValue += pEntry->GetProphetStrengthModifier() / 2;
		iRtnValue += (-1 * pEntry->GetProphetCostModifier()) / 2;
		iRtnValue += pEntry->GetMissionaryStrengthModifier();
		iRtnValue += (-1 * pEntry->GetMissionaryCostModifier());
	}

	// Chosen LATE?
	else
	{
		// Poor late
		iRtnValue += ((2 * iFlavorExpansion) + pEntry->GetSpreadDistanceModifier()) / 4;
		iRtnValue += ((2 * iFlavorScience) + pEntry->GetSpreadStrengthModifier()) / 4;
		iRtnValue += pEntry->GetMissionaryStrengthModifier() / 2;
		iRtnValue += (-1 * pEntry->GetMissionaryCostModifier()) / 2;

		// Good late
		iRtnValue += pEntry->GetProphetStrengthModifier();
		iRtnValue += (-1 * pEntry->GetProphetCostModifier());
	}

	//-----------------
	// REFORMATION BELIEFS
	//-----------------
	if (pEntry->FaithPurchaseAllGreatPeople())
	{
		// Count number of GP branches we have still to open and score based on that
		int iTemp = 0;
		PolicyBranchTypes eBranch;
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_TRADITION", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_HONOR", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_AESTHETICS", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_COMMERCE", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_EXPLORATION", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}
		eBranch = (PolicyBranchTypes)GC.getInfoTypeForString("POLICY_BRANCH_RATIONALISM", true /*bHideAssert*/);
		if (eBranch != NO_POLICY_BRANCH_TYPE && (!m_pPlayer->GetPlayerPolicies()->IsPolicyBranchFinished(eBranch) || m_pPlayer->GetPlayerPolicies()->IsPolicyBranchBlocked(eBranch)))
		{
			iTemp++;
		}

		iRtnValue += (iTemp * 6);
	}
	if (pEntry->ConvertsBarbarians())
	{
		MilitaryAIStrategyTypes eStrategyBarbs = (MilitaryAIStrategyTypes) GC.getInfoTypeForString("MILITARYAISTRATEGY_ERADICATE_BARBARIANS");
		if (m_pPlayer->GetMilitaryAI()->IsUsingStrategy(eStrategyBarbs))
		{
			iRtnValue += 40;
		}
		else
		{
			iRtnValue += 15;
		}
	}
	if (pEntry->GetCityStateInfluenceModifier() > 0)
	{
		iRtnValue += (pEntry->GetCityStateInfluenceModifier() * iFlavorDiplomacy / 5);
	}
	if (pEntry->GetInquisitorPressureRetention() > 0)
	{
		iRtnValue += (pEntry->GetInquisitorPressureRetention() / 2);
	}
	if (pEntry->GetSpyPressure() > 0)
	{
		iRtnValue += (pEntry->GetSpyPressure()* 5);
	}
	if (pEntry->GetFaithBuildingTourism() > 0)
	{
		iRtnValue += (pEntry->GetFaithBuildingTourism() * 20);
	}

	return iRtnValue;
}

/// AI's evaluation of this city as a target for a missionary
#ifdef AUI_CONSTIFY
int CvReligionAI::ScoreCityForMissionary(const CvCity* pCity, UnitHandle pUnit) const
#else
int CvReligionAI::ScoreCityForMissionary(CvCity* pCity, UnitHandle pUnit)
#endif
{
	int iScore = 0;

	ReligionTypes eMyReligion = GetReligionToSpread();

	// Skip if not revealed
	if(!pCity->plot()->isRevealed(m_pPlayer->getTeam()))
	{
		return iScore;
	}

	// Skip if already our religion
	if(pCity->GetCityReligions()->GetReligiousMajority() == eMyReligion)
	{
		return iScore;
	}

	// Base score based on if we are establishing majority
	iScore = 100;
	if(ShouldBecomeNewMajority(pCity, eMyReligion, pUnit->GetReligionData()->GetReligiousStrength() * GC.getRELIGION_MISSIONARY_PRESSURE_MULTIPLIER()))
	{
		iScore *= 2;
	}

	if (!GET_PLAYER(pCity->getOwner()).isMinorCiv())
	{
#ifdef NQM_AI_GIMP_NO_RELIGION_SPREAD
		if ((GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_RELIGION_SPREAD")) && GET_PLAYER(pCity->getOwner()).isHuman())
			return 0;
#endif
		if (m_pPlayer->GetDiplomacyAI()->IsPlayerAgreeNotToConvert(pCity->getOwner()))
		{
			return 0;
		}
	}
#ifdef NQ_AI_GIMP_NO_MINOR_SPREAD //EAP: From Bing with love - AI cannot spread religion to City-States
	else
	{
		if (GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_MINOR_RELIGION_SPREAD"))
		{
			return 0;
		}
	}
#endif

	CvPlayer& kCityPlayer = GET_PLAYER(pCity->getOwner());
	// Much better score if our own city or if this city owner isn't starting a religion
	if(pCity->getOwner() == m_pPlayer->GetID())
	{
		iScore *= 5;
	}
	else if(!kCityPlayer.GetReligions()->HasCreatedReligion())
	{
		iScore *= 3;
	}

	// Then subtract distance
	iScore -= plotDistance(pUnit->getX(), pUnit->getY(), pCity->getX(), pCity->getY());

	// Multiplier by how safe it is
	if(!atWar(m_pPlayer->getTeam(), kCityPlayer.getTeam()))
	{
		iScore *= 2;
	}

	// Holy city will anger folks, let's not do that one right away
	ReligionTypes eCityOwnersReligion = kCityPlayer.GetReligions()->GetReligionCreatedByPlayer();
	if (eCityOwnersReligion > RELIGION_PANTHEON && pCity->GetCityReligions()->IsHolyCityForReligion(eCityOwnersReligion))
	{
		iScore /= 2;
	}

	return iScore;
}

/// AI's evaluation of this city as a target for an inquisitor
#ifdef AUI_CONSTIFY
int CvReligionAI::ScoreCityForInquisitor(const CvCity* pCity, UnitHandle pUnit) const
#else
int CvReligionAI::ScoreCityForInquisitor(CvCity* pCity, UnitHandle pUnit)
#endif
{
	int iScore = 0;
	ReligionTypes eMyReligion = GetReligionToSpread();

	// Skip if already our religion
	if(pCity->GetCityReligions()->GetReligiousMajority() == eMyReligion)
	{
		return iScore;
	}

	// Base score based on if we are establishing majority
	iScore = 100;

	// Then subtract distance
	iScore -= plotDistance(pUnit->getX(), pUnit->getY(), pCity->getX(), pCity->getY());

	return iScore;
}

/// Is this enough new pressure to establish this as the new religious majority?
#ifdef AUI_CONSTIFY
bool CvReligionAI::ShouldBecomeNewMajority(const CvCity* pCity, ReligionTypes eReligion, int iNewPressure) const
#else
bool CvReligionAI::ShouldBecomeNewMajority(CvCity* pCity, ReligionTypes eReligion, int iNewPressure) const
#endif
{
	CvCityReligions* pCityReligions = pCity->GetCityReligions();
	int iTotalPressure = pCityReligions->GetTotalPressure() + iNewPressure;
	int iReligionPressure = pCityReligions->GetPressure(eReligion) + iNewPressure;

	return (iReligionPressure * 2) > iTotalPressure;
}

/// Are all of our own cities our religion?
bool CvReligionAI::AreAllOurCitiesConverted(ReligionTypes eReligion, bool bIncludePuppets) const
{
	bool bRtnValue = true;

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pLoopCity->GetCityReligions()->GetReligiousMajority() != eReligion)
		{
			if(bIncludePuppets || !pLoopCity->IsPuppet())
			{
				bRtnValue = false;
				break;
			}
		}
	}

	return bRtnValue;
}

/// Do all of our own cities have this religion's faith building if possible?
bool CvReligionAI::AreAllOurCitiesHaveFaithBuilding(ReligionTypes eReligion, bool bIncludePuppets) const
{
	bool bRtnValue = true;
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
	std::vector<BuildingClassTypes> eFaithBuildingClass = FaithBuildingAvailable(eReligion);
	if (eFaithBuildingClass.size() == 0)
	{
		return true;
	}
	for (std::vector<BuildingClassTypes>::iterator it = eFaithBuildingClass.begin(); it != eFaithBuildingClass.end() && bRtnValue; ++it)
	{
		BuildingTypes eFaithBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings((*it));

		if (eFaithBuilding == NO_BUILDING)
		{
			continue;
		}
#else
	BuildingClassTypes eFaithBuildingClass = FaithBuildingAvailable(eReligion);
	BuildingTypes eFaithBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(eFaithBuildingClass);

	if(eFaithBuildingClass == NO_BUILDINGCLASS || eFaithBuilding == NO_BUILDING)
	{
		return true;
	}
#endif

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = m_pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = m_pPlayer->nextCity(&iLoop))
	{
		if(pLoopCity->GetCityReligions()->GetReligiousMajority() == eReligion)
		{
			if(bIncludePuppets || !pLoopCity->IsPuppet())
			{
				if(pLoopCity->GetCityBuildings()->GetNumBuilding(eFaithBuilding) < 1)
				{
					bRtnValue = false;
					break;
				}
			}
		}
	}
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
	}
#endif

	return bRtnValue;
}

// Is there a civ nearby that isn't pressing religion?
bool CvReligionAI::HaveNearbyConversionTarget(ReligionTypes eReligion, bool bCanIncludeReligionStarter) const
{
	int iLoop;
	bool bStartedOwnReligion;
	TeamTypes eTeam = m_pPlayer->getTeam();
	UnitTypes eMissionary = (UnitTypes)GC.getInfoTypeForString("UNIT_MISSIONARY");
#ifdef AUI_WARNING_FIXES
	CvUnitEntry* pMissionaryInfo = GC.getUnitInfo(eMissionary);
	if (!pMissionaryInfo)
	{
		return false;
	}
	int iMissionaryMoves = pMissionaryInfo->GetMoves();
#else
	int iMissionaryMoves = GC.getUnitInfo(eMissionary)->GetMoves();
#endif

	CvCity* pCapital = m_pPlayer->getCapitalCity();
	if(pCapital == NULL)
	{
		return false;
	}

	for(int iPlayer = 0; iPlayer < MAX_PLAYERS; iPlayer++)
	{
		PlayerTypes ePlayer = (PlayerTypes)iPlayer;

		// Must be someone else
		if(ePlayer == m_pPlayer->GetID())
		{
			continue;

		}
		CvPlayer& kPlayer = GET_PLAYER(ePlayer);
		if(kPlayer.isAlive())
		{
			if(kPlayer.isMinorCiv())
			{
#ifdef NQ_AI_GIMP_NO_MINOR_SPREAD //EAP:  NQMP_Bing - AI cannot spread religion to City-States
				if (GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_MINOR_RELIGION_SPREAD"))
					continue;
#endif
				bStartedOwnReligion = false;
			}
			else
			{
				if (!kPlayer.isBarbarian())
				{
#ifdef NQM_AI_GIMP_NO_RELIGION_SPREAD
					if ((GC.getGame().isOption("GAMEOPTION_AI_TWEAKS") || GC.getGame().isOption("GAMEOPTION_AI_GIMP_NO_RELIGION_SPREAD")) && kPlayer.isHuman())
						continue;
#endif
					if (m_pPlayer->GetDiplomacyAI()->IsPlayerAgreeNotToConvert(ePlayer))
					{
						continue;
					}
				}

				if(kPlayer.GetReligionAI()->GetReligionToSpread() > RELIGION_PANTHEON)
				{
					bStartedOwnReligion = true;
				}
				else
				{
					bStartedOwnReligion = false;
				}
			}

			if(!bStartedOwnReligion || bCanIncludeReligionStarter)
			{
				for(CvCity* pCity = kPlayer.firstCity(&iLoop); pCity != NULL; pCity = kPlayer.nextCity(&iLoop))
				{
					// Revealed and not currently our religion
					if(pCity->isRevealed(eTeam, false) && pCity->GetCityReligions()->GetReligiousMajority() != eReligion)
					{
						// Within 10 times Missionary movement allowance of our
						if(plotDistance(pCapital->getX(), pCapital->getY(), pCity->getX(), pCity->getY()) <= (iMissionaryMoves * GC.getRELIGION_MISSIONARY_RANGE_IN_TURNS()))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

// Do we have as many Inquisitors as we need
bool CvReligionAI::HaveEnoughInquisitors(ReligionTypes eReligion) const
{
	int iLoop;

	// Need one for every city in our realm that is of another religion, plus one for defense
	int iNumNeeded = 1;
	for(CvCity* pCity = m_pPlayer->firstCity(&iLoop); pCity != NULL; pCity = m_pPlayer->nextCity(&iLoop))
	{
		ReligionTypes eCityReligion = pCity->GetCityReligions()->GetReligiousMajority();
		if(eCityReligion > RELIGION_PANTHEON && eCityReligion != eReligion)
		{
			iNumNeeded++;
		}
	}

	// Count Inquisitors of our religion
	int iNumInquisitors = 0;
	for(CvUnit* pUnit = m_pPlayer->firstUnit(&iLoop); pUnit != NULL; pUnit = m_pPlayer->nextUnit(&iLoop))
	{
		if(pUnit->getUnitInfo().IsRemoveHeresy())
		{
			if(pUnit->GetReligionData()->GetReligion() == eReligion)
			{
				iNumInquisitors++;
			}
		}
	}

	return iNumInquisitors >= iNumNeeded;
}

/// Do we have a belief that allows a faith generating building to be constructed?
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
std::vector<BuildingClassTypes> CvReligionAI::FaithBuildingAvailable(ReligionTypes eReligion) const
#else
BuildingClassTypes CvReligionAI::FaithBuildingAvailable(ReligionTypes eReligion) const
#endif
{
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
	std::vector<BuildingClassTypes> aeFaithBuildings;
#endif
	CvGameReligions* pReligions = GC.getGame().GetGameReligions();
	const CvReligion* pMyReligion = pReligions->GetReligion(eReligion, m_pPlayer->GetID());

	if (pMyReligion)
	{
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
		{
			if (pMyReligion->m_Beliefs.IsBuildingClassEnabled((BuildingClassTypes)iI))
			{
				BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings((BuildingClassTypes)iI);
				if(eBuilding != NO_BUILDING)
				{
					CvBuildingEntry* pBuildingEntry = GC.getBuildingInfo(eBuilding);
					if (pBuildingEntry && pBuildingEntry->GetYieldChange(YIELD_FAITH) > 0)
					{
#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
						aeFaithBuildings.push_back((BuildingClassTypes)iI);
#else
						return (BuildingClassTypes)iI;
#endif
					}
				}
			}
		}
	}

#ifdef AUI_RELIGION_FIX_MULTIPLE_FAITH_BUILDINGS
	return aeFaithBuildings;
#else
	return NO_BUILDINGCLASS;
#endif
}

/// Can we buy a non-Faith generating building?
bool CvReligionAI::CanBuyNonFaithBuilding() const
{
	PlayerTypes ePlayer = m_pPlayer->GetID();

	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = GET_PLAYER(ePlayer).firstCity(&iLoop); pLoopCity != NULL; pLoopCity = GET_PLAYER(ePlayer).nextCity(&iLoop))
	{
#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
#endif
		{
			BuildingTypes eBuilding = (BuildingTypes)m_pPlayer->getCivilizationInfo().getCivilizationBuildings(iI);
			if(eBuilding != NO_BUILDING)
			{
				CvBuildingEntry* pBuildingEntry = GC.GetGameBuildings()->GetEntry(eBuilding);

				// Check to make sure this isn't a Faith-generating building
				if(pBuildingEntry && pBuildingEntry->GetYieldChange(YIELD_FAITH) == 0)
				{
					if(pLoopCity->IsCanPurchase(true, true, (UnitTypes)-1, eBuilding, (ProjectTypes)-1, YIELD_FAITH))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

/// Which Great Person should we buy with Faith?
UnitTypes CvReligionAI::GetDesiredFaithGreatPerson() const
{
	SpecialUnitTypes eSpecialUnitGreatPerson = (SpecialUnitTypes) GC.getInfoTypeForString("SPECIALUNIT_PEOPLE");
	UnitTypes eRtnValue = NO_UNIT;
	int iBestScore = 0;
	ReligionTypes eReligion = GetReligionToSpread();

	// Loop through all Units and see if they're possible
#ifdef AUI_WARNING_FIXES
	for (uint iUnitLoop = 0; iUnitLoop < GC.getNumUnitInfos(); iUnitLoop++)
#else
	for(int iUnitLoop = 0; iUnitLoop < GC.getNumUnitInfos(); iUnitLoop++)
#endif
	{
		const UnitTypes eUnit = static_cast<UnitTypes>(iUnitLoop);
		CvUnitEntry* pkUnitInfo = GC.getUnitInfo(eUnit);

		if(pkUnitInfo == NULL)
			continue;

		UnitClassTypes eUnitClass = (UnitClassTypes)pkUnitInfo->GetUnitClassType();

		// Can't be able to train it
		if(pkUnitInfo->GetProductionCost() != -1)
		{
			continue;
		}

		// Must be a Great Person (defined in SpecialUnitType in Unit XML)
		if(pkUnitInfo->GetSpecialUnitType() != eSpecialUnitGreatPerson)
		{
			continue;
		}

		// Must be a Great Person for this player's civ
		if(!m_pPlayer->canTrain(eUnit, false /*bContinue*/, false /*bTestVisible*/, true /*bIgnoreCost*/))
		{
			continue;
		}

		// Can we purchase this one in the capital?
		CvCity *pCapital = m_pPlayer->getCapitalCity();
		if (pCapital)
		{
			if (pCapital->IsCanPurchase(false/*bTestPurchaseCost*/, false/*bTestTrainable*/, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
			{
				int iScore = 0;
				AIGrandStrategyTypes eVictoryStrategy = m_pPlayer->GetGrandStrategyAI()->GetActiveGrandStrategy();

				// Score it
				if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_PROPHET"))
				{
					if (GetReligionToSpread() > RELIGION_PANTHEON)
					{
						if (ChooseProphetConversionCity(true/*bOnlyBetterThanEnhancingReligion*/))
						{
							iScore = 1500;
						}
						else if (ChooseProphetConversionCity(false/*bOnlyBetterThanEnhancingReligion*/))
						{
							iScore = 750;
						}
						iScore /= (1+ m_pPlayer->GetReligions()->GetNumProphetsSpawned());
					}
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_WRITER"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CULTURE"))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getWritersFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_ARTIST"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CULTURE"))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getArtistsFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_MUSICIAN"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CULTURE"))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getMusiciansFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_SCIENTIST"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_SPACESHIP"))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
#ifndef AUI_WARNING_FIXES
					iScore = 1000;
#endif
					iScore /= (1+ m_pPlayer->getScientistsFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_MERCHANT"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_UNITED_NATIONS"))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getMerchantsFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_ENGINEER"))
				{
					EconomicAIStrategyTypes eStrategy = (EconomicAIStrategyTypes) GC.getInfoTypeForString("ECONOMICAISTRATEGY_GS_SPACESHIP_HOMESTRETCH");
					if (eStrategy != NO_ECONOMICAISTRATEGY && m_pPlayer->GetEconomicAI()->IsUsingStrategy(eStrategy))
					{
						iScore = 2000;
					}
					else
					{
						iScore = 500;
					}
					iScore /= (1+ m_pPlayer->getEngineersFromFaith());
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_GREAT_GENERAL"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CONQUEST")&& !(GC.getMap().GetAIMapHint() & 1))
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getGeneralsFromFaith() + m_pPlayer->GetNumUnitsWithUnitAI(UNITAI_GENERAL));
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_GREAT_ADMIRAL"))
				{
					if (eVictoryStrategy == (AIGrandStrategyTypes) GC.getInfoTypeForString("AIGRANDSTRATEGY_CONQUEST") && GC.getMap().GetAIMapHint() & 1)
					{
						iScore = 1000;
					}
					else
					{
						iScore = 400;
					}
					iScore /= (1+ m_pPlayer->getAdmiralsFromFaith() + m_pPlayer->GetNumUnitsWithUnitAI(UNITAI_ADMIRAL));
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_MISSIONARY"))
				{
					if (HaveNearbyConversionTarget(eReligion, false /*bCanIncludeReligionStarter*/))
					{
						iScore = 1000 / (m_pPlayer->GetNumUnitsWithUnitAI(UNITAI_MISSIONARY) + 1);		
					}
				}
				else if (eUnitClass == GC.getInfoTypeForString("UNITCLASS_INQUISITOR"))
				{
					if (HaveEnoughInquisitors(eReligion))
					{
						iScore = 1000 / (m_pPlayer->GetNumUnitsWithUnitAI(UNITAI_INQUISITOR) + 1);
					}
				}

				if (iScore > iBestScore)
				{
					iBestScore = iScore;
					eRtnValue = eUnit;
				}
			}
		}
	}

	return eRtnValue;
}

/// Log choices considered for beliefs
void CvReligionAI::LogBeliefChoices(CvWeightedVector<BeliefTypes, SAFE_ESTIMATE_NUM_BELIEFS, true>& beliefChoices, int iChoice)
{
	if(GC.getLogging() && GC.getAILogging())
	{
		CvString strOutBuf;
		CvString strBaseString;
		CvString strTemp;
		CvString playerName;
		CvString strDesc;
		BeliefTypes eBelief;

		// Find the name of this civ
		playerName = m_pPlayer->getCivilizationShortDescription();

		// Open the log file
		FILogFile* pLog;
		pLog = LOGFILEMGR.GetLog(GC.getGame().GetGameReligions()->GetLogFileName(), FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		strBaseString.Format("%03d, %d, ", GC.getGame().getElapsedGameTurns(), GC.getGame().getGameTurnYear());
		strBaseString += playerName + ", ";

		// Dump out the weight of each belief item
		for(int iI = 0; iI < beliefChoices.size(); iI++)
		{
			eBelief = beliefChoices.GetElement(iI);
			strDesc = GetLocalizedText(GC.GetGameBeliefs()->GetEntry(eBelief)->getShortDescription());
			strTemp.Format("Belief, %s, %d", strDesc.GetCString(), beliefChoices.GetWeight(iI));
			strOutBuf = strBaseString + strTemp;
			pLog->Msg(strOutBuf);
		}

		// Finally the chosen one
		eBelief = (BeliefTypes)iChoice;
		strDesc = GetLocalizedText(GC.GetGameBeliefs()->GetEntry(eBelief)->getShortDescription());
		strTemp.Format("CHOSEN, %s", strDesc.GetCString());
		strOutBuf = strBaseString + strTemp;
		pLog->Msg(strOutBuf);
	}
}

// AI HELPER ROUTINES

CvCity *CvReligionAIHelpers::GetBestCityFaithUnitPurchase(CvPlayer &kPlayer, UnitTypes eUnit, ReligionTypes eReligion)
{
	bool bReligious = false;
	CvCity *pHolyCity = NULL;
	SpecialUnitTypes eSpecialUnitGreatPerson = (SpecialUnitTypes) GC.getInfoTypeForString("SPECIALUNIT_PEOPLE");
	int iLoop;
	CvCity* pLoopCity;

	CvGameReligions* pReligions = GC.getGame().GetGameReligions();
	const CvReligion* pMyReligion = pReligions->GetReligion(eReligion, kPlayer.GetID());

	CvUnitEntry *pkUnitEntry = GC.getUnitInfo(eUnit);
	if (pkUnitEntry)
	{
		// Religious unit?
		if (pkUnitEntry->IsSpreadReligion() || pkUnitEntry->IsRemoveHeresy())
		{
			bReligious = true;
		}
	
		// Great person?
		else if (pkUnitEntry->GetSpecialUnitType() == eSpecialUnitGreatPerson)
		{
			for (pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			{
				if (pLoopCity->IsCanPurchase(true/*bTestPurchaseCost*/, true/*bTestTrainable*/, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
				{
					return pLoopCity;
				}
			}
			return NULL;
		}
	}

	// If religious, try to buy in the city with the Great Mosque first if a Missionary
	if (bReligious && pMyReligion)
	{
		for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
		{
			if (pLoopCity->getOwner() != kPlayer.GetID())
			{
				continue;
			}

			if (pLoopCity->GetCityBuildings()->GetMissionaryExtraSpreads() < 1 || pkUnitEntry->GetReligionSpreads() < 1)
			{
				continue;
			}

			if (pLoopCity->GetCityReligions()->GetReligiousMajority() == eReligion)
			{
				if(pLoopCity->IsCanPurchase(true, true, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
				{
					return pLoopCity;
				}
			}
		}
	}

	// If religious, next try to buy in the holy city, assuming it hasn't been converted
	if (bReligious && pMyReligion)
	{
		CvPlot* pHolyCityPlot = GC.getMap().plot(pMyReligion->m_iHolyCityX, pMyReligion->m_iHolyCityY);
		if (pHolyCityPlot)
		{
			pHolyCity = pHolyCityPlot->getPlotCity();

			if (pHolyCity && (pHolyCity->getOwner() == kPlayer.GetID()))
			{
				if (pHolyCity->GetCityReligions()->GetReligiousMajority() == eReligion && pHolyCity->IsCanPurchase(true, true, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
				{
					return pHolyCity;
				}
			}
		}
	}

	// Now see if there is another city with our majority religion
	for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
	{
		if (bReligious && pMyReligion && pLoopCity == pHolyCity)
		{
			continue;
		}

		if (pLoopCity->getOwner() != kPlayer.GetID())
		{
			continue;
		}

		if (pLoopCity->GetCityReligions()->GetReligiousMajority() == eReligion)
		{
			if(pLoopCity->IsCanPurchase(true, true, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
			{
				return pLoopCity;
			}
		}
	}

	return NULL;
}

CvCity *CvReligionAIHelpers::GetBestCityFaithBuildingPurchase(CvPlayer &kPlayer, BuildingTypes eBuilding, ReligionTypes eReligion)
{
	CvCity *pHolyCity = NULL;
	CvGameReligions* pReligions = GC.getGame().GetGameReligions();
	const CvReligion* pMyReligion = pReligions->GetReligion(eReligion, kPlayer.GetID());

	// Try to buy in the holy city first
	if (pMyReligion)
	{
		CvPlot* pHolyCityPlot = GC.getMap().plot(pMyReligion->m_iHolyCityX, pMyReligion->m_iHolyCityY);
		if (pHolyCityPlot)
		{
			pHolyCity = pHolyCityPlot->getPlotCity();

			if (pHolyCity && (pHolyCity->getOwner() == kPlayer.GetID()) && pHolyCity->IsCanPurchase(true, true, NO_UNIT, eBuilding, NO_PROJECT, YIELD_FAITH))
			{
				return pHolyCity;
			}
		}
	}

	// Now see if there is another city with our majority religion
	int iLoop;
	CvCity* pLoopCity;
	for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
	{
		if(pLoopCity == pHolyCity)
		{
			continue;
		}

		if(pLoopCity->getOwner() != kPlayer.GetID())
		{
			continue;
		}

		if(pLoopCity->IsCanPurchase(true, true, NO_UNIT, eBuilding, NO_PROJECT, YIELD_FAITH))
		{
			return pLoopCity;
		}
	}

	return NULL;
}

bool CvReligionAIHelpers::DoesUnitPassFaithPurchaseCheck(CvPlayer &kPlayer, UnitTypes eUnit)
{
	bool bRtnValue = true;

	CvUnitEntry *pkUnitEntry = GC.getUnitInfo(eUnit);
	if (pkUnitEntry)
	{
		if (pkUnitEntry->IsSpreadReligion() || pkUnitEntry->IsRemoveHeresy())
		{
			bRtnValue = false;

			// Religious unit, have to find a city with the majority religion we started
			// Now see if there is another city with our majority religion
			int iLoop;
			CvCity* pLoopCity;

			CvGameReligions* pReligions = GC.getGame().GetGameReligions();
			ReligionTypes eReligion = pReligions->GetReligionCreatedByPlayer(kPlayer.GetID());
			if (eReligion > RELIGION_PANTHEON)
			{
				for(pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
				{
					if (pLoopCity->getOwner() != kPlayer.GetID())
					{
						continue;
					}

					if (pLoopCity->GetCityReligions()->GetReligiousMajority() == eReligion)
					{
						if(pLoopCity->IsCanPurchase(false /*bTestPurchaseCost*/, true, eUnit, NO_BUILDING, NO_PROJECT, YIELD_FAITH))
						{
							return true;
						}
					}
				}
			}
		}
	}

	return bRtnValue;
}
