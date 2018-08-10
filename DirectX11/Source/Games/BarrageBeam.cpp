#include "DxLibrary\DX_Library.h"
#include "BarrageBeam.h"
using namespace DirectX;

//------------------------------------------------------------------------------
//
//  @brief		�����t���R���X�g���N�^
//
//------------------------------------------------------------------------------
BarrageBeam::BarrageBeam(const char* pFilepath, const size_t instanceNum, const XMFLOAT2& renderSize) :
	BaseBarrage(pFilepath, instanceNum, renderSize)
{
	for (size_t i = 0; i < instanceNum; ++i)
	{
		// �`�悳��Ȃ��悤�ɂ���
		m_pInstance2D->Disable(i);
		m_pPotList[i].x = DX_System::GetInstance()->GetWindowWidth() * 0.5f;
		m_pPotList[i].y = DX_System::GetInstance()->GetWindowHeight() - renderSize.y;
	}
}

//------------------------------------------------------------------------------
//
//  @brief		�X�V
//
//------------------------------------------------------------------------------
void BarrageBeam::DoUpdate()
{
	
}

//------------------------------------------------------------------------------
//
//  @brief		�`��
//
//------------------------------------------------------------------------------
void BarrageBeam::DoRender()
{
	m_pInstance2D->Render();
}