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
	LogMsg("LEKMOD: CacheGameDatabaseData called, m_bGameDatabaseNeedsCaching = %d", m_bGameDatabaseNeedsCaching);

	//Do not cache everything if we don't need to.
	if(m_bGameDatabaseNeedsCaching == false)
	{
		LogMsg("LEKMOD: Skipping cache, database already cached");
		return true;
	}

	//The following code depends on a valid initialized database.
	bool bSuccess = true;

	//TODO: Figure out how to handle cases where Validation has failed.
	/*bSuccess &= */
	LogMsg("LEKMOD: Validating database");
	ValidateGameDatabase();
	
#ifdef LEKMOD_POST_DLC_DATA_LOADING
	// Add our post-DLC loading here, after validation but before any other data loading
	// This ensures LEKMOD loads after base game DLCs but before regular mods
	LogMsg("LEKMOD: Performing post-DLC loading");
	bSuccess &= PerformPostDLCLoading();
#endif

	//HACK Legacy 'FindInfoByType' support.
	//In order to support the legacy code still using the old infos system,
	//all of the id/type pairs must be added to gc.m_infosMap
	//I apologize for this horrendous code, please remove it in the near future.
	LogMsg("LEKMOD: Setting up legacy info types");
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

	LogMsg("LEKMOD: Loading global defines");
	bSuccess &= LoadGlobalDefines();
	LogMsg("LEKMOD: Prefetching game data");
	bSuccess &= PrefetchGameData();
	LogMsg("LEKMOD: Updating playable civilization counts");
	bSuccess &= UpdatePlayableCivilizationCounts();

	LogMsg("LEKMOD: Acquiring types");
	CvTypes::AcquireTypes(DB);

	LogMsg("LEKMOD: Setting global action info");
	bSuccess &= SetGlobalActionInfo();

	//Clear out database cache and tune for runtime use.
	LogMsg("LEKMOD: Clearing database cache");
	DB.ClearCountCache();

	//Log Database Memory statistics
	LogMsg(DB.CalculateMemoryStats());

	CvAssertMsg(bSuccess, "Failed to load Gameplay Database Data! Not Good!");

	if(bSuccess)
	{
		LogMsg("LEKMOD: Database caching complete, setting m_bGameDatabaseNeedsCaching = false");
		m_bGameDatabaseNeedsCaching = false;
	}

	return bSuccess;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::FlushGameDatabaseData()
{
	LogMsg("LEKMOD: FlushGameDatabaseData called, setting m_bGameDatabaseNeedsCaching = true");
	m_bGameDatabaseNeedsCaching = true;
	return true;
}
//------------------------------------------------------------------------------
bool CvDllDatabaseUtility::PerformDatabasePostProcessing()
{
	LogMsg("LEKMOD: PerformDatabasePostProcessing called");

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
#ifdef LEKMOD_POST_DLC_DATA_LOADING
bool CvDllDatabaseUtility::CheckXMLFileValidity(const wchar_t* wszFilePath, const char* szFilePath) const
{
    FILE* file = NULL;
    _wfopen_s(&file, wszFilePath, L"r");
    if (!file)
    {
        return false;
    }
    
    // Read first 1024 bytes to check
    char buffer[1025] = {0};
    size_t bytesRead = fread(buffer, 1, 1024, file);
    fclose(file);
    
    if (bytesRead == 0)
    {
        return false;
    }
    
    // Ensure null termination
    buffer[bytesRead] = 0;
    
    // If the file doesn't contain an XML declaration, log the beginning
    if (strstr(buffer, "<?xml") == NULL)
    {
        // Find the first 3 newlines to print first few lines
        const char* firstLine = buffer;
        const char* secondLine = NULL;
        const char* thirdLine = NULL;
        const char* fourthLine = NULL;
        
        for (size_t i = 0; i < bytesRead; i++)
        {
            if (buffer[i] == '\n')
            {
                if (!secondLine)
                {
                    buffer[i] = '\0';
                    secondLine = &buffer[i+1];
                }
                else if (!thirdLine)
                {
                    buffer[i] = '\0';
                    thirdLine = &buffer[i+1];
                }
                else if (!fourthLine)
                {
                    buffer[i] = '\0';
                    fourthLine = &buffer[i+1];
                    break;
                }
            }
        }
        
        // Log the first few lines of the file
        LogMsg("LEKMOD: Invalid XML file content for %s:", szFilePath);
        LogMsg("Line 1: %s", firstLine);
        if (secondLine) LogMsg("Line 2: %s", secondLine);
        if (thirdLine) LogMsg("Line 3: %s", thirdLine);
        if (fourthLine) LogMsg("Line 4: %s", fourthLine);
        
        return false;
    }
    
    return true;
}

bool CvDllDatabaseUtility::PerformPostDLCLoading()
{
	Database::Connection* db = GC.GetGameDatabase();
	if(!db)
	{
		LogMsg("LEKMOD: Failed to get game database");
		return false;
	}

	LogMsg("LEKMOD: Starting post-DLC loading transaction");
	
	// Begin transaction for all our changes
	bool bSuccess = false;
	try
	{
		db->BeginTransaction();

		// Get the DLC path - use the game's Assets/DLC folder
		CvString strDLCPath = "Assets\\DLC\\";
		LogMsg("LEKMOD: Looking for mods in DLC path: %s", strDLCPath.c_str());
		
		// Find LEKMOD folder (with any version number)
		WIN32_FIND_DATAW ffd;
		wchar_t wstrDLCPath[MAX_PATH];
		MultiByteToWideChar(CP_UTF8, 0, strDLCPath.c_str(), -1, wstrDLCPath, MAX_PATH);
		HANDLE hFind = FindFirstFileW((std::wstring(wstrDLCPath) + L"LEKMOD*").c_str(), &ffd);

		// Set to track which files we've already loaded
		std::set<std::wstring> loadedFiles;
		int iFilesLoaded = 0;

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					// Convert wchar_t filename to char for logging
					char szFileName[MAX_PATH];
					WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, szFileName, MAX_PATH, NULL, NULL);
					LogMsg("LEKMOD: Found non-directory: %s", szFileName);
					continue;
				}

				// Found a LEKMOD folder, now look for XML folder
				std::wstring wstrModPath = std::wstring(wstrDLCPath) + ffd.cFileName + L"\\XML\\";
				
				// Convert wchar_t filename to char for logging
				char szFileName[MAX_PATH];
				WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, szFileName, MAX_PATH, NULL, NULL);
				LogMsg("LEKMOD: Found mod directory: %s", szFileName);
				
				// Create XML serializer
				Database::XMLSerializer serializer(*db);

				// Recursively load all XML files in the directory and its subdirectories
				int iPreviousCount = loadedFiles.size();
				LoadXMLFilesRecursively(wstrModPath, serializer, db, loadedFiles);
				iFilesLoaded += (loadedFiles.size() - iPreviousCount);

			} while(FindNextFileW(hFind, &ffd) != 0);
			FindClose(hFind);
		}
		else
		{
			LogMsg("LEKMOD: No LEKMOD folders found in DLC directory");
		}

		// Commit all changes
		LogMsg("LEKMOD: Loaded %d XML files, committing transaction...", iFilesLoaded);
		db->EndTransaction();
		LogMsg("LEKMOD: Transaction committed successfully");
		bSuccess = true;
	}
	catch (...)
	{
		LogMsg("LEKMOD: Exception during post-DLC loading, rolling back transaction");
		try
		{
			db->RollbackTransaction();
		}
		catch (...)
		{
			LogMsg("LEKMOD: Failed to rollback transaction");
		}
	}

	return bSuccess;
}

