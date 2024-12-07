/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#include "CvGameCoreDLLPCH.h"
#include ".\cvreplaymessage.h"
#include "CvEnumSerialization.h"

// include this after all other headers!
#include "LintFree.h"

#ifdef REPLAY_MESSAGE_EXTENDED
//------------------------------------------------------------------------------
unsigned int CvReplayMessage::Version()
{
# ifdef SAVE_BACKWARDS_COMPATIBILITY
	return BUMP_SAVE_VERSION_REPLAYMESSAGE;
# else
	return 2;
# endif
}
//------------------------------------------------------------------------------
CvReplayMessage::CvReplayMessage()
	: m_iTurn(-1)
	, m_iTimeMilliseconds(0)
	, m_iExtraData1(0)
	, m_iExtraData2(0)
	, m_eType(NO_REPLAY_MESSAGE)
	, m_ePlayer(NO_PLAYER)
{
}
//------------------------------------------------------------------------------
CvReplayMessage::CvReplayMessage(int iTurn, ReplayMessageTypes eType, PlayerTypes ePlayer) :
	m_iTurn(iTurn),
	m_iTimeMilliseconds(static_cast<int>(GC.getGame().getTimeElapsed() * 1000)),
	m_ePlayer(ePlayer),
	m_eType(eType)
{
}
CvReplayMessage::CvReplayMessage(int iTurn, int iData1, int iData2, ReplayMessageTypes eType, PlayerTypes ePlayer) :
	m_iTurn(iTurn),
	m_iTimeMilliseconds(static_cast<int>(GC.getGame().getTimeElapsed() * 1000)),
	m_iExtraData1(iData1),
	m_iExtraData2(iData2),
	m_ePlayer(ePlayer),
	m_eType(eType)
{
}
#else
//------------------------------------------------------------------------------
unsigned int CvReplayMessage::Version()
{
	return 2;
}
//------------------------------------------------------------------------------
CvReplayMessage::CvReplayMessage()
	: m_iTurn(-1)
	, m_eType(NO_REPLAY_MESSAGE)
	, m_ePlayer(NO_PLAYER)
{
}
//------------------------------------------------------------------------------
CvReplayMessage::CvReplayMessage(int iTurn, ReplayMessageTypes eType, PlayerTypes ePlayer) :
	m_iTurn(iTurn),
	m_ePlayer(ePlayer),
	m_eType(eType)
{
}
#endif
//------------------------------------------------------------------------------
CvReplayMessage::~CvReplayMessage()
{
}
//------------------------------------------------------------------------------
void CvReplayMessage::setTurn(int iTurn)
{
	m_iTurn = iTurn;
}
//------------------------------------------------------------------------------
int CvReplayMessage::getTurn() const
{
	return m_iTurn;
}
//------------------------------------------------------------------------------
void CvReplayMessage::setType(ReplayMessageTypes eType)
{
	m_eType = eType;
}
//------------------------------------------------------------------------------
ReplayMessageTypes CvReplayMessage::getType() const
{
	return m_eType;
}
//------------------------------------------------------------------------------
void CvReplayMessage::setPlayer(PlayerTypes ePlayer)
{
	m_ePlayer = ePlayer;
}
//------------------------------------------------------------------------------
PlayerTypes CvReplayMessage::getPlayer() const
{
	return m_ePlayer;
}
//------------------------------------------------------------------------------
void CvReplayMessage::setText(const CvString& strText)
{
	m_strText = strText;
}
//------------------------------------------------------------------------------
const CvString& CvReplayMessage::getText() const
{
	return m_strText;
}
//------------------------------------------------------------------------------
void CvReplayMessage::addPlot(int iPlotX, int iPlotY)
{
	short sPlotX = (short)iPlotX;
	short sPlotY = (short)iPlotY;

	for(PlotPositionList::iterator it = m_Plots.begin(); it != m_Plots.end(); ++it)
	{
		const PlotPosition& position = (*it);
		if(position.first == sPlotX && position.second == sPlotY)
			return;
	}

	m_Plots.push_back(PlotPosition(sPlotX, sPlotY));
}
//------------------------------------------------------------------------------
bool CvReplayMessage::getPlot(unsigned int idx, int& iPlotX, int& iPlotY) const
{
	if(idx < m_Plots.size())
	{
		const PlotPosition& position = m_Plots[idx];
		iPlotX = (int)position.first;
		iPlotY = (int)position.second;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------
unsigned int CvReplayMessage::getNumPlots() const
{
	return m_Plots.size();
}
//------------------------------------------------------------------------------
void CvReplayMessage::clearPlots()
{
	m_Plots.clear();
}
#ifdef REPLAY_MESSAGE_EXTENDED
void CvReplayMessage::setTimestamp(float fTime)
{
	m_iTimeMilliseconds = static_cast<int>(fTime);
}
int CvReplayMessage::getTimestamp() const
{
	return m_iTimeMilliseconds;
}
void CvReplayMessage::setExtraData1(int iData1)
{
	m_iExtraData1 = iData1;
}
int CvReplayMessage::getExtraData1() const
{
	return m_iExtraData1;
}
void CvReplayMessage::setExtraData2(int iData2)
{
	m_iExtraData2 = iData2;
}
int CvReplayMessage::getExtraData2() const
{
	return m_iExtraData2;
}
#endif
//------------------------------------------------------------------------------
void CvReplayMessage::read(FDataStream& kStream, unsigned int uiVersion)
{
	UNREFERENCED_PARAMETER(uiVersion);

	kStream >> m_iTurn;
	kStream >> m_eType;

	int nPlots = -1;
	kStream >> nPlots;
	if(nPlots > 0)
	{
		m_Plots.reserve(nPlots);
		for(int i = 0; i < nPlots; ++i)
		{
			short sPlotX, sPlotY;
			kStream >> sPlotX;
			kStream >> sPlotY;
			m_Plots.push_back(PlotPosition(sPlotX, sPlotY));
		}
	}
#ifdef REPLAY_MESSAGE_EXTENDED
	kStream >> m_iTimeMilliseconds;
	kStream >> m_iExtraData1;
	kStream >> m_iExtraData2;
#endif

	kStream >> m_ePlayer;
	kStream >> m_strText;
}
//------------------------------------------------------------------------------
void CvReplayMessage::write(FDataStream& kStream) const
{
	kStream << m_iTurn;
	kStream << m_eType;

	kStream << (int)m_Plots.size();
	for(PlotPositionList::const_iterator it = m_Plots.begin(); it != m_Plots.end(); ++it)
	{
		kStream << (*it).first;
		kStream << (*it).second;
	}
#ifdef REPLAY_MESSAGE_EXTENDED
	kStream << m_iTimeMilliseconds;
	kStream << m_iExtraData1;
	kStream << m_iExtraData2;
#endif

	kStream << m_ePlayer;
	kStream << m_strText;
}
//------------------------------------------------------------------------------
#ifdef REPLAY_EVENTS
CvReplayEvent::CvReplayEvent()
	: m_iTurn(-1)
	, m_iTimestamp(0)
	, m_eEventType(0)
	, m_strStringData("")
	, m_ePlayer(NO_PLAYER)
{
	std::vector<int> vEmpty;
	m_vNumericArgs = vEmpty;
}
CvReplayEvent::CvReplayEvent(int eType, std::vector<int> vNumArgs, CvString strArg)
	: m_eEventType(eType)
	, m_vNumericArgs(vNumArgs)
	, m_strStringData(strArg)
	, m_ePlayer(NO_PLAYER)
{
	m_iTurn = GC.getGame().getGameTurn();
	m_iTimestamp = static_cast<int>(GC.getGame().getTimeElapsed() * 1000);
}
CvReplayEvent::CvReplayEvent(int eType, PlayerTypes ePlayer, std::vector<int> vNumArgs, CvString strArg)
	: m_eEventType(eType)
	, m_vNumericArgs(vNumArgs)
	, m_strStringData(strArg)
	, m_ePlayer(ePlayer)
{
	m_iTurn = GC.getGame().getGameTurn();
	m_iTimestamp = static_cast<int>(GC.getGame().getTimeElapsed() * 1000);
}
CvReplayEvent::~CvReplayEvent()
{
}
//------------------------------------------------------------------------------
unsigned int CvReplayEvent::Version()
{
# ifdef SAVE_BACKWARDS_COMPATIBILITY
	return BUMP_SAVE_VERSION_REPLAYEVENT;
# else
	return 1;
# endif
}
//------------------------------------------------------------------------------
void CvReplayEvent::read(FDataStream& kStream, unsigned int uiVersion)
{
	UNREFERENCED_PARAMETER(uiVersion);

	kStream >> (int)m_eEventType;
	kStream >> m_ePlayer;
	kStream >> m_iTurn;
	kStream >> m_iTimestamp;
	int iSize = -1;
	kStream >> iSize;
	if (iSize > 0)
	{
		for (int i = 0; i < iSize; ++i)
		{
			int iArg;
			kStream >> iArg;
			m_vNumericArgs.push_back(iArg);
		}
	}
	kStream >> m_strStringData;
}
//------------------------------------------------------------------------------
void CvReplayEvent::write(FDataStream& kStream) const
{
	kStream << (int)m_eEventType;
	kStream << m_ePlayer;
	kStream << m_iTurn;
	kStream << m_iTimestamp;
	kStream << (int)m_vNumericArgs.size();
	for (std::vector<int>::const_iterator it = m_vNumericArgs.begin(); it != m_vNumericArgs.end(); ++it)
	{
		kStream << *it;
	}
	kStream << m_strStringData;
}
#endif
