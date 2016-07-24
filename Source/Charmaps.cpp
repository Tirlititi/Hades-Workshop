#include "Charmaps.h"

void CharmapDataStruct::CalculateTextSize(FF9String* str, CharmapDataStruct* chmapext, uint16_t* x, uint16_t* y) {
	uint16_t maxsizex = 0, tmpsizex = 0, maxsizey = 1, tmpsizey = 1;
	uint16_t codei = 0;
	uint8_t opcode;
	bool multichoiceon = false;
	unsigned int i;
	for (i=0;i<str->length;i++) {
		if (str->raw[i]<amount && str->raw[i]<0xF7) {
			tmpsizex += (width[str->raw[i]] >> 4)+1;
		} else if (str->raw[i]==CHARMAP_A_CODECHAR) {
			i++;
			if (str->raw[i]+0xF7<amount)
				tmpsizex += (width[str->raw[i]+0xF7] >> 4)+1;
		} else if (str->raw[i]==CHARMAP_B_CODECHAR) {
			i++;
			if (str->raw[i]+0x1F7<amount)
				tmpsizex += (width[str->raw[i]+0x1F7] >> 4)+1;
		} else if (str->raw[i]==CHARMAP_EXT_CODECHAR) {
			i++;
			if (chmapext)
				tmpsizex += (chmapext->width[str->raw[i]] >> 4)+1;
		} else if (str->raw[i]==0xF7) {
			if (!multichoiceon)
				maxsizex = max(maxsizex,tmpsizex);
			tmpsizex = 0;
			tmpsizey++;
		} else if (str->raw[i]==OPCODE_CHAR) {
			opcode = str->raw[i+1];
			if (opcode==0x8) { // INIT_MULTICHOICE
				multichoiceon = true;
			} else if (opcode==0x43) { // CARET_POSITION_REL
				tmpsizex += str->raw[i+2];
			} else if (opcode==0x44) { // CARET_POSITION_ABS
				maxsizex = max(maxsizex,tmpsizex);
				tmpsizex = str->raw[i+2];
			} else if (opcode==0x71) { // NEW_LINE_TABULAR
				tmpsizex += 18;
			} else if (opcode==0x38) { // BUTTON_START
				tmpsizex += 33;
			} else if (opcode==0x39) { // BUTTON_SELECT
				tmpsizex += 35;
			} else if (opcode==0x2F || opcode==0x30 || opcode==0x31 || opcode==0x32 || opcode==0x33 || opcode==0x34 || opcode==0x35 || opcode==0x36 || opcode==0x37/*
					*/ || opcode==0x3A || opcode==0x3B || opcode==0x3C || opcode==0x3D || opcode==0x3E || opcode==0x3F/*
					*/ || opcode==0x60 || opcode==0x61 || opcode==0x62 || opcode==0x63 || opcode==0x64 || opcode==0x65 || opcode==0x66 || opcode==0x67) { // small BUTTONs
				tmpsizex += 13;
			}
			i += str->code_arg_length[codei++];
		} else if (str->raw[i]==0xFE) {
			if (!multichoiceon)
				maxsizex = max(maxsizex,tmpsizex);
			tmpsizex = 0;
			maxsizey = max(maxsizey,tmpsizey);
			tmpsizey = 1;
			multichoiceon = false;
		}
	}
	if (!multichoiceon)
		*x = max(maxsizex,tmpsizex);
	else
		*x = maxsizex;
	*y = max(maxsizey,tmpsizey);
}

#define MACRO_CHARMAP_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,unknown1); \
	IO ## Short(f,unknown2); \
	IO ## Short(f,unknown3); \
	IO ## Short(f,amount); \
	IO ## Short(f,unknown4); \
	if (READ) { \
		img_x = new uint8_t[amount]; \
		img_y = new uint8_t[amount]; \
		img_width = new uint8_t[amount]; \
		width = new uint8_t[amount]; \
	} \
	for (i=0;i<amount;i++) { \
		IO ## Char(f,img_x[i]); \
		IO ## Char(f,img_y[i]); \
		IO ## Char(f,img_width[i]); \
		IO ## Char(f,width[i]); \
	} \
	if (PPF) PPFEndScanStep();


void CharmapDataStruct::Read(fstream& f) {
	if (loaded || GetGameType()!=GAME_TYPE_PSX)
		return;
	MACRO_CHARMAP_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	loaded = true;
}

void CharmapDataStruct::Write(fstream& f) {
	MACRO_CHARMAP_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void CharmapDataStruct::WritePPF(fstream& f) {
	MACRO_CHARMAP_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void CharmapDataStruct::ReadHWS(fstream& f) {
	MACRO_CHARMAP_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void CharmapDataStruct::WriteHWS(fstream& f) {
	MACRO_CHARMAP_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}
