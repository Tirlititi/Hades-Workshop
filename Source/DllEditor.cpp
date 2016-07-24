#include "DllEditor.h"

#include <algorithm>
#include <cstring>

#define G_N_ELEMENTS(arr) ((sizeof(arr))/(sizeof(arr[0])))

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
	TableType_Module,
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
	TableType_GenericParamConstraint
};

uint32_t ReadShortOrLong(fstream& f, int datasize) {
	if (datasize==2)
		return ReadShort(f);
	return ReadLong(f);
}

int DllMetaData::Load(fstream& f) {
	unsigned int i,j;
	size_t prevpos;
	uint8_t tmp8;
	char strbuffer[256];
	if (ReadShort(f)!=0x5A4D)
		return 1;
	f.seekg(0x3A,ios::cur);
	pe_header_offset = ReadLong(f);
	f.seekg(pe_header_offset);
	if (ReadLong(f)!=0x4550)
		return 1;
	architecture = ReadShort(f);
	section_amount = ReadShort(f);
	f.seekg(14,ios::cur);
	characteristics = ReadShort(f);
	pe_type = ReadShort(f);
	f.seekg(0x42,ios::cur);
	subsystem = ReadShort(f);
	dll_characteristics = ReadShort(f);
	f.seekg(pe_type==0x20B ? 0x98 : 0x88,ios::cur);
	cli_directory.virtual_address = ReadLong(f);
	cli_directory.size = ReadLong(f);
	f.seekg(8,ios::cur);
	section_name = new string[section_amount];
	section_virtual_size = new uint32_t[section_amount];
	section_virtual_offset = new uint32_t[section_amount];
	section_size = new uint32_t[section_amount];
	section_offset = new uint32_t[section_amount];
	for (i=0;i<section_amount;i++) {
		section_name[i] = "";
		f.read(strbuffer,8);
		for (j=0;j<8 && strbuffer[j]!=0;j++)
			section_name[i].push_back(strbuffer[j]);
		section_virtual_size[i] = ReadLong(f);
		section_virtual_offset[i] = ReadLong(f);
		section_size[i] = ReadLong(f);
		section_offset[i] = ReadLong(f);
		f.seekg(0x10,ios::cur);
	}
	f.seekg(GetVirtualAddressOffset(cli_directory.virtual_address));
	f.seekg(8,ios::cur);
	meta_data_directory.virtual_address = ReadLong(f);
	meta_data_directory.size = ReadLong(f);
	cli_attributes = ReadLong(f);
	cli_entry_point_token = ReadLong(f);
	resources_directory.virtual_address = ReadLong(f);
	resources_directory.size = ReadLong(f);
	strong_name_directory.virtual_address = ReadLong(f);
	strong_name_directory.size = ReadLong(f);
	meta_data_section = GetSectionAtVirtualAddress(meta_data_directory.virtual_address);
	f.seekg(GetVirtualAddressOffset(meta_data_directory.virtual_address));
	if (ReadLong(f)!=0x424a5342)
		return 1;
	f.seekg(8,ios::cur);
	meta_data_version_name_size = ReadLong(f);
	f.seekg(meta_data_version_name_size,ios::cur);
	f.seekg(2,ios::cur);
	meta_data_stream_amount = ReadShort(f);
	stream_offset = new uint32_t[meta_data_stream_amount];
	stream_size = new uint32_t[meta_data_stream_amount];
	stream_name = new string[meta_data_stream_amount];
	stream_table_heap = -1;
	stream_string_heap = -1;
	stream_blob_heap = -1;
	stream_guid_heap = -1;
	stream_user_string_heap = -1;
	for (i=0;i<meta_data_stream_amount;i++) {
		stream_offset[i] = ReadLong(f);
		stream_size[i] = ReadLong(f);
		stream_name[i] = "";
		for (tmp8=f.get();tmp8!=0;tmp8=f.get())
			stream_name[i].push_back(tmp8);
		f.seekg(GetAlignOffset(f.tellg()),ios::cur);
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
	f.seekg(GetStreamOffset(stream_string_heap));
	string_heap_raw = new char[stream_size[stream_string_heap]];
	f.read(string_heap_raw,stream_size[stream_string_heap]);
	f.seekg(GetStreamOffset(stream_table_heap));
	f.seekg(6,ios::cur);
	table_heap_sizes = f.get();
	stream_string_heap_index_size = (table_heap_sizes & 1)>0 ? 4 : 2;
	stream_blob_heap_index_size = (table_heap_sizes & 4)>0 ? 4 : 2;
	stream_guid_heap_index_size = (table_heap_sizes & 2)>0 ? 4 : 2;
	for (i=0;i<DLL_TABLE_CODED_INDEX;i++)
		coded_index_size[i] = 0;
	f.seekg(1,ios::cur);
	table_heap_valid = ReadLongLong(f);
	f.seekg(8,ios::cur);
	for (i=0;i<DLL_TABLE_HEAP_COUNT;i++)
		if ((table_heap_valid >> i) & 1)
			table[i].length = ReadLong(f);
		else
			table[i].length = 0;
	ComputeTableInformations((uint32_t)f.tellg()-section_offset[meta_data_section]);
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
	cil_fieldrva_virtual_address = new uint32_t[table[TableType_FieldRVA].length];
	cil_fieldrva_field_id = new uint32_t[table[TableType_FieldRVA].length];
	cil_memberref_class = new uint32_t[table[TableType_MemberRef].length];
	cil_memberref_name_id = new uint32_t[table[TableType_MemberRef].length];
	cil_memberref_signature = new uint32_t[table[TableType_MemberRef].length];
	f.seekg(section_offset[meta_data_section]+table[TableType_TypeDef].offset);
	for (i=0;i<table[TableType_TypeDef].length;i++) {
		cil_type_attributes[i] = ReadLong(f);
		cil_type_name_id[i] = ReadShortOrLong(f,stream_string_heap_index_size);
		cil_type_namespace_id[i] = ReadShortOrLong(f,stream_string_heap_index_size);
		cil_type_basetype[i] = ReadShortOrLong(f,GetCodedIndexSize(CodedIndex_TypeDefOrRef));
		cil_type_field_id[i] = ReadShortOrLong(f,GetTableIndexSize(TableType_Field));
		cil_type_method_id[i] = ReadShortOrLong(f,GetTableIndexSize(TableType_Method));
	}
	f.seekg(section_offset[meta_data_section]+table[TableType_Field].offset);
	for (i=0;i<table[TableType_Field].length;i++) {
		cil_field_flags[i] = ReadShort(f);
		cil_field_name_id[i] = ReadShortOrLong(f,stream_string_heap_index_size);
		cil_field_signature[i] = ReadShortOrLong(f,stream_blob_heap_index_size);
	}
	f.seekg(section_offset[meta_data_section]+table[TableType_Method].offset);
	for (i=0;i<table[TableType_Method].length;i++) {
		cil_method_virtual_address[i] = ReadLong(f);
		cil_method_implflags[i] = ReadShort(f);
		cil_method_flags[i] = ReadShort(f);
		cil_method_name_id[i] = ReadShortOrLong(f,stream_string_heap_index_size);
		cil_method_signature[i] = ReadShortOrLong(f,stream_blob_heap_index_size);
		cil_method_param_id[i] = ReadShortOrLong(f,GetTableIndexSize(TableType_Param));
	}
	f.seekg(section_offset[meta_data_section]+table[TableType_FieldRVA].offset);
	for (i=0;i<table[TableType_FieldRVA].length;i++) {
		cil_fieldrva_virtual_address[i] = ReadLong(f);
		cil_fieldrva_field_id[i] = ReadShortOrLong(f,GetTableIndexSize(TableType_Param));
	}
	f.seekg(section_offset[meta_data_section]+table[TableType_MemberRef].offset);
	for (i=0;i<table[TableType_MemberRef].length;i++) {
		cil_memberref_class[i] = ReadShortOrLong(f,GetCodedIndexSize(CodedIndex_MemberRefParent));
		cil_memberref_name_id[i] = ReadShortOrLong(f,stream_string_heap_index_size);
		cil_memberref_signature[i] = ReadShortOrLong(f,stream_blob_heap_index_size);
	}
	return 0;
}

int DllMetaData::GetMethodIndex(const char* tname, const char* mname, const char* namesp) {
	int i,j,methamount;
	for (i=0;i<table[TableType_TypeDef].length;i++) {
		if (IsNameMatching(cil_type_namespace_id[i],namesp) && IsNameMatching(cil_type_name_id[i],tname)) {
			if (cil_type_method_id[i]==0)
				return -1;
			if (i+1==table[TableType_TypeDef].length)
				methamount = table[TableType_Method].length-cil_type_method_id[i];
			else
				methamount = cil_type_method_id[i+1]-cil_type_method_id[i]-1;
			for (j=-1;j<methamount;j++)
				if (IsNameMatching(cil_method_name_id[cil_type_method_id[i]+j],mname))
					return cil_type_method_id[i]+j;
			return -1;
		}
	}
	return -1;
}

uint32_t DllMetaData::GetMethodOffset(unsigned int methid) {
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

string DllMetaData::GetMethodName(unsigned int methid, bool showtype) {
	string res = "";
	int i;
	if (showtype) {
		for (i=table[TableType_TypeDef].length-1;i>=0;i--)
			if (cil_type_method_id[i]!=0 && methid+1>=cil_type_method_id[i]) {
				if (cil_type_namespace_id[i]>0)
					res += GetNameAtPos(cil_type_namespace_id[i])+".";
				res += GetNameAtPos(cil_type_name_id[i])+"::";
				break;
			}
	}
	res += GetNameAtPos(cil_method_name_id[methid]);
	return res;
}

uint32_t DllMetaData::GetStaticFieldOffset(unsigned int staticfieldid) {
	return GetVirtualAddressOffset(cil_fieldrva_virtual_address[staticfieldid]);
}

uint32_t DllMetaData::GetTypeIdentifier(const char* tname) {
	unsigned int i;
	for (i=0;i<table[TableType_TypeDef].length;i++)
		if (IsNameMatching(cil_type_name_id[i],tname))
			return (i+1) | 0x02000000;
	return 0;
}

uint32_t DllMetaData::GetMemberIdentifier(const char* mname, const char* tname) {
	unsigned int i;
	uint32_t tid = GetTypeIdentifier(tname) & 0xFFFFFF;
	for (i=0;i<table[TableType_MemberRef].length;i++)
		if (cil_memberref_class[i]==tid && IsNameMatching(cil_memberref_name_id[i],mname))
			return (i+1) | 0x0A000000;
	return 0;
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
		return -1;
	return section_offset[section]+address-section_virtual_offset[section];
}

uint32_t DllMetaData::GetStreamOffset(unsigned int stream) {
	return section_offset[meta_data_section]+stream_offset[stream]+meta_data_directory.virtual_address-section_virtual_offset[meta_data_section];
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
	int foudmethod = -1;
	unsigned int i,j;
	uint32_t methbeg;
	uint32_t methend;
	for (i=0;i<table[TableType_Method].length;i++) {
		methend = GetMethodRange(i);
		if (methend>0) {
			methbeg = GetVirtualAddressOffset(cil_method_virtual_address[i]);
			methend += methbeg;
			if (offset>=methbeg && offset<methend) {
				foudmethod = i;
				break;
			}
		}
	}
	if (foudmethod<0)
		return -1;
	uint32_t fieldbeg;
	for (i=0;i<table[TableType_FieldRVA].length;i++) {
		fieldbeg = GetVirtualAddressOffset(cil_fieldrva_virtual_address[i]);
		if (fieldbeg>methbeg && fieldbeg<=offset && fieldbeg<methend)
			return -1;
	}
	return foudmethod;
}

string DllMetaData::GetNameAtPos(uint32_t stroff) {
	string res = "";
	unsigned int i;
	for (i=0;string_heap_raw[stroff+i]!=0;i++)
		res += string_heap_raw[stroff+i];
	return res;
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
int32_t DllMetaDataModification::GetSizeModification(unsigned int modifamount, DllMetaDataModification* modif) {
	int32_t res = (int64_t)new_length-(int64_t)base_length;
	int32_t prevmod = 0;
	unsigned int i;
	if (method_id<0)
		return res+GetAlignOffset(res);
	for (i=0;i<modifamount;i++)
		if (method_id==modif[i].method_id) {
			while (position!=modif[i].position && i<modifamount) {
				prevmod += (int64_t)modif[i].new_length-(int64_t)modif[i].base_length;
				i++;
			}
			if (i+1<modifamount && modif[i+1].method_id==method_id)
				return res;
			return res+GetAlignOffset(res+prevmod);
		}
	return res+GetAlignOffset(res);
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

uint32_t DllMetaData::ApplyOffsetModification(uint32_t offset, unsigned int modifamount, DllMetaDataModification* modif, int32_t baseoffset) {
	if (offset==0)
		return 0;
	uint32_t res = offset;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		if (baseoffset+offset>(int32_t)modif[i].position && baseoffset<(int32_t)modif[i].position)
			res += modif[i].GetSizeModification(modifamount,modif);
	return res;
}

uint32_t DllMetaData::ApplyVirtualOffsetModification(uint32_t baserva, uint32_t newsecoff, unsigned int modifamount, DllMetaDataModification* modif) {
	if (baserva==0)
		return 0;
	uint32_t baseoff = GetVirtualAddressOffset(baserva);
	uint32_t res = baseoff;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		if (baseoff>modif[i].position)
			res += modif[i].GetSizeModification(modifamount,modif);
	return res+newsecoff;
}

uint32_t DllMetaData::ApplySizeModification(uint32_t size, unsigned int modifamount, DllMetaDataModification* modif) {
	uint32_t res = size;
	unsigned int i;
	for (i=0;i<modifamount;i++)
		res += modif[i].GetSizeModification(modifamount,modif);
	return res;
}

uint32_t DllMetaData::WriteMethodSizeModification(fstream& fbase, fstream& fdest, uint32_t methid, unsigned int modifamount, DllMetaDataModification* modif) {
	uint32_t methbeg = GetVirtualAddressOffset(cil_method_virtual_address[methid]);
	uint32_t methend = 0;
	uint32_t buffer;
	uint32_t newsize, basesize = 0;
	unsigned int i;
	bool istiny;
	for (i=1;methend==0;i++) {
		if (methid+i>=table[TableType_Method].length)
			methend = GetVirtualAddressOffset(meta_data_directory.virtual_address);
		else if (cil_method_virtual_address[methid+i]!=0)
			methend = GetVirtualAddressOffset(cil_method_virtual_address[methid+i]);
	}
	for (i=0;i<modifamount;i++)
		if (modif[i].position>methbeg && modif[i].position<methend) {
			if (basesize==0) {
				buffer = ReadLong(fbase);
				istiny = (buffer & 3)==2;
				if (istiny) {
					basesize = ((buffer & 0xFF) >> 2);
					fbase.seekg(-3,ios::cur);
				} else
					basesize = ReadLong(fbase);
				newsize = basesize;
			}
			newsize += (int64_t)modif[i].new_length-(int64_t)modif[i].base_length;
		}
	if (newsize>0) {
		if (istiny && newsize<0x4F) {
			fdest.put(2 | (newsize << 2));
		} else if (istiny) {
			// ToDo: this causes error as it adds bytes in the middle of the file
			// Tiny methods shouldn't be modified...
			WriteShort(fdest,3); // transform to fat
			WriteShort(fdest,8); // max_stack_size
			WriteLong(fdest,newsize);
			WriteLong(fdest,0); // local_var_token
		} else {
			WriteLong(fdest,buffer);
			WriteLong(fdest,newsize);
			WriteLong(fdest,ReadLong(fbase));
		}
	}
	return basesize;
}

int DllMetaData::Duplicate(fstream& fbase, fstream& fdest, unsigned int modifamount, DllMetaDataModification* modif) {
	uint32_t copysize, offtmp, sizetmp, tmp32;
	uint32_t metasectionoff, metasectionvirtoff;
	unsigned int i,j,modifindex;
	uint32_t* newsecvpos, *newsecvsize;
	char* buffer;
	SortDllDataModifications(modifamount,modif);
	for (i=0;i<modifamount;i++)
		modif[i].method_id = GetMethodAtOffset(modif[i].position);
	newsecvpos = new uint32_t[section_amount];
	newsecvsize = new uint32_t[section_amount];
	for (i=0;i<section_amount;i++) {
		if (i<meta_data_section) {
			newsecvpos[i] = section_virtual_offset[i];
			newsecvsize[i] = section_virtual_size[i];
		} else if (i==meta_data_section) {
			newsecvsize[i] = ApplySizeModification(section_virtual_size[i],modifamount,modif);
			newsecvpos[i] = section_virtual_offset[i];
		} else {
			newsecvsize[i] = section_virtual_size[i];
			newsecvpos[i] = newsecvpos[i-1]+newsecvsize[i-1];
			newsecvpos[i] += GetAlignOffset(newsecvpos[i],0x2000);
		}
	}
	fbase.seekg(0);
	copysize = pe_header_offset+0x1C;
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	ReadLong(fbase);
	WriteLong(fdest,newsecvsize[meta_data_section]+GetAlignOffset(newsecvsize[meta_data_section],0x200)); // code size
//	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	
	ReadLong(fbase);
	WriteLong(fdest,newsecvpos[1]); // base of data
//	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	
	ReadLong(fbase);
	tmp32 = newsecvpos[2]+newsecvsize[2];
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32,0x2000)); // image size
//	WriteLong(fdest,ReadLong(fbase));
	
	copysize = (pe_type==0x20B ? 0x44 : 0x34);
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	ReadLong(fbase);
	WriteLong(fdest,newsecvpos[1]); // resource_table.offset
//	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	
	ReadLong(fbase);
	WriteLong(fdest,newsecvpos[2]); // base_relocation_table.offset
//	WriteLong(fdest,ReadLong(fbase));
	
	buffer = new char[0x44];
	fbase.read(buffer,0x44);
	fdest.write(buffer,0x44);
	delete[] buffer;
	
//	tmp32 = ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif);
//	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // cli_directory.virtual_address
	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	for (i=0;i<section_amount;i++) {
		WriteLongLong(fdest,ReadLongLong(fbase));
		ReadLong(fbase);
		ReadLong(fbase);
		WriteLong(fdest,newsecvsize[i]); // section.virtual_size
		WriteLong(fdest,newsecvpos[i]); // section.virtual_address
		if (i==meta_data_section) {
			metasectionvirtoff = newsecvpos[i];
			ReadLong(fbase);
			WriteLong(fdest,newsecvsize[i]+GetAlignOffset(newsecvsize[i],0x200)); // section.size
			metasectionoff = ApplyOffsetModification(ReadLong(fbase),modifamount,modif);
			metasectionoff += GetAlignOffset(metasectionoff,0x200);
			WriteLong(fdest,metasectionoff); // section.offset
		} else {
			WriteLong(fdest,ReadLong(fbase));
			tmp32 = ApplySizeModification(ReadLong(fbase),modifamount,modif);
			WriteLong(fdest,tmp32+GetAlignOffset(tmp32,0x200));
		}
		buffer = new char[0x10];
		fbase.read(buffer,0x10);
		fdest.write(buffer,0x10);
		delete[] buffer;
/*		buffer = new char[0x28];
		fbase.read(buffer,0x28);
		fdest.write(buffer,0x28);
		delete[] buffer;*/
	}
	copysize = GetVirtualAddressOffset(cli_directory.virtual_address)+8-fbase.tellg();
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // meta_data_directory.virtual_address
//	WriteLong(fdest,ReadLong(fbase));
//	ReadLong(fbase);
//	WriteLong(fdest,newsecvsize[meta_data_section]+newsecvpos[meta_data_section]-(tmp32+GetAlignOffset(tmp32))); // meta_data_directory.size
	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	WriteLong(fdest,ReadLong(fbase));
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // resources_directory.virtual_address
//	WriteLong(fdest,ReadLong(fbase));
	
	WriteLong(fdest,ReadLong(fbase));
	
	tmp32 = ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif);
	WriteLong(fdest,tmp32+GetAlignOffset(tmp32)); // strong_name_directory.virtual_address
	WriteLong(fdest,ReadLong(fbase));
	
	modifindex = 0;
	offtmp = GetVirtualAddressOffset(meta_data_directory.virtual_address);
	while (modifindex<modifamount && modif[modifindex].position<offtmp) {
		if (modif[modifindex].method_id>=0) {
			copysize = GetMethodOffset(modif[modifindex].method_id);
			if (copysize>fbase.tellg()) {
				copysize -= fbase.tellg();
				buffer = new char[copysize];
				fbase.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
				sizetmp = WriteMethodSizeModification(fbase,fdest,modif[modifindex].method_id,modifamount,modif);
				tmp32 = fbase.tellg();
			}
		}
		copysize = modif[modifindex].position;
		if (copysize>fbase.tellg()) {
			copysize -= fbase.tellg();
			buffer = new char[copysize];
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
			delete[] buffer;
		}
		fbase.seekg(modif[modifindex].base_length,ios::cur);
		fdest.write((const char*)modif[modifindex].value,modif[modifindex].new_length);
		if (modif[modifindex].method_id>=0 && (modifindex+1>=modifamount || modif[modifindex+1].method_id!=modif[modifindex].method_id)) {
			// DEBUG: alignement is miscalculated here because it doesn't use the trailing 0 of the base method
			// The same miscalculation is made everywhere so the offsets match
			copysize = GetMethodOffset(modif[modifindex].method_id)+GetMethodRange(modif[modifindex].method_id);
			if (copysize>fbase.tellg()) {
				copysize -= fbase.tellg();
				buffer = new char[copysize];
				fbase.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
				while (fdest.tellp()%4!=0)
					fdest.put(0);
			}
		} else if (modif[modifindex].method_id<0) { // Field modifications are assumed to change the whole field
			fbase.seekg(GetAlignOffset(fbase.tellg()),ios::cur);
			while (fdest.tellp()%4!=0)
				fdest.put(0);
		}
		modifindex++;
	}
	copysize = offtmp-fbase.tellg();
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	while (fdest.tellp()%4!=0)
		fdest.put(0);
	copysize = meta_data_version_name_size+0x14;
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	for (i=0;i<meta_data_stream_amount;i++) {
/*		WriteLong(fdest,ApplyOffsetModification(ReadLong(fbase),modifamount,modif,section_offset[meta_data_section]+meta_data_directory.virtual_address-section_virtual_offset[meta_data_section])); // stream_offset
		if (i==stream_table_heap)
			WriteLong(fdest,ApplySizeModification(ReadLong(fbase),modifamount,modif)); // stream_size
		else
			WriteLong(fdest,ReadLong(fbase));
		tmp32 = stream_name[i].length()+1+GetAlignOffset(stream_name[i].length()+1);
		while (tmp32>0) {
			WriteLong(fdest,ReadLong(fbase));
			tmp32 -= 4;
		}*/
		copysize = 0x8+stream_name[i].length()+1+GetAlignOffset(stream_name[i].length()+1);
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
	}
	// Nothing to change in stream_table_heap
	if (table[TableType_Method].length>0) {
		offtmp = section_offset[meta_data_section]+table[TableType_Method].offset;
		copysize = offtmp-fbase.tellg();
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		copysize = table[TableType_Method].row_size-4;
		buffer = new char[copysize];
		for (i=0;i<table[TableType_Method].length;i++) {
			tmp32 = ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif);
			WriteLong(fdest,tmp32); // method.virtual_address
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}
/*	if (table[TableType_FieldLayout].length>0) { // Not sure if offsets are rva here... FieldLayouts are not used anyway
		offtmp = section_offset[meta_data_section]+table[TableType_FieldLayout].offset;
		copysize = offtmp-fbase.tellg();
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		
		copysize = table[TableType_FieldLayout].row_size-4;
//		copysize = table[TableType_FieldLayout].row_size;
		
		buffer = new char[copysize];
		for (i=0;i<table[TableType_FieldLayout].length;i++) {
			WriteLong(fdest,ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif)); // fieldlayout.offset
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}*/
	if (table[TableType_FieldRVA].length>0) {
		offtmp = section_offset[meta_data_section]+table[TableType_FieldRVA].offset;
		copysize = offtmp-fbase.tellg();
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		copysize = table[TableType_FieldRVA].row_size-4;
		buffer = new char[copysize];
		for (i=0;i<table[TableType_FieldRVA].length;i++) {
			WriteLong(fdest,ApplyVirtualOffsetModification(ReadLong(fbase),metasectionvirtoff-metasectionoff,modifamount,modif)); // fieldrva.virtual_address
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
		}
		delete[] buffer;
	}
	// Modifications are assumed to be in the method/fieldrva code
/*	while (modifindex<modifamount) {
		copysize = modif[modifindex].position-fbase.tellg();
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
		fbase.seekg(modif[modifindex].base_length,ios::cur);
		fdest.write((const char*)modif[modifindex].value,modif[modifindex].new_length);
		modifindex++;
	}*/
	for (i=0;i<section_amount;i++) {
		if (section_offset[i]>fbase.tellg()) {
			copysize = section_offset[i]-fbase.tellg();
			buffer = new char[copysize];
			fbase.read(buffer,copysize);
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
				fbase.read(buffer,copysize);
				fdest.write(buffer,copysize);
				delete[] buffer;
				WriteLong(fdest,ReadLong(fbase)+newsecvpos[i]-section_virtual_offset[i]);
			}
		}
	}
	copysize = section_offset[section_amount-1]+section_size[section_amount-1]-fbase.tellg();
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	delete[] newsecvpos;
	delete[] newsecvsize;
	return 0;
}

