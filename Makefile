# Compiler and flags
CC = gcc
CFLAGS = -g -O2 -Wall
LFLAGS = 

# Targets and dependencies
all: final_project

final_project: final_project.o debounce.o buttons.o display.o pouring.o
	$(CC) $(LFLAGS) -o final_project final_project.o debounce.o buttons.o display.o pouring.o

final_project.o: final_project.c debounce.h buttons.h display.h pouring.h
	$(CC) $(CFLAGS) -c final_project.c

debounce.o: debounce.c debounce.h
	$(CC) $(CFLAGS) -c debounce.c

display.o: display.c display.h
	$(CC) $(CFLAGS) -c display.c

pouring.o: pouring.c pouring.h
	$(CC) $(CFLAGS) -c pouring.c

# Clean up build artifacts
clean:
	rm -f final_project *.o
