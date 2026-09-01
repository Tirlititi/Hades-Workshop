#include "File_Batching.h"

#include <wx/dir.h>
#include "Gui_Manipulation.h"
#include "Gui_LoadingDialog.h"
#include "Gui_TextEditor.h"
#include "Gui_ScriptEditor.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "Database_Resource.h"
#include "CommonUtility.h"
#include "main.h"

#define TXTBATCH_MAX_STRUCT 68

inline wxString FB_GetWxStringLine(wxString& str) {
	size_t pos = str.find_first_of(L"\r\n");
	if (pos == wxString::npos) {
		wxString strcpy = str;
		str = _(L"");
		return strcpy;
	}
	wxString res = str.Mid(0, pos);
	if (pos + 1 < str.Len() && str[pos] == L'\r' && str[pos + 1] == L'\n')
		pos++;
	str = str.Mid(pos + 1);
	return res;
}

inline wxString FB_GetNextWord(wxString& str) {
	size_t pos = str.find_first_of(L" \t\r\n");
	if (pos == wxString::npos) {
		wxString strcpy = str;
		str = _(L"");
		return strcpy;
	}
	wxString res = str.Mid(0, pos);
	while (pos < str.Len() && (str[pos] == L' ' || str[pos] == L'\t' || str[pos] == L'\r' || str[pos] == L'\n'))
		pos++;
	str = str.Mid(pos);
	return res;
}

//=============================//
//           Texts             //
//=============================//

// There are 3 different ways to index texts in these files:
// - Using "#HW newtext ID", the ID there is the local text index + 1
// - Using "#HW text ID", the ID there is the local text index
// - Using "#HW universal ID", the ID there is the universal text index (taking into account texts that don't appear in all languages)
// The last one is usually the best one
void WriteTextBlockWithUniversalID(wxFile& output, TextDataStruct* td, wstring blockname, unsigned int areaid, SteamLanguage singlelang) {
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	unsigned int textcount = max(td->base_amount, (unsigned int)td->text.size());
	output.Write(wxString::Format(wxT("#HW fileid %u // %s\n"), areaid, blockname));
	for (unsigned int i = 0; i < textcount; i++) {
		if (GetGameType() == GAME_TYPE_PSX) {
			output.Write(wxString::Format(wxT("#HW text %u\n"), i));
			output.Write(_(td->text[i].txt.str) + _(L"\n\n"));
		} else if (singlelang != STEAM_LANGUAGE_NONE) {
			int localindex = GetTextIdFromUniversalId(singlelang, areaid, i);
			if (localindex >= 0 && td->text[localindex].txt.multi_lang_init[singlelang]) {
				output.Write(wxString::Format(wxT("#HW universal %d\n"), i < td->base_amount ? i : td->text[localindex].id));
				output.Write(_(td->text[localindex].txt.multi_lang_str[singlelang]) + _(L"\n\n"));
			}
		} else {
			output.Write(wxString::Format(wxT("#HW universal %u\n"), i < td->base_amount ? i : td->text[i].id));
			for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (exportlang[lang]) {
					int localindex = GetTextIdFromUniversalId(lang, areaid, i);
					if (localindex >= 0 && td->text[localindex].txt.multi_lang_init[lang]) {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
						output.Write(_(td->text[localindex].txt.multi_lang_str[lang]) + _(L"\n\n"));
					}
				}
			}
		}
	}
}

void WriteTextSingleEntry(wxFile& output, FF9String& text, wxString declaration, SteamLanguage singlelang) {
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE && !text.multi_lang_init[singlelang])
		return;
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	output.Write(declaration);
	if (GetGameType() == GAME_TYPE_PSX) {
		output.Write(_(text.str) + _(L"\n\n"));
	} else if (singlelang != STEAM_LANGUAGE_NONE) {
		output.Write(_(text.multi_lang_str[singlelang]) + _(L"\n\n"));
	} else {
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (exportlang[lang] && text.multi_lang_init[lang]) {
				output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
				output.Write(_(text.multi_lang_str[lang]) + _(L"\n\n"));
			}
		}
	}
}

SteamLanguage GetSingleLanguageExport() {
	SteamLanguage singlelang = STEAM_LANGUAGE_NONE;
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		if (exportlang[lang]) {
			if (singlelang != STEAM_LANGUAGE_NONE)
				return STEAM_LANGUAGE_NONE;
			singlelang = lang;
		}
	}
	return singlelang;
}

int BatchExportDialog::ExportText(TextDataSet& data, wxString path, bool* exportlist, bool splitfile) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		if (!output.IsOpened())
			return 1;
		output.Write(_(L"#HW filetype TEXT\n\n"));
		if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
			output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	}
	for (unsigned int i = 0; i < data.amount; i++)
		if ((exportlist == NULL || exportlist[i]) && data.text_data[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				if (!output.IsOpened())
					return 1;
				output.Write(_(L"#HW filetype TEXT\n\n"));
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
			}
			WriteTextBlockWithUniversalID(output, data.text_data[i], data.name[i], data.struct_id[i], singlelang);
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportWorldTexts(WorldMapDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	if (GetGameType() == GAME_TYPE_PSX) {
		for (unsigned int i = 0; i < data.amount; i++) {
			if (data.text_data[i] != NULL && data.script[i] != NULL) {
				wxString name = _(L"Unknown World Map");
				for (unsigned int j = 0; j < HADES_STRING_WORLD_BLOCK_NAME.size(); j++) {
					if (HADES_STRING_WORLD_BLOCK_NAME[j].id == data.script[i]->object_id) {
						name = HADES_STRING_WORLD_BLOCK_NAME[j].label;
						break;
					}
				}
				WriteTextBlockWithUniversalID(output, data.text_data[i], name.ToStdWstring(), data.script[i]->object_id, singlelang);
			}
		}
	} else {
		WriteTextBlockWithUniversalID(output, data.text_data[0], L"World Map", STEAM_WORLD_MAP_TEXT_ID, singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportBattleTexts(EnemyDataSet& data, wxString path, bool* exportlist, bool splitfile) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	unsigned int i, j;
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		if (!output.IsOpened())
			return 1;
		output.Write(_(L"#HW filetype TEXT_BATTLE\n\n"));
		if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
			output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	}
	for (i = 0; i < data.battle_amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				if (!output.IsOpened())
					return 1;
				output.Write(_(L"#HW filetype TEXT_BATTLE\n\n"));
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
			}
			output.Write(wxString::Format(wxT("#HW fileid %u // %s\n"), data.battle_data[i]->object_id, data.battle_name[i]));
			TextDataStruct* td = data.text[i];
			for (j = 0; j < td->text.size(); j++)
				WriteTextSingleEntry(output, td->text[j].txt, wxString::Format(wxT("#HW text %u\n"), j), singlelang);
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportWorldPlaceNames(WorldMapDataStruct& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_INTERFACE\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	output.Write(_(L"#HW fileid 0\n\n"));
	for (unsigned int i = 0; i < WORLD_MAP_PLACE_AMOUNT; i++)
		WriteTextSingleEntry(output, data.place_name[i], wxString::Format(wxT("#HW text %u\n"), i), singlelang);
	output.Close();
	return 0;
}

int BatchExportDialog::ExportSpecialText(SpecialTextDataSet& data, wxString path, bool* exportlist, bool splitfile) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	unsigned int i, j;
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		if (!output.IsOpened())
			return 1;
		output.Write(_(L"#HW filetype TEXT_INTERFACE\n\n"));
		if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
			output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	}
	for (i = 0; i < data.text_block.size(); i++)
		if ((exportlist == NULL || exportlist[i]) && !data.text_block[i].is_localization) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				if (!output.IsOpened())
					return 1;
				output.Write(_(L"#HW filetype TEXT_INTERFACE\n\n"));
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), GetGameType() == GAME_TYPE_PSX ? i : i + 1));
			if (GetGameType() == GAME_TYPE_PSX)
				output.Write(_(L" // ") + HADES_STRING_SPECIAL_TEXT_BLOCK[i] + _(L"\n"));
			else
				output.Write(_(L" // ") + HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[i] + _(L"\n"));
			SpecialTextDataStruct& td = data.text_block[i];
			for (j = 0; j < td.text.size(); j++)
				WriteTextSingleEntry(output, td.text[j].txt, wxString::Format(wxT("#HW text %d\n"), td.text[j].id), singlelang);
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportCardNames(CardDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_INTERFACE\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	output.Write(_(L"#HW fileid 7 // Card Names\n\n"));
	for (unsigned int i = 0; i < data.card_amount; i++)
		WriteTextSingleEntry(output, data.card[i].name, wxString::Format(wxT("#HW text %d\n"), data.card[i].id), singlelang);
	output.Close();
	return 0;
}

int BatchExportDialog::ExportLocalizationEntries(SpecialTextDataStruct& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_LOCALIZATION\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.text.size(); i++)
		if (data.text[i].localization_field.length() > 0)
			WriteTextSingleEntry(output, data.text[i].txt, wxString::Format(wxT("#HW entry %s\n"), data.text[i].localization_field), singlelang);
	output.Close();
	return 0;
}

