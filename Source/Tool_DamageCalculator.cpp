#include "Tool_DamageCalculator.h"

#include <algorithm>
#include <set>
#include "Gui_Preferences.h"
#include "Database_Spell.h"
#include "Hades_Strings.h"
#include "main.h"

// Strings and macros

#define STRING_PANEL_CHARACTER		L"Player Character"
#define STRING_PANEL_ENEMY			L"Enemy"
#define STRING_LOAD_STAT_HINT		L"(Load the panel \"Party -> Stats\" of the main frame to automatically compute player stats)\n"
#define STRING_CHARACTER			"\t== %s ==\n"
#define STRING_ATTACKER				"\t== %s takes action ==\n"
#define STRING_DEFENDER				"\t== %s is targeted ==\n"
#define STRING_SEPARATOR			L"_______________________________________\n"
#define STRING_LEVEL				"Level: %d\n"
#define STRING_MAX_HP				"Max HP: %d\n"
#define STRING_SPEED				"Speed: %d\n"
#define STRING_STRENGTH				"Strength: %d\n"
#define STRING_MAGIC				"Magic: %d\n"
#define STRING_SPIRIT				"Spirit: %d\n"
#define STRING_SPEED_EX				"Speed: %d [%d (base) + %d (level) + %d (bonus) + %d (equipments) %s (custom)]\n"
#define STRING_STRENGTH_EX			"Strength: %d [%d (base) + %d (level) + %d (bonus) + %d (equipments) %s (custom)]\n"
#define STRING_MAGIC_EX				"Magic: %d [%d (base) + %d (level) + %d (bonus) + %d (equipments) %s (custom)]\n"
#define STRING_SPIRIT_EX			"Spirit: %d [%d (base) + %d (level) + %d (bonus) + %d (equipments) %s (custom)]\n"
#define STRING_WEAPON_ATTACK		"Attack power of the weapon: %d\n"
#define STRING_DEFENCE				"Defence: %d\n"
#define STRING_EVADE				"Evade: %d\n"
#define STRING_MAGIC_DEFENCE		"Magic Defence: %d\n"
#define STRING_MAGIC_EVADE			"Magic Evade: %d\n"

#define STRING_ATTACK				"The attack %s "
#define STRING_SPELL				"The spell %s "
#define STRING_PHYSICAL_ATTACK		"%s's physical attack "
#define STRING_DAMAGE_RANGE			"deals damage between %d and %d"
#define STRING_DAMAGE_SINGLE		"deals %d damage"
#define STRING_DAMAGE_SPECIAL		L"deals damage"
#define STRING_DAMAGE_MP			L"deals MP damage"
#define STRING_HEAL_MP				L"heals MP"
#define STRING_HEAL_RANGE			"heals between %d and %d HP"
#define STRING_HEAL_SINGLE			"heals %d HP"
#define STRING_HEAL_SPECIAL			L"heals HP"
#define STRING_MPHEAL_RANGE			"heals between %d and %d MP"
#define STRING_CURE_STATUS			"cures %s"
#define STRING_INFLICT_STATUS		"inflicts %s (if not guarded)"
#define STRING_UNKNOWN_EFFECT		"has unknown effect"
#define STRING_FAIL_ZERO_DIVISION	L"fails because of a division by zero (don't do that!)\n"
#define STRING_FAIL_ELEMENT			"fails because %s is immune to the element %s\n"
#define STRING_FAIL_STATUS			"fails because %s is under %s\n"
#define STRING_FAIL_DEAD_ZOMBIE		"fails because %s is both KO and Zombie\n"
#define STRING_FAIL_SELF_ATTACK		L"fails because of targeting oneself\n"
#define STRING_FAIL_ENEMY			"fails because %s is not a player character\n"
#define STRING_FAIL_ENEMY_BAD		"fails because %s is not a player character (undefined behaviour)\n"
#define STRING_FAIL_PLAYER			"fails because %s is a player character\n"
#define STRING_FAIL_PLAYER_BAD		"fails because %s is a player character (undefined behaviour)\n"
#define STRING_FAIL_NO_EAT_HUMAN	"fails because %s is human-type\n"
#define STRING_FAIL_FEMALE			"fails because %s is a female\n"
#define STRING_FAIL_TERRAN			"fails because %s is a terran\n"
#define STRING_FAIL_LEVEL_MOD		"fails because %s's level is not a multiple of %d\n"
#define STRING_FAIL_EARTH_FLY		"fails because the spell is earth-elemental and %s is flying/floating\n"
#define STRING_FAIL_SOUL_BLADE		"fails because %s's weapon is not weapon with ID between %d and %d\n"
#define STRING_FAIL_WHATS_THAT		L"fails because the battle is flagged with 'Scripted Start'\n"
#define STRING_FAIL_FLEE			L"fails because the battle is flagged with 'Can't Flee'\n"
#define STRING_SET_HP_VALUE			"brings %s's HP to %d"
#define STRING_FULL_HEAL_HP			"fully heals %s's HP"
#define STRING_ENFORCE_KILL			"kills %s (bypassing any status immunity)"
#define STRING_SACRIFICE			"fully heals %s's HP and MP then kills and removes all the MP of %s (bypassing any status immunity)"
#define STRING_CHANGE_STAT			"changes %s's %s to %d"
#define STRING_CHANGE_STAT_RND		"changes %s's %s to a random value between %d and %d"
#define STRING_LIMIT_GLOVE			"deals %d damage (always miss unless %s's HP is exactly 1)"
#define STRING_JUMP					"launches an attack after %d ticks (%.2f/%.2f/%.2f seconds in slow/normal/fast modes respectively)"
#define STRING_SIX_DRAGONS			"scrambles %s's HP and MP:\nFully heals HP and MP (10%%)\nFully heals HP (20%%)\nFully heals MP (20%%)\nReduces HP to 1 (15%%)\nReduces MP to 1 (15%%)\nReduces both HP and MP to 1 (20%%)"
#define STRING_CURSE				"weakens %s against %s"
#define STRING_CURSE_RND			"weakens %s against a random element"
#define STRING_ANGEL_SNACK			"uses a %s from the player's inventory on %s (if owned)"
#define STRING_LUCKY_SEVEN			"deals either 7, 77, 777 or 7777 damage if %s's HP end by 7 and deals 1 damage if not"
#define STRING_WHATS_THAT			"turns %s to the same direction as %s's normal facing direction"
#define STRING_WHATS_THAT_ROW		" and reset %s's row to its default"
#define STRING_FLEE					"triggers a player escape with a probability of %d%%, assuming that the enemy team has an average level of %d and the player's team has an average level of %d"
#define STRING_FLEE_SKILL			L"triggers a player escape"
#define STRING_SCAN					"displays %s's stats"
#define STRING_DETECT				"displays %s's stealable items"
#define STRING_CHANGE_ROW			"change %s's row"
#define STRING_STEAL				"steals an enemy item"
#define STRING_INVENTORY_STEAL		"removes a %s from the player's inventory"
#define STRING_INVENTORY_MUG		"deals damage and removes a %s from the player's inventory"
#define STRING_CHARGE				"makes player characters under Low HP attack random enemies"
#define STRING_ORE					"gives an ore to the player"
#define STRING_EAT					"eats if %s's HP are %d or below"
#define STRING_SELF_DESTRUCT		"deals damage equal to %s's current HP and sacrifices him/her/it"
#define STRING_HP_SWITCHING			"switches %s's and %s's HP"
#define STRING_ADD_ITEM				"gives a %s to the player"
#define STRING_GRAND_CROSS			L"inflicts random statuses amongst Petrify, Silence, Blind, Trouble, Zombie, Death, Confuse, Berserk, Stop, Poison, Sleep, Heat, Freeze, Doom, Mini and bringing the target's HP to 1-9, with 1/8 chances for each one"
#define STRING_BANISH				"removes %s from the battleground"
#define STRING_RATE_PERCENT			" with %.1f%% chances to "
#define STRING_RATE_PERCENT_MORE	"%.1f%% chances to "
#define STRING_RATE_HIT				"hit"
#define STRING_RATE_INFLICT_STATUS	"inflict %s (if not guarded)"
#define STRING_RATE_ADD_STATUS		"inflict %s (with Add Status enabled and only if not guarded)"
#define STRING_RATE_CURE_STATUS		"cure %s"
#define STRING_RATE_CRITICAL		"land a critical strike"

#define STRING_FORMULA_DAMAGE		"Damage Formula: %s\n"
#define STRING_FORMULA_RATE			"Hit Rate Formula: %s\n"
#define STRING_FORMULA_STATUS_RATE	"Status Rate Formula: %s\n"
#define STRING_FORMULA_HEAL			"Heal Formula: %s\n"
#define STRING_FORMULA_MPDAMAGE		"MP Damage Formula: %s\n"
#define STRING_FORMULA_MPHEAL		"MP Heal Formula: %s\n"
#define STRING_FORMULA_CRITICAL		L"Critical Rate Formula: (Spirit / 4 - 1) / 2\n"
#define STRING_FORMULA_JUMP_TIME	L"Jump duration Formula: 40 * (60 - Spirit) ticks\n"
#define STRING_FORMULA_GIL			L"Gil Loss Formula: Level * Power\n"
#define STRING_FORMULA_FLEE			L"Flee Rate Formula: 200 * TeamMeanLevel / EnemiesMeanLevel / 16\n"
#define STRING_FORMULA_STEAL		L"Steal Base Chance (without Bandit):\n"\
									L" If (CasterLevel + Spirit <= TargetLevel): 100 * (1 + CasterLevel + Spirit) / (2 * TargetLevel)\n"\
									L" If (CasterLevel + Spirit >= TargetLevel): 100 * (1 + (1 - TargetLevel) / (2 * (CasterLevel + Spirit)))\n"
#define STRING_FORMULA_BASE8		L"(Attack - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_BASE8_SPIRIT	L"(Attack - Defence) * ((Strength + Spirit) / 2 + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_BASE8_SPEED	L"(Attack - Defence) * ((Strength + Speed) / 2 + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_BASE8_MAGIC	L"(Attack - Defence) * ((Strength + Magic) / 2 + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_BASE8_RANDOM	L"(Attack - Defence) * (Random[0, Strength - 1] + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_BASE8_LEVEL	L"(Attack + Level - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_PHYSIC8		L"(Attack * Power / 10 - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_MAGIC_WEAPON	L"(Attack * Power / 10 - MagicDefence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_SWORD_MAGIC	L"(Attack + Power - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_SPEAR		L"1.5 * (Attack - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_SPEAR_TRANCE	L"1.5 * (Attack - Defence) * (Random[0, Strength - 1] + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_THROW		L"(2 * WeaponAttack - Defence) * (Strength + Random[0, (Level + Strength) / 8])"
#define STRING_FORMULA_PHYSIC4		L"(Power - Defence) * (Strength + Random[0, (Level + Strength) / 4])"
#define STRING_FORMULA_MAGIC8		L"(Power - MagicDefence) * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_OSMOSE		L"(Power - MagicDefence) * (Magic + Random[0, (Level + Magic) / 8]) / 4"
#define STRING_FORMULA_SUMMON_JEWEL	L"(Power + JewelCount - MagicDefence) * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_SUMMON_LEVEL	L"(Power + Level - MagicDefence) * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_SUMMON_ODIN	L"(Power + 100 - OreCount - MagicDefence) * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_HEAL8		L"Power * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_GOBLIN_PUNCH	L"(Power + Level) * (Magic + Random[0, (Level + Magic) / 8])"
#define STRING_FORMULA_LIFE			L"(TargetSpirit + Power) * TargetMaxHP / 100"
#define STRING_FORMULA_SUMMON_GRAV	L"TargetMaxHP * (Power + JewelCount) / 100"
#define STRING_FORMULA_GRAVITY		L"TargetMaxHP * Power / 100"
#define STRING_FORMULA_CHAKRA_MP	L"TargetMaxMP * Power / 100"
#define STRING_FORMULA_CANNON		L"TargetHP * Power / 100"
#define STRING_FORMULA_MAGIC_RANDOM	L"Power * Random[1, Level + Magic - 1]"
#define STRING_FORMULA_PURE			L"Power * 100 + Accuracy"
#define STRING_FORMULA_DRAGON_BR	L"TargetMaxHP - TargetHP"
#define STRING_FORMULA_MINUS_STRIKE	L"CasterMaxHP - CasterHP"
#define STRING_FORMULA_WHITE_WIND	L"CasterMaxHP / 3"
#define STRING_FORMULA_MAGIC_HAMMER	L"Random[0, TargetMP - 1]"
#define STRING_FORMULA_WHITE_DRAW	L"Random[0, TargetLevel * 2 - 1]"
#define STRING_FORMULA_FROG_DROP	L"Level * FrogCount"
#define STRING_FORMULA_THIEVERY		L"Speed * StealCount / 2"
#define STRING_FORMULA_DRAGON_CREST	L"DragonCount * DragonCount"
#define STRING_FORMULA_GRUDGE		L"2 ^ (TonberryCounter - 1)"
#define STRING_FORMULA_FLARE_STAR	L"Power * TargetLevel"
#define STRING_FORMULA_LANCER_MP	L"HPDamage / 16"
#define STRING_FORMULA_SPARE_CHANGE	L"(LostGil * LostGil * Spirit) / (TotalGil * 10)"
#define STRING_FORMULA_RATE_MAGIC	L"Accuracy + Magic / 4 + CasterLevel - TargetLevel"
#define STRING_FORMULA_RATE_ODIN	L"Accuracy + Magic / 4 + CasterLevel - TargetLevel + OreCount / 2"
#define STRING_FORMULA_RATE_METEOR	L"Level / 2 + Spirit + 1"

#define STRING_USE_JEWEL			"Use %d %s(s)\n"
#define STRING_DRAIN				L"It is a drain\n"
#define STRING_ZOMBIE				"The damage/heal effect is reversed because %s is a zombie/undead\n"
#define STRING_MODIF_DMG_RAISE		"%s increases the damage\n"
#define STRING_MODIF_DMG_LOW		"%s decreases the damage\n"
#define STRING_MODIF_HIT_RAISE		"%s increases the hit rate\n"
#define STRING_MODIF_HIT_LOW		"%s decreases the hit rate\n"
#define STRING_MODIF_HEAL_RAISE		"%s increases the heal\n"
#define STRING_MODIF_HEAL_LOW		"%s decreases the heal\n"
#define STRING_MODIF_ABSORB			"%s turns damage into heal\n"
#define STRING_MODIF_SELF_ATTACK	L"No evade is used because of targeting oneself\n"
#define STRING_MODIF_TRIGGER		"%s triggers\n"

#define STRING_DAMAGE_POSSIBLE		"Possible damage: %s\n"
#define STRING_HEAL_POSSIBLE		"Possible healing values: %s\n"
#define STRING_DAMAGE_MEAN			"Average damage: %.1f\n"
#define STRING_DAMAGE_MEAN1			"Average damage: %.1f (considering it hits everytime)\n"
#define STRING_DAMAGE_MEAN2			"Average damage: %.1f (taking hit rate into account)\n"
#define STRING_DAMAGE_MEAN3			"Average damage: %.1f (taking hit rate and critical strikes into account)\n"
#define STRING_HEAL_MEAN			"Average heal: %.1f\n"
#define STRING_HEAL_MEAN1			"Average heal: %.1f (considering it hits everytime)\n"
#define STRING_HEAL_MEAN2			"Average heal: %.1f (taking hit rate into account)\n"
#define STRING_HEAL_MEAN3			"Average heal: %.1f (taking hit rate and critical strikes into account)\n"

#define STRING_REFLECT				"The spell hits %s after reflecting on %d target(s)\n"
#define STRING_MULTI_TARGET_RATE	"It has %.1f chances to hit instead when multi-targeting\n"
#define STRING_IPSEN_CURSE			"The Ipsen curse inverts the weapon power from %d to %d\n"
#define STRING_SUMMON_STATUS		L"Also grants Protect when casting the long form of the summon\n"
#define STRING_CHAKRA				"Also heals %s's MP\n"
#define STRING_LANCER				L"Also deals MP damage\n"
#define STRING_DRAGON_BREATH		L"Note: the damage are always dealt to all the enemies independantly of the target picked\n"
#define STRING_WHITE_DRAW			L"Note: the MP heals are always applied to all the allies independantly of the target picked\n"
#define STRING_THROW				L"(Assume the weapon thrown is a copy of the equipped weapon: change that to select another throw)\n"
#define STRING_THROW_FLY			"Damage increased because %s has the flying classification\n"
#define STRING_ITEM					L"Item effects are not handled by the calculator\n"
#define STRING_ODIN_STATUS_FAIL		"The status cannot be applied because %s has Easy Kill Proof\n"
#define STRING_GRUDGE_NO_TONBERRY	L"It should not be used before any battle with tonberries ever started (to initialize the Tonberry counter to at least 1)\n"
#define STRING_BEAT_BACK_PLAYER		"Also pushes %s to the back row\n"
#define STRING_BEAT_BACK_ENEMY		"Also pushes %s forward, making any subsequent attack to or from the back row as efficient as if it were front row\n"

#define STRING_ALTERNATE_DAMAGE		"Damage with %s:"
#define STRING_ALTERNATE_HEAL		"Heals with %s:"
#define STRING_MULTI_TARGET			L"multi-targeting"
#define STRING_SHORT_SUMMON			L"short summon"
#define STRING_CRITICAL_STRIKE		L"critical strike"
#define STRING_POWER_UP				L"Power Up enabled"
#define STRING_POWER_THROW			L"Power Throw enabled"
#define STRING_HIGH_JUMP			L"High Jump enabled"

#define STRING_MAXHP_DEP_DAMAGE		"Damage with %d Max HP: "
#define STRING_HP_DEPENDANT_DAMAGE	"Damage with %d HP: "
#define STRING_MAXHP_DEP_HEAL		"Heal with %d Max HP: "
#define STRING_HP_DEPENDANT_HEAL	"Heal with %d HP: "
#define STRING_SPARE_CHANGE_FAIL	"Fail if the player has %d gil or less\n"
#define STRING_SPARE_CHANGE_DAMAGE	"Damage with %d gil: "
#define STRING_FROG_DROP_DAMAGE		"Damage with %d frogs: "
#define STRING_THIEVERY_DAMAGE		"Damage with %d steals: "
#define STRING_DRAGON_CREST_DAMAGE	"Damage with %d dragons killed: "
#define STRING_GRUDGE_DAMAGE		"Damage with %d tonberry killed: "
#define STRING_GRUDGE_DAMAGE_PLURAL	"Damage with %d tonberries killed: "

#define STRING_SPARE_CHANGE_ELAN	"When used under the command Elan, the damage is divided by the number of target(s)\n"
#define STRING_SPEAR_TRANCE			"The damage is divided by the number of target(s)\n"
#define STRING_FLEE_STATS			"When trying to flee using the bumpers, the check is performed every second that is not during an enemy turn\n"\
									"Mean fleeing time: %.2f seconds\n"\
									"Median fleeing time: %.0f seconds\n"
#define STRING_STEAL_ITEM			"Chances to steal the item in the %s slot: %.4f%%\n"
#define STRING_STEAL_NO_ITEM		"This enemy has no item\n"
#define STRING_STEAL_MUG			"Damage with Mug enabled: between %d and %d\n"
#define STRING_STEAL_GIL			"Gil obtained with Steal Gil enabled: between %d and %d\n"
#define STRING_ODIN_SWORD			L"The damage are dealt only if Odin Sword is activated and if the status alteration failed\n"


#define MACRO_CALCULATOR_SETFLAGS(VALUE, B1, B2, B3, B4, B5, B6, B7, B8) \
	B1->SetValue((VALUE & 0x01) != 0); \
	B2->SetValue((VALUE & 0x02) != 0); \
	B3->SetValue((VALUE & 0x04) != 0); \
	B4->SetValue((VALUE & 0x08) != 0); \
	B5->SetValue((VALUE & 0x10) != 0); \
	B6->SetValue((VALUE & 0x20) != 0); \
	B7->SetValue((VALUE & 0x40) != 0); \
	B8->SetValue((VALUE & 0x80) != 0);

#define MACRO_CALCULATOR_SETELEMS(VALUE) MACRO_CALCULATOR_SETFLAGS(VALUE, m_fire, m_ice, m_thunder, m_earth, m_water, m_wind, m_holy, m_shadow)
#define MACRO_CALCULATOR_SETELEMS_WIN(VALUE, WIN) MACRO_CALCULATOR_SETFLAGS(VALUE, WIN->m_fire, WIN->m_ice, WIN->m_thunder, WIN->m_earth, WIN->m_water, WIN->m_wind, WIN->m_holy, WIN->m_shadow)
#define MACRO_CALCULATOR_SETELEMS_WIN_NUM(VALUE, WIN, NUM) MACRO_CALCULATOR_SETFLAGS(VALUE, WIN->m_fire ## NUM, WIN->m_ice ## NUM, WIN->m_thunder ## NUM, WIN->m_earth ## NUM, WIN->m_water ## NUM, WIN->m_wind ## NUM, WIN->m_holy ## NUM, WIN->m_shadow ## NUM)
#define MACRO_CALCULATOR_SETCLASS(VALUE) MACRO_CALCULATOR_SETFLAGS(VALUE, m_human, m_beast, m_demon, m_dragon, m_undead, m_stone, m_bug, m_flying)

