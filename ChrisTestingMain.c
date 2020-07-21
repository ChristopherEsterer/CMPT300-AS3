// file for compling and testing code
#include <stdio.h>
#include <stdlib.h>
#include "UDPReceive.h"
#include "Print.h"
#include "ProtectedList.h"
int main(int argc, char** args)
{
    printf("Starting..\n");

    // Startup my modules
    // pthread_mutex_t sharedMutex = ...;
    // Receiver_init("Sire! A message! ", &sharedMutex);
    Receiver_init("Sire! A message! ");
    Printer_init();
    // Wait for user input
    printf("Enter something to print\n");
    char x;
    scanf("%c", &x);
    //SignalPrintMsg();
    // Shutdown my modules
    //char x;
    scanf("%c", &x);
    printf("Enter something to kill the receive thread\n");
    //char x;
    scanf("%c", &x);
    Receiver_shutdown();

    printf("done\n");
    return 0;
}