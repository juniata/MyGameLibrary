#include	"SkyLibrary/SkyLibrary.h"
#include	"SceneMain.h"

Object2D* p2D = nullptr;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMain::SceneMain()
{
	p2D = new Object2D();
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ---
//
//--------------------------------------------------------------------------------------
SceneMain::~SceneMain()
{
	DELETE_OBJ(p2D);
}

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMain::Initialize()
{
	auto result = false;
	result = p2D->Initialize("無題.png");
	
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

	p2D->Render();

	return result;
}
