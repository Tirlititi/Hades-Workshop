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

static MipsHelpField MIPS_HELP[] = {
	{ L"Introduction",	L"MIPS is the deepest coding language, shared by all the PSX games. As such, modifying it is extremely dangerous unless you know what you are doing.\n\n"
						L"This help only gives a few hints about modifying Final Fantasy IX's MIPS code. For a general documentation about MIPS, you can go to :\n"
						L"http://cgi.cse.unsw.edu.au/~cs3231/doc/R3000.pdf\n\n"
						L"You may find some basic operations done tediously. In particular, multiplying by constant numbers such as 100 is done by default with lines like these :\n"
						L" $2 = $1 << 1\n"
						L" $2 = $2 + $1\n"
						L" $2 = $2 << 3\n"
						L" $2 = $1 + $2\n"
						L" $1 = $2 << 2\n"
						L"This is because raw multiplications are considered slow and are avoided when possible. This kind of optimisation appears in scripts that are not looping (and thus don't cause slowness problems).\n"
						L"You may replace those by raw multiplications to get some space and don't bother doing this optimisation in your own code.\n\n"
						L"One other very important thing to note is that the jumps are always delayed by 1 instruction. That means the line following a jump is executed before actually jumping.\n" },
	{ L"Battle - Function Dependency",	L"Spell effects are parts of the whole battle code. It uses references to other parts of this code and sometimes to other spell effects.\n"
										L"Apart from the effects which are strictly the same, here is a list of the dependencies between spell effects :\n\n"
										L"%9 is partially used by %19 and %20\n"
										L"%11 is used by %108 and %109\n"
										L"%15 is partially used by %69\n"
										L"%16 is partially used by %26, %28 and %83\n"
										L"%17 is partially used by %91\n"
										L"%24 is partially used by %22 and %23\n"
										L"%26 is used by %28\n"
										L"%27 is partially used by %25\n"
										L"%32 is partially used by %17 and %91\n"
										L"%48 is partially used by %42\n"
										L"%50 is partially used by %14\n"
										L"%63 is partially used by %68\n"
										L"%65 is partially used by %51\n"
										L"%66 is partially used by %53\n"
										L"%69 is partially used by %10\n"
										L"%71 is partially used by %50\n"
										L"%72 is partially used by %13 and %71\n"
										L"%73 is used by %62 and %72\n"
										L"%93 is partially used by %76\n"
										L"%98 is partially used by %18, %21, %28, %41, %52, %54, %63 and %97\n"
										L"%99 is partially used by %74 and %75\n"
										L"%101 is partially used by %58\n"
										L"%102 is partially used by %101\n"
										L"%103 is partially used by %109\n"
										L"%104 is partially used by %38, %66, %67 and %68\n"
										L"%107 is partially used by %8 and %85\n\n"
										L"In addition to those, Apply Effect is called by most of spell effects."},
	{ L"Battle - Apply Effect",	L"Apply Effect allows to deal damage, add or remove a status or simply update the state of the battle situation.\n"
								L"Its arguments are set in the memory addresses $29[...]. Here is a list of those arguments, with the byte size in parentheses :\n\n"
								L"24(4) : Caster ID\n"
								L"28(4) : Target ID\n"
								L"32(4) : Maybe Spell Instance ID\n"
								L"38(2) : Damage Formula - Base\n"
								L"40(2) : Damage Formula - Defence\n"
								L"44(2) : Damage Formula - Bonus\n"
								L"48(2) : Effect Flags (Miss ; Dodge ; MP Attack ; Absorb ; ??? ; ??? ; Guard ; Set HP ; Add Status ; - last bits unused -)\n"
								L"50(1) : Caster Flags (HP Damage ; HP Heal ; Critical Message ; MP Damage ; MP Heal ; Miss Message ; Death Message ; Guard Message )\n"
								L"51(1) : Target Flags (HP Damage ; HP Heal ; Critical Message ; MP Damage ; MP Heal ; Miss Message ; Death Message ; Guard Message )\n"
								L"52(2) : Caster HP Damage\n"
								L"54(2) : Target HP Damage\n"
								L"56(2) : Caster MP Damage\n"
								L"58(2) : Target MP Damage\n" },
	{ L"Battle - Data Access",	L"MIPS code can always access to everything in the RAM... provided you know where to seek !\n"
								L"In the following list of useful datas for spell effects, the numbers in parentheses indicate the byte sizes and thus which mask you must use between 0xFF, 0xFFFF and 0xFFFFFFFF.\n\n"
								L"Spell Stats :\n"
								L" $1 = $17[8]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 1 for its animation(2)\n"
								L" 4 for its effect(1)\n"
								L" 5 for its power(1)\n"
								L" 6 for its element(1)\n"
								L" 7 for its accuracy(1)\n"
								L" 8 for its flags(1)\n"
								L" 9 for its status set(1)\n"
								L" 10 for its mp cost(1)\n"
								L" 12 for its alternate animation(2)\n\n"
								L"Caster Stats :\n"
								L" $1 = $21[N] & mask\n"
								L"Target Stats :\n"
								L" $1 = $19[N] & mask\n"
								L"with N replaced by :\n"
								L" 36 for max HP(2)\n"
								L" 38 for max MP(2)\n"
								L" 40 for HP(2)\n"
								L" 42 for MP(2)\n"
								L" 48 for speed(1)\n"
								L" 49 for strength(1)\n"
								L" 50 for magic(1)\n"
								L" 51 for spirit(1)\n"
								L" 60 for status(3)\n"
								L" 70 for category(1)\n"
								L" 108 for defence(1)\n"
								L" 109 for evade(1)\n"
								L" 110 for magic defence(1)\n"
								L" 111 for magic evade(1)\n"
								L" 112 for elemental immunity(1)\n"
								L" 113 for elemental absorption(1)\n"
								L" 114 for elemental resistance(1)\n"
								L" 115 for elemental weakness(1)\n"
								L" 116 for weapon ID(4)\n"
								L" 120 for trance gauge(1)\n"
								L" 121 for elemental boost(1)\n"
								L" 122 for level(1)\n"
								L" 196 for activate supporting abilities(8)\n\n"
								L"Weapon Stats :\n"
								L" $1 = $19[116] or $21[116]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 0 for its flags(1)\n"
								L" 1 for its status(1)\n"
								L" 2 for its model(2)\n"
								L" 4 for its damage formula(1)\n"
								L" 5 for its attack power(1)\n"
								L" 6 for its element(1)\n"
								L" 7 for its status accuracy(1)\n\n"
								L"Random Number (between 0 and 0xFFFFFFFF):\n"
								L" $16 = 32776 << 16\n"
								L" $1 = $16[-17448]\n"
								L" jump 0x4C600 (store return position to $31)\n"
								L" nothing\n\n"
								L"Other :\n"
								L" $16 : spell effect ID\n" },
	{ L"Generic - Data Access",	L"MIPS code can always access to everything in the RAM... provided you know where to seek !\n"
								L"In the following list of useful datas that are loaded in the RAM most of the time, the numbers in parentheses indicate the byte sizes.\n\n"
								L"General VAR_B variables :\n"
								L" $1 = 32776 << 16\n"
								L" $1 = $1 + 45992\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by the wanted variable ID (see the script help for a list of general variables)\n\n"
								L"Others :\n"
								L" $1 = 32775 << 16\n"
								L" $1 = $1[-32732]\n"
								L" $1 = $1[28]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 1376 for gil amount(4)\n"
								L" 1384 for catched frog amount(2)\n"
								L" 1386 for successful steal amount(2)\n"
								L" 1388 for dragon slain amount(2)\n" },
};

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

