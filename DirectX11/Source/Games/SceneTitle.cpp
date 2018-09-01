#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"SceneTitle.h"


//-----------------------------------------------------------------------------------------
//
//	�R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject("title.png")),
	m_pMenu(new DX_2DObject("menu.png")),
	m_IsRenderMenu(false)
{
	ZeroMemory(m_pButtonList, BUTTON_NUM);
}

//-----------------------------------------------------------------------------------------
//
//	�f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	DELETE_OBJ(m_pTitle);
	DELETE_OBJ(m_pMenu);

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		DELETE_OBJ(m_pButtonList[i]);
	}
}

//-----------------------------------------------------------------------------------------
//
//	�X�V
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Update()
{
	// �^�C�g����ʂ̎��ɍ��N���b�N�������烁�j���[��ʂ�\��
	if (m_IsRenderMenu == false && DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		m_IsRenderMenu = true;
	}

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RETURN))
	{
		DX_SceneManager::GetInstance()->ChangeScene(new SceneMain());
	}
	return true;
}


//-----------------------------------------------------------------------------------------
//
//	�`��
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Render()
{
	bool result = false;

	m_pView->Active();
	m_pView->Clear();

	result = m_IsRenderMenu ? m_pMenu->Render() : m_pTitle->Render();

	return result;
}
