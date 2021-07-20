/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvBuildingProductionAI.h"
#include "CvInfosSerializationHelper.h"

// include after all other headers
#include "LintFree.h"

/// Constructor
CvBuildingProductionAI::CvBuildingProductionAI(CvCity* pCity, CvCityBuildings* pCityBuildings):
	m_pCity(pCity),
	m_pCityBuildings(pCityBuildings)
{
}

/// Destructor
CvBuildingProductionAI::~CvBuildingProductionAI(void)
{
}

/// Clear out AI local variables
void CvBuildingProductionAI::Reset()
{
	CvAssertMsg(m_pCityBuildings != NULL, "Building Production AI init failure: city buildings are NULL");

	m_BuildingAIWeights.clear();

	// Loop through reading each one and add an entry with 0 weight to our vector
	if(m_pCityBuildings)
	{
#ifdef AUI_WARNING_FIXES
		for (uint i = 0; i < m_pCityBuildings->GetBuildings()->GetNumBuildings(); i++)
#else
		for(int i = 0; i < m_pCityBuildings->GetBuildings()->GetNumBuildings(); i++)
#endif
		{
			m_BuildingAIWeights.push_back(i, 0);
		}
	}
}

/// Serialization read
void CvBuildingProductionAI::Read(FDataStream& kStream)
{
	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;

	// Reset vector
	m_BuildingAIWeights.clear();

	// Loop through reading each one and adding it to our vector
	if(m_pCityBuildings)
	{
#ifdef AUI_WARNING_FIXES
		for (uint i = 0; i < m_pCityBuildings->GetBuildings()->GetNumBuildings(); i++)
#else
		for(int i = 0; i < m_pCityBuildings->GetBuildings()->GetNumBuildings(); i++)
#endif
		{
			m_BuildingAIWeights.push_back(i, 0);
		}

		int iNumEntries;
		FStringFixedBuffer(sTemp, 64);
		int iType;

		kStream >> iNumEntries;

		for(int iI = 0; iI < iNumEntries; iI++)
		{
			bool bValid = true;
			iType = CvInfosSerializationHelper::ReadHashed(kStream, &bValid);
			if(iType != -1 || !bValid)
			{
				int iWeight;
				kStream >> iWeight;
				if(iType != -1)
				{
					m_BuildingAIWeights.IncreaseWeight(iType, iWeight);
				}
				else
				{
					CvString szError;
					szError.Format("LOAD ERROR: Building Type not found");
					GC.LogMessage(szError.GetCString());
					CvAssertMsg(false, szError);
				}
			}
		}
	}
	else
	{
		CvAssertMsg(m_pCityBuildings != NULL, "Building Production AI init failure: city buildings are NULL");
	}
}

/// Serialization write
void CvBuildingProductionAI::Write(FDataStream& kStream)
{
	CvAssertMsg(m_pCityBuildings != NULL, "Building Production AI init failure: city buildings are NULL");

	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;

	if(m_pCityBuildings)
	{
		int iNumBuildings = m_pCityBuildings->GetBuildings()->GetNumBuildings();
		kStream << iNumBuildings;

		// Loop through writing each entry
		for(int iI = 0; iI < iNumBuildings; iI++)
		{
			const BuildingTypes eBuilding = static_cast<BuildingTypes>(iI);
			CvBuildingEntry* pkBuildingInfo = GC.getBuildingInfo(eBuilding);
			if(pkBuildingInfo)
			{
				CvInfosSerializationHelper::WriteHashed(kStream, pkBuildingInfo);
				kStream << m_BuildingAIWeights.GetWeight(iI);
			}
			else
			{
				kStream << (int)0;
			}
		}
	}
}

