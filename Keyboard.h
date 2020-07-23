// .h file for input.c, the keyboard input module

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_


void* KeyboardThread(void* unused); // *** to do
void KeyboardInit(void); // Initialize thread call
void KeyboardShutdown(void); // Keyboard Shutdown
//void KeyboardSignalMessage(void); // Mutex lock for the condition variable. This is the external signal call

#endif
 