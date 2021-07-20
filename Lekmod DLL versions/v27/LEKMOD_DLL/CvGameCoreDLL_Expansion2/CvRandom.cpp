/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */

#include "CvGameCoreDLLPCH.h"
#include "CvRandom.h"
#include "CvGlobals.h"
#include "FCallStack.h"
#include "FStlContainerSerialization.h"

#ifdef WIN32
#	include "Win32/FDebugHelper.h"
#endif//_WINPC

// include this after all other headers!
#include "LintFree.h"

#ifndef AUI_USE_SFMT_RNG
#ifdef AUI_RANDOM_FIX_CONSTANTS_SET_TO_MODULUS_2_POW_32
#define RANDOM_A      (214013u)
#define RANDOM_C      (2531011u)
#else
#define RANDOM_A      (1103515245)
#define RANDOM_C      (12345)
#endif
#define RANDOM_SHIFT  (16)

#ifdef AUI_BINOM_RNG
#define BINOM_SHIFT   (30)
#endif
#endif

CvRandom::CvRandom() :
	m_ulRandomSeed(0)
	, m_ulCallCount(0)
	, m_ulResetCount(0)
	, m_bSynchronous(false)
#ifdef _DEBUG
	, m_bExtendedCallStackDebugging(false)
	, m_kCallStacks()
	, m_seedHistory()
	, m_resolvedCallStacks()
#endif//_debug
{
	reset();
}

CvRandom::CvRandom(bool extendedCallStackDebugging) :
	m_ulRandomSeed(0)
	, m_ulCallCount(0)
	, m_ulResetCount(0)
	, m_bSynchronous(true)
#ifdef _DEBUG
	, m_bExtendedCallStackDebugging(extendedCallStackDebugging || GC.getOutOfSyncDebuggingEnabled())
	, m_kCallStacks()
	, m_seedHistory()
	, m_resolvedCallStacks()
#endif//_debug
{
	extendedCallStackDebugging;
}

CvRandom::CvRandom(const CvRandom& source) :
	m_ulRandomSeed(source.m_ulRandomSeed)
	, m_ulCallCount(source.m_ulCallCount)
	, m_ulResetCount(source.m_ulResetCount)
	, m_bSynchronous(source.m_bSynchronous)
#ifdef _DEBUG
	, m_bExtendedCallStackDebugging(source.m_bExtendedCallStackDebugging)
	, m_kCallStacks(source.m_kCallStacks)
	, m_seedHistory(source.m_seedHistory)
	, m_resolvedCallStacks(source.m_resolvedCallStacks)
#endif//_debug
{
#ifdef AUI_USE_SFMT_RNG
	m_MersenneTwister = source.m_MersenneTwister;
#endif
}

bool CvRandom::operator==(const CvRandom& source) const
{
#ifdef AUI_USE_SFMT_RNG
	return (m_MersenneTwister == source.m_MersenneTwister);
#else
	return(m_ulRandomSeed == source.m_ulRandomSeed);
#endif
}

bool CvRandom::operator!=(const CvRandom& source) const
{
	return !(*this == source);
}

#ifdef AUI_USE_SFMT_RNG
void CvRandom::syncInternals(const CvRandom& rhs)
{
	m_ulRandomSeed = rhs.m_ulRandomSeed;
	m_ulCallCount = rhs.m_ulCallCount;
	m_ulResetCount = rhs.m_ulResetCount;
	m_bSynchronous = rhs.m_bSynchronous;
	m_MersenneTwister = rhs.m_MersenneTwister;
}
#endif

CvRandom::~CvRandom()
{
	uninit();
}


#ifdef AUI_USE_SFMT_RNG
void CvRandom::init(uint32_t ulSeed)
#else
void CvRandom::init(unsigned long ulSeed)
#endif
{
	//--------------------------------
	// Init saved data
	reset(ulSeed);

	//--------------------------------
	// Init non-saved data
}


void CvRandom::uninit()
{
}


