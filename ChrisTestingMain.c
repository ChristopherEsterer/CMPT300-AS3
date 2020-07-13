// file for compling and testing code
#include <stdio.h>
#include <stdlib.h>
#include "UDPReceive.h"

int main(int argc, char** args)
{
    printf("Starting..\n");

    // Startup my modules
    // pthread_mutex_t sharedMutex = ...;
    // Receiver_init("Sire! A message! ", &sharedMutex);
    Receiver_init("Sire! A message! ");

    // Wait for user input
    printf("Enter something to kill the receive thread\n");
    char x;
    scanf("%c", &x);

    // Shutdown my modules
    Receiver_shutdown();

    printf("done\n");
    return 0;
}