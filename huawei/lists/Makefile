CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
EXECL = executables
HEADL = headers
SRC = source
OBJ = $(OBJL)/list.o $(OBJL)/sortlist.o

$(OBJL)/%.o: $(SRC)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $<

all: CompileList

CompileList: $(OBJ) 

test: $(OBJL)/test.o $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(EXECL)/test $(OBJ) $(OBJL)/test.o
	$(EXECL)/test

graph: $(OBJL)/graphicdump.o $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(EXECL)/graph $(OBJ) $(OBJL)/graphicdump.o
	$(EXECL)/graph

$(OBJL)/test.o: $(SRC)/test.cpp $(HEADL)/sortlist.hpp

$(OBJL)/graphicdump.o: $(SRC)/graphicdump.cpp

$(OBJL)/list.o: $(SRC)/list.cpp $(HEADL)/list.hpp

$(OBJL)/sortlist.o: $(SRC)/sortlist.cpp