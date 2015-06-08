#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jsmn.h>
#include <jsmnjson.h>

char *jsmntype_to_str[] = {
	"JSMN_PRIMITIVE",
	"JSMN_OBJECT",
	"JSMN_ARRAY",
	"JSMN_STRING"
};

int main(int argc, char *argv[])
{
	int i;
	int c;
	jsmn_parser p;
	jsmntok_t t[128];
	char *json = "{\"key\": \"valueABCDWEFGHJKI\", \"key2\": 10}";

	jsmn_init(&p);
	c = jsmn_parse(&p, json, strlen(json), t, ARRAY_SIZE(t));
	if (c < 0) {
		fprintf(stderr, "Failed to parse JSON: %d\n", c);
		return 1;
	}

	/* assume that top-level element is an object */
	if (c < 1 || t[0].type != JSMN_OBJECT) {
		fprintf(stderr, "Expected an object at top-level\n");
		return 1;
	}

	for (i = 0; i < c; i++) {
		printf("[%d].type = %s\n", i, jsmntype_to_str[t[i].type]);
		printf("[%d].size = %d\n", i, t[i].size);
		printf("\n");
	}
	
	printf("Value of 4: %ld\n", jsmnjson_get_int(&t[4], json));
	
	char buffer2[] = "valueVALUE";
	c = jsmnjson_strncopy(buffer2, sizeof(buffer2), &t[2], json);
	printf("Copied %d bytes: %s\n", c, buffer2);

	printf("Comparing result 1: %d\n", jsmnjson_strcmp(&t[2], json, "valueABCDWEFGHJKI"));
	printf("Comparing result 2: %d\n", jsmnjson_strcmp(&t[2], json, "valueABCDWEFGHJK"));
	printf("Comparing result 3: %d\n", jsmnjson_strcmp(&t[2], json, "xalueABCDWEFGHJKI"));
	printf("Comparing result 4: %d\n", jsmnjson_strcmp(&t[2], json, "valueABCDWEFGHJKIA"));

	return 0;
}
