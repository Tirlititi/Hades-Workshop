#ifndef _DLLEDITOR_H
#define _DLLEDITOR_H

/*  Assembly-CSharp.dll contains the code of the game's engine.
 *  Several data bases can be found inside and must be decompiled.
 * 
 *  DLL Structure :
 * DOS header (0 - 0x80)
 * PE header (0x80 - 0x200)
 * Main section header (0x200 - 0x2EC)
 * Methods (including exception handlers) + constant fields (0x2EC - MetaData offset)
 * MetaData header (up to Table heap)
 * Table heap (up to Strings heap)
 * Strings heap (up to User Strings heap)
 * User strings heap (up to Blob heap)
 * Blob heap (up to GUID heap)
 * GUID heap (up to Filesize - 0x600)
 * RSRC section (size of 0x400)
 * Reloc section (size of 0x200)
 */

struct DllDataTable;
struct DllDataDirectory;
struct DllMetaData;
struct DllMetaDataModification;
struct DllMethodInfo;
struct ILInstruction;
struct CILDataSet;

#define DLL_TABLE_HEAP_COUNT 45
#define DLL_TABLE_CODED_INDEX 13

#include "File_IOCommon.h"
#include "DllCilMacro.h"
#include "Database_Assembly.h"
#include "Database_Steam.h"

struct DllDataDirectory {
	uint32_t virtual_address;
	uint32_t size;
};

struct DllDataTable {
	uint32_t length;
	uint32_t offset;
	uint32_t row_size;
};

struct DllMetaData {
public:
	fstream dll_file;
	uint32_t pe_header_offset;
	uint16_t architecture;
	uint16_t section_amount;
	uint16_t characteristics;
	uint16_t pe_type;
	uint16_t subsystem;
	uint16_t dll_characteristics;
	DllDataDirectory cli_directory;
	DllDataDirectory meta_data_directory;
	DllDataDirectory resources_directory;
	DllDataDirectory strong_name_directory;
	
	string* section_name;
	uint32_t* section_virtual_size;
	uint32_t* section_virtual_offset;
	uint32_t* section_size;
	uint32_t* section_offset;
	
	uint32_t cli_attributes;
	uint32_t cli_entry_point_token;
	
	uint16_t meta_data_section;
	uint32_t meta_data_version_name_size;
//	string meta_data_version_name;
	uint16_t meta_data_stream_amount;
	
	int16_t stream_table_heap;
	int16_t stream_string_heap;
	int16_t stream_blob_heap;
	int16_t stream_guid_heap;
	int16_t stream_user_string_heap;
	int stream_string_heap_index_size;
	int stream_blob_heap_index_size;
	int stream_guid_heap_index_size;
	uint32_t* stream_offset;
	uint32_t* stream_size;
	string* stream_name;
	
	uint8_t table_heap_sizes;
	uint64_t table_heap_valid;
	DllDataTable table[DLL_TABLE_HEAP_COUNT];
	
	char* string_heap_raw;
	uint16_t* cil_module_generation;
	uint32_t* cil_module_name_id;
	uint32_t* cil_module_mv_id;
	uint32_t* cil_module_enc_id;
	uint32_t* cil_module_encbase_id;
	uint32_t* cil_typeref_resolution;
	uint32_t* cil_typeref_name_id;
	uint32_t* cil_typeref_namespace_id;
	uint32_t* cil_type_attributes;
	uint32_t* cil_type_name_id;
	uint32_t* cil_type_namespace_id;
	uint32_t* cil_type_basetype;
	uint32_t* cil_type_field_id;
	uint32_t* cil_type_method_id;
	uint16_t* cil_field_flags;
	uint32_t* cil_field_name_id;
	uint32_t* cil_field_signature;
	uint32_t* cil_method_virtual_address;
	uint16_t* cil_method_implflags;
	uint16_t* cil_method_flags;
	uint32_t* cil_method_name_id;
	uint32_t* cil_method_signature;
	uint32_t* cil_method_param_id;
	uint16_t* cil_param_flags;
	uint16_t* cil_param_sequence;
	uint32_t* cil_param_name_id;
	uint32_t* cil_member_class;
	uint32_t* cil_member_name_id;
	uint32_t* cil_member_signature;
	uint32_t* cil_moduleref_name_id;
	uint32_t* cil_typespec_signature;
	uint32_t* cil_fieldrva_virtual_address;
	uint32_t* cil_fieldrva_field_id;
	uint16_t* cil_genericparam_index;
	uint16_t* cil_genericparam_flags;
	uint32_t* cil_genericparam_owner;
	uint32_t* cil_genericparam_name_id;
	uint32_t* cil_methodspec_method;
	uint32_t* cil_methodspec_signature;
	
