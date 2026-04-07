#include "da.h"
#include "string.h"
#include "stdlib.h"

int	darr_init(t_darr *a, size_t cap, size_t el_size)
{
	bzero(a, sizeof(t_darr));
	a->_el_size = el_size;
	a->cap = cap;
	a->data = calloc((cap + 1), el_size);
	if (a->data == NULL)
		return (-1);
	return (0);
}

int	darr_push(t_darr *a, void *el)
{
	if (a->len + 1 > a->cap)
	{
		if (a->cap == 0)
		{
			a->cap = DARR_MIN_CAPACITY;
			a->data = malloc(a->cap * a->_el_size);
		}
		else
		{
			a->cap *= 2;
			a->data = realloc(a->data, a->cap * a->_el_size);
		}
		if (a->data == NULL)
			return (-1);
	}
	memcpy((uint8_t *)a->data + a->len * a->_el_size, el, a->_el_size);
	a->len++;
	return (0);
}
