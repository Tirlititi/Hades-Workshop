#include "Gui_ScriptEditor.h"

#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include "Gui_Manipulation.h"
#include "Hades_Strings.h"
#include "Script_Strings.h"

#define SCRIPT_ID_NO_ENTRY			0xFFFF
#define POSITION_PANEL_SIZE			200
#define WORLDMAP_PANEL_WIDTH		256
#define WORLDMAP_PANEL_HEIGHT		216
#define SCRIPT_FIXED_ENTRY_AMOUNT	9

const static wxColour FUNCTION_COLOR_NORMAL(255,255,255);
const static wxColour FUNCTION_COLOR_MODIFIED(255,200,0);
const static wxColour FUNCTION_COLOR_PARSED(40,255,40);
const static wxColour FUNCTION_COLOR_FAILED(255,0,0);
const static wxColour POSITION_POINT_COLOR(0,0,255);

/*********************************************************************
 * Interpreting and displaying the game scripts is done there.
 * 
 * Here are the different structures of the code controls.
 * On the left, the binary code,
 * On the right, corresponding C-like code.
 * 
 * ************************* if/then *********************************
 * 05 Condition					* if (Condition) {
 * 02 Jump ---\					*   CODE_A()
 * CODE_A     |					* }
 *        <---/					*
 * 								*
 * *********************** if/then/else ******************************
 * 05 Condition					* if (Condition) {
 * 02 Jump ---\					*   CODE_A()
 * CODE_A     |					* } else {
 * 01 Jump ---|--\				*   CODE_B()
 * CODE_B <---/	 |  			* }
 *        <------/				*
 * 								*
 * ************************ ifnot/then *******************************
 * 03 Condition					* ifnot (Condition) {
 * 02 Jump ---\					*   CODE_A()
 * CODE_A     |					* }
 *        <---/					*
 * 								*
 * ************************** while **********************************
 * 01 Jump ----------\			* while (Condition) {
 * CODE_A  <------\   |			*   CODE_A()
 * 05 Condition <-|---/			* }
 * 03 Jump -------/				*
 * 								*
 * ************************* do/while ********************************
 * CODE_A <------\				* do {
 * 05 Condition  |				*   CODE_A()
 * 03 Jump ------/				* } while (Condition)
 * 								*
 * ************************ switch(ex) *******************************
 * 05 Condition					* switch (Condition) from X {
 * 0B Switches 0  1 DEFAULT		* case 0:
 * CODE_A <----/  |    |		*   CODE_A()
 * 01 Jump -------|----|		*   break
 * CODE_B <-------/    |		* case 1:
 * 01 Jump ------------|		*   CODE_B()
 *        <------------/		*   break
 * 								* }
 * 								*
 * ******************** switch(ex)+default ***************************
 * 05 Condition					* switch (Condition) from X {
 * 0B Switches 0  1 DEFAULT		* case 0:
 * CODE_A <----/  |    |		*   CODE_A()
 * 01 Jump -------|----|---\	*   break
 * CODE_B <-------/    |   |	* case 1:
 * 01 Jump ------------|---|	*   CODE_B()
 * CODE_C <------------/   |	*   break
 * 01 Jump ----------------|	* default:
 *        <----------------/	*   CODE_C()
 * 								*	break
 * 								* }
 * 								*
 * *************************** loop **********************************
 * FUNC_START <---\				* function {
 * CODE_A         |				*   CODE_A()
 * 01 Jump -------/				*   loop
 * 								* }
 * 								*
 *********************************************************************/

#define DEFAULT_SET_STR	L"set VAR_B1_0 = 0"
#define TAB_STR			L"    "
#define SS_ARG_ID		500
#define SS_ARGBOX_MAXID	32
#define TIMER_TIMEOUT	5

#define SCRPT_MAX_OPERAND 0x100		// Max operand amount in variable code expressions
#define SCRPT_MAX_FUNC_LINE 0x10000	// Max line amount in one function
#define SCRPT_MAX_SWITCH_CASE 0x100	// Max case amount in a switch or switchex block

#define ARG_CONTROL_TEXT		0
#define ARG_CONTROL_FLAG		1
#define ARG_CONTROL_FLAGS		2
#define ARG_CONTROL_SPIN		3
#define ARG_CONTROL_CHOICE		4
#define ARG_CONTROL_DISC		5
#define ARG_CONTROL_POSITION	6
#define ARG_CONTROL_COLOR_CMY	7
#define ARG_CONTROL_COLOR_RGB	8

//=============================//
//          Generic            //
//=============================//

void RemoveSurroundingSpaces(wxString& str) {
	size_t pos = str.find_first_not_of(" \t");
	if (pos==string::npos) {
		str = wxEmptyString;
		return;
	}
	str = str.Mid(pos);
	pos = str.find_last_of(" \t");
	if (pos!=string::npos)
		str = str.Mid(0,pos);
}

wxString GetNextWord(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(L" \t");
	if (pos==string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	res = line.Mid(pos);
	pos = res.find_first_of(L" ,():;{}\t");
	if (pos!=string::npos) {
		res = res.Mid(0,pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else
		line = wxEmptyString;
	return res;
}

wxString GetNextArg(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos==string::npos)
		return wxEmptyString;
	res = line.Mid(pos);
	unsigned int parlvl = 0;
	size_t lineshift = pos, arglen = 0;
	bool ok = false;
	wxString tmp = res;
	while (!ok) {
		pos = tmp.find_first_of(L",()");
		arglen += pos;
		if (pos==string::npos) {
			line = wxEmptyString;
			return wxEmptyString;
		} else if (tmp[pos]==L'(') {
			parlvl++;
			tmp = tmp.Mid(pos+1);
			arglen++;
			while (parlvl>0) {
				pos = tmp.find_first_of(L"()");
				if (pos==string::npos) {
					line = wxEmptyString;
					return wxEmptyString;
				} else if (tmp[pos]==L'(')
					parlvl++;
				else
					parlvl--;
				tmp = tmp.Mid(pos+1);
				arglen += pos+1;
			}
		} else
			ok = true;
	}
	res = res.Mid(0,arglen);
	res.Trim();
	lineshift += arglen;
	line = line.Mid(lineshift);
	return res;
}

wxString GetNextPunc(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos==string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	res = line.Mid(pos);
	pos = res.find_first_not_of(L",():;{}");
	if (pos!=string::npos) {
		if (pos>1)
			pos = 1;
		res = res.Mid(0,pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else if (res.Length()>1) {
		res = res.Mid(0,1);
		line = line.Mid(lineshift+1);
	} else {
		if (res.Length()>0)
			res = res.Mid(0,1);
		line = wxEmptyString;
	}
	return res;
}

wxString GetNextThing(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos==string::npos)
		return wxEmptyString;
	res = line.Mid(pos);
	static wxString punclist = L",():;{}";
	for (unsigned int i=0;i<punclist.Length();i++)
		if (res[0]==punclist[i]) {
			res = res.Mid(0,1);
			line = line.Mid(pos+1);
			return res;
		}
	size_t lineshift = pos;
	pos = res.find_first_of(L" ,():;{}\t");
	if (pos!=string::npos) {
		res = res.Mid(0,pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else
		line = wxEmptyString;
	return res;
}

ScriptEditHandler::ScriptEditHandler(ScriptDataStruct& scpt) :
	script(scpt),
	entry_selection(SCRIPT_ID_NO_ENTRY),
	handler_dialog(NULL) {
//fstream fout("aaaa.txt",ios::app|ios::out); fout << "0.3" << endl; fout.close();
	unsigned int i;
	modellist_id = new uint16_t*[G_N_ELEMENTS(HADES_STRING_MODEL_NAME)];
	modellist_str.Alloc(G_N_ELEMENTS(HADES_STRING_MODEL_NAME));
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++) {
		modellist_str.Add(_(HADES_STRING_MODEL_NAME[i].label));
		modellist_id[i] = new uint16_t(HADES_STRING_MODEL_NAME[i].id);
	}
	entry_name.Alloc(script.entry_amount); // ToDo : when entries can be added/removed
	if (script.entry_amount>0)
		entry_name.Add(_(L"Main"));
	for (i=1;i<script.entry_amount;i++)
		entry_name.Add(wxString::Format(wxT("Entry%d"),i));
//fout.open("aaaa.txt",ios::app|ios::out); fout << "0.7" << endl; fout.close();
}

ScriptEditHandler::~ScriptEditHandler() {
	unsigned int i;
	for (i=0;i<modellist_str.Count();i++)
		delete[] modellist_id[i];
	delete[] modellist_id;
}

ScriptEditDialog::ScriptEditDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td, bool* dataloaded) :
	ScriptEditWindow(parent),
	ScriptEditHandler(scpt),
	enemy(ed),
	text(td),
	datas(sv),
	script_type(scpttype),
	current_opcode(0xFFFF),
	arg_control_type(NULL),
	extra_size(script.GetExtraSize()),
	refresh_control_disable(false),
	timer(new wxTimer(this)),
	help_dial(NULL) {
//fstream fout("aaaa.txt",ios::app|ios::out); fout << "1" << endl; fout.close();
	unsigned int i;
	handler_dialog = this;
	script.Copy(scpt);
//fout.open("aaaa.txt",ios::app|ios::out); fout << "2" << endl; fout.close();
	if (script_type==SCRIPT_TYPE_FIELD) {
		gl_window = new GLWindow(m_fielddisplaypanel,NULL);
		FieldTilesDataStruct* tiles;
		FieldWalkmeshDataStruct* walk;
		if (GetGameType()==GAME_TYPE_PSX) {
			tiles = (FieldTilesDataStruct*)&script.parent_cluster->chunk[script.parent_cluster->SearchChunkType(CHUNK_TYPE_FIELD_TILES)].GetObject(0);
			walk = (FieldWalkmeshDataStruct*)&script.parent_cluster->chunk[script.parent_cluster->SearchChunkType(CHUNK_TYPE_FIELD_WALK)].GetObject(0);
		} else {
			for (i=0;i<datas->fieldset->amount;i++)
				if (scpt.object_id==datas->fieldset->script_data[i]->object_id) {
					tiles = datas->fieldset->background_data[i];
					walk = datas->fieldset->walkmesh[i];
					break;
				}
		}
		gl_window->DisplayField(tiles,walk);
	} else if (script_type==SCRIPT_TYPE_WORLD) {
		world_pos_type = 0;
	}
	m_panelbattle->Enable(script_type==SCRIPT_TYPE_BATTLE);
	m_panelfield->Enable(script_type==SCRIPT_TYPE_FIELD);
	m_panelworld->Enable(script_type==SCRIPT_TYPE_WORLD);
	for (i=1;i<script.entry_amount;i++)
		if (enemy && i<=enemy->stat_amount) {
			entry_name[i] = _(enemy->stat[i-1].name.GetStr(2));
			entry_name[i].Replace(_(L" "),_(L"_"));
		}
	DisplayFunctionList(true);
	if (dataloaded[0]) {
		character_str.Alloc(13);
		for (i=0;i<8;i++)
			character_str.Add(_(datas->statset->initial_stat[i].default_name.str));
		character_str.Add(_(datas->statset->initial_stat[11].default_name.str));
		character_str.Add(_(datas->statset->initial_stat[8].default_name.str));
		character_str.Add(_(datas->statset->initial_stat[9].default_name.str));
		character_str.Add(_(datas->statset->initial_stat[10].default_name.str));
		character_str.Add(_(HADES_STRING_NULL_CHARACTER_SLOT));
		character_id = new uint16_t*[13];
		for (i=0;i<12;i++)
			character_id[i] = new uint16_t(i);
		character_id[12] = new uint16_t(0xFF);
		use_character = true;
	} else
		use_character = false;
	if (dataloaded[1]) {
		battle_str.Alloc(datas->enemyset->battle_amount);
		battle_id = new uint16_t*[datas->enemyset->battle_amount];
		for (i=0;i<datas->enemyset->battle_amount;i++) {
			battle_str.Add(_(datas->enemyset->battle_name[i]));
			battle_id[i] = new uint16_t[1];
			battle_id[i][0] = datas->enemyset->battle_data[i]->object_id;
		}
		use_battle = true;
	} else
		use_battle = false;
	m_intvalueattack->Enable(enemy);
	m_intvalueattacklabel->Enable(enemy);
	if (enemy) {
		attack_str.Alloc(enemy->spell_amount);
		for (i=0;i<enemy->spell_amount;i++)
			attack_str.Add(_(enemy->spell[i].name.str));
		use_attack = true;
	} else
		use_attack = false;
	if (dataloaded[2]) {
		field_str.Alloc(datas->fieldset->amount);
		field_id = new uint16_t*[datas->fieldset->amount];
		for (i=0;i<datas->fieldset->amount;i++) {
			field_str.Add(_(datas->fieldset->script_data[i]->name.str));
			field_id[i] = new uint16_t[1];
			field_id[i][0] = datas->fieldset->script_data[i]->object_id;
		}
		use_field = true;
	} else
		use_field = false;
	m_intvalueitem->Enable(dataloaded[3]);
	m_intvalueitemlabel->Enable(dataloaded[3]);
	if (dataloaded[3]) {
		item_str.Alloc(ITEM_AMOUNT+KEY_ITEM_AMOUNT+CARD_AMOUNT);
		item_id = new uint16_t*[ITEM_AMOUNT+KEY_ITEM_AMOUNT+CARD_AMOUNT];
		for (i=0;i<ITEM_AMOUNT;i++) {
			item_str.Add(_(datas->itemset->item[i].name.str));
			item_id[i] = new uint16_t[1];
			item_id[i][0] = i;
		}
		for (i=0;i<KEY_ITEM_AMOUNT;i++) {
			item_str.Add(_(datas->itemset->key_item[i].name.str));
			item_id[ITEM_AMOUNT+i] = new uint16_t[1];
			item_id[ITEM_AMOUNT+i][0] = 0x100+i;
		}
		if (dataloaded[6]) {
			for (i=0;i<CARD_AMOUNT;i++) {
				item_str.Add(_(datas->cardset->card[i].name.str));
				item_id[ITEM_AMOUNT+KEY_ITEM_AMOUNT+i] = new uint16_t[1];
				item_id[ITEM_AMOUNT+KEY_ITEM_AMOUNT+i][0] = 0x200+i;
			}
		} else {
			for (i=0;i<CARD_AMOUNT;i++) {
				item_str.Add(_(HADES_STRING_CARD_NAME[i].label));
				item_id[ITEM_AMOUNT+KEY_ITEM_AMOUNT+i] = new uint16_t[1];
				item_id[ITEM_AMOUNT+KEY_ITEM_AMOUNT+i][0] = 0x200+HADES_STRING_CARD_NAME[i].id;
			}
		}
		use_item = true;
	} else
		use_item = false;
	m_intvaluespell->Enable(dataloaded[4]);
	m_intvaluespelllabel->Enable(dataloaded[4]);
	if (dataloaded[4]) {
		ability_str.Alloc(SPELL_AMOUNT);
		for (i=0;i<SPELL_AMOUNT;i++)
			ability_str.Add(_(datas->spellset->spell[i].name.str));
		use_ability = true;
	} else
		use_ability = false;
	m_intvaluecmd->Enable(dataloaded[5]);
	m_intvaluecmdlabel->Enable(dataloaded[5]);
	if (dataloaded[5]) {
		command_str.Alloc(COMMAND_AMOUNT);
		for (i=0;i+1<COMMAND_AMOUNT;i++)
			command_str.Add(_(datas->cmdset->cmd[i].name.str));
		for (i=0;i<G_N_ELEMENTS(CommandAddendaName);i++)
			command_str.Add(_(CommandAddendaName[i]));
		use_command = true;
	} else
		use_command = false;
	if (text) {
		text_str.Alloc(text->amount);
		for (i=0;i<text->amount;i++)
			text_str.Add(_(text->text[i].str_nice.substr(0,30)));
		use_text = true;
	} else
		use_text = false;
	defaultbool_str.Alloc(16);
	for (i=0;i<16;i++)
		defaultbool_str.Add(_(wxString::Format(wxT("%u"),i+1)));
	disc_str.Alloc(G_N_ELEMENTS(DiscName));
	for (i=0;i<G_N_ELEMENTS(DiscName);i++)
		disc_str.Add(_(DiscName[i]));
	menutype_str.Alloc(G_N_ELEMENTS(MenuType));
	for (i=0;i<G_N_ELEMENTS(MenuType);i++)
		menutype_str.Add(_(MenuType[i]));
	shop_str.Alloc(G_N_ELEMENTS(HADES_STRING_SHOP_NAME));
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_SHOP_NAME);i++)
		shop_str.Add(_(HADES_STRING_SHOP_NAME[i].label));
	abilityset_str.Alloc(G_N_ELEMENTS(AbilitySetName));
	for (i=0;i<G_N_ELEMENTS(AbilitySetName);i++)
		abilityset_str.Add(_(AbilitySetName[i]));
	bubblesymbol_str.Alloc(G_N_ELEMENTS(BubbleSymbolName));
	for (i=0;i<G_N_ELEMENTS(BubbleSymbolName);i++)
		bubblesymbol_str.Add(_(BubbleSymbolName[i]));
	deck_str.Alloc(G_N_ELEMENTS(HADES_STRING_DECK_NAME));
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_DECK_NAME);i++)
		deck_str.Add(_(HADES_STRING_DECK_NAME[i].label));
	equipset_id = new uint16_t*[G_N_ELEMENTS(EquipSetName)];
	equipset_str.Alloc(G_N_ELEMENTS(EquipSetName));
	for (i=0;i<G_N_ELEMENTS(EquipSetName);i++) {
		equipset_str.Add(_(EquipSetName[i].name));
		equipset_id[i] = new uint16_t(EquipSetName[i].id);
	}
	fmv_id = new uint16_t*[G_N_ELEMENTS(FMVNameList)];
	fmv_str.Alloc(G_N_ELEMENTS(FMVNameList));
	for (i=0;i<G_N_ELEMENTS(FMVNameList);i++) {
		fmv_str.Add(_(FMVNameList[i].name));
		fmv_id[i] = new uint16_t(FMVNameList[i].id);
	}
	battlecode_id = new uint16_t*[G_N_ELEMENTS(BattleCodeName)];
	battlecode_str.Alloc(G_N_ELEMENTS(BattleCodeName));
	for (i=0;i<G_N_ELEMENTS(BattleCodeName);i++) {
		battlecode_str.Add(_(BattleCodeName[i].name));
		battlecode_id[i] = new uint16_t(BattleCodeName[i].id);
	}
	modelcode_id = new uint16_t*[G_N_ELEMENTS(ModelCodeName)];
	modelcode_str.Alloc(G_N_ELEMENTS(ModelCodeName));
	for (i=0;i<G_N_ELEMENTS(ModelCodeName);i++) {
		modelcode_str.Add(_(ModelCodeName[i].name));
		modelcode_id[i] = new uint16_t(ModelCodeName[i].id);
	}
	worldcode_id = new uint16_t*[G_N_ELEMENTS(WorldCodeName)];
	worldcode_str.Alloc(G_N_ELEMENTS(WorldCodeName));
	for (i=0;i<G_N_ELEMENTS(WorldCodeName);i++) {
		worldcode_str.Add(_(WorldCodeName[i].name));
		worldcode_id[i] = new uint16_t(WorldCodeName[i].id);
	}
	soundcode_id = new uint16_t*[G_N_ELEMENTS(SoundCodeName)];
	soundcode_str.Alloc(G_N_ELEMENTS(SoundCodeName));
	for (i=0;i<G_N_ELEMENTS(SoundCodeName);i++) {
		soundcode_str.Add(_(SoundCodeName[i].name));
		soundcode_id[i] = new uint16_t(SoundCodeName[i].id);
	}
	spscode_id = new uint16_t*[G_N_ELEMENTS(SpsCodeName)];
	spscode_str.Alloc(G_N_ELEMENTS(SpsCodeName));
	for (i=0;i<G_N_ELEMENTS(SpsCodeName);i++) {
		spscode_str.Add(_(SpsCodeName[i].name));
		spscode_id[i] = new uint16_t(SpsCodeName[i].id);
	}
	worldmap_id = new uint16_t*[G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME)];
	worldmap_str.Alloc(G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME));
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME);i++) {
		worldmap_str.Add(_(HADES_STRING_WORLD_BLOCK_NAME[i].label));
		worldmap_id[i] = new uint16_t(HADES_STRING_WORLD_BLOCK_NAME[i].id);
	}
	func_popup_menu = new wxMenu();
	func_popup_menu->Append(wxID_ADD,HADES_STRING_GENERIC_ADD);
	func_popup_menu->Append(wxID_REMOVE,HADES_STRING_GENERIC_REMOVE);
	func_popup_menu->Append(wxID_SET,HADES_STRING_GENERIC_PROPERTIES);
	func_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnFunctionRightClickMenu),NULL,this);
	wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
	m_localvarshowimg->SetBitmap(wxBitmap(tmpimg));
	Connect(wxEVT_TIMER,wxTimerEventHandler(ScriptEditDialog::OnTimer),NULL,this);
}

