#ifndef _DATABASE_ASSEMBLY_H
#define _DATABASE_ASSEMBLY_H

#include "Database_Common.h"

//====================================//
//             MIPS Script            //
//====================================//

struct MipsHelpField {
	wstring label;
	wstring help;
};

extern vector<MipsHelpField> MIPS_HELP;

//====================================//
//              CIL Script            //
//====================================//

#define IL_CODE_DUP			0x25
#define IL_CODE_STSFLD		0x80
#define IL_CODE_NEWARR		0x8D
#define IL_CODE_STELEMREF	0xA2
#define IL_CODEINDEX_EXTENDED	0xC0

#define ILT_NONE		0
#define ILT_INT			1
#define ILT_UINT		2
#define ILT_FLOAT		3
#define ILT_OFFSET		4
#define ILT_STRING		5
#define ILT_METHOD		6
#define ILT_CLASS		7
#define ILT_CTOR		8
#define ILT_FIELD		9
#define ILT_TYPE		10
#define ILT_CALLSITE	11
#define ILT_TOKEN		12
#define ILT_ETYPE		13
#define ILT_VALUETYPE	14
#define ILT_TYPETOK		15
#define ILT_THISTYPE	16
#define ILT_UNALIGN		17
#define ILT_NOCHECK		18
#define ILT_SWITCH		19

struct ILCode {
	uint16_t code;
	string name;
	int size;
	uint8_t type;
};

extern vector<ILCode> ILCodeList;
ILCode& GetILCode(uint16_t code);

#endif
