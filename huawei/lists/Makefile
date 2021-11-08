CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
EXECL = executables
OBJ = $(OBJL)/list.o

$(OBJL)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: CompileStack

CompileStack: $(OBJ)

test: $(OBJL)/test.o $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(EXECL)/test $(OBJ) $(OBJL)/test.o
	$(EXECL)/test

$(OBJL)/test.o: test.cpp
