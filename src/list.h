#ifndef LIST_H
#define LIST_H

#include "types.h"

typedef struct list {
	void *data;
	size_t element_size;
	u8 element_count;
} listT;

int list_init(listT *const list, size_t element_size,
	u8 initial_count, const void *const source);
void list_clear(listT *const list);
int list_add(listT *const list, const void *const element);
int list_remove(listT *const list);

#define CHK_INDEX(_l_, _i_) ((_i_) >= (_l_)->element_count)
#define CHK_LIST(_l_, _i_) (!(_l_) || CHK_INDEX(_l_, _i_))
#define GET_INDEX(_l_, _i_) ((char *)(_l_)->data + (_i_) * (_l_)->element_size)
static inline void *
list_element(const listT *const list, u8 idx)
{
	if (CHK_LIST(list, idx))
		return NULL;
	return GET_INDEX(list, idx);
}
#undef CHK_INDEX
#undef CHK_LIST
#undef GET_INDEX

#endif /* LIST_H */
