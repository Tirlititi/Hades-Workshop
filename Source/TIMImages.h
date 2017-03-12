#ifndef _TIMIMAGES_H
#define _TIMIMAGES_H

struct TIMImageDataStruct;

#include <inttypes.h>
#include <fstream>
#include "Configuration.h"
using namespace std;

typedef uint32_t TIM_Format;
#define TIM_FORMAT_4BPP_NOCLUT		0x0 // Unused
#define TIM_FORMAT_8BPP_NOCLUT		0x1 // Unused
#define TIM_FORMAT_16BPP			0x2 // Most used (Fields, Battle Scenes, some generic UI...) ; may be a single CLUT palette
#define TIM_FORMAT_24BPP			0x3 // Unused
#define TIM_FORMAT_4BPP				0x8 // Used by some generic UI (charmaps, menu icons...) ; actually 8 bits for 3 pixels
#define TIM_FORMAT_8BPP				0x9 // Used by models (except Battle Scenes') and few generic UI

typedef uint8_t TIM_BlendMode;
#define TIM_BLENDMODE_ALPHA		0x0
#define TIM_BLENDMODE_LIGHT		0x1
#define TIM_BLENDMODE_SHADE		0x2

struct TIMImageDataStruct : public ChunkChild {
public:
	// PSX
	// Header
	uint32_t magic_tim;
	uint32_t format;
	// Default palette, only used if (format & 8)
	uint32_t pal_size;
	uint16_t pal_x;
	uint16_t pal_y;
	uint16_t pal_width;
	uint16_t pal_height;
	uint8_t* pal_value;
	// Image, sometimes used as palette for another TIM
	uint32_t img_size;
	uint16_t pos_x;
	uint16_t pos_y;
	uint16_t width;
	uint16_t height;
	uint8_t* pixel_value; // Also used in Steam
	
	// Steam
	uint32_t steam_width;
	uint32_t steam_height;
	uint32_t steam_size1;
	uint32_t steam_format;
	int32_t steam_mip_count;
	uint32_t steam_flags;
	int32_t steam_image_count;
	int32_t steam_dimension;
	int32_t steam_filter_mode;
	int32_t steam_anisotropic;
	uint32_t steam_mip_bias; // float
	int32_t steam_wrap_mode;
	int32_t steam_lightmap_format;
	int32_t steam_color_space;
	uint32_t steam_size2;
	
	// Unused anymore ; data is stored in pixel_value and decompressed with squish
/*	uint8_t* steam_pixel_alpha;
	uint8_t* steam_pixel_alpha_ex;
	uint16_t* steam_pixel_alpha_flag1;
	uint16_t* steam_pixel_alpha_flag2;
	uint16_t* steam_pixel_alpha_flag3;
	uint16_t* steam_pixel_color;
	uint16_t* steam_pixel_color_ex;
	uint32_t* steam_pixel_color_flag;*/
	
	static uint8_t color[32]; // colors available for palettes
	
	// As images recquire a lot of memory allocation, a flush method is provided for this struct
	// Use Read before using the datas and Flush after that
	// ToDo : get with a way to keep track of modifications
	// Actually only used for the Steam version (Field backgrounds)
	string data_file_name;
	uint32_t data_file_offset;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void Flush();
	
	uint16_t GetPosX();
	uint16_t GetWidth();
	
	//  Return temporary array not to be deleted
	uint32_t* ConvertAsPalette(uint16_t palpos, bool usealpha=true);
	//  If NULL is passed as a palette, then palpos must be set
	// and the TIM must have an internal palette
	uint32_t* ConvertAsImage(uint16_t texpos, uint16_t sizex, uint16_t sizey, uint32_t* pal=NULL, uint16_t palpos=0, bool usealpha=true);
	// Return array allocated with "new" and should be deleted
	uint32_t* ConvertAsSteamImage(bool usealpha=true);
	uint32_t* ConvertAsFullImage(uint32_t* pal=NULL, uint16_t palpos=0, bool usealpha=true);
	// TIFF image exportation
	int Export(const char* outputfile, bool full=true, uint16_t texpos=0, uint32_t* pal=NULL, uint16_t palpos=0, bool usealpha=true);
	// Doesn't check if there is enough space in the cluster : check it manually
	// Charflag : (-1 = unused) (0 = import in even palettes) (1 = import in odd palettes)
	void Import(uint8_t* colordata, uint8_t* alphadata, uint16_t posx, uint16_t posy, uint16_t sizex, uint16_t sizey, uint32_t* pal, int charflag=-1);
	void ImportPalette(uint16_t amount, uint32_t** paldata);
	// Change pixel color using the palette value
	void SetPixelValue(uint16_t x, uint16_t y, uint8_t pxvalue, int charflag=-1);
	
	void LoadInVRam(bool loadallchunk = true);
	static uint32_t GetVRamPixel(unsigned int x, unsigned int y, unsigned int px, unsigned int py, bool shortformat = false);
	
	static uint32_t ComputeMidColor(uint32_t leftcolor, uint32_t rightcolor);
	static uint32_t* CreatePaletteFromData(uint8_t* colordata, uint8_t* alphadata, uint16_t sizex, uint16_t sizey);
	static TIMImageDataStruct& GetTIMPaletteStruct(TIMImageDataStruct& tim1, TIMImageDataStruct& tim2);
	static TIMImageDataStruct& GetTIMTextureStruct(TIMImageDataStruct& tim1, TIMImageDataStruct& tim2);
	
	// Create a RGBA image from the Unity-formatted texture imgbuffer (header must be included) ; destrgba must be deleted[]
	static bool ConvertFromSteamTexture(uint8_t* imgbuffer, uint32_t* destw, uint32_t* desth, uint8_t** destrgba);
	// Compute the file size of an Unity-formatted texture
	static uint32_t GetSteamTextureFileSize(uint32_t imgwidth, uint32_t imgheight, uint32_t textformat = 0x0C);
	// Create a Unity-formatted texture (DXT5) ; result must be deleted[]
	static uint8_t* CreateSteamTextureFile(uint32_t& datasize, uint32_t w, uint32_t h, uint8_t* rgba, uint32_t textformat = 0x0C, int quality = -1);
};

uint32_t ImageMergePixels(uint32_t pix1, uint32_t pix2, TIM_BlendMode mode = TIM_BLENDMODE_ALPHA);

#endif
