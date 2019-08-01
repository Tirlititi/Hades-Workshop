#include "Tool_Randomizer.h"

#include <random>
#include "Gui_Preferences.h"
#include "Database_SpellAnimation.h"
#include "main.h"

//=====================================//
//              Utilities              //
//=====================================//

static default_random_engine ThePRNGEngine;
inline int GetRandom(int min, int max) {
	uniform_int_distribution<int> distribution(min, max-1);
	return distribution(ThePRNGEngine);
}

inline void SetRandomSeed(unsigned int s) {
	ThePRNGEngine.seed(s);
}

inline bool SpellHasPower(uint8_t effect) {
	return effect==8 || effect==9 || effect==10 || (effect>=15 && effect<=21) || effect==23 || effect==26 || effect==32 || effect==37 || effect==38 || effect==39 || effect==63 || effect==85 || effect==86 || effect==87 || effect==91 || effect==98 || effect==99 || effect==100 || effect==102 || effect==107;
}

inline bool SpellHasStatus(SpellDataStruct& sp) {
	uint8_t eff = sp.effect;
	uint8_t acc = sp.accuracy;
	return (acc>0 && (eff==8 || eff==9 || eff==18 || eff==19 || eff==20 || eff==93 || eff==100 || eff==107)) || (eff>=11 && eff<=14) || eff==87 || eff==97 || eff==103 || eff==108 || eff==109;
}

inline bool IsNotSafeAbility(uint8_t abil, vector<uint8_t>& safelist) {
	for (unsigned int i=0; i<safelist.size(); i++)
		if (safelist[i]==abil)
			return false;
	return true;
}

inline bool DoesCharacterUseSpell(AbilitySetDataStruct& abillist, uint8_t spell) {
	for (unsigned int i=0; i<ABILITY_SET_CAPACITY; i++)
		if (abillist.ability[i]==spell)
			return true;
	return false;
}

inline bool IsSpellSummon(SpellDataStruct& sp) {
//	return sp.effect==49 || sp.effect==85 || sp.effect==86 || sp.effect==87; // Missing Carbuncle (Good Status)
	return sp.model==38 || sp.model==177 || sp.model==179 || sp.model==184 || sp.model==186 || sp.model==210 || sp.model==211 || sp.model==226 || sp.model==227 || sp.model==251 || sp.model==261 || sp.model==276 || sp.model==381 || sp.model==493 || sp.model==494 || sp.model==495;
}

inline Spell_Target_Amount GetEnemySpellTargetAmountFromModel(uint16_t modelid) {
	for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_SPELL_MODEL);i++)
		if (HADES_STRING_SPELL_MODEL[i].id==modelid) {
			if (HADES_STRING_SPELL_MODEL[i].target_amount==SPELL_TARGET_AMOUNT_ONE || HADES_STRING_SPELL_MODEL[i].target_amount==SPELL_TARGET_AMOUNT_GROUP)
				return HADES_STRING_SPELL_MODEL[i].target_amount;
			return SPELL_TARGET_AMOUNT_ZERO;
		}
	return SPELL_TARGET_AMOUNT_ZERO;
}

inline bool IsEnemySpellRandomizable(EnemySpellDataStruct& spell, vector<EnemySequenceCodeLine>& sequence, uint16_t* modelid = NULL, Spell_Target_Amount* ta = NULL) {
	if (spell.effect==0 || spell.effect==64 || spell.effect==10 || spell.effect==12 || spell.effect==13 || spell.effect==30 || spell.effect==37 || spell.effect==41 || spell.effect==43 || spell.effect==44 || spell.effect==45 || spell.effect==47 || spell.effect==52 || (spell.effect>=55 && spell.effect<=58) || spell.effect==96 || spell.effect==103 || spell.effect==106)
		return false;
	int seqcode, codearg;
	EnemyDataSet::GetSpellSequenceModelRef(sequence, &seqcode, &codearg);
	if (seqcode<0 || codearg<0)
		return false;
	Spell_Target_Amount targetamount = GetEnemySpellTargetAmountFromModel(sequence[seqcode].arg[codearg]);
	if (targetamount==SPELL_TARGET_AMOUNT_ZERO)
		return false;
	if (modelid)
		*modelid = sequence[seqcode].arg[codearg];
	if (ta)
		*ta = targetamount;
	return true;
}

inline bool ReplacePartySpell(SpellDataStruct* base, SpellDataStruct& copy) {
	bool ok = base->SetName(copy.name)==0;
	ok = base->SetHelp(copy.help)==0 || ok;
	base->accuracy = copy.accuracy;
	base->effect = copy.effect;
	base->element = copy.element;
	base->flag = copy.flag;
	base->help_size_x = copy.help_size_x;
	base->menu_flag = copy.menu_flag;
	base->model = copy.model;
	base->model_alt = copy.model_alt;
	base->mp = copy.mp;
	base->perform_name = copy.perform_name;
	base->power = copy.power;
	base->status = copy.status;
	base->target_flag = copy.target_flag;
	base->target_type = copy.target_type;
	return ok;
}

inline bool ReplaceEnemySpell(EnemySpellDataStruct* base, EnemySpellDataStruct& copy) {
	bool ok = base->SetName(copy.name)==0;
	base->accuracy = copy.accuracy;
	base->effect = copy.effect;
	base->element = copy.element;
	base->flag = copy.flag;
	base->model = copy.model;
	base->mp = copy.mp;
	base->power = copy.power;
	base->status = copy.status;
	base->target_flag = copy.target_flag;
	base->target_type = copy.target_type;
	base->unknown3 = copy.unknown3;
	base->unknown4 = copy.unknown4;
	return ok;
}

const static wstring TypicalStatusName[][STEAM_LANGUAGE_AMOUNT] = {
	{ L"Petrify", L"Petrify", L"石化", L"Stein", L"Fossile", L"Pietra", L"Piedra" },
	{ L"Venom", L"Venom", L"猛毒", L"Toxitus", L"Toxique", L"Fiele", L"Veneno+" },
	{ L"Virus", L"Virus", L"ウイルス", L"Infektion", L"Virus", L"Virus", L"Virus" },
	{ L"Silence", L"Silence", L"沈黙", L"Schweigen", L"Mutisme", L"Silenzio", L"Mudez" },
	{ L"Darkness", L"Darkness", L"暗闇", L"Blind", L"Obscurité", L"Blind", L"Ceguera" },
	{ L"Trouble", L"Trouble", L"迷惑", L"Neurose", L"Embrouilles", L"Disturbo", L"Molestia" },
	{ L"Zombie", L"Zombie", L"ゾンビ", L"Zombie", L"Zombie", L"Zombie", L"Zombi" },
	{ L"???", L"???", L"???", L"???", L"???", L"???", L"???" }, // Easy Kill
	{ L"KO", L"KO", L"戦闘不能", L"Kampfunfähigkeit", L"KO", L"K.O.", L"K. O." },
	{ L"???", L"???", L"???", L"???", L"???", L"???", L"???" }, // Low HP
	{ L"Confuse", L"Confuse", L"混乱", L"Konfus", L"Confusion", L"Confusione", L"Confusión" },
	{ L"Berserk", L"Berserk", L"バーサク", L"Tobsucht", L"Furie", L"Berserk", L"Locura" },
	{ L"Stop", L"Stop", L"ストップ", L"Stop", L"Stop", L"Stop", L"Paro" },
	{ L"Auto-Life", L"Auto-Life", L"リレイズ", L"Wiedergeburt", L"Pakaho", L"Risveglio", L"AutoLázaro" },
	{ L"Trance", L"Trance", L"トランス", L"Trance", L"Transe", L"Trance", L"Trance" },
	{ L"Defend", L"Defend", L"防御", L"Abwehr", L"Défense", L"Difesa", L"Defensa" },
	{ L"Poison", L"Poison", L"毒", L"Gift", L"Poison", L"Veleno", L"Veneno" },
	{ L"Sleep", L"Sleep", L"睡眠", L"Schlaf", L"Morphée", L"Sonno", L"Sueño" },
	{ L"Regen", L"Regen", L"リジェネ", L"Regena", L"Récup", L"Rigene", L"Revitalia" },
	{ L"Haste", L"Haste", L"ヘイスト", L"Hast", L"Booster", L"Haste", L"Prisa" },
	{ L"Slow", L"Slow", L"スロウ", L"Gemach", L"Somni", L"Lentezza", L"Freno" },
	{ L"Float", L"Float", L"レビテト", L"Levitas", L"Lévitation", L"Levita", L"Lévita" },
	{ L"Shell", L"Shell", L"シェル", L"Shell", L"Blindage", L"Shell", L"Escudo" },
	{ L"Protect", L"Protect", L"プロテス", L"Protes", L"Carapace", L"Protect", L"Coraza" },
	{ L"Heat", L"Heat", L"ヒート", L"Glut", L"Chaleur", L"Caldo", L"Ardor" },
	{ L"Freeze", L"Freeze", L"フリーズ", L"Frost", L"Gel", L"Freddo", L"Gélido" },
	{ L"Vanish", L"Vanish", L"バニシュ", L"Verschwinden", L"Invisibilité", L"Vanish", L"Invisible" },
	{ L"Doom", L"Doom", L"死の宣告", L"Todesurteil", L"Châtiment", L"Sentenza", L"Condena" },
	{ L"Mini", L"Mini", L"ミニマム", L"Wicht", L"Minimum", L"Minimo", L"Mínimo" },
	{ L"Reflect", L"Reflect", L"リフレク", L"Reflek", L"Boomerang", L"Reflex", L"Espejo" },
	{ L"???", L"???", L"???", L"???", L"???", L"???", L"???" }, // Jump
	{ L"Gradual Petrify", L"Gradual Petrify", L"徐々に石化", L"Gips", L"Pétra", L"Pietrificazione", L"→Piedra" }
};

