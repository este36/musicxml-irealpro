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
    struct {
        da_str first_name;
        da_str last_name;
    } composer;
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

INLINE void freeIrealProSong(IrealProSong* song)
{
    da_str_free(&song->title);
    da_str_free(&song->composer.first_name);
    da_str_free(&song->composer.last_name);
    da_str_free(&song->body);
}

INLINE IrealProSong* new_IrealProSong(void)
{
    IrealProSong* s = malloc(sizeof(IrealProSong));
    if (s == NULL) return NULL;
    s->title = DA_STR("");
    s->body = DA_STR("");
    s->composer.first_name = DA_STR("");
    s->composer.last_name = DA_STR("");
    s->tempo = 0;
    s->style = UNKNOWN;
    return s;
}

#endif // IREALPRO_H
