#include "DllEditor.h"

#include <algorithm>
#include <sstream>
#include <cstring>
#include "Configuration.h"

#define CIL_HWS_VERSION 1

//==================================================//
//                   DLL HEADER                     //
//==================================================//

// Read
enum DllCodedIndex {
	CodedIndex_TypeDefOrRef,
	CodedIndex_HasConstant,
	CodedIndex_HasCustomAttribute,
	CodedIndex_HasFieldMarshal,
	CodedIndex_HasDeclSecurity,
	CodedIndex_MemberRefParent,
	CodedIndex_HasSemantics,
	CodedIndex_MethodDefOrRef,
	CodedIndex_MemberForwarded,
	CodedIndex_Implementation,
	CodedIndex_CustomAttributeType,
	CodedIndex_ResolutionScope,
	CodedIndex_TypeOrMethodDef
};

enum DllTableType {
	TableType_Module = 0x00,
	TableType_TypeRef,
	TableType_TypeDef,
	TableType_FieldPtr,
	TableType_Field,
	TableType_MethodPtr,
	TableType_Method,
	TableType_ParamPtr,
	TableType_Param,
	TableType_InterfaceImpl,
	TableType_MemberRef,
	TableType_Constant,
	TableType_CustomAttribute,
	TableType_FieldMarshal,
	TableType_DeclSecurity,
	TableType_ClassLayout,
	TableType_FieldLayout,
	TableType_StandAloneSig,
	TableType_EventMap,
	TableType_EventPtr,
	TableType_Event,
	TableType_PropertyMap,
	TableType_PropertyPtr,
	TableType_Property,
	TableType_MethodSemantics,
	TableType_MethodImpl,
	TableType_ModuleRef,
	TableType_TypeSpec,
	TableType_ImplMap,
	TableType_FieldRVA,
	TableType_EncLog,
	TableType_EncMap,
	TableType_Assembly,
	TableType_AssemblyProcessor,
	TableType_AssemblyOS,
	TableType_AssemblyRef,
	TableType_AssemblyRefProcessor,
	TableType_AssemblyRefOS,
	TableType_File,
	TableType_ExportedType,
	TableType_ManifestResource,
	TableType_NestedClass,
	TableType_GenericParam,
	TableType_MethodSpec,
	TableType_GenericParamConstraint,
	TableType_String = 0x70
};

enum DllElementType {
	ElementType_None = 0x00,
	ElementType_Void = 0x01,
	ElementType_Boolean = 0x02,
	ElementType_Char = 0x03,
	ElementType_I1 = 0x04,
	ElementType_U1 = 0x05,
	ElementType_I2 = 0x06,
	ElementType_U2 = 0x07,
	ElementType_I4 = 0x08,
	ElementType_U4 = 0x09,
	ElementType_I8 = 0x0a,
	ElementType_U8 = 0x0b,
	ElementType_R4 = 0x0c,
	ElementType_R8 = 0x0d,
	ElementType_String = 0x0e,
	ElementType_Ptr = 0x0f,   // Followed by <type> token
	ElementType_ByRef = 0x10,   // Followed by <type> token
	ElementType_ValueType = 0x11,   // Followed by <type> token
	ElementType_Class = 0x12,   // Followed by <type> token
	ElementType_Var = 0x13,   // Followed by generic parameter number
	ElementType_Array = 0x14,   // <type> <rank> <boundsCount> <bound1>  <loCount> <lo1>
	ElementType_GenericInst = 0x15,   // <type> <type-arg-count> <type-1> ... <type-n> */
	ElementType_TypedByRef = 0x16,
	ElementType_I = 0x18,   // System.IntPtr
	ElementType_U = 0x19,   // System.UIntPtr
	ElementType_FnPtr = 0x1b,   // Followed by full method signature
	ElementType_Object = 0x1c,   // System.Object
	ElementType_SzArray = 0x1d,   // Single-dim array with 0 lower bound
	ElementType_MVar = 0x1e,   // Followed by generic parameter number
	ElementType_CModReqD = 0x1f,   // Required modifier : followed by a TypeDef or TypeRef token
	ElementType_CModOpt = 0x20,   // Optional modifier : followed by a TypeDef or TypeRef token
	ElementType_Internal = 0x21,   // Implemented within the CLI
	ElementType_Modifier = 0x40,   // Or'd with following element types
	ElementType_Sentinel = 0x41,   // Sentinel for varargs method signature
	ElementType_Pinned = 0x45,   // Denotes a local variable that points at a pinned object
	// special undocumented constants
	ElementType_Type = 0x50,
	ElementType_Boxed = 0x51,
	ElementType_Enum = 0x55
};

uint32_t ReadShortOrLong(fstream& f, int datasize) {
	if (datasize==2)
		return ReadShort(f);
	return ReadLong(f);
}

uint32_t GetTypeDefOrRef(uint32_t data) {
	uint32_t rid = data >> 2;
	switch (data & 3) {
		case 0: return rid | (TableType_TypeDef << 24);
		case 1: return rid | (TableType_TypeRef << 24);
		case 2: return rid | (TableType_TypeSpec << 24);
	}
	return 0;
}

uint32_t GetMethodDefOrRef(uint32_t data) {
	uint32_t rid = data >> 1;
	switch (data & 1) {
		case 0: return rid | (TableType_Method << 24);
		case 1: return rid | (TableType_MemberRef << 24);
	}
	return 0;
}

uint32_t GetTypeOrMethodDef(uint32_t data) {
	uint32_t rid = data >> 1;
	switch (data & 1) {
		case 0: return rid | (TableType_TypeDef << 24);
		case 1: return rid | (TableType_Method << 24);
	}
	return 0;
}

