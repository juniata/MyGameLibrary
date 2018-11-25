#include	"DxLibrary\DX_Library.h"
#include	"Stage.h"
#include	"Games/Player/Player.h"
#include	"SceneMain.h"

Stage* stage = nullptr;
Player* player = nullptr;
//-----------------------------------------------------------------------------------------
//
//  コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain()
{
	stage = new Stage();
	player = new Player();
}

//-----------------------------------------------------------------------------------------
//
//  デストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(stage);
	DELETE_OBJ(player);
}

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	bool result = false;

	result = stage->Initialize();
	result = player->Initialize(stage->GetInitPlayerPos());

	return result;
}
//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	bool result = true;

	m_pView->FreeCamera(2.0f);

	result = player->Update();

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_ESCAPE)) {
		DX_SceneManager::GetInstance()->GameEnd();
	}


	return result;
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Render()
{
	bool result = false;

	m_pView->Active();
	m_pView->Clear();

	result = stage->Render();
	result = player->Render();

	return result;
}