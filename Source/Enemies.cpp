#include "Enemies.h"

#include <algorithm>
#include "main.h"
#include "Gui_LoadingDialog.h"
#include "Database_Steam.h"
#include "Database_Resource.h"
#include "Database_SpellAnimation.h"
#include "Database_CSV.h"

#define HWS_BATTLE_SCENE_MOD_ID		0xFFF0

int EnemySpellDataStruct::SetName(wstring newvalue, SteamLanguage lang) {
	if (parent->parent->text[parent->id]->SetText(id+parent->stat_amount,newvalue,lang))
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

EnemyStatDataStruct* EnemySpellDataStruct::GetAssociatedStat() {
	BattleDataStruct* bd = parent->parent->battle_data[parent->id];
	if (id>bd->sequence_amount || bd->sequence_stat_id[id]>parent->stat_amount)
		return NULL;
	return &parent->stat[bd->sequence_stat_id[id]];
}

int EnemyStatDataStruct::SetName(wstring newvalue, SteamLanguage lang) {
	if (parent->parent->text[parent->id]->SetText(id,newvalue,lang))
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
	stat.resize(stat_amount+1,*copystat);
	stat[stat_amount].default_attack = 0;
	stat[stat_amount].text_amount = 0;
	stat[stat_amount].sequence_anim_base = bd.animation_amount;
	stat[stat_amount].id = stat_amount;
	stat[stat_amount].parent = this;
	stat_amount++;
	bd.animation_amount += copystat->sequence_anim_amount;
	bd.animation_id.insert(bd.animation_id.end(),copybd.animation_id.begin()+copystat->sequence_anim_base,copybd.animation_id.begin()+copystat->sequence_anim_base+copystat->sequence_anim_amount);
	parent->UpdateBattleName(id);
	return 0;
}

void EnemyDataStruct::RemoveStat(uint16_t statid) {
	BattleDataStruct& bd = *parent->battle_data[id];
	TextDataStruct& td = *parent->text[id];
	unsigned int i;
	bd.SetSize(bd.size-2*stat[statid].sequence_anim_amount);
	bd.animation_amount -= stat[statid].sequence_anim_amount;
	bd.animation_id.erase(bd.animation_id.begin()+stat[statid].sequence_anim_base,bd.animation_id.begin()+stat[statid].sequence_anim_amount);
	for (i=0;i<bd.sequence_amount;i++)
		if (bd.sequence_stat_id[i]==statid) {
			bd.sequence_base_anim[i] = 0;
			bd.sequence_stat_id[i] = 0;
		} else {
			if (bd.sequence_base_anim[i]>stat[statid].sequence_anim_base)
				bd.sequence_base_anim[i] -= stat[statid].sequence_anim_amount;
			if (bd.sequence_stat_id[i]>statid)
				bd.sequence_stat_id[i]--;
		}
	for (i=0;i<stat_amount;i++)
		if (stat[i].sequence_anim_base>stat[statid].sequence_anim_base)
			stat[i].sequence_anim_base -= stat[statid].sequence_anim_amount;
	SetSize(size-0x74);
	stat.erase(stat.begin()+statid);
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
	spell.resize(spell_amount+1,*copyspell);
	spell[spell_amount].id = spell_amount;
	spell[spell_amount].parent = this;
	spell_amount++;
	unsigned int seqamount = bs.sequence_amount;
	unsigned int newseqindex = bs.sequence_amount-1;
	bool lastisdummy = false;
	if (bs.sequence_amount>1)
		lastisdummy = bs.sequence_offset[bs.sequence_amount-1]==bs.sequence_offset[0];
	if (!lastisdummy) {
		seqamount += 2;
		newseqindex++;
		bs.sequence_offset.resize(seqamount);
		bs.sequence_base_anim.resize(seqamount);
		bs.sequence_stat_id.resize(seqamount);
		bs.sequence_code_amount.resize(seqamount);
		bs.sequence_code.resize(seqamount);
		bs.sequence_offset[seqamount-1] = bs.sequence_offset[0];
	}
	bs.sequence_offset[newseqindex] = 0;
	bs.sequence_base_anim[newseqindex] = 0;
	bs.sequence_stat_id[newseqindex] = 0;
	bs.sequence_code_amount[newseqindex] = copybs.sequence_code_amount[copyspell->id];
	bs.sequence_code[newseqindex].resize(bs.sequence_code_amount[newseqindex]);
	for (i=0;i<bs.sequence_code_amount[newseqindex];i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(copybs.sequence_code[copyspell->id][i].code);
		bs.sequence_code[newseqindex][i].parent = &bs;
		bs.sequence_code[newseqindex][i].code = seq.id;
		bs.sequence_code[newseqindex][i].arg = new uint32_t[seq.arg_amount];
		memcpy(bs.sequence_code[newseqindex][i].arg,copybs.sequence_code[copyspell->id][i].arg,seq.arg_amount*sizeof(uint32_t));
	}
	bs.sequence_amount = seqamount;
	bs.UpdateOffset();
	return 0;
}

void EnemyDataStruct::RemoveSpell(uint16_t spellid) {
	BattleDataStruct* bs = parent->battle_data[id];
	TextDataStruct* td = parent->text[id];
	spell_amount--;
	spell.erase(spell.begin()+spellid);
	UpdateOffset();
	bool lastisdummy = false;
	if (bs->sequence_amount>1)
		lastisdummy = bs->sequence_offset[bs->sequence_amount-1]==bs->sequence_offset[0];
	bs->sequence_offset.erase(bs->sequence_offset.begin()+spellid);
	bs->sequence_base_anim.erase(bs->sequence_base_anim.begin()+spellid);
	bs->sequence_stat_id.erase(bs->sequence_stat_id.begin()+spellid);
	bs->sequence_code_amount.erase(bs->sequence_code_amount.begin()+spellid);
	bs->sequence_code.erase(bs->sequence_code.begin()+spellid);
	if (lastisdummy) {
		bs->sequence_amount -= 2;
		bs->sequence_offset.resize(bs->sequence_amount);
		bs->sequence_base_anim.resize(bs->sequence_amount);
		bs->sequence_stat_id.resize(bs->sequence_amount);
		bs->sequence_code_amount.resize(bs->sequence_amount);
		bs->sequence_code.resize(bs->sequence_amount);
	} else if (bs->sequence_amount>1) {
		bs->sequence_offset.push_back(bs->sequence_offset[0]);
		bs->sequence_base_anim.push_back(0);
		bs->sequence_stat_id.push_back(0);
		bs->sequence_code_amount.push_back(0);
		bs->sequence_code.push_back(vector<EnemySequenceCodeLine>());
	}
	bs->UpdateOffset();
	td->RemoveText(stat_amount+spellid);
}

int EnemyDataStruct::AddGroup() {
	if (GetExtraSize()<0x38)
		return 1;
	unsigned int i;
	SetSize(size+0x38);
	group.resize(group_amount+1);
	group[group_amount].frequence = 0;
	group[group_amount].enemy_amount = 1;
	group[group_amount].camera_engage = 0;
	group[group_amount].ap = 0;
	for (i=0;i<4;i++) {
		group[group_amount].enemy_id[i] = 0;
		group[group_amount].targetable[i] = 1;
		group[group_amount].enemy_posx[i] = 1500-1000*i;
		group[group_amount].enemy_posz[i] = 0;
		group[group_amount].enemy_posy[i] = 600;
		group[group_amount].enemy_angle[i] = 0;
	}
	group[group_amount].id = group_amount;
	group[group_amount].parent = this;
	group_amount++;
	return 0;
}

void EnemyDataStruct::RemoveGroup(uint16_t groupid) {
	SetSize(size-0x38);
	group.erase(group.begin()+groupid);
	group_amount--;
}

int EnemyDataStruct::AddAnimation(uint16_t statid, uint16_t animid) {
	if (GetExtraSize()<2)
		return 1;
	BattleDataStruct& bs = *parent->battle_data[id];
	unsigned int i, animindex = stat[statid].sequence_anim_base+stat[statid].sequence_anim_amount;
	SetSize(size+2);
	bs.animation_amount++;
	bs.animation_id.insert(bs.animation_id.begin()+animindex,animid);
	for (i=0;i<bs.sequence_amount;i++)
		if (bs.sequence_base_anim[i]>=animindex && bs.sequence_stat_id[i]!=statid)
			bs.sequence_base_anim[i]++;
	for (i=0;i<stat_amount;i++)
		if (stat[i].sequence_anim_base>=animindex && i!=statid)
			stat[i].sequence_anim_base++;
	stat[statid].sequence_anim_amount++;
	return 0;
}

void EnemyDataStruct::RemoveAnimation(uint16_t animindex) {
	BattleDataStruct& bs = *parent->battle_data[id];
	unsigned int i;
	SetSize(size-2);
	bs.animation_amount--;
	bs.animation_id.erase(bs.animation_id.begin()+animindex);
	for (i=0;i<stat_amount;i++)
		if (animindex>=stat[i].sequence_anim_base && animindex<stat[i].sequence_anim_base+stat[i].sequence_anim_amount) {
			stat[i].sequence_anim_amount--;
		} else if (stat[i].sequence_anim_base>animindex) {
			stat[i].sequence_anim_base--;
		}
	for (i=0;i<bs.sequence_amount;i++)
		if (bs.sequence_base_anim[i]>animindex)
			bs.sequence_base_anim[i]--;
}

Spell_Panel EnemySpellDataStruct::GetPanel() {
	return (target_type >> 5) & 0x7;
}

void EnemySpellDataStruct::SetPanel(Spell_Panel newvalue) {
	target_type = (target_type & 0x1F) | (newvalue << 5);
}

Spell_Target_Priority EnemySpellDataStruct::GetTargetPriority() {
	return (target_type >> 4) & 0x1;
}

void EnemySpellDataStruct::SetTargetPriority(Spell_Target_Priority newvalue) {
	target_type = (target_type & 0xEF) | (newvalue << 4);
}

Spell_Target_Type EnemySpellDataStruct::GetTargetType() {
	uint8_t val = target_type & 0xF;
	if (val == 0x0 || val == 0x3 || val == 0x6 || val == 0x9) // In Steam, 0x6 and 0x9 are SPELL_TARGET_TYPE_EVERYONE
		return SPELL_TARGET_TYPE_ANY;
	if (val == 0x1 || val == 0x4 || val == 0x7 || val == 0xA)
		return SPELL_TARGET_TYPE_ALLY;
	if (val == 0x2 || val == 0x5 || val == 0x8 || val == 0xB)
		return SPELL_TARGET_TYPE_ENEMY;
	if (val == 0xC)
		return SPELL_TARGET_TYPE_EVERYONE;
	if (val == 0xD)
		return SPELL_TARGET_TYPE_SELF;
	return SPELL_TARGET_TYPE_IRRELEVANT;
}

void EnemySpellDataStruct::SetTargetType(Spell_Target_Type newvalue) {
	uint8_t ta = target_type & 0xF;
	ta = ta >= 0xC ? 0 : ta / 3;
	if (newvalue == SPELL_TARGET_TYPE_EVERYONE)
		target_type = (target_type & 0xF0) + 0xC;
	else if (newvalue == SPELL_TARGET_TYPE_IRRELEVANT)
		target_type = (target_type & 0xF0) + 0xE;
	else if (newvalue == SPELL_TARGET_TYPE_SELF)
		target_type = (target_type & 0xE0) + 0x10 + 0xD;
	else if (newvalue == SPELL_TARGET_TYPE_ANY)
		target_type = (target_type & 0xF0) + 3 * ta;
	else if (newvalue == SPELL_TARGET_TYPE_ALLY)
		target_type = (target_type & 0xE0) + 0x10 + 3 * ta + 1;
	else if (newvalue == SPELL_TARGET_TYPE_ENEMY)
		target_type = (target_type & 0xE0) + 3 * ta + 2;
}

Spell_Target_Amount EnemySpellDataStruct::GetTargetAmount() {
	uint8_t val = target_type & 0xF;
	if (val < 0x3 || val == 0xD)
		return SPELL_TARGET_AMOUNT_ONE;
	if (val < 0x6)
		return SPELL_TARGET_AMOUNT_VARIABLE;
	if (val < 0x9 || val >= 0xC)
		return SPELL_TARGET_AMOUNT_GROUP;
	return SPELL_TARGET_AMOUNT_RANDOM;
}

void EnemySpellDataStruct::SetTargetAmount(Spell_Target_Amount newvalue) {
	if (GetTargetType() == SPELL_TARGET_TYPE_SELF && newvalue != SPELL_TARGET_AMOUNT_ONE)
		SetTargetType(SPELL_TARGET_TYPE_ALLY);
	if (GetTargetType() == SPELL_TARGET_TYPE_EVERYONE && newvalue != SPELL_TARGET_AMOUNT_GROUP)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	if (GetTargetType() == SPELL_TARGET_TYPE_IRRELEVANT)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	uint8_t tt = target_type & 0xF;
	if (newvalue == SPELL_TARGET_AMOUNT_ONE)
		target_type = (target_type & 0xF0) + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_VARIABLE)
		target_type = (target_type & 0xF0) + 0x3 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_GROUP)
		target_type = (target_type & 0xF0) + 0x6 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_RANDOM)
		target_type = (target_type & 0xF0) + 0x9 + tt % 3;
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
		group.resize(group_amount); \
		stat.resize(stat_amount); \
		spell.resize(spell_amount); \
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
		IO ## Char(f,stat[i].zerostat); \
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
		IO ## Short(f,stat[i].shadow_size_x); \
		IO ## Short(f,stat[i].shadow_size_y); \
		IO ## Char(f,stat[i].shadow_bone1); \
		IO ## Char(f,stat[i].card_drop); \
		IO ## Short(f,(uint16_t&)stat[i].shadow_offset_x); \
		IO ## Short(f,(uint16_t&)stat[i].shadow_offset_y); \
		IO ## Char(f,stat[i].shadow_bone2); \
		IO ## Char(f,stat[i].zero1); \
		IO ## Short(f,stat[i].zero2); \
		IO ## Short(f,stat[i].zero3); \
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
		IO ## Char(f,spell[i].menu_flag); \
		IO ## Short(f,spell[i].model_alt); \
		IO ## Short(f,spell[i].name_offset); \
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