ScriptEditDialog::~ScriptEditDialog() {
	unsigned int i;
	for (i=0;i<script.entry_amount+6;i++)
		delete[] entrylist_id[i];
	delete[] entrylist_id;
	if (use_character) {
		for (i=0;i<character_str.Count();i++)
			delete[] character_id[i];
		delete[] character_id;
	}
	if (use_battle) {
		for (i=0;i<battle_str.Count();i++)
			delete[] battle_id[i];
		delete[] battle_id;
	}
	if (use_field) {
		for (i=0;i<field_str.Count();i++)
			delete[] field_id[i];
		delete[] field_id;
	}
	if (use_item) {
		for (i=0;i<item_str.Count();i++)
			delete[] item_id[i];
		delete[] item_id;
	}
	for (i=0;i<equipset_str.Count();i++)
		delete[] equipset_id[i];
	delete[] equipset_id;
	for (i=0;i<fmv_str.Count();i++)
		delete[] fmv_id[i];
	delete[] fmv_id;
	for (i=0;i<battlecode_str.Count();i++)
		delete[] battlecode_id[i];
	delete[] battlecode_id;
	for (i=0;i<modelcode_str.Count();i++)
		delete[] modelcode_id[i];
	delete[] modelcode_id;
	for (i=0;i<worldcode_str.Count();i++)
		delete[] worldcode_id[i];
	delete[] worldcode_id;
	for (i=0;i<soundcode_str.Count();i++)
		delete[] soundcode_id[i];
	delete[] soundcode_id;
	for (i=0;i<spscode_str.Count();i++)
		delete[] spscode_id[i];
	delete[] spscode_id;
	for (i=0;i<worldmap_str.Count();i++)
		delete[] worldmap_id[i];
	delete[] worldmap_id;
	func_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnFunctionRightClickMenu),NULL,this);
	Disconnect(wxEVT_TIMER,wxTimerEventHandler(ScriptEditDialog::OnTimer),NULL,this);
	delete timer;
}

int ScriptEditDialog::ShowModal() {
	unsigned int entryid = 0;
	GenerateFunctionStrings();
	m_buttonok->SetFocus();
	while (script.entry_function_amount[entryid]==0)
		entryid++;
	DisplayFunction(entryid,0);
	timer->Start(TIMER_TIMEOUT);
	return wxDialog::ShowModal();
}

//=============================//
//        Display Code         //
//=============================//

void ScriptEditHandler::GenerateFunctionList(bool firsttime) {
	unsigned int funcamount = 0;
	unsigned int i,j;
	if (!firsttime)
		functionlist_str.Empty();
	for (i=0;i<script.entry_amount;i++)
		funcamount += script.entry_function_amount[i];
	functionlist_str.Alloc(funcamount);
	functionlist_id = new uint16_t*[funcamount];
	funcamount = 0;
	for (i=0;i<script.entry_amount;i++) {
		for (j=0;j<script.entry_function_amount[i];j++) {
			functionlist_id[funcamount] = new uint16_t(script.function_type[i][j]);
			if (script.function_type[i][j]<G_N_ELEMENTS(FunctionTypeName))
				functionlist_str.Add(_(L"Function ")+wxString::Format(wxT("%s_%s"),entry_name[i],FunctionTypeName[script.function_type[i][j]]));
			else
				functionlist_str.Add(_(L"Function ")+wxString::Format(wxT("%s_%u"),entry_name[i],script.function_type[i][j]));
			funcamount++;
		}
	}
}

void ScriptEditDialog::DisplayFunctionList(bool firsttime, int newfunc, int removedfunc) {
	unsigned int funcamount = 0;
	unsigned int i,j;
	GenerateFunctionList(firsttime);
	if (!firsttime) {
		delete[] entrylist_id;
		delete[] functionlist_id;
		entrylist_str.Empty();
	}
	entry_selection = SCRIPT_ID_NO_ENTRY;
	entrylist_str.Alloc(script.entry_amount+6);
	entrylist_id = new uint16_t*[script.entry_amount+6];
	for (i=0;i<script.entry_amount;i++) {
		entrylist_str.Add(_(L"Entry ")+wxString::Format(wxT("%s"),entry_name[i]));
		entrylist_id[i] = new uint16_t(i);
		funcamount += script.entry_function_amount[i];
	}
	entrylist_str.Add(_(L"This"));
	entrylist_id[i++] = new uint16_t(0xFF);
	entrylist_str.Add(_(L"Player Character"));
	entrylist_id[i++] = new uint16_t(0xFA);
	entrylist_str.Add(_(L"Team Character 1"));
	entrylist_id[i++] = new uint16_t(0xFB);
	entrylist_str.Add(_(L"Team Character 2"));
	entrylist_id[i++] = new uint16_t(0xFC);
	entrylist_str.Add(_(L"Team Character 3"));
	entrylist_id[i++] = new uint16_t(0xFD);
	entrylist_str.Add(_(L"Team Character 4"));
	entrylist_id[i++] = new uint16_t(0xFE);
	if (!firsttime)
		m_functionlist->ClearAll();
	m_functionlist->AppendColumn(_(L"Functions"),wxLIST_FORMAT_LEFT,202);
	bool* newshouldparse;
	if (firsttime)
		func_should_parse = new bool[funcamount];
	else if (newfunc>=0)
		newshouldparse = new bool[funcamount];
	else
		newshouldparse = new bool[funcamount];
	funcamount = 0;
	for (i=0;i<script.entry_amount;i++) {
		for (j=0;j<script.entry_function_amount[i];j++) {
			m_functionlist->InsertItem(funcamount,functionlist_str[funcamount]);
			if (firsttime)
				func_should_parse[funcamount] = false;
			else if (newfunc>=0) {
				if (funcamount<newfunc)
					newshouldparse[funcamount] = func_should_parse[funcamount];
				else if (funcamount==newfunc)
					newshouldparse[funcamount] = false;
				else
					newshouldparse[funcamount] = func_should_parse[funcamount-1];
				if (newshouldparse[funcamount])
					m_functionlist->SetItemTextColour(funcamount,FUNCTION_COLOR_MODIFIED);
			} else {
				if (funcamount<removedfunc)
					newshouldparse[funcamount] = func_should_parse[funcamount];
				else
					newshouldparse[funcamount] = func_should_parse[funcamount+1];
				if (newshouldparse[funcamount])
					m_functionlist->SetItemTextColour(funcamount,FUNCTION_COLOR_MODIFIED);
			}
			funcamount++;
		}
	}
	if (!firsttime) {
		delete[] func_should_parse;
		func_should_parse = newshouldparse;
	}
}

wxString operandtmp[SCRPT_MAX_OPERAND];
unsigned int funcpostrack[SCRPT_MAX_FUNC_LINE];
unsigned int functrackline;
// varvoidcounter's purpose is solely to avoid adding a "break" in the functions that lead to World Maps
// In those, there's a "set Global_ScenarioCounter" dummy line followed by another "JUMP 0" dummy line not to be turned into a "break"
unsigned int varvoidcounter = 0;
wxString ScriptEditHandler::ConvertVarArgument(ScriptArgument& arg) {
	if (!arg.is_var)
		return wxEmptyString;
	
	unsigned int macroi;
	uint8_t varcat;
	uint16_t varid;
	#define MACRO_SEARCHVARNAME(STRING) \
		for (macroi=0;macroi<G_N_ELEMENTS(VarNameList);macroi++) \
			if (VarNameList[macroi].cat==varcat && VarNameList[macroi].id==varid) { \
				STRING = _(VarNameList[macroi].name); \
				break; \
			} \
		if (macroi==G_N_ELEMENTS(VarNameList)) { \
			for (macroi=0;macroi<script.local_data[entry_selection].amount;macroi++) \
				if (script.local_data[entry_selection].cat[macroi]==varcat && script.local_data[entry_selection].id[macroi]==varid) { \
					STRING = _(script.local_data[entry_selection].name[macroi]); \
					break; \
				} \
			if (macroi==script.local_data[entry_selection].amount) { \
				for (macroi=0;macroi<script.global_data.amount;macroi++) \
					if (script.global_data.cat[macroi]==varcat && script.global_data.id[macroi]==varid) { \
						STRING = _(script.global_data.name[macroi]); \
						break; \
					} \
				if (macroi==script.global_data.amount) { \
					STRING = _(VarOpList[varcat].opstring); \
					STRING << (int)varid; \
				} \
			} \
		}
	
	unsigned int operand = 0, i = 0, j, voidcheck = 0;
	uint8_t varbyte = arg.var[i++];
	int vartype = VarOpList[varbyte].type;
	while (vartype!=-1) {
		if (vartype==0) {
			operandtmp[operand-1] = _(L"( ")+_(VarOpList[varbyte].opstring)+operandtmp[operand-1]+_(L" )");
		} else if (vartype==1) {
			operandtmp[operand-1] = _(L"( ")+operandtmp[operand-1]+_(VarOpList[varbyte].opstring)+_(L" )");
		} else if (vartype==2) {
			operandtmp[operand-2] = _(L"( ")+operandtmp[operand-2]+_(L" ")+_(VarOpList[varbyte].opstring)+_(L" ")+operandtmp[operand-1]+_(L" )");
			operand--;
		} else if (vartype==3) {
			operandtmp[operand-1] += _(L"[");
			for (j=0;j<G_N_ELEMENTS(ScriptCharacterField);j++)
				if (ScriptCharacterField[j].id==arg.var[i]) {
					operandtmp[operand-1] << _(ScriptCharacterField[j].name);
					break;
				}
			if (j==G_N_ELEMENTS(ScriptCharacterField))
				operandtmp[operand-1] << (int)arg.var[i];
			i++;
			operandtmp[operand-1] += _(L"]");
		} else if (vartype==5) {
			operandtmp[operand].Empty();
			operandtmp[operand] << (unsigned int)arg.var[i++];
			operandtmp[operand++] += L"S";
		} else if (vartype==6) {
			operandtmp[operand].Empty();
			operandtmp[operand++] << (unsigned int)(arg.var[i]+(arg.var[i+1] << 8));
			i += 2;
		} else if (vartype==7) {
			operandtmp[operand].Empty();
			operandtmp[operand] << (unsigned int)(arg.var[i]+(arg.var[i+1] << 8)+(arg.var[i+2] << 16)+(arg.var[i+3] << 24));
			operandtmp[operand++] += L"L";
			i += 4;
		} else if (vartype>=10 && vartype<20) {
			varcat = varbyte;
			varid = arg.var[i++];
			MACRO_SEARCHVARNAME(operandtmp[operand])
			operand++;
		} else if (vartype>=20 && vartype<30) {
			varcat = varbyte;
			varid = arg.var[i]+(arg.var[i+1] << 8);
			i += 2;
			MACRO_SEARCHVARNAME(operandtmp[operand])
			operand++;
		} else if (vartype==50) {
			operandtmp[operand-1] = _(VarOpList[varbyte].opstring)+_(L"(")+operandtmp[operand-1]+_(L")");
		} else if (vartype==51) {
			operandtmp[operand-2] = _(VarOpList[varbyte].opstring)+_(L"(")+operandtmp[operand-2]+_(L", ")+operandtmp[operand-1]+_(L")");
			operand--;
		} else if (vartype==55) {
			if (arg.var[i+1]<G_N_ELEMENTS(VarEntryPropList)) {
				operandtmp[operand].Empty();
				operandtmp[operand] << _(VarEntryPropList[arg.var[i+1]].opstring) << _(L"(") << (unsigned int)arg.var[i] << _(L")");
				operand++;
				i += 2;
			} else {
				operandtmp[operand].Empty();
				operandtmp[operand] << _(VarOpList[varbyte].opstring) << _(L"(") << (unsigned int)arg.var[i+1] << _(L", ") << (unsigned int)arg.var[i] << _(L")");
				operand++;
				i += 2;
			}
		}
		varbyte = arg.var[i++];
		vartype = VarOpList[varbyte].type;
		voidcheck++;
	}
	if (operand==1) {
		if (voidcheck<=1)
			varvoidcounter = 3;
		if (operandtmp[0][0]==L'(')
			return operandtmp[0].substr(2,operandtmp[0].length()-4);
		return operandtmp[0];
	}
	return wxEmptyString;
}

