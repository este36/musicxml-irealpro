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

static void url_strcat(da_str *dst, const char *src)
{
	static const char hex_base[17] = "0123456789ABCDEF";
	char	buf[4];
	int		i;

	i = 0;
	buf[3] = '\0';
	while(src[i])
	{
		if (isalnum(src[i]) || src[i] == '-' || src[i] == '_'
			|| src[i] == '.' || src[i] == '~') {
			buf[0] = src[i];
			buf[1] = '\0';
		} else {
			buf[0] = '%';
			buf[1] = hex_base[src[i] >> 4];
			buf[2] = hex_base[src[i] & 0x0F];
		}
		da_strcat(dst, buf);
		i++;
	}
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
	url_strcat(dst, title_to_copy);
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

static void append_playback(da_str *dst, PlaybackEnum p)
{
	static const char *playbacks[PLAYBACK_MAX] = {
		NULL,
		"Q",
		"S",
		"<Fine>",
		"<D.C. al Coda>",
		"<D.C. al Fine>",
		"<D.S. al Coda>",
		"<D.S. al Fine>"
	};
	if (p > 0 && p < PLAYBACK_MAX)
		url_strcat(dst, playbacks[p]);
}

static void append_rehearsal(da_str *dst, RehearsalEnum r)
{
	static const char *rehearsals[REHEARSAL_MAX] = {
		NULL, "*i", "*V", "*A", "*B", "*C", "*D"
	};
	if (r > 0 && r < REHEARSAL_MAX)
		url_strcat(dst, rehearsals[r]);
}

static void append_ending(da_str *dst, EndingEnum e)
{
	static const char *endings[ENDING_MAX] = {
		NULL, "N1", "N2", "N3"
	};
	if (e > 0 && e < ENDING_MAX)
		da_strcat(dst, endings[e]);
}

static void append_chord(da_str *dst, t_chord *c, bool should_repeat)
{
	if (c->quality[0] == 'x') { // this indicate the 'repeat last chord' symbol
		if (should_repeat) da_strcat(dst, "x");
		else da_strcat(dst, " ");
		return;
	}
	const char *root_note = get_note_str(c->root);
	if (root_note == NULL) {
		if (should_repeat) da_strcat(dst, "x");
		else url_strcat(dst, " ");
		return;
	}
	const char *bass_note = get_note_str(c->bass);
	url_strcat(dst, root_note);
	url_strcat(dst, c->quality);
	if (bass_note != NULL) {
		url_strcat(dst, "/");
		url_strcat(dst, bass_note);
	}
}

static int duration_is_equiv(int d1, int d2, t_measure *m)
{
	double middle = (m->divisions * m->time_signature.beats) / 2;
	double middle_range_l = middle * 0.75;
	double middle_range_r = middle * 1.25;

	if (d1 < middle_range_l
		|| d1 > middle_range_r
		|| d2 < middle_range_l
		|| d2 > middle_range_r)
	{
		return 0;
	}
	return 1;
}

// return 1 if it ends with a small chord, so if the last bar was small we can put l
static int	append_chords(da_str *dst, t_measure *m, int is_s)
{
	switch (m->chords.count) {
	case 1:
		if (is_s) url_strcat(dst, "l");
		append_chord(dst, &m->chords.items[0], true);
		url_strcat(dst, "   "); // 3 spaces
		return 0;
	case 2:
		// three possible setup: A_B_ or A_AB or ABB_
		if (duration_is_equiv(m->chords.items[0].duration, m->chords.items[1].duration, m)) {
			if (is_s) url_strcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, " ");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, " ");
		} else if (m->chords.items[0].duration > m->chords.items[1].duration) {
			if (is_s) url_strcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, " s");
			append_chord(dst, &m->chords.items[0], false);
			url_strcat(dst, ",");
			append_chord(dst, &m->chords.items[1], false);
			return 1;
		} else {
			if (!is_s) url_strcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, ",");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, ",l");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, " ");
		}
		return 0;
	case 3:
		// three possible setup: A_BC or ABC_ or AB_C
		if (duration_is_equiv(
					m->chords.items[0].duration,
					m->chords.items[1].duration + m->chords.items[2].duration,
					m)) {
			if (is_s) url_strcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, " s");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, ",");
			append_chord(dst, &m->chords.items[2], false);
			return 1;
		} else if (duration_is_equiv(
					m->chords.items[0].duration + m->chords.items[1].duration,
					m->chords.items[2].duration,
					m)) {
			if (!is_s) url_strcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, ",");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, ",l");
			append_chord(dst, &m->chords.items[2], false);
			url_strcat(dst, " ");
			return 0;
		} else {
			if (!is_s) url_strcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			url_strcat(dst, ",l");
			append_chord(dst, &m->chords.items[1], false);
			url_strcat(dst, " s");
			append_chord(dst, &m->chords.items[2], false);
			return 1;
		}
	case 4:
		if (!is_s) url_strcat(dst, "s");
		append_chord(dst, &m->chords.items[0], true);
		url_strcat(dst, ",");
		append_chord(dst, &m->chords.items[1], false);
		url_strcat(dst, ",");
		append_chord(dst, &m->chords.items[2], false);
		url_strcat(dst, ",");
		append_chord(dst, &m->chords.items[3], false);
		return 1;
	default: return 0;
	}
}

