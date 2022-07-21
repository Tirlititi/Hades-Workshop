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

int TextDataStruct::AddText(uint16_t id, FF9String& value) {
	uint16_t reqlen;
	if (GetGameType()==GAME_TYPE_PSX) {
		reqlen = 4+value.length;
		if (has_format)
			reqlen += 4;
	} else {
		reqlen = value.length;
	}
	if (GetExtraSize()<reqlen)
		return 1;
	unsigned int i;
	SetSize(size+reqlen);
	amount++;
	FF9String* newtext = new FF9String[amount];
	for (i=0;i<id;i++)
		newtext[i] = text[i];
	newtext[id] = value;
	for (i=id;i+1<amount;i++)
		newtext[i+1] = text[i];
	delete[] text;
	text = newtext;
	if (GetGameType()==GAME_TYPE_PSX) {
		uint16_t* newoffset = new uint16_t[amount];
		uint8_t* newsizey = new uint8_t[amount];
		uint8_t* newflag = new uint8_t[amount];
		memcpy(newoffset,offset,id*sizeof(uint16_t));
		memcpy(newsizey,size_y,id*sizeof(uint8_t));
		memcpy(newflag,flag,id*sizeof(uint8_t));
		newoffset[id] = 0;
		newsizey[id] = 0;
		newflag[id] = 0;
		memcpy(newoffset+id+1,offset+id,(amount-id-1)*sizeof(uint16_t));
		memcpy(newsizey+id+1,size_y+id,(amount-id-1)*sizeof(uint8_t));
		memcpy(newflag+id+1,flag+id,(amount-id-1)*sizeof(uint8_t));
		delete[] offset;
		delete[] size_y;
		delete[] flag;
		offset = newoffset;
		size_y = newsizey;
		flag = newflag;
		if (has_format) {
			uint16_t* newformatoffset = new uint16_t[amount];
			uint16_t* newsizex = new uint16_t[amount];
			uint16_t* newformatamount = new uint16_t[amount];
			TextFormatStruct** newformatdata = new TextFormatStruct*[amount];
			memcpy(newformatoffset,format_offset,id*sizeof(uint16_t));
			memcpy(newsizex,size_x,id*sizeof(uint16_t));
			memcpy(newformatamount,format_amount,id*sizeof(uint16_t));
			memcpy(newformatdata,format_data,id*sizeof(TextFormatStruct*));
			newformatoffset[id] = 0;
			newsizex[id] = 0;
			newformatamount[id] = 0;
			newformatdata[id] = NULL;
			memcpy(newformatoffset+id+1,format_offset+id,(amount-id-1)*sizeof(uint16_t));
			memcpy(newsizex+id+1,size_x+id,(amount-id-1)*sizeof(uint16_t));
			memcpy(newformatamount+id+1,format_amount+id,(amount-id-1)*sizeof(uint16_t));
			memcpy(newformatdata+id+1,format_data+id,(amount-id-1)*sizeof(TextFormatStruct*));
			delete[] format_offset;
			delete[] size_x;
			delete[] format_amount;
			delete[] format_data;
			format_offset = newformatoffset;
			size_x = newsizex;
			format_amount = newformatamount;
			format_data = newformatdata;
		}
	}
	return 0;
}

void TextDataStruct::RemoveText(uint16_t id) {
	uint16_t usedlen;
	unsigned int i;
	if (GetGameType()==GAME_TYPE_PSX) {
		usedlen = 4+text[id].length;
		if (has_format) {
			usedlen += 4;
			for (unsigned int i=0;i<format_amount[id];i++)
				usedlen += format_data[id][i].length;
		}
	} else {
		usedlen = text[id].length;
	}
	SetSize(size-usedlen);
	amount--;
	FF9String* newtext = new FF9String[amount];
	for (i=0;i<id;i++)
		newtext[i] = text[i];
	for (i=id;i<amount;i++)
		newtext[i] = text[i+1];
	delete[] text;
	text = newtext;
	if (GetGameType()==GAME_TYPE_PSX) {
		uint16_t* newoffset = new uint16_t[amount];
		uint8_t* newsizey = new uint8_t[amount];
		uint8_t* newflag = new uint8_t[amount];
		memcpy(newoffset,offset,id*sizeof(uint16_t));
		memcpy(newsizey,size_y,id*sizeof(uint8_t));
		memcpy(newflag,flag,id*sizeof(uint8_t));
		memcpy(newoffset+id,offset+id+1,(amount-id)*sizeof(uint16_t));
		memcpy(newsizey+id,size_y+id+1,(amount-id)*sizeof(uint8_t));
		memcpy(newflag+id,flag+id+1,(amount-id)*sizeof(uint8_t));
		delete[] offset;
		delete[] size_y;
		delete[] flag;
		offset = newoffset;
		size_y = newsizey;
		flag = newflag;
		if (has_format) {
			uint16_t* newformatoffset = new uint16_t[amount];
			uint16_t* newsizex = new uint16_t[amount];
			uint16_t* newformatamount = new uint16_t[amount];
			TextFormatStruct** newformatdata = new TextFormatStruct*[amount];
			memcpy(newformatoffset,format_offset,id*sizeof(uint16_t));
			memcpy(newsizex,size_x,id*sizeof(uint16_t));
			memcpy(newformatamount,format_amount,id*sizeof(uint16_t));
			memcpy(newformatdata,format_data,id*sizeof(TextFormatStruct*));
			memcpy(newformatoffset+id,format_offset+id+1,(amount-id)*sizeof(uint16_t));
			memcpy(newsizex+id,size_x+id+1,(amount-id)*sizeof(uint16_t));
			memcpy(newformatamount+id,format_amount+id+1,(amount-id)*sizeof(uint16_t));
			memcpy(newformatdata+id,format_data+id+1,(amount-id)*sizeof(TextFormatStruct*));
			delete[] format_offset;
			delete[] size_x;
			delete[] format_amount;
			delete[] format_data;
			format_offset = newformatoffset;
			size_x = newsizex;
			format_amount = newformatamount;
			format_data = newformatdata;
		}
	}
}

int TextDataStruct::SetText(uint16_t id, wstring& newvalue, SteamLanguage lang) {
	FF9String tmp(text[id]);
	tmp.SetValue(newvalue,lang);
	uint16_t oldlen = text[id].length;
	uint16_t newlen = tmp.length;
	if (newlen>GetExtraSize()+oldlen)
		return 1;
	text[id].SetValue(newvalue,lang);
	SetSize(size+newlen-oldlen);
	return 0;
}