int BatchExportDialog::ExportSpellNames(SpellDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_ABILITY\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.spell.size(); i++) {
		WriteTextSingleEntry(output, data.spell[i].name, wxString::Format(wxT("#HW name %d\n"), data.spell[i].id), singlelang);
		WriteTextSingleEntry(output, data.spell[i].help, wxString::Format(wxT("#HW help %d\n"), data.spell[i].id), singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportSupportNames(SupportDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_SUPPORT\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.support.size(); i++) {
		WriteTextSingleEntry(output, data.support[i].name, wxString::Format(wxT("#HW name %d\n"), data.support[i].id), singlelang);
		WriteTextSingleEntry(output, data.support[i].help, wxString::Format(wxT("#HW help %d\n"), data.support[i].id), singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportCommandNames(CommandDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_COMMAND\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.cmd.size(); i++) {
		WriteTextSingleEntry(output, data.cmd[i].name, wxString::Format(wxT("#HW name %d\n"), data.cmd[i].id), singlelang);
		WriteTextSingleEntry(output, data.cmd[i].help, wxString::Format(wxT("#HW help %d\n"), data.cmd[i].id), singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportItemNames(ItemDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_ITEM\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.item.size(); i++) {
		WriteTextSingleEntry(output, data.item[i].name, wxString::Format(wxT("#HW name %d\n"), data.item[i].id), singlelang);
		WriteTextSingleEntry(output, data.item[i].help, wxString::Format(wxT("#HW help %d\n"), data.item[i].id), singlelang);
		WriteTextSingleEntry(output, data.item[i].battle_help, wxString::Format(wxT("#HW battlehelp %d\n"), data.item[i].id), singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportKeyItemNames(ItemDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_KEY_ITEM\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.key_item.size(); i++) {
		WriteTextSingleEntry(output, data.key_item[i].name, wxString::Format(wxT("#HW name %d\n"), data.key_item[i].id), singlelang);
		WriteTextSingleEntry(output, data.key_item[i].help, wxString::Format(wxT("#HW help %d\n"), data.key_item[i].id), singlelang);
		WriteTextSingleEntry(output, data.key_item[i].description, wxString::Format(wxT("#HW description %d\n"), data.key_item[i].id), singlelang);
	}
	output.Close();
	return 0;
}

int BatchExportDialog::ExportFieldNames(FieldDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_LOCATION\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.amount; i++)
		WriteTextSingleEntry(output, data.script_data[i]->name, wxString::Format(wxT("#HW fieldname %d\n"), data.script_data[i]->object_id), singlelang);
	output.Close();
	return 0;
}

int BatchExportDialog::ExportCharacterNames(StatDataSet& data, wxString path) {
	SteamLanguage singlelang = GetSingleLanguageExport();
	wxFile output;
	output.Open(path, wxFile::write);
	if (!output.IsOpened())
		return 1;
	output.Write(_(L"#HW filetype TEXT_STAT\n\n"));
	if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
		output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n\n"));
	for (unsigned int i = 0; i < data.initial_stat.size(); i++)
		WriteTextSingleEntry(output, data.initial_stat[i].default_name, wxString::Format(wxT("#HW name %d\n"), data.initial_stat[i].id), singlelang);
	output.Close();
	return 0;
}

struct ImportTextStruct {
	int fileid = -1;
	int textid = -1;
	int textkind = -1;
	SteamLanguage lang = GetSteamLanguage();
	wxString strkey = wxEmptyString;
	wxString str = wxEmptyString;
};

void CheckExtraSpaceForPSX(vector<ImportTextStruct>& importedtexts, unsigned int& i, LogStruct& log, TextDataStruct* td, wstring blocktype, int currentfileid) {
	unsigned int blockmaxsize = td->size + td->GetExtraSize();
	unsigned int blocksize = 4 + 8 * td->text.size();
	unsigned int j, k;
	for (j = 0; j < td->text.size(); j++) {
		blocksize += td->text[j].txt.length;
		for (k = 0; k < td->text[j].format_amount; k++)
			blocksize += td->text[j].format_data[k].length;
	}
	while (i < importedtexts.size() && importedtexts[i].fileid == currentfileid) {
		FF9String str;
		if (importedtexts[i].textid < (int)td->text.size())
			str = FF9String(td->text[importedtexts[i].textid].txt);
		else
			str.CreateEmpty();
		unsigned int opcount = CharCountInString(importedtexts[i].str, hades::SPECIAL_STRING_OPCODE_WCHAR);
		if (opcount != str.code_amount)
			log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_MISSMATCH_CODE), blocktype, currentfileid, importedtexts[i].textid, str.code_amount, opcount).ToStdWstring());
		str.SetValue(importedtexts[i].str.ToStdWstring());
		blocksize += str.length;
		if (importedtexts[i].textid < (int)td->text.size())
			blocksize -= td->text[importedtexts[i].textid].txt.length;
		else
			blocksize += 8;
		i++;
	}
	i--;
	if (blocksize > blockmaxsize)
		log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_SIZE), blocktype, currentfileid, blocksize - blockmaxsize).ToStdWstring());
}

FF9String CreateFF9StringToAdd(ImportTextStruct& txtdata, wstring nullstr) {
	FF9String str;
	str.CreateEmpty();
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		str.SetValue(nullstr, lang);
	str.SetValue(txtdata.str.ToStdWstring(), txtdata.lang);
	return str;
}

