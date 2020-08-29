#ifndef _DATABASE_CSV_H
#define _DATABASE_CSV_H

#include "Database_Common.h"

#define HADES_STRING_DICTIONARY_PATCH_FILE	"DictionaryPatch.txt"

#define HADES_STRING_CSV_SPELL_FILE		"Data\\Battle\\Actions.csv"
#define HADES_STRING_CSV_SPELL_HEADER	L"# This file contains character actions.;;;;;;;;;;;;;;;;;;\n"\
										L"# You must set 192 different items.;;;;;;;;;;;;;;;;;;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;;;;;;;\n"\
										L"# Comment;id;menuWindow;targets;defaultAlly;forDead;defaultOnDead;defaultCamera;animationId1;animationId2;scriptId;power;elements;rate;category;statusIndex;mp;type;\n"\
										L"#;Int32;UInt8;UInt8;Boolean;Boolean;Boolean;Boolean;Int16;UInt16;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;;;;;;;\n"

#define HADES_STRING_CSV_STATUS_FILE	"Data\\Battle\\StatusSets.csv"
#define HADES_STRING_CSV_STATUS_HEADER	L"# This file contains set of statuses what will be applied to targets in the battle.;;;;;;;;;;;;\n"\
										L"# You must set 128 different items.;;;;;;;;;;;;\n"\
										L"# --------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;\n"\
										L"# Comment;Id;State01;State02;State03;State04;State05;State06;State07;State08;State09;State10;\n"\
										L"#;Int32;# UInt32;UInt32;UInt32;UInt32;UInt32;UInt32;UInt32;UInt32;UInt32;UInt32;\n"\
										L"# --------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;\n"

#define HADES_STRING_CSV_STATINIT_FILE		"Data\\Characters\\BaseStats.csv"
#define HADES_STRING_CSV_STATINIT_HEADER	L"# This file contains base stats of characters.;;;;;;\n"\
											L"# You must set 12 different items.;;;;;;\n"\
											L"# -------------------------------------------------------------;;;;;;\n"\
											L"# Comment;Id;Dexterity;Strength;Magic;Will;Gems\n"\
											L"#;Int32;UInt8;UInt8;UInt8;UInt8;UInt8\n"\
											L"# -------------------------------------------------------------;;;;;;\n"

#define HADES_STRING_CSV_COMMAND_FILE	"Data\\Characters\\Commands.csv"
#define HADES_STRING_CSV_COMMAND_HEADER	L"# This file contains battle commands.\n"\
										L"# You must set 48 different items.\n"\
										L"# -----------------------------------\n"\
										L"# Type;Ability;Abilities\n"\
										L"# UInt8;UInt8;UInt8[]\n"\
										L"# -----------------------------------\n"

#define HADES_STRING_CSV_STATCMD_FILE	"Data\\Characters\\CommandSets.csv"
#define HADES_STRING_CSV_STATCMD_HEADER	L"# This file contains character command sets.;;;;\n"\
										L"# You must set 20 different items.;;;;\n"\
										L"# -----------------------------------;;;;\n"\
										L"# Regular 1;Regular 2;Trance 1;Trance 2;\n"\
										L"# UInt8;UInt8;UInt8;UInt8;# Indices from Commands.csv\n"\
										L"# -----------------------------------;;;;\n"

#define HADES_STRING_CSV_SPELLTITLE_FILE	"Data\\Characters\\CommandTitles.csv"
#define HADES_STRING_CSV_SPELLTITLE_HEADER	L"# This file contains map of active ability's id to command's title.;;\n"\
											L"# You must set at least 192 different items.;;\n"\
											L"# -----------------------------------;;\n"\
											L"# Comment;Id;MappedId\n"\
											L"#;Int32;Int32\n"\
											L"# -----------------------------------;;\n"

#define HADES_STRING_CSV_STATEQUIP_FILE		"Data\\Characters\\DefaultEquipment.csv"
#define HADES_STRING_CSV_STATEQUIP_HEADER	L"# This file contains predefined equipment sets;;;;;;;\n"\
											L"# You must set at least 15 different items.;;;;;;;\n"\
											L"# -------------------------------------------------------------;;;;;;;\n"\
											L"# Comment;Id;# Weapon;Head;Wrist;Armor;Accessory;\n"\
											L"#;Int32;# UInt8;UInt8;UInt8;UInt8;UInt8;\n"\
											L"# -------------------------------------------------------------;;;;;;;\n"

