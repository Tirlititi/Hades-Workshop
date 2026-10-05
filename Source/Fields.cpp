#include "Fields.h"

#define PSX_SCREEN_SIZE_X	640u
#define PSX_SCREEN_SIZE_Y	480u

#define HWS_ADDITION_INFO_VERSION_CURRENT	1

#include <sstream>
#include "Hades_Strings.h"
#include "Database_Steam.h"
#include "Database_CSV.h"
#include "Gui_LoadingDialog.h"
#include "TIMImages.h"
#include "main.h"
#include "CommonUtility.h"

void FieldTilesCameraDataStruct::UpdateSize() {
	unsigned int i, j;
	bool usescreen = false;
	int maxx = -0xFFFF;
	int maxy = -0xFFFF;
	pos_x = 0xFFFF;
	pos_y = 0xFFFF;
	for (i = 0; i < parent->tiles_amount; i++)
		if (parent->tiles[i].camera_id == id)
			for (j = 0; j < parent->tiles[i].tile_amount; j++) {
				if (parent->tiles[i].is_memoria_bgx != 0) {
					pos_x = min(pos_x, (int)parent->tiles[i].pos_x);
					pos_y = min(pos_y, (int)parent->tiles[i].pos_y);
					maxx = max(maxx, parent->tiles[i].pos_x + parent->tiles[i].width);
					maxy = max(maxy, parent->tiles[i].pos_y + parent->tiles[i].height);
				} else {
					pos_x = min(pos_x, parent->tiles[i].pos_x + parent->tiles[i].tile[j].pos_x);
					pos_y = min(pos_y, parent->tiles[i].pos_y + parent->tiles[i].tile[j].pos_y);
					maxx = max(maxx, parent->tiles[i].pos_x + parent->tiles[i].tile[j].pos_x + FIELD_TILE_BASE_SIZE);
					maxy = max(maxy, parent->tiles[i].pos_y + parent->tiles[i].tile[j].pos_y + FIELD_TILE_BASE_SIZE);
//					usescreen = usescreen || (parent->tiles[i].tile[j].res == 0 && parent->tiles[i].tile[j].trans && parent->tiles[i].tile[j].alpha == 2);
				}
			}
	width = pos_x < 0xFFFF ? maxx - pos_x : 0;
	height = pos_y < 0xFFFF ? maxy - pos_y : 0;
	if (GetGameType() != GAME_TYPE_PSX) {
		width = width * parent->parent->tile_size / FIELD_TILE_BASE_SIZE;
		height = height * parent->parent->tile_size / FIELD_TILE_BASE_SIZE;
//	} else if (usescreen) {
//		width = max(width, PSX_SCREEN_SIZE_X);
//		height = max(height, PSX_SCREEN_SIZE_Y);
	}
}

void FieldTilesDataStruct::Copy(FieldTilesDataStruct& cpy) {
	unsigned int i, j;
	*this = cpy;
	anim = new FieldTilesAnimDataStruct[anim_amount];
	tiles = new FieldTilesTilesetDataStruct[tiles_amount + title_tile_amount * STEAM_LANGUAGE_AMOUNT];
	tiles_sorted = new FieldTilesTilesetDataStruct*[tiles_amount];
	light = new FieldTilesLightDataStruct[light_amount];
	camera = new FieldTilesCameraDataStruct[camera_amount];
	for (i = 0; i < anim_amount; i++) {
		anim[i] = cpy.anim[i];
		anim[i].tile_list = new uint8_t[anim[i].tile_amount];
		anim[i].tile_duration = new uint8_t[anim[i].tile_amount];
		for (j = 0; j < anim[i].tile_amount; j++) {
			anim[i].tile_list[j] = cpy.anim[i].tile_list[j];
			anim[i].tile_duration[j] = cpy.anim[i].tile_duration[j];
		}
	}
	for (i = 0; i < tiles_amount + title_tile_amount * STEAM_LANGUAGE_AMOUNT; i++) {
		tiles[i] = cpy.tiles[i];
		tiles[i].tile = new FieldTilesTileDataStruct[tiles[i].tile_amount];
		for (j = 0; j < tiles[i].tile_amount; j++)
			tiles[i].tile[j] = cpy.tiles[i].tile[j];
	}
	for (i = 0; i < light_amount; i++) {
		light[i] = cpy.light[i];
	}
	for (i = 0; i < camera_amount; i++) {
		camera[i] = cpy.camera[i];
	}
	for (i = 0; i < title_tile_amount * STEAM_LANGUAGE_AMOUNT; i++)
		tiles[tiles_amount + i].SetupDataInfos(true);
	SetupDataInfos(true);
}

void FieldTilesDataStruct::AddTilesetToImage(uint32_t* imgdest, FieldTilesTilesetDataStruct& ts, bool showtp, uint32_t* steamimg, uint32_t steamimgwidth, uint32_t steamimgheight) {
	unsigned int i, x, y, pixelx, pixely, timtilex, timtiley, tilesize, tilegap, tileperiod;
	unsigned int camwidth = camera[ts.camera_id].width;
	unsigned int camheight = camera[ts.camera_id].height;
	uint32_t pix, psxalpha;
	TIM_BlendMode bm;
	bool psx = GetGameType() == GAME_TYPE_PSX;
	tilesize = parent->tile_size;
	tilegap = parent->tile_gap;
	tileperiod = tilesize + 2 * tilegap;
	if (ts.is_memoria_bgx != 0) {
		if (ts.memoria_bgx_shader == L"PSX/FieldMap_Abr_None") {
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_NONE;
		} else if (ts.memoria_bgx_shader == L"PSX/FieldMap_Abr_0") {
			psxalpha = 0x7F000000;
			bm = TIM_BLENDMODE_ABR_0;
		} else if (ts.memoria_bgx_shader == L"PSX/FieldMap_Abr_1") {
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_1;
		} else if (ts.memoria_bgx_shader == L"PSX/FieldMap_Abr_2") {
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_2;
		} else if (ts.memoria_bgx_shader == L"PSX/FieldMap_Abr_3") {
			psxalpha = 0x3F000000;
			bm = TIM_BLENDMODE_ABR_3;
		} else {
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_NONE;
		}
		unsigned int imgw = ts.width * parent->tile_size / FIELD_TILE_BASE_SIZE;
		unsigned int imgh = ts.height * parent->tile_size / FIELD_TILE_BASE_SIZE;
		wxImage img;
		{
			wxLogNull preventmsg;
			if (!img.LoadFile(ts.memoria_bgx_path))
				img.Create(imgw, imgh);
		}
		if (img.GetWidth() != imgw || img.GetHeight() != imgh)
			img.Rescale(imgw, imgh);
		if (!img.HasAlpha())
			img.InitAlpha();
		unsigned char* imgdata = img.GetData();
		unsigned char* imgalpha = img.GetAlpha();
		pixelx = ts.pos_x - camera[ts.camera_id].pos_x;
		pixely = ts.pos_y - camera[ts.camera_id].pos_y;
		if (!psx) {
			pixelx = (pixelx * tilesize) / FIELD_TILE_BASE_SIZE;
			pixely = (pixely * tilesize) / FIELD_TILE_BASE_SIZE;
		}
		float alphafactor;
		for (y = 0; y < imgh; y++) {
			if (pixely + y >= camheight)
				continue;
			for (x = 0; x < imgw; x++) {
				if (pixelx + x >= camwidth)
					continue;
				alphafactor = imgalpha[x + y * imgw] / 255.0f;
				pix = imgdata[3 * (x + y * imgw)] << 16;
				pix |= imgdata[3 * (x + y * imgw) + 1] << 8;
				pix |= imgdata[3 * (x + y * imgw) + 2];
				pix |= ((uint32_t)round(psxalpha * alphafactor) & 0xFF000000);
				imgdest[pixelx + x + (pixely + y) * camwidth] = ImageMergePixels(imgdest[pixelx + x + (pixely + y) * camwidth], pix, bm);
			}
		}
	} else {
		for (i = 0; i < ts.tile_amount; i++) {
			//AddTileToImage(imgdest, ts, i, showtp, steamimg, steamimgwidth, steamimgheight);
			pixely = ts.pos_y + ts.tile[i].pos_y - camera[ts.camera_id].pos_y;
			if (psx) {
				timtiley = ts.tile[i].page_y * 256 + ts.tile[i].source_v;
			} else {
				timtiley = (ts.tile[i].steam_id / (steamimgwidth / tileperiod)) * tileperiod + tilegap;
				pixely = (pixely * tilesize) / FIELD_TILE_BASE_SIZE;
			}
			//if (psx && ts.tile[i].trans && ts.tile[i].alpha == 2) // pixelpos or timtile is wrongly placed...
			if (ts.tile[i].trans) {
				switch (ts.tile[i].alpha) { // Before using ABR modes, default transparency was 0x80 before except for ABR 2 that was 0x1A
				case 0:
					psxalpha = 0x7F000000;
					bm = TIM_BLENDMODE_ABR_0;
					break;
				case 1:
					psxalpha = 0xFF000000;
					bm = TIM_BLENDMODE_ABR_1;
					break;
				case 2:
					psxalpha = 0xFF000000;
					bm = TIM_BLENDMODE_ABR_2;
					break;
				case 3:
					psxalpha = 0x3F000000;
					bm = TIM_BLENDMODE_ABR_3;
					break;
				}
			} else {
				psxalpha = 0xFF000000;
				bm = TIM_BLENDMODE_ABR_NONE;
			}
			if (GetFieldId() == 2259 && ts.id == 14) // Specially handled for some reason: Oeilvert/Star Display
				bm = TIM_BLENDMODE_ABR_NONE;
			for (y = 0; y < tilesize; y++) {
				pixelx = ts.pos_x + ts.tile[i].pos_x - camera[ts.camera_id].pos_x;
				if (psx) {
					timtilex = ts.tile[i].res > 0 ? ts.tile[i].page_x * 128 + ts.tile[i].source_u : ts.tile[i].page_x * 128 * 2 + ts.tile[i].source_u;
				} else {
					timtilex = (ts.tile[i].steam_id % (steamimgwidth / tileperiod)) * tileperiod + tilegap;
					pixelx = (pixelx * tilesize) / FIELD_TILE_BASE_SIZE;
				}
				for (x = 0; x < tilesize; x++) {
					if (ts.tile[i].res > 0) {
						if (psx)
							pix = TIMImageDataStruct::GetVRamPixel(timtilex, timtiley, ts.tile[i].clut_x, ts.tile[i].clut_y, false);
						else if (timtilex < steamimgwidth && timtiley < steamimgheight)
							pix = steamimg[timtilex + timtiley * steamimgwidth];
						else
							pix = 0;
						if (pix & 0xFF000000) {
							if (pixelx < camwidth && pixely < camheight) {
								if (psx)
									imgdest[pixelx + pixely * camwidth] = ImageMergePixels(imgdest[pixelx + pixely * camwidth], (pix & 0xFFFFFF) | psxalpha, bm);
								else
									imgdest[pixelx + pixely * camwidth] = ImageMergePixels(imgdest[pixelx + pixely * camwidth], pix, bm);
							}
						}
					} else {
						if (psx)
							pix = TIMImageDataStruct::GetVRamPixel(timtilex, timtiley, ts.tile[i].clut_x, ts.tile[i].clut_y, true);
						else if (timtilex < steamimgwidth && timtiley < steamimgheight)
							pix = steamimg[timtilex + timtiley * steamimgwidth];
						else
							pix = 0;
						if (showtp && (pix & 0xFF000000) && (pix != 0xFF000000)) {
							if (pixelx < camwidth && pixely < camheight) {
								if (psx)
									imgdest[pixelx + pixely * camwidth] = ImageMergePixels(imgdest[pixelx + pixely * camwidth], (pix & 0xFFFFFF) | psxalpha, bm);
								else
									imgdest[pixelx + pixely * camwidth] = ImageMergePixels(imgdest[pixelx + pixely * camwidth], pix, bm);
							}
						}
					}
					timtilex++;
					pixelx++;
				}
				timtiley++;
				pixely++;
			}
		}
	}
}

void FieldTilesDataStruct::AddTileToImage(uint32_t* imgdest, FieldTilesTilesetDataStruct& ts, int tid, bool showtp, uint32_t* steamimg, uint32_t steamimgwidth, uint32_t steamimgheight) {
	unsigned int x, y, pixelx, pixely, timtilex, timtiley;
	uint32_t pix, psxalpha;
	TIM_BlendMode bm;
	unsigned int tilesize = parent->tile_size;
	unsigned int tilegap = parent->tile_gap;
	unsigned int tileperiod = tilesize + 2 * tilegap;
	bool psx = GetGameType() == GAME_TYPE_PSX;
	pixely = ts.pos_y + ts.tile[tid].pos_y - camera[ts.camera_id].pos_y;
	if (psx) {
		timtiley = ts.tile[tid].page_y * 256 + ts.tile[tid].source_v;
	} else {
		timtiley = (ts.tile[tid].steam_id / (steamimgwidth / tileperiod)) * tileperiod + tilegap;
		pixely = (pixely * tilesize) / FIELD_TILE_BASE_SIZE;
	}
	//if (psx && ts.tile[tid].trans && ts.tile[tid].alpha == 2) // pixelpos or timtile is wrongly placed...
	if (ts.tile[tid].trans) {
		switch (ts.tile[tid].alpha) { // Before using ABR modes, default transparency was 0x80 before except for ABR 2 that was 0x1A
		case 0:
			psxalpha = 0x7F000000;
			bm = TIM_BLENDMODE_ABR_0;
			break;
		case 1:
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_1;
			break;
		case 2:
			psxalpha = 0xFF000000;
			bm = TIM_BLENDMODE_ABR_2;
			break;
		case 3:
			psxalpha = 0x3F000000;
			bm = TIM_BLENDMODE_ABR_3;
			break;
		}
	} else {
		psxalpha = 0xFF000000;
		bm = TIM_BLENDMODE_ABR_NONE;
	}
	if (GetFieldId() == 2259 && ts.id == 14) // Specially handled for some reason: Oeilvert/Star Display
		bm = TIM_BLENDMODE_ABR_NONE;
	for (y = 0; y < tilesize; y++) {
		pixelx = ts.pos_x + ts.tile[tid].pos_x - camera[ts.camera_id].pos_x;
		if (psx) {
			timtilex = ts.tile[tid].res > 0 ? ts.tile[tid].page_x * 128 + ts.tile[tid].source_u : ts.tile[tid].page_x * 128 * 2 + ts.tile[tid].source_u;
		} else {
			timtilex = (ts.tile[tid].steam_id % (steamimgwidth / tileperiod)) * tileperiod + tilegap;
			pixelx = (pixelx * tilesize) / FIELD_TILE_BASE_SIZE;
		}
		for (x = 0; x < tilesize; x++) {
			if (ts.tile[tid].res > 0) {
				if (psx)
					pix = TIMImageDataStruct::GetVRamPixel(timtilex, timtiley, ts.tile[tid].clut_x, ts.tile[tid].clut_y, false);
				else if (timtilex < steamimgwidth && timtiley < steamimgheight)
					pix = steamimg[timtilex + timtiley * steamimgwidth];
				else
					pix = 0;
				if (pix & 0xFF000000) {
					if (pixelx < camera[ts.camera_id].width && pixely < camera[ts.camera_id].height) {
						if (psx)
							imgdest[pixelx + pixely * camera[ts.camera_id].width] = ImageMergePixels(imgdest[pixelx + pixely * camera[ts.camera_id].width], (pix & 0xFFFFFF) | psxalpha, bm);
						else
							imgdest[pixelx + pixely * camera[ts.camera_id].width] = ImageMergePixels(imgdest[pixelx + pixely * camera[ts.camera_id].width], pix, bm);
					}
				}
			} else {
				if (psx)
					pix = TIMImageDataStruct::GetVRamPixel(timtilex, timtiley, ts.tile[tid].clut_x, ts.tile[tid].clut_y, true);
				else if (timtilex < steamimgwidth && timtiley < steamimgheight)
					pix = steamimg[timtilex + timtiley * steamimgwidth];
				else
					pix = 0;
				if (showtp && (pix & 0xFF000000) && (pix != 0xFF000000)) {
					if (pixelx < camera[ts.camera_id].width && pixely < camera[ts.camera_id].height) {
						if (psx)
							imgdest[pixelx + pixely * camera[ts.camera_id].width] = ImageMergePixels(imgdest[pixelx + pixely * camera[ts.camera_id].width], (pix & 0xFFFFFF) | psxalpha, bm);
						else
							imgdest[pixelx + pixely * camera[ts.camera_id].width] = ImageMergePixels(imgdest[pixelx + pixely * camera[ts.camera_id].width], pix, bm);
					}
				}
			}
			timtilex++;
			pixelx++;
		}
		timtiley++;
		pixely++;
	}
}

int FieldTilesDataStruct::GetRelatedTitleTileById(int tileid, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX || title_tile_amount == 0)
		return tileid;
	unsigned int infoid;
	for (infoid = 0; infoid < parent->title_info->amount; infoid++)
		if (parent->title_info->field_id[infoid] == GetFieldId()) {
			if (tileid < 0) {
				int tid = -tileid - 1;
				if (tid <= parent->title_info->title_tile_last[infoid] - parent->title_info->title_tile_start[infoid])
					return tiles_amount + lang * title_tile_amount - tid;
				return tileid;
			}
			if (tileid >= tiles_amount || tileid < parent->title_info->title_tile_start[infoid] || tileid > parent->title_info->title_tile_last[infoid])
				return tileid;
			return tiles_amount + lang * title_tile_amount + tileid - parent->title_info->title_tile_start[infoid];
		}
	return tileid;
}

uint32_t* FieldTilesDataStruct::ConvertAsImage(unsigned int cameraid, bool tileflag[], bool showtp) {
	unsigned int i, imgsize = camera[cameraid].width * camera[cameraid].height;
	TIMImageDataStruct* tim = parent->tim_data[id];
	uint32_t* res = new uint32_t[imgsize];
	uint32_t* rawimg = NULL;
	if (GetGameType() == GAME_TYPE_PSX)
		tim->LoadInVRam();
	else if (!IsBGXFormat())
		rawimg = tim->ConvertAsSteamImage();
	for (i = 0; i < imgsize; i++)
		res[i] = 0;
	for (i = 0; i < tiles_amount; i++) { // ToDo: mind the shader layering order ("QUEUE")
		FieldTilesTilesetDataStruct& ts = tiles[GetRelatedTitleTileById(tiles_sorted[i]->id, GetSteamLanguage())];
		if (ts.camera_id == cameraid && (tileflag == NULL || tileflag[ts.id]) && (tileflag != NULL || ts.is_static || ts.is_first_of_anim))
			AddTilesetToImage(res, ts, showtp, rawimg, tim->steam_width, tim->steam_height);
	}
	if (rawimg != NULL)
		delete[] rawimg;
	return res;
}

uint32_t* FieldTilesDataStruct::ConvertAsImageAccurate(unsigned int cameraid, bool tileflag[], bool showtp) {
	if (IsBGXFormat())
		return ConvertAsImage(cameraid, tileflag, showtp);
	unsigned int imgsize = camera[cameraid].width * camera[cameraid].height;
	int i; unsigned int j;
	TIMImageDataStruct* tim = parent->tim_data[id];
	uint32_t* rawimg = NULL;
	uint32_t* res = new uint32_t[imgsize];
	if (GetGameType() == GAME_TYPE_PSX)
		tim->LoadInVRam();
	else
		rawimg = tim->ConvertAsSteamImage();
	for (i = 0; i < (int)imgsize; i++)
		res[i] = 0;
	for (i = tile_per_depth.size() - 1; i >= 0; i--) {
		for (j = 0; j < tile_per_depth[i].size(); j++) {
			FieldTilesTilesetDataStruct& ts = tiles[tile_per_depth[i][j].first];
			if (ts.camera_id == cameraid && (tileflag == NULL || tileflag[ts.id]) && (tileflag != NULL || ts.is_static || ts.is_first_of_anim))
				AddTileToImage(res, ts, tile_per_depth[i][j].second, showtp, rawimg, tim->steam_width, tim->steam_height);
		}
	}
	if (GetGameType() != GAME_TYPE_PSX)
		delete[] rawimg;
	return res;
}

