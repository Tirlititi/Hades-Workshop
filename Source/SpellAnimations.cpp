#include "SpellAnimations.h"

#include <cstring>
#include "Hades_Strings.h"

#define MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(IO,SEEK,FROMPOS,TOOFFSET) \
	tmppos = f.tellg(); \
	SEEK(f,FROMPOS,TOOFFSET); \
	nexttmppos = f.tellg(); \
	f.seekg(tmppos); \
	while (f.tellg()<nexttmppos) \
		IO ## Char(f,padbyte);

#define MACRO_SPELL_ANIM_IOFUNCTION_HEADER(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,chunk_amount); \
	if (READ) { \
		chunk_index.resize(chunk_amount); \
		chunk_entry_amount.resize(chunk_amount); \
		chunk_entry_id.resize(chunk_amount); \
		chunk_entry_info.resize(chunk_amount); \
		chunk_entry_size.resize(chunk_amount); \
		chunk_entry_pos.resize(chunk_amount); \
		size = 1; \
	} \
	for (i=0;i<chunk_amount;i++) { \
		IO ## Short(f,chunk_index[i]); \
		IO ## Short(f,chunk_entry_amount[i]); \
		if (READ) { \
			chunk_entry_id[i].resize(chunk_entry_amount[i]); \
			chunk_entry_info[i].resize(chunk_entry_amount[i]); \
			chunk_entry_size[i].resize(chunk_entry_amount[i]); \
			chunk_entry_pos[i].resize(chunk_entry_amount[i]); \
		} \
		for (j=0;j<chunk_entry_amount[i];j++) { \
			if (READ) chunk_entry_pos[i][j] = 0x800*size; \
			IO ## Char(f,chunk_entry_id[i][j]); \
			IO ## Char(f,chunk_entry_info[i][j]); \
			IO ## Short(f,chunk_entry_size[i][j]); \
			size += chunk_entry_size[i][j]; \
			if (chunk_entry_id[i][j]==2 && chunk_index[i]==0) { \
				IO ## Short(f,chunk_entry2_additional_size); \
				size += chunk_entry2_additional_size; \
			} \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(IO,SEEK) \
	SpellAnimSequenceCodeLine seqtmp; \
	seq_code.clear(); \
	do { \
		IO ## Char(f,seqtmp.code); \
		for (i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++) \
			IO ## Char(f,seqtmp.arg[i]); \
		seqtmp.parent = this; \
		seq_code.push_back(seqtmp); \
	} while (seqtmp.code!=0); \
	seq_code_amount = seq_code.size();

#define MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(IO,SEEK,PPF) \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<seq_code_amount;i++) { \
		IO ## Char(f,seq_code[i].code); \
		for (j=0;j<SPELL_ANIMATION_SEQUENCE_ARG;j++) \
			IO ## Char(f,seq_code[i].arg[j]); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_ANIM_IOFUNCTION_IMAGE(IO,SEEK,READ,PPF) \
	chunkpos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,img.offset_list); \
	IO ## Long(f,img.offset_palette); \
	IO ## Long(f,img.palette_tim_amount); \
	IO ## Short(f,img.small_palette_amount); \
	IO ## Short(f,img.palette_amount); \
	if (READ) { \
		img.palette_tim.resize(img.palette_tim_amount); \
		img.small_palette_key.resize(img.small_palette_amount); \
		img.palette_key.resize(img.palette_amount); \
	} \
	for (i=0;i<img.small_palette_amount;i++) \
		IO ## Short(f,img.small_palette_key[i]); \
	for (i=0;i<img.palette_amount;i++) \
		IO ## Short(f,img.palette_key[i]); \
	if ((img.small_palette_amount+img.palette_amount) % 2) \
		IO ## Short(f,img.padding1); \
	else if (READ) \
		img.padding1 = 0xFF; \
	IO ## Long(f,img.offset_pixel); \
	IO ## Long(f,img.tim_amount); \
	if (READ) img.tim.resize(img.tim_amount); \
	for (i=0;i<img.tim_amount;i++) { \
		IO ## Short(f,img.tim[i].pos_x); \
		IO ## Short(f,img.tim[i].pos_y); \
		IO ## Short(f,img.tim[i].width); \
		IO ## Short(f,img.tim[i].height); \
		if (READ) { \
			img.tim[i].format = 0x2; \
			img.tim[i].img_size = img.tim[i].width*img.tim[i].height*2+0xC; \
			img.tim[i].pixel_value = new uint8_t[img.tim[i].img_size-0xC]; \
		} \
	} \
	for (i=0;i<img.palette_tim_amount;i++) { \
		IO ## Short(f,img.palette_tim[i].pos_x); \
		IO ## Short(f,img.palette_tim[i].pos_y); \
		IO ## Short(f,img.palette_tim[i].width); \
		IO ## Short(f,img.palette_tim[i].height); \
		if (READ) { \
			img.palette_tim[i].format = 0x2; \
			img.palette_tim[i].img_size = img.palette_tim[i].width*img.palette_tim[i].height*2+0xC; \
			img.palette_tim[i].pixel_value = new uint8_t[img.palette_tim[i].img_size-0xC]; \
		} \
		for (j=0;j<img.palette_tim[i].img_size-0xC;j++) \
			IO ## Char(f,img.palette_tim[i].pixel_value[j]); \
	} \
	x = 0; \
	for (i=0;i<img.tim_amount;i++) { \
		for (j=0;j<img.tim[i].img_size-0xC;j++) { \
			if (img.tim_chunk_size == x++) { \
				if (READ) { \
					SEEK(f,headerpos,img.linked_chunk_entry_pos); \
				} else { \
					padbyte = 0x55; \
					MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(IO,SEEK,headerpos,img.linked_chunk_entry_pos) \
					padbyte = 0xFF; \
				} \
			} \
			IO ## Char(f,img.tim[i].pixel_value[j]); \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_ANIM_IOFUNCTION_RESOURCE(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(f); \
	chunkpos = f.tellg(); \
	if (READ) res.offset.push_back(0); \
	IO ## Long(f,res.offset[0]); \
	if (READ) { \
		res.amount = res.offset[0]/4-2; \
		res.offset.resize(res.amount); \
		res.type.resize(res.amount); \
		res.data.resize(res.amount); \
	} \
	for (i=0;i<res.amount;i++) { \
		IO ## Long(f,res.offset[i]); \
		if (READ && ((res.offset[i] & 0xFFFF0000) || res.unknown.size()>0)) \
			res.unknown.push_back(res.offset[i]); \
	} \
	if (READ) { \
		res.amount -= res.unknown.size(); \
		res.offset.resize(res.amount); \
		res.type.resize(res.amount); \
		res.data.resize(res.amount); \
		if (res.unknown.size()!=0) { \
			res.unknown.push_back(0); \
			IO ## Long(f,res.unknown[res.unknown.size()-1]); \
		} \
	} else { \
		for (i=0;i<res.unknown.size();i++) \
			IO ## Long(f,res.unknown[i]); \
	} \
	if (res.unknown.size()==0) \
		IO ## Long(f,res.offset.back()); \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<res.amount;i++) { \
		if (i+1<res.offset.size()) { \
			SEEK(f,chunkpos,res.offset[i]); \
			if (PPF) PPFInitScanStep(f); \
			res.type[i] = SpellDataResourceType::AKAO; \
			res.data[i] = new SpellAnimResourceAKAO[1]; \
			SpellAnimResourceAKAO& akao = *static_cast<SpellAnimResourceAKAO*>(res.data[i]); \
			akao.raw.resize(res.offset[i+1]-res.offset[i]); \
			for (j=0;j<akao.raw.size();j++) \
				IO ## Char(f,akao.raw[j]); \
			if (PPF) PPFEndScanStep(); \
		} else { \
			res.type[i] = SpellDataResourceType::END; \
			res.data[i] = NULL; \
		} \
	}


int debugimgspellcounter = 0;
void SpellAnimationDataStruct::Read(fstream& f) {
	int debugimgcounter = 0; // DEBUG
	uint32_t headerpos = f.tellg();
	uint32_t chunkpos;
	uint32_t tmppos,nexttmppos;
	uint8_t padbyte = 0xFF;
	unsigned int i,j,x,y;
	unsigned int chunki, chunkj;
//fstream fout("aaaa.txt",ios::app|ios::out); fout << "NEW " << debugimgspellcounter << endl;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(FFIXRead,FFIXSeek,true,false)
		FFIXSeek(f,headerpos,0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(FFIXRead,FFIXSeek)
		FFIXSeek(f,headerpos,0x800);
	} else {
		raw_data = new uint8_t[raw_size];
		f.read((char*)raw_data,raw_size);
		SteamSeek(f,headerpos,0);
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(SteamRead,SteamSeek,true,false)
		SteamSeek(f,headerpos,0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(SteamRead,SteamSeek)
/*		for (chunki=0;chunki<chunk_amount;chunki++)
			for (chunkj=0;chunkj<chunk_entry_amount[chunki];chunkj++) {
				SteamSeek(f,headerpos,chunk_entry_pos[chunki][chunkj]);
//fout << "CHUNK " << chunkj << " (" << (int)chunk_entry_id[chunki][chunkj] << ") " << (long)f.tellg() << endl;
				if (chunk_entry_id[chunki][chunkj]==0) {
					SpellAnimImage img;
					img.tim_chunk_size = 0x4000*chunk_entry_info[chunki][chunkj];
					img.linked_chunk_entry_pos = 0;
					for (j=0;j<chunk_entry_amount[chunki];j++)
						if (j!=chunkj && chunk_entry_id[chunki][j]==1) {
							img.linked_chunk_entry_pos = chunk_entry_pos[chunki][j];
							break;
						}
					MACRO_SPELL_ANIM_IOFUNCTION_IMAGE(SteamRead,SteamSeek,true,false)
					image.push_back(img);
//					unsigned int minx=1024,maxx=0,miny=512,maxy=0;
//					for (i=0;i<img.tim_amount;i++) {
//						img.tim[i].LoadInVRam(false);
//						if (minx>img.tim[i].pos_x) minx = img.tim[i].pos_x;
//						if (miny>img.tim[i].pos_y) miny = img.tim[i].pos_y;
//						if (maxx<img.tim[i].pos_x+img.tim[i].width) maxx = img.tim[i].pos_x+img.tim[i].width;
//						if (maxy<img.tim[i].pos_y+img.tim[i].height) maxy = img.tim[i].pos_y+img.tim[i].height;
//					}
//					if (img.tim_amount>0) {
//						uint32_t pixelrgba;
//						minx *= 2;
//						maxx *= 2;
//						unsigned int w = maxx-minx;
//						unsigned int h = maxy-miny;
//						debugimgcounter = 0;
//						for (i=0;i<img.palette_amount;i++) {
//							uint32_t* pal = img.GetPaletteFromKey(img.palette_key[i],false);
//							unsigned char* imgrgb = (unsigned char*)malloc(3*w*h*sizeof(unsigned char));
//							unsigned char* imgalpha = (unsigned char*)malloc(w*h*sizeof(unsigned char));
//							x = 0;
//							y = 0;
//							for (j=0;j<w*h;j++) {
//								pixelrgba = TIMImageDataStruct::GetVRamPixel(minx+x,miny+y,pal);
//								imgrgb[3*j] = pixelrgba & 0xFF;
//								imgrgb[3*j+1] = (pixelrgba >> 8) & 0xFF;
//								imgrgb[3*j+2] = (pixelrgba >> 16) & 0xFF;
//								imgalpha[j] = pixelrgba >> 24;
//								x++;
//								if (x>=w) {
//									x = 0;
//									y++;
//								}
//							}
//							wxImage img(w,h,imgrgb,imgalpha);
//							img.SaveFile("ExportSpellImg"+wxString::Format("%u_%u_%u.png",debugimgspellcounter,image.size()-1,debugimgcounter),wxBITMAP_TYPE_PNG);
//							debugimgcounter++;
//						}
//						minx *= 2;
//						maxx *= 2;
//						w = maxx-minx;
//						h = maxy-miny;
//						debugimgcounter = 0;
//						for (i=0;i<img.small_palette_amount;i++) {
//							uint32_t* pal = img.GetPaletteFromKey(img.small_palette_key[i],true);
//							unsigned char* imgrgb = (unsigned char*)malloc(3*w*h*sizeof(unsigned char));
//							unsigned char* imgalpha = (unsigned char*)malloc(w*h*sizeof(unsigned char));
//							x = 0;
//							y = 0;
//							for (j=0;j<w*h;j++) {
//								pixelrgba = TIMImageDataStruct::GetVRamPixel(minx+x,miny+y,pal,true);
//								imgrgb[3*j] = pixelrgba & 0xFF;
//								imgrgb[3*j+1] = (pixelrgba >> 8) & 0xFF;
//								imgrgb[3*j+2] = (pixelrgba >> 16) & 0xFF;
//								imgalpha[j] = pixelrgba >> 24;
//								x++;
//								if (x>=w) {
//									x = 0;
//									y++;
//								}
//							}
//							wxImage img(w,h,imgrgb,imgalpha);
//							img.SaveFile("ExportSpellImg"+wxString::Format("%u_%u_small%u.png",debugimgspellcounter,image.size()-1,debugimgcounter),wxBITMAP_TYPE_PNG);
//							debugimgcounter++;
//						}
//					}
				} else if (chunk_entry_id[chunki][chunkj]==2) {
					if (chunk_index[chunki]==0)
						SteamSeek(f,headerpos,chunk_entry_pos[chunki][chunkj]+0x800*chunk_entry2_additional_size);
					SpellAnimResourceSet res;
					MACRO_SPELL_ANIM_IOFUNCTION_RESOURCE(SteamRead,SteamSeek,true,false)
//if (debugimgspellcounter==24) {
//for (i=0;i<res.amount;i++) if (res.type[i]==SpellDataResourceType::AKAO) {
//SpellAnimResourceAKAO& akaodata = *static_cast<SpellAnimResourceAKAO*>(res.data[i]);
//fstream foutbin((wxString("ExportSpellRes")+wxString::Format("%u_%u%u_%u",debugimgspellcounter,chunki,chunkj,i)).c_str().AsChar(),ios::out|ios::binary);
//for (j=0;j<akaodata.raw.size();j++) foutbin.put(akaodata.raw[j]);
//foutbin.close();
//}}
					resource.push_back(res);
				} else if (chunk_entry_id[chunki][chunkj]==3) {
//					uint32_t debugbuf;
//fstream foutbin((wxString("ExportSpellAnim")+wxString::Format("%u_%u_%u",debugimgspellcounter,chunki,chunk_entry_info[chunki][chunkj],i)).c_str().AsChar(),ios::out|ios::binary);
//					for (i=0;i<0x200*chunk_entry_size[chunki][chunkj];i++) {
//						SteamReadLong(f,debugbuf);
//						if (debugbuf==0xFFFFFFFF)
//							break;
//SteamWriteLong(foutbin,debugbuf);
//					}
//foutbin.close();
				}
			}
fout.close();*/
	}
	modified_data = 0;
}

void SpellAnimationDataStruct::Write(fstream& f) {
	uint32_t chunkpos,tmppos,nexttmppos;
	uint32_t headerpos = f.tellg();
	uint8_t padbyte = 0xFF;
	unsigned int i,j,x,y;
//fstream fout("aaaa.txt",ios::app|ios::out);
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(FFIXWrite,FFIXSeek,false,false)
		MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(FFIXWrite,FFIXSeek,headerpos,0x400)
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(FFIXWrite,FFIXSeek,false)
		MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(FFIXWrite,FFIXSeek,headerpos,0x800)
		modified_data = 0;
	} else {
		f.write((char*)raw_data,raw_size);
		f.seekg(headerpos);
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(SteamWrite,SteamSeek,false,false)
		MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(SteamWrite,SteamSeek,headerpos,0x400)
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(SteamWrite,SteamSeek,false)
		MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(SteamWrite,SteamSeek,headerpos,0x800)
	}
}

void SpellAnimationDataStruct::WritePPF(fstream& f) {
	uint32_t chunkpos,tmppos,nexttmppos;
	uint32_t headerpos = f.tellg();
	uint8_t padbyte = 0xFF;
	unsigned int i,j,x,y;
//fstream fout("aaaa.txt",ios::app|ios::out);
	MACRO_SPELL_ANIM_IOFUNCTION_HEADER(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(PPFStepAdd,FFIXSeek,headerpos,0x400)
	MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(PPFStepAdd,FFIXSeek,true)
	MACRO_SPELL_ANIM_IOFUNCTION_FFFILL(PPFStepAdd,FFIXSeek,headerpos,0x800)
}

void SpellAnimationDataStruct::ReadHWS(fstream& f) {
	Spell_Animation_Data_Type hwsmodif;
	uint32_t chunkpos,tmppos,nexttmppos;
	uint32_t headerpos = f.tellg();
	uint8_t padbyte = 0xFF;
	unsigned int i,j,x,y;
//fstream fout("aaaa.txt",ios::app|ios::out);
	HWSReadLong(f,hwsmodif);
	if (hwsmodif & SPELL_ANIMATION_DATA_TYPE_HEADER) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(HWSRead,HWSSeek,true,false)
	}
	if (hwsmodif & SPELL_ANIMATION_DATA_TYPE_SEQUENCE) {
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(HWSRead,HWSSeek)
	}
	modified_data |= hwsmodif;
}

void SpellAnimationDataStruct::WriteHWS(fstream& f) {
	uint32_t chunkpos,tmppos,nexttmppos;
	uint32_t headerpos = f.tellg();
	uint8_t padbyte = 0xFF;
	unsigned int i,j,x,y;
//fstream fout("aaaa.txt",ios::app|ios::out);
	HWSWriteLong(f,modified_data);
	if (modified_data & SPELL_ANIMATION_DATA_TYPE_HEADER) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(HWSWrite,HWSSeek,false,false)
	}
	if (modified_data & SPELL_ANIMATION_DATA_TYPE_SEQUENCE) {
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(HWSWrite,HWSSeek,false)
	}
}

void SpellAnimationDataStruct::Copy(SpellAnimationDataStruct& from) {
	unsigned int i;
	*this = from;
	for (i=0;i<seq_code_amount;i++)
		memcpy(seq_code[i].arg,from.seq_code[i].arg,SPELL_ANIMATION_SEQUENCE_ARG*sizeof(uint8_t));
}

bool SpellAnimationDataStruct::AddAnimationSequenceCode(unsigned int codeline, uint8_t newcode) {
	if (seq_code_amount>=0x155)
		return false;
	SpellAnimSequenceCodeLine newseq;
	newseq.parent = this;
	newseq.code = newcode;
	for (unsigned int i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++)
		newseq.arg[i] = 0;
	seq_code.insert(seq_code.begin()+codeline,newseq);
	seq_code_amount = seq_code.size();
	return true;
}

void SpellAnimationDataStruct::DeleteAnimationSequenceCode(unsigned int codeline) {
	seq_code.erase(seq_code.begin()+codeline);
	seq_code_amount = seq_code.size();
}

void SpellAnimationDataStruct::ChangeAnimationSequenceCode(unsigned int codeline, uint8_t newcode) {
	seq_code[codeline].code = newcode;
	for (unsigned int i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++)
		seq_code[codeline].arg[i] = 0;
}

// Assume there's only 1 spell_dir
void SpellAnimationDataSet::Load(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	amount = GetGameType()==GAME_TYPE_PSX ? globalmap.spell_dir->file_amount : SPELL_ANIMATION_AMOUNT;
	spell = new SpellAnimationDataStruct[amount];
	if (GetGameType()==GAME_TYPE_PSX) {
		for (i=0;i<amount;i++) {
			spell[i].id = i;
			spell[i].parent = this;
			if (globalmap.spell_dir->file_offset[i]!=0xFFFF) {
				spell[i].is_empty = false;
				ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
				spell[i].Read(ffbin);
			} else
				spell[i].is_empty = true;
		}
	} else {
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<amount;i++) {
			spell[i].id = i;
			spell[i].parent = this;
			if (config.spellanim_steam_file[i]>=0) {
debugimgspellcounter = i; // DEBUG
				spell[i].is_empty = false;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spellanim_steam_file[i]));
				spell[i].raw_size = config.meta_res.GetFileSizeByIndex(config.spellanim_steam_file[i]);
				spell[i].Read(ffbin);
			} else
				spell[i].is_empty = true;
		}
		ffbin.close();
	}
}

