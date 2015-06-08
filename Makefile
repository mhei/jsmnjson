all: jsmn/libjsmn.a src/libjsmnjson.a tests/test

jsmn/libjsmn.a:
	$(MAKE) -C jsmn

src/libjsmnjson.a: src/jsmnjson.o
	$(AR) rc $@ $^

CPPFLAGS = -I. -I./jsmn -I./src

tests/test: tests/main.o jsmn/libjsmn.a src/libjsmnjson.a
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f jsmn/*.o src/*.o tests/*.o
	rm -f jsmn/*.a src/*.a
	rm -f tests/test

.PHONY: all clean
