#include	"DX_LIBRARY\DX_Library.h"
#include	"Bullet.h"
#include	"SceneMain.h"
#include	"user_helper_class\OGGManager.h"

#define ISNTANCE_MESH_MAX 1000
//-----------------------------------------------------------------------------------------
//
//  グローバル変数
//
//-----------------------------------------------------------------------------------------
DX_View*			g_pView		= nullptr;
DX_Mesh*			g_pMesh		= nullptr;
DX_2DObject* g_tex = nullptr;
DX_2DObject* g_tex2 = nullptr;

SceneMain::SceneMain()
{
}
SceneMain::~SceneMain()
{
	DELETE_OBJ(g_pMesh);
	DELETE_OBJ(g_tex);
	DELETE_OBJ(g_tex2);
	DELETE_OBJ(g_pView);
	DELETE_OBJ(bullet);
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
	bullet = new Bullet();
	DX_Lighting::Initialize();
	//OGGManager::LoadOGG(0, "Resource\\Sound\\1-0004370502.320.ogg");
	//OGGManager::Play(0);

	return true;
}
DirectX::XMFLOAT3 pos;
//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	g_pView->FreeCamera(2.0f);

	bullet->Update();

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
void SceneMain::Render()
{
	g_pView->Clear();
	g_pView->Active();

	bullet->Render();
	g_tex->Render(tagRect(0, 0, DX_System::GetWindowWidth(), DX_System::GetWindowHeight()));
}