void SpellAnimationDataSet::Write(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty) {
			ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
			spell[i].Write(ffbin);
		}
	}
}

void SpellAnimationDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty) {
			ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
			spell[i].WritePPF(ffbin);
		}
	}
}

int* SpellAnimationDataSet::LoadHWS(fstream& ffhws) {
	uint32_t spam,tmp32;
	uint16_t spamid,i;
	int* res = new int[1];
	res[0] = 0;
	HWSReadLong(ffhws,tmp32); // spell_dir amount
	HWSReadLong(ffhws,tmp32); // spell_dir ID
	HWSReadLong(ffhws,spam);
	for (i=0;i<spam;i++) {
		HWSReadShort(ffhws,spamid);
		spell[spamid].ReadHWS(ffhws);
	}
	return res;
}

void SpellAnimationDataSet::WriteHWS(fstream& ffhws) {
	uint32_t spam = 0;
	uint16_t i;
	HWSWriteLong(ffhws,1); // spell_dir amount
	HWSWriteLong(ffhws,0); // spell_dir ID
	uint32_t spampos = ffhws.tellg();
	HWSWriteLong(ffhws,0); // amount of spell animations
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty && spell[i].modified_data) {
			HWSWriteShort(ffhws,i);
			spell[i].WriteHWS(ffhws);
			spam++;
		}
	}
	uint32_t endpos = ffhws.tellg();
	HWSSeek(ffhws,spampos,0);
	HWSWriteLong(ffhws,spam);
	HWSSeek(ffhws,endpos,0);
}

uint32_t* SpellAnimImage::GetPaletteFromKey(uint16_t key, bool shortformat) {
	uint16_t palsize = (shortformat ? 16 : 256)/2;
	uint16_t palx = (key & 0x3F)*32;
	uint16_t paly = (key >> 6) & 0x1FF;
	uint16_t palxshift = palx;
	unsigned int i;
	for (i=0;i<palette_tim_amount;i++) {
		if (shortformat)
			palxshift = palx/2+palette_tim[i].pos_x/2;
		if (!(palette_tim[i].pos_x>palxshift || palette_tim[i].pos_x+palette_tim[i].width<=palxshift+palsize || palette_tim[i].pos_y>paly || palette_tim[i].pos_y+palette_tim[i].height<=paly))
			return palette_tim[i].ConvertAsPalette(key,false,shortformat);
	}
	return NULL;
}
