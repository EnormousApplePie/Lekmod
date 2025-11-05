/*  -------------------------------------------------------------------------------------------------------
This is an experimental system to spice up the game without touching the core of civilizations.
It allows for the addition of "legacies" that can be granted to civilizations, providing unique bonuses or abilities based on historical or thematic elements.
This is mimicking the policy system in implementation, so reference that.
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvLegacyAI.h"
#include "CvFlavorManager.h"
#include "ICvDLLUserInterface.h"
#include "CvGameCoreUtils.h"
#include "CvEconomicAI.h"
#include "CvGrandStrategyAI.h"
#include "CvInfosSerializationHelper.h"

#include "LintFree.h"
#if defined(LEKMOD_LEGACY)
// Constructor
CvLegacyEntry::CvLegacyEntry(void) :
	m_iCivilization(NO_CIVILIZATION),
	m_iEraOffered(NO_ERA),

	m_piCityYieldChange(NULL)
{
}
// Destructor
CvLegacyEntry::~CvLegacyEntry(void)
{
	SAFE_DELETE_ARRAY(m_piCityYieldChange);
}

// Cache results from the database
bool CvLegacyEntry::CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility)
{
	// Base info consists of ID, Type, Text, Civilopedia, Strategy, Help, DisabledHelp, Description.
	if (!CvBaseInfo::CacheResults(kResults, kUtility))
		return false;
	
	const char* szCivilization = kResults.GetText("CivilizationType");
	m_iCivilization = GC.getInfoTypeForString(szCivilization, true);
	const char* szEra = kResults.GetText("EraOffered");
	m_iEraOffered = GC.getInfoTypeForString(szEra, true);

	// Arrays Start.
	const char* szLegacyType = GetType();
	kUtility.SetYields(m_piCityYieldChange, "Legacy_CityYieldChange", "LegacyType", szLegacyType);
	// Return true at the very end.
	return true;
}
// What civilization is the legacy offered to?
int CvLegacyEntry::GetCivilization() const
{
	return m_iCivilization;
}
// At what era is this Legacy offered to the player?
int CvLegacyEntry::GetEraOffered() const
{
	return m_iEraOffered;
}

// ARRAYS
int CvLegacyEntry::GetCityYieldChange(int i) const
{
	return m_piCityYieldChange ? m_piCityYieldChange[i] : 0;
}
//=====================================
// CvLegacyXMLEntries
//=====================================
/// Constructor
CvLegacyXMLEntries::CvLegacyXMLEntries(void)
{
}

/// Destructor
CvLegacyXMLEntries::~CvLegacyXMLEntries(void)
{
	DeleteLegacyArray();
}

// Get the vector of legacy entries
std::vector<CvLegacyEntry*>& CvLegacyXMLEntries::GetLegacyEntries()
{
	return m_paLegacyEntries;
}

// Get the number of legacies
int CvLegacyXMLEntries::GetNumLegacies()
{
	return m_paLegacyEntries.size();
}

void CvLegacyXMLEntries::DeleteLegacyArray()
{
	for (std::vector<CvLegacyEntry*>::iterator it = m_paLegacyEntries.begin(); it != m_paLegacyEntries.end(); ++it)
	{
		SAFE_DELETE(*it);
	}

	m_paLegacyEntries.clear();
}

CvLegacyEntry* CvLegacyXMLEntries::GetLegacyEntry(int iIndex)
{
	return m_paLegacyEntries[iIndex];
}

//=====================================
// CvPlayerLegacies
// //=====================================
/// Constructor
CvPlayerLegacies::CvPlayerLegacies(void):
	m_pabHasLegacy(NULL),
	m_pLegacies(NULL),
	m_pLegacyAI(NULL),
	m_pPlayer(NULL)
{
}
// Destructor
CvPlayerLegacies::~CvPlayerLegacies(void)
{
}
// Initialization
void CvPlayerLegacies::Init(CvLegacyXMLEntries* pLegacies, CvPlayer* pPlayer)
{
	// Store Pointers.
	m_pLegacies = pLegacies;
	m_pPlayer = pPlayer;

	// Allocate memory.
	m_pabHasLegacy = FNEW(bool[m_pLegacies->GetNumLegacies()], c_eCiv5GameplayDLL, 0);
	m_pLegacyAI = FNEW(CvLegacyAI(this), c_eCiv5GameplayDLL, 0);

	Reset();

	m_vCivHasLegacy.resize(GC.getNumLegacyInfos(), false);
}
// Uninitialization
void CvPlayerLegacies::Uninit()
{
	SAFE_DELETE_ARRAY(m_pabHasLegacy);
	SAFE_DELETE(m_pLegacyAI);
}
// Reset
void CvPlayerLegacies::Reset()
{
	m_vCivHasLegacy.resize(GC.getNumLegacyInfos(), false);
	m_vPotentiallyActiveCivLegacies.clear();

	if (m_pabHasLegacy)
	{
		for (int i = 0; i < m_pLegacies->GetNumLegacies(); i++)
		{
			m_pabHasLegacy[i] = false;
		}
	}

	m_pLegacyAI->Reset();
}
// Read
void CvPlayerLegacies::Read(FDataStream& kStream)
{
	// Read the data from the stream
	CvAssertMsg(m_pLegacies != NULL && GC.getNumLegacyInfos() > 0, "Number of legacies to serialize is expected to greater than 0");
	uint uiLegacyCount = 0;
	if (m_pLegacies)
	{
		uiLegacyCount = m_pLegacies->GetNumLegacies();
	}

	CvInfosSerializationHelper::ReadHashedDataArray(kStream, m_pabHasLegacy, uiLegacyCount);

	m_pLegacyAI->Read(kStream);
}
// Write
void CvPlayerLegacies::Write(FDataStream& kStream) const
{
	// Write the data to the stream
	CvAssertMsg(m_pLegacies != NULL && GC.getNumLegacyInfos() > 0, "Number of legacies to serialize is expected to greater than 0");
	uint uiLegacyCount = 0;
	if (m_pLegacies)
	{
		uiLegacyCount = m_pLegacies->GetNumLegacies();
	}

	CvInfosSerializationHelper::WriteHashedDataArray<LegacyTypes>(kStream, m_pabHasLegacy, uiLegacyCount);
	m_pLegacyAI->Write(kStream);
}
// Accessors
// Get the player
CvPlayer* CvPlayerLegacies::GetPlayer() const
{
	return m_pPlayer;
}
// Check if the player has a specific legacy
bool CvPlayerLegacies::HasLegacy(LegacyTypes eIndex) const
{
	CvAssertMsg(eIndex >= 0 && eIndex < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	return m_pabHasLegacy[eIndex];
}
// Set a legacy for the player
void CvPlayerLegacies::SetLegacy(LegacyTypes eIndex, bool bNewValue)
{
	CvAssertMsg(eIndex >= 0 && eIndex < GC.getNumLegacyInfos(), "Legacy index out of bounds");
	CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(eIndex);
	if (pkLegacy == NULL)
		return;

	if (HasLegacy(eIndex) != bNewValue)
	{
		m_pabHasLegacy[eIndex] = bNewValue;
		m_pPlayer->processLegacies(eIndex, bNewValue ? 1 : -1);
	}
	// Notify others
	for (int iNotifyLoop = 0; iNotifyLoop < MAX_MAJOR_CIVS; ++iNotifyLoop) {
		PlayerTypes eNotifyPlayer = (PlayerTypes)iNotifyLoop;
		CvPlayerAI& kCurNotifyPlayer = GET_PLAYER(eNotifyPlayer);

		// Issue notification if OTHER than target player
		if (m_pPlayer->GetID() != eNotifyPlayer)
		{
			CvTeam& kNotifyTeam = GET_TEAM(kCurNotifyPlayer.getTeam());
			const bool bHasMet = kNotifyTeam.isHasMet(m_pPlayer->getTeam());

			CvNotifications* pNotifications = kCurNotifyPlayer.GetNotifications();
			if (pNotifications)
			{
				CvString strBuffer;
				if (bHasMet)
				{
					strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_IDEOLOGY_CHOSEN", m_pPlayer->getCivilizationShortDescriptionKey(), pkLegacy->GetDescriptionKey());
				}
				else
				{
					strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_IDEOLOGY_CHOSEN_UNMET", pkLegacy->GetDescriptionKey());
				}

				CvString strSummary;
				strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_SUMMARY_IDEOLOGY_CHOSEN");

				pNotifications->Add((NotificationTypes)NOTIFICATION_LEGACY_CHOSEN, strBuffer, strSummary, -1, -1, m_pPlayer->GetID());
			}
		}
	}
}
// Get Legacy data from the XML
CvLegacyXMLEntries* CvPlayerLegacies::GetLegacies() const
{
	return m_pLegacies;
}
// Get the Legacies for this civ that have not been chosen.
std::vector<LegacyTypes> CvPlayerLegacies::GetPotentialCivLegacies() const
{
	std::vector<LegacyTypes> result;
	for (int iLegacy = 0; iLegacy < GC.getNumLegacyInfos(); iLegacy++)
	{
		CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(iLegacy);
		if (pkLegacy && pkLegacy->GetCivilization() == m_pPlayer->getCivilizationType() && !HasLegacy((LegacyTypes)iLegacy))
		{
			result.push_back((LegacyTypes)iLegacy);
		}
	}
	return result;
}
// Check if the player can choose a specific legacy
bool CvPlayerLegacies::CanChooseLegacy(LegacyTypes eLegacy) const
{
	CvAssertMsg(eLegacy >= 0, "eIndex is expected to be non-negative (invalid Index)");
	CvAssertMsg(eLegacy < GC.getNumLegacyInfos(), "eIndex is expected to be within maximum bounds (invalid Index)");

	CvLegacyEntry* pkLegacyEntry = GC.getLegacyInfo(eLegacy);
	if (pkLegacyEntry == NULL)
		return false;

	// Already has Legacy?
	if (HasLegacy(eLegacy))
	{
		return false;
	}

	ICvEngineScriptSystem1* pkScriptSystem = gDLL->GetScriptSystem();
	if (pkScriptSystem)
	{
		CvLuaArgsHandle args;
		args->Push(m_pPlayer->GetID());
		args->Push(eLegacy);

		// Attempt to execute the game events.
		// Will return false if there are no registered listeners.
		bool bResult = false;
		if (LuaSupport::CallTestAll(pkScriptSystem, "PlayerCanChooseLegacy", args.get(), bResult))
		{
			// Check the result.
			if (bResult == false)
			{
				return false;
			}
		}
	}

	return true;
}
// Check if it's time to choose a legacy
bool CvPlayerLegacies::IsTimeToChooseLegacy() const
{
	if (m_pLegacies == NULL)
		return false;
	EraTypes eCurrentEra = m_pPlayer->GetCurrentEra();
	for (int i = 0; i < GC.getNumLegacyInfos(); i++)
	{
		CvLegacyEntry* pkLegacy = m_pLegacies->GetLegacyEntry(i);
		if (pkLegacy && pkLegacy->GetCivilization() == m_pPlayer->getCivilizationType() && pkLegacy->GetEraOffered() == eCurrentEra)
		{
			return true;
		}
	}
	return false;
}
// Choose a legacy for the AI Player
void CvPlayerLegacies::DoChooseLegacy()
{
	m_pLegacyAI->ChooseLegacy(m_pPlayer);
}





#endif // LEKMOD_LEGACY