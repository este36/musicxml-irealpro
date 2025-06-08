#include "parser.h"
#include <string.h>

#define GET_CURR_MEASURE() (&parser_data->song->measures.items[parser_data->song->measures.count - 1])

typedef struct {
    bool attributes;
    bool rearsheal;
} MsrState;


static int parse_note(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {

        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.note)) return PARSER_STOP;
        }
        break;
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_attributes(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.attributes)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_direction(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.direction)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_harmony(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.harmony)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_barline(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.barline)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_measure(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;
    IrpMeasure *m = GET_CURR_MEASURE();
    MsrState *state = (MsrState*)parser_data->state;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (m->chords.count != 0 && str_ref_cmp(&n->target, &musicxml.note)) { 
                if (sax_parse_xml(parse_note, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (!state->attributes && str_ref_cmp(&n->target, &musicxml.attributes)) { 
                state->attributes = true;
                if (sax_parse_xml(parse_attributes, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (str_ref_cmp(&n->target, &musicxml.direction)) { 
                if (sax_parse_xml(parse_direction, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (str_ref_cmp(&n->target, &musicxml.harmony)) { 
                if (sax_parse_xml(parse_harmony, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (str_ref_cmp(&n->target, &musicxml.barline)) { 
                if (sax_parse_xml(parse_barline, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.measure)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:

        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_part(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_cmp(&n->target, &musicxml.measure)) { 
                da_append(parser_data->song->measures, (IrpMeasure){0});
                MsrState msr_state = {0};
                parser_data->state = &msr_state;
                if (sax_parse_xml(parse_measure, parser_data, context) != 0) return PARSER_STOP_ERROR;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.part)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:

        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_work(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (!parser_data->song->title[0] && str_ref_cmp(&n->target, &musicxml.work_title)) { 
                da_str_ref title;
                if (sax_get_content(context, &title) != 0) return PARSER_STOP_ERROR;
                if (title.len < IRP_MAX_CREDENTIALS ) 
                    memcpy(parser_data->song->title, title.buf, title.len);

                // we have what we need, so we skip the rest of parent content
                if (sax_skip_content(context, musicxml.work) != 0) return PARSER_STOP_ERROR;
                return PARSER_STOP;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.work)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:

        default: break;
    }
            
    return PARSER_CONTINUE;
}

static int parse_identification(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            da_str_ref composer_str = STR_REF("composer");
            if (!parser_data->song->composer[0]
                && str_ref_cmp(&n->target, &musicxml.creator)
                && n->attrc >= 1 
                && str_ref_cmp(&n->attrv[0].key, &musicxml.type)
                && str_ref_cmp(&n->attrv[0].value, &composer_str)
            ) {
                da_str_ref full_name;
                if (sax_get_content(context, &full_name) != 0) return PARSER_STOP_ERROR;
                if (full_name.len < IRP_MAX_CREDENTIALS ) 
                    memcpy(parser_data->song->composer, full_name.buf, full_name.len);

                // we have what we need, so we skip the rest of parent content
                if (sax_skip_content(context, musicxml.identification) != 0) return PARSER_STOP_ERROR;
                return PARSER_STOP;
            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.identification)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:

        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_song_partwise(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;

    if (n->type == XML_TAG_OPEN) {
        if (str_ref_cmp(&n->target, &musicxml.score_partwise)) {
            return PARSER_CONTINUE;
        }
        if (str_ref_cmp(&n->target, &musicxml.work)) {
            if (sax_parse_xml(parse_work, parser_data, context) != 0) return PARSER_STOP_ERROR;

        } else if (str_ref_cmp(&n->target, &musicxml.identification)) {
            if (sax_parse_xml(parse_identification, parser_data, context) != 0) return PARSER_STOP_ERROR;

        } else if (str_ref_cmp(&n->target, &musicxml.part)
                    && n->attrc >= 1
                    && str_ref_cmp(&n->attrv[0].key, &musicxml.id) 
                    && str_ref_cmp(&n->attrv[0].value, &parser_data->part_selected) 
                ) {
            if (sax_parse_xml(parse_part, parser_data, context) != 0) return PARSER_STOP_ERROR;
            return PARSER_STOP;
        } else {
            return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
        }
    }

    return PARSER_CONTINUE;
}

// All of irp_song members should be init to zero.
int parse_musicxml_song(IrpSong* irp_song, const da_str_ref part_id, const char* musicxml, const size_t musicxml_length)
{
    ParserData parser_data = {
        .song = irp_song,
        .part_selected = part_id,
    };

    sax_scanner scanner = sax_scanner_init(musicxml, musicxml_length);
    sax_context context = sax_context_init(&scanner);

    return sax_parse_xml(parse_song_partwise, &parser_data, &context);
}
