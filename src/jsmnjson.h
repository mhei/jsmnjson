#ifndef JSMNJSON_H
#define JSMNJSON_H

#include <jsmn.h>

#ifdef __cplusplus
extern "C" {
#endif

long int jsmnjson_get_int(jsmntok_t *t, const char *json);
int jsmnjson_get_boolean(jsmntok_t *t, const char *json);
int jsmnjson_strncopy(char *dest, size_t n, jsmntok_t *t, const char *json);
int jsmnjson_strcmp(jsmntok_t *t, const char *json, const char *s2);
int jsmnjson_get_size(jsmntok_t *t);
jsmntok_t *jsmnjson_get_by_key(jsmntok_t *t, const char *json, const char *key);

#ifdef __cplusplus
}
#endif

#endif /* JSMNJSON_H */
