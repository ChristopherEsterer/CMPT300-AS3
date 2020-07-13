// .h file for the protected list ADT
#include <list.h> // the list files from AS1

// Function declaration
char* GetMessageFromList(void); // get a Message from the list, wraps the list read in mutexes
void SetMessageToList(char* msg); // adds a message to the list, protects the list push call.