const static wstring TypicalElementName[][STEAM_LANGUAGE_AMOUNT] = {
	{ L"Fire", L"Fire", L"火", L"Hitze", L"Feu", L"Fuoco", L"Fuego" },
	{ L"Ice", L"Ice", L"冷", L"Kälte", L"Glace", L"Gelo", L"Frío" },
	{ L"Thunder", L"Thunder", L"雷", L"Donner", L"Tonnerre", L"Tuono", L"Rayo" },
	{ L"Earth", L"Earth", L"土", L"Erde", L"Terre", L"Terra", L"Tierra" },
	{ L"Water", L"Water", L"水", L"Wasser", L"Eau", L"Acqua", L"Agua" },
	{ L"Wind", L"Wind", L"風", L"Wind", L"Vent", L"Vento", L"Aire" },
	{ L"Holy", L"Holy", L"聖", L"Himmel", L"Sacré", L"Sacro", L"Sacro" },
	{ L"Shadow", L"Shadow", L"闇", L"Schatten", L"Ténèbres", L"Buio", L"Sombra" }
};

const static wstring TypicalNonElementName[STEAM_LANGUAGE_AMOUNT] = {
	L"Non-elemental", L"Non-elemental", L"無", L"Neutral", L"Neutre", L"Non-elementali", L"no elemental"
};

inline void ReplaceStatusOrElementName(int status_element, FF9String& base, uint32_t statellist, uint32_t newstatellist, SpellDataStruct* spdata = NULL) {
	unsigned int i, j, tokenamount = status_element==0 ? STATUS_AMOUNT : 8;
	SteamLanguage lang, foundlang;
	wstring basestr;
	size_t pos, len;
	for (lang=0; lang<STEAM_LANGUAGE_AMOUNT; lang++) {
		if (GetGameType()==GAME_TYPE_PSX) {
			basestr = base.str;
			lang = STEAM_LANGUAGE_AMOUNT;
		} else if (lang==GetSteamLanguage()) {
			basestr = base.str;
		} else if (base.multi_lang_init[lang]) {
			basestr = base.multi_lang_str[lang];
		} else {
			continue;
		}
		foundlang = lang;
		j = 0;
		for (i=0; i<tokenamount; i++)
			if ((statellist & (1 << i)) && !(newstatellist & (1 << i))) {
				if (GetGameType()==GAME_TYPE_PSX) {
					for (foundlang=0; foundlang<STEAM_LANGUAGE_AMOUNT; foundlang++) {
						pos = basestr.find(status_element==0 ? TypicalStatusName[i][foundlang] : TypicalElementName[i][foundlang]);
						if (pos!=wstring::npos) {
							len = (status_element==0 ? TypicalStatusName[i][foundlang].length() : TypicalElementName[i][foundlang].length());
							break;
						}
					}
				} else {
					pos = basestr.find(status_element==0 ? TypicalStatusName[i][foundlang] : TypicalElementName[i][foundlang]);
					len = (status_element==0 ? TypicalStatusName[i][foundlang].length() : TypicalElementName[i][foundlang].length());
				}
				if (pos==wstring::npos && status_element==1) {
					if (GetGameType()==GAME_TYPE_PSX) {
						for (foundlang=0; foundlang<STEAM_LANGUAGE_AMOUNT; foundlang++) {
							pos = basestr.find(TypicalNonElementName[foundlang]);
							if (pos!=wstring::npos) {
								len = TypicalNonElementName[foundlang].length();
								break;
							}
						}
					} else {
						pos = basestr.find(TypicalNonElementName[foundlang]);
						len = TypicalNonElementName[foundlang].length();
					}
				}
				if (pos==wstring::npos) {
					pos = basestr.find(L"???");
					len = 3;
				}
				if (pos!=wstring::npos) {
					while (j<tokenamount) {
						if ((newstatellist & (1 << j)) && !(statellist & (1 << j))) {
							basestr.replace(pos, len, status_element==0 ? TypicalStatusName[j][foundlang] : TypicalElementName[j][foundlang]);
							break;
						}
						j++;
					}
					if (j==tokenamount)
						basestr.replace(pos, len, L"???");
					else
						j++;
				}
			}
		if (GetGameType()==GAME_TYPE_PSX || lang==GetSteamLanguage()) {
			if (spdata)
				spdata->SetHelp(basestr);
			else
				base.SetValue(basestr);
		} else {
			base.SetValue(basestr, lang);
		}
	}
}

template<typename T>
inline T PickRandomInVector(vector<T>& v) {
	int pos = GetRandom(0, v.size());
	T res = v[pos];
	v.erase(v.begin()+pos);
	return res;
}

//=====================================//
//              Safe List              //
//=====================================//

class RandomizerListDialog : public RandomizerListWindow {
public:
	struct RLD_AbilityRegistered {
		uint8_t id;
		bool safe[5];
		RLD_AbilityRegistered(uint8_t abil) : id(abil) {
			for (unsigned int i=0; i<5; i++) safe[i] = false;
		}
	};
	vector<RLD_AbilityRegistered> list;

	ToolRandomizer* parent;
	vector<uint8_t>* safelist[5];

	wxString GetAbilName(uint8_t abil) {
		if (abil<SPELL_AMOUNT && parent->cddata->spellloaded)
			return _(parent->cddata->spellset.spell[abil].name.GetStr(hades::TEXT_PREVIEW_TYPE));
		if (abil>=SPELL_AMOUNT && parent->cddata->supportloaded)
			return _(parent->cddata->supportset.support[abil-SPELL_AMOUNT].name.GetStr(hades::TEXT_PREVIEW_TYPE));
		return wxString::Format(wxT("%i"), abil);
	}

	void UpdateDisplay() {
		int selsmall = m_safelist->GetSelection();
		if (selsmall==wxNOT_FOUND) {
			m_scramble->Enable(false);
			m_spell->Enable(false);
			m_support->Enable(false);
			m_weap->Enable(false);
			m_armor->Enable(false);
			m_scramble->SetValue(false);
			m_spell->SetValue(false);
			m_support->SetValue(false);
			m_weap->SetValue(false);
			m_armor->SetValue(false);
			return;
		}
		m_scramble->Enable(true);
		m_spell->Enable(true);
		m_support->Enable(true);
		m_weap->Enable(true);
		m_armor->Enable(true);
		m_scramble->SetValue(list[selsmall].safe[0]);
		m_spell->SetValue(list[selsmall].safe[1]);
		m_support->SetValue(list[selsmall].safe[2]);
		m_weap->SetValue(list[selsmall].safe[3]);
		m_armor->SetValue(list[selsmall].safe[4]);
	}

