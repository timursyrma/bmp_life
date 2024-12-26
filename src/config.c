#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include <sys/stat.h>

void error_exit(const char *message) {
    printf("%s\n", message);
    exit(1);
}

Config parse_args(int argc, char *argv[]) {
    if (argc < 3) {
        error_exit("Incorrect number of arguments");
    }

    Config config = {0};
    config.maxIter = 50;
    config.dumpFreq = 1;
    config.show = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            config.inputFile = argv[i + 1];
            printf("%s\n", config.inputFile);

        }
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            strncpy(config.outputDir, argv[++i], sizeof(config.outputDir) - 1);
            config.outputDir[sizeof(config.outputDir) - 1] = '\0'; // Гарантируем завершение строки
        }
        else if (strcmp(argv[i], "--max_iter") == 0 && i + 1 < argc) {
            config.maxIter = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--dump_freq") == 0 && i + 1 < argc) {
            config.dumpFreq = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--show") == 0) {
            config.show = 1;
        }
    }

    struct stat st = {0};
    if (stat(config.outputDir, &st) == -1) {
        if (mkdir(config.outputDir, 0700) != 0) {
            error_exit("Can't create output directory");
        }
    }

    return config;
}
