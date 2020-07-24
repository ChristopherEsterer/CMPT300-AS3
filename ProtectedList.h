// .h file for the protected list ADT

// mutex call from .c file 
// static pthread_mutex_t listLockMutex

// Function declaration
char* GetMessageFromInputList(void); // get a Message from the list, wraps the list read in mutexes
void GetMessageFromInputList2(char*);
char* GetMessageFromOutputList(void);
void SetMessageToInputList(char* msg); // adds a message to the list, protects the list push call.
void SetMessageToOutputList(char* msg);

int GetListSize(void);

void SignalPrintMsg(void); //
void ListInit(void); // allocate list memory
void ListShutdown(void);