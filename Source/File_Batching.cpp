#include "File_Batching.h"

#include "Gui_Manipulation.h"
#include "Gui_LoadingDialog.h"
#include "Gui_ScriptEditor.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "Database_Resource.h"
#include "main.h"

#define TXTBATCH_MAX_STRUCT G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME)

inline wxString FB_GetWxStringLine(wxString& str) {
	wxString tmpstr;
	wxString res = str.BeforeFirst(L'\n',&tmpstr);
	str = tmpstr;
	return res;
}

inline wxString FB_GetNextWord(wxString& str) {
	wxString tmpstr;
	wxString res = str.BeforeFirst(L' ',&tmpstr);
	str = tmpstr;
	return res;
}

inline unsigned int FB_GetCharCount(wstring& str, wchar_t c) {
	unsigned int count = 0;
	size_t pos = str.find(c);
	while (pos!=std::string::npos) {
		pos = str.find(c,pos+1);
		count++;
	}
	return count;
}

//=============================//
//           Texts             //
//=============================//

int BatchExportDialog::ExportText(wxString path, bool* exportlist, bool splitfile) {
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	TextDataSet& data = *dataset->textset;
	SteamLanguage singlelang = STEAM_LANGUAGE_NONE;
	SteamLanguage lang;
	unsigned int i,j;
	wxFile output;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (exportlang[lang]) {
			if (singlelang==STEAM_LANGUAGE_NONE)
				singlelang = lang;
			else
				break;
		}
	if (lang<STEAM_LANGUAGE_AMOUNT)
		singlelang = STEAM_LANGUAGE_NONE;
	if (!splitfile) {
		output.Open(path,wxFile::write);
		output.Write(_(L"#HW filetype TEXT\n\n"));
		if (GetGameType()!=GAME_TYPE_PSX && singlelang!=STEAM_LANGUAGE_NONE)
			output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang]+_(L"\n"));
	}
	for (i=0;i<data.amount;i++)
		if ((exportlist==NULL || exportlist[i]) && data.text_data[i]) {
			if (splitfile) {
				output.Open(path+wxString::Format(wxT("_%u.txt"),i+1),wxFile::write);
				output.Write(_(L"#HW filetype TEXT\n\n"));
				if (GetGameType()!=GAME_TYPE_PSX && singlelang!=STEAM_LANGUAGE_NONE)
					output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[singlelang]+_(L"\n"));
			}
			output.Write(_(L"#HW fileid ")+wxString::Format(wxT("%u"),data.struct_id[i])+_(L" // ")+_(data.name[i])+_(L"\n"));
			for (j=0;j<data.text_data[i]->amount;j++) {
				if (singlelang!=STEAM_LANGUAGE_NONE && !data.text_data[i]->text[j].multi_lang_init[singlelang])
					continue;
				output.Write(_(L"#HW newtext ")+wxString::Format(wxT("%u"),j+1)+_(L"\n"));
				if (GetGameType()==GAME_TYPE_PSX)
					output.Write(_(data.text_data[i]->text[j].str)+_(L"\n\n"));
				else if (singlelang!=STEAM_LANGUAGE_NONE)
					output.Write(_(data.text_data[i]->text[j].multi_lang_str[singlelang])+_(L"\n\n"));
				else
					for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
						if (exportlang[lang] && data.text_data[i]->text[j].multi_lang_init[lang]) {
							output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang]+_(L"\n"));
							output.Write(_(data.text_data[i]->text[j].multi_lang_str[lang])+_(L"\n\n"));
						}
			}
			if (splitfile)
				output.Close();
		}
	return 0;
}

