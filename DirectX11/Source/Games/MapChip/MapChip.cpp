#include	"DxLibrary/DX_Library.h"
#include	"MapChip.h"

MapChip::MapChip() : 
	m_pObjectList(nullptr),
	m_pPosList(nullptr),
	m_instanceNum(0)
{}

MapChip::~MapChip()
{
	DELETE_OBJ(m_pObjectList);
	// m_pPosList��DX_Instnace2DObject�̃f�X�g���N�^�ŊJ������邩��A�������ŊJ�����Ȃ��Ă��\��Ȃ��B
}

void MapChip::SetPos(const unsigned index, const unsigned int y, const unsigned int x)
{
	if (m_instanceNum > index)
	{
		DirectX::XMFLOAT3* temp = &m_pPosList[index];
		temp->x = CAST_F(x) * 32.0f;
		temp->y = CAST_F(y) * 32.0f;
	}
	else {
		// TODO: release�ł��o�͂���Ă��܂��B�F���ďo�͂ł���悤�ɂ������B
		OutputDebugString("index error");
	}
}
DirectX::XMFLOAT2 MapChip::GetPos(const unsigned index)const
{
	if (m_instanceNum > index)
	{
		return DirectX::XMFLOAT2(m_pPosList[index].x, m_pPosList[index].y);
	}
	else {
		// TODO: release�ł��o�͂���Ă��܂��B�F���ďo�͂ł���悤�ɂ������B
		OutputDebugString("index error");
	}

	return DirectX::XMFLOAT2(0.0f, 0.0f);
}
bool MapChip::Initialize(const char* pFilepath, const unsigned int instanceNum)
{
	m_pObjectList = new DX_Instance2DObject();
	m_instanceNum = instanceNum;

	if (m_pObjectList->Initialize(pFilepath, instanceNum, DirectX::XMFLOAT2(32.0f, 32.0f))) {
		m_pPosList = m_pObjectList->GetPosList();
		return true;
	}

	return false;
}

void MapChip::Disable(const unsigned index)
{
	if (m_instanceNum > index)
	{
		m_pObjectList->Disable(index);
	}
	else {
		// TODO: release�ł��o�͂���Ă��܂��B�F���ďo�͂ł���悤�ɂ������B
		OutputDebugString("index error");
	}
}
void MapChip::Enable(const unsigned index)
{
	if (m_instanceNum > index)
	{
		m_pObjectList->Enable(index);
	}
	else {
		// TODO: release�ł��o�͂���Ă��܂��B�F���ďo�͂ł���悤�ɂ������B
		OutputDebugString("index error");
	}
}

bool MapChip::Render()
{
	return m_pObjectList->Render();
}