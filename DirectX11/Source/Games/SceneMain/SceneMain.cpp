#include	"SkyLibrary/SkyLibrary.h"
#include	"SceneMain.h"

Object3D* obj3D = nullptr;
DirectX::XMFLOAT3 objAngle;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain()
{
	//obj3D = new Object3D();
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ---
//
//--------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	//DELETE_OBJ(obj3D);
}

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	auto result = true;

	//obj3D->Initialize("untitled.fbx");
	//obj3D->Initialize("human//rp_nathan_animated_003_walking.fbx");

	//objAngle = obj3D->GetAngle();
	//m_pView->SetPos(DirectX::XMFLOAT3(0.0f, 100.0f, -100.0f));
	
//	m_pView->SetPos(DirectX::XMFLOAT3(0.0f, 5.0f, -5.0f));
	//m_pView->SetTarget(obj3D->GetPos());

	//Lighting::Initialize();

	return result;
}

//-----------------------------------------------------------------------------------------
//
//	更新
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	auto result = true;

	DX_Input* input = DX_Input::GetInstance();
	if (input->IsKeyDown(DX_INPUT_KEY::DX_W)) {
		objAngle.x +=  (3.14f / 180.0f)* 2.0f;
	}
	if (input->IsKeyDown(DX_INPUT_KEY::DX_S)) {
		objAngle.x -= (3.14f / 180.0f)* 2.0f;
	}
	if (input->IsKeyDown(DX_INPUT_KEY::DX_A)) {
		objAngle.z += (3.14f / 180.0f)* 2.0f;
	}
	if (input->IsKeyDown(DX_INPUT_KEY::DX_D)) {
		objAngle.z -= (3.14f / 180.0f) * 2.0f;
	}

	//obj3D->SetAngle(objAngle);
	//obj3D->Update();

	return result;
}


//-----------------------------------------------------------------------------------------
//
//	描画
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Render()
{
	auto result = true;

	m_pView->Active();
	m_pView->Clear();
	m_pView->SetMatrixForTheView();
	
	//DX_Text::GetInstance()->Print(0, 10, 20, 20, "x=%s", "あ");
	//obj3D->Render();

	return result;
}
