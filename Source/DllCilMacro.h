#ifndef _DLLCILMACRO_H
#define _DLLCILMACRO_H

struct CILMacroBaseStruct;
struct CILMacroID;
struct CILMacro;

#include "DllEditor.h"

struct CILMacroBaseStruct {
	CILMacro* parent;
	
	virtual unsigned int GetMethodCount() { return 0; }
	virtual string GetMethodTypeName(unsigned int index) { return ""; }
	virtual string GetMethodName(unsigned int index) { return ""; }
	// Return a modifamount-long pointer, to be deleted[] ; modifamount should be set on GetMethodCount() (maybe it won't be anymore in the future)
	virtual DllMetaDataModification* ComputeModifications(unsigned int* modifamount) { return NULL; } // Require a DllMetaData access
	
	virtual void SetParameters(uint32_t* intparam = NULL) { }
	virtual void SetParameters(double* floatparam = NULL) { }
	virtual void SetParameters(wstring* strparam = NULL) { }
	virtual uint16_t GetSaveFlag() { return 0; }
	virtual void ReadHWS(fstream& ffhws, uint16_t flag) { }
	virtual void WriteHWS(fstream& ffhws) { }
};

struct CILMacroID {
	uint32_t id;
	wstring name;
	wstring description;
};

static CILMacroID CILMacroIDList[] = {
	{ 50, L"Custom Backgrounds",	L"Make the engine accept custom backgrounds imported with the background editor.\n"
									L"This macro is needed in both of the following situations:\n"
									L" - If backgrounds with custom resolution are used (different than 32),\n"
									L" - If backgrounds containing titles are used (examples: Alexandria's Main Street, Ice Cavern's Entrance, ...)\n\n"
									L"Parameters: resolution of the imported backgrounds." },
	{ 100, L"Disable Cheats",	L"Disable the Steam cheats (boosters) : Battle Assistance, Attack 9999, No Encounter, Master Skill, Level Max and Gil Max.\n"
								L"The High Speed mode can still be activated but timed minigames countdown are speeded up as well.\n"
								L"Game time is still unaffected by the High Speed mode." },
	{ 101, L"Unlock Ability Learning",	L"Allow the temporary characters to use abilities like a normal character.\n"
										L" - Abilites are learned with AP and are no more available from the start,\n"
										L" - Supporting abilities can be activated.\n"
										L"Important: To enable it for a character, his 1st ability in the Stats panel must require a non-zero AP amount (both ability sets for Beatrix).\n"
										L"For Cinna, Marcus and Blank, as they share their character slot with Quina, Eiko and Amarant respectively, learning abilities with them may result in learning abilities with their relative." },
	{ 102, L"Increase Damage Limit",	L"Allow attacks to deal more than 9999 damage, up to 32767 damage.\n"
										L"Warning : Most bosses have the following system in their AI script for ending the battle:\n"
										L" - Their Max HP is set 10000 higher than normally,\n"
										L" - Every frame (in the Loop function) they check if their current HP is under 10000,\n"
										L" - If that is the case, they perform some final move (flee, dialog, death animation...) and end the battle.\n"
										L"Increasing the damage limit mess up with this system as those bosses can then reach 0 HP. It softlocks the game most of the time." },
};

struct CILMacro {
public:
	CILMacroID* type;
	CILMacroBaseStruct* info;
	DllMetaData* data;
	
	CILMacro();
	void Init(uint32_t macroid, DllMetaData* d);
	CILMacro(uint32_t macroid, DllMetaData* d);
	~CILMacro();
	
	int FindMethod(unsigned int tid, unsigned int mid);
	int FindMethodById(unsigned int absmethid);
	
private:
	bool isdeletedforcopy;
	friend CILDataSet;
};

#endif
