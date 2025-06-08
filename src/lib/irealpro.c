#include "irealpro.h"

void irp_song_free(IrpSong* song)
{
    free(song->measures.items);
}
