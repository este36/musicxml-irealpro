from musicxml import chords

entries_str = '\n'.join(f"{k}, {i}" for i, (k,v) in enumerate(chords.items())) 

print(f'''%{{
#include <string.h>
%}}
%language=C
%readonly-tables
%define hash-function-name my_hash
%define lookup-function-name irealpro_chord

struct keyword {{ const char *name; int id; }};
%%
{entries_str}
%%''')
