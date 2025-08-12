#include "File_Manipulation.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <wx/string.h>
#include "Database_Text.h"
#include "main.h"
#include "makeppf3.h"
using namespace std;

#define SPECIAL_STRING_MAX_OPCODE 256
#define SPECIAL_STRING_MAX_ARGUMENT 256
#define SPECIAL_STRING_MAX_LENGTH 0x10000
#define UNKNOWN_CHAR L'?'

SteamLanguage TheSteamLanguage;
SteamLanguage TheHWSSteamLanguage;

SteamLanguage GetSteamLanguage() {
	return TheSteamLanguage;
}
void SetSteamLanguage(SteamLanguage sl) {
	TheSteamLanguage = sl;
}
SteamLanguage GetHWSSteamLanguage() {
	return TheHWSSteamLanguage;
}
void SetHWSSteamLanguage(SteamLanguage sl) {
	TheHWSSteamLanguage = sl;
}

vector<wchar_t> DEFAULT_CHARMAP = { L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'+',L'-',L'=',L'*',L'%',L' ',
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

vector<wchar_t> DEFAULT_SECONDARY_CHARMAP = { L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',L'?',
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

ExtendedCharmap ExtendedCharmap::CreateEmpty() {
	ExtendedCharmap res;
	res.amount = 0;
	res.id = NULL;
	res.letter = NULL;
	return res;
}

void ExtendedCharmap::Delete() {
	if (amount>0) {
		delete[] id;
		for (unsigned int i=0;i<amount;i++)
			delete[] letter[i];
		delete[] letter;
	}
	id = NULL;
	letter = NULL;
	amount = 0;
}

ExtendedCharmap ExtendedCharmap::Copy() {
	ExtendedCharmap res;
	res.amount = amount;
	if (amount>0) {
		res.id = new uint16_t[amount];
		res.letter = new wchar_t*[amount];
		memcpy(res.id,id,amount*sizeof(uint16_t));
		for (unsigned int i=0;i<amount;i++) {
			res.letter[i] = new wchar_t[256];
			memcpy(res.letter[i],letter[i],256*sizeof(wchar_t));
		}
	} else {
		res.id = NULL;
		res.letter = NULL;
	}
	return res;
}

wchar_t* ExtendedCharmap::GetCharmap(uint16_t chmapid) {
	for (unsigned int i=0;i<amount;i++)
		if (chmapid==id[i])
			return letter[i];
	return NULL;
}

inline void JumpIfNeeded(fstream& f) {
	if (((uint32_t)f.tellg()-FILE_IGNORE_DATA_FIRST)%FILE_IGNORE_DATA_PERIOD==0)
		f.seekg(FILE_IGNORE_DATA_AMOUNT,ios_base::cur);
}

wstring FF9String::GetUTF8FromByteCode(char* raw) {
	wxString wxstr = wxString::FromUTF8(raw);
	return wxstr.wc_str();
}

wstring FF9String::RemoveOpcodes(wstring str, int charlim) {
	uint16_t i,len = str.length();
	wstring res = L"";
	if (GetGameType()==GAME_TYPE_PSX) {
		for (i=0;i<len;i++)
			if (str[i]!=OPCODE_WCHAR) {
				if (charlim==0)
					break;
				else if (charlim>0)
					charlim--;
				res += str[i];
			}
	} else {
		for (i=0;i<len;i++) {
			if (str[i]==L'[') {
				i++;
				while (i<len && str[i]!=L']' && str[i]!=L'=')
					i++;
				while (i<len && str[i]!=L']')
					i++;
			} else {
				if (charlim==0)
					break;
				else if (charlim>0)
					charlim--;
				res += str[i];
			}
		}
	}
	if (charlim==0 && i<len)
		res += L"...";
	return res;
}

FF9String::FF9String() :
	created(false),
	raw(NULL),
	charmap(hades::SPECIAL_STRING_CHARMAP_DEFAULT),
	charmap_A(hades::SPECIAL_STRING_CHARMAP_A),
	charmap_B(hades::SPECIAL_STRING_CHARMAP_B),
	charmap_Ext(hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(0)),
	opcode_wchar(hades::SPECIAL_STRING_OPCODE_WCHAR) {
	for (unsigned int i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		multi_lang_init[i] = false;
		multi_lang_str[i] = L"";
	}
}

FF9String::FF9String(const FF9String& cp) :
	created(true),
	length(cp.length),
	str(cp.str),
	str_ext(cp.str_ext),
	str_nice(cp.str_nice),
	code_amount(cp.code_amount),
	null_terminated(cp.null_terminated),
	charmap(cp.charmap),
	charmap_A(cp.charmap_A),
	charmap_B(cp.charmap_B),
	charmap_Ext(cp.charmap_Ext),
	opcode_wchar(cp.opcode_wchar) {
	if (GetGameType()==GAME_TYPE_PSX) { // DEBUG ; maybe use a check on the str instead
		raw = new uint8_t[length];
		code_arg_length = new uint8_t[code_amount];
		code_arg = new uint8_t*[code_amount];
		copy(cp.raw,cp.raw+length,raw);
		copy(cp.code_arg_length,cp.code_arg_length+code_amount,code_arg_length);
		for (unsigned int i=0;i<code_amount;i++) {
			code_arg[i] = new uint8_t[code_arg_length[i]];
			copy(cp.code_arg[i],cp.code_arg[i]+code_arg_length[i],code_arg[i]);
		}
	} else {
		raw = NULL;
	}
	for (unsigned int i=0;i<STEAM_LANGUAGE_AMOUNT;i++) {
		multi_lang_init[i] = cp.multi_lang_init[i];
		if (multi_lang_init[i])
			multi_lang_str[i] = cp.multi_lang_str[i];
		else
			multi_lang_str[i] = L"";
	}
}

FF9String::~FF9String() {
/*	if (raw) {
		delete[] raw;
		delete[] code_arg_length;
		for (unsigned int i=0;i<code_amount;i++)
			delete[] code_arg[i];
		delete[] code_arg;
	}*/
}

void FF9String::CreateEmpty(bool forceinitall) {
	str = L"";
	str_ext = L"";
	str_nice = L"";
	code_amount = 0;
	if (raw)
		delete[] raw;
	if (GetGameType() == GAME_TYPE_PSX) {
		length = 1;
		null_terminated = 1;
		raw = new uint8_t[length];
		raw[0] = 0xFF;
	} else {
		length = 6;
		null_terminated = 1;
		raw = NULL;
	}
	created = true;
	if (forceinitall && GetGameType() != GAME_TYPE_PSX) {
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			multi_lang_init[lang] = true;
			multi_lang_str[lang] = L"";
		}
	}
}

uint8_t tmparg[SPECIAL_STRING_MAX_OPCODE][SPECIAL_STRING_MAX_ARGUMENT];
uint8_t tmparglen[SPECIAL_STRING_MAX_OPCODE];
uint8_t tmpstr[SPECIAL_STRING_MAX_LENGTH];
void FF9String::Read(fstream& ffbin, void (*ReadCharFunc)(fstream& fs, uint8_t& ch)) {
	unsigned int i;
	uint8_t opcode;
	length = 0;
	str = L"";
	code_amount = 0;
	null_terminated = 1;
	ReadCharFunc(ffbin, tmpstr[length++]);
	while (tmpstr[length - 1] != 0xFF) {
		if (tmpstr[length - 1] == OPCODE_CHAR) {
			ReadCharFunc(ffbin, tmpstr[length++]);
			if (tmpstr[length - 1] < HADES_STRING_TEXT_OPCODE.size()) {
				str += opcode_wchar;
				i = 0;
				tmparg[code_amount][i++] = tmpstr[length - 1];
				opcode = tmpstr[length - 1];
				if (HADES_STRING_TEXT_OPCODE[opcode].length != -1) {
					tmparglen[code_amount] = HADES_STRING_TEXT_OPCODE[opcode].length + 1;
				} else if (opcode == 0x04) { // TOKENIZE
					ReadCharFunc(ffbin, tmpstr[length++]);
					tmparg[code_amount][i++] = tmpstr[length - 1];
					tmparglen[code_amount] = 2 + tmpstr[length - 1] * 2;
				} else if (opcode == 0x48) { // 0x48
					ReadCharFunc(ffbin, tmpstr[length++]);
					tmparg[code_amount][i++] = tmpstr[length - 1];
					ReadCharFunc(ffbin, tmpstr[length++]);
					tmparglen[code_amount] = 3;
					while (tmpstr[length - 1] != 0xFF) {
						tmparg[code_amount][i++] = tmpstr[length - 1];
						tmparglen[code_amount]++;
						ReadCharFunc(ffbin, tmpstr[length++]);
					}
					tmparg[code_amount][i++] = tmpstr[length - 1];
				}
				while (i < tmparglen[code_amount]) {
					ReadCharFunc(ffbin, tmpstr[length++]);
					tmparg[code_amount][i++] = tmpstr[length - 1];
				}
				code_amount++;
				if (opcode == 0x01 || opcode == 0x09) {
					null_terminated = 0;
					break;
				}
			}
		} else if (tmpstr[length - 1] == CHARMAP_A_CODECHAR) {
			ReadCharFunc(ffbin, tmpstr[length++]);
			str += charmap_A[tmpstr[length - 1]];
		} else if (tmpstr[length - 1] == CHARMAP_B_CODECHAR) {
			ReadCharFunc(ffbin, tmpstr[length++]);
			str += charmap_B[tmpstr[length - 1]];
		} else if (tmpstr[length - 1] == CHARMAP_EXT_CODECHAR) {
			ReadCharFunc(ffbin, tmpstr[length++]);
			if (charmap_Ext)
				str += charmap_Ext[tmpstr[length - 1]];
			else
				str += UNKNOWN_CHAR;
		} else {
			str += charmap[tmpstr[length - 1]];
		}
		ReadCharFunc(ffbin, tmpstr[length++]);
	}
	if (raw) {
		delete[] raw;
		delete[] code_arg_length;
		delete[] code_arg;
	}
	raw = new uint8_t[length];
	code_arg_length = new uint8_t[code_amount];
	code_arg = new uint8_t * [code_amount];
	copy(tmpstr, tmpstr + length, raw);
	copy(tmparglen, tmparglen + code_amount, code_arg_length);
	for (i = 0; i < code_amount; i++) {
		code_arg[i] = new uint8_t[code_arg_length[i]];
		copy(tmparg[i], tmparg[i] + code_arg_length[i], code_arg[i]);
	}
	for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++) {
		multi_lang_init[i] = false;
		multi_lang_str[i] = L"";
	}
	GenerateStrExt();
	created = true;
}

