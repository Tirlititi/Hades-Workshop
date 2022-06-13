#ifndef _FIELDS_H
#define _FIELDS_H

// Many thanks to yaz0r and Zidane_2 for sharing scripts about reading field backgrounds

#define FIELD_TILE_BASE_SIZE	16

struct FieldTilesAnimDataStruct;
struct FieldTilesTileDataStruct;
struct FieldTilesLightDataStruct;
struct FieldTilesCameraDataStruct;
struct FieldTilesDataStruct;

struct FieldWalkmeshDataStruct;
struct FieldWalkmeshTriple;
struct FieldRoleDataStruct;

struct FieldSteamTitleInfo;
struct FieldDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

#define WALKMESH_TRIFLAG_ACTIVE			0x0001
#define WALKMESH_TRIFLAG_VISITED		0x0080
#define WALKMESH_TRIFLAG_ALTERNATE_STEP	0x1000
#define WALKMESH_TRIFLAG_PREVENT_NPC	0x4000
#define WALKMESH_TRIFLAG_PREVENT_PC		0x8000

struct FieldTilesAnimDataStruct {
public:
	uint8_t flag;
	uint32_t tile_amount;
	uint8_t camera_id;
	uint32_t default_frame;
	int16_t rate;
	uint16_t counter;
	uint32_t tile_list_offset;
	uint8_t* tile_list;
	uint8_t* tile_duration;
};

struct FieldTilesTileDataStruct {
public:
	uint16_t width;
	uint16_t height;
	int16_t default_x;
	int16_t default_y;
	int16_t pos_x;
	int16_t pos_y;
	int16_t pos_minx;
	int16_t pos_maxx;
	int16_t pos_miny;
	int16_t pos_maxy;
	int16_t screen_x;
	int16_t screen_y;
	int16_t pos_dx;
	int16_t pos_dy;
	int16_t pos_fracx;
	int16_t pos_fracy;
	uint8_t camera_id;
	uint16_t tile_amount;
	uint32_t tile_pos_offset;
	uint32_t tile_data_offset;
	uint32_t tile_packet_offset;
	uint32_t tile_tpage;
	
	bool is_screen_static;
	bool use_attaching;
	bool is_looping;
	bool has_parallax;
	bool is_static;
	bool is_scroll_with_offset;
	uint32_t depth;
	uint32_t default_depth;
	bool is_x_offset;
	uint8_t viewport_id;
	
	uint16_t* tile_depth;
	uint16_t* tile_pos_y;
	uint16_t* tile_pos_x;
	
	uint16_t* tile_clut_y;
	uint8_t* tile_clut_x;
	uint8_t* tile_page_y;
	uint8_t* tile_page_x;
	uint8_t* tile_res;
	uint8_t* tile_alpha;
	uint8_t* tile_source_v;
	uint8_t* tile_source_u;
	uint16_t* tile_h;
	uint16_t* tile_w;
	bool* tile_trans;
	unsigned int* tile_steam_id;
	
	bool is_first_of_anim;
	uint8_t id;
	FieldTilesDataStruct* parent;
	
	FieldTilesTileDataStruct() : tile_data_data1(NULL), tile_data_data2(NULL), tile_data_data3(NULL) {}
	
private:
	uint32_t data1;
	uint8_t data2;
	uint32_t* tile_data_data1;
	uint32_t* tile_data_data2;
	uint32_t* tile_data_data3;
	
	void SetupDataInfos(bool readway);
	void AllocTileData();
	
	friend FieldTilesDataStruct;
	friend FieldSteamTitleInfo;
};

struct FieldTilesLightDataStruct {
public:
	
};

struct FieldTilesCameraDataStruct {
public:
	uint16_t distance;
	int16_t matrix[9];
	int32_t offset_x;
	int32_t offset_z;
	int32_t offset_y;
	int16_t offset_centerx;
	int16_t offset_centery;
	int16_t offset_width;
	int16_t offset_height;
	int16_t min_x;
	int16_t max_x;
	int16_t min_y;
	int16_t max_y;
	int32_t depth;
	
	int pos_x;
	int pos_y;
	unsigned int width;
	unsigned int height;
	
	void UpdateSize();
	
	uint8_t id;
	FieldTilesDataStruct* parent;
};

struct FieldTilesDataStruct : public ChunkChild {
public:
	// header
	uint16_t tiles_size;
	uint16_t depth_shift;
	uint16_t anim_amount;
	uint16_t tiles_amount;
	uint16_t light_amount;
	uint16_t camera_amount;
	uint32_t anim_offset;
	uint32_t tiles_offset;
	uint32_t light_offset;
	uint32_t camera_offset;
	uint16_t anim_unk1;
	uint16_t tiles_unk1;
	uint16_t light_unk1;
	uint16_t camera_unk1;
	uint16_t anim_unk2;
	uint16_t tiles_unk2;
	uint16_t light_unk2;
	uint16_t camera_unk2;
	uint16_t anim_unk3;
	uint16_t tiles_unk3;
	uint16_t light_unk3;
	uint16_t camera_unk3;
	FieldTilesAnimDataStruct* anim;
	FieldTilesTileDataStruct* tiles;
	FieldTilesLightDataStruct* light; // seems unused at all...
	FieldTilesCameraDataStruct* camera;
	
	FieldTilesTileDataStruct** tiles_sorted; // For ConvertAsImage: each tile block is a layer with a unique depth
	vector<vector<pair<int, int>>> tile_per_depth; // For ConvertAsImageAccurate: each tile has its own depth
	
	unsigned int title_tile_amount; // Steam only
	
	~FieldTilesDataStruct();
	void Copy(FieldTilesDataStruct& cpy);
	int GetRelatedTitleTileById(int tileid, SteamLanguage lang);
	uint32_t* ConvertAsImage(unsigned int cameraid, bool tileflag[] = NULL, bool showtp = false); // delete[] it
	uint32_t* ConvertAsImageAccurate(unsigned int cameraid, bool tileflag[] = NULL, bool showtp = false); // delete[] it
	int Export(const char* outputfile, unsigned int cameraid, bool tileflag[] = NULL, bool showtp = false, bool mergetiles = false, bool depthorder = true, int steamtitlelang = -1);
	void SetupDataInfos(bool readway);
	void SetupTilePerDepth();
	
	void Read(fstream& f, unsigned int titletileamount);
	void Read(fstream& f) { Read(f,0); }
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	
	unsigned int id;
	FieldDataSet* parent;
	
private:
	void AddTilesetToImage(uint32_t* imgdest, FieldTilesTileDataStruct& t, bool showtp = false, uint32_t* steamimg = NULL, uint32_t steamimgwidth = 1, uint32_t steamimgheight = 1);
	void AddTileToImage(uint32_t* imgdest, FieldTilesTileDataStruct& t, int tid, bool showtp = false, uint32_t* steamimg = NULL, uint32_t steamimgwidth = 1, uint32_t steamimgheight = 1);
};

struct FieldWalkmeshTriple {
	int16_t index[3];

	FieldWalkmeshTriple() : index{ -1, -1, -1 } {}
	FieldWalkmeshTriple(int16_t a, int16_t b, int16_t c) : index{ a, b, c } {}
};

struct FieldWalkmeshDataStruct : public ChunkChild {
public:
	uint32_t magic_walkmesh;
	int16_t datasize;
	int16_t offset_orgx;
	int16_t offset_orgz;
	int16_t offset_orgy;
	int16_t offset_x;
	int16_t offset_z;
	int16_t offset_y;
	int16_t offset_minx;
	int16_t offset_minz;
	int16_t offset_miny;
	int16_t offset_maxx;
	int16_t offset_maxz;
	int16_t offset_maxy;
	int16_t offset_charx;
	int16_t offset_charz;
	int16_t offset_chary;
	int16_t active_walkpath;
	int16_t active_triangle;
	
