#ifndef BMPLIFE_BMP_H
#define BMPLIFE_BMP_H

#include <stdint.h>
#include <stdio.h>

typedef int32_t  LONG;
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef struct {
	WORD bfType;
	DWORD bfSize;
	DWORD bsReserved;
	DWORD bfOffBits;
} __attribute__ ((packed)) BITMAPFILEHEADER;

typedef struct {
	DWORD biSize;
	DWORD biWidth;
	DWORD biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	DWORD biXPelsPerMeter;
	DWORD biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} __attribute__ ((packed)) BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} __attribute__ ((packed)) RGBQUAD;

typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD *colorTable;
    BYTE *pixelData;
} BMPImage;

BMPImage readBMP(FILE *filePointer);
void saveBMP(BMPImage bmp, FILE *output);
void run(BMPImage bmp, int maxIter, int dumpFreq, int isShow, const char *outdir);

#endif // BMPLIFE_BMP_H