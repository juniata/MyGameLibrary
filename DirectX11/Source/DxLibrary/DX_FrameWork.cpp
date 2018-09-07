#include	"DX_Library.h"
//#include	"../../Resource/icon/resource.h"
#include	"Games\SceneTitle.h"
#include	"Games\SceneMain.h"
#include	<mmsystem.h>
#include	<stdio.h>
#pragma comment(lib,"winmm.lib")

#define FILE_MENU_RENDER_SOLID 1
#define FILE_MENU_RENDER_WIRE_FRAME 2

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
//	前方宣言
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND	hWnd, UINT	msg, WPARAM	wparam, LPARAM	lparam);

//-----------------------------------------------------------------------------------------
//
//	static変数
//
//-----------------------------------------------------------------------------------------
DX_FrameWork* DX_FrameWork::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	マクロ定義
//
//-----------------------------------------------------------------------------------------
#define SCENE_INITIALIZE	1
#define SCENE_UPDATE		2
#define SCENE_RENDER		3

//-----------------------------------------------------------------------------------------
//
//	メンバー変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_FrameWork::DX_FrameWork() :
m_pAppName(nullptr),
m_hInstance(NULL),
m_hWnd(NULL),
m_bResize(false),
m_lParam(0)
{
	ZeroMemory(&m_fps, sizeof(m_fps));
	m_fps.startTime = timeGetTime();
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウクラスを削除し、メモリを解放
//
//-----------------------------------------------------------------------------------------
DX_FrameWork::~DX_FrameWork()
{
	UnregisterClass(m_pAppName, m_hInstance);
}
//-----------------------------------------------------------------------------------------
//
//  自身のインスタンスを取得する
//
//-----------------------------------------------------------------------------------------
DX_FrameWork* DX_FrameWork::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_FrameWork();
	}

	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//  自身のインスタンスを解放する
//
//-----------------------------------------------------------------------------------------
void DX_FrameWork::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウを初期化する
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::Initialize()
{
	//	スクリーンサイズを設定
	DX_System::GetInstance()->SetWindowsSize(1280, 720);

	//	ウィンドウを作成
	if (!CreateAppWindow("DirectX11", 0, 0, DX_System::GetWindowWidth(), DX_System::GetWindowHeight())){
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  シーンを走らせる(更新＆描画)
//
//-----------------------------------------------------------------------------------------
void DX_FrameWork::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
	// メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif 

	MSG msg = { NULL };
	
	DX_System* pSystem	= DX_System::GetInstance();
	//	スワップチェインを取得
	IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();

	// グラフィックスを取得する
	DX_Graphics*	pGraphics = DX_Graphics::GetInstance();

	//	現在のシーンを取得
	DX_SceneManager* pSceneManager = DX_SceneManager::GetInstance();
	pSceneManager->SetStartScene(new SceneTitle());

	//	ループ処理
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			// 更新前にリサイズを行う
			if (m_bResize) {
				if (false == pSystem->BufferResize(LOWORD(m_lParam), HIWORD(m_lParam))) {
					break;
				}
			}

			//	FPSを更新
			FPSUpdate();
		
			// シーンの更新
			pSceneManager->Update(m_hWnd, msg.message, msg.wParam);

			// シーンの描画
			pSceneManager->Render(pGraphics, pSwapChain);

			// ゲームエンドか？
			if (pSceneManager->IsGameEnd()) {
				break;
			}

			// リサイズフラグを戻す
			m_bResize = false;
		}
	}
	
	// シーンマネージャーの開放
	DX_SceneManager::Release();
}

//------------------------------------------------------------------------------
//
//  アプリケーション名を取得する
//
//------------------------------------------------------------------------------
char* DX_FrameWork::GetAppName()const
{
	return m_pAppName;
}

//------------------------------------------------------------------------------
//
//  ウィンドウハンドルを取得
//
//------------------------------------------------------------------------------
HWND DX_FrameWork::GetHwnd()const
{
	return m_hWnd;
}

//------------------------------------------------------------------------------
//
//	インスタンスのハンドルを取得
//
//------------------------------------------------------------------------------
HINSTANCE DX_FrameWork::GetHinstance()const
{
	return m_hInstance;
}

//------------------------------------------------------------------------------
//
//	バックバッファのリサイズを行う
//
//------------------------------------------------------------------------------
void DX_FrameWork::DoResize(LPARAM lparam)
{
	m_bResize = true;
	m_lParam = lparam;
}

