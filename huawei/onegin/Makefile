CC = g++
CFLAGS = -std=c++11
OBJ = ReadWrite.o Compare.o sort.o Onegin.o main.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

ReadWrite.o: ReadWrite.cpp struct.hpp ReadWrite.hpp
	$(CC) $(CFLAGS) -c -o ReadWrite.o ReadWrite.cpp

Compare.o: Compare.cpp struct.hpp Compare.hpp
	$(CC) $(CFLAGS) -c -o Compare.o Compare.cpp

sort.o: sort.cpp struct.hpp Compare.hpp
	$(CC) $(CFLAGS) -c -o sort.o sort.cpp

Onegin.o: Onegin.cpp struct.hpp ReadWrite.hpp Compare.hpp sort.hpp
	$(CC) $(CFLAGS) -c -o Onegin.o Onegin.cpp

main.o: main.cpp struct.hpp ReadWrite.hpp Compare.hpp sort.hpp Onegin.hpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp
