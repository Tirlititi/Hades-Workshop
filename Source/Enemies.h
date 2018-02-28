#ifndef _ENEMIES_H
#define _ENEMIES_H

struct EnemySpellDataStruct;
struct EnemyStatDataStruct;
struct EnemyGroupDataStruct;
struct EnemyDataStruct;

struct EnemySequenceCodeLine;
struct BattleDataStruct;

struct BattleModelLinks;

struct EnemyDataSet;

#include <inttypes.h>
#include <fstream>
#include <vector>
#include "Configuration.h"
#include "Database_Script.h"
using namespace std;

#define BATTLE_FLAG_SCRIPTED_END	0x0001
#define BATTLE_FLAG_BACK_ATTACK		0x0002
#define BATTLE_FLAG_NO_FANFARE		0x0010
#define BATTLE_FLAG_NO_FLEE			0x0020
#define BATTLE_FLAG_OUT_OF_REACH	0x0040
#define BATTLE_FLAG_NO_MAGIC		0x0080
#define BATTLE_FLAG_IPSEN_CURSE		0x0100
#define BATTLE_FLAG_NO_REWARD		0x0800

#define ENEMY_CLASSIFICATION_HUMAN	0x01
#define ENEMY_CLASSIFICATION_BEAST	0x02
#define ENEMY_CLASSIFICATION_DEMON	0x04
#define ENEMY_CLASSIFICATION_DRAGON	0x08
#define ENEMY_CLASSIFICATION_UNDEAD	0x10
#define ENEMY_CLASSIFICATION_STONE	0x20
#define ENEMY_CLASSIFICATION_BUG	0x40
#define ENEMY_CLASSIFICATION_FLYING	0x80

#include "File_Manipulation.h"
#include "Configuration.h"

struct EnemySpellDataStruct {
public:
	FF9String name; // readonly
	uint8_t effect;
	uint8_t power;
	uint8_t element;
	uint8_t accuracy;
	uint8_t flag;
	uint8_t status;
	uint8_t mp;
	uint16_t model;
	uint8_t target_type;
	uint8_t target_flag;
	uint8_t unknown3;
	uint32_t unknown4;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetName(FF9String& newvalue);

	EnemyStatDataStruct* GetAssociatedStat();
	
	uint8_t id;
	EnemyDataStruct* parent;
};

struct EnemyStatDataStruct {
public:
	FF9String name; // readonly
	uint8_t lvl;
	uint8_t classification;
	uint16_t hp;
	uint16_t mp;
	uint16_t gils;
	uint16_t exp;
	uint8_t item_drop[4];
	uint8_t item_steal[4];
	uint8_t card_drop;
	uint16_t attack; // Unused
	uint8_t accuracy; // Unused
	uint8_t cur_capa;
	uint8_t max_capa;
	uint8_t trans;
	uint8_t speed;
	uint8_t strength;
	uint8_t magic;
	uint8_t spirit;
	uint8_t defence;
	uint8_t evade;
	uint8_t magic_defence;
	uint8_t magic_evade;
	uint8_t element_immune;
	uint8_t element_absorb;
	uint8_t element_half;
	uint8_t element_weak;
	uint32_t status_immune;
	uint32_t status_auto;
	uint32_t status_initial;
	uint8_t default_attack;
	uint8_t blue_magic;
	uint16_t death_flag; // 1 : die_atk ; 2 : die_dmg
	uint8_t text_amount;
	uint16_t radius;
	uint16_t model;
	uint16_t anim_idle;
	uint16_t anim_idle_alt;
	uint16_t anim_hit;
	uint16_t anim_hit_alt;
	uint16_t anim_death;
	uint16_t anim_death_alt;
	uint16_t mesh;
	uint16_t mesh_vanish;
	uint8_t bone_camera1;
	uint8_t bone_camera2;
	uint8_t bone_camera3;
	uint8_t bone_target;
	uint8_t selection_bone[6];
	int8_t selection_offsetz[6];
	int8_t selection_offsety[6];
	uint16_t shadow_size_x;
	uint16_t shadow_size_y;
	uint8_t shadow_bone1;
	uint8_t shadow_bone2;
	int16_t shadow_offset_x; // Unused
	int16_t shadow_offset_y; // Unused
	uint16_t sound_engage;
	uint16_t sound_death;
	uint8_t zero1;
	uint8_t zero2;
	uint32_t zero3;
	
	uint8_t sequence_anim_base;
	uint8_t sequence_anim_amount;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetName(FF9String& newvalue);
	
	uint8_t id;
	EnemyDataStruct* parent;
};

struct EnemyGroupDataStruct {
public:
	uint8_t frequence;
	uint8_t enemy_amount;
	uint16_t camera_engage;
	uint16_t ap;
	uint8_t enemy_id[4];
	uint8_t targetable[4];
	int16_t enemy_posx[4];
	int16_t enemy_posz[4];
	int16_t enemy_posy[4];
	int16_t enemy_angle[4]; // 360° = 0x1000
	
	uint8_t id;
	EnemyDataStruct* parent;
};

struct EnemyDataStruct : public ChunkChild {
public:
	uint16_t id;
	uint8_t unknown;
	uint8_t group_amount;
	uint8_t stat_amount;
	uint8_t spell_amount;
	vector<EnemyGroupDataStruct> group;
	vector<EnemyStatDataStruct> stat;
	vector<EnemySpellDataStruct> spell;
	uint16_t flag;
	
	// Battle Scene ID are from the PSX version ; Use SteamBattleScenePSXId[] for conversion
	uint16_t scene_id;
	
