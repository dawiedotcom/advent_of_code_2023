SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))

all : $(PROG) test

.PHONY: run
run: aoc
	./aoc < input

.PHONY: clean
clean:
	rm $(PROG)

.PHONY: test
test: $(PROG)
	@./test.sh
