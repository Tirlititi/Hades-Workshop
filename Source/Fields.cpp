#include "Fields.h"

#include <sstream>
#include "Steam_Strings.h"
#include "Gui_LoadingDialog.h"
#include "TIMImages.h"
#include "main.h"

#define TILE_SIZE		16
#define TILE_SIZE_STEAM 32

void FieldTilesTileDataStruct::AllocTileData() {
	tile_data_data1 = new uint32_t[tile_amount];
	tile_data_data2 = new uint32_t[tile_amount];
	tile_data_data3 = new uint32_t[tile_amount];
	tile_depth = new uint16_t[tile_amount];
	tile_pos_y = new uint16_t[tile_amount];
	tile_pos_x = new uint16_t[tile_amount];
	tile_clut_y = new uint16_t[tile_amount];
	tile_clut_x = new uint8_t[tile_amount];
	tile_page_y = new uint8_t[tile_amount];
	tile_page_x = new uint8_t[tile_amount];
	tile_tp = new uint8_t[tile_amount];
	tile_alpha = new uint8_t[tile_amount];
	tile_source_v = new uint8_t[tile_amount];
	tile_source_u = new uint8_t[tile_amount];
	tile_h = new uint16_t[tile_amount];
	tile_w = new uint16_t[tile_amount];
	tile_abr = new uint8_t[tile_amount];
	tile_trans = new bool[tile_amount];
	tile_steam_id = new unsigned int[tile_amount];
}

void FieldTilesCameraDataStruct::UpdateSize() {
	unsigned int i,j;
	int maxx = -0xFFFF;
	int maxy = -0xFFFF;
	pos_x = 0xFFFF;
	pos_y = 0xFFFF;
	for (i=0;i<parent->tiles_amount;i++)
		if (parent->tiles[i].camera_id==id)
			for (j=0;j<parent->tiles[i].tile_amount;j++) {
				pos_x = min(pos_x,parent->tiles[i].pos_x+parent->tiles[i].tile_pos_x[j]);
				pos_y = min(pos_y,parent->tiles[i].pos_y+parent->tiles[i].tile_pos_y[j]);
				maxx = max(maxx,parent->tiles[i].pos_x+parent->tiles[i].tile_pos_x[j]+TILE_SIZE);
				maxy = max(maxy,parent->tiles[i].pos_y+parent->tiles[i].tile_pos_y[j]+TILE_SIZE);
			}
	width = pos_x<0xFFFF ? maxx-pos_x : 0;
	height = pos_y<0xFFFF ? maxy-pos_y : 0;
	if (GetGameType()!=GAME_TYPE_PSX) {
		width *= 2;
		height *= 2;
	}
}

void FieldTilesDataStruct::Copy(FieldTilesDataStruct& cpy) {
	unsigned int i,j;
	*this = cpy;
	anim = new FieldTilesAnimDataStruct[anim_amount];
	tiles = new FieldTilesTileDataStruct[tiles_amount];
	light = new FieldTilesLightDataStruct[light_amount];
	camera = new FieldTilesCameraDataStruct[camera_amount];
	tiles_sorted = new FieldTilesTileDataStruct*[tiles_amount];
	for (i=0;i<anim_amount;i++) {
		anim[i] = cpy.anim[i];
		anim[i].tile_list = new uint8_t[anim[i].tile_amount];
		anim[i].tile_duration = new uint8_t[anim[i].tile_amount];
		for (j=0;j<anim[i].tile_amount;j++) {
			anim[i].tile_list[j] = cpy.anim[i].tile_list[j];
			anim[i].tile_duration[j] = cpy.anim[i].tile_duration[j];
		}
	}
	for (i=0;i<tiles_amount;i++) {
		tiles[i] = cpy.tiles[i];
		tiles[i].AllocTileData();
		for (j=0;j<tiles[i].tile_amount;j++) {
			tiles[i].tile_data_data1[j] = cpy.tiles[i].tile_data_data1[j];
			tiles[i].tile_data_data2[j] = cpy.tiles[i].tile_data_data2[j];
			tiles[i].tile_data_data3[j] = cpy.tiles[i].tile_data_data3[j];
			tiles[i].tile_steam_id[j] = cpy.tiles[i].tile_steam_id[j];
		}
	}
	for (i=0;i<light_amount;i++) {
		light[i] = cpy.light[i];
	}
	for (i=0;i<camera_amount;i++) {
		camera[i] = cpy.camera[i];
	}
	SetupDataInfos(true);
}

void FieldTilesDataStruct::AddTilesetToImage(uint32_t* imgdest, FieldTilesTileDataStruct& t, bool showtp, uint32_t* steamimg, uint32_t steamimgwidth) {
	unsigned int i,x,y,pixelx,pixely,timtilex,timtiley,tilesize,steamfactor;
	uint32_t pix,alpha;
	TIM_BlendMode bm;
	bool psx = GetGameType()==GAME_TYPE_PSX;
	tilesize = psx ? TILE_SIZE : TILE_SIZE_STEAM;
	for (i=0;i<t.tile_amount;i++) {
		pixely = t.pos_y+t.tile_pos_y[i]-camera[t.camera_id].pos_y;
		if (psx) {
			timtiley = t.tile_page_y[i]*256 + t.tile_source_v[i];
		} else {
			timtiley = (t.tile_steam_id[i]/(steamimgwidth/(TILE_SIZE_STEAM+4)))*(TILE_SIZE_STEAM+4)+2;
			pixely *= 2;
		}
		switch (t.tile_abr[i]) {
		case 0:
			alpha = t.tile_trans[i] ? 0x80000000 : 0xFF000000;
			bm = TIM_BLENDMODE_ALPHA;
			break;
		case 1:
			alpha = t.tile_trans[i] ? 0x80000000 : 0xFF000000;
			bm = TIM_BLENDMODE_LIGHT;
			break;
		case 2:
			alpha = 0x1A000000;
			bm = TIM_BLENDMODE_SHADE;
			break;
		case 3:
			alpha = 0x80000000;
			bm = TIM_BLENDMODE_LIGHT;
			break;
		}
		for (y=0;y<tilesize;y++) {
			pixelx = t.pos_x+t.tile_pos_x[i]-camera[t.camera_id].pos_x;
			if (psx) {
				timtilex = t.tile_tp[i]>0 ? t.tile_page_x[i]*128 + t.tile_source_u[i] : t.tile_page_x[i]*128*2 + t.tile_source_u[i];
			} else {
				timtilex = (t.tile_steam_id[i]%(steamimgwidth/(TILE_SIZE_STEAM+4)))*(TILE_SIZE_STEAM+4)+2;
				pixelx *= 2;
			}
			for (x=0;x<tilesize;x++) {
				if (t.tile_tp[i]>0) {
					if (psx)
						pix = TIMImageDataStruct::GetVRamPixel(timtilex,timtiley,t.tile_clut_x[i],t.tile_clut_y[i],false);
					else
						pix = steamimg[timtilex+timtiley*steamimgwidth];
					if (pix & 0xFF000000)
						imgdest[pixelx+pixely*camera[t.camera_id].width] = ImageMergePixels(imgdest[pixelx+pixely*camera[t.camera_id].width],(pix & 0xFFFFFF) | alpha,bm);
				} else {
					if (psx)
						pix = TIMImageDataStruct::GetVRamPixel(timtilex,timtiley,t.tile_clut_x[i],t.tile_clut_y[i],true);
					else
						pix = steamimg[timtilex+timtiley*steamimgwidth];
					if (showtp && (pix & 0xFF000000) && (pix!=0xFF000000))
						imgdest[pixelx+pixely*camera[t.camera_id].width] = ImageMergePixels(imgdest[pixelx+pixely*camera[t.camera_id].width],(pix & 0xFFFFFF) | alpha,bm);
				}
				timtilex++;
				pixelx++;
			}
			timtiley++;
			pixely++;
		}
	}
}