#define MACRO_CALCULATOR_GETFLAGS(VALUE, B1, B2, B3, B4, B5, B6, B7, B8) \
	VALUE = 0; \
	if (B1->GetValue()) VALUE |= 0x01; \
	if (B2->GetValue()) VALUE |= 0x02; \
	if (B3->GetValue()) VALUE |= 0x04; \
	if (B4->GetValue()) VALUE |= 0x08; \
	if (B5->GetValue()) VALUE |= 0x10; \
	if (B6->GetValue()) VALUE |= 0x20; \
	if (B7->GetValue()) VALUE |= 0x40; \
	if (B8->GetValue()) VALUE |= 0x80;

#define MACRO_CALCULATOR_GETELEMS(VALUE) MACRO_CALCULATOR_GETFLAGS(VALUE, m_fire, m_ice, m_thunder, m_earth, m_water, m_wind, m_holy, m_shadow)
#define MACRO_CALCULATOR_GETELEMS_WIN(VALUE, WIN) MACRO_CALCULATOR_GETFLAGS(VALUE, WIN->m_fire, WIN->m_ice, WIN->m_thunder, WIN->m_earth, WIN->m_water, WIN->m_wind, WIN->m_holy, WIN->m_shadow)
#define MACRO_CALCULATOR_GETELEMS_WIN_NUM(VALUE, WIN, NUM) MACRO_CALCULATOR_GETFLAGS(VALUE, WIN->m_fire ## NUM, WIN->m_ice ## NUM, WIN->m_thunder ## NUM, WIN->m_earth ## NUM, WIN->m_water ## NUM, WIN->m_wind ## NUM, WIN->m_holy ## NUM, WIN->m_shadow ## NUM)
#define MACRO_CALCULATOR_GETCLASS(VALUE) MACRO_CALCULATOR_GETFLAGS(VALUE, m_human, m_beast, m_demon, m_dragon, m_undead, m_stone, m_bug, m_flying)
#define MACRO_CALCULATOR_GETCLASS_WIN(VALUE, WIN) MACRO_CALCULATOR_GETFLAGS(VALUE, WIN->m_human, WIN->m_beast, WIN->m_demon, WIN->m_dragon, WIN->m_undead, WIN->m_stone, WIN->m_bug, WIN->m_flying)

#define MACRO_SWAP_WINDOW_ELEMENT(WIN, OTHERWIN, GETTER, SETTER) \
	tmp = WIN->GETTER(); \
	WIN->SETTER(OTHERWIN->GETTER()); \
	OTHERWIN->SETTER(tmp);


// Save/Load profiles

class DamageCalculatorSaveProfileEx : public DamageCalculatorSaveProfile {
public:
	DamageCalculatorSaveProfileEx(wxWindow* p) : DamageCalculatorSaveProfile(p) {}

private:
	void OnDeleteClick(wxCommandEvent& event) {
		EndModal(wxID_DELETE);
	}
};