int FieldTilesDataStruct::Export(const char* outputfile, unsigned int cameraid, bool tileflag[], bool showtp, bool mergetiles, bool depthorder, int steamtitlelang) {
	if (IsBGXFormat())
		return -1;
	fstream ftiff(outputfile, ios::out | ios::binary);
	if (!ftiff.is_open())
		return 1;
	TIMImageDataStruct* tim = parent->tim_data[id];
	unsigned int j, x, y, width, height;
	width = camera[cameraid].width;
	height = camera[cameraid].height;
	uint32_t* img = new uint32_t[width * height];
	uint32_t* rawimg = NULL;
	if (GetGameType() == GAME_TYPE_PSX)
		tim->LoadInVRam();
	else
		rawimg = tim->ConvertAsSteamImage();

	uint16_t tmp16;
	uint32_t tmp32;
	#define MACRO_WS(VAL) \
		tmp16 = VAL; \
		ftiff.write((const char*)&tmp16, 2);
	#define MACRO_WL(VAL) \
		tmp32 = VAL; \
		ftiff.write((const char*)&tmp32, 4);
	#define MACRO_WC(VAL) \
		tmp32 = VAL; \
		ftiff.write((const char*)&tmp32 + 2, 1); \
		ftiff.write((const char*)&tmp32 + 1, 1); \
		ftiff.write((const char*)&tmp32, 1); \
		ftiff.write((const char*)&tmp32 + 3, 1);

	// Global header
	uint32_t ifdoff = 0x10;
	MACRO_WS(0x4949) // Little-endian encoding
	MACRO_WS(42) // magic number
	MACRO_WL(ifdoff) // IFD offset
	// Rational 1/1
	MACRO_WL(1)	MACRO_WL(1)
	// IFD
	#define IFD_INFO_AMOUNT 13
	if (mergetiles) {
		int i;
		MACRO_WS(IFD_INFO_AMOUNT)
		MACRO_WS(0x100)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width) // Width
		MACRO_WS(0x101)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(height) // Length
		MACRO_WS(0x102)	MACRO_WS(1)	MACRO_WL(4)	MACRO_WL(0x08080808) // Bits per sample
		MACRO_WS(0x103)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Compression
		MACRO_WS(0x106)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(2) // Photometric interpretation
		MACRO_WS(0x111)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(ifdoff + 0x6 + IFD_INFO_AMOUNT * 0x0C) // Strip Offsets
		MACRO_WS(0x115)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(4) // Sample per pixel
		MACRO_WS(0x116)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(0xFFFFFFFF) // Rows per strip
		MACRO_WS(0x117)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width * height * 4) // Strip byte counts
		MACRO_WS(0x11A)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution X
		MACRO_WS(0x11B)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution Y
		MACRO_WS(0x128)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Resolution Unit
		MACRO_WS(0x152)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Extra sample (4th byte is alpha)
		MACRO_WL(0) // End of IFDs
		for (j = 0; j < width * height; j++)
			img[j] = 0;
		if (steamtitlelang < 0) {
			for (i = tile_per_depth.size() - 1; i >= 0; i--) {
				for (j = 0; j < tile_per_depth[i].size(); j++) {
					FieldTilesTilesetDataStruct& ts = tiles[tile_per_depth[i][j].first];
					if (ts.camera_id == cameraid && (tileflag == NULL || tileflag[ts.id]))
						AddTileToImage(img, ts, tile_per_depth[i][j].second, true, rawimg, tim->steam_width, tim->steam_height);
				}
			}
			for (i = title_tile_amount; i < (int)title_tile_amount * STEAM_LANGUAGE_AMOUNT; i++) {
				FieldTilesTilesetDataStruct& ts = tiles[tiles_amount + i];
				if (cameraid == ts.camera_id && (tileflag == NULL || tileflag[ts.id]))
					AddTilesetToImage(img, ts, true, rawimg, tim->steam_width, tim->steam_height);
			}
		} else {
			for (i = tile_per_depth.size() - 1; i >= 0; i--) {
				for (j = 0; j < tile_per_depth[i].size(); j++) {
					FieldTilesTilesetDataStruct& ts = tiles[tile_per_depth[i][j].first];
					if (ts.camera_id == cameraid && (tileflag == NULL || tileflag[ts.id]))
						AddTileToImage(img, ts, tile_per_depth[i][j].second, true, rawimg, tim->steam_width, tim->steam_height);
				}
			}
		}
		for (y = 0; y < height; y++)
			for (x = 0; x < width; x++) {
				MACRO_WC(img[y * width + x])
			}
	} else {
		unsigned int i;
		uint32_t lastifdp = 0x4;
		for (i = 0; i < tiles_amount + title_tile_amount * STEAM_LANGUAGE_AMOUNT; i++) {
			FieldTilesTilesetDataStruct* tptr;
			if (i < tiles_amount && steamtitlelang < 0)
				tptr = depthorder ? tiles_sorted[i] : &tiles[i];
			else if (i < tiles_amount)
				tptr = &tiles[GetRelatedTitleTileById(depthorder ? tiles_sorted[i]->id : i, steamtitlelang)];
			else if (i >= tiles_amount + title_tile_amount && steamtitlelang < 0)
				tptr = &tiles[i];
			else
				continue;
			FieldTilesTilesetDataStruct& ts = *tptr;
			if (cameraid == ts.camera_id && (tileflag == NULL || tileflag[ts.id])) {
				MACRO_WS(IFD_INFO_AMOUNT)
				MACRO_WS(0x100)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width) // Width
				MACRO_WS(0x101)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(height) // Length
				MACRO_WS(0x102)	MACRO_WS(1)	MACRO_WL(4)	MACRO_WL(0x08080808) // Bits per sample
				MACRO_WS(0x103)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Compression
				MACRO_WS(0x106)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(2) // Photometric interpretation
				MACRO_WS(0x111)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(ifdoff + 0x6 + IFD_INFO_AMOUNT * 0x0C) // Strip Offsets
				MACRO_WS(0x115)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(4) // Sample per pixel
				MACRO_WS(0x116)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(0xFFFFFFFF) // Rows per strip
				MACRO_WS(0x117)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width * height * 4) // Strip byte counts
				MACRO_WS(0x11A)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution X
				MACRO_WS(0x11B)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution Y
				MACRO_WS(0x128)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Resolution Unit
				MACRO_WS(0x152)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Extra sample (4th byte is alpha)
				ifdoff += 0x6 + IFD_INFO_AMOUNT * 0x0C + width * height * 4;
				lastifdp = ftiff.tellg();
				MACRO_WL(ifdoff)
				for (j = 0; j < width * height; j++)
					img[j] = 0;
				AddTilesetToImage(img, ts, true, rawimg, tim->steam_width, tim->steam_height);
				for (y = 0; y < height; y++)
					for (x = 0; x < width; x++) {
						MACRO_WC(img[y * width + x])
					}
			}
		}
		ftiff.seekg(lastifdp);
		MACRO_WL(0)
	}
	delete[] img;
	if (GetGameType() != GAME_TYPE_PSX)
		delete[] rawimg;
	ftiff.close();
	return 0;
}

vector<pair<int, int>> FieldTilesDataStruct::ExportLayerAsPNG(vector<pair<wxString, int>> fileandtileid) {
	TIMImageDataStruct* tim = parent->tim_data[id];
	unsigned int i, j, x, y, tminx, tminy, tmaxx, tmaxy, pixelx, pixely, camwidth, camheight;
	unsigned int tilesize = parent->tile_size;
	int currentcamerabuffer = -1;
	vector<pair<int, int>> sizes;
	uint32_t* fullimg = NULL;
	uint32_t* atlasimg = NULL;
	bool shouldflushtim = false;
	if (GetGameType() == GAME_TYPE_PSX) {
		tim->LoadInVRam();
	} else {
		shouldflushtim = !tim->loaded;
		fstream f;
		tim->Read(f);
		atlasimg = tim->ConvertAsSteamImage();
	}
	for (i = 0; i < fileandtileid.size(); i++) {
		FieldTilesTilesetDataStruct& ts = tiles[fileandtileid[i].second];
		if (ts.tile_amount == 0) {
			sizes.push_back({ 0, 0 });
			continue;
		}
		if (currentcamerabuffer != ts.camera_id) {
			currentcamerabuffer = ts.camera_id;
			camwidth = camera[currentcamerabuffer].width;
			camheight = camera[currentcamerabuffer].height;
			if (fullimg != NULL)
				delete[] fullimg;
			fullimg = new uint32_t[camwidth * camheight];
		}
		tminx = camwidth;
		tminy = camheight;
		tmaxx = 0;
		tmaxy = 0;
		for (j = 0; j < ts.tile_amount; j++) {
			if (ts.is_memoria_bgx != 0) {
				pixelx = ts.pos_x - camera[currentcamerabuffer].pos_x;
				pixely = ts.pos_y - camera[currentcamerabuffer].pos_y;
			} else {
				pixelx = ts.pos_x + ts.tile[j].pos_x - camera[currentcamerabuffer].pos_x;
				pixely = ts.pos_y + ts.tile[j].pos_y - camera[currentcamerabuffer].pos_y;
			}
			if (GetGameType() != GAME_TYPE_PSX) {
				pixelx = (pixelx * tilesize) / FIELD_TILE_BASE_SIZE;
				pixely = (pixely * tilesize) / FIELD_TILE_BASE_SIZE;
			}
			tminx = min(tminx, pixelx);
			tminy = min(tminy, pixely);
			tmaxx = max(tmaxx, pixelx + tilesize);
			tmaxy = max(tmaxy, pixely + tilesize);
		}
		for (x = tminx; x < tmaxx; x++)
			for (y = tminy; y < tmaxy; y++)
				fullimg[x + y * camwidth] = 0;
		AddTilesetToImage(fullimg, ts, true, atlasimg, tim->steam_width, tim->steam_height);
		int tswidth = tmaxx - tminx;
		int tsheight = tmaxy - tminy;
		unsigned char* rgbbuffer = new unsigned char[3 * tswidth * tsheight];
		unsigned char* alphabuffer = new unsigned char[tswidth * tsheight];
		for (x = tminx; x < tmaxx; x++) {
			for (y = tminy; y < tmaxy; y++) {
				rgbbuffer[3 * (x - tminx + (y - tminy) * tswidth)] = (fullimg[x + y * camwidth] >> 16) & 0xFF;
				rgbbuffer[3 * (x - tminx + (y - tminy) * tswidth) + 1] = (fullimg[x + y * camwidth] >> 8) & 0xFF;
				rgbbuffer[3 * (x - tminx + (y - tminy) * tswidth) + 2] = fullimg[x + y * camwidth] & 0xFF;
				alphabuffer[x - tminx + (y - tminy) * tswidth] = (fullimg[x + y * camwidth] >> 24) & 0xFF;
			}
		}
		wxImage img(tswidth, tsheight, rgbbuffer, alphabuffer, true);
		img.SaveFile(fileandtileid[i].first);
		delete[] rgbbuffer;
		delete[] alphabuffer;
		sizes.push_back({ tswidth, tsheight });
	}
	if (fullimg != NULL)
		delete[] fullimg;
	if (atlasimg != NULL)
		delete[] atlasimg;
	if (shouldflushtim)
		tim->Flush();
	return sizes;
}

void FieldTilesTilesetDataStruct::SetupDataInfos(bool readway) {
	unsigned int i;
	if (readway) {
		is_screen_static = data1 & 0x1;
		use_attaching = (data1 >> 1) & 0x1;
		is_looping = (data1 >> 2) & 0x1;
		has_parallax = (data1 >> 3) & 0x1;
		is_static = (data1 >> 4) & 0x1;
		is_scroll_with_offset = (data1 >> 7) & 0x1;
		depth = (data1 >> 8) & 0xFFF;
		default_depth = (data1 >> 20) & 0xFFF;
		is_x_offset = data2 & 0x1;
		viewport_id = data2 >> 1;
		for (i = 0; i < tile_amount; i++) {
			tile[i].clut_y = tile[i].data_data1 & 0x1FF;
			tile[i].clut_x = (tile[i].data_data1 >> 9) & 0x3F;
			tile[i].page_y = (tile[i].data_data1 >> 15) & 0x1;
			tile[i].page_x = (tile[i].data_data1 >> 16) & 0xF;
			tile[i].res = (tile[i].data_data1 >> 20) & 0x3;
			tile[i].alpha = (tile[i].data_data1 >> 22) & 0x3;
			tile[i].source_v = (tile[i].data_data1 >> 24) & 0xFF;
			tile[i].source_u = tile[i].data_data2 & 0xFF;
			tile[i].h = (tile[i].data_data2 >> 8) & 0x3FF;
			tile[i].w = (tile[i].data_data2 >> 18) & 0x3FF;
			tile[i].trans = (tile[i].data_data2 >> 28) & 0x1;
			tile[i].depth = tile[i].data_data3 & 0xFFF;
			tile[i].pos_y = (tile[i].data_data3 >> 12) & 0x3FF;
			tile[i].pos_x = (tile[i].data_data3 >> 22) & 0x3FF;
			// Unused 3 bits
		}
	} else {
		data1 = (is_screen_static ? 0x1 : 0)
			| (use_attaching ? 0x2 : 0)
			| (is_looping ? 0x4 : 0)
			| (has_parallax ? 0x8 : 0)
			| (is_static ? 0x10 : 0)
			| (is_scroll_with_offset ? 0x80 : 0)
			| ((depth & 0xFFF) << 8)
			| ((default_depth & 0xFFF) << 20);
		data2 = (is_x_offset ? 0x1 : 0)
			| (viewport_id << 1);
		for (i = 0; i < tile_amount; i++) {
			tile[i].data_data1 = (tile[i].clut_y & 0x1FF)
				| ((tile[i].clut_x & 0x3F) << 9)
				| ((tile[i].page_y & 0x1) << 15)
				| ((tile[i].page_x & 0xF) << 16)
				| ((tile[i].res & 0x3) << 20)
				| ((tile[i].alpha & 0x3) << 22)
				| ((tile[i].source_v & 0xFF) << 24);
			tile[i].data_data2 = (tile[i].source_u & 0xFF)
				| ((tile[i].h & 0x3FF) << 8)
				| ((tile[i].w & 0x3FF) << 18)
				| ((tile[i].trans ? 0x1 : 0) << 28);
			tile[i].data_data3 = (tile[i].depth & 0xFFF)
				| ((tile[i].pos_y & 0x3FF) << 12)
				| ((tile[i].pos_x & 0x3FF) << 22);
		}
	}
}

void FieldTilesDataStruct::SetupDataInfos(bool readway) {
	unsigned int i, j;
	if (readway) {
		for (i = 0; i < tiles_amount; i++)
			tiles[i].SetupDataInfos(readway);
		FieldTilesTilesetDataStruct* tmp;
		for (i = 0; i < tiles_amount; i++)
			tiles_sorted[i] = &tiles[i];
		for (i = 0; i < tiles_amount; i++)
			for (j = i + 1; j < tiles_amount; j++)
				if (tiles_sorted[j]->depth >= tiles_sorted[i]->depth) {
					tmp = tiles_sorted[j];
					tiles_sorted[j] = tiles_sorted[i];
					tiles_sorted[i] = tmp;
				}
		for (i = 0; i < anim_amount; i++)
			if (anim[i].tile_amount > 0)
				tiles[anim[i].tile_list[0]].is_first_of_anim = true;
		SetupTilePerDepth();
	} else {
		anim_offset = 0x34;
		tiles_offset = anim_offset + anim_amount * 0x10;
		light_offset = tiles_offset + tiles_amount * 0x39;
		for (i = 0; i < tiles_amount; i++) {
			if (tiles[i].is_memoria_bgx != 0) {
				light_offset += 4 + tiles[i].memoria_bgx_path.mb_str(wxConvUTF8).length();
				light_offset += 4 + tiles[i].memoria_bgx_shader.mb_str(wxConvUTF8).length();
			}
		}
		camera_offset = light_offset + light_amount * 0xC;
		uint32_t offset = camera_offset + camera_amount * 0x34;
		for (i = 0; i < anim_amount; i++) {
			anim[i].tile_list_offset = offset;
			offset += anim[i].tile_amount * 2;
		}
		for (i = 0; i < tiles_amount; i++) {
			tiles[i].tile_pos_offset = offset;
			offset += tiles[i].tile_amount * 4;
		}
		for (i = 0; i < tiles_amount; i++) {
			tiles[i].tile_data_offset = offset;
			offset += tiles[i].tile_amount * 8;
		}
		for (i = 0; i < tiles_amount; i++)
			tiles[i].SetupDataInfos(readway);
	}
}

void FieldTilesDataStruct::SetupTilePerDepth() {
	map<int, vector<pair<int, int>>> depthmap;
	unsigned int i, j;
	tile_per_depth.clear();
	for (i = 0; i < tiles_amount; i++)
		for (j = 0; j < tiles[i].tile_amount; j++)
			depthmap[tiles[i].depth + tiles[i].tile[j].depth].push_back({ i, j });
	for (auto it = depthmap.begin(); it != depthmap.end(); it++)
		tile_per_depth.push_back(it->second);
}

bool FieldTilesDataStruct::IsBGXFormat() const {
	return tiles_amount > 0 && tiles[0].is_memoria_bgx != 0;
}

int FieldTilesDataStruct::GetFieldId() const {
	return parent->GetIdByIndex(id);
}

void FieldTilesDataStruct::ConvertToBGX(wxString imgfolder) {
	if (IsBGXFormat())
		return;
	if (!imgfolder.EndsWith('\\') && !imgfolder.EndsWith('/'))
		imgfolder += _(L"\\");
	MainFrame::MakeDirForFile(imgfolder.ToStdString());
	vector<pair<wxString, int>> overlays;
	unsigned int i;
	for (i = 0; i < tiles_amount; i++) {
		wxString filename = "Background_" + to_string(i) + ".png";
		overlays.push_back({ imgfolder + filename, i });
	}
	vector<pair<int, int>> sizes = ExportLayerAsPNG(overlays);
	for (i = 0; i < tiles_amount; i++) {
		wxString filename = overlays[i].first.AfterLast(L'\\');
		FieldTilesTilesetDataStruct& ts = tiles[i];
		ts.is_memoria_bgx = 1;
		ts.memoria_bgx_path = overlays[i].first;
		if (ts.tile_amount > 0 && ts.tile[0].trans) {
			switch (ts.tile[0].alpha) {
			case 0:
				ts.memoria_bgx_shader = _(L"PSX/FieldMap_Abr_0");
				break;
			case 1:
				ts.memoria_bgx_shader = _(L"PSX/FieldMap_Abr_1");
				break;
			case 2:
				ts.memoria_bgx_shader = _(L"PSX/FieldMap_Abr_2");
				break;
			case 3:
				ts.memoria_bgx_shader = _(L"PSX/FieldMap_Abr_3");
				break;
			}
		} else {
			ts.memoria_bgx_shader = _(L"PSX/FieldMap_Abr_None");
		}
		ts.width = sizes[i].first * FIELD_TILE_BASE_SIZE / parent->tile_size;
		ts.height = sizes[i].second * FIELD_TILE_BASE_SIZE / parent->tile_size;
	}
}

