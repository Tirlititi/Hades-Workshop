#ifndef _CLUTS_H
#define _CLUTS_H

struct ClutDataStruct;

#include <inttypes.h>
#include <fstream>
#include "Configuration.h"
using namespace std;

struct ClutDataStruct : public ChunkChild {
public:
	uint8_t magic_number;
	uint8_t model_amount;
	uint32_t* model_unknown1;
	uint16_t* model_cluts_offset;
	uint8_t* model_unknown2;
	uint8_t* model_clut_amount;
	uint32_t* model_unknown3;
	uint32_t** clut_unknown1;
	uint32_t** clut_unknown2;
	uint32_t** clut_unknown3;
	uint32_t** clut_unknown4;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void Copy(ClutDataStruct& from, bool deleteold = false);

private:
	void UpdateOffset();
};

#endif
