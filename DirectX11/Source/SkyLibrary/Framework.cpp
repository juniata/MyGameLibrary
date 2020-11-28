#include	"SkyLibrary.h"
#include	"Games\SceneMain\SceneMain.h"
#include	<mmsystem.h>
#include	<stdio.h>
#pragma comment(lib,"winmm.lib")


//-----------------------------------------------------------------------------------------
//
//	送られたメッセージを元に様々な処理を行う
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM	lparam)
{
	switch (msg) {
	case WM_DESTROY:
		//	メッセージループを抜けます
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROYを発行する
		break;
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd); // WM_DESTROYを発行する
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

// メモリリークチェック
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
//#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_CLIENTBLOCK

//	debug時のみコンソール画面を開くための変数と関数---------
FILE*		g_pConsole = nullptr;

//	コンソール画面を開く
void OpneConsoleWindow()
{
	AllocConsole();
	freopen_s(&g_pConsole, "CON", "w", stdout);
}

//	コンソール画面を閉じる
void CloseConsoleWindow()
{
	if (g_pConsole) {
		fclose(g_pConsole);
		FreeConsole();
	}
}

#define DEBUG_OPNE_CONSOLE	OpneConsoleWindow();
#define DEBUG_CLOSE_CONSOLE	CloseConsoleWindow();

#else

#define DEBUG_OPNE_CONSOLE
#define DEBUG_CLOSE_CONSOLE

#endif

//-----------------------------------------------------------------------------------------
//
//	メンバー変数を初期化
//
//-----------------------------------------------------------------------------------------
FrameWork::FrameWork() :
	m_pAppName(nullptr),
	m_hInstance(NULL),
	m_hWnd(NULL)
{
	ZeroMemory(&m_fps, sizeof(m_fps));
	m_fps.startTime = timeGetTime();
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウクラスを削除し、メモリを解放
//
//-----------------------------------------------------------------------------------------
FrameWork::~FrameWork()
{
	// SkyLibraryを解放
	SceneManager::Release();

	//	DX_Libraryを解放する
	DX_System::Destroy();
	DX_Debug::GetInstance()->ReportLiveDeviceObjects("DX_System::Destroy() after");
	DX_Debug::Destroy();

	UnregisterClass(m_pAppName, m_hInstance);
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウを初期化する
//
//-----------------------------------------------------------------------------------------
bool FrameWork::Initialize(const unsigned int width, const unsigned int height)
{
#if defined(DEBUG) || defined(_DEBUG)
	// メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif 

	DX_System::Create();
	DX_System* pSystem = DX_System::GetInstance();

	//	ウィンドウを作成
	if (!CreateAppWindow("DirectX11", width, height)) {
		return false;
	}

	//	DirectXの初期化
	if (!pSystem->Initialize(m_hWnd, m_hInstance, width, height)) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  シーンを走らせる(更新＆描画)
//
//-----------------------------------------------------------------------------------------
void FrameWork::Run()
{
	MSG msg = { NULL };

	//	スワップチェインを取得
	IDXGISwapChain* pSwapChain = DX_System::GetInstance()->GetSwapChain();

	//	現在のシーンを取得
	SceneManager::SetBootScene(new SceneMain());
	
	//	ループ処理
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//	FPSを更新
			FPSUpdate();

			// シーンの更新
			SceneManager::Update(m_hWnd, msg.message, msg.wParam);

			// シーンの描画
			SceneManager::Render();

			// ゲームエンドか？
			if (SceneManager::IsGameEnd()) {
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウを作成する
//
//-----------------------------------------------------------------------------------------
bool FrameWork::CreateAppWindow(char* pAppName, const unsigned int width, const unsigned int height)
{
	//	アプリケーション名を保存
	m_pAppName = pAppName;

	//	hinstanceを取得
	m_hInstance = GetModuleHandle(NULL);

	// windowクラスを設定
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BACKUP_SPARSE_BLOCK);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_pAppName;

	//	クラスを登録する5
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "RegisterClass() failed", "Error", MB_OK);
		return false;
	}

	//	ウィンドウのスタイルを設定
	int style = WS_OVERLAPPEDWINDOW | WS_POPUP;

	// 画面の中心に描画されるようにする
	RECT rect;
	SetRect(&rect, 0, 0, DX::CAST::I(width), DX::CAST::I(height));
	AdjustWindowRect(&rect, style, TRUE);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	// ウィンドウの画面サイズを取得する
	RECT windowsSize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowsSize, 0);

	//　中心の描画されるようにするため、描画開始点を算出する
	const unsigned int centerPosX = static_cast<unsigned int>(windowsSize.right) / 2;
	const unsigned int centerPosY = static_cast<unsigned int>(windowsSize.bottom) / 2;

	const unsigned int halfWidth = rect.right / 2;
	const unsigned int halfHeight = rect.bottom / 2;

	const unsigned int leftPos = centerPosX - halfWidth;
	const unsigned int topPos = centerPosY - halfHeight;

	//	windowを作成する
	m_hWnd = CreateWindow(
		m_pAppName,
		m_pAppName,
		style,
		leftPos,
		topPos,
		rect.right,
		rect.bottom,
		NULL,
		NULL,
		m_hInstance,
		NULL);

	if (m_hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}

	//
	//	HMENU hMenu = CreateMenu();
	//	//	メニューを作成
	//#if defined(DEBUG) || defined(_DEBUG)
	//	HMENU renderFileMenu = CreateMenu();
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_SOLID, "ソリッド");
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_WIRE_FRAME, "ワイヤーフレーム");
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderFileMenu, "描画方法");
	//	SetMenu(m_hWnd, hMenu);
	//#endif
	//	HMENU renderSizeFileMenu = CreateMenu();
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderSizeFileMenu, MF_STRING, FILE_MENU_RENDER_WINDOW_MODE, "ウィンドウモード");
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderSizeFileMenu, MF_STRING, FILE_MENU_RENDER_FULL_SCREEN, "フルスクリーンモード");
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderSizeFileMenu, "ウィンドウサイズ");
	//
	//	SetMenu(m_hWnd, hMenu);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	FPSを更新する
//
//-----------------------------------------------------------------------------------------
void FrameWork::FPSUpdate()
{
	m_fps.count++;

	if (timeGetTime() >= (m_fps.startTime + 1000)) {
		m_fps.fps = m_fps.count;
		m_fps.count = 0;

		m_fps.startTime = timeGetTime();
	}

	m_fps.deltaTime = (1.0f / DX::CAST::F(m_fps.fps));
}