int DllMetaData::Load(const char* fname) {
	unsigned int i,j;
	size_t prevpos;
	uint8_t tmp8;
	uint32_t tmp32;
	char strbuffer[256];
	if (dll_file.is_open())
		dll_file.close();
	dll_file.open(fname,ios::in|ios::binary);
	if (!dll_file.is_open())
		return 1;
	if (ReadShort(dll_file)!=0x5A4D)
		return 1;
	dll_file.seekg(0x3A,ios::cur);
	pe_header_offset = ReadLong(dll_file);
	dll_file.seekg(pe_header_offset);
	if (ReadLong(dll_file)!=0x4550)
		return 1;
	architecture = ReadShort(dll_file);
	section_amount = ReadShort(dll_file);
	dll_file.seekg(14,ios::cur);
	characteristics = ReadShort(dll_file);
	pe_type = ReadShort(dll_file);
	dll_file.seekg(0x42,ios::cur);
	subsystem = ReadShort(dll_file);
	dll_characteristics = ReadShort(dll_file);
	dll_file.seekg(pe_type==0x20B ? 0x98 : 0x88,ios::cur);
	cli_directory.virtual_address = ReadLong(dll_file);
	cli_directory.size = ReadLong(dll_file);
	dll_file.seekg(8,ios::cur);
	section_name = new string[section_amount];
	section_virtual_size = new uint32_t[section_amount];
	section_virtual_offset = new uint32_t[section_amount];
	section_size = new uint32_t[section_amount];
	section_offset = new uint32_t[section_amount];
	for (i=0;i<section_amount;i++) {
		section_name[i] = "";
		dll_file.read(strbuffer,8);
		for (j=0;j<8 && strbuffer[j]!=0;j++)
			section_name[i].push_back(strbuffer[j]);
		section_virtual_size[i] = ReadLong(dll_file);
		section_virtual_offset[i] = ReadLong(dll_file);
		section_size[i] = ReadLong(dll_file);
		section_offset[i] = ReadLong(dll_file);
		dll_file.seekg(0x10,ios::cur);
	}
	dll_file.seekg(GetVirtualAddressOffset(cli_directory.virtual_address));
	dll_file.seekg(8,ios::cur);
	meta_data_directory.virtual_address = ReadLong(dll_file);
	meta_data_directory.size = ReadLong(dll_file);
	cli_attributes = ReadLong(dll_file);
	cli_entry_point_token = ReadLong(dll_file);
	resources_directory.virtual_address = ReadLong(dll_file);
	resources_directory.size = ReadLong(dll_file);
	strong_name_directory.virtual_address = ReadLong(dll_file);
	strong_name_directory.size = ReadLong(dll_file);
	meta_data_section = GetSectionAtVirtualAddress(meta_data_directory.virtual_address);
	dll_file.seekg(GetVirtualAddressOffset(meta_data_directory.virtual_address));
	if (ReadLong(dll_file)!=0x424a5342)
		return 1;
	dll_file.seekg(8,ios::cur);
	meta_data_version_name_size = ReadLong(dll_file);
	dll_file.seekg(meta_data_version_name_size,ios::cur);
	dll_file.seekg(2,ios::cur);
	meta_data_stream_amount = ReadShort(dll_file);
	stream_offset = new uint32_t[meta_data_stream_amount];
	stream_size = new uint32_t[meta_data_stream_amount];
	stream_name = new string[meta_data_stream_amount];
	stream_table_heap = -1;
	stream_string_heap = -1;
	stream_blob_heap = -1;
	stream_guid_heap = -1;
	stream_user_string_heap = -1;
	for (i=0;i<meta_data_stream_amount;i++) {
		stream_offset[i] = ReadLong(dll_file);
		stream_size[i] = ReadLong(dll_file);
		stream_name[i] = "";
		for (tmp8=dll_file.get();tmp8!=0;tmp8=dll_file.get())
			stream_name[i].push_back(tmp8);
		dll_file.seekg(GetAlignOffset(dll_file.tellg()),ios::cur);
		if (stream_name[i].compare("#~")==0)
			stream_table_heap = i;
		else if (stream_name[i].compare("#Strings")==0)
			stream_string_heap = i;
		else if (stream_name[i].compare("#Blob")==0)
			stream_blob_heap = i;
		else if (stream_name[i].compare("#GUID")==0)
			stream_guid_heap = i;
		else if (stream_name[i].compare("#US")==0)
			stream_user_string_heap = i;
	}
	if (stream_table_heap<0)
		return 1;
	if (stream_string_heap<0)
		return 1;
	dll_file.seekg(GetStreamOffset(stream_string_heap));
	string_heap_raw = new char[stream_size[stream_string_heap]];
	dll_file.read(string_heap_raw,stream_size[stream_string_heap]);
	dll_file.seekg(GetStreamOffset(stream_table_heap));
	dll_file.seekg(6,ios::cur);
	table_heap_sizes = dll_file.get();
	stream_string_heap_index_size = (table_heap_sizes & 1)>0 ? 4 : 2;
	stream_blob_heap_index_size = (table_heap_sizes & 4)>0 ? 4 : 2;
	stream_guid_heap_index_size = (table_heap_sizes & 2)>0 ? 4 : 2;
	for (i=0;i<DLL_TABLE_CODED_INDEX;i++)
		coded_index_size[i] = 0;
	dll_file.seekg(1,ios::cur);
	table_heap_valid = ReadLongLong(dll_file);
	dll_file.seekg(8,ios::cur);
	for (i=0;i<DLL_TABLE_HEAP_COUNT;i++)
		if ((table_heap_valid >> i) & 1)
			table[i].length = ReadLong(dll_file);
		else
			table[i].length = 0;
	ComputeTableInformations((uint32_t)dll_file.tellg()-section_offset[meta_data_section]);
	cil_module_generation = new uint16_t[table[TableType_Module].length];
	cil_module_name_id = new uint32_t[table[TableType_Module].length];
	cil_module_mv_id = new uint32_t[table[TableType_Module].length];
	cil_module_enc_id = new uint32_t[table[TableType_Module].length];
	cil_module_encbase_id = new uint32_t[table[TableType_Module].length];
	cil_typeref_resolution = new uint32_t[table[TableType_TypeRef].length];
	cil_typeref_name_id = new uint32_t[table[TableType_TypeRef].length];
	cil_typeref_namespace_id = new uint32_t[table[TableType_TypeRef].length];
	cil_type_attributes = new uint32_t[table[TableType_TypeDef].length];
	cil_type_name_id = new uint32_t[table[TableType_TypeDef].length];
	cil_type_namespace_id = new uint32_t[table[TableType_TypeDef].length];
	cil_type_basetype = new uint32_t[table[TableType_TypeDef].length];
	cil_type_field_id = new uint32_t[table[TableType_TypeDef].length];
	cil_type_method_id = new uint32_t[table[TableType_TypeDef].length];
	cil_field_flags = new uint16_t[table[TableType_Field].length];
	cil_field_name_id = new uint32_t[table[TableType_Field].length];
	cil_field_signature = new uint32_t[table[TableType_Field].length];
	cil_method_virtual_address = new uint32_t[table[TableType_Method].length];
	cil_method_implflags = new uint16_t[table[TableType_Method].length];
	cil_method_flags = new uint16_t[table[TableType_Method].length];
	cil_method_name_id = new uint32_t[table[TableType_Method].length];
	cil_method_signature = new uint32_t[table[TableType_Method].length];
	cil_method_param_id = new uint32_t[table[TableType_Method].length];
	cil_param_flags = new uint16_t[table[TableType_Param].length];
	cil_param_sequence = new uint16_t[table[TableType_Param].length];
	cil_param_name_id = new uint32_t[table[TableType_Param].length];
	cil_member_class = new uint32_t[table[TableType_MemberRef].length];
	cil_member_name_id = new uint32_t[table[TableType_MemberRef].length];
	cil_member_signature = new uint32_t[table[TableType_MemberRef].length];
	cil_member_string = new string[table[TableType_MemberRef].length];
	cil_moduleref_name_id = new uint32_t[table[TableType_ModuleRef].length];
	cil_typespec_signature = new uint32_t[table[TableType_TypeSpec].length];
	cil_fieldrva_virtual_address = new uint32_t[table[TableType_FieldRVA].length];
	cil_fieldrva_field_id = new uint32_t[table[TableType_FieldRVA].length];
	cil_genericparam_index = new uint16_t[table[TableType_GenericParam].length];
	cil_genericparam_flags = new uint16_t[table[TableType_GenericParam].length];
	cil_genericparam_owner = new uint32_t[table[TableType_GenericParam].length];
	cil_genericparam_name_id = new uint32_t[table[TableType_GenericParam].length];
	cil_methodspec_method = new uint32_t[table[TableType_MethodSpec].length];
	cil_methodspec_signature = new uint32_t[table[TableType_MethodSpec].length];
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_Module].offset);
	for (i=0;i<table[TableType_Module].length;i++) {
		cil_module_generation[i] = ReadShort(dll_file);
		cil_module_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_module_mv_id[i] = ReadShortOrLong(dll_file,stream_guid_heap_index_size);
		cil_module_enc_id[i] = ReadShortOrLong(dll_file,stream_guid_heap_index_size);
		cil_module_encbase_id[i] = ReadShortOrLong(dll_file,stream_guid_heap_index_size);
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_TypeRef].offset);
	for (i=0;i<table[TableType_TypeRef].length;i++) {
		cil_typeref_resolution[i] = ReadShortOrLong(dll_file,GetCodedIndexSize(CodedIndex_ResolutionScope));
		cil_typeref_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_typeref_namespace_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_TypeDef].offset);
	for (i=0;i<table[TableType_TypeDef].length;i++) {
		cil_type_attributes[i] = ReadLong(dll_file);
		cil_type_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_type_namespace_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_type_basetype[i] = ReadShortOrLong(dll_file,GetCodedIndexSize(CodedIndex_TypeDefOrRef));
		cil_type_field_id[i] = ReadShortOrLong(dll_file,GetTableIndexSize(TableType_Field));
		cil_type_method_id[i] = ReadShortOrLong(dll_file,GetTableIndexSize(TableType_Method));
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_Field].offset);
	for (i=0;i<table[TableType_Field].length;i++) {
		cil_field_flags[i] = ReadShort(dll_file);
		cil_field_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_field_signature[i] = ReadShortOrLong(dll_file,stream_blob_heap_index_size);
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_Method].offset);
	for (i=0;i<table[TableType_Method].length;i++) {
		cil_method_virtual_address[i] = ReadLong(dll_file);
		cil_method_implflags[i] = ReadShort(dll_file);
		cil_method_flags[i] = ReadShort(dll_file);
		cil_method_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_method_signature[i] = ReadShortOrLong(dll_file,stream_blob_heap_index_size);
		cil_method_param_id[i] = ReadShortOrLong(dll_file,GetTableIndexSize(TableType_Param));
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_Param].offset);
	for (i=0;i<table[TableType_Param].length;i++) {
		cil_param_flags[i] = ReadShort(dll_file);
		cil_param_sequence[i] = ReadShort(dll_file);
		cil_param_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_MemberRef].offset);
	for (i=0;i<table[TableType_MemberRef].length;i++) {
		cil_member_class[i] = ReadShortOrLong(dll_file,GetCodedIndexSize(CodedIndex_MemberRefParent));
		cil_member_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
		cil_member_signature[i] = ReadShortOrLong(dll_file,stream_blob_heap_index_size);
		cil_member_string[i] = "";
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_ModuleRef].offset);
	for (i=0;i<table[TableType_ModuleRef].length;i++)
		cil_moduleref_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_TypeSpec].offset);
	for (i=0;i<table[TableType_TypeSpec].length;i++)
		cil_typespec_signature[i] = ReadShortOrLong(dll_file,stream_blob_heap_index_size);
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_FieldRVA].offset);
	for (i=0;i<table[TableType_FieldRVA].length;i++) {
		cil_fieldrva_virtual_address[i] = ReadLong(dll_file);
		cil_fieldrva_field_id[i] = ReadShortOrLong(dll_file,GetTableIndexSize(TableType_Field));
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_GenericParam].offset);
	for (i=0;i<table[TableType_GenericParam].length;i++) {
		cil_genericparam_index[i] = ReadShort(dll_file);
		cil_genericparam_flags[i] = ReadShort(dll_file);
		cil_genericparam_owner[i] = ReadShortOrLong(dll_file,GetCodedIndexSize(CodedIndex_TypeOrMethodDef));
		cil_genericparam_name_id[i] = ReadShortOrLong(dll_file,stream_string_heap_index_size);
	}
	dll_file.seekg(section_offset[meta_data_section]+table[TableType_MethodSpec].offset);
	for (i=0;i<table[TableType_MethodSpec].length;i++) {
		cil_methodspec_method[i] = ReadShortOrLong(dll_file,GetCodedIndexSize(CodedIndex_MethodDefOrRef));
		cil_methodspec_signature[i] = ReadShortOrLong(dll_file,stream_blob_heap_index_size);
	}
	user_string_amount = 0;
	prevpos = GetStreamOffset(stream_user_string_heap);
	dll_file.seekg(prevpos+1);
	while (dll_file.tellg()<prevpos+stream_size[stream_user_string_heap]) {
		tmp32 = ReadCompressedUInt32();
		dll_file.seekg(tmp32,ios::cur);
		user_string_amount++;
	}
	user_string_offset = new uint32_t[user_string_amount];
	dll_file.seekg(prevpos+1);
	for (i=0;i<user_string_amount;i++) {
		user_string_offset[i] = (unsigned int)dll_file.tellg()-prevpos;
		tmp32 = ReadCompressedUInt32();
		dll_file.seekg(tmp32,ios::cur);
	}
	return 0;
}

int DllMetaData::GetMethodIdByName(const char* tname, const char* mname, const char* namesp) {
	int i,j,methamount;
	for (i=0;i<table[TableType_TypeDef].length;i++)
		if (IsNameMatching(cil_type_namespace_id[i],namesp) && IsNameMatching(cil_type_name_id[i],tname)) {
			if (cil_type_method_id[i]==0)
				return -1;
			methamount = GetTypeMethodAmount(i)-1;
			for (j=-1;j<methamount;j++)
				if (IsNameMatching(cil_method_name_id[cil_type_method_id[i]+j],mname))
					return cil_type_method_id[i]+j;
			return -1;
		}
	return -1;
}

uint32_t DllMetaData::GetMethodOffset(unsigned int methid) {
	if (cil_method_virtual_address[methid]==0)
		return 0;
	return GetVirtualAddressOffset(cil_method_virtual_address[methid]);
}

uint32_t DllMetaData::GetMethodRange(unsigned int methid) {
	if (cil_method_virtual_address[methid]==0)
		return 0;
	unsigned int i;
	uint32_t methend = 0;
	for (i=1;methend==0;i++)
		if (methid+i>=table[TableType_Method].length)
			methend = meta_data_directory.virtual_address;
		else if (cil_method_virtual_address[methid+i]!=0)
			methend = cil_method_virtual_address[methid+i];
	return methend-cil_method_virtual_address[methid];
}

int DllMetaData::GetStaticFieldIdFromToken(uint32_t fieldtoken) {
	if ((fieldtoken >> 24)!=TableType_Field)
		return -1;
	unsigned int i;
	fieldtoken &= 0xFFFFFF;
	for (i=0;i<table[TableType_FieldRVA].length;i++)
		if (cil_fieldrva_field_id[i]==fieldtoken)
			return i;
	return -1;
}

uint32_t DllMetaData::GetStaticFieldOffset(unsigned int staticfieldid) {
	return GetVirtualAddressOffset(cil_fieldrva_virtual_address[staticfieldid]);
}

uint32_t DllMetaData::GetTypeTokenIdentifier(const char* tname, const char* namesp) {
	unsigned int i;
	for (i=0;i<table[TableType_TypeDef].length;i++)
		if (IsNameMatching(cil_type_name_id[i],tname) && IsNameMatching(cil_type_namespace_id[i],namesp))
			return (i+1) | (TableType_TypeDef << 24);
	return 0;
}

uint32_t DllMetaData::GetTypeRefTokenIdentifier(const char* tname, const char* namesp) {
	unsigned int i;
	for (i=0;i<table[TableType_TypeRef].length;i++)
		if (IsNameMatching(cil_typeref_name_id[i],tname) && IsNameMatching(cil_typeref_namespace_id[i],namesp))
			return (i+1) | (TableType_TypeRef << 24);
	return 0;
}

uint32_t DllMetaData::GetFieldTokenIdentifier(const char* tname, const char* fname) {
	unsigned int i,j,nexttf;
	for (i=0;i<table[TableType_TypeDef].length;i++)
		if (IsNameMatching(cil_type_name_id[i],tname) && cil_type_field_id[i]!=0) {
			nexttf = 0;
			for (j=i+1;j<table[TableType_TypeDef].length && nexttf==0;j++)
				nexttf = cil_type_field_id[j];
			if (nexttf==0)
				nexttf = table[TableType_Field].length;
			else
				nexttf--;
			for (j=cil_type_field_id[i]-1;j<nexttf;j++)
				if (IsNameMatching(cil_field_name_id[j],fname))
					return (j+1) | (TableType_Field << 24);
		}
	return 0;
}

uint32_t DllMetaData::GetMethodTokenIdentifier(const char* tname, const char* mname, const char* namesp) {
	int absid = GetMethodIdByName(tname,mname,namesp);
	if (absid>=0)
		return (absid+1) | (TableType_Method << 24);
	return 0;
}

uint32_t DllMetaData::GetMemberTokenIdentifier(const char* descname) {
	unsigned int i;
	for (i=0;i<table[TableType_MemberRef].length;i++)
		if (GetMemberName(i,true).compare(descname)==0)
			return (i+1) | (TableType_MemberRef << 24);
	return 0;
}

uint32_t DllMetaData::GetStringToken(wstring value) {
	unsigned int i;
	size_t fpos = dll_file.tellg();
	for (i=0;i<user_string_amount;i++)
		if (value.compare(GetUserStringAtPos(user_string_offset[i],false))==0) {
			dll_file.seekg(fpos);
			return user_string_offset[i] | (TableType_String << 24);
		}
	dll_file.seekg(fpos);
	return 0;
}

unsigned int DllMetaData::GetTypeAmount() {
	return table[TableType_TypeDef].length;
}

