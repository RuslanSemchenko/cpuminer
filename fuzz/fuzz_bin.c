/*
 * Fuzzing harness for cpuminer binary/hex conversion helpers used by the
 * stratum/JSON-RPC protocol layer (util.c: hex2bin, hex2bin2, bin2hex).
 * Build with: clang -g -O1 -fsanitize=fuzzer,address,undefined fuzz/fuzz_bin.c \
 *             util.c -I. -Icompat/jansson -lcurl -lpthread -o fuzz_bin
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "miner.h"

extern bool hex2bin(unsigned char *bin, const char *hex, size_t len);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	/* hex2bin uses strlen on its input, so terminate the fuzz data */
	char *hex = malloc(size + 1);
	if (!hex)
		return 0;
	memcpy(hex, data, size);
	hex[size] = '\0';

	/* hex2bin only consumes hexlen/2 bytes, but must tolerate any input */
	unsigned char *bin = malloc((size + 1) / 2);
	if (bin) {
		hex2bin(bin, hex, size / 2);
		free(bin);
	}
	free(hex);
	return 0;
}