#define MACRO_TILES_IOFUNCTION(IO, SEEK, READ, PPF) \
	unsigned int i, j, k; \
	if (!READ) SetupDataInfos(false); \
	uint32_t headerpos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f, tiles_size); \
	IO ## Short(f, depth_shift); \
	IO ## Short(f, anim_amount); \
	IO ## Short(f, tiles_amount); \
	IO ## Short(f, light_amount); \
	IO ## Short(f, camera_amount); \
	IO ## Long(f, anim_offset); \
	IO ## Long(f, tiles_offset); \
	IO ## Long(f, light_offset); \
	IO ## Long(f, camera_offset); \
	IO ## Short(f, (uint16_t&)default_depth); \
	IO ## Short(f, (uint16_t&)depth); \
	IO ## Short(f, (uint16_t&)default_x); \
	IO ## Short(f, (uint16_t&)default_y); \
	IO ## Short(f, (uint16_t&)pos_x); \
	IO ## Short(f, (uint16_t&)pos_y); \
	IO ## Short(f, (uint16_t&)min_x); \
	IO ## Short(f, (uint16_t&)max_x); \
	IO ## Short(f, (uint16_t&)min_y); \
	IO ## Short(f, (uint16_t&)max_y); \
	IO ## Short(f, (uint16_t&)screen_x); \
	IO ## Short(f, (uint16_t&)screen_y); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		anim = new FieldTilesAnimDataStruct[anim_amount]; \
		tiles = new FieldTilesTilesetDataStruct[tiles_amount + title_tile_amount * STEAM_LANGUAGE_AMOUNT]; \
		tiles_sorted = new FieldTilesTilesetDataStruct*[tiles_amount]; \
		light = new FieldTilesLightDataStruct[light_amount]; \
		camera = new FieldTilesCameraDataStruct[camera_amount]; \
	} \
	for (i = 0; i < anim_amount; i++) { \
		SEEK(f, headerpos, anim_offset + i * 0x10); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Char(f, anim[i].flag); \
		IO ## Long3(f, anim[i].tile_amount); \
		IO ## Char(f, anim[i].camera_id); \
		IO ## Long3(f, anim[i].default_frame); \
		IO ## Short(f, (uint16_t&)anim[i].rate); \
		IO ## Short(f, anim[i].counter); \
		IO ## Long(f, anim[i].tile_list_offset); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			anim[i].tile_list = new uint8_t[anim[i].tile_amount]; \
			anim[i].tile_duration = new uint8_t[anim[i].tile_amount]; \
		} \
		SEEK(f, headerpos, anim[i].tile_list_offset); \
		if (PPF) PPFInitScanStep(f); \
		for (j = 0; j < anim[i].tile_amount; j++) { \
			IO ## Char(f, anim[i].tile_list[j]); \
			IO ## Char(f, anim[i].tile_duration[j]); \
		} \
		if (PPF) PPFEndScanStep(); \
	} \
	k = 0; \
	SEEK(f, headerpos, tiles_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i = 0; i < tiles_amount; i++) { \
		IO ## Long(f, tiles[i].data1); \
		IO ## Short(f, tiles[i].width); \
		IO ## Short(f, tiles[i].height); \
		IO ## Short(f, (uint16_t&)tiles[i].default_x); \
		IO ## Short(f, (uint16_t&)tiles[i].default_y); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_x); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_y); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_minx); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_maxx); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_miny); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_maxy); \
		IO ## Short(f, (uint16_t&)tiles[i].screen_x); \
		IO ## Short(f, (uint16_t&)tiles[i].screen_y); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_dx); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_dy); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_fracx); \
		IO ## Short(f, (uint16_t&)tiles[i].pos_fracy); \
		IO ## Char(f, tiles[i].camera_id); \
		IO ## Char(f, tiles[i].data2); \
		IO ## Short(f, tiles[i].tile_amount); \
		IO ## Long(f, tiles[i].tile_pos_offset); \
		IO ## Long(f, tiles[i].tile_data_offset); \
		IO ## Long(f, tiles[i].tile_packet); \
		IO ## Long(f, tiles[i].tile_tpage); \
		if (READ) { \
			tiles[i].tile = new FieldTilesTileDataStruct[tiles[i].tile_amount]; \
			tiles[i].parent = this; \
			tiles[i].id = i; \
			tiles[i].is_first_of_anim = false; \
			for (j = 0; j < tiles[i].tile_amount; j++) \
				tiles[i].tile[j].steam_id = k++; \
			if (!bgxallowed) { \
				tiles[i].is_memoria_bgx = 0; \
				tiles[i].memoria_bgx_path = _(L""); \
				tiles[i].memoria_bgx_shader = _(L"PSX/FieldMap_Abr_None"); \
			} \
		} \
		if (bgxallowed) \
			IO ## Char(f, tiles[i].is_memoria_bgx); \
		if (tiles[i].is_memoria_bgx != 0) { \
			if (READ) { \
				HWSReadWxString(f, tiles[i].memoria_bgx_path); \
				HWSReadWxString(f, tiles[i].memoria_bgx_shader); \
			} else { \
				HWSWriteWxString(f, tiles[i].memoria_bgx_path); \
				HWSWriteWxString(f, tiles[i].memoria_bgx_shader); \
			} \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i = 0; i < tiles_amount; i++) { \
		if (tiles[i].is_memoria_bgx == 0) { \
			for (j = 0; j < tiles[i].tile_amount; j++) { \
				SEEK(f, headerpos, tiles[i].tile_data_offset + j * 0x8); \
				if (PPF) PPFInitScanStep(f); \
				IO ## Long(f, tiles[i].tile[j].data_data1); \
				IO ## Long(f, tiles[i].tile[j].data_data2); \
				if (PPF) PPFEndScanStep(); \
				SEEK(f, headerpos, tiles[i].tile_pos_offset + j * 0x4); \
				if (PPF) PPFInitScanStep(f); \
				IO ## Long(f, tiles[i].tile[j].data_data3); \
				if (PPF) PPFEndScanStep(); \
			} \
		} \
	} \
	SEEK(f, headerpos, light_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i = 0; i < light_amount; i++) { \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(f, headerpos, camera_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i = 0; i < camera_amount; i++) { \
		IO ## Short(f, camera[i].distance); \
		for (j = 0; j < 9; j++) \
			IO ## Short(f, (uint16_t&)camera[i].matrix[j]); \
		IO ## Long(f, (uint32_t&)camera[i].offset_x); \
		IO ## Long(f, (uint32_t&)camera[i].offset_z); \
		IO ## Long(f, (uint32_t&)camera[i].offset_y); \
		IO ## Short(f, (uint16_t&)camera[i].offset_centerx); \
		IO ## Short(f, (uint16_t&)camera[i].offset_centery); \
		IO ## Short(f, (uint16_t&)camera[i].offset_width); \
		IO ## Short(f, (uint16_t&)camera[i].offset_height); \
		IO ## Short(f, (uint16_t&)camera[i].min_x); \
		IO ## Short(f, (uint16_t&)camera[i].max_x); \
		IO ## Short(f, (uint16_t&)camera[i].min_y); \
		IO ## Short(f, (uint16_t&)camera[i].max_y); \
		IO ## Long(f, (uint32_t&)camera[i].depth); \
		if (READ) { \
			camera[i].parent = this; \
			camera[i].id = i; \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		SetupDataInfos(true); \
		for (i = 0; i < camera_amount; i++) \
			camera[i].UpdateSize(); \
	}


void FieldTilesDataStruct::Read(fstream& f, unsigned int titletileamount) {
	if (loaded)
		return;
	bool bgxallowed = false;
	title_tile_amount = titletileamount;
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_TILES_IOFUNCTION(FFIXRead, FFIXSeek, true, false)
	} else {
		MACRO_TILES_IOFUNCTION(SteamRead, SteamSeek, true, false)
	}
	loaded = true;
}

void FieldTilesDataStruct::Write(fstream& f) {
	bool bgxallowed = false;
	MACRO_TILES_IOFUNCTION(FFIXWrite, FFIXSeek, false, false)
	modified = false;
}

void FieldTilesDataStruct::WritePPF(fstream& f) {
	bool bgxallowed = false;
	MACRO_TILES_IOFUNCTION(PPFStepAdd, FFIXSeek, false, true)
}

void FieldTilesDataStruct::ReadHWS(fstream& f) {
	bool bgxallowed = GetHWSGlobalVersion() >= 102;
	MACRO_TILES_IOFUNCTION(HWSRead, HWSSeek, true, false)
	MarkDataModified();
}

void FieldTilesDataStruct::WriteHWS(fstream& f) {
	bool bgxallowed = true;
	MACRO_TILES_IOFUNCTION(HWSWrite, HWSSeek, false, false)
}

int FieldTilesDataStruct::WriteBGX(string destfolder, string bgxfilename) {
	vector<pair<wxString, int>> overlays;
	vector<pair<int, int>> sizes;
	SteamLanguage lang;
	unsigned int i, j;
	if (destfolder.length() > 0 && destfolder[destfolder.length() - 1] != '\\' && destfolder[destfolder.length() - 1] != '/')
		destfolder += "\\";
	MainFrame::MakeDirForFile(destfolder);
	if (IsBGXFormat()) {
		for (i = 0; i < tiles_amount; i++) {
			FieldTilesTilesetDataStruct& ts = tiles[i];
			wxFileName fnformat(ts.memoria_bgx_path);
			wxString filename = _(destfolder) + fnformat.GetFullName();
			wxCopyFile(ts.memoria_bgx_path, filename);
			overlays.push_back({ filename, i });
			sizes.push_back({ ts.width * parent->tile_size / FIELD_TILE_BASE_SIZE, ts.height * parent->tile_size / FIELD_TILE_BASE_SIZE });
		}
	} else {
		for (i = 0; i < tiles_amount; i++) {
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				wxString filename = "Background_" + to_string(i);
				int titleid = GetRelatedTitleTileById(i, lang);
				if (titleid >= tiles_amount)
					filename += "_" + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[lang];
				filename += ".png";
				overlays.push_back({ _(destfolder) + filename, titleid });
				if (titleid < tiles_amount)
					break;
			}
		}
		sizes = ExportLayerAsPNG(overlays);
	}
	bool uselang = false;
	fstream f((destfolder + bgxfilename).c_str(), ios::out);
	if (!f.is_open())
		return 3;
	f << "TILESIZE: " << (int)parent->tile_size << "\n\n";
	for (i = 0; i < overlays.size(); i++) {
		wxString filename = overlays[i].first.AfterLast(L'\\');
		int titleid = overlays[i].second;
		if (titleid >= tiles_amount) {
			f << "LANGUAGE: " << filename.Mid(filename.Len() - 6, 2).Upper() << "\n";
			uselang = true;
		} else if (uselang) {
			f << "LANGUAGE: Any\n\n";
			uselang = false;
		}
		FieldTilesTilesetDataStruct& ts = tiles[titleid];
		f << "OVERLAY\n";
		f << "CameraId: " << (int)ts.camera_id << "\n";
		f << "ViewportId: " << (int)ts.viewport_id << "\n";
		f << "Position: " << (int)ts.pos_x << ", " << (int)ts.pos_y << ", " << (int)ts.depth << "\n";
		f << "Size: " << (int)(sizes[i].first * FIELD_TILE_BASE_SIZE / parent->tile_size) << ", " << (int)(sizes[i].second * FIELD_TILE_BASE_SIZE / parent->tile_size) << "\n";
		if (ts.is_scroll_with_offset) f << "ScrollWithOffset: " << (int)ts.pos_dx << ", " << (int)ts.pos_dy << "\n";
		if (sizes[i].first > 0 && sizes[i].second > 0) {
			f << "Image: " << filename.ToStdString() << "\n";
			if (ts.is_memoria_bgx != 0) {
				f << ts.memoria_bgx_shader.c_str() << "\n";
			} else {
				if (ts.tile_amount > 0 && ts.tile[0].trans) {
					switch (ts.tile[0].alpha) {
					case 0:
						f << "Shader: PSX/FieldMap_Abr_0\n";
						break;
					case 1:
						f << "Shader: PSX/FieldMap_Abr_1\n";
						break;
					case 2:
						f << "Shader: PSX/FieldMap_Abr_2\n";
						break;
					case 3:
						f << "Shader: PSX/FieldMap_Abr_3\n";
						break;
					default:
						f << "Shader: Unknown\n";
						break;
					}
				} else {
					f << "Shader: PSX/FieldMap_Abr_None\n";
				}
			}
		}
		f << "\n";
	}
	if (uselang)
		f << "LANGUAGE: Any\n\n";
	for (i = 0; i < anim_amount; i++) {
		FieldTilesAnimDataStruct& a = anim[i];
		f << "ANIMATION\n";
		f << "CameraId: " << (int)a.camera_id << "\n";
		f << "FrameRate: " << (int)a.rate << "\n";
		if (a.flag & 16)	f << "Loop\n";
		if (a.flag & 32)	f << "Palindrome\n";
		if (a.tile_amount > 0) {
			f << "Overlays: " << (int)a.tile_list[0];
			for (j = 1; j < a.tile_amount; j++) f << ", " << (int)a.tile_list[j];
			f << "\n";
		}
		f << "\n";
	}
	for (i = 0; i < camera_amount; i++) {
		FieldTilesCameraDataStruct& c = camera[i];
		f << "CAMERA\n";
		f << "ViewDistance: " << (int)c.distance << "\n";
		f << "CenterOffset: " << (int)c.offset_centerx << ", " << (int)c.offset_centery << "\n";
		f << "Position: " << (int)c.offset_x << ", " << (int)c.offset_z << ", " << (int)c.offset_y << "\n";
		f << "Range: " << (int)c.offset_width << ", " << (int)c.offset_height << "\n";
		f << "DepthOffset: " << (int)c.depth << "\n";
		f << "Viewport: " << (int)c.min_x << ", " << (int)c.max_x << ", " << (int)c.min_y << ", " << (int)c.max_y << "\n";
		f << "OrientationMatrix: " << (float)(c.matrix[0] / 4096.0f);
		for (j = 1; j < 9; j++) f << ", " << (float)(c.matrix[j] / 4096.0f);
		f << "\n\n";
	}
	f.close();
	return 0;
}

FieldTilesDataStruct::~FieldTilesDataStruct() {
	return; // TODO (triggers bugs, surely because of struct copying/assigning)
	if (loaded) {
		unsigned int i;
		for (i = 0; i < anim_amount; i++) {
			delete[] anim[i].tile_list;
			delete[] anim[i].tile_duration;
		}
		delete[] anim;
		for (i = 0; i < tiles_amount; i++)
			delete[] tiles[i].tile;
		delete[] tiles;
		delete[] light;
		delete[] camera;
		delete[] tiles_sorted;
	}
}

#define MACRO_WALKMESH_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j,k; \
	uint32_t headerpos; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,magic_walkmesh); \
	headerpos = f.tellg(); \
	IO ## Short(f,(uint16_t&)datasize); \
	IO ## Short(f,(uint16_t&)offset_orgx); \
	IO ## Short(f,(uint16_t&)offset_orgz); \
	IO ## Short(f,(uint16_t&)offset_orgy); \
	IO ## Short(f,(uint16_t&)offset_x); \
	IO ## Short(f,(uint16_t&)offset_z); \
	IO ## Short(f,(uint16_t&)offset_y); \
	IO ## Short(f,(uint16_t&)offset_minx); \
	IO ## Short(f,(uint16_t&)offset_minz); \
	IO ## Short(f,(uint16_t&)offset_miny); \
	IO ## Short(f,(uint16_t&)offset_maxx); \
	IO ## Short(f,(uint16_t&)offset_maxz); \
	IO ## Short(f,(uint16_t&)offset_maxy); \
	IO ## Short(f,(uint16_t&)offset_charx); \
	IO ## Short(f,(uint16_t&)offset_charz); \
	IO ## Short(f,(uint16_t&)offset_chary); \
	IO ## Short(f,(uint16_t&)active_walkpath); \
	IO ## Short(f,(uint16_t&)active_triangle); \
	IO ## Short(f,triangle_amount); \
	IO ## Short(f,triangle_offset); \
	IO ## Short(f,edge_amount); \
	IO ## Short(f,edge_offset); \
	IO ## Short(f,animation_amount); \
	IO ## Short(f,animation_offset); \
	IO ## Short(f,walkpath_amount); \
	IO ## Short(f,walkpath_offset); \
	IO ## Short(f,normal_amount); \
	IO ## Short(f,normal_offset); \
	IO ## Short(f,vertex_amount); \
	IO ## Short(f,vertex_offset); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		triangle_flag.resize(triangle_amount); \
		triangle_data.resize(triangle_amount); \
		triangle_walkpath.resize(triangle_amount); \
		triangle_normal.resize(triangle_amount); \
		triangle_thetax.resize(triangle_amount); \
		triangle_thetay.resize(triangle_amount); \
		triangle_vertex.resize(triangle_amount); \
		triangle_edge.resize(triangle_amount); \
		triangle_adjacenttriangle.resize(triangle_amount); \
		triangle_centerx.resize(triangle_amount); \
		triangle_centerz.resize(triangle_amount); \
		triangle_centery.resize(triangle_amount); \
		triangle_d.resize(triangle_amount); \
		edge_flag.resize(edge_amount); \
		edge_clone.resize(edge_amount); \
		animation_flag.resize(animation_amount); \
		animation_frameamount.resize(animation_amount); \
		animation_framerate.resize(animation_amount); \
		animation_counter.resize(animation_amount); \
		animation_currentframe.resize(animation_amount); \
		animation_frameoffset.resize(animation_amount); \
		animation_frameflag.resize(animation_amount); \
		animation_framevalue.resize(animation_amount); \
		animation_frametriangleamount.resize(animation_amount); \
		animation_frametriangleoffset.resize(animation_amount); \
		animation_frametriangle.resize(animation_amount); \
		walkpath_flag.resize(walkpath_amount); \
		walkpath_id.resize(walkpath_amount); \
		walkpath_orgx.resize(walkpath_amount); \
		walkpath_orgz.resize(walkpath_amount); \
		walkpath_orgy.resize(walkpath_amount); \
		walkpath_offsetx.resize(walkpath_amount); \
		walkpath_offsetz.resize(walkpath_amount); \
		walkpath_offsety.resize(walkpath_amount); \
		walkpath_minx.resize(walkpath_amount); \
		walkpath_minz.resize(walkpath_amount); \
		walkpath_miny.resize(walkpath_amount); \
		walkpath_maxx.resize(walkpath_amount); \
		walkpath_maxz.resize(walkpath_amount); \
		walkpath_maxy.resize(walkpath_amount); \
		walkpath_triangleamount.resize(walkpath_amount); \
		walkpath_trianglelistoffset.resize(walkpath_amount); \
		walkpath_trianglelist.resize(walkpath_amount); \
		normal_x.resize(normal_amount); \
		normal_z.resize(normal_amount); \
		normal_y.resize(normal_amount); \
		normal_overz.resize(normal_amount); \
		vertex_x.resize(vertex_amount); \
		vertex_z.resize(vertex_amount); \
		vertex_y.resize(vertex_amount); \
	} \
	for (i=0;i<triangle_amount;i++) { \
		SEEK(f,headerpos,triangle_offset+i*0x28); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,triangle_flag[i]); \
		IO ## Short(f,triangle_data[i]); \
		IO ## Short(f,triangle_walkpath[i]); \
		IO ## Short(f,(uint16_t&)triangle_normal[i]); \
		IO ## Short(f,triangle_thetax[i]); \
		IO ## Short(f,triangle_thetay[i]); \
		IO ## Short(f,(uint16_t&)triangle_vertex[i].index[0]); \
		IO ## Short(f,(uint16_t&)triangle_vertex[i].index[1]); \
		IO ## Short(f,(uint16_t&)triangle_vertex[i].index[2]); \
		IO ## Short(f,(uint16_t&)triangle_edge[i].index[0]); \
		IO ## Short(f,(uint16_t&)triangle_edge[i].index[1]); \
		IO ## Short(f,(uint16_t&)triangle_edge[i].index[2]); \
		IO ## Short(f,(uint16_t&)triangle_adjacenttriangle[i].index[0]); \
		IO ## Short(f,(uint16_t&)triangle_adjacenttriangle[i].index[1]); \
		IO ## Short(f,(uint16_t&)triangle_adjacenttriangle[i].index[2]); \
		IO ## Short(f,(uint16_t&)triangle_centerx[i]); \
		IO ## Short(f,(uint16_t&)triangle_centerz[i]); \
		IO ## Short(f,(uint16_t&)triangle_centery[i]); \
		IO ## Long(f,(uint32_t&)triangle_d[i]); \
		if (PPF) PPFEndScanStep(); \
	} \
	for (i=0;i<edge_amount;i++) { \
		SEEK(f,headerpos,edge_offset+i*0x4); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,edge_flag[i]); \
		IO ## Short(f,(uint16_t&)edge_clone[i]); \
		if (PPF) PPFEndScanStep(); \
	} \
	for (i=0;i<animation_amount;i++) { \
		SEEK(f,headerpos,animation_offset+i*0x10); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,animation_flag[i]); \
		IO ## Short(f,animation_frameamount[i]); \
		IO ## Short(f,(uint16_t&)animation_framerate[i]); \
		IO ## Short(f,animation_counter[i]); \
		IO ## Long(f,(uint32_t&)animation_currentframe[i]); \
		IO ## Long(f,animation_frameoffset[i]); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			animation_frameflag[i].resize(triangle_amount); \
			animation_framevalue[i].resize(triangle_amount); \
			animation_frametriangleamount[i].resize(triangle_amount); \
			animation_frametriangleoffset[i].resize(triangle_amount); \
			animation_frametriangle[i].resize(triangle_amount); \
		} \
		SEEK(f,headerpos,animation_frameoffset[i]); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<animation_frameamount[i];j++) { \
			IO ## Short(f,animation_frameflag[i][j]); \
			IO ## Short(f,(uint16_t&)animation_framevalue[i][j]); \
			IO ## Short(f,animation_frametriangleamount[i][j]); \
			IO ## Short(f,animation_frametriangleoffset[i][j]); \
		} \
		if (PPF) PPFEndScanStep(); \
		for (j=0;j<animation_frameamount[i];j++) { \
			if (READ) animation_frametriangle[i][j].resize(animation_frametriangleamount[i][j]); \
			SEEK(f,headerpos,animation_frametriangleoffset[i][j]); \
			if (PPF) PPFInitScanStep(f); \
			for (k=0;k<animation_frametriangleamount[i][j];k++) { \
				IO ## Long(f,animation_frametriangle[i][j][k]); \
			} \
			if (PPF) PPFEndScanStep(); \
		} \
	} \
	for (i=0;i<walkpath_amount;i++) { \
		SEEK(f,headerpos,walkpath_offset+i*0x20); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,walkpath_flag[i]); \
		IO ## Short(f,walkpath_id[i]); \
		IO ## Short(f,(uint16_t&)walkpath_orgx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_orgz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_orgy[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsetx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsetz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsety[i]); \
		IO ## Short(f,(uint16_t&)walkpath_minx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_minz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_miny[i]); \
		IO ## Short(f,(uint16_t&)walkpath_maxx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_maxz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_maxy[i]); \
		IO ## Short(f,walkpath_triangleamount[i]); \
		IO ## Short(f,walkpath_trianglelistoffset[i]); \
		if (PPF) PPFEndScanStep(); \
		if (READ) walkpath_trianglelist[i].resize(walkpath_triangleamount[i]); \
		SEEK(f,headerpos,walkpath_trianglelistoffset[i]); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<walkpath_triangleamount[i];j++) { \
			IO ## Long(f,walkpath_trianglelist[i][j]); \
		} \
		if (PPF) PPFEndScanStep(); \
	} \
	for (i=0;i<normal_amount;i++) { \
		SEEK(f,headerpos,normal_offset+i*0x10); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Long(f,(uint32_t&)normal_x[i]); \
		IO ## Long(f,(uint32_t&)normal_z[i]); \
		IO ## Long(f,(uint32_t&)normal_y[i]); \
		IO ## Long(f,(uint32_t&)normal_overz[i]); \
		if (PPF) PPFEndScanStep(); \
	} \
	for (i=0;i<vertex_amount;i++) { \
		SEEK(f,headerpos,vertex_offset+i*0x6); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,(uint16_t&)vertex_x[i]); \
		IO ## Short(f,(uint16_t&)vertex_z[i]); \
		IO ## Short(f,(uint16_t&)vertex_y[i]); \
		if (PPF) PPFEndScanStep(); \
	}


void FieldWalkmeshDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_WALKMESH_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		MACRO_WALKMESH_IOFUNCTION(SteamRead,SteamSeek,true,false)
	}
	loaded = true;
}

void FieldWalkmeshDataStruct::Write(fstream& f) {
	UpdateOffset();
	MACRO_WALKMESH_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void FieldWalkmeshDataStruct::WritePPF(fstream& f) {
	UpdateOffset();
	MACRO_WALKMESH_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void FieldWalkmeshDataStruct::ReadHWS(fstream& f) {
	MACRO_WALKMESH_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
	UpdateOffset();
}

void FieldWalkmeshDataStruct::WriteHWS(fstream& f) {
	UpdateOffset();
	MACRO_WALKMESH_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

void FieldWalkmeshDataStruct::UpdateOffset() {
	unsigned int i, j;
	uint32_t off = 0x3C;
	triangle_offset = off;
	off += 0x28 * triangle_amount;
	edge_offset = off;
	off += 4 * edge_amount;
	animation_offset = off;
	off += 0x10 * animation_amount;
	walkpath_offset = off;
	off += 0x20 * walkpath_amount;
	normal_offset = off;
	off += 0x10 * normal_amount;
	vertex_offset = off;
	off += 6 * vertex_amount;
	if ((vertex_amount & 1) != 0)
		off += 2;
	for (i = 0; i < animation_amount; i++) {
		animation_frameoffset[i] = off;
		off += 8 * animation_frameamount[i];
	}
	for (i = 0; i < walkpath_amount; i++) {
		walkpath_trianglelistoffset[i] = off;
		off += 4 * walkpath_triangleamount[i];
	}
	for (i = 0; i < animation_amount; i++)
		for (j = 0; j < animation_frameamount[i]; j++) {
			animation_frametriangleoffset[i][j] = off;
			off += 4 * animation_frametriangleamount[i][j];
		}
	datasize = off;
	SetSize(datasize + 4);
}

void FieldWalkmeshDataStruct::GetPathCoordinate(int coordtype, int pathid, int16_t& cx, int16_t& cy, int16_t& cz) {
	if (coordtype <= 0) {
		cx = walkpath_orgx[pathid];
		cy = walkpath_orgy[pathid];
		cz = walkpath_orgz[pathid];
	} else if (coordtype == 1) {
		cx = walkpath_offsetx[pathid];
		cy = walkpath_offsety[pathid];
		cz = walkpath_offsetz[pathid];
	} else if (coordtype == 2) {
		cx = walkpath_minx[pathid];
		cy = walkpath_miny[pathid];
		cz = walkpath_minz[pathid];
	} else if (coordtype == 3) {
		cx = walkpath_maxx[pathid];
		cy = walkpath_maxy[pathid];
		cz = walkpath_maxz[pathid];
	}
}

void FieldWalkmeshDataStruct::SetPathCoordinate(int coordtype, int pathid, int coordaxis, int16_t c) {
	if (coordtype < 0) {
		SetPathCoordinate(0, pathid, coordaxis, c);
		SetPathCoordinate(1, pathid, coordaxis, c);
		SetPathCoordinate(2, pathid, coordaxis, c);
		SetPathCoordinate(3, pathid, coordaxis, c);
		return;
	}
	if (coordtype == 0) {
		if (coordaxis == 0)			walkpath_orgx[pathid] = c;
		else if (coordaxis == 1)	walkpath_orgy[pathid] = c;
		else if (coordaxis == 2)	walkpath_orgz[pathid] = c;
	} else if (coordtype == 1) {
		if (coordaxis == 0)			walkpath_offsetx[pathid] = c;
		else if (coordaxis == 1)	walkpath_offsety[pathid] = c;
		else if (coordaxis == 2)	walkpath_offsetz[pathid] = c;
	} else if (coordtype == 2) {
		if (coordaxis == 0)			walkpath_minx[pathid] = c;
		else if (coordaxis == 1)	walkpath_miny[pathid] = c;
		else if (coordaxis == 2)	walkpath_minz[pathid] = c;
	} else if (coordtype == 3) {
		if (coordaxis == 0)			walkpath_maxx[pathid] = c;
		else if (coordaxis == 1)	walkpath_maxy[pathid] = c;
		else if (coordaxis == 2)	walkpath_maxz[pathid] = c;
	}
}

int FieldWalkmeshDataStruct::AddWalkpath(uint32_t& extrasize, int16_t x, int16_t y, int16_t z) {
	if (extrasize < 0x20)
		return 1;
	extrasize -= 0x20;
	walkpath_flag.push_back(0);
	walkpath_id.push_back(walkpath_amount);
	walkpath_orgx.push_back(x);
	walkpath_orgy.push_back(y);
	walkpath_orgz.push_back(z);
	walkpath_offsetx.push_back(x);
	walkpath_offsety.push_back(y);
	walkpath_offsetz.push_back(z);
	walkpath_minx.push_back(x);
	walkpath_miny.push_back(y);
	walkpath_minz.push_back(z);
	walkpath_maxx.push_back(x);
	walkpath_maxy.push_back(y);
	walkpath_maxz.push_back(z);
	walkpath_triangleamount.push_back(0);
	walkpath_trianglelistoffset.push_back(0);
	walkpath_trianglelist.resize(walkpath_trianglelist.size() + 1);
	walkpath_amount++;
	return 0;
}

int FieldWalkmeshDataStruct::AddTriangle2(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, int16_t adjtri1, int16_t adjtri2, int edgeid1, int edgeid2, int commonvert1, int commonvert2) {
	int uncommonvert1 = commonvert1 == edgeid1 ? (edgeid1 + 2) % 3 : edgeid1;
	int uncommonvert2 = commonvert2 == edgeid2 ? (edgeid2 + 2) % 3 : edgeid2;
	int v1 = triangle_vertex[adjtri1].index[uncommonvert1];
	int v2 = triangle_vertex[adjtri2].index[uncommonvert2];
	int v3 = triangle_vertex[adjtri1].index[commonvert1];
	array<int16_t, 3> v1pos = {
		walkpath_orgx[triangle_walkpath[adjtri1]] + vertex_x[v1] - walkpath_orgx[path],
		walkpath_orgy[triangle_walkpath[adjtri1]] + vertex_y[v1] - walkpath_orgy[path],
		walkpath_orgz[triangle_walkpath[adjtri1]] + vertex_z[v1] - walkpath_orgz[path]
	};
	array<int16_t, 3> v2pos = {
		walkpath_orgx[triangle_walkpath[adjtri2]] + vertex_x[v2] - walkpath_orgx[path],
		walkpath_orgy[triangle_walkpath[adjtri2]] + vertex_y[v2] - walkpath_orgy[path],
		walkpath_orgz[triangle_walkpath[adjtri2]] + vertex_z[v2] - walkpath_orgz[path]
	};
	array<int16_t, 3> v3pos = {
		walkpath_orgx[triangle_walkpath[adjtri1]] + vertex_x[v3] - walkpath_orgx[path],
		walkpath_orgy[triangle_walkpath[adjtri1]] + vertex_y[v3] - walkpath_orgy[path],
		walkpath_orgz[triangle_walkpath[adjtri1]] + vertex_z[v3] - walkpath_orgz[path]
	};
	return AddTriangle(extrasize, path, flag, d, thetax, thetay, { v1pos[0], v2pos[0], v3pos[0] }, { v1pos[1], v2pos[1], v3pos[1] }, { v1pos[2], v2pos[2], v3pos[2] });
	//int path1 = triangle_walkpath[adjtri1];
	//int path2 = triangle_walkpath[adjtri2];
	//bool samepath1 = path == path1;
	//bool samepath2 = path == path2;
	//int sizereq = 0x38 + (samepath1 ? 0 : 6) + (samepath2 ? 0 : 6) + (samepath1 && samepath2 ? 0 : 6);
	//if (extrasize < sizereq)
	//	return 1;
	//extrasize -= sizereq;
	//int uncommonvert1 = commonvert1 == edgeid1 ? (edgeid1 + 2) % 3 : edgeid1;
	//int uncommonvert2 = commonvert2 == edgeid2 ? (edgeid2 + 2) % 3 : edgeid2;
	//triangle_adjacenttriangle[adjtri1].index[edgeid1] = triangle_amount;
	//triangle_adjacenttriangle[adjtri2].index[edgeid2] = triangle_amount;
	//triangle_edge[adjtri1].index[edgeid1] = 0;
	//triangle_edge[adjtri2].index[edgeid2] = 1;
	//triangle_adjacenttriangle.push_back(FieldWalkmeshTriple(adjtri1, adjtri2, -1));
	//triangle_edge.push_back(FieldWalkmeshTriple(edge_amount, edge_amount + 1, edge_amount + 2));
	//edge_flag.push_back(0);
	//edge_flag.push_back(0);
	//edge_flag.push_back(0);
	//edge_clone.push_back(edgeid1);
	//edge_clone.push_back(edgeid2);
	//edge_clone.push_back(-1);
	//edge_amount += 3;
	//FieldWalkmeshTriple vert;
	//if (samepath1) {
	//	vert.index[0] = triangle_vertex[adjtri1].index[commonvert1];
	//} else if (samepath2) {
	//	vert.index[0] = triangle_vertex[adjtri2].index[commonvert2];
	//} else {
	//	int v = triangle_vertex[adjtri1].index[commonvert1];
	//	vert.index[0] = vertex_amount;
	//	vertex_x.push_back(walkpath_orgx[path1] + vertex_x[v] - walkpath_orgx[path]);
	//	vertex_y.push_back(walkpath_orgy[path1] + vertex_y[v] - walkpath_orgy[path]);
	//	vertex_z.push_back(walkpath_orgz[path1] + vertex_z[v] - walkpath_orgz[path]);
	//	vertex_amount++;
	//}
	//if (samepath1) {
	//	vert.index[2] = triangle_vertex[adjtri1].index[uncommonvert1];
	//} else {
	//	int v = triangle_vertex[adjtri1].index[uncommonvert1];
	//	vert.index[2] = vertex_amount;
	//	vertex_x.push_back(walkpath_orgx[path1] + vertex_x[v] - walkpath_orgx[path]);
	//	vertex_y.push_back(walkpath_orgy[path1] + vertex_y[v] - walkpath_orgy[path]);
	//	vertex_z.push_back(walkpath_orgz[path1] + vertex_z[v] - walkpath_orgz[path]);
	//	vertex_amount++;
	//}
	//if (samepath2) {
	//	vert.index[1] = triangle_vertex[adjtri2].index[uncommonvert2];
	//} else {
	//	int v = triangle_vertex[adjtri2].index[uncommonvert2];
	//	vert.index[1] = vertex_amount;
	//	vertex_x.push_back(walkpath_orgx[path2] + vertex_x[v] - walkpath_orgx[path]);
	//	vertex_y.push_back(walkpath_orgy[path2] + vertex_y[v] - walkpath_orgy[path]);
	//	vertex_z.push_back(walkpath_orgz[path2] + vertex_z[v] - walkpath_orgz[path]);
	//	vertex_amount++;
	//}
	//triangle_vertex.push_back(vert);
	//AddTriangleShared(path, flag, d, thetax, thetay);
	//return 0;
}

int FieldWalkmeshDataStruct::AddTriangle1(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, int16_t adjtri, int edgeid, int16_t newvertx, int16_t newverty, int16_t newvertz) {
	int v1 = triangle_vertex[adjtri].index[edgeid];
	int v2 = triangle_vertex[adjtri].index[(edgeid + 2) % 3];
	array<int16_t, 3> v1pos = {
		walkpath_orgx[triangle_walkpath[adjtri]] + vertex_x[v1] - walkpath_orgx[path],
		walkpath_orgy[triangle_walkpath[adjtri]] + vertex_y[v1] - walkpath_orgy[path],
		walkpath_orgz[triangle_walkpath[adjtri]] + vertex_z[v1] - walkpath_orgz[path]
	};
	array<int16_t, 3> v2pos = {
		walkpath_orgx[triangle_walkpath[adjtri]] + vertex_x[v2] - walkpath_orgx[path],
		walkpath_orgy[triangle_walkpath[adjtri]] + vertex_y[v2] - walkpath_orgy[path],
		walkpath_orgz[triangle_walkpath[adjtri]] + vertex_z[v2] - walkpath_orgz[path]
	};
	return AddTriangle(extrasize, path, flag, d, thetax, thetay, { v1pos[0], v2pos[0], newvertx }, { v1pos[1], v2pos[1], newverty }, { v1pos[2], v2pos[2], newvertz });
	//bool samepath = path == triangle_walkpath[adjtri];
	//int sizereq = 0x38 + (samepath ? 6 : 18);
	//if (extrasize < sizereq)
	//	return 1;
	//extrasize -= sizereq;
	//triangle_adjacenttriangle[adjtri].index[edgeid] = triangle_amount;
	//triangle_edge[adjtri].index[edgeid] = 0;
	//triangle_adjacenttriangle.push_back(FieldWalkmeshTriple(adjtri, -1, -1));
	//triangle_edge.push_back(FieldWalkmeshTriple(edge_amount, edge_amount + 1, edge_amount + 2));
	//edge_flag.push_back(0);
	//edge_flag.push_back(0);
	//edge_flag.push_back(0);
	//edge_clone.push_back(edgeid);
	//edge_clone.push_back(-1);
	//edge_clone.push_back(-1);
	//edge_amount += 3;
	//FieldWalkmeshTriple vert;
	//if (samepath) {
	//	vert.index[0] = triangle_vertex[adjtri].index[edgeid];
	//	vert.index[2] = triangle_vertex[adjtri].index[(edgeid + 2) % 3];
	//} else {
	//	int v1 = triangle_vertex[adjtri].index[edgeid];
	//	int v2 = triangle_vertex[adjtri].index[(edgeid + 2) % 3];
	//	vert.index[0] = vertex_amount;
	//	vert.index[2] = vertex_amount + 1;
	//	vertex_x.push_back(walkpath_orgx[triangle_walkpath[adjtri]] + vertex_x[v1] - walkpath_orgx[path]);
	//	vertex_y.push_back(walkpath_orgy[triangle_walkpath[adjtri]] + vertex_y[v1] - walkpath_orgy[path]);
	//	vertex_z.push_back(walkpath_orgz[triangle_walkpath[adjtri]] + vertex_z[v1] - walkpath_orgz[path]);
	//	vertex_x.push_back(walkpath_orgx[triangle_walkpath[adjtri]] + vertex_x[v2] - walkpath_orgx[path]);
	//	vertex_y.push_back(walkpath_orgy[triangle_walkpath[adjtri]] + vertex_y[v2] - walkpath_orgy[path]);
	//	vertex_z.push_back(walkpath_orgz[triangle_walkpath[adjtri]] + vertex_z[v2] - walkpath_orgz[path]);
	//	vertex_amount += 2;
	//}
	//vert.index[1] = vertex_amount;
	//triangle_vertex.push_back(vert);
	//vertex_x.push_back(newvertx);
	//vertex_y.push_back(newverty);
	//vertex_z.push_back(newvertz);
	//vertex_amount++;
	//AddTriangleShared(path, flag, d, thetax, thetay);
	//return 0;
}

int FieldWalkmeshDataStruct::AddTriangle(uint32_t& extrasize, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, array<int16_t, 3> newvertx, array<int16_t, 3> newverty, array<int16_t, 3> newvertz, array<int32_t, 4>* customnormal) {
	unsigned int i, j, k;
	// Setup and check errors
	for (i = 0; i < 3; i++)
		for (j = i + 1; j < 3; j++)
			if (newvertx[i] == newvertx[j] && newverty[i] == newverty[j] && newvertz[i] == newvertz[j])
				return 2;
	bool addnormal = customnormal != NULL || newvertz[0] != newvertz[1] || newvertz[0] != newvertz[2];
	FieldWalkmeshTriple existingvert = FieldWalkmeshTriple(-1, -1, -1);
	int16_t absposx[3], absposy[3], absposz[3];
	for (i = 0; i < 3; i++) {
		absposx[i] = walkpath_orgx[path] + newvertx[i];
		absposy[i] = walkpath_orgy[path] + newverty[i];
		absposz[i] = walkpath_orgz[path] + newvertz[i];
	}
	for (i = 0; i < triangle_amount; i++)
		if (path == triangle_walkpath[i])
			for (j = 0; j < 3; j++)
				for (k = 0; k < 3; k++)
					if (absposx[k] == walkpath_orgx[path] + vertex_x[triangle_vertex[i].index[j]] &&
						absposy[k] == walkpath_orgy[path] + vertex_y[triangle_vertex[i].index[j]] &&
						absposz[k] == walkpath_orgz[path] + vertex_z[triangle_vertex[i].index[j]])
						existingvert.index[k] = triangle_vertex[i].index[j];
	int newvertcount = 0;
	for (k = 0; k < 3; k++)
		if (existingvert.index[k] < 0)
			newvertcount++;
	unsigned int sizereq = 0x38 + 6 * newvertcount + (addnormal ? 0x10 : 0); // It doesn't take vertex_amount parity into account
	if (extrasize < sizereq)
		return 1;
	FieldWalkmeshTriple adjtri = FieldWalkmeshTriple(-1, -1, -1);
	FieldWalkmeshTriple adjedge = FieldWalkmeshTriple(-1, -1, -1);
	vector<pair<int, int>> commonvert;
	auto vertpairtoedge = [](int v1, int v2) {
		if (v1 < v2)
			return v1 == 1 ? 2 : (v2 == 2 ? 0 : 1);
		return v2 == 1 ? 2 : (v1 == 2 ? 0 : 1);
	};
	for (i = 0; i < triangle_amount; i++) {
		commonvert.clear();
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				if (absposx[k] == walkpath_orgx[triangle_walkpath[i]] + vertex_x[triangle_vertex[i].index[j]] &&
					absposy[k] == walkpath_orgy[triangle_walkpath[i]] + vertex_y[triangle_vertex[i].index[j]] &&
					absposz[k] == walkpath_orgz[triangle_walkpath[i]] + vertex_z[triangle_vertex[i].index[j]]) {
					commonvert.push_back({ j, k });
					break;
				}
		if (commonvert.size() == 3) {
			if (triangle_adjacenttriangle[i].index[0] >= 0 || triangle_adjacenttriangle[i].index[1] >= 0 || triangle_adjacenttriangle[i].index[2] >= 0)
				return 3;
			adjtri.index[0] = adjtri.index[1] = adjtri.index[2] = i;
			for (j = 0; j < 3; j++) {
				int edgeoldtri = vertpairtoedge(commonvert[j].first, commonvert[(j + 1) % 3].first);
				int edgenewtri = vertpairtoedge(commonvert[j].second, commonvert[(j + 1) % 3].second);
				adjedge.index[edgenewtri] = edgeoldtri;
			}
			break;
		} else if (commonvert.size() == 2) {
			int edgeoldtri = vertpairtoedge(commonvert[0].first, commonvert[1].first);
			int edgenewtri = vertpairtoedge(commonvert[0].second, commonvert[1].second);
			if (triangle_adjacenttriangle[i].index[edgeoldtri] >= 0)
				return 3;
			adjtri.index[edgenewtri] = i;
			adjedge.index[edgenewtri] = edgeoldtri;
		}
	}
	// Shift triangle index
	unsigned int insertpos = triangle_amount;
	while (insertpos > 0 && triangle_walkpath[insertpos - 1] > path)
		insertpos--;
	for (i = 0; i < triangle_amount; i++)
		for (j = 0; j < 3; j++)
			if (triangle_adjacenttriangle[i].index[j] >= (int)insertpos)
				triangle_adjacenttriangle[i].index[j]++;
	for (i = 0; i < animation_amount; i++)
		for (j = 0; j < animation_frameamount[i]; j++)
			for (k = 0; k < animation_frametriangleamount[i][j]; k++)
				if (animation_frametriangle[i][j][k] >= insertpos)
					animation_frametriangle[i][j][k]++;
	for (i = 0; i < walkpath_amount; i++)
		for (j = 0; j < walkpath_triangleamount[i]; j++)
			if (walkpath_trianglelist[i][j] >= insertpos)
				walkpath_trianglelist[i][j]++;
	for (i = 0; i < 3; i++)
		if (adjtri.index[i] >= (int)insertpos)
			adjtri.index[i]++;
	// Add triangle
	extrasize -= sizereq;
	triangle_adjacenttriangle.insert(triangle_adjacenttriangle.begin() + insertpos, adjtri);
	triangle_edge.insert(triangle_edge.begin() + insertpos, FieldWalkmeshTriple(edge_amount, edge_amount + 1, edge_amount + 2));
	edge_flag.push_back(0);
	edge_flag.push_back(0);
	edge_flag.push_back(0);
	edge_clone.push_back(adjedge.index[0]);
	edge_clone.push_back(adjedge.index[1]);
	edge_clone.push_back(adjedge.index[2]);
	edge_amount += 3;
	for (k = 0; k < 3; k++)
		if (adjtri.index[k] >= 0) {
			triangle_adjacenttriangle[adjtri.index[k]].index[adjedge.index[k]] = insertpos;
			edge_clone[triangle_edge[adjtri.index[k]].index[adjedge.index[k]]] = k;
		}
	FieldWalkmeshTriple vertlist = FieldWalkmeshTriple(-1, -1, -1);
	for (k = 0; k < 3; k++) {
		if (existingvert.index[k] < 0) {
			vertlist.index[k] = vertex_amount;
			vertex_x.push_back(newvertx[k]);
			vertex_y.push_back(newverty[k]);
			vertex_z.push_back(newvertz[k]);
			vertex_amount++;
		} else {
			vertlist.index[k] = existingvert.index[k];
		}
	}
	triangle_vertex.insert(triangle_vertex.begin() + insertpos, vertlist);
	AddTriangleShared(insertpos, path, flag, d, thetax, thetay, customnormal);
	if (addnormal && customnormal == NULL) {
		triangle_normal.insert(triangle_normal.begin() + insertpos, normal_amount);
		normal_x.push_back(0);
		normal_y.push_back(0);
		normal_z.push_back(0);
		normal_overz.push_back(0);
		normal_amount++;
		ComputeNormal(insertpos);
	}
	return 0;
}

void FieldWalkmeshDataStruct::AddTriangleShared(int insertpos, uint16_t path, uint16_t flag, int32_t d, uint16_t thetax, uint16_t thetay, array<int32_t, 4>* customnormal) {
	// TODO: find out how d should be computed
	uint16_t v1 = triangle_vertex[insertpos].index[0];
	uint16_t v2 = triangle_vertex[insertpos].index[1];
	uint16_t v3 = triangle_vertex[insertpos].index[2];
	triangle_centerx.insert(triangle_centerx.begin() + insertpos, (vertex_x[v1] + vertex_x[v2] + vertex_x[v3]) / 3);
	triangle_centery.insert(triangle_centery.begin() + insertpos, (vertex_y[v1] + vertex_y[v2] + vertex_y[v3]) / 3);
	triangle_centerz.insert(triangle_centerz.begin() + insertpos, (vertex_z[v1] + vertex_z[v2] + vertex_z[v3]) / 3);
	triangle_walkpath.insert(triangle_walkpath.begin() + insertpos, path);
	triangle_flag.insert(triangle_flag.begin() + insertpos, flag);
	triangle_d.insert(triangle_d.begin() + insertpos, d);
	triangle_thetax.insert(triangle_thetax.begin() + insertpos, thetax);
	triangle_thetay.insert(triangle_thetay.begin() + insertpos, thetay);
	triangle_data.insert(triangle_data.begin() + insertpos, 0);
	if (customnormal != NULL) {
		triangle_normal.insert(triangle_normal.begin() + insertpos, normal_amount);
		normal_x.push_back((*customnormal)[0]);
		normal_y.push_back((*customnormal)[1]);
		normal_z.push_back((*customnormal)[2]);
		normal_overz.push_back((*customnormal)[3]);
		normal_amount++;
	} else {
		triangle_normal.insert(triangle_normal.begin() + insertpos, -1);
	}
	walkpath_trianglelist[path].push_back(insertpos);
	walkpath_triangleamount[path]++;
	triangle_amount++;
}

int FieldWalkmeshDataStruct::AddNormal(uint32_t& extrasize, uint16_t triangle, int32_t nx, int32_t ny, int32_t nz, int32_t noverz) {
	int nindex = triangle_normal[triangle];
	unsigned int sizereq = nindex < 0 ? 0x10 : 0;
	if (extrasize < sizereq)
		return 1;
	extrasize -= sizereq;
	if (nindex < 0) {
		int i;
		nindex = normal_amount;
		for (i = 0; i < triangle; i++)
			if (triangle_normal[i] >= 0)
				nindex = triangle_normal[i] + 1;
		triangle_normal[triangle] = nindex;
		normal_x.insert(normal_x.begin() + nindex, nx);
		normal_y.insert(normal_y.begin() + nindex, ny);
		normal_z.insert(normal_z.begin() + nindex, nz);
		normal_overz.insert(normal_overz.begin() + nindex, noverz);
		normal_amount++;
		for (i = 0; i < triangle_amount; i++)
			if (i != triangle && triangle_normal[i] >= nindex)
				triangle_normal[i]++;
	} else {
		normal_x[nindex] = nx;
		normal_y[nindex] = ny;
		normal_z[nindex] = nz;
		normal_overz[nindex] = noverz;
	}
	return 0;
}

void FieldWalkmeshDataStruct::RemoveNormal(uint32_t& extrasize, uint16_t triangle) {
	int nindex = triangle_normal[triangle];
	if (nindex < 0)
		return;
	extrasize += 0x10;
	normal_x.erase(normal_x.begin() + nindex);
	normal_y.erase(normal_y.begin() + nindex);
	normal_z.erase(normal_z.begin() + nindex);
	normal_overz.erase(normal_overz.begin() + nindex);
	normal_amount--;
	for (int i = 0; i < triangle_amount; i++) {
		if (triangle_normal[i] == nindex)
			triangle_normal[i] = -1;
		else if (triangle_normal[i] > nindex)
			triangle_normal[i]--;
	}
}

void FieldWalkmeshDataStruct::ComputeNormal(uint16_t triangle) {
	int nindex = triangle_normal[triangle];
	if (nindex < 0)
		return;
	int v1 = triangle_vertex[triangle].index[0];
	int v2 = triangle_vertex[triangle].index[1];
	int v3 = triangle_vertex[triangle].index[2];
	int dx1 = vertex_x[v2] - vertex_x[v1];
	int dy1 = vertex_y[v2] - vertex_y[v1];
	int dz1 = vertex_z[v2] - vertex_z[v1];
	int dx2 = vertex_x[v3] - vertex_x[v1];
	int dy2 = vertex_y[v3] - vertex_y[v1];
	int dz2 = vertex_z[v3] - vertex_z[v1];
	long long nx = dy1 * dz2 - dz1 * dy2;
	long long ny = dz1 * dx2 - dx1 * dz2;
	long long nz = dx1 * dy2 - dy1 * dx2;
	double det = sqrt((double)(nx * nx + ny * ny + nz * nz));
	if (det < 1.0) {
		det = 1.0;
		nz = 1;
	}
	if (nz < 0) {
		nx = -nx;
		ny = -ny;
		nz = -nz;
	}
	normal_x[nindex] = (int32_t)round(nx / det * 65536.0);
	normal_y[nindex] = (int32_t)round(ny / det * 65536.0);
	normal_z[nindex] = (int32_t)round(nz / det * 65536.0);
	normal_overz[nindex] = nz != 0 ? (int32_t)round(det / nz * 65536.0) : (int32_t)round(100 * 65536.0);
}

int FieldWalkmeshDataStruct::ExportAsObj(string outputbase, FF9String fieldname, uint16_t fieldid) {
	unsigned int i, j, k, l;
	string filename = outputbase;
	for (i = outputbase.length() - 1; i > 0; i--)
		if (outputbase[i] == '/' || outputbase[i] == '\\') {
			filename = outputbase.substr(i + 1);
			break;
		}
	fstream fobj(outputbase.c_str(), ios::out);
	if (!fobj.is_open())
		return 1;
	fobj << "# Walkmesh of the field " << ConvertWStrToStr(fieldname.GetStr(2)) << " (" << (int)fieldid << ")" << endl;
	fobj << std::showpoint;
	uint16_t vertcount = 1;
	uint16_t vertncount = 1;
	vector<uint16_t> vertlistbypath;
	vector<uint16_t> vertlistpathindexbypath(vertex_amount, 0);
	for (i = 0; i < walkpath_amount; i++) {
		fobj << "o Walhpath_" << i << endl;
		vertlistbypath.clear();
		for (j = 0; j < walkpath_triangleamount[i]; j++) {
			for (k = 0; k < 3; k++) {
				for (l = 0; l < vertlistbypath.size(); l++)
					if (vertlistbypath[l] == triangle_vertex[walkpath_trianglelist[i][j]].index[k]) {
						vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[k]] = l;
						break;
					}
				if (l >= vertlistbypath.size()) {
					vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[k]] = vertlistbypath.size();
					vertlistbypath.push_back(triangle_vertex[walkpath_trianglelist[i][j]].index[k]);
				}
			}
		}
		for (j = 0; j < vertlistbypath.size(); j++) {
			double xx = offset_x + walkpath_orgx[i] + vertex_x[vertlistbypath[j]];
			double yy = offset_y + walkpath_orgy[i] + vertex_y[vertlistbypath[j]];
			double zz = offset_z + walkpath_orgz[i] + vertex_z[vertlistbypath[j]];
			fobj << "v " << xx << " " << -zz << " " << -yy << endl;
		}
		for (j = 0; j < walkpath_triangleamount[i]; j++) {
			if (triangle_normal[walkpath_trianglelist[i][j]] >= 0) {
				double nxx = normal_x[triangle_normal[walkpath_trianglelist[i][j]]] / 65536.0;
				double nyy = normal_y[triangle_normal[walkpath_trianglelist[i][j]]] / 65536.0;
				double nzz = normal_z[triangle_normal[walkpath_trianglelist[i][j]]] / 65536.0;
				fobj << "vn " << nxx << " " << -nzz << " " << -nyy << endl;
			} else {
				fobj << "vn " << 0.0 << " " << 65536.0 << " " << 0.0 << endl;
			}
		}
		for (j = 0; j < walkpath_triangleamount[i]; j++) {
			int vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[0]] + vertcount; fobj << "f " << vertindex << "//" << j + vertncount;
			vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[1]] + vertcount; fobj << " " << vertindex << "//" << j + vertncount;
			vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[2]] + vertcount; fobj << " " << vertindex << "//" << j + vertncount << endl;
			// Double-side
			//vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[0]] + vertcount; fobj << "f " << vertindex;
			//vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[1]] + vertcount; fobj << " " << vertindex;
			//vertindex = vertlistpathindexbypath[triangle_vertex[walkpath_trianglelist[i][j]].index[2]] + vertcount; fobj << " " << vertindex << endl;
		}
		vertcount += vertlistbypath.size();
		vertncount += walkpath_triangleamount[i];
	}
	fobj.close();
	return 0;
}