#define HADES_STRING_CSV_STATLEVEL_FILE		"Data\\Characters\\Leveling.csv"
#define HADES_STRING_CSV_STATLEVEL_HEADER	L"# This file contains amount of experience to the next level and bonus for HP and MP.;;;\n"\
											L"# You must set 99 different items.;;;\n"\
											L"# ----------------------------------------;;;\n"\
											L"# Experience;BonusHP;BonusMP;\n"\
											L"# UInt32;UInt16;UInt16;\n"\
											L"# ----------------------------------------;;;\n"

#define HADES_STRING_CSV_SUPPORT_FILE	"Data\\Characters\\Abilities\\AbilityGems.csv"
#define HADES_STRING_CSV_SUPPORT_HEADER	L"# This file contains a number of gems, which are required for the activation of passive abilities.;;\n"\
										L"# You must set 64 different items.;;\n"\
										L"# ---------------------------------------;;\n"\
										L"# Comment;Id;Gems\n"\
										L"#;Int32;UInt8\n"\
										L"# ---------------------------------------;;\n"

static string HADES_STRING_CSV_STATABIL_FILE[] = {
	"Data\\Characters\\Abilities\\Zidane.csv",
	"Data\\Characters\\Abilities\\Vivi.csv",
	"Data\\Characters\\Abilities\\Garnet.csv",
	"Data\\Characters\\Abilities\\Steiner.csv",
	"Data\\Characters\\Abilities\\Freya.csv",
	"Data\\Characters\\Abilities\\Quina.csv",
	"Data\\Characters\\Abilities\\Eiko.csv",
	"Data\\Characters\\Abilities\\Amarant.csv",
	"Data\\Characters\\Abilities\\Cinna1.csv",
	"Data\\Characters\\Abilities\\Cinna2.csv",
	"Data\\Characters\\Abilities\\Marcus1.csv",
	"Data\\Characters\\Abilities\\Marcus2.csv",
	"Data\\Characters\\Abilities\\Blank1.csv",
	"Data\\Characters\\Abilities\\Blank2.csv",
	"Data\\Characters\\Abilities\\Beatrix1.csv",
	"Data\\Characters\\Abilities\\Beatrix2.csv"
};
static wstring HADES_STRING_CSV_STATABIL_HEADER[] = {
	L"# This file contains a set of learnable Zidane's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Vivi's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Garnet's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Steiner's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Freya's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Quina's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Eiko's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Amarant's abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Cinna's first set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Cinna's second set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Marcus's first set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Marcus's second set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Blank's first set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Blank's second set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Beatrix's first set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n",
	L"# This file contains a set of learnable Beatrix's second set of abilities.\n"
	L"# You must set 48 different items.\n"
	L"# Use 0 for a void ability, 1-191 for active abilities and 192-254 (id + 192) for passive abilities.\n"
	L"# -----------------------------------\n"
	L"# Id;AP\n"
	L"# UInt8;UInt8\n"
	L"# -----------------------------------\n"
};

#define HADES_STRING_CSV_ARMOR_FILE		"Data\\Items\\Armors.csv"
#define HADES_STRING_CSV_ARMOR_HEADER	L"# This file contains a set of game armors.;;;;;\n"\
										L"# You must set at least 136 different items.;;;;;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;\n"\
										L"# Comment;Id;P.Def;P.Eva;M.Def;M.Eva\n"\
										L"#;Int32;Byte;Byte;Byte;Byte\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;\n"

#define HADES_STRING_CSV_USABLE_FILE	"Data\\Items\\ItemEffects.csv"
#define HADES_STRING_CSV_USABLE_HEADER	L"# This file contains a set of usable item effects.\n"\
										L"# You must set 32 different items.\n"\
										L"#-----------------------------------------------------------------------------------------------------------------------------------------------\n"\
										L"#Targets;DefaultAlly;Display;AnimationId;Dead;DefaultDead;ScriptId;Power;Rate;Element;Status;\n"\
										L"#UInt8;Bit;UInt8;Int16;Bit;Bit;UInt8;UInt8;UInt8;UInt8;UInt32;\n"\
										L"#-----------------------------------------------------------------------------------------------------------------------------------------------\n"