//==================================================//
//                     IL CODE                      //
//==================================================//

// Method header
uint32_t DllMethodInfo::JumpToCode(fstream& f) {
	uint32_t buffer, res = 0;
	buffer = f.get();	res++;
	if ((buffer & 3)==3)
		f.seekg(11,ios::cur);	res += 11;
	return res;
}

// Code
#define IL_CODE_DUP			0x25
#define IL_CODE_STSFLD		0x80
#define IL_CODE_NEWARR		0x8D
#define IL_CODE_STELEMREF	0xA2

struct ILCode {
	uint8_t code;
	string name;
	int size;
};

static ILCode ILCodeList[] = {
	{ 0x14, "ldnull", 0 },
	{ 0x15, "ldc.i4.m1", 0 },
	{ 0x16, "ldc.i4.0", 0 },
	{ 0x17, "ldc.i4.1", 0 },
	{ 0x18, "ldc.i4.2", 0 },
	{ 0x19, "ldc.i4.3", 0 },
	{ 0x1A, "ldc.i4.4", 0 },
	{ 0x1B, "ldc.i4.5", 0 },
	{ 0x1C, "ldc.i4.6", 0 },
	{ 0x1D, "ldc.i4.7", 0 },
	{ 0x1E, "ldc.i4.8", 0 },
	{ 0x1F, "ldc.i4.s", 1 },
	{ 0x20, "ldc.i4", 4 },
	
	{ 0x25, "dup", 0 },
	
	{ 0x28, "call", 4 },
	
	{ 0x67, "conv.i1", 0 },
	{ 0x68, "conv.i2", 0 },
	{ 0x69, "conv.i4", 0 },
	{ 0x6A, "conv.i8", 0 },
	
	{ 0x72, "ldstr", 4 },
	{ 0x73, "newobj", 4 },
	
	{ 0x80, "stsfld", 4 },
	
	{ 0x8D, "newarr", 4 },
	
	{ 0x9B, "stelem.i", 0 },
	{ 0x9C, "stelem.i1", 0 },
	{ 0x9D, "stelem.i2", 0 },
	{ 0x9E, "stelem.i4", 0 },
	{ 0x9F, "stelem.i8", 0 },
	{ 0xA0, "stelem.r4", 0 },
	{ 0xA1, "stelem.r8", 0 },
	{ 0xA2, "stelem.ref", 0 },
	
	{ 0xD0, "ldtoken", 4 },
	
	{ 0xFF, "invalid", 0 }
};

