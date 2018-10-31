# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS =  -Wall -g

# ****************************************************
# Entries to bring the executable up to date

test:  main.o
	$(CC) $(CFLAGS) -o test main.o

#hash.o: hash.c hash.h
#	$(CC) $(CFLAGS) -c hash.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm test main.o
