#include "irealpro.h"

const char* get_note_str(NoteEnum note)
{
    const char* notes[NOTE_MAX] = {
        NULL, 
        "Ab", "A", "A#",
        "Bb", "B", "B#",
        "Cb", "C", "C#",
        "Db", "D", "D#",
        "Eb", "E", "E#",
        "Fb", "F", "F#",
        "Gb", "G", "G#",
    };
    return note >= 1 && note < NOTE_MAX ? notes[note] : NULL;
}

void irp_song_free(t_irealpro_song* song)
{
    free(song->measures.items);
}
