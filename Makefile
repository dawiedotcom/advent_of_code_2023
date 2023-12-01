SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))

all : $(PROG)

.PHONY: run
run: aoc
	./aoc < input

.PHONY: clean
clean:
	rm $(PROG)
