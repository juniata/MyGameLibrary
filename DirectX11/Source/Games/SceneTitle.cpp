#include	"DxLibrary\DX_Library.h"
#include	"Player.h"
#include	"SceneMain.h"
#include	"SceneTitle.h"

SceneTitle::SceneTitle() : 
	m_pTitle(new DX_2DObject("title.png"))
{
}

SceneTitle::~SceneTitle()
{
	DELETE_OBJ(m_pTitle);
}

bool SceneTitle::Initialize()
{
	return true;
}

bool SceneTitle::Update()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RETURN))
	{
		DX_SceneManager::GetInstance()->ChangeScene(new SceneMain());
	}
	return true;
}
void SceneTitle::Render()
{
	m_pView->Active();
	m_pView->Clear();

	m_pTitle->Render();
}
