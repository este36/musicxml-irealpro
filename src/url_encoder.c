#include "irealpro.h"

void url_strcat(da_str *dst, const char *src)
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
			buf[1] = hex_base[(unsigned char)src[i] >> 4];
			buf[2] = hex_base[(unsigned char)src[i] & 0x0F];
		}
		da_strcat(dst, buf);
		i++;
	}
}

// lengths of before and after should be equal to each other
static void	str_replace(char *buf, const char *before, const char *after)
{
	int		i;
	char	*found;

	found = buf;
	while (1)
	{
		found = strstr(found, before);
		if (found == NULL)
			break;
		i = -1;
		while (before[++i]) 
			found[i] = after[i];
		found += i;
	}
}

void	url_scramble(char *body, size_t len)
{
	size_t	pos;
	char	tmp;

	pos = 0;
	tmp = 0;
	str_replace(body, "   ", "XyQ");
	str_replace(body, " |", "LZ");
	str_replace(body, "| x", "Kcl");

	while (len - pos > 51)
	{
		for (int i = 0; i < 5; i++) {
			tmp = body[pos + 49 - i];
			body[pos + 49 - i] = body[pos + i];
			body[pos + i] = tmp;
		}

		for (int i = 10; i < 24; i++) {
			tmp = body[pos + 49 - i];
			body[pos + 49 - i] = body[pos + i];
			body[pos + i] = tmp;
		}
		pos += 50;
	}
}
