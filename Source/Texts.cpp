#include "Texts.h"

#include <vector>
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "main.h"

#define TEXT_LOCALIZATION_HWS_FIELDS		0xFD
#define TEXT_LOCALIZATION_HWS_WHOLE_FILE	0xFE

//=============================//
//        Text Struct          //
//=============================//

int TextDataStruct::AddText(int id, FF9String& value) {
	uint16_t reqlen;
	if (GetGameType() == GAME_TYPE_PSX) {
		reqlen = 4 + value.length;
		if (has_format)
			reqlen += 4;
	} else {
		reqlen = value.length;
	}
	if (GetExtraSize() < reqlen)
		return 1;
	SetSize(size + reqlen);
	int insertindex = -1;
	if (id < 0) {
		for (unsigned int i = 0; i < text.size(); i++)
			id = max(id, text[i].id + 1);
	} else {
		insertindex = GetTextIndexById(id);
		if (insertindex >= 0)
			for (unsigned int i = 0; i < text.size(); i++)
				if (text[i].id >= id)
					text[i].id++;
	}
	TextDataEntry newentry;
	newentry.txt = value;
	newentry.id = id;
	if (GetGameType() == GAME_TYPE_PSX) {
		newentry.offset = 0;
		newentry.size_y = 0;
		newentry.flag = 0;
		if (has_format) {
			newentry.format_offset = 0;
			newentry.size_x = 0;
			newentry.format_amount = 0;
			newentry.format_data.clear();
		}
	}
	if (insertindex < 0)
		text.push_back(newentry);
	else
		text.insert(text.begin() + insertindex, newentry);
	return 0;
}

void TextDataStruct::RemoveText(int index) {
	uint16_t usedlen;
	text.erase(text.begin() + index);
	if (GetGameType() == GAME_TYPE_PSX) {
		usedlen = 4 + text[index].txt.length;
		if (has_format) {
			usedlen += 4;
			for (unsigned int i = 0; i < text[index].format_amount; i++)
				usedlen += text[index].format_data[i].length;
		}
	} else {
		usedlen = text[index].txt.length;
	}
	SetSize(size - usedlen);
}

int TextDataStruct::SetText(int index, wstring newvalue, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(text[index].txt);
		tmp.SetValue(newvalue, lang);
		uint16_t oldlen = text[index].txt.length;
		uint16_t newlen = tmp.length;
		if (newlen > GetExtraSize() + oldlen)
			return 1;
		SetSize(size + newlen - oldlen);
		text[index].txt.SetValue(newvalue, lang);
	}
	text[index].txt.SetValue(newvalue, lang);
	return 0;
}

int TextDataStruct::SetText(int index, FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		uint16_t oldlen = text[index].txt.length;
		uint16_t newlen = newvalue.length;
		if (newlen > GetExtraSize() + oldlen)
			return 1;
		SetSize(size + newlen - oldlen);
	}
	text[index].txt = FF9String(newvalue);
	return 0;
}

int TextDataStruct::AddFormatCode(int index, uint8_t opcode, uint8_t* arg) {
	unsigned int neededsize = 2 + HADES_STRING_TEXT_OPCODE[opcode].length;
	if (neededsize > GetExtraSize())
		return 1;
	TextFormatStruct newformat;
	newformat.length = neededsize;
	newformat.opcode = opcode;
	newformat.data.resize(neededsize);
	newformat.data[0] = OPCODE_CHAR;
	newformat.data[1] = opcode;
	memcpy(newformat.data.data() + 2, arg, HADES_STRING_TEXT_OPCODE[opcode].length * sizeof(uint8_t));
	text[index].format_data.push_back(newformat);
	text[index].format_amount++;
	return 0;
}

int TextDataStruct::RemoveFormatCode(int index, uint8_t formatid) {
	text[index].format_data.erase(text[index].format_data.begin() + formatid);
	text[index].format_amount--;
	return 0;
}

void TextDataEntry::SetDialogBoxSize(uint16_t x, uint16_t y, bool changey) {
	if (GetGameType() == GAME_TYPE_PSX) {
		size_x = (size_x & 0xF000) + x;
		if (changey)
			size_y = (size_y & 0x80) + y;
	}
}

#define MACRO_TEXT_IOFUNCTION(IO,SEEK,READ,PPF) \
	uint32_t strpos; \
	uint16_t amount = text.size(), zero16 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f, amount); \
	IO ## Short(f, zero16); \
	if (PPF) PPFEndScanStep(); \
	strpos = f.tellg(); \
	if (READ) { \
		text.resize(amount); \
		uint16_t firstoffset; \
		IO ## Short(f, firstoffset); \
		has_format = firstoffset > amount * 4; \
	} \
	SEEK(f,strpos, 0); \
	if (PPF) PPFInitScanStep(f); \
	for (i = 0; i < amount; i++) { \
		if (has_format) { \
			IO ## Short(f, text[i].format_offset); \
			IO ## Short(f, text[i].size_x); \
		} \
		IO ## Short(f, text[i].offset); \
		IO ## Char(f, text[i].size_y); \
		IO ## Char(f, text[i].flag); \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i = 0; i < amount; i++) { \
		if (has_format) { \
			SEEK(f, strpos, text[i].format_offset); \
			if (READ) { \
				unsigned int formatamount = 0, formatpos = 0; \
				uint8_t formatbyte, formatcode; \
				while ((int)formatpos < text[i].offset - text[i].format_offset) { \
					text[i].format_data.resize(formatamount + 1); \
					vector<uint8_t>& fdata = text[i].format_data[formatamount].data; \
					IO ## Char(f, formatbyte); fdata.push_back(formatbyte); \
					IO ## Char(f, formatcode); fdata.push_back(formatcode); \
					text[i].format_data[formatamount].opcode = formatcode; \
					if (HADES_STRING_TEXT_OPCODE[formatcode].length >= 0) { \
						text[i].format_data[formatamount].length = HADES_STRING_TEXT_OPCODE[formatcode].length + 2; \
						fdata.resize(text[i].format_data[formatamount].length); \
						for (j = 2; j < text[i].format_data[formatamount].length; j++) \
							IO ## Char(f, fdata[j]); \
					} else if (formatcode == 0x0D) { \
						unsigned int fchari = 0; \
						size_t argpos = f.tellg(); \
						IO ## Char(f, formatbyte); fdata.push_back(formatbyte); \
						IO ## Char(f, formatbyte); fdata.push_back(formatbyte); \
						IO ## Char(f, formatbyte); fdata.push_back(formatbyte); \
						while (fdata[fchari] != 0xFF || fdata[fchari + 1] != 0xFF || fdata[fchari + 2] != 0xFF) { \
							IO ## Char(f, formatbyte); fdata.push_back(formatbyte); \
							fchari++; \
						} \
						text[i].format_data[formatamount].length = fchari + 5; \
						SEEK(f, argpos, 0); \
					} \
					formatpos += text[i].format_data[formatamount].length; \
					formatamount++; \
				} \
				text[i].format_amount = formatamount; \
			} else { \
				if (PPF) PPFInitScanStep(f); \
				for (j = 0; j < text[i].format_amount; j++) \
					for (k = 0; k < text[i].format_data[j].length; k++) \
						IO ## Char(f, text[i].format_data[j].data[k]); \
				if (PPF) PPFEndScanStep(); \
			} \
		} \
		SEEK(f, strpos, text[i].offset); \
		if (PPF) PPFInitScanStep(f, true, text[i].txt.length); \
		if (READ && extended_charmap) text[i].txt.charmap_Ext = extended_charmap; \
		IO ## FF9String(f, text[i].txt); \
		if (PPF) PPFEndScanStep(); \
	}