unsigned int DllMetaData::GetTypeMethodAmount(unsigned int tid) {
	unsigned int nexttype = tid+1;
	if (cil_type_method_id[tid]==0)
		return 0;
	while (nexttype<table[TableType_TypeDef].length && cil_type_method_id[nexttype]==0)
		nexttype++;
	if (nexttype==table[TableType_TypeDef].length)
		return table[TableType_Method].length-(cil_type_method_id[tid]-1);
	return cil_type_method_id[nexttype]-cil_type_method_id[tid];
}

void DllMetaData::GetMethodRelativeId(unsigned int absmethid, unsigned int* tid, unsigned int* mid) {
	int i;
	for (i=table[TableType_TypeDef].length-1;i>=0;i--)
		if (cil_type_method_id[i]!=0 && absmethid+1>=cil_type_method_id[i]) {
			*tid = i;
			*mid = absmethid+1-cil_type_method_id[i];
			return;
		}
}

void DllMetaData::GetMethodSignatureString(uint8_t callingconvention, string* returntype, string* parameters) {
	unsigned int i;
	uint32_t arity;
	if (callingconvention & 0x10)
		ReadCompressedUInt32();
	arity = ReadCompressedUInt32();
	*returntype = GetSignatureString();
	*parameters = "(";
	for (i=0;i<arity;i++) {
		*parameters += i>0 ? ", " : " ";
		*parameters += GetSignatureString();
	}
	*parameters += arity>0 ? " )" : ")";
}

string DllMetaData::GetSignatureString(uint32_t signpos) {
	uint32_t sigsize,rid,arity,token = 0;
	uint8_t elemtype,toktype;
	unsigned int i;
	string tmpstr,res = "";
	if (signpos>0) {
		dll_file.seekg(GetStreamOffset(stream_blob_heap)+signpos);
		sigsize = ReadCompressedUInt32();
	}
	elemtype = dll_file.get();
	switch (elemtype) {
	case ElementType_ValueType:
		token = GetTypeDefOrRef(ReadCompressedUInt32());
		res = "valuetype ";
		break;
	case ElementType_Class:
		token = GetTypeDefOrRef(ReadCompressedUInt32());
		res = "class ";
		break;
	case ElementType_Ptr:
		return GetSignatureString()+"*";
	case ElementType_FnPtr: {
		string rtype, paramstr;
		GetMethodSignatureString(dll_file.get(),&rtype,&paramstr);
		return "method "+rtype+" *"+paramstr;
	}
	case ElementType_ByRef:
		return GetSignatureString()+"&";
	case ElementType_Pinned:
		return GetSignatureString();
	case ElementType_SzArray:
		return GetSignatureString()+"[]";
	case ElementType_Array: {
		stringstream ress;
		ress << GetSignatureString();
		arity = ReadCompressedUInt32();
		uint32_t sizeamount = ReadCompressedUInt32();
		uint32_t* sizes = new uint32_t[sizeamount];
		for (i=0;i<sizeamount;i++)
			sizes[i] = ReadCompressedUInt32();
		uint32_t lowerboundamount = ReadCompressedUInt32();
		uint32_t* lowerbound = new uint32_t[lowerboundamount];
		for (i=0;i<lowerboundamount;i++)
			lowerbound[i] = ReadCompressedUInt32();
		ress << "[";
		for (i=0;i<arity;i++) {
			if (i>0)
				ress << ",";
			if (i<lowerboundamount)
				ress << (unsigned int)lowerbound[i];
			if (i<lowerboundamount || i<sizeamount)
				ress << "...";
			if (i<sizeamount)
				ress << (unsigned int)(i<lowerboundamount ? lowerbound[i]+sizes[i] : sizes[i]);
		}
		ress << "]";
		return ress.str();
	}
	case ElementType_CModOpt:
		tmpstr = GetTokenDescription(GetTypeDefOrRef(ReadCompressedUInt32()));
		res = GetSignatureString()+" modopt(";
		res += tmpstr;
		res += ")";
		return res;
	case ElementType_CModReqD:
		tmpstr = GetTokenDescription(GetTypeDefOrRef(ReadCompressedUInt32()));
		res = GetSignatureString()+" modreq(";
		res += tmpstr;
		res += ")";
		return res;
	case ElementType_Sentinel:
		return "..., "+GetSignatureString();
	case ElementType_Var: {
		stringstream ress;
		ress << "!" << (unsigned int)ReadCompressedUInt32();
		return ress.str();
	}
	case ElementType_MVar: {
		stringstream ress;
		ress << "!!" << (unsigned int)ReadCompressedUInt32();
		return ress.str();
	}
	case ElementType_GenericInst:
		if (dll_file.get()==ElementType_ValueType)
			res += "valuetype ";
		res += GetTokenDescription(GetTypeDefOrRef(ReadCompressedUInt32()));
		arity = ReadCompressedUInt32();
		res += "<";
		for (i=0;i<arity;i++) {
			if (i>0)
				res += ",";
			res += GetSignatureString();
		}
		res += ">";
		return res;
	default:
		return GetPrimitiveTypeName(elemtype);
	}
	return res+GetTokenDescription(token);
}

string DllMetaData::GetPrimitiveTypeName(uint8_t elemtype) {
	switch (elemtype) {
		case ElementType_Boolean: return "bool";
		case ElementType_Char: return "char";
		case ElementType_I1: return "sbyte";
		case ElementType_U1: return "byte";
		case ElementType_I2: return "short";
		case ElementType_U2: return "ushort";
		case ElementType_I4: return "int";
		case ElementType_U4: return "uint";
		case ElementType_I8: return "long";
		case ElementType_U8: return "ulong";
		case ElementType_R4: return "float";
		case ElementType_R8: return "double";
		case ElementType_String: return "string";
		case ElementType_Object: return "object";
		case ElementType_Void: return "void";
		case ElementType_TypedByRef: return "typedreference";
		case ElementType_I: return "intptr";
		case ElementType_U: return "uintptr";
	}
	return "";
}

string DllMetaData::GetTypeName(unsigned int tid, bool shownamespace) {
	if (shownamespace && cil_type_namespace_id[tid]>0)
		return GetNameAtPos(cil_type_namespace_id[tid])+"."+GetNameAtPos(cil_type_name_id[tid]);
	return GetNameAtPos(cil_type_name_id[tid]);
}

string DllMetaData::GetTypeRefName(unsigned int tid, bool shownamespace) {
	if (shownamespace && cil_typeref_namespace_id[tid]>0)
		return GetNameAtPos(cil_typeref_namespace_id[tid])+"."+GetNameAtPos(cil_typeref_name_id[tid]);
	return GetNameAtPos(cil_typeref_name_id[tid]);
}

string DllMetaData::GetTypeSpecName(unsigned int tid) {
	return GetSignatureString(cil_typespec_signature[tid]);
}

string DllMetaData::GetMethodName(unsigned int tid, unsigned int mid) {
	return GetNameAtPos(cil_method_name_id[cil_type_method_id[tid]-1+mid]);
}

string DllMetaData::GetMethodNameById(unsigned int absmethid, bool showtype, bool shownamespace) {
	string res = "";
	if (showtype) {
		unsigned int tid, mid;
		GetMethodRelativeId(absmethid,&tid,&mid);
		res += GetTypeName(tid,shownamespace)+"::";
	}
	res += GetNameAtPos(cil_method_name_id[absmethid]);
	return res;
}

string DllMetaData::GetMethodSpecName(unsigned int mid) {
	return GetTokenDescription(GetMethodDefOrRef(cil_methodspec_method[mid]),true);
}

string DllMetaData::GetModuleName(unsigned int modid) {
	return GetNameAtPos(cil_module_name_id[modid]);
}

string DllMetaData::GetModuleRefName(unsigned int modid) {
	return GetNameAtPos(cil_moduleref_name_id[modid]);
}

string DllMetaData::GetFieldName(unsigned int fid, bool showtype) {
	string res = "";
	int i;
	if (showtype) {
		for (i=table[TableType_TypeDef].length-1;i>=0;i--)
			if (cil_type_field_id[i]!=0 && fid+1>=cil_type_field_id[i]) {
				if (cil_type_namespace_id[i]>0)
					res += GetNameAtPos(cil_type_namespace_id[i])+".";
				res += GetNameAtPos(cil_type_name_id[i])+"::";
				break;
			}
	}
	res += GetNameAtPos(cil_field_name_id[fid]);
	return res;
}

string DllMetaData::GetParamName(unsigned int pid) {
	return GetNameAtPos(cil_param_name_id[pid]);
}

string DllMetaData::GetMemberName(unsigned int mid, bool showclass) {
	if (showclass) {
		if (cil_member_string[mid].length()>0)
			return cil_member_string[mid];
		uint32_t rid = (cil_member_class[mid] >> 3);
		uint8_t toktype = (cil_member_class[mid] & 7);
		if (rid>0) {
			string rtype, paramstr;
			uint32_t sigsize;
			uint8_t flags;
			size_t dllpos;
			rid--;
			switch (toktype) {
			case 0:
			case 1:
			case 3:
			case 4:
				dllpos = dll_file.tellg();
				dll_file.seekg(GetStreamOffset(stream_blob_heap)+cil_member_signature[mid]);
				sigsize = ReadCompressedUInt32();
				flags = dll_file.get();
				if (flags==6) {
					cil_member_string[mid] = GetSignatureString();
				} else {
					GetMethodSignatureString(flags,&rtype,&paramstr);
					cil_member_string[mid] = rtype;
				}
				cil_member_string[mid] += " ";
				if (toktype==0)
					cil_member_string[mid] += GetTypeName(rid,true)+"::"+GetNameAtPos(cil_member_name_id[mid]);
				else if (toktype==1)
					cil_member_string[mid] += GetTypeRefName(rid,true)+"::"+GetNameAtPos(cil_member_name_id[mid]);
				else if (toktype==3)
					cil_member_string[mid] += GetMethodNameById(rid,true,true);
				else if (toktype==4)
					cil_member_string[mid] += GetTypeSpecName(rid)+"::"+GetNameAtPos(cil_member_name_id[mid]);
				if (flags!=6)
					cil_member_string[mid] += paramstr;
				dll_file.seekg(dllpos);
				return cil_member_string[mid];
			}
		}
	}
	return GetNameAtPos(cil_member_name_id[mid]);
}

string DllMetaData::GetNameAtPos(uint32_t stroff) {
	if (stroff==0)
		return "";
	string res = "";
	unsigned int i;
	for (i=0;string_heap_raw[stroff+i]!=0;i++)
		res += string_heap_raw[stroff+i];
	return res;
}

wstring DllMetaData::GetUserStringAtPos(uint32_t stroff, bool conservedllfilepos) {
	size_t fpos = dll_file.tellg();
	unsigned int i,strsize;
	wstring res = L"";
	dll_file.seekg(GetStreamOffset(stream_user_string_heap)+stroff);
	strsize = ReadCompressedUInt32() & ~1;
	for (i=0;i<strsize;i+=2)
		res += (wchar_t)ReadShort(dll_file);
	if (conservedllfilepos)
		dll_file.seekg(fpos);
	return res;
}

string DllMetaData::GetTokenDescription(uint32_t token, bool fulldesc) {
	uint8_t toktype = token >> 24;
	token &= 0xFFFFFF;
	switch (toktype) {
	case TableType_Module:
		if (token==0)
			return "Null(Module)";
		return GetModuleName(token-1);
	case TableType_TypeRef:
		if (token==0)
			return "Null(TypeRef)";
		return GetTypeRefName(token-1,fulldesc);
	case TableType_TypeDef:
		if (token==0)
			return "Null(TypeDef)";
		return GetTypeName(token-1,fulldesc);
	case TableType_Field:
		if (token==0)
			return "Null(Field)";
		return GetFieldName(token-1,true);
	case TableType_Method:
		if (token==0)
			return "Null(Method)";
		return GetMethodNameById(token-1,fulldesc);
	case TableType_Param:
		if (token==0)
			return "Null(Param)";
		return GetParamName(token-1);
	case TableType_MemberRef:
		if (token==0)
			return "Null(Member)";
		return GetMemberName(token-1,fulldesc);
	case TableType_ModuleRef:
		if (token==0)
			return "Null(ModuleRef)";
		return GetModuleRefName(token-1);
	case TableType_TypeSpec:
		if (token==0)
			return "Null(TypeSpec)";
		return GetTypeSpecName(token-1);
	case TableType_MethodSpec:
		if (token==0)
			return "Null(MethodSpec)";
		return GetMethodSpecName(token-1);
	}
	return "Unknown";
}

