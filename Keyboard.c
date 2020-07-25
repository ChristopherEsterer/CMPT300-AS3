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
#define MSG_MAX_LEN 1024
pthread_t threadKeyboard;

void* KeyboardThread(void* unused)
{   
    //char x[MSG_MAX_LEN];
    char* x = NULL;
   //do{
    size_t length =0;
    //while( strcmp(x,"!\n")){
    do{
        x = NULL;
        length = 0;
        //fgets(x,MSG_MAX_LEN,stdin);
       // size_t getln = 1024L;
        getline(&x,&length,stdin);
        fflush(stdout);  
        
        SetMessageToOutputList(x);
        SenderSignalMessage(); // signal Sender to send
            
            
    }while( strcmp(x,"!\n"));

    ShutdownSignalMessage(); // "!" typed to signal shutdown. signal the main thread to shut the threads down.
    
    return NULL;
}
void KeyboardInit(void)
{
    
    pthread_create(
        &threadKeyboard,         // PID (by pointer)
        NULL,                    // Attributes
        KeyboardThread,          // Function
        NULL);
}

void KeyboardShutdown(void)
{
    // Cancel thread
    pthread_cancel(threadKeyboard);

    // Waits for thread to finish
    pthread_join(threadKeyboard, NULL);
}