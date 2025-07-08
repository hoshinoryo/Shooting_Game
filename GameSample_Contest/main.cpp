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
	メイン、テンプレート
----------------------------------------------------*/


int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// DPIスケーリング
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HWND hWnd = GameWindow_Create(hInstance);

	// Initialization
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd); // 引数はウィンドウ

	Direct3D_Initialize(hWnd); // Direct3Dの初期化、必ず一番先頭
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

	ShowWindow(hWnd, nCmdShow); // ウィンドウ表示
	UpdateWindow(hWnd); // ウィンドウの中を更新する

	// fps・実行フレーム速度計測用
	double exec_last_time = SystemTimer_GetTime(); // 前回処理した時間を記録
	double fps_last_time = exec_last_time;         // fps計測開始の基準時間
	double current_time = 0.0;                     // 現在時刻（毎フレーム用）
	ULONG frame_count = 0;                         // フレーム数カウント用
	double fps = 0.0;                              // fps値を保存

	MSG msg;
	
	do
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// ウィンドウメッセージが来ていたら
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // ゲーム処理
		{
			// fps計測処理

			current_time = SystemTimer_GetTime(); // 現在のシステム時刻を取得
			double elapsed_time = current_time - fps_last_time; // fps計測のための経過時間を計算

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

				// ゲームの更新
				KeyLogger_Update(); // キーの状態を更新

				Game_Update(elapsed_time);

				SpriteAnim_Update(elapsed_time);
				
				// ゲームの描画
				Direct3D_Clear(); // Clear the screen

				Sprite_Begin();

				Game_Draw();

				// フレーム計測数表示
#if defined(DEBUG) || defined(_DEBUG)

				std::stringstream ss;
				ss << "fps: " << fps << std::endl;
				dt.SetText(ss.str().c_str(), { 0.0f, 0.0f, 1.0f, 1.0f });

				dt.Draw();
				dt.Clear();
#endif

				Direct3D_Present();

				frame_count++; //フレーム計測用、気を付けて
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