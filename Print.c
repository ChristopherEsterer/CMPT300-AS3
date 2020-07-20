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
     
        pthread_mutex_lock(&s_syncOkToPrintMutex);
        {
        pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex); // this will be signalled by external
        }
        pthread_mutex_unlock(&s_syncOkToPrintMutex);
        //GetMessageFromInputList2(messageToPrint);
        //for (const char* msg = GetMessageFromInputList(); *msg != '\0'; msg++) {

            //   printf("%c", *msg);
            //    fflush(stdout);
        //}




        
        //char* messageToPrint = GetMessageFromInputList();
        //GetMessageFromInputList2(messageToPrint);// this is protected from the protectedList ADT
      
       // printf("%s", messageToPrint);
       // fprintf( stdout, "%s" , messageToPrint );
       // fflush(stdout);
        //}
    }
    return NULL;
/*
    pthread_mutex_lock(&s_syncOkToPrintMutex);
    {
        pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex);
    }
    pthread_mutex_unlock(&s_syncOkToPrintMutex);
    for (const char* msg = (); *msg != '\0'; msg++) {
        // Wait until signalled
        //pthread_mutex_lock(&s_syncOkToPrintMutex);
        //{
       //     pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex);
       // }
       // pthread_mutex_unlock(&s_syncOkToPrintMutex);
        

        printf("%c", *msg);
        fflush(stdout);
    }

    return NULL;
*/
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
    
   // InitLists(); // Initalized the lists for memory allocation done in main. *** is it?

    //s_rxMessage = rxMessage;
    pthread_create(
        &threadPrinter,         // PID (by pointer)
        NULL,               // Attributes
        PrinterThread,      // Function
        NULL);
}

void Printer_shutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPrinter);

    // Waits for thread to finish
    pthread_join(threadPrinter, NULL);

    // Cleanup memory
  
}