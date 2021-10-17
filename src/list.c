#include "list.h"

#include <stdlib.h>
#include <string.h>

int
list_init(listT *const list, size_t element_size, u8 initial_count, const void *const source)
{
	if (!list)
		return -1;
	memset(list, 0, sizeof(*list));
	if (!element_size)
		return 0;
	if (initial_count) {
		if (!(list->data = calloc(1, element_size * initial_count)))
			return -1;
		if (source)
			memcpy(list->data, source, element_size * initial_count);
		list->element_count = initial_count;
	}
	list->element_size = element_size;
	return 0;
}
void
list_clear(listT *const list)
{
	if (list) {
		if (list->data)
			free(list->data);
		memset(list, 0, sizeof(*list));
	}
}
int
list_add(listT *const list, const void *const element)
{
	if (!list || !element)
		return -1;
	if (!list->data) {
		if (!(list->data = calloc(1, list->element_size)))
			return -1;
	} else {
		void *temp = list->data;
		if (!(list->data = realloc(list->data, list->element_size * (list->element_count + 1)))) {
			list->data = temp;
			return -1;
		}
	}
	if (element)
		memcpy(list->data + list->element_size * list->element_count, element, list->element_size);
	++list->element_count;
	return 0;
}
int
list_remove(listT *const list)
{
	void *temp = NULL;
	if (!list)
		return -1;
	if (!list->element_count)
		return 0;
	temp = list->data;
	if (!(list->data = realloc(list->data, list->element_size * (list->element_count - 1)))) {
		list->data = temp;
		return -1;
	}
	--list->element_count;
	return 0;
}
