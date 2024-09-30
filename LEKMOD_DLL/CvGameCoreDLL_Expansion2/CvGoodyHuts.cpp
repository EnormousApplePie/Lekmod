/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */

#include "CvGameCoreDLLPCH.h"
#include "CvGoodyHuts.h"

// static
int **CvGoodyHuts::m_aaiPlayerGoodyHutResults = NULL;

#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
int** CvGoodyHuts::m_aiGoodyHutsOncePerGame = NULL;
#endif

//	---------------------------------------------------------------------------
/// New Goody Hut Result from a Player
void CvGoodyHuts::DoPlayerReceivedGoody(PlayerTypes ePlayer, GoodyTypes eGoody)
{
	FAssert(ePlayer >= 0);
	FAssert(ePlayer < MAX_MAJOR_CIVS);
	FAssert(eGoody >= 0);
	//	FAssert(eGoody < DB.count("GoodyHuts"));

	// Push elements up in the array so that we free up element 0
	for (int iGoodySlotLoop = 0; iGoodySlotLoop < NUM_GOODIES_REMEMBERED-1; iGoodySlotLoop++)
	{
		m_aaiPlayerGoodyHutResults[ePlayer][iGoodySlotLoop+1] = m_aaiPlayerGoodyHutResults[ePlayer][iGoodySlotLoop];
	}

	// Most recent Goody gets slot 0
	m_aaiPlayerGoodyHutResults[ePlayer][0] = eGoody;
}

//	---------------------------------------------------------------------------
/// Are we allowed to get this Goody right now?
bool CvGoodyHuts::IsCanPlayerReceiveGoody(PlayerTypes ePlayer, GoodyTypes eGoody)
{
	if (IsHasPlayerReceivedGoodyLately(ePlayer, eGoody))
	{
		return false;
	}

	return true;
}

//	---------------------------------------------------------------------------
/// Have we gotten this type of Goody lately? (in the last 3 Goodies, defined by NUM_GOODIES_REMEMBERED)
bool CvGoodyHuts::IsHasPlayerReceivedGoodyLately(PlayerTypes ePlayer, GoodyTypes eGoody)
{
	FAssert(ePlayer >= 0);
	FAssert(ePlayer < MAX_MAJOR_CIVS);
	FAssert(eGoody >= 0);
	//	FAssert(eGoody < DB.count("GoodyHuts"));

	// Look at all of our Goody slots and see if the requested Goody matches anything
	for (int iGoodySlotLoop = 0; iGoodySlotLoop < NUM_GOODIES_REMEMBERED; iGoodySlotLoop++)
	{
		if (m_aaiPlayerGoodyHutResults[ePlayer][iGoodySlotLoop] == eGoody)
		{
			return true;
		}
	}

	return false;
}
#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
//	---------------------------------------------------------------------------
/// Set whether this goody hut type is only allowed to be received once per game
void CvGoodyHuts::DoPlayerReceivedGoodyOncePerGame(PlayerTypes ePlayer, GoodyTypes eGoody)
{
	FAssert(ePlayer >= 0);
	FAssert(ePlayer < MAX_MAJOR_CIVS);
	FAssert(eGoody >= 0);
	FAssert(eGoody < DB.count("GoodyHuts"));
	m_aiGoodyHutsOncePerGame[ePlayer][eGoody] = 1;
}
//	---------------------------------------------------------------------------
/// Is thie goody hut type only allowed to be received once per game?
bool CvGoodyHuts::IsGoodyHutOncePerGame(PlayerTypes ePlayer, GoodyTypes eGoody)
{
	FAssert(eGoody >= 0);
	FAssert(eGoody < DB.count("GoodyHuts"));
	if (m_aiGoodyHutsOncePerGame == NULL)
	{
		return false;
	}
	return m_aiGoodyHutsOncePerGame[ePlayer][eGoody] == 1;
}
#endif

