#include "parser.h"
#include <string.h>

static int parse_degree(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    t_mxl_degree *curr_deg = GET_TMP_CURR_DEGREE(parser_state);

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_eq(&n->target, &musicxml.degree_value)) {
                char value_str[4] = {0};
                if (sax_copy_content(context, value_str, 4) != 0)
					return PARSER_STOP_ERROR;
                int16_t value_int = atoi(value_str);
                // we do not accept additional degrees of third or seventh
                if (value_int % 7 == 3 || value_int % 7 == 0)
					return PARSER_STOP;
                curr_deg->value = value_int;
            } else if (curr_deg->value != 0 && str_ref_eq(&n->target, &musicxml.degree_alter)) {
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                curr_deg->value = atoi(alter_str);
            } else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.degree))
				return PARSER_STOP;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

// A is 65 in ascii code, so a bit of arithmetic and me can get the correct root note.
#define GET_NOTE_STEP() ((NoteEnum)(((int)GET_CHAR(context->scanner) - 'A') * 3 + 2))

static int parse_harmony(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    t_tmp_measure *msr_state = &parser_state->tmp_msr;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            // <degree> element is checked first because there can be a lot of them inside <harmony> element.
            if (msr_state->tmp_chord.degrees_count < TMP_CHORD_MAX_DEGREES
                        && str_ref_eq(&n->target, &musicxml.degree)) {
                msr_state->tmp_chord.degrees_count++;
                if (sax_parse_xml(parse_degree, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.root_step)) {
                // the scanner is just after the > of open tag. root-step content is only one char.
                msr_state->tmp_chord.root = GET_NOTE_STEP();
            }
			else if (str_ref_eq(&n->target, &musicxml.kind)
					&& sax_get_content(context, &msr_state->tmp_chord.qual) != 0)
					return PARSER_STOP_ERROR; 
            else if (str_ref_eq(&n->target, &musicxml.bass_step))
                msr_state->tmp_chord.bass = GET_NOTE_STEP(); // same as root-step
            else if (str_ref_eq(&n->target, &musicxml.root_alter)) {
                // same logic as root-alter
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                if (strncmp(alter_str, "1", 1) == 0)
					msr_state->tmp_chord.root += 1;
                else if (strncmp(alter_str, "-1", 2) == 0)
					msr_state->tmp_chord.root -= 1;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.harmony)) {
                // char kind[256] = {0};
                // We must prepare the chord quality and convert it for irealpro:
                // 1. concatenate every degrees and qual into kind
                // 2. hashtable lookup for the irealpro chord quality.
                // memcpy(kind, msr_state->tmp_chord.qual.buf, msr_state->tmp_chord.qual.len);

                // we copy the content of what we found to the current chord.
                t_chord* c = GET_CURR_CHORD(parser_state);
                c->root = msr_state->tmp_chord.root;
                c->bass = msr_state->tmp_chord.bass;

                return PARSER_STOP;
            }
			break;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_note(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    switch (n->type) {
		case XML_SELF_CLOSING:
		{
			// Chord self-closing tag element is always the first element
			// inside the <note> element and if it is present
			// we skip because the duration is already set
            if (str_ref_eq(&n->target, &musicxml.chord)) {
				if (sax_skip_content(context, musicxml.note) != 0)
					return PARSER_STOP_ERROR;
				return PARSER_STOP;
            }
			break;
		}
        case XML_TAG_OPEN:
        {
			if (str_ref_eq(&n->target, &musicxml.duration)) {
                t_chord* c = GET_CURR_CHORD(parser_state);
                char note_duration_buf[128] = {0};

                if (sax_copy_content(context, note_duration_buf, 128) != 0)
					return PARSER_STOP_ERROR;
                c->duration += strtod(note_duration_buf, NULL);
            } else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.note))
				return PARSER_STOP;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_attributes(t_parser_state *parser_state, t_sax_context *context)
{
	(void)parser_state;
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.attributes))
				return PARSER_STOP;
			break;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_direction(t_parser_state *parser_state, t_sax_context *context)
{
	(void)parser_state;
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        	break;
        }
        case XML_TAG_CLOSE:
        {
           if (str_ref_eq(&n->target, &musicxml.direction))
				return PARSER_STOP;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_barline(t_parser_state *parser_state, t_sax_context *context)
{
	(void)parser_state;
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.barline))
				return PARSER_STOP;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_measure(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    t_measure *m = GET_CURR_MEASURE(parser_state);

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (m->chords.count <= MAX_CHORDS
				&& str_ref_eq(&n->target, &musicxml.note)) {
					if (m->chords.count == 0)
						m->chords.count++;
					if (sax_parse_xml(parse_note, parser_state, context) != 0)
						return PARSER_STOP_ERROR;
            } else if (!parser_state->tmp_msr.is_attributes
				&& str_ref_eq(&n->target, &musicxml.attributes)) {
                parser_state->tmp_msr.is_attributes = true;
                if (sax_parse_xml(parse_attributes, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.direction)) {
					if (sax_parse_xml(parse_direction, parser_state, context) != 0)
						return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.harmony)) { 
                // chords are already allocated, there is no dynamic array.
                // chords.count indicate the number of chords inside the bar,
				// but it will not parse more dans IRP_MAX_CHORDS.
                // count act as a flag for note element parsing if there is more than IRP_MAX_CHORDS chords
                // Also, the first chords[0] act as empty chord when there is no chords
				// in the begining of the bar but only notes
                m->chords.count++;
                if (m->chords.count < MAX_CHORDS
						&& sax_parse_xml(parse_harmony, parser_state, context) != 0)
                	return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.barline)
				   	&& sax_parse_xml(parse_barline, parser_state, context) != 0) { 
					return PARSER_STOP_ERROR;
            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.measure))
				return PARSER_STOP;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_part(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_eq(&n->target, &musicxml.measure)) { 
                da_append(parser_state->song->measures, (t_measure){0});
				memset(&parser_state->tmp_msr, 0, sizeof(t_tmp_measure));
                if (sax_parse_xml(parse_measure, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.part))
				return PARSER_STOP;
        }
        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_work(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (!parser_state->song->title[0] && str_ref_eq(&n->target, &musicxml.work_title)) { 
                if (sax_copy_content(context, parser_state->song->title, MAX_CREDENTIALS) != 0)
					return PARSER_STOP_ERROR;
                // we have what we need, so we skip the rest of parent content
                if (sax_skip_content(context, musicxml.work) != 0)
					return PARSER_STOP_ERROR;
                return PARSER_STOP;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.work))
				return PARSER_STOP;
        }
        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_identification(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
            da_str_ref composer_str = STR_REF("composer");
            if (!parser_state->song->composer[0]
                && str_ref_eq(&n->target, &musicxml.creator)
                && n->attrc >= 1 
                && str_ref_eq(&n->attrv[0].key, &musicxml.type)
                && str_ref_eq(&n->attrv[0].value, &composer_str)
            ) {
                if (sax_copy_content(context, parser_state->song->composer, MAX_CREDENTIALS) != 0)
					return PARSER_STOP_ERROR;
                // we have what we need, so we skip the rest of parent content
                if (sax_skip_content(context, musicxml.identification) != 0)
					return PARSER_STOP_ERROR;
                return PARSER_STOP;
            } else {
                return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.identification))
				return PARSER_STOP;
        }
        default: break;
    }
    return PARSER_CONTINUE;
}