LogStruct BatchImportDialog::ImportText(SaveSet* dataset, set<int>& sectionmodified, wxString filetext, bool adjustsize, bool isjapan, bool fatalwarning) {
	wxString line, linebuf, hwword, token;
	vector<ImportTextStruct> result;
	set<int> universaliddone;
	ImportTextStruct current;
	unsigned long value;
	int textcommand;
	unsigned int i, linenum = 0;
	LogStruct log = LogStruct();
	bool legacycounter = false;
	bool legacyuitext = false;
	int filetypeid = -1;
	bool end = false;
	while (!filetext.IsEmpty() || current.textid >= 0 || current.strkey.Len() > 0) {
		linenum++;
		end = filetext.IsEmpty();
		line = FB_GetWxStringLine(filetext);
		linebuf = line;
		hwword = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")))
			token = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")) || end) {
			int currentfileid = current.fileid;
			int currenttextid = current.textid;
			int currenttextkind = current.textkind;
			SteamLanguage currentlang = current.lang;
			wxString currentstrkey = current.strkey;
			if ((currenttextid >= 0 || currentstrkey.Len() > 0) && currentlang != STEAM_LANGUAGE_NONE) {
				if (current.str.Right(2).IsSameAs(_(L"\n\n")))
					current.str = current.str.Mid(0, current.str.Len() - 2);
				if (filetypeid == 0 && currenttextkind == 2) {
					current.textid = GetTextIdFromUniversalId(currentlang, currentfileid, currenttextid);
					if (current.textid < 0)
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_INVALID_IN_LANGUAGE), L"Text Block", currentfileid, currenttextid, HADES_STRING_STEAM_LANGUAGE_LONG_NAME[current.lang]).ToStdWstring());
				}
				if (current.textid >= 0 || current.strkey.Len() > 0)
					result.push_back(current);
				current = ImportTextStruct();
				current.fileid = currentfileid;
				if (token.IsSameAs(_(L"language"))) {
					current.textkind = currenttextkind;
					current.textid = currenttextid;
					current.strkey = currentstrkey;
				} else {
					current.lang = currentlang;
				}
			} else if (end) {
				current.textid = -1;
			}
		}
		if (hwword.IsSameAs(_(L"#HW"))) {
			if (token.IsSameAs(_(L"filetype"))) {
				if (filetypeid < 0) {
					token = FB_GetNextWord(linebuf);
					if (token.IsSameAs(_(L"TEXT"))) {
						if (!dataset->sectionloaded[DATA_SECTION_TEXT] || !dataset->sectionloaded[DATA_SECTION_WORLD_MAP]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Texts and World Maps").ToStdWstring());
							return log;
						}
						filetypeid = 0;
						legacycounter = true;
					} else if (token.IsSameAs(_(L"TEXT_BATTLE"))) {
						if (!dataset->sectionloaded[DATA_SECTION_TEXT] || !dataset->sectionloaded[DATA_SECTION_ENMY]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Texts and Enemies").ToStdWstring());
							return log;
						}
						filetypeid = 1;
					} else if (token.IsSameAs(_(L"TEXT_ABILITY"))) {
						if (!dataset->sectionloaded[DATA_SECTION_SPELL]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Spells").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 0;
					} else if (token.IsSameAs(_(L"TEXT_SUPPORT"))) {
						if (!dataset->sectionloaded[DATA_SECTION_SUPPORT]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Supporting Abilities").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 1;
					} else if (token.IsSameAs(_(L"TEXT_COMMAND"))) {
						if (!dataset->sectionloaded[DATA_SECTION_CMD]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Commands").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 2;
					} else if (token.IsSameAs(_(L"TEXT_ITEM"))) {
						if (!dataset->sectionloaded[DATA_SECTION_ITEM]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Items").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 3;
					} else if (token.IsSameAs(_(L"TEXT_KEY_ITEM"))) {
						if (!dataset->sectionloaded[DATA_SECTION_ITEM]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Items").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 4;
					} else if (token.IsSameAs(_(L"TEXT_LOCATION"))) {
						if (!dataset->sectionloaded[DATA_SECTION_FIELD]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Fields").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 5;
					} else if (token.IsSameAs(_(L"TEXT_STAT"))) {
						if (!dataset->sectionloaded[DATA_SECTION_STAT]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Stats").ToStdWstring());
							return log;
						}
						filetypeid = 2;
						current.fileid = 6;
					} else if (token.IsSameAs(_(L"TEXT_INTERFACE"))) {
						if (!dataset->sectionloaded[DATA_SECTION_MENU_UI] || !dataset->sectionloaded[DATA_SECTION_WORLD_MAP] || !dataset->sectionloaded[DATA_SECTION_CARD]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"UI Texts, Cards and World Maps").ToStdWstring());
							return log;
						}
						filetypeid = 3;
					} else if (token.IsSameAs(_(L"UITEXT"))) {
						if (!dataset->sectionloaded[DATA_SECTION_MENU_UI]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"UI Texts").ToStdWstring());
							return log;
						}
						filetypeid = 3;
						legacyuitext = true;
					} else if (token.IsSameAs(_(L"TEXT_LOCALIZATION"))) {
						if (!dataset->sectionloaded[DATA_SECTION_MENU_UI]) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"UI Texts").ToStdWstring());
							return log;
						}
						filetypeid = 3;
						current.fileid = 100;
					} else {
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"TEXT, TEXT_INTERFACE, TEXT_LOCALIZATION, TEXT_BATTLE, TEXT_ABILITY, TEXT_ITEM, TEXT_KEY_ITEM, TEXT_LOCATION, TEXT_SUPPORT, TEXT_COMMAND, TEXT_STAT or UITEXT").ToStdWstring());
						return log;
					}
					if (GetGameType() == GAME_TYPE_PSX && filetypeid >= 2) {
						// TODO: add support for the PSX version (see TODO below)
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_PSX_NOT_SUPPORTED), token).ToStdWstring());
						return log;
					}
				} else {
					log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_FILETYPE_TWICE), linenum).ToStdWstring());
				}
			} else {
				if (filetypeid < 0) {
					log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_FILETYPE_NOTFIRST), linenum).ToStdWstring());
					return log;
				}
				if (token.IsSameAs(_(L"fileid"))) {
					current.fileid = -1;
					universaliddone.clear();
					if (filetypeid == 2 || (filetypeid == 3 && current.fileid == 100)) {
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
					} else if (!FB_GetNextWord(linebuf).ToULong(&value)) {
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, token).ToStdWstring());
					} else if (filetypeid == 3 && value > 7) {
						log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_WRONG_FILEID), linenum, 7).ToStdWstring());
					} else {
						if (legacyuitext) {
							if (value == 6)			value = 100;
							else if (value != 7)	value++;
						}
						current.fileid = value;
					}
				} else if (token.IsSameAs(_(L"language"))) {
					current.lang = STEAM_LANGUAGE_NONE;
					token = FB_GetNextWord(linebuf);
					for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token) || HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[i].IsSameAs(token)) {
							current.lang = i;
							break;
						}
					if (current.lang == STEAM_LANGUAGE_NONE)
						log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE), linenum, token).ToStdWstring());
				} else {
					textcommand = -1;
					if (token.IsSameAs(_(L"newtext")))
						textcommand = 0;
					else if (token.IsSameAs(_(L"text")))
						textcommand = 1;
					else if (token.IsSameAs(_(L"universal")))
						textcommand = 2;
					else if (token.IsSameAs(_(L"name")))
						textcommand = 3;
					else if (token.IsSameAs(_(L"help")))
						textcommand = 4;
					else if (token.IsSameAs(_(L"battlehelp")))
						textcommand = 5;
					else if (token.IsSameAs(_(L"description")))
						textcommand = 6;
					else if (token.IsSameAs(_(L"fieldname")))
						textcommand = 7;
					else if (token.IsSameAs(_(L"entry")))
						textcommand = 8;
					if (textcommand < 0) {
						log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_UNKNOWN), linenum, token).ToStdWstring());
						if ((current.textid >= 0 || current.strkey.Len() > 0) && current.lang != STEAM_LANGUAGE_NONE)
							current.str += line + _(L"\n");
					} else {
						current.textid = -1;
						current.strkey = wxEmptyString;
						if ((filetypeid == 0 || filetypeid == 1) && textcommand != 0 && textcommand != 1 && textcommand != 2) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 2 && current.fileid <= 2 && textcommand != 3 && textcommand != 4) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 2 && current.fileid == 3 && textcommand != 3 && textcommand != 4 && textcommand != 5) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 2 && current.fileid == 4 && textcommand != 3 && textcommand != 4 && textcommand != 6) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 2 && current.fileid == 5 && textcommand != 7) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 2 && current.fileid == 6 && textcommand != 3) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 3 && current.fileid != 100 && textcommand != 0 && textcommand != 1) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else if (filetypeid == 3 && current.fileid == 100 && textcommand != 0 && textcommand != 8) {
							log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, token).ToStdWstring());
						} else {
							if (textcommand == 8) {
								current.strkey = FB_GetNextWord(linebuf);
								for (i = 0; i < result.size(); i++) {
									if (result[i].fileid == current.fileid && result[i].strkey.IsSameAs(current.strkey)) {
										log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION_KEY), linenum, current.strkey).ToStdWstring());
										break;
									}
								}
							} else if (current.fileid < 0) {
								log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_MISSING_FILEID), linenum).ToStdWstring());
							} else if (!FB_GetNextWord(linebuf).ToULong(&value)) {
								log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, token).ToStdWstring());
							} else {
								if (legacycounter && (value == 0 || textcommand != 0))
									legacycounter = false;
								if (legacycounter)
									value--;
								current.textid = value;
								current.textkind = textcommand;
								if (textcommand == 2) {
									if (universaliddone.find(current.textid) != universaliddone.end())
										log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION), linenum, value).ToStdWstring());
									universaliddone.insert(current.textid);
								} else {
									for (i = 0; i < result.size(); i++) {
										if (result[i].fileid == current.fileid && result[i].textid == current.textid && result[i].textkind == current.textkind) {
											log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION), linenum, value).ToStdWstring());
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		} else if ((current.textid >= 0 || current.strkey.Len() > 0) && current.lang != STEAM_LANGUAGE_NONE) {
			current.str += line + _(L"\n");
		}
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		for (i = 0; i < result.size(); i++) {
			int currentfileid = result[i].fileid;
			if (filetypeid == 0 && currentfileid >= 9000) {
				int textblockindex = dataset->worldset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"World Map", currentfileid).ToStdWstring());
					continue;
				}
				CheckExtraSpaceForPSX(result, i, log, dataset->worldset->text_data[textblockindex], L"World Map", currentfileid);
			} else if (filetypeid == 0 && currentfileid < 9000) {
				int textblockindex = dataset->textset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"Text Block", currentfileid).ToStdWstring());
					continue;
				}
				CheckExtraSpaceForPSX(result, i, log, dataset->textset->text_data[textblockindex], L"Text Block", currentfileid);
			} else if (filetypeid == 1) {
				int textblockindex = dataset->enemyset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"Enemy", currentfileid).ToStdWstring());
					continue;
				}
				CheckExtraSpaceForPSX(result, i, log, dataset->enemyset->text[textblockindex], L"Enemy", currentfileid);
			} else {
				// TODO: support of these texts for the PSX version (see TODO above)
			}
		}
	}
	if (result.size() == 0)
		log.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOTHING)).ToStdWstring());
	if (fatalwarning && log.warning_amount > 0)
		log.ok = false;
	if (log.ok) {
		// Avoid adding warnings from there, to be consistent with "fatalwarning"
		CharmapDataStruct* chmap = GetGameType() == GAME_TYPE_PSX ? dataset->textset->charmap[dataset->textset->main_charmap_index] : NULL;
		CharmapDataStruct* chmapext = NULL;
		wstring nullstr = GetGameType() != GAME_TYPE_PSX && filetypeid < 2 ? L"[STRT=0,1]" : L"";
		for (i = 0; i < result.size(); i++) {
			int currentfileid = result[i].fileid;
			if (GetGameType() == GAME_TYPE_PSX && filetypeid == 0 && currentfileid >= 9000) {
				int textblockindex = dataset->worldset->GetIndexById(currentfileid);
				chmapext = dataset->worldset->charmap[textblockindex];
			} else if (GetGameType() == GAME_TYPE_PSX && filetypeid == 0 && currentfileid < 9000) {
				int textblockindex = dataset->textset->GetIndexById(currentfileid);
				chmapext = dataset->textset->charmap[textblockindex];
			}
			if (filetypeid == 0 && (currentfileid >= 9000 || currentfileid == STEAM_WORLD_MAP_TEXT_ID)) {
				int textblockindex = currentfileid == STEAM_WORLD_MAP_TEXT_ID ? 0 : dataset->worldset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					//log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"World Map", currentfileid).ToStdWstring());
					while (i < result.size() && result[i].fileid == currentfileid)
						i++;
					i--;
					continue;
				}
				if (adjustsize && GetGameType() != GAME_TYPE_PSX)
					TextSteamEditDialog::CalculateTextBestSize(GetTopWindow(), result[i].str);
				int textindex = dataset->worldset->text_data[textblockindex]->GetTextIndexById(result[i].textid);
				if (textindex >= 0)
					dataset->worldset->text_data[textblockindex]->SetText(textindex, result[i].str.ToStdWstring(), result[i].lang);
				else
					dataset->worldset->text_data[textblockindex]->AddText(result[i].textid, CreateFF9StringToAdd(result[i], nullstr));
				textindex = dataset->worldset->text_data[textblockindex]->GetTextIndexById(result[i].textid);
				if (adjustsize && GetGameType() == GAME_TYPE_PSX) {
					uint16_t sizex, sizey;
					chmap->CalculateTextSize(&dataset->worldset->text_data[textblockindex]->text[textindex].txt, chmapext, &sizex, &sizey);
					dataset->worldset->text_data[textblockindex]->text[textindex].SetDialogBoxSize(sizex, sizey, !isjapan);
				}
				sectionmodified.insert(DATA_SECTION_WORLD_MAP);
				dataset->sectionmodified[DATA_SECTION_WORLD_MAP] = true;
				dataset->worldset->text_data[textblockindex]->MarkDataModified();
			} else if (filetypeid == 0) {
				int textblockindex = dataset->textset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					//log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"Text Block", currentfileid).ToStdWstring());
					while (i < result.size() && result[i].fileid == currentfileid)
						i++;
					i--;
					continue;
				}
				if (adjustsize && GetGameType() != GAME_TYPE_PSX)
					TextSteamEditDialog::CalculateTextBestSize(GetTopWindow(), result[i].str);
				int textindex = dataset->textset->text_data[textblockindex]->GetTextIndexById(result[i].textid);
				if (textindex >= 0)
					dataset->textset->text_data[textblockindex]->SetText(textindex, result[i].str.ToStdWstring(), result[i].lang);
				else
					dataset->textset->text_data[textblockindex]->AddText(result[i].textid, CreateFF9StringToAdd(result[i], nullstr));
				textindex = dataset->textset->text_data[textblockindex]->GetTextIndexById(result[i].textid);
				if (adjustsize && GetGameType() == GAME_TYPE_PSX) {
					uint16_t sizex, sizey;
					chmap->CalculateTextSize(&dataset->textset->text_data[textblockindex]->text[textindex].txt, chmapext, &sizex, &sizey);
					dataset->textset->text_data[textblockindex]->text[textindex].SetDialogBoxSize(sizex, sizey, !isjapan);
				}
				sectionmodified.insert(DATA_SECTION_TEXT);
				dataset->sectionmodified[DATA_SECTION_TEXT] = true;
				dataset->textset->text_data[textblockindex]->MarkDataModified();
			} else if (filetypeid == 1) {
				int textblockindex = dataset->enemyset->GetIndexById(currentfileid);
				if (textblockindex < 0) {
					//log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_UNUSED), L"Enemy", currentfileid).ToStdWstring());
					while (i < result.size() && result[i].fileid == currentfileid)
						i++;
					i--;
					continue;
				}
				if (adjustsize && GetGameType() != GAME_TYPE_PSX)
					TextSteamEditDialog::CalculateTextBestSize(GetTopWindow(), result[i].str);
				int textindex = dataset->enemyset->text[textblockindex]->GetTextIndexById(result[i].textid);
				if (textindex >= 0)
					dataset->enemyset->text[textblockindex]->SetText(textindex, result[i].str.ToStdWstring(), result[i].lang);
				else
					dataset->enemyset->text[textblockindex]->AddText(result[i].textid, CreateFF9StringToAdd(result[i], nullstr));
				textindex = dataset->enemyset->text[textblockindex]->GetTextIndexById(result[i].textid);
				if (adjustsize && GetGameType() == GAME_TYPE_PSX) {
					uint16_t sizex, sizey;
					chmap->CalculateTextSize(&dataset->enemyset->text[textblockindex]->text[textindex].txt, chmapext, &sizex, &sizey);
					dataset->enemyset->text[textblockindex]->text[textindex].SetDialogBoxSize(sizex, sizey, !isjapan);
				}
				sectionmodified.insert(DATA_SECTION_ENMY);
				dataset->sectionmodified[DATA_SECTION_ENMY] = true;
				dataset->enemyset->text[textblockindex]->MarkDataModified();
			} else if (filetypeid == 2) {
				if (currentfileid == 0) {
					int textindex = dataset->spellset->GetSpellIndexById(result[i].textid);
					if (textindex >= 0 && result[i].textkind == 3)		dataset->spellset->spell[textindex].SetName(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 4)	dataset->spellset->spell[textindex].SetHelp(result[i].str.ToStdWstring(), result[i].lang);
					//else												log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Spell", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_SPELL);
					dataset->sectionmodified[DATA_SECTION_SPELL] = true;
				} else if (currentfileid == 1) {
					int textindex = dataset->supportset->GetSupportIndexById(result[i].textid);
					if (textindex >= 0 && result[i].textkind == 3)		dataset->supportset->support[textindex].SetName(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 4)	dataset->supportset->support[textindex].SetHelp(result[i].str.ToStdWstring(), result[i].lang);
					//else												log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Support", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_SUPPORT);
					dataset->sectionmodified[DATA_SECTION_SUPPORT] = true;
				} else if (currentfileid == 2) {
					int textindex = dataset->cmdset->GetCommandIndexById(result[i].textid);
					if (textindex >= 0 && result[i].textkind == 3)		dataset->cmdset->cmd[textindex].SetName(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 4)	dataset->cmdset->cmd[textindex].SetHelp(result[i].str.ToStdWstring(), result[i].lang);
					//else												log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Command", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_CMD);
					dataset->sectionmodified[DATA_SECTION_CMD] = true;
				} else if (currentfileid == 3) {
					int textindex = dataset->itemset->GetItemIndexById(result[i].textid);
					if (textindex >= 0 && result[i].textkind == 3)		dataset->itemset->item[textindex].SetName(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 4)	dataset->itemset->item[textindex].SetHelp(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 5)	dataset->itemset->item[textindex].SetBattleHelp(result[i].str.ToStdWstring(), result[i].lang);
					//else												log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Item", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_ITEM);
					dataset->sectionmodified[DATA_SECTION_ITEM] = true;
				} else if (currentfileid == 4) {
					int textindex = dataset->itemset->GetKeyItemIndexById(result[i].textid);
					if (textindex >= 0 && result[i].textkind == 3)		dataset->itemset->key_item[textindex].SetName(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 4)	dataset->itemset->key_item[textindex].SetHelp(result[i].str.ToStdWstring(), result[i].lang);
					else if (textindex >= 0 && result[i].textkind == 6)	dataset->itemset->key_item[textindex].SetDescription(result[i].str.ToStdWstring(), result[i].lang);
					//else												log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Key Item", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_ITEM);
					dataset->sectionmodified[DATA_SECTION_ITEM] = true;
				} else if (currentfileid == 5) {
					int textindex = dataset->fieldset->GetIndexById(result[i].textid);
					if (textindex >= 0)	dataset->fieldset->SetFieldName(textindex, result[i].str.ToStdWstring(), result[i].lang);
					//else				log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Field", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_FIELD);
					dataset->sectionmodified[DATA_SECTION_FIELD] = true;
					if (textindex >= 0)
						dataset->fieldset->script_data[textindex]->MarkDataModified();
				} else if (currentfileid == 6) {
					int textindex = dataset->statset->GetCharacterIndexById(result[i].textid);
					if (textindex >= 0)	dataset->statset->initial_stat[textindex].SetDefaultName(result[i].str.ToStdWstring(), result[i].lang);
					//else				log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Character", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_STAT);
					dataset->sectionmodified[DATA_SECTION_STAT] = true;
				}
			} else if (filetypeid == 3) {
				if (currentfileid == 0) {
					if (result[i].textid < WORLD_MAP_PLACE_AMOUNT)	dataset->worldset->world_data->SetName(result[i].textid, result[i].str.ToStdWstring(), result[i].lang);
					//else											log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"World Map Place", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_WORLD_MAP);
					dataset->sectionmodified[DATA_SECTION_WORLD_MAP] = true;
					dataset->worldset->world_data->MarkDataModified();
				} else if (currentfileid == 7) {
					if (result[i].textid < CARD_AMOUNT)	dataset->cardset->card[result[i].textid].SetName(result[i].str.ToStdWstring(), result[i].lang);
					//else								log.AddWarning(wxString::Format(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), L"Card", result[i].textid).ToStdWstring());
					sectionmodified.insert(DATA_SECTION_CARD);
					dataset->sectionmodified[DATA_SECTION_CARD] = true;
				} else if (currentfileid == 100) {
					int textindex = dataset->ffuiset->special_text->text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].GetTextIndexByKey(result[i].strkey.ToStdWstring());
					if (textindex >= 0) {
						dataset->ffuiset->special_text->text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].SetText(textindex, result[i].str.ToStdWstring(), result[i].lang);
					} else {
						FF9String newstr;
						newstr.CreateEmpty(true);
						newstr.SetValue(result[i].str.ToStdWstring(), result[i].lang);
						dataset->ffuiset->special_text->text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].AddTextByKey(result[i].strkey.ToStdWstring(), newstr);
					}
					sectionmodified.insert(DATA_SECTION_MENU_UI);
					dataset->sectionmodified[DATA_SECTION_MENU_UI] = true;
					dataset->ffuiset->special_text->modified = true;
				} else {
					int specialblock = currentfileid - 1;
					int textindex = dataset->ffuiset->special_text->text_block[specialblock].GetTextIndexById(result[i].textid);
					if (textindex >= 0) {
						dataset->ffuiset->special_text->text_block[specialblock].SetText(textindex, result[i].str.ToStdWstring(), result[i].lang);
					} else {
						FF9String newstr;
						newstr.CreateEmpty(true);
						newstr.SetValue(result[i].str.ToStdWstring(), result[i].lang);
						dataset->ffuiset->special_text->text_block[specialblock].AddText(result[i].textid, newstr);
					}
					sectionmodified.insert(DATA_SECTION_MENU_UI);
					dataset->sectionmodified[DATA_SECTION_MENU_UI] = true;
					dataset->ffuiset->special_text->modified = true;
				}
			}
		}
	}
	return log;
}