void ToolDamageCalculator::OnSaveProfile(wxCommandEvent& event) {
	if (m_panelleft->GetSelection() == 0)
		player_panel[0]->UpdatePlayerStatFromInterface();
	if (m_panelright->GetSelection() == 0)
		player_panel[1]->UpdatePlayerStatFromInterface();
	while (true) {
		DamageCalculatorSaveProfileEx dial(this);
		wxArrayString profileid, profilename;
		set<int> defaultiduse, defaultnameuse;
		int i, j;
		PreferencesDialog::GetToolCalculatorProfileList(&profileid, &profilename);
		for (i = 0; i < profilename.GetCount(); i++) {
			dial.m_listprofile->Append(profilename[i]);
			if (profilename[i].StartsWith(wxT(HADES_STRING_CALCULATOR_PROFILE_DEFAULT)))
				defaultnameuse.insert(wxAtoi(profilename[i].Mid(sizeof(HADES_STRING_CALCULATOR_PROFILE_DEFAULT) - 1)));
			for (j = 0; j < profilename.GetCount(); j++)
				if (profileid[i].IsSameAs(wxString::Format(wxT("Profile%d"), j))) {
					defaultiduse.insert(j);
					break;
				}
		}
		if (profilename.GetCount() > 0) {
			dial.m_listprofile->SetSelection(0);
		} else {
			dial.m_radioold->Enable(false);
			dial.m_buttondelete->Enable(false);
		}
		for (i = 1; defaultnameuse.find(i) != defaultnameuse.end(); i++) {}
		dial.m_profilename->SetValue(wxT(HADES_STRING_CALCULATOR_PROFILE_DEFAULT) + wxString::Format(wxT("%d"), i));
		int modalid = dial.ShowModal();
		if (modalid == wxID_OK) {
			wxString newname;
			wxString newid;
			if (dial.m_radionew->GetValue()) {
				newname = dial.m_profilename->GetValue();
				newid = _(L"UnknownProfile");
				for (j = 0; j <= profileid.GetCount(); j++)
					if (defaultiduse.find(j) == defaultiduse.end()) {
						newid = wxString::Format(wxT("Profile%d"), j);
						break;
					}
			} else {
				newname = profilename[dial.m_listprofile->GetSelection()];
				newid = profileid[dial.m_listprofile->GetSelection()];
			}
			if (!PreferencesDialog::SaveToolCalculatorProfile(this, newid, newname)) {
				wxMessageDialog popup(this, HADES_STRING_CALCULATOR_SAVE_ERROR, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			}
		} else if (modalid == wxID_DELETE) {
			wxString delid = profileid[dial.m_listprofile->GetSelection()];
			if (!PreferencesDialog::DeleteToolCalculatorProfile(this, delid)) {
				wxMessageDialog popup(this, HADES_STRING_CALCULATOR_SAVE_ERROR, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			}
		}
		if (modalid != wxID_DELETE)
			return;
	}
}

void ToolDamageCalculator::OnLoadProfile(wxCommandEvent& event) {
	wxArrayString profileid, profilename;
	PreferencesDialog::GetToolCalculatorProfileList(&profileid, &profilename);
	if (profilename.GetCount() == 0) {
		wxMessageDialog popup(this, HADES_STRING_CALCULATOR_LOAD_NONE, HADES_STRING_WARNING, wxOK | wxSTAY_ON_TOP | wxCENTRE);
		popup.ShowModal();
		return;
	}
	DamageCalculatorLoadProfile dial(this);
	for (int i = 0; i < profilename.GetCount(); i++)
		dial.m_listprofile->Append(profilename[i]);
	dial.m_listprofile->SetSelection(0);
	if (dial.ShowModal() == wxID_OK) {
		if (PreferencesDialog::LoadToolCalculatorProfile(this, profileid[dial.m_listprofile->GetSelection()])) {
			player_panel[0]->ApplyPlayerStatToInterface();
			player_panel[1]->ApplyPlayerStatToInterface();
			UpdateInformation();
		} else {
			wxMessageDialog popup(this, HADES_STRING_CALCULATOR_LOAD_ERROR, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
			popup.ShowModal();
		}
	}
}

wxString CombineMultipleStrings(wxArrayString arr) {
	int i, count = 0, max = 0;
	for (i = 0; i < arr.size(); i++)
		if (!arr[i].IsEmpty())
			max++;
	if (max == 0)
		return wxEmptyString;
	wxString combination = _(L"");
	for (i = 0; i < arr.size(); i++)
		if (!arr[i].IsEmpty()) {
			count++;
			if (count > 1 && count < max)
				combination += _(L", ") + arr[i];
			else if (count > 1)
				combination += _(L" and ") + arr[i];
			else
				combination += arr[i];
		}
	return combination;
}

// Damage Calculation process

struct DamageCalculation {
	// Input
	wxString attacker_name = _(L"???");
	bool attacker_is_player = false;
	int attacker_player_slot = -1;
	int attacker_level = 1;
	int attacker_max_hp = 0;
	int attacker_speed = 0;
	int attacker_strength = 0;
	int attacker_magic = 0;
	int attacker_spirit = 0;
	int attacker_row = 0;
	int attacker_weapon_power = 0;
	int attacker_weapon_index = -1;
	uint8_t attacker_weapon_element = 0;
	uint8_t attacker_element_boost = 0;
	uint32_t attacker_status = 0;
	uint32_t attacker_weapon_status = 0;
	uint64_t attacker_support = 0;
	wxString defender_name = _(L"???");
	bool defender_is_player = false;
	int defender_player_slot = -1;
	int defender_level = 1;
	int defender_max_hp = 0;
	int defender_max_mp = 0;
	int defender_speed = 0;
	int defender_strength = 0;
	int defender_magic = 0;
	int defender_spirit = 0;
	int defender_defence = 0;
	int defender_evade = 0;
	int defender_magic_defence = 0;
	int defender_magic_evade = 0;
	int defender_row = 0;
	bool defender_has_item_slot_1 = false;
	bool defender_has_item_slot_2 = false;
	bool defender_has_item_slot_3 = false;
	bool defender_has_item_slot_4 = false;
	uint8_t defender_element_absorb = 0;
	uint8_t defender_element_immune = 0;
	uint8_t defender_element_half = 0;
	uint8_t defender_element_weak = 0;
	uint8_t defender_class = 0;
	uint32_t defender_status = 0;
	uint64_t defender_support = 0;
	bool is_self_attack = false;
	bool is_back_attack = false;
	bool is_ipsen_curse = false;
	int reflect_count = 0;
	int jewel_count = 0;
	uint16_t battle_flag = 0;
	wxString spell_name = _(L"???");
	uint8_t spell_effect = 0;
	int spell_command = -1;
	int spell_index = -1;
	int spell_power = 0;
	int spell_accuracy = 0;
	uint8_t spell_element = 0;
	uint32_t spell_status = 0;
	uint16_t spell_model = 0x200;
	CDDataStruct* cddata = NULL;

	// Output
	wxString formulae = _(L"");
	wxString fail_reason = _(L"");
	wxString special_effect_description = _(L"");
	wxString alternate_damage_description = _(L"???");
	wxString addendum_effect = _(L"");
	wxString addendum_end = _(L"");
	wxArrayString special_damage_description;
	vector<int> damage;
	vector<int> alternate_damage;
	vector<int> multi_target_damage;
	vector<int> special_damage;
	bool no_effect = false;
	bool has_damage = false;
	bool has_heal = false;
	bool has_status = false;
	bool has_special_damage = false;
	bool use_ipsen_curse = false;
	bool use_self_attack_no_evade = false;
	bool use_multi_target_damage = false;
	bool use_multi_target_rate = false;
	bool use_back_attack = false;
	bool use_row = false;
	bool is_summon = false;
	bool is_drain = false;
	bool cure_status = false;
	bool weapon_status = false;
	int use_jewel_id = -1;
	double hit_rate = -1.0;
	double status_rate = -1.0;
	double critical_rate = -1.0;
	double multi_target_rate = -1.0;
	bool use_caster_level = false;
	bool use_caster_max_hp = false;
	bool use_caster_speed = false;
	bool use_caster_strength = false;
	bool use_caster_magic = false;
	bool use_caster_spirit = false;
	bool use_caster_weapon = false;
	uint64_t use_caster_support = 0;
	set<int> use_caster_element_boost;
	set<int> use_caster_status_inc_rate;
	set<int> use_caster_status_low_rate;
	set<int> use_caster_status_inc_damage;
	set<int> use_caster_status_low_damage;
	bool use_target_level = false;
	bool use_target_max_hp = false;
	bool use_target_speed = false;
	bool use_target_strength = false;
	bool use_target_magic = false;
	bool use_target_spirit = false;
	bool use_target_defence = false;
	bool use_target_evade = false;
	bool use_target_magic_defence = false;
	bool use_target_magic_evade = false;
	bool use_target_zombie = false;
	uint64_t use_target_support = 0;
	set<int> use_target_element_affinity;
	set<int> use_target_status_inc_rate;
	set<int> use_target_status_low_rate;
	set<int> use_target_status_inc_damage;
	set<int> use_target_status_low_damage;
	set<int> apply_status;

	// Intermediate variables
	int i_hit_rate = 0;
	int i_ev = 0;
	int i_at_pow = 0;
	vector<int> i_df_pow;
	vector<int> i_at_num;
	vector<int> i_at_num_multi_target;

	static int increase_damage_half(int num) { return 3 * num / 2; }
	static int decrease_damage_half(int num) { return num / 2; }

	void GetHPValueSample(vector<int>& list, int maxhp) {
		list.push_back(1);
		if (maxhp <= 1)
			return;
		int step = 10;
		int limit = 100;
		int mult = 5;
		while (maxhp > limit) {
			step *= mult;
			limit *= mult;
			mult = mult == 5 ? 2 : 5;
		}
		for (int hp = step; hp < maxhp; hp += step)
			list.push_back(hp);
		list.push_back(maxhp);
	}

	void ApplyStatus(uint32_t status, bool checksummon) {
		uint32_t s = 1;
		for (int i = 0; i < STATUS_AMOUNT; i++) {
			if ((status & s) != 0) {
				apply_status.insert(i);
				has_status = true;
			}
			s <<= 1;
		}
		if (checksummon && is_summon && spell_command == 20 && (status & STATUS_PROTECT) == 0)
			addendum_effect += _(STRING_SUMMON_STATUS);
	}
	
	bool CheckStatus(uint32_t status, uint32_t check, set<int>& reflist) {
		uint32_t s = 1;
		for (int i = 0; i < STATUS_AMOUNT; i++) {
			if ((check & s) != 0 && (status & s) != 0) {
				reflist.insert(i);
				return true;
			}
			s <<= 1;
		}
		return false;
	}

	bool CheckElement(uint8_t element, uint8_t check, set<int>& reflist) {
		uint32_t e = 1;
		for (int i = 0; i < 8; i++) {
			if ((check & e) != 0 && (element & e) != 0) {
				reflist.insert(i);
				return true;
			}
			e <<= 1;
		}
		return false;
	}

	bool CheckStatusFail(uint32_t status, uint32_t check, bool isattacker = false) {
		uint32_t s = 1;
		for (int i = 0; i < STATUS_AMOUNT; i++) {
			if ((check & s) != 0 && (status & s) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_STATUS), isattacker ? attacker_name : defender_name, HADES_STRING_STATUS_NAME[i]);
				return false;
			}
			s <<= 1;
		}
		return true;
	}

	bool CheckElementFail(uint8_t element, uint8_t check, bool isattacker = false) {
		uint32_t e = 1;
		for (int i = 0; i < 8; i++) {
			if ((check & e) != 0 && (element & e) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_ELEMENT), isattacker ? attacker_name : defender_name, HADES_STRING_ELEMENT_NAME[i]);
				return false;
			}
			e <<= 1;
		}
		return true;
	}

	bool CheckSelfAttackFail() {
		if (is_self_attack) {
			fail_reason = _(STRING_FAIL_SELF_ATTACK);
			return false;
		}
		return true;
	}

	bool CheckLevelMod() {
		use_target_level = true;
		if (spell_accuracy == 0) {
			fail_reason = _(STRING_FAIL_ZERO_DIVISION);
			return false;
		}
		if ((defender_level % spell_accuracy) != 0) {
			fail_reason = wxString::Format(wxT(STRING_FAIL_LEVEL_MOD), defender_name, spell_accuracy);
			return false;
		}
		return true;
	}

	bool PhysicalAccuracy(bool checkaccplus) {
		if (checkaccplus && (attacker_support & SUPPORT_ABILITY_ACCURACY_PLUS) != 0) {
			hit_rate = 1.0;
			use_caster_support |= SUPPORT_ABILITY_ACCURACY_PLUS;
			return true;
		}
		if (CheckStatus(defender_status, STATUS_VANISH, use_target_status_low_rate)) {
			fail_reason = wxString::Format(wxT(STRING_FAIL_STATUS), defender_name, HADES_STRING_STATUS_NAME[26]);
			return false;
		}
		use_target_evade = true;
		i_hit_rate = 100;
		i_ev = defender_evade;
		if (CheckStatus(attacker_status, STATUS_BLIND | STATUS_CONFUSE, use_caster_status_low_rate))
			i_hit_rate /= 2;
		if (CheckStatus(attacker_status, STATUS_TRANCE | STATUS_VANISH, use_caster_status_inc_rate))
			i_ev = 0;
		if ((defender_support & SUPPORT_ABILITY_DISTRACT) != 0) {
			use_target_support |= SUPPORT_ABILITY_DISTRACT;
			i_hit_rate /= 2;
		}
		if (CheckStatus(defender_status, STATUS_PETRIFY | STATUS_VENOM | STATUS_VIRUS | STATUS_BLIND | STATUS_CONFUSE | STATUS_STOP | STATUS_SLEEP | STATUS_FREEZE, use_target_status_inc_rate))
			i_ev = 0;
		if (CheckStatus(defender_status, STATUS_DEFEND, use_target_status_low_rate))
			i_hit_rate /= 2;
		if (is_self_attack) {
			use_self_attack_no_evade = true;
			i_ev = 0;
		}
		i_ev = min(100, i_ev);
		hit_rate = (i_hit_rate / 100.0) * ((100 - i_ev) / 100.0);
		return true;
	}

	void SetupAccuracyPhysic() {
		use_target_evade = true;
		i_hit_rate = 100;
		i_ev = defender_evade;
	}

	void SetupAccuracyMagic(bool isstatus = false, bool addformula = true) {
		use_caster_level = true;
		use_caster_magic = true;
		use_target_level = true;
		use_target_magic_evade = true;
		i_hit_rate = max(1, min(100, spell_accuracy + attacker_magic / 4 + attacker_level - defender_level));
		i_ev = defender_magic_evade;
		if (addformula) formulae += wxString::Format(isstatus ? wxT(STRING_FORMULA_STATUS_RATE) : wxT(STRING_FORMULA_RATE), _(STRING_FORMULA_RATE_MAGIC));
	}

	void SetupAttackCommand(uint8_t type, bool addformula = true) {
		int min = 0, max = (attacker_level + attacker_strength) / 8;
		use_caster_level = true;
		use_caster_strength = true;
		use_caster_weapon = true;
		use_target_defence = true;
		i_at_pow = attacker_weapon_power;
		i_df_pow.push_back(defender_defence);
		switch (type)
		{
		case 1:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8));
			min = attacker_strength;
			break;
		case 2:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8_SPIRIT));
			min = (attacker_strength + attacker_spirit) / 2;
			use_caster_spirit = true;
			break;
		case 3:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8_SPEED));
			min = (attacker_strength + attacker_speed) / 2;
			use_caster_speed = true;
			break;
		case 4:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8_MAGIC));
			min = (attacker_strength + attacker_magic) / 2;
			use_caster_magic = true;
			break;
		case 5:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8_RANDOM));
			for (int i = min; i <= min + max; i++)
				for (int j = 0; j < attacker_strength; j++)
					i_at_num.push_back(i + j);
			return;
		case 6:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8));
			min = attacker_strength;
			is_drain = true;
			break;
		case 7:
			if (addformula) formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_BASE8_LEVEL));
			i_at_pow += attacker_level;
			min = (attacker_strength + attacker_spirit) / 2;
			use_caster_spirit = true;
			break;
		}
		max += min;
		for (int i = min; i <= max; i++)
			i_at_num.push_back(i);
	}

	void SetupAttack(int divider, bool strength, bool physical) {
		int min = strength ? attacker_strength : attacker_magic;
		int max = min + (attacker_level + (strength ? attacker_strength : attacker_magic)) / divider;
		use_caster_level = true;
		if (strength)
			use_caster_strength = true;
		else
			use_caster_magic = true;
		if (physical)
			use_target_defence = true;
		else
			use_target_magic_defence = true;
		i_at_pow = spell_power;
		i_df_pow.push_back(physical ? defender_defence : defender_magic_defence);
		for (int i = min; i <= max; i++)
			i_at_num.push_back(i);
	}

	void ApplyKillerMPAttack() {
		bool killer = true;
		if ((attacker_support & SUPPORT_ABILITY_BIRD_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_FLYING) != 0) use_caster_support |= SUPPORT_ABILITY_BIRD_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_BUG_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_BUG) != 0) use_caster_support |= SUPPORT_ABILITY_BUG_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_STONE_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_STONE) != 0) use_caster_support |= SUPPORT_ABILITY_STONE_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_UNDEAD_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) use_caster_support |= SUPPORT_ABILITY_UNDEAD_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_DRAGON_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_DRAGON) != 0) use_caster_support |= SUPPORT_ABILITY_DRAGON_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_DEVIL_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_DEMON) != 0) use_caster_support |= SUPPORT_ABILITY_DEVIL_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_BEAST_KILLER) != 0 && (defender_class & ENEMY_CLASSIFICATION_BEAST) != 0) use_caster_support |= SUPPORT_ABILITY_BEAST_KILLER;
		else if ((attacker_support & SUPPORT_ABILITY_MAN_EATER) != 0 && (defender_class & ENEMY_CLASSIFICATION_HUMAN) != 0) use_caster_support |= SUPPORT_ABILITY_MAN_EATER;
		else killer = false;
		if (killer)
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
		if ((attacker_support & SUPPORT_ABILITY_MP_ATTACK) != 0)
		{
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			use_caster_support |= SUPPORT_ABILITY_MP_ATTACK;
		}
	}

	void ApplyGambleDefence() {
		if ((defender_support & SUPPORT_ABILITY_GAMBLE_DEFENCE) != 0) {
			vector<int> newdef;
			int i, j;
			for (i = 0; i < i_df_pow.size(); i++) {
				if (i_df_pow[i] == 0)
					newdef.push_back(0);
				else
					for (j = 2 * i_df_pow[i] - 1; j >= 0 ; j--)
						newdef.push_back(j);
			}
			i_df_pow = newdef;
			use_target_support |= SUPPORT_ABILITY_GAMBLE_DEFENCE;
		}
	}

	void ApplyBerserkTranceMini(vector<int>* atnum = NULL) {
		if (atnum == NULL)
			atnum = &i_at_num;
		if (CheckStatus(attacker_status, STATUS_BERSERK | STATUS_TRANCE, use_caster_status_inc_damage))
			transform(atnum->begin(), atnum->end(), atnum->begin(), increase_damage_half);
		if (CheckStatus(attacker_status, STATUS_MINI, use_caster_status_low_damage)) {
			atnum->clear();
			atnum->push_back(1);
		}
	}

	void ApplyCasterHalfMini(vector<int>* atnum = NULL) {
		if (atnum == NULL)
			atnum = &i_at_num;
		if (CheckStatus(attacker_status, STATUS_MINI, use_caster_status_low_damage))
			transform(atnum->begin(), atnum->end(), atnum->begin(), decrease_damage_half);
	}

	void ApplyDefendProtectSleepMini(vector<int>* atnum = NULL) {
		if (atnum == NULL)
			atnum = &i_at_num;
		if (CheckStatus(defender_status, STATUS_DEFEND | STATUS_PROTECT, use_target_status_low_damage))
			transform(atnum->begin(), atnum->end(), atnum->begin(), decrease_damage_half);
		if (CheckStatus(defender_status, STATUS_SLEEP | STATUS_MINI, use_target_status_inc_damage))
			transform(atnum->begin(), atnum->end(), atnum->begin(), increase_damage_half);
	}

	void ApplyShell() {
		if (CheckStatus(defender_status, STATUS_SHELL, use_target_status_low_damage))
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), decrease_damage_half);
	}

	void ApplyConcentrate() {
		if ((attacker_support & SUPPORT_ABILITY_CONCENTRATE) != 0)
		{
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			use_caster_support |= SUPPORT_ABILITY_CONCENTRATE;
		}
	}

	void ApplyBackAttackBackRow() {
		if (is_back_attack) {
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			use_back_attack = true;
		}
		if (abs(attacker_row - defender_row) > 1) {
			if ((attacker_support & SUPPORT_ABILITY_LONG_REACH) != 0) {
				use_caster_support |= SUPPORT_ABILITY_LONG_REACH;
			} else {
				transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), decrease_damage_half);
				use_row = true;
			}
		}
	}

	void ApplyShellAccuracy() {
		if (CheckStatus(defender_status, STATUS_SHELL, use_target_status_low_rate))
			i_hit_rate /= 2;
	}

	void ApplyElementBoost(uint8_t element) {
		if (CheckElement(attacker_element_boost, element, use_caster_element_boost)) {
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			if (use_multi_target_damage)
				transform(i_at_num_multi_target.begin(), i_at_num_multi_target.end(), i_at_num_multi_target.begin(), increase_damage_half);
		}
	}

	bool ApplyElementAffinity(uint8_t element, bool checkelemnull) {
		if (checkelemnull && (attacker_support & SUPPORT_ABILITY_MAG_ELEM_NULL) != 0) {
			if (element != 0)
				use_caster_support |= SUPPORT_ABILITY_MAG_ELEM_NULL;
			return true;
		}
		if (!CheckElementFail(defender_element_immune, element))
			return false;
		if (CheckElement(defender_element_half, element, use_target_element_affinity)) {
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), decrease_damage_half);
			if (use_multi_target_damage)
				transform(i_at_num_multi_target.begin(), i_at_num_multi_target.end(), i_at_num_multi_target.begin(), decrease_damage_half);
		}
		if (CheckElement(defender_element_absorb, element, use_target_element_affinity)) {
			i_df_pow.clear();
			i_df_pow.push_back(0);
			has_heal = true;
		}
		if (CheckElement(defender_element_weak, element, use_target_element_affinity)) {
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			if (use_multi_target_damage)
				transform(i_at_num_multi_target.begin(), i_at_num_multi_target.end(), i_at_num_multi_target.begin(), increase_damage_half);
		}
		return true;
	}

	bool ApplyEarthMiss() {
		if ((spell_element & ELEMENT_EARTH) != 0) {
			if ((defender_status & STATUS_FLOAT) != 0 || (defender_class & ENEMY_CLASSIFICATION_FLYING) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_EARTH_FLY), defender_name);
				return false;
			}
		}
		return true;
	}

	void ApplyIpsenCurse() {
		if (is_ipsen_curse)
		{
			int newpow = max(1, 60 - i_at_pow);
			addendum_effect += wxString::Format(wxT(STRING_IPSEN_CURSE), i_at_pow, newpow);
			i_at_pow = newpow;
		}
	}

	void SetupCritical() {
		use_caster_spirit = true;
		if (attacker_spirit < 8)
			return;
		critical_rate = (attacker_spirit / 4 - 1) * 0.005;
		formulae += _(STRING_FORMULA_CRITICAL);
		// TODO: compute differently for the PSX version (same random number is picked in the following formula)
		//if (Comn.random16() % (int)(v.caster.elem.wpr >> 2) > Comn.random16() % 100)
	}

	void SetupMultiTargetDamage() {
		for (int i = 0; i < i_at_num.size(); i++)
			i_at_num_multi_target.push_back(i_at_num[i] / 2);
		use_multi_target_damage = true;
	}

	void ComputeDamage(int minpow = 1, int minnum = 1) {
		int i, j;
		has_damage = true;
		for (i = 0; i < i_at_num.size(); i++)
			for (j = 0; j < i_df_pow.size(); j++)
				damage.push_back(min(9999, max(minpow, i_at_pow - i_df_pow[j]) * max(minnum, i_at_num[i])));
		if (use_multi_target_damage)
			for (i = 0; i < i_at_num_multi_target.size(); i++)
				for (j = 0; j < i_df_pow.size(); j++)
					multi_target_damage.push_back(min(9999, max(minpow, i_at_pow - i_df_pow[j]) * max(minnum, i_at_num_multi_target[i])));
		if (is_summon) {
			alternate_damage_description = _(STRING_SHORT_SUMMON);
			alternate_damage.clear();
			for (i = 0; i < i_at_num.size(); i++)
				for (j = 0; j < i_df_pow.size(); j++)
					alternate_damage.push_back(min(9999, max(minpow, i_at_pow - i_df_pow[j]) * max(minnum, i_at_num[i]) * 2 / 3));
		} else if (critical_rate >= 0) {
			alternate_damage_description = _(STRING_CRITICAL_STRIKE);
			alternate_damage.clear();
			for (i = 0; i < i_at_num.size(); i++)
				for (j = 0; j < i_df_pow.size(); j++)
					alternate_damage.push_back(min(9999, max(minpow, i_at_pow - i_df_pow[j]) * max(minnum, 2 * i_at_num[i])));
		}
	}

	void ComputeDamageGravity(bool maxhp) {
		if (i_at_num.size() < 1)
			return;
		int pow = i_at_num[0];
		vector<int> hp;
		int i;
		has_special_damage = true;
		use_target_max_hp = true;
		if (maxhp)
			hp.push_back(defender_max_hp);
		else
			GetHPValueSample(hp, defender_max_hp);
		wxFormatString dmgfmt = has_heal ? (maxhp ? wxT(STRING_MAXHP_DEP_HEAL) : wxT(STRING_HP_DEPENDANT_HEAL)) : (maxhp ? wxT(STRING_MAXHP_DEP_DAMAGE) : wxT(STRING_HP_DEPENDANT_DAMAGE));
		special_effect_description = has_heal ? _(STRING_HEAL_SPECIAL) : _(STRING_DAMAGE_SPECIAL);
		for (i = 0; i < hp.size(); i++) {
			special_damage_description.Add(wxString::Format(dmgfmt, hp[i]));
			special_damage.push_back(min(9999, min(100, pow) * hp[i] / 100));
		}
		if (is_summon) {
			special_damage_description.Add(_(L"\n") + wxString::Format(has_heal ? wxT(STRING_ALTERNATE_HEAL) : wxT(STRING_ALTERNATE_DAMAGE), _(STRING_SHORT_SUMMON)) + _(L"\n") + wxString::Format(dmgfmt, hp[0]));
			special_damage.push_back(min(9999, min(100, pow) * hp[0] / 100 * 2 / 3));
			for (i = 1; i < hp.size(); i++) {
				special_damage_description.Add(wxString::Format(dmgfmt, hp[i]));
				special_damage.push_back(min(9999, min(100, pow) * hp[i] / 100 * 2 / 3));
			}
		}
	}

	void ComputeHitRate(double& rate) {
		rate = (i_hit_rate / 100.0) * ((100 - i_ev) / 100.0);
		if (use_multi_target_rate)
			multi_target_rate = (i_hit_rate / 2 / 100.0) * ((100 - i_ev) / 100.0);
	}

	void ComputeHeal() {
		int i;
		if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
			use_target_zombie = true;
			has_damage = true;
		} else {
			has_heal = true;
		}
		for (i = 0; i < i_at_num.size(); i++)
			damage.push_back(min(9999, i_at_pow * i_at_num[i]));
		if (use_multi_target_damage)
			for (i = 0; i < i_at_num_multi_target.size(); i++)
				multi_target_damage.push_back(min(9999, i_at_pow * i_at_num_multi_target[i]));
	}

	void SetHPToPower() {
		if (spell_power > 0)
			special_effect_description = wxString::Format(wxT(STRING_SET_HP_VALUE), defender_name, spell_power);
		else
			special_effect_description = wxString::Format(wxT(STRING_ENFORCE_KILL), defender_name);
	}

	void ProcessMain() {
		int i, j;
		is_summon = spell_command == 16 || spell_command == 18 || spell_command == 20;
		switch (spell_effect) {
		default:
			no_effect = true;
			break;
		case 1: // Attack Standard
		case 2: // Attack Thief Sword
		case 3: // Attack Rackets
		case 4: // Attack Scrolls
		case 5: // Attack Forks
		case 6: // Attack Blood Sword
		case 7: // Attack Save The Queen
			if (spell_effect == 6 && is_self_attack) {
				fail_reason = _(STRING_FAIL_SELF_ATTACK);
				break;
			}
			if (!PhysicalAccuracy(true))
				break;
			SetupAttackCommand(spell_effect, true);
			ApplyKillerMPAttack();
			ApplyBerserkTranceMini();
			ApplyGambleDefence();
			ApplyDefendProtectSleepMini();
			if (spell_effect != 6 && attacker_player_slot == 3 && CheckStatus(attacker_status, STATUS_TRANCE, use_caster_status_inc_damage))
				transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), [](int num) { return 2 * num; });
			ApplyBackAttackBackRow();
			if (spell_effect != 6) {
				ApplyElementBoost(attacker_weapon_element);
				if (!ApplyElementAffinity(attacker_weapon_element, false))
					break;
			}
			SetupCritical();
			ApplyIpsenCurse();
			if (spell_effect == 6) {
				is_drain = true;
				if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
					use_target_zombie = true;
					has_heal = true;
				}
			}
			ComputeDamage();
			if (spell_effect != 6 && (attacker_support & SUPPORT_ABILITY_HEALER) != 0) {
				use_caster_support |= SUPPORT_ABILITY_HEALER;
				if ((defender_status & STATUS_ZOMBIE) == 0 && (defender_class & ENEMY_CLASSIFICATION_UNDEAD) == 0)
					has_heal = true;
				else
					use_target_zombie = true;
			}
			if (spell_effect != 6) {
				ApplyStatus(attacker_weapon_status, false);
				status_rate = spell_accuracy / 100.0;
				weapon_status = true;
			}
			break;
		case 8: // Enemy Attack
			if (!PhysicalAccuracy(false))
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC4));
			SetupAttack(4, true, true);
			ApplyBerserkTranceMini();
			ApplyGambleDefence();
			ApplyDefendProtectSleepMini();
			ApplyBackAttackBackRow();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			SetupCritical();
			ComputeDamage();
			ApplyStatus(spell_status, false);
			status_rate = spell_accuracy / 100.0;
			break;
		case 9: // Magic Attack
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC8));
			SetupAttack(8, false, false);
			ApplyCasterHalfMini();
			ApplyShell();
			SetupMultiTargetDamage();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			ComputeDamage();
			ApplyStatus(spell_status, false);
			status_rate = spell_accuracy / 100.0;
			break;
		case 10: // Magic Heal
			formulae += wxString::Format(wxT(STRING_FORMULA_HEAL), _(STRING_FORMULA_HEAL8));
			SetupAttack(8, false, false);
			ApplyConcentrate();
			ApplyCasterHalfMini();
			SetupMultiTargetDamage();
			ComputeHeal();
			break;
		case 11: // Bad Status
			SetupAccuracyMagic(true);
			ApplyShellAccuracy();
			use_multi_target_rate = true;
			ApplyStatus(spell_status, true);
			ComputeHitRate(status_rate);
			break;
		case 12: // Cure Status
			ApplyStatus(spell_status, false);
			cure_status = true;
			break;
		case 13: // Life
		{
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY))
				break;
			if ((defender_status & STATUS_DEATH) != 0 && (defender_status & STATUS_ZOMBIE) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_DEAD_ZOMBIE), defender_name);
				break;
			}
			if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
				use_target_status_low_rate.insert(6);
				SetupAccuracyMagic();
				ComputeHitRate(hit_rate);
				special_effect_description = wxString::Format(wxT(STRING_ENFORCE_KILL), defender_name);
				break;
			}
			if (!defender_is_player) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_ENEMY), defender_name);
				break;
			}
			formulae += wxString::Format(wxT(STRING_FORMULA_HEAL), _(STRING_FORMULA_LIFE));
			ApplyStatus(spell_status, false);
			cure_status = true;
			int dmg = defender_max_hp * (defender_spirit + spell_power);
			if ((attacker_support & SUPPORT_ABILITY_CONCENTRATE) != 0) {
				use_caster_support |= SUPPORT_ABILITY_CONCENTRATE;
				dmg /= 50;
			} else {
				dmg /= 100;
			}
			damage.push_back(min(9999, dmg));
			has_heal = true;
			break;
		}
		case 14: // Death
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL))
				break;
			if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
				if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
					break;
				special_effect_description = wxString::Format(wxT(STRING_FULL_HEAL_HP), defender_name);
				break;
			}
			SetupAccuracyMagic(true);
			ComputeHitRate(status_rate);
			ApplyStatus(spell_status, true);
			break;
		case 15: // Osmose
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
				break;
			if (!CheckSelfAttackFail())
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_MPDAMAGE), _(STRING_FORMULA_OSMOSE));
			is_drain = true;
			SetupAttack(8, false, false);
			ApplyCasterHalfMini();
			ApplyShell();
			has_damage = true;
			for (i = 0; i < i_at_num.size(); i++)
				for (j = 0; j < i_df_pow.size(); j++)
					damage.push_back(min(9999, max(0, i_at_pow - i_df_pow[j]) * max(1, i_at_num[i]) / 4));
			if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
				special_effect_description = _(STRING_HEAL_MP);
				use_target_zombie = true;
				has_heal = true;
			} else {
				special_effect_description = _(STRING_DAMAGE_MP);
			}
			break;
		case 16: // Drain
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
				break;
			if (!CheckSelfAttackFail())
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC8));
			is_drain = true;
			SetupAttack(8, false, false);
			ApplyCasterHalfMini();
			ApplyShell();
			ComputeDamage(0);
			if ((defender_status & STATUS_ZOMBIE) != 0 || (defender_class & ENEMY_CLASSIFICATION_UNDEAD) != 0) {
				use_target_zombie = true;
				has_heal = true;
			}
			break;
		case 17: // Gravity
		case 91: // Cannon
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL))
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), spell_effect == 17 ? _(STRING_FORMULA_GRAVITY) : _(STRING_FORMULA_CANNON));
			SetupAccuracyMagic();
			ApplyShellAccuracy();
			use_multi_target_rate = true;
			ComputeHitRate(hit_rate);
			i_at_num.push_back(spell_power);
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			ComputeDamageGravity(spell_effect == 17);
			break;
		case 18: // Random Magic
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC_RANDOM));
			if (spell_index == 46 && (spell_command == 22 || spell_command == 23)) {
				formulae += wxString::Format(wxT(STRING_FORMULA_RATE), _(STRING_FORMULA_RATE_METEOR));
				use_caster_spirit = true;
				use_caster_level = true;
				hit_rate = (attacker_level / 2 + attacker_spirit + 1) / 100.0;
				if (spell_model == 56)
					hit_rate *= 0.6640625;
			} else if (spell_model == 56) {
				hit_rate = 0.6640625;
			}
			use_caster_magic = true;
			use_caster_level = true;
			for (i = 0; i < attacker_magic + attacker_level; i++)
				i_at_num.push_back(i);
			i_at_pow = spell_power;
			i_df_pow.push_back(0);
			ApplyCasterHalfMini();
			ApplyShell();
			SetupMultiTargetDamage();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			break;
		case 19: // Physical Strike
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC8));
			SetupAttack(8, true, true);
			use_caster_weapon = true;
			i_at_pow = attacker_weapon_power * spell_power / 10;
			ApplyBerserkTranceMini();
			ApplyDefendProtectSleepMini();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(attacker_weapon_element, false))
				break;
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			break;
		case 20: // Magic Weapon
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC_WEAPON));
			SetupAttack(8, true, false);
			use_caster_weapon = true;
			i_at_pow = attacker_weapon_power * spell_power / 10;
			ApplyCasterHalfMini();
			ApplyShell();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			break;
		case 21: // Goblin Punch
			SetupAttack(8, false, false);
			use_caster_level = true;
			use_target_level = true;
			if (attacker_level == defender_level) {
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_GOBLIN_PUNCH));
				transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), [this](int num) { return num + this->attacker_level; });
				i_df_pow.clear();
				i_df_pow.push_back(0);
			} else {
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC8));
			}
			ApplyCasterHalfMini();
			ApplyShell();
			ComputeDamage();
			break;
		case 22: // LV? Death
			if (!CheckLevelMod())
				break;
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
				break;
			SetHPToPower();
			break;
		case 23: // LV? Attack
			if (!CheckLevelMod())
				break;
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC8));
			SetupAttack(8, false, false);
			ApplyCasterHalfMini();
			ApplyShell();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			ComputeDamage();
			break;
		case 24: // LV? Defless
			if (!CheckLevelMod())
				break;
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY | STATUS_DEATH))
				break;
			if (defender_defence != 0)
				special_effect_description += wxString::Format(wxT(STRING_CHANGE_STAT_RND), defender_name, L"defence", 0, defender_defence - 1);
			if (defender_defence != 0 && defender_magic_defence != 0)
				special_effect_description += _(L" and ");
			if (defender_magic_defence != 0)
				special_effect_description += wxString::Format(wxT(STRING_CHANGE_STAT_RND), defender_name, L"magic defence", 0, defender_magic_defence - 1);
			break;
		case 25: // Roulette
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL | STATUS_PETRIFY | STATUS_DEATH))
				break;
			SetHPToPower();
			break;
		case 26: // Pure Damage
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PURE));
			has_damage = true;
			damage.push_back(spell_power * 100 + spell_accuracy);
			break;
		case 27: // Matra Magic
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL | STATUS_PETRIFY | STATUS_DEATH))
				break;
			SetupAccuracyMagic();
			ApplyShellAccuracy();
			ComputeHitRate(hit_rate);
			SetHPToPower();
			break;
		case 28: // Limit Glove
			special_effect_description = wxString::Format(wxT(STRING_LIMIT_GLOVE), spell_power * 100 + spell_accuracy, attacker_name);
			break;
		case 29: // Minus Strike
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MINUS_STRIKE));
			vector<int> hp;
			GetHPValueSample(hp, attacker_max_hp);
			has_special_damage = true;
			use_caster_max_hp = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			for (i = 0; i < hp.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_HP_DEPENDANT_DAMAGE), hp[i]));
				special_damage.push_back(attacker_max_hp - hp[i]);
			}
			break;
		}
		case 30: // White Wind
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_WHITE_WIND));
			if ((defender_status & STATUS_ZOMBIE) != 0) {
				has_damage = true;
				use_target_zombie = true;
			} else {
				has_heal = true;
			}
			use_caster_max_hp = true;
			damage.push_back(attacker_max_hp / 3);
			break;
		case 31: // Magic Hammer
			formulae += wxString::Format(wxT(STRING_FORMULA_MPDAMAGE), _(STRING_FORMULA_MAGIC_HAMMER));
			special_effect_description = _(STRING_DAMAGE_MP);
			break;
		case 32: // Darkside
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC8));
			SetupAttack(8, true, true);
			use_caster_weapon = true;
			i_at_pow = attacker_weapon_power * spell_power / 10;
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			ComputeDamage(0);
			break;
		case 33: // Armor Break
		case 34: // Power Break
		case 35: // Mental Break
		case 36: // Magic Break
			SetupAccuracyMagic();
			ApplyShellAccuracy();
			ComputeHitRate(hit_rate);
			if (spell_effect == 33) {
				use_target_defence = true;
				special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"defence", defender_defence / 2);
			} else if (spell_effect == 34) {
				use_target_strength = true;
				special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"strength", defender_strength * 3 / 4);
			} else if (spell_effect == 35) {
				use_target_magic_defence = true;
				special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"magic defence", defender_magic_defence / 2);
			} else if (spell_effect == 36) {
				use_target_magic = true;
				special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"magic", defender_magic * 3 / 4);
			}
			break;
		case 37: // Chakra
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_GRAVITY));
			formulae += wxString::Format(wxT(STRING_FORMULA_MPDAMAGE), _(STRING_FORMULA_CHAKRA_MP));
			has_heal = true;
			use_target_max_hp = true;
			damage.push_back(defender_max_hp * spell_power / 100);
			alternate_damage.push_back(defender_max_hp * spell_power / 50);
			addendum_effect += wxString::Format(wxT(STRING_CHAKRA), defender_name);
			alternate_damage_description = _(STRING_POWER_UP);
			break;
		case 38: // Spare Change
		{
			if (spell_power == 0) {
				// Only divide by zero when power is 0 and the player's current gil is 0, but still warn anyways
				fail_reason = _(STRING_FAIL_ZERO_DIVISION);
				break;
			}
			formulae += _(STRING_FORMULA_GIL);
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SPARE_CHANGE));
			vector<int> gil;
			int basegil = spell_power * attacker_level;
			gil.push_back(basegil);
			gil.push_back(2 * basegil);
			gil.push_back(10 * basegil);
			if (10 * basegil < 10000)
				gil.push_back(10000);
			if (10 * basegil < 50000)
				gil.push_back(50000);
			if (10 * basegil < 100000)
				gil.push_back(100000);
			if (10 * basegil < 500000)
				gil.push_back(500000);
			if (10 * basegil < 1000000)
				gil.push_back(1000000);
			has_special_damage = true;
			use_caster_level = true;
			use_caster_spirit = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			special_damage_description.Add(wxString::Format(wxT(STRING_SPARE_CHANGE_FAIL), basegil - 1) + wxString::Format(wxT(STRING_SPARE_CHANGE_DAMAGE), gil[0]));
			special_damage.push_back(min(9999, basegil * basegil / 10 * attacker_spirit / gil[0]));
			for (i = 1; i < gil.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_SPARE_CHANGE_DAMAGE), gil[i]));
				special_damage.push_back(min(9999, basegil * basegil / 10 * attacker_spirit / gil[i]));
			}
			addendum_end += _(STRING_SPARE_CHANGE_ELAN);
			break;
		}
		case 39: // Lancer
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC8));
			formulae += wxString::Format(wxT(STRING_FORMULA_MPDAMAGE), _(STRING_FORMULA_LANCER_MP));
			SetupAttack(8, true, true);
			use_caster_weapon = true;
			i_at_pow = attacker_weapon_power * spell_power / 10;
			ApplyBerserkTranceMini();
			ApplyDefendProtectSleepMini();
			ComputeDamage(0);
			addendum_effect += _(STRING_LANCER);
			break;
		case 40: // Dragon Breath
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_DRAGON_BR));
			vector<int> hp;
			GetHPValueSample(hp, defender_max_hp);
			has_special_damage = true;
			use_target_max_hp = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			for (i = 0; i < hp.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_HP_DEPENDANT_DAMAGE), hp[i]));
				special_damage.push_back(min(9999, defender_max_hp - hp[i]));
			}
			addendum_effect += _(STRING_DRAGON_BREATH);
			break;
		}
		case 41: // White Draw
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_MPHEAL), _(STRING_FORMULA_WHITE_DRAW));
			if (!CheckStatusFail(defender_status, STATUS_DEATH))
				break;
			use_target_level = true;
			special_effect_description = wxString::Format(wxT(STRING_MPHEAL_RANGE), 0, 2 * defender_level);
			addendum_effect += _(STRING_WHITE_DRAW);
			break;
		}
		case 42: // Throw
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_THROW));
			vector<int> powerthrownum;
			use_caster_weapon = true;
			addendum_effect += _(STRING_THROW);
			alternate_damage_description = _(STRING_POWER_THROW);
			SetupAttack(8, true, true);
			i_at_pow = 2 * attacker_weapon_power;
			for (i = 0; i < i_at_num.size(); i++)
				powerthrownum.push_back(i_at_num[i] * 3 / 2);
			ApplyBerserkTranceMini();
			ApplyBerserkTranceMini(&powerthrownum);
			ApplyDefendProtectSleepMini();
			ApplyDefendProtectSleepMini(&powerthrownum);
			if ((defender_class & ENEMY_CLASSIFICATION_FLYING) != 0) {
				addendum_effect += wxString::Format(wxT(STRING_THROW_FLY), defender_name);
				transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
				transform(powerthrownum.begin(), powerthrownum.end(), powerthrownum.begin(), increase_damage_half);
			}
			ComputeDamage();
			for (i = 0; i < powerthrownum.size(); i++)
				for (j = 0; j < i_df_pow.size(); j++)
					alternate_damage.push_back(min(9999, max(1, i_at_pow - i_df_pow[j]) * max(1, powerthrownum[i])));
			if ((attacker_support & SUPPORT_ABILITY_HEALER) != 0) {
				use_caster_support |= SUPPORT_ABILITY_HEALER;
				if ((defender_status & STATUS_ZOMBIE) == 0 && (defender_class & ENEMY_CLASSIFICATION_UNDEAD) == 0)
					has_heal = true;
				else
					use_target_zombie = true;
			}
			break;
		}
		case 43: // Might
			if (spell_power == 0) {
				fail_reason = _(STRING_FAIL_ZERO_DIVISION);
				break;
			}
			use_target_strength = true;
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"strength", min(99, defender_strength + defender_strength / spell_power));
			break;
		case 44: // Focus
			if (spell_power == 0) {
				fail_reason = _(STRING_FAIL_ZERO_DIVISION);
				break;
			}
			use_target_magic = true;
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"magic", min(99, defender_magic + defender_magic / spell_power));
			break;
		case 45: // Sacrifice
			if (!CheckStatusFail(defender_status, STATUS_DEATH))
				break;
			if (!CheckStatusFail(attacker_status, STATUS_DEATH | STATUS_PETRIFY, true))
				break;
			special_effect_description = wxString::Format(wxT(STRING_SACRIFICE), defender_name, attacker_name);
			break;
		case 46: // Soul Blade
			if (attacker_weapon_index < 7 || attacker_weapon_index > 15) { // Not a Thief Sword slot
				fail_reason = wxString::Format(wxT(STRING_FAIL_SOUL_BLADE), attacker_name, 7, 15);
				break;
			}
			if ((attacker_weapon_status & STATUS_DEATH) == 0 || CheckStatusFail(defender_status, STATUS_EASY_KILL))
				ApplyStatus(attacker_weapon_status, false);
			break;
		case 48: // Spear
		case 83: // Trance Spear
		{
			formulae += _(STRING_FORMULA_JUMP_TIME);
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), spell_effect == 48 ? _(STRING_FORMULA_SPEAR) : _(STRING_FORMULA_SPEAR_TRANCE));
			int durationticks = 40 * (60 - attacker_spirit);
			use_caster_spirit = true;
			special_effect_description = wxString::Format(wxT(STRING_JUMP), durationticks, (float)durationticks / (8 * 15.0f), (float)durationticks / (10 * 15.0f), (float)durationticks / (14 * 15.0f));
			vector<int> highjumpnum;
			alternate_damage_description = _(STRING_HIGH_JUMP);
			SetupAttackCommand(spell_effect == 48 ? 1 : 5, false);
			for (i = 0; i < i_at_num.size(); i++)
				highjumpnum.push_back(2 * i_at_num[i]);
			transform(i_at_num.begin(), i_at_num.end(), i_at_num.begin(), increase_damage_half);
			ApplyCasterHalfMini();
			ApplyCasterHalfMini(&highjumpnum);
			ApplyDefendProtectSleepMini();
			ApplyDefendProtectSleepMini(&highjumpnum);
			if (spell_effect == 48) { // Spear
				ComputeDamage();
				for (i = 0; i < highjumpnum.size(); i++)
					for (j = 0; j < i_df_pow.size(); j++)
						alternate_damage.push_back(min(9999, max(1, i_at_pow - i_df_pow[j]) * max(1, highjumpnum[i])));
				if ((attacker_support & SUPPORT_ABILITY_HEALER) != 0) {
					use_caster_support |= SUPPORT_ABILITY_HEALER;
					if ((defender_status & STATUS_ZOMBIE) == 0 && (defender_class & ENEMY_CLASSIFICATION_UNDEAD) == 0)
						has_heal = true;
					else
						use_target_zombie = true;
				}
			} else { // Trance Spear
				has_damage = true;
				for (i = 0; i < i_at_num.size(); i++)
					for (j = 0; j < i_df_pow.size(); j++)
						damage.push_back(max(1, i_at_pow - i_df_pow[j]) * max(4, i_at_num[i]));
				for (i = 0; i < highjumpnum.size(); i++)
					for (j = 0; j < i_df_pow.size(); j++)
						alternate_damage.push_back(max(1, i_at_pow - i_df_pow[j]) * max(4, highjumpnum[i]));
				addendum_end += _(STRING_SPEAR_TRANCE);
			}
			break;
		}
		case 49: // Eidolon Phoenix
		{
			if (defender_is_player) {
				spell_effect = 13; // Life
				spell_index = 73; // Rebirth Flame spell slot
				spell_power = cddata->spellset.spell[73].power;
				spell_accuracy = cddata->spellset.spell[73].accuracy;
				spell_element = cddata->spellset.spell[73].element;
				spell_status = cddata->spellset.spell[73].status;
			} else {
				spell_effect = 9; // Magic Attack
				spell_index = 72; // Phoenix spell slot
				spell_power = cddata->spellset.spell[72].power;
				spell_accuracy = cddata->spellset.spell[72].accuracy;
				spell_element = cddata->spellset.spell[72].element;
				spell_status = cddata->spellset.spell[72].status;
			}
			ProcessMain();
			return;
		}
		case 50: // Six Dragon
			if (!CheckStatusFail(defender_status, STATUS_DEATH | STATUS_PETRIFY))
				break;
			special_effect_description = wxString::Format(wxT(STRING_SIX_DRAGONS), defender_name);
			break;
		case 51: // Curse
			if ((spell_command == 28 || spell_command == 29) && (spell_index == 129 || spell_index == 137)) {
				special_effect_description = wxString::Format(wxT(STRING_CURSE_RND), defender_name);
			} else {
				wxArrayString elemlist;
				for (i = 0; i < 8; i++)
					if (((1 << i) & spell_element) != 0)
						elemlist.Add(HADES_STRING_ELEMENT_NAME[i]);
				special_effect_description = wxString::Format(wxT(STRING_CURSE), defender_name, CombineMultipleStrings(elemlist));
			}
			break;
		case 52: // Angel's Snack
			if (cddata == NULL)
				break;
			special_effect_description = wxString::Format(wxT(STRING_ANGEL_SNACK), cddata->itemset.item[spell_power].name.str_nice, defender_name);
			break;
		case 53: // Lucky Seven
			special_effect_description = wxString::Format(wxT(STRING_LUCKY_SEVEN), attacker_name);
			break;
		case 54: // What's That!?
			special_effect_description = wxString::Format(wxT(STRING_WHATS_THAT), defender_name, attacker_name);
			if (defender_is_player) {
				special_effect_description += wxString::Format(wxT(STRING_WHATS_THAT_ROW), defender_name);
			} else if ((battle_flag & BATTLE_FLAG_SCRIPTED_END) != 0) {
				fail_reason = _(STRING_FAIL_WHATS_THAT);
				break;
			}
			break;
		case 55: // Change
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_ROW), defender_name);
			break;
		case 56: // Flee
		{
			if ((battle_flag & BATTLE_FLAG_NO_FLEE) != 0) {
				fail_reason = _(STRING_FAIL_FLEE);
				break;
			}
			formulae += _(STRING_FORMULA_FLEE);
			int chancepercheck = min(100, 200 / defender_level * attacker_level / 16);
			double normalized = chancepercheck / 100.0;
			double mean = chancepercheck == 0 ? 9999.99 : 1.0 / normalized;
			double median = chancepercheck == 0 ? 9999.99 :
				(chancepercheck == 100 ? 0.0 :
				ceil(-log(2) / log(1.0 - normalized)));
			special_effect_description = wxString::Format(wxT(STRING_FLEE), chancepercheck, defender_level, attacker_level);
			use_caster_level = true;
			use_target_level = true;
			addendum_end += wxString::Format(wxT(STRING_FLEE_STATS), mean, median);
			break;
		}
		case 57: // Flee (Skill)
			if ((battle_flag & BATTLE_FLAG_NO_FLEE) != 0) {
				fail_reason = _(STRING_FAIL_FLEE);
				break;
			}
			special_effect_description = _(STRING_FLEE_SKILL);
			break;
		case 58: // Steal
		{
			if (defender_is_player) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_PLAYER_BAD), defender_name);
				break;
			}
			formulae += _(STRING_FORMULA_STEAL);
			special_effect_description = _(STRING_STEAL);
			double baserate, remainrate = 1.0;
			double itemrate[4];
			use_caster_level = true;
			use_caster_spirit = true;
			use_target_level = true;
			i_hit_rate = attacker_level + attacker_spirit;
			i_ev = defender_level;
			if (i_hit_rate <= i_ev)
				baserate = (1.0 + i_hit_rate) / 2.0 / i_ev;
			else
				baserate = 1.0 - i_ev / 2.0 / i_hit_rate + 0.5 / i_hit_rate;
			if ((attacker_support & SUPPORT_ABILITY_BANDIT) != 0) {
				use_caster_support |= SUPPORT_ABILITY_BANDIT;
				baserate = 1.0;
			}
			if ((attacker_support & SUPPORT_ABILITY_MASTER_THIEF) != 0) {
				use_caster_support |= SUPPORT_ABILITY_MASTER_THIEF;
				if (defender_has_item_slot_4) {
					itemrate[3] = 0.125 * baserate;
					remainrate *= 0.875;
				}
				if (defender_has_item_slot_3) {
					itemrate[2] = 0.125 * remainrate * baserate;
					remainrate *= 0.875;
				}
				if (defender_has_item_slot_2) {
					itemrate[1] = 0.25 * remainrate * baserate;
					remainrate *= 0.75;
				}
				if (defender_has_item_slot_1)
					itemrate[0] = remainrate * baserate;
			} else {
				if (defender_has_item_slot_4)
					itemrate[3] = 0.00390625 * baserate;
				remainrate *= 0.99609375;
				if (defender_has_item_slot_3)
					itemrate[2] = 0.0625 * remainrate * baserate;
				remainrate *= 0.9375;
				if (defender_has_item_slot_2)
					itemrate[1] = 0.25 * remainrate * baserate;
				remainrate *= 0.75;
				if (defender_has_item_slot_1)
					itemrate[0] = remainrate * baserate;
			}
			if (defender_has_item_slot_1) addendum_end += wxString::Format(wxT(STRING_STEAL_ITEM), L"1st", 100.0 * itemrate[0]);
			if (defender_has_item_slot_2) addendum_end += wxString::Format(wxT(STRING_STEAL_ITEM), L"2nd", 100.0 * itemrate[1]);
			if (defender_has_item_slot_3) addendum_end += wxString::Format(wxT(STRING_STEAL_ITEM), L"3rd", 100.0 * itemrate[2]);
			if (defender_has_item_slot_4) addendum_end += wxString::Format(wxT(STRING_STEAL_ITEM), L"4th", 100.0 * itemrate[3]);
			if (!defender_has_item_slot_1 && !defender_has_item_slot_2 && !defender_has_item_slot_3 && !defender_has_item_slot_4) addendum_end += _(STRING_STEAL_NO_ITEM);
			addendum_end += wxString::Format(wxT(STRING_STEAL_MUG), 0, attacker_level * defender_level / 2 - 1);
			addendum_end += wxString::Format(wxT(STRING_STEAL_GIL), 0, attacker_level * defender_level / 4);
			break;
		}
		case 59: // Scan
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL))
				break;
			use_target_level = true;
			use_target_max_hp = true;
			special_effect_description = wxString::Format(wxT(STRING_SCAN), defender_name);
			break;
		case 60: // Detect
			special_effect_description = wxString::Format(wxT(STRING_DETECT), defender_name);
			break;
		case 61: // Charge!
			special_effect_description = _(STRING_CHARGE);
			return;
		case 62: // Item: Soft
			if ((defender_class & ENEMY_CLASSIFICATION_STONE) != 0) {
				if (!CheckStatusFail(defender_status, STATUS_DEATH | STATUS_PETRIFY))
					break;
				special_effect_description = wxString::Format(wxT(STRING_ENFORCE_KILL), defender_name);
			} else {
				addendum_effect += _(STRING_ITEM);
			}
			break;
		case 63: // Sword Magic
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SWORD_MAGIC));
			use_caster_weapon = true;
			SetupAttack(8, true, true);
			i_at_pow = attacker_weapon_power + spell_power;
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			ComputeDamage();
			break;
		case 65: // Eat
			if (spell_power == 0) {
				fail_reason = _(STRING_FAIL_ZERO_DIVISION);
				break;
			}
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL | STATUS_DEATH | STATUS_PETRIFY))
				break;
			if ((defender_class & ENEMY_CLASSIFICATION_HUMAN) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_NO_EAT_HUMAN), defender_name);
				break;
			}
			use_target_max_hp = true;
			special_effect_description = wxString::Format(wxT(STRING_EAT), defender_name, defender_max_hp / spell_power);
			break;
		case 66: // Frog Drop
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_FROG_DROP));
			vector<int> frog{ 0, 10, 20, 50, 100, 200 };
			has_special_damage = true;
			use_caster_level = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			for (i = 0; i < frog.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_FROG_DROP_DAMAGE), frog[i]));
				special_damage.push_back(max(1, min(9999, frog[i] * attacker_level)));
			}
			break;
		}
		case 67: // Thievery
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_THIEVERY));
			vector<int> steal{ 0, 10, 50, 100, 500, 1000 };
			has_special_damage = true;
			use_caster_speed = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			for (i = 0; i < steal.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_THIEVERY_DAMAGE), steal[i]));
				special_damage.push_back(min(9999, steal[i] * attacker_speed / 2));
			}
			break;
		}
		case 68: // Dragon's Crest
		{
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_DRAGON_CREST));
			vector<int> dragon{ 0, 10, 20, 30, 40, 50, 75, 100 };
			has_special_damage = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			for (i = 0; i < dragon.size(); i++) {
				special_damage_description.Add(wxString::Format(wxT(STRING_DRAGON_CREST_DAMAGE), dragon[i]));
				special_damage.push_back(min(9999, dragon[i] * dragon[i]));
			}
			break;
		}
		case 69: // Item: Heal
		case 70: // Item: MP Heal
		case 73: // Item: Cure Status
		case 74: // Item: Ore Heal
		case 75: // Item: Pepper Damage
		case 76: // Item: Tent
			addendum_effect += _(STRING_ITEM);
			break;
		case 71: // Item: Full Heal
		case 77: // Item: Dark Matter
			if (!CheckStatusFail(defender_status, STATUS_DEATH | STATUS_PETRIFY))
				break;
			addendum_effect += _(STRING_ITEM);
			break;
		case 72: // Item: Revive
			if (!CheckStatusFail(defender_status, STATUS_PETRIFY))
				break;
			if ((defender_status & STATUS_DEATH) != 0 && (defender_status & STATUS_ZOMBIE) != 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_DEAD_ZOMBIE), defender_name);
				break;
			}
			addendum_effect += _(STRING_ITEM);
			break;
		case 84: // Jewel
			SetupAccuracyMagic();
			ApplyShellAccuracy();
			ComputeHitRate(hit_rate);
			special_effect_description = _(STRING_ORE);
			break;
		case 85: // Eidolon Common
			SetupAttack(8, false, false);
			ApplyCasterHalfMini();
			ApplyShell();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			switch (spell_index) {
			case 49: // Shiva
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 233; // Opal
				i_at_pow += jewel_count;
				break;
			case 51: // Ifrit
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 234; // Topaz
				i_at_pow += jewel_count;
				break;
			case 53: // Ramuh
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 231; // Peridot
				i_at_pow += jewel_count;
				break;
			case 60: // Leviathan
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 226; // Aquamarine
				i_at_pow += jewel_count;
				break;
			case 62: // Bahamut
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 224; // Garnet
				i_at_pow += jewel_count;
				break;
			case 64: // Ark
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 235; // Lapis Lazuli
				i_at_pow += jewel_count;
				break;
			case 66: // Fenrir (Terrestrial Rage)
			case 67: // Fenrir (Millennial Decay)
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_JEWEL));
				use_jewel_id = 232; // Sapphire
				i_at_pow += jewel_count;
				break;
			case 74: // Madeen
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_LEVEL));
				use_caster_level = true;
				i_at_pow += attacker_level;
				break;
			default:
				formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_MAGIC8));
				break;
			}
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			break;
		case 86: // Eidolon Atomos
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL))
				break;
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_GRAV));
			i_at_num.push_back(spell_power);
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			if (!ApplyEarthMiss())
				break;
			use_jewel_id = 225; // Amethyst
			i_at_num[0] += jewel_count;
			ComputeDamageGravity(true);
			break;
		case 87: // Eidolon Odin
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_SUMMON_ODIN));
			formulae += wxString::Format(wxT(STRING_FORMULA_STATUS_RATE), _(STRING_FORMULA_RATE_ODIN));
			if ((defender_status & STATUS_EASY_KILL) != 0)
				addendum_effect += wxString::Format(wxT(STRING_ODIN_STATUS_FAIL), defender_name);
			SetupAccuracyMagic(true, false);
			use_jewel_id = 254; // Ore
			i_hit_rate += jewel_count / 2;
			ComputeHitRate(status_rate);
			ApplyStatus(spell_status, true);
			use_jewel_id = 254; // Ore
			SetupAttack(8, false, false);
			i_at_pow = spell_power + 100 - jewel_count;
			ComputeDamage();
			addendum_end += _(STRING_ODIN_SWORD);
			break;
		case 88: // Self-Destruct
			special_effect_description = wxString::Format(wxT(STRING_SELF_DESTRUCT), defender_name);
			break;
		case 89: // HP Switching
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL | STATUS_DEATH | STATUS_PETRIFY))
				break;
			special_effect_description = wxString::Format(wxT(STRING_HP_SWITCHING), attacker_name, defender_name);
			break;
		case 90: // Defless
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"defence", defender_defence / 2);
			special_effect_description += _(L" and ");
			special_effect_description += wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"magic defence", defender_magic_defence / 2);
			break;
		case 92: // Add Item
			special_effect_description = wxString::Format(wxT(STRING_ADD_ITEM), cddata->itemset.item[spell_power].name.str_nice);
			break;
		case 93: // Maelstrom
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL | STATUS_DEATH | STATUS_PETRIFY))
				break;
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT_RND), defender_name, L"HP", 1, 9);
			ApplyStatus(spell_status, false);
			break;
		case 94: // Absorb Magic
			use_caster_magic = true;
			use_target_magic = true;
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"magic", defender_magic / 2);
			special_effect_description += _(L" and ");
			special_effect_description += wxString::Format(wxT(STRING_CHANGE_STAT), attacker_name, L"magic", min(255, 2 * attacker_magic));
			break;
		case 95: // Absorb Strength
			use_caster_strength = true;
			use_target_strength = true;
			special_effect_description = wxString::Format(wxT(STRING_CHANGE_STAT), defender_name, L"strength", defender_strength / 2);
			special_effect_description += _(L" and ");
			special_effect_description += wxString::Format(wxT(STRING_CHANGE_STAT), attacker_name, L"strength", min(255, 2 * attacker_strength));
			break;
		case 96: // Trance
			ApplyStatus(STATUS_TRANCE, false);
			break;
		case 97: // Entice
			if (!defender_is_player) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_ENEMY_BAD), defender_name);
				break;
			}
			if (defender_player_slot == 2 || defender_player_slot == 4 || defender_player_slot == 6 || defender_player_slot == 11) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_FEMALE), defender_name);
				break;
			}
			ApplyStatus(spell_status, true);
			break;
		case 98: // Terran Strike
			if (!defender_is_player) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_ENEMY_BAD), defender_name);
				break;
			}
			if (defender_player_slot == 0) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_TERRAN), defender_name);
				break;
			}
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC4));
			SetupAttack(4, true, true);
			ComputeDamage();
			break;
		case 99: // Flare Star
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_FLARE_STAR));
			SetupAccuracyMagic();
			ApplyShellAccuracy();
			use_multi_target_rate = true;
			ComputeHitRate(hit_rate);
			has_damage = true;
			use_target_level = true;
			damage.push_back(defender_level * spell_power);
			break;
		case 100: // Enemy Accurate Attack
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC4));
			SetupAttack(4, true, true);
			ApplyBerserkTranceMini();
			ApplyGambleDefence();
			ApplyDefendProtectSleepMini();
			ApplyBackAttackBackRow();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			break;
		case 101: // Inventory Steal
			SetupAccuracyPhysic();
			ComputeHitRate(hit_rate);
			special_effect_description = wxString::Format(wxT(STRING_INVENTORY_STEAL), cddata->itemset.item[spell_accuracy].name.str_nice);
			break;
		case 102: // Inventory Mug
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC4));
			SetupAttack(4, true, true);
			ApplyBerserkTranceMini();
			ApplyGambleDefence();
			ApplyDefendProtectSleepMini();
			ApplyBackAttackBackRow();
			ComputeDamage();
			special_effect_description = wxString::Format(wxT(STRING_INVENTORY_MUG), cddata->itemset.item[spell_accuracy].name.str_nice);
			break;
		case 103: // Good Status
			ApplyStatus(spell_status, true);
			break;
		case 104: // Grudge
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_GRUDGE));
			has_special_damage = true;
			special_effect_description = _(STRING_DAMAGE_SPECIAL);
			addendum_effect += _(STRING_GRUDGE_NO_TONBERRY);
			for (i = 0; i <= 6; i++) {
				special_damage_description.Add(wxString::Format(i <= 1 ? wxT(STRING_GRUDGE_DAMAGE) : wxT(STRING_GRUDGE_DAMAGE_PLURAL), i));
				special_damage.push_back(1 << (2 * i));
			}
			special_damage_description.Add(wxString::Format(wxT(STRING_GRUDGE_DAMAGE), L"7 and more"));
			special_damage.push_back(9999);
			break;
		case 105: // Grand Cross
			if (!CheckStatusFail(defender_status, STATUS_DEATH | STATUS_PETRIFY))
				break;
			special_effect_description = _(STRING_GRAND_CROSS);
			break;
		case 106: // Banish
			if (!defender_is_player) {
				fail_reason = wxString::Format(wxT(STRING_FAIL_ENEMY_BAD), defender_name);
				break;
			}
			special_effect_description = wxString::Format(wxT(STRING_BANISH), defender_name);
			break;
		case 107: // Beat Back
			formulae += wxString::Format(wxT(STRING_FORMULA_DAMAGE), _(STRING_FORMULA_PHYSIC4));
			SetupAttack(4, true, true);
			ApplyBerserkTranceMini();
			ApplyGambleDefence();
			ApplyDefendProtectSleepMini();
			ApplyBackAttackBackRow();
			ApplyElementBoost(spell_element);
			if (!ApplyElementAffinity(spell_element, true))
				break;
			ComputeDamage();
			status_rate = spell_accuracy / 100.0;
			ApplyStatus(spell_status, false);
			if (defender_is_player)
				addendum_effect += wxString::Format(wxT(STRING_BEAT_BACK_PLAYER), defender_name);
			else
				addendum_effect += wxString::Format(wxT(STRING_BEAT_BACK_ENEMY), defender_name);
			break;
		case 108: // Iai Strike
			if (!CheckStatusFail(defender_status, STATUS_EASY_KILL))
				break;
			SetupAccuracyMagic(true);
			ApplyShellAccuracy();
			use_multi_target_rate = true;
			ComputeHitRate(status_rate);
			ApplyStatus(spell_status, true);
			break;
		case 109: // Mini
			if (CheckStatus(defender_status, STATUS_MINI, use_target_status_inc_rate)) {
				ApplyStatus(spell_status, true);
				break;
			}
			SetupAccuracyMagic(true);
			ApplyShellAccuracy();
			use_multi_target_rate = true;
			ComputeHitRate(status_rate);
			ApplyStatus(spell_status, true);
			break;
		}
		if (hit_rate >= 0.0) hit_rate = min(1.0, hit_rate);
		if (status_rate >= 0.0) status_rate = min(1.0, status_rate);
		if (critical_rate >= 0.0) critical_rate = min(1.0, critical_rate);
		if (multi_target_rate >= 0.0) multi_target_rate = min(1.0, multi_target_rate);
		if ((use_multi_target_damage || use_multi_target_rate) && (spell_index < 0 || (spell_index >= 0 && cddata->spellset.spell[spell_index].GetTargetAmount() != SPELL_TARGET_AMOUNT_VARIABLE))) {
			use_multi_target_damage = false;
			use_multi_target_rate = false;
		}
		if (reflect_count > 0 && (has_damage || has_heal || has_status || has_special_damage)) {
			bool reflecthaseffect = false;
			if (has_damage || has_heal || has_special_damage) {
				reflecthaseffect = true;
				int reflectfactor = reflect_count;
				if ((attacker_support & SUPPORT_ABILITY_REFLECTx2) != 0) {
					use_caster_support |= SUPPORT_ABILITY_REFLECTx2;
					reflectfactor *= 2;
				}
				if (has_damage || has_heal) {
					if (use_multi_target_damage && reflect_count > 1) {
						damage.clear();
						for (i = 0; i < multi_target_damage.size(); i++)
							damage.push_back(min(9999, reflectfactor * multi_target_damage[i]));
					} else {
						transform(damage.begin(), damage.end(), damage.begin(), [reflectfactor](int num) { return min(9999, reflectfactor * num); });
					}
				}
				if (has_special_damage) {
					transform(special_damage.begin(), special_damage.end(), special_damage.begin(), [reflectfactor](int num) { return min(9999, reflectfactor * num); });
				}
			}
			if (has_status && use_multi_target_rate && reflect_count > 1) {
				reflecthaseffect = true;
				if (hit_rate >= 0.0)
					hit_rate = multi_target_rate;
				else if (status_rate >= 0.0)
					status_rate = multi_target_rate;
				else
					reflecthaseffect = false;
			}
			if (reflecthaseffect) {
				addendum_effect += wxString::Format(wxT(STRING_REFLECT), defender_name, reflect_count);
				use_multi_target_damage = false;
				use_multi_target_rate = false;
			}
		}
	}
};

