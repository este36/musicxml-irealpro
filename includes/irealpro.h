#ifndef IREALPRO_H
#define IREALPRO_H

#include "da.h"
#include <ctype.h>

#define MAX_CREDENTIALS			256 
#define MAX_CHORD_QUALITIES		16
#define MAX_REHEARSAL_LEN		128
#define MAX_PLAYBACK_LEN		16
#define MAX_CHORDS 4

typedef enum
{
	NOTE_UNVALID,
    NOTE_Ab,
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
    NOTE_MAX,
}	NoteEnum;

typedef struct s_chord
{
    double		duration;
    NoteEnum	root;
    NoteEnum	bass;
    char		quality[MAX_CHORD_QUALITIES];
}	t_chord;

typedef struct s_chords
{
    size_t		count;
    t_chord	items[MAX_CHORDS];
}	t_chords;

typedef struct s_time_signature
{
    uint32_t	beats;
    uint32_t	beat_type;
}	t_time_signature;

typedef	enum
{
	REHEARSAL_NONE,
	REHEARSAL_INTRO,
	REHEARSAL_VERSE,
	REHEARSAL_A,
	REHEARSAL_B,
	REHEARSAL_C,
	REHEARSAL_D,
	REHEARSAL_MAX
}	RehearsalEnum;

typedef enum
{
	PLAYBACK_NONE,
	PLAYBACK_CODA,
	PLAYBACK_SEGNO,
	PLAYBACK_FINE,
	PLAYBACK_DC_AL_CODA,
	PLAYBACK_DC_AL_FINE,
	PLAYBACK_DS_AL_CODA,
	PLAYBACK_DS_AL_FINE,
	PLAYBACK_MAX
}	PlaybackEnum;

typedef enum
{
	ENDING_NONE,
	ENDING_FIRST,
	ENDING_SECOND,
	ENDING_THIRD,
	ENDING_MAX
}	EndingEnum;

typedef struct s_measure t_measure;
struct s_measure
{
	t_measure			*last;
	t_measure			*next;
    uint32_t			divisions; // quarter note reference integer
    t_time_signature	time_signature;
    t_chords			chords;
    char				barlines[2];
	RehearsalEnum		rehearsal;
	PlaybackEnum		playback;
	EndingEnum			ending;
};

typedef struct s_measures
{
    size_t		count;
    size_t		capacity;
    t_measure	*items;
}	t_measures;

typedef enum
{
    STYLE_UNSET,
    STYLE_MEDIUM_SWING,
	STYLE_BALLAD,
    STYLE_ROCK,
    STYLE_POP,
	STYLE_MAX
}	StyleEnum;

typedef enum
{
	ZOOM_NONE,
	ZOOM_OUT, // divide by 2 the n of bars
	ZOOM_IN // multiplies by 2 the n of bars
} ZoomEnum;

typedef struct s_irealpro_song
{
    NoteEnum	key;
    t_measures	measures;
    char		composer[MAX_CREDENTIALS];
    char		title[MAX_CREDENTIALS];
    uint16_t	tempo;
    uint16_t	repeat_times;
    StyleEnum	style;
	uint32_t	zoom;
	uint32_t	first_empty_bars;
}	t_irealpro_song;

typedef struct s_irealpro_playlist
{
    da_str	title;
    struct 
	{
        size_t			count;
        size_t			capacity;
        t_irealpro_song	*items;
    }	songs;
}	t_irealpro_playlist;

int			is_unvalid_time_signature(uint32_t b, uint32_t bt);
const char	*get_note_str(NoteEnum note);
void		irp_song_free(t_irealpro_song *song);
char		*irp_get_song_html(t_irealpro_song *song);

#endif // IREALPRO_H
