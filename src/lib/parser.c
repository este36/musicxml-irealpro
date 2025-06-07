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

static int parse_part_list(void* user_data, sax_context* context)
{
    return 0;
}

static int parse_work(void* user_data, sax_context* context)
{
    const xml_node* n = &context->found;
    ParserData* parser_data = (ParserData*)user_data;

    switch (context->found.type) {
        case XML_TAG_OPEN:
            if (!parser_data->song->title.len && str_ref_cmp(&n->target, &musicxml.work_title)) { 
                da_str_ref title;
                if (sax_get_content(context, &title) != 0) return PARSER_STOP_ERROR;
                da_str_append(&parser_data->song->title, title);
            }

            return PARSER_CONTINUE;
        case XML_TAG_CLOSE:
            if (str_ref_cmp(&n->target, &musicxml.work)) { 
                parser_data->state.work = true;
                return PARSER_STOP;
            }

            return PARSER_CONTINUE; 
        default: return PARSER_CONTINUE;
    }
}

static int parse_identification(void* user_data, sax_context* context)
{
    const xml_node* n = &context->found;
    ParserData* parser_data = (ParserData*)user_data;

    switch (context->found.type) {
        case XML_TAG_OPEN:
            da_str_ref composer_str = STR_REF("composer");
            if (!parser_data->song->composer.len
                && str_ref_cmp(&n->target, &musicxml.creator)
                && n->attrc >= 1 
                && str_ref_cmp(&n->attrv[0].key, &musicxml.type)
                && str_ref_cmp(&n->attrv[0].value, &composer_str)
            ) {
                da_str_ref full_name;
                if (sax_get_content(context, &full_name) != 0) return PARSER_STOP_ERROR;
                da_str_append(&parser_data->song->composer, full_name);

            } else if (str_ref_cmp(&n->target, &musicxml.encoding)) {
                if (sax_skip_content(context) != 0) return PARSER_STOP_ERROR;
            }

            return PARSER_CONTINUE;
        case XML_TAG_CLOSE:
            if (str_ref_cmp(&n->target, &musicxml.identification)) { 
                parser_data->state.identification = true;
                return PARSER_STOP;
            }

            return PARSER_CONTINUE; 
        default: return PARSER_CONTINUE;
    }
}

static int parse_song_partwise(void* user_data, sax_context* context)
{
    const xml_node* n = &context->found;
    ParserData* parser_data = (ParserData*)user_data;

    switch (context->found.type) {
        case XML_TAG_OPEN:
            if (!parser_data->state.work && str_ref_cmp(&n->target, &musicxml.work)) {
                if (sax_parse_xml(parse_work, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (!parser_data->state.identification && str_ref_cmp(&n->target, &musicxml.identification)) {
                if (sax_parse_xml(parse_identification, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (!parser_data->state.defaults && str_ref_cmp(&n->target, &musicxml.defaults)) {
                if (sax_skip_content(context) != 0) return PARSER_STOP_ERROR;
                parser_data->state.defaults = true;

            } else if (!parser_data->state.credit && str_ref_cmp(&n->target, &musicxml.credit)) {
                if (sax_skip_content(context) != 0) return PARSER_STOP_ERROR;
                parser_data->state.credit = true;

            } else if (!parser_data->state.part_list && str_ref_cmp(&n->target, &musicxml.part_list)) {
                if (sax_skip_content(context) != 0) return PARSER_STOP_ERROR;
                parser_data->state.part_list = true;

            } else if (str_ref_cmp(&n->target, &musicxml.part)) {
                return PARSER_STOP;
            }
                // main parser stuff
            return PARSER_CONTINUE;
        default: return PARSER_CONTINUE;
    }
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

    if (sax_parse_xml(parse_song_partwise, &parser_data, &context) != 0) {
        freeIrealProSong(parser_data.song);;
        free(parser_data.song);
        return NULL;
    }

    return parser_data.song;
}
