#include "irealpro.h"

int is_unvalid_time_signature(uint32_t b, uint32_t bt)
{
	return (b < 2 || bt < 2 || bt > 8 
			|| (b > 7 && bt == 4) || (b > 3 && bt == 2)
			|| ((b < 5 || (b > 9 && b != 12)) && bt == 8));
}

const char*	get_note_str(NoteEnum note)
{
    static const char *notes[NOTE_MAX] = {
        NULL, 
        "Ab", "A", "A#",
        "Bb", "B", "B#",
        "Cb", "C", "C#",
        "Db", "D", "D#",
        "Eb", "E", "E#",
        "Fb", "F", "F#",
        "Gb", "G", "G#",
    };
    return (note >= 1 && note < NOTE_MAX ? notes[note] : NULL);
}

const char *get_style_str(StyleEnum style)
{
	static const char *styles[STYLE_MAX] = {
		NULL,
		"Medium Swing",
		"Ballad",
		"Rock",
		"Pop"
	};
	return (style >= 1 && style < STYLE_MAX ? styles[style] : NULL);
}

void	irp_song_free(t_irealpro_song* song)
{
    free(song->measures.items);
}

