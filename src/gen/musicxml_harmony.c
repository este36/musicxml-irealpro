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

const char *
irealpro_chord (str, len)
     register const char *str;
     register size_t len;
{
  static const char * const wordlist[] =
    {
      "", "", "", "", "",
      "major",
      "major9",
      "major11",
      "", "", "", "", "", "", "",
      "minor",
      "minor9",
      "minorb6",
      "",
      "none",
      "power",
      "",
      "minor#5",
      "", "",
      "dominantb9",
      "",
      "dominantb9#9",
      "dominantb9#11",
      "",
      "dominant#9",
      "",
      "dominantb5#9",
      "dominant#9#11",
      "",
      "dominantb5",
      "major-seventh#11",
      "dominant#5#9",
      "",
      "minor-seventh9",
      "dominant#5",
      "dominantb5#5b9#9",
      "major-seventh#5#9",
      "major-seventh",
      "",
      "major-seventh#5",
      "minor-seventhb59",
      "dominantb5b9",
      "",
      "augmented",
      "dominant-13thb9",
      "repeat",
      "dominant#5b9",
      "minor-seventh",
      "",
      "dominant-13th#9",
      "dominant-13th#11",
      "", "", "",
      "diminished",
      "major2",
      "",
      "dominant-13th",
      "",
      "half-diminished",
      "major-minor",
      "major-minor9",
      "dominant",
      "major-ninth#11",
      "",
      "dominant-ninthb5",
      "dominant-ninth#11",
      "major-13th#11",
      "", "",
      "dominant-ninth#5",
      "",
      "diminished-seventh",
      "dominant-ninth",
      "",
      "major-ninth",
      "major-sixth9",
      "", "",
      "major-13th",
      "dominant#11",
      "", "", "", "",
      "minor-ninth",
      "minor-sixth9",
      "", "",
      "minor-11th",
      "major-sixth",
      "", "", "", "", "", "", "", "", "",
      "minor-sixth",
      "suspended-fourth7",
      "suspended-fourth79",
      "suspended-fourthb97",
      "suspended-fourthb137",
      "dominantb13",
      "", "", "", "",
      "major-seventh913",
      "",
      "dominant-11th",
      "", "",
      "suspended-fourth",
      "", "", "", "",
      "minor-seventhb13",
      "",
      "dominantb9b13",
      "", "", "", "", "", "", "",
      "suspended-second",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "",
      "dominantb913",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "",
      "suspended-fourth73",
      "", "", "",
      "suspended-fourth791113"
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = my_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key];

          if (*str == *s && !strcmp (str + 1, s + 1))
            return s;
        }
    }
  return 0;
}
#line 81 "./src/gen/musicxml_harmony.gperf"

