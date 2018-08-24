#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"user_helper_class\OGGManager.h"

#define ISNTANCE_MESH_MAX 1000
//-----------------------------------------------------------------------------------------
//
//  グローバル変数
//
//-----------------------------------------------------------------------------------------
DX_View*			g_pView		= nullptr;
DX_2DObject* g_tex = nullptr;
DX_2DObject* g_tex2 = nullptr;
DX_Box*	g_box = nullptr;

SceneMain::SceneMain()
{
}
SceneMain::~SceneMain()
{
	DELETE_OBJ(g_box);
	DELETE_OBJ(g_tex);
	DELETE_OBJ(g_tex2);
	DELETE_OBJ(g_pView);
	DELETE_OBJ(player);
}
DirectX::XMFLOAT3 angle(0.0f,0.0f,0.0f);

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
	g_box = new DX_Box();
	g_box->SetScale(DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f));
	g_box->SetPos(DirectX::XMFLOAT3(0.0f, 20.0f, 0.0f));

	player = new Player();

	//DX_Lighting::Initialize();
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

	player->Update();

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		angle.y += 0.1f;
		if (angle.y > 360.0f) {
			angle.y = -360.0f;
		}
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D)) {
		angle.y -= 0.1f;
		if (angle.y < -360.0f) {
			angle.y = 360.0f;
		}
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_W)) {
		angle.x += 0.1f;
		if (angle.x > 360.0f) {
			angle.x = -360.0f;
		}
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_S)) {
		angle.x -= 0.1f;
		if (angle.x < -360.0f) {
			angle.x = 360.0f;
		}
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_Z)) {
		angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	g_box->SetAngle(DirectX::XMFLOAT3(angle.x, angle.y, angle.z));
	g_box->Update();
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

	//g_tex->Render(tagRect(0, 0, CAST_F(DX_System::GetWindowWidth()), CAST_F(DX_System::GetWindowHeight())));

	player->Render();
	g_box->Render();
}