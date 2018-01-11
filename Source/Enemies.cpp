#include "Enemies.h"

#include <algorithm>
#include "Gui_LoadingDialog.h"
#include "Database_Steam.h"
#include "Database_Resource.h"
#include "Database_SpellAnimation.h"

#define HWS_BATTLE_SCENE_MOD_ID		0xFFF0

void DummyPrintAnimDB(fstream& archive, EnemyDataSet* es, UnityArchiveMetaData* animmeta, UnityArchiveAssetBundle* bundle);

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

int EnemyDataStruct::AddStat(EnemyStatDataStruct* copystat) {
	BattleDataStruct& bd = *parent->battle_data[id];
	BattleDataStruct& copybd = *copystat->parent->parent->battle_data[copystat->parent->id];
	TextDataStruct& td = *parent->text[id];
	uint32_t sizereqbd = 2*copystat->sequence_anim_amount;
	uint32_t sizereqes = 0x74;
	unsigned int i;
	if (bd.GetExtraSize()<sizereqbd)
		return 1;
	bd.SetSize(bd.size+sizereqbd);
	if (GetExtraSize()<sizereqes) {
		bd.SetSize(bd.size-sizereqbd);
		return 1;
	}
	SetSize(size+sizereqes);
	if (td.AddText(stat_amount,copystat->name)) {
		bd.SetSize(bd.size-sizereqbd);
		SetSize(size-sizereqes);
		return 1;
	}
	EnemyStatDataStruct* newstat = new EnemyStatDataStruct[stat_amount+1];
	for (i=0;i<stat_amount;i++) { // DEBUG: memcpy on wstring bugs ; this is a solution (the usual one is to copy fields 1 by 1)
		newstat[i].name = stat[i].name;
		memcpy(&newstat[i]+sizeof(FF9String),&stat[i]+sizeof(FF9String),sizeof(EnemyStatDataStruct)-sizeof(FF9String));
	}
	newstat[stat_amount].name = copystat->name;
	memcpy(&newstat[stat_amount]+sizeof(FF9String),copystat+sizeof(FF9String),sizeof(EnemyStatDataStruct)-sizeof(FF9String));
	newstat[stat_amount].default_attack = 0;
	newstat[stat_amount].text_amount = 0;
	newstat[stat_amount].sequence_anim_base = bd.animation_amount;
	newstat[stat_amount].id = stat_amount;
	newstat[stat_amount].parent = this;
	stat_amount++;
	uint16_t* newseqanimid = new uint16_t[bd.animation_amount+copystat->sequence_anim_amount];
	memcpy(newseqanimid,bd.animation_id,bd.animation_amount*sizeof(uint16_t));
	memcpy(newseqanimid+bd.animation_amount,copybd.animation_id+copystat->sequence_anim_base,copystat->sequence_anim_amount*sizeof(uint16_t));
	bd.animation_amount += copystat->sequence_anim_amount;
	delete[] bd.animation_id;
	bd.animation_id = newseqanimid;
	delete[] stat;
	stat = newstat;
	parent->UpdateBattleName(id);
	return 0;
}

/*int EnemyDataStruct::AddStat(EnemyStatDataStruct* copystat) {
	BattleModelLinks& modelinfo = BattleModelLinks::GetLinksByModel(modelid);
	BattleDataStruct& bd = *parent->battle_data[id];
	TextDataStruct& td = *parent->text[id];
	uint32_t sizereqbd = 2*modelinfo.sequence_anim_amount;
	uint32_t sizereqes = 0x74;
	FF9String newname;
	unsigned int i;
	if (bd.GetExtraSize()<sizereqbd)
		return 1;
	bd.SetSize(bd.size+sizereqbd);
	if (GetExtraSize()<sizereqes) {
		bd.SetSize(bd.size-sizereqbd);
		return 1;
	}
	SetSize(size+sizereqes);
	newname.CreateEmpty();
	if (GetGameType()!=GAME_TYPE_PSX)
		newname.SetValue(L"[STRT=0,1]");
	if (td.AddText(stat_amount,newname)) {
		bd.SetSize(bd.size-sizereqbd);
		SetSize(size-sizereqes);
		return 1;
	}
	EnemyStatDataStruct* newstat = new EnemyStatDataStruct[stat_amount+1];
	for (i=0;i<stat_amount;i++) { // DEBUG: memcpy on wstring bugs ; this is a solution (the usual one is to copy fields 1 by 1)
		newstat[i].name = stat[i].name;
		memcpy(&newstat[i]+sizeof(FF9String),&stat[i]+sizeof(FF9String),sizeof(EnemyStatDataStruct)-sizeof(FF9String));
	}
	modelinfo.ApplyToEnemyStat(newstat[stat_amount]);
	newstat[stat_amount].name = newname;
	newstat[stat_amount].lvl = 1;
	newstat[stat_amount].classification = 0;
	newstat[stat_amount].hp = 1;
	newstat[stat_amount].mp = 0;
	newstat[stat_amount].gils = 0;
	newstat[stat_amount].exp = 0;
	for (i=0;i<4;i++) {
		newstat[stat_amount].item_drop[i] = 255;
		newstat[stat_amount].item_steal[i] = 255;
	}
	newstat[stat_amount].card_drop = 255;
	newstat[stat_amount].attack = 0;
	newstat[stat_amount].accuracy = 0;
	newstat[stat_amount].cur_capa = 0;
	newstat[stat_amount].max_capa = 0;
	newstat[stat_amount].trans = 0;
	newstat[stat_amount].speed = 1;
	newstat[stat_amount].strength = 1;
	newstat[stat_amount].magic = 1;
	newstat[stat_amount].spirit = 1;
	newstat[stat_amount].defence = 0;
	newstat[stat_amount].evade = 0;
	newstat[stat_amount].magic_defence = 0;
	newstat[stat_amount].magic_evade = 0;
	newstat[stat_amount].element_immune = 0;
	newstat[stat_amount].element_absorb = 0;
	newstat[stat_amount].element_half = 0;
	newstat[stat_amount].element_weak = 0;
	newstat[stat_amount].status_immune = 0;
	newstat[stat_amount].status_auto = 0;
	newstat[stat_amount].status_initial = 0;
	newstat[stat_amount].default_attack = 0;
	newstat[stat_amount].blue_magic = 0;
	newstat[stat_amount].death_flag = 0;
	newstat[stat_amount].text_amount = 0;
	newstat[stat_amount].zero1 = 0;
	newstat[stat_amount].zero2 = 0;
	newstat[stat_amount].zero3 = 0;
	newstat[stat_amount].sequence_anim_base = bd.animation_amount;
	newstat[stat_amount].sequence_anim_amount = modelinfo.sequence_anim_amount;
	newstat[stat_amount].id = stat_amount;
	newstat[stat_amount].parent = this;
	delete[] stat;
	stat = newstat;
	stat_amount++;
	uint16_t* newseqanimid = new uint16_t[bd.animation_amount+modelinfo.sequence_anim_amount];
	memcpy(newseqanimid,bd.animation_id,bd.animation_amount*sizeof(uint16_t));
	memcpy(newseqanimid+bd.animation_amount,modelinfo.sequence_anim,modelinfo.sequence_anim_amount*sizeof(uint16_t));
	delete[] bd.animation_id;
	bd.animation_amount += modelinfo.sequence_anim_amount;
	bd.animation_id = newseqanimid;
	parent->UpdateBattleName(id);
	return 0;
}*/

void EnemyDataStruct::RemoveStat(uint16_t statid) {
	BattleDataStruct& bd = *parent->battle_data[id];
	TextDataStruct& td = *parent->text[id];
	unsigned int i;
	bd.SetSize(bd.size-2*stat[statid].sequence_anim_amount);
	uint16_t* newseqanimid = new uint16_t[bd.animation_amount-stat[statid].sequence_anim_amount];
	memcpy(newseqanimid,bd.animation_id,stat[statid].sequence_anim_base*sizeof(uint16_t));
	memcpy(newseqanimid+stat[statid].sequence_anim_base,bd.animation_id+stat[statid].sequence_anim_base+stat[statid].sequence_anim_amount,(bd.animation_amount-stat[statid].sequence_anim_base-stat[statid].sequence_anim_amount)*sizeof(uint16_t));
	delete[] bd.animation_id;
	bd.animation_amount -= stat[statid].sequence_anim_amount;
	bd.animation_id = newseqanimid;
	for (i=0;i<bd.sequence_amount;i++)
		if (bd.sequence_base_anim[i]==stat[statid].sequence_anim_base)
			bd.sequence_base_anim[i] = 0;
		else if (bd.sequence_base_anim[i]>stat[statid].sequence_anim_base)
			bd.sequence_base_anim[i] -= stat[statid].sequence_anim_amount;
	for (i=0;i<stat_amount;i++)
		if (stat[i].sequence_anim_base>stat[statid].sequence_anim_base)
			stat[i].sequence_anim_base -= stat[statid].sequence_anim_amount;
	SetSize(size-0x74);
	EnemyStatDataStruct* newstat = new EnemyStatDataStruct[stat_amount-1];
	for (i=0;i<statid;i++) {
		newstat[i].name = stat[i].name;
		memcpy(&newstat[i]+sizeof(FF9String),&stat[i]+sizeof(FF9String),sizeof(EnemyStatDataStruct)-sizeof(FF9String));
	}
	for (i=statid;i+1<stat_amount;i++) {
		newstat[i].name = stat[i+1].name;
		memcpy(&newstat[i]+sizeof(FF9String),&stat[i+1]+sizeof(FF9String),sizeof(EnemyStatDataStruct)-sizeof(FF9String));
	}
	delete[] stat;
	stat = newstat;
	stat_amount--;
	td.RemoveText(statid);
	parent->UpdateBattleName(id);
}

int EnemyDataStruct::AddSpell(EnemySpellDataStruct* copyspell) {
	unsigned int i,j;
	BattleDataStruct& bs = *parent->battle_data[id];
	BattleDataStruct& copybs = *copyspell->parent->parent->battle_data[copyspell->parent->id];
	TextDataStruct& td = *parent->text[id];
	uint16_t reqlenbattle = 1;
	uint16_t reqlenspell = 8;
	if (bs.sequence_amount%2==0)
		reqlenbattle += 4;
	for (i=0;i<copybs.sequence_code_amount[copyspell->id];i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(copybs.sequence_code[copyspell->id][i].code);
		reqlenbattle++;
		for (j=0;j<seq.arg_amount;j++)
			reqlenbattle += seq.arg_length[j];
	}
	while (reqlenbattle%4)
		reqlenbattle++;
	if (GetExtraSize()<reqlenbattle+reqlenspell)
		return 1;
	if (td.AddText(stat_amount+spell_amount,copyspell->name))
		return 1;
	SetSize(size+reqlenspell);
	EnemySpellDataStruct* newspell = new EnemySpellDataStruct[spell_amount+1];
	for (i=0;i<spell_amount;i++) {
		newspell[i].name = spell[i].name;
		memcpy(&newspell[i]+sizeof(FF9String),&spell[i]+sizeof(FF9String),sizeof(EnemySpellDataStruct)-sizeof(FF9String));
	}
	newspell[spell_amount].name = copyspell->name;
	memcpy(&newspell[spell_amount]+sizeof(FF9String),copyspell+sizeof(FF9String),sizeof(EnemySpellDataStruct)-sizeof(FF9String));
	newspell[spell_amount].id = spell_amount;
	newspell[spell_amount].parent = this;
	delete[] spell;
	spell = newspell;
	spell_amount++;
	unsigned int seqamount = bs.sequence_amount;
	unsigned int newseqindex = bs.sequence_amount-1;
	bool lastisdummy = false;
	uint16_t* newseqoff = bs.sequence_offset;
	uint8_t* newseqanim = bs.sequence_base_anim;
	unsigned int* newseqcodeamount = bs.sequence_code_amount;
	EnemySequenceCodeLine** newseqcode = bs.sequence_code;
	if (bs.sequence_amount>1)
		lastisdummy = bs.sequence_offset[bs.sequence_amount-1]==bs.sequence_offset[0];
	if (!lastisdummy) {
		seqamount += 2;
		newseqindex++;
		newseqoff = new uint16_t[seqamount];
		newseqanim = new uint8_t[seqamount];
		newseqcodeamount = new unsigned int[seqamount];
		newseqcode = new EnemySequenceCodeLine*[seqamount];
		memcpy(newseqoff,bs.sequence_offset,bs.sequence_amount*sizeof(uint16_t));
		memcpy(newseqanim,bs.sequence_base_anim,bs.sequence_amount*sizeof(uint8_t));
		memcpy(newseqcodeamount,bs.sequence_code_amount,bs.sequence_amount*sizeof(unsigned int));
		memcpy(newseqcode,bs.sequence_code,bs.sequence_amount*sizeof(EnemySequenceCodeLine*));
	}
	newseqoff[newseqindex] = 0;
	newseqanim[newseqindex] = 0;
	newseqcodeamount[newseqindex] = copybs.sequence_code_amount[copyspell->id];
	newseqcode[newseqindex] = new EnemySequenceCodeLine[newseqcodeamount[newseqindex]];
	for (i=0;i<newseqcodeamount[newseqindex];i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(copybs.sequence_code[copyspell->id][i].code);
		newseqcode[newseqindex][i].parent = &bs;
		newseqcode[newseqindex][i].code = seq.id;
		newseqcode[newseqindex][i].arg = new uint32_t[seq.arg_amount];
		memcpy(newseqcode[newseqindex][i].arg,copybs.sequence_code[copyspell->id][i].arg,seq.arg_amount*sizeof(uint32_t));
	}
	if (!lastisdummy) {
		newseqoff[seqamount-1] = newseqoff[0];
		delete[] bs.sequence_offset;
		delete[] bs.sequence_base_anim;
		delete[] bs.sequence_code_amount;
//		delete[] bs.sequence_code;
		bs.sequence_offset = newseqoff;
		bs.sequence_base_anim = newseqanim;
		bs.sequence_code_amount = newseqcodeamount;
		bs.sequence_code = newseqcode;
	}
	bs.sequence_amount = seqamount;
	bs.UpdateOffset();
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
		newspell[i].target_type = spell[j].target_type;
		newspell[i].target_flag = spell[j].target_flag;
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
		IO ## Short(f,stat[i].anim_idle); \
		IO ## Short(f,stat[i].anim_idle_alt); \
		IO ## Short(f,stat[i].anim_hit); \
		IO ## Short(f,stat[i].anim_hit_alt); \
		IO ## Short(f,stat[i].anim_death); \
		IO ## Short(f,stat[i].anim_death_alt); \
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
		IO ## Char(f,spell[i].target_type); \
		if (READ) { \
			IO ## Short(f,spell[i].model); \
			spell[i].model &= 0x1FF; \
		} else { \
			uint16_t tmp = spell[i].model | ~0x1FF; \
			IO ## Short(f,tmp); \
		} \
		IO ## Char(f,spell[i].target_flag); \
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
				SetupEnemyInfo(j);
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
			fsize = config.meta_res.GetFileSizeByIndex(config.enmy_text_file[GetSteamLanguage()][i]);
			buffer = new char[fsize];
			ffbin.read(buffer,fsize);
			text[i] = new TextDataStruct[1];
			text[i]->Init(true,CHUNK_TYPE_TEXT,config.enmy_id[i],&dummyclus[i],CLUSTER_TYPE_ENEMY);
			text[i]->amount = FF9String::CountSteamTextAmount(buffer,fsize);
			text[i]->text = new FF9String[text[i]->amount];
			text[i]->loaded = true;
			delete[] buffer;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.enmy_text_file[GetSteamLanguage()][i]));
			text[i]->size = config.meta_res.GetFileSizeByIndex(config.enmy_text_file[GetSteamLanguage()][i]);
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
			battle_data[i]->size = config.meta_battle.GetFileSizeByIndex(config.enmy_battle_file[i]);
			battle_data[i]->Read(ffbin);
			ffbin.seekg(config.meta_battle.GetFileOffsetByIndex(config.enmy_stat_file[i]));
			battle[i] = new EnemyDataStruct[1];
			battle[i]->Init(false,CHUNK_TYPE_ENEMY_STATS,config.enmy_id[i],&dummyclus[i]);
			battle[i]->parent = this;
			battle[i]->id = i;
			battle[i]->size = config.meta_battle.GetFileSizeByIndex(config.enmy_stat_file[i]);
			battle[i]->Read(ffbin);
			battle[i]->scene_id = 0;
			LoadingDialogUpdate(i);
		}
		ffbin.close();
		fname = config.steam_dir_assets + "p0data7.bin";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.enmy_script_file[GetSteamLanguage()][i]));
			script[i] = new ScriptDataStruct[1];
			script[i]->Init(false,CHUNK_TYPE_SCRIPT,config.enmy_id[i],&dummyclus[i]);
			script[i]->size = config.meta_battle.GetFileSizeByIndex(config.enmy_script_file[GetSteamLanguage()][i]);
			script[i]->Read(ffbin);
			l = 0;
			for (k=0;k<battle[i]->stat_amount && l<text[i]->amount;k++)
				battle[i]->stat[k].name = text[i]->text[l++];
			for (k=0;k<battle[i]->spell_amount && l<text[i]->amount;k++)
				battle[i]->spell[k].name = text[i]->text[l++];
			UpdateBattleName(i);
			SetupEnemyInfo(i);
/*wfstream fout("aaaa.txt",ios::app|ios::out);
for (j=0;j<battle[i]->stat_amount;j++)
fout << battle[i]->stat[j].name.str_nice << L" : " << (unsigned int)battle[i]->stat[j].model << L" : " << (unsigned int)battle[i]->stat[j].sound_engage << L" : " << (unsigned int)battle[i]->stat[j].sound_death << endl;
fout.close();*/
			LoadingDialogUpdate(i);
		}
		delete[] dummyclus;
		ffbin.close();
		DllMetaData& dlldata = clusset.config->meta_dll;
		DllMethodInfo methinfo;
		ILInstruction ilinst;
		wstring tmpstr,curscenename = L"";
		uint16_t curbattleid = 0xFFFF;
		wstring battlenameid[G_N_ELEMENTS(SteamBattleScript)];
		for (i=0;i<G_N_ELEMENTS(SteamBattleScript);i++) {
			battlenameid[i] = L"";
			for (j=11;j<SteamBattleScript[i].name.length();j++)
				battlenameid[i] += SteamBattleScript[i].name[j];
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(clusset.config->dll_battledb_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[1] = {
			{ 0x73, dlldata.GetMemberTokenIdentifier("void System.Collections.Generic.Dictionary`2<string,string>::.ctor()") }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,1,initinst);
		steam_method_position = dlldata.dll_file.tellg();
		ilinst.Read(dlldata.dll_file);
		while (ilinst.opcode!=0x80) { // stsfld FF9BattleDB::SceneData
			if (ilinst.opcode==0x72) { // ldstr
				tmpstr = dlldata.GetStringTokenDescription(ilinst.param);
				if (tmpstr.substr(0,4).compare(L"BSC_")==0) {
					tmpstr = tmpstr.substr(4);
					for (i=0;i<G_N_ELEMENTS(SteamBattleScript);i++)
						if (tmpstr.compare(battlenameid[i])==0) {
							for (j=0;j<battle_amount;j++)
								if (SteamBattleScript[i].battle_id==battle[j]->object_id) {
									curbattleid = j;
									break;
								}
							break;
						}
				} else if (tmpstr.substr(0,4).compare(L"BBG_")==0) {
					curscenename = tmpstr.substr(4);
				}
			} else if (ilinst.opcode==0x6F) { // callvirt Dictionary::Add
				if (curbattleid!=0xFFFF && curscenename.length()>0) {
					for (i=0;i<G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME);i++)
						if (HADES_STRING_BATTLE_SCENE_NAME[i].steamid.compare(curscenename)==0) {
							battle[curbattleid]->scene_id = HADES_STRING_BATTLE_SCENE_NAME[i].id;
							break;
						}
				}
				curbattleid = 0xFFFF;
				curscenename = L"";
			}
			ilinst.Read(dlldata.dll_file);
		}
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg()-steam_method_position;

		/* DEBUG
		UnityArchiveMetaData animmeta;
		UnityArchiveAssetBundle animbundle;
		fname = config.steam_dir_assets + "p0data5.bin";
		fstream unityarchive((const char*)fname.c_str(),ios::in | ios::binary);
		animmeta.Load(unityarchive);
		unityarchive.seekg(animmeta.GetFileOffset("",142));
		animbundle.Load(unityarchive);
		DummyPrintAnimDB(unityarchive,this,&animmeta,&animbundle);
		*/

	}
	LoadingDialogEnd();
}

DllMetaDataModification* EnemyDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	if (modified_battle_scene_amount==0) {
		*modifamount = 0;
		return new DllMetaDataModification[0];
	}
	DllMetaDataModification* res = new DllMetaDataModification[1];
	DllMetaData& dlldata = config.meta_dll;
	ILInstruction ilinst;
	wstring tmpstr;
	unsigned int i,j;
	uint16_t curbattleid = 0xFFFF;
	wstring battlenameid[G_N_ELEMENTS(SteamBattleScript)];
	for (i=0;i<G_N_ELEMENTS(SteamBattleScript);i++) {
		battlenameid[i] = L"";
		for (j=11;j<SteamBattleScript[i].name.length();j++)
			battlenameid[i] += SteamBattleScript[i].name[j];
	}
	res[0].position = steam_method_position;
	res[0].base_length = steam_method_base_length;
	res[0].new_length = steam_method_base_length;
	res[0].value = new uint8_t[res[0].new_length];
	dlldata.dll_file.seekg(steam_method_position);
	BufferInitPosition();
	while (dlldata.dll_file.tellg()<steam_method_position+steam_method_base_length) {
		ilinst.Read(dlldata.dll_file);
		if (ilinst.opcode==0x72) { // ldstr
			tmpstr = dlldata.GetStringTokenDescription(ilinst.param);
			if (tmpstr.substr(0,4).compare(L"BSC_")==0) {
				tmpstr = tmpstr.substr(4);
				for (i=0;i<G_N_ELEMENTS(SteamBattleScript);i++)
					if (tmpstr.compare(battlenameid[i])==0) {
						curbattleid = SteamBattleScript[i].battle_id;
						break;
					}
			} else if (tmpstr.substr(0,4).compare(L"BBG_")==0 && curbattleid!=0xFFFF) {
				for (i=0;i<modified_battle_scene_amount;i++)
					if (modified_battle_id[i]==curbattleid) {
						tmpstr = L"BBG_";
						for (j=0;j<G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME);j++)
							if (HADES_STRING_BATTLE_SCENE_NAME[j].id==modified_scene_id[i]) {
								tmpstr += HADES_STRING_BATTLE_SCENE_NAME[j].steamid;
								break;
							}
						ilinst.param = dlldata.GetStringToken(tmpstr);
						break;
					}
			}
		} else if (ilinst.opcode==0x6F) { // callvirt Dictionary::Add
			curbattleid = 0xFFFF;
		}
		ilinst.AppendToBuffer(res[0].value);
	}
	*modifamount = 1;
	return res;
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
			bool load = true;
			if (GetGameType()==GAME_TYPE_PSX) {
				clus = shared_map->parent_cluster;
				if (clustersize>clus->size+clus->extra_size) { // There should never be a problem about that...
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
					load = false;
				}
			}
			if (load) {
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
					if (loadmain)
						SetupEnemyInfo(j);
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
	ClusterData* clus = NULL;
	bool savemain = localflag & 1;
	bool savelocal = localflag & 2;
	HWSWriteShort(ffhws,nbmodified);
	if (modified_battle_scene_amount>0) {
		if (GetGameType()==GAME_TYPE_PSX) {
			clus = shared_map->parent_cluster;
			clus->UpdateOffset();
		}
		HWSWriteShort(ffhws,HWS_BATTLE_SCENE_MOD_ID);
		HWSWriteLong(ffhws,GetGameType()==GAME_TYPE_PSX ? clus->size : 0);
		HWSWriteChar(ffhws,CHUNK_TYPE_IMAGE_MAP);
		HWSWriteLong(ffhws,modified_battle_scene_amount*0x10);
		chunkpos = ffhws.tellg();
		uint16_t zero16 = 0;
		for (i=0;i<modified_battle_scene_amount;i++) {
			HWSWriteShort(ffhws,modified_battle_id[i]);
			HWSWriteShort(ffhws,zero16);
			HWSWriteShort(ffhws,modified_scene_id[i]);
			HWSWriteShort(ffhws,zero16);
			HWSWriteLong(ffhws,GetGameType()==GAME_TYPE_PSX ? modified_scene_offset[i] : 0);
			HWSWriteLong(ffhws,GetGameType()==GAME_TYPE_PSX ? modified_scene_size[i] : 0);
		}
		ffhws.seekg(chunkpos+modified_battle_scene_amount*0x10);
		HWSWriteChar(ffhws,0xFF);
		nbmodified++;
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
	bool newmod = true;
	unsigned int i,j;
	for (i=0;i<battle_amount;i++)
		if (battle_data[i]->object_id==battleid) {
			battle[i]->scene_id = newsceneid;
			break;
		}
	if (GetGameType()==GAME_TYPE_PSX) {
		bool foundbattle, foundscene, replacescene, scenehere;
		uint16_t replacingsceneid, scenepos;
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
	return 0;
}

int EnemyDataSet::ChangeBattleModel(uint16_t battleid, uint8_t enemyid, BattleModelLinks& newmodelinfo) {
	EnemyStatDataStruct& es = battle[battleid]->stat[enemyid];
	BattleDataStruct& bd = *battle_data[battleid];
	unsigned int i;
	int32_t sizereq = 2*(newmodelinfo.sequence_anim_amount-es.sequence_anim_amount);
	if ((int32_t)bd.GetExtraSize()<sizereq)
		return 1;
	bd.SetSize(bd.size+sizereq);
	newmodelinfo.ApplyToEnemyStat(es);
	uint16_t* newseqanimid = new uint16_t[bd.animation_amount+newmodelinfo.sequence_anim_amount-es.sequence_anim_amount];
	int animgap;
	memcpy(newseqanimid,bd.animation_id,es.sequence_anim_base*sizeof(uint16_t));
	memcpy(newseqanimid+es.sequence_anim_base,newmodelinfo.sequence_anim,newmodelinfo.sequence_anim_amount*sizeof(uint16_t));
	memcpy(newseqanimid+es.sequence_anim_base+newmodelinfo.sequence_anim_amount,bd.animation_id+es.sequence_anim_base+es.sequence_anim_amount,(bd.animation_amount-es.sequence_anim_base-es.sequence_anim_amount)*sizeof(uint16_t));
	delete[] bd.animation_id;
	animgap = (int)newmodelinfo.sequence_anim_amount-es.sequence_anim_amount;
	bd.animation_amount += animgap;
	bd.animation_id = newseqanimid;
	es.sequence_anim_amount = newmodelinfo.sequence_anim_amount;
	for (i=0;i<bd.sequence_amount;i++)
		if (bd.sequence_base_anim[i]>es.sequence_anim_base)
			bd.sequence_base_anim[i] += animgap;
	for (i=0;i<battle[battleid]->stat_amount;i++)
		if (battle[battleid]->stat[i].sequence_anim_base>es.sequence_anim_base)
			battle[battleid]->stat[i].sequence_anim_base += animgap;
	return 0;
}

void EnemyDataSet::SetupEnemyInfo(uint16_t battleid) { // DEBUG: Assume at least one sequence (spell) is configured for each stat
	EnemyDataStruct& ed = *battle[battleid];
	BattleDataStruct& bd = *battle_data[battleid];
	unsigned int i, j = 0, firstanim = 0,lastisdummy = 0;
	if (bd.sequence_amount>1)
		lastisdummy = (bd.sequence_offset[bd.sequence_amount-1]==bd.sequence_offset[0] ? 1 : 0);
	for (i=0;i<ed.stat_amount;i++) {
		ed.stat[i].sequence_anim_base = firstanim;
		while (j+lastisdummy<bd.sequence_amount && bd.sequence_base_anim[j]==firstanim)
			j++;
		if (j+lastisdummy<bd.sequence_amount) {
			ed.stat[i].sequence_anim_amount = bd.sequence_base_anim[j]-firstanim;
			firstanim = bd.sequence_base_anim[j];
		} else {
			ed.stat[i].sequence_anim_amount = bd.animation_amount-firstanim;
			firstanim = bd.animation_amount;
		}
	}
}

// Model Links
static BattleModelLinks BATTLE_MODEL_LINKS[] = {
	{ 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 65535, 65535, 0, NULL },
	{ 0, 72, 327, 60, 339, 72, 327, 448, 3, 0, 58, 0, 14, 0, { 34, 34, 34, 34, 34, 34 }, { -31, -2, -14, 1, -14, -30 }, { -15, -17, -36, -14, -27, 0 }, 1330, 768, 56, 56, 0, 0, 1285, 1309, 21, new uint16_t[21]{ 74, 68, 318, 36, 70, 322, 39, 63, 309, 32, 46, 335, 66, 34, 48, 337, 342, 330, 311, 331, 343 } },
	{ 1, 418, 418, 410, 410, 418, 418, 1056, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 1 }, { -52, 0, -30, -16, -30, -78 }, { -67, -89, -86, -79, -75, 0 }, 1302, 1308, 1, 2, 0, 0, 65535, 65535, 9, new uint16_t[9]{ 412, 425, 347, 416, 435, 408, 414, 433, 349 } },
	{ 5, 6162, 6157, 6159, 6157, 6162, 6157, 686, 0, 0, 11, 0, 20, 0, { 20, 20, 20, 20, 20, 1 }, { -10, 0, -6, -1, -6, -20 }, { -1, -2, -10, -9, -6, 0 }, 182, 180, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 6136, 6157, 6164, 6166, 6168 } },
	{ 7, 11962, 11962, 11962, 11962, 11962, 11962, 770, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0 }, { -10, 0, 0, 0, 0, -8 }, { 0, 0, -10, -8, -8, 0 }, 243, 243, 1, 1, 0, 0, 65535, 65535, 0, NULL },
	{ 9, 1348, 1348, 1345, 2834, 1343, 1343, 742, 0, 0, 27, 0, 7, 0, { 5, 5, 5, 5, 5, 1 }, { -8, 0, -3, -1, -3, -13 }, { 0, 0, -12, -8, -5, 0 }, 196, 192, 3, 1, 0, 0, 399, 398, 8, new uint16_t[8]{ 1349, 1351, 1344, 1346, 1352, 1347, 1350, 1342 } },
	{ 14, 11961, 11961, 11961, 11961, 11961, 11961, 238, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0 }, { -10, 0, -5, -2, -5, -38 }, { 0, 0, -10, -8, -4, 0 }, 252, 252, 1, 1, 0, 0, 65535, 65535, 12, new uint16_t[12]{ 1658, 1664, 1644, 1650, 1666, 1652, 1660, 1616, 1654, 1662, 1618, 1646 } },
	{ 28, 1373, 1373, 1370, 2848, 1367, 1367, 688, 0, 0, 28, 0, 12, 0, { 2, 2, 2, 2, 2, 1 }, { -7, 0, -3, 0, -3, -8 }, { 0, 0, -10, -7, -3, 0 }, 196, 192, 1, 1, 0, 0, 115, 123, 9, new uint16_t[9]{ 1374, 1376, 1368, 1371, 1378, 1372, 1375, 1366, 1373 } },
	{ 36, 7956, 7950, 7944, 7944, 12944, 12944, 752, 0, 0, 5, 0, 44, 0, { 45, 45, 45, 45, 45, 39 }, { -8, 0, -3, 1, -3, -13 }, { -3, -1, -9, -8, -5, 0 }, 560, 288, 24, 50, 128, 76, 1278, 1302, 20, new uint16_t[20]{ 7958, 7966, 7938, 7946, 7972, 7952, 7962, 7932, 7948, 7959, 7929, 7941, 7969, 7954, 7964, 7934, 7967, 7950, 7936, 12944 } },
	{ 38, 13872, 13872, 13860, 13860, 13886, 13886, 656, 0, 6, 7, 24, 50, 24, { 49, 49, 48, 49, 49, 30 }, { -10, 0, -5, 3, -5, -10 }, { 1, -6, -15, -7, -3, 0 }, 224, 552, 6, 11, 0, 191, 1281, 1305, 18, new uint16_t[18]{ 13874, 13883, 13854, 13862, 13891, 13868, 13878, 13848, 13866, 13876, 13846, 13870, 13880, 13850, 13859, 13889, 13893, 13885 } },
	{ 46, 5596, 5596, 5588, 5610, 5580, 5580, 688, 0, 14, 33, 0, 16, 0, { 8, 8, 8, 8, 8, 1 }, { -13, 0, -7, -2, -7, -18 }, { 0, -2, -10, -9, -4, 0 }, 154, 156, 1, 1, 0, 0, 23, 132, 15, new uint16_t[15]{ 5598, 5607, 5582, 5590, 5615, 5592, 5602, 5576, 5594, 5604, 5578, 5587, 5613, 5617, 5609 } },
	{ 47, 6223, 6231, 6215, 6239, 6209, 6201, 840, 0, 0, 17, 12, 17, 12, { 0, 0, 0, 0, 0, 1 }, { -20, -10, -20, -11, -20, -25 }, { 0, -1, -28, -26, -16, 0 }, 378, 384, 1, 1, 0, 0, 65535, 65535, 15, new uint16_t[15]{ 6233, 6211, 6217, 6241, 14599, 6221, 6229, 6207, 6219, 6227, 6205, 6213, 6237, 6243, 6235 } },
	{ 47, 6223, 6231, 6215, 6239, 6209, 6201, 840, 0, 0, 17, 30, 17, 30, { 0, 0, 0, 0, 0, 1 }, { -20, -10, -20, -11, -20, -25 }, { 0, -1, -28, -26, -16, 0 }, 378, 384, 1, 1, 0, 0, 65535, 65535, 0, NULL },
	{ 57, 1405, 1405, 1401, 2868, 1399, 1399, 832, 0, 0, 28, 0, 10, 0, { 7, 7, 7, 7, 7, 1 }, { -6, -4, -4, 0, -4, -10 }, { -1, -5, -8, -6, -4, 0 }, 112, 108, 0, 2, 0, 24, 65535, 65535, 6, new uint16_t[6]{ 1404, 1408, 1398, 1403, 1407, 1397 } },
	{ 58, 1439, 1439, 1435, 2830, 1432, 1432, 756, 0, 0, 17, 0, 49, 0, { 9, 9, 9, 9, 9, 1 }, { -11, 0, -10, -7, -10, -20 }, { -4, -14, -15, -9, -9, 0 }, 490, 492, 1, 5, 0, 0, 129, 127, 10, new uint16_t[10]{ 1438, 1442, 1431, 1434, 1444, 1436, 1440, 1429, 1433, 1443 } },
	{ 59, 1470, 1470, 1466, 2842, 1463, 1463, 768, 0, 0, 10, 0, 18, 0, { 12, 12, 12, 12, 12, 1 }, { -10, 0, -5, 0, -5, -13 }, { 0, -2, -12, -9, -6, 0 }, 196, 192, 1, 1, 0, 0, 128, 126, 13, new uint16_t[13]{ 1471, 1474, 1464, 1467, 1476, 1468, 1472, 1461, 1469, 1473, 1462, 1465, 1475 } },
	{ 60, 1494, 1494, 1491, 2856, 1489, 1489, 848, 0, 0, 21, 8, 12, 8, { 11, 11, 11, 11, 11, 11 }, { -7, 1, -3, 0, -3, -10 }, { -2, -2, -11, -8, -6, 0 }, 140, 264, 0, 2, 0, 0, 136, 135, 8, new uint16_t[8]{ 1495, 1497, 1490, 1492, 1498, 1493, 1496, 1488 } },
	{ 77, 4413, 4413, 4407, 4421, 4401, 4401, 928, 0, 0, 18, 0, 54, 0, { 0, 0, 0, 0, 0, 1 }, { -30, -23, -26, -16, -26, -34 }, { -1, -3, -27, -17, -21, 0 }, 434, 432, 4, 2, 0, 0, 771, 836, 8, new uint16_t[8]{ 4412, 4420, 4400, 4406, 4424, 4410, 4418, 4398 } },
	{ 78, 4813, 4813, 4803, 4826, 4795, 4795, 924, 0, 0, 24, 0, 19, 0, { 21, 21, 21, 21, 21, 1 }, { -26, -5, -15, -6, -21, -29 }, { 0, -41, -51, -52, -14, 0 }, 1960, 504, 15, 23, 272, 149, 113, 122, 19, new uint16_t[19]{ 4815, 4825, 4797, 4805, 4833, 4813, 4809, 4821, 4791, 4811, 4823, 4793, 4801, 4831, 4806, 4818, 4788, 4798, 4828 } },
	{ 79, 4377, 4377, 4369, 4387, 4363, 4363, 928, 0, 0, 65, 0, 8, 0, { 0, 0, 0, 0, 0, 1 }, { -13, 0, -8, -6, -8, -15 }, { 4, -10, -12, -8, -4, 0 }, 322, 852, 0, 15, 0, 96, 770, 835, 14, new uint16_t[14]{ 4371, 4381, 4357, 4365, 4389, 4393, 4374, 4384, 4360, 4376, 4386, 4362, 4368, 4392 } },
	{ 80, 4444, 4444, 4437, 4455, 4430, 4430, 896, 0, 0, 54, 0, 4, 0, { 2, 2, 2, 2, 2, 1 }, { -10, -8, -7, -1, -7, -31 }, { 0, -1, -17, -12, -8, 0 }, 462, 468, 1, 49, 0, 0, 775, 840, 11, new uint16_t[11]{ 4446, 4454, 4432, 4440, 4450, 4426, 4442, 4452, 4428, 4436, 4458 } },
	{ 81, 4855, 4855, 4847, 4866, 4841, 4841, 1056, 0, 0, 35, 3, 20, 3, { 3, 3, 3, 3, 3, 1 }, { -12, -2, -8, -2, -8, -13 }, { -5, -10, -22, -19, -12, 0 }, 280, 504, 1, 2, 0, 27, 774, 839, 13, new uint16_t[13]{ 4857, 4865, 4843, 4849, 4871, 4851, 4861, 4837, 4853, 4863, 4839, 4845, 4869 } },
	{ 82, 4888, 4888, 4882, 4896, 4880, 4880, 686, 0, 0, 1, 2, 6, 2, { 2, 2, 2, 2, 2, 1 }, { -10, -4, -7, -3, -7, -17 }, { -1, -1, -13, -11, -5, 0 }, 252, 192, 0, 0, 0, 0, 772, 837, 6, new uint16_t[6]{ 4887, 4895, 4879, 4885, 4893, 4877 } },
	{ 83, 5216, 5208, 5227, 5169, 5151, 5161, 752, 0, 0, 19, 0, 9, 0, { 9, 9, 9, 9, 9, 1 }, { -10, 0, -6, -2, -6, -23 }, { 0, 0, -14, -9, -5, 0 }, 336, 336, 0, 1, 10, -20, 1263, 1287, 12, new uint16_t[12]{ 5210, 5218, 5163, 5171, 5229, 5173, 5212, 5157, 5206, 5214, 5159, 5219 } },
	{ 84, 4087, 4087, 4079, 4099, 4073, 4073, 1056, 0, 0, 11, 3, 21, 3, { 3, 3, 3, 3, 3, 1 }, { -17, -3, -13, 0, -13, -16 }, { 3, -8, -10, -7, -2, 0 }, 280, 348, 0, 3, 0, 0, 1262, 1286, 13, new uint16_t[13]{ 4089, 4098, 4075, 4081, 4104, 4083, 4093, 4069, 4085, 4095, 4071, 4078, 4102 } },
	{ 85, 6181, 6181, 6173, 6193, 6165, 6165, 882, 0, 3, 3, 5, 26, 5, { 15, 15, 15, 15, 15, 1 }, { -17, -4, -10, -5, -10, -50 }, { -14, -5, -17, -11, -11, 0 }, 238, 504, 3, 44, 0, 165, 757, 822, 13, new uint16_t[13]{ 6183, 6192, 6167, 6175, 6198, 6177, 6187, 6161, 6179, 6189, 6163, 6172, 6196 } },
	{ 86, 4019, 4019, 4013, 4029, 4009, 4009, 840, 0, 0, 22, 0, 5, 0, { 3, 3, 3, 3, 3, 1 }, { -9, -2, -4, 0, -4, -13 }, { 0, 0, -14, -10, -5, 0 }, 224, 396, 0, 2, 0, -12, 134, 133, 9, new uint16_t[9]{ 4021, 4028, 4011, 4015, 4032, 4017, 4025, 4007, 4019 } },
	{ 87, 4265, 4265, 4255, 4281, 4245, 4245, 800, 0, 0, 29, 3, 14, 3, { 11, 11, 11, 11, 11, 11 }, { -14, 0, -9, 6, -9, -20 }, { 0, -17, -22, -12, -12, 0 }, 448, 456, 1, 2, 0, 0, 761, 826, 20, new uint16_t[20]{ 4267, 4278, 4247, 4257, 4288, 4261, 4273, 4241, 4263, 4275, 4243, 4254, 4286, 4290, 4280, 4259, 4271, 4239, 4251, 4283 } },
	{ 88, 4167, 4179, 4157, 4187, 4151, 4195, 816, 0, 0, 26, 0, 5, 0, { 5, 5, 5, 5, 5, 5 }, { -10, 0, -4, 0, -4, -13 }, { -1, 0, -16, -10, -6, 0 }, 350, 348, 1, 1, 0, 0, 760, 825, 18, new uint16_t[18]{ 4169, 4181, 4153, 4159, 4189, 4163, 4175, 4147, 4161, 4173, 4145, 4155, 4185, 4165, 4177, 4149, 4191, 4183 } },
	{ 89, 4221, 4221, 4213, 4233, 4205, 4205, 1200, 0, 3, 41, 2, 3, 2, { 3, 3, 3, 3, 3, 2 }, { -12, 0, -4, -1, -4, -14 }, { -4, -8, -16, -10, -9, 0 }, 420, 660, 0, 1, 0, -76, 759, 824, 13, new uint16_t[13]{ 4223, 4232, 4207, 4215, 4238, 4217, 4227, 4201, 4219, 4229, 4203, 4212, 4236 } },
	{ 90, 4125, 4125, 4117, 4137, 4111, 4111, 1088, 0, 0, 45, 2, 4, 2, { 3, 3, 3, 3, 3, 1 }, { -10, -1, -6, -3, -6, -19 }, { 0, -9, -15, -12, -7, 0 }, 392, 684, 0, 2, 0, 2, 758, 823, 13, new uint16_t[13]{ 4127, 4136, 4113, 4119, 4142, 4121, 4131, 4107, 4123, 4133, 4109, 4116, 4140 } },
	{ 92, 4615, 4615, 4607, 4623, 4603, 4603, 800, 0, 1, 9, 0, 24, 0, { 33, 33, 33, 33, 33, 1 }, { -8, 0, -3, 0, -3, -19 }, { 0, -2, -10, -7, -3, 0 }, 224, 216, 1, 1, 0, 0, 763, 828, 10, new uint16_t[10]{ 4613, 4619, 4601, 4606, 4626, 4628, 4622, 4611, 4617, 4599 } },
	{ 93, 937, 937, 4563, 4585, 4555, 4555, 1264, 0, 0, 28, 3, 5, 2, { 7, 7, 7, 7, 7, 2 }, { -8, 0, -3, 2, -3, -23 }, { -10, -8, -19, -12, -14, 0 }, 532, 756, 0, 2, 0, 28, 762, 827, 19, new uint16_t[19]{ 4567, 4577, 4547, 4557, 4587, 4573, 4583, 4553, 4571, 4581, 4551, 4561, 4591, 4569, 4579, 4549, 4559, 4589, 4575 } },
	{ 94, 4912, 4912, 4908, 4916, 4906, 4906, 656, 0, 3, 24, 0, 5, 0, { 2, 2, 2, 2, 2, 1 }, { -12, 0, -10, -2, -10, -19 }, { 0, 0, -11, -9, -6, 0 }, 154, 156, 1, 33, 0, 0, 766, 831, 3, new uint16_t[3]{ 4910, 4914, 4904 } },
	{ 95, 4307, 4307, 4301, 4315, 4295, 4295, 1168, 0, 0, 38, 0, 24, 0, { 1, 1, 1, 1, 1, 1 }, { -44, -23, -35, -27, -35, -48 }, { -1, -23, -30, -21, -15, 0 }, 812, 816, 1, 12, 0, 0, 765, 830, 8, new uint16_t[8]{ 4309, 4314, 4297, 4303, 4318, 4305, 4311, 4293 } },
	{ 96, 3119, 3129, 3109, 3135, 3103, 3093, 976, 0, 0, 1, 15, 14, 15, { 15, 15, 15, 15, 15, 1 }, { -32, -27, -31, -15, -31, -51 }, { 5, -14, -22, -16, -13, 0 }, 644, 648, 1, 15, 0, 0, 764, 829, 16, new uint16_t[16]{ 3117, 3127, 3101, 3115, 3125, 3099, 3121, 3131, 3105, 3111, 3137, 3113, 3123, 3097, 3139, 3133 } },
	{ 97, 4341, 4341, 4333, 4351, 4325, 4325, 1248, 0, 0, 18, 0, 15, 0, { 14, 14, 14, 14, 14, 1 }, { -8, 4, 0, 5, 0, -13 }, { -4, -16, -30, -25, -13, 0 }, 210, 708, 2, 19, 0, 50, 767, 832, 14, new uint16_t[14]{ 4343, 4350, 4327, 4335, 4356, 4337, 4345, 4321, 4339, 4347, 4323, 4332, 4354, 938 } },
	{ 105, 1656, 1656, 1648, 3865, 1642, 1642, 686, 0, 0, 34, 0, 15, 0, { 3, 3, 3, 3, 3, 1 }, { -8, 0, -5, -2, -5, -24 }, { 0, 0, -10, -8, -4, 0 }, 252, 252, 26, 27, 0, 0, 746, 811, 0, NULL },
	{ 125, 11278, 11272, 11274, 11272, 11272, 11272, 640, 0, 0, 24, 0, 11, 0, { 11, 11, 11, 11, 11, 1 }, { -14, 0, -8, -1, -8, -24 }, { 0, -2, -9, -8, -4, 0 }, 182, 180, 24, 24, 0, 0, 65535, 65535, 6, new uint16_t[6]{ 11276, 11280, 13078, 11282, 11272, 11278 } },
	{ 135, 3842, 3842, 3832, 3855, 3826, 3826, 752, 0, 28, 4, 0, 33, 0, { 29, 29, 29, 29, 29, 1 }, { -11, 0, -7, -3, -7, -20 }, { 0, 0, -9, -8, -4, 0 }, 182, 192, 1, 0, 0, 0, 745, 810, 14, new uint16_t[14]{ 3844, 3854, 3828, 3834, 3858, 3838, 3850, 3822, 3835, 3847, 3840, 3852, 3824, 3830 } },
	{ 136, 4496, 4496, 4490, 4505, 4484, 4484, 768, 0, 2, 34, 0, 5, 0, { 2, 2, 2, 2, 2, 1 }, { -8, 0, -5, -1, -5, -16 }, { 0, 0, -9, -7, -4, 0 }, 140, 336, 15, 4, 0, 0, 747, 812, 8, new uint16_t[8]{ 4498, 4504, 4486, 4492, 4508, 4493, 4501, 4481 } },
	{ 137, 3881, 3881, 3875, 3889, 3871, 3871, 1088, 0, 0, 9, 0, 7, 0, { 2, 2, 2, 2, 2, 1 }, { -16, 0, -11, 1, -11, -20 }, { -6, -20, -24, -23, -12, 0 }, 336, 492, 0, 1, 0, 0, 117, 124, 8, new uint16_t[8]{ 3880, 3888, 3870, 3874, 3892, 3878, 3886, 3868 } },
	{ 138, 4947, 4937, 4953, 4929, 4958, 4923, 800, 0, 1, 7, 0, 20, 0, { 18, 18, 18, 18, 18, 1 }, { -14, 0, -9, -3, -9, -14 }, { 3, 0, -8, -4, -3, 0 }, 266, 276, 1, 3, 0, 0, 119, 125, 14, new uint16_t[14]{ 4939, 4949, 4925, 4931, 4955, 4933, 4943, 4919, 4935, 4945, 4921, 4947, 4950, 4956 } },
	{ 141, 1543, 1543, 1179, 3049, 1175, 1175, 994, 0, 0, 45, 2, 8, 2, { 1, 1, 1, 1, 1, 1 }, { -52, -21, -45, -27, -45, -54 }, { 3, -24, -19, -19, -9, 0 }, 770, 564, 1, 2, 0, 0, 748, 813, 8, new uint16_t[8]{ 1545, 3048, 1177, 1181, 3052, 1184, 1546, 1172 } },
	{ 142, 4535, 4535, 4525, 4545, 4523, 4523, 1312, 0, 0, 58, 4, 18, 4, { 4, 4, 4, 4, 4, 1 }, { -39, -33, -36, -32, -36, -85 }, { 4, -4, -10, -5, -2, 0 }, 742, 744, 1, 1, 0, 0, 751, 816, 12, new uint16_t[12]{ 4528, 4538, 4516, 4532, 4542, 4520, 4534, 4544, 4522, 4530, 4540, 4518 } },
	{ 143, 3932, 3932, 3924, 3941, 3916, 3916, 1008, 0, 0, 37, 0, 21, 0, { 5, 5, 5, 5, 5, 1 }, { -19, -20, -12, -5, -12, -35 }, { 0, 0, -37, -34, -16, 0 }, 504, 504, 1, 1, 0, 0, 750, 815, 17, new uint16_t[17]{ 3934, 3940, 3918, 3926, 3946, 3928, 3936, 3912, 3930, 3938, 3914, 3922, 3944, 8722, 8724, 8725, 8727 } },
	{ 144, 1686, 1686, 1678, 3899, 1672, 1672, 704, 0, 0, 14, 0, 2, 0, { 2, 2, 2, 2, 2, 2 }, { -3, 0, -2, -1, -2, -8 }, { 0, 0, -5, -2, -2, 0 }, 84, 48, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 1688, 1694, 1674, 1680, 1696, 1683, 1691, 1669 } },
	{ 145, 1712, 1712, 1704, 3905, 1700, 1700, 1168, 0, 0, 32, 0, 24, 0, { 3, 3, 3, 3, 3, 2 }, { -11, 1, -6, 2, -6, -14 }, { -3, -12, -17, -14, -9, 0 }, 294, 564, 0, 2, 0, 0, 749, 814, 9, new uint16_t[9]{ 1714, 1718, 1702, 1706, 1720, 1708, 1715, 1697, 1710 } },
	{ 146, 1523, 1523, 1520, 2862, 1518, 1518, 960, 0, 0, 2, 3, 6, 3, { 6, 6, 6, 6, 6, 1 }, { -38, -26, -27, -19, -27, -94 }, { 2, -11, -20, -7, -5, 0 }, 742, 1884, 1, 24, 0, 511, 755, 820, 8, new uint16_t[8]{ 1522, 1525, 1517, 1519, 1526, 1521, 1524, 1516 } },
	{ 147, 13767, 13767, 13759, 13776, 13755, 13755, 752, 0, 0, 0, 14, 0, 14, { 32, 32, 32, 32, 32, 1 }, { -11, 0, -5, 0, -5, -30 }, { -6, -12, -14, -13, -8, 0 }, 238, 192, 12, 33, 0, 1, 754, 819, 8, new uint16_t[8]{ 13763, 13771, 13751, 13769, 13775, 13757, 13761, 13779 } },
	{ 148, 3996, 3996, 3990, 4003, 3986, 3986, 800, 0, 12, 49, 0, 9, 0, { 2, 2, 2, 2, 2, 1 }, { -9, -4, -5, -2, -5, -21 }, { 0, 0, -9, -5, -4, 0 }, 182, 180, 1, 1, 0, 0, 753, 818, 9, new uint16_t[9]{ 3998, 4002, 3988, 3992, 4006, 3993, 3999, 3983, 3996 } },
	{ 149, 3964, 3964, 3958, 3973, 3952, 3952, 728, 0, 3, 23, 0, 3, 0, { 2, 2, 2, 2, 2, 1 }, { -15, 0, -9, -1, -9, -22 }, { 0, 0, -11, -9, -5, 0 }, 140, 276, 1, 18, 0, 80, 752, 817, 12, new uint16_t[12]{ 3966, 3970, 3954, 3960, 3976, 3978, 3972, 14531, 9651, 6707, 6713, 6695 } },
	{ 150, 4054, 4054, 4046, 4063, 4040, 4040, 848, 0, 0, 15, 0, 21, 0, { 0, 0, 0, 0, 0, 1 }, { -14, -5, -10, -4, -10, -16 }, { -6, -18, -20, -19, -13, 0 }, 448, 420, 0, 3, 0, -24, 756, 821, 13, new uint16_t[13]{ 4056, 4062, 4042, 4048, 4068, 4050, 4058, 4036, 4052, 4060, 4038, 4044, 4066 } },
	{ 151, 1737, 1737, 1729, 3533, 1725, 1725, 880, 0, 0, 9, 0, 19, 0, { 13, 13, 13, 13, 13, 1 }, { -6, 0, -3, 0, -3, -9 }, { -4, -4, -10, -7, -7, 0 }, 238, 324, 34, 1, 0, 88, 131, 130, 8, new uint16_t[8]{ 1739, 1746, 1727, 1731, 1748, 1735, 1743, 1723 } },
	{ 152, 3485, 3485, 3477, 3497, 3469, 3469, 798, 0, 3, 52, 0, 12, 0, { 9, 9, 9, 9, 9, 1 }, { -7, 0, -1, 2, -1, -18 }, { 1, 0, -10, -7, -4, 0 }, 182, 180, 1, 1, 0, 0, 23, 132, 9, new uint16_t[9]{ 3487, 3496, 3471, 3479, 3500, 3483, 3493, 3467, 3476 } },
	{ 153, 3431, 3431, 3423, 3443, 3417, 3417, 720, 0, 0, 29, 0, 11, 0, { 0, 0, 0, 0, 0, 1 }, { -11, 0, -5, 0, -5, -13 }, { -1, 0, -11, -9, -6, 0 }, 322, 192, 1, 0, 0, 0, 397, 396, 11, new uint16_t[11]{ 3433, 3442, 3419, 3425, 3446, 3427, 3437, 3413, 3429, 3439, 3415 } },
	{ 154, 3549, 3549, 3543, 3559, 3539, 3539, 728, 0, 0, 4, 13, 21, 13, { 0, 0, 0, 0, 0, 1 }, { -20, -10, -16, -13, -16, -22 }, { 0, -3, -12, -8, -5, 0 }, 210, 204, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 3551, 3558, 3541, 3545, 3562, 3547, 3555, 3537 } },
	{ 155, 1769, 1757, 1759, 1757, 1753, 1757, 742, 0, 0, 29, 0, 10, 0, { 0, 0, 0, 0, 0, 1 }, { -19, -1, -6, 2, -6, -21 }, { -2, -6, -18, -15, -12, 0 }, 224, 228, 1, 1, 0, 0, 730, 729, 13, new uint16_t[13]{ 1771, 1778, 1755, 1761, 1782, 1765, 1775, 1751, 1763, 1773, 1749, 1757, 1767 } },
	{ 155, 9653, 9653, 1759, 1759, 1753, 1753, 756, 0, 0, 29, 0, 10, 0, { 0, 0, 0, 0, 0, 1 }, { -19, -1, -6, 2, -6, -21 }, { -2, -6, -18, -15, -12, 0 }, 224, 228, 1, 1, 0, 0, 730, 729, 1, new uint16_t[1]{ 1767 } },
	{ 156, 3459, 3459, 3453, 3463, 3451, 3451, 736, 0, 2, 24, 0, 14, 0, { 2, 2, 2, 2, 2, 1 }, { -14, -3, -8, -3, -8, -17 }, { -1, 0, -11, -8, -6, 0 }, 98, 96, 1, 1, 0, 0, 65535, 65535, 3, new uint16_t[3]{ 3457, 3461, 3449 } },
	{ 157, 3579, 3579, 3571, 3587, 3567, 3567, 1008, 0, 0, 11, 0, 4, 0, { 25, 25, 25, 25, 25, 25 }, { -17, 0, -7, 2, -7, -21 }, { 0, -12, -16, -11, -10, 0 }, 378, 480, 0, 1, 0, -78, 401, 400, 8, new uint16_t[8]{ 3581, 3586, 11856, 3573, 3590, 3577, 3583, 3565 } },
	{ 158, 3669, 3669, 3661, 3679, 3655, 3655, 816, 0, 0, 15, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -9, -2, -5, 1, -5, -10 }, { -3, -4, -14, -10, -8, 0 }, 182, 252, 0, 2, 0, 0, 740, 803, 11, new uint16_t[11]{ 3671, 3678, 3657, 3663, 3682, 3665, 3673, 3651, 3667, 3675, 3653 } },
	{ 159, 3621, 3621, 3611, 3637, 3601, 3601, 1536, 0, 0, 9, 23, 38, 23, { 38, 38, 38, 38, 38, 22 }, { -8, -1, -4, -1, -4, -10 }, { -5, -10, -21, -17, -14, 0 }, 378, 900, 1, 22, 0, 0, 395, 394, 15, new uint16_t[15]{ 3623, 3632, 3603, 3613, 3644, 3617, 3627, 3597, 3619, 3629, 3599, 3610, 3642, 3648, 3636 } },
	{ 160, 3761, 3761, 3755, 3771, 3751, 3751, 1184, 0, 0, 25, 10, 12, 10, { 17, 17, 17, 17, 17, 9 }, { -9, -2, -2, 0, -2, -14 }, { 1, -2, -7, -4, -2, 0 }, 252, 504, 0, 17, 0, 145, 743, 806, 12, new uint16_t[12]{ 3763, 3768, 3753, 3757, 3776, 3778, 3770, 3773, 3746, 3759, 3765, 3749 } },
	{ 161, 1799, 1799, 1791, 3741, 1789, 1789, 752, 0, 0, 1, 2, 6, 2, { 0, 0, 0, 0, 0, 1 }, { -16, -4, -10, -6, -10, -20 }, { 0, -2, -13, -8, -9, 0 }, 126, 132, 1, 1, 0, 0, 742, 805, 9, new uint16_t[9]{ 1797, 1805, 1787, 1795, 1803, 1785, 1793, 1801, 1783 } },
	{ 162, 3723, 3723, 3715, 3733, 3711, 3711, 728, 0, 12, 11, 0, 31, 0, { 27, 27, 27, 27, 27, 1 }, { -9, 0, -5, 0, -5, -17 }, { -1, 0, -11, -8, -5, 0 }, 252, 252, 1, 1, 0, 0, 741, 804, 11, new uint16_t[11]{ 3725, 3732, 3713, 3717, 3736, 3719, 3727, 3707, 3721, 3729, 3709 } },
	{ 163, 1823, 1823, 1815, 1815, 3793, 3793, 1456, 0, 0, 12, 0, 23, 0, { 2, 2, 2, 2, 2, 2 }, { -11, -4, -6, -2, -6, -23 }, { -10, -16, -20, -13, -14, 0 }, 448, 636, 0, 1, 0, 0, 744, 807, 11, new uint16_t[11]{ 1825, 1832, 1813, 1817, 1834, 1819, 1827, 1807, 1821, 1829, 1809 } },
	{ 164, 5257, 5265, 5249, 5271, 5245, 5284, 1056, 0, 0, 51, 19, 35, 19, { 25, 25, 25, 25, 25, 19 }, { -10, -2, -8, -2, -8, -20 }, { 0, -2, -13, -8, -6, 0 }, 196, 420, 1, 19, 0, 98, 785, 850, 11, new uint16_t[11]{ 5253, 5261, 5240, 5251, 5255, 5263, 5243, 5248, 5270, 5267, 5273 } },
	{ 184, 13241, 13241, 13241, 13241, 13241, 13241, 752, 0, 2, 0, 12, 0, 12, { 22, 22, 22, 22, 22, 1 }, { -10, 0, -6, 0, -6, -27 }, { -3, -3, -17, -14, -8, 0 }, 252, 252, 1, 6, 0, 0, 65535, 65535, 13, new uint16_t[13]{ 13243, 13250, 13225, 13233, 13256, 13237, 13219, 13235, 13239, 13247, 13221, 13230, 13254 } },
	{ 165, 5319, 5319, 5309, 5333, 5301, 5301, 1204, 0, 0, 45, 0, 19, 0, { 14, 14, 16, 14, 14, 2 }, { -12, 0, -8, -3, -8, -22 }, { -4, -12, -16, -20, -11, 0 }, 504, 852, 1, 2, 0, 0, 788, 853, 13, new uint16_t[13]{ 5321, 5332, 5303, 5311, 5340, 5315, 5327, 5288, 5317, 5329, 5290, 5308, 5338 } },
	{ 166, 6035, 6035, 6027, 6043, 6021, 6021, 1040, 0, 0, 3, 5, 13, 5, { 13, 13, 13, 13, 13, 1 }, { -20, -7, -14, 0, -14, -46 }, { -5, -5, -17, -10, -10, 0 }, 658, 660, 1, 35, 0, 0, 791, 856, 10, new uint16_t[10]{ 6033, 6041, 6019, 6035, 6031, 6039, 6017, 6029, 6037, 6015 } },
	{ 221, 5636, 5636, 5630, 5642, 5626, 5626, 1024, 0, 0, 2, 3, 2, 3, { 0, 0, 0, 0, 0, 1 }, { -27, -16, -22, -16, -22, -28 }, { -8, -19, -23, -19, -14, 0 }, 462, 300, 3, 3, 57, 0, 768, 833, 7, new uint16_t[7]{ 5634, 5640, 5624, 5629, 5632, 5638, 5622 } },
	{ 244, 9919, 9933, 9907, 9943, 9897, 9948, 686, 1, 0, 0, 11, 0, 11, { 1, 1, 1, 1, 1, 1 }, { -13, -6, -10, -6, -10, -16 }, { -1, -1, -11, -9, -6, 0 }, 98, 96, 1, 1, 0, 0, 65535, 65535, 18, new uint16_t[18]{ 9921, 9935, 9899, 9909, 9945, 9919, 9913, 9927, 9915, 9929, 9893, 9917, 9931, 9895, 9905, 9941, 9946, 9936 } },
	{ 261, 7835, 7835, 7827, 7845, 7821, 7821, 528, 0, 0, 33, 0, 50, 0, { 13, 13, 13, 13, 13, 1 }, { -20, -10, -20, 2, -20, -25 }, { -2, -12, -21, -19, -12, 0 }, 294, 300, 2, 1, 0, 0, 65535, 65535, 12, new uint16_t[12]{ 7837, 7844, 7823, 7829, 7831, 7839, 7817, 7833, 7841, 7819, 7826, 7848 } },
	{ 265, 5666, 5664, 5656, 5656, 5676, 5676, 736, 65535, 6, 8, 0, 26, 0, { 24, 24, 24, 24, 24, 1 }, { -10, 0, -6, -2, -6, -19 }, { 0, -1, -10, -9, -5, 0 }, 196, 216, 1, 0, 0, 0, 745, 810, 12, new uint16_t[12]{ 5668, 5675, 5654, 5658, 5679, 5662, 5672, 5650, 5660, 5670, 5648, 5664 } },
	{ 266, 5700, 5700, 5692, 5712, 5686, 5686, 816, 0, 0, 23, 43, 53, 43, { 0, 0, 0, 0, 0, 1 }, { -17, -7, -14, -7, -14, -21 }, { -6, -10, -24, -19, -13, 0 }, 280, 288, 2, 0, 0, 6, 769, 834, 12, new uint16_t[12]{ 5702, 5709, 5688, 5694, 5715, 5717, 5711, 5696, 5706, 5684, 5698, 5700 } },
	{ 278, 11366, 11376, 11354, 11384, 11348, 11340, 816, 0, 7, 4, 0, 28, 0, { 28, 28, 28, 28, 28, 1 }, { -16, 0, -7, -1, -7, -19 }, { 2, -7, -11, -8, -6, 0 }, 182, 180, 1, 1, 0, 0, 1272, 1296, 17, new uint16_t[17]{ 11368, 11378, 11350, 11356, 11386, 11362, 11372, 11344, 11360, 11370, 11342, 11376, 11960, 11348, 8717, 11340, 11388 } },
	{ 295, 2567, 2567, 2867, 2869, 2565, 2565, 704, 65535, 62, 22, 0, 2, 0, { 6, 6, 6, 6, 6, 1 }, { -12, -2, -9, -1, -9, -24 }, { 0, -1, -12, -9, -5, 0 }, 224, 228, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 2343, 2345, 2367, 2369, 2443 } },
	{ 296, 12251, 12251, 12247, 12256, 12243, 12243, 728, 65535, 14, 8, 0, 20, 0, { 20, 20, 20, 20, 20, 1 }, { -11, 0, -6, -1, -6, -22 }, { 0, -3, -10, -9, -5, 0 }, 210, 204, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 12253, 12255, 12245, 12249, 12259 } },
	{ 297, 14705, 14705, 14701, 14703, 14699, 14699, 688, 65535, 7, 8, 0, 23, 0, { 23, 23, 23, 23, 23, 1 }, { -11, 0, -7, -2, -7, -21 }, { 0, -2, -10, -8, -4, 0 }, 182, 192, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 14689, 14691, 14693, 14695, 14697 } },
	{ 298, 12218, 12224, 12206, 12206, 12202, 12202, 728, 0, 14, 4, 0, 20, 0, { 20, 20, 20, 20, 20, 1 }, { -11, 0, -6, -1, -6, -22 }, { 0, -3, -10, -9, -5, 0 }, 210, 204, 1, 1, 0, 0, 65535, 65535, 7, new uint16_t[7]{ 12220, 12231, 12204, 12208, 12235, 12212, 12224 } },
	{ 298, 12218, 9459, 12206, 9459, 12202, 9459, 728, 0, 14, 4, 0, 20, 0, { 20, 20, 20, 20, 20, 1 }, { -11, 0, -6, -1, -6, -22 }, { 0, -3, -10, -9, -5, 0 }, 210, 204, 1, 1, 0, 0, 65535, 65535, 7, new uint16_t[7]{ 12220, 12231, 12204, 12208, 12235, 12218, 9457 } },
	{ 298, 12218, 12222, 12206, 12206, 12202, 12202, 728, 0, 14, 4, 0, 20, 0, { 20, 20, 20, 20, 20, 1 }, { -11, 0, -6, -1, -6, -22 }, { 0, -3, -10, -9, -5, 0 }, 210, 204, 1, 1, 0, 0, 65535, 65535, 10, new uint16_t[10]{ 12220, 12231, 12204, 12208, 12235, 12218, 12216, 12228, 12210, 12222 } },
	{ 299, 8219, 8219, 8213, 8227, 8209, 8209, 742, 0, 0, 18, 0, 8, 0, { 8, 8, 8, 8, 8, 1 }, { -9, 0, -5, 0, -5, -17 }, { -1, -2, -13, -10, -5, 0 }, 210, 216, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 8221, 8226, 8211, 8215, 8230, 8217, 8223, 8207 } },
	{ 300, 10704, 10706, 10698, 10698, 10710, 10710, 672, 0, 0, 2, 11, 13, 11, { 13, 13, 12, 13, 13, 1 }, { -10, 0, -9, 2, -9, -23 }, { -1, -2, -14, -7, -8, 0 }, 168, 168, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 10706, 10692, 10700, 10704, 10694 } },
	{ 301, 7569, 7591, 7559, 7559, 7591, 7559, 720, 0, 7, 4, 0, 19, 0, { 19, 19, 19, 19, 19, 1 }, { -9, 0, -5, -2, -5, -23 }, { -1, -1, -11, -8, -5, 0 }, 224, 228, 1, 1, 0, 0, 65535, 65535, 22, new uint16_t[22]{ 7571, 7580, 7658, 7561, 7590, 7568, 7578, 7550, 7558, 7588, 7563, 7573, 7545, 7553, 7583, 7565, 7575, 7547, 7555, 7585, 7581, 7591 } },
	{ 302, 10730, 10734, 10724, 10724, 10738, 10738, 672, 0, 0, 2, 11, 13, 11, { 13, 13, 12, 13, 13, 1 }, { -10, 0, -9, 2, -9, -23 }, { -1, -2, -14, -7, -8, 0 }, 168, 168, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 10718, 10734, 10716, 10726, 10730 } },
	{ 303, 13074, 13074, 11232, 11234, 11230, 11230, 704, 0, 0, 23, 0, 7, 0, { 2, 2, 2, 2, 2, 1 }, { -16, 0, -8, -2, -8, -23 }, { -1, -2, -13, -10, -8, 0 }, 224, 216, 1, 1, 0, 0, 65535, 65535, 3, new uint16_t[3]{ 174, 13076, 170 } },
	{ 304, 11251, 11251, 11245, 11260, 11239, 11239, 704, 0, 0, 17, 0, 3, 0, { 3, 3, 3, 3, 3, 1 }, { -10, 0, -5, 0, -5, -20 }, { 0, -2, -10, -11, -4, 0 }, 210, 216, 2, 2, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 11253, 11259, 11241, 11247, 11263, 11248, 11256, 11236 } },
	{ 305, 11424, 11424, 11418, 11432, 11414, 11414, 720, 0, 14, 22, 0, 3, 0, { 2, 2, 2, 2, 2, 1 }, { -16, 0, -9, -3, -9, -24 }, { -1, -2, -12, -11, -5, 0 }, 224, 228, 2, 2, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 11426, 11431, 11416, 11420, 11435, 11422, 11428, 11412 } },
	{ 323, 10767, 10767, 10761, 10776, 10755, 10755, 688, 2, 0, 17, 0, 37, 0, { 37, 37, 37, 37, 37, 1 }, { -9, 0, -5, -2, -5, -17 }, { -1, 0, -9, -7, -4, 0 }, 140, 144, 1, 1, 0, 0, 773, 838, 8, new uint16_t[8]{ 10769, 10775, 10757, 10763, 10779, 10764, 10772, 10752 } },
	{ 324, 7746, 7746, 7736, 7755, 7730, 7730, 1776, 0, 0, 26, 2, 3, 2, { 3, 3, 3, 3, 3, 2 }, { -18, 0, -13, -1, -13, -41 }, { -6, -36, -43, -46, -21, 0 }, 1162, 1848, 0, 2, 0, 100, 776, 841, 13, new uint16_t[13]{ 7748, 7754, 7732, 7738, 7760, 7742, 7750, 7726, 7744, 7752, 7728, 7734, 7758 } },
	{ 325, 3074, 3080, 3064, 3086, 3060, 3091, 1056, 0, 0, 5, 23, 46, 23, { 43, 43, 43, 43, 43, 1 }, { -9, 0, -3, 2, -3, -16 }, { -4, -2, -12, -10, -7, 0 }, 252, 720, 1, 2, 0, 0, 777, 842, 10, new uint16_t[10]{ 3076, 3082, 3062, 3066, 3088, 3071, 3077, 3057, 3089, 3083 } },
	{ 326, 6270, 6270, 6262, 6283, 6256, 6256, 848, 0, 1, 38, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -10, -5, -7, -3, -7, -21 }, { 0, -1, -11, -8, -6, 0 }, 336, 336, 1, 35, 0, 0, 780, 845, 17, new uint16_t[17]{ 6272, 6278, 6258, 6264, 6290, 6266, 6274, 6252, 6268, 6276, 6254, 6260, 6286, 6292, 6280, 6282, 6248 } },
	{ 327, 6322, 6322, 6312, 6333, 6304, 6304, 944, 0, 7, 31, 2, 5, 2, { 4, 4, 4, 4, 4, 1 }, { -34, -2, -16, -3, -16, -47 }, { -1, -9, -22, -7, -14, 0 }, 616, 648, 26, 30, 0, 50, 779, 844, 15, new uint16_t[15]{ 6324, 6332, 6306, 6314, 6340, 6320, 6330, 6302, 6310, 6338, 6315, 6325, 6297, 6307, 6335 } },
	{ 328, 5736, 5736, 5730, 5742, 5724, 5724, 736, 0, 2, 43, 2, 13, 2, { 12, 12, 12, 12, 12, 1 }, { -15, -4, -11, -8, -11, -30 }, { -1, -7, -15, -11, -8, 0 }, 308, 300, 1, 1, 0, 0, 778, 843, 8, new uint16_t[8]{ 5732, 5738, 5720, 5728, 5744, 5734, 5740, 5722 } },
	{ 329, 5775, 5775, 5765, 5786, 5757, 5757, 704, 0, 0, 51, 0, 2, 0, { 6, 6, 6, 6, 6, 1 }, { -12, -4, -8, -1, -8, -26 }, { 0, -3, -19, -15, -8, 0 }, 322, 324, 1, 2, 0, 0, 781, 846, 11, new uint16_t[11]{ 5777, 5783, 5759, 5767, 5789, 5771, 5779, 5753, 5773, 5781, 5755 } },
	{ 330, 5815, 5815, 5807, 5824, 5801, 5801, 656, 0, 0, 30, 0, 9, 0, { 8, 8, 8, 8, 8, 1 }, { -8, 0, -3, 0, -3, -11 }, { 2, 0, -14, -9, -5, 0 }, 210, 492, 1, 29, 0, 141, 784, 849, 12, new uint16_t[12]{ 5817, 5823, 5803, 5809, 5811, 5819, 5797, 5813, 5821, 5799, 5805, 5827 } },
	{ 331, 7792, 7792, 7782, 7803, 7772, 7772, 640, 0, 0, 36, 2, 3, 2, { 3, 3, 3, 3, 3, 1 }, { -14, 0, -6, 4, -6, -18 }, { 0, -6, -11, -9, -4, 0 }, 504, 264, 1, 3, 0, 0, 783, 848, 8, new uint16_t[8]{ 7794, 7802, 7774, 7784, 7810, 7788, 7798, 7768 } },
	{ 332, 13204, 13204, 13196, 13213, 13192, 13192, 896, 0, 6, 0, 3, 6, 3, { 5, 5, 5, 5, 5, 1 }, { -23, 0, -7, 0, -7, -34 }, { -5, 0, -21, -14, -14, 0 }, 322, 432, 1, 41, 0, 26, 782, 847, 11, new uint16_t[11]{ 13206, 13212, 13194, 13198, 13216, 13200, 13208, 13188, 13202, 13210, 13190 } },
	{ 333, 5852, 5852, 5844, 5860, 5840, 5840, 672, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 1 }, { -14, 0, -5, 3, -5, -18 }, { 0, 0, -18, -17, -13, 0 }, 182, 180, 1, 1, 0, 0, 1264, 1288, 11, new uint16_t[11]{ 5851, 5859, 5839, 5849, 5857, 5837, 5846, 5854, 5834, 5842, 5862 } },
	{ 334, 9966, 9966, 9960, 9972, 9958, 9958, 848, 0, 0, 0, 10, 15, 0, { 16, 18, 0, 0, 0, 18 }, { 13, 0, -24, -19, -24, -21 }, { -11, 0, -16, -14, -21, 0 }, 168, 456, 1, 18, 0, 49, 65535, 65535, 6, new uint16_t[6]{ 9962, 9968, 9954, 9964, 9970, 9956 } },
	{ 335, 10794, 10794, 10786, 10786, 10804, 10804, 720, 0, 0, 49, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -8, 0, -6, -5, -6, -21 }, { 0, -2, -11, -8, -4, 0 }, 224, 228, 1, 1, 0, 0, 65535, 65535, 10, new uint16_t[10]{ 10792, 10802, 10782, 10788, 10798, 10790, 10800, 10780, 10794, 10804 } },
	{ 336, 7869, 7869, 7861, 7881, 7853, 7853, 1330, 0, 0, 27, 2, 4, 2, { 4, 4, 4, 4, 4, 2 }, { -24, 0, -8, 5, -8, -30 }, { -15, -12, -26, -20, -20, 0 }, 476, 972, 1, 2, 0, 100, 1266, 1290, 13, new uint16_t[13]{ 7871, 7880, 7855, 7863, 7888, 7865, 7875, 7849, 7867, 7877, 7851, 7860, 7886 } },
	{ 337, 13289, 13289, 13277, 13307, 13265, 13265, 1296, 0, 0, 26, 2, 2, 2, { 5, 5, 5, 5, 5, 2 }, { -18, 0, -4, 5, -4, -24 }, { -4, -21, -22, -17, -16, 0 }, 504, 744, 1, 3, 0, 76, 1265, 1289, 25, new uint16_t[25]{ 13291, 13304, 13267, 13279, 13316, 13318, 13306, 13285, 13299, 13261, 13281, 13295, 13257, 13271, 13309, 13283, 13297, 13259, 13273, 13311, 13287, 13301, 13263, 13276, 13314 } },
	{ 338, 3205, 3205, 3155, 3219, 3147, 3147, 2032, 0, 0, 37, 2, 8, 2, { 7, 7, 7, 7, 7, 7 }, { -42, 8, -38, -9, -38, -62 }, { 1, -18, -24, -14, -13, 0 }, 1246, 1764, 2, 31, 0, 218, 787, 852, 17, new uint16_t[17]{ 3207, 3218, 3149, 3157, 3224, 3161, 3213, 3143, 3159, 3211, 3141, 3205, 3163, 3215, 3145, 3154, 3222 } },
	{ 339, 5985, 5997, 5975, 6003, 5969, 6011, 1280, 0, 0, 31, 0, 13, 0, { 2, 2, 2, 2, 2, 1 }, { -18, -10, -12, -4, -12, -67 }, { 0, -2, -21, -17, -8, 0 }, 1008, 1056, 1, 27, 0, 0, 786, 851, 17, new uint16_t[17]{ 5987, 5999, 5971, 5977, 6005, 5981, 5993, 5965, 5979, 5991, 5963, 5983, 5995, 5967, 5974, 6002, 6007 } },
	{ 340, 2564, 2564, 2864, 2866, 2566, 2566, 1344, 0, 0, 24, 0, 29, 0, { 31, 31, 31, 31, 31, 2 }, { -9, 0, -9, -3, -9, -26 }, { 0, -9, -16, -13, -9, 0 }, 462, 504, 1, 2, 0, 36, 789, 854, 25, new uint16_t[25]{ 5075, 5077, 5079, 7246, 10505, 14555, 14557, 14559, 14561, 14563, 14565, 14567, 14569, 14571, 14573, 14575, 14577, 14579, 14581, 14583, 12908, 13628, 14549, 14551, 14553 } },
	{ 341, 5890, 5890, 5878, 5902, 5870, 5870, 686, 0, 0, 38, 0, 2, 0, { 2, 2, 2, 2, 3, 1 }, { -11, -3, -8, -5, -8, -27 }, { 3, -7, -12, -10, -3, 0 }, 742, 576, 38, 51, 0, 72, 790, 855, 18, new uint16_t[18]{ 5892, 5901, 5872, 5880, 5909, 5886, 5896, 5866, 5884, 5894, 5864, 5874, 5904, 5888, 5898, 5868, 5877, 5907 } },
	{ 342, 13341, 13341, 13333, 13353, 13327, 13327, 896, 0, 0, 43, 2, 38, 2, { 37, 37, 37, 37, 37, 32 }, { -12, 0, -7, 3, -7, -20 }, { -10, -16, -26, -18, -18, 0 }, 308, 912, 0, 2, 0, 91, 792, 857, 15, new uint16_t[15]{ 13343, 13350, 13329, 13335, 13358, 13337, 13345, 13323, 13339, 13347, 13325, 13332, 13356, 13360, 13352 } },
	{ 343, 6369, 6369, 6359, 6381, 6349, 6349, 966, 0, 3, 49, 2, 5, 2, { 4, 4, 4, 4, 4, 1 }, { -7, -2, -4, 0, -4, -33 }, { 0, -7, -14, -8, -7, 0 }, 532, 372, 1, 1, 0, 0, 1267, 1291, 18, new uint16_t[18]{ 6371, 6380, 6351, 6361, 6388, 6365, 6375, 6345, 6363, 6373, 6343, 6355, 6383, 6367, 6377, 6347, 6358, 6386 } },
	{ 344, 6405, 6391, 6397, 6391, 6413, 6391, 1264, 0, 3, 36, 2, 4, 2, { 42, 42, 10, 10, 10, 1 }, { -9, 1, -8, -1, -8, -31 }, { -9, -8, -13, -9, -6, 0 }, 322, 492, 0, 33, 0, 60, 1269, 1293, 9, new uint16_t[9]{ 6407, 6412, 6393, 6399, 6416, 6403, 6409, 6389, 6413 } },
	{ 345, 7987, 7987, 7979, 7995, 7975, 7975, 736, 0, 0, 19, 0, 3, 0, { 3, 3, 3, 3, 3, 1 }, { -8, -3, -3, -1, -3, -16 }, { 5, -2, -7, -3, -2, 0 }, 140, 240, 14, 1, 0, -60, 734, 733, 8, new uint16_t[8]{ 7989, 7994, 7977, 7981, 7998, 7985, 7991, 7973 } },
	{ 346, 13611, 13611, 13603, 13621, 13597, 13597, 1184, 0, 0, 0, 5, 9, 5, { 9, 9, 9, 9, 9, 1 }, { -7, 0, -4, 1, -4, -43 }, { 0, -8, -13, -9, -5, 0 }, 224, 540, 20, 2, 0, -64, 793, 858, 10, new uint16_t[10]{ 13613, 13607, 13615, 13593, 13609, 13617, 13595, 14395, 13624, 14601 } },
	{ 347, 10090, 10090, 10082, 10102, 10074, 10074, 1280, 0, 0, 7, 7, 7, 7, { 7, 7, 7, 7, 7, 7 }, { -24, -1, -20, -6, -20, -62 }, { -7, -14, -22, -10, -14, 0 }, 630, 672, 0, 1, 0, 0, 728, 727, 15, new uint16_t[15]{ 10092, 10101, 10076, 10090, 10086, 10096, 10070, 10084, 10094, 10068, 10080, 10104, 10088, 10098, 10072 } },
	{ 348, 11078, 11078, 11068, 11068, 11078, 11078, 720, 0, 0, 28, 5, 10, 5, { 9, 9, 11, 9, 9, 9 }, { -11, 4, -7, 2, -7, -15 }, { 0, -13, -6, -15, -9, 0 }, 770, 1536, 1, 1, 0, 0, 794, 859, 15, new uint16_t[15]{ 11080, 11089, 11065, 11070, 11093, 11074, 11084, 11060, 11076, 11086, 11062, 11067, 11091, 11072, 11082 } },
	{ 349, 11112, 11118, 11112, 11106, 11112, 11094, 640, 65535, 0, 48, 0, 11, 0, { 11, 11, 11, 14, 11, 11 }, { -13, 2, -7, 0, -7, -18 }, { -2, -10, -19, -11, -10, 0 }, 462, 1140, 46, 46, 0, 0, 1271, 1295, 18, new uint16_t[18]{ 11116, 11128, 11100, 11105, 11133, 11114, 11126, 11098, 11118, 11108, 11120, 11094, 11124, 11110, 11122, 11096, 11102, 11130 } },
	{ 350, 10134, 10134, 10122, 10112, 14605, 14605, 1248, 0, 0, 4, 9, 11, 9, { 10, 10, 12, 10, 10, 9 }, { -10, 0, -8, -4, -8, -19 }, { -6, -18, -20, -16, -11, 0 }, 532, 528, 1, 8, 0, 0, 1274, 1298, 17, new uint16_t[17]{ 10129, 10143, 10115, 10121, 10149, 10130, 10144, 10116, 10126, 10140, 10112, 10132, 10124, 10138, 10110, 10118, 10146 } },
	{ 351, 10170, 10170, 10160, 1395, 1400, 1400, 640, 0, 0, 79, 0, 2, 0, { 46, 46, 46, 46, 46, 1 }, { -24, -2, -5, -1, -5, -40 }, { -6, -8, -14, -10, -9, 0 }, 308, 264, 72, 79, 0, 100, 1280, 1304, 8, new uint16_t[8]{ 10172, 10180, 10159, 10162, 10184, 10166, 10174, 10154 } },
	{ 352, 10935, 10945, 10923, 10953, 10915, 10958, 686, 0, 0, 3, 35, 47, 35, { 38, 38, 38, 38, 38, 38 }, { -12, 0, -8, -1, -8, -14 }, { 0, -1, -14, -17, -8, 0 }, 700, 696, 1, 1, 0, 0, 795, 860, 17, new uint16_t[17]{ 10937, 10947, 10917, 10925, 10955, 10931, 10941, 10911, 10956, 10948, 10933, 10943, 10921, 10951, 10928, 10938, 10908 } },
	{ 353, 11027, 11043, 11013, 11049, 11007, 11054, 848, 14, 0, 8, 8, 8, 8, { 12, 12, 12, 12, 12, 13 }, { -32, -12, -18, -9, -18, -30 }, { 0, -27, -30, -23, -18, 0 }, 462, 192, 8, 8, 0, 0, 65535, 65535, 22, new uint16_t[22]{ 11029, 11045, 11009, 11015, 11051, 11024, 11040, 11004, 11023, 11039, 11003, 11017, 11033, 10998, 11043, 11019, 11035, 11021, 11037, 11001, 11010, 11046 } },
	{ 354, 13587, 13587, 13583, 13583, 13587, 13587, 624, 0, 0, 0, 6, 2, 6, { 2, 2, 2, 2, 2, 2 }, { -31, 0, -12, -1, -12, -63 }, { -20, -14, -23, -9, -16, 0 }, 420, 240, 1, 1, 0, 32, 65535, 65535, 3, new uint16_t[3]{ 13585, 13589, 13579 } },
	{ 355, 6439, 6439, 6431, 6447, 6425, 6425, 1072, 0, 0, 16, 3, 13, 3, { 8, 8, 8, 8, 8, 1 }, { -8, 2, -5, -1, -5, -34 }, { -2, -8, -15, -10, -6, 0 }, 378, 672, 2, 39, 0, 128, 799, 864, 14, new uint16_t[14]{ 6438, 6446, 6424, 6430, 6452, 6436, 6444, 6422, 6439, 6433, 6441, 6419, 6427, 6449 } },
	{ 356, 13641, 13631, 13635, 13635, 13631, 13631, 560, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 1 }, { -8, 0, -8, -2, -8, -13 }, { 1, 0, -6, -3, -3, 0 }, 112, 36, 2, 2, 0, 0, 65535, 65535, 6, new uint16_t[6]{ 13637, 13645, 13649, 13647, 13639, 13629 } },
	{ 357, 13661, 13653, 13655, 13653, 13653, 13653, 560, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 1 }, { -8, 0, -8, -2, -8, -13 }, { 1, 0, -6, -3, -3, 0 }, 112, 36, 2, 2, 0, 0, 65535, 65535, 6, new uint16_t[6]{ 13657, 13663, 13667, 13665, 13659, 13651 } },
	{ 359, 7639, 7629, 7633, 7629, 7629, 7629, 720, 0, 7, 13, 0, 21, 0, { 22, 22, 22, 22, 22, 1 }, { -9, 0, -5, -2, -5, -23 }, { -1, -1, -11, -8, -5, 0 }, 378, 372, 13, 2, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 7641, 7646, 7631, 7635, 7650, 7637, 7643, 7627 } },
	{ 362, 10889, 10889, 10877, 10900, 10867, 10867, 736, 0, 0, 42, 2, 5, 0, { 5, 5, 5, 5, 5, 1 }, { -11, 0, -7, 0, -7, -47 }, { 0, -14, -16, -12, -7, 0 }, 630, 624, 1, 1, 0, 0, 796, 861, 18, new uint16_t[18]{ 10891, 10899, 10869, 10879, 10907, 10885, 10895, 10863, 10882, 10892, 10860, 10872, 10902, 10887, 10897, 10865, 10875, 10905 } },
	{ 363, 13561, 13561, 13551, 13569, 13545, 13545, 1472, 0, 0, 0, 4, 8, 4, { 13, 13, 13, 13, 13, 11 }, { -10, 0, -5, -2, -5, -20 }, { 0, -8, -12, -5, -7, 0 }, 938, 876, 0, 1, 32, 74, 1275, 1299, 13, new uint16_t[13]{ 13560, 13568, 13544, 13550, 13574, 13558, 13566, 13542, 13555, 13563, 13539, 13547, 13571 } },
	{ 364, 9991, 9991, 9985, 9998, 9979, 9979, 686, 0, 0, 69, 0, 53, 0, { 46, 46, 46, 46, 46, 1 }, { -22, -8, -5, -1, -5, -39 }, { -4, -5, -15, -9, -12, 0 }, 196, 264, 69, 73, 0, 39, 1280, 1304, 8, new uint16_t[8]{ 9993, 9997, 9981, 9987, 10001, 9988, 9994, 9976 } },
	{ 369, 13478, 13490, 13464, 13498, 13458, 13503, 816, 0, 0, 53, 2, 8, 2, { 8, 8, 8, 8, 8, 1 }, { -11, -4, -6, -2, -6, -14 }, { 0, 4, -10, -7, -4, 0 }, 140, 252, 2, 1, 0, 10, 1310, 1311, 20, new uint16_t[20]{ 13480, 13492, 13460, 13466, 13500, 13501, 13475, 13487, 13455, 13474, 13486, 13454, 13470, 13482, 13468, 13472, 13484, 13452, 13461, 13495 } },
	{ 370, 11920, 11920, 11904, 11940, 11894, 11894, 1008, 0, 0, 72, 3, 9, 3, { 9, 3, 9, 9, 9, 9 }, { -14, -4, -5, -1, -5, -12 }, { -7, -13, -14, -9, -7, 0 }, 448, 708, 1, 1, 0, -78, 798, 863, 29, new uint16_t[29]{ 11922, 11937, 11896, 11906, 11947, 11918, 11934, 11892, 11916, 11932, 11890, 11902, 11944, 11910, 11926, 11884, 11912, 11928, 11886, 11924, 11938, 11898, 11908, 11948, 11914, 11930, 11888, 11900, 11942 } },
	{ 371, 11775, 11767, 11783, 11759, 11746, 11753, 432, 0, 0, 45, 0, 6, 0, { 6, 6, 6, 6, 6, 4 }, { -10, 3, -4, 1, -4, -16 }, { -2, -11, -15, -14, -7, 0 }, 322, 984, 35, 39, 0, 224, 1277, 1301, 15, new uint16_t[15]{ 11765, 11773, 11751, 11757, 11781, 11763, 11771, 11749, 11769, 11777, 11755, 11761, 11785, 11786, 11778 } },
	{ 378, 11816, 11816, 11802, 11802, 11832, 11832, 928, 0, 0, 27, 0, 42, 0, { 78, 78, 78, 78, 78, 78 }, { -16, -4, -10, -3, -10, -20 }, { 0, -12, -15, -9, -5, 0 }, 574, 576, 1, 17, 0, 0, 1282, 1306, 20, new uint16_t[20]{ 11818, 11831, 11798, 11812, 11826, 11792, 11808, 11822, 11814, 11828, 11794, 11810, 11824, 11790, 11806, 11820, 11788, 11800, 11834, 11816 } },
	{ 379, 7905, 7911, 7897, 7917, 7893, 7925, 784, 0, 0, 53, 0, 21, 0, { 0, 0, 0, 22, 0, 1 }, { -20, 0, -16, 4, -16, -24 }, { -3, -6, -15, -10, -9, 0 }, 280, 276, 1, 36, 0, 0, 1268, 1292, 10, new uint16_t[10]{ 7907, 7913, 7895, 7899, 7919, 7903, 7909, 7891, 7921, 7915 } },
	{ 379, 7911, 7905, 7917, 7897, 7925, 7893, 784, 0, 0, 53, 0, 21, 0, { 0, 0, 0, 22, 0, 1 }, { -20, 0, -16, 4, -16, -24 }, { -3, -6, -15, -10, -9, 0 }, 280, 276, 1, 36, 0, 0, 1268, 1292, 10, new uint16_t[10]{ 7907, 7913, 7895, 7899, 7919, 7903, 7909, 7891, 7921, 7915 } },
	{ 398, 6669, 6679, 6659, 6687, 6653, 6645, 816, 0, 0, 15, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -10, -2, -5, 1, -5, -14 }, { -3, -4, -14, -10, -7, 0 }, 182, 252, 0, 2, 0, 0, 2115, 2110, 11, new uint16_t[11]{ 6671, 6681, 6655, 6661, 6689, 6663, 6673, 6685, 6657, 6691, 6683 } },
	{ 399, 6709, 6709, 6703, 6719, 6697, 6697, 728, 0, 3, 21, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -15, 0, -9, -1, -9, -22 }, { 0, 0, -11, -9, -5, 0 }, 140, 276, 1, 18, 0, 80, 2113, 2109, 12, new uint16_t[12]{ 6711, 6716, 6699, 6705, 6722, 6724, 6718, 9655, 9657, 6707, 6713, 6695 } },
	{ 400, 6741, 6741, 6737, 6737, 6733, 6733, 736, 0, 2, 24, 0, 3, 0, { 2, 2, 2, 2, 2, 1 }, { -14, -3, -8, -3, -8, -17 }, { -1, 0, -11, -8, -6, 0 }, 98, 96, 1, 1, 0, 0, 65535, 65535, 4, new uint16_t[4]{ 6736, 6745, 6731, 6748 } },
	{ 401, 6770, 6770, 6760, 6760, 6754, 6754, 704, 0, 0, 13, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -3, 0, -2, -1, -2, -8 }, { 0, 0, -5, -2, 0, 0 }, 84, 48, 1, 1, 0, 0, 65535, 65535, 4, new uint16_t[4]{ 6749, 6757, 6765, 6773 } },
	{ 402, 6808, 6818, 6796, 6824, 6790, 6831, 704, 0, 0, 3, 0, 2, 0, { 6, 6, 6, 6, 6, 1 }, { -12, -4, -8, -1, -8, -26 }, { 0, -3, -19, -15, -8, 0 }, 322, 324, 1, 2, 0, 0, 2117, 2112, 6, new uint16_t[6]{ 6801, 6811, 6783, 6793, 6827, 6821 } },
	{ 403, 6855, 6863, 6845, 6867, 6839, 6871, 832, 0, 0, 23, 0, 10, 0, { 7, 7, 7, 7, 7, 1 }, { -6, -4, -4, 0, -4, -10 }, { -1, -5, -8, -6, -4, 0 }, 112, 108, 0, 2, 0, 24, 65535, 65535, 10, new uint16_t[10]{ 6843, 6833, 6869, 6865, 6849, 6857, 6863, 6852, 6860, 6836 } },
	{ 404, 6893, 6901, 6883, 6905, 6879, 6911, 686, 0, 0, 34, 2, 23, 2, { 2, 2, 2, 2, 2, 1 }, { -10, -4, -7, -3, -7, -17 }, { -1, -1, -13, -11, -5, 0 }, 210, 168, 0, 0, 0, 0, 2120, 2127, 6, new uint16_t[6]{ 6887, 6895, 6873, 6881, 6907, 6903 } },
	{ 405, 6937, 6945, 6929, 6951, 6923, 6917, 768, 0, 2, 27, 0, 2, 0, { 2, 2, 2, 2, 2, 1 }, { -8, 0, -5, -1, -5, -19 }, { 0, 0, -9, -7, -4, 0 }, 140, 336, 15, 4, 0, 0, 2116, 2111, 6, new uint16_t[6]{ 6933, 6941, 6927, 6919, 6955, 6949 } },
	{ 410, 14739, 4762, 14737, 4762, 14739, 4762, 784, 0, 7, 6, 0, 22, 0, { 21, 21, 21, 21, 21, 21 }, { -10, 0, -7, -2, -7, -12 }, { -1, 0, -9, -8, -5, 0 }, 182, 180, 1, 1, 0, 0, 65535, 65535, 13, new uint16_t[13]{ 33, 35, 37, 42, 44, 3468, 3470, 3472, 4148, 4749, 4762, 4456, 14739 } },
	{ 416, 11865, 11865, 11857, 11874, 11851, 11851, 976, 0, 0, 14, 0, 19, 0, { 19, 19, 19, 19, 19, 1 }, { -22, 5, -14, -7, -14, -45 }, { -4, -26, -14, -12, -9, 0 }, 616, 612, 17, 5, 0, -50, 1279, 1303, 13, new uint16_t[13]{ 11867, 11873, 11853, 11859, 11879, 11861, 11869, 11847, 11863, 11871, 11849, 11855, 11877 } },
	{ 427, 2844, 2844, 2840, 2843, 2839, 2839, 742, 0, 7, 9, 0, 18, 0, { 18, 18, 18, 18, 18, 1 }, { -10, 0, -6, -1, -6, -18 }, { -1, -2, -9, -8, -5, 0 }, 168, 180, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 2796, 2831, 2832, 2835, 2837, 2846, 2849, 2850 } },
	{ 428, 7609, 7625, 7601, 7601, 7595, 7595, 720, 1, 23, 4, 0, 24, 0, { 24, 24, 24, 24, 24, 1 }, { -10, 0, -7, -3, -7, -31 }, { -4, -3, -17, -15, -9, 0 }, 224, 228, 1, 1, 0, 0, 65535, 65535, 17, new uint16_t[17]{ 7611, 7618, 7597, 7603, 7624, 7608, 7616, 7594, 7600, 7622, 7605, 7613, 7651, 7653, 7655, 7619, 7625 } },
	{ 429, 7669, 7669, 7669, 7669, 7669, 7669, 686, 1, 0, 0, 0, 0, 0, { 3, 3, 3, 3, 3, 3 }, { -10, 0, 0, 0, 0, -8 }, { 0, 0, -10, -8, -8, 0 }, 308, 192, 1, 1, 0, 0, 65535, 65535, 1, new uint16_t[1]{ 7671 } },
	{ 430, 7673, 7673, 7673, 7673, 7673, 7673, 728, 1, 0, 0, 0, 0, 0, { 2, 2, 2, 2, 2, 2 }, { -10, 0, 0, 0, 0, -8 }, { 0, 0, -10, -8, -8, 0 }, 308, 192, 1, 1, 0, 0, 65535, 65535, 1, new uint16_t[1]{ 7675 } },
	{ 445, 11400, 11400, 11394, 11408, 2853, 2853, 742, 0, 2, 21, 0, 4, 0, { 2, 2, 2, 2, 2, 1 }, { -10, 0, -6, -2, -6, -22 }, { 0, -4, -10, -9, -5, 0 }, 224, 228, 1, 1, 0, 0, 65535, 65535, 9, new uint16_t[9]{ 11402, 11407, 11393, 11396, 11411, 11398, 11404, 11390, 11400 } },
	{ 446, 12555, 1503, 1394, 1394, 12555, 12555, 736, 0, 0, 5, 0, 51, 0, { 51, 51, 51, 51, 51, 1 }, { -10, 0, -6, -2, -6, -24 }, { 0, -2, -10, -8, -4, 0 }, 238, 228, 1, 1, 0, -16, 65535, 65535, 14, new uint16_t[14]{ 12555, 1402, 10014, 10016, 10018, 10020, 1409, 1411, 1499, 1396, 1503, 1385, 1392, 1501 } },
	{ 450, 11298, 11304, 11292, 11292, 11308, 11308, 736, 0, 0, 5, 0, 13, 0, { 13, 13, 13, 13, 13, 1 }, { -21, -3, -6, 2, -6, -27 }, { 0, -9, -15, -11, -10, 0 }, 294, 264, 1, 1, 0, 0, 1270, 1294, 11, new uint16_t[11]{ 11296, 11304, 11284, 11298, 8228, 6615, 11300, 11307, 11288, 11294, 11311 } },
	{ 451, 13703, 13703, 13695, 13713, 13687, 13687, 1424, 1, 0, 50, 2, 3, 2, { 3, 3, 3, 3, 3, 2 }, { -21, -2, -17, -5, -17, -26 }, { 7, -15, -16, -14, -1, 0 }, 392, 1272, 2, 50, 0, 301, 802, 802, 13, new uint16_t[13]{ 13705, 13712, 13689, 13697, 13718, 13701, 13709, 13685, 13694, 13716, 13699, 13707, 13683 } },
	{ 512, 13922, 13922, 13910, 13932, 13902, 13902, 976, 0, 0, 40, 0, 26, 0, { 26, 26, 26, 26, 26, 26 }, { -16, 0, -7, -3, -7, -25 }, { -11, -26, -29, -30, -21, 0 }, 448, 1272, 40, 40, 0, 208, 65535, 65535, 18, new uint16_t[18]{ 13917, 13927, 13897, 13907, 13937, 13919, 13929, 13899, 13915, 13925, 13895, 13905, 13935, 13921, 13931, 13901, 13909, 13939 } },
	{ 545, 10041, 10031, 10025, 10025, 10017, 10017, 944, 0, 0, 26, 0, 30, 0, { 30, 30, 30, 30, 30, 28 }, { -13, 0, -5, 1, -5, -14 }, { -3, -11, -18, -16, -9, 0 }, 1260, 600, 58, 64, -22, 88, 797, 862, 28, new uint16_t[28]{ 10043, 10057, 10019, 10027, 10065, 10037, 10053, 10013, 10033, 10049, 10009, 10021, 10061, 10058, 10031, 10017, 10047, 10035, 10051, 10011, 10023, 10063, 10029, 10045, 10006, 10039, 10055, 10015 } },
	{ 546, 14024, 14024, 14016, 14034, 14012, 14012, 752, 0, 1, 11, 0, 40, 0, { 39, 39, 39, 39, 39, 1 }, { -10, 0, -8, -3, -8, -18 }, { 1, -2, -11, -8, -5, 0 }, 196, 180, 1, 1, 0, 0, 801, 801, 17, new uint16_t[17]{ 14026, 14033, 14014, 14018, 14039, 14020, 14028, 14008, 14022, 14030, 14010, 14624, 14037, 9519, 9659, 9660, 9666 } },
	{ 548, 13430, 13438, 13422, 13444, 13416, 13449, 688, 2, 0, 5, 0, 47, 0, { 37, 37, 37, 37, 37, 1 }, { -9, 0, -5, -2, -5, -17 }, { -1, 0, -9, -7, -4, 0 }, 140, 144, 1, 1, 0, 0, 2118, 2114, 6, new uint16_t[6]{ 13425, 13433, 13411, 13419, 13447, 13441 } },
	{ 553, 13388, 13398, 13376, 13404, 13368, 13409, 896, 0, 6, 43, 3, 6, 3, { 5, 5, 7, 5, 5, 1 }, { -24, 0, -6, 0, -6, -40 }, { 0, 0, -10, -14, -9, 0 }, 322, 432, 1, 41, 0, 26, 2119, 2126, 6, new uint16_t[6]{ 13381, 13391, 13361, 13373, 13407, 13401 } },
	{ 556, 10981, 10981, 10973, 10992, 10965, 10965, 752, 0, 0, 7, 23, 23, 23, { 53, 53, 53, 53, 53, 23 }, { -14, -2, -8, 0, -8, -19 }, { -7, -20, -20, -22, -14, 0 }, 350, 900, 0, 17, 0, 252, 1276, 1300, 13, new uint16_t[13]{ 10983, 10991, 10967, 10975, 10997, 10977, 10987, 10961, 10979, 10989, 10963, 10971, 10995 } },
	{ 557, 12046, 12042, 12044, 12042, 12042, 12042, 864, 0, 0, 5, 0, 20, 0, { 11, 11, 11, 11, 11, 1 }, { -9, -2, -5, 0, -5, -9 }, { 0, -3, -11, -8, -7, 0 }, 154, 156, 1, 1, 0, 0, 65535, 65535, 1, new uint16_t[1]{ 12050 } },
	{ 558, 12298, 12292, 12294, 12292, 12292, 12292, 864, 0, 0, 24, 0, 9, 0, { 8, 7, 7, 7, 7, 1 }, { -10, -5, -6, 0, -6, -13 }, { 1, -2, -9, -10, -5, 0 }, 168, 168, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 12296, 12300, 12290, 12298, 12304 } },
	{ 566, 8710, 12976, 13087, 12976, 8710, 12976, 592, 0, 0, 29, 0, 42, 35, { 42, 42, 42, 42, 42, 1 }, { -14, -3, -9, -2, -9, -28 }, { -1, -3, -13, -13, -8, 0 }, 210, 216, 1, 1, 0, 0, 65535, 65535, 12, new uint16_t[12]{ 11283, 11285, 11287, 11279, 11281, 10754, 11289, 11290, 11293, 11837, 12980, 12978 } },
	{ 567, 11358, 10135, 11358, 10135, 11358, 10135, 640, 0, 0, 13, 0, 57, 0, { 37, 37, 37, 37, 37, 1 }, { -11, 0, -7, -2, -7, -24 }, { -1, -2, -12, -10, -7, 0 }, 196, 192, 2, 3, 0, 0, 65535, 65535, 4, new uint16_t[4]{ 11277, 11270, 11273, 11275 } },
	{ 569, 5491, 5491, 5487, 5489, 5485, 5485, 686, 65535, 28, 9, 0, 21, 0, { 21, 21, 21, 21, 21, 1 }, { -10, 0, -7, -1, -7, -18 }, { -1, -2, -11, -10, -6, 0 }, 196, 204, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 5475, 5477, 5479, 5481, 5483 } },
	{ 570, 5577, 5577, 5573, 5575, 5571, 5571, 656, 65535, 6, 5, 0, 19, 0, { 19, 19, 19, 19, 19, 1 }, { -10, 0, -7, -1, -7, -17 }, { -1, 0, -9, -7, -5, 0 }, 126, 132, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 5561, 5563, 5565, 5567, 5569 } },
	{ 571, 11213, 11213, 11207, 11220, 11203, 11203, 704, 65535, 7, 6, 0, 20, 0, { 19, 19, 19, 19, 19, 1 }, { -11, 0, -8, -3, -8, -18 }, { 0, -1, -9, -8, -3, 0 }, 154, 168, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 11215, 11219, 11205, 11209, 11223 } },
	{ 572, 5600, 5600, 5597, 5599, 5595, 5595, 816, 65535, 7, 5, 0, 18, 0, { 18, 18, 18, 18, 18, 1 }, { -13, -2, -7, 1, -7, -22 }, { 0, 0, -10, -11, -6, 0 }, 238, 240, 1, 1, 0, 0, 65535, 65535, 5, new uint16_t[5]{ 5585, 5586, 5589, 5591, 5593 } },
	{ 573, 5619, 8844, 5614, 5614, 8844, 8844, 816, 0, 7, 5, 0, 18, 0, { 18, 18, 18, 18, 18, 1 }, { -13, -2, -7, 1, -7, -22 }, { 0, 0, -10, -11, -6, 0 }, 238, 240, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 5603, 5605, 5606, 5608, 5611, 8843, 8844, 5619 } },
	{ 576, 13974, 13986, 13966, 14002, 13952, 14006, 1712, 0, 0, 0, 3, 64, 3, { 64, 64, 64, 64, 64, 64 }, { -11, -4, -9, -2, -9, -23 }, { -8, -14, -17, -10, -12, 0 }, 826, 828, 1, 1, 0, 0, 1284, 1308, 15, new uint16_t[15]{ 13976, 13970, 13984, 13950, 13972, 13986, 13978, 13988, 13954, 14004, 14006, 13990, 13956, 13952, 13992 } },
	{ 591, 10823, 10823, 10817, 10832, 10813, 10813, 752, 0, 3, 5, 0, 18, 0, { 18, 18, 18, 18, 18, 1 }, { -10, 0, -6, 0, -6, -19 }, { -1, -2, -10, -8, -5, 0 }, 168, 168, 1, 1, 0, 0, 65535, 65535, 9, new uint16_t[9]{ 10825, 10829, 10815, 10819, 10835, 10820, 10826, 10810, 11224 } },
	{ 592, 10848, 10848, 10842, 10856, 10838, 10838, 742, 0, 7, 5, 0, 16, 0, { 16, 16, 16, 16, 16, 1 }, { -11, 0, -7, -3, -7, -18 }, { -1, -1, -10, -8, -4, 0 }, 182, 180, 1, 1, 0, 0, 65535, 65535, 6, new uint16_t[6]{ 10850, 10855, 10840, 10844, 10859, 11201 } },
	{ 593, 11326, 11326, 11318, 11336, 11314, 11314, 688, 0, 14, 35, 0, 11, 0, { 11, 11, 11, 11, 11, 1 }, { -12, 0, -6, -1, -6, -18 }, { 0, -7, -8, -8, -4, 0 }, 182, 180, 1, 1, 0, 0, 1272, 1296, 10, new uint16_t[10]{ 11328, 11333, 11316, 11320, 11339, 11324, 11330, 11312, 11318, 11326 } },
	{ 610, 13526, 13526, 13518, 13535, 13512, 13512, 512, 0, 0, 7, 0, 35, 0, { 34, 34, 34, 34, 34, 22 }, { -11, -1, -7, 1, -7, -14 }, { -3, -7, -14, -14, -8, 0 }, 266, 492, 2, 22, 0, 0, 800, 865, 11, new uint16_t[11]{ 13528, 13534, 13514, 13520, 13538, 13522, 13530, 13508, 13524, 13532, 13510 } },
	{ 618, 14076, 14076, 14064, 14064, 14092, 14092, 928, 0, 0, 20, 0, 42, 0, { 78, 78, 78, 78, 78, 78 }, { -16, -4, -10, -3, -10, -20 }, { 0, -12, -15, -9, -5, 0 }, 574, 576, 1, 17, 0, 0, 1282, 1306, 14, new uint16_t[14]{ 14078, 14091, 14060, 14072, 14086, 14054, 14068, 14082, 14074, 14088, 14056, 14050, 14062, 14094 } },
	{ 619, 14118, 14118, 14110, 14110, 14130, 14130, 656, 0, 6, 18, 24, 50, 24, { 49, 49, 48, 49, 49, 30 }, { -10, 0, -5, 3, -5, -10 }, { 1, -6, -15, -7, -3, 0 }, 224, 552, 6, 11, 0, 191, 1281, 1305, 15, new uint16_t[15]{ 14120, 14127, 14104, 14112, 14135, 9995, 9996, 9999, 14117, 14125, 14101, 14109, 14133, 14137, 14129 } },
	{ 620, 14627, 14627, 14631, 11864, 11868, 11868, 640, 0, 0, 68, 0, 2, 0, { 46, 46, 46, 46, 46, 1 }, { -24, -2, -5, -1, -5, -40 }, { -6, -8, -14, -10, -9, 0 }, 308, 264, 72, 79, 0, 100, 1280, 1304, 8, new uint16_t[8]{ 14639, 14641, 14643, 14645, 14647, 14649, 14651, 14653 } },
	{ 621, 14176, 14176, 14158, 14196, 14148, 14148, 1008, 0, 0, 68, 3, 3, 3, { 9, 3, 9, 9, 9, 9 }, { -14, -4, -5, -1, -5, -12 }, { -7, -13, -14, -9, -7, 0 }, 448, 708, 1, 1, 0, -78, 798, 863, 24, new uint16_t[24]{ 14178, 14193, 14150, 14160, 14203, 14174, 14190, 14146, 14172, 14188, 14144, 14156, 14200, 14166, 14182, 14138, 14168, 14184, 14140, 14170, 14186, 14142, 14154, 14198 } },
	{ 639, 2994, 2994, 2994, 2994, 2994, 2994, 686, 0, 0, 19, 0, 3, 0, { 3, 3, 3, 3, 3, 1 }, { -10, 0, 0, 0, 0, -19 }, { 0, 0, -10, -8, -8, 0 }, 140, 144, 16, 16, 0, 0, 65535, 65535, 0, NULL },
	{ 663, 8326, 8326, 8330, 8358, 8329, 8329, 1088, 0, 0, 9, 0, 7, 0, { 2, 2, 2, 2, 2, 1 }, { -16, 0, -11, 1, -11, -20 }, { -6, -20, -24, -23, -12, 0 }, 336, 492, 0, 1, 0, 0, 117, 124, 7, new uint16_t[7]{ 8366, 8368, 8370, 8372, 8360, 8362, 8364 } },
	{ 664, 6616, 6616, 6620, 7182, 6619, 6619, 742, 0, 0, 18, 0, 8, 0, { 8, 8, 8, 8, 8, 1 }, { -9, 0, -5, 0, -5, -17 }, { -1, -2, -13, -10, -5, 0 }, 210, 216, 1, 1, 0, 0, 65535, 65535, 8, new uint16_t[8]{ 7184, 7186, 7188, 7190, 7192, 7971, 7974, 8225 } },
	{ 671, 9963, 9963, 9959, 9961, 9957, 9957, 686, 65535, 14, 4, 0, 19, 0, { 19, 19, 19, 19, 19, 1 }, { -10, 0, -7, -1, -7, -18 }, { 0, -2, -9, -8, -5, 0 }, 140, 144, 1, 1, 0, 36, 65535, 65535, 5, new uint16_t[5]{ 9947, 9949, 9950, 9953, 9955 } },
	{ 672, 10673, 10773, 11202, 11262, 10673, 10673, 1712, 0, 0, 3, 0, 13, 0, { 13, 1, 13, 13, 13, 13 }, { -28, 0, -12, 2, -12, -34 }, { 0, -30, -28, -22, -18, 0 }, 2422, 2424, 1, 1, 0, 0, 65535, 65535, 20, new uint16_t[20]{ 11165, 11167, 11169, 11177, 11179, 11171, 11173, 11175, 11011, 11012, 11014, 11268, 10732, 10673, 10735, 10737, 10739, 10753, 11295, 11264 } },
	{ 673, 10695, 10695, 10695, 10695, 10695, 10695, 770, 0, 0, 0, 0, 3, 0, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, 0, 0, 1, 1, 0, 0, 65535, 65535, 0, NULL },
	{ 676, 10785, 10785, 10793, 10795, 10787, 10787, 640, 0, 0, 36, 2, 3, 2, { 3, 3, 3, 3, 3, 1 }, { -14, 0, -6, 4, -6, -18 }, { 0, -6, -11, -9, -4, 0 }, 504, 264, 1, 3, 0, 0, 783, 848, 19, new uint16_t[19]{ 10796, 10799, 10801, 10803, 10805, 10822, 10824, 10827, 10828, 10833, 10785, 10812, 10814, 10816, 10818, 10821, 10807, 10809, 10811 } },
	{ 5458, 381, 381, 380, 3699, 3689, 3689, 960, 0, 0, 26, 0, 11, 0, { 2, 2, 2, 2, 2, 1 }, { -11, -2, -8, -3, -8, -18 }, { -2, -4, -12, -9, -7, 0 }, 224, 360, 29, 2, 0, 48, 393, 392, 9, new uint16_t[9]{ 383, 384, 378, 3692, 3702, 3693, 3697, 3687, 3695 } },
	{ 5459, 398, 398, 391, 3785, 386, 386, 848, 0, 1, 8, 0, 15, 0, { 14, 14, 14, 14, 14, 1 }, { -10, 2, -3, 2, -3, -25 }, { -9, -9, -18, -11, -12, 0 }, 392, 456, 1, 14, 0, 0, 111, 808, 8, new uint16_t[8]{ 399, 401, 388, 3784, 3788, 397, 400, 385 } },
	{ 5460, 361, 361, 358, 3815, 356, 356, 960, 0, 0, 14, 0, 10, 0, { 2, 2, 2, 2, 2, 1 }, { -10, -1, -6, 0, -6, -11 }, { -3, -7, -11, -10, -6, 0 }, 224, 360, 0, 1, 0, 88, 110, 809, 13, new uint16_t[13]{ 362, 375, 357, 359, 3820, 3808, 3813, 3801, 3810, 373, 355, 3806, 3818 } },
	{ 5461, 11536, 6910, 11528, 6910, 11520, 6910, 864, 0, 0, 2, 12, 3, 2, { 52, 0, 16, 0, 16, 1 }, { -3, -1, -3, 1, -3, -41 }, { -13, -22, -13, -20, -5, 0 }, 392, 384, 2, 3, 0, 0, 732, 731, 18, new uint16_t[18]{ 11538, 11546, 11524, 11530, 11576, 11580, 5497, 5499, 11518, 11527, 11569, 11578, 11544, 11534, 11542, 5500, 11548, 11536 } },
	{ 5461, 11536, 11536, 11528, 11528, 11520, 11520, 864, 0, 0, 2, 12, 3, 3, { 52, 0, 16, 0, 16, 1 }, { -3, -1, -3, 1, -3, -41 }, { -13, -22, -13, -20, -5, 0 }, 392, 384, 2, 3, 0, 0, 732, 731, 18, new uint16_t[18]{ 11538, 11546, 11524, 11530, 11576, 5497, 5499, 11518, 11527, 11569, 11520, 11578, 11544, 11534, 11542, 5500, 11548, 11536 } }
};

BattleModelLinks& BattleModelLinks::GetLinksByModel(uint16_t modelid) {
	unsigned int i;
	for (i=0;i<G_N_ELEMENTS(BATTLE_MODEL_LINKS);i++)
		if (modelid==BATTLE_MODEL_LINKS[i].model)
			return BATTLE_MODEL_LINKS[i];
	return BATTLE_MODEL_LINKS[0];
}

bool BattleModelLinks::IsBattleModel(uint16_t modelid) {
	if (modelid==0xFFFF)
		return false;
	unsigned int i;
	for (i=0;i<G_N_ELEMENTS(BATTLE_MODEL_LINKS);i++)
		if (modelid==BATTLE_MODEL_LINKS[i].model)
			return true;
	return false;
}

void BattleModelLinks::ApplyToEnemyStat(EnemyStatDataStruct& es) {
	unsigned int i;
	es.model = model;
	es.anim_idle = anim_idle;
	es.anim_idle_alt = anim_idle_alt;
	es.anim_hit = anim_hit;
	es.anim_hit_alt = anim_hit_alt;
	es.anim_death = anim_death;
	es.anim_death_alt = anim_death_alt;
	es.radius = radius;
	es.mesh = mesh;
	es.mesh_vanish = mesh_vanish;
	es.bone_camera1 = bone_camera1;
	es.bone_camera2 = bone_camera2;
	es.bone_camera3 = bone_camera3;
	es.bone_target = bone_target;
	es.shadow_x = shadow_x;
	es.shadow_y = shadow_y;
	es.shadow_bone1 = shadow_bone1;
	es.shadow_bone2 = shadow_bone2;
	es.shadow_offset_x = shadow_offset_x;
	es.shadow_offset_y = shadow_offset_y;
	es.sound_engage = sound_engage;
	es.sound_death = sound_death;
	for (i=0;i<6;i++) {
		es.selection_bone[i] = selection_bone[i];
		es.selection_offsetz[i] = selection_offsetz[i];
		es.selection_offsety[i] = selection_offsety[i];
	}
}






#include <string>

struct DummyBattleAnimStruct {
	uint32_t id;
	string name;
};

static DummyBattleAnimStruct DummyBattleAnim[] = {
	{ 125, "ANH_MAIN_B0_000_000" },
	{ 128, "ANH_MAIN_B0_000_001" },
	{ 112, "ANH_MAIN_B0_000_002" },
	{ 7202, "ANH_MAIN_B0_000_010" },
	{ 129, "ANH_MAIN_B0_000_011" },
	{ 123, "ANH_MAIN_B0_000_020" },
	{ 126, "ANH_MAIN_B0_000_021" },
	{ 109, "ANH_MAIN_B0_000_022" },
	{ 7183, "ANH_MAIN_B0_000_023" },
	{ 110, "ANH_MAIN_B0_000_032" },
	{ 116, "ANH_MAIN_B0_000_033" },
	{ 7197, "ANH_MAIN_B0_000_040" },
	{ 7199, "ANH_MAIN_B0_000_050" },
	{ 7195, "ANH_MAIN_B0_000_100" },
	{ 7217, "ANH_MAIN_B0_000_101" },
	{ 7177, "ANH_MAIN_B0_000_102" },
	{ 7181, "ANH_MAIN_B0_000_103" },
	{ 7221, "ANH_MAIN_B0_000_104" },
	{ 7223, "ANH_MAIN_B0_000_105" },
	{ 7209, "ANH_MAIN_B0_000_200" },
	{ 7219, "ANH_MAIN_B0_000_201" },
	{ 7179, "ANH_MAIN_B0_000_202" },
	{ 7211, "ANH_MAIN_B0_000_210" },
	{ 7207, "ANH_MAIN_B0_000_220" },
	{ 7203, "ANH_MAIN_B0_000_300" },
	{ 7205, "ANH_MAIN_B0_000_310" },
	{ 7191, "ANH_MAIN_B0_000_400" },
	{ 7215, "ANH_MAIN_B0_000_401" },
	{ 7175, "ANH_MAIN_B0_000_402" },
	{ 7193, "ANH_MAIN_B0_000_410" },
	{ 7187, "ANH_MAIN_B0_000_420" },
	{ 7189, "ANH_MAIN_B0_000_430" },
	{ 7185, "ANH_MAIN_B0_000_500" },
	{ 7213, "ANH_MAIN_B0_000_501" },
	{ 138, "ANH_MAIN_B0_000_B" },
	{ 9187, "ANH_MAIN_B0_000_P" },
	{ 13816, "ANH_MAIN_B0_001_000" },
	{ 13836, "ANH_MAIN_B0_001_001" },
	{ 13788, "ANH_MAIN_B0_001_002" },
	{ 13818, "ANH_MAIN_B0_001_010" },
	{ 13838, "ANH_MAIN_B0_001_011" },
	{ 13814, "ANH_MAIN_B0_001_020" },
	{ 14216, "ANH_MAIN_B0_001_021" },
	{ 13784, "ANH_MAIN_B0_001_022" },
	{ 13794, "ANH_MAIN_B0_001_023" },
	{ 13786, "ANH_MAIN_B0_001_032" },
	{ 13796, "ANH_MAIN_B0_001_033" },
	{ 13810, "ANH_MAIN_B0_001_040" },
	{ 13812, "ANH_MAIN_B0_001_050" },
	{ 13808, "ANH_MAIN_B0_001_100" },
	{ 13834, "ANH_MAIN_B0_001_101" },
	{ 13782, "ANH_MAIN_B0_001_102" },
	{ 13792, "ANH_MAIN_B0_001_103" },
	{ 13842, "ANH_MAIN_B0_001_104" },
	{ 13844, "ANH_MAIN_B0_001_105" },
	{ 13826, "ANH_MAIN_B0_001_200" },
	{ 13840, "ANH_MAIN_B0_001_201" },
	{ 13790, "ANH_MAIN_B0_001_202" },
	{ 13828, "ANH_MAIN_B0_001_210" },
	{ 13824, "ANH_MAIN_B0_001_220" },
	{ 13820, "ANH_MAIN_B0_001_300" },
	{ 13822, "ANH_MAIN_B0_001_310" },
	{ 13804, "ANH_MAIN_B0_001_400" },
	{ 13832, "ANH_MAIN_B0_001_401" },
	{ 13780, "ANH_MAIN_B0_001_402" },
	{ 13806, "ANH_MAIN_B0_001_410" },
	{ 13800, "ANH_MAIN_B0_001_420" },
	{ 13802, "ANH_MAIN_B0_001_430" },
	{ 13798, "ANH_MAIN_B0_001_500" },
	{ 13830, "ANH_MAIN_B0_001_501" },
	{ 10289, "ANH_MAIN_B0_001_B" },
	{ 10291, "ANH_MAIN_B0_001_P" },
	{ 6101, "ANH_MAIN_B0_002_000" },
	{ 6121, "ANH_MAIN_B0_002_001" },
	{ 6135, "ANH_MAIN_B0_002_002" },
	{ 6103, "ANH_MAIN_B0_002_010" },
	{ 6123, "ANH_MAIN_B0_002_011" },
	{ 6099, "ANH_MAIN_B0_002_020" },
	{ 6119, "ANH_MAIN_B0_002_021" },
	{ 6071, "ANH_MAIN_B0_002_022" },
	{ 6079, "ANH_MAIN_B0_002_023" },
	{ 6073, "ANH_MAIN_B0_002_032" },
	{ 6081, "ANH_MAIN_B0_002_033" },
	{ 6095, "ANH_MAIN_B0_002_040" },
	{ 6097, "ANH_MAIN_B0_002_050" },
	{ 6093, "ANH_MAIN_B0_002_100" },
	{ 6117, "ANH_MAIN_B0_002_101" },
	{ 6069, "ANH_MAIN_B0_002_102" },
	{ 6077, "ANH_MAIN_B0_002_103" },
	{ 6127, "ANH_MAIN_B0_002_104" },
	{ 6129, "ANH_MAIN_B0_002_105" },
	{ 6109, "ANH_MAIN_B0_002_200" },
	{ 6125, "ANH_MAIN_B0_002_201" },
	{ 6075, "ANH_MAIN_B0_002_202" },
	{ 6111, "ANH_MAIN_B0_002_210" },
	{ 6105, "ANH_MAIN_B0_002_300" },
	{ 6107, "ANH_MAIN_B0_002_310" },
	{ 6089, "ANH_MAIN_B0_002_400" },
	{ 6115, "ANH_MAIN_B0_002_401" },
	{ 6067, "ANH_MAIN_B0_002_402" },
	{ 6091, "ANH_MAIN_B0_002_410" },
	{ 6085, "ANH_MAIN_B0_002_420" },
	{ 6087, "ANH_MAIN_B0_002_430" },
	{ 6083, "ANH_MAIN_B0_002_500" },
	{ 6113, "ANH_MAIN_B0_002_501" },
	{ 6080, "ANH_MAIN_B0_002_600" },
	{ 6082, "ANH_MAIN_B0_002_601" },
	{ 6084, "ANH_MAIN_B0_002_602" },
	{ 9600, "ANH_MAIN_B0_002_B" },
	{ 9185, "ANH_MAIN_B0_002_P" },
	{ 11490, "ANH_MAIN_B0_003_000" },
	{ 11500, "ANH_MAIN_B0_003_001" },
	{ 11470, "ANH_MAIN_B0_003_002" },
	{ 11492, "ANH_MAIN_B0_003_010" },
	{ 11502, "ANH_MAIN_B0_003_011" },
	{ 11488, "ANH_MAIN_B0_003_020" },
	{ 11498, "ANH_MAIN_B0_003_021" },
	{ 11466, "ANH_MAIN_B0_003_022" },
	{ 11478, "ANH_MAIN_B0_003_023" },
	{ 11468, "ANH_MAIN_B0_003_032" },
	{ 11480, "ANH_MAIN_B0_003_033" },
	{ 11484, "ANH_MAIN_B0_003_040" },
	{ 11486, "ANH_MAIN_B0_003_050" },
	{ 11482, "ANH_MAIN_B0_003_100" },
	{ 11494, "ANH_MAIN_B0_003_200" },
	{ 11504, "ANH_MAIN_B0_003_201" },
	{ 11474, "ANH_MAIN_B0_003_202" },
	{ 11565, "ANH_MAIN_B0_003_210" },
	{ 11560, "ANH_MAIN_B0_003_300" },
	{ 11562, "ANH_MAIN_B0_003_310" },
	{ 11556, "ANH_MAIN_B0_003_400" },
	{ 11574, "ANH_MAIN_B0_003_401" },
	{ 11522, "ANH_MAIN_B0_003_402" },
	{ 11558, "ANH_MAIN_B0_003_410" },
	{ 11552, "ANH_MAIN_B0_003_420" },
	{ 11554, "ANH_MAIN_B0_003_430" },
	{ 11550, "ANH_MAIN_B0_003_500" },
	{ 11572, "ANH_MAIN_B0_003_501" },
	{ 5998, "ANH_MAIN_B0_003_600" },
	{ 6000, "ANH_MAIN_B0_003_601" },
	{ 6001, "ANH_MAIN_B0_003_602" },
	{ 6004, "ANH_MAIN_B0_003_603" },
	{ 6006, "ANH_MAIN_B0_003_604" },
	{ 10197, "ANH_MAIN_B0_003_B" },
	{ 10199, "ANH_MAIN_B0_003_P" },
	{ 11170, "ANH_MAIN_B0_004_000" },
	{ 11190, "ANH_MAIN_B0_004_001" },
	{ 11142, "ANH_MAIN_B0_004_002" },
	{ 11172, "ANH_MAIN_B0_004_010" },
	{ 11192, "ANH_MAIN_B0_004_011" },
	{ 11168, "ANH_MAIN_B0_004_020" },
	{ 11188, "ANH_MAIN_B0_004_021" },
	{ 11138, "ANH_MAIN_B0_004_022" },
	{ 11148, "ANH_MAIN_B0_004_023" },
	{ 11140, "ANH_MAIN_B0_004_032" },
	{ 11150, "ANH_MAIN_B0_004_033" },
	{ 11164, "ANH_MAIN_B0_004_040" },
	{ 11166, "ANH_MAIN_B0_004_050" },
	{ 11162, "ANH_MAIN_B0_004_100" },
	{ 11186, "ANH_MAIN_B0_004_101" },
	{ 11136, "ANH_MAIN_B0_004_102" },
	{ 11146, "ANH_MAIN_B0_004_103" },
	{ 11196, "ANH_MAIN_B0_004_104" },
	{ 11198, "ANH_MAIN_B0_004_105" },
	{ 11178, "ANH_MAIN_B0_004_200" },
	{ 11194, "ANH_MAIN_B0_004_201" },
	{ 11144, "ANH_MAIN_B0_004_202" },
	{ 11180, "ANH_MAIN_B0_004_210" },
	{ 11174, "ANH_MAIN_B0_004_300" },
	{ 11176, "ANH_MAIN_B0_004_310" },
	{ 11158, "ANH_MAIN_B0_004_400" },
	{ 11184, "ANH_MAIN_B0_004_401" },
	{ 11134, "ANH_MAIN_B0_004_402" },
	{ 11160, "ANH_MAIN_B0_004_410" },
	{ 11154, "ANH_MAIN_B0_004_420" },
	{ 11156, "ANH_MAIN_B0_004_430" },
	{ 11152, "ANH_MAIN_B0_004_500" },
	{ 11182, "ANH_MAIN_B0_004_501" },
	{ 6086, "ANH_MAIN_B0_004_600" },
	{ 6088, "ANH_MAIN_B0_004_601" },
	{ 6090, "ANH_MAIN_B0_004_602" },
	{ 10205, "ANH_MAIN_B0_004_B" },
	{ 10207, "ANH_MAIN_B0_004_P" },
	{ 11634, "ANH_MAIN_B0_005_000" },
	{ 11654, "ANH_MAIN_B0_005_001" },
	{ 11604, "ANH_MAIN_B0_005_002" },
	{ 11636, "ANH_MAIN_B0_005_010" },
	{ 11656, "ANH_MAIN_B0_005_011" },
	{ 11632, "ANH_MAIN_B0_005_020" },
	{ 11652, "ANH_MAIN_B0_005_021" },
	{ 11600, "ANH_MAIN_B0_005_022" },
	{ 11612, "ANH_MAIN_B0_005_023" },
	{ 11602, "ANH_MAIN_B0_005_032" },
	{ 11614, "ANH_MAIN_B0_005_033" },
	{ 11628, "ANH_MAIN_B0_005_040" },
	{ 11630, "ANH_MAIN_B0_005_050" },
	{ 11626, "ANH_MAIN_B0_005_100" },
	{ 11642, "ANH_MAIN_B0_005_200" },
	{ 11658, "ANH_MAIN_B0_005_201" },
	{ 11608, "ANH_MAIN_B0_005_202" },
	{ 11644, "ANH_MAIN_B0_005_210" },
	{ 11638, "ANH_MAIN_B0_005_300" },
	{ 11640, "ANH_MAIN_B0_005_310" },
	{ 11622, "ANH_MAIN_B0_005_400" },
	{ 11648, "ANH_MAIN_B0_005_401" },
	{ 11596, "ANH_MAIN_B0_005_402" },
	{ 11624, "ANH_MAIN_B0_005_410" },
	{ 11618, "ANH_MAIN_B0_005_420" },
	{ 11620, "ANH_MAIN_B0_005_430" },
	{ 11616, "ANH_MAIN_B0_005_500" },
	{ 11646, "ANH_MAIN_B0_005_501" },
	{ 6074, "ANH_MAIN_B0_005_600" },
	{ 6076, "ANH_MAIN_B0_005_601" },
	{ 6078, "ANH_MAIN_B0_005_602" },
	{ 9196, "ANH_MAIN_B0_005_603" },
	{ 9197, "ANH_MAIN_B0_005_604" },
	{ 10201, "ANH_MAIN_B0_005_B" },
	{ 10203, "ANH_MAIN_B0_005_P" },
	{ 3036, "ANH_MAIN_B0_006_000" },
	{ 3042, "ANH_MAIN_B0_006_001" },
	{ 3022, "ANH_MAIN_B0_006_002" },
	{ 3011, "ANH_MAIN_B0_006_010" },
	{ 3021, "ANH_MAIN_B0_006_011" },
	{ 3034, "ANH_MAIN_B0_006_020" },
	{ 3018, "ANH_MAIN_B0_006_021" },
	{ 2998, "ANH_MAIN_B0_006_022" },
	{ 3002, "ANH_MAIN_B0_006_023" },
	{ 3000, "ANH_MAIN_B0_006_032" },
	{ 3004, "ANH_MAIN_B0_006_033" },
	{ 3030, "ANH_MAIN_B0_006_040" },
	{ 3032, "ANH_MAIN_B0_006_050" },
	{ 2995, "ANH_MAIN_B0_006_100" },
	{ 3289, "ANH_MAIN_B0_006_101" },
	{ 2989, "ANH_MAIN_B0_006_102" },
	{ 2991, "ANH_MAIN_B0_006_103" },
	{ 3291, "ANH_MAIN_B0_006_104" },
	{ 3293, "ANH_MAIN_B0_006_105" },
	{ 3038, "ANH_MAIN_B0_006_200" },
	{ 3044, "ANH_MAIN_B0_006_201" },
	{ 3024, "ANH_MAIN_B0_006_202" },
	{ 3040, "ANH_MAIN_B0_006_210" },
	{ 3285, "ANH_MAIN_B0_006_220" },
	{ 3012, "ANH_MAIN_B0_006_300" },
	{ 3014, "ANH_MAIN_B0_006_310" },
	{ 3008, "ANH_MAIN_B0_006_400" },
	{ 3016, "ANH_MAIN_B0_006_401" },
	{ 2996, "ANH_MAIN_B0_006_402" },
	{ 3029, "ANH_MAIN_B0_006_410" },
	{ 3006, "ANH_MAIN_B0_006_420" },
	{ 3027, "ANH_MAIN_B0_006_430" },
	{ 2993, "ANH_MAIN_B0_006_500" },
	{ 3287, "ANH_MAIN_B0_006_501" },
	{ 5645, "ANH_MAIN_B0_006_600" },
	{ 5646, "ANH_MAIN_B0_006_601" },
	{ 5647, "ANH_MAIN_B0_006_602" },
	{ 5649, "ANH_MAIN_B0_006_603" },
	{ 5651, "ANH_MAIN_B0_006_604" },
	{ 142, "ANH_MAIN_B0_006_B" },
	{ 9192, "ANH_MAIN_B0_006_P" },
	{ 3297, "ANH_MAIN_B0_007_000" },
	{ 4780, "ANH_MAIN_B0_007_001" },
	{ 5044, "ANH_MAIN_B0_007_002" },
	{ 4763, "ANH_MAIN_B0_007_010" },
	{ 3300, "ANH_MAIN_B0_007_011" },
	{ 4760, "ANH_MAIN_B0_007_020" },
	{ 4778, "ANH_MAIN_B0_007_021" },
	{ 3295, "ANH_MAIN_B0_007_022" },
	{ 4740, "ANH_MAIN_B0_007_023" },
	{ 4735, "ANH_MAIN_B0_007_032" },
	{ 4742, "ANH_MAIN_B0_007_033" },
	{ 4756, "ANH_MAIN_B0_007_040" },
	{ 4758, "ANH_MAIN_B0_007_050" },
	{ 4754, "ANH_MAIN_B0_007_100" },
	{ 4776, "ANH_MAIN_B0_007_101" },
	{ 4732, "ANH_MAIN_B0_007_102" },
	{ 4738, "ANH_MAIN_B0_007_103" },
	{ 4784, "ANH_MAIN_B0_007_104" },
	{ 4786, "ANH_MAIN_B0_007_105" },
	{ 4768, "ANH_MAIN_B0_007_200" },
	{ 4782, "ANH_MAIN_B0_007_201" },
	{ 4736, "ANH_MAIN_B0_007_202" },
	{ 4770, "ANH_MAIN_B0_007_210" },
	{ 49, "ANH_MAIN_B0_007_220" },
	{ 4764, "ANH_MAIN_B0_007_300" },
	{ 4766, "ANH_MAIN_B0_007_310" },
	{ 4750, "ANH_MAIN_B0_007_400" },
	{ 4774, "ANH_MAIN_B0_007_401" },
	{ 4730, "ANH_MAIN_B0_007_402" },
	{ 4752, "ANH_MAIN_B0_007_410" },
	{ 4746, "ANH_MAIN_B0_007_420" },
	{ 4748, "ANH_MAIN_B0_007_430" },
	{ 4744, "ANH_MAIN_B0_007_500" },
	{ 4772, "ANH_MAIN_B0_007_501" },
	{ 6690, "ANH_MAIN_B0_007_600" },
	{ 9601, "ANH_MAIN_B0_007_B" },
	{ 9190, "ANH_MAIN_B0_007_P" },
	{ 10646, "ANH_MAIN_B0_008_000" },
	{ 10668, "ANH_MAIN_B0_008_001" },
	{ 10616, "ANH_MAIN_B0_008_002" },
	{ 10648, "ANH_MAIN_B0_008_010" },
	{ 10670, "ANH_MAIN_B0_008_011" },
	{ 3658, "ANH_MAIN_B0_008_012" },
	{ 10644, "ANH_MAIN_B0_008_020" },
	{ 10666, "ANH_MAIN_B0_008_021" },
	{ 10612, "ANH_MAIN_B0_008_022" },
	{ 10624, "ANH_MAIN_B0_008_023" },
	{ 10614, "ANH_MAIN_B0_008_032" },
	{ 10626, "ANH_MAIN_B0_008_033" },
	{ 10640, "ANH_MAIN_B0_008_040" },
	{ 10642, "ANH_MAIN_B0_008_050" },
	{ 10638, "ANH_MAIN_B0_008_100" },
	{ 10664, "ANH_MAIN_B0_008_101" },
	{ 10610, "ANH_MAIN_B0_008_102" },
	{ 10622, "ANH_MAIN_B0_008_103" },
	{ 10674, "ANH_MAIN_B0_008_104" },
	{ 10676, "ANH_MAIN_B0_008_105" },
	{ 10656, "ANH_MAIN_B0_008_200" },
	{ 10672, "ANH_MAIN_B0_008_201" },
	{ 10620, "ANH_MAIN_B0_008_202" },
	{ 10658, "ANH_MAIN_B0_008_210" },
	{ 10654, "ANH_MAIN_B0_008_220" },
	{ 10650, "ANH_MAIN_B0_008_300" },
	{ 10652, "ANH_MAIN_B0_008_310" },
	{ 10634, "ANH_MAIN_B0_008_400" },
	{ 10662, "ANH_MAIN_B0_008_401" },
	{ 10608, "ANH_MAIN_B0_008_402" },
	{ 10636, "ANH_MAIN_B0_008_410" },
	{ 10630, "ANH_MAIN_B0_008_420" },
	{ 10632, "ANH_MAIN_B0_008_430" },
	{ 10628, "ANH_MAIN_B0_008_500" },
	{ 10660, "ANH_MAIN_B0_008_501" },
	{ 11006, "ANH_MAIN_B0_008_600" },
	{ 11008, "ANH_MAIN_B0_008_601" },
	{ 10325, "ANH_MAIN_B0_008_B" },
	{ 10327, "ANH_MAIN_B0_008_P" },
	{ 12781, "ANH_MAIN_B0_009_000" },
	{ 12799, "ANH_MAIN_B0_009_001" },
	{ 12755, "ANH_MAIN_B0_009_002" },
	{ 12783, "ANH_MAIN_B0_009_010" },
	{ 12801, "ANH_MAIN_B0_009_011" },
	{ 12993, "ANH_MAIN_B0_009_012" },
	{ 12779, "ANH_MAIN_B0_009_020" },
	{ 12797, "ANH_MAIN_B0_009_021" },
	{ 12751, "ANH_MAIN_B0_009_022" },
	{ 12761, "ANH_MAIN_B0_009_023" },
	{ 12753, "ANH_MAIN_B0_009_032" },
	{ 12763, "ANH_MAIN_B0_009_033" },
	{ 12775, "ANH_MAIN_B0_009_040" },
	{ 12777, "ANH_MAIN_B0_009_050" },
	{ 12773, "ANH_MAIN_B0_009_100" },
	{ 12795, "ANH_MAIN_B0_009_101" },
	{ 12749, "ANH_MAIN_B0_009_102" },
	{ 12759, "ANH_MAIN_B0_009_103" },
	{ 12805, "ANH_MAIN_B0_009_104" },
	{ 12807, "ANH_MAIN_B0_009_105" },
	{ 12787, "ANH_MAIN_B0_009_200" },
	{ 12803, "ANH_MAIN_B0_009_201" },
	{ 12757, "ANH_MAIN_B0_009_202" },
	{ 12789, "ANH_MAIN_B0_009_210" },
	{ 12935, "ANH_MAIN_B0_009_300" },
	{ 12786, "ANH_MAIN_B0_009_310" },
	{ 12769, "ANH_MAIN_B0_009_400" },
	{ 12793, "ANH_MAIN_B0_009_401" },
	{ 12747, "ANH_MAIN_B0_009_402" },
	{ 12771, "ANH_MAIN_B0_009_410" },
	{ 12767, "ANH_MAIN_B0_009_420" },
	{ 13005, "ANH_MAIN_B0_009_430" },
	{ 12765, "ANH_MAIN_B0_009_500" },
	{ 12791, "ANH_MAIN_B0_009_501" },
	{ 10209, "ANH_MAIN_B0_009_B" },
	{ 10211, "ANH_MAIN_B0_009_P" },
	{ 14324, "ANH_MAIN_B0_010_000" },
	{ 14344, "ANH_MAIN_B0_010_001" },
	{ 14296, "ANH_MAIN_B0_010_002" },
	{ 14326, "ANH_MAIN_B0_010_010" },
	{ 14346, "ANH_MAIN_B0_010_011" },
	{ 14322, "ANH_MAIN_B0_010_020" },
	{ 14342, "ANH_MAIN_B0_010_021" },
	{ 14292, "ANH_MAIN_B0_010_022" },
	{ 14302, "ANH_MAIN_B0_010_023" },
	{ 14294, "ANH_MAIN_B0_010_032" },
	{ 14304, "ANH_MAIN_B0_010_033" },
	{ 14318, "ANH_MAIN_B0_010_040" },
	{ 14320, "ANH_MAIN_B0_010_050" },
	{ 14316, "ANH_MAIN_B0_010_100" },
	{ 14332, "ANH_MAIN_B0_010_200" },
	{ 14348, "ANH_MAIN_B0_010_201" },
	{ 14298, "ANH_MAIN_B0_010_202" },
	{ 14334, "ANH_MAIN_B0_010_210" },
	{ 14328, "ANH_MAIN_B0_010_300" },
	{ 14330, "ANH_MAIN_B0_010_310" },
	{ 14312, "ANH_MAIN_B0_010_400" },
	{ 14338, "ANH_MAIN_B0_010_401" },
	{ 14288, "ANH_MAIN_B0_010_402" },
	{ 14314, "ANH_MAIN_B0_010_410" },
	{ 14308, "ANH_MAIN_B0_010_420" },
	{ 14310, "ANH_MAIN_B0_010_430" },
	{ 14306, "ANH_MAIN_B0_010_500" },
	{ 14336, "ANH_MAIN_B0_010_501" },
	{ 10213, "ANH_MAIN_B0_010_B" },
	{ 10215, "ANH_MAIN_B0_010_P" },
	{ 7449, "ANH_MAIN_B0_011_000" },
	{ 7469, "ANH_MAIN_B0_011_001" },
	{ 7421, "ANH_MAIN_B0_011_002" },
	{ 7451, "ANH_MAIN_B0_011_010" },
	{ 7471, "ANH_MAIN_B0_011_011" },
	{ 7447, "ANH_MAIN_B0_011_020" },
	{ 7467, "ANH_MAIN_B0_011_021" },
	{ 7417, "ANH_MAIN_B0_011_022" },
	{ 7427, "ANH_MAIN_B0_011_023" },
	{ 7419, "ANH_MAIN_B0_011_032" },
	{ 7429, "ANH_MAIN_B0_011_033" },
	{ 7443, "ANH_MAIN_B0_011_040" },
	{ 7445, "ANH_MAIN_B0_011_050" },
	{ 7441, "ANH_MAIN_B0_011_100" },
	{ 7465, "ANH_MAIN_B0_011_101" },
	{ 7415, "ANH_MAIN_B0_011_102" },
	{ 7425, "ANH_MAIN_B0_011_103" },
	{ 7475, "ANH_MAIN_B0_011_104" },
	{ 7477, "ANH_MAIN_B0_011_105" },
	{ 12191, "ANH_MAIN_B0_011_110" },
	{ 12193, "ANH_MAIN_B0_011_111" },
	{ 12187, "ANH_MAIN_B0_011_112" },
	{ 12189, "ANH_MAIN_B0_011_113" },
	{ 7457, "ANH_MAIN_B0_011_200" },
	{ 7473, "ANH_MAIN_B0_011_201" },
	{ 7423, "ANH_MAIN_B0_011_202" },
	{ 7459, "ANH_MAIN_B0_011_210" },
	{ 7453, "ANH_MAIN_B0_011_300" },
	{ 7455, "ANH_MAIN_B0_011_310" },
	{ 7437, "ANH_MAIN_B0_011_400" },
	{ 7463, "ANH_MAIN_B0_011_401" },
	{ 7413, "ANH_MAIN_B0_011_402" },
	{ 7439, "ANH_MAIN_B0_011_410" },
	{ 7433, "ANH_MAIN_B0_011_420" },
	{ 7435, "ANH_MAIN_B0_011_430" },
	{ 7431, "ANH_MAIN_B0_011_500" },
	{ 7461, "ANH_MAIN_B0_011_501" },
	{ 2312, "ANH_MAIN_B0_011_600" },
	{ 6738, "ANH_MAIN_B0_011_601" },
	{ 9602, "ANH_MAIN_B0_011_B" },
	{ 9195, "ANH_MAIN_B0_011_P" },
	{ 14254, "ANH_MAIN_B0_012_000" },
	{ 14276, "ANH_MAIN_B0_012_001" },
	{ 14226, "ANH_MAIN_B0_012_002" },
	{ 14256, "ANH_MAIN_B0_012_010" },
	{ 14278, "ANH_MAIN_B0_012_011" },
	{ 14252, "ANH_MAIN_B0_012_020" },
	{ 14274, "ANH_MAIN_B0_012_021" },
	{ 14222, "ANH_MAIN_B0_012_022" },
	{ 14232, "ANH_MAIN_B0_012_023" },
	{ 14224, "ANH_MAIN_B0_012_032" },
	{ 14234, "ANH_MAIN_B0_012_033" },
	{ 14248, "ANH_MAIN_B0_012_040" },
	{ 14250, "ANH_MAIN_B0_012_050" },
	{ 14246, "ANH_MAIN_B0_012_100" },
	{ 14272, "ANH_MAIN_B0_012_101" },
	{ 14220, "ANH_MAIN_B0_012_102" },
	{ 14230, "ANH_MAIN_B0_012_103" },
	{ 14282, "ANH_MAIN_B0_012_104" },
	{ 14284, "ANH_MAIN_B0_012_105" },
	{ 14264, "ANH_MAIN_B0_012_200" },
	{ 14280, "ANH_MAIN_B0_012_201" },
	{ 14228, "ANH_MAIN_B0_012_202" },
	{ 14266, "ANH_MAIN_B0_012_210" },
	{ 14262, "ANH_MAIN_B0_012_220" },
	{ 14258, "ANH_MAIN_B0_012_300" },
	{ 14260, "ANH_MAIN_B0_012_310" },
	{ 14242, "ANH_MAIN_B0_012_400" },
	{ 14270, "ANH_MAIN_B0_012_401" },
	{ 14218, "ANH_MAIN_B0_012_402" },
	{ 14244, "ANH_MAIN_B0_012_410" },
	{ 14238, "ANH_MAIN_B0_012_420" },
	{ 14240, "ANH_MAIN_B0_012_430" },
	{ 14236, "ANH_MAIN_B0_012_500" },
	{ 14268, "ANH_MAIN_B0_012_501" },
	{ 10714, "ANH_MAIN_B0_012_600" },
	{ 10717, "ANH_MAIN_B0_012_601" },
	{ 10719, "ANH_MAIN_B0_012_602" },
	{ 10721, "ANH_MAIN_B0_012_603" },
	{ 10722, "ANH_MAIN_B0_012_604" },
	{ 10725, "ANH_MAIN_B0_012_605" },
	{ 10221, "ANH_MAIN_B0_012_B" },
	{ 10223, "ANH_MAIN_B0_012_P" },
	{ 5082, "ANH_MAIN_B0_013_000" },
	{ 5383, "ANH_MAIN_B0_013_001" },
	{ 2368, "ANH_MAIN_B0_013_002" },
	{ 5084, "ANH_MAIN_B0_013_010" },
	{ 5385, "ANH_MAIN_B0_013_011" },
	{ 5080, "ANH_MAIN_B0_013_020" },
	{ 5381, "ANH_MAIN_B0_013_021" },
	{ 2344, "ANH_MAIN_B0_013_022" },
	{ 3400, "ANH_MAIN_B0_013_023" },
	{ 2366, "ANH_MAIN_B0_013_032" },
	{ 3402, "ANH_MAIN_B0_013_033" },
	{ 5065, "ANH_MAIN_B0_013_040" },
	{ 5067, "ANH_MAIN_B0_013_050" },
	{ 5063, "ANH_MAIN_B0_013_100" },
	{ 5379, "ANH_MAIN_B0_013_101" },
	{ 2342, "ANH_MAIN_B0_013_102" },
	{ 3398, "ANH_MAIN_B0_013_103" },
	{ 5394, "ANH_MAIN_B0_013_104" },
	{ 5396, "ANH_MAIN_B0_013_105" },
	{ 5096, "ANH_MAIN_B0_013_200" },
	{ 5387, "ANH_MAIN_B0_013_201" },
	{ 2797, "ANH_MAIN_B0_013_202" },
	{ 5147, "ANH_MAIN_B0_013_210" },
	{ 5092, "ANH_MAIN_B0_013_220" },
	{ 5094, "ANH_MAIN_B0_013_230" },
	{ 5086, "ANH_MAIN_B0_013_300" },
	{ 5088, "ANH_MAIN_B0_013_310" },
	{ 5059, "ANH_MAIN_B0_013_400" },
	{ 5376, "ANH_MAIN_B0_013_401" },
	{ 2228, "ANH_MAIN_B0_013_402" },
	{ 5061, "ANH_MAIN_B0_013_410" },
	{ 3406, "ANH_MAIN_B0_013_420" },
	{ 4989, "ANH_MAIN_B0_013_430" },
	{ 3404, "ANH_MAIN_B0_013_500" },
	{ 5149, "ANH_MAIN_B0_013_501" },
	{ 12408, "ANH_MAIN_B0_013_B" },
	{ 12410, "ANH_MAIN_B0_013_P" },
	{ 5544, "ANH_MAIN_B0_014_000" },
	{ 5566, "ANH_MAIN_B0_014_001" },
	{ 5514, "ANH_MAIN_B0_014_002" },
	{ 5546, "ANH_MAIN_B0_014_010" },
	{ 5568, "ANH_MAIN_B0_014_011" },
	{ 3668, "ANH_MAIN_B0_014_012" },
	{ 5542, "ANH_MAIN_B0_014_020" },
	{ 5564, "ANH_MAIN_B0_014_021" },
	{ 5510, "ANH_MAIN_B0_014_022" },
	{ 5522, "ANH_MAIN_B0_014_023" },
	{ 5512, "ANH_MAIN_B0_014_032" },
	{ 5524, "ANH_MAIN_B0_014_033" },
	{ 5538, "ANH_MAIN_B0_014_040" },
	{ 5540, "ANH_MAIN_B0_014_050" },
	{ 5536, "ANH_MAIN_B0_014_100" },
	{ 5562, "ANH_MAIN_B0_014_101" },
	{ 5508, "ANH_MAIN_B0_014_102" },
	{ 5520, "ANH_MAIN_B0_014_103" },
	{ 5572, "ANH_MAIN_B0_014_104" },
	{ 5574, "ANH_MAIN_B0_014_105" },
	{ 5554, "ANH_MAIN_B0_014_200" },
	{ 5570, "ANH_MAIN_B0_014_201" },
	{ 5518, "ANH_MAIN_B0_014_202" },
	{ 5556, "ANH_MAIN_B0_014_210" },
	{ 5552, "ANH_MAIN_B0_014_220" },
	{ 5548, "ANH_MAIN_B0_014_300" },
	{ 5550, "ANH_MAIN_B0_014_310" },
	{ 5532, "ANH_MAIN_B0_014_400" },
	{ 5560, "ANH_MAIN_B0_014_401" },
	{ 5506, "ANH_MAIN_B0_014_402" },
	{ 5534, "ANH_MAIN_B0_014_410" },
	{ 5528, "ANH_MAIN_B0_014_420" },
	{ 5530, "ANH_MAIN_B0_014_430" },
	{ 5526, "ANH_MAIN_B0_014_500" },
	{ 5558, "ANH_MAIN_B0_014_501" },
	{ 9603, "ANH_MAIN_B0_014_B" },
	{ 9198, "ANH_MAIN_B0_014_P" },
	{ 5466, "ANH_MAIN_B0_015_000" },
	{ 5488, "ANH_MAIN_B0_015_001" },
	{ 5432, "ANH_MAIN_B0_015_002" },
	{ 5468, "ANH_MAIN_B0_015_010" },
	{ 5490, "ANH_MAIN_B0_015_011" },
	{ 5464, "ANH_MAIN_B0_015_020" },
	{ 5486, "ANH_MAIN_B0_015_021" },
	{ 5428, "ANH_MAIN_B0_015_022" },
	{ 5441, "ANH_MAIN_B0_015_023" },
	{ 5430, "ANH_MAIN_B0_015_032" },
	{ 5443, "ANH_MAIN_B0_015_033" },
	{ 5460, "ANH_MAIN_B0_015_040" },
	{ 5462, "ANH_MAIN_B0_015_050" },
	{ 5458, "ANH_MAIN_B0_015_100" },
	{ 5484, "ANH_MAIN_B0_015_101" },
	{ 5426, "ANH_MAIN_B0_015_102" },
	{ 5439, "ANH_MAIN_B0_015_103" },
	{ 5501, "ANH_MAIN_B0_015_104" },
	{ 5504, "ANH_MAIN_B0_015_105" },
	{ 5476, "ANH_MAIN_B0_015_200" },
	{ 5492, "ANH_MAIN_B0_015_201" },
	{ 5436, "ANH_MAIN_B0_015_202" },
	{ 5478, "ANH_MAIN_B0_015_210" },
	{ 5474, "ANH_MAIN_B0_015_220" },
	{ 5470, "ANH_MAIN_B0_015_300" },
	{ 5472, "ANH_MAIN_B0_015_310" },
	{ 5454, "ANH_MAIN_B0_015_400" },
	{ 5482, "ANH_MAIN_B0_015_401" },
	{ 5424, "ANH_MAIN_B0_015_402" },
	{ 5456, "ANH_MAIN_B0_015_410" },
	{ 5450, "ANH_MAIN_B0_015_420" },
	{ 5452, "ANH_MAIN_B0_015_430" },
	{ 5448, "ANH_MAIN_B0_015_500" },
	{ 5480, "ANH_MAIN_B0_015_501" },
	{ 5446, "ANH_MAIN_B0_015_B" },
	{ 5495, "ANH_MAIN_B0_015_P" },
	{ 12094, "ANH_MAIN_B0_016_000" },
	{ 12116, "ANH_MAIN_B0_016_001" },
	{ 12064, "ANH_MAIN_B0_016_002" },
	{ 12096, "ANH_MAIN_B0_016_010" },
	{ 12118, "ANH_MAIN_B0_016_011" },
	{ 12092, "ANH_MAIN_B0_016_020" },
	{ 12114, "ANH_MAIN_B0_016_021" },
	{ 12060, "ANH_MAIN_B0_016_022" },
	{ 12070, "ANH_MAIN_B0_016_023" },
	{ 12062, "ANH_MAIN_B0_016_032" },
	{ 12072, "ANH_MAIN_B0_016_033" },
	{ 12088, "ANH_MAIN_B0_016_040" },
	{ 12090, "ANH_MAIN_B0_016_050" },
	{ 12086, "ANH_MAIN_B0_016_100" },
	{ 12112, "ANH_MAIN_B0_016_101" },
	{ 12058, "ANH_MAIN_B0_016_102" },
	{ 12068, "ANH_MAIN_B0_016_103" },
	{ 12124, "ANH_MAIN_B0_016_104" },
	{ 12126, "ANH_MAIN_B0_016_105" },
	{ 12104, "ANH_MAIN_B0_016_200" },
	{ 12120, "ANH_MAIN_B0_016_201" },
	{ 12066, "ANH_MAIN_B0_016_202" },
	{ 12106, "ANH_MAIN_B0_016_210" },
	{ 12102, "ANH_MAIN_B0_016_220" },
	{ 12098, "ANH_MAIN_B0_016_300" },
	{ 12100, "ANH_MAIN_B0_016_310" },
	{ 12082, "ANH_MAIN_B0_016_400" },
	{ 12110, "ANH_MAIN_B0_016_401" },
	{ 12056, "ANH_MAIN_B0_016_402" },
	{ 12084, "ANH_MAIN_B0_016_410" },
	{ 12078, "ANH_MAIN_B0_016_420" },
	{ 12080, "ANH_MAIN_B0_016_430" },
	{ 12076, "ANH_MAIN_B0_016_500" },
	{ 12108, "ANH_MAIN_B0_016_501" },
	{ 12075, "ANH_MAIN_B0_016_B" },
	{ 12123, "ANH_MAIN_B0_016_P" },
	{ 12969, "ANH_MAIN_B0_017_000" },
	{ 12981, "ANH_MAIN_B0_017_001" },
	{ 12951, "ANH_MAIN_B0_017_002" },
	{ 12971, "ANH_MAIN_B0_017_010" },
	{ 12983, "ANH_MAIN_B0_017_011" },
	{ 12967, "ANH_MAIN_B0_017_020" },
	{ 12979, "ANH_MAIN_B0_017_021" },
	{ 12949, "ANH_MAIN_B0_017_022" },
	{ 13034, "ANH_MAIN_B0_017_023" },
	{ 13031, "ANH_MAIN_B0_017_032" },
	{ 13036, "ANH_MAIN_B0_017_033" },
	{ 13038, "ANH_MAIN_B0_017_040" },
	{ 13040, "ANH_MAIN_B0_017_050" },
	{ 12965, "ANH_MAIN_B0_017_100" },
	{ 12977, "ANH_MAIN_B0_017_101" },
	{ 12947, "ANH_MAIN_B0_017_102" },
	{ 12953, "ANH_MAIN_B0_017_103" },
	{ 12985, "ANH_MAIN_B0_017_104" },
	{ 12987, "ANH_MAIN_B0_017_105" },
	{ 13042, "ANH_MAIN_B0_017_200" },
	{ 13046, "ANH_MAIN_B0_017_201" },
	{ 13032, "ANH_MAIN_B0_017_202" },
	{ 13044, "ANH_MAIN_B0_017_210" },
	{ 13060, "ANH_MAIN_B0_017_300" },
	{ 13062, "ANH_MAIN_B0_017_310" },
	{ 12961, "ANH_MAIN_B0_017_400" },
	{ 12975, "ANH_MAIN_B0_017_401" },
	{ 12945, "ANH_MAIN_B0_017_402" },
	{ 12963, "ANH_MAIN_B0_017_410" },
	{ 12957, "ANH_MAIN_B0_017_420" },
	{ 12959, "ANH_MAIN_B0_017_430" },
	{ 12955, "ANH_MAIN_B0_017_500" },
	{ 12973, "ANH_MAIN_B0_017_501" },
	{ 11677, "ANH_MAIN_B0_017_B" },
	{ 11679, "ANH_MAIN_B0_017_P" },
	{ 6744, "ANH_MAIN_B0_018_000" },
	{ 6746, "ANH_MAIN_B0_018_001" },
	{ 6747, "ANH_MAIN_B0_018_002" },
	{ 6750, "ANH_MAIN_B0_018_010" },
	{ 6752, "ANH_MAIN_B0_018_011" },
	{ 6753, "ANH_MAIN_B0_018_020" },
	{ 6755, "ANH_MAIN_B0_018_021" },
	{ 6758, "ANH_MAIN_B0_018_022" },
	{ 6759, "ANH_MAIN_B0_018_023" },
	{ 6761, "ANH_MAIN_B0_018_032" },
	{ 6763, "ANH_MAIN_B0_018_033" },
	{ 6766, "ANH_MAIN_B0_018_040" },
	{ 6768, "ANH_MAIN_B0_018_050" },
	{ 6769, "ANH_MAIN_B0_018_100" },
	{ 6771, "ANH_MAIN_B0_018_101" },
	{ 6774, "ANH_MAIN_B0_018_102" },
	{ 6776, "ANH_MAIN_B0_018_103" },
	{ 6777, "ANH_MAIN_B0_018_104" },
	{ 6779, "ANH_MAIN_B0_018_105" },
	{ 6782, "ANH_MAIN_B0_018_200" },
	{ 6784, "ANH_MAIN_B0_018_201" },
	{ 6785, "ANH_MAIN_B0_018_202" },
	{ 6787, "ANH_MAIN_B0_018_210" },
	{ 6789, "ANH_MAIN_B0_018_220" },
	{ 6791, "ANH_MAIN_B0_018_300" },
	{ 6794, "ANH_MAIN_B0_018_310" },
	{ 6795, "ANH_MAIN_B0_018_400" },
	{ 6797, "ANH_MAIN_B0_018_401" },
	{ 6799, "ANH_MAIN_B0_018_402" },
	{ 6802, "ANH_MAIN_B0_018_410" },
	{ 6803, "ANH_MAIN_B0_018_420" },
	{ 6805, "ANH_MAIN_B0_018_430" },
	{ 6807, "ANH_MAIN_B0_018_500" },
	{ 6809, "ANH_MAIN_B0_018_501" },
	{ 6812, "ANH_MAIN_B0_018_600" },
	{ 6740, "ANH_MAIN_B0_018_B" },
	{ 6742, "ANH_MAIN_B0_018_P" },
	{ 12161, "ANH_MAIN_B0_022_B" },
	{ 12163, "ANH_MAIN_B0_022_P" },
	{ 12157, "ANH_MAIN_B0_023_B" },
	{ 12159, "ANH_MAIN_B0_023_P" },
	{ 11975, "ANH_MAIN_B0_024_B" },
	{ 11977, "ANH_MAIN_B0_024_P" },
	{ 11971, "ANH_MAIN_B0_025_B" },
	{ 11973, "ANH_MAIN_B0_025_P" },
	{ 11967, "ANH_MAIN_B0_026_B" },
	{ 11969, "ANH_MAIN_B0_026_P" },
	{ 11963, "ANH_MAIN_B0_027_B" },
	{ 11965, "ANH_MAIN_B0_027_P" },
	{ 12169, "ANH_MAIN_B0_028_B" },
	{ 12171, "ANH_MAIN_B0_028_P" },
	{ 12165, "ANH_MAIN_B0_029_B" },
	{ 12167, "ANH_MAIN_B0_029_P" },
	{ 12173, "ANH_MAIN_B0_030_B" },
	{ 12175, "ANH_MAIN_B0_030_P" },
	{ 14211, "ANH_MAIN_B0_031_B" },
	{ 14213, "ANH_MAIN_B0_031_P" },
	{ 14207, "ANH_MAIN_B0_032_B" },
	{ 14209, "ANH_MAIN_B0_032_P" },
	{ 13670, "ANH_MAIN_B0_033_B" },
	{ 13672, "ANH_MAIN_B0_033_P" },
	{ 14397, "ANH_MAIN_B0_034_B" },
	{ 14399, "ANH_MAIN_B0_034_P" },
	{ 839, "ANH_MAIN_B2_001" },
	{ 886, "ANH_MAIN_B2_001_000" },
	{ 890, "ANH_MAIN_B2_001_000_20" },
	{ 887, "ANH_MAIN_B2_001_001" },
	{ 836, "ANH_MAIN_B2_001_002" },
	{ 9605, "ANH_MAIN_B2_001_B" },
	{ 9204, "ANH_MAIN_B2_001_P" },
	{ 10000, "ANH_MAIN_B2_002_B" },
	{ 11927, "ANH_MAIN_B2_002_CUT_0" },
	{ 10002, "ANH_MAIN_B2_002_P" },
	{ 12511, "ANH_MAIN_B2_003_ALL" },
	{ 8052, "ANH_MAIN_B2_003_B" },
	{ 14400, "ANH_MAIN_B2_003_CUT_1" },
	{ 14404, "ANH_MAIN_B2_003_CUT_2" },
	{ 14402, "ANH_MAIN_B2_003_CUT_3" },
	{ 5559, "ANH_MAIN_B2_003_DOWN" },
	{ 4047, "ANH_MAIN_B2_003_IDLE" },
	{ 9765, "ANH_MAIN_B2_003_P" },
	{ 6514, "ANH_MAIN_B2_005_ACT1" },
	{ 6505, "ANH_MAIN_B2_005_ACT_A" },
	{ 6511, "ANH_MAIN_B2_005_ACT_B" },
	{ 6509, "ANH_MAIN_B2_005_ACT_C" },
	{ 6508, "ANH_MAIN_B2_005_IDLE" },
	{ 6515, "ANH_MAIN_B2_005_IDLE2ACT1" },
	{ 12412, "ANH_MAIN_B2_007_B" },
	{ 8357, "ANH_MAIN_B2_007_CUT_1" },
	{ 8363, "ANH_MAIN_B2_007_CUT_2" },
	{ 8361, "ANH_MAIN_B2_007_CUT_3" },
	{ 8367, "ANH_MAIN_B2_007_CUT_4" },
	{ 8365, "ANH_MAIN_B2_007_CUT_5" },
	{ 8371, "ANH_MAIN_B2_007_CUT_6" },
	{ 8369, "ANH_MAIN_B2_007_CUT_7" },
	{ 8373, "ANH_MAIN_B2_007_CUT_8" },
	{ 12414, "ANH_MAIN_B2_007_P" },
	{ 8359, "ANH_MAIN_B2_007_POSE" },
	{ 9606, "ANH_MAIN_B2_009_B" },
	{ 6160, "ANH_MAIN_B2_009_CUT3" },
	{ 11005, "ANH_MAIN_B2_009_CUT3_2" },
	{ 10137, "ANH_MAIN_B2_009_CUT4_1" },
	{ 10139, "ANH_MAIN_B2_009_CUT4_2" },
	{ 10141, "ANH_MAIN_B2_009_CUT5" },
	{ 11225, "ANH_MAIN_B2_009_CUT5_1" },
	{ 11226, "ANH_MAIN_B2_009_CUT5_2" },
	{ 9207, "ANH_MAIN_B2_009_P" },
	{ 14448, "ANH_MAIN_B2_011_1ST" },
	{ 14449, "ANH_MAIN_B2_011_2ND" },
	{ 9773, "ANH_MAIN_B2_011_B" },
	{ 8668, "ANH_MAIN_B2_011_CUT_1_JUMP" },
	{ 8670, "ANH_MAIN_B2_011_CUT_1_MOON" },
	{ 8671, "ANH_MAIN_B2_011_CUT_1_RUN" },
	{ 8673, "ANH_MAIN_B2_011_CUT_2" },
	{ 8674, "ANH_MAIN_B2_011_CUT_3" },
	{ 8676, "ANH_MAIN_B2_011_CUT_4" },
	{ 8677, "ANH_MAIN_B2_011_CUT_5" },
	{ 14443, "ANH_MAIN_B2_011_DOWN" },
	{ 14445, "ANH_MAIN_B2_011_END" },
	{ 14439, "ANH_MAIN_B2_011_JUMP" },
	{ 14441, "ANH_MAIN_B2_011_OPENING" },
	{ 9775, "ANH_MAIN_B2_011_P" },
	{ 14438, "ANH_MAIN_B2_011_RUN" },
	{ 9607, "ANH_MAIN_B2_012_B" },
	{ 8299, "ANH_MAIN_B2_012_CROSS" },
	{ 8306, "ANH_MAIN_B2_012_IDLE" },
	{ 8301, "ANH_MAIN_B2_012_OPEN" },
	{ 9210, "ANH_MAIN_B2_012_P" },
	{ 8303, "ANH_MAIN_B2_012_REACH" },
	{ 8036, "ANH_MAIN_B2_014_B" },
	{ 8694, "ANH_MAIN_B2_014_CUT_1" },
	{ 8695, "ANH_MAIN_B2_014_CUT_2_1" },
	{ 8697, "ANH_MAIN_B2_014_CUT_2_2" },
	{ 8698, "ANH_MAIN_B2_014_CUT_2_3" },
	{ 8700, "ANH_MAIN_B2_014_CUT_2_4" },
	{ 8701, "ANH_MAIN_B2_014_CUT_3" },
	{ 8703, "ANH_MAIN_B2_014_CUT_4" },
	{ 8704, "ANH_MAIN_B2_014_CUT_5" },
	{ 8706, "ANH_MAIN_B2_014_CUT_6" },
	{ 8707, "ANH_MAIN_B2_014_CUT_7" },
	{ 11349, "ANH_MAIN_B2_014_CUT_8" },
	{ 11357, "ANH_MAIN_B2_014_CUT_9" },
	{ 9767, "ANH_MAIN_B2_014_P" },
	{ 8709, "ANH_MAIN_B2_014_TEST" },
	{ 9609, "ANH_MAIN_B2_016_B" },
	{ 6245, "ANH_MAIN_B2_016_CUT_1" },
	{ 6250, "ANH_MAIN_B2_016_CUT_3" },
	{ 11835, "ANH_MAIN_B2_016_CUT_4" },
	{ 11836, "ANH_MAIN_B2_016_IDLE" },
	{ 9216, "ANH_MAIN_B2_016_P" },
	{ 7115, "ANH_MAIN_B2_016_V1_CUT2" },
	{ 7679, "ANH_MAIN_B2_016_V2_CUT1" },
	{ 9608, "ANH_MAIN_B2_017_B" },
	{ 3408, "ANH_MAIN_B2_017_CUT2" },
	{ 9213, "ANH_MAIN_B2_017_P" },
	{ 9611, "ANH_MAIN_B2_018_B" },
	{ 2780, "ANH_MAIN_B2_018_CUT1" },
	{ 2782, "ANH_MAIN_B2_018_CUT2" },
	{ 5170, "ANH_MAIN_B2_018_FLOATING" },
	{ 5172, "ANH_MAIN_B2_018_FLOATING_END" },
	{ 5175, "ANH_MAIN_B2_018_LANDING" },
	{ 5179, "ANH_MAIN_B2_018_MOT1" },
	{ 11237, "ANH_MAIN_B2_018_MOT2" },
	{ 9222, "ANH_MAIN_B2_018_P" },
	{ 5176, "ANH_MAIN_B2_018_ROT" },
	{ 5177, "ANH_MAIN_B2_018_SAVING" },
	{ 5178, "ANH_MAIN_B2_018_STICK_OUT" },
	{ 9610, "ANH_MAIN_B2_019_B" },
	{ 7519, "ANH_MAIN_B2_019_CUT_1" },
	{ 7521, "ANH_MAIN_B2_019_CUT_2" },
	{ 8022, "ANH_MAIN_B2_019_CUT_2_1" },
	{ 8024, "ANH_MAIN_B2_019_CUT_2_2" },
	{ 8838, "ANH_MAIN_B2_019_IDLE" },
	{ 13184, "ANH_MAIN_B2_019_IDLE_2" },
	{ 9219, "ANH_MAIN_B2_019_P" },
	{ 7089, "ANH_MAIN_B2_019_WIND_1" },
	{ 7085, "ANH_MAIN_B2_019_WIND_2" },
	{ 7087, "ANH_MAIN_B2_019_WIND_3" },
	{ 9755, "ANH_MAIN_B2_020_B" },
	{ 12043, "ANH_MAIN_B2_020_MOT1" },
	{ 9757, "ANH_MAIN_B2_020_P" },
	{ 9751, "ANH_MAIN_B2_021_B" },
	{ 12041, "ANH_MAIN_B2_021_MOT1" },
	{ 9753, "ANH_MAIN_B2_021_P" },
	{ 9747, "ANH_MAIN_B2_022_B" },
	{ 11976, "ANH_MAIN_B2_022_MOT1" },
	{ 9749, "ANH_MAIN_B2_022_P" },
	{ 12982, "ANH_MAIN_B2_023_B" },
	{ 12986, "ANH_MAIN_B2_023_P" },
	{ 12984, "ANH_MAIN_B2_024_B" },
	{ 12988, "ANH_MAIN_B2_024_P" },
	{ 14527, "ANH_MAIN_B2_026_B" },
	{ 14529, "ANH_MAIN_B2_026_P" },
	{ 8038, "ANH_MAIN_B2_027_B" },
	{ 9759, "ANH_MAIN_B2_027_P" },
	{ 9761, "ANH_MAIN_B2_028_B" },
	{ 9763, "ANH_MAIN_B2_028_P" },
	{ 14461, "ANH_MAIN_B2_CAM_011_1ST" },
	{ 14460, "ANH_MAIN_B2_CAM_011_2ND" },
	{ 14463, "ANH_MAIN_B2_CAM_011_DOWN" },
	{ 14458, "ANH_MAIN_B2_CAM_011_END" },
	{ 14451, "ANH_MAIN_B2_CAM_011_JUMP" },
	{ 14454, "ANH_MAIN_B2_CAM_011_OPENING" },
	{ 14455, "ANH_MAIN_B2_CAM_011_RUN" },
	{ 6712, "ANH_MAIN_B4_002_B" },
	{ 8627, "ANH_MAIN_B4_002_DETH" },
	{ 10005, "ANH_MAIN_B4_002_DETH_0" },
	{ 6714, "ANH_MAIN_B4_002_P" },
	{ 6516, "ANH_MAIN_B4_003_B" },
	{ 6519, "ANH_MAIN_B4_003_P" },
	{ 6715, "ANH_MAIN_B4_004_B" },
	{ 6717, "ANH_MAIN_B4_004_P" },
	{ 6720, "ANH_MAIN_B4_005_B" },
	{ 6721, "ANH_MAIN_B4_005_P" },
	{ 6912, "ANH_MAIN_B4_006_B" },
	{ 6914, "ANH_MAIN_B4_006_P" },
	{ 7218, "ANH_MAIN_B4_007_B" },
	{ 7220, "ANH_MAIN_B4_007_P" },
	{ 11962, "ANH_MON_B3_000_000" },
	{ 2997, "ANH_MON_B3_000_B" },
	{ 5306, "ANH_MON_B3_000_P" },
	{ 3485, "ANH_MON_B3_001_000" },
	{ 3469, "ANH_MON_B3_001_002" },
	{ 3477, "ANH_MON_B3_001_003" },
	{ 3497, "ANH_MON_B3_001_007" },
	{ 3487, "ANH_MON_B3_001_010" },
	{ 3496, "ANH_MON_B3_001_011" },
	{ 3471, "ANH_MON_B3_001_012" },
	{ 3479, "ANH_MON_B3_001_013" },
	{ 3500, "ANH_MON_B3_001_014" },
	{ 3481, "ANH_MON_B3_001_020" },
	{ 3491, "ANH_MON_B3_001_021" },
	{ 3465, "ANH_MON_B3_001_022" },
	{ 3483, "ANH_MON_B3_001_030" },
	{ 3493, "ANH_MON_B3_001_031" },
	{ 3467, "ANH_MON_B3_001_032" },
	{ 3476, "ANH_MON_B3_001_033" },
	{ 9297, "ANH_MON_B3_001_B" },
	{ 3490, "ANH_MON_B3_001_ILLUST" },
	{ 8702, "ANH_MON_B3_001_P" },
	{ 3473, "ANH_MON_B3_001_TEST" },
	{ 1737, "ANH_MON_B3_002_000" },
	{ 1725, "ANH_MON_B3_002_002" },
	{ 1729, "ANH_MON_B3_002_003" },
	{ 3533, "ANH_MON_B3_002_007" },
	{ 1739, "ANH_MON_B3_002_010" },
	{ 1746, "ANH_MON_B3_002_011" },
	{ 1727, "ANH_MON_B3_002_012" },
	{ 1731, "ANH_MON_B3_002_013" },
	{ 1748, "ANH_MON_B3_002_014" },
	{ 1733, "ANH_MON_B3_002_020" },
	{ 1741, "ANH_MON_B3_002_021" },
	{ 1721, "ANH_MON_B3_002_022" },
	{ 1735, "ANH_MON_B3_002_030" },
	{ 1743, "ANH_MON_B3_002_031" },
	{ 1723, "ANH_MON_B3_002_032" },
	{ 12478, "ANH_MON_B3_002_B" },
	{ 3532, "ANH_MON_B3_002_ILLUST" },
	{ 12480, "ANH_MON_B3_002_P" },
	{ 3535, "ANH_MON_B3_002_TEST" },
	{ 1439, "ANH_MON_B3_003_000" },
	{ 1432, "ANH_MON_B3_003_002" },
	{ 1435, "ANH_MON_B3_003_003" },
	{ 2830, "ANH_MON_B3_003_007" },
	{ 1437, "ANH_MON_B3_003_020" },
	{ 1441, "ANH_MON_B3_003_021" },
	{ 1430, "ANH_MON_B3_003_022" },
	{ 1438, "ANH_MON_B3_003_030" },
	{ 1442, "ANH_MON_B3_003_031" },
	{ 1431, "ANH_MON_B3_003_032" },
	{ 1434, "ANH_MON_B3_003_033" },
	{ 1444, "ANH_MON_B3_003_034" },
	{ 1436, "ANH_MON_B3_003_040" },
	{ 1440, "ANH_MON_B3_003_041" },
	{ 1429, "ANH_MON_B3_003_042" },
	{ 1433, "ANH_MON_B3_003_043" },
	{ 1443, "ANH_MON_B3_003_044" },
	{ 9296, "ANH_MON_B3_003_B" },
	{ 3504, "ANH_MON_B3_003_ILLUST" },
	{ 8699, "ANH_MON_B3_003_P" },
	{ 3501, "ANH_MON_B3_003_TEST" },
	{ 1348, "ANH_MON_B3_004_000" },
	{ 1343, "ANH_MON_B3_004_002" },
	{ 1345, "ANH_MON_B3_004_003" },
	{ 2834, "ANH_MON_B3_004_007" },
	{ 1349, "ANH_MON_B3_004_010" },
	{ 1351, "ANH_MON_B3_004_011" },
	{ 1344, "ANH_MON_B3_004_012" },
	{ 1346, "ANH_MON_B3_004_013" },
	{ 1352, "ANH_MON_B3_004_014" },
	{ 1347, "ANH_MON_B3_004_020" },
	{ 1350, "ANH_MON_B3_004_021" },
	{ 1342, "ANH_MON_B3_004_022" },
	{ 9300, "ANH_MON_B3_004_B" },
	{ 2833, "ANH_MON_B3_004_ILLUST" },
	{ 772, "ANH_MON_B3_004_P" },
	{ 2836, "ANH_MON_B3_004_TEST" },
	{ 1769, "ANH_MON_B3_005_000" },
	{ 9653, "ANH_MON_B3_005_001" },
	{ 1753, "ANH_MON_B3_005_002" },
	{ 1759, "ANH_MON_B3_005_003" },
	{ 1771, "ANH_MON_B3_005_010" },
	{ 1778, "ANH_MON_B3_005_011" },
	{ 1755, "ANH_MON_B3_005_012" },
	{ 1761, "ANH_MON_B3_005_013" },
	{ 1782, "ANH_MON_B3_005_014" },
	{ 1765, "ANH_MON_B3_005_020" },
	{ 1775, "ANH_MON_B3_005_021" },
	{ 1751, "ANH_MON_B3_005_022" },
	{ 1767, "ANH_MON_B3_005_030" },
	{ 1763, "ANH_MON_B3_005_040" },
	{ 1773, "ANH_MON_B3_005_041" },
	{ 1749, "ANH_MON_B3_005_042" },
	{ 1757, "ANH_MON_B3_005_043" },
	{ 1779, "ANH_MON_B3_005_044" },
	{ 288, "ANH_MON_B3_005_045" },
	{ 12482, "ANH_MON_B3_005_B" },
	{ 3412, "ANH_MON_B3_005_ILLUST" },
	{ 12484, "ANH_MON_B3_005_P" },
	{ 3409, "ANH_MON_B3_005_TEST" },
	{ 3549, "ANH_MON_B3_006_000" },
	{ 3539, "ANH_MON_B3_006_002" },
	{ 3543, "ANH_MON_B3_006_003" },
	{ 3559, "ANH_MON_B3_006_007" },
	{ 3551, "ANH_MON_B3_006_010" },
	{ 3558, "ANH_MON_B3_006_011" },
	{ 3541, "ANH_MON_B3_006_012" },
	{ 3545, "ANH_MON_B3_006_013" },
	{ 3562, "ANH_MON_B3_006_014" },
	{ 3547, "ANH_MON_B3_006_020" },
	{ 3555, "ANH_MON_B3_006_021" },
	{ 3537, "ANH_MON_B3_006_022" },
	{ 9299, "ANH_MON_B3_006_B" },
	{ 3554, "ANH_MON_B3_006_ILLUST" },
	{ 8708, "ANH_MON_B3_006_P" },
	{ 3563, "ANH_MON_B3_006_TEST" },
	{ 3431, "ANH_MON_B3_007_000" },
	{ 3417, "ANH_MON_B3_007_002" },
	{ 3423, "ANH_MON_B3_007_003" },
	{ 3443, "ANH_MON_B3_007_007" },
	{ 3433, "ANH_MON_B3_007_010" },
	{ 3442, "ANH_MON_B3_007_011" },
	{ 3419, "ANH_MON_B3_007_012" },
	{ 3425, "ANH_MON_B3_007_013" },
	{ 3446, "ANH_MON_B3_007_014" },
	{ 3427, "ANH_MON_B3_007_020" },
	{ 3437, "ANH_MON_B3_007_021" },
	{ 3413, "ANH_MON_B3_007_022" },
	{ 3429, "ANH_MON_B3_007_030" },
	{ 3439, "ANH_MON_B3_007_031" },
	{ 3415, "ANH_MON_B3_007_032" },
	{ 9298, "ANH_MON_B3_007_B" },
	{ 3436, "ANH_MON_B3_007_ILLUST" },
	{ 8705, "ANH_MON_B3_007_P" },
	{ 3421, "ANH_MON_B3_007_TEST" },
	{ 3579, "ANH_MON_B3_008_000" },
	{ 3567, "ANH_MON_B3_008_002" },
	{ 3571, "ANH_MON_B3_008_003" },
	{ 3587, "ANH_MON_B3_008_007" },
	{ 3581, "ANH_MON_B3_008_010" },
	{ 3586, "ANH_MON_B3_008_011" },
	{ 3569, "ANH_MON_B3_008_012" },
	{ 3573, "ANH_MON_B3_008_013" },
	{ 3590, "ANH_MON_B3_008_014" },
	{ 3577, "ANH_MON_B3_008_020" },
	{ 3583, "ANH_MON_B3_008_021" },
	{ 3565, "ANH_MON_B3_008_022" },
	{ 11856, "ANH_MON_B3_008_032" },
	{ 9302, "ANH_MON_B3_008_B" },
	{ 3576, "ANH_MON_B3_008_ILLUST" },
	{ 8715, "ANH_MON_B3_008_P" },
	{ 3591, "ANH_MON_B3_008_TEST" },
	{ 3459, "ANH_MON_B3_009_000" },
	{ 3451, "ANH_MON_B3_009_002" },
	{ 3453, "ANH_MON_B3_009_003" },
	{ 3463, "ANH_MON_B3_009_007" },
	{ 3457, "ANH_MON_B3_009_020" },
	{ 3461, "ANH_MON_B3_009_021" },
	{ 3449, "ANH_MON_B3_009_022" },
	{ 9301, "ANH_MON_B3_009_B" },
	{ 3456, "ANH_MON_B3_009_ILLUST" },
	{ 8712, "ANH_MON_B3_009_P" },
	{ 3447, "ANH_MON_B3_009_TEST" },
	{ 3621, "ANH_MON_B3_010_000" },
	{ 3601, "ANH_MON_B3_010_002" },
	{ 3611, "ANH_MON_B3_010_003" },
	{ 3637, "ANH_MON_B3_010_007" },
	{ 3623, "ANH_MON_B3_010_010" },
	{ 3632, "ANH_MON_B3_010_011" },
	{ 3603, "ANH_MON_B3_010_012" },
	{ 3613, "ANH_MON_B3_010_013" },
	{ 3644, "ANH_MON_B3_010_014" },
	{ 3617, "ANH_MON_B3_010_020" },
	{ 3627, "ANH_MON_B3_010_021" },
	{ 3597, "ANH_MON_B3_010_022" },
	{ 3619, "ANH_MON_B3_010_030" },
	{ 3629, "ANH_MON_B3_010_031" },
	{ 3599, "ANH_MON_B3_010_032" },
	{ 3610, "ANH_MON_B3_010_033" },
	{ 3642, "ANH_MON_B3_010_034" },
	{ 3648, "ANH_MON_B3_010_035" },
	{ 3636, "ANH_MON_B3_010_036" },
	{ 3615, "ANH_MON_B3_010_040" },
	{ 3625, "ANH_MON_B3_010_041" },
	{ 3595, "ANH_MON_B3_010_042" },
	{ 3607, "ANH_MON_B3_010_043" },
	{ 3639, "ANH_MON_B3_010_044" },
	{ 3645, "ANH_MON_B3_010_045" },
	{ 3633, "ANH_MON_B3_010_046" },
	{ 9305, "ANH_MON_B3_010_B" },
	{ 3594, "ANH_MON_B3_010_ILLUST" },
	{ 8726, "ANH_MON_B3_010_P" },
	{ 3605, "ANH_MON_B3_010_TEST" },
	{ 381, "ANH_MON_B3_011_000" },
	{ 3689, "ANH_MON_B3_011_002" },
	{ 380, "ANH_MON_B3_011_003" },
	{ 3699, "ANH_MON_B3_011_007" },
	{ 383, "ANH_MON_B3_011_010" },
	{ 384, "ANH_MON_B3_011_011" },
	{ 378, "ANH_MON_B3_011_012" },
	{ 3692, "ANH_MON_B3_011_013" },
	{ 3702, "ANH_MON_B3_011_014" },
	{ 3693, "ANH_MON_B3_011_020" },
	{ 3697, "ANH_MON_B3_011_021" },
	{ 3687, "ANH_MON_B3_011_022" },
	{ 3695, "ANH_MON_B3_011_030" },
	{ 379, "ANH_MON_B3_011_B" },
	{ 3686, "ANH_MON_B3_011_ILLUST" },
	{ 8723, "ANH_MON_B3_011_P" },
	{ 3683, "ANH_MON_B3_011_TEST" },
	{ 3669, "ANH_MON_B3_012_000" },
	{ 3655, "ANH_MON_B3_012_002" },
	{ 3661, "ANH_MON_B3_012_003" },
	{ 3679, "ANH_MON_B3_012_007" },
	{ 3671, "ANH_MON_B3_012_010" },
	{ 3678, "ANH_MON_B3_012_011" },
	{ 3657, "ANH_MON_B3_012_012" },
	{ 3663, "ANH_MON_B3_012_013" },
	{ 3682, "ANH_MON_B3_012_014" },
	{ 3665, "ANH_MON_B3_012_020" },
	{ 3673, "ANH_MON_B3_012_021" },
	{ 3651, "ANH_MON_B3_012_022" },
	{ 3667, "ANH_MON_B3_012_030" },
	{ 3675, "ANH_MON_B3_012_031" },
	{ 3653, "ANH_MON_B3_012_032" },
	{ 9304, "ANH_MON_B3_012_B" },
	{ 3650, "ANH_MON_B3_012_ILLUST" },
	{ 8721, "ANH_MON_B3_012_P" },
	{ 3659, "ANH_MON_B3_012_TEST" },
	{ 1470, "ANH_MON_B3_013_000" },
	{ 1463, "ANH_MON_B3_013_002" },
	{ 1466, "ANH_MON_B3_013_003" },
	{ 2842, "ANH_MON_B3_013_007" },
	{ 1471, "ANH_MON_B3_013_010" },
	{ 1474, "ANH_MON_B3_013_011" },
	{ 1464, "ANH_MON_B3_013_012" },
	{ 1467, "ANH_MON_B3_013_013" },
	{ 1476, "ANH_MON_B3_013_014" },
	{ 1468, "ANH_MON_B3_013_020" },
	{ 1472, "ANH_MON_B3_013_021" },
	{ 1461, "ANH_MON_B3_013_022" },
	{ 1469, "ANH_MON_B3_013_030" },
	{ 1473, "ANH_MON_B3_013_031" },
	{ 1462, "ANH_MON_B3_013_032" },
	{ 1465, "ANH_MON_B3_013_033" },
	{ 1475, "ANH_MON_B3_013_034" },
	{ 9303, "ANH_MON_B3_013_B" },
	{ 2841, "ANH_MON_B3_013_ILLUST" },
	{ 8718, "ANH_MON_B3_013_P" },
	{ 2838, "ANH_MON_B3_013_TEST" },
	{ 1494, "ANH_MON_B3_014_000" },
	{ 1489, "ANH_MON_B3_014_002" },
	{ 1491, "ANH_MON_B3_014_003" },
	{ 2856, "ANH_MON_B3_014_007" },
	{ 1495, "ANH_MON_B3_014_010" },
	{ 1497, "ANH_MON_B3_014_011" },
	{ 1490, "ANH_MON_B3_014_012" },
	{ 1492, "ANH_MON_B3_014_013" },
	{ 1498, "ANH_MON_B3_014_014" },
	{ 1493, "ANH_MON_B3_014_020" },
	{ 1496, "ANH_MON_B3_014_021" },
	{ 1488, "ANH_MON_B3_014_022" },
	{ 9308, "ANH_MON_B3_014_B" },
	{ 2852, "ANH_MON_B3_014_ICHIDA" },
	{ 2851, "ANH_MON_B3_014_ILLUST" },
	{ 8735, "ANH_MON_B3_014_P" },
	{ 2854, "ANH_MON_B3_014_TEST" },
	{ 3723, "ANH_MON_B3_015_000" },
	{ 3711, "ANH_MON_B3_015_002" },
	{ 3715, "ANH_MON_B3_015_003" },
	{ 3733, "ANH_MON_B3_015_007" },
	{ 3725, "ANH_MON_B3_015_010" },
	{ 3732, "ANH_MON_B3_015_011" },
	{ 3713, "ANH_MON_B3_015_012" },
	{ 3717, "ANH_MON_B3_015_013" },
	{ 3736, "ANH_MON_B3_015_014" },
	{ 3719, "ANH_MON_B3_015_020" },
	{ 3727, "ANH_MON_B3_015_021" },
	{ 3707, "ANH_MON_B3_015_022" },
	{ 3721, "ANH_MON_B3_015_030" },
	{ 3729, "ANH_MON_B3_015_031" },
	{ 3709, "ANH_MON_B3_015_032" },
	{ 9307, "ANH_MON_B3_015_B" },
	{ 3706, "ANH_MON_B3_015_ILLUST" },
	{ 8732, "ANH_MON_B3_015_P" },
	{ 3703, "ANH_MON_B3_015_TEST" },
	{ 1799, "ANH_MON_B3_016_000" },
	{ 1789, "ANH_MON_B3_016_002" },
	{ 1791, "ANH_MON_B3_016_003" },
	{ 3741, "ANH_MON_B3_016_007" },
	{ 1795, "ANH_MON_B3_016_020" },
	{ 1803, "ANH_MON_B3_016_021" },
	{ 1785, "ANH_MON_B3_016_022" },
	{ 1797, "ANH_MON_B3_016_030" },
	{ 1805, "ANH_MON_B3_016_031" },
	{ 1787, "ANH_MON_B3_016_032" },
	{ 1793, "ANH_MON_B3_016_040" },
	{ 1801, "ANH_MON_B3_016_041" },
	{ 1783, "ANH_MON_B3_016_042" },
	{ 12486, "ANH_MON_B3_016_B" },
	{ 3738, "ANH_MON_B3_016_ILLUST" },
	{ 12488, "ANH_MON_B3_016_P" },
	{ 3739, "ANH_MON_B3_016_TEST" },
	{ 3761, "ANH_MON_B3_017_000" },
	{ 3751, "ANH_MON_B3_017_002" },
	{ 3755, "ANH_MON_B3_017_003" },
	{ 3771, "ANH_MON_B3_017_007" },
	{ 3763, "ANH_MON_B3_017_010" },
	{ 3768, "ANH_MON_B3_017_011" },
	{ 3753, "ANH_MON_B3_017_012" },
	{ 3757, "ANH_MON_B3_017_013" },
	{ 3776, "ANH_MON_B3_017_014" },
	{ 3778, "ANH_MON_B3_017_015" },
	{ 3770, "ANH_MON_B3_017_016" },
	{ 3773, "ANH_MON_B3_017_017" },
	{ 3746, "ANH_MON_B3_017_018" },
	{ 3759, "ANH_MON_B3_017_020" },
	{ 3765, "ANH_MON_B3_017_021" },
	{ 3749, "ANH_MON_B3_017_022" },
	{ 9306, "ANH_MON_B3_017_B" },
	{ 3748, "ANH_MON_B3_017_ILLUST" },
	{ 8729, "ANH_MON_B3_017_P" },
	{ 3743, "ANH_MON_B3_017_TEST" },
	{ 1823, "ANH_MON_B3_018_000" },
	{ 1811, "ANH_MON_B3_018_002" },
	{ 1815, "ANH_MON_B3_018_003" },
	{ 3793, "ANH_MON_B3_018_007" },
	{ 1825, "ANH_MON_B3_018_010" },
	{ 1832, "ANH_MON_B3_018_011" },
	{ 1813, "ANH_MON_B3_018_012" },
	{ 1817, "ANH_MON_B3_018_013" },
	{ 1834, "ANH_MON_B3_018_014" },
	{ 1819, "ANH_MON_B3_018_020" },
	{ 1827, "ANH_MON_B3_018_021" },
	{ 1807, "ANH_MON_B3_018_022" },
	{ 1821, "ANH_MON_B3_018_030" },
	{ 1829, "ANH_MON_B3_018_031" },
	{ 1809, "ANH_MON_B3_018_032" },
	{ 12490, "ANH_MON_B3_018_B" },
	{ 3790, "ANH_MON_B3_018_ILLUST" },
	{ 12492, "ANH_MON_B3_018_P" },
	{ 3791, "ANH_MON_B3_018_TEST" },
	{ 398, "ANH_MON_B3_019_000" },
	{ 386, "ANH_MON_B3_019_002" },
	{ 391, "ANH_MON_B3_019_003" },
	{ 3785, "ANH_MON_B3_019_007" },
	{ 399, "ANH_MON_B3_019_010" },
	{ 401, "ANH_MON_B3_019_011" },
	{ 388, "ANH_MON_B3_019_012" },
	{ 3784, "ANH_MON_B3_019_013" },
	{ 3788, "ANH_MON_B3_019_014" },
	{ 397, "ANH_MON_B3_019_020" },
	{ 400, "ANH_MON_B3_019_021" },
	{ 385, "ANH_MON_B3_019_022" },
	{ 390, "ANH_MON_B3_019_B" },
	{ 3782, "ANH_MON_B3_019_ILLUST" },
	{ 8737, "ANH_MON_B3_019_P" },
	{ 3779, "ANH_MON_B3_019_TEST" },
	{ 1373, "ANH_MON_B3_020_000" },
	{ 1367, "ANH_MON_B3_020_002" },
	{ 1370, "ANH_MON_B3_020_003" },
	{ 2848, "ANH_MON_B3_020_007" },
	{ 1374, "ANH_MON_B3_020_010" },
	{ 1376, "ANH_MON_B3_020_011" },
	{ 1368, "ANH_MON_B3_020_012" },
	{ 1371, "ANH_MON_B3_020_013" },
	{ 1378, "ANH_MON_B3_020_014" },
	{ 1372, "ANH_MON_B3_020_020" },
	{ 1375, "ANH_MON_B3_020_021" },
	{ 1366, "ANH_MON_B3_020_022" },
	{ 1369, "ANH_MON_B3_020_023" },
	{ 1377, "ANH_MON_B3_020_024" },
	{ 811, "ANH_MON_B3_020_A" },
	{ 810, "ANH_MON_B3_020_B" },
	{ 2847, "ANH_MON_B3_020_ILLUST" },
	{ 812, "ANH_MON_B3_020_P" },
	{ 809, "ANH_MON_B3_020_TEST" },
	{ 2845, "ANH_MON_B3_020_TEST2" },
	{ 361, "ANH_MON_B3_021_000" },
	{ 356, "ANH_MON_B3_021_002" },
	{ 358, "ANH_MON_B3_021_003" },
	{ 3815, "ANH_MON_B3_021_007" },
	{ 362, "ANH_MON_B3_021_010" },
	{ 375, "ANH_MON_B3_021_011" },
	{ 357, "ANH_MON_B3_021_012" },
	{ 359, "ANH_MON_B3_021_013" },
	{ 3820, "ANH_MON_B3_021_014" },
	{ 376, "ANH_MON_B3_021_015" },
	{ 3808, "ANH_MON_B3_021_020" },
	{ 3813, "ANH_MON_B3_021_021" },
	{ 3801, "ANH_MON_B3_021_022" },
	{ 3810, "ANH_MON_B3_021_030" },
	{ 373, "ANH_MON_B3_021_031" },
	{ 355, "ANH_MON_B3_021_032" },
	{ 3806, "ANH_MON_B3_021_033" },
	{ 3818, "ANH_MON_B3_021_034" },
	{ 360, "ANH_MON_B3_021_040" },
	{ 366, "ANH_MON_B3_021_041" },
	{ 354, "ANH_MON_B3_021_042" },
	{ 377, "ANH_MON_B3_021_B" },
	{ 3812, "ANH_MON_B3_021_ILLUST" },
	{ 8666, "ANH_MON_B3_021_P" },
	{ 3803, "ANH_MON_B3_021_TEST" },
	{ 3842, "ANH_MON_B3_022_000" },
	{ 3826, "ANH_MON_B3_022_002" },
	{ 3832, "ANH_MON_B3_022_003" },
	{ 3855, "ANH_MON_B3_022_007" },
	{ 3844, "ANH_MON_B3_022_010" },
	{ 3854, "ANH_MON_B3_022_011" },
	{ 3828, "ANH_MON_B3_022_012" },
	{ 3834, "ANH_MON_B3_022_013" },
	{ 3858, "ANH_MON_B3_022_014" },
	{ 3838, "ANH_MON_B3_022_020" },
	{ 3850, "ANH_MON_B3_022_021" },
	{ 3822, "ANH_MON_B3_022_022" },
	{ 3840, "ANH_MON_B3_022_030" },
	{ 3852, "ANH_MON_B3_022_031" },
	{ 3824, "ANH_MON_B3_022_032" },
	{ 3830, "ANH_MON_B3_022_033" },
	{ 3835, "ANH_MON_B3_022_040" },
	{ 3847, "ANH_MON_B3_022_041" },
	{ 9275, "ANH_MON_B3_022_B" },
	{ 3846, "ANH_MON_B3_022_ILLUST" },
	{ 8664, "ANH_MON_B3_022_P" },
	{ 3859, "ANH_MON_B3_022_TEST" },
	{ 1656, "ANH_MON_B3_023_000" },
	{ 1642, "ANH_MON_B3_023_002" },
	{ 1648, "ANH_MON_B3_023_003" },
	{ 3865, "ANH_MON_B3_023_007" },
	{ 1658, "ANH_MON_B3_023_010" },
	{ 1664, "ANH_MON_B3_023_011" },
	{ 1644, "ANH_MON_B3_023_012" },
	{ 1650, "ANH_MON_B3_023_013" },
	{ 1666, "ANH_MON_B3_023_014" },
	{ 1652, "ANH_MON_B3_023_020" },
	{ 1660, "ANH_MON_B3_023_021" },
	{ 1616, "ANH_MON_B3_023_022" },
	{ 1654, "ANH_MON_B3_023_030" },
	{ 1662, "ANH_MON_B3_023_031" },
	{ 1618, "ANH_MON_B3_023_032" },
	{ 1646, "ANH_MON_B3_023_033" },
	{ 12462, "ANH_MON_B3_023_B" },
	{ 3864, "ANH_MON_B3_023_ILLUST" },
	{ 12464, "ANH_MON_B3_023_P" },
	{ 3861, "ANH_MON_B3_023_TEST" },
	{ 9279, "ANH_MON_B3_024_B" },
	{ 9281, "ANH_MON_B3_024_P" },
	{ 4937, "ANH_MON_B3_025_000" },
	{ 4947, "ANH_MON_B3_025_001" },
	{ 4923, "ANH_MON_B3_025_002" },
	{ 4929, "ANH_MON_B3_025_003" },
	{ 4953, "ANH_MON_B3_025_004" },
	{ 4956, "ANH_MON_B3_025_005" },
	{ 4950, "ANH_MON_B3_025_006" },
	{ 4958, "ANH_MON_B3_025_008" },
	{ 4939, "ANH_MON_B3_025_010" },
	{ 4949, "ANH_MON_B3_025_011" },
	{ 4925, "ANH_MON_B3_025_012" },
	{ 4931, "ANH_MON_B3_025_013" },
	{ 4955, "ANH_MON_B3_025_014" },
	{ 4933, "ANH_MON_B3_025_020" },
	{ 4943, "ANH_MON_B3_025_021" },
	{ 4919, "ANH_MON_B3_025_022" },
	{ 4935, "ANH_MON_B3_025_030" },
	{ 4945, "ANH_MON_B3_025_031" },
	{ 4921, "ANH_MON_B3_025_032" },
	{ 9278, "ANH_MON_B3_025_B" },
	{ 4941, "ANH_MON_B3_025_ILLUST" },
	{ 8675, "ANH_MON_B3_025_P" },
	{ 4926, "ANH_MON_B3_025_TEST" },
	{ 3881, "ANH_MON_B3_026_000" },
	{ 3871, "ANH_MON_B3_026_002" },
	{ 3875, "ANH_MON_B3_026_003" },
	{ 3889, "ANH_MON_B3_026_007" },
	{ 3878, "ANH_MON_B3_026_020" },
	{ 3886, "ANH_MON_B3_026_021" },
	{ 3868, "ANH_MON_B3_026_022" },
	{ 3880, "ANH_MON_B3_026_030" },
	{ 3888, "ANH_MON_B3_026_031" },
	{ 3870, "ANH_MON_B3_026_032" },
	{ 3874, "ANH_MON_B3_026_033" },
	{ 3892, "ANH_MON_B3_026_034" },
	{ 9277, "ANH_MON_B3_026_B" },
	{ 3884, "ANH_MON_B3_026_ILLUST" },
	{ 8672, "ANH_MON_B3_026_P" },
	{ 3893, "ANH_MON_B3_026_TEST" },
	{ 4496, "ANH_MON_B3_027_000" },
	{ 4484, "ANH_MON_B3_027_002" },
	{ 4490, "ANH_MON_B3_027_003" },
	{ 4505, "ANH_MON_B3_027_007" },
	{ 4498, "ANH_MON_B3_027_010" },
	{ 4504, "ANH_MON_B3_027_011" },
	{ 4486, "ANH_MON_B3_027_012" },
	{ 4492, "ANH_MON_B3_027_013" },
	{ 4508, "ANH_MON_B3_027_014" },
	{ 4493, "ANH_MON_B3_027_020" },
	{ 4501, "ANH_MON_B3_027_021" },
	{ 4481, "ANH_MON_B3_027_022" },
	{ 9276, "ANH_MON_B3_027_B" },
	{ 4500, "ANH_MON_B3_027_ILLUST" },
	{ 8669, "ANH_MON_B3_027_P" },
	{ 4487, "ANH_MON_B3_027_TEST" },
	{ 4510, "ANH_MON_B3_027_TESTMOTION" },
	{ 1543, "ANH_MON_B3_028_000" },
	{ 1175, "ANH_MON_B3_028_002" },
	{ 1179, "ANH_MON_B3_028_003" },
	{ 3049, "ANH_MON_B3_028_007" },
	{ 1545, "ANH_MON_B3_028_010" },
	{ 3048, "ANH_MON_B3_028_011" },
	{ 1177, "ANH_MON_B3_028_012" },
	{ 1181, "ANH_MON_B3_028_013" },
	{ 3052, "ANH_MON_B3_028_014" },
	{ 1184, "ANH_MON_B3_028_020" },
	{ 1546, "ANH_MON_B3_028_021" },
	{ 1172, "ANH_MON_B3_028_022" },
	{ 9285, "ANH_MON_B3_028_B" },
	{ 1183, "ANH_MON_B3_028_ILLUST" },
	{ 8678, "ANH_MON_B3_028_P" },
	{ 3053, "ANH_MON_B3_028_TEST" },
	{ 9282, "ANH_MON_B3_029_B" },
	{ 9284, "ANH_MON_B3_029_P" },
	{ 1712, "ANH_MON_B3_030_000" },
	{ 1700, "ANH_MON_B3_030_002" },
	{ 1704, "ANH_MON_B3_030_003" },
	{ 3905, "ANH_MON_B3_030_007" },
	{ 1714, "ANH_MON_B3_030_010" },
	{ 1718, "ANH_MON_B3_030_011" },
	{ 1702, "ANH_MON_B3_030_012" },
	{ 1706, "ANH_MON_B3_030_013" },
	{ 1720, "ANH_MON_B3_030_014" },
	{ 1708, "ANH_MON_B3_030_020" },
	{ 1715, "ANH_MON_B3_030_021" },
	{ 1697, "ANH_MON_B3_030_022" },
	{ 1710, "ANH_MON_B3_030_030" },
	{ 12470, "ANH_MON_B3_030_B" },
	{ 3902, "ANH_MON_B3_030_ILLUST" },
	{ 12472, "ANH_MON_B3_030_P" },
	{ 3903, "ANH_MON_B3_030_TEST" },
	{ 1686, "ANH_MON_B3_031_000" },
	{ 1672, "ANH_MON_B3_031_002" },
	{ 1678, "ANH_MON_B3_031_003" },
	{ 3899, "ANH_MON_B3_031_007" },
	{ 1688, "ANH_MON_B3_031_010" },
	{ 1694, "ANH_MON_B3_031_011" },
	{ 1674, "ANH_MON_B3_031_012" },
	{ 1680, "ANH_MON_B3_031_013" },
	{ 1696, "ANH_MON_B3_031_014" },
	{ 1683, "ANH_MON_B3_031_020" },
	{ 1691, "ANH_MON_B3_031_021" },
	{ 1669, "ANH_MON_B3_031_022" },
	{ 1681, "ANH_MON_B3_031_040" },
	{ 1689, "ANH_MON_B3_031_041" },
	{ 1667, "ANH_MON_B3_031_042" },
	{ 1675, "ANH_MON_B3_031_043" },
	{ 12466, "ANH_MON_B3_031_B" },
	{ 3898, "ANH_MON_B3_031_ILLUST" },
	{ 12468, "ANH_MON_B3_031_P" },
	{ 3895, "ANH_MON_B3_031_TEST" },
	{ 3932, "ANH_MON_B3_032_000" },
	{ 3916, "ANH_MON_B3_032_002" },
	{ 3924, "ANH_MON_B3_032_003" },
	{ 3941, "ANH_MON_B3_032_007" },
	{ 3934, "ANH_MON_B3_032_010" },
	{ 3940, "ANH_MON_B3_032_011" },
	{ 3918, "ANH_MON_B3_032_012" },
	{ 3926, "ANH_MON_B3_032_013" },
	{ 3946, "ANH_MON_B3_032_014" },
	{ 3928, "ANH_MON_B3_032_020" },
	{ 3936, "ANH_MON_B3_032_021" },
	{ 3912, "ANH_MON_B3_032_022" },
	{ 3930, "ANH_MON_B3_032_030" },
	{ 3938, "ANH_MON_B3_032_031" },
	{ 3914, "ANH_MON_B3_032_032" },
	{ 3922, "ANH_MON_B3_032_033" },
	{ 3944, "ANH_MON_B3_032_034" },
	{ 8722, "ANH_MON_B3_032_035" },
	{ 8724, "ANH_MON_B3_032_036" },
	{ 8725, "ANH_MON_B3_032_037" },
	{ 8727, "ANH_MON_B3_032_038" },
	{ 9287, "ANH_MON_B3_032_B" },
	{ 3908, "ANH_MON_B3_032_ILLUST" },
	{ 8684, "ANH_MON_B3_032_P" },
	{ 3910, "ANH_MON_B3_032_SAMPLE" },
	{ 3919, "ANH_MON_B3_032_TEST" },
	{ 4535, "ANH_MON_B3_033_000" },
	{ 4523, "ANH_MON_B3_033_002" },
	{ 4525, "ANH_MON_B3_033_003" },
	{ 4545, "ANH_MON_B3_033_007" },
	{ 4532, "ANH_MON_B3_033_020" },
	{ 4542, "ANH_MON_B3_033_021" },
	{ 4520, "ANH_MON_B3_033_022" },
	{ 4534, "ANH_MON_B3_033_030" },
	{ 4544, "ANH_MON_B3_033_031" },
	{ 4522, "ANH_MON_B3_033_032" },
	{ 4528, "ANH_MON_B3_033_040" },
	{ 4538, "ANH_MON_B3_033_041" },
	{ 4516, "ANH_MON_B3_033_042" },
	{ 4530, "ANH_MON_B3_033_050" },
	{ 4540, "ANH_MON_B3_033_051" },
	{ 4518, "ANH_MON_B3_033_052" },
	{ 9286, "ANH_MON_B3_033_B" },
	{ 4514, "ANH_MON_B3_033_ILLUST" },
	{ 8681, "ANH_MON_B3_033_P" },
	{ 4511, "ANH_MON_B3_033_TEST" },
	{ 3964, "ANH_MON_B3_034_000" },
	{ 3952, "ANH_MON_B3_034_002" },
	{ 3958, "ANH_MON_B3_034_003" },
	{ 3973, "ANH_MON_B3_034_007" },
	{ 3966, "ANH_MON_B3_034_010" },
	{ 3970, "ANH_MON_B3_034_011" },
	{ 3954, "ANH_MON_B3_034_012" },
	{ 3960, "ANH_MON_B3_034_013" },
	{ 3976, "ANH_MON_B3_034_014" },
	{ 3978, "ANH_MON_B3_034_015" },
	{ 3972, "ANH_MON_B3_034_016" },
	{ 14531, "ANH_MON_B3_034_017" },
	{ 9651, "ANH_MON_B3_034_018" },
	{ 3961, "ANH_MON_B3_034_020" },
	{ 3967, "ANH_MON_B3_034_021" },
	{ 3949, "ANH_MON_B3_034_022" },
	{ 9293, "ANH_MON_B3_034_B" },
	{ 3948, "ANH_MON_B3_034_ILLUST" },
	{ 8690, "ANH_MON_B3_034_P" },
	{ 3955, "ANH_MON_B3_034_TEST" },
	{ 3996, "ANH_MON_B3_035_000" },
	{ 3986, "ANH_MON_B3_035_002" },
	{ 3990, "ANH_MON_B3_035_003" },
	{ 4003, "ANH_MON_B3_035_007" },
	{ 3998, "ANH_MON_B3_035_010" },
	{ 4002, "ANH_MON_B3_035_011" },
	{ 3988, "ANH_MON_B3_035_012" },
	{ 3992, "ANH_MON_B3_035_013" },
	{ 4006, "ANH_MON_B3_035_014" },
	{ 3993, "ANH_MON_B3_035_020" },
	{ 3999, "ANH_MON_B3_035_021" },
	{ 3983, "ANH_MON_B3_035_022" },
	{ 9292, "ANH_MON_B3_035_B" },
	{ 3982, "ANH_MON_B3_035_ILLUST" },
	{ 8687, "ANH_MON_B3_035_P" },
	{ 3979, "ANH_MON_B3_035_TEST" },
	{ 13767, "ANH_MON_B3_036_000" },
	{ 13755, "ANH_MON_B3_036_002" },
	{ 13759, "ANH_MON_B3_036_003" },
	{ 13776, "ANH_MON_B3_036_007" },
	{ 13769, "ANH_MON_B3_036_010" },
	{ 13775, "ANH_MON_B3_036_011" },
	{ 13757, "ANH_MON_B3_036_012" },
	{ 13761, "ANH_MON_B3_036_013" },
	{ 13779, "ANH_MON_B3_036_014" },
	{ 13763, "ANH_MON_B3_036_020" },
	{ 13771, "ANH_MON_B3_036_021" },
	{ 13751, "ANH_MON_B3_036_022" },
	{ 13765, "ANH_MON_B3_036_030" },
	{ 13773, "ANH_MON_B3_036_031" },
	{ 13753, "ANH_MON_B3_036_032" },
	{ 9289, "ANH_MON_B3_036_B" },
	{ 9291, "ANH_MON_B3_036_P" },
	{ 1523, "ANH_MON_B3_037_000" },
	{ 1518, "ANH_MON_B3_037_002" },
	{ 1520, "ANH_MON_B3_037_003" },
	{ 2862, "ANH_MON_B3_037_007" },
	{ 1521, "ANH_MON_B3_037_020" },
	{ 1524, "ANH_MON_B3_037_021" },
	{ 1516, "ANH_MON_B3_037_022" },
	{ 1522, "ANH_MON_B3_037_030" },
	{ 1525, "ANH_MON_B3_037_031" },
	{ 1517, "ANH_MON_B3_037_032" },
	{ 1519, "ANH_MON_B3_037_033" },
	{ 1526, "ANH_MON_B3_037_034" },
	{ 12474, "ANH_MON_B3_037_B" },
	{ 2861, "ANH_MON_B3_037_ILLUST" },
	{ 12476, "ANH_MON_B3_037_P" },
	{ 2858, "ANH_MON_B3_037_TEST" },
	{ 1405, "ANH_MON_B3_038_000" },
	{ 1399, "ANH_MON_B3_038_002" },
	{ 1401, "ANH_MON_B3_038_003" },
	{ 2868, "ANH_MON_B3_038_007" },
	{ 1403, "ANH_MON_B3_038_020" },
	{ 1407, "ANH_MON_B3_038_021" },
	{ 1397, "ANH_MON_B3_038_022" },
	{ 1404, "ANH_MON_B3_038_030" },
	{ 1408, "ANH_MON_B3_038_031" },
	{ 1398, "ANH_MON_B3_038_032" },
	{ 9295, "ANH_MON_B3_038_B" },
	{ 8728, "ANH_MON_B3_038_ILLUST" },
	{ 8696, "ANH_MON_B3_038_P" },
	{ 8730, "ANH_MON_B3_038_TEST" },
	{ 4054, "ANH_MON_B3_039_000" },
	{ 4040, "ANH_MON_B3_039_002" },
	{ 4046, "ANH_MON_B3_039_003" },
	{ 4063, "ANH_MON_B3_039_007" },
	{ 4056, "ANH_MON_B3_039_010" },
	{ 4062, "ANH_MON_B3_039_011" },
	{ 4042, "ANH_MON_B3_039_012" },
	{ 4048, "ANH_MON_B3_039_013" },
	{ 4068, "ANH_MON_B3_039_014" },
	{ 4050, "ANH_MON_B3_039_020" },
	{ 4058, "ANH_MON_B3_039_021" },
	{ 4036, "ANH_MON_B3_039_022" },
	{ 4052, "ANH_MON_B3_039_030" },
	{ 4060, "ANH_MON_B3_039_031" },
	{ 4038, "ANH_MON_B3_039_032" },
	{ 4044, "ANH_MON_B3_039_033" },
	{ 4066, "ANH_MON_B3_039_034" },
	{ 9294, "ANH_MON_B3_039_B" },
	{ 8693, "ANH_MON_B3_039_P" },
	{ 4019, "ANH_MON_B3_040_000" },
	{ 4009, "ANH_MON_B3_040_002" },
	{ 4013, "ANH_MON_B3_040_003" },
	{ 4029, "ANH_MON_B3_040_007" },
	{ 4021, "ANH_MON_B3_040_010" },
	{ 4028, "ANH_MON_B3_040_011" },
	{ 4011, "ANH_MON_B3_040_012" },
	{ 4015, "ANH_MON_B3_040_013" },
	{ 4032, "ANH_MON_B3_040_014" },
	{ 4017, "ANH_MON_B3_040_020" },
	{ 4025, "ANH_MON_B3_040_021" },
	{ 4007, "ANH_MON_B3_040_022" },
	{ 9259, "ANH_MON_B3_040_B" },
	{ 4024, "ANH_MON_B3_040_ILLUST" },
	{ 8616, "ANH_MON_B3_040_P" },
	{ 14597, "ANH_MON_B3_040_P2" },
	{ 4033, "ANH_MON_B3_040_TEST" },
	{ 6181, "ANH_MON_B3_041_000" },
	{ 6165, "ANH_MON_B3_041_002" },
	{ 6173, "ANH_MON_B3_041_003" },
	{ 6193, "ANH_MON_B3_041_007" },
	{ 6183, "ANH_MON_B3_041_010" },
	{ 6192, "ANH_MON_B3_041_011" },
	{ 6167, "ANH_MON_B3_041_012" },
	{ 6175, "ANH_MON_B3_041_013" },
	{ 6198, "ANH_MON_B3_041_014" },
	{ 6177, "ANH_MON_B3_041_020" },
	{ 6187, "ANH_MON_B3_041_021" },
	{ 6161, "ANH_MON_B3_041_022" },
	{ 6179, "ANH_MON_B3_041_030" },
	{ 6189, "ANH_MON_B3_041_031" },
	{ 6163, "ANH_MON_B3_041_032" },
	{ 6172, "ANH_MON_B3_041_033" },
	{ 6196, "ANH_MON_B3_041_034" },
	{ 9258, "ANH_MON_B3_041_B" },
	{ 6186, "ANH_MON_B3_041_ILLUST" },
	{ 8613, "ANH_MON_B3_041_P" },
	{ 6169, "ANH_MON_B3_041_TEST" },
	{ 4087, "ANH_MON_B3_042_000" },
	{ 4073, "ANH_MON_B3_042_002" },
	{ 4079, "ANH_MON_B3_042_003" },
	{ 4099, "ANH_MON_B3_042_007" },
	{ 4089, "ANH_MON_B3_042_010" },
	{ 4098, "ANH_MON_B3_042_011" },
	{ 4075, "ANH_MON_B3_042_012" },
	{ 4081, "ANH_MON_B3_042_013" },
	{ 4104, "ANH_MON_B3_042_014" },
	{ 4083, "ANH_MON_B3_042_020" },
	{ 4093, "ANH_MON_B3_042_021" },
	{ 4069, "ANH_MON_B3_042_022" },
	{ 4085, "ANH_MON_B3_042_030" },
	{ 4095, "ANH_MON_B3_042_031" },
	{ 4071, "ANH_MON_B3_042_032" },
	{ 4078, "ANH_MON_B3_042_033" },
	{ 4102, "ANH_MON_B3_042_034" },
	{ 9257, "ANH_MON_B3_042_B" },
	{ 4092, "ANH_MON_B3_042_ILLUST" },
	{ 8610, "ANH_MON_B3_042_P" },
	{ 4105, "ANH_MON_B3_042_TEST" },
	{ 5596, "ANH_MON_B3_043_000" },
	{ 5580, "ANH_MON_B3_043_002" },
	{ 5588, "ANH_MON_B3_043_003" },
	{ 5610, "ANH_MON_B3_043_007" },
	{ 5598, "ANH_MON_B3_043_010" },
	{ 5607, "ANH_MON_B3_043_011" },
	{ 5582, "ANH_MON_B3_043_012" },
	{ 5590, "ANH_MON_B3_043_013" },
	{ 5615, "ANH_MON_B3_043_014" },
	{ 5592, "ANH_MON_B3_043_020" },
	{ 5602, "ANH_MON_B3_043_021" },
	{ 5576, "ANH_MON_B3_043_022" },
	{ 5594, "ANH_MON_B3_043_030" },
	{ 5604, "ANH_MON_B3_043_031" },
	{ 5578, "ANH_MON_B3_043_032" },
	{ 5587, "ANH_MON_B3_043_033" },
	{ 5613, "ANH_MON_B3_043_034" },
	{ 5617, "ANH_MON_B3_043_035" },
	{ 5609, "ANH_MON_B3_043_036" },
	{ 9256, "ANH_MON_B3_043_B" },
	{ 5601, "ANH_MON_B3_043_ILLUST" },
	{ 8607, "ANH_MON_B3_043_P" },
	{ 5584, "ANH_MON_B3_043_TEST" },
	{ 4125, "ANH_MON_B3_044_000" },
	{ 4111, "ANH_MON_B3_044_002" },
	{ 4117, "ANH_MON_B3_044_003" },
	{ 4137, "ANH_MON_B3_044_007" },
	{ 4127, "ANH_MON_B3_044_010" },
	{ 4136, "ANH_MON_B3_044_011" },
	{ 4113, "ANH_MON_B3_044_012" },
	{ 4119, "ANH_MON_B3_044_013" },
	{ 4142, "ANH_MON_B3_044_014" },
	{ 4121, "ANH_MON_B3_044_020" },
	{ 4131, "ANH_MON_B3_044_021" },
	{ 4107, "ANH_MON_B3_044_022" },
	{ 4123, "ANH_MON_B3_044_030" },
	{ 4133, "ANH_MON_B3_044_031" },
	{ 4109, "ANH_MON_B3_044_032" },
	{ 4116, "ANH_MON_B3_044_033" },
	{ 4140, "ANH_MON_B3_044_034" },
	{ 9263, "ANH_MON_B3_044_B" },
	{ 4130, "ANH_MON_B3_044_ILLUST" },
	{ 8628, "ANH_MON_B3_044_P" },
	{ 4143, "ANH_MON_B3_044_TEST" },
	{ 4221, "ANH_MON_B3_045_000" },
	{ 4205, "ANH_MON_B3_045_002" },
	{ 4213, "ANH_MON_B3_045_003" },
	{ 4233, "ANH_MON_B3_045_007" },
	{ 4223, "ANH_MON_B3_045_010" },
	{ 4232, "ANH_MON_B3_045_011" },
	{ 4207, "ANH_MON_B3_045_012" },
	{ 4215, "ANH_MON_B3_045_013" },
	{ 4238, "ANH_MON_B3_045_014" },
	{ 4217, "ANH_MON_B3_045_020" },
	{ 4227, "ANH_MON_B3_045_021" },
	{ 4201, "ANH_MON_B3_045_022" },
	{ 4219, "ANH_MON_B3_045_030" },
	{ 4229, "ANH_MON_B3_045_031" },
	{ 4203, "ANH_MON_B3_045_032" },
	{ 4212, "ANH_MON_B3_045_033" },
	{ 4236, "ANH_MON_B3_045_034" },
	{ 936, "ANH_MON_B3_045_ATTACK" },
	{ 9262, "ANH_MON_B3_045_B" },
	{ 4226, "ANH_MON_B3_045_ILLUST" },
	{ 8625, "ANH_MON_B3_045_P" },
	{ 4209, "ANH_MON_B3_045_TEST" },
	{ 4167, "ANH_MON_B3_046_000" },
	{ 4179, "ANH_MON_B3_046_001" },
	{ 4151, "ANH_MON_B3_046_002" },
	{ 4157, "ANH_MON_B3_046_003" },
	{ 4187, "ANH_MON_B3_046_004" },
	{ 4191, "ANH_MON_B3_046_005" },
	{ 4183, "ANH_MON_B3_046_006" },
	{ 4195, "ANH_MON_B3_046_008" },
	{ 4169, "ANH_MON_B3_046_010" },
	{ 4181, "ANH_MON_B3_046_011" },
	{ 4153, "ANH_MON_B3_046_012" },
	{ 4159, "ANH_MON_B3_046_013" },
	{ 4189, "ANH_MON_B3_046_014" },
	{ 4163, "ANH_MON_B3_046_020" },
	{ 4175, "ANH_MON_B3_046_021" },
	{ 4147, "ANH_MON_B3_046_022" },
	{ 4165, "ANH_MON_B3_046_030" },
	{ 4177, "ANH_MON_B3_046_031" },
	{ 4149, "ANH_MON_B3_046_032" },
	{ 4161, "ANH_MON_B3_046_040" },
	{ 4173, "ANH_MON_B3_046_041" },
	{ 4145, "ANH_MON_B3_046_042" },
	{ 4155, "ANH_MON_B3_046_043" },
	{ 4185, "ANH_MON_B3_046_044" },
	{ 4200, "ANH_MON_B3_046_ATACK_0" },
	{ 4198, "ANH_MON_B3_046_ATACK_1" },
	{ 9261, "ANH_MON_B3_046_B" },
	{ 4172, "ANH_MON_B3_046_ILLUST" },
	{ 8622, "ANH_MON_B3_046_P" },
	{ 4193, "ANH_MON_B3_046_TEST" },
	{ 4265, "ANH_MON_B3_047_000" },
	{ 11297, "ANH_MON_B3_047_001" },
	{ 4245, "ANH_MON_B3_047_002" },
	{ 4255, "ANH_MON_B3_047_003" },
	{ 4281, "ANH_MON_B3_047_007" },
	{ 4267, "ANH_MON_B3_047_010" },
	{ 4278, "ANH_MON_B3_047_011" },
	{ 4247, "ANH_MON_B3_047_012" },
	{ 4257, "ANH_MON_B3_047_013" },
	{ 4288, "ANH_MON_B3_047_014" },
	{ 4261, "ANH_MON_B3_047_020" },
	{ 4273, "ANH_MON_B3_047_021" },
	{ 4241, "ANH_MON_B3_047_022" },
	{ 4263, "ANH_MON_B3_047_030" },
	{ 4275, "ANH_MON_B3_047_031" },
	{ 4243, "ANH_MON_B3_047_032" },
	{ 4254, "ANH_MON_B3_047_033" },
	{ 4286, "ANH_MON_B3_047_034" },
	{ 4290, "ANH_MON_B3_047_035" },
	{ 4280, "ANH_MON_B3_047_036" },
	{ 4259, "ANH_MON_B3_047_040" },
	{ 4271, "ANH_MON_B3_047_041" },
	{ 4239, "ANH_MON_B3_047_042" },
	{ 4251, "ANH_MON_B3_047_043" },
	{ 4283, "ANH_MON_B3_047_044" },
	{ 11299, "ANH_MON_B3_047_045" },
	{ 9260, "ANH_MON_B3_047_B" },
	{ 11301, "ANH_MON_B3_047_IDOLING1" },
	{ 11302, "ANH_MON_B3_047_IDOLING2" },
	{ 11305, "ANH_MON_B3_047_ILLUST" },
	{ 8619, "ANH_MON_B3_047_P" },
	{ 11306, "ANH_MON_B3_047_TEST" },
	{ 11309, "ANH_MON_B3_047_TSUME" },
	{ 4575, "ANH_MON_B3_048_000" },
	{ 937, "ANH_MON_B3_048_001" },
	{ 4555, "ANH_MON_B3_048_002" },
	{ 4563, "ANH_MON_B3_048_003" },
	{ 4585, "ANH_MON_B3_048_007" },
	{ 4573, "ANH_MON_B3_048_020" },
	{ 4583, "ANH_MON_B3_048_021" },
	{ 4553, "ANH_MON_B3_048_022" },
	{ 4569, "ANH_MON_B3_048_040" },
	{ 4579, "ANH_MON_B3_048_041" },
	{ 4549, "ANH_MON_B3_048_042" },
	{ 4559, "ANH_MON_B3_048_043" },
	{ 4589, "ANH_MON_B3_048_044" },
	{ 4571, "ANH_MON_B3_048_050" },
	{ 4581, "ANH_MON_B3_048_051" },
	{ 4551, "ANH_MON_B3_048_052" },
	{ 4561, "ANH_MON_B3_048_053" },
	{ 4591, "ANH_MON_B3_048_054" },
	{ 4567, "ANH_MON_B3_048_060" },
	{ 4595, "ANH_MON_B3_048_060_064" },
	{ 4577, "ANH_MON_B3_048_061" },
	{ 4547, "ANH_MON_B3_048_062" },
	{ 4557, "ANH_MON_B3_048_063" },
	{ 4587, "ANH_MON_B3_048_064" },
	{ 9265, "ANH_MON_B3_048_B" },
	{ 4566, "ANH_MON_B3_048_ILLUST" },
	{ 8634, "ANH_MON_B3_048_P" },
	{ 4593, "ANH_MON_B3_048_TEST" },
	{ 4615, "ANH_MON_B3_049_000" },
	{ 4603, "ANH_MON_B3_049_002" },
	{ 4607, "ANH_MON_B3_049_003" },
	{ 4623, "ANH_MON_B3_049_007" },
	{ 4611, "ANH_MON_B3_049_020" },
	{ 4617, "ANH_MON_B3_049_021" },
	{ 4599, "ANH_MON_B3_049_022" },
	{ 4613, "ANH_MON_B3_049_030" },
	{ 4619, "ANH_MON_B3_049_031" },
	{ 4601, "ANH_MON_B3_049_032" },
	{ 4606, "ANH_MON_B3_049_033" },
	{ 4626, "ANH_MON_B3_049_034" },
	{ 4628, "ANH_MON_B3_049_035" },
	{ 4622, "ANH_MON_B3_049_036" },
	{ 9264, "ANH_MON_B3_049_B" },
	{ 8720, "ANH_MON_B3_049_ILLUST" },
	{ 8631, "ANH_MON_B3_049_P" },
	{ 8719, "ANH_MON_B3_049_TEST" },
	{ 3119, "ANH_MON_B3_050_000" },
	{ 3129, "ANH_MON_B3_050_001" },
	{ 3103, "ANH_MON_B3_050_002" },
	{ 3109, "ANH_MON_B3_050_003" },
	{ 3135, "ANH_MON_B3_050_004" },
	{ 3139, "ANH_MON_B3_050_005" },
	{ 3133, "ANH_MON_B3_050_006" },
	{ 3093, "ANH_MON_B3_050_008" },
	{ 3121, "ANH_MON_B3_050_010" },
	{ 3131, "ANH_MON_B3_050_011" },
	{ 3105, "ANH_MON_B3_050_012" },
	{ 3111, "ANH_MON_B3_050_013" },
	{ 3137, "ANH_MON_B3_050_014" },
	{ 3115, "ANH_MON_B3_050_020" },
	{ 3125, "ANH_MON_B3_050_021" },
	{ 3099, "ANH_MON_B3_050_022" },
	{ 3117, "ANH_MON_B3_050_030" },
	{ 3127, "ANH_MON_B3_050_031" },
	{ 3101, "ANH_MON_B3_050_032" },
	{ 3113, "ANH_MON_B3_050_040" },
	{ 3123, "ANH_MON_B3_050_041" },
	{ 3097, "ANH_MON_B3_050_042" },
	{ 9269, "ANH_MON_B3_050_B" },
	{ 3096, "ANH_MON_B3_050_ILLUST" },
	{ 8646, "ANH_MON_B3_050_P" },
	{ 3107, "ANH_MON_B3_050_TEST" },
	{ 6223, "ANH_MON_B3_051_000" },
	{ 6231, "ANH_MON_B3_051_001" },
	{ 6209, "ANH_MON_B3_051_002" },
	{ 6215, "ANH_MON_B3_051_003" },
	{ 6239, "ANH_MON_B3_051_004" },
	{ 6243, "ANH_MON_B3_051_005" },
	{ 6235, "ANH_MON_B3_051_006" },
	{ 6201, "ANH_MON_B3_051_008" },
	{ 6225, "ANH_MON_B3_051_010" },
	{ 6233, "ANH_MON_B3_051_011" },
	{ 6211, "ANH_MON_B3_051_012" },
	{ 6217, "ANH_MON_B3_051_013" },
	{ 6241, "ANH_MON_B3_051_014" },
	{ 14599, "ANH_MON_B3_051_015" },
	{ 6221, "ANH_MON_B3_051_020" },
	{ 6229, "ANH_MON_B3_051_021" },
	{ 6207, "ANH_MON_B3_051_022" },
	{ 6219, "ANH_MON_B3_051_040" },
	{ 6227, "ANH_MON_B3_051_041" },
	{ 6205, "ANH_MON_B3_051_042" },
	{ 6213, "ANH_MON_B3_051_043" },
	{ 6237, "ANH_MON_B3_051_044" },
	{ 9268, "ANH_MON_B3_051_B" },
	{ 6204, "ANH_MON_B3_051_ILLUST" },
	{ 8643, "ANH_MON_B3_051_P" },
	{ 6199, "ANH_MON_B3_051_TEST" },
	{ 4307, "ANH_MON_B3_052_000" },
	{ 4295, "ANH_MON_B3_052_002" },
	{ 4301, "ANH_MON_B3_052_003" },
	{ 4315, "ANH_MON_B3_052_007" },
	{ 4309, "ANH_MON_B3_052_010" },
	{ 4314, "ANH_MON_B3_052_011" },
	{ 4297, "ANH_MON_B3_052_012" },
	{ 4303, "ANH_MON_B3_052_013" },
	{ 4318, "ANH_MON_B3_052_014" },
	{ 4305, "ANH_MON_B3_052_020" },
	{ 4311, "ANH_MON_B3_052_021" },
	{ 4293, "ANH_MON_B3_052_022" },
	{ 9267, "ANH_MON_B3_052_B" },
	{ 4292, "ANH_MON_B3_052_ILLUST" },
	{ 8640, "ANH_MON_B3_052_P" },
	{ 4299, "ANH_MON_B3_052_TEST" },
	{ 4912, "ANH_MON_B3_053_000" },
	{ 4906, "ANH_MON_B3_053_002" },
	{ 4908, "ANH_MON_B3_053_003" },
	{ 4916, "ANH_MON_B3_053_007" },
	{ 4910, "ANH_MON_B3_053_020" },
	{ 4914, "ANH_MON_B3_053_021" },
	{ 4904, "ANH_MON_B3_053_022" },
	{ 9266, "ANH_MON_B3_053_B" },
	{ 4903, "ANH_MON_B3_053_ILLUST" },
	{ 8637, "ANH_MON_B3_053_P" },
	{ 4900, "ANH_MON_B3_053_TEST" },
	{ 13241, "ANH_MON_B3_054_000" },
	{ 13223, "ANH_MON_B3_054_002" },
	{ 13231, "ANH_MON_B3_054_003" },
	{ 13251, "ANH_MON_B3_054_007" },
	{ 13243, "ANH_MON_B3_054_010" },
	{ 13250, "ANH_MON_B3_054_011" },
	{ 13225, "ANH_MON_B3_054_012" },
	{ 13233, "ANH_MON_B3_054_013" },
	{ 13256, "ANH_MON_B3_054_014" },
	{ 13237, "ANH_MON_B3_054_020" },
	{ 13245, "ANH_MON_B3_054_021" },
	{ 13219, "ANH_MON_B3_054_022" },
	{ 13239, "ANH_MON_B3_054_030" },
	{ 13247, "ANH_MON_B3_054_031" },
	{ 13221, "ANH_MON_B3_054_032" },
	{ 13230, "ANH_MON_B3_054_033" },
	{ 13254, "ANH_MON_B3_054_034" },
	{ 13235, "ANH_MON_B3_054_040" },
	{ 9273, "ANH_MON_B3_054_B" },
	{ 13218, "ANH_MON_B3_054_ILLUST" },
	{ 8658, "ANH_MON_B3_054_P" },
	{ 13227, "ANH_MON_B3_054_TEST" },
	{ 7835, "ANH_MON_B3_055_000" },
	{ 7821, "ANH_MON_B3_055_002" },
	{ 7827, "ANH_MON_B3_055_003" },
	{ 7845, "ANH_MON_B3_055_007" },
	{ 7837, "ANH_MON_B3_055_010" },
	{ 7844, "ANH_MON_B3_055_011" },
	{ 7823, "ANH_MON_B3_055_012" },
	{ 7829, "ANH_MON_B3_055_013" },
	{ 7831, "ANH_MON_B3_055_020" },
	{ 7839, "ANH_MON_B3_055_021" },
	{ 7817, "ANH_MON_B3_055_022" },
	{ 7833, "ANH_MON_B3_055_030" },
	{ 7841, "ANH_MON_B3_055_031" },
	{ 7819, "ANH_MON_B3_055_032" },
	{ 7826, "ANH_MON_B3_055_033" },
	{ 7848, "ANH_MON_B3_055_034" },
	{ 9272, "ANH_MON_B3_055_B" },
	{ 7816, "ANH_MON_B3_055_ILLUST" },
	{ 8655, "ANH_MON_B3_055_P" },
	{ 7813, "ANH_MON_B3_055_POSE" },
	{ 7811, "ANH_MON_B3_055_TEST" },
	{ 4341, "ANH_MON_B3_056_000" },
	{ 938, "ANH_MON_B3_056_001" },
	{ 4325, "ANH_MON_B3_056_002" },
	{ 4333, "ANH_MON_B3_056_003" },
	{ 4351, "ANH_MON_B3_056_007" },
	{ 4343, "ANH_MON_B3_056_010" },
	{ 4350, "ANH_MON_B3_056_011" },
	{ 4327, "ANH_MON_B3_056_012" },
	{ 4335, "ANH_MON_B3_056_013" },
	{ 4356, "ANH_MON_B3_056_014" },
	{ 4337, "ANH_MON_B3_056_020" },
	{ 4345, "ANH_MON_B3_056_021" },
	{ 4321, "ANH_MON_B3_056_022" },
	{ 4339, "ANH_MON_B3_056_030" },
	{ 4347, "ANH_MON_B3_056_031" },
	{ 4323, "ANH_MON_B3_056_032" },
	{ 4332, "ANH_MON_B3_056_033" },
	{ 4354, "ANH_MON_B3_056_034" },
	{ 9271, "ANH_MON_B3_056_B" },
	{ 4320, "ANH_MON_B3_056_ILLUST" },
	{ 8652, "ANH_MON_B3_056_P" },
	{ 4329, "ANH_MON_B3_056_TEST" },
	{ 5636, "ANH_MON_B3_057_000" },
	{ 5626, "ANH_MON_B3_057_002" },
	{ 5630, "ANH_MON_B3_057_003" },
	{ 5642, "ANH_MON_B3_057_007" },
	{ 5632, "ANH_MON_B3_057_020" },
	{ 5638, "ANH_MON_B3_057_021" },
	{ 5622, "ANH_MON_B3_057_022" },
	{ 5634, "ANH_MON_B3_057_030" },
	{ 5640, "ANH_MON_B3_057_031" },
	{ 5624, "ANH_MON_B3_057_032" },
	{ 5629, "ANH_MON_B3_057_033" },
	{ 9270, "ANH_MON_B3_057_B" },
	{ 5621, "ANH_MON_B3_057_ILLUST" },
	{ 8649, "ANH_MON_B3_057_P" },
	{ 5618, "ANH_MON_B3_057_TEST" },
	{ 5700, "ANH_MON_B3_058_000" },
	{ 5686, "ANH_MON_B3_058_002" },
	{ 5692, "ANH_MON_B3_058_003" },
	{ 5712, "ANH_MON_B3_058_007" },
	{ 5702, "ANH_MON_B3_058_010" },
	{ 5709, "ANH_MON_B3_058_011" },
	{ 5688, "ANH_MON_B3_058_012" },
	{ 5694, "ANH_MON_B3_058_013" },
	{ 5715, "ANH_MON_B3_058_014" },
	{ 5717, "ANH_MON_B3_058_015" },
	{ 5711, "ANH_MON_B3_058_016" },
	{ 5696, "ANH_MON_B3_058_020" },
	{ 5706, "ANH_MON_B3_058_021" },
	{ 5684, "ANH_MON_B3_058_022" },
	{ 5698, "ANH_MON_B3_058_030" },
	{ 5691, "ANH_MON_B3_058_B" },
	{ 5705, "ANH_MON_B3_058_P" },
	{ 5666, "ANH_MON_B3_059_000" },
	{ 5652, "ANH_MON_B3_059_002" },
	{ 5656, "ANH_MON_B3_059_003" },
	{ 5676, "ANH_MON_B3_059_007" },
	{ 5668, "ANH_MON_B3_059_010" },
	{ 5675, "ANH_MON_B3_059_011" },
	{ 5654, "ANH_MON_B3_059_012" },
	{ 5658, "ANH_MON_B3_059_013" },
	{ 5679, "ANH_MON_B3_059_014" },
	{ 5662, "ANH_MON_B3_059_020" },
	{ 5672, "ANH_MON_B3_059_021" },
	{ 5650, "ANH_MON_B3_059_022" },
	{ 5664, "ANH_MON_B3_059_030" },
	{ 5660, "ANH_MON_B3_059_040" },
	{ 5670, "ANH_MON_B3_059_041" },
	{ 5648, "ANH_MON_B3_059_042" },
	{ 9274, "ANH_MON_B3_059_B" },
	{ 8661, "ANH_MON_B3_059_P" },
	{ 4377, "ANH_MON_B3_060_000" },
	{ 4363, "ANH_MON_B3_060_002" },
	{ 4369, "ANH_MON_B3_060_003" },
	{ 4387, "ANH_MON_B3_060_007" },
	{ 4374, "ANH_MON_B3_060_020" },
	{ 4384, "ANH_MON_B3_060_021" },
	{ 4360, "ANH_MON_B3_060_022" },
	{ 4376, "ANH_MON_B3_060_030" },
	{ 4386, "ANH_MON_B3_060_031" },
	{ 4362, "ANH_MON_B3_060_032" },
	{ 4368, "ANH_MON_B3_060_033" },
	{ 4392, "ANH_MON_B3_060_034" },
	{ 4371, "ANH_MON_B3_060_040" },
	{ 4381, "ANH_MON_B3_060_041" },
	{ 4357, "ANH_MON_B3_060_042" },
	{ 4365, "ANH_MON_B3_060_043" },
	{ 4389, "ANH_MON_B3_060_044" },
	{ 4393, "ANH_MON_B3_060_045" },
	{ 9240, "ANH_MON_B3_060_B" },
	{ 4380, "ANH_MON_B3_060_ILLUST" },
	{ 8559, "ANH_MON_B3_060_P" },
	{ 4395, "ANH_MON_B3_060_TEST" },
	{ 9919, "ANH_MON_B3_061_000" },
	{ 9933, "ANH_MON_B3_061_001" },
	{ 9897, "ANH_MON_B3_061_002" },
	{ 9907, "ANH_MON_B3_061_003" },
	{ 9943, "ANH_MON_B3_061_004" },
	{ 9946, "ANH_MON_B3_061_005" },
	{ 9936, "ANH_MON_B3_061_006" },
	{ 9948, "ANH_MON_B3_061_008" },
	{ 9921, "ANH_MON_B3_061_010" },
	{ 9935, "ANH_MON_B3_061_011" },
	{ 9899, "ANH_MON_B3_061_012" },
	{ 9909, "ANH_MON_B3_061_013" },
	{ 9945, "ANH_MON_B3_061_014" },
	{ 9915, "ANH_MON_B3_061_040" },
	{ 9929, "ANH_MON_B3_061_041" },
	{ 9893, "ANH_MON_B3_061_042" },
	{ 9917, "ANH_MON_B3_061_050" },
	{ 9931, "ANH_MON_B3_061_051" },
	{ 9895, "ANH_MON_B3_061_052" },
	{ 9905, "ANH_MON_B3_061_053" },
	{ 9941, "ANH_MON_B3_061_054" },
	{ 9911, "ANH_MON_B3_061_060" },
	{ 9925, "ANH_MON_B3_061_061" },
	{ 9890, "ANH_MON_B3_061_062" },
	{ 9902, "ANH_MON_B3_061_063" },
	{ 9938, "ANH_MON_B3_061_064" },
	{ 9913, "ANH_MON_B3_061_070" },
	{ 9927, "ANH_MON_B3_061_071" },
	{ 9239, "ANH_MON_B3_061_B" },
	{ 9923, "ANH_MON_B3_061_ILLUST" },
	{ 8556, "ANH_MON_B3_061_P" },
	{ 9900, "ANH_MON_B3_061_TEST" },
	{ 4813, "ANH_MON_B3_062_000" },
	{ 4795, "ANH_MON_B3_062_002" },
	{ 4803, "ANH_MON_B3_062_003" },
	{ 4826, "ANH_MON_B3_062_007" },
	{ 4815, "ANH_MON_B3_062_010" },
	{ 4825, "ANH_MON_B3_062_011" },
	{ 4797, "ANH_MON_B3_062_012" },
	{ 4805, "ANH_MON_B3_062_013" },
	{ 4833, "ANH_MON_B3_062_014" },
	{ 4809, "ANH_MON_B3_062_020" },
	{ 4821, "ANH_MON_B3_062_021" },
	{ 4791, "ANH_MON_B3_062_022" },
	{ 4811, "ANH_MON_B3_062_030" },
	{ 4823, "ANH_MON_B3_062_031" },
	{ 4793, "ANH_MON_B3_062_032" },
	{ 4801, "ANH_MON_B3_062_033" },
	{ 4831, "ANH_MON_B3_062_034" },
	{ 4806, "ANH_MON_B3_062_040" },
	{ 4818, "ANH_MON_B3_062_041" },
	{ 4788, "ANH_MON_B3_062_042" },
	{ 4798, "ANH_MON_B3_062_043" },
	{ 4828, "ANH_MON_B3_062_044" },
	{ 9238, "ANH_MON_B3_062_B" },
	{ 4817, "ANH_MON_B3_062_ILLUST" },
	{ 8553, "ANH_MON_B3_062_P" },
	{ 4834, "ANH_MON_B3_062_TEST" },
	{ 4413, "ANH_MON_B3_063_000" },
	{ 4401, "ANH_MON_B3_063_002" },
	{ 4407, "ANH_MON_B3_063_003" },
	{ 4421, "ANH_MON_B3_063_007" },
	{ 4410, "ANH_MON_B3_063_020" },
	{ 4418, "ANH_MON_B3_063_021" },
	{ 4398, "ANH_MON_B3_063_022" },
	{ 4412, "ANH_MON_B3_063_030" },
	{ 4420, "ANH_MON_B3_063_031" },
	{ 4400, "ANH_MON_B3_063_032" },
	{ 4406, "ANH_MON_B3_063_033" },
	{ 4424, "ANH_MON_B3_063_034" },
	{ 9237, "ANH_MON_B3_063_B" },
	{ 4416, "ANH_MON_B3_063_ILLUST" },
	{ 8550, "ANH_MON_B3_063_P" },
	{ 4403, "ANH_MON_B3_063_TEST" },
	{ 4888, "ANH_MON_B3_064_000" },
	{ 4880, "ANH_MON_B3_064_002" },
	{ 4882, "ANH_MON_B3_064_003" },
	{ 4896, "ANH_MON_B3_064_007" },
	{ 4885, "ANH_MON_B3_064_020" },
	{ 4893, "ANH_MON_B3_064_021" },
	{ 4877, "ANH_MON_B3_064_022" },
	{ 4887, "ANH_MON_B3_064_030" },
	{ 4895, "ANH_MON_B3_064_031" },
	{ 4879, "ANH_MON_B3_064_032" },
	{ 9244, "ANH_MON_B3_064_B" },
	{ 4891, "ANH_MON_B3_064_ILLUST" },
	{ 8571, "ANH_MON_B3_064_P" },
	{ 4898, "ANH_MON_B3_064_TEST" },
	{ 10767, "ANH_MON_B3_065_000" },
	{ 10755, "ANH_MON_B3_065_002" },
	{ 10761, "ANH_MON_B3_065_003" },
	{ 10776, "ANH_MON_B3_065_007" },
	{ 10769, "ANH_MON_B3_065_010" },
	{ 10775, "ANH_MON_B3_065_011" },
	{ 10757, "ANH_MON_B3_065_012" },
	{ 10763, "ANH_MON_B3_065_013" },
	{ 10779, "ANH_MON_B3_065_014" },
	{ 10764, "ANH_MON_B3_065_020" },
	{ 10772, "ANH_MON_B3_065_021" },
	{ 10752, "ANH_MON_B3_065_022" },
	{ 9243, "ANH_MON_B3_065_B" },
	{ 10771, "ANH_MON_B3_065_ILLUST" },
	{ 8568, "ANH_MON_B3_065_P" },
	{ 10758, "ANH_MON_B3_065_TEST" },
	{ 4855, "ANH_MON_B3_066_000" },
	{ 4841, "ANH_MON_B3_066_002" },
	{ 4847, "ANH_MON_B3_066_003" },
	{ 4866, "ANH_MON_B3_066_007" },
	{ 4857, "ANH_MON_B3_066_010" },
	{ 4865, "ANH_MON_B3_066_011" },
	{ 4843, "ANH_MON_B3_066_012" },
	{ 4849, "ANH_MON_B3_066_013" },
	{ 4871, "ANH_MON_B3_066_014" },
	{ 4851, "ANH_MON_B3_066_020" },
	{ 4861, "ANH_MON_B3_066_021" },
	{ 4837, "ANH_MON_B3_066_022" },
	{ 4853, "ANH_MON_B3_066_030" },
	{ 4863, "ANH_MON_B3_066_031" },
	{ 4839, "ANH_MON_B3_066_032" },
	{ 4845, "ANH_MON_B3_066_033" },
	{ 4869, "ANH_MON_B3_066_034" },
	{ 9242, "ANH_MON_B3_066_B" },
	{ 8565, "ANH_MON_B3_066_P" },
	{ 4444, "ANH_MON_B3_067_000" },
	{ 4430, "ANH_MON_B3_067_002" },
	{ 4437, "ANH_MON_B3_067_003" },
	{ 4455, "ANH_MON_B3_067_007" },
	{ 4446, "ANH_MON_B3_067_010" },
	{ 4454, "ANH_MON_B3_067_011" },
	{ 4432, "ANH_MON_B3_067_012" },
	{ 4440, "ANH_MON_B3_067_020" },
	{ 4450, "ANH_MON_B3_067_021" },
	{ 4426, "ANH_MON_B3_067_022" },
	{ 4442, "ANH_MON_B3_067_030" },
	{ 4452, "ANH_MON_B3_067_031" },
	{ 4428, "ANH_MON_B3_067_032" },
	{ 4436, "ANH_MON_B3_067_033" },
	{ 4458, "ANH_MON_B3_067_034" },
	{ 9241, "ANH_MON_B3_067_B" },
	{ 4448, "ANH_MON_B3_067_ILLUST" },
	{ 8562, "ANH_MON_B3_067_P" },
	{ 4433, "ANH_MON_B3_067_TEST" },
	{ 7746, "ANH_MON_B3_068_000" },
	{ 7730, "ANH_MON_B3_068_002" },
	{ 7736, "ANH_MON_B3_068_003" },
	{ 7755, "ANH_MON_B3_068_007" },
	{ 7748, "ANH_MON_B3_068_010" },
	{ 7754, "ANH_MON_B3_068_011" },
	{ 7732, "ANH_MON_B3_068_012" },
	{ 7738, "ANH_MON_B3_068_013" },
	{ 7760, "ANH_MON_B3_068_014" },
	{ 7742, "ANH_MON_B3_068_020" },
	{ 7750, "ANH_MON_B3_068_021" },
	{ 7726, "ANH_MON_B3_068_022" },
	{ 7744, "ANH_MON_B3_068_030" },
	{ 7752, "ANH_MON_B3_068_031" },
	{ 7728, "ANH_MON_B3_068_032" },
	{ 7734, "ANH_MON_B3_068_033" },
	{ 7758, "ANH_MON_B3_068_034" },
	{ 9246, "ANH_MON_B3_068_B" },
	{ 7740, "ANH_MON_B3_068_ILLUST" },
	{ 8577, "ANH_MON_B3_068_P" },
	{ 7761, "ANH_MON_B3_068_TEST" },
	{ 3074, "ANH_MON_B3_069_000" },
	{ 3080, "ANH_MON_B3_069_001" },
	{ 3060, "ANH_MON_B3_069_002" },
	{ 3064, "ANH_MON_B3_069_003" },
	{ 3086, "ANH_MON_B3_069_004" },
	{ 3089, "ANH_MON_B3_069_005" },
	{ 3083, "ANH_MON_B3_069_006" },
	{ 3091, "ANH_MON_B3_069_008" },
	{ 3076, "ANH_MON_B3_069_010" },
	{ 3082, "ANH_MON_B3_069_011" },
	{ 3062, "ANH_MON_B3_069_012" },
	{ 3066, "ANH_MON_B3_069_013" },
	{ 3088, "ANH_MON_B3_069_014" },
	{ 3071, "ANH_MON_B3_069_020" },
	{ 3077, "ANH_MON_B3_069_021" },
	{ 3057, "ANH_MON_B3_069_022" },
	{ 9245, "ANH_MON_B3_069_B" },
	{ 3068, "ANH_MON_B3_069_ILLUST" },
	{ 8574, "ANH_MON_B3_069_P" },
	{ 3055, "ANH_MON_B3_069_TEST" },
	{ 3070, "ANH_MON_B3_069_TEST2" },
	{ 5736, "ANH_MON_B3_070_000" },
	{ 5724, "ANH_MON_B3_070_002" },
	{ 5730, "ANH_MON_B3_070_003" },
	{ 5742, "ANH_MON_B3_070_007" },
	{ 5734, "ANH_MON_B3_070_020" },
	{ 5740, "ANH_MON_B3_070_021" },
	{ 5722, "ANH_MON_B3_070_022" },
	{ 5732, "ANH_MON_B3_070_040" },
	{ 5738, "ANH_MON_B3_070_041" },
	{ 5720, "ANH_MON_B3_070_042" },
	{ 5728, "ANH_MON_B3_070_043" },
	{ 5744, "ANH_MON_B3_070_044" },
	{ 9250, "ANH_MON_B3_070_B" },
	{ 8589, "ANH_MON_B3_070_P" },
	{ 6322, "ANH_MON_B3_071_000" },
	{ 6304, "ANH_MON_B3_071_002" },
	{ 6312, "ANH_MON_B3_071_003" },
	{ 6333, "ANH_MON_B3_071_007" },
	{ 6324, "ANH_MON_B3_071_010" },
	{ 6332, "ANH_MON_B3_071_011" },
	{ 6306, "ANH_MON_B3_071_012" },
	{ 6314, "ANH_MON_B3_071_013" },
	{ 6340, "ANH_MON_B3_071_014" },
	{ 6318, "ANH_MON_B3_071_020" },
	{ 6328, "ANH_MON_B3_071_021" },
	{ 6300, "ANH_MON_B3_071_022" },
	{ 6320, "ANH_MON_B3_071_030" },
	{ 6330, "ANH_MON_B3_071_031" },
	{ 6302, "ANH_MON_B3_071_032" },
	{ 6310, "ANH_MON_B3_071_033" },
	{ 6338, "ANH_MON_B3_071_034" },
	{ 6315, "ANH_MON_B3_071_040" },
	{ 6325, "ANH_MON_B3_071_041" },
	{ 6297, "ANH_MON_B3_071_042" },
	{ 6307, "ANH_MON_B3_071_043" },
	{ 6335, "ANH_MON_B3_071_044" },
	{ 9249, "ANH_MON_B3_071_B" },
	{ 6296, "ANH_MON_B3_071_ILLUST" },
	{ 8586, "ANH_MON_B3_071_P" },
	{ 6293, "ANH_MON_B3_071_TEST" },
	{ 5208, "ANH_MON_B3_072_000" },
	{ 5216, "ANH_MON_B3_072_001" },
	{ 5161, "ANH_MON_B3_072_002" },
	{ 5169, "ANH_MON_B3_072_003" },
	{ 5227, "ANH_MON_B3_072_004" },
	{ 5219, "ANH_MON_B3_072_006" },
	{ 5151, "ANH_MON_B3_072_008" },
	{ 5210, "ANH_MON_B3_072_010" },
	{ 5218, "ANH_MON_B3_072_011" },
	{ 5163, "ANH_MON_B3_072_012" },
	{ 5171, "ANH_MON_B3_072_013" },
	{ 5229, "ANH_MON_B3_072_014" },
	{ 5173, "ANH_MON_B3_072_020" },
	{ 5212, "ANH_MON_B3_072_021" },
	{ 5157, "ANH_MON_B3_072_022" },
	{ 5206, "ANH_MON_B3_072_030" },
	{ 5214, "ANH_MON_B3_072_031" },
	{ 5159, "ANH_MON_B3_072_032" },
	{ 9248, "ANH_MON_B3_072_B" },
	{ 5155, "ANH_MON_B3_072_ILLUST" },
	{ 8583, "ANH_MON_B3_072_P" },
	{ 5167, "ANH_MON_B3_072_SOFT" },
	{ 5164, "ANH_MON_B3_072_TEST" },
	{ 6270, "ANH_MON_B3_073_000" },
	{ 6256, "ANH_MON_B3_073_002" },
	{ 6262, "ANH_MON_B3_073_003" },
	{ 6288, "ANH_MON_B3_073_004" },
	{ 6283, "ANH_MON_B3_073_007" },
	{ 6272, "ANH_MON_B3_073_010" },
	{ 6278, "ANH_MON_B3_073_011" },
	{ 6258, "ANH_MON_B3_073_012" },
	{ 6264, "ANH_MON_B3_073_013" },
	{ 6290, "ANH_MON_B3_073_014" },
	{ 6266, "ANH_MON_B3_073_020" },
	{ 6274, "ANH_MON_B3_073_021" },
	{ 6252, "ANH_MON_B3_073_022" },
	{ 6268, "ANH_MON_B3_073_030" },
	{ 6276, "ANH_MON_B3_073_031" },
	{ 6254, "ANH_MON_B3_073_032" },
	{ 6260, "ANH_MON_B3_073_033" },
	{ 6286, "ANH_MON_B3_073_034" },
	{ 6292, "ANH_MON_B3_073_035" },
	{ 6280, "ANH_MON_B3_073_036" },
	{ 6282, "ANH_MON_B3_073_037" },
	{ 6248, "ANH_MON_B3_073_038" },
	{ 9247, "ANH_MON_B3_073_B" },
	{ 8580, "ANH_MON_B3_073_P" },
	{ 5775, "ANH_MON_B3_074_000" },
	{ 5757, "ANH_MON_B3_074_002" },
	{ 5765, "ANH_MON_B3_074_003" },
	{ 5786, "ANH_MON_B3_074_007" },
	{ 5777, "ANH_MON_B3_074_010" },
	{ 5783, "ANH_MON_B3_074_011" },
	{ 5759, "ANH_MON_B3_074_012" },
	{ 5767, "ANH_MON_B3_074_013" },
	{ 5789, "ANH_MON_B3_074_014" },
	{ 5771, "ANH_MON_B3_074_020" },
	{ 5779, "ANH_MON_B3_074_021" },
	{ 5753, "ANH_MON_B3_074_022" },
	{ 5773, "ANH_MON_B3_074_030" },
	{ 5781, "ANH_MON_B3_074_031" },
	{ 5755, "ANH_MON_B3_074_032" },
	{ 5769, "ANH_MON_B3_074_B" },
	{ 5747, "ANH_MON_B3_074_ILLUST" },
	{ 5785, "ANH_MON_B3_074_P" },
	{ 5763, "ANH_MON_B3_074_SOFT" },
	{ 5751, "ANH_MON_B3_074_SOFT2" },
	{ 5760, "ANH_MON_B3_074_TEST" },
	{ 5749, "ANH_MON_B3_074_TEST2" },
	{ 13204, "ANH_MON_B3_075_000" },
	{ 13192, "ANH_MON_B3_075_002" },
	{ 13196, "ANH_MON_B3_075_003" },
	{ 13213, "ANH_MON_B3_075_007" },
	{ 13206, "ANH_MON_B3_075_010" },
	{ 13212, "ANH_MON_B3_075_011" },
	{ 13194, "ANH_MON_B3_075_012" },
	{ 13198, "ANH_MON_B3_075_013" },
	{ 13216, "ANH_MON_B3_075_014" },
	{ 13200, "ANH_MON_B3_075_020" },
	{ 13208, "ANH_MON_B3_075_021" },
	{ 13188, "ANH_MON_B3_075_022" },
	{ 13202, "ANH_MON_B3_075_030" },
	{ 13210, "ANH_MON_B3_075_031" },
	{ 13190, "ANH_MON_B3_075_032" },
	{ 9253, "ANH_MON_B3_075_B" },
	{ 13186, "ANH_MON_B3_075_ILLUST" },
	{ 8598, "ANH_MON_B3_075_P" },
	{ 13183, "ANH_MON_B3_075_TEST" },
	{ 7792, "ANH_MON_B3_076_000" },
	{ 7772, "ANH_MON_B3_076_002" },
	{ 7782, "ANH_MON_B3_076_003" },
	{ 7803, "ANH_MON_B3_076_007" },
	{ 7794, "ANH_MON_B3_076_010" },
	{ 7802, "ANH_MON_B3_076_011" },
	{ 7774, "ANH_MON_B3_076_012" },
	{ 7784, "ANH_MON_B3_076_013" },
	{ 7810, "ANH_MON_B3_076_014" },
	{ 7788, "ANH_MON_B3_076_020" },
	{ 7798, "ANH_MON_B3_076_021" },
	{ 7768, "ANH_MON_B3_076_022" },
	{ 7790, "ANH_MON_B3_076_030" },
	{ 7800, "ANH_MON_B3_076_031" },
	{ 7770, "ANH_MON_B3_076_032" },
	{ 7780, "ANH_MON_B3_076_033" },
	{ 7808, "ANH_MON_B3_076_034" },
	{ 7785, "ANH_MON_B3_076_040" },
	{ 7795, "ANH_MON_B3_076_041" },
	{ 7765, "ANH_MON_B3_076_042" },
	{ 7777, "ANH_MON_B3_076_043" },
	{ 7805, "ANH_MON_B3_076_044" },
	{ 9252, "ANH_MON_B3_076_B" },
	{ 7764, "ANH_MON_B3_076_ILLUST" },
	{ 8595, "ANH_MON_B3_076_P" },
	{ 7775, "ANH_MON_B3_076_TEST" },
	{ 5815, "ANH_MON_B3_077_000" },
	{ 5801, "ANH_MON_B3_077_002" },
	{ 5807, "ANH_MON_B3_077_003" },
	{ 5824, "ANH_MON_B3_077_007" },
	{ 5817, "ANH_MON_B3_077_010" },
	{ 5823, "ANH_MON_B3_077_011" },
	{ 5803, "ANH_MON_B3_077_012" },
	{ 5809, "ANH_MON_B3_077_013" },
	{ 5829, "ANH_MON_B3_077_014" },
	{ 5811, "ANH_MON_B3_077_020" },
	{ 5819, "ANH_MON_B3_077_021" },
	{ 5797, "ANH_MON_B3_077_022" },
	{ 5813, "ANH_MON_B3_077_030" },
	{ 5821, "ANH_MON_B3_077_031" },
	{ 5799, "ANH_MON_B3_077_032" },
	{ 5805, "ANH_MON_B3_077_033" },
	{ 5827, "ANH_MON_B3_077_034" },
	{ 9251, "ANH_MON_B3_077_B" },
	{ 5792, "ANH_MON_B3_077_BASE" },
	{ 5795, "ANH_MON_B3_077_ILLUST" },
	{ 8592, "ANH_MON_B3_077_P" },
	{ 5790, "ANH_MON_B3_077_TEST" },
	{ 9966, "ANH_MON_B3_078_000" },
	{ 9958, "ANH_MON_B3_078_002" },
	{ 9960, "ANH_MON_B3_078_003" },
	{ 9972, "ANH_MON_B3_078_007" },
	{ 9964, "ANH_MON_B3_078_020" },
	{ 9970, "ANH_MON_B3_078_021" },
	{ 9956, "ANH_MON_B3_078_022" },
	{ 9962, "ANH_MON_B3_078_040" },
	{ 9968, "ANH_MON_B3_078_041" },
	{ 9954, "ANH_MON_B3_078_042" },
	{ 9255, "ANH_MON_B3_078_B" },
	{ 9951, "ANH_MON_B3_078_ILLUST" },
	{ 8604, "ANH_MON_B3_078_P" },
	{ 9952, "ANH_MON_B3_078_TEST" },
	{ 5852, "ANH_MON_B3_079_000" },
	{ 5840, "ANH_MON_B3_079_002" },
	{ 5844, "ANH_MON_B3_079_003" },
	{ 5860, "ANH_MON_B3_079_007" },
	{ 5849, "ANH_MON_B3_079_020" },
	{ 5857, "ANH_MON_B3_079_021" },
	{ 5837, "ANH_MON_B3_079_022" },
	{ 5851, "ANH_MON_B3_079_030" },
	{ 5859, "ANH_MON_B3_079_031" },
	{ 5839, "ANH_MON_B3_079_032" },
	{ 5846, "ANH_MON_B3_079_040" },
	{ 5854, "ANH_MON_B3_079_041" },
	{ 5834, "ANH_MON_B3_079_042" },
	{ 5842, "ANH_MON_B3_079_043" },
	{ 5862, "ANH_MON_B3_079_044" },
	{ 9254, "ANH_MON_B3_079_B" },
	{ 5833, "ANH_MON_B3_079_ILLUST" },
	{ 8601, "ANH_MON_B3_079_P" },
	{ 5830, "ANH_MON_B3_079_TEST" },
	{ 10794, "ANH_MON_B3_080_000" },
	{ 10784, "ANH_MON_B3_080_002" },
	{ 10786, "ANH_MON_B3_080_003" },
	{ 10804, "ANH_MON_B3_080_007" },
	{ 10790, "ANH_MON_B3_080_020" },
	{ 10800, "ANH_MON_B3_080_021" },
	{ 10780, "ANH_MON_B3_080_022" },
	{ 10792, "ANH_MON_B3_080_030" },
	{ 10802, "ANH_MON_B3_080_031" },
	{ 10782, "ANH_MON_B3_080_032" },
	{ 10788, "ANH_MON_B3_080_040" },
	{ 10798, "ANH_MON_B3_080_041" },
	{ 9312, "ANH_MON_B3_080_B" },
	{ 10806, "ANH_MON_B3_080_BONESET" },
	{ 10797, "ANH_MON_B3_080_ILLUST" },
	{ 8749, "ANH_MON_B3_080_P" },
	{ 10808, "ANH_MON_B3_080_TEST" },
	{ 13289, "ANH_MON_B3_081_000" },
	{ 13265, "ANH_MON_B3_081_002" },
	{ 13277, "ANH_MON_B3_081_003" },
	{ 13307, "ANH_MON_B3_081_007" },
	{ 13291, "ANH_MON_B3_081_010" },
	{ 13304, "ANH_MON_B3_081_011" },
	{ 13267, "ANH_MON_B3_081_012" },
	{ 13279, "ANH_MON_B3_081_013" },
	{ 13316, "ANH_MON_B3_081_014" },
	{ 13318, "ANH_MON_B3_081_015" },
	{ 13306, "ANH_MON_B3_081_016" },
	{ 13285, "ANH_MON_B3_081_020" },
	{ 13299, "ANH_MON_B3_081_021" },
	{ 13261, "ANH_MON_B3_081_022" },
	{ 13287, "ANH_MON_B3_081_030" },
	{ 13301, "ANH_MON_B3_081_031" },
	{ 13263, "ANH_MON_B3_081_032" },
	{ 13276, "ANH_MON_B3_081_033" },
	{ 13314, "ANH_MON_B3_081_034" },
	{ 13281, "ANH_MON_B3_081_040" },
	{ 13295, "ANH_MON_B3_081_041" },
	{ 13257, "ANH_MON_B3_081_042" },
	{ 13271, "ANH_MON_B3_081_043" },
	{ 13309, "ANH_MON_B3_081_044" },
	{ 13283, "ANH_MON_B3_081_050" },
	{ 13297, "ANH_MON_B3_081_051" },
	{ 13259, "ANH_MON_B3_081_052" },
	{ 13273, "ANH_MON_B3_081_053" },
	{ 13311, "ANH_MON_B3_081_054" },
	{ 9311, "ANH_MON_B3_081_B" },
	{ 13294, "ANH_MON_B3_081_ILLUST" },
	{ 8746, "ANH_MON_B3_081_P" },
	{ 13269, "ANH_MON_B3_081_TEST" },
	{ 5257, "ANH_MON_B3_082_000" },
	{ 5265, "ANH_MON_B3_082_001" },
	{ 5245, "ANH_MON_B3_082_002" },
	{ 5249, "ANH_MON_B3_082_003" },
	{ 5271, "ANH_MON_B3_082_004" },
	{ 5273, "ANH_MON_B3_082_005" },
	{ 5267, "ANH_MON_B3_082_006" },
	{ 5284, "ANH_MON_B3_082_008" },
	{ 5253, "ANH_MON_B3_082_020" },
	{ 5261, "ANH_MON_B3_082_021" },
	{ 5240, "ANH_MON_B3_082_022" },
	{ 5255, "ANH_MON_B3_082_030" },
	{ 5263, "ANH_MON_B3_082_031" },
	{ 5243, "ANH_MON_B3_082_032" },
	{ 5248, "ANH_MON_B3_082_033" },
	{ 5270, "ANH_MON_B3_082_034" },
	{ 5251, "ANH_MON_B3_082_040" },
	{ 9310, "ANH_MON_B3_082_B" },
	{ 8743, "ANH_MON_B3_082_P" },
	{ 7869, "ANH_MON_B3_083_000" },
	{ 7853, "ANH_MON_B3_083_002" },
	{ 7861, "ANH_MON_B3_083_003" },
	{ 7881, "ANH_MON_B3_083_007" },
	{ 7871, "ANH_MON_B3_083_010" },
	{ 7880, "ANH_MON_B3_083_011" },
	{ 7855, "ANH_MON_B3_083_012" },
	{ 7863, "ANH_MON_B3_083_013" },
	{ 7888, "ANH_MON_B3_083_014" },
	{ 7865, "ANH_MON_B3_083_020" },
	{ 7875, "ANH_MON_B3_083_021" },
	{ 7849, "ANH_MON_B3_083_022" },
	{ 7867, "ANH_MON_B3_083_030" },
	{ 7877, "ANH_MON_B3_083_031" },
	{ 7851, "ANH_MON_B3_083_032" },
	{ 7860, "ANH_MON_B3_083_033" },
	{ 7886, "ANH_MON_B3_083_034" },
	{ 7889, "ANH_MON_B3_083_ATACK" },
	{ 9309, "ANH_MON_B3_083_B" },
	{ 7874, "ANH_MON_B3_083_ILLUST" },
	{ 8740, "ANH_MON_B3_083_P" },
	{ 7883, "ANH_MON_B3_083_SYAKU" },
	{ 7857, "ANH_MON_B3_083_TEST" },
	{ 5985, "ANH_MON_B3_084_000" },
	{ 5997, "ANH_MON_B3_084_001" },
	{ 5969, "ANH_MON_B3_084_002" },
	{ 5975, "ANH_MON_B3_084_003" },
	{ 6003, "ANH_MON_B3_084_004" },
	{ 6007, "ANH_MON_B3_084_005" },
	{ 6011, "ANH_MON_B3_084_008" },
	{ 5987, "ANH_MON_B3_084_010" },
	{ 5999, "ANH_MON_B3_084_011" },
	{ 5971, "ANH_MON_B3_084_012" },
	{ 5977, "ANH_MON_B3_084_013" },
	{ 6005, "ANH_MON_B3_084_014" },
	{ 5981, "ANH_MON_B3_084_020" },
	{ 5993, "ANH_MON_B3_084_021" },
	{ 5965, "ANH_MON_B3_084_022" },
	{ 5983, "ANH_MON_B3_084_030" },
	{ 5995, "ANH_MON_B3_084_031" },
	{ 5967, "ANH_MON_B3_084_032" },
	{ 5974, "ANH_MON_B3_084_033" },
	{ 6002, "ANH_MON_B3_084_034" },
	{ 5979, "ANH_MON_B3_084_040" },
	{ 5991, "ANH_MON_B3_084_041" },
	{ 5963, "ANH_MON_B3_084_042" },
	{ 9316, "ANH_MON_B3_084_B" },
	{ 5990, "ANH_MON_B3_084_ILLUST" },
	{ 8761, "ANH_MON_B3_084_P" },
	{ 6009, "ANH_MON_B3_084_TEST" },
	{ 3205, "ANH_MON_B3_085_000" },
	{ 3147, "ANH_MON_B3_085_002" },
	{ 3155, "ANH_MON_B3_085_003" },
	{ 3219, "ANH_MON_B3_085_007" },
	{ 3207, "ANH_MON_B3_085_010" },
	{ 3218, "ANH_MON_B3_085_011" },
	{ 3149, "ANH_MON_B3_085_012" },
	{ 3157, "ANH_MON_B3_085_013" },
	{ 3224, "ANH_MON_B3_085_014" },
	{ 3161, "ANH_MON_B3_085_020" },
	{ 3213, "ANH_MON_B3_085_021" },
	{ 3143, "ANH_MON_B3_085_022" },
	{ 3163, "ANH_MON_B3_085_030" },
	{ 3215, "ANH_MON_B3_085_031" },
	{ 3145, "ANH_MON_B3_085_032" },
	{ 3154, "ANH_MON_B3_085_033" },
	{ 3222, "ANH_MON_B3_085_034" },
	{ 3159, "ANH_MON_B3_085_040" },
	{ 3211, "ANH_MON_B3_085_041" },
	{ 3141, "ANH_MON_B3_085_042" },
	{ 9315, "ANH_MON_B3_085_B" },
	{ 3225, "ANH_MON_B3_085_ICHIDA" },
	{ 3210, "ANH_MON_B3_085_ILLUST" },
	{ 8758, "ANH_MON_B3_085_P" },
	{ 3151, "ANH_MON_B3_085_TEST" },
	{ 5319, "ANH_MON_B3_086_000" },
	{ 5301, "ANH_MON_B3_086_002" },
	{ 5309, "ANH_MON_B3_086_003" },
	{ 5333, "ANH_MON_B3_086_007" },
	{ 5321, "ANH_MON_B3_086_010" },
	{ 5332, "ANH_MON_B3_086_011" },
	{ 5303, "ANH_MON_B3_086_012" },
	{ 5311, "ANH_MON_B3_086_013" },
	{ 5340, "ANH_MON_B3_086_014" },
	{ 5315, "ANH_MON_B3_086_020" },
	{ 5327, "ANH_MON_B3_086_021" },
	{ 5288, "ANH_MON_B3_086_022" },
	{ 5317, "ANH_MON_B3_086_030" },
	{ 5329, "ANH_MON_B3_086_031" },
	{ 5290, "ANH_MON_B3_086_032" },
	{ 5308, "ANH_MON_B3_086_033" },
	{ 5338, "ANH_MON_B3_086_034" },
	{ 5313, "ANH_MON_B3_086_040" },
	{ 5325, "ANH_MON_B3_086_041" },
	{ 5286, "ANH_MON_B3_086_042" },
	{ 5305, "ANH_MON_B3_086_043" },
	{ 5335, "ANH_MON_B3_086_044" },
	{ 9314, "ANH_MON_B3_086_B" },
	{ 8755, "ANH_MON_B3_086_P" },
	{ 2564, "ANH_MON_B3_087_000" },
	{ 2566, "ANH_MON_B3_087_002" },
	{ 2864, "ANH_MON_B3_087_003" },
	{ 2866, "ANH_MON_B3_087_007" },
	{ 5069, "ANH_MON_B3_087_020" },
	{ 5071, "ANH_MON_B3_087_021" },
	{ 5073, "ANH_MON_B3_087_022" },
	{ 5075, "ANH_MON_B3_087_030" },
	{ 5077, "ANH_MON_B3_087_031" },
	{ 5079, "ANH_MON_B3_087_032" },
	{ 7246, "ANH_MON_B3_087_033" },
	{ 10505, "ANH_MON_B3_087_034" },
	{ 12908, "ANH_MON_B3_087_040" },
	{ 13628, "ANH_MON_B3_087_041" },
	{ 14549, "ANH_MON_B3_087_042" },
	{ 14551, "ANH_MON_B3_087_043" },
	{ 14553, "ANH_MON_B3_087_044" },
	{ 14555, "ANH_MON_B3_087_050" },
	{ 14557, "ANH_MON_B3_087_051" },
	{ 14559, "ANH_MON_B3_087_052" },
	{ 14561, "ANH_MON_B3_087_053" },
	{ 14563, "ANH_MON_B3_087_054" },
	{ 14565, "ANH_MON_B3_087_060" },
	{ 14567, "ANH_MON_B3_087_061" },
	{ 14569, "ANH_MON_B3_087_062" },
	{ 14571, "ANH_MON_B3_087_063" },
	{ 14573, "ANH_MON_B3_087_064" },
	{ 14575, "ANH_MON_B3_087_070" },
	{ 14577, "ANH_MON_B3_087_071" },
	{ 14579, "ANH_MON_B3_087_072" },
	{ 14581, "ANH_MON_B3_087_073" },
	{ 14583, "ANH_MON_B3_087_074" },
	{ 9313, "ANH_MON_B3_087_B" },
	{ 8752, "ANH_MON_B3_087_P" },
	{ 5890, "ANH_MON_B3_088_000" },
	{ 5870, "ANH_MON_B3_088_002" },
	{ 5878, "ANH_MON_B3_088_003" },
	{ 5902, "ANH_MON_B3_088_007" },
	{ 5892, "ANH_MON_B3_088_010" },
	{ 5901, "ANH_MON_B3_088_011" },
	{ 5872, "ANH_MON_B3_088_012" },
	{ 5880, "ANH_MON_B3_088_013" },
	{ 5909, "ANH_MON_B3_088_014" },
	{ 5886, "ANH_MON_B3_088_020" },
	{ 5896, "ANH_MON_B3_088_021" },
	{ 5866, "ANH_MON_B3_088_022" },
	{ 5888, "ANH_MON_B3_088_030" },
	{ 5898, "ANH_MON_B3_088_031" },
	{ 5868, "ANH_MON_B3_088_032" },
	{ 5877, "ANH_MON_B3_088_033" },
	{ 5907, "ANH_MON_B3_088_034" },
	{ 5884, "ANH_MON_B3_088_040" },
	{ 5894, "ANH_MON_B3_088_041" },
	{ 5864, "ANH_MON_B3_088_042" },
	{ 5874, "ANH_MON_B3_088_043" },
	{ 5904, "ANH_MON_B3_088_044" },
	{ 9317, "ANH_MON_B3_088_B" },
	{ 8764, "ANH_MON_B3_088_P" },
	{ 6035, "ANH_MON_B3_092_000" },
	{ 6021, "ANH_MON_B3_092_002" },
	{ 6027, "ANH_MON_B3_092_003" },
	{ 6043, "ANH_MON_B3_092_007" },
	{ 6031, "ANH_MON_B3_092_020" },
	{ 6039, "ANH_MON_B3_092_021" },
	{ 6017, "ANH_MON_B3_092_022" },
	{ 6033, "ANH_MON_B3_092_030" },
	{ 6041, "ANH_MON_B3_092_031" },
	{ 6019, "ANH_MON_B3_092_032" },
	{ 6029, "ANH_MON_B3_092_040" },
	{ 6037, "ANH_MON_B3_092_041" },
	{ 6015, "ANH_MON_B3_092_042" },
	{ 9319, "ANH_MON_B3_092_B" },
	{ 6014, "ANH_MON_B3_092_ILLUST" },
	{ 8770, "ANH_MON_B3_092_P" },
	{ 6026, "ANH_MON_B3_092_SOFT" },
	{ 6023, "ANH_MON_B3_092_TEST" },
	{ 13341, "ANH_MON_B3_093_000" },
	{ 13327, "ANH_MON_B3_093_002" },
	{ 13333, "ANH_MON_B3_093_003" },
	{ 13353, "ANH_MON_B3_093_007" },
	{ 13343, "ANH_MON_B3_093_010" },
	{ 13350, "ANH_MON_B3_093_011" },
	{ 13329, "ANH_MON_B3_093_012" },
	{ 13335, "ANH_MON_B3_093_013" },
	{ 13358, "ANH_MON_B3_093_014" },
	{ 13337, "ANH_MON_B3_093_020" },
	{ 13345, "ANH_MON_B3_093_021" },
	{ 13323, "ANH_MON_B3_093_022" },
	{ 13339, "ANH_MON_B3_093_030" },
	{ 13347, "ANH_MON_B3_093_031" },
	{ 13325, "ANH_MON_B3_093_032" },
	{ 13332, "ANH_MON_B3_093_033" },
	{ 13356, "ANH_MON_B3_093_034" },
	{ 13360, "ANH_MON_B3_093_035" },
	{ 13352, "ANH_MON_B3_093_036" },
	{ 9318, "ANH_MON_B3_093_B" },
	{ 13322, "ANH_MON_B3_093_ILLUST" },
	{ 8767, "ANH_MON_B3_093_P" },
	{ 13319, "ANH_MON_B3_093_TEST" },
	{ 6369, "ANH_MON_B3_094_000" },
	{ 6349, "ANH_MON_B3_094_002" },
	{ 6359, "ANH_MON_B3_094_003" },
	{ 6381, "ANH_MON_B3_094_007" },
	{ 6371, "ANH_MON_B3_094_010" },
	{ 6380, "ANH_MON_B3_094_011" },
	{ 6351, "ANH_MON_B3_094_012" },
	{ 6361, "ANH_MON_B3_094_013" },
	{ 6388, "ANH_MON_B3_094_014" },
	{ 6365, "ANH_MON_B3_094_020" },
	{ 6375, "ANH_MON_B3_094_021" },
	{ 6345, "ANH_MON_B3_094_022" },
	{ 6367, "ANH_MON_B3_094_030" },
	{ 6377, "ANH_MON_B3_094_031" },
	{ 6347, "ANH_MON_B3_094_032" },
	{ 6358, "ANH_MON_B3_094_033" },
	{ 6386, "ANH_MON_B3_094_034" },
	{ 6363, "ANH_MON_B3_094_040" },
	{ 6373, "ANH_MON_B3_094_041" },
	{ 6343, "ANH_MON_B3_094_042" },
	{ 6355, "ANH_MON_B3_094_043" },
	{ 6383, "ANH_MON_B3_094_044" },
	{ 9320, "ANH_MON_B3_094_B" },
	{ 6342, "ANH_MON_B3_094_ILLUST" },
	{ 8773, "ANH_MON_B3_094_P" },
	{ 6353, "ANH_MON_B3_094_TEST" },
	{ 7905, "ANH_MON_B3_102_000" },
	{ 7911, "ANH_MON_B3_102_001" },
	{ 7893, "ANH_MON_B3_102_002" },
	{ 7897, "ANH_MON_B3_102_003" },
	{ 7917, "ANH_MON_B3_102_004" },
	{ 7921, "ANH_MON_B3_102_005" },
	{ 7915, "ANH_MON_B3_102_006" },
	{ 7925, "ANH_MON_B3_102_008" },
	{ 7907, "ANH_MON_B3_102_010" },
	{ 7913, "ANH_MON_B3_102_011" },
	{ 7895, "ANH_MON_B3_102_012" },
	{ 7899, "ANH_MON_B3_102_013" },
	{ 7919, "ANH_MON_B3_102_014" },
	{ 7903, "ANH_MON_B3_102_020" },
	{ 7909, "ANH_MON_B3_102_021" },
	{ 7891, "ANH_MON_B3_102_022" },
	{ 9345, "ANH_MON_B3_102_B" },
	{ 7902, "ANH_MON_B3_102_ILLUST" },
	{ 8848, "ANH_MON_B3_102_P" },
	{ 7923, "ANH_MON_B3_102_TEST" },
	{ 6405, "ANH_MON_B3_103_000" },
	{ 6391, "ANH_MON_B3_103_002" },
	{ 6397, "ANH_MON_B3_103_003" },
	{ 6413, "ANH_MON_B3_103_007" },
	{ 6407, "ANH_MON_B3_103_010" },
	{ 6412, "ANH_MON_B3_103_011" },
	{ 6393, "ANH_MON_B3_103_012" },
	{ 6399, "ANH_MON_B3_103_013" },
	{ 6416, "ANH_MON_B3_103_014" },
	{ 6403, "ANH_MON_B3_103_020" },
	{ 6409, "ANH_MON_B3_103_021" },
	{ 6389, "ANH_MON_B3_103_022" },
	{ 9344, "ANH_MON_B3_103_B" },
	{ 6417, "ANH_MON_B3_103_ICHIDA" },
	{ 6402, "ANH_MON_B3_103_ILLUST" },
	{ 8845, "ANH_MON_B3_103_P" },
	{ 6395, "ANH_MON_B3_103_TEST" },
	{ 7569, "ANH_MON_B3_105_000" },
	{ 7551, "ANH_MON_B3_105_002" },
	{ 7559, "ANH_MON_B3_105_003" },
	{ 7581, "ANH_MON_B3_105_007" },
	{ 7591, "ANH_MON_B3_105_008" },
	{ 7571, "ANH_MON_B3_105_010" },
	{ 7580, "ANH_MON_B3_105_011" },
	{ 7658, "ANH_MON_B3_105_012" },
	{ 7561, "ANH_MON_B3_105_013" },
	{ 7590, "ANH_MON_B3_105_014" },
	{ 7568, "ANH_MON_B3_105_030" },
	{ 7578, "ANH_MON_B3_105_031" },
	{ 7550, "ANH_MON_B3_105_032" },
	{ 7558, "ANH_MON_B3_105_033" },
	{ 7588, "ANH_MON_B3_105_034" },
	{ 7563, "ANH_MON_B3_105_040" },
	{ 7573, "ANH_MON_B3_105_041" },
	{ 7545, "ANH_MON_B3_105_042" },
	{ 7553, "ANH_MON_B3_105_043" },
	{ 7583, "ANH_MON_B3_105_044" },
	{ 7565, "ANH_MON_B3_105_050" },
	{ 7575, "ANH_MON_B3_105_051" },
	{ 7547, "ANH_MON_B3_105_052" },
	{ 7555, "ANH_MON_B3_105_053" },
	{ 7585, "ANH_MON_B3_105_054" },
	{ 9347, "ANH_MON_B3_105_B" },
	{ 8854, "ANH_MON_B3_105_P" },
	{ 7639, "ANH_MON_B3_106_000" },
	{ 7629, "ANH_MON_B3_106_002" },
	{ 7633, "ANH_MON_B3_106_003" },
	{ 7647, "ANH_MON_B3_106_007" },
	{ 7641, "ANH_MON_B3_106_010" },
	{ 7646, "ANH_MON_B3_106_011" },
	{ 7631, "ANH_MON_B3_106_012" },
	{ 7635, "ANH_MON_B3_106_013" },
	{ 7650, "ANH_MON_B3_106_014" },
	{ 7637, "ANH_MON_B3_106_020" },
	{ 7643, "ANH_MON_B3_106_021" },
	{ 7627, "ANH_MON_B3_106_022" },
	{ 9346, "ANH_MON_B3_106_B" },
	{ 8851, "ANH_MON_B3_106_P" },
	{ 7609, "ANH_MON_B3_107_000" },
	{ 7595, "ANH_MON_B3_107_002" },
	{ 7601, "ANH_MON_B3_107_003" },
	{ 7619, "ANH_MON_B3_107_007" },
	{ 7625, "ANH_MON_B3_107_008" },
	{ 7611, "ANH_MON_B3_107_010" },
	{ 7618, "ANH_MON_B3_107_011" },
	{ 7597, "ANH_MON_B3_107_012" },
	{ 7603, "ANH_MON_B3_107_013" },
	{ 7624, "ANH_MON_B3_107_014" },
	{ 7608, "ANH_MON_B3_107_030" },
	{ 7616, "ANH_MON_B3_107_031" },
	{ 7594, "ANH_MON_B3_107_032" },
	{ 7600, "ANH_MON_B3_107_033" },
	{ 7622, "ANH_MON_B3_107_034" },
	{ 7605, "ANH_MON_B3_107_040" },
	{ 7613, "ANH_MON_B3_107_041" },
	{ 7651, "ANH_MON_B3_107_042" },
	{ 7653, "ANH_MON_B3_107_043" },
	{ 7655, "ANH_MON_B3_107_044" },
	{ 10283, "ANH_MON_B3_107_B" },
	{ 10285, "ANH_MON_B3_107_P" },
	{ 7987, "ANH_MON_B3_108_000" },
	{ 7975, "ANH_MON_B3_108_002" },
	{ 7979, "ANH_MON_B3_108_003" },
	{ 7995, "ANH_MON_B3_108_007" },
	{ 7989, "ANH_MON_B3_108_010" },
	{ 7994, "ANH_MON_B3_108_011" },
	{ 7977, "ANH_MON_B3_108_012" },
	{ 7981, "ANH_MON_B3_108_013" },
	{ 7998, "ANH_MON_B3_108_014" },
	{ 7985, "ANH_MON_B3_108_020" },
	{ 7991, "ANH_MON_B3_108_021" },
	{ 7973, "ANH_MON_B3_108_022" },
	{ 9349, "ANH_MON_B3_108_B" },
	{ 7984, "ANH_MON_B3_108_ILLUST" },
	{ 8860, "ANH_MON_B3_108_P" },
	{ 7999, "ANH_MON_B3_108_TEST" },
	{ 11536, "ANH_MON_B3_109_000" },
	{ 11544, "ANH_MON_B3_109_001" },
	{ 11520, "ANH_MON_B3_109_002" },
	{ 11528, "ANH_MON_B3_109_003" },
	{ 11570, "ANH_MON_B3_109_004" },
	{ 11578, "ANH_MON_B3_109_005" },
	{ 11548, "ANH_MON_B3_109_006" },
	{ 11566, "ANH_MON_B3_109_007" },
	{ 11580, "ANH_MON_B3_109_008" },
	{ 6910, "ANH_MON_B3_109_009" },
	{ 11538, "ANH_MON_B3_109_010" },
	{ 11546, "ANH_MON_B3_109_011" },
	{ 11524, "ANH_MON_B3_109_012" },
	{ 11530, "ANH_MON_B3_109_013" },
	{ 11576, "ANH_MON_B3_109_014" },
	{ 11534, "ANH_MON_B3_109_020" },
	{ 11542, "ANH_MON_B3_109_021" },
	{ 5500, "ANH_MON_B3_109_022" },
	{ 5497, "ANH_MON_B3_109_030" },
	{ 5499, "ANH_MON_B3_109_031" },
	{ 11518, "ANH_MON_B3_109_032" },
	{ 11527, "ANH_MON_B3_109_033" },
	{ 11569, "ANH_MON_B3_109_034" },
	{ 11532, "ANH_MON_B3_109_040" },
	{ 11540, "ANH_MON_B3_109_041" },
	{ 9348, "ANH_MON_B3_109_B" },
	{ 8857, "ANH_MON_B3_109_P" },
	{ 10090, "ANH_MON_B3_110_000" },
	{ 10074, "ANH_MON_B3_110_002" },
	{ 10082, "ANH_MON_B3_110_003" },
	{ 10102, "ANH_MON_B3_110_007" },
	{ 10092, "ANH_MON_B3_110_010" },
	{ 10101, "ANH_MON_B3_110_011" },
	{ 10076, "ANH_MON_B3_110_012" },
	{ 10086, "ANH_MON_B3_110_020" },
	{ 10096, "ANH_MON_B3_110_021" },
	{ 10070, "ANH_MON_B3_110_022" },
	{ 10088, "ANH_MON_B3_110_030" },
	{ 10098, "ANH_MON_B3_110_031" },
	{ 10072, "ANH_MON_B3_110_032" },
	{ 10084, "ANH_MON_B3_110_040" },
	{ 10094, "ANH_MON_B3_110_041" },
	{ 10068, "ANH_MON_B3_110_042" },
	{ 10080, "ANH_MON_B3_110_043" },
	{ 10104, "ANH_MON_B3_110_044" },
	{ 9351, "ANH_MON_B3_110_B" },
	{ 10067, "ANH_MON_B3_110_ILLUST" },
	{ 8866, "ANH_MON_B3_110_P" },
	{ 10078, "ANH_MON_B3_110_TEST" },
	{ 11298, "ANH_MON_B3_111_000" },
	{ 11286, "ANH_MON_B3_111_002" },
	{ 11292, "ANH_MON_B3_111_003" },
	{ 11308, "ANH_MON_B3_111_007" },
	{ 11300, "ANH_MON_B3_111_010" },
	{ 11307, "ANH_MON_B3_111_011" },
	{ 11288, "ANH_MON_B3_111_012" },
	{ 11294, "ANH_MON_B3_111_013" },
	{ 11311, "ANH_MON_B3_111_014" },
	{ 11296, "ANH_MON_B3_111_020" },
	{ 11304, "ANH_MON_B3_111_021" },
	{ 11284, "ANH_MON_B3_111_022" },
	{ 7549, "ANH_MON_B3_111_030" },
	{ 7546, "ANH_MON_B3_111_031" },
	{ 7548, "ANH_MON_B3_111_032" },
	{ 8579, "ANH_MON_B3_111_033" },
	{ 8581, "ANH_MON_B3_111_034" },
	{ 8228, "ANH_MON_B3_111_040" },
	{ 6615, "ANH_MON_B3_111_041" },
	{ 11291, "ANH_MON_B3_111_B" },
	{ 11303, "ANH_MON_B3_111_P" },
	{ 13611, "ANH_MON_B3_112_000" },
	{ 13597, "ANH_MON_B3_112_002" },
	{ 13603, "ANH_MON_B3_112_003" },
	{ 13621, "ANH_MON_B3_112_007" },
	{ 13613, "ANH_MON_B3_112_010" },
	{ 13620, "ANH_MON_B3_112_011" },
	{ 13599, "ANH_MON_B3_112_012" },
	{ 13605, "ANH_MON_B3_112_013" },
	{ 13626, "ANH_MON_B3_112_014" },
	{ 13607, "ANH_MON_B3_112_020" },
	{ 13615, "ANH_MON_B3_112_021" },
	{ 13593, "ANH_MON_B3_112_022" },
	{ 13609, "ANH_MON_B3_112_030" },
	{ 13617, "ANH_MON_B3_112_031" },
	{ 13595, "ANH_MON_B3_112_032" },
	{ 14395, "ANH_MON_B3_112_033" },
	{ 13624, "ANH_MON_B3_112_034" },
	{ 14601, "ANH_MON_B3_112_035" },
	{ 9350, "ANH_MON_B3_112_B" },
	{ 13592, "ANH_MON_B3_112_ILLUST" },
	{ 8863, "ANH_MON_B3_112_P" },
	{ 13601, "ANH_MON_B3_112_TEST" },
	{ 11278, "ANH_MON_B3_113_000" },
	{ 11272, "ANH_MON_B3_113_002" },
	{ 11274, "ANH_MON_B3_113_003" },
	{ 11282, "ANH_MON_B3_113_007" },
	{ 11276, "ANH_MON_B3_113_020" },
	{ 11280, "ANH_MON_B3_113_021" },
	{ 13078, "ANH_MON_B3_113_022" },
	{ 2317, "ANH_MON_B3_113_030" },
	{ 6608, "ANH_MON_B3_113_031" },
	{ 14733, "ANH_MON_B3_113_B" },
	{ 14735, "ANH_MON_B3_113_P" },
	{ 11118, "ANH_MON_B3_114_000" },
	{ 11106, "ANH_MON_B3_114_003" },
	{ 11114, "ANH_MON_B3_114_020" },
	{ 11126, "ANH_MON_B3_114_021" },
	{ 11098, "ANH_MON_B3_114_022" },
	{ 11116, "ANH_MON_B3_114_030" },
	{ 11128, "ANH_MON_B3_114_031" },
	{ 11100, "ANH_MON_B3_114_032" },
	{ 11105, "ANH_MON_B3_114_033" },
	{ 11133, "ANH_MON_B3_114_034" },
	{ 11110, "ANH_MON_B3_114_040" },
	{ 11122, "ANH_MON_B3_114_041" },
	{ 11096, "ANH_MON_B3_114_042" },
	{ 11102, "ANH_MON_B3_114_043" },
	{ 11130, "ANH_MON_B3_114_044" },
	{ 11112, "ANH_MON_B3_114_050" },
	{ 11124, "ANH_MON_B3_114_051" },
	{ 11108, "ANH_MON_B3_114_060" },
	{ 11120, "ANH_MON_B3_114_061" },
	{ 11094, "ANH_MON_B3_114_062" },
	{ 14603, "ANH_MON_B3_114_063" },
	{ 9353, "ANH_MON_B3_114_B" },
	{ 8872, "ANH_MON_B3_114_P" },
	{ 11366, "ANH_MON_B3_115_000" },
	{ 11376, "ANH_MON_B3_115_001" },
	{ 11348, "ANH_MON_B3_115_002" },
	{ 11354, "ANH_MON_B3_115_003" },
	{ 11384, "ANH_MON_B3_115_004" },
	{ 11388, "ANH_MON_B3_115_005" },
	{ 11960, "ANH_MON_B3_115_006" },
	{ 8717, "ANH_MON_B3_115_007" },
	{ 11340, "ANH_MON_B3_115_008" },
	{ 11368, "ANH_MON_B3_115_010" },
	{ 11378, "ANH_MON_B3_115_011" },
	{ 11350, "ANH_MON_B3_115_012" },
	{ 11356, "ANH_MON_B3_115_013" },
	{ 11386, "ANH_MON_B3_115_014" },
	{ 11362, "ANH_MON_B3_115_020" },
	{ 11372, "ANH_MON_B3_115_021" },
	{ 11344, "ANH_MON_B3_115_022" },
	{ 11360, "ANH_MON_B3_115_040" },
	{ 11370, "ANH_MON_B3_115_041" },
	{ 11342, "ANH_MON_B3_115_042" },
	{ 11359, "ANH_MON_B3_115_B" },
	{ 11381, "ANH_MON_B3_115_P" },
	{ 11326, "ANH_MON_B3_116_000" },
	{ 11314, "ANH_MON_B3_116_002" },
	{ 11318, "ANH_MON_B3_116_003" },
	{ 11336, "ANH_MON_B3_116_007" },
	{ 11328, "ANH_MON_B3_116_010" },
	{ 11333, "ANH_MON_B3_116_011" },
	{ 11316, "ANH_MON_B3_116_012" },
	{ 11320, "ANH_MON_B3_116_013" },
	{ 11339, "ANH_MON_B3_116_014" },
	{ 11324, "ANH_MON_B3_116_020" },
	{ 11330, "ANH_MON_B3_116_021" },
	{ 11312, "ANH_MON_B3_116_022" },
	{ 11323, "ANH_MON_B3_116_B" },
	{ 11335, "ANH_MON_B3_116_P" },
	{ 11078, "ANH_MON_B3_117_000" },
	{ 11068, "ANH_MON_B3_117_003" },
	{ 11080, "ANH_MON_B3_117_010" },
	{ 11089, "ANH_MON_B3_117_011" },
	{ 11065, "ANH_MON_B3_117_012" },
	{ 11070, "ANH_MON_B3_117_013" },
	{ 11093, "ANH_MON_B3_117_014" },
	{ 11074, "ANH_MON_B3_117_020" },
	{ 11084, "ANH_MON_B3_117_021" },
	{ 11060, "ANH_MON_B3_117_022" },
	{ 11076, "ANH_MON_B3_117_030" },
	{ 11086, "ANH_MON_B3_117_031" },
	{ 11062, "ANH_MON_B3_117_032" },
	{ 11067, "ANH_MON_B3_117_033" },
	{ 11091, "ANH_MON_B3_117_034" },
	{ 11072, "ANH_MON_B3_117_040" },
	{ 11082, "ANH_MON_B3_117_041" },
	{ 9352, "ANH_MON_B3_117_B" },
	{ 11059, "ANH_MON_B3_117_ILLUST" },
	{ 8869, "ANH_MON_B3_117_P" },
	{ 11056, "ANH_MON_B3_117_TEST" },
	{ 10134, "ANH_MON_B3_118_000" },
	{ 10122, "ANH_MON_B3_118_003" },
	{ 10136, "ANH_MON_B3_118_010" },
	{ 10130, "ANH_MON_B3_118_020" },
	{ 10144, "ANH_MON_B3_118_021" },
	{ 10116, "ANH_MON_B3_118_022" },
	{ 10132, "ANH_MON_B3_118_030" },
	{ 10129, "ANH_MON_B3_118_050" },
	{ 10143, "ANH_MON_B3_118_051" },
	{ 10115, "ANH_MON_B3_118_052" },
	{ 10121, "ANH_MON_B3_118_053" },
	{ 10149, "ANH_MON_B3_118_054" },
	{ 10124, "ANH_MON_B3_118_060" },
	{ 10138, "ANH_MON_B3_118_061" },
	{ 10110, "ANH_MON_B3_118_062" },
	{ 10118, "ANH_MON_B3_118_063" },
	{ 10146, "ANH_MON_B3_118_064" },
	{ 10126, "ANH_MON_B3_118_070" },
	{ 10140, "ANH_MON_B3_118_071" },
	{ 10112, "ANH_MON_B3_118_072" },
	{ 14605, "ANH_MON_B3_118_073" },
	{ 9354, "ANH_MON_B3_118_B" },
	{ 10107, "ANH_MON_B3_118_ILLUST" },
	{ 8875, "ANH_MON_B3_118_P" },
	{ 10108, "ANH_MON_B3_118_TEST" },
	{ 10730, "ANH_MON_B3_119_000" },
	{ 10720, "ANH_MON_B3_119_002" },
	{ 10724, "ANH_MON_B3_119_003" },
	{ 10738, "ANH_MON_B3_119_007" },
	{ 10728, "ANH_MON_B3_119_020" },
	{ 10736, "ANH_MON_B3_119_021" },
	{ 10718, "ANH_MON_B3_119_022" },
	{ 10726, "ANH_MON_B3_119_040" },
	{ 10734, "ANH_MON_B3_119_041" },
	{ 10716, "ANH_MON_B3_119_042" },
	{ 2992, "ANH_MON_B3_119_050" },
	{ 10723, "ANH_MON_B3_119_B" },
	{ 10733, "ANH_MON_B3_119_P" },
	{ 10704, "ANH_MON_B3_120_000" },
	{ 10696, "ANH_MON_B3_120_002" },
	{ 10698, "ANH_MON_B3_120_003" },
	{ 10710, "ANH_MON_B3_120_007" },
	{ 10702, "ANH_MON_B3_120_020" },
	{ 10708, "ANH_MON_B3_120_021" },
	{ 10694, "ANH_MON_B3_120_022" },
	{ 10700, "ANH_MON_B3_120_040" },
	{ 10706, "ANH_MON_B3_120_041" },
	{ 10692, "ANH_MON_B3_120_042" },
	{ 2990, "ANH_MON_B3_120_050" },
	{ 10713, "ANH_MON_B3_120_B" },
	{ 10715, "ANH_MON_B3_120_P" },
	{ 10935, "ANH_MON_B3_121_000" },
	{ 10945, "ANH_MON_B3_121_001" },
	{ 10915, "ANH_MON_B3_121_002" },
	{ 10923, "ANH_MON_B3_121_003" },
	{ 10953, "ANH_MON_B3_121_004" },
	{ 10956, "ANH_MON_B3_121_005" },
	{ 10948, "ANH_MON_B3_121_006" },
	{ 10958, "ANH_MON_B3_121_008" },
	{ 10937, "ANH_MON_B3_121_010" },
	{ 10947, "ANH_MON_B3_121_011" },
	{ 10917, "ANH_MON_B3_121_012" },
	{ 10925, "ANH_MON_B3_121_013" },
	{ 10955, "ANH_MON_B3_121_014" },
	{ 10931, "ANH_MON_B3_121_020" },
	{ 10941, "ANH_MON_B3_121_021" },
	{ 10911, "ANH_MON_B3_121_022" },
	{ 10933, "ANH_MON_B3_121_030" },
	{ 10943, "ANH_MON_B3_121_031" },
	{ 10913, "ANH_MON_B3_121_032" },
	{ 10921, "ANH_MON_B3_121_033" },
	{ 10951, "ANH_MON_B3_121_034" },
	{ 10928, "ANH_MON_B3_121_040" },
	{ 10938, "ANH_MON_B3_121_041" },
	{ 10908, "ANH_MON_B3_121_042" },
	{ 9335, "ANH_MON_B3_121_B" },
	{ 10927, "ANH_MON_B3_121_ILLUST" },
	{ 8818, "ANH_MON_B3_121_P" },
	{ 10918, "ANH_MON_B3_121_TEST" },
	{ 14739, "ANH_MON_B3_122_000" },
	{ 14737, "ANH_MON_B3_122_003" },
	{ 33, "ANH_MON_B3_122_010" },
	{ 35, "ANH_MON_B3_122_011" },
	{ 37, "ANH_MON_B3_122_012" },
	{ 42, "ANH_MON_B3_122_013" },
	{ 44, "ANH_MON_B3_122_014" },
	{ 3468, "ANH_MON_B3_122_020" },
	{ 3470, "ANH_MON_B3_122_021" },
	{ 3472, "ANH_MON_B3_122_022" },
	{ 4148, "ANH_MON_B3_122_040" },
	{ 4456, "ANH_MON_B3_122_041" },
	{ 4749, "ANH_MON_B3_122_042" },
	{ 4762, "ANH_MON_B3_122_043" },
	{ 10409, "ANH_MON_B3_122_B" },
	{ 10411, "ANH_MON_B3_122_P" },
	{ 10889, "ANH_MON_B3_123_000" },
	{ 10867, "ANH_MON_B3_123_002" },
	{ 10877, "ANH_MON_B3_123_003" },
	{ 10900, "ANH_MON_B3_123_007" },
	{ 10891, "ANH_MON_B3_123_010" },
	{ 10899, "ANH_MON_B3_123_011" },
	{ 10869, "ANH_MON_B3_123_012" },
	{ 10879, "ANH_MON_B3_123_013" },
	{ 10907, "ANH_MON_B3_123_014" },
	{ 10885, "ANH_MON_B3_123_020" },
	{ 10895, "ANH_MON_B3_123_021" },
	{ 10863, "ANH_MON_B3_123_022" },
	{ 10887, "ANH_MON_B3_123_030" },
	{ 10897, "ANH_MON_B3_123_031" },
	{ 10865, "ANH_MON_B3_123_032" },
	{ 10875, "ANH_MON_B3_123_033" },
	{ 10905, "ANH_MON_B3_123_034" },
	{ 10882, "ANH_MON_B3_123_040" },
	{ 10892, "ANH_MON_B3_123_041" },
	{ 10860, "ANH_MON_B3_123_042" },
	{ 10872, "ANH_MON_B3_123_043" },
	{ 10902, "ANH_MON_B3_123_044" },
	{ 9334, "ANH_MON_B3_123_B" },
	{ 10881, "ANH_MON_B3_123_ILLUST" },
	{ 8815, "ANH_MON_B3_123_P" },
	{ 10870, "ANH_MON_B3_123_TEST" },
	{ 13561, "ANH_MON_B3_124_000" },
	{ 13545, "ANH_MON_B3_124_002" },
	{ 13551, "ANH_MON_B3_124_003" },
	{ 13569, "ANH_MON_B3_124_007" },
	{ 13558, "ANH_MON_B3_124_020" },
	{ 13566, "ANH_MON_B3_124_021" },
	{ 13542, "ANH_MON_B3_124_022" },
	{ 13560, "ANH_MON_B3_124_030" },
	{ 13568, "ANH_MON_B3_124_031" },
	{ 13544, "ANH_MON_B3_124_032" },
	{ 13550, "ANH_MON_B3_124_033" },
	{ 13574, "ANH_MON_B3_124_034" },
	{ 13555, "ANH_MON_B3_124_040" },
	{ 13563, "ANH_MON_B3_124_041" },
	{ 13539, "ANH_MON_B3_124_042" },
	{ 13547, "ANH_MON_B3_124_043" },
	{ 13571, "ANH_MON_B3_124_044" },
	{ 9337, "ANH_MON_B3_124_B" },
	{ 13554, "ANH_MON_B3_124_ILLUST" },
	{ 8824, "ANH_MON_B3_124_P" },
	{ 13575, "ANH_MON_B3_124_TEST" },
	{ 6162, "ANH_MON_B3_125_000" },
	{ 6159, "ANH_MON_B3_125_003" },
	{ 6164, "ANH_MON_B3_125_020" },
	{ 6166, "ANH_MON_B3_125_021" },
	{ 6168, "ANH_MON_B3_125_022" },
	{ 6136, "ANH_MON_B3_125_040" },
	{ 6157, "ANH_MON_B3_125_041" },
	{ 47, "ANH_MON_B3_125_B" },
	{ 58, "ANH_MON_B3_125_P" },
	{ 11027, "ANH_MON_B3_126_000" },
	{ 11043, "ANH_MON_B3_126_001" },
	{ 11007, "ANH_MON_B3_126_002" },
	{ 11013, "ANH_MON_B3_126_003" },
	{ 11049, "ANH_MON_B3_126_004" },
	{ 11054, "ANH_MON_B3_126_008" },
	{ 11029, "ANH_MON_B3_126_010" },
	{ 11045, "ANH_MON_B3_126_011" },
	{ 11009, "ANH_MON_B3_126_012" },
	{ 11015, "ANH_MON_B3_126_013" },
	{ 11051, "ANH_MON_B3_126_014" },
	{ 11024, "ANH_MON_B3_126_020" },
	{ 11040, "ANH_MON_B3_126_021" },
	{ 11004, "ANH_MON_B3_126_022" },
	{ 11021, "ANH_MON_B3_126_040" },
	{ 11037, "ANH_MON_B3_126_041" },
	{ 11001, "ANH_MON_B3_126_042" },
	{ 11010, "ANH_MON_B3_126_043" },
	{ 11046, "ANH_MON_B3_126_044" },
	{ 11023, "ANH_MON_B3_126_050" },
	{ 11039, "ANH_MON_B3_126_051" },
	{ 11003, "ANH_MON_B3_126_052" },
	{ 11017, "ANH_MON_B3_126_060" },
	{ 11033, "ANH_MON_B3_126_061" },
	{ 10998, "ANH_MON_B3_126_062" },
	{ 11019, "ANH_MON_B3_126_070" },
	{ 11035, "ANH_MON_B3_126_071" },
	{ 9336, "ANH_MON_B3_126_B" },
	{ 11031, "ANH_MON_B3_126_ILLUST" },
	{ 8821, "ANH_MON_B3_126_P" },
	{ 11052, "ANH_MON_B3_126_TEST" },
	{ 10981, "ANH_MON_B3_127_000" },
	{ 10965, "ANH_MON_B3_127_002" },
	{ 10973, "ANH_MON_B3_127_003" },
	{ 10992, "ANH_MON_B3_127_007" },
	{ 10983, "ANH_MON_B3_127_010" },
	{ 10991, "ANH_MON_B3_127_011" },
	{ 10967, "ANH_MON_B3_127_012" },
	{ 10975, "ANH_MON_B3_127_013" },
	{ 10997, "ANH_MON_B3_127_014" },
	{ 10977, "ANH_MON_B3_127_020" },
	{ 10987, "ANH_MON_B3_127_021" },
	{ 10961, "ANH_MON_B3_127_022" },
	{ 10979, "ANH_MON_B3_127_030" },
	{ 10989, "ANH_MON_B3_127_031" },
	{ 10963, "ANH_MON_B3_127_032" },
	{ 10971, "ANH_MON_B3_127_033" },
	{ 10995, "ANH_MON_B3_127_034" },
	{ 9683, "ANH_MON_B3_127_B" },
	{ 10985, "ANH_MON_B3_127_ILLUST" },
	{ 9685, "ANH_MON_B3_127_P" },
	{ 10968, "ANH_MON_B3_127_TEST" },
	{ 13922, "ANH_MON_B3_128_000" },
	{ 13902, "ANH_MON_B3_128_002" },
	{ 13910, "ANH_MON_B3_128_003" },
	{ 13932, "ANH_MON_B3_128_007" },
	{ 13919, "ANH_MON_B3_128_020" },
	{ 13929, "ANH_MON_B3_128_021" },
	{ 13899, "ANH_MON_B3_128_022" },
	{ 13921, "ANH_MON_B3_128_030" },
	{ 13931, "ANH_MON_B3_128_031" },
	{ 13901, "ANH_MON_B3_128_032" },
	{ 13909, "ANH_MON_B3_128_033" },
	{ 13939, "ANH_MON_B3_128_034" },
	{ 13915, "ANH_MON_B3_128_040" },
	{ 13925, "ANH_MON_B3_128_041" },
	{ 13895, "ANH_MON_B3_128_042" },
	{ 13905, "ANH_MON_B3_128_043" },
	{ 13935, "ANH_MON_B3_128_044" },
	{ 13917, "ANH_MON_B3_128_050" },
	{ 13927, "ANH_MON_B3_128_051" },
	{ 13897, "ANH_MON_B3_128_052" },
	{ 13907, "ANH_MON_B3_128_053" },
	{ 13937, "ANH_MON_B3_128_054" },
	{ 8046, "ANH_MON_B3_128_B" },
	{ 13913, "ANH_MON_B3_128_ILLUST" },
	{ 5005, "ANH_MON_B3_128_P" },
	{ 13940, "ANH_MON_B3_128_TEST" },
	{ 13587, "ANH_MON_B3_130_000" },
	{ 13583, "ANH_MON_B3_130_003" },
	{ 13585, "ANH_MON_B3_130_020" },
	{ 13589, "ANH_MON_B3_130_021" },
	{ 13579, "ANH_MON_B3_130_022" },
	{ 9340, "ANH_MON_B3_130_B" },
	{ 13578, "ANH_MON_B3_130_ILLUST" },
	{ 8833, "ANH_MON_B3_130_P" },
	{ 13581, "ANH_MON_B3_130_TEST" },
	{ 11767, "ANH_MON_B3_131_000" },
	{ 11775, "ANH_MON_B3_131_001" },
	{ 11753, "ANH_MON_B3_131_002" },
	{ 11759, "ANH_MON_B3_131_003" },
	{ 11783, "ANH_MON_B3_131_004" },
	{ 11786, "ANH_MON_B3_131_005" },
	{ 11778, "ANH_MON_B3_131_006" },
	{ 11746, "ANH_MON_B3_131_008" },
	{ 11769, "ANH_MON_B3_131_010" },
	{ 11777, "ANH_MON_B3_131_011" },
	{ 11755, "ANH_MON_B3_131_012" },
	{ 11761, "ANH_MON_B3_131_013" },
	{ 11785, "ANH_MON_B3_131_014" },
	{ 11763, "ANH_MON_B3_131_020" },
	{ 11771, "ANH_MON_B3_131_021" },
	{ 11749, "ANH_MON_B3_131_022" },
	{ 11765, "ANH_MON_B3_131_030" },
	{ 11773, "ANH_MON_B3_131_031" },
	{ 11751, "ANH_MON_B3_131_032" },
	{ 11757, "ANH_MON_B3_131_033" },
	{ 11781, "ANH_MON_B3_131_034" },
	{ 9339, "ANH_MON_B3_131_B" },
	{ 11745, "ANH_MON_B3_131_ILLUST" },
	{ 8830, "ANH_MON_B3_131_P" },
	{ 11742, "ANH_MON_B3_131_TEST" },
	{ 9991, "ANH_MON_B3_132_000" },
	{ 9979, "ANH_MON_B3_132_002" },
	{ 9985, "ANH_MON_B3_132_003" },
	{ 9998, "ANH_MON_B3_132_007" },
	{ 9993, "ANH_MON_B3_132_010" },
	{ 9997, "ANH_MON_B3_132_011" },
	{ 9981, "ANH_MON_B3_132_012" },
	{ 9987, "ANH_MON_B3_132_013" },
	{ 10001, "ANH_MON_B3_132_014" },
	{ 9988, "ANH_MON_B3_132_020" },
	{ 9994, "ANH_MON_B3_132_021" },
	{ 9976, "ANH_MON_B3_132_022" },
	{ 9338, "ANH_MON_B3_132_B" },
	{ 9975, "ANH_MON_B3_132_ILLUST" },
	{ 8827, "ANH_MON_B3_132_P" },
	{ 9982, "ANH_MON_B3_132_TEST" },
	{ 7956, "ANH_MON_B3_136_000" },
	{ 7936, "ANH_MON_B3_136_002" },
	{ 7944, "ANH_MON_B3_136_003" },
	{ 7967, "ANH_MON_B3_136_007" },
	{ 7958, "ANH_MON_B3_136_010" },
	{ 7966, "ANH_MON_B3_136_011" },
	{ 7938, "ANH_MON_B3_136_012" },
	{ 7946, "ANH_MON_B3_136_013" },
	{ 7972, "ANH_MON_B3_136_014" },
	{ 7952, "ANH_MON_B3_136_020" },
	{ 7962, "ANH_MON_B3_136_021" },
	{ 7932, "ANH_MON_B3_136_022" },
	{ 7954, "ANH_MON_B3_136_030" },
	{ 7964, "ANH_MON_B3_136_031" },
	{ 7934, "ANH_MON_B3_136_032" },
	{ 7948, "ANH_MON_B3_136_040" },
	{ 7959, "ANH_MON_B3_136_041" },
	{ 7929, "ANH_MON_B3_136_042" },
	{ 7941, "ANH_MON_B3_136_043" },
	{ 7969, "ANH_MON_B3_136_044" },
	{ 7950, "ANH_MON_B3_136_050" },
	{ 12944, "ANH_MON_B3_136_051" },
	{ 9342, "ANH_MON_B3_136_B" },
	{ 7928, "ANH_MON_B3_136_ILLUST" },
	{ 8839, "ANH_MON_B3_136_P" },
	{ 7939, "ANH_MON_B3_136_TEST" },
	{ 11865, "ANH_MON_B3_137_000" },
	{ 11851, "ANH_MON_B3_137_002" },
	{ 11857, "ANH_MON_B3_137_003" },
	{ 11874, "ANH_MON_B3_137_007" },
	{ 11867, "ANH_MON_B3_137_010" },
	{ 11873, "ANH_MON_B3_137_011" },
	{ 11853, "ANH_MON_B3_137_012" },
	{ 11859, "ANH_MON_B3_137_013" },
	{ 11879, "ANH_MON_B3_137_014" },
	{ 11861, "ANH_MON_B3_137_020" },
	{ 11869, "ANH_MON_B3_137_021" },
	{ 11847, "ANH_MON_B3_137_022" },
	{ 11863, "ANH_MON_B3_137_030" },
	{ 11871, "ANH_MON_B3_137_031" },
	{ 11849, "ANH_MON_B3_137_032" },
	{ 11855, "ANH_MON_B3_137_033" },
	{ 11877, "ANH_MON_B3_137_034" },
	{ 9341, "ANH_MON_B3_137_B" },
	{ 11845, "ANH_MON_B3_137_ILLUST" },
	{ 8836, "ANH_MON_B3_137_P" },
	{ 11842, "ANH_MON_B3_137_TEST" },
	{ 10041, "ANH_MON_B3_138_000" },
	{ 10017, "ANH_MON_B3_138_002" },
	{ 10025, "ANH_MON_B3_138_003" },
	{ 10058, "ANH_MON_B3_138_007" },
	{ 10043, "ANH_MON_B3_138_010" },
	{ 10057, "ANH_MON_B3_138_011" },
	{ 10019, "ANH_MON_B3_138_012" },
	{ 10027, "ANH_MON_B3_138_013" },
	{ 10065, "ANH_MON_B3_138_014" },
	{ 10037, "ANH_MON_B3_138_020" },
	{ 10053, "ANH_MON_B3_138_021" },
	{ 10013, "ANH_MON_B3_138_022" },
	{ 10039, "ANH_MON_B3_138_030" },
	{ 10055, "ANH_MON_B3_138_031" },
	{ 10015, "ANH_MON_B3_138_032" },
	{ 10033, "ANH_MON_B3_138_040" },
	{ 10049, "ANH_MON_B3_138_041" },
	{ 10009, "ANH_MON_B3_138_042" },
	{ 10021, "ANH_MON_B3_138_043" },
	{ 10061, "ANH_MON_B3_138_044" },
	{ 10035, "ANH_MON_B3_138_050" },
	{ 10051, "ANH_MON_B3_138_051" },
	{ 10011, "ANH_MON_B3_138_052" },
	{ 10023, "ANH_MON_B3_138_053" },
	{ 10063, "ANH_MON_B3_138_054" },
	{ 10029, "ANH_MON_B3_138_060" },
	{ 10045, "ANH_MON_B3_138_061" },
	{ 10006, "ANH_MON_B3_138_062" },
	{ 10031, "ANH_MON_B3_138_070" },
	{ 10047, "ANH_MON_B3_138_071" },
	{ 9343, "ANH_MON_B3_138_B" },
	{ 10003, "ANH_MON_B3_138_ILLUST" },
	{ 8842, "ANH_MON_B3_138_P" },
	{ 10004, "ANH_MON_B3_138_TEST" },
	{ 10170, "ANH_MON_B3_140_000" },
	{ 10178, "ANH_MON_B3_140_001" },
	{ 10160, "ANH_MON_B3_140_003" },
	{ 10182, "ANH_MON_B3_140_004" },
	{ 10186, "ANH_MON_B3_140_005" },
	{ 10190, "ANH_MON_B3_140_008" },
	{ 10172, "ANH_MON_B3_140_010" },
	{ 10180, "ANH_MON_B3_140_011" },
	{ 10159, "ANH_MON_B3_140_012" },
	{ 10162, "ANH_MON_B3_140_013" },
	{ 10184, "ANH_MON_B3_140_014" },
	{ 10166, "ANH_MON_B3_140_020" },
	{ 10174, "ANH_MON_B3_140_021" },
	{ 10154, "ANH_MON_B3_140_022" },
	{ 10168, "ANH_MON_B3_140_030" },
	{ 10176, "ANH_MON_B3_140_031" },
	{ 10156, "ANH_MON_B3_140_032" },
	{ 1391, "ANH_MON_B3_140_040" },
	{ 1393, "ANH_MON_B3_140_041" },
	{ 1395, "ANH_MON_B3_140_042" },
	{ 1400, "ANH_MON_B3_140_043" },
	{ 1406, "ANH_MON_B3_140_044" },
	{ 9330, "ANH_MON_B3_140_B" },
	{ 10165, "ANH_MON_B3_140_ILLUST" },
	{ 8803, "ANH_MON_B3_140_P" },
	{ 10188, "ANH_MON_B3_140_TEST" },
	{ 13872, "ANH_MON_B3_141_000" },
	{ 13852, "ANH_MON_B3_141_002" },
	{ 13860, "ANH_MON_B3_141_003" },
	{ 13886, "ANH_MON_B3_141_007" },
	{ 13874, "ANH_MON_B3_141_010" },
	{ 13883, "ANH_MON_B3_141_011" },
	{ 13854, "ANH_MON_B3_141_012" },
	{ 13862, "ANH_MON_B3_141_013" },
	{ 13891, "ANH_MON_B3_141_014" },
	{ 13868, "ANH_MON_B3_141_020" },
	{ 13878, "ANH_MON_B3_141_021" },
	{ 13848, "ANH_MON_B3_141_022" },
	{ 13870, "ANH_MON_B3_141_030" },
	{ 13880, "ANH_MON_B3_141_031" },
	{ 13850, "ANH_MON_B3_141_032" },
	{ 13859, "ANH_MON_B3_141_033" },
	{ 13889, "ANH_MON_B3_141_034" },
	{ 13893, "ANH_MON_B3_141_035" },
	{ 13885, "ANH_MON_B3_141_036" },
	{ 13866, "ANH_MON_B3_141_040" },
	{ 13876, "ANH_MON_B3_141_041" },
	{ 13846, "ANH_MON_B3_141_042" },
	{ 1861, "ANH_MON_B3_141_B" },
	{ 13865, "ANH_MON_B3_141_ILLUST" },
	{ 2227, "ANH_MON_B3_141_P" },
	{ 13856, "ANH_MON_B3_141_TEST" },
	{ 11816, "ANH_MON_B3_142_000" },
	{ 11796, "ANH_MON_B3_142_002" },
	{ 11802, "ANH_MON_B3_142_003" },
	{ 11832, "ANH_MON_B3_142_007" },
	{ 11838, "ANH_MON_B3_142_008" },
	{ 11840, "ANH_MON_B3_142_009" },
	{ 11818, "ANH_MON_B3_142_010" },
	{ 11831, "ANH_MON_B3_142_011" },
	{ 11798, "ANH_MON_B3_142_012" },
	{ 11812, "ANH_MON_B3_142_020" },
	{ 11826, "ANH_MON_B3_142_021" },
	{ 11792, "ANH_MON_B3_142_022" },
	{ 11814, "ANH_MON_B3_142_030" },
	{ 11828, "ANH_MON_B3_142_031" },
	{ 11794, "ANH_MON_B3_142_032" },
	{ 11808, "ANH_MON_B3_142_040" },
	{ 11822, "ANH_MON_B3_142_041" },
	{ 11788, "ANH_MON_B3_142_042" },
	{ 11800, "ANH_MON_B3_142_043" },
	{ 11834, "ANH_MON_B3_142_044" },
	{ 11810, "ANH_MON_B3_142_050" },
	{ 11824, "ANH_MON_B3_142_051" },
	{ 11790, "ANH_MON_B3_142_052" },
	{ 11806, "ANH_MON_B3_142_060" },
	{ 11820, "ANH_MON_B3_142_061" },
	{ 8412, "ANH_MON_B3_142_B" },
	{ 8414, "ANH_MON_B3_142_P" },
	{ 11920, "ANH_MON_B3_143_000" },
	{ 11894, "ANH_MON_B3_143_002" },
	{ 11904, "ANH_MON_B3_143_003" },
	{ 11940, "ANH_MON_B3_143_007" },
	{ 11922, "ANH_MON_B3_143_010" },
	{ 11937, "ANH_MON_B3_143_011" },
	{ 11896, "ANH_MON_B3_143_012" },
	{ 11906, "ANH_MON_B3_143_013" },
	{ 11947, "ANH_MON_B3_143_014" },
	{ 11918, "ANH_MON_B3_143_020" },
	{ 11934, "ANH_MON_B3_143_021" },
	{ 11892, "ANH_MON_B3_143_022" },
	{ 11914, "ANH_MON_B3_143_040" },
	{ 11930, "ANH_MON_B3_143_041" },
	{ 11888, "ANH_MON_B3_143_042" },
	{ 11900, "ANH_MON_B3_143_043" },
	{ 11942, "ANH_MON_B3_143_044" },
	{ 11916, "ANH_MON_B3_143_050" },
	{ 11932, "ANH_MON_B3_143_051" },
	{ 11890, "ANH_MON_B3_143_052" },
	{ 11902, "ANH_MON_B3_143_053" },
	{ 11944, "ANH_MON_B3_143_054" },
	{ 11910, "ANH_MON_B3_143_060" },
	{ 11926, "ANH_MON_B3_143_061" },
	{ 11884, "ANH_MON_B3_143_062" },
	{ 11912, "ANH_MON_B3_143_070" },
	{ 11928, "ANH_MON_B3_143_071" },
	{ 11886, "ANH_MON_B3_143_072" },
	{ 11924, "ANH_MON_B3_143_080" },
	{ 11938, "ANH_MON_B3_143_081" },
	{ 11898, "ANH_MON_B3_143_082" },
	{ 11908, "ANH_MON_B3_143_083" },
	{ 11948, "ANH_MON_B3_143_084" },
	{ 9329, "ANH_MON_B3_143_B" },
	{ 8800, "ANH_MON_B3_143_P" },
	{ 11950, "ANH_MON_B3_143_P2" },
	{ 10135, "ANH_MON_B3_144_000" },
	{ 11358, "ANH_MON_B3_144_001" },
	{ 11270, "ANH_MON_B3_144_020" },
	{ 11273, "ANH_MON_B3_144_021" },
	{ 11275, "ANH_MON_B3_144_022" },
	{ 11277, "ANH_MON_B3_144_040" },
	{ 10235, "ANH_MON_B3_144_B" },
	{ 10237, "ANH_MON_B3_144_P" },
	{ 8710, "ANH_MON_B3_145_000" },
	{ 12976, "ANH_MON_B3_145_001" },
	{ 12978, "ANH_MON_B3_145_002" },
	{ 13087, "ANH_MON_B3_145_003" },
	{ 12980, "ANH_MON_B3_145_007" },
	{ 11279, "ANH_MON_B3_145_020" },
	{ 11281, "ANH_MON_B3_145_021" },
	{ 10754, "ANH_MON_B3_145_022" },
	{ 11283, "ANH_MON_B3_145_030" },
	{ 11285, "ANH_MON_B3_145_031" },
	{ 11287, "ANH_MON_B3_145_032" },
	{ 11289, "ANH_MON_B3_145_040" },
	{ 11290, "ANH_MON_B3_145_041" },
	{ 11293, "ANH_MON_B3_145_042" },
	{ 11837, "ANH_MON_B3_145_043" },
	{ 10231, "ANH_MON_B3_145_B" },
	{ 10233, "ANH_MON_B3_145_P" },
	{ 13974, "ANH_MON_B3_146_000" },
	{ 13986, "ANH_MON_B3_146_001" },
	{ 13952, "ANH_MON_B3_146_002" },
	{ 13966, "ANH_MON_B3_146_003" },
	{ 14002, "ANH_MON_B3_146_004" },
	{ 14004, "ANH_MON_B3_146_005" },
	{ 13992, "ANH_MON_B3_146_006" },
	{ 14006, "ANH_MON_B3_146_008" },
	{ 13976, "ANH_MON_B3_146_010" },
	{ 13970, "ANH_MON_B3_146_020" },
	{ 13984, "ANH_MON_B3_146_021" },
	{ 13950, "ANH_MON_B3_146_022" },
	{ 13972, "ANH_MON_B3_146_030" },
	{ 13968, "ANH_MON_B3_146_040" },
	{ 13982, "ANH_MON_B3_146_041" },
	{ 13946, "ANH_MON_B3_146_042" },
	{ 13962, "ANH_MON_B3_146_043" },
	{ 13998, "ANH_MON_B3_146_044" },
	{ 13948, "ANH_MON_B3_146_052" },
	{ 13964, "ANH_MON_B3_146_053" },
	{ 14000, "ANH_MON_B3_146_054" },
	{ 13942, "ANH_MON_B3_146_062" },
	{ 13958, "ANH_MON_B3_146_063" },
	{ 13994, "ANH_MON_B3_146_064" },
	{ 13944, "ANH_MON_B3_146_072" },
	{ 13960, "ANH_MON_B3_146_073" },
	{ 13996, "ANH_MON_B3_146_074" },
	{ 13978, "ANH_MON_B3_146_080" },
	{ 13988, "ANH_MON_B3_146_081" },
	{ 13954, "ANH_MON_B3_146_082" },
	{ 13980, "ANH_MON_B3_146_090" },
	{ 13990, "ANH_MON_B3_146_091" },
	{ 13956, "ANH_MON_B3_146_092" },
	{ 10507, "ANH_MON_B3_146_B" },
	{ 939, "ANH_MON_B3_146_ILLUST" },
	{ 10509, "ANH_MON_B3_146_P" },
	{ 4643, "ANH_MON_B3_146_TEST" },
	{ 72, "ANH_MON_B3_147_000" },
	{ 327, "ANH_MON_B3_147_001" },
	{ 60, "ANH_MON_B3_147_003" },
	{ 339, "ANH_MON_B3_147_004" },
	{ 343, "ANH_MON_B3_147_005" },
	{ 331, "ANH_MON_B3_147_006" },
	{ 345, "ANH_MON_B3_147_009" },
	{ 74, "ANH_MON_B3_147_010" },
	{ 68, "ANH_MON_B3_147_020" },
	{ 318, "ANH_MON_B3_147_021" },
	{ 36, "ANH_MON_B3_147_022" },
	{ 70, "ANH_MON_B3_147_030" },
	{ 322, "ANH_MON_B3_147_031" },
	{ 39, "ANH_MON_B3_147_032" },
	{ 63, "ANH_MON_B3_147_040" },
	{ 309, "ANH_MON_B3_147_041" },
	{ 32, "ANH_MON_B3_147_042" },
	{ 46, "ANH_MON_B3_147_043" },
	{ 335, "ANH_MON_B3_147_044" },
	{ 66, "ANH_MON_B3_147_050" },
	{ 311, "ANH_MON_B3_147_051" },
	{ 34, "ANH_MON_B3_147_052" },
	{ 48, "ANH_MON_B3_147_053" },
	{ 337, "ANH_MON_B3_147_054" },
	{ 342, "ANH_MON_B3_147_055" },
	{ 330, "ANH_MON_B3_147_056" },
	{ 334, "ANH_MON_B3_147_057" },
	{ 14729, "ANH_MON_B3_147_B" },
	{ 307, "ANH_MON_B3_147_ILLUST" },
	{ 14731, "ANH_MON_B3_147_P" },
	{ 43, "ANH_MON_B3_147_TEST" },
	{ 12218, "ANH_MON_B3_148_000" },
	{ 12202, "ANH_MON_B3_148_002" },
	{ 12206, "ANH_MON_B3_148_003" },
	{ 12232, "ANH_MON_B3_148_007" },
	{ 12220, "ANH_MON_B3_148_010" },
	{ 12231, "ANH_MON_B3_148_011" },
	{ 12204, "ANH_MON_B3_148_012" },
	{ 12208, "ANH_MON_B3_148_013" },
	{ 12235, "ANH_MON_B3_148_014" },
	{ 12216, "ANH_MON_B3_148_030" },
	{ 12228, "ANH_MON_B3_148_031" },
	{ 12210, "ANH_MON_B3_148_040" },
	{ 12222, "ANH_MON_B3_148_041" },
	{ 12212, "ANH_MON_B3_148_050" },
	{ 12224, "ANH_MON_B3_148_051" },
	{ 9457, "ANH_MON_B3_148_060" },
	{ 9459, "ANH_MON_B3_148_061" },
	{ 9671, "ANH_MON_B3_148_B" },
	{ 9673, "ANH_MON_B3_148_P" },
	{ 12046, "ANH_MON_B3_149_000" },
	{ 12042, "ANH_MON_B3_149_002" },
	{ 12044, "ANH_MON_B3_149_003" },
	{ 12050, "ANH_MON_B3_149_007" },
	{ 10193, "ANH_MON_B3_149_B" },
	{ 10195, "ANH_MON_B3_149_P" },
	{ 12298, "ANH_MON_B3_151_000" },
	{ 12302, "ANH_MON_B3_151_001" },
	{ 12292, "ANH_MON_B3_151_002" },
	{ 12294, "ANH_MON_B3_151_003" },
	{ 12306, "ANH_MON_B3_151_005" },
	{ 12304, "ANH_MON_B3_151_007" },
	{ 12296, "ANH_MON_B3_151_020" },
	{ 12300, "ANH_MON_B3_151_021" },
	{ 12290, "ANH_MON_B3_151_022" },
	{ 9675, "ANH_MON_B3_151_B" },
	{ 9677, "ANH_MON_B3_151_P" },
	{ 8219, "ANH_MON_B3_152_000" },
	{ 8209, "ANH_MON_B3_152_002" },
	{ 8213, "ANH_MON_B3_152_003" },
	{ 8227, "ANH_MON_B3_152_007" },
	{ 8221, "ANH_MON_B3_152_010" },
	{ 8226, "ANH_MON_B3_152_011" },
	{ 8211, "ANH_MON_B3_152_012" },
	{ 8215, "ANH_MON_B3_152_013" },
	{ 8230, "ANH_MON_B3_152_014" },
	{ 8217, "ANH_MON_B3_152_020" },
	{ 8223, "ANH_MON_B3_152_021" },
	{ 8207, "ANH_MON_B3_152_022" },
	{ 11265, "ANH_MON_B3_152_B" },
	{ 11267, "ANH_MON_B3_152_P" },
	{ 10848, "ANH_MON_B3_154_000" },
	{ 10838, "ANH_MON_B3_154_002" },
	{ 10842, "ANH_MON_B3_154_003" },
	{ 10856, "ANH_MON_B3_154_007" },
	{ 11201, "ANH_MON_B3_154_008" },
	{ 10850, "ANH_MON_B3_154_010" },
	{ 10855, "ANH_MON_B3_154_011" },
	{ 10840, "ANH_MON_B3_154_012" },
	{ 10844, "ANH_MON_B3_154_013" },
	{ 10859, "ANH_MON_B3_154_014" },
	{ 10846, "ANH_MON_B3_154_020" },
	{ 10852, "ANH_MON_B3_154_021" },
	{ 10836, "ANH_MON_B3_154_022" },
	{ 11269, "ANH_MON_B3_154_B" },
	{ 11271, "ANH_MON_B3_154_P" },
	{ 2844, "ANH_MON_B3_155_000" },
	{ 2839, "ANH_MON_B3_155_002" },
	{ 2840, "ANH_MON_B3_155_003" },
	{ 2843, "ANH_MON_B3_155_007" },
	{ 5469, "ANH_MON_B3_155_008" },
	{ 2796, "ANH_MON_B3_155_010" },
	{ 2831, "ANH_MON_B3_155_011" },
	{ 2832, "ANH_MON_B3_155_012" },
	{ 2835, "ANH_MON_B3_155_013" },
	{ 2837, "ANH_MON_B3_155_014" },
	{ 2846, "ANH_MON_B3_155_020" },
	{ 2849, "ANH_MON_B3_155_021" },
	{ 2850, "ANH_MON_B3_155_022" },
	{ 12890, "ANH_MON_B3_155_B" },
	{ 12892, "ANH_MON_B3_155_P" },
	{ 6709, "ANH_MON_B3_156_000" },
	{ 6697, "ANH_MON_B3_156_002" },
	{ 6703, "ANH_MON_B3_156_003" },
	{ 6719, "ANH_MON_B3_156_007" },
	{ 6711, "ANH_MON_B3_156_010" },
	{ 6716, "ANH_MON_B3_156_011" },
	{ 6699, "ANH_MON_B3_156_012" },
	{ 6705, "ANH_MON_B3_156_013" },
	{ 6722, "ANH_MON_B3_156_014" },
	{ 6724, "ANH_MON_B3_156_015" },
	{ 6718, "ANH_MON_B3_156_016" },
	{ 9655, "ANH_MON_B3_156_017" },
	{ 9657, "ANH_MON_B3_156_018" },
	{ 6707, "ANH_MON_B3_156_020" },
	{ 6713, "ANH_MON_B3_156_021" },
	{ 6695, "ANH_MON_B3_156_022" },
	{ 9679, "ANH_MON_B3_156_B" },
	{ 6694, "ANH_MON_B3_156_ILLUST" },
	{ 9681, "ANH_MON_B3_156_P" },
	{ 6701, "ANH_MON_B3_156_TEST" },
	{ 6669, "ANH_MON_B3_157_000" },
	{ 6679, "ANH_MON_B3_157_001" },
	{ 6653, "ANH_MON_B3_157_002" },
	{ 6659, "ANH_MON_B3_157_003" },
	{ 6687, "ANH_MON_B3_157_004" },
	{ 6691, "ANH_MON_B3_157_005" },
	{ 6683, "ANH_MON_B3_157_006" },
	{ 6645, "ANH_MON_B3_157_008" },
	{ 6671, "ANH_MON_B3_157_010" },
	{ 6681, "ANH_MON_B3_157_011" },
	{ 6655, "ANH_MON_B3_157_012" },
	{ 6661, "ANH_MON_B3_157_013" },
	{ 6689, "ANH_MON_B3_157_014" },
	{ 6665, "ANH_MON_B3_157_020" },
	{ 6675, "ANH_MON_B3_157_021" },
	{ 6649, "ANH_MON_B3_157_022" },
	{ 6667, "ANH_MON_B3_157_030" },
	{ 6677, "ANH_MON_B3_157_031" },
	{ 6651, "ANH_MON_B3_157_032" },
	{ 6663, "ANH_MON_B3_157_040" },
	{ 6673, "ANH_MON_B3_157_041" },
	{ 6657, "ANH_MON_B3_157_043" },
	{ 6685, "ANH_MON_B3_157_044" },
	{ 9331, "ANH_MON_B3_157_B" },
	{ 8806, "ANH_MON_B3_157_P" },
	{ 6741, "ANH_MON_B3_158_000" },
	{ 6733, "ANH_MON_B3_158_002" },
	{ 6737, "ANH_MON_B3_158_003" },
	{ 6739, "ANH_MON_B3_158_020" },
	{ 6743, "ANH_MON_B3_158_021" },
	{ 6729, "ANH_MON_B3_158_022" },
	{ 6745, "ANH_MON_B3_158_031" },
	{ 6731, "ANH_MON_B3_158_032" },
	{ 6736, "ANH_MON_B3_158_033" },
	{ 6748, "ANH_MON_B3_158_034" },
	{ 9333, "ANH_MON_B3_158_B" },
	{ 6726, "ANH_MON_B3_158_ILLUST" },
	{ 8812, "ANH_MON_B3_158_P" },
	{ 6727, "ANH_MON_B3_158_TEST" },
	{ 6937, "ANH_MON_B3_159_000" },
	{ 6945, "ANH_MON_B3_159_001" },
	{ 6923, "ANH_MON_B3_159_002" },
	{ 6929, "ANH_MON_B3_159_003" },
	{ 6951, "ANH_MON_B3_159_004" },
	{ 6955, "ANH_MON_B3_159_005" },
	{ 6949, "ANH_MON_B3_159_006" },
	{ 6917, "ANH_MON_B3_159_008" },
	{ 6939, "ANH_MON_B3_159_010" },
	{ 6947, "ANH_MON_B3_159_011" },
	{ 6925, "ANH_MON_B3_159_012" },
	{ 6931, "ANH_MON_B3_159_013" },
	{ 6953, "ANH_MON_B3_159_014" },
	{ 6935, "ANH_MON_B3_159_020" },
	{ 6943, "ANH_MON_B3_159_021" },
	{ 6921, "ANH_MON_B3_159_022" },
	{ 6933, "ANH_MON_B3_159_040" },
	{ 6941, "ANH_MON_B3_159_041" },
	{ 6919, "ANH_MON_B3_159_042" },
	{ 6927, "ANH_MON_B3_159_043" },
	{ 9332, "ANH_MON_B3_159_B" },
	{ 8809, "ANH_MON_B3_159_P" },
	{ 6808, "ANH_MON_B3_160_000" },
	{ 6818, "ANH_MON_B3_160_001" },
	{ 6790, "ANH_MON_B3_160_002" },
	{ 6796, "ANH_MON_B3_160_003" },
	{ 6824, "ANH_MON_B3_160_004" },
	{ 6827, "ANH_MON_B3_160_005" },
	{ 6821, "ANH_MON_B3_160_006" },
	{ 6831, "ANH_MON_B3_160_008" },
	{ 6810, "ANH_MON_B3_160_010" },
	{ 6820, "ANH_MON_B3_160_011" },
	{ 6792, "ANH_MON_B3_160_012" },
	{ 6798, "ANH_MON_B3_160_013" },
	{ 6826, "ANH_MON_B3_160_014" },
	{ 6804, "ANH_MON_B3_160_020" },
	{ 6814, "ANH_MON_B3_160_021" },
	{ 6786, "ANH_MON_B3_160_022" },
	{ 6806, "ANH_MON_B3_160_030" },
	{ 6816, "ANH_MON_B3_160_031" },
	{ 6788, "ANH_MON_B3_160_032" },
	{ 6801, "ANH_MON_B3_160_040" },
	{ 6811, "ANH_MON_B3_160_041" },
	{ 6783, "ANH_MON_B3_160_042" },
	{ 6793, "ANH_MON_B3_160_043" },
	{ 9324, "ANH_MON_B3_160_B" },
	{ 6800, "ANH_MON_B3_160_ILLUST" },
	{ 8785, "ANH_MON_B3_160_P" },
	{ 6829, "ANH_MON_B3_160_TEST" },
	{ 13430, "ANH_MON_B3_161_000" },
	{ 13438, "ANH_MON_B3_161_001" },
	{ 13416, "ANH_MON_B3_161_002" },
	{ 13422, "ANH_MON_B3_161_003" },
	{ 13444, "ANH_MON_B3_161_004" },
	{ 13447, "ANH_MON_B3_161_005" },
	{ 13441, "ANH_MON_B3_161_006" },
	{ 13449, "ANH_MON_B3_161_008" },
	{ 13432, "ANH_MON_B3_161_010" },
	{ 13440, "ANH_MON_B3_161_011" },
	{ 13418, "ANH_MON_B3_161_012" },
	{ 13424, "ANH_MON_B3_161_013" },
	{ 13446, "ANH_MON_B3_161_014" },
	{ 13427, "ANH_MON_B3_161_020" },
	{ 13435, "ANH_MON_B3_161_021" },
	{ 13413, "ANH_MON_B3_161_022" },
	{ 13425, "ANH_MON_B3_161_040" },
	{ 13433, "ANH_MON_B3_161_041" },
	{ 13411, "ANH_MON_B3_161_042" },
	{ 13419, "ANH_MON_B3_161_043" },
	{ 9323, "ANH_MON_B3_161_B" },
	{ 8782, "ANH_MON_B3_161_P" },
	{ 6770, "ANH_MON_B3_162_000" },
	{ 6754, "ANH_MON_B3_162_002" },
	{ 6760, "ANH_MON_B3_162_003" },
	{ 6772, "ANH_MON_B3_162_010" },
	{ 6778, "ANH_MON_B3_162_011" },
	{ 6756, "ANH_MON_B3_162_012" },
	{ 6762, "ANH_MON_B3_162_013" },
	{ 6780, "ANH_MON_B3_162_014" },
	{ 6767, "ANH_MON_B3_162_020" },
	{ 6775, "ANH_MON_B3_162_021" },
	{ 6751, "ANH_MON_B3_162_022" },
	{ 6765, "ANH_MON_B3_162_040" },
	{ 6773, "ANH_MON_B3_162_041" },
	{ 6749, "ANH_MON_B3_162_042" },
	{ 6757, "ANH_MON_B3_162_043" },
	{ 9322, "ANH_MON_B3_162_B" },
	{ 8779, "ANH_MON_B3_162_P" },
	{ 13388, "ANH_MON_B3_163_000" },
	{ 13398, "ANH_MON_B3_163_001" },
	{ 13368, "ANH_MON_B3_163_002" },
	{ 13376, "ANH_MON_B3_163_003" },
	{ 13404, "ANH_MON_B3_163_004" },
	{ 13407, "ANH_MON_B3_163_005" },
	{ 13401, "ANH_MON_B3_163_006" },
	{ 13409, "ANH_MON_B3_163_008" },
	{ 13390, "ANH_MON_B3_163_010" },
	{ 13400, "ANH_MON_B3_163_011" },
	{ 13370, "ANH_MON_B3_163_012" },
	{ 13378, "ANH_MON_B3_163_013" },
	{ 13406, "ANH_MON_B3_163_014" },
	{ 13384, "ANH_MON_B3_163_020" },
	{ 13394, "ANH_MON_B3_163_021" },
	{ 13364, "ANH_MON_B3_163_022" },
	{ 13386, "ANH_MON_B3_163_030" },
	{ 13396, "ANH_MON_B3_163_031" },
	{ 13366, "ANH_MON_B3_163_032" },
	{ 13381, "ANH_MON_B3_163_040" },
	{ 13391, "ANH_MON_B3_163_041" },
	{ 13361, "ANH_MON_B3_163_042" },
	{ 13373, "ANH_MON_B3_163_043" },
	{ 9321, "ANH_MON_B3_163_B" },
	{ 13380, "ANH_MON_B3_163_ILLUST" },
	{ 8776, "ANH_MON_B3_163_P" },
	{ 13371, "ANH_MON_B3_163_TEST" },
	{ 6893, "ANH_MON_B3_164_000" },
	{ 6901, "ANH_MON_B3_164_001" },
	{ 6879, "ANH_MON_B3_164_002" },
	{ 6883, "ANH_MON_B3_164_003" },
	{ 6905, "ANH_MON_B3_164_004" },
	{ 6907, "ANH_MON_B3_164_005" },
	{ 6903, "ANH_MON_B3_164_006" },
	{ 6911, "ANH_MON_B3_164_008" },
	{ 6890, "ANH_MON_B3_164_020" },
	{ 6898, "ANH_MON_B3_164_021" },
	{ 6876, "ANH_MON_B3_164_022" },
	{ 6892, "ANH_MON_B3_164_030" },
	{ 6900, "ANH_MON_B3_164_031" },
	{ 6878, "ANH_MON_B3_164_032" },
	{ 6887, "ANH_MON_B3_164_040" },
	{ 6895, "ANH_MON_B3_164_041" },
	{ 6873, "ANH_MON_B3_164_042" },
	{ 6881, "ANH_MON_B3_164_043" },
	{ 9328, "ANH_MON_B3_164_B" },
	{ 6886, "ANH_MON_B3_164_ILLUST" },
	{ 8797, "ANH_MON_B3_164_P" },
	{ 6909, "ANH_MON_B3_164_TEST" },
	{ 6855, "ANH_MON_B3_165_000" },
	{ 6863, "ANH_MON_B3_165_001" },
	{ 6839, "ANH_MON_B3_165_002" },
	{ 6845, "ANH_MON_B3_165_003" },
	{ 6867, "ANH_MON_B3_165_004" },
	{ 6869, "ANH_MON_B3_165_005" },
	{ 6865, "ANH_MON_B3_165_006" },
	{ 6871, "ANH_MON_B3_165_008" },
	{ 6852, "ANH_MON_B3_165_020" },
	{ 6860, "ANH_MON_B3_165_021" },
	{ 6836, "ANH_MON_B3_165_022" },
	{ 6854, "ANH_MON_B3_165_030" },
	{ 6862, "ANH_MON_B3_165_031" },
	{ 6838, "ANH_MON_B3_165_032" },
	{ 6849, "ANH_MON_B3_165_040" },
	{ 6857, "ANH_MON_B3_165_041" },
	{ 6833, "ANH_MON_B3_165_042" },
	{ 6843, "ANH_MON_B3_165_043" },
	{ 9327, "ANH_MON_B3_165_B" },
	{ 6848, "ANH_MON_B3_165_ILLUST" },
	{ 8794, "ANH_MON_B3_165_P" },
	{ 6841, "ANH_MON_B3_165_TEST" },
	{ 6439, "ANH_MON_B3_166_000" },
	{ 6425, "ANH_MON_B3_166_002" },
	{ 6431, "ANH_MON_B3_166_003" },
	{ 6447, "ANH_MON_B3_166_007" },
	{ 6436, "ANH_MON_B3_166_020" },
	{ 6444, "ANH_MON_B3_166_021" },
	{ 6422, "ANH_MON_B3_166_022" },
	{ 6438, "ANH_MON_B3_166_030" },
	{ 6446, "ANH_MON_B3_166_031" },
	{ 6424, "ANH_MON_B3_166_032" },
	{ 6430, "ANH_MON_B3_166_033" },
	{ 6452, "ANH_MON_B3_166_034" },
	{ 6433, "ANH_MON_B3_166_040" },
	{ 6441, "ANH_MON_B3_166_041" },
	{ 6419, "ANH_MON_B3_166_042" },
	{ 6427, "ANH_MON_B3_166_043" },
	{ 6449, "ANH_MON_B3_166_044" },
	{ 9326, "ANH_MON_B3_166_B" },
	{ 8791, "ANH_MON_B3_166_P" },
	{ 13478, "ANH_MON_B3_167_000" },
	{ 13490, "ANH_MON_B3_167_001" },
	{ 13458, "ANH_MON_B3_167_002" },
	{ 13464, "ANH_MON_B3_167_003" },
	{ 13498, "ANH_MON_B3_167_004" },
	{ 13501, "ANH_MON_B3_167_005" },
	{ 13493, "ANH_MON_B3_167_006" },
	{ 13503, "ANH_MON_B3_167_008" },
	{ 13480, "ANH_MON_B3_167_010" },
	{ 13492, "ANH_MON_B3_167_011" },
	{ 13460, "ANH_MON_B3_167_012" },
	{ 13466, "ANH_MON_B3_167_013" },
	{ 13500, "ANH_MON_B3_167_014" },
	{ 13475, "ANH_MON_B3_167_020" },
	{ 13487, "ANH_MON_B3_167_021" },
	{ 13455, "ANH_MON_B3_167_022" },
	{ 13472, "ANH_MON_B3_167_040" },
	{ 13484, "ANH_MON_B3_167_041" },
	{ 13452, "ANH_MON_B3_167_042" },
	{ 13461, "ANH_MON_B3_167_043" },
	{ 13495, "ANH_MON_B3_167_044" },
	{ 13474, "ANH_MON_B3_167_050" },
	{ 13486, "ANH_MON_B3_167_051" },
	{ 13454, "ANH_MON_B3_167_052" },
	{ 13468, "ANH_MON_B3_167_060" },
	{ 13470, "ANH_MON_B3_167_070" },
	{ 13482, "ANH_MON_B3_167_071" },
	{ 9325, "ANH_MON_B3_167_B" },
	{ 8788, "ANH_MON_B3_167_P" },
	{ 5491, "ANH_MON_B3_168_000" },
	{ 5485, "ANH_MON_B3_168_002" },
	{ 5487, "ANH_MON_B3_168_003" },
	{ 5489, "ANH_MON_B3_168_007" },
	{ 5475, "ANH_MON_B3_168_010" },
	{ 5477, "ANH_MON_B3_168_011" },
	{ 5479, "ANH_MON_B3_168_012" },
	{ 5481, "ANH_MON_B3_168_013" },
	{ 5483, "ANH_MON_B3_168_014" },
	{ 10297, "ANH_MON_B3_168_B" },
	{ 10299, "ANH_MON_B3_168_P" },
	{ 9963, "ANH_MON_B3_169_000" },
	{ 9957, "ANH_MON_B3_169_002" },
	{ 9959, "ANH_MON_B3_169_003" },
	{ 9961, "ANH_MON_B3_169_007" },
	{ 9947, "ANH_MON_B3_169_010" },
	{ 9949, "ANH_MON_B3_169_011" },
	{ 9950, "ANH_MON_B3_169_012" },
	{ 9953, "ANH_MON_B3_169_013" },
	{ 9955, "ANH_MON_B3_169_014" },
	{ 9965, "ANH_MON_B3_169_020" },
	{ 9967, "ANH_MON_B3_169_021" },
	{ 9969, "ANH_MON_B3_169_022" },
	{ 9942, "ANH_MON_B3_169_B" },
	{ 9944, "ANH_MON_B3_169_P" },
	{ 11213, "ANH_MON_B3_170_000" },
	{ 11203, "ANH_MON_B3_170_002" },
	{ 11207, "ANH_MON_B3_170_003" },
	{ 11220, "ANH_MON_B3_170_007" },
	{ 11215, "ANH_MON_B3_170_010" },
	{ 11219, "ANH_MON_B3_170_011" },
	{ 11205, "ANH_MON_B3_170_012" },
	{ 11209, "ANH_MON_B3_170_013" },
	{ 11223, "ANH_MON_B3_170_014" },
	{ 11210, "ANH_MON_B3_170_020" },
	{ 11216, "ANH_MON_B3_170_021" },
	{ 11200, "ANH_MON_B3_170_022" },
	{ 10309, "ANH_MON_B3_170_B" },
	{ 10311, "ANH_MON_B3_170_P" },
	{ 12251, "ANH_MON_B3_171_000" },
	{ 12243, "ANH_MON_B3_171_002" },
	{ 12247, "ANH_MON_B3_171_003" },
	{ 12256, "ANH_MON_B3_171_007" },
	{ 12253, "ANH_MON_B3_171_010" },
	{ 12255, "ANH_MON_B3_171_011" },
	{ 12245, "ANH_MON_B3_171_012" },
	{ 12249, "ANH_MON_B3_171_013" },
	{ 12259, "ANH_MON_B3_171_014" },
	{ 5471, "ANH_MON_B3_171_B" },
	{ 5473, "ANH_MON_B3_171_P" },
	{ 2567, "ANH_MON_B3_172_000" },
	{ 2565, "ANH_MON_B3_172_002" },
	{ 2867, "ANH_MON_B3_172_003" },
	{ 2869, "ANH_MON_B3_172_007" },
	{ 2343, "ANH_MON_B3_172_010" },
	{ 2345, "ANH_MON_B3_172_011" },
	{ 2367, "ANH_MON_B3_172_012" },
	{ 2369, "ANH_MON_B3_172_013" },
	{ 2443, "ANH_MON_B3_172_014" },
	{ 2779, "ANH_MON_B3_172_020" },
	{ 2781, "ANH_MON_B3_172_021" },
	{ 2795, "ANH_MON_B3_172_022" },
	{ 10305, "ANH_MON_B3_172_B" },
	{ 10307, "ANH_MON_B3_172_P" },
	{ 5577, "ANH_MON_B3_173_000" },
	{ 5571, "ANH_MON_B3_173_002" },
	{ 5573, "ANH_MON_B3_173_003" },
	{ 5575, "ANH_MON_B3_173_007" },
	{ 5561, "ANH_MON_B3_173_010" },
	{ 5563, "ANH_MON_B3_173_011" },
	{ 5565, "ANH_MON_B3_173_012" },
	{ 5567, "ANH_MON_B3_173_013" },
	{ 5569, "ANH_MON_B3_173_014" },
	{ 5579, "ANH_MON_B3_173_020" },
	{ 5581, "ANH_MON_B3_173_021" },
	{ 5583, "ANH_MON_B3_173_022" },
	{ 10301, "ANH_MON_B3_173_B" },
	{ 10303, "ANH_MON_B3_173_P" },
	{ 14705, "ANH_MON_B3_174_000" },
	{ 14699, "ANH_MON_B3_174_002" },
	{ 14701, "ANH_MON_B3_174_003" },
	{ 14703, "ANH_MON_B3_174_007" },
	{ 14689, "ANH_MON_B3_174_010" },
	{ 14691, "ANH_MON_B3_174_011" },
	{ 14693, "ANH_MON_B3_174_012" },
	{ 14695, "ANH_MON_B3_174_013" },
	{ 14697, "ANH_MON_B3_174_014" },
	{ 10317, "ANH_MON_B3_174_B" },
	{ 10319, "ANH_MON_B3_174_P" },
	{ 5600, "ANH_MON_B3_175_000" },
	{ 5595, "ANH_MON_B3_175_002" },
	{ 5597, "ANH_MON_B3_175_003" },
	{ 5599, "ANH_MON_B3_175_007" },
	{ 5585, "ANH_MON_B3_175_010" },
	{ 5586, "ANH_MON_B3_175_011" },
	{ 5589, "ANH_MON_B3_175_012" },
	{ 5591, "ANH_MON_B3_175_013" },
	{ 5593, "ANH_MON_B3_175_014" },
	{ 10313, "ANH_MON_B3_175_B" },
	{ 10315, "ANH_MON_B3_175_P" },
	{ 13526, "ANH_MON_B3_176_000" },
	{ 13512, "ANH_MON_B3_176_002" },
	{ 13518, "ANH_MON_B3_176_003" },
	{ 13535, "ANH_MON_B3_176_007" },
	{ 13528, "ANH_MON_B3_176_010" },
	{ 13534, "ANH_MON_B3_176_011" },
	{ 13514, "ANH_MON_B3_176_012" },
	{ 13520, "ANH_MON_B3_176_013" },
	{ 13538, "ANH_MON_B3_176_014" },
	{ 13522, "ANH_MON_B3_176_020" },
	{ 13530, "ANH_MON_B3_176_021" },
	{ 13508, "ANH_MON_B3_176_022" },
	{ 13524, "ANH_MON_B3_176_030" },
	{ 13532, "ANH_MON_B3_176_031" },
	{ 13510, "ANH_MON_B3_176_032" },
	{ 9667, "ANH_MON_B3_176_B" },
	{ 13506, "ANH_MON_B3_176_ILLUST" },
	{ 9669, "ANH_MON_B3_176_P" },
	{ 13515, "ANH_MON_B3_176_TEST" },
	{ 10823, "ANH_MON_B3_177_000" },
	{ 10813, "ANH_MON_B3_177_002" },
	{ 10817, "ANH_MON_B3_177_003" },
	{ 10832, "ANH_MON_B3_177_007" },
	{ 11224, "ANH_MON_B3_177_008" },
	{ 10825, "ANH_MON_B3_177_010" },
	{ 10829, "ANH_MON_B3_177_011" },
	{ 10815, "ANH_MON_B3_177_012" },
	{ 10819, "ANH_MON_B3_177_013" },
	{ 10835, "ANH_MON_B3_177_014" },
	{ 10820, "ANH_MON_B3_177_020" },
	{ 10826, "ANH_MON_B3_177_021" },
	{ 10810, "ANH_MON_B3_177_022" },
	{ 11227, "ANH_MON_B3_177_B" },
	{ 11229, "ANH_MON_B3_177_P" },
	{ 11251, "ANH_MON_B3_178_000" },
	{ 11239, "ANH_MON_B3_178_002" },
	{ 11245, "ANH_MON_B3_178_003" },
	{ 11260, "ANH_MON_B3_178_007" },
	{ 11253, "ANH_MON_B3_178_010" },
	{ 11259, "ANH_MON_B3_178_011" },
	{ 11241, "ANH_MON_B3_178_012" },
	{ 11247, "ANH_MON_B3_178_013" },
	{ 11263, "ANH_MON_B3_178_014" },
	{ 11248, "ANH_MON_B3_178_020" },
	{ 11256, "ANH_MON_B3_178_021" },
	{ 11236, "ANH_MON_B3_178_022" },
	{ 11243, "ANH_MON_B3_178_B" },
	{ 11255, "ANH_MON_B3_178_P" },
	{ 13074, "ANH_MON_B3_179_000" },
	{ 11230, "ANH_MON_B3_179_002" },
	{ 11232, "ANH_MON_B3_179_003" },
	{ 11234, "ANH_MON_B3_179_007" },
	{ 174, "ANH_MON_B3_179_020" },
	{ 13076, "ANH_MON_B3_179_021" },
	{ 170, "ANH_MON_B3_179_022" },
	{ 7062, "ANH_MON_B3_179_B" },
	{ 7064, "ANH_MON_B3_179_P" },
	{ 11424, "ANH_MON_B3_180_000" },
	{ 11414, "ANH_MON_B3_180_002" },
	{ 11418, "ANH_MON_B3_180_003" },
	{ 11432, "ANH_MON_B3_180_007" },
	{ 11426, "ANH_MON_B3_180_010" },
	{ 11431, "ANH_MON_B3_180_011" },
	{ 11416, "ANH_MON_B3_180_012" },
	{ 11420, "ANH_MON_B3_180_013" },
	{ 11435, "ANH_MON_B3_180_014" },
	{ 11422, "ANH_MON_B3_180_020" },
	{ 11428, "ANH_MON_B3_180_021" },
	{ 11412, "ANH_MON_B3_180_022" },
	{ 11437, "ANH_MON_B3_180_B" },
	{ 11439, "ANH_MON_B3_180_P" },
	{ 11400, "ANH_MON_B3_181_000" },
	{ 2853, "ANH_MON_B3_181_002" },
	{ 11394, "ANH_MON_B3_181_003" },
	{ 11408, "ANH_MON_B3_181_007" },
	{ 11402, "ANH_MON_B3_181_010" },
	{ 11407, "ANH_MON_B3_181_011" },
	{ 11393, "ANH_MON_B3_181_012" },
	{ 11396, "ANH_MON_B3_181_013" },
	{ 11411, "ANH_MON_B3_181_014" },
	{ 11398, "ANH_MON_B3_181_020" },
	{ 11404, "ANH_MON_B3_181_021" },
	{ 11390, "ANH_MON_B3_181_022" },
	{ 10413, "ANH_MON_B3_181_B" },
	{ 10415, "ANH_MON_B3_181_P" },
	{ 5619, "ANH_MON_B3_182_000" },
	{ 5612, "ANH_MON_B3_182_002" },
	{ 5614, "ANH_MON_B3_182_003" },
	{ 5616, "ANH_MON_B3_182_007" },
	{ 5603, "ANH_MON_B3_182_010" },
	{ 5605, "ANH_MON_B3_182_011" },
	{ 5606, "ANH_MON_B3_182_012" },
	{ 5608, "ANH_MON_B3_182_013" },
	{ 5611, "ANH_MON_B3_182_014" },
	{ 5620, "ANH_MON_B3_182_030" },
	{ 8843, "ANH_MON_B3_182_040" },
	{ 8844, "ANH_MON_B3_182_041" },
	{ 10321, "ANH_MON_B3_182_B" },
	{ 10323, "ANH_MON_B3_182_P" },
	{ 7669, "ANH_MON_B3_183_002" },
	{ 7671, "ANH_MON_B3_183_007" },
	{ 9355, "ANH_MON_B3_183_B" },
	{ 8878, "ANH_MON_B3_183_P" },
	{ 7673, "ANH_MON_B3_184_002" },
	{ 7675, "ANH_MON_B3_184_007" },
	{ 9357, "ANH_MON_B3_184_B" },
	{ 8884, "ANH_MON_B3_184_P" },
	{ 12555, "ANH_MON_B3_185_000" },
	{ 1394, "ANH_MON_B3_185_003" },
	{ 1396, "ANH_MON_B3_185_007" },
	{ 1503, "ANH_MON_B3_185_008" },
	{ 1385, "ANH_MON_B3_185_010" },
	{ 1392, "ANH_MON_B3_185_011" },
	{ 1501, "ANH_MON_B3_185_012" },
	{ 1409, "ANH_MON_B3_185_020" },
	{ 1411, "ANH_MON_B3_185_021" },
	{ 1499, "ANH_MON_B3_185_022" },
	{ 1402, "ANH_MON_B3_185_040" },
	{ 10014, "ANH_MON_B3_185_041" },
	{ 10016, "ANH_MON_B3_185_042" },
	{ 10018, "ANH_MON_B3_185_043" },
	{ 10020, "ANH_MON_B3_185_044" },
	{ 10417, "ANH_MON_B3_185_B" },
	{ 10419, "ANH_MON_B3_185_P" },
	{ 418, "ANH_MON_B3_186_000" },
	{ 410, "ANH_MON_B3_186_003" },
	{ 414, "ANH_MON_B3_186_020" },
	{ 433, "ANH_MON_B3_186_021" },
	{ 349, "ANH_MON_B3_186_022" },
	{ 416, "ANH_MON_B3_186_030" },
	{ 435, "ANH_MON_B3_186_031" },
	{ 408, "ANH_MON_B3_186_032" },
	{ 412, "ANH_MON_B3_186_040" },
	{ 425, "ANH_MON_B3_186_041" },
	{ 347, "ANH_MON_B3_186_042" },
	{ 14589, "ANH_MON_B3_186_B" },
	{ 422, "ANH_MON_B3_186_ILLUST" },
	{ 14591, "ANH_MON_B3_186_P" },
	{ 437, "ANH_MON_B3_186_TEST" },
	{ 13641, "ANH_MON_B3_187_000" },
	{ 13631, "ANH_MON_B3_187_002" },
	{ 13635, "ANH_MON_B3_187_003" },
	{ 13649, "ANH_MON_B3_187_007" },
	{ 13639, "ANH_MON_B3_187_020" },
	{ 13647, "ANH_MON_B3_187_021" },
	{ 13629, "ANH_MON_B3_187_022" },
	{ 13637, "ANH_MON_B3_187_040" },
	{ 13645, "ANH_MON_B3_187_041" },
	{ 9356, "ANH_MON_B3_187_B" },
	{ 13644, "ANH_MON_B3_187_ILLUST" },
	{ 8881, "ANH_MON_B3_187_P" },
	{ 13633, "ANH_MON_B3_187_TEST" },
	{ 13661, "ANH_MON_B3_188_000" },
	{ 13653, "ANH_MON_B3_188_002" },
	{ 13655, "ANH_MON_B3_188_003" },
	{ 13667, "ANH_MON_B3_188_007" },
	{ 13659, "ANH_MON_B3_188_020" },
	{ 13665, "ANH_MON_B3_188_021" },
	{ 13651, "ANH_MON_B3_188_022" },
	{ 13657, "ANH_MON_B3_188_040" },
	{ 13663, "ANH_MON_B3_188_041" },
	{ 9359, "ANH_MON_B3_188_B" },
	{ 8890, "ANH_MON_B3_188_P" },
	{ 14024, "ANH_MON_B3_189_000" },
	{ 14012, "ANH_MON_B3_189_002" },
	{ 14016, "ANH_MON_B3_189_003" },
	{ 14034, "ANH_MON_B3_189_007" },
	{ 14026, "ANH_MON_B3_189_010" },
	{ 14033, "ANH_MON_B3_189_011" },
	{ 14014, "ANH_MON_B3_189_012" },
	{ 14018, "ANH_MON_B3_189_013" },
	{ 14039, "ANH_MON_B3_189_014" },
	{ 14020, "ANH_MON_B3_189_020" },
	{ 14028, "ANH_MON_B3_189_021" },
	{ 14008, "ANH_MON_B3_189_022" },
	{ 14022, "ANH_MON_B3_189_030" },
	{ 14030, "ANH_MON_B3_189_031" },
	{ 14010, "ANH_MON_B3_189_032" },
	{ 14624, "ANH_MON_B3_189_033" },
	{ 14037, "ANH_MON_B3_189_034" },
	{ 9519, "ANH_MON_B3_189_035" },
	{ 9659, "ANH_MON_B3_189_036" },
	{ 9660, "ANH_MON_B3_189_037" },
	{ 9666, "ANH_MON_B3_189_038" },
	{ 9358, "ANH_MON_B3_189_B" },
	{ 8887, "ANH_MON_B3_189_P" },
	{ 13703, "ANH_MON_B3_190_000" },
	{ 13687, "ANH_MON_B3_190_002" },
	{ 13695, "ANH_MON_B3_190_003" },
	{ 13713, "ANH_MON_B3_190_007" },
	{ 13705, "ANH_MON_B3_190_010" },
	{ 13712, "ANH_MON_B3_190_011" },
	{ 13689, "ANH_MON_B3_190_012" },
	{ 13697, "ANH_MON_B3_190_013" },
	{ 13718, "ANH_MON_B3_190_014" },
	{ 13699, "ANH_MON_B3_190_020" },
	{ 13707, "ANH_MON_B3_190_021" },
	{ 13683, "ANH_MON_B3_190_022" },
	{ 13701, "ANH_MON_B3_190_030" },
	{ 13709, "ANH_MON_B3_190_031" },
	{ 13685, "ANH_MON_B3_190_032" },
	{ 13694, "ANH_MON_B3_190_033" },
	{ 13716, "ANH_MON_B3_190_034" },
	{ 9360, "ANH_MON_B3_190_B" },
	{ 13682, "ANH_MON_B3_190_ILLUST" },
	{ 8893, "ANH_MON_B3_190_P" },
	{ 13691, "ANH_MON_B3_190_TEST" },
	{ 14627, "ANH_MON_B3_191_000" },
	{ 14629, "ANH_MON_B3_191_001" },
	{ 14631, "ANH_MON_B3_191_003" },
	{ 14633, "ANH_MON_B3_191_004" },
	{ 14635, "ANH_MON_B3_191_005" },
	{ 14637, "ANH_MON_B3_191_008" },
	{ 11858, "ANH_MON_B3_191_009" },
	{ 14639, "ANH_MON_B3_191_010" },
	{ 14641, "ANH_MON_B3_191_011" },
	{ 14643, "ANH_MON_B3_191_012" },
	{ 14645, "ANH_MON_B3_191_013" },
	{ 14647, "ANH_MON_B3_191_014" },
	{ 14649, "ANH_MON_B3_191_020" },
	{ 14651, "ANH_MON_B3_191_021" },
	{ 14653, "ANH_MON_B3_191_022" },
	{ 14655, "ANH_MON_B3_191_030" },
	{ 14657, "ANH_MON_B3_191_031" },
	{ 14659, "ANH_MON_B3_191_032" },
	{ 11860, "ANH_MON_B3_191_040" },
	{ 11862, "ANH_MON_B3_191_041" },
	{ 11864, "ANH_MON_B3_191_042" },
	{ 11866, "ANH_MON_B3_191_043" },
	{ 11868, "ANH_MON_B3_191_044" },
	{ 12546, "ANH_MON_B3_191_B" },
	{ 14661, "ANH_MON_B3_191_ILLUST" },
	{ 12548, "ANH_MON_B3_191_P" },
	{ 14663, "ANH_MON_B3_191_TEST" },
	{ 14118, "ANH_MON_B3_192_000" },
	{ 14102, "ANH_MON_B3_192_002" },
	{ 14110, "ANH_MON_B3_192_003" },
	{ 14130, "ANH_MON_B3_192_007" },
	{ 14120, "ANH_MON_B3_192_010" },
	{ 14127, "ANH_MON_B3_192_011" },
	{ 14104, "ANH_MON_B3_192_012" },
	{ 14112, "ANH_MON_B3_192_013" },
	{ 14135, "ANH_MON_B3_192_014" },
	{ 9995, "ANH_MON_B3_192_020" },
	{ 9996, "ANH_MON_B3_192_021" },
	{ 9999, "ANH_MON_B3_192_022" },
	{ 14117, "ANH_MON_B3_192_030" },
	{ 14125, "ANH_MON_B3_192_031" },
	{ 14101, "ANH_MON_B3_192_032" },
	{ 14109, "ANH_MON_B3_192_033" },
	{ 14133, "ANH_MON_B3_192_034" },
	{ 14137, "ANH_MON_B3_192_035" },
	{ 14129, "ANH_MON_B3_192_036" },
	{ 14114, "ANH_MON_B3_192_040" },
	{ 14122, "ANH_MON_B3_192_041" },
	{ 14098, "ANH_MON_B3_192_042" },
	{ 12542, "ANH_MON_B3_192_B" },
	{ 14097, "ANH_MON_B3_192_ILLUST" },
	{ 12544, "ANH_MON_B3_192_P" },
	{ 14106, "ANH_MON_B3_192_TEST" },
	{ 14076, "ANH_MON_B3_193_000" },
	{ 14058, "ANH_MON_B3_193_002" },
	{ 14064, "ANH_MON_B3_193_003" },
	{ 14092, "ANH_MON_B3_193_007" },
	{ 14046, "ANH_MON_B3_193_008" },
	{ 14048, "ANH_MON_B3_193_009" },
	{ 14078, "ANH_MON_B3_193_010" },
	{ 14091, "ANH_MON_B3_193_011" },
	{ 14060, "ANH_MON_B3_193_012" },
	{ 14072, "ANH_MON_B3_193_020" },
	{ 14086, "ANH_MON_B3_193_021" },
	{ 14054, "ANH_MON_B3_193_022" },
	{ 14074, "ANH_MON_B3_193_030" },
	{ 14088, "ANH_MON_B3_193_031" },
	{ 14056, "ANH_MON_B3_193_032" },
	{ 14068, "ANH_MON_B3_193_040" },
	{ 14082, "ANH_MON_B3_193_041" },
	{ 14050, "ANH_MON_B3_193_042" },
	{ 14062, "ANH_MON_B3_193_043" },
	{ 14094, "ANH_MON_B3_193_044" },
	{ 14070, "ANH_MON_B3_193_050" },
	{ 14084, "ANH_MON_B3_193_051" },
	{ 14052, "ANH_MON_B3_193_052" },
	{ 14066, "ANH_MON_B3_193_060" },
	{ 14080, "ANH_MON_B3_193_061" },
	{ 12538, "ANH_MON_B3_193_B" },
	{ 12540, "ANH_MON_B3_193_P" },
	{ 14176, "ANH_MON_B3_194_000" },
	{ 14148, "ANH_MON_B3_194_002" },
	{ 14158, "ANH_MON_B3_194_003" },
	{ 14196, "ANH_MON_B3_194_007" },
	{ 14178, "ANH_MON_B3_194_010" },
	{ 14193, "ANH_MON_B3_194_011" },
	{ 14150, "ANH_MON_B3_194_012" },
	{ 14160, "ANH_MON_B3_194_013" },
	{ 14203, "ANH_MON_B3_194_014" },
	{ 14174, "ANH_MON_B3_194_020" },
	{ 14190, "ANH_MON_B3_194_021" },
	{ 14146, "ANH_MON_B3_194_022" },
	{ 14170, "ANH_MON_B3_194_040" },
	{ 14186, "ANH_MON_B3_194_041" },
	{ 14142, "ANH_MON_B3_194_042" },
	{ 14154, "ANH_MON_B3_194_043" },
	{ 14198, "ANH_MON_B3_194_044" },
	{ 14172, "ANH_MON_B3_194_050" },
	{ 14188, "ANH_MON_B3_194_051" },
	{ 14144, "ANH_MON_B3_194_052" },
	{ 14156, "ANH_MON_B3_194_053" },
	{ 14200, "ANH_MON_B3_194_054" },
	{ 14166, "ANH_MON_B3_194_060" },
	{ 14182, "ANH_MON_B3_194_061" },
	{ 14138, "ANH_MON_B3_194_062" },
	{ 14168, "ANH_MON_B3_194_070" },
	{ 14184, "ANH_MON_B3_194_071" },
	{ 14140, "ANH_MON_B3_194_072" },
	{ 14180, "ANH_MON_B3_194_080" },
	{ 14194, "ANH_MON_B3_194_081" },
	{ 14152, "ANH_MON_B3_194_082" },
	{ 14162, "ANH_MON_B3_194_083" },
	{ 14204, "ANH_MON_B3_194_084" },
	{ 12550, "ANH_MON_B3_194_B" },
	{ 12552, "ANH_MON_B3_194_P" },
	{ 14164, "ANH_MON_B3_194_P2" },
	{ 2994, "ANH_MON_B3_195_000" },
	{ 13180, "ANH_MON_B3_195_B" },
	{ 13738, "ANH_MON_B3_195_P" },
	{ 11961, "ANH_MON_B3_196_000" },
	{ 4070, "ANH_MON_B3_196_B" },
	{ 8326, "ANH_MON_B3_197_000" },
	{ 8329, "ANH_MON_B3_197_002" },
	{ 8330, "ANH_MON_B3_197_003" },
	{ 8358, "ANH_MON_B3_197_007" },
	{ 8360, "ANH_MON_B3_197_020" },
	{ 8362, "ANH_MON_B3_197_021" },
	{ 8364, "ANH_MON_B3_197_022" },
	{ 8366, "ANH_MON_B3_197_030" },
	{ 8368, "ANH_MON_B3_197_031" },
	{ 8370, "ANH_MON_B3_197_032" },
	{ 8372, "ANH_MON_B3_197_033" },
	{ 8374, "ANH_MON_B3_197_034" },
	{ 7968, "ANH_MON_B3_197_B" },
	{ 7970, "ANH_MON_B3_197_P" },
	{ 6616, "ANH_MON_B3_198_000" },
	{ 6619, "ANH_MON_B3_198_002" },
	{ 6620, "ANH_MON_B3_198_003" },
	{ 7182, "ANH_MON_B3_198_007" },
	{ 7184, "ANH_MON_B3_198_010" },
	{ 7186, "ANH_MON_B3_198_011" },
	{ 7188, "ANH_MON_B3_198_012" },
	{ 7190, "ANH_MON_B3_198_013" },
	{ 7192, "ANH_MON_B3_198_014" },
	{ 7971, "ANH_MON_B3_198_020" },
	{ 7974, "ANH_MON_B3_198_021" },
	{ 8225, "ANH_MON_B3_198_022" },
	{ 2315, "ANH_MON_B3_198_B" },
	{ 2316, "ANH_MON_B3_198_P" },
	{ 10673, "ANH_MON_B3_199_000" },
	{ 10773, "ANH_MON_B3_199_001" },
	{ 11202, "ANH_MON_B3_199_003" },
	{ 11262, "ANH_MON_B3_199_004" },
	{ 11264, "ANH_MON_B3_199_006" },
	{ 11171, "ANH_MON_B3_199_010" },
	{ 11173, "ANH_MON_B3_199_011" },
	{ 11175, "ANH_MON_B3_199_012" },
	{ 11011, "ANH_MON_B3_199_020" },
	{ 11012, "ANH_MON_B3_199_021" },
	{ 11014, "ANH_MON_B3_199_022" },
	{ 11266, "ANH_MON_B3_199_030" },
	{ 11268, "ANH_MON_B3_199_031" },
	{ 10732, "ANH_MON_B3_199_032" },
	{ 11165, "ANH_MON_B3_199_040" },
	{ 11167, "ANH_MON_B3_199_041" },
	{ 11169, "ANH_MON_B3_199_042" },
	{ 11177, "ANH_MON_B3_199_043" },
	{ 11179, "ANH_MON_B3_199_044" },
	{ 10735, "ANH_MON_B3_199_050" },
	{ 10737, "ANH_MON_B3_199_051" },
	{ 10739, "ANH_MON_B3_199_052" },
	{ 10753, "ANH_MON_B3_199_053" },
	{ 11295, "ANH_MON_B3_199_054" },
	{ 11535, "ANH_MON_B3_199_055" },
	{ 10183, "ANH_MON_B3_199_100" },
	{ 10669, "ANH_MON_B3_199_B" },
	{ 10671, "ANH_MON_B3_199_P" },
	{ 10695, "ANH_MON_B3_200_000" },
	{ 10675, "ANH_MON_B3_200_B" },
	{ 10712, "ANH_MON_B3_200_P" },
	{ 10785, "ANH_MON_B3_205_000" },
	{ 10787, "ANH_MON_B3_205_002" },
	{ 10793, "ANH_MON_B3_205_003" },
	{ 10795, "ANH_MON_B3_205_007" },
	{ 10796, "ANH_MON_B3_205_010" },
	{ 10799, "ANH_MON_B3_205_011" },
	{ 10801, "ANH_MON_B3_205_012" },
	{ 10803, "ANH_MON_B3_205_013" },
	{ 10805, "ANH_MON_B3_205_014" },
	{ 10807, "ANH_MON_B3_205_020" },
	{ 10809, "ANH_MON_B3_205_021" },
	{ 10811, "ANH_MON_B3_205_022" },
	{ 10812, "ANH_MON_B3_205_030" },
	{ 10814, "ANH_MON_B3_205_031" },
	{ 10816, "ANH_MON_B3_205_032" },
	{ 10818, "ANH_MON_B3_205_033" },
	{ 10821, "ANH_MON_B3_205_034" },
	{ 10822, "ANH_MON_B3_205_040" },
	{ 10824, "ANH_MON_B3_205_041" },
	{ 10827, "ANH_MON_B3_205_042" },
	{ 10828, "ANH_MON_B3_205_043" },
	{ 10833, "ANH_MON_B3_205_044" },
	{ 10783, "ANH_MON_B3_205_B" },
	{ 10834, "ANH_MON_B3_205_P" },
	{ 12500, "ANH_WEP_B1_011_B" },
	{ 12502, "ANH_WEP_B1_011_P" },
	{ 9536, "ANH_WEP_B1_012_B" },
	{ 9538, "ANH_WEP_B1_012_P" },
	{ 9532, "ANH_WEP_B1_013_B" },
	{ 9534, "ANH_WEP_B1_013_P" },
	{ 9544, "ANH_WEP_B1_014_B" },
	{ 9546, "ANH_WEP_B1_014_P" },
	{ 12744, "ANH_WEP_B1_015_B" },
	{ 12746, "ANH_WEP_B1_015_P" },
	{ 9540, "ANH_WEP_B1_016_B" },
	{ 9542, "ANH_WEP_B1_016_P" },
	{ 12690, "ANH_WEP_B1_017_B" },
	{ 12692, "ANH_WEP_B1_017_P" },
	{ 12726, "ANH_WEP_B1_018_B" },
	{ 12728, "ANH_WEP_B1_018_P" },
	{ 12722, "ANH_WEP_B1_019_B" },
	{ 12724, "ANH_WEP_B1_019_P" },
	{ 12706, "ANH_WEP_B1_020_B" },
	{ 12708, "ANH_WEP_B1_020_P" },
	{ 12702, "ANH_WEP_B1_021_B" },
	{ 12704, "ANH_WEP_B1_021_P" },
	{ 12698, "ANH_WEP_B1_022_B" },
	{ 12700, "ANH_WEP_B1_022_P" },
	{ 12694, "ANH_WEP_B1_023_B" },
	{ 12696, "ANH_WEP_B1_023_P" },
	{ 12718, "ANH_WEP_B1_024_B" },
	{ 12720, "ANH_WEP_B1_024_P" },
	{ 12714, "ANH_WEP_B1_025_B" },
	{ 12716, "ANH_WEP_B1_025_P" },
	{ 12710, "ANH_WEP_B1_026_B" },
	{ 12712, "ANH_WEP_B1_026_P" },
	{ 9512, "ANH_WEP_B1_027_B" },
	{ 9514, "ANH_WEP_B1_027_P" },
	{ 9520, "ANH_WEP_B1_028_B" },
	{ 9522, "ANH_WEP_B1_028_P" },
	{ 9516, "ANH_WEP_B1_029_B" },
	{ 9518, "ANH_WEP_B1_029_P" },
	{ 13092, "ANH_WEP_B1_030_B" },
	{ 13094, "ANH_WEP_B1_030_P" },
	{ 12400, "ANH_WEP_B1_031_B" },
	{ 12402, "ANH_WEP_B1_031_P" },
	{ 9524, "ANH_WEP_B1_032_B" },
	{ 9526, "ANH_WEP_B1_032_P" },
	{ 13088, "ANH_WEP_B1_033_B" },
	{ 13090, "ANH_WEP_B1_033_P" },
	{ 13104, "ANH_WEP_B1_034_B" },
	{ 13106, "ANH_WEP_B1_034_P" },
	{ 13100, "ANH_WEP_B1_035_B" },
	{ 13102, "ANH_WEP_B1_035_P" },
	{ 13096, "ANH_WEP_B1_036_B" },
	{ 13098, "ANH_WEP_B1_036_P" },
	{ 9528, "ANH_WEP_B1_037_B" },
	{ 9530, "ANH_WEP_B1_037_P" },
	{ 12404, "ANH_WEP_B1_038_B" },
	{ 12406, "ANH_WEP_B1_038_P" },
	{ 13108, "ANH_WEP_B1_039_B" },
	{ 13110, "ANH_WEP_B1_039_P" },
	{ 12384, "ANH_WEP_B1_040_B" },
	{ 12386, "ANH_WEP_B1_040_P" },
	{ 13084, "ANH_WEP_B1_041_B" },
	{ 13086, "ANH_WEP_B1_041_P" },
	{ 9480, "ANH_WEP_B1_042_B" },
	{ 9482, "ANH_WEP_B1_042_P" },
	{ 9476, "ANH_WEP_B1_043_B" },
	{ 9478, "ANH_WEP_B1_043_P" },
	{ 12736, "ANH_WEP_B1_044_B" },
	{ 12738, "ANH_WEP_B1_044_P" },
	{ 9488, "ANH_WEP_B1_045_B" },
	{ 9490, "ANH_WEP_B1_045_P" },
	{ 9484, "ANH_WEP_B1_046_B" },
	{ 9486, "ANH_WEP_B1_046_P" },
	{ 12388, "ANH_WEP_B1_047_B" },
	{ 12390, "ANH_WEP_B1_047_P" },
	{ 12396, "ANH_WEP_B1_048_B" },
	{ 12398, "ANH_WEP_B1_048_P" },
	{ 12392, "ANH_WEP_B1_049_B" },
	{ 12394, "ANH_WEP_B1_049_P" },
	{ 12678, "ANH_WEP_B1_050_B" },
	{ 12680, "ANH_WEP_B1_050_P" },
	{ 13744, "ANH_WEP_B1_051_B" },
	{ 13746, "ANH_WEP_B1_051_P" },
	{ 9496, "ANH_WEP_B1_052_B" },
	{ 9498, "ANH_WEP_B1_052_P" },
	{ 9492, "ANH_WEP_B1_053_B" },
	{ 9494, "ANH_WEP_B1_053_P" },
	{ 9504, "ANH_WEP_B1_054_B" },
	{ 9506, "ANH_WEP_B1_054_P" },
	{ 12740, "ANH_WEP_B1_055_B" },
	{ 12742, "ANH_WEP_B1_055_P" },
	{ 12682, "ANH_WEP_B1_056_B" },
	{ 12684, "ANH_WEP_B1_056_P" },
	{ 9500, "ANH_WEP_B1_057_B" },
	{ 9502, "ANH_WEP_B1_057_P" },
	{ 9508, "ANH_WEP_B1_058_B" },
	{ 9510, "ANH_WEP_B1_058_P" },
	{ 12686, "ANH_WEP_B1_059_B" },
	{ 12688, "ANH_WEP_B1_059_P" },
	{ 9446, "ANH_WEP_B1_060_B" },
	{ 9448, "ANH_WEP_B1_060_P" },
	{ 9442, "ANH_WEP_B1_061_B" },
	{ 9444, "ANH_WEP_B1_061_P" },
	{ 12376, "ANH_WEP_B1_062_B" },
	{ 12378, "ANH_WEP_B1_062_P" },
	{ 12416, "ANH_WEP_B1_063_B" },
	{ 12418, "ANH_WEP_B1_063_P" },
	{ 12428, "ANH_WEP_B1_064_B" },
	{ 12430, "ANH_WEP_B1_064_P" },
	{ 12424, "ANH_WEP_B1_065_B" },
	{ 12426, "ANH_WEP_B1_065_P" },
	{ 12420, "ANH_WEP_B1_066_B" },
	{ 12422, "ANH_WEP_B1_066_P" },
	{ 12380, "ANH_WEP_B1_067_B" },
	{ 12382, "ANH_WEP_B1_067_P" },
	{ 9454, "ANH_WEP_B1_068_B" },
	{ 9456, "ANH_WEP_B1_068_P" },
	{ 9450, "ANH_WEP_B1_069_B" },
	{ 9452, "ANH_WEP_B1_069_P" },
	{ 9462, "ANH_WEP_B1_070_B" },
	{ 9464, "ANH_WEP_B1_070_P" },
	{ 9458, "ANH_WEP_B1_071_B" },
	{ 9460, "ANH_WEP_B1_071_P" },
	{ 12436, "ANH_WEP_B1_072_B" },
	{ 12438, "ANH_WEP_B1_072_P" },
	{ 12432, "ANH_WEP_B1_073_B" },
	{ 12434, "ANH_WEP_B1_073_P" },
	{ 12440, "ANH_WEP_B1_074_B" },
	{ 12442, "ANH_WEP_B1_074_P" },
	{ 9469, "ANH_WEP_B1_075_B" },
	{ 9176, "ANH_WEP_B1_075_P" },
	{ 9466, "ANH_WEP_B1_076_B" },
	{ 9468, "ANH_WEP_B1_076_P" },
	{ 5961, "ANH_WEP_B1_077_B" },
	{ 9173, "ANH_WEP_B1_077_P" },
	{ 9474, "ANH_WEP_B1_078_B" },
	{ 9179, "ANH_WEP_B1_078_P" },
	{ 9471, "ANH_WEP_B1_079_B" },
	{ 9473, "ANH_WEP_B1_079_P" },
	{ 9559, "ANH_WEP_B1_080_B" },
	{ 9182, "ANH_WEP_B1_080_P" },
	{ 9556, "ANH_WEP_B1_081_B" },
	{ 9558, "ANH_WEP_B1_081_P" },
	{ 9552, "ANH_WEP_B1_082_B" },
	{ 9554, "ANH_WEP_B1_082_P" },
	{ 9548, "ANH_WEP_B1_083_B" },
	{ 9550, "ANH_WEP_B1_083_P" },
	{ 9573, "ANH_WEP_B1_084_B" },
	{ 9575, "ANH_WEP_B1_084_P" },
	{ 9569, "ANH_WEP_B1_085_B" },
	{ 9571, "ANH_WEP_B1_085_P" },
	{ 9565, "ANH_WEP_B1_086_B" },
	{ 9567, "ANH_WEP_B1_086_P" },
	{ 9561, "ANH_WEP_B1_087_B" },
	{ 9563, "ANH_WEP_B1_087_P" },
	{ 9581, "ANH_WEP_B1_088_B" },
	{ 9583, "ANH_WEP_B1_088_P" },
	{ 9577, "ANH_WEP_B1_089_B" },
	{ 9579, "ANH_WEP_B1_089_P" },
	{ 9593, "ANH_WEP_B1_090_B" },
	{ 9595, "ANH_WEP_B1_090_P" },
	{ 8442, "ANH_WEP_B1_091_B" },
	{ 8444, "ANH_WEP_B1_091_P" },
	{ 9589, "ANH_WEP_B1_092_B" },
	{ 9591, "ANH_WEP_B1_092_P" },
	{ 9585, "ANH_WEP_B1_093_B" },
	{ 9587, "ANH_WEP_B1_093_P" },
	{ 9597, "ANH_WEP_B1_094_B" },
	{ 9599, "ANH_WEP_B1_094_P" },
	{ 13112, "ANH_WEP_B1_095_B" },
	{ 13114, "ANH_WEP_B1_095_P" },
	{ 838, "ANM_MAIN_B2_001" },
	{ 14462, "ANM_MAIN_B2_CAM_011_1ST" },
	{ 14459, "ANM_MAIN_B2_CAM_011_2ND" },
	{ 14464, "ANM_MAIN_B2_CAM_011_DOWN" },
	{ 14457, "ANM_MAIN_B2_CAM_011_END" },
	{ 14452, "ANM_MAIN_B2_CAM_011_JUMP" },
	{ 14453, "ANM_MAIN_B2_CAM_011_OPENING" },
	{ 14456, "ANM_MAIN_B2_CAM_011_RUN" }
};

#include "ModelMesh.h"

void DummyPrintAnimDB(fstream& archive, EnemyDataSet* es, UnityArchiveMetaData* meta, UnityArchiveAssetBundle* bundle) {
	unsigned int i,j,k;
	int modelid,fileaid;
	uint8_t seqanimbase,seqanimamount;
	BattleDataStruct* bd = NULL;
	EnemyDataStruct* en = NULL;
	wxString filepath;
	string desc;
	bool ok;
	fstream fout("aaanim.txt",ios::out);
	for (i=0;i<G_N_ELEMENTS(DummyBattleAnim);i++) {
		ModelAnimationData animdata;
		modelid = -1;
		for (j=0;j<bundle->amount;j++) {
			filepath = bundle->path[j];
			fileaid = wxAtoi(filepath.AfterLast('//').BeforeFirst('.'));
			if (fileaid==DummyBattleAnim[i].id) {
				modelid = wxAtoi(filepath.BeforeLast('//').AfterLast('//'));
				archive.seekg(meta->GetFileOffsetByIndex(bundle->index[j]-1));
				animdata.Read(archive);
				break;
			}
		}
		desc = "";
		if (modelid<0)
			goto block_print;
		if ((animdata.locals_amount==0 || animdata.locals[0].transform_amount<=2) && (animdata.localt_amount==0 || animdata.localt[0].transform_amount<=2) && (animdata.localw_amount==0 || animdata.localw[0].transform_amount<=2)) {
			desc = "Dummy "+wxString(DummyBattleAnim[i].name).AfterLast('_').Capitalize();
			goto block_print;
		}
		BattleModelLinks& modellink = BattleModelLinks::GetLinksByModel(modelid);
		if (modellink.model==0xFFFF)
			goto block_searchanimseq;
		if (DummyBattleAnim[i].id==modellink.anim_idle) {
			desc = "Idle";
			goto block_print;
		} else if (DummyBattleAnim[i].id==modellink.anim_hit) {
			desc = "Hit";
			goto block_print;
		} else if (DummyBattleAnim[i].id==modellink.anim_death) {
			desc = "Death";
			goto block_print;
		} else if (DummyBattleAnim[i].id==modellink.anim_idle_alt) {
			desc = "Idle Alternate";
			goto block_print;
		} else if (DummyBattleAnim[i].id==modellink.anim_hit_alt) {
			desc = "Hit Alternate";
			goto block_print;
		} else if (DummyBattleAnim[i].id==modellink.anim_death_alt) {
			desc = "Death Alternate";
			goto block_print;
		}
		block_searchanimseq:
		ok = false;
		for (j=0;j<es->battle_amount;j++) {
			for (k=0;k<es->battle[j]->stat_amount;k++) {
				if (es->battle[j]->stat[k].model==modelid) {
					seqanimbase = es->battle[j]->stat[k].sequence_anim_base;
					seqanimamount = es->battle[j]->stat[k].sequence_anim_amount;
					bd = es->battle_data[j];
					en = es->battle[j];
					j = es->battle_amount;
					ok = true;
					break;
				}
			}
		}
		if (!ok)
			goto block_print;
		unsigned int seqplayanimcounter = 0;
		for (j=0;j<seqanimamount;j++) {
			for (k=0;k<bd->sequence_code_amount[seqanimbase+j];k++) {
				if (bd->sequence_code[seqanimbase+j][k].code==0x05) {
					seqplayanimcounter++;
					if (bd->animation_id[bd->sequence_base_anim[seqanimbase+j]+bd->sequence_code[seqanimbase+j][k].arg[0]]==DummyBattleAnim[i].id) {
						desc = wxString(en->spell[seqanimbase+j].name.str_nice)+" "+to_string(seqplayanimcounter);
						goto block_print;
					}
				}
			}
		}
		block_print:
		fout << modelid << "$" << (int)DummyBattleAnim[i].id << "$" << DummyBattleAnim[i].name << "$" << desc << endl;
	}
	fout.close();
}
