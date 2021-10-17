#include "levenshtein.h"

#include "matrix.h"
#include "list.h"

static int
i_basic_distance(leven_dstT *const dst,
	const char *const a, const char *const b,
	u8 *la, u8 *lb)
{
	if (!a) {
		if (!b || !(*lb = strlen(b))) {
			memset(dst, 0, sizeof(*dst));
			return 1;
		} else {
			dst->distance = *lb;
			dst->ratio = 0;
			return 1;
		}
	} else if (!b) {
		if (!(*la = strlen(a))) {
			memset(dst, 0, sizeof(*dst));
			return 1;
		} else {
			dst->distance = *la;
			dst->ratio = 0;
			return 1;
		}
	} else if (!(*la = strlen(a))) {
		dst->distance = strlen(b);
		dst->ratio = 1;
		return 1;
	} else if (!(*lb = strlen(b))) {
		dst->distance = *la;
		dst->ratio = 0;
		return 1;
	}
	return 0;
}
int
levenshtein_distance(leven_dstT *const dst, const char *const a, const char *const b)
{
	/* Is there a way to do this without generating the matrix again and again? */
	matrixT matrix;
	u8 la, lb, lt;
	if (!dst) {
		return -1;
	} else if (i_basic_distance(dst, a, b, &la, &lb)) {
		return 0;
	} else if (matrix_init(&matrix, la + 1, lb + 1, sizeof(u8))) {
		return -1;
	}
	lt = la + lb;
	for (u8 i = 0; i <= la; ++i)
		matrix_set(&matrix, i, 0, &i);
	for (u8 j = 0; j <= lb; ++j)
		matrix_set(&matrix, 0, j, &j);
	for (u8 i = 1; i <= la; ++i) {
		for (u8 j = 1; j <= lb; ++j) {
			if (a[i - 1] == b[j - 1]) {
				matrix_copy_element(&matrix, i - 1, j - 1, i, j);
			} else {
				u8 S,I,D;
				matrix_get(&matrix, i - 1, j - 1, &S);
				matrix_get(&matrix, i    , j - 1, &I);
				matrix_get(&matrix, i - 1, j    , &D);
				++S,++I,++D;
				u8 M = S;
				if (I < M) M = I;
				if (D < M) M = D;
				matrix_set(&matrix, i, j, &M);
			}
		}
	}
	matrix_get(&matrix, la, lb, &dst->distance);
	matrix_free(&matrix);
	dst->ratio = (long double)(lt - dst->distance) / lt;
	return 0;
}
int
osa_distance(leven_dstT *dst, const char *const a, const char *const b)
{
	matrixT matrix;
	u8 la, lb, lt;
	if (!dst) {
		return -1;
	} else if (i_basic_distance(dst, a, b, &la, &lb)) {
		return 0;
	} else if (matrix_init(&matrix, la + 1, lb + 1, sizeof(u8))) {
		return -1;
	}
	lt = la + lb;
	for (u8 i = 0; i <= la; ++i)
		matrix_set(&matrix, i, 0, &i);
	for (u8 j = 0; j <= lb; ++j)
		matrix_set(&matrix, 0, j, &j);
	for (u8 i = 1; i <= la; ++i) {
		for (u8 j = 1; j <= lb; ++j) {
			if (a[i - 1] == b[j - 1]) {
				matrix_copy_element(&matrix, i - 1, j - 1, i, j);
			} else {
				u8 S,I,D;
				matrix_get(&matrix, i - 1, j - 1, &S);
				matrix_get(&matrix, i    , j - 1, &I);
				matrix_get(&matrix, i - 1, j    , &D);
				++S,++I,++D;
				u8 M = S;
				if (I < M) M = I;
				if (D < M) M = D;
				if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]){
					u8 T = *(u8 *)matrix_element(&matrix, i - 2, j - 2) + 1;
					if (T < M) M = T;
				}
				matrix_set(&matrix, i, j, &M);
			}
		}
	}
	matrix_get(&matrix, la, lb, &dst->distance);
	matrix_free(&matrix);
	dst->ratio = (long double)(lt - dst->distance) / lt;
	return 0;
}
int
damerau_distance(leven_dstT *dst, const char *const a, const char *const b)
{
	u8 list[1024] = {0};
	matrixT matrix;
	u8 la, lb, lt;
	if (!dst) {
		return -1;
	} else if (i_basic_distance(dst, a, b, &la, &lb)) {
		return 0;
	} else if (matrix_init(&matrix, la + 2, lb + 2, sizeof(u8))) {
		return -1;
	}
	lt = la + lb;
	matrix_set(&matrix, 0, 0, &lt);
	for (u8 i = 0; i <= la; ++i) {
		matrix_set(&matrix, i + 1, 0, &lt);
		matrix_set(&matrix, i + 1, 1, &i);
	}
	for (u8 j = 0; j <= lb; ++j) {
		matrix_set(&matrix, 0, j + 1, &lt);
		matrix_set(&matrix, 1, j + 1, &j);
	}
	for (u8 i = 2; i <= la + 1; ++i) {
		u8 d = 0;
		for (u8 j = 2; j <= lb + 1; ++j) {
			u8 k = list[b[j - 2]], l = d;
#if 0 /* This is the technically correct implementation on wikipedia */
			int c = 1;
			if (a[i - 2] == b[j - 2]) {
				d = j - 1;
				c = 0;
			}
			u8 S, I, D, T, M;
			matrix_get(&matrix, i - 1, j - 1, &S);
			matrix_get(&matrix, i,     j - 1, &I);
			matrix_get(&matrix, i - 1, j    , &D);
			matrix_get(&matrix, k - 1, l - 1, &T);
			S+=c,++I,++D;
			T += (i - k - 1) + (j - l - 1) + 1;
			M = S;
			if (I < M) M = I;
			if (D < M) M = D;
			if (T < M) M = T;
			matrix_set(&matrix, i, j, &M);
#else
			if (a[i - 2] == b[j - 2]) {
				d = j - 1;
				matrix_copy_element(&matrix, i - 1, j - 1, i, j);
			} else {
				u8 S, I, D, T, M;
				matrix_get(&matrix, i - 1, j - 1, &S);
				matrix_get(&matrix, i,     j - 1, &I);
				matrix_get(&matrix, i - 1, j    , &D);
				matrix_get(&matrix, k - 1, l - 1, &T);
				++S,++I,++D;
				T += (i - k - 1) + (j - l - 1) + 1;
				M = S;
				if (I < M) M = I;
				if (D < M) M = D;
				if (T < M) M = T;
				matrix_set(&matrix, i, j, &M);
			}
#endif
		}
		list[a[i]] = i;
	}
	matrix_get(&matrix, la, lb, &dst->distance);
	matrix_free(&matrix);
	dst->ratio = (long double)(lt - dst->distance) / lt;
	return 0;
}
