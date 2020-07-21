#makefile for Main folder. Has Git and build commands for CMPT As3

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build:
	gcc $(CFLAGS) demo_conditions.c general.c printer.c signaller.c instructorList.o -o demo_conditions -lpthread 
# include instructorList.o to compile the list ADT

buildCTest:
	gcc $(CFLAGS) instructorList.o UDPReceive.c UDPSend.c Print.c ProtectedList.c ChrisTestingMain.c -o TestChris -lpthread 

run: build
	./demo_conditions

valgrind: build
	valgrind --leak-check=full ./demo_conditions

clean:
	rm -f demo_conditions

pull:
	git pull origin master