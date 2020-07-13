// A wrapper to make the list from AS1 protected with mutexes
// This will be a protected wrapper for the list.
// almost like a C++ class
// the list callfunctions need to be verified

#include <pthread.h> // for mutex
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ProtectedList.h" // the .h file for the function prototypes
#include "list.h" // the list files from AS1 instructorList.o will need to be included into the makefile for compileing

// the lists will have a mutex that it will lock and unlock itself.
// this will prevent one thread from calling Get() while another thread calls Set().

static pthread_mutex_t INlistLockMutex = PTHREAD_MUTEX_INITIALIZER; 
static pthread_mutex_t OUTlistLockMutex = PTHREAD_MUTEX_INITIALIZER;

static List* inputList; //= malloc(sizeof(List)); // a list for input messages (messages to be printed)
static List* outputList; // a list for output messages (messages to be sent)

char* GetMessageFromInputList(void) // pops a message from the tail of the input list. protected
{
    static char* tempMsg; // make a temporary message holder

    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
        tempMsg = List_trim(inputList); // returns the tail of the list and removes it
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList

    TestPrintMsg(tempMsg);
    return tempMsg; // return the temp message
}
char* GetMessageFromOutputList(void) // pops a message from the tail of the output list. protected
{
    static char* tempMsg; // make a temporary message holder

    pthread_mutex_lock(&OUTlistLockMutex); //lock Outputlist
    {
        tempMsg = List_trim(outputList); // returns the tail of the list and removes it
    }
    pthread_mutex_unlock(&OUTlistLockMutex);//unlock Outputlist

    return tempMsg; // return the temp message
}


void SetMessageToInputList(char* msg)
{
    TestPrintMsg(msg);
    int error;
    pthread_mutex_lock(&INlistLockMutex); //lock Inputlist
    {
        error = List_prepend(inputList, msg);
    }
    pthread_mutex_unlock(&INlistLockMutex);//unlock InputList
    
    printf("The set error is %d : \n", error);
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
void TestPrintMsg(char* msg) // simple printf function
{
    printf("The list says the msg is %s : \n", msg);
}
void InitLists(void)// allocate the memory for the lists
{   
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