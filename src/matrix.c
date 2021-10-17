#include "matrix.h"

#include <stdlib.h>

int
matrix_init(matrixT *const matrix, u8 width, u8 height, size_t element_size)
{
	matrixT new = {0};
	if (!matrix)
		return -1;
	if (!width || !height)
		return 0;
	new.area = width * height;
	if (!(new.array = calloc(1, new.area * element_size)))
		return -1;
	new.width = width;
	new.height = height;
	new.element_size = element_size;
	*matrix = new;
	return 0;
}
int
matrix_resize(matrixT *const matrix, u8 new_width, u8 new_height)
{
	u8 new_area;
	if (!matrix) {
		return -1;
	} else if ((new_area = new_width * new_height) != matrix->area) {
		if (!new_width || !new_height) {
			free(matrix->array);
			matrix->array = NULL;
		} else {
			void *temp = matrix->array;
			if (!(matrix->array = realloc(matrix->array, new_area * matrix->element_size))) {
				matrix->array = temp;
				return -1;
			}
			matrix->width = new_width;
			matrix->height = new_height;
			matrix->area = new_area;
		}
	}
	return 0;
}
void
matrix_free(matrixT *const matrix)
{
	if (matrix) {
		if (matrix->array)
			free(matrix->array);
		memset(matrix, 0, sizeof(*matrix));
	}
}
