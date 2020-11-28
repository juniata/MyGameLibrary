#include	"SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
Scene*				SceneManager::m_pCurScene = nullptr;	//	���݂̃V�[��
Scene*				SceneManager::m_pNextScene = nullptr;	//	���̃V�[��
SceneManager::STATE	SceneManager::m_state = SceneManager::STATE::NONE;

//-----------------------------------------------------------------------------------------
//
//	������s��
//
//-----------------------------------------------------------------------------------------
void SceneManager::Release()
{
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	�Q�[���N�����̃V�[����ݒ�
//
//-----------------------------------------------------------------------------------------
void SceneManager::SetBootScene(Scene* pScene)
{
	m_pCurScene = pScene;
	m_state = SceneManager::STATE::INIT;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[�����X�V����
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
		DX_Input::GetInstance()->Update(hWnd, message, wParam);
		m_pCurScene->Update() ? m_state = SceneManager::STATE::RENDER : GameEnd();
		break;
	}

}

//-----------------------------------------------------------------------------------------
//
//	�V�[����`�悷��
//
//-----------------------------------------------------------------------------------------
void SceneManager::Render()
{
	DX_Graphics* graphics = DX_Graphics::GetInstance();

	switch (m_state)
	{
	case SceneManager::STATE::RENDER:

		//	�`��J�n
		graphics->BeginRender();

		//	�V�[����`��
		if (m_pCurScene->Render() == false)
		{
			// ���s�Ȃ�Q�[���I��
			GameEnd();
		}
		else
		{
			m_state = SceneManager::STATE::UPDATE;
		}

		//	�`��I��
		graphics->EndRender();

		// ���̃V�[�����ݒ肳�ꂽ��V�[����ύX����(���݂̃V�[�����`�悳��I�������Ɏ��̃V�[���ւ̑J�ڂ��s��)
		if (m_pNextScene)
		{
			ChangeScene();
		}
		break;
	case SceneManager::STATE::END:
		EndScene();
	}

}

//-----------------------------------------------------------------------------------------
//
//	�Q�[���I�����ǂ���
//
//-----------------------------------------------------------------------------------------
bool SceneManager::IsGameEnd()
{
	return m_state == SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����ύX����
//
//-----------------------------------------------------------------------------------------
void SceneManager::ChangeScene(Scene* pNextScene)
{
	m_pNextScene = pNextScene;
}

//-----------------------------------------------------------------------------------------
//
//	�Q�[�����I������
//
//-----------------------------------------------------------------------------------------
void SceneManager::GameEnd()
{
	m_state = SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����J�ڂ���
//
//-----------------------------------------------------------------------------------------
void SceneManager::ChangeScene()
{
	// �j�������ɐ��������Ƃ������J�ڏ��������s
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
//	���݂̃V�[����j������
//
//-----------------------------------------------------------------------------------------
void SceneManager::EndScene()
{
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
	}
}