void ReadCharFuncDummy(fstream& fs, uint8_t& ch) {
}
void FF9String::ReadFromChar(uint8_t* rawvalue) {
	unsigned int len = 0;
	while (rawvalue[len]!=0xFF)
		len++;
	memcpy(tmpstr,rawvalue,len+1);
	fstream dummystream;
	Read(dummystream,&ReadCharFuncDummy);
}

void FF9String::SetValue(wstring value, SteamLanguage lang) {
	if (GetGameType()!=GAME_TYPE_PSX && hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
		return;
	uint16_t len = value.length();
	if (len==0) {
		if (lang<STEAM_LANGUAGE_AMOUNT && GetGameType()!=GAME_TYPE_PSX) {
			multi_lang_init[lang] = true;
			multi_lang_str[lang] = L"";
		}
		if (lang==GetSteamLanguage())
			CreateEmpty();
		return;
	}
	if (GetGameType()==GAME_TYPE_PSX) {
		uint16_t i,j,rawi = 0,codei = 0;
		bool identified, kept;
		str = L"";
		for (i=0;i<len;i++) {
			identified = false;
			kept = true;
			if (value[i]==opcode_wchar) {
				if (codei<code_amount) {
					tmpstr[rawi++] = OPCODE_CHAR;
					for (j=0;j<code_arg_length[codei];j++)
						tmpstr[rawi++] = code_arg[codei][j];
					codei++;
				} else
					kept = false;
				identified = true;
			}
			if (!identified)
				for (j=0;j<0x100;j++)
					if (charmap[j]==value[i]) {
						tmpstr[rawi++] = j;
						identified = true;
						break;
					}
			if (!identified)
				for (j=0;j<0x100;j++)
					if (charmap_A[j]==value[i]) {
						tmpstr[rawi++] = CHARMAP_A_CODECHAR;
						tmpstr[rawi++] = j;
						identified = true;
						break;
					}
			if (!identified)
				for (j=0;j<0x100;j++)
					if (charmap_B[j]==value[i]) {
						tmpstr[rawi++] = CHARMAP_B_CODECHAR;
						tmpstr[rawi++] = j;
						identified = true;
						break;
					}
			if (!identified && charmap_Ext)
				for (j=0;j<0x100;j++)
					if (charmap_Ext[j]==value[i]) {
						tmpstr[rawi++] = CHARMAP_EXT_CODECHAR;
						tmpstr[rawi++] = j;
						identified = true;
						break;
					}
			if (identified && kept)
				str += value[i];
		}
		code_amount = min(code_amount,codei);
		null_terminated = (code_amount>0 && (code_arg[code_amount-1][0]==0x01 || code_arg[code_amount-1][0]==0x09)) ? 0 : 1;
		if (null_terminated)
			tmpstr[rawi++] = 0xFF;
		if (raw)
			delete[] raw;
		length = rawi;
		raw = new uint8_t[length];
		copy(tmpstr,tmpstr+length,raw);
		GenerateStrExt();
	} else {
		if (raw)
			delete[] raw;
		raw = NULL;
		if (lang>=STEAM_LANGUAGE_AMOUNT || (lang<STEAM_LANGUAGE_AMOUNT && GetSteamLanguage()==lang)) {
			wxString wxstr(value);
			wxCharBuffer buffer = wxstr.mb_str(wxConvUTF8);
			null_terminated = 1;
			code_amount = 0;
			length = buffer.length()+6;
			if (value[len-1]==L']') {
				size_t lastopposbeg = value.find_last_of(L'[');
				if (lastopposbeg!=string::npos && lastopposbeg+6<=value.length()) {
					if (value.substr(lastopposbeg+1,4).compare(L"TIME")==0) {
						null_terminated = 0;
						length = buffer.length();
					}
				}
			}
			str = value;
			GenerateStrExt();
		}
		if (lang<STEAM_LANGUAGE_AMOUNT) {
			multi_lang_init[lang] = true;
			multi_lang_str[lang] = value;
		}
	}
}

void FF9String::SetCharmaps(wchar_t* newmapdef, wchar_t* newmapa, wchar_t* newmapb, wchar_t* newmapext) {
	uint16_t i,codei = 0;
	charmap = newmapdef;
	charmap_A = newmapa;
	charmap_B = newmapb;
	charmap_Ext = newmapext;
	str = L"";
	for (i=0;i+null_terminated<length;i++) {
		if (raw[i]==OPCODE_CHAR) {
			str += opcode_wchar;
			i += code_arg_length[codei++];
		} else if (raw[i]==CHARMAP_A_CODECHAR)
			str += charmap_A[raw[++i]];
		else if (raw[i]==CHARMAP_B_CODECHAR)
			str += charmap_B[raw[++i]];
		else if (raw[i]==CHARMAP_EXT_CODECHAR) {
			if (charmap_Ext)
				str += charmap_Ext[raw[++i]];
			else {
				str += UNKNOWN_CHAR;
				i++;
			}
		} else
			str += charmap[raw[i]];
	}
	GenerateStrExt();
}

void FF9String::SetOpcodeChar(wchar_t newchar) {
	uint16_t i,len = str.length();
	for (i=0;i<len;i++)
		if (str[i]==opcode_wchar)
			str[i] = newchar;
	opcode_wchar = newchar;
}

void FF9String::ChangeSteamLanguage(SteamLanguage newlang) {
	if (multi_lang_init[newlang]) {
		SetValue(multi_lang_str[newlang]);
	} else {
		str = L"";
		str_ext = L"";
		str_nice = L"";
		length = 0;
	}
}

void FF9String::PermuteCode(uint16_t code1, uint16_t code2) {
	uint8_t tmp = code_arg_length[code1];
	code_arg_length[code1] = code_arg_length[code2];
	code_arg_length[code2] = tmp;
	uint8_t* tmpp = code_arg[code1];
	code_arg[code1] = code_arg[code2];
	code_arg[code2] = tmpp;
}

void FF9String::AddCode(uint8_t* codearg, uint8_t codelength, uint16_t pos) {
	code_amount++;
	uint8_t* arglen = new uint8_t[code_amount];
	uint8_t** arg = new uint8_t*[code_amount];
	copy(code_arg_length,code_arg_length+pos,arglen);
	copy(code_arg,code_arg+pos,arg);
	arglen[pos] = codelength;
	arg[pos] = new uint8_t[codelength];
	memcpy(arg[pos],codearg,codelength);
	copy(code_arg_length+pos,code_arg_length+code_amount-1,arglen+pos+1);
	copy(code_arg+pos,code_arg+code_amount-1,arg+pos+1);
	delete[] code_arg_length;
	delete[] code_arg;
	code_arg_length = arglen;
	code_arg = arg;
	if (pos==0) {
		str = opcode_wchar+str;
	} else {
		uint16_t codei = 0;
		uint16_t len = str.length();
		for (unsigned int i=0;i<len;i++)
			if (str[i]==opcode_wchar) {
				codei++;
				if (codei==pos) {
					str = str.substr(0,i)+opcode_wchar+str.substr(i+1);
					break;
				}
			}
	}
	SetValue(str);
}

void FF9String::SetCode(uint16_t codeid, uint8_t* codearg, uint8_t codelength) {
	delete[] code_arg[codeid];
	code_arg_length[codeid] = codelength;
	code_arg[codeid] = new uint8_t[codelength];
	memcpy(code_arg[codeid],codearg,codelength);
}

void FF9String::RemoveCode(uint16_t codeid) {
	code_amount--;
	for (unsigned int i=codeid;i<code_amount;i++) {
		code_arg_length[i] = code_arg_length[i+1];
		code_arg[i] = code_arg[i+1];
	}
	SetValue(str);
}

wstring& FF9String::GetStr(int strtype) {
	if (strtype==1)
		return str_ext;
	if (strtype==2)
		return str_nice;
	return str;
}

uint16_t FF9String::GetLength(SteamLanguage lang, bool withend) {
	if (lang==GetSteamLanguage() && !multi_lang_init[lang])
		return 0;
	if (lang>=STEAM_LANGUAGE_AMOUNT || lang==GetSteamLanguage())
		return length-(!withend && null_terminated==1 ? (GetGameType()==GAME_TYPE_PSX ? 1 : 6) : 0);
	if (!multi_lang_init[lang])
		return 0;
	wxString wxstr(multi_lang_str[lang]);
	wxCharBuffer buffer = wxstr.mb_str(wxConvUTF8);
	if (withend && wxstr[wxstr.Length()-1]==L']') {
		size_t lastopposbeg = wxstr.find_last_of(L'[');
		if (lastopposbeg!=string::npos && lastopposbeg+6<=wxstr.Length()) {
			if (wxstr.substr(lastopposbeg+1,4).compare(L"TIME")==0) {
				return buffer.length();
			}
		}
	}
	return buffer.length()+(withend ? 6 : 0);
}

void FF9String::GenerateStrExt() {
	uint16_t i,len = str.length();
	uint16_t codei = 0;
	if (GetGameType()==GAME_TYPE_PSX) {
		str_ext = L"";
		str_nice = L"";
		for (i=0;i<len;i++) {
			if (str[i]==opcode_wchar) {
				str_ext += L"["+HADES_STRING_TEXT_OPCODE[code_arg[codei][0]].label+L"]";
				str_nice += HADES_STRING_TEXT_OPCODE[code_arg[codei++][0]].converted;
			} else {
				str_ext += str[i];
				str_nice += str[i];
			}
		}
	} else { // ToDo: improve that (opcode formats are in NGUIText.cs)
		wstring opcodestr;
		unsigned int j;
		str_ext = str;
		str_nice = L"";
		for (i=0;i<len;i++) {
			if (str[i]==L'[') {
				opcodestr = L"";
				i++;
				while (i<len && str[i]!=L']' && str[i]!=L'=')
					opcodestr += str[i++];
				while (i<len && str[i]!=L']')
					i++;
				if (i>=len)
					break;
				for (j=0;j<TEXT_STEAM_OPCODE_CLOSING_TAG;j++)
					if (opcodestr.compare(HADES_STRING_TEXT_STEAM_OPCODE[j].id)==0) {
						str_nice += HADES_STRING_TEXT_STEAM_OPCODE[j].converted;
						break;
					}
			} else {
				str_nice += str[i];
			}
		}
	}
}

void FF9String::SteamToPSX() {
	// ToDo : handle opcodes...
	GameType gt = GetGameType();
	SetGameType(GAME_TYPE_PSX);
	SetValue(str_nice);
	SetGameType(gt);
}

void FF9String::PSXToSteam() {
	wstring tmpstr = str;
	GameType gt = GetGameType();
	bool timedend = null_terminated == 0 && code_amount > 0;
	int timedendvalue;
	int i;
	if (timedend) {
		if (code_arg[code_amount - 1][0] == 0x01)
			timedendvalue = code_arg[code_amount - 1][1];
		else // 0x09
			timedendvalue = -1;
	}
	SetGameType(GAME_TYPE_STEAM);
	CreateEmpty();
	for (i = 0; i < (int)tmpstr.length(); i++)
		if (tmpstr[i] == hades::SPECIAL_STRING_OPCODE_WCHAR) {
			tmpstr.erase(i, 1);
			i--;
		}
	if (timedend) {
		wstringstream sstr;
		sstr << tmpstr << L"[TIME=" << timedendvalue << L"]";
		tmpstr = sstr.str();
	}
	SetValue(tmpstr);
	SetGameType(gt);
}

uint32_t FF9String::CountSteamTextAmount(char* data, unsigned int size) {
	unsigned int pos = 0;
	uint32_t res = 0;
	while (pos<size)
		if (data[pos++]=='[' && pos<size) {
			if (data[pos]=='E') {
				pos++;
				if (data[pos++]=='N' && pos<size)
					if (data[pos++]=='D' && pos<size)
						if (data[pos++]=='N' && pos<size)
							if (data[pos++]==']')
								res++;
			} else if (data[pos]=='T') {
				pos++;
				if (data[pos++]=='I' && pos<size)
					if (data[pos++]=='M' && pos<size)
						if (data[pos++]=='E' && pos<size)
							if (data[pos++]=='=')
								res++;
			} else
				pos++;
		}
	return res;
}

LogStruct::LogStruct() :
	error(L""),
	warning(L""),
	error_amount(0),
	warning_amount(0),
	ok(true) {
}

void LogStruct::AddError(wstring str) {
	error += str;
	error_amount++;
	ok = false;
}

void LogStruct::AddWarning(wstring str) {
	warning += str;
	warning_amount++;
}

void LogStruct::AddLog(LogStruct otherlog) {
	error += otherlog.error;
	warning += otherlog.warning;
	error_amount += otherlog.error_amount;
	warning_amount += otherlog.warning_amount;
	ok = ok && otherlog.ok;
}

void ReadCharFuncFFIX(fstream& fs,uint8_t& ch) {
	ch = fs.get();
	JumpIfNeeded(fs);
}
void FFIXReadFF9String(fstream& f, FF9String& deststr) {
	deststr.Read(f,&ReadCharFuncFFIX);
}

void FFIXWriteFF9String(fstream& f, FF9String& str) {
	for (int i=0;i<str.length;i++) {
		f.put(str.raw[i]);
		JumpIfNeeded(f);
	}
}

uint32_t FFIXReadLongAlign(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	destvalue |= f.get() << 24;
	JumpIfNeeded(f);
	return destvalue;
}

uint32_t FFIXReadLongBE(fstream& f, uint32_t& destvalue) {
	destvalue = f.get() << 24;
	JumpIfNeeded(f);
	destvalue |= f.get() << 16;
	JumpIfNeeded(f);
	destvalue |= f.get() << 8;
	JumpIfNeeded(f);
	destvalue |= f.get();
	JumpIfNeeded(f);
	return destvalue;
}

uint32_t FFIXReadLong(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	JumpIfNeeded(f);
	destvalue |= f.get() << 8;
	JumpIfNeeded(f);
	destvalue |= f.get() << 16;
	JumpIfNeeded(f);
	destvalue |= f.get() << 24;
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteLong(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 8) & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 16) & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 24) & 0xFF);
	JumpIfNeeded(f);
}