static ILCode& GetILCode(uint8_t code) {
	for (unsigned int i=0;G_N_ELEMENTS(ILCodeList);i++)
		if (ILCodeList[i].code==code)
			return ILCodeList[i];
	return ILCodeList[G_N_ELEMENTS(ILCodeList)-1];
};

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

bool debug_il_integer_is_string; // debug
int64_t ILGetNextIntegerValue(fstream& f, uint32_t* integercodepos = NULL) {
	uint8_t code;
	int64_t argvalue;
	debug_il_integer_is_string = false;
	do {
		if (integercodepos!=NULL)
			*integercodepos = f.tellg();
		code = f.get();
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
			argvalue = f.get();
			return argvalue;
		} else if (code==0x20) {
			argvalue = f.get();
			argvalue |= (f.get() << 8);
			argvalue |= (f.get() << 16);
			argvalue |= (f.get() << 24);
			return argvalue;
		} else if (code==0x72) {
			argvalue = f.get();
			argvalue |= (f.get() << 8);
			argvalue |= (f.get() << 16);
			argvalue |= (f.get() << 24);
			debug_il_integer_is_string = true;
			return 1; // ToDo: convert string to ID
		}
		f.seekg(GetILCode(code).size,ios::cur);
	} while (true);
}

uint8_t* ConvertILScriptToRawData_Object(fstream& f, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[]) {
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
		arrindex = ILGetNextIntegerValue(f);
		curobjpos = 0;
		for (j=0;j<fieldamount;j++) {
			if (array==NULL || array[j]==0) {
				argvalue = ILGetNextIntegerValue(f);
				ILStoreValueToRawData(res,arrindex*objsize+curobjpos,argvalue,fieldsize[j]);
				curobjpos += fieldsize[j];
			} else {
				do {
					code = f.get();
					f.seekg(GetILCode(code).size,ios::cur);
				} while (code!=IL_CODE_NEWARR);
				code = f.get();
				while (code==IL_CODE_DUP) {
					arrindexsub = ILGetNextIntegerValue(f);
					argvalue = ILGetNextIntegerValue(f);
					curobjpossub = 0;
					for (k=0;k<arrindexsub;k++)
						curobjpossub += fieldsize[j+k];
					ILStoreValueToRawData(res,arrindex*objsize+curobjpos+curobjpossub,argvalue,fieldsize[j+arrindexsub]);
					code = f.get(); // stelem
					f.seekg(GetILCode(code).size,ios::cur);
					code = f.get();
				}
				f.seekg(-1,ios::cur);
				for (k=0;k<array[j];k++)
					curobjpos += fieldsize[j+k];
				j += array[j]-1;
			}
		}
		do {
			code = f.get();
			f.seekg(GetILCode(code).size,ios::cur);
		} while (code!=IL_CODE_STELEMREF);
	}
	return res;
}

