#include	"DX_Library.h"
#include	<stdio.h>
#include	<dxgidebug.h>


/// <summary>
/// Debugコンソールに文字を出力する。
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
/// メンバ変数の初期化を行う。
/// </summary>
DX_Debug::DX_Debug()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_consoleHandle = NULL;
	ZeroMemory(&m_csbi, sizeof(m_csbi));
#endif
}


/// <summary>
/// デバイスの生成等を行う
/// </summary>
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

/// <summary>
/// ID3D系の生存オブジェクトを調査する
/// </summary>
/// <param name="message">デバッグコンソールに表示する内容</param>
void DX_Debug::ReportLiveDeviceObjects(const char* message)
{
#if defined(DEBUG) || defined(_DEBUG)
	TRACE(message)
	m_debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
#endif
}

/// <summary>
///  HRESULT型をチェックする
/// </summary>
/// <param name="hr"></param>
/// <returns>成否</returns>
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
/// HRESULT型をチェックする
/// </summary>
/// <param name="hr">HRESULT型の戻り値</param>
/// <returns>成否</returns>
bool DX_Debug::IsFailedHresult(HRESULT hr)
{
	return !CheckHresult(hr);
}

/// <summary>
/// IDXGIDebugを生成する
/// </summary>
void DX_Debug::CreateDebugDevice()
{
	// 作成
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_debug);
}
