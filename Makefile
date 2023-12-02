SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))
CPPFLAGS=-Ilib/

LIB=$(wildcard lib/*.cpp)
LIB_OBJ=$(patsubst %.cpp, %.o, $(LIB))

$(info $(LIB))
$(info $(LIB_OBJ))

all : $(PROG) test

%.o : %.cpp
	g++ -MD $(CPPFLAGS) -c $< -o $@

$(PROG) : % : %.o $(LIB_OBJ)
	@echo 'HI'
	g++ $< $(LIB_OBJ) -o $@

lib/%.o : lib/%.cpp
	g++ -MD -c $< -o $@

.PHONY: run
run: aoc
	./aoc < input

.PHONY: clean
clean:
	rm $(SRC:%.cpp=%.d)
	rm $(PROG)
	rm $(LIB_OBJ)

.PHONY: test
test: $(PROG)
	@./test.sh

-include $(SRC:%.cpp=%.d)
