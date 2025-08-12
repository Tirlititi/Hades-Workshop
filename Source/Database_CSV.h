#ifndef _DATABASE_CSV_H
#define _DATABASE_CSV_H

#include "Database_Common.h"

#define HADES_STRING_DICTIONARY_PATCH_FILE	"DictionaryPatch.txt"

#define HADES_STRING_CSV_SPELL_FILE		"Data\\Battle\\Actions.csv"
#define HADES_STRING_CSV_SPELL_HEADER	L"#! IncludeCastingTitleType\n"\
										L"# This file contains character actions.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;id;menuWindow;targets;defaultAlly;forDead;defaultOnDead;defaultCamera;animationId1;animationId2;scriptId;power;elements;rate;category;statusIndex;mp;type;commandTitle\n"\
										L"# String;Int32;UInt8;UInt8;Boolean;Boolean;Boolean;Boolean;Int16;UInt16;Int32;Int32;UInt8;Int32;UInt8;Int32;Int32;UInt8;UInt8\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATUS_FILE	"Data\\Battle\\StatusSets.csv"
#define HADES_STRING_CSV_STATUS_HEADER	L"#! UnshiftStatuses\n"\
										L"# This file contains set of statuses what will be applied to targets in the battle.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;Id;Statuses\n"\
										L"# String;Int32;UInt32[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATUSDATA_FILE	"Data\\Battle\\StatusData.csv"
#define HADES_STRING_CSV_STATUSDATA_HEADER	L"#! UnshiftStatuses\n"\
											L"#! IncludeVisuals\n"\
											L"# This file contains data of statuses.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;Priority(unused);OprCount(tick);ContiCount(duration);ClearOnApply;ImmunityProvided;SPSEffect;SPSAttach;SPSExtraPos;SHPEffect;SHPAttach;SHPExtraPos;ColorKind;ColorPriority;ColorBase\n"\
											L"# ;Int32;UInt8;UInt8;UInt16;Status[];Status[];Int32;Int32;Vector3;Int32;Int32;Vector3;Int32;Int32;Int32[3]\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_CHARPARAM_FILE		"Data\\Characters\\CharacterParameters.csv"
#define HADES_STRING_CSV_CHARPARAM_HEADER	L"# This file define basic parameters of characters.\n"\
											L"# -----------------------------------------------\n"\
											L"# Id;DefaultRow;DefaultWinPose;DefaultCategory;DefaultCommandSet;DefaultEquipmentSet;BattleParameterFormula;NameKeyword\n"\
											L"# Int32;Boolean;Boolean;UInt8;Int32;Int32;String;String\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_BATTLEPARAM_FILE	"Data\\Characters\\BattleParameters.csv"
#define HADES_STRING_CSV_BATTLEPARAM_HEADER	L"#! IncludeWeaponSound\n"\
											L"# This file define battle parameters of characters.\n"\
											L"# -----------------------------------------------\n"\
											L"# Id;AvatarSprite;ModelId;TranceModelId;TranceGlowingColor;MP_IDLE_NORMAL;MP_IDLE_DYING;MP_DAMAGE1;MP_DAMAGE2;MP_DISABLE;MP_GET_UP_DYING;MP_GET_UP_DISABLE;MP_DOWN_DYING;MP_DOWN_DISABLE;MP_IDLE_CMD;MP_NORMAL_TO_CMD;MP_DYING_TO_CMD;MP_IDLE_TO_DEF;MP_DEFENCE;MP_DEF_TO_IDLE;MP_COVER;MP_AVOID;MP_ESCAPE;MP_WIN;MP_WIN_LOOP;MP_SET;MP_RUN;MP_RUN_TO_ATTACK;MP_ATTACK;MP_BACK;MP_ATK_TO_NORMAL;MP_IDLE_TO_CHANT;MP_CHANT;MP_MAGIC;MP_STEP_FORWARD;MP_STEP_BACK;MP_ITEM1;MP_CMD_TO_NORMAL;MP_SPECIAL;AttackSequence;WeaponBone;ShadowData;StatusBones;StatusOffsetsY;StatusOffsetsZ;WeaponSounds\n"\
											L"# Int32;String;String;String;UInt8[];String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;String;Int32;UInt8;UInt8[];UInt8[];Int8[];Int8[];Int32[]\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATINIT_FILE		"Data\\Characters\\BaseStats.csv"
