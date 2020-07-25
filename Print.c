// this will be signaled by the UDPReceive when it has a new message to print.
// and keep printing as long as something is in the list
// Code edited by Chris Esterer 301116353. Supplied by Brian Fraiser.

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h> 
#include "Print.h" // this thread
// for the Convar from Receive to signal a Print.
#include "ProtectedList.h" // our list ADT with mutex protection.

#define DYNAMIC_LEN 128

pthread_t threadPrinter;

static pthread_cond_t s_syncOkToPrintCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToPrintMutex = PTHREAD_MUTEX_INITIALIZER;

char* messageToPrint;


void* PrinterThread(void* unused){
    while(1) {
     
     //wait untill we are signaled from out side to print
        pthread_mutex_lock(&s_syncOkToPrintMutex);
        {
            pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex); // this will be signalled by external
        }
        pthread_mutex_unlock(&s_syncOkToPrintMutex);    
        GetMessageFromInputList2(messageToPrint);
        
        fputs(messageToPrint,stdout); // print call some work better than others.

        fflush(stdout);

    
    }
    return NULL;

}


void Printer_signalMessage(void) // from workshop. Use to lock the condition variable. This is the external signnal call
{
    pthread_mutex_lock(&s_syncOkToPrintMutex);
    {
        pthread_cond_signal(&s_syncOkToPrintCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToPrintMutex);
}


void Printer_init()
{
    messageToPrint = malloc(DYNAMIC_LEN);
    

    pthread_create(
        &threadPrinter,         // PID (by pointer)
        NULL,               // Attributes
        PrinterThread,      // Function
        NULL);
}

void PrinterShutdown(void)
{
    free(messageToPrint);
    // Cancel thread
    pthread_cancel(threadPrinter);

    // Waits for thread to finish
    pthread_join(threadPrinter, NULL);

    // Cleanup memory
  
}