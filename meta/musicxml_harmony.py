from musicxml import chords

# gperf trandorfm key to index -> then we use index for value
entries_str = '\n'.join(f"{k}, {i}" for i, (k,v) in enumerate(chords.items())) 

print(f'''%{{
#ifndef IREALPRO_CHORD_H
#define IREALPRO_CHORD_H
#include <string.h>
#if defined(__clang__)
  #pragma clang diagnostic ignored "-Wmissing-field-initializers"
  #pragma clang diagnostic ignored "-Wduplicate-decl-specifier"

#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
%}}
%language=ANSI-C
%struct-type
%readonly-tables
%define hash-function-name my_hash
%define lookup-function-name irealpro_chord_lookup

struct keyword {{ const char *name; int id; }};
%%
{entries_str}
%%
#endif /* IREALPRO_CHORD_H */
''')
