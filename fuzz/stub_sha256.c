/*
 * Stubs for the assembly SHA256 entry points that the C shim (sha2.c,
 * compiled with EXTERN_SHA256 on x86_64/ARM/PPC when USE_ASM is on)
 * expects. In the real miner these come from sha2-x64.S / sha2-x86.S.
 * The fuzz harness only exercises RPC/protocol parsing paths, so the
 * correctness of these symbols does not affect fuzzing quality.
 */
#include <stdint.h>

void sha256_transform(uint32_t *state, const uint32_t *block, int swap) {
	(void)state; (void)block; (void)swap;
}

void sha256_use_4way(void) {}
void sha256_use_8way(void) {}

void sha256d_ms(uint32_t *hash, uint32_t *W,
	const uint32_t *midstate, const uint32_t *prehash) {
	(void)hash; (void)W; (void)midstate; (void)prehash;
}

void sha256d_ms_4way(uint32_t *hash, uint32_t *data,
	const uint32_t *midstate, const uint32_t *prehash) {
	(void)hash; (void)data; (void)midstate; (void)prehash;
}

void sha256d_ms_8way(uint32_t *hash, uint32_t *data,
	const uint32_t *midstate, const uint32_t *prehash) {
	(void)hash; (void)data; (void)midstate; (void)prehash;
}
