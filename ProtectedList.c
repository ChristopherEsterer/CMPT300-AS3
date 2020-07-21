// A wrapper to make the list from AS1 protected with mutexes
// This will be a protected wrapper for the list.
// almost like a C++ class
// the list callfunctions need to be verified

#include <pthread.h> // for mutex
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Print.h"
#include "ProtectedList.h" // the .h file for the function prototypes
#include "list.h" // the list files from AS1 instructorList.o will need to be included into the makefile for compileing

// the lists will have a mutex that it will lock and unlock itself.
// this will prevent one thread from calling Get() while another thread calls Set().
#define DYNAMIC_LEN 128

static pthread_mutex_t INlistLockMutex = PTHREAD_MUTEX_INITIALIZER; 
static pthread_mutex_t OUTlistLockMutex = PTHREAD_MUTEX_INITIALIZER;

static List* inputList;  // a list for input messages (messages to be printed)
static List* outputList; // a list for output messages (messages to be sent)

//static char* tempMsg; // dynamic message for string copy

char* GetMessageFromInputList(void) // pops a message from the tail of the input list. protected
{
    char* tempMsg; // make a temporary message holder

    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
        tempMsg = List_trim(inputList); // returns the tail of the list and removes it
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList

    
    return tempMsg; // return the temp message
}
void GetMessageFromInputList2(char* holder) // pops a message from the tail of the input list. protected
{
    char* tempMsg; // make a temporary message holder

    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
        tempMsg = List_trim(inputList);
        strncpy(holder, tempMsg , strlen(tempMsg) ); // ***new
        
        //tempMsg = List_trim(inputList); // returns the tail of the list and removes it
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList

    
    return; // return the temp message
}
char* GetMessageFromOutputList(void) // pops a message from the tail of the output list. protected
{
    char* tempMsg; // make a temporary message holder

    pthread_mutex_lock(&OUTlistLockMutex); //lock Outputlist
    {
        tempMsg = List_trim(outputList); // returns the tail of the list and removes it
    }
    pthread_mutex_unlock(&OUTlistLockMutex);//unlock Outputlist

    return tempMsg; // return the temp message
}


void SetMessageToInputList(char* msg)
{
    //int error;
    //char* tempMsg = msg; // make a temporary message holder
    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
       // error = 
       //tempMsg =
       //strncpy(tempMsg, msg, strlen(msg)); //**new
       //List_prepend(inputList, tempMsg); // old
       List_prepend(inputList, msg);
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList
    
    
    return;
}
void SetMessageToOutputList(char* msg)
{
    pthread_mutex_lock(&OUTlistLockMutex); //lock Outputlist
    {
        List_prepend(outputList, msg);
    }
    pthread_mutex_unlock(&OUTlistLockMutex);//unlock Outputlist

    return;
}
void SignalPrintMsg(void)  //char* msg) // simple printf function
{
    Printer_signalMessage(); // Printer call to tell its convar to run. Printer holds the mutex for the convar.
   // printf("The list says signaling print/n");
}
int GetListSize(void)
{
int i = 0;
i = List_count(inputList);
return i;
}
void InitLists(void)// allocate the memory for the lists
{   
    //tempMsg = malloc(DYNAMIC_LEN);
    
    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
    inputList = List_create();
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList
    
    pthread_mutex_lock(&OUTlistLockMutex); //lock Outputlist
    {
    outputList = List_create();
    }
    pthread_mutex_unlock(&OUTlistLockMutex);//unlock Outputlist
}