int TextDataStruct::SetText(uint16_t id, FF9String& newvalue) {
	uint16_t oldlen = text[id].length;
	uint16_t newlen = newvalue.length;
	if (newlen>GetExtraSize()+oldlen)
		return 1;
	text[id] = FF9String(newvalue);
	SetSize(size+newlen-oldlen);
	return 0;
}

int TextDataStruct::AddFormatCode(uint16_t id, uint8_t opcode, uint8_t* arg) {
	int neededsize = 2+HADES_STRING_TEXT_OPCODE[opcode].length;
	if (neededsize>GetExtraSize())
		return 1;
	TextFormatStruct* newformat = new TextFormatStruct[format_amount[id]+1];
	memcpy(newformat,format_data[id],format_amount[id]*sizeof(TextFormatStruct));
	newformat[format_amount[id]].length = neededsize;
	newformat[format_amount[id]].opcode = opcode;
	newformat[format_amount[id]].data = new uint8_t[neededsize];
	newformat[format_amount[id]].data[0] = OPCODE_CHAR;
	newformat[format_amount[id]].data[1] = opcode;
	memcpy(newformat[format_amount[id]].data+2,arg,HADES_STRING_TEXT_OPCODE[opcode].length*sizeof(uint8_t));
	delete[] format_data[id];
	format_data[id] = newformat;
	format_amount[id]++;
	return 0;
}

int TextDataStruct::RemoveFormatCode(uint16_t id, uint8_t formatid) {
	TextFormatStruct* newformat = new TextFormatStruct[format_amount[id]-1];
	memcpy(newformat,format_data[id],formatid*sizeof(TextFormatStruct));
	memcpy(newformat+formatid,format_data[id]+formatid+1,(format_amount[id]-formatid-1)*sizeof(TextFormatStruct));
	delete[] format_data[id];
	format_data[id] = newformat;
	format_amount[id]--;
	return 0;
}

void TextDataStruct::SetDialogBoxSize(uint16_t id, uint16_t x, uint16_t y, bool changey) {
	if (GetGameType()==GAME_TYPE_PSX) {
		size_x[id] = (size_x[id] & 0xF000) + x;
		if (changey)
			size_y[id] = (size_y[id] & 0x80) + y;
	}
}

TextFormatStruct tmpformatlist[0x100];
uint8_t tmpformatchar[0x100];
#define MACRO_TEXT_IOFUNCTION(IO,SEEK,READ,PPF) \
	uint32_t strpos; \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,amount); \
	IO ## Short(f,zero16); \
	if (PPF) PPFEndScanStep(); \
	strpos = f.tellg(); \
	if (READ) { \
		text = new FF9String[amount]; \
		size_y = new uint8_t[amount]; \
		flag = new uint8_t[amount]; \
		offset = new uint16_t[amount]; \
		uint16_t firstoffset; \
		IO ## Short(f,firstoffset); \
		has_format = firstoffset > amount*4; \
		if (has_format) { \
			format_offset = new uint16_t[amount]; \
			size_x = new uint16_t[amount]; \
			format_amount = new uint16_t[amount]; \
			format_data = new TextFormatStruct*[amount]; \
		} \
	} \
	SEEK(f,strpos,0); \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<amount;i++) { \
		if (has_format) { \
			IO ## Short(f,format_offset[i]); \
			IO ## Short(f,size_x[i]); \
		} \
		IO ## Short(f,offset[i]); \
		IO ## Char(f,size_y[i]); \
		IO ## Char(f,flag[i]); \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<amount;i++) { \
		if (has_format) { \
			SEEK(f,strpos,format_offset[i]); \
			if (READ) { \
				unsigned int formatamount = 0, formatpos = 0; \
				uint8_t formatf9, formatcode; \
				while (formatpos<offset[i]-format_offset[i]) { \
					IO ## Char(f,formatf9); \
					IO ## Char(f,formatcode); \
					tmpformatlist[formatamount].opcode = formatcode; \
					if (HADES_STRING_TEXT_OPCODE[formatcode].length>=0) \
						tmpformatlist[formatamount].length = HADES_STRING_TEXT_OPCODE[formatcode].length+2; \
					else if (formatcode==0x0D) { \
						unsigned int fchari = 0; \
						size_t argpos = f.tellg(); \
						IO ## Char(f,tmpformatchar[fchari++]); \
						IO ## Char(f,tmpformatchar[fchari++]); \
						IO ## Char(f,tmpformatchar[fchari++]); \
						while (tmpformatchar[fchari-1]!=0xFF || tmpformatchar[fchari-2]!=0xFF || tmpformatchar[fchari-3]!=0xFF) \
							IO ## Char(f,tmpformatchar[fchari++]); \
						tmpformatlist[formatamount].length = fchari+2; \
						SEEK(f,argpos,0); \
					} \
					tmpformatlist[formatamount].data = new uint8_t[tmpformatlist[formatamount].length]; \
					tmpformatlist[formatamount].data[0] = formatf9; \
					tmpformatlist[formatamount].data[1] = formatcode; \
					for (j=2;j<tmpformatlist[formatamount].length;j++) \
						IO ## Char(f,tmpformatlist[formatamount].data[j]); \
					formatpos += tmpformatlist[formatamount].length; \
					formatamount++; \
				} \
				format_amount[i] = formatamount; \
				format_data[i] = new TextFormatStruct[formatamount]; \
				memcpy(format_data[i],tmpformatlist,formatamount*sizeof(TextFormatStruct)); \
			} else { \
				if (PPF) PPFInitScanStep(f); \
				for (j=0;j<format_amount[i];j++) \
					for (k=0;k<format_data[i][j].length;k++) \
						IO ## Char(f,format_data[i][j].data[k]); \
				if (PPF) PPFEndScanStep(); \
			} \
		} \
		SEEK(f,strpos,offset[i]); \
		if (PPF) PPFInitScanStep(f,true,text[i].length); \
		if (READ && extended_charmap) text[i].charmap_Ext = extended_charmap; \
		IO ## FF9String(f,text[i]); \
		if (PPF) PPFEndScanStep(); \
	}


