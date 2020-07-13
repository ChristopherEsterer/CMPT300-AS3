// .h file for the protected list ADT

// mutex call from .c file 
// static pthread_mutex_t listLockMutex

// Function declaration
char* GetMessageFromInputList(void); // get a Message from the list, wraps the list read in mutexes
char* GetMessageFromOutputList(void);
void SetMessageToInputList(char* msg); // adds a message to the list, protects the list push call.
void SetMessageToOutputList(char* msg);

void TestPrintMsg(char* msg); // simple printf function
void InitLists(void); // allocate list memory?