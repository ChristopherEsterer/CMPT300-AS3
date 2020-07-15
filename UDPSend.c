// UDP Send module .c file.
// Coded by Chris Esterer and Gurjeet ***

// **code Coppied from drivefile

#include "ProtectedList.h" // include the protected list module
#include <UDPSend.h>

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

static pthread_t threadPID;
static int socketDescriptor;
static char* s_txMessage;

void* sendThread(void* unused)
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
		static char messageTx[MSG_MAX_LEN];
		recvfrom(socketDescriptor,
			messageTx, MSG_MAX_LEN, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

        // Do something amazing with the received message!
       /* *** changed to use the protected list
        pthread_mutex_lock(&dynamicMsgMutex);
        {
            printf("%s >> %s: %s\n", dynamicMessage, s_rxMessage, messageRx);
        }
        pthread_mutex_unlock(&dynamicMsgMutex);
        */

        //printf("msg = %s \n", messageRx);
        SetMessageToInputList(messageTx);
        char* tempMsg = GetMessageFromInputList();
        printf("msg from List = %s \n", tempMsg );
	}
    // NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


}
UDPThreadCreate() // passed in will be list, conVar, and port info
{
pthread_create(&thread, NULL, UDPInit, //Passed in stuff );
}