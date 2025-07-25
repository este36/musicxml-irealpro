#include "parser.h"

int parse_degree(t_parser_state *parser_state, t_sax_context *context)
{
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

int parse_harmony(t_parser_state *parser_state, t_sax_context *context)
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
			} else if (str_ref_eq(&n->target, &musicxml.kind)) {
				if (sax_get_content(context, &msr_state->tmp_chord.qual) != 0)
					return PARSER_STOP_ERROR; 
            } else if (str_ref_eq(&n->target, &musicxml.root_step)) {
                msr_state->tmp_chord.root = GET_NOTE_STEP();
            } else if (str_ref_eq(&n->target, &musicxml.bass_step)) {
                msr_state->tmp_chord.bass = GET_NOTE_STEP(); // same as root-step
            } else if (str_ref_eq(&n->target, &musicxml.root_alter)) {
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                if (strncmp(alter_str, "1", 1) == 0)
					msr_state->tmp_chord.root += 1;
                else if (strncmp(alter_str, "-1", 2) == 0)
					msr_state->tmp_chord.root -= 1;
            }
            else if (str_ref_eq(&n->target, &musicxml.bass_alter)) {
                // same logic as root-alter
                char alter_str[4] = {0};
                if (sax_copy_content(context, alter_str, 4) != 0)
					return PARSER_STOP_ERROR;
                if (strncmp(alter_str, "1", 1) == 0)
					msr_state->tmp_chord.bass += 1;
                else if (strncmp(alter_str, "-1", 2) == 0)
					msr_state->tmp_chord.bass -= 1;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.harmony)) {
                // We must prepare the chord quality and convert it for irealpro:
                // 1. concatenate every degrees and qual into kind
                // 2. hashtable lookup for the irealpro chord quality.
                char kind[256] = {0};
				char *ref = kind;
                memcpy(kind, msr_state->tmp_chord.qual.buf, msr_state->tmp_chord.qual.len);
				ref += strlen(ref);
				for (uint32_t i = 0; i < msr_state->tmp_chord.degrees_count; i++) {
					// -4 bc we can append 4 chars at once
					size_t len_left = (ref - kind) < (256 - 4) ? 256 - (ref - kind) : 0;
					if (len_left == 0)
						break ;
					if (msr_state->tmp_chord.degrees[i].alter > 0)
						strcpy(ref, "#");
					else if (msr_state->tmp_chord.degrees[i].alter < 0)
						strcpy(ref, "b");
					ref += strlen(ref);
					char nb[3] = {0};
					if (msr_state->tmp_chord.degrees[i].value > 10) {
						nb[0] = (msr_state->tmp_chord.degrees[i].value / 10) + '0';
						nb[1] = (msr_state->tmp_chord.degrees[i].value % 10) + '0';
					} else {
						nb[0] = msr_state->tmp_chord.degrees[i].value + '0';
					}
					strcpy(ref, nb);
					ref += strlen(ref);
				}
				const struct keyword *kw = irealpro_chord_lookup(kind, ref - kind);
				if (kw == NULL)
					return PARSER_STOP_ERROR;
                // we copy the content of what we found to the current chord.
                t_chord* c = GET_CURR_CHORD(parser_state);
                c->root = msr_state->tmp_chord.root;
                c->bass = msr_state->tmp_chord.bass;
				strcpy(c->quality, musicxml_chords[kw->id]);
				memset(&parser_state->tmp_msr.tmp_chord, 0 , sizeof(t_mxl_chord));
                return PARSER_STOP;
            }
			break;
        }
		default: break;
    }
    return PARSER_CONTINUE;
}

