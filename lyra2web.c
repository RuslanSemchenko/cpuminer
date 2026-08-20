/*
 * Lyra2-webchain (MintMe) scanhash wrapper for cpuminer.
 *
 * The Lyra2 core (Lyra2.c / Sponge.c) is the Lyra PHC reference
 * implementation, placed in the public domain by the Lyra PHC team.
 * The parameter set (timeCost=4, NROWS=16384, NCOLS=4, 6 MB RAM per
 * thread) is taken from the mintme-com/miner project (XMRig-based
 * Webchain CPU miner) and matches what the mintme-com/pool proxy
 * uses to verify shares (CalcHashLYRA2 with timeCost=4).
 *
 * Memory: 12 uint64_t x 4 columns x 8 bytes x 16384 rows = 6 MB per
 * thread (supplied via scratchbuf, allocated by the miner thread).
 *
 * Interface (see miner.h):
 *   pdata[17..18]  - 64-bit little-endian nonce, incremented each hash
 *   blob           - full Webchain job blob (header + transactions)
 *   blob_size      - blob length in bytes (>= 8; minimum 76)
 *   pdata[32..63]  - on success, the 32-byte winning hash is written here
 *
 * Webchain PoW (compatible with XMRig / mintme-com/miner):
 *   - 64-bit nonce placed at blob[size-8 .. size-1] before hashing
 *   - hash = Lyra2(ctx, kLen=32, pwd=blob, pwdlen=blob_size, timeCost=1)
 *
 * Target format: Webchain compares the hash read as a big-endian uint64
 * (big-endian interpretation of the little-endian output bytes) against
 * the job target:
 *     bswap64(le64dec(hash)) <= target
 * The pool encodes the target as 8 little-endian bytes, which is exactly
 * cpuminer's LE layout in work->target[0..1], so no conversion is needed.
 *
 * Note: cpuminer's generic fulltest() performs a little-endian word-wise
 * comparison, which is NOT what Webchain expects, so this module uses
 * its own fulltest equivalent (lyra2web_fulltest).
 */

#include "miner.h"
#include "Lyra2.h"

#include <string.h>

static __thread void *g_lyra2_ctx;

/* Big-endian comparison of the Lyra2 hash against the LE-layout target. */
static bool lyra2web_fulltest(const uint8_t *hash, const uint32_t *ptarget)
{
	uint64_t hb, tb;

	hb = bswap64(le64dec(hash));
	tb = ((uint64_t)ptarget[1] << 32) | ptarget[0];

	return hb <= tb;
}

int scanhash_lyra2web(int thr_id, uint32_t *pdata,
	const unsigned char *blob, size_t blob_size,
	unsigned char *scratchbuf, const uint32_t *ptarget,
	uint32_t max_nonce, unsigned long *hashes_done)
{
	uint8_t hash[32];
	uint8_t blob_n[2048];
	uint64_t nonce;
	uint32_t n, hi, first;

	(void)scratchbuf;

	if (!g_lyra2_ctx)
		g_lyra2_ctx = LYRA2_create();

	if (blob_size < 8 || blob_size > sizeof(blob_n))
		return 0;

	n = pdata[17] - 1;
	hi = pdata[18];
	first = pdata[17];
	nonce = ((uint64_t)hi << 32) | n;

	/* keep a per-thread copy of the blob so we only patch the nonce */
	memcpy(blob_n, blob, blob_size);

	do {
		nonce++;
		pdata[17] = (uint32_t)nonce;
		pdata[18] = (uint32_t)(nonce >> 32);
		memcpy(blob_n + blob_size - 8, &nonce, 8);
		/* Webchain PoW: LYRA2 with timeCost=4 (XMRig VARIANT_0).
		 * The pool verifies shares with the same parameter set, see
		 * mintme-com/pool proxy/miner.go CalcHashLYRA2(..., 4). */
		LYRA2(g_lyra2_ctx, hash, 32, blob_n, blob_size, 4);
		if (lyra2web_fulltest(hash, ptarget)) {
			/* expose the winning hash to the miner thread */
			memcpy((uint8_t *)pdata + 32, hash, 32);
			*hashes_done = (uint32_t)nonce - first + 1;
			return 1;
		}
	} while ((uint32_t)nonce < max_nonce && !work_restart[thr_id].restart);

	*hashes_done = (uint32_t)nonce - first + 1;
	pdata[17] = (uint32_t)nonce;
	pdata[18] = hi;
	return 0;
}

/* Self-test against the Webchain reference vector (XMRig VARIANT_0,
 * i.e. the lyra2v2 hash used by current Webchain pools). */
bool lyra2web_test(void)
{
	extern const uint8_t lyra2web_test_input[76];
	extern const uint8_t lyra2web_test_output[32];
	uint8_t hash[32];
	void *ctx = LYRA2_create();

	if (!ctx)
		return false;
	LYRA2(ctx, hash, 32, lyra2web_test_input, 76, 4);
	LYRA2_destroy(ctx);
	return !memcmp(hash, lyra2web_test_output, 32);
}

/* 6 MB scratch buffer: 12 x 4 x 8 x 16384 */
unsigned char *lyra2web_buffer_alloc(void)
{
	return malloc(LYRA2_MEMSIZE);
}
