#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	コンソール画面に出力する文字の色
enum class SET_PRINT_COLOR : int {
	BLUE,		//	青色
	RED,		//	赤色
	GREEN,		//	緑	
	DEFAULT,	//	初期の色
};


//****************************************************************************************************
//
//	debug時のみ、値チェックをする。
//
//****************************************************************************************************
#if defined(DEBUG) || defined(_DEBUG)
template<class T> bool DebugValueCheck(T value, const char* pErrMsg)
{
	bool isSucceed = value ? true : false;
	
	if (isSucceed == false) {
		MessageBox(DX_System::GetInstance()->GetWindowHandle(), pErrMsg,  "Error", MB_OK);
	}

	return isSucceed;
}


void __Trace(const char* pFile, int line, LPCSTR pszFormat, ...);
#define TRACE(x)			__Trace(__FILE__, __LINE__, x)
#define TRACE2(x, x2)		__Trace(__FILE__, __LINE__, x, x2)
#define TRACE3(x, x2, x3)	__Trace(__FILE__, __LINE__, x, x2, x3)

#else
#define DebugValueCheck(bFlag, pMessage) true
#define TRACE(x)
#define TRACE2(x, x2)
#define TRACE3(x, x2, x3)
#endif

struct IDXGIDebug;
//****************************************************************************************************
//
//	DX_Debug
//
//****************************************************************************************************
class DX_Debug : public DX_Singleton<DX_Debug>
{
	friend class DX_Singleton<DX_Debug>;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の初期化を行う
	//
	//------------------------------------------------------------------------------
	void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D系の生存オブジェクトを調査する
	//
	//------------------------------------------------------------------------------
	void ReportLiveDeviceObjects(const char* pMessage);

	//------------------------------------------------------------------------------
	//
	//  @brief		戻り値をチェックする
	//	@param[in]	hr	さまざまな関数の戻り値(D3D系)
	//	@retrun		true:成功	false:失敗
	//
	//------------------------------------------------------------------------------
	bool CheckHresult(HRESULT hr);

	//------------------------------------------------------------------------------
	//
	//  @brief		戻り値をチェックする
	//	@param[in]	hr	さまざまな関数の戻り値(D3D系)
	//	@retrun		true:成功	false:失敗
	//
	//------------------------------------------------------------------------------
	bool IsFailedHresult(HRESULT hr);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーファイルをチェック
	//	@param[in]	hr			D3DX11CompileFromFile()の戻り値
	//	@param[in]	pBytecord	D3DX11CompileFromFile()で出来たバイトコード
	//
	//------------------------------------------------------------------------------
	void CheckSourceCordOfShaderFile(HRESULT hr, ID3DBlob* pBytecord);

	//------------------------------------------------------------------------------
	//
	//  @brief		コンソール画面に出力する文字の色を設定
	//	@param[in]	printColor	SET_PRINT_COLOR
	//	
	//------------------------------------------------------------------------------
	void SetPrintColor(SET_PRINT_COLOR printColor);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		コンソール画面に文字を出力する
	//	@param[in]	pFormat printfのラッパーなので同じ使い方
	//	@note		debug時のみ出力
	//	
	//------------------------------------------------------------------------------
	void Printf(const char* pFormat, ...);
private:
#if defined(DEBUG) || defined(_DEBUG)
	Microsoft::WRL::ComPtr<IDXGIDebug> m_debug;
	HANDLE	m_consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
#endif

	//------------------------------------------------------------------------------
	//
	//  @brief		コンストラクタ
	//
	//------------------------------------------------------------------------------
	DX_Debug();

#if defined(DEBUG) || defined(_DEBUG)
	//------------------------------------------------------------------------------
	//
	//  @brief		m_debugを生成する
	//
	//------------------------------------------------------------------------------
	void CreateDebugDevice();
#endif


};
#endif // !__DX_DEBUG_H_
