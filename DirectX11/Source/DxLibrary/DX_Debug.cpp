#include	"DX_Library.h"
#include	<stdio.h>
#include <dxgidebug.h>
//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
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
//	メンバ変数の初期化を行う
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
//	メンバ変数の初期化を行う
//
//-----------------------------------------------------------------------------------------
void DX_Debug::Initialize()
{
#if defined(DEBUG) || defined(_DEBUG)
	//ハンドルを取得
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
	//現在の画面の情報をcsbiに保存
	GetConsoleScreenBufferInfo(m_consoleHandle, &m_csbi);

	CreateDebugDevice();
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
	char message[512] = { NULL };
	sprintf_s(message, "%s\n", pMessage);
	OutputDebugString(message);
	
	// 出力
	m_debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugString("\n\n===============================================================================================================================\n");
#endif
}


//-----------------------------------------------------------------------------------------
//
//	戻り値をチェックする
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
//	シェーダーファイルをチェックする
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CheckSourceCordOfShaderFile(HRESULT hr, ID3DBlob* pBytecord)
{
#if defined(DEBUG) || defined(_DEBUG)

	//	コンパイルが失敗した時
	if (FAILED(hr)){
		
		//	シェーダーファイルがあった場合,デバッグウィンドウに情報を出力
		if (pBytecord){
			MessageBox(DX_System::GetInstance()->GetWindowHandle(), (char*)pBytecord->GetBufferPointer(), NULL, MB_OK);
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
}
//-----------------------------------------------------------------------------------------
//
//	m_pDebugを生成
//
//-----------------------------------------------------------------------------------------
void DX_Debug::CreateDebugDevice()
{
	// 作成
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_debug);
}
#endif
