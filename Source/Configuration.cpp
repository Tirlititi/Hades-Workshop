#include "Configuration.h"

#include <algorithm>
#include <sstream>
#include <cstring>
#include <string>
#include "main.h"
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"
#include "Database_Steam.h"
#include "Database_Resource.h"
#include "Database_CSV.h"
using namespace std;

SaveSet::SaveSet(SpellDataSet* sp, CommandDataSet* cmd, EnemyDataSet* enmy, ShopDataSet* shop, TextDataSet* text,/*
			*/  WorldMapDataSet* world, BattleSceneDataSet* scene, ItemDataSet* item, SupportDataSet* support,/*
			*/  StatDataSet* stat, CardDataSet* card, FieldDataSet* field, SpellAnimationDataSet* spellanim,/*
			*/  MenuUIDataSet* ui, PartySpecialDataSet* partyspecial, MipsDataSet* mips, CILDataSet* cil) :
	spellset(sp),
	cmdset(cmd),
	enemyset(enmy),
	shopset(shop),
	textset(text),
	worldset(world),
	sceneset(scene),
	itemset(item),
	supportset(support),
	statset(stat),
	cardset(card),
	fieldset(field),
	spellanimset(spellanim),
	ffuiset(ui),
	partyspecialset(partyspecial),
	mipsset(mips),
	cilset(cil) {
}

GameType TheGameType;
GameType TheHWSGameType;

GameType GetGameType() {
	return TheGameType;
}
void SetGameType(GameType gt) {
	TheGameType = gt;
}
GameType GetHWSGameType() {
	return TheHWSGameType;
}
void SetHWSGameType(GameType gt) {
	TheHWSGameType = gt;
}

//==================================================//
//                  CONFIG FILES                    //
//==================================================//
#define CONFIG_EXT ".hwf"
static string CONFIG_HEADER = "HAWO";
#define CONFIG_VERSION (uint16_t)0x000C
#define CONFIG_HEADERSIZE (uint16_t)3

#define G_N_ELEMENTS(arr) ((sizeof(arr))/(sizeof(arr[0])))
static string CONFIG_END_SECTIONS = "ENDC";
static string CONFIG_SECTIONS[] = {
	"SPEL", "CMDS", "CLUS", "SHOP", "ITEM", "SUPP", "STAT", "CARD", "MENU", "PASP", "MIPS"
};

string GetFileConfigurationPath(string filepath) {
	if (filepath[filepath.length()-4]=='.')
		filepath.replace(filepath.length()-4,4,CONFIG_EXT);
	else
		filepath += CONFIG_EXT;
	return filepath;
}

/////////////
// Reading //
/////////////
template <typename Word>
void read_word(fstream& f, Word& value) {
	value=0;
	for (unsigned size=0;size<sizeof(Word);++size)
		value |= f.get()<<(8*size);
}

void ConfigReadSectionSpell(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	for (i=0;i<5;i++)
		read_word(cf,dest.spell_data_offset[i]);
	read_word(cf,dest.spell_naming_rules_offset);
	read_word(cf,dest.spell_text_offset);
	read_word(cf,dest.spell_name_space_total);
	read_word(cf,dest.spell_help_space_total);
}

void ConfigReadSectionCommand(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	for (i=0;i<3;i++)
		read_word(cf,dest.cmd_data_offset[i]);
	read_word(cf,dest.cmd_name_space_total);
	read_word(cf,dest.cmd_help_space_total);
}

void ConfigReadSectionCluster(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.cluster_amount);
	dest.cluster_offset = new uint32_t[dest.cluster_amount];
	for (i=0;i<dest.cluster_amount;i++)
		read_word(cf,dest.cluster_offset[i]);
}

void ConfigReadSectionShop(fstream& cf, ConfigurationSet& dest) {
	read_word(cf,dest.shop_data_offset);
	read_word(cf,dest.synthesis_data_offset);
}

void ConfigReadSectionItem(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.item_data_offset);
	read_word(cf,dest.item_stat_offset);
	read_word(cf,dest.item_weaponstat_offset);
	for (i=0;i<4;i++)
		read_word(cf,dest.item_help_offset[i]);
	read_word(cf,dest.item_name_space_total);
	read_word(cf,dest.item_help_space_total);
	read_word(cf,dest.item_help_space2_total);
	read_word(cf,dest.item_key_name_space_total);
	read_word(cf,dest.item_key_help_space_total);
	read_word(cf,dest.item_key_desc_space_total);
}

void ConfigReadSectionSupport(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.support_data_offset);
	for (i=0;i<4;i++)
		read_word(cf,dest.support_text_offset[i]);
	read_word(cf,dest.support_name_space_total);
	read_word(cf,dest.support_help_space_total);
}

void ConfigReadSectionStat(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.stat_defaultname_offset);
	read_word(cf,dest.stat_initialequip_offset);
	read_word(cf,dest.stat_initialstat_offset);
	for (i=0;i<3;i++)
		read_word(cf,dest.stat_cmddata_offset[i]);
	read_word(cf,dest.stat_defaultname_space_total);
}

void ConfigReadSectionCard(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.card_text_offset);
	read_word(cf,dest.card_set_offset);
	read_word(cf,dest.card_deck_offset);
	for (i=0;i<2;i++)
		read_word(cf,dest.card_stat_offset[i]);
	read_word(cf,dest.card_name_space_total);
}

void ConfigReadSectionMenuUI(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	read_word(cf,dest.spetext_amount);
	dest.spetext_offset = new uint32_t[dest.spetext_amount];
	dest.spetext_space_total = new uint16_t[dest.spetext_amount];
	for (i=0;i<dest.spetext_amount;i++) {
		read_word(cf,dest.spetext_offset[i]);
		read_word(cf,dest.spetext_space_total[i]);
	}
}

void ConfigReadSectionPartySpecial(fstream& cf, ConfigurationSet& dest) {
	read_word(cf,dest.party_magicsword_offset);
}

void ConfigReadSectionMips(fstream& cf, ConfigurationSet& dest) {
	read_word(cf,dest.mips_battle_offset_list);
	read_word(cf,dest.mips_battle_code_offset);
	read_word(cf,dest.mips_battle_code_amount);
}

void ConfigReadSection(fstream& cf, ConfigurationSet& dest, unsigned int section) {
	if (section==0)
		ConfigReadSectionSpell(cf,dest);
	else if (section==1)
		ConfigReadSectionCommand(cf,dest);
	else if (section==2)
		ConfigReadSectionCluster(cf,dest);
	else if (section==3)
		ConfigReadSectionShop(cf,dest);
	else if (section==4)
		ConfigReadSectionItem(cf,dest);
	else if (section==5)
		ConfigReadSectionSupport(cf,dest);
	else if (section==6)
		ConfigReadSectionStat(cf,dest);
	else if (section==7)
		ConfigReadSectionCard(cf,dest);
	else if (section==8)
		ConfigReadSectionMenuUI(cf,dest);
	else if (section==9)
		ConfigReadSectionPartySpecial(cf,dest);
	else if (section==10)
		ConfigReadSectionMips(cf,dest);
}

int LoadConfiguration(string filepath, ConfigurationSet& dest) {
	filepath = GetFileConfigurationPath(filepath);
	fstream cf(filepath.c_str(),ios::binary | ios::in);
	if (!cf.is_open())
		return -2;
	char buffer[5];
	buffer[4] = 0;
	cf.read(buffer,4);
	if (CONFIG_HEADER.compare(buffer))
		return -2;
	read_word(cf,dest.version);
	if (dest.version<CONFIG_VERSION)
		return 1;
	read_word(cf,dest.headersize);
	uint32_t datapos = dest.headersize;
	datapos += cf.tellg();
	read_word(cf,dest.cd);
	dest.language = LANGUAGE_VERSION_UNKNOWN;
	dest.gametype = GAME_TYPE_PSX;
	if (dest.version>=0x7)
		read_word(cf,dest.language);
	if (dest.version>=0xB)
		read_word(cf,dest.gametype);
	cf.seekg(datapos);
	unsigned int i;
	cf.read(buffer,4);
	while (!cf.eof() && !cf.fail() && CONFIG_END_SECTIONS.compare(buffer)) {
		read_word(cf,datapos);
		datapos += cf.tellg();
		for (i=0;i<G_N_ELEMENTS(CONFIG_SECTIONS);i++)
			if (!CONFIG_SECTIONS[i].compare(buffer)) {
				ConfigReadSection(cf,dest,i);
				i = G_N_ELEMENTS(CONFIG_SECTIONS);
			}
		cf.seekg(datapos);
		cf.read(buffer,4);
	}
	cf.close();
	return 0;
}

int FileCheckConfig(string filename, ConfigurationSet& config) {
	fstream f(filename.c_str(),fstream::in | fstream::binary);
	if (!f.is_open())
		return -1;
	f.close();
	return LoadConfiguration(filename,config);
}

/////////////
// Writing //
/////////////
template <typename Word>
void write_word(fstream& f, Word value) {
	for (unsigned size=sizeof(Word);size;--size) {
		f.put(static_cast<char>(value & 0xFF));
		value>>=8;
	}
}

void ConfigWriteSectionSpell(fstream& cf, ConfigurationSet& conf) {
	unsigned int i;
	for (i=0;i<5;i++)
		write_word(cf,conf.spell_data_offset[i]);
	write_word(cf,conf.spell_naming_rules_offset);
	write_word(cf,conf.spell_text_offset);
	write_word(cf,conf.spell_name_space_total);
	write_word(cf,conf.spell_help_space_total);
}

void ConfigWriteSectionCommand(fstream& cf, ConfigurationSet& conf) {
	unsigned int i;
	for (i=0;i<3;i++)
		write_word(cf,conf.cmd_data_offset[i]);
	write_word(cf,conf.cmd_name_space_total);
	write_word(cf,conf.cmd_help_space_total);
}

void ConfigWriteSectionCluster(fstream& cf, ConfigurationSet& conf) {
	unsigned int i;
	write_word(cf,conf.cluster_amount);
	for (i=0;i<conf.cluster_amount;i++)
		write_word(cf,conf.cluster_offset[i]);
}

void ConfigWriteSectionShop(fstream& cf, ConfigurationSet& conf) {
	write_word(cf,conf.shop_data_offset);
	write_word(cf,conf.synthesis_data_offset);
}

void ConfigWriteSectionItem(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	write_word(cf,dest.item_data_offset);
	write_word(cf,dest.item_stat_offset);
	write_word(cf,dest.item_weaponstat_offset);
	for (i=0;i<4;i++)
		write_word(cf,dest.item_help_offset[i]);
	write_word(cf,dest.item_name_space_total);
	write_word(cf,dest.item_help_space_total);
	write_word(cf,dest.item_help_space2_total);
	write_word(cf,dest.item_key_name_space_total);
	write_word(cf,dest.item_key_help_space_total);
	write_word(cf,dest.item_key_desc_space_total);
}

void ConfigWriteSectionSupport(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	write_word(cf,dest.support_data_offset);
	for (i=0;i<4;i++)
		write_word(cf,dest.support_text_offset[i]);
	write_word(cf,dest.support_name_space_total);
	write_word(cf,dest.support_help_space_total);
}

void ConfigWriteSectionStat(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	write_word(cf,dest.stat_defaultname_offset);
	write_word(cf,dest.stat_initialequip_offset);
	write_word(cf,dest.stat_initialstat_offset);
	for (i=0;i<3;i++)
		write_word(cf,dest.stat_cmddata_offset[i]);
	write_word(cf,dest.stat_defaultname_space_total);
}

void ConfigWriteSectionCard(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	write_word(cf,dest.card_text_offset);
	write_word(cf,dest.card_set_offset);
	write_word(cf,dest.card_deck_offset);
	for (i=0;i<2;i++)
		write_word(cf,dest.card_stat_offset[i]);
	write_word(cf,dest.card_name_space_total);
}

void ConfigWriteSectionMenuUI(fstream& cf, ConfigurationSet& dest) {
	unsigned int i;
	write_word(cf,dest.spetext_amount);
	for (i=0;i<dest.spetext_amount;i++) {
		write_word(cf,dest.spetext_offset[i]);
		write_word(cf,dest.spetext_space_total[i]);
	}
}

void ConfigWriteSectionPartySpecial(fstream& cf, ConfigurationSet& dest) {
	write_word(cf,dest.party_magicsword_offset);
}

void ConfigWriteSectionMips(fstream& cf, ConfigurationSet& dest) {
	write_word(cf,dest.mips_battle_offset_list);
	write_word(cf,dest.mips_battle_code_offset);
	write_word(cf,dest.mips_battle_code_amount);
}

void ConfWriteSection(fstream& cf, ConfigurationSet& conf, unsigned int section) {
	if (section==0)
		ConfigWriteSectionSpell(cf,conf);
	else if (section==1)
		ConfigWriteSectionCommand(cf,conf);
	else if (section==2)
		ConfigWriteSectionCluster(cf,conf);
	else if (section==3)
		ConfigWriteSectionShop(cf,conf);
	else if (section==4)
		ConfigWriteSectionItem(cf,conf);
	else if (section==5)
		ConfigWriteSectionSupport(cf,conf);
	else if (section==6)
		ConfigWriteSectionStat(cf,conf);
	else if (section==7)
		ConfigWriteSectionCard(cf,conf);
	else if (section==8)
		ConfigWriteSectionMenuUI(cf,conf);
	else if (section==9)
		ConfigWriteSectionPartySpecial(cf,conf);
	else if (section==10)
		ConfigWriteSectionMips(cf,conf);
}

int WriteConfiguration(string filepath, ConfigurationSet& conf) {
	filepath = GetFileConfigurationPath(filepath);
	fstream cf(filepath.c_str(),ios::binary | ios::out);
	if (!cf.is_open())
		return -1;
	cf << CONFIG_HEADER.c_str();
	write_word(cf,CONFIG_VERSION);
	write_word(cf,CONFIG_HEADERSIZE);
	// Version 0x0001
	write_word(cf,conf.cd); // size: 1
	// Version 0x0007
	write_word(cf,conf.language); // 1
	// Version 0x000B
	write_word(cf,conf.gametype); // 1
	unsigned int i;
	uint32_t poss,pose;
	for (i=0;i<G_N_ELEMENTS(CONFIG_SECTIONS);i++) {
		cf << CONFIG_SECTIONS[i];
		poss = cf.tellg();
		cf.seekg(4,ios::cur);
		ConfWriteSection(cf,conf,i);
		pose = cf.tellg();
		cf.seekg(poss);
		write_word(cf,pose-poss-4);
		cf.seekg(pose);
	}
	cf << CONFIG_END_SECTIONS;
	cf.close();
	return 0;
}

/////////////
// Finding //
/////////////
#define BIN_HEADER 0x00009318L
#define SLES_HEADER 0x0000E5C8L
#define CLUSTER_MAX_AMOUNT 2000
ClusterData TMP_CLUSTER;

// data is assumed to be a 4 bytes aligned offset
int ConfigFindSingle(fstream& f, uint32_t data[], unsigned int amount, uint32_t maxpos) {
	unsigned int j;
	uint32_t tmp32;
	while (f.tellg()<=maxpos && !f.eof())
		if (FFIXReadLongAlign(f,tmp32)==data[0]) {
			for (j=1;j<amount;j++)
				if (FFIXReadLongAlign(f,tmp32)!=data[j])
					break;
			if (j==amount)
				return 0;
		}
	f.close();
	return 1;
}