// Interface

ToolDamageCalculator::ToolDamageCalculator(wxWindow* p) : DamageCalculatorWindow(p) {
	player_panel[0] = new ToolDamageCalculatorPlayer(m_panelleft);
	player_panel[0]->parent = this;
	player_panel[1] = new ToolDamageCalculatorPlayer(m_panelright);
	player_panel[1]->parent = this;
	enemy_panel[0] = new ToolDamageCalculatorEnemy(m_panelleft);
	enemy_panel[0]->parent = this;
	enemy_panel[1] = new ToolDamageCalculatorEnemy(m_panelright);
	enemy_panel[1]->parent = this;
	m_panelleft->AddPage(player_panel[0], _(STRING_PANEL_CHARACTER), true);
	m_panelleft->AddPage(enemy_panel[0], _(STRING_PANEL_ENEMY));
	m_panelright->AddPage(player_panel[1], _(STRING_PANEL_CHARACTER));
	m_panelright->AddPage(enemy_panel[1], _(STRING_PANEL_ENEMY), true);
}

int ToolDamageCalculator::ShowModal(CDDataStruct* data) {
	cddata = data;
	unsigned int i, j, k, nb, *battleid;
	EnemyStatDataStruct** stats;
	set<pair<int, int>> listed;
	vector<EnemyStatDataStruct*> sample;
	for (i = 0; i < cddata->enemyset.battle_amount; i++) {
		for (j = 0; j < cddata->enemyset.battle[i]->stat_amount; j++) {
			if (listed.find({ i, j }) != listed.end())
				continue;
			EnemyStatDataStruct& statcurrent = cddata->enemyset.battle[i]->stat[j];
			sample.push_back(&statcurrent);
			stats = cddata->enemyset.GetSimilarEnemyStats(statcurrent, &nb, &battleid);
			for (k = 0; k < nb; k++)
				listed.insert({ battleid[k], stats[k]->id });
		}
	}
	sort(sample.begin(), sample.end(), [](EnemyStatDataStruct* first, EnemyStatDataStruct* second) { return first->name.str_nice.compare(second->name.str_nice) < 0; });
	for (i = 0; i < sample.size(); i++) {
		enemy_list_battle.push_back(sample[i]->parent->id);
		enemy_list_stat.push_back(sample[i]->id);
	}
	player_panel[0]->Init(cddata, true);
	player_panel[1]->Init(cddata, false);
	enemy_panel[0]->Init(cddata);
	enemy_panel[1]->Init(cddata);
	player_panel[0]->SelectBaseCharacter(0);
	player_panel[1]->SelectBaseCharacter(0);
	enemy_panel[0]->SelectBaseEnemy(0);
	enemy_panel[1]->SelectBaseEnemy(0);
	player_panel[1]->EnableSpellOptions(false);
	enemy_panel[1]->EnableSpellOptions(false);
	initialized = true;
	UpdateInformation();
	return DamageCalculatorWindow::ShowModal();
}