int FieldWalkmeshDataStruct::ImportFromObj(wxString intputfilename, wxString* message) {
	wxFile fobj(intputfilename, wxFile::read);
	if (!fobj.IsOpened())
		return -1;
	wxString input;
	if (!fobj.ReadAll(&input))
		return -1;
	fobj.Close();
	vector<array<int, 3>> vertexpos;
	vector<array<int, 3>> normalpos;
	vector<array<int, 3>> trianglevertex;
	vector<int> trianglenormal;
	vector<uint16_t> trianglepath;
	vector<uint16_t> triangleflag;
	vector<int32_t> triangled;
	vector<uint16_t> trianglethetax;
	vector<uint16_t> trianglethetay;
	uint32_t extrasize = size + GetExtraSize();
	uint16_t pathcount, pathindex = 0;
	wxString line, operation, tmpstr, argv, argvt, argvn;
	bool warninganimationloss = animation_amount > 0;
	bool warningmultiplenormals = false;
	bool warningquadface = false;
	int warningdegentri = 0;
	unsigned int i, j, k;
	array<int, 3> tmppos;
	int curvn, sharedvn;
	double bufferdouble;
	long bufferlong;
	int errcode;
	while (!input.IsEmpty()) {
		line = input.BeforeFirst(L'\n', &tmpstr);
		input = tmpstr;
		if (!line.IsEmpty()) {
			operation = line.BeforeFirst(L' ', &tmpstr);
			line = tmpstr;
			if (operation.StartsWith(_(L"#")))
				continue;
			if (operation.IsSameAs(_(L"o"))) {
				if (trianglevertex.size() == 0)
					continue;
				pathindex++;
			} else if (operation.IsSameAs(_(L"v"))) {
				for (i = 0; i < 3; i++) {
					line = line.BeforeFirst(L' ', &tmpstr);
					line.Trim().Trim(false);
					if (!line.ToDouble(&bufferdouble))
						return -2;
					tmppos[i] = (int)round(bufferdouble);
					line = tmpstr;
				}
				vertexpos.push_back(tmppos);
			} else if (operation.IsSameAs(_(L"vn"))) {
				for (i = 0; i < 3; i++) {
					line = line.BeforeFirst(L' ', &tmpstr);
					line.Trim().Trim(false);
					if (!line.ToDouble(&bufferdouble))
						return -2;
					tmppos[i] = (int)round(65536.0 * bufferdouble);
					line = tmpstr;
				}
				normalpos.push_back(tmppos);
			} else if (operation.IsSameAs(_(L"f"))) {
				sharedvn = -1;
				for (i = 0; i < 3; i++) {
					line = line.BeforeFirst(L' ', &tmpstr);
					argv = line.BeforeFirst(L'/', &argvt);
					argv.Trim().Trim(false);
					if (!argv.ToLong(&bufferlong))
						return -2;
					tmppos[i] = bufferlong - 1;
					argvt = argvt.BeforeFirst(L'/', &argvn);
					argvn.Trim().Trim(false);
					if (!argvn.IsEmpty()) {
						if (!argvn.ToLong(&bufferlong))
							return -2;
						curvn = bufferlong - 1;
						if (sharedvn >= 0 && sharedvn != curvn)
							warningmultiplenormals = true;
						else
							sharedvn = curvn;
					}
					line = tmpstr;
				}
				line.Trim().Trim(false);
				if (!line.IsEmpty())
					warningquadface = true;
				trianglevertex.push_back(tmppos);
				trianglenormal.push_back(sharedvn);
				trianglepath.push_back(pathindex);
				triangleflag.push_back(WALKMESH_TRIFLAG_ACTIVE);
				triangled.push_back(0);
				trianglethetax.push_back(0);
				trianglethetay.push_back(0);
			}
		}
	}
	for (i = 0; i < trianglevertex.size(); i++) {
		for (j = 0; j < 3; j++)
			if (trianglevertex[i][j] < 0 || trianglevertex[i][j] >= (int)vertexpos.size())
				return -3;
		if (trianglenormal[i] >= (int)normalpos.size())
			return -4;
	}
	pathcount = pathindex + 1;
	FieldWalkmeshDataStruct result = *this;
	result.active_walkpath = 0;
	result.active_triangle = 0;
	result.triangle_amount = 0;
	result.edge_amount = 0;
	result.animation_amount = 0;
	result.walkpath_amount = 0;
	result.normal_amount = 0;
	result.vertex_amount = 0;
	result.triangle_flag.clear();
	result.triangle_data.clear();
	result.triangle_walkpath.clear();
	result.triangle_normal.clear();
	result.triangle_thetax.clear();
	result.triangle_thetay.clear();
	result.triangle_vertex.clear();
	result.triangle_edge.clear();
	result.triangle_adjacenttriangle.clear();
	result.triangle_centerx.clear();
	result.triangle_centerz.clear();
	result.triangle_centery.clear();
	result.triangle_d.clear();
	result.edge_flag.clear();
	result.edge_clone.clear();
	result.animation_flag.clear();
	result.animation_frameamount.clear();
	result.animation_framerate.clear();
	result.animation_counter.clear();
	result.animation_currentframe.clear();
	result.animation_frameoffset.clear();
	result.animation_frameflag.clear();
	result.animation_framevalue.clear();
	result.animation_frametriangleamount.clear();
	result.animation_frametriangleoffset.clear();
	result.animation_frametriangle.clear();
	result.walkpath_flag.clear();
	result.walkpath_id.clear();
	result.walkpath_orgx.clear();
	result.walkpath_orgz.clear();
	result.walkpath_orgy.clear();
	result.walkpath_offsetx.clear();
	result.walkpath_offsetz.clear();
	result.walkpath_offsety.clear();
	result.walkpath_minx.clear();
	result.walkpath_minz.clear();
	result.walkpath_miny.clear();
	result.walkpath_maxx.clear();
	result.walkpath_maxz.clear();
	result.walkpath_maxy.clear();
	result.walkpath_triangleamount.clear();
	result.walkpath_trianglelistoffset.clear();
	result.walkpath_trianglelist.clear();
	result.normal_x.clear();
	result.normal_z.clear();
	result.normal_y.clear();
	result.normal_overz.clear();
	result.vertex_x.clear();
	result.vertex_z.clear();
	result.vertex_y.clear();
	int min[3] = { INT_MAX, INT_MAX, INT_MAX };
	int max[3] = { INT_MIN, INT_MIN, INT_MIN };
	for (i = 0; i < trianglevertex.size(); i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++) {
				if (min[k] > vertexpos[trianglevertex[i][j]][k]) min[k] = vertexpos[trianglevertex[i][j]][k];
				if (max[k] < vertexpos[trianglevertex[i][j]][k]) max[k] = vertexpos[trianglevertex[i][j]][k];
			}
	result.offset_x = result.offset_orgx = result.offset_minx = result.offset_maxx = result.offset_charx = (min[0] + max[0]) / 2;
	result.offset_y = result.offset_orgy = result.offset_miny = result.offset_maxy = result.offset_chary = -(min[2] + max[2]) / 2;
	result.offset_z = result.offset_orgz = result.offset_minz = result.offset_maxz = result.offset_charz = -(min[1] + max[1]) / 2;
	for (pathindex = 0; pathindex < pathcount; pathindex++) {
		min[0] = min[1] = min[2] = INT_MAX;
		max[0] = max[1] = max[2] = INT_MIN;
		for (i = 0; i < trianglevertex.size(); i++)
			if (trianglepath[i] == pathindex)
				for (j = 0; j < 3; j++)
					for (k = 0; k < 3; k++) {
						if (min[k] > vertexpos[trianglevertex[i][j]][k]) min[k] = vertexpos[trianglevertex[i][j]][k];
						if (max[k] < vertexpos[trianglevertex[i][j]][k]) max[k] = vertexpos[trianglevertex[i][j]][k];
					}
		errcode = result.AddWalkpath(extrasize,
			(min[0] + max[0]) / 2 - result.offset_x,
			-(min[2] + max[2]) / 2 - result.offset_y,
			-(min[1] + max[1]) / 2 - result.offset_z);
		if (errcode != 0)
			return errcode;
	}
	array<int16_t, 3> newvertx, newverty, newvertz;
	array<int32_t, 4> newnormal;
	for (i = 0; i < trianglevertex.size(); i++) {
		for (j = 0; j < 3; j++) {
			newvertx[j] = vertexpos[trianglevertex[i][j]][0] - result.offset_x - result.walkpath_orgx[trianglepath[i]];
			newverty[j] = -vertexpos[trianglevertex[i][j]][2] - result.offset_y - result.walkpath_orgy[trianglepath[i]];
			newvertz[j] = -vertexpos[trianglevertex[i][j]][1] - result.offset_z - result.walkpath_orgz[trianglepath[i]];
		}
		if (trianglenormal[i] >= 0) {
			newnormal[0] = normalpos[trianglenormal[i]][0];
			newnormal[1] = -normalpos[trianglenormal[i]][2];
			newnormal[2] = -normalpos[trianglenormal[i]][1];
			newnormal[3] = 0;
			errcode = result.AddTriangle(extrasize, trianglepath[i], triangleflag[i], triangled[i], trianglethetax[i], trianglethetay[i], newvertx, newverty, newvertz, &newnormal);
		} else {
			errcode = result.AddTriangle(extrasize, trianglepath[i], triangleflag[i], triangled[i], trianglethetax[i], trianglethetay[i], newvertx, newverty, newvertz);
		}
		if (errcode == 1)
			return errcode;
		else if (errcode == 2)
			warningdegentri++;
	}
	*this = result;
	UpdateOffset();
	if (message != NULL) {
		*message = wxString::Format(wxT(HADES_STRING_WALKMESH_IMPORT_SUCCESS), walkpath_amount, triangle_amount, vertex_amount);
		if (warningdegentri > 0 || warninganimationloss || warningmultiplenormals || warningquadface) {
			*message += _(L"\n") + _(HADES_STRING_WARNING) + _(L":\n");
			if (warningdegentri > 0)
				*message += wxString::Format(wxT(HADES_STRING_WALKMESH_IMPORT_DEGEN_TRI), warningdegentri);
			if (warninganimationloss)
				*message += _(HADES_STRING_WALKMESH_IMPORT_ANIM_LOSS);
			if (warningmultiplenormals)
				*message += _(HADES_STRING_WALKMESH_IMPORT_NORMALS);
			if (warningquadface)
				*message += _(HADES_STRING_WALKMESH_IMPORT_QUADS);
		}
		*message += _(HADES_STRING_WALKMESH_IMPORT_HINT);
	}
	return 0;
}

