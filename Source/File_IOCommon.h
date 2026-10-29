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

void SetDebugLogInfo(int infotype, string info);
fstream& GetDebugLog();

#define StreamAsHex(VALUE) hex << showbase << (unsigned long)VALUE << dec << noshowbase

#define MACRO_IOFUNCTIONGENERIC_STATUS(FFBIN, CONDITION, IO, READ, STATUSSET, DEBUGINFO) \
	if (CONDITION) { \
		int macrostatuscount = 0; \
		if (READ) { \
			STATUSSET.clear(); \
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
		if (READ) { \
			if (!STATUSSET.empty()) { \
				uint32_t macroremovedstatuses = ~macrostatusraw & GetStatusBitList(STATUSSET); \
				if (macroremovedstatuses != 0) { \
					bool macroaddcomma = false; \
					for (int statusnum = 0; statusnum < 32; statusnum++) { \
						if ((macroremovedstatuses & (1u << statusnum)) != 0) { \
							GetDebugLog() << (macroaddcomma ? ", " : DEBUGINFO) << HADES_STRING_STATUS_NAME[statusnum].c_str(); \
							macroaddcomma = true; \
						} \
					} \
					if (macroaddcomma) \
						GetDebugLog() << endl; \
				} \
			} \
			SetupStatusList(STATUSSET, macrostatusraw); \
		} \
	}

#define MACRO_IOFUNCTIONGENERIC_ELEMCLASS(FFBIN, CONDITION, IO, READ, ELEMSET) \
	if (CONDITION) { \
		int macroelemcount = 0; \
		if (READ) { \
			ELEMSET.clear(); \
			IO ## FlexibleChar(FFBIN, macroelemcount, true); \
			for (int elemit = 0; elemit < macroelemcount; elemit++) { \
				int elemnum = 0; \
				IO ## FlexibleChar(FFBIN, elemnum, true); \
				ELEMSET.insert(elemnum); \
			} \
		} else { \
			macroelemcount = ELEMSET.size(); \
			IO ## FlexibleChar(FFBIN, macroelemcount, true); \
			for (auto elemit = ELEMSET.begin(); elemit != ELEMSET.end(); elemit++) { \
				int elemnum = *elemit; \
				IO ## FlexibleChar(FFBIN, elemnum, true); \
			} \
		} \
	} else { \
		uint8_t macroelemraw = 0; \
		if (!READ) macroelemraw = GetStatusBitList(ELEMSET); \
		IO ## Char(FFBIN, macroelemraw); \
		if (READ) SetupStatusList(ELEMSET, macroelemraw); \
	}

#endif