/// Establish weights for one flavor; can be called multiple times to layer strategies
void CvBuildingProductionAI::AddFlavorWeights(FlavorTypes eFlavor, int iWeight)
{
#ifdef AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS
	CvPlayer* pPlayer = m_pCity->GetPlayer();
	CvPlayerPolicies* pPlayerPolicies = NULL;
	if (pPlayer)
		pPlayerPolicies = pPlayer->GetPlayerPolicies();
#endif
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
	const CvReligion* pReligion = GC.getGame().GetGameReligions()->GetReligion(m_pCity->GetCityReligions()->GetReligiousMajority(), m_pCity->getOwner());
#endif
	CvBuildingXMLEntries* pkBuildings = m_pCityBuildings->GetBuildings();

	// Loop through all buildings
#ifdef AUI_WARNING_FIXES
	for (uint iBuilding = 0; iBuilding < m_pCityBuildings->GetBuildings()->GetNumBuildings(); iBuilding++)
#else
	for(int iBuilding = 0; iBuilding < m_pCityBuildings->GetBuildings()->GetNumBuildings(); iBuilding++)
#endif
	{
		CvBuildingEntry* entry = pkBuildings->GetEntry(iBuilding);
		if(entry)
		{
			// Set its weight by looking at building's weight for this flavor and using iWeight multiplier passed in
#if defined(AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS) || defined(AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS) || defined(AUI_BUILDING_PRODUCTION_AI_LUA_FLAVOR_WEIGHTS) || defined(AUI_BUILDING_PRODUCTION_AI_CONSIDER_FREE_STUFF)
			int iFlavorValue = entry->GetFlavorValue(eFlavor);
#endif
#ifdef AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS
			if (pPlayerPolicies)
			{
				for (int iI = 0; iI < GC.getNumPolicyInfos(); iI++)
				{
					PolicyTypes ePolicy = static_cast<PolicyTypes>(iI);
					CvPolicyEntry* pPolicy = GC.getPolicyInfo(ePolicy);
					if (pPolicy && pPlayerPolicies->HasPolicy(ePolicy))
					{
						iFlavorValue += pPolicy->GetBuildingClassFlavorChanges(entry->GetBuildingClassType(), eFlavor);
					}
				}
			}
#endif
#ifdef AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS
			if (pReligion)
			{
				pReligion->m_Beliefs.GetBuildingClassFlavorChange(static_cast<BuildingClassTypes>(entry->GetBuildingClassType()), eFlavor);
			}
#endif
#ifdef AUI_BUILDING_PRODUCTION_AI_CONSIDER_FREE_STUFF
#ifdef AUI_WARNING_FIXES
			for (uint iI = 0; iI < GC.getNumUnitInfos(); iI++)
#else
			for (int iI = 0; iI < GC.getNumUnitInfos(); iI++)
#endif
			{
				int iNumFreeUnits = entry->GetNumFreeUnits(iI);
				if (iNumFreeUnits > 0)
				{
					iFlavorValue += iNumFreeUnits * m_pCity->GetCityStrategyAI()->GetUnitProductionAI()->GetWeight((UnitTypes)iI);
				}
			}
#endif
#if defined(AUI_POLICY_BUILDING_CLASS_FLAVOR_MODIFIERS) || defined(AUI_BELIEF_BUILDING_CLASS_FLAVOR_MODIFIERS) || defined(AUI_BUILDING_PRODUCTION_AI_LUA_FLAVOR_WEIGHTS) || defined(AUI_BUILDING_PRODUCTION_AI_CONSIDER_FREE_STUFF)
			m_BuildingAIWeights.IncreaseWeight(iBuilding, iFlavorValue * iWeight);
#else
			m_BuildingAIWeights.IncreaseWeight(iBuilding, entry->GetFlavorValue(eFlavor) * iWeight);
#endif
		}
	}
}

/// Retrieve sum of weights on one item
int CvBuildingProductionAI::GetWeight(BuildingTypes eBuilding)
{
#ifdef AUI_BUILDING_PRODUCTION_AI_CONSIDER_FREE_STUFF
	CvBuildingXMLEntries* pkBuildings = m_pCityBuildings->GetBuildings();
	int iWeight = m_BuildingAIWeights.GetWeight(eBuilding);
	CvBuildingEntry* entry = pkBuildings->GetEntry(eBuilding);
	if (entry)
	{
		CvPlayer* pPlayer = m_pCity->GetPlayer();
		int iLoop = 0;

		BuildingTypes eFreeBuildingThisCity = static_cast<BuildingTypes>(entry->GetFreeBuildingThisCity());
		if (eFreeBuildingThisCity != NO_BUILDING)
		{
			if (m_pCityBuildings->GetNumBuilding(eFreeBuildingThisCity) == 0)
				iWeight += m_BuildingAIWeights.GetWeight(eFreeBuildingThisCity);
		}

		BuildingClassTypes eFreeBuildingClassAllCities = static_cast<BuildingClassTypes>(entry->GetFreeBuildingClass());
		if (eFreeBuildingClassAllCities != NO_BUILDINGCLASS)
		{
			BuildingTypes eFreeBuilding = static_cast<BuildingTypes>(m_pCity->getCivilizationInfo().getCivilizationBuildings(eFreeBuildingClassAllCities));
			for (CvCity* pLoopCity = pPlayer->firstCity(&iLoop); pLoopCity != NULL; pLoopCity = pPlayer->nextCity(&iLoop))
			{
				if (pLoopCity->GetCityBuildings()->GetNumBuilding(eFreeBuilding) == 0)
					iWeight += pLoopCity->GetCityStrategyAI()->GetBuildingProductionAI()->GetWeight(eFreeBuilding);
			}
		}

#ifdef AUI_WARNING_FIXES
		for (uint iI = 0; iI < GC.getNumUnitInfos(); iI++)
#else
		for (int iI = 0; iI < GC.getNumUnitInfos(); iI++)
#endif
		{
			int iNumFreeUnits = entry->GetNumFreeUnits(iI);
			if (iNumFreeUnits > 0)
			{
				iWeight += iNumFreeUnits * m_pCity->GetCityStrategyAI()->GetUnitProductionAI()->GetWeight((UnitTypes)iI);
			}
		}

		if (entry->GetInstantMilitaryIncrease())
		{
			FFastVector<UnitTypes, true, c_eCiv5GameplayDLL> aExtraUnits;
			for (CvUnit* pLoopUnit = pPlayer->firstUnit(&iLoop); pLoopUnit != NULL; pLoopUnit = pPlayer->nextUnit(&iLoop))
			{
				if (pLoopUnit->getDomainType() == DOMAIN_LAND && pLoopUnit->IsCombatUnit())
				{
					UnitTypes eCurrentUnitType = pLoopUnit->getUnitType();

					// check for duplicate unit
					bool bAddUnit = true;
					for (uint ui = 0; ui < aExtraUnits.size(); ui++)
					{
						if (aExtraUnits[ui] == eCurrentUnitType)
						{
							bAddUnit = false;
							break;
						}
					}
					if (bAddUnit)
					{
						aExtraUnits.push_back(eCurrentUnitType);
					}
				}
			}
			for (uint ui = 0; ui < aExtraUnits.size(); ui++)
			{
				iWeight += m_pCity->GetCityStrategyAI()->GetUnitProductionAI()->GetWeight(aExtraUnits[ui]);
			}
		}
	}
	return iWeight;
#else
	return m_BuildingAIWeights.GetWeight(eBuilding);
#endif
}