void ToolDamageCalculator::UpdateInformation() {
	if (!initialized)
		return;
	DamageCalculation calc;
	wxString description = _(L"");
	int i, j;
	// Initialize inputs
	calc.cddata = cddata;
	calc.attacker_is_player = m_panelleft->GetSelection() == 0;
	calc.defender_is_player = m_panelright->GetSelection() == 0;
	if (calc.attacker_is_player) {
		calc.attacker_player_slot = player_panel[0]->m_basechar->GetSelection();
		calc.attacker_name = player_panel[0]->m_basechar->GetString(calc.attacker_player_slot);
		calc.attacker_weapon_index = player_panel[0]->m_weapon->GetSelection();
		if (calc.attacker_weapon_index >= 0 && calc.attacker_weapon_index < ITEM_WEAPON_AMOUNT) {
			calc.attacker_weapon_element = cddata->itemset.weapon[calc.attacker_weapon_index].element;
			calc.attacker_weapon_status = cddata->spellset.status_set[cddata->itemset.weapon[calc.attacker_weapon_index].status].status;
		}
		calc.attacker_weapon_power = max(0, min(255, player_panel[0]->equip.attack + player_panel[0]->stat->attack));
		calc.attacker_level = player_panel[0]->m_level->GetValue();
		calc.attacker_max_hp = max(0, min(9999, player_panel[0]->base_stat_max_hp + player_panel[0]->stat->max_hp));
		calc.attacker_support = player_panel[0]->stat->supporting_ability_on;
		if ((calc.attacker_support & SUPPORT_ABILITY_HP10) != 0) calc.attacker_max_hp = min(9999, calc.attacker_max_hp + calc.attacker_max_hp / 10);
		if ((calc.attacker_support & SUPPORT_ABILITY_HP20) != 0) calc.attacker_max_hp = min(9999, calc.attacker_max_hp + calc.attacker_max_hp / 5);
		calc.attacker_speed = max(0, min(50, player_panel[0]->base_stat_speed + player_panel[0]->equip.speed + player_panel[0]->stat->speed));
		calc.attacker_strength = max(0, min(99, player_panel[0]->base_stat_strength + player_panel[0]->equip.strength + player_panel[0]->stat->strength));
		calc.attacker_magic = max(0, min(99, player_panel[0]->base_stat_magic + player_panel[0]->equip.magic + player_panel[0]->stat->magic));
		calc.attacker_spirit = max(0, min(50, player_panel[0]->base_stat_spirit + player_panel[0]->equip.spirit + player_panel[0]->stat->spirit));
		calc.attacker_element_boost = player_panel[0]->equip.element_boost | player_panel[0]->stat->buff.element_boost;
		calc.attacker_row = player_panel[0]->stat->buff.back_row ? 0 : 1;
		calc.attacker_status = player_panel[0]->stat->buff.status;
		calc.is_back_attack = player_panel[0]->stat->buff.back_attack;
		calc.is_ipsen_curse = player_panel[0]->stat->buff.ipsen_curse;
		calc.jewel_count = player_panel[0]->stat->jewel_count;
		calc.spell_index = player_panel[0]->m_basespell->GetSelection();
		calc.spell_name = player_panel[0]->m_basespell->GetString(calc.spell_index);
		calc.spell_model = cddata->spellset.spell[calc.spell_index].model;
		calc.spell_effect = ((SortedChoiceItemWithHelp*)(player_panel[0]->m_effect->GetClientData(player_panel[0]->m_effect->GetSelection())))->id;
		calc.spell_power = player_panel[0]->m_power->GetValue();
		calc.spell_accuracy = player_panel[0]->m_accuracy->GetValue();
		MACRO_CALCULATOR_GETELEMS_WIN(calc.spell_element, player_panel[0])
		calc.spell_status = cddata->spellset.status_set[player_panel[0]->m_status->GetSelection()].status;
		if (cddata->saveset.sectionloaded[DATA_SECTION_CMD]) {
			if (calc.spell_index == 67 || calc.spell_index == 69 || calc.spell_index == 70 || calc.spell_index == 71 || calc.spell_index == 73)
				calc.spell_command = 20;
			for (i = 0; i < cddata->cmdset.cmd.size() && calc.spell_command < 0; i++) {
				if (cddata->cmdset.cmd[i].panel == COMMAND_PANEL_NONE && cddata->cmdset.cmd[i].spell_list[0] == calc.spell_index)
					calc.spell_command = i;
				else if (cddata->cmdset.cmd[i].panel == COMMAND_PANEL_SPELL)
					for (j = 0; j < cddata->cmdset.cmd[i].spell_amount; j++)
						if (cddata->cmdset.cmd[i].spell_list[j] == calc.spell_index) {
							calc.spell_command = i;
							break;
						}
			}
		}
		calc.reflect_count = (cddata->spellset.spell[calc.spell_index].flag & SPELL_FLAG_REFLECTABLE) != 0 ? player_panel[0]->stat->buff.reflect_count : 0;
	} else {
		calc.attacker_name = enemy_panel[0]->m_baseenemy->GetString(enemy_panel[0]->m_baseenemy->GetSelection());
		calc.attacker_level = enemy_panel[0]->m_level->GetValue();
		calc.attacker_max_hp = enemy_panel[0]->m_maxhp->GetValue();
		calc.attacker_speed = enemy_panel[0]->m_speed->GetValue();
		calc.attacker_strength = enemy_panel[0]->m_strength->GetValue();
		calc.attacker_magic = enemy_panel[0]->m_magic->GetValue();
		calc.attacker_spirit = enemy_panel[0]->m_spirit->GetValue();
		calc.attacker_element_boost = enemy_panel[0]->buff.element_boost;
		calc.attacker_row = 2;
		calc.attacker_status = enemy_panel[0]->buff.status;
		calc.is_back_attack = enemy_panel[0]->buff.back_attack;
		calc.is_ipsen_curse = enemy_panel[0]->buff.ipsen_curse;
		calc.spell_name = enemy_panel[0]->m_basespell->GetString(enemy_panel[0]->m_basespell->GetSelection());
		calc.spell_model = cddata->enemyset.battle[enemy_list_battle[enemy_panel[0]->m_baseenemy->GetSelection()]]->spell[enemy_panel[0]->m_basespell->GetSelection()].model;
		calc.spell_effect = ((SortedChoiceItemWithHelp*)(enemy_panel[0]->m_effect->GetClientData(enemy_panel[0]->m_effect->GetSelection())))->id;
		calc.spell_power = enemy_panel[0]->m_power->GetValue();
		calc.spell_accuracy = enemy_panel[0]->m_accuracy->GetValue();
		MACRO_CALCULATOR_GETELEMS_WIN(calc.spell_element, enemy_panel[0])
		calc.spell_status = cddata->spellset.status_set[enemy_panel[0]->m_status->GetSelection()].status;
		calc.reflect_count = (cddata->enemyset.battle[enemy_list_battle[enemy_panel[0]->m_baseenemy->GetSelection()]]->spell[enemy_panel[0]->m_basespell->GetSelection()].flag & SPELL_FLAG_REFLECTABLE) != 0 ? enemy_panel[0]->buff.reflect_count : 0;
	}
	if (calc.defender_is_player) {
		calc.defender_player_slot = player_panel[1]->m_basechar->GetSelection();
		calc.defender_name = player_panel[1]->m_basechar->GetString(calc.defender_player_slot);
		calc.defender_level = player_panel[1]->m_level->GetValue();
		calc.defender_max_hp = max(0, min(9999, player_panel[1]->base_stat_max_hp + player_panel[1]->stat->max_hp));
		calc.defender_support = player_panel[1]->stat->supporting_ability_on;
		if ((calc.defender_support & SUPPORT_ABILITY_HP10) != 0) calc.defender_max_hp = min(9999, calc.defender_max_hp + calc.defender_max_hp / 10);
		if ((calc.defender_support & SUPPORT_ABILITY_HP20) != 0) calc.defender_max_hp = min(9999, calc.defender_max_hp + calc.defender_max_hp / 5);
		calc.defender_speed = max(0, min(50, player_panel[1]->base_stat_speed + player_panel[1]->equip.speed + player_panel[1]->stat->speed));
		calc.defender_strength = max(0, min(99, player_panel[1]->base_stat_strength + player_panel[1]->equip.strength + player_panel[1]->stat->strength));
		calc.defender_magic = max(0, min(99, player_panel[1]->base_stat_magic + player_panel[1]->equip.magic + player_panel[1]->stat->magic));
		calc.defender_spirit = max(0, min(50, player_panel[1]->base_stat_spirit + player_panel[1]->equip.spirit + player_panel[1]->stat->spirit));
		calc.defender_defence = max(0, min(255, player_panel[1]->stat->defence + player_panel[1]->equip.defence));
		calc.defender_evade = max(0, min(255, player_panel[1]->stat->evade + player_panel[1]->equip.evade));
		calc.defender_magic_defence = max(0, min(255, player_panel[1]->stat->magic_defence + player_panel[1]->equip.magic_defence));
		calc.defender_magic_evade = max(0, min(255, player_panel[1]->stat->magic_evade + player_panel[1]->equip.magic_evade));
		calc.defender_element_absorb = player_panel[1]->equip.element_absorb | player_panel[1]->stat->buff.element_absorb;
		calc.defender_element_immune = player_panel[1]->equip.element_immune | player_panel[1]->stat->buff.element_immune;
		calc.defender_element_half = player_panel[1]->equip.element_half | player_panel[1]->stat->buff.element_half;
		calc.defender_element_weak = player_panel[1]->equip.element_absorb | player_panel[1]->stat->buff.element_weak;
		calc.defender_row = player_panel[1]->stat->buff.back_row ? 0 : 1;
		calc.defender_status = player_panel[1]->stat->buff.status;
	} else {
		int enemysel = enemy_panel[1]->m_baseenemy->GetSelection();
		EnemyDataStruct& btl = *cddata->enemyset.battle[enemy_list_battle[enemysel]];
		EnemyStatDataStruct& stat = btl.stat[enemy_list_stat[enemysel]];
		calc.battle_flag = btl.flag;
		calc.defender_name = enemy_panel[1]->m_baseenemy->GetString(enemysel);
		calc.defender_level = enemy_panel[1]->m_level->GetValue();
		calc.defender_max_hp = enemy_panel[1]->m_maxhp->GetValue();
		calc.defender_speed = enemy_panel[1]->m_speed->GetValue();
		calc.defender_strength = enemy_panel[1]->m_strength->GetValue();
		calc.defender_magic = enemy_panel[1]->m_magic->GetValue();
		calc.defender_spirit = enemy_panel[1]->m_spirit->GetValue();
		calc.defender_defence = enemy_panel[1]->m_defence->GetValue();
		calc.defender_evade = enemy_panel[1]->m_evade->GetValue();
		calc.defender_magic_defence = enemy_panel[1]->m_magicdefence->GetValue();
		calc.defender_magic_evade = enemy_panel[1]->m_magicevade->GetValue();
		calc.defender_element_absorb = enemy_panel[1]->buff.element_absorb;
		calc.defender_element_immune = enemy_panel[1]->buff.element_immune;
		calc.defender_element_half = enemy_panel[1]->buff.element_half;
		calc.defender_element_weak = enemy_panel[1]->buff.element_weak;
		calc.defender_has_item_slot_1 = stat.item_steal[0] != 255;
		calc.defender_has_item_slot_2 = stat.item_steal[1] != 255;
		calc.defender_has_item_slot_3 = stat.item_steal[2] != 255;
		calc.defender_has_item_slot_4 = stat.item_steal[3] != 255;
		MACRO_CALCULATOR_GETCLASS_WIN(calc.defender_class, enemy_panel[1])
		calc.defender_row = 2;
		calc.defender_status = enemy_panel[1]->buff.status;
	}
	if (calc.attacker_is_player && calc.defender_is_player && player_panel[0]->m_basechar->GetSelection() == player_panel[1]->m_basechar->GetSelection()) calc.is_self_attack = true;
	else if (!calc.attacker_is_player && !calc.defender_is_player && enemy_panel[0]->m_baseenemy->GetSelection() == enemy_panel[1]->m_baseenemy->GetSelection()) calc.is_self_attack = true;
	// Calculate
	calc.ProcessMain();
	if (calc.no_effect) {
		calc.use_caster_level = true;
		calc.use_caster_max_hp = true;
		calc.use_caster_speed = true;
		calc.use_caster_strength = true;
		calc.use_caster_magic = true;
		calc.use_caster_spirit = true;
		calc.use_caster_weapon = calc.attacker_is_player;
		calc.use_target_level = true;
		calc.use_target_max_hp = true;
		calc.use_target_speed = true;
		calc.use_target_strength = true;
		calc.use_target_magic = true;
		calc.use_target_spirit = true;
		calc.use_target_defence = true;
		calc.use_target_evade = true;
		calc.use_target_magic_defence = true;
		calc.use_target_magic_evade = true;
	}
	// Display attacker informations
	if (calc.no_effect)
		description += wxString::Format(wxT(STRING_CHARACTER), calc.attacker_name);
	else
		description += wxString::Format(wxT(STRING_ATTACKER), calc.attacker_name);
	if (calc.use_caster_level) description += wxString::Format(wxT(STRING_LEVEL), calc.attacker_level);
	if (calc.use_caster_max_hp) description += wxString::Format(wxT(STRING_MAX_HP), calc.attacker_max_hp);
	if (calc.use_caster_speed) {
		if (calc.attacker_is_player) {
			description += wxString::Format(wxT(STRING_SPEED_EX),
				calc.attacker_speed,
				player_panel[0]->computed_base_stat_speed,
				player_panel[0]->computed_level_stat_speed,
				player_panel[0]->computed_bonus_stat_speed,
				player_panel[0]->equip.speed,
				wxString::Format(player_panel[0]->stat->speed < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[0]->stat->speed)));
		} else {
			description += wxString::Format(wxT(STRING_SPEED), calc.attacker_speed);
		}
	}
	if (calc.use_caster_strength) {
		if (calc.attacker_is_player) {
			description += wxString::Format(wxT(STRING_STRENGTH_EX),
				calc.attacker_strength,
				player_panel[0]->computed_base_stat_strength,
				player_panel[0]->computed_level_stat_strength,
				player_panel[0]->computed_bonus_stat_strength,
				player_panel[0]->equip.strength,
				wxString::Format(player_panel[0]->stat->strength < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[0]->stat->strength)));
		} else {
			description += wxString::Format(wxT(STRING_STRENGTH), calc.attacker_strength);
		}
	}
	if (calc.use_caster_magic) {
		if (calc.attacker_is_player) {
			description += wxString::Format(wxT(STRING_MAGIC_EX),
				calc.attacker_magic,
				player_panel[0]->computed_base_stat_magic,
				player_panel[0]->computed_level_stat_magic,
				player_panel[0]->computed_bonus_stat_magic,
				player_panel[0]->equip.magic,
				wxString::Format(player_panel[0]->stat->magic < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[0]->stat->magic)));
		} else {
			description += wxString::Format(wxT(STRING_MAGIC), calc.attacker_magic);
		}
	}
	if (calc.use_caster_spirit) {
		if (calc.attacker_is_player) {
			description += wxString::Format(wxT(STRING_SPIRIT_EX),
				calc.attacker_spirit,
				player_panel[0]->computed_base_stat_spirit,
				player_panel[0]->computed_level_stat_spirit,
				player_panel[0]->computed_bonus_stat_spirit,
				player_panel[0]->equip.spirit,
				wxString::Format(player_panel[0]->stat->spirit < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[0]->stat->spirit)));
		} else {
			description += wxString::Format(wxT(STRING_SPIRIT), calc.attacker_spirit);
		}
	}
	if (!cddata->saveset.sectionloaded[DATA_SECTION_STAT] && calc.attacker_is_player && (calc.use_caster_speed || calc.use_caster_strength || calc.use_caster_magic || calc.use_caster_spirit)) description += _(STRING_LOAD_STAT_HINT);
	if (calc.use_caster_weapon) description += wxString::Format(wxT(STRING_WEAPON_ATTACK), calc.attacker_weapon_power);
	// Display defender informations
	description += _(L"\n");
	if (calc.no_effect)
		description += wxString::Format(wxT(STRING_CHARACTER), calc.defender_name);
	else
		description += wxString::Format(wxT(STRING_DEFENDER), calc.defender_name);
	if (calc.use_target_level) description += wxString::Format(wxT(STRING_LEVEL), calc.defender_level);
	if (calc.use_target_max_hp) description += wxString::Format(wxT(STRING_MAX_HP), calc.defender_max_hp);
	if (calc.use_target_speed) {
		if (calc.defender_is_player) {
			description += wxString::Format(wxT(STRING_SPEED_EX),
				calc.defender_speed,
				player_panel[1]->computed_base_stat_speed,
				player_panel[1]->computed_level_stat_speed,
				player_panel[1]->computed_bonus_stat_speed,
				player_panel[1]->equip.speed,
				wxString::Format(player_panel[1]->stat->speed < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[1]->stat->speed)));
		} else {
			description += wxString::Format(wxT(STRING_SPEED), calc.defender_speed);
		}
	}
	if (calc.use_target_strength) {
		if (calc.defender_is_player) {
			description += wxString::Format(wxT(STRING_STRENGTH_EX),
				calc.defender_strength,
				player_panel[1]->computed_base_stat_strength,
				player_panel[1]->computed_level_stat_strength,
				player_panel[1]->computed_bonus_stat_strength,
				player_panel[1]->equip.strength,
				wxString::Format(player_panel[1]->stat->strength < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[1]->stat->strength)));
		} else {
			description += wxString::Format(wxT(STRING_STRENGTH), calc.defender_strength);
		}
	}
	if (calc.use_target_magic) {
		if (calc.defender_is_player) {
			description += wxString::Format(wxT(STRING_MAGIC_EX),
				calc.defender_magic,
				player_panel[1]->computed_base_stat_magic,
				player_panel[1]->computed_level_stat_magic,
				player_panel[1]->computed_bonus_stat_magic,
				player_panel[1]->equip.magic,
				wxString::Format(player_panel[1]->stat->magic < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[1]->stat->magic)));
		} else {
			description += wxString::Format(wxT(STRING_MAGIC), calc.defender_magic);
		}
	}
	if (calc.use_target_spirit) {
		if (calc.defender_is_player) {
			description += wxString::Format(wxT(STRING_SPIRIT_EX),
				calc.defender_spirit,
				player_panel[1]->computed_base_stat_spirit,
				player_panel[1]->computed_level_stat_spirit,
				player_panel[1]->computed_bonus_stat_spirit,
				player_panel[1]->equip.spirit,
				wxString::Format(player_panel[1]->stat->spirit < 0 ? wxT(" - %d") : wxT(" + %d"), abs(player_panel[1]->stat->spirit)));
		} else {
			description += wxString::Format(wxT(STRING_SPIRIT), calc.defender_spirit);
		}
	}
	if (!cddata->saveset.sectionloaded[DATA_SECTION_STAT] && calc.defender_is_player && (calc.use_target_speed || calc.use_target_strength || calc.use_target_magic || calc.use_target_spirit)) description += _(STRING_LOAD_STAT_HINT);
	if (calc.use_target_defence) description += wxString::Format(wxT(STRING_DEFENCE), calc.defender_defence);
	if (calc.use_target_evade) description += wxString::Format(wxT(STRING_EVADE), calc.defender_evade);
	if (calc.use_target_magic_defence) description += wxString::Format(wxT(STRING_MAGIC_DEFENCE), calc.defender_magic_defence);
	if (calc.use_target_magic_evade) description += wxString::Format(wxT(STRING_MAGIC_EVADE), calc.defender_magic_evade);
	// Display effect informations
	if (!calc.no_effect) {
		description += _(L"\n");
		description += _(STRING_SEPARATOR);
		if (calc.attacker_is_player && calc.spell_index == SPELL_ATTACK_ID)
			description += wxString::Format(wxT(STRING_PHYSICAL_ATTACK), calc.attacker_name);
		else if (calc.attacker_is_player)
			description += wxString::Format(wxT(STRING_SPELL), calc.spell_name);
		else
			description += wxString::Format(wxT(STRING_ATTACK), calc.spell_name);
		if (!calc.fail_reason.IsEmpty()) {
			description += calc.fail_reason;
		} else {
			wxFormatString dmgraisefmt = calc.has_heal ? wxT(STRING_MODIF_HEAL_RAISE) : wxT(STRING_MODIF_DMG_RAISE);
			wxFormatString dmglowfmt = calc.has_heal ? wxT(STRING_MODIF_HEAL_LOW) : wxT(STRING_MODIF_DMG_LOW);
			int dmglow = calc.damage.size() > 0 ? calc.damage[0] : 0;
			int dmghigh = calc.damage.size() > 0 ? calc.damage[calc.damage.size() - 1] : 0;
			bool dmgrange = dmglow < dmghigh;
			bool statussaid = false;
			wxArrayString statusstrlist;
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.apply_status.find(i) != calc.apply_status.end()) statusstrlist.Add(HADES_STRING_STATUS_NAME[i]);
			if (!calc.special_effect_description.IsEmpty()) description += calc.special_effect_description;
			else if (calc.has_heal && dmgrange) description += wxString::Format(wxT(STRING_HEAL_RANGE), dmglow, dmghigh);
			else if (calc.has_heal) description += wxString::Format(wxT(STRING_HEAL_SINGLE), dmglow);
			else if (calc.has_damage && dmgrange) description += wxString::Format(wxT(STRING_DAMAGE_RANGE), dmglow, dmghigh);
			else if (calc.has_damage) description += wxString::Format(wxT(STRING_DAMAGE_SINGLE), dmglow);
			else if (calc.has_status && calc.cure_status) {
				description += wxString::Format(wxT(STRING_CURE_STATUS), CombineMultipleStrings(statusstrlist));
				statussaid = true;
			} else if (calc.has_status) {
				description += wxString::Format(wxT(STRING_INFLICT_STATUS), CombineMultipleStrings(statusstrlist));
				statussaid = true;
			} else description += _(STRING_UNKNOWN_EFFECT);
			if (calc.has_status && !statussaid && calc.status_rate < 0.0) {
				description += _(L" and ");
				if (calc.cure_status) description += wxString::Format(wxT(STRING_CURE_STATUS), CombineMultipleStrings(statusstrlist));
				else description += wxString::Format(wxT(STRING_INFLICT_STATUS), CombineMultipleStrings(statusstrlist));
				statussaid = true;
			}
			if (calc.hit_rate >= 0.0) {
				description += wxString::Format(wxT(STRING_RATE_PERCENT), 100.0 * calc.hit_rate);
				description += _(STRING_RATE_HIT);
			} else if (statussaid && calc.status_rate >= 0.0) {
				description += wxString::Format(wxT(STRING_RATE_PERCENT), 100.0 * calc.status_rate);
				description += _(STRING_RATE_HIT);
			} else if (calc.has_status && !statussaid) {
				description += wxString::Format(wxT(STRING_RATE_PERCENT), 100.0 * calc.status_rate);
				if (calc.cure_status) description += wxString::Format(wxT(STRING_RATE_CURE_STATUS), CombineMultipleStrings(statusstrlist));
				else description += wxString::Format(wxT(STRING_RATE_INFLICT_STATUS), CombineMultipleStrings(statusstrlist));
				statussaid = true;
			}
			if (calc.has_status && !statussaid) {
				description += calc.critical_rate >= 0 ? _(L", ") : _(L" and ");
				description += wxString::Format(wxT(STRING_RATE_PERCENT_MORE), 100.0 * calc.status_rate);
				if (calc.cure_status) description += wxString::Format(wxT(STRING_RATE_CURE_STATUS), CombineMultipleStrings(statusstrlist));
				else if (calc.weapon_status) description += wxString::Format(wxT(STRING_RATE_ADD_STATUS), CombineMultipleStrings(statusstrlist));
				else description += wxString::Format(wxT(STRING_RATE_INFLICT_STATUS), CombineMultipleStrings(statusstrlist));
				statussaid = true;
			}
			if (calc.critical_rate >= 0) {
				description += _(L" and ");
				description += wxString::Format(wxT(STRING_RATE_PERCENT_MORE), 100.0 * calc.critical_rate);
				description += _(STRING_RATE_CRITICAL);
			}
			description += _(L"\n");
			if (calc.use_multi_target_rate) description += wxString::Format(wxT(STRING_MULTI_TARGET_RATE), 100.0 * calc.multi_target_rate);
			description += calc.addendum_effect;
			description += _(L"\n");
			if (!calc.formulae.IsEmpty()) description += calc.formulae + _(L"\n");
			if ((calc.has_damage || calc.has_heal) && dmgrange) {
				wxFormatString dmgpossible = calc.has_heal ? wxT(STRING_HEAL_POSSIBLE) : wxT(STRING_DAMAGE_POSSIBLE);
				wxFormatString dmgmean = calc.has_heal ? wxT(STRING_HEAL_MEAN) : wxT(STRING_DAMAGE_MEAN);
				wxFormatString dmgmean1 = calc.has_heal ? wxT(STRING_HEAL_MEAN1) : wxT(STRING_DAMAGE_MEAN1);
				wxFormatString dmgmean2 = calc.has_heal ? wxT(STRING_HEAL_MEAN2) : wxT(STRING_DAMAGE_MEAN2);
				wxArrayString dmgaltstrlist;
				wxArrayString dmgstrlist;
				int totalsum = 0;
				int totalsumalt = 0;
				for (i = 0; i < calc.damage.size(); i++) {
					dmgstrlist.Add(wxString::Format(wxT("%d"), calc.damage[i]));
					totalsum += calc.damage[i];
				}
				for (i = 0; i < calc.alternate_damage.size(); i++) {
					dmgaltstrlist.Add(wxString::Format(wxT("%d"), calc.alternate_damage[i]));
					totalsumalt += calc.alternate_damage[i];
				}
				description += wxString::Format(dmgpossible, CombineMultipleStrings(dmgstrlist));
				if (calc.hit_rate >= 0.0) {
					description += wxString::Format(dmgmean1, (double)totalsum / (double)calc.damage.size());
					description += wxString::Format(dmgmean2, calc.hit_rate * totalsum / calc.damage.size());
					if (calc.critical_rate >= 0.0)
						description += wxString::Format(calc.has_heal ? wxT(STRING_HEAL_MEAN3) : wxT(STRING_DAMAGE_MEAN3), calc.hit_rate * (totalsum * (1.0 - calc.critical_rate) / calc.damage.size() + totalsumalt * calc.critical_rate / calc.alternate_damage.size()));
				} else {
					description += wxString::Format(dmgmean, (double)totalsum / (double)calc.damage.size());
				}
				description += _(L"\n");
				if (calc.alternate_damage.size() > 0) {
					description += wxString::Format(calc.has_heal ? wxT(STRING_ALTERNATE_HEAL) : wxT(STRING_ALTERNATE_DAMAGE), calc.alternate_damage_description) + _(L"\n");
					description += wxString::Format(dmgpossible, CombineMultipleStrings(dmgaltstrlist));
					if (calc.hit_rate >= 0.0) {
						description += wxString::Format(dmgmean, (double)totalsumalt / (double)calc.alternate_damage.size());
						description += wxString::Format(dmgmean2, calc.hit_rate * totalsumalt / calc.alternate_damage.size());
					} else {
						description += wxString::Format(dmgmean, (double)totalsumalt / (double)calc.alternate_damage.size());
					}
					description += _(L"\n");
				}
				if (calc.use_multi_target_damage) {
					wxArrayString dmgmultistrlist;
					int totalsummulti = 0;
					for (i = 0; i < calc.multi_target_damage.size(); i++) {
						dmgmultistrlist.Add(wxString::Format(wxT("%d"), calc.multi_target_damage[i]));
						totalsummulti += calc.multi_target_damage[i];
					}
					description += wxString::Format(calc.has_heal ? wxT(STRING_ALTERNATE_HEAL) : wxT(STRING_ALTERNATE_DAMAGE), _(STRING_MULTI_TARGET)) + _(L"\n");
					description += wxString::Format(dmgpossible, CombineMultipleStrings(dmgmultistrlist));
					if (calc.hit_rate >= 0.0) {
						description += wxString::Format(dmgmean1, (double)totalsummulti / (double)calc.multi_target_damage.size());
						description += wxString::Format(dmgmean2, calc.hit_rate * totalsummulti / calc.multi_target_damage.size());
					} else {
						description += wxString::Format(dmgmean, (double)totalsummulti / (double)calc.multi_target_damage.size());
					}
					description += _(L"\n");
				}
			} else if ((calc.has_damage || calc.has_heal) && calc.alternate_damage.size() > 0) {
				description += wxString::Format(calc.has_heal ? wxT(STRING_ALTERNATE_HEAL) : wxT(STRING_ALTERNATE_DAMAGE), calc.alternate_damage_description) + _(L" ") + wxString::Format(wxT("%d\n"), calc.alternate_damage[0]);
			} else if ((calc.has_damage || calc.has_heal) && calc.use_multi_target_damage) {
				description += wxString::Format(calc.has_heal ? wxT(STRING_ALTERNATE_HEAL) : wxT(STRING_ALTERNATE_DAMAGE), _(STRING_MULTI_TARGET)) + _(L" ") + wxString::Format(wxT("%d\n"), calc.multi_target_damage[0]);
			}
			if (calc.has_special_damage) {
				int sdcount = min(calc.special_damage.size(), calc.special_damage_description.GetCount());
				for (i = 0; i < sdcount; i++)
					description += calc.special_damage_description[i] + wxString::Format(wxT("%d\n"), calc.special_damage[i]);
			}
			int desclengthtmp = description.Len();
			if (calc.use_jewel_id >= 0) description += wxString::Format(wxT(STRING_USE_JEWEL), calc.jewel_count, cddata->itemset.item[calc.use_jewel_id].name.str_nice);
			if (calc.is_drain) description += _(STRING_DRAIN);
			if (calc.use_target_zombie) description += wxString::Format(wxT(STRING_ZOMBIE), calc.defender_name);
			if ((calc.use_caster_support & SUPPORT_ABILITY_HEALER) != 0) description += wxString::Format(wxT(STRING_MODIF_TRIGGER), L"Healer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_MAG_ELEM_NULL) != 0) description += wxString::Format(wxT(STRING_MODIF_TRIGGER), L"Magic Element Null");
			if ((calc.use_target_support & SUPPORT_ABILITY_GAMBLE_DEFENCE) != 0) description += wxString::Format(wxT(STRING_MODIF_TRIGGER), L"Gamble Defence");
			for (i = 0; i < 8; i++) if (calc.use_target_element_affinity.find(i) != calc.use_target_element_affinity.end() && (calc.defender_element_absorb & (1 << i)) != 0) description += wxString::Format(wxT(STRING_MODIF_ABSORB), HADES_STRING_ELEMENT_NAME[i]);
			for (i = 0; i < 8; i++) if (calc.use_caster_element_boost.find(i) != calc.use_caster_element_boost.end()) description += wxString::Format(dmgraisefmt, wxString::Format(wxT("%s boost"), HADES_STRING_ELEMENT_NAME[i]));
			for (i = 0; i < 8; i++) if (calc.use_target_element_affinity.find(i) != calc.use_target_element_affinity.end() && (calc.defender_element_weak & (1 << i)) != 0) description += wxString::Format(dmgraisefmt, wxString::Format(wxT("%s weakness"), HADES_STRING_ELEMENT_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_caster_status_inc_damage.find(i) != calc.use_caster_status_inc_damage.end()) description += wxString::Format(dmgraisefmt, wxString::Format(wxT("%s's %s"), calc.attacker_name, HADES_STRING_STATUS_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_target_status_inc_damage.find(i) != calc.use_target_status_inc_damage.end()) description += wxString::Format(dmgraisefmt, wxString::Format(wxT("%s's %s"), calc.defender_name, HADES_STRING_STATUS_NAME[i]));
			if ((calc.use_caster_support & SUPPORT_ABILITY_LONG_REACH) != 0) description += wxString::Format(dmgraisefmt, L"Long Reach");
			if ((calc.use_caster_support & SUPPORT_ABILITY_MP_ATTACK) != 0) description += wxString::Format(dmgraisefmt, L"MP Attack");
			if ((calc.use_caster_support & SUPPORT_ABILITY_BIRD_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Bird Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_BUG_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Bug Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_STONE_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Stone Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_UNDEAD_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Undead Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_DRAGON_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Dragon Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_DEVIL_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Devil Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_BEAST_KILLER) != 0) description += wxString::Format(dmgraisefmt, L"Beast Killer");
			if ((calc.use_caster_support & SUPPORT_ABILITY_MAN_EATER) != 0) description += wxString::Format(dmgraisefmt, L"Man Eater");
			if ((calc.use_caster_support & SUPPORT_ABILITY_REFLECTx2) != 0) description += wxString::Format(dmgraisefmt, L"Reflectx2");
			if ((calc.use_caster_support & SUPPORT_ABILITY_CONCENTRATE) != 0) description += wxString::Format(dmgraisefmt, L"Concentrate");
			if (calc.use_back_attack) description += wxString::Format(dmgraisefmt, L"The back attack");
			for (i = 0; i < 8; i++) if (calc.use_target_element_affinity.find(i) != calc.use_target_element_affinity.end() && (calc.defender_element_half & (1 << i)) != 0) description += wxString::Format(dmglowfmt, wxString::Format(wxT("%s resistance"), HADES_STRING_ELEMENT_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_caster_status_low_damage.find(i) != calc.use_caster_status_low_damage.end()) description += wxString::Format(dmglowfmt, wxString::Format(wxT("%s's %s"), calc.attacker_name, HADES_STRING_STATUS_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_target_status_low_damage.find(i) != calc.use_target_status_low_damage.end()) description += wxString::Format(dmglowfmt, wxString::Format(wxT("%s's %s"), calc.defender_name, HADES_STRING_STATUS_NAME[i]));
			if (calc.use_row) description += wxString::Format(dmglowfmt, L"The back row");
			if (calc.use_self_attack_no_evade) description += _(STRING_MODIF_SELF_ATTACK);
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_caster_status_inc_rate.find(i) != calc.use_caster_status_inc_rate.end()) description += wxString::Format(wxT(STRING_MODIF_HIT_RAISE), wxString::Format(wxT("%s's %s"), calc.attacker_name, HADES_STRING_STATUS_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_target_status_inc_rate.find(i) != calc.use_target_status_inc_rate.end()) description += wxString::Format(wxT(STRING_MODIF_HIT_RAISE), wxString::Format(wxT("%s's %s"), calc.defender_name, HADES_STRING_STATUS_NAME[i]));
			if ((calc.use_caster_support & SUPPORT_ABILITY_ACCURACY_PLUS) != 0) description += wxString::Format(wxT(STRING_MODIF_HIT_RAISE), L"Accuracy+");
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_caster_status_low_rate.find(i) != calc.use_caster_status_low_rate.end()) description += wxString::Format(wxT(STRING_MODIF_HIT_LOW), wxString::Format(wxT("%s's %s"), calc.attacker_name, HADES_STRING_STATUS_NAME[i]));
			for (i = 0; i < STATUS_AMOUNT; i++) if (calc.use_target_status_low_rate.find(i) != calc.use_target_status_low_rate.end()) description += wxString::Format(wxT(STRING_MODIF_HIT_LOW), wxString::Format(wxT("%s's %s"), calc.defender_name, HADES_STRING_STATUS_NAME[i]));
			if ((calc.use_target_support & SUPPORT_ABILITY_DISTRACT) != 0) description += wxString::Format(wxT(STRING_MODIF_HIT_LOW), L"Distract");
			if (!calc.addendum_end.IsEmpty()) description += desclengthtmp > description.Len() ? _(L"\n") + calc.addendum_end : calc.addendum_end;
		}
	}
	m_description->ChangeValue(description);
}

void ToolDamageCalculator::UpdateIpsenCurseFlag(EnemyDataStruct* enemy) {
	if (!initialized)
		return;
	bool curseactive = false;
	if (enemy == NULL) {
		if (!curseactive && m_panelleft->GetSelection() == 1 && enemy_panel[0]->m_baseenemy->GetSelection() != wxNOT_FOUND) curseactive = (cddata->enemyset.battle[enemy_list_battle[enemy_panel[0]->m_baseenemy->GetSelection()]]->flag & BATTLE_FLAG_IPSEN_CURSE) != 0;
		if (!curseactive && m_panelright->GetSelection() == 1 && enemy_panel[1]->m_baseenemy->GetSelection() != wxNOT_FOUND) curseactive = (cddata->enemyset.battle[enemy_list_battle[enemy_panel[1]->m_baseenemy->GetSelection()]]->flag & BATTLE_FLAG_IPSEN_CURSE) != 0;
	} else {
		curseactive = (enemy->flag & BATTLE_FLAG_IPSEN_CURSE) != 0;
	}
	player_panel[0]->stat->buff.ipsen_curse = curseactive;
	player_panel[1]->stat->buff.ipsen_curse = curseactive;
	enemy_panel[0]->buff.ipsen_curse = curseactive;
	enemy_panel[1]->buff.ipsen_curse = curseactive;
}

void ToolDamageCalculator::OnCharacterTypeChange(wxBookCtrlEvent& event) {
	if (!initialized)
		return;
	int sel = event.GetSelection();
	int id = event.GetId();
	if (sel == 0 && id == wxID_LEFT)
		player_panel[0]->ApplyPlayerStatToInterface();
	else if (sel == 0 && id == wxID_RIGHT)
		player_panel[1]->ApplyPlayerStatToInterface();
	else if (sel == 1)
		UpdateIpsenCurseFlag();
	UpdateInformation();
}

void ToolDamageCalculator::OnButtonSwap(wxCommandEvent& event) {
	bool attackerisplayer = m_panelleft->GetSelection() == 0;
	bool defenderisplayer = m_panelright->GetSelection() == 0;
	if (attackerisplayer && defenderisplayer) {
		player_panel[0]->Swap(player_panel[1]);
	} else if (!attackerisplayer && !defenderisplayer) {
		enemy_panel[0]->Swap(enemy_panel[1]);
	} else if (attackerisplayer && !defenderisplayer) {
		player_panel[0]->Swap(player_panel[1]);
		enemy_panel[0]->Swap(enemy_panel[1]);
		m_panelleft->ChangeSelection(1);
		m_panelright->ChangeSelection(0);
	} else if (!attackerisplayer && defenderisplayer) {
		player_panel[0]->Swap(player_panel[1]);
		enemy_panel[0]->Swap(enemy_panel[1]);
		m_panelleft->ChangeSelection(0);
		m_panelright->ChangeSelection(1);
	}
	UpdateInformation();
}

ToolDamageCalculatorPlayer::ToolDamageCalculatorPlayer(wxWindow* p) : DamageCalculatorPlayerPanel(p) {}

void ToolDamageCalculatorPlayer::Init(CDDataStruct* data, bool initializeprofile) {
	wxArrayString choicelist;
	int pchar, i;
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++)
		choicelist.Add(_(data->itemset.item[i].name.str_nice));
	m_weapon->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->itemset.item.size(); i++)
		choicelist.Add(_(data->itemset.item[i].name.str_nice));
	m_head->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->itemset.item.size(); i++)
		choicelist.Add(_(data->itemset.item[i].name.str_nice));
	m_wrist->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->itemset.item.size(); i++)
		choicelist.Add(_(data->itemset.item[i].name.str_nice));
	m_armor->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->itemset.item.size(); i++)
		choicelist.Add(_(data->itemset.item[i].name.str_nice));
	m_accessory->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->spellset.spell.size(); i++)
		choicelist.Add(_(data->spellset.spell[i].name.str_nice));
	m_basespell->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT); i++)
		m_effect->Append(HADES_STRING_SPELL_EFFECT[i].label, (void*)&HADES_STRING_SPELL_EFFECT[i]);
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		choicelist.Add(data->spellset.status_set[i].name);
	m_status->Append(choicelist);
	choicelist.Clear();
	if (initializeprofile) {
		for (pchar = 0; pchar < PLAYABLE_CHAR_AMOUNT; pchar++) {
			stat = &parent->player_profile[pchar];
			ItemDataSet& itset = parent->cddata->itemset;
			uint16_t charfilter = 1 << (11 - pchar);
			bool weaponset = false;
			bool headset = false;
			bool wristset = false;
			bool armorset = false;
			bool accessoryset = false;
			stat->buff.back_row = pchar == 1 || pchar == 2 || pchar == 6;
			for (i = 0; i < itset.item.size(); i++) {
				ItemDataStruct& it = itset.item[i];
				if ((it.char_availability & charfilter) != 0) {
					if (!weaponset && (it.type & ITEM_TYPE_WEAPON) != 0 && it.weapon_id >= 0) {
						stat->weapon_id = i;
						weaponset = true;
					}
					if (!headset && (it.type & ITEM_TYPE_HEAD) != 0) {
						stat->head_id = i;
						headset = true;
					}
					if (!wristset && (it.type & ITEM_TYPE_WRIST) != 0) {
						stat->wrist_id = i;
						wristset = true;
					}
					if (!armorset && (it.type & ITEM_TYPE_ARMOR) != 0) {
						stat->armor_id = i;
						armorset = true;
					}
					if (!accessoryset && (it.type & ITEM_TYPE_ACCESSORY) != 0) {
						stat->accessory_id = i;
						accessoryset = true;
					}
				}
			}
		}
	}
	base_character_selection = 0;
	stat = &parent->player_profile[base_character_selection];
	m_basechar->SetSelection(base_character_selection);
	ApplyPlayerStatToInterface();
	m_basespell->SetSelection(0);
	m_effect->SetSelection(0);
	m_status->SetSelection(0);
}

