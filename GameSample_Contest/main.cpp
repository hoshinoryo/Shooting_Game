#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>
#include <DirectXMath.h>
//#include "debug_ostream.h"
#include "game_window.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "sprite_anim.h"
#include "debug_text.h"
#include "system_timer.h"
#include "polygon.h"
#include "mouse.h"
#include "key_logger.h"
#include "game.h"
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

using namespace DirectX;

/*--------------------------------------------------
	���C���A�e���v���[�g
----------------------------------------------------*/


int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// DPI�X�P�[�����O
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HWND hWnd = GameWindow_Create(hInstance);

	// Initialization
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd); // �����̓E�B���h�E

	Direct3D_Initialize(hWnd); // Direct3D�̏������A�K����Ԑ擪
	Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	SpriteAnim_Initialize();
	Polygon_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	Game_Initialize();


	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(),
		Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 16.0f);

	Mouse_SetVisible(true);

	ShowWindow(hWnd, nCmdShow); // �E�B���h�E�\��
	UpdateWindow(hWnd); // �E�B���h�E�̒����X�V����

	// fps�E���s�t���[�����x�v���p
	double exec_last_time = SystemTimer_GetTime(); // �O�񏈗��������Ԃ��L�^
	double fps_last_time = exec_last_time;         // fps�v���J�n�̊����
	double current_time = 0.0;                     // ���ݎ����i���t���[���p�j
	ULONG frame_count = 0;                         // �t���[�����J�E���g�p
	double fps = 0.0;                              // fps�l��ۑ�

	MSG msg;
	
	do
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// �E�B���h�E���b�Z�[�W�����Ă�����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // �Q�[������
		{
			// fps�v������

			current_time = SystemTimer_GetTime(); // ���݂̃V�X�e���������擾
			double elapsed_time = current_time - fps_last_time; // fps�v���̂��߂̌o�ߎ��Ԃ��v�Z

			if (elapsed_time >= 1.0)
			{
				fps = frame_count / elapsed_time;
				fps_last_time = current_time;
				frame_count = 0;
			}

			elapsed_time = current_time - exec_last_time;
			if (elapsed_time >= (1.0 / 60.0))
			{
				exec_last_time = current_time;

				// �Q�[���̍X�V
				KeyLogger_Update(); // �L�[�̏�Ԃ��X�V

				Game_Update(elapsed_time);

				SpriteAnim_Update(elapsed_time);
				
				// �Q�[���̕`��
				Direct3D_Clear(); // Clear the screen

				Sprite_Begin();

				Game_Draw();

				// �t���[���v�����\��
#if defined(DEBUG) || defined(_DEBUG)

				std::stringstream ss;
				ss << "fps: " << fps << std::endl;
				dt.SetText(ss.str().c_str(), { 0.0f, 0.0f, 1.0f, 1.0f });

				dt.Draw();
				dt.Clear();
#endif

				Direct3D_Present();

				frame_count++; //�t���[���v���p�A�C��t����
			}

		}

	} while (msg.message != WM_QUIT);
	
	Game_Finalize();

	Polygon_Finalize();
	SpriteAnim_Finalize();
	Texture_Finalize();
	Sprite_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();
	Mouse_Finalize();

	return (int)msg.wParam;

	return 0;
}