	uint16_t triangle_amount;
	uint16_t triangle_offset;
	uint16_t edge_amount;
	uint16_t edge_offset;
	uint16_t animation_amount;
	uint16_t animation_offset;
	uint16_t walkpath_amount;
	uint16_t walkpath_offset;
	uint16_t normal_amount;
	uint16_t normal_offset;
	uint16_t vertex_amount;
	uint16_t vertex_offset;
	
	vector<uint16_t> triangle_flag;
	vector<uint16_t> triangle_data;
	vector<uint16_t> triangle_walkpath;
	vector<int16_t> triangle_normal;
	vector<uint16_t> triangle_thetax;
	vector<uint16_t> triangle_thetay;
	vector<FieldWalkmeshTriple> triangle_vertex;
	vector<FieldWalkmeshTriple> triangle_edge;
	vector<FieldWalkmeshTriple> triangle_adjacenttriangle;
	vector<int16_t> triangle_centerx;
	vector<int16_t> triangle_centerz;
	vector<int16_t> triangle_centery;
	vector<int32_t> triangle_d;
	
	vector<uint16_t> edge_flag;
	vector<int16_t> edge_clone;
	
	vector<uint16_t> animation_flag;
	vector<uint16_t> animation_frameamount;
	vector<int16_t> animation_framerate;
	vector<uint16_t> animation_counter;
	vector<int32_t> animation_currentframe;
	vector<uint32_t> animation_frameoffset;
	vector<vector<uint16_t>> animation_frameflag;
	vector<vector<int16_t>> animation_framevalue;
	vector<vector<uint16_t>> animation_frametriangleamount;
	vector<vector<uint16_t>> animation_frametriangleoffset;
	vector<vector<vector<uint32_t>>> animation_frametriangle;
	
	vector<uint16_t> walkpath_flag;
	vector<uint16_t> walkpath_id;
	vector<int16_t> walkpath_orgx;
	vector<int16_t> walkpath_orgz;
	vector<int16_t> walkpath_orgy;
	vector<int16_t> walkpath_offsetx;
	vector<int16_t> walkpath_offsetz;
	vector<int16_t> walkpath_offsety;
	vector<int16_t> walkpath_minx;
	vector<int16_t> walkpath_minz;
	vector<int16_t> walkpath_miny;
	vector<int16_t> walkpath_maxx;
	vector<int16_t> walkpath_maxz;
	vector<int16_t> walkpath_maxy;
	vector<uint16_t> walkpath_triangleamount;
	vector<uint16_t> walkpath_trianglelistoffset;
	vector<vector<uint32_t>> walkpath_trianglelist;
	
	vector<int32_t> normal_x;
	vector<int32_t> normal_z;
	vector<int32_t> normal_y;
	vector<int32_t> normal_overz;
	
	vector<int16_t> vertex_x;
	vector<int16_t> vertex_z;
	vector<int16_t> vertex_y;

	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void UpdateOffset();

	void GetPathCoordinate(int coordtype, int pathid, int16_t& cx, int16_t& cy, int16_t& cz);
	void SetPathCoordinate(int coordtype, int pathid, int coordaxis, int16_t c);
	int AddWalkpath(uint32_t& extrasize, int16_t x, int16_t y, int16_t z);
	int AddTriangle2(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, int16_t adjtri1, int16_t adjtri2, int edgeid1, int edgeid2, int commonvert1, int commonvert2);
	int AddTriangle1(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, int16_t adjtri, int edgeid, int16_t newvertx, int16_t newverty, int16_t newvertz);
	int AddTriangle(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, array<int16_t, 3> newvertx, array<int16_t, 3> newverty, array<int16_t, 3> newvertz, array<int32_t, 4>* customnormal = NULL);
	int AddNormal(uint32_t& extrasize, uint16_t triangle, int32_t nx, int32_t ny, int32_t nz, int32_t noverz);
	void RemoveNormal(uint32_t& extrasize, uint16_t triangle);
	void ComputeNormal(uint16_t triangle);
	int ExportAsObj(string outputbase, FF9String fieldname, uint16_t fieldid);
	int ImportFromObj(wxString intputfilename, wxString* message = NULL);

private:
	void AddTriangleShared(uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, array<int32_t, 4>* customnormal = NULL);
};

