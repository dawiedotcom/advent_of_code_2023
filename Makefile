SRC=$(wildcard *.cpp)
PROG=$(patsubst %.cpp, %, $(SRC))

all : $(PROG)
