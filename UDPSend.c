// UDP Reveive module
// Some code supplied from workshops
#include "ProtectedList.h" // include the protected list module
#include "UDPSend.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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
#define BUF_SIZE 500



//static pthread_mutex_t dynamicMsgMutex = PTHREAD_MUTEX_INITIALIZER; // implemented in the Protected list

int portNumber = 0;

static pthread_t threadPID;
static int socketDescriptor;
//static char* s_rxMessage;

static char* dynamicMessage; // not sure if needed

static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;


void* SendThread(void* unused)
{
    // Dynamically allocate a message
    /*
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strcpy(dynamicMessage, "Dynamic!");
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
    */


    //setupfor network connection
    //ie:get adddrinfo setup
    struct addrinfo hints;
           struct addrinfo *result, *rp;
           int sfd, s;
           struct sockaddr_storage peer_addr;
           socklen_t peer_addr_len;
           ssize_t nread;
           char buf[BUF_SIZE];

           if (argc != 2) {
               fprintf(stderr, "Usage: %s port\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           memset(&hints, 0, sizeof(struct addrinfo));
           hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
           hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
           hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
           hints.ai_protocol = 0;          /* Any protocol */
           hints.ai_canonname = NULL;
           hints.ai_addr = NULL;
           hints.ai_next = NULL;

           s = getaddrinfo(NULL, argv[1], &hints, &result);
           if (s != 0) {
               fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
               exit(EXIT_FAILURE);
           }

           /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully bind(2).
              If socket(2) (or bind(2)) fails, we (close the socket
              and) try the next address. */

           for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                       rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
                   break;                  /* Success */

               close(sfd);
           }

           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not bind\n");
               exit(EXIT_FAILURE);
           }

           freeaddrinfo(result);           /* No longer needed */


	// Address
    struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(portNumber);                 // Host to Network short ** should all be changed to network to host as its reciveing
	
	// Create the socket for IPv4 UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
    int bindError =0;
	//bindError = bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	printf("SBind Err: (%d)\n",bindError);
    printf("Sport Numb: (%d)\n",sin.sin_port);
	while (1) {

        pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex); // wait condition


		// Send the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);

		static char* messageTx;
        messageTx = GetMessageFromOutputList(); // get output message from List
		int sendError = 0;
        sendError = sendto(socketDescriptor,messageTx, MSG_MAX_LEN, 0,(struct sockaddr *) &sin, sin_len);

        printf("msg sent: %s (%d)\n", messageTx, sendError );
	}
    // NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


void SenderInit(int portNum)
{
    dynamicMessage = malloc(DYNAMIC_LEN);
    
    portNumber = portNum;
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

void SenderChangeDynamicMessage(char* newDynamic)
{
    //pthread_mutex_lock(&dynamicMsgMutex);
    //{
    //    strncpy(dynamicMessage, newDynamic, DYNAMIC_LEN);
    //}
    //pthread_mutex_unlock(&dynamicMsgMutex);
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