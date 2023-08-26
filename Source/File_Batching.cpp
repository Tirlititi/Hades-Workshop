#include "File_Batching.h"

#include <set>
#include "Gui_Manipulation.h"
#include "Gui_LoadingDialog.h"
#include "Gui_ScriptEditor.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "Database_Resource.h"
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

inline unsigned int FB_GetCharCount(wstring& str, wchar_t c) {
	unsigned int count = 0;
	size_t pos = str.find(c);
	while (pos != std::string::npos) {
		pos = str.find(c, pos + 1);
		count++;
	}
	return count;
}

//=============================//
//           Texts             //
//=============================//

int BatchExportDialog::ExportText(TextDataSet& data, wxString path, bool* exportlist, bool splitfile) {
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	SteamLanguage singlelang = STEAM_LANGUAGE_NONE;
	SteamLanguage lang;
	unsigned int i, j;
	wxFile output;
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		if (exportlang[lang]) {
			if (singlelang == STEAM_LANGUAGE_NONE)
				singlelang = lang;
			else
				break;
		}
	if (lang < STEAM_LANGUAGE_AMOUNT)
		singlelang = STEAM_LANGUAGE_NONE;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		if (!output.IsOpened())
			return 1;
		output.Write(_(L"#HW filetype TEXT\n\n"));
		if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
			output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
	}
	for (i = 0; i < data.amount; i++)
		if ((exportlist == NULL || exportlist[i]) && data.text_data[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				if (!output.IsOpened())
					return 1;
				output.Write(_(L"#HW filetype TEXT\n\n"));
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang] + _(L"\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), data.struct_id[i]) + _(L" // ") + _(data.name[i]) + _(L"\n"));
			for (j = 0; j < data.text_data[i]->amount; j++) {
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE && !data.text_data[i]->text[j].multi_lang_init[singlelang])
					continue;
				output.Write(_(L"#HW newtext ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
				if (GetGameType() == GAME_TYPE_PSX)
					output.Write(_(data.text_data[i]->text[j].str) + _(L"\n\n"));
				else if (singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(data.text_data[i]->text[j].multi_lang_str[singlelang]) + _(L"\n\n"));
				else
					for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
						if (exportlang[lang] && data.text_data[i]->text[j].multi_lang_init[lang]) {
							output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
							output.Write(_(data.text_data[i]->text[j].multi_lang_str[lang]) + _(L"\n\n"));
						}
			}
			if (splitfile)
				output.Close();
		}
	return 0;
}

unsigned int tmptextstructid[TXTBATCH_MAX_STRUCT];
int* tmptextid[TXTBATCH_MAX_STRUCT];
FF9String* tmptextstr[TXTBATCH_MAX_STRUCT];
LogStruct BatchImportDialog::ImportText(TextDataSet& data, wxString filetext, bool adjustsize, bool isjapan, bool fatalwarning) {
	wxString line, linebuf, errstr, hwword, token, txtvalue, inputstr = filetext;
	int currenttextstruct = -1, currenttext = -1;
	SteamLanguage lang = GetSteamLanguage();
	int nexttextstruct = 0, nexttext;
	unsigned int i, j, linenum = 0;
	bool end, filetypeok = false;
	unsigned long value;
	wstring strvalue;
	LogStruct res = LogStruct();
	while (!inputstr.IsEmpty() || currenttext >= 0) {
		linenum++;
		end = inputstr.IsEmpty();
		line = FB_GetWxStringLine(inputstr);
		linebuf = line;
		hwword = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")))
			token = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")) || end) {
			if (currenttextstruct >= 0 && currenttext >= 0 && txtvalue.Len() > 0) {
				if (txtvalue.Right(2).IsSameAs(_(L"\n\n")))
					txtvalue = txtvalue.Mid(0, txtvalue.Len() - 2);
				strvalue = txtvalue.ToStdWstring();
				if (GetGameType() == GAME_TYPE_PSX || !hades::STEAM_SINGLE_LANGUAGE_MODE || lang == GetSteamLanguage())
					tmptextstr[currenttextstruct][currenttext].SetValue(strvalue, lang);
				tmptextid[currenttextstruct][nexttext] = -1;
				if (GetGameType() == GAME_TYPE_PSX) {
					value = FB_GetCharCount(strvalue, hades::SPECIAL_STRING_OPCODE_WCHAR);
					if (value != data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]].code_amount) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_MISSMATCH_CODE), tmptextstructid[currenttextstruct], tmptextid[currenttextstruct][currenttext], data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]].code_amount, value);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
				txtvalue = _(L"");
				if (!token.IsSameAs(_(L"language")))
					currenttext = -1;
			}
		}
		if (hwword.IsSameAs(_(L"#HW"))) {
			if (token.IsSameAs(_(L"filetype"))) {
				if (!filetypeok) {
					token = FB_GetNextWord(linebuf);
					if (!token.IsSameAs(_(L"TEXT"))) {
						errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"TEXT");
						res.AddError(errstr.ToStdWstring());
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
					return res;
				}
				if (token.IsSameAs(_(L"fileid"))) {
					if (nexttextstruct >= TXTBATCH_MAX_STRUCT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TOO_MANY), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					token = FB_GetNextWord(linebuf);
					currenttextstruct = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						for (i = 0; i < data.amount; i++)
							if (data.struct_id[i] == value) {
								currenttextstruct = nexttextstruct++;
								tmptextstructid[currenttextstruct] = i;
								tmptextid[currenttextstruct] = new int[data.text_data[i]->amount + 1]; // +1 for terminating value
								tmptextstr[currenttextstruct] = new FF9String[data.text_data[i]->amount];
								tmptextid[currenttextstruct][0] = -1;
								currenttext = -1;
								nexttext = 0;
								break;
							}
						if (i == data.amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_UNUSED), value);
							res.AddWarning(errstr.ToStdWstring());
						}
					}
				} else if (token.IsSameAs(_(L"newtext"))) {
					token = FB_GetNextWord(linebuf);
					currenttext = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newtext");
						res.AddError(errstr.ToStdWstring());
					} else if (currenttextstruct >= 0) {
						if (value < 0 || value >= data.text_data[tmptextstructid[currenttextstruct]]->amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), linenum, tmptextstructid[currenttextstruct], value);
							res.AddError(errstr.ToStdWstring());
						} else {
							for (i = 0; (int)i < nexttext; i++)
								if (tmptextid[currenttextstruct][i] == value) {
									errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION), linenum, value);
									res.AddWarning(errstr.ToStdWstring());
									currenttext = i;
									break;
								}
							if (currenttext == -1)
								currenttext = nexttext++;
							tmptextid[currenttextstruct][currenttext] = value;
							tmptextstr[currenttextstruct][currenttext] = FF9String(data.text_data[tmptextstructid[currenttextstruct]]->text[value]);
							lang = GetSteamLanguage();
							txtvalue = _(L"");
						}
					}
				} else if (token.IsSameAs(_(L"language"))) {
					token = FB_GetNextWord(linebuf);
					for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							lang = i;
							break;
						}
					if (i >= STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE), linenum, token);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
			}
		} else if (currenttextstruct >= 0 && currenttext >= 0) {
			txtvalue += line + _(L"\n");
		}
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		uint32_t blocksize;
		for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
			blocksize = 4 + 8 * data.text_data[tmptextstructid[currenttextstruct]]->amount;
			for (i = 0; i < data.text_data[tmptextstructid[currenttextstruct]]->amount; i++) {
				for (j = 0; j < data.text_data[tmptextstructid[currenttextstruct]]->format_amount[i]; j++)
					blocksize += data.text_data[tmptextstructid[currenttextstruct]]->format_data[i][j].length;
				for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++)
					if (tmptextid[currenttextstruct][currenttext] == i) {
						blocksize += tmptextstr[currenttextstruct][currenttext].length;
						break;
					}
				if (tmptextid[currenttextstruct][currenttext] < 0)
					blocksize += data.text_data[tmptextstructid[currenttextstruct]]->text[i].length;
			}
			if (blocksize > data.text_data[tmptextstructid[currenttextstruct]]->size + data.text_data[tmptextstructid[currenttextstruct]]->GetExtraSize()) {
				errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_SIZE), tmptextstructid[currenttextstruct], blocksize - data.text_data[tmptextstructid[currenttextstruct]]->size - data.text_data[tmptextstructid[currenttextstruct]]->GetExtraSize());
				res.AddError(errstr.ToStdWstring());
			}
		}
	}
	if (nexttextstruct == 0) {
		errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
		res.AddError(errstr.ToStdWstring());
	}
	CharmapDataStruct* chmap = GetGameType() == GAME_TYPE_PSX ? data.charmap[data.main_charmap_index] : NULL;
	CharmapDataStruct* chmapext;
	uint16_t sizex, sizey;
	wstring nullstr = L"";
	if (fatalwarning && res.warning_amount > 0)
		res.ok = false;
	if (res.ok) {
		for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
			chmapext = GetGameType() == GAME_TYPE_PSX ? data.charmap[tmptextstructid[currenttextstruct]] : NULL;
			for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++)
				data.text_data[tmptextstructid[currenttextstruct]]->SetText(tmptextid[currenttextstruct][currenttext], nullstr);
			for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++) {
				data.text_data[tmptextstructid[currenttextstruct]]->SetText(tmptextid[currenttextstruct][currenttext], tmptextstr[currenttextstruct][currenttext]);
				if (adjustsize && GetGameType() == GAME_TYPE_PSX) { // TODO: implement the feature in Steam
					chmap->CalculateTextSize(&data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]], chmapext, &sizex, &sizey);
					data.text_data[tmptextstructid[currenttextstruct]]->SetDialogBoxSize(tmptextid[currenttextstruct][currenttext], sizex, sizey, !isjapan);
				}
			}
			data.text_data[tmptextstructid[currenttextstruct]]->MarkDataModified();
		}
	}
	for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
		delete[] tmptextid[currenttextstruct];
		delete[] tmptextstr[currenttextstruct];
	}
	return res;
}

