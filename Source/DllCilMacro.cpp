#include "DllCilMacro.h"

#include "File_Manipulation.h"
#include "Assembly_Strings.h"

//=============================//
//           Common            //
//=============================//

static ILInstruction MACROINST_NULL[] = {
};

#define MACRO_CILMACRO_COMPUTEBEG() \
	unsigned int i,currentpos,nextpos,frompos,topos,modi = 0,modamount = GetMethodCount(); \
	DllMetaDataModification* res = new DllMetaDataModification[modamount]; \
	DllMetaData* data = parent->data; \
	fstream& dllfile = data->dll_file; \
	DllMethodInfo methinfo; \
	int absmethid, offsetmodif;

#define MACRO_CILMACRO_READMETHODINFO(NAMESP) \
	absmethid = data->GetMethodIdByName(GetMethodTypeName(modi).c_str(),GetMethodName(modi).c_str(),NAMESP); \
	dllfile.seekg(data->GetMethodOffset(absmethid)); \
	methinfo.ReadMethodInfo(dllfile);

#define MACRO_CILMACRO_COMPUTEREPLACEALL(METHINST) \
	res[modi].position = dllfile.tellg(); \
	res[modi].method_id = absmethid; \
	res[modi].base_length = methinfo.code_size; \
	res[modi].new_length = 0; \
	for (i=0;i<G_N_ELEMENTS(METHINST);i++) \
		res[modi].new_length += METHINST[i].GetLength(); \
	res[modi].value = new uint8_t[res[modi].new_length]; \
	BufferInitPosition(); \
	for (i=0;i<G_N_ELEMENTS(METHINST);i++) \
		METHINST[i].AppendToBuffer(res[modi].value); \
	modi++;

#define MACRO_CILMACRO_COMPUTEREPLACE(INSTFROM,INSTTO,INSTVALUE) \
	res[modi].position = dllfile.tellg(); \
	res[modi].method_id = absmethid; \
	res[modi].base_length = methinfo.code_size; \
	res[modi].new_length = 0; \
	if (INSTFROM!=MACROINST_NULL) \
		frompos = methinfo.JumpToInstructions(dllfile,G_N_ELEMENTS(INSTFROM),INSTFROM,true); \
	else \
		frompos = 0; \
	if (INSTTO!=MACROINST_NULL) \
		topos = methinfo.JumpToInstructions(dllfile,G_N_ELEMENTS(INSTTO),INSTTO,true); \
	else \
		topos = frompos; \
	if (INSTVALUE!=MACROINST_NULL) \
		for (i=0;i<G_N_ELEMENTS(INSTVALUE);i++) \
			res[modi].new_length += INSTVALUE[i].GetLength(); \
	offsetmodif = res[modi].new_length+frompos-topos; \
	res[modi].new_length += methinfo.code_size+frompos-topos; \
	res[modi].value = new uint8_t[res[modi].new_length]; \
	res[modi].code_block_pos = res[modi].position+frompos; \
	dllfile.seekg(res[modi].position); \
	currentpos = 0; \
	BufferInitPosition(); \
	while (currentpos<methinfo.code_size) { \
		if (currentpos==frompos && INSTVALUE!=MACROINST_NULL) { \
			for (i=0;i<G_N_ELEMENTS(INSTVALUE);i++) \
				INSTVALUE[i].AppendToBuffer(res[modi].value); \
		} \
		ILInstruction inst; \
		inst.Read(dllfile); \
		nextpos = currentpos+inst.GetLength(); \
		if (currentpos<frompos || currentpos>=topos) { \
			ILCode& instcode = inst.GetCode(); \
			if (instcode.type==ILT_SWITCH) { \
				for (i=0;i<inst.param;i++) \
					if (currentpos<frompos && nextpos+inst.switchpos[i]>=topos) \
						inst.switchpos[i] += offsetmodif; \
					else if (currentpos>=topos && nextpos+inst.switchpos[i]<frompos) \
						inst.switchpos[i] -= offsetmodif; \
			} else if (instcode.type==ILT_OFFSET) { \
				if (currentpos<frompos && nextpos+inst.param>=topos) \
					inst.param += offsetmodif; \
				else if (currentpos>=topos && nextpos+inst.param<frompos) \
					inst.param -= offsetmodif; \
			} \
			inst.AppendToBuffer(res[modi].value); \
		} \
		currentpos = nextpos; \
	} \
	modi++;

//=============================//
//         Particular          //
//=============================//