int FindConfiguration(string filepath, ConfigurationSet& dest) {
	fstream f(filepath.c_str(),ios::binary | ios::in);
	if (!f.is_open())
		return -1;
	uint32_t fsize;
	uint32_t maxpos;
	unsigned int i;
	uint8_t tmp8;
	uint16_t tmp16;
	uint16_t glaststr;
	uint32_t tmp32,counter;
	f.seekg(0,ios::end);
	fsize = f.tellg();
	if (fsize<SLES_HEADER)
		return -2;
	// (SLES FILE)
	//-- Get sure the SLES file header is not RNC-compressed : some deprotection patch does that
	f.seekg(SLES_HEADER);
	maxpos = IMG_HEADER_OFFSET;
	if (fsize<maxpos)
		return -2;
	static uint32_t rnch = 0x01434E52L;
	while (f.tellg()<=SLES_HEADER+FILE_IGNORE_DATA_PERIOD && !f.eof())
		if (FFIXReadLongAlign(f,tmp32)==rnch) {
			f.close();
			return -3;
		}
	// (IMG HEADER)
	//-- Read the .img header to have some determining offsets
	FFIXSeek(f,IMG_HEADER_OFFSET,0x8);
	uint32_t metadataamount;
	FFIXReadLong(f,metadataamount);
	FFIXReadChar(f,dest.cd);
	FFIXReadChar(f,tmp8);
	FFIXReadShort(f,tmp16);
	uint32_t* metadatatype = new uint32_t[metadataamount];
	uint32_t* metadatadataamount = new uint32_t[metadataamount];
	uint32_t* metadatasector = new uint32_t[metadataamount];
	uint32_t* metafirstdatasector = new uint32_t[metadataamount];
	for (i=0;i<metadataamount;i++) {
		FFIXReadLong(f,metadatatype[i]);
		FFIXReadLong(f,metadatadataamount[i]);
		FFIXReadLong(f,metadatasector[i]);
		FFIXReadLong(f,metafirstdatasector[i]);
	}
	if (fsize<IMG_HEADER_OFFSET+metadatasector[metadataamount-1]*FILE_IGNORE_DATA_PERIOD)
		return -2;
	f.seekg(IMG_HEADER_OFFSET+metadatasector[0]*FILE_IGNORE_DATA_PERIOD);
	uint16_t* firstmetadataid = new uint16_t[metadatadataamount[0]];
	uint16_t* firstmetadatatype = new uint16_t[metadatadataamount[0]];
	uint32_t* firstmetadatasector = new uint32_t[metadatadataamount[0]];
	for (i=0;i<metadatadataamount[0];i++) {
		FFIXReadShort(f,firstmetadataid[i]);
		FFIXReadShort(f,firstmetadatatype[i]);
		FFIXReadLong(f,firstmetadatasector[i]);
	}
	/* DEBUG: Exporting AKAO sounds...
	f.seekg(IMG_HEADER_OFFSET+metadatasector[11]*FILE_IGNORE_DATA_PERIOD);
	uint16_t* dbgsoundid = new uint16_t[metadatadataamount[11]];
	uint16_t* dbgsoundtype = new uint16_t[metadatadataamount[11]];
	uint32_t* dbgsoundsector = new uint32_t[metadatadataamount[11]];
	for (i=0;i<metadatadataamount[11];i++) {
		FFIXReadShort(f,dbgsoundid[i]);
		FFIXReadShort(f,dbgsoundtype[i]);
		FFIXReadLong(f,dbgsoundsector[i]);
		uint32_t dbgtmpoffset = f.tellg();
		f.seekg(IMG_HEADER_OFFSET+dbgsoundsector[i]*FILE_IGNORE_DATA_PERIOD+0x14);
		uint32_t dbgsoundsize;
		FFIXReadLong(f,dbgsoundsize);
		dbgsoundsize -= 0x30;
		FFIXSeek(f,f.tellg(),0x28);
		uint8_t dbgbuffer;
		stringstream foutname;
		foutname << "Sound\\" << (unsigned int)dbgsoundid[i];
		fstream fout(foutname.str().c_str(),ios::out|ios::binary);
		for (unsigned int j=0;j<dbgsoundsize;j++) {
			FFIXReadChar(f,dbgbuffer);
			fout.put(dbgbuffer);
		}
		fout.close();
		f.seekg(dbgtmpoffset);
	}
	f.seekg(IMG_HEADER_OFFSET+metadatasector[9]*FILE_IGNORE_DATA_PERIOD);
	uint16_t* dbgmusicid = new uint16_t[metadatadataamount[9]];
	uint16_t* dbgmusictype = new uint16_t[metadatadataamount[9]];
	uint32_t* dbgmusicsector = new uint32_t[metadatadataamount[9]];
	for (i=0;i<metadatadataamount[9];i++) {
		FFIXReadShort(f,dbgmusicid[i]);
		FFIXReadShort(f,dbgmusictype[i]);
		FFIXReadLong(f,dbgmusicsector[i]);
		uint32_t dbgtmpoffset = f.tellg();
		f.seekg(IMG_HEADER_OFFSET+dbgmusicsector[i]*FILE_IGNORE_DATA_PERIOD+0x54);
		uint32_t dbgmusicsize;
		FFIXReadLong(f,dbgmusicsize);
		dbgmusicsize -= 0x30;
		FFIXSeek(f,f.tellg(),0x28);
		uint8_t dbgbuffer;
		stringstream foutname;
		foutname << "Music\\" << (unsigned int)dbgmusicid[i];
		fstream fout(foutname.str().c_str(),ios::out|ios::binary);
		for (unsigned int j=0;j<dbgmusicsize;j++) {
			FFIXReadChar(f,dbgbuffer);
			fout.put(dbgbuffer);
		}
		fout.close();
		f.seekg(dbgtmpoffset);
	}*/
	// (BIN HEADER)
	//-- Get language version from the top-level header
	FFIXSeek(f,BIN_HEADER,0x330);
	unsigned long versionraw = 0;
	tmp32 = 10000000L;
	for (i=0;i<8;i++) {
		FFIXReadChar(f,tmp8);
		versionraw += (tmp8-0x30)*tmp32;
		tmp32 /= 10;
	}
	switch (versionraw) {
	case 609013L:
		dest.language = LANGUAGE_VERSION_JAPAN;
		break;
	case 928124L:
		dest.language = LANGUAGE_VERSION_AMERICA;
		break;
	case 1128112L:
		dest.language = LANGUAGE_VERSION_FRANCE;
		break;
	case 1128134L:
		dest.language = LANGUAGE_VERSION_ENGLAND;
		break;
	case 1128151L:
		dest.language = LANGUAGE_VERSION_GERMANY;
		break;
	default:
		if (versionraw>=1128100L && versionraw<1128200L)
			dest.language = LANGUAGE_VERSION_EUROPE;
		else
			dest.language = LANGUAGE_VERSION_UNKNOWN;
	}
	// (SPECIAL TEXTS)
	//-- Find special text offsets all accross both the SLES and the IMG files
	f.seekg(SLES_HEADER);
	maxpos = IMG_HEADER_OFFSET+metafirstdatasector[1]*FILE_IGNORE_DATA_PERIOD;
	uint16_t stspacearray[G_N_ELEMENTS(HADES_STRING_SPECIAL_TEXT_BLOCK)];
	uint32_t stoffarray[G_N_ELEMENTS(HADES_STRING_SPECIAL_TEXT_BLOCK)];
	unsigned int textfoundcounter = 0, stcounter = 0;
	uint32_t sttxtoff;
	while (f.tellg()<maxpos) {
		FFIXReadLongAlign(f,tmp32);
		if (tmp32<0x100 && tmp32>1) {
			if (FFIXReadShort(f,tmp16)==tmp32*4) {
				FFIXReadChar(f,tmp8);
				FFIXReadChar(f,tmp8);
				if ((tmp8 & 0xF0)>0 && (tmp8 & 0x7)==0) {
					FFIXSeekBack(f,f.tellg(),8);
					sttxtoff = f.tellg();
					FFIXSeek(f,sttxtoff,tmp32*4);
					FFIXReadShort(f,glaststr);
					FFIXSeek(f,sttxtoff,glaststr+4);
					glaststr++;
					while (FFIXReadChar(f,tmp8)!=0xFF)
						glaststr++;
					while (f.tellg()%4!=0) {
						FFIXReadChar(f,tmp8);
						glaststr++;
					}
					if (textfoundcounter==0) {
						dest.stat_defaultname_offset = sttxtoff;
						dest.stat_defaultname_space_total = glaststr;
					} else if (textfoundcounter==2) {
						dest.card_text_offset = sttxtoff;
						dest.card_name_space_total = glaststr;
						if (dest.card_text_offset<stoffarray[0]+0x10000)
							dest.card_stat_offset[0] = f.tellg(); // Japanese version
						else
							dest.card_stat_offset[0] = 0; // Other versions ; find card_stat_offset later
					} else {
						stoffarray[stcounter] = sttxtoff;
						stspacearray[stcounter] = glaststr;
						if (stcounter==5)
							dest.spell_naming_rules_offset = f.tellg();
						stcounter++;
					}
					textfoundcounter++;
				}
			} else {
				FFIXSeekBack(f,f.tellg(),2);
			}
		}
	}
	if (stcounter!=G_N_ELEMENTS(HADES_STRING_SPECIAL_TEXT_BLOCK))
		return -2;
	dest.spetext_amount = stcounter;
	dest.spetext_offset = new uint32_t[stcounter];
	dest.spetext_space_total = new uint16_t[stcounter];
	for (i=0;i<stcounter;i++) {
		dest.spetext_offset[i] = stoffarray[i];
		dest.spetext_space_total[i] = stspacearray[i];
	}
	// (SLES FILE)
	//-- Find Card stats for non-japanese versions
	f.seekg(SLES_HEADER);
	maxpos = IMG_HEADER_OFFSET;
	if (dest.card_stat_offset[0]==0) {
		static uint32_t dcso[] = { 0x08040201L, 0x80402010L };
		if (ConfigFindSingle(f,dcso,2,maxpos))
			return -2;
		dest.card_stat_offset[0] = f.tellg();
	} else {
		f.seekg(dest.card_stat_offset[0]);
	}
	//-- Get card stat values to make the following search easier
	uint32_t dcs[4];
	for (i=0;i<4;i++)
		FFIXReadLong(f,dcs[i]);
	//-- Find item datas
	static uint32_t dito[] = { 0x6000E3FDL, 0x60C0FFFFL };
	if (ConfigFindSingle(f,dito,2,maxpos))
		return -2;
	dest.item_data_offset = f.tellg();
	//-- Get name space (regular then key items)
	FFIXSeek(f,dest.item_data_offset,18*ITEM_AMOUNT-18);
	uint16_t ithplaststr;
	FFIXReadShort(f,glaststr);
	FFIXReadShort(f,ithplaststr);
	FFIXSeek(f,f.tellg(),14);
	uint32_t itnmstrbegin = f.tellg();
	FFIXSeek(f,itnmstrbegin,glaststr);
	glaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		glaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		glaststr++;
	}
	dest.item_name_space_total = glaststr;
	counter = 5;
	while (FFIXReadLong(f,tmp32)!=0xFFFFFFFFL) // Assume there are a bunch of unamed key items
		counter += 4;
	while (FFIXReadChar(f,tmp8)==0xFF)
		counter++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		counter++;
	}
	dest.item_key_name_space_total = counter;
	FFIXSeek(f,f.tellg(),12*ITEM_USABLE_AMOUNT+8*KEY_ITEM_AMOUNT-8);
	uint16_t kinmlaststr,kihplaststr,kidclaststr;
	FFIXReadShort(f,kinmlaststr);
	FFIXReadShort(f,kihplaststr);
	FFIXReadShort(f,tmp16);
	FFIXReadShort(f,kidclaststr);
	//-- Find support, stat datas and some others which are nearby
	static uint32_t dslo[] = { 0x05060509L, 0x05060506L, 0x16151515L };
	if (ConfigFindSingle(f,dslo,3,maxpos))
		return -2;
	dest.stat_initialequip_offset = f.tellg();
	FFIXSeek(f,dest.stat_initialequip_offset,ABILITY_SET_AMOUNT*5+ABILITY_SET_AMOUNT*ABILITY_SET_CAPACITY*2);
	dest.support_data_offset = f.tellg();
	FFIXSeek(f,dest.support_data_offset,SUPPORT_AMOUNT*8-6);
	uint16_t sunmlaststr,suhplaststr;
	FFIXReadShort(f,sunmlaststr);
	FFIXReadShort(f,suhplaststr);
	FFIXReadShort(f,tmp16);
	dest.item_stat_offset = f.tellg();
	FFIXSeek(f,dest.item_stat_offset,ITEM_ARMOR_AMOUNT*4+ITEM_STAT_AMOUNT*12);
	dest.stat_initialstat_offset = f.tellg();
	FFIXSeek(f,dest.stat_initialstat_offset,PLAYABLE_CHAR_AMOUNT*6+MAX_LEVEL*8);
	dest.item_weaponstat_offset = f.tellg();
	// (IMG FILE)
	// (ABILITY + EQUIP MENU)
	//-- Find the 1st of 5 offset of spell datas
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[8]*FILE_IGNORE_DATA_PERIOD);
	maxpos = IMG_HEADER_OFFSET+metafirstdatasector[1]*FILE_IGNORE_DATA_PERIOD;
	static uint32_t dspo[] = { 0x7FFCB070L, 0x001F0C0EL };
	if (ConfigFindSingle(f,dspo,2,maxpos))
		return -2;
	dest.spell_data_offset[0] = f.tellg();
	//-- Get spell values to make following search easier
	FFIXSeek(f,dest.spell_data_offset[0],16);
	uint32_t dsp[4];
	for (i=0;i<4;i++)
		FFIXReadLong(f,dsp[i]);
	//-- Get both the name space allocated and the names first chars
	FFIXSeek(f,dest.spell_data_offset[0],SPELL_AMOUNT*16-2);
	FFIXReadShort(f,glaststr);
	uint32_t spnmstrbegin = f.tellg();
	uint32_t dspstr[4];
	for (i=0;i<4;i++)
		FFIXReadLong(f,dspstr[i]);
	FFIXSeek(f,spnmstrbegin,glaststr);
	glaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		glaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		glaststr++;
	}
	dest.spell_name_space_total = glaststr;
	//-- The help space allocated is more tricky ; assuming there is no help string starting by "000"
	uint32_t spnmstrend = f.tellg();
	uint32_t check;
	FFIXReadLong(f,check);
	while (check!=0xFF) // Searching the hexa sequence : "FF000000" (not necessary aligned)
		check = (check >> 8) | (FFIXReadChar(f,tmp8) << 24);
	FFIXSeekBack(f,f.tellg(),3);
	while (f.tellg()%4!=0)
		FFIXReadChar(f,tmp8);
	uint32_t sphpstrend = f.tellg();
	dest.spell_help_space_total = GetFFIXOffsetSub(spnmstrend,sphpstrend);
	FFIXSeek(f,sphpstrend,4*SPELL_AMOUNT);
	//-- Next come 1st support skill names and helps
	dest.support_text_offset[0] = f.tellg();
	FFIXSeek(f,dest.support_text_offset[0],sunmlaststr);
	sunmlaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		sunmlaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		sunmlaststr++;
	}
	dest.support_name_space_total = sunmlaststr;
	FFIXSeek(f,f.tellg(),suhplaststr);
	suhplaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		suhplaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		suhplaststr++;
	}
	dest.support_help_space_total = suhplaststr;
	//-- Next come the 2nd command data offset
	dest.cmd_data_offset[1] = f.tellg();
	//-- Get command values to make following search easier
	FFIXSeek(f,f.tellg(),0x0C);
	uint32_t dcmd[3];
	for (i=0;i<3;i++)
		FFIXReadLong(f,dcmd[i]);
	uint32_t cmdcheckpoint = f.tellg();
	// (BATTLE MODULE)
	//-- Return back to battle module since we needed to collect dspstr beforehand
	//-- MIPS battle code and offset list
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[2]*FILE_IGNORE_DATA_PERIOD);
	FFIXSeek(f,f.tellg(),4);
	while (FFIXReadChar(f,tmp8)!=0) {}
	while (f.tellg()%4!=0)
		FFIXReadChar(f,tmp8);
	dest.mips_battle_offset_list = f.tellg();
	static uint32_t dmbc[] = { 0x27BDFFE8L, 0xAFBF0014L };
	if (ConfigFindSingle(f,dmbc,2,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),4);
	dest.mips_battle_code_offset = f.tellg();
	dest.mips_battle_code_amount = 0;
	while (FFIXReadLongAlign(f,tmp32)!=0xFFFF)
		dest.mips_battle_code_amount++;
	dest.mips_battle_code_amount--;
	//-- Magic Sword Requirements (search base datas for that...)
	static uint32_t dmso[] = { 0x1D1B1A19L, 0x22211F1EL };
	if (ConfigFindSingle(f,dmso,2,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),8);
	dest.party_magicsword_offset = f.tellg();
	//-- Search the spell text offset using the first chars collected
	if (ConfigFindSingle(f,dspstr,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x10);
	dest.spell_text_offset = f.tellg();
	//-- 1st command data offset is after the item help
	if (ConfigFindSingle(f,dcmd,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x18);
	dest.cmd_data_offset[0] = f.tellg();
	//-- Get name and help space allocated
	FFIXSeek(f,dest.cmd_data_offset[0],12*(COMMAND_AMOUNT-1));
	uint16_t cmnmlaststr,cmhplaststr;
	FFIXReadShort(f,cmnmlaststr);
	FFIXReadShort(f,cmhplaststr);
	FFIXSeek(f,f.tellg(),8);
	FFIXSeek(f,f.tellg(),cmnmlaststr);
	cmnmlaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		cmnmlaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		cmnmlaststr++;
	}
	dest.cmd_name_space_total = cmnmlaststr;
	FFIXSeek(f,f.tellg(),cmhplaststr);
	cmhplaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		cmhplaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		cmhplaststr++;
	}
	dest.cmd_help_space_total = cmhplaststr;
	//-- Get stat_cmddata_offset[0] and collect values for the others
	FFIXSeek(f,f.tellg(),COMMAND_SPELL_AMOUNT);
	dest.stat_cmddata_offset[0] = f.tellg();
	uint32_t dstc[3];
	for (i=0;i<3;i++)
		FFIXReadLong(f,dstc[i]);
	//-- Since item help here is different, we get to it from behind
	FFIXSeekBack(f,dest.cmd_data_offset[0],2);
	FFIXReadShort(f,glaststr);
	FFIXSeek(f,dest.spell_text_offset,dest.spell_name_space_total+dest.spell_help_space_total+SPELL_AMOUNT*4);
	dest.item_help_offset[0] = f.tellg();
	FFIXSeek(f,dest.item_help_offset[0],glaststr);
	glaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		glaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		glaststr++;
	}
	dest.item_help_space2_total = glaststr;
	// (ABILITY + EQUIP MENU)
	//-- Return to the Ability Menu for stat_cmddata_offset[1]
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[8]*FILE_IGNORE_DATA_PERIOD);
	if (ConfigFindSingle(f,dstc,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0xC);
	dest.stat_cmddata_offset[1] = f.tellg();
	//-- Return after the cmd offset[1]
	f.seekg(cmdcheckpoint);
	//-- spell offset[1]
	if (ConfigFindSingle(f,dsp,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x20);
	dest.spell_data_offset[1] = f.tellg();
	//-- support text offset[1]
	FFIXSeek(f,dest.spell_data_offset[1],SPELL_AMOUNT*16+dest.spell_name_space_total);
	dest.support_text_offset[1] = f.tellg();
	//-- item help offset[1] is after some datas about texts in the EQUIP MENU but it slightly differs depending on version
	FFIXSeek(f,dest.support_text_offset[1],dest.support_name_space_total+3);
	if (FFIXReadChar(f,tmp8) & 0x80) { // Japanese version
		FFIXSeek(f,f.tellg(),0x84);
	} else { // European and US versions
		FFIXSeek(f,f.tellg(),0x6C);
		while (FFIXReadChar(f,tmp8)!=0) {}
		while (f.tellg()%4!=0)
			FFIXReadChar(f,tmp8);
	}
	dest.item_help_offset[1] = f.tellg();
	uint32_t ditmstr[3];
	for (i=0;i<3;i++)
		FFIXReadLong(f,ditmstr[i]);
	FFIXSeek(f,dest.item_help_offset[1],ithplaststr);
	ithplaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		ithplaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		ithplaststr++;
	}
	dest.item_help_space_total = ithplaststr;
/*	// (CARD MENU)
	//-- card_image_offset
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[10]*FILE_IGNORE_DATA_PERIOD);
	// Going through UI text
	if (versiontype==1) // Japan
		FFIXSeek(f,f.tellg(),0x70);
	else // Europe
		FFIXSeek(f,f.tellg(),0x18);
	uint32_t uicdtxtamount;
	FFIXReadLong(f,uicdtxtamount);
	uint32_t uicdtxtoffset = f.tellg();
	FFIXSeek(f,uicdtxtoffset,uicdtxtamount*4-4);
	uint16_t uicdnmlaststr;
	FFIXReadShort(f,uicdnmlaststr);
	FFIXSeek(f,uicdtxtoffset,uicdnmlaststr);
	uicdnmlaststr++;
	while (FFIXReadChar(f,tmp8)!=0xFF)
		uicdnmlaststr++;
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		uicdnmlaststr++;
	}
	dest.card_image_offset = f.tellg();*/
	// (ITEM MENU)
	//-- item help offset[2]
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[11]*FILE_IGNORE_DATA_PERIOD);
	if (ConfigFindSingle(f,ditmstr,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0xC);
	dest.item_help_offset[2] = f.tellg();
	FFIXSeek(f,dest.item_help_offset[2],dest.item_help_space_total+kihplaststr);
	while (f.tellg()%4!=0) { // key help/description offsets are sometime 1 byte out of bounds...
		FFIXReadChar(f,tmp8);
		kihplaststr++;
	}
	dest.item_key_help_space_total = kihplaststr;
	FFIXSeek(f,f.tellg(),kidclaststr);
	while (f.tellg()%4!=0) {
		FFIXReadChar(f,tmp8);
		kidclaststr++;
	}
	dest.item_key_desc_space_total = kidclaststr;
	// (STATUS MENU)
	//-- cmd offset[2]
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[12]*FILE_IGNORE_DATA_PERIOD);
	if (ConfigFindSingle(f,dcmd,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x18);
	dest.cmd_data_offset[2] = f.tellg();
	//-- stat_cmddata_offset[2]
	if (ConfigFindSingle(f,dstc,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x10);
	dest.stat_cmddata_offset[2] = f.tellg();
	//-- spell offset[2]
	if (ConfigFindSingle(f,dsp,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x20);
	dest.spell_data_offset[2] = f.tellg();
	//-- support text offset[2]
	FFIXSeek(f,dest.spell_data_offset[2],SPELL_AMOUNT*16+dest.spell_name_space_total);
	dest.support_text_offset[2] = f.tellg();
	// (SHOP MENU)
	//-- item help offset[3]
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[16]*FILE_IGNORE_DATA_PERIOD);
	if (ConfigFindSingle(f,ditmstr,3,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0xC);
	dest.item_help_offset[3] = f.tellg();
	//-- shop offset
	static uint32_t dshop[] = { 0x01E403E0L, 0x00030010L };
	if (ConfigFindSingle(f,dshop,2,maxpos))
		return -2;
	FFIXSeek(f,f.tellg(),0x8);
	dest.shop_data_offset = f.tellg();
	//-- synthesis offset
	FFIXSeek(f,dest.shop_data_offset,SHOP_AMOUNT*SHOP_ITEM_AMOUNT+0x1C);
	if (FFIXReadChar(f,tmp8)==0xA4) // European versions
		FFIXSeek(f,f.tellg(),0x9B);
	else // Japanese version
		FFIXSeek(f,f.tellg(),0xAB);
	dest.synthesis_data_offset = f.tellg();
	// (BATTLE REWARD MENU)
	//-- spell offset[3]
	f.seekg(IMG_HEADER_OFFSET+firstmetadatasector[20]*FILE_IGNORE_DATA_PERIOD);
	if (ConfigFindSingle(f,dsp,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x20);
	dest.spell_data_offset[3] = f.tellg();
	//-- support text offset[3]
	FFIXSeek(f,dest.spell_data_offset[3],SPELL_AMOUNT*16+dest.spell_name_space_total);
	dest.support_text_offset[3] = f.tellg();
	// (OTHER TETRA MASTER MODULE)
	//-- card stat offset[1] and deck datas
	f.seekg(IMG_HEADER_OFFSET+metafirstdatasector[1]*FILE_IGNORE_DATA_PERIOD);
	maxpos = IMG_HEADER_OFFSET+metafirstdatasector[2]*FILE_IGNORE_DATA_PERIOD;
	if (ConfigFindSingle(f,dcs,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x10);
	dest.card_stat_offset[1] = f.tellg();
	dest.card_set_offset = dest.card_stat_offset[1]+FILE_IGNORE_DATA_PERIOD;
	dest.card_deck_offset = dest.card_stat_offset[1]+2*FILE_IGNORE_DATA_PERIOD;
	// (OTHER BATTLE MODULE)
	// Actually in a 0x00 chunk data
	//-- spell offset[4]
	if (ConfigFindSingle(f,dsp,4,maxpos))
		return -2;
	FFIXSeekBack(f,f.tellg(),0x20);
	dest.spell_data_offset[4] = f.tellg();
	// (CLUSTERS)
	//-- Data clusters
	uint32_t tmpclusteroffset[CLUSTER_MAX_AMOUNT];
	unsigned int j;
	uint32_t tmpoff;
	dest.cluster_amount = 0;
	for (i=1;i<12;i++) {
		f.seekg(IMG_HEADER_OFFSET+metadatasector[i]*FILE_IGNORE_DATA_PERIOD);
		for (j=0;j<metadatadataamount[i];j++) {
			FFIXReadLong(f,tmp32);
			FFIXReadLong(f,tmp32);
			if (dest.cluster_amount==0 || IMG_HEADER_OFFSET+tmp32*FILE_IGNORE_DATA_PERIOD!=tmpclusteroffset[dest.cluster_amount-1]) {
				tmpoff = f.tellg();
				f.seekg(IMG_HEADER_OFFSET+tmp32*FILE_IGNORE_DATA_PERIOD);
				if (FFIXReadChar(f,tmp8)==0xDB)
					tmpclusteroffset[dest.cluster_amount++] = IMG_HEADER_OFFSET+tmp32*FILE_IGNORE_DATA_PERIOD;
				f.seekg(tmpoff);
			}
		}
	}
	dest.cluster_offset = new uint32_t[dest.cluster_amount];
	copy(tmpclusteroffset,tmpclusteroffset+dest.cluster_amount,dest.cluster_offset);
	f.close();
	delete[] metadatatype;
	delete[] metadatadataamount;
	delete[] metadatasector;
	delete[] metafirstdatasector;
	delete[] firstmetadataid;
	delete[] firstmetadatatype;
	delete[] firstmetadatasector;
	return 0;
}

//==================================================//
//                 STEAM VERSION                    //
//==================================================//

string ConfigurationSet::GetSteamAssetPath(UnityArchiveFile arch, int32_t fileid) {
	if (arch == UNITY_ARCHIVE_DATA2)
		return "StreamingAssets\\" + meta_battle.GetFileFullName(fileid, arch, &bundle_battle, &indexlist_res);
	if (arch == UNITY_ARCHIVE_DATA3)
		return "StreamingAssets\\" + meta_world.GetFileFullName(fileid, arch, &bundle_world, &indexlist_res);
	if (arch == UNITY_ARCHIVE_DATA7)
		return "StreamingAssets\\" + meta_script.GetFileFullName(fileid, arch, &bundle_script, &indexlist_res);
	if (arch >= UNITY_ARCHIVE_DATA11 && arch <= UNITY_ARCHIVE_DATA19)
		return "StreamingAssets\\" + meta_field[arch-UNITY_ARCHIVE_DATA11].GetFileFullName(fileid, arch, &bundle_field[arch-UNITY_ARCHIVE_DATA11], &indexlist_res);
	if (arch == UNITY_ARCHIVE_RESOURCES)
		return "FF9_Data\\" + meta_res.GetFileFullName(fileid, arch, NULL, &indexlist_res);
	return "unexpected_export\\unexpected_export.bin";
}

int InitSteamConfiguration(string filepath, ConfigurationSet& dest) {
	static string langdir[STEAM_LANGUAGE_AMOUNT] = { "us", "uk", "jp", "gr", "fr", "it", "es" };
	unsigned int i,j;
	uint32_t fid;
	fstream unityarchive;
	string unityarchivename,token;
	dest.steam_dir_assets = filepath+"StreamingAssets\\";
	dest.steam_dir_data = filepath+"x64\\FF9_Data\\";
	dest.steam_dir_managed = filepath+"x64\\FF9_Data\\Managed\\";
	// Loading meta datas
	unityarchivename = dest.steam_dir_data+"resources.assets";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_res.Load(unityarchive, UNITY_ARCHIVE_RESOURCES);
	unityarchive.close();
	string subfilepath;
	unityarchivename = dest.steam_dir_data+"mainData";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_main.Load(unityarchive, UNITY_ARCHIVE_MAINDATA);
	uint32_t mesindexfileid = dest.meta_main.GetFileIndex("",147);
	unityarchive.seekg(dest.meta_main.GetFileOffsetByIndex(mesindexfileid));
	dest.indexlist_res.Load(unityarchive,dest.meta_main.GetFileSizeByIndex(mesindexfileid));
	unityarchive.close();
	unityarchivename = dest.steam_dir_assets+"p0data7.bin";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_script.Load(unityarchive, UNITY_ARCHIVE_DATA7);
	unityarchive.seekg(dest.meta_script.GetFileOffset("",142));
	dest.bundle_script.Load(unityarchive);
	unityarchive.close();
	dest.enmy_amount = G_N_ELEMENTS(SteamBattleScript);
	dest.enmy_stat_file = new int32_t[dest.enmy_amount];
	dest.enmy_battle_file = new int32_t[dest.enmy_amount];
	dest.enmy_id = new uint16_t[dest.enmy_amount];
	dest.field_amount = G_N_ELEMENTS(SteamFieldScript);
	dest.field_role_file = new int32_t[dest.field_amount];
	dest.field_preload_file = new int32_t[dest.field_amount];
	dest.field_walkmesh_file = new int32_t[dest.field_amount];
	dest.field_image_file = new int32_t[dest.field_amount];
	dest.field_tiles_file = new int32_t*[dest.field_amount];
	dest.field_tiles_localized = new bool[dest.field_amount];
	dest.field_file_id = new uint8_t[dest.field_amount];
	dest.field_id = new uint16_t[dest.field_amount];
	dest.world_amount = WORLD_BLOCK_VALID_AMOUNT; // Both EVT_WORLD_WORLDTS and EVT_WORLD_WORLDSV are japanese-only and are leftovers
	dest.world_id = new uint16_t[dest.world_amount];
	unityarchivename = dest.steam_dir_assets+"p0data2.bin";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_battle.Load(unityarchive, UNITY_ARCHIVE_DATA2);
	unityarchive.seekg(dest.meta_battle.GetFileOffset("",142));
	dest.bundle_battle.Load(unityarchive);
	unityarchive.close();
	unityarchivename = dest.steam_dir_assets+"p0data3.bin";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_world.Load(unityarchive, UNITY_ARCHIVE_DATA3);
	unityarchive.seekg(dest.meta_world.GetFileOffset("",142));
	dest.bundle_world.Load(unityarchive);
	unityarchive.close();
	for (i=0;i<9;i++) {
		stringstream fieldunityarchivename;
		fieldunityarchivename << dest.steam_dir_assets << "p0data1" << i+1 << ".bin";
		unityarchive.open(fieldunityarchivename.str().c_str(),ios::in | ios::binary);
		if (!unityarchive.is_open()) return -3;
		dest.meta_field[i].Load(unityarchive, (UnityArchiveFile)(UNITY_ARCHIVE_DATA11+i));
		unityarchive.seekg(dest.meta_field[i].GetFileOffset("",142));
		dest.bundle_field[i].Load(unityarchive);
		unityarchive.close();
	}
	dest.dll_usage = 0;
	unityarchivename = dest.steam_dir_managed+"Assembly-CSharp.dll";
	if (dest.meta_dll.Load(unityarchivename.c_str())) return -3;
	if (dest.meta_dll.GetMethodIdByName("Configuration", ".cctor", "Memoria") != -1) {
		// Identified a Memoria-modded DLL
		dest.dll_usage = 1;
		dest.meta_dll.dll_file.close();
		unityarchivename = dest.steam_dir_managed + "Assembly-CSharp_Vanilla.dll";
		if (dest.meta_dll.Load(unityarchivename.c_str()) || dest.meta_dll.GetMethodIdByName("Configuration", ".cctor", "Memoria") != -1) {
			dest.dll_usage = 2;
			if (dest.meta_dll.dll_file.is_open())
				dest.meta_dll.dll_file.close();
			unityarchivename = dest.steam_dir_managed + "Assembly-CSharp.bak";
			if (dest.meta_dll.Load(unityarchivename.c_str()) || dest.meta_dll.GetMethodIdByName("Configuration", ".cctor", "Memoria") != -1) {
				dest.dll_usage = 3;
				if (dest.meta_dll.dll_file.is_open())
					dest.meta_dll.dll_file.close();
				unityarchivename = "Assembly-CSharp_Vanilla.dll";
				if (dest.meta_dll.Load(unityarchivename.c_str()) || dest.meta_dll.GetMethodIdByName("Configuration", ".cctor", "Memoria") != -1) {
					dest.dll_usage = 0;
					if (dest.meta_dll.dll_file.is_open())
						dest.meta_dll.dll_file.close();
					return -4;
				}
			}
		}
	}
	
	// Battles
	string battlenamelower[G_N_ELEMENTS(SteamBattleScript)];
	for (i=0;i<G_N_ELEMENTS(SteamBattleScript);i++) {
		dest.enmy_id[i] = SteamBattleScript[i].battle_id;
		battlenamelower[i] = SteamBattleScript[i].name;
		transform(battlenamelower[i].begin(),battlenamelower[i].end(),battlenamelower[i].begin(),::tolower);
	}
	for (i=0;i<dest.enmy_amount;i++) {
		
		#define MACRO_CONFIG_SEARCHOFFBATTLE_STEAM(NAME,FILE,VARIABLE) \
			subfilepath = "assets/resources/battlemap/battlescene/"+NAME+"/"+FILE+".bytes"; \
			dest.VARIABLE = dest.meta_battle.GetFileIndexByInfo(dest.bundle_battle.GetFileInfo(subfilepath),49);
		
		stringstream bnamestr;
		bnamestr << (unsigned int)SteamBattleScript[i].battle_id << ".raw17";
		MACRO_CONFIG_SEARCHOFFBATTLE_STEAM(battlenamelower[i],bnamestr.str(),enmy_battle_file[i])
		MACRO_CONFIG_SEARCHOFFBATTLE_STEAM(battlenamelower[i],"dbfile0000.raw16",enmy_stat_file[i])
	}
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		dest.enmy_script_file[i] = new int32_t[dest.enmy_amount];
		
		#define MACRO_CONFIG_SEARCHOFFSCRIPT_STEAM(MODULE,NAME,VARIABLE) \
			subfilepath = string("assets/resources/commonasset/eventengine/eventbinary/")+MODULE+langdir[i]+"/"+NAME+".eb.bytes"; \
			dest.VARIABLE = dest.meta_script.GetFileIndexByInfo(dest.bundle_script.GetFileInfo(subfilepath),49);
		
		for (j=0;j<dest.enmy_amount;j++) {
			MACRO_CONFIG_SEARCHOFFSCRIPT_STEAM("battle/",battlenamelower[j],enmy_script_file[i][j])
		}
	}
	
	// Fields
	dest.field_title_info = dest.meta_res.GetFileIndex("mapLocalizeAreaTitle.txt");
	string fieldnamelower[G_N_ELEMENTS(SteamFieldScript)];
	string fieldbacknamelower[G_N_ELEMENTS(SteamFieldScript)];
	for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
		fieldnamelower[i] = SteamFieldScript[i].script_name;
		fieldbacknamelower[i] = SteamFieldScript[i].background_name;
		transform(fieldnamelower[i].begin(),fieldnamelower[i].end(),fieldnamelower[i].begin(),::tolower);
		transform(fieldbacknamelower[i].begin(),fieldbacknamelower[i].end(),fieldbacknamelower[i].begin(),::tolower);
	}
	for (i=0;i<dest.field_amount;i++) {
		dest.field_id[i] = SteamFieldScript[i].script_id;
		dest.field_file_id[i] = SteamFieldScript[i].file_id;
		dest.field_tiles_localized[i] = SteamFieldScript[i].has_localization;
		subfilepath = "assets/resources/commonasset/eventengine/eventanimation/"+fieldnamelower[i]+".txt.bytes";
		dest.field_preload_file[i] = dest.meta_script.GetFileIndexByInfo(dest.bundle_script.GetFileInfo(subfilepath),49);
		subfilepath = "assets/resources/commonasset/mapconfigdata/"+fieldnamelower[i]+".bytes";
		dest.field_role_file[i] = dest.meta_script.GetFileIndexByInfo(dest.bundle_script.GetFileInfo(subfilepath),49);
		if (dest.field_file_id[i]==0) {
			dest.field_walkmesh_file[i] = 0;
			dest.field_image_file[i] = 0;
			dest.field_tiles_file[i] = new int32_t[1];
			dest.field_tiles_file[i][0] = 0;
			continue;
		}
		subfilepath = "assets/resources/fieldmaps/"+fieldbacknamelower[i]+"/"+fieldbacknamelower[i]+".bgi.bytes";
		dest.field_walkmesh_file[i] = dest.meta_field[dest.field_file_id[i]-1].GetFileIndexByInfo(dest.bundle_field[dest.field_file_id[i]-1].GetFileInfo(subfilepath),49);
		subfilepath = "assets/resources/fieldmaps/"+fieldbacknamelower[i]+"/atlas.png";
		dest.field_image_file[i] = dest.meta_field[dest.field_file_id[i]-1].GetFileIndexByInfo(dest.bundle_field[dest.field_file_id[i]-1].GetFileInfo(subfilepath),28);
		int64_t finfo;
		int32_t fsharedbgs;
		subfilepath = "assets/resources/fieldmaps/"+fieldbacknamelower[i]+"/"+fieldbacknamelower[i]+".bgs.bytes";
		fsharedbgs = dest.meta_field[dest.field_file_id[i]-1].GetFileIndexByInfo(dest.bundle_field[dest.field_file_id[i]-1].GetFileInfo(subfilepath),49);
		if (dest.field_tiles_localized[i]) {
			dest.field_tiles_file[i] = new int32_t[STEAM_LANGUAGE_AMOUNT];
			for (j=0;j<STEAM_LANGUAGE_AMOUNT;j++) {
				subfilepath = "assets/resources/fieldmaps/"+fieldbacknamelower[i]+"/"+fieldbacknamelower[i]+"_"+langdir[j]+".bgs.bytes";
				finfo = dest.bundle_field[dest.field_file_id[i]-1].GetFileInfo(subfilepath);
				if (finfo==0)
					dest.field_tiles_file[i][j] = fsharedbgs;
				else
					dest.field_tiles_file[i][j] = dest.meta_field[dest.field_file_id[i]-1].GetFileIndexByInfo(finfo,49);
			}
		} else {
			dest.field_tiles_file[i] = new int32_t[1];
			dest.field_tiles_file[i][0] = fsharedbgs;
		}
	}
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		dest.field_script_file[i] = new int32_t[dest.field_amount];
		for (j=0;j<dest.field_amount;j++) {
			MACRO_CONFIG_SEARCHOFFSCRIPT_STEAM("field/",fieldnamelower[j],field_script_file[i][j])
		}
	}
	
	// Worlds
	string worldnamelower[WORLD_BLOCK_VALID_AMOUNT];
	for (i=0;i<WORLD_BLOCK_VALID_AMOUNT;i++) {
		dest.world_id[i] = HADES_STRING_WORLD_BLOCK_NAME[i].id;
		worldnamelower[i] = HADES_STRING_WORLD_BLOCK_NAME[i].steamid.ToStdString();
		transform(worldnamelower[i].begin(),worldnamelower[i].end(),worldnamelower[i].begin(),::tolower);
	}
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		dest.world_script_file[i] = new int32_t[dest.world_amount];
		for (j=0;j<dest.world_amount;j++) {
			MACRO_CONFIG_SEARCHOFFSCRIPT_STEAM("world/",worldnamelower[j],world_script_file[i][j])
		}
	}
	dest.world_fx_file[0] = dest.meta_world.GetFileIndex("fx.ntp",49,0);
	dest.world_fx_file[1] = dest.meta_world.GetFileIndex("fx.ntp",49,1);
	dest.world_disc_file[0] = dest.meta_world.GetFileIndex("disc.img",49,0);
	dest.world_disc_file[1] = dest.meta_world.GetFileIndex("disc.img",49,1);
	dest.world_discmr_file[0] = dest.meta_world.GetFileIndex("discmr.img",49,0);
	dest.world_discmr_file[1] = dest.meta_world.GetFileIndex("discmr.img",49,1);
	
	// Text Blocks
	vector<uint16_t> textidtmp;
	vector<string> textfilenametmp;
	for (i=0;i<dest.indexlist_res.amount;i++) {
		if (dest.indexlist_res.path[i].length()>30) {
			token = dest.indexlist_res.path[i].substr(19, 9);
			if (token.compare("us/field/")==0) {
				token = dest.indexlist_res.path[i].substr(28);
				token = token.substr(0, token.find('.'));
				fid = strtol(token.c_str(), NULL, 10);
				if (token.length()>0 && fid!=STEAM_WORLD_MAP_TEXT_ID) {
					textfilenametmp.push_back(token);
					if (token.back()>='0' && token.back()<='9')
						textidtmp.push_back(fid);
					else
						textidtmp.push_back(0x2000+fid); // Only Qu's Marsh has an alternate text block for some reason (71m.mes)
				}
			}
		}
	}
	dest.text_amount = textidtmp.size();
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		dest.enmy_text_file[i] = new int32_t[dest.enmy_amount];
		dest.text_file[i] = new int32_t[dest.text_amount];
	}
	dest.text_id = new uint16_t[dest.text_amount];
	memcpy(dest.text_id,textidtmp.data(),dest.text_amount*sizeof(uint16_t));
	
	// Resource file
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		
		#define MACRO_CONFIG_SEARCHOFFTXT_STEAM(NAME,VARIABLE) \
			subfilepath = "embeddedasset/text/"+langdir[i]+NAME; \
			dest.VARIABLE = dest.indexlist_res.GetFileIndex(subfilepath)-1;
		
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/ability/aa_name.mes",spell_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/ability/aa_help.mes",spell_help_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/command/com_name.mes",cmd_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/command/com_help.mes",cmd_help_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/ability/sa_name.mes",support_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/ability/sa_help.mes",support_help_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/item/itm_name.mes",item_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/item/itm_help.mes",item_help_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/item/itm_btl.mes",item_help2_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/keyitem/imp_name.mes",itemkey_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/keyitem/imp_help.mes",itemkey_help_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/keyitem/imp_skin.mes",itemkey_desc_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/location/loc_name.mes",field_text_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/field/68.mes",world_text_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/worldloc.mes",world_worldplace_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/minista.mes",card_name_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/follow.mes",spetext_battleinfo_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/libra.mes",spetext_battlescan_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/cmdtitle.mes",spetext_spellnaming_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/ff9choco.mes",spetext_chocomenu_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/card.mes",spetext_cardrank_file[i])
		MACRO_CONFIG_SEARCHOFFTXT_STEAM("/etc/minigame.mes",spetext_tetramaster_file[i])
		for (j=0;j<G_N_ELEMENTS(SteamBattleScript);j++) {
			stringstream bnamestr;
			bnamestr << "/battle/" << (unsigned int)SteamBattleScript[j].battle_id << ".mes";
			MACRO_CONFIG_SEARCHOFFTXT_STEAM(bnamestr.str(),enmy_text_file[i][j])
		}
		for (j=0;j<dest.text_amount;j++) {
			stringstream fnamestr;
			fnamestr << "/field/" << textfilenametmp[j] << ".mes";
			MACRO_CONFIG_SEARCHOFFTXT_STEAM(fnamestr.str(),text_file[i][j])
		}
	}
	dest.card_stat_file = dest.meta_res.GetFileIndex("MINIGAME_CARD_DATA_ADDRESS");
	dest.card_deck_file = dest.meta_res.GetFileIndex("MINIGAME_STAGE_ADDRESS");
	dest.card_set_file = dest.meta_res.GetFileIndex("MINIGAME_CARD_LEVEL_ADDRESS");
	dest.spetext_amount = STEAM_SPECIAL_TEXT_AMOUNT;
	dest.spetext_localization_file = dest.meta_res.GetFileIndex("Localization.txt");
	dest.spetext_battleinfo_amount = 39;
	dest.spetext_battlescan_amount = 22;
	dest.spetext_spellnaming_amount = 8;
	dest.spetext_chocomenu_amount = 82;
	dest.spetext_cardrank_amount = 32;
	dest.spetext_tetramaster_amount = 53;
	dest.spetext_localization_amount = 490;
	for (i=0;i<SPELL_ANIMATION_AMOUNT;i++) {
		stringstream fnamestr;
		fnamestr << "ef";
		fnamestr.fill('0');
		fnamestr.width(3);
		fnamestr << (unsigned int)i;
		dest.spellanim_steam_file[i] = dest.meta_res.GetFileIndex(fnamestr.str(),49);
	}
	
	// DLL file
	fstream& dllfile = dest.meta_dll.dll_file;
	DllMethodInfo fieldmethinfo;
	ILInstruction fieldinst;
	int fieldmethodid;
	dest.dll_battledb_method_id = dest.meta_dll.GetMethodIdByName("FF9BattleDB",".cctor");
	dest.dll_ability_method_id = dest.meta_dll.GetMethodIdByName("ff9abil",".cctor","FF9");
	dest.dll_rdata_method_id = dest.meta_dll.GetMethodIdByName("rdata",".cctor");
	dest.dll_level_method_id = dest.meta_dll.GetMethodIdByName("ff9level",".cctor");
	dest.dll_item_method_id = dest.meta_dll.GetMethodIdByName("ff9item",".cctor");
	dest.dll_armor_method_id = dest.meta_dll.GetMethodIdByName("ff9armor",".cctor","FF9");
	dest.dll_weapon_method_id = dest.meta_dll.GetMethodIdByName("ff9weap",".cctor","FF9");
	dest.dll_equip_method_id = dest.meta_dll.GetMethodIdByName("ff9equip",".cctor","FF9");
	dest.dll_mix_method_id = dest.meta_dll.GetMethodIdByName("ff9mix",".cctor","FF9");
	dllfile.seekg(dest.meta_dll.GetMethodOffset(dest.dll_battledb_method_id));
	fieldmethinfo.ReadMethodInfo(dllfile);
	ILInstruction statussetinst[3] = {
		{ 0x20, STATUS_SET_AMOUNT*2 }, // DEBUG: Steam version has 2x more status sets...
		{ 0x8D, dest.meta_dll.GetTypeRefTokenIdentifier("UInt32","System") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,3,statussetinst);
	fieldinst.Read(dllfile);
	dest.dll_statusset_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	fieldmethodid = dest.meta_dll.GetMethodIdByName("BattleHUD",".cctor");
	dllfile.seekg(dest.meta_dll.GetMethodOffset(fieldmethodid));
	fieldmethinfo.ReadMethodInfo(dllfile);
	ILInstruction spellnaminginst[3] = {
		{ 0x20, SPELL_AMOUNT },
		{ 0x8D, dest.meta_dll.GetTypeRefTokenIdentifier("Byte","System") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,3,spellnaminginst);
	fieldinst.Read(dllfile);
	dest.dll_spellnaming_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	dllfile.seekg(dest.meta_dll.GetMethodOffset(dest.dll_rdata_method_id));
	fieldmethinfo.ReadMethodInfo(dllfile);
	ILInstruction cmdspelllistinst[3] = {
		{ 0x20, COMMAND_SPELL_AMOUNT },
		{ 0x8D, dest.meta_dll.GetTypeRefTokenIdentifier("Int32","System") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,3,cmdspelllistinst);
	fieldinst.Read(dllfile);
	dest.dll_cmdspelllist_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	dllfile.seekg(fieldmethinfo.code_abs_offset);
	ILInstruction statcmdtranceinst[4] = {
		{ 0x1F, COMMAND_SET_AMOUNT },
		{ 0x19 },
		{ 0x73, dest.meta_dll.GetMemberTokenIdentifier("void byte[,]::.ctor( int, int )") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,4,statcmdtranceinst);
	fieldinst.Read(dllfile);
	dest.dll_statcmdtrance_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	fieldinst.Read(dllfile); // Initialize Array
	dest.dll_cmdtrancenones_offset = dllfile.tellg();
	size_t trancenonepos = dest.dll_cmdtrancenones_offset;
	while (dllfile.tellg() < fieldmethinfo.code_abs_offset+fieldmethinfo.code_size) {
		fieldinst.Read(dllfile);
		if (fieldinst.opcode != 0x25) // dup
			break;
		fieldinst.Read(dllfile);
		if (!fieldinst.IsStackNumberPush())
			break;
		fieldinst.Read(dllfile);
		if (!fieldinst.IsStackNumberPush())
			break;
		fieldinst.Read(dllfile);
		if (fieldinst.opcode != 0x7E) // ldsfld
			break;
		fieldinst.Read(dllfile);
		if (fieldinst.opcode != 0x28) // call
			break;
		trancenonepos = dllfile.tellg();
	}
	dest.dll_cmdtrancenones_size = trancenonepos-dest.dll_cmdtrancenones_offset;
	dllfile.seekg(dest.meta_dll.GetMethodOffset(dest.dll_level_method_id));
	fieldmethinfo.ReadMethodInfo(dllfile);
	ILInstruction statexpinst[3] = {
		{ 0x1F, MAX_LEVEL },
		{ 0x8D, dest.meta_dll.GetTypeRefTokenIdentifier("UInt64","System") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,3,statexpinst);
	fieldinst.Read(dllfile);
	dest.dll_statexp_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	fieldmethodid = dest.meta_dll.GetMethodIdByName("BattleHUD","SetAbilityMagic");
	dllfile.seekg(dest.meta_dll.GetMethodOffset(fieldmethodid));
	fieldmethinfo.ReadMethodInfo(dllfile);
	ILInstruction magicswordinst[3] = {
		{ 0x1F, MAGIC_SWORD_AMOUNT },
		{ 0x8D, dest.meta_dll.GetTypeRefTokenIdentifier("Int32","System") },
		{ 0x25 }
	};
	fieldmethinfo.JumpToInstructions(dllfile,3,magicswordinst);
	fieldinst.Read(dllfile);
	dest.dll_magicsword_field_id = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	fieldmethodid = dest.meta_dll.GetMethodIdByName("ff9buy",".cctor","FF9");
	dllfile.seekg(dest.meta_dll.GetMethodOffset(fieldmethodid));
	fieldmethinfo.ReadMethodInfo(dllfile);
	fieldinst.Read(dllfile);
	while (dllfile.tellg()<fieldmethinfo.code_abs_offset+fieldmethinfo.code_size && fieldinst.opcode!=0xD0)
		fieldinst.Read(dllfile);
	dest.dll_shop_field_id[0] = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	for (i=1;i<SHOP_AMOUNT;i++) {
		if (i<23)
			dest.dll_shop_field_id[i] = dest.dll_shop_field_id[0]+i;
		else if (i>24)
			dest.dll_shop_field_id[i] = dest.dll_shop_field_id[0]+i-2;
		else
			dest.dll_shop_field_id[i] = -1;
	}
	fieldmethodid = dest.meta_dll.GetMethodIdByName("ff9play","FF9Play_SetDefEquips");
	dllfile.seekg(dest.meta_dll.GetMethodOffset(fieldmethodid));
	fieldmethinfo.ReadMethodInfo(dllfile);
	fieldinst.Read(dllfile);
	while (dllfile.tellg()<fieldmethinfo.code_abs_offset+fieldmethinfo.code_size && fieldinst.opcode!=0xD0)
		fieldinst.Read(dllfile);
	dest.dll_equipset_field_id[0] = dest.meta_dll.GetStaticFieldIdFromToken(fieldinst.param);
	for (i=1;i<EQUIP_SET_AMOUNT;i++)
		dest.dll_equipset_field_id[i] = dest.dll_equipset_field_id[0]+i;

	unityarchivename = dest.steam_dir_data+"sharedassets2.assets";
	unityarchive.open(unityarchivename.c_str(),ios::in | ios::binary);
	if (!unityarchive.is_open()) return -3;
	dest.meta_atlas.Load(unityarchive, UNITY_ARCHIVE_SHARED2);
	for (i=0;;i++) {
		dest.atlas_iconsprite_file = dest.meta_atlas.GetFileIndex("",-1,i);
		if (dest.atlas_iconsprite_file<0)
			break;
		unityarchive.seekg(dest.meta_atlas.GetFileOffsetByIndex(dest.atlas_iconsprite_file)+0x2C);
		if (ReadLong(unityarchive)==377) // DEBUG: should find it from GameObject hierarchy?
			break;
	}
	dest.atlas_icontexture_file = dest.meta_atlas.GetFileIndex("Icon Atlas", 28);
	unityarchive.close();

	dest.language = LANGUAGE_VERSION_UNKNOWN;
	dest.spell_name_space_total = 0xFFFF;
	dest.spell_help_space_total = 0xFFFF;
	dest.cmd_name_space_total = 0xFFFF;
	dest.cmd_help_space_total = 0xFFFF;
	dest.item_name_space_total = 0xFFFF;
	dest.item_help_space_total = 0xFFFF;
	dest.item_help_space2_total = 0xFFFF;
	dest.item_key_name_space_total = 0xFFFF;
	dest.item_key_help_space_total = 0xFFFF;
	dest.item_key_desc_space_total = 0xFFFF;
	dest.support_name_space_total = 0xFFFF;
	dest.support_help_space_total = 0xFFFF;
	dest.stat_defaultname_space_total = 0xFFFF;
	dest.card_name_space_total = 0xFFFF;
	dest.spetext_steam_space_total = new uint32_t[dest.spetext_amount];
	for (i=0;i<dest.spetext_amount;i++)
		dest.spetext_steam_space_total[i] = 0xFFFFFF;
	return 0;
}

int CreateSteamMod(string destfolder, bool* section, ConfigurationSet& config, SaveSet& saveset, int dllformat, int assetformat, bool deleteold) {
	// For chunks, we use the functions WriteHWS as the writing process is the same
	// A WriteSteam function is needed only when texts are involved
	SteamLanguage lang;
	unsigned int i, j;
	int result = 0;
	if (destfolder.back() != '\\') destfolder += "\\";
	string dirassets = destfolder + "StreamingAssets\\";
	string dirdata = destfolder + "x64\\FF9_Data\\";
	string dirmanaged = destfolder + "x64\\FF9_Data\\Managed\\";
	string fname;
	if (config.steam_dir_assets.compare(dirassets) == 0)
		return 1;
	if (deleteold) {
		// Delete previous mod files
		if (assetformat == 0) {
			fname = dirassets + "p0data2.bin";
			remove(fname.c_str());
			fname = dirassets + "p0data3.bin";
			remove(fname.c_str());
			fname = dirassets + "p0data7.bin";
			remove(fname.c_str());
			for (i = 0; i < 9; i++) {
				stringstream ffieldname;
				ffieldname << dirassets << "p0data1" << (i+1) << ".bin";
				remove(ffieldname.str().c_str());
			}
			fname = dirdata + "resources.assets";
			remove(fname.c_str());
		}
		if (dllformat==0) {
			fname = dirmanaged + "Assembly-CSharp.dll";
			remove(fname.c_str());
		} else if (dllformat==1) {
			fname = dirassets + HADES_STRING_CSV_SPELL_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_STATUS_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_STATINIT_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_COMMAND_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_STATCMD_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_SPELLTITLE_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_STATEQUIP_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_STATLEVEL_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_SUPPORT_FILE;
			remove(fname.c_str());
			for (i=0; i<G_N_ELEMENTS(HADES_STRING_CSV_STATABIL_FILE); i++) {
				fname = dirassets + HADES_STRING_CSV_STATABIL_FILE[i];
				remove(fname.c_str());
			}
			fname = dirassets + HADES_STRING_CSV_ARMOR_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_USABLE_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_ITEM_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_SHOP_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_ITEMSTAT_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_SYNTHESIS_FILE;
			remove(fname.c_str());
			fname = dirassets + HADES_STRING_CSV_WEAPON_FILE;
			remove(fname.c_str());
			// Keep the old file and let EnemyDataSet::GenerateCSV patch its content by modifying the "BattleMapModel" lines only
//			fname = destfolder + HADES_STRING_DICTIONARY_PATCH_FILE;
//			remove(fname.c_str());
		} else if (dllformat==2) {
			fname = destfolder + "Assembly-CSharp_Additions.cs";
			remove(fname.c_str());
		}
	}

	// Update the offsets/sizes
	if (section[DATA_SECTION_TEXT])
		for (i = 0; i < saveset.textset->amount; i++)
			saveset.textset->text_data[i]->parent_cluster->UpdateOffset();
	if (section[DATA_SECTION_ENMY])
		for (i = 0; i < saveset.enemyset->battle_amount; i++)
			saveset.enemyset->battle_data[i]->parent_cluster->UpdateOffset();
	if (section[DATA_SECTION_WORLD_MAP])
		for (i = 0; i < saveset.worldset->amount; i++)
			saveset.worldset->script[i]->parent_cluster->UpdateOffset();
	if (section[DATA_SECTION_FIELD])
		for (i = 0; i < saveset.fieldset->amount; i++)
			saveset.fieldset->script_data[i]->parent_cluster->UpdateOffset();
	if (section[DATA_SECTION_SPELL])
		saveset.spellset->UpdateOffset();
	if (section[DATA_SECTION_CMD])
		saveset.cmdset->UpdateOffset();
	if (section[DATA_SECTION_ITEM])
		saveset.itemset->UpdateOffset();
	if (section[DATA_SECTION_SUPPORT])
		saveset.supportset->UpdateOffset();
	if (section[DATA_SECTION_STAT])
		saveset.statset->UpdateOffset();
	if (section[DATA_SECTION_CARD])
		saveset.cardset->UpdateOffset();
	if (section[DATA_SECTION_MENU_UI])
		for (i = 0; i < saveset.ffuiset->special_text->amount; i++)
			saveset.ffuiset->special_text->text_block[i].UpdateOffset();

	fstream filebase, filedest;
	// p0data2 : battle files
	if (assetformat == 0) {
		if (section[DATA_SECTION_ENMY]) {
			fname = config.steam_dir_assets + "p0data2.bin";
			filebase.open(fname.c_str(), ios::in | ios::binary);
			fname = dirassets + "p0data2.bin";
			filedest.open(fname.c_str(), ios::out | ios::binary);
			if (!filebase.is_open())
				return 2;
			if (!filedest.is_open())
				return 3;
			vector<bool> copylist(config.meta_battle.header_file_amount);
			vector<uint32_t> filenewsize(config.meta_battle.header_file_amount);
			for (i = 0; i < config.meta_battle.header_file_amount; i++) {
				for (j = 0; j < config.enmy_amount; j++)
					if (saveset.enemyset->battle_data[j]->modified && i == config.enmy_battle_file[j]) {
						copylist[i] = false;
						filenewsize[i] = saveset.enemyset->battle_data[j]->size;
						break;
					} else if (saveset.enemyset->battle[j]->modified && i == config.enmy_stat_file[j]) {
						copylist[i] = false;
						filenewsize[i] = saveset.enemyset->battle[j]->size;
						break;
					}
					if (j == config.enmy_amount) {
						copylist[i] = true;
						filenewsize[i] = config.meta_battle.file_size[i];
					}
			}
			vector<uint32_t> unitydataoff = config.meta_battle.Duplicate(filebase, filedest, copylist, filenewsize);
			for (i = 0; i < config.meta_battle.header_file_amount; i++) {
				for (j = 0; j < config.enmy_amount; j++)
					if (saveset.enemyset->battle_data[j]->modified && i == config.enmy_battle_file[j]) {
						filedest.seekg(unitydataoff[i]);
						saveset.enemyset->battle_data[j]->WriteHWS(filedest);
						break;
					} else if (saveset.enemyset->battle[j]->modified && i == config.enmy_stat_file[j]) {
						filedest.seekg(unitydataoff[i]);
						saveset.enemyset->battle[j]->WriteHWS(filedest);
						break;
					}
			}
			filebase.close();
			filedest.close();
		}
	} else if (assetformat == 1) {
		for (i = 0; i < config.enmy_amount; i++) {
			fname = destfolder + config.GetSteamAssetPath(UNITY_ARCHIVE_DATA2, config.enmy_battle_file[i]);
			if (deleteold) remove(fname.c_str());
			if (section[DATA_SECTION_ENMY] && saveset.enemyset->battle_data[i]->modified) {
				MainFrame::MakeDirForFile(fname);
				filedest.open(fname.c_str(), ios::out | ios::binary);
				if (!filedest.is_open())
					return 3;
				saveset.enemyset->battle_data[i]->WriteHWS(filedest);
				filedest.close();
			}
			fname = destfolder + config.GetSteamAssetPath(UNITY_ARCHIVE_DATA2, config.enmy_stat_file[i]);
			if (deleteold) remove(fname.c_str());
			if (section[DATA_SECTION_ENMY] && saveset.enemyset->battle[i]->modified) {
				MainFrame::MakeDirForFile(fname);
				filedest.open(fname.c_str(), ios::out | ios::binary);
				if (!filedest.is_open())
					return 3;
				saveset.enemyset->battle[i]->WriteHWS(filedest);
				filedest.close();
			}
		}
	}

	// p0data3 : world map files
	if (assetformat == 0) {
		if (section[DATA_SECTION_WORLD_MAP] && saveset.worldset->world_data->modified) {
			fname = config.steam_dir_assets + "p0data3.bin";
			filebase.open(fname.c_str(), ios::in | ios::binary);
			fname = dirassets + "p0data3.bin";
			filedest.open(fname.c_str(), ios::out | ios::binary);
			if (!filebase.is_open())
				return 2;
			if (!filedest.is_open())
				return 3;
			vector<bool> copylist(config.meta_world.header_file_amount);
			vector<uint32_t> filenewsize(config.meta_world.header_file_amount);
			for (i = 0; i < config.meta_world.header_file_amount; i++) {
				if (i == config.world_fx_file[0] || i == config.world_fx_file[1]) {
					copylist[i] = false;
					filenewsize[i] = saveset.worldset->world_data->size;
				} else if (i == config.world_disc_file[0] || i == config.world_disc_file[1] || i == config.world_discmr_file[0] || i == config.world_discmr_file[1]) {
					copylist[i] = true; // DEBUG: those files are not replaced but only overwritten for now (no size change)
					filenewsize[i] = config.meta_world.file_size[i];
				} else {
					copylist[i] = true;
					filenewsize[i] = config.meta_world.file_size[i];
				}
			}
			vector<uint32_t> unitydataoff = config.meta_world.Duplicate(filebase, filedest, copylist, filenewsize);
			filedest.seekp(unitydataoff[config.world_fx_file[0]]);
			saveset.worldset->world_data->WriteHWS(filedest, false);
			filedest.seekp(unitydataoff[config.world_fx_file[1]]);
			saveset.worldset->world_data->WriteHWS(filedest, false);
			filedest.seekp(unitydataoff[config.world_disc_file[0]]);
			saveset.worldset->world_data->WriteHWS(filedest, false, 0);
			filedest.seekp(unitydataoff[config.world_disc_file[1]]);
			saveset.worldset->world_data->WriteHWS(filedest, false, 1);
			filedest.seekp(unitydataoff[config.world_discmr_file[0]]);
			saveset.worldset->world_data->WriteHWS(filedest, false, 2);
			filedest.seekp(unitydataoff[config.world_discmr_file[1]]);
			saveset.worldset->world_data->WriteHWS(filedest, false, 3);
			filebase.close();
			filedest.close();
		}
	} else if (assetformat == 1) {

		#define MACRO_SAVE_ASSET_WORLD(FILEID, DISCMR, COPYBASE) \
			fname = destfolder + config.GetSteamAssetPath(UNITY_ARCHIVE_DATA3, config.FILEID); \
			if (deleteold) remove(fname.c_str()); \
			if (section[DATA_SECTION_WORLD_MAP] && saveset.worldset->world_data->modified) { \
				MainFrame::MakeDirForFile(fname); \
				filedest.open(fname.c_str(), ios::out | ios::binary); \
				if (!filedest.is_open()) return 3; \
				if (COPYBASE) \
				{ \
					filebase.seekg(config.meta_world.GetFileOffsetByIndex(config.FILEID)); \
					uint32_t copysize = config.meta_world.GetFileSizeByIndex(config.FILEID); \
					char* buffer = new char[copysize]; \
					filebase.read(buffer, copysize); \
					filedest.write(buffer, copysize); \
					delete[] buffer; \
					filedest.seekp(0); \
				} \
				saveset.worldset->world_data->WriteHWS(filedest, false, DISCMR); \
				filedest.close(); \
			}

		fname = config.steam_dir_assets + "p0data3.bin";
		filebase.open(fname.c_str(), ios::in | ios::binary);
		if (!filebase.is_open())
			return 2;
		MACRO_SAVE_ASSET_WORLD(world_fx_file[0], -1, false)
		MACRO_SAVE_ASSET_WORLD(world_fx_file[1], -1, false)
		MACRO_SAVE_ASSET_WORLD(world_disc_file[0], 0, true)
		MACRO_SAVE_ASSET_WORLD(world_disc_file[1], 1, true)
		MACRO_SAVE_ASSET_WORLD(world_discmr_file[0], 2, true)
		MACRO_SAVE_ASSET_WORLD(world_discmr_file[1], 3, true)
		filebase.close();
	}

	// p0data7 : script files
	if (assetformat == 0) {
		if (section[DATA_SECTION_ENMY] || section[DATA_SECTION_FIELD] || section[DATA_SECTION_WORLD_MAP]) {
			fname = config.steam_dir_assets + "p0data7.bin";
			filebase.open(fname.c_str(), ios::in | ios::binary);
			fname = dirassets + "p0data7.bin";
			filedest.open(fname.c_str(), ios::out | ios::binary);
			if (!filebase.is_open())
				return 2;
			if (!filedest.is_open())
				return 3;
			vector<bool> copylist(config.meta_script.header_file_amount);
			vector<uint32_t> filenewsize(config.meta_script.header_file_amount);
			for (i = 0; i < config.meta_script.header_file_amount; i++) {
				copylist[i] = true;
				filenewsize[i] = config.meta_script.file_size[i];
				if (section[DATA_SECTION_ENMY])
					for (j = 0; j < config.enmy_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.enmy_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.enemyset->script[j]->IsDataModified(lang)) {
									copylist[i] = false;
									filenewsize[i] = saveset.enemyset->script[j]->GetDataSize(lang);
								}
								j = config.enmy_amount;
								break;
							}
				if (copylist[i] && section[DATA_SECTION_FIELD])
					for (j = 0; j < config.field_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.field_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.fieldset->script_data[j]->IsDataModified(lang)) {
									copylist[i] = false;
									filenewsize[i] = saveset.fieldset->script_data[j]->GetDataSize(lang);
								}
								j = config.field_amount;
								break;
							}
				if (copylist[i] && section[DATA_SECTION_WORLD_MAP])
					for (j = 0; j < config.world_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.world_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.worldset->script[j]->IsDataModified(lang)) {
									copylist[i] = false;
									filenewsize[i] = saveset.worldset->script[j]->GetDataSize(lang);
								}
								j = config.world_amount;
								break;
							}
			}
			vector<uint32_t> unitydataoff = config.meta_script.Duplicate(filebase, filedest, copylist, filenewsize);
			for (i = 0; i < config.meta_script.header_file_amount; i++) {
				if (section[DATA_SECTION_ENMY])
					for (j = 0; j < config.enmy_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.enmy_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.enemyset->script[j]->IsDataModified(lang)) {
									filedest.seekg(unitydataoff[i]);
									saveset.enemyset->script[j]->WriteHWS(filedest, lang);
								}
								j = config.enmy_amount;
								break;
							}
				if (section[DATA_SECTION_FIELD])
					for (j = 0; j < config.field_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.field_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.fieldset->script_data[j]->IsDataModified(lang)) {
									filedest.seekg(unitydataoff[i]);
									saveset.fieldset->script_data[j]->WriteHWS(filedest, lang);
								}
								j = config.field_amount;
								break;
							}
				if (section[DATA_SECTION_WORLD_MAP])
					for (j = 0; j < config.world_amount; j++)
						for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
							if (i == config.world_script_file[lang][j]) {
								if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.worldset->script[j]->IsDataModified(lang)) {
									filedest.seekg(unitydataoff[i]);
									saveset.worldset->script[j]->WriteHWS(filedest, lang);
								}
								j = config.world_amount;
								break;
							}
			}
			filebase.close();
			filedest.close();
		}
	} else if (assetformat == 1) {

		#define MACRO_SAVE_ASSET_SCRIPT(SEC, AMOUNT, DATA, FILEID) \
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) \
				for (i = 0; i < config.AMOUNT; i++) { \
					fname = destfolder + config.GetSteamAssetPath(UNITY_ARCHIVE_DATA7, config.FILEID[lang][i]); \
					if (deleteold) remove(fname.c_str()); \
					if (section[SEC] && hades::STEAM_LANGUAGE_SAVE_LIST[lang] && saveset.DATA[i]->IsDataModified(lang)) { \
						MainFrame::MakeDirForFile(fname); \
						filedest.open(fname.c_str(), ios::out | ios::binary); \
						if (!filedest.is_open()) return 3; \
						saveset.DATA[i]->WriteHWS(filedest, lang); \
						filedest.close(); \
					} \
				}

		MACRO_SAVE_ASSET_SCRIPT(DATA_SECTION_ENMY, enmy_amount, enemyset->script, enmy_script_file)
		MACRO_SAVE_ASSET_SCRIPT(DATA_SECTION_FIELD, field_amount, fieldset->script_data, field_script_file)
		MACRO_SAVE_ASSET_SCRIPT(DATA_SECTION_WORLD_MAP, world_amount, worldset->script, world_script_file)
	}
	
	// p0data11 to 19 : field files
	if (assetformat == 0) {
		if (section[DATA_SECTION_FIELD]) {
			for (int fieldi = 0; fieldi < 9; fieldi++) {
				bool duplicate = false;
				stringstream ffieldname;
				ffieldname << "p0data1" << (fieldi + 1) << ".bin";
				vector<bool> copylist(config.meta_field[fieldi].header_file_amount);
				vector<uint32_t> filenewsize(config.meta_field[fieldi].header_file_amount);
				for (i = 0; i < config.meta_field[fieldi].header_file_amount; i++) {
					copylist[i] = true;
					filenewsize[i] = config.meta_field[fieldi].file_size[i];
					for (j = 0; j < config.field_amount; j++) {
						if (config.field_file_id[j] == 0 || fieldi + 1 != config.field_file_id[j])
							continue;
						if (i == config.field_tiles_file[j][0]) {
							if (saveset.fieldset->background_data[j]->modified) {
								copylist[i] = false;
								filenewsize[i] = saveset.fieldset->background_data[j]->size;
								duplicate = true;
							}
							break;
						} else if (i == config.field_walkmesh_file[j]) {
							if (saveset.fieldset->walkmesh[j]->modified) {
								copylist[i] = false;
								filenewsize[i] = saveset.fieldset->walkmesh[j]->size;
								duplicate = true;
							}
							break;
						}
					}
				}
				if (!duplicate)
					continue;
				fname = config.steam_dir_assets + ffieldname.str();
				filebase.open(fname.c_str(), ios::in | ios::binary);
				fname = dirassets + ffieldname.str();
				filedest.open(fname.c_str(), ios::out | ios::binary);
				if (!filebase.is_open())
					return 2;
				if (!filedest.is_open())
					return 3;
				vector<uint32_t> unitydataoff = config.meta_field[fieldi].Duplicate(filebase, filedest, copylist, filenewsize);
				for (i = 0; i < config.meta_field[fieldi].header_file_amount; i++) {
					for (j = 0; j < config.field_amount; j++) {
						if (config.field_file_id[j] == 0 || fieldi + 1 != config.field_file_id[j])
							continue;
						if (i == config.field_tiles_file[j][0]) {
							if (saveset.fieldset->background_data[j]->modified) {
								filedest.seekg(unitydataoff[i]);
								saveset.fieldset->background_data[j]->WriteHWS(filedest);
							}
							break;
						} else if (i == config.field_walkmesh_file[j]) {
							if (saveset.fieldset->walkmesh[j]->modified) {
								filedest.seekg(unitydataoff[i]);
								saveset.fieldset->walkmesh[j]->WriteHWS(filedest);
							}
							break;
						}
					}
				}
				filebase.close();
				filedest.close();
			}
		}
	} else if (assetformat == 1) {
		for (i = 0; i < config.field_amount; i++) {
			if (config.field_file_id[i] == 0)
				continue;
			UnityArchiveFile archtype = (UnityArchiveFile)(UNITY_ARCHIVE_DATA11 + config.field_file_id[i] - 1);
			fname = destfolder + config.GetSteamAssetPath(archtype, config.field_tiles_file[i][0]);
			if (deleteold) remove(fname.c_str());
			if (section[DATA_SECTION_FIELD] && saveset.fieldset->background_data[i]->modified) {
				MainFrame::MakeDirForFile(fname);
				filedest.open(fname.c_str(), ios::out | ios::binary);
				if (!filedest.is_open())
					return 3;
				saveset.fieldset->background_data[i]->WriteHWS(filedest);
				filedest.close();
			}
			fname = destfolder + config.GetSteamAssetPath(archtype, config.field_walkmesh_file[i]);
			if (deleteold) remove(fname.c_str());
			if (section[DATA_SECTION_FIELD] && saveset.fieldset->walkmesh[i]->modified) {
				MainFrame::MakeDirForFile(fname);
				filedest.open(fname.c_str(), ios::out | ios::binary);
				if (!filedest.is_open())
					return 3;
				saveset.fieldset->walkmesh[i]->WriteHWS(filedest);
				filedest.close();
			}
		}
	}
	
	// resources.assets : text and card files
	if (assetformat == 0) {
		fname = config.steam_dir_data + "resources.assets";
		filebase.open(fname.c_str(), ios::in | ios::binary);
		fname = dirdata + "resources.assets";
		filedest.open(fname.c_str(), ios::out | ios::binary);
		if (!filebase.is_open())
			return 2;
		if (!filedest.is_open())
			return 3;
		vector<bool> copylist(config.meta_res.header_file_amount);
		vector<uint32_t> filenewsize(config.meta_res.header_file_amount);
		for (i = 0; i < config.meta_res.header_file_amount; i++) {
			copylist[i] = true;
			filenewsize[i] = config.meta_res.file_size[i];

			#define MACRO_STEAM_CHECKFILEUPDATE(FILEID,CONDITION,SIZE) \
				if (CONDITION && i==config.FILEID) { \
					copylist[i] = false; \
					filenewsize[i] = SIZE; \
				}

			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang])
					continue;
				MACRO_STEAM_CHECKFILEUPDATE(spell_name_file[lang], section[DATA_SECTION_SPELL], saveset.spellset->GetSteamTextSize(0, lang))
				MACRO_STEAM_CHECKFILEUPDATE(spell_help_file[lang], section[DATA_SECTION_SPELL], saveset.spellset->GetSteamTextSize(1, lang))
				MACRO_STEAM_CHECKFILEUPDATE(cmd_name_file[lang], section[DATA_SECTION_CMD], saveset.cmdset->GetSteamTextSize(0, lang))
				MACRO_STEAM_CHECKFILEUPDATE(cmd_help_file[lang], section[DATA_SECTION_CMD], saveset.cmdset->GetSteamTextSize(1, lang))
				MACRO_STEAM_CHECKFILEUPDATE(support_name_file[lang], section[DATA_SECTION_SUPPORT], saveset.supportset->GetSteamTextSize(0, lang))
				MACRO_STEAM_CHECKFILEUPDATE(support_help_file[lang], section[DATA_SECTION_SUPPORT], saveset.supportset->GetSteamTextSize(1, lang))
				MACRO_STEAM_CHECKFILEUPDATE(item_name_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(0, lang))
				MACRO_STEAM_CHECKFILEUPDATE(item_help_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(1, lang))
				MACRO_STEAM_CHECKFILEUPDATE(item_help2_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(2, lang))
				MACRO_STEAM_CHECKFILEUPDATE(itemkey_name_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(3, lang))
				MACRO_STEAM_CHECKFILEUPDATE(itemkey_help_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(4, lang))
				MACRO_STEAM_CHECKFILEUPDATE(itemkey_desc_file[lang], section[DATA_SECTION_ITEM], saveset.itemset->GetSteamTextSize(5, lang))
				MACRO_STEAM_CHECKFILEUPDATE(field_text_file[lang], section[DATA_SECTION_FIELD], saveset.fieldset->GetSteamTextSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(world_text_file[lang], section[DATA_SECTION_WORLD_MAP], saveset.worldset->GetSteamTextSize(0, lang))
				MACRO_STEAM_CHECKFILEUPDATE(world_worldplace_name_file[lang], section[DATA_SECTION_WORLD_MAP], saveset.worldset->GetSteamTextSize(1, lang))
				MACRO_STEAM_CHECKFILEUPDATE(card_name_file[lang], section[DATA_SECTION_CARD], saveset.cardset->GetSteamTextSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_battleinfo_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[0].GetDataSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_battlescan_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[1].GetDataSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_spellnaming_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[2].GetDataSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_chocomenu_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[3].GetDataSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_cardrank_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[4].GetDataSize(lang))
				MACRO_STEAM_CHECKFILEUPDATE(spetext_tetramaster_file[lang], section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[5].GetDataSize(lang))
				for (j = 0; j < config.enmy_amount; j++) {
					MACRO_STEAM_CHECKFILEUPDATE(enmy_text_file[lang][j], section[DATA_SECTION_ENMY] && saveset.enemyset->text[j]->modified, saveset.enemyset->text[j]->GetDataSize(lang))
				}
				for (j = 0; j < config.text_amount; j++) {
					MACRO_STEAM_CHECKFILEUPDATE(text_file[lang][j], section[DATA_SECTION_TEXT] && saveset.textset->text_data[j]->modified, saveset.textset->text_data[j]->GetDataSize(lang))
				}
			}
			MACRO_STEAM_CHECKFILEUPDATE(spetext_localization_file, section[DATA_SECTION_MENU_UI], saveset.ffuiset->special_text->text_block[6].GetDataSize())
			MACRO_STEAM_CHECKFILEUPDATE(card_stat_file, section[DATA_SECTION_CARD], 5 * CARD_AMOUNT)
			MACRO_STEAM_CHECKFILEUPDATE(card_deck_file, section[DATA_SECTION_CARD], 2 * CARD_DECK_AMOUNT)
			MACRO_STEAM_CHECKFILEUPDATE(card_set_file, section[DATA_SECTION_CARD], CARD_SET_AMOUNT*CARD_SET_CAPACITY)
			for (j = 0; j < SPELL_ANIMATION_AMOUNT; j++) {
				MACRO_STEAM_CHECKFILEUPDATE(spellanim_steam_file[j], section[DATA_SECTION_SPELL_ANIM] && saveset.spellanimset->spell[j].modified_data != 0, saveset.spellanimset->spell[j].raw_size)
			}
		}
		vector<uint32_t> unitydataoff = config.meta_res.Duplicate(filebase, filedest, copylist, filenewsize);
		for (i = 0; i < config.meta_res.header_file_amount; i++) {

			#define MACRO_STEAM_WRITEFILEUPDATE(FILEID,SAVEFUNC,BREAK) \
				if (i==config.FILEID) { \
					filedest.seekg(unitydataoff[i]); \
					saveset.SAVEFUNC; \
					if (BREAK) \
						break; \
				}

			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (!hades::STEAM_LANGUAGE_SAVE_LIST[lang])
					continue;
				if (section[DATA_SECTION_SPELL]) {
					MACRO_STEAM_WRITEFILEUPDATE(spell_name_file[lang], spellset->WriteSteamText(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spell_help_file[lang], spellset->WriteSteamText(filedest, 1, lang), false)
				}
				if (section[DATA_SECTION_CMD]) {
					MACRO_STEAM_WRITEFILEUPDATE(cmd_name_file[lang], cmdset->WriteSteamText(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(cmd_help_file[lang], cmdset->WriteSteamText(filedest, 1, lang), false)
				}
				if (section[DATA_SECTION_ENMY])
					for (j = 0; j < config.enmy_amount; j++)
						if (saveset.enemyset->text[j]->modified) {
							MACRO_STEAM_WRITEFILEUPDATE(enmy_text_file[lang][j], enemyset->text[j]->WriteSteam(filedest, lang), true)
						}
				if (section[DATA_SECTION_TEXT])
					for (j = 0; j < config.text_amount; j++)
						if (saveset.textset->text_data[j]->modified) {
							MACRO_STEAM_WRITEFILEUPDATE(text_file[lang][j], textset->text_data[j]->WriteSteam(filedest, lang), true)
						}
				if (section[DATA_SECTION_WORLD_MAP]) {
					MACRO_STEAM_WRITEFILEUPDATE(world_text_file[lang], worldset->WriteSteamText(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(world_worldplace_name_file[lang], worldset->WriteSteamText(filedest, 1, lang), false)
				}
				if (section[DATA_SECTION_ITEM]) {
					MACRO_STEAM_WRITEFILEUPDATE(item_name_file[lang], itemset->WriteSteamText(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(item_help_file[lang], itemset->WriteSteamText(filedest, 1, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(item_help2_file[lang], itemset->WriteSteamText(filedest, 2, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(itemkey_name_file[lang], itemset->WriteSteamText(filedest, 3, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(itemkey_help_file[lang], itemset->WriteSteamText(filedest, 4, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(itemkey_desc_file[lang], itemset->WriteSteamText(filedest, 5, lang), false)
				}
				if (section[DATA_SECTION_SUPPORT]) {
					MACRO_STEAM_WRITEFILEUPDATE(support_name_file[lang], supportset->WriteSteamText(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(support_help_file[lang], supportset->WriteSteamText(filedest, 1, lang), false)
				}
				if (section[DATA_SECTION_CARD]) {
					MACRO_STEAM_WRITEFILEUPDATE(card_name_file[lang], cardset->WriteSteamText(filedest, lang), false)
				}
				if (section[DATA_SECTION_FIELD]) {
					MACRO_STEAM_WRITEFILEUPDATE(field_text_file[lang], fieldset->WriteSteamText(filedest, lang), false)
				}
				if (section[DATA_SECTION_MENU_UI]) {
					MACRO_STEAM_WRITEFILEUPDATE(spetext_battleinfo_file[lang], ffuiset->special_text->WriteSteam(filedest, 0, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spetext_battlescan_file[lang], ffuiset->special_text->WriteSteam(filedest, 1, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spetext_spellnaming_file[lang], ffuiset->special_text->WriteSteam(filedest, 2, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spetext_chocomenu_file[lang], ffuiset->special_text->WriteSteam(filedest, 3, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spetext_cardrank_file[lang], ffuiset->special_text->WriteSteam(filedest, 4, lang), false)
					MACRO_STEAM_WRITEFILEUPDATE(spetext_tetramaster_file[lang], ffuiset->special_text->WriteSteam(filedest, 5, lang), false)
				}
			}
			if (section[DATA_SECTION_MENU_UI]) {
				MACRO_STEAM_WRITEFILEUPDATE(spetext_localization_file, ffuiset->special_text->WriteSteam(filedest, 6), false)
			}
			if (section[DATA_SECTION_CARD]) {
				MACRO_STEAM_WRITEFILEUPDATE(card_stat_file, cardset->WriteSteamData(filedest, 0), false)
				MACRO_STEAM_WRITEFILEUPDATE(card_deck_file, cardset->WriteSteamData(filedest, 1), false)
				MACRO_STEAM_WRITEFILEUPDATE(card_set_file, cardset->WriteSteamData(filedest, 2), false)
			}
			if (section[DATA_SECTION_SPELL_ANIM])
				for (j = 0; j < SPELL_ANIMATION_AMOUNT; j++)
					if (saveset.spellanimset->spell[j].modified_data != 0) {
						MACRO_STEAM_WRITEFILEUPDATE(spellanim_steam_file[j], spellanimset->spell[j].Write(filedest), true)
					}
		}
		filebase.close();
		filedest.close();
	} else if (assetformat == 1) {

		#define MACRO_SAVE_ASSET_RESOURCES(FILEID, CONDITION, SAVEFUNC) \
			fname = destfolder + config.GetSteamAssetPath(UNITY_ARCHIVE_RESOURCES, config.FILEID); \
			if (deleteold) remove(fname.c_str()); \
			if (CONDITION) { \
				MainFrame::MakeDirForFile(fname); \
				filedest.open(fname.c_str(), ios::out | ios::binary); \
				if (!filedest.is_open()) return 3; \
				saveset.SAVEFUNC; \
				filedest.close(); \
			}

		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			MACRO_SAVE_ASSET_RESOURCES(spell_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_SPELL], spellset->WriteSteamText(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(spell_help_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_SPELL], spellset->WriteSteamText(filedest, 1, lang))
			MACRO_SAVE_ASSET_RESOURCES(cmd_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_CMD], cmdset->WriteSteamText(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(cmd_help_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_CMD], cmdset->WriteSteamText(filedest, 1, lang))
			for (i = 0; i < config.enmy_amount; i++) {
				MACRO_SAVE_ASSET_RESOURCES(enmy_text_file[lang][i], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ENMY] && saveset.enemyset->text[i]->modified, enemyset->text[i]->WriteSteam(filedest, lang))
			}
			for (i = 0; i < config.text_amount; i++) {
				MACRO_SAVE_ASSET_RESOURCES(text_file[lang][i], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_TEXT] && saveset.textset->text_data[i]->modified, textset->text_data[i]->WriteSteam(filedest, lang))
			}
			MACRO_SAVE_ASSET_RESOURCES(world_text_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_WORLD_MAP], worldset->WriteSteamText(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(world_worldplace_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_WORLD_MAP], worldset->WriteSteamText(filedest, 1, lang))
			MACRO_SAVE_ASSET_RESOURCES(item_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(item_help_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 1, lang))
			MACRO_SAVE_ASSET_RESOURCES(item_help2_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 2, lang))
			MACRO_SAVE_ASSET_RESOURCES(itemkey_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 3, lang))
			MACRO_SAVE_ASSET_RESOURCES(itemkey_help_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 4, lang))
			MACRO_SAVE_ASSET_RESOURCES(itemkey_desc_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_ITEM], itemset->WriteSteamText(filedest, 5, lang))
			MACRO_SAVE_ASSET_RESOURCES(support_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_SUPPORT], supportset->WriteSteamText(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(support_help_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_SUPPORT], supportset->WriteSteamText(filedest, 1, lang))
			MACRO_SAVE_ASSET_RESOURCES(card_name_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_CARD], cardset->WriteSteamText(filedest, lang))
			MACRO_SAVE_ASSET_RESOURCES(field_text_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_FIELD], fieldset->WriteSteamText(filedest, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_battleinfo_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 0, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_battlescan_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 1, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_spellnaming_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 2, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_chocomenu_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 3, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_cardrank_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 4, lang))
			MACRO_SAVE_ASSET_RESOURCES(spetext_tetramaster_file[lang], hades::STEAM_LANGUAGE_SAVE_LIST[lang] && section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 5, lang))
		}
		MACRO_SAVE_ASSET_RESOURCES(spetext_localization_file, section[DATA_SECTION_MENU_UI], ffuiset->special_text->WriteSteam(filedest, 6))
		MACRO_SAVE_ASSET_RESOURCES(card_stat_file, section[DATA_SECTION_CARD], cardset->WriteSteamData(filedest, 0))
		MACRO_SAVE_ASSET_RESOURCES(card_deck_file, section[DATA_SECTION_CARD], cardset->WriteSteamData(filedest, 1))
		MACRO_SAVE_ASSET_RESOURCES(card_set_file, section[DATA_SECTION_CARD], cardset->WriteSteamData(filedest, 2))
		for (i = 0; i < SPELL_ANIMATION_AMOUNT; i++)
			if (config.spellanim_steam_file[i]>=0) {
				MACRO_SAVE_ASSET_RESOURCES(spellanim_steam_file[i], section[DATA_SECTION_SPELL_ANIM] && saveset.spellanimset->spell[i].modified_data != 0, spellanimset->spell[i].Write(filedest))
			}
	}
	
	// Assembly-CSharp.dll : everything else...
	if (dllformat == 0) {
		unsigned int dllmodifcount, dllmodifmax, sectionmodifcount;
		dllmodifmax = 100; // DEBUG: arbitrary number higher than number of modifications (except CIL Code modifs); consider using vector<>
		if (section[DATA_SECTION_CIL]) {
			dllmodifmax += saveset.cilset->rawmodifamount;
			for (i=0;i<saveset.cilset->macromodifamount;i++)
				dllmodifmax += saveset.cilset->macromodif[i].info->GetMethodCount();
		}
		DllMetaDataModification* dllmodif = new DllMetaDataModification[dllmodifmax];
		DllMetaDataModification* sectionmodif;
		dllmodifcount = 0;
		if (section[DATA_SECTION_CIL]) {
			for (i=0;i<saveset.cilset->rawmodifamount;i++) {
				dllmodif[dllmodifcount] = saveset.cilset->rawmodif[i];
				dllmodifcount++;
			}
			for (i=0;i<saveset.cilset->macromodifamount;i++) {
				sectionmodif = saveset.cilset->macromodif[i].info->ComputeModifications(&sectionmodifcount);
				memcpy(&dllmodif[dllmodifcount],sectionmodif,sectionmodifcount*sizeof(DllMetaDataModification));
				delete[] sectionmodif;
				dllmodifcount += sectionmodifcount;
			}
		}
		
		#define MACRO_DLL_ADD_MODIF(SECTION,SET) \
			if (section[SECTION]) { \
				sectionmodif = saveset.SET->ComputeSteamMod(config,&sectionmodifcount); \
				memcpy(&dllmodif[dllmodifcount],sectionmodif,sectionmodifcount*sizeof(DllMetaDataModification)); \
				delete[] sectionmodif; \
				dllmodifcount += sectionmodifcount; \
			}
		
		MACRO_DLL_ADD_MODIF(DATA_SECTION_SPELL,spellset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_CMD,cmdset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_SHOP,shopset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_ITEM,itemset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_SUPPORT,supportset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_STAT,statset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_PARTY_SPECIAL,partyspecialset)
		MACRO_DLL_ADD_MODIF(DATA_SECTION_ENMY,enemyset)
		if (dllmodifcount>0) {
			fname = dirmanaged+"Assembly-CSharp.dll";
			filedest.open(fname.c_str(),ios::out | ios::binary);
			if (!filedest.is_open()) {
				delete[] dllmodif;
				return 3;
			}
			config.meta_dll.Duplicate(filedest,dllmodifcount,dllmodif);
			filedest.close();
		}
		delete[] dllmodif;
	} else if (dllformat == 1) {
		if (section[DATA_SECTION_SPELL]) {
			fname = dirassets + HADES_STRING_CSV_SPELL_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_STATUS_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_SPELLTITLE_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.spellset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_CMD]) {
			fname = dirassets + HADES_STRING_CSV_COMMAND_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.cmdset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_SHOP]) {
			fname = dirassets + HADES_STRING_CSV_SHOP_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_SYNTHESIS_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.shopset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_ITEM]) {
			fname = dirassets + HADES_STRING_CSV_ITEM_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_WEAPON_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_ARMOR_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_USABLE_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_ITEMSTAT_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.itemset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_SUPPORT]) {
			fname = dirassets + HADES_STRING_CSV_SUPPORT_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.supportset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_STAT]) {
			fname = dirassets + HADES_STRING_CSV_STATINIT_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_STATCMD_HEADER;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_STATEQUIP_FILE;
			MainFrame::MakeDirForFile(fname);
			fname = dirassets + HADES_STRING_CSV_STATLEVEL_FILE;
			MainFrame::MakeDirForFile(fname);
			for (i=0; i<G_N_ELEMENTS(HADES_STRING_CSV_STATABIL_FILE); i++) {
				fname = dirassets + HADES_STRING_CSV_STATABIL_FILE[i];
				MainFrame::MakeDirForFile(fname);
			}
			if (!saveset.statset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_PARTY_SPECIAL]) {
			fname = dirassets + HADES_STRING_CSV_MGCSWORD_FILE;
			MainFrame::MakeDirForFile(fname);
			if (!saveset.partyspecialset->GenerateCSV(dirassets)) return 3;
		}
		if (section[DATA_SECTION_ENMY] && saveset.enemyset->modified_battle_scene_amount>0) {
//			fname = destfolder + HADES_STRING_DICTIONARY_PATCH_FILE;
//			MainFrame::MakeDirForFile(fname);
			if (!saveset.enemyset->GenerateCSV(destfolder)) return 3;
		}
		if (section[DATA_SECTION_CIL]) result = -1;
	} else if (dllformat == 2) {
		vector<string> csharpmodifications;
		if (section[DATA_SECTION_CIL]) saveset.cilset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_SPELL]) saveset.spellset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_CMD]) saveset.cmdset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_SHOP]) saveset.shopset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_ITEM]) saveset.itemset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_SUPPORT]) saveset.supportset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_STAT]) saveset.statset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_PARTY_SPECIAL]) saveset.partyspecialset->GenerateCSharp(csharpmodifications);
		if (section[DATA_SECTION_ENMY]) saveset.enemyset->GenerateCSharp(csharpmodifications);
		if (csharpmodifications.size()>0) {
			fname = destfolder + "Assembly-CSharp_Additions.cs";
			filedest.open(fname.c_str(),ios::out);
			if (!filedest.is_open())
				return 3;
			filedest << HADES_STRING_STEAM_SAVE_CSHARP_PIECES_HEADER;
			for (i = 0; i < csharpmodifications.size(); i++) {
				filedest << "\n" << std::endl;
				filedest << csharpmodifications[i];
			}
			filedest << "\n\n\n";
			filedest.close();
		}
	}