int64_t** ConvertILScriptToRawData_Array2(fstream& f, unsigned int objamount, unsigned int arrayamount) {
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
		arrindex1 = ILGetNextIntegerValue(f);
		arrindex2 = ILGetNextIntegerValue(f);
		argvalue = ILGetNextIntegerValue(f);
		res[arrindex1][arrindex2] = argvalue;
		code = f.get(); // Should be "call array::Set"
		f.seekg(GetILCode(code).size,ios::cur);
		code = f.get();
		f.seekg(GetILCode(code).size,ios::cur);
	} while (code!=IL_CODE_STSFLD);
	f.seekg(-5,ios::cur); // End at the STSFLD position
	return res;
}

uint32_t ILScriptJumpToInstructions(fstream& f, unsigned int amount, ILInstruction* inst) {
	uint32_t tmppos,basepos = f.tellg();
	unsigned int i;
	uint8_t opcode;
	while (!f.eof()) {
		tmppos = (unsigned int)f.tellg()+1;
		for (i=0;i<amount;i++) {
			opcode = f.get();
			if (opcode!=inst[i].opcode)
				break;
			if (GetILCode(opcode).size==1) {
				if (f.get()!=inst[i].param)
					break;
			} else if (GetILCode(opcode).size==2) {
				if (ReadShort(f)!=inst[i].param)
					break;
			} else if (GetILCode(opcode).size==4) {
				if (ReadLong(f)!=inst[i].param)
					break;
			}
		}
		if (i==amount)
			return (unsigned int)f.tellg()-basepos;
		else if (f.tellg()!=tmppos)
			f.seekg(tmppos);
	}
	return 0;
}

