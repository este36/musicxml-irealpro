#ifndef __MXL2IRP_H__
#define __MXL2IRP_H__

#include <stdint.h>
#include <stdbool.h>

#define LIBXML_SAX1_ENABLED
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "uthash.h"
#include "da_append.h"

#ifdef MXL2IRP_DEBUG
#include <stdio.h>
#else
#define printf(...) ((void)0)
#endif

#ifndef MXL2IRP_URL_BUFFER_LEN
#define MXL2IRP_URL_BUFFER_LEN 1024 * 32
#endif

typedef enum {
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
} irp_duration;

typedef struct irp_chord {
    irp_duration duration;
    char quality[16];
} irp_chord;

typedef struct irp_chords {
    size_t count;
    size_t capacity;
    irp_chord* items;
} irp_chords;

typedef struct irp_time_signature {
    irp_duration beats;
    irp_duration beat_type;
} irp_time_signature;

typedef struct irp_measures_attributes {
    irp_time_signature time_signature;
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
    irp_measures_attributes* attributes;
} irp_measure;

typedef struct irp_measures {
    size_t count;
    size_t capacity;
    irp_chord* items;
} irp_measures;

typedef struct irp_song {
    irp_measures measures;
    struct {
        char first_name[256];
        char last_name[256];
    } composer;
    char title[256];
} irp_song;

typedef struct mxl2irp_convert_params {
    struct {
        char* name;
        char* content;
        size_t length;
    } file;
    uint16_t part_number;
    uint16_t time_factor;
    uint16_t max_chords_per_bar;
    bool no_visual_exeptations;
    char* include_only;
} mxl2irp_convert_params;

int mxl2irp_get_url(mxl2irp_convert_params* params, char* urlBuffer);
int mxl2irp_load_parser();
#endif // __MXL2IRP_H__
