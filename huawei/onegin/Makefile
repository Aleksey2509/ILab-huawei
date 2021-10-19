CXX = g++
CPPFLAGS = -std=c++11
OBJL = objfiles
OBJ = $(OBJL)/ReadWrite.o $(OBJL)/Compare.o $(OBJL)/sort.o $(OBJL)/Onegin.o $(OBJL)/testoutput.o $(OBJL)/main.o

$(OBJL)/%.o: %.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: compile

compile: $(OBJ)
	$(CXX) $(CPPFLAGS) -o main $(OBJ)

test:
	./main input.txt out

$(OBJL)/ReadWrite.o: ReadWrite.cpp

$(OBJL)/Compare.o: Compare.cpp

$(OBJL)/sort.o: sort.cpp

$(OBJL)/Onegin.o: Onegin.cpp

$(OBJL)/testoutput.o: testoutput.cpp

$(OBJL)/main.o: main.cpp