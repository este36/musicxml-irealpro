#ifndef __MXL2IRP_H__
#define __MXL2IRP_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "lib/uthash.h"
#include "lib/da.h"

#define MXL2IRP_DEBUG
#ifdef MXL2IRP_DEBUG
#include <stdio.h>
#else
#define printf(...) ((void)0)
#endif

#ifndef MXL2IRP_URL_BUFFER_LEN
#define MXL2IRP_URL_BUFFER_LEN (1024 * 32)
#endif

int parse_strict_int(const char *str, int *out);

enum {
    NOTE_UNDEFINED = 0,
    NOTE_QUARTER = 360360,
    NOTE_WHOLE = NOTE_QUARTER * 4,
    NOTE_HALF = NOTE_QUARTER * 2,
    NOTE_EIGHTH = NOTE_QUARTER / 2,
    NOTE_SIXTEENTH = NOTE_QUARTER / 4,
    NOTE_THIRTYSECOND = NOTE_QUARTER / 8,

    NOTE_DOTTED_WHOLE = NOTE_WHOLE + (NOTE_WHOLE / 2),
    NOTE_DOTTED_QUARTER = NOTE_QUARTER + (NOTE_QUARTER / 2),
    NOTE_DOTTED_HALF = NOTE_HALF + (NOTE_HALF / 2),
    NOTE_DOTTED_EIGHTH = NOTE_EIGHTH + (NOTE_EIGHTH / 2),
    NOTE_DOTTED_SIXTEENTH = NOTE_SIXTEENTH + (NOTE_SIXTEENTH / 2),

    NOTE_TRIPLET_HALF = (NOTE_HALF * 2) / 3,
    NOTE_TRIPLET_QUARTER = (NOTE_QUARTER * 2) / 3,
    NOTE_TRIPLET_EIGHTH = (NOTE_EIGHTH * 2) / 3,
    NOTE_TRIPLET_SIXTEENTH = (NOTE_SIXTEENTH * 2) / 3,

    NOTE_QUINTOLET_QUARTER = (NOTE_QUARTER * 4) / 5,
    NOTE_QUINTOLET_EIGHTH = (NOTE_EIGHTH * 4) / 5,
    NOTE_QUINTOLET_SIXTEENTH = (NOTE_SIXTEENTH * 4) / 5,

    NOTE_SEPTOLET_QUARTER = (NOTE_QUARTER * 4) / 7,
    NOTE_SEPTOLET_EIGHTH = (NOTE_EIGHTH * 4) / 7,
    NOTE_SEPTOLET_SIXTEENTH = (NOTE_SIXTEENTH * 4) / 7
};

typedef struct irp_chord {
    uint32_t duration;
    char quality[16];
} irp_chord;

typedef struct irp_chords {
    uint32_t count;
    uint32_t capacity;
    irp_chord* items;
} irp_chords;

typedef struct musicxml_time_signature {
    uint32_t beats;
    uint32_t beat_type;
} musicxml_time_signature;

typedef struct irp_measures_attributes {
    musicxml_time_signature time_signature;
    char key[4];
    uint32_t divisions;
} irp_measures_attributes;

typedef struct irp_measure {
    char barlines[2];
    bool repeat;
    bool segno;
    bool DC_al_segno;
    bool coda;
    bool DC_al_coda;
    irp_chords chords;
    irp_measures_attributes attributes;
} irp_measure;

typedef struct irp_measures {
    uint32_t count;
    uint32_t capacity;
    irp_measure* items;
} irp_measures;

#ifndef IRP_MAX_CREDENTIALS
#define IRP_MAX_CREDENTIALS 256 
#endif

typedef struct irp_song {
    irp_measures measures;
    struct {
        char first_name[IRP_MAX_CREDENTIALS];
        char last_name[IRP_MAX_CREDENTIALS];
    } composer;
    char title[IRP_MAX_CREDENTIALS];
} irp_song;

void free_irp_song(irp_song* s);

typedef struct mxl2irp_convert_params {
    uint16_t part_number;
    uint16_t time_factor;
    uint16_t max_chords_per_bar;
    bool no_visual_exeptations;
    da_str include_only;
    da_str file;
    da_str filename;
} mxl2irp_convert_params;

void mxl2irp_free_convert_params(mxl2irp_convert_params* cp);

enum {
    HEADER_UNSET,
    HEADER_WORK,
    HEADER_WORK_TITLE,
    HEADER_ID,
    HEADER_ID_COMPOSER,
    HEADER_PARTLIST,
    HEADER_PARTLIST_SCOREPART,
};

typedef struct xmlUserData {
    bool score_partwise;
    uint8_t header_element;
    bool part;
    uint8_t measure_element;
    da_str current_buf;
    irp_song song;
    mxl2irp_convert_params params;
} xmlUserData;

int mxl2irp_get_url(da_str* urlBuffer);
void mxl2irp_free_xmlUserData(xmlUserData* ctxt);

int mxl2irp_load_parser();
#endif // __MXL2IRP_H__
