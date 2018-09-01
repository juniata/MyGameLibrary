#include "DxLibrary\DX_Library.h"
#include "BarrageBeam.h"
using namespace DirectX;

//------------------------------------------------------------------------------
//
//  @brief		引数付きコンストラクタ
//
//------------------------------------------------------------------------------
BarrageBeam::BarrageBeam(const char* pFilepath, const UINT instanceNum, const XMFLOAT2& renderSize) :
	BaseBarrage(pFilepath, instanceNum, renderSize)
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
bool BarrageBeam::DoUpdate()
{
	return true;
}

//------------------------------------------------------------------------------
//
//  @brief		描画
//
//------------------------------------------------------------------------------
bool BarrageBeam::DoRender()
{
	return m_pInstance2D->Render();
}