unsigned int tmptextstructid[TXTBATCH_MAX_STRUCT];
uint16_t* tmptextid[TXTBATCH_MAX_STRUCT];
FF9String* tmptextstr[TXTBATCH_MAX_STRUCT];
LogStruct BatchImportDialog::ImportText(wxString filetext, bool adjustsize, bool isjapan, bool fatalwarning) {
	TextDataSet& data = *dataset->textset;
	wxString line,linebuf,errstr,token,txtvalue,inputstr = filetext;
	int currenttextstruct = -1, currenttext = -1;
	SteamLanguage lang = GetSteamLanguage();
	int nexttextstruct = 0, nexttext;
	unsigned int i,j,linenum = 0;
	bool end,filetypeok = false;
	unsigned long value;
	wstring strvalue;
	LogStruct res = LogStruct();
	while (!inputstr.IsEmpty() || currenttext>=0) {
		linenum++;
		end = inputstr.IsEmpty();
		line = FB_GetWxStringLine(inputstr);
		linebuf = line;
		token = FB_GetNextWord(linebuf);
		if (token.IsSameAs(_(L"#HW")) || end) {
			if (currenttextstruct>=0 && currenttext>=0) {
				if (txtvalue.Right(2).IsSameAs(_(L"\n\n")))
					txtvalue = txtvalue.Mid(0,txtvalue.Len()-2);
				strvalue = txtvalue.ToStdWstring();
				if (GetGameType()==GAME_TYPE_PSX || !hades::STEAM_SINGLE_LANGUAGE_MODE || lang==GetSteamLanguage())
					tmptextstr[currenttextstruct][currenttext].SetValue(strvalue,lang);
				tmptextid[currenttextstruct][nexttext] = 0;
				if (GetGameType()==GAME_TYPE_PSX) {
					value = FB_GetCharCount(strvalue,hades::SPECIAL_STRING_OPCODE_WCHAR);
					if (value!=data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]-1].code_amount) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_MISSMATCH_CODE),tmptextstructid[currenttextstruct],tmptextid[currenttextstruct][currenttext],data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]-1].code_amount,value);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
				currenttext = -1;
			}
		}
		if (token.IsSameAs(_(L"#HW"))) {
			token = FB_GetNextWord(linebuf);
			if (token.IsSameAs(_(L"filetype"))) {
				if (!filetypeok) {
					token = FB_GetNextWord(linebuf);
					if (!token.IsSameAs(_(L"TEXT"))) {
						errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE),linenum,L"TEXT");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					filetypeok = true;
				} else {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_TWICE),linenum);
					res.AddWarning(errstr.ToStdWstring());
				}
			} else {
				if (!filetypeok) {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_NOTFIRST),linenum);
					res.AddError(errstr.ToStdWstring());
					return res;
				}
				if (token.IsSameAs(_(L"fileid"))) {
					if (nexttextstruct>=TXTBATCH_MAX_STRUCT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TOO_MANY),linenum,L"fileid");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					token = FB_GetNextWord(linebuf);
					currenttextstruct = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER),linenum,L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						for (i=0;i<data.amount;i++)
							if (data.struct_id[i]==value) {
								currenttextstruct = nexttextstruct++;
								tmptextstructid[currenttextstruct] = i;
								tmptextid[currenttextstruct] = new uint16_t[data.text_data[i]->amount+1];
								tmptextstr[currenttextstruct] = new FF9String[data.text_data[i]->amount+1];
								tmptextid[currenttextstruct][0] = 0;
								currenttext = -1;
								nexttext = 0;
								break;
							}
						if (i==data.amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_UNUSED),value);
							res.AddWarning(errstr.ToStdWstring());
						}
					}
				} else if (token.IsSameAs(_(L"newtext"))) {
					token = FB_GetNextWord(linebuf);
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER),linenum,L"newtext");
						res.AddError(errstr.ToStdWstring());
						currenttext = -1;
					} else if (currenttextstruct>=0) {
						if (value<=0 || value>data.text_data[tmptextstructid[currenttextstruct]]->amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID),linenum,tmptextstructid[currenttextstruct],value);
							res.AddError(errstr.ToStdWstring());
						} else {
							currenttext = -1;
							for (i=0;i<nexttext;i++)
								if (tmptextid[currenttextstruct][i]==value) {
									errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION),linenum,value);
									res.AddWarning(errstr.ToStdWstring());
									currenttext = i;
									break;
								}
							if (currenttext==-1)
								currenttext = nexttext++;
							tmptextid[currenttextstruct][currenttext] = value;
							tmptextstr[currenttextstruct][currenttext] = FF9String(data.text_data[tmptextstructid[currenttextstruct]]->text[value-1]);
							txtvalue = _(L"");
						}
					}
				} else if (token.IsSameAs(_(L"language"))) {
					token = FB_GetNextWord(linebuf);
					for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							lang = i;
							break;
						}
					if (i>=STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE),linenum,token);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
			}
		} else if (currenttextstruct>=0 && currenttext>=0) {
			txtvalue += line+_(L"\n");
		}
	}
	if (GetGameType()==GAME_TYPE_PSX) {
		uint32_t blocksize;
		for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
			blocksize = 4+8*data.text_data[tmptextstructid[currenttextstruct]]->amount;
			for (i=0;i<data.text_data[tmptextstructid[currenttextstruct]]->amount;i++) {
				for (j=0;j<data.text_data[tmptextstructid[currenttextstruct]]->format_amount[i];j++)
					blocksize += data.text_data[tmptextstructid[currenttextstruct]]->format_data[i][j].length;
				for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++)
					if (tmptextid[currenttextstruct][currenttext]==i+1) {
						blocksize += tmptextstr[currenttextstruct][currenttext].length;
						break;
					}
				if (tmptextid[currenttextstruct][currenttext]==0)
					blocksize += data.text_data[tmptextstructid[currenttextstruct]]->text[i].length;
			}
			if (blocksize>data.text_data[tmptextstructid[currenttextstruct]]->size+data.text_data[tmptextstructid[currenttextstruct]]->GetExtraSize()) {
				errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_SIZE),tmptextstructid[currenttextstruct],blocksize-data.text_data[tmptextstructid[currenttextstruct]]->size-data.text_data[tmptextstructid[currenttextstruct]]->GetExtraSize());
				res.AddError(errstr.ToStdWstring());
			}
		}
	}
	if (nexttextstruct==0) {
		errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
		res.AddError(errstr.ToStdWstring());
	}
	CharmapDataStruct* chmap = GetGameType()==GAME_TYPE_PSX ? data.charmap[data.main_charmap_index] : NULL;
	CharmapDataStruct* chmapext;
	uint16_t sizex, sizey;
	wstring nullstr = L"";
	if ((!fatalwarning || res.warning_amount==0) && res.error_amount==0) {
		for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
			chmapext = GetGameType()==GAME_TYPE_PSX ? data.charmap[tmptextstructid[currenttextstruct]] : NULL;
			for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++)
				data.text_data[tmptextstructid[currenttextstruct]]->SetText(tmptextid[currenttextstruct][currenttext]-1,nullstr);
			for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++) {
				data.text_data[tmptextstructid[currenttextstruct]]->SetText(tmptextid[currenttextstruct][currenttext]-1,tmptextstr[currenttextstruct][currenttext]);
				if (adjustsize && GetGameType()==GAME_TYPE_PSX) { // ToDo: implement the feature in Steam
					chmap->CalculateTextSize(&data.text_data[tmptextstructid[currenttextstruct]]->text[tmptextid[currenttextstruct][currenttext]-1],chmapext,&sizex,&sizey);
					data.text_data[tmptextstructid[currenttextstruct]]->SetDialogBoxSize(tmptextid[currenttextstruct][currenttext]-1,sizex,sizey,!isjapan);
				}
			}
			data.text_data[tmptextstructid[currenttextstruct]]->MarkDataModified();
		}
		res.ok = true;
	} else
		res.ok = false;
	for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
		delete[] tmptextid[currenttextstruct];
		delete[] tmptextstr[currenttextstruct];
	}
	return res;
}

