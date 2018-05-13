#ifndef __DX_SCENE_MANAGER_H_
#define __DX_SCENE_MANAGER_H_

//****************************************************************************************************
//
//	SceneManager
//
//****************************************************************************************************
class SceneManager
{
private:
	static dx_library::Scene*	m_pCurScene;	//	現在のシーン
	static dx_library::Scene*	m_pNextScene;	//	次のシーン

	SceneManager() = delete;
public:
	~SceneManager(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化する
	//	@param[in]	pCurScene	現在のシーン
	//
	//------------------------------------------------------------------------------
	static void Initialize(
		dx_library::Scene* pCurScene
		);

	
	//------------------------------------------------------------------------------
	//
	//  @brief		現在のシーンを取得する
	//	@return		m_pCurScene
	//
	//------------------------------------------------------------------------------
	static dx_library::Scene* GetCurScene();
};
#endif // !__DX_SCENE_MANAGER_H_
