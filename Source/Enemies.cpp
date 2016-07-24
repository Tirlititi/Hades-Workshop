#include "Enemies.h"

#include <algorithm>
#include "Gui_LoadingDialog.h"
#include "Steam_Strings.h"

#define HWS_BATTLE_SCENE_MOD_ID		0xFFF0

int EnemySpellDataStruct::SetName(wstring newvalue) {
	if (parent->parent->text[parent->id]->SetText(id+parent->stat_amount,newvalue))
		return 1;
	name.SetValue(newvalue);
	return 0;
}

int EnemySpellDataStruct::SetName(FF9String& newvalue) {
	if (parent->parent->text[parent->id]->SetText(id+parent->stat_amount,newvalue))
		return 1;
	name = newvalue;
	return 0;
}

int EnemyStatDataStruct::SetName(wstring newvalue) {
	if (parent->parent->text[parent->id]->SetText(id,newvalue))
		return 1;
	name.SetValue(newvalue);
	parent->parent->UpdateBattleName(parent->id);
	return 0;
}

int EnemyStatDataStruct::SetName(FF9String& newvalue) {
	if (parent->parent->text[parent->id]->SetText(id,newvalue))
		return 1;
	name = newvalue;
	parent->parent->UpdateBattleName(parent->id);
	return 0;
}

int EnemyDataStruct::AddSpell(uint16_t copyid) {
	unsigned int i,j;
	BattleDataStruct* bs = parent->battle_data[id];
	TextDataStruct* td = parent->text[id];
	uint16_t reqlenbattle = 1;
	uint16_t reqlenspell = 8;
	uint16_t reqlentext = 8;
	if (bs->sequence_amount%2==0)
		reqlenbattle += 4;
	for (i=0;i<bs->sequence_code_amount[copyid];i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(bs->sequence_code[copyid][i].code);
		reqlenbattle++;
		for (j=0;j<seq.arg_amount;j++)
			reqlenbattle += seq.arg_length[j];
	}
	while (reqlenbattle%4)
		reqlenbattle++;
	if (GetExtraSize()<reqlenbattle+reqlenspell+reqlentext)
		return 1;
	SetSize(size+reqlenspell);
	EnemySpellDataStruct* newspell = new EnemySpellDataStruct[spell_amount+1];
	for (i=0;i<spell_amount+1;i++) {
		if (i<spell_amount) {
			j = i;
			newspell[i].name = spell[j].name;;
		} else {
			j = copyid;
			newspell[i].name.CreateEmpty();
		}
		newspell[i].effect = spell[j].effect;
		newspell[i].power = spell[j].power;
		newspell[i].element = spell[j].element;
		newspell[i].accuracy = spell[j].accuracy;
		newspell[i].flag = spell[j].flag;
		newspell[i].status = spell[j].status;
		newspell[i].mp = spell[j].mp;
		newspell[i].model = spell[j].model;
		newspell[i].unknown1 = spell[j].unknown1;
		newspell[i].unknown2 = spell[j].unknown2;
		newspell[i].unknown3 = spell[j].unknown3;
		newspell[i].unknown4 = spell[j].unknown4;
		newspell[i].id = i;
		newspell[i].parent = this;
	}
	delete[] spell;
	spell = newspell;
	spell_amount++;
	unsigned int seqamount = bs->sequence_amount;
	unsigned int newseqindex = bs->sequence_amount-1;
	bool lastisdummy = false;
	uint16_t* newseqoff = bs->sequence_offset;
	uint8_t* newseqanim = bs->sequence_base_anim;
	unsigned int* newseqcodeamount = bs->sequence_code_amount;
	EnemySequenceCodeLine** newseqcode = bs->sequence_code;
	if (bs->sequence_amount>1)
		lastisdummy = bs->sequence_offset[bs->sequence_amount-1]==bs->sequence_offset[0];
	if (!lastisdummy) {
		seqamount += 2;
		newseqindex++;
		newseqoff = new uint16_t[seqamount];
		newseqanim = new uint8_t[seqamount];
		newseqcodeamount = new unsigned int[seqamount];
		newseqcode = new EnemySequenceCodeLine*[seqamount];
		memcpy(newseqoff,bs->sequence_offset,bs->sequence_amount*sizeof(uint16_t));
		memcpy(newseqanim,bs->sequence_base_anim,bs->sequence_amount*sizeof(uint8_t));
		memcpy(newseqcodeamount,bs->sequence_code_amount,bs->sequence_amount*sizeof(unsigned int));
		memcpy(newseqcode,bs->sequence_code,bs->sequence_amount*sizeof(EnemySequenceCodeLine*));
	}
	newseqoff[newseqindex] = 0;
	newseqanim[newseqindex] = newseqanim[copyid];
	newseqcodeamount[newseqindex] = newseqcodeamount[copyid];
	newseqcode[newseqindex] = new EnemySequenceCodeLine[newseqcodeamount[newseqindex]];
	for (i=0;i<newseqcodeamount[newseqindex];i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(newseqcode[copyid][i].code);
		newseqcode[newseqindex][i].parent = bs;
		newseqcode[newseqindex][i].code = newseqcode[copyid][i].code;
		newseqcode[newseqindex][i].arg = new uint32_t[seq.arg_amount];
		memcpy(newseqcode[newseqindex][i].arg,newseqcode[copyid][i].arg,seq.arg_amount*sizeof(uint32_t));
	}
	if (!lastisdummy) {
		newseqoff[seqamount-1] = newseqoff[0];
		delete[] bs->sequence_offset;
		delete[] bs->sequence_base_anim;
		delete[] bs->sequence_code_amount;
//		delete[] bs->sequence_code;
		bs->sequence_offset = newseqoff;
		bs->sequence_base_anim = newseqanim;
		bs->sequence_code_amount = newseqcodeamount;
		bs->sequence_code = newseqcode;
	}
	bs->sequence_amount = seqamount;
	bs->UpdateOffset();
	FF9String newstr;
	newstr.CreateEmpty();
	td->AddText(stat_amount+spell_amount-1,newstr);
	return 0;
}

