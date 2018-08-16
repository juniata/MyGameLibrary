#ifndef __DX_SCENE_MANAGER_H_
#define __DX_SCENE_MANAGER_H_

//****************************************************************************************************
//
//	SceneManager
//
//****************************************************************************************************
class DX_SceneManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化する
	//	@param[in]	pCurScene	現在のシーン
	//
	//------------------------------------------------------------------------------
	void Initialize(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	static void Release();
	
	void Update(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Render(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static DX_SceneManager* GetInstance();
private:
	static DX_SceneManager* m_pInstance;

	DX_Scene*	m_pCurScene;	//	現在のシーン
	DX_Scene*	m_pNextScene;	//	次のシーン

	DX_SceneManager();
	~DX_SceneManager();
};
#endif // !__DX_SCENE_MANAGER_H_
