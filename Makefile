# Recursively search for prog*.cpp files.
YEARS=$(wildcard 20*)
SRC:=$(foreach YEAR, $(YEARS), $(shell find $(YEAR) -name '*.cpp'))
PROG=$(patsubst %.cpp, %, $(SRC))
CPPFLAGS=-std=c++23 -Ilib/ -Itest/

# Include library
LIB=$(wildcard lib/*.cpp)
LIB_OBJ=$(patsubst %.cpp, %.o, $(LIB))

# Unit tests
TEST_SRC=$(wildcard test/*.cpp)
TEST_PROG+=$(patsubst %.cpp, %, $(TEST_SRC))
PROG+=$(TEST_PROG)

all : $(PROG) .test .run

%.o : %.cpp
	g++ -MD $(CPPFLAGS) -c $< -o $@

$(PROG) : % : %.o $(LIB_OBJ)
	g++ -MD $< $(LIB_OBJ) -o $@

lib/%.o : lib/%.cpp
	g++ -MD -c $< -o $@

# run.sh is excetuted with out of date targets ($?) of .run
.run: $(PROG)
	@./run.sh $?
	@touch $@


.PHONY: clean
clean:
	@echo "[CLEAN]"
	@rm .run .test $(SRC:%.cpp=%.d) $(SRC:%.cpp=%.o) lib/*.o lib/*.d $(PROG)

# .test is a placeholder. Unit tests are run if they have been recently compiled.
.test: $(TEST_PROG)
	@for test in $? ; do \
		$${test} ; \
	done
	@touch .test

# Include .d files generated by -MD flag
-include $(SRC:%.cpp=%.d)
-include $(TEST_PROG:%=%.d)
