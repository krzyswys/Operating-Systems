CC = gcc -Wall -Wextra -Wpedantic
all: static shared

static: libblockarray.a
shared: libblockarray.so

lib_static.o: lib.c
	$(CC)  -c -o $@ $^

libblockarray.a: lib_static.o
	ar rcs $@ $^

lib_shared.o: lib.c
	$(CC) -c -fPIC -o $@ $^

libblockarray.so: lib_shared.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.a *.so *.o
