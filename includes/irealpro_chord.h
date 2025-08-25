/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --language=ANSI-C ./src/musicxml_harmony.gperf  */
/* Computed positions: -k'3,8-11,14-15,18,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "./src/musicxml_harmony.gperf"

#ifndef IREALPRO_CHORD_H
#define IREALPRO_CHORD_H
#include <string.h>
#if defined(__clang__)
  #pragma clang diagnostic ignored "-Wmissing-field-initializers"
  #pragma clang diagnostic ignored "-Wduplicate-decl-specifier"

#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#line 19 "./src/musicxml_harmony.gperf"
struct keyword { const char *name; int id; };

#define TOTAL_KEYWORDS 110
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 236
/* maximum key range = 231, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static inline unsigned int
my_hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237,   0,  50, 237, 237, 237,
      237, 237, 237, 237, 237,   0,  35, 237, 237,   5,
       10,  20,   5,  15,  85,   0, 237,   0,   0, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237,   0,  25,  85,
       10,  10,  10,  30,  10, 100,   5, 237,   5,   0,
        0,   5,  40, 237,  19,  35,   0,   0,  10,  30,
       10, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
      237, 237, 237, 237, 237, 237, 237
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
      case 17:
      case 16:
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
      case 12:
      case 11:
        hval += asso_values[(unsigned char)str[10]+1];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]+1];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

static inline const struct keyword *
irealpro_chord_lookup (register const char *str, register size_t len)
{
  static const struct keyword wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 110 "./src/musicxml_harmony.gperf"
      {"minor9", 89},
#line 128 "./src/musicxml_harmony.gperf"
      {"Italian", 107},
#line 32 "./src/musicxml_harmony.gperf"
      {"dominant", 11},
      {""},
#line 40 "./src/musicxml_harmony.gperf"
      {"dominant#9", 19},
#line 95 "./src/musicxml_harmony.gperf"
      {"major9", 74},
      {""}, {""},
#line 113 "./src/musicxml_harmony.gperf"
      {"none", 92},
#line 129 "./src/musicxml_harmony.gperf"
      {"Neapolitan", 108},
#line 94 "./src/musicxml_harmony.gperf"
      {"major4", 73},
#line 92 "./src/musicxml_harmony.gperf"
      {"major11", 71},
      {""},
#line 33 "./src/musicxml_harmony.gperf"
      {"dominant4", 12},
#line 73 "./src/musicxml_harmony.gperf"
      {"pedal", 52},
#line 93 "./src/musicxml_harmony.gperf"
      {"major2", 72},
#line 97 "./src/musicxml_harmony.gperf"
      {"minor#5", 76},
      {""},
#line 96 "./src/musicxml_harmony.gperf"
      {"minor", 75},
#line 127 "./src/musicxml_harmony.gperf"
      {"German", 106},
#line 126 "./src/musicxml_harmony.gperf"
      {"French", 105},
#line 29 "./src/musicxml_harmony.gperf"
      {"major#5", 8},
      {""},
#line 72 "./src/musicxml_harmony.gperf"
      {"major", 51},
#line 49 "./src/musicxml_harmony.gperf"
      {"dominant-13th#9", 28},
      {""}, {""}, {""},
#line 114 "./src/musicxml_harmony.gperf"
      {"other", 93},
#line 63 "./src/musicxml_harmony.gperf"
      {"dominantb9", 42},
#line 36 "./src/musicxml_harmony.gperf"
      {"dominant#11", 15},
      {""},
#line 46 "./src/musicxml_harmony.gperf"
      {"dominant-13th", 25},
#line 47 "./src/musicxml_harmony.gperf"
      {"dominant-13th4", 26},
#line 57 "./src/musicxml_harmony.gperf"
      {"dominant13", 36},
#line 48 "./src/musicxml_harmony.gperf"
      {"dominant-13th#11", 27},
#line 53 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#11", 32},
#line 45 "./src/musicxml_harmony.gperf"
      {"dominant-11th", 24},
#line 51 "./src/musicxml_harmony.gperf"
      {"dominant-ninth", 30},
#line 52 "./src/musicxml_harmony.gperf"
      {"dominant-ninth4", 31},
#line 116 "./src/musicxml_harmony.gperf"
      {"repeat", 95},
      {""},
#line 111 "./src/musicxml_harmony.gperf"
      {"minorb13", 90},
#line 104 "./src/musicxml_harmony.gperf"
      {"minor-seventh9", 83},
#line 30 "./src/musicxml_harmony.gperf"
      {"diminished", 9},
#line 54 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#5", 33},
      {""},
#line 27 "./src/musicxml_harmony.gperf"
      {"majorb13", 6},
#line 115 "./src/musicxml_harmony.gperf"
      {"power", 94},
#line 50 "./src/musicxml_harmony.gperf"
      {"dominant-13thb9", 29},
#line 34 "./src/musicxml_harmony.gperf"
      {"dominantb94", 13},
#line 35 "./src/musicxml_harmony.gperf"
      {"dominantb134", 14},
#line 102 "./src/musicxml_harmony.gperf"
      {"minor-seventh", 81},
      {""},
#line 26 "./src/musicxml_harmony.gperf"
      {"augmented9", 5},
#line 56 "./src/musicxml_harmony.gperf"
      {"dominant-ninth13", 35},
#line 85 "./src/musicxml_harmony.gperf"
      {"major-seventh9#11", 64},
#line 83 "./src/musicxml_harmony.gperf"
      {"major-seventh", 62},
      {""},
#line 105 "./src/musicxml_harmony.gperf"
      {"minor-seventh11", 84},
#line 84 "./src/musicxml_harmony.gperf"
      {"major-seventh#11", 63},
#line 69 "./src/musicxml_harmony.gperf"
      {"dominantb913", 48},
#line 43 "./src/musicxml_harmony.gperf"
      {"dominant#9#11", 22},
#line 21 "./src/musicxml_harmony.gperf"
      {"augmented", 0},
      {""},
#line 58 "./src/musicxml_harmony.gperf"
      {"dominantb13", 37},
#line 88 "./src/musicxml_harmony.gperf"
      {"major-seventh#5#9", 67},
      {""}, {""}, {""},
#line 55 "./src/musicxml_harmony.gperf"
      {"dominant-ninthb5", 34},
#line 41 "./src/musicxml_harmony.gperf"
      {"dominant#9#5", 20},
      {""}, {""}, {""},
#line 89 "./src/musicxml_harmony.gperf"
      {"major-seventh913", 68},
      {""}, {""}, {""},
#line 87 "./src/musicxml_harmony.gperf"
      {"major-seventh#5", 66},
      {""},
#line 68 "./src/musicxml_harmony.gperf"
      {"dominantb9#9", 47},
      {""}, {""}, {""},
#line 107 "./src/musicxml_harmony.gperf"
      {"minor-seventhb59", 86},
#line 112 "./src/musicxml_harmony.gperf"
      {"minorb6", 91},
#line 66 "./src/musicxml_harmony.gperf"
      {"dominantb9#11", 45},
      {""}, {""}, {""},
#line 28 "./src/musicxml_harmony.gperf"
      {"majorb6", 7},
      {""}, {""}, {""},
#line 106 "./src/musicxml_harmony.gperf"
      {"minor-seventhb13", 85},
#line 65 "./src/musicxml_harmony.gperf"
      {"dominantb9#5", 44},
      {""}, {""},
#line 103 "./src/musicxml_harmony.gperf"
      {"minor-seventhb5", 82},
      {""},
#line 130 "./src/musicxml_harmony.gperf"
      {"Tristan", 109},
      {""}, {""},
#line 37 "./src/musicxml_harmony.gperf"
      {"dominant#5", 16},
      {""},
#line 42 "./src/musicxml_harmony.gperf"
      {"dominant#9b5", 21},
#line 25 "./src/musicxml_harmony.gperf"
      {"augmented-seventh9", 4},
#line 23 "./src/musicxml_harmony.gperf"
      {"augmented-seventh#9", 2},
      {""}, {""}, {""},
#line 44 "./src/musicxml_harmony.gperf"
      {"dominant#9b13", 23},
      {""}, {""},
#line 125 "./src/musicxml_harmony.gperf"
      {"suspended-second", 104},
#line 22 "./src/musicxml_harmony.gperf"
      {"augmented-seventh", 1},
#line 31 "./src/musicxml_harmony.gperf"
      {"diminished-seventh", 10},
      {""},
#line 98 "./src/musicxml_harmony.gperf"
      {"minor-11th", 77},
      {""}, {""}, {""}, {""},
#line 74 "./src/musicxml_harmony.gperf"
      {"major-11th", 53},
      {""}, {""}, {""}, {""},
#line 59 "./src/musicxml_harmony.gperf"
      {"dominantb5", 38},
#line 67 "./src/musicxml_harmony.gperf"
      {"dominantb9b13#11", 46},
#line 64 "./src/musicxml_harmony.gperf"
      {"dominantb9b5", 43},
      {""},
#line 24 "./src/musicxml_harmony.gperf"
      {"augmented-seventhb9", 3},
#line 99 "./src/musicxml_harmony.gperf"
      {"minor-13th", 78},
      {""}, {""},
#line 70 "./src/musicxml_harmony.gperf"
      {"dominantb9b13", 49},
      {""},
#line 75 "./src/musicxml_harmony.gperf"
      {"major-13th", 54},
      {""},
#line 38 "./src/musicxml_harmony.gperf"
      {"dominant#5#9", 17},
      {""},
#line 86 "./src/musicxml_harmony.gperf"
      {"major-13th9#11", 65},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 118 "./src/musicxml_harmony.gperf"
      {"suspended-fourth7", 97},
#line 120 "./src/musicxml_harmony.gperf"
      {"suspended-fourth79", 99},
#line 123 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb97", 102},
      {""},
#line 71 "./src/musicxml_harmony.gperf"
      {"half-diminished", 50},
      {""}, {""}, {""},
#line 122 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb137", 101},
#line 117 "./src/musicxml_harmony.gperf"
      {"suspended-fourth", 96},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 61 "./src/musicxml_harmony.gperf"
      {"dominantb5#9", 40},
#line 124 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb57", 103},
      {""}, {""},
#line 60 "./src/musicxml_harmony.gperf"
      {"dominantb5#5b9#9", 39},
      {""}, {""}, {""}, {""},
#line 121 "./src/musicxml_harmony.gperf"
      {"suspended-fourth791113", 100},
#line 39 "./src/musicxml_harmony.gperf"
      {"dominant#5b9", 18},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 79 "./src/musicxml_harmony.gperf"
      {"major-minor9", 58},
#line 76 "./src/musicxml_harmony.gperf"
      {"major-13th#11", 55},
      {""}, {""}, {""},
#line 119 "./src/musicxml_harmony.gperf"
      {"suspended-fourth73", 98},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 80 "./src/musicxml_harmony.gperf"
      {"major-minor911", 59},
      {""}, {""},
#line 62 "./src/musicxml_harmony.gperf"
      {"dominantb5b9", 41},
#line 78 "./src/musicxml_harmony.gperf"
      {"major-minorb5", 57},
      {""},
#line 77 "./src/musicxml_harmony.gperf"
      {"major-minor", 56},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 100 "./src/musicxml_harmony.gperf"
      {"minor-ninth", 79},
#line 109 "./src/musicxml_harmony.gperf"
      {"minor-sixth9", 88},
      {""}, {""}, {""},
#line 81 "./src/musicxml_harmony.gperf"
      {"major-ninth", 60},
#line 91 "./src/musicxml_harmony.gperf"
      {"major-sixth9", 70},
#line 101 "./src/musicxml_harmony.gperf"
      {"minor-ninthb5", 80},
#line 82 "./src/musicxml_harmony.gperf"
      {"major-ninth#11", 61},
      {""},
#line 108 "./src/musicxml_harmony.gperf"
      {"minor-sixth", 87},
      {""}, {""}, {""}, {""},
#line 90 "./src/musicxml_harmony.gperf"
      {"major-sixth", 69}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = my_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 131 "./src/musicxml_harmony.gperf"

#endif /* IREALPRO_CHORD_H */

