#ifndef _SCRIPTS_H
#define _SCRIPTS_H

#define SCRIPT_NAME_MAX_LENGTH 84

#define SCRIPT_TYPE_ANY		-1
#define SCRIPT_TYPE_FIELD	0
#define SCRIPT_TYPE_BATTLE	1
#define SCRIPT_TYPE_WORLD	2

#define SCRIPT_VARIABLE_LOCALTYPE_UNKNOWN	0
#define SCRIPT_VARIABLE_LOCALTYPE_LOCAL		1
#define SCRIPT_VARIABLE_LOCALTYPE_GLOBAL	2
#define SCRIPT_VARIABLE_LOCALTYPE_GENERAL	3

#define SCRIPT_VARIABLE_TYPE_UNKNOWN	-1
#define SCRIPT_VARIABLE_TYPE_INT		0
#define SCRIPT_VARIABLE_TYPE_UINT		1
#define SCRIPT_VARIABLE_TYPE_BOOL		2
#define SCRIPT_VARIABLE_TYPE_SBOOL		3

#define SCRIPT_CUSTOM_API_FILENAME	"ScriptAPI.txt"

struct ScriptLanguageLink;
struct ScriptLocalVariableSet;
struct ScriptArgument;
struct ScriptOperation;
struct ScriptFunction;
struct ScriptDataStruct;

#include <inttypes.h>
#include <fstream>
#include <vector>
#include <set>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct ScriptLanguageLink {
	// Structure to hold the data for the old HWS storage of similar scripts
	// Not used in any context other than importing old HWS (with CHUNK_STEAM_SCRIPT_MULTILANG)
	SteamLanguage lang_base;
	uint8_t lang_link_count;
	vector<SteamLanguage> lang_link;
	vector<uint16_t> id_count;
	vector<vector<uint16_t>> id_base;
	vector<vector<uint16_t>> id_link;

	bool InitFromHWS(fstream& ffhws, SteamLanguage baselang);
	void Apply(vector<ScriptOperation>& baselangop, int linkindex);
	void RevertLastApply();

private:
	vector<ScriptArgument*> modarg;
	vector<uint16_t> modbaseval;
};

struct ScriptLocalVariableSet {
	unsigned int amount = 0;
	uint8_t allocate_amount = 0;
	vector<uint8_t> local_type;
	vector<int16_t> type;
	vector<uint8_t> size;
	vector<wstring> name;
	vector<uint8_t> cat;
	vector<uint16_t> id;
};

struct ScriptArgument {
	ScriptOperation* parent;
	uint8_t typesize;
	unsigned int size;
	uint32_t value;
	vector<uint8_t> var;
	bool is_var;
	bool is_signed;
	
	bool SetValue(long long newvalue);
	// newvaluevar must be malloc'ed and not freed (the arg takes control of it)
	// no check performed...
	void SetValueVar(vector<uint8_t> newvaluevar);
	int64_t GetValue() const; // Get value with sign if needed
	
	void Read(fstream& f, uint8_t sz, bool isvar, bool issign);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, uint8_t sz, bool isvar, bool issign);
	void WriteHWS(fstream& f);
	void WriteSteam(fstream& f);
};

struct ScriptOperation {
public:
	ScriptFunction* parent;
	uint16_t opcode;
	uint8_t vararg_flag;	// Only used for some opcodes
	uint8_t size_byte;		// Only used for some opcodes
	uint8_t arg_amount;
	vector<ScriptArgument> arg;
	unsigned int size;		// size of the full operation

	ScriptOperation& operator=(const ScriptOperation& from);
	ScriptOperation(const ScriptOperation& from);
	ScriptOperation() {}

	// 0: identical, 1: different argument(s) but same opcode and vararg structure, 2: different vararg formulas but same size, 3: different opcodes or size
	int Compare(const ScriptOperation& other) const;
	void InitialiseArgumentVector(unsigned int count);
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void WriteSteam(fstream& f);
};

struct ScriptFunction {
public:
	ScriptDataStruct* parent;
	vector<ScriptOperation> op;
	vector<int> indices[STEAM_LANGUAGE_AMOUNT];
	uint16_t function_point;
	uint16_t function_type;