#define BLOCK_TYPE_ROOT			0
#define BLOCK_TYPE_IF			1
#define BLOCK_TYPE_IFN			2
#define BLOCK_TYPE_ELSE			3
#define BLOCK_TYPE_WHILE		4
#define BLOCK_TYPE_SWITCH		5
#define BLOCK_TYPE_SWITCHDEF	6
#define BLOCK_TYPE_SWITCHEX		7
#define BLOCK_TYPE_SWITCHEXDEF	8
bool ScriptEditHandler::GenerateFunctionStrings_Rec(wxString& str, ScriptFunction& func, unsigned int& funcpos, unsigned int& oppos, int endfuncpos, unsigned int tabpos, int blocktype, int endblockpos) {
	int macroop,macropos;
	#define MACRO_FINDOP(OFFSET) \
		macroop = oppos+1; \
		macropos = 0; \
		if (OFFSET>=0) \
			while (macropos<OFFSET) \
				macropos += func.op[macroop++].size; \
		else \
			while (macropos>OFFSET) \
				macropos -= func.op[--macroop].size;
	
	unsigned int i,j;
	wxString tabstr = L"";
	for (i=0;i<tabpos;i++)
		tabstr += _(TAB_STR);
	if (endfuncpos==-1)
		endfuncpos = func.length;
	if (endblockpos==-1)
		endblockpos = func.length;
	while (funcpos<endfuncpos) {
		if (varvoidcounter>0)
			varvoidcounter--;
		// Assume there are 0x05 opcodes before jump opcodes recquiring a stack value
		switch (func.op[oppos].opcode) {
		case 0x01: {
			if (blocktype==BLOCK_TYPE_ROOT && funcpos+3+func.op[oppos].arg[0].GetValue()==0) {
				str += tabstr+_(L"loop\n");
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.length;
				return false;
			} else if (funcpos+3+func.op[oppos].arg[0].GetValue()<endblockpos) {
				MACRO_FINDOP(func.op[oppos].arg[0].GetValue());
				if (macroop+1<=func.op_amount && func.op[macroop+1].opcode==0x03 && 3+macropos+func.op[macroop].size+func.op[macroop+1].arg[0].GetValue()==0) {
					str += tabstr+_(L"while ( ");
					str += ConvertVarArgument(func.op[macroop].arg[0]);
					str += _(L" ) {\n");
					funcpostrack[functrackline++] = funcpos;
					funcpos += func.op[oppos++].size;
					GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+macropos,tabpos+1,BLOCK_TYPE_WHILE,funcpos+macropos);
					str += tabstr+_(L"}\n");
					funcpostrack[functrackline] = funcpostrack[functrackline-1];
					functrackline++;
					funcpos += func.op[oppos++].size;
					funcpos += func.op[oppos++].size;
				} else if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos) {
					funcpos += func.op[oppos++].size;
					return true;
				} else if (func.op[oppos].arg[0].GetValue()==0 && varvoidcounter>0) {
					funcpos += func.op[oppos++].size;
				} else {
					str += tabstr+_(L"break\n");
					funcpostrack[functrackline++] = funcpos;
					funcpos += func.op[oppos++].size;
					return false;
				}
			} else if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos && funcpos+3+func.op[oppos].arg[0].GetValue()<=endblockpos) {
				funcpos += func.op[oppos++].size;
				return true;
			} else {
				str += tabstr+_(L"break\n");
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
				if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos && func.op[oppos].opcode==0x01) {
					funcpos += func.op[oppos++].size;
					return true;
				}
				return false;
			}
			break;
		}
		case 0x02: {
			varvoidcounter = 0;
			str += tabstr+_(L"if ( ");
			str += ConvertVarArgument(func.op[oppos-1].arg[0]);
			str += _(L" ) {\n");
			funcpostrack[functrackline++] = funcpos;
			funcpos += func.op[oppos++].size;
			if (GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_IF,endblockpos)) {
				str += tabstr+_(L"} else {\n");
				funcpostrack[functrackline++] = funcpos;
				bool allreturn = GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_ELSE,endblockpos);
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			} else {
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			}
			break;
		}
		case 0x03: {
			varvoidcounter = 0;
			if (func.op[oppos].arg[0].GetValue()>=0) {
				str += tabstr+_(L"ifnot ( ");
				str += ConvertVarArgument(func.op[oppos-1].arg[0]);
				str += _(L" ) {\n");
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_IFN,endblockpos);
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			} else {
				unsigned int j;
				wxString lstr = str,rstr;
				str = L"";
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				lstr = lstr.BeforeLast(L'\n',&rstr);
				for (j=functrackline-1;j>0 && funcpostrack[j]>=funcpos+3+func.op[oppos].arg[0].GetValue();j--) {
					funcpostrack[j] = funcpostrack[j-1];
					lstr = lstr.BeforeLast(L'\n',&rstr);
					str = _(L"\n")+_(TAB_STR)+rstr+str;
				}
				funcpostrack[j+1] = funcpostrack[j+2];
				str = lstr+_(L"\n")+tabstr+_(L"do {")+str+_(L"\n");
				str += tabstr+_(L"} while ( ");
				str += ConvertVarArgument(func.op[oppos-1].arg[0]);
				str += _(L" )\n");
				functrackline++;
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
			}
			break;
		}
		case 0x04: {
			str += tabstr+_(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label)+_(L"\n");
			funcpostrack[functrackline++] = funcpos;
			if (blocktype==BLOCK_TYPE_ROOT) {
				funcpos = func.length;
				return false;
			}
			funcpos += func.op[oppos++].size;
			if (funcpos<func.length && func.op[oppos].opcode==0x01) {
				if (funcpos+3+func.op[oppos].arg[0].GetValue()<endblockpos) {
					MACRO_FINDOP(func.op[oppos].arg[0].GetValue());
					if (macroop+1<=func.op_amount && func.op[macroop+1].opcode==0x03 && 3+macropos+func.op[macroop].size+func.op[macroop+1].arg[0].GetValue()==0) {
						return blocktype==BLOCK_TYPE_ELSE;
					} else if (funcpos+3==endfuncpos) {
						funcpos += func.op[oppos++].size;
						return true;
					}
				} else if (funcpos+3==endfuncpos && funcpos+3+func.op[oppos].arg[0].GetValue()<=endblockpos) {
					funcpos += func.op[oppos++].size;
					return true;
				}
			}
			return blocktype==BLOCK_TYPE_ELSE;
		}
		case 0x05: {
			if (func.op[oppos+1].opcode!=0x02 && func.op[oppos+1].opcode!=0x03 && func.op[oppos+1].opcode!=0x06 && func.op[oppos+1].opcode!=0x0B) {
				str += tabstr+_(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label)+_(L" ")+ConvertVarArgument(func.op[oppos].arg[0])+_(L"\n");
				funcpostrack[functrackline++] = funcpos;
			}
			funcpos += func.op[oppos++].size;
			break;
		}
		case 0x06: {
			varvoidcounter = 0;
			ScriptOperation& swop = func.op[oppos];
			str += tabstr+_(L"switchex ");
			str << (unsigned int)swop.size_byte;
			str += _(L" ( ")+ConvertVarArgument(func.op[oppos-1].arg[0]);
			str += _(L" ) {\n");
			funcpostrack[functrackline++] = funcpos;
			unsigned int swoppos = oppos;
			unsigned int swpos = funcpos+4;
			unsigned int swdefpos = swpos+swop.arg[0].GetValue();
			int swendpos = -1;
			unsigned int currentcasepos = 0xFFFF;
			unsigned int nextcasepos = swop.arg[0].GetValue();
			unsigned int nbcasedone = 0;
			bool casedone[SCRPT_MAX_SWITCH_CASE];
			for (i=0;i<swop.size_byte;i++)
				casedone[i] = false;
			while (nbcasedone<swop.size_byte) {
				for (i=0;i<swop.size_byte;i++)
					if (!casedone[i] && currentcasepos>swop.arg[2+i*2].GetValue())
						currentcasepos = swop.arg[2+i*2].GetValue();
				nextcasepos = swop.arg[0].GetValue();
				for (i=0;i<swop.size_byte;i++)
					if (nextcasepos>swop.arg[2+i*2].GetValue() && swop.arg[2+i*2].GetValue()>currentcasepos)
						nextcasepos = swop.arg[2+i*2].GetValue();
				bool needsemicolon = false;
				str += tabstr+_(L"case ");
				for (i=0;i<swop.size_byte;i++)
					if (currentcasepos==swop.arg[2+i*2].GetValue()) {
						if (needsemicolon)
							str << _(L" ; ");
						else
							needsemicolon = true;
						str << (int)swop.arg[1+i*2].GetValue();
						casedone[i] = true;
						nbcasedone++;
					}
				str += _(L":\n");
				funcpos = swpos+swop.size-4;
				oppos = swoppos+1;
				while (funcpos<swpos+currentcasepos) {
					funcpos += func.op[oppos].size;
					oppos++;
				}
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,swpos+nextcasepos,tabpos+1,BLOCK_TYPE_SWITCHEX,swpos+nextcasepos);
				if (func.op[oppos-1].opcode==0x01)
					swendpos = funcpos+func.op[oppos-1].arg[0].GetValue();
				currentcasepos = nextcasepos;
			}
			funcpos = swpos+swop.size-4;
			oppos = swoppos+1;
			while (funcpos<swdefpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			if (swendpos>=0 && funcpos<swendpos) {
				str += tabstr+_(L"default:\n");
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,swendpos,tabpos+1,BLOCK_TYPE_SWITCHEXDEF,swendpos);
			}
			str += tabstr+_(L"}\n");
			funcpostrack[functrackline] = funcpostrack[functrackline-1];
			functrackline++;
			break;
		}
		case 0x0B: {
			varvoidcounter = 0;
			ScriptOperation& swop = func.op[oppos];
			str += tabstr+_(L"switch ");
			str << (unsigned int)swop.size_byte;
			str += _(L" ( ")+ConvertVarArgument(func.op[oppos-1].arg[0]);
			str += _(L" ) from ");
			str << (unsigned int)swop.arg[0].GetValue();
			str += _(" {\n");
			funcpostrack[functrackline++] = funcpos;
			unsigned int swoppos = oppos;
			unsigned int swpos = funcpos+1;
			unsigned int swdefpos = swpos+swop.arg[1].GetValue();
			int caseendpos = swdefpos;
			int swendpos = -1;
			unsigned int highestpos = swop.arg[1].GetValue();
			unsigned int highestendpos = swdefpos;
			unsigned int currentcasepos = 0xFFFF;
			unsigned int nextcasepos = 0;
			unsigned int nbcasedone = 0;
			bool casedone[SCRPT_MAX_SWITCH_CASE];
			for (i=0;i<swop.size_byte;i++) {
				casedone[i] = false;
				if (highestpos<swop.arg[2+i].GetValue())
					highestpos = swop.arg[2+i].GetValue();
				// Default position may be one of the case position
				// It's problematic if that is not the last case because it removes info on case's limits
				// Cases are then re-ordered
				if (swop.arg[2+i].GetValue()>swop.arg[1].GetValue()) {
					caseendpos = -1;
					if (swendpos==-1 || swendpos>swpos+swop.arg[2+i].GetValue())
						swendpos = swpos+swop.arg[2+i].GetValue();
				}
			}
			while (nbcasedone<swop.size_byte && nextcasepos<highestpos) {
				for (i=0;i<swop.size_byte;i++)
					if (!casedone[i] && currentcasepos>swop.arg[2+i].GetValue() && swop.arg[2+i].GetValue()!=swop.arg[1].GetValue())
						currentcasepos = swop.arg[2+i].GetValue();
				nextcasepos = 0xFFFF;
				for (i=0;i<swop.size_byte;i++)
					if (nextcasepos>swop.arg[2+i].GetValue() && swop.arg[2+i].GetValue()>currentcasepos)
						nextcasepos = swop.arg[2+i].GetValue();
				bool needsemicolon = false;
				str += tabstr+_(L"case +");
				for (i=0;i<swop.size_byte;i++)
					if (currentcasepos==swop.arg[2+i].GetValue()) {
						if (needsemicolon)
							str << _(L" ; +");
						else
							needsemicolon = true;
						str << (int)i;
						casedone[i] = true;
						nbcasedone++;
					}
				str += _(L":\n");
				funcpos = swpos+swop.size-1;
				oppos = swoppos+1;
				while (funcpos<swpos+currentcasepos) {
					funcpos += func.op[oppos].size;
					oppos++;
				}
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,nextcasepos!=0xFFFF ? swpos+nextcasepos : caseendpos,tabpos+1,BLOCK_TYPE_SWITCH,nextcasepos!=0xFFFF ? swpos+nextcasepos : -1);
				if (highestendpos<funcpos)
					highestendpos = funcpos;
				if (func.op[oppos-1].opcode==0x01 && swendpos==-1)
					swendpos = funcpos+func.op[oppos-1].arg[0].GetValue();
				currentcasepos = nextcasepos;
			}
			if ((int)highestendpos<swendpos)
				highestendpos = swendpos;
			funcpos = swpos+swop.size-1;
			oppos = swoppos+1;
			while (funcpos<swdefpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			if (swendpos>=0 && funcpos<swendpos) {
				str += tabstr+_(L"default:\n");
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,swendpos,tabpos+1,BLOCK_TYPE_SWITCHDEF,swendpos);
			}
			while (funcpos<highestendpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			str += tabstr+_(L"}\n");
			funcpostrack[functrackline] = funcpostrack[functrackline-1];
			functrackline++;
			break;
		}
		default: {
			str += tabstr+_(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label)+_(L"( ");
			for (i=0;i<func.op[oppos].arg_amount;i++) {
				ScriptArgument& arg = func.op[oppos].arg[i];
				if (arg.is_var)
					str += ConvertVarArgument(arg);
				else if (func.op[oppos].opcode==0x29)
					str << L"( " << (int16_t)(arg.GetValue() & 0xFFFF) << L", " << (int16_t)((arg.GetValue() >> 16) & 0xFFFF) << L" )";
				else
					str << arg.GetValue();
				if (i+1<func.op[oppos].arg_amount)
					str += _(L", ");
			}
			str += _(L" )\n");
			funcpostrack[functrackline++] = funcpos;
			if (func.op[oppos].opcode==0x2F) {
				for (j=0;j<modellist_str.Count();j++)
					if (func.op[oppos].arg[0].GetValue()==*modellist_id[j]) {
						EntryChangeName(entry_selection,modellist_str[j]);
						break;
					}
			} else if (func.op[oppos].opcode==0x07) {
				if (!func.op[oppos].arg[0].is_var && func.op[oppos].arg[0].GetValue()<script.entry_amount)
					EntryChangeName(func.op[oppos].arg[0].GetValue(),wxString::Format(wxT("Code%u"),func.op[oppos].arg[0].GetValue()));
			} else if (func.op[oppos].opcode==0x08) {
				if (!func.op[oppos].arg[0].is_var && func.op[oppos].arg[0].GetValue()<script.entry_amount)
					EntryChangeName(func.op[oppos].arg[0].GetValue(),wxString::Format(wxT("Region%u"),func.op[oppos].arg[0].GetValue()));
			}
			funcpos += func.op[oppos++].size;
		}
		}
	}
	return false;
}

void ScriptEditHandler::GenerateFunctionStrings() {
	unsigned int i,j,funci = 0,funcpos,oppos,entrytmp = entry_selection;
	func_str = new wxString*[script.entry_amount];
	localvar_str = new wxString[script.entry_amount];
/*fstream fout("aaaa.txt",ios::app|ios::out);
for (i=0;i<script.entry_amount;i++) for (j=0;j<script.entry_function_amount[i];j++) {
fout << "New Function : " << i << " " << j << endl;
for (unsigned int k=0;k<script.func[i][j].op_amount;k++) {fout << std::hex << (unsigned int)script.func[i][j].op[k].size << " : " << std::hex << (unsigned int)script.func[i][j].op[k].opcode;
for (unsigned int l=0;l<script.func[i][j].op[k].arg_amount;l++) fout << " " << std::hex << (unsigned int)script.func[i][j].op[k].arg[l].value;
fout << endl;} fout << endl;}*/
	globalvar_str = _(L"");
	if (script.global_data.amount>0) {
		for (j=0;j<script.global_data.amount;j++) {
			globalvar_str += _(L"global ");
			if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_BOOL)
				globalvar_str += _(L"bool ");
			else {
				if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_INT)
					globalvar_str += _(L"int");
				else if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_UINT)
					globalvar_str += _(L"uint");
				globalvar_str += wxString::Format(wxT("%u "),script.global_data.size[j]);
			}
			globalvar_str += wxString::Format(wxT("%s %s%u\n"),script.global_data.name[j],VarOpList[script.global_data.cat[j]].opstring,script.global_data.id[j]);
		}
	}
	for (i=0;i<script.entry_amount;i++) {
		localvar_str[i] = _(L"");
		if (script.entry_local_var[i]>0)
			localvar_str[i] += wxString::Format(wxT("allocate %u\n"),script.entry_local_var[i]);
		if (script.global_data.amount>0) {
			if (script.entry_local_var[i]>0)
				localvar_str[i] += _(L"\n");
			localvar_str[i] += globalvar_str;
		}
		if (script.local_data[i].amount>0) {
			localvar_str[i] += _(L"\n");
			for (j=0;j<script.local_data[i].amount;j++) {
				localvar_str[i] += _(L"local ");
				if (script.local_data[i].type[j]==SCRIPT_VARIABLE_TYPE_BOOL)
					localvar_str[i] += _(L"bool ");
				else {
					if (script.local_data[i].type[j]==SCRIPT_VARIABLE_TYPE_INT)
						localvar_str[i] += _(L"int");
					else if (script.local_data[i].type[j]==SCRIPT_VARIABLE_TYPE_UINT)
						localvar_str[i] += _(L"uint");
					localvar_str[i] += wxString::Format(wxT("%u "),script.local_data[i].size[j]);
				}
				localvar_str[i] += wxString::Format(wxT("%s %s%u\n"),script.local_data[i].name[j],VarOpList[script.local_data[i].cat[j]].opstring,script.local_data[i].id[j]);
			}
		}
		func_str[i] = new wxString[script.entry_function_amount[i]];
		entry_selection = i;
		for (j=0;j<script.entry_function_amount[i];j++) {
			funcpos = 0;
			oppos = 0;
			functrackline = 0;
			funcpostrack[functrackline++] = funcpos;
			while (funcpos<script.func[i][j].length)
				GenerateFunctionStrings_Rec(func_str[i][j],script.func[i][j],funcpos,oppos);
			funcpostrack[functrackline++] = script.func[i][j].length;
			func_str[i][j] = functionlist_str[funci]+_(L"\n")+func_str[i][j];
			funci++;
		}
	}
	entry_selection = entrytmp;
}

void ScriptEditDialog::UpdateGlobalLocalStrings(int ignoreentry) {
	unsigned int i,j;
	wxString line, tmpstr, tmprest;
	size_t pos;
	globalvar_str = _(L"");
	for (j=0;j<script.global_data.amount;j++) {
		globalvar_str += _(L"global ");
		if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_BOOL)
			globalvar_str += _(L"bool ");
		else {
			if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_INT)
				globalvar_str += _(L"int");
			else if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_UINT)
				globalvar_str += _(L"uint");
			globalvar_str += wxString::Format(wxT("%u "),script.global_data.size[j]);
		}
		globalvar_str += wxString::Format(wxT("%s %s%u\n"),script.global_data.name[j],VarOpList[script.global_data.cat[j]].opstring,script.global_data.id[j]);
	}
	for (i=0;i<script.entry_amount;i++) {
		if (i==ignoreentry)
			continue;
		wxString localstr = localvar_str[i];
		wxString localstrtmp = localstr;
		pos = 0;
		while (localstrtmp.Len()>0) {
			line = localstrtmp.BeforeFirst(L'\n',&tmprest);
			localstrtmp = tmprest;
			if (line.Mid(0,7).IsSameAs(L"global ")) {
				tmpstr = localstr.Mid(0,pos);
				localstr = tmpstr+localstr.Mid(pos+line.Len()+1);
			} else
				pos += line.Len()+1;
		}
		if (localstr.Mid(0,9).IsSameAs(L"allocate ")) {
			tmpstr = localstr.BeforeFirst(L'\n',&tmprest)+_(L"\n\n");
			if (tmprest[0]==L'\n' && tmprest[1]==L'\n')
				tmprest = tmprest.Mid(1);
		} else
			tmpstr = _(L"");
		if (script.global_data.amount>0) {
			tmpstr += globalvar_str;
		}
		localvar_str[i] = tmpstr+tmprest;
	}
}

void ScriptEditHandler::EntryChangeName(unsigned int entry, wxString newname) {
	unsigned int i,j,funci = 0;
		entry_name[entry] = newname;
	if (handler_dialog)
		handler_dialog->entrylist_str[entry] = _(L"Entry ")+newname;
	for (i=0;i<script.entry_amount;i++)
		for (j=0;j<script.entry_function_amount[i];j++) {
			if (i==entry) {
				if (script.function_type[i][j]<G_N_ELEMENTS(FunctionTypeName))
					functionlist_str[funci] = _(L"Function ")+wxString::Format(wxT("%s_%s"),newname,FunctionTypeName[script.function_type[i][j]]);
				else
					functionlist_str[funci] = _(L"Function ")+wxString::Format(wxT("%s_%u"),newname,script.function_type[i][j]);
				if (handler_dialog)
					handler_dialog->m_functionlist->SetItemText(funci,functionlist_str[funci]);
			}
			funci++;
		}
}

//=============================//
//         Parse Code          //
//=============================//

uint8_t varargtmp[0x800];
bool IsAlphaNum(wchar_t c) {
	return (c>=L'0' && c<=L'9') || (c>=L'a' && c<=L'z') || (c>=L'A' && c<=L'Z') || c==L'_';
}
bool IsOperator(wchar_t c) {
	return c==L'+' || c==L'-' || c==L'*' || c==L'/' || c==L'%' || c==L'&' || c==L'|' || c==L'<' || c==L'>' || c==L'=' || c==L'!' || c==L'~' || c==L'#' || c==L'^' || c==L'$'; // || c==L',' || c==L'[' || c==L']'
}
wxString GetNextVarThing(wxString& varstr) {
	wxString tmp, res = L"";
	size_t pos = varstr.find_first_not_of(L' ');
	if (pos==string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	tmp = varstr.Mid(pos);
	res += tmp[0];
	if (IsAlphaNum(tmp[0])) {
		for (pos=1;IsAlphaNum(tmp[pos]);pos++)
			res += tmp[pos];
	} else if (IsOperator(tmp[0])) {
		for (pos=1;IsOperator(tmp[pos]);pos++)
			res += tmp[pos];
	} else {
		pos = 1;
	}
	lineshift += pos;
	varstr = varstr.Mid(lineshift);
	return res;
}
uint8_t* ConvertStringToVararg(wxString varstr, uint8_t& varsize, wxString& errmsg, ScriptLocalVariableSet* localvar) {
	int objecttype = -1, lastobjecttype = -1;
	unsigned int i,j,len;
	unsigned int parlvl = 0, lvlmove[SCRPT_MAX_OPERAND];
	int expectedval = 1;
	uint8_t sz = 0, bufferpos = 0;
	wxString tmpstr;
	uint8_t* res;
	uint16_t tmp16;
	uint32_t tmp32;
	if (varstr.IsEmpty()) {
		errmsg = _(HADES_STRING_SCRIPT_VARARG_EMPTY);
		return NULL;
	}
	
	unsigned int macroi;
	#define MACRO_APPEND_BYTE(BYTE,INCREASEPOS) \
		for (macroi=sz;macroi>bufferpos;macroi--) \
			varargtmp[macroi] = varargtmp[macroi-1]; \
		sz++; \
		varargtmp[bufferpos] = BYTE; \
		if (INCREASEPOS) bufferpos++;
	
	size_t replacepos;
	for (i=0;i<G_N_ELEMENTS(VarNameList);i++) {
		tmpstr = _(VarOpList[VarNameList[i].cat].opstring);
		tmpstr << VarNameList[i].id;
		replacepos = varstr.find(VarNameList[i].name);
		while (replacepos!=wxNOT_FOUND) {
			if ((replacepos==0 || !IsAlphaNum(varstr[replacepos-1])) && (replacepos+VarNameList[i].name.length()==varstr.Len() || !IsAlphaNum(varstr[replacepos+VarNameList[i].name.length()])))
				varstr.replace(replacepos,VarNameList[i].name.length(),tmpstr);
			replacepos = varstr.find(VarNameList[i].name,replacepos+1);
		}
	}
	for (i=0;i<localvar->amount;i++) {
		tmpstr = _(VarOpList[localvar->cat[i]].opstring);
		tmpstr << localvar->id[i];
		replacepos = varstr.find(localvar->name[i]);
		while (replacepos!=wxNOT_FOUND) {
			if ((replacepos==0 || !IsAlphaNum(varstr[replacepos-1])) && (replacepos+localvar->name[i].length()==varstr.Len() || !IsAlphaNum(varstr[replacepos+localvar->name[i].length()])))
				varstr.replace(replacepos,localvar->name[i].length(),tmpstr);
			replacepos = varstr.find(localvar->name[i],replacepos+1);
		}
	}
	lvlmove[0] = 0;
	MACRO_APPEND_BYTE(0x7F,false)
	while (varstr.Length()>0) {
		tmpstr = GetNextVarThing(varstr);
		len = tmpstr.Length();
		if (tmpstr.IsSameAs(L"(")) {
			lvlmove[++parlvl] = 0;
		} else if (tmpstr.IsSameAs(L")")) {
			if (parlvl==0) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_TOO_PAR);
				return NULL;
			}
			if (lvlmove[parlvl]==2) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_COMMA);
				return NULL;
			}
			if (lvlmove[parlvl--])
				bufferpos++;
		} else if (tmpstr.IsSameAs(L",")) {
			if (lvlmove[parlvl]<2) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_TOO_COMMA);
				return NULL;
			}
			lvlmove[parlvl]--;
		} else if (tmpstr.IsSameAs(L"[")) {
			MACRO_APPEND_BYTE(0x29,true)
			tmpstr = GetNextVarThing(varstr);
			if (tmpstr.IsNumber()) {
				MACRO_APPEND_BYTE(wxAtoi(tmpstr),true)
			} else {
				for (j=0;j<G_N_ELEMENTS(ScriptCharacterField);j++)
					if (tmpstr.IsSameAs(ScriptCharacterField[j].name)) {
						MACRO_APPEND_BYTE(ScriptCharacterField[j].id,true)
						break;
					}
				if (j==G_N_ELEMENTS(ScriptCharacterField)) {
					errmsg = _(HADES_STRING_SCRIPT_VARARG_ARRAY);
					return NULL;
				}
			}
			tmpstr = GetNextVarThing(varstr);
			if (!tmpstr.IsSameAs(L"]")) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_BRACKETS);
				return NULL;
			}
		} else if (tmpstr.IsNumber() && !tmpstr.IsSameAs(L"-") && !tmpstr.IsSameAs(L"+")) {
			objecttype = 6;
			tmp16 = wxAtoi(tmpstr);
			MACRO_APPEND_BYTE(0x7D,true)
			MACRO_APPEND_BYTE(tmp16 & 0xFF,true)
			MACRO_APPEND_BYTE((tmp16 >> 8) & 0xFF,true)
			expectedval--;
		} else if (tmpstr.Mid(0,len-1).IsNumber() && tmpstr[len-1]==L'L') {
			objecttype = 7;
			tmp32 = wxAtoi(tmpstr);
			MACRO_APPEND_BYTE(0x7E,true)
			MACRO_APPEND_BYTE(tmp32 & 0xFF,true)
			MACRO_APPEND_BYTE((tmp32 >> 8) & 0xFF,true)
			MACRO_APPEND_BYTE((tmp32 >> 16) & 0xFF,true)
			MACRO_APPEND_BYTE((tmp32 >> 24) & 0xFF,true)
			expectedval--;
		} else {
			for (i=0;i<256;i++) {
				if (tmpstr.IsSameAs(VarOpList[i].opstring) || (VarOpList[i].type>=10 && VarOpList[i].type<30 && tmpstr.Mid(0,VarOpList[i].opstring.length()).IsSameAs(VarOpList[i].opstring))) {
					if (VarOpList[i].type==1) // Determine whether it's pre-fixed or post-fixed operator
						if (lastobjecttype<10 || lastobjecttype>=30)
							i += 2;
					objecttype = VarOpList[i].type;
					if (objecttype==0) { // unary operator pre-fixed
						MACRO_APPEND_BYTE(i,false)
						lvlmove[parlvl] = 1;
					} else if (objecttype==1) { // unary operator post-fixed
						MACRO_APPEND_BYTE(i,true)
					} else if (objecttype==2) { // binary operator
						MACRO_APPEND_BYTE(i,false)
						lvlmove[parlvl] = 1;
						expectedval++;
					} else if (objecttype>=10 && objecttype<20) { // get variable (array byte = 1)
						MACRO_APPEND_BYTE(i,true)
						wxString num = tmpstr.Mid(VarOpList[i].opstring.length());
						if (!num.IsNumber()) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN),tmpstr);
							return NULL;
						}
						MACRO_APPEND_BYTE(wxAtoi(num),true)
						expectedval--;
					} else if (objecttype>=20 && objecttype<30) { // get variable (array byte = 2)
						MACRO_APPEND_BYTE(i,true)
						wxString num = tmpstr.Mid(VarOpList[i].opstring.length());
						if (!num.IsNumber()) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN),tmpstr);
							return NULL;
						}
						tmp16 = wxAtoi(num);
						MACRO_APPEND_BYTE(tmp16 & 0xFF,true)
						MACRO_APPEND_BYTE((tmp16 >> 8) & 0xFF,true)
						expectedval--;
					} else if (objecttype==50) { // function (1 arg)
						MACRO_APPEND_BYTE(i,false)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L"(")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
							return NULL;
						}
						lvlmove[++parlvl] = 1;
					} else if (objecttype==51) { // function (2 args)
						MACRO_APPEND_BYTE(i,false)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L"(")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
							return NULL;
						}
						lvlmove[++parlvl] = 2;
						expectedval++;
					} else if (objecttype==55) { // GetEntryProperty
						MACRO_APPEND_BYTE(i,true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L"(")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
							return NULL;
						}
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsNumber()) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
							return NULL;
						}
						tmp32 = wxAtoi(tmpstr);
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L",")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_COMMA);
							return NULL;
						}
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsNumber()) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
							return NULL;
						}
						MACRO_APPEND_BYTE(wxAtoi(tmpstr),true)
						MACRO_APPEND_BYTE(tmp32,true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L")")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
							return NULL;
						}
						expectedval--;
					} else { // unknown/unused
						errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN),tmpstr);
						return NULL;
					}
					break;
				}
			}
			if (i>=256) {
				for (i=0;i<G_N_ELEMENTS(VarEntryPropList);i++)
					if (tmpstr.IsSameAs(VarEntryPropList[i].opstring)) { // GetEntry specific property
						MACRO_APPEND_BYTE(0x78,true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L"(")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
							return NULL;
						}
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsNumber()) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
							return NULL;
						}
						MACRO_APPEND_BYTE(wxAtoi(tmpstr),true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L")")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
							return NULL;
						}
						MACRO_APPEND_BYTE(VarEntryPropList[i].type-1000,true)
						expectedval--;
						break;
					}
				if (i>=G_N_ELEMENTS(VarEntryPropList)) {
					errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN),tmpstr);
					return NULL;
				}
			}
		}
		lastobjecttype = objecttype;
	}
	if (parlvl>0) {
		errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
		return NULL;
	}
	if (expectedval!=0) {
		errmsg = _(HADES_STRING_SCRIPT_VARARG_OPVAMISMATCH);
		return NULL;
	}
	varsize = sz;
	res = new uint8_t[varsize];
	memcpy(res,varargtmp,varsize);
	return res;
}

