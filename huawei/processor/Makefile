CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
SRCL = Source
EXECL = executables
HEADL = Headers
OBJ = $(OBJL)/textFunc.o $(OBJL)/stack.o

$(OBJL)/%.o: $(SRCL)/%.cpp $(HEADL)/%.hpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(OBJL)/%.o: $(SRCL)/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(HEADL)/%.hpp:
	

all: utils

utils: $(OBJ)

compile_all: compile_proc compile_asm

compile_proc: $(OBJ) $(OBJL)/processor.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/proc  $(OBJ) $(OBJL)/processor.o

compile_asm: $(OBJ) $(OBJL)/assembler.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/assembler  $(OBJ) $(OBJL)/assembler.o

compile_disasm: $(OBJ) $(OBJL)/disassembler.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/disassembler  $(OBJ) $(OBJL)/disassembler.o

run_all: run_asm run_proc

run_proc:
	$(EXECL)/proc code.txt

run_asm:
	$(EXECL)/assembler instructs

run_disasm:
	$(EXECL)/disassembler code.txt

$(OBJL)/textFunc.o: $(SRCL)/textFunc.cpp $(HEADL)/textFunc.hpp

$(OBJL)/stack.o: $(SRCL)/stack.cpp $(HEADL)/stack.hpp

$(OBJL)/processor.o: $(SRCL)/processor.cpp $(HEADL)/processor.hpp

$(OBJL)/assembler.o: $(SRCL)/assembler.cpp $(HEADL)/assembler.hpp