wstring DllMetaData::GetStringTokenDescription(uint32_t token) {
	uint8_t toktype = token >> 24;
	token &= 0xFFFFFF;
	if (toktype!=TableType_String)
		return L"[INVALID STRING]";
	return GetUserStringAtPos(token);
}

void DllMetaData::ComputeTableInformations(uint32_t baseoffset) {
	uint32_t size, offset = baseoffset;
	unsigned int i;
	for (i=0;i<DLL_TABLE_HEAP_COUNT;i++) {
		if (((table_heap_valid >> i) & 1)==0)
			continue;
		switch (i) {
		case TableType_Module:
			size = 2	// Generation
				+ stream_string_heap_index_size	// Name
				+ (stream_guid_heap_index_size * 3);	// Mvid, EncId, EncBaseId
			break;
		case TableType_TypeRef:
			size = GetCodedIndexSize(CodedIndex_ResolutionScope)	// ResolutionScope
				+ (stream_string_heap_index_size * 2);	// Name, Namespace
			break;
		case TableType_TypeDef:
			size = 4	// Flags
				+ (stream_string_heap_index_size * 2)	// Name, Namespace
				+ GetCodedIndexSize(CodedIndex_TypeDefOrRef)	// BaseType
				+ GetTableIndexSize(TableType_Field)	// FieldList
				+ GetTableIndexSize(TableType_Method);	// MethodList
			break;
		case TableType_FieldPtr:
			size = GetTableIndexSize(TableType_Field);	// Field
			break;
		case TableType_Field:
			size = 2	// Flags
				+ stream_string_heap_index_size	// Name
				+ stream_blob_heap_index_size;	// Signature
			break;
		case TableType_MethodPtr:
			size = GetTableIndexSize(TableType_Method);	// Method
			break;
		case TableType_Method:
			size = 8	// Rva 4, ImplFlags 2, Flags 2
				+ stream_string_heap_index_size	// Name
				+ stream_blob_heap_index_size	// Signature
				+ GetTableIndexSize(TableType_Param); // ParamList
			break;
		case TableType_ParamPtr:
			size = GetTableIndexSize(TableType_Param); // Param
			break;
		case TableType_Param:
			size = 4	// Flags 2, Sequence 2
				+ stream_string_heap_index_size;	// Name
			break;
		case TableType_InterfaceImpl:
			size = GetTableIndexSize(TableType_TypeDef)	// Class
				+ GetCodedIndexSize(CodedIndex_TypeDefOrRef);	// Interface
			break;
		case TableType_MemberRef:
			size = GetCodedIndexSize(CodedIndex_MemberRefParent)	// Class
				+ stream_string_heap_index_size	// Name
				+ stream_blob_heap_index_size;	// Signature
			break;
		case TableType_Constant:
			size = 2	// Type
				+ GetCodedIndexSize(CodedIndex_HasConstant)	// Parent
				+ stream_blob_heap_index_size;	// Value
			break;
		case TableType_CustomAttribute:
			size = GetCodedIndexSize(CodedIndex_HasCustomAttribute)	// Parent
				+ GetCodedIndexSize(CodedIndex_CustomAttributeType)	// Type
				+ stream_blob_heap_index_size;	// Value
			break;
		case TableType_FieldMarshal:
			size = GetCodedIndexSize(CodedIndex_HasFieldMarshal)	// Parent
				+ stream_blob_heap_index_size;	// NativeType
			break;
		case TableType_DeclSecurity:
			size = 2	// Action
				+ GetCodedIndexSize(CodedIndex_HasDeclSecurity)	// Parent
				+ stream_blob_heap_index_size;	// PermissionSet
			break;
		case TableType_ClassLayout:
			size = 6	// PackingSize 2, ClassSize 4
				+ GetTableIndexSize(TableType_TypeDef);	// Parent
			break;
		case TableType_FieldLayout:
			size = 4	// Offset
				+ GetTableIndexSize(TableType_Field);	// Field
			break;
		case TableType_StandAloneSig:
			size = stream_blob_heap_index_size;	// Signature
			break;
		case TableType_EventMap:
			size = GetTableIndexSize(TableType_TypeDef)	// Parent
				+ GetTableIndexSize(TableType_Event);	// EventList
			break;
		case TableType_EventPtr:
			size = GetTableIndexSize(TableType_Event);	// Event
			break;
		case TableType_Event:
			size = 2	// Flags
				+ stream_string_heap_index_size // Name
				+ GetCodedIndexSize(CodedIndex_TypeDefOrRef);	// EventType
			break;
		case TableType_PropertyMap:
			size = GetTableIndexSize(TableType_TypeDef)	// Parent
				+ GetTableIndexSize(TableType_Property);	// PropertyList
			break;
		case TableType_PropertyPtr:
			size = GetTableIndexSize(TableType_Property);	// Property
			break;
		case TableType_Property:
			size = 2	// Flags
				+ stream_string_heap_index_size	// Name
				+ stream_blob_heap_index_size;	// Type
			break;
		case TableType_MethodSemantics:
			size = 2	// Semantics
				+ GetTableIndexSize(TableType_Method)	// Method
				+ GetCodedIndexSize(CodedIndex_HasSemantics);	// Association
			break;
		case TableType_MethodImpl:
			size = GetTableIndexSize(TableType_TypeDef)	// Class
				+ GetCodedIndexSize(CodedIndex_MethodDefOrRef)	// MethodBody
				+ GetCodedIndexSize(CodedIndex_MethodDefOrRef);	// MethodDeclaration
			break;
		case TableType_ModuleRef:
			size = stream_string_heap_index_size;	// Name
			break;
		case TableType_TypeSpec:
			size = stream_blob_heap_index_size;	// Signature
			break;
		case TableType_ImplMap:
			size = 2	// MappingFlags
				+ GetCodedIndexSize(CodedIndex_MemberForwarded)	// MemberForwarded
				+ stream_string_heap_index_size	// ImportName
				+ GetTableIndexSize(TableType_ModuleRef);	// ImportScope
			break;
		case TableType_FieldRVA:
			size = 4	// RVA
				+ GetTableIndexSize(TableType_Field);	// Field
			break;
		case TableType_EncLog:
			size = 8;
			break;
		case TableType_EncMap:
			size = 4;
			break;
		case TableType_Assembly:
			size = 16 // HashAlgId 4, Version 4 * 2, Flags 4
				+ stream_blob_heap_index_size	// PublicKey
				+ (stream_string_heap_index_size * 2);	// Name, Culture
			break;
		case TableType_AssemblyProcessor:
			size = 4;	// Processor
			break;
		case TableType_AssemblyOS:
			size = 12;	// Platform 4, Version 2 * 4
			break;
		case TableType_AssemblyRef:
			size = 12	// Version 2 * 4 + Flags 4
				+ (stream_blob_heap_index_size * 2)	// PublicKeyOrToken, HashValue
				+ (stream_string_heap_index_size * 2);	// Name, Culture
			break;
		case TableType_AssemblyRefProcessor:
			size = 4	// Processor
				+ GetTableIndexSize(TableType_AssemblyRef);	// AssemblyRef
			break;
		case TableType_AssemblyRefOS:
			size = 12	// Platform 4, Version 2 * 4
				+ GetTableIndexSize(TableType_AssemblyRef);	// AssemblyRef
			break;
		case TableType_File:
			size = 4	// Flags
				+ stream_string_heap_index_size	// Name
				+ stream_blob_heap_index_size;	// HashValue
			break;
		case TableType_ExportedType:
			size = 8	// Flags 4, TypeDefId 4
				+ (stream_string_heap_index_size * 2)	// Name, Namespace
				+ GetCodedIndexSize(CodedIndex_Implementation);	// Implementation
			break;
		case TableType_ManifestResource:
			size = 8	// Offset, Flags
				+ stream_string_heap_index_size	// Name
				+ GetCodedIndexSize(CodedIndex_Implementation);	// Implementation
			break;
		case TableType_NestedClass:
			size = GetTableIndexSize(TableType_TypeDef)	// NestedClass
				+ GetTableIndexSize(TableType_TypeDef);	// EnclosingClass
			break;
		case TableType_GenericParam:
			size = 4	// Number, Flags
				+ GetCodedIndexSize(CodedIndex_TypeOrMethodDef)	// Owner
				+ stream_string_heap_index_size;	// Name
			break;
		case TableType_MethodSpec:
			size = GetCodedIndexSize(CodedIndex_MethodDefOrRef)	// Method
				+ stream_blob_heap_index_size;	// Instantiation
			break;
		case TableType_GenericParamConstraint:
			size = GetTableIndexSize(TableType_GenericParam)	// Owner
				+ GetCodedIndexSize(CodedIndex_TypeDefOrRef);	// Constraint
			break;
		}
		table[i].row_size = size;
		table[i].offset = offset;
		offset += size*table[i].length;
	}
}

int DllMetaData::GetSectionAtVirtualAddress(uint32_t address) {
	unsigned int i;
	for (i=0;i<section_amount;i++)
		if (address>=section_virtual_offset[i] && address<section_virtual_offset[i]+section_size[i])
			return i;
	return -1;
}

uint32_t DllMetaData::GetVirtualAddressOffset(uint32_t address) {
	int section = GetSectionAtVirtualAddress(address);
	if (section<0)
		return 0;
	return section_offset[section]+address-section_virtual_offset[section];
}

uint32_t DllMetaData::GetStreamOffset(unsigned int stream) {
	return section_offset[meta_data_section]+stream_offset[stream]+meta_data_directory.virtual_address-section_virtual_offset[meta_data_section];
}

uint32_t DllMetaData::ReadCompressedUInt32() {
	uint32_t res;
	uint8_t c = dll_file.get();
	if ((c & 0x80)==0) {
		res = c;
	} else if ((c & 0x40)==0) {
		res = ((c & 0x7F) << 8);
		res |= dll_file.get();
	} else {
		res = ((c & 0x3F) << 24);
		res |= (dll_file.get() << 16);
		res |= (dll_file.get() << 8);
		res |= dll_file.get();
	}
	return res;
}

int DllMetaData::GetCodedIndexSize(unsigned int codedindex) {
	if (coded_index_size[codedindex]!=0)
		return coded_index_size[codedindex];
	uint32_t maxlen = 0;
	uint8_t relamount,bits;
	unsigned int* relevantid;
	unsigned int i;
	switch (codedindex) {
	case CodedIndex_TypeDefOrRef:
		bits = 2;
		relamount = 3;
		relevantid = new unsigned int[3]{ TableType_TypeDef, TableType_TypeRef, TableType_TypeSpec };
		break;
	case CodedIndex_HasConstant:
		bits = 2;
		relamount = 3;
		relevantid = new unsigned int[3]{ TableType_Field, TableType_Param, TableType_Property };
		break;
	case CodedIndex_HasCustomAttribute:
		bits = 5;
		relamount = 20;
		relevantid = new unsigned int[20]{ TableType_Method, TableType_Field, TableType_TypeRef, TableType_TypeDef,
			TableType_Param, TableType_InterfaceImpl, TableType_MemberRef, TableType_Module, TableType_DeclSecurity,
			TableType_Property, TableType_Event, TableType_StandAloneSig, TableType_ModuleRef, TableType_TypeSpec,
			TableType_Assembly, TableType_AssemblyRef, TableType_File, TableType_ExportedType,
			TableType_ManifestResource, TableType_GenericParam };
		break;
	case CodedIndex_HasFieldMarshal:
		bits = 1;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_Field, TableType_Param };
		break;
	case CodedIndex_HasDeclSecurity:
		bits = 2;
		relamount = 3;
		relevantid = new unsigned int[3]{ TableType_TypeDef, TableType_Method, TableType_Assembly };
		break;
	case CodedIndex_MemberRefParent:
		bits = 3;
		relamount = 5;
		relevantid = new unsigned int[5]{ TableType_TypeDef, TableType_TypeRef, TableType_ModuleRef, TableType_Method, TableType_TypeSpec };
		break;
	case CodedIndex_HasSemantics:
		bits = 1;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_Event, TableType_Property };
		break;
	case CodedIndex_MethodDefOrRef:
		bits = 1;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_Method, TableType_MemberRef };
		break;
	case CodedIndex_MemberForwarded:
		bits = 1;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_Field, TableType_Method };
		break;
	case CodedIndex_Implementation:
		bits = 2;
		relamount = 3;
		relevantid = new unsigned int[3]{ TableType_File, TableType_AssemblyRef, TableType_ExportedType };
		break;
	case CodedIndex_CustomAttributeType:
		bits = 3;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_Method, TableType_MemberRef };
		break;
	case CodedIndex_ResolutionScope:
		bits = 2;
		relamount = 4;
		relevantid = new unsigned int[4]{ TableType_Module, TableType_ModuleRef, TableType_AssemblyRef, TableType_TypeRef };
		break;
	case CodedIndex_TypeOrMethodDef:
		bits = 1;
		relamount = 2;
		relevantid = new unsigned int[2]{ TableType_TypeDef, TableType_Method };
		break;
	}
	for (i=0;i<relamount;i++)
		maxlen = max(table[relevantid[i]].length,maxlen);
	delete[] relevantid;
	coded_index_size[codedindex] = maxlen < (1 << (16-bits)) ? 2 : 4;
	return coded_index_size[codedindex];
}

