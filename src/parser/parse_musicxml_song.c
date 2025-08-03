#include "parser.h"

int parse_part(t_parser_state *parser_state, t_sax_context *context)
{
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
            if (str_ref_eq(&n->target, &musicxml.part))
				return PARSER_STOP;
        }
        default: break;
    }
    return PARSER_CONTINUE;
}

int parse_work(t_parser_state *parser_state, t_sax_context *context)
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

int parse_identification(t_parser_state *parser_state, t_sax_context *context)
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

int parse_song_partwise(t_parser_state *parser_state, t_sax_context *context)
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

	if (sax_parse_xml(parse_song_partwise, &parser_state, &context) != 0)
		return XML_FILE_CORRUPT;
	irp_song->first_measure = &irp_song->measures.items[0];
	// irp_song->zoom = ZOOM_OUT;
	if (irp_song_apply_zoom(irp_song) != 0)
		return -1;
    return 0;
}
