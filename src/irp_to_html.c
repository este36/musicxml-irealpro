#include "irealpro.h"

static void append_time_signature(t_dstr *dst, const t_measure *m)
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
	dstrcat(dst, buf);
}

static void append_playback(t_dstr *dst, PlaybackEnum p)
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
		dstrcat(dst, playbacks[p]);
}

static void append_rehearsal(t_dstr *dst, RehearsalEnum r)
{
	static const char *rehearsals[REHEARSAL_MAX] = {
		NULL, "*i", "*V", "*A", "*B", "*C", "*D"
	};
	if (r > 0 && r < REHEARSAL_MAX)
		dstrcat(dst, rehearsals[r]);
}

static void append_ending(t_dstr *dst, EndingEnum e)
{
	static const char *endings[ENDING_MAX] = {
		NULL, "N1", "N2", "N3"
	};
	if (e > 0 && e < ENDING_MAX)
		dstrcat(dst, endings[e]);
}

static void append_chord(t_dstr *dst, t_chord *c, bool should_repeat)
{
	if (c->quality[0] == 'x') { // this indicate the 'repeat last chord' symbol
		if (should_repeat) dstrcat(dst, "x");
		else dstrcat(dst, " ");
		return;
	} else if (c->quality[0] == 'n') {
		dstrcat(dst, "n");
		return;
	}
	const char *root_note = get_note_str(c->root);
	if (root_note == NULL) {
		if (should_repeat) dstrcat(dst, "x");
		else dstrcat(dst, " ");
		return;
	}
	const char *bass_note = get_note_str(c->bass);
	dstrcat(dst, root_note);
	dstrcat(dst, c->quality);
	if (bass_note != NULL) {
		dstrcat(dst, "/");
		dstrcat(dst, bass_note);
	}
}

int duration_is_equiv(double d1, double d2)
{
	double middle = (d1 + d2) / 2;
	double middle_range_l = middle * 0.67;
	double middle_range_r = middle * 1.33;

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
static int	append_chords(t_dstr *dst, t_measure *m, int is_s)
{
	// quick fix, temporary
	if (m->is_too_much_chords) {
		if (is_s) dstrcat(dst, "l");
		dstrcat(dst, "n   ");
		return 0;
	}
	switch (m->chords.count) {
	case 1:
		if (is_s) dstrcat(dst, "l");
		append_chord(dst, &m->chords.items[0], true);
		dstrcat(dst, ",XyQ"); // 3 spaces
		return 0;
	case 2:
		// three possible setup: A_B_ or A_AB or ABB_
		if (duration_is_equiv(m->chords.items[0].duration, m->chords.items[1].duration)) {
			if (is_s) dstrcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, " ");
			append_chord(dst, &m->chords.items[1], false);
			dstrcat(dst, " ");
		} else if (m->chords.items[0].duration > m->chords.items[1].duration) {
			if (is_s) dstrcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, "  s");
			append_chord(dst, &m->chords.items[1], false);
			return 1;
		} else {
			if (!is_s) dstrcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, ",l");
			append_chord(dst, &m->chords.items[1], false);
			dstrcat(dst, "  ");
		}
		return 0;
	case 3:
		// three possible setup: A_BC or ABC_ or AB_C
		if (duration_is_equiv(
					m->chords.items[0].duration,
					m->chords.items[1].duration + m->chords.items[2].duration)) {
			if (is_s) dstrcat(dst, "l");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, " s");
			append_chord(dst, &m->chords.items[1], false);
			dstrcat(dst, ",");
			append_chord(dst, &m->chords.items[2], false);
			return 1;
		} else if (duration_is_equiv(
					m->chords.items[0].duration + m->chords.items[1].duration,
					m->chords.items[2].duration)) {
			if (!is_s) dstrcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, ",");
			append_chord(dst, &m->chords.items[1], false);
			dstrcat(dst, ",l");
			append_chord(dst, &m->chords.items[2], false);
			dstrcat(dst, " ");
			return 0;
		} else {
			if (!is_s) dstrcat(dst, "s");
			append_chord(dst, &m->chords.items[0], true);
			dstrcat(dst, ",l");
			append_chord(dst, &m->chords.items[1], false);
			dstrcat(dst, " s");
			append_chord(dst, &m->chords.items[2], false);
			return 1;
		}
	case 4:
		if (!is_s) dstrcat(dst, "s");
		append_chord(dst, &m->chords.items[0], true);
		dstrcat(dst, ",");
		append_chord(dst, &m->chords.items[1], false);
		dstrcat(dst, ",");
		append_chord(dst, &m->chords.items[2], false);
		dstrcat(dst, ",");
		append_chord(dst, &m->chords.items[3], false);
		return 1;
	default: return 0;
	}
}

