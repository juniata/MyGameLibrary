#include	"DX_Library.h"
#include	<stdio.h>
#include	<dxgidebug.h>


/// <summary>
/// DebugƒRƒ“ƒ\[ƒ‹‚É•¶š‚ğo—Í‚·‚éB
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
/// ƒƒ“ƒo•Ï”‚Ì‰Šú‰»‚ğs‚¤B
/// </summary>
DX_Debug::DX_Debug()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_consoleHandle = NULL;
	ZeroMemory(&m_csbi, sizeof(m_csbi));
#endif
}

/// <summary>
/// ï¿½fï¿½oï¿½Cï¿½Xï¿½ï¿½ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
/// </summary>
DX_Debug::~DX_Debug()
{
	ReportLiveDeviceObjects("timing of call DX_System::~DX_System()");
}


/// <summary>
/// ƒfƒoƒCƒX‚Ì¶¬“™‚ğs‚¤
/// </summary>
void DX_Debug::Initialize()
{
#if defined(DEBUG) || defined(_DEBUG)
	//ƒnƒ“ƒhƒ‹‚ğæ“¾
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
	//Œ»İ‚Ì‰æ–Ê‚Ìî•ñ‚ğcsbi‚É•Û‘¶
	GetConsoleScreenBufferInfo(m_consoleHandle, &m_csbi);

	CreateDebugDevice();
#endif
}

/// <summary>
/// ID3DŒn‚Ì¶‘¶ƒIƒuƒWƒFƒNƒg‚ğ’²¸‚·‚é
/// </summary>
/// <param name="message">ƒfƒoƒbƒOƒRƒ“ƒ\[ƒ‹‚É•\¦‚·‚é“à—e</param>
void DX_Debug::ReportLiveDeviceObjects(const char* message)
{
#if defined(DEBUG) || defined(_DEBUG)
	TRACE(message)
	m_debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
#endif
}

/// <summary>
///  HRESULTŒ^‚ğƒ`ƒFƒbƒN‚·‚é
/// </summary>
/// <param name="hr"></param>
/// <returns>¬”Û</returns>
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
/// HRESULTŒ^‚ğƒ`ƒFƒbƒN‚·‚é
/// </summary>
/// <param name="hr">HRESULTŒ^‚Ì–ß‚è’l</param>
/// <returns>¬”Û</returns>
bool DX_Debug::IsFailedHresult(HRESULT hr)
{
	return !CheckHresult(hr);
}

/// <summary>
/// IDXGIDebug‚ğ¶¬‚·‚é
/// </summary>
void DX_Debug::CreateDebugDevice()
{
	// ì¬
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&m_debug);
}
