CC = gcc
CFLAGS = -Iinclude
CRELEASEFLAGS = -O2 -march=native -mtune=native
DEBUGFLAGS = -ggdb -Wall
LFLAGS = -lm
DEBUGGER = ddd

main: main.o include/lib.h lib/lib.o
	$(CC) -o main $(LFLAGS) main.o lib/lib.o

main_debug: main_debug.o include/lib.h lib/lib_debug.o
	$(CC) -o main_debug $(LFLAGS) $(DEBUGFLAGS) lib/lib_debug.o main_debug.o

main.o: main.c include/lib.h
	$(CC) -c $(CFLAGS) $(CRELEASEFLAGS) main.c -o main.o

main_debug.o: main.c include/lib.h
	$(CC) -c $(CFLAGS) $(DEBUGFLAGS) main.c -o main_debug.o

lib/lib.o: include/lib.h lib/lib.c
	$(CC) -c $(CFLAGS) $(CRELEASEFLAGS) -o lib/lib.o lib/lib.c

lib/lib_debug.o: include/lib.h lib/lib.c
	$(CC) -c $(CFLAGS) $(DEBUGFLAGS) -o lib/lib_debug.o lib/lib.c

clean:
	rm -f main main.o lib/lib.o
	rm -f main_debug main_debug.o lib/lib_debug.o

debug: main_debug
	$(DEBUGGER) main_debug

run: main
	./main

.PHONY: clean debug run
