#ifndef __DX_SCENE_MANAGER_H_
#define __DX_SCENE_MANAGER_H_

//****************************************************************************************************
//
//	SceneManager
//
//****************************************************************************************************
class DX_SceneManager
{
private:
	static DX_Scene*	m_pCurScene;	//	現在のシーン
	static DX_Scene*	m_pNextScene;	//	次のシーン

	DX_SceneManager() = delete;
public:
	~DX_SceneManager(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化する
	//	@param[in]	pCurScene	現在のシーン
	//
	//------------------------------------------------------------------------------
	static void Initialize(
		DX_Scene* pCurScene
		);

	static void Release();
	//------------------------------------------------------------------------------
	//
	//  @brief		現在のシーンを取得する
	//	@return		m_pCurScene
	//
	//------------------------------------------------------------------------------
	static DX_Scene* GetCurScene();
};
#endif // !__DX_SCENE_MANAGER_H_
