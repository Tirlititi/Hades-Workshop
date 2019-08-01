#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <inttypes.h>
#include <fstream>
#include <string>

#define DATA_SECTION_AMOUNT 16
enum {
	DATA_SECTION_SPELL,
	DATA_SECTION_CMD,
	DATA_SECTION_ENMY,
	DATA_SECTION_SHOP,
	DATA_SECTION_TEXT,
	DATA_SECTION_BATTLE_SCENE,
	DATA_SECTION_WORLD_MAP,
	DATA_SECTION_ITEM,
	DATA_SECTION_SUPPORT,
	DATA_SECTION_STAT,
	DATA_SECTION_CARD,
	DATA_SECTION_FIELD,
	DATA_SECTION_SPELL_ANIM,
	DATA_SECTION_MENU_UI,
	DATA_SECTION_PARTY_SPECIAL,
	DATA_SECTION_ASSEMBLY,
	DATA_SECTION_MIPS = DATA_SECTION_ASSEMBLY,
	DATA_SECTION_CIL = DATA_SECTION_ASSEMBLY
};

typedef uint8_t GameType;
#define GAME_TYPE_PSX	0
#define GAME_TYPE_STEAM	1

#define STEAM_SPECIAL_TEXT_AMOUNT	7
#define STEAM_WORLD_MAP_TEXT_ID		68

typedef uint8_t LanguageVersion;
#define LANGUAGE_VERSION_UNKNOWN	0
#define LANGUAGE_VERSION_JAPAN		1
#define LANGUAGE_VERSION_AMERICA	2
#define LANGUAGE_VERSION_FRANCE		4
#define LANGUAGE_VERSION_ENGLAND	8
#define LANGUAGE_VERSION_GERMANY	16
#define LANGUAGE_VERSION_EUROPE		LANGUAGE_VERSION_FRANCE | LANGUAGE_VERSION_ENGLAND | LANGUAGE_VERSION_GERMANY
#define LANGUAGE_VERSION_PALUS		LANGUAGE_VERSION_EUROPE | LANGUAGE_VERSION_AMERICA

struct ConfigurationSet;
struct SaveSet;
struct UnusedSaveBackupPart;
struct UnusedSaveBackup;

#include "File_Manipulation.h"
#include "Spells.h"
#include "Supports.h"
#include "Items.h"
#include "Cards.h"
#include "Stats.h"
#include "Commands.h"
#include "Shops.h"
#include "ClusterData.h"
#include "ImageMaps.h"
#include "TIMImages.h"
#include "Charmaps.h"
#include "Texts.h"
#include "Scripts.h"
#include "Fields.h"
#include "Enemies.h"
#include "WorldMaps.h"
#include "BattleScenes.h"
#include "SpellAnimations.h"
#include "PartySpecials.h"
#include "MenuUI.h"
#include "MIPS.h"
#include "UnityArchiver.h"
#include "DllEditor.h"
using namespace std;

struct ConfigurationSet {
public:
	// Header
	uint16_t version;
	uint16_t headersize;
	uint8_t cd;
	LanguageVersion language;
	GameType gametype;
	
	string steam_dir_assets;
	string steam_dir_data;
	string steam_dir_managed;

	UnityArchiveIndexListData indexlist_res;
	UnityArchiveAssetBundle bundle_script;
	UnityArchiveAssetBundle bundle_battle;
	UnityArchiveAssetBundle bundle_world;
	UnityArchiveAssetBundle bundle_field[9];
	UnityArchiveMetaData meta_res;
	UnityArchiveMetaData meta_main;
	UnityArchiveMetaData meta_script;
	UnityArchiveMetaData meta_battle;
	UnityArchiveMetaData meta_world;
	UnityArchiveMetaData meta_field[9];
	UnityArchiveMetaData meta_atlas;
	DllMetaData meta_dll;
	
	// Assembly-CSharp sections
	int32_t dll_battledb_method_id;
	int32_t dll_ability_method_id;
	int32_t dll_rdata_method_id;
	int32_t dll_level_method_id;
	int32_t dll_item_method_id;
	int32_t dll_armor_method_id;
	int32_t dll_weapon_method_id;
	int32_t dll_equip_method_id;
	int32_t dll_mix_method_id;
	int32_t dll_statusset_field_id;
	int32_t dll_spellnaming_field_id;
	int32_t dll_cmdspelllist_field_id;
	int32_t dll_statexp_field_id;
	int32_t dll_statcmdtrance_field_id;
	int32_t dll_magicsword_field_id;
	int32_t dll_equipset_field_id[EQUIP_SET_AMOUNT];
	int32_t dll_shop_field_id[SHOP_AMOUNT];
	
