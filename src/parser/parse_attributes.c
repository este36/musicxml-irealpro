#include "parser.h"

int parse_time(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
	t_measure *m = GET_CURR_MEASURE(parser_state);

    switch (n->type) {
        case XML_TAG_OPEN:
        {
			int res;
			if (str_ref_eq(&n->target, &musicxml.beats)) {
                if (sax_get_int(context, &res) != 0)
					return PARSER_STOP_ERROR;
				if (res <= 1)
					return PARSER_STOP_ERROR;
				m->time_signature.beats = (uint32_t)res;
			} else if (str_ref_eq(&n->target, &musicxml.beat_type)) {
                if (sax_get_int(context, &res) != 0)
					return PARSER_STOP_ERROR;
				if (res <= 1)
					return PARSER_STOP_ERROR;
				m->time_signature.beat_type = (uint32_t)res;
			} else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.time)) {
				uint32_t b = m->time_signature.beats;
				uint32_t bt = m->time_signature.beat_type;
				if (b && bt) {
					if (bt % 2 != 0)
						return PARSER_STOP_ERROR;
					if (parser_state->song->zoom == ZOOM_NONE
						&& is_unvalid_time_signature(b, bt)) {
						// try to zoom in or out
						if (!is_unvalid_time_signature(b, bt*2))
							parser_state->song->zoom = ZOOM_OUT;
						else if (!is_unvalid_time_signature(b, bt/2))
							parser_state->song->zoom = ZOOM_IN;
					}
				} else if (b != bt)
					return PARSER_STOP_ERROR;
				return PARSER_STOP;
			}
			break;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

NoteEnum get_key_from_fifths(int mxl_fifths)
{
	int	alter = 0;
	const NoteEnum fifths[] = {
		NOTE_C, NOTE_G, NOTE_D, NOTE_A,
		NOTE_E, NOTE_B, NOTE_F, NOTE_C
	};

	if (mxl_fifths < -7 || mxl_fifths > 7)
		return NOTE_UNVALID;
	if (mxl_fifths < 0) {
		if (mxl_fifths < -1)
			alter = -1;
		mxl_fifths += 7;
	} else if (mxl_fifths > 5) {
		alter = 1;
	}
	return (fifths[mxl_fifths] + alter);
}

int parse_key(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
	int res;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
			if (str_ref_eq(&n->target, &musicxml.fifths)) {
                if (sax_get_int(context, &res) != 0)
					return PARSER_STOP_ERROR;
				NoteEnum key = get_key_from_fifths(res);
				if (key == NOTE_UNVALID)
					return PARSER_STOP_ERROR;
				parser_state->song->key = key;
			} else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
			break;
		}
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.key))
				return PARSER_STOP;
			break;
        }
		default: break;
    }
	return PARSER_CONTINUE;
}


int parse_attributes(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
	int res;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
			if (str_ref_eq(&n->target, &musicxml.time)) {
                if (sax_parse_xml(parse_time, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
			} else if (str_ref_eq(&n->target, &musicxml.divisions)) {
                if (sax_get_int(context, &res) != 0)
					return PARSER_STOP_ERROR;
				if (res <= 0)
					return PARSER_STOP_ERROR;
				GET_CURR_MEASURE(parser_state)->divisions = res;
			} else if (parser_state->song->key == NOTE_UNVALID
					&& str_ref_eq(&n->target, &musicxml.key)) {
                if (sax_parse_xml(parse_key, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
			} else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
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

