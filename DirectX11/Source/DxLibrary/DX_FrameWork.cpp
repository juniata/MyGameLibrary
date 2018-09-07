#include	"DX_Library.h"
//#include	"../../Resource/icon/resource.h"
#include	"Games\SceneTitle.h"
#include	"Games\SceneMain.h"
#include	<mmsystem.h>
#include	<stdio.h>
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
//	�O���錾
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND	hWnd, UINT	msg, WPARAM	wparam, LPARAM	lparam);

//-----------------------------------------------------------------------------------------
//
//	static�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_FrameWork* DX_FrameWork::m_pInstance = nullptr;

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
m_hWnd(NULL),
m_bResize(false),
m_lParam(0)
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
//  ���g�̃C���X�^���X���擾����
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
//  ���g�̃C���X�^���X���������
//
//-----------------------------------------------------------------------------------------
void DX_FrameWork::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E������������
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::Initialize()
{
	//	�X�N���[���T�C�Y��ݒ�
	DX_System::GetInstance()->SetWindowsSize(1280, 720);

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
	
	DX_System* pSystem	= DX_System::GetInstance();
	//	�X���b�v�`�F�C�����擾
	IDXGISwapChain* pSwapChain = pSystem->GetSwapChain();

	// �O���t�B�b�N�X���擾����
	DX_Graphics*	pGraphics = DX_Graphics::GetInstance();

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
			// �X�V�O�Ƀ��T�C�Y���s��
			if (m_bResize) {
				if (false == pSystem->BufferResize(LOWORD(m_lParam), HIWORD(m_lParam))) {
					break;
				}
			}

			//	FPS���X�V
			FPSUpdate();
		
			// �V�[���̍X�V
			pSceneManager->Update(m_hWnd, msg.message, msg.wParam);

			// �V�[���̕`��
			pSceneManager->Render(pGraphics, pSwapChain);

			// �Q�[���G���h���H
			if (pSceneManager->IsGameEnd()) {
				break;
			}

			// ���T�C�Y�t���O��߂�
			m_bResize = false;
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

//------------------------------------------------------------------------------
//
//	�o�b�N�o�b�t�@�̃��T�C�Y���s��
//
//------------------------------------------------------------------------------
void DX_FrameWork::DoResize(LPARAM lparam)
{
	m_bResize = true;
	m_lParam = lparam;
}

//------------------------------------------------------------------------------
//
//	���T�C�Y���s�������ǂ���
//
//------------------------------------------------------------------------------
bool DX_FrameWork::IsResize() const
{
	return m_bResize;
}
//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_FrameWork::CreateAppWindow(char* pAppName, const int x, const int y, const int width, const int height)
{
	//	�A�v���P�[�V��������ۑ�
	m_pAppName = pAppName;

	//	hinstance���擾
	m_hInstance = GetModuleHandle(NULL);

	// window�N���X��ݒ�
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

	//	�N���X��o�^����5
	if (!RegisterClass(&wc)){
		MessageBox(NULL, "RegisterClass() failed", "Error", MB_OK);
		return false;
	}

	//	�E�B���h�E�̃X�^�C����ݒ�
	int style = WS_OVERLAPPEDWINDOW;
	// ��ʂ̒��S�ɕ`�悳���悤�ɂ���
	RECT rect;
	SetRect(&rect, x, y, width, height);
	AdjustWindowRect(&rect, style, TRUE);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	// �E�B���h�E�̉�ʃT�C�Y���擾����
	RECT windowsSize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &windowsSize, 0);

	//�@���S�̕`�悳���悤�ɂ��邽�߁A�`��J�n�_���Z�o����
	const unsigned int centerPosX = static_cast<unsigned int>(windowsSize.right) / 2;
	const unsigned int centerPosY = static_cast<unsigned int>(windowsSize.bottom) / 2;

	const unsigned int halfWidth = rect.right / 2;
	const unsigned int halfHeight = rect.bottom / 2;

	const unsigned int leftPos = centerPosX - halfWidth;
	const unsigned int topPos = centerPosY - halfHeight;

	//	window���쐬����
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
	

	//	���j���[���쐬
#if defined(DEBUG) || defined(_DEBUG)
	HMENU hMenu = CreateMenu();
	HMENU renderFileMenu = CreateMenu();
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_SOLID, "�\���b�h");
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_WIRE_FRAME, "���C���[�t���[��");
	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderFileMenu, "�`����@");
	SetMenu(m_hWnd, hMenu);
#endif

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	FPS���X�V����
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
//	����ꂽ���b�Z�[�W�����ɗl�X�ȏ������s��
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND	hWnd, UINT	msg, WPARAM	wparam, LPARAM	lparam)
{
	DX_FrameWork* pFramework	= DX_FrameWork::GetInstance();

	switch (msg){
	case WM_DESTROY:
		//	���b�Z�[�W���[�v�𔲂��܂�
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROY�𔭍s����
		break;

	case WM_SIZE:
		pFramework->DoResize(lparam);
		break;

	case WM_KEYDOWN:

		switch (wparam){
			//	F12���������ꍇ�X�N���[�����[�h��ύX
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
	//	�f�o�b�O���̂݃R���\�[����ʂ��J��
	DEBUG_OPNE_CONSOLE

	//	�t���[�����[�N�𐶐�
	DX_FrameWork* pFramework = DX_FrameWork::GetInstance();

	//	�t���[�����[�N�̏�����
	if (!pFramework->Initialize()) { return FALSE; }

	//	DirectX�̏�����
	if (!DX_System::GetInstance()->InitD3D(pFramework->GetHwnd())) { return FALSE; }

	//	�V�[���𑖂点��
	pFramework->Run();

	//	DX_Library���������
	DX_System::Release();

	// �t���[�����[�N�̊J�����s��
	DX_FrameWork::Release();

	//	�f�o�b�O���̂݃R���\�[����ʂ����
	DEBUG_CLOSE_CONSOLE

	return TRUE;
}