uint32_t FFIXReadLong3(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	JumpIfNeeded(f);
	destvalue |= f.get() << 8;
	JumpIfNeeded(f);
	destvalue |= f.get() << 16;
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteLong3(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 8) & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 16) & 0xFF);
	JumpIfNeeded(f);
}

uint16_t FFIXReadShort(fstream& f, uint16_t& destvalue) {
	destvalue = f.get();
	JumpIfNeeded(f);
	destvalue |= f.get() << 8;
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteShort(fstream& f, uint16_t value) {
	f.put(value & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 8) & 0xFF);
	JumpIfNeeded(f);
}

uint8_t FFIXReadChar(fstream& f, uint8_t& destvalue) {
	destvalue = f.get();
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteChar(fstream& f, uint8_t value) {
	f.put(value);
	JumpIfNeeded(f);
}

int FFIXReadFlexibleShort(fstream& f, int& destvalue, bool asint) {
	destvalue = f.get();
	JumpIfNeeded(f);
	destvalue |= f.get() << 8;
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteFlexibleShort(fstream& f, int value, bool asint) {
	f.put(value & 0xFF);
	JumpIfNeeded(f);
	f.put((value >> 8) & 0xFF);
	JumpIfNeeded(f);
}

int FFIXReadFlexibleChar(fstream& f, int& destvalue, bool asint) {
	destvalue = f.get();
	JumpIfNeeded(f);
	return destvalue;
}

void FFIXWriteFlexibleChar(fstream& f, int value, bool asint) {
	f.put(value & 0xFF);
	JumpIfNeeded(f);
}

void FFIXSeek(fstream& f, uint32_t abspos, uint32_t offset) {
	uint32_t nextignore = GetFFIXNextIgnore(abspos);
	uint32_t realoffset = offset;
	f.seekg(abspos);
	if (offset>=nextignore) {
		offset -= nextignore;
		f.seekg(FILE_IGNORE_DATA_AMOUNT*(offset/FILE_IGNORE_DATA_SIZE+1),ios_base::cur);
	}
	f.seekg(realoffset,ios_base::cur);
}

void FFIXSeekBack(fstream& f, uint32_t abspos, uint32_t minusoffset) {
	uint32_t lastignore = (abspos-FILE_IGNORE_DATA_FIRST)%FILE_IGNORE_DATA_PERIOD-FILE_IGNORE_DATA_AMOUNT;
	uint32_t realoffset = minusoffset;
	if (minusoffset>lastignore) {
		minusoffset -= lastignore;
		f.seekg(abspos-realoffset-FILE_IGNORE_DATA_AMOUNT*(minusoffset/FILE_IGNORE_DATA_SIZE+1),ios_base::cur);
	} else
		f.seekg(abspos-realoffset);
}

uint32_t GetFFIXOffsetSub(uint32_t beg, uint32_t end) {
	uint32_t sub = end-beg;
	uint32_t nextignore = GetFFIXNextIgnore(beg);
	if (sub>=nextignore)
		sub -= FILE_IGNORE_DATA_AMOUNT*((sub-nextignore)/FILE_IGNORE_DATA_PERIOD+1);
	return sub;
}

uint32_t GetFFIXNextIgnore(uint32_t fromoffset) {
	return FILE_IGNORE_DATA_PERIOD-(fromoffset-FILE_IGNORE_DATA_FIRST)%FILE_IGNORE_DATA_PERIOD;
}

void SteamReadFF9String(fstream& f, FF9String& deststr, SteamLanguage lang) {
	unsigned int i = 0;
	bool reachend = false;
	bool mainstr = lang==STEAM_LANGUAGE_NONE || lang==GetSteamLanguage();
	if (mainstr) {
		deststr.code_amount = 0;
		if (deststr.raw)
			delete[] deststr.raw;
		deststr.raw = NULL;
	}
	while (!reachend) {
		tmpstr[i] = f.get();
		if (tmpstr[i]==0) {
			reachend = true;
			if (mainstr) {
				deststr.null_terminated = 1;
				deststr.length = i+6;
			}
		} else if (i>=5 && tmpstr[i-5]=='[' && tmpstr[i-4]=='E' && tmpstr[i-3]=='N' && tmpstr[i-2]=='D' && tmpstr[i-1]=='N' && tmpstr[i]==']') {
			reachend = true;
			i -= 5;
			if (mainstr) {
				deststr.null_terminated = 1;
				deststr.length = i+6;
			}
		} else if (i>=5 && tmpstr[i-5]=='[' && tmpstr[i-4]=='T' && tmpstr[i-3]=='I' && tmpstr[i-2]=='M' && tmpstr[i-1]=='E' && tmpstr[i]=='=') {
			reachend = true;
			tmpstr[++i] = f.get();
			while (tmpstr[i]!=L']')
				tmpstr[++i] = f.get();
			i++;
			if (mainstr) {
				deststr.null_terminated = 0;
				deststr.length = i;
			}
		} else {
			i++;
		}
	}
	tmpstr[i] = 0;
	if (lang==STEAM_LANGUAGE_NONE) {
		deststr.str = FF9String::GetUTF8FromByteCode((char*)tmpstr);
		deststr.GenerateStrExt();
	} else {
		deststr.multi_lang_init[lang] = true;
		deststr.multi_lang_str[lang] = FF9String::GetUTF8FromByteCode((char*)tmpstr);
		if (lang==GetSteamLanguage()) {
			deststr.str = deststr.multi_lang_str[lang];
			deststr.GenerateStrExt();
		}
	}
	deststr.created = true;
}

void SteamWriteFF9String(fstream& f, FF9String& str, SteamLanguage lang, bool writeend) {
	bool mainstr = lang==STEAM_LANGUAGE_NONE || lang==GetSteamLanguage();
	if (str.GetLength(lang)==0)
		return;
	wxString wxstr;
	if (mainstr)
		wxstr = _(str.str);
	else
		wxstr = _(str.multi_lang_str[lang]);
	wxCharBuffer buffer = wxstr.mb_str(wxConvUTF8);
	unsigned int i;
	for (i=0;i<buffer.length();i++)
		f.put(buffer[i]);
	if (writeend) {
		uint8_t nt;
		if (mainstr) {
			nt = str.null_terminated;
		} else {
			wstring& strval = str.multi_lang_str[lang];
			nt = 1;
			if (strval.length()>0 && strval[strval.length()-1]==L']') {
				size_t lastopposbeg = strval.find_last_of(L'[');
				if (lastopposbeg!=string::npos && lastopposbeg+6<=strval.length())
					if (strval.substr(lastopposbeg+1,4).compare(L"TIME")==0)
						nt = 0;
			}
		}
		if (nt==1) {
			f.put('[');
			f.put('E');
			f.put('N');
			f.put('D');
			f.put('N');
			f.put(']');
		}
	}
}

uint32_t SteamReadLong(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	destvalue |= f.get() << 24;
	return destvalue;
}

void SteamWriteLong(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
	f.put((value >> 16) & 0xFF);
	f.put((value >> 24) & 0xFF);
}

uint32_t SteamReadLong3(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	return destvalue;
}

void SteamWriteLong3(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
	f.put((value >> 16) & 0xFF);
}

uint16_t SteamReadShort(fstream& f, uint16_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	return destvalue;
}

void SteamWriteShort(fstream& f, uint16_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
}

uint8_t SteamReadChar(fstream& f, uint8_t& destvalue) {
	destvalue = f.get();
	return destvalue;
}

void SteamWriteChar(fstream& f, uint8_t value) {
	f.put(value & 0xFF);
}

int SteamReadFlexibleShort(fstream& f, int& destvalue, bool asint) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	return destvalue;
}

void SteamWriteFlexibleShort(fstream& f, int value, bool asint) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
}

