#include "irealpro.h"

void freeIrealProSong(IrealProSong* song)
{
    da_str_free(&song->title);
    da_str_free(&song->composer.first_name);
    da_str_free(&song->composer.last_name);
    da_str_free(&song->body);
}

IrealProSong* new_IrealProSong(void)
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