//=============================//
//          Scripts            //
//=============================//

int BatchExportDialog::ExportEnemyScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, bool appendmode, int addedinfo) {
	EnemyDataSet& data = *dataset->enemyset;
	wxString line, tmprest, localstr;
	unsigned int i, j, k;
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		output.Write(_(L"#HW filetype ENEMYSCRIPT\n\n"));
	}
	LoadingDialogInit(data.battle_amount, _(L"Exporting AI scripts..."));
	for (i = 0; i < data.battle_amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				output.Write(_(L"#HW filetype ENEMYSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), data.battle_data[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				output.Write(_(L" // ") + _(data.battle_name[i]));
			output.Write(_(L"\n"));
			ScriptEditHandler scpthand(*data.script[i], SCRIPT_TYPE_BATTLE, dataset, data.battle[i], data.text[i]);
			for (j = 1; j < scpthand.script.entry_amount; j++)
				if (j <= data.battle[i]->stat_amount) {
					scpthand.entry_name[j] = _(data.battle[i]->stat[j - 1].name.GetStr(2));
					scpthand.entry_name[j].Replace(_(L" "), _(L"_"));
				}
			scpthand.GenerateFunctionList();
			scpthand.GenerateEntryNames();
			scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
			if (scpthand.script.append_mode != 0)
				output.Write(_(L"#HW option appendmode\n"));
			if (scpthand.script.global_data.amount > 0) {
				output.Write(_(L"#HW globals\n"));
				output.Write(scpthand.globalvar_str);
				output.Write(_(L"#HW endglobals\n\n"));
			}
			for (j = 0; j < scpthand.script.entry_amount; j++) {
				if (appendmode && scpthand.script.append_mode != 0 && (scpthand.script.entry[j].append_mode & 1) == 0)
					continue;
				if (scpthand.script.entry[j].function_amount > 0) {
					if (scpthand.script.entry[j].player_link < 0)
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
					else
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("PC%d"), scpthand.script.entry[j].player_link) + _(L"\n"));
					if ((scpthand.script.entry[j].append_mode & 1) != 0)
						output.Write(_(L"#HW option appendentry\n"));
					if ((scpthand.script.entry[j].append_mode & 2) != 0)
						output.Write(_(L"#HW option autoinit\n"));
					if (scpthand.script.local_data[j].allocate_amount > 0 || scpthand.script.local_data[j].amount > 0) {
						localstr = scpthand.localvar_str[j];
						output.Write(_(L"#HW locals\n"));
						while (localstr.Len() > 0) {
							line = localstr.BeforeFirst(L'\n', &tmprest);
							localstr = tmprest;
							if (!line.Mid(0, 7).IsSameAs(L"global "))
								output.Write(line + _(L"\n"));
						}
						output.Write(_(L"#HW endlocals\n\n"));
					}
					for (k = 0; k < scpthand.script.entry[j].function_amount; k++) {
						output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.entry[j].func[k].function_type) + _(L"\n"));
						output.Write(scpthand.func_str[j][k] + _(L"\n\n"));
					}
				}
			}
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i + 1);
		}
	LoadingDialogEnd();
	return 0;
}