void EnemyDataSet::GetSpellSequenceModelRef(vector<EnemySequenceCodeLine>& sequence, int* code, int* arg) {
	unsigned int i, j;
	for (i=0; i<sequence.size(); i++) {
		EnemySequenceCode& c = GetEnemySequenceCode(sequence[i].code);
		for (j=0; (int)j<c.arg_amount; j++)
			if (c.arg_type[j]==EAAT_SPELL_ANIM) {
				*code = i;
				*arg = j;
				return;
			}
	}
	*code = -1;
	*arg = -1;
}

EnemyStatDataStruct* GetSimilarEnemyStatsResult[1024];
EnemySpellDataStruct* GetSimilarEnemySpellsResult[1024];
unsigned int GetSimilarEnemyBattlesId[1024];
EnemyStatDataStruct** EnemyDataSet::GetSimilarEnemyStats(EnemyStatDataStruct& stat, unsigned int* amountfound, unsigned int** battleid) {
	unsigned int i,j,nb = 0;
	wstring& statname = stat.name.str_nice;
	for (i=0;i<battle_amount;i++)
		for (j=0;j<battle[i]->stat_amount;j++)
			if (battle[i]->stat[j].name.str_nice==statname && battle[i]->stat[j].lvl==stat.lvl) {
				GetSimilarEnemyStatsResult[nb] = &(battle[i]->stat[j]);
				GetSimilarEnemyBattlesId[nb++] = i;
			}
	*amountfound = nb;
	*battleid = GetSimilarEnemyBattlesId;
	return GetSimilarEnemyStatsResult;
}

