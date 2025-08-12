#ifndef _FILE_MANIPULATION_H
#define _FILE_MANIPULATION_H

struct ExtendedCharmap;
struct FF9String;

#include <inttypes.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <wx/string.h>
using namespace std;

#define FILE_IGNORE_DATA_FIRST -0x118
#define FILE_IGNORE_DATA_PERIOD 0x930
#define FILE_IGNORE_DATA_AMOUNT 0x130
#define FILE_IGNORE_DATA_SIZE   0x800
#define CHARMAP_A_CODECHAR 0xFA
#define CHARMAP_B_CODECHAR 0xFB
#define CHARMAP_EXT_CODECHAR 0xFC
#define OPCODE_CHAR 0xF9
#define OPCODE_WCHAR L'µ'

typedef uint8_t SteamLanguage;
#define STEAM_LANGUAGE_US		0
#define STEAM_LANGUAGE_EN		1
#define STEAM_LANGUAGE_JA		2
#define STEAM_LANGUAGE_GE		3
#define STEAM_LANGUAGE_FR		4
#define STEAM_LANGUAGE_IT		5
#define STEAM_LANGUAGE_SP		6
#define STEAM_LANGUAGE_AMOUNT	7
#define STEAM_LANGUAGE_NONE		0xFF

SteamLanguage GetSteamLanguage();
void SetSteamLanguage(SteamLanguage sl);
SteamLanguage GetHWSSteamLanguage();
void SetHWSSteamLanguage(SteamLanguage gt);

#define CHARMAP_TABLE_SIZE 256
extern vector<wchar_t> DEFAULT_CHARMAP;
extern vector<wchar_t> DEFAULT_SECONDARY_CHARMAP;

struct ExtendedCharmap {
	uint16_t amount;
	uint16_t* id;
	wchar_t** letter;
	
	static ExtendedCharmap CreateEmpty();
	void Delete();
	ExtendedCharmap Copy();
	wchar_t* GetCharmap(uint16_t chmapid);
};

struct FF9String {
public:
	// Shared PSX-Steam
	bool created;
	uint16_t length; // Length of raw data ; for Steam, it is the length of the multibyte representation of str
	wstring str; // PSX: string with µ at opcode positions ; Steam: same as str_ext
	wstring str_ext; // String with extended opcode names
	wstring str_nice; // String without opcodes (some are converted)
	uint8_t null_terminated; // PSX: some opcodes terminate the string so no 0 is appended ; Steam: same but the 0-terminating char is replaced by [ENDN]
	// PSX only
	uint8_t* raw; // array of raw data
	uint16_t code_amount; // Match with the amount of µ in str
	uint8_t* code_arg_length;
	uint8_t** code_arg;
	wchar_t* charmap;
	wchar_t* charmap_A;
	wchar_t* charmap_B;
	wchar_t* charmap_Ext;
	wchar_t opcode_wchar;
	// Steam multiple language
	bool multi_lang_init[STEAM_LANGUAGE_AMOUNT];
	wstring multi_lang_str[STEAM_LANGUAGE_AMOUNT];
	
	FF9String();
	FF9String(const FF9String& cp);
	~FF9String();
	void CreateEmpty(bool forceinitall = false);
	void Read(fstream& ffbin, void (*ReadCharFunc)(fstream& fs,uint8_t& ch));
	void ReadFromChar(uint8_t* rawvalue);
	void SetValue(wstring value, SteamLanguage = GetSteamLanguage()); // If there are more µ than "code_amount", the latest ones are ignored
	void SetCharmaps(wchar_t* newmapdef, wchar_t* newmapa, wchar_t* newmapb, wchar_t* newmapext); // newmaps must be 256-sized arrays, never destroyed
	void SetOpcodeChar(wchar_t newchar);
	void ChangeSteamLanguage(SteamLanguage newlang);
	void PermuteCode(uint16_t code1, uint16_t code2); // Assume they are < code_amount
	void AddCode(uint8_t* codearg, uint8_t codelength, uint16_t pos); // codearg must be an array of size codelength
	void SetCode(uint16_t codeid, uint8_t* codearg, uint8_t codelength);
	void RemoveCode(uint16_t codeid); // Assume codeid < code_amount
	wstring& GetStr(int strtype); // 0 = str ; 1 = str_ext ; 2 = str_nice
	uint16_t GetLength(SteamLanguage lang = GetSteamLanguage(), bool withend = true);
	void SteamToPSX();
	void PSXToSteam();
	
	static uint32_t CountSteamTextAmount(char* data, unsigned int size);
	static wstring GetUTF8FromByteCode(char* raw);
	static wstring RemoveOpcodes(wstring str, int charlim = -1);
	
	void GenerateStrExt();
};

struct LogStruct {
	wstring error;
	wstring warning;
	int error_amount;
	int warning_amount;
	bool ok;
	
	LogStruct();
	void AddError(wstring str);
	void AddWarning(wstring str);
	void AddLog(LogStruct otherlog);
};