static int parse_song_partwise(t_parser_state *parser_state, t_sax_context *context)
{
    const t_xml_node *n = &context->found;

    if (n->type == XML_TAG_OPEN) {
        if (str_ref_eq(&n->target, &musicxml.score_partwise)) 
            return PARSER_CONTINUE;
        if (str_ref_eq(&n->target, &musicxml.work)) {
            if (sax_parse_xml(parse_work, parser_state, context) != 0)
                return PARSER_STOP_ERROR;
        } else if (str_ref_eq(&n->target, &musicxml.identification)) {
            if (sax_parse_xml(parse_identification, parser_state, context) != 0)
                return PARSER_STOP_ERROR;
        } else if (str_ref_eq(&n->target, &musicxml.part)
                    && n->attrc >= 1
                    && str_ref_eq(&n->attrv[0].key, &musicxml.id) 
                    && str_ref_eq(&n->attrv[0].value, &parser_state->part_selected) 
                ) {
            if (sax_parse_xml(parse_part, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            return PARSER_STOP;
        } else {
            return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
        }
    }

    return PARSER_CONTINUE;
}

// All of irp_song members should be init to zero.
int parse_musicxml_song(t_irealpro_song* irp_song,
						const da_str_ref part_id,
						const char* musicxml,
						const size_t musicxml_length)
{
    t_parser_state parser_state = {
        .song = irp_song,
        .part_selected = part_id,
    };

    t_sax_scanner scanner = sax_scanner_init(musicxml, musicxml_length);
    t_sax_context context = sax_context_init(&scanner);

    return sax_parse_xml(parse_song_partwise, &parser_state, &context);
}
