#include "Cluts.h"

#include <cstring>

#define MACRO_CLUT_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j; \
	uint32_t filepos,modelpos; \
	uint16_t zero16 = 0; \
	filepos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Char(f,magic_number); \
	IO ## Char(f,model_amount); \
	IO ## Short(f,zero16); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		model_unknown1 = new uint32_t[model_amount]; \
		model_cluts_offset = new uint16_t[model_amount]; \
		model_unknown2 = new uint8_t[model_amount]; \
		model_clut_amount = new uint8_t[model_amount]; \
		model_unknown3 = new uint32_t[model_amount]; \
		clut_unknown1 = new uint32_t*[model_amount]; \
		clut_unknown2 = new uint32_t*[model_amount]; \
		clut_unknown3 = new uint32_t*[model_amount]; \
		clut_unknown4 = new uint32_t*[model_amount]; \
	} \
	for (i=0;i<model_amount;i++) { \
		SEEK(f,filepos,4+i*0xC); \
		modelpos = f.tellg(); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Long(f,model_unknown1[i]); \
		IO ## Short(f,model_cluts_offset[i]); \
		IO ## Char(f,model_unknown2[i]); \
		IO ## Char(f,model_clut_amount[i]); \
		IO ## Long(f,model_unknown3[i]); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			clut_unknown1[i] = new uint32_t[model_clut_amount[i]]; \
			clut_unknown2[i] = new uint32_t[model_clut_amount[i]]; \
			clut_unknown3[i] = new uint32_t[model_clut_amount[i]]; \
			clut_unknown4[i] = new uint32_t[model_clut_amount[i]]; \
		} \
		SEEK(f,modelpos,model_cluts_offset[i]); \
		for (j=0;j<model_clut_amount[i];j++) { \
			if (PPF) PPFInitScanStep(f); \
			IO ## Long(f,clut_unknown1[i][j]); \
			IO ## Long(f,clut_unknown2[i][j]); \
			IO ## Long(f,clut_unknown3[i][j]); \
			IO ## Long(f,clut_unknown4[i][j]); \
			if (PPF) PPFEndScanStep(); \
		} \
	}


void ClutDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	MACRO_CLUT_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	loaded = true;
}

void ClutDataStruct::Write(fstream& f) {
	MACRO_CLUT_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void ClutDataStruct::WritePPF(fstream& f) {
	MACRO_CLUT_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void ClutDataStruct::ReadHWS(fstream& f) {
	MACRO_CLUT_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void ClutDataStruct::WriteHWS(fstream& f) {
	MACRO_CLUT_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

void ClutDataStruct::Copy(ClutDataStruct& from, bool deleteold) {
	unsigned int i;
	if (deleteold) {
		for (i=0;i<model_amount;i++) {
			delete[] clut_unknown1[i];
			delete[] clut_unknown2[i];
			delete[] clut_unknown3[i];
			delete[] clut_unknown4[i];
		}
		delete[] model_unknown1;
		delete[] model_cluts_offset;
		delete[] model_unknown2;
		delete[] model_clut_amount;
	}
	*this = from;
	model_unknown1 = new uint32_t[model_amount];
	model_cluts_offset = new uint16_t[model_amount];
	model_unknown2 = new uint8_t[model_amount];
	model_clut_amount = new uint8_t[model_amount];
	model_unknown3 = new uint32_t[model_amount];
	clut_unknown1 = new uint32_t*[model_amount];
	clut_unknown2 = new uint32_t*[model_amount];
	clut_unknown3 = new uint32_t*[model_amount];
	clut_unknown4 = new uint32_t*[model_amount];
	memcpy(model_unknown1,from.model_unknown1,model_amount*sizeof(uint32_t));
	memcpy(model_cluts_offset,from.model_cluts_offset,model_amount*sizeof(uint16_t));
	memcpy(model_unknown2,from.model_unknown2,model_amount*sizeof(uint8_t));
	memcpy(model_clut_amount,from.model_clut_amount,model_amount*sizeof(uint8_t));
	memcpy(model_unknown3,from.model_unknown3,model_amount*sizeof(uint32_t));
	for (i=0;i<model_amount;i++) {
		clut_unknown1[i] = new uint32_t[model_clut_amount[i]];
		clut_unknown2[i] = new uint32_t[model_clut_amount[i]];
		clut_unknown3[i] = new uint32_t[model_clut_amount[i]];
		clut_unknown4[i] = new uint32_t[model_clut_amount[i]];
		memcpy(clut_unknown1[i],from.clut_unknown1[i],model_clut_amount[i]*sizeof(uint32_t));
		memcpy(clut_unknown2[i],from.clut_unknown2[i],model_clut_amount[i]*sizeof(uint32_t));
		memcpy(clut_unknown3[i],from.clut_unknown3[i],model_clut_amount[i]*sizeof(uint32_t));
		memcpy(clut_unknown4[i],from.clut_unknown4[i],model_clut_amount[i]*sizeof(uint32_t));
	}
}

void ClutDataStruct::UpdateOffset() {
	unsigned int i;
	uint32_t totalsize = 4+0xC*model_amount;
	for (i=0;i<model_amount;i++)
		totalsize += 0x10*model_clut_amount[i];
	SetSize(totalsize);
}
