// A wrapper to make the list from AS1 protected with mutexes
// This will be a protected wrapper for the list.
// almost like a C++ class
// the list callfunctions need to be verified

#include mutexes and list stuff.
//#include <pthread.h> // for mutexes
#include <ProtectedList.h>

mutex listMutex; // the list will have a mutex that it will lock and unlock itself. this will prevent one thread from calling Get() while another thread calls Set().

static list thisList; // a list for this ListWrapper

char* GetMessageFromList()
{
listMutex_lock()
tempMsg = thisList_Append();
//? whatever gets the item from the front of the list
listMutex_unlock()
return tempMsg;
// not sure if this needs to be static. We don’t want to send a pointer but a copy of the msg data.
}

void SetMessageToList(char* msg)
{
    listMutex_lock()
    thisList_Push(msg) // push a the message onto the end of the list
    listMutec_unlock()
}
