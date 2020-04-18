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
	// TODO:���������[�N�̂��ߍ폜���邪�A�{���͂��Ȃ��B(����Ă͂����Ȃ�)
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
		DX_Input::Update(hWnd, message, wParam);
		m_pCurScene->Update() ? m_state = SceneManager::STATE::RENDER : GameEnd();
		break;
	}

}

//-----------------------------------------------------------------------------------------
//
//	�V�[����`�悷��
//
//-----------------------------------------------------------------------------------------
void SceneManager::Render(IDXGISwapChain* pSwapChain)
{
	switch (m_state)
	{
	case SceneManager::STATE::RENDER:

		//	�`��J�n
		DX_Graphics::BeginRender(pSwapChain);

		//	�V�[����`��
		if (false == m_pCurScene->Render()) {
			// ���s�Ȃ�Q�[���I��
			GameEnd();
		}
		else {
			m_state = SceneManager::STATE::UPDATE;
		}

		//	�`��I��
		DX_Graphics::EndRender(pSwapChain);

		// ���̃V�[�����ݒ肳�ꂽ��V�[����ύX����(���݂̃V�[�����`�悳��I�������Ɏ��̃V�[���ւ̑J�ڂ��s��)
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