EnemySpellDataStruct** EnemyDataSet::GetSimilarEnemySpells(EnemySpellDataStruct& spell, unsigned int* amountfound, unsigned int** battleid) {
	int baseseqcode, basecodearg, seqcode, codearg;
	unsigned int i,j,nbstat, nb = 0;
	unsigned int* bid;
	EnemyStatDataStruct* stat = spell.GetAssociatedStat();
	if (stat==NULL) {
		*amountfound = 0;
		return NULL;
	}
	EnemyStatDataStruct** similarstat = GetSimilarEnemyStats(*stat,&nbstat,&bid);
	wstring& spellname = spell.name.str_nice;
	GetSpellSequenceModelRef(battle_data[spell.parent->id]->sequence_code[spell.id], &baseseqcode, &basecodearg);
	for (i=0;i<nbstat;i++)
		for (j=0;j<battle[bid[i]]->spell_amount;j++)
			if (stat==similarstat[i] && &battle[bid[i]]->spell[j]==&spell) {
				GetSimilarEnemySpellsResult[nb] = &(battle[bid[i]]->spell[j]);
				GetSimilarEnemyBattlesId[nb++] = bid[i];
			} else if (battle_data[bid[i]]->sequence_stat_id[j]==similarstat[i]->id && battle[bid[i]]->spell[j].name.str_nice==spellname && battle[bid[i]]->spell[j].effect==spell.effect && battle[bid[i]]->spell[j].power==spell.power) {
				GetSpellSequenceModelRef(battle_data[bid[i]]->sequence_code[j], &seqcode, &codearg);
				if ((baseseqcode<0 && seqcode<0) || (baseseqcode>=0 && battle_data[bid[i]]->sequence_code[j][seqcode].arg[codearg]==battle_data[spell.parent->id]->sequence_code[spell.id][baseseqcode].arg[basecodearg])) {
					GetSimilarEnemySpellsResult[nb] = &(battle[bid[i]]->spell[j]);
					GetSimilarEnemyBattlesId[nb++] = bid[i];
				}
			}
	*amountfound = nb;
	*battleid = GetSimilarEnemyBattlesId;
	return GetSimilarEnemySpellsResult;
}

