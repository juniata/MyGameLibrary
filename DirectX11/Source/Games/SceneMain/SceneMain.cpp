#include	"DxLibrary\DX_Library.h"
#include	"SceneMain.h"
using namespace DirectX;

DX_Box* pBox = nullptr;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain() :
	m_pTest(new DX_2DObject())
{
	pBox = new DX_Box();
	pBox->SetPos(XMFLOAT3(0.0f, 0.0f, 20.0f));
	pBox->SetScale(XMFLOAT3(10.0f, 10.0f, 10.0f));
	pBox->Update();
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ---
//
//--------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(m_pTest);
	DELETE_OBJ(pBox);
}

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	bool result = false;

	result = m_pTest->Initialize("mapchip.png");
	
//	m_pTest->SetUV(0.0f, 0.0f, 64.0f, 64.0f);
	//m_pTest->SetUV(64.0f, 0.0f, 128.0f, 64.0f);
	m_pTest->Update(true, true);
	
	return result;
}

//-----------------------------------------------------------------------------------------
//
//	更新
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Update()
{
	bool result = true;
	m_pView->SetTarget(pBox->GetPos());

	return result;
}


//-----------------------------------------------------------------------------------------
//
//	描画
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Render()
{
	bool result = true;

	m_pView->Active();
	m_pView->Clear();
	//result = m_pTest->Render();
	pBox->Render();

	return result;
}
