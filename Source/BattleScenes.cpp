#include "BattleScenes.h"

#include <vector>
#include <sstream>
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"

#define MACRO_BATTLESCENE_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j; \
	uint32_t datapos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,total_size); \
	IO ## Short(f,face_amount); \
	IO ## Short(f,obj_amount); \
	IO ## Short(f,obj_offset); \
	IO ## Short(f,tim_amount); \
	IO ## Short(f,tim_offset); \
	IO ## Short(f,vert_amount); \
	IO ## Short(f,vert_offset); \
	IO ## Short(f,tex_amount); \
	IO ## Short(f,tex_offset); \
	IO ## Short(f,face_offset); \
	IO ## Short(f,tvert_offset); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		obj_tim_index = new uint16_t[obj_amount]; \
		obj_vert_amount = new uint16_t[obj_amount]; \
		obj_vert_index = new uint16_t[obj_amount]; \
		obj_tex_offset = new uint16_t[obj_amount]; \
		obj_face_offset = new uint16_t[obj_amount]; \
		obj_tvert_offset = new uint16_t[obj_amount]; \
		obj_quad_amount = new uint16_t[obj_amount]; \
		obj_trgl_amount = new uint16_t[obj_amount]; \
		obj_tex_unknown1 = new uint8_t*[obj_amount]; \
		obj_tex_unknown2 = new uint8_t*[obj_amount]; \
		obj_tex_unknown3 = new uint8_t*[obj_amount]; \
		obj_tex_id = new uint8_t*[obj_amount]; \
		obj_face_quadp = new uint16_t*[obj_amount]; \
		obj_face_trglp = new uint16_t*[obj_amount]; \
		obj_tvert_quadx = new uint8_t*[obj_amount]; \
		obj_tvert_quady = new uint8_t*[obj_amount]; \
		obj_tvert_trglx = new uint8_t*[obj_amount]; \
		obj_tvert_trgly = new uint8_t*[obj_amount]; \
		tim_palpos = new uint16_t[tim_amount]; \
		tim_texpos = new uint16_t[tim_amount]; \
		vert_x = new uint16_t[vert_amount]; \
		vert_y = new uint16_t[vert_amount]; \
		vert_z = new uint16_t[vert_amount]; \
	} \
	SEEK(f,datapos,obj_offset); \
	for (i=0;i<obj_amount;i++) { \
		uint32_t objpos = f.tellg(); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,obj_tim_index[i]); \
		IO ## Short(f,obj_vert_amount[i]); \
		IO ## Short(f,obj_vert_index[i]); \
		IO ## Short(f,obj_tex_offset[i]); \
		IO ## Short(f,obj_face_offset[i]); \
		IO ## Short(f,obj_tvert_offset[i]); \
		IO ## Short(f,obj_quad_amount[i]); \
		IO ## Short(f,obj_trgl_amount[i]); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			obj_tex_unknown1[i] = new uint8_t[obj_quad_amount[i]+obj_trgl_amount[i]]; \
			obj_tex_unknown2[i] = new uint8_t[obj_quad_amount[i]+obj_trgl_amount[i]]; \
			obj_tex_unknown3[i] = new uint8_t[obj_quad_amount[i]+obj_trgl_amount[i]]; \
			obj_tex_id[i] = new uint8_t[obj_quad_amount[i]+obj_trgl_amount[i]]; \
			obj_face_quadp[i] = new uint16_t[obj_quad_amount[i]*4]; \
			obj_face_trglp[i] = new uint16_t[obj_trgl_amount[i]*3]; \
			obj_tvert_quadx[i] = new uint8_t[obj_quad_amount[i]*4]; \
			obj_tvert_quady[i] = new uint8_t[obj_quad_amount[i]*4]; \
			obj_tvert_trglx[i] = new uint8_t[obj_trgl_amount[i]*3]; \
			obj_tvert_trgly[i] = new uint8_t[obj_trgl_amount[i]*3]; \
		} \
		SEEK(f,objpos,obj_tex_offset[i]); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<obj_quad_amount[i]+obj_trgl_amount[i];j++) { \
			IO ## Char(f,obj_tex_unknown1[i][j]); \
			IO ## Char(f,obj_tex_unknown2[i][j]); \
			IO ## Char(f,obj_tex_unknown3[i][j]); \
			IO ## Char(f,obj_tex_id[i][j]); \
		} \
		if (PPF) PPFEndScanStep(); \
		SEEK(f,objpos,obj_face_offset[i]); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<obj_quad_amount[i]*4;j++) \
			IO ## Short(f,obj_face_quadp[i][j]); \
		for (j=0;j<obj_trgl_amount[i]*3;j++) \
			IO ## Short(f,obj_face_trglp[i][j]); \
		if (PPF) PPFEndScanStep(); \
		SEEK(f,objpos,obj_tvert_offset[i]); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<obj_quad_amount[i]*4;j++) { \
			IO ## Char(f,obj_tvert_quadx[i][j]); \
			IO ## Char(f,obj_tvert_quady[i][j]); \
		} \
		for (j=0;j<obj_trgl_amount[i]*3;j++) { \
			IO ## Char(f,obj_tvert_trglx[i][j]); \
			IO ## Char(f,obj_tvert_trgly[i][j]); \
		} \
		if (PPF) PPFEndScanStep(); \
		SEEK(f,objpos,0x10); \
	} \
	SEEK(f,datapos,tim_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<tim_amount;i++) { \
		IO ## Short(f,tim_texpos[i]); \
		IO ## Short(f,tim_palpos[i]); \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(f,datapos,vert_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<vert_amount;i++) { \
		IO ## Short(f,vert_x[i]); \
		IO ## Short(f,vert_y[i]); \
		IO ## Short(f,vert_z[i]); \
	} \
	if (PPF) PPFEndScanStep();

void BattleSceneDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	MACRO_BATTLESCENE_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	loaded = true;
}

void BattleSceneDataStruct::Write(fstream& f) {
	MACRO_BATTLESCENE_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void BattleSceneDataStruct::WritePPF(fstream& f) {
	MACRO_BATTLESCENE_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void BattleSceneDataStruct::ReadHWS(fstream& f) {
	MACRO_BATTLESCENE_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void BattleSceneDataStruct::WriteHWS(fstream& f) {
	MACRO_BATTLESCENE_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

void BattleSceneDataStruct::UpdateOffset() {
	unsigned int i;
	obj_offset = 0x18;
	tim_offset = obj_offset+0x10*obj_amount;
	vert_offset = tim_offset+0x4*tim_amount;
	tex_offset = vert_offset+0x6*vert_amount;
	face_offset = tex_offset+0x4*tex_amount;
	tvert_offset = face_offset+0x2*face_amount;
	for (i=0;i<obj_amount;i++)
		tvert_offset += 0x2*(obj_trgl_amount[i]%2);
	uint16_t vert = 0;
	uint16_t tex = tex_offset-0x18;
	uint16_t face = face_offset-0x18;
	uint16_t tvert = tvert_offset-0x18;
	for (i=0;i<obj_amount;i++) {
		obj_vert_index[i] = vert;
		obj_tex_offset[i] = tex;
		obj_face_offset[i] = face;
		obj_tvert_offset[i] = tvert;
		vert += obj_vert_amount[i];
		tex += 0x4*(obj_quad_amount[i]+obj_trgl_amount[i])-0x10;
		face += 0x8*obj_quad_amount[i]+0x6*obj_trgl_amount[i]+0x2*(obj_trgl_amount[i]%2)-0x10;
		tvert += 0x8*obj_quad_amount[i]+0x6*obj_trgl_amount[i]+0x2*(obj_trgl_amount[i]%2)-0x10;
	}
	total_size = tvert_offset+0x2*face_amount;
	for (i=0;i<obj_amount;i++)
		total_size += 0x2*(obj_trgl_amount[i]%2);
	SetSize(total_size);
}

struct ObjectModelVertice {
	double x;
	double y;
	double z;
	void Read(wstring& line, LogStruct& log, uint32_t linenum);
};
struct ObjectModelTVertice {
	double x;
	double y;
	void Read(wstring& line, LogStruct& log, uint32_t linenum);
};
struct ObjectModelFacePoint {
	unsigned int v;
	unsigned int rel_v;
	unsigned int vt;
	unsigned int rel_vt;
	unsigned int n;
	unsigned int rel_n;
};
struct ObjectModelFace {
	std::vector<ObjectModelFacePoint> pt;
	void Read(wstring& line, LogStruct& log, uint32_t linenum);
};
struct ObjectModelObject;
struct ObjectModelImage {
	wstring name;
	uint32_t img_id;
	uint32_t face_id;
	void Read(wstring& line, LogStruct& log, uint32_t linenum);
};
struct ObjectModelObject {
	std::vector<ObjectModelVertice> vert;
	std::vector<ObjectModelTVertice> tvert;
	std::vector<ObjectModelFace> face;
	std::vector<ObjectModelImage> img;
};
struct ObjectModel {
	std::vector<ObjectModelObject> obj;
};
void ObjectModelVertice::Read(wstring& line, LogStruct& log, uint32_t linenum) {
	wstring word;
	std::size_t strpos;
	#define MACRO_READ_VERTICE_LOOP(DEST) { \
		strpos = line.find(L' '); \
		word = line.substr(0,strpos); \
		if (strpos!=std::string::npos) \
			line = line.substr(strpos+1); \
		wstringstream numbuffer(word); \
		if (word.empty()) { \
			wstringstream buffer; \
			buffer << L" - Expected decimal number at line " << linenum << L" ; treated as 0." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = 0; \
		} else if (!(numbuffer >> DEST)) { \
			wstringstream buffer; \
			buffer << L" - Couldn't read decimal number '" << word << L"' at line " << linenum << L"." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = 0; \
		} else if (DEST>327.67 || DEST<-327.68) { \
			wstringstream buffer; \
			buffer << L" - Decimal number '" << word << L"' at line " << linenum << L" is too large." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = DEST>327.67 ? 327.67 : -327.68; \
		} \
	}
	MACRO_READ_VERTICE_LOOP(this->x)
	MACRO_READ_VERTICE_LOOP(this->y)
	MACRO_READ_VERTICE_LOOP(this->z)
}
void ObjectModelTVertice::Read(wstring& line, LogStruct& log, uint32_t linenum) {
	wstring word;
	std::size_t strpos;
	#define MACRO_READ_TVERTICE_LOOP(DEST) { \
		strpos = line.find(L' '); \
		word = line.substr(0,strpos); \
		if (strpos!=std::string::npos) \
			line = line.substr(strpos+1); \
		wstringstream numbuffer(word); \
		if (word.empty()) { \
			wstringstream buffer; \
			buffer << L" - Expected decimal number at line " << linenum << L" ; treated as 0." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = 0; \
		} else if (!(numbuffer >> DEST)) { \
			wstringstream buffer; \
			buffer << L" - Couldn't read decimal number '" << word << L"' at line " << linenum << L"." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = 0; \
		} else if (DEST>1 || DEST<0) { \
			wstringstream buffer; \
			buffer << L" - TVertice '" << word << L"' at line " << linenum << L" should be between 0 and 1." << endl; \
			log.AddWarning(buffer.str()); \
			DEST = DEST>1 ? 1 : 0; \
		} \
	}
	MACRO_READ_TVERTICE_LOOP(this->x)
	MACRO_READ_TVERTICE_LOOP(this->y)
	this->y = 1-this->y;
}
void ObjectModelFace::Read(wstring& line, LogStruct& log, uint32_t linenum) {
	wstring word,wordsub;
	std::size_t strpos,strpossub;
	strpos = line.find(L' ');
	bool end = false;
	while (!end) {
		if (strpos==std::string::npos)
			end = true;
		word = line.substr(0,strpos);
		line = line.substr(strpos+1);
		if (word.empty())
			continue;
		ObjectModelFacePoint newpt;
		#define MACRO_READ_FACE_POINT_LOOP(DEST) { \
			strpossub = word.find(L"/"); \
			wordsub = word.substr(0,strpossub); \
			if (strpossub!=std::wstring::npos) \
				word = word.substr(strpossub+1); \
			else \
				word.clear(); \
			wstringstream numbuffer(wordsub); \
			if (wordsub.empty()) { \
				wstringstream buffer; \
				buffer << L" - Unexpected face description at " << linenum << L"." << endl; \
				log.AddError(buffer.str()); \
				DEST = 0; \
			} else if (!(numbuffer >> DEST)) { \
				wstringstream buffer; \
				buffer << L" - Couldn't read integer '" << wordsub << L"' at line " << linenum << L"." << endl; \
				log.AddError(buffer.str()); \
				DEST = 0; \
			} \
		}
		MACRO_READ_FACE_POINT_LOOP(newpt.v)
		MACRO_READ_FACE_POINT_LOOP(newpt.vt)
		newpt.n = 0;
		this->pt.push_back(newpt);
		strpos = line.find(L' ');
	}
}
void ObjectModelImage::Read(wstring& line, LogStruct& log, uint32_t linenum) {
	this->name = line.substr(0,line.find(L' '));
	if (this->name.empty()) {
		wstringstream buffer;
		buffer << L" - Expected image identifier at line " << linenum << L"." << endl;
		log.AddError(buffer.str());
	}
}

int BattleSceneDataStruct::Export(const char* outputbase, bool usequads) {
	unsigned int i,j;
	char buffer[256];
	char* filename;
	for (i=strlen(outputbase)-1;i>0;i--)
		if (outputbase[i] == '/' || outputbase[i]=='\\') {
			filename = (char*)outputbase+i+1;
			break;
		}
	if (parent->image[id])
		for (i=0;i<tim_amount;i++) {
			uint8_t texloc = 1;
			uint32_t* imgpal = parent->image[id][0].ConvertAsPalette(tim_palpos[i]);
			if (imgpal==NULL) {
				imgpal = parent->image[id][1].ConvertAsPalette(tim_palpos[i]);
				texloc = 0;
				if (imgpal==NULL)
					return 3;
			}
			sprintf(buffer,"%s_tex%d.tga",outputbase,i+1);
			if (parent->image[id][texloc].Export(buffer,false,tim_texpos[i],imgpal))
				return -i-1;
		}
	sprintf(buffer,"%s.obj",outputbase);
	fstream fobj(buffer,ios::out);
	if (!fobj.is_open())
		return 1;
	sprintf(buffer,"%s_mtl.lib",outputbase);
	fstream mtl(buffer,ios::out);
	if (!mtl.is_open()) {
		fobj.close();
		return 2;
	}
	for (i=0;i<tim_amount;i++) {
		mtl << "newmtl tex" << i+1 << endl;
		mtl << "Ka 1.000 1.000 1.000" << endl << "Kd 1.000 1.000 1.000" << endl << "Ks 0.000 0.000 0.000" << endl;
		mtl << "d 1.0" << endl << "illum 0" << endl;
		mtl << "map_Kd " << filename << "_tex" << i+1 << ".tga" << endl;
	}
	mtl.close();
	fobj << std::showpoint;
	fobj << "mtllib " << filename << "_mtl.lib" << endl;
	uint16_t vertcount = 1;
	uint16_t tvertcount = 1;
	for (i=0;i<obj_amount;i++) {
		fobj << "o Object_" << i+1 << endl;
		for (j=0;j<obj_vert_amount[i];j++) {
			double xx = int16_t(vert_x[obj_vert_index[i]+j])/100.0;
			double yy = int16_t(vert_y[obj_vert_index[i]+j])/100.0;
			double zz = int16_t(vert_z[obj_vert_index[i]+j])/100.0;
			fobj << "v " << xx << " " << -yy << " " << zz << endl;
		}
		for (j=0;j<obj_quad_amount[i]*4;j++) {
			double txx = obj_tvert_quadx[i][j]/255.0;
			double tyy = 1.0-obj_tvert_quady[i][j]/255.0;
			fobj << "vt " << txx << " " << tyy << endl;
		}
		for (j=0;j<obj_trgl_amount[i]*3;j++) {
			double txx = obj_tvert_trglx[i][j]/255.0;
			double tyy = 1.0-obj_tvert_trgly[i][j]/255.0;
			fobj << "vt " << txx << " " << tyy << endl;
		}
		uint8_t prevtex = (uint8_t)-1;
		uint8_t newtex;
		for (j=0;j<obj_quad_amount[i];j++) {
			newtex = obj_tex_id[i][j] & 0x1F;
			if (prevtex!=newtex) {
				prevtex = newtex;
				fobj << "usemtl tex" << (int)newtex+1 << endl;
			}
			if (usequads) {
				fobj << "f " << obj_face_quadp[i][j*4+2]/4+vertcount << "/" << tvertcount+2;
				fobj << " " << obj_face_quadp[i][j*4]/4+vertcount << "/" << tvertcount;
				fobj << " " << obj_face_quadp[i][j*4+1]/4+vertcount << "/" << tvertcount+1;
				fobj << " " << obj_face_quadp[i][j*4+3]/4+vertcount << "/" << tvertcount+3 << endl;
				tvertcount += 4;
			} else {
				fobj << "f " << obj_face_quadp[i][j*4]/4+vertcount << "/" << tvertcount++;
				fobj << " " << obj_face_quadp[i][j*4+1]/4+vertcount << "/" << tvertcount++;
				fobj << " " << obj_face_quadp[i][j*4+2]/4+vertcount << "/" << tvertcount++ << endl;
				fobj << "f " << obj_face_quadp[i][j*4+3]/4+vertcount << "/" << tvertcount--;
				fobj << " " << obj_face_quadp[i][j*4+2]/4+vertcount << "/" << tvertcount--;
				fobj << " " << obj_face_quadp[i][j*4+1]/4+vertcount << "/" << tvertcount << endl;
				tvertcount += 3;
			}
		}
		for (j=0;j<obj_trgl_amount[i];j++) {
			newtex = obj_tex_id[i][j+obj_quad_amount[i]] & 0x1F;
			if (prevtex!=newtex) {
				prevtex = newtex;
				fobj << "usemtl tex" << (int)newtex+1 << endl;
			}
			fobj << "f " << obj_face_trglp[i][j*3]/4+vertcount << "/" << tvertcount++;
			fobj << " " << obj_face_trglp[i][j*3+1]/4+vertcount << "/" << tvertcount++;
			fobj << " " << obj_face_trglp[i][j*3+2]/4+vertcount << "/" << tvertcount++ << endl;
		}
		vertcount += obj_vert_amount[i];
	}
	fobj.close();
	return 0;
}

LogStruct BattleSceneDataStruct::Import(const char* inputfile, unsigned int* numtex, wstring** texname) {
	unsigned int i,j;
	std::size_t strpos;
	LogStruct res = LogStruct();
	wfstream fobj;
	fobj.open(inputfile,ios::in);
	if (!fobj.is_open()) {
		wstringstream buffer;
		buffer << L" - Can't open the file '" << inputfile << L"'." << endl;
		res.AddError(buffer.str());
		return res;
	}
	ObjectModel tmpobj;
	wchar_t linec[1024];
	wstring line;
	wstring word;
	uint32_t obj = 0;
	uint32_t vert = 0;
	uint32_t vidoffset = 1;
	uint32_t tvert = 0;
	uint32_t vtidoffset = 1;
	uint32_t facet = 0;
	uint32_t faceq = 0;
	uint32_t objfacet[100];
	uint32_t objfaceq[100];
	uint32_t tim = 0;
	wstring* timname = new wstring[100];
	uint32_t linenum = 0;
	while (!fobj.eof()) {
		linenum++;
		fobj.getline(linec,1024);
		if (!linec[0])
			continue;
		line = linec;
		line = line.substr(line.find_first_not_of(L' '),line.find(L'#'));
		strpos = line.find(L' ');
		word = line.substr(0,strpos);
		if (line.empty())
			continue;
		#define MACRO_CREATE_NEW_OBJECT(MDL) { \
			ObjectModelObject* newobj = new ObjectModelObject(); \
			MDL.obj.push_back(*newobj); \
			objfacet[obj] = 0; \
			objfaceq[obj] = 0; \
			obj++; \
		}
		if (!word.compare(L"mtllib")) {
		} else if (!word.compare(L"o")) {
			if (!tmpobj.obj.empty()) {
				vidoffset += tmpobj.obj[obj-1].vert.size();
				vtidoffset += tmpobj.obj[obj-1].tvert.size();
				// Why? WHY?????
				// Doesn't seem to work otherwise...
				while (tmpobj.obj[obj-1].vert.size()%6) {
					ObjectModelVertice newvert;
					newvert.x = 0;
					newvert.y = 0;
					newvert.z = 0;
					tmpobj.obj[obj-1].vert.push_back(newvert);
					vert++;
				}
			}
			MACRO_CREATE_NEW_OBJECT(tmpobj);
		} else if (!word.compare(L"v")) {
			if (tmpobj.obj.empty())
				MACRO_CREATE_NEW_OBJECT(tmpobj);
			word = line.substr(strpos+1);
			ObjectModelVertice newvert;
			newvert.Read(word,res,linenum);
			tmpobj.obj[obj-1].vert.push_back(newvert);
			vert++;
		} else if (!word.compare(L"vt")) {
			if (tmpobj.obj.empty())
				MACRO_CREATE_NEW_OBJECT(tmpobj);
			word = line.substr(strpos+1);
			ObjectModelTVertice newtvert;
			newtvert.Read(word,res,linenum);
			tmpobj.obj[obj-1].tvert.push_back(newtvert);
			tvert++;
		} else if (!word.compare(L"f")) {
			if (tmpobj.obj.empty())
				MACRO_CREATE_NEW_OBJECT(tmpobj);
			word = line.substr(strpos+1);
			ObjectModelFace newface;
			newface.Read(word,res,linenum);
			for (i=0;i<newface.pt.size();i++) {
				if (newface.pt[i].v>=vidoffset)
					newface.pt[i].rel_v = newface.pt[i].v-vidoffset;
				else {
					wstringstream buffer;
					buffer << L" - Vertice number '" << newface.pt[i].v << L"' doesn't belong to the current object at line " << linenum << L"." << endl;
					res.AddError(buffer.str());
				}
				if (newface.pt[i].vt>=vtidoffset)
					newface.pt[i].rel_vt = newface.pt[i].vt-vtidoffset;
				else {
					wstringstream buffer;
					buffer << L" - TVertice number '" << newface.pt[i].vt << L"' doesn't belong to the current object at line " << linenum << L"." << endl;
					res.AddError(buffer.str());
				}
				newface.pt[i].rel_n = newface.pt[i].n;
			}
			tmpobj.obj[obj-1].face.push_back(newface);
			if (newface.pt.size()==3) {
				facet++;
				objfacet[obj-1]++;
			} else if (newface.pt.size()==4) {
				faceq++;
				objfaceq[obj-1]++;
			} else {
				wstringstream buffer;
				buffer << L" - Unhandled number of face points at line " << linenum << L"." << endl;
				res.AddWarning(buffer.str());
			}
		} else if (!word.compare(L"usemtl")) {
			if (tmpobj.obj.empty())
				MACRO_CREATE_NEW_OBJECT(tmpobj);
			word = line.substr(strpos+1);
			ObjectModelImage newimg;
			newimg.Read(word,res,linenum);
			newimg.face_id = tmpobj.obj[obj-1].face.size();
			bool newtim = true;
			for (i=0;i<tim;i++)
				if (!newimg.name.compare(timname[i])) {
					newtim = false;
					newimg.img_id = i;
					break;
				}
			if (newtim) {
				timname[tim] = newimg.name;
				newimg.img_id = tim++;
			}
			tmpobj.obj[obj-1].img.push_back(newimg);
		} else {
			wstringstream buffer;
			buffer << L" - Unrecognized object definition '" << word << L"' at line " << linenum << L"." << endl;
			res.AddWarning(buffer.str());
		}
	}
	if (obj>0)
		while (tmpobj.obj[obj-1].vert.size()%6) {
			ObjectModelVertice newvert;
			newvert.x = 0;
			newvert.y = 0;
			newvert.z = 0;
			tmpobj.obj[obj-1].vert.push_back(newvert);
			vert++;
		}
	uint32_t totalsize = 0x18+0x10*obj+0x6*vert+0x10*facet+0x14*faceq;
	for (i=0;i<obj;i++)
		totalsize += 0x4*(objfacet[i]%2);
	if (totalsize>GetExtraSize()+size-0x4*tim_amount) {
		wstringstream buffer;
		buffer << L" - Not enough space : the imported model takes " << totalsize << L" bytes for " << GetExtraSize()+size-0x4*tim_amount << L" bytes available." << endl;
		res.AddError(buffer.str());
	}
	fobj.close();
	if (res.ok) {
		face_amount = faceq*4+facet*3;
		obj_amount = obj;
//		tim_amount = tim;
		vert_amount = vert;
		tex_amount = faceq+facet;
//		obj_tim_index = new uint16_t[obj_amount];
		obj_vert_amount = new uint16_t[obj_amount];
		obj_vert_index = new uint16_t[obj_amount];
		obj_tex_offset = new uint16_t[obj_amount];
		obj_face_offset = new uint16_t[obj_amount];
		obj_tvert_offset = new uint16_t[obj_amount];
		obj_quad_amount = new uint16_t[obj_amount];
		obj_trgl_amount = new uint16_t[obj_amount];
		obj_tex_unknown1 = new uint8_t*[obj_amount];
		obj_tex_unknown2 = new uint8_t*[obj_amount];
		obj_tex_unknown3 = new uint8_t*[obj_amount];
		obj_tex_id = new uint8_t*[obj_amount];
		obj_face_quadp = new uint16_t*[obj_amount];
		obj_face_trglp = new uint16_t*[obj_amount];
		obj_tvert_quadx = new uint8_t*[obj_amount];
		obj_tvert_quady = new uint8_t*[obj_amount];
		obj_tvert_trglx = new uint8_t*[obj_amount];
		obj_tvert_trgly = new uint8_t*[obj_amount];
//		tim_palpos = new uint16_t[tim_amount];
//		tim_texpos = new uint16_t[tim_amount];
		vert_x = new uint16_t[vert_amount];
		vert_y = new uint16_t[vert_amount];
		vert_z = new uint16_t[vert_amount];
		uint32_t verti = 0;
		for (obj=0;obj<obj_amount;obj++) {
			obj_tex_unknown1[obj] = new uint8_t[objfaceq[obj]+objfacet[obj]];
			obj_tex_unknown2[obj] = new uint8_t[objfaceq[obj]+objfacet[obj]];
			obj_tex_unknown3[obj] = new uint8_t[objfaceq[obj]+objfacet[obj]];
			obj_tex_id[obj] = new uint8_t[objfaceq[obj]+objfacet[obj]];
			obj_face_quadp[obj] = new uint16_t[objfaceq[obj]*4];
			obj_face_trglp[obj] = new uint16_t[objfacet[obj]*3];
			obj_tvert_quadx[obj] = new uint8_t[objfaceq[obj]*4];
			obj_tvert_quady[obj] = new uint8_t[objfaceq[obj]*4];
			obj_tvert_trglx[obj] = new uint8_t[objfacet[obj]*3];
			obj_tvert_trgly[obj] = new uint8_t[objfacet[obj]*3];
//			obj_tim_index[obj] = 0;
			obj_vert_amount[obj] = tmpobj.obj[obj].vert.size();
			obj_quad_amount[obj] = objfaceq[obj];
			obj_trgl_amount[obj] = objfacet[obj];
			uint32_t curimg = 0;
			std::vector<ObjectModelImage>& tmpimg = tmpobj.obj[obj].img;
			for (i=0;i<obj_quad_amount[obj]+obj_trgl_amount[obj];i++) {
				for (j=0;j<tmpimg.size();j++)
					if (i==tmpimg[j].face_id) {
						curimg = tmpimg[j].img_id;
						break;
					}
				obj_tex_unknown1[obj][i] = 0x80;
				obj_tex_unknown2[obj][i] = 0x80;
				obj_tex_unknown3[obj][i] = 0x80;
				obj_tex_id[obj][i] = curimg;
			}
			uint32_t quadi = 0;
			uint32_t trgli = 0;
			for (i=0;i<tmpobj.obj[obj].face.size();i++)
				if (tmpobj.obj[obj].face[i].pt.size()==4) {
					obj_face_quadp[obj][quadi+2] = tmpobj.obj[obj].face[i].pt[0].rel_v*4;
					obj_tvert_quadx[obj][quadi+2] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[0].rel_vt].x*255);
					obj_tvert_quady[obj][quadi+2] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[0].rel_vt].y*255);
					obj_face_quadp[obj][quadi] = tmpobj.obj[obj].face[i].pt[1].rel_v*4;
					obj_tvert_quadx[obj][quadi] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[1].rel_vt].x*255);
					obj_tvert_quady[obj][quadi] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[1].rel_vt].y*255);
					obj_face_quadp[obj][quadi+1] = tmpobj.obj[obj].face[i].pt[2].rel_v*4;
					obj_tvert_quadx[obj][quadi+1] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[2].rel_vt].x*255);
					obj_tvert_quady[obj][quadi+1] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[2].rel_vt].y*255);
					obj_face_quadp[obj][quadi+3] = tmpobj.obj[obj].face[i].pt[3].rel_v*4;
					obj_tvert_quadx[obj][quadi+3] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[3].rel_vt].x*255);
					obj_tvert_quady[obj][quadi+3] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[3].rel_vt].y*255);
					quadi += 4;
				} else if (tmpobj.obj[obj].face[i].pt.size()==3)
					for (j=0;j<3;j++) {
						obj_face_trglp[obj][trgli] = tmpobj.obj[obj].face[i].pt[j].rel_v*4;
						obj_tvert_trglx[obj][trgli] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[j].rel_vt].x*255);
						obj_tvert_trgly[obj][trgli] = uint8_t(tmpobj.obj[obj].tvert[tmpobj.obj[obj].face[i].pt[j].rel_vt].y*255);
						trgli++;
					}
			for (i=0;i<tmpobj.obj[obj].vert.size();i++) {
				vert_x[verti] = uint16_t(int16_t(tmpobj.obj[obj].vert[i].x*100));
				vert_y[verti] = uint16_t(int16_t(tmpobj.obj[obj].vert[i].y*-100));
				vert_z[verti] = uint16_t(int16_t(tmpobj.obj[obj].vert[i].z*100));
				verti++;
			}
		}