void TextDataStruct::Read(fstream& f) {
	unsigned int i, j, k;
	if (loaded)
		return;
	if (parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP) >= 0) {
		uint16_t chmapid = parent_cluster->chunk[parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)].object_id[0];
		extended_charmap = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(chmapid);
	} else if (parent_cluster->clus_type == CLUSTER_TYPE_ENEMY)
		extended_charmap = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(0);
	else
		extended_charmap = NULL;
	if (size > 0) {
		MACRO_TEXT_IOFUNCTION(FFIXRead, FFIXSeek, true, false)
	} else {
		text.clear();
		has_format = false;
	}
	loaded = true;
}

void TextDataStruct::Write(fstream& f) {
	unsigned int i, j, k;
	if (size > 0) {
		MACRO_TEXT_IOFUNCTION(FFIXWrite, FFIXSeek, false, false)
	}
	modified = false;
}

void TextDataStruct::WritePPF(fstream& f) {
	unsigned int i, j, k;
	if (size > 0) {
		MACRO_TEXT_IOFUNCTION(PPFStepAdd, FFIXSeek, false, true)
	}
}

void TextDataStruct::ReadHWS(fstream& f, bool multilang) {
	uint16_t version = 0;
	unsigned int i, j, k;
	if (!multilang) {
		if (size > 0) {
			MACRO_TEXT_IOFUNCTION(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < amount; i++)
					text[i].txt.PSXToSteam();
		}
	} else {
		vector<int> indexes;
		uint16_t newamount;
		HWSReadShort(f, newamount);
		indexes.resize(newamount);
		if (GetHWSGlobalVersion() >= 100) {
			int newtextid;
			for (i = 0; i < newamount; i++) {
				HWSReadFlexibleChar(f, newtextid, true);
				indexes[i] = -1;
				for (j = 0; j < text.size(); j++) {
					if (newtextid == text[j].id) {
						indexes[i] = j;
						break;
					}
				}
				if (indexes[i] < 0) {
					FF9String newstr;
					newstr.CreateEmpty();
					if (AddText(newtextid, newstr) == 0)
						indexes[i] = GetTextIndexById(newtextid);
				}
			}
		} else {
			for (i = 0; i < newamount; i++)
				indexes[i] = i;
			if (newamount > text.size()) {
				unsigned int prevsize = text.size();
				text.resize(newamount);
				for (i = prevsize; i < newamount; i++)
					text[i].id = i;
			}
		}
		uint32_t txtdatasize, txtdatatotalsize;
		SteamLanguage lang;
		HWSReadChar(f, lang);
		while (lang != STEAM_LANGUAGE_NONE) {
			HWSReadLong(f, txtdatatotalsize);
			if ((GetGameType() == GAME_TYPE_PSX || hades::STEAM_SINGLE_LANGUAGE_MODE) && lang != GetSteamLanguage()) {
				f.seekg(txtdatatotalsize, ios::cur);
				HWSReadChar(f, lang);
				continue;
			}
			txtdatasize = 0;
			for (i = 0; i < newamount && txtdatasize < txtdatatotalsize; i++) {
				if (indexes[i] < 0) {
					FF9String dummystr;
					SteamReadFF9String(f, dummystr, lang);
					txtdatasize += dummystr.GetLength(lang);
				} else {
					SteamReadFF9String(f, text[indexes[i]].txt, lang);
					if (GetGameType() == GAME_TYPE_PSX)
						text[indexes[i]].txt.SteamToPSX();
					txtdatasize += text[indexes[i]].txt.GetLength(lang);
				}
			}
			HWSReadChar(f, lang);
		}
	}
	UpdateOffset();
	MarkDataModified();
}

void TextDataStruct::WriteHWS(fstream& f, bool multilang) {
	unsigned int i, j, k;
	if (GetGameType() == GAME_TYPE_PSX) {
		if (size > 0) {
			MACRO_TEXT_IOFUNCTION(HWSWrite, HWSSeek, false, false)
		}
	} else {
		if (multilang) {
			SteamLanguage lang;
			HWSWriteShort(f, text.size());
			for (i = 0; i < text.size(); i++)
				HWSWriteFlexibleChar(f, text[i].id, true);
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
				if (hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
					HWSWriteChar(f, lang);
					HWSWriteLong(f, GetDataSize(lang));
					for (i = 0; i < text.size(); i++)
						SteamWriteFF9String(f, text[i].txt, lang);
				}
			HWSWriteChar(f, STEAM_LANGUAGE_NONE);
		} else {
			HWSWriteShort(f, (uint16_t)text.size());
			for (i = 0; i < text.size(); i++)
				SteamWriteFF9String(f, text[i].txt);
		}
	}
}

void TextDataStruct::WriteSteamPatch(fstream& f, wxArrayString basefile, SteamLanguage lang) {
	int lastid = -1;
	for (unsigned int i = 0; i < text.size(); i++) {
		if (!text[i].txt.multi_lang_init[lang])
			continue;
		wxString str = lang == GetSteamLanguage() ? _(text[i].txt.str) : _(text[i].txt.multi_lang_str[lang]);
		if (text[i].id < (int)basefile.GetCount() && basefile[text[i].id].IsSameAs(str))
			continue;
		if (text[i].id >= (int)basefile.GetCount() && (str.IsEmpty() || str.IsSameAs(L"[STRT=0,1]")))
			continue;
		if (text[i].id != lastid + 1)
			SteamWriteFF9StringLight(f, L"[TXID=" + to_wstring(text[i].id) + L"]");
		SteamWriteFF9String(f, text[i].txt, lang);
		lastid = text[i].id;
	}
}

void TextDataStruct::WriteSteam(fstream& f, SteamLanguage lang) {
	for (unsigned int i = 0; i < text.size(); i++)
		SteamWriteFF9String(f, text[i].txt, lang);
}

int TextDataStruct::GetDataSize(SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX)
		return size;
	int res = 0;
	for (unsigned int i = 0; i < text.size(); i++)
		res += text[i].txt.GetLength(lang);
	return res;
}

void TextDataStruct::UpdateOffset() {
	if (size == 0 && GetGameType() == GAME_TYPE_PSX)
		return;
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	unsigned int i, j, k;
	j = has_format ? text.size() * 8 : text.size() * 4;
	if (has_format) {
		for (i = 0; i < text.size(); i++) {
			text[i].format_offset = j;
			for (k = 0; k < text[i].format_amount; k++)
				j += text[i].format_data[k].length;
			text[i].offset = j;
			j += text[i].txt.length;
		}
	} else {
		for (i = 0; i < text.size(); i++) {
			text[i].offset = j;
			j += text[i].txt.length;
		}
	}
	if (j % 4)
		j += 4 - j % 4;
	SetSize(j + 4);
}

int TextDataStruct::GetTextIndexById(int id) {
	if (GetGameType() == GAME_TYPE_PSX || id < (int)base_amount) {
		if (id >= 0 && id < (int)text.size())
			return id;
	} else {
		for (unsigned int i = base_amount; i < text.size(); i++)
			if (text[i].id == id)
				return i;
	}
	return -1;
}