uint32_t* FieldTilesDataStruct::ConvertAsImage(unsigned int cameraid, bool tileflag[], bool showtp) {
	unsigned int i,imgsize = camera[cameraid].width*camera[cameraid].height;
	TIMImageDataStruct* tim = parent->tim_data[id];
	uint32_t* rawimg = NULL;
	uint32_t* res = new uint32_t[imgsize];
	if (GetGameType()==GAME_TYPE_PSX)
		tim->LoadInVRam();
	else
		rawimg = tim->ConvertAsSteamImage();
	for (i=0;i<imgsize;i++)
		res[i] = 0;
	for (i=0;i<tiles_amount;i++) {
		FieldTilesTileDataStruct& t = *tiles_sorted[i];
		if (t.camera_id==cameraid && (tileflag==NULL || tileflag[t.id]) && (tileflag!=NULL || t.is_static || t.is_first_of_anim))
			AddTilesetToImage(res,t,showtp,rawimg,tim->steam_width);
	}
	return res;
}

int FieldTilesDataStruct::Export(const char* outputfile, unsigned int cameraid, bool tileflag[], bool showtp, bool mergetiles) {
	fstream ftiff(outputfile,ios::out|ios::binary);
	if (!ftiff.is_open())
		return 1;
	TIMImageDataStruct* tim = parent->tim_data[id];
	unsigned int i,j,x,y,width,height;
	width = camera[cameraid].width;
	height = camera[cameraid].height;
	uint32_t* img = new uint32_t[width*height];
	uint32_t* rawimg = NULL;
	if (GetGameType()==GAME_TYPE_PSX)
		tim->LoadInVRam();
	else
		rawimg = tim->ConvertAsSteamImage();
	
	uint16_t tmp16;
	uint32_t tmp32;
	#define MACRO_WS(VAL) \
		tmp16 = VAL; \
		ftiff.write((const char*)&tmp16,2);
	#define MACRO_WL(VAL) \
		tmp32 = VAL; \
		ftiff.write((const char*)&tmp32,4);
	#define MACRO_WC(VAL) \
		tmp32 = VAL; \
		ftiff.write((const char*)&tmp32+2,1); \
		ftiff.write((const char*)&tmp32+1,1); \
		ftiff.write((const char*)&tmp32,1); \
		ftiff.write((const char*)&tmp32+3,1);
	
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
		MACRO_WS(IFD_INFO_AMOUNT)
		MACRO_WS(0x100)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width) // Width
		MACRO_WS(0x101)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(height) // Length
		MACRO_WS(0x102)	MACRO_WS(1)	MACRO_WL(4)	MACRO_WL(0x08080808) // Bits per sample
		MACRO_WS(0x103)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Compression
		MACRO_WS(0x106)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(2) // Photometric interpretation
		MACRO_WS(0x111)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(ifdoff+0x6+IFD_INFO_AMOUNT*0x0C) // Strip Offsets
		MACRO_WS(0x115)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(4) // Sample per pixel
		MACRO_WS(0x116)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(0xFFFFFFFF) // Rows per strip
		MACRO_WS(0x117)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width*height*4) // Strip byte counts
		MACRO_WS(0x11A)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution X
		MACRO_WS(0x11B)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution Y
		MACRO_WS(0x128)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Resolution Unit
		MACRO_WS(0x152)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Extra sample (4th byte is alpha)
		MACRO_WL(0) // End of IFDs
		for (j=0;j<width*height;j++)
			img[j] = 0;
		for (i=0;i<tiles_amount;i++) {
			FieldTilesTileDataStruct& t = *tiles_sorted[i];
			if (cameraid==t.camera_id && (tileflag==NULL || tileflag[t.id]))
				AddTilesetToImage(img,t,true,rawimg,tim->steam_width);
		}
		for (y=0;y<height;y++)
			for (x=0;x<width;x++) {
				MACRO_WC(img[y*width+x])
			}
	} else {
		uint32_t lastifdp = 0x4;
		for (i=0;i<tiles_amount;i++) {
			FieldTilesTileDataStruct& t = *tiles_sorted[i];
			if (cameraid==t.camera_id && (tileflag==NULL || tileflag[t.id])) {
				MACRO_WS(IFD_INFO_AMOUNT)
				MACRO_WS(0x100)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width) // Width
				MACRO_WS(0x101)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(height) // Length
				MACRO_WS(0x102)	MACRO_WS(1)	MACRO_WL(4)	MACRO_WL(0x08080808) // Bits per sample
				MACRO_WS(0x103)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Compression
				MACRO_WS(0x106)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(2) // Photometric interpretation
				MACRO_WS(0x111)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(ifdoff+0x6+IFD_INFO_AMOUNT*0x0C) // Strip Offsets
				MACRO_WS(0x115)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(4) // Sample per pixel
				MACRO_WS(0x116)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(0xFFFFFFFF) // Rows per strip
				MACRO_WS(0x117)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(width*height*4) // Strip byte counts
				MACRO_WS(0x11A)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution X
				MACRO_WS(0x11B)	MACRO_WS(5)	MACRO_WL(1)	MACRO_WL(0x8) // Resolution Y
				MACRO_WS(0x128)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Resolution Unit
				MACRO_WS(0x152)	MACRO_WS(4)	MACRO_WL(1)	MACRO_WL(1) // Extra sample (4th byte is alpha)
				ifdoff += 0x6+IFD_INFO_AMOUNT*0x0C+width*height*4;
				lastifdp = ftiff.tellg();
				MACRO_WL(ifdoff)
				for (j=0;j<width*height;j++)
					img[j] = 0;
				AddTilesetToImage(img,t,true,rawimg,tim->steam_width);
				for (y=0;y<height;y++)
					for (x=0;x<width;x++) {
						MACRO_WC(img[y*width+x])
					}
			}
		}
		ftiff.seekg(lastifdp);
		MACRO_WL(0)
	}
	ftiff.close();
	return 0;
}