//		for (i=0;i<tim_amount;i++) {
//			tim_texpos[i] = 
//			tim_palpos[i] = 
//		}
		*numtex = tim;
		*texname = timname;
	} else
		delete[] timname;
	return res;
}

void BattleSceneDataSet::Load(fstream& ffbin, ClusterSet& clusset) {
	unsigned int i,j,k,l;
	scene_amount = clusset.battle_scene_amount;
	cluster_id = new uint16_t[scene_amount];
	scene_name = new wstring[scene_amount];
	scene = new BattleSceneDataStruct*[scene_amount];
	image = new TIMImageDataStruct*[scene_amount];
	j = 0;
	LoadingDialogInit(scene_amount,_(L"Reading battle scenes..."));
	for (i=0;i<clusset.amount;i++) {
		if (clusset.clus_type[i]==CLUSTER_TYPE_BATTLE_SCENE) {
			ClusterData& clus = clusset.clus[i];
			cluster_id[j] = i;
			clus.CreateChildren(ffbin);
			for (k=0;k<clus.chunk_amount;k++) {
				for (l=0;l<clus.chunk[k].object_amount;l++) {
					ffbin.seekg(clus.chunk[k].object_offset[l]);
					clus.chunk[k].GetObject(l).Read(ffbin);
				}
			}
			ChunkData& chunkscene = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_BATTLE_SCENE)];
			scene[j] = (BattleSceneDataStruct*)&chunkscene.GetObject(0);
			if (clus.SearchChunkType(CHUNK_TYPE_TIM)>=0) {
				ChunkData& chunktim = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_TIM)];
				image[j] = (TIMImageDataStruct*)&chunktim.GetObject(0);
			} else
				image[j] = NULL;
			scene[j]->parent = this;
			scene[j]->id = j;
			for (k=0;k<G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME);k++)
				if (scene[j]->object_id==HADES_STRING_BATTLE_SCENE_NAME[k].id) {
					scene_name[j] = HADES_STRING_BATTLE_SCENE_NAME[k].label;
					break;
				}
			j++;
			LoadingDialogUpdate(j);
		}
	}
	LoadingDialogEnd();
}