int SteamReadFlexibleChar(fstream& f, int& destvalue, bool asint) {
	destvalue = f.get();
	return destvalue;
}

void SteamWriteFlexibleChar(fstream& f, int value, bool asint) {
	f.put(value & 0xFF);
}

void SteamSeek(fstream& f, uint32_t abspos, uint32_t offset) {
	f.seekg(abspos+offset);
}

#define PPF_MAX_DATA_SEQUENCE 0x1000000
uint64_t ppfstepoffset;
uint32_t ppfstepsize;
uint32_t ppfstepnextignore;
unsigned char ppfstepx[PPF_MAX_DATA_SEQUENCE];
unsigned char ppfstepy[PPF_MAX_DATA_SEQUENCE];
void PPFInitScanStep(fstream& f, bool datastring, uint16_t len) {
	ppfstepoffset = f.tellg();
	ppfstepnextignore = GetFFIXNextIgnore(ppfstepoffset);
	if (datastring) {
		FFIXSeek(f,ppfstepoffset,len);
		len = f.tellg();
		len -= ppfstepoffset;
		for (unsigned int i=0;i<len;i++) {
			ppfstepx[i] = 0;
			ppfstepy[i] = 0;
		}
		ppfstepsize = len;
		f.seekg(ppfstepoffset);
	} else
		ppfstepsize = 0;
}