/*	if (deleteold) {
		// Clear empty directories
		MainFrame::DeleteFullDir(dirassets);
		MainFrame::DeleteFullDir(dirdata);
		MainFrame::DeleteFullDir(dirmanaged);
		MainFrame::DeleteFullDir(destfolder + "p0data11\\");
		MainFrame::DeleteFullDir(destfolder + "p0data12\\");
		MainFrame::DeleteFullDir(destfolder + "p0data13\\");
		MainFrame::DeleteFullDir(destfolder + "p0data14\\");
		MainFrame::DeleteFullDir(destfolder + "p0data15\\");
		MainFrame::DeleteFullDir(destfolder + "p0data16\\");
		MainFrame::DeleteFullDir(destfolder + "p0data17\\");
		MainFrame::DeleteFullDir(destfolder + "p0data18\\");
		MainFrame::DeleteFullDir(destfolder + "p0data19\\");
		MainFrame::DeleteFullDir(destfolder + "p0data2\\");
		MainFrame::DeleteFullDir(destfolder + "p0data3\\");
		MainFrame::DeleteFullDir(destfolder + "p0data7\\");
	}*/
	return result;
}

//==================================================//
//                   SAVE FILES                     //
//==================================================//
static string SAVE_HEADER_PSX = "HWSA";
static string SAVE_HEADER_STEAM = "HWSS";