uint8_t GetLocalCategoryFromType(int localtype, int type, int size) {
	if (localtype==SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==8)
			return 0xD2;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==16)
			return 0xDA;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==24)
			return 0xCA;
//		if (type==SCRIPT_VARIABLE_TYPE_INT && size==32)
//			return 0xD2;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==8)
			return 0xD6;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==16)
			return 0xDE;
//		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==24)
//			return 0xCA;
//		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==32)
//			return 0xD2;
		return 0xCA;
	} else if (localtype==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==8)
			return 0xD1;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==16)
			return 0xD9;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==24)
			return 0xC9;
//		if (type==SCRIPT_VARIABLE_TYPE_INT && size==32)
//			return 0xD1;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==8)
			return 0xD5;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==16)
			return 0xDD;
//		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==24)
//			return 0xC9;
//		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==32)
//			return 0xD1;
		return 0xC9;
	}
}

struct ScriptLocalVariableSetTmp {
	int local_type;
	int type;
	int size;
	wstring name;
	uint8_t cat;
	uint16_t id;
};
ScriptLocalVariableSetTmp localvartmp[0x100];
ScriptLocalVariableSet* ScriptEditDialog::ParseLocal(LogStruct& log, wxString str) {
	ScriptLocalVariableSet* res = new ScriptLocalVariableSet;
	wxString linestr, token, errstr;
	unsigned int i,j,k,vari;
	size_t pos;
	bool isok;
	res->allocate_amount = 0;
	
	#define MACRO_REINIT_LOCAL() \
		localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_UNKNOWN; \
		localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_UNKNOWN; \
		localvartmp[vari].size = 0; \
		localvartmp[vari].name = L""; \
		localvartmp[vari].cat = 0; \
		localvartmp[vari].id = 0;
	
	vari = 0;
	MACRO_REINIT_LOCAL()
	while (!str.IsEmpty()) {
		pos = str.find_first_of(L'\n');
		linestr = str.Mid(0,pos);
		if (pos!=string::npos)
			str = str.Mid(pos+1);
		else
			str = wxEmptyString;
		token = GetNextThing(linestr);
		if (token==wxEmptyString)
			continue;
		if (token.IsSameAs(L"allocate")) {
			token = GetNextThing(linestr);
			if (!token.IsNumber())
				log.AddError(HADES_STRING_LSCRIPT_ALLOC_NUMBER);
			else
				res->allocate_amount = wxAtoi(token);
		} else if (token.IsSameAs(L"local") || token.IsSameAs(L"global")) {
			isok = true;
			if (token.IsSameAs(L"local"))
				localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_LOCAL;
			else
				localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_GLOBAL;
			token = GetNextThing(linestr);
			if (token.Mid(0,3).IsSameAs(L"int")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_INT;
				if (token.Mid(3).IsNumber()) {
					localvartmp[vari].size = wxAtoi(token.Mid(3));
					if (localvartmp[vari].size!=8 && localvartmp[vari].size!=16 && localvartmp[vari].size!=24) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
						log.AddWarning(errstr.ToStdWstring());
						isok = false;
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
			} else if (token.Mid(0,4).IsSameAs(L"uint")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_UINT;
				if (token.Mid(4).IsNumber()) {
					localvartmp[vari].size = wxAtoi(token.Mid(4));
					if (localvartmp[vari].size!=8 && localvartmp[vari].size!=16) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
						log.AddWarning(errstr.ToStdWstring());
						isok = false;
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
			/* DEBUG
			} else if (token.IsSameAs(L"bool")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_BOOL;
				localvartmp[vari].size = 1;*/
			} else {
				errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
				log.AddWarning(errstr.ToStdWstring());
				isok = false;
			}
			if (isok) {
				token = GetNextThing(linestr);
				if (token==wxEmptyString) {
					log.AddWarning(HADES_STRING_LSCRIPT_MISS_NAME);
					isok = false;
				} else
					localvartmp[vari].name = token.ToStdWstring();
			}
			token = GetNextThing(linestr);
			while (isok && token!=wxEmptyString) {
				if (token.Mid(0,4).IsSameAs(L"VAR_")) {
					isok = false;
					for (i=0;i<G_N_ELEMENTS(VarOpList);i++)
						if (VarOpList[i].type>=10 && VarOpList[i].type<30 && token.Mid(0,VarOpList[i].opstring.length()).IsSameAs(VarOpList[i].opstring)) {
							localvartmp[vari].cat = i;
							localvartmp[vari].id = wxAtoi(token.Mid(VarOpList[i].opstring.length()));
							isok = true;
							break;
						}
					if (!isok) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
						log.AddWarning(errstr.ToStdWstring());
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
				token = GetNextThing(linestr);
			}
			if (isok)
				vari++;
			MACRO_REINIT_LOCAL()
		} else {
			errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
			log.AddWarning(errstr.ToStdWstring());
		}
	}
	bool localvarused[0x100];
	bool globalvarused[0x100];
	for (i=0;i<0x100;i++) {
		localvarused[i] = false;
		globalvarused[i] = false;
	}
	for (i=0;i<vari;i++)
		if (localvartmp[i].cat>0) {
			if (localvartmp[i].size==1) {
				if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
					localvarused[localvartmp[i].id >> 3] = true;
				else if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
					globalvarused[localvartmp[i].id >> 3] = true;
			} else {
				for (j=0;j<(localvartmp[i].size >> 3);j++) {
					if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
						localvarused[localvartmp[i].id+j] = true;
					else if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
						globalvarused[localvartmp[i].id+j] = true;
				}
			}
		}
	res->amount = vari;
	res->local_type = new uint8_t[vari];
	res->type = new int16_t[vari];
	res->size = new uint8_t[vari];
	res->name = new wstring[vari];
	res->cat = new uint8_t[vari];
	res->id = new uint16_t[vari];
	for (i=0;i<res->amount;i++) {
		res->local_type[i] = localvartmp[i].local_type;
		res->type[i] = localvartmp[i].type;
		res->size[i] = localvartmp[i].size;
		res->name[i] = localvartmp[i].name;
		if (localvartmp[i].cat>0) {
			res->cat[i] = localvartmp[i].cat;
			res->id[i] = localvartmp[i].id;
		} else {
			res->cat[i] = GetLocalCategoryFromType(res->local_type[i],res->type[i],res->size[i]);
			for (j=0;j<0x100;j++) {
				isok = true;
				if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
					for (k=0;k<max(1,res->size[i] >> 3);k++)
						if (j+k>=0x100 || localvarused[j+k]) {
							isok = false;
							j += k;
							break;
						}
				} else if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
					for (k=0;k<max(1,res->size[i] >> 3);k++)
						if (j+k>=0x100 || globalvarused[j+k]) {
							isok = false;
							j += k;
							break;
						}
				}
				if (isok) {
					if (res->size[i]==1)
						res->id[i] = j << 3;
					else
						res->id[i] = j;
					for (k=0;k<max(1,res->size[i] >> 3);k++) {
						if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
							localvarused[j+k] = true;
						else if (localvartmp[i].local_type==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
							globalvarused[j+k] = true;
					}
					break;
				}
			}
		}
	}
	for (int l=0xFF;l>=res->allocate_amount;l--)
		if (localvarused[l]) {
			errstr.Printf(wxT(HADES_STRING_LSCRIPT_MISS_ALLOC),l+1);
			log.AddWarning(errstr.ToStdWstring());
			break;
		}
	return res;
}

static wxString keywords[] = {
	L"if",		L"else",		L"ifnot",
	L"while",	L"do",			L"loop",
	L"switch",	L"switchex",	L"case",
	L"default",	L"break",		L"Function"
};
ScriptOperation parseop[0x8000];
unsigned int parseopamount = 0;
unsigned int parseoplength = 0;
LogStruct ScriptEditDialog::ParseFunction(wxString str, unsigned int entry, unsigned int function) {
	size_t pos;
	unsigned int rawpos = 0, lvlamount = 0, lvlrawpos[SCRPT_MAX_OPERAND], lvloppos[SCRPT_MAX_OPERAND], lvltype[SCRPT_MAX_OPERAND], lvlcaseamount[SCRPT_MAX_OPERAND];
	unsigned int breaklvlamount = 0, breaklvli[SCRPT_MAX_OPERAND], breaklvlrawpos[SCRPT_MAX_OPERAND][SCRPT_MAX_OPERAND], breaklvloppos[SCRPT_MAX_OPERAND][SCRPT_MAX_OPERAND];
	unsigned int i, j, line = 0, opi = 0;
	wxString linestr, token, errstr;
	wxString argstr[SCRPT_MAX_OPERAND];
	LogStruct res = LogStruct();
	uint8_t* lvlargval[SCRPT_MAX_OPERAND];
	uint8_t* argval, *argvalarray[SCRPT_MAX_OPERAND];
	uint8_t lvlargvalsize[SCRPT_MAX_OPERAND];
	uint8_t argvalsize, argvalarraysize[SCRPT_MAX_OPERAND];
	uint8_t varargbyte;
	int tokentype;
	bool islastarg;
	
	unsigned int macroi;
	wxString macrostr;
	#define MACRO_NEW_OPCODE(OPCODE) \
		parseop[opi].opcode = OPCODE; \
		if (OPCODE>=0xFF) { \
			parseop[opi].base_code = 0xFF; \
			parseop[opi].ext_code = OPCODE-0x100; \
		} else \
			parseop[opi].base_code = OPCODE; \
		parseop[opi].arg_amount = HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_amount; \
		if (parseop[opi].arg_amount>=0) { \
			parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount,&parseop[opi]); \
			for (macroi=0;macroi<parseop[opi].arg_amount;macroi++) { \
				parseop[opi].arg[macroi].is_signed = IsScriptArgTypeSigned(HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_type[macroi]); \
				parseop[opi].arg[macroi].typesize = HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_length[macroi]; \
			} \
		}
	#define MACRO_OPCODE_SIZE_DONE() \
		parseop[opi].size = parseop[opi].base_code==0xFF ? 2 : 1; \
		if (HADES_STRING_SCRIPT_OPCODE[parseop[opi].opcode].use_vararg) \
			parseop[opi].size += 1; \
		if (parseop[opi].opcode==0x06 || parseop[opi].opcode==0x0B || parseop[opi].opcode==0x29) \
			parseop[opi].size += 1; \
		for (macroi=0;macroi<parseop[opi].arg_amount;macroi++) \
			parseop[opi].size += parseop[opi].arg[macroi].size; \
		rawpos += parseop[opi++].size;
	#define MACRO_RISE_LEVEL(RAW,OP) \
		lvlrawpos[lvlamount] = RAW; \
		lvloppos[lvlamount] = OP; \
		lvltype[lvlamount] = tokentype; \
		lvlcaseamount[lvlamount++] = 0;
	#define MACRO_CHECK_PUNC_ERROR(PUNC) \
		if (!token.IsSameAs(PUNC)) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,PUNC); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_NUMBER_ERROR() \
		if (!token.IsNumber()) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_NUMBER),line); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_VARARG_ERROR(DEST,SIZEDEST) \
		DEST = ConvertStringToVararg(token,SIZEDEST,macrostr,localvar); \
		if (DEST==NULL) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MAIN),line,macrostr); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_TRAILING_WARNING() \
		linestr.Trim(false); \
		if (linestr.Length()>0) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_IGNORE),line,linestr); \
			res.AddWarning(errstr.ToStdWstring()); \
		}
	
	ScriptLocalVariableSet* localvar = ParseLocal(res,m_localvartext->GetValue());
	while (!str.IsEmpty()) {
		pos = str.find_first_of(L'\n');
		linestr = str.Mid(0,pos);
		if (pos!=string::npos)
			str = str.Mid(pos+1);
		else
			str = wxEmptyString;
		line++;
		token = GetNextThing(linestr);
		tokentype = -1;
		if (token==wxEmptyString)
			continue;
		for (i=0;i<G_N_ELEMENTS(keywords);i++)
			if (token.IsSameAs(keywords[i])) {
				tokentype = i+1;
				switch (i) {
				case 0: // if
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval,argvalsize);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x02)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos,opi-2)
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 1: // else
					errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L"}");
					res.AddError(errstr.ToStdWstring());
					break;
				case 2: // ifnot
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval,argvalsize);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x03)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos,opi-2)
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 3: // while
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(lvlargval[lvlamount],lvlargvalsize[lvlamount])
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos,opi-1)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 4: // do
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_RISE_LEVEL(rawpos,opi)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 5: // loop
					if (lvlamount>0) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_LOOP),line);
						res.AddError(errstr.ToStdWstring());
						break;
					}
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(-(rawpos+3));
					MACRO_OPCODE_SIZE_DONE()
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 6: // switch
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[0] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextThing(linestr);
					MACRO_CHECK_PUNC_ERROR(L"from")
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[1] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval,argvalsize);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x0B)
					parseop[opi].size_byte = wxAtoi(argstr[0]);
					parseop[opi].arg_amount = 2+parseop[opi].size_byte;
					parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount,&parseop[opi]);
					parseop[opi].arg[0].is_signed = false;
					parseop[opi].arg[0].typesize = 2;
					parseop[opi].arg[0].SetValue(wxAtoi(argstr[1]));
					for (j=1;j<parseop[opi].arg_amount;j++) {
						parseop[opi].arg[j].is_signed = true;
						parseop[opi].arg[j].typesize = 2;
						parseop[opi].arg[j].SetValue(-1);
					}
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos+1-parseop[opi-1].size,opi-2)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 7: // switchex
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[0] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval,argvalsize);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x06)
					parseop[opi].size_byte = wxAtoi(argstr[0]);
					parseop[opi].arg_amount = 1+parseop[opi].size_byte*2;
					parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount,&parseop[opi]);
					parseop[opi].arg[0].is_signed = true;
					parseop[opi].arg[0].typesize = 2;
					parseop[opi].arg[0].SetValue(-1);
					for (j=1;j<parseop[opi].arg_amount;j++) {
						parseop[opi].arg[j].is_signed = j%2==0;
						parseop[opi].arg[j].typesize = 2;
						parseop[opi].arg[j].SetValue(0);
					}
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos+4-parseop[opi-1].size,opi-2)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 8: { // case
					if (lvltype[lvlamount-1]!=7 && lvltype[lvlamount-1]!=8) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOTSWITCH),line,L"default");
						res.AddError(errstr.ToStdWstring());
						break;
					}
					bool endenum = false;
					token = GetNextThing(linestr);
					while (!endenum) {
						if (lvltype[lvlamount-1]==7) {
							if (token[0]!=L'+') {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L"+");
								res.AddError(errstr.ToStdWstring());
								break;
							}
							token = token.Mid(1);
						}
						MACRO_CHECK_NUMBER_ERROR()
						if (lvlcaseamount[lvlamount-1]>=parseop[lvloppos[lvlamount-1]+1].size_byte) {
							errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE),line);
							res.AddError(errstr.ToStdWstring());
							break;
						}
						if (lvltype[lvlamount-1]==7) {
							if (wxAtoi(token)>=parseop[lvloppos[lvlamount-1]+1].size_byte || wxAtoi(token)<0) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE_RANGE),line);
								res.AddError(errstr.ToStdWstring());
								break;
							}
							if (parseop[lvloppos[lvlamount-1]+1].arg[2+wxAtoi(token)].GetValue()!=-1) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE_TWICE),line,token);
								res.AddError(errstr.ToStdWstring());
								break;
							}
							parseop[lvloppos[lvlamount-1]+1].arg[2+wxAtoi(token)].SetValue(rawpos-lvlrawpos[lvlamount-1]);
						} else {
							parseop[lvloppos[lvlamount-1]+1].arg[1+lvlcaseamount[lvlamount-1]*2].SetValue(wxAtoi(token));
							parseop[lvloppos[lvlamount-1]+1].arg[2+lvlcaseamount[lvlamount-1]*2].SetValue(rawpos-lvlrawpos[lvlamount-1]);
						}
						lvlcaseamount[lvlamount-1]++;
						token = GetNextThing(linestr);
						if (token.IsSameAs(L";"))
							token = GetNextThing(linestr);
						else if (token.IsSameAs(L":"))
							endenum = true;
						else {
							errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L";' or ':");
							res.AddError(errstr.ToStdWstring());
							break;
						}
					}
					if (!endenum)
						break;
					MACRO_CHECK_TRAILING_WARNING()
					break;
				}
				case 9: // default
					if (lvltype[lvlamount-1]!=7 && lvltype[lvlamount-1]!=8) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOTSWITCH),line,L"default");
						res.AddError(errstr.ToStdWstring());
						break;
					}
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L":")
					if (lvltype[lvlamount-1]==7)
						parseop[lvloppos[lvlamount-1]+1].arg[1].SetValue(rawpos-lvlrawpos[lvlamount-1]);
					else
						parseop[lvloppos[lvlamount-1]+1].arg[0].SetValue(rawpos-lvlrawpos[lvlamount-1]);
					for (i=0;i<parseop[lvloppos[lvlamount-1]+1].size_byte;i++)
						if (parseop[lvloppos[lvlamount-1]+1].arg[2+i].GetValue()==-1)
							parseop[lvloppos[lvlamount-1]+1].arg[2+i].SetValue(rawpos-lvlrawpos[lvlamount-1]);
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 10: // break
					if (breaklvlamount>0) {
						MACRO_NEW_OPCODE(0x01)
						parseop[opi].arg[0].SetValue(0);
						breaklvloppos[breaklvlamount-1][breaklvli[breaklvlamount-1]] = opi;
						MACRO_OPCODE_SIZE_DONE()
						breaklvlrawpos[breaklvlamount-1][breaklvli[breaklvlamount-1]] = rawpos;
						breaklvli[breaklvlamount-1]++;
						MACRO_CHECK_TRAILING_WARNING()
					} else {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOBREAK),line,L"break");
						res.AddError(errstr.ToStdWstring());
					}
					break;
				case 11: // Function
					break;
				}
				break;
			}
		if (tokentype<0 && token.IsSameAs(L"}")) {
			tokentype = 0x80;
			if (lvlamount==0) {
				errstr.Printf(wxT(HADES_STRING_SCRIPT_BLOCKTOOMANY),line);
				res.AddError(errstr.ToStdWstring());
				break;
			}
			lvlamount--;
			switch (lvltype[lvlamount]) {
			case 1: // if
				token = GetNextThing(linestr);
				if (token.IsSameAs(L"else")) {
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_CHECK_TRAILING_WARNING()
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					parseop[lvloppos[lvlamount]+1].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
					tokentype = 2;
					MACRO_RISE_LEVEL(rawpos,opi-1)
				} else {
					parseop[lvloppos[lvlamount]+1].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
				}
				break;
			case 2: // else
				parseop[lvloppos[lvlamount]].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
				break;
			case 3: // ifnot
				parseop[lvloppos[lvlamount]+1].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
				break;
			case 4: // while
				parseop[lvloppos[lvlamount]].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
				MACRO_NEW_OPCODE(0x05)
				parseop[opi].arg[0].SetValueVar(lvlargval[lvlamount],lvlargvalsize[lvlamount]);
				MACRO_OPCODE_SIZE_DONE()
				MACRO_NEW_OPCODE(0x03)
				parseop[opi].arg[0].SetValue(lvlrawpos[lvlamount]-(rawpos+3));
				MACRO_OPCODE_SIZE_DONE()
				breaklvlamount--;
				for (i=0;i<breaklvli[breaklvlamount];i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos-breaklvlrawpos[breaklvlamount][i]);
				break;
			case 5: // do
				token = GetNextThing(linestr);
				MACRO_CHECK_PUNC_ERROR(L"while")
				token = GetNextPunc(linestr);
				MACRO_CHECK_PUNC_ERROR(L"(")
				token = GetNextArg(linestr);
				MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
				token = GetNextPunc(linestr);
				MACRO_CHECK_PUNC_ERROR(L")")
				MACRO_NEW_OPCODE(0x05)
				parseop[opi].arg[0].SetValueVar(argval,argvalsize);
				MACRO_OPCODE_SIZE_DONE()
				MACRO_NEW_OPCODE(0x03)
				parseop[opi].arg[0].SetValue(lvlrawpos[lvlamount]-(rawpos+3));
				MACRO_OPCODE_SIZE_DONE()
				breaklvlamount--;
				for (i=0;i<breaklvli[breaklvlamount];i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos-breaklvlrawpos[breaklvlamount][i]);
				break;
			case 7: // switch
				for (i=0;i<parseop[lvloppos[lvlamount]+1].size_byte;i++)
					if (parseop[lvloppos[lvlamount]+1].arg[2+i].GetValue()==-1)
						parseop[lvloppos[lvlamount]+1].arg[2+i].SetValue(rawpos-lvlrawpos[lvlamount]);
				if (parseop[lvloppos[lvlamount]+1].arg[1].GetValue()==-1)
					parseop[lvloppos[lvlamount]+1].arg[1].SetValue(rawpos-lvlrawpos[lvlamount]);
				breaklvlamount--;
				for (i=0;i<breaklvli[breaklvlamount];i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos-breaklvlrawpos[breaklvlamount][i]);
				break;
			case 8: // switchex
				if (lvlcaseamount[lvlamount]<parseop[lvloppos[lvlamount]+1].size_byte) {
					errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE),line);
					res.AddError(errstr.ToStdWstring());
					break;
				}
				if (parseop[lvloppos[lvlamount]+1].arg[0].GetValue()==-1)
					parseop[lvloppos[lvlamount]+1].arg[0].SetValue(rawpos-lvlrawpos[lvlamount]);
				breaklvlamount--;
				for (i=0;i<breaklvli[breaklvlamount];i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos-breaklvlrawpos[breaklvlamount][i]);
				break;
			}
			MACRO_CHECK_TRAILING_WARNING()
		}
		if (tokentype<0) {
			for (i=0;i<G_N_ELEMENTS(HADES_STRING_SCRIPT_OPCODE);i++) {
				if (token.IsSameAs(HADES_STRING_SCRIPT_OPCODE[i].label)) {
					tokentype = 0x100+i;
					switch (i) {
					case 0x04:
						MACRO_NEW_OPCODE(0x04)
						MACRO_OPCODE_SIZE_DONE()
						break;
					case 0x05:
						token = linestr;
						linestr = wxEmptyString;
						token.Trim(true);
						token.Trim(false);
						MACRO_CHECK_VARARG_ERROR(argval,argvalsize)
						MACRO_NEW_OPCODE(0x05)
						parseop[opi].arg[0].SetValueVar(argval,argvalsize);
						MACRO_OPCODE_SIZE_DONE()
						break;
					case 0x29:
						token = GetNextPunc(linestr);
						MACRO_CHECK_PUNC_ERROR(L"(")
						islastarg = false;
						varargbyte = 0;
						j = 0;
						while (!islastarg) {
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L"(")
							token = GetNextThing(linestr);
							MACRO_CHECK_NUMBER_ERROR()
							argstr[j++] = token;
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L",")
							token = GetNextThing(linestr);
							MACRO_CHECK_NUMBER_ERROR()
							argstr[j++] = token;
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L")")
							// ToDo : can't use vararg with FIELD_REGION opcode for now
/*							token = GetNextArg(linestr);
							if (!token.IsNumber()) {
								MACRO_CHECK_VARARG_ERROR(argvalarray[j],argvalarraysize[j])
								varargbyte |= 0x1 << j;
							}
							argstr[j++] = token;*/
							token = GetNextPunc(linestr);
							if (token.IsSameAs(L")"))
								islastarg = true;
							else if (!token.IsSameAs(L",")) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L",' or ')");
								res.AddError(errstr.ToStdWstring());
								break;
							}
						}
						if (!islastarg)
							break;
						MACRO_NEW_OPCODE(0x29)
						parseop[opi].vararg_flag = varargbyte;
						parseop[opi].size_byte = j/2;
						parseop[opi].arg_amount = j/2;
						parseop[opi].arg = NewScriptArgumentArray(j/2,&parseop[opi]);
						for (j=0;j<parseop[opi].arg_amount;j++) {
							parseop[opi].arg[j].is_signed = true;
							parseop[opi].arg[j].typesize = 4;
							if (argstr[2*j].IsNumber())
								parseop[opi].arg[j].SetValue((wxAtoi(argstr[2*j]) & 0xFFFF) | ((wxAtoi(argstr[2*j+1]) & 0xFFFF) << 16));
							else
								parseop[opi].arg[j].SetValueVar(argvalarray[2*j],argvalarraysize[2*j]);
						}
						MACRO_OPCODE_SIZE_DONE()
						break;
					default:
						token = GetNextPunc(linestr);
						MACRO_CHECK_PUNC_ERROR(L"(")
						if (HADES_STRING_SCRIPT_OPCODE[i].arg_amount>0) {
							islastarg = false;
							varargbyte = 0;
							j = 0;
							while (!islastarg) {
								token = GetNextArg(linestr);
								if (!token.IsNumber()) {
									MACRO_CHECK_VARARG_ERROR(argvalarray[j],argvalarraysize[j])
									varargbyte |= 0x1 << j;
								}
								argstr[j++] = token;
								token = GetNextPunc(linestr);
								if (token.IsSameAs(L")"))
									islastarg = true;
								else if (!token.IsSameAs(L",")) {
									errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L",' or ')");
									res.AddError(errstr.ToStdWstring());
									break;
								}
							}
							if (!islastarg)
								break;
							islastarg = false; // becomes a check for errors
							if (j!=HADES_STRING_SCRIPT_OPCODE[i].arg_amount) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_ARGAMOUNT),line,_(HADES_STRING_SCRIPT_OPCODE[i].label),HADES_STRING_SCRIPT_OPCODE[i].arg_amount,j);
								res.AddError(errstr.ToStdWstring());
								islastarg = true;
							}
							if (!HADES_STRING_SCRIPT_OPCODE[i].use_vararg && varargbyte>0) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_NOVARARG),line,_(HADES_STRING_SCRIPT_OPCODE[i].label));
								res.AddError(errstr.ToStdWstring());
								islastarg = true;
							}
							if (islastarg)
								break;
						} else {
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L")")
						}
						MACRO_NEW_OPCODE(i)
						if (HADES_STRING_SCRIPT_OPCODE[i].use_vararg)
							parseop[opi].vararg_flag = varargbyte;
						for (j=0;j<parseop[opi].arg_amount;j++) {
							if (argstr[j].IsNumber())
								parseop[opi].arg[j].SetValue(wxAtoi(argstr[j]));
							else
								parseop[opi].arg[j].SetValueVar(argvalarray[j],argvalarraysize[j]);
						}
						MACRO_OPCODE_SIZE_DONE()
						break;
					}
					MACRO_CHECK_TRAILING_WARNING()
				}
			}
		}
		if (tokentype<0) {
			errstr.Printf(wxT(HADES_STRING_SCRIPT_UNKNOWN),line,token);
			res.AddError(errstr.ToStdWstring());
		}
	}
	if (lvlamount>0) {
		errstr.Printf(wxT(HADES_STRING_SCRIPT_BLOCKMISSING),line,lvlamount);
		res.AddError(errstr.ToStdWstring());
	}
	parseopamount = opi;
	parseoplength = rawpos;
	if (res.ok) {
		unsigned int localam = 0, globalam = 0;
		bool newglobal;
		for (i=0;i<localvar->amount;i++)
			if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
				localam++;
			else if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
				globalam++;
		script.entry_local_var[entry] = localvar->allocate_amount;
		script.local_data[entry].allocate_amount = localvar->allocate_amount;
		script.local_data[entry].amount = localam;
		script.local_data[entry].local_type = new uint8_t[localam];
		script.local_data[entry].type = new int16_t[localam];
		script.local_data[entry].size = new uint8_t[localam];
		script.local_data[entry].name = new wstring[localam];
		script.local_data[entry].cat = new uint8_t[localam];
		script.local_data[entry].id = new uint16_t[localam];
		script.global_data.allocate_amount = 0;
		script.global_data.amount = globalam;
		script.global_data.local_type = new uint8_t[globalam];
		script.global_data.type = new int16_t[globalam];
		script.global_data.size = new uint8_t[globalam];
		script.global_data.name = new wstring[globalam];
		script.global_data.cat = new uint8_t[globalam];
		script.global_data.id = new uint16_t[globalam];
		localam = 0;
		globalam = 0;
		for (i=0;i<localvar->amount;i++)
			if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
				script.local_data[entry].local_type[localam] = localvar->local_type[i];
				script.local_data[entry].type[localam] = localvar->type[i];
				script.local_data[entry].size[localam] = localvar->size[i];
				script.local_data[entry].name[localam] = localvar->name[i];
				script.local_data[entry].cat[localam] = localvar->cat[i];
				script.local_data[entry].id[localam] = localvar->id[i];
				localam++;
			} else if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
				script.global_data.local_type[globalam] = localvar->local_type[i];
				script.global_data.type[globalam] = localvar->type[i];
				script.global_data.size[globalam] = localvar->size[i];
				script.global_data.name[globalam] = localvar->name[i];
				script.global_data.cat[globalam] = localvar->cat[i];
				script.global_data.id[globalam] = localvar->id[i];
				globalam++;
			}
		delete localvar;
		UpdateGlobalLocalStrings(entry);
	}
	return res;
}

