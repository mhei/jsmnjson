#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <jsmn.h>
#include "jsmnjson.h"

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

#ifndef JSMNJSON_MAX_DEPTH
#define JSMNJSON_MAX_DEPTH 8
#endif

static int __dbg(int level, const char *format, ...)
{
	va_list ap;
	int rv1, rv2;

	rv1 = printf("%*s[%d] ", 4 * (JSMNJSON_MAX_DEPTH - level), " ", level);
	if (rv1 < 0)
		return rv1;

	va_start(ap, format);
	rv2 = vprintf(format, ap);
	va_end(ap);

	if (rv2 < 0)
		return rv2;

	return rv1 + rv2;
}

static int __jsmnjson_get_size(jsmntok_t *t, int level)
{
	int i, size = 1;
	jsmntok_t *p = t + 1;

	if (level == 0)
		return -1;

	if (t->size == 0)
		return 1;

	for (i = 0; i < t->size; i++) {
		int subsize = __jsmnjson_get_size(p, level - 1);
		if (subsize == -1)
			return -1;

		p += subsize;
		size += subsize;
	}

	return size;
}

int jsmnjson_get_size(jsmntok_t *t)
{
	return __jsmnjson_get_size(t, JSMNJSON_MAX_DEPTH);
}

long int jsmnjson_get_int(jsmntok_t *t, const char *json)
{
	if (t->type != JSMN_PRIMITIVE) {
		errno = EINVAL;
		return 0;
	}

	return strtol(&json[t->start], NULL, 10);
}

int jsmnjson_get_boolean(jsmntok_t *t, const char *json)
{
	int value;

	if (t->type != JSMN_PRIMITIVE) {
		errno = EINVAL;
		return 0;
	}

	if (json[t->start] == 't')
		return 1;
	if (json[t->start] == 'f')
		return 0;

	value = atoi(&json[t->start]);
	return !!value;
}

int jsmnjson_strncopy(char *dest, size_t n, jsmntok_t *t, const char *json)
{
	int c;

	if (t->type != JSMN_STRING) {
		errno = EINVAL;
		return -1;
	}

	c = min(n - 1, t->end - t->start);
	strncpy(dest, &json[t->start], c);
	dest[c] = '\0';

	return strlen(dest);
}

int jsmnjson_strcmp(jsmntok_t *t, const char *json, const char *s2)
{
	if (t->type != JSMN_STRING) {
		errno = EINVAL;
		return -1;
	}

	if (strlen(s2) == (t->end - t->start))
		return strncmp(&json[t->start], s2, t->end - t->start);

	return -1;
}

jsmntok_t *__jsmnjson_get_by_key(jsmntok_t *t, const char *json, const char *key)
{
	int i, size;
	jsmntok_t *p = t + 1;


	if ((t->type != JSMN_OBJECT) || (t->size == 0)) {
		errno = EINVAL;
		return NULL;
	}

	for (i = 0; i < t->size; i++) {
		if (jsmnjson_strcmp(p, json, key) == 0)
			return p + 1;

		size = __jsmnjson_get_size(p, JSMNJSON_MAX_DEPTH);
		if (size == -1)
			return NULL;

		p += size;
	}

	return NULL;
}

jsmntok_t *jsmnjson_get_by_key(jsmntok_t *t, const char *json, const char *key)
{
	char buf[128];
	int level = JSMNJSON_MAX_DEPTH;
	jsmntok_t *p = t;
	char *s = buf;
	char *d;

	if (strlen(key) >= ARRAY_SIZE(buf))
		return NULL;

	strcpy(buf, key);

	while (level-- && (d = strchr(s, '/'))) {
		*d = '\0';
		p = __jsmnjson_get_by_key(p, json, s);
		if (p == NULL)
			return NULL;
		s = d + 1;
	}

	return __jsmnjson_get_by_key(p, json, s);
}
