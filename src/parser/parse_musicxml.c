#include "parser.h"

int parse_part(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (str_ref_eq(&n->target, &musicxml.measure)) { 
                da_append(parser_state->song->measures, (t_measure){0});
				memset(&parser_state->tmp_chord, 0, sizeof(t_mxl_chord));
                if (sax_parse_xml(parse_measure, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            }
        	break;
        }
        case XML_TAG_CLOSE:
        {
            if (str_ref_eq(&n->target, &musicxml.part)) {
				// Continue to check for another eventual part if no chords found.
				if (parser_state->song->measures.count != 0)
					return PARSER_STOP;
			}
        }
        default: break;
    }
    return PARSER_CONTINUE;
}

int parse_work(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (parser_state->song->title[0] == '\0' && str_ref_eq(&n->target, &musicxml.work_title)) { 
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

int	parse_credit(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;

    switch (n->type) {
        case XML_TAG_OPEN:
        {
			if (str_ref_eq(&n->target, &musicxml.credit_type)) {
				da_str_ref content;
				if (sax_get_content(context, &content) != 0)
					return PARSER_STOP_ERROR;
				if (str_ref_eq(&content, &musicxml.composer) && parser_state->song->composer[0] == '\0') {
					parser_state->credit_type_is_composer = true;
				} else if (str_ref_eq(&content, &musicxml.title) && parser_state->song->title[0] == '\0') {
					parser_state->credit_type_is_title = true;
				}
			} else if (str_ref_eq(&n->target, &musicxml.credit_words)) {
				if (parser_state->credit_type_is_title) {
					if (sax_copy_content(context, parser_state->song->title, MAX_CREDENTIALS) != 0)
						return PARSER_STOP_ERROR;
					parser_state->credit_type_is_title = false;
				} else if (parser_state->credit_type_is_composer) {
					if (sax_copy_content(context, parser_state->song->composer, MAX_CREDENTIALS) != 0)
						return PARSER_STOP_ERROR;
					parser_state->credit_type_is_composer = false;
				}
			} else {
				return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
			}
			break;
		}
		case XML_TAG_CLOSE:
		{
			if (str_ref_eq(&n->target, &musicxml.credit)) {
				parser_state->credit_type_is_composer = false;
				parser_state->credit_type_is_title = false;
				return PARSER_STOP;
			}
		}
		default: break; 
	}
	return PARSER_CONTINUE;
}

int parse_identification(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
    const t_xml_node *n = &context->found;
    switch (n->type) {
        case XML_TAG_OPEN:
        {
            if (parser_state->song->composer[0] == '\0'
                && str_ref_eq(&n->target, &musicxml.creator)
                && n->attrc >= 1 
                && str_ref_eq(&n->attrv[0].key, &musicxml.type)
                && str_ref_eq(&n->attrv[0].value, &musicxml.composer)
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

int parse_song_partwise(void *user_data, t_sax_context *context)
{
	t_parser_state *parser_state = (t_parser_state *)user_data;
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
        } else if (str_ref_eq(&n->target, &musicxml.credit)) {
            if (sax_parse_xml(parse_credit, parser_state, context) != 0)
                return PARSER_STOP_ERROR;
        } else if (str_ref_eq(&n->target, &musicxml.part)) {
            if (sax_parse_xml(parse_part, parser_state, context) != 0)
					return PARSER_STOP_ERROR;
            return PARSER_STOP;
        } else {
            return PARSER_CONTINUE | SKIP_ENTIRE_NODE;
        }
    }
    return PARSER_CONTINUE;
}

t_mxl2irp_result	*parse_musicxml(const char* musicxml, const size_t musicxml_length) {
	t_irealpro_song *irp_song = calloc(1, sizeof(t_irealpro_song));
	t_mxl2irp_result *result = calloc(1, sizeof(t_mxl2irp_result));
	if (!irp_song || !result)
		return NULL;
    t_parser_state parser_state = {
		.result = result,
        .song = irp_song,
        .part_selected = 1,
    };

    t_sax_scanner scanner = sax_scanner_init(musicxml, musicxml_length);
    t_sax_context context = sax_context_init(&scanner);

	if (sax_parse_xml(parse_song_partwise, &parser_state, &context) != 0) {
		if (result->error_code == ERROR_UNSET)
			result->error_code = ERROR_XML_FILE_CORRUPT;
		free(irp_song);
		return result;
	}
	if (irp_song->measures.count == 0) {
		result->error_code = ERROR_EMPTY_SCORE;
		free(irp_song);
		return result;
	}
	if (irp_song->zoom == ZOOM_IN) {
		t_measure *tmp = realloc(irp_song->measures.items, sizeof(t_measure) * irp_song->measures.count * 2);
		if (tmp == NULL) {
			free(irp_song);
			return NULL;
		}
		irp_song->measures.items = tmp;
		irp_song->measures.capacity = irp_song->measures.count * 2;
	}
	irp_song->first_measure = &irp_song->measures.items[0];
	irp_song->first_measure = &irp_song->measures.items[0];
	irp_song->first_measure->next = NULL;
	for (size_t i = 0; i < irp_song->measures.count - 1; i++) {
		irp_song->measures.items[i].next = &irp_song->measures.items[i + 1];
	}
	irp_song->measures.items[irp_song->measures.count - 1].next = NULL;

	irp_song_cleanup_and_factor_out(irp_song);
	if (irp_song_apply_zoom(irp_song) != 0) {
		result->error_code = ERROR_ZOOM_FAILED;
		free(irp_song);
		return result;
	}
	result->item = irp_song;
    return result;
}