int BatchExportDialog::ExportSpecialText(SpecialTextDataSet& data, wxString path, bool* exportlist, bool splitfile) {
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	SteamLanguage singlelang = STEAM_LANGUAGE_NONE;
	SteamLanguage lang;
	unsigned int i, j;
	wxFile output;
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		if (exportlang[lang]) {
			if (singlelang == STEAM_LANGUAGE_NONE)
				singlelang = lang;
			else
				break;
		}
	if (lang < STEAM_LANGUAGE_AMOUNT)
		singlelang = STEAM_LANGUAGE_NONE;
	if (!splitfile) {
		output.Open(path, wxFile::write);
		output.Write(_(L"#HW filetype UITEXT\n\n"));
	}
	for (i = 0; i < data.amount; i++)
		if (exportlist == NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path + wxString::Format(wxT("_%u.txt"), i + 1), wxFile::write);
				output.Write(_(L"#HW filetype UITEXT\n\n"));
			}
			output.Write(_(L"#HW fileid ") + wxString::Format(wxT("%u"), i));
			if (GetGameType() == GAME_TYPE_PSX)
				output.Write(_(L" // ") + HADES_STRING_SPECIAL_TEXT_BLOCK[i] + _(L"\n"));
			else
				output.Write(_(L" // ") + HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[i] + _(L"\n"));
			for (j = 0; j < data.text_block[i].amount; j++) {
				if (GetGameType() != GAME_TYPE_PSX && singlelang != STEAM_LANGUAGE_NONE && !data.text_block[i].text[j].multi_lang_init[singlelang])
					continue;
				output.Write(_(L"#HW newtext ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
				if (GetGameType() == GAME_TYPE_PSX)
					output.Write(_(data.text_block[i].text[j].str) + _(L"\n\n"));
				else if (singlelang != STEAM_LANGUAGE_NONE)
					output.Write(_(data.text_block[i].text[j].multi_lang_str[singlelang]) + _(L"\n\n"));
				else
					for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
						if (exportlang[lang] && data.text_block[i].text[j].multi_lang_init[lang]) {
							output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
							output.Write(_(data.text_block[i].text[j].multi_lang_str[lang]) + _(L"\n\n"));
						}
			}
			if (splitfile)
				output.Close();
		}
	return 0;
}

