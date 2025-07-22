#ifndef __SAX_H__
#define __SAX_H__

#include <stdio.h>
#include <ctype.h>

#include "da.h"

#define ADVANCE(scanner) (scanner)->pos++
#define IS_EOF(scanner) ((scanner)->pos >= (scanner)->xml.len)
#define GET_CHAR(scanner) (((scanner)->xml.buf[(scanner)->pos]))
#define GET_PTR(scanner) ((scanner)->xml.buf + ((scanner)->pos))
#define GET_NEXT_CHAR(scanner) (((scanner)->pos + 1 >= (scanner)->xml.len) ? '\0': ((scanner)->xml.buf[(scanner)->pos + 1]))
#define GET_PREV_CHAR(scanner) (((scanner)->xml.buf[(scanner)->pos -1]))
#define SKIP_UNTIL(scanner, c) while(!IS_EOF(scanner) && GET_CHAR(scanner) != c) ADVANCE(scanner)
#define SKIP_WHILE(scanner, fn) while(!IS_EOF(scanner) && fn(GET_CHAR(scanner))) ADVANCE(scanner)
#define SKIP_WHILE_NOT(scanner, fn) while(!IS_EOF(scanner) && !fn(GET_CHAR(scanner))) ADVANCE(scanner)
#define SKIP_VALID_MXL_NAME(scanner) while (!IS_EOF(scanner) && (isalpha(GET_CHAR(scanner)) || GET_CHAR(scanner) == '-')) ADVANCE(scanner)

#define XML_MAX_ATTRIBUTES 32

#define SKIP_ENTIRE_NODE   0x01
#define PARSER_STATUS_MASK 0x30
#define PARSER_CONTINUE    0x00
#define PARSER_STOP        0x10
#define PARSER_STOP_ERROR  0x20
#define XML_FILE_CORRUPT 1

typedef struct
{
    size_t		pos;
    da_str_ref	xml;
}	sax_scanner;

typedef enum
{
    XML_TAG_OPEN = 1,
    XML_TAG_CLOSE,
    XML_SELF_CLOSING,
}	xml_node_type;

typedef struct
{
    da_str_ref	key;
    da_str_ref	value;
}	xml_attribute;

typedef struct
{
    xml_node_type	type;
    da_str_ref		target; // name or text content
    xml_attribute	attrv[XML_MAX_ATTRIBUTES];
    size_t			attrc;
}	xml_node;

typedef struct
{
    sax_scanner*	scanner;
    xml_node		found;
}	sax_context;

sax_scanner sax_scanner_init(const char* buffer, size_t length);
sax_context sax_context_init(sax_scanner* l);
void		xml_clear_node(xml_node* n);
int			sax_get_content(sax_context* context, da_str_ref* str_ref);
int			sax_copy_content(sax_context* context, char* buf, size_t buf_len);
int			sax_skip_content(sax_context* context, da_str_ref node_name);
int			sax_parse_xml(int (*fn)(void* user_data, sax_context* ctxt),
							void* user_data, sax_context* context);

#endif // __SAX_H__
