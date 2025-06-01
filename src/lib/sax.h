#ifndef __SAX_H__
#define __SAX_H__

#include <stdio.h>
#include <ctype.h>

#include "da.h"

#ifdef SAX_DEBUG
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
#define GET_LAST_CHAR(lexer) (((lexer)->xml.buf[(lexer)->pos -1]))
#define SKIP_UNTIL(lexer, c) while(!IS_EOF(lexer) && GET_CHAR(lexer) != c) ADVANCE(lexer)
#define SKIP_WHILE(lexer, fn) while(!IS_EOF(lexer) && fn(GET_CHAR(lexer))) ADVANCE(lexer)
#define SKIP_WHILE_NOT(lexer, fn) while(!IS_EOF(lexer) && !fn(GET_CHAR(lexer))) ADVANCE(lexer)

typedef enum xml_node_type {
    XML_UNSET,
    XML_TAG_OPEN,
    XML_TAG_CLOSE,
    XML_SELF_CLOSING,
    XML_VARIABLE,
    XML_CHARACTERS,
    XML_PARENT
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

#define SKIP_VALID_MXL_NAME() while (!IS_EOF(context->lexer) && (isalpha(GET_CHAR(context->lexer)) || GET_CHAR(context->lexer) == '-')) ADVANCE(context->lexer)

// It should not get the entire tag, only name and attributes.
// Because the closing /> or > is parsed after.
// It must end on last white space or " (end attribute quote)
INLINE int sax_parse_tag_body(sax_context* context) // TODO
{
    da_string_ref* name = &context->found.target;

    // get name
    name->buf = GET_PTR(context->lexer);

    ADVANCE(context->lexer);
    SKIP_VALID_MXL_NAME();
    if (IS_EOF(context->lexer)) return XML_FILE_CORRUPT;

    name->len = GET_PTR(context->lexer) - name->buf;

    SKIP_WHILE(context->lexer, isspace);
    // no need to check if EOF because while loop checks it

    size_t attrc = 0;
    while (!IS_EOF(context->lexer) && attrc < XML_MAX_ATTRIBUTES) {
        xml_attribute* a = &context->found.attrv[attrc];

        // get key 
        a->key.buf = GET_PTR(context->lexer);
        SKIP_VALID_MXL_NAME();
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


INLINE int sax_parse_xml(int (*fn)(void* user_data, sax_context* ctxt), void* user_data, const char* buf, size_t len) 
{
    sax_lexer lexer = { 
        .xml = { 
            .buf = buf, 
            .len = len 
        }, 
        .pos = 0
    };

    sax_context context = {
        .lexer = &lexer,
        .found = {0}
    };

    while (!IS_EOF(&lexer)) {
        if (GET_CHAR(&lexer) == '<') { // we found a tag
            int status = PARSER_CONTINUE;
            char nc = GET_NEXT_CHAR(&lexer);

            if (nc == '/') { // we found a closing tag
                ADVANCE(&lexer);
                ADVANCE(&lexer); 
                if (sax_parse_tag_body(&context) != 0) return XML_FILE_CORRUPT;

                char closing = GET_CHAR(&lexer);
                if (closing != '>') return XML_FILE_CORRUPT; // it is the only option since </.../> is not valid
                ADVANCE(&lexer);
                // we good ! that is valid CLOSING tag
                context.found.type = XML_TAG_CLOSE;
                // now give the data to user
                status = fn(user_data, &context);
                xml_clear_node(&context.found);

            } else if (isalpha(nc)) { // continue... (first char of musicxml tags are only alphabetical)
                ADVANCE(&lexer);
                if (sax_parse_tag_body(&context) != 0) return XML_FILE_CORRUPT;

                // now we check for either self closing tag or opening tag

                char closing = GET_CHAR(&lexer);
                if (closing == '/') { // that is probably a self closing
                    ADVANCE(&lexer);

                    char closing_end = GET_CHAR(&lexer);
                    if (closing_end != '>') return XML_FILE_CORRUPT;
                    ADVANCE(&lexer);
                    // we good ! that is valid SELF_CLOSING tag
                    // now give the data to user
                    context.found.type = XML_SELF_CLOSING;
                    status = fn(user_data, &context);
                    xml_clear_node(&context.found);

                } else if (closing == '>') { // this is parent or variable opening tag

                    ADVANCE(&lexer);
                    // we good ! that is valid OPENING tag
                    // now give the data to user
                    context.found.type = XML_TAG_OPEN;
                    status = fn(user_data, &context);
                    xml_clear_node(&context.found);

                    if (status & NODE_IS_VARIABLE) { // now alls the next choped characters have a value
                        ADVANCE(&lexer);
                        context.found.target.buf = GET_PTR(&lexer);

                        // we have to search next closing tag.
                        // we do not compare the nodes names bc that is the frontend parser job.
                        SKIP_UNTIL(&lexer, '<');
                        if (IS_EOF(&lexer)) return XML_FILE_CORRUPT;

                        if (GET_CHAR(&lexer) != '/') return XML_FILE_CORRUPT;
                        ADVANCE(&lexer);

                        context.found.target.len = GET_PTR(&lexer) - context.found.target.buf - 1;
                        context.found.type = XML_CHARACTERS;

                        status = fn(user_data, &context);
                        xml_clear_node(&context.found);
                    }
                } else {
                    return XML_FILE_CORRUPT;
                }
            } else if ( nc == '?' || nc == '!') {
                // TODO: retourner le contenu dans found.target et creer un event XML_INFO, pour le found.type
            } else {
                return XML_FILE_CORRUPT;
            }

            if ((status & PARSER_STATUS_MASK) == PARSER_STOP) return 0;
            if ((status & PARSER_STATUS_MASK) == PARSER_STOP_ERROR) return XML_FILE_CORRUPT;
        } else {
            ADVANCE(&lexer);
        }
    }

    return 0;
}

#endif // __SAX_H__