inline void PPFStepIgnoreData(fstream& f) {
	if (ppfstepsize==ppfstepnextignore) {
		for (int i=0;i<FILE_IGNORE_DATA_AMOUNT;i++) {
			ppfstepx[ppfstepsize] = f.get();
			ppfstepy[ppfstepsize] = ppfstepx[ppfstepsize];
			ppfstepsize++;
		}
		ppfstepnextignore += FILE_IGNORE_DATA_PERIOD;
	}
}

void PPFStepAddFlexibleChar(fstream& f, int value, bool asint) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value;
	PPFStepIgnoreData(f);
}

void PPFStepAddFlexibleShort(fstream& f, int value, bool asint) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 8) & 0xFF;
	PPFStepIgnoreData(f);
}

void PPFStepAddChar(fstream& f, uint8_t value) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value;
	PPFStepIgnoreData(f);
}

void PPFStepAddShort(fstream& f, uint16_t value) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 8) & 0xFF;
	PPFStepIgnoreData(f);
}

void PPFStepAddLong3(fstream& f, uint32_t value) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 8) & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 16) & 0xFF;
	PPFStepIgnoreData(f);
}

void PPFStepAddLong(fstream& f, uint32_t value) {
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = value & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 8) & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 16) & 0xFF;
	PPFStepIgnoreData(f);
	ppfstepx[ppfstepsize] = f.get();
	ppfstepy[ppfstepsize++] = (value >> 24) & 0xFF;
	PPFStepIgnoreData(f);
}

