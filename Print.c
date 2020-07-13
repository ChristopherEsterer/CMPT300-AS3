// this will be signaled by the UDPReceive when it has a new message to print.
// and keep printing as long as something is in the list
#include stdio // idk the call? ***
#include <Print.h>

main(){
receiveConVar_wait();
messageToPrint = inList.GetMessage()// this is protected from the protectedList ADT
printf(“%s /n”, messageToPrint);
}