// FUNCTION: reset()
// Initializes data members that are serialized.
#ifdef AUI_USE_SFMT_RNG
void CvRandom::reset(uint32_t uiSeed)
#else
void CvRandom::reset(unsigned long ulSeed)
#endif
{
	//--------------------------------
	// Uninit class
	uninit();

	recordCallStack();
#ifdef AUI_USE_SFMT_RNG
	m_MersenneTwister.sfmt_init_gen_rand(uiSeed);
	m_ulCallCount = 0;
	m_ulRandomSeed = uiSeed;
#else
	m_ulRandomSeed = ulSeed;
#endif
	m_ulResetCount++;
}

#if defined(AUI_USE_SFMT_RNG) || defined(AUI_WARNING_FIXES)
unsigned int CvRandom::get(unsigned int uiNum, const char* pszLog)
#else
unsigned short CvRandom::get(unsigned short usNum, const char* pszLog)
#endif
{
#ifdef AUI_USE_SFMT_RNG
	unsigned int uiRtnValue = 0;
	if (uiNum > 1)
	{
		recordCallStack();
		m_ulCallCount++;
		uiRtnValue = m_MersenneTwister.sfmt_genrand_uint32() % uiNum;
	}
#else
	recordCallStack();
	m_ulCallCount++;

	unsigned long ulNewSeed = ((RANDOM_A * m_ulRandomSeed) + RANDOM_C);
#ifdef AUI_WARNING_FIXES
	unsigned short us = ((unsigned short)((((ulNewSeed >> RANDOM_SHIFT) & MAX_UNSIGNED_SHORT) * (uiNum)) / (MAX_UNSIGNED_SHORT + 1)));
#else
	unsigned short us = ((unsigned short)((((ulNewSeed >> RANDOM_SHIFT) & MAX_UNSIGNED_SHORT) * ((unsigned long)usNum)) / (MAX_UNSIGNED_SHORT + 1)));
#endif
#endif

	if(GC.getLogging())
	{
		int iRandLogging = GC.getRandLogging();
		if(iRandLogging > 0 && (m_bSynchronous || (iRandLogging & RAND_LOGGING_ASYNCHRONOUS_FLAG) != 0))
		{
#if !defined(FINAL_RELEASE)
			if(!gDLL->IsGameCoreThread() && gDLL->IsGameCoreExecuting() && m_bSynchronous)
			{
				CvAssertMsg(0, "App side is accessing the synchronous random number generator while the game core is running.");
			}
#endif
			CvGame& kGame = GC.getGame();
			if(kGame.getTurnSlice() > 0 || ((iRandLogging & RAND_LOGGING_PREGAME_FLAG) != 0))
			{
#ifdef AUI_USE_SFMT_RNG
				FILogFile* pLog = LOGFILEMGR.GetLog("RandCalls.csv", FILogFile::kDontTimeStamp, "Game Turn, Turn Slice, Range, Value, Initial Seed, Call Count, Instance, Type, Location\n");
#else
				FILogFile* pLog = LOGFILEMGR.GetLog("RandCalls.csv", FILogFile::kDontTimeStamp, "Game Turn, Turn Slice, Range, Value, Seed, Instance, Type, Location\n");
#endif
				if(pLog)
				{
					char szOut[1024] = {0};
#ifdef AUI_USE_SFMT_RNG
					sprintf_s(szOut, "%d, %d, %u, %u, %u, %u, %8x, %s, %s\n", kGame.getGameTurn(), kGame.getTurnSlice(), uiNum, uiRtnValue, m_ulRandomSeed, m_ulCallCount, (intptr_t)this, m_bSynchronous ? "sync" : "async", (pszLog != NULL) ? pszLog : "Unknown");
#elif defined(AUI_WARNING_FIXES)
					sprintf_s(szOut, "%d, %d, %u, %u, %u, %8x, %s, %s\n", kGame.getGameTurn(), kGame.getTurnSlice(), uiNum, (uint)us, getSeed(), (intptr_t)this, m_bSynchronous ? "sync" : "async", (pszLog != NULL) ? pszLog : "Unknown");
#else
					sprintf_s(szOut, "%d, %d, %u, %u, %u, %8x, %s, %s\n", kGame.getGameTurn(), kGame.getTurnSlice(), (uint)usNum, (uint)us, getSeed(), (uint)this, m_bSynchronous?"sync":"async", (pszLog != NULL)?pszLog:"Unknown");
#endif
					pLog->Msg(szOut);

#if !defined(FINAL_RELEASE)
					if((iRandLogging & RAND_LOGGING_CALLSTACK_FLAG) != 0)
					{
#ifdef _DEBUG
						if(m_bExtendedCallStackDebugging)
						{
							// Use the callstack from the extended callstack debugging system
							const FCallStack& callStack = m_kCallStacks.back();
							std::string stackTrace = callStack.toString(true, 6);
							pLog->Msg(stackTrace.c_str());
						}
						else
#endif
						{
#ifdef WIN32
							// Get callstack directly
							FCallStack callStack;
							FDebugHelper::GetInstance().GetCallStack(&callStack, 0, 8);
							std::string stackTrace = callStack.toString(true, 6);
							pLog->Msg(stackTrace.c_str());
#endif
						}
					}
#endif
				}
			}
		}
	}

#ifdef AUI_USE_SFMT_RNG
	return uiRtnValue;
#else
	m_ulRandomSeed = ulNewSeed;
#ifdef AUI_WARNING_FIXES
	return (uint)us;
#else
	return us;
#endif
#endif
}

