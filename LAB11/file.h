#pragma once

#pragma pack (push,1)

typedef struct BITMAPFILEHEADER {
	unsigned char Signature[2];
	unsigned int FileSize;
	unsigned char Reserved[4];
	unsigned int DataOffset;
}File_Header;

typedef struct BITMAPINFOHEADER {
	unsigned int Size;
	unsigned int Width;
	unsigned int Height;
	unsigned short Planes;
	unsigned short BitCount;
	unsigned int Compression;
	unsigned int ImageSize;
	unsigned int XpixelsPerM;
	unsigned int YpixelsPerM;
	unsigned int ColorsUsed;
	unsigned int ColorsImportant;
}Info_Header;

typedef struct PIXEL_24 {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}pixel_24;

typedef struct FILEPICTURE {
	File_Header file;
	Info_Header info;
	pixel_24** data;
	unsigned char ** pixel_8;
}Picture;

typedef struct tagRGBQUAD
{
	unsigned char  rgbBlue;
	unsigned char  rgbGreen;
	unsigned char  rgbRed;
	unsigned char  rgbReserved;
} RGBQUAD;

#pragma pack(pop)

Picture* loadPicture(const char* file_name, RGBQUAD** palitr);

void freePicture(Picture** picture);

void makeFileResult(Picture* picture,const char* name, RGBQUAD* palitr);

char* inputFileName();

void ckekerForFormat(Picture** picture);

void chekerForFileInFolder(char** file_name);

int findRowPadding(const Picture* picture);

void makeMasOfPixels(FILE* file, Picture** picture);