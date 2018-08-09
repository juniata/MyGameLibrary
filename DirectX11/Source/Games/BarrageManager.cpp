#include "DxLibrary\DX_Library.h"
#include "BaseBarrage.h"
#include "BarrageManager.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	引数付きコンストラクタ
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
//	デストラクタ
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
//	弾幕クラスを追加する
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
//	更新
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
//	描画
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