void PPFStepAddFF9String(fstream& f, FF9String& value) {
	uint32_t index;
	for (int i=0;i<value.length;i++) {
		index = f.tellg();
		index -= ppfstepoffset;
		FFIXReadChar(f,ppfstepx[index]);
		ppfstepy[index] = value.raw[i];
	}
}

int PPFEndScanStep() {
	return PPFWriteChanges(ppfstepx,ppfstepy,ppfstepsize,ppfstepoffset);
}

void ReadCharFuncHWS(fstream& fs, uint8_t& ch) {
	ch = fs.get();
}
void HWSReadFF9String(fstream& f, FF9String& deststr) {
	deststr.Read(f,&ReadCharFuncHWS);
}

void HWSWriteFF9String(fstream& f, FF9String& str) {
	for (int i=0;i<str.length;i++)
		f.put(str.raw[i]);
}

uint32_t HWSReadLong(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	destvalue |= f.get() << 24;
	return destvalue;
}

void HWSWriteLong(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
	f.put((value >> 16) & 0xFF);
	f.put((value >> 24) & 0xFF);
}

uint32_t HWSReadLong3(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	return destvalue;
}

void HWSWriteLong3(fstream& f, uint32_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
	f.put((value >> 16) & 0xFF);
}

uint16_t HWSReadShort(fstream& f, uint16_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	return destvalue;
}

