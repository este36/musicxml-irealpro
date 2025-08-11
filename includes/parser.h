#ifndef PARSER_H
#define PARSER_H

#include "irealpro_chord.h"
#include "da.h"
#include "musicxml.h"
#include "irealpro.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

#define XML_MAX_ATTRIBUTES	32

#define SKIP_ENTIRE_NODE	0x01
#define PARSER_STATUS_MASK	0x30
#define PARSER_CONTINUE		0x00
#define PARSER_STOP			0x10
#define PARSER_STOP_ERROR	0x20
#define XML_FILE_CORRUPT	-1

#define GET_CURR_MEASURE(parser_state) (&(parser_state)->song->measures.items[(parser_state)->song->measures.count - 1])
#define GET_CURR_CHORD(parser_state) (&(GET_CURR_MEASURE(parser_state))->chords.items[(GET_CURR_MEASURE(parser_state))->chords.count - 1])
#define GET_TMP_CHORD(parser_state) (&(parser_state)->tmp_chord)
#define GET_TMP_CURR_DEGREE(parser_state) (&GET_TMP_CHORD(parser_state)->degrees[GET_TMP_CHORD(parser_state)->degrees_count - 1])

// a bit of ascii arithmetic and me can get the correct root note.
// the scanner will be at the begining of content in after the open tag
#define GET_NOTE_STEP() ((NoteEnum)(((int)GET_CHAR(context->scanner) - 'A') * 3 + 2))

#define TMP_CHORD_MAX_LEN		128
#define TMP_CHORD_MAX_DEGREES	4

typedef struct s_sax_scanner
{
    size_t		pos;
    da_str_ref	xml;
}	t_sax_scanner;

typedef enum s_xml_node_type
{
	XML_TAG_UNSET,
    XML_TAG_OPEN,
    XML_TAG_CLOSE,
    XML_SELF_CLOSING,
}	t_xml_node_type;

typedef struct s_xml_attribute
{
    da_str_ref	key;
    da_str_ref	value;
}	t_xml_attribute;

typedef struct s_xml_node
{
    t_xml_node_type	type;
    da_str_ref		target; // name or text content
    t_xml_attribute	attrv[XML_MAX_ATTRIBUTES];
    size_t			attrc;
}	t_xml_node;

typedef struct s_sax_context
{
    t_sax_scanner	*scanner;
    t_xml_node		found;
}	t_sax_context;

typedef struct s_mxl_degree
{
    int	value;
    int	alter;
}	t_mxl_degree;

typedef struct s_mxl_chord
{
	NoteEnum		root;
	NoteEnum		bass;
	da_str_ref		qual;
	t_mxl_degree	degrees[TMP_CHORD_MAX_DEGREES];
	uint32_t		degrees_count;
}	t_mxl_chord;

typedef struct s_parser_state
{
	t_irealpro_song		*song;
	uint32_t			part_selected;
	t_mxl_chord			tmp_chord;
	t_time_signature	tmp_time_signature; 
	uint32_t			tmp_divisions;
}	t_parser_state;

t_sax_scanner	sax_scanner_init(const char *buffer, size_t length);
t_sax_context	sax_context_init(t_sax_scanner *l);
void			xml_clear_node(t_xml_node *n);
int				sax_get_content(t_sax_context *context, da_str_ref *str_ref);
int				sax_copy_content(t_sax_context *context, char *buf, size_t buf_len);
int				sax_get_int(t_sax_context *context, int *res);
int				sax_get_attrv(t_sax_context *context, da_str_ref *dst, char *key_buf);
int				sax_skip_content(t_sax_context *context, da_str_ref node_name);
int				sax_parse_xml(int (*fn)(t_parser_state *parser_state, t_sax_context *ctxt),
							t_parser_state *parser_state, t_sax_context *context);

int	parse_direction(t_parser_state *parser_state, t_sax_context *context);
int parse_attributes(t_parser_state *parser_state, t_sax_context *context);
int parse_harmony(t_parser_state *parser_state, t_sax_context *context);
int parse_measure(t_parser_state *parser_state, t_sax_context *context);
int parse_musicxml_song(t_irealpro_song *irp_song,
					const char *musicxml,
					const size_t musicxml_length);
#endif // PARSER_H
