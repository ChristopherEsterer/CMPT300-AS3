// .h file for input.c, the keyboard input module

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_


void* KeyboardThread(void* unused); 
void KeyboardInit(void); // Initialize thread call
void KeyboardShutdown(void); // Keyboard Shutdown

#endif
 