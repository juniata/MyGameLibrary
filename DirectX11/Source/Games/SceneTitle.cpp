#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"SceneTitle.h"


//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject("title.png")),
	m_pMenu(new DX_2DObject("menu.png")),
	m_IsRenderMenu(false)
{
	ZeroMemory(m_pButtonList, BUTTON_NUM);
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ
//
//-----------------------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	DELETE_OBJ(m_pTitle);
	DELETE_OBJ(m_pMenu);

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		DELETE_OBJ(m_pButtonList[i]);
	}
}

//-----------------------------------------------------------------------------------------
//
//	更新
//
//-----------------------------------------------------------------------------------------
bool SceneTitle::Update()
{
	// タイトル画面の時に左クリックをしたらメニュー画面を表示
	if (m_IsRenderMenu == false && DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		m_IsRenderMenu = true;
	}

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RETURN))
	{
		DX_SceneManager::GetInstance()->ChangeScene(new SceneMain());
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

	result = m_IsRenderMenu ? m_pMenu->Render() : m_pTitle->Render();

	return result;
}
