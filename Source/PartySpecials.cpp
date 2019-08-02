#include "PartySpecials.h"

#include "Database_CSV.h"

#define PARTY_SPECIAL_HWS_VERSION 1

#define MACRO_MAGICSWORD_IOFUNCTION(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<MAGIC_SWORD_AMOUNT;i++) \
		IO ## Char(ffbin,magic_sword_requirement[i]); \
	if (PPF) PPFEndScanStep();


void PartySpecialDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	magic_sword[0] = 0x32;	magic_sword[1] = 0x34;	magic_sword[2] = 0x36;
	magic_sword[3] = 0x38;	magic_sword[4] = 0x39;	magic_sword[5] = 0x3B;
	magic_sword[6] = 0x3D;	magic_sword[7] = 0x3F;	magic_sword[8] = 0x41;
	magic_sword[9] = 0x4B;	magic_sword[10] = 0x4C;
	magic_sword[11] = 0xBD;	magic_sword[12] = 0xBF;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.party_magicsword_offset);
		MACRO_MAGICSWORD_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_magicsword_field_id));
		for (i=0;i<MAGIC_SWORD_AMOUNT;i++) {
			SteamReadChar(dlldata.dll_file,magic_sword_requirement[i]);
			dlldata.dll_file.seekg(3,ios::cur);
		}
	}
}

DllMetaDataModification* PartySpecialDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[1];
	DllMetaData& dlldata = config.meta_dll;
	unsigned int i;
	res[0].position = dlldata.GetStaticFieldOffset(config.dll_magicsword_field_id);
	res[0].base_length = 4*MAGIC_SWORD_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_magicsword_field_id);
	res[0].new_length = 4*MAGIC_SWORD_AMOUNT;
	res[0].value = new uint8_t[res[0].new_length];
	BufferInitPosition();
	for (i=0;i<MAGIC_SWORD_AMOUNT;i++)
		BufferWriteLong(res[0].value,magic_sword_requirement[i]);
	*modifamount = 1;
	return res;
}

void PartySpecialDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream mgcswddb;
	mgcswddb << "// Method: BattleHUD::SetAbilityMagic\n\n";
	mgcswddb << "\t// ...\n";
	mgcswddb << "\tint[] array2 = new int[] {\n";
	for (i = 0; i < MAGIC_SWORD_AMOUNT; i++)
		mgcswddb << "\t\t" << (int)magic_sword_requirement[i] << (i+1==MAGIC_SWORD_AMOUNT ? "" : ",") << "\n";
	mgcswddb << "\t};\n";
	mgcswddb << "\t// ...\n";
	buffer.push_back(mgcswddb.str());
}

bool PartySpecialDataSet::GenerateCSV(string basefolder) {
	unsigned int i;
	string fname = basefolder + HADES_STRING_CSV_MGCSWORD_FILE;
	wfstream csv(fname.c_str(), ios::out);
	if (!csv.is_open()) return false;
	csv << HADES_STRING_CSV_MGCSWORD_HEADER;
	csv << L"\tprivate static void SetAbilityMagic(BattleHUD.AbilityPlayerDetail abilityPlayer) {\n\t\tif (abilityPlayer.Player.Index != CharacterIndex.Steiner) {\n\t\t\treturn;\n\t\t}\n\t\tCharacterCommand characterCommand = CharacterCommands.Commands[31];\n\t\tPLAYER player = FF9StateSystem.Common.FF9.player[(int)CharacterPresetId.Vivi];\n\t\tCharacterAbility[] array = ff9abil._FF9Abil_PaData[(int)CharacterPresetId.Vivi];\n";
	csv << L"\t\tint[] array2 = new int[] {\n";
	for (i = 0; i < MAGIC_SWORD_AMOUNT; i++)
		csv << L"\t\t\t" << (int)magic_sword_requirement[i] << (i+1==MAGIC_SWORD_AMOUNT ? L"" : L",") << L"\n";
	csv << L"\t\t};\n";
	csv << L"\t\tint num = Math.Min(characterCommand.Abilities.Length, array2.Length);\n\t\tfor (int i = 0; i < num; i++) {\n\t\t\tint key = (int)characterCommand.Abilities[i];\n\t\t\tint num2 = ff9abil.FF9Abil_GetIndex(1, array2[i]);\n\t\t\tif (num2 > -1) {\n\t\t\t\tabilityPlayer.AbilityPaList[key] = (int)player.pa[num2];\n\t\t\t\tabilityPlayer.AbilityMaxPaList[key] = (int)array[num2].Ap;\n\t\t\t}\n\t\t}\n\t\tfor (int j = 0; j < 5; j++) {\n\t\t\tint num3 = (int)player.equip[j];\n\t\t\tif (num3 != 255) {\n\t\t\t\tFF9ITEM_DATA ff9ITEM_DATA = ff9item._FF9Item_Data[num3];\n\t\t\t\tfor (int k = 0; k < 3; k++) {\n\t\t\t\t\tint num4 = (int)ff9ITEM_DATA.ability[k];\n\t\t\t\t\tif (num4 != 0 && num4 < 192) {\n\t\t\t\t\t\tfor (int l = 0; l < num; l++) {\n\t\t\t\t\t\t\tif (num4 == array2[l]) {\n\t\t\t\t\t\t\t\tint key2 = (int)characterCommand.Abilities[l];\n\t\t\t\t\t\t\t\tabilityPlayer.AbilityEquipList[key2] = true;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\t}\n";
	csv.close();
	return true;
}

void PartySpecialDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	ffbin.seekg(config.party_magicsword_offset);
	MACRO_MAGICSWORD_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
}

void PartySpecialDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	ffbin.seekg(config.party_magicsword_offset);
	MACRO_MAGICSWORD_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void PartySpecialDataSet::LoadHWS(fstream& ffbin) {
	uint16_t version;
	unsigned int i;
	HWSReadShort(ffbin,version);
	MACRO_MAGICSWORD_IOFUNCTION(HWSRead,HWSSeek,true,false)
}

void PartySpecialDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i;
	HWSWriteShort(ffbin,PARTY_SPECIAL_HWS_VERSION);
	MACRO_MAGICSWORD_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}
