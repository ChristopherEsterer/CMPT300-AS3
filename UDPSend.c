// UDP Reveive module
// Some code supplied from workshops
#include "ProtectedList.h" // include the protected list module
#include "UDPSend.h"

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

char* addressNumber;
char* portNumber;

static pthread_t threadPID;
static int socketDescriptor;
//static char* s_rxMessage;

static char* dynamicMessage; // not sure if needed

static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;


void* SendThread(void* unused)
{
    struct addrinfo *sendInfo, *p, hints;
	// Address
	//struct sockaddr_in sin;

	//memset(&sin, 0, sizeof(sin));
	//sin.sin_family = AF_INET;                   // Connection may be from network
	//sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	//sin.sin_port = htons(portNumber);                 // Host to Network short ** should all be changed to network to host as its reciveing
	
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = PF_INET;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int addrError = 0;
    if ((addrError = getaddrinfo(addressNumber, portNumber, &hints, &sendInfo)) != 0) {
        
        fprintf(stderr, "Send getaddrinfo error: %s\n", gai_strerror(addrError));
        
        //exit(1);
    }

    // loop through all the results and make a socket
    for(p = sendInfo; p != NULL; p = p->ai_next) {
        if ((socketDescriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
    }



    freeaddrinfo(sendInfo);



	// Create the socket for IPv4 UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
    //int bindError = 0;
	//bindError = bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	//if (bindError == -1){
    //    printf("Send bind error: (%d)\n",bindError);
    //   printf("Port Numb: (%d)\n",sin.sin_port);
    //}
    //printf("SBind Err: (%d)\n",bindError);
    
	while (1) {

        pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex); // wait condition


		// Send the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		//struct sockaddr_in sinRemote;
		//unsigned int sin_len = sizeof(sinRemote);

		static char* messageTx;
        messageTx = GetMessageFromOutputList(); // get output message from List
		int sendError = 0;
        
       // sendError = sendto(socketDescriptor,messageTx, MSG_MAX_LEN, 0, sendInfo->ai_addr, sizeof(sendInfo->ai_addr));
        if (sendError == -1){
            printf("sendto error: %s (%d)\n", messageTx, sendError ); 
        }

        if ((sendError = sendto(socketDescriptor, messageTx, strlen(messageTx), 0, p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }
        //printf("msg sent: %s (%d)\n", messageTx, sendError );
	}
    // NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


void SenderInit(char* addr , char* portNum)
{
    dynamicMessage = malloc(DYNAMIC_LEN);
    
    portNumber = portNum;
    addressNumber = addr;
    //InitLists(); // Initalized the lists for memory allocation **shuold both lists be initialized once? So theyll be passed to UDPSend.c? Or better to initalize seperately?

   // s_rxMessage = rxMessage;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        SendThread,      // Function
        NULL);
}

void SenderSignalMessage(void) // External Signal Call to tell the Sender to send. Protected
{
    pthread_mutex_lock(&s_syncOkToSendMutex);
    {
        pthread_cond_signal(&s_syncOkToSendCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToSendMutex);
}




void SenderShutdown(void)
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