static ILCode ILCodeList[] = {
	{ 0x00, "nop", 0, ILT_NONE },
	{ 0x01, "break", 0, ILT_NONE },
	{ 0x02, "ldarg.0", 0, ILT_NONE },
	{ 0x03, "ldarg.1", 0, ILT_NONE },
	{ 0x04, "ldarg.2", 0, ILT_NONE },
	{ 0x05, "ldarg.3", 0, ILT_NONE },
	{ 0x06, "ldloc.0", 0, ILT_NONE },
	{ 0x07, "ldloc.1", 0, ILT_NONE },
	{ 0x08, "ldloc.2", 0, ILT_NONE },
	{ 0x09, "ldloc.3", 0, ILT_NONE },
	{ 0x0A, "stloc.0", 0, ILT_NONE },
	{ 0x0B, "stloc.1", 0, ILT_NONE },
	{ 0x0C, "stloc.2", 0, ILT_NONE },
	{ 0x0D, "stloc.3", 0, ILT_NONE },
	{ 0x0E, "ldarg.s", 1, ILT_UINT },
	{ 0x0F, "ldarga.s", 1, ILT_UINT },
	{ 0x10, "starg.s", 1, ILT_UINT },
	{ 0x11, "ldloc.s", 1, ILT_UINT },
	{ 0x12, "ldloca.s", 1, ILT_UINT },
	{ 0x13, "stloc.s", 1, ILT_UINT },
	{ 0x14, "ldnull", 0, ILT_NONE },
	{ 0x15, "ldc.i4.m1", 0, ILT_NONE },
	{ 0x16, "ldc.i4.0", 0, ILT_NONE },
	{ 0x17, "ldc.i4.1", 0, ILT_NONE },
	{ 0x18, "ldc.i4.2", 0, ILT_NONE },
	{ 0x19, "ldc.i4.3", 0, ILT_NONE },
	{ 0x1A, "ldc.i4.4", 0, ILT_NONE },
	{ 0x1B, "ldc.i4.5", 0, ILT_NONE },
	{ 0x1C, "ldc.i4.6", 0, ILT_NONE },
	{ 0x1D, "ldc.i4.7", 0, ILT_NONE },
	{ 0x1E, "ldc.i4.8", 0, ILT_NONE },
	{ 0x1F, "ldc.i4.s", 1, ILT_INT },
	{ 0x20, "ldc.i4", 4, ILT_INT },
	{ 0x21, "ldc.i8", 8, ILT_INT },
	{ 0x22, "ldc.r4", 4, ILT_FLOAT },
	{ 0x23, "ldc.r8", 8, ILT_FLOAT },
	
	{ 0x25, "dup", 0, ILT_NONE },
	{ 0x26, "pop", 0, ILT_NONE },
	{ 0x27, "jmp", 4, ILT_METHOD },
	{ 0x28, "call", 4, ILT_METHOD },
	{ 0x29, "calli", 4, ILT_CALLSITE }, // callsitedescr
	{ 0x2A, "ret", 0, ILT_NONE },
	{ 0x2B, "br.s", 1, ILT_OFFSET },
	{ 0x2C, "brfalse.s", 1, ILT_OFFSET },
	{ 0x2D, "brtrue.s", 1, ILT_OFFSET },
	{ 0x2E, "beq.s", 1, ILT_OFFSET },
	{ 0x2F, "bge.s", 1, ILT_OFFSET },
	{ 0x30, "bgt.s", 1, ILT_OFFSET },
	{ 0x31, "ble.s", 1, ILT_OFFSET },
	{ 0x32, "blt.s", 1, ILT_OFFSET },
	{ 0x33, "bne.un.s", 1, ILT_OFFSET },
	{ 0x34, "bge.un.s", 1, ILT_OFFSET },
	{ 0x35, "bgt.un.s", 1, ILT_OFFSET },
	{ 0x36, "ble.un.s", 1, ILT_OFFSET },
	{ 0x37, "blt.un.s", 1, ILT_OFFSET },
	{ 0x38, "br", 4, ILT_OFFSET },
	{ 0x39, "brfalse", 4, ILT_OFFSET },
	{ 0x3A, "brtrue", 4, ILT_OFFSET },
	{ 0x3B, "beq", 4, ILT_OFFSET },
	{ 0x3C, "bge", 4, ILT_OFFSET },
	{ 0x3D, "bgt", 4, ILT_OFFSET },
	{ 0x3E, "ble", 4, ILT_OFFSET },
	{ 0x3F, "blt", 4, ILT_OFFSET },
	{ 0x40, "bne.un", 4, ILT_OFFSET },
	{ 0x41, "bge.un", 4, ILT_OFFSET },
	{ 0x42, "bgt.un", 4, ILT_OFFSET },
	{ 0x43, "ble.un", 4, ILT_OFFSET },
	{ 0x44, "blt.un", 4, ILT_OFFSET },
	{ 0x45, "switch", -1, ILT_SWITCH },
	{ 0x46, "ldind.i1", 0, ILT_NONE },
	{ 0x47, "ldind.u1", 0, ILT_NONE },
	{ 0x48, "ldind.i2", 0, ILT_NONE },
	{ 0x49, "ldind.u2", 0, ILT_NONE },
	{ 0x4A, "ldind.i4", 0, ILT_NONE },
	{ 0x4B, "ldind.u4", 0, ILT_NONE },
	{ 0x4C, "ldind.i8", 0, ILT_NONE },
	{ 0x4D, "ldind.i", 0, ILT_NONE },
	{ 0x4E, "ldind.r4", 0, ILT_NONE },
	{ 0x4F, "ldind.r8", 0, ILT_NONE },
	{ 0x50, "ldind.ref", 0, ILT_NONE },
	{ 0x51, "stind.ref", 0, ILT_NONE },
	{ 0x52, "stind.i1", 0, ILT_NONE },
	{ 0x53, "stind.i2", 0, ILT_NONE },
	{ 0x54, "stind.i4", 0, ILT_NONE },
	{ 0x55, "stind.i8", 0, ILT_NONE },
	{ 0x56, "stind.r4", 0, ILT_NONE },
	{ 0x57, "stind.r8", 0, ILT_NONE },
	{ 0x58, "add", 0, ILT_NONE },
	{ 0x59, "sub", 0, ILT_NONE },
	{ 0x5A, "mul", 0, ILT_NONE },
	{ 0x5B, "div", 0, ILT_NONE },
	{ 0x5C, "div.un", 0, ILT_NONE },
	{ 0x5D, "rem", 0, ILT_NONE },
	{ 0x5E, "rem.un", 0, ILT_NONE },
	{ 0x5F, "and", 0, ILT_NONE },
	{ 0x60, "or", 0, ILT_NONE },
	{ 0x61, "xor", 0, ILT_NONE },
	{ 0x62, "shl", 0, ILT_NONE },
	{ 0x63, "shr", 0, ILT_NONE },
	{ 0x64, "shr.un", 0, ILT_NONE },
	{ 0x65, "neg", 0, ILT_NONE },
	{ 0x66, "not", 0, ILT_NONE },
	{ 0x67, "conv.i1", 0, ILT_NONE },
	{ 0x68, "conv.i2", 0, ILT_NONE },
	{ 0x69, "conv.i4", 0, ILT_NONE },
	{ 0x6A, "conv.i8", 0, ILT_NONE },
	{ 0x6B, "conv.r4", 0, ILT_NONE },
	{ 0x6C, "conv.r8", 0, ILT_NONE },
	{ 0x6D, "conv.u4", 0, ILT_NONE },
	{ 0x6E, "conv.u8", 0, ILT_NONE },
	{ 0x6F, "callvirt", 4, ILT_METHOD },
	{ 0x70, "cpobj", 4, ILT_TYPETOK }, // typeTok
	{ 0x71, "ldobj", 4, ILT_TYPETOK }, // typeTok
	{ 0x72, "ldstr", 4, ILT_STRING },
	{ 0x73, "newobj", 4, ILT_CTOR },
	{ 0x74, "castclass", 4, ILT_CLASS }, // class
	{ 0x75, "isinst", 4, ILT_CLASS }, // class
	{ 0x76, "conv.r.un", 0, ILT_NONE },
	
	{ 0x79, "unbox", 4, ILT_VALUETYPE }, // valuetype
	{ 0x7A, "throw", 0, ILT_NONE },
	{ 0x7B, "ldfld", 4, ILT_FIELD },
	{ 0x7C, "ldflda", 4, ILT_FIELD },
	{ 0x7D, "stfld", 4, ILT_FIELD },
	{ 0x7E, "ldsfld", 4, ILT_FIELD },
	{ 0x7F, "ldsflda", 4, ILT_FIELD },
	{ 0x80, "stsfld", 4, ILT_FIELD },
	{ 0x81, "stobj", 4, ILT_TYPETOK }, // typeTok
	{ 0x82, "conv.ovf.i1.un", 0, ILT_NONE },
	{ 0x83, "conv.ovf.i2.un", 0, ILT_NONE },
	{ 0x84, "conv.ovf.i4.un", 0, ILT_NONE },
	{ 0x85, "conv.ovf.i8.un", 0, ILT_NONE },
	{ 0x86, "conv.ovf.u1.un", 0, ILT_NONE },
	{ 0x87, "conv.ovf.u2.un", 0, ILT_NONE },
	{ 0x88, "conv.ovf.u4.un", 0, ILT_NONE },
	{ 0x89, "conv.ovf.u8.un", 0, ILT_NONE },
	{ 0x8A, "conv.ovf.i.un", 0, ILT_NONE },
	{ 0x8B, "conv.ovf.u.un", 0, ILT_NONE },
	{ 0x8C, "box", 4, ILT_TYPETOK }, // typeTok
	{ 0x8D, "newarr", 4, ILT_ETYPE },
	{ 0x8E, "ldlen", 0, ILT_NONE },
	{ 0x8F, "ldelema", 4, ILT_CLASS }, // class
	{ 0x90, "ldelem.i1", 0, ILT_NONE },
	{ 0x91, "ldelem.u1", 0, ILT_NONE },
	{ 0x92, "ldelem.i2", 0, ILT_NONE },
	{ 0x93, "ldelem.u2", 0, ILT_NONE },
	{ 0x94, "ldelem.i4", 0, ILT_NONE },
	{ 0x95, "ldelem.u4", 0, ILT_NONE },
	{ 0x96, "ldelem.u8", 0, ILT_NONE },
	{ 0x97, "ldelem.i", 0, ILT_NONE },
	{ 0x98, "ldelem.r4", 0, ILT_NONE },
	{ 0x99, "ldelem.r8", 0, ILT_NONE },
	{ 0x9A, "ldelem.ref", 0, ILT_NONE },
	{ 0x9B, "stelem.i", 0, ILT_NONE },
	{ 0x9C, "stelem.i1", 0, ILT_NONE },
	{ 0x9D, "stelem.i2", 0, ILT_NONE },
	{ 0x9E, "stelem.i4", 0, ILT_NONE },
	{ 0x9F, "stelem.i8", 0, ILT_NONE },
	{ 0xA0, "stelem.r4", 0, ILT_NONE },
	{ 0xA1, "stelem.r8", 0, ILT_NONE },
	{ 0xA2, "stelem.ref", 0, ILT_NONE },
	{ 0xA3, "ldelem", 4, ILT_TYPETOK }, // typeTok
	{ 0xA4, "stelem", 4, ILT_TYPETOK }, // typeTok
	{ 0xA5, "unbox.any", 4, ILT_TYPETOK }, // typeTok
	
	{ 0xB3, "conv.ovf.i1", 0, ILT_NONE },
	{ 0xB4, "conv.ovf.u1", 0, ILT_NONE },
	{ 0xB5, "conv.ovf.i2", 0, ILT_NONE },
	{ 0xB6, "conv.ovf.u2", 0, ILT_NONE },
	{ 0xB7, "conv.ovf.i4", 0, ILT_NONE },
	{ 0xB8, "conv.ovf.u4", 0, ILT_NONE },
	{ 0xB9, "conv.ovf.i8", 0, ILT_NONE },
	{ 0xBA, "conv.ovf.u8", 0, ILT_NONE },
	
	{ 0xC2, "refanyval", 4, ILT_TYPE }, // type
	{ 0xC3, "ckfinite", 0, ILT_NONE },
	
	{ 0xC6, "mkrefany", 4, ILT_CLASS }, // class
	
	{ 0xD0, "ldtoken", 4, ILT_TOKEN },
	{ 0xD1, "conv.u2", 0, ILT_NONE },
	{ 0xD2, "conv.u1", 0, ILT_NONE },
	{ 0xD3, "conv.i", 0, ILT_NONE },
	{ 0xD4, "conv.ovf.i", 0, ILT_NONE },
	{ 0xD5, "conv.ovf.u", 0, ILT_NONE },
	{ 0xD6, "add.ovf", 0, ILT_NONE },
	{ 0xD7, "add.ovf.un", 0, ILT_NONE },
	{ 0xD8, "mul.ovf", 0, ILT_NONE },
	{ 0xD9, "mul.ovf.un", 0, ILT_NONE },
	{ 0xDA, "sub.ovf", 0, ILT_NONE },
	{ 0xDB, "sub.ovf.un", 0, ILT_NONE },
	{ 0xDC, "endfault", 0, ILT_NONE },
	{ 0xDD, "leave", 4, ILT_OFFSET },
	{ 0xDE, "leave.s", 1, ILT_OFFSET },
	{ 0xDF, "stind.i", 0, ILT_NONE },
	{ 0xE0, "conv.u", 0, ILT_NONE },
	
	{ 0xFE, "EXTENDED", 0, ILT_NONE },
	{ 0xFE00, "arglist", 0, ILT_NONE },
	{ 0xFE01, "ceq", 0, ILT_NONE },
	{ 0xFE02, "cgt", 0, ILT_NONE },
	{ 0xFE03, "cgt.un", 0, ILT_NONE },
	{ 0xFE04, "clt", 0, ILT_NONE },
	{ 0xFE05, "clt.un", 0, ILT_NONE },
	{ 0xFE06, "ldftn", 4, ILT_METHOD },
	{ 0xFE07, "ldvirtftn", 4, ILT_METHOD },
	
	{ 0xFE09, "ldarg", 2, ILT_UINT },
	{ 0xFE0A, "ldarga", 2, ILT_UINT },
	{ 0xFE0B, "starg", 2, ILT_UINT },
	{ 0xFE0C, "ldloc", 2, ILT_UINT },
	{ 0xFE0D, "ldloca", 2, ILT_UINT },
	{ 0xFE0E, "stloc", 2, ILT_UINT },
	{ 0xFE0F, "localloc", 0, ILT_NONE },
	
	{ 0xFE11, "endfilter", 0, ILT_NONE },
	{ 0xFE12, "unaligned.", 1, ILT_UNALIGN }, // 1, 2 pr 4
	{ 0xFE13, "volatile.", 0, ILT_NONE },
	{ 0xFE14, "tail.", 0, ILT_NONE },
	{ 0xFE15, "initobj", 4, ILT_TYPETOK }, // typeTok
	{ 0xFE16, "constrained.", 4, ILT_THISTYPE }, // thisType
	{ 0xFE17, "cpblk", 0, ILT_NONE },
	{ 0xFE18, "initblk", 0, ILT_NONE },
	{ 0xFE19, "no.", 1, ILT_NOCHECK }, // typecheck|rangecheck|nullcheck
	{ 0xFE1A, "rethrow", 0, ILT_NONE },
	
	{ 0xFE1C, "sizeof", 4, ILT_TYPETOK }, // typeTok
	{ 0xFE1D, "refanytype", 0, ILT_NONE },
	{ 0xFE1E, "readonly.", 0, ILT_NONE },
	{ 0xFF, "INVALID", 0, ILT_NONE }
};

static ILCode& GetILCode(uint16_t code) {
	unsigned int i;
	if ((code >> 8)==0xFE)
		for (i=IL_CODEINDEX_EXTENDED;i<G_N_ELEMENTS(ILCodeList);i++)
			if (ILCodeList[i].code==code)
				return ILCodeList[i];
	for (i=0;i<IL_CODEINDEX_EXTENDED;i++)
		if (ILCodeList[i].code==code)
			return ILCodeList[i];
	return ILCodeList[G_N_ELEMENTS(ILCodeList)-1];
};

#endif