void TextDataStruct::Read(fstream& f) {
	unsigned int i,j,k;
	if (loaded)
		return;
	if (parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)>=0) {
		uint16_t chmapid = parent_cluster->chunk[parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)].object_id[0];
		extended_charmap = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(chmapid);
	} else if (parent_cluster->clus_type==CLUSTER_TYPE_ENEMY)
		extended_charmap = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(0);
	else
		extended_charmap = NULL;
	if (size>0) {
		MACRO_TEXT_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		amount = 0;
		has_format = false;
	}
	loaded = true;
}

void TextDataStruct::Write(fstream& f) {
	unsigned int i,j,k;
	if (size>0) {
		MACRO_TEXT_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	}
	modified = false;
}

void TextDataStruct::WritePPF(fstream& f) {
	unsigned int i,j,k;
	if (size>0) {
		MACRO_TEXT_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
	}
}

void TextDataStruct::ReadHWS(fstream& f, bool multilang) {
	unsigned int i,j,k;
	if (GetHWSGameType()==GAME_TYPE_PSX) {
		if (size>0) {
			MACRO_TEXT_IOFUNCTION(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX) {
				for (i=0;i<amount;i++)
					text[i].PSXToSteam();
			}
		}
	} else {
		SteamLanguage lang;
		uint32_t txtdatasize, txtdatatotalsize;
		bool allocatedtext = false;
		uint16_t newamount;
		HWSReadShort(f,newamount);
		if (multilang) {
			HWSReadChar(f,lang);
			while (lang!=STEAM_LANGUAGE_NONE) {
				HWSReadLong(f,txtdatatotalsize);
				if ((GetGameType()==GAME_TYPE_PSX || hades::STEAM_SINGLE_LANGUAGE_MODE) && lang!=GetSteamLanguage()) {
					f.seekg(txtdatatotalsize,ios::cur);
					HWSReadChar(f,lang);
					continue;
				}
				if (!allocatedtext) {
					if (amount<newamount) {
						FF9String* newtext = new FF9String[max(amount,newamount)];
						for (i=0;i<amount;i++)
							newtext[i] = text[i];
						delete[] text;
						text = newtext;
					}
					if (GetGameType()==GAME_TYPE_PSX)
						amount = newamount;
					else
						amount = max(amount,newamount);
					allocatedtext = true;
				}
				txtdatasize = 0;
				for (i=0;i<newamount && txtdatasize<txtdatatotalsize;i++) {
					SteamReadFF9String(f,text[i],lang);
					if (GetGameType()==GAME_TYPE_PSX)
						text[i].SteamToPSX();
					txtdatasize += text[i].GetLength(lang);
				}
				HWSReadChar(f,lang);
			}
		} else {
			if (amount<newamount) {
				FF9String* newtext = new FF9String[max(amount,newamount)];
				for (i=0;i<amount;i++)
					newtext[i] = text[i];
				delete[] text;
				text = newtext;
			}
			if (GetGameType()==GAME_TYPE_PSX)
				amount = newamount;
			else
				amount = max(amount,newamount);
			for (i=0;i<newamount;i++)
				SteamReadFF9String(f,text[i]);
		}
	}
	UpdateOffset();
	MarkDataModified();
}

void TextDataStruct::WriteHWS(fstream& f, bool multilang) {
	unsigned int i,j,k;
	if (GetGameType()==GAME_TYPE_PSX) {
		if (size>0) {
			MACRO_TEXT_IOFUNCTION(HWSWrite,HWSSeek,false,false)
		}
	} else {
		if (multilang) {
			SteamLanguage lang;
			HWSWriteShort(f,amount);
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
					HWSWriteChar(f,lang);
					HWSWriteLong(f,GetDataSize(lang));
					for (i=0;i<amount;i++)
						SteamWriteFF9String(f,text[i],lang);
				}
			HWSWriteChar(f,STEAM_LANGUAGE_NONE);
		} else {
			HWSWriteShort(f,amount);
			for (i=0;i<amount;i++)
				SteamWriteFF9String(f,text[i]);
		}
	}
}

void TextDataStruct::WriteSteam(fstream& f, SteamLanguage lang) {
	unsigned int i;
	for (i=0;i<amount;i++)
		SteamWriteFF9String(f,text[i],lang);
}

int TextDataStruct::GetDataSize(SteamLanguage lang) {
	if (GetGameType()==GAME_TYPE_PSX)
		return size;
	unsigned int i;
	int res = 0;
	for (i=0;i<amount;i++)
		res += text[i].GetLength(lang);
	return res;
}

void TextDataStruct::UpdateOffset() {
	if (size==0 && GetGameType()==GAME_TYPE_PSX)
		return;
	if (GetGameType()!=GAME_TYPE_PSX)
		return;
	unsigned int i,j,k;
	j = has_format ? amount*8 : amount*4;
	if (has_format) {
		for (i=0;i<amount;i++) {
			format_offset[i] = j;
			for (k=0;k<format_amount[i];k++)
				j += format_data[i][k].length;
			offset[i] = j;
			j += text[i].length;
		}
	} else {
		for (i=0;i<amount;i++) {
			offset[i] = j;
			j += text[i].length;
		}
	}
	if (j%4)
		j += 4-j%4;
	SetSize(j+4);
}

void TextDataSet::Load(fstream& ffbin, ClusterSet& clusset) {
	unsigned int i,j,k,l;
	amount = clusset.text_amount;
	struct_id.resize(amount);
	name.resize(amount);
	text_data.resize(amount);
	tim_amount.resize(amount);
	j = 0;
	LoadingDialogInit(amount,_(L"Reading text blocks..."));
	if (GetGameType()==GAME_TYPE_PSX) {
		cluster_id.resize(amount);
		charmap.resize(amount);
		chartim.resize(amount);
		uint16_t numchmap = 0;
		for (i=0;i<clusset.amount;i++) {
			if (clusset.clus_type[i]==CLUSTER_TYPE_FIELD_TEXT) {
				ClusterData& clus = clusset.clus[i];
				cluster_id[j] = i;
				clus.CreateChildren(ffbin);
				for (k=0;k<clus.chunk_amount;k++) {
					for (l=0;l<clus.chunk[k].object_amount;l++) {
						ffbin.seekg(clus.chunk[k].object_offset[l]);
						clus.chunk[k].GetObject(l).Read(ffbin);
					}
				}
				if (clus.SearchChunkType(CHUNK_TYPE_TEXT)>=0) {
					ChunkData& chunktxt = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_TEXT)];
					text_data[j] = (TextDataStruct*)&chunktxt.GetObject(0);
					struct_id[j] = chunktxt.object_id[0];
				} else {
					text_data[j] = NULL;
					struct_id[j] = 0x1000+numchmap++;
					main_charmap_index = j;
				}
				if (clus.SearchChunkType(CHUNK_TYPE_CHARMAP)>=0) {
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
				for (k=0;k<G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME);k++)
					if (struct_id[j]==HADES_STRING_TEXT_BLOCK_NAME[k].id) {
						name[j] = HADES_STRING_TEXT_BLOCK_NAME[k].label;
						break;
					}
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
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<amount;i++) {
			ClusterData* dummyclus;
			SteamLanguage lang;
			uint16_t text_lang_amount[STEAM_LANGUAGE_AMOUNT];
			tim_amount[i] = 0;
			struct_id[i] = config.text_id[i];
			text_data[i] = new TextDataStruct[1];
			text_data[i]->Init(true,CHUNK_TYPE_TEXT,config.text_id[i],&dummyclus,CLUSTER_TYPE_FIELD_TEXT);
			text_data[i]->amount = 0;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.text_file[lang][i]));
				fsize = config.meta_res.GetFileSizeByIndex(config.text_file[lang][i]);
				buffer = new char[fsize];
				ffbin.read(buffer,fsize);
				text_lang_amount[lang] = FF9String::CountSteamTextAmount(buffer,fsize);
				text_data[i]->amount = max(text_data[i]->amount,text_lang_amount[lang]);
				delete[] buffer;
			}
			text_data[i]->text = new FF9String[text_data[i]->amount];
