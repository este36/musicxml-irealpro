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
	song->measures.items = NULL;
	free(song);
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
	case ZOOM_NONE:
		return 0;
	case ZOOM_IN:
	{
		while (m1)
		{
			if (is_unvalid_time_signature(m1->time_signature.beats, m1->time_signature.beat_type)) {
				if (is_unvalid_time_signature(m1->time_signature.beats, m1->time_signature.beat_type / 2))
					return 1;
				m1->time_signature.beat_type /= 2;
				m1 = m1->next;
			} else {
				double bar_duration = 0;

				for (uint32_t i = 0; i < m1->chords.count; i++)
					bar_duration += m1->chords.items[i].duration;
				da_append(song->measures, (t_measure){0});
				song->measures.items[song->measures.count - 1].next = m1->next;
				m1->next = &song->measures.items[song->measures.count - 1];
				m1->next->last = m1;
				m2 = m1->next;
				m2->divisions = m1->divisions;
				m2->time_signature = m1->time_signature;
				switch (m1->chords.count) {
					case 1:
						m2->chords.count = 1;
						m2->chords.items[0].duration = bar_duration;
						break;
					case 2:
						// 3 possibilities : A_B_, AB__, A__B
						if (duration_is_equiv(m1->chords.items[0].duration, m1->chords.items[1].duration)) {
							m2->chords.items[0] = m1->chords.items[1];
							m1->chords.count = m2->chords.count = 1;
						} else if (m1->chords.items[0].duration > m1->chords.items[1].duration) {
							m2->chords = m1->chords;
							m1->chords.items[0].duration = bar_duration;
							m1->chords.count = 1;
						} else {
							m2->chords.items[0] = m1->chords.items[1];
							m2->chords.items[0].duration = bar_duration;;
							m2->chords.count = 1;
						}
						break;
					case 3:
						if (duration_is_equiv(
									m1->chords.items[0].duration + m1->chords.items[1].duration,
									m1->chords.items[2].duration)) {
							// the third chord is always at m2
							m2->chords.items[0] = m1->chords.items[2];
							m2->chords.count = 1;
							m1->chords.count = 2;
						} else if (duration_is_equiv(
									m1->chords.items[0].duration,
									m1->chords.items[1].duration + m1->chords.items[2].duration)) {
							m2->chords.items[0] = m1->chords.items[1];
							m2->chords.items[1] = m1->chords.items[2];
							m2->chords.count = 2;
							m1->chords.count = 1;
						}
						break;
					case 4:
						// very basic
						m2->chords.items[0] = m1->chords.items[2];
						m2->chords.items[1] = m1->chords.items[3];
						m2->chords.count = 2;
						m1->chords.count = 2;
					default: break;
				}
				m1->next->barlines[1] = m1->barlines[1];
				m1->barlines[1] = '\0';
				m1 = m2->next;
			}
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
			if (m1) new->next = new + 1;
			else new->next = NULL;
			new = new->next;
		}
		song->measures.count /= 2;
		return 0;
	}
	default:
		return 0;
	}
}

void	irp_playlist_append(t_irealpro_playlist *playlist, t_irealpro_song *song)
{
	da_append(playlist->songs, song);
}

t_irealpro_playlist *irp_playlist_create(const char *title)
{
	t_irealpro_playlist *playlist = calloc(1, sizeof(t_irealpro_playlist));
	if (playlist == NULL)
		return NULL;
	if (*title != '\0') da_strcat(&playlist->title, title);
	else da_strcat(&playlist->title, "Playlist Name");
	return playlist;
}

void	irp_playlist_free(t_irealpro_playlist *playlist)
{
	for (size_t i = 0; i < playlist->songs.count; i++) {
		irp_song_free(playlist->songs.items[i]);
	}
	free(playlist->title.buf);
	free(playlist->songs.items);
	free(playlist);
}
