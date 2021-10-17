#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include "types.h"

typedef struct leven_dst {
	u8 distance;
	long double ratio;
} leven_dstT;
typedef struct leven_pair {
	leven_dstT distance;
	const char *a, *b;
	long double threshold;
} leven_pairT;

int levenshtein_distance(leven_dstT *dst, const char *const a, const char *const b);
int osa_distance(leven_dstT *dst, const char *const a, const char *const b);
int damerau_distance(leven_dstT *dst, const char *const a, const char *const b);

#endif /* LEVENSHTEIN_H */
