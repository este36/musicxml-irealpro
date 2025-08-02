#include "parser.h"
#include <strings.h>

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

RehearsalEnum get_rehearsal_enum(char *xml_content)
{
	if (strcasecmp(xml_content, "intro") == 0)
		return (REHEARSAL_INTRO);
	if (strcasecmp(xml_content, "in") == 0)
		return (REHEARSAL_INTRO);
	if (strcasecmp(xml_content, "verse") == 0)
		return (REHEARSAL_VERSE);
	else if (strcasecmp(xml_content, "A") == 0)
		return (REHEARSAL_A);
	else if (strcasecmp(xml_content, "B") == 0)
		return (REHEARSAL_B);
	else if (strcasecmp(xml_content, "C") == 0)
		return (REHEARSAL_C);
	else if (strcasecmp(xml_content, "D") == 0)
		return (REHEARSAL_D);
	return (REHEARSAL_NONE);
}

PlaybackEnum get_playback_enum(char *xml_content)
{
	if (strcasecmp(xml_content, "fine") == 0)
		return (PLAYBACK_FINE);
	else if (strcasecmp(xml_content, "d.c al coda") == 0)
		return (PLAYBACK_DC_AL_CODA);
	else if (strcasecmp(xml_content, "d.c al fine") == 0)
		return (PLAYBACK_DC_AL_FINE);
	else if (strcasecmp(xml_content, "d.s al coda") == 0)
		return (PLAYBACK_DS_AL_CODA);
	else if (strcasecmp(xml_content, "d.s al fine") == 0)
		return (PLAYBACK_DS_AL_FINE);
	return (PLAYBACK_NONE);
}

int parse_direction(t_parser_state *parser_state, t_sax_context *context)
{
	const t_xml_node *n = &context->found;
	t_measure	*m = GET_CURR_MEASURE(parser_state);

	switch (n->type) {
		case XML_TAG_OPEN:
		{
			char buf[128];

			if (str_ref_eq(&n->target, &musicxml.rehearsal)) {
				if (sax_copy_content(context, buf, 128) != 0)
					return PARSER_STOP_ERROR;
				m->rehearsal = get_rehearsal_enum(buf);
			} else if (str_ref_eq(&n->target, &musicxml.segno)) {
				m->playback = PLAYBACK_SEGNO;
			} else if (str_ref_eq(&n->target, &musicxml.coda)) {
				m->playback = PLAYBACK_CODA;
			} else if (str_ref_eq(&n->target, &musicxml.words)) {
				if (sax_copy_content(context, buf, 128) != 0)
					return PARSER_STOP_ERROR;
				m->playback = get_playback_enum(buf);
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
						m->barlines[1] = '}';
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
			} else if (str_ref_eq(&n->target, &musicxml.segno)) {
				m->playback = PLAYBACK_SEGNO;
			} else if (str_ref_eq(&n->target, &musicxml.coda)) {
				m->playback = PLAYBACK_CODA;
			} else if (str_ref_eq(&n->target, &musicxml.ending)) {
				if (sax_get_attrv(context, &val, "number") == 0) {
					if (val.buf[0] - '0' > 0 && val.buf[0] - '0' < ENDING_MAX
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
				if (m->chords.count + 1 > MAX_CHORDS)
                	return PARSER_STOP_ERROR;
                m->chords.count++;
                if (sax_parse_xml(parse_harmony, parser_state, context) != 0)
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
            if (str_ref_eq(&n->target, &musicxml.measure)) {
				if (m->chords.count > MAX_CHORDS)
					return PARSER_STOP_ERROR;
				if (parser_state->song->measures.count > 1) {
					m->last = m - 1;
					(m - 1)->next = m;
				} else if (m->chords.items[0].root == NOTE_UNVALID
                           && parser_state->song->measures.count == 1) {
					parser_state->song->measures.count = 0;
					parser_state->song->first_empty_bars += 1;
				}
				if (m->time_signature.beats == 0 && m->time_signature.beat_type == 0)
					m->time_signature = parser_state->tmp_time_signature;
				else 
					parser_state->tmp_time_signature = m->time_signature;
				if (m->divisions == 0)
					m->divisions = parser_state->tmp_divisions;
				else
					parser_state->tmp_divisions = m->divisions;
				return PARSER_STOP;
			}
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