int FieldRoleDataStruct::AddModelRole(uint16_t modelid) {
	if (GetExtraSize() < 0x10)
		return 1;
	SetSize(size + 0x10);
	uint16_t* newmodel = new uint16_t[unk2_amount + 1];
	uint8_t* newunk1 = new uint8_t[unk2_amount + 1];
	uint8_t* newunk2 = new uint8_t[unk2_amount + 1];
	uint8_t* newunk3 = new uint8_t[unk2_amount + 1];
	uint8_t* newunk4 = new uint8_t[unk2_amount + 1];
	uint8_t* newunk5 = new uint8_t[unk2_amount + 1];
	uint8_t* newunk6 = new uint8_t[unk2_amount + 1];
	memcpy(newmodel, unk2_model, unk2_amount * sizeof(uint16_t));
	memcpy(newunk1, unk2_unknown1, unk2_amount * sizeof(uint8_t));
	memcpy(newunk2, unk2_unknown2, unk2_amount * sizeof(uint8_t));
	memcpy(newunk3, unk2_unknown3, unk2_amount * sizeof(uint8_t));
	memcpy(newunk4, unk2_unknown4, unk2_amount * sizeof(uint8_t));
	memcpy(newunk5, unk2_unknown5, unk2_amount * sizeof(uint8_t));
	memcpy(newunk6, unk2_unknown6, unk2_amount * sizeof(uint8_t));
	newmodel[unk2_amount] = modelid;
	newunk1[unk2_amount] = 0;
	newunk2[unk2_amount] = 0;
	newunk3[unk2_amount] = 0x10;
	newunk4[unk2_amount] = 0x10;
	newunk5[unk2_amount] = 0x10;
	newunk6[unk2_amount] = 0;
	delete[] unk2_model;
	delete[] unk2_unknown1;
	delete[] unk2_unknown2;
	delete[] unk2_unknown3;
	delete[] unk2_unknown4;
	delete[] unk2_unknown5;
	delete[] unk2_unknown6;
	unk2_model = newmodel;
	unk2_unknown1 = newunk1;
	unk2_unknown2 = newunk2;
	unk2_unknown3 = newunk3;
	unk2_unknown4 = newunk4;
	unk2_unknown5 = newunk5;
	unk2_unknown6 = newunk6;
	unk2_amount++;
	unk2_amount2++;
	return 0;
}

