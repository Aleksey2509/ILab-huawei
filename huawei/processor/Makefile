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

disassembler: $(OBJ) $(OBJL)/disassembler.o
	$(CXX) $(CPPFLAGS) -o disassembler  $(OBJ) $(OBJL)/disassembler.o

test_proc:
	./proc code.txt

test_asm:
	./assembler instructs

test_disasm:
	./disassembler code.txt

$(OBJL)/ReadWrite.o: ReadWrite.cpp

$(OBJL)/stack.o: stack.cpp

$(OBJL)/processor.o: processor.cpp

$(OBJL)/assembler.o: assembler.cpp