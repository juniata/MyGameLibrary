#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	コンソール画面に出力する文字の色
enum class SET_PRINT_COLOR : int{
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
#define DEBUG_VALUE_CHECK(bFlag, pMessage)										\
{																				\
	if (!bFlag){																\
		char l_message[128] = { NULL };											\
		int	 l_strlen = 0;														\
																				\
		const char* l_pDirectory = strstr(__FILE__, "dx_library\\");			\
		if (l_pDirectory == nullptr){											\
			l_pDirectory = strstr(__FILE__, "myself_made_source\\");			\
			l_strlen = strlen("myself_made_source\\");							\
		}else{																	\
			l_strlen = strlen("dx_library\\");									\
		}																		\
		sprintf_s(l_message, "%s line[%d]", &l_pDirectory[l_strlen], __LINE__);	\
		MessageBox(NULL, pMessage, l_message, MB_OK);							\
		exit(EXIT_FAILURE);														\
	}																			\
}																				
#else
#define DEBUG_VALUE_CHECK(bFlag,pMessage)
#endif

//****************************************************************************************************
//
//	Debug
//
//****************************************************************************************************
class Debug
{
private:
#if defined(DEBUG) || defined(_DEBUG)
	static ComPtr<ID3D11Debug>	m_debug;
	static HANDLE	m_consoleHandle;
	static CONSOLE_SCREEN_BUFFER_INFO m_csbi;
#endif

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	Debug() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~Debug(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の初期化を行う
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の解放を行う
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D系の生存オブジェクトを調査する
	//
	//------------------------------------------------------------------------------
	static void ReportLiveDeviceObjects(
		const char* pMessage
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		戻り値をチェックする
	//	@param[in]	hr	さまざまな関数の戻り値(D3D系)
	//	@retrun		true:成功	false:失敗
	//
	//------------------------------------------------------------------------------
	static bool IsHresultCheck(
		HRESULT hr
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーファイルをチェック
	//	@param[in]	hr			D3DX11CompileFromFile()の戻り値
	//	@param[in]	pBytecord	D3DX11CompileFromFile()で出来たバイトコード
	//
	//------------------------------------------------------------------------------
	static void CheckSourceCordOfShaderFile(
		HRESULT		hr,
		ID3DBlob*	pBytecord
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		コンソール画面に出力する文字の色を設定
	//	@param[in]	printColor	SET_PRINT_COLOR
	//	
	//------------------------------------------------------------------------------
	static void SetPrintColor(
		dx_library::SET_PRINT_COLOR printColor
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		コンソール画面に文字を出力する
	//	@param[in]	pFormat printfのラッパーなので同じ使い方
	//	@note		debug時のみ出力
	//	
	//------------------------------------------------------------------------------
	static void Printf(
		const char* pFormat,
		...
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		m_pDebugを生成する
	//
	//------------------------------------------------------------------------------
	static void CreateDebugDevice();


};
#endif // !__DX_DEBUG_H_
