#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"

//-----------------------------------------------------------------------------------------
//
//  グローバル変数
//
//-----------------------------------------------------------------------------------------
DX_Box* g_box;
SceneMain::SceneMain()
{
}
SceneMain::~SceneMain()
{
	DELETE_OBJ(g_box);
	DELETE_OBJ(player);
}


//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	g_box = new DX_Box();
	player = new Player();

	g_box->SetPos(DirectX::XMFLOAT3(0.0f, 20.0f, -40.0f));
	g_box->Update();

	//DX_Lighting::Initialize();
	//OGGManager::LoadOGG(0, "Resource\\Sound\\1-0004370502.320.ogg");
	//OGGManager::Play(0);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	m_pView->FreeCamera(2.0f);

	player->Update();

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_ESCAPE)) {
		DX_SceneManager::GetInstance()->GameEnd();
	}
	return true;
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
void SceneMain::Render()
{
	m_pView->Active();
	m_pView->Clear();
	m_pView->SetMatrixForTheView();

	g_box->Render();
	player->Render();

}