void HWSWriteShort(fstream& f, uint16_t value) {
	f.put(value & 0xFF);
	f.put((value >> 8) & 0xFF);
}

uint8_t HWSReadChar(fstream& f, uint8_t& destvalue) {
	destvalue = f.get();
	return destvalue;
}

void HWSWriteChar(fstream& f, uint8_t value) {
	f.put(value);
}

int HWSReadFlexibleShort(fstream& f, int& destvalue, bool asint) {
	if (asint) {
		destvalue = f.get();
		destvalue |= f.get() << 8;
		destvalue |= f.get() << 16;
		destvalue |= f.get() << 24;
	} else {
		destvalue = f.get();
		destvalue |= f.get() << 8;
	}
	return destvalue;
}

void HWSWriteFlexibleShort(fstream& f, int value, bool asint) {
	if (asint) {
		f.put(value & 0xFF);
		f.put((value >> 8) & 0xFF);
		f.put((value >> 16) & 0xFF);
		f.put((value >> 24) & 0xFF);
	} else {
		f.put(value & 0xFF);
		f.put((value >> 8) & 0xFF);
	}
}

int HWSReadFlexibleChar(fstream& f, int& destvalue, bool asint) {
	if (asint) {
		destvalue = f.get();
		destvalue |= f.get() << 8;
		destvalue |= f.get() << 16;
		destvalue |= f.get() << 24;
	} else {
		destvalue = f.get();
	}
	return destvalue;
}

