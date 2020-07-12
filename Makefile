#makefile for Main folder. Has Git and build commands for CMPT As3

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build:
	gcc $(CFLAGS) demo_conditions.c general.c printer.c signaller.c -o demo_conditions -lpthread 


run: build
	./demo_conditions

valgrind: build
	valgrind --leak-check=full ./demo_conditions

clean:
	rm -f demo_conditions

pull:
	git pull origin master