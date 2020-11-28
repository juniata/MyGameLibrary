#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	コンソール画面に出力する文字の色
enum class SET_PRINT_COLOR : int {
	BLUE,		//	青色
	RED,		//	赤色
	GREEN,		//	緑	
	DEFAULT,	//	初期の色
};


/// <summary>
/// Debugコンソールに文字を出力する。
/// </summary>
#if defined(DEBUG) || defined(_DEBUG)
	void __Trace(const char* file, int line, LPCSTR pszFormat, ...);
	#define TRACE(...)			__Trace(__FILE__, __LINE__, __VA_ARGS__);
#else
	#define TRACE(...)
#endif

struct IDXGIDebug;

/// <summary>
/// IDXGIDebugを管理
/// </summary>
class DX_Debug : public DX_Singleton<DX_Debug>
{
public:
	/// <summary>
	/// デバイスの生成等を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// ID3D系の生存オブジェクトを調査する
	/// </summary>
	/// <param name="message">デバッグコンソールに表示する内容</param>
	void ReportLiveDeviceObjects(const char* message);

	/// <summary>
	/// HRESULT型をチェックする
	/// </summary>
	/// <param name="hr">HRESULT型の戻り値</param>
	/// <returns>成否</returns>
	bool CheckHresult(HRESULT hr);

	/// <summary>
	/// HRESULT型をチェックする
	/// </summary>
	/// <param name="hr">HRESULT型の戻り値</param>
	/// <returns>成否</returns>
	bool IsFailedHresult(HRESULT hr);

private:
	friend class DX_Singleton<DX_Debug>;

	/// <summary>
	/// メンバ変数の初期化を行う。
	/// </summary>
	DX_Debug();


#if defined(DEBUG) || defined(_DEBUG)
	Microsoft::WRL::ComPtr<IDXGIDebug> m_debug;
	HANDLE	m_consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
	
	/// <summary>
	/// IDXGIDebugを生成する
	/// </summary>
	void CreateDebugDevice();
#endif


};
#endif // !__DX_DEBUG_H_
