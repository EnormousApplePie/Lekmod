/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

//
//  AUTHOR:  Casey O'Toole  --  8/21/2002
//
//  PURPOSE: A* Pathfinding - based off of A* Explorer from "AI Game Programming Wisdom"
//

#ifndef		CVASTAR_H
#define		CVASTAR_H
#pragma		once

#include	"CvAStarNode.h"

class CvAStar;

#ifdef AUI_CONSTIFY
typedef int(*CvAPointFunc)(int, int, const void*, const CvAStar*);
#else
typedef int(*CvAPointFunc)(int, int, const void*, CvAStar*);
#endif
typedef int(*CvAHeuristic)(int, int, int, int);
typedef int(*CvAStarFunc)(CvAStarNode*, CvAStarNode*, int, const void*, CvAStar*);
typedef int(*CvANumExtraChildren)(CvAStarNode*, CvAStar*);
typedef int(*CvAGetExtraChild)(CvAStarNode*, int, int&, int&, CvAStar*);
typedef void(*CvABegin)(const void*, CvAStar*);
typedef void(*CvAEnd)(const void*, CvAStar*);

// PATHFINDER FLAGS
// WARNING: Some of these flags are passed into the unit mission and stored in the missions iFlags member.
//          Because the mission's iFlags are sharing space with the pathfinder flags, we currently have mission
//			modifier flags listed below that really don't have anything to do with the pathfinder.
//			A fix for this would be to have the mission contain separate pathfinder and modifier flags.
// These flags determine plots that can be entered
#define MOVE_TERRITORY_NO_UNEXPLORED		(0x00000001)
#define MOVE_TERRITORY_NO_ENEMY				(0x00000002)
#define MOVE_IGNORE_STACKING                (0x00000004)
// These two tell about presence of enemy units
#define MOVE_UNITS_IGNORE_DANGER			(0x00000008)
#define MOVE_UNITS_THROUGH_ENEMY			(0x00000010)
// Used for human player movement
#define MOVE_DECLARE_WAR					(0x00000020)
// Used for AI group attacks (??). Not really a pathfinder flag
#define MISSION_MODIFIER_DIRECT_ATTACK		(0x00000040)
#define MISSION_MODIFIER_NO_DEFENSIVE_SUPPORT (0x00000100)