void FieldTilesDataStruct::SetupDataInfos(bool readway) {
	unsigned int i,j;
	if (readway) {
		for (i=0;i<tiles_amount;i++) {
			tiles[i].is_screen_static = tiles[i].data1 & 0x1;
			tiles[i].use_attaching = (tiles[i].data1 >> 1) & 0x1;
			tiles[i].is_looping = (tiles[i].data1 >> 2) & 0x1;
			tiles[i].has_parallax = (tiles[i].data1 >> 3) & 0x1;
			tiles[i].is_static = (tiles[i].data1 >> 4) & 0x1;
			tiles[i].is_scroll_with_offset = (tiles[i].data1 >> 7) & 0x1;
			tiles[i].distance = (tiles[i].data1 >> 8) & 0xFFF;
			tiles[i].default_distance = (tiles[i].data1 >> 20) & 0xFFF;
			tiles[i].is_x_offset = tiles[i].data2 & 0x1;
			tiles[i].viewport_id = tiles[i].data2 >> 1;
			for (j=0;j<tiles[i].tile_amount;j++) {
				tiles[i].tile_clut_y[j] = tiles[i].tile_data_data1[j] & 0x1FF;
				tiles[i].tile_clut_x[j] = (tiles[i].tile_data_data1[j] >> 9) & 0x3F;
				tiles[i].tile_page_y[j] = (tiles[i].tile_data_data1[j] >> 15) & 0x1;
				tiles[i].tile_page_x[j] = (tiles[i].tile_data_data1[j] >> 16) & 0xF;
				tiles[i].tile_tp[j] = (tiles[i].tile_data_data1[j] >> 20) & 0x3;
				tiles[i].tile_alpha[j] = (tiles[i].tile_data_data1[j] >> 22) & 0x3;
				tiles[i].tile_source_v[j] = (tiles[i].tile_data_data1[j] >> 24) & 0xFF;
				tiles[i].tile_source_u[j] = tiles[i].tile_data_data2[j] & 0xFF;
				tiles[i].tile_h[j] = (tiles[i].tile_data_data1[j] >> 8) & 0x3FF;
				tiles[i].tile_w[j] = (tiles[i].tile_data_data1[j] >> 18) & 0x3FF;
				tiles[i].tile_trans[j] = (tiles[i].tile_data_data2[j] >> 28) & 0x1;
				tiles[i].tile_depth[j] = tiles[i].tile_data_data3[j] & 0xFFF;
				tiles[i].tile_pos_y[j] = (tiles[i].tile_data_data3[j] >> 12) & 0x3FF;
				tiles[i].tile_pos_x[j] = (tiles[i].tile_data_data3[j] >> 22) & 0x3FF;
				// Unused 3 bits
				tiles[i].tile_abr[j] = tiles[i].tile_trans[j] ? 1 : 0;
			}
		}
		FieldTilesTileDataStruct* tmp;
		for (i=0;i<tiles_amount;i++)
			tiles_sorted[i] = &tiles[i];
		for (i=0;i<tiles_amount;i++)
			for (j=i+1;j<tiles_amount;j++)
				if ((tiles_sorted[j]->distance)>=(tiles_sorted[i]->distance)) {
					tmp = tiles_sorted[j];
					tiles_sorted[j] = tiles_sorted[i];
					tiles_sorted[i] = tmp;
				}
		for (i=0;i<anim_amount;i++)
			if (anim[i].tile_amount>0)
				tiles[anim[i].tile_list[0]].is_first_of_anim = true;
	} else {
		// ToDo
	}
}

#define MACRO_TILES_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j,k; \
	if (!READ) SetupDataInfos(false); \
	uint32_t headerpos = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,tiles_size); \
	IO ## Short(f,depth_shift); \
	IO ## Short(f,anim_amount); \
	IO ## Short(f,tiles_amount); \
	IO ## Short(f,light_amount); \
	IO ## Short(f,camera_amount); \
	IO ## Long(f,anim_offset); \
	IO ## Long(f,tiles_offset); \
	IO ## Long(f,light_offset); \
	IO ## Long(f,camera_offset); \
	IO ## Short(f,anim_unk1); \
	IO ## Short(f,tiles_unk1); \
	IO ## Short(f,light_unk1); \
	IO ## Short(f,camera_unk1); \
	IO ## Short(f,anim_unk2); \
	IO ## Short(f,tiles_unk2); \
	IO ## Short(f,light_unk2); \
	IO ## Short(f,camera_unk2); \
	IO ## Short(f,anim_unk3); \
	IO ## Short(f,tiles_unk3); \
	IO ## Short(f,light_unk3); \
	IO ## Short(f,camera_unk3); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		anim = new FieldTilesAnimDataStruct[anim_amount]; \
		tiles = new FieldTilesTileDataStruct[tiles_amount]; \
		tiles_sorted = new FieldTilesTileDataStruct*[tiles_amount]; \
		light = new FieldTilesLightDataStruct[light_amount]; \
		camera = new FieldTilesCameraDataStruct[camera_amount]; \
	} \
	for (i=0;i<anim_amount;i++) { \
		SEEK(f,headerpos,anim_offset+i*0x10); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Char(f,anim[i].flag); \
		IO ## Long3(f,anim[i].tile_amount); \
		IO ## Char(f,anim[i].camera_id); \
		IO ## Long3(f,anim[i].default_frame); \
		IO ## Short(f,(uint16_t&)anim[i].rate); \
		IO ## Short(f,anim[i].counter); \
		IO ## Long(f,anim[i].tile_list_offset); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			anim[i].tile_list = new uint8_t[anim[i].tile_amount]; \
			anim[i].tile_duration = new uint8_t[anim[i].tile_amount]; \
		} \
		SEEK(f,headerpos,anim[i].tile_list_offset); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<anim[i].tile_amount;j++) { \
			IO ## Char(f,anim[i].tile_list[j]); \
			IO ## Char(f,anim[i].tile_duration[j]); \
		} \
		if (PPF) PPFEndScanStep(); \
	} \
	k = 0; \
	for (i=0;i<tiles_amount;i++) { \
		SEEK(f,headerpos,tiles_offset+i*0x38); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Long(f,tiles[i].data1); \
		IO ## Short(f,tiles[i].height); \
		IO ## Short(f,tiles[i].width); \
		IO ## Short(f,(uint16_t&)tiles[i].default_x); \
		IO ## Short(f,(uint16_t&)tiles[i].default_y); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_x); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_y); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_minx); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_maxx); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_miny); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_maxy); \
		IO ## Short(f,(uint16_t&)tiles[i].screen_x); \
		IO ## Short(f,(uint16_t&)tiles[i].screen_y); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_dx); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_dy); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_fracx); \
		IO ## Short(f,(uint16_t&)tiles[i].pos_fracy); \
		IO ## Char(f,tiles[i].camera_id); \
		IO ## Char(f,tiles[i].data2); \
		IO ## Short(f,tiles[i].tile_amount); \
		IO ## Long(f,tiles[i].tile_pos_offset); \
		IO ## Long(f,tiles[i].tile_data_offset); \
		IO ## Long(f,tiles[i].tile_packet_offset); \
		IO ## Long(f,tiles[i].tile_tpage); \
		if (PPF) PPFEndScanStep(); \
		if (READ) { \
			tiles[i].AllocTileData(); \
			tiles[i].parent = this; \
			tiles[i].id = i; \
			tiles[i].is_first_of_anim = false; \
		} \
		for (j=0;j<tiles[i].tile_amount;j++) { \
			tiles[i].tile_steam_id[j] = k++; \
			SEEK(f,headerpos,tiles[i].tile_data_offset+j*0x8); \
			if (PPF) PPFInitScanStep(f); \
			IO ## Long(f,tiles[i].tile_data_data1[j]); \
			IO ## Long(f,tiles[i].tile_data_data2[j]); \
			if (PPF) PPFEndScanStep(); \
			SEEK(f,headerpos,tiles[i].tile_pos_offset+j*0x4); \
			if (PPF) PPFInitScanStep(f); \
			IO ## Long(f,tiles[i].tile_data_data3[j]); \
			if (PPF) PPFEndScanStep(); \
		} \
	} \
	SEEK(f,headerpos,light_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<light_amount;i++) { \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(f,headerpos,camera_offset); \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<camera_amount;i++) { \
		IO ## Short(f,camera[i].distance); \
		IO ## Short(f,(uint16_t&)camera[i].angle_x); \
		IO ## Short(f,(uint16_t&)camera[i].angle_a); \
		IO ## Short(f,(uint16_t&)camera[i].eye_x); \
		IO ## Short(f,(uint16_t&)camera[i].angle_b); \
		IO ## Short(f,(uint16_t&)camera[i].eye_y); \
		IO ## Short(f,(uint16_t&)camera[i].eye_z); \
		IO ## Short(f,(uint16_t&)camera[i].focal_x); \
		IO ## Short(f,(uint16_t&)camera[i].focal_y); \
		IO ## Short(f,(uint16_t&)camera[i].focal_z); \
		IO ## Long(f,(uint32_t&)camera[i].offset_x); \
		IO ## Long(f,(uint32_t&)camera[i].offset_z); \
		IO ## Long(f,(uint32_t&)camera[i].offset_y); \
		IO ## Short(f,(uint16_t&)camera[i].offset_centerx); \
		IO ## Short(f,(uint16_t&)camera[i].offset_centery); \
		IO ## Short(f,(uint16_t&)camera[i].offset_width); \
		IO ## Short(f,(uint16_t&)camera[i].offset_height); \
		IO ## Short(f,(uint16_t&)camera[i].min_x); \
		IO ## Short(f,(uint16_t&)camera[i].max_x); \
		IO ## Short(f,(uint16_t&)camera[i].min_y); \
		IO ## Short(f,(uint16_t&)camera[i].max_y); \
		IO ## Long(f,(uint32_t&)camera[i].depth); \
		if (READ) { \
			camera[i].parent = this; \
			camera[i].id = i; \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		SetupDataInfos(true); \
		for (i=0;i<camera_amount;i++) \
			camera[i].UpdateSize(); \
	}


void FieldTilesDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_TILES_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		MACRO_TILES_IOFUNCTION(SteamRead,SteamSeek,true,false)
	}
	loaded = true;
}

