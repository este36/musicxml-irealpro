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
    bool inside_root;
} ParserState;

typedef struct {
    uint16_t part_selected;
} ParserParams;

typedef struct {
    IrealProSong* song;
    ParserParams* params;
    ParserState state;
} ParserData;

/*---------- from irealpro.h ----------*/
// typedef struct {
//     da_str title;
//     struct composer {
//         da_str first_name;
//         da_str last_name;
//     };
//     da_str body;
//     uint16_t tempo;
//     StyleEnum style;
// } IrealProSong;

//  We want to get theses infos:
//   - Song Title
//   - Composer first name & last name
//   - Style
//   - Tempo
int get_song_credentials(void* user_data, sax_context* context);

int get_parts_count(void* user_data, sax_context* context);

int find_part(void* user_data, sax_context* context);

int get_irealpro_song_body(void* user_data, sax_context* context);


IrealProSong* parse_musicxml_song(ParserParams* parameters, const char* musicxml, const size_t musicxml_length);

#endif // __PARSER_H__
