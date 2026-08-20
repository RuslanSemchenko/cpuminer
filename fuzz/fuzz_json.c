/*
 * Fuzzing harness for the in-tree jansson JSON parser (json_loads).
 * Build with: clang -g -O1 -fsanitize=fuzzer,address,undefined fuzz/fuzz_json.c \
 *             compat/jansson/dump.c compat/jansson/hashtable.c compat/jansson/load.c \
 *             compat/jansson/strbuffer.c compat/jansson/utf.c compat/jansson/value.c \
 *             -I. -Icompat/jansson -o fuzz_json
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "jansson.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	/* json_loads requires a NUL-terminated string */
	char *input = malloc(size + 1);
	if (!input)
		return 0;
	memcpy(input, data, size);
	input[size] = '\0';

	json_error_t error;
	json_t *root = json_loads(input, 0, &error);
	if (root) {
		/* Exercise serialization/dumping on whatever parsed */
		char *dump = json_dumps(root, JSON_COMPACT);
		free(dump);
		json_decref(root);
	}
	free(input);
	return 0;
}
