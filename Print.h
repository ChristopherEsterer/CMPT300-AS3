// .h File for Print.c
// create as thread for the printing module
#ifndef _PRINTER_H_
#define _PRINTER_H_


void PrintThreadCreate(void); // *** to do
void Printer_init(void); // Initialize thread call
void Printer_shutdown(void); // Printer Shutdown
void Printer_signalMessage(void); // Mutex lock for the condition variable.

#endif