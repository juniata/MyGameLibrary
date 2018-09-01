#include	"DX_Library.h"
//#include	"../../Resource/icon/resource.h"
#include	"Games\SceneTitle.h"
#include	"Games\SceneMain.h"
#include	<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define FILE_MENU_RENDER_SOLID 1
#define FILE_MENU_RENDER_WIRE_FRAME 2

// ���������[�N�`�F�b�N
#if defined(DEBUG) || defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>
	//#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
	//#define new DEBUG_CLIENTBLOCK

//	debug���̂݃R���\�[����ʂ��J�����߂̕ϐ��Ɗ֐�---------
FILE*		g_pConsole = nullptr;

//	�R���\�[����ʂ��J��
void OpneConsoleWindow()
{
	AllocConsole();
	freopen_s(&g_pConsole, "CON", "w", stdout);
}

//	�R���\�[����ʂ����
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
//	�}�N����`
//
//-----------------------------------------------------------------------------------------
#define SCENE_INITIALIZE	1
#define SCENE_UPDATE		2
#define SCENE_RENDER		3

//-----------------------------------------------------------------------------------------
//
//	�����o�[�ϐ���������
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
//	�E�B���h�E�N���X���폜���A�����������
//
//-----------------------------------------------------------------------------------------
DX_FrameWork::~DX_FrameWork()
{
	UnregisterClass(m_pAppName, m_hInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E������������
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::Initialize()
{
	//	�X�N���[���T�C�Y��ݒ�
	DX_System::GetInstance()->SetWindowsSize(CREATE_WINDOW_SIZE::SCREEN_720p);

	//	�E�B���h�E���쐬
	if (!CreateAppWindow("DirectX11", 0, 0, DX_System::GetWindowWidth(), DX_System::GetWindowHeight())){
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//  �V�[���𑖂点��(�X�V���`��)
//
//-----------------------------------------------------------------------------------------
void DX_FrameWork::Run()
{
#if defined(DEBUG) || defined(_DEBUG)
	// ���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif 

	MSG msg = { NULL };
	
	//	�X���b�v�`�F�C�����擾
	IDXGISwapChain* pSwapChain = DX_System::GetInstance()->GetSwapChain();

	//	���݂̃V�[�����擾
	DX_SceneManager* pSceneManager = DX_SceneManager::GetInstance();
	pSceneManager->SetStartScene(new SceneTitle());

	//	���[�v����
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			//	FPS���X�V
			FPSUpdate();
		
			// �V�[���̍X�V
			pSceneManager->Update(msg.message, msg.wParam);

			// �V�[���̕`��
			pSceneManager->Render(pSwapChain);

			// �Q�[���G���h���H
			if (pSceneManager->IsGameEnd()) {
				break;
			}
		}
	}
	
	// �V�[���}�l�[�W���[�̊J��
	DX_SceneManager::Release();
}

//------------------------------------------------------------------------------
//
//  �A�v���P�[�V���������擾����
//
//------------------------------------------------------------------------------
char* DX_FrameWork::GetAppName()const
{
	return m_pAppName;
}

//------------------------------------------------------------------------------
//
//  �E�B���h�E�n���h�����擾
//
//------------------------------------------------------------------------------
HWND DX_FrameWork::GetHwnd()const
{
	return m_hWnd;
}

//------------------------------------------------------------------------------
//
//	�C���X�^���X�̃n���h�����擾
//
//------------------------------------------------------------------------------
HINSTANCE DX_FrameWork::GetHinstance()const
{
	return m_hInstance;
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E���쐬����
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
	//	�A�v���P�[�V��������ۑ�
	m_pAppName = pAppName;

	//	hinstance���擾
	m_hInstance = GetModuleHandle(NULL);

	// window�N���X��ݒ�
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
	
	//	�N���X��o�^����
	if (!RegisterClassEx(&l_wc)){
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		return false;
	}


	//	�E�B���h�E�̃X�^�C����ݒ�
	int l_style = WS_OVERLAPPEDWINDOW;

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	//	window���쐬����
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		m_pAppName, 
		m_pAppName,
		l_style,
		x,
		y, 
		rect.right,
		rect.bottom,
		NULL, 
		NULL, 
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

#if defined(DEBUG) || defined(_DEBUG)
	//	���j���[���쐬
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_RENDER_SOLID, "�\���b�h");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_RENDER_WIRE_FRAME, "���C���[�t���[��");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "�`����@");
	SetMenu(m_hWnd, hMenu);
#endif

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	����ꂽ���b�Z�[�W�����ɗl�X�ȏ������s��
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK DX_FrameWork::WndProc(HWND	hWnd, UINT	msg, WPARAM	wparam, LPARAM	lparam)
{
	DX_System* pSystem = DX_System::GetInstance();

	switch (msg){
	case WM_DESTROY:
		//	���b�Z�[�W���[�v�𔲂��܂�
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROY�𔭍s����
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
			//	F12���������ꍇ�X�N���[�����[�h��ύX
		case VK_F12:
		/*	IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();
			if (nullptr != pSwapChain) {
				BOOL fullScreen = false;
				pSwapChain->GetFullscreenState(&fullScreen, NULL);
				pSwapChain->SetFullscreenState(!fullScreen, NULL);
			}*/

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
//	FPS���X�V����
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
	//	�f�o�b�O���̂݃R���\�[����ʂ��J��
	DEBUG_OPNE_CONSOLE

	//	�t���[�����[�N�𐶐�
	DX_FrameWork dxFramework;

	//	�t���[�����[�N�̏�����
	if (!dxFramework.Initialize()){ return FALSE; }

	//	DirectX�̏�����
	if (!DX_System::GetInstance()->InitD3D(dxFramework.GetHwnd())){ return FALSE; }

	//	�V�[���𑖂点��
	dxFramework.Run();
	
	//	DX_Library���������
	DX_System::Release();

	//	�f�o�b�O���̂݃R���\�[����ʂ����
	DEBUG_CLOSE_CONSOLE

	return TRUE;

}
