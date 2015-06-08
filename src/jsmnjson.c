#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <jsmn.h>
#include "jsmnjson.h"

long int jsmnjson_get_int(jsmntok_t *t, const char *json)
{
	if (t->type != JSMN_PRIMITIVE) {
		errno = EINVAL;
		return 0;
	}

	return strtol(&json[t->start], NULL, 10);
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
