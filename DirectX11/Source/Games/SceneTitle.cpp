#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneTitle.h"
#include	"SceneMenu.h"

using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject("SceneTitle\\title.png"))
{}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ
//
//-----------------------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	DELETE_OBJ(m_pTitle);
}

//-----------------------------------------------------------------------------------------
//
//	更新
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Update()
{
	// タイトル画面の時に左クリックをしたらメニュー画面を表示
	if (DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		DX_SceneManager::GetInstance()->ChangeScene(new SceneMenu());
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	描画
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Render()
{
	bool result = false;

	m_pView->Active();
	m_pView->Clear();
	result = m_pTitle->Render();

	return result;
}
