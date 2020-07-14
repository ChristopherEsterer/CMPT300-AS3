// this will be signaled by the UDPReceive when it has a new message to print.
// and keep printing as long as something is in the list
#include stdio // idk the call? ***
#include <Print.h>
#include <pthread.h>
#include <signal.h>

void* printerThread(void* unused){
/* *** sudo
    while(1) {
        receiveConVar_wait();
    }
    receiveConVar_wait();
    messageToPrint = inList.GetMessage()// this is protected from the protectedList ADT
    printf(“%s /n”, messageToPrint);
*/
}

void Printer_init(char* rxMessage)
{
    dynamicMessage = malloc(DYNAMIC_LEN);
    
    InitLists(); // Initalized the lists for memory allocation

    s_rxMessage = rxMessage;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        receiveThread,      // Function
        NULL);
}

void Printer_shutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);

    // Waits for thread to finish
    pthread_join(threadPID, NULL);

    // Cleanup memory
  
}