	RandomizerListDialog(ToolRandomizer* p) : RandomizerListWindow(p), parent(p) {
		unsigned int i, j, safei;
		bool addnew;
		safelist[0] = &parent->safe_abil_scramble;
		safelist[1] = &parent->safe_abil_spell;
		safelist[2] = &parent->safe_abil_support;
		safelist[3] = &parent->safe_abil_weap;
		safelist[4] = &parent->safe_abil_armor;
		for (i=0; i<SPELL_AMOUNT+SUPPORT_AMOUNT; i++)
			m_fulllist->Append(GetAbilName(i));
		for (safei=0; safei<5; safei++)
			for (i=0; i<safelist[safei]->size(); i++) {
				addnew = true;
				for (j=0; j<list.size(); j++)
					if ((*safelist[safei])[i]==list[j].id) {
						list[j].safe[safei] = true;
						addnew = false;
						break;
					}
				if (addnew) {
					m_safelist->Append(GetAbilName((*safelist[safei])[i]));
					list.push_back(RLD_AbilityRegistered((*safelist[safei])[i]));
					list.back().safe[safei] = true;
				}
			}
		UpdateDisplay();
	}

	void AddAbility(uint8_t abil, unsigned int pos) {
		unsigned int i, safei;
		for (i=0; i<list.size(); i++)
			if (abil==list[i].id) {
				for (safei=0; safei<5; safei++)
					list[i].safe[safei] = true;
				m_safelist->SetSelection(i);
				UpdateDisplay();
				return;
			}
		m_safelist->Insert(GetAbilName(abil), pos);
		list.insert(list.begin()+pos, RLD_AbilityRegistered(abil));
		for (safei=0; safei<5; safei++)
			list[pos].safe[safei] = true;
		m_safelist->SetSelection(pos);
		UpdateDisplay();
	}

private:
	void OnCheckBox(wxCommandEvent& event) {
		int id = event.GetId();
		int selsmall = m_safelist->GetSelection();
		bool val = event.IsChecked();
		if (selsmall==wxNOT_FOUND)
			return;
		if (id==wxID_SPELLSTAT)
			list[selsmall].safe[0] = val;
		else if (id==wxID_SPELL)
			list[selsmall].safe[1] = val;
		else if (id==wxID_SUPPORT)
			list[selsmall].safe[2] = val;
		else if (id==wxID_WEAPON)
			list[selsmall].safe[3] = val;
		else if (id==wxID_ARMOR)
			list[selsmall].safe[4] = val;
		UpdateDisplay();
	}

	void OnButtonClick(wxCommandEvent& event) {
		int id = event.GetId();
		if (id==wxID_ADD) {
			int selfull = m_fulllist->GetSelection();
			if (selfull==wxNOT_FOUND)
				return;
			int selsmall = m_safelist->GetSelection();
			if (selsmall==wxNOT_FOUND)
				AddAbility(selfull, m_safelist->GetCount());
			else
				AddAbility(selfull, selsmall);
		} else if (id==wxID_REMOVE) {
			int selsmall = m_safelist->GetSelection();
			if (selsmall!=wxNOT_FOUND) {
				m_safelist->Delete(selsmall);
				list.erase(list.begin()+selsmall);
				m_safelist->SetSelection(wxNOT_FOUND);
				UpdateDisplay();
			}
		} else {
			EndModal(id);
		}
	}

	void OnListDClick(wxCommandEvent& event) {
		int id = event.GetId();
		int sel = event.GetSelection();
		if (id==wxID_FULLLIST) {
			int selsmall = m_safelist->GetSelection();
			if (selsmall==wxNOT_FOUND)
				AddAbility(sel, m_safelist->GetCount());
			else
				AddAbility(sel, selsmall);
		} else if (id==wxID_USERLIST) {
			m_safelist->Delete(sel);
			list.erase(list.begin()+sel);
			m_safelist->SetSelection(wxNOT_FOUND);
			UpdateDisplay();
		}
	}

	void OnList(wxCommandEvent& event) {
		UpdateDisplay();
	}
};

//=====================================//
//             Main Frame              //
//=====================================//

ToolRandomizer::ToolRandomizer(wxWindow* p) : RandomizerWindow(p),
	used_seed(-1),
	safe_abil_scramble({ 173, 174, 175 }),
	safe_abil_spell({ 50, 52, 54, 56, 57, 59, 61, 63, 65, 75, 76, 189, 191, 82, 101, 109, 110, 111, 112, 113, 114, 115, 116, 148, 173, 174, 175 }),
	safe_abil_support(),
	safe_abil_weap({ 101, 148, 233 }),
	safe_abil_armor({ 101, 148, 233 }) {
	PreferencesDialog::LoadToolRandomizerConfig(this);
	if (used_seed<0 || !m_enableseed->IsChecked()) {
		SetRandomSeed(time(NULL));
		m_seedctrl->SetValue(GetRandom(0, 0x1000000));
	} else
		m_seedctrl->SetValue(used_seed & 0xFFFFFF);
}

ToolRandomizer::~ToolRandomizer() {
	PreferencesDialog::SaveToolRandomizerConfig(this);
}

int ToolRandomizer::ShowModal(CDDataStruct* data) {
	cddata = data;
	UpdateMenuAvailability();
	return RandomizerWindow::ShowModal();
}

void ToolRandomizer::UpdateMenuAvailability() {
	m_seedctrl->Enable(m_enableseed->IsChecked());
	m_charspellstatsafe->Enable(m_charspellpower->GetSelection()!=0 || m_charspellstatus->GetSelection()!=0);
	m_charmpboost->Enable(m_charspell->IsChecked());
	m_charsummon->Enable(m_charspell->IsChecked());
	m_charquinaaplabel->Enable(m_charspell->IsChecked());
	m_charquinaap->Enable(m_charspell->IsChecked());
	m_charelan->Enable(m_charspell->IsChecked());
	m_charspellsafe->Enable(m_charspell->IsChecked());
	m_charsupportsafe->Enable(m_charsupport->IsChecked());
	m_weapsafe->Enable(m_weapabil->GetSelection()!=0);
	m_weapall->Enable(m_weapabil->GetSelection()==2);
	m_weapslotlabel->Enable(m_weapabil->GetSelection()==2);
	m_weapslot->Enable(m_weapabil->GetSelection()==2);
	m_weapsupportlabel->Enable(m_weapabil->GetSelection()==2);
	m_weapsupport->Enable(m_weapabil->GetSelection()==2);
	m_armorsafe->Enable(m_armorabil->GetSelection()!=0);
	m_armorall->Enable(m_armorabil->GetSelection()==2);
	m_armorslotlabel->Enable(m_armorabil->GetSelection()==2);
	m_armorslot->Enable(m_armorabil->GetSelection()==2);
	m_armorsupportlabel->Enable(m_armorabil->GetSelection()==2);
	m_armorsupport->Enable(m_armorabil->GetSelection()==2);
	m_battleprop->Enable(m_battlespell->IsChecked());
	m_battlempfactor->Enable(m_battlemp->IsChecked());
	bool charok = cddata->spellloaded && cddata->cmdloaded && cddata->statloaded;
	m_enablechar->Enable(charok);
	if (!charok) m_enablechar->SetValue(false);
	bool weapok = cddata->itemloaded;
	m_enableweap->Enable(weapok);
	if (!weapok) m_enableweap->SetValue(false);
	bool armorok = cddata->itemloaded;
	m_enablearmor->Enable(armorok);
	if (!armorok) m_enablearmor->SetValue(false);
	bool battleok = cddata->enemyloaded;
	m_enablebattle->Enable(battleok);
	if (!battleok) m_enablebattle->SetValue(false);
	bool cardok = cddata->cardloaded && cddata->enemyloaded;
	m_enablecard->Enable(cardok);
	if (!cardok) m_enablecard->SetValue(false);
	if (!m_enablechar->IsChecked() && !m_enableweap->IsChecked() && !m_enablearmor->IsChecked() && !m_enablebattle->IsChecked() && !m_enablecard->IsChecked()) {
		m_buttonapply->Enable(false);
		m_buttonok->Enable(false);
	} else {
		m_buttonapply->Enable(true);
		m_buttonok->Enable(true);
	}
	if (charok && weapok && armorok && battleok && cardok)
		m_loadallbtn->Enable(false);
}

