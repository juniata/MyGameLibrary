#include	"DX_Library.h"
#include	"Games\SceneMain.h"

//-----------------------------------------------------------------------------------------
//
//	staticƒƒ“ƒo•Ï”
//
//-----------------------------------------------------------------------------------------
DX_SceneManager*	DX_SceneManager::m_pInstance = nullptr;

DX_SceneManager::DX_SceneManager() :
	m_pCurScene(nullptr),
	m_pNextScene(nullptr)
{
}
//-----------------------------------------------------------------------------------------
//
//	ƒƒ“ƒo[•Ï”‚ð‰Šú‰»‚·‚é
//
//-----------------------------------------------------------------------------------------
void DX_SceneManager::Initialize(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCurScene = new SceneMain();
	m_pCurScene->Initialize();
}
DX_SceneManager::~DX_SceneManager()
{
	DELETE_OBJ(m_pCurScene);
	DELETE_OBJ(m_pNextScene);
}
DX_SceneManager* DX_SceneManager::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_SceneManager();
	}

	return m_pInstance;
}


void DX_SceneManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

void DX_SceneManager::Update(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCurScene->Update(pSystem, pDevice, pContext);
}
void DX_SceneManager::Render(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCurScene->Render(pSystem, pDevice, pContext);
}