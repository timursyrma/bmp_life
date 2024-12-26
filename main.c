#include <stdio.h>
#include <stdlib.h>
#include "src/config.h"
#include "src/bmp.h"

int main(int argc, char *argv[]) {
    Config config = parse_args(argc, argv);
    printf("%s\n", config.inputFile);


    FILE *fptr = fopen(config.inputFile, "rb");
    if (!fptr) {
        error_exit("File not found");
    }

    BMPImage bmpImage = readBMP(fptr);
    fclose(fptr); // Закрываем файл после чтения

    FILE *outputFile = fopen("output.bmp", "wb");
    saveBMP(bmpImage, outputFile);
    fclose(outputFile);

    free(bmpImage.colorTable);
    free(bmpImage.pixelData);

    return 0;
}