EnemySequenceCodeLine::~EnemySequenceCodeLine() {
//	delete[] arg;
}

#define MACRO_BATTLE_IOFUNCTION_SEQREAD(IO) \
	sequence_code[i].clear(); \
	j = 0; \
	do { \
		sequence_code[i].push_back(EnemySequenceCodeLine()); \
		sequence_code[i][j].parent = this; \
		IO ## Char(f,sequence_code[i][j].code); \
		EnemySequenceCode& seq = GetEnemySequenceCode(sequence_code[i][j].code); \
		sequence_code[i][j].arg = new uint32_t[seq.arg_amount]; \
		for (k=0;k<seq.arg_amount;k++) { \
			if (seq.arg_length[k]==1) { \
				IO ## Char(f,tmp8); \
				sequence_code[i][j].arg[k] = tmp8; \
			} else if (seq.arg_length[k]==2) { \
				IO ## Short(f,tmp16); \
				sequence_code[i][j].arg[k] = tmp16; \
			} else if (seq.arg_length[k]==4) { \
				IO ## Long(f,tmp32); \
				sequence_code[i][j].arg[k] = tmp32; \
			} \
		} \
	} while (sequence_code[i][j++].code!=0); \
	sequence_code_amount[i] = j;

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
		animation_id.resize(animation_amount); \
		sequence_offset.resize(sequence_amount); \
		sequence_base_anim.resize(sequence_amount); \
		sequence_stat_id.resize(sequence_amount); \
		sequence_code_amount.resize(sequence_amount); \
		sequence_code.resize(sequence_amount); \
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
			sequence_code[i].clear(); \
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

