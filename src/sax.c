#include "parser.h"

void xml_clear_node(t_xml_node *n)
{
    n->type = 0;
    n->attrc = 0;
    n->target.buf = NULL;
    n->target.len = 0;
};

t_sax_scanner sax_scanner_init(const char *buffer, size_t length)
{
    return (t_sax_scanner){ .xml = { .buf = buffer, .len = length}, .pos = 0};
}

t_sax_context sax_context_init(t_sax_scanner *l)
{
    return (t_sax_context){ .scanner = l, .found = {0} };
}

// Suppose that the scanner is inside the parent node
int sax_get_content(t_sax_context *context, da_str_ref *str_ref)
{
    str_ref->buf = GET_PTR(context->scanner);
    // now we try to find a closing tag with the same name.
    while (!IS_EOF(context->scanner)) 
    {
        if (GET_CHAR(context->scanner) == '<' && GET_NEXT_CHAR(context->scanner) == '/') {
            da_str_ref name = {0};

            // update the len
            str_ref->len = GET_PTR(context->scanner) - str_ref->buf;
            ADVANCE(context->scanner);
            ADVANCE(context->scanner);
            name.buf = GET_PTR(context->scanner);
            SKIP_VALID_MXL_NAME(context->scanner);
            if (IS_EOF(context->scanner) || GET_CHAR(context->scanner) != '>')
                return XML_FILE_CORRUPT;
            name.len = GET_PTR(context->scanner) - name.buf;
            ADVANCE(context->scanner);
            if (str_ref_eq(&context->found.target, &name))
                return 0;
        } 
        ADVANCE(context->scanner);
    }
    return XML_FILE_CORRUPT;
}

int sax_copy_content(t_sax_context *context, char *buf, size_t buf_len)
{
    da_str_ref str_ref = {0};
    str_ref.buf = GET_PTR(context->scanner);

    // now we try to find a closing tag with the same name.
    while (!IS_EOF(context->scanner)) 
    {
        if (GET_CHAR(context->scanner) == '<'
		&& GET_NEXT_CHAR(context->scanner) == '/')
		{
            da_str_ref name = {0};

            // update the len
            str_ref.len = GET_PTR(context->scanner) - str_ref.buf;
            ADVANCE(context->scanner);
            ADVANCE(context->scanner);
            name.buf = GET_PTR(context->scanner);
            SKIP_VALID_MXL_NAME(context->scanner);
            if (IS_EOF(context->scanner) || GET_CHAR(context->scanner) != '>')
                return XML_FILE_CORRUPT;
            name.len = GET_PTR(context->scanner) - name.buf;
            ADVANCE(context->scanner);
            if (str_ref_eq(&context->found.target, &name)) {
                // perfom copy str_ref to buf
                size_t copy_len = str_ref.len > buf_len ? buf_len : str_ref.len;
                memcpy(buf, str_ref.buf, copy_len);
                buf[copy_len] = '\0';
                return 0;
            }
        } 
        ADVANCE(context->scanner);
    }
    return XML_FILE_CORRUPT;
}

int	sax_get_int(t_sax_context *context, int *res)
{
	char buf[32] = {0};

	if (sax_copy_content(context, buf, 32) != 0)
		return XML_FILE_CORRUPT;
	*res = atoi(buf);
	return 0;
}

int	sax_get_attrv(t_sax_context *context, da_str_ref *dst, char *key_buf)
{
	size_t i = 0;
	const da_str_ref key = {
		.buf = key_buf,
		.len = strlen(key_buf)
	};

	while (i < context->found.attrc)
	{
		if (str_ref_eq(&context->found.attrv[i].key, &key)) {
			*dst = context->found.attrv[i].value;
			return 0;
		}
		i++;
	}
	return 1;
}

// return 0 if exists, 1 if not found
int	sax_cpy_attrv(t_sax_context *context, char *dst, char *key_buf, size_t dst_len)
{
	size_t i = 0;
	const da_str_ref key = {
		.buf = key_buf,
		.len = strlen(key_buf)
	};

	while (i < context->found.attrc)
	{
		if (str_ref_eq(&context->found.attrv[0].key, &key)) {
			strncpy(dst, context->found.attrv[0].key.buf, dst_len);
			return 0;
		}
		i++;
	}
	return 1;
}

// Suppose that the scanner is inside the parent node
int sax_skip_content(t_sax_context *context, da_str_ref node_name)
{
    while (!IS_EOF(context->scanner)) 
    {
        if (GET_CHAR(context->scanner) == '<'
			&& GET_NEXT_CHAR(context->scanner) == '/')
		{
            da_str_ref name = {0};

            ADVANCE(context->scanner);
            ADVANCE(context->scanner);
            name.buf = GET_PTR(context->scanner);
            SKIP_VALID_MXL_NAME(context->scanner);
            if (IS_EOF(context->scanner) || GET_CHAR(context->scanner) != '>') 
                return XML_FILE_CORRUPT;
            name.len = GET_PTR(context->scanner) - name.buf;
            ADVANCE(context->scanner);
            if (str_ref_eq(&node_name, &name))
                return 0;
        } 
        ADVANCE(context->scanner);
    }
    return XML_FILE_CORRUPT;
}