int BatchExportDialog::ExportSpecialText(wxString path, bool* exportlist, bool splitfile) {
	bool* exportlang = hades::STEAM_LANGUAGE_SAVE_LIST;
	SpecialTextDataSet& data = *dataset->ffuiset->special_text;
	SteamLanguage singlelang = STEAM_LANGUAGE_NONE;
	SteamLanguage lang;
	unsigned int i,j;
	wxFile output;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (exportlang[lang]) {
			if (singlelang==STEAM_LANGUAGE_NONE)
				singlelang = lang;
			else
				break;
		}
	if (lang<STEAM_LANGUAGE_AMOUNT)
		singlelang = STEAM_LANGUAGE_NONE;
	if (!splitfile) {
		output.Open(path,wxFile::write);
		output.Write(_(L"#HW filetype UITEXT\n\n"));
	}
	for (i=0;i<data.amount;i++)
		if (exportlist==NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path+wxString::Format(wxT("_%u.txt"),i+1),wxFile::write);
				output.Write(_(L"#HW filetype UITEXT\n\n"));
			}
			output.Write(_(L"#HW fileid ")+wxString::Format(wxT("%u"),i));
			if (GetGameType()==GAME_TYPE_PSX)
				output.Write(_(L" // ")+HADES_STRING_SPECIAL_TEXT_BLOCK[i]+_(L"\n"));
			else
				output.Write(_(L" // ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[i]+_(L"\n"));
			for (j=0;j<data.text_block[i].amount;j++) {
				if (singlelang!=STEAM_LANGUAGE_NONE && !data.text_block[i].text[j].multi_lang_init[singlelang])
					continue;
				output.Write(_(L"#HW newtext ")+wxString::Format(wxT("%u"),j+1)+_(L"\n"));
				if (GetGameType()==GAME_TYPE_PSX)
					output.Write(_(data.text_block[i].text[j].str)+_(L"\n\n"));
				else if (singlelang!=STEAM_LANGUAGE_NONE)
					output.Write(_(data.text_block[i].text[j].multi_lang_str[singlelang])+_(L"\n\n"));
				else
					for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
						if (exportlang[lang] && data.text_block[i].text[j].multi_lang_init[lang]) {
							output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang]+_(L"\n"));
							output.Write(_(data.text_block[i].text[j].multi_lang_str[lang])+_(L"\n\n"));
						}
			}
			if (splitfile)
				output.Close();
		}
	return 0;
}