void BattleDataStruct::UpdateOffset() {
	unsigned int i,j,k;
	uint32_t size;
	animblock_offset = 0x4;
	if (sequence_amount%2) {
		sequence_amount++;
		sequence_offset.push_back(sequence_offset[0]);
		sequence_base_anim.push_back(0);
		sequence_stat_id.push_back(0);
		sequence_code_amount.push_back(0);
		sequence_code.push_back(vector<EnemySequenceCodeLine>());
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
		uint16_t text_lang_amount[STEAM_LANGUAGE_AMOUNT];
		SteamLanguage lang;
		uint32_t fsize;
		char* buffer;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			text[i] = new TextDataStruct[1];
			text[i]->Init(true,CHUNK_TYPE_TEXT,config.enmy_id[i],&dummyclus[i],CLUSTER_TYPE_ENEMY);
			text[i]->amount = 0;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.enmy_text_file[lang][i]));
				fsize = config.meta_res.GetFileSizeByIndex(config.enmy_text_file[lang][i]);
				buffer = new char[fsize];
				ffbin.read(buffer,fsize);
				text_lang_amount[lang] = FF9String::CountSteamTextAmount(buffer,fsize);
				text[i]->amount = max(text[i]->amount,text_lang_amount[lang]);
				delete[] buffer;
			}
			text[i]->text = new FF9String[text[i]->amount];
			text[i]->loaded = true;
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.enmy_text_file[lang][i]));
				for (j=0;j<text_lang_amount[lang];j++)
					SteamReadFF9String(ffbin,text[i]->text[j],lang);
			}
//			text[i]->size = config.meta_res.GetFileSizeByIndex(config.enmy_text_file[lang][i]);
			LoadingDialogUpdate(i, wxString::Format(wxT("%u / %u (1/3)"), i, battle_amount));
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
			LoadingDialogUpdate(i, wxString::Format(wxT("%u / %u (2/3)"), i, battle_amount));
		}
		ffbin.close();
		fname = config.steam_dir_assets + "p0data7.bin";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<battle_amount;i++) {
			script[i] = new ScriptDataStruct[1];
			script[i]->Init(false,CHUNK_TYPE_SCRIPT,config.enmy_id[i],&dummyclus[i]);
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.enmy_script_file[lang][i]));
				script[i]->Read(ffbin,lang);
			}
			script[i]->ChangeSteamLanguage(GetSteamLanguage());
			script[i]->size = config.meta_battle.GetFileSizeByIndex(config.enmy_script_file[GetSteamLanguage()][i]); // DEBUG: Verify that Steam doesn't care about this
			script[i]->LinkSimilarLanguageScripts();
			l = 0;
			for (k=0;k<battle[i]->stat_amount && l<text[i]->amount;k++)
				battle[i]->stat[k].name = text[i]->text[l++];
			for (k=0;k<battle[i]->spell_amount && l<text[i]->amount;k++)
				battle[i]->spell[k].name = text[i]->text[l++];
			UpdateBattleName(i);
			SetupEnemyInfo(i);
/*wfstream fout("aaaa.txt",ios::app|ios::out);
fout << battle_name[i] << L"->";
for (j=0;j<battle[i]->group_amount;j++) {
fout << L" (";
for (k=0;k<4;k++)
fout << (k<battle[i]->group[j].enemy_amount ? L"o:" : L"x:") << (int)battle[i]->group[j].enemy_angle[k] << L" ";
fout << L")";
}
fout << endl;
fout.close();*/
			LoadingDialogUpdate(i, wxString::Format(wxT("%u / %u (3/3)"), i, battle_amount));
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

void EnemyDataSet::GenerateCSharp(vector<string>& buffer) {
	if (modified_battle_scene_amount==0)
		return;
	unsigned int i, j;
	int battleindex, bscindex, bbgindex;
	stringstream bscenedb;
	bscenedb << "// Method: FF9BattleDB::.cctor\n\n";
	bscenedb << "\t// Inside \"FF9BattleDB.MapModel\" dictionary:\n";
	for (i = 0; i < modified_battle_scene_amount; i++) {
		battleindex = -1;
		bscindex = -1;
		bbgindex = -1;
		for (j = 0; j < battle_amount; j++)
			if (modified_battle_id[i]==battle_data[j]->object_id) {
				battleindex = j;
				break;
			}
		for (j = 0; j < G_N_ELEMENTS(SteamBattleScript); j++)
			if (modified_battle_id[i]==SteamBattleScript[j].battle_id) {
				bscindex = j;
				break;
			}
		if (battleindex < 0 || bscindex < 0) {
			bscenedb << "\t\t// Error: unexpected Battle ID " << (int)modified_battle_id[i] << "\n";
			continue;
		}
		for (j = 0; j < G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); j++)
			if (modified_scene_id[i]==HADES_STRING_BATTLE_SCENE_NAME[j].id) {
				bbgindex = j;
				break;
			}
		if (bbgindex < 0) {
			bscenedb << "\t\t// Error: unexpected Battle Scene ID " << (int)modified_scene_id[i] << "\n";
			continue;
		}
		bscenedb << "\t\t// Replace the entry of \"BSC_" << SteamBattleScript[bscindex].name.substr(11) << "\":\n";
		bscenedb << "\t\t{ \"BSC_" << SteamBattleScript[bscindex].name.substr(11) << "\", \"BBG_" << HADES_STRING_BATTLE_SCENE_NAME[bbgindex].steamid.ToStdString() << "\" }, // " << ConvertWStrToStr(battle_name[battleindex]) << " (" << (int)modified_battle_id[i] << ")\n";
	}
	buffer.push_back(bscenedb.str());
}

