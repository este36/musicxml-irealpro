#ifndef IREALPRO_H
#define IREALPRO_H

#include "sax.h"

#define IRP_MAX_CREDENTIALS			256 
#define IRP_MAX_CHORD_QUALITIES		16
#define IRP_MAX_CHORDS 5 

typedef enum
{
    NOTE_Ab = 1,
    NOTE_A, 
    NOTE_Ac,
    NOTE_Bb,
    NOTE_B,
    NOTE_Bc,
    NOTE_Cb,
    NOTE_C,
    NOTE_Cc,
    NOTE_Db,
    NOTE_D,
    NOTE_Dc,
    NOTE_Eb,
    NOTE_E,
    NOTE_Ec,
    NOTE_Fb,
    NOTE_F,
    NOTE_Fc,
    NOTE_Gb,
    NOTE_G,
    NOTE_Gc,
    NOTE_MAX
}	NoteEnum;

typedef struct
{
    double		duration;
    NoteEnum	root;
    NoteEnum	bass;
    char		quality[IRP_MAX_CHORD_QUALITIES];
}	IrpChord;

typedef struct
{
    size_t		count;
    IrpChord	items[IRP_MAX_CHORDS];
}	IrpChords;

typedef struct
{
    uint32_t	beats;
    uint32_t	beat_type;
}	IrpTimeSignature;

typedef struct
{
    NoteEnum	Tonic;
    bool		minor;
}	IrpKey;

typedef struct
{
    uint32_t			divisions; // quarter note reference integer
    IrpTimeSignature	time_signature;
    IrpChords			chords;
    char				barlines[2];
    bool				repeat;
    bool				segno;
    bool				DC_al_segno;
    bool				coda;
    bool				DC_al_coda;
}	IrpMeasure;

typedef struct
{
    size_t		count;
    size_t		capacity;
    IrpMeasure	*items;
}	IrpMeasures;

typedef enum
{
    JAZZ = 1,
    JAZZ_LATIN,
    ROCK,
    POP
}	StyleEnum;

typedef struct
{
    IrpKey		key;
    IrpMeasures	measures;
    char		composer[IRP_MAX_CREDENTIALS];
    char		title[IRP_MAX_CREDENTIALS];
    uint16_t	tempo;
    uint16_t	repeat_times;
    StyleEnum	style;
}	IrpSong;

typedef struct
{
    da_str	title;
    struct {
        size_t	count;
        size_t	capacity;
        IrpSong	*items;
    }	song;
}	IrpPlaylist;

const char*	get_note_str(NoteEnum note);
void		irp_song_free(IrpSong *song);
int			irp_render_song(IrpSong *song, char *buf, size_t len);
int			irp_render_playlist(IrpPlaylist *playlist, char *buf, size_t len);

#endif // IREALPRO_H