#define HADES_STRING_CSV_STATINIT_HEADER	L"# This file contains base stats of characters.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;Dexterity;Strength;Magic;Will;Gems\n"\
											L"# String;Int32;Int32;Int32;Int32;Int32;Int32\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_COMMAND_FILE	"Data\\Characters\\Commands.csv"
#define HADES_STRING_CSV_COMMAND_HEADER	L"#! IncludeId\n"\
										L"# This file contains battle commands.\n"\
										L"# -----------------------------------------------\n"\
										L"# Id;Type;Ability;Abilities\n"\
										L"# Int32;UInt8;Int32;Int32[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATCMD_FILE	"Data\\Characters\\CommandSets.csv"
#define HADES_STRING_CSV_STATCMD_HEADER	L"#! IncludeId\n"\
										L"#! IncludeFullSet\n"\
										L"# This file contains character command sets.\n"\
										L"# -----------------------------------------------\n"\
										L"# Id;Attack;Defend;Regular 1;Regular 2;Item;Change;Attack(Trance);Defend(Trance);Trance 1;Trance 2;Item(Trance);Change(Trance)\n"\
										L"# Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32;Int32\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_SPELLTITLE_FILE	"Data\\Characters\\CommandTitles.csv"
#define HADES_STRING_CSV_SPELLTITLE_HEADER	L"# This file contains map of active ability's id to command's title.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;MappedId\n"\
											L"# String;Int32;Int32\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATEQUIP_FILE		"Data\\Characters\\DefaultEquipment.csv"
#define HADES_STRING_CSV_STATEQUIP_HEADER	L"# This file contains predefined equipment sets.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;Weapon;Head;Wrist;Armor;Accessory\n"\
											L"# String;Int32;Int32;Int32;Int32;Int32;Int32\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_STATLEVEL_FILE		"Data\\Characters\\Leveling.csv"
#define HADES_STRING_CSV_STATLEVEL_HEADER	L"# This file contains amount of experience to the next level and bonus for HP and MP.\n"\
											L"# -----------------------------------------------\n"\
											L"# Experience;BonusHP;BonusMP\n"\
											L"# UInt32;UInt16;UInt16\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_SUPPORT_FILE	"Data\\Characters\\Abilities\\AbilityGems.csv"
#define HADES_STRING_CSV_SUPPORT_HEADER	L"#! IncludeBoosted\n"\
										L"# This file contains a number of gems, which are required for the activation of passive abilities.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;Id;Gems;BoostedVersion(s)\n"\
										L"# String;Int32;Int32;Int32[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_MGCSWORD_FILE		"Data\\Battle\\MagicSwordSets.csv"
#define HADES_STRING_CSV_MGCSWORD_HEADER	L"#! IncludeStatusBlockers\n"\
											L"# This file contains sets of ablities that get unlocked by other abilities.\n"\
											L"# -----------------------------------------------\n"\
											L"# Id;Supporter;Beneficiary;BaseAbilities;UnlockedAbilities;SupporterBlockingStatus;BeneficiaryBlockingStatus\n"\
											L"# Int32;Int32;Int32;Ability[];Ability[];Status[];Status[]\n"\
											L"# -----------------------------------------------\n"

extern vector<string> HADES_STRING_CSV_STATABIL_FILE;
extern vector<wstring> HADES_STRING_CSV_STATABIL_HEADER;
#define HADES_STRING_CSV_STATABIL_DEFAULT_FILE		"Data\\Characters\\Abilities\\"
#define HADES_STRING_CSV_STATABIL_DEFAULT_HEADER	L"# This file contains a set of learnable abilities for a custom character.\n"\
													L"# -----------------------------------\n"\
													L"# Id;AP\n"\
													L"# Ability;Int32\n"\
													L"# -----------------------------------\n"

#define HADES_STRING_CSV_ARMOR_FILE		"Data\\Items\\Armors.csv"
#define HADES_STRING_CSV_ARMOR_HEADER	L"# This file contains a set of game armors.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;Id;P.Def;P.Eva;M.Def;M.Eva\n"\
										L"# String;Int32;Int32;Int32;Int32;Int32\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_USABLE_FILE	"Data\\Items\\ItemEffects.csv"