FF9String dummytext;
FF9String& TextDataStruct::GetTextById(int id) {
	int index = GetTextIndexById(id);
	if (index >= 0)
		return text[index].txt;
	dummytext.CreateEmpty();
	dummytext.SetValue(L"[Invalid Text]");
	return dummytext;
}

wstring TextDataStruct::GetTextByFullListIndex(int index, int strtype) {
	if (index < 0 || index > (int)text.size())
		return L"[Invalid Text]";
	if (GetGameType() != GAME_TYPE_PSX && index < (int)base_amount) {
		int localindex = GetTextIdFromUniversalId(GetSteamLanguage(), block_id, index);
		if (localindex >= 0)
			return text[localindex].txt.GetStr(strtype);
		else
			return HADES_STRING_TEXT_INVALID_LANG;
	}
	return text[index].txt.GetStr(strtype);
}

FF9String TextDataStruct::GetTextByFullListIndex(int index) {
	if (GetGameType() != GAME_TYPE_PSX && index < (int)base_amount) {
		FF9String matchup;
		matchup.CreateEmpty();
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			int localindex = GetTextIdFromUniversalId(lang, block_id, index);
			if (localindex >= 0 && text[localindex].txt.multi_lang_init[lang])
				matchup.SetValue(lang == GetSteamLanguage() ? text[localindex].txt.str : text[localindex].txt.multi_lang_str[lang], lang);
		}
		return matchup;
	}
	return text[index].txt;
}

void TextDataSet::Load(fstream& ffbin, ClusterSet& clusset) {
	unsigned int i, j, k, l;
	amount = clusset.text_amount;
	struct_id.resize(amount);
	name.resize(amount);
	text_data.resize(amount);
	tim_amount.resize(amount);
	j = 0;
	LoadingDialogInit(amount, _(L"Reading text blocks..."));
	if (GetGameType() == GAME_TYPE_PSX) {
		cluster_id.resize(amount);
		charmap.resize(amount);
		chartim.resize(amount);
		uint16_t numchmap = 0;
		for (i = 0; i < clusset.amount; i++) {
			if (clusset.clus_type[i] == CLUSTER_TYPE_FIELD_TEXT) {
				ClusterData& clus = clusset.clus[i];
				cluster_id[j] = i;
				clus.CreateChildren(ffbin);
				for (k = 0; k < clus.chunk_amount; k++) {
					for (l = 0; l < clus.chunk[k].object_amount; l++) {
						ffbin.seekg(clus.chunk[k].object_offset[l]);
						clus.chunk[k].GetObject(l).Read(ffbin);
					}
				}
				if (clus.SearchChunkType(CHUNK_TYPE_TEXT) >= 0) {
					ChunkData& chunktxt = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_TEXT)];
					text_data[j] = (TextDataStruct*)&chunktxt.GetObject(0);
					for (k = 0; k < text_data[j]->text.size(); k++)
						text_data[j]->text[k].id = k;
					struct_id[j] = chunktxt.object_id[0];
					text_data[j]->block_id = struct_id[j];
				} else {
					text_data[j] = NULL;
					struct_id[j] = 0x1000 + numchmap++;
					main_charmap_index = j;
				}
				if (clus.SearchChunkType(CHUNK_TYPE_CHARMAP) >= 0) {
					ChunkData& chmap = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_CHARMAP)];
					charmap[j] = (CharmapDataStruct*)&chmap.GetObject(0);
					ChunkData& chtim = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_TIM)];
					chartim[j] = (TIMImageDataStruct*)&chtim.GetObject(0);
					tim_amount[j] = chtim.object_amount;
				} else {
					tim_amount[j] = 0;
					charmap[j] = NULL;
					chartim[j] = NULL;
				}
				if (auto search = HADES_STRING_TEXT_BLOCK_NAME.find(struct_id[j]); search != HADES_STRING_TEXT_BLOCK_NAME.end())
					name[j] = search->second.label;
				j++;
				LoadingDialogUpdate(j);
			}
		}
	} else {
		ConfigurationSet& config = *clusset.config;
		string fname = config.steam_dir_data;
		uint32_t fsize;
		char* buffer;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (i = 0; i < amount; i++) {
			ClusterData* dummyclus;
			SteamLanguage lang;
			unsigned int text_lang_amount[STEAM_LANGUAGE_AMOUNT];
			unsigned int textamount = 0;
			tim_amount[i] = 0;
			struct_id[i] = config.text_id[i];
			text_data[i] = new TextDataStruct[1];
			text_data[i]->Init(true, CHUNK_TYPE_TEXT, config.text_id[i], &dummyclus, CLUSTER_TYPE_FIELD_TEXT);
			text_data[i]->block_id = struct_id[i];
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.text_file[lang][i]));
				fsize = config.meta_res.GetFileSizeByIndex(config.text_file[lang][i]);
				buffer = new char[fsize];
				ffbin.read(buffer, fsize);
				text_lang_amount[lang] = FF9String::CountSteamTextAmount(buffer, fsize);
				textamount = max(textamount, text_lang_amount[lang]);
				delete[] buffer;
			}
			text_data[i]->base_amount = 0;
			if (auto search = HADES_STRING_TEXT_BLOCK_NAME.find(struct_id[i]); search != HADES_STRING_TEXT_BLOCK_NAME.end()) {
				text_data[i]->base_amount = search->second.base_text_count;
				name[i] = search->second.label;
			}
			textamount = max(textamount, text_data[i]->base_amount);
			text_data[i]->text.resize(textamount);
			for (j = 0; j < textamount; j++)
				text_data[i]->text[j].id = j;
			text_data[i]->loaded = true;
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.text_file[lang][i]));
				text_data[i]->size = config.meta_res.GetFileSizeByIndex(config.text_file[lang][i]);
				for (j = 0; j < text_lang_amount[lang]; j++)
					SteamReadFF9String(ffbin, text_data[i]->text[j].txt, lang);
				while (j < textamount) // Now, all the texts are initialised in all languages (except in STEAM_SINGLE_LANGUAGE_MODE)
					text_data[i]->text[j++].txt.SetValue(L"[STRT=0,1]", lang);
			}
			LoadingDialogUpdate(i);
		}
		ffbin.close();
	}
	LoadingDialogEnd();
}

void TextDataSet::Write(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i = 0; i < amount; i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].Write(ffbin);
	}
}

void TextDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i = 0; i < amount; i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].WritePPF(ffbin);
	}
}

int* TextDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i, j, k;
	uint32_t chunksize, clustersize, chunkpos, objectpos, objectsize;
	uint16_t nbmodified, objectid, fileobjectid;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[3];
	res[0] = 0; res[1] = 0; res[2] = 0;
	HWSReadShort(ffhws, nbmodified);
	for (i = 0; i < nbmodified; i++) {
		objectpos = ffhws.tellg();
		HWSReadShort(ffhws, objectid);
		HWSReadLong(ffhws, clustersize);
		for (j = 0; j < amount; j++) {
			fileobjectid = struct_id[j];
			if (text_data[j])
				clus = text_data[j]->parent_cluster;
			else
				clus = charmap[j]->parent_cluster;
			if (objectid == fileobjectid) {
				if (clustersize <= clus->size + clus->extra_size) {
					HWSReadChar(ffhws, chunktype);
					while (chunktype != 0xFF) {
						HWSReadLong(ffhws, chunksize);
						chunkpos = ffhws.tellg();
						if (chunktype == CHUNK_TYPE_TEXT && text_data[j]) {
							text_data[j]->ReadHWS(ffhws);
							text_data[j]->SetSize(chunksize - (GetHWSGameType() == GAME_TYPE_PSX ? 0 : 2));
						} else if (chunktype == CHUNK_TYPE_CHARMAP && GetGameType() == GAME_TYPE_PSX && charmap[j]) {
							charmap[j]->ReadHWS(ffhws);
							charmap[j]->SetSize(chunksize);
						} else if (chunktype == CHUNK_TYPE_TIM && GetGameType() == GAME_TYPE_PSX && chartim[j]) {
							uint16_t timid;
							HWSReadShort(ffhws, timid);
							for (k = 0; k < chartim[j]->parent_chunk->object_amount; k++)
								if (chartim[j][k].object_id == timid) {
									chartim[j][k].ReadHWS(ffhws);
									chartim[j][k].SetSize(chunksize - 2);
								}
						} else if (chunktype == CHUNK_STEAM_TEXT_MULTILANG) {
							text_data[j]->ReadHWS(ffhws, true);
						} else
							res[1]++;
						ffhws.seekg(chunkpos + chunksize);
						HWSReadChar(ffhws, chunktype);
					}
				} else {
					objectsize = 7;
					HWSReadChar(ffhws, chunktype);
					while (chunktype != 0xFF) {
						HWSReadLong(ffhws, chunksize);
						ffhws.seekg(chunksize, ios::cur);
						HWSReadChar(ffhws, chunktype);
						objectsize += chunksize + 5;
					}
					ffhws.seekg(objectpos);
					backup.Add(ffhws, objectsize);
					res[0]++;
				}
				j = amount;
			} else if (j + 1 == amount) {
				objectsize = 7;
				HWSReadChar(ffhws, chunktype);
				while (chunktype != 0xFF) {
					HWSReadLong(ffhws, chunksize);
					ffhws.seekg(chunksize, ios::cur);
					HWSReadChar(ffhws, chunktype);
					objectsize += chunksize + 5;
				}
				ffhws.seekg(objectpos);
				backup.Add(ffhws, objectsize);
				res[2]++;
			}
		}
	}
	return res;
}

void TextDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i, j;
	uint16_t fileobjectid, nbmodified = 0;
	uint32_t chunkpos, chunksize, nboffset = ffhws.tellg();
	ClusterData* clus;
	HWSWriteShort(ffhws, nbmodified);
	for (i = 0; i < amount; i++) {
		fileobjectid = struct_id[i];
		if (text_data[i])
			clus = text_data[i]->parent_cluster;
		else
			clus = charmap[i]->parent_cluster;
		if (clus->modified) {
			clus->UpdateOffset();
			HWSWriteShort(ffhws, fileobjectid);
			HWSWriteLong(ffhws, clus->size);
			if (GetGameType() == GAME_TYPE_PSX) {
				if (text_data[i] && text_data[i]->modified) {
					HWSWriteChar(ffhws, CHUNK_TYPE_TEXT);
					HWSWriteLong(ffhws, text_data[i]->size);
					chunkpos = ffhws.tellg();
					text_data[i]->WriteHWS(ffhws);
					ffhws.seekg(chunkpos + text_data[i]->size);
				}
				if (charmap[i] && charmap[i]->modified) {
					HWSWriteChar(ffhws, CHUNK_TYPE_CHARMAP);
					HWSWriteLong(ffhws, charmap[i]->size);
					chunkpos = ffhws.tellg();
					charmap[i]->WriteHWS(ffhws);
					ffhws.seekg(chunkpos + charmap[i]->size);
				}
				if (chartim[i]) {
					for (j = 0; j < chartim[i]->parent_chunk->object_amount; j++)
						if (chartim[i][j].modified) {
							HWSWriteChar(ffhws, CHUNK_TYPE_TIM);
							HWSWriteLong(ffhws, chartim[i][j].size + 2);
							chunkpos = ffhws.tellg();
							HWSWriteShort(ffhws, chartim[i][j].object_id);
							chartim[i][j].WriteHWS(ffhws);
							ffhws.seekg(chunkpos + chartim[i][j].size + 2);
						}
				}
			} else {
				if (text_data[i] && text_data[i]->modified) {
					HWSWriteChar(ffhws, CHUNK_STEAM_TEXT_MULTILANG);
					HWSWriteLong(ffhws, 0);
					chunkpos = ffhws.tellg();
					text_data[i]->WriteHWS(ffhws, true);
					chunksize = (uint32_t)ffhws.tellg() - chunkpos;
					ffhws.seekg(chunkpos - 4);
					HWSWriteLong(ffhws, chunksize);
					ffhws.seekg(chunkpos + chunksize);
				}
			}
			HWSWriteChar(ffhws, 0xFF);
			nbmodified++;
		}
	}
	for (i = 0; i < backup.save_amount; i++)
		for (j = 0; j < backup.save_size[i]; j++)
			HWSWriteChar(ffhws, backup.save_data[i][j]);
	nbmodified += backup.save_amount;
	uint32_t endoffset = ffhws.tellg();
	ffhws.seekg(nboffset);
	HWSWriteShort(ffhws, nbmodified);
	ffhws.seekg(endoffset);
}

int TextDataSet::GetIndexById(uint16_t blockid) {
	for (unsigned int i = 0; i < amount; i++)
		if (blockid == struct_id[i])
			return i;
	return -1;
}

//=============================//
//     Special Text Struct     //
//=============================//

// Eg.:
// "This entry has a quote ("") \n and a comma (,)"
// ->
// This entry has a quote (") 
//  and a comma (,)
wstring SpecialTextDataStruct::LocalizationRawToNice(wstring entry) {
	wstring nicestr = L"";
	unsigned int i = 0;
	unsigned int len = entry.size();
	if (len >= 2 && entry[0] == '\"' && entry[len - 1] == '\"') {
		i++;
		len--;
	}
	for (; i < len; i++) {
		if (i + 1 < len && entry[i] == L'\"' && entry[i + 1] == L'\"') {
			nicestr += L"\"";
			i++;
		} else if (i + 1 < len && entry[i] == L'\\' && entry[i + 1] == L'n') {
			nicestr += L"\n";
			i++;
		} else {
			nicestr += entry[i];
		}
	}
	return nicestr;
}

// Eg.:
// This entry has a quote (") 
//  and a comma (,)
// ->
// "This entry has a quote ("") \n and a comma (,)"
wstring SpecialTextDataStruct::LocalizationNiceToRaw(wstring entry) {
	wstring rawstr = L"";
	bool addquotes = false;
	for (unsigned int i = 0; i < entry.size(); i++) {
		if (entry[i] == 0xD) {
		} else if (entry[i] == 0xA) {
			rawstr += L"\\n";
		} else if (entry[i] == L'\"') {
			rawstr += L"\"\"";
			addquotes = true;
		} else if (entry[i] == L',') {
			rawstr += L",";
			addquotes = true;
		} else {
			rawstr += entry[i];
		}
	}
	if (addquotes)
		return L"\"" + rawstr + L"\"";
	return rawstr;
}

