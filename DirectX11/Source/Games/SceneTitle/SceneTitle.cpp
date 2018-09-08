#include	"DxLibrary\DX_Library.h"
#include	"SceneTitle.h"
#include	"../SceneMenu/SceneMenu.h"
using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//	�R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject())
{}

//-----------------------------------------------------------------------------------------
//
//	�f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	DELETE_OBJ(m_pTitle);
}

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Initialize()
{
	bool result = false;

	result = m_pTitle->Initialize("SceneTitle\\title.png");

	return result;
}

//-----------------------------------------------------------------------------------------
//
//	�X�V
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Update()
{
	bool result = true;

	// �^�C�g����ʂ̎��ɍ��N���b�N�������烁�j���[��ʂ�\��
	if (DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		DX_SceneManager::GetInstance()->ChangeScene(new SceneMenu());
	}

	return result;
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
	result = m_pTitle->Render();

	return result;
}
