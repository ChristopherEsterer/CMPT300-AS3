// UDP Send module .c file.
// Coded by Chris Esterer and Gurjeet ***

// **code Coppied from drivefile

#include the UDP libraries and thread libraries // ***
#include the protectedlist.h library // ***
#include <UDPSend.h>

pthread thread; // declare a thread for this module
List* outList; // pointer to the out list
convar* sendConVar;

UDPInit(PortInfo)
{
// some code to init the UDP Port for Sending
// Then call the UDPSend Function?
}
UDPSend()
{
while(1)? // i think this needs to loop, maybe a 1 or maybe a run bool
{
sendConVar.wait();    // Wait until an signal from the keyboard Thread sendConVar
// When there is something to send from the output list (there should be because we unblocked with the signal.
msg = outList->GetMessageFromList() // ask the list for an item. The list will deal with mutexes.
// Send the message over UDP to the outPort
}
}
UDPThreadCreate() // passed in will be list, conVar, and port info
{
pthread_create(&thread, NULL, UDPInit, //Passed in stuff );
}