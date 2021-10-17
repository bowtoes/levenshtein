#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"

#include <string.h>

typedef struct matrix {
	void *array;
	u8 width, height;
	size_t element_size;
	u8 area;
} matrixT;

int matrix_init(matrixT *const matrix, u8 width, u8 height, size_t element_size);
int matrix_resize(matrixT *const matrix, u8 new_width, u8 new_height);
void matrix_free(matrixT *const matrix);

#define CHK_INDEX(_m_, _x_, _y_) ((_x_) >= (_m_)->width || (_y_) >= (_m_)->height)
#define CHK_MATRIX(_m_, _x_, _y_) (!(_m_) || CHK_INDEX(_m_, _x_, _y_))
#define GET_INDEX(_m_, _x_, _y_) ((char *)(_m_)->array + ((_x_) + (_y_) * (_m_)->width) * (_m_)->element_size)
static inline void *
matrix_element(const matrixT *const matrix, u8 column, u8 row)
{
	if (CHK_MATRIX(matrix, column, row))
		return NULL;
	else if (matrix->array)
		return GET_INDEX(matrix, column, row);
	else
		return NULL;
}
static inline int
matrix_get(const matrixT *const matrix, u8 column, u8 row, void *const dst)
{
	if (CHK_MATRIX(matrix, column, row))
		return -1;
	else if (!dst)
		return 0;
	memcpy(dst, GET_INDEX(matrix, column, row), matrix->element_size);
	return 0;
}
static inline int
matrix_set(matrixT *const matrix, u8 column, u8 row, const void *const src)
{
	if (CHK_MATRIX(matrix, column, row))
		return -1;
	else if (!src)
		return 0;
	memcpy(GET_INDEX(matrix, column, row), src, matrix->element_size);
	return 0;
}
static inline int
matrix_copy_element(matrixT *const matrix, u8 column_a, u8 row_a, u8 column_b, u8 row_b)
{
	if (CHK_MATRIX(matrix, column_a, row_a) || CHK_INDEX(matrix, column_b, row_b))
		return -1;
	if (column_a != column_b || row_a != row_b)
		memmove(GET_INDEX(matrix, column_b, row_b), GET_INDEX(matrix, column_a, row_a), matrix->element_size);
	return 0;
}
#undef CHK_INDEX
#undef CHK_MATRIX
#undef GET_INDEX

#endif /* MATRIX_H */
