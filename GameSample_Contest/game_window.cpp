/*==============================================================================

�@ �Q�[���E�B���h�E[game_window.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/05/12
--------------------------------------------------------------------------------

==============================================================================*/
#include <algorithm>
#include "game_window.h"
#include "keyboard.h"
#include "mouse.h"

/*--------------------------------------------------
�E�B���h�E���
----------------------------------------------------*/
static constexpr char WINDOW_CLASS[] = "Game Windowy"; // ���C���E�B���h�E�N���X��
static constexpr char TITLE[] = "My Window"; // �^�C�g���o�[�̃e�L�X�g�A���Ƃ͉ۑ薼�Ƃ��Q�[�����Ƃ�
constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 900; // ����͕`����͈́A���j���[�ƃo�[���܂܂�Ȃ�


HWND GameWindow_Create(HINSTANCE hInstance)
{
    /* �E�B���h�E�N���X�̓o�^ */
    WNDCLASSEX wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc; // �֐��̃|�C���^
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // hInstance���K�v������Amain�ň����Ƃ��ēn��
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName = nullptr; // ���j���[�͍��Ȃ�
    wcex.lpszClassName = WINDOW_CLASS;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    /* ���C���E�B���h�E�̍쐬 */

    RECT window_rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // �����R�[�h�͏o�����Ȃ��悤��

    DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

    AdjustWindowRect(&window_rect, style, FALSE);

    const int WINDOW_WIDTH = window_rect.right - window_rect.left;
    const int WINDOW_HEIGHT = window_rect.bottom - window_rect.top; // �E�B���h�E�̑傫��

    // �f�X�N�g�b�v�̃T�C�Y���擾����
    // �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
    int desktop_width = GetSystemMetrics(SM_CXSCREEN);
    int desktop_height = GetSystemMetrics(SM_CYSCREEN);

    // �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
    // ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
    const int WINDOW_X = std::max((desktop_width - WINDOW_WIDTH) / 2, 0);
    const int WINDOW_Y = std::max((desktop_height - WINDOW_HEIGHT) / 2, 0);

    HWND hWnd = CreateWindow(
        WINDOW_CLASS,
        TITLE,
        style,
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    return hWnd;
}

/*-------------------------------------------------------------
                 �E�B���h�E�v���V�[�W��
---------------------------------------------------------------*/


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATEAPP:
        Keyboard_ProcessMessage(message, wParam, lParam);
        Mouse_ProcessMessage(message, wParam, lParam);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) // escape�L�[����������I��
        {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
         Keyboard_ProcessMessage(message, wParam, lParam);
         break;
    case WM_CLOSE:
        if (MessageBox(hWnd, "�I�����Ă�낵���ł����H", "�I��", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
        {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
        PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M�A�����ɂȂ�����ɃE�B���h�E�����ƁA�v���O�����S�̂��I���ł��Ȃ��Ȃ�\��������܂�
        break;
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        Mouse_ProcessMessage(message, wParam, lParam);
        break;
    default:
        // �ʏ�̃��b�Z�[�W�����͂��̊֐��ɔC����
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
