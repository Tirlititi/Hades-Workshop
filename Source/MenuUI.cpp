#include "MenuUI.h"

#define MENUUI_HWS_VERSION 1


void MenuUIDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	special_text = new SpecialTextDataSet();
	special_text->Load(ffbin,config);
}

void MenuUIDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	special_text->Write(ffbin,config);
}

void MenuUIDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	special_text->WritePPF(ffbin,config);
}

int* MenuUIDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart* backup, bool usetext) {
	int* res = new int[2];
	uint16_t version;
	uint32_t txtsize;
	HWSReadShort(ffhws,version);
	HWSReadLong(ffhws,txtsize);
	if (usetext && txtsize>0) {
		int* textres = special_text->LoadHWS(ffhws,backup[0]);
		res[0] = textres[0];
		res[1] = textres[1];
		delete[] textres;
	} else {
		ffhws.seekg(txtsize,ios::cur);
		res[0] = 0;
		res[1] = 0;
	}
	return res;
}

void MenuUIDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart* backup) {
	uint32_t txtpos, txtsizepos, txtsize = 0;
	HWSWriteShort(ffhws,MENUUI_HWS_VERSION);
	txtsizepos = ffhws.tellg();
	HWSWriteLong(ffhws,txtsize);
	if (special_text->modified) {
		txtpos = ffhws.tellg();
		special_text->WriteHWS(ffhws,backup[0]);
		txtsize = (uint32_t)ffhws.tellg()-txtpos;
		txtpos = ffhws.tellg();
		ffhws.seekg(txtsizepos);
		HWSWriteLong(ffhws,txtsize);
		ffhws.seekg(txtpos);
	}
}