#define HADES_STRING_CSV_ITEM_FILE		"Data\\Items\\Items.csv"
#define HADES_STRING_CSV_ITEM_HEADER	L"# This file contains a set of game items.\n"\
										L"# You must set 256 different items.\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"\
										L"# Price;GraphicsId;ColorId;Quality;BonusId;AbilityIds;Weapon;Armlet;Helmet;Armor;Accessory;Item;Gem;Usable;Order;Zidane;Vivi;Garnet;Steiner;Freya;Quina;Eiko;Amarant;Cinna;Marcus;Blank;Beatrix;\n"\
										L"# UInt16;UInt8;UInt8;UInt8;UInt8;UInt8[];Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;UInt8;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"

#define HADES_STRING_CSV_SHOP_FILE		"Data\\Items\\ShopItems.csv"
#define HADES_STRING_CSV_SHOP_HEADER	L"# This file contains a set of item shops.;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"\
										L"# You must set at least 32 different items.;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"\
										L"# Comment;Id;# Item01;Item02;Item03;Item04;Item05;Item06;Item07;Item08;Item09;Item10;Item11;Item12;Item13;Item14;Item15;Item16;Item17;Item18;Item19;Item20;Item21;Item22;Item23;Item24;Item25;Item26;Item27;Item28;Item29;Item30;Item31;Item32;;\n"\
										L"#;Int32;# UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;UInt8;;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"

#define HADES_STRING_CSV_ITEMSTAT_FILE		"Data\\Items\\Stats.csv"
#define HADES_STRING_CSV_ITEMSTAT_HEADER	L"# This file contains a set of bonuses of equipment.;;;;;;;;;;\n"\
											L"# You must set at least 176 different items.;;;;;;;;;;\n"\
											L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;\n"\
											L"# Comment;Id;Dexterity;Strength;Magic;Will;AttackElement;GuardElement;AbsorbElement;HalfElement;WeakElement\n"\
											L"#;Int32;Byte;Byte;Byte;Byte;Byte;Byte;Byte;Byte;Byte\n"\
											L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;\n"

#define HADES_STRING_CSV_SYNTHESIS_FILE		"Data\\Items\\Synthesis.csv"
#define HADES_STRING_CSV_SYNTHESIS_HEADER	L"# This file contains a set of game items that can be synthesized.;;;;;;\n"\
											L"# You must set 64 different items.;;;;;;\n"\
											L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;\n"\
											L"# Comment;Id;Shops;Price;Result;Item1;Item2\n"\
											L"#;Int32;Byte;Byte;Byte;Byte;Byte\n"\
											L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;\n"

#define HADES_STRING_CSV_WEAPON_FILE	"Data\\Items\\Weapons.csv"
#define HADES_STRING_CSV_WEAPON_HEADER	L"# This file contains a set of game weapons.;;;;;;;;;;\n"\
										L"# You must set 88 different items.;;;;;;;;;;\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;\n"\
										L"# Comment;Id;Category;StatusIndex;Model;ScriptId;Power;Elements;Rate;Offset1;Offset2\n"\
										L"#;Int32;Byte;Byte;Byte;Byte;Byte;Byte;Byte;Int16;Int16\n"\
										L"# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------;;;;;;;;;;\n"

#define HADES_STRING_CSV_MGCSWORD_FILE		"Script\\Sources\\Incomplete\\MagicSwordRequirement.cs"
#define HADES_STRING_CSV_MGCSWORD_HEADER	L"// Memoria does not allow to change the Magic Sword requirements yet.\n"\
											L"// The following script needs to be injected in the Assemble-CSharp.dll in order for this feature to work.\n"\
											L"// It should replace the method \"SetAbilityMagic\" of the class \"BattleHUD\"\n\n"

// Unused anymore; BattleMap change is now written to HADES_STRING_DICTIONARY_PATCH_FILE
#define HADES_STRING_CSV_BATTLEMAP_FILE		"Script\\Sources\\Incomplete\\BattleMap.cs"
#define HADES_STRING_CSV_BATTLEMAP_HEADER	L"// Memoria does not allow to change the battle battlegrounds yet.\n"\
											L"// The following script needs to be injected in the Assemble-CSharp.dll in order for this feature to work.\n"\
											L"// It should replace the corresponding entries of the dictionary \"MapModel\" of the class \"FF9BattleDB\"\n\n"

#endif