int SpecialTextDataStruct::AddText(int id, FF9String& value) {
	SpecialTextDataEntry newentry;
	if (GetGameType() == GAME_TYPE_PSX) {
		uint16_t reqlen = GetGameType() == GAME_TYPE_PSX ? 4 + value.length : value.length;
		if (space_total < space_used + reqlen)
			return 1;
		space_used += reqlen;
		newentry.offset = 0;
		newentry.size_x = 0;
	}
	int insertindex = -1;
	if (id < 0) {
		for (unsigned int i = 0; i < text.size(); i++)
			id = max(id, text[i].id + 1);
	} else {
		insertindex = GetTextIndexById(id);
		if (insertindex >= 0)
			for (unsigned int i = 0; i < text.size(); i++)
				if (text[i].id >= id)
					text[i].id++;
	}
	newentry.id = id;
	newentry.txt = value;
	newentry.localization_field = is_localization ? L"CustomField" + to_string(id) : L"";
	if (insertindex < 0)
		text.push_back(newentry);
	else
		text.insert(text.begin() + insertindex, newentry);
	return 0;
}

int SpecialTextDataStruct::AddTextByKey(wstring key, FF9String& value) {
	if (GetGameType() == GAME_TYPE_PSX)
		return 1;
	int id = 0;
	for (unsigned int i = 0; i < text.size(); i++)
		id = max(id, text[i].id + 1);
	SpecialTextDataEntry newentry;
	newentry.id = id;
	newentry.txt = value;
	newentry.localization_field = key;
	text.push_back(newentry);
	return 0;
}

void SpecialTextDataStruct::RemoveText(int index) {
	if (GetGameType() == GAME_TYPE_PSX)
		space_used -= text[index].txt.length;
	text.erase(text.begin() + index);
}

int SpecialTextDataStruct::SetText(int index, wstring newvalue, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(text[index].txt);
		tmp.SetValue(newvalue);
		uint16_t oldlen = text[index].txt.length;
		uint16_t newlen = tmp.length;
		if (space_used + newlen > space_total + oldlen)
			return 1;
		space_used += newlen - oldlen;
	}
	text[index].txt.SetValue(newvalue, lang);
	parent->modified = true;
	return 0;
}

int SpecialTextDataStruct::SetText(int index, FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		uint16_t oldlen = text[index].txt.length;
		uint16_t newlen = newvalue.length;
		if (space_used + newlen > space_total + oldlen)
			return 1;
		space_used += newlen - oldlen;
	}
	text[index].txt = FF9String(newvalue);
	parent->modified = true;
	return 0;
}

int SpecialTextDataStruct::GetHWSDataSize(SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX)
		return space_used;
	unsigned int i;
	int res = 0;
	if (is_localization) {
		for (i = 0; i < text.size(); i++) {
			if (text[i].localization_field.size() == 0)
				continue;
			if (lang == STEAM_LANGUAGE_NONE)
				res += _(LocalizationNiceToRaw(text[i].localization_field)).mb_str(wxConvUTF8).length() + 1;
			else if (lang == GetSteamLanguage())
				res += _(LocalizationNiceToRaw(text[i].txt.str)).mb_str(wxConvUTF8).length() + 1;
			else
				res += _(LocalizationNiceToRaw(text[i].txt.multi_lang_str[lang])).mb_str(wxConvUTF8).length() + 1;
		}
	} else {
		for (i = 0; i < text.size(); i++)
			res += text[i].txt.GetLength(lang);
	}
	return res;
}

int SpecialTextDataStruct::GetDataSize(SteamLanguage lang) {
	if (!is_localization)
		return GetHWSDataSize(lang);
	int res = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		if (text[i].localization_field.size() == 0)
			continue;
		res += _(LocalizationNiceToRaw(text[i].localization_field)).mb_str(wxConvUTF8).length() + STEAM_LANGUAGE_AMOUNT + 1;
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (lang == GetSteamLanguage())
				res += _(LocalizationNiceToRaw(text[i].txt.str)).mb_str(wxConvUTF8).length();
			else
				res += _(LocalizationNiceToRaw(text[i].txt.multi_lang_str[lang])).mb_str(wxConvUTF8).length();
		}
	}
	return res;
}

void SpecialTextDataStruct::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	space_used = text.size() * 4;
	for (unsigned int i = 0; i < text.size(); i++) {
		text[i].offset = space_used;
		space_used += text[i].txt.length;
	}
}

int SpecialTextDataStruct::GetTextIndexById(int id) {
	if (is_localization)
		return -1;
	for (unsigned int i = 0; i < text.size(); i++)
		if (text[i].id == id)
			return i;
	return -1;
}

FF9String& SpecialTextDataStruct::GetTextById(int id) {
	int index = GetTextIndexById(id);
	if (index >= 0)
		return text[index].txt;
	dummytext.CreateEmpty();
	dummytext.SetValue(L"[Invalid Text]");
	return dummytext;
}

int SpecialTextDataStruct::GetTextIndexByKey(wstring key) {
	if (!is_localization)
		return -1;
	for (unsigned int i = 0; i < text.size(); i++)
		if (text[i].localization_field.compare(key) == 0)
			return i;
	return -1;
}

FF9String& SpecialTextDataStruct::GetTextByKey(wstring key) {
	int index = GetTextIndexByKey(key);
	if (index >= 0)
		return text[index].txt;
	dummytext.CreateEmpty();
	dummytext.SetValue(L"[Invalid Text]");
	return dummytext;
}

