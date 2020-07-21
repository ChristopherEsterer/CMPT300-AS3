// file for compling and testing code
#include <stdio.h>
#include <stdlib.h>
#include "UDPReceive.h"
#include "UDPSend.h"
#include "Print.h"
#include "ProtectedList.h"

int senderPort = 0;
int receiverPort = 0;

int main(int argCount, char** args)
{
	printf("Arguments (%d) are: \n", argCount);
	for (int i = 0; i < argCount; i++) {
		printf("Arg %d: %s\n", i, args[i]);
	}
    senderPort = atoi(args[1]);
    receiverPort = atoi(args[2]);
	printf("Chris's Testing on UDP Sending port %d:\n", senderPort);
	printf("Connect using: \n");
	printf("    netcat -u 127.0.0.1 %d\n", receiverPort);
    printf("Starting..\n");

    // Startup my modules
    // pthread_mutex_t sharedMutex = ...;
    // Receiver_init("Sire! A message! ", &sharedMutex);
    printf("List..\n");
    InitLists();
    printf("Receiver..\n");
    Receiver_init(receiverPort);
    printf("Printer..\n");
    Printer_init();
    printf("Sender..\n");
    SenderInit(senderPort);
    // Wait for user input
    char x[1024];
    do{
        printf("Enter something to send: \n");
        //char x[1024];
        scanf("%s", x);
        SetMessageToOutputList(x);
        SenderSignalMessage(); // signal Sender to send
    }while(*x != '!');
    //SignalPrintMsg();
    // Shutdown my modules
    //char x;
    //scanf("%c", &x);
   // printf("Enter something to kill the receive thread\n");
    //char x;
    //scanf("%c", &x);
    Receiver_shutdown();
    Printer_shutdown();
    SenderShutdown();
    printf("done\n");
    return 0;
}