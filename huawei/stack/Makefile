CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
EXECL = executables
OBJ = $(OBJL)/stack.o
TESTL = tests

$(OBJL)/%.o: $(TESTL)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

$(OBJL)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: compile

compile: $(OBJ)

PopEmpty: $(OBJ) $(OBJL)/PopEmpty.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/PopEmpty $(OBJL)/PopEmpty.o $(OBJ) 
	$(EXECL)/PopEmpty

TwiceDestr: $(OBJ) $(OBJL)/TwiceDestr.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/TwiceDestr $(OBJL)/TwiceDestr.o $(OBJ) 
	$(EXECL)/TwiceDestr

Annihilate: $(OBJ) $(OBJL)/Annihilation.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/Annihilation $(OBJL)/Annihilation.o $(OBJ) 
	$(EXECL)/Annihilation

ResizeTest: $(OBJ) $(OBJL)/ResizeTest.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/ResizeTest $(OBJL)/ResizeTest.o $(OBJ) 
	$(EXECL)/ResizeTest

ZeroCap: $(OBJ) $(OBJL)/ZeroCap.o
	$(CXX) $(CPPFLAGS) -o $(EXECL)/ZeroCap $(OBJL)/ZeroCap.o $(OBJ) 
	$(EXECL)/ZeroCap

$(OBJL)/stack.o: stack.cpp

$(OBJL)/PopEmpty.o: $(TESTL)/PopEmpty.cpp

$(OBJL)/TwiceDestr.o: $(TESTL)/TwiceDestr.cpp

$(OBJL)/Annihilation.o: $(TESTL)/Annihilation.cpp

$(OBJL)/ResizeTest.o: $(TESTL)/ResizeTest.cpp

$(OBJL)/ZeroCap.o: $(TESTL)/ZeroCap.cpp