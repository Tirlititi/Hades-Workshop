#include "Cards.h"

#include "main.h"
#include "CommonUtility.h"

#define CARD_HWS_VERSION 2

int CardDataStruct::SetName(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		int oldlen = name.length;
		int newlen = tmp.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name.SetValue(newvalue);
	return 0;
}

int CardDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = name.length;
		int newlen = newvalue.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name = newvalue;
	return 0;
}

#define MACRO_CARD_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	uint32_t txtpos; \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Long(ffbin,card_amount); \
	txtpos = ffbin.tellg(); \
	for (i=0;i<CARD_AMOUNT;i++) { \
		IO ## Short(ffbin,card[i].name_offset); \
		IO ## Short(ffbin,card[i].name_size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) name_space_used = 4*CARD_AMOUNT; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<CARD_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,card[i].name_offset); \
		IO ## FF9String(ffbin,card[i].name); \
		if (READ) name_space_used += card[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total);

#define MACRO_CARD_IOFUNCTIONCARDDATA(IO,SEEK,READ,PPF,NPC) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<CARD_AMOUNT;i++) { \
		if (NPC) { \
			IO ## Char(ffbin,card[i].npc_attack); \
			IO ## Char(ffbin,card[i].npc_type); \
			IO ## Char(ffbin,card[i].npc_defence); \
			IO ## Char(ffbin,card[i].npc_magicdefence); \
		} else { \
			IO ## Char(ffbin,card[i].player_attack); \
			IO ## Char(ffbin,card[i].player_type); \
			IO ## Char(ffbin,card[i].player_defence); \
			IO ## Char(ffbin,card[i].player_magicdefence); \
		} \
		IO ## Char(ffbin,card[i].points); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_CARD_IOFUNCTIONSET(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<CARD_SET_AMOUNT;i++) { \
		for (unsigned int j=0;j<CARD_SET_CAPACITY;j++) { \
			IO ## Char(ffbin,set[i].card[j]); \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_CARD_IOFUNCTIONDECK(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<CARD_DECK_AMOUNT;i++) { \
		IO ## Char(ffbin,deck[i].set); \
		IO ## Char(ffbin,deck[i].difficulty); \
	} \
	if (PPF) PPFEndScanStep();



void CardDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	name_space_total = config.card_name_space_total;
	card.resize(CARD_AMOUNT);
	for (i = 0; i < CARD_AMOUNT; i++) {
		card[i].parent = this;
		card[i].id = i;
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.card_text_offset);
		MACRO_CARD_IOFUNCTIONNAME(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.card_stat_offset[0]);
		MACRO_CARD_IOFUNCTIONCARDDATA(FFIXRead, FFIXSeek, true, false, false)
		ffbin.seekg(config.card_stat_offset[1]);
		MACRO_CARD_IOFUNCTIONCARDDATA(FFIXRead, FFIXSeek, true, false, true)
		ffbin.seekg(config.card_deck_offset);
		MACRO_CARD_IOFUNCTIONDECK(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.card_set_offset);
		MACRO_CARD_IOFUNCTIONSET(FFIXRead, FFIXSeek, true, false)
	} else {
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.card_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.card_name_file[lang]);
			for (i = 0; i < CARD_AMOUNT; i++)
				SteamReadFF9String(ffbin, card[i].name, lang);
		}
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.card_stat_file));
		MACRO_CARD_IOFUNCTIONCARDDATA(SteamRead, SteamSeek, true, false, false)
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.card_deck_file));
		MACRO_CARD_IOFUNCTIONDECK(SteamRead, SteamSeek, true, false)
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.card_set_file));
		MACRO_CARD_IOFUNCTIONSET(SteamRead, SteamSeek, true, false)
		ffbin.close();
	}
}

void CardDataSet::WriteSteamText(fstream& ffbin, bool onlymodified, bool asmes, SteamLanguage lang) {
	vector<int> writesubset;
	if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<CardDataStruct>(&writesubset, GetGameConfiguration()->card_name_file, card, [lang](CardDataStruct& c) { return c.name.multi_lang_str[lang]; }, lang))
		WriteSteamTextGeneric(ffbin, card, &CardDataStruct::name, &writesubset, asmes, lang);
	else
		WriteSteamTextGeneric(ffbin, card, &CardDataStruct::name, NULL, asmes, lang);
}

void CardDataSet::WriteSteamData(fstream& ffbin, unsigned int datatype) {
	unsigned int i;
	if (datatype==0) {
		MACRO_CARD_IOFUNCTIONCARDDATA(HWSWrite,HWSSeek,false,false,false)
	} else if (datatype==1) {
		MACRO_CARD_IOFUNCTIONDECK(HWSWrite,HWSSeek,false,false)
	} else {
		MACRO_CARD_IOFUNCTIONSET(HWSWrite,HWSSeek,false,false)
	}
}

void CardDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	UpdateOffset();
	ffbin.seekg(config.card_text_offset);
	MACRO_CARD_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.card_stat_offset[0]);
	MACRO_CARD_IOFUNCTIONCARDDATA(FFIXWrite,FFIXSeek,false,false,false)
	ffbin.seekg(config.card_stat_offset[1]);
	MACRO_CARD_IOFUNCTIONCARDDATA(FFIXWrite,FFIXSeek,false,false,true)
	ffbin.seekg(config.card_set_offset);
	MACRO_CARD_IOFUNCTIONSET(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.card_deck_offset);
	MACRO_CARD_IOFUNCTIONDECK(FFIXWrite,FFIXSeek,false,false)
}

void CardDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	UpdateOffset();
	ffbin.seekg(config.card_text_offset);
	MACRO_CARD_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.card_stat_offset[0]);
	MACRO_CARD_IOFUNCTIONCARDDATA(PPFStepAdd,FFIXSeek,false,true,false)
	ffbin.seekg(config.card_stat_offset[1]);
	MACRO_CARD_IOFUNCTIONCARDDATA(PPFStepAdd,FFIXSeek,false,true,true)
	ffbin.seekg(config.card_set_offset);
	MACRO_CARD_IOFUNCTIONSET(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.card_deck_offset);
	MACRO_CARD_IOFUNCTIONDECK(PPFStepAdd,FFIXSeek,false,true)
}

int CardDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	bool useextendedtype = false;
	SteamLanguage lg;
	int txtspace;
	int i;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total;
	HWSReadShort(ffbin, version);
	HWSReadShort(ffbin, name_space_total);
	if (version >= 2) {
		useextendedtype = true;
	}
	if (name_space_total <= namesize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			MACRO_CARD_IOFUNCTIONNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < CARD_AMOUNT; i++)
					card[i].name.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < CARD_AMOUNT; i++)
						SteamReadFF9String(ffbin, card[i].name, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < CARD_AMOUNT; i++) {
						SteamReadFF9String(ffbin, card[i].name);
						card[i].name.SteamToPSX();
					}
				ffbin.seekg(tmppos + txtspace);
				HWSReadChar(ffbin, lg);
			}
		}
	} else {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			ffbin.seekg(name_space_total + 4, ios::cur);
		} else {
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				ffbin.seekg(txtspace, ios::cur);
				HWSReadChar(ffbin, lg);
			}
		}
		if (usetext)
			res |= 1;
	}
	name_space_total = namesize;
	MACRO_CARD_IOFUNCTIONCARDDATA(HWSRead, HWSSeek, true, false, false)
	MACRO_CARD_IOFUNCTIONCARDDATA(HWSRead, HWSSeek, true, false, true)
	MACRO_CARD_IOFUNCTIONSET(HWSRead, HWSSeek, true, false)
	MACRO_CARD_IOFUNCTIONDECK(HWSRead, HWSSeek, true, false)
	UpdateOffset();
	return res;
}

void CardDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i;
	UpdateOffset();
	HWSWriteShort(ffbin, CARD_HWS_VERSION);
	uint16_t namesize = name_space_total;
	name_space_total = name_space_used;
	HWSWriteShort(ffbin, name_space_total);
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_CARD_IOFUNCTIONNAME(HWSWrite, HWSSeek, false, false)
	} else {
		SteamLanguage lg;
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			HWSWriteChar(ffbin, lg);
			HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(card, &CardDataStruct::name, false, lg), true);
			for (i = 0; i < CARD_AMOUNT; i++)
				SteamWriteFF9String(ffbin, card[i].name, lg);
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
	}
	MACRO_CARD_IOFUNCTIONCARDDATA(HWSWrite, HWSSeek, false, false, false)
	MACRO_CARD_IOFUNCTIONCARDDATA(HWSWrite, HWSSeek, false, false, true)
	MACRO_CARD_IOFUNCTIONSET(HWSWrite, HWSSeek, false, false)
	MACRO_CARD_IOFUNCTIONDECK(HWSWrite, HWSSeek, false, false)
	name_space_total = namesize;
}

void CardDataSet::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	unsigned int i;
	uint16_t j;
	j = 4 * CARD_AMOUNT;
	for (i = 0; i < CARD_AMOUNT; i++) {
		card[i].name_offset = j;
		j += card[i].name.length;
	}
	name_space_used = j;
}
