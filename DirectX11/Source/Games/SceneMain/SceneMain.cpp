#include	"DxLibrary\DX_Library.h"
#include	"SceneMain.h"
using namespace DirectX;

DX_Box* pBox = nullptr;
DX_2DObject* p2D = nullptr;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain()
{
	pBox = new DX_Box();
	p2D = new DX_2DObject();
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ---
//
//--------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(p2D);
	DELETE_OBJ(pBox);
}

DirectX::XMFLOAT3 pos(1.0f, 1.0f, 1.0f);
//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	auto result = false;
	result = pBox->Initialize();
	result = p2D->Initialize("mapchip.png");
	m_pView->SetTarget(pBox->GetPos());

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


	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		pos.x -= 1.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_X)) {
		pos.x += 1.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_W)) {
		pos.z += 1.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_S)) {
		pos.z -= 1.0f;
	}
	m_pView->SetPos(pos);
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

//	p2D->Render();
	pBox->Render();

	return result;
}