#define MACRO_SPECIALTEXT_IOFUNCTION(IO,SEEK,READ,PPF) \
	uint32_t strpos, textamount = text_block[i].text.size(); \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Long(ffbin, textamount); \
	if (READ) text_block[i].text.resize(textamount); \
	strpos = ffbin.tellg(); \
	for (j = 0; j < textamount; j++) { \
		IO ## Short(ffbin, text_block[i].text[j].offset); \
		IO ## Short(ffbin, text_block[i].text[j].size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) text_block[i].space_used = 4 * textamount; \
	if (PPF) PPFInitScanStep(ffbin,true,text_block[i].space_total); \
	for (j = 0; j < textamount; j++) { \
		SEEK(ffbin, strpos, text_block[i].text[j].offset); \
		IO ## FF9String(ffbin, text_block[i].text[j].txt); \
		if (READ) text_block[i].space_used += text_block[i].text[j].txt.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin, strpos, text_block[i].space_total);


bool SetupLocalizationLanguageOrderByKey(wstring langkey, int& lang) {
	if (langkey.compare(L"English(US)") == 0)
		lang = STEAM_LANGUAGE_US;
	else if (langkey.compare(L"English(UK)") == 0)
		lang = STEAM_LANGUAGE_EN;
	else if (langkey.compare(L"Japanese") == 0)
		lang = STEAM_LANGUAGE_JA;
	else if (langkey.compare(L"Spanish") == 0)
		lang = STEAM_LANGUAGE_SP;
	else if (langkey.compare(L"French") == 0)
		lang = STEAM_LANGUAGE_FR;
	else if (langkey.compare(L"German") == 0)
		lang = STEAM_LANGUAGE_GE;
	else if (langkey.compare(L"Italian") == 0)
		lang = STEAM_LANGUAGE_IT;
	else
		return false;
	return true;
}

void SpecialTextDataStruct::LocalizationRead(fstream& ffbin, uint32_t spaceused) {
	uint32_t bufferpos = 0;
	vector<char> bufferstr;
	SteamLanguage lang;
	unsigned int i, j;
	char* buffer = new char[spaceused];
	int langorder[STEAM_LANGUAGE_AMOUNT];
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		langorder[lang] = lang;
	bufferstr.reserve(0x1000);
	ffbin.read(buffer, spaceused);
	for (i = 0; bufferpos < spaceused; i++) {
		int entryindex = -1;
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT + 1; lang++) {
			bufferstr.clear();
			if (buffer[bufferpos] == '"') {
				// Inside quotes: commas can be used, quotes are escaped as double-quotes and non-escaped line breaks might be used (although it's better to still escape them)
				bufferstr.push_back('"');
				bufferpos++;
				while (bufferpos < spaceused) {
					if (buffer[bufferpos] == '"') {
						if (bufferpos + 1 < spaceused && buffer[bufferpos + 1] == '"') {
							bufferstr.push_back('"');
							bufferstr.push_back('"');
							bufferpos++;
						} else {
							break;
						}
					} else if (buffer[bufferpos] == 0xD) {
					} else {
						bufferstr.push_back(buffer[bufferpos]);
					}
					bufferpos++;
				}
				bufferstr.push_back('"');
				bufferpos++;
			} else {
				while (bufferpos < spaceused && buffer[bufferpos] != ',' && buffer[bufferpos] != 0xD && buffer[bufferpos] != 0xA)
					bufferstr.push_back(buffer[bufferpos++]);
			}
			bufferstr.push_back(0);
			if (i == 0 && entryindex >= 0 && text[entryindex].localization_field.compare(L"KEY") == 0) {
				wstring langkey = LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data()));
				SetupLocalizationLanguageOrderByKey(langkey, langorder[lang - 1]);
				text[entryindex].txt.SetValue(langkey, langorder[lang - 1]);
			} else if (lang == 0) {
				wstring fieldstr = LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data()));
				if (!fieldstr.empty()) {
					for (j = 0; j < text.size(); j++) {
						if (text[j].localization_field.compare(fieldstr) == 0) {
							entryindex = j;
							break;
						}
					}
				}
				if (entryindex < 0) {
					entryindex = text.size();
					SpecialTextDataEntry newentry;
					newentry.txt.CreateEmpty();
					text.push_back(newentry);
				}
				text[entryindex].localization_field = fieldstr;
			} else {
				text[entryindex].txt.SetValue(LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data())), langorder[lang - 1]);
			}
			if (lang == STEAM_LANGUAGE_AMOUNT)
				while (bufferpos < spaceused && buffer[bufferpos] != 0xA)
					bufferpos++;
			if (buffer[bufferpos] == 0xD || buffer[bufferpos] == 0xA) {
				if (lang == 0)
					lang++;
				while (lang < STEAM_LANGUAGE_AMOUNT + 1) {
					if (!hades::STEAM_SINGLE_LANGUAGE_MODE && !text[entryindex].txt.multi_lang_init[langorder[lang - 1]])
						text[entryindex].txt.SetValue(L"", langorder[lang - 1]);
					lang++;
				}
			}
			if (buffer[bufferpos] == 0xA) {
				bufferpos++;
				while (bufferpos < spaceused && (buffer[bufferpos] == 0xD || buffer[bufferpos] == 0xA))
					bufferpos++;
			} else if (buffer[bufferpos] == ',') {
				bufferpos++;
			}
		}
	}
	delete[] buffer;
}

void SpecialTextDataSet::Load(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i, j;
	text_block.resize(configset.spetext_amount);
	if (GetGameType() == GAME_TYPE_PSX) {
		for (i = 0; i < text_block.size(); i++) {
			text_block[i].parent = this;
			text_block[i].space_total = configset.spetext_space_total[i];
			text_block[i].is_localization = false;
			ffbin.seekg(configset.spetext_offset[i]);
			MACRO_SPECIALTEXT_IOFUNCTION(FFIXRead, FFIXSeek, true, false)
			for (j = 0; j < text_block[i].text.size(); j++)
				text_block[i].text[j].id = j;
		}
	} else {
		SteamLanguage lang;
		for (i = 0; i < text_block.size(); i++) {
			text_block[i].parent = this;
			text_block[i].space_total = configset.spetext_steam_space_total[i];
		}
		string fname = configset.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);

		#define MACRO_READ_SPETEXT_STEAM(INDEX,TYPE) \
			text_block[INDEX].space_used = configset.meta_res.GetFileSizeByIndex(configset.spetext_ ## TYPE ## _file[GetSteamLanguage()]); \
			text_block[INDEX].is_localization = false; \
			text_block[INDEX].text.resize(configset.spetext_ ## TYPE ## _amount); \
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) { \
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage()) \
					continue; \
				ffbin.seekg(configset.meta_res.GetFileOffsetByIndex(configset.spetext_ ## TYPE ## _file[lang])); \
				for (j = 0; j < text_block[INDEX].text.size(); j++) { \
					text_block[INDEX].text[j].id = j; \
					SteamReadFF9String(ffbin, text_block[INDEX].text[j].txt, lang); \
				} \
			}

		text_block[0].base_amount = 39;
		text_block[1].base_amount = 22;
		text_block[2].base_amount = 8;
		text_block[3].base_amount = 82;
		text_block[4].base_amount = 32;
		text_block[5].base_amount = 53;
		text_block[6].base_amount = 493;
		MACRO_READ_SPETEXT_STEAM(0, battleinfo)
		MACRO_READ_SPETEXT_STEAM(1, battlescan)
		MACRO_READ_SPETEXT_STEAM(2, spellnaming)
		MACRO_READ_SPETEXT_STEAM(3, chocomenu)
		MACRO_READ_SPETEXT_STEAM(4, cardrank)
		MACRO_READ_SPETEXT_STEAM(5, tetramaster)
		for (j = 15; j < text_block[1].text.size(); j++) // "Weak against Ice." etc... missing in JP
			if (!hades::STEAM_SINGLE_LANGUAGE_MODE || STEAM_LANGUAGE_JA == GetSteamLanguage())
				text_block[1].text[j].txt.SetValue(L"", STEAM_LANGUAGE_JA);
		ffbin.seekg(configset.meta_res.GetFileOffsetByIndex(configset.spetext_localization_file));
		text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].is_localization = true;
		text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].space_used = configset.meta_res.GetFileSizeByIndex(configset.spetext_localization_file);
		text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].LocalizationRead(ffbin, text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].space_used);
		for (j = 0; j < text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].text.size(); j++)
			text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].text[j].id = j;
		ffbin.close();
	}
	for (i = 0; i < text_block.size(); i++)
		text_block[i].UpdateOffset();
	modified = false;
}

