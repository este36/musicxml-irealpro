#include "parser.h"
#include <string.h>

#define GET_CURR_MEASURE(parser_data) (&(parser_data)->song->measures.items[(parser_data)->song->measures.count - 1])
#define GET_CURR_CHORD(parser_data) (&(GET_CURR_MEASURE(parser_data))->chords.items[(GET_CURR_MEASURE(parser_data))->chords.count - 1])

#define TMP_CHORD_MAX_LEN 128
#define TMP_CHORD_MAX_DEGREES 3
typedef struct {
    int16_t value;
    int16_t alter;
} mxl_degree;

typedef struct {
    bool rehearsal;
    bool attributes;
    struct {
        NoteEnum root;
        NoteEnum bass;
        da_str_ref qual;
        mxl_degree degrees[TMP_CHORD_MAX_DEGREES];
        uint32_t deg_count;
    } curr_chord;
} MsrState;

static int parse_degree(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;
    MsrState *msr_state = (MsrState*)parser_data->state;
    mxl_degree* curr_deg = &msr_state->curr_chord.degrees[msr_state->curr_chord.deg_count];

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_cmp(&n->target, &musicxml.degree_value)) {
                char value_str[4] = {0};
                if (sax_copy_content(context, value_str, 4) != 0) return PARSER_STOP_ERROR;
                int16_t value_int = atoi(value_str);
                // we do not accept additional degrees of third or seventh
                if (value_int % 7 == 3 || value_int % 7 == 0) return PARSER_STOP;
                curr_deg->value = value_int;
            } else if (curr_deg->value != 0 && str_ref_cmp(&n->target, &musicxml.degree_alter)) {
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0) return PARSER_STOP_ERROR;
                curr_deg->value = atoi(alter_str);
            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.degree)) return PARSER_STOP;
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

// A is 65 in ascii code, so a bit of arithmetic and me can get the correct root note.
#define GET_NOTE_STEP() ((NoteEnum)(((int)GET_CHAR(context->scanner) - 'A') * 3 + 1))

static int parse_harmony(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    ParserData *parser_data = (ParserData*)user_data;
    MsrState *msr_state = (MsrState*)parser_data->state;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            // <degree> element is checked first because there can be a lot of them inside <harmony> element.
            if ( msr_state->curr_chord.deg_count < TMP_CHORD_MAX_DEGREES
                        && str_ref_cmp(&n->target, &musicxml.degree)
                    ) {
                if (sax_parse_xml(parse_degree, parser_data, context) != 0) return PARSER_STOP_ERROR;
                msr_state->curr_chord.deg_count++;

            } else if (str_ref_cmp(&n->target, &musicxml.root_step)) {
                // the scanner is just after the > of open tag. root-step content is only one char.
                msr_state->curr_chord.root = GET_NOTE_STEP();

            } else if (str_ref_cmp(&n->target, &musicxml.root_alter)) {
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0) return PARSER_STOP_ERROR;

                if (strcmp(alter_str, "1")) msr_state->curr_chord.root++;
                else if (strcmp(alter_str, "-1")) msr_state->curr_chord.root--;

            } else if (str_ref_cmp(&n->target, &musicxml.kind)) {
                if (sax_get_content(context, &msr_state->curr_chord.qual) != 0) return PARSER_STOP_ERROR; 

            } else if (str_ref_cmp(&n->target, &musicxml.bass_step)) {
                // same as root-step
                msr_state->curr_chord.bass = GET_NOTE_STEP();

            } else if (str_ref_cmp(&n->target, &musicxml.bass_alter)) {
                // same logic as root-alter
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0) return PARSER_STOP_ERROR;

                if (strcmp(alter_str, "1")) msr_state->curr_chord.bass++;
                else if (strcmp(alter_str, "-1")) msr_state->curr_chord.bass--;

            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.harmony)) {
                // char kind[256] = {0};
                // We must prepare the chord quality and convert it for irealpro:
                // 1. concatenate every degrees and qual into kind
                // 2. hashtable lookup for the irealpro chord quality.
                // memcpy(kind, msr_state->curr_chord.qual.buf, msr_state->curr_chord.qual.len);

                // we copy the content of what we found to the current chord.
                IrpChord* c = GET_CURR_CHORD(parser_data);
                c->root = msr_state->curr_chord.root;
                c->bass = msr_state->curr_chord.bass;

                return PARSER_STOP;
            }
        }
        case XML_SELF_CLOSING:
    }
    return PARSER_CONTINUE;
}

static int parse_note(void *user_data, sax_context *context)
{
    const xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_cmp(&n->target, &musicxml.duration)) {
                ParserData *parser_data = (ParserData*)user_data;
                IrpMeasure *m = GET_CURR_MEASURE(parser_data);
                char note_duration_buf[128] = {0};

                if (sax_copy_content(context, note_duration_buf, 128) != 0) return PARSER_STOP_ERROR;

                m->chords.items[m->chords.count].duration += (double)strtod(note_duration_buf, NULL);
            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        }
        break;
        case XML_TAG_CLOSE:
        {
            if (str_ref_cmp(&n->target, &musicxml.note)) return PARSER_STOP;
        }
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
    IrpMeasure *m = GET_CURR_MEASURE(parser_data);
    MsrState *state = (MsrState*)parser_data->state;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (m->chords.count <= IRP_MAX_CHORDS && str_ref_cmp(&n->target, &musicxml.note)) { 
                if (sax_parse_xml(parse_note, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (!state->attributes && str_ref_cmp(&n->target, &musicxml.attributes)) { 
                state->attributes = true;
                if (sax_parse_xml(parse_attributes, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (str_ref_cmp(&n->target, &musicxml.direction)) { 
                if (sax_parse_xml(parse_direction, parser_data, context) != 0) return PARSER_STOP_ERROR;

            } else if (str_ref_cmp(&n->target, &musicxml.harmony)) { 
                // chords are already allocated, there is no dynamic array.
                // chords.count indicate the number of chords inside the bar, but it will not parse more dans IRP_MAX_CHORDS.
                // count act as a flag for note element parsing if there is more than IRP_MAX_CHORDS chords
                // Also, the first chords[0] act as empty chord when there is no chords in the begining of the bar but only notes
                m->chords.count++;
                if (m->chords.count < IRP_MAX_CHORDS ) { 
                    if (sax_parse_xml(parse_harmony, parser_data, context) != 0) return PARSER_STOP_ERROR;
                }

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
                if (sax_copy_content(context, parser_data->song->title, IRP_MAX_CREDENTIALS) != 0) return PARSER_STOP_ERROR;

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
                if (sax_copy_content(context, parser_data->song->composer, IRP_MAX_CREDENTIALS) != 0) return PARSER_STOP_ERROR;

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
