CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wpedantic
CPP_DEBUG_FLAGS = -g -O1 # add this stuff usage
OBJL = objfiles
SRCL = Source
OBJ = $(OBJL)/ReadWrite.o $(OBJL)/Compare.o $(OBJL)/Onegin.o $(OBJL)/testoutput.o $(OBJL)/main.o

$(OBJL)/%.o: $(SRCL)/%.cpp 
	$(CXX) $(CPPFLAGS) -c -o $@ $^

all: compile

compile: $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(OBJL)/main $(OBJ)

withCheck:
	$(OBJL)/main input.txt out

test:
	$(OBJL)/main input.txt out

$(OBJL)/ReadWrite.o: $(SRCL)/ReadWrite.cpp

$(OBJL)/Compare.o: $(SRCL)/Compare.cpp

$(OBJL)/Onegin.o: $(SRCL)/Onegin.cpp

$(OBJL)/testoutput.o: $(SRCL)/testoutput.cpp

$(OBJL)/main.o: $(SRCL)/main.cpp