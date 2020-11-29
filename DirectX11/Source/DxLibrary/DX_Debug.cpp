#include	"DX_Library.h"
#include	<stdio.h>
#include	<dxgidebug.h>


/// <summary>
/// Debug�R���\�[���ɕ������o�͂���B
/// </summary>
#if defined(DEBUG) || defined(_DEBUG)
void __Trace(const char* file, int line, LPCSTR pszFormat, ...)
{
	va_list	argp;
	char pszBuf[256];
	char outputDebugString[512];


	va_start(argp, pszFormat);
	vsprintf_s(pszBuf, sizeof(pszBuf), pszFormat, argp);
	va_end(argp);

	sprintf_s(outputDebugString, "[FILE : %s] [LINE : %d] %s\n", file, line, pszBuf);

	OutputDebugString(outputDebugString);
}
#endif

/// <summary>
/// �����o�ϐ��̏��������s���B
/// </summary>
DX_Debug::DX_Debug()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_consoleHandle = NULL;
	ZeroMemory(&m_csbi, sizeof(m_csbi));
#endif
}

/// <summary>
/// �f�o�C�X���̉����s��
/// </summary>
DX_Debug::~DX_Debug()
{
	ReportLiveDeviceObjects("timing of call DX_System::~DX_System()");
}


/// <summary>
/// �f�o�C�X�̐��������s��
/// </summary>
void DX_Debug::Initialize()
{
#if defined(DEBUG) || defined(_DEBUG)
	//�n���h�����擾
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
	//���݂̉�ʂ̏���csbi�ɕۑ�
	GetConsoleScreenBufferInfo(m_consoleHandle, &m_csbi);

	CreateDebugDevice();
#endif
}

/// <summary>
/// ID3D�n�̐����I�u�W�F�N�g�𒲍�����
/// </summary>
/// <param name="message">�f�o�b�O�R���\�[���ɕ\��������e</param>
void DX_Debug::ReportLiveDeviceObjects(const char* message)
{
#if defined(DEBUG) || defined(_DEBUG)
	TRACE(message)
	m_debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
#endif
}

/// <summary>
///  HRESULT�^���`�F�b�N����
/// </summary>
/// <param name="hr"></param>
/// <returns>����</returns>
bool DX_Debug::CheckHresult(HRESULT hr)
{
	bool result = true;

	if (FAILED(hr))
	{
		LPVOID lpMsgBuf = nullptr;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(DX_System::GetInstance()->GetWindowHandle(), (LPTSTR)lpMsgBuf, NULL, MB_OK);
		result = false;
	}

	return result;
}

/// <summary>
/// HRESULT�^���`�F�b�N����
/// </summary>
/// <param name="hr">HRESULT�^�̖߂�l</param>
/// <returns>����</returns>
bool DX_Debug::IsFailedHresult(HRESULT hr)
{
	return !CheckHresult(hr);
}

/// <summary>
/// IDXGIDebug�𐶐�����
/// </summary>
void DX_Debug::CreateDebugDevice()
{
	// �쐬
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_debug);
}
