#ifndef _SCRIPTS_H
#define _SCRIPTS_H

#define SCRIPT_NAME_MAX_LENGTH 84

#define SCRIPT_TYPE_FIELD	1
#define SCRIPT_TYPE_BATTLE	2
#define SCRIPT_TYPE_WORLD	3

#define SCRIPT_VARIABLE_LOCALTYPE_UNKNOWN	0
#define SCRIPT_VARIABLE_LOCALTYPE_LOCAL		1
#define SCRIPT_VARIABLE_LOCALTYPE_GLOBAL	2
#define SCRIPT_VARIABLE_LOCALTYPE_GENERAL	3

#define SCRIPT_VARIABLE_TYPE_UNKNOWN	-1
#define SCRIPT_VARIABLE_TYPE_INT		0
#define SCRIPT_VARIABLE_TYPE_UINT		1
#define SCRIPT_VARIABLE_TYPE_BOOL		2
#define SCRIPT_VARIABLE_TYPE_SBOOL		3

struct ScriptLocalVariableSet;
struct ScriptArgument;
struct ScriptOperation;
struct ScriptFunction;
struct ScriptDataStruct;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct ScriptLocalVariableSet {
	unsigned int amount;
	uint8_t allocate_amount;
	uint8_t* local_type;
	int16_t* type;
	uint8_t* size;
	wstring* name;
	uint8_t* cat;
	uint16_t* id;
};

struct ScriptArgument {
	ScriptOperation* parent;
	uint8_t typesize;
	uint8_t size;
	uint32_t value;
	uint8_t* var;
	bool is_var;
	bool is_signed;
	
	bool SetValue(long long newvalue);
	// newvaluevar must be malloc'ed and not freed (the arg takes control of it)
	// no check performed...
	void SetValueVar(uint8_t* newvaluevar, uint8_t newsize);
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
	uint8_t base_code;
	uint8_t ext_code;		// Only used for the base_code 0xFF
	uint8_t vararg_flag;	// Only used for some opcodes
	uint8_t size_byte;		// Only used for some opcodes
	uint8_t arg_amount;
	ScriptArgument* arg;
	unsigned int size;		// size of the full operation
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

struct ScriptFunction {
	ScriptDataStruct* parent;
	unsigned int op_amount;
	ScriptOperation* op;
	unsigned int length;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

struct ScriptDataStruct : public ChunkChild {
public:
	FF9String name; // readonly
	ScriptFunction** func;
	uint16_t magic_number;
	uint8_t header_unknown1;
	uint8_t entry_amount;
	uint8_t header_unknown2[20];
	uint8_t header_unknown3[20];
	uint8_t header_name[SCRIPT_NAME_MAX_LENGTH];
	uint16_t* entry_offset;
	uint16_t* entry_size;
	uint8_t* entry_local_var;
	uint8_t* entry_flag;
	uint8_t* entry_type;
	uint8_t* entry_function_amount;
	uint16_t** function_type;
	uint16_t** function_point;
	ScriptLocalVariableSet global_data;
	ScriptLocalVariableSet* local_data;
	
	uint16_t related_charmap_id;
	
	// Limited by a size of 24 bytes ; return 1 if too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	void AddFunction(int entryid, int funcidpos, uint16_t functype); // Needs 4 or 8 bytes available
	int RemoveFunction(int entryid, int funcid); // Returns nb of bytes freed
	void AddEntry(int entrypos, uint8_t entrytype); // Needs 16 bytes available
	int RemoveEntry(int entrypos, int* modifiedargamount = NULL); // Returns nb of bytes freed ; *modifiedargamount is incremented by the amount of arguments previously using the removed entry
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool usetext = true);
	void WriteHWS(fstream& f);
	void ReadLocalHWS(fstream& f);
	void WriteLocalHWS(fstream& f);
	void Copy(ScriptDataStruct& from, bool deleteold = false);
	void UpdateOffset();
};

ScriptArgument* NewScriptArgumentArray(unsigned int amount, ScriptOperation* p);
bool IsScriptArgTypeSigned(uint8_t argtype);

#endif
