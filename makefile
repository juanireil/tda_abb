CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
FLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes 

pruebas: pruebas.c abb.c pa2mm.h
	$(CC) pruebas.c abb.c -o abb $(CFLAGS)

minipruebas: abb_minipruebas.c abb.c
	$(CC)  abb_minipruebas.c abb.c -o abb $(CFLAGS)
.PHONY : clean
clean : 
	-rm abb

debug_pruebas : pruebas
	gdb ./abb

valgrind_pruebas: pruebas
	valgrind $(FLAGS_VALGRIND) ./abb

debug_minipruebas : minipruebas
	gdb ./abb

valgrind_minipruebas: minipruebas
	valgrind $(FLAGS_VALGRIND) ./abb