void SpecialTextDataSet::WriteSteamLocalizationPatch(fstream& fileout, fstream& baseresourcefile, ConfigurationSet& configset) {
	if (GetGameType() == GAME_TYPE_PSX)
		return;
	SpecialTextDataStruct& textblock = text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM];
	SpecialTextDataStruct baselocalization;
	bool keyentrywritten = false;
	SteamLanguage lang;
	unsigned int i, j;
	baseresourcefile.seekg(configset.meta_res.GetFileOffsetByIndex(configset.spetext_localization_file));
	baselocalization.is_localization = true;
	baselocalization.space_used = configset.meta_res.GetFileSizeByIndex(configset.spetext_localization_file);
	baselocalization.LocalizationRead(baseresourcefile, text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM].space_used);
	for (i = 0; i < textblock.text.size(); i++) {
		if (textblock.text[i].localization_field.size() == 0)
			continue;
		bool skipentry = false;
		for (j = 0; j < baselocalization.text.size(); j++) {
			if (baselocalization.text[j].localization_field.compare(textblock.text[i].localization_field) == 0) {
				skipentry = true;
				for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
					if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang])
						continue;
					wstring basestr = lang == GetSteamLanguage() ? baselocalization.text[j].txt.str : baselocalization.text[j].txt.multi_lang_str[lang];
					wstring str = lang == GetSteamLanguage() ? textblock.text[i].txt.str : textblock.text[i].txt.multi_lang_str[lang];
					if (basestr.compare(str) != 0) {
						skipentry = false;
						break;
					}
				}
				break;
			}
		}
		if (skipentry)
			continue;
		if (i == 0) // Assume the "KEY" entry is still the first one
			keyentrywritten = true;
		if (!keyentrywritten) {
			SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[0].localization_field));
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang])
					continue;
				fileout.put(',');
				if (lang == GetSteamLanguage())
					SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[0].txt.str));
				else
					SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[0].txt.multi_lang_str[lang]));
			}
			fileout.put(0xA);
		}
		SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].localization_field));
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang])
				continue;
			fileout.put(',');
			if (lang == GetSteamLanguage())
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].txt.str));
			else
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].txt.multi_lang_str[lang]));
		}
		fileout.put(0xA);
	}
}

void SpecialTextDataSet::WriteSteamPatch(fstream& fileout, unsigned int blockid, wxArrayString basefile, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX)
		return;
	SpecialTextDataStruct& textblock = text_block[blockid];
	int lastid = -1;
	for (unsigned int i = 0; i < textblock.text.size(); i++) {
		SpecialTextDataEntry& entry = textblock.text[i];
		if (!entry.txt.multi_lang_init[lang])
			continue;
		wxString str = lang == GetSteamLanguage() ? _(entry.txt.str) : _(entry.txt.multi_lang_str[lang]);
		if (lang == STEAM_LANGUAGE_JA && blockid == 1 && entry.id >= 15 && entry.id < (int)textblock.base_amount && str.IsEmpty())
			continue;
		if (entry.id >= (int)basefile.GetCount() || !basefile[entry.id].IsSameAs(str)) {
			if (entry.id != lastid + 1)
				SteamWriteFF9StringLight(fileout, L"[TXID=" + to_wstring(entry.id) + L"]");
			SteamWriteFF9String(fileout, entry.txt, lang);
			lastid = entry.id;
		}
	}
}

void SpecialTextDataSet::WriteSteamLocalization(fstream& fileout, bool alllang) {
	if (GetGameType() == GAME_TYPE_PSX)
		return;
	SpecialTextDataStruct& textblock = text_block[SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM];
	for (unsigned int i = 0; i < textblock.text.size(); i++) {
		if (textblock.text[i].localization_field.size() == 0)
			continue;
		SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].localization_field));
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (!alllang && !hades::STEAM_LANGUAGE_SAVE_LIST[lang])
				continue;
			fileout.put(',');
			if (lang == GetSteamLanguage())
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].txt.str));
			else
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(textblock.text[i].txt.multi_lang_str[lang]));
		}
		fileout.put(0xA);
	}
}

void SpecialTextDataSet::WriteSteam(fstream& fileout, unsigned int blockid, SteamLanguage lang) {
	if (text_block[blockid].is_localization) {
		WriteSteamLocalization(fileout, true);
	} else {
		for (unsigned int i = 0; i < text_block[blockid].text.size(); i++)
			SteamWriteFF9String(fileout, text_block[blockid].text[i].txt, lang);
	}
}

void SpecialTextDataSet::WriteHWSSteam(fstream& fileout, unsigned int blockid, SteamLanguage lang) {
	unsigned int i;
	if (text_block[blockid].is_localization) {
		for (i = 0; i < text_block[blockid].text.size(); i++) {
			if (lang == STEAM_LANGUAGE_NONE)
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(text_block[blockid].text[i].localization_field));
			else if (lang == GetSteamLanguage())
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(text_block[blockid].text[i].txt.str));
			else
				SteamWriteFF9StringLight(fileout, SpecialTextDataStruct::LocalizationNiceToRaw(text_block[blockid].text[i].txt.multi_lang_str[lang]));
			fileout.put(0); // Null-terminated strings, for allowing the presence or absence of [ENDN]
		}
	} else {
		for (i = 0; i < text_block[blockid].text.size(); i++)
			SteamWriteFF9String(fileout, text_block[blockid].text[i].txt, lang);
	}
}

void SpecialTextDataSet::Write(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i, j;
	for (i = 0; i < text_block.size(); i++) {
		text_block[i].UpdateOffset();
		ffbin.seekg(configset.spetext_offset[i]);
		MACRO_SPECIALTEXT_IOFUNCTION(FFIXWrite, FFIXSeek, false, false)
	}
	modified = false;
}

void SpecialTextDataSet::WritePPF(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i, j;
	for (i = 0; i < text_block.size(); i++) {
		text_block[i].UpdateOffset();
		ffbin.seekg(configset.spetext_offset[i]);
		MACRO_SPECIALTEXT_IOFUNCTION(PPFStepAdd, FFIXSeek, false, true)
	}
}