// Final Fantasy IX PSX binary manipulation
void FFIXReadFF9String(fstream& f, FF9String& deststr);
void FFIXWriteFF9String(fstream& f, FF9String& str);
uint32_t FFIXReadLongAlign(fstream& f, uint32_t& destvalue);
uint32_t FFIXReadLongBE(fstream& f, uint32_t& destvalue);
uint32_t FFIXReadLong(fstream& f, uint32_t& destvalue);
void FFIXWriteLong(fstream& f, uint32_t value);
uint32_t FFIXReadLong3(fstream& f, uint32_t& destvalue);
void FFIXWriteLong3(fstream& f, uint32_t value);
uint16_t FFIXReadShort(fstream& f, uint16_t& destvalue);
void FFIXWriteShort(fstream& f, uint16_t value);
uint8_t FFIXReadChar(fstream& f, uint8_t& destvalue);
void FFIXWriteChar(fstream& f, uint8_t value);
int FFIXReadFlexibleShort(fstream& f, int& destvalue, bool asint);
void FFIXWriteFlexibleShort(fstream& f, int value, bool asint);
int FFIXReadFlexibleChar(fstream& f, int& destvalue, bool asint);
void FFIXWriteFlexibleChar(fstream& f, int value, bool asint);
void FFIXSeek(fstream& f, uint32_t abspos, uint32_t offset);
void FFIXSeekBack(fstream &f, uint32_t abspos, uint32_t minusoffset);
uint32_t GetFFIXOffsetSub(uint32_t beg, uint32_t end);
uint32_t GetFFIXNextIgnore(uint32_t fromoffset);

// Final Fantasy IX Steam binary manipulation
void SteamReadFF9String(fstream& f, FF9String& deststr, SteamLanguage lang = GetSteamLanguage());
void SteamWriteFF9String(fstream& f, FF9String& str, SteamLanguage lang = GetSteamLanguage(), bool writeend = true);
uint32_t SteamReadLong(fstream& f, uint32_t& destvalue);
void SteamWriteLong(fstream& f, uint32_t value);
uint32_t SteamReadLong3(fstream& f, uint32_t& destvalue);
void SteamWriteLong3(fstream& f, uint32_t value);
uint16_t SteamReadShort(fstream& f, uint16_t& destvalue);
void SteamWriteShort(fstream& f, uint16_t value);
uint8_t SteamReadChar(fstream& f, uint8_t& destvalue);
void SteamWriteChar(fstream& f, uint8_t value);
int SteamReadFlexibleShort(fstream& f, int& destvalue, bool asint);
void SteamWriteFlexibleShort(fstream& f, int value, bool asint);
int SteamReadFlexibleChar(fstream& f, int& destvalue, bool asint);
void SteamWriteFlexibleChar(fstream& f, int value, bool asint);
void SteamSeek(fstream& f, uint32_t abspos, uint32_t offset);

// For PPF conversion
void PPFInitScanStep(fstream& f, bool datastring = false, uint16_t len = 0); // len is only used for strings
void PPFStepAddFlexibleChar(fstream& f, int value, bool asint);
void PPFStepAddFlexibleShort(fstream& f, int value, bool asint);
void PPFStepAddChar(fstream& f, uint8_t value);
void PPFStepAddShort(fstream& f, uint16_t value);
void PPFStepAddLong3(fstream& f, uint32_t value);
void PPFStepAddLong(fstream& f, uint32_t value);
int PPFEndScanStep();
// Should never be used with another data type in a scan step
void PPFStepAddFF9String(fstream& f, FF9String& value);

// For HWS / raw binary manipulation
void HWSReadFF9String(fstream& f, FF9String& deststr);
void HWSWriteFF9String(fstream& f, FF9String& str);
uint32_t HWSReadLong(fstream& f, uint32_t& destvalue);
void HWSWriteLong(fstream& f, uint32_t value);
uint32_t HWSReadLong3(fstream& f, uint32_t& destvalue);
void HWSWriteLong3(fstream& f, uint32_t value);
uint16_t HWSReadShort(fstream& f, uint16_t& destvalue);
void HWSWriteShort(fstream& f, uint16_t value);
uint8_t HWSReadChar(fstream& f, uint8_t& destvalue);
void HWSWriteChar(fstream& f, uint8_t value);
int HWSReadFlexibleShort(fstream& f, int& destvalue, bool asint);
void HWSWriteFlexibleShort(fstream& f, int value, bool asint);
int HWSReadFlexibleChar(fstream& f, int& destvalue, bool asint);
void HWSWriteFlexibleChar(fstream& f, int value, bool asint);
void HWSSeek(fstream& f, uint32_t abspos, uint32_t offset);
void HWSReadWString(fstream& f, wstring& str);
void HWSWriteWString(fstream& f, wstring str);
void HWSReadWxString(fstream& f, wxString& str);
void HWSWriteWxString(fstream& f, wxString str);
void HWSReadCSVFormatting(fstream& f, wxString& header, wxString& format);
void HWSWriteCSVFormatting(fstream& f, wxString& header, wxString& format);
void HWSReadCSVFields(fstream& f, map<wxString, wxString>& fields);
void HWSWriteCSVFields(fstream& f, map<wxString, wxString>& fields);
// dummy CSVFields functions for macros
inline void FFIXWriteCSVFields(fstream& f, map<wxString, wxString>& fields) {}
inline void FFIXReadCSVFields(fstream& f, map<wxString, wxString>& fields) {}
inline void SteamWriteCSVFields(fstream& f, map<wxString, wxString>& fields) {}
inline void SteamReadCSVFields(fstream& f, map<wxString, wxString>& fields) {}
inline void PPFStepAddCSVFields(fstream& f, map<wxString, wxString>& fields) {}

#endif