void EnemyDataStruct::RemoveSpell(uint16_t spellid) {
	unsigned int i,j;
	BattleDataStruct* bs = parent->battle_data[id];
	TextDataStruct* td = parent->text[id];
	spell_amount--;
	EnemySpellDataStruct* newspell = new EnemySpellDataStruct[spell_amount];
	for (i=0;i<spell_amount;i++) {
		if (i<spellid)
			j = i;
		else
			j = i+1;
		newspell[i].name = spell[j].name;
		newspell[i].effect = spell[j].effect;
		newspell[i].power = spell[j].power;
		newspell[i].element = spell[j].element;
		newspell[i].accuracy = spell[j].accuracy;
		newspell[i].flag = spell[j].flag;
		newspell[i].status = spell[j].status;
		newspell[i].mp = spell[j].mp;
		newspell[i].model = spell[j].model;
		newspell[i].unknown1 = spell[j].unknown1;
		newspell[i].unknown2 = spell[j].unknown2;
		newspell[i].unknown3 = spell[j].unknown3;
		newspell[i].unknown4 = spell[j].unknown4;
		newspell[i].id = i;
		newspell[i].parent = this;
	}
	delete[] spell;
	spell = newspell;
	UpdateOffset();
	bool lastisdummy = false;
	if (bs->sequence_amount>1)
		lastisdummy = bs->sequence_offset[bs->sequence_amount-1]==bs->sequence_offset[0];
	if (lastisdummy) {
		bs->sequence_amount -= 2;
		uint16_t* newseqoff = new uint16_t[bs->sequence_amount];
		uint8_t* newseqanim = new uint8_t[bs->sequence_amount];
		unsigned int* newseqcodeamount = new unsigned int[bs->sequence_amount];
		EnemySequenceCodeLine** newseqcode = new EnemySequenceCodeLine*[bs->sequence_amount];
		memcpy(newseqoff,bs->sequence_offset,spellid*sizeof(uint16_t));
		memcpy(newseqanim,bs->sequence_base_anim,spellid*sizeof(uint8_t));
		memcpy(newseqcodeamount,bs->sequence_code_amount,spellid*sizeof(unsigned int));
		memcpy(newseqcode,bs->sequence_code,spellid*sizeof(EnemySequenceCodeLine*));
		memcpy(newseqoff+spellid,bs->sequence_offset+spellid+1,(bs->sequence_amount-spellid)*sizeof(uint16_t));
		memcpy(newseqanim+spellid,bs->sequence_base_anim+spellid+1,(bs->sequence_amount-spellid)*sizeof(uint8_t));
		memcpy(newseqcodeamount+spellid,bs->sequence_code_amount+spellid+1,(bs->sequence_amount-spellid)*sizeof(unsigned int));
		memcpy(newseqcode+spellid,bs->sequence_code+spellid+1,(bs->sequence_amount-spellid)*sizeof(EnemySequenceCodeLine*));
		delete[] bs->sequence_offset;
		delete[] bs->sequence_base_anim;
		delete[] bs->sequence_code_amount;
//		delete[] bs->sequence_code;
		bs->sequence_offset = newseqoff;
		bs->sequence_base_anim = newseqanim;
		bs->sequence_code_amount = newseqcodeamount;
		bs->sequence_code = newseqcode;
	} else if (bs->sequence_amount>1) {
		memmove(bs->sequence_offset+spellid,bs->sequence_offset+spellid+1,(bs->sequence_amount-spellid-1)*sizeof(uint16_t));
		memmove(bs->sequence_base_anim+spellid,bs->sequence_base_anim+spellid+1,(bs->sequence_amount-spellid-1)*sizeof(uint8_t));
		memmove(bs->sequence_code_amount+spellid,bs->sequence_code_amount+spellid+1,(bs->sequence_amount-spellid-1)*sizeof(unsigned int));
		memmove(bs->sequence_code+spellid,bs->sequence_code+spellid+1,(bs->sequence_amount-spellid-1)*sizeof(EnemySequenceCodeLine*));
		bs->sequence_offset[bs->sequence_amount-1] = bs->sequence_offset[0];
	}
	bs->UpdateOffset();
	td->RemoveText(stat_amount+spellid);
}

int EnemyDataStruct::AddGroup() {
	if (GetExtraSize()<0x38)
		return 1;
	unsigned int i;
	SetSize(size+0x38);
	EnemyGroupDataStruct* newgroup = new EnemyGroupDataStruct[group_amount+1];
	memcpy(newgroup,group,group_amount*sizeof(EnemyGroupDataStruct));
	newgroup[group_amount].frequence = 0;
	newgroup[group_amount].enemy_amount = 1;
	newgroup[group_amount].camera_engage = 0;
	newgroup[group_amount].ap = 0;
	for (i=0;i<4;i++) {
		newgroup[group_amount].enemy_id[i] = 0;
		newgroup[group_amount].targetable[i] = 1;
		newgroup[group_amount].enemy_posx[i] = 1500-1000*i;
		newgroup[group_amount].enemy_posz[i] = 0;
		newgroup[group_amount].enemy_posy[i] = 600;
		newgroup[group_amount].enemy_angle[i] = 0;
	}
	newgroup[group_amount].id = group_amount;
	newgroup[group_amount].parent = this;
	delete[] group;
	group = newgroup;
	group_amount++;
	return 0;
}

void EnemyDataStruct::RemoveGroup(uint16_t groupid) {
	SetSize(size-0x38);
	EnemyGroupDataStruct* newgroup = new EnemyGroupDataStruct[group_amount-1];
	memcpy(newgroup,group,groupid*sizeof(EnemyGroupDataStruct));
	memcpy(newgroup+groupid,group+groupid+1,(group_amount-groupid-1)*sizeof(EnemyGroupDataStruct));
	delete[] group;
	group = newgroup;
	group_amount--;
}