void BattleSceneDataSet::Write(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<scene_amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].Write(ffbin);
	}
}

void BattleSceneDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<scene_amount;i++) {
		ffbin.seekg(clusset.clus[cluster_id[i]].offset);
		clusset.clus[cluster_id[i]].WritePPF(ffbin);
	}
}

int* BattleSceneDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i,j,k;
	uint32_t chunksize,clustersize,chunkpos,objectpos,objectsize;
	uint16_t nbmodified,objectid;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[3];
	res[0] = 0; res[1] = 0; res[2] = 0;
	HWSReadShort(ffhws,nbmodified);
	for (i=0;i<nbmodified;i++) {
		objectpos = ffhws.tellg();
		HWSReadShort(ffhws,objectid);
		HWSReadLong(ffhws,clustersize);
		for (j=0;j<scene_amount;j++) {
			if (objectid==scene[j]->object_id) {
				clus = scene[j]->parent_cluster;
				if (clustersize<=clus->size+clus->extra_size) {
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						chunkpos = ffhws.tellg();
						if (chunktype==CHUNK_TYPE_BATTLE_SCENE) {
							scene[j]->ReadHWS(ffhws);
							scene[j]->SetSize(chunksize);
						} else if (chunktype==CHUNK_TYPE_TIM && image[j]) {
							uint16_t timid;
							HWSReadShort(ffhws,timid);
							for (k=0;k<image[j]->parent_chunk->object_amount;k++)
								if (image[j][k].object_id==timid) {
									image[j][k].ReadHWS(ffhws);
									image[j][k].SetSize(chunksize-2);
								}
						} else
							res[1]++;
						ffhws.seekg(chunkpos+chunksize);
						HWSReadChar(ffhws,chunktype);
					}
				} else {
					objectsize = 7;
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						ffhws.seekg(chunksize,ios::cur);
						HWSReadChar(ffhws,chunktype);
						objectsize += chunksize+5;
					}
					ffhws.seekg(objectpos);
					backup.Add(ffhws,objectsize);
					res[0]++;
				}
				j = scene_amount;
			} else if (j+1==scene_amount) {
				objectsize = 7;
				HWSReadChar(ffhws,chunktype);
				while (chunktype!=0xFF) {
					HWSReadLong(ffhws,chunksize);
					ffhws.seekg(chunksize,ios::cur);
					HWSReadChar(ffhws,chunktype);
					objectsize += chunksize+5;
				}
				ffhws.seekg(objectpos);
				backup.Add(ffhws,objectsize);
				res[2]++;
			}
		}
	}
	return res;
}

void BattleSceneDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup) {
	unsigned int i,j;
	uint16_t nbmodified = 0;
	uint32_t chunkpos, nboffset = ffhws.tellg();
	ClusterData* clus;
	HWSWriteShort(ffhws,nbmodified);
	for (i=0;i<scene_amount;i++) {
		clus = scene[i]->parent_cluster;
		if (clus->modified) {
			clus->UpdateOffset();
			HWSWriteShort(ffhws,scene[i]->object_id);
			HWSWriteLong(ffhws,clus->size);
			if (scene[i]->modified) {
				HWSWriteChar(ffhws,CHUNK_TYPE_BATTLE_SCENE);
				HWSWriteLong(ffhws,scene[i]->size);
				chunkpos = ffhws.tellg();
				scene[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+scene[i]->size);
			}
			if (image[i]) {
				for (j=0;j<image[i]->parent_chunk->object_amount;j++)
					if (image[i][j].modified) {
						HWSWriteChar(ffhws,CHUNK_TYPE_TIM);
						HWSWriteLong(ffhws,image[i][j].size+2);
						chunkpos = ffhws.tellg();
						HWSWriteShort(ffhws,image[i][j].object_id);
						image[i][j].WriteHWS(ffhws);
						ffhws.seekg(chunkpos+image[i][j].size+2);
					}
			}
			HWSWriteChar(ffhws,0xFF);
			nbmodified++;
		}
	}
	for (i=0;i<backup.save_amount;i++)
		for (j=0;j<backup.save_size[i];j++)
			HWSWriteChar(ffhws,backup.save_data[i][j]);
	nbmodified += backup.save_amount;
	uint32_t endoffset = ffhws.tellg();
	ffhws.seekg(nboffset);
	HWSWriteShort(ffhws,nbmodified);
	ffhws.seekg(endoffset);
}
