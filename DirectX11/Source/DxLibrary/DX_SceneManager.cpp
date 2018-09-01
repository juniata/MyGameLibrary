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
	// TODO:���������[�N�̂��ߍ폜���邪�A�{���͂���Ȃ��B(����Ă͂����Ȃ�)
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}

//-----------------------------------------------------------------------------------------
//
//	�V�[����J�ڂ���
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
}

//-----------------------------------------------------------------------------------------
//
//	�V�[�����X�V����
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
		//	�S�L�[�X�V
		DX_Input::Update(message, wParam);

		// �V�[���̍X�V
		m_pCurScene->Update();
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

		m_pCurScene->Render();

		//	�`��I��
		DX_Graphics::EndRender(pSwapChain);

		// ���̃V�[�����ݒ肳�ꂽ��V�[����ύX����
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