LogStruct BatchImportDialog::ImportSpecialText(wxString filetext, bool fatalwarning) {
	SpecialTextDataSet& data = *dataset->ffuiset->special_text;
	wxString line,linebuf,errstr,token,txtvalue,inputstr = filetext;
	int currenttextstruct = -1, currenttext = -1;
	SteamLanguage lang = GetSteamLanguage();
	int nexttextstruct = 0, nexttext;
	unsigned int i,linenum = 0;
	bool end,filetypeok = false;
	unsigned long value;
	wstring strvalue;
	LogStruct res = LogStruct();
	while (!inputstr.IsEmpty() || currenttext>=0) {
		linenum++;
		end = inputstr.IsEmpty();
		line = FB_GetWxStringLine(inputstr);
		linebuf = line;
		token = FB_GetNextWord(linebuf);
		if (token.IsSameAs(_(L"#HW")) || end) {
			if (currenttextstruct>=0 && currenttext>=0) {
				if (txtvalue.Right(2).IsSameAs(_(L"\n\n")))
					txtvalue = txtvalue.Mid(0,txtvalue.Len()-2);
				strvalue = txtvalue.ToStdWstring();
				if (GetGameType()==GAME_TYPE_PSX || !hades::STEAM_SINGLE_LANGUAGE_MODE || lang==GetSteamLanguage())
					tmptextstr[currenttextstruct][currenttext].SetValue(strvalue,lang);
				tmptextid[currenttextstruct][nexttext] = 0;
				value = FB_GetCharCount(strvalue,hades::SPECIAL_STRING_OPCODE_WCHAR);
				if (value!=data.text_block[tmptextstructid[currenttextstruct]].text[tmptextid[currenttextstruct][currenttext]-1].code_amount) {
					errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_MISSMATCH_CODE),tmptextstructid[currenttextstruct],tmptextid[currenttextstruct][currenttext],data.text_block[tmptextstructid[currenttextstruct]].text[tmptextid[currenttextstruct][currenttext]-1].code_amount,value);
					res.AddWarning(errstr.ToStdWstring());
				}
				currenttext = -1;
			}
		}
		if (token.IsSameAs(_(L"#HW"))) {
			token = FB_GetNextWord(linebuf);
			if (token.IsSameAs(_(L"filetype"))) {
				if (!filetypeok) {
					token = FB_GetNextWord(linebuf);
					if (!token.IsSameAs(_(L"UITEXT"))) {
						errstr.Printf(wxT(HADES_STRING_BATCH_WRONG_FILETYPE),linenum,L"UITEXT");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					filetypeok = true;
				} else {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_TWICE),linenum);
					res.AddWarning(errstr.ToStdWstring());
				}
			} else {
				if (!filetypeok) {
					errstr.Printf(wxT(HADES_STRING_BATCH_FILETYPE_NOTFIRST),linenum);
					res.AddError(errstr.ToStdWstring());
					return res;
				}
				if (token.IsSameAs(_(L"fileid"))) {
					if (nexttextstruct>=TXTBATCH_MAX_STRUCT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_TOO_MANY),linenum,L"fileid");
						res.AddError(errstr.ToStdWstring());
						return res;
					}
					token = FB_GetNextWord(linebuf);
					currenttextstruct = -1;
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER),linenum,L"fileid");
						res.AddError(errstr.ToStdWstring());
					} else {
						if (value<data.amount) {
							currenttextstruct = nexttextstruct++;
							tmptextstructid[currenttextstruct] = value;
							tmptextid[currenttextstruct] = new uint16_t[data.text_block[value].amount+1];
							tmptextstr[currenttextstruct] = new FF9String[data.text_block[value].amount+1];
							tmptextid[currenttextstruct][0] = 0;
							currenttext = -1;
							nexttext = 0;
						} else {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_UNUSED),value);
							res.AddWarning(errstr.ToStdWstring());
						}
					}
				} else if (token.IsSameAs(_(L"newtext"))) {
					token = FB_GetNextWord(linebuf);
					if (!token.ToULong(&value)) {
						errstr.Printf(wxT(HADES_STRING_BATCH_MISSING_INTEGER),linenum,L"newtext");
						res.AddError(errstr.ToStdWstring());
						currenttext = -1;
					} else if (currenttextstruct>=0) {
						if (value<=0 || value>data.text_block[tmptextstructid[currenttextstruct]].amount) {
							errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_WRONG_ID),linenum,tmptextstructid[currenttextstruct],value);
							res.AddError(errstr.ToStdWstring());
						} else {
							currenttext = -1;
							for (i=0;i<nexttext;i++)
								if (tmptextid[currenttextstruct][i]==value) {
									errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_REDEFINITION),linenum,value);
									res.AddWarning(errstr.ToStdWstring());
									currenttext = i;
									break;
								}
							if (currenttext==-1)
								currenttext = nexttext++;
							tmptextid[currenttextstruct][currenttext] = value;
							tmptextstr[currenttextstruct][currenttext] = FF9String(data.text_block[tmptextstructid[currenttextstruct]].text[value-1]);
							txtvalue = _(L"");
						}
					}
				} else if (token.IsSameAs(_(L"language"))) {
					token = FB_GetNextWord(linebuf);
					for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
						if (HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i].IsSameAs(token)) {
							lang = i;
							break;
						}
					if (i>=STEAM_LANGUAGE_AMOUNT) {
						errstr.Printf(wxT(HADES_STRING_BATCH_INVALID_LANGUAGE),linenum,token);
						res.AddWarning(errstr.ToStdWstring());
					}
				}
			}
		} else if (currenttextstruct>=0 && currenttext>=0) {
			txtvalue += line+_(L"\n");
		}
	}
	uint32_t blocksize;
	for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
		blocksize = 4*data.text_block[tmptextstructid[currenttextstruct]].amount;
		for (i=0;i<data.text_block[tmptextstructid[currenttextstruct]].amount;i++) {
			for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++)
				if (tmptextid[currenttextstruct][currenttext]==i+1) {
					blocksize += tmptextstr[currenttextstruct][currenttext].length;
					break;
				}
			if (tmptextid[currenttextstruct][currenttext]==0)
				blocksize += data.text_block[tmptextstructid[currenttextstruct]].text[i].length;
		}
		if (blocksize>data.text_block[tmptextstructid[currenttextstruct]].space_total) {
			errstr.Printf(wxT(HADES_STRING_BATCH_TEXT_SIZE),tmptextstructid[currenttextstruct],blocksize-data.text_block[tmptextstructid[currenttextstruct]].space_total);
			res.AddError(errstr.ToStdWstring());
		}
	}
	if (nexttextstruct==0) {
		errstr.Printf(wxT(HADES_STRING_BATCH_NOTHING));
		res.AddError(errstr.ToStdWstring());
	}
	wstring nullstr = L"";
	if ((!fatalwarning || res.warning_amount==0) && res.error_amount==0) {
		for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
			for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++)
				data.text_block[tmptextstructid[currenttextstruct]].SetText(tmptextid[currenttextstruct][currenttext]-1,nullstr);
			for (currenttext=0;tmptextid[currenttextstruct][currenttext]>0;currenttext++)
				data.text_block[tmptextstructid[currenttextstruct]].SetText(tmptextid[currenttextstruct][currenttext]-1,tmptextstr[currenttextstruct][currenttext]);
		}
		res.ok = true;
	} else
		res.ok = false;
	for (currenttextstruct=0;currenttextstruct<nexttextstruct;currenttextstruct++) {
		delete[] tmptextid[currenttextstruct];
		delete[] tmptextstr[currenttextstruct];
	}
	return res;
}

