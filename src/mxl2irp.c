#include "mxl2irp.h"

int parse_strict_int(const char *str, int *out) {
    if (str == NULL || *str == '\0') return 0;

    char *end;
    errno = 0;

    long val = strtol(str, &end, 10);

    if (errno != 0 || *end != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;

    *out = (int) val;
    return 1;
}

/*------------------------------------------------------------*/

struct chord_qualities_hash {
    char musicxml[32];         /* key (string is WITHIN the structure) */
    char irealpro[16];
    UT_hash_handle hh;         /* makes this structure hashable */
};

static struct chord_qualities_hash* chord_qualities = {0};


#define ADD_CHORD_QUALITY(key, value)                          \
    do {                                                       \
        struct chord_qualities_hash *s = malloc(sizeof *s);           \
        strncpy(s->musicxml, key, sizeof(s->musicxml) - 1);    \
        s->musicxml[sizeof(s->musicxml) - 1] = '\0';           \
        strncpy(s->irealpro, value, sizeof(s->irealpro) - 1);  \
        s->irealpro[sizeof(s->irealpro) - 1] = '\0';           \
        HASH_ADD_STR(chord_qualities, musicxml, s);            \
    } while (0)

static const char* get_irealpro_chord(const char* musicxml) 
{
    struct chord_qualities_hash *s;
    HASH_FIND_STR(chord_qualities, musicxml, s);
    return s ? s->irealpro : NULL;
}

static void chord_qualities_cleanup() {
    struct chord_qualities_hash *current, *tmp;
    HASH_ITER(hh, chord_qualities, current, tmp) {
        HASH_DEL(chord_qualities, current);
        free(current);
    }
}

/*------------------------------------------------------------*/

struct time_signatures_hash {
    musicxml_time_signature time_signature;
    char val[4];
    UT_hash_handle hh;         /* makes this structure hashable */
};

static struct time_signatures_hash* time_signatures = {0};

#define ADD_TIME_SIGNATURE(beats_val, beat_type_val, str_val)       \
    do {                                                            \
        struct time_signatures_hash *s = malloc(sizeof *s);         \
        s->time_signature.beats = (beats_val);                      \
        s->time_signature.beat_type = (beat_type_val);              \
        strncpy(s->val, str_val, sizeof(s->val) - 1);               \
        s->val[sizeof(s->val) - 1] = '\0';                          \
        HASH_ADD(hh, time_signatures, time_signature,               \
                 sizeof(musicxml_time_signature), s);               \
    } while (0)

static const char* get_irealpro_time_signature(musicxml_time_signature* ts)
{
    struct time_signatures_hash* s;
    HASH_FIND(hh, time_signatures, ts, sizeof(musicxml_time_signature), s);
    return s ? s->val : NULL;
};
 
void time_signature_cleanup() {
    struct time_signatures_hash *s, *tmp;
    HASH_ITER(hh, time_signatures, s, tmp) {
        HASH_DEL(time_signatures, s);
        free(s);
    }
}

/*------------------------------------------------------------*/

int mxl2irp_load_parser()
{
    // 1. Init hash map for chord qualities
    ADD_CHORD_QUALITY("augmented", "+");
    ADD_CHORD_QUALITY("diminished", "o");
    ADD_CHORD_QUALITY("diminished-seventh", "o7");
    ADD_CHORD_QUALITY("dominant", "7");
    ADD_CHORD_QUALITY("dominant#11", "7#11");
    ADD_CHORD_QUALITY("dominant#5", "7#5");
    ADD_CHORD_QUALITY("dominant#5#9", "7#9#5");
    ADD_CHORD_QUALITY("dominant#5b9", "7b9#5");
    ADD_CHORD_QUALITY("dominant#9", "7#9");
    ADD_CHORD_QUALITY("dominant#9#11", "7#9#11");
    ADD_CHORD_QUALITY("dominant-11th", "11");
    ADD_CHORD_QUALITY("dominant-13th", "13");
    ADD_CHORD_QUALITY("dominant-13th#11", "13#11");
    ADD_CHORD_QUALITY("dominant-13th#9", "13#9");
    ADD_CHORD_QUALITY("dominant-13thb9", "13b9");
    ADD_CHORD_QUALITY("dominant-ninth", "9");
    ADD_CHORD_QUALITY("dominant-ninth#11", "9#11");
    ADD_CHORD_QUALITY("dominant-ninth#5", "9#5");
    ADD_CHORD_QUALITY("dominant-ninthb5", "9b5");
    ADD_CHORD_QUALITY("dominantb13", "7b13");
    ADD_CHORD_QUALITY("dominantb5", "7b5");
    ADD_CHORD_QUALITY("dominantb5#5b9#9", "7alt");
    ADD_CHORD_QUALITY("dominantb5#9", "7#9b5");
    ADD_CHORD_QUALITY("dominantb5b9", "7b9b5");
    ADD_CHORD_QUALITY("dominantb9", "7b9");
    ADD_CHORD_QUALITY("dominantb9#11", "7b9#11");
    ADD_CHORD_QUALITY("dominantb9#9", "7b9#9");
    ADD_CHORD_QUALITY("dominantb913", "13b9");
    ADD_CHORD_QUALITY("dominantb9b13", "7b9b13");
    ADD_CHORD_QUALITY("half-diminished", "-7b5");
    ADD_CHORD_QUALITY("major", "");
    ADD_CHORD_QUALITY("major-13th", "^13");
    ADD_CHORD_QUALITY("major-13th#11", "^7#11");
    ADD_CHORD_QUALITY("major-minor", "-^7");
    ADD_CHORD_QUALITY("major-minor9", "-^9");
    ADD_CHORD_QUALITY("major-ninth", "^9");
    ADD_CHORD_QUALITY("major-ninth#11", "^9#11");
    ADD_CHORD_QUALITY("major-seventh", "^7");
    ADD_CHORD_QUALITY("major-seventh#11", "^7#11");
    ADD_CHORD_QUALITY("major-seventh#5", "^7#5");
    ADD_CHORD_QUALITY("major-seventh#5#9", "^7#5");
    ADD_CHORD_QUALITY("major-seventh913", "^13");
    ADD_CHORD_QUALITY("major-sixth", "6");
    ADD_CHORD_QUALITY("major-sixth9", "69");
    ADD_CHORD_QUALITY("major11", "sus");
    ADD_CHORD_QUALITY("major2", "2");
    ADD_CHORD_QUALITY("major9", "add9");
    ADD_CHORD_QUALITY("minor", "-");
    ADD_CHORD_QUALITY("minor#5", "-#5");
    ADD_CHORD_QUALITY("minor-11th", "-11");
    ADD_CHORD_QUALITY("minor-ninth", "-9");
    ADD_CHORD_QUALITY("minor-seventh", "-7");
    ADD_CHORD_QUALITY("minor-seventh9", "-9");
    ADD_CHORD_QUALITY("minor-seventhb13", "-b6");
    ADD_CHORD_QUALITY("minor-seventhb59", "h9");
    ADD_CHORD_QUALITY("minor-sixth", "-6");
    ADD_CHORD_QUALITY("minor-sixth9", "-69");
    ADD_CHORD_QUALITY("minor9", "-add9");
    ADD_CHORD_QUALITY("minorb6", "-b6");
    ADD_CHORD_QUALITY("none", "x");
    ADD_CHORD_QUALITY("power", "5");
    ADD_CHORD_QUALITY("repeat", "x");
    ADD_CHORD_QUALITY("suspended-fourth", "sus4");
    ADD_CHORD_QUALITY("suspended-fourth7", "7sus");
    ADD_CHORD_QUALITY("suspended-fourth73", "7susadd3");
    ADD_CHORD_QUALITY("suspended-fourth79", "9sus");
    ADD_CHORD_QUALITY("suspended-fourth791113", "13sus");
    ADD_CHORD_QUALITY("suspended-fourthb137", "7b13sus");
    ADD_CHORD_QUALITY("suspended-fourthb97", "7b9sus");
    ADD_CHORD_QUALITY("suspended-second", "sus2");

    // 2. Init hash map for time_signatures
    ADD_TIME_SIGNATURE(12, 16, "T12");
    ADD_TIME_SIGNATURE(12, 8, "T12");
    ADD_TIME_SIGNATURE(2, 2, "T22");
    ADD_TIME_SIGNATURE(2, 4, "T24");
    ADD_TIME_SIGNATURE(3, 2, "T32");
    ADD_TIME_SIGNATURE(3, 4, "T34");
    ADD_TIME_SIGNATURE(4, 4, "T44");
    ADD_TIME_SIGNATURE(5, 16, "T58");
    ADD_TIME_SIGNATURE(5, 4, "T54");
    ADD_TIME_SIGNATURE(5, 8, "T58");
    ADD_TIME_SIGNATURE(6, 16, "T68");
    ADD_TIME_SIGNATURE(6, 4, "T64");
    ADD_TIME_SIGNATURE(6, 8, "T68");
    ADD_TIME_SIGNATURE(7, 16, "T78");
    ADD_TIME_SIGNATURE(7, 4, "T74");
    ADD_TIME_SIGNATURE(7, 8, "T78");
    ADD_TIME_SIGNATURE(9, 16, "T98");
    ADD_TIME_SIGNATURE(9, 8, "T98");

    // 3. Init global xml env
    xmlInitParser();

    return 0;
}

/*------------------------------------------------------------*/

//  xml elements have three types:
//  PARENT : <parent>
//               <child/>
//           </parent>
//  Parents can only store child elements like variables or self-closing elements.
//  
//  VARIABLE : <var>brother ewww</var>
//  Variables can only store plain text in them slot.
//  
//  SELF_CLOSING : <solo/>
//  self-closing elements can only store additional attributes.
//  
//  All xml types can have indeterminates number of attributes.
//  key="value"

// ## Rehearsal
//
//  -- IREAL PRO --
//
//  *A A section
//  *B B section
//  *C C Section
//  *D D Section
//  *V Verse
//  *i Intro
//  S Segno
//  Q Coda
//  f Fermata
//
//  Example: *A[C |A- |SD- |G7 QZ
//
//  -- MUSICXML -- 
//  https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/rehearsal/

// static void parse_rehearsal(xmlNodePtr n, irp_measure* m)
// {
// }

// -- IREAL PRO --
// 
// Time signatures:
// to be placed before a bar line
// T44 4/4
// T34 3/4
// T24 2/4
// T54 5/4
// T64 6/4
// T74 7/4
// T22 2/2
// T32 3/2
// T58 5/8
// T68 6/8
// T78 7/8
// T98 9/8
// T12 12/8
//
// Example: T44[C |A- T98|D- |G7 Z
// 
//  -- MUSICXML --
//  https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/time/

// ## Key Signature
// -- MUSICXML --
// https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/key/

// static void parse_measure_attributes(xmlNode* n, irp_measures_attributes* ma)
// {
// }

//  ## Bar Lines
//  -- IREAL PRO --
//  
//  | single bar line
//  [ opening double bar line
//  ] closing double bar line
//  { opening repeat bar line
//  } closing repeat bar line
//  Z Final thick double bar line
//  
//  Example: {C |A- |D- |G7 }[C |A- |D- |G7 Z
//  
//  -- MUSICXML --
//  
//  <barline> XML::PARENT
//      https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/barline/
//  
//  SLOTS:
//  <bar-style> XML::VARIABLE (optional)
//      https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/bar-style/
//      # Content :
//          - most possible values :
//              heavy, heavy-heavy, heavy-light, light-heavy, light-light, regular
//  <repeat> XML::SELF_CLOSING (optional)
//      # Attributes :
//           - (required) {key: direction , value: backward | forward}
//  <ending> XML::SELF_CLOSING (optional)
//      # Attributes :
//          - (required) {key: ending-number, value: ([ ]*)|([1-9][0-9]*(, ?[1-9][0-9]*)*) }
//          - (required) {key: type, value: start|stop|discontinue }
//  ## Endings
// -- IREAL PRO --
//  N1 First ending
//  N2 Second Ending
//  N3 Third Ending
//  N0 No text Ending
// 
//  Example: T44{C |A- |N1D- |G7 } |N2D- G7 |C6 Z

// static void parse_barline(xmlNode* n, irp_measure* m)
// {
// };

//  -- IREAL PRO --
//
//  Example of full song body: {*AT44D- D-/C |Bh7, Bb7(A7b9) |D-/A G-7 |D-/F sEh,A7,|Y|lD- D-/C |Bh7, Bb7(A7b9) |D-/A G-7 |N1D-/F sEh,A7} Y|N2sD-,G-,lD- ][*BC-7 F7 |Bb^7 |C-7 F7 |Bb^7 n ||C-7 F7 |Bb^7 |B-7 E7 |A7,p,p,p,][*AD- D-/C |Bh7, Bb7(A7b9) |D-/A G-7 |D-/F sEh,A7,||lD- D-/C |Bh7, Bb7(A7b9) |D-/A G-7 |D-/F sEh,A7Z
//
//  -- MUSICXML --
//   
//  <measure> XML::PARENT
//      https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/measure-partwise/
//      # Attributes : 5
//          - (required) {key: number, value: [1-9]} 
//      # Content :
//          - Any child elements can be at any place without order, beside <notes> and <harmony> logical places
//          - Can by empty      
//

// static void parse_measure(xmlNodePtr n, irp_measure* m)
// {
//     printf("%s, %s", (const char *)n.name, m->barlines);
//     //  1.  Check eventual <attributes>
//     //  2.  Check eventual barlines and endings
//     //  3.  Check eventual rehearsal
//
//     //  4.  Check for harmony, then all the notes after <harmony> before another <harmony> encounter.
//     //      Then determine the duration of harmony chord by adding every notes duration
//     //      and divide by the current <divisions> reference.
// };
//

void start_element(void *userData, const xmlChar *name, const xmlChar **attrs) {
    printf("Début de l'élément : %s\n", (const char*)name);
}  

void end_element(void *user_data, const xmlChar *name) {
    printf("Fin de l'élément : %s\n", (const char*)name);
}

void free_irp_song(irp_song* s)
{
    da_free(&s->measures);
}

void mxl2irp_free_convert_params(mxl2irp_convert_params* cp)
{
    if (!cp) return;
    da_str_free(&cp->include_only);
    da_str_free(&cp->filename);
}

void mxl2irp_free_xmlUserData(xmlUserData* ud)
{
    da_str_free(&ud->current_buf);
    free_irp_song(&ud->song);
    mxl2irp_free_convert_params(&ud->params);
}

int mxl2irp_get_url(xmlUserData* ud, da_str* urlBuffer)
{
    static xmlSAXHandler saxHandler = {0};
    saxHandler.startElement = start_element;
    saxHandler.endElement = end_element;

    if(xmlSAXUserParseMemory((xmlSAXHandlerPtr)&saxHandler, ud, ud->params.file.buf, ud->params.file.len) != 0) {
        return -1;
    }

    *urlBuffer = DA_STR("hey!\n");
    str_append(urlBuffer, ud->params.include_only.buf);
    return urlBuffer->len;
};


