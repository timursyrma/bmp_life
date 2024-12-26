#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

void show(uint8_t *data, int w, int h) {
    int aligned_w   = (w + 31) & ~31;
    int padded_raw  = aligned_w / 8;

    int needed_raw  = (w + 8 - 1) / 8; 


    printf("\033[H");

    for (int y = 0; y < h; y++) {
        const uint8_t *row_ptr = data + y * padded_raw;
        for (int x = 0; x < needed_raw; x++) {
            uint8_t byte_val = row_ptr[x];
            for (int bit = 7; bit >= 0; bit--) {
                int pixel_pos = x * 8 + (7 - bit);
                // Проверяем, не выходим ли мы за реальную ширину
                if (pixel_pos < w) {
                    printf("%s", (byte_val & (1 << bit)) ? "  " : "\033[07m  \033[m");
                }
            }
        }
        printf("\033[E");
    }

    fflush(stdout);
}

void evolve(uint8_t *data, int w, int h) {
    int aligned_w   = (w + 31) & ~31;        // ширина, выровненная до 32 бит
    int padded_raw  = aligned_w / 8;         // кол-во байт на строку с учётом выравнивания

    // Буфер для следующего поколения
    uint8_t *next   = malloc(padded_raw * h);
    memset(next, 0, padded_raw * h);

    // Макрос для получения бита (x,y) с учётом тороидальной топологии (зацикливание по краям)
    #define GET_BIT(DATA, X, Y) ({           \
        int px = ((X) + w) % w;             \
        int py = ((Y) + h) % h;             \
        int off = py * padded_raw + (px / 8); \
        int sh  = 7 - (px % 8);             \
        ((DATA)[off] & (1 << sh)) ? 1 : 0;  \
    })

    // Макрос для установки бита (x,y) в 0 или 1
    #define SET_BIT(DATA, X, Y, VAL) ({      \
        int px = (X);                       \
        int py = (Y);                       \
        int off = py * padded_raw + (px / 8); \
        int sh  = 7 - (px % 8);             \
        if (VAL) (DATA)[off] |=  (1 << sh); \
        else     (DATA)[off] &= ~(1 << sh); \
    })

    // Считаем новое поколение
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // Подсчёт соседей в квадрате 3x3 вокруг (x,y)
            int n = 0;
            for (int yy = y - 1; yy <= y + 1; yy++) {
                for (int xx = x - 1; xx <= x + 1; xx++) {
                    if (GET_BIT(data, xx, yy)) {
                        n++;
                    }
                }
            }
            // Если клетка была живой — вычитаем её саму из числа соседей
            int alive = GET_BIT(data, x, y);
            if (alive) {
                n--;
            }
            // Правила жизни Конвея: рождается, выживает, умирает
            int next_val = (n == 3 || (n == 2 && alive)) ? 1 : 0;
            SET_BIT(next, x, y, next_val);
        }
    }

    // Переносим новое поколение в основной массив и освобождаем буфер
    memcpy(data, next, padded_raw * h);
    free(next);
}