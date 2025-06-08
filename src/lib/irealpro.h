#ifndef IREALPRO_H
#define IREALPRO_H

#include "sax.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif

typedef enum {
    NOTE_UNSET,
    NOTE_C,
    NOTE_Cb,
    NOTE_Cc,
    NOTE_D,
    NOTE_Db,
    NOTE_Dc,
    NOTE_E,
    NOTE_Eb,
    NOTE_Ec,
    NOTE_F,
    NOTE_Fb,
    NOTE_Fc,
    NOTE_G,
    NOTE_Gb,
    NOTE_Gc,
    NOTE_A, 
    NOTE_Ab,
    NOTE_Ac,
    NOTE_B,
    NOTE_Bb,
    NOTE_Bc
} NoteEnum;

#define IRP_MAX_CHORD_QUALITY_LEN 16

typedef struct {
    uint32_t duration;
    NoteEnum root;
    NoteEnum bass;
    char quality[IRP_MAX_CHORD_QUALITY_LEN];
} IrpChord;

#define IRP_MAX_CHORDS 8 

typedef struct {
    size_t count;
    IrpChord items[IRP_MAX_CHORDS];
} IrpChords;

typedef struct {
    uint32_t beats;
    uint32_t beat_type;
} IrpTimeSignature;

typedef struct {
    NoteEnum Tonic;
    bool minor;
}IrpKey;

typedef struct {
    uint32_t divisions; // quarter note reference integer
    IrpTimeSignature time_signature;
    IrpChords chords;
    char barlines[2];
    bool repeat;
    bool segno;
    bool DC_al_segno;
    bool coda;
    bool DC_al_coda;
} IrpMeasure;

typedef struct IrpMeasures {
    size_t count;
    size_t capacity;
    IrpMeasure *items;
} IrpMeasures;

#ifndef IRP_MAX_CREDENTIALS
#define IRP_MAX_CREDENTIALS 256 
#endif

typedef enum {
    UNKNOWN,
    JAZZ,
    JAZZ_LATIN,
    ROCK,
    POP
} StyleEnum;

typedef struct {
    IrpKey key;
    IrpMeasures measures;
    char composer[IRP_MAX_CREDENTIALS];
    char title[IRP_MAX_CREDENTIALS];
    uint16_t tempo;
    uint16_t repeat_times;
    StyleEnum style;
} IrpSong;

typedef struct {
    da_str title;
    struct {
        size_t count;
        size_t capacity;
        IrpSong *items;
    } song;
} IrpPlaylist;

void irp_song_free(IrpSong *song);
int irp_render_song(IrpSong *song, char *buf, size_t len);
int irp_render_playlist(IrpPlaylist *playlist, char *buf, size_t len);

#endif // IREALPRO_H
