#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
ComPtr<ID3D11Debug>	DX_Debug::m_debug;
HANDLE			DX_Debug::m_consoleHandle	= NULL;
CONSOLE_SCREEN_BUFFER_INFO DX_Debug::m_csbi = { NULL };
#endif


//-----------------------------------------------------------------------------------------
//
//	メンバ変数の初期化を行う
//
//-----------------------------------------------------------------------------------------
void DX_Debug::Initialize()
{
	PROFILE("DX_Debug::Initialize()");
#if defined(DEBUG) || defined(_DEBUG)
	//ハンドルを取得
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
	//現在の画面の情報をcsbiに保存
	GetConsoleScreenBufferInfo(m_consoleHandle, &m_csbi);

#endif


	try{
		//	デバッグデバイスを作成する
		CreateDebugDevice();
	}
	catch (char* pMessage){
		throw pMessage;
	}
}


//-----------------------------------------------------------------------------------------
//
//	メンバ変数の解放を行う
//
//-----------------------------------------------------------------------------------------
void DX_Debug::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
#endif
}

//-----------------------------------------------------------------------------------------
//
//	ID3D系の生存オブジェクトを調査する
//
//-----------------------------------------------------------------------------------------
void DX_Debug::ReportLiveDeviceObjects(const char* pMessage)
{
#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugString("\n\n===============================================================================================================================\n");
	char l_message[512] = { NULL };
	sprintf_s(l_message, "%s\n", pMessage);
	OutputDebugString(l_message);
	m_debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
	OutputDebugString("\n\n===============================================================================================================================\n");
#endif
}


//-----------------------------------------------------------------------------------------
//
//	戻り値をチェックする
//
//-----------------------------------------------------------------------------------------
bool DX_Debug::IsHresultCheck(HRESULT hr)
{
	if (FAILED(hr)){
		LPVOID lpMsgBuf = nullptr;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPTSTR)&lpMsgBuf, NULL, MB_OK);
		return false;
	}
	return true;
}



//-----------------------------------------------------------------------------------------
//
//	シェーダーファイルをチェックする
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CheckSourceCordOfShaderFile(
	HRESULT		hr,
	ID3DBlob*	pBytecord
	)
{
#if defined(DEBUG) || defined(_DEBUG)

	//	コンパイルが失敗した時
	if (FAILED(hr)){
		
		//	シェーダーファイルがあった場合,デバッグウィンドウに情報を出力
		if (pBytecord){
			MessageBox(NULL, (char*)pBytecord->GetBufferPointer(), NULL, MB_OK);
			//DXTRACE_ERR((char*)pBytecord->GetBufferPointer(), hr);
			throw "ShaderFile Compile Error";
		}
		else{
			//	無かった場合
			throw "Not Found ShaderFile";
		}
	}
	
#endif
}

//-----------------------------------------------------------------------------------------
//
//	コンソール画面に出力する文字の色を設定
//
//-----------------------------------------------------------------------------------------
void DX_Debug::SetPrintColor(dx_library::SET_PRINT_COLOR printColor)
{
#if defined(DEBUG) || defined(_DEBUG)
	WORD l_attribute = FOREGROUND_INTENSITY;
	
	switch (printColor){
	case dx_library::SET_PRINT_COLOR::RED:		l_attribute |= FOREGROUND_RED;		break;
	case dx_library::SET_PRINT_COLOR::BLUE:		l_attribute |= FOREGROUND_BLUE;		break;
	case dx_library::SET_PRINT_COLOR::GREEN:	l_attribute |= FOREGROUND_GREEN;	break;
	case dx_library::SET_PRINT_COLOR::DEFAULT:	l_attribute = m_csbi.wAttributes;	break;
	}

	//ハンドルと色を渡す
	SetConsoleTextAttribute(m_consoleHandle, l_attribute);
#endif
}

//-----------------------------------------------------------------------------------------
//
//	コンソール画面に文字を出力する
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
//	m_pDebugを生成
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CreateDebugDevice()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (!IsHresultCheck(DX_System::GetInstance()->GetDevice()->QueryInterface(__uuidof(ID3D11Debug), (void**)m_debug.GetAddressOf()))){
		throw "ID3D11Device::QueryInterface() : create ID3D11Debug failed";
	}
#endif
}

