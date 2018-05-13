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
	static dx_library::Scene*	m_pCurScene;	//	���݂̃V�[��
	static dx_library::Scene*	m_pNextScene;	//	���̃V�[��

	SceneManager() = delete;
public:
	~SceneManager(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ�������������
	//	@param[in]	pCurScene	���݂̃V�[��
	//
	//------------------------------------------------------------------------------
	static void Initialize(
		dx_library::Scene* pCurScene
		);

	
	//------------------------------------------------------------------------------
	//
	//  @brief		���݂̃V�[�����擾����
	//	@return		m_pCurScene
	//
	//------------------------------------------------------------------------------
	static dx_library::Scene* GetCurScene();
};
#endif // !__DX_SCENE_MANAGER_H_
