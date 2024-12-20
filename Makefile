# Recursively search for prog*.cpp files.
YEARS=$(wildcard 20*)
SRC:=$(foreach YEAR, $(YEARS), $(shell find $(YEAR) -name 'part*.cpp'))
PROG=$(patsubst %.cpp, %, $(SRC))
CPPFLAGS=-std=c++23 -Ilib/ -Itest/

# Include library
LIB=$(wildcard lib/*.cpp)
LIB_OBJ=$(patsubst %.cpp, %.o, $(LIB))

# Unit tests
TEST_SRC=$(wildcard test/*.cpp)
TEST_PROG+=$(patsubst %.cpp, %, $(TEST_SRC))
PROG+=$(TEST_PROG)

# Examples
EXAMPLE_SRC=$(wildcard examples/*.cpp)
EXAMPLE_PROG+=$(patsubst %.cpp, %, $(EXAMPLE_SRC))
PROG+=$(EXAMPLE_PROG)

all : $(PROG) .test .run

%.o : %.cpp
	@echo -e "[\033[1;36mCPP\033[0m]\t$<"
	@g++ -MD $(CPPFLAGS) -c $< -o $@

$(PROG) : % : %.o $(LIB_OBJ)
	@echo -e "[\033[1;34mLD\033[0m]\t$@"
	@g++ -MD $< $(LIB_OBJ) -o $@

lib/%.o : lib/%.cpp
	@echo -e "[\033[1;36mCPP\033[0m]\t$<"
	@g++ -MD $(CPPFLAGS) -c $< -o $@

# run.sh is excetuted with out of date targets ($?) of .run
.run: $(PROG)
	@./run.sh $?
	@touch $@

.PHONY: run test clean
clean:
	@echo "[CLEAN]"
	@rm -f .run .test
	@rm -f $(TEST_SRC:%.cpp=%.d) $(TEST_SRC:%.cpp=%.o)
	@rm -f $(SRC:%.cpp=%.d) $(SRC:%.cpp=%.o) lib/*.o lib/*.d $(PROG)

run:
	@./run.sh $(PROG)

test:
	@for test in $(TEST_PROG) ; do \
		$${test} ; \
	done

# .test is a placeholder. Unit tests are run if they have been recently compiled.
.test: $(TEST_PROG)
	@for test in $? ; do \
		$${test} ; \
	done
	@touch .test

# Include .d files generated by -MD flag
-include $(SRC:%.cpp=%.d)
-include $(TEST_PROG:%=%.d)