#define MACRO_ENEMY_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j; \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Char(f,unknown); \
	IO ## Char(f,group_amount); \
	IO ## Char(f,stat_amount); \
	IO ## Char(f,spell_amount); \
	IO ## Short(f,flag); \
	IO ## Short(f,zero16); \
	if (READ) { \
		group = new EnemyGroupDataStruct[group_amount]; \
		stat = new EnemyStatDataStruct[stat_amount]; \
		spell = new EnemySpellDataStruct[spell_amount]; \
	} \
	for (i=0;i<group_amount;i++) { \
		if (READ) group[i].parent = this; \
		if (READ) group[i].id = i; \
		IO ## Char(f,group[i].frequence); \
		IO ## Char(f,group[i].enemy_amount); \
		IO ## Short(f,group[i].camera_engage); \
		IO ## Short(f,group[i].ap); \
		IO ## Short(f,zero16); \
		for (j=0;j<4;j++) { \
			IO ## Char(f,group[i].enemy_id[j]); \
			IO ## Char(f,group[i].targetable[j]); \
			IO ## Short(f,zero16); \
			IO ## Short(f,(uint16_t&)group[i].enemy_posx[j]); \
			IO ## Short(f,(uint16_t&)group[i].enemy_posz[j]); \
			IO ## Short(f,(uint16_t&)group[i].enemy_posy[j]); \
			IO ## Short(f,(uint16_t&)group[i].enemy_angle[j]); \
		} \
	} \
	for (i=0;i<stat_amount;i++) { \
		if (READ) stat[i].parent = this; \
		if (READ) stat[i].id = i; \
		IO ## Long(f,stat[i].status_immune); \
		IO ## Long(f,stat[i].status_auto); \
		IO ## Long(f,stat[i].status_initial); \
		IO ## Short(f,stat[i].hp); \
		IO ## Short(f,stat[i].mp); \
		IO ## Short(f,stat[i].gils); \
		IO ## Short(f,stat[i].exp); \
		IO ## Char(f,stat[i].item_drop[0]); \
		IO ## Char(f,stat[i].item_drop[1]); \
		IO ## Char(f,stat[i].item_drop[2]); \
		IO ## Char(f,stat[i].item_drop[3]); \
		IO ## Char(f,stat[i].item_steal[0]); \
		IO ## Char(f,stat[i].item_steal[1]); \
		IO ## Char(f,stat[i].item_steal[2]); \
		IO ## Char(f,stat[i].item_steal[3]); \
		IO ## Short(f,stat[i].radius); \
		IO ## Short(f,stat[i].model); \
		IO ## Short(f,stat[i].anim_stand); \
		IO ## Short(f,stat[i].anim_death1); \
		IO ## Short(f,stat[i].anim_hit1); \
		IO ## Short(f,stat[i].anim_hit2); \
		IO ## Short(f,stat[i].anim_death2); \
		IO ## Short(f,stat[i].anim_death3); \
		IO ## Short(f,stat[i].mesh); \
		IO ## Short(f,stat[i].mesh_vanish); \
		IO ## Short(f,stat[i].death_flag); \
		IO ## Short(f,stat[i].attack); \
		IO ## Char(f,stat[i].speed); \
		IO ## Char(f,stat[i].strength); \
		IO ## Char(f,stat[i].magic); \
		IO ## Char(f,stat[i].spirit); \
		IO ## Char(f,stat[i].zero1); \
		IO ## Char(f,stat[i].trans); \
		IO ## Char(f,stat[i].cur_capa); \
		IO ## Char(f,stat[i].max_capa); \
		IO ## Char(f,stat[i].element_immune); \
		IO ## Char(f,stat[i].element_absorb); \
		IO ## Char(f,stat[i].element_half); \
		IO ## Char(f,stat[i].element_weak); \
		IO ## Char(f,stat[i].lvl); \
		IO ## Char(f,stat[i].classification); \
		IO ## Char(f,stat[i].accuracy); \
		IO ## Char(f,stat[i].defence); \
		IO ## Char(f,stat[i].evade); \
		IO ## Char(f,stat[i].magic_defence); \
		IO ## Char(f,stat[i].magic_evade); \
		IO ## Char(f,stat[i].blue_magic); \
		IO ## Char(f,stat[i].bone_camera1); \
		IO ## Char(f,stat[i].bone_camera2); \
		IO ## Char(f,stat[i].bone_camera3); \
		IO ## Char(f,stat[i].bone_target); \
		IO ## Short(f,stat[i].sound_death); \
		IO ## Char(f,stat[i].default_attack); \
		IO ## Char(f,stat[i].text_amount); \
		IO ## Char(f,stat[i].selection_bone[0]); \
		IO ## Char(f,stat[i].selection_bone[1]); \
		IO ## Char(f,stat[i].selection_bone[2]); \
		IO ## Char(f,stat[i].selection_bone[3]); \
		IO ## Char(f,stat[i].selection_bone[4]); \
		IO ## Char(f,stat[i].selection_bone[5]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[0]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[1]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[2]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[3]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[4]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsetz[5]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[0]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[1]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[2]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[3]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[4]); \
		IO ## Char(f,(uint8_t&)stat[i].selection_offsety[5]); \
		IO ## Short(f,stat[i].sound_engage); \
		IO ## Short(f,stat[i].shadow_x); \
		IO ## Short(f,stat[i].shadow_y); \
		IO ## Char(f,stat[i].shadow_bone1); \
		IO ## Char(f,stat[i].card_drop); \
		IO ## Short(f,(uint16_t&)stat[i].shadow_offset_x); \
		IO ## Short(f,(uint16_t&)stat[i].shadow_offset_y); \
		IO ## Char(f,stat[i].shadow_bone2); \
		IO ## Char(f,stat[i].zero2); \
		IO ## Long(f,stat[i].zero3); \
	} \
	for (i=0;i<spell_amount;i++) { \
		if (READ) spell[i].parent = this; \
		if (READ) spell[i].id = i; \
		IO ## Char(f,spell[i].unknown1); \
		if (READ) { \
			IO ## Short(f,spell[i].model); \
			spell[i].model &= 0x1FF; \
		} else { \
			uint16_t tmp = spell[i].model | ~0x1FF; \
			IO ## Short(f,tmp); \
		} \
		IO ## Char(f,spell[i].unknown2); \
		IO ## Char(f,spell[i].effect); \
		IO ## Char(f,spell[i].power); \
		IO ## Char(f,spell[i].element); \
		IO ## Char(f,spell[i].accuracy); \
		IO ## Char(f,spell[i].flag); \
		IO ## Char(f,spell[i].status); \
		IO ## Char(f,spell[i].mp); \
		IO ## Char(f,spell[i].unknown3); \
		IO ## Long(f,spell[i].unknown4); \
	} \
	if (PPF) PPFEndScanStep();

void EnemyDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_ENEMY_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		MACRO_ENEMY_IOFUNCTION(SteamRead,SteamSeek,true,false)
	}
	loaded = true;
}