	// Spell Config
	uint32_t spell_data_offset[5];
	uint32_t spell_text_offset;
	uint32_t spell_naming_rules_offset;
	uint16_t spell_name_space_total;
	uint16_t spell_help_space_total;
	
	int32_t spell_help_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spell_name_file[STEAM_LANGUAGE_AMOUNT];
	
	// Command config
	uint32_t cmd_data_offset[3];
	uint16_t cmd_name_space_total;
	uint16_t cmd_help_space_total;
	
	int32_t cmd_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t cmd_help_file[STEAM_LANGUAGE_AMOUNT];
	
	// Item config
	uint32_t item_data_offset;
	uint32_t item_stat_offset;
	uint32_t item_weaponstat_offset;
	uint32_t item_help_offset[4];
	uint16_t item_name_space_total;
	uint16_t item_help_space_total;
	uint16_t item_help_space2_total;
	uint16_t item_key_name_space_total;
	uint16_t item_key_help_space_total;
	uint16_t item_key_desc_space_total;
	
	int32_t item_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t item_help_file[STEAM_LANGUAGE_AMOUNT];
	int32_t item_help2_file[STEAM_LANGUAGE_AMOUNT];
	int32_t itemkey_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t itemkey_help_file[STEAM_LANGUAGE_AMOUNT];
	int32_t itemkey_desc_file[STEAM_LANGUAGE_AMOUNT];
	
	// Support config
	uint32_t support_data_offset;
	uint32_t support_text_offset[4];
	uint16_t support_name_space_total;
	uint16_t support_help_space_total;
	
	int32_t support_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t support_help_file[STEAM_LANGUAGE_AMOUNT];
	
	// Stat config
	uint32_t stat_defaultname_offset;
	uint32_t stat_initialequip_offset;
	uint32_t stat_initialstat_offset;
	uint32_t stat_cmddata_offset[3];
	uint16_t stat_defaultname_space_total;
	
	// Card config
	uint32_t card_text_offset;
	uint32_t card_set_offset;
	uint32_t card_deck_offset;
	uint32_t card_stat_offset[2];
	uint16_t card_name_space_total;
	
	int32_t card_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t card_set_file;
	int32_t card_deck_file;
	int32_t card_stat_file;
	
	// Cluster Data config
	uint16_t cluster_amount;
	uint32_t* cluster_offset;
	
	// Unity Asset files
	uint16_t enmy_amount;
	int32_t* enmy_text_file[STEAM_LANGUAGE_AMOUNT];
	int32_t* enmy_script_file[STEAM_LANGUAGE_AMOUNT];
	int32_t* enmy_stat_file;
	int32_t* enmy_battle_file;
	uint16_t* enmy_id;
	
	uint16_t text_amount;
	int32_t* text_file[STEAM_LANGUAGE_AMOUNT];
	uint16_t* text_id;
	
	uint16_t field_amount;
	int32_t field_title_info;
	int32_t field_text_file[STEAM_LANGUAGE_AMOUNT];
	int32_t* field_script_file[STEAM_LANGUAGE_AMOUNT];
	int32_t* field_role_file;
	int32_t* field_preload_file;
	int32_t* field_walkmesh_file;
	int32_t* field_image_file;
	int32_t** field_tiles_file;
	bool* field_tiles_localized;
	uint8_t* field_file_id;
	uint16_t* field_id;
	
	uint16_t world_amount;
	int32_t world_fx_file[2];
	int32_t world_disc_file[2];
	int32_t world_discmr_file[2];
	int32_t world_worldplace_name_file[STEAM_LANGUAGE_AMOUNT];
	int32_t world_text_file[STEAM_LANGUAGE_AMOUNT];
	int32_t* world_script_file[STEAM_LANGUAGE_AMOUNT];
	uint16_t* world_id;
	
	// Shop config
	uint32_t shop_data_offset;
	uint32_t synthesis_data_offset;
	
	// Special Texts
	uint32_t spetext_amount;
	uint32_t* spetext_offset;
	uint16_t* spetext_space_total;
	uint32_t* spetext_steam_space_total;
	
