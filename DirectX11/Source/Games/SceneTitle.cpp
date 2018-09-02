#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"SceneTitle.h"

using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//	コンストラクタ
//
//-----------------------------------------------------------------------------------------
SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject("SceneTitle\\title.png")),
	m_pMenu(new DX_2DObject("SceneTitle\\menu.png")),
	m_IsRenderMenu(false)
{
	ZeroMemory(m_stageButtonList, BUTTON_NUM);

	AddStageButton();
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
		DELETE_OBJ(m_stageButtonList[i].pButton);
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

	if (m_IsRenderMenu) {
		for (int i = 0; i < BUTTON_NUM; ++i) {
			if (m_stageButtonList[i].pButton == nullptr) {
				continue;
			}

			if (m_stageButtonList[i].pButton->IsClickRelease()) {
				DX_SceneManager::GetInstance()->ChangeScene(new SceneMain());
			}
		}
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

	if (m_IsRenderMenu) {
		result = m_pMenu->Render();

		for (int i = 0; i < BUTTON_NUM; ++i) {
			if (m_stageButtonList[i].pButton == nullptr) {
				continue;
			}

			if (false == (result = m_stageButtonList[i].pButton->Render())) {
				break;
			}
		}
	}
	else {
		result = m_pTitle->Render();
	}

	return result;
}


//-----------------------------------------------------------------------------------------
//
//	ステージボタンを追加する
//
//-----------------------------------------------------------------------------------------
void SceneTitle::AddStageButton()
{
	const float renderposX = CAST_F(DX_System::GetInstance()->GetWindowWidth()) * 0.8f;
	const float renderposY = CAST_F(DX_System::GetInstance()->GetWindowHeight()) * 0.2f;

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		if (m_stageButtonList[i].pButton == nullptr) {			
			m_stageButtonList[i].pos.x = renderposX;
			m_stageButtonList[i].pos.y = renderposY;

			char filepath[_MAX_PATH] = { '0' };
			sprintf_s(filepath, "SceneTitle\\%s%i.png", "stage_button_", i + 1);
			m_stageButtonList[i].pButton = new DX_Button(filepath, m_stageButtonList[i].pos, XMFLOAT2(256,128));
			break;
		}
	}
}