struct CILMacroCustomBackgrounds : public CILMacroBaseStruct {
	
	uint16_t resolution;
	
	void SetParameters(uint32_t* intparam) { resolution = intparam[0]; }
	uint16_t GetSaveFlag() { return 0x1; }
	void ReadHWS(fstream& ffhws, uint16_t flag) {
		uint16_t macrosize;
		HWSReadShort(ffhws,macrosize);
		if (macrosize<2) {
			ffhws.seekg(macrosize,ios::cur);
			return;
		}
		HWSReadShort(ffhws,resolution);
		macrosize -= 2;
	}
	void WriteHWS(fstream& ffhws) {
		HWSWriteShort(ffhws,2);
		HWSWriteLong(ffhws,resolution);
	}
	
	unsigned int GetMethodCount() {
		return 3;
	}
	string GetMethodTypeName(unsigned int index) {
		switch (index) {
			case 0:	return "BGSCENE_DEF";
			case 1:	return "BGSCENE_DEF";
			case 2:	return "BGSCENE_DEF";
		}
		return "";
	}
	string GetMethodName(unsigned int index) {
		switch (index) {
			case 0:	return "ExtractSpriteData";
			case 1:	return "LoadEBG";
			case 2:	return "_LoadDummyEBG";
		}
		return "";
	}
	DllMetaDataModification* ComputeModifications(unsigned int* modifamount) {
		MACRO_CILMACRO_COMPUTEBEG()
		ILInstruction srchbeginstlde[] = {
			{ 0x02 },
			{ 0x07 },
			{ 0x28, data->GetMethodTokenIdentifier("BGSCENE_DEF","ExtractOverlayData") }
		};
		ILInstruction srchendinstlde[] = {
			{ 0x11, 12 },
			{ 0x11, 7 },
			{ 0x3E, 0 }
		};
		ILInstruction replinstlde[] = {
			{ 0x03 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGSCENE_DEF","atlas") },
			{ 0x6F, data->GetMemberTokenIdentifier("int UnityEngine.Texture::get_width()") },
			{ 0x13, 4 },
			{ 0x03 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGSCENE_DEF","atlas") },
			{ 0x6F, data->GetMemberTokenIdentifier("int UnityEngine.Texture::get_height()") },
			{ 0x13, 5 },
			{ 0x0E, 4 },
			{ 0x7B, data->GetFieldTokenIdentifier("FieldMapLocalizeAreaTitleInfo","startOvrIdx") },
			{ 0x13, 6 },
			{ 0x0E, 4 },
			{ 0x7B, data->GetFieldTokenIdentifier("FieldMapLocalizeAreaTitleInfo","endOvrIdx") },
			{ 0x13, 7 },
			{ 0x0E, 4 },
			{ 0x7B, data->GetFieldTokenIdentifier("FieldMapLocalizeAreaTitleInfo","hasUK") },
			{ 0x13, 8 },
			{ 0x0E, 4 },
			{ 0x0E, 5 },
			{ 0x6F, data->GetMethodTokenIdentifier("FieldMapLocalizeAreaTitleInfo","GetSpriteStartIndex") },
			{ 0x13, 9 },
			{ 0x11, 4 },
			{ 0x1F, resolution+4 },
			{ 0x5B },
			{ 0x13, 10 },
			{ 0x11, 9 },
			{ 0x13, 11 },
			{ 0x11, 6 },
			{ 0x13, 12 },
			{ 0x38, 0xF3 },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGSCENE_DEF","overlayList") },
			{ 0x11, 12 },
			{ 0x6F, data->GetMemberTokenIdentifier("!0 System.Collections.Generic.List`1<class BGOVERLAY_DEF>::get_Item( int )") },
			{ 0x13, 13 },
			{ 0x07 },
			{ 0x6F, data->GetMemberTokenIdentifier("class System.IO.Stream System.IO.BinaryReader::get_BaseStream()") },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","prmOffset") },
			{ 0x6E },
			{ 0x16 },
			{ 0x6F, data->GetMemberTokenIdentifier("long System.IO.Stream::Seek( long, valuetype System.IO.SeekOrigin )") },
			{ 0x26 },
			{ 0x16 },
			{ 0x13, 14 },
			{ 0x38, 0x23 },
			{ 0x73, data->GetMethodTokenIdentifier("BGSPRITE_LOC_DEF",".ctor") },
			{ 0x13, 15 },
			{ 0x11, 15 },
			{ 0x07 },
			{ 0x6F, data->GetMethodTokenIdentifier("BGSPRITE_LOC_DEF","ReadData_BGSPRITE_DEF") },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","spriteList") },
			{ 0x11, 15 },
			{ 0x6F, data->GetMemberTokenIdentifier("void System.Collections.Generic.List`1<class BGSPRITE_LOC_DEF>::Add( !0 )") },
			{ 0x11, 14 },
			{ 0x17 },
			{ 0x58 },
			{ 0x13, 14 },
			{ 0x11, 14 },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","spriteCount") },
			{ 0x3F, 0xFFFFFFCF },
			{ 0x07 },
			{ 0x6F, data->GetMemberTokenIdentifier("class System.IO.Stream System.IO.BinaryReader::get_BaseStream()") },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","locOffset") },
			{ 0x6E },
			{ 0x16 },
			{ 0x6F, data->GetMemberTokenIdentifier("long System.IO.Stream::Seek( long, valuetype System.IO.SeekOrigin )") },
			{ 0x26 },
			{ 0x16 },
			{ 0x13, 16 },
			{ 0x38, 0x65 },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","spriteList") },
			{ 0x11, 16 },
			{ 0x6F, data->GetMemberTokenIdentifier("!0 System.Collections.Generic.List`1<class BGSPRITE_LOC_DEF>::get_Item( int )") },
			{ 0x13, 17 },
			{ 0x11, 17 },
			{ 0x07 },
			{ 0x6F, data->GetMethodTokenIdentifier("BGSPRITE_LOC_DEF","ReadData_BGSPRITELOC_DEF") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGSCENE_DEF","useUpscaleFM") },
			{ 0x39, 0x3C },
			{ 0x11, 17 },
			{ 0x18 },
			{ 0x11, 11 },
			{ 0x11, 10 },
			{ 0x5D },
			{ 0x1F, resolution+4 },
			{ 0x5A },
			{ 0x58 },
			{ 0xD1 },
			{ 0x7D, data->GetFieldTokenIdentifier("BGSPRITE_LOC_DEF","atlasX") },
			{ 0x11, 17 },
			{ 0x18 },
			{ 0x11, 11 },
			{ 0x11, 10 },
			{ 0x5B },
			{ 0x1F, resolution+4 },
			{ 0x5A },
			{ 0x58 },
			{ 0xD1 },
			{ 0x7D, data->GetFieldTokenIdentifier("BGSPRITE_LOC_DEF","atlasY") },
			{ 0x11, 17 },
			{ 0x1F, resolution },
			{ 0x7D, data->GetFieldTokenIdentifier("BGSPRITE_LOC_DEF","w") },
			{ 0x11, 17 },
			{ 0x1F, resolution },
			{ 0x7D, data->GetFieldTokenIdentifier("BGSPRITE_LOC_DEF","h") },
			{ 0x11, 11 },
			{ 0x17 },
			{ 0x58 },
			{ 0x13, 11 },
			{ 0x11, 16 },
			{ 0x17 },
			{ 0x58 },
			{ 0x13, 16 },
			{ 0x11, 16 },
			{ 0x11, 13 },
			{ 0x7B, data->GetFieldTokenIdentifier("BGOVERLAY_DEF","spriteCount") },
			{ 0x3F, 0xFFFFFF8D },
			{ 0x11, 12 },
			{ 0x17 },
			{ 0x58 },
			{ 0x13, 12 },
			{ 0x11, 12 },
			{ 0x11, 7 },
			{ 0x3E, 0xFFFFFF04 }
		};
		for (modi=0;modi<2;modi++) {
			ILInstruction ilinst;
			MACRO_CILMACRO_READMETHODINFO(NULL)
			res[modi].position = dllfile.tellg();
			res[modi].method_id = absmethid;
			res[modi].base_length = methinfo.code_size;
			res[modi].new_length = methinfo.code_size;
			res[modi].value = new uint8_t[res[modi].new_length];
			BufferInitPosition();
			while (dllfile.tellg()<methinfo.code_abs_offset+methinfo.code_size) {
				ilinst.Read(dllfile);
				if (ilinst.opcode==0x1F && ilinst.param==32)
					ilinst.param = resolution;
				else if (ilinst.opcode==0x1F && ilinst.param==36)
					ilinst.param = resolution+4;
				ilinst.AppendToBuffer(res[modi].value);
			}
		}
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACE(srchbeginstlde,srchendinstlde,replinstlde)
		if (modifamount)
			*modifamount = modamount;
		return res;
	};
};

struct CILMacroDisableCheat : public CILMacroBaseStruct {
	
	bool disable_booster[8]; // speedup, battle assistance, etc... then speedup advantage
	
	uint16_t GetSaveFlag() { return 0x0; }
	void ReadHWS(fstream& ffhws, uint16_t flag) {
		unsigned int i;
		uint16_t macrosize;
		uint8_t tmp8;
		HWSReadShort(ffhws,macrosize);
		if (macrosize<1) {
			disable_booster[0] = false;
			for (i=1;i<8;i++)
				disable_booster[i] = true;
			ffhws.seekg(macrosize,ios::cur);
			return;
		}
		HWSReadChar(ffhws,tmp8);
		for (i=0;i<8;i++)
			disable_booster[i] = (tmp8 & (1 << i));
		macrosize--;
	}
	void WriteHWS(fstream& ffhws) {
		unsigned int i;
		uint8_t tmp8 = 0;
		HWSWriteShort(ffhws,1);
		for (i=0;i<8;i++)
			tmp8 |= disable_booster[i] ? (1 << i) : 0;
		HWSWriteChar(ffhws,tmp8);
	}
	
	unsigned int GetMethodCount() {
		return 4;
	}
	string GetMethodTypeName(unsigned int index) {
		switch (index) {
			case 0:	return "SettingsState";
			case 1:	return "ConfigUI";
			case 2:	return "TimerUI";
			case 3:	return "UIKeyTrigger";
			// ConfigUI::Awake and such for greying the booster buttons
		}
		return "";
	}
	string GetMethodName(unsigned int index) {
		switch (index) {
			case 0:	return "CallBoosterButtonFuntion";
			case 1:	return "OnKeyConfirm";
			case 2:	return "Update";
			case 3:	return "HandleBoosterButton";
		}
		return "";
	}
	DllMetaDataModification* ComputeModifications(unsigned int* modifamount) {
		MACRO_CILMACRO_COMPUTEBEG()
		ILInstruction modinstcbbf[] = {
			{ 0x03 },
			{ 0x0A },
			{ 0x06 },
			{ 0x45, 6, new int32_t[6]{ 0, 5, 0x11, 0, 0, 0x2F } },
			{ 0x38, 0x48 },
			{ 0x02 },
			{ 0x04 },
			{ 0x28, data->GetMethodTokenIdentifier("SettingsState","SetFastForward") },
			{ 0x38, 0x3C },
			{ 0x28, data->GetMethodTokenIdentifier("FF9StateSystem","get_Settings") },
			{ 0x7B, data->GetFieldTokenIdentifier("SettingsState","IsBoosterButtonActive") },
			{ 0x03 },
			{ 0x04 },
			{ 0x9C },
			{ 0x28, data->GetMethodTokenIdentifier("UIManager","get_Input") },
			{ 0x1C },
			{ 0x17 },
			{ 0x6F, data->GetMethodTokenIdentifier("UIKeyTrigger","SendKeyCode") },
			{ 0x38, 0x1E },
			{ 0x28, data->GetMethodTokenIdentifier("FF9StateSystem","get_Settings") },
			{ 0x7B, data->GetFieldTokenIdentifier("SettingsState","IsBoosterButtonActive") },
			{ 0x03 },
			{ 0x04 },
			{ 0x9C },
			{ 0x28, data->GetMethodTokenIdentifier("UIManager","get_Input") },
			{ 0x1D },
			{ 0x17 },
			{ 0x6F, data->GetMethodTokenIdentifier("UIKeyTrigger","SendKeyCode") },
			{ 0x38, 0 },
			{ 0x2A }
		};
		ILInstruction srchbeginstokc[] = {
			{ 0x03 },
			{ 0x28, data->GetMethodTokenIdentifier("GameObjectExtension","GetParent") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("ConfigUI","BoosterPanel") },
			{ 0x28, data->GetMemberTokenIdentifier("bool UnityEngine.Object::op_Equality( class UnityEngine.Object, class UnityEngine.Object )") },
			{ 0x39, 0 }
		};
		ILInstruction srchendinstokc[] = {
			{ 0x1F, 9 },
			{ 0x02 },
			{ 0xFE06, data->GetMethodTokenIdentifier("ConfigUI","AfterBoosterFinish") },
			{ 0x73, data->GetMemberTokenIdentifier("void System.Action::.ctor( object, intptr )") },
			{ 0x6F, data->GetMethodTokenIdentifier("BoosterSlider","ShowWaringDialog") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("ConfigUI","hitpointScreenButton") },
			{ 0x1F, 15 },
			{ 0x7D, data->GetFieldTokenIdentifier("OnScreenButton","KeyCommand") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("ConfigUI","WarningDialogHitPoint") },
			{ 0x17 },
			{ 0x6F, data->GetMemberTokenIdentifier("void UnityEngine.GameObject::SetActive( bool )") }
		};
		ILInstruction srchinsttuiu[] = {
			{ 0x28, data->GetMethodTokenIdentifier("FF9StateSystem","get_Settings") },
			{ 0x7B, data->GetFieldTokenIdentifier("SettingsState","time") },
			{ 0x7E, data->GetFieldTokenIdentifier("TimerUI","lastTime") },
			{ 0x59 }
		};
		ILInstruction appinsttuiu[] = {
			{ 0x28, data->GetMethodTokenIdentifier("FF9StateSystem","get_Settings") },
			{ 0x6F, data->GetMethodTokenIdentifier("SettingsState","get_FastForwardFactor") },
			{ 0x6C },
			{ 0x5A }
		};
		ILInstruction srchbeginsthbb[] = {
			{ 0x7B, data->GetFieldTokenIdentifier("UIManager","Booster") },
			{ 0x17 },
			{ 0x07 },
			{ 0x6F, data->GetMethodTokenIdentifier("BoosterSlider","SetBoosterButton") }
		};
		ILInstruction srchendinsthbb[] = {
			{ 0x7B, data->GetFieldTokenIdentifier("UIManager","Booster") },
			{ 0x1F, 9 },
			{ 0x14 },
			{ 0x6F, data->GetMethodTokenIdentifier("BoosterSlider","ShowWaringDialog") }
		};
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACEALL(modinstcbbf)
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACE(srchbeginstokc,srchendinstokc,MACROINST_NULL)
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACE(srchinsttuiu,MACROINST_NULL,appinsttuiu)
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACE(srchbeginsthbb,srchendinsthbb,MACROINST_NULL)
		if (modifamount)
			*modifamount = modamount;
		return res;
	};
};

struct CILMacroUnlockAbilityAccess : public CILMacroBaseStruct {
	
	unsigned int GetMethodCount() {
		return 1;
	}
	string GetMethodTypeName(unsigned int index) {
		switch (index) {
			case 0:	return "ff9abil";
			// JsonParser::ParseCommonDataToJson for save format
		}
		return "";
	}
	string GetMethodName(unsigned int index) {
		switch (index) {
			case 0:	return "FF9Abil_HasAp";
		}
		return "";
	}
	DllMetaDataModification* ComputeModifications(unsigned int* modifamount) {
		MACRO_CILMACRO_COMPUTEBEG()
		ILInstruction modinst[] = {
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER","info") },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER_INFO","menu_type") },
			{ 0x1F, 16 },
			{ 0x3C, 0x3C },
			{ 0x7E, data->GetFieldTokenIdentifier("ff9abil","_FF9Abil_PaData") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER","info") },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER_INFO","menu_type") },
			{ 0x9A },
			{ 0x16 },
			{ 0x9A },
			{ 0x7B, data->GetFieldTokenIdentifier("PA_DATA","id") },
			{ 0x39, 0x1F },
			{ 0x7E, data->GetFieldTokenIdentifier("ff9abil","_FF9Abil_PaData") },
			{ 0x02 },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER","info") },
			{ 0x7B, data->GetFieldTokenIdentifier("PLAYER_INFO","menu_type") },
			{ 0x9A },
			{ 0x16 },
			{ 0x9A },
			{ 0x7B, data->GetFieldTokenIdentifier("PA_DATA","max_ap") },
			{ 0x39, 2 },
			{ 0x17 },
			{ 0x2A },
			{ 0x16 },
			{ 0x2A }
		};
		MACRO_CILMACRO_READMETHODINFO("FF9")
		MACRO_CILMACRO_COMPUTEREPLACEALL(modinst)
		if (modifamount)
			*modifamount = modamount;
		return res;
	};
};