void FieldRoleDataStruct::RemoveModelRole(uint16_t unk2id) {
	SetSize(size - 0x10);
	uint16_t* newmodel = new uint16_t[unk2_amount - 1];
	uint8_t* newunk1 = new uint8_t[unk2_amount - 1];
	uint8_t* newunk2 = new uint8_t[unk2_amount - 1];
	uint8_t* newunk3 = new uint8_t[unk2_amount - 1];
	uint8_t* newunk4 = new uint8_t[unk2_amount - 1];
	uint8_t* newunk5 = new uint8_t[unk2_amount - 1];
	uint8_t* newunk6 = new uint8_t[unk2_amount - 1];
	memcpy(newmodel, unk2_model, unk2id * sizeof(uint16_t));
	memcpy(newunk1, unk2_unknown1, unk2id * sizeof(uint8_t));
	memcpy(newunk2, unk2_unknown2, unk2id * sizeof(uint8_t));
	memcpy(newunk3, unk2_unknown3, unk2id * sizeof(uint8_t));
	memcpy(newunk4, unk2_unknown4, unk2id * sizeof(uint8_t));
	memcpy(newunk5, unk2_unknown5, unk2id * sizeof(uint8_t));
	memcpy(newunk6, unk2_unknown6, unk2id * sizeof(uint8_t));
	memcpy(newmodel + unk2id, unk2_model + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint16_t));
	memcpy(newunk1 + unk2id, unk2_unknown1 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	memcpy(newunk2 + unk2id, unk2_unknown2 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	memcpy(newunk3 + unk2id, unk2_unknown3 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	memcpy(newunk4 + unk2id, unk2_unknown4 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	memcpy(newunk5 + unk2id, unk2_unknown5 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	memcpy(newunk6 + unk2id, unk2_unknown6 + unk2id + 1, (unk2_amount - unk2id - 1) * sizeof(uint8_t));
	delete[] unk2_model;
	delete[] unk2_unknown1;
	delete[] unk2_unknown2;
	delete[] unk2_unknown3;
	delete[] unk2_unknown4;
	delete[] unk2_unknown5;
	delete[] unk2_unknown6;
	unk2_model = newmodel;
	unk2_unknown1 = newunk1;
	unk2_unknown2 = newunk2;
	unk2_unknown3 = newunk3;
	unk2_unknown4 = newunk4;
	unk2_unknown5 = newunk5;
	unk2_unknown6 = newunk6;
	unk2_amount--;
	unk2_amount2--;
}

#define MACRO_ROLE_IOFUNCTION(IO, SEEK, READ, PPF) \
	unsigned int i; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f, magic_role); \
	IO ## Short(f, zero1); \
	IO ## Char(f, unk1_amount); \
	IO ## Char(f, unk1_amount2); \
	IO ## Char(f, unk2_amount); \
	IO ## Char(f, unk2_amount2); \
	IO ## Short(f, zero2); \
	if (READ) { \
		unk1_unknown1 = new uint8_t[unk1_amount]; \
		unk1_unknown2 = new uint8_t[unk1_amount]; \
		unk1_unknown3 = new uint8_t[unk1_amount]; \
		unk1_unknown4 = new uint8_t[unk1_amount]; \
		unk1_unknown5 = new uint8_t[unk1_amount]; \
		unk1_unknown6 = new uint8_t[unk1_amount]; \
		unk1_unknown7 = new uint8_t[unk1_amount]; \
		unk1_unknown8 = new uint8_t[unk1_amount]; \
		unk1_unknown9 = new uint8_t[unk1_amount]; \
		unk1_unknown10 = new uint8_t[unk1_amount]; \
		unk1_unknown11 = new uint8_t[unk1_amount]; \
		unk1_unknown12 = new uint8_t[unk1_amount]; \
		unk2_model = new uint16_t[unk2_amount]; \
		unk2_unknown1 = new uint8_t[unk2_amount]; \
		unk2_unknown2 = new uint8_t[unk2_amount]; \
		unk2_unknown3 = new uint8_t[unk2_amount]; \
		unk2_unknown4 = new uint8_t[unk2_amount]; \
		unk2_unknown5 = new uint8_t[unk2_amount]; \
		unk2_unknown6 = new uint8_t[unk2_amount]; \
	} \
	for (i = 0; i < unk1_amount; i++) { \
		IO ## Char(f, unk1_unknown1[i]); \
		IO ## Char(f, unk1_unknown2[i]); \
		IO ## Char(f, unk1_unknown3[i]); \
		IO ## Char(f, unk1_unknown4[i]); \
		IO ## Char(f, unk1_unknown5[i]); \
		IO ## Char(f, unk1_unknown6[i]); \
		IO ## Char(f, unk1_unknown7[i]); \
		IO ## Char(f, unk1_unknown8[i]); \
		IO ## Char(f, unk1_unknown9[i]); \
		IO ## Char(f, unk1_unknown10[i]); \
		IO ## Char(f, unk1_unknown11[i]); \
		IO ## Char(f, unk1_unknown12[i]); \
	} \
	for (i = 0; i < unk2_amount; i++) { \
		IO ## Short(f, unk2_model[i]); \
		IO ## Char(f, unk2_unknown1[i]); \
		IO ## Char(f, unk2_unknown2[i]); \
		IO ## Char(f, unk2_unknown3[i]); \
		IO ## Char(f, unk2_unknown4[i]); \
		IO ## Char(f, unk2_unknown5[i]); \
		IO ## Char(f, unk2_unknown6[i]); \
	} \
	if (PPF) PPFEndScanStep();


void FieldRoleDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_ROLE_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		MACRO_ROLE_IOFUNCTION(SteamRead,SteamSeek,true,false)
	}
	loaded = true;
}

void FieldRoleDataStruct::Write(fstream& f) {
	MACRO_ROLE_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void FieldRoleDataStruct::WritePPF(fstream& f) {
	MACRO_ROLE_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void FieldRoleDataStruct::ReadHWS(fstream& f) {
	MACRO_ROLE_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void FieldRoleDataStruct::WriteHWS(fstream& f) {
	MACRO_ROLE_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

bool FieldSteamTitleInfo::ReadTitleTileId(FieldTilesDataStruct* tileset, ConfigurationSet& config) {
	unsigned int i, j, k, newtileid, tileindex;
	SteamLanguage lang, langi;
	for (i = 0; i < amount; i++)
		if (field_id[i] == tileset->GetFieldId()) {
			stringstream fnamestream;
			fnamestream << config.steam_dir_assets << "p0data1" << (unsigned int)config.field_file_id[tileset->id] << ".bin";
			fstream ffbin(fnamestream.str().c_str(), ios::in | ios::binary);
			for (langi = STEAM_LANGUAGE_US; langi < STEAM_LANGUAGE_AMOUNT; langi++) {
				if (langi == STEAM_LANGUAGE_EN && !has_uk[i])
					lang = STEAM_LANGUAGE_US;
				else
					lang = langi;
				FieldTilesDataStruct localbackground;
				ffbin.seekg(config.meta_field[config.field_file_id[tileset->id] - 1].GetFileOffsetByIndex(config.field_tiles_file[tileset->id][lang]));
				localbackground.Init(NULL, CHUNK_TYPE_FIELD_TILES, config.field_id[tileset->id]);
				localbackground.parent = tileset->parent;
				// localbackground.id = i;
				localbackground.size = config.meta_field[config.field_file_id[tileset->id] - 1].GetFileSizeByIndex(config.field_tiles_file[tileset->id][lang]);
				localbackground.Read(ffbin);
				if (lang == STEAM_LANGUAGE_US) {
					newtileid = atlas_title_pos[STEAM_LANGUAGE_US][i];
					for (j = 0; j < title_tile_start[i]; j++)
						newtileid += tileset->tiles[j].tile_amount;
				} else
					newtileid = atlas_title_pos[langi][i];
				for (j = title_tile_start[i]; j <= title_tile_last[i]; j++) {
					tileindex = tileset->tiles_amount + langi * tileset->title_tile_amount + j - title_tile_start[i];
					tileset->tiles[tileindex].data1 = localbackground.tiles[j].data1;
					tileset->tiles[tileindex].width = localbackground.tiles[j].width;
					tileset->tiles[tileindex].height = localbackground.tiles[j].height;
					tileset->tiles[tileindex].default_x = localbackground.tiles[j].default_x;
					tileset->tiles[tileindex].default_y = localbackground.tiles[j].default_y;
					tileset->tiles[tileindex].pos_x = localbackground.tiles[j].pos_x;
					tileset->tiles[tileindex].pos_y = localbackground.tiles[j].pos_y;
					tileset->tiles[tileindex].pos_minx = localbackground.tiles[j].pos_minx;
					tileset->tiles[tileindex].pos_maxx = localbackground.tiles[j].pos_maxx;
					tileset->tiles[tileindex].pos_miny = localbackground.tiles[j].pos_miny;
					tileset->tiles[tileindex].pos_maxy = localbackground.tiles[j].pos_maxy;
					tileset->tiles[tileindex].screen_x = localbackground.tiles[j].screen_x;
					tileset->tiles[tileindex].screen_y = localbackground.tiles[j].screen_y;
					tileset->tiles[tileindex].pos_dx = localbackground.tiles[j].pos_dx;
					tileset->tiles[tileindex].pos_dy = localbackground.tiles[j].pos_dy;
					tileset->tiles[tileindex].pos_fracx = localbackground.tiles[j].pos_fracx;
					tileset->tiles[tileindex].pos_fracy = localbackground.tiles[j].pos_fracy;
					tileset->tiles[tileindex].camera_id = localbackground.tiles[j].camera_id;
					tileset->tiles[tileindex].data2 = localbackground.tiles[j].data2;
					tileset->tiles[tileindex].tile_amount = localbackground.tiles[j].tile_amount;
					tileset->tiles[tileindex].tile_pos_offset = localbackground.tiles[j].tile_pos_offset;
					tileset->tiles[tileindex].tile_data_offset = localbackground.tiles[j].tile_data_offset;
					tileset->tiles[tileindex].tile_packet = localbackground.tiles[j].tile_packet;
					tileset->tiles[tileindex].tile_tpage = localbackground.tiles[j].tile_tpage;
					tileset->tiles[tileindex].tile = new FieldTilesTileDataStruct[tileset->tiles[tileindex].tile_amount];
					for (k = 0; k < tileset->tiles[tileindex].tile_amount; k++) {
						tileset->tiles[tileindex].tile[k] = localbackground.tiles[j].tile[k];
						tileset->tiles[tileindex].tile[k].steam_id = newtileid++;
					}
				}
			}
			ffbin.close();
			return true;
		}
	return false;
}

void FieldSteamTitleInfo::Read(fstream& f) {
	unsigned int i, j;
	char* buffer;
	string line, file;
	stringstream bufferstr, filestr;
	buffer = new char[size + 1];
	f.read(buffer, size);
	buffer[size] = 0;
	bufferstr << buffer;
	delete[] buffer;
	amount = 0;
	file = bufferstr.str();
	getline(bufferstr, line);
	while (line.length() > 0) {
		amount++;
		getline(bufferstr, line);
	}
	field_id = new uint16_t[amount];
	width = new uint16_t[amount];
	height = new uint16_t[amount];
	title_tile_start = new uint16_t[amount];
	title_tile_last = new uint16_t[amount];
	has_uk = new bool[amount];
	for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++) {
		atlas_title_pos[i] = new uint32_t[amount];
		atlas_title_amount[i] = new uint32_t[amount];
	}
	filestr << file;
	getline(filestr, line);
	for (i = 0; i < amount; i++) {
		stringstream linestr;
		string fieldname;
		int hasukint;
		char c;
		linestr << line;
		while (!linestr.eof() && (c = linestr.get()) != ',') { fieldname.push_back(c); }
		for (j = 0; j < G_V_ELEMENTS(SteamFieldScript); j++)
			if (fieldname.compare(SteamFieldScript[j].background_name) == 0) {
				field_id[i] = SteamFieldScript[j].script_id;
				break;
			}
		linestr >> width[i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> height[i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> title_tile_start[i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> title_tile_last[i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> hasukint;
		has_uk[i] = hasukint == 1;
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_US][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_US][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_JA][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_JA][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_SP][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_SP][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_FR][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_FR][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_GE][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_GE][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_IT][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_IT][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_pos[STEAM_LANGUAGE_EN][i];
		while (!linestr.eof() && linestr.get() != ',') {}
		linestr >> atlas_title_amount[STEAM_LANGUAGE_EN][i];
		getline(filestr, line);
	}
}

void FieldSteamTitleInfo::Write(fstream& f) {
	// ToDo ; also compute the size before
}

int FieldAdditionDataStruct::ExportAssets(ConfigurationSet& config, SaveSet& saveset, string destfolder) {
	string mapid = ConvertWStrToStr(L"FBG_N" + (area_id < 10 ? L"0" + to_wstring(area_id) : to_wstring(area_id)) + L"_" + map_id);
	string fieldid = ConvertWStrToStr(L"EVT_" + field_id);
	if (destfolder.back() != '\\' && destfolder.back() != '/')
		destfolder += "\\";
	if (saveset.fieldset->background_data[index] != NULL) {
		int errorcode = saveset.fieldset->background_data[index]->WriteBGX(destfolder + "StreamingAssets\\Assets\\Resources\\FieldMaps\\" + mapid + "\\", mapid + ".bgx");
		if (errorcode != 0)
			return errorcode;
	}
	if (saveset.fieldset->walkmesh[index] != NULL) {
		string fname = destfolder + "StreamingAssets\\Assets\\Resources\\FieldMaps\\" + mapid + "\\" + mapid + ".bgi.bytes";
		MainFrame::MakeDirForFile(fname);
		fstream filedest(fname.c_str(), ios::out | ios::binary);
		if (!filedest.is_open())
			return 3;
		saveset.fieldset->walkmesh[index]->WriteHWS(filedest);
		filedest.close();
	}
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		string fname = destfolder + "StreamingAssets\\Assets\\Resources\\CommonAsset\\EventEngine\\EventBinary\\Field\\" + HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[lang].ToStdString() + "\\" + fieldid + ".eb.bytes";
		MainFrame::MakeDirForFile(fname);
		fstream filedest(fname.c_str(), ios::out | ios::binary);
		if (!filedest.is_open())
			return 3;
		saveset.fieldset->script_data[index]->WriteSteam(filedest, true, lang);
		filedest.close();
	}
	int spsindex = saveset.fieldset->GetIndexById(sps_pool);
	if (spsindex >= 0 && config.field_file_id[spsindex] > 0) {
		fstream filebase(config.steam_dir_assets + "p0data1" + to_string(config.field_file_id[spsindex]) + ".bin", ios::in | ios::binary);
		if (!filebase.is_open())
			return 2;
		if (config.field_spt_file[spsindex] >= 0) {
			string fname = destfolder + "StreamingAssets\\Assets\\Resources\\FieldMaps\\" + mapid + "\\spt.tcb.bytes";
			int errcode = ConfigurationSet::SteamExtractAssetWithPath(fname, filebase, config.meta_field[config.field_file_id[spsindex] - 1], config.field_spt_file[spsindex]);
			if (errcode != 0)
				return errcode;
		}
		for (unsigned int i = 0; i < SteamFieldScript[spsindex].sps_list.size(); i++) {
			if (config.field_sps_file[spsindex][i] < 0)
				continue;
			string fname = destfolder + "StreamingAssets\\Assets\\Resources\\FieldMaps\\" + mapid + "\\" + to_string(SteamFieldScript[spsindex].sps_list[i]) + ".sps.bytes";
			int errcode = ConfigurationSet::SteamExtractAssetWithPath(fname, filebase, config.meta_field[config.field_file_id[spsindex] - 1], config.field_sps_file[spsindex][i]);
			if (errcode != 0)
				return errcode;
		}
		filebase.close();
	}
	int evid = saveset.fieldset->GetIdByIndex(index);
	if (!MemoriaUtility::ExportDictionaryPatchLines(destfolder + HADES_STRING_DICTIONARY_PATCH_FILE, wxString::Format(wxT("FieldScene %d "), evid), wxString::Format(wxT("FieldScene %d %d %s %s %d\n"), evid, area_id, map_id, field_id, text_block_id)))
		return 3;
	return 0;
}

void FieldAdditionDataStruct::ReadHWS(fstream& f) {
	uint8_t version;
	HWSReadFlexibleShort(f, base_field_id, true);
	HWSReadChar(f, version);
	HWSReadChar(f, area_id);
	HWSReadWString(f, map_id);
	HWSReadWString(f, field_id);
	HWSReadFlexibleChar(f, text_block_id, true);
	HWSReadShort(f, sps_pool);
}

void FieldAdditionDataStruct::WriteHWS(fstream& f) {
	HWSWriteFlexibleShort(f, base_field_id, true);
	HWSWriteChar(f, HWS_ADDITION_INFO_VERSION_CURRENT);
	HWSWriteChar(f, area_id);
	HWSWriteWString(f, map_id);
	HWSWriteWString(f, field_id);
	HWSWriteFlexibleChar(f, text_block_id, true);
	HWSWriteShort(f, sps_pool);
}

bool FieldDataSet::CreateCustomField(ConfigurationSet& config, uint16_t basefieldindex, int customid) {
	string fname = config.steam_dir_assets + "p0data7.bin";
	fstream ffbinscript(fname.c_str(), ios::in | ios::binary);
	if (config.field_file_id[basefieldindex] == 0)
		fname = config.steam_dir_assets + "p0data11.bin";
	else
		fname = config.steam_dir_assets + "p0data1" + to_string((unsigned int)config.field_file_id[basefieldindex]) + ".bin";
	fstream ffbinfield(fname.c_str(), ios::in | ios::binary);
	if (!ffbinscript.is_open() || !ffbinfield.is_open())
		return false;
	uint16_t baseid = (uint16_t)GetIdByIndex(basefieldindex);
	ClusterData* dummyclus = NULL;
	SteamLanguage lang;
	script_data.push_back(new ScriptDataStruct[1]);
	script_data[amount]->related_charmap_id = 0;
	script_data[amount]->Init(true, CHUNK_TYPE_SCRIPT, baseid, &dummyclus, CLUSTER_TYPE_FIELD);
	script_data[amount]->is_field_script = true;
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
			continue;
		ffbinscript.seekg(config.meta_script.GetFileOffsetByIndex(config.field_script_file[lang][basefieldindex]));
		script_data[amount]->Read(ffbinscript, lang);
	}
	script_data[amount]->size = config.meta_script.GetFileSizeByIndex(config.field_script_file[GetSteamLanguage()][basefieldindex]);
	script_data[amount]->name = script_data[basefieldindex]->name;
	ffbinscript.seekg(config.meta_script.GetFileOffsetByIndex(config.field_role_file[basefieldindex]));
	role.push_back(new FieldRoleDataStruct[1]);
	role[amount]->Init(false, CHUNK_TYPE_FIELD_ROLE, baseid, &dummyclus);
	role[amount]->size = config.meta_script.GetFileSizeByIndex(config.field_role_file[basefieldindex]);
	role[amount]->Read(ffbinscript);
	preload.push_back(NULL);
	if (config.field_file_id[basefieldindex] == 0) {
		walkmesh.push_back(NULL);
		tim_data.push_back(NULL);
		background_data.push_back(NULL);
	} else {
		ffbinfield.seekg(config.meta_field[config.field_file_id[basefieldindex] - 1].GetFileOffsetByIndex(config.field_walkmesh_file[basefieldindex]));
		walkmesh.push_back(new FieldWalkmeshDataStruct[1]);
		walkmesh[amount]->Init(false, CHUNK_TYPE_FIELD_WALK, baseid, &dummyclus);
		walkmesh[amount]->size = config.meta_field[config.field_file_id[basefieldindex] - 1].GetFileSizeByIndex(config.field_walkmesh_file[basefieldindex]);
		walkmesh[amount]->Read(ffbinfield);
		//ffbinfield.seekg(config.meta_field[config.field_file_id[basefieldindex] - 1].GetFileOffsetByIndex(config.field_image_file[basefieldindex]));
		tim_data.push_back(new TIMImageDataStruct[1]);
		tim_data[amount]->Init(false, CHUNK_TYPE_TIM, baseid, &dummyclus);
		tim_data[amount]->data_file_name = tim_data[basefieldindex]->data_file_name;
		tim_data[amount]->data_file_offset = tim_data[basefieldindex]->data_file_offset;
		//tim_data[amount]->Read(ffbinfield);
		ffbinfield.seekg(config.meta_field[config.field_file_id[basefieldindex] - 1].GetFileOffsetByIndex(config.field_tiles_file[basefieldindex][0]));
		background_data.push_back(new FieldTilesDataStruct[1]);
		background_data[amount]->Init(false, CHUNK_TYPE_FIELD_TILES, baseid, &dummyclus);
		background_data[amount]->parent = this;
		background_data[amount]->id = amount;
		background_data[amount]->size = config.meta_field[config.field_file_id[basefieldindex] - 1].GetFileSizeByIndex(config.field_tiles_file[basefieldindex][0]);
		background_data[amount]->Read(ffbinfield, 0);
	}
	struct_id.push_back(customid);
	related_text.push_back(related_text[basefieldindex]);
	FieldAdditionDataStruct* add = new FieldAdditionDataStruct[1];
	add->index = amount;
	add->base_field_id = baseid;
	add->area_id = 57;
	add->map_id = L"CUSTOM_FIELD_" + to_wstring(customid);
	add->field_id = L"CUSTOM_FIELD_" + to_wstring(customid);
	add->text_block_id = related_text[basefieldindex] != NULL ? related_text[basefieldindex]->block_id : 0;
	add->sps_pool = baseid;
	addition.push_back(add);
	amount++;
	ffbinscript.close();
	ffbinfield.close();
	return true;
}

void FieldDataSet::DeleteCustomField(int index) {
	struct_id.erase(struct_id.begin() + index);
	script_data.erase(script_data.begin() + index);
	preload.erase(preload.begin() + index);
	background_data.erase(background_data.begin() + index);
	walkmesh.erase(walkmesh.begin() + index);
	tim_data.erase(tim_data.begin() + index);
	role.erase(role.begin() + index);
	addition.erase(addition.begin() + index);
	related_text.erase(related_text.begin() + index);
	amount--;
	for (unsigned int i = index; i < amount; i++) {
		if (addition[i] != NULL)
			addition[i]->index = i;
		background_data[i]->id = i;
	}
}

int FieldDataSet::SetFieldName(unsigned int fieldindex, wstring newvalue, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX && lang != GetSteamLanguage())
		return 0;
	int oldlen = script_data[fieldindex]->name.length;
	int res = script_data[fieldindex]->SetName(newvalue, lang);
/*	if (res == 0 && GetGameType() != GAME_TYPE_PSX && lang == GetSteamLanguage())
		name_space_used += script_data[fieldindex]->name.length - oldlen;*/
	return res;
}

int FieldDataSet::SetFieldName(unsigned int fieldindex, FF9String& newvalue) {
	int oldlen = script_data[fieldindex]->name.length;
	int res = script_data[fieldindex]->SetName(newvalue);
/*	if (res == 0 && GetGameType() != GAME_TYPE_PSX && lang == GetSteamLanguage())
		name_space_used += script_data[fieldindex]->name.length - oldlen;*/
	return res;
}

void FieldDataSet::Load(fstream& ffbin, ClusterSet& clusset, TextDataSet* textset) {
	unsigned int i, j, k, l;
	int relatedtxtid;
	amount = clusset.field_amount;
	struct_id.resize(amount);
	script_data.resize(amount);
	preload.resize(amount);
	background_data.resize(amount);
	walkmesh.resize(amount);
	tim_data.resize(amount);
	role.resize(amount);
	addition.resize(amount);
	related_text.resize(amount);
	tile_size = GetGameType() == GAME_TYPE_PSX ? FIELD_TILE_BASE_SIZE : hades::FIELD_BACKGROUND_RESOLUTION;
	tile_gap = GetGameType() == GAME_TYPE_PSX ? 0 : 2;
	j = 0;
	LoadingDialogInit(amount, _(L"Reading fields..."));
	if (GetGameType() == GAME_TYPE_PSX) {
		cluster_id.resize(amount);
		for (i = 0; i < clusset.amount; i++) {
			if (clusset.clus_type[i] == CLUSTER_TYPE_FIELD) {
				clusset.clus[i].CreateChildren(ffbin);
				for (k = 0; k < clusset.clus[i].chunk_amount; k++) {
					for (l = 0; l < clusset.clus[i].chunk[k].object_amount; l++) {
						ffbin.seekg(clusset.clus[i].chunk[k].object_offset[l]);
						clusset.clus[i].chunk[k].GetObject(l).Read(ffbin);
					}
				}
				cluster_id[j] = i;
				ClusterData& clustim = (ClusterData&)clusset.clus[i].chunk[clusset.clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA)].GetObject(0);
				clustim.CreateChildren(ffbin);
				for (k = 0; k < clustim.chunk_amount; k++) {
					for (l = 0; l < clustim.chunk[k].object_amount; l++) {
						ffbin.seekg(clustim.chunk[k].object_offset[l]);
						clustim.chunk[k].GetObject(l).Read(ffbin);
					}
				}
				ClusterData& clus = (ClusterData&)clusset.clus[i].chunk[clusset.clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA)].GetObject(1);
				clus.CreateChildren(ffbin);
				for (k = 0; k < clus.chunk_amount; k++) {
					if (clus.chunk[k].type == CHUNK_TYPE_SCRIPT)
						((ScriptDataStruct*)&clus.chunk[k].GetObject(0))->is_field_script = true;
					for (l = 0; l < clus.chunk[k].object_amount; l++) {
						ffbin.seekg(clus.chunk[k].object_offset[l]);
						clus.chunk[k].GetObject(l).Read(ffbin);
					}
				}
				ChunkData& chunkscript = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_SCRIPT)];
				script_data[j] = (ScriptDataStruct*)&chunkscript.GetObject(0);
				ChunkData& chunktiles = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_FIELD_TILES)];
				background_data[j] = (FieldTilesDataStruct*)&chunktiles.GetObject(0);
				background_data[j]->parent = this;
				background_data[j]->id = j;
				ChunkData& chunkwalk = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_FIELD_WALK)];
				walkmesh[j] = (FieldWalkmeshDataStruct*)&chunkwalk.GetObject(0);
				ChunkData& chunkrole = clus.chunk[clus.SearchChunkType(CHUNK_TYPE_FIELD_ROLE)];
				role[j] = (FieldRoleDataStruct*)&chunkrole.GetObject(0);
				ChunkData& chunktim = clustim.chunk[clustim.SearchChunkType(CHUNK_TYPE_TIM)];
				tim_data[j] = (TIMImageDataStruct*)&chunktim.GetObject(0);
				ChunkData& chunkimgmap = clusset.clus[i].chunk[clusset.clus[i].SearchChunkType(CHUNK_TYPE_IMAGE_MAP)];
				preload[j] = (ImageMapDataStruct*)&chunkimgmap.GetObject(0);
				struct_id[j] = script_data[j]->object_id;
				GlobalMapCommonDirStruct& mapdir = clusset.global_map.common_dir[GLOBAL_MAP_DIR_FIELD];
				relatedtxtid = -1;
				for (k = 0; k < mapdir.file_amount; k++)
					if (mapdir.file_id[k] == struct_id[j]) {
						relatedtxtid = mapdir.file_related_id[k];
						break;
					}
				if (relatedtxtid >= 0) {
					related_text[j] = (TextDataStruct*)clusset.FindObjectById(CHUNK_TYPE_TEXT, relatedtxtid);
					if (related_text[j]->parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP) >= 0)
						script_data[j]->related_charmap_id = related_text[j]->parent_cluster->chunk[related_text[j]->parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)].object_id[0];
					else
						script_data[j]->related_charmap_id = 0;
				} else {
					related_text[j] = NULL;
					script_data[j]->related_charmap_id = 0;
				}
				script_data[j]->name.charmap_Ext = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(script_data[j]->related_charmap_id);
				script_data[j]->name.ReadFromChar(script_data[j]->header_name[GetSteamLanguage()]);
				addition[j] = NULL;
				j++;
				LoadingDialogUpdate(j);
			}
		}
	} else {
		ClusterData** dummyclus = new ClusterData*[amount];
		ConfigurationSet& config = *clusset.config;
		string fname = config.steam_dir_data;
		bool fieldnameinit = false;
		FF9String* fieldname = new FF9String[amount];
		uint16_t* fieldnameid = new uint16_t[amount];
		uint16_t fieldnameidtmp;
		uint8_t strbuffer[10];
		SteamLanguage lang;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.field_text_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.field_text_file[lang]);
			for (i = 0; i < amount; i++) {
				j = 0;
				do SteamReadChar(ffbin, strbuffer[j]);
				while (strbuffer[j++] != ':');
				strbuffer[j - 1] = 0;
				fieldnameidtmp = atoi((const char*)strbuffer);
				if (fieldnameidtmp == 1805) { // DEBUG: Registered field lacking data
					FF9String dumpedstr;
					SteamReadFF9String(ffbin, dumpedstr);
					i--;
				} else if (!fieldnameinit) {
					fieldnameid[i] = fieldnameidtmp;
					SteamReadFF9String(ffbin, fieldname[i], lang);
				} else if (fieldnameid[i] == fieldnameidtmp) {
					SteamReadFF9String(ffbin, fieldname[i], lang);
				} else {
					for (k = 0; k < amount; k++)
						if (fieldnameid[k] == fieldnameidtmp) {
							SteamReadFF9String(ffbin, fieldname[k], lang);
							break;
						}
					if (k >= amount) {
						FF9String dumpedstr;
						SteamReadFF9String(ffbin, dumpedstr);
					}
				}
			}
			fieldnameinit = true;
		}
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.field_title_info));
		title_info = new FieldSteamTitleInfo[1];
		title_info->size = config.meta_res.GetFileSizeByIndex(config.field_title_info);
		title_info->Read(ffbin);
		ffbin.close();
		fname = config.steam_dir_assets + "p0data7.bin";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		int scriptfileamount = G_V_ELEMENTS(SteamFieldScript);
		int txtfileamount = G_V_ELEMENTS(SteamTextFile);
		vector<string> steamtxtfile;
		string fieldfilename;
		steamtxtfile.reserve(txtfileamount);
		for (i = 0; i < (unsigned int)txtfileamount; i++)
			steamtxtfile.push_back(SteamTextFile[i].name.substr(4)); // remove "MES_"
		for (i = 0; i < amount; i++) {
			struct_id[i] = config.field_id[i];
			related_text[i] = NULL;
			relatedtxtid = -1;
			fieldfilename = "";
			for (j = 0; j < (unsigned int)scriptfileamount; j++)
				if (SteamFieldScript[j].script_id == struct_id[i]) {
					fieldfilename = SteamFieldScript[j].script_name.substr(4); // remove "EVT_"
					break;
				}
			for (j = 0; j < (unsigned int)txtfileamount; j++) {
				string& txtfilename = steamtxtfile[j];
				if (txtfilename.length() < fieldfilename.length() && fieldfilename[txtfilename.length()] == '_' && txtfilename.compare(fieldfilename.substr(0, txtfilename.length())) == 0) {
					relatedtxtid = SteamTextFile[j].id;
					break;
				}
			}
			if (relatedtxtid >= 0 && textset)
				for (j = 0; j < textset->amount; j++)
					if (textset->struct_id[j] == relatedtxtid) {
						related_text[i] = textset->text_data[j];
						break;
					}
			script_data[i] = new ScriptDataStruct[1];
			script_data[i]->related_charmap_id = 0;
			script_data[i]->Init(true, CHUNK_TYPE_SCRIPT, config.field_id[i], &dummyclus[i], CLUSTER_TYPE_FIELD);
			script_data[i]->is_field_script = true;
			for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
				if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
					continue;
				ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_script_file[lang][i]));
				script_data[i]->Read(ffbin, lang);
			}
			script_data[i]->size = config.meta_script.GetFileSizeByIndex(config.field_script_file[GetSteamLanguage()][i]);
			for (j = 0; j < amount; j++)
				if (fieldnameid[j] == struct_id[i]) {
					script_data[i]->name = fieldname[j];
					break;
				}
			ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_role_file[i]));
			role[i] = new FieldRoleDataStruct[1];
			role[i]->Init(false, CHUNK_TYPE_FIELD_ROLE, config.field_id[i], &dummyclus[i]);
			role[i]->size = config.meta_script.GetFileSizeByIndex(config.field_role_file[i]);
			role[i]->Read(ffbin);
			if (config.field_preload_file[i] >= 0) {
				ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_preload_file[i]));
				preload[i] = new ImageMapDataStruct[1];
				preload[i]->Init(false, CHUNK_TYPE_IMAGE_MAP, config.field_id[i], &dummyclus[i]);
				preload[i]->size = config.meta_script.GetFileSizeByIndex(config.field_preload_file[i]);
				preload[i]->Read(ffbin);
			} else {
				preload[i] = NULL;
			}
			LoadingDialogUpdate(i, wxString::Format(wxT("%u / %u (1/2)"), i, amount));
		}
		ffbin.close();
		delete[] fieldname;
		delete[] fieldnameid;
		for (i = 0; i < amount; i++) {
			if (config.field_file_id[i] == 0) {
				walkmesh[i] = NULL;
				background_data[i] = NULL;
				tim_data[i] = NULL;
				continue;
			}
			stringstream fnamestream;
			fnamestream << config.steam_dir_assets << "p0data1" << (unsigned int)config.field_file_id[i] << ".bin";
			fname = fnamestream.str();
			ffbin.open(fname.c_str(), ios::in | ios::binary);
			int sharedwalkfile = -1;
			int sharedtimfile = -1;
			int sharedtilefile = -1;
			for (j = 0; j < i; j++)
				if (config.field_file_id[i] == config.field_file_id[j]) {
					if (sharedwalkfile < 0 && config.field_walkmesh_file[i] == config.field_walkmesh_file[j])
						sharedwalkfile = j;
					if (sharedtimfile < 0 && config.field_image_file[i] == config.field_image_file[j])
						sharedtimfile = j;
					if (sharedtilefile < 0 && config.field_tiles_file[i][0] == config.field_tiles_file[j][0])
						sharedtilefile = j;
				}
			if (sharedwalkfile >= 0) {
				walkmesh[i] = walkmesh[sharedwalkfile];
			} else {
				ffbin.seekg(config.meta_field[config.field_file_id[i] - 1].GetFileOffsetByIndex(config.field_walkmesh_file[i]));
				walkmesh[i] = new FieldWalkmeshDataStruct[1];
				walkmesh[i]->Init(false, CHUNK_TYPE_FIELD_WALK, config.field_id[i], &dummyclus[i]);
				walkmesh[i]->size = config.meta_field[config.field_file_id[i] - 1].GetFileSizeByIndex(config.field_walkmesh_file[i]);
				walkmesh[i]->Read(ffbin);
			}
			if (sharedtimfile >= 0) {
				tim_data[i] = tim_data[sharedtimfile];
			} else {
				//ffbin.seekg(config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_image_file[i]));
				tim_data[i] = new TIMImageDataStruct[1];
				tim_data[i]->Init(false, CHUNK_TYPE_TIM, config.field_id[i], &dummyclus[i]);
				tim_data[i]->data_file_name = fname;
				tim_data[i]->data_file_offset = config.meta_field[config.field_file_id[i] - 1].GetFileOffsetByIndex(config.field_image_file[i]);
				//tim_data[i]->Read(ffbin);
			}
			if (sharedtilefile >= 0) {
				background_data[i] = background_data[sharedtilefile];
				ffbin.close();
			} else {
				// Whichever language, the US tilesets are used ; the others are only there for the title tiles 
				ffbin.seekg(config.meta_field[config.field_file_id[i] - 1].GetFileOffsetByIndex(config.field_tiles_file[i][0]));
				background_data[i] = new FieldTilesDataStruct[1];
				background_data[i]->Init(false, CHUNK_TYPE_FIELD_TILES, config.field_id[i], &dummyclus[i]);
				background_data[i]->parent = this;
				background_data[i]->id = i;
				unsigned int titletileamount = 0;
				if (config.field_tiles_localized[i])
					for (j = 0; j < title_info->amount; j++)
						if (title_info->field_id[j] == config.field_id[i]) {
							titletileamount = title_info->title_tile_last[j] - title_info->title_tile_start[j] + 1;
							break;
						}
				background_data[i]->size = config.meta_field[config.field_file_id[i] - 1].GetFileSizeByIndex(config.field_tiles_file[i][0]);
				background_data[i]->Read(ffbin, titletileamount);
				ffbin.close();
				if (config.field_tiles_localized[i])
					title_info->ReadTitleTileId(background_data[i], config);
			}
			addition[i] = NULL;
			LoadingDialogUpdate(i, wxString::Format(wxT("%u / %u (2/2)"), i, amount));
		}
		delete[] dummyclus;
	}
	LoadingDialogEnd();
}

