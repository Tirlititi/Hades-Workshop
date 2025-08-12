#ifndef _FILE_IOCOMMON_H
#define _FILE_IOCOMMON_H

#include <inttypes.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

uint64_t ReadLongLong(fstream& f);
uint32_t ReadLong(fstream& f);
uint32_t ReadLongBE(fstream& f);
uint32_t ReadLong3(fstream& f);
uint16_t ReadShort(fstream& f);
uint16_t ReadShortBE(fstream& f);
float ReadFloat(fstream& f);
double ReadDouble(fstream& f);
void WriteLongLong(fstream& f, uint64_t value);
void WriteLong(fstream& f, uint32_t value);
void WriteLongBE(fstream& f, uint32_t value);
void WriteLong3(fstream& f, uint32_t value);
void WriteShort(fstream& f, uint16_t value);
void WriteShortBE(fstream& f, uint16_t value);
void WriteFloat(fstream& f, float value);
void WriteDouble(fstream& f, double value);
void WriteString(fstream& f, string value);

uint32_t GetAlignOffset(uint32_t baseoffset, uint32_t align = 4);

void BufferInitPosition(uint32_t pos = 0);
uint32_t BufferGetPosition();
uint64_t BufferReadLongLong(uint8_t* buffer, uint64_t& destvalue);
uint32_t BufferReadLong(uint8_t* buffer, uint32_t& destvalue);
uint32_t BufferReadLongBE(uint8_t* buffer, uint32_t& destvalue);
uint32_t BufferReadLong3(uint8_t* buffer, uint32_t& destvalue);
uint16_t BufferReadShort(uint8_t* buffer, uint16_t& destvalue);
uint16_t BufferReadShortBE(uint8_t* buffer, uint16_t& destvalue);
uint8_t BufferReadChar(uint8_t* buffer, uint8_t& destvalue);
void BufferWriteLongLong(uint8_t* buffer, uint64_t value);
void BufferWriteLong(uint8_t* buffer, uint32_t value);
void BufferWriteLongBE(uint8_t* buffer, uint32_t value);
void BufferWriteLong3(uint8_t* buffer, uint32_t value);
void BufferWriteShort(uint8_t* buffer, uint16_t value);
void BufferWriteShortBE(uint8_t* buffer, uint16_t value);
void BufferWriteChar(uint8_t* buffer, uint8_t value);

uint32_t Raw_FloatToUInt(float f);
uint64_t Raw_DoubleToUInt(double f);
float Raw_UIntToFloat(uint32_t v);
double Raw_UIntToDouble(uint64_t v);

string ConvertToString(int val);
string ConvertWStrToStr(const wstring& wstr);
wstring ConvertStrToWStr(const string& str);

#define StreamAsHex(VALUE) hex << showbase << (unsigned long)VALUE << dec << noshowbase

#define MACRO_IOFUNCTIONGENERIC_STATUS(FFBIN, CONDITION, IO, READ, STATUSSET) \
	if (CONDITION) { \
		int macrostatuscount = 0; \
		if (READ) { \
			IO ## FlexibleChar(FFBIN, macrostatuscount, true); \
			for (int statusit = 0; statusit < macrostatuscount; statusit++) { \
				int statusnum = 0; \
				IO ## FlexibleChar(FFBIN, statusnum, true); \
				STATUSSET.insert(statusnum); \
			} \
		} else { \
			macrostatuscount = STATUSSET.size(); \
			IO ## FlexibleChar(FFBIN, macrostatuscount, true); \
			for (auto statusit = STATUSSET.begin(); statusit != STATUSSET.end(); statusit++) { \
				int statusnum = *statusit; \
				IO ## FlexibleChar(FFBIN, statusnum, true); \
			} \
		} \
	} else { \
		uint32_t macrostatusraw = 0; \
		if (!READ) \
			macrostatusraw = GetStatusBitList(STATUSSET); \
		IO ## Long(FFBIN, macrostatusraw); \
		if (READ) \
			SetupStatusList(STATUSSET, macrostatusraw); \
	}

#endif
