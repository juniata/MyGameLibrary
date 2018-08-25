#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"user_helper_class\OGGManager.h"

#define ISNTANCE_MESH_MAX 1000
#define BOX_NUM 1000
//-----------------------------------------------------------------------------------------
//
//  グローバル変数
//
//-----------------------------------------------------------------------------------------
DX_View*			g_pView		= nullptr;
DX_2DObject* g_tex = nullptr;
DX_2DObject* g_tex2 = nullptr;

SceneMain::SceneMain()
{
}
SceneMain::~SceneMain()
{
	DELETE_OBJ(g_tex);
	DELETE_OBJ(g_tex2);
	DELETE_OBJ(g_pView);
	DELETE_OBJ(player);
}


//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	g_pView = new DX_View();

	g_tex = new DX_2DObject("14_s2bgay4bpz.jpg");

	g_tex2 = new DX_2DObject("14_s2bgay4bpz.jpg");

	player = new Player();

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
	g_pView->FreeCamera(2.0f);

	player->Update();

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
void SceneMain::Render()
{
	g_pView->Active();
	g_pView->Clear();
	g_pView->SetMatrixForTheView();

	//g_tex->Render(tagRect(0, 0, CAST_F(DX_System::GetWindowWidth()), CAST_F(DX_System::GetWindowHeight())));

	player->Render();

}