#define HADES_STRING_CSV_USABLE_HEADER	L"#! IncludeId\n"\
										L"#! UseStatusList\n"\
										L"# This file contains a set of usable item effects.\n"\
										L"# -----------------------------------------------\n"\
										L"# Id;Targets;DefaultAlly;Display;AnimationId;Dead;DefaultDead;ScriptId;Power;Rate;Element;Status\n"\
										L"# Int32;UInt8;Bit;UInt8;Int16;Bit;Bit;Int32;Int32;Int32;UInt8;Status[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_ITEM_FILE		"Data\\Items\\Items.csv"
#define HADES_STRING_CSV_ITEM_HEADER	L"#! IncludeId\n"\
										L"#! IncludeAuxiliaryIds\n"\
										L"#! IncludeSellingPrice\n"\
										L"# This file contains a set of game items.\n"\
										L"# -----------------------------------------------\n"\
										L"# Id;WeaponId;ArmorId;EffectId;Price;SellingPrice;GraphicsId;ColorId;Quality;BonusId;AbilityIds;Weapon;Armlet;Helmet;Armor;Accessory;Item;Gem;Usable;Order;Zidane;Vivi;Garnet;Steiner;Freya;Quina;Eiko;Amarant;Cinna;Marcus;Blank;Beatrix\n"\
										L"# Int32;Int32;Int32;Int32;UInt32;Int32;Int32;UInt8;Single;Int32;Ability[];Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Single;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit;Bit\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_SHOP_FILE		"Data\\Items\\ShopItems.csv"
#define HADES_STRING_CSV_SHOP_HEADER	L"# This file contains a set of item shops.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;Id;Items\n"\
										L"# String;Int32;Int32[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_ITEMSTAT_FILE		"Data\\Items\\Stats.csv"
#define HADES_STRING_CSV_ITEMSTAT_HEADER	L"# This file contains a set of bonuses of equipment.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;Dexterity;Strength;Magic;Will;AttackElement;GuardElement;AbsorbElement;HalfElement;WeakElement\n"\
											L"# String;Int32;Byte;Byte;Byte;Byte;Byte;Byte;Byte;Byte;Byte\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_SYNTHESIS_FILE		"Data\\Items\\Synthesis.csv"
#define HADES_STRING_CSV_SYNTHESIS_HEADER	L"#! UseShopList\n"\
											L"# This file contains a set of game items that can be synthesized.\n"\
											L"# -----------------------------------------------\n"\
											L"# Comment;Id;Shops;Price;Result;Ingredients\n"\
											L"# String;Int32;Int32[];UInt32;Int32;Int32[]\n"\
											L"# -----------------------------------------------\n"

#define HADES_STRING_CSV_WEAPON_FILE	"Data\\Items\\Weapons.csv"
#define HADES_STRING_CSV_WEAPON_HEADER	L"#! IncludeHitSfx\n"\
										L"#! IncludeCustomTexture\n"\
										L"# This file contains a set of game weapons.\n"\
										L"# -----------------------------------------------\n"\
										L"# Comment;Id;Category;StatusIndex;Model;ScriptId;Power;Elements;Rate;Offset1;Offset2;HitSfx;CustomTexture\n"\
										L"# String;Int32;Byte;Int32;String;Int32;Int32;Byte;Int32;Int16;Int16;Int32;String[]\n"\
										L"# -----------------------------------------------\n"

#define HADES_STRING_EQUIP_PATCH_FILE		"Data\\Items\\ItemEquipPatch.txt"
#define HADES_STRING_SKILL_PATCH_FILE		"Data\\Items\\ItemAbilityPatch.txt"
#define HADES_STRING_SHOP_PATCH_FILE		"Data\\Items\\ItemShopPatch.txt"

// Unused anymore; BattleMap change is now written to HADES_STRING_DICTIONARY_PATCH_FILE
#define HADES_STRING_CSV_BATTLEMAP_FILE		"Script\\Sources\\Incomplete\\BattleMap.cs"
#define HADES_STRING_CSV_BATTLEMAP_HEADER	L"// Memoria does not allow to change the battle battlegrounds yet.\n"\
											L"// The following script needs to be injected in the Assemble-CSharp.dll in order for this feature to work.\n"\
											L"// It should replace the corresponding entries of the dictionary \"MapModel\" of the class \"FF9BattleDB\"\n\n"

#endif
