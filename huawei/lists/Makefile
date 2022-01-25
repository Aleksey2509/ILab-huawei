CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
EXECL = executables
HEADL = headers
SRC = source
OBJ = $(OBJL)/list.o

$(OBJL)/%.o: $(SRC)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $<

all: CompileList

CompileList: $(OBJ) 

test: $(OBJL)/test.o $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(EXECL)/test $(OBJ) $(OBJL)/test.o
	$(EXECL)/test

$(OBJL)/test.o: $(SRC)/test.cpp

$(OBJL)/list.o: $(SRC)/list.cpp $(HEADL)/list.hpp