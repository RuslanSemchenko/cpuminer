noinst_LIBRARIES	= libjansson.a

libjansson_a_SOURCES	= \
			  jansson_private_config.h	\
			  jansson_config.h		\
			  error.c			\
			  dump.c			\
			  hashtable.c			\
			  hashtable.h			\
			  hashtable_seed.c		\
			  jansson.h			\
			  jansson_private.h		\
			  load.c			\
			  memory.c			\
			  pack_unpack.c			\
			  strconv.c			\
			  strbuffer.c			\
			  strbuffer.h			\
			  utf.c				\
			  utf.h				\
			  value.c			\
			  version.c

# JSON_INTEGER_IS_LONG_LONG and JSON_INLINE are provided by
# jansson_config.h, so JANSSON_USING_CMAKE is intentionally NOT defined:
# the public type block (json_int_t) must stay enabled.
libjansson_a_CPPFLAGS	= -include jansson_private_config.h