//=============================//
//          Code List          //
//=============================//

class ScriptHelpDialog : public ScriptHelpWindow {
public:
	wxArrayString func_list;
	wxArrayString var_list;
	wxArrayString varcode_list;
	void** func_list_item;
	void** var_list_item;
	void** varcode_list_item;
	
	ScriptHelpDialog(ScriptEditDialog* p);
	~ScriptHelpDialog();
	void DisplayHelpFunction(SortedChoiceItemScriptOpcode* item);
	void DisplayHelpVariable(VariableName* item);
	void DisplayHelpVarCode(VariableOperation* item);
	void SearchUpdate();

private:
	ScriptEditDialog* parent;
	
	void OnSearch(wxCommandEvent& event);
	void OnSearchButton(wxMouseEvent& event);
	void OnListClick(wxCommandEvent& event);
	void OnListDoubleClick(wxCommandEvent& event);
};

ScriptHelpDialog::ScriptHelpDialog(ScriptEditDialog* p) :
	ScriptHelpWindow(p),
	parent(p) {
	unsigned int i,j;
	j = 0;
	func_list.Alloc(G_N_ELEMENTS(HADES_STRING_SCRIPT_OPCODE));
	func_list_item = (void**)new SortedChoiceItemScriptOpcode*[G_N_ELEMENTS(HADES_STRING_SCRIPT_OPCODE)];
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_SCRIPT_OPCODE);i++)
		if (HADES_STRING_SCRIPT_OPCODE[i].id>=0x07 && HADES_STRING_SCRIPT_OPCODE[i].id!=0x0B && HADES_STRING_SCRIPT_OPCODE[i].id<0xFF) {
			func_list.Add(HADES_STRING_SCRIPT_OPCODE[i].label);
			func_list_item[j] = (void*)&HADES_STRING_SCRIPT_OPCODE[i];
			m_listfunction->Append(func_list[j],func_list_item[j]);
			j++;
		}
	var_list.Alloc(G_N_ELEMENTS(VarNameList));
	var_list_item = (void**)new VariableName*[G_N_ELEMENTS(VarNameList)];
	for (i=0;i<G_N_ELEMENTS(VarNameList);i++) {
		var_list.Add(VarNameList[i].name);
		var_list_item[i] = (void*)&VarNameList[i];
		m_listvariable->Append(var_list[i],var_list_item[i]);
	}
	j = 0;
	varcode_list.Alloc(G_N_ELEMENTS(VarOpList)+G_N_ELEMENTS(VarEntryPropList));
	varcode_list_item = (void**)new VariableOperation*[G_N_ELEMENTS(VarOpList)+G_N_ELEMENTS(VarEntryPropList)];
	varcode_list.Add(_(L"[DATA_ACCESS]"));
	varcode_list_item[j] = (void*)&VarOpList[0x29];
	m_listvarcode->Append(varcode_list[j],varcode_list_item[j]);
	j++;
	for (i=0;i<G_N_ELEMENTS(VarOpList);i++)
		if (VarOpList[i].type==0 || VarOpList[i].type==2 || VarOpList[i].type==50 || VarOpList[i].type==51 || VarOpList[i].type==55) {
			varcode_list.Add(_(VarOpList[i].opstring));
			varcode_list_item[j] = (void*)&VarOpList[i];
			m_listvarcode->Append(varcode_list[j],varcode_list_item[j]);
			j++;
		}
	for (i=0;i<G_N_ELEMENTS(VarEntryPropList);i++) {
		varcode_list.Add(_(VarEntryPropList[i].opstring));
		varcode_list_item[j] = (void*)&VarEntryPropList[i];
		m_listvarcode->Append(varcode_list[j],varcode_list_item[j]);
		j++;
	}
	wxImage tmpimg = wxBITMAP(findglass_image).ConvertToImage();
	m_searchbtn->SetBitmap(tmpimg.Rescale(16,16,wxIMAGE_QUALITY_HIGH));
}

ScriptHelpDialog::~ScriptHelpDialog() {
}

void ScriptHelpDialog::DisplayHelpFunction(SortedChoiceItemScriptOpcode* item) {
	m_helptextctrl->SetValue(item->help);
}

void ScriptHelpDialog::DisplayHelpVariable(VariableName* item) {
	m_helptextctrl->SetValue(item->name+_(L"\n")+_(VarOpList[item->cat].opstring)+wxString::Format(wxT("%u"),item->id)+_(L"\n\n")+_(item->description));
}

void ScriptHelpDialog::DisplayHelpVarCode(VariableOperation* item) {
	if (item->type==3) {
		wxString desc = _(item->description);
		for (unsigned int i=0;i<G_N_ELEMENTS(ScriptCharacterField);i++)
			desc += _(ScriptCharacterField[i].name)+_(L"\n");
		desc += _(ARRAY_ADDITIONAL_INFO);
		m_helptextctrl->SetValue(desc);
	} else if (item->type>=1000) {
		wxString desc;
		desc << _(VarOpList[0x78].opstring) << _(L"(") << (unsigned int)(item->type-1000) << _(L", Entry)\n\n") << _(item->description);
		m_helptextctrl->SetValue(desc);
	} else
		m_helptextctrl->SetValue(item->description);
}

void ScriptHelpDialog::SearchUpdate() {
	wxString srch = m_searchtextctrl->GetValue().Lower();
	unsigned int i;
	m_listfunction->Clear();
	for (i=0;i<func_list.Count();i++)
		if (func_list[i].Lower().Find(srch)!=wxNOT_FOUND)
			m_listfunction->Append(func_list[i],func_list_item[i]);
	m_listvariable->Clear();
	for (i=0;i<var_list.Count();i++)
		if (var_list[i].Lower().Find(srch)!=wxNOT_FOUND)
			m_listvariable->Append(var_list[i],var_list_item[i]);
	m_listvarcode->Clear();
	for (i=0;i<varcode_list.Count();i++)
		if (varcode_list[i].Lower().Find(srch)!=wxNOT_FOUND)
			m_listvarcode->Append(varcode_list[i],varcode_list_item[i]);
}

void ScriptHelpDialog::OnSearch(wxCommandEvent& event) {
	SearchUpdate();
}

void ScriptHelpDialog::OnSearchButton(wxMouseEvent& event) {
	SearchUpdate();
}

void ScriptHelpDialog::OnListClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_FUNCTION)
		DisplayHelpFunction(static_cast<SortedChoiceItemScriptOpcode*>(event.GetClientData()));
	else if (id==wxID_VARIABLE)
		DisplayHelpVariable(static_cast<VariableName*>(event.GetClientData()));
	else if (id==wxID_VARCODE)
		DisplayHelpVarCode(static_cast<VariableOperation*>(event.GetClientData()));
}