// It should not get the entire tag, only name and attributes.
// Because the closing /> or > is parsed after.
// It must end on last white space or " (end attribute quote)
int sax_parse_tag_body(t_sax_context *context)
{
	size_t attrc;
    da_str_ref *name = &context->found.target;
    // get name
    name->buf = GET_PTR(context->scanner);
    ADVANCE(context->scanner);
    SKIP_VALID_MXL_NAME(context->scanner);
    if (IS_EOF(context->scanner)) 
        return XML_FILE_CORRUPT;
    name->len = GET_PTR(context->scanner) - name->buf;
    SKIP_WHILE(context->scanner, isspace);

    // no need to check if EOF because while loop checks it
    attrc = 0;
    while (!IS_EOF(context->scanner) && attrc < XML_MAX_ATTRIBUTES) {
        t_xml_attribute* a = &context->found.attrv[attrc];

        // get key 
        a->key.buf = GET_PTR(context->scanner);
        SKIP_VALID_MXL_NAME(context->scanner);
        if (IS_EOF(context->scanner)) 
            return XML_FILE_CORRUPT;
        a->key.len = GET_PTR(context->scanner) - a->key.buf;
        // check if no key found
        if (a->key.len == 0)
            break;
        // no orphan attribute is valid in musicxml
        if (GET_CHAR(context->scanner) != '=')
            return XML_FILE_CORRUPT;
        ADVANCE(context->scanner);
        if (GET_CHAR(context->scanner) != '"')
            return XML_FILE_CORRUPT;
        ADVANCE(context->scanner);
        a->value.buf = GET_PTR(context->scanner);
        // " chars are escaped only with &quot so its safe.
        SKIP_UNTIL(context->scanner, '"');
        if (IS_EOF(context->scanner))
            return XML_FILE_CORRUPT;
        a->value.len = GET_PTR(context->scanner) - a->value.buf;
        ADVANCE(context->scanner);
        attrc++;
        SKIP_WHILE(context->scanner, isspace);
    }
    // update attributes count
    context->found.attrc = attrc;
    return IS_EOF(context->scanner) ? XML_FILE_CORRUPT : 0;
}

int sax_parse_xml(int (*fn)(void *user_data, t_sax_context *ctxt),
					void *user_data, t_sax_context *context) 
{
    while (!IS_EOF(context->scanner)) {
        if (GET_CHAR(context->scanner) == '<') { // we found a tag
            int status = PARSER_CONTINUE;
            char nc = GET_NEXT_CHAR(context->scanner);

            if (nc == '/') { // we found a closing tag
                ADVANCE(context->scanner);
                ADVANCE(context->scanner); 

                // get tag name
                context->found.target.buf = GET_PTR(context->scanner);
                SKIP_VALID_MXL_NAME(context->scanner);
                if (IS_EOF(context->scanner) || GET_CHAR(context->scanner) != '>') 
                    return XML_FILE_CORRUPT;
                context->found.target.len = GET_PTR(context->scanner) - context->found.target.buf;
                context->found.type = XML_TAG_CLOSE;
                // Here we are sure thats a valid CLOSING tag
                status = fn(user_data, context);
                xml_clear_node(&context->found);

            } else if (isalpha(nc)) { // continue... (first char of musicxml tags are only alphabetical)
                ADVANCE(context->scanner);
                if (sax_parse_tag_body(context) != 0)
                    return XML_FILE_CORRUPT;
                // now we check for either self closing tag or opening tag
                char closing = GET_CHAR(context->scanner);
                if (closing == '/') { // that is probably a self closing
                    ADVANCE(context->scanner);
                    char closing_end = GET_CHAR(context->scanner);
                    if (closing_end != '>')
                        return XML_FILE_CORRUPT;
                    ADVANCE(context->scanner);
                    // Here we are sure thats a valid SELF_CLOSING tag
                    context->found.type = XML_SELF_CLOSING;
                    status = fn(user_data, context);
                    xml_clear_node(&context->found);
                } else if (closing == '>') { // this is parent or variable opening tag
                    ADVANCE(context->scanner);
                    // Here we are sure thats a valid OPENING tag
                    context->found.type = XML_TAG_OPEN;
                    status = fn(user_data, context);
                    if (status == SKIP_ENTIRE_NODE) {
                        if (sax_skip_content(context, context->found.target) != 0)
							return XML_FILE_CORRUPT;
					}
                    xml_clear_node(&context->found);
                } else {
                    return XML_FILE_CORRUPT;
                }
            } else if ( nc == '?' || nc == '!') {
                SKIP_UNTIL(context->scanner, '>');
                // TODO: retourner le contenu dans found.target et creer un event XML_INFO, pour le found.type
            } else {
                return XML_FILE_CORRUPT;
            }

            if ((status & PARSER_STATUS_MASK) == PARSER_STOP)
                return 0;
            if ((status & PARSER_STATUS_MASK) == PARSER_STOP_ERROR)
                return XML_FILE_CORRUPT;
        } else {
            ADVANCE(context->scanner);
        }
    }
    return 0;
}