	// always append one
	int AddStat(EnemyStatDataStruct* copystat);
	int AddSpell(EnemySpellDataStruct* copyspell);
	int AddGroup();
	int AddAnimation(uint16_t statid, uint16_t animid);
	void RemoveStat(uint16_t statid);
	void RemoveSpell(uint16_t spellid);
	void RemoveGroup(uint16_t groupid);
	void RemoveAnimation(uint16_t animindex);
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void UpdateOffset();
	
	EnemyDataSet* parent;
};

struct EnemySequenceCodeLine {
	BattleDataStruct* parent;
	uint8_t code;
	uint32_t* arg;
	
	~EnemySequenceCodeLine();
};

struct BattleDataStruct : public ChunkChild {
public:
	uint16_t id;
	uint16_t animblock_offset;
	uint16_t animation_amount;
	vector<uint16_t> animation_id;
	uint16_t sequence_amount;
	vector<uint16_t> sequence_offset; // If sequence_amount should be odd, a dummy sequence is added with same offset as the 1st one
	vector<uint8_t> sequence_base_anim;
	vector<unsigned int> sequence_code_amount;
	vector< vector<EnemySequenceCodeLine> > sequence_code;
	uint16_t camerablock_offset;
	
	unsigned int camera_size; // ToDo : read cameras
	uint8_t* camera_raw;
	
	// unread for now
	uint16_t engage_camera_offset; // ?
	uint16_t* camera_offset; // array[9] ?
	uint16_t* camera_flag;
	uint16_t** camera_struct_offset;

	vector<uint8_t> sequence_stat_id;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void UpdateOffset();
	
	// Return false if there is no space
	// Arguments are defaulted
	// Unused ; see EnemyAnimSequenceEditDialog
//	bool AddAnimationSequenceCode(unsigned int sequence, unsigned int codeline, uint8_t newcode);
//	void DeleteAnimationSequenceCode(unsigned int sequence, unsigned int codeline);
//	bool ChangeAnimationSequenceCode(unsigned int sequence, unsigned int codeline, uint8_t newcode);
	
	EnemyDataSet* parent;
};

struct EnemyDataSet {
public:
	uint16_t battle_amount;
	uint16_t* cluster_id;
	wstring* battle_name;
	BattleDataStruct** battle_data;
	EnemyDataStruct** battle;
	TextDataStruct** text;
	ScriptDataStruct** script;
	ImageMapDataStruct** preload; // PSX only
	ImageMapDataStruct* shared_map; // PSX only
	ImageMapDataStruct*** image_map; // PSX only - pointer to the cluster's image map list
	unsigned int image_map_amount;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;
	
	// Return temporary array not to be destroyed (as for battleids)
	EnemyStatDataStruct** GetSimilarEnemyStats(EnemyStatDataStruct& stat, unsigned int* amountfound, unsigned int** battleid);
	EnemySpellDataStruct** GetSimilarEnemySpells(EnemySpellDataStruct& stat, unsigned int* amountfound, unsigned int** battleid);
	// Change "battle_name[battleid]" according to battle's monster names
	void UpdateBattleName(unsigned int battleid);
	// For PSX: Make the modifications inside field and world map image maps only... Mark the updated image maps as modified
	// For Steam: Only register the change ; nothing more needed
	int ChangeBattleScene(uint16_t battleid, uint16_t newsceneid, uint32_t newsceneoffset = 0, uint32_t newscenesize = 0);
	int ChangeBattleModel(uint16_t battleid, uint8_t enemyid, BattleModelLinks& newmodelinfo);
	
	void Load(fstream& ffbin, ClusterSet& clusset);
	// Use false if World Maps or fields are saved in the normal process : it prevents the image maps to be saved twice
	void Write(fstream& ffbin, ClusterSet& clusset, bool saveworldmap = true, bool savefieldmap = true);
	void WritePPF(fstream& ffbin, ClusterSet& clusset, bool saveworldmap = true, bool savefieldmap = true);
	// return value is int[4] to be deleted
	// {Number of oversized battles, Number of unknown data, Number of unused battles, Number of ignored Preload data}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	
private:
	unsigned int modified_battle_scene_amount;
	uint16_t modified_battle_id[0x1000];
	uint16_t modified_scene_id[0x1000];
	uint32_t modified_scene_offset[0x1000];
	uint32_t modified_scene_size[0x1000];
	
	void SetupEnemyInfo(uint16_t battleid);
};

struct BattleModelLinks {
	uint16_t model;
	uint16_t anim_idle;
	uint16_t anim_idle_alt;
	uint16_t anim_hit;
	uint16_t anim_hit_alt;
	uint16_t anim_death;
	uint16_t anim_death_alt;
	uint16_t radius;
	uint16_t mesh;
	uint16_t mesh_vanish;
	uint8_t bone_camera1;
	uint8_t bone_camera2;
	uint8_t bone_camera3;
	uint8_t bone_target;
	uint8_t selection_bone[6];
	int8_t selection_offsetz[6];
	int8_t selection_offsety[6];
	uint16_t shadow_size_x;
	uint16_t shadow_size_y;
	uint8_t shadow_bone1;
	uint8_t shadow_bone2;
	int16_t shadow_offset_x;
	int16_t shadow_offset_y;
	uint16_t sound_engage;
	uint16_t sound_death;
	uint16_t sequence_anim_amount;
	uint16_t* sequence_anim;
	
	static BattleModelLinks& GetLinksByModel(uint16_t modelid);
	static bool IsBattleModel(uint16_t modelid);
	
	void ApplyToEnemyStat(EnemyStatDataStruct& es);
};

#endif