	int32_t spetext_battleinfo_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_battlescan_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_spellnaming_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_chocomenu_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_cardrank_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_tetramaster_file[STEAM_LANGUAGE_AMOUNT];
	int32_t spetext_localization_file;
	uint32_t spetext_battleinfo_amount;
	uint32_t spetext_battlescan_amount;
	uint32_t spetext_spellnaming_amount;
	uint32_t spetext_chocomenu_amount;
	uint32_t spetext_cardrank_amount;
	uint32_t spetext_tetramaster_amount;
	uint32_t spetext_localization_amount;

	int32_t atlas_icontexture_file;
	int32_t atlas_iconsprite_file;
	
	// Party Special
	uint32_t party_magicsword_offset;
	
	// Spell Animations
	int32_t spellanim_steam_file[SPELL_ANIMATION_AMOUNT];
	
	// MIPS
	uint32_t mips_battle_offset_list;
	uint32_t mips_battle_code_offset;
	uint32_t mips_battle_code_amount;

	string GetSteamAssetPath(UnityArchiveFile arch, int32_t fileid);
};

struct SaveSet {
public:
	SpellDataSet* spellset;
	CommandDataSet* cmdset;
	EnemyDataSet* enemyset;
	ShopDataSet* shopset;
	TextDataSet* textset;
	WorldMapDataSet* worldset;
	BattleSceneDataSet* sceneset;
	ItemDataSet* itemset;
	SupportDataSet* supportset;
	StatDataSet* statset;
	CardDataSet* cardset;
	FieldDataSet* fieldset;
	SpellAnimationDataSet* spellanimset;
	MenuUIDataSet* ffuiset;
	PartySpecialDataSet* partyspecialset;
	MipsDataSet* mipsset;
	CILDataSet* cilset;
	
	SaveSet(SpellDataSet* sp, CommandDataSet* cmd, EnemyDataSet* enmy, ShopDataSet* shop, TextDataSet* text,/*
		*/  WorldMapDataSet* world, BattleSceneDataSet* scene, ItemDataSet* item, SupportDataSet* support,/*
		*/  StatDataSet* stat, CardDataSet* card, FieldDataSet* field, SpellAnimationDataSet* spellanim,/*
		*/  MenuUIDataSet* ui, PartySpecialDataSet* partyspecial, MipsDataSet* mips, CILDataSet* cil);
};

struct UnusedSaveBackupPart {
public:
	unsigned int save_amount;
	uint32_t* save_size;
	uint8_t** save_data;
	
	UnusedSaveBackupPart() : save_amount(0), save_size(NULL), save_data(NULL) {}
	void Add(fstream& f, uint32_t size);
};

struct UnusedSaveBackup {
public:
	UnusedSaveBackupPart enemy;
	UnusedSaveBackupPart text;
	UnusedSaveBackupPart scene;
	UnusedSaveBackupPart world;
	UnusedSaveBackupPart field;
	UnusedSaveBackupPart* menu_ui;
	
	UnusedSaveBackup() : enemy(), text(), scene(), world(), field(), menu_ui(new UnusedSaveBackupPart[1]) {}
};

// Return -1 if the file can't be opened, -2 if the config file can't be opened (or doesn't exist),
// Return 1 if configuration is oudated and 0 on success (in which case, config is loaded).
int FileCheckConfig(string filename, ConfigurationSet& config);
// Return 0 if full success.
int LoadConfiguration(string filepath, ConfigurationSet& dest);
int WriteConfiguration(string filepath, ConfigurationSet& conf);
int FindConfiguration(string filepath, ConfigurationSet& dest);
int InitSteamConfiguration(string filepath, ConfigurationSet& dest);

int PreloadHWS(string filepath, bool* section);
wstring* LoadHWS(string filepath, bool* section, bool* sectext, bool* localsec, SaveSet& saveset, UnusedSaveBackup& backup);
int WriteHWS(string filepath, bool* section, bool* localsec, SaveSet& saveset, UnusedSaveBackup& backup);

int CreateSteamMod(string destfolder, bool* section, ConfigurationSet& config, SaveSet& saveset, int dllformat, int assetformat, bool deleteold);

GameType GetGameType();
void SetGameType(GameType gt);
GameType GetHWSGameType();
void SetHWSGameType(GameType gt);

#endif
