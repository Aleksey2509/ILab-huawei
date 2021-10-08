CXX = g++
CPPFLAGS = -std=c++11
OBJL = objectfiles
OBJ = $(OBJL)/ReadWrite.o $(OBJL)/stack.o $(OBJL)/proc.o

$(OBJL)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: proc

proc: $(OBJ)
	$(CXX) $(CPPFLAGS) -o proc  $(OBJ)

test:
	./proc instructs

$(OBJL)/ReadWrite.o: ReadWrite.cpp

$(OBJL)/stack.o: stack.cpp

$(OBJL)/proc.o: proc.cpp
