# Compiler and flags
CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS = 

# Targets and dependencies
all: final_project

final_project: final_project.o debounce.o buttons.o
	$(CC) $(LFLAGS) -o final_project final_project.o debounce.o buttons.o

final_project.o: final_project.c debounce.h buttons.h
	$(CC) $(CFLAGS) -c final_project.c

debounce.o: debounce.c debounce.h
	$(CC) $(CFLAGS) -c debounce.c

buttons.o: buttons.c buttons.h
	$(CC) $(CFLAGS) -c buttons.c

# Clean up build artifacts
clean:
	rm -f final_project *.o