	unsigned int user_string_amount;
	uint32_t* user_string_offset;
	
	~DllMetaData() { if (dll_file.is_open()) dll_file.close(); }
	
	int Load(const char* fname);
	int GetMethodIdByName(const char* tname, const char* mname, const char* namesp = NULL);
	uint32_t GetMethodOffset(unsigned int methid);
	uint32_t GetMethodRange(unsigned int methid);
	int GetStaticFieldIdFromToken(uint32_t fieldtoken);
	uint32_t GetStaticFieldOffset(unsigned int staticfieldid);
	uint32_t GetStaticFieldRange(unsigned int staticfieldid);
	uint32_t GetTypeTokenIdentifier(const char* tname, const char* namesp = NULL);
	uint32_t GetTypeRefTokenIdentifier(const char* tname, const char* namesp = NULL);
	uint32_t GetFieldTokenIdentifier(const char* tname, const char* fname);
	uint32_t GetMethodTokenIdentifier(const char* tname, const char* mname, const char* namesp = NULL);
	uint32_t GetMemberTokenIdentifier(const char* descname);
	uint32_t GetStringToken(wstring value);
	
	unsigned int GetTypeAmount();
	unsigned int GetTypeMethodAmount(unsigned int tid);
	void GetMethodRelativeId(unsigned int absmethid, unsigned int* tid, unsigned int* mid);
	string GetPrimitiveTypeName(uint8_t elemtype);
	string GetTypeName(unsigned int tid, bool shownamespace = false);
	string GetTypeRefName(unsigned int tid, bool shownamespace = false);
	string GetTypeSpecName(unsigned int tid);
	string GetMethodName(unsigned int tid, unsigned int mid);
	string GetMethodNameById(unsigned int absmethid, bool showtype = true, bool shownamespace = true);
	string GetMethodSpecName(unsigned int mid);
	string GetModuleName(unsigned int modid);
	string GetModuleRefName(unsigned int modid);
	string GetFieldName(unsigned int fid, bool showtype = true);
	string GetParamName(unsigned int pid);
	string GetMemberName(unsigned int mid, bool showclass = true);
	string GetTokenDescription(uint32_t token, bool fulldesc = true);
	string GetNameAtPos(uint32_t stroff);
	wstring GetStringTokenDescription(uint32_t token);
	wstring GetUserStringAtPos(uint32_t stroff, bool conservedllfilepos = true);
	
	// sizes in bits ; assuming the sum is a multiple of 8
	uint8_t* ConvertScriptToRaw_Object(unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[] = NULL);
	int64_t** ConvertScriptToRaw_Array2(unsigned int objamount, unsigned int arrayamount);
	DllMetaDataModification ConvertRawToScript_Object(uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[] = NULL);
	DllMetaDataModification ConvertRawToScript_Array2(uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int arrayamount, uint32_t arraysettoken);
	int64_t ScriptGetNextIntegerValue(bool includestr, uint32_t* integercodepos = NULL, SteamWDictionary* strdico = NULL);
	
	int Duplicate(fstream& fdest, unsigned int modifamount, DllMetaDataModification* modif); // Change dll_file stream

private:
	int coded_index_size[DLL_TABLE_CODED_INDEX];
	string* cil_member_string;
	
	void GetMethodSignatureString(uint8_t callingconvention, string* returntype, string* parameters); // Change dll_file stream
	string GetSignatureString(uint32_t signpos = 0); // Change dll_file stream
	void ComputeTableInformations(uint32_t baseoffset);
	int GetSectionAtVirtualAddress(uint32_t address);
	uint32_t GetVirtualAddressOffset(uint32_t address);
	uint32_t GetStreamOffset(unsigned int stream);
	uint32_t ReadCompressedUInt32(); // Change dll_file stream
	int GetCodedIndexSize(unsigned int codedindex);
	int GetTableIndexSize(unsigned int tableindex);
	int GetMethodAtOffset(uint32_t offset);
	bool IsNameMatching(uint32_t stroff, const char* match);
	
