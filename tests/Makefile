CC=g++ -g -std=c++14 -Wall -I ../

all: example

EXAMPLE_SRC=example.cpp

# Compile
example: ${EXAMPLE_SRC}
	${CC} -o $@ $^ -ldl

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<

# test src files
test_%: test_%.cpp
	${CC} -o $@_runner $^ -ldl

clean:
	rm -f ${EXAMPLE_OBJS} example
