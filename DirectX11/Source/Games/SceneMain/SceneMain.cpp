#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"

//-----------------------------------------------------------------------------------------
//
//  �R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain() :
	player(new Player())
{}

//-----------------------------------------------------------------------------------------
//
//  �f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(player);
}

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	bool result = false;

	result = player->Initialize();

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �X�V
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	bool result = false;

	m_pView->FreeCamera(2.0f);

	result = player->Update();

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_ESCAPE)) {
		DX_SceneManager::GetInstance()->GameEnd();
	}

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �`��
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Render()
{
	bool result = false;

	m_pView->Active();
	m_pView->Clear();

	result = player->Render();

	return result;
}