/// Recommend highest-weighted building
BuildingTypes CvBuildingProductionAI::RecommendBuilding()
{
#ifdef AUI_WARNING_FIXES
	uint iBldgLoop;
#else
	int iBldgLoop;
#endif
	int iWeight;
	int iTurnsLeft;

	// Reset list of all the possible buildings
	m_Buildables.clear();

	// Loop through adding the available buildings
	for(iBldgLoop = 0; iBldgLoop < GC.GetGameBuildings()->GetNumBuildings(); iBldgLoop++)
	{
		// Make sure this building can be built now
		if(m_pCity->canConstruct((BuildingTypes)iBldgLoop))
		{
			// Update weight based on turns to construct
			iTurnsLeft = m_pCity->getProductionTurnsLeft((BuildingTypes) iBldgLoop, 0);
			iWeight = CityStrategyAIHelpers::ReweightByTurnsLeft(m_BuildingAIWeights.GetWeight((BuildingTypes)iBldgLoop), iTurnsLeft);
			m_Buildables.push_back(iBldgLoop, iWeight);
		}
	}

	// Sort items and grab the first one
	if(m_Buildables.size() > 0)
	{
		m_Buildables.SortItems();
		LogPossibleBuilds();
		return (BuildingTypes)m_Buildables.GetElement(0);
	}

	// Unless we didn't find any
	else
	{
		return NO_BUILDING;
	}
}

/// Log all potential builds
void CvBuildingProductionAI::LogPossibleBuilds()
{
	if(GC.getLogging() && GC.getAILogging())
	{
		// Find the name of this civ and city
		CvString playerName = GET_PLAYER(m_pCity->getOwner()).getCivilizationShortDescription();
		CvString cityName = m_pCity->getName();

		// Open the log file
		FILogFile* pLog = LOGFILEMGR.GetLog(m_pCity->GetCityStrategyAI()->GetLogFileName(playerName, cityName), FILogFile::kDontTimeStamp);

		// Get the leading info for this line
		CvString strBaseString;
		strBaseString.Format("%03d, ", GC.getGame().getElapsedGameTurns());
		strBaseString += playerName + ", " + cityName + ", ";

		// Dump out the weight of each buildable item
		CvBuildingXMLEntries* pGameBuildings = GC.GetGameBuildings();
		if(pGameBuildings != NULL)
		{
			for(int iI = 0; iI < m_Buildables.size(); iI++)
			{
				CvBuildingEntry* pBuildingEntry = pGameBuildings->GetEntry(m_Buildables.GetElement(iI));;
				if(pBuildingEntry != NULL)
				{
					CvString strTemp;
					strTemp.Format("Building, %s, %d", pBuildingEntry->GetDescription(), m_Buildables.GetWeight(iI));
					CvString strOutBuf = strBaseString + strTemp;
					pLog->Msg(strOutBuf);
				}
			}
		}
	}
}

