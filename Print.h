// .h File for Print.c
// create as thread for the printing module
#ifndef _PRINTER_H_
#define _PRINTER_H_


void PrintThreadCreate(void); // Initialize Print threat
void Printer_init(void); // Initialize thread call
void PrinterShutdown(void); // Printer Shutdown
void Printer_signalMessage(void); // Mutex lock for the condition variable. This is the external signal call

#endif