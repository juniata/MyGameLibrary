#include	"DxLibrary/DX_Library.h"
#include	"MapChip.h"
#include	"Games/GameDefine.h"

MapChip::MapChip() : 
	m_pObjectList(nullptr),
	m_instanceNum(0)
{}

MapChip::~MapChip()
{
	DELETE_OBJ(m_pObjectList);
}
bool MapChip::Initialize(const char* pFilepath, const unsigned int instanceNum)
{
	m_pObjectList = new DX_Instance2DObject();
	m_instanceNum = instanceNum;

	if (m_pObjectList->Initialize(pFilepath, instanceNum, DirectX::XMFLOAT2(RENDER_SIZE_X, RENDER_SIZE_Y), DirectX::XMFLOAT2(64.0f, 64.0f))) {
		return true;
	}

	return false;
}
DX_Instance2DObject* MapChip::GetInstance2DObject()
{
	return m_pObjectList;
}
bool MapChip::Render()
{
	return m_pObjectList->Render();
}