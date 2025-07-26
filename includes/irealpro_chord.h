/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --language=ANSI-C ./src/musicxml_harmony.gperf  */
/* Computed positions: -k'2,9-11,14-15,$' */

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
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#line 13 "./src/musicxml_harmony.gperf"
struct keyword { const char *name; int id; };

#define TOTAL_KEYWORDS 73
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 177
/* maximum key range = 172, duplicates = 0 */

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
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178,   5,   0, 178, 178, 178,
      178, 178, 178, 178, 178,  30,   0, 178, 178,   0,
       90,  30,   0,   5,   0,   0, 178,   0,   5, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178,   5,   0,  25,
       35,   0, 178,   0,   0,  10,  15, 178, 178, 178,
        0,  15, 178, 178,  60,  35,   0,   0,   5, 178,
       65, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
      178, 178, 178, 178, 178, 178, 178
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
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
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
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
#line 79 "./src/musicxml_harmony.gperf"
      {"repeat", 64},
      {""}, {""}, {""}, {""},
#line 63 "./src/musicxml_harmony.gperf"
      {"major9", 48},
#line 61 "./src/musicxml_harmony.gperf"
      {"major11", 46},
      {""}, {""},
#line 46 "./src/musicxml_harmony.gperf"
      {"major-13th", 31},
#line 74 "./src/musicxml_harmony.gperf"
      {"minor9", 59},
#line 75 "./src/musicxml_harmony.gperf"
      {"minorb6", 60},
#line 47 "./src/musicxml_harmony.gperf"
      {"major-13th#11", 32},
#line 77 "./src/musicxml_harmony.gperf"
      {"none", 62},
#line 66 "./src/musicxml_harmony.gperf"
      {"minor-11th", 51},
      {""},
#line 65 "./src/musicxml_harmony.gperf"
      {"minor#5", 50},
#line 18 "./src/musicxml_harmony.gperf"
      {"dominant", 3},
#line 55 "./src/musicxml_harmony.gperf"
      {"major-13th9#11", 40},
#line 39 "./src/musicxml_harmony.gperf"
      {"dominantb9", 24},
#line 50 "./src/musicxml_harmony.gperf"
      {"major-ninth", 35},
#line 41 "./src/musicxml_harmony.gperf"
      {"dominantb9#9", 26},
#line 40 "./src/musicxml_harmony.gperf"
      {"dominantb9#11", 25},
#line 51 "./src/musicxml_harmony.gperf"
      {"major-ninth#11", 36},
#line 23 "./src/musicxml_harmony.gperf"
      {"dominant#9", 8},
#line 67 "./src/musicxml_harmony.gperf"
      {"minor-ninth", 52},
#line 37 "./src/musicxml_harmony.gperf"
      {"dominantb5#9", 22},
#line 24 "./src/musicxml_harmony.gperf"
      {"dominant#9#11", 9},
      {""},
#line 35 "./src/musicxml_harmony.gperf"
      {"dominantb5", 20},
      {""},
#line 21 "./src/musicxml_harmony.gperf"
      {"dominant#5#9", 6},
#line 52 "./src/musicxml_harmony.gperf"
      {"major-seventh", 37},
      {""},
#line 20 "./src/musicxml_harmony.gperf"
      {"dominant#5", 5},
#line 36 "./src/musicxml_harmony.gperf"
      {"dominantb5#5b9#9", 21},
      {""},
#line 68 "./src/musicxml_harmony.gperf"
      {"minor-seventh", 53},
#line 69 "./src/musicxml_harmony.gperf"
      {"minor-seventh9", 54},
      {""},
#line 53 "./src/musicxml_harmony.gperf"
      {"major-seventh#11", 38},
#line 54 "./src/musicxml_harmony.gperf"
      {"major-seventh9#11", 39},
#line 76 "./src/musicxml_harmony.gperf"
      {"minorb13", 61},
      {""}, {""},
#line 71 "./src/musicxml_harmony.gperf"
      {"minor-seventhb59", 56},
#line 57 "./src/musicxml_harmony.gperf"
      {"major-seventh#5#9", 42},
      {""}, {""},