void ScriptHelpDialog::OnListDoubleClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id==wxID_FUNCTION) {
		SortedChoiceItemScriptOpcode* item = static_cast<SortedChoiceItemScriptOpcode*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->label)+_(L"( "));
		for (i=0;i+1<item->arg_amount;i++)
			parent->m_scripttext->WriteText(_(L"0, "));
		if (item->arg_amount>0)
			parent->m_scripttext->WriteText(_(L"0 "));
		parent->m_scripttext->WriteText(_(L")"));
	} else if (id==wxID_VARIABLE) {
		VariableName* item = static_cast<VariableName*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->name));
	} else if (id==wxID_VARCODE) {
		VariableOperation* item = static_cast<VariableOperation*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->opstring));
		if (item->type==3)
			parent->m_scripttext->WriteText(_(L"[0]"));
		else if (item->type==50 || item->type>=1000)
			parent->m_scripttext->WriteText(_(L"(0)"));
		else if (item->type==51 || item->type==55)
			parent->m_scripttext->WriteText(_(L"(0, 0)"));
	}
}

//=============================//
//  Display Hints & Arguments  //
//=============================//

struct FlagsStruct {
	unsigned int amount;
	wxCheckBox** box;
};
struct DiscFieldStruct {
	wxChoice* field;
	wxChoice* disc;
};
struct PositionStruct {
	int zoom;
	wxPoint drawpt;
	bool use_z;
	int32_t x;
	int32_t y;
	int32_t z;
	
	wxPoint GetAsTranslation() {
		return wxPoint(drawpt.x-POSITION_PANEL_SIZE/2,-drawpt.y+POSITION_PANEL_SIZE/2)*2*zoom/POSITION_PANEL_SIZE;
	}
};


void WorldMapDraw(wxDC& dc, bool drawx, bool drawy, int32_t x, int32_t y) {
	unsigned char* bmpdata = (unsigned char*)malloc(WORLDMAP_PANEL_WIDTH*WORLDMAP_PANEL_HEIGHT*3*sizeof(unsigned char));
	for (unsigned int i=0;i<WORLDMAP_PANEL_WIDTH*WORLDMAP_PANEL_HEIGHT*3;i++)
		bmpdata[i] = 255;
	wxImage mdcimg(WORLDMAP_PANEL_WIDTH,WORLDMAP_PANEL_HEIGHT,bmpdata);
	wxBitmap mdcbmp(mdcimg);
	wxMemoryDC mdc(mdcbmp);
	mdc.DrawBitmap(wxBITMAP(worldmap_image),0,0);
	mdc.SetPen(wxPen(POSITION_POINT_COLOR));
	mdc.SetBrush(wxBrush(POSITION_POINT_COLOR));
	x = (x*(WORLDMAP_PANEL_WIDTH-16)/0x60000L)%(WORLDMAP_PANEL_WIDTH-16);
	y = (y*(WORLDMAP_PANEL_HEIGHT-16)/0x50000L)%(WORLDMAP_PANEL_HEIGHT-16);
	if (x<0)
		x += (WORLDMAP_PANEL_WIDTH-16);
	if (y<0)
		y += (WORLDMAP_PANEL_HEIGHT-16);
	x += 8;
	y = WORLDMAP_PANEL_HEIGHT-8-y;
	if (drawx && drawy)
		mdc.DrawCircle(x,y,2);
	else if (drawx)
		mdc.DrawLine(x,0,x,WORLDMAP_PANEL_HEIGHT);
	else if (drawy)
		mdc.DrawLine(0,y,WORLDMAP_PANEL_WIDTH,y);
	dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
}

void ScriptEditDialog::DisplayFunction(unsigned int entry, unsigned int function) {
	m_scripttext->ChangeValue(func_str[entry][function]);
	m_localvartext->ChangeValue(localvar_str[entry]);
	line_selection = -1;
	text_x_selection = -1;
	DisplayOperation(wxEmptyString);
	entry_selection = entry;
	function_selection = function;
}

void ScriptEditDialog::DisplayOperation(wxString line, bool refreshargcontrol, bool refresharg) {
	wxString tmpstr;
	uint16_t opcode = 0xFFFF;
	unsigned int argi;
	int helpsx,helpsy,i;
	bool cleanarg = true;
	if (arg_control_type && refreshargcontrol) {
		for (i=0;i<arg_amount;i++) {
			arg_label[i]->Destroy();
			arg_control[i]->Destroy();
		}
		delete[] arg_control_type;
		delete[] arg_label;
		delete[] arg_control;
		arg_control_type = NULL;
	}
	tmpstr = GetNextWord(line);
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_SCRIPT_OPCODE);i++)
		if (tmpstr.IsSameAs(HADES_STRING_SCRIPT_OPCODE[i].label)) {
			opcode = HADES_STRING_SCRIPT_OPCODE[i].id;
			break;
		}
	if (refreshargcontrol)
		current_opcode = opcode;
	if (current_opcode==0xFFFF) {
		if (refreshargcontrol) {
			m_helptextctrl->SetValue(_(HADES_STRING_SCRIPT_OPCODE[0].help));
			arg_amount = 0;
			arg_control_type = NULL;
			Layout();
		}
		return;
	}
	if (refreshargcontrol) {
		m_helptextctrl->SetValue(_(HADES_STRING_SCRIPT_OPCODE[current_opcode].help));
		arg_amount = 0;
		for (i=0;i<HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_amount;i++)
			if (HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_NONE && /*
			 */ HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_POSITION_Y && HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_POSITION_Z && /*
			 */ HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_COLOR_MAGENTA && HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_COLOR_YELLOW && /*
			 */ HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_COLOR_GREEN && HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]!=AT_COLOR_BLUE)
				 arg_amount++;
		arg_control_type = new int[arg_amount];
		arg_label = new wxStaticText*[arg_amount];
		arg_control = new wxWindow*[arg_amount];
	}
	wxString arg;
	tmpstr = GetNextPunc(line);
	if (!tmpstr.IsSameAs(L"("))
		cleanarg = false;
	argi = 0;
	if (current_opcode==0x29 && script_type==SCRIPT_TYPE_FIELD) {
		unsigned int ptamount = 0;
		int16_t regionpts[100];
		bool islastarg = false;
		while (!islastarg && cleanarg) {
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L"("))
				cleanarg = false;
			tmpstr = GetNextThing(line);
			regionpts[2*ptamount] = wxAtoi(tmpstr);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			tmpstr = GetNextThing(line);
			regionpts[2*ptamount+1] = wxAtoi(tmpstr);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L")"))
				cleanarg = false;
			tmpstr = GetNextPunc(line);
			if (tmpstr.IsSameAs(L")"))
				islastarg = true;
			else if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			ptamount++;
		}
		if (cleanarg)
			gl_window->DisplayFieldRegion(ptamount,regionpts);
	}
	for (i=0;i<arg_amount;i++) {
		if (refreshargcontrol && HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]!=AT_NONE) {
			arg_label[i] = new wxStaticText(m_argpanel,wxID_ANY,_(HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_help[i]));
			arg_label[i]->Wrap(-1);
			argsizer->Add(arg_label[i],0,wxALL,5);
		}
		tmpstr = GetNextArg(line);
		if (cleanarg)
			arg = tmpstr;
		else
			arg = wxEmptyString;
		if (HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]==AT_NONE) {
			i--;
			tmpstr = GetNextPunc(line);
			if (!(tmpstr.IsSameAs(L",") && i+1<arg_amount) && !(tmpstr.IsSameAs(L")") && i+1==arg_amount))
				cleanarg = false;
			argi++;
			continue;
		}
		if (refreshargcontrol) {
			switch (HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]) {
			case AT_JUMP:
				arg_control[i] = ArgCreateText(arg,i);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_SPIN:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],true);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_USPIN:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_BOOL:
				arg_control[i] = ArgCreateFlag(arg,i);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_BOOLLIST:
				arg_control[i] = ArgCreateFlags(arg,i,8*HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],defaultbool_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_TEXT:
				if (use_text)
					arg_control[i] = ArgCreateChoice(arg,i,NULL,text_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_BATTLE:
				if (use_battle)
					arg_control[i] = ArgCreateChoice(arg,i,battle_id,battle_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_FIELD:
				if (use_field)
					arg_control[i] = ArgCreateChoice(arg,i,field_id,field_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_DISC_FIELD:
				if (use_field)
					arg_control[i] = ArgCreateDiscFieldChoice(arg,i,field_id,field_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_ATTACK:
				if (use_attack)
					arg_control[i] = ArgCreateChoice(arg,i,NULL,attack_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_ITEM:
				if (use_item)
					arg_control[i] = ArgCreateChoice(arg,i,item_id,item_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_MENUTYPE:
				arg_control[i] = ArgCreateChoice(arg,i,NULL,menutype_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_MENU:
				if (static_cast<wxChoice*>(arg_control[0])->GetSelection()==1 && use_character)
					arg_control[i] = ArgCreateChoice(arg,i,character_id,character_str);
				else if (static_cast<wxChoice*>(arg_control[0])->GetSelection()==2)
					arg_control[i] = ArgCreateChoice(arg,i,NULL,shop_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_CHARACTER:
				if (use_character)
					arg_control[i] = ArgCreateFlags(arg,i,PLAYABLE_CHAR_AMOUNT,character_str);
				else
					arg_control[i] = ArgCreateFlags(arg,i,PLAYABLE_CHAR_AMOUNT,defaultbool_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_LCHARACTER:
				if (use_character)
					arg_control[i] = ArgCreateChoice(arg,i,character_id,character_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_ABILITYSET:
				arg_control[i] = ArgCreateChoice(arg,i,NULL,abilityset_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_EQUIPSET:
				arg_control[i] = ArgCreateChoice(arg,i,equipset_id,equipset_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_FMV:
				arg_control[i] = ArgCreateChoice(arg,i,fmv_id,fmv_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_BATTLECODE:
				arg_control[i] = ArgCreateChoice(arg,i,battlecode_id,battlecode_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_MODELCODE:
				arg_control[i] = ArgCreateChoice(arg,i,modelcode_id,modelcode_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_WORLDCODE:
				arg_control[i] = ArgCreateChoice(arg,i,worldcode_id,worldcode_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_SOUNDCODE:
				arg_control[i] = ArgCreateChoice(arg,i,soundcode_id,soundcode_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_SPSCODE:
				arg_control[i] = ArgCreateChoice(arg,i,spscode_id,spscode_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_ENTRY:
				arg_control[i] = ArgCreateChoice(arg,i,entrylist_id,entrylist_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_FUNCTION: {
				arg_control[i] = ArgCreateChoice(arg,i,functionlist_id,functionlist_str);
				if ((argi>0 && HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi-1]==AT_ENTRY) && static_cast<wxChoice*>(arg_control[i-1])->GetSelection()!=wxNOT_FOUND) {
					unsigned int entrysel = static_cast<wxChoice*>(arg_control[i-1])->GetSelection();
					uint16_t funcid = wxAtoi(arg), funclistpos = 0;
					for (unsigned int j=0;j<script.entry_amount;j++)
						for (unsigned int k=0;k<script.entry_function_amount[j];k++) {
							if (entrysel==j && script.function_type[j][k]==funcid)
								static_cast<wxChoice*>(arg_control[i])->SetSelection(funclistpos);
							funclistpos++;
						}
				}
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			}
			case AT_MODEL:
				arg_control[i] = ArgCreateChoice(arg,i,modellist_id,modellist_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_SOUND:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_AUDIO:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_BUBBLESYMBOL:
				arg_control[i] = ArgCreateChoice(arg,i,NULL,bubblesymbol_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_WORLDMAP:
				arg_control[i] = ArgCreateChoice(arg,i,worldmap_id,worldmap_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_TILE:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_TILEANIM:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_ABILITY:
				if (use_ability)
					arg_control[i] = ArgCreateChoice(arg,i,NULL,ability_str);
				else
					arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_DECK:
				arg_control[i] = ArgCreateChoice(arg,i,NULL,deck_str);
				argsizer->Add(arg_control[i],0,wxALL,5);
				break;
			case AT_POSITION_X:
			case AT_COLOR_CYAN:
			case AT_COLOR_RED:
				break;
			default:
				arg_control[i] = ArgCreateSpin(arg,i,HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_length[argi],false);
				argsizer->Add(arg_control[i],0,wxALL,5);
			}
		}
		if (refresharg) {
			if (!refreshargcontrol) {
				switch (arg_control_type[i]) {
				case ARG_CONTROL_TEXT:
					break;
				case ARG_CONTROL_FLAG:
					static_cast<wxCheckBox*>(arg_control[i])->SetValue(wxAtoi(arg)!=0);
					break;
				case ARG_CONTROL_FLAGS: {
					FlagsStruct* flagctrl = static_cast<FlagsStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
					uint32_t flagval = wxAtoi(arg);
					for (unsigned int j=0;j<flagctrl->amount;j++)
						flagctrl->box[j]->SetValue(flagval & (0x1 << j));
					break;
				}
				case ARG_CONTROL_SPIN:
					static_cast<wxSpinCtrl*>(arg_control[i])->SetValue(wxAtoi(arg));
					break;
				case ARG_CONTROL_CHOICE: {
					wxChoice* choicectrl = static_cast<wxChoice*>(arg_control[i]);
					uint16_t choiceval = wxAtoi(arg);
					if (choicectrl->GetClientData(0)) {
						for (unsigned int j=0;j<choicectrl->GetCount();j++)
							if (*(uint16_t*)choicectrl->GetClientData(j)==choiceval) {
								choicectrl->SetSelection(j);
								break;
							}
					} else
						choicectrl->SetSelection(choiceval);
					break;
				}
				case ARG_CONTROL_DISC: {
					DiscFieldStruct* discctrl = static_cast<DiscFieldStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
					uint16_t choicefieldval = wxAtoi(arg) & 0x3FFF;
					if (discctrl->field->GetClientData(0)) {
						for (unsigned int j=0;j<discctrl->field->GetCount();j++)
							if (*(uint16_t*)discctrl->field->GetClientData(j)==choicefieldval) {
								discctrl->field->SetSelection(j);
								break;
							}
					} else
						discctrl->field->SetSelection(choicefieldval);
					discctrl->disc->SetSelection(wxAtoi(arg) >> 14);
					break;
				}
				}
			}
		}
		switch (HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]) {
		case AT_POSITION_X: {
			wxArrayString arrarg;
			bool usez;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			usez = HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]==AT_POSITION_Z;
			if (usez) {
				tmpstr = GetNextPunc(line);
				if (!tmpstr.IsSameAs(L","))
					cleanarg = false;
				argi++;
				tmpstr = GetNextArg(line);
				if (cleanarg)
					arg = tmpstr;
				else
					arg = wxEmptyString;
				arrarg.Add(arg);
			}
			if (refreshargcontrol) {
				arg_control[i] = ArgCreatePosition(arrarg,i);
				argsizer->Add(arg_control[i],0,wxALL,5);
			} else if (refresharg) {
				PositionStruct* posctrl = static_cast<PositionStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
				posctrl->x = wxAtoi(arrarg[0]);
				if (usez) {
					posctrl->z = wxAtoi(arrarg[1]);
					posctrl->y = wxAtoi(arrarg[2]);
				} else
					posctrl->y = wxAtoi(arrarg[1]);
			}
			if (script_type==SCRIPT_TYPE_FIELD) {
				if (usez)
					gl_window->DisplayFieldPoint3D(wxAtoi(arrarg[0]),wxAtoi(arrarg[2]),wxAtoi(arrarg[1]));
				else
					gl_window->DisplayFieldPoint2D(wxAtoi(arrarg[0]),wxAtoi(arrarg[1]));
			} else if (script_type==SCRIPT_TYPE_WORLD) {
				world_pos_type = 3;
				world_pos_x = wxAtoi(arrarg[0]);
				if (usez)
					world_pos_y = wxAtoi(arrarg[2]);
				else
					world_pos_y = wxAtoi(arrarg[1]);
				wxClientDC dc(m_worlddisplaypanel);
				WorldMapDraw(dc,world_pos_type & 1,world_pos_type & 2,world_pos_x,world_pos_y);
			}
			break;
		}
		case AT_COLOR_CYAN:
		case AT_COLOR_RED: {
			bool rgb = HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[argi]==AT_COLOR_RED;
			wxArrayString arrarg;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			if (refreshargcontrol) {
				arg_control[i] = ArgCreateColorPicker(arrarg,i,rgb);
				argsizer->Add(arg_control[i],0,wxALL,5);
			} else if (refresharg) {
				if (rgb)
					static_cast<wxColourPickerCtrl*>(arg_control[i])->SetColour(wxColour(wxAtoi(arrarg[0]),wxAtoi(arrarg[1]),wxAtoi(arrarg[2])));
				else
					static_cast<wxColourPickerCtrl*>(arg_control[i])->SetColour(wxColour(255-wxAtoi(arrarg[0]),255-wxAtoi(arrarg[1]),255-wxAtoi(arrarg[2])));
			}
			break;
		}
		case AT_WALKTRIANGLE:
			gl_window->DisplayFieldTrianglePath(wxAtoi(arg));
			break;
		case AT_WALKPATH:
			gl_window->DisplayFieldPath(wxAtoi(arg));
			break;
		}
		tmpstr = GetNextPunc(line);
		if (!(tmpstr.IsSameAs(L",") && i+1<arg_amount) && !(tmpstr.IsSameAs(L")") && i+1==arg_amount))
			cleanarg = false;
		argi++;
	}
	if (refreshargcontrol) {
		Layout();
		Refresh();
	}
}

void ScriptEditDialog::UpdateLineHelp(long x, long y) {
	wxString line = m_scripttext->GetLineText(y);
	wxString tmp = line;
	size_t relpos = tmp.find_first_of(L"-0123456789"), abspos = relpos, numbegin = 0;
	bool selnum = x==abspos;
	int i;
	while (relpos!=string::npos && abspos<=x) {
		numbegin = abspos;
		tmp = tmp.Mid(relpos);
		relpos = tmp.find_first_not_of(L"-0123456789");
		abspos += relpos;
		if (relpos==string::npos || abspos>=x) {
			selnum = true;
			break;
		}
		tmp = tmp.Mid(relpos);
		relpos = tmp.find_first_of(L"-0123456789");
		abspos += relpos;
	}
	tmp = line.Mid(numbegin,relpos);
	long selectedint = selnum ? wxAtoi(tmp) : 0;
	m_intvaluesignedint->ChangeValue(wxString::Format(wxT("%hd"),selectedint));
	m_intvaluesignedlong->ChangeValue(wxString::Format(wxT("%d"),selectedint));
	m_intvaluebase16->ChangeValue(wxString::Format(wxT("%#.4x"),selectedint));
	wxString base64str = _(L"[ ");
	for (i=0;i<=3;i++) {
		base64str << (unsigned int)((selectedint >> (6*i)) & 0x3F);
		if (i<3)
			base64str << _(L" ; ");
	}
	base64str << _(L" ]");
	m_intvaluebase64->ChangeValue(base64str);
	if (use_item && selectedint>=0) {
		for (i=0;i<item_str.Count();i++)
			if (*item_id[i]==selectedint) {
				m_intvalueitem->ChangeValue(item_str[i]);
				break;
			}
		if (i==item_str.Count())
			m_intvalueitem->ChangeValue(_(HADES_STRING_VOID));
	} else
		m_intvalueitem->ChangeValue(_(HADES_STRING_VOID));
	if (use_attack && selectedint>=0) {
		wxString attackstr = _(L"[ ");
		for (i=0;i<=3;i++) {
			uint8_t atk = (selectedint >> (6*i)) & 0x3F;
			if (atk<enemy->spell_amount)
				attackstr << attack_str[atk];
			else
				attackstr << _(HADES_STRING_VOID);
			if (i<3)
				attackstr << _(L" ; ");
		}
		attackstr << _(L" ]");
		m_intvalueattack->ChangeValue(attackstr);
	} else
		m_intvalueattack->ChangeValue(_(HADES_STRING_VOID));
	if (use_ability && selectedint<SPELL_AMOUNT && selectedint>=0)
		m_intvaluespell->ChangeValue(ability_str[selectedint]);
	else
		m_intvaluespell->ChangeValue(_(HADES_STRING_VOID));
	if (use_command && selectedint+1<COMMAND_AMOUNT+G_N_ELEMENTS(CommandAddendaName) && selectedint>=0)
		m_intvaluecmd->ChangeValue(command_str[selectedint]);
	else
		m_intvaluecmd->ChangeValue(_(HADES_STRING_VOID));
	wxString buttonstr;
	unsigned int btncount = 0;
	for (i=0;i<G_N_ELEMENTS(PlaystationButton);i++) {
		if ((selectedint >> i) & 1) {
			if (btncount>0)
				buttonstr << _(L" | ");
			buttonstr << _(PlaystationButton[i]);
			btncount++;
		}
	}
	m_intvaluebutton->ChangeValue(btncount>0 ? buttonstr : _(HADES_STRING_VOID));
	wxString statusstr;
	unsigned int statuscount = 0;
	for (i=0;i<24;i++) {
		if ((selectedint >> i) & 1) {
			if (statuscount>0)
				statusstr << _(L" | ");
			statusstr << _(HADES_STRING_STATUS[i]);
			statuscount++;
		}
	}
	m_intvaluestatusa->ChangeValue(statuscount>0 ? statusstr : _(HADES_STRING_STATUS_NONE));
	statusstr = wxEmptyString;
	statuscount = 0;
	for (i=24;i<G_N_ELEMENTS(HADES_STRING_STATUS);i++) {
		if ((selectedint >> (i-24)) & 1) {
			if (statuscount>0)
				statusstr << _(L" | ");
			statusstr << _(HADES_STRING_STATUS[i]);
			statuscount++;
		}
	}
	m_intvaluestatusb->ChangeValue(statuscount>0 ? statusstr : _(HADES_STRING_STATUS_NONE));
	if (script_type==SCRIPT_TYPE_FIELD && !refresh_control_disable) {
		gl_window->DisplayFieldClear();
		gl_window->DisplayFieldPlane(m_fieldcoordchoice->GetSelection(),selectedint);
	} else if (script_type==SCRIPT_TYPE_WORLD && !refresh_control_disable) {
		world_pos_type = m_worldcoordchoice->GetSelection();
		world_pos_x = selectedint;
		world_pos_y = selectedint;
		wxClientDC dc(m_worlddisplaypanel);
		WorldMapDraw(dc,world_pos_type & 1,world_pos_type & 2,world_pos_x,world_pos_y);
	}
}

wxCheckBox* ScriptEditDialog::ArgCreateFlag(wxString& arg, unsigned int id) {
	wxCheckBox* res = new wxCheckBox(m_argpanel,SS_ARG_ID+id,wxEmptyString);
	res->SetLabel(_(HADES_STRING_ON_OFF_BUTTON));
	if (arg.IsNumber())
		res->SetValue(wxAtoi(arg));
	res->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(ScriptEditDialog::OnArgFlag),NULL,this);
	arg_control_type[id] = ARG_CONTROL_FLAG;
	return res;
}

wxTextCtrl* ScriptEditDialog::ArgCreateText(wxString& arg, unsigned int id) {
	wxTextCtrl* res = new wxTextCtrl(m_argpanel,SS_ARG_ID+id,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	res->SetLabel(_(HADES_STRING_NYI));
	arg_control_type[id] = ARG_CONTROL_TEXT;
	return res;
}

wxSpinCtrl* ScriptEditDialog::ArgCreateSpin(wxString& arg, unsigned int id, int size, bool sign) {
	wxSpinCtrl* res = new wxSpinCtrl(m_argpanel,SS_ARG_ID+id);
	long long range = (1 << (8*size));
	if (sign)
		res->SetRange(-range/2,range/2-1);
	else
		res->SetRange(0,range-1);
	if (arg.IsNumber())
		res->SetValue(wxAtoi(arg));
	else
		res->SetValue(0);
	res->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,wxSpinEventHandler(ScriptEditDialog::OnArgSpin),NULL,this);
	arg_control_type[id] = ARG_CONTROL_SPIN;
	return res;
}

wxChoice* ScriptEditDialog::ArgCreateChoice(wxString& arg, unsigned int id, uint16_t** choiceid, wxArrayString& choicestr) {
	wxChoice* res = new wxChoice(m_argpanel,SS_ARG_ID+id);
	if (choiceid)
		res->Append(choicestr,(void**)choiceid);
	else
		res->Append(choicestr);
	if (arg.IsNumber()) {
		if (choiceid) {
			for (unsigned int i=0;i<choicestr.Count();i++)
				if (wxAtoi(arg)==*choiceid[i]) {
					res->SetSelection(i);
					break;
				}
		} else {
			res->SetSelection(wxAtoi(arg));
		}
	}
	res->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgChoice),NULL,this);
	arg_control_type[id] = ARG_CONTROL_CHOICE;
	return res;
}

wxPanel* ScriptEditDialog::ArgCreateDiscFieldChoice(wxString& arg, unsigned int id, uint16_t** choiceid, wxArrayString& choicestr) {
	wxPanel* res = new wxPanel(m_argpanel);
	wxGridSizer* grid = new wxGridSizer(0,1,0,0);
	wxChoice* field = new wxChoice(res,SS_ARG_ID+id);
	wxChoice* disc = new wxChoice(res,SS_ARG_ID+id);
	unsigned int i;
	if (choiceid)
		field->Append(choicestr,(void**)choiceid);
	else
		field->Append(choicestr);
	disc->Append(disc_str);
	if (arg.IsNumber()) {
		if (choiceid) {
			for (i=0;i<choicestr.Count();i++)
				if ((wxAtoi(arg) & 0x3FFF)==*choiceid[i]) {
					field->SetSelection(i);
					break;
				}
			if (i==choicestr.Count())
				field->SetSelection(0);
		} else
			field->SetSelection(wxAtoi(arg) & 0x3FFF);
		disc->SetSelection(wxAtoi(arg) >> 14);
	} else {
		field->SetSelection(0);
		disc->SetSelection(0);
	}
	grid->Add(field,0,wxALL,5);
	grid->Add(disc,0,wxALL,5);
	DiscFieldStruct* datagroup = new DiscFieldStruct;
	datagroup->disc = disc;
	datagroup->field = field;
	res->SetClientData((void*)datagroup);
	field->SetClientData((void*)datagroup);
	disc->SetClientData((void*)datagroup);
	field->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgField),NULL,this);
	disc->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgDisc),NULL,this);
	res->SetSizer(grid);
	res->Layout();
	grid->Fit(res);
	arg_control_type[id] = ARG_CONTROL_DISC;
	return res;
}

wxPanel* ScriptEditDialog::ArgCreateFlags(wxString& arg, unsigned int id, unsigned int amount, wxArrayString& flagstr) {
	wxPanel* res = new wxPanel(m_argpanel);
	wxGridSizer* grid = new wxGridSizer(0,4,0,0);
	FlagsStruct* datagroup = new FlagsStruct;
	wxCheckBox* box[amount];
	unsigned int i;
	datagroup->amount = amount;
	datagroup->box = new wxCheckBox*[amount];
	for (i=0;i<amount;i++) {
		box[i] = new wxCheckBox(res,SS_ARG_ID+id*SS_ARGBOX_MAXID+i,wxEmptyString);
		box[i]->SetLabel(flagstr[i]);
		grid->Add(box[i],0,wxALL,5);
		datagroup->box[i] = box[i];
		box[i]->SetClientData((void*)datagroup);
		box[i]->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(ScriptEditDialog::OnArgFlags),NULL,this);
	}
	res->SetClientData((void*)datagroup);
	res->SetSizer(grid);
	res->Layout();
	grid->Fit(res);
	if (arg.IsNumber())
		for (i=0;i<amount;i++)
			box[i]->SetValue(wxAtoi(arg) & (0x1 << i));
	arg_control_type[id] = ARG_CONTROL_FLAGS;
	return res;
}

void ArgPositionDraw(wxDC& dc, PositionStruct* posdata) {
	unsigned char* bmpdata = (unsigned char*)malloc(POSITION_PANEL_SIZE*POSITION_PANEL_SIZE*3*sizeof(unsigned char));
	for (unsigned int i=0;i<POSITION_PANEL_SIZE*POSITION_PANEL_SIZE*3;i++)
		bmpdata[i] = 255;
	wxImage mdcimg(POSITION_PANEL_SIZE,POSITION_PANEL_SIZE,bmpdata);
	wxBitmap mdcbmp(mdcimg);
	wxMemoryDC mdc(mdcbmp);
	mdc.SetFont(wxFont(wxFontInfo(8)));
	mdc.DrawLine(POSITION_PANEL_SIZE/2,0,POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE);
	mdc.DrawLine(0,POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE,POSITION_PANEL_SIZE/2);
	mdc.DrawText(wxString::Format(wxT("%d"),-2*posdata->zoom/3),POSITION_PANEL_SIZE/6-15,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),-posdata->zoom/3),POSITION_PANEL_SIZE/3-10,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),posdata->zoom/3),2*POSITION_PANEL_SIZE/3-10,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2-3,2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),2*posdata->zoom/3),5*POSITION_PANEL_SIZE/6-15,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2-3,5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),2*posdata->zoom/3),POSITION_PANEL_SIZE/2+5,POSITION_PANEL_SIZE/6-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4,POSITION_PANEL_SIZE/6);
	mdc.DrawText(wxString::Format(wxT("%d"),posdata->zoom/3),POSITION_PANEL_SIZE/2+5,POSITION_PANEL_SIZE/3-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4,POSITION_PANEL_SIZE/3);
	mdc.DrawText(wxString::Format(wxT("%d"),-posdata->zoom/3),POSITION_PANEL_SIZE/2+5,2*POSITION_PANEL_SIZE/3-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4,2*POSITION_PANEL_SIZE/3);
	mdc.DrawText(wxString::Format(wxT("%d"),-2*posdata->zoom/3),POSITION_PANEL_SIZE/2+5,5*POSITION_PANEL_SIZE/6-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4,5*POSITION_PANEL_SIZE/6);
	mdc.SetPen(wxPen(POSITION_POINT_COLOR));
	mdc.SetBrush(wxBrush(POSITION_POINT_COLOR));
	mdc.DrawCircle(posdata->drawpt,5);
	dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
}