//	--------------------------------------------------------------------------------
/// Reset
void CvGoodyHuts::Reset()
{
	int iI, iJ;

	// Allocate memory
	if (m_aaiPlayerGoodyHutResults == NULL)
	{
		m_aaiPlayerGoodyHutResults = FNEW(int*[MAX_MAJOR_CIVS], c_eCiv5GameplayDLL, 0);
		for (iI = 0; iI < MAX_MAJOR_CIVS; iI++)
		{
			m_aaiPlayerGoodyHutResults[iI] = FNEW(int[NUM_GOODIES_REMEMBERED], c_eCiv5GameplayDLL, 0);
		}
	}

	// Default values
	for (iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
		for (iJ = 0; iJ < NUM_GOODIES_REMEMBERED; iJ++)
		{
			m_aaiPlayerGoodyHutResults[iI][iJ] = -1;
		}
	}

#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
	//Allocate memory
	if (m_aiGoodyHutsOncePerGame == NULL)
	{
		m_aiGoodyHutsOncePerGame = FNEW(int*[MAX_MAJOR_CIVS], c_eCiv5GameplayDLL, 0);
		for (iI = 0; iI < MAX_MAJOR_CIVS; iI++)
		{
			m_aiGoodyHutsOncePerGame[iI] = FNEW(int[DB.Count("GoodyHuts")], c_eCiv5GameplayDLL, 0);
		}
	}

	// Default values
	for (iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
		for (iJ = 0; iJ < DB.Count("GoodyHuts"); iJ++)
		{
			m_aiGoodyHutsOncePerGame[iI][iJ] = 0;
		}
	}
#endif
	
}

//	---------------------------------------------------------------------------
/// Uninit
void CvGoodyHuts::Uninit()
{
	if (m_aaiPlayerGoodyHutResults != NULL)
	{
		for (int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
		{
			SAFE_DELETE_ARRAY(m_aaiPlayerGoodyHutResults[iI]);
		}
		SAFE_DELETE_ARRAY(m_aaiPlayerGoodyHutResults);
	}
#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
	if (m_aiGoodyHutsOncePerGame != NULL)
	{
		for (int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
		{
			SAFE_DELETE_ARRAY(m_aiGoodyHutsOncePerGame[iI]);
		}
		SAFE_DELETE_ARRAY(m_aiGoodyHutsOncePerGame);
	}
#endif
}

//	---------------------------------------------------------------------------
#ifdef AUI_WARNING_FIXES
void CvGoodyHuts::Read(FDataStream& kStream, uint /*uiParentVersion*/)
#else
void CvGoodyHuts::Read(FDataStream& kStream, uint uiParentVersion)
#endif
{
	Reset();

	uint uiVersion;
	kStream >> uiVersion;

	for (int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
#ifdef AUI_WARNING_FIXES
		ArrayWrapper<int> kWrapper(NUM_GOODIES_REMEMBERED, m_aaiPlayerGoodyHutResults[iI]);
		kStream >> kWrapper;
#else
		kStream >> ArrayWrapper<int>(NUM_GOODIES_REMEMBERED, m_aaiPlayerGoodyHutResults[iI]);
#endif
#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
		
		if (uiVersion >= 1)
		{
			for (int iJ = 0; iJ < DB.Count("GoodyHuts"); iJ++)
			{
				kStream >> m_aiGoodyHutsOncePerGame[iI][iJ];
			}
		}
#endif
	
	}
}

//	---------------------------------------------------------------------------
void CvGoodyHuts::Write(FDataStream& kStream)
{
	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;

	for (int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
#ifdef AUI_WARNING_FIXES
		ArrayWrapper<int> kWrapper(NUM_GOODIES_REMEMBERED, m_aaiPlayerGoodyHutResults[iI]);
		kStream << kWrapper;
#else
		kStream << ArrayWrapper<int>(NUM_GOODIES_REMEMBERED, m_aaiPlayerGoodyHutResults[iI]);
#endif
	}

#ifdef LEKMOD_NEW_ANCIENT_RUIN_REWARDS
	for (int iI = 0; iI < MAX_MAJOR_CIVS; iI++)
	{
		for (int iJ = 0; iJ < DB.Count("GoodyHuts"); iJ++)
		{
			kStream << m_aiGoodyHutsOncePerGame[iI][iJ];
		}
	}
#endif
}