void ToolRandomizer::ApplyRandomizerCharacter() {
	unsigned int i,j,k,rnd;
	if (m_charspellpower->GetSelection()==1) {
		vector<uint8_t> powerlist;
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasPower(cddata->spellset.spell[i].effect) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble)))
				powerlist.push_back(cddata->spellset.spell[i].power);
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasPower(cddata->spellset.spell[i].effect) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble)))
				cddata->spellset.spell[i].power = PickRandomInVector(powerlist);
		cddata->MarkDataSpellModified();
	} else if (m_charspellpower->GetSelection()==2) {
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasPower(cddata->spellset.spell[i].effect) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble)))
				cddata->spellset.spell[i].power = GetRandom(5, 121);
		cddata->MarkDataSpellModified();
	}
	if (m_charspellstatus->GetSelection()==1) {
		vector<uint8_t> statuslist;
		uint32_t prevstatus;
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasStatus(cddata->spellset.spell[i]) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble)))
				statuslist.push_back(cddata->spellset.spell[i].status);
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasStatus(cddata->spellset.spell[i]) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble))) {
				prevstatus = cddata->spellset.status_set[cddata->spellset.spell[i].status];
				cddata->spellset.spell[i].status = PickRandomInVector(statuslist);
				ReplaceStatusOrElementName(0, cddata->spellset.spell[i].help, prevstatus, cddata->spellset.status_set[cddata->spellset.spell[i].status], &cddata->spellset.spell[i]);
			}
		cddata->MarkDataSpellModified();
	} else if (m_charspellstatus->GetSelection()==2) {
		array<uint32_t, STATUS_SET_AMOUNT> oldset;
		vector<uint8_t> validstatus;
		unsigned int newcount;
		uint32_t oldstatus;
		uint8_t newstatus;
		wstring help;
		for (i=39; i<STATUS_SET_AMOUNT; i++) { // Use defaultly unused status sets to avoid messing with items/enemy spells
			oldset[i] = cddata->spellset.status_set[i];
			cddata->spellset.status_set[i] = 0;
			newcount = 1; // approx.: [1 50%] [2 25%] [3 12.5%] etc...
			while (newcount<29 && GetRandom(0, 2))
				newcount++;
			for (j=0; j<newcount; j++) {
				newstatus = GetRandom(0, 29);
				if (newstatus>=7) newstatus++; // Easy Kill, Low HP, Jump
				if (newstatus>=9) newstatus++;
				if (newstatus>=30) newstatus++;
				cddata->spellset.status_set[i] |= (1 << newstatus);
			}
			cddata->UpdateSpellStatusName(i);
		}
		for (i=0; i<SPELL_AMOUNT; i++)
			if (SpellHasStatus(cddata->spellset.spell[i]) && (!m_charspellstatsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_scramble))) {
				oldstatus = (cddata->spellset.spell[i].status>=39 ? oldset[cddata->spellset.spell[i].status] : cddata->spellset.status_set[cddata->spellset.spell[i].status]);
				cddata->spellset.spell[i].status = GetRandom(39, STATUS_SET_AMOUNT);
				ReplaceStatusOrElementName(0, cddata->spellset.spell[i].help, oldstatus, cddata->spellset.status_set[cddata->spellset.spell[i].status], &cddata->spellset.spell[i]);
			}
		cddata->MarkDataSpellModified();
	}
	if (m_charspell->IsChecked()) {
		unsigned int spellamount, cmdi;
		vector<uint8_t> spellpoolalt;
		vector<uint8_t> spellpool;
		vector<uint8_t> flairpool;
		vector<uint8_t> elanpool;
		uint8_t cmdindex;
		bool newspell;
		if (m_charelan->IsChecked()) {
			spellamount = 0;
			if (cddata->cmdset.cmd[28].panel==COMMAND_PANEL_SPELL) spellamount = cddata->cmdset.cmd[28].spell_amount;
			for (i=0; i<spellamount; i++)
				if (cddata->cmdset.cmd[28].spell_list[i]>0 && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->cmdset.cmd[28].spell_list[i], safe_abil_spell)))
					flairpool.push_back(cddata->cmdset.cmd[28].spell_list[i]);
			spellamount = 0;
			if (cddata->cmdset.cmd[29].panel==COMMAND_PANEL_SPELL) spellamount = cddata->cmdset.cmd[29].spell_amount;
			for (i=0; i<spellamount; i++)
				if (cddata->cmdset.cmd[29].spell_list[i]>0 && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->cmdset.cmd[29].spell_list[i], safe_abil_spell)))
					elanpool.push_back(cddata->cmdset.cmd[29].spell_list[i]);
		}
		for (i=0; i<COMMAND_SET_AMOUNT; i++)
			for (cmdi=0; cmdi<4; cmdi++) {
				if (cmdi==0)		cmdindex = cddata->statset.command_list[i].first_command;
				else if (cmdi==1)	cmdindex = cddata->statset.command_list[i].second_command;
				else if (cmdi==2)	cmdindex = cddata->statset.command_list[i].first_command_trance;
				else if (cmdi==3)	cmdindex = cddata->statset.command_list[i].second_command_trance;
				spellamount = 0;
				if (cddata->cmdset.cmd[cmdindex].panel==COMMAND_PANEL_SPELL) spellamount = cddata->cmdset.cmd[cmdindex].spell_amount;
				for (j=0; j<spellamount; j++)
					if (cddata->cmdset.cmd[cmdindex].spell_list[j]>0 && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->cmdset.cmd[cmdindex].spell_list[j], safe_abil_spell))) {
						newspell = true;
						for (k=0; k<spellpool.size(); k++)
							if (spellpool[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
								newspell = false;
								break;
							}
						for (k=0; k<spellpoolalt.size(); k++)
							if (spellpoolalt[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
								newspell = false;
								break;
							}
						if (m_charelan->IsChecked())
							for (k=0; k<elanpool.size(); k++)
								if (elanpool[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
									newspell = false;
									break;
								}
						if (newspell) {
							if (m_charsummon->IsChecked() && IsSpellSummon(cddata->spellset.spell[cddata->cmdset.cmd[cmdindex].spell_list[j]]))
								spellpoolalt.push_back(cddata->cmdset.cmd[cmdindex].spell_list[j]);
							else
								spellpool.push_back(cddata->cmdset.cmd[cmdindex].spell_list[j]);
						}
					}
			}
		vector<pair<uint8_t, SpellDataStruct>> spellpoolaltdata;
		vector<pair<uint8_t, SpellDataStruct>> spellpooldata;
		pair<uint8_t, SpellDataStruct> spellpick;
		for (i=0; i<spellpool.size(); i++)
			spellpooldata.push_back(make_pair(spellpool[i], cddata->spellset.spell[spellpool[i]]));
		for (i=0; i<spellpoolalt.size(); i++)
			spellpoolaltdata.push_back(make_pair(spellpoolalt[i], cddata->spellset.spell[spellpoolalt[i]]));
		spellpool.clear();
		spellpoolalt.clear();
		for (i=0; i<COMMAND_SET_AMOUNT; i++)
			for (cmdi=0; cmdi<4; cmdi++) {
				if (cmdi==0)		cmdindex = cddata->statset.command_list[i].first_command;
				else if (cmdi==1)	cmdindex = cddata->statset.command_list[i].second_command;
				else if (cmdi==2)	cmdindex = cddata->statset.command_list[i].first_command_trance;
				else if (cmdi==3)	cmdindex = cddata->statset.command_list[i].second_command_trance;
				spellamount = 0;
				if (cddata->cmdset.cmd[cmdindex].panel==COMMAND_PANEL_SPELL) spellamount = cddata->cmdset.cmd[cmdindex].spell_amount;
				for (j=0; j<spellamount; j++)
					if (cddata->cmdset.cmd[cmdindex].spell_list[j]>0 && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->cmdset.cmd[cmdindex].spell_list[j], safe_abil_spell))) {
						newspell = true;
						for (k=0; k<spellpool.size(); k++)
							if (spellpool[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
								newspell = false;
								break;
							}
						for (k=0; k<spellpoolalt.size(); k++)
							if (spellpoolalt[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
								newspell = false;
								break;
							}
						if (m_charelan->IsChecked())
							for (k=0; k<elanpool.size(); k++)
								if (elanpool[k]==cddata->cmdset.cmd[cmdindex].spell_list[j]) {
									newspell = false;
									break;
								}
						if (newspell) {
							if (m_charsummon->IsChecked() && IsSpellSummon(cddata->spellset.spell[cddata->cmdset.cmd[cmdindex].spell_list[j]])) {
								spellpick = PickRandomInVector(spellpoolaltdata);
								spellpoolalt.push_back(cddata->cmdset.cmd[cmdindex].spell_list[j]);
							} else {
								spellpick = PickRandomInVector(spellpooldata);
								spellpool.push_back(cddata->cmdset.cmd[cmdindex].spell_list[j]);
							}
							cddata->spellset.spell[cddata->cmdset.cmd[cmdindex].spell_list[j]] = spellpick.second;
							if (cddata->cmdset.cmd[cmdindex].spell_list[j]==46) { // Meteor slot
								if (cddata->spellset.spell[46].model_alt!=143)
									cddata->spellset.spell[46].model_alt = cddata->spellset.spell[46].model;
							} else if (cddata->cmdset.cmd[cmdindex].spell_list[j]==66) { // Fenrir slot
								ReplacePartySpell(&cddata->spellset.spell[67], spellpick.second);
								ReplaceStatusOrElementName(1, cddata->spellset.spell[67].help, cddata->spellset.spell[67].element, 0x20, &cddata->spellset.spell[67]);
								cddata->spellset.spell[67].element = 0x20;
							} else if (cddata->cmdset.cmd[cmdindex].spell_list[j]==68) { // Carbuncle slot
								ReplacePartySpell(&cddata->spellset.spell[69], spellpick.second);
								ReplaceStatusOrElementName(0, cddata->spellset.spell[69].help, cddata->spellset.status_set[cddata->spellset.spell[69].status], cddata->spellset.status_set[8], &cddata->spellset.spell[69]);
								cddata->spellset.spell[69].status = 8;
								ReplacePartySpell(&cddata->spellset.spell[70], spellpick.second);
								ReplaceStatusOrElementName(0, cddata->spellset.spell[70].help, cddata->spellset.status_set[cddata->spellset.spell[70].status], cddata->spellset.status_set[6], &cddata->spellset.spell[70]);
								cddata->spellset.spell[70].status = 6;
								ReplacePartySpell(&cddata->spellset.spell[71], spellpick.second);
								ReplaceStatusOrElementName(0, cddata->spellset.spell[71].help, cddata->spellset.status_set[cddata->spellset.spell[71].status], cddata->spellset.status_set[24], &cddata->spellset.spell[71]);
								cddata->spellset.spell[71].status = 24;
							}
							if (m_charelan->IsChecked()) // Flair/Elan
								for (k=0; k<flairpool.size() && k<elanpool.size(); k++)
									if (cddata->cmdset.cmd[cmdindex].spell_list[j]==flairpool[k]) {
										cddata->spellset.spell[flairpool[k]].SetTargetAmount(SPELL_TARGET_AMOUNT_ONE);
										ReplacePartySpell(&cddata->spellset.spell[elanpool[k]], spellpick.second);
										cddata->spellset.spell[elanpool[k]].SetTargetAmount(SPELL_TARGET_AMOUNT_GROUP);
										if (GetSpellModelInfo(cddata->spellset.spell[elanpool[k]].model).target_amount==SPELL_TARGET_AMOUNT_ONE)
											cddata->spellset.spell[elanpool[k]].model = 31; // Explosion
										break;
									}
						}
					}
			}
		for (i=0; i<ABILITY_SET_CAPACITY; i++) // Quina AP
			if (cddata->statset.ability_list[5].ability[i]>0 && cddata->statset.ability_list[5].ability[i]<SPELL_AMOUNT && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->statset.ability_list[5].ability[i], safe_abil_spell)))
				cddata->statset.ability_list[5].ap_cost[i] = m_charquinaap->GetValue();
		for (i=0; i<SPELL_AMOUNT; i++)
			if (!m_charspellsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_spell)) {
				cddata->spellset.spell[i].menu_flag &= 0xFB;
				cddata->UpdateSpellName(i);
			}
		if (m_charmpboost->GetSelection()==1) {
			for (i=0; i<SPELL_AMOUNT; i++)
				if (IsSpellSummon(cddata->spellset.spell[i]) && (!m_charspellsafe->IsChecked() || IsNotSafeAbility(i, safe_abil_spell)))
					cddata->spellset.spell[i].menu_flag |= 4;
		} else if (m_charmpboost->GetSelection()==2) { // Summon command
			unsigned int spellcmdamount = 0;
			if (cddata->cmdset.cmd[16].panel==COMMAND_PANEL_NONE) spellcmdamount = 1;
			if (cddata->cmdset.cmd[16].panel==COMMAND_PANEL_SPELL) spellcmdamount = cddata->cmdset.cmd[16].spell_amount;
			for (i=0; i<spellcmdamount; i++)
				if (!m_charspellsafe->IsChecked() || IsNotSafeAbility(cddata->cmdset.cmd[16].spell_list[i], safe_abil_spell))
					cddata->spellset.spell[cddata->cmdset.cmd[16].spell_list[i]].menu_flag |= 4;
		}
		cddata->MarkDataSpellModified();
		cddata->MarkDataStatModified();
		cddata->SpellDisplayNames();
	}
	if (m_charsupport->IsChecked()) {
		vector<uint8_t> supportfulllist;
		vector<uint8_t> supportavaillist;
		vector<uint8_t> supportnewlist;
		array<uint8_t, SUPPORT_AMOUNT> apcost;
		for (i=0; i+1<SUPPORT_AMOUNT; i++)
			if (!m_charsupportsafe->IsChecked() || IsNotSafeAbility(SPELL_AMOUNT+i, safe_abil_support))
				supportfulllist.push_back(SPELL_AMOUNT+i);
		for (i=0; i<ABILITY_SET_AMOUNT; i++) {
			supportnewlist.clear();
			for (j=0; j<SUPPORT_AMOUNT; j++)
				apcost[j] = 0;
			supportavaillist = supportfulllist;
			for (j=0; j<ABILITY_SET_CAPACITY; j++)
				if (cddata->statset.ability_list[i].ability[j]>=SPELL_AMOUNT) {
					apcost[cddata->statset.ability_list[i].ability[j]-SPELL_AMOUNT] = cddata->statset.ability_list[i].ap_cost[j];
					if (!m_charsupportsafe->IsChecked() || IsNotSafeAbility(cddata->statset.ability_list[i].ability[j], safe_abil_support))
						supportnewlist.push_back(PickRandomInVector(supportavaillist));
					else
						supportnewlist.push_back(cddata->statset.ability_list[i].ability[j]);
				}
			sort(supportnewlist.begin(), supportnewlist.end());
			k = 0;
			for (j=0; j<ABILITY_SET_CAPACITY; j++)
				if (cddata->statset.ability_list[i].ability[j]>=SPELL_AMOUNT) {
					cddata->statset.ability_list[i].ability[j] = supportnewlist[k];
					if (apcost[supportnewlist[k]-SPELL_AMOUNT]>0) {
						cddata->statset.ability_list[i].ap_cost[j] = apcost[supportnewlist[k]-SPELL_AMOUNT];
					} else {
						rnd = GetRandom(0, 50); // AP cost between 5 and 95 with higher density around 50
						rnd = rnd*rnd/50;
						rnd -= rnd%5;
						cddata->statset.ability_list[i].ap_cost[j] = 50+(2*GetRandom(0, 2)-1)*rnd;
					}
					k++;
				}
		}
		cddata->MarkDataStatModified();
	}
}

