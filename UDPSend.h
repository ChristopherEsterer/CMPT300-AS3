//.h file for UDPSend module
//some code from Workshops


//Start background send thread
//void Receiver_init(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);
void SenderInit(char* addr , char* port);
void* SendThread(void* unused);
//Set the dynamic message -- Threadsafe
void SenderChangeDynamicMessage(char* txMessage);
void SenderSignalMessage(void);
//Stop background receive thread and cleanup
void SenderShutdown(void);