wxPanel* ScriptEditDialog::ArgCreatePosition(wxArrayString& arg, unsigned int id) {
	wxPanel* res = new wxPanel(m_argpanel,SS_ARG_ID+id,wxDefaultPosition,wxSize(POSITION_PANEL_SIZE,POSITION_PANEL_SIZE));
	PositionStruct* posdata = new PositionStruct;
	posdata->zoom = script_type==SCRIPT_TYPE_WORLD ? 2400 : 150;
	posdata->drawpt = wxPoint(POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE/2);
	posdata->use_z = arg.Count()==3;
	posdata->x = wxAtoi(arg[0]);
	if (posdata->use_z) {
		posdata->y = wxAtoi(arg[2]);
		posdata->z = wxAtoi(arg[1]);
	} else
		posdata->y = wxAtoi(arg[1]);
	res->SetClientData((void*)posdata);
	res->Connect(wxEVT_PAINT,wxPaintEventHandler(ScriptEditDialog::OnArgPositionPaint),NULL,this);
	res->Connect(wxEVT_MOTION,wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseMove),NULL,this);
	res->Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseMove),NULL,this);
	res->Connect(wxEVT_LEFT_UP,wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseUp),NULL,this);
	res->Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseWheel),NULL,this);
	res->Connect(wxEVT_CHAR_HOOK,wxKeyEventHandler(ScriptEditDialog::OnArgPositionKeyboard),NULL,this);
	arg_control_type[id] = ARG_CONTROL_POSITION;
	return res;
}

wxColourPickerCtrl* ScriptEditDialog::ArgCreateColorPicker(wxArrayString& arg, unsigned int id, bool rgb) {
	wxColourPickerCtrl* res = new wxColourPickerCtrl(m_argpanel,SS_ARG_ID+id);
	if (rgb) {
		res->SetColour(wxColour(wxAtoi(arg[0]),wxAtoi(arg[1]),wxAtoi(arg[2])));
		arg_control_type[id] = ARG_CONTROL_COLOR_RGB;
	} else {
		res->SetColour(wxColour(255-wxAtoi(arg[0]),255-wxAtoi(arg[1]),255-wxAtoi(arg[2])));
		arg_control_type[id] = ARG_CONTROL_COLOR_CMY;
	}
	res->Connect(wxEVT_COLOURPICKER_CHANGED,wxColourPickerEventHandler(ScriptEditDialog::OnArgColorPicker),NULL,this);
	return res;
}

void ScriptEditDialog::ScriptChangeArg(int argi, int64_t value, int argshift) {
	wxString scripttxt = m_scripttext->GetLineText(line_selection);
	size_t argpos = scripttxt.Length();
	wxString token, newtxt = scripttxt;
	int shiftedargi = argi;
	int i;
	GetNextWord(scripttxt);
	token = GetNextPunc(scripttxt);
	if (!token.IsSameAs(L'('))
		return;
	for (i=0;i<shiftedargi;i++) {
		GetNextArg(scripttxt);
		token = GetNextPunc(scripttxt);
		if (!token.IsSameAs(L','))
			return;
		if (current_opcode!=0xFFFF && i<HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_amount) {
			if (HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_POSITION_X || HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_POSITION_Z || /*
			 */ HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_COLOR_CYAN || HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_COLOR_MAGENTA || /*
			 */ HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_COLOR_RED || HADES_STRING_SCRIPT_OPCODE[current_opcode].arg_type[i]==AT_COLOR_GREEN)
				 shiftedargi++;
		}
	}
	for (i=0;i<argshift;i++) {
		GetNextArg(scripttxt);
		token = GetNextPunc(scripttxt);
		if (!token.IsSameAs(L','))
			return;
	}
	scripttxt.Trim(false);
	argpos -= scripttxt.Length();
	GetNextArg(scripttxt);
	newtxt = newtxt.Mid(0,argpos);
	newtxt << value;
	if (argi+1==arg_amount)
		newtxt += _(L" ");
	newtxt += scripttxt;
	long from = m_scripttext->XYToPosition(0,line_selection);
	long to = from+m_scripttext->GetLineLength(line_selection);
	m_scripttext->Replace(from,to,newtxt);
}

void ScriptEditDialog::OnFunctionChoose(wxListEvent& event) {
	unsigned int entryid = 0, funcid = 0;
	long sel = event.GetIndex();
	while (script.entry_function_amount[entryid]==0)
		entryid++;
	while (sel>0) {
		funcid++;
		while (funcid>=script.entry_function_amount[entryid]) {
			entryid++;
			funcid = 0;
		}
		sel--;
	}
	if (entry_selection!=SCRIPT_ID_NO_ENTRY) {
		localvar_str[entry_selection] = m_localvartext->GetValue();
		func_str[entry_selection][function_selection] = m_scripttext->GetValue();
	}
	DisplayFunction(entryid,funcid);
}

void ScriptEditDialog::OnIntValueText(wxCommandEvent& event) {
	int id = event.GetId();
	wxString intvalue = _(L"");
	wxString txtvalue = event.GetString();
	unsigned int i,j;
	if (id==wxID_INT) {
		uint16_t val = wxAtoi(txtvalue);
		intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_LONG) {
		uint32_t val = wxAtoi(txtvalue);
		intvalue = wxString::Format(wxT("%uL"),wxAtoi(txtvalue));
	} else if (id==wxID_HEXA) {
		long val;
		txtvalue.ToLong(&val,16);
		val = abs(val);
		if (val>0x7FFF)
			intvalue = wxString::Format(wxT("%uL"),val);
		else
			intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_ITEM) {
		for (i=0;i<item_str.Count();i++)
			if (item_str[i].IsSameAs(txtvalue,false)) {
				intvalue = wxString::Format(wxT("%u"),*item_id[i]);
				break;
			}
	} else if (id==wxID_BUTTON) {
		wxArrayString tokens = wxStringTokenize(txtvalue,L"|",wxTOKEN_STRTOK);
		long val = 0;
		for (i=0;i<tokens.Count();i++) {
			RemoveSurroundingSpaces(tokens[i]);
			for (j=0;j<G_N_ELEMENTS(PlaystationButton);j++) {
				if (tokens[i].IsSameAs(PlaystationButton[j],false)) {
					val |= (1 << j);
					break;
				}
			}
		}
		if (val>0x7FFF)
			intvalue = wxString::Format(wxT("%uL"),val);
		else if (val>0)
			intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_STATUSA || id==wxID_STATUSB) {
		wxArrayString tokens = wxStringTokenize(txtvalue,L"|",wxTOKEN_STRTOK);
		long val = 0;
		unsigned int min = id==wxID_STATUSA ? 0 : 24;
		unsigned int max = id==wxID_STATUSA ? 24 : G_N_ELEMENTS(HADES_STRING_STATUS);
		for (i=0;i<tokens.Count();i++) {
			RemoveSurroundingSpaces(tokens[i]);
			for (j=min;j<max;j++) {
				if (tokens[i].IsSameAs(HADES_STRING_STATUS[j],false)) {
					val |= (1 << (j-min));
					break;
				}
			}
		}
		if (val>0x7FFF)
			intvalue = wxString::Format(wxT("%uL"),val);
		else if (val>0)
			intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_LIST) {
		wxArrayString tokens = wxStringTokenize(txtvalue,L";");
		long val = 0;
		for (i=0;i<tokens.Count();i++) {
			if (tokens[i][0]==L'[')
				tokens[i] = tokens[i].Mid(1);
			if (tokens[i].Last()==L']')
				tokens[i] = tokens[i].Mid(0,tokens[i].Len()-1);
			RemoveSurroundingSpaces(tokens[i]);
			val |= ((wxAtoi(tokens[i]) & 0x3F) << (i*6));
		}
		if (val>0x7FFF)
			intvalue = wxString::Format(wxT("%uL"),val);
		else
			intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_SPLIST) {
		wxArrayString tokens = wxStringTokenize(txtvalue,L";");
		long val = 0;
		for (i=0;i<tokens.Count();i++) {
			if (tokens[i][0]==L'[')
				tokens[i] = tokens[i].Mid(1);
			if (tokens[i].Last()==L']')
				tokens[i] = tokens[i].Mid(0,tokens[i].Len()-1);
			RemoveSurroundingSpaces(tokens[i]);
			for (j=0;j<enemy->spell_amount;j++) {
				if (tokens[i].IsSameAs(attack_str[j],false)) {
					val |= (j << (i*6));
					break;
				}
			}
		}
		if (val>0x7FFF)
			intvalue = wxString::Format(wxT("%uL"),val);
		else
			intvalue = wxString::Format(wxT("%u"),val);
	} else if (id==wxID_SPELL) {
		for (i=0;i<ability_str.Count();i++)
			if (ability_str[i].IsSameAs(txtvalue,false)) {
				intvalue = wxString::Format(wxT("%u"),i);
				break;
			}
	} else if (id==wxID_CMD) {
		for (i=0;i<command_str.Count();i++)
			if (command_str[i].IsSameAs(txtvalue,false)) {
				intvalue = wxString::Format(wxT("%u"),i);
				break;
			}
	}
	m_scripttext->WriteText(intvalue);
	if (intvalue.Last()==L'L')
		m_scripttext->SetInsertionPoint(m_scripttext->GetInsertionPoint()-1);
}

