// UDP Reveive module
// Some code supplied from workshops
// Coded by Chris Esterer

#include "ProtectedList.h" //include the protected list module
#include "UDPSend.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			//for strncmp()
#include <unistd.h>			//for close()
#include <pthread.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>


#define DYNAMIC_LEN 128
#define MSG_MAX_LEN 1024
#define PORT        22110

char* addressNumber;
char* portNumber;

static pthread_t threadPID;
static int socketDescriptor;

static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;


void* SendThread(void* unused)
{
    struct addrinfo *sendInfo, *p, hints;
	//Address
	
    memset(&hints, 0, sizeof hints);    //make sure the struct is empty
    hints.ai_family = PF_INET;          //don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;     //TCP stream sockets
    hints.ai_flags = AI_PASSIVE;        //fill in my IP for me

    int addrError = 0;
    if ((addrError = getaddrinfo(addressNumber, portNumber, &hints, &sendInfo)) != 0) {
        
        fprintf(stderr, "Send getaddrinfo error: %s\n", gai_strerror(addrError));
        
        //exit(1);
    }

    //loop through all the results and make a socket
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



	//Create the socket for IPv4 UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    
	while (1) {

        pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex); //wait condition. Will get triggered externaly (by keyboard)

        static char* messageTx;
        messageTx = GetMessageFromOutputList(); //get output message from List
        int sendError = 0;
        sendError = strlen(messageTx);
        if ((sendError = sendto(socketDescriptor, messageTx, strlen(messageTx), 0, p->ai_addr, p->ai_addrlen)) == -1) {
            printf("sendto error");
    
        exit(1);
        }
        
	}
    //NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


void SenderInit(char* addr , char* portNum)
{

    portNumber = portNum;
    addressNumber = addr;
    
    pthread_create(
        &threadPID,         //PID (by pointer)
        NULL,               //Attributes
        SendThread,         //Function
        NULL);
}

void SenderSignalMessage(void) //External Signal Call to tell the Sender to send. Protected
{
    pthread_mutex_lock(&s_syncOkToSendMutex);
    {
        pthread_cond_signal(&s_syncOkToSendCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToSendMutex);
}




void SenderShutdown(void)
{
    close(socketDescriptor);
    //Cancel thread
    pthread_cancel(threadPID);

    //Waits for thread to finish
    pthread_join(threadPID, NULL);

    //Cleanup memory
 
}