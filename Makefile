CC=gcc
CXX=g++
CFLAGS= -fopenmp -Wall -Werror -O0
SOURCES=pr3.c
EXECUTABLE=prob3

all: qs tim omp
	./$(EXECUTABLE) 3 3
omp:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)
qs:
	$(CC) $(CFLAGS) qs.c -o qs 
san:
	$(CC) $(CFLAGS) -fsanitize=address $(SOURCES) -o $(EXECUTABLE)
run: qs tim omp
	./sc.sh
tim:
	$(CXX) tim.cpp -o tim
format:
	clang-format -i -style=Chromium *.c

clean:
	rm -rf prob3 qs tim