void ToolDamageCalculatorPlayer::EnableSpellOptions(bool enable) {
	m_basespell->Enable(enable);
	m_effect->Enable(enable);
	m_power->Enable(enable);
	m_accuracy->Enable(enable);
	m_status->Enable(enable);
	m_fire->Enable(enable);
	m_ice->Enable(enable);
	m_thunder->Enable(enable);
	m_earth->Enable(enable);
	m_water->Enable(enable);
	m_wind->Enable(enable);
	m_holy->Enable(enable);
	m_shadow->Enable(enable);
}

void ToolDamageCalculatorPlayer::ComputeEquipCumulatedStat() {
	ItemDataSet& itemset = parent->cddata->itemset;
	ItemDataStruct& weapon = itemset.item[m_weapon->GetSelection()];
	ItemDataStruct& head = itemset.item[m_head->GetSelection()];
	ItemDataStruct& wrist = itemset.item[m_wrist->GetSelection()];
	ItemDataStruct& armor = itemset.item[m_armor->GetSelection()];
	ItemDataStruct& accessory = itemset.item[m_accessory->GetSelection()];
	ItemStatDataStruct& weaponstat = itemset.GetStatById(weapon.stat_id);
	ItemStatDataStruct& headstat = itemset.GetStatById(head.stat_id);
	ItemStatDataStruct& wriststat = itemset.GetStatById(wrist.stat_id);
	ItemStatDataStruct& armorstat = itemset.GetStatById(armor.stat_id);
	ItemStatDataStruct& accessorystat = itemset.GetStatById(accessory.stat_id);
	ItemArmorDataStruct* weaponarmor = weapon.armor_id >= 0 ? &itemset.GetArmorById(weapon.armor_id) : NULL;
	ItemArmorDataStruct* headarmor = head.armor_id >= 0 ? &itemset.GetArmorById(head.armor_id) : NULL;
	ItemArmorDataStruct* wristarmor = wrist.armor_id >= 0 ? &itemset.GetArmorById(wrist.armor_id) : NULL;
	ItemArmorDataStruct* armorarmor = armor.armor_id >= 0 ? &itemset.GetArmorById(armor.armor_id) : NULL;
	ItemArmorDataStruct* accessoryarmor = accessory.armor_id >= 0 ? &itemset.GetArmorById(accessory.armor_id) : NULL;
	equip.attack = weapon.weapon_id >= 0 ? itemset.GetWeaponById(weapon.weapon_id).power : 0;
	equip.speed = weaponstat.speed + headstat.speed + wriststat.speed + armorstat.speed + accessorystat.speed;
	equip.strength = weaponstat.strength + headstat.strength + wriststat.strength + armorstat.strength + accessorystat.strength;
	equip.magic = weaponstat.magic + headstat.magic + wriststat.magic + armorstat.magic + accessorystat.magic;
	equip.spirit = weaponstat.spirit + headstat.spirit + wriststat.spirit + armorstat.spirit + accessorystat.spirit;
	equip.defence = (weaponarmor != NULL ? weaponarmor->defence : 0) + (headarmor != NULL ? headarmor->defence : 0) + (wristarmor != NULL ? wristarmor->defence : 0) + (armorarmor != NULL ? armorarmor->defence : 0) + (accessoryarmor != NULL ? accessoryarmor->defence : 0);
	equip.evade = (weaponarmor != NULL ? weaponarmor->evade : 0) + (headarmor != NULL ? headarmor->evade : 0) + (wristarmor != NULL ? wristarmor->evade : 0) + (armorarmor != NULL ? armorarmor->evade : 0) + (accessoryarmor != NULL ? accessoryarmor->evade : 0);
	equip.magic_defence = (weaponarmor != NULL ? weaponarmor->magic_defence : 0) + (headarmor != NULL ? headarmor->magic_defence : 0) + (wristarmor != NULL ? wristarmor->magic_defence : 0) + (armorarmor != NULL ? armorarmor->magic_defence : 0) + (accessoryarmor != NULL ? accessoryarmor->magic_defence : 0);
	equip.magic_evade = (weaponarmor != NULL ? weaponarmor->magic_evade : 0) + (headarmor != NULL ? headarmor->magic_evade : 0) + (wristarmor != NULL ? wristarmor->magic_evade : 0) + (armorarmor != NULL ? armorarmor->magic_evade : 0) + (accessoryarmor != NULL ? accessoryarmor->magic_evade : 0);
	equip.element_absorb = weaponstat.element_absorb | headstat.element_absorb | wriststat.element_absorb | armorstat.element_absorb | accessorystat.element_absorb;
	equip.element_immune = weaponstat.element_immune | headstat.element_immune | wriststat.element_immune | armorstat.element_immune | accessorystat.element_immune;
	equip.element_half = weaponstat.element_half | headstat.element_half | wriststat.element_half | armorstat.element_half | accessorystat.element_half;
	equip.element_weak = weaponstat.element_weak | headstat.element_weak | wriststat.element_weak | armorstat.element_weak | accessorystat.element_weak;
	equip.element_boost = weaponstat.element_boost | headstat.element_boost | wriststat.element_boost | armorstat.element_boost | accessorystat.element_boost;
}

void ToolDamageCalculatorPlayer::ComputePlayerBaseStat() {
	if (!parent->cddata->saveset.sectionloaded[DATA_SECTION_STAT]) {
		computed_base_stat_speed = 0;
		computed_base_stat_strength = 0;
		computed_base_stat_magic = 0;
		computed_base_stat_spirit = 0;
		computed_level_stat_speed = 0;
		computed_level_stat_strength = 0;
		computed_level_stat_magic = 0;
		computed_level_stat_spirit = 0;
		computed_bonus_stat_speed = 0;
		computed_bonus_stat_strength = 0;
		computed_bonus_stat_magic = 0;
		computed_bonus_stat_spirit = 0;
		base_stat_max_hp = 1;
		base_stat_speed = 0;
		base_stat_strength = 0;
		base_stat_magic = 0;
		base_stat_spirit = 0;
	} else {
		int lvl = m_level->GetValue();
		StatDataSet& statset = parent->cddata->statset;
		InitialStatDataStruct& base = statset.initial_stat[m_basechar->GetSelection()];
		LevelDataStruct& lvldata = statset.level[lvl - 1];
		int cumulatedspd = 0;
		int cumulatedstr = 0;
		int cumulatedmgc = 0;
		int cumulatedspr = 0;
		for (int i = 0; i < lvl; i++) {
			cumulatedspd += stat->equip_speed[i];
			cumulatedstr += stat->equip_strength[i];
			cumulatedmgc += stat->equip_magic[i];
			cumulatedspr += stat->equip_spirit[i];
		}
		computed_base_stat_speed = base.speed;
		computed_base_stat_strength = base.strength;
		computed_base_stat_magic = base.magic;
		computed_base_stat_spirit = base.spirit;
		computed_level_stat_speed = lvl / 10;
		computed_level_stat_strength = 3 * lvl / 10;
		computed_level_stat_magic = 3 * lvl / 10;
		computed_level_stat_spirit = 3 * lvl / 20;
		computed_bonus_stat_speed = cumulatedspd / 32;
		computed_bonus_stat_strength = (3 * (lvl - 1) + cumulatedstr) / 32;
		computed_bonus_stat_magic = (3 * (lvl - 1) + cumulatedmgc) / 32;
		computed_bonus_stat_spirit = (1 * (lvl - 1) + cumulatedspr) / 32;
		base_stat_speed = min(50, computed_base_stat_speed + computed_level_stat_speed + computed_bonus_stat_speed);
		base_stat_strength = min(99, computed_base_stat_strength + computed_level_stat_strength + computed_bonus_stat_strength);
		base_stat_magic = min(99, computed_base_stat_magic + computed_level_stat_magic + computed_bonus_stat_magic);
		base_stat_spirit = min(50, computed_base_stat_spirit + computed_level_stat_spirit + computed_bonus_stat_spirit);
		int totalstr = max(0, min(99, base_stat_strength + stat->strength));
		base_stat_max_hp = min(9999, lvldata.hp_table * totalstr / 50);
	}
}

void ToolDamageCalculatorPlayer::ApplyPlayerStatToInterface() {
	m_level->SetValue(stat->level);
	m_weapon->SetSelection(stat->weapon_id);
	m_head->SetSelection(stat->head_id);
	m_wrist->SetSelection(stat->wrist_id);
	m_armor->SetSelection(stat->armor_id);
	m_accessory->SetSelection(stat->accessory_id);
}

void ToolDamageCalculatorPlayer::UpdatePlayerStatFromInterface() {
	stat->level = m_level->GetValue();
	stat->weapon_id = m_weapon->GetSelection();
	stat->head_id = m_head->GetSelection();
	stat->wrist_id = m_wrist->GetSelection();
	stat->armor_id = m_armor->GetSelection();
	stat->accessory_id = m_accessory->GetSelection();
}

void ToolDamageCalculatorPlayer::ApplyStatToDialog(DamageCalculatorStats* dialog) {
	dialog->m_maxhp->SetValue(stat->max_hp);
	dialog->m_attack->SetValue(stat->attack);
	dialog->m_speed->SetValue(stat->speed);
	dialog->m_strength->SetValue(stat->strength);
	dialog->m_magic->SetValue(stat->magic);
	dialog->m_spirit->SetValue(stat->spirit);
	dialog->m_defence->SetValue(stat->defence);
	dialog->m_evade->SetValue(stat->evade);
	dialog->m_magicdefence->SetValue(stat->magic_defence);
	dialog->m_magicevade->SetValue(stat->magic_evade);
	dialog->m_jewelcount->SetValue(stat->jewel_count);
}

void ToolDamageCalculatorPlayer::UpdateStatFromDialog(DamageCalculatorStats* dialog) {
	stat->max_hp = dialog->m_maxhp->GetValue();
	stat->attack = dialog->m_attack->GetValue();
	stat->speed = dialog->m_speed->GetValue();
	stat->strength = dialog->m_strength->GetValue();
	stat->magic = dialog->m_magic->GetValue();
	stat->spirit = dialog->m_spirit->GetValue();
	stat->defence = dialog->m_defence->GetValue();
	stat->evade = dialog->m_evade->GetValue();
	stat->magic_defence = dialog->m_magicdefence->GetValue();
	stat->magic_evade = dialog->m_magicevade->GetValue();
	stat->jewel_count = dialog->m_jewelcount->GetValue();
}

void ToolDamageCalculatorPlayer::ApplySupportToDialog(DamageCalculatorSupporting* dialog) {
	dialog->m_hp10->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_HP10) != 0);
	dialog->m_hp20->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_HP20) != 0);
	dialog->m_longreach->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_LONG_REACH) != 0);
	dialog->m_accuracyplus->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_ACCURACY_PLUS) != 0);
	dialog->m_distract->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_DISTRACT) != 0);
	dialog->m_mpattack->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_MP_ATTACK) != 0);
	dialog->m_birdkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_BIRD_KILLER) != 0);
	dialog->m_bugkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_BUG_KILLER) != 0);
	dialog->m_stonekiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_STONE_KILLER) != 0);
	dialog->m_undeadkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_UNDEAD_KILLER) != 0);
	dialog->m_dragonkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_DRAGON_KILLER) != 0);
	dialog->m_devilkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_DEVIL_KILLER) != 0);
	dialog->m_beastkiller->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_BEAST_KILLER) != 0);
	dialog->m_maneater->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_MAN_EATER) != 0);
	dialog->m_healer->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_HEALER) != 0);
	dialog->m_reflectx2->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_REFLECTx2) != 0);
	dialog->m_magelemnull->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_MAG_ELEM_NULL) != 0);
	dialog->m_concentrate->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_CONCENTRATE) != 0);
	dialog->m_gambledefence->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_GAMBLE_DEFENCE) != 0);
	dialog->m_bandit->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_BANDIT) != 0);
	dialog->m_masterthief->SetValue((stat->supporting_ability_on & SUPPORT_ABILITY_MASTER_THIEF) != 0);
}