//=============================//
//          Scripts            //
//=============================//

int BatchExportDialog::ExportEnemyScript(wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	EnemyDataSet& data = *dataset->enemyset;
	SteamLanguage lang,sublang,curlang;
	wxString line,tmprest,localstr;
	unsigned int i,j,k;
	wxFile output;
	if (!splitfile) {
		output.Open(path,wxFile::write);
		output.Write(_(L"#HW filetype ENEMYSCRIPT\n\n"));
	}
	LoadingDialogInit(data.battle_amount,_(L"Exporting AI scripts..."));
	for (i=0;i<data.battle_amount;i++)
		if (exportlist==NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path+wxString::Format(wxT("_%u.txt"),i+1),wxFile::write);
				output.Write(_(L"#HW filetype ENEMYSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ")+wxString::Format(wxT("%u"),data.battle_data[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				output.Write(_(L" // ")+_(data.battle_name[i]));
			output.Write(_(L"\n"));
			curlang = data.script[i]->current_language;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (GetGameType()!=GAME_TYPE_PSX) {
					if (data.script[i]->multi_lang_script==NULL) {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()]+_(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang]!=lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang]==lang && !hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang]+_(L"\n"));
						for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
							if (sublang!=lang && data.script[i]->multi_lang_script->base_script_lang[sublang]==lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid,textid;
								output.Write(_(L"#HW languagelink ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang]+_(L"\n"));
								if (langtextlinkcount>0) {
									output.Write(_(L"#HW languagetextlink ")+wxString::Format(wxT("%u"),langtextlinkcount)+_(L"\n"));
									for (j=0;j<langtextlinkcount;j++) {
										btextid = data.script[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"),btextid,textid);
										if (addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) {
											line += _(L" // \"")+_(FF9String::RemoveOpcodes(data.text[i]->text[btextid].multi_lang_str[lang]))+_(L"\" = \"")+_(FF9String::RemoveOpcodes(data.text[i]->text[textid].multi_lang_str[sublang]))+_(L"\"");
											line.Replace(_(L"\n"),_(L" "));
										}
										output.Write(line+_(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script[i],SCRIPT_TYPE_BATTLE,dataset,data.battle[i],data.text[i],dataloaded);
				for (j=1;j<scpthand.script.entry_amount;j++)
					if (j<=data.battle[i]->stat_amount) {
						scpthand.entry_name[j] = _(data.battle[i]->stat[j-1].name.GetStr(2));
						scpthand.entry_name[j].Replace(_(L" "),_(L"_"));
					}
				scpthand.GenerateFunctionList();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount>0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j=0;j<scpthand.script.entry_amount;j++) {
					if (scpthand.script.entry_function_amount[j]>0) {
						output.Write(_(L"#HW newentry ")+wxString::Format(wxT("%u"),j)+_(L"\n"));
						if (scpthand.script.local_data[j].allocate_amount>0 || scpthand.script.local_data[j].amount>0) {
							localstr = scpthand.localvar_str[j];
							output.Write(_(L"#HW locals\n"));
							while (localstr.Len()>0) {
								line = localstr.BeforeFirst(L'\n',&tmprest);
								localstr = tmprest;
								if (!line.Mid(0,7).IsSameAs(L"global "))
									output.Write(line+_(L"\n"));
							}
							output.Write(_(L"#HW endlocals\n\n"));
						}
						for (k=0;k<scpthand.script.entry_function_amount[j];k++) {
							output.Write(_(L"#HW newfunction ")+wxString::Format(wxT("%u"),scpthand.script.function_type[j][k])+_(L"\n"));
							output.Write(scpthand.func_str[j][k]+_(L"\n\n"));
						}
					}
				}
			}
			if (data.script[i]->current_language!=curlang)
				data.script[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i+1);
		}
	LoadingDialogEnd();
	return 0;
}

int BatchExportDialog::ExportWorldScript(wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	WorldMapDataSet& data = *dataset->worldset;
	SteamLanguage lang,sublang,curlang;
	wxString line,tmprest,localstr;
	unsigned int i,j,k;
	wxFile output;
	if (!splitfile) {
		output.Open(path,wxFile::write);
		output.Write(_(L"#HW filetype WORLDSCRIPT\n\n"));
	}
	for (i=0;i<data.amount;i++)
		if (exportlist==NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path+wxString::Format(wxT("_%u.txt"),i+1),wxFile::write);
				output.Write(_(L"#HW filetype WORLDSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ")+wxString::Format(wxT("%u"),data.script[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				for (j=0;j<G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME);j++)
					if (HADES_STRING_WORLD_BLOCK_NAME[j].id==data.script[i]->object_id) {
						output.Write(_(L" // ")+HADES_STRING_WORLD_BLOCK_NAME[j].label);
						break;
					}
			output.Write(_(L"\n"));
			curlang = data.script[i]->current_language;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (GetGameType()!=GAME_TYPE_PSX) {
					if (data.script[i]->multi_lang_script==NULL) {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()]+_(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang]!=lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else if (data.script[i]->multi_lang_script->base_script_lang[lang]==lang && !hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang]+_(L"\n"));
						for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
							if (sublang!=lang && data.script[i]->multi_lang_script->base_script_lang[sublang]==lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid,textid;
								output.Write(_(L"#HW languagelink ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang]+_(L"\n"));
								if (langtextlinkcount>0) {
									output.Write(_(L"#HW languagetextlink ")+wxString::Format(wxT("%u"),langtextlinkcount)+_(L"\n"));
									for (j=0;j<langtextlinkcount;j++) {
										btextid = data.script[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"),btextid,textid);
										if (addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) {
											line += _(L" // \"")+_(FF9String::RemoveOpcodes(data.text_data[i]->text[btextid].multi_lang_str[lang]))+_(L"\" = \"")+_(FF9String::RemoveOpcodes(data.text_data[i]->text[textid].multi_lang_str[sublang]))+_(L"\"");
											line.Replace(_(L"\n"),_(L" "));
										}
										output.Write(line+_(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script[i],SCRIPT_TYPE_WORLD,dataset,NULL,data.text_data[i],dataloaded);
				scpthand.GenerateFunctionList();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount>0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j=0;j<scpthand.script.entry_amount;j++) {
					if (scpthand.script.entry_function_amount[j]>0) {
						output.Write(_(L"#HW newentry ")+wxString::Format(wxT("%u"),j)+_(L"\n"));
						if (scpthand.script.local_data[j].allocate_amount>0 || scpthand.script.local_data[j].amount>0) {
							localstr = scpthand.localvar_str[j];
							output.Write(_(L"#HW locals\n"));
							while (localstr.Len()>0) {
								line = localstr.BeforeFirst(L'\n',&tmprest);
								localstr = tmprest;
								if (!line.Mid(0,7).IsSameAs(L"global "))
									output.Write(line+_(L"\n"));
							}
							output.Write(_(L"#HW endlocals\n\n"));
						}
						for (k=0;k<scpthand.script.entry_function_amount[j];k++) {
							output.Write(_(L"#HW newfunction ")+wxString::Format(wxT("%u"),scpthand.script.function_type[j][k])+_(L"\n"));
							output.Write(_(scpthand.func_str[j][k])+_(L"\n\n"));
						}
					}
				}
			}
			if (data.script[i]->current_language!=curlang)
				data.script[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
		}
	return 0;
}

int BatchExportDialog::ExportFieldScript(wxString path, bool* exportlist, bool splitfile, int addedinfo) {
	FieldDataSet& data = *dataset->fieldset;
	SteamLanguage lang,sublang,curlang;
	wxString line,tmprest,localstr;
	unsigned int i,j,k;
	wxFile output;
	if (!splitfile) {
		output.Open(path,wxFile::write);
		output.Write(_(L"#HW filetype FIELDSCRIPT\n\n"));
	}
	LoadingDialogInit(data.amount,_(L"Exporting field scripts..."));
	for (i=0;i<data.amount;i++)
		if (exportlist==NULL || exportlist[i]) {
			if (splitfile) {
				output.Open(path+wxString::Format(wxT("_%u.txt"),i+1),wxFile::write);
				output.Write(_(L"#HW filetype FIELDSCRIPT\n\n"));
			}
			output.Write(_(L"#HW fileid ")+wxString::Format(wxT("%u"),data.script_data[i]->object_id));
			if (addedinfo & BATCHING_SCRIPT_INFO_FILENAME)
				output.Write(_(L" // ")+_(data.script_data[i]->name.str_nice));
			output.Write(_(L"\n"));
			curlang = data.script_data[i]->current_language;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (GetGameType()!=GAME_TYPE_PSX) {
					if (data.script_data[i]->multi_lang_script==NULL) {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[GetSteamLanguage()]+_(L"\n"));
						lang = STEAM_LANGUAGE_AMOUNT;
					} else if (data.script_data[i]->multi_lang_script->base_script_lang[lang]!=lang && hades::STEAM_LANGUAGE_SAVE_LIST[data.script_data[i]->multi_lang_script->base_script_lang[lang]]) {
						continue;
					} else if (data.script_data[i]->multi_lang_script->base_script_lang[lang]==lang && !hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						continue;
					} else {
						output.Write(_(L"#HW language ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang]+_(L"\n"));
						for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
							if (sublang!=lang && data.script_data[i]->multi_lang_script->base_script_lang[sublang]==lang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang]) {
								unsigned int langtextlinkcount = data.script_data[i]->multi_lang_script->base_script_text_id[sublang].size();
								uint16_t btextid,textid;
								output.Write(_(L"#HW languagelink ")+HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[sublang]+_(L"\n"));
								if (langtextlinkcount>0) {
									output.Write(_(L"#HW languagetextlink ")+wxString::Format(wxT("%u"),langtextlinkcount)+_(L"\n"));
									for (j=0;j<langtextlinkcount;j++) {
										btextid = data.script_data[i]->multi_lang_script->base_script_text_id[sublang][j];
										textid = data.script_data[i]->multi_lang_script->lang_script_text_id[sublang][j];
										wxString line = wxString::Format(wxT("%u %u"),btextid,textid);
										if ((addedinfo & BATCHING_SCRIPT_INFO_TEXT_LINK) && data.related_text[i]!=NULL) {
											line += _(L" // \"")+_(FF9String::RemoveOpcodes(data.related_text[i]->text[btextid].multi_lang_str[lang]))+_(L"\" = \"")+_(FF9String::RemoveOpcodes(data.related_text[i]->text[textid].multi_lang_str[sublang]))+_(L"\"");
											line.Replace(_(L"\n"),_(L" "));
										}
										output.Write(line+_(L"\n"));
									}
									output.Write(_(L"\n"));
								}
							}
						data.script_data[i]->ChangeSteamLanguage(lang);
					}
				} else {
					lang = STEAM_LANGUAGE_AMOUNT;
				}
				ScriptEditHandler scpthand(*data.script_data[i],SCRIPT_TYPE_FIELD,dataset,NULL,data.related_text[i],dataloaded);
				scpthand.GenerateFunctionList();
				scpthand.GenerateFunctionStrings(addedinfo & BATCHING_SCRIPT_INFO_ARGUMENT);
				if (scpthand.script.global_data.amount>0) {
					output.Write(_(L"#HW globals\n"));
					output.Write(scpthand.globalvar_str);
					output.Write(_(L"#HW endglobals\n\n"));
				}
				for (j=0;j<scpthand.script.entry_amount;j++) {
					if (scpthand.script.entry_function_amount[j]>0) {
						output.Write(_(L"#HW newentry ")+wxString::Format(wxT("%u"),j)+_(L"\n"));
						if (scpthand.script.local_data[j].allocate_amount>0 || scpthand.script.local_data[j].amount>0) {
							localstr = scpthand.localvar_str[j];
							output.Write(_(L"#HW locals\n"));
							while (localstr.Len()>0) {
								line = localstr.BeforeFirst(L'\n',&tmprest);
								localstr = tmprest;
								if (!line.Mid(0,7).IsSameAs(L"global "))
									output.Write(line+_(L"\n"));
							}
							output.Write(_(L"#HW endlocals\n\n"));
						}
						for (k=0;k<scpthand.script.entry_function_amount[j];k++) {
							output.Write(_(L"#HW newfunction ")+wxString::Format(wxT("%u"),scpthand.script.function_type[j][k])+_(L"\n"));
							output.Write(_(scpthand.func_str[j][k])+_(L"\n\n"));
						}
					}
				}
			}
			if (data.script_data[i]->current_language!=curlang)
				data.script_data[i]->ChangeSteamLanguage(curlang);
			if (splitfile)
				output.Close();
			LoadingDialogUpdate(i+1);
		}
	LoadingDialogEnd();
	return 0;
}

//=============================//
//           Images            //
//=============================//

int BatchExportDialog::ExportImageBackground(wxString path, bool* exportlist, bool mergetile, bool depthorder, int steamtitlelang) {
	FieldDataSet& data = *dataset->fieldset;
	unsigned int i,j;
	bool mustflush;
	LoadingDialogInit(data.amount,_(L"Exporting field backgrounds..."));
	for (i=0;i<data.amount;i++)
		if (exportlist==NULL || exportlist[i]) {
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

//=============================//
//            GUI              //
//=============================//

// Export
BatchExportDialog::BatchExportDialog(wxWindow* p) :
	BatchExportWindow(p) {
	list_popup_menu = new wxMenu();
	list_popup_menu->Append(wxID_SELECTALL,HADES_STRING_GENERIC_SELECT_ALL);
	list_popup_menu->Append(wxID_CLEAR,HADES_STRING_GENERIC_DESELECT_ALL);
	list_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(BatchExportDialog::OnSelectRightClickMenu),NULL,this);
}

BatchExportDialog::~BatchExportDialog() {
	list_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(BatchExportDialog::OnSelectRightClickMenu),NULL,this);
}

int BatchExportDialog::ShowModal(int type, SaveSet* datas, wxArrayString objlist, unsigned int* objlistsort, bool* dload) {
	unsigned int i;
	datatype = type;
	sortlist = objlistsort;
	dataset = datas;
	dataloaded = dload;
	m_exportlist->Append(objlist);
	for (i=0;i<objlist.Count();i++)
		m_exportlist->Check(i);
	if (type==3 || type==4 || type==5) {
		m_splitfilepanel->Show(false);
		m_scriptpanel->Show();
	} else if (type==10) {
		m_splitfilepanel->Show(false);
		m_backgroundpanel->Show();
	}
	return wxDialog::ShowModal();
}

void BatchExportDialog::OnFilePick(wxFileDirPickerEvent& event) {
	wxString fname = event.GetPath();
	if (datatype<10) {
		wxCheckBox* splitbox = (datatype==1 || datatype==2 ? m_splitfile : m_scriptsplitfile);
		bool suffix = fname.Right(4).IsSameAs(_(L".txt"));
		if (splitbox->IsChecked() && suffix)
			m_filepicker->SetPath(fname.Mid(0,fname.Len()-4));
		else if (!splitbox->IsChecked() && !suffix)
			m_filepicker->SetPath(fname+_(L".txt"));
	} else if (datatype<20) {
		bool suffix = fname.Right(5).IsSameAs(_(L".tiff"));
		if (suffix)
			m_filepicker->SetPath(fname.Mid(0,fname.Len()-5));
	}
}

void BatchExportDialog::OnSplitFileCheck(wxCommandEvent& event) {
	wxString fname = m_filepicker->GetPath();
	bool suffix = fname.Right(4).IsSameAs(_(L".txt"));
	if (event.IsChecked() && suffix)
		m_filepicker->SetPath(fname.Mid(0,fname.Len()-4));
	else if (!event.IsChecked() && !suffix && fname.Len()>0)
		m_filepicker->SetPath(fname+_(L".txt"));
}

void BatchExportDialog::OnListRightClick(wxMouseEvent& event) {
	int newsel = m_exportlist->HitTest(event.GetPosition());
	if (newsel!=wxNOT_FOUND) {
		m_exportlist->SetSelection(newsel);
		m_exportlist->PopupMenu(list_popup_menu);
	}
}

void BatchExportDialog::OnSelectRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id==wxID_SELECTALL) {
		for (i=0;i<m_exportlist->GetCount();i++)
			m_exportlist->Check(i,true);
	} else if (id==wxID_CLEAR) {
		for (i=0;i<m_exportlist->GetCount();i++)
			m_exportlist->Check(i,false);
	}
}

void BatchExportDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i,j;
	if (id==wxID_OK && !m_filepicker->GetPath().IsEmpty() && !m_filepicker->GetPath().IsSameAs(_(L".txt"))) {
		bool* exportlist = new bool[m_exportlist->GetCount()];
		for (i=0;i<m_exportlist->GetCount();i++) {
			j = sortlist==NULL ? i : sortlist[i];
			if (m_exportlist->IsChecked(i))
				exportlist[j] = true;
			else
				exportlist[j] = false;
		}
		switch (datatype) {
		case 1:
			ExportText(m_filepicker->GetPath(),exportlist,m_splitfile->IsChecked());
			break;
		case 2:
			ExportSpecialText(m_filepicker->GetPath(),exportlist,m_splitfile->IsChecked());
			break;
		case 3:
			ExportEnemyScript(m_filepicker->GetPath(),exportlist,m_scriptsplitfile->IsChecked(),BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 4:
			ExportWorldScript(m_filepicker->GetPath(),exportlist,m_scriptsplitfile->IsChecked(),BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 5:
			ExportFieldScript(m_filepicker->GetPath(),exportlist,m_scriptsplitfile->IsChecked(),BATCHING_SCRIPT_INFO_FILENAME | BATCHING_SCRIPT_INFO_TEXT_LINK | (m_scriptcomment->IsChecked() ? BATCHING_SCRIPT_INFO_ARGUMENT : 0));
			break;
		case 10:
			ExportImageBackground(m_filepicker->GetPath(),exportlist,m_mergetile->IsChecked(),m_exportorder->IsChecked(),m_languagetitle->GetSelection()-1);
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
	list_popup_menu->Append(wxID_SELECTALL,HADES_STRING_GENERIC_SELECT_ALL);
	list_popup_menu->Append(wxID_CLEAR,HADES_STRING_GENERIC_DESELECT_ALL);
	list_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(BatchImportDialog::OnSelectRightClickMenu),NULL,this);
}

BatchImportDialog::~BatchImportDialog() {
	list_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(BatchImportDialog::OnSelectRightClickMenu),NULL,this);
}

int BatchImportDialog::ShowModal(int type, SaveSet* datas, bool isjapan) {
	datatype = type;
	dataset = datas;
	japanversion = isjapan;
	datamodified = false;
	if (type!=1 || GetGameType()!=GAME_TYPE_PSX) {
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
		wxFileName tmpname = wxFileName(fnamestr+wxString::Format(wxT("_%u.txt"),i+1));
		while (tmpname.IsFileReadable()) {
			m_importlist->Append(tmpname.GetFullName());
			m_importlist->Check(i);
			i++;
			tmpname = wxFileName(fnamestr+wxString::Format(wxT("_%u.txt"),i+1));
		}
	}
}

void BatchImportDialog::OnListRightClick(wxMouseEvent& event) {
	int newsel = m_importlist->HitTest(event.GetPosition());
	if (newsel!=wxNOT_FOUND) {
		m_importlist->SetSelection(newsel);
		m_importlist->PopupMenu(list_popup_menu);
	}
}

void BatchImportDialog::OnSelectRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id==wxID_SELECTALL) {
		for (i=0;i<m_importlist->GetCount();i++)
			m_importlist->Check(i,true);
	} else if (id==wxID_CLEAR) {
		for (i=0;i<m_importlist->GetCount();i++)
			m_importlist->Check(i,false);
	}
}

void BatchImportDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	bool ret = true;
	if (id==wxID_OK) {
		for (i=0;i<m_importlist->GetCount();i++) {
			if (m_importlist->IsChecked(i)) {
				wxString fname = m_filepicker->GetPath();
				if (!wxFileName(fname).IsFileReadable())
					fname = fname+wxString::Format(wxT("_%u.txt"),i+1);
				wxFile input(fname);
				wxString filestr = _(L"");
				if (input.IsOpened())
					input.ReadAll(&filestr);
				switch (datatype) {
				case 1: {
					LogStruct log = ImportText(filestr,m_adjustsize->IsChecked(),japanversion,m_fatalwarning->IsChecked());
					LogDialog dial(this,log);
					dial.SetTitle(dial.GetTitle()+_(L" : ")+wxFileName(fname).GetName());
					dial.ShowModal();
					datamodified = datamodified || log.ok;
					if (log.ok)
						m_importlist->Check(i,false);
					else
						ret = false;
					break;
				}
				case 2: {
					LogStruct log = ImportSpecialText(filestr,m_fatalwarning->IsChecked());
					LogDialog dial(this,log);
					dial.SetTitle(dial.GetTitle()+_(L" : ")+wxFileName(fname).GetName());
					dial.ShowModal();
					datamodified = datamodified || log.ok;
					if (log.ok)
						m_importlist->Check(i,false);
					else
						ret = false;
					break;
				}
				case 3:
					// ToDo
					break;
				case 4:
					break;
				case 5:
					break;
				}
			}
		}
	}
	if (ret)
		EndModal(id);
}
