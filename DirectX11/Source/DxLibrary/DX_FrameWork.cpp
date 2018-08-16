#include	"DX_Library.h"
//#include	"../../Resource/icon/resource.h"
#include	"Games\SceneMain.h"
#include	<mmsystem.h>
#pragma comment(lib,"winmm.lib")

// メモリリークチェック
#if defined(DEBUG) || defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>
	//#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
	//#define new DEBUG_CLIENTBLOCK

#endif

//-----------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------
//	debug時のみコンソール画面を開くための変数と関数---------
#if defined(DEBUG) || defined(_DEBUG)
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
	if (g_pConsole){
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
DX_FrameWork::~DX_FrameWork()
{
	UnregisterClass(m_pAppName, m_hInstance);
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウを初期化する
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::Initialize()
{
	//	スクリーンサイズを設定
	DX_System::GetInstance()->SetWindowsSize(CREATE_WINDOW_SIZE::SCREEN_720p);

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

	MSG l_msg = { NULL };
	
	DX_System* pSystem = DX_System::GetInstance();
	ID3D11Device* pDevice = pSystem->GetDevice();
	ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

	//	スワップチェインを取得
	IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();

	// DX_Graphicsを取得
	DX_Graphics* pGraphics = DX_Graphics::GetInstance();

	//	現在のシーンを取得
	DX_SceneManager* pScenemanager = DX_SceneManager::GetInstance();
	pScenemanager->Initialize(pSystem, pDevice, pContext);

	//	ループ処理
	while (l_msg.message != WM_QUIT){
		if (PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}
		else{
			//	FPSを更新
			FPSUpdate();
			
			//	全キー更新
			DX_Input::Update(l_msg.message, l_msg.wParam);

			// シーンの更新
			pScenemanager->Update(pSystem, pDevice, pContext);

			//	描画開始
			pGraphics->BeginRender(pContext, pSwapChain);

			// シーンの描画
			pScenemanager->Render(pSystem, pDevice, pContext);

			//	描画終了
			pGraphics->EndRender(pSwapChain);
		}
	}
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

//-----------------------------------------------------------------------------------------
//
//	ウィンドウを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::CreateAppWindow(
	char* pAppName,
	const int x,
	const int y,
	const int width,
	const int height
	)
{
	//	アプリケーション名を保存
	m_pAppName = pAppName;

	//	hinstanceを取得
	m_hInstance = GetModuleHandle(NULL);

	// windowクラスを設定
	WNDCLASSEX l_wc;
	l_wc.cbSize			= sizeof(WNDCLASSEX);
	l_wc.style			= CS_HREDRAW | CS_VREDRAW;
	l_wc.lpfnWndProc	= WndProc;
	l_wc.cbClsExtra		= 0;
	l_wc.cbWndExtra		= 0;
	l_wc.hInstance		= m_hInstance;
	//l_wc.hIcon			= LoadIcon(m_hInstance, (LPCSTR)IDI_ICON1);
	l_wc.hIcon			= LoadIcon(m_hInstance, IDI_APPLICATION);
	l_wc.hIconSm		= l_wc.hIcon;
	l_wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	l_wc.hbrBackground	= (HBRUSH)GetStockObject(BACKUP_SPARSE_BLOCK);
	l_wc.lpszMenuName	= NULL;
	l_wc.lpszClassName	= m_pAppName;

	//	クラスを登録する
	if (!RegisterClassEx(&l_wc)){
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		return false;
	}

	//	ウィンドウのスタイルを設定
	int l_style = WS_OVERLAPPEDWINDOW;

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	//	windowを作成する
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		m_pAppName, 
		m_pAppName,
		l_style,
		x,
		y, 
		rect.right,
		rect.bottom,
		NULL, NULL, 
		m_hInstance,
		NULL);

	if (m_hWnd == NULL){
		MessageBox(NULL, "CreateWindowEx() failed", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}

	::ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);


	return true;
}

//-----------------------------------------------------------------------------------------
//
//	送られたメッセージを元に様々な処理を行う
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK DX_FrameWork::WndProc(
	HWND	hWnd, 
	UINT	msg,
	WPARAM	wparam,
	LPARAM	lparam
	)
{
	DX_System* pSystem = DX_System::GetInstance();

	switch (msg){
	case WM_DESTROY:
		//	メッセージループを抜けます
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROYを発行する
		break;
		
	case WM_SIZE:
		if (nullptr == pSystem->GetDevice() || wparam == SIZE_MINIMIZED) {
			break;
		}

		pSystem->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);
		pSystem->GetSwapChain()->ResizeBuffers(1, LOWORD(lparam), HIWORD(lparam), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		pSystem->InitBuckBuffer();
		break;

	case WM_KEYDOWN:

		switch (wparam){
		case VK_ESCAPE: 
			PostMessage(hWnd, WM_CLOSE, 0, 0);;
			break;

			//	F12を押した場合スクリーンモードを変更
		case VK_F12:
		/*	IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();
			if (nullptr != pSwapChain) {
				BOOL fullScreen = false;
				pSwapChain->GetFullscreenState(&fullScreen, NULL);
				pSwapChain->SetFullscreenState(!fullScreen, NULL);
			}*/

			break;
		}
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

//-----------------------------------------------------------------------------------------
//
//	FPSを更新する
//
//-----------------------------------------------------------------------------------------
void DX_FrameWork::FPSUpdate()
{
	m_fps.count++;

	if (timeGetTime() >= (m_fps.startTime + 1000)){
		m_fps.fps = m_fps.count;
		m_fps.count = 0;

		m_fps.startTime = timeGetTime();
	}

	m_fps.deltaTime = (1.0f / CAST_F(m_fps.fps));
	DX_Debug::GetInstance()->Printf("FPS[%d] deltaTime[%f]\n", m_fps.fps, m_fps.deltaTime);
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
	DX_FrameWork l_DX_FrameWork;

	//	フレームワークの初期化
	if (!l_DX_FrameWork.Initialize()){ return FALSE; }

	//	DirectXの初期化
	if (!DX_System::GetInstance()->InitD3D(l_DX_FrameWork.GetHwnd())){ return FALSE; }

	//	シーンを走らせる
	l_DX_FrameWork.Run();
	
	DX_SceneManager::Release();

	//	DX_Libraryを解放する
	DX_System::Release();


	//	デバッグ時のみコンソール画面を閉じる
	DEBUG_CLOSE_CONSOLE

	return TRUE;

}