void EnemyDataStruct::Write(fstream& f) {
	MACRO_ENEMY_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void EnemyDataStruct::WritePPF(fstream& f) {
	MACRO_ENEMY_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void EnemyDataStruct::ReadHWS(fstream& f) {
	MACRO_ENEMY_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void EnemyDataStruct::WriteHWS(fstream& f) {
	MACRO_ENEMY_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

void EnemyDataStruct::UpdateOffset() {
	SetSize(group_amount*0x38+stat_amount*0x74+spell_amount*0x10+8);
}

EnemyStatDataStruct* GetSimilarEnemyStatsResult[1024];
unsigned int GetSimilarEnemyStatsId[1024];
EnemyStatDataStruct** EnemyDataSet::GetSimilarEnemyStats(EnemyStatDataStruct& stat, unsigned int* amountfound, unsigned int** battleid) {
	unsigned int i,j,nb = 0;
	wstring& statname = stat.name.str_nice;
	for (i=0;i<battle_amount;i++)
		for (j=0;j<battle[i]->stat_amount;j++)
			if (battle[i]->stat[j].name.str_nice==statname && battle[i]->stat[j].lvl==stat.lvl) {
				GetSimilarEnemyStatsResult[nb] = &(battle[i]->stat[j]);
				GetSimilarEnemyStatsId[nb++] = i;
			}
	*amountfound = nb;
	*battleid = GetSimilarEnemyStatsId;
	return GetSimilarEnemyStatsResult;
}

EnemySequenceCodeLine::~EnemySequenceCodeLine() {
//	delete[] arg;
}

EnemySequenceCodeLine enmyseqcodetmp[0x400];
#define MACRO_BATTLE_IOFUNCTION_SEQREAD(IO) \
	j = 0; \
	do { \
		enmyseqcodetmp[j].parent = this; \
		IO ## Char(f,enmyseqcodetmp[j].code); \
		EnemySequenceCode& seq = GetEnemySequenceCode(enmyseqcodetmp[j].code); \
		enmyseqcodetmp[j].arg = new uint32_t[seq.arg_amount]; \
		for (k=0;k<seq.arg_amount;k++) { \
			if (seq.arg_length[k]==1) { \
				IO ## Char(f,tmp8); \
				enmyseqcodetmp[j].arg[k] = tmp8; \
			} else if (seq.arg_length[k]==2) { \
				IO ## Short(f,tmp16); \
				enmyseqcodetmp[j].arg[k] = tmp16; \
			} else if (seq.arg_length[k]==4) { \
				IO ## Long(f,tmp32); \
				enmyseqcodetmp[j].arg[k] = tmp32; \
			} \
		} \
	} while (enmyseqcodetmp[j++].code!=0); \
	sequence_code_amount[i] = j; \
	sequence_code[i] = new EnemySequenceCodeLine[sequence_code_amount[i]]; \
	memcpy(sequence_code[i],enmyseqcodetmp,sequence_code_amount[i]*sizeof(EnemySequenceCodeLine));

#define MACRO_BATTLE_IOFUNCTION_SEQWRITE(IO) \
	for (j=0;j<sequence_code_amount[i];j++) { \
		IO ## Char(f,sequence_code[i][j].code); \
		EnemySequenceCode& seq = GetEnemySequenceCode(sequence_code[i][j].code); \
		for (k=0;k<seq.arg_amount;k++) { \
			if (seq.arg_length[k]==1) { \
				tmp8 = sequence_code[i][j].arg[k]; \
				IO ## Char(f,tmp8); \
			} else if (seq.arg_length[k]==2) { \
				tmp16 = sequence_code[i][j].arg[k]; \
				IO ## Short(f,tmp16); \
			} else if (seq.arg_length[k]==4) { \
				tmp32 = sequence_code[i][j].arg[k]; \
				IO ## Long(f,tmp32); \
			} \
		} \
	}

#define MACRO_BATTLE_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j,k; \
	uint8_t zero8 = 0; \
	uint16_t zero16 = 0; \
	uint32_t headerpos = f.tellg(); \
	uint32_t animpos; \
	uint8_t tmp8; \
	uint16_t tmp16; \
	uint32_t tmp32; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,animblock_offset); \
	IO ## Short(f,camerablock_offset); \
	if (PPF) PPFEndScanStep(); \
	SEEK(f,headerpos,animblock_offset); \
	animpos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,sequence_amount); \
	IO ## Short(f,animation_amount); \
	if (READ) { \
		animation_id = new uint16_t[animation_amount]; \
		sequence_offset = new uint16_t[sequence_amount]; \
		sequence_base_anim = new uint8_t[sequence_amount]; \
		sequence_code_amount = new unsigned int[sequence_amount]; \
		sequence_code = new EnemySequenceCodeLine*[sequence_amount]; \
	} \
	for (i=0;i<sequence_amount;i++) \
		IO ## Short(f,sequence_offset[i]); \
	for (i=0;i<animation_amount;i++) { \
		IO ## Short(f,animation_id[i]); \
		IO ## Short(f,zero16); \
	} \
	for (i=0;i<sequence_amount;i++) \
		IO ## Char(f,sequence_base_anim[i]); \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<sequence_amount;i++) { \
		if (i+1!=sequence_amount || sequence_offset[i]!=sequence_offset[0]) { \
			SEEK(f,animpos,sequence_offset[i]); \
			if (PPF) PPFInitScanStep(f); \
			if (READ) { \
				MACRO_BATTLE_IOFUNCTION_SEQREAD(IO) \
			} else { \
				MACRO_BATTLE_IOFUNCTION_SEQWRITE(IO) \
			} \
			if (PPF) PPFEndScanStep(); \
		} else if (READ) { \
			sequence_code_amount[i] = 0; \
			sequence_code[i] = NULL; \
		} \
	} \
	SEEK(f,headerpos,camerablock_offset); \
	if (READ) { \
		camera_size = size-camerablock_offset; \
		camera_raw = new uint8_t[camera_size]; \
	} \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<camera_size;i++) \
		IO ## Char(f,camera_raw[i]); \
	if (PPF) PPFEndScanStep();
	

void BattleDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_BATTLE_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		MACRO_BATTLE_IOFUNCTION(SteamRead,SteamSeek,true,false)
	}
	loaded = true;
}

