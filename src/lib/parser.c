#include "parser.h"

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
int get_song_credentials(void* user_data, sax_context* context)
{
    const xml_node* n = &context->found;

    switch (context->found.type) {
        case XML_TAG_OPEN:
            if (str_ref_cmp(&n->target, &musicxml.score_partwise)) {
                printf("we entered score-parwise");
                return PARSER_STOP;
            }
            return PARSER_CONTINUE;
        case XML_TAG_CLOSE:
           return PARSER_CONTINUE; 
        case XML_SELF_CLOSING:
           return PARSER_CONTINUE; 
        default: return PARSER_CONTINUE;
    }
    return 0;
}

int get_parts_count(void* user_data, sax_context* context)
{
    return 0;
}

int find_part(void* user_data, sax_context* context)
{
    return 0;
}

int get_irealpro_song_body(void* user_data, sax_context* context)
{
    return 0;
}

IrealProSong* parse_musicxml_song(ParserParams* parameters, const char* musicxml, const size_t musicxml_length)
{
    ParserData parser_data = {
        .song = new_IrealProSong(),
        .params = parameters,
        .state = {0}
    };
    
    if (parser_data.song == NULL) return NULL;
    
    sax_scanner scanner = sax_scanner_init(musicxml, musicxml_length);
    sax_context context = sax_context_init(&scanner);

    if (sax_parse_xml(get_song_credentials, &parser_data, &context) != 0) {
        freeIrealProSong(parser_data.song);;
        free(parser_data.song);
        return NULL;
    }

    return parser_data.song;
}