// Helper function to recursively load XML files
void CvDllDatabaseUtility::LoadXMLFilesRecursively(const std::wstring& wstrPath, Database::XMLSerializer& serializer, Database::Connection* db, std::set<std::wstring>& loadedFiles)
{
	// Convert wstring to string for logging
	char szPath[MAX_PATH];
	WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, szPath, MAX_PATH, NULL, NULL);
	LogMsg("LEKMOD: Searching for XML files in: %s", szPath);
	
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFileW((wstrPath + L"*").c_str(), &ffd);

	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Skip . and .. directories
				if(wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
					continue;

				// Recursively process subdirectory
				LoadXMLFilesRecursively(wstrPath + ffd.cFileName + L"\\", serializer, db, loadedFiles);
			}
			else
			{
				// Check if this is an XML file
				wchar_t* pExtension = wcsrchr(ffd.cFileName, L'.');
				if (pExtension && _wcsicmp(pExtension, L".xml") == 0)
				{
					// Get the full file path
					std::wstring wstrFilePath = wstrPath + ffd.cFileName;
					
					// Convert wstring to string for logging
					char szFilePath[MAX_PATH];
					WideCharToMultiByte(CP_UTF8, 0, wstrFilePath.c_str(), -1, szFilePath, MAX_PATH, NULL, NULL);
					
					// Check if this file has already been loaded
					if (loadedFiles.find(wstrFilePath) == loadedFiles.end())
					{
						bool bSuccess = false;
						
						try
						{
							bSuccess = serializer.Load(wstrFilePath.c_str());
							if (bSuccess)
							{
								LogMsg("LEKMOD: Successfully loaded XML file: %s", szFilePath);
								loadedFiles.insert(wstrFilePath);
							}
							else
							{
								LogMsg("LEKMOD: Failed to load XML file with serializer, trying direct method: %s", szFilePath);
								
								// Try fallback method
								if (TryDirectSQLFromXML(db, wstrFilePath.c_str(), szFilePath))
								{
									LogMessageToFile("LEKMOD: Successfully loaded XML file using direct SQL: %s", szFilePath);
									loadedFiles.insert(wstrFilePath);
									bSuccess = true;
								}
								else
								{
									LogMessageToFile("LEKMOD: All methods failed for XML file: %s", szFilePath);
								}
							}
						}
						catch (...)
						{
							LogMsg("LEKMOD: Exception while loading XML file: %s", szFilePath);
							
							// Try fallback method after exception
							if (TryDirectSQLFromXML(db, wstrFilePath.c_str(), szFilePath))
							{
								LogMessageToFile("LEKMOD: Successfully loaded XML file using direct SQL after exception: %s", szFilePath);
								loadedFiles.insert(wstrFilePath);
							}
						}
					}
					else
					{
						LogMsg("LEKMOD: Skipping already loaded file: %s", szFilePath);
					}
				}
			}
		} while(FindNextFileW(hFind, &ffd) != 0);
		FindClose(hFind);
	}
	else
	{
		char szPath[MAX_PATH];
		WideCharToMultiByte(CP_UTF8, 0, wstrPath.c_str(), -1, szPath, MAX_PATH, NULL, NULL);
		LogMsg("LEKMOD: No files found in directory: %s", szPath);
	}
}

