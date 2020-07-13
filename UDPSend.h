//pseudocode from drive
/* 
// .h file for UDPSend Module

// Function Declarations
void UDPSend(void); // UDPSend function ***
void UDPThreadCreate(/* pList, conVar, portinfo*//*); // Creates thread for UDP send ***
 */

// .h file for UDPSend module
// some code from Workshops
#ifndef _SENDER_H_
#define _SENDER_H_

// Start background send thread
// void Receiver_init(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);
void Sender_init(char* txMessage);

// Set the dynamic message -- Threadsafe
void Sender_changeDynamicMessage(char* txMessage);

// Stop background receive thread and cleanup
void Sender_shutdown(void);

#endif