bool EnemyDataSet::GenerateCSV(string basefolder) {
	if (modified_battle_scene_amount==0)
		return true;
	int battleindex, bscindex, bbgindex;
	unsigned int i, j;
	string fname = basefolder + HADES_STRING_DICTIONARY_PATCH_FILE;
	string previouspatch = "";
	fstream inpatchfile(fname.c_str(), ios::in);
	if (inpatchfile.is_open()) {
		stringstream inpatchstream;
		inpatchstream << inpatchfile.rdbuf();
		string wholepreviouspatch = inpatchstream.str();
		size_t prevpos = 0, battlemapentrypos = wholepreviouspatch.find("BattleMapModel ");
		while (battlemapentrypos != string::npos) {
			if (battlemapentrypos > 0 && wholepreviouspatch[battlemapentrypos-1] != '\n') {
				battlemapentrypos = wholepreviouspatch.find("BattleMapModel ", battlemapentrypos+1);
				continue;
			}
			if (battlemapentrypos > 0)
				previouspatch += wholepreviouspatch.substr(prevpos, battlemapentrypos-prevpos);
			prevpos = wholepreviouspatch.find("\n", battlemapentrypos+1);
			if (prevpos == string::npos)
				break;
			prevpos++;
			battlemapentrypos = wholepreviouspatch.find("BattleMapModel ", prevpos);
		}
		previouspatch += wholepreviouspatch.substr(prevpos);
		if (previouspatch.length() > 0 && previouspatch.back() != '\n')
			previouspatch += "\n";
	}
	fstream patchfile(fname.c_str(), ios::out);
	if (!patchfile.is_open()) return false;
	patchfile << previouspatch;
	for (i = 0; i < modified_battle_scene_amount; i++) {
		battleindex = -1;
		bscindex = -1;
		bbgindex = -1;
		for (j = 0; j < battle_amount; j++)
			if (modified_battle_id[i]==battle_data[j]->object_id) {
				battleindex = j;
				break;
			}
		for (j = 0; j < G_N_ELEMENTS(SteamBattleScript); j++)
			if (modified_battle_id[i]==SteamBattleScript[j].battle_id) {
				bscindex = j;
				break;
			}
		if (battleindex < 0 || bscindex < 0) {
			patchfile << "// [Hades Workshop] Error: unexpected Battle ID " << (int)modified_battle_id[i] << "\n";
			continue;
		}
		for (j = 0; j < G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); j++)
			if (modified_scene_id[i]==HADES_STRING_BATTLE_SCENE_NAME[j].id) {
				bbgindex = j;
				break;
			}
		if (bbgindex < 0) {
			patchfile << "// [Hades Workshop] Error: unexpected Battle Scene ID " << (int)modified_scene_id[i] << "\n";
			continue;
		}
		patchfile << "BattleMapModel BSC_" << SteamBattleScript[bscindex].name.substr(11) << " BBG_" << HADES_STRING_BATTLE_SCENE_NAME[bbgindex].steamid.ToStdString() << "\n";
	}
	patchfile.close();
	return true;
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
	SteamLanguage lang,sublang;
	bool shouldread;
	uint8_t langcount;
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
						} else if (chunktype==CHUNK_STEAM_TEXT_MULTILANG) {
							if (usetext && loadmain)
								text[j]->ReadHWS(ffhws,true);
						} else if (chunktype==CHUNK_STEAM_SCRIPT_MULTILANG) {
							if (loadmain) {
								uint16_t langcorrcount;
								uint32_t langcorrpos;
								vector<uint16_t> corrlinkbase,corrlink;
								HWSReadChar(ffhws,lang);
								while (lang!=STEAM_LANGUAGE_NONE) {
									uint32_t langdatasize;
									shouldread = false;
									HWSReadChar(ffhws,langcount);
									langcorrpos = ffhws.tellg();
									for (k=0;k<langcount;k++) {
										HWSReadChar(ffhws,sublang);
										HWSReadLong(ffhws,langdatasize);
										if (hades::STEAM_SINGLE_LANGUAGE_MODE && sublang==GetSteamLanguage()) {
											shouldread = true;
											HWSReadShort(ffhws,langcorrcount);
											corrlinkbase.resize(langcorrcount);
											corrlink.resize(langcorrcount);
											for (l=0;l<langcorrcount;l++) {
												HWSReadShort(ffhws,corrlinkbase[l]);
												HWSReadShort(ffhws,corrlink[l]);
											}
										} else {
											ffhws.seekg((long long)ffhws.tellg()+langdatasize);
										}
									}
									HWSReadLong(ffhws,langdatasize);
									if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage()) {
										if (shouldread) {
											script[j]->ReadHWS(ffhws,false);
											script[j]->ApplyDialogLink(corrlink,corrlinkbase);
										} else {
											ffhws.seekg(langdatasize,ios::cur);
										}
									} else {
										script[j]->ReadHWS(ffhws,false,lang);
										if (script[j]->multi_lang_script!=NULL) {
											uint32_t endlangpos = ffhws.tellg();
											ffhws.seekg(langcorrpos);
											for (k=0;k<langcount;k++) {
												HWSReadChar(ffhws,sublang);
												HWSReadLong(ffhws,langdatasize);
												HWSReadShort(ffhws,langcorrcount);
												corrlinkbase.resize(langcorrcount);
												corrlink.resize(langcorrcount);
												for (l=0;l<langcorrcount;l++) {
													HWSReadShort(ffhws,corrlinkbase[l]);
													HWSReadShort(ffhws,corrlink[l]);
												}
												script[j]->LinkLanguageScripts(sublang,lang,corrlink,corrlinkbase);
											}
											ffhws.seekg(endlangpos);
										}
									}
									HWSReadChar(ffhws,lang);
								}
							}
						} else if (chunktype==CHUNK_SPECIAL_TYPE_LOCAL_MULTILANG) {
							if (loadlocal) {
								HWSReadChar(ffhws,lang);
								while (lang!=STEAM_LANGUAGE_NONE) {
									uint32_t langdatasize;
									shouldread = false;
									HWSReadChar(ffhws,langcount);
									for (k=0;k<langcount;k++) {
										HWSReadChar(ffhws,sublang);
										if (sublang==GetSteamLanguage())
											shouldread = true;
									}
									HWSReadLong(ffhws,langdatasize);
									if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage()) {
										if (shouldread)
											script[j]->ReadLocalHWS(ffhws);
										else
											ffhws.seekg(langdatasize,ios::cur);
									} else {
										script[j]->ReadLocalHWS(ffhws,lang);
									}
									HWSReadChar(ffhws,lang);
								}
							}
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
	uint32_t chunkpos, chunksize, nboffset = ffhws.tellg();
	uint32_t aftlinkpos, linkpos;
	SteamLanguage lang, sublang;
	ClusterData* clus = NULL;
	uint8_t nbscriptlink;
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
			if (GetGameType()==GAME_TYPE_PSX) {
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
				if (preload[i]->modified && savemain) {
					HWSWriteChar(ffhws,CHUNK_TYPE_IMAGE_MAP);
					HWSWriteLong(ffhws,preload[i]->size);
					chunkpos = ffhws.tellg();
					preload[i]->WriteHWS(ffhws);
					ffhws.seekg(chunkpos+preload[i]->size);
				}
			} else {
				if (text[i]->modified && savemain) {
					HWSWriteChar(ffhws,CHUNK_STEAM_TEXT_MULTILANG);
					HWSWriteLong(ffhws,0);
					chunkpos = ffhws.tellg();
					text[i]->WriteHWS(ffhws,true);
					chunksize = (long long)ffhws.tellg()-chunkpos;
					ffhws.seekg(chunkpos-4);
					HWSWriteLong(ffhws,chunksize);
					ffhws.seekg(chunkpos+chunksize);
				}
				for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
					if (savemain && hades::STEAM_LANGUAGE_SAVE_LIST[lang] && script[i]->IsDataModified(lang))
						break;
				if (lang<STEAM_LANGUAGE_AMOUNT) {
					HWSWriteChar(ffhws,CHUNK_STEAM_SCRIPT_MULTILANG);
					HWSWriteLong(ffhws,0);
					chunkpos = ffhws.tellg();
					for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
						if (hades::STEAM_LANGUAGE_SAVE_LIST[lang] && script[i]->IsDataModified(lang)) {
							if (script[i]->multi_lang_script!=NULL && script[i]->multi_lang_script->base_script_lang[lang]!=lang && hades::STEAM_LANGUAGE_SAVE_LIST[script[i]->multi_lang_script->base_script_lang[lang]])
								continue;
							HWSWriteChar(ffhws,lang);
							HWSWriteChar(ffhws,0);
							if (script[i]->multi_lang_script!=NULL && script[i]->multi_lang_script->base_script_lang[lang]==lang) {
								linkpos = ffhws.tellg();
								nbscriptlink = 0;
								for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
									if (lang!=sublang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang] && script[i]->multi_lang_script->base_script_lang[sublang]==lang) {
										uint16_t textcorresp = script[i]->multi_lang_script->base_script_text_id[sublang].size();
										HWSWriteChar(ffhws,sublang);
										HWSWriteLong(ffhws,2+4*textcorresp);
										HWSWriteShort(ffhws,textcorresp);
										for (j=0;j<textcorresp;j++) {
											HWSWriteShort(ffhws,script[i]->multi_lang_script->base_script_text_id[sublang][j]);
											HWSWriteShort(ffhws,script[i]->multi_lang_script->lang_script_text_id[sublang][j]);
										}
										nbscriptlink++;
									}
								aftlinkpos = ffhws.tellg();
								ffhws.seekg(linkpos-1);
								HWSWriteChar(ffhws,nbscriptlink);
								ffhws.seekg(aftlinkpos);
							}
							HWSWriteLong(ffhws,script[i]->GetDataSize(lang));
							script[i]->WriteHWS(ffhws,lang);
						}
					HWSWriteChar(ffhws,STEAM_LANGUAGE_NONE);
					chunksize = (long long)ffhws.tellg()-chunkpos;
					ffhws.seekg(chunkpos-4);
					HWSWriteLong(ffhws,chunksize);
					ffhws.seekg(chunkpos+chunksize);
				}
				for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
					if (savelocal && hades::STEAM_LANGUAGE_SAVE_LIST[lang])
						break;
				if (lang<STEAM_LANGUAGE_AMOUNT) {
					uint32_t langdatasize, langdatapos;
					HWSWriteChar(ffhws,CHUNK_SPECIAL_TYPE_LOCAL_MULTILANG);
					HWSWriteLong(ffhws,0);
					chunkpos = ffhws.tellg();
					for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
						if (hades::STEAM_LANGUAGE_SAVE_LIST[lang]) {
							if (script[i]->multi_lang_script!=NULL && script[i]->multi_lang_script->base_script_lang[lang]!=lang && hades::STEAM_LANGUAGE_SAVE_LIST[script[i]->multi_lang_script->base_script_lang[lang]])
								continue;
							HWSWriteChar(ffhws,lang);
							HWSWriteChar(ffhws,0);
							if (script[i]->multi_lang_script!=NULL && script[i]->multi_lang_script->base_script_lang[lang]==lang) {
								nbscriptlink = 0;
								for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
									if (lang!=sublang && hades::STEAM_LANGUAGE_SAVE_LIST[sublang] && script[i]->multi_lang_script->base_script_lang[sublang]==lang) {
										HWSWriteChar(ffhws,sublang);
										nbscriptlink++;
									}
								ffhws.seekg((long long)ffhws.tellg()-nbscriptlink-1);
								HWSWriteChar(ffhws,nbscriptlink);
								ffhws.seekg((long long)ffhws.tellg()+nbscriptlink);
							}
							HWSWriteLong(ffhws,0);
							langdatapos = ffhws.tellg();
							script[i]->WriteLocalHWS(ffhws,lang);
							langdatasize = (long long)ffhws.tellg()-langdatapos;
							ffhws.seekg(langdatapos-4);
							HWSWriteLong(ffhws,langdatasize);
							ffhws.seekg(langdatapos+langdatasize);
						}
					HWSWriteChar(ffhws,STEAM_LANGUAGE_NONE);
					chunksize = (long long)ffhws.tellg()-chunkpos;
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
	int animgap = (int)newmodelinfo.sequence_anim_amount-es.sequence_anim_amount;
	bd.animation_amount += animgap;
	bd.animation_id.erase(bd.animation_id.begin()+es.sequence_anim_base,bd.animation_id.begin()+es.sequence_anim_base+es.sequence_anim_amount);
	for (i=0;i<newmodelinfo.sequence_anim_amount;i++)
		bd.animation_id.insert(bd.animation_id.begin()+es.sequence_anim_base+i,newmodelinfo.sequence_anim[i]);
	es.sequence_anim_amount = newmodelinfo.sequence_anim_amount;
	for (i=0;i<bd.sequence_amount;i++)
		if (bd.sequence_base_anim[i]>es.sequence_anim_base)
			bd.sequence_base_anim[i] += animgap;
	for (i=0;i<battle[battleid]->stat_amount;i++)
		if (battle[battleid]->stat[i].sequence_anim_base>es.sequence_anim_base)
			battle[battleid]->stat[i].sequence_anim_base += animgap;
	return 0;
}

