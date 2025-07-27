#ifndef IREALPRO_H
#define IREALPRO_H

#include "da.h"

#define MAX_CREDENTIALS			256 
#define MAX_CHORD_QUALITIES		16
#define MAX_REHEARSAL_LEN		128
#define MAX_PLAYBACK_LEN		16
#define MAX_CHORDS 5 

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

typedef struct s_measure
{
    uint32_t			divisions; // quarter note reference integer
    t_time_signature	time_signature;
    t_chords			chords;
    char				barlines[2];
	char				rehearsal[MAX_REHEARSAL_LEN];
	char				playback[MAX_PLAYBACK_LEN];
    bool				is_segno;
    bool				is_coda;
    bool				is_DC_al_segno;
    bool				is_DC_al_coda;
}	t_measure;

typedef struct s_measures
{
    size_t		count;
    size_t		capacity;
    t_measure	*items;
}	t_measures;

typedef enum
{
    JAZZ = 1,
    JAZZ_LATIN,
    ROCK,
    POP
}	StyleEnum;

typedef struct s_irealpro_song
{
    NoteEnum		key;
    t_measures	measures;
    char		composer[MAX_CREDENTIALS];
    char		title[MAX_CREDENTIALS];
    uint16_t	tempo;
    uint16_t	repeat_times;
    StyleEnum	style;
}	t_irealpro_song;

typedef struct s_irealpro_playlist
{
    da_str	title;
    struct {
        size_t	count;
        size_t	capacity;
        t_irealpro_song	*items;
    }	songs;
}	t_irealpro_playlist;

const char	*get_note_str(NoteEnum note);
void		irp_song_free(t_irealpro_song *song);
int			irp_render_song(t_irealpro_song *song, char *buf, size_t len);
int			irp_render_playlist(t_irealpro_playlist *playlist, char *buf, size_t len);

#endif // IREALPRO_H