#ifdef AUI_BINOM_RNG
unsigned int CvRandom::getBinom(unsigned int uiNum, const char* pszLog)
{
#ifdef AUI_USE_SFMT_RNG
	unsigned int uiRtnValue = 0;
	if (uiNum > 1)
	{
		recordCallStack();
		m_ulCallCount += uiNum;
		for (unsigned int uiI = 1; uiI < uiNum; uiI++)  // starts at 1 because the generation is not inclusive (so we need one less cycle than normal)
		{
			uiRtnValue += m_MersenneTwister.sfmt_genrand_uint32() & 1;
		}
	}
#else
	unsigned int uiRet = 0;
	unsigned long ulNewSeed = m_ulRandomSeed;
	if (uiNum > 1)
	{
		recordCallStack();
		m_ulCallCount += uiNum;
		for (unsigned int uiI = 1; uiI < uiNum; uiI++) // starts at 1 because the generation is not inclusive (so we need one less cycle than normal)
		{
			// no need to worry about masking with MAX_UNSIGNED_SHORT, max cycle number takes care of it
			ulNewSeed = (RANDOM_A * ulNewSeed) + RANDOM_C;
			uiRet += (ulNewSeed >> BINOM_SHIFT) & 1; // need the shift so results only repeat after 2^BINOM_SHIFT iterations
		}
	}
#endif

	if (GC.getLogging())
	{
		int iRandLogging = GC.getRandLogging();
		if (iRandLogging > 0 && (m_bSynchronous || (iRandLogging & RAND_LOGGING_ASYNCHRONOUS_FLAG) != 0))
		{
#if !defined(FINAL_RELEASE)
			if (!gDLL->IsGameCoreThread() && gDLL->IsGameCoreExecuting() && m_bSynchronous)
			{
				CvAssertMsg(0, "App side is accessing the synchronous random number generator while the game core is running.");
			}
#endif
			CvGame& kGame = GC.getGame();
			if (kGame.getTurnSlice() > 0 || ((iRandLogging & RAND_LOGGING_PREGAME_FLAG) != 0))
			{
#ifdef AUI_USE_SFMT_RNG
				FILogFile* pLog = LOGFILEMGR.GetLog("RandBinomCalls.csv", FILogFile::kDontTimeStamp, "Game Turn, Turn Slice, Range, Value, Initial Seed, Call Count, Instance, Type, Location\n");
#else
				FILogFile* pLog = LOGFILEMGR.GetLog("RandBinomCalls.csv", FILogFile::kDontTimeStamp, "Game Turn, Turn Slice, Range, Value, Initial Seed, Instance, Type, Location\n");
#endif
				if (pLog)
				{
					char szOut[1024] = { 0 };
#ifdef AUI_USE_SFMT_RNG
					sprintf_s(szOut, "%d, %d, %u, %u, %u, %u, %8x, %s, %s\n", kGame.getGameTurn(), kGame.getTurnSlice(), uiNum, uiRtnValue, m_ulRandomSeed, m_ulCallCount, (intptr_t)this, m_bSynchronous ? "sync" : "async", (pszLog != NULL) ? pszLog : "Unknown");
#else
					sprintf_s(szOut, "%d, %d, %u, %u, %u, %8x, %s, %s\n", kGame.getGameTurn(), kGame.getTurnSlice(), uiNum, uiRet, getSeed(), (intptr_t)this, m_bSynchronous ? "sync" : "async", (pszLog != NULL) ? pszLog : "Unknown");
#endif
					pLog->Msg(szOut);

#if !defined(FINAL_RELEASE)
					if ((iRandLogging & RAND_LOGGING_CALLSTACK_FLAG) != 0)
					{
#ifdef _DEBUG
						if (m_bExtendedCallStackDebugging)
						{
							// Use the callstack from the extended callstack debugging system
							const FCallStack& callStack = m_kCallStacks.back();
							std::string stackTrace = callStack.toString(true, 6);
							pLog->Msg(stackTrace.c_str());
						}
						else
#endif
						{
#ifdef WIN32
							// Get callstack directly
							FCallStack callStack;
							FDebugHelper::GetInstance().GetCallStack(&callStack, 0, 8);
							std::string stackTrace = callStack.toString(true, 6);
							pLog->Msg(stackTrace.c_str());
#endif
						}
					}
#endif
				}
			}
		}
	}

#ifdef AUI_USE_SFMT_RNG
	return uiRtnValue;
#else
	m_ulRandomSeed = ulNewSeed;

	return uiRet;
#endif
}
#endif