void ToolRandomizer::ApplyRandomizerWeapon() {
	unsigned int i,j,k;
	if (m_weapabil->GetSelection()==1) {
		unsigned int curchari, otherchari;
		uint16_t curchar, interestchar;
		vector<uint16_t> charlist;
		vector<uint8_t> abillist;
		bool addchar;
		for (i=0; i<PLAYABLE_CHAR_AMOUNT; i++) // Handle abilities of single characters + add multi-character weapons during the process
			charlist.push_back(1 << i);
		for (curchari=0; curchari<charlist.size(); curchari++) {
			curchar = charlist[curchari];
			abillist.clear();
			for (i=0; i<ITEM_WEAPON_AMOUNT; i++)
				if (curchar==(curchar & cddata->itemset.item[i].char_availability))
					for (j=0; j<3; j++)
						if (cddata->itemset.item[i].skill[j]>0 && (!m_weapsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[i].skill[j], safe_abil_weap)))
							if (curchari<PLAYABLE_CHAR_AMOUNT && cddata->itemset.item[i].skill[j]>=SPELL_AMOUNT) {
								abillist.push_back(cddata->itemset.item[i].skill[j]);
							} else if (cddata->itemset.item[i].skill[j]<SPELL_AMOUNT) {
								interestchar = 0;
								for (k=0; k<PLAYABLE_CHAR_AMOUNT; k++)
									if (((1 << k) & cddata->itemset.item[i].char_availability) && DoesCharacterUseSpell(cddata->statset.ability_list[cddata->statset.GetCharacterCommandsId(PLAYABLE_CHAR_AMOUNT-k-1)[0]], cddata->itemset.item[i].skill[j]))
										interestchar |= 1 << k;
								if (interestchar==curchar) {
									abillist.push_back(cddata->itemset.item[i].skill[j]);
								} else if (interestchar>0) {
									addchar = true;
									for (otherchari=0; otherchari<charlist.size(); otherchari++)
										if (interestchar==charlist[otherchari]) {
											addchar = false;
											break;
										}
									if (addchar)
										charlist.push_back(interestchar);
								}
							}
			for (i=0; i<ITEM_WEAPON_AMOUNT; i++)
				if (curchar==(curchar & cddata->itemset.item[i].char_availability))
					for (j=0; j<3; j++)
						if (cddata->itemset.item[i].skill[j]>0 && (!m_weapsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[i].skill[j], safe_abil_weap)))
							if (curchari<PLAYABLE_CHAR_AMOUNT && cddata->itemset.item[i].skill[j]>=SPELL_AMOUNT) {
								cddata->itemset.item[i].skill[j] = PickRandomInVector(abillist);
							} else if (cddata->itemset.item[i].skill[j]<SPELL_AMOUNT) {
								interestchar = 0;
								for (k=0; k<PLAYABLE_CHAR_AMOUNT; k++)
									if (((1 << k) & cddata->itemset.item[i].char_availability) && DoesCharacterUseSpell(cddata->statset.ability_list[cddata->statset.GetCharacterCommandsId(PLAYABLE_CHAR_AMOUNT-k-1)[0]], cddata->itemset.item[i].skill[j]))
										interestchar |= 1 << k;
								if (interestchar==curchar)
									cddata->itemset.item[i].skill[j] = PickRandomInVector(abillist);
							}
		}
		cddata->MarkDataItemModified();
	} else if (m_weapabil->GetSelection()==2) {
		vector<uint8_t> availableslotlist;
		vector<uint8_t> charspelllist;
		vector<uint8_t> charsupplist;
		vector<uint8_t> charweaplist;
		uint8_t slotpick, abilpick;
		bool addspell;
		for (i=0; i<ITEM_WEAPON_AMOUNT; i++)
			for (j=0; j<3; j++)
				if (!m_weapsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[i].skill[j], safe_abil_weap))
					cddata->itemset.item[i].skill[j] = 0;
		for (i=0; i<PLAYABLE_CHAR_AMOUNT; i++) {
			availableslotlist.clear();
			charspelllist.clear();
			charsupplist.clear();
			charweaplist.clear();
			AbilitySetDataStruct& abil = cddata->statset.ability_list[cddata->statset.GetCharacterCommandsId(i)[0]];
			for (j=0; j<ABILITY_SET_CAPACITY; j++)
				if (abil.ability[j]>0 && (!m_weapsafe->IsChecked() || IsNotSafeAbility(abil.ability[j], safe_abil_weap))) {
					if (abil.ability[j]<SPELL_AMOUNT)
						charspelllist.push_back(abil.ability[j]);
					else
						charsupplist.push_back(abil.ability[j]);
				}
			if (charspelllist.size()==0 && charsupplist.size()==0)
				continue;
			for (j=0; j<ITEM_WEAPON_AMOUNT; j++)
				if (cddata->itemset.item[j].char_availability & (1 << (PLAYABLE_CHAR_AMOUNT-i-1))) {
					charweaplist.push_back(j);
					for (k=0; k<3; k++)
						if (cddata->itemset.item[j].skill[k]==0)
							availableslotlist.push_back(j);
				}
			if (charweaplist.size()==0)
				continue;
			if (m_weapall->IsChecked() && charspelllist.size()>0)
				for (j=0; j<charspelllist.size() && availableslotlist.size()>0; j++) {
					addspell = true;
					for (slotpick=0; slotpick<charweaplist.size() && addspell; slotpick++)
						for (k=0; k<3; k++)
							if (charsupplist[j]==cddata->itemset.item[charweaplist[slotpick]].skill[k]) {
								addspell = false;
								break;
							}
					if (!addspell)
						continue;
					slotpick = PickRandomInVector(availableslotlist);
					for (k=0; k<3; k++)
						if (cddata->itemset.item[slotpick].skill[k]==0) {
							cddata->itemset.item[slotpick].skill[k] = charspelllist[j];
							break;
						}
				}
			while (100*availableslotlist.size()>3*(100-m_weapslot->GetValue())*charweaplist.size()) {
				slotpick = PickRandomInVector(availableslotlist);
				for (k=0; k<3; k++)
					if (cddata->itemset.item[slotpick].skill[k]==0) {
						if (charspelllist.size()==0 || (charsupplist.size()>0 && GetRandom(0,100)<m_weapsupport->GetValue()))
							abilpick = charsupplist[GetRandom(0, charsupplist.size())];
						else
							abilpick = charspelllist[GetRandom(0, charspelllist.size())];
						cddata->itemset.item[slotpick].skill[k] = abilpick;
						break;
					}
			}
		}
		cddata->MarkDataItemModified();
	}
}