int* SpecialTextDataSet::LoadHWS(fstream& ffbin, UnusedSaveBackupPart& backup) {
	uint16_t nbmodified, size, i;
	int* res = new int[2];
	unsigned int j, k, modifcounter;
	vector<char> bufferstr;
	vector<int> indexes;
	int localizelangorder[STEAM_LANGUAGE_AMOUNT];
	int localizelangkeyindex = -1;
	SteamLanguage lang;
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		localizelangorder[lang] = lang;
	res[0] = 0; res[1] = 0;
	HWSReadShort(ffbin, nbmodified);
	for (modifcounter = 0; modifcounter < nbmodified; modifcounter++) {
		HWSReadShort(ffbin, i);
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			uint16_t spacetmp;
			if (i >= text_block.size() || GetGameType() != GAME_TYPE_PSX) { // DEBUG: give up importing PSX special text in Steam
				HWSReadShort(ffbin, size);
				ffbin.seekg(-4, ios::cur);
				backup.Add(ffbin, size + 8);
				res[1]++;
				continue;
			}
			size = text_block[i].space_total;
			HWSReadShort(ffbin, spacetmp);
			text_block[i].space_total = spacetmp;
			if (text_block[i].space_total > size) {
				ffbin.seekg(-4, ios::cur);
				backup.Add(ffbin, text_block[i].space_total + 8);
				res[0]++;
			} else {
				MACRO_SPECIALTEXT_IOFUNCTION(HWSRead, HWSSeek, true, false)
				if (GetGameType() != GAME_TYPE_PSX)
					for (j = 0; j < text_block[i].text.size(); j++)
						text_block[i].text[j].txt.PSXToSteam();
			}
			text_block[i].space_total = size;
		} else {
			SpecialTextDataStruct& textblock = text_block[i];
			uint16_t newamount;
			HWSReadShort(ffbin, newamount);
			if (!textblock.is_localization) {
				indexes.resize(newamount);
				if (GetHWSGlobalVersion() >= 100) {
					int newtextid;
					for (j = 0; j < newamount; j++) {
						HWSReadFlexibleChar(ffbin, newtextid, true);
						if (GetGameType() == GAME_TYPE_PSX)
							continue;
						indexes[j] = -1;
						for (k = 0; k < textblock.text.size(); k++) {
							if (newtextid == textblock.text[k].id) {
								indexes[j] = k;
								break;
							}
						}
						if (indexes[j] < 0) {
							FF9String newstr;
							newstr.CreateEmpty();
							if (textblock.AddText(newtextid, newstr) == 0)
								indexes[j] = textblock.GetTextIndexById(newtextid);
						}
					}
				} else if (GetGameType() != GAME_TYPE_PSX) {
					for (j = 0; j < newamount; j++)
						indexes[j] = j;
					if (newamount > textblock.text.size()) {
						unsigned int prevsize = textblock.text.size();
						textblock.text.resize(newamount);
						for (j = prevsize; j < newamount; j++)
							textblock.text[j].id = j;
					}
				}
			}
			uint32_t tmppos, spacetmp;
			HWSReadChar(ffbin, lang);
			while (lang != STEAM_LANGUAGE_NONE) {
				HWSReadLong(ffbin, spacetmp);
				tmppos = ffbin.tellg();
				if (GetGameType() == GAME_TYPE_PSX) { // DEBUG: give up importing Steam special text in PSX
					ffbin.seekg(tmppos + spacetmp);
					HWSReadChar(ffbin, lang);
					res[1]++;
					continue;
				}
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage() && lang != TEXT_LOCALIZATION_HWS_WHOLE_FILE && (lang != TEXT_LOCALIZATION_HWS_FIELDS || !text_block[i].is_localization)) {
					ffbin.seekg(tmppos + spacetmp);
					HWSReadChar(ffbin, lang);
					continue;
				}
				if (lang == TEXT_LOCALIZATION_HWS_WHOLE_FILE) {
					if (textblock.is_localization) {
						textblock.LocalizationRead(ffbin, spacetmp);
					} else {
						ffbin.seekg(tmppos + spacetmp);
						HWSReadChar(ffbin, lang);
						res[1]++;
						continue;
					}
				} else {
					if (lang == TEXT_LOCALIZATION_HWS_FIELDS) {
						if (textblock.is_localization) {
							for (j = 0; j < newamount; j++) {
								bufferstr.clear();
								bufferstr.push_back(ffbin.get());
								while (bufferstr[bufferstr.size() - 1] != 0)
									bufferstr.push_back(ffbin.get());
								textblock.text[j].localization_field = SpecialTextDataStruct::LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data()));
								if (textblock.text[j].localization_field.compare(L"KEY") == 0)
									localizelangkeyindex = j;
							}
						} else {
							ffbin.seekg(tmppos + spacetmp);
							HWSReadChar(ffbin, lang);
							res[1]++;
							continue;
						}
					} else if (textblock.is_localization) {
						if (newamount > 0) {
							bufferstr.clear();
							bufferstr.push_back(ffbin.get());
							while (bufferstr[bufferstr.size() - 1] != 0)
								bufferstr.push_back(ffbin.get());
							wstring firstentrystr = SpecialTextDataStruct::LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data()));
							if (localizelangkeyindex == 0 && GetHWSGlobalVersion() < 100)
								SetupLocalizationLanguageOrderByKey(firstentrystr, localizelangorder[lang]);
							textblock.text[0].txt.SetValue(firstentrystr, localizelangorder[lang]);
							for (j = 1; j < newamount; j++) {
								bufferstr.clear();
								bufferstr.push_back(ffbin.get());
								while (bufferstr[bufferstr.size() - 1] != 0)
									bufferstr.push_back(ffbin.get());
								textblock.text[j].txt.SetValue(SpecialTextDataStruct::LocalizationRawToNice(FF9String::GetUTF8FromByteCode(bufferstr.data())), localizelangorder[lang]);
							}
						}
					} else {
						for (j = 0; j < newamount; j++) {
							if (indexes[j] < 0) {
								FF9String dummystr;
								SteamReadFF9String(ffbin, dummystr, lang);
							} else {
								SteamReadFF9String(ffbin, textblock.text[indexes[j]].txt, lang);
								if (GetHWSGlobalVersion() < 100 && lang == STEAM_LANGUAGE_JA && i == 1 && textblock.text[indexes[j]].id >= 15 && textblock.text[indexes[j]].id < (int)textblock.base_amount)
									textblock.text[indexes[j]].txt.SetValue(L"", STEAM_LANGUAGE_JA);
							}
						}
					}
				}
				ffbin.seekg(tmppos + spacetmp);
				HWSReadChar(ffbin, lang);
			}
		}
		text_block[i].UpdateOffset();
	}
	modified = true;
	return res;
}

void SpecialTextDataSet::WriteHWS(fstream& ffbin, UnusedSaveBackupPart& backup) {
	uint16_t size, i, spacetmp;
	unsigned int j;
	SteamLanguage lang;
	if (GetGameType() == GAME_TYPE_PSX)
		HWSWriteShort(ffbin, HADES_STRING_SPECIAL_TEXT_BLOCK.size() + backup.save_amount);
	else
		HWSWriteShort(ffbin, HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM.size() + backup.save_amount);
	for (i = 0; i < text_block.size(); i++) {
		text_block[i].UpdateOffset();
		HWSWriteShort(ffbin, i);
		if (GetGameType() == GAME_TYPE_PSX) {
			size = text_block[i].space_total;
			text_block[i].space_total = text_block[i].space_used;
			spacetmp = text_block[i].space_total;
			HWSWriteShort(ffbin, spacetmp);
			MACRO_SPECIALTEXT_IOFUNCTION(HWSWrite, HWSSeek, false, false)
			text_block[i].space_total = size;
		} else {
			HWSWriteShort(ffbin, text_block[i].text.size());
			if (text_block[i].is_localization) {
				HWSWriteChar(ffbin, TEXT_LOCALIZATION_HWS_WHOLE_FILE);
				HWSWriteLong(ffbin, 0);
				uint32_t locpos = ffbin.tellg();
				WriteSteamLocalization(ffbin, false);
				uint32_t locsize = (uint32_t)ffbin.tellg() - locpos;
				ffbin.seekg(locpos - 4);
				HWSWriteLong(ffbin, locsize);
				ffbin.seekg(locpos + locsize);
			} else {
				for (j = 0; j < text_block[i].text.size(); j++)
					HWSWriteFlexibleChar(ffbin, text_block[i].text[j].id, true);
				for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
					if (hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
						HWSWriteChar(ffbin, lang);
						HWSWriteLong(ffbin, text_block[i].GetHWSDataSize(lang));
						WriteHWSSteam(ffbin, i, lang);
					}
			}
			HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
		}
	}
	for (i = 0; i < backup.save_amount; i++)
		for (j = 0; j < backup.save_size[i]; j++)
			HWSWriteChar(ffbin, backup.save_data[i][j]);
}

int SpecialTextDataSet::GetSpellNamingIndex() {
	if (GetGameType()==GAME_TYPE_PSX)
		return SPECIAL_TEXT_SPELL_NAMING_INDEX_PSX;
	return SPECIAL_TEXT_SPELL_NAMING_INDEX_STEAM;
}