float CvRandom::getFloat()
{
#ifdef AUI_USE_SFMT_RNG
	return 1.0f;
#else
	return (((float)(get(MAX_UNSIGNED_SHORT))) / ((float)MAX_UNSIGNED_SHORT));
#endif
}


#ifdef AUI_USE_SFMT_RNG
void CvRandom::reseed(unsigned int uiNewSeed)
#else
void CvRandom::reseed(unsigned long ulNewValue)
#endif
{
	recordCallStack();
	m_ulResetCount++;
#ifdef AUI_USE_SFMT_RNG
	m_ulCallCount = 0;
	m_MersenneTwister.sfmt_init_gen_rand(uiNewSeed);
	m_ulRandomSeed = uiNewSeed;
#else
	m_ulRandomSeed = ulNewValue;
#endif
}


#ifdef AUI_USE_SFMT_RNG
std::pair<unsigned long, unsigned long> CvRandom::getSeed() const
{
	return std::pair<unsigned long, unsigned long>(m_ulCallCount, m_ulRandomSeed);
}
#else
unsigned long CvRandom::getSeed() const
{
	return m_ulRandomSeed;
}
#endif

unsigned long CvRandom::getCallCount() const
{
	return m_ulCallCount;
}

unsigned long CvRandom::getResetCount() const
{
	return m_ulResetCount;
}

void CvRandom::read(FDataStream& kStream)
{
	reset();

	// Version number to maintain backwards compatibility
	uint uiVersion;
	kStream >> uiVersion;

#ifdef AUI_USE_SFMT_RNG
	kStream >> m_MersenneTwister;
#endif
	kStream >> m_ulRandomSeed;
	kStream >> m_ulCallCount;
	kStream >> m_ulResetCount;
#ifdef _DEBUG
	kStream >> m_bExtendedCallStackDebugging;
	if(m_bExtendedCallStackDebugging)
	{
		kStream >> m_seedHistory;
		kStream >> m_resolvedCallStacks;
	}
#else
	bool b;
	kStream >> b;
#endif//_DEBUG
}


