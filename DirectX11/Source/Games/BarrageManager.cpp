#include "DxLibrary\DX_Library.h"
#include "BaseBarrage.h"
#include "BarrageManager.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	�����t���R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
BarrageManager::BarrageManager(const size_t barrageNum) : 
	m_barrageNum(barrageNum)
{
	m_pBarrageList = new BaseBarrage*[barrageNum];
	for (size_t i = 0; i < m_barrageNum; ++i)
	{
		m_pBarrageList[i] = nullptr;
	}
}


//-----------------------------------------------------------------------------------------
//
//	�f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
BarrageManager::~BarrageManager()
{
	for (size_t i = 0; i < m_barrageNum; ++i)
	{
		DELETE_OBJ(m_pBarrageList[i]);
	}
	DELETE_OBJ_ARRAY(m_pBarrageList);
}

//-----------------------------------------------------------------------------------------
//
//	�e���N���X��ǉ�����
//
//-----------------------------------------------------------------------------------------
void BarrageManager::AddBarrage(BaseBarrage* pCreateBarrage)
{
	for (size_t i = 0; i < m_barrageNum; ++i)
	{
		if (m_pBarrageList[i] == nullptr)
		{
			m_pBarrageList[i] = pCreateBarrage;
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	�X�V
//
//-----------------------------------------------------------------------------------------
void BarrageManager::Update()
{
	for (size_t i = 0; i < m_barrageNum; ++i)
	{
		if (m_pBarrageList[i])
		{
			m_pBarrageList[i]->Update();
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	�`��
//
//-----------------------------------------------------------------------------------------
void BarrageManager::Render()
{
	for (size_t i = 0; i < m_barrageNum; ++i)
	{
		if (m_pBarrageList[i] == nullptr)
		{
			m_pBarrageList[i]->Render();
		}
	}
}