/////////////
// Reading //
/////////////
void UnusedSaveBackupPart::Add(fstream& f, uint32_t size) {
	if (GetGameType()!=GAME_TYPE_PSX) {
		f.seekg(size,ios::cur);
		return;
	}
	unsigned int i;
	uint32_t* sstmp = new uint32_t[save_amount+1];
	uint8_t** sdtmp = new uint8_t*[save_amount+1];
	for (i=0;i<save_amount;i++) {
		sstmp[i] = save_size[i];
		sdtmp[i] = save_data[i];
	}
	if (save_size)
		delete[] save_size;
	if (save_data)
		delete[] save_data;
	save_size = sstmp;
	save_data = sdtmp;
	save_size[save_amount] = size;
	save_data[save_amount] = new uint8_t[size];
	for (i=0;i<size;i++)
		HWSReadChar(f,save_data[save_amount][i]);
	save_amount++;
}

int PreloadHWS(string filepath, bool* section) {
	unsigned int i;
	GameType hwsgt = GAME_TYPE_PSX;
	fstream save(filepath.c_str(),ios::in|ios::binary);
	if (!save.is_open())
		return 1;
	char buffer[5];
	buffer[4] = 0;
	save.read(buffer,4);
	if (SAVE_HEADER_PSX.compare(buffer)) {
		hwsgt = GAME_TYPE_STEAM;
		if (SAVE_HEADER_STEAM.compare(buffer))
			return 2;
	}
	for (i=0;i<DATA_SECTION_AMOUNT;i++)
		section[i] = false;
	uint32_t nbsection,tmp32;
	uint8_t sectiontype;
	HWSReadLong(save,nbsection);
	for (i=0;i<nbsection;i++) {
		HWSReadChar(save,sectiontype);
		HWSReadLong(save,tmp32);
		save.seekg(tmp32,ios::cur);
		if (sectiontype<DATA_SECTION_AMOUNT)
			section[sectiontype] = true;
	}
	save.close();
	return 0;
}