int DllMetaData::GetTableIndexSize(unsigned int tableindex) {
	return table[tableindex].length<65536 ? 2 : 4;
}

int DllMetaData::GetMethodAtOffset(uint32_t offset) {
	int i,foundmethod = -1;
	uint32_t methbeg;
	size_t dllfpos = dll_file.tellg();
	for (i=table[TableType_Method].length-1;i>=0;i--) {
		methbeg = GetMethodOffset(i);
		if (methbeg>0 && methbeg<=offset) {
			DllMethodInfo methinfo;
			dll_file.seekg(methbeg);
			methinfo.ReadMethodInfo(dll_file);
			if (methinfo.code_abs_offset+methinfo.code_size>offset)
				foundmethod = i;
			break;
		}
	}
	dll_file.seekg(dllfpos);
	return foundmethod;
}

bool DllMetaData::IsNameMatching(uint32_t stroff, const char* match) {
	unsigned int i;
	if (match==NULL)
		return stroff==0;
	for (i=0;match[i]!=0;i++)
		if (string_heap_raw[stroff+i]!=match[i])
			return false;
	return string_heap_raw[stroff+i]==0;
}

// Write
int32_t DllMetaDataModification::GetSizeModification(unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo& methinfo) {
	int64_t basefulllen = base_length, newfulllen = new_length;
	int32_t res = newfulllen-basefulllen;
	int32_t tinyfatmodif = 0;
	unsigned int i;
	if (method_id<0) // field modification
		return res+GetAlignOffset(res);
	// If the modif is the last one for this method, return also the size change from padding and tiny to fat
	// Otherwise, return only the size change from the modification itself
	basefulllen = methinfo.code_size;
	newfulllen = basefulllen+res;
	if (tiny_fat_change==1 || tiny_fat_change==2) {
		tinyfatmodif = 11;
		basefulllen +=1; // code is right after the size in tiny methods
	}
	for (i=0;i<modifamount;i++)
		if (method_id==modif[i].method_id) {
			while (position!=modif[i].position && i<modifamount) {
				newfulllen += (int64_t)modif[i].new_length-(int64_t)modif[i].base_length;
				i++;
			}
			if (i+1<modifamount && modif[i+1].method_id==method_id)
				return res;
			break;
		}
	return res+tinyfatmodif+(int32_t)GetAlignOffset(newfulllen)-(int32_t)GetAlignOffset(basefulllen);
}

void SortDllDataModifications(unsigned int modifamount, DllMetaDataModification*& modif) {
	DllMetaDataModification* sorted = new DllMetaDataModification[modifamount];
	unsigned int i,j,next;
	uint32_t nextpos,curpos = 0;
	for (i=0;i<modifamount;i++) {
		nextpos = 0xFFFFFFFF;
		next = 0;
		for (j=0;j<modifamount;j++)
			if (modif[j].position<nextpos && modif[j].position>curpos) {
				next = j;
				nextpos = modif[j].position;
			}
		sorted[i] = modif[next];
		curpos = sorted[i].position;
	}
	for (i=0;i<modifamount;i++)
		modif[i] = sorted[i];
	delete[] sorted;
}

DllMethodInfo** DllMetaData::ComputeTinyFatAndMethodInfo(unsigned int modifamount, DllMetaDataModification* modif) {
	DllMethodInfo** res = new DllMethodInfo*[modifamount];
	unsigned int i,j;
	for (i=0;i<modifamount;i++) {
		if (modif[i].method_id>=0) {
			if (i>0 && modif[i].method_id==modif[i-1].method_id) {
				res[i] = res[i-1];
				if (modif[i-1].tiny_fat_change==1 || modif[i-1].tiny_fat_change==2)
					modif[i].tiny_fat_change = 2; // turning tiny to fat
			} else {
				res[i] = new DllMethodInfo;
				dll_file.seekg(GetMethodOffset(modif[i].method_id));
				res[i]->ReadMethodInfo(dll_file);
				if (res[i]->is_tiny) { // only turn tiny to fat, not the other way...
					DllMethodInfo modifinfo = *res[i];
					for (j=i;j<modifamount && modif[i].method_id==modif[j].method_id;j++)
						modifinfo.code_size += (int64_t)modif[j].new_length-(int64_t)modif[j].base_length;
					modifinfo.UpdateInformation();
					if (!modifinfo.is_tiny)
						modif[i].tiny_fat_change = 1; // 1st modif of a method turning tiny to fat
				}
			}
		} else
			res[i] = NULL;
	}
	return res;
}

uint32_t DllMetaData::ApplyOffsetModification(uint32_t offset, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo, int32_t baseoffset) {
	if (offset==0)
		return 0;
	uint32_t res = offset;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		if (baseoffset+offset>(int32_t)modif[i].position && baseoffset<(int32_t)modif[i].position)
			res += modif[i].GetSizeModification(modifamount,modif,*modifmethinfo[i]);
	return res;
}

uint32_t DllMetaData::ApplyVirtualOffsetModification(uint32_t baserva, uint32_t newsecoff, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo) {
	if (baserva==0)
		return 0;
	uint32_t baseoff = GetVirtualAddressOffset(baserva);
	uint32_t res = baseoff;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		if (baseoff>modif[i].position)
			res += modif[i].GetSizeModification(modifamount,modif,*modifmethinfo[i]);
	return res+newsecoff;
}

uint32_t DllMetaData::ApplySizeModification(uint32_t size, unsigned int modifamount, DllMetaDataModification* modif, DllMethodInfo** modifmethinfo) {
	uint32_t res = size;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		res += modif[i].GetSizeModification(modifamount,modif,*modifmethinfo[i]);
	return res;
}

uint32_t DllMetaData::WriteMethodSizeModification(fstream& fdest, uint32_t methid, unsigned int modifamount, DllMetaDataModification* modif) {
	DllMethodInfo methinfo;
	unsigned int i;
	uint32_t basesize;
	methinfo.ReadMethodInfo(dll_file);
	basesize = methinfo.code_size;
	for (i=0;i<modifamount;i++)
		if (modif[i].method_id==methid)
			methinfo.code_size += (int64_t)modif[i].new_length-(int64_t)modif[i].base_length;
	methinfo.UpdateInformation();
	methinfo.WriteMethodInfo(fdest);
	return basesize;
}

int DllMetaData::Duplicate(fstream& fdest, unsigned int modifamount, DllMetaDataModification* modif) {
	uint32_t copysize, offtmp, tmp32;
	uint32_t metasectionoff, metasectionvirtoff;
	unsigned int i,j,modifindex;
	uint32_t* newsecvpos, *newsecvsize;
	DllMethodInfo** modifmethinfo;
	char* buffer;
	SortDllDataModifications(modifamount,modif);
	for (i=0;i<modifamount;i++) {
		modif[i].method_id = GetMethodAtOffset(modif[i].position);
		modif[i].tiny_fat_change = 0;
	}
	modifmethinfo = ComputeTinyFatAndMethodInfo(modifamount,modif);
	newsecvpos = new uint32_t[section_amount];
	newsecvsize = new uint32_t[section_amount];
	for (i=0;i<section_amount;i++) {
		if (i<meta_data_section) {
			newsecvpos[i] = section_virtual_offset[i];
			newsecvsize[i] = section_virtual_size[i];
		} else if (i==meta_data_section) {
			newsecvsize[i] = ApplySizeModification(section_virtual_size[i],modifamount,modif,modifmethinfo);
			newsecvpos[i] = section_virtual_offset[i];
		} else {
			newsecvsize[i] = section_virtual_size[i];
			newsecvpos[i] = newsecvpos[i-1]+newsecvsize[i-1];
			newsecvpos[i] += GetAlignOffset(newsecvpos[i],0x2000);
		}
	}
	dll_file.seekg(0);
	copysize = pe_header_offset+0x1C;
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	ReadLong(dll_file);
	WriteLong(fdest,newsecvsize[meta_data_section]+GetAlignOffset(newsecvsize[meta_data_section],0x200)); // code size
//	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	
	ReadLong(dll_file);
	WriteLong(fdest,newsecvpos[1]); // base of data
//	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	
	ReadLong(dll_file);
	tmp32 = newsecvpos[2]+newsecvsize[2];
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32,0x2000)); // image size
//	WriteLong(fdest,ReadLong(dll_file));
	
	copysize = (pe_type==0x20B ? 0x44 : 0x34);
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	ReadLong(dll_file);
	WriteLong(fdest,newsecvpos[1]); // resource_table.offset
//	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	
	ReadLong(dll_file);
	WriteLong(fdest,newsecvpos[2]); // base_relocation_table.offset
//	WriteLong(fdest,ReadLong(dll_file));
	
	buffer = new char[0x44];
	dll_file.read(buffer,0x44);
	fdest.write(buffer,0x44);
	delete[] buffer;
	