void ToolRandomizer::ApplyRandomizerArmor() {
	unsigned int i,j,k;
	if (m_armorabil->GetSelection()==1) {
		vector<uint8_t> abillist;
		for (i=0; i<ITEM_ARMOR_AMOUNT+ITEM_USABLE_AMOUNT; i++)
			for (j=0; j<3; j++)
				if (cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j]>0 && (!m_armorsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j], safe_abil_armor)))
					abillist.push_back(cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j]);
		for (i=0; i<ITEM_ARMOR_AMOUNT+ITEM_USABLE_AMOUNT; i++)
			for (j=0; j<3; j++)
				if (cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j]>0 && (!m_armorsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j], safe_abil_armor)))
					cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j] = PickRandomInVector(abillist);
		cddata->MarkDataItemModified();
	} else if (m_armorabil->GetSelection()==2) {
		vector<uint8_t> availableslotlist;
		vector<uint8_t> chararmorlist;
		vector<uint8_t> charspelllist;
		vector<uint8_t> charsupplist;
		uint8_t slotpick, abilpick;
		bool addsupp;
		for (i=0; i<ITEM_ARMOR_AMOUNT+ITEM_USABLE_AMOUNT; i++)
			for (j=0; j<3; j++)
				if (!m_armorsafe->IsChecked() || IsNotSafeAbility(cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j], safe_abil_armor))
					cddata->itemset.item[ITEM_WEAPON_AMOUNT+i].skill[j] = 0;
		for (i=0; i<PLAYABLE_CHAR_AMOUNT; i++) { // ToDo: Looping over "char_availability" instead of characters may be a better randomization in some cases...
			availableslotlist.clear();
			chararmorlist.clear();
			charspelllist.clear();
			charsupplist.clear();
			AbilitySetDataStruct& abil = cddata->statset.ability_list[cddata->statset.GetCharacterCommandsId(i)[0]];
			for (j=0; j<ABILITY_SET_CAPACITY; j++)
				if (abil.ability[j]>0 && (!m_armorsafe->IsChecked() || IsNotSafeAbility(abil.ability[j], safe_abil_armor))) {
					if (abil.ability[j]<SPELL_AMOUNT)
						charspelllist.push_back(abil.ability[j]);
					else
						charsupplist.push_back(abil.ability[j]);
				}
			if (charspelllist.size()==0 && charsupplist.size()==0)
				continue;
			for (j=0; j<ITEM_ARMOR_AMOUNT+ITEM_USABLE_AMOUNT; j++)
				if (cddata->itemset.item[ITEM_WEAPON_AMOUNT+j].char_availability & (1 << (PLAYABLE_CHAR_AMOUNT-i-1))) {
					chararmorlist.push_back(ITEM_WEAPON_AMOUNT+j);
					for (k=0; k<3; k++)
						if (cddata->itemset.item[ITEM_WEAPON_AMOUNT+j].skill[k]==0)
							availableslotlist.push_back(ITEM_WEAPON_AMOUNT+j);
				}
			if (chararmorlist.size()==0)
				continue;
			if (m_armorall->IsChecked() && charspelllist.size()>0)
				for (j=0; j<charsupplist.size() && availableslotlist.size()>0; j++) {
					addsupp = true;
					for (slotpick=0; slotpick<chararmorlist.size() && addsupp; slotpick++)
						for (k=0; k<3; k++)
							if (charsupplist[j]==cddata->itemset.item[chararmorlist[slotpick]].skill[k]) {
								addsupp = false;
								break;
							}
					if (!addsupp)
						continue;
					slotpick = PickRandomInVector(availableslotlist);
					for (k=0; k<3; k++)
						if (cddata->itemset.item[slotpick].skill[k]==0) {
							cddata->itemset.item[slotpick].skill[k] = charsupplist[j];
							break;
						}
				}
			while (100*availableslotlist.size()>3*(100-m_armorslot->GetValue())*chararmorlist.size()) {
				slotpick = PickRandomInVector(availableslotlist);
				for (k=0; k<3; k++)
					if (cddata->itemset.item[slotpick].skill[k]==0) {
						if (charspelllist.size()==0 || (charsupplist.size()>0 && GetRandom(0,100)<m_armorsupport->GetValue()))
							abilpick = charsupplist[GetRandom(0, charsupplist.size())];
						else
							abilpick = charspelllist[GetRandom(0, charspelllist.size())];
						cddata->itemset.item[slotpick].skill[k] = abilpick;
						break;
					}
			}
		}
		cddata->MarkDataItemModified();
	}
}

