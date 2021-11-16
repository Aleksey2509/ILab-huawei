CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
EXECL = executables
OBJ = $(OBJL)/stack.o
TESTL = tests

$(OBJL)/%.o: $(TESTL)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

%:$(OBJ) $(OBJL)/%.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/$@ $^
	$(EXECL)/$@

all: CompileStack

CompileStack: $(OBJ)

PopEmpty: $(OBJ) $(OBJL)/PopEmpty.o

TwiceDestr: $(OBJ) $(OBJL)/TwiceDestr.o

Annihilate: $(OBJ) $(OBJL)/Annihilation.o

ResizeTest: $(OBJ) $(OBJL)/ResizeTest.o

ZeroCap: $(OBJ) $(OBJL)/ZeroCap.o

NotInit: $(OBJ) $(OBJL)/NotInit.o

PopPoisoned: $(OBJ) $(OBJL)/PopPoisoned.o

$(OBJL)/stack.o: stack.cpp config.h stack.hpp
	$(CXX) $(CPPFLAGS) -c -o $(OBJL)/stack.o stack.cpp

$(OBJL)/PopEmpty.o: $(TESTL)/PopEmpty.cpp

$(OBJL)/TwiceDestr.o: $(TESTL)/TwiceDestr.cpp

$(OBJL)/Annihilation.o: $(TESTL)/Annihilation.cpp

$(OBJL)/ResizeTest.o: $(TESTL)/ResizeTest.cpp

$(OBJL)/ZeroCap.o: $(TESTL)/ZeroCap.cpp

$(OBJL)/NotInit.o: $(TESTL)/NotInit.cpp

$(OBJL)/PopPoisoned.o: $(TESTL)/PopPoisoned.cpp