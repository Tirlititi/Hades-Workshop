#ifndef _FILE_MANIPULATION_H
#define _FILE_MANIPULATION_H

struct ExtendedCharmap;
struct FF9String;

#include <inttypes.h>
#include <fstream>
#include <string>
using namespace std;

#define FILE_IGNORE_DATA_FIRST -0x118
#define FILE_IGNORE_DATA_PERIOD 0x930
#define FILE_IGNORE_DATA_AMOUNT 0x130
#define CHARMAP_A_CODECHAR 0xFA
#define CHARMAP_B_CODECHAR 0xFB
#define CHARMAP_EXT_CODECHAR 0xFC
#define OPCODE_CHAR 0xF9
#define OPCODE_WCHAR L'µ'

static wchar_t DEFAULT_CHARMAP[256] = {	L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'+',L'-',L'=',L'*',L'%',L' ',
										L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',L'K',L'L',L'M',L'N',L'O',L'P',
										L'Q',L'R',L'S',L'T',L'U',L'V',L'W',L'X',L'Y',L'Z',L'(',L'!',L'?',L'“',L':',L'.',
										L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',L'k',L'l',L'm',L'n',L'o',L'p',
										L'q',L'r',L's',L't',L'u',L'v',L'w',L'x',L'y',L'z',L')',L',',L'/',L'+',L'~',L'&',
										L'Á',L'À',L'Â',L'Ä',L'É',L'È',L'Ê',L'Ë',L'Í',L'Ì',L'Î',L'Ï',L'Ó',L'Ò',L'Ô',L'Ö',
										L'Ú',L'Ù',L'Û',L'Ü',L'á',L'à',L'â',L'ä',L'é',L'è',L'ê',L'ë',L'í',L'ì',L'î',L'ï',
										L'ó',L'ò',L'ô',L'ö',L'ú',L'ù',L'û',L'ü',L'Ç',L'Ñ',L'ç',L'ñ',L'Œ',L'ß',L'\'',L'”',
										L'_',L'}',L'{',L'∴',L'∵',L'♪',L'→',L'∈',L'×',L'♦',L'§',L'<',L'>',L'←',L'∋',L'↑',
										L'△',L'□',L'∞',L'♥',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
										L'?',L'«',L'»',L'↓',L'―',L'°',L'★',L'♂',L'♀',L'☺',L'?',L'„',L'‘',L'#',L'※',L';',
										L'¡',L'¿',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
										L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
										L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
										L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
										L'?',L'?',L'?',L'?',L'?',L'?',L' ',L'\n',L'\t',L'µ',L' ',L'?',L' ',L' ',L'¶',L'\0' };

static wchar_t DEFAULT_SECONDARY_CHARMAP[256] = {	L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
													L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?' };

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
	uint8_t* raw; // array of raw data ; unused for Steam version
	uint16_t length; // Length of raw data ; for Steam, it is the length of the multibyte representation of str
	wstring str; // String with µ at opcode positions
	wstring str_ext; // String with extended opcode names
	wstring str_nice; // String without opcodes (some are converted)
	uint16_t code_amount; // Match with the amount of µ in str
	uint8_t* code_arg_length;
	uint8_t** code_arg;
	uint8_t null_terminated; // PSX: some opcodes terminate the string so no 0 is appended ; Steam: same but the 0-terminating char is replaced by [ENDN]
	wchar_t* charmap;
	wchar_t* charmap_A;
	wchar_t* charmap_B;
	wchar_t* charmap_Ext;
	wchar_t opcode_wchar;
	bool created;
	
	FF9String();
	FF9String(FF9String& cp);
	~FF9String();
	void CreateEmpty();
	void Read(fstream& ffbin, void (*ReadCharFunc)(fstream& fs,uint8_t& ch));
	void ReadFromChar(uint8_t* rawvalue);
	void SetValue(wstring value); // If there are more µ than "code_amount", the latest ones are ignored
	void SetCharmaps(wchar_t* newmapdef, wchar_t* newmapa, wchar_t* newmapb, wchar_t* newmapext); // newmaps must be 256-sized arrays, never destroyed
	void SetOpcodeChar(wchar_t newchar);
	void PermuteCode(uint16_t code1, uint16_t code2); // Assume they are < code_amount
	void AddCode(uint8_t* codearg, uint8_t codelength, uint16_t pos); // codearg must be an array of size codelength
	void SetCode(uint16_t codeid, uint8_t* codearg, uint8_t codelength);
	void RemoveCode(uint16_t codeid); // Assume codeid < code_amount
	wstring& GetStr(int strtype); // 0 = str ; 1 = str_ext ; 2 = str_nice
	void SteamToPSX();
	void PSXToSteam();
	
	static uint32_t CountSteamTextAmount(char* data, unsigned int size);
	static wstring GetUTF8FromByteCode(char* raw);
	
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
void FFIXSeek(fstream& f, uint32_t abspos, uint32_t offset);
void FFIXSeekBack(fstream &f, uint32_t abspos, uint32_t minusoffset);
uint32_t GetFFIXOffsetSub(uint32_t beg, uint32_t end);
uint32_t GetFFIXNextIgnore(uint32_t fromoffset);

// Final Fantasy IX Steam binary manipulation
void SteamReadFF9String(fstream& f, FF9String& deststr);
void SteamWriteFF9String(fstream& f, FF9String& str, bool writeend = true);
uint32_t SteamReadLong(fstream& f, uint32_t& destvalue);
void SteamWriteLong(fstream& f, uint32_t value);
uint32_t SteamReadLong3(fstream& f, uint32_t& destvalue);
void SteamWriteLong3(fstream& f, uint32_t value);
uint16_t SteamReadShort(fstream& f, uint16_t& destvalue);
void SteamWriteShort(fstream& f, uint16_t value);
uint8_t SteamReadChar(fstream& f, uint8_t& destvalue);
void SteamWriteChar(fstream& f, uint8_t value);
void SteamSeek(fstream& f, uint32_t abspos, uint32_t offset);

// For PPF conversion
void PPFInitScanStep(fstream& f, bool datastring = false, uint16_t len = 0); // len is only used for strings
void PPFStepAddChar(fstream& f, uint8_t value);
void PPFStepAddShort(fstream& f, uint16_t value);
void PPFStepAddLong3(fstream& f, uint32_t value);
void PPFStepAddLong(fstream& f, uint32_t value);
int PPFEndScanStep();
// Should never be used with another data type in a scan step
void PPFStepAddFF9String(fstream& f, FF9String& value);

// For HWS manipulation
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
void HWSSeek(fstream& f, uint32_t abspos, uint32_t offset);

#endif
