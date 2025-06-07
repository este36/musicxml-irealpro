#ifndef __MXL_PARSER_H__
#define __MXL_PARSER_H__

#include "da.h"
#include "../gen/musicxml.h"
#include "irealpro.h"
#include "sax.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif

typedef struct {
    uint16_t part_selected;
} ParserParams;

typedef struct {
    bool work;
    bool identification;
    bool defaults;
    bool credit;
    bool part_list;
} ParserState;

typedef struct {
    IrpSong* song;
    ParserParams* params;
    ParserState state;
} ParserData;

int parse_musicxml_song(IrpSong* irp_song, ParserParams* parameters, const char* musicxml, const size_t musicxml_length);
#endif // __PARSER_H__
