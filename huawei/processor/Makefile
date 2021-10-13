CXX = g++
CPPFLAGS = -std=c++11
OBJL = objectfiles
OBJ = $(OBJL)/ReadWrite.o $(OBJL)/stack.o

$(OBJL)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: standart

standart: $(OBJ)

proc: $(OBJ) $(OBJL)/processor.o
	$(CXX) $(CPPFLAGS) -o proc  $(OBJ) $(OBJL)/processor.o

assembler: $(OBJ) $(OBJL)/assembler.o
	$(CXX) $(CPPFLAGS) -o assembler  $(OBJ) $(OBJL)/assembler.o

test_proc:
	./proc code.txt

test_asm:
	./assembler instructs

$(OBJL)/ReadWrite.o: ReadWrite.cpp

$(OBJL)/stack.o: stack.cpp

$(OBJL)/processor.o: processor.cpp

$(OBJL)/assembler.o: assembler.cpp