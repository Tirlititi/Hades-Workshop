#ifndef _CHARMAPS_H
#define _CHARMAPS_H

struct CharmapDataStruct;

#include <inttypes.h>
#include <fstream>
#include "Configuration.h"
using namespace std;

struct CharmapDataStruct : public ChunkChild {
public:
	uint32_t unknown1;
	uint16_t unknown2;
	uint16_t unknown3;
	uint16_t amount;
	uint16_t unknown4;
	uint8_t* img_x;
	uint8_t* img_y;
	uint8_t* img_width;
	uint8_t* width;
	
	void CalculateTextSize(FF9String* str, CharmapDataStruct* chmapext, uint16_t* x, uint16_t* y);
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

#endif