void ToolDamageCalculatorPlayer::UpdateSupportFromDialog(DamageCalculatorSupporting* dialog) {
	stat->supporting_ability_on = 0;
	if (dialog->m_hp10->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_HP10;
	if (dialog->m_hp20->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_HP20;
	if (dialog->m_longreach->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_LONG_REACH;
	if (dialog->m_accuracyplus->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_ACCURACY_PLUS;
	if (dialog->m_distract->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_DISTRACT;
	if (dialog->m_mpattack->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_MP_ATTACK;
	if (dialog->m_birdkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_BIRD_KILLER;
	if (dialog->m_bugkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_BUG_KILLER;
	if (dialog->m_stonekiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_STONE_KILLER;
	if (dialog->m_undeadkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_UNDEAD_KILLER;
	if (dialog->m_dragonkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_DRAGON_KILLER;
	if (dialog->m_devilkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_DEVIL_KILLER;
	if (dialog->m_beastkiller->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_BEAST_KILLER;
	if (dialog->m_maneater->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_MAN_EATER;
	if (dialog->m_healer->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_HEALER;
	if (dialog->m_reflectx2->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_REFLECTx2;
	if (dialog->m_magelemnull->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_MAG_ELEM_NULL;
	if (dialog->m_concentrate->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_CONCENTRATE;
	if (dialog->m_gambledefence->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_GAMBLE_DEFENCE;
	if (dialog->m_bandit->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_BANDIT;
	if (dialog->m_masterthief->GetValue()) stat->supporting_ability_on |= SUPPORT_ABILITY_MASTER_THIEF;
}

void ToolDamageCalculatorPlayer::SelectBaseCharacter(int sel) {
	if (sel != base_character_selection) {
		base_character_selection = sel;
		stat = &parent->player_profile[sel];
	}
	ApplyPlayerStatToInterface();
	ComputeEquipCumulatedStat();
	ComputePlayerBaseStat();
	int weapid = parent->cddata->itemset.item[m_weapon->GetSelection()].weapon_id;
	if (weapid >= 0) {
		m_basespell->SetSelection(SPELL_ATTACK_ID);
		SelectBaseSpell(SPELL_ATTACK_ID);
	} else if (parent->cddata->saveset.sectionloaded[DATA_SECTION_STAT] && parent->cddata->saveset.sectionloaded[DATA_SECTION_CMD]) {
		int spellid = 0;
		CommandSetDataStruct& cmd = parent->cddata->statset.command_list[parent->cddata->statset.GetCharacterCommandsIndices(sel)[0]];
		CommandDataStruct& firstcmd = parent->cddata->cmdset.GetCommandById(cmd.first_command);
		CommandDataStruct& secondcmd = parent->cddata->cmdset.GetCommandById(cmd.second_command);
		if (firstcmd.panel == COMMAND_PANEL_SPELL && firstcmd.spell_amount > 0)
			spellid = firstcmd.spell_list[0];
		else if (secondcmd.panel == COMMAND_PANEL_SPELL && secondcmd.spell_amount > 0)
			spellid = secondcmd.spell_list[0];
		else if (firstcmd.panel == COMMAND_PANEL_NONE)
			spellid = firstcmd.spell_list[0];
		else if (secondcmd.panel == COMMAND_PANEL_NONE)
			spellid = secondcmd.spell_list[0];
		m_basespell->SetSelection(spellid);
		SelectBaseSpell(spellid);
	} else {
		m_basespell->SetSelection(0);
		SelectBaseSpell(0);
	}
}

void ToolDamageCalculatorPlayer::SelectBaseSpell(int sel) {
	if (sel == SPELL_ATTACK_ID) {
		ItemDataSet& itset = parent->cddata->itemset;
		int weapid = itset.item[m_weapon->GetSelection()].weapon_id;
		if (weapid >= 0) {
			ItemWeaponDataStruct& weap = itset.GetWeaponById(weapid);
			for (int i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT); i++)
				if (HADES_STRING_SPELL_EFFECT[i].id == weap.damage_formula) {
					m_effect->SetSelection(i);
					break;
				}
			m_accuracy->SetValue(weap.status_accuracy);
			m_status->SetSelection(weap.status);
			//m_power->SetValue(weap.power);
			m_power->SetValue(0);
			MACRO_CALCULATOR_SETELEMS(weap.element)
			parent->UpdateInformation();
			return;
		}
	}
	SpellDataStruct& spell = parent->cddata->spellset.spell[sel];
	for (int i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT); i++)
		if (HADES_STRING_SPELL_EFFECT[i].id == spell.effect) {
			m_effect->SetSelection(i);
			break;
		}
	m_power->SetValue(spell.power);
	m_accuracy->SetValue(spell.accuracy);
	m_status->SetSelection(spell.status);
	m_power->SetValue(spell.power);
	MACRO_CALCULATOR_SETELEMS(spell.element)
	parent->UpdateInformation();
}

void ToolDamageCalculatorPlayer::Swap(ToolDamageCalculatorPlayer* other) {
	swap(base_stat_max_hp, other->base_stat_max_hp);
	swap(base_stat_speed, other->base_stat_speed);
	swap(base_stat_strength, other->base_stat_strength);
	swap(base_stat_magic, other->base_stat_magic);
	swap(base_stat_spirit, other->base_stat_spirit);
	swap(stat, other->stat);
	swap(equip, other->equip);
	swap(computed_base_stat_speed, other->computed_base_stat_speed);
	swap(computed_base_stat_strength, other->computed_base_stat_strength);
	swap(computed_base_stat_magic, other->computed_base_stat_magic);
	swap(computed_base_stat_spirit, other->computed_base_stat_spirit);
	swap(computed_level_stat_speed, other->computed_level_stat_speed);
	swap(computed_level_stat_strength, other->computed_level_stat_strength);
	swap(computed_level_stat_magic, other->computed_level_stat_magic);
	swap(computed_level_stat_spirit, other->computed_level_stat_spirit);
	swap(computed_bonus_stat_speed, other->computed_bonus_stat_speed);
	swap(computed_bonus_stat_strength, other->computed_bonus_stat_strength);
	swap(computed_bonus_stat_magic, other->computed_bonus_stat_magic);
	swap(computed_bonus_stat_spirit, other->computed_bonus_stat_spirit);
	{
		int tmp;
		MACRO_SWAP_WINDOW_ELEMENT(m_basechar, other->m_basechar, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_level, other->m_level, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_weapon, other->m_weapon, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_head, other->m_head, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_wrist, other->m_wrist, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_armor, other->m_armor, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_accessory, other->m_accessory, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_basespell, other->m_basespell, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_effect, other->m_effect, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_power, other->m_power, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_accuracy, other->m_accuracy, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_status, other->m_status, GetSelection, SetSelection)
	}
	{
		bool tmp;
		MACRO_SWAP_WINDOW_ELEMENT(m_fire, other->m_fire, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_ice, other->m_ice, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_thunder, other->m_thunder, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_earth, other->m_earth, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_water, other->m_water, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_wind, other->m_wind, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_holy, other->m_holy, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_shadow, other->m_shadow, GetValue, SetValue)
	}
}

void ToolDamageCalculatorPlayer::OnCharacterChange(wxCommandEvent& event) {
	UpdatePlayerStatFromInterface();
	SelectBaseCharacter(event.GetSelection());
}

void ToolDamageCalculatorPlayer::OnSpellChange(wxCommandEvent& event) {
	SelectBaseSpell(event.GetSelection());
}

void ToolDamageCalculatorPlayer::OnLevelChange(wxSpinEvent& event) {
	int newlevel = event.GetPosition();
	if (newlevel > stat->level) {
		ComputeEquipCumulatedStat();
		for (int i = stat->level; i < newlevel; i++) {
			stat->equip_speed[i] = equip.speed;
			stat->equip_strength[i] = equip.strength;
			stat->equip_magic[i] = equip.magic;
			stat->equip_spirit[i] = equip.spirit;
		}
	} else if (newlevel < stat->level) {
		for (int i = stat->level - 1; i >= newlevel; i--) {
			stat->equip_speed[i] = 0;
			stat->equip_strength[i] = 0;
			stat->equip_magic[i] = 0;
			stat->equip_spirit[i] = 0;
		}
	}
	stat->level = newlevel;
	ComputePlayerBaseStat();
	parent->UpdateInformation();
}

void ToolDamageCalculatorPlayer::OnEquipChange(wxCommandEvent& event) {
	ComputeEquipCumulatedStat();
	if (event.GetId() == wxID_WEAPON && m_basespell->GetSelection() == SPELL_ATTACK_ID)
		SelectBaseSpell(SPELL_ATTACK_ID);
	parent->UpdateInformation();
}

void ToolDamageCalculatorPlayer::OnStatButton(wxCommandEvent& event) {
	DamageCalculatorStats dial(parent);
	ApplyStatToDialog(&dial);
	if (dial.ShowModal() == wxID_OK) {
		UpdateStatFromDialog(&dial);
		ComputePlayerBaseStat();
		parent->UpdateInformation();
	}
}

void ToolDamageCalculatorPlayer::OnSupportingButton(wxCommandEvent& event) {
	DamageCalculatorSupporting dial(parent);
	ApplySupportToDialog(&dial);
	if (dial.ShowModal() == wxID_OK) {
		UpdateSupportFromDialog(&dial);
		parent->UpdateInformation();
	}
}

void ToolDamageCalculatorPlayer::OnBuffButton(wxCommandEvent& event) {
	DamageCalculatorBuff dial(parent);
	stat->buff.ApplyToDialog(&dial);
	if (dial.ShowModal() == wxID_OK) {
		stat->buff.UpdateFromDialog(&dial);
		parent->UpdateInformation();
	}
}

void ToolDamageCalculatorPlayer::OnUpdate(wxCommandEvent& event) {
	parent->UpdateInformation();
}

void ToolDamageCalculatorPlayer::OnUpdateSpin(wxSpinEvent& event) {
	parent->UpdateInformation();
}

ToolDamageCalculatorEnemy::ToolDamageCalculatorEnemy(wxWindow* p) : DamageCalculatorEnemyPanel(p) {}

void ToolDamageCalculatorEnemy::Init(CDDataStruct* data) {
	wxArrayString choicelist;
	int i;
	for (i = 0; i < parent->enemy_list_battle.size(); i++)
		choicelist.Add(_(data->enemyset.battle[parent->enemy_list_battle[i]]->stat[parent->enemy_list_stat[i]].name.str_nice));
	m_baseenemy->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < data->enemyset.battle[parent->enemy_list_battle[0]]->spell_amount; i++)
		choicelist.Add(_(data->enemyset.battle[parent->enemy_list_battle[0]]->spell[i].name.str_nice));
	m_basespell->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT); i++)
		m_effect->Append(HADES_STRING_SPELL_EFFECT[i].label, (void*)&HADES_STRING_SPELL_EFFECT[i]);
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		choicelist.Add(data->spellset.status_set[i].name);
	m_status->Append(choicelist);
	choicelist.Clear();
	m_baseenemy->SetSelection(0);
	m_basespell->SetSelection(0);
	m_effect->SetSelection(0);
	m_status->SetSelection(0);
}

void ToolDamageCalculatorEnemy::EnableSpellOptions(bool enable) {
	m_basespell->Enable(enable);
	m_effect->Enable(enable);
	m_power->Enable(enable);
	m_accuracy->Enable(enable);
	m_status->Enable(enable);
	m_fire->Enable(enable);
	m_ice->Enable(enable);
	m_thunder->Enable(enable);
	m_earth->Enable(enable);
	m_water->Enable(enable);
	m_wind->Enable(enable);
	m_holy->Enable(enable);
	m_shadow->Enable(enable);
}

void ToolDamageCalculatorEnemy::SelectBaseEnemy(int sel) {
	m_basespell->Clear();
	EnemyDataStruct* enemy = parent->cddata->enemyset.battle[parent->enemy_list_battle[sel]];
	EnemyStatDataStruct& stat = enemy->stat[parent->enemy_list_stat[sel]];
	for (int i = 0; i < enemy->spell_amount; i++)
		m_basespell->Append(_(enemy->spell[i].name.str_nice));
	m_basespell->SetSelection(0);
	m_level->SetValue(stat.lvl);
	m_maxhp->SetValue(stat.hp);
	MACRO_CALCULATOR_SETCLASS(stat.classification)
	m_speed->SetValue(stat.speed);
	m_strength->SetValue(stat.strength);
	m_magic->SetValue(stat.magic);
	m_spirit->SetValue(stat.spirit);
	m_defence->SetValue(stat.defence);
	m_evade->SetValue(stat.evade);
	m_magicdefence->SetValue(stat.magic_defence);
	m_magicevade->SetValue(stat.magic_evade);
	buff.element_absorb = stat.element_absorb;
	buff.element_immune = stat.element_immune;
	buff.element_half = stat.element_half;
	buff.element_weak = stat.element_weak;
	buff.element_boost = 0; // TODO: include the bug of the PSX version
	buff.status = stat.status_auto | stat.status_initial;
	buff.back_row = false;
	buff.back_attack = (enemy->flag & BATTLE_FLAG_BACK_ATTACK) != 0;
	parent->UpdateIpsenCurseFlag(enemy);
	buff.reflect_count = 0;
	SelectBaseSpell(0);
}

void ToolDamageCalculatorEnemy::SelectBaseSpell(int sel) {
	EnemySpellDataStruct& spell = parent->cddata->enemyset.battle[parent->enemy_list_battle[m_baseenemy->GetSelection()]]->spell[sel];
	for (int i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT); i++)
		if (HADES_STRING_SPELL_EFFECT[i].id == spell.effect) {
			m_effect->SetSelection(i);
			break;
		}
	m_power->SetValue(spell.power);
	m_accuracy->SetValue(spell.accuracy);
	m_status->SetSelection(spell.status);
	m_power->SetValue(spell.power);
	MACRO_CALCULATOR_SETELEMS(spell.element)
	parent->UpdateInformation();
}

void ToolDamageCalculatorEnemy::Swap(ToolDamageCalculatorEnemy* other) {
	int i;
	int spellsel1 = m_basespell->GetSelection();
	int spellsel2 = other->m_basespell->GetSelection();
	EnemyDataStruct* enemy1 = parent->cddata->enemyset.battle[parent->enemy_list_battle[m_baseenemy->GetSelection()]];
	EnemyDataStruct* enemy2 = parent->cddata->enemyset.battle[parent->enemy_list_battle[other->m_baseenemy->GetSelection()]];
	m_basespell->Clear();
	other->m_basespell->Clear();
	for (i = 0; i < enemy1->spell_amount; i++)
		other->m_basespell->Append(_(enemy1->spell[i].name.str_nice));
	for (i = 0; i < enemy2->spell_amount; i++)
		m_basespell->Append(_(enemy2->spell[i].name.str_nice));
	other->m_basespell->SetSelection(spellsel1);
	m_basespell->SetSelection(spellsel2);
	swap(buff, other->buff);
	{
		int tmp;
		MACRO_SWAP_WINDOW_ELEMENT(m_baseenemy, other->m_baseenemy, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_level, other->m_level, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_maxhp, other->m_maxhp, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_speed, other->m_speed, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_strength, other->m_strength, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_magic, other->m_magic, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_spirit, other->m_spirit, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_defence, other->m_defence, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_evade, other->m_evade, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_magicdefence, other->m_magicdefence, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_magicevade, other->m_magicevade, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_effect, other->m_effect, GetSelection, SetSelection)
		MACRO_SWAP_WINDOW_ELEMENT(m_power, other->m_power, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_accuracy, other->m_accuracy, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_status, other->m_status, GetSelection, SetSelection)
	}
	{
		bool tmp;
		MACRO_SWAP_WINDOW_ELEMENT(m_fire, other->m_fire, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_ice, other->m_ice, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_thunder, other->m_thunder, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_earth, other->m_earth, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_water, other->m_water, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_wind, other->m_wind, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_holy, other->m_holy, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_shadow, other->m_shadow, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_human, other->m_human, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_beast, other->m_beast, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_demon, other->m_demon, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_dragon, other->m_dragon, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_undead, other->m_undead, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_stone, other->m_stone, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_bug, other->m_bug, GetValue, SetValue)
		MACRO_SWAP_WINDOW_ELEMENT(m_flying, other->m_flying, GetValue, SetValue)
	}
}

void ToolDamageCalculatorEnemy::OnEnemyChange(wxCommandEvent& event) {
	SelectBaseEnemy(event.GetSelection());
}

void ToolDamageCalculatorEnemy::OnSpellChange(wxCommandEvent& event) {
	SelectBaseSpell(event.GetSelection());
}

void ToolDamageCalculatorEnemy::OnBuffButton(wxCommandEvent& event) {
	DamageCalculatorBuff dial(parent);
	buff.ApplyToDialog(&dial);
	dial.m_backrow->Enable(false);
	if (dial.ShowModal() == wxID_OK) {
		buff.UpdateFromDialog(&dial);
		parent->UpdateInformation();
	}
}

void ToolDamageCalculatorEnemy::OnUpdate(wxCommandEvent& event) {
	parent->UpdateInformation();
}

void ToolDamageCalculatorEnemy::OnUpdateSpin(wxSpinEvent& event) {
	parent->UpdateInformation();
}

void CalculatorBuff::ApplyToDialog(DamageCalculatorBuff* dialog) {
	MACRO_CALCULATOR_SETELEMS_WIN_NUM(element_absorb, dialog, 0)
	MACRO_CALCULATOR_SETELEMS_WIN_NUM(element_immune, dialog, 1)
	MACRO_CALCULATOR_SETELEMS_WIN_NUM(element_half, dialog, 2)
	MACRO_CALCULATOR_SETELEMS_WIN_NUM(element_weak, dialog, 3)
	MACRO_CALCULATOR_SETELEMS_WIN_NUM(element_boost, dialog, 4)
	dialog->m_evastatus->SetValue((status & STATUS_VIRUS) != 0);
	dialog->m_petrify->SetValue((status & STATUS_PETRIFY) != 0);
	dialog->m_blind->SetValue((status & STATUS_BLIND) != 0);
	dialog->m_zombie->SetValue((status & STATUS_ZOMBIE) != 0);
	dialog->m_confuse->SetValue((status & STATUS_CONFUSE) != 0);
	dialog->m_berserk->SetValue((status & STATUS_BERSERK) != 0);
	dialog->m_trance->SetValue((status & STATUS_TRANCE) != 0);
	dialog->m_defend->SetValue((status & STATUS_DEFEND) != 0);
	dialog->m_sleep->SetValue((status & STATUS_SLEEP) != 0);
	dialog->m_mini->SetValue((status & STATUS_MINI) != 0);
	dialog->m_protect->SetValue((status & STATUS_PROTECT) != 0);
	dialog->m_shell->SetValue((status & STATUS_SHELL) != 0);
	dialog->m_float->SetValue((status & STATUS_FLOAT) != 0);
	dialog->m_vanish->SetValue((status & STATUS_VANISH) != 0);
	dialog->m_easykill->SetValue((status & STATUS_EASY_KILL) != 0);
	dialog->m_backrow->SetValue(back_row);
	dialog->m_backattack->SetValue(back_attack);
	dialog->m_ipsen->SetValue(ipsen_curse);
	dialog->m_reflecting->SetValue(reflect_count);
}

void CalculatorBuff::UpdateFromDialog(DamageCalculatorBuff* dialog) {
	MACRO_CALCULATOR_GETELEMS_WIN_NUM(element_absorb, dialog, 0)
	MACRO_CALCULATOR_GETELEMS_WIN_NUM(element_immune, dialog, 1)
	MACRO_CALCULATOR_GETELEMS_WIN_NUM(element_half, dialog, 2)
	MACRO_CALCULATOR_GETELEMS_WIN_NUM(element_weak, dialog, 3)
	MACRO_CALCULATOR_GETELEMS_WIN_NUM(element_boost, dialog, 4)
	status = 0;
	if (dialog->m_evastatus->GetValue()) status |= STATUS_VIRUS;
	if (dialog->m_petrify->GetValue()) status |= STATUS_PETRIFY;
	if (dialog->m_blind->GetValue()) status |= STATUS_BLIND;
	if (dialog->m_zombie->GetValue()) status |= STATUS_ZOMBIE;
	if (dialog->m_confuse->GetValue()) status |= STATUS_CONFUSE;
	if (dialog->m_berserk->GetValue()) status |= STATUS_BERSERK;
	if (dialog->m_trance->GetValue()) status |= STATUS_TRANCE;
	if (dialog->m_defend->GetValue()) status |= STATUS_DEFEND;
	if (dialog->m_sleep->GetValue()) status |= STATUS_SLEEP;
	if (dialog->m_mini->GetValue()) status |= STATUS_MINI;
	if (dialog->m_protect->GetValue()) status |= STATUS_PROTECT;
	if (dialog->m_shell->GetValue()) status |= STATUS_SHELL;
	if (dialog->m_float->GetValue()) status |= STATUS_FLOAT;
	if (dialog->m_vanish->GetValue()) status |= STATUS_VANISH;
	if (dialog->m_easykill->GetValue()) status |= STATUS_EASY_KILL;
	back_row = dialog->m_backrow->GetValue();
	back_attack = dialog->m_backattack->GetValue();
	ipsen_curse = dialog->m_ipsen->GetValue();
	reflect_count = dialog->m_reflecting->GetValue();
}
