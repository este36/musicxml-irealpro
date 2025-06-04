#ifndef __SAX_H__
#define __SAX_H__

#include <stdio.h>
#include <ctype.h>

#include "da.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif

typedef struct sax_scanner {
    size_t pos;
    da_str_ref xml;
}sax_scanner;

#define ADVANCE(scanner) (scanner)->pos++
#define IS_EOF(scanner) ((scanner)->pos >= (scanner)->xml.len)
#define GET_CHAR(scanner) (((scanner)->xml.buf[(scanner)->pos]))
#define GET_PTR(scanner) ((scanner)->xml.buf + ((scanner)->pos))
#define GET_NEXT_CHAR(scanner) (((scanner)->pos + 1 >= (scanner)->xml.len) ? '\0': ((scanner)->xml.buf[(scanner)->pos + 1]))
#define GET_PREV_CHAR(scanner) (((scanner)->xml.buf[(scanner)->pos -1]))
#define SKIP_UNTIL(scanner, c) while(!IS_EOF(scanner) && GET_CHAR(scanner) != c) ADVANCE(scanner)
#define SKIP_WHILE(scanner, fn) while(!IS_EOF(scanner) && fn(GET_CHAR(scanner))) ADVANCE(scanner)
#define SKIP_WHILE_NOT(scanner, fn) while(!IS_EOF(scanner) && !fn(GET_CHAR(scanner))) ADVANCE(scanner)

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
    da_str_ref key;
    da_str_ref value;
} xml_attribute;

typedef struct xml_node {
    xml_node_type type;
    da_str_ref target; // name or text content
    xml_attribute attrv[XML_MAX_ATTRIBUTES];
    size_t attrc;
} xml_node;

void xml_clear_node(xml_node* n);

typedef struct sax_context {
    sax_scanner* scanner;
    xml_node found;
} sax_context;

// orders/infos to talk to backend parser
// usage : PARSER_CONTINUE | SKIP_ENTIRE_NODE
#define SKIP_ENTIRE_NODE   0x01

#define PARSER_STATUS_MASK 0x30
#define PARSER_CONTINUE    0x00
#define PARSER_STOP        0x10
#define PARSER_STOP_ERROR  0x20

#define XML_FILE_CORRUPT 1

sax_scanner sax_scanner_init(const char* buffer, size_t length);

sax_context sax_context_init(sax_scanner* l);

#define SKIP_VALID_MXL_NAME(scanner) while (!IS_EOF(scanner) && (isalpha(GET_CHAR(scanner)) || GET_CHAR(scanner) == '-')) ADVANCE(scanner)

// this will search for next closing tag of the context.found node.
// So the context.found is exepted to be a container of either text or other child elements, especially the opening tag of this container.
// the scanner is expected to be at the begining of the content, after the > char of opening tag.
// the user of this function is supposed to be sure that the element is a container.
// if the str_ref is null then the node will just be skiped without doing buffering
int sax_get_content(sax_context* context, da_str_ref* str_ref);

// Same implementation as sax_get_content but NULL in second param, bc it means litteraly to not save the content but to go through entirely.
int sax_skip_content(sax_context* context);

int sax_parse_xml(int (*fn)(void* user_data, sax_context* ctxt), void* user_data, sax_context* context);


#endif // __SAX_H__
