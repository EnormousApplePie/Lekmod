/*
*/
#pragma once

#ifndef LEK_CIV5_LEGACYAI_H
#define LEK_CIV5_LEGACYAI_H
#if defined(LEKMOD_LEGACY)
class CvLegacyAI
{
public:
	CvLegacyAI(CvPlayerLegacies* currentLegacies);
	~CvLegacyAI(void);
	// Initialization
	void Reset();
	// Serialization routines
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

	int ChooseLegacy(CvPlayer* pPlayer);
private:
	// Internal methods
	CvPlayerLegacies* m_pCurrentLegacies;
};
#endif
#endif // LEK_CIV5_LEGACYAI_H