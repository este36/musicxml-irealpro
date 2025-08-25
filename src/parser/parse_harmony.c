#include "parser.h"

int parse_degree(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
    t_mxl_degree *curr_deg = GET_TMP_CURR_DEGREE(parser_state);

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_eq(&n->target, &musicxml.degree_value)) {
                int val;
                if (sax_get_int(context, &val) != 0)
					return PARSER_STOP_ERROR;
                curr_deg->value = val;
            } else if (curr_deg->value != 0 && str_ref_eq(&n->target, &musicxml.degree_alter)) {
                int alter;
                if (sax_get_int(context, &alter) != 0)
					return PARSER_STOP_ERROR;
                curr_deg->alter = alter;
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

const struct keyword *get_keyword_from_chord(t_mxl_chord *c)
{
	// We must prepare the chord quality and convert it for irealpro:
	// 1. concatenate every degrees and qual into kind
	// 2. hashtable lookup for the irealpro chord quality.
	char kind[256] = {0};
	char *ref = kind;
	memcpy(kind, c->qual.buf, c->qual.len);
	ref += strlen(ref);
	for (uint32_t i = 0; i < c->degrees_count; i++) {
		// -4 bc we can append 4 chars at once
		size_t len_left = (ref - kind) < (256 - 4) ? 256 - (ref - kind) : 0;
		if (len_left == 0)
			break ;
		if (c->degrees[i].alter > 0)
			strcpy(ref, "#");
		else if (c->degrees[i].alter < 0)
			strcpy(ref, "b");
		ref += strlen(ref);
		char nb[3] = {0};
		if (c->degrees[i].value > 10) {
			nb[0] = (c->degrees[i].value / 10) + '0';
			nb[1] = (c->degrees[i].value % 10) + '0';
		} else {
			nb[0] = c->degrees[i].value + '0';
		}
		strcpy(ref, nb);
		ref += strlen(ref);
	}
	return irealpro_chord_lookup(kind, ref - kind);
}

int parse_harmony(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
	t_mxl_chord *c = &parser_state->tmp_chord;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            // <degree> element is checked first because there can be a lot of them inside <harmony> element.
            if (str_ref_eq(&n->target, &musicxml.degree)) {
				if (c->degrees_count + 1 > TMP_CHORD_MAX_DEGREES) {
					parser_state->result->error_code = ERROR_TOO_MUCH_DEGREES;
					return PARSER_STOP_ERROR;
				}
                c->degrees_count++;
                if (sax_parse_xml(parse_degree, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
				t_mxl_degree *curr_deg = GET_TMP_CURR_DEGREE(parser_state);
				if (curr_deg->value == 3) {
					curr_deg->value = 0;
					curr_deg->alter = 0;
                	c->degrees_count--;
				}
			} else if (str_ref_eq(&n->target, &musicxml.kind)) {
				if (sax_get_content(context, &c->qual) != 0)
					return PARSER_STOP_ERROR; 
            } else if (str_ref_eq(&n->target, &musicxml.root_step)) {
                c->root = GET_NOTE_STEP();
            } else if (str_ref_eq(&n->target, &musicxml.bass_step)) {
                c->bass = GET_NOTE_STEP(); // same as root-step
            } else if (str_ref_eq(&n->target, &musicxml.root_alter)) {
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                if (strncmp(alter_str, "1", 1) == 0)
					c->root += 1;
                else if (strncmp(alter_str, "-1", 2) == 0)
					c->root -= 1;
            } else if (str_ref_eq(&n->target, &musicxml.bass_alter)) {
                // same logic as root-alter
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                if (strncmp(alter_str, "1", 1) == 0)
					c->bass += 1;
                else if (strncmp(alter_str, "-1", 2) == 0)
					c->bass -= 1;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.harmony)) {
				int musicxml_chords_index = -1;
				while (1)
				{
					const struct keyword *kw = get_keyword_from_chord(c);
					if (kw != NULL) {
						musicxml_chords_index = kw->id;
						break ;
					} else if (c->degrees_count == 0) {
						break ;
					}
					c->degrees_count--;
				}
				if (musicxml_chords_index == -1) {
					parser_state->result->error_code = ERROR_UNVALID_CHORD_KIND;
					return PARSER_STOP_ERROR;
				}
                // we copy the content of what we found to the current chord.
                t_chord* curr = GET_CURR_CHORD(parser_state);
                curr->root = c->root;
                curr->bass = c->bass;
				strcpy(curr->quality, musicxml_chords[musicxml_chords_index]);
				memset(c, 0, sizeof(*c));
                return PARSER_STOP;
            }
			break;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

