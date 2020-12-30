CC=g++ -g -std=c++14 -Wall -I ../

# List of source files for your example
lfu_src=../libcache/lfu.cpp
# lru_src=../libcache/lru.cpp
# EXAMPLE_SOURCES=${lfu_src} ${lru_src}
EXAMPLE_SOURCES=${lfu_src}

# Generate the names of EXAMPLE's object files
EXAMPLE_OBJS=${EXAMPLE_SOURCES:.cpp=.o}

all: example

# Compile
example: ${EXAMPLE_OBJS}
	${CC} -o $@ $^ -ldl

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${EXAMPLE_OBJS} example
