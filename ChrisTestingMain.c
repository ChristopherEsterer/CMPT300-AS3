// file for compling and testing code
#include <stdio.h>
#include <stdlib.h>
#include "UDPReceive.h"
#include "UDPSend.h"
#include "Print.h"
#include "ProtectedList.h"
#include <string.h>
#include <pthread.h>
#include "Keyboard.h"
#include "ChrisTestingMain.h"


static pthread_cond_t s_syncOkToShutdownCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToShutdownMutex = PTHREAD_MUTEX_INITIALIZER;

int senderPort = 0;
char* receiverPort;
char* address = "10.0.0.168";

int main(int argCount, char** args)
{
	//printf("Arguments (%d) are: \n", argCount);
	//for (int i = 0; i < argCount; i++) {
	//	printf("Arg %d: %s\n", i, args[i]);
	//}
    senderPort = atoi(args[1]);
    //receiverPort = atoi(args[2]);

    receiverPort = args[2];
    address = args[3];
	//printf("Chris's Testing on UDP Sending port %d:\n", senderPort);
	//printf("Connect using: \n");
	//printf("    netcat -u 127.0.0.1 %d\n", receiverPort);
    //printf("Starting..\n");

    // Startup my modules
    // pthread_mutex_t sharedMutex = ...;
    // Receiver_init("Sire! A message! ", &sharedMutex);
    //printf("List..\n");
    InitLists(); // should be called first.
    printf("Receiver..\n");

    Receiver_init(address ,receiverPort);
    //printf("Printer..\n");
    Printer_init();
    //printf("Sender..\n");
    SenderInit(senderPort);
    //printf("Keyboard...\n");
    KeyboardInit();


// Wait for condition signal from SignalMessage.
    pthread_mutex_lock(&s_syncOkToShutdownMutex);
        {
        pthread_cond_wait(&s_syncOkToShutdownCondVar, &s_syncOkToShutdownMutex); // this will be signalled by external
        }
    pthread_mutex_unlock(&s_syncOkToShutdownMutex);
    
    MainShutdown(); // shutdown subroutine

    printf("done\n");
    return 0;
}
void ShutdownSignalMessage(void) // External Signal Call to tell the Sender to send. Protected
{
    //printf("Main: Shutdown Triggered!\n");

    pthread_mutex_lock(&s_syncOkToShutdownMutex);
    {
        pthread_cond_signal(&s_syncOkToShutdownCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToShutdownMutex);
}

void MainShutdown(void) //shutdown subrouttine
{
    Receiver_shutdown();
    Printer_shutdown();
    SenderShutdown();
    KeyboardShutdown();
}