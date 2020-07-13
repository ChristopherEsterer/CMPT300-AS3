// UDP Reveive module
// Some code supplied from workshops
#include "ProtectedList.h" // include the protected list module
#include "UDPReceive.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <signal.h>

#define DYNAMIC_LEN 128
#define MSG_MAX_LEN 1024
#define PORT        22110

//static pthread_mutex_t dynamicMsgMutex = PTHREAD_MUTEX_INITIALIZER; // implemented in the Protected list

static pthread_t threadPID;
static int socketDescriptor;
static char* s_rxMessage;

static char* dynamicMessage; // not sure if needed

void* receiveThread(void* unused)
{
    // Dynamically allocate a message
    /*
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strcpy(dynamicMessage, "Dynamic!");
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
    */

	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short
	
	// Create the socket for UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	
	while (1) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[MSG_MAX_LEN];
		recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

        // Do something amazing with the received message!
       /* *** changed to use the protected list
        pthread_mutex_lock(&dynamicMsgMutex);
        {
            printf("%s >> %s: %s\n", dynamicMessage, s_rxMessage, messageRx);
        }
        pthread_mutex_unlock(&dynamicMsgMutex);
        */

       SetMessageToInputList(messageRx);

	}
    // NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


void Receiver_init(char* rxMessage)
{
    dynamicMessage = malloc(DYNAMIC_LEN);

    s_rxMessage = rxMessage;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        receiveThread,      // Function
        NULL);
}

void Receiver_changeDynamicMessage(char* newDynamic)
{
    //pthread_mutex_lock(&dynamicMsgMutex);
    //{
    //    strncpy(dynamicMessage, newDynamic, DYNAMIC_LEN);
    //}
    //pthread_mutex_unlock(&dynamicMsgMutex);
}


void Receiver_shutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);

    // Waits for thread to finish
    pthread_join(threadPID, NULL);

    // Cleanup memory
    /*
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        free(dynamicMessage);
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
    */
}