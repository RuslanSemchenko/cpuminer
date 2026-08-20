/*
 * Minimal stubs so that util.c can be linked into fuzz harnesses without
 * pulling in the whole miner (cpu-miner.c defines these globals).
 * Only the subset actually reachable from json_rpc_call() / hex2bin()
 * is provided; anything else would indicate a harness bug.
 */
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool opt_debug = false;
bool opt_protocol = false;
bool opt_redirect = true;
int opt_timeout = 300;
bool want_longpoll = true;
bool have_longpoll = false;
bool have_gbt = true;
bool allow_getwork = true;
bool want_stratum = true;
bool have_stratum = false;
char *opt_cert;
char *opt_proxy;
long opt_proxy_type = 1; /* CURLPROXY_HTTP */
bool use_syslog = false;
pthread_mutex_t applog_lock = PTHREAD_MUTEX_INITIALIZER;

struct thr_info *thr_info;
int longpoll_thr_id = -1;
int stratum_thr_id = -1;

struct work_restart {
	volatile uint8_t restart;
	char padding[127];
};
struct work_restart *work_restart;