	// Change dll_file stream - Return a new'd list of methodinfo matching the modif (NULL for non-method modifs)
	DllMethodInfo** ComputeTinyFatAndMethodInfo(unsigned int modifamount, DllMetaDataModification* modif);
	uint32_t ApplyVirtualOffsetModification(uint32_t baserva, uint32_t newsecoff, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo);
	static uint32_t ApplyOffsetModification(uint32_t offset, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo, int32_t baseoffset = 0);
	static uint32_t ApplySizeModification(uint32_t size, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo);
	// Change dll_file stream - Return base size
	uint32_t WriteMethodSizeModification(fstream& fdest, uint32_t methid, unsigned int modifamount, DllMetaDataModification* modif);

	friend DllMetaDataModification;
};

struct DllMetaDataModification {
	uint32_t position;
	uint32_t base_length;
	uint32_t new_length;
	uint8_t* value;
	
	int method_id;
	int tiny_fat_change;
	int8_t base_end_padding;
	uint32_t code_block_pos = 0;
	
	int32_t GetSizeModification(unsigned int modifid, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** methinfolist);
};

struct DllMethodInfo {
	size_t code_abs_offset; // readonly
	bool is_tiny; // readonly
	uint16_t flags;
	uint16_t max_stack_size;
	uint32_t code_size;
	uint32_t local_var_token;
	
	void ReadMethodInfo(fstream& f);
	void WriteMethodInfo(fstream& f);
	uint32_t JumpToInstructions(fstream& f, unsigned int amount, ILInstruction* inst, bool ignoreoffsetval = false);
	
	void UpdateInformation(); // Update code_abs_offset and is_tiny if the other fields were modified
	
	// Same as ReadMethodInfo without storing infos
	static uint32_t JumpToCode(fstream& f);
	// Assume that the range [f.tellg(), f.tellg()+offset] is included in a method and that f.tellg() is a valid CIL position
	// Only works with positive offset for now
	static bool IsCILOffsetValidFromHere(fstream& f, int offset);
};

struct ILInstruction {
	uint16_t opcode;
	uint64_t param;
	int32_t* switchpos;
	
	ILInstruction() : switchpos(NULL), ilcode(NULL) {}
	ILInstruction(uint16_t o) : opcode(o), param(0), switchpos(NULL), ilcode(NULL) {}
	ILInstruction(uint16_t o, uint64_t p) : opcode(o), param(p), switchpos(NULL), ilcode(NULL) {}
	ILInstruction(uint16_t o, uint64_t p, int32_t* sw) : opcode(o), param(p), switchpos(sw), ilcode(NULL) {}
	~ILInstruction() { if (switchpos) delete[] switchpos; }
	
	void Read(fstream& f);
	bool operator==(ILInstruction& ilcmp);
	unsigned int GetLength();
	ILCode& GetCode();
	void AppendToBuffer(uint8_t* buffer);
	
private:
	ILCode* ilcode;
};

struct CILDataSet {
	DllMetaData* data; // readonly
	unsigned int rawmodifamount;
	unsigned int macromodifamount;
	DllMetaDataModification* rawmodif;
	CILMacro* macromodif;
	
	void Init(DllMetaData* d);
	int GetEnabledMacroIndex(uint32_t macroid);
	// return a new'ed array to be deleted[]
	uint8_t* GetModifiedMethodRaw(unsigned int tid, unsigned int mid, uint32_t* length);
	// Assume it can be added (it's not present already and its methods have no rawmodif)
	void AddMacroModif(uint32_t macroid);
	void RemoveMacroModif(uint32_t macroid);
	// return true if the new modification is added and false if an old one is replaced
	bool UpdateWithNewModification(DllMetaDataModification& newmodif, unsigned int checkmacrostartindex = 0);
	
	// return value is int[4] to be deleted
	// {Number of rawmodif methods not found, Number of rawmodif methods not matching,
	//  Number of unknown macros, Number of un-activable macro}
	int* LoadHWS(fstream &ffhws);
	void WriteHWS(fstream &ffhws);
	void GenerateCSharp(vector<string>& buffer);
};

#endif
