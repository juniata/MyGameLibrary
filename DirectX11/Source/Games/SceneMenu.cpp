#include	"DxLibrary\DX_Library.h"
#include	"SceneMenu.h"
#include	"SceneMain.h"

using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//	�R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
SceneMenu::SceneMenu() :
	m_pMenu(new DX_2DObject("SceneTitle\\menu.png"))
{
	ZeroMemory(m_stageButtonList, BUTTON_NUM);

	AddStageButton();
}

//-----------------------------------------------------------------------------------------
//
//	�f�X�g���N�^
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
//	�X�V
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
//	�`��
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
//	�X�e�[�W�{�^����ǉ�����
//
//-----------------------------------------------------------------------------------------
void SceneMenu::AddStageButton()
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
			m_stageButtonList[i].pButton = new DX_Button(filepath, m_stageButtonList[i].pos, XMFLOAT2(256, 128));
			break;
		}
	}
}