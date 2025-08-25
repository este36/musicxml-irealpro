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

#define TOTAL_KEYWORDS 108
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 291
/* maximum key range = 286, duplicates = 0 */

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
  static const unsigned short asso_values[] =
    {
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292,   0,  20, 292, 292, 292,
      292, 292, 292, 292, 292,   5,  50, 292, 292,   0,
        5,  25,   0,  15,  65,   0, 292,   0,  35, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292,   0,  25,  55,
        5,  15,   5,   0,  10,  60,   5, 292,  50,   0,
        0,   0,  95, 292, 115,   5,   0,  10,   5,  15,
       20, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292, 292, 292, 292,
      292, 292, 292, 292, 292, 292, 292
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
#line 108 "./src/musicxml_harmony.gperf"
      {"minor9", 87},
#line 126 "./src/musicxml_harmony.gperf"
      {"Italian", 105},
#line 32 "./src/musicxml_harmony.gperf"
      {"dominant", 11},
#line 33 "./src/musicxml_harmony.gperf"
      {"dominant4", 12},
#line 127 "./src/musicxml_harmony.gperf"
      {"Neapolitan", 106},
#line 93 "./src/musicxml_harmony.gperf"
      {"major9", 72},
#line 91 "./src/musicxml_harmony.gperf"
      {"major11", 70},
      {""}, {""}, {""},
#line 92 "./src/musicxml_harmony.gperf"
      {"major2", 71},
      {""}, {""},
#line 111 "./src/musicxml_harmony.gperf"
      {"none", 90},
      {""},
#line 36 "./src/musicxml_harmony.gperf"
      {"dominant#11", 15},
#line 95 "./src/musicxml_harmony.gperf"
      {"minor#5", 74},
      {""},
#line 47 "./src/musicxml_harmony.gperf"
      {"dominant-13th4", 26},
#line 49 "./src/musicxml_harmony.gperf"
      {"dominant-13th#9", 28},
#line 48 "./src/musicxml_harmony.gperf"
      {"dominant-13th#11", 27},
#line 29 "./src/musicxml_harmony.gperf"
      {"major#5", 8},
      {""}, {""}, {""},
#line 124 "./src/musicxml_harmony.gperf"
      {"French", 103},
      {""},
#line 46 "./src/musicxml_harmony.gperf"
      {"dominant-13th", 25},
#line 21 "./src/musicxml_harmony.gperf"
      {"augmented", 0},
#line 52 "./src/musicxml_harmony.gperf"
      {"dominant-ninth4", 31},
      {""},
#line 53 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#11", 32},
#line 45 "./src/musicxml_harmony.gperf"
      {"dominant-11th", 24},
#line 102 "./src/musicxml_harmony.gperf"
      {"minor-seventh9", 81},
#line 103 "./src/musicxml_harmony.gperf"
      {"minor-seventh11", 82},
      {""},
#line 35 "./src/musicxml_harmony.gperf"
      {"dominantb134", 14},
      {""},
#line 51 "./src/musicxml_harmony.gperf"
      {"dominant-ninth", 30},
#line 40 "./src/musicxml_harmony.gperf"
      {"dominant#9", 19},
#line 83 "./src/musicxml_harmony.gperf"
      {"major-seventh#11", 62},
#line 84 "./src/musicxml_harmony.gperf"
      {"major-seventh9#11", 63},
#line 100 "./src/musicxml_harmony.gperf"
      {"minor-seventh", 79},
      {""},
#line 50 "./src/musicxml_harmony.gperf"
      {"dominant-13thb9", 29},
#line 54 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#5", 33},
      {""},
#line 82 "./src/musicxml_harmony.gperf"
      {"major-seventh", 61},
      {""},
#line 30 "./src/musicxml_harmony.gperf"
      {"diminished", 9},
      {""}, {""},
#line 109 "./src/musicxml_harmony.gperf"
      {"minorb13", 88},
      {""},
#line 72 "./src/musicxml_harmony.gperf"
      {"pedal", 51},
#line 56 "./src/musicxml_harmony.gperf"
      {"dominant-ninth13", 35},
#line 87 "./src/musicxml_harmony.gperf"
      {"major-seventh#5#9", 66},
#line 27 "./src/musicxml_harmony.gperf"
      {"majorb13", 6},
      {""},
#line 26 "./src/musicxml_harmony.gperf"
      {"augmented9", 5},
#line 57 "./src/musicxml_harmony.gperf"
      {"dominantb13", 36},
#line 128 "./src/musicxml_harmony.gperf"
      {"Tristan", 107},
#line 43 "./src/musicxml_harmony.gperf"
      {"dominant#9#11", 22},
      {""},
#line 62 "./src/musicxml_harmony.gperf"
      {"dominantb9", 41},
#line 88 "./src/musicxml_harmony.gperf"
      {"major-seventh913", 67},
#line 110 "./src/musicxml_harmony.gperf"
      {"minorb6", 89},
      {""}, {""},
#line 86 "./src/musicxml_harmony.gperf"
      {"major-seventh#5", 65},
#line 55 "./src/musicxml_harmony.gperf"
      {"dominant-ninthb5", 34},
#line 28 "./src/musicxml_harmony.gperf"
      {"majorb6", 7},
      {""}, {""},
#line 96 "./src/musicxml_harmony.gperf"
      {"minor-11th", 75},
#line 105 "./src/musicxml_harmony.gperf"
      {"minor-seventhb59", 84},
#line 41 "./src/musicxml_harmony.gperf"
      {"dominant#9#5", 20},
      {""}, {""},
#line 73 "./src/musicxml_harmony.gperf"
      {"major-11th", 52},
#line 34 "./src/musicxml_harmony.gperf"
      {"dominantb94", 13},
      {""}, {""}, {""},
#line 37 "./src/musicxml_harmony.gperf"
      {"dominant#5", 16},
#line 104 "./src/musicxml_harmony.gperf"
      {"minor-seventhb13", 83},
#line 67 "./src/musicxml_harmony.gperf"
      {"dominantb9#9", 46},
#line 65 "./src/musicxml_harmony.gperf"
      {"dominantb9#11", 44},
      {""},
#line 101 "./src/musicxml_harmony.gperf"
      {"minor-seventhb5", 80},
#line 123 "./src/musicxml_harmony.gperf"
      {"suspended-second", 102},
#line 38 "./src/musicxml_harmony.gperf"
      {"dominant#5#9", 17},
      {""}, {""}, {""},
#line 114 "./src/musicxml_harmony.gperf"
      {"repeat", 93},
#line 68 "./src/musicxml_harmony.gperf"
      {"dominantb913", 47},
#line 25 "./src/musicxml_harmony.gperf"
      {"augmented-seventh9", 4},
#line 23 "./src/musicxml_harmony.gperf"
      {"augmented-seventh#9", 2},
#line 97 "./src/musicxml_harmony.gperf"
      {"minor-13th", 76},
      {""},
#line 64 "./src/musicxml_harmony.gperf"
      {"dominantb9#5", 43},
      {""}, {""},
#line 74 "./src/musicxml_harmony.gperf"
      {"major-13th", 53},
      {""},
#line 22 "./src/musicxml_harmony.gperf"
      {"augmented-seventh", 1},
      {""}, {""},
#line 58 "./src/musicxml_harmony.gperf"
      {"dominantb5", 37},
      {""},
#line 42 "./src/musicxml_harmony.gperf"
      {"dominant#9b5", 21},
      {""}, {""},
#line 94 "./src/musicxml_harmony.gperf"
      {"minor", 73},
#line 125 "./src/musicxml_harmony.gperf"
      {"German", 104},
#line 60 "./src/musicxml_harmony.gperf"
      {"dominantb5#9", 39},
#line 75 "./src/musicxml_harmony.gperf"
      {"major-13th#11", 54},
      {""},
#line 71 "./src/musicxml_harmony.gperf"
      {"major", 50},
#line 59 "./src/musicxml_harmony.gperf"
      {"dominantb5#5b9#9", 38},
      {""},
#line 44 "./src/musicxml_harmony.gperf"
      {"dominant#9b13", 23},
#line 24 "./src/musicxml_harmony.gperf"
      {"augmented-seventhb9", 3},
#line 112 "./src/musicxml_harmony.gperf"
      {"other", 91},
#line 66 "./src/musicxml_harmony.gperf"
      {"dominantb9b13#11", 45},
#line 39 "./src/musicxml_harmony.gperf"
      {"dominant#5b9", 18},
      {""}, {""},
#line 113 "./src/musicxml_harmony.gperf"
      {"power", 92},
      {""}, {""}, {""},
#line 85 "./src/musicxml_harmony.gperf"
      {"major-13th9#11", 64},
      {""}, {""},
#line 63 "./src/musicxml_harmony.gperf"
      {"dominantb9b5", 42},
      {""}, {""}, {""}, {""}, {""},
#line 31 "./src/musicxml_harmony.gperf"
      {"diminished-seventh", 10},
#line 81 "./src/musicxml_harmony.gperf"
      {"major-ninth#11", 60},
      {""},
#line 98 "./src/musicxml_harmony.gperf"
      {"minor-ninth", 77},
      {""},
#line 69 "./src/musicxml_harmony.gperf"
      {"dominantb9b13", 48},
      {""},
#line 70 "./src/musicxml_harmony.gperf"
      {"half-diminished", 49},
#line 80 "./src/musicxml_harmony.gperf"
      {"major-ninth", 59},
#line 61 "./src/musicxml_harmony.gperf"
      {"dominantb5b9", 40},
#line 99 "./src/musicxml_harmony.gperf"
      {"minor-ninthb5", 78},
      {""}, {""}, {""},
#line 107 "./src/musicxml_harmony.gperf"
      {"minor-sixth9", 86},
      {""}, {""}, {""}, {""},
#line 90 "./src/musicxml_harmony.gperf"
      {"major-sixth9", 69},
      {""}, {""}, {""},
#line 106 "./src/musicxml_harmony.gperf"
      {"minor-sixth", 85},
      {""}, {""}, {""}, {""},
#line 89 "./src/musicxml_harmony.gperf"
      {"major-sixth", 68},
#line 78 "./src/musicxml_harmony.gperf"
      {"major-minor9", 57},
      {""},
#line 79 "./src/musicxml_harmony.gperf"
      {"major-minor911", 58},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 77 "./src/musicxml_harmony.gperf"
      {"major-minorb5", 56},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 116 "./src/musicxml_harmony.gperf"
      {"suspended-fourth7", 95},
#line 118 "./src/musicxml_harmony.gperf"
      {"suspended-fourth79", 97},
#line 121 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb97", 100},
#line 120 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb137", 99},
      {""}, {""}, {""}, {""}, {""},
#line 115 "./src/musicxml_harmony.gperf"
      {"suspended-fourth", 94},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 122 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb57", 101},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 119 "./src/musicxml_harmony.gperf"
      {"suspended-fourth791113", 98},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 117 "./src/musicxml_harmony.gperf"
      {"suspended-fourth73", 96},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 76 "./src/musicxml_harmony.gperf"
      {"major-minor", 55}
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
#line 129 "./src/musicxml_harmony.gperf"

#endif /* IREALPRO_CHORD_H */

