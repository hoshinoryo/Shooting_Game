// ==========================================================================================
// 
// File Name: key_logger.h
// Date: 2025/08/26
// Author: Gu Anyi
// Description: Key logger header file
// 
// ==========================================================================================
#ifndef KEY_LOGGER_H
#define KEY_LOGGER_H
#include "keyboard.h"

void KeyLogger_Initialize();

void KeyLogger_Update();
bool KeyLogger_IsPressed(Keyboard_Keys key);
bool KeyLogger_IsTrigger(Keyboard_Keys key); // âüÇµÇΩèuä‘ÇæÇØÇ≈true
bool KeyLogger_IsRelease(Keyboard_Keys key); // ó£ÇµÇΩèuä‘

void KeyLogger_Reset();

#endif // KEY_LOGGER_H