static int	append_song_body(da_str *dst, t_irealpro_song *song)
{
	t_measure			*m;
	char				barline_buf[2];
	t_time_signature	curr_ts = {0};
	int					is_s = 0;

	barline_buf[1] = '\0';
	m = &song->measures.items[0];
	while (m != NULL)
	{
		if (m->barlines[0])
			barline_buf[0] = m->barlines[0];
		else
			barline_buf[0] = '|';
		url_strcat(dst, barline_buf);
		if (curr_ts.beats != m->time_signature.beats
			|| curr_ts.beat_type != m->time_signature.beat_type) {
			append_time_signature(dst, m);
			curr_ts = m->time_signature;
		}
		append_ending(dst, m->ending);
		append_rehearsal(dst, m->rehearsal);
		append_playback(dst, m->playback);
		is_s = append_chords(dst, m, is_s);
		if (m->barlines[1]) {
			barline_buf[0] = m->barlines[1];
			url_strcat(dst, barline_buf);
		}
		m = m->next;
	}
	return 0;
}

static void append_composer(da_str *dst, char *composer)
{
	if (composer[0] == '\0') {
		url_strcat(dst, "Composer Unknown");
		return;
	}

	char *last_name = composer;
	while (!isspace(*last_name) && *last_name != '\0')
		last_name++;
	if (*last_name == '\0') {
		url_strcat(dst, composer);
		return;
	}
	*last_name = '\0';
	last_name++;
	while(isspace(*last_name))
		last_name++;

	if (*last_name == '\0') {
		url_strcat(dst, composer);
		return;
	}
	url_strcat(dst, last_name);
	da_strcat(dst, "%20"); // url encoded space
	url_strcat(dst, composer); // first name
}

static	int	append_song(da_str *dst, t_irealpro_song *song)
{
	append_song_title(dst, song->title);
	da_strcat(dst, "=");
	append_composer(dst, song->composer);
	da_strcat(dst, "=");
	const char *style = get_style_str(song->style);
	if (style != NULL)
		url_strcat(dst, style);
	else
		da_strcat(dst, "Medium%20Swing");
	da_strcat(dst, "=");
	const char *key = get_note_str(song->key);
	if (key != NULL)
		da_strcat(dst, key);
	else
		da_strcat(dst, "C");
	da_strcat(dst, "=n=");
	append_song_body(dst, song);
	return 0;
}

char	*irp_get_song_html(t_irealpro_song *song)
{
	da_str	res;

	if (((song->zoom == ZOOM_OUT) && (song->measures.count % 2 != 0))
		|| song->measures.count == 0
		|| da_str_init(&res, 512) != 0) // irealpro urls are long usually
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
