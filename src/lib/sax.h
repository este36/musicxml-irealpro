#ifndef __SAX_H__
#define __SAX_H__

#include <stdio.h>
#include <ctype.h>

#include "da.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#define INLINE static inline
#else
#define INLINE static
#endif

typedef struct sax_lexer {
    size_t pos;
    da_string_ref xml;
}sax_lexer;

#define ADVANCE(lexer) (lexer)->pos++
#define IS_EOF(lexer) ((lexer)->pos >= (lexer)->xml.len)
#define GET_CHAR(lexer) (((lexer)->xml.buf[(lexer)->pos]))
#define GET_PTR(lexer) ((lexer)->xml.buf + ((lexer)->pos))
#define GET_NEXT_CHAR(lexer) (((lexer)->pos + 1 >= (lexer)->xml.len) ? '\0': ((lexer)->xml.buf[(lexer)->pos + 1]))
#define GET_PREV_CHAR(lexer) (((lexer)->xml.buf[(lexer)->pos -1]))
#define SKIP_UNTIL(lexer, c) while(!IS_EOF(lexer) && GET_CHAR(lexer) != c) ADVANCE(lexer)
#define SKIP_WHILE(lexer, fn) while(!IS_EOF(lexer) && fn(GET_CHAR(lexer))) ADVANCE(lexer)
#define SKIP_WHILE_NOT(lexer, fn) while(!IS_EOF(lexer) && !fn(GET_CHAR(lexer))) ADVANCE(lexer)

typedef enum xml_node_type {
    XML_UNSET,
    XML_TAG_OPEN,
    XML_TAG_CLOSE,
    XML_SELF_CLOSING,
} xml_node_type;

#ifndef XML_MAX_ATTRIBUTES
#define XML_MAX_ATTRIBUTES 32
#endif

typedef struct xml_attribute {
    da_string_ref key;
    da_string_ref value;
} xml_attribute;

typedef struct xml_node {
    xml_node_type type;
    da_string_ref target; // name or text content
    xml_attribute attrv[XML_MAX_ATTRIBUTES];
    size_t attrc;
} xml_node;

INLINE void xml_clear_node(xml_node* n)
{
    n->type = XML_UNSET;
    n->attrc = 0;
    n->target.buf = NULL;
    n->target.len = 0;
};

typedef struct sax_context {
    sax_lexer* lexer;
    xml_node found;
} sax_context;

// orders/infos to talk to backend parser
// usage : PARSER_CONTINUE | NODE_IS_VARIABLE
#define NODE_IS_VARIABLE   0x01
#define NODE_IS_PARENT     0x02

#define PARSER_STATUS_MASK 0x30
#define PARSER_CONTINUE    0x00
#define PARSER_STOP        0x10
#define PARSER_STOP_ERROR  0x20

#define XML_FILE_CORRUPT 1

INLINE sax_lexer sax_lexer_init(const char* buffer, size_t length)
{
    return (sax_lexer){ .xml = { .buf = buffer, .len = length}, .pos = 0};
}

INLINE sax_context sax_context_init(sax_lexer* l)
{
    return (sax_context){ .lexer = l, .found = {0} };
}

#define SKIP_VALID_MXL_NAME(lexer) while (!IS_EOF(lexer) && (isalpha(GET_CHAR(lexer)) || GET_CHAR(lexer) == '-')) ADVANCE(lexer)

// this will search for next closing tag of the context.found node.
// So the context.found is exepted to be a container of either text or other child elements, especially the opening tag of this container.
// the lexer is expected to be at the begining of the content, after the > char of opening tag.
// the user of this function is supposed to be sure that the element is a container.
INLINE int sax_get_content(sax_context* context, da_string_ref* str_ref)
{
    str_ref->buf = GET_PTR(context->lexer);

    // now we try to find a closing tag with the same name.
    while (!IS_EOF(context->lexer)) 
    {
        if (GET_CHAR(context->lexer) == '<' && GET_NEXT_CHAR(context->lexer) == '/') {
            // update the len
            str_ref->len = GET_PTR(context->lexer) - str_ref->buf;

            ADVANCE(context->lexer);
            ADVANCE(context->lexer);

            da_string_ref name = {0};
            name.buf = GET_PTR(context->lexer);
            SKIP_VALID_MXL_NAME(context->lexer);
            if (IS_EOF(context->lexer) || GET_CHAR(context->lexer) != '>') return XML_FILE_CORRUPT;

            name.len = GET_PTR(context->lexer) - name.buf;

            ADVANCE(context->lexer);
            if (str_ref_cmp(&context->found.target, &name)) {
                return 0;
            }
        } 
        ADVANCE(context->lexer);
    }
    return XML_FILE_CORRUPT;
}