//------------------------------------------------------------------------------
//
//	リサイズを行ったかどうか
//
//------------------------------------------------------------------------------
bool DX_FrameWork::IsResize() const
{
	return m_bResize;
}
//-----------------------------------------------------------------------------------------
//
//	ウィンドウを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::CreateAppWindow(char* pAppName, const int x, const int y, const int width, const int height)
{
	//	アプリケーション名を保存
	m_pAppName = pAppName;

	//	hinstanceを取得
	m_hInstance = GetModuleHandle(NULL);

	// windowクラスを設定
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	//wc.hIcon			= LoadIcon(m_hInstance, (LPCSTR)IDI_ICON1);
	wc.hIcon			= LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BACKUP_SPARSE_BLOCK);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_pAppName;

	//	クラスを登録する5
	if (!RegisterClass(&wc)){
		MessageBox(NULL, "RegisterClass() failed", "Error", MB_OK);
		return false;
	}

	//	ウィンドウのスタイルを設定
	int style = WS_OVERLAPPEDWINDOW;
	// 画面の中心に描画されるようにする
	RECT rect;
	SetRect(&rect, x, y, width, height);
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

	if (m_hWnd == NULL){
		MessageBox(NULL, "CreateWindow() failed", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}
	

	//	メニューを作成
#if defined(DEBUG) || defined(_DEBUG)
	HMENU hMenu = CreateMenu();
	HMENU renderFileMenu = CreateMenu();
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_SOLID, "ソリッド");
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_WIRE_FRAME, "ワイヤーフレーム");
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderFileMenu, "描画方法");
	SetMenu(m_hWnd, hMenu);
#endif

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
void DX_FrameWork::FPSUpdate()
{
	m_fps.count++;

	if (timeGetTime() >= (m_fps.startTime + 1000)) {
		m_fps.fps = m_fps.count;
		m_fps.count = 0;

		m_fps.startTime = timeGetTime();
	}

	m_fps.deltaTime = (1.0f / CAST_F(m_fps.fps));
	DX_Debug::GetInstance()->Printf("FPS[%d] deltaTime[%f]\n", m_fps.fps, m_fps.deltaTime);
}

//-----------------------------------------------------------------------------------------
//
//	送られたメッセージを元に様々な処理を行う
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND	hWnd, UINT	msg, WPARAM	wparam, LPARAM	lparam)
{
	DX_FrameWork* pFramework	= DX_FrameWork::GetInstance();

	switch (msg){
	case WM_DESTROY:
		//	メッセージループを抜けます
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROYを発行する
		break;

	case WM_SIZE:
		pFramework->DoResize(lparam);
		break;

	case WM_KEYDOWN:

		switch (wparam){
			//	F12を押した場合スクリーンモードを変更
		case VK_F12:
/*
			IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();
			if (nullptr != pSwapChain) {
				BOOL fullScreen = false;
				pSwapChain->GetFullscreenState(&fullScreen, NULL);
				pSwapChain->SetFullscreenState(!fullScreen, NULL);
			}
*/
			break;
		}
		break;

	case WM_COMMAND:
		switch (wparam) {
#if defined(DEBUG) || defined(_DEBUG)
		case FILE_MENU_RENDER_SOLID:
			DX_RenderState::GetInstance()->SwitchSolidRS();
			break;
		case FILE_MENU_RENDER_WIRE_FRAME:
			DX_RenderState::GetInstance()->SwitchWireframeRS();
			break;
#endif
		case NULL:
			break;
		default:
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}


//-----------------------------------------------------------------------------------------
//
//	Entry Point WinMain
//
//-----------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE arg_hInst, HINSTANCE arg_hPrevInst, LPSTR arg_szStr, INT arg_iCmdShow)
{
	//	デバッグ時のみコンソール画面を開く
	DEBUG_OPNE_CONSOLE

	//	フレームワークを生成
	DX_FrameWork* pFramework = DX_FrameWork::GetInstance();

	//	フレームワークの初期化
	if (!pFramework->Initialize()) { return FALSE; }

	//	DirectXの初期化
	if (!DX_System::GetInstance()->InitD3D(pFramework->GetHwnd())) { return FALSE; }

	//	シーンを走らせる
	pFramework->Run();

	//	DX_Libraryを解放する
	DX_System::Release();

	// フレームワークの開放を行う
	DX_FrameWork::Release();

	//	デバッグ時のみコンソール画面を閉じる
	DEBUG_CLOSE_CONSOLE

	return TRUE;
}
