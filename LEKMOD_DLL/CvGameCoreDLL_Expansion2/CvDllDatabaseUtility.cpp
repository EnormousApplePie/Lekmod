/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
//
//  FILE:    CvDllDatabaseUtility.cpp
//
//  AUTHOR:		Eric MacDonald  --  8/2003
//				Mustafa Thamer
//				Shaun Seckman	--	3/2009
//  PURPOSE: Group of functions to load in the xml files for Civilization 5
//
#include "CvGameCoreDLLPCH.h"
#include <algorithm>
#include "CvDLLDatabaseUtility.h"
#include "CvDllContext.h"
#include "CvGlobals.h"
#include "CvGrandStrategyAI.h"
#include "CvEconomicAI.h"
#include "CvMilitaryAI.h"
#include "CvCitySpecializationAI.h"
#include "CvMinorCivAI.h"
#include "CvImprovementClasses.h"
#include "CvDatabaseUtility.h"
#include "CvGameTextMgr.h"
#include "CvGameCoreUtils.h"
#include "cvStopWatch.h"
#include "CvTypes.h"

#include <CvLocalization.h>
#include <CvGameDatabase.h>
// include this after all other headers
#include "LintFree.h"

#include <windows.h>

#define GAMEPLAYXML_PATH "Gameplay\\XML\\"


//Just a quick utility function to save from writing lots of verbose code.
void InsertGameDefine(Database::Results& kInsertDefine, const char* szValue, int iValue)
{
	kInsertDefine.Bind(1, szValue);
	kInsertDefine.Bind(2, iValue);
	kInsertDefine.Execute();
	kInsertDefine.Reset();
}

