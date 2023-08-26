#ifndef _DLLCILMACRO_H
#define _DLLCILMACRO_H

struct CILMacroBaseStruct;
struct CILMacroID;
struct CILMacro;

#include "DllEditor.h"
#include <vector>
using namespace std;

struct CILMacroBaseStruct {
	CILMacro* parent;
	
	virtual unsigned int GetMethodCount() { return 0; }
	virtual string GetMethodTypeName(unsigned int index) { return ""; }
	virtual string GetMethodName(unsigned int index) { return ""; }
	// Return a modifamount-long pointer, to be deleted[] ; modifamount will likely be set on GetMethodCount() (maybe it won't be anymore in the future)
	virtual DllMetaDataModification* ComputeModifications(unsigned int* modifamount) { return NULL; } // Require a DllMetaData access
	virtual void GenerateCSharp(vector<string>& buffer) { }
	
	virtual void SetParameters(uint32_t* intparam = NULL) { }
	virtual void SetParameters(double* floatparam = NULL) { }
	virtual void SetParameters(wstring* strparam = NULL) { }
	virtual uint16_t GetSaveFlag() { return 0; }
	virtual void ReadHWS(fstream& ffhws, uint16_t flag) { }
	virtual void WriteHWS(fstream& ffhws) { }
};

struct CILMacroID {
	uint32_t id;
	wstring name;
	wstring description;
};

extern vector<CILMacroID> CILMacroIDList;

struct CILMacro {
public:
	CILMacroID* type;
	CILMacroBaseStruct* info;
	DllMetaData* data;
	
	CILMacro();
	void Init(uint32_t macroid, DllMetaData* d);
	CILMacro(uint32_t macroid, DllMetaData* d);
	~CILMacro();
	
	int FindMethod(unsigned int tid, unsigned int mid);
	int FindMethodById(unsigned int absmethid);
	
private:
	bool isdeletedforcopy;
	friend CILDataSet;
};

#endif