LogStruct BatchImportDialog::ImportSpecialText(SpecialTextDataSet& data, wxString filetext, bool fatalwarning) {
	wxString line, linebuf, errstr, hwword, token, txtvalue, inputstr = filetext;
	int currenttextstruct = -1, currenttext = -1;
	SteamLanguage lang = GetSteamLanguage();
	int nexttextstruct = 0, nexttext;
	unsigned int i, linenum = 0;
	bool end, filetypeok = false;
	unsigned long value;
	wstring strvalue;
	LogStruct res = LogStruct();
	while (!inputstr.IsEmpty() || currenttext >= 0) {
		linenum++;
		end = inputstr.IsEmpty();
		line = FB_GetWxStringLine(inputstr);
		linebuf = line;
		hwword = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")))
			token = FB_GetNextWord(linebuf);
		if (hwword.IsSameAs(_(L"#HW")) || end) {
			if (currenttextstruct >= 0 && currenttext >= 0 && txtvalue.Len() > 0) {
				if (txtvalue.Right(2).IsSameAs(_(L"\n\n")))
					txtvalue = txtvalue.Mid(0, txtvalue.Len() - 2);
				strvalue = txtvalue.ToStdWstring();
				if (GetGameType() == GAME_TYPE_PSX || !hades::STEAM_SINGLE_LANGUAGE_MODE || lang == GetSteamLanguage())
					tmptextstr[currenttextstruct][currenttext].SetValue(strvalue, lang);
				tmptextid[currenttextstruct][nexttext] = -1;
				if (GetGameType() == GAME_TYPE_PSX) {
					value = FB_GetCharCount(strvalue, hades::SPECIAL_STRING_OPCODE_WCHAR);
					if (value != data.text_block[tmptextstructid[currenttextstruct]].text[tmptextid[currenttextstruct][currenttext]].code_amount) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_MISSMATCH_CODE), tmptextstructid[currenttextstruct], tmptextid[currenttextstruct][currenttext], data.text_block[tmptextstructid[currenttextstruct]].text[tmptextid[currenttextstruct][currenttext]].code_amount, value);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
				txtvalue = _(L"");
				if (!token.IsSameAs(_(L"language")))
					currenttext = -1;
			}
		}
		if (hwword.IsSameAs(_(L"#HW"))) {
			if (token.IsSameAs(_(L"filetype"))) {
				if (!filetypeok) {
					token = FB_GetNextWord(linebuf);
					if (!token.IsSameAs(_(L"UITEXT"))) {
						errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE), linenum, L"UITEXT");
						res.AddError(errstr.ToStdWstring());
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
					return res;
				}
				if (token.IsSameAs(_(L"fileid"))) {
					if (nexttextstruct >= TXTBATCH_MAX_STRUCT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TOO_MANY), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					token = FB_GetNextWord(linebuf);
					currenttextstruct = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						if (value < data.amount) {
							currenttextstruct = nexttextstruct++;
							tmptextstructid[currenttextstruct] = value;
							tmptextid[currenttextstruct] = new int[data.text_block[value].amount + 1];
							tmptextstr[currenttextstruct] = new FF9String[data.text_block[value].amount];
							tmptextid[currenttextstruct][0] = -1;
							currenttext = -1;
							nexttext = 0;
						} else {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_UNUSED), value);
							res.AddWarning(errstr.ToStdWstring());
						}
					}
				} else if (token.IsSameAs(_(L"newtext"))) {
					token = FB_GetNextWord(linebuf);
					currenttext = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newtext");
						res.AddError(errstr.ToStdWstring());
					} else if (currenttextstruct >= 0) {
						if (value < 0 || value >= data.text_block[tmptextstructid[currenttextstruct]].amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID), linenum, tmptextstructid[currenttextstruct], value);
							res.AddError(errstr.ToStdWstring());
						} else {
							for (i = 0; (int)i < nexttext; i++)
								if (tmptextid[currenttextstruct][i] == value) {
									errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION), linenum, value);
									res.AddWarning(errstr.ToStdWstring());
									currenttext = i;
									break;
								}
							if (currenttext == -1)
								currenttext = nexttext++;
							tmptextid[currenttextstruct][currenttext] = value;
							tmptextstr[currenttextstruct][currenttext] = FF9String(data.text_block[tmptextstructid[currenttextstruct]].text[value]);
							lang = GetSteamLanguage();
							txtvalue = _(L"");
						}
					}
				} else if (token.IsSameAs(_(L"language"))) {
					token = FB_GetNextWord(linebuf);
					for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							lang = i;
							break;
						}
					if (i >= STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE), linenum, token);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
			}
		} else if (currenttextstruct >= 0 && currenttext >= 0) {
			txtvalue += line + _(L"\n");
		}
	}
	uint32_t blocksize;
	for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
		blocksize = 4 * data.text_block[tmptextstructid[currenttextstruct]].amount;
		for (i = 0; i < data.text_block[tmptextstructid[currenttextstruct]].amount; i++) {
			for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++)
				if (tmptextid[currenttextstruct][currenttext] == i) {
					blocksize += tmptextstr[currenttextstruct][currenttext].length;
					break;
				}
			if (tmptextid[currenttextstruct][currenttext] < 0)
				blocksize += data.text_block[tmptextstructid[currenttextstruct]].text[i].length;
		}
		if (blocksize > data.text_block[tmptextstructid[currenttextstruct]].space_total) {
			errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_SIZE), tmptextstructid[currenttextstruct], blocksize - data.text_block[tmptextstructid[currenttextstruct]].space_total);
			res.AddError(errstr.ToStdWstring());
		}
	}
	if (nexttextstruct == 0) {
		errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
		res.AddError(errstr.ToStdWstring());
	}
	wstring nullstr = L"";
	if (fatalwarning && res.warning_amount > 0)
		res.ok = false;
	if (res.ok) {
		for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
			for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++)
				data.text_block[tmptextstructid[currenttextstruct]].SetText(tmptextid[currenttextstruct][currenttext], nullstr);
			for (currenttext = 0; tmptextid[currenttextstruct][currenttext] >= 0; currenttext++) {
				data.text_block[tmptextstructid[currenttextstruct]].SetText(tmptextid[currenttextstruct][currenttext], tmptextstr[currenttextstruct][currenttext]);
			}
		}
	}
	for (currenttextstruct = 0; currenttextstruct < nexttextstruct; currenttextstruct++) {
		delete[] tmptextid[currenttextstruct];
		delete[] tmptextstr[currenttextstruct];
	}
	return res;
}