void ToolRandomizer::ApplyRandomizerBattle() {
	unsigned int i,j,k;
	if (m_battlespell->IsChecked()) {
		vector<tuple<unsigned int, EnemySpellDataStruct*, EnemySpellDataStruct, uint16_t>> swaplist;
		vector<vector<bool>> spelldone(cddata->enemyset.battle_amount);
		vector<pair<EnemySpellDataStruct, uint16_t>> spelllistsingle;
		vector<pair<EnemySpellDataStruct, uint16_t>> spelllistmulti;
		pair<EnemySpellDataStruct, uint16_t> curspell;
		Spell_Target_Amount targamount;
		int seqcode, codearg;
		uint16_t spellmodel;
		EnemySpellDataStruct** similarspells;
		unsigned int* similarbattleid;
		unsigned int similaramount;
		for (i=0; i<cddata->enemyset.battle_amount; i++)
			spelldone[i] = vector<bool>(cddata->enemyset.battle[i]->spell_amount, false);
		for (i=0; i<cddata->enemyset.battle_amount; i++)
			for (j=0; j<cddata->enemyset.battle[i]->spell_amount; j++)
				if (!spelldone[i][j] && IsEnemySpellRandomizable(cddata->enemyset.battle[i]->spell[j], cddata->enemyset.battle_data[i]->sequence_code[j], &spellmodel, &targamount)) {
					if (targamount==SPELL_TARGET_AMOUNT_ONE)
						spelllistsingle.push_back(make_pair(cddata->enemyset.battle[i]->spell[j], spellmodel));
					else
						spelllistmulti.push_back(make_pair(cddata->enemyset.battle[i]->spell[j], spellmodel));
					similarspells = cddata->enemyset.GetSimilarEnemySpells(cddata->enemyset.battle[i]->spell[j], &similaramount, &similarbattleid);
					for (k=0; k<similaramount; k++)
						spelldone[similarbattleid[k]][similarspells[k]->id] = true;
				}
		for (i=0; i<cddata->enemyset.battle_amount; i++)
			for (j=0; j<cddata->enemyset.battle[i]->spell_amount; j++)
				spelldone[i][j] = false;
		if (m_battleprop->IsChecked()) {
			for (i=0; i<cddata->enemyset.battle_amount; i++)
				for (j=0; j<cddata->enemyset.battle[i]->spell_amount; j++)
					if (!spelldone[i][j] && IsEnemySpellRandomizable(cddata->enemyset.battle[i]->spell[j], cddata->enemyset.battle_data[i]->sequence_code[j], &spellmodel, &targamount)) {
						if (targamount==SPELL_TARGET_AMOUNT_ONE)
							curspell = PickRandomInVector(spelllistsingle);
						else
							curspell = PickRandomInVector(spelllistmulti);
						similarspells = cddata->enemyset.GetSimilarEnemySpells(cddata->enemyset.battle[i]->spell[j], &similaramount, &similarbattleid);
						for (k=0; k<similaramount; k++) {
							spelldone[similarbattleid[k]][similarspells[k]->id] = true;
							swaplist.push_back(make_tuple(similarbattleid[k], similarspells[k], curspell.first, curspell.second));
						}
					}
		} else {
			unsigned int mergecount;
			for (i=0; i<spelllistsingle.size(); i++) {
				mergecount = 1;
				for (j=i+1; j<spelllistsingle.size(); )
					if (spelllistsingle[i].second==spelllistsingle[j].second && spelllistsingle[i].first.effect==spelllistsingle[j].first.effect && spelllistsingle[i].first.element==spelllistsingle[j].first.element && spelllistsingle[i].first.status==spelllistsingle[j].first.status && spelllistsingle[i].first.name.str_nice==spelllistsingle[j].first.name.str_nice) {
						spelllistsingle[i].first.accuracy = (mergecount*spelllistsingle[i].first.accuracy+spelllistsingle[j].first.accuracy)/(mergecount+1);
						spelllistsingle[i].first.mp = (mergecount*spelllistsingle[i].first.mp+spelllistsingle[j].first.mp)/(mergecount+1);
						spelllistsingle[i].first.power = (mergecount*spelllistsingle[i].first.power+spelllistsingle[j].first.power)/(mergecount+1);
						mergecount++;
						spelllistsingle.erase(spelllistsingle.begin()+j);
					} else {
						j++;
					}
			}
			for (i=0; i<spelllistmulti.size(); i++) {
				mergecount = 1;
				for (j=i+1; j<spelllistmulti.size(); )
					if (spelllistmulti[i].second==spelllistmulti[j].second && spelllistmulti[i].first.effect==spelllistmulti[j].first.effect && spelllistmulti[i].first.element==spelllistmulti[j].first.element && spelllistmulti[i].first.status==spelllistmulti[j].first.status && spelllistmulti[i].first.name.str_nice==spelllistmulti[j].first.name.str_nice) {
						spelllistmulti[i].first.accuracy = (mergecount*spelllistmulti[i].first.accuracy+spelllistmulti[j].first.accuracy)/(mergecount+1);
						spelllistmulti[i].first.mp = (mergecount*spelllistmulti[i].first.mp+spelllistmulti[j].first.mp)/(mergecount+1);
						spelllistmulti[i].first.power = (mergecount*spelllistmulti[i].first.power+spelllistmulti[j].first.power)/(mergecount+1);
						mergecount++;
						spelllistmulti.erase(spelllistmulti.begin()+j);
					} else {
						j++;
					}
			}
			for (i=0; i<cddata->enemyset.battle_amount; i++)
				for (j=0; j<cddata->enemyset.battle[i]->spell_amount; j++)
					if (!spelldone[i][j] && IsEnemySpellRandomizable(cddata->enemyset.battle[i]->spell[j], cddata->enemyset.battle_data[i]->sequence_code[j], &spellmodel, &targamount)) {
						if (targamount==SPELL_TARGET_AMOUNT_ONE)
							curspell = spelllistsingle[GetRandom(0, spelllistsingle.size())];
						else
							curspell = spelllistmulti[GetRandom(0, spelllistmulti.size())];
						similarspells = cddata->enemyset.GetSimilarEnemySpells(cddata->enemyset.battle[i]->spell[j], &similaramount, &similarbattleid);
						for (k=0; k<similaramount; k++) {
							spelldone[similarbattleid[k]][similarspells[k]->id] = true;
							swaplist.push_back(make_tuple(similarbattleid[k], similarspells[k], curspell.first, curspell.second));
						}
					}
		}
		for (i=0; i<swaplist.size(); i++) {
			ReplaceEnemySpell(get<1>(swaplist[i]), get<2>(swaplist[i]));
			EnemyDataSet::GetSpellSequenceModelRef(cddata->enemyset.battle_data[get<0>(swaplist[i])]->sequence_code[get<1>(swaplist[i])->id], &seqcode, &codearg);
			cddata->enemyset.battle_data[get<0>(swaplist[i])]->sequence_code[get<1>(swaplist[i])->id][seqcode].arg[codearg] = get<3>(swaplist[i]);
			cddata->MarkDataEnemyModified(get<0>(swaplist[i]), CHUNK_TYPE_ENEMY_STATS);
			cddata->MarkDataEnemyModified(get<0>(swaplist[i]), CHUNK_TYPE_BATTLE_DATA);
			cddata->MarkDataEnemyModified(get<0>(swaplist[i]), CHUNK_TYPE_TEXT);
		}
	}
	if (m_battlemp->IsChecked())
		for (i=0; i<cddata->enemyset.battle_amount; i++) {
			for (j=0; j<cddata->enemyset.battle[i]->stat_amount; j++)
				cddata->enemyset.battle[i]->stat[j].mp = m_battlempfactor->GetValue()*cddata->enemyset.battle[i]->stat[j].lvl;
			cddata->MarkDataEnemyModified(i, CHUNK_TYPE_ENEMY_STATS);
		}
}