struct FieldRoleDataStruct : public ChunkChild {
public:
	uint32_t magic_role;
	uint16_t zero1;
	uint8_t unk1_amount;
	uint8_t unk1_amount2;
	uint8_t unk2_amount;
	uint8_t unk2_amount2;
	uint16_t zero2;
	
	uint8_t* unk1_unknown1;
	uint8_t* unk1_unknown2;
	uint8_t* unk1_unknown3;
	uint8_t* unk1_unknown4;
	uint8_t* unk1_unknown5;
	uint8_t* unk1_unknown6;
	uint8_t* unk1_unknown7;
	uint8_t* unk1_unknown8;
	uint8_t* unk1_unknown9;
	uint8_t* unk1_unknown10;
	uint8_t* unk1_unknown11;
	uint8_t* unk1_unknown12;
	
	uint16_t* unk2_model;
	uint8_t* unk2_unknown1;
	uint8_t* unk2_unknown2;
	uint8_t* unk2_unknown3;
	uint8_t* unk2_unknown4;
	uint8_t* unk2_unknown5;
	uint8_t* unk2_unknown6;
	
	int AddModelRole(uint16_t modelid);
	void RemoveModelRole(uint16_t unk2id);
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

struct FieldSteamTitleInfo {
	uint32_t size;
	unsigned int amount;
	uint16_t* field_id;
	uint16_t* width;
	uint16_t* height;
	uint16_t* title_tile_start;
	uint16_t* title_tile_last;
	bool* has_uk;
	uint32_t* atlas_title_pos[STEAM_LANGUAGE_AMOUNT];
	uint32_t* atlas_title_amount[STEAM_LANGUAGE_AMOUNT];
	
	bool ReadTitleTileId(FieldTilesDataStruct* tileset, ConfigurationSet& config);
	
	void Read(fstream& f);
	void Write(fstream& f);
};

struct FieldDataSet {
public:
	uint16_t amount;
	uint16_t* cluster_id;
	uint16_t* struct_id;
	ScriptDataStruct** script_data;
	ImageMapDataStruct** preload;
	FieldTilesDataStruct** background_data;
	FieldWalkmeshDataStruct** walkmesh;
	TIMImageDataStruct** tim_data;
	FieldRoleDataStruct** role;
	
	TextDataStruct** related_text;
	FieldSteamTitleInfo* title_info; // Steam version only
	uint32_t name_space_used; // Steam version only
	
	unsigned int tile_size;
	unsigned int tile_gap;
	
	int SetFieldName(unsigned int fieldid, wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetFieldName(unsigned int fieldid, FF9String& newvalue);
	
	void Load(fstream& ffbin, ClusterSet& clusset, TextDataSet* textset = NULL);
	void Write(fstream& ffbin, ClusterSet& clusset);
	void WritePPF(fstream& ffbin, ClusterSet& clusset);
	// return value is int[5] to be deleted
	// {Number of oversized blocks, Number of unknown data, Number of unused blocks, Number of oversized parent block, Number of ignored Preload data}
	// Parent block is a data block containing preloading datas and TIM images in Field & World Map clusters.
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag);
	int GetSteamTextSize(SteamLanguage lang = GetSteamLanguage());
	void WriteSteamText(fstream& ffbin, SteamLanguage lang = GetSteamLanguage());
};

#endif
