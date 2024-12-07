/*	-------------------------------------------------------------------------------------------------------
	© 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once
#ifndef CVREPLAYMESSAGE_H
#define CVREPLAYMESSAGE_H

class CvReplayMessage
{
public:
	static unsigned int Version();	//used for serialization.

	CvReplayMessage();
	CvReplayMessage(int iTurn, ReplayMessageTypes eType = NO_REPLAY_MESSAGE, PlayerTypes ePlayer = NO_PLAYER);
#ifdef REPLAY_MESSAGE_EXTENDED
	CvReplayMessage(int iTurn, int iData1, int iData2, ReplayMessageTypes eType = NO_REPLAY_MESSAGE, PlayerTypes ePlayer = NO_PLAYER);
#endif
	virtual ~CvReplayMessage();

	// Accessors
	void setTurn(int iTurn);
	int getTurn() const;
	void setType(ReplayMessageTypes eType);
	ReplayMessageTypes getType() const;

	void setPlayer(PlayerTypes ePlayer);
	PlayerTypes getPlayer() const;
	void setText(const CvString& pszText);
	const CvString& getText() const;

	void addPlot(int iPlotX, int iPlotY);
	bool getPlot(unsigned int idx, int& iPlotX, int& iPlotY) const;
	unsigned int getNumPlots() const;
	void clearPlots();
#ifdef REPLAY_MESSAGE_EXTENDED
	void setTimestamp(float fTime);
	int getTimestamp() const;
	void setExtraData1(int iData1);
	int getExtraData1() const;
	void setExtraData2(int iData1);
	int getExtraData2() const;
#endif

	void read(FDataStream& kStream, unsigned int uiVersion);
	void write(FDataStream& kStream) const;

private:
	int m_iTurn;
	ReplayMessageTypes m_eType;

	typedef std::pair<short, short> PlotPosition;
	typedef std::vector<PlotPosition> PlotPositionList;
#ifdef REPLAY_MESSAGE_EXTENDED
	int m_iTimeMilliseconds;
	int m_iExtraData1;
	int m_iExtraData2;
#endif

	PlotPositionList m_Plots;
	PlayerTypes m_ePlayer;
	CvString m_strText;
};

#endif
#ifdef REPLAY_EVENTS
class CvReplayEvent
{
public:
	static unsigned int Version();	//used for serialization.

	CvReplayEvent();
	CvReplayEvent(int eType, std::vector<int> vNumArgs, CvString strArg = "");
	CvReplayEvent(int eType, PlayerTypes ePlayer, std::vector<int> vNumArgs, CvString strArg = "");

	virtual ~CvReplayEvent();

	void read(FDataStream& kStream, unsigned int uiVersion);
	void write(FDataStream& kStream) const;
	int m_eEventType;
	PlayerTypes m_ePlayer;
	int m_iTurn;
	int m_iTimestamp;
	std::vector<int> m_vNumericArgs;
	CvString m_strStringData;
private:
};
#endif