void ToolRandomizer::ApplyRandomizerCard() {
	unsigned int i,j;
	if (m_carddrop->IsChecked())
		for (i=0; i<cddata->enemyset.battle_amount; i++) {
			for (j=0; j<cddata->enemyset.battle[i]->stat_amount; j++)
				cddata->enemyset.battle[i]->stat[j].card_drop = GetRandom(0, 100);
			cddata->MarkDataEnemyModified(i, CHUNK_TYPE_ENEMY_STATS);
		}
	if (m_carddeck->GetSelection()==1) {
		uint8_t cardtmp;
		uint8_t selset;
		for (i=0; i+1<CARD_SET_AMOUNT; i++) {
			selset = GetRandom(i, CARD_SET_AMOUNT);
			for (j=0; j<CARD_SET_CAPACITY; j++) {
				cardtmp = cddata->cardset.set[i].card[j];
				cddata->cardset.set[i].card[j] = cddata->cardset.set[selset].card[j];
				cddata->cardset.set[selset].card[j] = cardtmp;
			}
		}
		cddata->MarkDataCardModified();
	} else if (m_carddeck->GetSelection()==2) {
		for (i=0; i<CARD_SET_AMOUNT; i++)
			for (j=0; j<CARD_SET_CAPACITY; j++)
				cddata->cardset.set[i].card[j] = GetRandom(0, 100);
		cddata->MarkDataCardModified();
	}
}

void ToolRandomizer::ApplyRandomizer() { // ToDo: cddata->MarkAsModified
	used_seed = m_seedctrl->GetValue();
	SetRandomSeed(used_seed);
	if (m_enablechar->IsChecked())
		ApplyRandomizerCharacter();
	if (m_enableweap->IsChecked())
		ApplyRandomizerWeapon();
	if (m_enablearmor->IsChecked())
		ApplyRandomizerArmor();
	if (m_enablebattle->IsChecked())
		ApplyRandomizerBattle();
	if (m_enablecard->IsChecked())
		ApplyRandomizerCard();
	cddata->DisplayCurrentData();
	m_seedctrl->SetValue(GetRandom(0, 0x1000000));
}

void ToolRandomizer::OnChoice(wxCommandEvent& event) {
	UpdateMenuAvailability();
}

void ToolRandomizer::OnCheckBox(wxCommandEvent& event) {
	UpdateMenuAvailability();
}

void ToolRandomizer::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_SAFESET) {
		RandomizerListDialog dial(this);
		if (dial.ShowModal()==wxID_OK) {
			unsigned int i;
			safe_abil_scramble.clear();
			safe_abil_spell.clear();
			safe_abil_support.clear();
			safe_abil_weap.clear();
			safe_abil_armor.clear();
			for (i=0; i<dial.list.size(); i++) {
				if (dial.list[i].safe[0]) safe_abil_scramble.push_back(dial.list[i].id);
				if (dial.list[i].safe[1]) safe_abil_spell.push_back(dial.list[i].id);
				if (dial.list[i].safe[2]) safe_abil_support.push_back(dial.list[i].id);
				if (dial.list[i].safe[3]) safe_abil_weap.push_back(dial.list[i].id);
				if (dial.list[i].safe[4]) safe_abil_armor.push_back(dial.list[i].id);
			}
		}
	} else if (id==wxID_LOADALL) {
		m_loadallbtn->Enable(false);
		cddata->InitSpell();
		cddata->InitCommand();
		cddata->InitStat();
		cddata->InitItem();
		cddata->InitEnemy();
		cddata->InitCard();
		if (!m_enablechar->IsEnabled()) m_enablechar->SetValue(true);
		if (!m_enableweap->IsEnabled()) m_enableweap->SetValue(true);
		if (!m_enablearmor->IsEnabled()) m_enablearmor->SetValue(true);
		if (!m_enablebattle->IsEnabled()) m_enablebattle->SetValue(true);
		if (!m_enablecard->IsEnabled()) m_enablecard->SetValue(true);
		UpdateMenuAvailability();
	} else if (id==wxID_APPLY) {
		ApplyRandomizer(); // ToDo: hidden for now; show when cddata is not modified directly and preview exists in the tool
	} else if (id==wxID_CANCEL) {
		EndModal(id);
	} else if (id==wxID_OK) {
		ApplyRandomizer();
		EndModal(id);
	}
}
