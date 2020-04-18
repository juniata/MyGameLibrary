#ifndef __DX_SCENE_MANAGER_H_
#define __DX_SCENE_MANAGER_H_

class DX_Graphics;
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
	static void SetBootScene(DX_Scene* pScene);

	//------------------------------------------------------------------------------
	//
	//  @brief	シーンを更新する
	//	@param	HWND	hWnd	ウィンドウハンドル	
	//	@param	UINT	message ウィンドウメッセージ
	//	@param	WPARAM  wParam ウィンドウズパラメーター
	//
	//------------------------------------------------------------------------------
	static void Update(const HWND hWnd, const UINT message, const WPARAM wParam);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	シーンを描画する
	 //	@param	pSwapChain	スワップチェイン
	 //
	 //------------------------------------------------------------------------------
	 static void Render(IDXGISwapChain* pSwapChain);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	ゲーム終了するかどうか
	 //	@return する場合はtrue
	 //
	 //------------------------------------------------------------------------------
	 static bool IsGameEnd();

	 //------------------------------------------------------------------------------
	 //
	 // @brief	シーンの変更を行う
	 //	@param pNextScene 次のシーン
	 //
	 //------------------------------------------------------------------------------
	 static void ChangeScene(DX_Scene* pNextScene);

	 //------------------------------------------------------------------------------
	 //
	 // @brief	ゲームを終了する
	 //
	 //------------------------------------------------------------------------------
	 static  void GameEnd();
private:
	enum class STATE : int {
		NONE,
		INIT,
		UPDATE,
		RENDER,
		END,
	};
	static Scene*	m_pCurScene;	//	現在のシーン
	static Scene*	m_pNextScene;	//	次のシーン
	static STATE m_state;

	//------------------------------------------------------------------------------
	//
	// @brief	コンストラクタ
	//
	//------------------------------------------------------------------------------
	DX_SceneManager() {}

	//------------------------------------------------------------------------------
	//
	// @brief	デストラクタ
	//
	//------------------------------------------------------------------------------
	~DX_SceneManager() {}

	//------------------------------------------------------------------------------
	//
	// @brief	シーンの変更を行う
	//
	//------------------------------------------------------------------------------
	static void ChangeScene();

	//------------------------------------------------------------------------------
	//
	// @brief	現在のシーンを破棄する
	//
	//------------------------------------------------------------------------------
	static void EndScene();
};
#endif // !__DX_SCENE_MANAGER_H_
