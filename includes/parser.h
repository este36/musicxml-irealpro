#ifndef __MXL_PARSER_H__
#define __MXL_PARSER_H__

#include "da.h"
#include "musicxml.h"
#include "irealpro.h"
#include "sax.h"

#define GET_CURR_MEASURE(parser_data) (&(parser_data)->song->measures.items[(parser_data)->song->measures.count - 1])
#define GET_CURR_CHORD(parser_data) (&(GET_CURR_MEASURE(parser_data))->chords.items[(GET_CURR_MEASURE(parser_data))->chords.count - 1])

#define TMP_CHORD_MAX_LEN 128
#define TMP_CHORD_MAX_DEGREES 3

typedef struct
{
    int16_t	value;
    int16_t	alter;
}	mxl_degree;

typedef struct
{
    bool	rehearsal;
    bool	attributes;
    struct
	{
        NoteEnum	root;
        NoteEnum	bass;
        da_str_ref	qual;
        mxl_degree	degrees[TMP_CHORD_MAX_DEGREES];
        uint32_t	deg_count;
    }	curr_chord;
}	MsrState;

typedef struct {
	IrpSong     *song;
	da_str_ref  part_selected;
	void        *state;
}   ParserData;

int parse_musicxml_song(IrpSong *irp_song,
					const da_str_ref part_id,
					const char *musicxml,
					const size_t musicxml_length);
#endif // __PARSER_H__