wstring* LoadHWS(string filepath, bool* section, bool* sectext, bool* localsec, SaveSet& saveset, UnusedSaveBackup& backup) {
	unsigned int i;
	bool problem = false;
	wstringstream problemres;
	problemres << HADES_STRING_HWS_OPEN_WARNING;
	fstream save(filepath.c_str(),ios::in|ios::binary);
	wstring* res = new wstring[2];
	if (!save.is_open()) {
		res[0] = HADES_STRING_OPEN_ERROR_FAIL_NF;
		res[1] = HADES_STRING_ERROR;
		return res;
	}
	char buffer[5];
	buffer[4] = 0;
	save.read(buffer,4);
	if (SAVE_HEADER_PSX.compare(buffer)==0) {
		SetHWSGameType(GAME_TYPE_PSX);
	} else if (SAVE_HEADER_STEAM.compare(buffer)==0) {
		SetHWSGameType(GAME_TYPE_STEAM);
	} else {
		save.close();
		res[0] = HADES_STRING_OPEN_ERROR_FAIL_NF;
		res[1] = HADES_STRING_ERROR;
		return res;
	}
	uint32_t nbsection,sectionpos;
	HWSReadLong(save,nbsection);
	uint8_t* sectiontype = new uint8_t[nbsection];
	uint32_t* sectionlength = new uint32_t[nbsection];
	unsigned int enmyload = section[DATA_SECTION_ENMY] | (localsec[0] << 1);
	unsigned int worldload = section[DATA_SECTION_WORLD_MAP] | (localsec[1] << 1);
	unsigned int fieldload = section[DATA_SECTION_FIELD] | (localsec[2] << 1);
	section[DATA_SECTION_ENMY] |= localsec[0];
	section[DATA_SECTION_WORLD_MAP] |= localsec[1];
	section[DATA_SECTION_FIELD] |= localsec[2];
//	LoadingDialogInit(nbsection,_(L"Importing HWS datas..."));
	for (i=0;i<nbsection;i++) {
		HWSReadChar(save,sectiontype[i]);
		HWSReadLong(save,sectionlength[i]);
		sectionpos = save.tellg();
		if (section[sectiontype[i]]) {
			
			#define MACRO_HANDLE_FLAG_PROBLEM(flag,str) \
				if (res & flag) { \
					problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << str; \
					problem = true; \
				}
			#define MACRO_HANDLE_QUANTIFIED_PROBLEM(index,str) \
				if (res[index]) { \
					problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << res[index] << str; \
					problem = true; \
				}
			
			if (sectiontype[i]==DATA_SECTION_SPELL) {
				int res = saveset.spellset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_SPELL_NAME)
				MACRO_HANDLE_FLAG_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_SPELL_HELP)
			} else if (sectiontype[i]==DATA_SECTION_CMD) {
				int res = saveset.cmdset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_CMD_NAME)
				MACRO_HANDLE_FLAG_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_CMD_HELP)
			} else if (sectiontype[i]==DATA_SECTION_ENMY) {
				int* res = saveset.enemyset->LoadHWS(save,backup.enemy,sectext[sectiontype[i]],enmyload);
				MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_ENEMY_SIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_ENEMY_UNKNOWN)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_ENEMY_UNUSED)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(3,HADES_STRING_HWS_OPEN_WARNING_ENEMY_PRELOAD_UNAVAILABLE)
				delete[] res;
			} else if (sectiontype[i]==DATA_SECTION_SHOP) {
				saveset.shopset->LoadHWS(save,sectionlength[i]>SHOP_AMOUNT*SHOP_ITEM_AMOUNT);
			} else if (sectiontype[i]==DATA_SECTION_TEXT) {
				int* res = saveset.textset->LoadHWS(save,backup.text);
				MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_TEXT_SIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_TEXT_UNKNOWN)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_TEXT_UNUSED)
				delete[] res;
			} else if (sectiontype[i]==DATA_SECTION_BATTLE_SCENE) {
				if (GetGameType()==GAME_TYPE_PSX) {
					int* res = saveset.sceneset->LoadHWS(save,backup.scene);
					MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_SIZE)
					MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNKNOWN)
					MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNUSED)
					delete[] res;
				} else {
					problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNAVAILABLE;
					problem = true;
				}
			} else if (sectiontype[i]==DATA_SECTION_WORLD_MAP) {
				int* res = saveset.worldset->LoadHWS(save,backup.world,sectext[sectiontype[i]],worldload);
				MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_SIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_UNKNOWN)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_UNUSED)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(3,HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_PARENTSIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(4,HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_PRELOAD_UNAVAILABLE)
				delete[] res;
			} else if (sectiontype[i]==DATA_SECTION_ITEM) {
				int res = saveset.itemset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(0x1,HADES_STRING_HWS_OPEN_WARNING_ITEM_NAME)
				MACRO_HANDLE_FLAG_PROBLEM(0x2,HADES_STRING_HWS_OPEN_WARNING_ITEM_HELP)
				MACRO_HANDLE_FLAG_PROBLEM(0x4,HADES_STRING_HWS_OPEN_WARNING_ITEM_HELP2)
				MACRO_HANDLE_FLAG_PROBLEM(0x8,HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_NAME)
				MACRO_HANDLE_FLAG_PROBLEM(0x10,HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_HELP)
				MACRO_HANDLE_FLAG_PROBLEM(0x20,HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_DESC)
			} else if (sectiontype[i]==DATA_SECTION_SUPPORT) {
				int res = saveset.supportset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_SUPPORT_NAME)
				MACRO_HANDLE_FLAG_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_SUPPORT_HELP)
			} else if (sectiontype[i]==DATA_SECTION_STAT) {
				int res = saveset.statset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_STAT_DEFAULTNAME)
			} else if (sectiontype[i]==DATA_SECTION_CARD) {
				int res = saveset.cardset->LoadHWS(save,sectext[sectiontype[i]]);
				MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_CARD_NAME)
			} else if (sectiontype[i]==DATA_SECTION_FIELD) {
				int* res = saveset.fieldset->LoadHWS(save,backup.field,sectext[sectiontype[i]],fieldload);
				MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_FIELD_SIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_FIELD_UNKNOWN)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_FIELD_UNUSED)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(3,HADES_STRING_HWS_OPEN_WARNING_FIELD_PARENTSIZE)
				MACRO_HANDLE_QUANTIFIED_PROBLEM(4,HADES_STRING_HWS_OPEN_WARNING_FIELD_PRELOAD_UNAVAILABLE)
				delete[] res;
			} else if (sectiontype[i]==DATA_SECTION_SPELL_ANIM) {
				int* res = saveset.spellanimset->LoadHWS(save);
				MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_SPELL_ANIMATION_SIZE)
				delete[] res;
			} else if (sectiontype[i]==DATA_SECTION_MENU_UI) {
				if (GetGameType()==GetHWSGameType()) {
					int* res = saveset.ffuiset->LoadHWS(save,backup.menu_ui,true);
					MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_SIZE)
					MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_UNKNOWN)
					delete[] res;
				} else {
					problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_UNAVAILABLE;
					problem = true;
				}
			} else if (sectiontype[i]==DATA_SECTION_PARTY_SPECIAL) {
				saveset.partyspecialset->LoadHWS(save);
			} else if (sectiontype[i]==DATA_SECTION_ASSEMBLY) {
				if (GetHWSGameType()==GAME_TYPE_PSX) { // MIPS
					if (GetGameType()==GAME_TYPE_PSX) {
						int res = saveset.mipsset->LoadHWS(save);
						MACRO_HANDLE_FLAG_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_MIPS_SIZE)
					} else {
						problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << HADES_STRING_HWS_OPEN_WARNING_MIPS_UNAVAILABLE;
						problem = true;
					}
				} else { // CIL
					if (GetGameType()==GAME_TYPE_PSX) {
						problemres << HADES_STRING_HWS_OPEN_WARNING_COMMON << HADES_STRING_HWS_OPEN_WARNING_CIL_UNAVAILABLE;
						problem = true;
					} else {
						int* res = saveset.cilset->LoadHWS(save);
						MACRO_HANDLE_QUANTIFIED_PROBLEM(0,HADES_STRING_HWS_OPEN_WARNING_CIL_RAW_NOT_FOUND)
						MACRO_HANDLE_QUANTIFIED_PROBLEM(1,HADES_STRING_HWS_OPEN_WARNING_CIL_RAW_NOT_MATCHING)
						MACRO_HANDLE_QUANTIFIED_PROBLEM(2,HADES_STRING_HWS_OPEN_WARNING_CIL_MACRO_UNKNOWN)
						MACRO_HANDLE_QUANTIFIED_PROBLEM(3,HADES_STRING_HWS_OPEN_WARNING_CIL_MACRO_FAIL)
						delete[] res;
					}
				}
			}
		}
		save.seekg(sectionpos+sectionlength[i]);
