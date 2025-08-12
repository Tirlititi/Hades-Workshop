#ifndef _STATUSES_H
#define _STATUSES_H

#define STATUS_AMOUNT		32
#define STATUS_SET_AMOUNT	64
struct StatusDataStruct;
struct StatusDataSet;

#include <inttypes.h>
#include <fstream>
#include <array>
#include "DllEditor.h"
using namespace std;

#define STATUS_PETRIFY			0x00000001
#define STATUS_VENOM			0x00000002
#define STATUS_VIRUS			0x00000004
#define STATUS_SILENCE			0x00000008
#define STATUS_BLIND			0x00000010
#define STATUS_TROUBLE			0x00000020
#define STATUS_ZOMBIE			0x00000040
#define STATUS_EASY_KILL		0x00000080
#define STATUS_DEATH			0x00000100
#define STATUS_LOW_HP			0x00000200
#define STATUS_CONFUSE			0x00000400
#define STATUS_BERSERK			0x00000800
#define STATUS_STOP				0x00001000
#define STATUS_AUTOLIFE			0x00002000
#define STATUS_TRANCE			0x00004000
#define STATUS_DEFEND			0x00008000
#define STATUS_POISON			0x00010000
#define STATUS_SLEEP			0x00020000
#define STATUS_REGEN			0x00040000
#define STATUS_HASTE			0x00080000
#define STATUS_SLOW				0x00100000
#define STATUS_FLOAT			0x00200000
#define STATUS_SHELL			0x00400000
#define STATUS_PROTECT			0x00800000
#define STATUS_HEAT				0x01000000
#define STATUS_FREEZE			0x02000000
#define STATUS_VANISH			0x04000000
#define STATUS_DOOM				0x08000000
#define STATUS_MINI				0x10000000
#define STATUS_REFLECT			0x20000000
#define STATUS_JUMP				0x40000000
#define STATUS_GRADUAL_PETRIFY	0x80000000

#include "File_Manipulation.h"
#include "Configuration.h"

// Currently unused; might add it in the future to generate "StatusData.csv" and setup custom status names...
struct StatusDataStruct {
public:
	int id;
	wxString name;
	int priority;
	int tick = 0;
	int duration = 0;
	set<int> clear;
	set<int> immune;
	int sps = -1;
	int sps_attach = 0;
	array<int, 3> sps_pos = { 0, 0, 0 };
	int shp = -1;
	int shp_attach = 0;
	array<int, 3> shp_pos = { 0, 0, 0 };
	int color_kind = -1;
	int color_priority = 0;
	array<int, 3> color = { 0, 0, 0 };

	map<wxString, wxString> custom_field;

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	StatusDataSet* parent;
	friend StatusDataSet;
};

struct StatusDataSet {
public:
	vector<StatusDataStruct> status;

	wxString csv_header;
	wxString csv_format;
	map<wxString, wxString> custom_field;

	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	bool GenerateCSV(string basefolder);
};

#endif
