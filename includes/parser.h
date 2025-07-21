#ifndef __MXL_PARSER_H__
#define __MXL_PARSER_H__

#include "da.h"
#include "musicxml.h"
#include "irealpro.h"
#include "sax.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif

typedef struct {
    IrpSong *song;
    da_str_ref part_selected;
    void *state;
} ParserData;

int parse_musicxml_song(IrpSong *irp_song, const da_str_ref part_id, const char *musicxml, const size_t musicxml_length);
#endif // __PARSER_H__