void ScriptEditDialog::OnFunctionRightClick(wxListEvent& event) {
	unsigned int entryid = 0, funcid = 0;
	long sel = event.GetIndex();
	if (sel!=wxNOT_FOUND) {
		while (script.entry_function_amount[entryid]==0)
			entryid++;
		while (sel>0) {
			funcid++;
			while (funcid>=script.entry_function_amount[entryid]) {
				entryid++;
				funcid = 0;
			}
			sel--;
		}
		m_functionlist->SetItemState(event.GetItem(), wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		m_functionlist->PopupMenu(func_popup_menu);
	}
}

void ScriptEditDialog::OnFunctionRightClickMenu(wxCommandEvent& event) {
	long sel = m_functionlist->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	long seltmp = sel;
	if (sel==-1)
		return;
	unsigned int entryid = 0, funcid = 0;
	while (script.entry_function_amount[entryid]==0)
		entryid++;
	while (seltmp>0) {
		funcid++;
		while (funcid>=script.entry_function_amount[entryid]) {
			entryid++;
			funcid = 0;
		}
		seltmp--;
	}
	int id = event.GetId();
	unsigned int i;
	if (id==wxID_ADD) {
		if (extra_size<8) {
			wxMessageDialog popup(NULL,HADES_STRING_MISSING_SPACE,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popup.ShowModal();
			return;
		}
		ScriptEditPropertiesWindow dial(this);
		for (i=0;i<script.entry_amount;i++)
			dial.m_entryctrl->Append(entry_name[i]);
		dial.m_entryctrl->SetSelection(entryid);
		dial.m_entrytypectrl->SetValue(script.entry_type[entryid]);
		dial.m_entrytypectrl->Enable(false);
		dial.m_typectrl->SetValue(script.function_type[entryid][funcid]);
		if (dial.ShowModal()==wxID_OK) {
			if (entryid!=dial.m_entryctrl->GetSelection()) {
				entryid = dial.m_entryctrl->GetSelection();
				funcid = script.entry_function_amount[entryid];
				seltmp = 0;
				sel = 0;
				while (seltmp<entryid) {
					sel += script.entry_function_amount[seltmp];
					seltmp++;
				}
				seltmp = 0;
				while (seltmp<funcid) {
					sel++;
					seltmp++;
				}
			} else {
				funcid++;
				sel++;
			}
			wxArrayString newfuncstr(script.entry_function_amount[entryid],func_str[entryid]);
//			script.entry_type[entryid] = dial.m_entrytypectrl->GetValue();
			if (script.entry_function_amount[entryid]==0) {
				extra_size -= 8;
				if (script.entry_type[entryid]==0xFF)
					script.entry_type[entryid] = 0;
			} else
				extra_size -= 4;
			script.AddFunction(entryid,funcid,dial.m_typectrl->GetValue());
			DisplayFunctionList(false,sel,-1);
			newfuncstr.Insert(functionlist_str[sel]+_(L"\n"),funcid);
			delete[] func_str[entryid];
			func_str[entryid] = new wxString[script.entry_function_amount[entryid]];
			for (i=0;i<script.entry_function_amount[entryid];i++)
				func_str[entryid][i] = newfuncstr[i];
			m_functionlist->SetItemState(sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	} else if (id==wxID_REMOVE) {
		if (entryid==0 && funcid==0) {
			wxMessageDialog popup(NULL,HADES_STRING_SCRIPT_NO_DELETE,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popup.ShowModal();
			return;
		}
		extra_size += script.RemoveFunction(entryid,funcid);
		DisplayFunctionList(false,-1,sel);
		for (i=funcid;i<script.entry_function_amount[entryid];i++)
			func_str[entryid][i] = func_str[entryid][i+1];
		if (funcid<m_functionlist->GetItemCount())
			m_functionlist->SetItemState(funcid, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	} else if (id==wxID_SET) {
		ScriptEditPropertiesWindow dial(this);
		for (i=0;i<script.entry_amount;i++)
			dial.m_entryctrl->Append(entry_name[i]);
		dial.m_entryctrl->SetSelection(entryid);
		dial.m_entryctrl->Enable(false);
		dial.m_entrytypectrl->SetValue(script.entry_type[entryid]);
		dial.m_typectrl->SetValue(script.function_type[entryid][funcid]);
		if (dial.ShowModal()==wxID_OK) {
			script.entry_type[entryid] = dial.m_entrytypectrl->GetValue();
			script.function_type[entryid][funcid] = dial.m_typectrl->GetValue();
		}
	}
}

void ScriptEditDialog::OnFunctionUpdate(wxCommandEvent& event) {
	unsigned int i,j,funclistpos = 0;
	for (i=0;i<entry_selection;i++)
		for (j=0;j<script.entry_function_amount[i];j++)
			funclistpos++;
	for (j=0;j<function_selection;j++)
		funclistpos++;
	m_functionlist->SetItemTextColour(funclistpos,FUNCTION_COLOR_MODIFIED);
	func_should_parse[funclistpos] = true;
}

void ScriptEditDialog::OnFunctionNewLine(wxCommandEvent& event) {
	long selfrom, selto;
	m_scripttext->GetSelection(&selfrom,&selto);
	if (selfrom!=selto)
		return;
	wxString indentstr = _(L"");
	long y, ip = m_scripttext->GetInsertionPoint();
	if (ip>1 && m_scripttext->GetRange(ip-2,ip-1).IsSameAs(L"{"))
		indentstr += TAB_STR;
	if (m_scripttext->PositionToXY(m_scripttext->GetInsertionPoint(),NULL,&y) && y>0) {
		wxString scriptline = m_scripttext->GetLineText(y-1);
		indentstr += scriptline.Mid(0,scriptline.find_first_not_of(L' '));
	}
	if (indentstr.Length()>0)
		m_scripttext->WriteText(indentstr);
}

void ScriptEditDialog::OnChoiceSelection(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_FIELD) {
		if (event.GetSelection()>0)
			UpdateLineHelp(text_x_selection,line_selection);
		else
			gl_window->DisplayFieldPlane(0,0);
	} else if (id==wxID_WORLD) {
		if (event.GetSelection()>0)
			UpdateLineHelp(text_x_selection,line_selection);
		else {
			world_pos_type = 0;
			wxClientDC dc(m_worlddisplaypanel);
			WorldMapDraw(dc,world_pos_type & 1,world_pos_type & 2,world_pos_x,world_pos_y);
		}
	}
}

void ScriptEditDialog::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_FBACK) {
		gl_window->field_showtiles = event.IsChecked();
		gl_window->Draw();
	} else if (id==wxID_FWALK) {
		gl_window->field_showwalk = event.IsChecked();
		gl_window->Draw();
	}
}

void ScriptEditDialog::OnWorldMapPaint(wxPaintEvent& event) {
	wxPaintDC dc(m_worlddisplaypanel);
	WorldMapDraw(dc,world_pos_type & 1,world_pos_type & 2,world_pos_x,world_pos_y);
}

void ScriptEditDialog::OnShowHideLocalVar(wxMouseEvent& event) {
	if (m_localvartext->IsShown()) {
		m_localvartext->Hide();
		wxImage tmpimg = wxBITMAP(bulletright_image).ConvertToImage();
		m_localvarshowimg->SetBitmap(wxBitmap(tmpimg));
	} else {
		m_localvartext->Show();
		wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
		m_localvarshowimg->SetBitmap(tmpimg);
	}
	m_scripttext->Layout();
	Layout();
	Refresh();
}

void ScriptEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i,j,funclistpos = 0;
	if (id==wxID_PARSE) {
		LogStruct log = ParseFunction(m_scripttext->GetValue(),entry_selection,function_selection);
		unsigned int entrynewsize = script.entry_size[entry_selection]-script.func[entry_selection][function_selection].length+parseoplength;
		if (entrynewsize%4)
			entrynewsize += 4-entrynewsize%4;
		int sizegap = extra_size+script.entry_size[entry_selection]-entrynewsize;
		if (sizegap<0) {
			wxString errstr;
			errstr.Printf(wxT(HADES_STRING_LOGERROR_SPACE),-sizegap);
			log.AddError(errstr.ToStdWstring());
		}
		LogDialog dial(this,log);
		dial.ShowModal();
		if (log.ok) {
			extra_size = sizegap;
			script.entry_size[entry_selection] = entrynewsize;
			script.func[entry_selection][function_selection].length = parseoplength;
			script.func[entry_selection][function_selection].op_amount = parseopamount;
			script.func[entry_selection][function_selection].op = new ScriptOperation[parseopamount];
			memcpy(script.func[entry_selection][function_selection].op,parseop,parseopamount*sizeof(ScriptOperation));
			for (i=0;i<entry_selection;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					funclistpos++;
			for (j=0;j<function_selection;j++)
				funclistpos++;
			m_functionlist->SetItemTextColour(funclistpos,FUNCTION_COLOR_PARSED);
			func_should_parse[funclistpos] = false;
		} else {
			unsigned int funclistpos = 0;
			for (i=0;i<entry_selection;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					funclistpos++;
			for (j=0;j<function_selection;j++)
				funclistpos++;
			m_functionlist->SetItemTextColour(funclistpos,FUNCTION_COLOR_FAILED);
			func_should_parse[funclistpos] = true;
		}
	} else if (id==wxID_HELP) {
		if (help_dial==NULL || !help_dial->IsShown()) {
			help_dial = new ScriptHelpDialog(this);
			help_dial->Show();
		} else {
			help_dial->SetFocus();
		}
	} else {
		if (id==wxID_OK) {
			bool shouldparse = false;
			for (i=0;i<script.entry_amount;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					if (func_should_parse[funclistpos++])
						shouldparse = true;
			if (shouldparse) {
				wxMessageDialog popup(this,_(HADES_STRING_SCRIPT_SHOULDPARSE),_(HADES_STRING_WARNING),wxYES_NO|wxCENTRE);
				if (popup.ShowModal()==wxID_YES)
					EndModal(id);
			} else
				EndModal(id);
		} else
			EndModal(id);
	}
}

void ScriptEditDialog::OnTimer(wxTimerEvent& event) {
	if (m_functionlist->GetSelectedItemCount()==0)
		m_functionlist->SetItemState(0,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	long x,y;
	if (!m_scripttext->PositionToXY(m_scripttext->GetInsertionPoint(),&x,&y))
		return;
	if (!wxGetMouseState().LeftIsDown())
		refresh_control_disable = false;
	if (x==text_x_selection && y==line_selection)
		return;
	wxString line = m_scripttext->GetLineText(y);
	bool refreshctrl = y!=line_selection;
	UpdateLineHelp(x,y);
	text_x_selection = x;
	line_selection = y;
	if (!refresh_control_disable)
		DisplayOperation(line,refreshctrl);
}

void ScriptEditDialog::OnArgFlag(wxCommandEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	ScriptChangeArg(argi,event.IsChecked() ? 1 : 0);
}

void ScriptEditDialog::OnArgSpin(wxSpinEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	ScriptChangeArg(argi,event.GetPosition());
}

void ScriptEditDialog::OnArgChoice(wxCommandEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	uint16_t* objid = (uint16_t*)event.GetClientData();
	if (objid)
		ScriptChangeArg(argi,*objid);
	else
		ScriptChangeArg(argi,event.GetInt());
}

void ScriptEditDialog::OnArgFlags(wxCommandEvent& event) {
	int argi = (event.GetId()-SS_ARG_ID)/SS_ARGBOX_MAXID;
	FlagsStruct* datagroup = static_cast<FlagsStruct*>(static_cast<wxCheckBox*>(event.GetEventObject())->GetClientData());
	int64_t value = 0;
	unsigned int i;
	for (i=0;i<datagroup->amount;i++)
		value |= (datagroup->box[i]->IsChecked() ? 1 : 0) << i;
	ScriptChangeArg(argi,value);
}

void ScriptEditDialog::OnArgField(wxCommandEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	DiscFieldStruct* datagroup = static_cast<DiscFieldStruct*>(static_cast<wxCheckBox*>(event.GetEventObject())->GetClientData());
	uint16_t* objid = (uint16_t*)datagroup->field->GetClientData(event.GetInt());
	if (objid)
		ScriptChangeArg(argi,(datagroup->disc->GetSelection() << 14) | *objid);
	else
		ScriptChangeArg(argi,(datagroup->disc->GetSelection() << 14) | event.GetInt());
}

void ScriptEditDialog::OnArgDisc(wxCommandEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	DiscFieldStruct* datagroup = static_cast<DiscFieldStruct*>(static_cast<wxCheckBox*>(event.GetEventObject())->GetClientData());
	uint16_t* objid = (uint16_t*)datagroup->field->GetClientData(datagroup->field->GetSelection());
	if (objid)
		ScriptChangeArg(argi,(event.GetInt() << 14) | *objid);
	else
		ScriptChangeArg(argi,(event.GetInt() << 14) | datagroup->field->GetSelection());
}

void ScriptEditDialog::OnArgPositionPaint(wxPaintEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	wxPaintDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc,posdata);
}

void ScriptEditDialog::OnArgPositionMouseMove(wxMouseEvent& event) {
	if (!event.LeftIsDown())
		return;
	refresh_control_disable = true;
	int argi = event.GetId()-SS_ARG_ID;
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	posdata->drawpt = wxPoint(event.GetPosition());
	ScriptChangeArg(argi,posdata->x+posdata->GetAsTranslation().x);
	if (posdata->use_z)
		ScriptChangeArg(argi,posdata->y+posdata->GetAsTranslation().y,2);
	else
		ScriptChangeArg(argi,posdata->y+posdata->GetAsTranslation().y,1);
	wxString line = m_scripttext->GetLineText(line_selection);
	DisplayOperation(line,false,false);
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc,posdata);
	event.Skip();
}

void ScriptEditDialog::OnArgPositionMouseUp(wxMouseEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	posdata->x += posdata->GetAsTranslation().x;
	posdata->y += posdata->GetAsTranslation().y;
	posdata->drawpt = wxPoint(POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE/2);
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc,posdata);
	event.Skip();
}

void ScriptEditDialog::OnArgPositionMouseWheel(wxMouseEvent& event) {
	if (event.LeftIsDown())
		return;
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	unsigned int maxzoom = script_type==SCRIPT_TYPE_WORLD ? 153600 : 2400;
	if (event.GetWheelRotation()>0 && posdata->zoom>75)
		posdata->zoom /= 2;
	else if (event.GetWheelRotation()<0 && posdata->zoom<maxzoom)
		posdata->zoom *= 2;
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc,posdata);
}

void ScriptEditDialog::OnArgPositionKeyboard(wxKeyEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	int argi = event.GetId()-SS_ARG_ID;
	int key = event.GetKeyCode();
	if (key==WXK_UP && posdata->use_z) {
		posdata->z -= posdata->zoom/6;
		ScriptChangeArg(argi,posdata->z,1);
		wxString line = m_scripttext->GetLineText(line_selection);
		DisplayOperation(line,false,false);
	} else if (key==WXK_DOWN && posdata->use_z) {
		posdata->z += posdata->zoom/6;
		ScriptChangeArg(argi,posdata->z,1);
		wxString line = m_scripttext->GetLineText(line_selection);
		DisplayOperation(line,false,false);
	} else
		event.Skip();
}

void ScriptEditDialog::OnArgColorPicker(wxColourPickerEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	if (arg_control_type[argi]==ARG_CONTROL_COLOR_RGB) {
		ScriptChangeArg(argi,event.GetColour().Red());
		ScriptChangeArg(argi,event.GetColour().Green(),1);
		ScriptChangeArg(argi,event.GetColour().Blue(),2);
	} else {
		ScriptChangeArg(argi,255-event.GetColour().Red());
		ScriptChangeArg(argi,255-event.GetColour().Green(),1);
		ScriptChangeArg(argi,255-event.GetColour().Blue(),2);
	}
}

ScriptEditEntryDialog::ScriptEditEntryDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype) :
	ScriptEditEntryWindow(parent),
	script_type(scpttype) {
	unsigned int i;
	base_entry_amount = scpt.entry_amount;
	entry_amount = scpt.entry_amount;
	base_entry_id = new int[entry_amount];
	entry_type = new uint8_t[entry_amount];
	for (i=0;i<entry_amount;i++) {
		base_entry_id[i] = i;
		entry_type[i] = scpt.entry_type[i];
	}
	has_character_entry = entry_amount>SCRIPT_FIXED_ENTRY_AMOUNT;
	extra_size = scpt.GetExtraSize();
}

ScriptEditEntryDialog::~ScriptEditEntryDialog() {
	delete[] base_entry_id;
	delete[] entry_type;
}

int ScriptEditEntryDialog::ShowModal() {
	unsigned int i,normalentry = entry_amount;
	if (entry_amount>0)
		m_entrylist->Append(_(L"Main"));
	if (has_character_entry)
		normalentry -= SCRIPT_FIXED_ENTRY_AMOUNT;
	for (i=1;i<normalentry;i++) {
		if (entry_type[i]==0)
			m_entrylist->Append(_(L"Code"));
		else if (entry_type[i]==1)
			m_entrylist->Append(_(L"Region"));
		else if (entry_type[i]==2)
			m_entrylist->Append(_(L"Object"));
		else 
			m_entrylist->Append(_(L"Unknown"));
	}
	if (has_character_entry) {
		for (i=0;i<8;i++)
			m_entrylist->Append(HADES_STRING_CHARACTER_DEFAULT_NAME[i]);
		m_entrylist->Append(HADES_STRING_CHARACTER_DEFAULT_NAME[11]); // Beatrix
	}
	if (entry_amount>0) {
		m_entrylist->SetSelection(0);
		DisplayEntry(0);
	}
	return wxDialog::ShowModal();
}

void ScriptEditEntryDialog::ApplyModifications(ScriptDataStruct& scpt) {
	unsigned int i,j;
	int lostentryarg = 0;
	j = 0;
	for (i=0;i<entry_amount;i++) {
		if (j<base_entry_amount && base_entry_id[j]==i) {
			j++;
		} else {
			while (j<base_entry_amount && base_entry_id[j]<0) {
				scpt.RemoveEntry(i,&lostentryarg);
				j++;
			}
			if (j>=base_entry_amount || base_entry_id[j]!=i)
				scpt.AddEntry(i,entry_type[i]);
			else
				j++;
		}
	}
	while (j<base_entry_amount && base_entry_id[j]<0) {
		scpt.RemoveEntry(entry_amount,&lostentryarg);
		j++;
	}
	if (lostentryarg>0) {
		wxString warningstr;
		warningstr.Printf(wxT(HADES_STRING_ENTRY_ARG_LOST),lostentryarg);
		wxMessageDialog popup(NULL,warningstr,HADES_STRING_WARNING,wxOK|wxCENTRE);
		popup.ShowModal();
	}
}

void ScriptEditEntryDialog::DisplayEntry(int sel) {
	m_entrytype->SetValue(entry_type[sel]);
	if (has_character_entry) {
		m_buttonadd->Enable(sel+SCRIPT_FIXED_ENTRY_AMOUNT<entry_amount);
		m_buttonremove->Enable(sel+SCRIPT_FIXED_ENTRY_AMOUNT<entry_amount && sel>0);
	} else {
		m_buttonremove->Enable(sel>0);
	}
}

void ScriptEditEntryDialog::OnEntrySelect(wxCommandEvent& event) {
	int sel = event.GetSelection();
	DisplayEntry(sel);
}

void ScriptEditEntryDialog::OnSpinCtrl(wxSpinEvent& event) {
	int sel = m_entrylist->GetSelection();
	if (sel==wxNOT_FOUND)
		return;
	entry_type[sel] = event.GetPosition();
	if (entry_type[sel]==0)
		m_entrylist->SetString(sel,_(L"Code"));
	else if (entry_type[sel]==1)
		m_entrylist->SetString(sel,_(L"Region"));
	else if (entry_type[sel]==2)
		m_entrylist->SetString(sel,_(L"Object"));
	else
		m_entrylist->SetString(sel,_(L"Unknown"));
}

void ScriptEditEntryDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	int sel = m_entrylist->GetSelection();
	unsigned int i;
	if (id==wxID_ADD) {
		if (extra_size<0x10) {
			wxMessageDialog popup(NULL,HADES_STRING_MISSING_SPACE,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popup.ShowModal();
			return;
		}
		sel++;
		uint8_t* newentrytype = new uint8_t[entry_amount+1];
		memcpy(newentrytype,entry_type,sel*sizeof(uint8_t));
		newentrytype[sel] = 0;
		memcpy(newentrytype+sel+1,entry_type+sel,(entry_amount-sel)*sizeof(uint8_t));
		delete[] entry_type;
		entry_type = newentrytype;
		for (i=0;i<base_entry_amount;i++)
			if (base_entry_id[i]>=sel)
				base_entry_id[i]++;
		entry_amount++;
		extra_size -= 0x10;
		m_entrylist->Insert(_(L"Code"),sel);
		m_entrylist->SetSelection(sel);
		DisplayEntry(sel);
	} else if (id==wxID_REMOVE) {
		if (sel==wxNOT_FOUND)
			return;
		entry_amount--;
		uint8_t* newentrytype = new uint8_t[entry_amount];
		memcpy(newentrytype,entry_type,sel*sizeof(uint8_t));
		memcpy(newentrytype+sel,entry_type+sel+1,(entry_amount-sel)*sizeof(uint8_t));
		delete[] entry_type;
		entry_type = newentrytype;
		for (i=0;i<base_entry_amount;i++)
			if (base_entry_id[i]==sel)
				base_entry_id[i] = -1;
			else if (base_entry_id[i]>sel)
				base_entry_id[i]--;
		extra_size += 0x10;
		m_entrylist->Delete(sel);
		if (sel==entry_amount)
			sel--;
		m_entrylist->SetSelection(sel);
		DisplayEntry(sel);
	} else
		EndModal(id);
}
