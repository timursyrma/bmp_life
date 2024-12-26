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

    run(bmpImage, config.maxIter, config.dumpFreq, config.show, config.outputDir);


    free(bmpImage.colorTable);
    free(bmpImage.pixelData);

    return 0;
}