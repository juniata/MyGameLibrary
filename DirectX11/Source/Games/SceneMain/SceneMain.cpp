#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"Enemy.h"
#include	"EnemyManager.h"

//-----------------------------------------------------------------------------------------
//
//  �R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain() :
	player(new Player()),
	m_pEnemyManager(new EnemyManager())
{}

//-----------------------------------------------------------------------------------------
//
//  �f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(player);
	DELETE_OBJ(m_pEnemyManager);
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

	result = m_pEnemyManager->Update(player->GetPos());

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
	result = m_pEnemyManager->Render();

	return result;
}