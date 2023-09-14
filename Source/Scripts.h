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

struct ScriptLocalVariableSet;
struct ScriptArgument;
struct ScriptOperation;
struct ScriptFunction;
struct ScriptDataStruct;
struct MultiLanguageScriptDataStruct;

#include <inttypes.h>
#include <fstream>
#include <vector>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct ScriptLocalVariableSet {
	unsigned int amount;
	uint8_t allocate_amount;
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
	int64_t GetValue(); // Get value with sign if needed
	
	void Read(fstream& f, uint8_t sz, bool isvar, bool issign);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, uint8_t sz, bool isvar, bool issign);
	void WriteHWS(fstream& f);
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
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

struct ScriptFunction {
	ScriptDataStruct* parent;
	unsigned int op_amount;
	vector<ScriptOperation> op;
	unsigned int length;

	ScriptFunction& operator=(const ScriptFunction& from);
	ScriptFunction(const ScriptFunction& from);
	ScriptFunction() {}

	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

struct MultiLanguageScriptDataStruct {
	vector< vector<ScriptFunction> > func[STEAM_LANGUAGE_AMOUNT];
	uint16_t magic_number[STEAM_LANGUAGE_AMOUNT];
	uint8_t header_unknown1[STEAM_LANGUAGE_AMOUNT];
	uint8_t entry_amount[STEAM_LANGUAGE_AMOUNT];
	uint8_t header_unknown2[STEAM_LANGUAGE_AMOUNT][20];
	uint8_t header_unknown3[STEAM_LANGUAGE_AMOUNT][20];
	uint8_t header_name[STEAM_LANGUAGE_AMOUNT][SCRIPT_NAME_MAX_LENGTH];
	vector<uint16_t> entry_offset[STEAM_LANGUAGE_AMOUNT];
	vector<uint16_t> entry_size[STEAM_LANGUAGE_AMOUNT];
	vector<uint8_t> entry_local_var[STEAM_LANGUAGE_AMOUNT];
	vector<uint8_t> entry_flag[STEAM_LANGUAGE_AMOUNT];
	vector<uint8_t> entry_type[STEAM_LANGUAGE_AMOUNT];
	vector<uint8_t> entry_function_amount[STEAM_LANGUAGE_AMOUNT];
	vector< vector<uint16_t> > function_type[STEAM_LANGUAGE_AMOUNT];
	vector< vector<uint16_t> > function_point[STEAM_LANGUAGE_AMOUNT];
	ScriptLocalVariableSet global_data[STEAM_LANGUAGE_AMOUNT];
	vector<ScriptLocalVariableSet> local_data[STEAM_LANGUAGE_AMOUNT];
	bool is_loaded[STEAM_LANGUAGE_AMOUNT];
	bool is_modified[STEAM_LANGUAGE_AMOUNT];
	//  If scripts are the same in different languages (except possibly for AT_TEXT arguments),
	// do not duplicate the functions but use a base script and an AT_TEXT argument correspondancy
	SteamLanguage base_script_lang[STEAM_LANGUAGE_AMOUNT];
	vector<uint16_t> base_script_text_id[STEAM_LANGUAGE_AMOUNT]; // lang_script_text_id[lang][i] == Translation(base_script_text_id[base_script_lang[lang]][i])
	vector<uint16_t> lang_script_text_id[STEAM_LANGUAGE_AMOUNT];
};

struct ScriptDataStruct : public ChunkChild {
public:
	FF9String name; // readonly
	vector< vector<ScriptFunction> > func;
	uint16_t magic_number;
	uint8_t header_unknown1;
	uint8_t entry_amount;
	uint8_t header_unknown2[20];
	uint8_t header_unknown3[20];
	uint8_t header_name[SCRIPT_NAME_MAX_LENGTH];
	vector<uint16_t> entry_offset;
	vector<uint16_t> entry_size;
	vector<uint8_t> entry_local_var;
	vector<uint8_t> entry_flag;
	vector<uint8_t> entry_type;
	vector<uint8_t> entry_function_amount;
	vector< vector<uint16_t> > function_type;
	vector< vector<uint16_t> > function_point;
	ScriptLocalVariableSet global_data;
	vector<ScriptLocalVariableSet> local_data;
	
	MultiLanguageScriptDataStruct* multi_lang_script = NULL; // Storage for multiple language scripts ; note that accessing its pointers of index "current_language" is unsafe (use the normal variables instead)
	SteamLanguage current_language;
	uint16_t related_charmap_id;

	ScriptDataStruct& operator=(const ScriptDataStruct& from);
	
	// Limited by a size of 24 bytes ; return 1 if too long (PSX)
	int SetName(wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetName(FF9String& newvalue);
	void AddFunction(int entryid, int funcidpos, uint16_t functype); // Needs 4 or 8 bytes available
	int RemoveFunction(int entryid, int funcid); // Returns nb of bytes freed
	void AddEntry(int entrypos, uint8_t entrytype); // Needs 16 bytes available
	int RemoveEntry(int entrypos, int* modifiedargamount = NULL); // Returns nb of bytes freed ; *modifiedargamount is incremented by the amount of arguments previously using the removed entry
	int ShiftArgument(int argtype, vector<pair<int, int>> shift);
	void ChangeSteamLanguage(SteamLanguage newlang);
	void UpdateSteamMultiLanguage(); // Update the datas of multi_lang_script->data[current_language]

	// Note: Similarity and link methods require multi_lang_script to be up-to-date if lang or baselang is the current_language
	bool CheckLanguageSimilarity(SteamLanguage lang, SteamLanguage baselang, vector<uint16_t>* langtextid = NULL, vector<uint16_t>* baselangtextid = NULL);
	// Use baselang = lang to break a link
	void LinkLanguageScripts(SteamLanguage lang, SteamLanguage baselang, vector<uint16_t> langtextid = vector<uint16_t>(), vector<uint16_t> baselangtextid = vector<uint16_t>(), bool markmodified = true);
	void LinkSimilarLanguageScripts();
	// Replace the dialog IDs without any linking process
	void ApplyDialogLink(vector<uint16_t> langtextid, vector<uint16_t> baselangtextid);

	void Read(fstream& f, SteamLanguage lang);
	void Read(fstream& f) { Read(f, STEAM_LANGUAGE_NONE); }
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool usetext = true, SteamLanguage lang = STEAM_LANGUAGE_NONE);
	void WriteHWS(fstream& f, SteamLanguage lang = STEAM_LANGUAGE_NONE); // Remark: unlike other WriteHWS methods, this writes only 1 language by calls, not all those flagged by hades::STEAM_LANGUAGE_SAVE_LIST
	void ReadLocalHWS(fstream& f, SteamLanguage lang = STEAM_LANGUAGE_NONE);
	void WriteLocalHWS(fstream& f, SteamLanguage lang = STEAM_LANGUAGE_NONE);
	bool IsDataModified(SteamLanguage lang = GetSteamLanguage());
	int GetDataSize(SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
};

vector<ScriptArgument> NewScriptArgumentArray(unsigned int amount, ScriptOperation* p);
bool IsScriptArgTypeSigned(uint8_t argtype);

#endif
