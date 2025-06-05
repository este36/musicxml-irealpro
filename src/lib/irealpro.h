#ifndef IREALPRO_H
#define IREALPRO_H

#include "sax.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#define INLINE static inline
#else
#define INLINE static
#endif

typedef enum {
    UNKNOWN,
    JAZZ,
    JAZZ_LATIN,
    ROCK,
    POP,
} StyleEnum;

typedef struct {
    da_str title;
    da_str composer;
    da_str body;
    uint16_t tempo;
    StyleEnum style;
} IrealProSong;

typedef struct {
    da_str title;
    struct {
        size_t count;
        size_t capacity;
        IrealProSong* items;
    } song;
} IrealProPlaylist;

void freeIrealProSong(IrealProSong* song);

IrealProSong* new_IrealProSong(void);

#endif // IREALPRO_H