struct CILMacroDamageLimit : public CILMacroBaseStruct {
	
	unsigned int GetMethodCount() {
		return 6;
	}
	string GetMethodTypeName(unsigned int index) {
		switch (index) {
			case 0:	return "btl_calc";
			case 1:	return "btl_calc";
			case 2:	return "btl_calc";
			case 3:	return "btl_calc";
			case 4:	return "btl_calc";
			case 5:	return "btl_calc";
		}
		return "";
	}
	string GetMethodName(unsigned int index) {
		switch (index) {
			case 0:	return "CalcMain";
			case 1:	return "CalcResult";
			case 2:	return "CalcSub_200";
			case 3:	return "CalcSub_202";
			case 4:	return "CalcSub_203";
			case 5:	return "CalcSub_208";
		}
		return "";
	}
	DllMetaDataModification* ComputeModifications(unsigned int* modifamount) {
		MACRO_CILMACRO_COMPUTEBEG()
		for (modi=0;modi<modamount;modi++) {
			ILInstruction ilinst;
			MACRO_CILMACRO_READMETHODINFO(NULL)
			res[modi].position = dllfile.tellg();
			res[modi].method_id = absmethid;
			res[modi].base_length = methinfo.code_size;
			res[modi].new_length = methinfo.code_size;
			res[modi].value = new uint8_t[res[modi].new_length];
			BufferInitPosition();
			while (dllfile.tellg()<methinfo.code_abs_offset+methinfo.code_size) {
				ilinst.Read(dllfile);
				if (ilinst.opcode==0x20 && ilinst.param==9999)
					ilinst.param = 32767;
				ilinst.AppendToBuffer(res[modi].value);
			}
		}
		if (modifamount)
			*modifamount = modamount;
		return res;
	};
};

