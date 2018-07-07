#include	"DX_Library.h"
//#include	"../../Resource/icon/resource.h"
#include	"../SceneMain.h"
#include	<mmsystem.h>
#pragma comment(lib,"winmm.lib")

//-----------------------------------------------------------------------------------------
//
//	debug���̂݃R���\�[����ʂ��J�����߂̕ϐ��Ɗ֐�
//
//-----------------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
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
	MSG l_msg = { NULL };


	//	�X���b�v�`�F�C�����擾
	ComPtr<IDXGISwapChain> l_swapChain = DX_System::GetInstance()->GetSwapChain();

	//	���݂̃V�[�����擾
	DX_Scene*	l_pScene = DX_SceneManager::GetCurScene();
	l_pScene->Initialize();

	//	���[�v����
	while (l_msg.message != WM_QUIT){
		if (PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}
		else{
			//	FPS���X�V
			FPSUpdate();
		
			// �������Z

			// �Փ˔���
			//DX_ColliderManager::Update();

			//	�S�L�[�X�V
			DX_Input::Update(l_msg.message, l_msg.wParam);

			//	�V�[���̍X�V
			//l_pScene->Update();

			//	�`��J�n
			DX_Graphics::BeginRender(l_swapChain.Get());

			//l_pScene->Render();

			//	�`��I��
			DX_Graphics::EndRender(l_swapChain.Get());
		}
	}

	
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
	int l_style = 
		WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	//	window���쐬����
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		m_pAppName, 
		m_pAppName,
		l_style,
		x,
		y, 
		width,
		height,
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
//	����ꂽ���b�Z�[�W�����ɗl�X�ȏ������s��
//
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK DX_FrameWork::WndProc(
	HWND	hWnd, 
	UINT	msg,
	WPARAM	wparam,
	LPARAM	lparam
	)
{
	switch (msg){
	case WM_DESTROY:
		//	���b�Z�[�W���[�v�𔲂��܂�
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd); // WM_DESTROY�𔭍s����
		break;
		
	case WM_SIZE:
		break;

	case WM_KEYDOWN:

		switch (wparam){
		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;

			//	F12���������ꍇ�X�N���[�����[�h��ύX
		case VK_F12:
			//DX_System::ChengedScreenMode();

			break;
		}
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
	printf_s("FPS[%d] deltaTime[%f]\n", m_fps.fps, m_fps.deltaTime);
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
	DX_FrameWork l_DX_FrameWork;

	//	�t���[�����[�N�̏�����
	if (!l_DX_FrameWork.Initialize()){ return FALSE; }

	//	DirectX�̏�����
	if (!DX_System::GetInstance()->InitD3D(l_DX_FrameWork.GetHwnd())){ return FALSE; }

	//	�V�[����o�^����
	DX_SceneManager::Initialize(new SceneMain());

	//	�V�[���𑖂点��
	l_DX_FrameWork.Run();
	
	DX_SceneManager::Release();
	//	DX_Library���������
	DX_System::Release();
	DX_Debug::GetInstance()->ReportLiveDeviceObjects("DX_System::Release");

	//	�f�o�b�O���̂݃R���\�[����ʂ����
	DEBUG_CLOSE_CONSOLE

	return TRUE;

}