static int	append_song_body(t_dstr *dst, t_irealpro_song *song)
{
	t_measure			*m;
	char				barline_buf[2];
	t_time_signature	curr_ts = {0};
	int					is_s = 0;
	int					endings_found = 0;
	size_t				curr_measures_pos = 0;

	barline_buf[1] = '\0';
	m = &song->measures.items[0];
	while (m != NULL)
	{
		curr_measures_pos++;
		if (m->ending == ENDING_FIRST) {
			endings_found++;
		} else if (m->ending != ENDING_NONE
					&& endings_found != 0
					&& song->endings_lengths[endings_found - 1] < 4
					&& (curr_measures_pos - 1) % 4 == 0
					&& song->measures.count - curr_measures_pos > 4) {
			for (int i = 0; i < song->endings_lengths[endings_found - 1]; ++i) {
				dstrcat(dst, "    ");
			}
		}
		if (m->barlines[0])
			barline_buf[0] = m->barlines[0];
		else
			barline_buf[0] = '|';
		dstrcat(dst, barline_buf);
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
			// if we are at the end of song and there is a light-heavy, then put a Z
			if (m->next == NULL && m->barlines[1] == ']')
				barline_buf[0] = 'Z'; 
			else
				barline_buf[0] = m->barlines[1];
			dstrcat(dst, barline_buf);
		} else if (m->next != NULL && (m->next->barlines[0] == '{' || m->next->barlines[0] == '[')){
			dstrcat(dst, "|");
		}
		m = m->next;
	}
	return 0;
}

static void append_composer(t_dstr *dst, char *composer_ref)
{
    char *composer = strdup(composer_ref);
	if (composer[0] == '\0') {
		url_strcat(dst, "Composer Unknown");
		return;
	}

	// check if there is more than 1 space.
	// if yes, then just print the composer name as is.
	size_t composer_len = strlen(composer);
	int num_spaces = 0;
	for (size_t i = 0; i < composer_len; i++) {
		if (composer[i] == ' ')
			num_spaces++;
	}
	if (num_spaces > 1) {
		url_strcat(dst, composer);
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
	url_strcat(dst, " "); // url encoded space
	url_strcat(dst, composer); // first name
    free(composer);
}

static void	append_song_title(t_dstr *dst, char *title)
{
	if (title[0] == '\0') {
		url_strcat(dst, "Song Title");
		return;
	}

	char title_to_copy[MAX_CREDENTIALS];

	title_to_copy[0] = '\0';
	if (strncmp(title, "The ", 4) == 0) {
		strcat(title_to_copy, title + 4);
		strcat(title_to_copy, ", The");
	} else {
		strcat(title_to_copy, title);
	}
	url_strcat(dst, title_to_copy);
}

static char *utoa(char *dest, uint16_t src)
{
    if (src >= 10)
        dest = utoa(dest, src / 10); // on descend récursivement
    *dest = (src % 10) + '0';  // écrit le chiffre actuel
    return dest + 1;            // retourne le pointeur pour le prochain chiffre
}

static	int	append_song(t_dstr *dst, t_irealpro_song *song)
{
	t_dstr raw_body;

	dstr_init(&raw_body, 256);
	append_song_title(dst, song->title);
	dstrcat(dst, "=");
	append_composer(dst, song->composer);
	dstrcat(dst, "==");
	const char *style = get_style_str(song->style);
	if (style != NULL)
		url_strcat(dst, style);
	else
		url_strcat(dst, "Even 8ths");
	dstrcat(dst, "=");
	const char *key = get_note_str(song->key);
	if (key != NULL)
		url_strcat(dst, key);
	else
		dstrcat(dst, "C");
	dstrcat(dst, "==1r34LbKcu7");
	append_song_body(&raw_body, song);
	url_scramble(raw_body.buf, raw_body.len);
	url_strcat(dst, raw_body.buf);
	url_strcat(dst, " ");
	dstrcat(dst, "==");
	if (song->tempo != 0) {
		char tempo[16] = {0};
		utoa(tempo, song->tempo);
		dstrcat(dst, tempo);
		dstrcat(dst, "=3");
	} else {
		dstrcat(dst, "0=0");
	}
	free(raw_body.buf);
	return 0;
}

char	*irp_song_get_html(t_irealpro_song *song)
{
	t_dstr	res;

	dstr_init(&res, 512);
	dstrcat(&res, "<a href=\"irealb://");
	if (append_song(&res, song) != 0) {
		free(res.buf);
		return NULL;
	}
	dstrcat(&res, "\">");
	if (*song->title) dstrcat(&res, song->title);
	else dstrcat(&res, "Song Title");
	dstrcat(&res, "</a>");
	return res.buf;
}

char	*irp_playlist_get_html(t_irealpro_playlist *playlist)
{
	t_dstr	res;

	dstr_init(&res, 1024);
	dstrcat(&res, "<a href=\"irealb://");
	for (size_t i = 0; i < playlist->songs.count; i++) {
		if (append_song(&res, playlist->songs.items[i]) != 0) {
			free(res.buf);
			return NULL;
		}
		dstrcat(&res, "===");
	}
	url_strcat(&res, playlist->title.buf);
	dstrcat(&res, "\">");
	dstrcat(&res, playlist->title.buf);
	dstrcat(&res, "</a>");
	return res.buf;
}
