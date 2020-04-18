#include	"SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
Scene*				SceneManager::m_pCurScene = nullptr;	//	現在のシーン
Scene*				SceneManager::m_pNextScene = nullptr;	//	次のシーン
SceneManager::STATE	SceneManager::m_state = SceneManager::STATE::NONE;

//-----------------------------------------------------------------------------------------
//
//	解放を行う
//
//-----------------------------------------------------------------------------------------
void SceneManager::Release()
{
	// TODO:メモリリークのため削除するが、本来はしない。(されてはいけない)
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	ゲーム起動時のシーンを設定
//
//-----------------------------------------------------------------------------------------
void SceneManager::SetBootScene(Scene* pScene)
{
	m_pCurScene = pScene;
	m_state = SceneManager::STATE::INIT;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを更新する
//
//-----------------------------------------------------------------------------------------
void SceneManager::Update(const HWND hWnd, const UINT message, const WPARAM wParam)
{
	switch (m_state)
	{
	case SceneManager::STATE::INIT:
		if (m_pCurScene->Initialize()) {
			m_state = SceneManager::STATE::UPDATE;
		}
		else {
			GameEnd();
			break;
		}

	case SceneManager::STATE::UPDATE:
		DX_Input::Update(hWnd, message, wParam);
		m_pCurScene->Update() ? m_state = SceneManager::STATE::RENDER : GameEnd();
		break;
	}

}

//-----------------------------------------------------------------------------------------
//
//	シーンを描画する
//
//-----------------------------------------------------------------------------------------
void SceneManager::Render(IDXGISwapChain* pSwapChain)
{
	switch (m_state)
	{
	case SceneManager::STATE::RENDER:

		//	描画開始
		DX_Graphics::BeginRender(pSwapChain);

		//	シーンを描画
		if (false == m_pCurScene->Render()) {
			// 失敗ならゲーム終了
			GameEnd();
		}
		else {
			m_state = SceneManager::STATE::UPDATE;
		}

		//	描画終了
		DX_Graphics::EndRender(pSwapChain);

		// 次のシーンが設定されたらシーンを変更する(現在のシーンが描画され終わった後に次のシーンへの遷移を行う)
		if (m_pNextScene) {
			ChangeScene();
		}
		break;
	case SceneManager::STATE::END:
		EndScene();
	}

}

//-----------------------------------------------------------------------------------------
//
//	ゲーム終了かどうか
//
//-----------------------------------------------------------------------------------------
bool SceneManager::IsGameEnd()
{
	return m_state == SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを変更する
//
//-----------------------------------------------------------------------------------------
void SceneManager::ChangeScene(Scene* pNextScene)
{
	m_pNextScene = pNextScene;
}

//-----------------------------------------------------------------------------------------
//
//	ゲームを終了する
//
//-----------------------------------------------------------------------------------------
void SceneManager::GameEnd()
{
	m_state = SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを遷移する
//
//-----------------------------------------------------------------------------------------
void SceneManager::ChangeScene()
{
	// 破棄処理に成功したときだけ遷移処理を実行
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
		m_pCurScene = m_pNextScene;
		m_pNextScene = nullptr;
		m_state = SceneManager::STATE::INIT;
	}
	else {
		GameEnd();
	}

}

//-----------------------------------------------------------------------------------------
//
//	現在のシーンを破棄する
//
//-----------------------------------------------------------------------------------------
void SceneManager::EndScene()
{
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
	}
}
