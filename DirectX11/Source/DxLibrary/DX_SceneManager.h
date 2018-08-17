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
	static DX_Scene*	m_pCurScene;	//	���݂̃V�[��
	static DX_Scene*	m_pNextScene;	//	���̃V�[��

	DX_SceneManager() = delete;
public:
	~DX_SceneManager(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ�������������
	//	@param[in]	pCurScene	���݂̃V�[��
	//
	//------------------------------------------------------------------------------
	static void Initialize(
		DX_Scene* pCurScene
		);

	static void Release();
	//------------------------------------------------------------------------------
	//
	//  @brief		���݂̃V�[�����擾����
	//	@return		m_pCurScene
	//
	//------------------------------------------------------------------------------
	static DX_Scene* GetCurScene();
};
#endif // !__DX_SCENE_MANAGER_H_