void CvRandom::write(FDataStream& kStream) const
{
	// Current version number
	uint uiVersion = 1;
	kStream << uiVersion;

#ifdef AUI_USE_SFMT_RNG
	kStream << m_MersenneTwister;
#endif
	kStream << m_ulRandomSeed;
	kStream << m_ulCallCount;
	kStream << m_ulResetCount;
#ifdef _DEBUG
	kStream << m_bExtendedCallStackDebugging;
	if(m_bExtendedCallStackDebugging)
	{
		resolveCallStacks();
		kStream << m_seedHistory;
		kStream << m_resolvedCallStacks;
	}
#else
	kStream << false;
#endif
}

void CvRandom::recordCallStack()
{
#ifdef _DEBUG
	if(m_bExtendedCallStackDebugging)
	{
		FDebugHelper& debugHelper = FDebugHelper::GetInstance();
		FCallStack callStack;
		debugHelper.GetCallStack(&callStack, 1, 8);
		m_kCallStacks.push_back(callStack);
		m_seedHistory.push_back(m_ulRandomSeed);
	}
#endif//_DEBUG
}

void CvRandom::resolveCallStacks() const
{
#ifdef _DEBUG
	std::vector<FCallStack>::const_iterator i;
	for(i = m_kCallStacks.begin() + m_resolvedCallStacks.size(); i != m_kCallStacks.end(); ++i)
	{
		const FCallStack callStack = *i;
		std::string stackTrace = callStack.toString(true);
		m_resolvedCallStacks.push_back(stackTrace);
	}
#endif//_DEBUG
}

const std::vector<std::string>& CvRandom::getResolvedCallStacks() const
{
#ifdef _DEBUG
	return m_resolvedCallStacks;
#else
	static std::vector<std::string> empty;
	return empty;
#endif//_debug
}

const std::vector<unsigned long>& CvRandom::getSeedHistory() const
{
#ifdef _DEBUG
	return m_seedHistory;
#else
	static std::vector<unsigned long> empty;
	return empty;
#endif//_DEBUG
}

bool CvRandom::callStackDebuggingEnabled() const
{
#ifdef _DEBUG
	return m_bExtendedCallStackDebugging;
#else
	return false;
#endif//_DEBUG
}

void CvRandom::setCallStackDebuggingEnabled(bool enabled)
{
#ifdef _DEBUG
	m_bExtendedCallStackDebugging = enabled;
#endif//_DEBUG
	enabled;
}

void CvRandom::clearCallstacks()
{
#ifdef _DEBUG
	m_kCallStacks.clear();
	m_seedHistory.clear();
	m_resolvedCallStacks.clear();
#endif//_DEBUG
}
FDataStream& operator<<(FDataStream& saveTo, const CvRandom& readFrom)
{
	readFrom.write(saveTo);
	return saveTo;
}

FDataStream& operator>>(FDataStream& loadFrom, CvRandom& writeTo)
{
	writeTo.read(loadFrom);
	return loadFrom;
}
#ifdef AUI_USE_SFMT_RNG
FDataStream& operator<<(FDataStream& saveTo, const SFMersenneTwister& readFrom)
{
	saveTo << readFrom.m_sfmt.idx;
	const w128_t * pstate = readFrom.m_sfmt.state;
	for (uint uiI = 0; uiI < SFMT_N; uiI++)
	{
		const int* iArray = pstate[uiI].si.m128i_i32;
		saveTo << iArray[0];
		saveTo << iArray[1];
		saveTo << iArray[2];
		saveTo << iArray[3];
	}
	return saveTo;
}

FDataStream& operator>>(FDataStream& loadFrom, SFMersenneTwister& writeTo)
{
	loadFrom >> writeTo.m_sfmt.idx;
	ALIGN16 w128_t * pstate = writeTo.m_sfmt.state;
	int iP1, iP2, iP3, iP4;
	for (uint uiI = 0; uiI < SFMT_N; uiI++)
	{
		loadFrom >> iP1;
		loadFrom >> iP2;
		loadFrom >> iP3;
		loadFrom >> iP4;
		pstate[uiI].si = _mm_set_epi32(iP4, iP3, iP2, iP1);
	}
	return loadFrom;
}
#endif