//	tmp32 = ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo);
//	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // cli_directory.virtual_address
	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	for (i=0;i<section_amount;i++) {
		WriteLongLong(fdest,ReadLongLong(dll_file));
		ReadLong(dll_file);
		ReadLong(dll_file);
		WriteLong(fdest,newsecvsize[i]); // section.virtual_size
		WriteLong(fdest,newsecvpos[i]); // section.virtual_address
		if (i==meta_data_section) {
			metasectionvirtoff = newsecvpos[i];
			ReadLong(dll_file);
			WriteLong(fdest,newsecvsize[i]+GetAlignOffset(newsecvsize[i],0x200)); // section.size
			metasectionoff = ApplyOffsetModification(ReadLong(dll_file),modifamount,modif,modifmethinfo);
			metasectionoff += GetAlignOffset(metasectionoff,0x200);
			WriteLong(fdest,metasectionoff); // section.offset
		} else {
			WriteLong(fdest,ReadLong(dll_file));
			tmp32 = ApplySizeModification(ReadLong(dll_file),modifamount,modif,modifmethinfo);
			WriteLong(fdest,tmp32+GetAlignOffset(tmp32,0x200));
		}
		buffer = new char[0x10];
		dll_file.read(buffer,0x10);
		fdest.write(buffer,0x10);
		delete[] buffer;
/*		buffer = new char[0x28];
		dll_file.read(buffer,0x28);
		fdest.write(buffer,0x28);
		delete[] buffer;*/
	}
	copysize = GetVirtualAddressOffset(cli_directory.virtual_address)+8-dll_file.tellg();
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // meta_data_directory.virtual_address
//	WriteLong(fdest,ReadLong(dll_file));
//	ReadLong(dll_file);
//	WriteLong(fdest,newsecvsize[meta_data_section]+newsecvpos[meta_data_section]-(tmp32+GetAlignOffset(tmp32))); // meta_data_directory.size
	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	WriteLong(fdest,ReadLong(dll_file));
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // resources_directory.virtual_address
//	WriteLong(fdest,ReadLong(dll_file));
	
	WriteLong(fdest,ReadLong(dll_file));
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // strong_name_directory.virtual_address
	WriteLong(fdest,ReadLong(dll_file));
	
	modifindex = 0;
	offtmp = GetVirtualAddressOffset(meta_data_directory.virtual_address);
	while (modifindex<modifamount && modif[modifindex].position<offtmp) {
		if (modif[modifindex].method_id>=0) {
			copysize = GetMethodOffset(modif[modifindex].method_id);
			if (copysize>dll_file.tellg()) {
				copysize -= dll_file.tellg();
				buffer = new char[copysize];
				dll_file.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
				WriteMethodSizeModification(fdest,modif[modifindex].method_id,modifamount,modif);
			} else if (copysize==dll_file.tellg()) {
				WriteMethodSizeModification(fdest,modif[modifindex].method_id,modifamount,modif);
			}
		}
		copysize = modif[modifindex].position;
		if (copysize>dll_file.tellg()) {
			copysize -= dll_file.tellg();
			buffer = new char[copysize];
			dll_file.read(buffer,copysize);
			fdest.write(buffer,copysize);
			delete[] buffer;
		}
		dll_file.seekg(modif[modifindex].base_length,ios::cur);
		fdest.write((const char*)modif[modifindex].value,modif[modifindex].new_length);
		if (modif[modifindex].method_id>=0 && (modifindex+1>=modifamount || modif[modifindex+1].method_id!=modif[modifindex].method_id)) {
			copysize = modifmethinfo[modifindex]->code_abs_offset+modifmethinfo[modifindex]->code_size;
			if (copysize>dll_file.tellg()) {
				copysize -= dll_file.tellg();
				buffer = new char[copysize];
				dll_file.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
			}
			dll_file.seekg(GetAlignOffset(dll_file.tellg()),ios::cur);
			while (fdest.tellp()%4!=0)
				fdest.put(0);
		} else if (modif[modifindex].method_id<0) { // Field modifications are assumed to change the whole field
			dll_file.seekg(GetAlignOffset(dll_file.tellg()),ios::cur);
			while (fdest.tellp()%4!=0)
				fdest.put(0);
		}
		modifindex++;
	}
	copysize = offtmp-dll_file.tellg();
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	while (fdest.tellp()%4!=0)
		fdest.put(0);
	copysize = meta_data_version_name_size+0x14;
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	for (i=0;i<meta_data_stream_amount;i++) {
/*		WriteLong(fdest,ApplyOffsetModification(ReadLong(dll_file),modifamount,modif,modifmethinfo,section_offset[meta_data_section]+meta_data_directory.virtual_address-section_virtual_offset[meta_data_section])); // stream_offset
		if (i==stream_table_heap)
			WriteLong(fdest,ApplySizeModification(ReadLong(dll_file),modifamount,modif,modifmethinfo)); // stream_size
		else
			WriteLong(fdest,ReadLong(dll_file));
		tmp32 = stream_name[i].length()+1+GetAlignOffset(stream_name[i].length()+1);
		while (tmp32>0) {
			WriteLong(fdest,ReadLong(dll_file));
			tmp32 -= 4;
		}*/
		copysize = 0x8+stream_name[i].length()+1+GetAlignOffset(stream_name[i].length()+1);
		buffer = new char[copysize];
		dll_file.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
	}
	// Nothing to change in stream_table_heap
	if (table[TableType_Method].length>0) {
		offtmp = section_offset[meta_data_section]+table[TableType_Method].offset;
		copysize = offtmp-dll_file.tellg();
		buffer = new char[copysize];
		dll_file.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		copysize = table[TableType_Method].row_size-4;
		buffer = new char[copysize];
		for (i=0;i<table[TableType_Method].length;i++) {
			tmp32 = ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo);
			WriteLong(fdest,tmp32); // method.virtual_address
			dll_file.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}
/*	if (table[TableType_FieldLayout].length>0) { // Not sure if offsets are rva here... FieldLayouts are not used anyway
		offtmp = section_offset[meta_data_section]+table[TableType_FieldLayout].offset;
		copysize = offtmp-dll_file.tellg();
		buffer = new char[copysize];
		dll_file.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		
		copysize = table[TableType_FieldLayout].row_size-4;
//		copysize = table[TableType_FieldLayout].row_size;
		
		buffer = new char[copysize];
		for (i=0;i<table[TableType_FieldLayout].length;i++) {
			WriteLong(fdest,ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo)); // fieldlayout.offset
			dll_file.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}*/
	if (table[TableType_FieldRVA].length>0) {
		offtmp = section_offset[meta_data_section]+table[TableType_FieldRVA].offset;
		copysize = offtmp-dll_file.tellg();
		buffer = new char[copysize];
		dll_file.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		copysize = table[TableType_FieldRVA].row_size-4;
		buffer = new char[copysize];
		for (i=0;i<table[TableType_FieldRVA].length;i++) {
			WriteLong(fdest,ApplyVirtualOffsetModification(ReadLong(dll_file),metasectionvirtoff-metasectionoff,modifamount,modif,modifmethinfo)); // fieldrva.virtual_address
			dll_file.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}
	// Modifications are assumed to be in the method/fieldrva code
/*	while (modifindex<modifamount) {
		copysize = modif[modifindex].position-dll_file.tellg();
		buffer = new char[copysize];
		dll_file.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		dll_file.seekg(modif[modifindex].base_length,ios::cur);
		fdest.write((const char*)modif[modifindex].value,modif[modifindex].new_length);
		modifindex++;
	}*/
	for (i=0;i<section_amount;i++) {
		if (section_offset[i]>dll_file.tellg()) {
			copysize = section_offset[i]-dll_file.tellg();
			buffer = new char[copysize];
			dll_file.read(buffer,copysize);
			fdest.write(buffer,copysize);
			delete[] buffer;
			copysize = GetAlignOffset(fdest.tellp(),0x200);
			if (copysize>0) {
				buffer = new char[copysize]();
				fdest.write(buffer,copysize);
				delete[] buffer;
			}
			if (i==1) { // DEBUG: rsrc section
				copysize = 0x48;
				buffer = new char[copysize];
				dll_file.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
				WriteLong(fdest,ReadLong(dll_file)+newsecvpos[i]-section_virtual_offset[i]);
			}
		}
	}
	copysize = section_offset[section_amount-1]+section_size[section_amount-1]-dll_file.tellg();
	buffer = new char[copysize];
	dll_file.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	delete[] newsecvpos;
	delete[] newsecvsize;
	for (i=0;i<modifamount;i++) {
		if (modifmethinfo[i]!=NULL && (i+1>=modifamount || modifmethinfo[i]!=modifmethinfo[i+1]))
			delete modifmethinfo[i];
	}
	delete[] modifmethinfo;
	return 0;
}

//==================================================//
//                    CIL CODE                      //
//==================================================//

// Method header
void DllMethodInfo::ReadMethodInfo(fstream& f) {
	flags = f.get();
	is_tiny = (flags & 0x3)==0x2;
	if (is_tiny) { // Tiny
		code_size = flags >> 2;
		flags &= 0x3;
		max_stack_size = 8;
		local_var_token = 0;
	} else if ((flags & 0x3)==0x3) { // Fat
		flags |= (f.get() << 8);
		max_stack_size = ReadShort(f);
		code_size = ReadLong(f);
		local_var_token = ReadLong(f);
	}
	code_abs_offset = f.tellg();
}

void DllMethodInfo::WriteMethodInfo(fstream& f) {
	if (is_tiny) {
		f.put(flags | (code_size << 2));
	} else if ((flags & 0x3)==0x3) {
		WriteShort(f,flags);
		WriteShort(f,max_stack_size);
		WriteLong(f,code_size);
		WriteLong(f,local_var_token);
	}
}

uint32_t DllMethodInfo::JumpToInstructions(fstream& f, unsigned int amount, ILInstruction* inst, bool ignoreoffsetval) {
	uint32_t tmppos;
	unsigned int i;
	if (f.tellg()<code_abs_offset)
		return 0;
	while (f.tellg()<code_abs_offset+code_size) {
		for (i=0;i<amount;i++) {
			ILInstruction dllinst;
			dllinst.Read(f);
			if (i==0)
				tmppos = f.tellg();
			if ((!ignoreoffsetval || (dllinst.GetCode().type!=ILT_OFFSET && dllinst.GetCode().type!=ILT_SWITCH)) && !(dllinst==inst[i]))
				break;
		}
		if (i==amount)
			return (unsigned int)f.tellg()-code_abs_offset;
		f.seekg(tmppos);
	}
	return 0;
}

void DllMethodInfo::UpdateInformation() {
	if (is_tiny) {
		is_tiny = code_size<=0x3F && max_stack_size==8 && local_var_token==0 && flags==2;
		if (!is_tiny) {
			code_abs_offset += 11;
			flags = (flags & 0xCFFC) | 0x3003;
		}
	}
}

uint32_t DllMethodInfo::JumpToCode(fstream& f) {
	uint32_t buffer, res = 0;
	buffer = f.get();	res++;
	if ((buffer & 3)==3)
		f.seekg(11,ios::cur);	res += 11;
	return res;
}

bool DllMethodInfo::IsCILOffsetValidFromHere(fstream& f, int offset) {
	while (offset>0) {
		ILInstruction inst;
		inst.Read(f);
		offset -= inst.GetLength();
	}
	return offset==0;
}

// Code
template <typename Word>
void ILStoreValueToRawData(uint8_t* raw, unsigned int pos, Word& value, unsigned int size) {
	unsigned int i,j;
	uint8_t mask,modpos = pos%8;
	for (i=0;i<size;i+=8) {
		mask = 0;
		for (j=0;j<8;j++)
			mask |= ((j>=modpos || i>0) && (i+j<pos+size)) << j;
		raw[(pos+i)/8] = (raw[(pos+i)/8] & ~mask) | (((value >> i) << modpos) & mask);
	}
}

bool debug_il_integer_is_string;
int64_t DllMetaData::ScriptGetNextIntegerValue(bool includestr, uint32_t* integercodepos, SteamWDictionary* strdico) {
	uint8_t code;
	int64_t argvalue;
	debug_il_integer_is_string = false;
	do {
		if (integercodepos!=NULL)
			*integercodepos = dll_file.tellg();
		code = dll_file.get();
		if (code==0x15) {
			return -1;
		} else if (code==0x16 || code==0x14) {
			debug_il_integer_is_string = code==0x14;
			return 0;
		} else if (code==0x17) {
			return 1;
		} else if (code==0x18) {
			return 2;
		} else if (code==0x19) {
			return 3;
		} else if (code==0x1A) {
			return 4;
		} else if (code==0x1B) {
			return 5;
		} else if (code==0x1C) {
			return 6;
		} else if (code==0x1D) {
			return 7;
		} else if (code==0x1E) {
			return 8;
		} else if (code==0x1F) {
			argvalue = dll_file.get();
			return argvalue;
		} else if (code==0x20) {
			argvalue = dll_file.get();
			argvalue |= (dll_file.get() << 8);
			argvalue |= (dll_file.get() << 16);
			argvalue |= (dll_file.get() << 24);
			return argvalue;
		} else if (code==0x72) {
			argvalue = dll_file.get();
			argvalue |= (dll_file.get() << 8);
			argvalue |= (dll_file.get() << 16);
			argvalue |= (dll_file.get() << 24);
			debug_il_integer_is_string = true;
			if (!strdico || (argvalue >> 24)!=0x70)
				return 1;
			wstring strval = GetUserStringAtPos(argvalue & 0xFFFFFF);
			for (unsigned int i=0;i<G_N_ELEMENTS(SteamWeaponModel);i++) // Debug
				if (strval.compare(strdico[i].name)==0)
					return strdico[i].id;
			return 1;
		}
		dll_file.seekg(GetILCode(code).size,ios::cur);
	} while (true);
}