// It should not get the entire tag, only name and attributes.
// Because the closing /> or > is parsed after.
// It must end on last white space or " (end attribute quote)
INLINE int sax_parse_tag_body(sax_context* context) // TODO
{
    da_string_ref* name = &context->found.target;

    // get name
    name->buf = GET_PTR(context->lexer);

    ADVANCE(context->lexer);
    SKIP_VALID_MXL_NAME(context->lexer);
    if (IS_EOF(context->lexer)) return XML_FILE_CORRUPT;

    name->len = GET_PTR(context->lexer) - name->buf;

    SKIP_WHILE(context->lexer, isspace);
    // no need to check if EOF because while loop checks it

    size_t attrc = 0;
    while (!IS_EOF(context->lexer) && attrc < XML_MAX_ATTRIBUTES) {
        xml_attribute* a = &context->found.attrv[attrc];

        // get key 
        a->key.buf = GET_PTR(context->lexer);
        SKIP_VALID_MXL_NAME(context->lexer);
        if (IS_EOF(context->lexer)) return XML_FILE_CORRUPT;
        a->key.len = GET_PTR(context->lexer) - a->key.buf;

        // check if no key found
        if (a->key.len == 0) break;

        // no orphan attribute is valid in musicxml
        if (GET_CHAR(context->lexer) != '=') return XML_FILE_CORRUPT;
        ADVANCE(context->lexer);

        if (GET_CHAR(context->lexer) != '"') return XML_FILE_CORRUPT;
        ADVANCE(context->lexer);

        a->value.buf = GET_PTR(context->lexer);
        // " chars are escaped only with &quot so its safe.
        SKIP_UNTIL(context->lexer, '"');
        if (IS_EOF(context->lexer)) return XML_FILE_CORRUPT;

        a->value.len = GET_PTR(context->lexer) - a->value.buf;
        ADVANCE(context->lexer);
        attrc++;

        SKIP_WHILE(context->lexer, isspace);
    }

    // update attributes count
    context->found.attrc = attrc;

    return IS_EOF(context->lexer) ? XML_FILE_CORRUPT : 0;
}


INLINE int sax_parse_xml(int (*fn)(void* user_data, sax_context* ctxt), void* user_data, sax_context* context) 
{
    while (!IS_EOF(context->lexer)) {
        if (GET_CHAR(context->lexer) == '<') { // we found a tag
            int status = PARSER_CONTINUE;
            char nc = GET_NEXT_CHAR(context->lexer);

            if (nc == '/') { // we found a closing tag
                ADVANCE(context->lexer);
                ADVANCE(context->lexer); 

                // get tag name
                context->found.target.buf = GET_PTR(context->lexer);
                SKIP_VALID_MXL_NAME(context->lexer);
                if (IS_EOF(context->lexer) || GET_CHAR(context->lexer) != '>') return XML_FILE_CORRUPT;
                context->found.target.len = GET_PTR(context->lexer) - context->found.target.buf;

                // we good ! that is valid CLOSING tag
                context->found.type = XML_TAG_CLOSE;
                // now give the data to user
                status = fn(user_data, context);
                xml_clear_node(&context->found);

            } else if (isalpha(nc)) { // continue... (first char of musicxml tags are only alphabetical)
                ADVANCE(context->lexer);
                if (sax_parse_tag_body(context) != 0) return XML_FILE_CORRUPT;

                // now we check for either self closing tag or opening tag

                char closing = GET_CHAR(context->lexer);
                if (closing == '/') { // that is probably a self closing
                    ADVANCE(context->lexer);

                    char closing_end = GET_CHAR(context->lexer);
                    if (closing_end != '>') return XML_FILE_CORRUPT;
                    ADVANCE(context->lexer);
                    // we good ! that is valid SELF_CLOSING tag
                    // now give the data to user
                    context->found.type = XML_SELF_CLOSING;
                    status = fn(user_data, context);
                    xml_clear_node(&context->found);

                } else if (closing == '>') { // this is parent or variable opening tag

                    ADVANCE(context->lexer);
                    // we good ! that is valid OPENING tag
                    // now give the data to user
                    context->found.type = XML_TAG_OPEN;
                    status = fn(user_data, context);
                    xml_clear_node(&context->found);
                } else {
                    return XML_FILE_CORRUPT;
                }
            } else if ( nc == '?' || nc == '!') {
                SKIP_UNTIL(context->lexer, '>');
                // TODO: retourner le contenu dans found.target et creer un event XML_INFO, pour le found.type
            } else {
                return XML_FILE_CORRUPT;
            }

            if ((status & PARSER_STATUS_MASK) == PARSER_STOP) return 0;
            if ((status & PARSER_STATUS_MASK) == PARSER_STOP_ERROR) return XML_FILE_CORRUPT;
        } else {
            ADVANCE(context->lexer);
        }
    }

    return 0;
}

#endif // __SAX_H__
