/*

*/
#pragma once

#ifndef LEK_CIV5_LEGACYCLASSES_H
#define LEK_CIV5_LEGACYCLASSES_H
#if defined(LEKMOD_LEGACY)
// LegacyAI
class CvLegacyAI;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvLegacyEntry
//!  \brief		A single entry in Legacies
//
//!  Key Attributes:
//!  - Populated from XML\GameInfo\CIV5LegacyInfos.xml (if we ever swap to using multiple files again.)
//!  - Array of these contained in CvLegacyXMLEntries class
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvLegacyEntry: public CvBaseInfo
{
public:
    CvLegacyEntry(void);
    ~CvLegacyEntry(void);

	virtual bool CacheResults(Database::Results& kResults, CvDatabaseUtility& kUtility);

    // Accessor Functions
    int GetCivilization() const;
    int GetEraOffered() const;

    // Arrays
	int GetCityYieldChange(int i) const;
private:
    int m_iCivilization;
    int m_iEraOffered;

    //Arrays
	int* m_piCityYieldChange;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvLegacyXMLEntries
//!  \brief		Game-wide information about the Legacies
//! Key Attributes:
//! - Plan is it will be contained in CvGameRules object within CvGame class
//! - Populated from XML\GameInfo\CIV5LegacyInfos.xml if we ever use separate files again
//! - Contains an array of CvLegacyEntry from the above XML file
//! - One instance for the entire game
//! - Accessed heavily by CvPlayerLegacies class (which stores the legacy state for 1 player)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvLegacyXMLEntries
{
public:
    CvLegacyXMLEntries(void);
    ~CvLegacyXMLEntries(void);

	std::vector<CvLegacyEntry*>& GetLegacyEntries();
    // Legacy Functions
	int GetNumLegacies();
	CvLegacyEntry* GetLegacyEntry(int iIndex);
    void DeleteLegacyArray();
private:
	std::vector<CvLegacyEntry*> m_paLegacyEntries;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPlayerLegacies
//!  \brief		Information about the legacies of a single player
//
//!  Key Attributes:
//!  - Plan is it will be contained in CvPlayerState object within CvPlayer class
//!  - One instance for each civ (player or AI)
//!  - Accessed by any class that needs to check legacy state
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPlayerLegacies
{
public:
    CvPlayerLegacies(void);
    ~CvPlayerLegacies(void);
    // Initialization, Cleanup and Save/Load
    void Init(CvLegacyXMLEntries* pLegacies, CvPlayer* pPlayer);
    void Uninit();
    void Reset();
    void Read(FDataStream& kStream);
    void Write(FDataStream& kStream) const;

    CvPlayer* GetPlayer() const;

	bool HasLegacy(LegacyTypes eIndex) const;
    void SetLegacy(LegacyTypes eIndex, bool bNewValue);
    CvLegacyXMLEntries* GetLegacies() const;
	std::vector<LegacyTypes> GetPotentialCivLegacies() const;
	bool CanChooseLegacy(LegacyTypes eIndex) const;
    // Activation Time!
    bool IsTimeToChooseLegacy() const;
    // Do AI for legacy choices. will likely be random until the feature settles.
    void DoLegacyAI();
    void DoChooseLegacy();

private:
    bool* m_pabHasLegacy;
	CvLegacyXMLEntries* m_pLegacies;
    CvLegacyAI* m_pLegacyAI;
	CvPlayer* m_pPlayer;
    std::vector<bool> m_vCivHasLegacy; 
	std::vector<LegacyTypes> m_vPotentiallyActiveCivLegacies; 
};
#endif
#endif //LEK_CIV5_LEGACYCLASSES_H
