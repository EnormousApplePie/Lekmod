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

	m_iHappinessPerOriginalCity(0),
	m_iGoldenAgeTurns(0),
	m_iGreatGeneralSiegeBonus(0),
	m_iResistanceTimeReduction(0),
	m_iYieldModCapitalProximity(0),

	m_pbFreePromotion(NULL),
	m_piCityYieldChange(NULL),
	m_piCityYieldModifier(NULL),
	m_piBuildingClassProductionModifier(NULL),
	m_piBuildingClassHappinessChange(NULL),
	m_piBuildingClassGlobalHappinessChange(NULL),
	m_paiUnitResourceRequirementChange(NULL),
	m_piUnitRangedStrengthChange(NULL),
	m_piUnitStrengthChange(NULL),
	m_paiBuildingClassYieldChange(NULL),
	m_paiBuildingClassYieldModifier(NULL),
	m_paiResourceYieldChange(NULL),
	m_paiResourceClassYieldChange(NULL),
	m_paiSpecialistYieldChange(NULL),
	m_piSpecialistHappinessChange(NULL)
{
}
// Destructor
CvLegacyEntry::~CvLegacyEntry(void)
{
	SAFE_DELETE_ARRAY(m_pbFreePromotion);
	SAFE_DELETE_ARRAY(m_piCityYieldChange);
	SAFE_DELETE_ARRAY(m_piCityYieldModifier);
	SAFE_DELETE_ARRAY(m_piBuildingClassProductionModifier);
	SAFE_DELETE_ARRAY(m_piBuildingClassHappinessChange);
	SAFE_DELETE_ARRAY(m_piBuildingClassGlobalHappinessChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiUnitResourceRequirementChange);
	SAFE_DELETE_ARRAY(m_piUnitRangedStrengthChange);
	SAFE_DELETE_ARRAY(m_piUnitStrengthChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiBuildingClassYieldModifier);
	CvDatabaseUtility::SafeDelete2DArray(m_paiResourceYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiResourceClassYieldChange);
	CvDatabaseUtility::SafeDelete2DArray(m_paiSpecialistYieldChange);
	SAFE_DELETE_ARRAY(m_piSpecialistHappinessChange);
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

	m_iHappinessPerOriginalCity = kResults.GetInt("HappinessPerOriginalCity");
	m_iGoldenAgeTurns = kResults.GetInt("GoldenAgeTurns");
	m_iGreatGeneralSiegeBonus = kResults.GetInt("GreatGeneralSiegeBonus");
	m_iResistanceTimeReduction = kResults.GetInt("ResistanceTimeReduction");
	m_iYieldModCapitalProximity = kResults.GetInt("YieldModCapitalProximity");

	// Arrays Start.
	const char* szLegacyType = GetType();
	kUtility.SetYields(m_piCityYieldChange, "Legacy_CityYieldChange", "LegacyType", szLegacyType);
	kUtility.PopulateArrayByValue(m_piCityYieldModifier, "Yields", "Legacy_CityYieldModifier", "YieldType", "LegacyType", szLegacyType, "Modifier");
	kUtility.PopulateArrayByExistence(m_pbFreePromotion, "UnitPromotions", "Legacy_FreePromotions", "PromotionType", "LegacyType", szLegacyType);
	kUtility.PopulateArrayByValue(m_piBuildingClassProductionModifier, "BuildingClasses", "Legacy_BuildingClassProductionModifiers", "BuildingClassType", "LegacyType", szLegacyType, "Modifier");
	kUtility.PopulateArrayByValue(m_piSpecialistHappinessChange, "Specialists", "Legacy_SpecialistHappinessChange", "SpecialistType", "LegacyType", szLegacyType, "HappinessTimes100");
	// Complex/Compound Arrays 
	{
		// Building Class Yields and Yield Modifiers
		kUtility.Initialize2DArray(m_paiBuildingClassYieldChange, "BuildingClasses", "Yields");
		kUtility.Initialize2DArray(m_paiBuildingClassYieldModifier, "BuildingClasses", "Yields");
		std::string strKey("Legacy_BuildingClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, Yields.ID as YieldID, Yield, Modifier FROM Legacy_BuildingClassYieldChange "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iBuildingClass = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);
			const int iYieldModifier = pResults->GetInt(3);
			m_paiBuildingClassYieldChange[iBuildingClass][iYield] = iYieldChange;
			m_paiBuildingClassYieldModifier[iBuildingClass][iYield] = iYieldModifier;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piBuildingClassHappinessChange, "BuildingClasses", 0);
		kUtility.InitializeArray(m_piBuildingClassGlobalHappinessChange, "BuildingClasses", 0);
		std::string strKey("Legacy_BuildingClassHappinessChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT BuildingClasses.ID as BuildingClassID, Happiness, GlobalHappiness FROM Legacy_BuildingClassHappinessChange "
				"INNER JOIN BuildingClasses ON BuildingClasses.Type = BuildingClassType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iBuildingClass = pResults->GetInt(0);
			const int iHappiness = pResults->GetInt(1);
			const int iGlobalHappiness = pResults->GetInt(2);
			m_piBuildingClassHappinessChange[iBuildingClass] = iHappiness;
			m_piBuildingClassGlobalHappinessChange[iBuildingClass] = iGlobalHappiness;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiResourceYieldChange, "Resources", "Yields");
		std::string strKey("Legacy_ResourceYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Resources.ID as ResourceID, Yields.ID as YieldID, Yield FROM Legacy_ResourceYieldChange "
				"INNER JOIN Resources ON Resources.Type = ResourceType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iResource = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);
			m_paiResourceYieldChange[iResource][iYieldID] = iYield;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiResourceClassYieldChange, "ResourceClasses", "Yields");
		std::string strKey("Legacy_ResourceClassYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT ResourceClasses.ID as ResourceClass, Yields.ID as YieldID, Yield FROM Legacy_ResourceClassYieldChange "
				"INNER JOIN ResourceClasses ON ResourceClasses.Type = ResourceClassType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iResourceClass = pResults->GetInt(0);
			const int iYieldID = pResults->GetInt(1);
			const int iYield = pResults->GetInt(2);
			m_paiResourceClassYieldChange[iResourceClass][iYieldID] = iYield;
		}
		pResults->Reset();
	}
	{
		kUtility.Initialize2DArray(m_paiSpecialistYieldChange, "Specialists", "Yields");
		std::string strKey("Legacy_SpecialistYieldChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Specialists.ID as SpecialistID, Yields.ID as YieldID, Yield FROM Legacy_SpecialistYieldChange "
				"INNER JOIN Specialists ON Specialists.Type = SpecialistType "
				"INNER JOIN Yields ON Yields.Type = YieldType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iSpecialist = pResults->GetInt(0);
			const int iYield = pResults->GetInt(1);
			const int iYieldChange = pResults->GetInt(2);
			m_paiSpecialistYieldChange[iSpecialist][iYield] = iYieldChange;
		}
		pResults->Reset();
	}
	{
		std::string sqlKey = "Legacy_FreePromotionsUnitType";
		Database::Results* pResults = kUtility.GetResults(sqlKey);
		if (pResults == NULL)
		{
			const char* szSQL =
				"select UnitPromotions.ID, Units.ID "
				"from Legacy_FreePromotionsUnitType, UnitPromotions, Units "
				"where LegacyType = ? and PromotionType = UnitPromotions.Type and UnitType = Units.Type";
			pResults = kUtility.PrepareResults(sqlKey, szSQL);
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int unitPromotionID = pResults->GetInt(0);
			const int unitCombatInfoID = pResults->GetInt(1);
			m_FreePromotionUnitType.insert(std::pair<int, int>(unitPromotionID, unitCombatInfoID));
		}
		pResults->Reset();

		//Trim extra memory off container since this is mostly read-only.
		std::multimap<int, int>(m_FreePromotionUnitType).swap(m_FreePromotionUnitType);
	}
	{
		kUtility.Initialize2DArray(m_paiUnitResourceRequirementChange, "Units", "Resources");
		std::string strKey("Legacy_UnitResourceRequirementChange");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, Resources.ID as ResourceID, QuantityChange FROM Legacy_UnitResourceRequirementChange "
				"INNER JOIN Units ON Units.Type = UnitType "
				"INNER JOIN Resources ON Resources.Type = ResourceType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iUnit = pResults->GetInt(0);
			const int iResource = pResults->GetInt(1);
			const int iChange = pResults->GetInt(2);
			m_paiUnitResourceRequirementChange[iUnit][iResource] = iChange;
		}
		pResults->Reset();
	}
	{
		kUtility.InitializeArray(m_piUnitRangedStrengthChange, "Units", 0);
		kUtility.InitializeArray(m_piUnitStrengthChange, "Units", 0);
		std::string strKey("Legacy_UnitStrengthChanges");
		Database::Results* pResults = kUtility.GetResults(strKey);
		if (pResults == NULL)
		{
			pResults = kUtility.PrepareResults(strKey,
				"SELECT Units.ID as UnitID, RangedStrengthChange, StrengthChange FROM Legacy_UnitStrengthChanges "
				"INNER JOIN Units ON Units.Type = UnitType "
				"WHERE LegacyType = ?");
		}
		pResults->Bind(1, szLegacyType);
		while (pResults->Step())
		{
			const int iUnit = pResults->GetInt(0);
			const int iRangedStrengthChange = pResults->GetInt(1);
			const int iStrengthChange = pResults->GetInt(2);
			m_piUnitRangedStrengthChange[iUnit] = iRangedStrengthChange;
			m_piUnitStrengthChange[iUnit] = iStrengthChange;
		}
		pResults->Reset();
	}
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
int CvLegacyEntry::GetHappinessPerOriginalCity() const
{
	return m_iHappinessPerOriginalCity;
}
int CvLegacyEntry::GetGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}
int CvLegacyEntry::GetGreatGeneralSiegeBonus() const
{
	return m_iGreatGeneralSiegeBonus;
}
int CvLegacyEntry::GetResistanceTimeReduction() const
{
	return m_iResistanceTimeReduction;
}
int CvLegacyEntry::GetYieldModCapitalProximity() const
{
	return m_iYieldModCapitalProximity;
}
// ARRAYS
int CvLegacyEntry::IsFreePromotion(int i) const
{
	return m_pbFreePromotion ? m_pbFreePromotion[i] : -1;
}
bool CvLegacyEntry::IsFreePromotionUnitType(const int promotionID, const int unitTypeID) const
{
	std::multimap<int, int>::const_iterator it = m_FreePromotionUnitType.find(promotionID);
	if (it != m_FreePromotionUnitType.end())
	{
		// get an iterator to the element that is one past the last element associated with key
		std::multimap<int, int>::const_iterator lastElement = m_FreePromotionUnitType.upper_bound(promotionID);

		// for each element in the sequence [itr, lastElement)
		for (; it != lastElement; ++it)
		{
			if (it->second == unitTypeID)
			{
				return true;
			}
		}
	}

	return false;
}
int CvLegacyEntry::GetCityYieldChange(int i) const
{
	return m_piCityYieldChange ? m_piCityYieldChange[i] : 0;
}
int CvLegacyEntry::GetCityYieldModifier(int i) const
{
	return m_piCityYieldModifier ? m_piCityYieldModifier[i] : 0;
}
int CvLegacyEntry::GetBuildingClassProductionModifier(int i) const
{
	return m_piBuildingClassProductionModifier ? m_piBuildingClassProductionModifier[i] : 0;
}
int CvLegacyEntry::GetBuildingClassHappinessChange(int i) const
{
	return m_piBuildingClassHappinessChange ? m_piBuildingClassHappinessChange[i] : 0;
}
int CvLegacyEntry::GetBuildingClassGlobalHappinessChange(int i) const
{
	return m_piBuildingClassGlobalHappinessChange ? m_piBuildingClassGlobalHappinessChange[i] : 0;
}
int CvLegacyEntry::GetUnitResourceRequirementChange(int i, int j) const
{
	return m_paiUnitResourceRequirementChange ? m_paiUnitResourceRequirementChange[i][j] : 0;
}
int CvLegacyEntry::GetUnitRangedStrengthChange(int i) const
{
	return m_piUnitRangedStrengthChange ? m_piUnitRangedStrengthChange[i] : 0;
}
int CvLegacyEntry::GetUnitStrengthChange(int i) const
{
	return m_piUnitStrengthChange ? m_piUnitStrengthChange[i] : 0;
}
int CvLegacyEntry::GetBuildingClassYieldChange(int i, int j) const
{
	return m_paiBuildingClassYieldChange ? m_paiBuildingClassYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetBuildingClassYieldModifier(int i, int j) const
{
	return m_paiBuildingClassYieldModifier ? m_paiBuildingClassYieldModifier[i][j] : 0;
}
int CvLegacyEntry::GetResourceYieldChange(int i, int j) const
{
	return m_paiResourceYieldChange ? m_paiResourceYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetResourceClassYieldChange(int i, int j) const
{
	return m_paiResourceClassYieldChange ? m_paiResourceClassYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetSpecialistYieldChange(int i, int j) const
{
	return m_paiSpecialistYieldChange ? m_paiSpecialistYieldChange[i][j] : 0;
}
int CvLegacyEntry::GetSpecialistHappinessChange(int i) const
{
	return m_piSpecialistHappinessChange ? m_piSpecialistHappinessChange[i] : 0;
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
//=====================================
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
	m_viCityYieldChange.clear();
	m_viCityYieldModifier.clear();
	m_viUnitRangedStrengthChanges.clear();
	m_viUnitStrengthChanges.clear();
	m_viBuildingClassProductionModifiers.clear();
	m_viBuildingClassHappinessChanges.clear();
	m_viBuildingClassGlobalHappinessChanges.clear();
	m_vaaiBuildingClassYieldChanges.clear();
	m_vaaiBuildingClassYieldModifiers.clear();
	m_vaaiResourceYieldChanges.clear();
	m_vaaiResourceClassYieldChanges.clear();
	m_vaaiSpecialistYieldChanges.clear();
	m_viSpecialistHappinessChanges.clear();
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
	// Gameplay Effects
	m_iHappinessPerOriginalCity = 0;
	m_iGoldenAgeTurns = 0;
	m_iGreatGeneralSiegeBonus = 0;
	m_iResistanceTimeReduction = 0;
	m_iYieldModCapitalProximity = 0;
	
	// Arrays
	// Units
	m_viUnitStrengthChanges.clear();
	m_viUnitStrengthChanges.resize(GC.getNumUnitInfos());
	m_viUnitRangedStrengthChanges.clear();
	m_viUnitRangedStrengthChanges.resize(GC.getNumUnitInfos());
	for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
	{
		m_viUnitStrengthChanges[iUnit] = 0;
		m_viUnitRangedStrengthChanges[iUnit] = 0;
	}
	// BuildingClass
	m_viBuildingClassProductionModifiers.clear();
	m_viBuildingClassProductionModifiers.resize(GC.getNumBuildingClassInfos());
	m_viBuildingClassHappinessChanges.clear();
	m_viBuildingClassHappinessChanges.resize(GC.getNumBuildingClassInfos());
	m_viBuildingClassGlobalHappinessChanges.clear();
	m_viBuildingClassGlobalHappinessChanges.resize(GC.getNumBuildingClassInfos());
	for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
	{
		m_viBuildingClassProductionModifiers[iBuildingClass] = 0;
		m_viBuildingClassHappinessChanges[iBuildingClass] = 0;
		m_viBuildingClassGlobalHappinessChanges[iBuildingClass] = 0;
	}
	// Specialists
	m_viSpecialistHappinessChanges.clear();
	m_viSpecialistHappinessChanges.resize(GC.getNumSpecialistInfos());
	for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
	{
		m_viSpecialistHappinessChanges[iSpecialist] = 0;
	}
	// START YIELD RESETS
	m_viCityYieldChange.clear();
	m_viCityYieldChange.resize(NUM_YIELD_TYPES);
	m_viCityYieldModifier.clear();
	m_viCityYieldModifier.resize(NUM_YIELD_TYPES);
	m_vaaiBuildingClassYieldChanges.clear();
	m_vaaiBuildingClassYieldChanges.resize(GC.getNumBuildingClassInfos());
	m_vaaiBuildingClassYieldModifiers.clear();
	m_vaaiBuildingClassYieldModifiers.resize(GC.getNumBuildingClassInfos());
	m_vaaiResourceYieldChanges.clear();
	m_vaaiResourceYieldChanges.resize(GC.getNumResourceInfos());
	m_vaaiResourceClassYieldChanges.clear();
	m_vaaiResourceClassYieldChanges.resize(GC.getNumResourceClassInfos());
	m_vaaiSpecialistYieldChanges.clear();
	m_vaaiSpecialistYieldChanges.resize(GC.getNumSpecialistInfos());

	Firaxis::Array< int, NUM_YIELD_TYPES > yield;
	for (unsigned int j = 0; j < NUM_YIELD_TYPES; ++j)
	{
		yield[j] = 0;
	}
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		m_viCityYieldChange[iYield] = 0;
		m_viCityYieldModifier[iYield] = 0;
		// BuildingClass
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
		{
			m_vaaiBuildingClassYieldChanges[iBuildingClass] = yield;
			m_vaaiBuildingClassYieldModifiers[iBuildingClass] = yield;
		}
		// Specialist
		for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			m_vaaiSpecialistYieldChanges[iSpecialist] = yield;
		}
		// Resource
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			m_vaaiResourceYieldChanges[iResource] = yield;
		}
		// ResourceClass
		for (int iResourceClass = 0; iResourceClass < GC.getNumResourceClassInfos(); iResourceClass++)
		{
			m_vaaiResourceClassYieldChanges[iResourceClass] = yield;
		}
	} // END NUM_YIELD_TYPES LOOP

	// Reset the AI
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
	kStream >> m_iHappinessPerOriginalCity;
	kStream >> m_iGoldenAgeTurns;
	kStream >> m_iGreatGeneralSiegeBonus;
	kStream >> m_iResistanceTimeReduction;
	kStream >> m_iYieldModCapitalProximity;

	kStream >> m_viCityYieldChange;
	kStream >> m_viCityYieldModifier;
	kStream >> m_viUnitRangedStrengthChanges;
	kStream >> m_viUnitStrengthChanges;
	kStream >> m_viBuildingClassProductionModifiers;
	kStream >> m_viBuildingClassHappinessChanges;
	kStream >> m_viBuildingClassGlobalHappinessChanges;
	kStream >> m_vaaiBuildingClassYieldChanges;
	kStream >> m_vaaiBuildingClassYieldModifiers;
	kStream >> m_vaaiResourceYieldChanges;
	kStream >> m_vaaiResourceClassYieldChanges;
	kStream >> m_vaaiSpecialistYieldChanges;
	kStream >> m_viSpecialistHappinessChanges;

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
	kStream << m_iHappinessPerOriginalCity;
	kStream << m_iGoldenAgeTurns;
	kStream << m_iGreatGeneralSiegeBonus;
	kStream << m_iResistanceTimeReduction;
	kStream << m_iYieldModCapitalProximity;

	kStream << m_viCityYieldChange;
	kStream << m_viCityYieldModifier;
	kStream << m_viUnitRangedStrengthChanges;
	kStream << m_viUnitStrengthChanges;
	kStream << m_viBuildingClassProductionModifiers;
	kStream << m_viBuildingClassHappinessChanges;
	kStream << m_viBuildingClassGlobalHappinessChanges;
	kStream << m_vaaiBuildingClassYieldChanges;
	kStream << m_vaaiBuildingClassYieldModifiers;
	kStream << m_vaaiResourceYieldChanges;
	kStream << m_vaaiResourceClassYieldChanges;
	kStream << m_vaaiSpecialistYieldChanges;
	kStream << m_viSpecialistHappinessChanges;

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
		updatePlayerLegacies(eIndex); // Loads the legacy effects into the player legacy object
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
// Give the player a notification to choose a legacy
void CvPlayerLegacies::testLegacyNotification() const
{
	if (m_pPlayer->GetNumFreeLegacies() > 0) // Has a legacy to choose
	{
		CvNotifications* pNotifications = m_pPlayer->GetNotifications();
		if (pNotifications)
		{
			CvString strBuffer = GetLocalizedText("TXT_KEY_NOTIFICATION_CHOOSE_LEGACY", GC.getEraInfo(m_pPlayer->GetCurrentEra())->GetTextKey());
			CvString strSummary = GetLocalizedText("TXT_KEY_NOTIFICATION_CHOOSE_LEGACY_SUMMARY");
			pNotifications->Add((NotificationTypes)NOTIFICATION_CHOOSE_LEGACY, strBuffer, strSummary, -1, -1, m_pPlayer->GetID());
		}
	}
}
// Load Legacy Entry info into the PlayerLegacies object for access
void CvPlayerLegacies::updatePlayerLegacies(LegacyTypes eLegacy)
{
	CvLegacyEntry* pkLegacyEntry = GC.getLegacyInfo(eLegacy);
	if (pkLegacyEntry == NULL)
		return;
	int iChange = 0;
	const CvLegacyEntry& kLegacy = (*pkLegacyEntry);
	m_iHappinessPerOriginalCity += kLegacy.GetHappinessPerOriginalCity();
	m_iGoldenAgeTurns += kLegacy.GetGoldenAgeTurns();
	m_iGreatGeneralSiegeBonus += kLegacy.GetGreatGeneralSiegeBonus();
	m_iResistanceTimeReduction += kLegacy.GetResistanceTimeReduction();
	m_iYieldModCapitalProximity += kLegacy.GetYieldModCapitalProximity();
	for (int iYield = 0; iYield < NUM_YIELD_TYPES; iYield++)
	{
		iChange = kLegacy.GetCityYieldChange((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viCityYieldChange[iYield] += iChange;
		}
		iChange = kLegacy.GetCityYieldModifier((YieldTypes)iYield);
		if (iChange != 0)
		{
			m_viCityYieldModifier[iYield] += iChange;
		}
		// BuildingClass
		for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
		{
			iChange = kLegacy.GetBuildingClassYieldChange((BuildingClassTypes)iBuildingClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiBuildingClassYieldChanges[iBuildingClass][iYield] += iChange;
			}
			iChange = kLegacy.GetBuildingClassYieldModifier((BuildingClassTypes)iBuildingClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiBuildingClassYieldModifiers[iBuildingClass][iYield] += iChange;
			}
		}
		// Specialist
		for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
		{
			iChange = kLegacy.GetSpecialistYieldChange((SpecialistTypes)iSpecialist, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiSpecialistYieldChanges[iSpecialist][iYield] += iChange;
			}
		}
		// Resource
		for (int iResource = 0; iResource < GC.getNumResourceInfos(); iResource++)
		{
			iChange = kLegacy.GetResourceYieldChange((ResourceTypes)iResource, (YieldTypes)iYield);
			if(iChange != 0)
			{
				m_vaaiResourceYieldChanges[iResource][iYield] += iChange;
			}
		}
		// ResourceClass
		for (int iResourceClass = 0; iResourceClass < GC.getNumResourceClassInfos(); iResourceClass++)
		{
			iChange = kLegacy.GetResourceClassYieldChange((ResourceClassTypes)iResourceClass, (YieldTypes)iYield);
			if (iChange != 0)
			{
				m_vaaiResourceClassYieldChanges[iResourceClass][iYield] += iChange;
			}
		}
	} // END NUM_YIELD_TYPES LOOP
	// BuildingClass - Non-yield
	for (int iBuildingClass = 0; iBuildingClass < GC.getNumBuildingClassInfos(); iBuildingClass++)
	{
		m_viBuildingClassProductionModifiers[iBuildingClass] += kLegacy.GetBuildingClassProductionModifier(iBuildingClass);
		m_viBuildingClassHappinessChanges[iBuildingClass] += kLegacy.GetBuildingClassHappinessChange(iBuildingClass);
		m_viBuildingClassGlobalHappinessChanges[iBuildingClass] += kLegacy.GetBuildingClassGlobalHappinessChange(iBuildingClass);
		
	}
	// Specialist - Non-yield
	for (int iSpecialist = 0; iSpecialist < GC.getNumSpecialistInfos(); iSpecialist++)
	{
		iChange = kLegacy.GetSpecialistHappinessChange(iSpecialist);
		if (iChange != 0)
		{
			m_viSpecialistHappinessChanges[iSpecialist] += iChange;
		}
	}
	// Units - Non-yield
	for (int iUnit = 0; iUnit < GC.getNumUnitInfos(); iUnit++)
	{
		m_viUnitRangedStrengthChanges[iUnit] += kLegacy.GetUnitRangedStrengthChange(iUnit);
		m_viUnitStrengthChanges[iUnit] += kLegacy.GetUnitStrengthChange(iUnit);
	}
}
// How Much happiness per original city does the player get from legacies
int CvPlayerLegacies::GetHappinessPerOriginalCity() const
{
	return m_iHappinessPerOriginalCity;
}
// How many golden age turns does the player get from legacies
int CvPlayerLegacies::GetGoldenAgeTurns() const
{
	return m_iGoldenAgeTurns;
}
// How much siege bonus does the player get for great generals from legacies
int CvPlayerLegacies::GetGreatGeneralSiegeBonus() const
{
	return m_iGreatGeneralSiegeBonus;
}
// How much resistance time reduction does the player get from legacies
int CvPlayerLegacies::GetResistanceTimeReduction() const
{
	return m_iResistanceTimeReduction;
}
// Change the behavoir of the CityYieldMod table based on this value
int CvPlayerLegacies::GetYieldModCapitalProximity() const
{
	return m_iYieldModCapitalProximity;
}
bool CvPlayerLegacies::HasFreePromotionUnitType(PromotionTypes ePromotion, UnitTypes eUnitType) const
{
	CvAssertMsg((promotionID >= 0), "promotionID is less than zero");
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				if (pkLegacy->IsFreePromotionUnitType(ePromotion, eUnitType))
				{
					return true;
				}
			}
		}
	}

	return false;
}
int CvPlayerLegacies::GetCityYieldChange(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eYield != NO_YIELD ? m_viCityYieldChange[eYield] : 0;
}
int CvPlayerLegacies::GetCityYieldModifier(YieldTypes eYield) const
{
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eYield != NO_YIELD ? m_viCityYieldModifier[eYield] : 0;
}
// How much production modifier does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassProductionModifier(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassProductionModifiers[(int)eBuildingClass] : 0;
}
int CvPlayerLegacies::GetBuildingClassHappinessChange(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassHappinessChanges[(int)eBuildingClass] : 0;
}
int CvPlayerLegacies::GetBuildingClassGlobalHappinessChange(BuildingClassTypes eBuildingClass) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_viBuildingClassGlobalHappinessChanges[(int)eBuildingClass] : 0;
}
// How much resource requirement change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitResourceRequirementChange(UnitTypes eUnit, ResourceTypes eResource) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	for (int iI = 0; iI < GC.getNumLegacyInfos(); iI++)
	{
		const LegacyTypes eLegacy = static_cast<LegacyTypes>(iI);
		CvLegacyEntry* pkLegacy = GC.getLegacyInfo(eLegacy);
		if (pkLegacy)
		{
			if (HasLegacy(eLegacy))
			{
				int iChange = pkLegacy->GetUnitResourceRequirementChange(eUnit, eResource);
				if (iChange != 0)
				{
					return iChange;
				}
			}
		}
	}
	return 0;
}
// How much ranged strength change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitRangedStrengthChange(UnitTypes eUnit) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	return eUnit != NO_UNIT ? m_viUnitRangedStrengthChanges[(int)eUnit] : 0;
}
// How much strength change does the player get for a unit from legacies
int CvPlayerLegacies::GetUnitStrengthChange(UnitTypes eUnit) const
{
	CvAssertMsg(eUnit >= 0 && eUnit < GC.getNumUnitInfos(), "Unit index out of bounds");
	return eUnit != NO_UNIT ? m_viUnitStrengthChanges[(int)eUnit] : 0;
}
// How much yield change does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassYieldChange(BuildingClassTypes eBuildingClass, YieldTypes eYield) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_vaaiBuildingClassYieldChanges[(int)eBuildingClass][(int)eYield] : 0;
}
// How much yield modifier does the player get for a building class from legacies
int CvPlayerLegacies::GetBuildingClassYieldModifier(BuildingClassTypes eBuildingClass, YieldTypes eYield) const
{
	CvAssertMsg(eBuildingClass >= 0 && eBuildingClass < GC.getNumBuildingClassInfos(), "BuildingClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eBuildingClass != NO_BUILDINGCLASS ? m_vaaiBuildingClassYieldModifiers[(int)eBuildingClass][(int)eYield] : 0;
}
// How much of eYield does eResource get from Legacies?
int CvPlayerLegacies::GetResourceYieldChange(ResourceTypes eResource, YieldTypes eYield) const
{
	CvAssertMsg(eResource >= 0 && eResource < GC.getNumResourceInfos(), "Resource index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eResource != NO_RESOURCE ? m_vaaiResourceYieldChanges[(int)eResource][(int)eYield] : 0;
}
// How much of eYield does eResourceClass get from Legacies?
int CvPlayerLegacies::GetResourceClassYieldChange(ResourceClassTypes eResourceClass, YieldTypes eYield) const
{
	CvAssertMsg(eResourceClass >= 0 && eResourceClass < GC.getNumResourceClaasInfos(), "ResourceClass index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eResourceClass != NO_RESOURCECLASS ? m_vaaiResourceClassYieldChanges[(int)eResourceClass][(int)eYield] : 0;
}
// How much yield change does the player get for a specialist from legacies
int CvPlayerLegacies::GetSpecialistYieldChange(SpecialistTypes eSpecialist, YieldTypes eYield) const
{
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "Specialist index out of bounds");
	CvAssertMsg(eYield >= 0 && eYield < NUM_YIELD_TYPES, "Yield index out of bounds");
	return eSpecialist != NO_SPECIALIST ? m_vaaiSpecialistYieldChanges[(int)eSpecialist][(int)eYield] : 0;
}
// How much happiness change does the player get for a specialist from legacies
int CvPlayerLegacies::GetSpecialistHappinessChange(SpecialistTypes eSpecialist) const
{
	CvAssertMsg(eSpecialist >= 0 && eSpecialist < GC.getNumSpecialistInfos(), "Specialist index out of bounds");
	return eSpecialist != NO_SPECIALIST ? m_viSpecialistHappinessChanges[(int)eSpecialist] : 0;
}
void CvPlayerLegacies::DoLegacyAI()
{
	//
}
// Choose a legacy for the AI Player
void CvPlayerLegacies::DoChooseLegacy()
{
	m_pLegacyAI->ChooseLegacy(m_pPlayer);
}
#endif // LEKMOD_LEGACY