#include "irealpro.h"

void irp_song_init(IrpSong* song)
{
    memset(song->composer, 0, sizeof(song->composer));
    memset(song->title, 0, sizeof(song->title));
}