/*
struct CILMacroTEMPLATE : public CILMacroBaseStruct {
	
	uint16_t GetSaveFlag() { return 0x1; }
	void ReadHWS(fstream& ffhws, uint16_t flag) {
		uint16_t macrosize;
		HWSReadShort(ffhws,macrosize);
	}
	void WriteHWS(fstream& ffhws) { HWSWriteShort(ffhws,0); }
	
	unsigned int GetMethodCount() {
		return 0;
	}
	string GetMethodTypeName(unsigned int index) {
		switch (index) {
			case 0:	return "";
		}
		return "";
	}
	string GetMethodName(unsigned int index) {
		switch (index) {
			case 0:	return "";
		}
		return "";
	}
	DllMetaDataModification* ComputeModifications(unsigned int* modifamount) {
		MACRO_CILMACRO_COMPUTEBEG()
		ILInstruction modinst[] = {
			{ 0x7E, data->GetFieldTokenIdentifier("","") },
			{ 0x02 },
		};
		MACRO_CILMACRO_READMETHODINFO(NULL)
		MACRO_CILMACRO_COMPUTEREPLACEALL(modinst)
		if (modifamount)
			*modifamount = modamount;
		return res;
	};
};
*/

//=============================//
//          CILMacro           //
//=============================//

CILMacro::CILMacro() :
	type(NULL),
	info(NULL),
	data(NULL),
	isdeletedforcopy(false) {
}

