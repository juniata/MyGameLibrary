#include	"SkyLibrary.h"
#include	"Games\SceneMain\SceneMain.h"
#include	<mmsystem.h>
#include	<stdio.h>
#pragma comment(lib,"winmm.lib")


//-----------------------------------------------------------------------------------------
//
//	����ꂽ���b�Z�[�W�����ɗl�X�ȏ������s��
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM	lparam)
{
	switch (msg) {
	case WM_DESTROY:
		//	���b�Z�[�W���[�v�𔲂��܂�
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROY�𔭍s����
		break;
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd); // WM_DESTROY�𔭍s����
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

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
//	�����o�[�ϐ���������
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
//	�E�B���h�E�N���X���폜���A�����������
//
//-----------------------------------------------------------------------------------------
FrameWork::~FrameWork()
{
	// SkyLibrary�����
	SceneManager::Release();

	//	DX_Library���������
	DX_System::Destroy();
	DX_Debug::GetInstance()->ReportLiveDeviceObjects("DX_System::Destroy() after");
	DX_Debug::Destroy();

	UnregisterClass(m_pAppName, m_hInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E������������
//
//-----------------------------------------------------------------------------------------
bool FrameWork::Initialize(const unsigned int width, const unsigned int height)
{
#if defined(DEBUG) || defined(_DEBUG)
	// ���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif 

	DX_System::Create();
	DX_System* pSystem = DX_System::GetInstance();

	//	�E�B���h�E���쐬
	if (!CreateAppWindow("DirectX11", width, height)) {
		return false;
	}

	//	DirectX�̏�����
	if (!pSystem->Initialize(m_hWnd, m_hInstance, width, height)) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  �V�[���𑖂点��(�X�V���`��)
//
//-----------------------------------------------------------------------------------------
void FrameWork::Run()
{
	MSG msg = { NULL };

	//	�X���b�v�`�F�C�����擾
	IDXGISwapChain* pSwapChain = DX_System::GetInstance()->GetSwapChain();

	//	���݂̃V�[�����擾
	SceneManager::SetBootScene(new SceneMain());
	
	//	���[�v����
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//	FPS���X�V
			FPSUpdate();

			// �V�[���̍X�V
			SceneManager::Update(m_hWnd, msg.message, msg.wParam);

			// �V�[���̕`��
			SceneManager::Render();

			// �Q�[���G���h���H
			if (SceneManager::IsGameEnd()) {
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E���쐬����
//
//-----------------------------------------------------------------------------------------
bool FrameWork::CreateAppWindow(char* pAppName, const unsigned int width, const unsigned int height)
{
	//	�A�v���P�[�V��������ۑ�
	m_pAppName = pAppName;

	//	hinstance���擾
	m_hInstance = GetModuleHandle(NULL);

	// window�N���X��ݒ�
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

	//	�N���X��o�^����5
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "RegisterClass() failed", "Error", MB_OK);
		return false;
	}

	//	�E�B���h�E�̃X�^�C����ݒ�
	int style = WS_OVERLAPPEDWINDOW | WS_POPUP;

	// ��ʂ̒��S�ɕ`�悳���悤�ɂ���
	RECT rect;
	SetRect(&rect, 0, 0, DX::CAST::I(width), DX::CAST::I(height));
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

	if (m_hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed", "Error", MB_OK);
		PostQuitMessage(0);
		return false;
	}

	//
	//	HMENU hMenu = CreateMenu();
	//	//	���j���[���쐬
	//#if defined(DEBUG) || defined(_DEBUG)
	//	HMENU renderFileMenu = CreateMenu();
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_SOLID, "�\���b�h");
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderFileMenu, MF_STRING, FILE_MENU_RENDER_WIRE_FRAME, "���C���[�t���[��");
	//	AppendMenu(renderFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderFileMenu, "�`����@");
	//	SetMenu(m_hWnd, hMenu);
	//#endif
	//	HMENU renderSizeFileMenu = CreateMenu();
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderSizeFileMenu, MF_STRING, FILE_MENU_RENDER_WINDOW_MODE, "�E�B���h�E���[�h");
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(renderSizeFileMenu, MF_STRING, FILE_MENU_RENDER_FULL_SCREEN, "�t���X�N���[�����[�h");
	//	AppendMenu(renderSizeFileMenu, MF_SEPARATOR, NULL, NULL);
	//	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)renderSizeFileMenu, "�E�B���h�E�T�C�Y");
	//
	//	SetMenu(m_hWnd, hMenu);

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
