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

static void	append_song_title(da_str *dst, char *title)
{
	char title_to_copy[MAX_CREDENTIALS];

	title_to_copy[0] = '\0';
	if (title[0] == 'T'
		&& title[1] == 'h'
		&& title[2] == 'e'
		&& title[3] == ' ')
	{
		strcat(title_to_copy, "The, ");
		strcat(title_to_copy, title + 4);
	}
	else
	{
		strcat(title_to_copy, title);
	}
	da_strcat(dst, title_to_copy);
}

static void append_time_signature(da_str *dst, const t_measure *m)
{
	uint32_t b = m->time_signature.beats; // 3/4 -> 4
	uint32_t bt = m->time_signature.beat_type; // 3/4 -> 3
	if (b == 0 && bt == 0)
		return ;
	char buf[4];
	buf[0] = 'T';
	if (b == 12) {
		buf[1] = '1';
		buf[2] = '2';
	} else {
		buf[1] = b + '0';
		buf[2] = bt + '0';
	}
	buf[3] = '\0';
	da_strcat(dst, buf);
}

// append_rehearsal(da_str *dst, char *rehearsal)
// {
// }

static int	append_song_body(da_str *dst, t_irealpro_song *song)
{
	size_t		i;
	t_measure	*m;
	char		barline_buf[2];

	i = 0;
	barline_buf[1] = '\0';
	while (i < song->measures.count)
	{
		m = &song->measures.items[i];
		if (m->barlines[0])
			barline_buf[0] = m->barlines[0];
		else
			barline_buf[0] = '|';
		da_strcat(dst, barline_buf);
		append_time_signature(dst, m);
		da_strcat(dst, "__CHORDS__");
		if (m->barlines[1]) {
			barline_buf[0] = m->barlines[1];
			da_strcat(dst, barline_buf);
		}
		i++;
	}
	return 0;
}

static	int	append_song(da_str *dst, t_irealpro_song *song)
{
	append_song_title(dst, song->title);
	da_strcat(dst, "=");
	if (song->composer[0])
		da_strcat(dst, song->composer);
	else
		da_strcat(dst, "Composer Unknown");
	da_strcat(dst, "=");
	const char *style = get_style_str(song->style);
	if (style != NULL)
		da_strcat(dst, style);
	else
		da_strcat(dst, "Medium Swing");
	da_strcat(dst, "=");
	const char *key = get_note_str(song->key);
	if (key != NULL)
		da_strcat(dst, key);
	else
		da_strcat(dst, "C");
	da_strcat(dst, "=n=");
	return (append_song_body(dst, song));
}

char	*irp_get_song_html(t_irealpro_song *song)
{
	da_str	res;

	if (da_str_init(&res, 512) != 0) // irealpro urls are long usually
		return NULL;
	da_strcat(&res, "<a href=\"irealbook://"); // we use the open url scheme
	if (append_song(&res, song) != 0) {
		free(res.buf);
		return NULL;
	}
	da_strcat(&res, "\">");
	append_song_title(&res, song->title);
	da_strcat(&res, "</a>");
	return res.buf;
}