#define MOVE_MAXIMIZE_EXPLORE				(0x00000080)
//
// Used for route information
#define MOVE_ANY_ROUTE					    (0x80000000) // because we're passing in the player number as well as the route flag
#define MOVE_ROUTE_ALLOW_UNEXPLORED			(0x40000000) // When searching for a route, allow the search to use unrevealed plots
//#define MOVE_NON_WAR_ROUTE				 // we're passing the player id and other flags in as well. This flag checks to see if it can get from point to point without going into territory with a team we're at war with

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  CLASS:      CvAStar
//
//  DESC:       CvAStar pathfinding class
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvAStar
{

	//--------------------------------------- PUBLIC METHODS -------------------------------------------
public:
	enum RANGES
	{
		SCRATCH_BUFFER_SIZE = 512
	};
	// Constructor
	CvAStar();

	// Destructor
	~CvAStar();

	// Initializes the CvAStar class. iSize = Dimensions of Pathing Grid(ie. [iSize][iSize]
	void Initialize(int iColumns, int iRows, bool bWrapX, bool bWrapY, CvAPointFunc IsPathDestFunc, CvAPointFunc DestValidFunc, CvAHeuristic HeuristicFunc, CvAStarFunc CostFunc, CvAStarFunc ValidFunc, CvAStarFunc NotifyChildFunc, CvAStarFunc NotifyListFunc, CvANumExtraChildren NumExtraChildrenFunc, CvAGetExtraChild GetExtraChildFunc, CvABegin InitializeFunc, CvAEnd UninitializeFunc, const void* pData);

	void DeInit();		// free memory

	// Generates a path
	bool GeneratePath(int iXstart, int iYstart, int iXdest, int iYdest, int iInfo = 0, bool bReuse = false);

	// Gets the last node in the path (from the origin) - Traverse the parents to get full path (linked list starts at destination)
#ifdef AUI_CONSTIFY
	inline CvAStarNode* GetLastNode() const
#else
	inline CvAStarNode* GetLastNode()
#endif
	{
		return m_pBest;
	}

#ifdef AUI_ASTAR_GET_PENULTIMATE_NODE
	// Gets the node before the last node in the path (from the origin)
	inline CvAStarNode* GetPenultimateNode() const
	{
		return (m_pBest ? m_pBest->m_pParent : NULL);
}
#endif

#ifdef AUI_CONSTIFY
	inline bool IsPathStart(int iX, int iY) const
#else
	inline bool IsPathStart(int iX, int iY)
#endif
	{
		return ((m_iXstart == iX) && (m_iYstart == iY));
	}

#ifdef AUI_CONSTIFY
	inline bool IsPathDest(int iX, int iY) const
#else
	inline bool IsPathDest(int iX, int iY)
#endif
	{
		if(udIsPathDest && udIsPathDest(iX, iY, m_pData, this))
		{
			return TRUE;
		}
		return FALSE;
	}

#ifdef AUI_CONSTIFY
	inline int GetStartX() const
#else
	inline int GetStartX()
#endif
	{
		return m_iXstart;
	}

#ifdef AUI_CONSTIFY
	inline int GetStartY() const
#else
	inline int GetStartY()
#endif
	{
		return m_iYstart;
	}

#ifdef AUI_CONSTIFY
	inline int GetDestX() const
#else
	inline int GetDestX()
#endif
	{
		return m_iXdest;
	}

#ifdef AUI_CONSTIFY
	inline int GetDestY() const
#else
	inline int GetDestY()
#endif
	{
		return m_iYdest;
	}

#ifdef AUI_CONSTIFY
	inline int GetInfo() const
#else
	inline int GetInfo()
#endif
	{
		return m_iInfo;
	}

	inline void ForceReset()
	{
		m_bForceReset = true;
	}

#ifdef AUI_ASTAR_TURN_LIMITER
	inline int GetMaxTurns() const
	{
		return m_iMaxTurns;
	}

	inline void SetMaxTurns(int iMaxTurns)
	{
		if (m_bDataChangeInvalidatesCache && m_iMaxTurns != iMaxTurns)
			m_bForceReset = true;
		m_iMaxTurns = iMaxTurns;
	}
#endif

#ifdef AUI_ASTAR_TURN_LIMITER
	inline void SetData(const void* pData, int iMaxTurns = MAX_INT)
	{
		if (m_bDataChangeInvalidatesCache && (m_pData != pData || m_iMaxTurns != iMaxTurns))
#else
	inline void SetData(const void* pData)
	{
		if(m_bDataChangeInvalidatesCache && m_pData != pData)
#endif
			m_bForceReset = true;
		m_pData = pData;
#ifdef AUI_ASTAR_TURN_LIMITER
		m_iMaxTurns = iMaxTurns;
#endif
	}

	inline bool IsMPCacheSafe() const
	{
		return m_bIsMPCacheSafe;
	}

	inline bool SetMPCacheSafe(bool bState)
	{
		bool bOldState = m_bIsMPCacheSafe;
		if(bState != m_bIsMPCacheSafe)
		{
			m_bForceReset = true;
			m_bIsMPCacheSafe = bState;
		}

		return bOldState;
	}

	inline bool GetDataChangeInvalidatesCache() const
	{
		return m_bDataChangeInvalidatesCache;
	}

	inline bool SetDataChangeInvalidatesCache(bool bState)
	{
		bool bOldState = m_bDataChangeInvalidatesCache;
		m_bDataChangeInvalidatesCache = bState;

		return bOldState;
	}

#ifdef AUI_CONSTIFY
	inline CvAPointFunc GetIsPathDestFunc() const
#else
	inline CvAPointFunc GetIsPathDestFunc()
#endif
	{
		return udIsPathDest;
	}

	inline void SetIsPathDestFunc(CvAPointFunc newIsPathDestFunc)
	{
		udIsPathDest = newIsPathDestFunc;
	}

#ifdef AUI_CONSTIFY
	inline CvAPointFunc GetDestValidFunc() const
#else
	inline CvAPointFunc GetDestValidFunc()
#endif
	{
		return udDestValid;
	}

	inline void SetDestValidFunc(CvAPointFunc newDestValidFunc)
	{
		udDestValid = newDestValidFunc;
	}

#ifdef AUI_CONSTIFY
	inline CvAHeuristic GetHeuristicFunc() const
#else
	inline CvAHeuristic GetHeuristicFunc()
#endif
	{
		return udHeuristic;
	}

	inline void SetHeuristicFunc(CvAHeuristic newHeuristicFunc)
	{
		udHeuristic = newHeuristicFunc;
	}

#if defined(AUI_CONSTIFY)
	inline CvAStarFunc GetCostFunc() const
#else
	inline CvAStarFunc GetCostFunc()
#endif
	{
		return udCost;
	}

	inline void SetCostFunc(CvAStarFunc newCostFunc)
	{
		udCost = newCostFunc;
	}

#ifdef AUI_CONSTIFY
	inline CvAStarFunc GetValidFunc() const
#else
	inline CvAStarFunc GetValidFunc()
#endif
	{
		return udValid;
	}

	inline void SetValidFunc(CvAStarFunc newValidFunc)
	{
		udValid = newValidFunc;
	}

#if defined(AUI_CONSTIFY)
	inline CvAStarFunc GetNotifyChildFunc() const
#else
	inline CvAStarFunc GetNotifyChildFunc()
#endif
	{
		return udNotifyChild;
	}

	inline void SetNotifyChildFunc(CvAStarFunc newNotifyChildFunc)
	{
		udNotifyChild = newNotifyChildFunc;
	}

#if defined(AUI_CONSTIFY)
	inline CvAStarFunc GetNotifyListFunc() const
#else
	inline CvAStarFunc GetNotifyListFunc()
#endif
	{
		return udNotifyList;
	}

	inline void SetNotifyListFunc(CvAStarFunc newNotifyListFunc)
	{
		udNotifyList = newNotifyListFunc;
	}

#ifdef AUI_CONSTIFY
	inline CvANumExtraChildren GetNumExtraChildrenFunc() const
#else
	inline CvANumExtraChildren GetNumExtraChildrenFunc()
#endif
	{
		return udNumExtraChildrenFunc;
	}

	inline void SetNumExtraChildrenFunc(CvANumExtraChildren newNumExtraChildrenFunc)
	{
		udNumExtraChildrenFunc = newNumExtraChildrenFunc;
	}

#ifdef AUI_CONSTIFY
	inline CvAGetExtraChild GetExtraChildGetterFunc() const
#else
	inline CvAGetExtraChild GetExtraChildGetterFunc()
#endif
	{
		return udGetExtraChildFunc;
	}

	inline void SetExtraChildGetterFunc(CvAGetExtraChild newGetExtraChildFunc)
	{
		udGetExtraChildFunc = newGetExtraChildFunc;
	}

	void AddToOpen(CvAStarNode* addnode);

	// Copy the supplied node and its parent nodes into an array of simpler path nodes for caching purposes.
	// It is ok to pass in NULL, the resulting array will contain zero elements
	static void CopyPath(const CvAStarNode* pkEndNode, CvPathNodeArray& kPathArray);

#ifndef AUI_ASTAR_MINOR_OPTIMIZATION
	void* GetScratchPointer1() { return m_pScratchPtr1; }
	void  SetScratchPointer1(void* pPtr) { m_pScratchPtr1 = pPtr; }
	void* GetScratchPointer2() { return m_pScratchPtr2; }
	void  SetScratchPointer2(void* pPtr) { m_pScratchPtr1 = pPtr; }
#endif

#ifdef AUI_CONSTIFY
	const void* CvAStar::GetScratchBuffer() const { return &m_ScratchBuffer[0]; }
#endif
	void* GetScratchBuffer() { return &m_ScratchBuffer[0]; }
	//--------------------------------------- PROTECTED FUNCTIONS -------------------------------------------
protected:

	int     Step();
	void    Reset()
	{
		m_pBest = NULL;
	}

	CvAStarNode*	GetBest();

#ifdef AUI_ASTAR_PRECALCULATE_NEIGHBORS_ON_INITIALIZE
	void PrecalcNeighbors(CvAStarNode* node) const;
#endif
	void CreateChildren(CvAStarNode* node);
	void LinkChild(CvAStarNode* node, CvAStarNode* check);
	void UpdateOpenNode(CvAStarNode* node);
	void UpdateParents(CvAStarNode* node);

	void Push(CvAStarNode* node);
	CvAStarNode* Pop();

#ifdef AUI_CONSTIFY
	inline int xRange(int iX) const;
	inline int yRange(int iY) const;
	inline bool isValid(int iX, int iY) const;
#else
	inline int xRange(int iX);
	inline int yRange(int iY);
	inline bool isValid(int iX, int iY);
#endif

	inline int udFunc(CvAStarFunc func, CvAStarNode* param1, CvAStarNode* param2, int data, const void* cb);

	//--------------------------------------- PROTECTED DATA -------------------------------------------
protected:
	CvAPointFunc udIsPathDest;					// Determines if this node is the destination of the path
	CvAPointFunc udDestValid;				    // Determines destination is valid
	CvAHeuristic udHeuristic;				    // Determines heuristic cost
	CvAStarFunc udCost;						    // Called when cost value is need
	CvAStarFunc udValid;					    // Called to check validity of a coordinate
	CvAStarFunc udNotifyChild;				    // Called when child is added/checked (LinkChild)
	CvAStarFunc udNotifyList;				    // Called when node is added to Open/Closed list
	CvANumExtraChildren udNumExtraChildrenFunc; // Determines if CreateChildren should consider any additional nodes
	CvAGetExtraChild udGetExtraChildFunc;	    // Get the extra children nodes
	CvABegin udInitializeFunc;					// Called at the start, to initialize any run specific data
	CvAEnd udUninitializeFunc;					// Called at the end to uninitialize any run specific data


	const void* m_pData;			// Data passed back to functions

#ifdef AUI_ASTAR_TURN_LIMITER
	int m_iMaxTurns;				// Pathfinder never lets a path's turn cost become higher than this number
#endif

	int m_iColumns;					// Used to calculate node->number
	int m_iRows;					// Used to calculate node->number
	int m_iXstart;
	int m_iYstart;
	int m_iXdest;
	int m_iYdest;
	int m_iInfo;

	bool m_bWrapX;
	bool m_bWrapY;
	bool m_bForceReset;
	bool m_bIsMPCacheSafe;
	bool m_bDataChangeInvalidatesCache;
#ifdef AUI_ASTAR_MINOR_OPTIMIZATION
	bool m_bIsMultiplayer;
#endif

	CvAStarNode* m_pOpen;            // The open list
	CvAStarNode* m_pOpenTail;        // The open list tail pointer (to speed up inserts)
	CvAStarNode* m_pClosed;          // The closed list
	CvAStarNode* m_pBest;            // The best node
	CvAStarNode* m_pStackHead;		// The Push/Pop stack head

	CvAStarNode** m_ppaaNodes;

#ifndef AUI_ASTAR_MINOR_OPTIMIZATION
	// Scratch buffers
	void* m_pScratchPtr1;						// Will be cleared to NULL before each GeneratePath call
	void* m_pScratchPtr2;						// Will be cleared to NULL before each GeneratePath call
#endif

	char  m_ScratchBuffer[SCRATCH_BUFFER_SIZE];	// Will NOT be modified directly by CvAStar
};


#ifdef AUI_CONSTIFY
inline int CvAStar::xRange(int iX) const
#else
inline int CvAStar::xRange(int iX)
#endif
{
	if(m_bWrapX)
	{
		if(iX < 0)
		{
			return (m_iColumns + (iX % m_iColumns));
		}
		else if(iX >= m_iColumns)
		{
			return (iX % m_iColumns);
		}
		else
		{
			return iX;
		}
	}
	else
	{
		return iX;
	}
}


#ifdef AUI_CONSTIFY
inline int CvAStar::yRange(int iY) const
#else
inline int CvAStar::yRange(int iY)
#endif
{
	if(m_bWrapY)
	{
		if(iY < 0)
		{
			return (m_iRows + (iY % m_iRows));
		}
		else if(iY >= m_iRows)
		{
			return (iY % m_iRows);
		}
		else
		{
			return iY;
		}
	}
	else
	{
		return iY;
	}
}


#ifdef AUI_CONSTIFY
inline bool CvAStar::isValid(int iX, int iY) const
#else
inline bool CvAStar::isValid(int iX, int iY)
#endif
{
	if((iX < 0) || (iX >= m_iColumns))
	{
		return false;
	}

	if((iY < 0) || (iY >= m_iRows))
	{
		return false;
	}

	return true;
}


inline int CvAStar::udFunc(CvAStarFunc func, CvAStarNode* param1, CvAStarNode* param2, int data, const void* cb)
{
	return (func) ? func(param1, param2, data, cb, this) : 1;
}

// C-style non-member functions (used by path finder)
int PathAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int PathValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int PathDestValid(int iToX, int iToY, const void* pointer, const CvAStar* finder);

int PathDest(int iToX, int iToyY, const void* pointer, const CvAStar* finder);
#else
int PathDestValid(int iToX, int iToY, const void* pointer, CvAStar* finder);

int PathDest(int iToX, int iToyY, const void* pointer, CvAStar* finder);
#endif
int PathHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int PathCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int PathNodeAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int IgnoreUnitsDestValid(int iToX, int iToY, const void* pointer, const CvAStar* finder);
#else
int IgnoreUnitsDestValid(int iToX, int iToY, const void* pointer, CvAStar* finder);
#endif
int IgnoreUnitsCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int IgnoreUnitsValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int IgnoreUnitsPathAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int StepDestValid(int iToX, int iToY, const void* pointer, const CvAStar* finder);
#else
int StepDestValid(int iToX, int iToY, const void* pointer, CvAStar* finder);
#endif
int StepHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int StepValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int StepValidAnyArea(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int StepCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int StepAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int RouteValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int RouteGetNumExtraChildren(CvAStarNode* node,  CvAStar* finder);
int RouteGetExtraChild(CvAStarNode* node, int iIndex, int& iX, int& iY, CvAStar* finder);
int WaterRouteValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int AreaValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int JoinArea(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int LandmassValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int JoinLandmass(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int InfluenceDestValid(int iToX, int iToY, const void* pointer, const CvAStar* finder);
#else
int InfluenceDestValid(int iToX, int iToY, const void* pointer, CvAStar* finder);
#endif
int InfluenceHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int InfluenceValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int InfluenceCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int InfluenceAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int BuildRouteCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int BuildRouteValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int UIPathAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int UIPathValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int AttackPathAdd(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int AttackPathDest(int iToX, int iToY, const void* pointer, const CvAStar* finder);
int AttackFortifiedPathDest(int iToX, int iToY, const void* pointer, const CvAStar* finder);
int AttackCityPathDest(int iToX, int iToY, const void* pointer, const CvAStar* finder);
#else
int AttackPathDest(int iToX, int iToY, const void* pointer, CvAStar* finder);
int AttackFortifiedPathDest(int iToX, int iToY, const void* pointer, CvAStar* finder);
int AttackCityPathDest(int iToX, int iToY, const void* pointer, CvAStar* finder);
#endif
int TacticalAnalysisMapPathValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#ifdef AUI_CONSTIFY
int FindValidDestinationDest(int iToX, int iToY, const void* pointer, const CvAStar* finder);
#else
int FindValidDestinationDest(int iToX, int iToY, const void* pointer, CvAStar* finder);
#endif
int FindValidDestinationPathValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
#if defined(AUI_ASTAR_TURN_LIMITER)
int TurnsToReachTarget(UnitHandle pUnit, const CvPlot* pTarget, bool bReusePaths = false, bool bIgnoreUnits = false, bool bIgnoreStacking = false, int iTargetTurns = MAX_INT);
int TurnsToReachTargetFromPlot(UnitHandle pUnit, const CvPlot* pTarget, const CvPlot* pFromPlot, bool bReusePaths = false, bool bIgnoreUnits = false, bool bIgnoreStacking = false, int iTargetTurns = MAX_INT);
#else
int TurnsToReachTarget(UnitHandle pUnit, CvPlot* pTarget, bool bReusePaths=false, bool bIgnoreUnits=false, bool bIgnoreStacking=false);
#endif
bool CanReachInXTurns(UnitHandle pUnit, CvPlot* pTarget, int iTurns, bool bIgnoreUnits=false, int* piTurns = NULL);
int TradeRouteHeuristic(int iFromX, int iFromY, int iToX, int iToY);
int TradeRouteLandPathCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int TradeRouteLandValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int TradeRouteWaterPathCost(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
int TradeRouteWaterValid(CvAStarNode* parent, CvAStarNode* node, int data, const void* pointer, CvAStar* finder);
void UnitPathInitialize(const void* pointer, CvAStar* finder);
void UnitPathUninitialize(const void* pointer, CvAStar* finder);
void TradePathInitialize(const void* pointer, CvAStar* finder);
void TradePathUninitialize(const void* pointer, CvAStar* finder);

#ifdef AUI_ASTAR_TWEAKED_OPTIMIZED_BUT_CAN_STILL_USE_ROADS
void IncreaseMoveRangeForRoads(const CvUnit* pUnit, int& iRange);
int GetIncreasedMoveRangeForRoads(const CvUnit* pUnit, int iRange);
#endif

// Derived classes (for more convenient access to pathfinding)
class CvTwoLayerPathFinder: public CvAStar
{
public:
	CvTwoLayerPathFinder();
	~CvTwoLayerPathFinder();
	void Initialize(int iColumns, int iRows, bool bWrapX, bool bWrapY, CvAPointFunc IsPathDestFunc, CvAPointFunc DestValidFunc, CvAHeuristic HeuristicFunc, CvAStarFunc CostFunc, CvAStarFunc ValidFunc, CvAStarFunc NotifyChildFunc, CvAStarFunc NotifyListFunc, CvABegin InitializeFunc, CvAEnd UninitializeFunc, const void* pData);
	void DeInit();
#ifdef AUI_ASTAR_MINOR_OPTIMIZATION
	inline CvAStarNode* GetPartialMoveNode(int iCol, int iRow) const;
#elif defined(AUI_CONSTIFY)
	CvAStarNode* GetPartialMoveNode(int iCol, int iRow) const;
#else
	CvAStarNode* GetPartialMoveNode(int iCol, int iRow);
#endif
	CvPlot* GetPathEndTurnPlot() const;

#ifdef AUI_ASTAR_TURN_LIMITER
	bool GenerateUnitPath(const CvUnit* pkUnit, int iXstart, int iYstart, int iXdest, int iYdest, int iInfo = 0, bool bReuse = false, int iTargetTurns = MAX_INT);
#else
	bool GenerateUnitPath(const CvUnit* pkUnit, int iXstart, int iYstart, int iXdest, int iYdest, int iInfo = 0, bool bReuse = false);
#endif

private:
	CvAStarNode** m_ppaaPartialMoveNodes;
};

class CvStepPathFinder: public CvAStar
{
public:
	int GetStepDistanceBetweenPoints(PlayerTypes ePlayer, PlayerTypes eEnemy, CvPlot* pStartPlot, CvPlot* pEndPlot);
	bool DoesPathExist(PlayerTypes ePlayer, PlayerTypes eEnemy, CvPlot* pStartPlot, CvPlot* pEndPlot);
	CvPlot* GetLastOwnedPlot(PlayerTypes ePlayer, PlayerTypes eEnemy, CvPlot* pStartPlot, CvPlot* pEndPlot) const;
	CvPlot* GetXPlotsFromEnd(PlayerTypes ePlayer, PlayerTypes eEnemy, CvPlot* pStartPlot, CvPlot* pEndPlot, int iPlotsFromEnd, bool bLeaveEnemyTerritory) const;
};

class CvIgnoreUnitsPathFinder: public CvAStar
{
public:
#if defined(AUI_ASTAR_TURN_LIMITER) && defined(AUI_ASTAR_MINOR_OPTIMIZATION)
	bool DoesPathExist(const CvUnit* pUnit, const CvPlot* pStartPlot, const CvPlot* pEndPlot, const int iMaxTurns = MAX_INT);
#elif defined(AUI_ASTAR_TURN_LIMITER)
	bool DoesPathExist(CvUnit& unit, CvPlot* pStartPlot, CvPlot* pEndPlot, const int iMaxTurns = MAX_INT);
#elif defined(AUI_ASTAR_MINOR_OPTIMIZATION)
	bool DoesPathExist(const CvUnit* pUnit, const CvPlot* pStartPlot, const CvPlot* pEndPlot);
#else
	bool DoesPathExist(CvUnit& unit, CvPlot* pStartPlot, CvPlot* pEndPlot);
#endif
	CvPlot* GetLastOwnedPlot(CvPlot* pStartPlot, CvPlot* pEndPlot, PlayerTypes iOwner) const;
#ifdef AUI_CONSTIFY
	int GetPathLength() const;
#else
	int GetPathLength();
#endif
	CvPlot* GetPathFirstPlot() const;
	CvPlot* GetPathEndTurnPlot() const;
	CvPlot* GetLastPlot();
	CvPlot* GetPreviousPlot();

private:
	CvAStarNode* m_pCurNode;
};

#endif	//CVASTAR_H