uint8_t buffertmp[0x100000];
DllMetaDataModification ModifyILScript_Object(fstream& f, uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int fieldamount, const unsigned int fieldsize[], const unsigned int array[]) {
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
			macrotmp = f.tellg(); \
			f.seekg(FROM); \
			buffersize = TO-FROM; \
			buffer = new char[buffersize]; \
			f.read(buffer,buffersize); \
			memcpy(buffertmp+posbuffer,buffer,buffersize); \
			posbuffer += buffersize; \
			f.seekg(macrotmp); \
		}
	
	f.seekg(pos);
	for (i=0;i<objamount;i++) {
		copystart = f.tellg();
		ILGetNextIntegerValue(f,&copyend);
		MACRO_ILSCRIPT_COPY(copystart,copyend)
		MACRO_ILSCRIPT_WRITE_INTEGER(i,minusone32)
		for (j=0;j<fieldamount;j++) {
			if (array==NULL || array[j]==0) {
				copystart = f.tellg();
				k = ILGetNextIntegerValue(f,&copyend);
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
				copystart = f.tellg();
				do {
					code = f.get();
					f.seekg(GetILCode(code).size,ios::cur);
				} while (code!=IL_CODE_NEWARR);
				copyend = f.tellg();
				MACRO_ILSCRIPT_COPY(copystart,copyend)
				// Do not copy array
				code = f.get();
				while (code==IL_CODE_DUP) {
					ILGetNextIntegerValue(f); // array sub-index
					ILGetNextIntegerValue(f); // argument value
					code = f.get(); // stelem
					f.seekg(GetILCode(code).size,ios::cur);
					code = f.get();
				}
				f.seekg(-1,ios::cur);
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
		copystart = f.tellg();
		do {
			code = f.get();
			f.seekg(GetILCode(code).size,ios::cur);
		} while (code!=IL_CODE_STELEMREF);
		copyend = f.tellg();
		MACRO_ILSCRIPT_COPY(copystart,copyend)
	}
	res.new_length = posbuffer;
	res.value = new uint8_t[res.new_length];
	memcpy(res.value,buffertmp,res.new_length);
	res.position = pos;
	res.base_length = baselen;
	return res;
}

DllMetaDataModification ModifyILScript_Array2(fstream& f, uint32_t** newfieldvalues, uint32_t pos, uint32_t baselen, unsigned int objamount, unsigned int arrayamount) {
	unsigned int i,j,posbuffer;
	uint32_t minusone32 = 0xFFFFFFFF;
	char* buffer;
	DllMetaDataModification res;
	posbuffer = 0;
	for (i=0;i<objamount;i++) {
		for (j=0;j<arrayamount;j++) {
			if (newfieldvalues[i][j]!=0) {
				MACRO_ILSCRIPT_WRITE_INTEGER(i,minusone32)
				MACRO_ILSCRIPT_WRITE_INTEGER(j,minusone32)
				MACRO_ILSCRIPT_WRITE_INTEGER(newfieldvalues[i][j],minusone32)
				buffertmp[posbuffer++] = 0x28; // call
				BufferInitPosition(posbuffer);
				BufferWriteLong(buffertmp,0x0A0001ED); // array::Set
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
