#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_SceneManager*	DX_SceneManager::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
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
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_SceneManager::~DX_SceneManager()
{
	// TODO:���������[�N�̂��ߍ폜���邪�A�{���͂��Ȃ��B(����Ă͂����Ȃ�)
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
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
//	�C���X�^���X�̉�����s��
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�Q�[���N�����̃V�[����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::SetStartScene(DX_Scene* pScene)
{
	m_pCurScene = pScene;
	m_state = DX_SceneManager::STATE::INIT;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[�����X�V����
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
		//	�S�L�[�X�V
		DX_Input::Update(hWnd, message, wParam);

		// �V�[���̍X�V
		if (false == m_pCurScene->Update()) {
			// ���s������Q�[���I��
			ErrorGameEnd();
		}
		break;
	}
	
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����`�悷��
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Render(IDXGISwapChain* pSwapChain)
{
	switch (m_state)
	{
	case DX_SceneManager::STATE::UPDATE:

		//	�`��J�n
		DX_Graphics::BeginRender(pSwapChain);

		//	�V�[����`��
		if (false == m_pCurScene->Render()) {
			// ���s�Ȃ�Q�[���I��
			ErrorGameEnd();
		}

		//	�`��I��
		DX_Graphics::EndRender(pSwapChain);

		// ���̃V�[�����ݒ肳�ꂽ��V�[����ύX����(���݂̃V�[�����`�悳��I�������Ɏ��̃V�[���ւ̑J�ڂ��s��)
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
//	�Q�[���I�����ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_SceneManager::IsGameEnd()
{
	return m_isGameEnd;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����ύX����
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ChangeScene(DX_Scene* pNextScene)
{
	m_pNextScene = pNextScene;
}

//-----------------------------------------------------------------------------------------
//
//	�Q�[�����I������
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::GameEnd()
{
	m_state = DX_SceneManager::STATE::END;
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����J�ڂ���
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ChangeScene()
{
	// �j�������ɐ��������Ƃ������J�ڏ��������s
	if (m_pCurScene->Destroy()) {
		DELETE_OBJ(m_pCurScene);
		m_pCurScene = m_pNextScene;
		m_pNextScene = nullptr;
		m_state = DX_SceneManager::STATE::INIT;
	}
	
}

//-----------------------------------------------------------------------------------------
//
//	���݂̃V�[����j������
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
//	�Ȃɂ�����̏����Ɏ��s�������߃Q�[�����I������
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::ErrorGameEnd()
{
	m_isGameEnd = true;
	m_state = DX_SceneManager::STATE::NONE;
}