int BatchExportDialog::ExportWorldScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, bool appendmode, int addedinfo) {
	WorldMapDataSet& data = *dataset->worldset;
	wxString line, tmprest, localstr;
	unsigned int i, j, k;
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		output.Write(_(L"#HW filetype WORLDSCRIPT\n\n"));
	}
	for (i = 0; i < data.amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				output.Write(_(L"#HW filetype WORLDSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), data.script[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				for (j = 0; j < G_V_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME); j++)
					if (HADES_STRING_WORLD_BLOCK_NAME[j].id == data.script[i]->object_id) {
						output.Write(_(L" // ") + HADES_STRING_WORLD_BLOCK_NAME[j].label);
						break;
					}
			output.Write(_(L"\n"));
			ScriptEditHandler scpthand(*data.script[i], SCRIPT_TYPE_WORLD, dataset, NULL, data.text_data[i]);
			scpthand.GenerateFunctionList();
			scpthand.GenerateEntryNames();
			scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
			if (scpthand.script.append_mode != 0)
				output.Write(_(L"#HW option appendmode\n"));
			if (scpthand.script.global_data.amount > 0) {
				output.Write(_(L"#HW globals\n"));
				output.Write(scpthand.globalvar_str);
				output.Write(_(L"#HW endglobals\n\n"));
			}
			for (j = 0; j < scpthand.script.entry_amount; j++) {
				if (appendmode && scpthand.script.append_mode != 0 && (scpthand.script.entry[j].append_mode & 1) == 0)
					continue;
				if (scpthand.script.entry[j].function_amount > 0) {
					if (scpthand.script.entry[j].player_link < 0)
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
					else
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("PC%d"), scpthand.script.entry[j].player_link) + _(L"\n"));
					if ((scpthand.script.entry[j].append_mode & 1) != 0)
						output.Write(_(L"#HW option appendentry\n"));
					if ((scpthand.script.entry[j].append_mode & 2) != 0)
						output.Write(_(L"#HW option autoinit\n"));
					if (scpthand.script.local_data[j].allocate_amount > 0 || scpthand.script.local_data[j].amount > 0) {
						localstr = scpthand.localvar_str[j];
						output.Write(_(L"#HW locals\n"));
						while (localstr.Len() > 0) {
							line = localstr.BeforeFirst(L'\n', &tmprest);
							localstr = tmprest;
							if (!line.Mid(0, 7).IsSameAs(L"global "))
								output.Write(line + _(L"\n"));
						}
						output.Write(_(L"#HW endlocals\n\n"));
					}
					for (k = 0; k < scpthand.script.entry[j].function_amount; k++) {
						output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.entry[j].func[k].function_type) + _(L"\n"));
						output.Write(_(scpthand.func_str[j][k]) + _(L"\n\n"));
					}
				}
			}
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportFieldScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, bool appendmode, int addedinfo) {
	FieldDataSet& data = *dataset->fieldset;
	wxString line, tmprest, localstr;
	unsigned int i, j, k;
	set<uint16_t> functypecheck;
	wxFile output;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		output.Write(_(L"#HW filetype FIELDSCRIPT\n\n"));
	}
	LoadingDialogInit(data.amount, _(L"Exporting field scripts..."));
	for (i = 0; i < data.amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				output.Write(_(L"#HW filetype FIELDSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), data.script_data[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				output.Write(_(L" // ") + _(data.script_data[i]->name.str_nice));
			output.Write(_(L"\n"));
			ScriptEditHandler scpthand(*data.script_data[i], SCRIPT_TYPE_FIELD, dataset, NULL, data.related_text[i]);
			scpthand.GenerateFunctionList();
			scpthand.GenerateEntryNames();
			scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
			if (scpthand.script.append_mode != 0)
				output.Write(_(L"#HW option appendmode\n"));
			if (scpthand.script.global_data.amount > 0) {
				output.Write(_(L"#HW globals\n"));
				output.Write(scpthand.globalvar_str);
				output.Write(_(L"#HW endglobals\n\n"));
			}
			for (j = 0; j < scpthand.script.entry_amount; j++) {
				if (appendmode && scpthand.script.append_mode != 0 && (scpthand.script.entry[j].append_mode & 1) == 0)
					continue;
				if (scpthand.script.entry[j].function_amount > 0) {
					if (scpthand.script.entry[j].player_link < 0)
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
					else
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("PC%d"), scpthand.script.entry[j].player_link) + _(L"\n"));
					if ((scpthand.script.entry[j].append_mode & 1) != 0)
						output.Write(_(L"#HW option appendentry\n"));
					if ((scpthand.script.entry[j].append_mode & 2) != 0)
						output.Write(_(L"#HW option autoinit\n"));
					if (scpthand.script.local_data[j].allocate_amount > 0 || scpthand.script.local_data[j].amount > 0) {
						localstr = scpthand.localvar_str[j];
						output.Write(_(L"#HW locals\n"));
						while (localstr.Len() > 0) {
							line = localstr.BeforeFirst(L'\n', &tmprest);
							localstr = tmprest;
							if (!line.Mid(0, 7).IsSameAs(L"global "))
								output.Write(line + _(L"\n"));
						}
						output.Write(_(L"#HW endlocals\n\n"));
					}
					functypecheck.clear();
					for (k = 0; k < scpthand.script.entry[j].function_amount; k++) {
						if (functypecheck.count(scpthand.script.entry[j].func[k].function_type) > 0) {
							output.Write(wxString::Format(wxT("// Duplicated function %u is discarded\n\n"), scpthand.script.entry[j].func[k].function_type));
						} else {
							output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.entry[j].func[k].function_type) + _(L"\n"));
							output.Write(_(scpthand.func_str[j][k]) + _(L"\n\n"));
							functypecheck.insert(scpthand.script.entry[j].func[k].function_type);
						}
					}
				}
			}
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i + 1);
		}
	LoadingDialogEnd();
	return 0;
}

