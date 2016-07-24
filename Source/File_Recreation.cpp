#include "File_Recreation.h"

#include "File_Manipulation.h"
#include "ImageMaps.h"

uint8_t binarydata[0x30000000];

void ReadLong(fstream& f, uint32_t& destvalue) {
	destvalue = f.get();
	destvalue |= f.get() << 8;
	destvalue |= f.get() << 16;
	destvalue |= f.get() << 24;
}

void ReadLongBE(fstream& f, uint32_t& destvalue) {
	destvalue = f.get() << 24;
	destvalue |= f.get() << 16;
	destvalue |= f.get() << 8;
	destvalue |= f.get();
}

uint32_t GetNextImgPacketIndexer(uint32_t indexer) {
	indexer = (indexer >> 8);
	indexer++;
	if ((indexer & 0xF)>=0xA)
		indexer += (0x10-(indexer & 0xF));
	if ((indexer & 0xFF)>=0x75)
		indexer += (0x100-(indexer & 0xFF));
	if ((indexer & 0xF00)>=0xA00)
		indexer += (0x1000-(indexer & 0xF00));
	if ((indexer & 0xFF00)>=0x6000)
		indexer += (0x10000-(indexer & 0xF00));
	if ((indexer & 0xF0000)>=0xA0000)
		indexer += (0x100000-(indexer & 0xF0000));
	return (indexer << 8) | 0x2;
}

void RecreateTest(const char* fname, unsigned int fieldid) {
	fstream f(fname,ios::in | ios::binary);
	fstream fout("aaaa.bin",ios::out | ios::binary);
	size_t posi = 0, fsize;
	unsigned int i;
	
	#define MACRO_COPYTEST(BEG,END) \
		f.seekg(BEG); \
		f.read((char*)binarydata+posi,END-BEG); \
		posi += (END - BEG);
	
	f.seekg(0,ios::end);
	fsize = f.tellg();
	f.seekg(IMG_HEADER_OFFSET+0x58);
	uint32_t fieldblockpos;
	ReadLong(f,fieldblockpos);
	MACRO_COPYTEST(0,IMG_HEADER_OFFSET+fieldblockpos*FILE_IGNORE_DATA_PERIOD+8*fieldid+12)
	for (i=5;i<14;i++)
		binarydata[IMG_HEADER_OFFSET+28+i*16]++;
	uint32_t nextfieldpos,objectpos;
	ReadLong(f,nextfieldpos);
	objectpos = nextfieldpos;
	while (f.tellg()<0x23C808) { // end of global map
		if (objectpos>=nextfieldpos && ((unsigned long)f.tellg()-FILE_IGNORE_DATA_FIRST)%FILE_IGNORE_DATA_PERIOD>FILE_IGNORE_DATA_AMOUNT)
			objectpos++;
		binarydata[posi++] = objectpos & 0xFF; binarydata[posi++] = (objectpos >> 8) & 0xFF;
		binarydata[posi++] = (objectpos >> 16) & 0xFF; binarydata[posi++] = (objectpos >> 24) & 0xFF;
		MACRO_COPYTEST(f.tellg(),(unsigned long)f.tellg()+4)
		ReadLong(f,objectpos);
	}
	binarydata[posi++] = objectpos & 0xFF; binarydata[posi++] = (objectpos >> 8) & 0xFF;
	binarydata[posi++] = (objectpos >> 16) & 0xFF; binarydata[posi++] = (objectpos >> 24) & 0xFF;
	nextfieldpos = IMG_HEADER_OFFSET+nextfieldpos*FILE_IGNORE_DATA_PERIOD;
	objectpos = f.tellg();
	MACRO_COPYTEST(objectpos,nextfieldpos)
	uint32_t indexer;
	f.seekg(nextfieldpos-0xC);
	ReadLongBE(f,indexer);
	f.seekg(nextfieldpos);
	for (i=0;i<FILE_IGNORE_DATA_PERIOD-0x17;i++)
		binarydata[posi++] = 0;
	for (i=0;i<10;i++)
		binarydata[posi++] = 0xFF;
	binarydata[posi++] = 0;
	indexer = GetNextImgPacketIndexer(indexer);
	binarydata[posi++] = (indexer >> 24) & 0xFF; binarydata[posi++] = (indexer >> 16) & 0xFF;
	binarydata[posi++] = (indexer >> 8) & 0xFF; binarydata[posi++] = indexer & 0xFF;
	uint32_t tmp80000 = 0x80000;
	binarydata[posi++] = tmp80000 & 0xFF; binarydata[posi++] = (tmp80000 >> 8) & 0xFF;
	binarydata[posi++] = (tmp80000 >> 16) & 0xFF; binarydata[posi++] = (tmp80000 >> 24) & 0xFF;
	binarydata[posi++] = tmp80000 & 0xFF; binarydata[posi++] = (tmp80000 >> 8) & 0xFF;
	binarydata[posi++] = (tmp80000 >> 16) & 0xFF; binarydata[posi++] = (tmp80000 >> 24) & 0xFF;
	uint32_t cpybeg = nextfieldpos, cpyend = cpybeg+FILE_IGNORE_DATA_PERIOD-0xC;
	MACRO_COPYTEST(cpybeg,cpyend)
	ReadLongBE(f,indexer);
	indexer = GetNextImgPacketIndexer(indexer);
	binarydata[posi++] = (indexer >> 24) & 0xFF; binarydata[posi++] = (indexer >> 16) & 0xFF;
	binarydata[posi++] = (indexer >> 8) & 0xFF; binarydata[posi++] = indexer & 0xFF;
	while ((unsigned long)f.tellg()+FILE_IGNORE_DATA_PERIOD<=fsize) {
		cpybeg = f.tellg();
		cpyend = cpybeg+FILE_IGNORE_DATA_PERIOD-0x4;
		MACRO_COPYTEST(cpybeg,cpyend)
		ReadLongBE(f,indexer);
		indexer = GetNextImgPacketIndexer(indexer);
		binarydata[posi++] = (indexer >> 24) & 0xFF; binarydata[posi++] = (indexer >> 16) & 0xFF;
		binarydata[posi++] = (indexer >> 8) & 0xFF; binarydata[posi++] = indexer & 0xFF;
	}
	cpybeg = f.tellg();
	cpyend = fsize;
	MACRO_COPYTEST(cpybeg,cpyend)
	fout.write((char*)binarydata,posi);
}