void FieldTilesDataStruct::Write(fstream& f) {
	MACRO_TILES_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void FieldTilesDataStruct::WritePPF(fstream& f) {
	MACRO_TILES_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void FieldTilesDataStruct::ReadHWS(fstream& f) {
	MACRO_TILES_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void FieldTilesDataStruct::WriteHWS(fstream& f) {
	MACRO_TILES_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

FieldTilesDataStruct::~FieldTilesDataStruct() {
	if (loaded) {
		unsigned int i;
		for (i=0;i<anim_amount;i++) {
			delete[] anim[i].tile_list;
			delete[] anim[i].tile_duration;
		}
		delete[] anim;
		for (i=0;i<tiles_amount;i++) {
			delete[] tiles[i].tile_depth;
			delete[] tiles[i].tile_pos_x;
			delete[] tiles[i].tile_pos_y;
			delete[] tiles[i].tile_clut_y;
			delete[] tiles[i].tile_clut_x;
			delete[] tiles[i].tile_page_y;
			delete[] tiles[i].tile_page_x;
			delete[] tiles[i].tile_tp;
			delete[] tiles[i].tile_alpha;
			delete[] tiles[i].tile_source_v;
			delete[] tiles[i].tile_source_u;
			delete[] tiles[i].tile_h;
			delete[] tiles[i].tile_w;
			delete[] tiles[i].tile_abr;
			delete[] tiles[i].tile_trans;
			delete[] tiles[i].tile_steam_id;
			delete[] tiles[i].tile_data_data1;
			delete[] tiles[i].tile_data_data2;
			delete[] tiles[i].tile_data_data3;
		}
		delete[] tiles;
		delete[] light;
		delete[] camera;
		delete[] tiles_sorted;
	}
}

#define MACRO_WALKMESH_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i,j; \
	uint32_t headerpos; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,magic_walkmesh); \
	headerpos = f.tellg(); \
	IO ## Short(f,(uint16_t&)unknown1); \
	IO ## Short(f,(uint16_t&)offset_x2); \
	IO ## Short(f,(uint16_t&)offset_z2); \
	IO ## Short(f,(uint16_t&)offset_y2); \
	IO ## Short(f,(uint16_t&)offset_x); \
	IO ## Short(f,(uint16_t&)offset_z); \
	IO ## Short(f,(uint16_t&)offset_y); \
	IO ## Short(f,(uint16_t&)offset_x3); \
	IO ## Short(f,(uint16_t&)offset_z3); \
	IO ## Short(f,(uint16_t&)offset_y3); \
	IO ## Short(f,(uint16_t&)unknown4); \
	IO ## Short(f,(uint16_t&)unknown5); \
	IO ## Short(f,(uint16_t&)unknown6); \
	IO ## Short(f,(uint16_t&)unknown7); \
	IO ## Short(f,(uint16_t&)unknown8); \
	IO ## Short(f,(uint16_t&)unknown9); \
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
	IO ## Short(f,vertice_amount); \
	IO ## Short(f,vertice_offset); \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		triangle_unk1 = new uint8_t[triangle_amount]; \
		triangle_stepsound = new uint8_t[triangle_amount]; \
		triangle_unk2 = new uint16_t[triangle_amount]; \
		triangle_walkpath = new uint16_t[triangle_amount]; \
		triangle_normal = new uint16_t[triangle_amount]; \
		triangle_unk4 = new uint16_t[triangle_amount]; \
		triangle_unk5 = new uint16_t[triangle_amount]; \
		triangle_vertice1 = new uint16_t[triangle_amount]; \
		triangle_vertice2 = new uint16_t[triangle_amount]; \
		triangle_vertice3 = new uint16_t[triangle_amount]; \
		triangle_edge1 = new uint16_t[triangle_amount]; \
		triangle_edge2 = new uint16_t[triangle_amount]; \
		triangle_edge3 = new uint16_t[triangle_amount]; \
		triangle_adjacenttriangle1 = new uint16_t[triangle_amount]; \
		triangle_adjacenttriangle2 = new uint16_t[triangle_amount]; \
		triangle_adjacenttriangle3 = new uint16_t[triangle_amount]; \
		triangle_centerx = new int16_t[triangle_amount]; \
		triangle_centerz = new int16_t[triangle_amount]; \
		triangle_centery = new int16_t[triangle_amount]; \
		triangle_unk9 = new uint16_t[triangle_amount]; \
		triangle_unk10 = new uint16_t[triangle_amount]; \
		edge_flag = new uint16_t[edge_amount]; \
		edge_clone = new int16_t[edge_amount]; \
		animation_flag = new uint16_t[animation_amount]; \
		animation_frameamount = new uint16_t[animation_amount]; \
		animation_framerate = new int16_t[animation_amount]; \
		animation_counter = new uint16_t[animation_amount]; \
		animation_currentframe = new int32_t[animation_amount]; \
		animation_frameoffset = new uint32_t[animation_amount]; \
		walkpath_unk1 = new uint16_t[walkpath_amount]; \
		walkpath_unk2 = new uint16_t[walkpath_amount]; \
		walkpath_minx = new int16_t[walkpath_amount]; \
		walkpath_minz = new int16_t[walkpath_amount]; \
		walkpath_miny = new int16_t[walkpath_amount]; \
		walkpath_offsetx = new int16_t[walkpath_amount]; \
		walkpath_offsetz = new int16_t[walkpath_amount]; \
		walkpath_offsety = new int16_t[walkpath_amount]; \
		walkpath_unkx3 = new int16_t[walkpath_amount]; \
		walkpath_unkz3 = new int16_t[walkpath_amount]; \
		walkpath_unky3 = new int16_t[walkpath_amount]; \
		walkpath_unkx4 = new int16_t[walkpath_amount]; \
		walkpath_unkz4 = new int16_t[walkpath_amount]; \
		walkpath_unky4 = new int16_t[walkpath_amount]; \
		walkpath_triangleamount = new uint16_t[walkpath_amount]; \
		walkpath_trianglelistoffset = new uint16_t[walkpath_amount]; \
		walkpath_trianglelist = new uint32_t*[walkpath_amount]; \
		normal_x = new int32_t[normal_amount]; \
		normal_z = new int32_t[normal_amount]; \
		normal_y = new int32_t[normal_amount]; \
		normal_overz = new int32_t[normal_amount]; \
		vertice_x = new int16_t[vertice_amount]; \
		vertice_z = new int16_t[vertice_amount]; \
		vertice_y = new int16_t[vertice_amount]; \
	} \
	for (i=0;i<triangle_amount;i++) { \
		SEEK(f,headerpos,triangle_offset+i*0x28); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Char(f,triangle_unk1[i]); \
		IO ## Char(f,triangle_stepsound[i]); \
		IO ## Short(f,triangle_unk2[i]); \
		IO ## Short(f,triangle_walkpath[i]); \
		IO ## Short(f,triangle_normal[i]); \
		IO ## Short(f,triangle_unk4[i]); \
		IO ## Short(f,triangle_unk5[i]); \
		IO ## Short(f,triangle_vertice1[i]); \
		IO ## Short(f,triangle_vertice2[i]); \
		IO ## Short(f,triangle_vertice3[i]); \
		IO ## Short(f,triangle_edge1[i]); \
		IO ## Short(f,triangle_edge2[i]); \
		IO ## Short(f,triangle_edge3[i]); \
		IO ## Short(f,triangle_adjacenttriangle1[i]); \
		IO ## Short(f,triangle_adjacenttriangle2[i]); \
		IO ## Short(f,triangle_adjacenttriangle3[i]); \
		IO ## Short(f,(uint16_t&)triangle_centerx[i]); \
		IO ## Short(f,(uint16_t&)triangle_centerz[i]); \
		IO ## Short(f,(uint16_t&)triangle_centery[i]); \
		IO ## Short(f,triangle_unk9[i]); \
		IO ## Short(f,triangle_unk10[i]); \
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
	} \
	for (i=0;i<walkpath_amount;i++) { \
		SEEK(f,headerpos,walkpath_offset+i*0x20); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,walkpath_unk1[i]); \
		IO ## Short(f,walkpath_unk2[i]); \
		IO ## Short(f,(uint16_t&)walkpath_minx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_minz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_miny[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsetx[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsetz[i]); \
		IO ## Short(f,(uint16_t&)walkpath_offsety[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unkx3[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unkz3[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unky3[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unkx4[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unkz4[i]); \
		IO ## Short(f,(uint16_t&)walkpath_unky4[i]); \
		IO ## Short(f,walkpath_triangleamount[i]); \
		IO ## Short(f,walkpath_trianglelistoffset[i]); \
		if (PPF) PPFEndScanStep(); \
		if (READ) walkpath_trianglelist[i] = new uint32_t[walkpath_triangleamount[i]]; \
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
	for (i=0;i<vertice_amount;i++) { \
		SEEK(f,headerpos,vertice_offset+i*0x6); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,(uint16_t&)vertice_x[i]); \
		IO ## Short(f,(uint16_t&)vertice_z[i]); \
		IO ## Short(f,(uint16_t&)vertice_y[i]); \
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
	MACRO_WALKMESH_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void FieldWalkmeshDataStruct::WritePPF(fstream& f) {
	MACRO_WALKMESH_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void FieldWalkmeshDataStruct::ReadHWS(fstream& f) {
	MACRO_WALKMESH_IOFUNCTION(HWSRead,HWSSeek,true,false)
	MarkDataModified();
}

void FieldWalkmeshDataStruct::WriteHWS(fstream& f) {
	MACRO_WALKMESH_IOFUNCTION(HWSWrite,HWSSeek,false,false)
}

int FieldRoleDataStruct::AddModelRole(uint16_t modelid) {
	if (GetExtraSize()<0x10)
		return 1;
	SetSize(size+0x10);
	uint16_t* newmodel = new uint16_t[unk2_amount+1];
	uint8_t* newunk1 = new uint8_t[unk2_amount+1];
	uint8_t* newunk2 = new uint8_t[unk2_amount+1];
	uint8_t* newunk3 = new uint8_t[unk2_amount+1];
	uint8_t* newunk4 = new uint8_t[unk2_amount+1];
	uint8_t* newunk5 = new uint8_t[unk2_amount+1];
	uint8_t* newunk6 = new uint8_t[unk2_amount+1];
	memcpy(newmodel,unk2_model,unk2_amount*sizeof(uint16_t));
	memcpy(newunk1,unk2_unknown1,unk2_amount*sizeof(uint8_t));
	memcpy(newunk2,unk2_unknown2,unk2_amount*sizeof(uint8_t));
	memcpy(newunk3,unk2_unknown3,unk2_amount*sizeof(uint8_t));
	memcpy(newunk4,unk2_unknown4,unk2_amount*sizeof(uint8_t));
	memcpy(newunk5,unk2_unknown5,unk2_amount*sizeof(uint8_t));
	memcpy(newunk6,unk2_unknown6,unk2_amount*sizeof(uint8_t));
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
	SetSize(size-0x10);
	uint16_t* newmodel = new uint16_t[unk2_amount-1];
	uint8_t* newunk1 = new uint8_t[unk2_amount-1];
	uint8_t* newunk2 = new uint8_t[unk2_amount-1];
	uint8_t* newunk3 = new uint8_t[unk2_amount-1];
	uint8_t* newunk4 = new uint8_t[unk2_amount-1];
	uint8_t* newunk5 = new uint8_t[unk2_amount-1];
	uint8_t* newunk6 = new uint8_t[unk2_amount-1];
	memcpy(newmodel,unk2_model,unk2id*sizeof(uint16_t));
	memcpy(newunk1,unk2_unknown1,unk2id*sizeof(uint8_t));
	memcpy(newunk2,unk2_unknown2,unk2id*sizeof(uint8_t));
	memcpy(newunk3,unk2_unknown3,unk2id*sizeof(uint8_t));
	memcpy(newunk4,unk2_unknown4,unk2id*sizeof(uint8_t));
	memcpy(newunk5,unk2_unknown5,unk2id*sizeof(uint8_t));
	memcpy(newunk6,unk2_unknown6,unk2id*sizeof(uint8_t));
	memcpy(newmodel+unk2id,unk2_model+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint16_t));
	memcpy(newunk1+unk2id,unk2_unknown1+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
	memcpy(newunk2+unk2id,unk2_unknown2+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
	memcpy(newunk3+unk2id,unk2_unknown3+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
	memcpy(newunk4+unk2id,unk2_unknown4+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
	memcpy(newunk5+unk2id,unk2_unknown5+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
	memcpy(newunk6+unk2id,unk2_unknown6+unk2id+1,(unk2_amount-unk2id-1)*sizeof(uint8_t));
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

#define MACRO_ROLE_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,magic_role); \
	IO ## Short(f,zero1); \
	IO ## Char(f,unk1_amount); \
	IO ## Char(f,unk1_amount2); \
	IO ## Char(f,unk2_amount); \
	IO ## Char(f,unk2_amount2); \
	IO ## Short(f,zero2); \
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
	for (i=0;i<unk1_amount;i++) { \
		IO ## Char(f,unk1_unknown1[i]); \
		IO ## Char(f,unk1_unknown2[i]); \
		IO ## Char(f,unk1_unknown3[i]); \
		IO ## Char(f,unk1_unknown4[i]); \
		IO ## Char(f,unk1_unknown5[i]); \
		IO ## Char(f,unk1_unknown6[i]); \
		IO ## Char(f,unk1_unknown7[i]); \
		IO ## Char(f,unk1_unknown8[i]); \
		IO ## Char(f,unk1_unknown9[i]); \
		IO ## Char(f,unk1_unknown10[i]); \
		IO ## Char(f,unk1_unknown11[i]); \
		IO ## Char(f,unk1_unknown12[i]); \
	} \
	for (i=0;i<unk2_amount;i++) { \
		IO ## Short(f,unk2_model[i]); \
		IO ## Char(f,unk2_unknown1[i]); \
		IO ## Char(f,unk2_unknown2[i]); \
		IO ## Char(f,unk2_unknown3[i]); \
		IO ## Char(f,unk2_unknown4[i]); \
		IO ## Char(f,unk2_unknown5[i]); \
		IO ## Char(f,unk2_unknown6[i]); \
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

int FieldDataSet::SetFieldName(unsigned int fieldid, wstring newvalue) {
	int oldlen = script_data[fieldid]->name.length;
	int res = script_data[fieldid]->SetName(newvalue);
	if (res==0 && GetGameType()!=GAME_TYPE_PSX)
		name_space_used += script_data[fieldid]->name.length-oldlen;
	return res;
}

int FieldDataSet::SetFieldName(unsigned int fieldid, FF9String& newvalue) {
	int oldlen = script_data[fieldid]->name.length;
	int res = script_data[fieldid]->SetName(newvalue);
	if (res==0 && GetGameType()!=GAME_TYPE_PSX)
		name_space_used += script_data[fieldid]->name.length-oldlen;
	return res;
}

void FieldDataSet::Load(fstream& ffbin, ClusterSet& clusset, TextDataSet* textset) {
	unsigned int i,j,k,l;
	int relatedtxtid;
	amount = clusset.field_amount;
	struct_id = new uint16_t[amount];
	script_data = new ScriptDataStruct*[amount];
	preload = new ImageMapDataStruct*[amount];
	background_data = new FieldTilesDataStruct*[amount];
	walkmesh = new FieldWalkmeshDataStruct*[amount];
	tim_data = new TIMImageDataStruct*[amount];
	role = new FieldRoleDataStruct*[amount];
	related_text = new TextDataStruct*[amount];
	j = 0;
	LoadingDialogInit(amount,_(L"Reading fields..."));
	if (GetGameType()==GAME_TYPE_PSX) {
		cluster_id = new uint16_t[amount];
		for (i=0;i<clusset.amount;i++) {
			if (clusset.clus_type[i]==CLUSTER_TYPE_FIELD) {
				clusset.clus[i].CreateChildren(ffbin);
				for (k=0;k<clusset.clus[i].chunk_amount;k++) {
					for (l=0;l<clusset.clus[i].chunk[k].object_amount;l++) {
						ffbin.seekg(clusset.clus[i].chunk[k].object_offset[l]);
						clusset.clus[i].chunk[k].GetObject(l).Read(ffbin);
					}
				}
				cluster_id[j] = i;
				ClusterData& clustim = (ClusterData&)clusset.clus[i].chunk[clusset.clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA)].GetObject(0);
				clustim.CreateChildren(ffbin);
				for (k=0;k<clustim.chunk_amount;k++) {
					for (l=0;l<clustim.chunk[k].object_amount;l++) {
						ffbin.seekg(clustim.chunk[k].object_offset[l]);
						clustim.chunk[k].GetObject(l).Read(ffbin);
					}
				}
				ClusterData& clus = (ClusterData&)clusset.clus[i].chunk[clusset.clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA)].GetObject(1);
				clus.CreateChildren(ffbin);
				for (k=0;k<clus.chunk_amount;k++) {
					for (l=0;l<clus.chunk[k].object_amount;l++) {
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
				for (k=0;k<mapdir.file_amount;k++)
					if (mapdir.file_id[k]==script_data[j]->object_id) {
						relatedtxtid = mapdir.file_related_id[k];
						break;
					}
				if (relatedtxtid>=0) {
					related_text[j] = (TextDataStruct*)clusset.FindObjectById(CHUNK_TYPE_TEXT,relatedtxtid);
					if (related_text[j]->parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)>=0)
						script_data[j]->related_charmap_id = related_text[j]->parent_cluster->chunk[related_text[j]->parent_cluster->SearchChunkType(CHUNK_TYPE_CHARMAP)].object_id[0];
					else
						script_data[j]->related_charmap_id = 0;
				} else {
					related_text[j] = NULL;
					script_data[j]->related_charmap_id = 0;
				}
				script_data[j]->name.charmap_Ext = hades::SPECIAL_STRING_CHARMAP_EXT.GetCharmap(script_data[j]->related_charmap_id);
				script_data[j]->name.ReadFromChar(script_data[j]->header_name);
				j++;
				LoadingDialogUpdate(j);
			}
		}
	} else {
		ClusterData** dummyclus = new ClusterData*[amount];
		ConfigurationSet& config = *clusset.config;
		string fname = config.steam_dir_data;
		uint32_t fsize;
		FF9String* fieldname = new FF9String[amount];
		uint16_t* fieldnameid = new uint16_t[amount];
		uint8_t strbuffer[10];
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.field_text_file[GetSteamLanguage()]));
		SteamReadLong(ffbin,name_space_used);
		for (i=0;i<amount;i++) {
			j = 0;
			do SteamReadChar(ffbin,strbuffer[j]);
			while (strbuffer[j++]!=':');
			strbuffer[j-1] = 0;
			fieldnameid[i] = atoi((const char*)strbuffer);
			SteamReadFF9String(ffbin,fieldname[i]);
		}
		ffbin.close();
		fname = config.steam_dir_assets + "p0data7.bin";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		string fieldfilename, txtfilename;
		for (i=0;i<amount;i++) {
			struct_id[i] = config.field_id[i];
			related_text[i] = NULL;
			relatedtxtid = -1;
			fieldfilename = "";
			for (j=0;j<G_N_ELEMENTS(SteamFieldScript);j++)
				if (SteamFieldScript[j].script_id==struct_id[i]) {
					fieldfilename = SteamFieldScript[j].script_name.substr(4); // remove "EVT_"
					break;
				}
			for (j=0;j<G_N_ELEMENTS(SteamTextFile);j++) {
				txtfilename = SteamTextFile[j].name.substr(4); // remove "MES_"
				if (txtfilename.length()<=fieldfilename.length() && txtfilename.compare(fieldfilename.substr(0,txtfilename.length()))==0) {
					relatedtxtid = SteamTextFile[j].id;
					break;
				}
			}
			if (relatedtxtid>=0 && textset)
				for (j=0;j<textset->amount;j++)
					if (textset->struct_id[j]==relatedtxtid) {
						related_text[i] = textset->text_data[j];
						break;
					}
			ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_script_file[GetSteamLanguage()][i]));
			script_data[i] = new ScriptDataStruct[1];
			script_data[i]->related_charmap_id = 0;
			script_data[i]->Init(true,CHUNK_TYPE_SCRIPT,config.field_id[i],&dummyclus[i],CLUSTER_TYPE_FIELD);
			SteamReadLong(ffbin,script_data[i]->size);
			script_data[i]->Read(ffbin);
			for (j=0;j<amount;j++)
				if (fieldnameid[j]==script_data[i]->object_id) {
					script_data[i]->name = fieldname[j];
					break;
				}
			ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_role_file[i]));
			role[i] = new FieldRoleDataStruct[1];
			role[i]->Init(false,CHUNK_TYPE_FIELD_ROLE,config.field_id[i],&dummyclus[i]);
			SteamReadLong(ffbin,role[i]->size);
			role[i]->Read(ffbin);
			if (config.field_preload_file[i]>=0) {
				ffbin.seekg(config.meta_script.GetFileOffsetByIndex(config.field_preload_file[i]));
				preload[i] = new ImageMapDataStruct[1];
				preload[i]->Init(false,CHUNK_TYPE_IMAGE_MAP,config.field_id[i],&dummyclus[i]);
				SteamReadLong(ffbin,preload[i]->size);
				preload[i]->Read(ffbin);
			} else
				preload[i] = NULL;
			LoadingDialogUpdate(i);
		}
		ffbin.close();
		delete[] fieldname;
		delete[] fieldnameid;
		for (i=0;i<amount;i++) {
			if (config.field_file_id[i]==0) {
				walkmesh[i] = NULL;
				background_data[i] = NULL;
				tim_data[i] = NULL;
				continue;
			}
			stringstream fnamestream;
			fnamestream << config.steam_dir_assets << "p0data1" << (unsigned int)config.field_file_id[i] << ".bin";
			fname = fnamestream.str();
			ffbin.open(fname.c_str(),ios::in | ios::binary);
			ffbin.seekg(config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_walkmesh_file[i]));
			walkmesh[i] = new FieldWalkmeshDataStruct[1];
			walkmesh[i]->Init(false,CHUNK_TYPE_FIELD_WALK,config.field_id[i],&dummyclus[i]);
			SteamReadLong(ffbin,walkmesh[i]->size);
			walkmesh[i]->Read(ffbin);
//			ffbin.seekg(config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_image_file[i]));
			tim_data[i] = new TIMImageDataStruct[1];
			tim_data[i]->Init(false,CHUNK_TYPE_TIM,config.field_id[i],&dummyclus[i]);
			tim_data[i]->data_file_name = fname;
			tim_data[i]->data_file_offset = config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_image_file[i]);
//			tim_data[i]->Read(ffbin);
			if (config.field_tiles_localized[i])
				ffbin.seekg(config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_tiles_file[i][GetSteamLanguage()]));
			else
				ffbin.seekg(config.meta_field[config.field_file_id[i]-1].GetFileOffsetByIndex(config.field_tiles_file[i][0]));
			background_data[i] = new FieldTilesDataStruct[1];
			background_data[i]->Init(false,CHUNK_TYPE_FIELD_TILES,config.field_id[i],&dummyclus[i]);
			background_data[i]->parent = this;
			background_data[i]->id = i;
			SteamReadLong(ffbin,background_data[i]->size);
			background_data[i]->Read(ffbin);
			LoadingDialogUpdate(i);
			ffbin.close();
		}
		delete[] dummyclus;
	}
	LoadingDialogEnd();
}