// Global logging helper function for free functions 
void LogMessageToFile(const char* format, ...)
{
    const size_t kBuffSize = 1024;
    static char buf[kBuffSize];
    const uint uiFlags = 0;  // Default (0) is to not write to console and to time stamp

    va_list vl;
    va_start(vl, format);
    vsprintf_s(buf, format, vl);
    va_end(vl);

    LOGFILEMGR.GetLog("xml.log", uiFlags)->Msg(buf);
}

// Helper function to try loading an XML file using direct SQL execution
bool TryDirectSQLFromXML(Database::Connection* db, const wchar_t* wszFilePath, const char* szFilePath)
{
    // This is a fallback method to try processing simple XML files directly
    // For simple XML files containing SQL statements
    
    FILE* file = NULL;
    _wfopen_s(&file, wszFilePath, L"r");
    if (!file)
    {
        return false;
    }
    
    // Read file contents
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    
    if (fileSize <= 0)
    {
        fclose(file);
        return false;
    }
    
    char* buffer = (char*)malloc(fileSize + 1);
    if (!buffer)
    {
        fclose(file);
        return false;
    }
    
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    fclose(file);
    
    if (bytesRead == 0)
    {
        free(buffer);
        return false;
    }
    
    buffer[bytesRead] = 0;
    
    // Look for SQL statements in the XML
    const char* pInsert = strstr(buffer, "INSERT INTO");
    const char* pUpdate = strstr(buffer, "UPDATE ");
    
    // Also look for XML Update structures
    const char* pXmlUpdate = strstr(buffer, "<Update>");
    
    bool bSuccess = false;
    
    if (pInsert || pUpdate)
    {
        // Found possible SQL, try to execute it directly
        try
        {
            bSuccess = db->Execute(buffer);
        }
        catch (...)
        {
            // Ignore errors
        }
    }
    else if (pXmlUpdate)
    {
        // Process the XML update structure
        // Find all tables inside the Update tag
        const char* pUpdateStart = pXmlUpdate + 8; // Skip "<Update>"
        const char* pUpdateEnd = strstr(pUpdateStart, "</Update>");
        
        if (!pUpdateEnd) {
            free(buffer);
            return false;
        }
        
        // Extract the Update content
        size_t updateContentLen = pUpdateEnd - pUpdateStart;
        char* updateContent = (char*)malloc(updateContentLen + 1);
        if (!updateContent) {
            free(buffer);
            return false;
        }
        
        strncpy_s(updateContent, updateContentLen + 1, pUpdateStart, updateContentLen);
        updateContent[updateContentLen] = 0;
        
        // Process each table update
        const char* pTableStart = updateContent;
        bool anyTableSuccess = false;
        
        while ((pTableStart = strstr(pTableStart, "<")) != NULL) {
            // Find the table name
            pTableStart++; // Skip "<"
            const char* pTableNameEnd = strstr(pTableStart, ">");
            if (!pTableNameEnd) break;
            
            int tableNameLen = pTableNameEnd - pTableStart;
            char* tableName = (char*)malloc(tableNameLen + 1);
            if (!tableName) break;
            
            strncpy_s(tableName, tableNameLen + 1, pTableStart, tableNameLen);
            tableName[tableNameLen] = 0;
            
            // Skip to the table content
            const char* pTableContentStart = pTableNameEnd + 1;
            
            // Find the end of this table section
            char closeTag[256];
            sprintf_s(closeTag, sizeof(closeTag), "</%s>", tableName);
            const char* pTableContentEnd = strstr(pTableContentStart, closeTag);
            if (!pTableContentEnd) {
                free(tableName);
                break;
            }
            
            // Extract the table content
            size_t tableContentLen = pTableContentEnd - pTableContentStart;
            char* tableContent = (char*)malloc(tableContentLen + 1);
            if (!tableContent) {
                free(tableName);
                break;
            }
            
            strncpy_s(tableContent, tableContentLen + 1, pTableContentStart, tableContentLen);
            tableContent[tableContentLen] = 0;
            
            // Process all Update commands for this table
            const char* pUpdateCmdStart = tableContent;
            bool tableSuccess = false;
            
            while ((pUpdateCmdStart = strstr(pUpdateCmdStart, "<Update>")) != NULL) {
                pUpdateCmdStart += 8; // Skip "<Update>"
                
                // Find the end of this update command
                const char* pUpdateCmdEnd = strstr(pUpdateCmdStart, "</Update>");
                if (!pUpdateCmdEnd) break;
                
                // Extract update command content
                size_t updateCmdLen = pUpdateCmdEnd - pUpdateCmdStart;
                char* updateCmd = (char*)malloc(updateCmdLen + 1);
                if (!updateCmd) break;
                
                strncpy_s(updateCmd, updateCmdLen + 1, pUpdateCmdStart, updateCmdLen);
                updateCmd[updateCmdLen] = 0;
                
                // Find Set and Where parts
                const char* pSetStart = strstr(updateCmd, "<Set");
                const char* pWhereStart = strstr(updateCmd, "<Where");
                
                if (pSetStart && pWhereStart) {
                    // Extract Set values
                    char setClause[1024] = {0};
                    const char* pSetTagEnd = strstr(pSetStart, ">");
                    if (!pSetTagEnd) {
                        free(updateCmd);
                        pUpdateCmdStart = pUpdateCmdEnd + 9; // Skip "</Update>"
                        continue;
                    }
                    
                    // Extract values from Set tag attributes
                    const char* pSetAttrStart = pSetStart + 4; // Skip "<Set"
                    while (pSetAttrStart < pSetTagEnd) {
                        // Skip whitespace
                        while (pSetAttrStart < pSetTagEnd && isspace(*pSetAttrStart)) 
                            pSetAttrStart++;
                        
                        if (pSetAttrStart >= pSetTagEnd) break;
                        
                        // Find attribute name end
                        const char* pSetAttrNameEnd = strpbrk(pSetAttrStart, "= \t\r\n");
                        if (!pSetAttrNameEnd || pSetAttrNameEnd >= pSetTagEnd) break;
                        
                        // Extract attribute name
                        int attrNameLen = pSetAttrNameEnd - pSetAttrStart;
                        char attrName[256] = {0};
                        strncpy_s(attrName, sizeof(attrName), pSetAttrStart, attrNameLen);
                        
                        // Find attribute value
                        const char* pSetAttrValueStart = strchr(pSetAttrNameEnd, '"');
                        if (!pSetAttrValueStart || pSetAttrValueStart >= pSetTagEnd) break;
                        pSetAttrValueStart++; // Skip "
                        
                        const char* pSetAttrValueEnd = strchr(pSetAttrValueStart, '"');
                        if (!pSetAttrValueEnd || pSetAttrValueEnd >= pSetTagEnd) break;
                        
                        // Extract attribute value
                        int attrValueLen = pSetAttrValueEnd - pSetAttrValueStart;
                        char attrValue[256] = {0};
                        strncpy_s(attrValue, sizeof(attrValue), pSetAttrValueStart, attrValueLen);
                        
                        // Add to SET clause
                        if (setClause[0] == 0) {
                            sprintf_s(setClause, sizeof(setClause), "%s = '%s'", attrName, attrValue);
                        } else {
                            char temp[1024];
                            sprintf_s(temp, sizeof(temp), ", %s = '%s'", attrName, attrValue);
                            strcat_s(setClause, sizeof(setClause), temp);
                        }
                        
                        // Move to next attribute
                        pSetAttrStart = pSetAttrValueEnd + 1;
                    }
                    
                    // Extract Where values
                    char whereClause[1024] = {0};
                    const char* pWhereTagEnd = strstr(pWhereStart, ">");
                    if (!pWhereTagEnd) {
                        free(updateCmd);
                        pUpdateCmdStart = pUpdateCmdEnd + 9; // Skip "</Update>"
                        continue;
                    }
                    
                    // Extract values from Where tag attributes
                    const char* pWhereAttrStart = pWhereStart + 6; // Skip "<Where"
                    while (pWhereAttrStart < pWhereTagEnd) {
                        // Skip whitespace
                        while (pWhereAttrStart < pWhereTagEnd && isspace(*pWhereAttrStart)) 
                            pWhereAttrStart++;
                        
                        if (pWhereAttrStart >= pWhereTagEnd) break;
                        
                        // Find attribute name end
                        const char* pWhereAttrNameEnd = strpbrk(pWhereAttrStart, "= \t\r\n");
                        if (!pWhereAttrNameEnd || pWhereAttrNameEnd >= pWhereTagEnd) break;
                        
                        // Extract attribute name
                        int attrNameLen = pWhereAttrNameEnd - pWhereAttrStart;
                        char attrName[256] = {0};
                        strncpy_s(attrName, sizeof(attrName), pWhereAttrStart, attrNameLen);
                        
                        // Find attribute value
                        const char* pWhereAttrValueStart = strchr(pWhereAttrNameEnd, '"');
                        if (!pWhereAttrValueStart || pWhereAttrValueStart >= pWhereTagEnd) break;
                        pWhereAttrValueStart++; // Skip "
                        
                        const char* pWhereAttrValueEnd = strchr(pWhereAttrValueStart, '"');
                        if (!pWhereAttrValueEnd || pWhereAttrValueEnd >= pWhereTagEnd) break;
                        
                        // Extract attribute value
                        int attrValueLen = pWhereAttrValueEnd - pWhereAttrValueStart;
                        char attrValue[256] = {0};
                        strncpy_s(attrValue, sizeof(attrValue), pWhereAttrValueStart, attrValueLen);
                        
                        // Add to WHERE clause
                        if (whereClause[0] == 0) {
                            sprintf_s(whereClause, sizeof(whereClause), "%s = '%s'", attrName, attrValue);
                        } else {
                            char temp[1024];
                            sprintf_s(temp, sizeof(temp), " AND %s = '%s'", attrName, attrValue);
                            strcat_s(whereClause, sizeof(whereClause), temp);
                        }
                        
                        // Move to next attribute
                        pWhereAttrStart = pWhereAttrValueEnd + 1;
                    }
                    
                    // Build and execute SQL update statement
                    if (setClause[0] != 0 && whereClause[0] != 0) {
                        char sqlStatement[4096];
                        sprintf_s(sqlStatement, sizeof(sqlStatement), "UPDATE %s SET %s WHERE %s", 
                                 tableName, setClause, whereClause);
                        
                        try {
                            LogMessageToFile("LEKMOD: Executing SQL from XML Update: %s", sqlStatement);
                            bool cmdSuccess = db->Execute(sqlStatement);
                            if (cmdSuccess) {
                                tableSuccess = true;
                            }
                        } catch (...) {
                            LogMessageToFile("LEKMOD: Failed to execute SQL from XML Update: %s", sqlStatement);
                        }
                    }
                }
                
                free(updateCmd);
                pUpdateCmdStart = pUpdateCmdEnd + 9; // Skip "</Update>"
            }
            
            if (tableSuccess) {
                anyTableSuccess = true;
            }
            
            free(tableContent);
            free(tableName);
            
            // Move to next table
            pTableStart = pTableContentEnd + strlen(closeTag);
        }
        
        free(updateContent);
        bSuccess = anyTableSuccess;
    }
    
    free(buffer);
    return bSuccess;
}
#endif