uint8_t* DllMetaData::ConvertScriptToRaw_Object(unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[]) {
	unsigned int i,j,k,curobjpos,curobjpossub,objsize = 0;
	uint8_t code;
	uint8_t* res;
	int64_t arrindex,arrindexsub,argvalue;
	for (i=0;i<fieldamount;i++)
		objsize += fieldsize[i];
	res = new uint8_t[objamount*objsize/8];
	j = 0;
	for (i=0;i<objamount*objsize;i+=8)
		res[j++] = 0;
	for (i=0;i<objamount;i++) {
		arrindex = ScriptGetNextIntegerValue(true);
		curobjpos = 0;
		for (j=0;j<fieldamount;j++) {
			if (array==NULL || array[j]==0) {
				argvalue = ScriptGetNextIntegerValue(true,NULL,SteamWeaponModel); // Debug : handle string <-> ID conversion
				ILStoreValueToRawData(res,arrindex*objsize+curobjpos,argvalue,fieldsize[j]);
				curobjpos += fieldsize[j];
			} else {
				do {
					code = dll_file.get();
					dll_file.seekg(GetILCode(code).size,ios::cur);
				} while (code!=IL_CODE_NEWARR);
				code = dll_file.get();
				while (code==IL_CODE_DUP) {
					arrindexsub = ScriptGetNextIntegerValue(true);
					argvalue = ScriptGetNextIntegerValue(true);
					curobjpossub = 0;
					for (k=0;k<arrindexsub;k++)
						curobjpossub += fieldsize[j+k];
					ILStoreValueToRawData(res,arrindex*objsize+curobjpos+curobjpossub,argvalue,fieldsize[j+arrindexsub]);
					code = dll_file.get(); // stelem
					dll_file.seekg(GetILCode(code).size,ios::cur);
					code = dll_file.get();
				}
				dll_file.seekg(-1,ios::cur);
				for (k=0;k<array[j];k++)
					curobjpos += fieldsize[j+k];
				j += array[j]-1;
			}
		}
		do {
			code = dll_file.get();
			dll_file.seekg(GetILCode(code).size,ios::cur);
		} while (code!=IL_CODE_STELEMREF);
	}
	return res;
}

int64_t** DllMetaData::ConvertScriptToRaw_Array2(unsigned int objamount, unsigned int arrayamount) {
	unsigned int i,j,curobjpos,objsize = 0;
	uint8_t code;
	int64_t** res;
	int64_t arrindex1,arrindex2,argvalue;
	res = new int64_t*[objamount];
	for (i=0;i<objamount;i++) {
		res[i] = new int64_t[arrayamount];
		for (j=0;j<arrayamount;j++)
			res[i][j] = 0;
	}
	do {
		arrindex1 = ScriptGetNextIntegerValue(true);
		arrindex2 = ScriptGetNextIntegerValue(true);
		argvalue = ScriptGetNextIntegerValue(true);
		res[arrindex1][arrindex2] = argvalue;
		code = dll_file.get(); // Should be "call array::Set"
		dll_file.seekg(GetILCode(code).size,ios::cur);
		code = dll_file.get();
		dll_file.seekg(GetILCode(code).size,ios::cur);
	} while (code!=IL_CODE_STSFLD);
	dll_file.seekg(-5,ios::cur); // End at the STSFLD position
	return res;
}

uint8_t buffertmp[0x100000];
DllMetaDataModification DllMetaData::ConvertRawToScript_Object(uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[]) {
	unsigned int i,j,k,posbuffer,buffersize,objsize = 0;
	uint32_t copystart,copyend,macrotmp;
	uint32_t minusone32 = 0xFFFFFFFF;
	uint16_t minusone16 = 0xFFFF;
	uint8_t minusone8 = 0xFF;
	uint8_t code;
	char* buffer;
	DllMetaDataModification res;
	for (i=0;i<fieldamount;i++)
		objsize += fieldsize[i];
	posbuffer = 0;
	
	#define MACRO_ILSCRIPT_WRITE_INTEGER(VALUE,MINUSONE) \
		if (VALUE==MINUSONE) { \
			buffertmp[posbuffer++] = 0x15; \
		} else if (VALUE==0) { \
			buffertmp[posbuffer++] = 0x16; \
		} else if (VALUE==1) { \
			buffertmp[posbuffer++] = 0x17; \
		} else if (VALUE==2) { \
			buffertmp[posbuffer++] = 0x18; \
		} else if (VALUE==3) { \
			buffertmp[posbuffer++] = 0x19; \
		} else if (VALUE==4) { \
			buffertmp[posbuffer++] = 0x1A; \
		} else if (VALUE==5) { \
			buffertmp[posbuffer++] = 0x1B; \
		} else if (VALUE==6) { \
			buffertmp[posbuffer++] = 0x1C; \
		} else if (VALUE==7) { \
			buffertmp[posbuffer++] = 0x1D; \
		} else if (VALUE==8) { \
			buffertmp[posbuffer++] = 0x1E; \
		} else if (VALUE<0x80) { \
			buffertmp[posbuffer++] = 0x1F; \
			buffertmp[posbuffer++] = VALUE; \
		} else { \
			buffertmp[posbuffer++] = 0x20; \
			buffertmp[posbuffer++] = (VALUE & 0xFF); \
			buffertmp[posbuffer++] = ((VALUE >> 8) & 0xFF); \
			buffertmp[posbuffer++] = ((VALUE >> 16) & 0xFF); \
			buffertmp[posbuffer++] = ((VALUE >> 24) & 0xFF); \
		}
	
	#define MACRO_ILSCRIPT_COPY(FROM,TO) \
		if (FROM<TO) { \
			macrotmp = dll_file.tellg(); \
			dll_file.seekg(FROM); \
			buffersize = TO-FROM; \
			dll_file.read((char*)buffertmp+posbuffer,buffersize); \
			posbuffer += buffersize; \
			dll_file.seekg(macrotmp); \
		}
	
	dll_file.seekg(pos);
	for (i=0;i<objamount;i++) {
		copystart = dll_file.tellg();
		ScriptGetNextIntegerValue(true,&copyend);
		MACRO_ILSCRIPT_COPY(copystart,copyend)
		MACRO_ILSCRIPT_WRITE_INTEGER(i,minusone32)
		for (j=0;j<fieldamount;j++) {
			if (array==NULL || array[j]==0) {
				copystart = dll_file.tellg();
				k = ScriptGetNextIntegerValue(true,&copyend,SteamWeaponModel); // Debug : handle string <-> ID conversion
				MACRO_ILSCRIPT_COPY(copystart,copyend)
				if (debug_il_integer_is_string) {
					if (k==0) {
						buffertmp[posbuffer++] = 0x14;
					} else {
						MACRO_ILSCRIPT_COPY(copyend,copyend+5)
					}
				} else if (fieldsize[j]<=8) {
					MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j],minusone8)
				} else if (fieldsize[j]<=16) {
					MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j],minusone16)
				} else {
					MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j],minusone32)
				}
			} else {
				copystart = dll_file.tellg();
				do {
					code = dll_file.get();
					dll_file.seekg(GetILCode(code).size,ios::cur);
				} while (code!=IL_CODE_NEWARR);
				copyend = dll_file.tellg();
				MACRO_ILSCRIPT_COPY(copystart,copyend)
				// Do not copy array
				code = dll_file.get();
				while (code==IL_CODE_DUP) {
					ScriptGetNextIntegerValue(true); // array sub-index
					ScriptGetNextIntegerValue(true); // argument value
					code = dll_file.get(); // stelem
					dll_file.seekg(GetILCode(code).size,ios::cur);
					code = dll_file.get();
				}
				dll_file.seekg(-1,ios::cur);
				for (k=0;k<array[j];k++) {
					buffertmp[posbuffer++] = IL_CODE_DUP;
					MACRO_ILSCRIPT_WRITE_INTEGER(k,minusone32)
					if (fieldsize[j]<=8) {
						MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j+k],minusone8)
						buffertmp[posbuffer++] = 0x9C; // stelem.i1
					} else if (fieldsize[j]<=16) {
						MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j+k],minusone16)
						buffertmp[posbuffer++] = 0x9D; // stelem.i2
					} else {
						MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j+k],minusone32)
						buffertmp[posbuffer++] = 0x9E; // stelem.i4
					}
				}
				j += array[j]-1;
			}
		}
		copystart = dll_file.tellg();
		do {
			code = dll_file.get();
			dll_file.seekg(GetILCode(code).size,ios::cur);
		} while (code!=IL_CODE_STELEMREF);
		copyend = dll_file.tellg();
		MACRO_ILSCRIPT_COPY(copystart,copyend)
	}
	res.new_length = posbuffer;
	res.value = new uint8_t[res.new_length];
	memcpy(res.value,buffertmp,res.new_length);
	res.position = pos;
	res.base_length = baselen;
	return res;
}

DllMetaDataModification DllMetaData::ConvertRawToScript_Array2(uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int arrayamount, uint32_t arraysettoken) {
	unsigned int i,j,posbuffer;
	uint32_t minusone32 = 0xFFFFFFFF;
	DllMetaDataModification res;
	char* buffer;
	posbuffer = 0;
	for (i=0;i<objamount;i++) {
		for (j=0;j<arrayamount;j++) {
			if (newfieldvalues[i][j]!=0) {
				MACRO_ILSCRIPT_WRITE_INTEGER(i,minusone32)
				MACRO_ILSCRIPT_WRITE_INTEGER(j,minusone32)
				MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j],minusone32)
				buffertmp[posbuffer++] = 0x28; // call
				BufferInitPosition(posbuffer);
				BufferWriteLong(buffertmp,arraysettoken);
				posbuffer += 4;
				buffertmp[posbuffer++] = 0x25; // dup
			}
		}
	}
	posbuffer--; // remove last dup
	res.new_length = posbuffer;
	res.value = new uint8_t[res.new_length];
	memcpy(res.value,buffertmp,res.new_length);
	res.position = pos;
	res.base_length = baselen;
	return res;
}

// ILInstruction
void ILInstruction::Read(fstream& f) {
	if (switchpos)
		delete[] switchpos;
	if (ilcode)
		ilcode = NULL;
	opcode = f.get();
	if (opcode==0xFE)
		opcode = (opcode << 8) | f.get();
	ILCode& code = GetCode();
	switch (code.size) {
		case -1:
			param = ReadLong(f);
			switchpos = new int32_t[param];
			for (unsigned int i=0;i<param;i++)
				switchpos[i] = (int32_t)ReadLong(f);
			break;
		case 0: param = 0; switchpos = NULL; break;
		case 1: param = f.get(); switchpos = NULL; break;
		case 2: param = ReadShort(f); switchpos = NULL; break;
		case 4: param = ReadLong(f); switchpos = NULL; break;
		case 8: param = ReadLongLong(f); switchpos = NULL; break;
	}
}

bool ILInstruction::operator==(ILInstruction& ilcmp) {
	if (opcode==ilcmp.opcode && param==ilcmp.param) {
		if (switchpos==NULL)
			return true;
		for (unsigned int i=0;i<param;i++)
			if (switchpos[i]!=ilcmp.switchpos[i])
				return false;
		return true;
	}
	return false;
}

unsigned int ILInstruction::GetLength() {
	unsigned int res = opcode>0xFF ? 2 : 1;
	ILCode& code = GetCode();
	if (code.size>=0)
		res += code.size;
	else
		res += 4+4*param;
	return res;
}

void ILInstruction::AppendToBuffer(uint8_t* buffer) {
	ILCode& code = GetCode();
	if (opcode>0xFF) {
		BufferWriteChar(buffer,0xFE);
		BufferWriteChar(buffer,opcode & 0xFF);
	} else {
		BufferWriteChar(buffer,opcode);
	}
	switch (code.size) {
		case 1: BufferWriteChar(buffer,param); break;
		case 2: BufferWriteShort(buffer,param); break;
		case 4: BufferWriteLong(buffer,param); break;
		case 8: BufferWriteLongLong(buffer,param); break;
		case -1:
			BufferWriteLong(buffer,param);
			for (unsigned int i=0;i<param;i++)
				BufferWriteLong(buffer,(uint32_t)switchpos[i]);
			break;
	}
}

