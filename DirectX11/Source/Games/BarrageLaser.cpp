#include "DxLibrary\DX_Library.h"
#include "BarrageLaser.h"
using namespace DirectX;

//------------------------------------------------------------------------------
//
//  @brief		引数付きコンストラクタ
//
//------------------------------------------------------------------------------
BarrageLaser::BarrageLaser(const char* pFilepath, const size_t instanceNum, const XMFLOAT2& renderSize) : 
	BaseBarrage(pFilepath, instanceNum, renderSize),
	m_interval(5),
	m_index(0)
{
	for (size_t i = 0; i < instanceNum; ++i)
	{
		// 描画されないようにする
		m_pInstance2D->Disable(i);
		m_pPotList[i].x = DX_System::GetInstance()->GetWindowWidth() * 0.5f;
		m_pPotList[i].y = DX_System::GetInstance()->GetWindowHeight() - renderSize.y;
	}
}

//------------------------------------------------------------------------------
//
//  @brief		更新
//
//------------------------------------------------------------------------------
void BarrageLaser::Update()
{
	
	if (--m_interval == 0)
	{
		m_interval = 5;

		if (m_pPotList[m_index].z >= 1.0f)
		{
		//	m_pPotList[m_index].z = 0.0f;
		}

		if (++m_index >= m_instanecNum) {
			m_index = 0;
		}
	}

	for (size_t i = 0; i < m_instanecNum; ++i)
	{
		// 非描画のものは無視
		if (m_pPotList[i].z >= 1.0f) {
			continue;
		}

		m_pPotList[i].y -= 3.5f;
		if (m_pPotList[i].y < 0.0f) {
			m_pPotList[i].x = 0.0f;
			m_pPotList[i].y = DX_System::GetInstance()->GetWindowHeight();
			m_pPotList[i].z = 1.1f;
		}
	}
}

//------------------------------------------------------------------------------
//
//  @brief		描画
//
//------------------------------------------------------------------------------
void BarrageLaser::Render()
{
	m_pInstance2D->Render();
}