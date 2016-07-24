#ifndef _FILE_IOCOMMON_H
#define _FILE_IOCOMMON_H

#include <inttypes.h>
#include <fstream>
#include <string>
using namespace std;

uint64_t ReadLongLong(fstream& f);
uint32_t ReadLong(fstream& f);
uint32_t ReadLongBE(fstream& f);
uint32_t ReadLong3(fstream& f);
uint16_t ReadShort(fstream& f);
uint16_t ReadShortBE(fstream& f);
void WriteLongLong(fstream& f, uint64_t value);
void WriteLong(fstream& f, uint32_t value);
void WriteLongBE(fstream& f, uint32_t value);
void WriteLong3(fstream& f, uint32_t value);
void WriteShort(fstream& f, uint16_t value);
void WriteShortBE(fstream& f, uint16_t value);

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

#endif