ILCode& ILInstruction::GetCode() {
	if (ilcode)
		return *ilcode;
	ilcode = &GetILCode(opcode);
	return *ilcode;
}

// CIL Data Set
void CILDataSet::Init(DllMetaData* d) {
	data = d;
	rawmodifamount = 0;
	macromodifamount = 0;
	rawmodif = NULL;
	macromodif = NULL;
}

bool CILDataSet::IsMacroEnabled(uint32_t macroid) {
	for (unsigned int i=0;i<macromodifamount;i++)
		if (macromodif[i].type->id==macroid)
			return true;
	return false;
}

uint8_t* CILDataSet::GetModifiedMethodRaw(unsigned int tid, unsigned int mid, uint32_t* length) {
	uint32_t methoff,modifoff,bufferpos,bufferread;
	DllMetaDataModification* newmacromod = NULL;
	unsigned int i,absmethid,methamount;
	fstream& dllfile = data->dll_file;
	DllMethodInfo methinfo;
	int curmodif,macrometh;
	uint8_t* res;
	absmethid = data->cil_type_method_id[tid]-1+mid;
	methoff = data->GetMethodOffset(absmethid);
	if (methoff==0) {
		*length = 0;
		return NULL;
	}
	dllfile.seekg(methoff);
	methinfo.ReadMethodInfo(dllfile);
	methoff = dllfile.tellg();
	*length = methinfo.code_size;
	for (i=0;i<rawmodifamount;i++)
		if (rawmodif[i].method_id==absmethid)
			*length += rawmodif[i].new_length-rawmodif[i].base_length;
	for (i=0;i<macromodifamount;i++) {
		macrometh = macromodif[i].FindMethod(tid,mid);
		if (macrometh>=0) {
			newmacromod = macromodif[i].info->ComputeModifications(&methamount);
			*length += newmacromod[macrometh].new_length-newmacromod[macrometh].base_length;
			break;
		}
	}
	res = new uint8_t[*length];
	curmodif = -1;
	modifoff = methoff;
	bufferpos = 0;
	dllfile.seekg(methoff);
	if (newmacromod!=NULL) {
		bufferread = newmacromod[macrometh].position-dllfile.tellg();
		dllfile.read((char*)&res[bufferpos],bufferread);
		bufferpos += bufferread;
		dllfile.seekg(newmacromod[macrometh].base_length,ios::cur);
		memcpy(&res[bufferpos],newmacromod[macrometh].value,newmacromod[macrometh].new_length);
		bufferpos += newmacromod[macrometh].new_length;
		delete[] newmacromod;
		curmodif = -2;
	}
	while (curmodif>=-1) {
		for (i=0;i<rawmodifamount;i++)
			if (rawmodif[i].method_id==absmethid && rawmodif[i].position>=modifoff && (curmodif==-1 || rawmodif[i].position<rawmodif[curmodif].position))
				curmodif = i;
		if (curmodif>=0) {
			modifoff = rawmodif[curmodif].position+rawmodif[curmodif].base_length;
			bufferread = rawmodif[curmodif].position-dllfile.tellg();
			dllfile.read((char*)&res[bufferpos],bufferread);
			bufferpos += bufferread;
			dllfile.seekg(rawmodif[curmodif].base_length,ios::cur);
			memcpy(&res[bufferpos],rawmodif[curmodif].value,rawmodif[curmodif].new_length);
			bufferpos += rawmodif[curmodif].new_length;
			curmodif = -1;
		} else {
			curmodif = -2;
		}
	}
	bufferread = methoff+methinfo.code_size-dllfile.tellg();
	dllfile.read((char*)&res[bufferpos],bufferread);
	return res;
}

void CILDataSet::AddMacroModif(uint32_t macroid) {
	unsigned int i;
	CILMacro* newmodiflist = new CILMacro[macromodifamount+1];
	memcpy(newmodiflist,macromodif,macromodifamount*sizeof(CILMacro));
	newmodiflist[macromodifamount].Init(macroid,data);
	for (i=0;i<macromodifamount;i++)
		macromodif[i].isdeletedforcopy = true;
	delete[] macromodif;
	macromodif = newmodiflist;
	macromodifamount++;
	for (i=0;i<macromodifamount;i++)
		if (macromodif[i].info)
			macromodif[i].info->parent = &macromodif[i];
}

void CILDataSet::RemoveMacroModif(uint32_t macroid) {
	unsigned int i,index = macromodifamount;
	for (i=0;i<macromodifamount;i++)
		if (macromodif[i].type->id==macroid) {
			index = i;
			break;
		}
	if (index==macromodifamount)
		return;
	CILMacro* newmodiflist = new CILMacro[macromodifamount-1];
	memcpy(newmodiflist,macromodif,index*sizeof(CILMacro));
	memcpy(newmodiflist+index,macromodif+index+1,(macromodifamount-index-1)*sizeof(CILMacro));
	for (i=0;i<macromodifamount;i++)
		macromodif[i].isdeletedforcopy = i!=index;
	delete[] macromodif;
	macromodif = newmodiflist;
	macromodifamount--;
	for (i=0;i<macromodifamount;i++)
		if (macromodif[i].info)
			macromodif[i].info->parent = &macromodif[i];
}

bool CILDataSet::UpdateWithNewModification(DllMetaDataModification& newmodif, unsigned int checkmacrostartindex) {
	unsigned int j,k,tid,mid,methamount,macroconvertedamount;
	int i,macrometh;
	bool shouldconvertmacro;
	data->GetMethodRelativeId(newmodif.method_id,&tid,&mid);
	for (i=0;i<rawmodifamount;i++) {
		if (newmodif.method_id==rawmodif[i].method_id) {
			delete[] rawmodif[i].value;
			rawmodif[i] = newmodif;
			return false;
		}
	}
	for (i=checkmacrostartindex;i<macromodifamount;i++) {
		macrometh = macromodif[i].FindMethod(tid,mid);
		if (macrometh>=0) {
			DllMetaDataModification* newmacromod = macromodif[i].info->ComputeModifications(&methamount);
			macroconvertedamount = 0;
			for (j=0;j<methamount;j++) {
				shouldconvertmacro = newmacromod[j].method_id!=newmodif.method_id;
				if (shouldconvertmacro) {
					for (k=0;k<rawmodifamount;k++) {
						if (rawmodif[k].method_id==newmacromod[j].method_id) {
							shouldconvertmacro = false;
							break;
						}
					}
				}
				if (shouldconvertmacro) {
					UpdateWithNewModification(newmacromod[j],i+1);
					macroconvertedamount++;
				}
			}
			if (macroconvertedamount==0)
				delete[] newmacromod;
			RemoveMacroModif(macromodif[i].type->id);
			i--;
		}
	}
	DllMetaDataModification* newmodiflist = new DllMetaDataModification[rawmodifamount+1];
	memcpy(newmodiflist,rawmodif,rawmodifamount*sizeof(DllMetaDataModification));
	newmodiflist[rawmodifamount] = newmodif;
	delete[] rawmodif;
	rawmodif = newmodiflist;
	rawmodifamount++;
	return true;
}

int* CILDataSet::LoadHWS(fstream &ffhws) {
	int* res = new int[4];
	unsigned int i,j;
	bool methok;
	uint32_t rawamount,macroamount,methodid,newpos,newblen,newmlen,macrotype;
	string s,expectedname,mname,tname,nspace,argstr;
	uint16_t version,macroflag;
	char c;
	DllMethodInfo methinfo;
	res[0] = 0; res[1] = 0; res[2] = 0; res[3] = 0;
	HWSReadShort(ffhws,version);
	HWSReadLong(ffhws,rawamount);
	HWSReadLong(ffhws,macroamount);
	for (i=0;i<rawamount;i++) {
		HWSReadLong(ffhws,methodid);
		nspace = ""; tname = ""; mname = ""; argstr = "";
		while ((c = ffhws.get())!=0) nspace += c;
		while ((c = ffhws.get())!=0) tname += c;
		while ((c = ffhws.get())!=0) mname += c;
		while ((c = ffhws.get())!=0) argstr += c;
		HWSReadLong(ffhws,newpos);
		HWSReadLong(ffhws,newblen);
		HWSReadLong(ffhws,newmlen);
		methok = methodid>=0 && methodid<data->table[TableType_Method].length;
		if (methok) {
			expectedname = data->GetMethodNameById(methodid);
			if (nspace.length()>0)
				s = nspace+"."+tname+"::"+mname;
			else
				s = tname+"::"+mname;
			if (expectedname.compare(s)!=0)
				methok = false;
		}
		if (!methok) {
			methodid = data->GetMethodIdByName(tname.c_str(),mname.c_str(),nspace.c_str());
			if (methodid>=0)
				methok = true;
			else
				res[0]++;
		}
		if (methok) {
			data->dll_file.seekg(data->GetMethodOffset(methodid));
			methinfo.ReadMethodInfo(data->dll_file);
			if (newpos+newblen>methinfo.code_size)
				methok = false;
			else if (!methinfo.IsCILOffsetValidFromHere(data->dll_file,newpos))
				methok = false;
			else if (!methinfo.IsCILOffsetValidFromHere(data->dll_file,newblen))
				methok = false;
			if (!methok)
				res[1]++;
		}
		if (methok) {
			DllMetaDataModification newmodif;
			data->dll_file.seekg(data->GetMethodOffset(methodid));
			methinfo.JumpToCode(data->dll_file);
			newmodif.method_id = methodid;
			newmodif.position = (unsigned int)data->dll_file.tellg()+newpos;
			newmodif.base_length = newblen;
			newmodif.new_length = newmlen;
			newmodif.value = new uint8_t[newmlen];
			ffhws.read((char*)newmodif.value,newmlen);
			UpdateWithNewModification(newmodif);
		} else {
			ffhws.seekg(newmlen,ios::cur);
		}
	}
	for (i=0;i<macroamount;i++) {
		HWSReadShort(ffhws,macroflag);
		HWSReadLong(ffhws,macrotype);
		CILMacro macro(macrotype,data);
		if (macro.info==NULL) {
			res[2]++;
		} else if (IsMacroEnabled(macrotype)) {
		} else {
			methok = true;
			for (j=0;j<rawmodifamount;j++)
				if (macro.FindMethodById(rawmodif[j].method_id)>=0) {
					methok = false;
					break;
				}
			if (methok)
				AddMacroModif(macrotype);
			else
				res[3]++;
		}
	}
	return res;
}

void CILDataSet::WriteHWS(fstream &ffhws) {
	unsigned int i;
	string s,mname,tname,nspace;
	size_t pos;
	HWSWriteShort(ffhws,CIL_HWS_VERSION);
	HWSWriteLong(ffhws,rawmodifamount);
	HWSWriteLong(ffhws,macromodifamount);
	SortDllDataModifications(rawmodifamount,rawmodif);
	for (i=0;i<rawmodifamount;i++) {
		HWSWriteLong(ffhws,rawmodif[i].method_id);
		s = data->GetMethodNameById(rawmodif[i].method_id);
		pos = s.rfind("::");
		mname = s.substr(pos+2);
		s = s.substr(0,pos);
		pos = s.rfind('.');
		if (pos!=string::npos) {
			tname = s.substr(pos+1);
			nspace = s.substr(0,pos);
		} else {
			tname = s;
			nspace = "";
		}
		ffhws << nspace << ends;
		ffhws << tname << ends;
		ffhws << mname << ends;
		ffhws << "" << ends; // ToDo: write argument specifications here in the future
		data->dll_file.seekg(data->GetMethodOffset(rawmodif[i].method_id));
		DllMethodInfo::JumpToCode(data->dll_file);
		HWSWriteLong(ffhws,rawmodif[i].position-data->dll_file.tellg());
		HWSWriteLong(ffhws,rawmodif[i].base_length);
		HWSWriteLong(ffhws,rawmodif[i].new_length);
		ffhws.write((const char*)rawmodif[i].value,rawmodif[i].new_length);
	}
	for (i=0;i<macromodifamount;i++) {
		HWSWriteShort(ffhws,0); // Slot reserved for flags
		HWSWriteLong(ffhws,macromodif[i].type->id);
	}
}