LogStruct BatchImportDialog::ImportScript(SaveSet* dataset, set<int>& sectionmodified, int scripttype, wxString filescript, bool fatalwarning, bool* datamodif, vector<ScriptDataStruct*>* modifiedscripts) {
	wxString line, linebuf, errstr, token, codevalue, globalcode, localcode, inputstr = filescript;
	int filescripttype, currentscriptindex = -1, currententry = -1, currentfunction = -1;
	unsigned int i, codelinenum, linenum = 0;
	bool updatecur = false, langlinkblock = false, localblock = false, globalblock = false;
	bool end, tokencheck, filetypeok = false, nothingdone = true;
	vector<uint16_t> langtextid, baselangtextid;
	uint16_t tmptextid, tmpbasetextid;
	long entrysizegap;
	unsigned long value;
	LogStruct tmplog, res = LogStruct();
	ScriptEditHandler* current_handler = NULL;
	ScriptDataStruct* current_script_ptr = NULL;
	wstring current_script_name;
	while (!inputstr.IsEmpty() || currentfunction >= 0) {
		linenum++;
		end = inputstr.IsEmpty();
		line = FB_GetWxStringLine(inputstr);
		linebuf = line;
		token = FB_GetNextWord(linebuf);
		if (token.IsSameAs(_(L"#HW")) || end) {
			if (currentfunction >= 0) {
				current_handler->currentvar_str = globalcode + _(L"\n") + localcode;
				tmplog = current_handler->ParseFunction(codevalue, currententry, currentfunction, codelinenum);
				entrysizegap = current_script_ptr->GetExtraSize() + current_handler->script.entry[currententry].size - current_handler->GetParsedEntryNewSize();
				if (entrysizegap < 0) {
					errstr.Printf(wxT(HADES_STRING_LOGERROR_SPACE), -entrysizegap);
					tmplog.AddError(errstr.ToStdWstring());
				}
				if (tmplog.error_amount > 0) {
					errstr.Printf(wxT(HADES_STRING_BATCH_SCRIPT_FUNCTION), current_handler->functionlist_str[current_handler->GetFunctionAbsolutePos(currententry, currentfunction)], current_script_name);
					res.AddError(errstr.ToStdWstring());
				}
				if (tmplog.warning_amount > 0) {
					errstr.Printf(wxT(HADES_STRING_BATCH_SCRIPT_FUNCTION), current_handler->functionlist_str[current_handler->GetFunctionAbsolutePos(currententry, currentfunction)], current_script_name);
					res.AddWarning(errstr.ToStdWstring());
				}
				res.AddLog(tmplog);
				updatecur = updatecur && tmplog.error_amount == 0 && (!fatalwarning || tmplog.warning_amount == 0);
				current_handler->ApplyParsedFunction();
				currentfunction = -1;
			}
		}
		if (token.IsSameAs(_(L"#HW"))) {
			token = FB_GetNextWord(linebuf);
			if (token.IsSameAs(_(L"filetype"))) {
				if (!filetypeok) {
					token = FB_GetNextWord(linebuf);
					filescripttype = SCRIPT_TYPE_ANY;
					if (token.IsSameAs(_(L"FIELDSCRIPT")))
						filescripttype = SCRIPT_TYPE_FIELD;
					else if (token.IsSameAs(_(L"ENEMYSCRIPT")))
						filescripttype = SCRIPT_TYPE_BATTLE;
					else if (token.IsSameAs(_(L"WORLDSCRIPT")))
						filescripttype = SCRIPT_TYPE_WORLD;
					if (filescripttype == SCRIPT_TYPE_ANY) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_FILETYPE), linenum);
						res.AddError(errstr.ToStdWstring());
						errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					if (scripttype != SCRIPT_TYPE_ANY) {
						if (scripttype == SCRIPT_TYPE_FIELD && filescripttype != SCRIPT_TYPE_FIELD) {
							errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"FIELDSCRIPT");
							res.AddError(errstr.ToStdWstring());
							errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
							res.AddError(errstr.ToStdWstring());
							return res;
						} else if (scripttype == SCRIPT_TYPE_BATTLE && filescripttype != SCRIPT_TYPE_BATTLE) {
							errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"ENEMYSCRIPT");
							res.AddError(errstr.ToStdWstring());
							errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
							res.AddError(errstr.ToStdWstring());
							return res;
						} else if (scripttype == SCRIPT_TYPE_WORLD && filescripttype != SCRIPT_TYPE_WORLD) {
							errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"WORLDSCRIPT");
							res.AddError(errstr.ToStdWstring());
							errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
							res.AddError(errstr.ToStdWstring());
							return res;
						}
					}
					if (filescripttype == SCRIPT_TYPE_FIELD && !dataset->sectionloaded[DATA_SECTION_FIELD]) {
						res.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Fields").ToStdWstring());
						return res;
					} else if (filescripttype == SCRIPT_TYPE_BATTLE && !dataset->sectionloaded[DATA_SECTION_ENMY]) {
						res.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"Enemies").ToStdWstring());
						return res;
					} else if (filescripttype == SCRIPT_TYPE_WORLD && !dataset->sectionloaded[DATA_SECTION_WORLD_MAP]) {
						res.AddError(wxString::Format(wxT(HADES_STRING_BATCH_NOT_LOADED), L"World Maps").ToStdWstring());
						return res;
					}
					filetypeok = true;
				} else {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_TWICE), linenum);
					res.AddWarning(errstr.ToStdWstring());
				}
			} else {
				if (!filetypeok) {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_NOTFIRST), linenum);
					res.AddError(errstr.ToStdWstring());
					errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
					res.AddError(errstr.ToStdWstring());
					return res;
				}
				if (token.IsSameAs(_(L"endglobals")) && !globalblock) {
					errstr.Printf(wxT(HADES_STRING_BATCH_SCRIPT_NOGLOBALS), linenum);
					res.AddWarning(errstr.ToStdWstring());
				} else if (token.IsSameAs(_(L"endlocals")) && !localblock) {
					errstr.Printf(wxT(HADES_STRING_BATCH_SCRIPT_NOLOCALS), linenum);
					res.AddWarning(errstr.ToStdWstring());
				}
				langlinkblock = globalblock = localblock = false;
				if (token.IsSameAs(_(L"fileid"))) {
					if (updatecur) {
						*current_script_ptr = current_handler->script;
						current_script_ptr->GuaranteePlayerLinks();
						current_script_ptr->MarkDataModified();
						if (modifiedscripts != NULL)
							modifiedscripts->push_back(current_script_ptr);
						updatecur = false;
						nothingdone = false;
					}
					globalcode = localcode = _(L"");
					if (current_handler != NULL) {
						delete current_handler;
						current_handler = NULL;
					}
					token = FB_GetNextWord(linebuf);
					currentscriptindex = -1;
					currententry = -1;
					currentfunction = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						if (filescripttype == SCRIPT_TYPE_FIELD) {
							for (i = 0; i < dataset->fieldset->amount; i++)
								if (dataset->fieldset->script_data[i]->object_id == value) {
									currentscriptindex = i;
									current_script_ptr = dataset->fieldset->script_data[i];
									current_handler = new ScriptEditHandler(*current_script_ptr, filescripttype, dataset, NULL, NULL);
									current_handler->GenerateFunctionList();
									current_script_name = dataset->fieldset->script_data[i]->name.GetStr(2);
									updatecur = true;
									break;
								}
							if (i == dataset->fieldset->amount) {
								errstr.Printf(wxT(HADES_STRING_BATCH_FIELD_UNUSED), linenum, value);
								res.AddWarning(errstr.ToStdWstring());
							}
						} else if (filescripttype == SCRIPT_TYPE_BATTLE) {
							for (i = 0; i < dataset->enemyset->battle_amount; i++)
								if (dataset->enemyset->battle_data[i]->object_id == value) {
									currentscriptindex = i;
									current_script_ptr = dataset->enemyset->script[i];
									current_handler = new ScriptEditHandler(*current_script_ptr, filescripttype, dataset, NULL, NULL);
									current_handler->GenerateFunctionList();
									current_script_name = dataset->enemyset->battle_name[i];
									updatecur = true;
									break;
								}
							if (i == dataset->enemyset->battle_amount) {
								errstr.Printf(wxT(HADES_STRING_BATCH_BATTLE_UNUSED), linenum, value);
								res.AddWarning(errstr.ToStdWstring());
							}
						} else {
							for (i = 0; i < dataset->worldset->amount; i++)
								if (dataset->worldset->script[i]->object_id == value) {
									currentscriptindex = i;
									current_script_ptr = dataset->worldset->script[i];
									current_handler = new ScriptEditHandler(*current_script_ptr, filescripttype, dataset, NULL, NULL);
									current_handler->GenerateFunctionList();
									current_script_name = dataset->worldset->name[i];
									updatecur = true;
									break;
								}
							if (i == dataset->worldset->amount) {
								errstr.Printf(wxT(HADES_STRING_BATCH_WORLD_UNUSED), linenum, value);
								res.AddWarning(errstr.ToStdWstring());
							}
						}
					}
				} else if (token.IsSameAs(_(L"newentry"))) {
					localcode = _(L"");
					if (current_handler != NULL) {
						token = FB_GetNextWord(linebuf);
						if (token.StartsWith(_(L"PC")) && token.Mid(2).ToULong(&value)) {
							unsigned long entrytype;
							token = FB_GetNextWord(linebuf);
							if (!token.ToULong(&entrytype))
								entrytype = 2;
							currententry = -1;
							for (i = 0; i < current_handler->script.entry_amount; i++) {
								if (current_handler->script.entry[i].player_link == value) {
									current_handler->script.entry[i].type = entrytype;
									currententry = i;
									break;
								}
							}
							if (currententry < 0) {
								currententry = current_handler->script.entry_amount;
								current_handler->AddEntry(currententry, entrytype, value);
							}
						} else if (token.ToULong(&value)) {
							currententry = value;
							token = FB_GetNextWord(linebuf);
							tokencheck = token.ToULong(&value);
							if (currententry >= current_handler->script.entry_amount)
								current_handler->AddEntry(currententry, tokencheck ? value : 0, -1);
							else if (tokencheck)
								current_handler->script.entry[currententry].type = value;
							currentfunction = -1;
						} else {
							errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newentry");
							res.AddError(errstr.ToStdWstring());
							currententry = -1;
							currentfunction = -1;
						}
					}
				} else if (token.IsSameAs(_(L"newfunction"))) {
					token = FB_GetNextWord(linebuf);
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newfunction");
						res.AddError(errstr.ToStdWstring());
						currentfunction = -1;
					} else if (currententry >= 0) {
						for (i = 0; i < current_handler->script.entry[currententry].function_amount; i++)
							if (current_handler->script.entry[currententry].func[i].function_type == value) {
								currentfunction = i;
								break;
							}
						if (i == current_handler->script.entry[currententry].function_amount) {
							for (i = 0; i < current_handler->script.entry[currententry].function_amount; i++)
								if (current_handler->script.entry[currententry].func[i].function_type > value) {
									if (i > 0)
										i--;
									break;
								}
							current_handler->AddFunction(currententry, i, value);
							currentfunction = i;
						}
						codevalue = _(L"");
						codelinenum = linenum;
					}
				} else if (token.IsSameAs(_(L"globals"))) {
					globalblock = true;
				} else if (token.IsSameAs(_(L"locals"))) {
					localblock = true;
				} else if (token.IsSameAs(_(L"option"))) {
					if (current_handler != NULL) {
						bool irrelevant = true;
						token = FB_GetNextWord(linebuf);
						if (token.IsSameAs(_(L"appendmode"))) {
							current_handler->script.append_mode = 1;
							irrelevant = false;
						} else if (token.IsSameAs(_(L"appendentry"))) {
							if (currententry >= 0) {
								current_handler->script.entry[currententry].append_mode |= 1;
								irrelevant = false;
							}
						} else if (token.IsSameAs(_(L"autoinit"))) {
							if (currententry >= 0) {
								current_handler->script.entry[currententry].append_mode |= 2;
								irrelevant = false;
							}
						} else {
							errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_OPTION), linenum, token);
							res.AddWarning(errstr.ToStdWstring());
							irrelevant = false;
						}
						if (irrelevant) {
							errstr.Printf(wxT(HADES_STRING_BATCH_COMMAND_IRRELEVANT), linenum, _(L"option ") + token);
							res.AddWarning(errstr.ToStdWstring());
						}
					}
				} else if (token.IsSameAs(_(L"languagetextlink"))) {
					errstr.Printf(wxT(HADES_STRING_BATCH_UNSUPPORT_ANYMORE), linenum, token);
					res.AddWarning(errstr.ToStdWstring());
				} else if (token.IsSameAs(_(L"language"))) {
					errstr.Printf(wxT(HADES_STRING_BATCH_UNSUPPORT_ANYMORE), linenum, token);
					res.AddWarning(errstr.ToStdWstring());
				} else if (token.IsSameAs(_(L"languagelink"))) {
					errstr.Printf(wxT(HADES_STRING_BATCH_UNSUPPORT_ANYMORE), linenum, token);
					res.AddWarning(errstr.ToStdWstring());
				}
			}
		} else {
			if (langlinkblock) {
				tokencheck = true;
				if (token.ToULong(&value))
					tmpbasetextid = value;
				else
					tokencheck = false;
				token = FB_GetNextWord(linebuf);
				if (token.ToULong(&value))
					tmptextid = value;
				else
					tokencheck = false;
				if (tokencheck) {
					baselangtextid.push_back(tmpbasetextid);
					langtextid.push_back(tmptextid);
				}
			} else if (globalblock)
				globalcode += line + _(L"\n");
			else if (localblock)
				localcode += line + _(L"\n");
			else if (currentfunction >= 0)
				codevalue += line + _(L"\n");
		}
	}
	if (updatecur) {
		if (filescripttype == SCRIPT_TYPE_FIELD)
			sectionmodified.insert(DATA_SECTION_FIELD);
		else if (filescripttype == SCRIPT_TYPE_BATTLE)
			sectionmodified.insert(DATA_SECTION_ENMY);
		else if (filescripttype == SCRIPT_TYPE_WORLD)
			sectionmodified.insert(DATA_SECTION_WORLD_MAP);
		*current_script_ptr = current_handler->script;
		current_script_ptr->GuaranteePlayerLinks();
		current_script_ptr->MarkDataModified();
		if (modifiedscripts != NULL)
			modifiedscripts->push_back(current_script_ptr);
		updatecur = false;
		nothingdone = false;
	}
	if (current_handler != NULL)
		delete current_handler;
	if (nothingdone) {
		errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
		res.AddError(errstr.ToStdWstring());
	} else if (datamodif != NULL) {
		*datamodif = true;
	}
	if (fatalwarning && res.warning_amount > 0)
		res.ok = false;
	return res;
}

