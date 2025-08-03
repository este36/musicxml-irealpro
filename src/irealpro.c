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

// returns 1 on failure, 0 on success
int	irp_song_apply_zoom(t_irealpro_song* song)
{
	if (song->zoom == ZOOM_OUT && (song->measures.count % 2 != 0))
		return 1;
	t_measure *m1 = song->first_measure;
	t_measure *new = m1;
	t_measure *m2;

	switch (song->zoom) {
	case ZOOM_IN:
	{
		while (m1)
		{
		}
		return 0;
	}
	case ZOOM_OUT:
	{
		while (m1)
		{
			m2 = m1->next; // null case already checked
			if (m1->chords.count > 2 || m2->chords.count > 2
				|| m1->barlines[1] != '\0'
				|| m1->time_signature.beats != m2->time_signature.beats
				|| m1->time_signature.beat_type != m2->time_signature.beat_type)
				return 1;
			for (size_t i = 0; i < m1->chords.count; i++) {
				new->chords.items[i] = m1->chords.items[i];
				new->chords.items[i].duration /= 2;
			}
			new->chords.count = m1->chords.count;
			for (size_t i = 0; i < m2->chords.count; i++) {
				new->chords.items[new->chords.count + i] = m2->chords.items[i];
				new->chords.items[new->chords.count + i].duration /= 2;
			}
			new->chords.count += m2->chords.count;
			new->barlines[1] = m2->barlines[1];
			new->rehearsal = m1->rehearsal;
			new->playback = m1->playback;
			new->ending = m1->ending;
			m1->rehearsal = 0;
			m1->playback = 0;
			m1->ending = 0;
			m2->rehearsal = 0;
			m2->playback = 0;
			m2->ending = 0;
			m1 = m2->next;
			if (m1) new = new->next;
			else new->next = NULL;
		}
		song->measures.count /= 2;
		return 0;
	}
	default:
		return 0;
	}
}
