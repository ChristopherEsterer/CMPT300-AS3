// UDP Reveive module
// Some code supplied from workshops
#include "ProtectedList.h" //include the protected list module
#include "UDPReceive.h"
#include "ChrisTestingMain.h" //for shutdown signal from sender
#include "Print.h"
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


char* portRNumber;
char* addressNumber;



static pthread_t threadPID;
static int socketDescriptor;

struct addrinfo *receiveInfo;
struct addrinfo hints;
//struct sockaddr_in sin;

void* ReceiveThread(void* unused)
{ 
    
    //struct addrinfo *p;
    

	// Address
    int addrError = 0;
	

    //struct addrinfo hints; //no idea ** ?
	//memset(&sin, 0, sizeof(sin));
	//sin.sin_family = AF_INET;                       //Connection may be from network
	//.sin_addr.s_addr = htonl(INADDR_ANY);        //Host to Network long
	//sin.sin_port = htons(atoi(portRNumber));        //Host to Network short 
	
    memset(&hints, 0, sizeof hints);    // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;          // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;     // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;        // fill in my IP for me
    
    

    //if ((addrError = getaddrinfo(addressNumber, portRNumber, &hints, &receiveInfo)) != 0) {
    if ((addrError = getaddrinfo(NULL, portRNumber, &hints, &receiveInfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addrError));
        
    }
    
    //loop through all the results and make a socket
   // for(p = receiveInfo; p != NULL; p = p->ai_next) {
        if ((socketDescriptor = socket(receiveInfo->ai_family, receiveInfo->ai_socktype, receiveInfo->ai_protocol)) == -1) {
            perror("talker: socket");
            //continue;
        }

        //break;
    //}

	//Create the socket for UDP
	//socketDescriptor = socket(receiveInfo->ai_family, SOCK_DGRAM, receiveInfo->ai_protocol); // this doesnt need?
    //socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);// this works
	
    if (socketDescriptor == -1){
        printf("socket Error Receive:(%d) \n", socketDescriptor);
    }

	//Bind the socket to the port (PORT) that we specify
    int bindError = 0;
    bindError = bind(socketDescriptor, (const struct sockaddr*) (receiveInfo->ai_addr) ,(size_t) receiveInfo->ai_addrlen);
    //bindError = bind(socketDescriptor, p->ai_addr ,p->ai_addrlen);
	if (bindError == -1){
    printf("Bind Error Receive:(%d) \n", bindError);
    //printf("Port Numb: (%s)\n",receiveInfo->ai_canonname);//ai_addr->sa_data);
    printf("RecInfo =: \n SD: %d \n adrlen %d \n data : %s \n", socketDescriptor, (int) receiveInfo->ai_addrlen, receiveInfo->ai_addr->sa_data );
    printf("name: %s \n ", receiveInfo->ai_canonname);
    printf("addrInfo : %s : %s \n" , addressNumber , portRNumber );
    }
    int recvError = 0;

	while (1) {
        
		//Get the data (blocking)
		//Will change sin (the address) to be the address of the client.
		//Note: sin passes information in and out of call!
		//struct sockaddr_in sinRemote;
		unsigned int receiveInfolen = sizeof((struct sockaddr *) &receiveInfo->ai_addr);
		char messageRx[MSG_MAX_LEN];
        recvError = recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0, (struct sockaddr *) &receiveInfo->ai_addr, &receiveInfolen);
        //recvError = recv(socketDescriptor, messageRx, MSG_MAX_LEN, 0);
		if (recvError == -1){
            
            printf("recvfrom error \n");
            ShutdownSignalMessage();
            return NULL;
        }

        
        if( !strcmp(messageRx,"!\n") ) // heck for shutdown (might need tweek)
        {
            printf("Receive: Shutdown!\n");
            ShutdownSignalMessage();
            return NULL;
        }
        
        SetMessageToInputList(messageRx);
        Printer_signalMessage();
	}
    //NOTE NEVER EXECUTES BECEAUSE THREAD IS CANCELLED
	return NULL;
}


void ReceiverInit(char* addr, char* portNumb)
{
    addressNumber = addr;
    portRNumber = portNumb;

    pthread_create(&threadPID ,NULL ,ReceiveThread ,NULL );
}



void ReceiverShutdown(void)
{   

    freeaddrinfo(receiveInfo); // to do
    //freeaddrinfo(&hints);
    //freeaddrinfo(&sin);
    //Cancel thread
    close(socketDescriptor);
    pthread_cancel(threadPID);

    //Waits for thread to finish
    pthread_join(threadPID, NULL);

    // Cleanup memory
    //freeaddrinfo(receiveInfo); // to do
}