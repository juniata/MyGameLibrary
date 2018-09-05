#ifndef __DX_FRAMEWORK_H_
#define __DX_FRAMEWORK_H_

//****************************************************************************************************
//
//	FrameWork
//
//****************************************************************************************************
class DX_FrameWork
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		自身のインスタンスを取得する
	//
	//------------------------------------------------------------------------------
	static DX_FrameWork* GetInstance();

	//------------------------------------------------------------------------------
	//
	//  @brief		自身のインスタンスを解放する
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウを初期化する
	//
	//------------------------------------------------------------------------------
	bool Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		シーンを走らせる(更新＆描画)
	//
	//------------------------------------------------------------------------------
	void Run();

	//------------------------------------------------------------------------------
	//
	//  @brief		アプリケーション名を取得する
	//	@return		m_pAppNameを返す
	//
	//------------------------------------------------------------------------------
	char* GetAppName()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウハンドルを取得
	//	@return		m_hWndを返し
	//
	//------------------------------------------------------------------------------
	HWND GetHwnd()const;

	//------------------------------------------------------------------------------
	//
	//	@brief		インスタンスのハンドルを取得
	//	@return		m_hInstanceを返す
	//
	//------------------------------------------------------------------------------
	HINSTANCE GetHinstance()const;

	//------------------------------------------------------------------------------
	//
	//	@brief		バックバッファのリサイズを行う
	//
	//------------------------------------------------------------------------------
	void DoResize(LPARAM lparam);

	//------------------------------------------------------------------------------
	//
	//	@brief		リサイズを行ったかどうか
	//
	//------------------------------------------------------------------------------
	bool IsResize() const;
private:
	static DX_FrameWork* m_pInstance;

	char*		m_pAppName;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool		m_bResize;
	LPARAM		m_lParam;

	struct tagFPS {
		int		fps;
		int		count;
		float	deltaTime;
		unsigned long startTime;
	}m_fps;
	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウを作成する
	//  @param[in]	x		ウィンドウの左の位置
	//  @param[in]	y		ウィンドウの上の位置
	//  @param[in]	width	ウィンドウの右の位置
	//  @param[in]	height	ウィンドウの下の位置
	//	@return		true:成功	false:失敗
	//
	//------------------------------------------------------------------------------
	bool CreateAppWindow(char* pAppName, const int x, const int y, const int width, const int height);

	//------------------------------------------------------------------------------
	//
	//  @brief		FPSを更新する
	//
	//------------------------------------------------------------------------------
	void FPSUpdate();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数の初期化
	//
	//------------------------------------------------------------------------------
	DX_FrameWork();

	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウクラスを削除し、メモリを解放
	//
	//------------------------------------------------------------------------------
	~DX_FrameWork();
};

#endif // !__DX_FRAMEWORK_H_
