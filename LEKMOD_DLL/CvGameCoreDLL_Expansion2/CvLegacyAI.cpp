#include "CvGameCoreDLLPCH.h"
#include "CvGameCoreDLLUtil.h"
#include "CvLegacyAI.h"
#include "CvGrandStrategyAI.h"
#include "CvInfosSerializationHelper.h"
// Include this after all other headers.
#include "LintFree.h"

// Constructor
CvLegacyAI::CvLegacyAI(CvPlayerLegacies* currentLegacies):
	m_pCurrentLegacies(currentLegacies)
{
}
// Destructor
CvLegacyAI::~CvLegacyAI(void)
{
}
// Reset
void CvLegacyAI::Reset()
{
}
// Read
void CvLegacyAI::Read(FDataStream& kStream)
{
	// Read the data from the stream
	//kStream >> m_pCurrentLegacies;
}
//Write
void CvLegacyAI::Write(FDataStream& kStream)
{
	//kStream << m_pCurrentLegacies;
}

// Choose an available legacy
int CvLegacyAI::ChooseLegacy(CvPlayer* pPlayer)
{
	// Not ready for this yet.
	return NULL;
}