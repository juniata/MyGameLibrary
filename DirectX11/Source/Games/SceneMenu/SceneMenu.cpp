#include	"DxLibrary\DX_Library.h"
#include	"SceneMenu.h"
#include	"../SceneMain\SceneMain.h"
#include	<stdio.h>

using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMenu::SceneMenu() :
	m_pMenu(new DX_2DObject())
{
	ZeroMemory(m_stageButtonList, BUTTON_NUM);
}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ
//
//-----------------------------------------------------------------------------------------
SceneMenu::~SceneMenu()
{
	DELETE_OBJ(m_pMenu);

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		DELETE_OBJ(m_stageButtonList[i].pButton);
	}
}

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool SceneMenu::Initialize()
{
	bool result = false;

	result = m_pMenu->Initialize("SceneMenu\\menu.png");
	result = AddStageButton();

	return result;
}
//-----------------------------------------------------------------------------------------
//
//	更新
//
//-----------------------------------------------------------------------------------------
bool SceneMenu::Update()
{
	for (int i = 0; i < BUTTON_NUM; ++i) {
		if (m_stageButtonList[i].pButton == nullptr) {
			continue;
		}

		if (m_stageButtonList[i].pButton->IsClickRelease()) {
			DX_SceneManager::GetInstance()->ChangeScene(new SceneMain());
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	描画
//
//-----------------------------------------------------------------------------------------
bool SceneMenu::Render()
{
	bool result = false;

	m_pView->Active();
	m_pView->Clear();

	result = m_pMenu->Render();

	for (int i = 0; i < BUTTON_NUM; ++i) {
		if (m_stageButtonList[i].pButton == nullptr) {
			continue;
		}

		if (false == (result = m_stageButtonList[i].pButton->Render())) {
			break;
		}
	}

	return result;
}


//-----------------------------------------------------------------------------------------
//
//	ステージボタンを追加する
//
//-----------------------------------------------------------------------------------------
bool SceneMenu::AddStageButton()
{
	bool result = false;

	const float renderposX = CAST_F(DX_System::GetInstance()->GetWindowWidth()) * 0.8f;
	const float renderposY = CAST_F(DX_System::GetInstance()->GetWindowHeight()) * 0.2f;

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		if (m_stageButtonList[i].pButton == nullptr) {
			m_stageButtonList[i].pos.x = renderposX;
			m_stageButtonList[i].pos.y = renderposY;

			char filepath[_MAX_PATH] = { '0' };
			sprintf_s(filepath, "SceneMenu\\%s%i.png", "stage_button_", i + 1);
			m_stageButtonList[i].pButton = new DX_Button();
			result = m_stageButtonList[i].pButton->Initialize(filepath, m_stageButtonList[i].pos, XMFLOAT2(256, 128));
			break;
		}
	}

	return result;
}