/*			text_data[i]->size_y = new uint8_t[text_data[i]->amount];
			text_data[i]->flag = new uint8_t[text_data[i]->amount];
			text_data[i]->offset = new uint16_t[text_data[i]->amount];
			text_data[i]->format_offset = new uint16_t[text_data[i]->amount];
			text_data[i]->size_x = new uint16_t[text_data[i]->amount];
			text_data[i]->format_amount = new uint16_t[text_data[i]->amount];
			text_data[i]->format_data = new TextFormatStruct*[text_data[i]->amount];
			text_data[i]->has_format = true;*/
			text_data[i]->loaded = true;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.text_file[lang][i]));
				text_data[i]->size = config.meta_res.GetFileSizeByIndex(config.text_file[lang][i]);
				for (j=0;j<text_lang_amount[lang];j++) { // ToDo: handle the sizes etc...
					SteamReadFF9String(ffbin,text_data[i]->text[j],lang);
/*					text_data[i]->size_y[j] = 0;
					text_data[i]->flag[j] = 0;
					text_data[i]->offset[j] = 0;
					text_data[i]->format_offset[j] = 0;
					text_data[i]->size_x[j] = 0;
					text_data[i]->format_amount[j] = 0;
					text_data[i]->format_data[j] = new TextFormatStruct[0];*/
				}
			}
			for (j=0;j<G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME);j++)
				if (struct_id[i]==HADES_STRING_TEXT_BLOCK_NAME[j].id) {
					name[i] = HADES_STRING_TEXT_BLOCK_NAME[j].label;
					break;
				}
			LoadingDialogUpdate(i);
		}
		ffbin.close();
	}
	LoadingDialogEnd();
}

void TextDataSet::Write(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].Write(ffbin);
	}
}

void TextDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].WritePPF(ffbin);
	}
}

int* TextDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i,j,k;
	uint32_t chunksize,clustersize,chunkpos,objectpos,objectsize;
	uint16_t nbmodified,objectid,fileobjectid;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[3];
	res[0] = 0; res[1] = 0; res[2] = 0;
	HWSReadShort(ffhws,nbmodified);
	for (i=0;i<nbmodified;i++) {
		objectpos = ffhws.tellg();
		HWSReadShort(ffhws,objectid);
		HWSReadLong(ffhws,clustersize);
		for (j=0;j<amount;j++) {
			fileobjectid = struct_id[j];
			if (text_data[j])
				clus = text_data[j]->parent_cluster;
			else
				clus = charmap[j]->parent_cluster;
			if (objectid==fileobjectid) {
				if (clustersize<=clus->size+clus->extra_size) {
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						chunkpos = ffhws.tellg();
						if (chunktype==CHUNK_TYPE_TEXT && text_data[j]) {
							text_data[j]->ReadHWS(ffhws);
							text_data[j]->SetSize(chunksize - (GetHWSGameType()==GAME_TYPE_PSX ? 0 : 2));
						} else if (chunktype==CHUNK_TYPE_CHARMAP && GetGameType()==GAME_TYPE_PSX && charmap[j]) {
							charmap[j]->ReadHWS(ffhws);
							charmap[j]->SetSize(chunksize);
						} else if (chunktype==CHUNK_TYPE_TIM && GetGameType()==GAME_TYPE_PSX && chartim[j]) {
							uint16_t timid;
							HWSReadShort(ffhws,timid);
							for (k=0;k<chartim[j]->parent_chunk->object_amount;k++)
								if (chartim[j][k].object_id==timid) {
									chartim[j][k].ReadHWS(ffhws);
									chartim[j][k].SetSize(chunksize-2);
								}
						} else if (chunktype==CHUNK_STEAM_TEXT_MULTILANG) {
							text_data[j]->ReadHWS(ffhws,true);
						} else
							res[1]++;
						ffhws.seekg(chunkpos+chunksize);
						HWSReadChar(ffhws,chunktype);
					}
				} else {
					objectsize = 7;
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						ffhws.seekg(chunksize,ios::cur);
						HWSReadChar(ffhws,chunktype);
						objectsize += chunksize+5;
					}
					ffhws.seekg(objectpos);
					backup.Add(ffhws,objectsize);
					res[0]++;
				}
				j = amount;
			} else if (j+1==amount) {
				objectsize = 7;
				HWSReadChar(ffhws,chunktype);
				while (chunktype!=0xFF) {
					HWSReadLong(ffhws,chunksize);
					ffhws.seekg(chunksize,ios::cur);
					HWSReadChar(ffhws,chunktype);
					objectsize += chunksize+5;
				}
				ffhws.seekg(objectpos);
				backup.Add(ffhws,objectsize);
				res[2]++;
			}
		}
	}
	return res;
}

void TextDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i,j;
	uint16_t fileobjectid,nbmodified = 0;
	uint32_t chunkpos, chunksize, nboffset = ffhws.tellg();
	ClusterData* clus;
	HWSWriteShort(ffhws,nbmodified);
	for (i=0;i<amount;i++) {
		fileobjectid = struct_id[i];
		if (text_data[i])
			clus = text_data[i]->parent_cluster;
		else
			clus = charmap[i]->parent_cluster;
		if (clus->modified) {
			clus->UpdateOffset();
			HWSWriteShort(ffhws,fileobjectid);
			HWSWriteLong(ffhws,clus->size);
			if (GetGameType()==GAME_TYPE_PSX) {
				if (text_data[i] && text_data[i]->modified) {
					HWSWriteChar(ffhws,CHUNK_TYPE_TEXT);
					HWSWriteLong(ffhws,text_data[i]->size);
					chunkpos = ffhws.tellg();
					text_data[i]->WriteHWS(ffhws);
					ffhws.seekg(chunkpos+text_data[i]->size);
				}
				if (charmap[i] && charmap[i]->modified) {
					HWSWriteChar(ffhws,CHUNK_TYPE_CHARMAP);
					HWSWriteLong(ffhws,charmap[i]->size);
					chunkpos = ffhws.tellg();
					charmap[i]->WriteHWS(ffhws);
					ffhws.seekg(chunkpos+charmap[i]->size);
				}
				if (chartim[i]) {
					for (j=0;j<chartim[i]->parent_chunk->object_amount;j++)
						if (chartim[i][j].modified) {
							HWSWriteChar(ffhws,CHUNK_TYPE_TIM);
							HWSWriteLong(ffhws,chartim[i][j].size+2);
							chunkpos = ffhws.tellg();
							HWSWriteShort(ffhws,chartim[i][j].object_id);
							chartim[i][j].WriteHWS(ffhws);
							ffhws.seekg(chunkpos+chartim[i][j].size+2);
						}
				}
			} else {
				if (text_data[i] && text_data[i]->modified) {
					HWSWriteChar(ffhws,CHUNK_STEAM_TEXT_MULTILANG);
					HWSWriteLong(ffhws,0);
					chunkpos = ffhws.tellg();
					text_data[i]->WriteHWS(ffhws,true);
					chunksize = (uint32_t)ffhws.tellg()-chunkpos;
					ffhws.seekg(chunkpos-4);
					HWSWriteLong(ffhws,chunksize);
					ffhws.seekg(chunkpos+chunksize);
				}
			}
			HWSWriteChar(ffhws,0xFF);
			nbmodified++;
		}
	}
	for (i=0;i<backup.save_amount;i++)
		for (j=0;j<backup.save_size[i];j++)
			HWSWriteChar(ffhws,backup.save_data[i][j]);
	nbmodified += backup.save_amount;
	uint32_t endoffset = ffhws.tellg();
	ffhws.seekg(nboffset);
	HWSWriteShort(ffhws,nbmodified);
	ffhws.seekg(endoffset);
}

int TextDataSet::GetIndexById(uint16_t textid) {
	for (unsigned int i = 0; i < amount; i++)
		if (textid == struct_id[i])
			return i;
	return -1;
}

//=============================//
//     Special Text Struct     //
//=============================//

int SpecialTextDataStruct::AddText(uint16_t id, FF9String& value) {
	uint16_t reqlen;
	if (GetGameType()==GAME_TYPE_PSX)
		reqlen = 4+value.length;
	else
		reqlen = value.length;
	if (space_total<space_used+reqlen)
		return 1;
	unsigned int i;
	space_used += reqlen;
	amount++;
	FF9String* newtext = new FF9String[amount];
	wstring* newfieldtext = NULL;
	for (i=0;i<id;i++)
		newtext[i] = text[i];
	newtext[id] = value;
	for (i=id;i+1<amount;i++)
		newtext[i+1] = text[i];
	delete[] text;
	text = newtext;
	if (is_localization) {
		newfieldtext = new wstring[amount];
		for (i=0;i<id;i++)
			newfieldtext[i] = localization_field[i];
		newfieldtext[id] = L"CustomField"+to_string(amount);
		for (i=id;i+1<amount;i++)
			newfieldtext[i+1] = localization_field[i];
		delete[] localization_field;
		localization_field = newfieldtext;
	}
	if (GetGameType()==GAME_TYPE_PSX) {
		uint16_t* newoffset = new uint16_t[amount];
		uint16_t* newsizex = new uint16_t[amount];
		memcpy(newoffset,offset,id*sizeof(uint16_t));
		memcpy(newsizex,size_x,id*sizeof(uint16_t));
		newoffset[id] = 0;
		newsizex[id] = 0;
		memcpy(newoffset+id+1,offset+id,(amount-id-1)*sizeof(uint16_t));
		memcpy(newsizex+id+1,size_x+id,(amount-id-1)*sizeof(uint16_t));
		delete[] offset;
		delete[] size_x;
		offset = newoffset;
		size_x = newsizex;
	}
	return 0;
}

void SpecialTextDataStruct::RemoveText(uint16_t id) {
	unsigned int i;
	space_used -= text[id].length;
	amount--;
	FF9String* newtext = new FF9String[amount];
	wstring* newfieldtext = NULL;
	for (i=0;i<id;i++)
		newtext[i] = text[i];
	for (i=id;i<amount;i++)
		newtext[i] = text[i+1];
	delete[] text;
	text = newtext;
	if (is_localization) {
		newfieldtext = new wstring[amount];
		for (i=0;i<id;i++)
			newfieldtext[i] = localization_field[i];
		for (i=id;i<amount;i++)
			newfieldtext[i] = localization_field[i+1];
		delete[] localization_field;
		localization_field = newfieldtext;
	}
	if (GetGameType()==GAME_TYPE_PSX) {
		space_used -= 4;
		uint16_t* newoffset = new uint16_t[amount];
		uint16_t* newsizex = new uint16_t[amount];
		memcpy(newoffset,offset,id*sizeof(uint16_t));
		memcpy(newsizex,size_x,id*sizeof(uint16_t));
		memcpy(newoffset+id,offset+id+1,(amount-id)*sizeof(uint16_t));
		memcpy(newsizex+id,size_x+id+1,(amount-id)*sizeof(uint16_t));
		delete[] offset;
		delete[] size_x;
		offset = newoffset;
		size_x = newsizex;
	}
}

int SpecialTextDataStruct::SetText(uint16_t id, wstring& newvalue) {
	FF9String tmp(text[id]);
	tmp.SetValue(newvalue);
	uint16_t oldlen = text[id].length;
	uint16_t newlen = tmp.length;
	if (space_used+newlen>space_total+oldlen)
		return 1;
	text[id].SetValue(newvalue);
	space_used += newlen-oldlen;
	parent->modified = true;
	return 0;
}

