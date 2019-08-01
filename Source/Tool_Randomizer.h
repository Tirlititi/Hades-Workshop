#ifndef _TOOL_RANDOMIZER_H
#define _TOOL_RANDOMIZER_H

class ToolRandomizer;
class ToolRandomizerList;

#include "Gui_Manipulation.h"
#include "Configuration.h"

class ToolRandomizerList : public RandomizerListWindow {
};

class ToolRandomizer : public RandomizerWindow {
public:
	CDDataStruct* cddata;
	int used_seed;
	vector<uint8_t> safe_abil_scramble;
	vector<uint8_t> safe_abil_spell;
	vector<uint8_t> safe_abil_support;
	vector<uint8_t> safe_abil_weap;
	vector<uint8_t> safe_abil_armor;
	
	ToolRandomizer(wxWindow* p);
	~ToolRandomizer();
	int ShowModal(CDDataStruct* data);

private:
	void UpdateMenuAvailability();
	void ApplyRandomizerCharacter();
	void ApplyRandomizerWeapon();
	void ApplyRandomizerArmor();
	void ApplyRandomizerBattle();
	void ApplyRandomizerCard();
	void ApplyRandomizer();

	void OnChoice(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