//=============================//
//          Scripts            //
//=============================//

int BatchExportDialog::ExportEnemyScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	EnemyDataSet& data = *dataset->enemyset;
	SteamLanguage lang, sublang, curlang;
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
			curlang = data.script[i]->current_language;
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (GetGameType() != GAME_TYPE_PSX) {
					if (data.script[i]->multi_lang_script == NULL) {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()] + _(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang] != lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
						for (sublang = 0; sublang < STEAM_LANGUAGE_AMOUNT; sublang++)
							if (sublang != lang && data.script[i]->multi_lang_script->base_script_lang[sublang] == lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid, textid;
								output.Write(_(L"#HW languagelink ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang] + _(L"\n"));
								if (langtextlinkcount > 0) {
									output.Write(_(L"#HW languagetextlink ") + wxString::Format(wxT("%u"), langtextlinkcount) + _(L"\n"));
									for (j = 0; j < langtextlinkcount; j++) {
										btextid = data.script[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"), btextid, textid);
										if (addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) {
											line += _(L" // \"") + _(FF9String::RemoveOpcodes(data.text[i]->text[btextid].multi_lang_str[lang])) + _(L"\" = \"") + _(FF9String::RemoveOpcodes(data.text[i]->text[textid].multi_lang_str[sublang])) + _(L"\"");
											line.Replace(_(L"\n"), _(L" "));
										}
										output.Write(line + _(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script[i], SCRIPT_TYPE_BATTLE, dataset, data.battle[i], data.text[i]);
				for (j = 1; j < scpthand.script.entry_amount; j++)
					if (j <= data.battle[i]->stat_amount) {
						scpthand.entry_name[j] = _(data.battle[i]->stat[j - 1].name.GetStr(2));
						scpthand.entry_name[j].Replace(_(L" "), _(L"_"));
					}
				scpthand.GenerateFunctionList();
				scpthand.GenerateEntryNames();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount > 0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j = 0; j < scpthand.script.entry_amount; j++) {
					if (scpthand.script.entry_function_amount[j] > 0) {
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
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
						for (k = 0; k < scpthand.script.entry_function_amount[j]; k++) {
							output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.function_type[j][k]) + _(L"\n"));
							output.Write(scpthand.func_str[j][k] + _(L"\n\n"));
						}
					}
				}
			}
			if (data.script[i]->current_language != curlang)
				data.script[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i + 1);
		}
	LoadingDialogEnd();
	return 0;
}

int BatchExportDialog::ExportWorldScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	WorldMapDataSet& data = *dataset->worldset;
	SteamLanguage lang, sublang, curlang;
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
			curlang = data.script[i]->current_language;
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (GetGameType() != GAME_TYPE_PSX) {
					if (data.script[i]->multi_lang_script == NULL) {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()] + _(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang] != lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
						for (sublang = 0; sublang < STEAM_LANGUAGE_AMOUNT; sublang++)
							if (sublang != lang && data.script[i]->multi_lang_script->base_script_lang[sublang] == lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid, textid;
								output.Write(_(L"#HW languagelink ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang] + _(L"\n"));
								if (langtextlinkcount > 0) {
									output.Write(_(L"#HW languagetextlink ") + wxString::Format(wxT("%u"), langtextlinkcount) + _(L"\n"));
									for (j = 0; j < langtextlinkcount; j++) {
										btextid = data.script[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"), btextid, textid);
										if (addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) {
											line += _(L" // \"") + _(FF9String::RemoveOpcodes(data.text_data[i]->text[btextid].multi_lang_str[lang])) + _(L"\" = \"") + _(FF9String::RemoveOpcodes(data.text_data[i]->text[textid].multi_lang_str[sublang])) + _(L"\"");
											line.Replace(_(L"\n"), _(L" "));
										}
										output.Write(line + _(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script[i], SCRIPT_TYPE_WORLD, dataset, NULL, data.text_data[i]);
				scpthand.GenerateFunctionList();
				scpthand.GenerateEntryNames();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount > 0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j = 0; j < scpthand.script.entry_amount; j++) {
					if (scpthand.script.entry_function_amount[j] > 0) {
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
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
						for (k = 0; k < scpthand.script.entry_function_amount[j]; k++) {
							output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.function_type[j][k]) + _(L"\n"));
							output.Write(_(scpthand.func_str[j][k]) + _(L"\n\n"));
						}
					}
				}
			}
			if (data.script[i]->current_language != curlang)
				data.script[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportFieldScript(SaveSet* dataset, wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	FieldDataSet& data = *dataset->fieldset;
	SteamLanguage lang, sublang, curlang;
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
			curlang = data.script_data[i]->current_language;
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (GetGameType() != GAME_TYPE_PSX) {
					if (data.script_data[i]->multi_lang_script == NULL) {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()] + _(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else if (data.script_data[i]->multi_lang_script->base_script_lang[lang] != lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script_data[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else {
						output.Write(_(L"#HW language ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang] + _(L"\n"));
						for (sublang = 0; sublang < STEAM_LANGUAGE_AMOUNT; sublang++)
							if (sublang != lang && data.script_data[i]->multi_lang_script->base_script_lang[sublang] == lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script_data[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid, textid;
								output.Write(_(L"#HW languagelink ") + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang] + _(L"\n"));
								if (langtextlinkcount > 0) {
									output.Write(_(L"#HW languagetextlink ") + wxString::Format(wxT("%u"), langtextlinkcount) + _(L"\n"));
									for (j = 0; j < langtextlinkcount; j++) {
										btextid = data.script_data[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script_data[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"), btextid, textid);
										if ((addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) && data.related_text[i] != NULL) {
											line += _(L" // \"") + _(FF9String::RemoveOpcodes(data.related_text[i]->text[btextid].multi_lang_str[lang])) + _(L"\" = \"") + _(FF9String::RemoveOpcodes(data.related_text[i]->text[textid].multi_lang_str[sublang])) + _(L"\"");
											line.Replace(_(L"\n"), _(L" "));
										}
										output.Write(line + _(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script_data[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script_data[i], SCRIPT_TYPE_FIELD, dataset, NULL, data.related_text[i]);
				scpthand.GenerateFunctionList();
				scpthand.GenerateEntryNames();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount > 0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j = 0; j < scpthand.script.entry_amount; j++) {
					if (scpthand.script.entry_function_amount[j] > 0) {
						output.Write(_(L"#HW newentry ") + wxString::Format(wxT("%u"), j) + _(L"\n"));
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
						for (k = 0; k < scpthand.script.entry_function_amount[j]; k++) {
							if (functypecheck.count(scpthand.script.function_type[j][k]) > 0) {
								output.Write(wxString::Format(wxT("// Duplicated function %u is discarded\n\n"), scpthand.script.function_type[j][k]));
							} else {
								output.Write(_(L"#HW newfunction ") + wxString::Format(wxT("%u"), scpthand.script.function_type[j][k]) + _(L"\n"));
								output.Write(_(scpthand.func_str[j][k]) + _(L"\n\n"));
								functypecheck.insert(scpthand.script.function_type[j][k]);
							}
						}
					}
				}
			}
			if (data.script_data[i]->current_language != curlang)
				data.script_data[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i + 1);
		}
	LoadingDialogEnd();
	return 0;
}

LogStruct BatchImportDialog::ImportScript(SaveSet* dataset, int scripttype, wxString filescript, bool fatalwarning, bool* datamodif) {
	wxString line, linebuf, errstr, token, codevalue, globalcode, localcode, inputstr = filescript;
	int filescripttype, currentbattle = -1, currententry = -1, currentfunction = -1;
	SteamLanguage lang = GetSteamLanguage(), linklang = STEAM_LANGUAGE_NONE;
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
			if (currentfunction >= 0 && (lang == GetSteamLanguage() || (current_handler != NULL && current_handler->script.multi_lang_script != NULL && current_handler->script.multi_lang_script->is_loaded[lang]))) {
				current_handler->currentvar_str = globalcode + _(L"\n") + localcode;
				tmplog = current_handler->ParseFunction(codevalue, currententry, currentfunction, codelinenum);
				entrysizegap = current_script_ptr->GetExtraSize() + current_handler->script.entry_size[currententry] - current_handler->GetParsedEntryNewSize();
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
						res.AddError(HADES_STRING_BATCH_FIELD_NOT_LOADED);
						return res;
					} else if (filescripttype == SCRIPT_TYPE_BATTLE && !dataset->sectionloaded[DATA_SECTION_ENMY]) {
						res.AddError(HADES_STRING_BATCH_ENEMY_NOT_LOADED);
						return res;
					} else if (filescripttype == SCRIPT_TYPE_WORLD && !dataset->sectionloaded[DATA_SECTION_WORLD_MAP]) {
						res.AddError(HADES_STRING_BATCH_WORLD_NOT_LOADED);
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
				if (linklang != STEAM_LANGUAGE_NONE && !token.IsSameAs(_(L"languagetextlink"))) {
					if (current_handler != NULL)
						current_handler->script.LinkLanguageScripts(linklang, lang, langtextid, baselangtextid);
					linklang = STEAM_LANGUAGE_NONE;
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
						if (current_handler->script.multi_lang_script != NULL && current_handler->script.current_language != GetSteamLanguage())
							current_handler->script.ChangeSteamLanguage(GetSteamLanguage());
						*current_script_ptr = current_handler->script;
						current_script_ptr->MarkDataModified();
						updatecur = false;
						nothingdone = false;
					}
					globalcode = localcode = _(L"");
					if (current_handler != NULL) {
						delete current_handler;
						current_handler = NULL;
					}
					token = FB_GetNextWord(linebuf);
					currentbattle = -1;
					currententry = -1;
					currentfunction = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						if (filescripttype == SCRIPT_TYPE_FIELD) {
							for (i = 0; i < dataset->fieldset->amount; i++)
								if (dataset->fieldset->script_data[i]->object_id == value) {
									currentbattle = i;
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
									currentbattle = i;
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
									currentbattle = i;
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
					token = FB_GetNextWord(linebuf);
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newentry");
						res.AddError(errstr.ToStdWstring());
						currententry = -1;
						currentfunction = -1;
					} else if (currentbattle >= 0) {
						currententry = value;
						token = FB_GetNextWord(linebuf);
						tokencheck = token.ToULong(&value);
						if (current_handler != NULL) {
							if (current_handler->script.entry_amount < currententry)
								current_handler->AddEntry(currententry, tokencheck ? value : 0);
							else if (tokencheck)
								current_handler->script.entry_type[currententry] = value;
						}
						currentfunction = -1;
					}
				} else if (token.IsSameAs(_(L"newfunction"))) {
					token = FB_GetNextWord(linebuf);
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER), linenum, L"newfunction");
						res.AddError(errstr.ToStdWstring());
						currentfunction = -1;
					} else if (currententry >= 0) {
						for (i = 0; i < current_handler->script.entry_function_amount[currententry]; i++)
							if (current_handler->script.function_type[currententry][i] == value) {
								currentfunction = i;
								break;
							}
						if (i == current_handler->script.entry_function_amount[currententry]) {
							for (i = 0; i < current_handler->script.entry_function_amount[currententry]; i++)
								if (current_handler->script.function_type[currententry][i] > value) {
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
				} else if (token.IsSameAs(_(L"languagetextlink"))) {
					langlinkblock = true;
				} else if (token.IsSameAs(_(L"language"))) {
					token = FB_GetNextWord(linebuf);
					for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							lang = i;
							if (current_handler != NULL && current_handler->script.multi_lang_script != NULL) {
								current_handler->script.LinkLanguageScripts(lang, lang);
								current_handler->script.ChangeSteamLanguage(lang);
								if (current_handler->script.multi_lang_script->is_loaded[lang])
									current_handler->script.multi_lang_script->is_modified[lang] = true;
							}
							break;
						}
					if (i >= STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE), linenum, token);
						res.AddWarning(errstr.ToStdWstring());
					}
				} else if (token.IsSameAs(_(L"languagelink"))) {
					baselangtextid.clear();
					langtextid.clear();
					token = FB_GetNextWord(linebuf);
					for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							linklang = i;
							break;
						}
					if (i >= STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE), linenum, token);
						res.AddWarning(errstr.ToStdWstring());
					}
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
		if (current_handler->script.multi_lang_script != NULL && current_handler->script.current_language != GetSteamLanguage())
			current_handler->script.ChangeSteamLanguage(GetSteamLanguage());
		*current_script_ptr = current_handler->script;
		current_script_ptr->MarkDataModified();
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
	if (type == 3 || type == 4 || type == 5) {
		m_splitfilepanel->Show(false);
		m_scriptpanel->Show();
	} else if (type == 10) {
		m_splitfilepanel->Show(false);
		m_backgroundpanel->Show();
	} else if (type == 11) {
		m_splitfilepanel->Show(false);
	}
	return wxDialog::ShowModal();
}

void BatchExportDialog::OnFilePick(wxFileDirPickerEvent& event) {
	wxString fname = event.GetPath();
	if (datatype < 10) {
		wxCheckBox* splitbox = (datatype == 1 || datatype == 2 ? m_splitfile : m_scriptsplitfile);
		bool suffix = fname.Right(4).IsSameAs(_(L".txt"));
		if (splitbox->IsChecked() && suffix)
			m_filepicker->SetPath(fname.Mid(0, fname.Len() - 4));
		else if (!splitbox->IsChecked() && !suffix)
			m_filepicker->SetPath(fname + _(L".txt"));
	} else if (datatype < 20) {
		bool suffix = fname.Right(5).IsSameAs(_(L".tiff"));
		if (suffix)
			m_filepicker->SetPath(fname.Mid(0, fname.Len() - 5));
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
			ExportSpecialText(*dataset->ffuiset->special_text, m_filepicker->GetPath(), exportlist, m_splitfile->IsChecked());
			break;
		case 3:
			ExportEnemyScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 4:
			ExportWorldScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 5:
			ExportFieldScript(dataset, m_filepicker->GetPath(), exportlist, m_scriptsplitfile->IsChecked(), BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 10:
			ExportImageBackground(*dataset->fieldset, m_filepicker->GetPath(), exportlist, m_mergetile->IsChecked(), m_exportorder->IsChecked(), m_languagetitle->GetSelection() - 1);
			break;
		case 11:
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
	if (type != 1 || GetGameType() != GAME_TYPE_PSX) {
		m_adjustsizepanel->Show(false);
	}
	return wxDialog::ShowModal();
}

void BatchImportDialog::OnFilePick(wxFileDirPickerEvent& event) {
	wxFileName fname = m_filepicker->GetFileName();
	wxString fnamestr = event.GetPath();
	unsigned int i = 0;
	m_importlist->Clear();
	if (fname.IsFileReadable()) {
		m_importlist->Append(fname.GetFullName());
		m_importlist->Check(0);
	} else {
		wxFileName tmpname = wxFileName(fnamestr + wxString::Format(wxT("_%u.txt"), i + 1));
		while (tmpname.IsFileReadable()) {
			m_importlist->Append(tmpname.GetFullName());
			m_importlist->Check(i);
			i++;
			tmpname = wxFileName(fnamestr + wxString::Format(wxT("_%u.txt"), i + 1));
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
	unsigned int i;
	bool ret = true;
	bool shownlog = false;
	bool haschecked = false;
	if (id == wxID_OK) {
		for (i = 0; i < m_importlist->GetCount(); i++) {
			if (m_importlist->IsChecked(i)) {
				haschecked = true;
				wxString fname = m_filepicker->GetPath();
				if (!wxFileName(fname).IsFileReadable())
					fname = fname + wxString::Format(wxT("_%u.txt"), i + 1);
				wxFile input(fname);
				wxString filestr = _(L"");
				if (input.IsOpened())
					input.ReadAll(&filestr);
				switch (datatype) {
				case 1: {
					LogStruct log = ImportText(*dataset->textset, filestr, m_adjustsize->IsChecked(), japanversion, m_fatalwarning->IsChecked());
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
				case 2: {
					LogStruct log = ImportSpecialText(*dataset->ffuiset->special_text, filestr, m_fatalwarning->IsChecked());
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
				case 3: {
					LogStruct log = ImportScript(dataset, SCRIPT_TYPE_BATTLE, filestr, m_fatalwarning->IsChecked(), &datamodified);
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
				case 4: {
					LogStruct log = ImportScript(dataset, SCRIPT_TYPE_WORLD, filestr, m_fatalwarning->IsChecked(), &datamodified);
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
				case 5: {
					LogStruct log = ImportScript(dataset, SCRIPT_TYPE_FIELD, filestr, m_fatalwarning->IsChecked(), &datamodified);
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
	}
	if (!shownlog) {
		LogStruct log;
		if (!haschecked)
			log.AddError(_(HADES_STRING_BATCH_NOTHING).ToStdWstring());
		LogDialog dial(this, log);
		dial.ShowModal();
	}
	if (ret)
		EndModal(id);
}
