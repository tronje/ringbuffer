.PHONY: default clang lib test clean cleanup

### definitions ###
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
OBJS = ringbuffer.o test.o
EXECUTABLE = test.x
LIBNAME = ringbuffer.so

### targets ###
# default target
default: lib

# clang target
clang: CC = clang
clang: default

# compile into shared object
lib: ringbuffer.c ringbuffer.h
	${CC} -shared -o ${LIBNAME} -fPIC ringbuffer.c

# test target
test: test.x
	./test.x

# test executable
test.x: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${EXECUTABLE}

# object files
ringbuffer.o: ringbuffer.c ringbuffer.h
	${CC} ${CFLAGS} -c $<

test.o: test.c ringbuffer.h
	${CC} ${CLFAGS} -c $<

### cleanup targets ###
clean:
	rm -f *.o *.x

cleanup: clean
	rm -f *.so