void EnemyDataSet::SetupEnemyInfo(uint16_t battleid) { // DEBUG: Assume at least one sequence (spell) is configured for each stat, in the order of the stats
	EnemyDataStruct& ed = *battle[battleid];
	BattleDataStruct& bd = *battle_data[battleid];
	unsigned int i, j = 0, firstanim = 0,lastisdummy = 0;
	if (bd.sequence_amount>1 && bd.sequence_offset[bd.sequence_amount-1]==bd.sequence_offset[0]) {
		lastisdummy = 1;
		bd.sequence_stat_id[bd.sequence_amount-1] = 0;
	}
	for (i=0;i<ed.stat_amount;i++) {
		ed.stat[i].sequence_anim_base = firstanim;
		while (j+lastisdummy<bd.sequence_amount && bd.sequence_base_anim[j]==firstanim)
			bd.sequence_stat_id[j++] = i;
		if (j+lastisdummy<bd.sequence_amount) {
			ed.stat[i].sequence_anim_amount = bd.sequence_base_anim[j]-firstanim;
			firstanim = bd.sequence_base_anim[j];
		} else {
			ed.stat[i].sequence_anim_amount = bd.animation_amount-firstanim;
			firstanim = bd.animation_amount;
		}
	}
	while (j+lastisdummy<bd.sequence_amount && bd.sequence_base_anim[j]==firstanim)
		bd.sequence_stat_id[j++] = ed.stat_amount-1;
	while (j+lastisdummy<bd.sequence_amount)
		for (i=0;i<ed.stat_amount;i++)
			if (ed.stat[i].sequence_anim_base==bd.sequence_base_anim[j]) {
				bd.sequence_stat_id[j] = i;
				break;
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
	for (i=1;i<G_N_ELEMENTS(BATTLE_MODEL_LINKS);i++)
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
	if (model!=0xFFFF)
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
	es.shadow_size_x = shadow_size_x;
	es.shadow_size_y = shadow_size_y;
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
