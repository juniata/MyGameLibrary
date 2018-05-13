#ifndef __DX_FRAMEWORK_H_
#define __DX_FRAMEWORK_H_

//****************************************************************************************************
//
//	FrameWork
//
//****************************************************************************************************
class DX_FrameWork
{
private:
	char*		m_pAppName;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	struct tagFPS{
		int		fps;
		int		count;
		float	deltaTime;
		unsigned long startTime;
	}m_fps;
public:
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

private:
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
	bool CreateAppWindow(
		char* pAppName,
		const int x,
		const int y,
		const int width,
		const int height
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		送られたメッセージを元に様々な処理を行う
	//  @param[in]	hWnd	引数はあるが、自身で値を設定しない
	//  @param[in]	msg		同上
	//  @param[in]	wparam	同上
	//  @param[in]	lparam	同上
	//	@return		上と同じ理由で、知る必要が無し
	//
	//------------------------------------------------------------------------------
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT msg,
		WPARAM wparam, 
		LPARAM lparam
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		FPSを更新する
	//
	//------------------------------------------------------------------------------
	void FPSUpdate();
};

#endif // !__DX_FRAMEWORK_H_
