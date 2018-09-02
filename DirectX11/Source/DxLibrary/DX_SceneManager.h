#ifndef __DX_SCENE_MANAGER_H_
#define __DX_SCENE_MANAGER_H_

//****************************************************************************************************
//
//	DX_SceneManager
//	シーンを管理しています。
//	ロードシーンは自作してください。
//
//****************************************************************************************************
class DX_SceneManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief	自身のインスタンスを取得する
	//	@return	new DX_SceneManager()
	//
	//------------------------------------------------------------------------------
	static DX_SceneManager* GetInstance();

	//------------------------------------------------------------------------------
	//
	//  @brief	インスタンスの開放を行う
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief	ゲーム起動時のシーンを設定
	//	@param	pScene	シーン
	//
	//------------------------------------------------------------------------------
	void SetStartScene(DX_Scene* pScene);

	//------------------------------------------------------------------------------
	//
	//  @brief	シーンを更新する
	//	@param	HWND	hWnd	ウィンドウハンドル	
	//	@param	UINT	message ウィンドウメッセージ
	//	@param	WPARAM  wParam ウィンドウズパラメーター
	//
	//------------------------------------------------------------------------------
	 void Update(const HWND hWnd, const UINT message, const WPARAM wParam);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	シーンを描画する
	 //	@param	pSwapChain	スワップチェイン
	 //
	 //------------------------------------------------------------------------------
	 void Render(IDXGISwapChain* pSwapChain);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	ゲーム終了するかどうか
	 //	@return する場合はtrue
	 //
	 //------------------------------------------------------------------------------
	 bool IsGameEnd();

	 //------------------------------------------------------------------------------
	 //
	 // @brief	シーンの変更を行う
	 //	@param pNextScene 次のシーン
	 //
	 //------------------------------------------------------------------------------
	 void ChangeScene(DX_Scene* pNextScene);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	ゲームを終了する
	 //
	 //------------------------------------------------------------------------------
	 void GameEnd();
private:
	enum class STATE : int {
		NONE,
		INIT,
		UPDATE,
		END,
	};
	static DX_SceneManager* m_pInstance;
	DX_Scene*	m_pCurScene;	//	現在のシーン
	DX_Scene*	m_pNextScene;	//	次のシーン
	STATE m_state;
	bool m_isGameEnd;

	//------------------------------------------------------------------------------
	//
	// @brief	コンストラクタ
	//
	//------------------------------------------------------------------------------
	DX_SceneManager();

	//------------------------------------------------------------------------------
	//
	// @brief	デストラクタ
	//
	//------------------------------------------------------------------------------
	~DX_SceneManager();

	//------------------------------------------------------------------------------
	//
	// @brief	シーンの変更を行う
	//
	//------------------------------------------------------------------------------
	void ChangeScene();

	//------------------------------------------------------------------------------
	//
	// @brief	現在のシーンを破棄する
	//
	//------------------------------------------------------------------------------
	void EndScene();

	//------------------------------------------------------------------------------
	//
	// @brief	何かしらの処理に失敗したためゲームを終了する
	//
	//------------------------------------------------------------------------------
	void ErrorGameEnd();
};
#endif // !__DX_SCENE_MANAGER_H_
