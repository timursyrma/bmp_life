#ifndef BMPLIFE_CONFIG_H
#define BMPLIFE_CONFIG_H

typedef struct {
    char *inputFile;
    char outputDir[255];
    int maxIter;
    int dumpFreq;
    int show;
} Config;

Config parse_args(int argc, char *argv[]);
void error_exit(const char *message);

#endif // BMPLIFE_CONFIG_H
