/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvDllNetMessageHandler.h"
#include "CvDllContext.h"

#include "CvDiplomacyAI.h"
#include "CvTypes.h"
#include "CvGameCoreUtils.h"

CvDllNetMessageHandler::CvDllNetMessageHandler()
{
}
//------------------------------------------------------------------------------
CvDllNetMessageHandler::~CvDllNetMessageHandler()
{
}
//------------------------------------------------------------------------------
void* CvDllNetMessageHandler::QueryInterface(GUID guidInterface)
{
	if(guidInterface == ICvUnknown::GetInterfaceId() ||
	        guidInterface == ICvNetMessageHandler1::GetInterfaceId() ||
	        guidInterface == ICvNetMessageHandler2::GetInterfaceId() ||
			guidInterface == ICvNetMessageHandler3::GetInterfaceId())
	{
		return this;
	}

	return NULL;
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::Destroy()
{
	// Do nothing.
	// This is a static class whose instance is managed externally.
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::operator delete(void* p)
{
	CvDllGameContext::Free(p);
}
//------------------------------------------------------------------------------
void* CvDllNetMessageHandler::operator new(size_t bytes)
{
	return CvDllGameContext::Allocate(bytes);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseAdvancedStartAction(PlayerTypes ePlayer, AdvancedStartActionTypes eAction, int iX, int iY, int iData, bool bAdd)
{
	GET_PLAYER(ePlayer).doAdvancedStartAction(eAction, iX, iY, iData, bAdd);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseAutoMission(PlayerTypes ePlayer, int iUnitID)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);
	if(pkUnit)
	{
		pkUnit->AutoMission();
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseBarbarianRansom(PlayerTypes ePlayer, int iOptionChosen, int iUnitID)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

	// Pay ransom
	if(iOptionChosen == 0)
	{
		CvTreasury* pkTreasury = kPlayer.GetTreasury();
		int iNumGold = /*100*/ GC.getBARBARIAN_UNIT_GOLD_RANSOM_exp();
		const int iTreasuryGold = pkTreasury->GetGold();
		if(iNumGold > iTreasuryGold)
		{
			iNumGold = iTreasuryGold;
		}

		pkTreasury->ChangeGold(-iNumGold);
	}
	// Abandon Unit
	else if(iOptionChosen == 1)
	{
		CvUnit* pkUnit = kPlayer.getUnit(iUnitID);
		if(pkUnit != NULL)
			pkUnit->kill(true, BARBARIAN_PLAYER);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseChangeWar(PlayerTypes ePlayer, TeamTypes eRivalTeam, bool bWar)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
#ifdef AUI_WARNING_FIXES
	const TeamTypes eTeam = kPlayer.getTeam();
	CvTeam& kTeam = GET_TEAM(eTeam);
#else
	CvTeam& kTeam = GET_TEAM(kPlayer.getTeam());
	const TeamTypes eTeam = kPlayer.getTeam();
#endif

	FAssert(eTeam != eRivalTeam);

	if(bWar)
	{
		kTeam.declareWar(eRivalTeam);
	}
	else
	{
		kTeam.makePeace(eRivalTeam);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseIgnoreWarning(PlayerTypes ePlayer, TeamTypes eRivalTeam)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
#ifdef AUI_WARNING_FIXES
	const TeamTypes eTeam = kPlayer.getTeam();
	CvTeam& kTeam = GET_TEAM(eTeam);
#else
	CvTeam& kTeam = GET_TEAM(kPlayer.getTeam());
	const TeamTypes eTeam = kPlayer.getTeam();
#endif
	FAssert(eTeam != eRivalTeam);
	
	kTeam.PushIgnoreWarning(eRivalTeam);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityBuyPlot(PlayerTypes ePlayer, int iCityID, int iX, int iY)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity != NULL)
	{
		CvPlot* pkPlot = NULL;

		// (-1,-1) means pick a random plot to buy
		if(iX == -1 && iY == -1)
		{
			pkPlot = pkCity->GetNextBuyablePlot();
		}
		else
		{
			pkPlot = GC.getMap().plot(iX, iY);
		}

		if(pkPlot != NULL)
		{
			if(pkCity->CanBuyPlot(pkPlot->getX(), pkPlot->getY()))
			{
				pkCity->BuyPlot(pkPlot->getX(), pkPlot->getY());
				if(ePlayer == GC.getGame().getActivePlayer() && GC.GetEngineUserInterface()->isCityScreenUp())
				{
					GC.GetEngineUserInterface()->setDirty(CityScreen_DIRTY_BIT, true);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityDoTask(PlayerTypes ePlayer, int iCityID, TaskTypes eTask, int iData1, int iData2, bool bOption, bool bAlt, bool bShift, bool bCtrl)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);

	if(pkCity != NULL)
	{
		pkCity->doTask(eTask, iData1, iData2, bOption, bAlt, bShift, bCtrl);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityPopOrder(PlayerTypes ePlayer, int iCityID, int iNum)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity != NULL)
	{
		pkCity->popOrder(iNum);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityPurchase(PlayerTypes ePlayer, int iCityID, UnitTypes eUnitType, BuildingTypes eBuildingType, ProjectTypes eProjectType)
{
	ResponseCityPurchase(ePlayer, iCityID, eUnitType, eBuildingType, eProjectType, NO_YIELD);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityPurchase(PlayerTypes ePlayer, int iCityID, UnitTypes eUnitType, BuildingTypes eBuildingType, ProjectTypes eProjectType, int ePurchaseYield)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity && ePurchaseYield >= -1 && ePurchaseYield < NUM_YIELD_TYPES)
	{
		pkCity->Purchase(eUnitType, eBuildingType, eProjectType, static_cast<YieldTypes>(ePurchaseYield));
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCityPushOrder(PlayerTypes ePlayer, int iCityID, OrderTypes eOrder, int iData, bool bAlt, bool bShift, bool bCtrl)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity != NULL)
	{
		pkCity->pushOrder(eOrder, iData, -1, bAlt, bShift, bCtrl);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseCitySwapOrder(PlayerTypes ePlayer, int iCityID, int iNum)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity != NULL)
	{
		pkCity->swapOrder(iNum);
	}
}
//------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvDllNetMessageHandler::ResponseChooseElection(PlayerTypes, int, int)
#else
void CvDllNetMessageHandler::ResponseChooseElection(PlayerTypes ePlayer, int iSelection, int iVoteId)
#endif
{
	// Unused
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseDestroyUnit(PlayerTypes ePlayer, int iUnitID)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);

	if(pkUnit)
	{
		pkUnit->kill(true, ePlayer);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseDiplomacyFromUI(PlayerTypes ePlayer, PlayerTypes eOtherPlayer, FromUIDiploEventTypes eEvent, int iArg1, int iArg2)
{
	GET_PLAYER(eOtherPlayer).GetDiplomacyAI()->DoFromUIDiploEvent(ePlayer, eEvent, iArg1, iArg2);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseDiploVote(PlayerTypes ePlayer, PlayerTypes eVotePlayer)
{
	TeamTypes eVotingTeam = GET_PLAYER(ePlayer).getTeam();
	TeamTypes eVote = GET_PLAYER(eVotePlayer).getTeam();

	GC.getGame().SetVoteCast(eVotingTeam, eVote);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseDoCommand(PlayerTypes ePlayer, int iUnitID, CommandTypes eCommand, int iData1, int iData2, bool bAlt)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);

	if(pkUnit != NULL)
	{
#ifdef AUI_WARNING_FIXES
		CvCommandInfo* pCommandInfo = GC.getCommandInfo(eCommand);
		if (bAlt && pCommandInfo && pCommandInfo->getAll())
#else
		if(bAlt && GC.getCommandInfo(eCommand)->getAll())
#endif
		{
			const UnitTypes eUnitType = pkUnit->getUnitType();

			CvUnit* pkLoopUnit = NULL;
			int iLoop = 0;

			for(pkLoopUnit = kPlayer.firstUnit(&iLoop); pkLoopUnit != NULL; pkLoopUnit = kPlayer.nextUnit(&iLoop))
			{
				if(pkLoopUnit->getUnitType() == eUnitType)
				{
					pkLoopUnit->doCommand(eCommand, iData1, iData2);
				}
			}
		}
		else
		{
			pkUnit->doCommand(eCommand, iData1, iData2);
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseExtendedGame(PlayerTypes ePlayer)
{
	GET_PLAYER(ePlayer).makeExtendedGame();
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseFoundPantheon(PlayerTypes ePlayer, BeliefTypes eBelief)
{
	CvGame& kGame(GC.getGame());
	CvGameReligions* pkGameReligions(kGame.GetGameReligions());
	CvBeliefXMLEntries* pkBeliefs = GC.GetGameBeliefs();
	CvBeliefEntry* pEntry = pkBeliefs->GetEntry((int)eBelief);

	// Pantheon belief, or adding one through Reformation?
	if (pEntry && ePlayer != NO_PLAYER)
	{
		if (pEntry->IsPantheonBelief())
		{
			CvGameReligions::FOUNDING_RESULT eResult = pkGameReligions->CanCreatePantheon(ePlayer, true);
			if(eResult == CvGameReligions::FOUNDING_OK)
			{
				if(pkGameReligions->IsPantheonBeliefAvailable(eBelief))
				{
					pkGameReligions->FoundPantheon(ePlayer, eBelief);
				}
				else
				{
					CvGameReligions::NotifyPlayer(ePlayer, CvGameReligions::FOUNDING_BELIEF_IN_USE);
				}
			}
			else
			{
				CvGameReligions::NotifyPlayer(ePlayer, eResult);
			}
		}
		else if (pEntry->IsReformationBelief())
		{
#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
			CvGameReligions::FOUNDING_RESULT eResult = pkGameReligions->CanAddReformationBelief(ePlayer, eBelief);
			if (eResult == CvGameReligions::FOUNDING_OK)
			{
				ReligionTypes eReligion = GET_PLAYER(ePlayer).GetReligions()->GetReligionCreatedByPlayer();
#else
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
			if (!pkGameReligions->HasAddedReformationBelief(ePlayer) && kPlayer.GetReligions()->HasCreatedReligion())
			{
				ReligionTypes eReligion = kPlayer.GetReligions()->GetReligionCreatedByPlayer();
#endif
				pkGameReligions->AddReformationBelief(ePlayer, eReligion, eBelief);
			}
#ifdef AUI_RELIGION_FIX_NO_BELIEFS_AVAILABLE_CHECK_FOR_NON_PANTHEON_MOVED
			else
			{
				CvGameReligions::NotifyPlayer(ePlayer, eResult);
			}
#endif
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseFoundReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, BeliefTypes eBelief3, BeliefTypes eBelief4, int iCityX, int iCityY)
{
	CvGame& kGame(GC.getGame());
	CvGameReligions* pkGameReligions(kGame.GetGameReligions());

	CvCity* pkCity = GC.getMap().plot(iCityX, iCityY)->getPlotCity();
	if(pkCity && ePlayer != NO_PLAYER)
	{
		CvGameReligions::FOUNDING_RESULT eResult = pkGameReligions->CanFoundReligion(ePlayer, eReligion, szCustomName, eBelief1, eBelief2, eBelief3, eBelief4, pkCity);
		if(eResult == CvGameReligions::FOUNDING_OK)
			pkGameReligions->FoundReligion(ePlayer, eReligion, szCustomName, eBelief1, eBelief2, eBelief3, eBelief4, pkCity);
		else
		{
			CvGameReligions::NotifyPlayer(ePlayer, eResult);
			// We don't want them to lose the opportunity to found the religion, and the Great Prophet is already gone so just repost the notification
			// If someone beat them to the last religion, well... tough luck.
			CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
			if (kPlayer.isHuman() && eResult != CvGameReligions::FOUNDING_NO_RELIGIONS_AVAILABLE && eResult != CvGameReligions::FOUNDING_NO_BELIEFS_AVAILABLE)
#else
			if(kPlayer.isHuman() && eResult != CvGameReligions::FOUNDING_NO_RELIGIONS_AVAILABLE)
#endif
			{
				CvNotifications* pNotifications = kPlayer.GetNotifications();
				if(pNotifications)
				{
					CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_FOUND_RELIGION");
					CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_FOUND_RELIGION");
					pNotifications->Add(NOTIFICATION_FOUND_RELIGION, strBuffer, strSummary, iCityX, iCityY, -1, pkCity->GetID());
				}
				kPlayer.GetReligions()->SetFoundingReligion(true);
			}
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
			else if (kPlayer.getCapitalCity())
			{
				UnitTypes eUnit = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
				if (eUnit != NO_UNIT)
				{
					kPlayer.getCapitalCity()->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, false, true);
				}
			}
#endif
		}
	}
}
//------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvDllNetMessageHandler::ResponseEnhanceReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char*, BeliefTypes eBelief1, BeliefTypes eBelief2, int iCityX, int iCityY)
#else
void CvDllNetMessageHandler::ResponseEnhanceReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, int iCityX, int iCityY)
#endif
{
	CvGame& kGame(GC.getGame());
	CvGameReligions* pkGameReligions(kGame.GetGameReligions());

	CvGameReligions::FOUNDING_RESULT eResult = pkGameReligions->CanEnhanceReligion(ePlayer, eReligion, eBelief1, eBelief2);
	if(eResult == CvGameReligions::FOUNDING_OK)
		pkGameReligions->EnhanceReligion(ePlayer, eReligion, eBelief1, eBelief2);
	else
	{
		CvGameReligions::NotifyPlayer(ePlayer, eResult);
		// We don't want them to lose the opportunity to enhance the religion, and the Great Prophet is already gone so just repost the notification
		CvCity* pkCity = GC.getMap().plot(iCityX, iCityY)->getPlotCity();
		CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
		if (kPlayer.isHuman() && pkCity && eResult != CvGameReligions::FOUNDING_NO_BELIEFS_AVAILABLE)
#else
		if(kPlayer.isHuman() && eResult != CvGameReligions::FOUNDING_NO_RELIGIONS_AVAILABLE && pkCity)
#endif
		{
			CvNotifications* pNotifications = kPlayer.GetNotifications();
			if(pNotifications)
			{
				CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_ENHANCE_RELIGION");
				CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_ENHANCE_RELIGION");
				pNotifications->Add(NOTIFICATION_ENHANCE_RELIGION, strBuffer, strSummary, iCityX, iCityY, -1, pkCity->GetID());
			}
			kPlayer.GetReligions()->SetFoundingReligion(true);
		}
#ifdef AUI_DLLNETMESSAGEHANDLER_FIX_RESPAWN_PROPHET_IF_BEATEN_TO_LAST_RELIGION
		else if (kPlayer.getCapitalCity())
		{
			UnitTypes eUnit = (UnitTypes)GC.getInfoTypeForString("UNIT_PROPHET", true);
			if (eUnit != NO_UNIT)
			{
				kPlayer.getCapitalCity()->GetCityCitizens()->DoSpawnGreatPerson(eUnit, false /*bIncrementCount*/, false, true);
			}
		}
#endif
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMoveSpy(PlayerTypes ePlayer, int iSpyIndex, int iTargetPlayer, int iTargetCity, bool bAsDiplomat)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvPlayerEspionage* pPlayerEspionage = kPlayer.GetEspionage();

	if(pPlayerEspionage)
	{
		if(iTargetCity == -1)
		{
			pPlayerEspionage->ExtractSpyFromCity(iSpyIndex);
			GC.GetEngineUserInterface()->setDirty(EspionageScreen_DIRTY_BIT, true);
		}
		else
		{
			CvAssertMsg(iTargetPlayer != -1, "iTargetPlayer is -1");
			if(iTargetPlayer != -1)
			{
				PlayerTypes eTargetPlayer = (PlayerTypes)iTargetPlayer;
				CvCity* pCity = GET_PLAYER(eTargetPlayer).getCity(iTargetCity);
				CvAssertMsg(pCity, "pCity is null");
				if(pCity)
				{
					pPlayerEspionage->MoveSpyTo(pCity, iSpyIndex, bAsDiplomat);
					GC.GetEngineUserInterface()->setDirty(EspionageScreen_DIRTY_BIT, true);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseStageCoup(PlayerTypes eSpyPlayer, int iSpyIndex)
{
	CvAssertMsg(eSpyPlayer != NO_PLAYER, "eSpyPlayer invalid");
	CvAssertMsg(iSpyIndex >= 0, "iSpyIndex invalid");

	CvPlayerAI& kPlayer = GET_PLAYER(eSpyPlayer);
	CvPlayerEspionage* pPlayerEspionage = kPlayer.GetEspionage();

	CvAssertMsg(pPlayerEspionage, "pPlayerEspionage is null");
	if(pPlayerEspionage)
	{
#ifdef AUI_WARNING_FIXES
		pPlayerEspionage->AttemptCoup(iSpyIndex);
#else
		bool bCoupSuccess = pPlayerEspionage->AttemptCoup(iSpyIndex);
#endif
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseFaithPurchase(PlayerTypes ePlayer, FaithPurchaseTypes eFaithPurchaseType, int iFaithPurchaseIndex)
{
	CvAssertMsg(ePlayer != NO_PLAYER, "ePlayer invalid");
	CvAssertMsg(eFaithPurchaseType > -1, "Faith Purchase Type invalid");
	CvAssertMsg(iFaithPurchaseIndex > -1, "Faith Purchase Index invalid");

	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	kPlayer.SetFaithPurchaseType(eFaithPurchaseType);
	kPlayer.SetFaithPurchaseIndex(iFaithPurchaseIndex);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueVoteEnact(LeagueTypes /*eLeague*/, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueVoteEnact(LeagueTypes eLeague, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	CvAssertMsg(pLeague->CanVote(eVoter), "eVoter not allowed to vote. Please send Anton your save file and version.");
	pLeague->DoVoteEnact(iResolutionID, eVoter, iNumVotes, iChoice);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueVoteRepeal(LeagueTypes /*eLeague*/, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueVoteRepeal(LeagueTypes eLeague, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	CvAssertMsg(pLeague->CanVote(eVoter), "eVoter not allowed to vote. Please send Anton your save file and version.");
	pLeague->DoVoteRepeal(iResolutionID, eVoter, iNumVotes, iChoice);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueVoteAbstain(LeagueTypes /*eLeague*/, PlayerTypes eVoter, int iNumVotes)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueVoteAbstain(LeagueTypes eLeague, PlayerTypes eVoter, int iNumVotes)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eVoter != NO_PLAYER, "eVoter invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	CvAssertMsg(pLeague->CanVote(eVoter), "eVoter not allowed to vote. Please send Anton your save file and version.");
	pLeague->DoVoteAbstain(eVoter, iNumVotes);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueProposeEnact(LeagueTypes /*eLeague*/, ResolutionTypes eResolution, PlayerTypes eProposer, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eResolution != NO_RESOLUTION, "eResolution invalid");
	CvAssertMsg(eProposer != NO_PLAYER, "eProposer invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueProposeEnact(LeagueTypes eLeague, ResolutionTypes eResolution, PlayerTypes eProposer, int iChoice)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eResolution != NO_RESOLUTION, "eResolution invalid");
	CvAssertMsg(eProposer != NO_PLAYER, "eProposer invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	CvAssertMsg(pLeague->CanProposeEnact(eResolution, eProposer, iChoice), "eProposer not allowed to enact Resolution. Please send Anton your save file and version.");
	pLeague->DoProposeEnact(eResolution, eProposer, iChoice);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueProposeRepeal(LeagueTypes /*eLeague*/, int iResolutionID, PlayerTypes eProposer)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eProposer != NO_PLAYER, "eProposer invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueProposeRepeal(LeagueTypes eLeague, int iResolutionID, PlayerTypes eProposer)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");
	CvAssertMsg(eProposer != NO_PLAYER, "eProposer invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	CvAssertMsg(pLeague->CanProposeRepeal(iResolutionID, eProposer), "eProposer not allowed to repeal Resolution. Please send Anton your save file and version.");
	pLeague->DoProposeRepeal(iResolutionID, eProposer);
}
//------------------------------------------------------------------------------
#ifdef AUI_LEAGUES_FIX_POSSIBLE_DEALLOCATION_CRASH
void CvDllNetMessageHandler::ResponseLeagueEditName(LeagueTypes /*eLeague*/, PlayerTypes ePlayer, const char* szCustomName)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetActiveLeague();
	if (pLeague == NULL)
		return;
#else
void CvDllNetMessageHandler::ResponseLeagueEditName(LeagueTypes eLeague, PlayerTypes ePlayer, const char* szCustomName)
{
	CvAssertMsg(eLeague != NO_LEAGUE, "eLeague invalid");

	CvLeague* pLeague = GC.getGame().GetGameLeagues()->GetLeague(eLeague);
#endif
	pLeague->DoChangeCustomName(ePlayer, szCustomName);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseSetSwappableGreatWork(PlayerTypes ePlayer, int iWorkClass, int iWorkIndex)
{
	CvAssertMsg(ePlayer != NO_PLAYER, "ePlayer invalid");
	
	// is this player alive
	if (GET_PLAYER(ePlayer).isAlive())
	{
		// -1 indicates that they want to clear the slot
		if (iWorkIndex == -1)
		{
			if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_ARTIFACT"))
			{
				GET_PLAYER(ePlayer).GetCulture()->SetSwappableArtifactIndex(-1);
			}
			else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_ART"))
			{
				GET_PLAYER(ePlayer).GetCulture()->SetSwappableArtIndex(-1);
			}
			else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_LITERATURE"))
			{
				GET_PLAYER(ePlayer).GetCulture()->SetSwappableWritingIndex(-1);
			}			
			else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_MUSIC"))
			{
				GET_PLAYER(ePlayer).GetCulture()->SetSwappableMusicIndex(-1);
			}
		}
		else
		{
			// does this player control this work
			if (GET_PLAYER(ePlayer).GetCulture()->ControlsGreatWork(iWorkIndex))
			{
				if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_ARTIFACT"))
				{
					GET_PLAYER(ePlayer).GetCulture()->SetSwappableArtifactIndex(iWorkIndex);
				}
				else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_ART"))
				{
					GET_PLAYER(ePlayer).GetCulture()->SetSwappableArtIndex(iWorkIndex);
				}
				else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_LITERATURE"))
				{
					GET_PLAYER(ePlayer).GetCulture()->SetSwappableWritingIndex(iWorkIndex);
				}			
				else if (iWorkClass == GC.getInfoTypeForString("GREAT_WORK_MUSIC"))
				{
					GET_PLAYER(ePlayer).GetCulture()->SetSwappableMusicIndex(iWorkIndex);
				}				
			}
		}
		GC.GetEngineUserInterface()->setDirty(GreatWorksScreen_DIRTY_BIT, true);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseSwapGreatWorks(PlayerTypes ePlayer1, int iWorkIndex1, PlayerTypes ePlayer2, int iWorkIndex2)
{
	GC.getGame().GetGameCulture()->SwapGreatWorks(ePlayer1, iWorkIndex1, ePlayer2, iWorkIndex2);
}

//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMoveGreatWorks(PlayerTypes ePlayer, int iCity1, int iBuildingClass1, int iWorkIndex1, 
																																				 int iCity2, int iBuildingClass2, int iWorkIndex2)
{
	GC.getGame().GetGameCulture()->MoveGreatWorks(ePlayer, iCity1, iBuildingClass1, iWorkIndex1, iCity2, iBuildingClass2, iWorkIndex2);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseChangeIdeology(PlayerTypes ePlayer)
{
	CvAssertMsg(ePlayer != NO_PLAYER, "ePlayer invalid");

	// is this player alive
	CvPlayer &kPlayer = GET_PLAYER(ePlayer);
	if (kPlayer.isAlive())
	{
		PolicyBranchTypes ePreferredIdeology = kPlayer.GetCulture()->GetPublicOpinionPreferredIdeology();
		kPlayer.SetAnarchyNumTurns(GC.getSWITCH_POLICY_BRANCHES_ANARCHY_TURNS());
		kPlayer.GetPlayerPolicies()->DoSwitchIdeologies(ePreferredIdeology);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseGiftUnit(PlayerTypes ePlayer, PlayerTypes eMinor, int iUnitID)
{
	CvUnit* pkUnit = GET_PLAYER(ePlayer).getUnit(iUnitID);
	GET_PLAYER(eMinor).DoDistanceGift(ePlayer, pkUnit);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseLaunchSpaceship(PlayerTypes ePlayer, VictoryTypes eVictory)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvTeam& kTeam = GET_TEAM(kPlayer.getTeam());

	if(kTeam.canLaunch(eVictory))
	{
		kPlayer.launch(eVictory);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseLiberatePlayer(PlayerTypes ePlayer, PlayerTypes eLiberatedPlayer, int iCityID)
{
	GET_PLAYER(ePlayer).DoLiberatePlayer(eLiberatedPlayer, iCityID);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorCivBullyGold(PlayerTypes ePlayer, PlayerTypes eMinor, int iGold)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->DoMajorBullyGold(ePlayer, iGold);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorCivBullyUnit(PlayerTypes ePlayer, PlayerTypes eMinor, UnitTypes eUnitType)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->DoMajorBullyUnit(ePlayer, eUnitType);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorCivGiftGold(PlayerTypes ePlayer, PlayerTypes eMinor, int iGold)
{
#ifdef NQ_BELIEF_TOGGLE_ALLOW_FAITH_GIFTS_TO_MINORS
	if (iGold > 100000) // HACK!!! ... when sending a faith gift, we still use this function but we add in 100K to clue us in
	{
		int iFaith = iGold - 100000;
		// Enough Faith?
		if(GET_PLAYER(ePlayer).GetFaith() >= iFaith)
		{
			GET_PLAYER(eMinor).GetMinorCivAI()->DoFaithGiftFromMajor(ePlayer, iFaith);
		}
	}
	else
	{
		// Enough Gold?
		if(GET_PLAYER(ePlayer).GetTreasury()->GetGold() >= iGold)
		{
			GET_PLAYER(eMinor).GetMinorCivAI()->DoGoldGiftFromMajor(ePlayer, iGold);
		}
	}
#else
	// Enough Gold?
	if(GET_PLAYER(ePlayer).GetTreasury()->GetGold() >= iGold)
	{
		GET_PLAYER(eMinor).GetMinorCivAI()->DoGoldGiftFromMajor(ePlayer, iGold);
	}
#endif
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorCivGiftTileImprovement(PlayerTypes eMajor, PlayerTypes eMinor, int iPlotX, int iPlotY)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->DoTileImprovementGiftFromMajor(eMajor, iPlotX, iPlotY);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorCivBuyout(PlayerTypes eMajor, PlayerTypes eMinor)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->DoBuyout(eMajor);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMinorNoUnitSpawning(PlayerTypes ePlayer, PlayerTypes eMinor, bool bValue)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->SetUnitSpawningDisabled(ePlayer, bValue);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponsePlayerDealFinalized(PlayerTypes eFromPlayer, PlayerTypes eToPlayer, PlayerTypes eActBy, bool bAccepted)
{
	CvGame& game = GC.getGame();
#ifndef AUI_WARNING_FIXES
	PlayerTypes eActivePlayer = game.getActivePlayer();
#endif

	// is the deal valid?
	if(!game.GetGameDeals()->FinalizeDeal(eFromPlayer, eToPlayer, bAccepted))
	{
		Localization::String strMessage;
		Localization::String strSummary = Localization::Lookup("TXT_KEY_DEAL_EXPIRED");

		CvPlayerAI& kToPlayer = GET_PLAYER(eToPlayer);
		CvPlayerAI& kFromPlayer = GET_PLAYER(eFromPlayer);
#ifndef AUI_WARNING_FIXES
		CvPlayerAI& kActivePlayer = GET_PLAYER(eActivePlayer);
#endif

		strMessage = Localization::Lookup("TXT_KEY_DEAL_EXPIRED_FROM_YOU");
		strMessage << kToPlayer.getNickName();
		kFromPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eToPlayer, -1, -1);

		strMessage = Localization::Lookup("TXT_KEY_DEAL_EXPIRED_FROM_THEM");
		strMessage << kFromPlayer.getNickName();
		kToPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eFromPlayer, -1, -1);
	}
	else
	{
		CvPlayerAI& kToPlayer = GET_PLAYER(eToPlayer);
		CvPlayerAI& kFromPlayer = GET_PLAYER(eFromPlayer);
		if(bAccepted)
		{
			Localization::String strSummary = Localization::Lookup("TXT_KEY_DEAL_ACCEPTED");
			Localization::String strMessage = Localization::Lookup("TXT_KEY_DEAL_ACCEPTED_BY_THEM");
			strMessage << kToPlayer.getNickName();
			kFromPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eToPlayer, -1, -1);

			strSummary = Localization::Lookup("TXT_KEY_DEAL_ACCEPTED");
			strMessage = Localization::Lookup("TXT_KEY_DEAL_ACCEPTED_BY_YOU");
			strMessage << kFromPlayer.getNickName();
			kToPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eFromPlayer, -1, -1);
		}
		else
		{
			if(eActBy == eFromPlayer)
			{
				Localization::String strSummary = Localization::Lookup("TXT_KEY_DEAL_WITHDRAWN");
				Localization::String strMessage = Localization::Lookup("TXT_KEY_DEAL_WITHDRAWN_BY_YOU");
				strMessage << kToPlayer.getNickName();
				kFromPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eToPlayer, -1, -1);

				strSummary = Localization::Lookup("TXT_KEY_DEAL_WITHDRAWN");
				strMessage = Localization::Lookup("TXT_KEY_DEAL_WITHDRAWN_BY_THEM");
				strMessage << kFromPlayer.getNickName();
				kToPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eFromPlayer, -1, -1);
			}
			else
			{
				Localization::String strSummary = Localization::Lookup("TXT_KEY_DEAL_REJECTED");
				Localization::String strMessage = Localization::Lookup("TXT_KEY_DEAL_REJECTED_BY_THEM");
				strMessage << kToPlayer.getNickName();
				kFromPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eToPlayer, -1, -1);

				strSummary = Localization::Lookup("TXT_KEY_DEAL_REJECTED");
				strMessage = Localization::Lookup("TXT_KEY_DEAL_REJECTED_BY_YOU");
				strMessage << kFromPlayer.getNickName();
				kToPlayer.GetNotifications()->Add(NOTIFICATION_PLAYER_DEAL_RESOLVED, strMessage.toUTF8(), strSummary.toUTF8(), eFromPlayer, -1, -1);
			}
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponsePlayerOption(PlayerTypes ePlayer, PlayerOptionTypes eOption, bool bValue)
{
	GET_PLAYER(ePlayer).setOption(eOption, bValue);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponsePledgeMinorProtection(PlayerTypes ePlayer, PlayerTypes eMinor, bool bValue, bool bPledgeNowBroken)
{
	GET_PLAYER(eMinor).GetMinorCivAI()->DoChangeProtectionFromMajor(ePlayer, bValue, bPledgeNowBroken);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponsePushMission(PlayerTypes ePlayer, int iUnitID, MissionTypes eMission, int iData1, int iData2, int iFlags, bool bShift)
{
	CvUnit::dispatchingNetMessage(true);

	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);

	if(pkUnit != NULL)
	{
		pkUnit->PushMission(eMission, iData1, iData2, iFlags, bShift, true);
	}

	CvUnit::dispatchingNetMessage(false);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseGreatPersonChoice(PlayerTypes ePlayer, UnitTypes eGreatPersonUnit)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pCity = kPlayer.GetGreatPersonSpawnCity(eGreatPersonUnit);
	if(pCity)
	{
		// GJS NQMP - changed 2nd parameter to false so that "free" Great People from liberty finisher & buildings are actually free
		pCity->GetCityCitizens()->DoSpawnGreatPerson(eGreatPersonUnit, false, false);
	}
	kPlayer.ChangeNumFreeGreatPeople(-1);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseMayaBonusChoice(PlayerTypes ePlayer, UnitTypes eGreatPersonUnit)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pCity = kPlayer.GetGreatPersonSpawnCity(eGreatPersonUnit);
	if(pCity)
	{
		pCity->GetCityCitizens()->DoSpawnGreatPerson(eGreatPersonUnit, true, false);
	}
	kPlayer.ChangeNumMayaBoosts(-1);
	kPlayer.GetPlayerTraits()->SetUnitBaktun(eGreatPersonUnit);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseFaithGreatPersonChoice(PlayerTypes ePlayer, UnitTypes eGreatPersonUnit)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pCity = kPlayer.GetGreatPersonSpawnCity(eGreatPersonUnit);
	if(pCity)
	{
		pCity->GetCityCitizens()->DoSpawnGreatPerson(eGreatPersonUnit, true, true);
	}
	kPlayer.ChangeNumFaithGreatPeople(-1);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseGoodyChoice(PlayerTypes ePlayer, int iPlotX, int iPlotY, GoodyTypes eGoody, int iUnitID)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvPlot* pPlot = GC.getMap().plot(iPlotX, iPlotY);
	CvUnit* pUnit = kPlayer.getUnit(iUnitID);
	kPlayer.receiveGoody(pPlot, eGoody, pUnit);
}

//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseArchaeologyChoice(PlayerTypes ePlayer, ArchaeologyChoiceType eChoice)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	kPlayer.GetCulture()->DoArchaeologyChoice(eChoice);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseIdeologyChoice(PlayerTypes ePlayer, PolicyBranchTypes eChoice)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	kPlayer.GetPlayerPolicies()->SetPolicyBranchUnlocked(eChoice, true, false);
}
//------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvDllNetMessageHandler::ResponseRenameCity(PlayerTypes ePlayer, int iCityID, _In_z_ const char* szName)
#else
void CvDllNetMessageHandler::ResponseRenameCity(PlayerTypes ePlayer, int iCityID, const char* szName)
#endif
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvCity* pkCity = kPlayer.getCity(iCityID);
	if(pkCity)
	{
		CvString strName = szName;
		pkCity->setName(strName);
	}
}
//------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvDllNetMessageHandler::ResponseRenameUnit(PlayerTypes ePlayer, int iUnitID, _In_z_ const char* szName)
#else
void CvDllNetMessageHandler::ResponseRenameUnit(PlayerTypes ePlayer, int iUnitID, const char* szName)
#endif
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);
	if(pkUnit)
	{
		CvString strName = szName;
		pkUnit->setName(strName);
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseResearch(PlayerTypes ePlayer, TechTypes eTech, int iDiscover, bool bShift)
{
	ResponseResearch(ePlayer, eTech, iDiscover, NO_PLAYER, bShift);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseResearch(PlayerTypes ePlayer, TechTypes eTech, int iDiscover, PlayerTypes ePlayerToStealFrom, bool bShift)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvTeam& kTeam = GET_TEAM(kPlayer.getTeam());

	// Free tech
	if(iDiscover > 0)
	{
		// Make sure we can research this tech for free
		if(kPlayer.GetPlayerTechs()->CanResearchForFree(eTech))
		{
			kTeam.setHasTech(eTech, true, ePlayer, true, true);

			if(iDiscover > 1)
			{
				if(ePlayer == GC.getGame().getActivePlayer())
				{
					kPlayer.chooseTech(iDiscover - 1);
				}
			}
			kPlayer.SetNumFreeTechs(max(0, iDiscover - 1));
		}
	}
	// Stealing tech
	else if(ePlayerToStealFrom != NO_PLAYER)
	{
		// make sure we can still take a tech
		CvAssertMsg(kPlayer.GetEspionage()->m_aiNumTechsToStealList[ePlayerToStealFrom] > 0, "No techs to steal from player");
		CvAssertMsg(kPlayer.GetEspionage()->m_aaPlayerStealableTechList[ePlayerToStealFrom].size() > 0, "No techs to be stolen from this player");
		CvAssertMsg(kPlayer.GetPlayerTechs()->CanResearch(eTech), "Player can't research this technology");
		CvAssertMsg(GET_TEAM(GET_PLAYER(ePlayerToStealFrom).getTeam()).GetTeamTechs()->HasTech(eTech), "ePlayerToStealFrom does not have the requested tech");
		if (kPlayer.GetEspionage()->m_aiNumTechsToStealList[ePlayerToStealFrom] > 0)
		{
			kTeam.setHasTech(eTech, true, ePlayer, true, true);
			kPlayer.GetEspionage()->m_aiNumTechsToStealList[ePlayerToStealFrom]--;
		}
	}
	// Normal tech
	else
	{
		CvPlayerTechs* pPlayerTechs = kPlayer.GetPlayerTechs();
		CvTeamTechs* pTeamTechs = kTeam.GetTeamTechs();

		if(eTech == NO_TECH)
		{
			kPlayer.clearResearchQueue();
		}
		else if(pPlayerTechs->CanEverResearch(eTech))
		{
			if((pTeamTechs->HasTech(eTech) || pPlayerTechs->IsResearchingTech(eTech)) && !bShift)
			{
				kPlayer.clearResearchQueue();
			}

			kPlayer.pushResearch(eTech, !bShift);
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseReturnCivilian(PlayerTypes ePlayer, PlayerTypes eToPlayer, int iUnitID, bool bReturn)
{
	GET_PLAYER(ePlayer).DoCivilianReturnLogic(bReturn, eToPlayer, iUnitID);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseSellBuilding(PlayerTypes ePlayer, int iCityID, BuildingTypes eBuilding)
{
	CvCity* pCity = GET_PLAYER(ePlayer).getCity(iCityID);
	if(pCity)
	{
		pCity->GetCityBuildings()->DoSellBuilding(eBuilding);

		ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
		if (pkScriptSystem) 
		{
			CvLuaArgsHandle args;
			args->Push(ePlayer);
			args->Push(iCityID);
			args->Push(eBuilding);

			bool bResult;
			LuaSupport::CallHook(pkScriptSystem, "CitySoldBuilding", args.get(), bResult);
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseSetCityAIFocus(PlayerTypes ePlayer, int iCityID, CityAIFocusTypes eFocus)
{
	CvCity* pCity = GET_PLAYER(ePlayer).getCity(iCityID);
	if(pCity != NULL)
	{
		CvCityCitizens* pkCitizens = pCity->GetCityCitizens();
		if(pkCitizens != NULL)
		{
			pkCitizens->SetFocusType(eFocus);
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseSetCityAvoidGrowth(PlayerTypes ePlayer, int iCityID, bool bAvoidGrowth)
{
	CvCity* pCity = GET_PLAYER(ePlayer).getCity(iCityID);
	if(pCity != NULL)
	{
		CvCityCitizens* pkCitizens = pCity->GetCityCitizens();
		if(pkCitizens != NULL)
		{
			pkCitizens->SetForcedAvoidGrowth(bAvoidGrowth);
		}
	}
}
//------------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvDllNetMessageHandler::ResponseSwapUnits(PlayerTypes ePlayer, int iUnitID, MissionTypes, int iData1, int iData2, int, bool bShift)
#else
void CvDllNetMessageHandler::ResponseSwapUnits(PlayerTypes ePlayer, int iUnitID, MissionTypes eMission, int iData1, int iData2, int iFlags, bool bShift)
#endif
{
	CvUnit::dispatchingNetMessage(true);

	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);
	CvUnit* pkUnit = kPlayer.getUnit(iUnitID);

	if(pkUnit != NULL)
	{
		// Get target plot
		CvMap& kMap = GC.getMap();
		CvPlot* pkTargetPlot = kMap.plot(iData1, iData2);

		if(pkTargetPlot != NULL)
		{
			CvPlot* pkOriginationPlot = pkUnit->plot();

			// Find unit to move out
#ifdef AUI_WARNING_FIXES
			for (uint iI = 0; iI < pkTargetPlot->getNumUnits(); iI++)
#else
			for(int iI = 0; iI < pkTargetPlot->getNumUnits(); iI++)
#endif
			{
				CvUnit* pkUnit2 = pkTargetPlot->getUnitByIndex(iI);

				if(pkUnit2 && pkUnit2->AreUnitsOfSameType(*pkUnit))
				{
					// Start the swap
					pkUnit->PushMission(CvTypes::getMISSION_MOVE_TO(), iData1, iData2, MOVE_IGNORE_STACKING, bShift, true);

					// Move the other unit back out, again splitting if necessary
					pkUnit2->PushMission(CvTypes::getMISSION_MOVE_TO(), pkOriginationPlot->getX(), pkOriginationPlot->getY());
				}
			}
		}
	}
	CvUnit::dispatchingNetMessage(false);
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseUpdateCityCitizens(PlayerTypes ePlayer, int iCityID)
{
	CvCity* pCity = GET_PLAYER(ePlayer).getCity(iCityID);
	if(NULL != pCity && pCity->GetCityCitizens())
	{
		CvCityCitizens* pkCitizens = pCity->GetCityCitizens();
		if(pkCitizens != NULL)
		{
			pkCitizens->DoVerifyWorkingPlots();
			pkCitizens->DoReallocateCitizens();
		}
	}
}
//------------------------------------------------------------------------------
void CvDllNetMessageHandler::ResponseUpdatePolicies(PlayerTypes ePlayer, bool bNOTPolicyBranch, int iPolicyID, bool bValue)
{
	CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

	// Policy Update
	if(bNOTPolicyBranch)
	{
		const PolicyTypes ePolicy = static_cast<PolicyTypes>(iPolicyID);
		if(bValue)
		{
			kPlayer.doAdoptPolicy(ePolicy);
		}
		else
		{
			kPlayer.setHasPolicy(ePolicy, bValue);
			kPlayer.DoUpdateHappiness();
#ifdef AUI_CITIZENS_MID_TURN_ASSIGN_RUNS_SELF_CONSISTENCY
			kPlayer.doSelfConsistencyCheckAllCities();
#endif
		}
	}
	// Policy Branch Update
	else
	{
		const PolicyBranchTypes eBranch = static_cast<PolicyBranchTypes>(iPolicyID);
		CvPlayerPolicies* pPlayerPolicies = kPlayer.GetPlayerPolicies();

		// If Branch was blocked by another branch, then unblock this one - this may be the only thing this NetMessage does
		if(pPlayerPolicies->IsPolicyBranchBlocked(eBranch))
		{
			// Can't switch to a Branch that's still locked. DoUnlockPolicyBranch below will handle this for us
			if(pPlayerPolicies->IsPolicyBranchUnlocked(eBranch))
			{
				//pPlayerPolicies->ChangePolicyBranchBlockedCount(eBranch, -1);
				pPlayerPolicies->DoSwitchToPolicyBranch(eBranch);
			}
		}

		// Unlock the branch if it hasn't been already
		if(!pPlayerPolicies->IsPolicyBranchUnlocked(eBranch))
		{
			pPlayerPolicies->DoUnlockPolicyBranch(eBranch);
		}
	}
}
//------------------------------------------------------------------------------
