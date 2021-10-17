#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "matrix.h"
#include "list.h"
#include "levenshtein.h"

typedef struct state {
	u8 max_input_len;
	long double threshold;
	unsigned char print_more:1;
	unsigned char print0:1;
	unsigned char distance_formula:2;
} stateT;
static int
gen_pairs(stateT *const state, const listT *const inputs, listT *const pairs)
{
	for (int i = 0; i < inputs->element_count; ++i) {
		for (int j = i + 1; j < inputs->element_count; ++j) {
			char *a = *(char **)list_element(inputs, i), *b = *(char **)list_element(inputs, j);
			matrixT state_matrix = {0};
			leven_dstT d = {0};
			leven_pairT p = {0};
			if (state->distance_formula == 0) {
				if (levenshtein_distance(&d, a, b))
					return 1;
			} else if (state->distance_formula == 1) {
				if (damerau_distance(&d, a, b))
					return 1;
			} else if (state->distance_formula == 2) {
				if (osa_distance(&d, a, b))
					return 1;
			}
			if (d.ratio < state->threshold)
				continue;
			p.a = a, p.b = b, p.distance = d, p.threshold = state->threshold;
			if (list_add(pairs, &p))
				return -1;
		}
	}
	return 0;
}
static int
parse_args(stateT *const state, listT *const inputs, int argc, char **argv)
{
	for (int i = 0; i < argc; ++i){
		if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-threshold") == 0) {
			long double a;
			if (i + 1 >= argc)
				return -1;
			a = strtod(argv[i + 1], NULL);
			if (a > 1) a = 1;
			if (a < 0) a = 0;
			state->threshold = a;
			++i;
		} else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			state->print_more = !state->print_more;
		} else if (strcmp(argv[i], "-0") == 0 || strcmp(argv[i], "--null-sep") == 0) {
			state->print0 = !state->print0;
			if (state->print0)
				state->print_more = 0;
		} else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--levenshtein") == 0) {
			state->distance_formula = 0;
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--damerau") == 0) {
			state->distance_formula = 1;
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--osa") == 0) {
			state->distance_formula = 2;
		} else {
			u8 s = 0;
			if (list_add(inputs, &argv[i]))
				return -1;
			s = strlen(argv[i]);
			if (s > state->max_input_len)
				state->max_input_len = s;
		}
	}
	return 0;
}
int
main(int argc, char **argv)
{
	stateT state = {0};
	listT pairs = {0};
	listT inputs = {0};
	list_init(&pairs, sizeof(leven_pairT), 0, NULL);
	list_init(&inputs, sizeof(*argv), 0, NULL);
	if (parse_args(&state, &inputs, argc - 1, argv + 1))
		return 2;
	if (gen_pairs(&state, &inputs, &pairs)) {
		list_clear(&pairs);
		list_clear(&inputs);
		return 1;
	}
	for (u8 i = 0; i < pairs.element_count; ++i) {
		leven_pairT *pair = list_element(&pairs, i);
		if (!state.print_more) {
			char c = state.print0?0:'\n';
			printf("%s%c%s%c%.*Lg\n", pair->a, c, pair->b, c, LDBL_DIG, pair->distance.ratio);
		} else {
			printf("%*s ~ %*s : %.*Lg	%llu	Threshold %.*Lg\n",
			    state.max_input_len, pair->a, state.max_input_len, pair->b,
				LDBL_DIG, pair->distance.ratio, pair->distance.distance,
				LDBL_DIG, pair->threshold);
		}
	}
	list_clear(&pairs);
	list_clear(&inputs);
}
