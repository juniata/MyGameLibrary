#pragma once

class DX_Graphics;
//****************************************************************************************************
//
//	DX_SceneManager
//	シーンを管理しています。
//	ロードシーンは自作してください。
//
//****************************************************************************************************
class SceneManager
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
	static void SetBootScene(Scene* pScene);

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
	static void Render();

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
	static void ChangeScene(Scene* pNextScene);

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
	SceneManager() {}

	//------------------------------------------------------------------------------
	//
	// @brief	デストラクタ
	//
	//------------------------------------------------------------------------------
	~SceneManager() {}

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