#line 56 "./src/musicxml_harmony.gperf"
      {"major-seventh#5", 41},
#line 34 "./src/musicxml_harmony.gperf"
      {"dominantb13", 19},
#line 38 "./src/musicxml_harmony.gperf"
      {"dominantb5b9", 23},
#line 26 "./src/musicxml_harmony.gperf"
      {"dominant-13th", 11},
      {""},
#line 29 "./src/musicxml_harmony.gperf"
      {"dominant-13thb9", 14},
      {""},
#line 22 "./src/musicxml_harmony.gperf"
      {"dominant#5b9", 7},
      {""}, {""},
#line 28 "./src/musicxml_harmony.gperf"
      {"dominant-13th#9", 13},
#line 27 "./src/musicxml_harmony.gperf"
      {"dominant-13th#11", 12},
#line 49 "./src/musicxml_harmony.gperf"
      {"major-minor9", 34},
#line 17 "./src/musicxml_harmony.gperf"
      {"diminished-seventh", 2},
      {""},
#line 45 "./src/musicxml_harmony.gperf"
      {"major", 30},
#line 58 "./src/musicxml_harmony.gperf"
      {"major-seventh913", 43},
      {""}, {""},
#line 30 "./src/musicxml_harmony.gperf"
      {"dominant-ninth", 15},
#line 64 "./src/musicxml_harmony.gperf"
      {"minor", 49},
#line 70 "./src/musicxml_harmony.gperf"
      {"minor-seventhb13", 55},
      {""}, {""},
#line 15 "./src/musicxml_harmony.gperf"
      {"augmented", 0},
#line 78 "./src/musicxml_harmony.gperf"
      {"power", 63},
#line 33 "./src/musicxml_harmony.gperf"
      {"dominant-ninthb5", 18},
#line 31 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#11", 16},
#line 43 "./src/musicxml_harmony.gperf"
      {"dominantb9b13", 28},
      {""}, {""},
#line 32 "./src/musicxml_harmony.gperf"
      {"dominant-ninth#5", 17},
      {""}, {""}, {""},
#line 16 "./src/musicxml_harmony.gperf"
      {"diminished", 1},
#line 59 "./src/musicxml_harmony.gperf"
      {"major-sixth", 44},
#line 60 "./src/musicxml_harmony.gperf"
      {"major-sixth9", 45},
      {""}, {""}, {""},
#line 72 "./src/musicxml_harmony.gperf"
      {"minor-sixth", 57},
#line 73 "./src/musicxml_harmony.gperf"
      {"minor-sixth9", 58},
      {""}, {""}, {""},
#line 62 "./src/musicxml_harmony.gperf"
      {"major2", 47},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 44 "./src/musicxml_harmony.gperf"
      {"half-diminished", 29},
      {""}, {""}, {""}, {""}, {""},
#line 19 "./src/musicxml_harmony.gperf"
      {"dominant#11", 4},
      {""}, {""}, {""}, {""},
#line 48 "./src/musicxml_harmony.gperf"
      {"major-minor", 33},
      {""}, {""}, {""}, {""},
#line 87 "./src/musicxml_harmony.gperf"
      {"suspended-second", 72},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 80 "./src/musicxml_harmony.gperf"
      {"suspended-fourth", 65},
#line 81 "./src/musicxml_harmony.gperf"
      {"suspended-fourth7", 66},
#line 83 "./src/musicxml_harmony.gperf"
      {"suspended-fourth79", 68},
#line 86 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb97", 71},
#line 85 "./src/musicxml_harmony.gperf"
      {"suspended-fourthb137", 70},
      {""},
#line 42 "./src/musicxml_harmony.gperf"
      {"dominantb913", 27},
#line 25 "./src/musicxml_harmony.gperf"
      {"dominant-11th", 10},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 82 "./src/musicxml_harmony.gperf"
      {"suspended-fourth73", 67},
      {""}, {""}, {""},
#line 84 "./src/musicxml_harmony.gperf"
      {"suspended-fourth791113", 69}
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
#line 88 "./src/musicxml_harmony.gperf"

#endif /* IREALPRO_CHORD_H */