	ScriptFunction& operator=(const ScriptFunction& from);
	ScriptFunction(const ScriptFunction& from);
	ScriptFunction() {}

	void SetDefaultScriptForced();
	void Import(const vector<ScriptOperation>& langop, SteamLanguage lang);
	void FlushUnusedOperations();

	void Read(fstream& f, unsigned int length, SteamLanguage lang);
	void Write(fstream& f, SteamLanguage lang);
	void WritePPF(fstream& f, SteamLanguage lang);
	void ReadHWS(fstream& f, unsigned int length, SteamLanguage lang, ScriptLanguageLink* langlink = NULL);
	void WriteHWS(fstream& f);
	void WriteSteam(fstream& f, SteamLanguage lang = GetSteamLanguage());
	unsigned int GetLength(SteamLanguage lang = GetSteamLanguage());
	unsigned int GetSubLength(const vector<unsigned int>& indices);

private:
	bool SearchOperationCorrespondance(const vector<ScriptOperation>& langop, SteamLanguage lang, unsigned int& langindex, unsigned int& searchindex, unsigned int& codecount, unsigned int codediff, set<unsigned int>& elsejumpoffsets, unsigned int& codeoffset);
};

struct ScriptEntry {
public:
	vector<ScriptFunction> func;
	uint16_t offset;
	uint16_t size;
	uint8_t flag; // mostly 0, sometimes 1 for PC entries whose presence depends on whether they are currently in the team or not (Dali fields, Esto Gaza/Altar, Gulug/Path and Pand./Event)
	uint8_t type;
	uint8_t function_amount;

	// Memoria only
	int memoria_id; // 0-63: entries existing in vanilla scripts, 1000+: custom entries
	int player_link;
	uint8_t append_mode; // 1: include the entry to file.eb.bytes, 2: auto-init the entry without changing "Main_Init"
};

struct ScriptDataStruct : public ChunkChild {
public:
	FF9String name; // readonly
	vector<ScriptEntry> entry;
	uint16_t magic_number;
	uint8_t version;
	uint8_t entry_amount;
	uint8_t header_unknown2[20]; // 0 for battle scripts, many non-0 in world/field scripts
	uint8_t header_unknown3[20]; // same as above
	uint8_t header_name[STEAM_LANGUAGE_AMOUNT][SCRIPT_NAME_MAX_LENGTH];
	ScriptLocalVariableSet global_data;
	vector<ScriptLocalVariableSet> local_data;

	bool is_field_script = false;
	bool is_battle_script = false;
	uint8_t has_language = 0;
	uint8_t append_mode = 0;
	uint16_t related_charmap_id;

	ScriptDataStruct& operator=(const ScriptDataStruct& from);
	
	// Limited by a size of 24 bytes ; return 1 if too long (PSX)
	int SetName(wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetName(FF9String& newvalue);
	void AddFunction(int entryid, int funcidpos, uint16_t functype); // Needs 4 or 8 bytes available
	void RemoveFunction(int entryid, int funcid);
	void AddEntry(int entrypos, uint8_t entrytype, int playerlink); // Needs 16 bytes available
	int RemoveEntry(int entrypos); // Returns the amount of arguments previously using the removed entry
	void GuaranteePlayerLinks();
	int ShiftArgument(int argtype, vector<pair<int, int>> shift);

	void Read(fstream& f, SteamLanguage lang);
	void Read(fstream& f) { Read(f, GetSteamLanguage()); }
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool usetext = true, SteamLanguage lang = GetSteamLanguage(), ScriptLanguageLink* langlink = NULL);
	void WriteHWS(fstream& f, bool uselatestversion = true); // Remark: unlike other WriteHWS methods, this writes all the languages that have been read, without taking hades::STEAM_LANGUAGE_SAVE_LIST into account
	void WriteSteam(fstream& f, bool uselatestversion = true, SteamLanguage lang = GetSteamLanguage());
	void ReadLocalHWS(fstream& f);
	void WriteLocalHWS(fstream& f);
	bool IsDataModified(SteamLanguage lang = GetSteamLanguage());
	int GetDataSize(SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset(SteamLanguage lang = GetSteamLanguage());
	void RegisterEnemyCountForOldHWS(int battleenemycount);
};

bool IsScriptArgTypeSigned(uint8_t argtype);

#endif
