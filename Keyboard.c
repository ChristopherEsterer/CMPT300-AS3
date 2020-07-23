// thread module for takeing input from keyboard.
// Will wait for keyboard commands and push them to the input list
// Will look for an escape key and signal the escape ConVar which will trigger main to exit
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h> 
#include "Keyboard.h" // this thread
#include "ProtectedList.h"
#include "UDPSend.h"
#include "ChrisTestingMain.h"

//Defines
#define DYNAMIC_LEN 128
pthread_t threadKeyboard;

void* KeyboardThread(void* unused)
{   
    char x[1024];
    do{// != "!\n"){
        

            fgets(x,1024,stdin);
            
            SetMessageToOutputList(x);
            SenderSignalMessage(); // signal Sender to send
    }while( strcmp(x,"!\n"));
    printf("Keyboard: Exit triggered! \n");
    ShutdownSignalMessage();
    return NULL;
}
void KeyboardInit(void)
{
    
    pthread_create(
        &threadKeyboard,         // PID (by pointer)
        NULL,               // Attributes
        KeyboardThread,      // Function
        NULL);
}

void KeyboardShutdown(void)
{
        // Cancel thread
    pthread_cancel(threadKeyboard);

    // Waits for thread to finish
    pthread_join(threadKeyboard, NULL);

    // Cleanup memory
}
//protectedList* Outlist;
//convar* sendConVar;
//inPutMain()
// This will stdin and save to a temp string (char*)
//outlist->SetMessageToList(tempMsg);
//signal the UDPSend to send a message from the outlist
//sendConVar.signal()
//{
//
//InitThread(protectedList outList){ // like all the other threads
//save the outList pointer
// create a thread and call the main function
//
//} 
