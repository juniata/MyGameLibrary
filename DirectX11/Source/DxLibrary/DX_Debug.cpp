#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_Debug* DX_Debug::m_pInstance = nullptr;


//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
DX_Debug::DX_Debug()
#if defined(DEBUG) || defined(_DEBUG)
	: m_consoleHandle(NULL), m_csbi({NULL}), m_pDebug(nullptr)
#endif
{}


//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
DX_Debug::~DX_Debug()
{
#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(m_pDebug);
#endif
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_Debug* DX_Debug::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_Debug();
	}
	
	return m_pInstance;
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

#endif

	try{
		//	�f�o�b�O�f�o�C�X���쐬����
		CreateDebugDevice();
	}
	catch (char* pMessage){
		throw pMessage;
	}
}


//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ��̉�����s��
//
//-----------------------------------------------------------------------------------------
void DX_Debug::Release()
{
	DELETE_OBJ(m_pInstance);
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
	char l_message[512] = { NULL };
	sprintf_s(l_message, "%s\n", pMessage);
	OutputDebugString(l_message);
	m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
	OutputDebugString("\n\n===============================================================================================================================\n");
#endif
}


//-----------------------------------------------------------------------------------------
//
//	�߂�l���`�F�b�N����
//
//-----------------------------------------------------------------------------------------
bool DX_Debug::IsHresultCheck(HRESULT hr)
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
#endif
}
//-----------------------------------------------------------------------------------------
//
//	m_pDebug�𐶐�
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CreateDebugDevice()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (!IsHresultCheck(DX_System::GetInstance()->GetDevice()->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug))){
		throw "ID3D11Device::QueryInterface() : create ID3D11Debug failed";
	}
#endif
}

