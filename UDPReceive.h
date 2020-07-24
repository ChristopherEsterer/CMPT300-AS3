// .h file for UDPReceive module
// some code from Workshops

// Start background receive thread
// void ReceiverInit(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);
void* ReceiveThread(void* unused);
void ReceiverInit(char* addr, char* port);

// Set the dynamic message -- Threadsafe
void Receiver_changeDynamicMessage(char* rxMessage);

// Stop background receive thread and cleanup
void ReceiverShutdown(void);