int SpecialTextDataStruct::SetText(uint16_t id, FF9String& newvalue) {
	uint16_t oldlen = text[id].length;
	uint16_t newlen = newvalue.length;
	if (space_used+newlen>space_total+oldlen)
		return 1;
	text[id] = FF9String(newvalue);
	space_used += newlen-oldlen;
	parent->modified = true;
	return 0;
}

int SpecialTextDataStruct::GetHWSDataSize(SteamLanguage lang) {
	if (GetGameType()==GAME_TYPE_PSX)
		return space_used;
	unsigned int i;
	int res = 0;
	if (is_localization) {
		for (i=0;i<amount;i++) {
			if (lang==STEAM_LANGUAGE_NONE) {
				FF9String fieldstr;
				fieldstr.SetValue(localization_field[i]);
				res += fieldstr.GetLength(GetSteamLanguage(),false)+1;
			} else {
				res += text[i].GetLength(lang,false)+1;
			}
		}
	} else {
		for (i=0;i<amount;i++)
			res += text[i].GetLength(lang);
	}
	return res;
}

int SpecialTextDataStruct::GetDataSize(SteamLanguage lang) {
	if (!is_localization)
		return GetHWSDataSize(lang);
	unsigned int i;
	int res = 0;
	for (i=0;i<amount;i++) {
		FF9String fieldstr;
		fieldstr.SetValue(localization_field[i]);
		res += fieldstr.GetLength(GetSteamLanguage(),false)+STEAM_LANGUAGE_AMOUNT+1;
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
			res += text[i].GetLength(lang,false);
			if (lang==GetSteamLanguage())
				res += (text[i].str.find(L',')!=string::npos ? 2 : 0);
			else
				res += (text[i].multi_lang_str[lang].find(L',')!=string::npos ? 2 : 0);
		}
	}
	return res;
}

void SpecialTextDataStruct::UpdateOffset() {
	unsigned int i,j;
	if (GetGameType()!=GAME_TYPE_PSX)
		return;
	j = amount*4;
	for (i=0;i<amount;i++) {
		offset[i] = j;
		j += text[i].length;
	}
	space_used = j;
}

#define MACRO_SPECIALTEXT_IOFUNCTION(IO,SEEK,READ,PPF) \
	uint32_t strpos; \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Long(ffbin,text_block[i].amount); \
	if (READ) { \
		text_block[i].text = new FF9String[text_block[i].amount]; \
		text_block[i].offset = new uint16_t[text_block[i].amount]; \
		text_block[i].size_x = new uint16_t[text_block[i].amount]; \
	} \
	strpos = ffbin.tellg(); \
	for (j=0;j<text_block[i].amount;j++) { \
		IO ## Short(ffbin,text_block[i].offset[j]); \
		IO ## Short(ffbin,text_block[i].size_x[j]); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) text_block[i].space_used = 4*text_block[i].amount; \
	if (PPF) PPFInitScanStep(ffbin,true,text_block[i].space_total); \
	for (j=0;j<text_block[i].amount;j++) { \
		SEEK(ffbin,strpos,text_block[i].offset[j]); \
		IO ## FF9String(ffbin,text_block[i].text[j]); \
		if (READ) text_block[i].space_used += text_block[i].text[j].length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,strpos,text_block[i].space_total);

#define MACRO_SPECIALTEXT_LOCALIZATION_READ(TEXTPTR,FIELDTEXTPTR,AMOUNT,SPACE) \
	uint32_t posbeg, poslen, bufferpos = 0; \
	char* buffer, *bufferstr; \
	bufferstr = new char[SPACE]; \
	buffer = new char[SPACE]; \
	ffbin.read(buffer,SPACE); \
	vector<FF9String> vecstr; \
	vector<wstring> vecfield; \
	for (i=0;bufferpos<SPACE;i++) { \
		vecstr.resize(i+1); \
		vecfield.resize(i+1); \
		vecstr[i].CreateEmpty(); \
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT+1;lang++) { \
			if (buffer[bufferpos]=='"') { \
				posbeg = ++bufferpos; \
				while (buffer[bufferpos++]!='"') {} \
				poslen = bufferpos-1-posbeg; \
				bufferpos++; \
			} else { \
				posbeg = bufferpos; \
				while (buffer[bufferpos]!=',' && buffer[bufferpos]!=0xA) {bufferpos++;} \
				poslen = (bufferpos++)-posbeg; \
			} \
			memcpy(bufferstr,&buffer[posbeg],poslen); \
			bufferstr[poslen] = 0; \
			if (lang==0) { \
				FF9String fieldstr; \
				fieldstr.CreateEmpty(); \
				fieldstr.SetValue(FF9String::GetUTF8FromByteCode(bufferstr)); \
				vecfield[i] = fieldstr.str; \
			} else { \
				vecstr[i].SetValue(FF9String::GetUTF8FromByteCode(bufferstr),lang-1); \
			} \
			if (lang<STEAM_LANGUAGE_AMOUNT && buffer[bufferpos-1]==0xA) { \
				while (lang<STEAM_LANGUAGE_AMOUNT) \
					vecstr[i].SetValue(L"",lang++); \
				break; \
			} \
			if (lang==STEAM_LANGUAGE_AMOUNT && buffer[bufferpos-1]!=0xA) \
				while (buffer[bufferpos++]!=0xA) {} \
		} \
	} \
	AMOUNT = i; \
	delete[] bufferstr; \
	delete[] buffer; \
	TEXTPTR = new FF9String[AMOUNT]; \
	FIELDTEXTPTR = new wstring[AMOUNT]; \
	for (i=0;i<AMOUNT;i++) { \
		TEXTPTR[i] = vecstr[i]; \
		FIELDTEXTPTR[i] = vecfield[i]; \
	}