//////////////////////////////////////////////////////////////////////////
CvDllDatabaseUtility::CvDllDatabaseUtility(Database::Connection& db)
	: m_kGameplayDatabase(db)
	, m_bGameDatabaseNeedsCaching(true)
	, m_uiRefCount(1)
{
}
//------------------------------------------------------------------------------
CvDllDatabaseUtility::~CvDllDatabaseUtility()
{
}
//------------------------------------------------------------------------------
void* CvDllDatabaseUtility::QueryInterface(GUID guidInterface)
{
	if(guidInterface == ICvUnknown::GetInterfaceId() ||
	        guidInterface == ICvDLLDatabaseUtility1::GetInterfaceId())
	{
		IncrementReference();
		return this;
	}

	return NULL;
}
//------------------------------------------------------------------------------
unsigned int CvDllDatabaseUtility::IncrementReference()
{
	++m_uiRefCount;
	return m_uiRefCount;
}
//------------------------------------------------------------------------------
unsigned int CvDllDatabaseUtility::DecrementReference()
{
	if(m_uiRefCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		--m_uiRefCount;
		return m_uiRefCount;
	}
}
//------------------------------------------------------------------------------
unsigned int CvDllDatabaseUtility::GetReferenceCount()
{
	return m_uiRefCount;
}
//------------------------------------------------------------------------------
void CvDllDatabaseUtility::Destroy()
{
	DecrementReference();
}
//------------------------------------------------------------------------------
void CvDllDatabaseUtility::operator delete(void* p)
{
	CvDllGameContext::Free(p);
}
//------------------------------------------------------------------------------
void* CvDllDatabaseUtility::operator new(size_t bytes)
{
	return CvDllGameContext::Allocate(bytes);
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::CacheGameDatabaseData()
{
	//Do not cache everything if we don't need to.
	if(m_bGameDatabaseNeedsCaching == false)
		return true;

	//The following code depends on a valid initialized database.
	bool bSuccess = true;

	//TODO: Figure out how to handle cases where Validation has failed.
	/*bSuccess &= */
	ValidateGameDatabase();
	
#ifdef LEKMOD_POST_DLC_DATA_LOADING
	// Add our post-DLC loading here, after validation but before prefetching
	bSuccess &= PerformPostDLCLoading();
#endif

	//HACK Legacy 'FindInfoByType' support.
	//In order to support the legacy code still using the old infos system,
	//all of the id/type pairs must be added to gc.m_infosMap
	//I apologize for this horrendous code, please remove it in the near future.
	GC.infoTypeFromStringReset();
	Database::Results kTables("name");
	if(DB.SelectAt(kTables, "sqlite_master", "type", "table"))
	{
		while(kTables.Step())
		{
			Database::Results kTypes;
			if(DB.SelectAll(kTypes, kTables.GetText(0)))
			{
				bool bFirstStep = true;
				while(kTypes.Step())
				{
					if(bFirstStep)
					{
						if(!kTypes.HasColumn("ID") || !kTypes.HasColumn("Type"))
						{
							break;
						}

						bFirstStep = false;
					}

					const int rowid = kTypes.GetInt("ID");
					const char* szType = kTypes.GetText("Type");
					if(szType)
						GC.setInfoTypeFromString(szType, rowid);
				}
			}
		}
	}

	bSuccess &= LoadGlobalDefines();
	bSuccess &= PrefetchGameData();
	bSuccess &= UpdatePlayableCivilizationCounts();

	CvTypes::AcquireTypes(DB);

	bSuccess &= SetGlobalActionInfo();

	//Clear out database cache and tune for runtime use.
	DB.ClearCountCache();

	//Log Database Memory statistics
	LogMsg(DB.CalculateMemoryStats());

	CvAssertMsg(bSuccess, "Failed to load Gameplay Database Data! Not Good!");

	if(bSuccess)
		m_bGameDatabaseNeedsCaching = false;

	return bSuccess;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::FlushGameDatabaseData()
{
	m_bGameDatabaseNeedsCaching = true;
	return true;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::PerformDatabasePostProcessing()
{
	//Insert any database methods that you would like performed after the database
	//has been fully loaded.  This method will execute every single time the game
	//is run.
	//Updates performed here are done AFTER the database has been built or read
	//from cache.
	Database::Connection* db = GC.GetGameDatabase();

	//Update Defines table from references in PostDefines table
	db->BeginTransaction();
	Database::Results kPostDefines;

	//Build insertion statement
	Database::Results kInsert;
	db->Execute(kInsert, "INSERT OR REPLACE INTO Defines(Name, Value) VALUES(?, ?)");

	db->SelectAll(kPostDefines, "PostDefines");
	while(kPostDefines.Step())
	{
		const char* szName = kPostDefines.GetText("Name");
		const char* szKeyName = kPostDefines.GetText("Key");
		const char* szTableName = kPostDefines.GetText("Table");
		char szSQL[512];

		sprintf_s(szSQL, "select ROWID from %s where Type = '%s' LIMIT 1", szTableName, szKeyName);

		Database::Results kLookup;

		//Compile the command.
		if(db->Execute(kLookup, szSQL))
		{
			//Run the command.
			if(kLookup.Step())
			{
				//Perform insertion
				kInsert.Bind(1, szName);
				kInsert.Bind(2, kLookup.GetInt(0));
				kInsert.Step();
				kInsert.Reset();
			}
		}
	}

	// ** Modify ResourceUsage of Resources table **
	// Set ResourceUsage to 1 if it's referenced in Unit_ResourceQuantityRequirements
	// NOTE: This query could be simplified using the IN operator but when analyzed this
	//			statement generates faster operations.
	const char* szStrategicResource
	    = "UPDATE Resources SET ResourceUsage = 1 WHERE EXISTS (SELECT * FROM Unit_ResourceQuantityRequirements WHERE ResourceType = Type)";
	db->Execute(szStrategicResource);

	// Set ResourceUsage to 2 if the Resource has a happiness value greater than 0
	const char* szLuxoryResource
	    = "UPDATE Resources SET ResourceUsage = 2 where Happiness > 0";
	db->Execute(szLuxoryResource);

	//These are hard-coded GameDefines enum values, let's share them with the database so that they
	//get the same amount of love as the DB.
	Database::Results kInsertDefine;
	if(db->Execute(kInsertDefine, "INSERT OR REPLACE INTO Defines(Name, Value) VALUES(?, ?);"))
	{
		InsertGameDefine(kInsertDefine, "MAX_CIV_PLAYERS", MAX_CIV_PLAYERS);
		InsertGameDefine(kInsertDefine, "MAX_CIV_TEAMS", MAX_CIV_TEAMS);
		InsertGameDefine(kInsertDefine, "MAX_MAJOR_CIVS", MAX_MAJOR_CIVS);
		InsertGameDefine(kInsertDefine, "MAX_MINOR_CIVS", MAX_MINOR_CIVS);
		InsertGameDefine(kInsertDefine, "MAX_PLAYERS", MAX_PLAYERS);
		InsertGameDefine(kInsertDefine, "MAX_TEAMS", MAX_TEAMS);
		InsertGameDefine(kInsertDefine, "BARBARIAN_PLAYER", BARBARIAN_PLAYER);
		InsertGameDefine(kInsertDefine, "BARBARIAN_TEAM", BARBARIAN_TEAM);

		InsertGameDefine(kInsertDefine, "NUM_CITY_PLOTS", NUM_CITY_PLOTS);
		InsertGameDefine(kInsertDefine, "CITY_HOME_PLOT", CITY_HOME_PLOT);
		InsertGameDefine(kInsertDefine, "CITY_PLOTS_RADIUS", CITY_PLOTS_RADIUS);
		InsertGameDefine(kInsertDefine, "CITY_PLOTS_DIAMETER", CITY_PLOTS_DIAMETER);
	}

	db->EndTransaction();

	return true;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::PrefetchGameData()
{
	cvStopWatch kTest("PrefetchGameData", "xml-perf.log");

	//Because Colors and PlayerColors are used everywhere during load
	//(by the translator) we load interface infos first.
	//Interface
	PrefetchCollection(GC.GetColorInfo(), "Colors");
	PrefetchCollection(GC.GetPlayerColorInfo(), "PlayerColors");
	PrefetchCollection(GC.getInterfaceModeInfo(), "InterfaceModes");

	//AI
	PrefetchCollection(GC.getAICityStrategyInfo(), "AICityStrategies");
	PrefetchCollection(GC.getEconomicAIStrategyInfo(), "AIEconomicStrategies");
	PrefetchCollection(GC.getAIGrandStrategyInfo(), "AIGrandStrategies");
	PrefetchCollection(GC.getMilitaryAIStrategyInfo(), "AIMilitaryStrategies");
	PrefetchCollection(GC.getCitySpecializationInfo(), "CitySpecializations");
	PrefetchCollection(GC.getTacticalMoveInfo(), "TacticalMoves");

	//BasicInfos
	//AnimationOperators?
	//Attitudes?
	PrefetchCollection(GC.getUnitAIInfo(), "UnitAIInfos");
	PrefetchCollection(GC.getUnitCombatClassInfo(), "UnitCombatInfos");

	//Buildings
	PrefetchCollection(GC.getBuildingClassInfo(), "BuildingClasses");
	PrefetchCollection(GC.getBuildingInfo(), "Buildings");

	//GameInfo
	PrefetchCollection(GC.getEmphasisInfo(), "EmphasizeInfos");
	PrefetchCollection(GC.getEraInfo(), "Eras");
	PrefetchCollection(GC.getGameOptionInfo(), "GameOptions");
	PrefetchCollection(GC.getGameSpeedInfo(), "GameSpeeds");
	PrefetchCollection(GC.getHandicapInfo(), "HandicapInfos");
	PrefetchCollection(GC.getHurryInfo(), "HurryInfos");
	PrefetchCollection(GC.getMPOptionInfo(), "MultiplayerOptions");
	PrefetchCollection(GC.getPlayerOptionInfo(), "PlayerOptions");
	PrefetchCollection(GC.getPolicyInfo(), "Policies");
	PrefetchCollection(GC.getPolicyBranchInfo(), "PolicyBranchTypes");
	PrefetchCollection(GC.getProcessInfo(), "Processes");
	PrefetchCollection(GC.getProjectInfo(), "Projects");
	PrefetchCollection(GC.getSmallAwardInfo(), "SmallAwards");
	PrefetchCollection(GC.getSpecialistInfo(), "Specialists");
	PrefetchCollection(GC.getVictoryInfo(), "Victories");
	PrefetchCollection(GC.getVoteInfo(), "Votes");
	PrefetchCollection(GC.getVoteSourceInfo(), "VoteSources");
	PrefetchCollection(GC.getUnitDomainInfo(), "Domains");

	//Leaders
	PrefetchCollection(GC.getLeaderHeadInfo(), "Leaders");

	//Misc
	PrefetchCollection(GC.getRouteInfo(), "Routes");

	CvNotificationXMLEntries* pkNotificationEntries =  GC.GetNotificationEntries();
	if(pkNotificationEntries != NULL)
	{
		PrefetchCollection(pkNotificationEntries->GetNotificationEntries(), "Notifications");
	}

	//Technologies
	PrefetchCollection(GC.getTechInfo(), "Technologies");

	//Terrain
	PrefetchCollection(GC.getFeatureInfo(), "Features");
	PrefetchCollection(GC.getImprovementInfo(), "Improvements");
	PrefetchCollection(GC.getResourceClassInfo(), "ResourceClasses");
	PrefetchCollection(GC.getResourceInfo(), "Resources");
	PrefetchCollection(GC.getTerrainInfo(), "Terrains");
	PrefetchCollection(GC.getYieldInfo(), "Yields");

	//Units
	PrefetchCollection(GC.getAutomateInfo(), "Automates");
	PrefetchCollection(GC.getBuildInfo(), "Builds");
	PrefetchCollection(GC.getCommandInfo(), "Commands");
	PrefetchCollection(GC.getControlInfo(), "Controls");
	PrefetchCollection(GC.getEntityEventInfo(), "EntityEvents");
	PrefetchCollection(GC.getMissionInfo(), "Missions");
	PrefetchCollection(GC.getMultiUnitFormationInfo(), "MultiUnitFormations");
	PrefetchCollection(GC.getSpecialUnitInfo(), "SpecialUnits");
	PrefetchCollection(GC.getUnitClassInfo(), "UnitClasses");
	PrefetchCollection(GC.getPromotionInfo(), "UnitPromotions");
	PrefetchCollection(GC.getUnitInfo(), "Units");

	//Civilizations - must be after buildings and units
	PrefetchCollection(GC.getCivilizationInfo(), "Civilizations");
	PrefetchCollection(GC.getMinorCivInfo(), "MinorCivilizations");
	PrefetchCollection(GC.getTraitInfo(), "Traits");
	PrefetchCollection(GC.getReligionInfo(), "Religions");
	PrefetchCollection(GC.getBeliefInfo(), "Beliefs");
	PrefetchCollection(GC.getLeagueSpecialSessionInfo(), "LeagueSpecialSessions");
	PrefetchCollection(GC.getLeagueNameInfo(), "LeagueNames");
	PrefetchCollection(GC.getLeagueProjectInfo(), "LeagueProjects");
	PrefetchCollection(GC.getLeagueProjectRewardInfo(), "LeagueProjectRewards");
	PrefetchCollection(GC.getResolutionInfo(), "Resolutions");

	//Copy flavors into string array
	{
		CvDatabaseUtility kUtility;
		CvString*& paFlavors = GC.getFlavorTypes();
		const int iNumFlavors = kUtility.MaxRows("Flavors");
		//GC.getNumFlavorTypes() = iNumFlavors;
		GC.setNumFlavorTypes(iNumFlavors);
		paFlavors = FNEW(CvString[iNumFlavors], c_eCiv5GameplayDLL, 0);

		Database::Results kResults;
		if(DB.SelectWhere(kResults, "Flavors", "ID > -1"))
		{
			while(kResults.Step())
			{
				const int iFlavor = kResults.GetInt("ID");
				CvAssert(iFlavor >= 0 && iFlavor < iNumFlavors);
				if(iFlavor >= 0 && iFlavor < iNumFlavors)
				{
					paFlavors[iFlavor] = kResults.GetText("Type");

				}
			}
		}
		else
		{
			CvAssertMsg(false, DB.ErrorMessage());
		}
	}

	ValidatePrefetchProcess();

	return true;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::ValidateGameDatabase()
{
	//This function contains a suite of useful game database unit tests that will
	//ensure the database conforms to certain unique rules of Civ5.
	if(!gDLL->ShouldValidateGameDatabase())
	{
		LogMsg("*** SKIPPING Game Database Validation. ****");
		LogMsg("You can enable it by setting 'ValidateGameDatabase = 1' in config.ini");
		return true;
	}

	cvStopWatch kPerfTest("Validating Game Database", "xml-perf.log");
	bool bError = false;

	LogMsg("**** Validating Game Database *****");

	//Test that all Tables w/ 'ID' column start at 0 and not 1.
	{
		cvStopWatch watch("Ensure All Tables with 'ID' column start at 0");
		Database::Results kTables("name");
		if(DB.SelectAt(kTables, "sqlite_master", "type", "table"))
		{
			while(kTables.Step())
			{
				const char* szTableName = kTables.GetText(0);
				if(DB.Count(szTableName) > 0)
				{
					//Test if table has 'ID' column
					bool bHasIDColumn = false;
					{
						//Execute "select ID from <table_name> limit 1;
						//If there's a SQL error, it's most likely due to a lack of an 'id' column.
						char szSQL[512];
						sprintf_s(szSQL, "pragma table_info(%s)", szTableName);
						Database::Results kResults;
						DB.Execute(kResults, szSQL);
						while(kResults.Step())
						{
							const char* szName = kResults.GetText("name");
							if(strcmp(szTableName, szName) == 0)
							{
								bHasIDColumn = true;
								break;
							}

						}
					}

					if(bHasIDColumn)
					{
						Database::SingleResult kTest;
						if(!DB.SelectAt(kTest, szTableName, "ID", 0))
						{
							//Table has 'ID' column and contains data but does not use ID 0.
							char szError[512];
							sprintf_s(szError, "Table '%s' contains 'ID' column that starts at 1 instead of 0.", szTableName);
							LogMsg(szError);
							bError = true;
						}
					}

				}
			}
		}
	}

	//Validate FK constraints
	{
		cvStopWatch watch("Validate FK Constraints");
		DB.ValidateFKConstraints();
	}

	LogMsg("Performing Localization Checks");
	{
		cvStopWatch watch("Localization Checks");
		LogMsg("Checking Tag Format...");
		LogMsg("Note: Tags must only use [A-Z_] characters, start with 'TXT_KEY_', and be under 128 characters long.");

		Database::Results kLanguages;
		if(DB.SelectAll(kLanguages, "Languages"))
		{
			while(kLanguages.Step())
			{
				const char* szTagColumnName = kLanguages.GetText("TagColumnName");
				const char* szTableName = kLanguages.GetText("TableName");

				Database::Results kResults;
				char szSQL[512];
				sprintf_s(szSQL, "SELECT DISTINCT %s from %s", szTagColumnName, szTableName);
				if(DB.Execute(kResults, szSQL))
				{
					bool bFirst = true;
					while(kResults.Step())
					{
						const char* szTag = kResults.GetText(0);
						const size_t len = strlen(szTag);
						if(len > 128)
						{
							if(bFirst)
							{
								LogMsg("In table %s...", szTableName);
								bFirst = false;
							}

							bError =  true;
							LogMsg("Tag (%s) has size greater than 127 characters.", szTag);
						}
						else
						{
							if(strncmp("TXT_KEY_", szTag, 8) != 0)
							{
								if(bFirst)
								{
									LogMsg("In table %s...", szTableName);
									bFirst = false;
								}

								bError = true;
								LogMsg("Tag (%s) does not start with 'TXT_KEY_'", szTag);
							}

							for(size_t i = 0; i < len; ++i)
							{
								char ch = szTag[i];
								if(ch != '_' && !((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')))
								{
									if(bFirst)
									{
										LogMsg("In table %s...", szTableName);
										bFirst = false;
									}

									bError = true;
									LogMsg("Tag (%s) contains invalid character (%.1s)", szTag, &ch);
								}
							}
						}
					}
				}
			}
		}
	}

	LogMsg("Validating UnitGameplay2DScripts");
	{
		cvStopWatch watch("Validating UnitGameplay2DScripts");
		Database::Results kResults;
		if(DB.Execute(kResults, "Select Type from Units where not exists (select 1 from UnitGameplay2DScripts where UnitType = Units.Type limit 1)"))
		{
			while(kResults.Step())
			{
				const char* szUnitType = kResults.GetText(0);
				LogMsg("Missing Entry for %s", szUnitType);
				bError = true;
			}
		}
	}

	if(bError)
	{
		LogMsg("**** VALIDATION FAILED *****");
	}
	else
	{
		LogMsg("**** Validation Success *****");
	}

	kPerfTest.EndPerfTest();
	LogMsg("Validation Took %f seconds", kPerfTest.GetDeltaInSeconds());

	return !bError;
}


#ifdef AUI_WARNING_FIXES
#define ValidateCount(func) if(func() <= 0){bError = true; LogMsg("ERROR: %s <= 0", #func);}
#else
#define ValidateCount(func) if(func() <= 0){bError = true, LogMsg("ERROR: %s <= 0", #func);}
#endif
#define ValidateVectorSize(vec) ValidateCount(gc.##vec);
bool CvDllDatabaseUtility::ValidatePrefetchProcess()
{
	bool bError = false;
	CvGlobals& gc = GC;

	LogMsg("**** Validating Prefetch Process *****");
	ValidateVectorSize(getNumWorldInfos);
	ValidateVectorSize(getNumClimateInfos);
	ValidateVectorSize(getNumSeaLevelInfos);
	ValidateVectorSize(GetNumColorInfos);
	ValidateVectorSize(GetNumPlayerColorInfos);
	ValidateVectorSize(getNumEntityEventInfos);
	ValidateVectorSize(getNumMultiUnitFormationInfos);
	ValidateVectorSize(getNumTerrainInfos);
	ValidateVectorSize(getNumResourceClassInfos);
	ValidateVectorSize(getNumResourceInfos);
	ValidateVectorSize(getNumFeatureInfos);
	ValidateVectorSize(getNumCivilizationInfos);
	ValidateVectorSize(getNumMinorCivInfos);
	ValidateVectorSize(getNumLeaderHeadInfos);
	ValidateVectorSize(getNumTraitInfos);
	ValidateVectorSize(getNumUnitInfos);
	ValidateVectorSize(getNumSpecialUnitInfos);
	ValidateVectorSize(getNumVoteSourceInfos);
	ValidateVectorSize(getNumUnitCombatClassInfos);

	ValidateCount(gc.getUnitAIInfo().size);


	ValidateVectorSize(getNumGameOptionInfos);
	ValidateVectorSize(getNumMPOptionInfos);

	ValidateCount(gc.getPlayerOptionInfo().size);
	ValidateCount(gc.getYieldInfo().size);

	ValidateVectorSize(getNumRouteInfos);
	ValidateVectorSize(getNumImprovementInfos);
	ValidateVectorSize(getNumBuildInfos);
	ValidateVectorSize(getNumHandicapInfos);
	ValidateVectorSize(getNumGameSpeedInfos);
	ValidateVectorSize(getNumProcessInfos);
	ValidateVectorSize(getNumVoteInfos);
	ValidateVectorSize(getNumProjectInfos);
	ValidateVectorSize(getNumBuildingClassInfos);
	ValidateVectorSize(getNumBuildingInfos);
	ValidateVectorSize(getNumUnitClassInfos);
	//ValidateVectorSize(getNumActionInfos);	//Action Infos are generated as a post process.

	ValidateCount(gc.getMissionInfo().size);
	ValidateCount(gc.getControlInfo().size);
	ValidateCount(gc.getCommandInfo().size);

	ValidateVectorSize(getNumAutomateInfos);
	ValidateVectorSize(getNumPromotionInfos);
	ValidateVectorSize(getNumSpecialistInfos);
	ValidateVectorSize(getNumEconomicAIStrategyInfos);
	ValidateVectorSize(getNumCitySpecializationInfos);
	ValidateVectorSize(getNumTacticalMoveInfos);
	ValidateVectorSize(getNumMilitaryAIStrategyInfos);
	ValidateVectorSize(getNumAIGrandStrategyInfos);
	ValidateVectorSize(getNumAICityStrategyInfos);
	ValidateVectorSize(getNumPolicyInfos);
	ValidateVectorSize(getNumPolicyBranchInfos);
	ValidateVectorSize(getNumTechInfos);
	ValidateVectorSize(getNumEraInfos);
	ValidateVectorSize(getNumHurryInfos);
	ValidateVectorSize(getNumEmphasisInfos);
	ValidateVectorSize(getNumVictoryInfos);

	// The domains are a special case in that the contents must match a populated enum exactly.
#define ValidateDomain(domain) { CvDomainInfo* pkDomainInfo; if (GC.getNumUnitDomainInfos() <= (int)domain || (pkDomainInfo = GC.getUnitDomainInfo(domain)) == NULL || strcmp(pkDomainInfo->GetType(), #domain) != 0) bError = true; }

	ValidateDomain(DOMAIN_SEA);
	ValidateDomain(DOMAIN_AIR);
	ValidateDomain(DOMAIN_LAND);
	ValidateDomain(DOMAIN_IMMOBILE);
	ValidateDomain(DOMAIN_HOVER);

	if(bError)
	{
		LogMsg("**** VALIDATION FAILED *****");
	}
	else
	{
		LogMsg("**** Validation Success *****");
	}

	return !bError;
}

//------------------------------------------------------------------------------------------------------
//
//  FUNCTION:   LoadPostGlobalsGlobalDefines()
//
//  PURPOSE :   This function assumes that the LoadGlobalDefines function has already been called
//							it then loads the few global defines that needed to reference a global variable that
//							hadn't been loaded in prior to the SetGlobalDefines call
//
//------------------------------------------------------------------------------------------------------
bool CvDllDatabaseUtility::LoadGlobalDefines()
{
	GC.cacheGlobals();
	return true;
}

//
// Globals which must be loaded before the main menus.
// Don't put anything in here unless it has to be loaded before the main menus,
// instead try to load things in LoadPostMenuGlobals()
//
bool CvDllDatabaseUtility::UpdatePlayableCivilizationCounts()
{
	cvStopWatch kPerfTest("UpdatePlayableCivilizationCounts", "xml-perf.log");

	int numPlayableCivilizations = 0;
	int numAIPlayableCivilizations = 0;

	// Check Playables
#ifdef AUI_WARNING_FIXES
	for (uint i = 0; i < GC.getNumCivilizationInfos(); ++i)
#else
	for(int i = 0; i < GC.getNumCivilizationInfos(); ++i)
#endif
	{
		CvCivilizationInfo* pkCivilization = GC.getCivilizationInfo((CivilizationTypes) i);
		if(pkCivilization)
		{
			if(pkCivilization->isPlayable())
				numPlayableCivilizations++;

			if(pkCivilization->isAIPlayable())
				numAIPlayableCivilizations++;
		}

		GC.getNumPlayableCivilizationInfos() = numPlayableCivilizations;
		GC.getNumAIPlayableCivilizationInfos() = numAIPlayableCivilizations;
	}

	return true;
}

//------------------------------------------------------------------------------------------------------
//
//  FUNCTION:   SetGlobalActionInfo(CvActionInfo** ppActionInfo, int* iNumVals)
//
//  PURPOSE :   Takes the szTagName parameter and if it exists in the xml it loads the ppActionInfo
//				with the value under it and sets the value of the iNumVals parameter to the total number
//				of occurances of the szTagName tag in the xml.
//
//------------------------------------------------------------------------------------------------------
bool CvDllDatabaseUtility::SetGlobalActionInfo()
{
	cvStopWatch kPerfTest("SetGlobalActionInfo", "xml-perf.log");
	LogMsg("SetGlobalActionInfo\n");

	typedef std::vector<CvActionInfo*> ActionInfoVector;
	ActionInfoVector& actionInfos = GC.getActionInfo();

	//First, clear out action info data
	for(ActionInfoVector::iterator it = actionInfos.begin(); it != actionInfos.end(); ++it)
	{
		CvActionInfo* pActionInfo = (*it);
		delete pActionInfo;
	}
	actionInfos.clear();

#ifdef AUI_WARNING_FIXES
	const uint iNumMissionTypes = CvTypes::getNUM_MISSION_TYPES();
#else
	const int iNumMissionTypes = CvTypes::getNUM_MISSION_TYPES();
#endif

	//Verify action counts
	if(!(NUM_INTERFACEMODE_TYPES > 0))
	{
		LogMsg("NUM_INTERFACE_TYPES is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(GC.getNumBuildInfos() > 0))
	{
		LogMsg("GC.getNumBuildInfos() is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(GC.getNumPromotionInfos() > 0))
	{
		LogMsg("GC.getNumPromotionInfos() is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(GC.getNumSpecialistInfos() > 0))
	{
		LogMsg("GC.getNumSpecialistInfos() is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(NUM_CONTROL_TYPES > 0))
	{
		LogMsg("NUM_CONTROL_TYPES is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(GC.getNumAutomateInfos() > 0))
	{
		LogMsg("GC.getNumAutomateInfos() is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(NUM_COMMAND_TYPES > 0))
	{
		LogMsg("NUM_COMMAND_TYPES is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}
	if(!(iNumMissionTypes > 0))
	{
		LogMsg("NUM_MISSION_TYPES is not greater than zero in CvDllDatabaseUtility::SetGlobalActionInfo.");
	}

	int iEstimatedNumActionInfos =
	    NUM_INTERFACEMODE_TYPES +
	    GC.getNumBuildInfos() +
	    GC.getNumPromotionInfos() +
	    GC.getNumSpecialistInfos() +
	    NUM_CONTROL_TYPES +
	    NUM_COMMAND_TYPES +
	    GC.getNumAutomateInfos() +
	    iNumMissionTypes;

	int* piBuffer = FNEW(int[iEstimatedNumActionInfos * 4], c_eCiv5GameplayDLL, 0);
	memset(piBuffer,0, sizeof(int) * iEstimatedNumActionInfos * 4);

	int* piIndexList = piBuffer + iEstimatedNumActionInfos * 0;				//FNEW(int[iNumActionInfos], c_eCiv5GameplayDLL, 0);
	int* piPriorityList = piBuffer + iEstimatedNumActionInfos * 1;			//FNEW(int[iNumActionInfos], c_eCiv5GameplayDLL, 0);
	int* piActionInfoTypeList = piBuffer + iEstimatedNumActionInfos * 2;	//FNEW(int[iNumActionInfos], c_eCiv5GameplayDLL, 0);
	int* piOrderedIndex = piBuffer + iEstimatedNumActionInfos * 3;			//FNEW(int[iNumActionInfos], c_eCiv5GameplayDLL, 0);


	//Gather all available infos, checking for NULL entries as they may have been removed.
#ifdef AUI_WARNING_FIXES
	uint iTotalActionInfoCount = 0;
	uint i = 0;
#else
	int iTotalActionInfoCount = 0;
	int i = 0;
#endif
	for(i=0; i<NUM_COMMAND_TYPES; i++)
	{
		CvCommandInfo* commandInfo = GC.getCommandInfo((CommandTypes)i);
		if(commandInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = commandInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_COMMAND;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<NUM_INTERFACEMODE_TYPES; i++)
	{
		CvInterfaceModeInfo* interfaceInfo = GC.getInterfaceModeInfo((InterfaceModeTypes)i);
		if(interfaceInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = interfaceInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_INTERFACEMODE;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<GC.getNumBuildInfos(); i++)
	{
		CvBuildInfo* buildInfo = GC.getBuildInfo((BuildTypes)i);
		if(buildInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = buildInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_BUILD;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<GC.getNumPromotionInfos(); i++)
	{
		CvPromotionEntry* promotionEntry = GC.getPromotionInfo((PromotionTypes)i);
		if(promotionEntry)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = promotionEntry->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_PROMOTION;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<GC.getNumSpecialistInfos(); i++)
	{
		CvSpecialistInfo* specialistInfo = GC.getSpecialistInfo((SpecialistTypes)i);
		if(specialistInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = specialistInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_SPECIALIST;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<NUM_CONTROL_TYPES; i++)
	{
		CvControlInfo* controlInfo = GC.getControlInfo((ControlTypes)i);
		if(controlInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = controlInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_CONTROL;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<GC.getNumAutomateInfos(); i++)
	{
		CvAutomateInfo* automateInfo = GC.getAutomateInfo(i);
		if(automateInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = automateInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_AUTOMATE;
			iTotalActionInfoCount++;
		}
	}

	for(i=0; i<iNumMissionTypes; i++)
	{
		CvMissionInfo* missionInfo = GC.getMissionInfo((MissionTypes)i);
		if(missionInfo)
		{
			piIndexList[iTotalActionInfoCount] = i;
			piPriorityList[iTotalActionInfoCount] = missionInfo->getOrderPriority();
			piActionInfoTypeList[iTotalActionInfoCount] = ACTIONSUBTYPE_MISSION;
			iTotalActionInfoCount++;
		}
	}

	//Preallocate
	actionInfos.reserve(iTotalActionInfoCount);

	//Sort and add action infos.
	orderHotkeyInfo(&piOrderedIndex, piPriorityList, iTotalActionInfoCount);
	for(i = 0; i < iTotalActionInfoCount; i++)
	{
		CvActionInfo* pActionInfo = FNEW(CvActionInfo, c_eCiv5GameplayDLL, 0);
		CvAssert(piIndexList[piOrderedIndex[i]] != -1);

		pActionInfo->setOriginalIndex(piIndexList[piOrderedIndex[i]]);
		pActionInfo->setSubType((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]]);
		if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_COMMAND)
		{
			GC.getCommandInfo((CommandTypes)piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_INTERFACEMODE)
		{
			GC.getInterfaceModeInfo((InterfaceModeTypes)piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_BUILD)
		{
			GC.getBuildInfo((BuildTypes)piIndexList[piOrderedIndex[i]])->setMissionType(GC.getInfoTypeForString("MISSION_BUILD"));
			GC.getBuildInfo((BuildTypes)piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_PROMOTION)
		{
			CvPromotionEntry* pkEntry = GC.getPromotionInfo((PromotionTypes)piIndexList[piOrderedIndex[i]]);

			pkEntry->SetCommandType(GC.getInfoTypeForString("COMMAND_PROMOTION"));
			pkEntry->setActionInfoIndex(i);
			CvString strHotKey = pkEntry->CreateHotKeyFromDescription(pkEntry->getHotKey(), pkEntry->isShiftDown(), pkEntry->isAltDown(), pkEntry->isCtrlDown());
			pkEntry->setHotKeyDescription(pkEntry->GetDescription(), GC.getCommandInfo((CommandTypes)(pkEntry->GetCommandType()))->GetDescription(), strHotKey);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_SPECIALIST)
		{
			CvSpecialistInfo* pkSpecialist = GC.getSpecialistInfo((SpecialistTypes)piIndexList[piOrderedIndex[i]]);

			pkSpecialist->setMissionType(GC.getInfoTypeForString("MISSION_JOIN"));
			pkSpecialist->setActionInfoIndex(i);
			CvString strHotKey = pkSpecialist->CreateHotKeyFromDescription(pkSpecialist->getHotKey(), pkSpecialist->isShiftDown(), pkSpecialist->isAltDown(), pkSpecialist->isCtrlDown());
			pkSpecialist->setHotKeyDescription(pkSpecialist->GetDescription(), GC.getMissionInfo((MissionTypes)(pkSpecialist->getMissionType()))->GetDescription(), strHotKey);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_CONTROL)
		{
			GC.getControlInfo((ControlTypes)piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_AUTOMATE)
		{
			GC.getAutomateInfo(piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}
		else if((ActionSubTypes)piActionInfoTypeList[piOrderedIndex[i]] == ACTIONSUBTYPE_MISSION)
		{
			GC.getMissionInfo((MissionTypes)piIndexList[piOrderedIndex[i]])->setActionInfoIndex(i);
		}

		GC.getActionInfo().push_back(pActionInfo);
	}

	SAFE_DELETE_ARRAY(piBuffer);
// 	SAFE_DELETE_ARRAY(piOrderedIndex);
// 	SAFE_DELETE_ARRAY(piIndexList);
// 	SAFE_DELETE_ARRAY(piPriorityList);
// 	SAFE_DELETE_ARRAY(piActionInfoTypeList);

	return true;
}


//
// helper sort predicate
//
struct OrderIndex
{
	int m_iPriority;
	int m_iIndex;
};
bool sortHotkeyPriority(const OrderIndex& orderIndex1, const OrderIndex& orderIndex2)
{
	return (orderIndex1.m_iPriority > orderIndex2.m_iPriority);
}

template <class T>
void CvDllDatabaseUtility::orderHotkeyInfo(int** ppiSortedIndex, T* pHotkeyInfos, int iLength)
{
	int iI;
	int* piSortedIndex;
	std::vector<OrderIndex> viOrderPriority;

	viOrderPriority.resize(iLength);
	piSortedIndex = *ppiSortedIndex;

	// set up vector
	for(iI=0; iI<iLength; iI++)
	{
		viOrderPriority[iI].m_iPriority = pHotkeyInfos[iI].getOrderPriority();
		viOrderPriority[iI].m_iIndex = iI;
	}

	// sort the array
	std::sort(viOrderPriority.begin(), viOrderPriority.end(), sortHotkeyPriority);

	// insert new order into the array to return
	for(iI=0; iI<iLength; iI++)
	{
		piSortedIndex[iI] = viOrderPriority[iI].m_iIndex;
	}
}

void CvDllDatabaseUtility::orderHotkeyInfo(int** ppiSortedIndex, int* pHotkeyIndex, int iLength)
{
	int iI;
	int* piSortedIndex;
	std::vector<OrderIndex> viOrderPriority;

	viOrderPriority.resize(iLength);
	piSortedIndex = *ppiSortedIndex;

	// set up vector
	for(iI=0; iI<iLength; iI++)
	{
		viOrderPriority[iI].m_iPriority = pHotkeyIndex[iI];
		viOrderPriority[iI].m_iIndex = iI;
	}

	// sort the array
	std::sort(viOrderPriority.begin(), viOrderPriority.end(), sortHotkeyPriority);

	// insert new order into the array to return
	for(iI=0; iI<iLength; iI++)
	{
		piSortedIndex[iI] = viOrderPriority[iI].m_iIndex;
	}
}

#ifdef LEKMOD_POST_DLC_DATA_LOADING
bool CvDllDatabaseUtility::PerformPostDLCLoading()
{
	Database::Connection* db = GC.GetGameDatabase();
	if(!db)
		return false;

	// Begin transaction for all our changes
	db->BeginTransaction();

	// Use a direct path to the DLC folder
	std::wstring wstrDLCPath = L"Assets\\DLC\\";
	std::string strDLCPath;
	int len = WideCharToMultiByte(CP_UTF8, 0, wstrDLCPath.c_str(), -1, NULL, 0, NULL, NULL);
	if(len > 0) {
		strDLCPath.resize(len - 1);
		WideCharToMultiByte(CP_UTF8, 0, wstrDLCPath.c_str(), -1, &strDLCPath[0], len, NULL, NULL);
	}
	this->LogMsg("[LEKMOD] Searching DLC path: %s", strDLCPath.c_str());

	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW((wstrDLCPath + L"LEKMOD*").c_str(), &ffd);

	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			// Found a LEKMOD folder, now look for XML folder
			std::wstring wstrModPath = wstrDLCPath + ffd.cFileName + L"\\XML\\";
			std::string strModPath;
			int len2 = WideCharToMultiByte(CP_UTF8, 0, wstrModPath.c_str(), -1, NULL, 0, NULL, NULL);
			if(len2 > 0) {
				strModPath.resize(len2 - 1);
				WideCharToMultiByte(CP_UTF8, 0, wstrModPath.c_str(), -1, &strModPath[0], len2, NULL, NULL);
			}
			this->LogMsg("[LEKMOD] Found LEKMOD folder: %s", strModPath.c_str());

			Database::XMLSerializer serializer(*db);
			
			// TWO-PASS LOADING: First load all text files, then other XML files
			this->LogMsg("[LEKMOD] FIRST PASS: Loading text XML files from %s", strModPath.c_str());
			LoadTextXMLFiles(wstrModPath, serializer);
			
			this->LogMsg("[LEKMOD] SECOND PASS: Loading non-text XML files from %s", strModPath.c_str());
			LoadNonTextXMLFiles(wstrModPath, serializer);

		} while(FindNextFileW(hFind, &ffd) != 0);
		FindClose(hFind);
	}
	else
	{
		this->LogMsg("[LEKMOD] No LEKMOD folders found in: %s", strDLCPath.c_str());
	}

	db->EndTransaction();
	return true;
}

// Helper function to determine if a file is a text XML
bool CvDllDatabaseUtility::IsTextXMLFile(const std::wstring& filePath)
{
	// Convert to lowercase for case-insensitive checks
	std::wstring lowerPath = filePath;
	std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::towlower);
	
	// Check if in a Text folder
	if(lowerPath.find(L"\\text\\") != std::wstring::npos)
		return true;
	
	// Check if filename contains Text
	if(lowerPath.find(L"text.xml") != std::wstring::npos)
		return true;
	
	// Check if it's a language XML file
	if(lowerPath.find(L"language_") != std::wstring::npos)
		return true;
		
	return false;
}

// Load only text XML files - first pass
void CvDllDatabaseUtility::LoadTextXMLFiles(const std::wstring& wstrPath, Database::XMLSerializer& serializer)
{
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW((wstrPath + L"*").c_str(), &ffd);
	std::string strPath;
	int len = WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, NULL, 0, NULL, NULL);
	if(len > 0) {
		strPath.resize(len - 1);
		WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, &strPath[0], len, NULL, NULL);
	}
	if(hFind == INVALID_HANDLE_VALUE)
	{
		this->LogMsg("[LEKMOD] Directory not found: %s", strPath.c_str());
		return;
	}
	
	do
	{
		if(wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
			continue;
			
		std::wstring fullPath = wstrPath + ffd.cFileName;
		
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Recursively process directories
			LoadTextXMLFiles(fullPath + L"\\", serializer);
		}
		else
		{
			// Check if it's an XML file
			size_t extPos = fullPath.find_last_of(L'.');
			if(extPos != std::wstring::npos && _wcsicmp(fullPath.substr(extPos).c_str(), L".xml") == 0)
			{
				// Check if it's a text XML file
				if(IsTextXMLFile(fullPath))
				{
					std::string strFullPath;
					int len2 = WideCharToMultiByte(CP_UTF8, 0, fullPath.c_str(), -1, NULL, 0, NULL, NULL);
					if(len2 > 0) {
						strFullPath.resize(len2 - 1);
						WideCharToMultiByte(CP_UTF8, 0, fullPath.c_str(), -1, &strFullPath[0], len2, NULL, NULL);
					}
					this->LogMsg("[LEKMOD] Loading TEXT XML: %s", strFullPath.c_str());
					
					// First verify the file exists and is readable
					HANDLE hFile = CreateFileW(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE) {
						DWORD error = GetLastError();
						this->LogMsg("[LEKMOD] ERROR: Cannot open file: %s, Windows error: %lu", strFullPath.c_str(), error);
					} else {
						// Read a small portion to verify content
						char buffer[256] = {0};
						DWORD bytesRead = 0;
						ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
						CloseHandle(hFile);
						
						// Log first bytes of file to help diagnose issues
						buffer[bytesRead] = 0;
						this->LogMsg("[LEKMOD] File preview (%lu bytes): %.100s", bytesRead, buffer);
						
						// Look for Language tag to confirm it's a text file
						bool languageTagFound = (strstr(buffer, "<Language_") != NULL);
						this->LogMsg("[LEKMOD] Language tag found: %s", languageTagFound ? "YES" : "NO");
						
						// Try manual load if it's confirmed to be a language file
						if (languageTagFound) {
							// Now try to load it
							bool loaded = serializer.Load(fullPath.c_str());
							if(loaded) {
								this->LogMsg("[LEKMOD] Successfully loaded TEXT XML: %s", strFullPath.c_str());
							} else {
								this->LogMsg("[LEKMOD] FAILED to load TEXT XML: %s", strFullPath.c_str());
								// Try manual import as a last resort
								this->LogMsg("[LEKMOD] Attempting to load text data via alternate method...");
								
								// Get database from globals
								Database::Connection* db = GC.GetGameDatabase();
								if (db) {
									// Try to read the file content directly
									HANDLE hFileContent = CreateFileW(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
									if (hFileContent != INVALID_HANDLE_VALUE) {
										// Get file size
										DWORD fileSize = GetFileSize(hFileContent, NULL);
										if (fileSize != INVALID_FILE_SIZE) {
											this->LogMsg("[LEKMOD] File size: %lu bytes", fileSize);
											
											// Read entire file
											char* xmlBuffer = new char[fileSize + 1];
											DWORD bytesRead = 0;
											if (ReadFile(hFileContent, xmlBuffer, fileSize, &bytesRead, NULL) && bytesRead > 0) {
												xmlBuffer[bytesRead] = 0; // Null terminate
												
												// Try to manually parse the XML and insert records
												this->LogMsg("[LEKMOD] Read %lu bytes, attempting manual parsing", bytesRead);
												
												// Simple check for language table name
												const char* langStart = strstr(xmlBuffer, "<Language_");
												if (langStart) {
													// Extract language table name
													char langTableName[64] = {0};
													if (sscanf(langStart, "<Language_%[^>]", langTableName) == 1) {
														this->LogMsg("[LEKMOD] Found language table: Language_%s", langTableName);
														
														// Look for tag/text pairs
														int entriesAdded = 0;
														char* pos = xmlBuffer;
														char tagBuffer[256];
														char textBuffer[4096];
														
														// Find all <Row> elements (each contains a Tag and Text)
														while ((pos = strstr(pos, "<Row>")) != NULL) {
															char* endRow = strstr(pos, "</Row>");
															if (!endRow) break;
															
															// Extract the Tag and Text
															char* tagStart = strstr(pos, "<Tag>");
															char* textStart = strstr(pos, "<Text>");
															
															if (tagStart && textStart && tagStart < endRow && textStart < endRow) {
																tagStart += 5; // Skip <Tag>
																char* tagEnd = strstr(tagStart, "</Tag>");
																
																textStart += 6; // Skip <Text>
																char* textEnd = strstr(textStart, "</Text>");
																
																if (tagEnd && textEnd && tagEnd < endRow && textEnd < endRow) {
																	// Copy tag and text to buffers
																	size_t tagLen = tagEnd - tagStart;
																	size_t textLen = textEnd - textStart;
																	
																	if (tagLen < sizeof(tagBuffer) && textLen < sizeof(textBuffer)) {
																		strncpy(tagBuffer, tagStart, tagLen);
																		tagBuffer[tagLen] = 0;
																		
																		strncpy(textBuffer, textStart, textLen);
																		textBuffer[textLen] = 0;
																		
																		// Insert into database
																		std::string tableName = "Language_";
																		tableName += langTableName;
																		
																		std::string query = "INSERT OR REPLACE INTO ";
																		query += tableName;
																		query += " (Tag, Text) VALUES (?, ?);";
																		
																		Database::Results insert;
																		if (db->Execute(insert, query.c_str())) {
																			insert.Bind(1, tagBuffer);
																			insert.Bind(2, textBuffer);
																			if (insert.Step()) {
																				entriesAdded++;
																			}
																			insert.Reset();
																		}
																	}
																}
															}
															pos = endRow;
														}
														
														this->LogMsg("[LEKMOD] Manually added %d text entries to %s", entriesAdded, langTableName);
													}
												} else {
													this->LogMsg("[LEKMOD] Could not find language table in XML");
												}
											}
											
											delete[] xmlBuffer;
										}
										CloseHandle(hFileContent);
									} else {
										DWORD error = GetLastError();
										this->LogMsg("[LEKMOD] Failed to open file for manual parsing, error: %lu", error);
									}
								}
							}
						} else {
							this->LogMsg("[LEKMOD] File does not appear to be a language file, skipping text loader");
							// It's not actually a text file - retry in non-text pass
						}
					}
				}
			}
		}
	} while(FindNextFileW(hFind, &ffd) != 0);
	
	FindClose(hFind);
}

// Load non-text XML files - second pass
void CvDllDatabaseUtility::LoadNonTextXMLFiles(const std::wstring& wstrPath, Database::XMLSerializer& serializer)
{
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW((wstrPath + L"*").c_str(), &ffd);
	std::string strPath;
	int len = WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, NULL, 0, NULL, NULL);
	if(len > 0) {
		strPath.resize(len - 1);
		WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, &strPath[0], len, NULL, NULL);
	}
	if(hFind == INVALID_HANDLE_VALUE)
	{
		this->LogMsg("[LEKMOD] Directory not found: %s", strPath.c_str());
		return;
	}
	
	do
	{
		if(wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
			continue;
			
		std::wstring fullPath = wstrPath + ffd.cFileName;
		
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Recursively process directories
			LoadNonTextXMLFiles(fullPath + L"\\", serializer);
		}
		else
		{
			// Check if it's an XML file
			size_t extPos = fullPath.find_last_of(L'.');
			if(extPos != std::wstring::npos && _wcsicmp(fullPath.substr(extPos).c_str(), L".xml") == 0)
			{
				// Skip text XML files in this pass
				if(!IsTextXMLFile(fullPath))
				{
					std::string strFullPath;
					int len2 = WideCharToMultiByte(CP_UTF8, 0, fullPath.c_str(), -1, NULL, 0, NULL, NULL);
					if(len2 > 0) {
						strFullPath.resize(len2 - 1);
						WideCharToMultiByte(CP_UTF8, 0, fullPath.c_str(), -1, &strFullPath[0], len2, NULL, NULL);
					}
					this->LogMsg("[LEKMOD] Loading regular XML: %s", strFullPath.c_str());
					
					bool loaded = serializer.Load(fullPath.c_str());
					if(loaded)
					{
						this->LogMsg("[LEKMOD] Successfully loaded regular XML: %s", strFullPath.c_str());
					}
					else
					{
						this->LogMsg("[LEKMOD] FAILED to load regular XML: %s", strFullPath.c_str());
					}
				}
			}
		}
	} while(FindNextFileW(hFind, &ffd) != 0);
	
	FindClose(hFind);
}

// Helper function to recursively load XML files - kept for backward compatibility
void CvDllDatabaseUtility::LoadXMLFilesRecursively(const std::wstring& wstrPath, Database::XMLSerializer& serializer)
{
	// First load text files, then other XML files
	LoadTextXMLFiles(wstrPath, serializer);
	LoadNonTextXMLFiles(wstrPath, serializer);
}
#endif

//------------------------------------------------------------------------------
//
// PRIVATE FUNCTIONS
//
//
// for logging
//
void CvDllDatabaseUtility::LogMsg(const char* format, ...) const
{
	const size_t kBuffSize = 1024;
	static char buf[kBuffSize];
	const uint uiFlags = 0;    // Default (0) is to not write to console and to time stamp

	va_list vl;
	va_start(vl,format);
	vsprintf_s(buf, format, vl);
	va_end(vl);

	LOGFILEMGR.GetLog("xml.log", uiFlags)->Msg(buf);
}



