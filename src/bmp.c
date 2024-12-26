#include <stdlib.h>
#include <unistd.h>

#include "bmp.h"
#include "utils.h"
#include "config.h"

#define BMP_TYPE 0x4D42

BMPImage readBMP(FILE *filePointer) {
    BMPImage bmp;

    fread(&bmp.fileHeader, sizeof(BITMAPFILEHEADER), 1, filePointer);

    if (bmp.fileHeader.bfType != BMP_TYPE) {
        error_exit("Invalid BMP file format");
    }

    fread(&bmp.infoHeader, sizeof(BITMAPINFOHEADER), 1, filePointer);

    bmp.colorTable = NULL;
    if (bmp.infoHeader.biBitCount <= 8) {
        DWORD colors =  bmp.infoHeader.biClrUsed 
                        ? bmp.infoHeader.biClrUsed 
                        : 1 << bmp.infoHeader.biBitCount;


        bmp.colorTable = (RGBQUAD *)malloc(colors * sizeof(RGBQUAD));
        fread(bmp.colorTable, sizeof(RGBQUAD), colors, filePointer);
    }

    DWORD pixelDataSize = bmp.infoHeader.biSizeImage;
    bmp.pixelData = (BYTE *)malloc(pixelDataSize);
    fread(bmp.pixelData, sizeof(BYTE), pixelDataSize, filePointer);

    return bmp;
}

void saveBMP(BMPImage bmp, FILE *output) {
    fwrite(&bmp.fileHeader, sizeof(BITMAPFILEHEADER), 1, output);

    fwrite(&bmp.infoHeader, sizeof(BITMAPINFOHEADER), 1, output);

    if (bmp.colorTable) {
        DWORD colorsCount = bmp.infoHeader.biClrUsed 
                            ? bmp.infoHeader.biClrUsed 
                            : (1 << bmp.infoHeader.biBitCount);
        fwrite(bmp.colorTable, sizeof(RGBQUAD), colorsCount, output);
    }

    DWORD pixelDataSize = bmp.infoHeader.biSizeImage;
    fwrite(bmp.pixelData, sizeof(BYTE), pixelDataSize, output);
}

void run(BMPImage bmp, int maxIter, int dumpFreq, int isShow, const char *outdir) {
    for (int iter = 0; iter < maxIter; iter++) {
        evolve(bmp.pixelData, bmp.infoHeader.biWidth, bmp.infoHeader.biHeight);

        if (dumpFreq > 0 && (iter % dumpFreq == 0)) {
            char filename[300];
            sprintf(filename, "%s/output_%d.bmp", outdir, iter);
            
            FILE *fout = fopen(filename, "wb");
            saveBMP(bmp, fout);
            fclose(fout);
        }

        if (isShow) {
            usleep(300000);
            show(bmp.pixelData, bmp.infoHeader.biWidth, bmp.infoHeader.biHeight);
        }
    }
}