void SpecialTextDataSet::Load(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i,j;
	amount = configset.spetext_amount;
	text_block = new SpecialTextDataStruct[amount];
	if (GetGameType()==GAME_TYPE_PSX) {
		for (i=0;i<amount;i++) {
			text_block[i].parent = this;
			text_block[i].space_total = configset.spetext_space_total[i];
			text_block[i].is_localization = false;
			ffbin.seekg(configset.spetext_offset[i]);
			MACRO_SPECIALTEXT_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
		}
	} else {
		SteamLanguage lang;
		for (i=0;i<amount;i++) {
			text_block[i].parent = this;
			text_block[i].space_total = configset.spetext_steam_space_total[i];
		}
		string fname = configset.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		
		#define MACRO_READ_SPETEXT_STEAM(INDEX,TYPE) \
			text_block[INDEX].space_used = configset.meta_res.GetFileSizeByIndex(configset.spetext_ ## TYPE ## _file[GetSteamLanguage()]); \
			text_block[INDEX].amount = configset.spetext_ ## TYPE ## _amount; \
			text_block[INDEX].is_localization = false; \
			text_block[INDEX].text = new FF9String[text_block[INDEX].amount]; \
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) { \
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage()) \
					continue; \
				ffbin.seekg(configset.meta_res.GetFileOffsetByIndex(configset.spetext_ ## TYPE ## _file[lang])); \
				for (j=0;j<text_block[INDEX].amount;j++) \
					SteamReadFF9String(ffbin,text_block[INDEX].text[j],lang); \
			}
		
		MACRO_READ_SPETEXT_STEAM(0,battleinfo)
		MACRO_READ_SPETEXT_STEAM(1,battlescan)
		MACRO_READ_SPETEXT_STEAM(2,spellnaming)
		MACRO_READ_SPETEXT_STEAM(3,chocomenu)
		MACRO_READ_SPETEXT_STEAM(4,cardrank)
		MACRO_READ_SPETEXT_STEAM(5,tetramaster)
		// DEBUG : Steam version is buggy for texts in Localization.txt
		// commas are used both inside texts and as language separator
		ffbin.seekg(configset.meta_res.GetFileOffsetByIndex(configset.spetext_localization_file));
//		text_block[6].amount = configset.spetext_localization_amount;
		text_block[6].is_localization = true;
//		text_block[6].text = new FF9String[text_block[6].amount];
//		text_block[6].localization_field = new wstring[text_block[6].amount];
		text_block[6].space_used = configset.meta_res.GetFileSizeByIndex(configset.spetext_localization_file);
		MACRO_SPECIALTEXT_LOCALIZATION_READ(text_block[6].text,text_block[6].localization_field,text_block[6].amount,text_block[6].space_used)
		ffbin.close();
	}
	for (i=0;i<amount;i++)
		text_block[i].UpdateOffset();
	modified = false;
}

void SpecialTextDataSet::WriteSteam(fstream& ffbin, unsigned int blockid, SteamLanguage lang) { // DEBUG: Localization.txt is always fully updated, whatever the Steam language saving options
	unsigned int i;
	bool quotestr;
	if (text_block[blockid].is_localization) {
		for (i=0;i<text_block[blockid].amount;i++) {
			FF9String fieldstr;
			fieldstr.SetValue(text_block[blockid].localization_field[i]);
			SteamWriteFF9String(ffbin,fieldstr,GetSteamLanguage(),false);
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				ffbin.put(',');
				if (lang==GetSteamLanguage())
					quotestr = text_block[blockid].text[i].str.find(L',')!=string::npos;
				else
					quotestr = text_block[blockid].text[i].multi_lang_str[lang].find(L',')!=string::npos;
				if (quotestr)
					ffbin.put('\"');
				SteamWriteFF9String(ffbin,text_block[blockid].text[i],lang,false);
				if (quotestr)
					ffbin.put('\"');
			}
			ffbin.put(0xA);
		}
	} else {
		for (i=0;i<text_block[blockid].amount;i++)
			SteamWriteFF9String(ffbin,text_block[blockid].text[i],lang);
	}
}

void SpecialTextDataSet::WriteHWSSteam(fstream& ffbin, unsigned int blockid, SteamLanguage lang) {
	unsigned int i;
	if (text_block[blockid].is_localization) {
		for (i=0;i<text_block[blockid].amount;i++) {
			if (lang==STEAM_LANGUAGE_NONE) {
				FF9String fieldstr;
				fieldstr.SetValue(text_block[blockid].localization_field[i]);
				SteamWriteFF9String(ffbin,fieldstr,GetSteamLanguage(),false);
				ffbin.put(0); // Null-terminated strings, for allowing the presence or absence of [ENDN]
			} else {
				SteamWriteFF9String(ffbin,text_block[blockid].text[i],lang,false);
				ffbin.put(0);
			}
		}
	} else {
		for (i=0;i<text_block[blockid].amount;i++)
			SteamWriteFF9String(ffbin,text_block[blockid].text[i],lang);
	}
}

void SpecialTextDataSet::Write(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i,j;
	for (i=0;i<amount;i++) {
		text_block[i].UpdateOffset();
		ffbin.seekg(configset.spetext_offset[i]);
		MACRO_SPECIALTEXT_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	}
	modified = false;
}

void SpecialTextDataSet::WritePPF(fstream& ffbin, ConfigurationSet& configset) {
	unsigned int i,j;
	for (i=0;i<amount;i++) {
		text_block[i].UpdateOffset();
		ffbin.seekg(configset.spetext_offset[i]);
		MACRO_SPECIALTEXT_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
	}
}

