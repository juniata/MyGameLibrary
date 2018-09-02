#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_SceneManager*	DX_SceneManager::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_SceneManager::DX_SceneManager() :
	m_pCurScene(nullptr),
	m_pNextScene(nullptr),
	m_state(DX_SceneManager::STATE::NONE),
	m_isGameEnd(false)
{}

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_SceneManager::~DX_SceneManager()
{
	// TODO:メモリリークのため削除するが、本来はしない。(されてはいけない)
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_SceneManager* DX_SceneManager::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_SceneManager();
	}

	return m_pInstance;
}
//-----------------------------------------------------------------------------------------
//
//	インスタンスの解放を行う
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	ゲーム起動時のシーンを設定
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::SetStartScene(DX_Scene* pScene)
{
	m_pCurScene = pScene;
	m_state = DX_SceneManager::STATE::INIT;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを更新する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Update(const HWND hWnd, const UINT message, const WPARAM wParam)
{
	switch (m_state)
	{
	case DX_SceneManager::STATE::INIT:
		m_pCurScene->Initialize();
		m_state = DX_SceneManager::STATE::UPDATE;
	case DX_SceneManager::STATE::UPDATE:
		//	全キー更新
		DX_Input::Update(hWnd, message, wParam);

		// シーンの更新
		if (false == m_pCurScene->Update()) {
			// 失敗したらゲーム終了
			ErrorGameEnd();
		}
		break;
	}
	
}

//-----------------------------------------------------------------------------------------
//
//	シーンを描画する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Render(IDXGISwapChain* pSwapChain)
{
	switch (m_state)
	{
	case DX_SceneManager::STATE::UPDATE:

		//	描画開始
		DX_Graphics::BeginRender(pSwapChain);

		//	シーンを描画
		if (false == m_pCurScene->Render()) {
			// 失敗ならゲーム終了
			ErrorGameEnd();
		}

		//	描画終了
		DX_Graphics::EndRender(pSwapChain);

		// 次のシーンが設定されたらシーンを変更する(現在のシーンが描画され終わった後に次のシーンへの遷移を行う)
		if (m_pNextScene) {
			ChangeScene();
		}
		break;
	case DX_SceneManager::STATE::END:
		EndScene();
	}
	
}

//-----------------------------------------------------------------------------------------
//
//	ゲーム終了かどうか
//
//-----------------------------------------------------------------------------------------
bool DX_SceneManager::IsGameEnd()
{
	return m_isGameEnd;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを変更する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ChangeScene(DX_Scene* pNextScene)
{
	m_pNextScene = pNextScene;
}

//-----------------------------------------------------------------------------------------
//
//	ゲームを終了する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::GameEnd()
{
	m_state = DX_SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	シーンを遷移する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ChangeScene()
{
	// 破棄処理に成功したときだけ遷移処理を実行
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
		m_pCurScene = m_pNextScene;
		m_pNextScene = nullptr;
		m_state = DX_SceneManager::STATE::INIT;
	}
	
}

//-----------------------------------------------------------------------------------------
//
//	現在のシーンを破棄する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::EndScene()
{
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
		m_isGameEnd = true;
	}
}

//-----------------------------------------------------------------------------------------
//
//	なにかしらの処理に失敗したためゲームを終了する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ErrorGameEnd()
{
	m_isGameEnd = true;
	m_state = DX_SceneManager::STATE::NONE;
}
