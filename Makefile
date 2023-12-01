SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))

all : $(PROG)

.PHONY: run
run:
	./aoc < input

.PHONY: clean
clean:
	rm $(PROG)
