# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS =  -Wall -g

# ****************************************************
# Entries to bring the executable up to date

#hash.o: hash.c hash.h
#	$(CC) $(CFLAGS) -c hash.c
all: main.o join.o
	gcc -o main main.o join.o -g

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

join.o: join.c
	$(CC) $(CFLAGS) -c join.c

clean :
	rm  main.o join.o main
