#include "MenuUI.h"

#include <wx/string.h>
#include <wx/translation.h>

#define MENUUI_HWS_VERSION 1


bool AtlasDataStruct::LoadAtlas(fstream& archive, UnityArchiveMetaData& meta, unsigned int img, unsigned int spt) {
	uint8_t* imgbuffer, *imgrgba;
	uint32_t imgw, imgh;
	uint32_t imgfilesize = meta.GetFileSizeByIndex(img);
	archive.seekg(meta.GetFileOffsetByIndex(img));
	imgbuffer = new uint8_t[imgfilesize];
	archive.read((char*)imgbuffer,imgfilesize);
	bool success = TIMImageDataStruct::ConvertFromSteamTexture(imgbuffer,&imgw,&imgh,&imgrgba);
	delete[] imgbuffer;
	if (!success)
		return false;
	uint32_t spritex,spritey,spritew,spriteh;
	uint32_t spritecount,spritenamelen;
	wxString spritename;
	wxImage spriteimg;
	unsigned int imgpixindex;
	unsigned int i,j,k;
	archive.seekg(meta.GetFileOffsetByIndex(spt)+0x2C);
	spritecount = ReadLong(archive);
	for (i=0;i<spritecount;i++) {
		spritenamelen = ReadLong(archive);
		spritename = _(L"");
		for (j=0;j<spritenamelen;j++)
			spritename += archive.get();
		archive.seekg(GetAlignOffset(archive.tellg()),ios::cur);
		spritex = ReadLong(archive);
		spritey = ReadLong(archive);
		spritew = ReadLong(archive);
		spriteh = ReadLong(archive);
		spriteimg.Create(spritew,spriteh);
		spriteimg.SetAlpha();
		for (j=0;j<spritew;j++)
			for (k=0;k<spriteh;k++) {
				imgpixindex = (spritex+j+(spritey+k)*imgw)*4;
				spriteimg.SetRGB(j,k,imgrgba[imgpixindex],imgrgba[imgpixindex+1],imgrgba[imgpixindex+2]);
				spriteimg.SetAlpha(j,k,imgrgba[imgpixindex+3]);
			}
		sprite.insert(pair<wxString,wxBitmap>(spritename,wxBitmap(spriteimg)));
		archive.seekg(0x20,ios::cur);
	}
	delete[] imgrgba;
	return true;
}

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
