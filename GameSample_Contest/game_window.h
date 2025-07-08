/*==============================================================================

�@ �Q�[���E�B���h�E[game_window.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/12
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef GAME_WONDOW_H
#define GAME_WONDOW_H

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*-----------------------------------------------------
�E�B���h�E�v���V�[�W���@�v���g�^�C�v�錾
-------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND GameWindow_Create(HINSTANCE hInstance);


#endif // GAME_WINDOW_H