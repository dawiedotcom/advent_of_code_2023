SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))
CPPFLAGS=-std=c++23 -Ilib/

LIB=$(wildcard lib/*.cpp)
LIB_OBJ=$(patsubst %.cpp, %.o, $(LIB))

all : $(PROG) test

%.o : %.cpp
	g++ -MD $(CPPFLAGS) -c $< -o $@

$(PROG) : % : %.o $(LIB_OBJ)
	g++ $< $(LIB_OBJ) -o $@

lib/%.o : lib/%.cpp
	g++ -MD -c $< -o $@

.PHONY: run
run: aoc
	./aoc < input

.PHONY: clean
clean:
	rm *.d *.o lib/*.o lib/*.d $(PROG)

.PHONY: test
test: $(PROG)
	@./test.sh

-include $(SRC:%.cpp=%.d)
