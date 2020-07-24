#makefile for Main folder. Has Git and build commands for CMPT As3

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build:
	gcc $(CFLAGS) demo_conditions.c general.c printer.c signaller.c instructorList.o -o demo_conditions -lpthread 
# include instructorList.o to compile the list ADT

buildCTest:
	gcc $(CFLAGS) instructorList.o UDPReceive.c UDPSend.c Print.c ProtectedList.c Keyboard.c ChrisTestingMain.c -o TestChris -lpthread 

run: build
	./demo_conditions

valgrindCT: buildCTest
	valgrind --leak-check=full ./TestChris

valgrind: build
	valgrind --leak-check=full ./demo_conditions	

clean:
	rm -f demo_conditions

pull:
	git pull origin master

CsendCSIL:
	scp -P24 TestChris cesterer@csil-cpu5.cs.surrey.sfu.ca:~/cmpt300/

GsendCSIL:
	cp -P24 TestChris gsg27@csil-cpu7.cs.surrey.sfu.ca:~/sfuhome/cmpt-300

runC:
	./TestChris 22112 csil-cpu5.cs.surrey.sfu.ca 22111

runG:
	./TestChris 22111 csil-cpu7.cs.surrey.sfu.ca 22112

runCT:
	gnome-terminal -- ./TestChris 22111 10.0.0.168 22112
	./TestChris 22112 10.0.0.168 22111

# some mad long ones
buildAndTest:
	gcc $(CFLAGS) instructorList.o UDPReceive.c UDPSend.c Print.c ProtectedList.c Keyboard.c ChrisTestingMain.c -o s-talk -lpthread

	gnome-terminal -- make remoteTest
	
	putty -P 24 cesterer@csil-cpu6.cs.surrey.sfu.ca
	scp -P24 s-talk cesterer@csil-cpu5.cs.surrey.sfu.ca:~/cmpt300/
	./s-talk 25011 cesterer@csil-cpu5 26011

remoteTest:
	putty -P 24 cesterer@csil-cpu5.cs.surrey.sfu.ca
	./s-talk 26011 csil-cpu6.cs.surrey.sfu.ca 25011

updateRemote:
	gcc $(CFLAGS) instructorList.o UDPReceive.c UDPSend.c Print.c ProtectedList.c Keyboard.c ChrisTestingMain.c -o s-talk -lpthread
	scp -P24 s-talk cesterer@csil-cpu5.cs.surrey.sfu.ca:~/cmpt300/

	gnome-terminal -- scp -P24 s-talk cesterer@csil-cpu7.cs.surrey.sfu.ca:~/cmpt300/
buildDebug:
	gcc $(CFLAGS) instructorList.o UDPReceive.c UDPSend.c Print.c ProtectedList.c Keyboard.c ChrisTestingMain.c -o s-talk -lpthread
	./s-talk 27012 10.0.0.168 27011
