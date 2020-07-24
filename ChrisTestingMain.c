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

char* senderPort;
char* receiverPort;
char* address;

int main(int argCount, char** args)
{
    //s-talk [my port number] [remote machine name] [remote port number]
    
    receiverPort = args[1];
    address = args[2];
    senderPort = args[3];

    ListInit(); // Should be called first.

    ReceiverInit(address ,receiverPort);
    Printer_init();
    SenderInit(address ,senderPort);
    KeyboardInit();


// Wait for condition signal from SignalMessage.
    pthread_mutex_lock(&s_syncOkToShutdownMutex);
        {
        pthread_cond_wait(&s_syncOkToShutdownCondVar, &s_syncOkToShutdownMutex); // this will be signalled by external
        }
    pthread_mutex_unlock(&s_syncOkToShutdownMutex);
    
    MainShutdown(); // Shutdown Subroutine

    printf("done\n");
    return 0;
}
void ShutdownSignalMessage(void) // External Signal Call to tell the Sender to send. Protected
{
    pthread_mutex_lock(&s_syncOkToShutdownMutex);
    {
        pthread_cond_signal(&s_syncOkToShutdownCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToShutdownMutex);
}

void MainShutdown(void) //shutdown subrouttine
{
    ReceiverShutdown();
    PrinterShutdown();
    SenderShutdown();
    KeyboardShutdown();
    ListShutdown();

}