//=============================//
//           Images            //
//=============================//

int BatchExportDialog::ExportImageBackground(FieldDataSet& data, wxString path, bool* exportlist, bool mergetile, bool depthorder, int steamtitlelang) {
	unsigned int i,j;
	bool mustflush;
	LoadingDialogInit(data.amount,_(L"Exporting field backgrounds..."));
	for (i=0;i<data.amount;i++)
		if (exportlist==NULL || exportlist[i]) {
			if (data.background_data[i] == NULL)
				continue;
			mustflush = false;
			if (GetGameType()!=GAME_TYPE_PSX && data.tim_data[i]!=NULL && !data.tim_data[i]->loaded) {
				fstream ftmp;
				data.tim_data[i]->Read(ftmp);
				mustflush = true;
			}
			if (data.background_data[i]->camera_amount==1)
				data.background_data[i]->Export((path+wxString::Format(wxT("_%u.tiff"),i+1)).mb_str(),0,NULL,true,mergetile,depthorder,steamtitlelang);
			else
				for (j=0;j<data.background_data[i]->camera_amount;j++)
					data.background_data[i]->Export((path+wxString::Format(wxT("_%u_%u.tiff"),i+1,j+1)).mb_str(),j,NULL,true,mergetile,depthorder,steamtitlelang);
			if (mustflush)
				data.tim_data[i]->Flush();
			LoadingDialogUpdate(i+1);
		}
	LoadingDialogEnd();
	return 0;
}

//===============================//
//           Walkmeshes          //
//===============================//

int BatchExportDialog::ExportWalkmesh(FieldDataSet& data, wxString path, bool* exportlist) {
	unsigned int i;
	LoadingDialogInit(data.amount, _(L"Exporting field walkmeshes..."));
	for (i = 0; i < data.amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (data.walkmesh[i] == NULL)
				continue;
			data.walkmesh[i]->ExportAsObj((path + wxString::Format(wxT("_%u.obj"), i + 1)).ToStdString(), data.script_data[i]->name, data.script_data[i]->object_id);
			LoadingDialogUpdate(i + 1);
		}
	LoadingDialogEnd();
	return 0;
}

//=============================//
//            GUI              //
//=============================//

// Export
BatchExportDialog::BatchExportDialog(wxWindow* p) :
	BatchExportWindow(p) {
	list_popup_menu = new wxMenu();
	list_popup_menu->Append(wxID_SELECTALL, HADES_STRING_GENERIC_SELECT_ALL);
	list_popup_menu->Append(wxID_CLEAR, HADES_STRING_GENERIC_DESELECT_ALL);
	list_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(BatchExportDialog::OnSelectRightClickMenu), NULL, this);
}

BatchExportDialog::~BatchExportDialog() {
	list_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(BatchExportDialog::OnSelectRightClickMenu), NULL, this);
}

int BatchExportDialog::ShowModal(int type, SaveSet* datas, wxArrayString objlist, unsigned int* objlistsort) {
	unsigned int i;
	datatype = type;
	sortlist = objlistsort;
	dataset = datas;
	m_exportlist->Append(objlist);
	for (i = 0; i < objlist.Count(); i++)
		m_exportlist->Check(i);
	if (type < 100) {
		m_splitfile->SetValue(false);
		m_splitfilepanel->Show(type == 1 || type == 3 || type == 5);
	} else if (type >= 100 && type < 200) {
		m_splitfilepanel->Show(false);
		m_scriptpanel->Show(true);
	} else if (type == 200) {
		m_splitfilepanel->Show(false);
		m_backgroundpanel->Show(true);
	} else if (type == 300) {
		m_splitfilepanel->Show(false);
	}
	return wxDialog::ShowModal();
}

void BatchExportDialog::OnFilePick(wxFileDirPickerEvent& event) {
	wxString fname = event.GetPath();
	if (datatype < 200) {
		wxCheckBox* splitbox = datatype >= 100 && datatype < 200 ? m_scriptsplitfile : m_splitfile;
		bool suffix = fname.Right(4).IsSameAs(_(L".txt"));
		if (splitbox->IsChecked() && suffix)
			m_filepicker->SetPath(fname.Mid(0, fname.Len() - 4));
		else if (!splitbox->IsChecked() && !suffix)
			m_filepicker->SetPath(fname + _(L".txt"));
	} else if (datatype < 300) {
		bool suffix = fname.Right(5).IsSameAs(_(L".tiff"));
		if (suffix)
			m_filepicker->SetPath(fname.Mid(0, fname.Len() - 5));
	} else if (datatype < 400) {
		bool suffix = fname.Right(4).IsSameAs(_(L".obj"));
		if (suffix)
			m_filepicker->SetPath(fname.Mid(0, fname.Len() - 4));
	}
}