void CILMacro::Init(uint32_t macroid, DllMetaData* d) {
	unsigned int i;
	data = d;
	for (i=0;i<G_N_ELEMENTS(CILMacroIDList);i++)
		if (CILMacroIDList[i].id==macroid) {
			type = &CILMacroIDList[i];
			break;
		}
	switch (macroid) {
		case 50: info = new CILMacroCustomBackgrounds; break;
		case 100: info = new CILMacroDisableCheat; break;
		case 101: info = new CILMacroUnlockAbilityAccess; break;
		case 102: info = new CILMacroDamageLimit; break;
	}
	if (info)
		info->parent = this;
}

CILMacro::CILMacro(uint32_t macroid, DllMetaData* d) :
	type(NULL),
	info(NULL),
	isdeletedforcopy(false) {
	Init(macroid,d);
}

CILMacro::~CILMacro() {
	if (info && !isdeletedforcopy)
		delete info;
}

int CILMacro::FindMethod(unsigned int tid, unsigned int mid) {
	// ToDo; handle the case of several methods sharing the same name and type
	string tname = data->GetTypeName(tid);
	string mname = data->GetMethodName(tid,mid);
	unsigned int i;
	for (i=0;i<info->GetMethodCount();i++)
		if (tname.compare(info->GetMethodTypeName(i))==0 && mname.compare(info->GetMethodName(i))==0)
			return i;
	return -1;
}

int CILMacro::FindMethodById(unsigned int absmethid) {
	unsigned int tid, mid;
	data->GetMethodRelativeId(absmethid,&tid,&mid);
	return FindMethod(tid,mid);
}
