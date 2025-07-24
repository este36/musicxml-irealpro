#include "parser.h"

int parse_divisions(t_parser_state *parser_state, t_sax_context *context)
{

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