void HWSWriteFlexibleChar(fstream& f, int value, bool asint) {
	if (asint) {
		f.put(value & 0xFF);
		f.put((value >> 8) & 0xFF);
		f.put((value >> 16) & 0xFF);
		f.put((value >> 24) & 0xFF);
	} else {
		f.put(value & 0xFF);
	}
}

void HWSSeek(fstream& f, uint32_t abspos, uint32_t offset) {
	f.seekg(abspos+offset);
}

void HWSReadWString(fstream& f, wstring& str) {
	int strlen;
	char* raw;
	HWSReadFlexibleChar(f, strlen, true);
	raw = new char[strlen];
	for (int i = 0; i < strlen; i++)
		raw[i] = f.get();
	str = wxString::FromUTF8(raw, strlen).wc_str();
}

void HWSWriteWString(fstream& f, wstring str) {
	wxString wxstr(str);
	wxCharBuffer raw = wxstr.mb_str(wxConvUTF8);
	int strlen = raw.length();
	HWSWriteFlexibleChar(f, strlen, true);
	for (unsigned int i = 0; (int)i < strlen; i++)
		f.put(raw[i]);
}

void HWSReadWxString(fstream& f, wxString& str) {
	int strlen;
	char* raw;
	HWSReadFlexibleChar(f, strlen, true);
	raw = new char[strlen];
	for (int i = 0; i < strlen; i++)
		raw[i] = f.get();
	str = wxString::FromUTF8(raw, strlen);
}

void HWSWriteWxString(fstream& f, wxString str) {
	wxCharBuffer raw = str.mb_str(wxConvUTF8);
	int strlen = raw.length();
	HWSWriteFlexibleChar(f, strlen, true);
	for (unsigned int i = 0; (int)i < strlen; i++)
		f.put(raw[i]);
}

void HWSReadCSVFormatting(fstream& f, wxString& header, wxString& format) {
	HWSReadWxString(f, header);
	HWSReadWxString(f, format);
}

void HWSWriteCSVFormatting(fstream& f, wxString& header, wxString& format) {
	HWSWriteWxString(f, header);
	HWSWriteWxString(f, format);
}

void HWSReadCSVFields(fstream& f, map<wxString, wxString>& fields) {
	wxString name, defaultvalue;
	int fieldcount;
	HWSReadFlexibleChar(f, fieldcount, true);
	for (int i = 0; i < fieldcount; i++) {
		HWSReadWxString(f, name);
		HWSReadWxString(f, defaultvalue);
		fields[name] = defaultvalue;
	}
}

void HWSWriteCSVFields(fstream& f, map<wxString, wxString>& fields) {
	HWSWriteFlexibleChar(f, fields.size(), true);
	for (auto it = fields.begin(); it != fields.end(); it++) {
		HWSWriteWxString(f, it->first);
		HWSWriteWxString(f, it->second);
	}
}
