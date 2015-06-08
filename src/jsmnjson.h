#ifndef JSMNJSON_H
#define JSMNJSON_H

#include <jsmn.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	   _a < _b ? _a : _b; })

#define max(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	   _a > _b ? _a : _b; })

long int jsmnjson_get_int(jsmntok_t *t, const char *json);
int jsmnjson_strncopy(char *dest, size_t n, jsmntok_t *t, const char *json);
int jsmnjson_strcmp(jsmntok_t *t, const char *json, const char *s2);

#ifdef __cplusplus
}
#endif

#endif /* JSMNJSON_H */
