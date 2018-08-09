#include "DxLibrary\DX_Library.h"
#include "BarrageLaser.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	引数付きコンストラクタ
//
//-----------------------------------------------------------------------------------------
BaseBarrage::BaseBarrage(const char* pFilepath, const size_t instanceNum, const XMFLOAT2& renderSize) :
	m_pInstance2D(new DX_Instance2DObject(pFilepath, instanceNum, renderSize)),
	m_instanecNum(instanceNum)
{
	m_pPotList = m_pInstance2D->GetPosList();
}

//-----------------------------------------------------------------------------------------
//
//	デフォルトコンストラクタ
//
//-----------------------------------------------------------------------------------------
BaseBarrage::~BaseBarrage()
{
	DELETE_OBJ(m_pInstance2D);
}

//-----------------------------------------------------------------------------------------
//
//	描画
//
//-----------------------------------------------------------------------------------------
void BaseBarrage::Render()
{
	m_pInstance2D->Render();
}