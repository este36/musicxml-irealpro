/* C code produced by gperf version 3.1 */
/* Command-line: gperf ./src/gen/musicxml_harmony.gperf  */
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
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "./src/gen/musicxml_harmony.gperf"

#include <string.h>
#line 10 "./src/gen/musicxml_harmony.gperf"
struct keyword { const char *name; int id; };

#define TOTAL_KEYWORDS 70
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 197
/* maximum key range = 193, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
my_hash (str, len)
     register const char *str;
     register size_t len;
{
  static const unsigned char asso_values[] =
    {
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198,   5,   0, 198, 198, 198,
      198, 198, 198, 198, 198,  20,  15, 198, 198,   0,
       55,  85,   0,   5,   0,   0, 198,   0, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198,   0,   0,  15,
       20,   0, 198,   5,  15,  10,   0, 198, 198, 198,
        0,  15, 198, 198,   0,  40,  45,   0,   0, 198,
       15, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
      198, 198, 198, 198, 198, 198, 198
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

const struct keyword *
irealpro_chord_lookup (str, len)
     register const char *str;
     register size_t len;
{
  static const struct keyword wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 42 "./src/gen/musicxml_harmony.gperf"
      {"major", 30},
#line 58 "./src/gen/musicxml_harmony.gperf"
      {"major9", 46},
#line 56 "./src/gen/musicxml_harmony.gperf"
      {"major11", 44},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 59 "./src/gen/musicxml_harmony.gperf"
      {"minor", 47},
#line 69 "./src/gen/musicxml_harmony.gperf"
      {"minor9", 57},
#line 70 "./src/gen/musicxml_harmony.gperf"
      {"minorb6", 58},
      {""},
#line 71 "./src/gen/musicxml_harmony.gperf"
      {"none", 59},
#line 72 "./src/gen/musicxml_harmony.gperf"
      {"power", 60},
      {""},
#line 60 "./src/gen/musicxml_harmony.gperf"
      {"minor#5", 48},
      {""}, {""},
#line 36 "./src/gen/musicxml_harmony.gperf"
      {"dominantb9", 24},
      {""},
#line 38 "./src/gen/musicxml_harmony.gperf"
      {"dominantb9#9", 26},
#line 37 "./src/gen/musicxml_harmony.gperf"
      {"dominantb9#11", 25},
      {""},
#line 20 "./src/gen/musicxml_harmony.gperf"
      {"dominant#9", 8},
      {""},
#line 34 "./src/gen/musicxml_harmony.gperf"
      {"dominantb5#9", 22},
#line 21 "./src/gen/musicxml_harmony.gperf"
      {"dominant#9#11", 9},
      {""},
#line 32 "./src/gen/musicxml_harmony.gperf"
      {"dominantb5", 20},
#line 50 "./src/gen/musicxml_harmony.gperf"
      {"major-seventh#11", 38},
#line 18 "./src/gen/musicxml_harmony.gperf"
      {"dominant#5#9", 6},
      {""},
#line 64 "./src/gen/musicxml_harmony.gperf"
      {"minor-seventh9", 52},
#line 17 "./src/gen/musicxml_harmony.gperf"
      {"dominant#5", 5},
#line 33 "./src/gen/musicxml_harmony.gperf"
      {"dominantb5#5b9#9", 21},
#line 52 "./src/gen/musicxml_harmony.gperf"
      {"major-seventh#5#9", 40},
#line 49 "./src/gen/musicxml_harmony.gperf"
      {"major-seventh", 37},
      {""},
#line 51 "./src/gen/musicxml_harmony.gperf"
      {"major-seventh#5", 39},
#line 66 "./src/gen/musicxml_harmony.gperf"
      {"minor-seventhb59", 54},
#line 35 "./src/gen/musicxml_harmony.gperf"
      {"dominantb5b9", 23},
      {""},
#line 12 "./src/gen/musicxml_harmony.gperf"
      {"augmented", 0},
#line 26 "./src/gen/musicxml_harmony.gperf"
      {"dominant-13thb9", 14},
#line 73 "./src/gen/musicxml_harmony.gperf"
      {"repeat", 61},
#line 19 "./src/gen/musicxml_harmony.gperf"
      {"dominant#5b9", 7},
#line 63 "./src/gen/musicxml_harmony.gperf"
      {"minor-seventh", 51},
      {""},
#line 25 "./src/gen/musicxml_harmony.gperf"
      {"dominant-13th#9", 13},
#line 24 "./src/gen/musicxml_harmony.gperf"
      {"dominant-13th#11", 12},
      {""}, {""}, {""},
#line 13 "./src/gen/musicxml_harmony.gperf"
      {"diminished", 1},
#line 57 "./src/gen/musicxml_harmony.gperf"
      {"major2", 45},
      {""},
#line 23 "./src/gen/musicxml_harmony.gperf"
      {"dominant-13th", 11},
      {""},
#line 41 "./src/gen/musicxml_harmony.gperf"
      {"half-diminished", 29},
#line 45 "./src/gen/musicxml_harmony.gperf"
      {"major-minor", 33},
#line 46 "./src/gen/musicxml_harmony.gperf"
      {"major-minor9", 34},
#line 15 "./src/gen/musicxml_harmony.gperf"
      {"dominant", 3},
#line 48 "./src/gen/musicxml_harmony.gperf"
      {"major-ninth#11", 36},
      {""},
#line 30 "./src/gen/musicxml_harmony.gperf"
      {"dominant-ninthb5", 18},
#line 28 "./src/gen/musicxml_harmony.gperf"
      {"dominant-ninth#11", 16},
#line 44 "./src/gen/musicxml_harmony.gperf"
      {"major-13th#11", 32},
      {""}, {""},
#line 29 "./src/gen/musicxml_harmony.gperf"
      {"dominant-ninth#5", 17},
      {""},
#line 14 "./src/gen/musicxml_harmony.gperf"
      {"diminished-seventh", 2},
#line 27 "./src/gen/musicxml_harmony.gperf"
      {"dominant-ninth", 15},
      {""},
#line 47 "./src/gen/musicxml_harmony.gperf"
      {"major-ninth", 35},
#line 55 "./src/gen/musicxml_harmony.gperf"
      {"major-sixth9", 43},
      {""}, {""},
#line 43 "./src/gen/musicxml_harmony.gperf"
      {"major-13th", 31},
#line 16 "./src/gen/musicxml_harmony.gperf"
      {"dominant#11", 4},
      {""}, {""}, {""}, {""},
#line 62 "./src/gen/musicxml_harmony.gperf"
      {"minor-ninth", 50},
#line 68 "./src/gen/musicxml_harmony.gperf"
      {"minor-sixth9", 56},
      {""}, {""},
#line 61 "./src/gen/musicxml_harmony.gperf"
      {"minor-11th", 49},
#line 54 "./src/gen/musicxml_harmony.gperf"
      {"major-sixth", 42},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 67 "./src/gen/musicxml_harmony.gperf"
      {"minor-sixth", 55},
#line 75 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourth7", 63},
#line 77 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourth79", 65},
#line 80 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourthb97", 68},
#line 79 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourthb137", 67},
#line 31 "./src/gen/musicxml_harmony.gperf"
      {"dominantb13", 19},
      {""}, {""}, {""}, {""},
#line 53 "./src/gen/musicxml_harmony.gperf"
      {"major-seventh913", 41},
      {""},
#line 22 "./src/gen/musicxml_harmony.gperf"
      {"dominant-11th", 10},
      {""}, {""},
#line 74 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourth", 62},
      {""}, {""}, {""}, {""},
#line 65 "./src/gen/musicxml_harmony.gperf"
      {"minor-seventhb13", 53},
      {""},
#line 40 "./src/gen/musicxml_harmony.gperf"
      {"dominantb9b13", 28},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 81 "./src/gen/musicxml_harmony.gperf"
      {"suspended-second", 69},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 39 "./src/gen/musicxml_harmony.gperf"
      {"dominantb913", 27},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 76 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourth73", 64},
      {""}, {""}, {""},
#line 78 "./src/gen/musicxml_harmony.gperf"
      {"suspended-fourth791113", 66}
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
#line 82 "./src/gen/musicxml_harmony.gperf"

