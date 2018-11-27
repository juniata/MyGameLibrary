#include	"DxLibrary/DX_Library.h"
#include	"MapChip.h"

MapChip::MapChip() : 
	m_pObjectList(nullptr),
	m_pInstance2DList(nullptr),
	m_instanceNum(0)
{}

MapChip::~MapChip()
{
	DELETE_OBJ(m_pObjectList);
	// m_pPosListはDX_Instnace2DObjectのデストラクタで開放されるから、こっちで開放しなくても構わない。
}

void MapChip::SetPos(const unsigned index, const float y, const float x)
{
	if (m_instanceNum > index)
	{
		m_pInstance2DList[index].x = x;
		m_pInstance2DList[index].y = y;
	}
	else {
		// TODO: releaseでも出力されてしまう。色つけて出力できるようにしたい。
		OutputDebugString("index error");
	}
}
DirectX::XMFLOAT2 MapChip::GetPos(const unsigned index)const
{
	if (m_instanceNum > index)
	{
		return DirectX::XMFLOAT2(m_pInstance2DList[index].x, m_pInstance2DList[index].y);
	}
	else {
		// TODO: releaseでも出力されてしまう。色つけて出力できるようにしたい。
		OutputDebugString("index error");
	}

	return DirectX::XMFLOAT2(0.0f, 0.0f);
}
bool MapChip::Initialize(const char* pFilepath, const unsigned int instanceNum)
{
	m_pObjectList = new DX_Instance2DObject();
	m_instanceNum = instanceNum;

	if (m_pObjectList->Initialize(pFilepath, instanceNum, DirectX::XMFLOAT2(32.0f, 32.0f))) {
		m_pInstance2DList = m_pObjectList->GetInstanceList();
		return true;
	}

	return false;
}

void MapChip::Disable(const unsigned index)
{
	if (m_instanceNum > index)
	{
		m_pInstance2DList[index].Disable();
	}
	else {
		// TODO: releaseでも出力されてしまう。色つけて出力できるようにしたい。
		OutputDebugString("index error");
	}
}
void MapChip::Enable(const unsigned index)
{
	if (m_instanceNum > index)
	{
		m_pInstance2DList[index].Enable();
	}
	else {
		// TODO: releaseでも出力されてしまう。色つけて出力できるようにしたい。
		OutputDebugString("index error");
	}
}

bool MapChip::Render()
{
	return m_pObjectList->Render();
}