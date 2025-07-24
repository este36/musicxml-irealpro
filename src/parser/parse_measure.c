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

int parse_attributes(t_parser_state *parser_state, t_sax_context *context)
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

int parse_direction(t_parser_state *parser_state, t_sax_context *context)
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

int parse_barline(t_parser_state *parser_state, t_sax_context *context)
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

