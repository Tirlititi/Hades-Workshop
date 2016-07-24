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

#define DLL_TABLE_HEAP_COUNT 45
#define DLL_TABLE_CODED_INDEX 13

#include "File_IOCommon.h"

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
	uint32_t* cil_fieldrva_virtual_address;
	uint32_t* cil_fieldrva_field_id;
	uint32_t* cil_memberref_class;
	uint32_t* cil_memberref_name_id;
	uint32_t* cil_memberref_signature;
	
	int Load(fstream& f);
	int GetMethodIndex(const char* tname, const char* mname, const char* namesp = NULL);
	uint32_t GetMethodOffset(unsigned int methid);
	uint32_t GetMethodRange(unsigned int methid);
	string GetMethodName(unsigned int methid, bool showtype = true);
	uint32_t GetStaticFieldOffset(unsigned int staticfieldid);
	uint32_t GetTypeIdentifier(const char* tname);
	uint32_t GetMemberIdentifier(const char* mname, const char* tname);
	
	int Duplicate(fstream& fbase, fstream& fdest, unsigned int modifamount, DllMetaDataModification* modif);

private:
	int coded_index_size[DLL_TABLE_CODED_INDEX];
	
	void ComputeTableInformations(uint32_t baseoffset);
	int GetSectionAtVirtualAddress(uint32_t address);
	uint32_t GetVirtualAddressOffset(uint32_t address);
	uint32_t GetStreamOffset(unsigned int stream);
	int GetCodedIndexSize(unsigned int codedindex);
	int GetTableIndexSize(unsigned int tableindex);
	int GetMethodAtOffset(uint32_t offset);
	string GetNameAtPos(uint32_t stroff);
	bool IsNameMatching(uint32_t stroff, const char* match);
	
	uint32_t ApplyOffsetModification(uint32_t offset, unsigned int modifamount, DllMetaDataModification* modif, int32_t baseoffset = 0);
	uint32_t ApplyVirtualOffsetModification(uint32_t baserva, uint32_t newsecoff, unsigned int modifamount, DllMetaDataModification* modif);
	uint32_t ApplySizeModification(uint32_t size, unsigned int modifamount, DllMetaDataModification* modif);
	uint32_t WriteMethodSizeModification(fstream& fbase, fstream& fdest, uint32_t methid, unsigned int modifamount, DllMetaDataModification* modif); // Return base size
};

struct DllMetaDataModification {
	uint32_t position;
	uint32_t base_length;
	uint32_t new_length;
	uint8_t* value;
	
	int method_id;
	
	int32_t GetSizeModification(unsigned int modifamount, DllMetaDataModification* modif);
};

struct DllMethodInfo {
	uint32_t JumpToCode(fstream& f);
};

struct ILInstruction {
	uint8_t opcode;
	uint32_t param;
};

// sizes in bits ; assuming the sum is a multiple of 8
uint8_t* ConvertILScriptToRawData_Object(fstream& f, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[] = NULL);
int64_t** ConvertILScriptToRawData_Array2(fstream& f, unsigned int objamount, unsigned int arrayamount);

DllMetaDataModification ModifyILScript_Object(fstream& f, uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[] = NULL);
DllMetaDataModification ModifyILScript_Array2(fstream& f, uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int arrayamount);

uint32_t ILScriptJumpToInstructions(fstream& f, unsigned int amount, ILInstruction* inst);

#endif