void BattleDataStruct::Write(fstream& f) {
	MACRO_BATTLE_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void BattleDataStruct::WritePPF(fstream& f) {
	MACRO_BATTLE_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void BattleDataStruct::ReadHWS(fstream& f) {
	MACRO_BATTLE_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void BattleDataStruct::WriteHWS(fstream& f) {
	MACRO_BATTLE_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

/*
bool BattleDataStruct::AddAnimationSequenceCode(unsigned int sequence, unsigned int codeline, uint8_t newcode) {
	EnemySequenceCode& newseq = GetEnemySequenceCode(newcode);
	uint16_t newlen = 1;
	unsigned int i;
	for (i=0;i<newseq.arg_amount;i++)
		newlen += newseq.arg_length[i];
	if (newlen+3>GetExtraSize()) // Secure 3 bytes for padding...
		return false;
	EnemySequenceCodeLine* buffer = new EnemySequenceCodeLine[sequence_code_amount[sequence]+1];
	memcpy(buffer,sequence_code[sequence],codeline*sizeof(EnemySequenceCodeLine));
	buffer[codeline].parent = this;
	buffer[codeline].code = newcode;
	for (i=0;i<newseq.arg_amount;i++) {
		if (newseq.arg_type[i]==EAAT_TIME)
			buffer[codeline].arg[i] = 1;
		else
			buffer[codeline].arg[i] = 0;
	}
	memcpy(buffer+codeline+1,sequence_code[sequence]+codeline,(sequence_code_amount[sequence]-codeline)*sizeof(EnemySequenceCodeLine));
	delete[] sequence_code[sequence];
	sequence_code[sequence] = buffer;
	sequence_code_amount[sequence]++;
	SetSize(size+newlen);
	return true;
}

void BattleDataStruct::DeleteAnimationSequenceCode(unsigned int sequence, unsigned int codeline) {
	EnemySequenceCode& oldseq = GetEnemySequenceCode(sequence_code[sequence][codeline].code);
	uint16_t oldlen = 1;
	unsigned int i;
	for (i=0;i<oldseq.arg_amount;i++)
		oldlen += oldseq.arg_length[i];
	EnemySequenceCodeLine* buffer = new EnemySequenceCodeLine[sequence_code_amount[sequence]-1];
	memcpy(buffer,sequence_code[sequence],codeline*sizeof(EnemySequenceCodeLine));
	memcpy(buffer+codeline,sequence_code[sequence]+codeline+1,(sequence_code_amount[sequence]-codeline-1)*sizeof(EnemySequenceCodeLine));
	delete[] sequence_code[sequence];
	sequence_code[sequence] = buffer;
	sequence_code_amount[sequence]--;
	SetSize(size-oldlen);
}

bool BattleDataStruct::ChangeAnimationSequenceCode(unsigned int sequence, unsigned int codeline, uint8_t newcode) {
	EnemySequenceCode& oldseq = GetEnemySequenceCode(sequence_code[sequence][codeline].code);
	EnemySequenceCode& newseq = GetEnemySequenceCode(newcode);
	uint16_t oldlen = 1;
	uint16_t newlen = 1;
	unsigned int i;
	for (i=0;i<oldseq.arg_amount;i++)
		oldlen += oldseq.arg_length[i];
	for (i=0;i<newseq.arg_amount;i++)
		newlen += newseq.arg_length[i];
	if (newlen+3>GetExtraSize()+oldlen) // Secure 3 bytes for padding...
		return false;
	sequence_code[sequence][codeline].code = newcode;
	delete[] sequence_code[sequence][codeline].arg;
	sequence_code[sequence][codeline].arg = new uint32_t[newseq.arg_amount];
	for (i=0;i<newseq.arg_amount;i++) {
		if (newseq.arg_type[i]==EAAT_TIME)
			sequence_code[sequence][codeline].arg[i] = 1;
		else
			sequence_code[sequence][codeline].arg[i] = 0;
	}
	SetSize(size+newlen-oldlen);
}
*/

void BattleDataStruct::UpdateOffset() {
	unsigned int i,j,k;
	uint32_t size;
	animblock_offset = 0x4;
	if (sequence_amount%2) {
		sequence_amount++;
		uint16_t* newoff = new uint16_t[sequence_amount];
		memcpy(newoff,sequence_offset,(sequence_amount-1)*sizeof(uint16_t));
		newoff[sequence_amount-1] = newoff[0];
		delete[] sequence_offset;
		sequence_offset = newoff;
	}
	bool lastisdummy = false;
	if (sequence_amount>1)
		lastisdummy = sequence_offset[sequence_amount-1]==sequence_offset[0];
	size = animation_amount*4+3*sequence_amount+4;
	for (i=0;i<sequence_amount;i++) {
		if (lastisdummy && i+1==sequence_amount) {
			sequence_offset[i] = sequence_offset[0];
		} else {
			sequence_offset[i] = size;
			for (j=0;j<sequence_code_amount[i];j++) {
				EnemySequenceCode& seq = GetEnemySequenceCode(sequence_code[i][j].code);
				for (k=0;k<seq.arg_amount;k++)
					size += seq.arg_length[k];
				size++;
			}
		}
	}
	if (size%4)
		size += 4-size%4;
	size += 4;
	camerablock_offset = size;
	size += camera_size;
	SetSize(size);
}

void EnemyDataSet::UpdateBattleName(unsigned int battleid) {
	unsigned int i;
	battle_name[battleid] = L"";
	for (i=0;i+1<battle[battleid]->stat_amount;i++)
		battle_name[battleid] += battle[battleid]->stat[i].name.str_nice + L" ; ";
	battle_name[battleid] += battle[battleid]->stat[battle[battleid]->stat_amount-1].name.str_nice;
}

void EnemyDataSet::Load(fstream& ffbin, ClusterSet& clusset) {
	unsigned int i,j,k,l;
	modified_battle_scene_amount = 0;
	image_map_amount = clusset.image_map_amount;
	battle_amount = clusset.enemy_amount;
	battle_name = new wstring[battle_amount];
	battle_data = new BattleDataStruct*[battle_amount];
	battle = new EnemyDataStruct*[battle_amount];
	text = new TextDataStruct*[battle_amount];
	script = new ScriptDataStruct*[battle_amount];
	j = 0;
	LoadingDialogInit(battle_amount,_(L"Reading enemy formations..."));
	if (GetGameType()==GAME_TYPE_PSX) {
		cluster_id = new uint16_t[battle_amount];
		shared_map = clusset.enemy_shared_map;
		image_map = &clusset.image_map;
		preload = new ImageMapDataStruct*[battle_amount];
		for (i=0;i<clusset.amount;i++) {
			if (clusset.clus_type[i]==CLUSTER_TYPE_ENEMY) {
				ClusterData& clus = clusset.clus[i];
				cluster_id[j] = i;
				clus.CreateChildren(ffbin);
				for (k=0;k<clus.chunk_amount;k++) {
					for (l=0;l<clus.chunk[k].object_amount;l++) {
						ffbin.seekg(clus.chunk[k].object_offset[l]);
						clus.chunk[k].GetObject(l).Read(ffbin);
					}
					if (clus.chunk_type[k]==CHUNK_TYPE_BATTLE_DATA)
						battle_data[j] = (BattleDataStruct*)&clus.chunk[k].GetObject(0);
					else if (clus.chunk_type[k]==CHUNK_TYPE_ENEMY_STATS)
						battle[j] = (EnemyDataStruct*)&clus.chunk[k].GetObject(0);
					else if (clus.chunk_type[k]==CHUNK_TYPE_TEXT)
						text[j] = (TextDataStruct*)&clus.chunk[k].GetObject(0);
					else if (clus.chunk_type[k]==CHUNK_TYPE_SCRIPT)
						script[j] = (ScriptDataStruct*)&clus.chunk[k].GetObject(0);
					else if (clus.chunk_type[k]==CHUNK_TYPE_IMAGE_MAP)
						preload[j] = (ImageMapDataStruct*)&clus.chunk[k].GetObject(0);
				}
				battle_data[j]->parent = this;
				battle_data[j]->id = j;
				battle[j]->parent = this;
				battle[j]->id = j;
				battle[j]->scene_id = 0;
				for (k=0;k<preload[j]->amount;k++)
					if (preload[j]->data_type[k]==CHUNK_TYPE_BATTLE_SCENE) {
						battle[j]->scene_id = preload[j]->data_id[k];
						break;
					}
				l = 0;
				for (k=0;k<battle[j]->stat_amount;k++)
					battle[j]->stat[k].name = text[j]->text[l++];
				for (k=0;k<battle[j]->spell_amount;k++)
					battle[j]->spell[k].name = text[j]->text[l++];
				UpdateBattleName(j);
				j++;
				LoadingDialogUpdate(j);
			}
		}
	} else {
		ClusterData** dummyclus = new ClusterData*[battle_amount];
		ConfigurationSet& config = *clusset.config;
		string fname = config.steam_dir_data;
		uint32_t fsize;
		char* buffer;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.enmy_text_file[GetSteamLanguage()][i]));
			SteamReadLong(ffbin,fsize);
			buffer = new char[fsize];
			ffbin.read(buffer,fsize);
			text[i] = new TextDataStruct[1];
			text[i]->Init(true,CHUNK_TYPE_TEXT,config.enmy_id[i],&dummyclus[i],CLUSTER_TYPE_ENEMY);
			text[i]->amount = FF9String::CountSteamTextAmount(buffer,fsize);
			text[i]->text = new FF9String[text[i]->amount];
			text[i]->loaded = true;
			delete[] buffer;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.enmy_text_file[GetSteamLanguage()][i]));
			SteamReadLong(ffbin,text[i]->size);
			for (j=0;j<text[i]->amount;j++)
				SteamReadFF9String(ffbin,text[i]->text[j]);
			LoadingDialogUpdate(i);
		}
		ffbin.close();
		fname = config.steam_dir_assets + "p0data2.bin";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			ffbin.seekg(config.meta_battle.GetFileOffsetByIndex(config.enmy_battle_file[i]));
			battle_data[i] = new BattleDataStruct[1];
			battle_data[i]->Init(false,CHUNK_TYPE_BATTLE_DATA,config.enmy_id[i],&dummyclus[i]);
			battle_data[i]->parent = this;
			battle_data[i]->id = i;
			SteamReadLong(ffbin,battle_data[i]->size);
			battle_data[i]->Read(ffbin);
			ffbin.seekg(config.meta_battle.GetFileOffsetByIndex(config.enmy_stat_file[i]));
			battle[i] = new EnemyDataStruct[1];
			battle[i]->Init(false,CHUNK_TYPE_ENEMY_STATS,config.enmy_id[i],&dummyclus[i]);
			battle[i]->parent = this;
			battle[i]->id = i;
			SteamReadLong(ffbin,battle[i]->size);
			battle[i]->Read(ffbin);
			LoadingDialogUpdate(i);
		}
		ffbin.close();
		fname = config.steam_dir_assets + "p0data7.bin";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.enmy_script_file[GetSteamLanguage()][i]));
			script[i] = new ScriptDataStruct[1];
			script[i]->Init(false,CHUNK_TYPE_SCRIPT,config.enmy_id[i],&dummyclus[i]);
			SteamReadLong(ffbin,script[i]->size);
			script[i]->Read(ffbin);
			l = 0;
			for (k=0;k<battle[i]->stat_amount && l<text[i]->amount;k++)
				battle[i]->stat[k].name = text[i]->text[l++];
			for (k=0;k<battle[i]->spell_amount && l<text[i]->amount;k++)
				battle[i]->spell[k].name = text[i]->text[l++];
			UpdateBattleName(i);
			LoadingDialogUpdate(i);
		}
		delete[] dummyclus;
		ffbin.close();
	}
	LoadingDialogEnd();
}