//		LoadingDialogUpdate(i);
	}
//	LoadingDialogEnd();
	save.close();
	if (problem) {
		res[0] = problemres.str();
		res[1] = HADES_STRING_WARNING;
		return res;
	}
	res[0] = HADES_STRING_HWS_OPEN_SUCCESS;
	res[1] = HADES_STRING_SUCCESS;
	return res;
}

int WriteHWS(string filepath, bool* section, bool* localsec, SaveSet& saveset, UnusedSaveBackup& backup) {
	unsigned int i;
	fstream save(filepath.c_str(),ios::out|ios::binary);
	if (!save.is_open())
		return 1;
	if (GetGameType()==GAME_TYPE_PSX)
		save << SAVE_HEADER_PSX.c_str();
	else
		save << SAVE_HEADER_STEAM.c_str();
	uint32_t nbsection = 0;
	for (i=0;i<DATA_SECTION_AMOUNT;i++)
		if (section[i])
			nbsection++;
	if (localsec[0] && !section[DATA_SECTION_ENMY]) nbsection++;
	if (localsec[1] && !section[DATA_SECTION_WORLD_MAP]) nbsection++;
	if (localsec[2] && !section[DATA_SECTION_FIELD]) nbsection++;
	HWSWriteLong(save,nbsection);
	uint32_t sectionpos,sectionlength = 0;
	unsigned int enmysave = section[DATA_SECTION_ENMY] | (localsec[0] << 1);
	unsigned int worldsave = section[DATA_SECTION_WORLD_MAP] | (localsec[1] << 1);
	unsigned int fieldsave = section[DATA_SECTION_FIELD] | (localsec[2] << 1);
	section[DATA_SECTION_ENMY] |= localsec[0];
	section[DATA_SECTION_WORLD_MAP] |= localsec[1];
	section[DATA_SECTION_FIELD] |= localsec[2];
	for (i=0;i<DATA_SECTION_AMOUNT;i++) {
		if (section[i]) {
			HWSWriteChar(save,i);
			HWSWriteLong(save,sectionlength);
			sectionpos = save.tellg();
			if (i==DATA_SECTION_SPELL)
				saveset.spellset->WriteHWS(save);
			else if (i==DATA_SECTION_CMD)
				saveset.cmdset->WriteHWS(save);
			else if (i==DATA_SECTION_ENMY)
				saveset.enemyset->WriteHWS(save,backup.enemy,enmysave);
			else if (i==DATA_SECTION_SHOP)
				saveset.shopset->WriteHWS(save);
			else if (i==DATA_SECTION_TEXT)
				saveset.textset->WriteHWS(save,backup.text);
			else if (i==DATA_SECTION_BATTLE_SCENE)
				saveset.sceneset->WriteHWS(save,backup.scene);
			else if (i==DATA_SECTION_WORLD_MAP)
				saveset.worldset->WriteHWS(save,backup.world,worldsave);
			else if (i==DATA_SECTION_ITEM)
				saveset.itemset->WriteHWS(save);
			else if (i==DATA_SECTION_SUPPORT)
				saveset.supportset->WriteHWS(save);
			else if (i==DATA_SECTION_STAT)
				saveset.statset->WriteHWS(save);
			else if (i==DATA_SECTION_CARD)
				saveset.cardset->WriteHWS(save);
			else if (i==DATA_SECTION_FIELD)
				saveset.fieldset->WriteHWS(save,backup.field,fieldsave);
			else if (i==DATA_SECTION_SPELL_ANIM)
				saveset.spellanimset->WriteHWS(save);
			else if (i==DATA_SECTION_MENU_UI)
				saveset.ffuiset->WriteHWS(save,backup.menu_ui);
			else if (i==DATA_SECTION_PARTY_SPECIAL)
				saveset.partyspecialset->WriteHWS(save);
			else if (i==DATA_SECTION_ASSEMBLY) {
				if (GetGameType()==GAME_TYPE_PSX)
					saveset.mipsset->WriteHWS(save);
				else
					saveset.cilset->WriteHWS(save);
			}
			sectionlength = save.tellg();
			sectionlength -= sectionpos;
			save.seekg(sectionpos-4);
			HWSWriteLong(save,sectionlength);
			save.seekg(sectionpos+sectionlength);
		}
	}
	save.close();
	return 0;
}