void FieldDataSet::WriteSteamText(fstream& ffbin) {
	unsigned int i;
	for (i=0;i<amount;i++) {
		wstringstream namedefstream;
		FF9String namedef;
		namedefstream << (unsigned int)script_data[i]->object_id << L":" << script_data[i]->name.str_ext;
		namedef.CreateEmpty();
		namedef.SetValue(namedefstream.str());
		SteamWriteFF9String(ffbin,namedef);
		WriteShort(ffbin,0x0A0D);
	}
}

void FieldDataSet::Write(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<amount;i++) {
		ClusterData& clus = clusset.clus[cluster_id[i]];
		ffbin.seekg(clus.offset);
		clus.Write(ffbin);
	}
}

void FieldDataSet::WritePPF(fstream& ffbin, ClusterSet& clusset) {
	for (unsigned int i=0;i<amount;i++) {
		ClusterData& clus = clusset.clus[cluster_id[i]];
		ffbin.seekg(clus.offset);
		clus.WritePPF(ffbin);
	}
}

int* FieldDataSet::LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag) {
	unsigned int i,j,k;
	uint32_t chunksize,clustersize,chunkpos,objectpos,objectsize;
	uint16_t nbmodified,objectid;
	uint8_t chunktype;
	ClusterData* clus;
	int* res = new int[5];
	res[0] = 0; res[1] = 0; res[2] = 0; res[3] = 0; res[4] = 0;
	bool loadmain = localflag & 1;
	bool loadlocal = localflag & 2;
	HWSReadShort(ffhws,nbmodified);
	for (i=0;i<nbmodified;i++) {
		objectpos = ffhws.tellg();
		HWSReadShort(ffhws,objectid);
		HWSReadLong(ffhws,clustersize);
		for (j=0;j<amount;j++) {
			if (objectid==script_data[j]->object_id) {
				clus = script_data[j]->parent_cluster;
				if (clustersize<=clus->size+clus->extra_size) {
					HWSReadChar(ffhws,chunktype);
					while (chunktype!=0xFF) {
						HWSReadLong(ffhws,chunksize);
						chunkpos = ffhws.tellg();
						if (chunktype==CHUNK_TYPE_SCRIPT) {
							if (loadmain) {
								script_data[j]->ReadHWS(ffhws,usetext);
								script_data[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_FIELD_TILES) {
							if (loadmain) {
								background_data[j]->ReadHWS(ffhws);
								background_data[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_FIELD_WALK) {
							if (loadmain) {
								walkmesh[j]->ReadHWS(ffhws);
								walkmesh[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_TIM) {
							if (loadmain) {
								uint16_t timid;
								HWSReadShort(ffhws,timid);
								for (k=0;k<tim_data[j]->parent_chunk->object_amount;k++)
									if (tim_data[j][k].object_id==timid) {
										tim_data[j][k].ReadHWS(ffhws);
										tim_data[j][k].SetSize(chunksize-2);
									}
							}
						} else if (chunktype==CHUNK_TYPE_FIELD_ROLE) {
							if (loadmain) {
								role[j]->ReadHWS(ffhws);
								role[j]->SetSize(chunksize);
							}
						} else if (chunktype==CHUNK_TYPE_IMAGE_MAP && preload[j]) {
							if (loadmain) {
								uint32_t parentclussize;
								HWSReadLong(ffhws,parentclussize);
								if (GetHWSGameType()!=GetGameType()) {
									res[4]++;
								} else if (parentclussize<=preload[j]->parent_cluster->size+preload[j]->parent_cluster->extra_size) {
									if (GetHWSGameType()==GAME_TYPE_PSX)
										preload[j]->SetSize(chunksize-4);
									else
										preload[j]->SetSize(chunksize);
									preload[j]->ReadHWS(ffhws);
								} else
									res[3]++;
							}
						} else if (chunktype==CHUNK_STEAM_FIELD_NAME) {
							if (loadmain && usetext) {
								FF9String locname;
								SteamReadFF9String(ffhws,locname);
								if (GetGameType()==GAME_TYPE_PSX)
									locname.SteamToPSX();
								SetFieldName(j,locname);
							}
						} else if (chunktype==CHUNK_SPECIAL_TYPE_LOCAL) {
							if (loadlocal)
								script_data[j]->ReadLocalHWS(ffhws);
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
				j = amount;
			} else if (j+1==amount) {
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

void FieldDataSet::WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag) {
	unsigned int i,j;
	uint16_t nbmodified = 0;
	uint32_t chunkpos, nboffset = ffhws.tellg();
	ClusterData* clus;
	bool savemain = localflag & 1;
	bool savelocal = localflag & 2;
	HWSWriteShort(ffhws,nbmodified);
	for (i=0;i<amount;i++) {
		clus = script_data[i]->parent_cluster;
		if (clus->modified || (clus->parent_cluster && clus->parent_cluster->modified)) {
			clus->UpdateOffset();
			HWSWriteShort(ffhws,script_data[i]->object_id);
			HWSWriteLong(ffhws,clus->size);
			if (script_data[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_SCRIPT);
				HWSWriteLong(ffhws,script_data[i]->size);
				chunkpos = ffhws.tellg();
				script_data[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+script_data[i]->size);
				if (GetGameType()!=GAME_TYPE_PSX) {
					HWSWriteChar(ffhws,CHUNK_STEAM_FIELD_NAME);
					HWSWriteLong(ffhws,script_data[i]->name.length);
					chunkpos = ffhws.tellg();
					SteamWriteFF9String(ffhws,script_data[i]->name);
					ffhws.seekg(chunkpos+script_data[i]->name.length);
				}
			}
			if (background_data[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_FIELD_TILES);
				HWSWriteLong(ffhws,background_data[i]->size);
				chunkpos = ffhws.tellg();
				background_data[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+background_data[i]->size);
			}
			if (walkmesh[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_FIELD_WALK);
				HWSWriteLong(ffhws,walkmesh[i]->size);
				chunkpos = ffhws.tellg();
				walkmesh[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+walkmesh[i]->size);
			}
			for (j=0;j<tim_data[i]->parent_chunk->object_amount;j++)
				if (tim_data[i][j].modified && savemain) {
					HWSWriteChar(ffhws,CHUNK_TYPE_TIM);
					HWSWriteLong(ffhws,tim_data[i][j].size+2);
					chunkpos = ffhws.tellg();
					HWSWriteShort(ffhws,tim_data[i][j].object_id);
					tim_data[i][j].WriteHWS(ffhws);
					ffhws.seekg(chunkpos+tim_data[i][j].size+2);
				}
			if (role[i]->modified && savemain) {
				HWSWriteChar(ffhws,CHUNK_TYPE_FIELD_ROLE);
				HWSWriteLong(ffhws,role[i]->size);
				chunkpos = ffhws.tellg();
				role[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+role[i]->size);
			}
			if (preload[i] && preload[i]->modified && savemain) {
				preload[i]->parent_cluster->UpdateOffset();
				HWSWriteChar(ffhws,CHUNK_TYPE_IMAGE_MAP);
				HWSWriteLong(ffhws,preload[i]->size+4);
				chunkpos = ffhws.tellg();
				HWSWriteLong(ffhws,preload[i]->parent_cluster->size);
				preload[i]->WriteHWS(ffhws);
				ffhws.seekg(chunkpos+preload[i]->size+4);
			}
			if (savelocal) {
				uint32_t localsize = 0;
				HWSWriteChar(ffhws,CHUNK_SPECIAL_TYPE_LOCAL);
				HWSWriteLong(ffhws,localsize);
				chunkpos = ffhws.tellg();
				script_data[i]->WriteLocalHWS(ffhws);
				localsize = (long long)ffhws.tellg()-chunkpos;
				ffhws.seekg(chunkpos-4);
				HWSWriteLong(ffhws,localsize);
				ffhws.seekg(chunkpos+localsize);
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