void EnemyDataSet::Write(fstream& ffbin, ClusterSet& clusset, bool saveworldmap, bool savefieldmap) {
	unsigned int i;
	for (i=0;i<battle_amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].Write(ffbin);
	}
	ffbin.seekg(clusset.clus[clusset.enemy_shared_map_index].offset);
	clusset.clus[clusset.enemy_shared_map_index].Write(ffbin);
	if (saveworldmap || savefieldmap)
		for (i=0;i<image_map_amount;i++)
			if ((image_map[0][i]->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && saveworldmap) || /*
			 */ (image_map[0][i]->parent_cluster->clus_type==CLUSTER_TYPE_FIELD && savefieldmap)) {
				ffbin.seekg(image_map[0][i]->parent_cluster->offset);
				image_map[0][i]->parent_cluster->Write(ffbin,false);
			}
}

void EnemyDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset, bool saveworldmap, bool savefieldmap) {
	unsigned int i;
	for (i=0;i<battle_amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].WritePPF(ffbin);
	}
	ffbin.seekg(clusset.clus[clusset.enemy_shared_map_index].offset);
	clusset.clus[clusset.enemy_shared_map_index].WritePPF(ffbin);
	if (saveworldmap || savefieldmap)
		for (i=0;i<image_map_amount;i++)
			if ((image_map[0][i]->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && saveworldmap) || /*
			 */ (image_map[0][i]->parent_cluster->clus_type==CLUSTER_TYPE_FIELD && savefieldmap)) {
				ffbin.seekg(image_map[0][i]->parent_cluster->offset);
				image_map[0][i]->parent_cluster->WritePPF(ffbin,false);
			}
}

int* EnemyDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag) {
	unsigned int i,j,k,l;
	uint32_t chunksize,clustersize,chunkpos,objectpos,objectsize;
	uint16_t nbmodified,objectid;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[4];
	res[0] = 0; res[1] = 0; res[2] = 0; res[3] = 0;
	bool loadmain = localflag & 1;
	bool loadlocal = localflag & 2;
	HWSReadShort(ffhws,nbmodified);
	for (i=0;i<nbmodified;i++) {
		objectpos = ffhws.tellg();
		HWSReadShort(ffhws,objectid);
		HWSReadLong(ffhws,clustersize);
		if (GetHWSGameType()==GAME_TYPE_PSX && GetGameType()!=GAME_TYPE_PSX) {
			for (j=0;j<G_N_ELEMENTS(SteamBattleScript);j++)
				if (SteamBattleScript[j].psx_id==objectid) {
					objectid = SteamBattleScript[j].battle_id;
					break;
				}
		} else if (GetHWSGameType()!=GAME_TYPE_PSX && GetGameType()==GAME_TYPE_PSX) {
			for (j=0;j<G_N_ELEMENTS(SteamBattleScript);j++)
				if (SteamBattleScript[j].battle_id==objectid) {
					objectid = SteamBattleScript[j].psx_id;
					break;
				}
		}
		if (objectid==HWS_BATTLE_SCENE_MOD_ID) {
			if (GetGameType()==GAME_TYPE_PSX) {
				clus = shared_map->parent_cluster;
				if (clustersize<=clus->size+clus->extra_size) { // There should never be a problem about that...
					HWSReadChar(ffhws,chunktype); // 0x14
					HWSReadLong(ffhws,chunksize);
					chunkpos = ffhws.tellg();
					unsigned int bsceneamount = chunksize/0x10;
					uint32_t soff, ssz;
					uint16_t bid, sid;
					uint16_t zero16;
					for (j=0;j<bsceneamount;j++) {
						HWSReadShort(ffhws,bid);
						HWSReadShort(ffhws,zero16); // Might be useful in the future...
						HWSReadShort(ffhws,sid);
						HWSReadShort(ffhws,zero16);
						HWSReadLong(ffhws,soff);
						HWSReadLong(ffhws,ssz);
						ChangeBattleScene(bid,sid,soff,ssz);
					}
					HWSReadChar(ffhws,chunktype); // 0xFF
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
			} else {
				HWSReadChar(ffhws,chunktype);
				while (chunktype!=0xFF) {
					HWSReadLong(ffhws,chunksize);
					ffhws.seekg(chunksize,ios::cur);
					HWSReadChar(ffhws,chunktype);
				}
			}
			continue;
		}
		for (j=0;j<battle_amount;j++) {
			if (objectid==battle[j]->object_id) {
				clus = battle[j]->parent_cluster;
				if (clustersize<=clus->size+clus->extra_size) {
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						chunkpos = ffhws.tellg();
						if (chunktype==CHUNK_TYPE_BATTLE_DATA) {
							if (loadmain) {
								battle_data[j]->SetSize(chunksize);
								battle_data[j]->ReadHWS(ffhws);
								battle_data[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_ENEMY_STATS) {
							if (loadmain) {
								battle[j]->ReadHWS(ffhws);
								battle[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_TEXT) {
							if (usetext && loadmain) {
								text[j]->ReadHWS(ffhws);
								text[j]->SetSize(chunksize - (GetHWSGameType()==GAME_TYPE_PSX ? 0 : 2));
							}
						} else if (chunktype==CHUNK_TYPE_SCRIPT) {
							if (loadmain) {
								script[j]->ReadHWS(ffhws);
								script[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_IMAGE_MAP) {
							if (loadmain) {
								if (GetHWSGameType()!=GetGameType()) {
									res[3]++;
								} else {
									preload[j]->ReadHWS(ffhws);
									preload[j]->SetSize(chunksize);
								}
							}
						} else if (chunktype==CHUNK_SPECIAL_TYPE_LOCAL) {
							if (loadlocal)
								script[j]->ReadLocalHWS(ffhws);
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
				if (GetGameType()==GAME_TYPE_PSX) {
					for (k=0;k<preload[j]->amount;k++)
						if (preload[j]->data_type[k]==CHUNK_TYPE_BATTLE_SCENE) {
							battle[j]->scene_id = preload[j]->data_id[k];
							break;
						}
				}
				l = 0;
				for (k=0;k<battle[j]->stat_amount;k++)
					battle[j]->stat[k].name = text[j]->text[l++];
				for (k=0;k<battle[j]->spell_amount;k++)
					battle[j]->spell[k].name = text[j]->text[l++];
				UpdateBattleName(j);
				j = battle_amount;
			} else if (j+1==battle_amount) {
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

void EnemyDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag) {
	unsigned int i,j;
	uint16_t nbmodified = 0;
	uint32_t chunkpos, nboffset = ffhws.tellg();
	ClusterData* clus;
	bool savemain = localflag & 1;
	bool savelocal = localflag & 2;
	HWSWriteShort(ffhws,nbmodified);
	if (modified_battle_scene_amount>0) {
		if (GetGameType()==GAME_TYPE_PSX) {
			clus = shared_map->parent_cluster;
			clus->UpdateOffset();
			HWSWriteShort(ffhws,HWS_BATTLE_SCENE_MOD_ID);
			HWSWriteLong(ffhws,clus->size);
			HWSWriteChar(ffhws,CHUNK_TYPE_IMAGE_MAP);
			HWSWriteLong(ffhws,modified_battle_scene_amount*0x10);
			chunkpos = ffhws.tellg();
			uint16_t zero16 = 0;
			for (i=0;i<modified_battle_scene_amount;i++) {
				HWSWriteShort(ffhws,modified_battle_id[i]);
				HWSWriteShort(ffhws,zero16);
				HWSWriteShort(ffhws,modified_scene_id[i]);
				HWSWriteShort(ffhws,zero16);
				HWSWriteLong(ffhws,modified_scene_offset[i]);
				HWSWriteLong(ffhws,modified_scene_size[i]);
			}
			ffhws.seekg(chunkpos+modified_battle_scene_amount*0x10);
			HWSWriteChar(ffhws,0xFF);
			nbmodified++;
		}
	}
	for (i=0;i<battle_amount;i++) {
		clus = battle[i]->parent_cluster;
		if (clus->modified) {
			clus->UpdateOffset();
			HWSWriteShort(ffhws,battle[i]->object_id);
			HWSWriteLong(ffhws,clus->size);
			if (battle_data[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_BATTLE_DATA);
				HWSWriteLong(ffhws,battle_data[i]->size);
				chunkpos = ffhws.tellg();
				battle_data[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+battle_data[i]->size);
			}
			if (battle[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_ENEMY_STATS);
				HWSWriteLong(ffhws,battle[i]->size);
				chunkpos = ffhws.tellg();
				battle[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+battle[i]->size);
			}
			if (text[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_TEXT);
				HWSWriteLong(ffhws,text[i]->size+2);
				chunkpos = ffhws.tellg();
				text[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+text[i]->size+2);
			}
			if (script[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_SCRIPT);
				HWSWriteLong(ffhws,script[i]->size);
				chunkpos = ffhws.tellg();
				script[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+script[i]->size);
			}
			if (GetGameType()==GAME_TYPE_PSX && preload[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_IMAGE_MAP);
				HWSWriteLong(ffhws,preload[i]->size);
				chunkpos = ffhws.tellg();
				preload[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+preload[i]->size);
			}
			if (savelocal) {
				uint32_t localsize = 0;
				HWSWriteChar(ffhws,CHUNK_SPECIAL_TYPE_LOCAL);
				HWSWriteLong(ffhws,localsize);
				chunkpos = ffhws.tellg();
				script[i]->WriteLocalHWS(ffhws);
				localsize = (long long)ffhws.tellg()-chunkpos;
				ffhws.seekg(chunkpos-4);
				HWSWriteLong(ffhws,localsize);
				ffhws.seekg(chunkpos+localsize);
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

int EnemyDataSet::ChangeBattleScene(uint16_t battleid, uint16_t newsceneid, uint32_t newsceneoffset, uint32_t newscenesize) {
	bool foundbattle, foundscene, replacescene, scenehere, newmod = true;
	uint16_t replacingsceneid, scenepos;
	unsigned int i,j;
	int res = 0;
	if (newsceneoffset==0) // find the offset and size by itself
		for (i=0;i<image_map_amount;i++)
			for (j=0;j<image_map[0][i]->amount;j++) {
				if (image_map[0][i]->data_type[j]==CHUNK_TYPE_BATTLE_SCENE && image_map[0][i]->data_id[j]==newsceneid) {
					newsceneoffset = image_map[0][i]->data_offset[j];
					newscenesize = image_map[0][i]->data_size[j];
					i = image_map_amount;
					break;
				}
			}
	for (i=0;i<image_map_amount;i++) {
		foundbattle = false;
		for (j=0;j<image_map[0][i]->amount;j++) {
			if (image_map[0][i]->data_type[j]==CHUNK_TYPE_BATTLE_DATA && image_map[0][i]->data_id[j]==battleid && image_map[0][i]->data_related_type[j]==CHUNK_TYPE_BATTLE_SCENE) {
				image_map[0][i]->data_related_id[j] = newsceneid;
				image_map[0][i]->MarkDataModified();
				foundbattle = true;
			}
		}
		if (foundbattle) {
			scenehere = false;
			foundscene = false;
			replacescene = false;
			for (j=0;j<image_map[0][i]->amount;j++) {
				if (image_map[0][i]->data_type[j]==CHUNK_TYPE_BATTLE_SCENE && image_map[0][i]->data_related_id[j]==battleid) {
					replacingsceneid = image_map[0][i]->data_id[j];
					scenepos = j;
					foundscene = true;
					scenehere = replacingsceneid==newsceneid;
					break;
				}
			}
			if (scenehere)
				continue;
			if (foundscene) {
				replacescene = true;
				for (j=0;j<image_map[0][i]->amount;j++) {
					if (image_map[0][i]->data_type[j]==CHUNK_TYPE_BATTLE_DATA && image_map[0][i]->data_related_id[j]==replacingsceneid) {
						replacescene = false;
						break;
					}
				}
			}
			if (replacescene)
				image_map[0][i]->RemoveDataByPos(scenepos);
			res += image_map[0][i]->AddDataSingle(newsceneid,CHUNK_TYPE_BATTLE_SCENE,0,newsceneoffset,newscenesize,battleid,CHUNK_TYPE_BATTLE_DATA,0);
			image_map[0][i]->UpdateOffset();
		}
	}
	for (i=0;i<modified_battle_scene_amount;i++)
		if (modified_battle_id[i]==battleid) {
			modified_scene_id[i] = newsceneid;
			modified_scene_offset[i] = newsceneoffset;
			modified_scene_size[i] = newscenesize;
			newmod = false;
			break;
		}
	if (newmod) {
		modified_battle_id[modified_battle_scene_amount] = battleid;
		modified_scene_id[modified_battle_scene_amount] = newsceneid;
		modified_scene_offset[modified_battle_scene_amount] = newsceneoffset;
		modified_scene_size[modified_battle_scene_amount] = newscenesize;
		modified_battle_scene_amount++;
	}
}