int FieldDataSet::GetSteamTextSize(SteamLanguage lang) {
	int res = 0;
	for (unsigned int i = 0; i < amount; i++) {
		string namedefstream = to_string(GetIdByIndex(i)) + ":";
		res += namedefstream.length() + script_data[i]->name.GetLength(lang) + 2;
	}
	return res;
}

void FieldDataSet::WriteSteamText(fstream& ffbin, SteamLanguage lang) {
	for (unsigned int i = 0; i < amount; i++) {
		string namedefstream = to_string(GetIdByIndex(i)) + ":";
		ffbin.write(namedefstream.c_str(), namedefstream.length());
		SteamWriteFF9String(ffbin, script_data[i]->name, lang);
		WriteShort(ffbin, 0x0A0D);
	}
}

void FieldDataSet::WriteSteamTextPatch(fstream& fileout, fstream& baseresourcefile, ConfigurationSet& configset, SteamLanguage lang) {
	baseresourcefile.seekg(configset.meta_res.GetFileOffsetByIndex(configset.field_text_file[lang]));
	uint32_t baselength = configset.meta_res.GetFileSizeByIndex(configset.field_text_file[lang]);
	map<int, wxString> basenames;
	wxString fieldidtxt = _(L"");
	long fieldid = -1;
	unsigned int i;
	char c;
	for (i = 0; i < baselength; i++) {
		c = baseresourcefile.get();
		if (c == '\r' || c == '\n') {
			fieldidtxt = _(L"");
		} else if (c == ':') {
			FF9String dumpedstr;
			SteamReadFF9String(baseresourcefile, dumpedstr);
			if (fieldidtxt.ToLong(&fieldid))
				basenames[fieldid] = _(dumpedstr.str);
			fieldidtxt = _(L"");
		} else {
			fieldidtxt += c;
		}
	}
	for (i = 0; i < amount; i++) {
		if (!script_data[i]->name.multi_lang_init[lang])
			continue;
		wxString str = (lang == GetSteamLanguage() ? _(script_data[i]->name.str) : _(script_data[i]->name.multi_lang_str[lang]));
		if (auto search = basenames.find(GetIdByIndex(i)); search != basenames.end())
			if (search->second.IsSameAs(str))
				continue;
		string namedefstream = to_string(GetIdByIndex(i)) + ":";
		fileout.write(namedefstream.c_str(), namedefstream.length());
		SteamWriteFF9String(fileout, script_data[i]->name, lang);
		WriteShort(fileout, 0x0A0D);
	}
}

int FieldDataSet::GetIndexById(int fieldid) {
	for (unsigned int i = 0; i < amount; i++)
		if (fieldid == GetIdByIndex(i))
			return i;
	return -1;
}

int FieldDataSet::GetIdByIndex(int fieldindex) {
	return struct_id[fieldindex];
}

void FieldDataSet::Write(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i = 0; i < amount; i++) {
		ClusterData& clus = clusset.clus[cluster_id[i]];
		ffbin.seekg(clus.offset);
		clus.Write(ffbin);
	}
}

void FieldDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i = 0; i < amount; i++) {
		ClusterData& clus = clusset.clus[cluster_id[i]];
		ffbin.seekg(clus.offset);
		clus.WritePPF(ffbin);
	}
}

int* FieldDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag) {
	unsigned int i, j;
	uint32_t chunksize, clustersize, chunkpos, objectpos, objectsize;
	uint16_t nbmodified;
	int objindex, objectid;
	SteamLanguage lang, sublang;
	uint8_t langcount;
	bool shouldread;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[5];
	res[0] = 0; res[1] = 0; res[2] = 0; res[3] = 0; res[4] = 0;
	bool loadmain = localflag & 1;
	bool loadlocal = localflag & 2;
	HWSReadShort(ffhws, nbmodified);
	for (i = 0; i < nbmodified; i++) {
		objectpos = ffhws.tellg();
		HWSReadFlexibleShort(ffhws, objectid, GetHWSGlobalVersion() >= 102);
		HWSReadLong(ffhws, clustersize);
	read_start:
		objindex = GetIndexById(objectid);
		if (objindex >= 0) {
			clus = script_data[objindex]->parent_cluster;
			if (clustersize <= clus->size + clus->extra_size) {
				HWSReadChar(ffhws, chunktype);
				while (chunktype != CHUNK_SPECIAL_END) {
					HWSReadLong(ffhws, chunksize);
					chunkpos = ffhws.tellg();
					if (chunktype == CHUNK_TYPE_SCRIPT) {
						if (loadmain) {
							script_data[objindex]->ReadHWS(ffhws, usetext && GetGameType() == GAME_TYPE_PSX);
							script_data[objindex]->SetSize(chunksize);
						}
					} else if (chunktype == CHUNK_TYPE_FIELD_TILES && background_data[objindex]) {
						if (loadmain) {
							background_data[objindex]->ReadHWS(ffhws);
							background_data[objindex]->SetSize(chunksize);
						}
					} else if (chunktype == CHUNK_TYPE_FIELD_WALK && walkmesh[objindex]) {
						if (loadmain) {
							walkmesh[objindex]->ReadHWS(ffhws);
							walkmesh[objindex]->SetSize(chunksize);
						}
					} else if (chunktype == CHUNK_TYPE_TIM && tim_data[objindex]) {
						if (loadmain) {
							uint16_t timid;
							HWSReadShort(ffhws, timid);
							for (j = 0; j < tim_data[objindex]->parent_chunk->object_amount; j++)
								if (tim_data[objindex][j].object_id == timid) {
									tim_data[objindex][j].ReadHWS(ffhws);
									tim_data[objindex][j].SetSize(chunksize - 2);
								}
						}
					} else if (chunktype == CHUNK_TYPE_FIELD_ROLE) {
						if (loadmain) {
							role[objindex]->ReadHWS(ffhws);
							role[objindex]->SetSize(chunksize);
						}
					} else if (chunktype == CHUNK_TYPE_IMAGE_MAP && preload[objindex]) {
						if (loadmain) {
							uint32_t parentclussize;
							HWSReadLong(ffhws, parentclussize);
							if (GetHWSGameType() != GetGameType()) {
								res[4]++;
							} else if (parentclussize <= preload[objindex]->parent_cluster->size + preload[objindex]->parent_cluster->extra_size) {
								if (GetHWSGameType() == GAME_TYPE_PSX)
									preload[objindex]->SetSize(chunksize - 4);
								else
									preload[objindex]->SetSize(chunksize);
								preload[objindex]->ReadHWS(ffhws);
							} else
								res[3]++;
						}
					} else if (chunktype == CHUNK_STEAM_FIELD_NAME || chunktype == CHUNK_STEAM_FIELD_MULTINAME) {
						if (loadmain && usetext) {
							if (chunktype == CHUNK_STEAM_FIELD_NAME)
								lang = GetSteamLanguage();
							else
								HWSReadChar(ffhws, lang);
							while (lang != STEAM_LANGUAGE_NONE) {
								FF9String locname;
								SteamReadFF9String(ffhws, locname);
								if (GetGameType() == GAME_TYPE_PSX)
									locname.SteamToPSX();
								SetFieldName(j, locname.str, lang);
								if (chunktype == CHUNK_STEAM_FIELD_NAME)
									lang = STEAM_LANGUAGE_NONE;
								else
									HWSReadChar(ffhws, lang);
							}
						}
					} else if (chunktype == CHUNK_SPECIAL_TYPE_LOCAL) {
						if (loadlocal)
							script_data[objindex]->ReadLocalHWS(ffhws);
					} else if (chunktype == CHUNK_STEAM_SCRIPT_MULTILANG) {
						if (loadmain) {
							HWSReadChar(ffhws, lang);
							while (lang != STEAM_LANGUAGE_NONE) {
								ScriptLanguageLink langlink;
								uint32_t langdatasize;
								shouldread = langlink.InitFromHWS(ffhws, lang);
								HWSReadLong(ffhws, langdatasize);
								if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage()) {
									if (shouldread)
										script_data[objindex]->ReadHWS(ffhws, false, lang, &langlink);
									else
										ffhws.seekg(langdatasize, ios::cur);
								} else {
									script_data[objindex]->ReadHWS(ffhws, false, lang, &langlink);
								}
								HWSReadChar(ffhws, lang);
							}
						}
					} else if (chunktype == CHUNK_STEAM_SCRIPT_MERGED) {
						if (loadmain) {
							script_data[objindex]->ReadHWS(ffhws, usetext, STEAM_LANGUAGE_AMOUNT);
							script_data[objindex]->SetSize(chunksize);
						}
					} else if (chunktype == CHUNK_SPECIAL_TYPE_LOCAL_MULTILANG) {
						if (loadlocal) {
							HWSReadChar(ffhws, lang);
							while (lang != STEAM_LANGUAGE_NONE) {
								uint32_t langdatasize;
								shouldread = false;
								HWSReadChar(ffhws, langcount);
								for (j = 0; j < langcount; j++) {
									HWSReadChar(ffhws, sublang);
									if (sublang == GetSteamLanguage())
										shouldread = true;
								}
								HWSReadLong(ffhws, langdatasize);
								if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage()) {
									if (shouldread)
										script_data[objindex]->ReadLocalHWS(ffhws);
									else
										ffhws.seekg(langdatasize, ios::cur);
								} else {
									script_data[objindex]->ReadLocalHWS(ffhws);
								}
								HWSReadChar(ffhws, lang);
							}
						}
					} else if (chunktype == CHUNK_SPECIAL_TYPE_FIELD_ADDITION) {
						if (loadmain) {
							addition[objindex]->ReadHWS(ffhws);
							related_text[objindex] = NULL;
							for (j = 0; j < related_text.size(); j++) {
								if (j != objindex && related_text[j] != NULL && related_text[j]->block_id == addition[objindex]->text_block_id) {
									related_text[objindex] = related_text[j];
									break;
								}
							}
						}
					} else
						res[1]++;
					ffhws.seekg(chunkpos + chunksize);
					HWSReadChar(ffhws, chunktype);
				}
			} else {
				objectsize = 7;
				HWSReadChar(ffhws, chunktype);
				while (chunktype != CHUNK_SPECIAL_END) {
					HWSReadLong(ffhws, chunksize);
					ffhws.seekg(chunksize, ios::cur);
					HWSReadChar(ffhws, chunktype);
					objectsize += chunksize + 5;
				}
				ffhws.seekg(objectpos);
				backup.Add(ffhws, objectsize);
				res[0]++;
			}
		} else {
			int basefieldid = -1;
			if (GetGameType() == GAME_TYPE_STEAM && GetGameConfiguration() != NULL && GetGameConfiguration()->dll_usage != 0) {
				chunkpos = ffhws.tellg();
				HWSReadChar(ffhws, chunktype);
				while (chunktype != CHUNK_SPECIAL_END && chunktype != CHUNK_SPECIAL_TYPE_FIELD_ADDITION) {
					HWSReadLong(ffhws, chunksize);
					ffhws.seekg(chunksize, ios::cur);
					HWSReadChar(ffhws, chunktype);
				}
				if (chunktype == CHUNK_SPECIAL_TYPE_FIELD_ADDITION) {
					HWSReadLong(ffhws, chunksize);
					HWSReadFlexibleShort(ffhws, basefieldid, true);
					if (!CreateCustomField(*GetGameConfiguration(), GetIndexById(basefieldid), objectid))
						basefieldid = -1;
				}
				ffhws.seekg(chunkpos);
			}
			if (basefieldid >= 0) {
				goto read_start;
			} else {
				objectsize = 7;
				HWSReadChar(ffhws, chunktype);
				while (chunktype != CHUNK_SPECIAL_END) {
					HWSReadLong(ffhws, chunksize);
					ffhws.seekg(chunksize, ios::cur);
					HWSReadChar(ffhws, chunktype);
					objectsize += chunksize + 5;
				}
				ffhws.seekg(objectpos);
				backup.Add(ffhws, objectsize);
				res[2]++;
			}
		}
	}
	return res;
}

void FieldDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag) {
	unsigned int i, j;
	uint16_t nbmodified = 0;
	uint32_t chunksize, chunkpos, nboffset = ffhws.tellg();
	ClusterData* clus;
	bool savemain = localflag & 1;
	bool savelocal = localflag & 2;
	HWSWriteShort(ffhws, nbmodified);
	for (i = 0; i < amount; i++) {
		clus = script_data[i]->parent_cluster;
		if (clus->modified || (clus->parent_cluster && clus->parent_cluster->modified)) {
			clus->UpdateOffset();
			HWSWriteFlexibleShort(ffhws, GetIdByIndex(i), true);
			HWSWriteLong(ffhws, clus->size);
			if (savemain && addition[i] != NULL) {
				HWSWriteChar(ffhws, CHUNK_SPECIAL_TYPE_FIELD_ADDITION);
				HWSWriteLong(ffhws, 0);
				chunkpos = ffhws.tellg();
				addition[i]->WriteHWS(ffhws);
				chunksize = (long long)ffhws.tellg() - chunkpos;
				ffhws.seekg(chunkpos - 4);
				HWSWriteLong(ffhws, chunksize);
				ffhws.seekg(chunkpos + chunksize);
			}
			if (background_data[i] && background_data[i]->modified && savemain) {
				HWSWriteChar(ffhws, CHUNK_TYPE_FIELD_TILES);
				HWSWriteLong(ffhws, background_data[i]->size);
				chunkpos = ffhws.tellg();
				background_data[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos + background_data[i]->size);
			}
			if (walkmesh[i] && walkmesh[i]->modified && savemain) {
				walkmesh[i]->UpdateOffset();
				HWSWriteChar(ffhws, CHUNK_TYPE_FIELD_WALK);
				HWSWriteLong(ffhws, walkmesh[i]->size);
				chunkpos = ffhws.tellg();
				walkmesh[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos + walkmesh[i]->size);
			}
			if (role[i]->modified && savemain) {
				HWSWriteChar(ffhws, CHUNK_TYPE_FIELD_ROLE);
				HWSWriteLong(ffhws, role[i]->size);
				chunkpos = ffhws.tellg();
				role[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos + role[i]->size);
			}
			if (preload[i] && preload[i]->modified && savemain) {
				preload[i]->parent_cluster->UpdateOffset();
				HWSWriteChar(ffhws, CHUNK_TYPE_IMAGE_MAP);
				HWSWriteLong(ffhws, preload[i]->size + 4);
				chunkpos = ffhws.tellg();
				HWSWriteLong(ffhws, preload[i]->parent_cluster->size);
				preload[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos + preload[i]->size + 4);
			}
			if (savelocal) {
				uint32_t localsize = 0;
				HWSWriteChar(ffhws, CHUNK_SPECIAL_TYPE_LOCAL);
				HWSWriteLong(ffhws, localsize);
				chunkpos = ffhws.tellg();
				script_data[i]->WriteLocalHWS(ffhws);
				localsize = (long long)ffhws.tellg() - chunkpos;
				ffhws.seekg(chunkpos - 4);
				HWSWriteLong(ffhws, localsize);
				ffhws.seekg(chunkpos + localsize);
			}
			if (GetGameType() == GAME_TYPE_PSX) {
				for (j = 0; j < tim_data[i]->parent_chunk->object_amount; j++)
					if (tim_data[i][j].modified && savemain) {
						HWSWriteChar(ffhws, CHUNK_TYPE_TIM);
						HWSWriteLong(ffhws, tim_data[i][j].size + 2);
						chunkpos = ffhws.tellg();
						HWSWriteShort(ffhws, tim_data[i][j].object_id);
						tim_data[i][j].WriteHWS(ffhws);
						ffhws.seekg(chunkpos + tim_data[i][j].size + 2);
					}
				if (script_data[i]->modified && savemain) {
					HWSWriteChar(ffhws, CHUNK_TYPE_SCRIPT);
					HWSWriteLong(ffhws, script_data[i]->size);
					chunkpos = ffhws.tellg();
					script_data[i]->WriteHWS(ffhws, false);
					ffhws.seekg(chunkpos + script_data[i]->size);
				}
			} else {
				if (savemain && script_data[i]->IsDataModified()) {
					HWSWriteChar(ffhws, CHUNK_STEAM_SCRIPT_MERGED);
					HWSWriteLong(ffhws, 0);
					chunkpos = ffhws.tellg();
					script_data[i]->WriteHWS(ffhws, true);
					chunksize = (long long)ffhws.tellg() - chunkpos;
					ffhws.seekg(chunkpos - 4);
					HWSWriteLong(ffhws, chunksize);
					ffhws.seekg(chunkpos + chunksize);
				}
			}
			HWSWriteChar(ffhws, CHUNK_SPECIAL_END);
			nbmodified++;
		}
	}
	for (i = 0; i < backup.save_amount; i++)
		for (j = 0; j < backup.save_size[i]; j++)
			HWSWriteChar(ffhws, backup.save_data[i][j]);
	nbmodified += backup.save_amount;
	uint32_t endoffset = ffhws.tellg();
	ffhws.seekg(nboffset);
	HWSWriteShort(ffhws, nbmodified);
	ffhws.seekg(endoffset);
}
