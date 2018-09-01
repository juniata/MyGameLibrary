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
	// TODO:メモリリークのため削除するが、本来はされない。(されてはいけない)
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	シーンを遷移する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ChangeScene()
{
	DELETE_OBJ(m_pCurScene);
	m_pCurScene = m_pNextScene;
	m_pNextScene = nullptr;
	m_state = DX_SceneManager::STATE::NONE;
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
}

//-----------------------------------------------------------------------------------------
//
//	シーンを更新する
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Update(const UINT message, const WPARAM wParam)
{
	switch (m_state)
	{
	case DX_SceneManager::STATE::NONE:
		m_state = DX_SceneManager::STATE::INIT;
	case DX_SceneManager::STATE::INIT:
		m_pCurScene->Initialize();
		m_state = DX_SceneManager::STATE::UPDATE;
		break;
	case DX_SceneManager::STATE::UPDATE:
		//	全キー更新
		DX_Input::Update(message, wParam);

		// シーンの更新
		m_pCurScene->Update();
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

		m_pCurScene->Render();

		//	描画終了
		DX_Graphics::EndRender(pSwapChain);

		// 次のシーンが設定されたらシーンを変更する
		if (m_pNextScene) {
			ChangeScene();
		}
		break;
	case DX_SceneManager::STATE::END:
		DELETE_OBJ(m_pCurScene);
		m_isGameEnd = true;
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