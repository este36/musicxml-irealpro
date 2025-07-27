#include "parser.h"

int parse_note(t_parser_state *parser_state, t_sax_context *context)
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

int parse_direction(t_parser_state *parser_state, t_sax_context *context)
{
	const t_xml_node *n = &context->found;
	t_measure	*m = GET_CURR_MEASURE(parser_state);

	switch (n->type) {
		case XML_TAG_OPEN:
		{
			if (m->rehearsal[0] == '\0'
				&& str_ref_eq(&n->target, &musicxml.rehearsal)) {
				if (sax_copy_content(context, m->rehearsal, MAX_REHEARSAL_LEN) != 0)
					return PARSER_STOP_ERROR;
			} else if (str_ref_eq(&n->target, &musicxml.segno)) {
				m->is_segno = true;
			} else if (str_ref_eq(&n->target, &musicxml.coda)) {
				m->is_coda = true;
			} else if (m->playback[0] == '\0'
						&& str_ref_eq(&n->target, &musicxml.words)) {
				if (sax_copy_content(context, m->playback, MAX_PLAYBACK_LEN) != 0)
					return PARSER_STOP_ERROR;
			} else if (!str_ref_eq(&n->target, &musicxml.direction_type)) {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
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

int parse_barline(t_parser_state *parser_state, t_sax_context *context)
{
	const t_xml_node *n = &context->found;
	t_measure	*m = GET_CURR_MEASURE(parser_state);
	char content[128];
	da_str_ref val;

    switch (n->type) {
		case XML_SELF_CLOSING:
		{
			if (str_ref_eq(&n->target, &musicxml.repeat)) {
				if (sax_get_attrv(context, &val, "direction") == 0) {
					if (strncmp(val.buf, "backward", val.len) == 0) {
						if (m->chords.count >= 1)
							m->barlines[1] = '}';
						else if (parser_state->song->measures.count > 1)
							(m - 1)->barlines[1] = '}';
					} else if (strncmp(val.buf, "forward", val.len) == 0) {
						m->barlines[0] = '{';
					}
				}
			}
			break;
		}
        case XML_TAG_OPEN:
		{
			if (str_ref_eq(&n->target, &musicxml.bar_style)) {
				if (sax_copy_content(context, content, 128) != 0)
					return PARSER_STOP_ERROR;
				if (strcmp(content, "heavy-light") == 0)
					m->barlines[0] = '[';
				else if (strcmp(content, "light-heavy") == 0)
					m->barlines[1] = ']';
				else if (strcmp(content, "heavy-heavy") == 0)
					m->barlines[1] = 'Z';
			} else if (str_ref_eq(&n->target, &musicxml.coda)) {
				m->is_coda = true;
			} else if (str_ref_eq(&n->target, &musicxml.segno)) {
				m->is_segno = true;
			} else if (str_ref_eq(&n->target, &musicxml.ending)) {
				if (sax_get_attrv(context, &val, "number") == 0) {
					if (val.buf[0] - '0' > 0 && val.buf[0] - '0' < 4
						&& val.buf[1] != ',')
						m->ending = val.buf[0] - '0';
				}
			} else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
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

// chords are already allocated, there is no dynamic array.
// chords.count indicate the number of chords inside the bar,
// but it will not parse more dans IRP_MAX_CHORDS.
// count act as a flag for note element parsing if there is more than IRP_MAX_CHORDS chords
// Also, the first chords[0] act as empty chord when there is no chords
// in the begining of the bar but only notes
int parse_measure(t_parser_state *parser_state, t_sax_context *context)
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
            } else if (str_ref_eq(&n->target, &musicxml.attributes)) {
                if (sax_parse_xml(parse_attributes, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.direction)) {
				if (sax_parse_xml(parse_direction, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.harmony)) { 
                m->chords.count++;
                if (m->chords.count < MAX_CHORDS
					&& sax_parse_xml(parse_harmony, parser_state, context) != 0)
                	return PARSER_STOP_ERROR;
            } else if (str_ref_eq(&n->target, &musicxml.barline)) {
				if (sax_parse_xml(parse_barline, parser_state, context) != 0)
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