int* SpecialTextDataSet::LoadHWS(fstream& ffbin, UnusedSaveBackupPart& backup) {
	uint16_t nbmodified,size,i;
	int* res = new int[2];
	unsigned int j,k;
	res[0] = 0; res[1] = 0;
	HWSReadShort(ffbin,nbmodified);
	for (k=0;k<nbmodified;k++) {
		HWSReadShort(ffbin,i);
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			uint16_t spacetmp;
			if (i>=amount || GetGameType()!=GAME_TYPE_PSX) { // DEBUG: give up importing PSX special text in Steam
				HWSReadShort(ffbin,size);
				ffbin.seekg(-4,ios::cur);
				backup.Add(ffbin,size+8);
				res[1]++;
				continue;
			}
			size = text_block[i].space_total;
			HWSReadShort(ffbin,spacetmp);
			text_block[i].space_total = spacetmp;
			if (text_block[i].space_total>size) {
				ffbin.seekg(-4,ios::cur);
				backup.Add(ffbin,text_block[i].space_total+8);
				res[0]++;
			} else {
				MACRO_SPECIALTEXT_IOFUNCTION(HWSRead,HWSSeek,true,false)
/*				if (GetGameType()!=GAME_TYPE_PSX)
					for (j=0;j<text_block[i].amount;j++)
						text_block[i].text[j].PSXToSteam();*/
			}
			text_block[i].space_total = size;
		} else {
			bool allocatedtext = false;
			SteamLanguage lang;
			uint16_t modifamount;
			uint32_t tmppos, spacetmp;
			vector<char> bufferstr;
			HWSReadShort(ffbin,modifamount);
			HWSReadChar(ffbin,lang);
			while (lang!=STEAM_LANGUAGE_NONE) {
				HWSReadLong(ffbin,spacetmp);
				tmppos = ffbin.tellg();
				if (GetGameType()==GAME_TYPE_PSX) { // DEBUG: give up importing Steam special text in PSX
					ffbin.seekg(tmppos+spacetmp);
					HWSReadChar(ffbin,lang);
					res[1]++;
					continue;
				}
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage() && lang!=TEXT_LOCALIZATION_HWS_WHOLE_FILE && (lang!=TEXT_LOCALIZATION_HWS_FIELDS || !text_block[i].is_localization)) {
					ffbin.seekg(tmppos+spacetmp);
					HWSReadChar(ffbin,lang);
					continue;
				}
				if (lang==TEXT_LOCALIZATION_HWS_WHOLE_FILE) {
					if (text_block[i].is_localization) {
						MACRO_SPECIALTEXT_LOCALIZATION_READ(text_block[i].text,text_block[i].localization_field,modifamount,spacetmp)
					} else {
						ffbin.seekg(tmppos+spacetmp);
						HWSReadChar(ffbin,lang);
						res[1]++;
						continue;
					}
				} else {
					if (!allocatedtext) {
						if (text_block[i].amount<modifamount) {
							FF9String* newtext = new FF9String[max((uint16_t)text_block[i].amount,modifamount)];
							for (j=0;j<text_block[i].amount;j++)
								newtext[j] = text_block[i].text[j];
							delete[] text_block[i].text;
							text_block[i].text = newtext;
							if (text_block[i].is_localization) {
								wstring* newlocfieldstr = new wstring[max((uint16_t)text_block[i].amount,modifamount)];
								for (j=0;j<text_block[i].amount;j++)
									newlocfieldstr[j] = text_block[i].localization_field[j];
								delete[] text_block[i].localization_field;
								text_block[i].localization_field = newlocfieldstr;
							}
						}
						text_block[i].amount = max((uint16_t)text_block[i].amount,modifamount);
						allocatedtext = true;
					}
					if (lang==TEXT_LOCALIZATION_HWS_FIELDS) {
						if (text_block[i].is_localization) {
							for (j=0;j<modifamount;j++) {
								bufferstr.clear();
								bufferstr.push_back(ffbin.get());
								while (bufferstr[bufferstr.size()-1]!=0)
									bufferstr.push_back(ffbin.get());
								char* bufferstrptr = new char[bufferstr.size()];
								for (k=0;k<bufferstr.size();k++)
									bufferstrptr[k] = bufferstr[k];
								FF9String fieldstr;
								fieldstr.CreateEmpty();
								fieldstr.SetValue(FF9String::GetUTF8FromByteCode(bufferstrptr));
								text_block[i].localization_field[j] = fieldstr.str;
								delete[] bufferstrptr;
							}
						} else {
							ffbin.seekg(tmppos+spacetmp);
							HWSReadChar(ffbin,lang);
							res[1]++;
							continue;
						}
					} else if (text_block[i].is_localization) {
						for (j=0;j<modifamount;j++) {
							bufferstr.clear();
							bufferstr.push_back(ffbin.get());
							while (bufferstr[bufferstr.size()-1]!=0)
								bufferstr.push_back(ffbin.get());
							char* bufferstrptr = new char[bufferstr.size()];
							for (k=0;k<bufferstr.size();k++)
								bufferstrptr[k] = bufferstr[k];
							text_block[i].text[j].SetValue(FF9String::GetUTF8FromByteCode(bufferstrptr),lang);
							delete[] bufferstrptr;
						}
					} else {
						for (j=0;j<modifamount;j++)
							SteamReadFF9String(ffbin,text_block[i].text[j],lang);
					}
				}
				ffbin.seekg(tmppos+spacetmp);
				HWSReadChar(ffbin,lang);
			}
		}
		text_block[i].UpdateOffset();
	}
	modified = true;
	return res;
}

void SpecialTextDataSet::WriteHWS(fstream& ffbin, UnusedSaveBackupPart& backup) {
	uint16_t size,i,spacetmp;
	unsigned int j;
	SteamLanguage lang;
	if (GetGameType()==GAME_TYPE_PSX)
		HWSWriteShort(ffbin,G_N_ELEMENTS(HADES_STRING_SPECIAL_TEXT_BLOCK)+backup.save_amount);
	else
		HWSWriteShort(ffbin,G_N_ELEMENTS(HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM)+backup.save_amount);
	for (i=0;i<amount;i++) {
		text_block[i].UpdateOffset();
		HWSWriteShort(ffbin,i);
		if (GetGameType()==GAME_TYPE_PSX) {
			size = text_block[i].space_total;
			text_block[i].space_total = text_block[i].space_used;
			spacetmp = text_block[i].space_total;
			HWSWriteShort(ffbin,spacetmp);
			MACRO_SPECIALTEXT_IOFUNCTION(HWSWrite,HWSSeek,false,false)
			text_block[i].space_total = size;
		} else {
			HWSWriteShort(ffbin,text_block[i].amount);
			if (text_block[i].is_localization) {
				HWSWriteChar(ffbin,TEXT_LOCALIZATION_HWS_FIELDS);
				HWSWriteLong(ffbin,text_block[i].GetHWSDataSize(STEAM_LANGUAGE_NONE));
				WriteHWSSteam(ffbin,i,STEAM_LANGUAGE_NONE);
			}
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
					HWSWriteChar(ffbin,lang);
					HWSWriteLong(ffbin,text_block[i].GetHWSDataSize(lang));
					WriteHWSSteam(ffbin,i,lang);
				}
			HWSWriteChar(ffbin,STEAM_LANGUAGE_NONE);
		}
	}
	for (i=0;i<backup.save_amount;i++)
		for (j=0;j<backup.save_size[i];j++)
			HWSWriteChar(ffbin,backup.save_data[i][j]);
}

int SpecialTextDataSet::GetSpellNamingIndex() {
	if (GetGameType()==GAME_TYPE_PSX)
		return SPECIAL_TEXT_SPELL_NAMING_INDEX_PSX;
	return SPECIAL_TEXT_SPELL_NAMING_INDEX_STEAM;
}
