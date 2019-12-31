#include	"DX_Library.h"
#include	<stdio.h>
#include <dxgidebug.h>
//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------

void __Trace(const char* pFile, int line, LPCSTR pszFormat, ...)
{
	va_list	argp;
	char pszBuf[256];
	char pszBuf2[512];


	va_start(argp, pszFormat);
	vsprintf_s(pszBuf, sizeof(pszBuf), pszFormat, argp);
	va_end(argp);

	sprintf_s(pszBuf2, "[FILE : %s] [LINE : %d] %s", pFile, line, pszBuf);

	OutputDebugString(pszBuf2);
}


//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ��̏��������s��
//
//-----------------------------------------------------------------------------------------
DX_Debug::DX_Debug()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_consoleHandle = NULL;
	ZeroMemory(&m_csbi, sizeof(m_csbi));
#endif
}


//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ��̏��������s��
//
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
//
//	ID3D�n�̐����I�u�W�F�N�g�𒲍�����
//
//-----------------------------------------------------------------------------------------
void DX_Debug::ReportLiveDeviceObjects(const char* pMessage)
{
#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugString("\n\n===============================================================================================================================\n");
	char message[512] = { NULL };
	sprintf_s(message, "%s\n", pMessage);
	OutputDebugString(message);
	
	// �o��
	m_debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugString("\n\n===============================================================================================================================\n");
#endif
}


//-----------------------------------------------------------------------------------------
//
//	�߂�l���`�F�b�N����
//
//-----------------------------------------------------------------------------------------
bool DX_Debug::CheckHresult(HRESULT hr)
{
	bool result = true;

	if (FAILED(hr)){
		LPVOID lpMsgBuf = nullptr;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(DX_System::GetInstance()->GetWindowHandle(), (LPTSTR)lpMsgBuf, NULL, MB_OK);
		result = false;
	}

	return result;
}

bool DX_Debug::IsFailedHresult(HRESULT hr)
{
	return !CheckHresult(hr);
}


//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[�t�@�C�����`�F�b�N����
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CheckSourceCordOfShaderFile(HRESULT hr, ID3DBlob* pBytecord)
{
#if defined(DEBUG) || defined(_DEBUG)

	//	�R���p�C�������s������
	if (FAILED(hr)){
		
		//	�V�F�[�_�[�t�@�C�����������ꍇ,�f�o�b�O�E�B���h�E�ɏ����o��
		if (pBytecord){
			MessageBox(DX_System::GetInstance()->GetWindowHandle(), (char*)pBytecord->GetBufferPointer(), NULL, MB_OK);
			throw "ShaderFile Compile Error";
		}
		else{
			//	���������ꍇ
			throw "Not Found ShaderFile";
		}
	}
	
#endif
}

//-----------------------------------------------------------------------------------------
//
//	�R���\�[����ʂɏo�͂��镶���̐F��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Debug::SetPrintColor(SET_PRINT_COLOR printColor)
{
#if defined(DEBUG) || defined(_DEBUG)
	WORD l_attribute = FOREGROUND_INTENSITY;
	
	switch (printColor){
	case SET_PRINT_COLOR::RED:		l_attribute |= FOREGROUND_RED;		break;
	case SET_PRINT_COLOR::BLUE:		l_attribute |= FOREGROUND_BLUE;		break;
	case SET_PRINT_COLOR::GREEN:	l_attribute |= FOREGROUND_GREEN;	break;
	case SET_PRINT_COLOR::DEFAULT:	l_attribute = m_csbi.wAttributes;	break;
	}

	//�n���h���ƐF��n��
	SetConsoleTextAttribute(m_consoleHandle, l_attribute);
#endif
}

//-----------------------------------------------------------------------------------------
//
//	�R���\�[����ʂɕ������o�͂���
//
//-----------------------------------------------------------------------------------------
void DX_Debug::Printf(const char* pFormat, ...)
{
#if defined(DEBUG) || defined(_DEBUG)
	va_list l_arg;
	va_start(l_arg, pFormat);

	vprintf_s(pFormat, l_arg);

	va_end(l_arg);
}
//-----------------------------------------------------------------------------------------
//
//	m_pDebug�𐶐�
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CreateDebugDevice()
{
	// �쐬
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_debug);
}
#endif
