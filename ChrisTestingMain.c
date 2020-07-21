// file for compling and testing code
#include <stdio.h>
#include <stdlib.h>
#include "UDPReceive.h"
#include "UDPSend.h"
#include "Print.h"
#include "ProtectedList.h"

int senderPort = 0;
int receiverPort = 0;

int main(int argc, char** args)
{
    printf("Starting..\n");

    // Startup my modules
    // pthread_mutex_t sharedMutex = ...;
    // Receiver_init("Sire! A message! ", &sharedMutex);
    Receiver_init(receiverPort);
    InitLists();
    Printer_init();
    SenderInit(senderPort);
    // Wait for user input
    for (int i=0; i<5; i++){
        printf("Enter something to send: \n");
        char x[1024];
        scanf("%s", x);
        SetMessageToOutputList(x);
        SenderSignalMessage(); // signal Sender to send
    }
    //SignalPrintMsg();
    // Shutdown my modules
    char x;
    //scanf("%c", &x);
    printf("Enter something to kill the receive thread\n");
    //char x;
    scanf("%c", &x);
    Receiver_shutdown();

    printf("done\n");
    return 0;
}