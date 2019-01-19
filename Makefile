# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS =  -Wall -g

# ****************************************************
# Entries to bring the executable up to date

#hash.o: hash.c hash.h
#	$(CC) $(CFLAGS) -c hash.c
all: main.o unitTest.o join.o bucketChain.o query_parser.o process_query.o nmap.o
	gcc -o main main.o bucketChain.o join.o query_parser.o process_query.o nmap.o -g
	gcc -o unitTest unitTest.o bucketChain.o join.o query_parser.o process_query.o nmap.o -g

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

unitTest.o: unitTest.c
	$(CC) $(CFLAGS) -c unitTest.c

bucketChain.o: bucketChain.c
	$(CC) $(CFLAGS) -c bucketChain.c

join.o: join.c
	$(CC) $(CFLAGS) -c join.c

query_parser.o: query_parser.c
	$(CC) $(CFLAGS) -c query_parser.c

nmap.o: nmap.c
	$(CC) $(CFLAGS) -c nmap.c

process_query.o: process_query.c
	$(CC) $(CFLAGS) -c process_query.c

clean :
	rm  main.o join.o main bucketChain.o query_parser.o nmap.o 