void BatchExportDialog::OnSplitFileCheck(wxCommandEvent& event) {
	wxString fname = m_filepicker->GetPath();
	bool suffix = fname.Right(4).IsSameAs(_(L".txt"));
	if (event.IsChecked() && suffix)
		m_filepicker->SetPath(fname.Mid(0, fname.Len() - 4));
	else if (!event.IsChecked() && !suffix && fname.Len() > 0)
		m_filepicker->SetPath(fname + _(L".txt"));
}

void BatchExportDialog::OnListRightClick(wxMouseEvent& event) {
	int newsel = m_exportlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_exportlist->SetSelection(newsel);
		m_exportlist->PopupMenu(list_popup_menu);
	}
}

void BatchExportDialog::OnSelectRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_SELECTALL) {
		for (i = 0; i < m_exportlist->GetCount(); i++)
			m_exportlist->Check(i, true);
	} else if (id == wxID_CLEAR) {
		for (i = 0; i < m_exportlist->GetCount(); i++)
			m_exportlist->Check(i, false);
	}
}

void BatchExportDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i, j;
	if (id == wxID_OK && !m_filepicker->GetPath().IsEmpty() && !m_filepicker->GetPath().IsSameAs(_(L".txt"))) {
		bool* exportlist = new bool[m_exportlist->GetCount()];
		for (i = 0; i < m_exportlist->GetCount(); i++) {
			j = sortlist == NULL ? i : sortlist[i];
			if (m_exportlist->IsChecked(i))
				exportlist[j] = true;
			else
				exportlist[j] = false;
		}
		switch (datatype) {
		case 1:
			ExportText(*dataset->textset, m_filepicker->GetPath(), exportlist, m_splitfile->IsChecked());
			break;
		case 2:
			ExportWorldTexts(*dataset->worldset, m_filepicker->GetPath());
			break;
		case 3:
			ExportBattleTexts(*dataset->enemyset, m_filepicker->GetPath(), exportlist, m_splitfile->IsChecked());
			break;
		case 4:
			ExportWorldPlaceNames(*dataset->worldset->world_data, m_filepicker->GetPath());
			break;
		case 5:
			ExportSpecialText(*dataset->ffuiset->special_text, m_filepicker->GetPath(), exportlist, m_splitfile->IsChecked());
			break;
		case 6:
			ExportCardNames(*dataset->cardset, m_filepicker->GetPath());
			break;
		case 7:
			ExportLocalizationEntries(dataset->ffuiset->special_text->text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM], m_filepicker->GetPath());
			break;
		case 8:
			ExportSpellNames(*dataset->spellset, m_filepicker->GetPath());
			break;
		case 9:
			ExportSupportNames(*dataset->supportset, m_filepicker->GetPath());
			break;
		case 10:
			ExportCommandNames(*dataset->cmdset, m_filepicker->GetPath());
			break;
		case 11:
			ExportItemNames(*dataset->itemset, m_filepicker->GetPath());
			break;
		case 12:
			ExportKeyItemNames(*dataset->itemset, m_filepicker->GetPath());
			break;
		case 13:
			ExportFieldNames(*dataset->fieldset, m_filepicker->GetPath());
			break;
		case 14:
			ExportCharacterNames(*dataset->statset, m_filepicker->GetPath());
			break;
		case 100:
			ExportEnemyScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), m_scriptcomplyappendmode->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 101:
			ExportWorldScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), m_scriptcomplyappendmode->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 102:
			ExportFieldScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), m_scriptcomplyappendmode->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 200:
			ExportImageBackground(*dataset->fieldset, m_filepicker->GetPath(), exportlist, m_mergetile->IsChecked(), m_exportorder->IsChecked(), m_languagetitle->GetSelection() - 1);
			break;
		case 300:
			ExportWalkmesh(*dataset->fieldset, m_filepicker->GetPath(), exportlist);
			break;
		}
		delete[] exportlist;
	}
	EndModal(id);
}

// Import
BatchImportDialog::BatchImportDialog(wxWindow* p) :
	BatchImportWindow(p) {
	list_popup_menu = new wxMenu();
	list_popup_menu->Append(wxID_SELECTALL, HADES_STRING_GENERIC_SELECT_ALL);
	list_popup_menu->Append(wxID_CLEAR, HADES_STRING_GENERIC_DESELECT_ALL);
	list_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(BatchImportDialog::OnSelectRightClickMenu), NULL, this);
}

BatchImportDialog::~BatchImportDialog() {
	list_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(BatchImportDialog::OnSelectRightClickMenu), NULL, this);
}

int BatchImportDialog::ShowModal(int type, SaveSet* datas, bool isjapan) {
	datatype = type;
	dataset = datas;
	japanversion = isjapan;
	datamodified = false;
	datasectionmodified.clear();
	if (type != 1 || GetGameType() != GAME_TYPE_PSX)
		m_adjustsizepanel->Show(false);
	return wxDialog::ShowModal();
}

void BatchImportDialog::OnFilePick(wxFileDirPickerEvent& event) {
	wxFileName fname = m_filepicker->GetFileName();
	wxString fnamestr = event.GetPath();
	m_importlist->Clear();
	if (fname.IsFileReadable() && !fname.IsDir() && !wxDir::Exists(fnamestr)) {
		m_importlist->Append(fname.GetFullName());
		m_importlist->Check(0);
	} else {
		wxArrayString filelist;
		wxString dirpath = wxFileName(fnamestr).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
		wxString basename = wxFileName(fnamestr).GetName();
		wxDir::GetAllFiles(dirpath, &filelist, basename + _(L"*.txt"), wxDIR_FILES);
		for (unsigned int i = 0; i < filelist.GetCount(); i++) {
			if (wxFileName(filelist[i]).IsFileReadable()) {
				m_importlist->Append(wxFileName(filelist[i]).GetFullName());
				m_importlist->Check(i);
			}
		}
	}
}

void BatchImportDialog::OnListRightClick(wxMouseEvent& event) {
	int newsel = m_importlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_importlist->SetSelection(newsel);
		m_importlist->PopupMenu(list_popup_menu);
	}
}

void BatchImportDialog::OnSelectRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_SELECTALL) {
		for (i = 0; i < m_importlist->GetCount(); i++)
			m_importlist->Check(i, true);
	} else if (id == wxID_CLEAR) {
		for (i = 0; i < m_importlist->GetCount(); i++)
			m_importlist->Check(i, false);
	}
}

void BatchImportDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	bool ret = true;
	if (id == wxID_OK) {
		bool shownlog = false;
		bool haschecked = false;
		for (unsigned int i = 0; i < m_importlist->GetCount(); i++) {
			if (m_importlist->IsChecked(i)) {
				haschecked = true;
				wxString dirpath = wxFileName(m_filepicker->GetPath()).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
				wxString fname = dirpath + m_importlist->GetString(i);
				wxFile input(fname);
				wxString filestr = _(L"");
				if (input.IsOpened())
					input.ReadAll(&filestr);
				switch (datatype) {
				case 1: {
					LogStruct log = ImportText(dataset, datasectionmodified, filestr, m_adjustsize->IsChecked(), japanversion, m_fatalwarning->IsChecked());
					if (log.ok) {
						m_importlist->Check(i, false);
						datamodified = true;
					} else {
						ret = false;
					}
					if (log.warning_amount > 0 || log.error_amount > 0) {
						shownlog = true;
						LogDialog dial(this, log);
						dial.SetTitle(dial.GetTitle() + _(L" : ") + wxFileName(fname).GetName());
						dial.ShowModal();
					}
					break;
				}
				case 100: {
					LogStruct log = ImportScript(dataset, datasectionmodified, SCRIPT_TYPE_BATTLE, filestr, m_fatalwarning->IsChecked(), &datamodified);
					if (log.ok)
						m_importlist->Check(i, false);
					else
						ret = false;
					if (log.warning_amount > 0 || log.error_amount > 0) {
						shownlog = true;
						LogDialog dial(this, log);
						dial.SetTitle(dial.GetTitle() + _(L" : ") + wxFileName(fname).GetName());
						dial.ShowModal();
					}
					break;
				}
				case 101: {
					LogStruct log = ImportScript(dataset, datasectionmodified, SCRIPT_TYPE_WORLD, filestr, m_fatalwarning->IsChecked(), &datamodified);
					if (log.ok)
						m_importlist->Check(i, false);
					else
						ret = false;
					if (log.warning_amount > 0 || log.error_amount > 0) {
						shownlog = true;
						LogDialog dial(this, log);
						dial.SetTitle(dial.GetTitle() + _(L" : ") + wxFileName(fname).GetName());
						dial.ShowModal();
					}
					break;
				}
				case 102: {
					LogStruct log = ImportScript(dataset, datasectionmodified, SCRIPT_TYPE_FIELD, filestr, m_fatalwarning->IsChecked(), &datamodified);
					if (log.ok)
						m_importlist->Check(i, false);
					else
						ret = false;
					if (log.warning_amount > 0 || log.error_amount > 0) {
						shownlog = true;
						LogDialog dial(this, log);
						dial.SetTitle(dial.GetTitle() + _(L" : ") + wxFileName(fname).GetName());
						dial.ShowModal();
					}
					break;
				}
				}
			}
		}
		if (!shownlog) {
			LogStruct log;
			if (!haschecked)
				log.AddError(_(HADES_STRING_BATCH_NOTHING).ToStdWstring());
			LogDialog dial(this, log);
			dial.ShowModal();
		}
	}
	if (ret)
		EndModal(id);
}
