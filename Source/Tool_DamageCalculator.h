#ifndef _TOOL_DAMAGECALCULATOR_H
#define _TOOL_DAMAGECALCULATOR_H

struct CalculatorBuff;
struct CalculatorEquipStat;
struct CalculatorPlayerStat;
class ToolDamageCalculator;
class ToolDamageCalculatorPlayer;
class ToolDamageCalculatorEnemy;

#include "Gui_Manipulation.h"
#include "Configuration.h"

struct CalculatorBuff {
	uint8_t element_absorb = 0;
	uint8_t element_immune = 0;
	uint8_t element_half = 0;
	uint8_t element_weak = 0;
	uint8_t element_boost = 0;
	uint32_t status = 0;
	bool back_row = false;
	bool back_attack = false;
	bool ipsen_curse = false;
	int reflect_count = 0;

	void ApplyToDialog(DamageCalculatorBuff* dialog);
	void UpdateFromDialog(DamageCalculatorBuff* dialog);
};

struct CalculatorEquipStat {
	int attack = 0;
	int speed = 0;
	int strength = 0;
	int magic = 0;
	int spirit = 0;
	int defence = 0;
	int evade = 0;
	int magic_defence = 0;
	int magic_evade = 0;
	uint8_t element_absorb = 0;
	uint8_t element_immune = 0;
	uint8_t element_half = 0;
	uint8_t element_weak = 0;
	uint8_t element_boost = 0;
};

struct CalculatorPlayerStat {
	int level = 1;
	int max_hp = 0;
	int attack = 0;
	int speed = 0;
	int strength = 0;
	int magic = 0;
	int spirit = 0;
	int defence = 0;
	int evade = 0;
	int magic_defence = 0;
	int magic_evade = 0;
	int equip_speed[MAX_LEVEL]{ 0 };
	int equip_strength[MAX_LEVEL]{ 0 };
	int equip_magic[MAX_LEVEL]{ 0 };
	int equip_spirit[MAX_LEVEL]{ 0 };
	int jewel_count = 0;
	int weapon_id = 0;
	int head_id = 0;
	int wrist_id = 0;
	int armor_id = 0;
	int accessory_id = 0;
	uint64_t supporting_ability_on = 0;
	CalculatorBuff buff;
};

class ToolDamageCalculatorPlayer : public DamageCalculatorPlayerPanel {
public:
	ToolDamageCalculator* parent;
	int base_character_selection = 0;
	int base_stat_max_hp = 0;
	int base_stat_speed = 0;
	int base_stat_strength = 0;
	int base_stat_magic = 0;
	int base_stat_spirit = 0;
	CalculatorPlayerStat* stat;
	CalculatorEquipStat equip;

	int computed_base_stat_speed = 0;
	int computed_base_stat_strength = 0;
	int computed_base_stat_magic = 0;
	int computed_base_stat_spirit = 0;
	int computed_level_stat_speed = 0;
	int computed_level_stat_strength = 0;
	int computed_level_stat_magic = 0;
	int computed_level_stat_spirit = 0;
	int computed_bonus_stat_speed = 0;
	int computed_bonus_stat_strength = 0;
	int computed_bonus_stat_magic = 0;
	int computed_bonus_stat_spirit = 0;

	ToolDamageCalculatorPlayer(wxWindow* p);
	void Init(CDDataStruct* data, bool initializeprofile);
	void EnableSpellOptions(bool enable);
	void ComputeEquipCumulatedStat();
	void ComputePlayerBaseStat();
	void ApplyPlayerStatToInterface();
	void UpdatePlayerStatFromInterface();
	void ApplyStatToDialog(DamageCalculatorStats* dialog);
	void UpdateStatFromDialog(DamageCalculatorStats* dialog);
	void ApplySupportToDialog(DamageCalculatorSupporting* dialog);
	void UpdateSupportFromDialog(DamageCalculatorSupporting* dialog);
	void SelectBaseCharacter(int sel);
	void SelectBaseSpell(int sel);
	void Swap(ToolDamageCalculatorPlayer* other);

private:
	void OnCharacterChange(wxCommandEvent& event);
	void OnSpellChange(wxCommandEvent& event);
	void OnLevelChange(wxSpinEvent& event);
	void OnEquipChange(wxCommandEvent& event);
	void OnStatButton(wxCommandEvent& event);
	void OnSupportingButton(wxCommandEvent& event);
	void OnBuffButton(wxCommandEvent& event);
	void OnUpdate(wxCommandEvent& event);
	void OnUpdateSpin(wxSpinEvent& event);
};

class ToolDamageCalculatorEnemy : public DamageCalculatorEnemyPanel {
public:
	ToolDamageCalculator* parent;
	CalculatorBuff buff;

	ToolDamageCalculatorEnemy(wxWindow* p);
	void Init(CDDataStruct* data);
	void EnableSpellOptions(bool enable);
	void SelectBaseEnemy(int sel);
	void SelectBaseSpell(int sel);
	void Swap(ToolDamageCalculatorEnemy* other);

private:
	void OnEnemyChange(wxCommandEvent& event);
	void OnSpellChange(wxCommandEvent& event);
	void OnBuffButton(wxCommandEvent& event);
	void OnUpdate(wxCommandEvent& event);
	void OnUpdateSpin(wxSpinEvent& event);
};

class ToolDamageCalculator : public DamageCalculatorWindow {
public:
	CDDataStruct* cddata;
	ToolDamageCalculatorPlayer* player_panel[2];
	ToolDamageCalculatorEnemy* enemy_panel[2];
	vector<int> enemy_list_battle;
	vector<int> enemy_list_stat;
	CalculatorPlayerStat player_profile[PLAYABLE_CHAR_AMOUNT];

	ToolDamageCalculator(wxWindow* p);
	int ShowModal(CDDataStruct* data);
	void UpdateInformation();
	void UpdateIpsenCurseFlag(EnemyDataStruct* enemy = NULL);

private:
	bool initialized = false;

	void OnCharacterTypeChange(wxBookCtrlEvent& event);
	void OnButtonSwap(wxCommandEvent& event);
	void OnSaveProfile(wxCommandEvent& event);
	void OnLoadProfile(wxCommandEvent& event);
};

#endif
