CXX = g++
CPPFLAGS = -std=c++11 -Iheaders
OBJL = objfiles
EXECL = executables
HEADL = headers
SRC = source
OBJ = $(OBJL)/list.o

$(OBJL)/%.o: $(SRC)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $<

all: CompileList

CompileList: $(OBJ) 

test: $(OBJ) $(OBJL)/test.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/test $(OBJ) $(OBJL)/test.o
	$(EXECL)/test

$(OBJL)/test.o: $(SRC)/test.cpp

$(OBJL)/list.o: $(SRC)/list.cpp $(HEADL)/list.hpp
	@mkdir -p executables
	@mkdir -p objfiles
	$(CXX) $(CPPFLAGS) -c -o $@ $<