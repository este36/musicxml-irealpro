#include "da.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

t_dstr	dstr_from_cstr(const char *str)
{
	t_dstr s = {0};

	if (!str)
		return (t_dstr){0};

	size_t len = strlen(str);
	if (dstr_resize(&s, len) != 0)
		return (t_dstr){0};
	s.len = len;
	memcpy(s.buf, str, s.len);
	s.buf[s.len] = '\0';
	return s;
}

t_dstr	dstr_from_str_ref(const char *str, size_t len)
{
	t_dstr s = {0};

	if (!str)
		return (t_dstr){0};

	if (dstr_resize(&s, len) != 0)
		return (t_dstr){0};
	s.len = len;
	memcpy(s.buf, str, s.len);
	s.buf[s.len] = '\0';
	return s;
}

int		dstr_init(t_dstr *dst, size_t capacity)
{
	dst->buf = (char *)malloc(sizeof(char) * capacity);
	if (dst->buf == NULL)
		return -1;
	dst->cap = capacity;
	dst->len = 0;
	return 0;
}

int	dstrcat(t_dstr *dst, const char *src)
{
	size_t	src_len = strlen(src);
	size_t needed = dst->len + src_len;

	if (needed + 1 > dst->cap)
	{
		if (dstr_resize(dst, needed) != 0)
			return (-1);
	}

	memcpy(dst->buf + dst->len, src, src_len);
	dst->len += src_len;
	dst->buf[dst->len] = '\0';
	return 0;
}

int	dstrncat(t_dstr *dst, const char *src, size_t src_len)
{
	size_t needed = dst->len + src_len;

	if (needed + 1 > dst->cap)
	{
		if (dstr_resize(dst, needed) != 0)
			return (-1);
	}

	memcpy(dst->buf + dst->len, src, src_len);
	dst->len += src_len;
	dst->buf[dst->len] = '\0';
	return 0;
}


int dstr_resize(t_dstr *s, size_t new_len)
{
	if (new_len <= s->len)
	{
		s->len = new_len;
		s->buf[s->len] = '\0';
		return (0);
	}

	new_len += 1;
	size_t new_cap = s->cap;
	if (!new_cap)
		new_cap = DSTR_MIN_CAPACITY;
	while (new_cap < new_len)
		new_cap *= 2;

	char *tmp = (char *)realloc(s->buf, new_cap);
	if (!tmp)
		return (-1);

	s->buf = tmp;
	s->cap = new_cap;
	return (0);
}

int dstr_appendf(t_dstr *dest, char *fmt, ...)
{
	va_list args;
	va_list args_copy;
	int		len;

	va_start(args, fmt);
	va_copy(args_copy, args);
	len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (len < 0)
	{
		va_end(args_copy);
		return (-1);
	}

	size_t new_len = dest->len + len;

	if (new_len + 1 > dest->cap)
	{
		if (dstr_resize(dest, new_len) != 0)
		{
			va_end(args_copy);
			return (-1);
		}
	}

	len = vsnprintf(dest->buf + dest->len, dest->cap - dest->len, fmt, args_copy);
	va_end(args_copy);
	if (len < 0)
		return (-1);
	dest->len = new_len;
	return (0);
}

void dstr_free(t_dstr *s) 
{
	free(s->buf);
	s->buf = NULL;
	s->len = s->cap = 0;
}
